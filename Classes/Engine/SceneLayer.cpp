#include "OnlineNetworkManager.h"
#include "SceneLayer.h"
#include "LevelDefine.h"
#include "SpriteSeer.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "UserData.h"
#include "ParticleManager.h"
#include "AParticleNode.h"
#include "UIManager.h"
#include "GameAudioManager.h"
#include "CConnectionData.h"
#include "SkeletonAnimRcsManager.h"
#include "LoadingLayer.h"
#include "TaskManager.h"
#include "MarketManager.h"
#include "XLogicManager.h"
#include "GMessage.h"
#include "SpriteElfManager.h"
#include "MainLandManager.h"
#include "StoryDataCenter.h"
#include "InstanceManager.h"
#include "StoryInstanceDirector.h"
#include "GameConfigFromLuaManager.h"
#include "GameUILayer.h"
#include "UIControlsApearControler.h"
#include "GameDataManager.h"
#include "PopupRewardManager.h"
#include "SpriteElfConfigFromLua.h"
#include "LuaTinkerManager.h"
#include "MessageFilter.h"
#include "TalentLayer.h"
#include "SystemConditionManager.h"
#include "GameMovieLayer.h"
#include "AutoOnHook.h"
#include "TutorialsManager.h"
#define LOADINGTIME 10

static bool bFirstInitData = false;
static bool second_to_mainland = false;//current is the go to mainland before the niu bi fu ben
SceneLayer::SceneLayer()
:m_curMapId(-1)
,m_lastMapId(-1)
,m_fromMapType(-1)
,m_LevelLayer(NULL)
,m_uiLayer(NULL)
,_barFrame(NULL)
,m_pStepMapLoader(NULL)
{
	this->m_accumulator = 0;
	this->m_lastTickTime = 0;
    this->m_pLoadinglayout = NULL;
	m_pStepMapLoader = new StepMapLoader(this);
}

SceneLayer::~SceneLayer()
{  
	if (m_pStepMapLoader)
	{
		delete m_pStepMapLoader;
		m_pStepMapLoader = NULL;
	}

	if (_barFrame)
		_barFrame->release();
	_barFrame = NULL;

	TXGUI::UIManager::sharedManager()->RemoveUILayout("LoadingLayer");
}


//////////////////////////////////////////////////////////////////////////
bool SceneLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}    
	//this->joystick = HSJoystick::create();
	//this->addChild(joystick, 2);
	HSJoystick* joystick = HSJoystick::sharedJoystick();
	//joystick->retain();
	//joystick->removeFromParent();
	float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
	joystick->setScale(scale);
	this->addChild(joystick, 2);
	//joystick->release();

	m_uiLayer = CCLayer::create();
	addChild(m_uiLayer, SCENE_Z_DYNAMIC_UI, SCENE_Z_DYNAMIC_UI);
	return true;
}

//////////////////////////////////////////////////////////////////////////
SpriteSeer*	SceneLayer::getSpriteSeer()
{
	if (m_LevelLayer)
	{
		return m_LevelLayer->getSpriteSeer();
	}

	return NULL;
}

MainMenuLayer *SceneLayer::getMainMenuLayer()
{
	if(m_uiLayer != NULL)
	{
		return (MainMenuLayer *)m_uiLayer->getChildByTag(SCENE_Z_MAIN_MENU);
	}
	return NULL;
}

CCLayer* SceneLayer::getUiLayerByTag(int tag)
{
	return (CCLayer*)m_uiLayer->getChildByTag(tag);
}


bool SceneLayer::isInHometownMap()
{
	return (0 == m_curMapId);
}

//////////////////////////////////////////////////////////////////////////

void SceneLayer::setUiLayer(CCLayer* layer,int tag)
{
	if(layer)
	{
		m_uiLayer->removeAllChildrenWithCleanup(true);
		layer->setTag(tag);
		m_uiLayer->addChild(layer);
	}
}

//////////////////////////////////////////////////////////////////////////

void SceneLayer::addUiLayer(CCLayer* layer,float zorder,int tag)
{
	if(layer)
	{
		m_uiLayer->addChild(layer,zorder,tag);
	}
}

void SceneLayer::setMainMenuLayerVisible(bool isVisible)
{
	if (this->getChildByTag(SCENE_Z_MAIN_MENU))
	{
		this->getChildByTag(SCENE_Z_MAIN_MENU)->setVisible(isVisible);
	}
}

//////////////////////////////////////////////////////////////////////////
void SceneLayer::setupMap()
{ 
	showLoading();

	needUpdate = false;
	//if(m_lastMapId != -1 && m_lastMapId != m_curMapId)
	{        
		removeMap(); 
		needUpdate = true;
	}
}

void SceneLayer::showLoading(bool bShowOnly /* = false */)
{
	if(m_pLoadingUI)
	{
		m_pLoadingUI->ShowLoading(bShowOnly);
	}
}

void SceneLayer::hideLoading()
{
    //_barFrame->setVisible(true);
    {  
		/// load map now!		
		loadResoure(10);
        loadMap();
    }
}

////// load map update function, called by schedule function
void SceneLayer::loadMapUpdate(float dt)
{
	m_pStepMapLoader->Update();
	if (m_pStepMapLoader->isCompleted())
	{
		unschedule(schedule_selector(SceneLayer::loadMapUpdate));
        bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
        USERINFO mUserData = UserData::GetUserInfo();

		bool enablenbfb = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "GetenableNiuBiFuBen");
			
        if(enablenbfb && bMainLandLevel && mUserData.first_login)
        {
            XLogicManager::sharedManager()->goToLayer(XLogicManager::E_FIRST_FIGHT, 0);
        }
        else if (enablenbfb && bMainLandLevel == false && mUserData.first_login)
		{
			CCLayerColor* pBlackColorLayer = CCLayerColor::create(ccc4(0,0,0,255));
			if (pBlackColorLayer)
			{
				int tag = MainMenuLayer::getLayerTagByName("FirstStartGameAnimLayer");
				this->addChild(pBlackColorLayer,Z_Order_StartMovieLayer,tag);
			}

			// Note: 添加开场动画
			USERINFO mUserData = UserData::GetUserInfo();
			{
				GameMovieLayer* pMovieLayer = GameMovieLayer::create();
				if (pMovieLayer)
				{
					int tag = MainMenuLayer::getLayerTagByName("GameMovieLayer");
					this->addChild(pMovieLayer,Z_Order_LoadingLayer,tag);
				}
			}

			if(m_pLoadingUI)
			{
				m_pLoadingUI->HideLoading();
			}
			TXGUI::UIManager::sharedManager()->RemoveUILayout("LoadingLayer");
		}
		else
		{
		    bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
            //check if we should show the auto fight panel
		    if(bMainLandLevel)
			{
				// play the movie then first login
				bool play = CCUserDefault::sharedUserDefault()->getBoolForKey("movie", false);
				if(!play)
				{
					CCUserDefault::sharedUserDefault()->setBoolForKey("movie", true);
					CCUserDefault::sharedUserDefault()->flush();
					GameMovieLayer* pMovieLayer = GameMovieLayer::create();
					if (pMovieLayer)
					{
						int tag = MainMenuLayer::getLayerTagByName("GameMovieLayer");
						this->addChild(pMovieLayer,Z_Order_LoadingLayer,tag);
					}
				}
               AutoOnHook::createAfterStartUp();
			
			   LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "EnterFristAppearUI");
			}
			schedule(schedule_selector(SceneLayer::hideloading),1.3f);
		}		    
        
       
	}
}
//this will called after the niu bi zhan dou
void SceneLayer::onFirstEnterMainland(){
    #if 0
    USERINFO mUserData = UserData::GetUserInfo();
    //main land go here 
    bFirstLauchGame = false;
    bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
    if(!bMainLandLevel)
        return;
    if(mUserData.first_login)
    {
        //play the niu bi fu ben
        XLogicManager::sharedManager()->goToLayer(XLogicManager::E_FIRST_FIGHT, 0);
	}
    else
    {
        schedule(schedule_selector(SceneLayer::hideloading),1.3f);
    }
    #endif
    //set this to false so .after finish the niu bi fu ben 
    //we can send the request info again to get backpack info from server
    bFirstInitData = false;
    second_to_mainland = true;
    SpriteElfConfigFromLuaManager::getInstance()->ClearElfLayerAllData();
    //next we will go back to the main land
}

void SceneLayer::StepMapLoader::Update()
{
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());

	// TODO: Need Update 
	

	switch(m_iCurStep)
	{
	case E_REQ:
		{
			
            USERINFO mUserData = UserData::GetUserInfo();
			XLogicManager::sharedManager()->GetWaittingForRsp().Start();
			// TODO : add more
			if (false == bFirstInitData || (mUserData.first_login && !bMainLandLevel))
			{
                if(second_to_mainland || (mUserData.first_login && !bMainLandLevel)) {
                //when first go to main land before niu bi fu ben, can not send this
                //before niu bi fu ben
                //since it will cause crash. send it second time
                OnlineNetworkManager::sShareInstance()->sendGetUserSkillReqMessage(UserData::getUserId());
                OnlineNetworkManager::sShareInstance()->sendGetSlotReqMessage();
                }					
            
				OnlineNetworkManager::sShareInstance()->sendBackPackMessage();	
				MarketManager::Get()->ReqShopTimesFromServer();
				OnlineNetworkManager::sShareInstance()->sendGetChipStatusReq(ITEM_TARGET_EQUIP_CHIP);
				OnlineNetworkManager::sShareInstance()->sendGetChipStatusReq(ITEM_TARGET_MONSTER_CHIP);
				OnlineNetworkManager::sShareInstance()->sendMonthCardInfoReq();
				//save liu liang, not send this msg .jackniu
				//OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(NORMAL_INSTANCE_MIN, NORMAL_INSTANCE_MAX);
				OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(JING_YING_FU_BEN_MIN, JING_YING_FU_BEN_MAX);
				//OnlineNetworkManager::sShareInstance()->sendGetAnnouncementPreReq();
				CCLOG("Loading Step Send CSAllItemsReq Message ");
			}
			m_iCurStep = E_RSP;
		}
		break;
	case E_RSP:
		{
			WaittingForRsp& rsp = XLogicManager::sharedManager()->GetWaittingForRsp();

			/*if(!rsp.IsReceived("CSGetUserSkillRsp"))
			{
				break;
			}*/

			// TODO : add more

			if (false == bFirstInitData)
			{
				if(!rsp.IsReceived("CSAllItemsRsp"))
				{
					CCLOG("Loading Step Have not Recieve CSAllItemsRsp Message ");
					break;
				}
				bFirstInitData = true;
			}			

			rsp.Stop();
			m_pScene->setupLoadbar(30);
			m_iCurStep = E_ANIM;
		}
		break;
	case E_ANIM:
		{			
			SkeletonAnimRcsManager::getInstance()->ClearAllRoleConnectionData();

			if (bMainLandLevel)
			{
				std::vector<unsigned int> vecRoleIds;
				vecRoleIds.push_back(1);
				vecRoleIds.push_back(2);
				vecRoleIds.push_back(3);
				vecRoleIds.push_back(4);
				vecRoleIds.push_back(5);
				vecRoleIds.push_back(6);
				SkeletonAnimRcsManager::getInstance()->addOneRoleTypeIdToMap(vecRoleIds);
			}
			else
			{
				// Note: PVE 模式
				bool bPVEMode = LevelManager::sShareInstance()->isCurrentPVELevel();
				if (bPVEMode)
				{
					int heroTypeId = GameManager::Get()->getHeroTypeId();
					if (heroTypeId != -1)
					{
						std::vector<unsigned int> vecRoleIds;
						vecRoleIds.push_back(heroTypeId);
						SkeletonAnimRcsManager::getInstance()->addOneRoleTypeIdToMap(vecRoleIds);
					}
				}
				else
				{
					//bool bPPVEMode = LevelManager::sShareInstance()->isCurrentPPVELevel();
					//bool bPVAIMode = LevelManager::sShareInstance()->isCurrentPVAILevel();
					//bool bPVPMode = LevelManager::sShareInstance()->isCurrentPVPLevel();
					//if (bPPVEMode || bPVAIMode || bPVPMode)
					{
						std::vector<unsigned int> vecRoleIds;

						const std::set<unsigned int> setPlayerTypes = LevelManager::sShareInstance()->GetPlayerTypes();
						for (std::set<unsigned int>::const_iterator iter = setPlayerTypes.begin();
							iter != setPlayerTypes.end();iter++)
						{
							vecRoleIds.push_back((*iter));	
						}						
						LevelManager::sShareInstance()->ClearAllPlayerTypes();
						SkeletonAnimRcsManager::getInstance()->addOneRoleTypeIdToMap(vecRoleIds);
					}										
				}
			}

			sActiveLevelConfig config;
			config.fromType = m_pScene->m_fromMapType;

			CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
			CCTextureCache::sharedTextureCache()->removeUnusedTextures();

			if(bMainLandLevel)
			{
				LevelManager::sShareInstance()->setCurrentLevelStatus(EInHomeTown);
			}
			else
			{
				LevelManager::sShareInstance()->setCurrentLevelStatus(EInInstance);
			}
			SpriteElfManager::Get()->ClearAllData();
			SkeletonAnimRcsManager::getInstance()->ClearSpriteElfVecIds();
			SkeletonAnimRcsManager::getInstance()->LoadCommonRcs(!bMainLandLevel);
            m_pScene->setupLoadbar(50);
		}

		/// goto next step
		m_iCurStep = E_ROLE;
		break;

	case E_ROLE:
		{
			
			SkeletonAnimRcsManager::getInstance()->LoadRoleRcsOneMap(bMainLandLevel);
			
            m_pScene->setupLoadbar(70);
		}
		m_iCurStep = E_MAP;
		break;
	case E_MAP:
		{
			unsigned int mapId = m_pScene->m_curMapId;
			unsigned int instanceID = InstanceManager::Get()->getCurInstanceId();
			SkeletonAnimRcsManager::getInstance()->LoadOneMapRcs(instanceID,mapId,!bMainLandLevel);
			if (bMainLandLevel == false)
			{
				GameAudioManager::sharedManager()->LoadLevelMonsterAudio(instanceID,mapId);
			}			
				
			if (!MessageFilter::Get()->getIsInited())
			{
				MessageFilter::Get()->Init();
			}	
			
            m_pScene->setupLoadbar(80);
		}
		m_iCurStep = E_STORY;
		break;

	case E_STORY:
		{
			sActiveLevelConfig config;
			config.fromType = m_pScene->m_fromMapType;

			CCLOG("began to active level: %d",m_pScene->m_curMapId);
			unsigned int nStoryType = 0;

			bool bLoadMapNormal = true;
			bool bPVEMode = LevelManager::sShareInstance()->isCurrentPVELevel();
			bool bStoryMode = false;
			int nInstanceStars = InstanceManager::Get()->getOneInstanceStars(InstanceManager::Get()->getCurInstanceId());
			if(bPVEMode && false == bMainLandLevel && nInstanceStars <= 0)
			{				
				bStoryMode = StoryDataCenter::Get()->IsOneItemHasStory(InstanceManager::Get()->getCurInstanceId(),m_pScene->m_curMapId,0,nStoryType);
				if (bStoryMode)
				{
					// Note: 进入副本时触发剧情
					if (nStoryType == 0)
					{
						m_pScene->m_LevelLayer = LevelManager::sShareInstance()->activeLevel(m_pScene->m_curMapId, config,true);
						StoryInstanceDirector::Get()->Begin(InstanceManager::Get()->getCurInstanceId(),m_pScene->m_curMapId,nStoryType);
						bLoadMapNormal = false;
					}
				}
			}

			if (bLoadMapNormal)
			{
				m_pScene->m_LevelLayer = LevelManager::sShareInstance()->activeLevel(m_pScene->m_curMapId, config);
				OnlineNetworkManager::sShareInstance()->sendLoadCompleteMessage();
				if (!bMainLandLevel && (LevelManager::sShareInstance()->isCurrentPPVELevel()
					                     || LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel()
										|| LevelManager::sShareInstance()->isCurrentPVPLevel()
										|| LevelManager::sShareInstance()->isCurrentCityDefendLevel()
										|| LevelManager::sShareInstance()->isCurrentBestFighterLevel()) )
				{
					//fix bug，始终显示等待其它玩家
					//if (InstanceManager::Get()->getPlayerCount() > 1)
					{
						BattleCoverLayer* battleCoverLayer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
						battleCoverLayer->openBattleCoverLayer(COVER_WAITINGOTHER);	
					}					
				}
			}

			m_pScene->m_LevelLayer->setPosition(ccp(0, 0));
			//重置timescale
			CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);


			// Note: 获取任务信息
			if (bMainLandLevel)
			{
				TaskManager::getInstance()->RqsTaskFromServer();
                // 默认开始精英副本
				//if (GameDataManager::Get()->IsEliteTranspointValid())
				//{
					SpriteTransPoint* tranpoint = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getEliteTranspoint();
					if (tranpoint)
					{
						tranpoint->setAppearence(true);
					}
				//}

				//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "UpdateDailyTaskEnterButtonState");


				//世界boss入口显示面板处理
				bool isInWorldBossEntrance = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS);
				if (isInWorldBossEntrance)
				{
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/WorldBossMgr.lua", "OnEnterWorldBoss", false);
				}

				//bool isInBestFighterEntrance = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER);
				//if (isInBestFighterEntrance)
				//{
				//	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "OnEnterBestFighterEntrance");
				//}
			}
			else
			{
				if (LevelManager::sShareInstance()->isCurrentWorldBossLevel())
				{
					//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/WorldBossMgr.lua", "OnEnterWorldBoss", true);
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/CoinBossMgr.lua", "OnEnterCoinBossInstance");
				}

				if (LevelManager::sShareInstance()->isCurrentHeroInstanceLevel() || LevelManager::sShareInstance()->isCurrentCityDefendLevel())
				{
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/HeroInstanceMgr.lua", "OnEnterHeroInstance");
				}
			}
			CCLOG("finish to active level: %d",m_pScene->m_curMapId);

			playMapBGM();
			OnlineNetworkManager::sShareInstance()->sendGetOtherPlayersMessage();
			m_pScene->needUpdate = true;


			if(m_pScene->needUpdate && m_pScene->m_LevelLayer)
			{
				m_pScene->addChild(m_pScene->m_LevelLayer, SCENE_Z_MAP, SCENE_Z_MAP);
			}

            USERINFO mUserData = UserData::GetUserInfo();
			XLogicManager::sharedManager()->GetWaittingForRsp().Start();
			// TODO : add more
			//if ((mUserData.first_login && !bMainLandLevel))
			{
			   
				SpriteSeer * seer = GameManager::Get()->getHero();
				OnlineNetworkManager::sShareInstance()->sendReqOneHeroElfMessage(UserData::getUserId(),seer->m_name.c_str());
			}

            m_pScene->setupLoadbar(100);
			if(bMainLandLevel)
			{
				if(GetMainMenuLayer())
				{
					GetMainMenuLayer()->showPreOpenLayer();

                    GameUILayer* glayer = dynamic_cast<GameUILayer*>(GetMainMenuLayer()->GetGameUILayer());
                    if (glayer != NULL)
                    {
                        glayer->ShowIconEffect();
                    }
				}
			}
		}
		m_iCurStep = E_END;

		break;

	default:
		m_iCurStep = E_END;		
		break;
	}
}

void SceneLayer::StepMapLoader::playMapBGM()
{
	int musicId = 0;
	if(MainLandManager::Get()->isMainLand(m_pScene->m_curMapId))
	{
		musicId = MainLandManager::Get()->GetCityBGM(m_pScene->m_curMapId);
		
	}
	else
	{
		int instanceId = InstanceManager::Get()->getCurInstanceId();
		musicId = GameConfigFromLuaManager::Get()->getInstanceMusicId(instanceId,m_pScene->m_curMapId);
	}
	GameAudioManager::sharedManager()->playBGMWithMap(musicId);
}

void SceneLayer::loadMap()
{
	//CCAssert(!m_LevelLayer,"m_LevelLayer not NULL");
    if(!m_LevelLayer)
	{
		m_pStepMapLoader->Begin();
		schedule(schedule_selector(SceneLayer::loadMapUpdate), 0.1f);
	}
	else
	{
		CCLOG("Error: Loading Step LevelLayer Null");
	}
}

//////////////////////////////////////////////////////////////////////////
void SceneLayer::removeMap()
{   
	CCLOG("began to remove map :%d", m_lastMapId);
	LevelManager::sShareInstance()->deactiveCurrentLevel(true, true);
	GameAudioManager::sharedManager()->stopPlayBGM();
	CCLOG("finish remove map :%d", m_lastMapId);
	m_LevelLayer = NULL;
}

void SceneLayer::removeUi()
{
	m_uiLayer->removeAllChildrenWithCleanup(true);
	//CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void SceneLayer::clearCurScene()
{
	removeUi();
	removeMap();    
	bFirstInitData = false;
}
//////////////////////////////////////////////////////////////////////////


void SceneLayer::changeMap(int toMapId,KMapFromType fromMapType)
{    
	CCLOG("change map to :%d", toMapId);
	if (m_curMapId == toMapId)
	{
		CCLOG("SceneLayer::changeMap same id %d",m_curMapId);
		return ;
	}

	LevelManager::sShareInstance()->setCurrentLevelStatus(EChangeMap);
	GameAudioManager::sharedManager()->stopAllEffect();
   
	//
	m_lastMapId = m_curMapId;
	m_curMapId  = toMapId;
	m_fromMapType = fromMapType;

	MainMenuLayer* mLayer = getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ChangeMapUI(toMapId);
	}

	setupMap();
}

//////////////////////////////////////////////////////////////////////////
/// 进入主城
void SceneLayer::enterMyRoom()
{
	// create main menu layer and add it to the SceneLayer as its child
	MainMenuLayer *mainMenu = getMainMenuLayer();
	if (!mainMenu)
	{
		mainMenu = (MainMenuLayer *)MainMenuLayer::create();
		m_uiLayer->addChild(mainMenu, SCENE_Z_MAIN_MENU, SCENE_Z_MAIN_MENU);
		mainMenu->resetMailUIMsg();

        int tag = mainMenu->getLayerTagByName("LoadingLayer");
        m_pLoadingUI = (LoadingLayer*)mainMenu->getChildByTag(tag);
        setupLoadbar(10);

		USERINFO mUserData = UserData::GetUserInfo();
		if ((false == mUserData.first_login))
		{
			UIControlAppearControler::Get()->FirstHideAllControls();
			UIControlAppearControler::Get()->InitPushAllAutoControls();
			UIControlAppearControler::Get()->SendGetControlsState();
			UIControlAppearControler::Get()->TackHeroLevelEvent();
			UIControlAppearControler::Get()->CheckButtonStateAtFirst();

			if (false == GameDataManager::Get()->IsEliteTranspointValid())
			{
				GameDataManager::Get()->sendEliteTranspointValidReq();
			}

			StoryInstanceDirector::Get()->SendReqCheckPlayedStory();
//jackniu
//			PopupRewardManager::Get()->SendReqPopupItemsState();		
			OnlineNetworkManager::sShareInstance()->sendDailyTaskReqMessage();
			SystemConditionManager::Get()->SendSystemCheckMessage();
			TutorialsManager::Get()->SendTutorialStateRqsMessage();
		}

		//初始化活动时间数据管理
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/TimingActivityMgr.lua", "InitTimingActivityMgr");

		//拉取活动开启时间
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/TimingActivityMgr.lua", "SendGetTimingActivityDataReq");

		//拉取设置
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameSetting.lua", "sendSettingAttrReq");

		//拉取在线奖励时间与重数
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/OnlineRewardUI.lua", "registOnlineRewardRspAndSendReq");

		//拉取首充奖励数据
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/FirstPayRewardUI.lua", "registTopupRewardRsp");
		
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/operationUI.lua", "sendCSGetActivityDataReq");

		//拉取累计登录
	//	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/SevenDaysUI.lua", "registSevenDaysRewardRspAndSendReq");
		
	}        

	int mapId = UserData::GetUserMapId();
	//by benyang: test
	changeMap(mapId, KMapFromMyRoom);	
}

void SceneLayer::EnterFB()
{
	MainMenuLayer *mainMenu = getMainMenuLayer();
	if (NULL == mainMenu)
	{
		mainMenu = (MainMenuLayer *)MainMenuLayer::create();
		m_uiLayer->addChild(mainMenu, SCENE_Z_MAIN_MENU, SCENE_Z_MAIN_MENU);
		mainMenu->resetMailUIMsg();

		int tag = mainMenu->getLayerTagByName("LoadingLayer");
        m_pLoadingUI = (LoadingLayer*)mainMenu->getChildByTag(tag);
        setupLoadbar(10);
	}
}

void SceneLayer::resumeLastMap()
{
	changeMap(m_curMapId,KMapFromLast);
}

//////////////////////////////////////////////////////////////////////////
void SceneLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	// convert touch point
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
	touchPoint = LevelMultiResolution::sTransformWindowPointToMap(touchPoint);
    
	//let game manager unified process the touch
	GameManager::Get()->processTouchBegan(touchPoint);
}

void SceneLayer::setupLoadbar(int percent)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	UIPicture *barLoadingPic = NULL;
    UILayout *pLayout = TXGUI::UIManager::sharedManager()->getUILayout("LoadingLayer");
	if (pLayout == NULL)
	{
		TXGUI::UIManager *manager = TXGUI::UIManager::sharedManager();
		m_pLoadinglayout= manager->CreateUILayoutFromFile("UIplist/loading.plist", m_pLoadingUI, "LoadingLayer");

		TXGUI::UIButton* skipAnimationBtn = m_pLoadinglayout->FindChildObjectByName<UIButton>("skipBtn");	 skipAnimationBtn->setVisible(false);
		TXGUI::UILabel* rcsUpdateSpeedLabel = m_pLoadinglayout->FindChildObjectByName<UILabel>("rcsUpdateSpeedLabel");	rcsUpdateSpeedLabel->setVisible(false);
		TXGUI::UILabel* rcsUpdateProcessLabel_1 = m_pLoadinglayout->FindChildObjectByName<UILabel>("rcsUpdateProgressLabel_1");	rcsUpdateProcessLabel_1->setVisible(false);
		TXGUI::UILabel* rcsUpdateProcessLabel_2 = m_pLoadinglayout->FindChildObjectByName<UILabel>("rcsUpdateProgressLabel_2");	rcsUpdateProcessLabel_2->setVisible(false);
		TXGUI::UILabel* rcsUpdateCurVersionLabel = m_pLoadinglayout->FindChildObjectByName<UILabel>("rcsUpdateCurVersionLabel");	rcsUpdateCurVersionLabel->setVisible(false);
		TXGUI::UILabel* rcsUpdateNewVersionLabel = m_pLoadinglayout->FindChildObjectByName<UILabel>("rcsUpdateNewVersionLabel");	rcsUpdateNewVersionLabel->setVisible(false);
		TXGUI::UIPicture* rcsUpdateFrameLoading = m_pLoadinglayout->FindChildObjectByName<UIPicture>("rcsUpdateFrameLoading");	rcsUpdateFrameLoading->setVisible(false);
		TXGUI::UIPicture* rcsUpdateBarLoading = m_pLoadinglayout->FindChildObjectByName<UIPicture>("rcsUpdateBarLoading");	rcsUpdateBarLoading->setVisible(false);
		
		barLoadingPic = m_pLoadinglayout->FindChildObjectByName<UIPicture>("barLoading");
		CCSprite* psprite = AspriteManager::getInstance()->getOneFrame("UI/ui_loading.bin","map_ui_loading_FRAME_BAR_LOADING");
		barLoadingPic->setSprite(psprite);
		barLoadingPic->getCurrentNode()->setAnchorPoint(ccp(0,0.5));
		barLoadingPic->setPosition(ccpSub(barLoadingPic->getPosition(),ccp(barLoadingPic->getSpriteSize().width/2, 0)));

		// Note: 显示提示小贴士
		UILabel *tipLabel = m_pLoadinglayout->FindChildObjectByName<UILabel>("textTips");
		if (tipLabel)
		{
			const char* content = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/Loading/loadingTips.lua","LoadingTips_CalRandomTxt");
			tipLabel->setString(content);
			tipLabel->setColor(ccc3(255, 211, 93));
		}
	}
	else
	{
		barLoadingPic = m_pLoadinglayout->FindChildObjectByName<UIPicture>("barLoading");
	}
    
	float scale = UIManager::sharedManager()->getScaleFactor();
    float f_percent = 1.0f*percent/100;        
    barLoadingPic->setScale(f_percent*scale, scale);
}

void SceneLayer::loadResoure(int percent)
{
}

void SceneLayer::hideloadbar_func(cocos2d::CCNode *sender)
{
	hideloadlayer();
}

void SceneLayer::removeColorLayer()
{
   this->removeChildByTag(1000,true);

   bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
   if (bMainLandLevel)
   {
	   MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	   if (mLayer)
	   {
		   //mLayer->GetGameUILayer()->showSpecialButtonEffect();
	   }
   }

   if (bMainLandLevel)
   {
	   NotificationCenter::defaultCenter()->broadcast(GM_E_ENTER_MAINLAND_COLOR_LAYER_OVER, this);
   }
}

void  SceneLayer::hideloading(float dt)
{
	unschedule(schedule_selector(SceneLayer::hideloading));
	hideloadlayer();
}

void SceneLayer::hideloadlayerOnFirstGame(float dt)
{
	unschedule(schedule_selector(SceneLayer::hideloadlayerOnFirstGame));

	float duration = 1.1f;
	CCLayerColor* l = CCLayerColor::create(ccc4(0,0,0,255));
	addChild(l,0,1000);

	CCFadeOut *fadeout = CCFadeOut::create(duration);
	l->runAction(fadeout);

	CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(SceneLayer::removeColorLayer));
	CCSequence *seq = (CCSequence*)CCSequence::create(CCDelayTime::create(duration),rc,NULL);
	runAction(seq);
}

void  SceneLayer::hideloadlayer()
{
	if(m_pLoadingUI)
	{
	 
		m_pLoadingUI->HideLoading();
		
		CCLOG("loading ui hideloaoding\n");
	}

    TXGUI::UIManager::sharedManager()->RemoveUILayout("LoadingLayer");

    float duration = 1.1f;
    CCLayerColor* l = CCLayerColor::create(ccc4(0,0,0,255));
    addChild(l,0,1000);
    
    CCFadeOut *fadeout = CCFadeOut::create(duration);
    l->runAction(fadeout);
    
    CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(SceneLayer::removeColorLayer));
    CCSequence *seq = (CCSequence*)CCSequence::create(CCDelayTime::create(duration),rc,NULL);
    runAction(seq);
}
// -----------------------------------------------------------------------------------------------------

MainLandLayer::MainLandLayer()
	: SceneLayer()
{

}

MainLandLayer::~MainLandLayer()
{

}

// -----------------------------------------------------------------------------------------------------
FBLayer::FBLayer()
	: SceneLayer()
{

}

FBLayer::~FBLayer()
{

}
