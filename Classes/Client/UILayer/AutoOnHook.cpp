//
//  AutoOnHook.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-5-20.
//
//


#include "PVPMessage.h"
#include "AutoOnHook.h"
#include "GameResourceManager.h"
#include "Localization.h"
#include "GameFontManager.h"
#include "GMessage.h"
#include "PVPMessage.h"
#include "ItemManager.h"
#include "particle_manager.h"
#include "GameConfigFromLuaManager.h"
#include "TipMgr.h"
#include "LuaTinkerManager.h"
#include "MainMenuLayer.h"
#include "TaskManager.h"
#include "UserData.h"
#include "TimeManager.h"
#include "OnlineNetworkManager.h"
#include "XLogicManager.h"
#include "WaitingLayerManager.h"
#include "GameConfigFromLuaManager.h"
#include "MessageBoxLayer.h"
#include "InterfaceManager.h"

using namespace TXGUI;
bool g_rcv_battle_rsp=false;
std::vector<autoOnHookReward> g_rewards;
static const char * uiLayoutNames[]={
"AutoOnHookUI",
"autoFightOfJingYing",
"autoFightOfUnder",
};
static const int TAG_START_BUTTON = 100;
static bool timer_wrong = false;
static bool receiveBattleRsp = false;//should have reward
unsigned int missing_ret;
unsigned int missing_instance_id ;
unsigned int missing_type;

AutoOnHook::AutoOnHook()
{
	haveParentPanel = false;
    inithook();	
}

AutoOnHook::AutoOnHook(bool fromInstancePanel)
{
	haveParentPanel = fromInstancePanel;
	inithook();	
}

void AutoOnHook::inithook()
{
	this->m_pListV = NULL;
	 this->m_FuBenIcon = NULL;
	 this->m_FuBenName =  NULL;
	 this->m_MasterFuBenName = NULL;
	 this->m_pIconButtonLayer = NULL;
	 
	 
	 this->m_pLabelCDNumber = NULL;
	 this->m_pLabelCDText = NULL;
	 this->m_pCDFrame = NULL;
	 this->m_pProgressBg = NULL;
	 this->m_pLabelSweep = NULL;
	 this->m_pLabelCue = NULL;
	 this->m_pProgressTimer = NULL;
	
	 this->m_dt  = 0;
	 this->m_cnt = 60;
	 this->m_musicTime = 0;
	 
	 this->m_pStartBtn = NULL;
	 this->m_pSpeedUpBtn = NULL;
	 this->m_pStopBtn = NULL;
	 
	 this->m_pTurnBtn1 = NULL;
	 this->m_pTurnBtn2 = NULL;
	 this->m_pTurnBtn3 = NULL;
	 
	 this->m_pTurnText1 = NULL;
	 this->m_pTurnText2 = NULL;
	 
	 this->m_pTurnSami1 = NULL;
	 this->m_pTurnSami2 = NULL;
	 this->m_pTurnSami3 = NULL;
	 
	 this->m_pTurnFrame = NULL;
	 
	 this->m_turnId = 1;
	 this->m_instanceId = 0;
	 
	 this->m_bCloseLayerCallBack = false;
	 
	 this->m_pStartLabel = NULL;
	 this->m_pSpeedupLabel = NULL;
	 this->m_pStopLabel = NULL;
	 this->m_pUseDiamand = NULL;
	 this->m_bStopSel = false;
	 
	 this->m_pCloseBtn = NULL;
	 
	 this->m_num_diamand = 0;
	 this->m_num_coin = 0;
	 this->m_num_exploit = 0;
	 this->m_num_exp = 0;
	 
	 this->m_remainTime = 0;
		
	 instanceListLayout = NULL;
	 m_itemMap.clear();
	
	 this->chapterList = NULL;
	 m_currentTag = 0;
	 instanceType = INSTANCE_TYPE_NOTHING;
	 m_currentChapterId=-1;
	 jingYingLayout = NULL;
	 m_state = -1;
	 coolDownEndTime = -1;
	 NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
	
	 instanceUnderLayout = NULL;
	 chapterLabel = NULL;

	 this->m_rewards.clear();
	 this->m_isWaitingReward = false;
	 this->m_isShownReward = false;
}

AutoOnHook::~AutoOnHook()
{
	ShowChatBar(true);

    removeLayout();
    unregisterTime();
    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	if(haveParentPanel)
	{
		InstanceListLayer * instanceListLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
		if(instanceListLayer){
			instanceListLayer->setVisible(true);
		}
		ShowChatBar(false);
	}
}

void AutoOnHook::unregisterTime()
{
    CCLOG("%s", __FUNCTION__);
    TimeManager::Get()->unregisterTimer(this, PB_ATTR_AB_NORMAL_MAP_CD);
	TimeManager::Get()->unregisterTimer(this, PB_ATTR_AB_SPECIAL_MAP_CD);
	TimeManager::Get()->unregisterTimer(this, PB_ATTR_AB_UGCITY_MAP_CD);
    m_initremainTime = 0;
}

void AutoOnHook::removeLayout()
{
  for(int i = 0; i < sizeof(uiLayoutNames)/sizeof(uiLayoutNames[0]); i ++)
       UIManager::sharedManager()->RemoveUILayout(uiLayoutNames[i]);
    UIManager::sharedManager()->RemoveUILayout("UseDiamand_AutoHook");

}

UILayout* AutoOnHook::getCurLayout()
{
    switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
        return instanceListLayout;
    case INSTANCE_TYPE_JINGYING:
        return jingYingLayout;
	case INSTANCE_TYPE_DUNGEON:
        return instanceUnderLayout;
    default:
        return NULL;
    }
    return NULL;
}

void AutoOnHook::createAfterStartUp()
{

    if(g_rcv_battle_rsp)
    {
        g_rcv_battle_rsp = false;
        AutoOnHook* hook = GetAutoOnHook();
        if(!hook)
            return;
        hook->initAutoOnHook(missing_instance_id, 
            (PB_INSTANCE_TYPE_T)missing_type);
       
    	hook->MsgAutoBattleRsp(missing_ret);
        hook->changeTime();
        int s = g_rewards.size();
        for(int i =0; i<s; i++) {
            hook->setItemIcon(g_rewards[i].item_num, g_rewards[i].item_id, g_rewards[i].item_level);

        }
        g_rewards.clear();
        hook->sendCSABMonsterReq(missing_instance_id);

    }
}

void AutoOnHook::initAutoOnHook(int instance_id, PB_INSTANCE_TYPE_T type)
{
    AutoOnHook* hook = GetAutoOnHook();
    if(hook == NULL)
        return;

    INSTANCE_TYPE t = hook->getInstanceType();
    if(t == INSTANCE_TYPE_NOTHING)
    {
        //recreate the auto fight panel
           switch(type) {
            case PB_INSTANCE_TYPE_PVE:
                //normal fu ben
                hook->setInstanceType(instance_id, INSTANCE_TYPE_NORMAL);
                break;
            case PB_INSTANCE_TYPE_PVE2:
                //jing ying fu ben
                hook->setInstanceType(instance_id, INSTANCE_TYPE_JINGYING);
                break;
			case PB_INSTANCE_TYPE_PVE3:
                //jing ying fu ben
                hook->setInstanceType(instance_id, INSTANCE_TYPE_DUNGEON);
                break;
            default:
                    break;
            }
	}

	hook->setinstanceId(instance_id);
}

void AutoOnHook::createLayout(int instanceId)
{
	UIManager *manager = UIManager::sharedManager();

	switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		if (!instanceListLayout)
		{
			instanceListLayout = manager->CreateUILayoutFromFile("UIplist/AutoOnHookUI.plist", this, uiLayoutNames[instanceType]);	
            //special for normal fuben
            manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClickTurn1",	menu_selector(AutoOnHook::OnClickTurn1), this);
            manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClickTurn2",	menu_selector(AutoOnHook::OnClickTurn2), this);
            manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClickTurn3",	menu_selector(AutoOnHook::OnClickTurn3), this);
		}
		break;

	case INSTANCE_TYPE_JINGYING:
		if (!jingYingLayout)
#if 0
		{
			jingYingLayout = manager->CreateUILayoutFromFile("UIplist/autoFightOfJingYingUI.plist", this, uiLayoutNames[instanceType]);	
            chapterList = jingYingLayout->FindChildObjectByName<UIScrollList>("hook_characterScrollList_H");
			chapterArrow = jingYingLayout->FindChildObjectByName<UIPicture>("autohook_chapterselect_arrow");
            UIManager::sharedManager()->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::onPressJYChapterBtn",
			menu_selector(AutoOnHook::onPressJYChapterBtn), this);
            //updateCoinAndDiamand(jingYingLayout);
		}
#else
		{
			jingYingLayout = manager->CreateUILayoutFromFile("UIplist/AutoOnHookUI.plist", this, uiLayoutNames[instanceType]);	
            
            manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClickTurn1",	menu_selector(AutoOnHook::OnClickTurn1), this);
            manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClickTurn2",	menu_selector(AutoOnHook::OnClickTurn2), this);
            manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClickTurn3",	menu_selector(AutoOnHook::OnClickTurn3), this);
		}
#endif
		break;

	case INSTANCE_TYPE_DUNGEON:
		if (!instanceUnderLayout)
		{
			instanceUnderLayout = manager->CreateUILayoutFromFile("UIplist/autoFightOfJingYingUI.plist", this, uiLayoutNames[instanceType]);	
            chapterList = instanceUnderLayout->FindChildObjectByName<UIScrollList>("hook_characterScrollList_H");
			chapterArrow = instanceUnderLayout->FindChildObjectByName<UIPicture>("autohook_chapterselect_arrow");
            UIManager::sharedManager()->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::onPressJYChapterBtn",
			menu_selector(AutoOnHook::onPressJYChapterBtn), this);
            //updateCoinAndDiamand(instanceUnderLayout);
		}
		break;

	default:
		break;
	}
    //update coin and diamond
    updateCoinAndDiamand(getCurLayout());

    //function used by all type of auto fight
    UILayout *l = getCurLayout();
    if(l == NULL)
        return;
	m_pListV = l->FindChildObjectByName<UIScrollList>("hook_characterScrollList_V");
	m_pLabelCue = l->FindChildObjectByName<UILabel>("AUTOFIGHT_TEXT_CUE");

	if(instanceType == INSTANCE_TYPE_NORMAL || instanceType == INSTANCE_TYPE_JINGYING){

		InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
		GameConfigFromLuaManager::Get()->getInstanceBonusInfo(instanceId, bonusInfo);
		InstanceInfo* pInfo = InstanceManager::Get()->getOneInstance(instanceId);

		m_pLabelCue->setString(CCString::createWithFormat("%s%d/%d    %s%d",
			Localizatioin::getLocalization("M_AUTOFIGHT_REMAINTIME"),
			bonusInfo->max_number - pInfo->battle_times,
			bonusInfo->max_number,
			Localizatioin::getLocalization("M_REMAIN_STAMINA"),
			UserData::GetUserInfo().m_stamina)->getCString());

	}else{
		m_pLabelCue->setString("");
	}

	initProgressTimer();
    //inst start stop speed button
    initCommonButtonUi();
    //init reward grid
    setupIconButtons();
    //init count down ui
    initCountDownTimerUi();
    switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
        initNormalFuBenUi(instanceId);        
        break;

    case INSTANCE_TYPE_JINGYING:
#if 0
        initJingYingFuBenUi();
#else
	   initNormalFuBenUi(instanceId);    
#endif
		break;

	case INSTANCE_TYPE_DUNGEON:
        initUnderInstanceUI();
        break;

    default:
        CCAssert(false, "can not go here");
    }
    
	
	//UIManager *manager = UIManager::sharedManager();
    manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnClose",	menu_selector(AutoOnHook::OnClose), this);
    manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnSpeedUpHook",	menu_selector(AutoOnHook::OnSpeedUpHook), this);
    manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnStopHook",	menu_selector(AutoOnHook::OnStopHook), this);
    manager->RegisterMenuHandler(uiLayoutNames[instanceType],"AutoOnHook::OnStartAutoHook",	menu_selector(AutoOnHook::OnStartAutoHook), this);
    
    //this->setIconAllGet(0, 0, 0, 0);
   
    m_pCloseBtn = l->FindChildObjectByName<UIButton>("hook_close");

    //diamand panel
    m_pUseDiamand = manager->CreateUILayoutFromFile("UIplist/UseDiamand_AutoHook.plist", this, "UseDiamand_AutoHook", true);
    manager->RegisterMenuHandler("UseDiamand_AutoHook","AutoOnHook::OnConfirm",	menu_selector(AutoOnHook::OnConfirm), this);
    manager->RegisterMenuHandler("UseDiamand_AutoHook","AutoOnHook::OnCancle",	menu_selector(AutoOnHook::OnCancle), this);
    m_pUseDiamand->setVisible(false);
    
    setDiamandTxt();
    m_initremainTime = 0;
    //after 5 seconds send a timer request  if the TimeManager did not init
    //cause we need TimeManager to calc cold down time. if it didn't work
    //we can not work.
    long nowTime = TimeManager::Get()->getCurServerTime();
    if(nowTime == 0) 
    {
        timer_wrong = true;
        OnlineNetworkManager::sShareInstance()->sendReqNetStateMessage();
        WaitingLayerManager::Get()->PushOneMessage("CSPingReq");
        
    }
}

void AutoOnHook::RefreshTimesAndStamina(CCObject* obj)
{

	if (instanceType == INSTANCE_TYPE_NORMAL || instanceType == INSTANCE_TYPE_JINGYING)
	{
		InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
		GameConfigFromLuaManager::Get()->getInstanceBonusInfo(m_instanceId, bonusInfo);
		InstanceInfo* pInfo = InstanceManager::Get()->getOneInstance(m_instanceId);

		m_pLabelCue->setString(CCString::createWithFormat("%s%d/%d    %s%d",
			Localizatioin::getLocalization("M_AUTOFIGHT_REMAINTIME"),
			bonusInfo->max_number - pInfo->battle_times,
			bonusInfo->max_number,
			Localizatioin::getLocalization("M_REMAIN_STAMINA"),
			UserData::GetUserInfo().m_stamina)->getCString());
	}
}

void AutoOnHook::sendAtrReq(){
    CCLOG("send PB_ATTR_AB_NORMAL_MAP_CD*******************");
   // this->unschedule(schedule_selector(AutoOnHook::sendAtrReq));
    
    switch (instanceType)
	{
	#if 0
	
	case INSTANCE_TYPE_NORMAL:
        XLogicManager::sharedManager()->reqPlayerAttributes(PB_ATTR_AB_NORMAL_MAP_CD);
        break;
    case INSTANCE_TYPE_JINGYING:
        XLogicManager::sharedManager()->reqPlayerAttributes(PB_ATTR_AB_SPECIAL_MAP_CD);
       // OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(JING_YING_FU_BEN_MIN, JING_YING_FU_BEN_MAX);

        break;
	   #else
	   //no cd
	   #endif
	case INSTANCE_TYPE_DUNGEON:
        XLogicManager::sharedManager()->reqPlayerAttributes(PB_ATTR_AB_UGCITY_MAP_CD);
        OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(UNDERGROUND_INSTANCE_MIN, UNDERGROUND_INSTANCE_MAX);

        break;
    default:
        CCAssert(false, "can not go here");
    }
        
}

void AutoOnHook::setInstanceType(int instanceId, INSTANCE_TYPE v)
{	
	if(this->instanceType != v)
	{
        for(int i = 0; i < sizeof(uiLayoutNames)/sizeof(uiLayoutNames[0]); i ++)
		{
			UIManager::sharedManager()->RemoveUILayout(uiLayoutNames[i]);
		}
            
		jingYingLayout = NULL;
		instanceListLayout = NULL;
		instanceUnderLayout = NULL;
        UIManager::sharedManager()->RemoveUILayout("UseDiamand_AutoHook");
        instanceType = v;
    
        //init the layout
        this->createLayout(instanceId);
	}
}

void AutoOnHook::setinstanceId(unsigned int s)
{
    switch(instanceType)
    {
    case INSTANCE_TYPE_NORMAL:
    {
        this->m_instanceId = s;
        break;
    }
	
    case INSTANCE_TYPE_JINGYING:
    {
		#if 0
        this->m_currentChapterId = s;
		#else
		this->m_instanceId = s;
		#endif
        break;
    }
	
	case INSTANCE_TYPE_DUNGEON:
    {
        this->m_currentChapterId = s;
        break;
    }
    default:
        break;
    }

    std::map<int, ChapterInfo> *nameList = InstanceManager::Get()->getAllchapterName(instanceType);
    if(m_currentChapterId > 0 && chapterLabel)
    chapterLabel->setString((*nameList)[m_currentChapterId].name.c_str());
}

void AutoOnHook::setCoolDownEndTime(long endTime)
{
    this->coolDownEndTime = endTime;
}

void AutoOnHook::MsgAutoBattleRsp(int ret)
{
    if(ret == 0) {
	    USERINFO userinfo = UserData::GetUserInfo();
        int vip = userinfo.m_vip;
        int cd_time=0;
        switch (instanceType)
        {
        case INSTANCE_TYPE_NORMAL:
        {
         //   cd_time = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", vip, "Fight");           
			cd_time=1;//jackniu now no cd 

			break;
        }
        case INSTANCE_TYPE_JINGYING:
        {
           // cd_time = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", vip, "Fight");           
		   cd_time=1;//jackniu now no cd 

		   break;
        }
		case INSTANCE_TYPE_DUNGEON:
        {
            cd_time = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", vip, "Underground_Fight_Time");           
            break;
        }
        default:
            break;
        }
		/*
        if(cd_time == 1)
           startCD(0);
        else{
           setButtonState(1);
           
        }
		*/
        receiveBattleRsp = true;
    }
    #if 0
    else {
      char buff[50];
      sprintf(buff, "MsgAutoBattleRsp ret %d", ret);
      ShowMessage(buff);
    }
    #endif
}

void AutoOnHook::changeTime(){
    USERINFO info = UserData::GetUserInfo();
    switch (instanceType)
    {
    #if 0//jackniu no cd 
    case INSTANCE_TYPE_NORMAL:
        setColdTime(info.m_autofightNormalCd);
        break;
    case INSTANCE_TYPE_JINGYING:
        setColdTime(info.m_autofightJingYingCd);
        break;
	case INSTANCE_TYPE_DUNGEON:
        setColdTime(info.m_autofightUnderCd);
		break;
	#endif
    default:
        break;
    }     
}

void AutoOnHook::enterBagLayer(CCObject* sender)
{
    OnClose(NULL);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        int tag = mLayer->getLayerTagByName("BackPackLayer");
        mLayer->showUILayerByTag(tag,true);

        haveParentPanel =false;
    }
}
void AutoOnHook::enterMallChipPage(CCObject* sender)
{
    OnClose(NULL);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(true,1);

        haveParentPanel =false;
    }
}

void AutoOnHook::enterBuyStaminaPage(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "onStaminaClicked");
}

void AutoOnHook::onBroadcastMessage(BroadcastMessage* msg)
{
	
	if(msg->message == GM_ATTR_AUTO_FIGHT_CD && !timer_wrong) {
        changeTime();
        
    } else if(msg->message == GM_ATTR_SERVER_TIME_SYNCED){
        //we already sync with server.go to get the time value again from server
        if(timer_wrong && receiveBattleRsp){
            sendAtrReq();
            
            timer_wrong = false;
            changeTime();
        }
	} else if(msg->message == GM_ATTR_STAMINA_UPDATE || msg->message == GM_UPDATE_AUTO_BATTLE_TIMES){
		RefreshTimesAndStamina(NULL);
	}
	
}

 
void AutoOnHook::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

//make sure before init. set the instanceType
bool AutoOnHook::init()
{
    if(!CCLayer::init())
	{
		return false;
	}
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	return true;
}

void AutoOnHook::clearVecIDs()
{
    m_VecIDs.clear();
}

void AutoOnHook::sendCSABMonsterReq(unsigned int instance_id)
{
    m_instanceId = instance_id;
    PVPMessage::Get()->sendCSABMonsterReq(m_instanceId);
    //setPictureAndWord(instance_id);
}


//time up or speed up
void AutoOnHook::startCD(int timeout)
{   
#if 0
    //time out. go to complete state
    if(timeout != 0)
    {
        CCAssert(false, "startCD can not go to here");
        return;//not used jackniu

        processVisibleCD(true);
        convertSecToMinute(timeout);
        
        char buf[10];
        sprintf(buf, "%d", m_musicTime);
        std::string strname = buf;
        
        strname += ":";
        char buf2[10];
        sprintf(buf2, "%d", m_cnt);
        
        if(m_cnt < 10)
        {
            strname += "0";
            strname += buf2;
        }
        else
            strname += buf2;
        
        setUpCDTime(strname.c_str());
        //unscheduleUpdate();
        scheduleUpdate();
        showStartBtn(false);
    }
    else
#endif
    {
       
        updateCoinAndDiamand(getCurLayout());
		switch (instanceType)
		{
		case INSTANCE_TYPE_NORMAL:

			break;

		case INSTANCE_TYPE_JINGYING:
			{
				//refresh jing ying fu ben data base
				//cause when we finish this chapter .when we press the wan cheng button
				// show this chapter button in gray color
				// OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(JING_YING_FU_BEN_MIN, JING_YING_FU_BEN_MAX);

				break;
			}
		case INSTANCE_TYPE_DUNGEON:
			{

				OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(UNDERGROUND_INSTANCE_MIN, UNDERGROUND_INSTANCE_MAX);
				break;
			}
		default:
			break;
		}

		setButtonState(2);
	}
}

void AutoOnHook::ClearReward(){
	this->m_rewards.clear();
}

void AutoOnHook::SetReward(int item_num, int item_id, unsigned int item_level){
	autoOnHookReward reward;
	reward.item_id = item_id;
	reward.item_level = item_level;
	reward.item_num = item_num;
	m_rewards.push_back(reward);
}

void AutoOnHook::ShowReward(){
	int s = m_rewards.size();
	for(int i =0; i<s; i++) {
		setItemIcon(m_rewards[i].item_num, m_rewards[i].item_id, m_rewards[i].item_level);
	}
	ClearReward();
	setIsShownReward(true);
	startCD(0);

	getCurLayout()->setTouchEnabled(true);
	m_pLabelSweep->setVisible(false);
}

void AutoOnHook::progressDone(CCNode *sender){
	setIsProgressDone(true);

	if(!getIsWaitingReward() && !getIsShownReward())
	{
		ShowReward();
	}
}

void AutoOnHook::processVisibleCD(bool visible)
{
    m_pLabelCDNumber->setVisible(visible);
    
    m_pCDFrame->setVisible(visible);
    switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
	case INSTANCE_TYPE_JINGYING://jackniu
    {
		
		//no dao ji shi for normal and jingying fu ben
		m_pLabelCDText->setVisible(false);
		
        m_pTurnBtn1->setVisible(!visible);
        m_pTurnBtn2->setVisible(!visible);
        m_pTurnBtn3->setVisible(!visible);
        
        m_pTurnFrame->setVisible(!visible);
        m_pTurnText1->setVisible(!visible);
        m_pTurnText2->setVisible(!visible);
        
        m_pTurnSami1->setVisible(!visible);
        m_pTurnSami2->setVisible(!visible);
        m_pTurnSami3->setVisible(!visible);
        break;
    }
	#if 0
    case INSTANCE_TYPE_JINGYING:
    {
        CCLOG("%s visibal %d", __FUNCTION__, visible);
        chapterList->setVisible(!visible);
        chapterArrow->setVisible(!visible);
        
        break;
    }
	#endif
	case INSTANCE_TYPE_DUNGEON:
    {
		m_pLabelCDText->setVisible(visible);
        chapterList->setVisible(!visible);
        chapterArrow->setVisible(!visible);
        
        break;
    }
    default:
        break;
    }
   
}

///////////////////////////////////////////////////
//code for normal fu ben 
////////////////////////////////////////////////////
void AutoOnHook::initNormalFuBenUi(int instanceId)
{    
	UILayout * instanceListLayout = this->getCurLayout();
  
    //set normal fu ben turn text and get all the turn button 
    UILabel* plabel1_turn = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_turn1");
    UILabel* plabel1_stamina = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_stamina1");
    UILabel* plabel2_turn = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_turn2");
    UILabel* plabel2_stamina = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_stamina2");
    UILabel* plabel3_stamina = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_stamina3");

	int id = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/MapMusicTable.lua","getInstanceId", instanceId);
	int power = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Power");

    const char* turnstr1 = Localizatioin::getLocalization("M_AUTOFIGHT_TURN1");
    const char* turnstr1_cost = CCString::createWithFormat(Localizatioin::getLocalization("M_AUTOFIGHT_TURN1_COST"),power * -1)->getCString();
    const char* turnstr2 = Localizatioin::getLocalization("M_AUTOFIGHT_TURN2");
    const char* turnstr2_cost = CCString::createWithFormat(Localizatioin::getLocalization("M_AUTOFIGHT_TURN1_COST"),power * -5)->getCString();
    const char* turnstr3_cost = Localizatioin::getLocalization("M_AUTOFIGHT_TURN3_COST");
	

	plabel1_turn->setString(turnstr1);
	plabel1_stamina->setString(turnstr1_cost);
	if(instanceType == INSTANCE_TYPE_NORMAL) {
	plabel2_turn->setString(turnstr2);
	plabel2_stamina->setString(turnstr2_cost);
	}
	else
	{
		plabel2_turn->setString("");
			plabel2_stamina->setString("");

	}
	

	plabel3_stamina->setString(turnstr3_cost);
    
    plabel1_turn->setColor(ccWHITE);
    plabel1_stamina->setColor(ccWHITE);
    
    plabel2_turn->setColor(ccWHITE);
    plabel2_stamina->setColor(ccWHITE);
    
    plabel3_stamina->setColor(ccWHITE);
    
    m_pTurnFrame = instanceListLayout->FindChildObjectByName<UIPicture>("autohook_turn_background");
    m_pTurnText1 = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_turn1");
    m_pTurnText2 = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_turn2");
    
    m_pTurnBtn1 = instanceListLayout->FindChildObjectByName<UIButton>("hook_turn1");
    m_pTurnBtn2 = instanceListLayout->FindChildObjectByName<UIButton>("hook_turn2");
    m_pTurnBtn3 = instanceListLayout->FindChildObjectByName<UIButton>("hook_turn3");
    
    m_pTurnBtn1->getMenuItemSprite()->selected();
    
    m_pTurnSami1 = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_stamina1");
    m_pTurnSami2 = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_stamina2");
    m_pTurnSami3 = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_stamina3");
    
    if(instanceType == INSTANCE_TYPE_JINGYING){
		m_pTurnBtn2->setVisible(false);
		m_pTurnBtn2->setEnabled(false);
	}
    processVisibleCD(false);
}

///added code jackniu
void AutoOnHook::initCountDownTimerUi()
{
    UILayout * l = getCurLayout();
    m_pCDFrame = l->FindChildObjectByName<UIPicture>("autohook_frame_countdown");
    m_pLabelCDText = l->FindChildObjectByName<UILabel>("autohook_text_countdown");
    m_pLabelCDNumber = l->FindChildObjectByName<UILabel>("auto_number_countdown");

}

void AutoOnHook::initProgressTimer(){
	UILayout * l = getCurLayout();
	m_pLabelSweep = l->FindChildObjectByName<UILabel>("AUTOFIGHT_GRID_SWEEP_TEXT");
	m_pLabelSweep->setString(Localizatioin::getLocalization("M_SWEEPING"));
	m_pLabelSweep->setVisible(false);

	m_pProgressBg = l->FindChildObjectByName<UIPicture>("AUTOFIGHT_GRID_TUBE");
	
	float scale = UIManager::sharedManager()->getScaleFactor();

	int progressTimerId = getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_GRID_ENERGY");
	ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN2);
	CCPoint ptProgressTimer;
	m_pProgressTimer = as->getSpriteFromFrame_Middle(progressTimerId, 0, ptProgressTimer);
	m_pProgressTimer->setScaleX(0);
	m_pProgressTimer->setScaleY(scale);
	m_pProgressTimer->setAnchorPoint(ccp(0,0.5));
	m_pProgressTimer->setPosition(ccp(m_pProgressBg->getPosition().x-m_pProgressTimer->getContentSize().width * scale / 2,m_pProgressBg->getPosition().y));
	m_pProgressBg->getBindNode()->addChild(m_pProgressTimer,m_pProgressBg->getZOrder()+1);
}

void AutoOnHook::initCommonButtonUi(){
    UILayout * l = getCurLayout();
    m_pStartBtn = l->FindChildObjectByName<UIButton>("hook_start");
    m_pSpeedUpBtn = l->FindChildObjectByName<UIButton>("hook_speedup");
    m_pStopBtn = l->FindChildObjectByName<UIButton>("hook_stop");
    
    m_pStartBtn->setVisible(true);
    m_pSpeedUpBtn->setVisible(false);
    m_pStopBtn->setVisible(false);
    
    m_pStartLabel = l->FindChildObjectByName<UILabel>("auto_text_start");
    m_pSpeedupLabel = l->FindChildObjectByName<UILabel>("auto_text_speedup");
    m_pStopLabel = l->FindChildObjectByName<UILabel>("auto_text_stop");
    
    const char* start_txt = Localizatioin::getLocalization("M_AUTOFIGHT_START");
    const char* speedup_txt = Localizatioin::getLocalization("M_AUTOFIGHT_SPEEDUP");
    const char* stop_txt = Localizatioin::getLocalization("M_AUTOFIGHT_STOP");
    
    m_pStartLabel->setString(start_txt);
    m_pSpeedupLabel->setString(speedup_txt);
    m_pStopLabel->setString(stop_txt);
    
    m_pStartLabel->setVisible(true);
    m_pSpeedupLabel->setVisible(false);
    m_pStopLabel->setVisible(false);
}



//end add code
void AutoOnHook::setFuBen(const char *fuBenName)
{
    if(m_MasterFuBenName)
        m_MasterFuBenName->setString(fuBenName);
}

void AutoOnHook::setFuBen(int frame, const char* fuBenName)
{
    if(m_FuBenName)
        m_FuBenName->setString(fuBenName);
    
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui2.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    CCPoint pt = CCPointZero;
    m_FuBenIcon = pAsprite->getSpriteFromFrame_Middle(frame, 0, pt);
}


//set monster list
void AutoOnHook::setScrollist(const std::vector<OnHookA2>& onHooksA2)
{
    m_pListV->clearChildrens();
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui2.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    std::vector<OnHookA2>::const_iterator it = onHooksA2.begin();
    for(; it != onHooksA2.end(); ++it)
    {
        int mon_id = it->mon_id;
        int mon_num = it->mon_num;
        
        std::string name = "ENEMY_NAME_";
        char buf[10];
        sprintf(buf, "%d", mon_id);
        name += buf;
        
        name = Localizatioin::getLocalization(name.c_str());
        
        sprintf(buf, "%d", mon_num);
        string value = "X";
        value += buf;
        
        CCPoint pt = CCPointZero;
		int idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_TEXT_MONSTERNAME");
        CCSprite* normal = pAsprite->getSpriteFromFrame_Middle(idx ,0, pt);
		idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_TEXT_MONSTERNAME");
        CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle(idx, 0, pt);
        UIButton* pItem = m_pListV->AddNewItem(normal,
                                              clicked,
                                              "TalentLayer::OnClickListItem",
                                              "",
                                              ccGREEN,
                                              kCCScrollMenuPriority);
        if(pItem)
        {
            UILabelTTF* label_name = UILabelTTF::create(name.c_str(), KJLinXin, 24);
            UILabelTTF* label_value = UILabelTTF::create(value.c_str(), KJLinXin, 24);
            pItem->getAnimationNode()->addChild(label_name,10);
            pItem->getAnimationNode()->addChild(label_value,10);
            
            label_name->setPosition(ccp(label_name->getContentSize().width/2-normal->getContentSize().width/2, 0));
            label_value->setPosition(ccp(normal->getContentSize().width/2 , 0));
        }
    }
    if(instanceType == INSTANCE_TYPE_NORMAL 
	|| instanceType == INSTANCE_TYPE_JINGYING)
        setPictureAndWord(m_instanceId);
}

void AutoOnHook::setupIconButtons()
{
    m_pIconButtonLayer = CCLayer::create();
    UILayout *l = getCurLayout();
	if(l)
	{
		UIPicture* frame = l->FindChildObjectByName<UIPicture>("autohook_grid_area");
		if(frame)
		{
			int row = 2;
			int column = 6;
			CCSize size = frame->getSpriteSize();
			float width = size.width / column;
			float height = size.height / row;
			ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
			for(int i=0; i<row; i++)
			{
				for(int j=0; j<column; j++)
				{
					CCSprite* pEmptyItem = CCSprite::create();
					pEmptyItem->setContentSize(CCSize(width,height));
					CCPoint pt;
					int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_ICON_CLICKED");
					CCSprite* pSelectItem = as->getSpriteFromFrame_Middle(iFrameId,0,pt);
                    
                    float scaleFactor = UIManager::sharedManager()->getScaleFactor();
                    pSelectItem->setScale(scaleFactor);
                    
					IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, menu_selector(AutoOnHook::onItemTapped));
                    //IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, menu_selector(AccountsLayer::onItemTapped));

					pIcon->autorelease();
					int index = i * column + j;
					pIcon->SetButtonIndex(index+1);
					pIcon->setContentSize(pEmptyItem->getContentSize());
					pIcon->creatDragReceiver(EAlchemyBag);
					//pIcon->SetTappedHandler(this,menu_selector(AutoOnHook::onItemTapped),menu_selector(AutoOnHook::onTapCancel));
					float x = (j - column /2 + 0.5f) * width;
					float y = (-i + row /2 - 0.5f) * height;
                    pIcon->setTag(-1);
					pIcon->setPosition(ccp(x,y));
					m_pIconButtonList.push_back(pIcon);
					m_pIconButtonLayer->addChild(pIcon,index+1);
				}
			}
			m_pIconButtonLayer->setPosition(frame->getWorldPosition());
			l->getCurrentNode()->addChild(m_pIconButtonLayer);
		}
	}
}

//add item to grid
void AutoOnHook::setItemIcon(int item_num, int item_id, unsigned int item_level)
{
    CCLOG("autohook item_id:%d\n", item_id);
#if 0
    //check if timer is up
     {
        
        USERINFO info = UserData::GetUserInfo();
        int time = -1;
        switch (instanceType)
        {
        case INSTANCE_TYPE_NORMAL:
            time = info.m_autofightNormalCd;
            break;
        case INSTANCE_TYPE_JINGYING:
            time = info.m_autofightJingYingCd;
            break;
    	case INSTANCE_TYPE_DUNGEON:
            time = info.m_autofightUnderCd;
            break;
        default:
            break;
        }     

        if(time == 0)
        {
            setButtonState(2);
        }
        else
        {
            setButtonState(1);
        }
    }
#endif
  
       
    UILayout *curLayout = getCurLayout();
    //server now can send one more reward message in auto fight
    //so item_index will be always 0 fix this
    //int curIndex = m_VecIDs.size();
    if(curLayout && curLayout->isVisible())
	{
		float scaleFactor = UIManager::sharedManager()->getScaleFactor();
        int ret = -1;
    	if (m_itemMap.find(item_id) == m_itemMap.end())
    	{
    	    //new item
    		int curIndex = m_VecIDs.size();
            CCLOG("insert item index is %d", curIndex);

            m_itemMap[item_id] = item_num;
            m_VecIDs.push_back(item_id);
            IconButton* button = m_pIconButtonList[curIndex];
            button->setTag(curIndex);
            if(button)
            {
                CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item_id);
                sprite->setScale(scaleFactor);

				/*ItemBase* item = ItemManager::Get()->getItemInfoById(item_id);
				if(item)
				{
					CCSprite* framePic = ItemManager::Get()->getIconFrame(item->m_quality);
					if(framePic)
					{
						sprite->addChild(framePic);
						framePic->setPosition(ccp(sprite->getContentSize().width * 0.5f,sprite->getContentSize().height * 0.5f));
					}
				}*/
             
                char buf[10];
                sprintf(buf, "%d", m_itemMap[item_id]);
                UILabelTTF* label_name = UILabelTTF::create(buf, KJLinXin, 20);
                label_name->setTag(100);
				label_name->setScale(scaleFactor);
                //sprite->addChild(label_name);
                //label_name->setPosition(ccp(sprite->getContentSize().width*scaleFactor/2  + button->getContentSize().width /2 - label_name->getContentSize().width*scaleFactor/2 - 5,
                //                            sprite->getContentSize().height*scaleFactor/2 - button->getContentSize().height /2 + label_name->getContentSize().height*scaleFactor/2 + 5));
				label_name->setPosition(ccp(button->getContentSize().width/2 - 5*scaleFactor, - button->getContentSize().height/2 + 5*scaleFactor));
                label_name->setAnchorPoint(ccp(1,0));
                button->SetNormalIcon(sprite);
                button->creatDragReceiver(EAlchemyBag);
				button->addChild(label_name,1000,100);
                showSpriteParticleEffect(sprite,item_id,ccp(sprite->getContentSize().width * 0.5,sprite->getContentSize().height * 0.5));
            }
    	}  
        else 
    	{
    	    size_t i ;
            m_itemMap[item_id] += item_num;
            for(i = 0; i < m_VecIDs.size(); i++ )
            {
               if(m_VecIDs[i] == item_id)  
                break;
            }
            IconButton* button = m_pIconButtonList[i];
            CCAssert(button->getTag() == i, "button index is not right");
            if(button){
                CCSprite* sprite = button->getnormalTex();
                char buf[10];
                sprintf(buf, "%d", m_itemMap[item_id]);
                UILabelTTF* label_name = (UILabelTTF *)button->getChildByTag(100);
				label_name->setString(buf);
				label_name->setPosition(ccp(button->getContentSize().width/2 - 5*scaleFactor, - button->getContentSize().height/2 + 5*scaleFactor));
            }
        }
      
        
        /*
        int num = m_itemMap[item_id];
        item_num += num;
        m_itemMap[item_id] = item_num;
        */
       
    }
}

void AutoOnHook::showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt)
{
	char particleName[30];
	sprintf(particleName,"particle_effect_spirit_%d",id);
    CCNode* particle = ParticleManagerX::sharedManager()->getParticles(particleName);
    if(particle != NULL)
    {
		CCNode* child = parent->getChildByTag(100);
		if(child)
		{
			child->removeFromParentAndCleanup(true);
			child = NULL;
		}
        parent->addChild(particle,100,100);
        particle->setPosition(pt);
    }
}

void AutoOnHook::setIconAllGet(int number_diamand, int number_coin, int number_exploit, int number_exp)
{
	//no need to show these number
	//by arthurwang 14.10.13
    /*
    UILayout *curLayout = getCurLayout();
    UIPicture* allget = curLayout->FindChildObjectByName<UIPicture>("autohook_icon_allget");
    UILabel* diamand = curLayout->FindChildObjectByName<UILabel>("auto_number_diamond");
    UILabel* coin = curLayout->FindChildObjectByName<UILabel>("auto_number_coin");
    UILabel* exploit = curLayout->FindChildObjectByName<UILabel>("auto_number_exploit");
    UILabel* exp = curLayout->FindChildObjectByName<UILabel>("auto_number_exp");
    
    UILabel* autotxt = curLayout->FindChildObjectByName<UILabel>("auto_text_get");
    std::string msg = "";
    msg = Localizatioin::getLocalization("M_AUTOFIGHT_GETREWARD");
    autotxt->setString(msg.c_str());
    
    m_num_diamand += number_diamand;
    char buf[10];
    sprintf(buf, "%d\n", m_num_diamand);
    diamand->setString(buf);
    
    m_num_coin += number_coin;
    sprintf(buf, "%d\n", m_num_coin);
    coin->setString(buf);
    
    m_num_exploit += number_exploit;
    sprintf(buf, "%d\n", m_num_exploit);
    exploit->setString(buf);
    
    m_num_exp += number_exp;
    sprintf(buf, "%d\n", m_num_exp);
    exp->setString(buf);
    
    autotxt->setVisible(true);
    allget->setVisible(true);
    diamand->setVisible(true);
    coin->setVisible(true);
    exploit->setVisible(true);
    exp->setVisible(true);
	*/
}

void AutoOnHook::setIconAllGet(bool visible)
{
	//no need to show these number
	//by arthurwang 14.10.13
	/*
    UILayout *curLayout = getCurLayout();

    UIPicture* allget = curLayout->FindChildObjectByName<UIPicture>("autohook_icon_allget");
    UILabel* diamand = curLayout->FindChildObjectByName<UILabel>("auto_number_diamond");
    UILabel* coin = curLayout->FindChildObjectByName<UILabel>("auto_number_coin");
    UILabel* exploit = curLayout->FindChildObjectByName<UILabel>("auto_number_exploit");
    UILabel* exp = curLayout->FindChildObjectByName<UILabel>("auto_number_exp");
    UILabel* autotxt = curLayout->FindChildObjectByName<UILabel>("auto_text_get");
    
    allget->setVisible(visible);
    diamand->setVisible(visible);
    coin->setVisible(visible);
    exploit->setVisible(visible);
    exp->setVisible(visible);
	autotxt->setVisible(visible);
	*/
}



void AutoOnHook::setPictureAndWord(int instance_id)
{
    if(m_instanceId <= 0)
        return;
    UILayout *instanceListLayout = this->getCurLayout();

	const char * instanceText = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", m_instanceId, "RaidName");	
	string instanceConfigName = Localizatioin::getLocalization(instanceText);

    UIPicture* icon_instance = instanceListLayout->FindChildObjectByName<UIPicture>("autohook_frame_instance");
    
    UILabel* text_instance = instanceListLayout->FindChildObjectByName<UILabel>("autohook_text_instance");
    UILabel* text_chapter = instanceListLayout->FindChildObjectByName<UILabel>("autohook_chapter");
    
    text_instance->setString(instanceConfigName.c_str());
    text_chapter->setVisible(false);
    
	instanceConfigName = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", instance_id, "Icon");
	std::transform(instanceConfigName.begin(), instanceConfigName.end(), instanceConfigName.begin(), towupper);
	instanceConfigName = std::string("map_ui_system_icon_FRAME_") + instanceConfigName;
    
    CCPoint pt;
    int instanceIconId = getResourceIDByName(instanceConfigName.c_str());
	ASprite *as1 = AspriteManager::getInstance()->getAsprite(KICON_BIN);
	CCSprite *pIcon = as1->getSpriteFromFrame_Middle(instanceIconId, 0, pt);
    CCNode*node = icon_instance->getCurrentNode();
    node->removeChildByTag(100);
    node->addChild(pIcon,10,100);
    pIcon->setPosition(ccp(node->getContentSize().width/2, node->getContentSize().height/2));
}

void AutoOnHook::setUpCDTime(const char* strtime)
{
    //string strval = strtime;
    m_pLabelCDNumber->setString(strtime);
}

void AutoOnHook::setUpCDText(const char* strtxt)
{
    std::string strover = Localizatioin::getLocalization("M_AUTOFIGHT_OVER");
    m_pLabelCDNumber->setString(strover.c_str());
}

void AutoOnHook::setDiamandTxt()
{
    UILabel* confirmLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_confirm");
    UILabel* cancleLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_cancel");
    UILabel* tileLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_tile");
    
    std::string msg = "";
    msg = Localizatioin::getLocalization("M_CONFIRM");
    confirmLabel->setString(msg.c_str());
    
    msg = Localizatioin::getLocalization("M_CANCEL");
    cancleLabel->setString(msg.c_str());
    
    msg = Localizatioin::getLocalization("M_SKIP_CD");
    tileLabel->setString(msg.c_str());
    
    confirmLabel->setColor(ccWHITE);
    cancleLabel->setColor(ccWHITE);
    tileLabel->setColor(ccWHITE);
}

void AutoOnHook::convertSecToMinute(int time)
{
    m_musicTime = time /60;
    m_cnt = time % 60;
    return;
}

void AutoOnHook::showStartBtn(bool visible)
{
    m_pStartBtn->setVisible(visible);
    m_pStartLabel->setVisible(visible);
    
    m_pSpeedUpBtn->setVisible(!visible);
    m_pSpeedupLabel->setVisible(!visible);
    
    m_pStopBtn->setVisible(!visible);
    m_pStopLabel->setVisible(!visible);
}

void AutoOnHook::clearItemBtn()
{
    m_VecIDs.clear();
    m_itemMap.clear();
    for(size_t i=0; i<m_pIconButtonList.size(); i++)
    {
        IconButton* button = m_pIconButtonList[i];
        button->SetNormalIcon(NULL);
		button->removeChildByTag(100);
    }
}

void AutoOnHook::closeLayerCallBack( void )
{
	m_bCloseLayerCallBack = true;
	OnClose(NULL);
	m_bCloseLayerCallBack = false;
}

void AutoOnHook::OnSpeedUpHook(CCObject* sender)
{
    boolean ret;
    CCLOG("onSpeedUpHook\n");
    ret = updateSpendDiamand();
    if(!ret)//dimond is 0 not show
        return;
    m_pUseDiamand->setVisible(true);
}

void AutoOnHook::OnStopHook(CCObject* sender)
{
    CCLOG("onStopHook\n");
    PVPMessage::Get()->sendCSCancelAutoBattleReq();
    setButtonState(0);
    //start auto fight, then  restart the game. then press stop. should show the chapter list
    //so add bellow code to insert the label
    #if 0
    if(instanceType == INSTANCE_TYPE_JINGYING)
    {   //refresh the chapter data
        m_currentChapterId = -1;
        setchapterList(InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_JINGYING));
    }
	else 
	#endif
	if(instanceType == INSTANCE_TYPE_DUNGEON)
    {   //refresh the chapter data
        m_currentChapterId = -1;
        setchapterList(InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_DUNGEON));
    }
	
    unregisterTime();
    freshBag();
}
//0 for start. 1 for speed and stop 2 for time over
void AutoOnHook::setButtonState(int state)
{
    if(m_state == state)
        return;
    //state change 
    if(m_state==2 && state == 1)
        return;
    
        
        m_state = state;
        CCLOG("%s change state to %d", __FUNCTION__, state);
        switch(state){
        case 0:
    		{
            showStartBtn(true);
            const char* start_txt = Localizatioin::getLocalization("M_AUTOFIGHT_START");
            m_pStartLabel->setString(start_txt);
            processVisibleCD(false);
            m_pCloseBtn->setVisible(true);
			m_pProgressTimer->setScaleX(0);
            break;
    		}
        case 1:
    	  {
             
            //setColdTime(long seconds)
            showStartBtn(false);
            
            m_pCloseBtn->setVisible(false);
            
            processVisibleCD(true);
            //changeTime();


    		break;
    		}
       case 2:
    	   {
              
            showStartBtn(true);
            //change start button text to complete
            const char* start_txt = Localizatioin::getLocalization("M_AUTOFIGHT_COMPLETE");
            m_pStartLabel->setString(start_txt);
            setUpCDText("");
            processVisibleCD(true);
			m_pCloseBtn->setVisible(false);
            TaskManager::getInstance()->RqsTaskFromServer();
    	   }
        default:
            break;
        }
    
}
void AutoOnHook::OnStartAutoHook(CCObject* sender)
{
    //finish auto fight
    if(m_state == 2 || m_bStopSel)
    {
#if 0
        if(instanceType == INSTANCE_TYPE_JINGYING)
        {   //refresh the chapter data
            m_currentChapterId = -1;
            setchapterList(InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_JINGYING));
        }

		else 
#endif
		if(instanceType == INSTANCE_TYPE_DUNGEON)
        {   //refresh the chapter data
            m_currentChapterId = -1;
            setchapterList(InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_DUNGEON));
        }
        setButtonState(0);
        freshBag();
        
    }
    else
    {
    
        USERINFO info =  UserData::GetUserInfo();
        int stamina = info.m_stamina;

		int staminaCost = 0;
		if(instanceType == INSTANCE_TYPE_NORMAL || instanceType == INSTANCE_TYPE_JINGYING)
		{
			InstanceInfoBonusInfo bonusInfo =  InstanceInfoBonusInfo();
			GameConfigFromLuaManager::Get()->getInstanceBonusInfo(m_instanceId, &bonusInfo);	
			const InstanceInfo *battleinfo = InstanceManager::Get()->getOneInstance(m_instanceId);
			staminaCost = bonusInfo.power;
			if (m_turnId == 5)
			{
				staminaCost = staminaCost*5;
			}

			if ( (m_turnId == 0 && bonusInfo.max_number == battleinfo->battle_times )|| (bonusInfo.max_number < battleinfo->battle_times + m_turnId)){

				if (instanceType == INSTANCE_TYPE_NORMAL)
				{
					// 弹出确认提示框
					ShowConfirmBox();
					return;
				}
				if (instanceType == INSTANCE_TYPE_JINGYING)
				{
					// 弹出确认提示框
					ShowJYConfirmBox();
					return;
				}
			}
		}

		float scaleFactor = UIManager::sharedManager()->getScaleFactor();

        switch (instanceType)
    	{
    	case INSTANCE_TYPE_NORMAL:
		case INSTANCE_TYPE_JINGYING:
        {

	
			if(m_turnId == 0 || m_turnId == 5){
				if(UserData::GetVipLevel() < 1){
					ShowMessage(Localizatioin::getLocalization("M_NO_VIP"));
					return;
				}
			}

            if(stamina < staminaCost){
             //ShowMessage(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"));
              InterfaceManager::Get()->ShowNOEnoughStaminaBox(this, menu_selector(AutoOnHook::enterBuyStaminaPage));  
             return;
            }

			PVPMessage::Get()->sendCSAutoBattleReq(m_instanceId, m_turnId);

			setIsWaitingReward(true);
			setIsProgressDone(false);
			setIsShownReward(false);

			m_pLabelSweep->setVisible(true);
			m_pProgressTimer->setScaleX(0);
			m_pProgressTimer->runAction(CCSequence::create(
				CCScaleTo::create(3,scaleFactor,scaleFactor),
				CCCallFuncN::create(this, callfuncN_selector(AutoOnHook::progressDone)),
				NULL));
           
    	}
            break;
			#if 0
        case INSTANCE_TYPE_JINGYING:
			{
            std::map<int, ChapterInfo>* values = InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_JINGYING);
            if(values->size() == 0)
            {
                CCLOG("no chaper can auto fight");
                ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISHALL"));
                return;
            }
            vallimte = 5;
            if(stamina < vallimte)
            {
                //ShowMessage(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"));
                 InterfaceManager::Get()->ShowNOEnoughStaminaBox(this, menu_selector(AutoOnHook::enterBuyStaminaPage));
                return;
            }
            if(m_currentChapterId == -1)
                return;
            std::map<int, ChapterInfo> *nameList = InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_JINGYING);
            if((*nameList)[m_currentChapterId].active == false)
            {
                ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISHALL"));
                return;
            }
            PVPMessage::Get()->sendCSAutoBattleReq(m_currentChapterId, PB_INSTANCE_TYPE_PVE2);
            
            break;
			}
		#endif
		case INSTANCE_TYPE_DUNGEON:
			{
            std::map<int, ChapterInfo>* values = InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_DUNGEON);
            if(values->size() == 0)
            {
                CCLOG("no chaper can auto fight");
                ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISHALL"));
                return;
            }
            //vallimte = 5; //jackniu dungeon didn't need stamina. disable this
            //if(stamina < vallimte)
            //{
                //ShowMessage(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"));
           //     InterfaceManager::Get()->ShowNOEnoughStaminaBox(this, menu_selector(AutoOnHook::enterBagLayer) , menu_selector(AutoOnHook::enterMallChipPage));
           //     return;
            //}
            if(m_currentChapterId == -1)
                return;
            std::map<int, ChapterInfo> *nameList = InstanceManager::Get()->getAllchapterName(INSTANCE_TYPE_DUNGEON);
            if((*nameList)[m_currentChapterId].active == false)
            {
                ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISHALL"));
                return;
            }

			setIsWaitingReward(true);
			setIsProgressDone(false);
			setIsShownReward(false);

			PVPMessage::Get()->sendCSAutoBattleReq(m_currentChapterId, PB_INSTANCE_TYPE_PVE3);

			m_pLabelSweep->setVisible(true);
			m_pProgressTimer->runAction(CCSequence::create(
				CCScaleTo::create(3,scaleFactor,scaleFactor),
				CCCallFuncN::create(this, callfuncN_selector(AutoOnHook::progressDone)),
				NULL));
            
            break;
			}
        default:
            break;
        }

		getCurLayout()->setTouchEnabled(false);

        //WaitingLayerManager::Get()->PushOneMessage("CSAutoBattleReq");

        
    }
}

void AutoOnHook::onTapCancel(CCObject* sender)
{
    TipMgr::Get()->destroyTip();
}

void AutoOnHook::onItemTapped(CCObject* sender)
{
    if(m_VecIDs.size() <= 0)
        return;
    
    /*
    UIMenu* menu = dynamic_cast<UIMenu*>(sender);
	if(menu)
	{
		CCNode* parent = menu->getParent();
		IconButton* button = dynamic_cast<IconButton*>(parent);
		if(button)
		{
    
            int item_index = button->getTag();
            if(item_index < 0)return;
            int item_id = m_VecIDs[item_index];
            ItemManager::Get()->showItemTipsById(item_id, menu->getTouchPoint());
		}
	}*/
    
    CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
                int item_index = button->getTag();
                if(item_index < 0)return;
                int item_id = m_VecIDs[item_index];
                ItemManager::Get()->showItemTipsById(item_id, menu->getTouchPoint());
            }
        }
    }
}

void AutoOnHook::OnMasterClickListItem(CCObject* sender)
{
}

void AutoOnHook::OnClickTurn1(CCObject* sender)
{
    CCLOG("OnClickTurn1\n");
    m_pTurnBtn1->getMenuItemSprite()->selected();
    m_pTurnBtn2->getMenuItemSprite()->unselected();
    m_pTurnBtn3->getMenuItemSprite()->unselected();
    this->m_turnId = 1;
}

void AutoOnHook::OnClickTurn2(CCObject* sender)
{
    CCLOG("OnClickTurn2\n");
    m_pTurnBtn2->getMenuItemSprite()->selected();
    m_pTurnBtn1->getMenuItemSprite()->unselected();
    m_pTurnBtn3->getMenuItemSprite()->unselected();
    this->m_turnId = 5;
}

void AutoOnHook::OnClickTurn3(CCObject* sender)
{
    CCLOG("OnClickTurn3\n");
    m_pTurnBtn3->getMenuItemSprite()->selected();
    m_pTurnBtn1->getMenuItemSprite()->unselected();
    m_pTurnBtn2->getMenuItemSprite()->unselected();
    this->m_turnId = 0;
}

void AutoOnHook::OnClose(CCObject *sender)
{
    //unscheduleUpdate();
    UIManager *manager = UIManager::sharedManager();
    removeLayout();
    NotificationCenter::defaultCenter()->broadcast(GM_CLOSE_AUTOONHOOK, this);
    PVPMessage::Get()->sendCSCancelAutoBattleReq();    
}
//dimond button call back
void AutoOnHook::OnConfirm(CCObject *sender)
{
    int diamand_id = 10400;

    switch(instanceType)
    {
    case INSTANCE_TYPE_NORMAL:
        diamand_id = 10400;
        break;
    case INSTANCE_TYPE_JINGYING:
        diamand_id = 10500;
        break;
    case INSTANCE_TYPE_DUNGEON:
		diamand_id = 10501;
        break;
    default:
        break;
    }
    WaitingLayerManager::Get()->PushOneMessage("CSExchangeParameterReq");
    PVPMessage::Get()->sendSpeedUpReq(diamand_id);
    m_pUseDiamand->setVisible(false);
}

void AutoOnHook::OnCancle(CCObject *sender)
{
    m_pUseDiamand->setVisible(false);
}
//diamond
void AutoOnHook::freshBag()
{

    clearItemBtn();
    const char* start_txt = Localizatioin::getLocalization("M_AUTOFIGHT_START");
    m_pStartLabel->setString(start_txt);
    processVisibleCD(false);
    m_bStopSel = false;
   
    
    this->m_num_diamand = 0;
    this->m_num_coin = 0;
    this->m_num_exploit = 0;
    this->m_num_exp = 0;
    //setIconAllGet(0,0,0,0);
    setUpCDTime("00 : 00");    
    TaskManager::getInstance()->RqsTaskFromServer();
}

void AutoOnHook::hideCloseBtn()
{
    m_pCloseBtn->setVisible(false);
}

boolean AutoOnHook::updateSpendDiamand()
{
#if 0
    const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", 10400, "FromItems");
    string exchangeStr = string(fromItems);
    int dashPos = exchangeStr.find_first_of("/");
    if(dashPos != exchangeStr.npos)
    {
        unsigned int length = exchangeStr.size();
        std::string idStr = exchangeStr.substr(0,dashPos);
        std::string numStr = exchangeStr.substr(dashPos+1,length-1);
        int id = atoi(idStr.c_str());
        int num = atoi(numStr.c_str());
        ASSERT(id == 416002,"cost is not diamond!");
          
        unsigned int diamond = (m_remainTime / 60) * num;
        if(m_remainTime % 60 != 0)
        {
             diamond += num;
        }
      
        UILabel* diamand_label = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_num");
        char buf[10];
        sprintf(buf, "%d", diamond);
        diamand_label->setString(buf);
    }
#endif
    int exchangeId;
    switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
    {
        exchangeId = 10400;
        break;
    }
    case INSTANCE_TYPE_JINGYING:
    {
        exchangeId = 10500;
        break;
    }	
    case INSTANCE_TYPE_DUNGEON:
    {
        exchangeId = 10501;
        break;
    }   
    default:
        break;
    }
    int num = LuaTinkerManager::Get()->callLuaFunc<int>("Script/GameConfig/ExchangeParameterControl.lua", 
    "getExchangeParameterCostNum",exchangeId);
    CCLOG("exchange diamond is %d", num);
    unsigned int diamond = (m_remainTime / 60) * num;
    if(m_remainTime % 60 != 0)
    {
         diamond += num;
    }
    if(diamond <= 0)
        return false;
    UILabel* diamand_label = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_num");
    char buf[10];
    sprintf(buf, "%d", diamond);
    diamand_label->setString(buf);
    return true;
}

void AutoOnHook::setColdTime(long coolDownEndTime)
{
    long nowTime = TimeManager::Get()->getCurServerTime();
    if(nowTime == 0)
        return;
    if(coolDownEndTime == 0 && receiveBattleRsp){
       
        startCD(0);        
        return;
    }
    if(coolDownEndTime == 0) //when the timer up. server will send a 0.and go here.no need this.
        return;
    if(coolDownEndTime - nowTime < 0)
    {
		startCD(0);        
        return;
    }

    switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		{
        initNormalTimer(coolDownEndTime);
        m_initremainTime = 1;
		break;
		}
    case INSTANCE_TYPE_JINGYING:
		{
        initJingyingTimer(coolDownEndTime);
        m_initremainTime = 1;
		break;
		}
	case INSTANCE_TYPE_DUNGEON:
		{
        initUnderTimer(coolDownEndTime);
        m_initremainTime = 1;
		break;
		}
    default:
        return;
    }

}

void AutoOnHook::initNormalTimer(long coolDownEndTime)
{
	//long nowTime = TimeManager::Get()->getCurServerTime();
	if(TimeManager::Get()->hasTimer(PB_ATTR_AB_NORMAL_MAP_CD))
	{
		TimeManager::Get()->renewTimer(PB_ATTR_AB_NORMAL_MAP_CD, coolDownEndTime);
		if (!TimeManager::Get()->hasObserver(this, PB_ATTR_AB_NORMAL_MAP_CD))
		{
			TimeManager::Get()->attachTimer(this, PB_ATTR_AB_NORMAL_MAP_CD);
		}
	}
	else
	{
		TimeManager::Get()->registerTimer(this, PB_ATTR_AB_NORMAL_MAP_CD, coolDownEndTime);
	}
}


void AutoOnHook::onUpdateRemainTime(long remainTime)
{
    if(m_state != 1)
        return;
    char timeString[30];
    unsigned int hour = remainTime / 3600;
    unsigned int min = (remainTime - hour * 3600) / 60;
    unsigned int second = remainTime % 60;
    if(hour > 0)
    {
        sprintf(timeString,"%02d : %02d : %02d",hour,min,second);
    }
    else
    {
        sprintf(timeString,"%02d : %02d",min,second);
    }
    if(remainTime <= 0)
    {
        startCD(0);
    }
    else 
    {
        setUpCDTime(timeString);
    }
    m_remainTime = remainTime;
}


void AutoOnHook::setchapterList(std::map<int, ChapterInfo>* values)
{
    
    chapterList->clearChildrens();
    m_buttonList.clear();
    m_currentChapterId = -1;
    int mapLen = values->size();
    if(mapLen == 0)
    {
        CCLOG("no chaper can auto fight");
        return;
    }
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite(KUI_BIN2);

    CCPoint parentPos;

    //fix bug 副本页数的标签建议修改为左对齐 
    //add some invisible button
    int num = 3 - mapLen;
    if(num > 0)
    {	

        while(num > 0)
        {
            int idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_BUTTON_CHAPTERUNSELECT");
            CCSprite* disable = pAsprite->getSpriteFromFrame_Middle(idx ,0, parentPos);
            
        	idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_BUTTON_CHAPTERSELECT");
            CCSprite* normal= pAsprite->getSpriteFromFrame_Middle(idx, 0, parentPos);
        	idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_BUTTON_CHAPTERCLICKED");
            CCSprite* clicked = pAsprite->getSpriteFromFrame_Middle(idx ,0, parentPos);
            UIButton* button;
            CCLOG("add invisible button %d", num);
            button = chapterList->AddNewItem(disable, disable, "", 
                "",
        		ccWHITE,
        		KCCMessageBoxContorlPripority);     
            num--;
            button->setVisible(false);
        }  
    }

    
    std::map<int, ChapterInfo>::reverse_iterator it = values->rbegin();

    for(; it != values->rend(); ++it)
    {   
		CCLOG("insert %d", it->first);
        string chapterName = it->second.name;
        int idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_BUTTON_CHAPTERUNSELECT");
        CCSprite* disable = pAsprite->getSpriteFromFrame_Middle(idx ,0, parentPos);
        
    	idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_BUTTON_CHAPTERSELECT");
        CCSprite* normal= pAsprite->getSpriteFromFrame_Middle(idx, 0, parentPos);
    	idx = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_BUTTON_CHAPTERCLICKED");
        CCSprite* clicked = pAsprite->getSpriteFromFrame_Middle(idx ,0, parentPos);
        UIButton* button;

        if(!it->second.active)
        {
            button = chapterList->AddNewItem(disable, disable, "AutoOnHook::onPressJYChapterBtn", 
            "",
    		ccWHITE,
    		kCCScrollMenuPriority);         
        }
        else 
        {
            button = chapterList->AddNewItem(normal, clicked, "AutoOnHook::onPressJYChapterBtn", 
            "",
    		ccWHITE,
    		kCCScrollMenuPriority);        
        }

       // UIButton* button = new IconButton(normal, NULL, clicked, this, menu_selector(AutoOnHook::onPressJYChapterBtn));
   
       if(button)
	   {			
			CCPoint pt;
			int iFrameId = TXGUI::getResourceIDByName("map_ui2_FRAME_AUTOFIGHT_TEXT_CHAPTERSELECT");
			CCSprite* sprite = pAsprite->getSpriteFromFrame_Middle(iFrameId,0,pt);
			CCSize labelSize = sprite->getContentSize();
			UILabelTTF* labelName = UILabelTTF::create(chapterName.c_str(),KJLinXin,30,CCSize(300,labelSize.height),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
			labelName->setPosition(ccpSub(pt,parentPos));
			labelName->setColor(ccWHITE);
			button->getAnimationNode()->addChild(labelName,2);

			button->setButtonIndex(it->first);
           
            m_buttonList[it->first]=button;
            //set the last active chapter be the current chapter 
            if(it->second.active && m_currentChapterId < 0)
            {
                m_currentChapterId = it->first;
                button->getMenuItemSprite()->selected();
                
            }
            
       }
    }
   
    setchaptername(instanceType);
    
}

void AutoOnHook::onPressJYChapterBtn(CCObject *sender)
{
    
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(sender);
	if(item)
	{
		CCNode* node = item->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			UIControlBase* controller = menu->getControlNode();
			UIButton* button = dynamic_cast<UIButton*>(controller);
            
			if(button)
			{				
    			/*if(! button->isEnabled())
                {
                    CCLOG("button is disableed return");
                    return;
                }*/
				unsigned int chapterid = button->getButtonIndex();
                CCLOG("select button is %d", chapterid);
				if(m_currentChapterId > 0)
		        {
                    UIButton* m_selectedButton = m_buttonList[m_currentChapterId];
                    if(m_selectedButton != NULL)
				    {
    					CCMenuItem* lastItem = m_selectedButton->getMenuItemSprite();
    					if(lastItem)
    					{
    						lastItem->unselected();
    					}
		    		}
                }
				m_currentChapterId = chapterid;
				item->selected();
                setchaptername(instanceType);
			}
		}
       
	}
    
}

void AutoOnHook::setchaptername(INSTANCE_TYPE type)
{
    if(m_currentChapterId < 0)
        return;
	switch (type)
	{
#if 0
	case INSTANCE_TYPE_JINGYING:
		PVPMessage::Get()->sendCSABMonsterReq(m_currentChapterId, PB_INSTANCE_TYPE_PVE2);
		break;
#endif
	case INSTANCE_TYPE_DUNGEON:
		PVPMessage::Get()->sendCSABMonsterReq(m_currentChapterId, PB_INSTANCE_TYPE_PVE3);
		break;

	default:
		break;
	}
    

    std::map<int, ChapterInfo> *nameList = InstanceManager::Get()->getAllchapterName(instanceType);
    if(m_currentChapterId > 0)
    chapterLabel->setString((*nameList)[m_currentChapterId].name.c_str());
}

void AutoOnHook::initJingYingFuBenUi()
{
    if(jingYingLayout == NULL)
        return;

    chapterLabel = jingYingLayout->FindChildObjectByName<UILabel>("autohook_chapter");

    processVisibleCD(false);
}

void AutoOnHook::initUnderInstanceUI()
{
	if(instanceUnderLayout == NULL)
        return;

    chapterLabel = instanceUnderLayout->FindChildObjectByName<UILabel>("autohook_chapter");

    processVisibleCD(false);
}

void AutoOnHook::initJingyingTimer(long coolDownEndTime)
{
	if(TimeManager::Get()->hasTimer(PB_ATTR_AB_SPECIAL_MAP_CD))
	{
		TimeManager::Get()->renewTimer(PB_ATTR_AB_SPECIAL_MAP_CD, coolDownEndTime);
		if (!TimeManager::Get()->hasObserver(this, PB_ATTR_AB_SPECIAL_MAP_CD))
		{
			TimeManager::Get()->attachTimer(this, PB_ATTR_AB_SPECIAL_MAP_CD);
		}
	}
	else
	{
		TimeManager::Get()->registerTimer(this, PB_ATTR_AB_SPECIAL_MAP_CD, coolDownEndTime);
	}
}

void AutoOnHook::initUnderTimer(long coolDownEndTime)
{
	if(TimeManager::Get()->hasTimer(PB_ATTR_AB_UGCITY_MAP_CD))
	{
		TimeManager::Get()->renewTimer(PB_ATTR_AB_UGCITY_MAP_CD, coolDownEndTime);
		if (!TimeManager::Get()->hasObserver(this, PB_ATTR_AB_UGCITY_MAP_CD))
		{
			TimeManager::Get()->attachTimer(this, PB_ATTR_AB_UGCITY_MAP_CD);
		}
	}
	else
	{
		TimeManager::Get()->registerTimer(this, PB_ATTR_AB_UGCITY_MAP_CD, coolDownEndTime);
	}
}

void AutoOnHook::updateCoinAndDiamand(UILayout* player)
{
    //instanceListLayout
    if(!player)
        return;
    
    UILabel* gold_label = player->FindChildObjectByName<UILabel>("AUTOFIGHT_NUMBER_COIN_INALL");
    UILabel* diamand_label = player->FindChildObjectByName<UILabel>("AUTOFIGHT_NUMBER_DIAMOND_INALL");
    
    USERINFO info =  UserData::GetUserInfo();
    int goldnum = info.m_gold;
    int diamandnum = info.m_diamond;
    
    const char* buf = MainMenuLayer::GetNumByFormat(goldnum);
    if(buf)
        gold_label->setString(buf);

	char buffer[100];
    sprintf(buffer,"%d",diamandnum);
    //buf = MainMenuLayer::GetNumByFormat(diamandnum);
    if(buffer)
        diamand_label->setString(buffer);
}

void AutoOnHook::ShowConfirmBox()
{
	InstanceInfo *battleinfo = InstanceManager::Get()->getOneInstance(m_instanceId);

    int times = battleinfo->refresh_times;
    if (times >= 3)
    {
        MessageBox::Show(Localizatioin::getLocalization("M_RESET_INSTANCE_FULL"), this, NULL, NULL, MB_OK);
        return;
    }

    int basicCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 20520);
    int addedCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 20521);

    CCString* pString = CCString::createWithFormat(Localizatioin::getLocalization("M_RESET_INSTANCE"), basicCost + addedCost * (times));

    MessageBox::Show(pString->getCString(), this, menu_selector(AutoOnHook::OnResetTimes), 
        menu_selector(AutoOnHook::OnCancel),MB_OKCANCELCLOSE);
    MessageBox::AdjustStyle(Localizatioin::getLocalization("M_CONFIRM"), Localizatioin::getLocalization("M_CANCEL"));
    MessageBox::showCloseBtn();
}

void AutoOnHook::ShowJYConfirmBox()
{
	InstanceInfo *battleinfo = InstanceManager::Get()->getOneInstance(m_instanceId);

	int maxTimes = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", UserData::GetVipLevel(), "Elite");
	int times = battleinfo->refresh_times;
	if (times >= maxTimes)
	{
		MessageBox::Show(Localizatioin::getLocalization("M_RESET_NOTIME"), this, NULL, NULL, MB_OK);
		return;
	}

	int cost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 20000 + battleinfo->refresh_times);

	CCString* pString = CCString::createWithFormat(Localizatioin::getLocalization("M_RESET_INSTANCE"), cost);
	CCString* pString1 = CCString::createWithFormat("%s%d/%d",Localizatioin::getLocalization("M_RESET_NUMBER"), maxTimes-battleinfo->refresh_times, maxTimes);

	MessageBox::Show(pString->getCString(), this, menu_selector(AutoOnHook::OnResetTimes), 
		menu_selector(AutoOnHook::OnCancel),MB_OKCANCELCLOSE);
	MessageBox::ShowTextExt(pString1->getCString());
	MessageBox::AdjustStyle(Localizatioin::getLocalization("M_CONFIRM"), Localizatioin::getLocalization("M_CANCEL"));
	MessageBox::showCloseBtn();
}

void AutoOnHook::OnCancel(CCObject* sender){}
void AutoOnHook::OnResetTimes(CCObject* sender)
{
    OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(20520, &(m_instanceId), 1);
}

