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
#include "GameUILayer.h"
#include <stdlib.h>
#include <stdio.h>
#include "XLogicManager.h"
#include "GameManager.h"
#include "UserData.h"
#include "LevelDefine.h"
#include "UIManager.h"
#include "TXGUIHeader.h"
#include "Define.h"
#include "GameFontManager.h"
#include "CrossUtil.h"
#include "SpriteSeer.h"
#include "ParticleManager.h"
#include "GameAudioManager.h"
#include "BackPackLayer.h"
#include "MainMenuLayer.h"
#include "ItemBase.h"
#include "EquipmentItem.h"
#include "GameConfigFromLuaManager.h"
#include "HSJoystick.h"
#include "GameDataManager.h"
#include "TaskManager.h"
#include "Localization.h"
#include "CityListLayer.h"
#include "NotificationCenter.h"
#include "GMessage.h"
#include "SpriteElfConfigFromLua.h"
#include "CCLuaEngine.h"
#include "particle_manager.h"
#include "GameResourceManager.h"
#include "MessageBoxLayer.h"
#include "particle_manager.h"
#include "TipMgr.h"
#include "TalentLayer.h"
#include "LuaTinkerManager.h"
#include "UIPopupReward.h"
#include "CameraController.h"
#include "TutorialsManager.h"
#include "PopupRewardManager.h"
#include "StoryInstanceDirector.h"
#include "UIControlsApearControler.h"
#include "AlchemyUILayer.h"
#include "EquipmentUpgradeUILayer.h"
#include "CardUILayer.h"
#include "PvAIManager.h"
#include "ItemManager.h"
#include "TimeManager.h"
#include "UserData.h"
#include "TimeManager.h"
#include "LoginManager.h"
#include <math.h>
#include "CallNativeFuncManager.h"
#include "SkeletonAnimRcsManager.h"
#include "GuildManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NcSDKManager.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "TmLoginCommon.h"
#include "TmLoginManager.h"
#endif

using namespace TXGUI;

//////////////////////////////////////////////////////////////////////////
///  
///  
///

GameUILayer::GameUILayer()
	: m_pRedNamePublishNumLabel(NULL)
	, m_pRedNameTimerLabel(NULL)
	, m_pRedNameBackground(NULL)
	, m_pRedNamePublishTxtLabel(NULL)
    , m_firstPlayEffect(NULL)
	, m_bShowTipOnMarketButton(false)
	//, m_pRedNameContainer(NULL)
{
	m_bPuckered = false;
	m_pBtnShrink = NULL;
	last_effectiveness = 0 ;
	
}
 
GameUILayer::~GameUILayer()
{
	HideRedNamePanel();

	CCLOG("GameUILayer::~GameUILayer()");
	UIManager::sharedManager()->RemoveUILayout("MainMenu");
	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
	TimeManager::Get()->unregisterTimer(this,PB_ATTR_EQUIP_LVUP_CD);
	ParticleManagerX::purgeParticleManager();	
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&GameUILayer::GameUIUpdate), this);

    setFirstPlayEffect(NULL);
}

bool GameUILayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    GameAudioManager::sharedManager()->setBackgroundMusicVolume(CCUserDefault::sharedUserDefault()->getIntegerForKey("GameSettingSliderUpperKey",100));
   
    GameAudioManager::sharedManager()->setEffectsVolume( CCUserDefault::sharedUserDefault()->getIntegerForKey("GameSettingSliderDownKey",100));
	setTouchEnabled(true);

	UIManager *manager = UIManager::sharedManager();
	UILayout* testLayout = manager->CreateUILayoutFromFile("UIplist/MainMenu.plist",this,  "MainMenu");
    CCAssert(testLayout,"GameUILayer create ERROR");

	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonBag",
		menu_selector(GameUILayer::OnButtonBag), this);
    
    manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonTalent",
                                 menu_selector(GameUILayer::OnButtonTalent), this);
    /*
    no need. 
	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonSetting",
		menu_selector(GameUILayer::OnButtonSetting), this);
	*/
	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonShrink",
		menu_selector(GameUILayer::OnButtonShrink), this);
	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonUpgrade",
		menu_selector(GameUILayer::OnButtonUpgrade), this);
	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonAlchemy",
		menu_selector(GameUILayer::OnButtonAlchemy), this);
	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnBuyStaminaClicked",
		menu_selector(GameUILayer::OnBuyStaminaClicked), this);
	
	//manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonActivity",
		//menu_selector(GameUILayer::OnButtonActivity), this);
    
    manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonReWard",
                                 menu_selector(GameUILayer::OnButtonReWard), this);

	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonFairy",
		menu_selector(GameUILayer::OnButtonFairy), this);

	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonInstanceClick",
		menu_selector(GameUILayer::OnButtonInstanceClick), this);

	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonTask",
		menu_selector(GameUILayer::OnButtonTask), this);

	/*manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonRank",
		menu_selector(GameUILayer::OnButtonRank), this);	*/

	/*manager->RegisterMenuHandler("MainMenu","GameUILayer::OnButtonStrength",
		menu_selector(GameUILayer::OnButtonStrength), this);	*/
	
    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonFirstPayReward",
        menu_selector(GameUILayer::OnButtonFirstPayReward), this);

	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnHeadShotClick",
		menu_selector(GameUILayer::OnHeadShotClick), this);		

	manager->RegisterMenuHandler("MainMenu","GameUILayer::OnCardClick",
		menu_selector(GameUILayer::OnCardClick), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonRankClicked",
        menu_selector(GameUILayer::OnButtonRankClicked), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonMallClicked",
        menu_selector(GameUILayer::OnButtonMallClicked), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonBOSSCombatSettingClicked",
        menu_selector(GameUILayer::OnButtonBOSSCombatSettingClicked), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonPlatActivity",
        menu_selector(GameUILayer::OnButtonPlatActivity), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonStarsRewardClicked",
        menu_selector(GameUILayer::OnButtonStarsRewardClicked), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonTreasureExploreClicked",
        menu_selector(GameUILayer::OnButtonTreasureExploreClicked), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnButtonSociatyClicked",
        menu_selector(GameUILayer::OnButtonSociatyClicked), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnBuyDiamond",
        menu_selector(GameUILayer::onButtonCharge), this);

    manager->RegisterMenuHandler("MainMenu", "GameUILayer::OnVipPrivilegeClicked",
        menu_selector(GameUILayer::OnVipPrivilegeClicked), this);
	
	UILayout* layout = manager->getUILayout("MainMenu");
	
    //MapButtonPointsH(layout, "button_talent");
	//MapButtonPointsH(layout, "button_alchemy");
	//MapButtonPointsH(layout, "button_enchase");
	MapButtonPointsH(layout, "button_friendship",0);
	//MapButtonPointsW(layout, "button_rank");
	//MapButtonPointsW(layout, "button_setting");
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&GameUILayer::GameUIUpdate),this,1.0,false);

	if (layout)
	{




		//以右下角的按钮为基准，按钮动态展开代码并未删除
		UIButton* btn = layout->FindChildObjectByName<TXGUI::UIButton>("button_friendship");
		if (btn)
		{
			m_pBtnShrink = btn;
			m_ptDestShrink = btn->getWorldPosition();
			m_ptLocalDestShrink = btn->getPosition();
			mShrinkSize = btn->getMenuItemSprite()->getContentSize();
			mShrinkSize.width = mShrinkSize.width * TXGUI::UIManager::sharedManager()->getScaleFactor();
			mShrinkSize.height = mShrinkSize.height * TXGUI::UIManager::sharedManager()->getScaleFactor();
		}


		UIButton* bt = layout->FindChildObjectByName<UIButton>("headShotBtn");
		if(bt){
			m_expPotentiometer = CCControlPotentiometer::create(  
				"UI/ui_exp_frame.png",  
				"UI/ui_exp_bar.png",  
				"UI/ui_exp_blank.png");
			m_expPotentiometer->setTouchEnabled(false);
			m_expPotentiometer->setScale(UIManager::sharedManager()->getScaleFactor());
			m_expPotentiometer->setAnchorPoint(ccp(0,0));
			m_expPotentiometer->setPosition(bt->getPosition());
			m_expPotentiometer->setMinimumValue(0);
			m_expPotentiometer->getProgressTimer()->setReverseProgress(true);
			bt->getBindNode()->addChild(m_expPotentiometer,100);

			UIPicture* expMark = layout->FindChildObjectByName<UIPicture>("frameExpMark");
			if(expMark)
			{
				CCSprite * expMarkPic = new CCSprite();
				expMarkPic->initWithFile("UI/ui_exp_mark.png");
				expMarkPic->setScale(UIManager::sharedManager()->getScaleFactor());
				expMarkPic->setPosition(expMark->getPosition());
				bt->getBindNode()->addChild(expMarkPic,m_expPotentiometer->getZOrder()+1);
			}

			RefreshPotentiometer();
		}


		UIPicture* pic = layout->FindChildObjectByName<UIPicture>("iconHeadShot");
		if(pic)
		{

			ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
			const char* headIconName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", UserData::GetUserType(), "HeadPicture");
			int idx = getResourceIDByName(headIconName);

			CCPoint pt;
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(lockPic)
			{
				pic->setSprite(lockPic);
			}
		}


		//初始化时计算pvai btn是否需要提示
		UIButton* pvaiBtn = layout->FindChildObjectByName<TXGUI::UIButton>("button_communicate");
		if (pvaiBtn && UserData::GetUserLevel() >= 2)
		{
            int roleRank = PvAIManager::Get()->getHeroRank();
			int remainPvAICount = PvAIManager::Get()->getRemainCount();
			if (remainPvAICount > 0 && roleRank != 1)
			{				
				pvaiBtn->showTipIcon();
			}
			else
			{
				pvaiBtn->hideTipIcon();
			}
            if(roleRank == -1)
            {
                OnlineNetworkManager::sShareInstance()->SendGetPVAIInfo();
            }
		}

        // request starreward
        OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(NORMAL_INSTANCE_MIN, NORMAL_INSTANCE_MAX);
        OnlineNetworkManager::sShareInstance()->SendGetTreasureExploreInfo();
        GuildManager::Get()->SendGetGuildInfo();
        GuildManager::Get()->SendGetGuildMember();
		GuildManager::Get()->SendGetGuildRedEnvoInfoReq();
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameUILayer::OnNextFrame), this);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameUILayer::OnNextFrame), this, 15.0f, 1, 15.0f, false);

		// Note: 红名
		//UIContainer* pRedNameContainer = layout->FindChildObjectByName<UIContainer>("RedNameContainer");
		//m_pRedNameContainer = pRedNameContainer;
		//if (pRedNameContainer)
		{
			m_pRedNamePublishNumLabel = layout->FindChildObjectByName<TXGUI::UILabel>("pushlishNumLabel");
			m_pRedNameTimerLabel = layout->FindChildObjectByName<TXGUI::UILabel>("redNameTimerLabel");
			m_pRedNameBackground = layout->FindChildObjectByName<TXGUI::UIPicture>("redNameBackGround");
			m_pRedNamePublishTxtLabel = layout->FindChildObjectByName<TXGUI::UILabel>("pushlishTextLabel");
			showRedNamePanel(false);
		}
	}


	resetPlayerGoldLabel();
	resetPlayerDiamondLabel();
	resetPlayerReputationLabel();
	resetPlayerStaminaLabel();
    setPlayerLevel();
	setPlayerStrength();
	//checkEquipUpgradeIcon();
	showEquipFragCanComposeIcon();
	showFairyTipIcon();    
	showTargetTipIcon();
	showNewSkillTipIcon();
    ShowMarketButtonIcon();
    HideIndexButton();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Skill/SkillDataCenter.lua", "UpdateNewSkillCheckInfo");
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Talent/TalentMgr.lua", "InitTalentMgr");
	//MapButtonPointsH("button_bag",0);
	//MapButtonPointsH("button_fairy",1);
	//MapButtonPointsH("button_dailytask",2);

	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

#ifdef TargetForCoCoAppStore
    if(NcSDKManager::getInitIAPFlag() == false)
    {
        NcSDKManager::requestIAPInitData();
        NcSDKManager::setInitIAPFlag(true);
    }
    NcSDKManager::reSendVerifyFailedReceipt();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::SetFloatButtonHidden(false);
#endif
    
    return true;
}

void GameUILayer::RefreshPotentiometer()
{
	m_expPotentiometer->setMaximumValue(UserData::GetUserInfo().nextExp);
	m_expPotentiometer->setValue(UserData::GetUserExp());
}

void GameUILayer::OnNextFrame(float dt)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameUILayer::OnNextFrame), this);

    UILayout * layout = UIManager::sharedManager()->getUILayout("MainMenu");	
    if (!layout)
    {
        return;
    }
    UIButton * mainRewardBtn = layout->FindChildObjectByName<UIButton>("button_starReward");
    if(mainRewardBtn->isVisible() == false)
    {
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/starRewardUI.lua", "sendCheckInstanceStarRewardReq");
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/starRewardUI.lua", "setIsOpenRewardBtn", true);
    }
}

void GameUILayer::UpdateTaskUpRightIcon(std::string icon_name,eTaskState state /* = kType_TASK_UNKNOW */)
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	UIButton *pBtn = layout->FindChildObjectByName<UIButton>("button_instance");
	if(pBtn)
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
		int idx;
		CCPoint pt;

		bool inUnderCity = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_UNDERGROUND);
		bool inCityDefend = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_CITYDEFEND);
		bool inWorldBoss = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS);
		bool inBestFighter = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER);
		bool inGuildScene = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE);
        if (inUnderCity || inCityDefend || inWorldBoss || inBestFighter || inGuildScene)
		{
			LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "HideSpecialEntranceBtn");
			if (inBestFighter)
			{
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "OnEnterBestFighterEntrance");
			}

			idx = getResourceIDByName("map_ui_system_icon_FRAME_ICON_BACKTOTOWN");
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(lockPic)
			{
				pBtn->setIconSprite(lockPic);
			}

			if (inBestFighter)
			{
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "ShowBestFighterRankListBtn");
			}
		}
		else
		{
			idx = getResourceIDByName(icon_name.c_str());
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(lockPic)
			{
				if(icon_name.compare("map_ui_system_icon_FRAME_ICON_INSTANCE") != 0)
				{
					CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_taskguide");
					if(particle != NULL)
					{
						CCNode* child = lockPic->getChildByTag(100);
						if(child)
						{
							child->removeFromParentAndCleanup(true);
							child = NULL;
						}
						lockPic->addChild(particle,100,100);
						//particle->setPosition(CCPointZero);
						particle->setPosition(ccp(lockPic->getContentSize().width * 0.5,lockPic->getContentSize().height * 0.5));
					}
				}
				pBtn->setIconSprite(lockPic);
			}
		}

        // show combat setting button
        if(inWorldBoss)
        {
            LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "ShowWBSettingButton");

            // show tip
            if(!CCUserDefault::sharedUserDefault()->getBoolForKey("clicked_for_combatsetting"))
            {
                CallNativeFuncManager::getInstance()->ShowWBCombatSettingTip(true);
            }
            else
            {
                CallNativeFuncManager::getInstance()->ShowWBCombatSettingTip(false);
            }
        }

        // guild scene
        if(inGuildScene)
        {
            SpriteTransPoint* tranpoint = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getEliteTranspoint();
            if (tranpoint)
            {
                tranpoint->setAppearence(false);
            }
        }
	}
}

void GameUILayer::MapButtonPointsH(const char * szName,unsigned int tag)
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if (layout)
	{
		MapButtonPointsH(layout, szName,tag);
		ReCalMapButtonPos(true);
	}
}

void GameUILayer::MapButtonPointsW(const char * szName,unsigned int tag)
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if (layout)
	{
		MapButtonPointsW(layout, szName,tag);
		ReCalMapButtonPos(false);
	}
}

void GameUILayer::ReCalMapButtonPos(bool bHor)
{
	CCPoint tmpPos = m_ptDestShrink;
	CCPoint tmpLocalPos = m_ptLocalDestShrink;

	int count = 1;

	if (bHor)
	{
		float deltaDst = 10 * TXGUI::UIManager::sharedManager()->getScaleFactor();
		float cellWidth = mShrinkSize.width+deltaDst;
		for (map<unsigned int,TXGUI::UIButton*>::iterator iter = m_mapButtonIndexH.begin();
			iter != m_mapButtonIndexH.end(); iter++)
		{
			UIButton* pBtn = (*iter).second;
			if(m_pBtnShrink != pBtn)
			{
				CCPoint newPoint = ccp(tmpPos.x-count*cellWidth,tmpPos.y);
				pBtn->setWorldPosition(newPoint);
				CCPoint newLocalPoint = ccp(tmpLocalPos.x - count*cellWidth,tmpLocalPos.y);
				pBtn->setPosition(newLocalPoint);
				m_mapBttonPointH[pBtn] = newPoint;
				count ++;
			}
		}
	}
	else
	{
		float cellHeight = mShrinkSize.height ;
		for (map<unsigned int,TXGUI::UIButton*>::iterator iter = m_mapButtonIndexW.begin();
			iter != m_mapButtonIndexW.end(); iter++)
		{
			UIButton* pBtn = (*iter).second;
			if(m_pBtnShrink != pBtn)
			{
				CCPoint newPoint = ccp(tmpPos.x,tmpPos.y+count*cellHeight);
				pBtn->setWorldPosition(newPoint);
				CCPoint newLocalPoint = ccp(tmpLocalPos.x,tmpLocalPos.y+count*cellHeight);
				pBtn->setPosition(newLocalPoint);
				m_mapBttonPointW[pBtn] = newPoint;
				count ++;
			}
		}
	}
}

void GameUILayer::MapButtonPointsH(TXGUI::UILayout* layout, const char * szName,unsigned int tag)
{
    if (layout)
	{
		TXGUI::UIButton* btn = layout->FindChildObjectByName<TXGUI::UIButton>(szName);
		if (btn)
		{
			CCPoint pt = btn->getWorldPosition();
			this->m_mapBttonPointH[btn] = pt;
			this->m_mapButtonIndexH[tag] = btn;
			btn->setVisible(true);
		}
	}
}

/// pucker buttons to right bottom corner
void   GameUILayer::PuckerButtonsH(CCPoint ptDest)
{
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

	map<TXGUI::UIButton*, CCPoint>::iterator it = m_mapBttonPointH.begin();
	for( ; it != m_mapBttonPointH.end(); it++)
	{
		TXGUI::UIButton* btn = it->first;
		CCPoint pt0 = it->second;
		pt0.x  = ptDest.x - pt0.x;
		pt0.y  = ptDest.y - pt0.y;
		CCActionInterval* actionMove = CCMoveBy::create(0.3f, pt0);
		CCActionInterval* actionFade = CCFadeOut::create(0.3f);
		CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionFade,actionMove);
		CCCallFuncN* hideFun =  CCCallFuncN::create(this,callfuncN_selector(GameUILayer::HideButton));

		btn->getAnimationNode()->runAction(CCSequence::create(FirstAction, hideFun, NULL));
	}

	m_bPuckered =  true;
}

/// un pucker buttons from right bottom corner
void  GameUILayer::UnPuckerButtonsH()
{
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

	map<TXGUI::UIButton*, CCPoint>::iterator it = m_mapBttonPointH.begin();
	for( ; it != m_mapBttonPointH.end(); it++)
	{
		TXGUI::UIButton* btn = it->first;

		CCPoint ptDest = it->second;
		CCPoint pt0 = btn->getWorldPosition();
		pt0.x  = ptDest.x - pt0.x - 30;
		pt0.y  = ptDest.y - pt0.y;
		CCActionInterval* actionMove = CCMoveBy::create(0.3f, pt0);
		CCActionInterval* actionFade = CCFadeIn::create(0.3f);
		CCActionInterval* actionMoveX1 = CCMoveBy::create(0.1f, CCPoint(5, 0));
		CCActionInterval* actionMoveX2 = CCMoveBy::create(0.1f, CCPoint(25, 0));
		CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionMove, actionFade );
		CCCallFuncN* showFun =  CCCallFuncN::create(this,callfuncN_selector(GameUILayer::ShowButton));

		btn->getAnimationNode()->runAction(CCSequence::create(showFun, FirstAction, actionMoveX1, actionMoveX2, NULL));
	}

	m_bPuckered = false;
}


void GameUILayer::MapButtonPointsW(TXGUI::UILayout* layout, const char * szName,unsigned int tag)
{
	if (layout)
	{
		TXGUI::UIButton* btn = layout->FindChildObjectByName<TXGUI::UIButton>(szName);
		if (btn)
		{
			CCPoint pt = btn->getWorldPosition();
			this->m_mapBttonPointW[btn] = pt;
			this->m_mapButtonIndexW[tag] = btn;
			btn->setVisible(true);
		}
	}
}

/// pucker buttons to right bottom corner
void   GameUILayer::PuckerButtonsW(CCPoint ptDest)
{
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

	map<TXGUI::UIButton*, CCPoint>::iterator it = m_mapBttonPointW.begin();
	for( ; it != m_mapBttonPointW.end(); it++)
	{
		TXGUI::UIButton* btn = it->first;
		CCPoint pt0 = it->second;
		pt0.x  = ptDest.x - pt0.x;
		pt0.y  = ptDest.y - pt0.y;
		CCActionInterval* actionMove = CCMoveBy::create(0.3f, pt0);
		CCActionInterval* actionFade = CCFadeOut::create(0.3f);
		CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionFade,actionMove);
		CCCallFuncN* hideFun =  CCCallFuncN::create(this,callfuncN_selector(GameUILayer::HideButton));
		btn->getAnimationNode()->runAction(CCSequence::create(FirstAction, hideFun, NULL));
	}

	m_bPuckered = true;
}

/// un pucker buttons from right bottom corner
void  GameUILayer::UnPuckerButtonsW()
{
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

	map<TXGUI::UIButton*, CCPoint>::iterator it = m_mapBttonPointW.begin();
	for( ; it != m_mapBttonPointW.end(); it++)
	{
		TXGUI::UIButton* btn = it->first;

		CCPoint ptDest = it->second;
		CCPoint pt0 = btn->getWorldPosition();
		pt0.x  = ptDest.x - pt0.x;
		pt0.y  = ptDest.y - pt0.y + 30;
		CCActionInterval* actionMove = CCMoveBy::create(0.3f, pt0);
		CCActionInterval* actionFade = CCFadeIn::create(0.3f);
		CCActionInterval* actionMoveX1 = CCMoveBy::create(0.1f, CCPoint(0, -5));
		CCActionInterval* actionMoveX2 = CCMoveBy::create(0.1f, CCPoint(0, -25));
		CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionMove, actionFade );
		CCCallFuncN* showFun =  CCCallFuncN::create(this,callfuncN_selector(GameUILayer::ShowButton));

		btn->getAnimationNode()->runAction(CCSequence::create(showFun, FirstAction, actionMoveX1, actionMoveX2, NULL));
	}

	m_bPuckered = false;
}

void GameUILayer::HideButton(CCNode* btn)
{
	if (btn)
	{
		btn->setVisible(false);
	}
}

void GameUILayer::ShowButton(CCNode* btn)
{
	if (btn)
	{
		btn->setVisible(true);
	}
}

void GameUILayer::setVisible(bool visible)
{
	CCLayer::setVisible(visible);

	/// hide / show UILayerOut
	UIManager *manager = UIManager::sharedManager();
	UILayout* layout = manager->getUILayout("MainMenu");
	if (layout)
	{
		layout->setVisible(visible);		
	}

}

extern long long millisecondNow();

void GameUILayer::PuckerButtons()
{
	
	if (!m_bPuckered)
	{
		PuckerButtonsH(m_ptDestShrink);
		PuckerButtonsW(m_ptDestShrink);
		CCActionInterval* actionRotate = CCRotateTo::create(0.3f, 180.0f);
		m_pBtnShrink->getAnimationNode()->runAction(actionRotate);
	}
}

bool GameUILayer::IsOneButtonInExpandList(TXGUI::UIButton* pBtn)
{
	map<TXGUI::UIButton*, CCPoint>::iterator iter = m_mapBttonPointH.find(pBtn);
	if (iter != m_mapBttonPointH.end())
	{
		return true;
	}

	iter = m_mapBttonPointW.find(pBtn);
	if (iter != m_mapBttonPointW.end())
	{
		return true;
	}

	return false;
}

bool GameUILayer::ShrinkButtonClickRightNow(bool TrackUserSet /* = false */,bool bPushed /* = false */,CCCallFunc* pfnOkClicked /* = NULL */)
{
	if (TrackUserSet)
	{
		if (bPushed != m_bPuckered)
		{	
			return false;
		}
	}
	if (!m_bPuckered)
	{
        //disable button touch
        map<TXGUI::UIButton*, CCPoint>::iterator it = m_mapBttonPointH.begin();
    	for( ; it != m_mapBttonPointH.end(); it++)
    	{
    		TXGUI::UIButton* btn = it->first;
            btn->getMenuItemSprite()->setEnabled(false);
        }
        it = m_mapBttonPointW.begin();
    	for( ; it != m_mapBttonPointW.end(); it++)
    	{
    	    TXGUI::UIButton* btn = it->first;
            btn->getMenuItemSprite()->setEnabled(false);
    	}
		PuckerButtonsH(m_ptDestShrink);
		PuckerButtonsW(m_ptDestShrink);
		CCActionInterval* actionRotate = CCRotateTo::create(0.3f, 180.0f);
		CCSequence *seq = (CCSequence*)CCSequence::create(actionRotate,pfnOkClicked,NULL);	
		m_pBtnShrink->getAnimationNode()->runAction(seq);
	}
	else
	{
	    
		UnPuckerButtonsH();
		UnPuckerButtonsW();
		CCActionInterval* actionRotate = CCRotateTo::create(0.3f, 0);
		CCSequence *seq = (CCSequence*)CCSequence::create(actionRotate,pfnOkClicked,NULL);	
		m_pBtnShrink->getAnimationNode()->runAction(seq);
          //enable  button touch
        map<TXGUI::UIButton*, CCPoint>::iterator it = m_mapBttonPointH.begin();
    	for( ; it != m_mapBttonPointH.end(); it++)
    	{
    		TXGUI::UIButton* btn = it->first;
            btn->getMenuItemSprite()->setEnabled(true);
        }
        it = m_mapBttonPointW.begin();
    	for( ; it != m_mapBttonPointW.end(); it++)
    	{
    	    TXGUI::UIButton* btn = it->first;
            btn->getMenuItemSprite()->setEnabled(true);
    	}
	}

	return true;
}

void GameUILayer::OnButtonShrink(CCObject* sender)
{
	//change to customService

	//CCLOG("GameUILayer::OnButtonShrink");

	//static long  long nLastClickTime = 0;
 //   long long vTime = millisecondNow();
	//if (vTime - nLastClickTime < 0.5 * 1000)
	//{
	//	return;
	//}

	//nLastClickTime = vTime;

	//ShrinkButtonClickRightNow();
}

void GameUILayer::OnButtonStrength(CCObject* sender)
{
	stopHeroMove();
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
		layer->openBackPack(true);
		layer->openAttributeBoard(NULL);
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/backPackLayer.lua","showUserAttributes");
	}
}

void GameUILayer::OnButtonBag(CCObject *sender)
{
	CCLOG("GameUILayer::OnButtonBag");
	stopHeroMove();
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
		layer->onBagButtonClicked();
	}
}
 
void GameUILayer::OnBuyStaminaClicked(CCObject *sender)
{
	stopHeroMove();
	MainMenuLayer* layer = GetMainMenuLayer();
	if(layer)
	{
		 layer->ShowMarketLayer(true,1);
    }
}

void GameUILayer::OnButtonTalent(cocos2d::CCObject *sender)
{
    CCLOG("gameUIlayer on btton\n");
	stopHeroMove();
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
        mLayer->ShowTalentLayer();
	}
    //CameraController::sharedCameraController()->shakeOnly(0.05, 10);
}


void GameUILayer::OnButtonSetting(CCObject *sender)
{
   /*
	CCLOG("GameUILayer::OnButtonSetting");
	stopHeroMove();
	string toJoy = Localizatioin::getLocalization("T_operator_text_1");
	string toMouse = Localizatioin::getLocalization("T_operator_text_2");

	Move_Type type = GameDataManager::Get()->getHeroMoveType();

	switch (type)
	{
		case MOVE_BY_JOY:
			ShowMessage(toMouse.c_str());
			GameDataManager::Get()->setHeroMoveType(MOVE_BY_MOUSE);
			break;

		case MOVE_BY_MOUSE:
			ShowMessage(toJoy.c_str());			
			GameDataManager::Get()->setHeroMoveType(MOVE_BY_JOY);
			break;
            
		default:
			break;
	}*/
}

void GameUILayer::OnButtonUpgrade(CCObject* sender)
{
	stopHeroMove();
	//OnlineNetworkManager::sShareInstance()->sentEquipUpgradeMessage();
	int tag = MainMenuLayer::getLayerTagByName("EquipmentUpgradeUILayer");
	MainMenuLayer::showUILayerByTag<EquipmentUpgradeUILayer>(tag);
}


//void GameUILayer::OnButtonActivity(CCObject* sender)
//{
//    stopHeroMove();
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		mLayer->ShowPPVELayer();
//	}
//}

void GameUILayer::OnButtonFairy(CCObject* sender)
{
	//GameAudioManager::sharedManager()->playEffect(350001,false);
	//stopHeroMove();
	//SpriteElfConfigFromLuaManager::getInstance()->CreateSpriteElfLayer();
}

void GameUILayer::OnButtonReWard(cocos2d::CCObject *sender)
{
	stopHeroMove();
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowPVPLayer();
	}
}

void GameUILayer::OnButtonInstanceClick(CCObject* sender)
{
	stopHeroMove();

	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_UNDERGROUND))
	{
		InstanceManager::Get()->onLeaveSpecialEntrance(ENTERANCE_UNDERGROUND);
		return;
	}

	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_CITYDEFEND))
	{
		InstanceManager::Get()->onLeaveSpecialEntrance(ENTERANCE_CITYDEFEND);
		return;
	}

	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS))
	{
		InstanceManager::Get()->onLeaveSpecialEntrance(ENTERANCE_WORLDBOSS);
		return;
	}

	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER))
	{
		InstanceManager::Get()->onLeaveSpecialEntrance(ENTERANCE_BESTFIGHTER);
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "CancelBestFighter");
		return;
	}

    if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE))
    {
        InstanceManager::Get()->onLeaveSpecialEntrance(ENTERANCE_GUILDSCENE);
        return;
    }
	

	BattleCoverLayer * layer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
	layer->openBattleCoverLayer(COVER_AUTOSEARCH);

	TaskManager::getInstance()->TackInstanceIconClickEvent();
}

void GameUILayer::OnButtonRankClicked(CCObject* sender)
{
    GameAudioManager::sharedManager()->playEffect(350001,false);
    CallNativeFuncManager::getInstance()->AddIndexLayerToMainScene(0);
}

void GameUILayer::OnButtonMallClicked(CCObject* sender)
{
    GameAudioManager::sharedManager()->playEffect(350001,false);
    OpenMarketLayer();
}
 
void GameUILayer::OnButtonBOSSCombatSettingClicked(CCObject* sender)
{
    CallNativeFuncManager::getInstance()->ShowCombatSettingLayer(true);
    CCUserDefault::sharedUserDefault()->setBoolForKey("clicked_for_combatsetting", true);
    CallNativeFuncManager::getInstance()->ShowWBCombatSettingTip(false);
}

void GameUILayer::OnButtonPlatActivity(CCObject* sender)
{
    OpenPlatAcitivityLayer();
}

void GameUILayer::OnButtonStarsRewardClicked(CCObject* sender)
{
   /* int size = InstanceManager::Get()->curInstanceList.size();
    int maxInstanceId = 0;
    if (size != 0)
    {
        maxInstanceId = InstanceManager::Get()->curInstanceList[size - 1].id;
    }*/

    LuaTinkerManager::Get()->callLuaFunc<void,int>("Script/UILayer/starRewardUI.lua", "showStarRewardUI", 
    InstanceManager::Get()->getMaxNormalInstanceId());
}

void GameUILayer::OnButtonTreasureExploreClicked(CCObject* sender)
{
    GameAudioManager::sharedManager()->playEffect(350001,false);
    CallNativeFuncManager::getInstance()->ShowTreasureExploreLayer(true);
}

void GameUILayer::OnButtonSociatyClicked(CCObject* sender)
{
    GameAudioManager::sharedManager()->playEffect(350001,false);
    stopHeroMove();
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        // clear reopen guildinfolayer flag
        if(GuildManager::Get()->getIsGuildInfoNeedOpen() == true)
        {
            GuildManager::Get()->setIsGuildInfoNeedOpen(false);
        }

        // open index guild layer
        int highGid = UserData::GetUserInfo().m_guildId.getHighID();
        int Lowid = UserData::GetUserInfo().m_guildId.getLowID();
        if(Lowid == 0 && highGid == 0)
        {
            mLayer->ShowGuildListLayer(true);
        }
        else
        {
            mLayer->ShowGuildInfoLayer(true);
        }
    }
}

void GameUILayer::OnButtonFirstPayReward(CCObject* sender)
{
    OpenFirstChargeLayer();
}

void GameUILayer::onButtonCharge(CCObject* sender)
{
    OpenChargeLayer();
}

void GameUILayer::OnVipPrivilegeClicked(CCObject* sender)
{
    OpenVipLayer();
}

void GameUILayer::OnPopupRewardLayerOkBtnClick(CCObject* pObject)
{
	unsigned int tutorial_id = 120;
	//TutorialsManager::Get()->StartOneTutorial(tutorial_id,kTutorialPopupReward);		
	TutorialsManager::Get()->StartOneTutorial(tutorial_id,kTutorialUI);		
}

void GameUILayer::OnButtonTask(CCObject* sender)
{	
	stopHeroMove();
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		TaskListLayer* pTaskListLayer = mLayer->GetTaskListLayer();
		if (pTaskListLayer == 0)
		{
			mLayer->CreateTaskListLayer();
			GameAudioManager::sharedManager()->playEffect(350001,false);
		}
		else
		{
			pTaskListLayer->ShowTaskListLayer(true);
			GameAudioManager::sharedManager()->playEffect(350001,false);
		}
	}
}

void GameUILayer::OpenMarketLayer()
{
    stopHeroMove();
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(true);
    }
}

void GameUILayer::OpenPlatAcitivityLayer()
{
    stopHeroMove();
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowPlatActivityLayer(true);
    }
}

void GameUILayer::OpenFirstChargeLayer()
{
    stopHeroMove();
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowFirstChargeLayer(true);
    }
}

void GameUILayer::OpenChargeLayer()
{
    stopHeroMove();
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
         mLayer->ShowChargeLayer(true);
    }
}

void GameUILayer::OpenVipLayer()
{
    stopHeroMove();
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowChargeLayer(true, 1);
    }
}


void GameUILayer::OnButtonRank(CCObject* sender)
{
	stopHeroMove();
	//MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	//if (mLayer)
	//{
		CityListLayer* cityListLayer = MainMenuLayer::getUILayerByTag<CityListLayer>(MainMenuLayer::getLayerTagByName("CityListLayer"));
		cityListLayer->openCityList();		
	//}
}

void GameUILayer::OnCardClick(CCObject* obj)
{
	//ShowCardUILayer();
	int tag = MainMenuLayer::getLayerTagByName("CardUILayer");
	MainMenuLayer::showUILayerByTag<CardUILayer>(tag);
}

void GameUILayer::OnHeadShotClick(CCObject* sender)
{
	stopHeroMove();
	//std::string msg = Localizatioin::getLocalization("S_Del_Role_Content");			
	//MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&GameUILayer::OnDelRoleMessageOK), NULL, MB_YESNO);
	int tag = MainMenuLayer::getLayerTagByName("UserInfoUILayer");
	MainMenuLayer::showUILayerByTag<CCLayer>(tag);
}

void GameUILayer::OnDelRoleMessageOK(CCObject* pObject)
{
	OnlineNetworkManager::sShareInstance()->SendDelRoleReqMessage();
}

void GameUILayer::OnButtonAlchemy(CCObject* sender)
{
	stopHeroMove();
	int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
	MainMenuLayer::showUILayerByTag<AlchemyUILayer>(tag);
}

void GameUILayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_E_ENTER_FIRST_LOGIN_STATE :
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "EnterFristAppearUI");
		break;
	case GM_ATTR_COIN_UPDATE:
		resetPlayerGoldLabel();
		break;
	case GM_ATTR_SP_DIAMOND_UPDATE:
		resetPlayerDiamondLabel();
		break;
	case GM_ATTR_STAMINA_UPDATE:
		resetPlayerStaminaLabel();
		break;
	case GM_ATTR_REPUTATION_UPDATE:
		resetPlayerReputationLabel();
		break;
	case GM_ATTR_HERO_LEVEL_UPDATE:
		{
			setPlayerLevel();
			checkEquipUpgradeIcon();
			break;
		}
	case GM_ATTR_EQUIP_LVUP_CD:
	case GM_ATTR_EQUIP_LVUP_UESED_TIMES:
		checkEquipUpgradeIcon();
		break;
	case GM_ATTR_ABILITY_ALL_UPDATE:
		{
			USERINFO info = UserData::GetUserInfo();
			 int num = info.effectiveness - last_effectiveness ;
			 char  Msg[100];
			 if(num >0)
			 {
				 snprintf(Msg ,100 ,"+%d",num);
				  ShowMessage(Msg ,ccc3(0,255,0) ,1);
			 }
			 else if(num < 0 )
			 {
				 snprintf(Msg ,100 ,"-%d",-num);
				 ShowMessage(Msg ,ccc3(255,0,0) ,1);
			 }
			
			setPlayerStrength();
		}
		
		break;
	case GM_ATTR_FRAG_CAN_CONPOSE:
	case GM_ATTR_NEW_EQUIP_CHANGE:
		{
			showEquipFragCanComposeIcon();
			break;
		}
	case GM_ATTR_FARIY_FLAG_ENOUGH:
	case GM_ATTR_NEW_FAIRY_INFO:
		{
			showFairyTipIcon();
			break;
		}
	case GM_UPDATE_TARGET_ICON:
		{
			showTargetTipIcon();
			break;
		}
    case GM_ATTR_SPRITE_EXTRACT :
        {
            ShowMarketButtonIcon();
            break;
        }

    case GM_UPDATE_SHOP_REFRESH_TIMES :
        {
            ShowMarketButtonIcon();
            break;
        }
	case GM_UPDATE_MARKET_TIME_INFO:
	case GM_ATTR_VIP_LEVEL_CHANGE:
		{
			ShowMarketButtonIcon();
			break;
		}
	case GM_ATTR_EXP_UPDATE:
		{
			RefreshPotentiometer();
			break;
		}
	case GM_GET_NEW_SKILL:
		{
			showNewSkillTipIcon();
		}
	}
}


void GameUILayer::showEquipFragCanComposeIcon()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UIButton* bt = layout->FindChildObjectByName<UIButton>("button_bag");
		if(bt)
		{
			USERINFO info = UserData::GetUserInfo();
			bool hasNewEquip = ItemManager::Get()->checkNewEquipInfo();
			if(info.b_hasEnoughEquipFrag || hasNewEquip)
			{
				bt->showTipIcon();
			}
			else
			{
				bt->hideTipIcon();
			}
		}
	}
}

void GameUILayer::showFairyTipIcon()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UIButton* bt = layout->FindChildObjectByName<UIButton>("button_fairy");
		if(bt)
		{
			USERINFO info = UserData::GetUserInfo();
			bool hasNewFairy = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Fairy/FairyLuaInterface.lua","G_hasNewFairy");
			//if(info.b_hasEnouchFairyFlag || hasNewFairy)
			if(hasNewFairy)
			{
				bt->showTipIcon();
			}
			else
			{
				bt->hideTipIcon();
			}
		}
	}
}

void GameUILayer::showNewSkillTipIcon()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UIButton* bt = layout->FindChildObjectByName<UIButton>("headShotBtn");
		if(bt)
		{
			unsigned int skill_id = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Skill/SkillDataCenter.lua", "getNewSkillId");
			if(skill_id != 0)
			{
				bt->showTipIcon();
			}
			else
			{
				bt->hideTipIcon();
			}
		}
	}

}

void GameUILayer::showTargetTipIcon()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UIButton* bt = layout->FindChildObjectByName<UIButton>("button_target");
		if(bt)
		{
			USERINFO info = UserData::GetUserInfo();
			if(info.b_hasTargetIconFlag)
			{
				bt->showTipIcon();
			}
			else
			{
				bt->hideTipIcon();
			}
		}
	}
}
void GameUILayer::ShowMarketButtonIcon()
{
    UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
    if(layout)
    {
        UIButton* bt = layout->FindChildObjectByName<UIButton>("button_mall");
        if(bt)
        {
            USERINFO info = UserData::GetUserInfo();

            long junior_time = info.m_junior_explore_time;
            long senior_time = info.m_senior_explore_time;
            long nowTime = TimeManager::Get()->getCurServerTime();
            unsigned int freeTimes = info.m_refreshTimes;

			m_bShowTipOnMarketButton = !(MarketManager::Get()->isShopedCurrentVipPacks());

            if (nowTime > junior_time ||
                nowTime > senior_time ||
                info.m_green_extract_times > 0 ||
                freeTimes > 0 ||
				m_bShowTipOnMarketButton)
            {
                bt->showTipIcon();
            } 
			else
            {
                bt->hideTipIcon();
            }
        }
    }
}

void GameUILayer::ShowIconEffect()
{
    return;
    UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
    if(layout)
    {
        UIButton* first_btn = layout->FindChildObjectByName<UIButton>("button_firstpay");
        if(first_btn && first_btn->isVisible() == true)
        {
            SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(444);
            EffectSprite* effect = ParticleManager::Get()->createEffectSprite(444,"",false);
            if (effect)
            {
                this->addChild(effect);
                effect->setPosition(first_btn->getWorldPosition());
                effect->SetAnim(kType_Play, 1, true);
                setFirstPlayEffect(effect);
                effect->getRoot()->setScale(UIManager::sharedManager()->getScaleFactor());
            }
        }

        UIButton* charge_btn = layout->FindChildObjectByName<UIButton>("buyDiamond");
        if(charge_btn)
        {
            SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(444);
            EffectSprite* effect = ParticleManager::Get()->createEffectSprite(444,"",false);
            if (effect)
            {
                this->addChild(effect);
                effect->setPosition(charge_btn->getWorldPosition());
                effect->SetAnim(kType_Play, 1, true);
                effect->getRoot()->setScale(UIManager::sharedManager()->getScaleFactor());
            }
        }

    }
}

// 隐藏一些指定的按钮
void GameUILayer::HideIndexButton()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int channelId = LoginManager::Get()->getChannelID();
    if (channelId != 2)
    {
        UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
        if(layout)
        {
            UIButton* bt = layout->FindChildObjectByName<UIButton>("button_platActivity");
            if (bt)
            {
                bt->setVisible(false);
            }
        }
    }
#endif
}

void GameUILayer::resetPlayerDiamondLabel()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UILabel *pLabel = layout->FindChildObjectByName<UILabel>("diamond");
		if(pLabel)
		{
			unsigned int diamond = UserData::GetDiamond();
			//const char* str = MainMenuLayer::GetNumByFormat(diamond);
			char str[20];
			sprintf(str,"%u",diamond);
			if(str)
			{
				pLabel->setString(str);
			}
		}
	}
};

void GameUILayer::resetPlayerGoldLabel()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UILabel *pLabel = layout->FindChildObjectByName<UILabel>("coin");
		if(pLabel)
		{
			unsigned int gold = UserData::GetGold();
			const char* str = MainMenuLayer::GetNumByFormat(gold);
			if(str)
			{
				pLabel->setString(str);
			}
		}
	}
}

void GameUILayer::resetPlayerReputationLabel()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UILabel *pLabel = layout->FindChildObjectByName<UILabel>("reputationText");
		if(pLabel)
		{
			USERINFO info = UserData::GetUserInfo();
			unsigned int reputation = info.m_reputation;
			stringstream ss;
			ss << reputation;
			string repuStr = ss.str();

			const char* str = repuStr.c_str();
			if(str)
			{
				pLabel->setString(str);
			}
		}
	}
}

void GameUILayer::resetPlayerStaminaLabel()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UILabel *pLabel = layout->FindChildObjectByName<UILabel>("staminaText");
		if(pLabel)
		{
			USERINFO info = UserData::GetUserInfo();
			unsigned int stamina = info.m_stamina;
			char spNum[20];
			sprintf(spNum,"%d/100",stamina);
			pLabel->setString(spNum);
		}
	}
}

void GameUILayer::stopHeroMove()
{
	SpriteSeer* hero = GameManager::Get()->getHero();
	if (hero)
	{
		hero->moveToTarget(hero->getPosition(), hero->getPosition());
	}

	BattleCoverLayer * coverLayer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
	if (coverLayer)
	{
		coverLayer->closeBattleCoverLayer(NULL);
	}
}

void GameUILayer::setPlayerLevel()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UILabel* label = layout->FindChildObjectByName<UILabel>("playerlevel");
		if(label)
		{
			unsigned int level = UserData::GetUserLevel();
			char levelStr[10];
			sprintf(levelStr,"Lv%d",level);
			label->setString(levelStr);
		}
	}
}

void GameUILayer::setPlayerStrength()
{
	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	if(layout)
	{
		UILabelAtlas* label = layout->FindChildObjectByName<UILabelAtlas>("strengthlabel");
		if(label)
		{
			USERINFO info = UserData::GetUserInfo();
			unsigned int num = info.effectiveness;
			last_effectiveness = num ;
			char str[10];
			sprintf(str,"%d",num);
			label->setString(str);
		}
	}
}

void GameUILayer::showSpecialButtonEffect()
{
	UIManager *manager = UIManager::sharedManager();
	UILayout* layout = manager->getUILayout("MainMenu");
	CCNode* pTalentBtn = (layout->FindChildObjectByName<TXGUI::UIButton>("button_talent"))->getAnimationNode();
	//CCNode* pSkillBtn = (layout->FindChildObjectByName<TXGUI::UIButton>("button_skillupgrade"))->getAnimationNode();
	CCNode* pEquipBtn = (layout->FindChildObjectByName<TXGUI::UIButton>("button_equipupgrade"))->getAnimationNode();

	if (pTalentBtn)
	{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_transformelite");
		if(particle != NULL)
		{
			CCNode* child = pTalentBtn->getChildByTag(10100);
			if(child)
			{
				child->removeFromParentAndCleanup(true);
				child = NULL;
			}
			pTalentBtn->addChild(particle,100,10100);
		}
	}	

	/*if (pSkillBtn)
	{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_transformelite");
		if(particle != NULL)
		{
			CCNode* child = pSkillBtn->getChildByTag(10101);
			if(child)
			{
				child->removeFromParentAndCleanup(true);
				child = NULL;
			}
			pSkillBtn->addChild(particle,100,10101);
		}
	}*/

	if (pEquipBtn)
	{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_transformelite");
		if(particle != NULL)
		{
			CCNode* child = pEquipBtn->getChildByTag(10102);
			if(child)
			{
				child->removeFromParentAndCleanup(true);
				child = NULL;
			}
			pEquipBtn->addChild(particle,100,10102);
		}
	}
}

void GameUILayer::ShowAutoButtons(bool bShow)
{
	for (map<TXGUI::UIButton*, CCPoint>::iterator iter = m_mapBttonPointH.begin();
		iter != m_mapBttonPointH.end(); iter++)
	{
		(*iter).first->setVisible(bShow);
	}


	for (map<TXGUI::UIButton*, CCPoint>::iterator iter = m_mapBttonPointW.begin();
		iter != m_mapBttonPointW.end(); iter++)
	{
		(*iter).first->setVisible(bShow);
	}

	//m_pBtnShrink->setVisible(bShow);
}

void GameUILayer::onUpdateRemainTime(long remainTime)
{
	if(remainTime == 0)
	{
		checkEquipUpgradeIcon();
	}

	if (remainTime > 0)
	{
		if (m_pRedNameTimerLabel)
		{
			//int dayNum = floor((double)(remainTime / 86400));
			//int leftTime = fmod((double)(remainTime),double(86400));
			int hourNum = floor((double)(remainTime / 3600));
			int leftTime = fmod((double)(remainTime),double(3600));
			int minuterNum = floor((double)(leftTime / 60));
			int secNum = fmod((double)(leftTime),(double)60);

			char timeStr[100];
			if (hourNum >= 100)
			{
				if (hourNum > 999)
				{
					hourNum = 999;
				}
				sprintf(timeStr,"%03d:%02d:%02d", hourNum,minuterNum,secNum);
			}
			else
			{
				sprintf(timeStr,"%02d:%02d:%02d", hourNum,minuterNum,secNum);
			}
			
			m_pRedNameTimerLabel->setString(timeStr);
		}
	}
	else
	{
		showRedNamePanel(false);
		SpriteSeer* pHero = GameManager::Get()->getHero();
		if (pHero)
		{
			pHero->ShowNameLabelColor(false);
		}
	}
}

void GameUILayer::checkEquipUpgradeIcon()
{
	return;
	bool equipCdFlag = true; 
	bool equipLevelFlag = false; 

	long nowTime = TimeManager::Get()->getCurServerTime();
	USERINFO info = UserData::GetUserInfo();
	long remainTime = info.m_equipLvUpCd - nowTime;
	remainTime = (remainTime > 0) ? remainTime : 0;

	if(remainTime == 0 && info.m_equipLvFlag != 0)
	{
		info.m_equipLvFlag = 0;
		UserData::SetUserInfo(info);
	}
	if(info.m_equipLvFlag == 0)
	{
		equipCdFlag = true;
	}
	else
	{
		if(TimeManager::Get()->hasTimer(PB_ATTR_EQUIP_LVUP_CD))
		{
			if(!TimeManager::Get()->hasObserver(this,PB_ATTR_EQUIP_LVUP_CD))
			{
				TimeManager::Get()->attachTimer(this, PB_ATTR_EQUIP_LVUP_CD);
			}
		}
		else
		{
			TimeManager::Get()->registerTimer(this, PB_ATTR_EQUIP_LVUP_CD, remainTime + nowTime);
		}
		equipCdFlag = false;
	}

	
	unsigned int playerLevel = UserData::GetUserLevel();
	// 玩家装备
	for(int i= 1;i<6; i++)
	{
		int pos = i + kItemHeroEquip;
		BackPackItem* backPack = ItemManager::Get()->findItemByPos(pos);
		if(backPack && !backPack->isEmpty && !backPack->isLocked)
		{
			if(backPack->itemLevel < playerLevel )
			{
				equipLevelFlag = true;
				break;
			}
		}
	}
	// 精灵装备
	for(int i= 1;i<6; i++)
	{
		int pos = i + kItemFairyEquip;
		BackPackItem* backPack = ItemManager::Get()->findItemByPos(pos);
		if(backPack && !backPack->isEmpty && !backPack->isLocked)
		{
			if(backPack->itemLevel < playerLevel )
			{
				equipLevelFlag = true;
				break;
			}
		}
	}

	UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
	UIButton *pBtn = layout->FindChildObjectByName<UIButton>("button_equipupgrade");
	if(pBtn)
	{
		if(equipCdFlag && equipLevelFlag)
		{
			pBtn->showTipIcon();
		}
		else
		{
			pBtn->hideTipIcon();
		}
	}

	
}

void  GameUILayer::GameUIUpdate(float dt) 
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "updateGameUI",dt);
}

void GameUILayer::ShowRedNamePanel()
{
	//if (m_pRedNameContainer)
	{
		//m_pRedNameContainer->setVisible(true);
		showRedNamePanel(true);
		if (m_pRedNamePublishNumLabel)
		{
			char buffer[100];
			sprintf(buffer,"%d%%",UserData::GetUserInfo().m_pvpRed);
			m_pRedNamePublishNumLabel->setString(buffer);
		}
		if (m_pRedNameTimerLabel)
		{
			RenewRedNameTimer();
		}
	}
}

void GameUILayer::showRedNamePanel(bool bShow)
{
	if (m_pRedNameBackground)
	{
		m_pRedNameBackground->setVisible(bShow);
	}	
	if (m_pRedNamePublishTxtLabel)
	{
		m_pRedNamePublishTxtLabel->setVisible(bShow);
	}	
	if (m_pRedNamePublishNumLabel)
	{
		m_pRedNamePublishNumLabel->setVisible(bShow);
	}
	if (m_pRedNameTimerLabel)
	{
		m_pRedNameTimerLabel->setVisible(bShow);
	}	
}

void GameUILayer::HideRedNamePanel()
{
	//if (m_pRedNameContainer)
	{
		//m_pRedNameContainer->setVisible(false);
		showRedNamePanel(false);
		TimeManager::Get()->unregisterTimer(this,TIMER_USER_RED_NAME_REMAIN);
	}
}

void GameUILayer::RenewRedNameTimer()
{
	long nowTime = TimeManager::Get()->getCurServerTime();
	if (UserData::GetUserInfo().m_pvpRedTimer >= nowTime)
	{
		if(TimeManager::Get()->hasTimer(TIMER_USER_RED_NAME_REMAIN))
		{
			TimeManager::Get()->renewTimer(TIMER_USER_RED_NAME_REMAIN, UserData::GetUserInfo().m_pvpRedTimer);
			if (!TimeManager::Get()->hasObserver(this, TIMER_USER_RED_NAME_REMAIN))
			{
				TimeManager::Get()->attachTimer(this, TIMER_USER_RED_NAME_REMAIN);
			}
		}
		else
		{
			TimeManager::Get()->registerTimer(this, TIMER_USER_RED_NAME_REMAIN, UserData::GetUserInfo().m_pvpRedTimer);
		}
	}
}