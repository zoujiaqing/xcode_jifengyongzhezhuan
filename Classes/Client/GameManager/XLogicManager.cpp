// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "ResourcesUpdateManager.h"
#include "XLogicManager.h"
#include "GameManager.h"
#include "ItemManager.h"
#include "HelloWorldScene.h"
#include "CCDirector.h"
#include "UserData.h"
#include "EnginePlayerManager.h" 
#include "MapDataManager.h"
#include "MainMenuLayer.h"
#include "Localization.h"
#include "SpriteElfDataCenter.h"
#include "StoryInstanceDirector.h"
#include "ElfExploreManager.h"
#include "CameraController.h"
#include "GemStonesFromLua.h"
#include "CharacterSelectLayer.h"
#include "UpdateResourceLayer.h"
#include "UpdateInfoFromServer.h"
#include "AppDelegate.h"
#include "UserLoginStatistics.h"
#include "SkillDispMgr.h"
#include "LoginManager.h"
#include "PlayerRoleManager.h"
#include "MonthCardManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include "TmLoginManager.h"

//#include <android/log.h> 
#endif

#include "LevelManager.h"
#include "ItemManager.h"
#include "InstanceManager.h"
#include "GameConfigFromLuaManager.h"
#include "GameDataManager.h"

#include "TXGUIHeader.h"
#include "GMessage.h"
#include "NetStateChecker.h"
#include "InstanceListLayer.h"

#include "HeroRoleManager.h"
#include "BoneNPCManager.h"
#include "TaskManager.h"
#include "SkeletonAnimRcsManager.h"
#include "TaskConfigFromLuaManager.h"
#include "SpriteElfConfigFromLua.h"
#include "SpriteElfManager.h"

#include "MessageBoxLayer.h"
#include "SpriteFactory.h"
#include "MainLandManager.h"
#include "CCLuaEngine.h"
#include "StoryDataCenter.h"
#include "TimeManager.h"
#include "SkillUIManager.h"
#include "LuaTinkerManager.h"
#include "ItemUpdateManager.h"
#include "DressUpManager.h"
#include "TutorialsManager.h"
#include "PvAIManager.h"
#include "GuildInstanceManager.h"
#include "GameAudioManager.h"
#include "PopupRewardManager.h"
#include "ParticleManager.h"
#include "AccountsLayer.h"
#include "MessageFilter.h"
#include "LoginManager.h"
#include "NoticeSceneLayer.h"
#include "UIControlsApearControler.h"
#include "WaitingLayerManager.h"
#include "SystemConditionManager.h"
#include "SkeletonAnimRcsManager.h"

using namespace TXGUI;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NcSDKManager.h"
#include "GlobalFunction.h"
#endif

// -----------------------------------------------------------------------------------------

void WaittingForRsp::Start()
{
	m_bStart = true;
	m_cmdList.clear();
}

void WaittingForRsp::Stop()
{
	m_bStart = false;
	m_cmdList.clear();
}

void WaittingForRsp::OnHandleMessage( const std::string& cmd )
{
	if(m_bStart)
		m_cmdList.push_back(cmd);
}

bool WaittingForRsp::IsReceived( char* pCmdName )
{
	return std::find(m_cmdList.begin(), m_cmdList.end(), std::string(pCmdName)) != m_cmdList.end();
}



// -----------------------------------------------------------------------------------------
static XLogicManager *g_sLogicMgr = NULL;

#ifndef FPS
#define FPS  (1/24.0f)

#endif



extern long long millisecondNow();

XLogicManager::XLogicManager()
	: m_pCurLayer(NULL)
	, m_bShowPPVEPanel(false)
    , isIOSDisconnect(false)
    , isIOSInScene(false)
    , isChangeScene(false)
    , isIOSNeedWaitingLayer(false)
    , accountName("")
    , hasStarReward(false)
{
	// Ë¶ÅËá™Â∑±ÈáäÊîæËá™Â∑±„�?
	this->retain();

	GameManager::Create();
	GameManager::Get()->SetLogicManager(this);

	// ÂàùÂßãÂåñÁâ©ÂìÅÁÆ°ÁêÜÁ±ª
	GameConfigFromLuaManager::Create();

	ItemManager::Create();
	ItemManager::Get()->init();

	InstanceManager::Create();

	GameDataManager::Create();

	TimeManager::Create();

	LuaTinkerManager::Create();

	ItemUpdateManager::Create();

	PvAIManager::Create();

	GuildInstanceManager::Create();

	/// add more!
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
	m_autoLogin = true ;
	m_eCurLayer = E_NOE;
	m_eCurStatus = E_WAITIN_FOR_COMMOND;
	m_loginStep = 0;
	mErrorCode = dberr_succ;
	isGameStart = false;
	isRequiredItemsInfo = false;
	isInitLoginData = false;
	m_loginDataTime = 0;
	b_isLoginGame = false;
	CCScheduler * pScheduler = CCDirector::sharedDirector()->getScheduler();
	//pScheduler->scheduleSelector(schedule_selector(XLogicManager::updateFps), this, FPS, false);

	//m_LastEnterBackgroundTime = millisecondNow();
	
	HeroRoleManager::Create();
	
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(XLogicManager::getConfigInfo), this, 0.1f, false);
}

XLogicManager::~XLogicManager()
{
	// TODOÔºöËøôÈáåÁöÑ‰ª£Á†ÅÈÉΩË¶ÅÁßªÂä®Âà∞exitGameÔºàÔºâ
	HeroRoleManager::Destroy();

	OnlineNetworkManager::sShareInstance()->UnRegisterEvent(this);
    
	//CCScheduler * pScheduler = CCDirector::sharedDirector()->getScheduler();
	//pScheduler->unscheduleSelector(schedule_selector(XLogicManager::updateFps), this);

	////////// ÊûêÊûÑÊ∏∏ÊàèÁÆ°ÁêÜÂô?
	/// destroy gameconfigfromluaManager
	GameConfigFromLuaManager::Destroy();
	/// destroy ItemManager
	ItemManager::Destroy();
	LoginManager::Destroy();
	
	WaitingLayerManager::Destroy();
	InstanceManager::Destroy();

	GameDataManager::Destroy();

	TimeManager::Destroy();
	/// add more!

	GameManager::Destroy();

	SkeletonAnimRcsManager::Destroy();
	BoneNPCManager::Destroy();
	TaskManager::Destroy();
	TaskConfigFromLuaManager::Destroy();
	SpriteElfConfigFromLuaManager::Destroy();
	SpriteElfManager::Destroy();
	SpriteElfDataCenter::Destroy();
	SpriteFactory::Destroy();

	StoryDataCenter::Destroy();
	SkillUIManager::Destroy();

	ElfExploreManger::Destroy();

	LuaTinkerManager::Destroy();

	ItemUpdateManager::Destroy();
	GemStonesFromLua::Destroy();
	DressUpManager::Destroy();
	TutorialsManager::Destroy();

	PvAIManager::Destroy();
	GuildInstanceManager::Destroy();
	//jackniu
	//PopupRewardManager::Destroy();
	NetWorkConfigInfoFromServer::puresharedInstance();
	ResourceUpdateManager::pureInstance();
	
	SystemConditionManager::Destroy();
	UIControlAppearControler::Destroy();
	NotificationCenter::Destroy() ;
}
void XLogicManager::getConfigInfo(float f)
{
	
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(XLogicManager::getConfigInfo), this);
		 NetWorkConfigInfoFromServer::sharedInstance()->downloadConfigFileList();
		
		NetWorkConfigInfoFromServer::sharedInstance()->parseRcvMessageFromServer();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		XLogicManager::sharedManager()->sendStatAtLaunch();
#endif
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			XLogicManager::sharedManager()->goToLayer(XLogicManager::E_UPDATE_RESOURCE, NULL);
#else
			XLogicManager::sharedManager()->goToLayer(XLogicManager::E_SELECT_ZONE, NULL);
#endif	
}

//void XLogicManager::updateFps(float dt)
//{ 
//	// TODO : ‚àè∆í‚â•‚Ä¶≈í‚Äú‚àö¬´‚àë¬¢√Ä√ï¬´√é¬´√�?¬∞‚àë‚àëÀõ≈í√í‚àÜÀú¬™√í¬µ‚àö‚Äì‚âà≈ì¬¢-¬∞‚àë¬µ¬ª¬•ÀùŒ©¬∑œÄÀö-¬∞‚àë¬∫√É‚Äì¬Ø¬∫‚Äù‚Äò√ø¬£¬®SceneLayer : StepLoader
//	if(isInitLoginData)
//	{
//		m_loginDataTime += dt;
//		if(m_loginDataTime > 3.0f)
//		{
//			initUserInfoFromSever(dt);
//		}
//	}
//
//	/// update appliaction background flag
////	long long fCurrentTime = millisecondNow();
////	if (fCurrentTime - m_LastEnterBackgroundTime > 30 * 1000)
////	{
////		/// application has stayed in background for more than 30 seconds
////#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
////		OnlineNetworkManager::sShareInstance()->cleanServerConnect(false);
////#endif
////	}
////
////	m_LastEnterBackgroundTime = fCurrentTime;
//}

/////////////////////////////////////////////////////////////
/// application will resume from background
void XLogicManager::WillEnterForeground()
{
    // TODO : 
    if(isGameStart)
    {
        /// send ping to server , used to check network whether is OK
        /// OnlineNetworkManager::sShareInstance()->sendReqNetStateMessage();
    }
	int tag1 = MainMenuLayer::getLayerTagByName("ForcePKAnimLayer");
	MainMenuLayer::hideUILayerByTag(tag1);
	/// send global application resume message
	NotificationCenter::defaultCenter()->broadcast(GM_APPLICATION_RESUME, this);
}

void XLogicManager::DidEnterBackground()
{
	/// send global application resume message
	NotificationCenter::defaultCenter()->broadcast(GM_APPLICATION_ENTER_BACKGROUND, this);
	CCLOG("XLogicManager::DidEnterBackground()");
	//m_LastEnterBackgroundTime = millisecondNow();
}


/*
* @prototype: goToFrontend()
* @note:
*     bring the game to the front of the screen. prepare the resources and init all the game components.
* @param: void
* @return: void
*/

bool XLogicManager::isAlreadyLogin()
{
	return b_isLoginGame;
}

void XLogicManager::loginSuccess()
{
	//const char* ip = UserData::GetLastIP();
	const char* name = UserData::GetLastServerName();
	//unsigned int port = UserData::GetLastPort();
	USERINFO info = UserData::GetUserInfo();
	unsigned int serverId = info.serverId;
	unsigned int regTime = info.id.getRegTime();
	//CCUserDefault::sharedUserDefault()->setIntegerForKey("lastPort",port);
	//CCUserDefault::sharedUserDefault()->setStringForKey("lastIp",ip);
	CCUserDefault::sharedUserDefault()->setStringForKey("lastServerName",name);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("serverId",serverId);
	
	CCUserDefault::sharedUserDefault()->flush();

    UserLoginStatistics::sendStatInfoToServer("login_success");
	sendUserDeviceInfo();
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	{
		
		LoginDeveloperInfo pInfo;
		char temp[500];
	    
        snprintf(temp, sizeof(temp), "%u", info.id.getID());
        pInfo["roleId"] =  string(temp);
		pInfo["roleName"] =  string(info.szName);
  		snprintf(temp, sizeof(temp), "%u", info.level);
        pInfo["roleLevel"] = string(temp);
        snprintf(temp, sizeof(temp), "%u", serverId);
        pInfo["zoneId"] = string(temp);
        pInfo["zoneName"] = string(info.lastServerName);
		TmLoginManager::tmLoginManager()->doSendMessage(pInfo);
		{//statistic
		
			const char *pchannel = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/svn_version.lua", "getCocoId");
			
			//const char *pchannel = LuaTinkerManager::Get()->getLuaConfig<const char *>("channel", "channel", channel, "coco_number");

		   	CCLOG("%s %d coco channel id:%s", __FUNCTION__, __LINE__,pchannel);
	       	JniMethodInfo t;
	       	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxActivity", 
		   		"sendStatistic", "(Ljava/lang/String;IIILjava/lang/String;)V")) {
			   	CCLOG("%s %d", __FUNCTION__, __LINE__);
				jstring jname = t.env->NewStringUTF(info.szName);
				jstring jchannel = t.env->NewStringUTF(pchannel);
				
	            t.env->CallStaticVoidMethod(t.classID, t.methodID, 
					jchannel, serverId,info.id.getID(),regTime, jname);
				t.env->DeleteLocalRef(jname);
				t.env->DeleteLocalRef(jchannel);
	            t.env->DeleteLocalRef(t.classID);
	        }
    	}
	}
#endif
}

void XLogicManager::sendUserDeviceInfo()
{
	OnlineNetworkManager::sShareInstance()->sendUserDeviceInfoToServer(
		LoginManager::Get()->getDeviceId().c_str(),
		LoginManager::Get()->getDeviceOS().c_str(),
		LoginManager::Get()->getIpAddress().c_str(),
		LoginManager::Get()->getMacAddress().c_str(),
		LoginManager::Get()->getDeviceType().c_str(),
		AppDelegate::GetShareAppDelegate()->GetSvnVersion());
}

/*
* @prototype: startGame()
* @note:
*     start the game now, bring the player to the hometown.
* @param: void
* @return: void
*/
void XLogicManager::startGame()
{
	if(!isGameStart)
	{
	
		//unschedule(schedule_selector(XLogicManager::updateFps));
		/*this->retain();
		this->removeFromParent();
		pGameScenelayer->addChild(this);
		this->autorelease();*/
		
		//schedule(schedule_selector(XLogicManager::initUserInfoFromSever),1.0f);
		m_loginDataTime = 0;
        
		isInitLoginData = true;
		CCScheduler * pScheduler = CCDirector::sharedDirector()->getScheduler();
		//pScheduler->scheduleSelector(schedule_selector(XLogicManager::updateFps), this, FPS,false);
		
        //CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(XLogicManager::updateChangeScene), this);
        //CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(XLogicManager::updateChangeScene), this, 1.0f, false);
        

		isGameStart = true;
		isRequiredItemsInfo = true;
		
		/// now start say hello to server! keep one eye on net connection
		NetStateChecker::Get()->Start();

      
        /// register ISocketEvent to online network manager
        OnlineNetworkManager::sShareInstance()->RegisterEvent(this);

		// Note: ÂºÄÂßãËøõÂÖ•Ê∏∏Ê�?ÊàñËÄ?ÁΩëÁªúÊñ≠ÂºÄÈáçÊñ∞ËøõÂÖ•Ê∏∏Êà�?
		// 
		SpriteFactory::sharedFactory()->ResetValue();
		SpriteElfManager::Get()->ClearAllData();
		SpriteElfDataCenter::Get()->ResetValue();
		SpriteElfConfigFromLuaManager::getInstance()->ResetValue();
		SpriteElfConfigFromLuaManager::getInstance()->ClearElfLayerAllData();
		StoryInstanceDirector::Get()->ResetAllData();
		ElfExploreManger::Get()->ResetValue();

		CameraController::sharedCameraController()->ResetValue();
		MainMenuLayer::ClearLayersInQueneData();

		TaskManager::getInstance()->ResetValue();
		BoneNPCManager::getInstance()->ResetValue();

		InstanceManager::Get()->resetData();
		ItemUpdateManager::Get()->resetData();
		TimeManager::Get()->ResetData();
		GemStonesFromLua::getInstance();
		PvAIManager::Get()->resetData();
		GuildInstanceManager::Get()->resetData();
		TutorialsManager::Get()->ResetValue();
		MessageFilter::Get()->resetValue();
		
		//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Marquee/MarqueeDataCenter.lua","initMarqueeData");
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/SpriteElf/SpriteElfDataCenter.lua","InitSpriteElfDataCenter");
		

		SkillUIManager::getInstance()->ResetValue();
		GameConfigFromLuaManager::Get()->InitSkillUIDataCenter();
		SkillDispMgr::Get()->ClearUserSkillData();
		MainLandManager::Get()->resetData();
	}
}

void XLogicManager::exitGame()
{
	//ÂæàÂ§öÊï∞ÊçÆÂøÖÈ°ªÊòØËøô‰∏™Ê∏∏ÊàèÂ∑•Á®ã‰∏≠ÈÉΩË¶ÅÂ≠òÂú®ÁöÑ„ÄÇÊØîÂ¶ÇÂä®Á�?
	//g_sLogicMgr->release();
	//g_sLogicMgr = NULL;
}
void XLogicManager::pureXLogicManager()
{
	 if(g_sLogicMgr)
	 {
		delete g_sLogicMgr ;
		g_sLogicMgr = NULL ;
	 }

}
//static instance
XLogicManager* XLogicManager::sharedManager()
{
	if (g_sLogicMgr == NULL)
	{
		g_sLogicMgr = new XLogicManager();
		//g_sLogicMgr->autorelease();
	}
	return g_sLogicMgr;
}

// functions
// go to a specific scene
void  XLogicManager::goToLayer(GAME_LOGIC_LAYER sceneTag, void *userData )
{
	//clear ParticleManager buff when change layer
	ParticleManager::Get()->clearEffectBuff();		
	TutorialsManager::Get()->ResetTutorialInstance();

	switch(sceneTag)
	{
		case E_UPDATE_RESOURCE:
			{
                CCLOG("download step 1");
				UserLoginStatistics::sendStatInfoToServer("before_resource_update");
				CCLOG("download step 2");

				m_eCurLayer = E_UPDATE_RESOURCE;
				CCScene *pScene = UpdateRcsScene::create();
				if(CCDirector::sharedDirector()->getRunningScene() != NULL)
					CCDirector::sharedDirector()->replaceScene(pScene);
				else
					CCDirector::sharedDirector()->runWithScene(pScene);

				//ResourceUpdateManager::sharedInstance()->FirstCheckResourceVersionAndOperator();
				UpdateResourceLayer* pLayer = UpdateResourceLayer::create();
				pScene->addChild(pLayer, 1,E_UPDATE_RESOURCE);				
			}
			break;
            
        case E_Notice:
            {
                CCScene *pScene = CCScene::create();
                AspriteManager::getInstance()->OnlyLoadNoticeAsprite();
                NoticeSceneLayer* pNoticeParent = NoticeSceneLayer::create();
                pScene->addChild(pNoticeParent);
                
                if(CCDirector::sharedDirector()->getRunningScene() != NULL)
					CCDirector::sharedDirector()->replaceScene(pScene);
				else
					CCDirector::sharedDirector()->runWithScene(pScene);
                
                //goToLayer(XLogicManager::E_SELECT_ZONE,NULL);
            }
            
            break;
            
		case E_SELECT_ZONE:
			{
				UserLoginStatistics::sendStatInfoToServer("before_resource_load");
				AppDelegate *appDelegate = AppDelegate::GetShareAppDelegate();
				appDelegate->InitOnGameStart();

				if(m_pCurLayer)
				{
					CCDirector::sharedDirector()->getRunningScene()->removeChild(m_pCurLayer);
					m_pCurLayer = NULL;
				}

				CCScene *pScene = LoginScene::create();
				if(CCDirector::sharedDirector()->getRunningScene() != NULL)
					CCDirector::sharedDirector()->replaceScene(pScene);
				else
					CCDirector::sharedDirector()->runWithScene(pScene);

				//create a scene. it's an autorelease object
				HelloWorld *pLayer = HelloWorld::create();
				CC_BREAK_IF(! pLayer);
                
				//add layer as a child to scene
				pScene->addChild(pLayer, 0, E_SELECT_ZONE);
                
				m_pCurLayer = pLayer;
				m_eCurLayer = E_SELECT_ZONE;
			}
			break;
		case E_CREATE_PLAYER_LAYER:
			// go to create player layer
			m_eCurLayer = E_CREATE_PLAYER_LAYER;
			m_pCurLayer = NULL;
			break;

		case E_SELECT_CHARACTER:
			{
				m_pCurLayer = NULL;
				m_eCurLayer = E_SELECT_CHARACTER;
			}
			
			break;
       	case E_FIRST_FIGHT:
			{
				//NotificationCenter::defaultCenter()->broadcast(GM_E_ENTER_FB, this);
				
				//USERINFO mUserData = UserData::GetUserInfo();
		
				// TODO : 
  
				// first login ,player enter map
				//m_eCurStatus = E_WAITIN_FOR_COMMOND;

				//FB_DATA* pData = (FB_DATA*)userData;
				//goToFb(pData->mapID, pData->battleID);
				
			   // USERINFO mUserData = UserData::GetUserInfo();
               
			
               // InstanceManager::Get()->setCurInstanceId(280000);
                   
                prepareBattle(280000,0);
			}
			break; 
		case E_ENTER_MAINLAND:
			{
				NotificationCenter::defaultCenter()->broadcast(GM_E_ENTER_MAINLAND, this);
				
				USERINFO mUserData = UserData::GetUserInfo();
				mUserData.ResetRagePoint();
				UserData::SetUserInfo(mUserData);

				// TODO : 
				CCDirector *pDirector = CCDirector::sharedDirector();
				
				unsigned int mapId = *(unsigned int*)userData;
				if(!isAlreadyLogin())
				{
					if(m_pCurLayer)
					{
						pDirector->getRunningScene()->removeChild(m_pCurLayer);
						m_pCurLayer = NULL;
					}

					// first login ,player enter map
					m_eCurStatus = E_WAITIN_FOR_COMMOND;
					b_isLoginGame = true;
					CCScene *pScene = GameScene::create();
					if(CCDirector::sharedDirector()->getRunningScene() != NULL)
						CCDirector::sharedDirector()->replaceScene(pScene);
					else
						CCDirector::sharedDirector()->runWithScene(pScene);
					MainLandLayer* pLayer = MainLandLayer::create();	
					pScene->addChild(pLayer, 0, E_ENTER_MAINLAND);
					m_pCurLayer = pLayer;

					pLayer->enterMyRoom();
					if(pDirector->getRunningScene()->getChildByTag(E_XLOGICMANAGER) == NULL)
						pDirector->getRunningScene()->addChild(this, 0, E_XLOGICMANAGER);
					startGame();
					MainLandManager::Get()->setCurCityId(mapId);
				}
				else
				{
					m_eCurStatus = E_WAITIN_FOR_COMMOND;
					if(m_pCurLayer)
					{
						pDirector->getRunningScene()->removeChild(m_pCurLayer);
						m_pCurLayer = NULL;
					}
					MainLandLayer* pLayer = MainLandLayer::create();	
					pDirector->getRunningScene()->addChild(pLayer, 0, E_ENTER_MAINLAND);
					
					m_pCurLayer = pLayer;
					pLayer->enterMyRoom();
					changeMap(mapId, KMapFromTransition);
					/// to do , show ppve hall if win in ppve battle.
					checkNeedShowPPVEPanel();
				}
			}
			break;

		case E_ENTER_FB:
			{
				//SkeletonAnimRcsManager::getInstance()->FirstLoadOnStartFBLayer();
				NotificationCenter::defaultCenter()->broadcast(GM_E_ENTER_FB, this);

				if(m_pCurLayer)
				{
					CCDirector::sharedDirector()->getRunningScene()->removeChild(m_pCurLayer);
					m_pCurLayer = NULL;
				}

				FB_DATA* pData = (FB_DATA*)userData;
				goToFb(pData->mapID, pData->battleID);
				m_eCurLayer = E_ENTER_FB;
			}
			break;
		case E_MAIN_GAME_LAYER:
			m_eCurLayer = E_MAIN_GAME_LAYER;
			break;
		
		default:
			CCLOGWARN("invalid paramerter: %d", sceneTag);
			break;
	}	
}

void XLogicManager::setStartConectint()
{
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(scene)
	{
		CCNode* layer = scene->getChildByTag(E_SELECT_ZONE);
		HelloWorld* helloLayer = dynamic_cast<HelloWorld*>(layer);
		if(helloLayer)
		{
			helloLayer->setNeedCheckNetWork(false);
		}
	}
}

void XLogicManager::connectToServer()
{
	CCLOG("XLogicManager::connectToServer()");

	b_isLoginGame = false;
	//const char* serverIP = NetWorkConfigInfoFromServer::sharedInstance()->GetGetWayIP();
	//int serverPort = NetWorkConfigInfoFromServer::sharedInstance()->GetGetWayPort();
	//OnlineNetworkManager::sShareInstance()->setServerIP(serverIP);
	//OnlineNetworkManager::sShareInstance()->setServerPort(serverPort);
	OnlineNetworkManager::sShareInstance()->startConnect();
    setStartConectint();
    
}

void XLogicManager::backToLogin()
{
    m_loginStep = 0;
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(scene)
	{
		CCNode* layer = scene->getChildByTag(E_SELECT_ZONE);
		HelloWorld* helloLayer = dynamic_cast<HelloWorld*>(layer);
		if(helloLayer)
		{
			helloLayer->backToLogin();
		}
	}
	LoginManager::Get()->doLoginOut();
}

void XLogicManager::setLoginStatus(int step)
{
	m_loginStep = step;
}

void XLogicManager::startXXX()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(XLogicManager::updateChangeScene), this, 0.1f, false);
}

void XLogicManager::updateChangeScene(float f)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(XLogicManager::updateChangeScene), this);
    networkDisConnected();
	NetStateChecker::Get()->Stop();
   
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_loginStep = 2;
    LoginManager::Get()->doLoginOut();
#endif
}

void XLogicManager::setHasStarReward(bool flag)
{
    hasStarReward = flag;
}

int XLogicManager::getLoginStep()
{
	return m_loginStep;
}

void XLogicManager::onConnectToSever()
{
	m_loginStep = 3;
}

void XLogicManager::logOut()
{
	OnlineNetworkManager::sShareInstance()->sendLogoutMessage();
	playerlogOut();
}

void XLogicManager::logIn()
{
	CCLOG("XLogicManager::logIn()");
	/*if(m_eCurStatus == E_SENDING_SEVER)
	{
		return;
	}

	m_eCurStatus = E_SENDING_SEVER;*/
	b_isLoginGame = false;
	m_loginStep = 0 ;
	OnlineNetworkManager::sShareInstance()->startConnect();
}

void XLogicManager::logInError(db_errcode_t error)
{
	//(*pNetErrFun)(error);
	mErrorCode = error;
	switch(mErrorCode)
	{
	case olerr_invalid_req:
	case olerr_can_transfer_next:
		{
			m_eCurStatus = E_WAITIN_FOR_COMMOND;
		}
		break;
	case olerr_need_complete_login_proc:
	case olerr_be_kick_off:
	case olerr_token_id:
		{
			if(!isAlreadyLogin())
			{
				//CCLOG("token time out");
				ShowMessage("token time out");
				USERINFO info = UserData::GetUserInfo();
				info.tokenId = 0;
				UserData::SetUserInfo(info);
				backToLogin();
			}
			break;
		}
	case olerr_not_enough_treasure_box:
		{
			std::string msg = Localizatioin::getLocalization("M_TREASUREBOX_NOTENOUGH");	
			TXGUI::MessageBox::AdjustPriority(true);
			TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&XLogicManager::OnBuyKeyComfirmClicked),
				NULL, MB_YESNO,ccWHITE,24,KMessageBoxDefaultPriority);
			TXGUI::MessageBox::AdjustStyle(Localizatioin::getLocalization("M_KEY_FILLUP"),Localizatioin::getLocalization("S_CANCEL"));
			TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
			break;
		}
	case olerr_not_enough_key:
		{
			std::string msg = Localizatioin::getLocalization("M_KEY_NOTENOUGH");	
			TXGUI::MessageBox::AdjustPriority(true);
			TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&XLogicManager::OnBuyKeyComfirmClicked),
				NULL, MB_YESNO,ccWHITE,24,KMessageBoxDefaultPriority);
			TXGUI::MessageBox::AdjustStyle(Localizatioin::getLocalization("M_KEY_FILLUP"),Localizatioin::getLocalization("S_CANCEL"));
			TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
		break;
		}
	default:
		break;
	}

	// login error,may be session error
	if(mErrorCode >= acc_err_base && mErrorCode <= acc_err_max)
	{
		if(!isAlreadyLogin())
		{
			//CCLOG("login error");
			char msg[50];
			sprintf(msg,"login error: %d",(int)mErrorCode);
			ShowMessage(msg);
			USERINFO info = UserData::GetUserInfo();
			info.tokenId = 0;
			UserData::SetUserInfo(info);
			backToLogin();
		}
	}
}

db_errcode_t  XLogicManager::getNetErrorCode()
{
	if(mErrorCode != dberr_succ)
	{
		db_errcode_t preError = mErrorCode;
		mErrorCode = dberr_succ;
		return preError;
	}
	return dberr_succ;
}



void XLogicManager::notifyShowNextAppearUI()
{
	NotificationCenter::defaultCenter()->broadcast(GM_E_ENTER_FIRST_LOGIN_STATE,this);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//              networkDisConnected()
//
void XLogicManager::networkDisConnected()
{
	m_eCurLayer = E_SELECT_ZONE;
	m_eCurStatus = E_WAITIN_FOR_COMMOND;
	m_loginStep = 0;
	m_autoLogin =1 ;
    if(isGameStart)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        XLogicManager::sharedManager()->isIOSDisconnect = true;
#endif
		OnlineNetworkManager::sShareInstance()->UnRegisterEvent(this);
        GameManager::Get()->goToSplash();
		goToLayer(E_SELECT_ZONE, NULL);
		clearUserData();
	}
    
    //goToLayer(E_SELECT_ZONE,NULL);
	isGameStart = false;
	LevelManager::sShareInstance()->setCurrentLevelStatus(EUninit);
	NotificationCenter::defaultCenter()->broadcast(GM_NETWORK_DISCONNCT,this);
}
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XLogicManager::fromGameBackToLogin()
{
    XLogicManager::sharedManager()->isIOSInScene = false;
    
	//OnlineNetworkManager::sShareInstance()->cleanServerConnect(false);

	///// stop say hello to server now!
	NetStateChecker::Get()->Stop();
 //   //NetStateChecker::getInstance()->stopQuickCheck();
	//MessageFilter::Get()->Uninit();

	//m_loginStep = 0;
	m_autoLogin = 0 ;
	//networkDisConnected();
	NetStateChecker::Get()->Stop();
	//OnlineNetworkManager::sShareInstance()->sendLoginOutMessage();
	m_eCurLayer = E_SELECT_ZONE;
	m_eCurStatus = E_WAITIN_FOR_COMMOND;
	b_isLoginGame = false;
	GameManager::Get()->goToSplash();
	goToLayer(E_SELECT_ZONE, NULL);
	clearUserData();
	isGameStart = false;
	LevelManager::sShareInstance()->setCurrentLevelStatus(EUninit);
}

void XLogicManager::clearUserData()
{

	// 除了用于帐号登录相关的一些信息保存外，其它用户信息都得置
	//if(OnlineNetworkManager::sShareInstance()->getSocketStatus() != KSocketConnecting)
	{
		USERINFO userInfo = UserData::GetUserInfo();
		USERINFO newInfo;
		newInfo.id = userInfo.id;
		newInfo.tokenId = userInfo.tokenId;
		newInfo.serverId = userInfo.serverId;

		sprintf(newInfo.lastServerName,"%s",userInfo.lastServerName);
		sprintf(newInfo.userSession,"%s",userInfo.userSession);
		sprintf(newInfo.accessToken,"%s",userInfo.accessToken);
		UserData::SetUserInfo(newInfo);
	} 
	ItemManager::Get()->clearItems();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Talent/TalentMgr.lua", "ResetTalentMgr");
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/UnderBoxLayer.lua", "ResetUnderBoxData");
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "ResetReputationExchangeTime");

	UserData::Get()->clearOtherUserInfo();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UserDataManager.lua","clearUserInfo");
	//LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/ThreeDaysUI.lua","clearThreeDaysData");

	UIControlAppearControler::Get()->SetConfigDataReGetOrNot(true);
	TutorialsManager::Get()->SetConfigDataReGetOrNot(true);
	StoryInstanceDirector::Get()->SetConfigDataReGetOrNot(true);
	//jackniu
	//PopupRewardManager::Get()->SetConfigDataReGetOrNot(true);
	GameDataManager::Get()->resetEliteTranspointValid();
	TutorialsManager::Get()->ResetValue();
	MonthCardManager::Get()->resetUserMonthCardInfo();
	//jackniu task#135719  clear user autofight flag
	CCLOG("reset auto fight");
	CCUserDefault::sharedUserDefault()->setBoolForKey("autofight", false);
	CCUserDefault::sharedUserDefault()->flush();
	
}

void XLogicManager::sendPlatformLoginMessage(const char* sess,int platformId,int serverId,int userId,const char* appID,
						const char* appKey,const char* cpId,const char* channelId,const char* extraId,
						unsigned int gameId)
{
	string deviceId = LoginManager::Get()->getDeviceId();
	string os = LoginManager::Get()->getDeviceOS();
	string	macAddress = LoginManager::Get()->getMacAddress();
	string deviceType = LoginManager::Get()->getDeviceType();
	string version = AppDelegate::GetShareAppDelegate()->GetSvnVersion();
	OnlineNetworkManager::sShareInstance()->sendPlatformLoginMessage(sess,platformId,serverId,userId,appID,
		appKey,cpId,channelId,extraId,gameId,deviceId.c_str(),os.c_str(),macAddress.c_str(),deviceType.c_str(),version.c_str());
}

void XLogicManager::OnConnectedNetFrame(float dt)
{
	connectToServer();
}

void XLogicManager::onPlatformCallBack(int error)
{
	if(error == 0)
	{
		connectToServer();
		//scheduleOnce(schedule_selector(XLogicManager::OnConnectedNetFrame),0.01f);
		m_loginStep = 1;
	}
	else
	{
		//m_loginStep = 2;
		backToLogin();
	}
}

void XLogicManager::onCreateRoleSuceess()
{
	UserLoginStatistics::sendStatInfoToServer("create_role_success");
	OnlineNetworkManager::sShareInstance()->sendLoginMessage();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //NcSDKManager::HailStonecreateRoleWithAccout(UserData::getUserId().getID(), UserData::GetUserInfo().szName, "");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");
	if(channel == PPS)//pps is shit . need role info
	{
		LoginDeveloperInfo pInfo;
		char temp[500];
	    USERINFO info = UserData::GetUserInfo();
		unsigned int serverId = info.serverId;

        snprintf(temp, sizeof(temp), "%u", serverId);
        pInfo["zoneId"] = string(temp);
        pInfo["createRole"] = "true";
		TmLoginManager::tmLoginManager()->doSendMessage(pInfo);
	}
#endif

}

void XLogicManager::onReceiveSession()
{
	UserLoginStatistics::sendStatInfoToServer("check_session_success");
	m_loginStep = 2;
	USERINFO info = UserData::GetUserInfo();
	if(info.serverId != 0)
	{
		OnlineNetworkManager::sShareInstance()->sendGetRoleListMessage(info.serverId);
	}
	OnlineNetworkManager::sShareInstance()->sendGetTokenMessage();
	info.tokenId = 0;
	UserData::SetUserInfo(info);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::HailStoneloginWithAccout(UserData::getUserId().getID());
    NcSDKManager::trackReg(IntToStr(UserData::getUserId().getID()));
    NcSDKManager::trackLogin(IntToStr(UserData::getUserId().getID()));
#endif
}

void XLogicManager::createRole(const std::string nick, const unsigned int type)
{
	OnlineNetworkManager::sShareInstance()->sendCreateRoleMessage(nick,type,
		LoginManager::Get()->getDeviceId().c_str(),
		LoginManager::Get()->getDeviceOS().c_str(),
		LoginManager::Get()->getIpAddress().c_str(),
		LoginManager::Get()->getMacAddress().c_str(),
		LoginManager::Get()->getDeviceType().c_str(),
		AppDelegate::GetShareAppDelegate()->GetSvnVersion());
	USERINFO mUserData = UserData::GetUserInfo();
	strcpy(mUserData.szName,nick.c_str());
	mUserData.type = type;
	UserData::SetUserInfo(mUserData);
}

void XLogicManager::goToFb(unsigned int mapId, unsigned int battleId)
{
	FBLayer* pLayer = FBLayer::create();	
	CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 0, E_ENTER_FB);
	m_pCurLayer = pLayer;

	pLayer->EnterFB();

	m_eCurStatus = E_WAITIN_FOR_COMMOND;
	USERINFO mUserData = UserData::GetUserInfo();

	mUserData.mapId = mapId;
	//// add battle id
	mUserData.battleId = battleId;

	//LevelManager::sShareInstance()->SetBattleLevel(battleId);
	//InstanceManager::Get()->setCurInstanceId(battleId);

	RemovePPVELayer(false);
    RemovePVPLayer(false);
	InstanceListLayer * layer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
	if (layer)
	{
		layer->closeInstanceList(NULL);
	}

	UserData::SetUserInfo(mUserData);
	changeMap(mUserData.mapId, KMapFromTransition);
}

void XLogicManager::changeMap(int mapId, KMapFromType fromMapType)
{
	SceneLayer* pScene = dynamic_cast<SceneLayer*>(m_pCurLayer);

	if(pScene)
	{
		pScene->changeMap(mapId, fromMapType);
	}
}

////////////////////////////////////////////////////////////////////
// called when finish battle ,and go to town
void XLogicManager::backToTown()
{
	if(m_eCurStatus == E_SENDING_SEVER)
	{
		return;
	}
    
	/// if in pve level , stop local server now!
	//if (LevelManager::sShareInstance()->isCurrentPVELevel())
	//{
	//	MessageFilter::Get()->ReleaseBattleLevel();
	//}
	
	//LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/DeathTip/DeathTipSystem.lua","DeathTipSystem_SetBackToTown");
    
	m_eCurStatus = E_SENDING_SEVER;
	int curCityId ;
	bool isUnderMap = LevelManager::sShareInstance()->isCurrentUndergroundLevel();
	bool isCityDefendMap = LevelManager::sShareInstance()->isCurrentCityDefendLevel();
	bool isWorldBossMap = LevelManager::sShareInstance()->isCurrentWorldBossLevel();
	bool isBestFighterMap = LevelManager::sShareInstance()->isCurrentBestFighterLevel();
	bool isGuildInstanceMap = LevelManager::sShareInstance()->isCurrentGuildInstanceLevel();
	if (isUnderMap)
	{
		curCityId = InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_UNDERGROUND); 
	}
	//else if (isCityDefendMap)
	//{
	//	curCityId = InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_CITYDEFEND); 
	//}
	//else if (isWorldBossMap)
	//{
	//	curCityId = InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS);
	//}

	else if (isBestFighterMap)
	{
		curCityId = InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER);
	}
	else if (isGuildInstanceMap && GuildInstanceManager::Get()->getIsEnterFromGuildScene())
	{
		curCityId = InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE);
	}
	else
	{
		curCityId = MainLandManager::Get()->getCurCityId();
	}
	
	CCPoint heroBornCell = MainLandManager::Get()->getCityCell(curCityId, "born");
	CCPoint heroBornPoint = LevelManager::sShareInstance()->pointCellToPixel(heroBornCell);
	OnlineNetworkManager::sShareInstance()->sendPlayerEnterMapMessage(curCityId,heroBornPoint.x,heroBornPoint.y,0);	

	if (GameManager::Get()->GetSceneLayer())
	{
		GameManager::Get()->GetSceneLayer()->showLoading(true);
	}
}

void XLogicManager::LeaveBattle()
{
	OnlineNetworkManager::sShareInstance()->sendLeaveBattleReqMessage();
}

void XLogicManager::FinishBattle()
{
	if(GameManager::Get()->getHero())
	{
		//const unsigned int VOICE[7] ={0, 3211013, 3211213,  3211313, 3211413, 3211513, 3211613};
		//GameAudioManager::sharedManager()->playEffect(VOICE[GameManager::Get()->getHero()->getType()], false);
	}
	
	if (AccountsLayer::s_accountDetailData.isPveMode || AccountsLayer::s_accountDetailData.isPAIvEMode)
	{
		AccountsLayer *player = AccountsLayer::create();

		MainMenuLayer* layer = GetMainMenuLayer();
		CCAssert(layer != NULL,"");
		int tag = layer->getLayerTagByName("AccountsLayer");
        
        // 检查是否有这个layer
        if (GetMainMenuLayer()->getChildByTag(tag) != NULL)
        {
            return;
        }

		GetMainMenuLayer()->addChild(player,10,tag);
		player->openWinBattle();

		if (AccountsLayer::s_accountDetailData.isPveMode || AccountsLayer::s_accountDetailData.isPAIvEMode){
			player->accountBattle(AccountsLayer::s_accountDetailData.star, AccountsLayer::s_accountDetailData.killtime, 
				AccountsLayer::s_accountDetailData.killnum, AccountsLayer::s_accountDetailData.killcombo, 
				AccountsLayer::s_accountDetailData.add_coin, AccountsLayer::s_accountDetailData.add_diamond, 
				AccountsLayer::s_accountDetailData.add_exp, AccountsLayer::s_accountDetailData.add_exploit, 
				AccountsLayer::s_accountDetailData.add_contributions,
				AccountsLayer::s_accountDetailData.vec);
		}

		AccountsLayer::s_accountDetailData.ResetData();
	}
}

void XLogicManager::prepareBattle(int instanceId, int mapId)
{
	int preparingInstanceId = InstanceManager::Get()->getPreInstanceId();
	if (preparingInstanceId)
	{
		return;
	}

	InstanceManager::Get()->setPreInstanceId(instanceId);
	InstanceManager::Get()->setPreMapId(mapId);

	OnlineNetworkManager::sShareInstance()->sendBattlePrepareReq(instanceId);
}

void XLogicManager::createBattle(int battleId, int mapId)
{
	if(m_eCurStatus == E_SENDING_SEVER)
	{
		return;
	}

	//LevelManager::sShareInstance()->SetBattleLevel(battleId);
	//InstanceManager::Get()->setCurInstanceId(battleId);

	/// if it is pve battle map, load monster configuration from server
	//if (LevelManager::sShareInstance()->isCurrentPVELevel())
	//{
	//	MessageFilter::Get()->PrepareBattleLevel(battleId, mapId);
	//}

	m_eCurStatus = E_SENDING_SEVER;
	OnlineNetworkManager::sShareInstance()->sendBattleCreateMessage(battleId,mapId);

	bool isTest = false;
	CCLog("test...");
	if(isTest)
	{
		OnlineNetworkManager::sShareInstance()->sendBattleCreateMessage(battleId,mapId);
	}
}

void XLogicManager::receivedItemsInfo()
{
	if(isRequiredItemsInfo)
	{
		isRequiredItemsInfo = false;
	}
	else
	{
		//ItemManager::Get()->openBackPack();
	}
}
//
//void XLogicManager::initUserInfoFromSever(float time)
//{
//	//OnlineNetworkManager::sShareInstance()->sendBackPackMessage();
//	//OnlineNetworkManager::sShareInstance()->sendUserInfoReq(UserData::getUserId());
//	//OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(0,0);
//
//	//this->reqAllPlayerAttributes();
//	isInitLoginData = false;
//}


void XLogicManager::setNeedShowPPVEAfterBacktoTown(bool bShowPPve)
{
	this->m_bShowPPVEPanel = bShowPPve;
}

void XLogicManager::checkNeedShowPPVEPanel()
{
	if (this->m_bShowPPVEPanel)
	{
		GetPPVELayer();
		m_bShowPPVEPanel = false;
	}
}
void XLogicManager::entryCreateRole()
{
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(scene)
	{
		CCNode* layer = scene->getChildByTag(E_SELECT_ZONE);
		HelloWorld* helloLayer = dynamic_cast<HelloWorld*>(layer);
		if(helloLayer)
		{
			helloLayer->enterCreateRoleLayer();
		}
	}
}
void XLogicManager::entryLoginRole()
{
	//CSGetRoleListRsp::Roles role = msg->roles(i);
	USERINFO info = UserData::GetUserInfo();
	/*CCAssert(role.uid() == info.id.getID(),"receive error uid");
	CCAssert(role.channel_id() == info.id.getChannel(),"receive error channel_id");
	CCAssert(role.reg_time() != 0,"receive error reg_time, 0");
	info.id.setRegTime(role.reg_time());*/
	UserData::SetUserInfo(info);
	OnlineNetworkManager::sShareInstance()->sendLoginMessage();
}
void XLogicManager::onReceivedRoleList(CSGetRoleListRsp* msg)
{
    XLogicManager::sharedManager()->isIOSNeedWaitingLayer = false;
	PlayerRoleManager::Get()->onReceivedRoleList(msg);
	/*CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(scene)
	{
		CCNode* layer = scene->getChildByTag(E_SELECT_ZONE);
		HelloWorld* helloLayer = dynamic_cast<HelloWorld*>(layer);
		if(helloLayer)
		{
			helloLayer->getFrontLayer()->onReceivedRoleList(msg);
		}
	}*/
	
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////
/////                Ê∂àÊÅØÂìçÂ∫îÂáΩÊï�? ËøôÈáåÂèØ‰ª•Â§ÑÁêÜ‰∏Ä‰∫õÁ≥ªÁªüÊ∂àÊÅ?
///////
void XLogicManager::onBroadcastMessage(BroadcastMessage* msg)
{
	 switch(msg->message)
	 {
	 case GM_CLOSE_PPVELAYER:
		 CCLOG("receive close ppve message !!! for test!!");
		 break;
	 case GM_ATTR_HERO_LEVEL_UPDATE:
		 ItemManager::Get()->checkAlchemyEquipUnlock();
		 break;
	 }
}
void XLogicManager::OnConnected(int nResult)
{
	HideWaitingLayer();
}

void XLogicManager::OnDisconnect()
{
    /// network is down
	m_eCurStatus = E_WAITIN_FOR_COMMOND;
    
	/// stop say hello to server now!
	NetStateChecker::Get()->Stop();
    //NetStateChecker::getInstance()->stopQuickCheck();
	MessageFilter::Get()->Uninit();

	// OnlineNetworkManager will auto disconnect.
	////OnlineNetworkManager::sShareInstance()->cleanServerConnect(false);

	std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");	
	ShowMessage(msg.c_str());
	TXGUI::MessageBox::AdjustPriority(true);
	TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&XLogicManager::OnNetworkMessageOK),
		NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
	TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
}

void XLogicManager::OnBuyKeyComfirmClicked(CCObject* pObject)
{
	MainMenuLayer* layer = GetMainMenuLayer();
	if(layer)
	{
		layer->ShowMarketLayer(true,1);
	}
}

void XLogicManager::OnNetworkMessageOK(CCObject* pObject)
{
	/// use notification center to call exit game , since if release sceneLayer here, the MessageBoxLayer will crash!
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    CCDirector::sharedDirector()->end();
//    exit(0);
//    return;
//#endif
    
	XLogicManager::sharedManager()->isIOSInScene = false;
	networkDisConnected();
}

void XLogicManager::OnSocketError(int nError)
{
	
}

void XLogicManager::reqPlayerAttributes(int* attributes,int length)
{
	OnlineNetworkManager::sShareInstance()->sendAttributesReq(attributes,length);
}

void XLogicManager::reqPlayerAttributes(int attributeId)
{
	int attributes[1];
	attributes[0] = attributeId;
	OnlineNetworkManager::sShareInstance()->sendAttributesReq(attributes,1);
}
 
void XLogicManager::reqAllPlayerAttributes()
{
	int attributes[23];
	attributes[0] = PB_ATTR_BUY_BAG_CAPACITY;
	attributes[1] = PB_ATTR_BUY_STORE_CAPACITY;
	attributes[2] = PB_ATTR_BUY_SS_BAG_CAPACITY;
	attributes[3] = PB_ATTR_PLAYER_PRESTIGE;
	attributes[4] = PB_ATTR_PLAYER_SP;
	attributes[5] = PB_ATTR_PLAYER_SS_MOD_ID;
	attributes[6] = PB_ATTR_PLAYER_GEM_BACKPACK_CAPACITY;
	attributes[7] = PB_ATTR_PLAYER_RESET_GEM_TIMES;
	attributes[8] = PB_ATTR_PLAYER_LAST_LOGIN_MAP;
	attributes[9] = PB_ATTR_PLAYER_COIN;
	attributes[10] = PB_ATTR_PLAYER_DIAMOND;
	attributes[11] = PB_ATTR_EXP;
	attributes[12] = PB_ATTR_PLAYER_GEM_ANIMA;
	attributes[13] = PB_ATTR_PLAYER_SS_CHIP;
	attributes[14] = PB_ATTR_EQUIP_LVUP_FLAG;
	attributes[15] = PB_ATTR_PLAYER_RESET_GEM_TIMES;
	attributes[16] = PB_ATTR_PLAYER_STAMINA;
	attributes[17] = PB_ATTR_EFFECTIVENESS;
	attributes[18] = PB_ATTR_PLAYER_EXPLOIT;
	attributes[19] = PB_ATTR_PLAYER_EXPLORE_TIMES;
	attributes[20] = PB_ATTR_PLAYER_EXPLORE_EXP;
	attributes[21] = PB_ATTR_DAILY_TIMES_EXPLORE;
	attributes[22] = PB_ATTR_GET_STAMINA_RESTORE_TIME ;

	OnlineNetworkManager::sShareInstance()->sendAttributesReq(attributes,23);
}

void XLogicManager::OnHandleMessage( const std::string& cmd )
{
	m_waittForRsp.OnHandleMessage(cmd);
}

void XLogicManager::setMessageStatus( MESSAGE_STATUS msgStatus )
{
	m_msgStatus = msgStatus;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void XLogicManager::sendStatAtLaunch()
{
    UserLoginStatistics::sendLaunchStat(LoginManager::Get()->getChannelID(),
                                        AppDelegate::GetShareAppDelegate()->GetSvnVersion(),
                                        LoginManager::Get()->getDeviceOS().c_str(),
                                        LoginManager::Get()->getDeviceId().c_str(),
                                        LoginManager::Get()->getDeviceType().c_str(),
                                        LoginManager::Get()->getMacAddress().c_str()
                                        );
}
#endif

/////////////////////////////////////////////////////////////////////////////

extern "C"{

	void playerlogOut()
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "logOut", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
#endif 
	}

}
