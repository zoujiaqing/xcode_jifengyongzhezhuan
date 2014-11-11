#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "HelloWorldScene.h"
#include "CConnectionData.h"
#include "CXLog.h"
#include "cocos2d.h"
#include "ASprite.h"
#include "AspriteManager.h"
#include "EnginePlayerManager.h"
#include "GameResourceManager.h"
#include "XLogicManager.h"
#include "RoleBase.h"
#include "RoleActorBase.h"
#include "SkeletonAnimRcsManager.h"
#include "HeroRoleManager.h"
#include "CharacterSelectLayer.h"
#include "ParticleManager.h"
#include "MainLandManager.h"
#include "MarketManager.h"
#include "StoryDataCenter.h"
#include "Localization.h"
#include "MessageBoxLayer.h"
#include "DressUpManager.h"
#include "TutorialsManager.h"
#include "PopupRewardManager.h"
#include "UIControlsApearControler.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"
#include "MainMenuLayer.h"
#include "GameAudioManager.h"
#include "UserLoginStatistics.h"
#include "SystemConditionManager.h"
#include "WaitingLayerManager.h"
#include "ResourceVersionCheckFromServer.h"
#include "NetWorkConfigInfoFromServer.h"
#include "TreasureExploreManager.h"
#include "ServerInfoFromServer.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "TmLoginManager.h"
#endif

// -----------------------------------------------------------------------------
LoginScene::LoginScene()
{

}

LoginScene::~LoginScene()
{

}

LoginScene * LoginScene::create( void )
{
	LoginScene *pRet = new LoginScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

// -----------------------------------------------------------------------------
UpdateRcsScene::UpdateRcsScene()
{

}

UpdateRcsScene::~UpdateRcsScene()
{

}

UpdateRcsScene * UpdateRcsScene::create( void )
{
	UpdateRcsScene *pRet = new UpdateRcsScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}


// -----------------------------------------------------------------------------


GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	XLogicManager::sharedManager()->exitGame();

}

GameScene * GameScene::create( void )
{
	GameScene *pRet = new GameScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}


// ------------------------------------------------------------------------------
//// errror message cached
static  string g_ErrorMsgCached;

HelloWorld::HelloWorld()
	:pFrontLayer(NULL)
,pUIMessageLayer(NULL)
,pCharacterLayer(NULL)
{
	isUpdateRunning = false;
    isNeedCheckNetWork = true;
	OnlineNetworkManager::sShareInstance()->RegisterEvent(this);
}

HelloWorld::~HelloWorld(){
	OnlineNetworkManager::sShareInstance()->UnRegisterEvent(this);
	MessageBox::clearMessageBox();
    WaitingLayerManager::Get()->clearMessages();
}

CCScene* HelloWorld::scene(){
    CCScene * scene = NULL;
    do{
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        //HelloWorld *layer = HelloWorld::create();
        //CC_BREAK_IF(! layer);

        //// add layer as a child to scene
        //scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	bool bRet = false;

    if(!CCLayerColor::initWithColor(ccc4(200, 200, 200, 255)))
    {
        return bRet;
    }

    // add frontCover image
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("UI/frontCover.jpg");
    CCSprite* pSprite = CCSprite::create(path);
    float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
    float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
    float scale = (scaleX > scaleY) ? scaleX : scaleY;
    pSprite->setScaleX(scale);
    pSprite->setScaleY(scale);
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(pSprite, 0);

    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HelloWorld::OnNextFrame), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HelloWorld::OnNextFrame), this, 0,0,0,false);
    return true;
}

void HelloWorld::OnNextFrame(float dt)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HelloWorld::OnNextFrame), this);

    do {
        AspriteManager::getInstance()->loadAllAsprite();
        if (false == NetWorkConfigInfoFromServer::sharedInstance()->IsDownLoadContentFromServer())
        {
            this->scheduleOnce(schedule_selector(HelloWorld::PopupWarnningMessageBox),0);
            break;
        }

        setTouchEnabled(true);
        resetReloadResourceID();

        SkeletonAnimRcsManager::getInstance()->LoadAllData();
        StoryDataCenter::Get()->LoadDataFromXML();
        MainLandManager::Create();
        MainLandManager::Get()->LoadData();
        DressUpManager::Get()->LoadXML();
        ParticleManager::Create();
        TutorialsManager::Get()->LoadData();
        //jackniu 2014-5-2. not use popup function
        //PopupRewardManager::Get()->InitXMLData();
        UIControlAppearControler::Get()->InitXMLData();
        SystemConditionManager::Get()->Init();
        LoginManager::Create();
        ParticleManager::Get()->AddParticleConditonData();
        MarketManager::Get();
        TreasureExploreManager::Get();
        //this->scheduleUpdate();

        CCDirector::sharedDirector()->setGLDefaultValues();

        pUIMessageLayer = UIMessageLayer::create();
        this->addChild(pUIMessageLayer,100,0);

        GameAudioManager::sharedManager()->setBackgroundMusicVolume(CCUserDefault::sharedUserDefault()->getIntegerForKey("GameSettingSliderUpperKey",100));               
        GameAudioManager::sharedManager()->setEffectsVolume( CCUserDefault::sharedUserDefault()->getIntegerForKey("GameSettingSliderDownKey",100));        
        GameAudioManager::sharedManager()->playLoadingBGM();	    

        updateOnNextFrame(0.0f);
        this->schedule(schedule_selector(HelloWorld::updateManager),0.01f);
        this->setKeypadEnabled(true);

    } while (0);

    //this->setTag(TAG_HELLOWORLD);
}

void HelloWorld::PopupWarnningMessageBox(float dt)
{
	std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
	TXGUI::MessageBox::AdjustPriority(true);
	TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
	TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
	TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
}

void HelloWorld::updateOnNextFrame(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool bEnableVersionCheck = false;
	if (bEnableVersionCheck)
	{
		if (sendCheckNewVersionMessage())
		{
			bool isNeedUpdateRcs = ResouceVersionCheckFromServer::sharedInstance()->IsNeedUpdateResources();
			if (isNeedUpdateRcs)
			{
				std::string msg = Localizatioin::getLocalization("M_FORCEUPDATE_HINT");
				TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&HelloWorld::OnVersionCheckBoxOkBtnClick),
					NULL, MB_OK,ccWHITE,24);

				return ;
			}
		}
	}
#endif

	setUpLogin();
}

bool HelloWorld::sendCheckNewVersionMessage()
{
	if (false == ResouceVersionCheckFromServer::sharedInstance()->downloadConfigFileList())
	{
		std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
		TXGUI::MessageBox::AdjustPriority(true);
		TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
		TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
		TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
		return false;
	}
	else
	{
		return ResouceVersionCheckFromServer::sharedInstance()->parseRcvMessageFromServer();
	}
}

void HelloWorld::OnVersionCheckBoxOkBtnClick(CCObject* pObject)
{	
	CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

void HelloWorld::setUpLogin()
{
	unsigned int channelId = LoginManager::Get()->getChannelID();
	char channelStr[20];
	sprintf(channelStr,"%d",channelId);
	bool receiveFlag = ServerInfoFromServer::sharedInstance()->downloadConfigFileList(channelStr);
	if(receiveFlag)
	{
		USERINFO info =	UserData::GetUserInfo();
		if(ServerInfoFromServer::sharedInstance()->parseRcvMessageFromServer())
		{
			if(OnlineNetworkManager::sShareInstance()->getSocketStatus() != KSocketConnecting)
			{
				bool hasServer = false;
				std::vector<OnlineServerInfo> serverList = ServerInfoFromServer::sharedInstance()->GetServerInfo();
				for(std::vector<OnlineServerInfo>::iterator it = serverList.begin();it != serverList.end();it++)
				{
					int serverId = atoi((*it).tServerId.c_str());
					if(serverId == info.serverId)
					{
						OnlineNetworkManager::sShareInstance()->setServerIP((*it).tIp.c_str());
						OnlineNetworkManager::sShareInstance()->setServerPort((*it).tPort);
						hasServer = true;
						break;
					}
					else
					{
						if((*it).mergedServerVec.size() > 0)
						{
							for(std::vector<std::string>::iterator iter = (*it).mergedServerVec.begin();iter != (*it).mergedServerVec.end();iter++)
							{
								int mergedServerId = atoi((*iter).c_str());
								if(mergedServerId == info.serverId)
								{
									string oldServerName = info.lastServerName;
									OnlineNetworkManager::sShareInstance()->setServerIP((*it).tIp.c_str());
									OnlineNetworkManager::sShareInstance()->setServerPort((*it).tPort);
									hasServer = true;
									info.serverId = serverId;
									snprintf(info.lastServerName,30,"%s",(*it).tServerName.c_str());
									UserData::SetUserInfo(info);
									
									std::string msg = "";
									msg.append(Localizatioin::getLocalization("M_COMBINESERVE_HINT1"));
									msg.append(oldServerName);
									msg.append(Localizatioin::getLocalization("M_COMBINESERVE_HINT2"));
									msg.append(info.lastServerName);
									msg.append(Localizatioin::getLocalization("M_COMBINESERVE_HINT3"));
									TXGUI::MessageBox::AdjustPriority(true);
									TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,20,KMessageBoxDefaultPriority);
									TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
									TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);

									break;
								}
							}
						}
					}
				}
				if(!hasServer)
				{
					const char* serverIP = NetWorkConfigInfoFromServer::sharedInstance()->GetGetWayIP();
					int serverPort = NetWorkConfigInfoFromServer::sharedInstance()->GetGetWayPort();
					OnlineNetworkManager::sShareInstance()->setServerIP(serverIP);
					OnlineNetworkManager::sShareInstance()->setServerPort(serverPort);
				}
			}
			else
			{
				if(info.serverId != 0)
				{
					OnlineNetworkManager::sShareInstance()->sendGetRoleListMessage(info.serverId);
				}
			}
		}
	}
	else
	{
		std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
		TXGUI::MessageBox::AdjustPriority(true);
		TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
		TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
		TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	this->scheduleUpdate();
	isUpdateRunning = true;
	scheduleOnce(schedule_selector(HelloWorld::updateLogin), 0.1f);
#else
	this->scheduleUpdate();
    CCLog("111111111111111111111");
	isUpdateRunning = true;
	if(!this->checkUserLogin())
	{
		scheduleOnce(schedule_selector(HelloWorld::updateLogin), 0.1f);
	}		
#endif

}

void HelloWorld::OkBtnClick(CCObject* obj)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TmLoginManager::tmLoginManager()->doSdkExit();
#endif
	CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

void HelloWorld::CancelBtnClick(CCObject* obj)
{
	CCLOG("HelloWorld::CancelBtnClick");
}


void HelloWorld::keyBackClicked()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");
	
			if(channel == Nd91 ) 
			{
				TmLoginManager::tmLoginManager()->doSdkExit();
				return;
			}
			
#endif

	std::string content = Localizatioin::getLocalization("S_Message_common_content");
	TXGUI::MessageBox::AdjustPriority(true);
	TXGUI::MessageBox::Show(content.c_str(),
		this,
		menu_selector(HelloWorld::OkBtnClick),
		menu_selector(HelloWorld::CancelBtnClick),
		MB_OKCANCEL,
		ccWHITE,
		24,
		KMessageBoxExitApp);
	TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
}

void HelloWorld::keyMenuClicked()
{
	CCLOG("keyMenuClicked");
}

void HelloWorld::enterCreateRoleLayer()
{
	UserLoginStatistics::sendStatInfoToServer("before_create_role");
	if(pFrontLayer != NULL)
	{
		this->removeChild(pFrontLayer,true);
		pFrontLayer = NULL;
	}
	if(pCharacterLayer == NULL)
	{
		pCharacterLayer = CharacterSelectLayer::create();
		this->addChild(pCharacterLayer,4,2);
	}
	pCharacterLayer->setVisible(true);
}

void HelloWorld::backToLogin()
{
	//OnlineNetworkManager::sShareInstance()->RegisterEvent(this);
	if(!isUpdateRunning)
	{
		this->scheduleUpdate();
		isUpdateRunning = true;
	}
    
}

void HelloWorld::enterFrontCoverLayer()
{
    if(pFrontLayer == NULL)
	{
        pFrontLayer = FrontCoverLayer::create();
		this->addChild(pFrontLayer,3,1);
	}
	CCNode* loginLayer = this->getChildByTag(MainMenuLayer::getLayerTagByName("LoginLayer"));
	if(loginLayer)
	{
		loginLayer->setVisible(false);
	}
	CCNode* registerLayer = this->getChildByTag(MainMenuLayer::getLayerTagByName("RigisterLayer"));
	if(registerLayer)
	{
		registerLayer->setVisible(false);
	}
	
    pFrontLayer->setVisible(true);
}

void HelloWorld::updateLogin(float dt)
{
	UserLoginStatistics::sendStatInfoToServer("before_user_login");
	if(OnlineNetworkManager::sShareInstance()->getSocketStatus() != KSocketConnecting)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		XLogicManager::sharedManager()->connectToServer();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		bool platformFlag = false;
		unsigned int nPlatformFlag = NetWorkConfigInfoFromServer::sharedInstance()->GetUsePlatformFlag();
		if (nPlatformFlag != 0)
		{
			platformFlag = true;
		}
		if(platformFlag)
		{
			LoginManager::Get()->doPlatformLogin();
			//enterFrontCoverLayer();
			//this->unscheduleUpdate();
		}
		else
		{
			LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","enterLoginLayer");
		}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#ifdef TargetForNw
	   LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","enterLoginLayer");
#else
	   LoginManager::Get()->doPlatformLogin();
#endif
#else
    	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","enterLoginLayer");
#endif
	}
}

void HelloWorld::update(float dt)
{
	//left = right = down = false; 
	if(this->checkUserLogin())
	{
        CCLog("2222222222222222");
		enterFrontCoverLayer();
		this->unscheduleUpdate();
		isUpdateRunning = false;
	}
}

void HelloWorld::updateManager(float dt)
{
	WaitingLayerManager::Get()->update(dt);
}

bool HelloWorld::checkUserLogin()
{
	bool isLogin = false;
	int step = XLogicManager::sharedManager()->getLoginStep();
    CCLog("step = %d", step);
	if(step > 1)
	{
		isLogin = true;
	}
	return isLogin;
}

void HelloWorld::setNeedCheckNetWork(bool flag)
{
    isNeedCheckNetWork = flag;
}

void HelloWorld::OnConnected(int nResult)
{
	CCLOG("HelloWorld::OnConnected: %d",nResult);
	WaitingLayerManager::Get()->clearMessages();
	HideWaitingLayer();
	if(OnlineNetworkManager::sShareInstance()->getSocketStatus() == KSocketConnecting)
	{
		if(!checkUserLogin())
		{
			UserLoginStatistics::sendStatInfoToServer("before_check_session");
			LoginManager::Get()->onPlatformLoginSuccess();
		}
		else
		{
			OnlineNetworkManager::sShareInstance()->sendCheckTokenMessage();
		}
		
		//OnlineNetworkManager::sShareInstance()->UnRegisterEvent(this);
	}
    isNeedCheckNetWork = true;
}

void HelloWorld::OnDisconnect()
{
	WaitingLayerManager::Get()->clearMessages();
	HideWaitingLayer();
    bool isTimeOut = true;
    if(OnlineNetworkManager::sShareInstance()->getSocketStatus() == KSocketDisconnect)
    {
        isTimeOut = false;
    }
    if(isNeedCheckNetWork || isTimeOut)
    {
        if(pCharacterLayer != NULL)
        {
            std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
            showMessage(msg.c_str());
            TXGUI::MessageBox::AdjustPriority(true);
            TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&HelloWorld::onNetWorkCallBackToFront),
                                    NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
            TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
        }
        else
        {
            std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
            showMessage(msg.c_str());
            TXGUI::MessageBox::AdjustPriority(true);
            TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&HelloWorld::onNetWorkCallBackExit),
                                    NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
            TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
        }
        XLogicManager::sharedManager()->setLoginStatus(2);
    }
    isNeedCheckNetWork = true;
	
}

void HelloWorld::OnSocketError(int nError)
{
	
}

void HelloWorld::showMessage(const char* msg)
{
	if(pUIMessageLayer)
	{
		pUIMessageLayer->ShowMessage(msg,KMessageDefaultColor);
	}
	else
	{
        g_ErrorMsgCached = msg;
    }
}

FrontCoverLayer* HelloWorld::getFrontLayer()
{
	return pFrontLayer;
}

void HelloWorld::onNetWorkCallBackToFront(CCObject* obj)
{
	if(pCharacterLayer != NULL)
	{
		pCharacterLayer->removeFromParentAndCleanup(true);
		pCharacterLayer = NULL;
	}
	enterFrontCoverLayer();
}

void HelloWorld::onNetWorkCallBackExit(CCObject* obj)
{
	CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

//void HelloWorld::keyBackClicked()
//{
//	std::string content = Localizatioin::getLocalization("S_Message_common_content");
//	TXGUI::MessageBox::Show(content.c_str(),this,menu_selector(HelloWorld::OkBtnClick),menu_selector(HelloWorld::CancelBtnClick));
//}
//
//void HelloWorld::keyMenuClicked()
//{
//	CCLOG("keyMenuClicked");
//}
//
//void HelloWorld::OkBtnClick(CCObject* obj)
//{
//	CCDirector::sharedDirector()->end();
//}
//
//void HelloWorld::CancelBtnClick(CCObject* obj)
//{
//	CCLOG("HelloWorld::CancelBtnClick");
//}


