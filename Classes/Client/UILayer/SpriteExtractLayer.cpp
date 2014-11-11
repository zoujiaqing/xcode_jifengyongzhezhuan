#include "OnlineNetworkManager.h"
#include "SpriteExtractLayer.h"
#include "UIDefine.h"
#include "GameManager.h"
#include "TimeManager.h"
#include "LuaTinkerManager.h"
#include "UIManager.h"
#include "ItemManager.h"
#include "MainMenuLayer.h"
#include "TutorialsManager.h"
#include "MarketManager.h"
#include "GameAudioManager.h"
#include "Localization.h"
#include "AspriteManager.h"
#include "MarketLayer.h"
#include "GMessage.h"

SpriteExtractLayer::SpriteExtractLayer() :
	m_extractLayer1(NULL),
	m_extractLayer3(NULL),
	m_extractLayer4(NULL),
	m_extractLayer5(NULL),
    m_extractBtn1(NULL),
    //m_extractBtn2(NULL),
    m_extractBtn3(NULL),
	m_extractBtn4(NULL),
	m_extractBtn5(NULL),
    //m_blueLabel(NULL),
    m_purpleLabel(NULL),
    m_purpleDesc(NULL),
    m_label5(NULL),
    m_greenCost(NULL),
    //m_blueCost(NULL),
    m_purpleCost(NULL),
	m_yellowCost(NULL),
	m_redCost(NULL),
    m_greenLabel(NULL),
    m_xxx(0),
    m_yyy(0),
    m_uiLayer(NULL),
    m_diamondAmount(NULL),
    m_spriteListBtn(NULL),
    m_greenNote(NULL),
    //m_blueNote(NULL),
	m_purpleNote(NULL)
{
    long nowTime = TimeManager::Get()->getCurServerTime();
    m_xxx = nowTime + 20;
    m_yyy = nowTime + 10000;
}

SpriteExtractLayer::~SpriteExtractLayer()
{
	setExtractLayer1(NULL);
	setExtractLayer3(NULL);
	setExtractLayer4(NULL);
	setExtractLayer5(NULL);

    setExtractBtn1(NULL);
    //setExtractBtn2(NULL);
    setExtractBtn3(NULL);
	setExtractBtn4(NULL);
	setExtractBtn5(NULL);

    setSpriteListBtn(NULL);

    setGreenLabel(NULL);
    //setBlueLabel(NULL);
    setPurpleLabel(NULL);
    setPurpleDesc(NULL);
    setLabel5(NULL);

    setGreenCost(NULL);
    //setBlueCost(NULL);
    setPurpleCost(NULL);
	setYellowCost(NULL);
	setRedCost(NULL);
    
    setDiamondAmount(NULL);
    setGreenNote(NULL);
	setPurpleNote(NULL);

    UIManager::sharedManager()->RemoveLayout("sprite_extract");
    delete m_uiLayer;

    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler SpriteExtractLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SpriteExtractLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "extract_1", SpriteExtractLayer::onExtract1);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "extract_2", SpriteExtractLayer::onExtract2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "extract_3", SpriteExtractLayer::onExtract3);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "extract_4", SpriteExtractLayer::onExtract4);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "extract_5", SpriteExtractLayer::onExtract5);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "sprite_list", SpriteExtractLayer::onClickSpriteList);
    return NULL;
}

bool SpriteExtractLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_layer_1", CCLayer*, m_extractLayer1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_layer_3", CCLayer*, m_extractLayer3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_layer_4", CCLayer*, m_extractLayer4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_layer_5", CCLayer*, m_extractLayer5);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_btn_1", CCControlButton*, m_extractBtn1);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_btn_2", CCControlButton*, m_extractBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_btn_3", CCControlButton*, m_extractBtn3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_btn_4", CCControlButton*, m_extractBtn4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extract_btn_5", CCControlButton*, m_extractBtn5);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_list",   CCControlButton*, m_spriteListBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "green_label",   CCLabelTTF*,      m_greenLabel);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "blue_label",    CCLabelTTF*,      m_blueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "purple_label",  CCLabelTTF*,      m_purpleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "purple_desc",   CCLabelTTF*,      m_purpleDesc);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_5",       CCLabelTTF*,      m_label5);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "green_cost",    CCLabelTTF*,      m_greenCost);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "blue_cost",     CCLabelTTF*,      m_blueCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "purple_cost",   CCLabelTTF*,      m_purpleCost);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "yellow_cost",   CCLabelTTF*,      m_yellowCost);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "red_cost",      CCLabelTTF*,      m_redCost);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "diamond",       CCLabelTTF*,      m_diamondAmount);
    return false;
}

void SpriteExtractLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    setTouchEnabled(true);
    InitLayer();

    m_label5->setString(Localizatioin::getLocalization("M_FAIRYSHOP_REMAIN"));

    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    // int greenCost  = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21070);
    // int blueCost   = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21071);
    // int purpleCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21072);
    // int yellowCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21073);
    int greenCost = MarketManager::Get()->GetExchangePrice(21070);
    //int blueCost = MarketManager::Get()->GetExchangePrice(21071);
    int purpleCost = MarketManager::Get()->GetExchangePrice(21072);
	int yellowCost = MarketManager::Get()->GetExchangePrice(21073);
	int redCost = MarketManager::Get()->GetExchangePrice(21074);

    m_greenCost->setString(CCString::createWithFormat("%d", greenCost)->getCString());
    //m_blueCost->setString(CCString::createWithFormat("%d", blueCost)->getCString());
    m_purpleCost->setString(CCString::createWithFormat("%d", purpleCost)->getCString());
	m_yellowCost->setString(CCString::createWithFormat("%d", yellowCost)->getCString());
	m_redCost->setString(CCString::createWithFormat("%d", redCost)->getCString());

    m_spriteListBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_FAIRY_DATABASE_TITLE1")), CCControlStateNormal);

    // 初始化探寻卷的数量
/*    unsigned int count = ItemManager::Get()->getItemNumberById(416021);*/
    USERINFO info = UserData::GetUserInfo();
    m_greenLabel->setString(CCString::createWithFormat("%d", info.m_green_extract_times)->getCString());

    if (info.m_green_extract_times > 0)
    {
        m_greenNote->setVisible(true);
    } else
    {
        m_greenNote->setVisible(false);
    }

    // 添加新手指引相关参数
    m_uiLayer = new SpriteExtractUILayer(this);
    m_uiLayer->setInfoLayer(this);
    UIManager::sharedManager()->AddLayout(m_uiLayer, "sprite_extract");

    m_diamondAmount->setString(CCString::createWithFormat("%d", UserData::GetDiamond())->getCString());

    // 向服务器请求商品价格
    SendGetPrices();
}

bool SpriteExtractLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void SpriteExtractLayer::InitLayer()
{
	InitPositionAndVisible();
    InitNote();
    InitTime();
    InitPurpleDesc();
}

void SpriteExtractLayer::InitPositionAndVisible(){
	bool chouka_teshu = LuaTinkerManager::Get()->getLuaConfig<bool>("vip", "Vip", UserData::GetVipLevel(), "chouka_teshu");

	if (!chouka_teshu)
	{
		m_extractLayer5->setVisible(false);
		m_extractLayer5->setTouchEnabled(false);

		CCPoint posBegin = m_extractLayer1->getPosition();
		CCPoint posEnd = m_extractLayer5->getPosition();

		m_extractLayer1->setPositionX(posBegin.x + 50);
		m_extractLayer4->setPositionX(posEnd.x - 50);
		m_extractLayer3->setPositionX((posBegin.x + posEnd.x)/2);
	}
}

void SpriteExtractLayer::InitPurpleDesc()
{
    USERINFO info = UserData::GetUserInfo();
    unsigned int times = info.m_spec_left_times;

    if (times > 0)
    {
        m_purpleDesc->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_FAIRY_PUMPING"), times)->getCString());
    } else
    {
        m_purpleDesc->setString(Localizatioin::getLocalization("M_FAIRY_MUSTBEPURPLE"));
    }
}

void SpriteExtractLayer::InitTime()
{
    USERINFO info = UserData::GetUserInfo();

    //long junior_time = info.m_junior_explore_time;
    long senior_time = info.m_senior_explore_time;

    long nowTime = TimeManager::Get()->getCurServerTime();

    bool flag = false;
    /*
	if (junior_time > nowTime)
    {
        CCString* pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_FAIRYSHOP_TIMEFREE"), GetTimeStr(junior_time - nowTime).c_str());
        m_blueLabel->setString(pStr->getCString());
        m_blueNote->setVisible(false);
        flag = true;
    } else
    {
        CCString* pStr = CCString::createWithFormat("%s 1/1", Localizatioin::getLocalization("M_FAIRYSHOP_FREETODAY"));
        m_blueLabel->setString(pStr->getCString());
        m_blueNote->setVisible(true);
    }
	*/

    if (senior_time > nowTime)
    {
        CCString* pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_FAIRYSHOP_TIMEFREE"), GetTimeStr(senior_time - nowTime).c_str());
        m_purpleLabel->setString(pStr->getCString());
        m_purpleNote->setVisible(false);
        flag = true;
    } else
    {
        CCString* pStr = CCString::createWithFormat("%s 1/1", Localizatioin::getLocalization("M_FAIRYSHOP_FREETODAY"));
        m_purpleLabel->setString(pStr->getCString());
        m_purpleNote->setVisible(true);
    }

    if (flag == true)
    {
        unschedule(schedule_selector(SpriteExtractLayer::updateTime));
        schedule(schedule_selector(SpriteExtractLayer::updateTime), 1.0f, kCCRepeatForever, 0.0f);
    }
}

// 请求商品价格
void SpriteExtractLayer::SendGetPrices()
{
    std::vector<int> exchangeIds;
    exchangeIds.clear();
    exchangeIds.push_back(21071);
    exchangeIds.push_back(21072);
	exchangeIds.push_back(21073);
	exchangeIds.push_back(21074);
    OnlineNetworkManager::sShareInstance()->SendGetExchangeParameters(exchangeIds);
}

void SpriteExtractLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_UPDATE_EXCHANGE_PARAMETER :
        {
            RefreshPrice();
            break;
        }

    default:
        break;
    }
}

void SpriteExtractLayer::updateTime(float f)
{
    USERINFO info = UserData::GetUserInfo();

    //long junior_time = info.m_junior_explore_time;
    long senior_time = info.m_senior_explore_time;

    bool flag = false;

    long nowTime = TimeManager::Get()->getCurServerTime();
    /*
	if (junior_time > nowTime)
    {
        CCString* pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_FAIRYSHOP_TIMEFREE"), GetTimeStr(junior_time - nowTime).c_str());
        m_blueLabel->setString(pStr->getCString());
        m_blueNote->setVisible(false);
        flag = true;
    } else
    {
        CCString* pStr = CCString::createWithFormat("%s 1/1", Localizatioin::getLocalization("M_FAIRYSHOP_FREETODAY"));
        m_blueLabel->setString(pStr->getCString());
        m_blueNote->setVisible(true);
    }
	*/

    if (senior_time > nowTime)
    {
        CCString* pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_FAIRYSHOP_TIMEFREE"), GetTimeStr(senior_time - nowTime).c_str());
        m_purpleLabel->setString(pStr->getCString());
        m_purpleNote->setVisible(false);
        flag = true;
    } else
    {
        CCString* pStr = CCString::createWithFormat("%s 1/1", Localizatioin::getLocalization("M_FAIRYSHOP_FREETODAY"));
        m_purpleLabel->setString(pStr->getCString());
        m_purpleNote->setVisible(true);
    }

    if (flag == false)
    {
        unschedule(schedule_selector(SpriteExtractLayer::updateTime));
    }
}

void SpriteExtractLayer::InitNote()
{
    // green
    setGreenNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));
    m_greenNote->setPosition(ccp(20, 0));
    m_greenLabel->getParent()->addChild(m_greenNote);

    CCActionInterval* action1 = NULL;
    action1 = CCMoveBy::create(0.4,ccp(0,10));

    CCActionInterval* action_back1 = action1->reverse();
    CCActionInterval* seq1 = (CCActionInterval*)(CCSequence::create( action1, action_back1, NULL ));

    CCRepeatForever* pAction1 = CCRepeatForever::create(seq1);
    m_greenNote->runAction(pAction1);

    // blue
	/*
    setBlueNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));
    m_blueNote->setPosition(ccp(20, 0));
    m_blueLabel->getParent()->addChild(m_blueNote);

    CCActionInterval* action2 = NULL;
    action2 = CCMoveBy::create(0.4,ccp(0,10));

    CCActionInterval* action_back2 = action2->reverse();
    CCActionInterval* seq2 = (CCActionInterval*)(CCSequence::create( action2, action_back2, NULL ));

    CCRepeatForever* pAction2 = CCRepeatForever::create(seq2);
	m_blueNote->runAction(pAction2);
	*/

    // purple
    setPurpleNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));
    m_purpleNote->setPosition(ccp(20, 0));
    m_purpleLabel->getParent()->addChild(m_purpleNote);

    CCActionInterval* action3 = NULL;
    action3 = CCMoveBy::create(0.4,ccp(0,10));

    CCActionInterval* action_back3 = action3->reverse();
    CCActionInterval* seq3 = (CCActionInterval*)(CCSequence::create( action3, action_back3, NULL ));

    CCRepeatForever* pAction3 = CCRepeatForever::create(seq3);
    m_purpleNote->runAction(pAction3);
}

void SpriteExtractLayer::RefreshPrice()
{
    int greenCost = MarketManager::Get()->GetExchangePrice(21070);
    //int blueCost = MarketManager::Get()->GetExchangePrice(21071);
    int purpleCost = MarketManager::Get()->GetExchangePrice(21072);
    int yellowCost = MarketManager::Get()->GetExchangePrice(21073);
	int redCost = MarketManager::Get()->GetExchangePrice(21074);

    m_greenCost->setString(CCString::createWithFormat("%d", greenCost)->getCString());
    //m_blueCost->setString(CCString::createWithFormat("%d", blueCost)->getCString());
    m_purpleCost->setString(CCString::createWithFormat("%d", purpleCost)->getCString());
	m_yellowCost->setString(CCString::createWithFormat("%d", yellowCost)->getCString());
	m_redCost->setString(CCString::createWithFormat("%d", redCost)->getCString());
}

std::string SpriteExtractLayer::GetTimeStr(long _time)
{
    char timeString[30];
    unsigned int hour = _time / 3600;
    unsigned int min = (_time - hour * 3600) / 60;
    unsigned int second = _time % 60;
    if(hour > 0)
    {
        sprintf(timeString,"%02d : %02d : %02d",hour,min,second);
    }
    else
    {
        sprintf(timeString,"%02d : %02d",min,second);
    }

    return std::string(timeString);
}

void SpriteExtractLayer::onEnter()
{
    CCLayer::onEnter();

    m_extractBtn1->setTouchPriority(kCCScrollMenuPriority);
    //m_extractBtn2->setTouchPriority(kCCScrollMenuPriority);
    m_extractBtn3->setTouchPriority(kCCScrollMenuPriority);
	m_extractBtn4->setTouchPriority(kCCScrollMenuPriority);
	m_extractBtn5->setTouchPriority(kCCScrollMenuPriority);

    m_spriteListBtn->setTouchPriority(kCCScrollMenuPriority);
}

void SpriteExtractLayer::onExtract1(CCObject* sender, CCControlEvent pControlEvent)
{
    OnlineNetworkManager::sShareInstance()->SendExtractSprite(1);

    GameAudioManager::sharedManager()->playEffect(350000,false);
    TutorialsManager::Get()->HandleOneEvent("extract_1", 1);
}

/*
void SpriteExtractLayer::onExtract2(CCObject* sender, CCControlEvent pControlEvent)
{
    USERINFO info = UserData::GetUserInfo();
    long junior_time = info.m_junior_explore_time;
    long nowTime = TimeManager::Get()->getCurServerTime();
    if (junior_time < nowTime)
    {
        GameAudioManager::sharedManager()->playEffect(350000,false);
        OnlineNetworkManager::sShareInstance()->SendExtractSprite(2);
    } else
    {
        // 检查钻石
        // int blueCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21071);
        int blueCost = MarketManager::Get()->GetExchangePrice(21071);
        if (UserData::GetDiamond() < blueCost)
        {
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",blueCost);
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
            return;
        }

        GameAudioManager::sharedManager()->playEffect(350000,false);
        OnlineNetworkManager::sShareInstance()->SendExtractSprite(3);
    }

    TutorialsManager::Get()->HandleOneEvent("extract_2", 1);
}
*/

void SpriteExtractLayer::onExtract3(CCObject* sender, CCControlEvent pControlEvent)
{
    USERINFO info = UserData::GetUserInfo();
    long senior_time = info.m_senior_explore_time;
    long nowTime = TimeManager::Get()->getCurServerTime();
    if (senior_time < nowTime)
    {
        GameAudioManager::sharedManager()->playEffect(350000,false);
        OnlineNetworkManager::sShareInstance()->SendExtractSprite(4);
    } else
    {   
        // 检查钻石
        // int purpleCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21072);
        int purpleCost = MarketManager::Get()->GetExchangePrice(21072);
        if (UserData::GetDiamond() < purpleCost)
        {
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",purpleCost);
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
            return;
        }

        GameAudioManager::sharedManager()->playEffect(350000,false);
        OnlineNetworkManager::sShareInstance()->SendExtractSprite(5);
    }

    TutorialsManager::Get()->HandleOneEvent("extract_3", 1);
}

void SpriteExtractLayer::onExtract4(CCObject* sender, CCControlEvent pControlEvent)
{
    // 检查钻石
    // int yellowCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21073);
    int yellowCost = MarketManager::Get()->GetExchangePrice(21073);
    if (UserData::GetDiamond() < yellowCost)
    {
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",yellowCost);
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
        return;
    }

    GameAudioManager::sharedManager()->playEffect(350000,false);
    OnlineNetworkManager::sShareInstance()->SendExtractSprite(6);
    TutorialsManager::Get()->HandleOneEvent("extract_4", 1);
}

void SpriteExtractLayer::onExtract5(CCObject* sender, CCControlEvent pControlEvent)
{
	// 检查钻石
	// int redCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21074);
	int redCost = MarketManager::Get()->GetExchangePrice(21074);
	if (UserData::GetDiamond() < redCost)
	{
		LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",redCost);
		LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
		return;
	}

	GameAudioManager::sharedManager()->playEffect(350000,false);
	OnlineNetworkManager::sShareInstance()->SendExtractSprite(7);
	TutorialsManager::Get()->HandleOneEvent("extract_5", 1);
}

void SpriteExtractLayer::onClickSpriteList(CCObject* sender, CCControlEvent event)
{
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/FairyLayer/FairyCollectionLayer.lua","CreateFairyCollectionLayer");
}

SpriteExtractUILayer::SpriteExtractUILayer(CCNode* parent) :
    m_infoLayer(NULL)
{
    UILayerColorBase* pLayer =  new UILayerColorBase(true, true, ccc4(255,255,255,0));
    pLayer->init();
    pLayer->autorelease();
    pLayer->setPosition(CCPointZero);
    UILayout::m_pControlNode = pLayer;

    CCNode *pNode = getCurrentNode();
    parent->addChild(pNode);
}

SpriteExtractUILayer::~SpriteExtractUILayer()
{

}

CCNode* SpriteExtractUILayer::getTutorialNode(std::string name)
{
    if (strcmp("extract_1", name.c_str()) == 0)
    {
        return m_infoLayer->getExtractBtn1();
    } else if (strcmp("extract_2", name.c_str()) == 0)
    {
        //return m_infoLayer->getExtractBtn2();
    } else if (strcmp("extract_3", name.c_str()) == 0)
    {
        return m_infoLayer->getExtractBtn3();
    } else if (strcmp("extract_4", name.c_str()) == 0)
    {
        return m_infoLayer->getExtractBtn4();
	} else if (strcmp("extract_5", name.c_str()) == 0)
	{
		return m_infoLayer->getExtractBtn5();
	} else if (strcmp("close", name.c_str()) == 0)
    {
        MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
        if (mLayer)
        {
            MarketLayer* layer = dynamic_cast<MarketLayer*>(mLayer->getMarketLayer());
            return layer->getCloseBtn();
        }
    }

    return NULL;
}