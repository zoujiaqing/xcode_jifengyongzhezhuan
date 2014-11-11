#include "OnlineNetworkManager.h"
#include "TreasureExploreLayer.h"
#include "TreasureExploreManager.h"
#include "Localization.h"
#include "CCBLoaderManager.h"
#include "CallNativeFuncManager.h"
#include "InterfaceManager.h"
#include "GlobalFunction.h"
#include "ItemManager.h"
#include "ParticleManager.h"
#include "UIManager.h"
#include "LuaTinkerManager.h"
#include "MainMenuLayer.h"
#include "SkeletonAnimRcsManager.h"
#include "GameAudioManager.h"
#include "SoundEnum.h"
#include "TutorialsManager.h"

TreasureExploreLayer::TreasureExploreLayer()
    :  m_pTitleLabel(NULL),
       m_pLabel1(NULL),	
       m_pLabel2(NULL),	
       m_pLabel3(NULL),	
       m_pLabel4(NULL),	
       m_pLabel5(NULL),	
       m_pLabel6(NULL),
       m_pLabel7(NULL),
       m_pCloseBtn(NULL),   
       m_pExploreBtn(NULL), 
       m_pGetRewardBtn(NULL),
       m_pCurMapName(NULL),
       m_pSuccessNum(NULL),
       m_pMaterialNum(NULL),
       m_pDiamondNum(NULL),
       m_pRemainNum(NULL),
       m_pProgressTimer(NULL),
       m_pRateLayer(NULL),
       m_pPercentSprite1(NULL),
       m_pPercentSprite2(NULL),
       m_pPercentSprite3(NULL),
       m_pRateEffectLayer(NULL),
       m_pTableViewLayer(NULL),
       m_pTableView(NULL),	 
       m_pbgSprite(NULL),
       m_pLabel8(NULL),	 
       m_pLabel9(NULL),
       m_pDiamondOwn(NULL),
       m_pMaterialOwn(NULL),
       m_pBallEffectLayer(NULL),
       m_curMapIndex(1),
       m_curPercentage(0),
       m_curExploreTimes(0),
       m_durationTime(0.5),
       m_exploreClicked(false),
       m_rewardClicked(false),
       m_uiLayer(NULL),
       m_pRefreshBtn(NULL),
       m_pLabel10(NULL),	    
       m_pRefreshDiamond(NULL),
       //m_pRefreshTimes(NULL),	
       m_pWordRefresh(NULL),   
	   m_pWordUpgrade(NULL), 
	   m_pLabel11(NULL),
	   m_pResetTimes(NULL),
	   m_pLabel12(NULL)
{
    m_singleVec.clear();
	m_singleVec.resize(10);
	m_refreshAwardVec.clear();
	m_refreshAwardVec.resize(4);
    m_singleMapsVec.clear();
}
TreasureExploreLayer::~TreasureExploreLayer()
{
    setTitleLabel(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
    setLabel4(NULL);
    setLabel5(NULL);
    setLabel6(NULL);
    setLabel7(NULL);
    setCloseBtn(NULL);
    setExploreBtn(NULL);
    setGetRewardBtn(NULL);
    setCurMapName(NULL);
    setSuccessNum(NULL);
    setMaterialNum(NULL);
    setDiamondNum(NULL);
    setRemainNum(NULL);
    setProgressTimer(NULL);
    setRateLayer(NULL);
    setPercentSprite1(NULL);
    setPercentSprite2(NULL);
    setPercentSprite3(NULL);
    setRateEffectLayer(NULL);
    setTableView(NULL);
    setTableViewLayer(NULL);
    setBgSprite(NULL);
    setDiamondOwn(NULL);
    setMaterialOwn(NULL);
    setLabel8(NULL);
    setLabel9(NULL);
    setRefreshBtn(NULL);
    setLabel10(NULL);    
    setRefreshDiamond(NULL);
    //setRefreshTimes(NULL);	
    setWordRefresh(NULL); 
    setWordUpgrade(NULL);  
    setBallEffectLayer(NULL);
	setLabel11(NULL);
	setLabel12(NULL);
	setResetTimes(NULL);
    UIManager::sharedManager()->RemoveLayout("TreasureExploreLayer");
    delete m_uiLayer;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "updateTreasureInfo");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "refreshTreasureItems");    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "treasure_upgrade_success");
}
SEL_MenuHandler TreasureExploreLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
SEL_CCControlHandler TreasureExploreLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",  TreasureExploreLayer::OnCloseBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "explore",    TreasureExploreLayer::OnExploreBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "get_reward", TreasureExploreLayer::OnGetRewardBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "ball_click", TreasureExploreLayer::onBallClicked);

    return NULL;
}
bool TreasureExploreLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleLabel", CCLabelTTF*, m_pTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel1",     CCLabelTTF*, m_pLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel2",     CCLabelTTF*, m_pLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel3",     CCLabelTTF*, m_pLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel4",     CCLabelTTF*, m_pLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel5",     CCLabelTTF*, m_pLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel6",     CCLabelTTF*, m_pLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel7",     CCLabelTTF*, m_pLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel8",     CCLabelTTF*, m_pLabel8);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel9",     CCLabelTTF*, m_pLabel9);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel12",     CCLabelTTF*, m_pLabel12);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",      CCControlButton*, m_pCloseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExploreBtn",  CCControlButton*, m_pExploreBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGetRewardBtn",CCControlButton*, m_pGetRewardBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurMapName",  CCLabelTTF*, m_pCurMapName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSuccessNum",  CCLabelTTF*, m_pSuccessNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMaterialNum", CCLabelTTF*, m_pMaterialNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDiamondNum",  CCLabelTTF*, m_pDiamondNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRemainNum",   CCLabelTTF*, m_pRemainNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMaterialOwn", CCLabelTTF*, m_pMaterialOwn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDiamondOwn",  CCLabelTTF*, m_pDiamondOwn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single1",        SingleTreasureLayer*, m_singleVec[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single2",        SingleTreasureLayer*, m_singleVec[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single3",        SingleTreasureLayer*, m_singleVec[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single4",        SingleTreasureLayer*, m_singleVec[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single5",        SingleTreasureLayer*, m_singleVec[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single6",        SingleTreasureLayer*, m_singleVec[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single7",        SingleTreasureLayer*, m_singleVec[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single8",        SingleTreasureLayer*, m_singleVec[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single9",        SingleTreasureLayer*, m_singleVec[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single10",       SingleTreasureLayer*, m_singleVec[9]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refreshAward1",       SingleTreasureLayer*, m_refreshAwardVec[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refreshAward2",       SingleTreasureLayer*, m_refreshAwardVec[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refreshAward3",       SingleTreasureLayer*, m_refreshAwardVec[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refreshAward4",       SingleTreasureLayer*, m_refreshAwardVec[3]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRateLayer",        CCLayer*,              m_pRateLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRateEffectLayer",  CCLayer*,              m_pRateEffectLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPercentSprite1",   CCSprite*,             m_pPercentSprite1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPercentSprite2",   CCSprite*,             m_pPercentSprite2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPercentSprite3",   CCSprite*,             m_pPercentSprite3);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pbgSprite",         CCSprite*,             m_pbgSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "n_pRefreshbtn",       CCControlButton*,      m_pRefreshBtn);

    // table view
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableViewLayer",   CCLayer*,              m_pTableViewLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableView",        CCTableView*,          m_pTableView);

    // refresh
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel10",          CCLabelTTF*,          m_pLabel10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabel11",			 CCLabelTTF*,		   m_pLabel11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRefreshDiamond",   CCLabelTTF*,          m_pRefreshDiamond);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRefreshTimes",     CCLabelTTF*,          m_pRefreshTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWordRefresh",      CCSprite*,            m_pWordRefresh);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWordUpgrade",      CCSprite*,            m_pWordUpgrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBallEffectLayer",  CCLayer*,             m_pBallEffectLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pResetTimes",       CCLabelTTF*,          m_pResetTimes);
    return true;
}
void TreasureExploreLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    this->setTouchEnabled(true);
    this->InitMultiLanguage();
    this->InitItems();
    this->AddProgressBar();
    this->AddTableView();

    // set prioriry 
    m_pExploreBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pGetRewardBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pCloseBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pRefreshBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pMaterialOwn->setString(IntToStr(0));

	m_refreshAwardVec[0]->InitLayer(0,442086,1);
	m_refreshAwardVec[1]->InitLayer(0,442111,1);
	m_refreshAwardVec[2]->InitLayer(0,442113,1);
	m_refreshAwardVec[3]->InitLayer(0,442112,1);

    OnlineNetworkManager::sShareInstance()->SendGetTreasureExploreInfo();

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TreasureExploreLayer::OnReceiveTreasureInfoUpdate), "updateTreasureInfo", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TreasureExploreLayer::onRefreshItems), "refreshTreasureItems", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TreasureExploreLayer::onUpgradeSuccess), "treasure_upgrade_success", NULL);

    // 添加新手指引相关参数
    m_uiLayer = new TreasureExploreUILayer(this);
    m_uiLayer->setInfoLayer(this);
    UIManager::sharedManager()->AddLayout(m_uiLayer, "TreasureExploreLayer");
}

bool TreasureExploreLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void TreasureExploreLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
}
void TreasureExploreLayer::onEnter()
{
    CCLayer::onEnter();
}
void TreasureExploreLayer::onExit()
{
    CCLayer::onExit();
    this->stopAllActions();
    this->unscheduleAllSelectors();
}

void TreasureExploreLayer::OnReceiveTreasureInfoUpdate(CCObject* Data)
{
    // update success_rate, times, mapid
    USERINFO info = UserData::GetUserInfo();
    int rate = info.m_treasure_explore_success_rate;
    int mapindex = info.m_treasure_explore_mapIndex;
    int times = info.m_treasure_explore_times;
    int alreadyIndex = info.m_treausre_already_get_index;
    int diamond = UserData::GetDiamond();

    // show reward msg
    ShowGetMessage(alreadyIndex);
    
    // diamond
    m_pDiamondOwn->setString(MainMenuLayer::GetNumByFormat(diamond));
    m_pMaterialOwn->setString(MainMenuLayer::GetNumByFormat(ItemManager::Get()->getItemNumberById(416022)));

    // rate
    m_curPercentage = rate;
    bool flag = false;
    if((mapindex - m_curMapIndex) > 0)
    {
        flag = true;
    }
    SetSuccessRate(m_curPercentage, flag);

    // map
    if(m_curMapIndex != mapindex)
    {
        SetCurMap(mapindex);
    }    
    m_exploreClicked = false;

    // big map
    int tempNum = mapindex;
    if(tempNum > 10)
    {
        tempNum = 10;
    }
    CCSpriteFrame* frame = TreasureExploreManager::Get()->GetMapSpriteFrame(tempNum);
    if(frame)
    {
        m_pbgSprite->setDisplayFrame(frame);
    }
    // name
    CCString* strName = CCString::createWithFormat(Localizatioin::getLocalization("M_TREASURE_NAME"), tempNum);
    m_pCurMapName->setString(strName->getCString());

    // times
    m_curExploreTimes = times;
    SetExploreRemainTimes(m_curExploreTimes);

    SetAlreadyGetIndex(alreadyIndex, mapindex);

    InitRefresh();
}

void TreasureExploreLayer::InitRefresh()
{
    // 刷新等级次数 和 开启等级
    int refreshOpenLv = TreasureExploreManager::Get()->getRefreshOpenLv();
	int resetTimes = TreasureExploreManager::Get()->getResetTimes();
	m_pResetTimes->setString(CCString::createWithFormat("%d", resetTimes)->getCString());


    //int refreshTimes = TreasureExploreManager::Get()->getRefreshTimes();
    //int isUpgrade = UserData::GetUserInfo().m_treasure_levelup_flag;
    //if(UserData::GetUserLevel() >= refreshOpenLv && isUpgrade != 0)
    {
        // refresh
        m_pWordRefresh->setVisible(true);
        m_pWordUpgrade->setVisible(false);
        /*
		CCString* timesStr = CCString::createWithFormat(Localizatioin::getLocalization("G_NUMBER"), refreshTimes);
		m_pRefreshTimes->setString(timesStr->getCString());
        if(refreshTimes <= 0)
        {
            m_pRefreshTimes->setColor(SYS_FONT_RED);
        }
        else
        {
            m_pRefreshTimes->setColor(SYS_FONT_WHITE);
        }
		*/

        std::string refreshDiamond = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", REFRESH_DIAMON, "FromItems");
        int index = refreshDiamond.find_first_of("/");
        int refresgDiamondNum = atoi(refreshDiamond.substr(index + 1, refreshDiamond.length() - index - 1).c_str());
        m_pRefreshDiamond->setString(IntToStr(refresgDiamondNum));
    } 
	/*
    else
    {
        // upgrade
        m_pWordRefresh->setVisible(false);
        m_pWordUpgrade->setVisible(true);
        
		m_pRefreshTimes->setColor(SYS_FONT_WHITE);
        CCString* lVStr = CCString::createWithFormat(Localizatioin::getLocalization("M_TREASURE_OPENLEVER"), refreshOpenLv);
        m_pRefreshTimes->setString(lVStr->getCString());

        std::string upgradeDiamond = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", UPGRADE_DIAMON, "FromItems");
        int index = upgradeDiamond.find_first_of("/");
        int upgradeDiamondNum = atoi(upgradeDiamond.substr(index + 1, upgradeDiamond.length() - index - 1).c_str());
        m_pRefreshDiamond->setString(IntToStr(upgradeDiamondNum));
    }
	*/
}

void TreasureExploreLayer::onUpgradeSuccess(CCObject* Data)
{
    InitRefresh();

    /*for(int i = 0; i < m_singleVec.size(); i++)
    {
        m_singleVec[i]->showUpgradeEffect();
    }*/
}

void TreasureExploreLayer::ShowGetMessage(int alreadyIndex)
{
    if(m_rewardClicked == true)
    {
        m_rewardClicked = false;
        int forwardIdx = TreasureExploreManager::Get()->getForwardAlreadyIdx();
        for(int i = forwardIdx + 1; i <= alreadyIndex; i++)
        {
            int itemid = TreasureExploreManager::Get()->GetTreasureItemId(i);
            int itemCount = TreasureExploreManager::Get()->GetTreasureItemCount(i);
            std::string itemName = ItemManager::Get()->getItemInfoById(itemid)->m_name;
            std::string tempStr = Localizatioin::getLocalization("M_PK_INTRODUCTION_2");
            tempStr.append(" ").append(itemName.c_str()).append(" x ").append(IntToStr(itemCount));
            ShowMessage(tempStr.c_str());
        }
    }
    TreasureExploreManager::Get()->setForwardAlreadyIdx(alreadyIndex);
}

void TreasureExploreLayer::InitMultiLanguage()
{
    // labels 
    m_pTitleLabel->setString(Localizatioin::getLocalization("M_TREASURE_TITLE"));
    m_pLabel1->setString(Localizatioin::getLocalization("M_TREASURE_RESET"));
    m_pLabel2->setString(Localizatioin::getLocalization("M_TREASURE_QUANTITY"));
    m_pLabel3->setString(Localizatioin::getLocalization("M_TREASURE_PROBABILITY"));
    m_pLabel4->setString(Localizatioin::getLocalization("M_TREASURE_FULL"));
    m_pLabel5->setString(Localizatioin::getLocalization("M_TREASURE_EXPEND"));
    m_pLabel6->setString(Localizatioin::getLocalization("M_FINDENEMY_1"));
    m_pLabel7->setString(Localizatioin::getLocalization("M_TREASURE_FREE"));
    m_pLabel8->setString(Localizatioin::getLocalization("M_TREASURE_NOW"));
    m_pLabel9->setString(Localizatioin::getLocalization("M_TREASURE_RESET2"));
	m_pLabel10->setString(Localizatioin::getLocalization("G_COST"));
	m_pLabel11->setString(Localizatioin::getLocalization("M_TREASURE_RESET4"));
	m_pLabel12->setString(Localizatioin::getLocalization("M_TREASURE_RESET3"));
    
    // buttons
    m_pExploreBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_TREASURE_TITLE")), CCControlStateNormal);
    m_pGetRewardBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_LINGQUJIANGLI")), CCControlStateNormal);
}

void TreasureExploreLayer::InitItems()
{
    // init consume diamond
    std::string materials = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", MATERIAL_NUM, "FromItems");
    int index = materials.find_first_of("/");
    int materialsNum = atoi(materials.substr(index + 1, materials.length() - index - 1).c_str());
    m_pMaterialNum->setString(IntToStr(materialsNum));
    
    std::string diamonds = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", DIAMOND_NUM, "FromItems");
    int index2 = diamonds.find_first_of("/");
    int diamondsNum = atoi(diamonds.substr(index + 1, diamonds.length() - index - 1).c_str());
    m_pDiamondNum->setString(IntToStr(diamondsNum));

    onRefreshItems(NULL);

    // add ball effect
    m_pBallEffectLayer->removeAllChildren();
    SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(452);
    EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(452,"",false);
    effect1->setPosition(ccp(m_pBallEffectLayer->getContentSize().width * 0.5f, m_pBallEffectLayer->getContentSize().height * 0.5f));
    effect1->SetAnim(kType_Play,1,true);
    m_pBallEffectLayer->addChild(effect1);
}

void TreasureExploreLayer::onRefreshItems(CCObject* Data)
{
    // init base items
    //for(int i = 0; i < m_singleVec.size(); i++)
    //{
    //    int itemid = TreasureExploreManager::Get()->GetTreasureItemId(i + 1);
    //    int count = TreasureExploreManager::Get()->GetTreasureItemCount(i + 1);
    //    m_singleVec[i]->InitLayer(i + 1, itemid, count);
    //}
    InitRefresh();

    // init items
    std::vector<tresureItems> itemVec =  TreasureExploreManager::Get()->getTreasureItemsFromServer();
    for(int i = 0; i < m_singleVec.size(); i++)
    {
        if(i >= itemVec.size())
        {
            continue;
        }
        int itemid = itemVec[i].id;
        int count = itemVec[i].num;
        m_singleVec[i]->InitLayer(i + 1, itemid, count);
    }

	if (TreasureExploreManager::Get()->getIsReset())
	{
		MoveMapsToFirstAction();
		m_curMapIndex = 1;
		m_pSuccessNum->setString("0 / 10");
		ShowMessage(Localizatioin::getLocalization("M_TREASURE_RESET5"));

        // play reset effect
        for(int i = 0; i < m_singleVec.size(); i++)
        {
            m_singleVec[i]->showUpgradeEffect();
        }
	}
}

void TreasureExploreLayer::onBallClicked(CCObject* pSender, CCControlEvent event)
{
    GameAudioManager::sharedManager()->playEffect(SOUND_EFFECT_CLICK, false);
    if(m_pRefreshBtn->getTag() != 0)
    {
        return;
    }
    m_pRefreshBtn->setTag(1);

	/*
    //int isUpgrade = UserData::GetUserInfo().m_treasure_levelup_flag;
    //if(isUpgrade == 0)
    {
        OnlineNetworkManager::sShareInstance()->SendExploreTreasure(UPGRADE_DIAMON);
    }
	else
	*/
    {
        OnlineNetworkManager::sShareInstance()->SendExploreTreasure(REFRESH_DIAMON);
    }

    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(TreasureExploreLayer::BackExploreBtnPrority),this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TreasureExploreLayer::BackExploreBtnPrority),
        this, 0.6,1,0.6,false);
}

void TreasureExploreLayer::AddTableView()
{
    if(this->m_pTableView)
    {
        this->m_pTableView->removeFromParentAndCleanup(true);
        setTableView(NULL);
    }

    CCSize size = CCSizeMake(this->m_pTableViewLayer->getContentSize().width, this->m_pTableViewLayer->getContentSize().height);
    CCTableView* tableview = CCTableView::create(this, size);
    tableview->setDirection(kCCScrollViewDirectionHorizontal);
    tableview->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableview->setDelegate(this);
    tableview->setBounceable(false);
    setTableView(tableview);
    this->m_pTableViewLayer->addChild(m_pTableView);
    m_pTableView->setTouchPriority(kCCLayerMenuTouchPriority);
    this->m_pTableView->reloadData();
}

void TreasureExploreLayer::PutMapSpriteToTableViewCell(CCTableViewCell* cell)
{
    int mapindex = 1;
    m_singleMapsVec.clear();
    
    // add map sprite
    for(int i = 1; i <= 10; i++)
    {
        CCSpriteFrame* frame = TreasureExploreManager::Get()->GetMapSpriteFrame(i);
        if(!frame)
        {
            continue;
        }
        SingleTreasureMapLayer* mapLayer = createSingleTreasureMapLayer();
        if(!mapLayer)
        {
            return;
        }
        mapLayer->getMapSprite()->setDisplayFrame(frame);
        mapLayer->getBackSprite()->setVisible(false);
        if(i < mapindex)
        {
            mapLayer->setScale(global_scale);
        }
        else if(i > mapindex)
        {
            mapLayer->setScale(global_scale);
        }

        cell->addChild(mapLayer, i, i);
        mapLayer->setAnchorPoint(ccp(0.5, 0.5));
        m_singleMapsVec.push_back(mapLayer);
    }

    // reset position
    if(m_singleMapsVec.size() != 10)
    {
        return;
    }
    for(int j = 0; j < 10; j++)
    {
        if(j == 0)
        {
            m_singleMapsVec[j]->setPosition(ccp(m_pTableViewLayer->getContentSize().width / 2, m_pTableViewLayer->getContentSize().height / 2));
            continue;
        }
        m_singleMapsVec[j]->setPosition(ccp((m_singleMapsVec[j - 1]->getPosition().x) + (m_singleMapsVec[j - 1]->getContentSize().width / 2) + (m_singleMapsVec[j]->getContentSize().width * m_singleMapsVec[j - 1]->getScaleX() / 2), 
                                          m_pTableViewLayer->getContentSize().height / 2));
    }
}

void TreasureExploreLayer::MoveMapsToLeftAction()
{
    if(m_singleMapsVec.size() != 10)
    {
        return;
    }

    // collect map init pos
    std::vector<CCPoint> mapPosVec;
    mapPosVec.clear();
    for(int j = 0; j < 10; j++)
    {
        mapPosVec.push_back(m_singleMapsVec[j]->getPosition());
    }

    for(int i = 0; i < 10; i++)
    {
        if(i == 0)
        {
            // move pos first
            CCPoint targetPos = ccp((m_singleMapsVec[i]->getPosition().x) - (m_singleMapsVec[i]->getContentSize().width / 2) - (m_singleMapsVec[i]->getContentSize().width * m_singleMapsVec[i]->getScaleX() / 2),
                                m_pTableViewLayer->getContentSize().height / 2);
            CCAction* action = CCMoveTo::create(m_durationTime, targetPos);            
            m_singleMapsVec[i]->runAction(action);            

            // scale action 
            if(m_curMapIndex == i + 1)
            {
                CCAction* scale1 = CCScaleTo::actionWithDuration(m_durationTime, global_scale);
                CCAction* scale2 = CCScaleTo::actionWithDuration(m_durationTime, 1.0);
                m_singleMapsVec[i]->runAction(scale1);
                m_singleMapsVec[i + 1]->runAction(scale2);
            }            
            continue;
        }

        CCAction* action2 = CCMoveTo::create(m_durationTime, mapPosVec[i - 1]);
        m_singleMapsVec[i]->runAction(action2);
    }

    // scale action
    CCAction* scale1 = CCScaleTo::actionWithDuration(m_durationTime, global_scale);
    CCAction* scale2 = CCScaleTo::actionWithDuration(m_durationTime, 1.0);
    m_singleMapsVec[m_curMapIndex - 1]->runAction(scale1);
    m_singleMapsVec[m_curMapIndex]->runAction(scale2);
}

void TreasureExploreLayer::MoveMapsToFirstAction()
{
	if(m_singleMapsVec.size() != 10)
	{
		return;
	}

	if (m_curMapIndex == 1)
	{
		return;
	}

	float durationTime = 0.2f;

	CCPoint tmp;
	float w = m_singleMapsVec[0]->getContentSize().width;


	for(int i = 0; i < 10; i++)
	{
		if(i == 0){
			tmp = ccp(m_pTableViewLayer->getContentSize().width / 2, m_pTableViewLayer->getContentSize().height / 2);
		}else if(i == 1){
			tmp = ccp(tmp.x + w, tmp.y);
		}else{
			tmp = ccp(tmp.x + w /2 + w * global_scale / 2, tmp.y);
		}

		CCMoveTo* move = CCMoveTo::create(durationTime * (m_curMapIndex - 1), tmp);

		if(i == 0){
			CCActionInterval * delay = CCDelayTime::create(durationTime * (m_curMapIndex - 2));
			CCAction* scale2 = CCScaleTo::create(durationTime, 1.0f);
			CCFiniteTimeAction  * sequence = CCSequence::create(delay, scale2, NULL);
			m_singleMapsVec[i]->runAction(move);
			m_singleMapsVec[i]->runAction(sequence);
		}else if(i < m_curMapIndex - 1){
			CCActionInterval* delay = CCDelayTime::create(durationTime * (m_curMapIndex - i - 2));
			CCAction* scale1 = CCScaleTo::create(durationTime, global_scale);
			CCAction* scale2 = CCScaleTo::create(durationTime, 1.0f);
			CCFiniteTimeAction  * sequence = CCSequence::create(delay, scale2, scale1, NULL);
			m_singleMapsVec[i]->runAction(move);
			m_singleMapsVec[i]->runAction(sequence);
		}else if(i == m_curMapIndex - 1){
			CCAction* scale1 = CCScaleTo::create(durationTime, global_scale);
			m_singleMapsVec[i]->runAction(move);
			m_singleMapsVec[i]->runAction(scale1);
		}else{
			m_singleMapsVec[i]->runAction(move);
		}
		m_singleMapsVec[i]->getBackSprite()->setVisible(false);
	}
}

void TreasureExploreLayer::ConfigUI()
{

}

void TreasureExploreLayer::OnCloseBtnClicked(CCObject* pSender, CCControlEvent event)
{
    CallNativeFuncManager::getInstance()->ShowTreasureExploreLayer(false);
}

void TreasureExploreLayer::OnExploreBtnClicked(CCObject* pSender, CCControlEvent event)
{
    m_durationTime = DURATION;
    m_exploreClicked = true;
    m_pExploreBtn->setEnabled(false);
    OnlineNetworkManager::sShareInstance()->SendExploreTreasure(MATERIAL_NUM);

    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(TreasureExploreLayer::BackExploreBtnPrority),this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TreasureExploreLayer::BackExploreBtnPrority),
                                                this, 0.6,1,0.6,false);
    GameAudioManager::sharedManager()->playEffect(SOUND_EFFECT_CLICK, false);

    TutorialsManager::Get()->HandleOneEvent("explore", 1);
}

void TreasureExploreLayer::BackExploreBtnPrority(float dt)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(TreasureExploreLayer::BackExploreBtnPrority),this);
    m_pExploreBtn->setEnabled(true);
    m_pRefreshBtn->setTag(0);
}

void TreasureExploreLayer::OnGetRewardBtnClicked(CCObject* pSender, CCControlEvent event)
{
    m_exploreClicked = false;
    m_rewardClicked = true;
    OnlineNetworkManager::sShareInstance()->SendGetTreasureReward();
    GameAudioManager::sharedManager()->playEffect(SOUND_EFFECT_CLICK, false);
}

void TreasureExploreLayer::AddProgressBar()
{
    if(m_pProgressTimer == NULL)
    {
        CCProgressTimer* timer = InterfaceManager::Get()->CreateProgressTimer("ccbResources/treasureexplore.plist", "explore_progress.png");
        setProgressTimer(timer);

        m_pRateLayer->addChild(m_pProgressTimer);
        m_pProgressTimer->setType(kCCProgressTimerTypeBar);
        m_pProgressTimer->setAnchorPoint(ccp(0.0, 0.0));
        m_pProgressTimer->setBarChangeRate(ccp(1, 0));
        m_pProgressTimer->setMidpoint(ccp(0, m_pProgressTimer->getContentSize().width));
        m_pProgressTimer->setPercentage(m_curPercentage);
    }
}

void TreasureExploreLayer::SetSuccessRate(int rate, bool needEffect)
{
    if(rate > 100)
    {
        rate = 100;
    }
    // 百位，个位
    int num1 = 0;
    int num2 = 0;

    if(needEffect && m_exploreClicked == true)
    {
        // add effect
        m_pRateEffectLayer->removeAllChildren();
        SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(446);
        EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(446,"",false);
        effect1->setPosition(ccp(m_pRateEffectLayer->getContentSize().width * 0.5f, m_pRateEffectLayer->getContentSize().height * 0.5f));
        effect1->SetAnim(kType_Play,1,false);
        m_pRateEffectLayer->addChild(effect1);
        GameAudioManager::sharedManager()->playEffect(350013, false);

        // progress timer action
        CCFiniteTimeAction* timerAction = CCProgressTo::create(DURATION, 0);
        CCFiniteTimeAction* toNewAc = CCProgressTo::create(0.5, rate);
        m_pProgressTimer->runAction(CCSequence::create(timerAction, CCDelayTime::create(0.3), toNewAc,NULL));
    }
    else
    {
        // progress timer action
        CCAction* timerAction = CCProgressTo::create(DURATION, rate);
        m_pProgressTimer->runAction(timerAction);
    }

    if(rate == 100)
    {
        m_pPercentSprite1->setVisible(true);
    }
    else
    {
        m_pPercentSprite1->setVisible(false);
        int newRate = rate % 100;
        num1 = newRate / 10;
        num2 = newRate % 10;
    }

    CCString* str1 = CCString::createWithFormat("blue_%d.png", num1);
    CCString* str2 = CCString::createWithFormat("blue_%d.png", num2);

    CCSpriteFrame* frame1 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", str1->getCString());
    CCSpriteFrame* frame2 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", str2->getCString());
    m_pPercentSprite2->setDisplayFrame(frame1);
    m_pPercentSprite3->setDisplayFrame(frame2);
}

void TreasureExploreLayer::SetExploreRemainTimes(int times)
{
    USERINFO info = UserData::GetUserInfo();
    int mapindex = info.m_treasure_explore_mapIndex;
    int freetimes = TreasureExploreManager::Get()->GetCurMapFreeTimes(mapindex);
    int tempNum = freetimes - times;
    if(tempNum <= 0)
    {
        tempNum = 0;
    }
    m_pRemainNum->setString(IntToStr(tempNum));
}

void TreasureExploreLayer::SetCurMap(int mapindex)
{
    // num    
    std::string numStr = IntToStr(mapindex - 1);
    numStr.append(" / 10");
    m_pSuccessNum->setString(numStr.c_str());
    if(m_curMapIndex == 10)
    {
        return;
    }
    if(mapindex == 11 && (m_curMapIndex == 1))
    {
        mapindex = 10;
    }

    // init pos
    if(m_exploreClicked == false)
    {
        ResetMapInitPostion(mapindex);
        m_curMapIndex = mapindex;
        return;
    }


    // move maps to left action
    int a = m_curMapIndex;
    int count = 0;
    for(int i = a; i < mapindex; i++)
    {
        count ++;
    }  
    if(m_curMapIndex == 1 && count >= 2)
    {
        m_durationTime = 0.0f;
    }
    if(count > 0)
    {
        count--;
    }
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TreasureExploreLayer::DelayExacuteFunc), this,
        m_durationTime + 0.1, count, 0.01, false);
}

void TreasureExploreLayer::DelayExacuteFunc(float dt)
{
    MoveMapsToLeftAction();
    m_curMapIndex ++;
}

void TreasureExploreLayer::ResetMapInitPostion(int mapindex)
{
    m_singleMapsVec[mapindex - 1]->setPosition(ccp(m_pTableViewLayer->getContentSize().width / 2, m_pTableViewLayer->getContentSize().height / 2));
    m_singleMapsVec[mapindex - 1]->setScale(1.0);

    for(int j = mapindex - 2; j >= 0; j--)
    {
        m_singleMapsVec[j]->setScale(global_scale);
        m_singleMapsVec[j]->setPosition(ccp((m_singleMapsVec[j + 1]->getPosition().x) - (m_singleMapsVec[j + 1]->getContentSize().width / 2) - (m_singleMapsVec[j]->getContentSize().width * m_singleMapsVec[j + 1]->getScaleX() / 2), 
            m_pTableViewLayer->getContentSize().height / 2));
    }
    for(int k = mapindex; k < 10; k++)
    {
        m_singleMapsVec[k]->setScale(global_scale);
        m_singleMapsVec[k]->setPosition(ccp((m_singleMapsVec[k - 1]->getPosition().x) + (m_singleMapsVec[k - 1]->getContentSize().width / 2) + (m_singleMapsVec[k]->getContentSize().width * m_singleMapsVec[k - 1]->getScaleX() / 2), 
            m_pTableViewLayer->getContentSize().height / 2));
    }

}

void TreasureExploreLayer::SetAlreadyGetIndex(int alreadyIndex, int toIndex)
{
    for(int i = 0; i < alreadyIndex; i++)
    {
        m_singleVec[i]->showCanGetEffect(false);
        m_singleVec[i]->showGetFlag(true);
        m_singleVec[i]->setNameGreen();
    }
    for(int j = alreadyIndex; j < toIndex - 1; j++)
    {
        m_singleVec[j]->showCanGetEffect(true);
        m_singleVec[j]->showGetFlag(false);
        m_singleVec[j]->setNameGreen();
    }
    for(int k = toIndex - 1; k < m_singleVec.size(); k++)
    {
        m_singleVec[k]->showCanGetEffect(false);
        m_singleVec[k]->showGetFlag(false);
        m_singleVec[k]->setNameGray();
    }
    for(int m = 0; m < toIndex - 1; m++)
    {
        m_singleMapsVec[m]->getBackSprite()->setVisible(true);
    }
}

SingleTreasureLayer* TreasureExploreLayer::createSingleTreasureLayer()
{
    CCNodeLoader* pLoader = new SingleTreasureLayerLoader();
    CCNode* pNode = CCBLoaderManager::getInstance()->LoadFromCCBI(pLoader, "SingleTreasure", "SingleTreasure");
    SingleTreasureLayer* pLayer = dynamic_cast<SingleTreasureLayer*>(pNode);
    return pLayer;
}

SingleTreasureMapLayer* TreasureExploreLayer::createSingleTreasureMapLayer()
{
    CCNodeLoader* pLoader = new SingleTreasureMapLayerLoader();
    CCNode* pNode = CCBLoaderManager::getInstance()->LoadFromCCBI(pLoader, "SingleTreasureMap", "SingleTreasureMap");
    SingleTreasureMapLayer* pLayer = dynamic_cast<SingleTreasureMapLayer*>(pNode);
    return pLayer;
}

// tableview source
CCSize TreasureExploreLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(this->m_pTableViewLayer->getContentSize().width, this->m_pTableViewLayer->getContentSize().height);
}
CCTableViewCell* TreasureExploreLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->dequeueCell();
    if(cell)
    {
        cell->removeAllChildrenWithCleanup(true);
    }
    else
    {
        cell = new CCTableViewCell();
        cell->autorelease();
    }

    this->PutMapSpriteToTableViewCell(cell);

    return cell;
}
unsigned int TreasureExploreLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 1;
}
void TreasureExploreLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void TreasureExploreLayer::closeLayerCallBack(void)
{
	CallNativeFuncManager::getInstance()->ShowTreasureExploreLayer(false,false);
}

/////////////////////////////////single treasure//////////////////////////////////////////
SingleTreasureLayer::SingleTreasureLayer()
    :  m_pItemLayer(NULL),  
       m_pItemBtn(NULL),    
       m_pItemName(NULL),   
       m_pEffectLayer(NULL),
       m_pGetFlag(NULL),
       m_pItemCount(NULL),
       m_pUpgradeEffectLayer(NULL),
       m_curItemId(0)
{
    
}
SingleTreasureLayer::~SingleTreasureLayer()
{
   setItemLayer(NULL);
   setItemBtn(NULL);
   setItemName(NULL);
   setEffectLayer(NULL);
   setGetFlag(NULL);
   setItemCount(NULL);
   setUpgradeEffectLayer(NULL);
}

SEL_MenuHandler SingleTreasureLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
SEL_CCControlHandler SingleTreasureLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "item_click", SingleTreasureLayer::OnItemClicked);
    return NULL;
}
bool SingleTreasureLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemLayer",    CCLayer*,          m_pItemLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBtn",      CCControlButton*,  m_pItemBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName",     CCLabelTTF*,       m_pItemName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectLayer",  CCLayer*,          m_pEffectLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGetFlag",      CCSprite*,         m_pGetFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCount",    CCLabelTTF*,       m_pItemCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpgradeEffectLayer",    CCLayer*,       m_pUpgradeEffectLayer);

    return true;
}
void SingleTreasureLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    m_pItemBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pItemName->setColor(SYS_FONT_WHITE);
}
void SingleTreasureLayer::InitLayer(int idx, int itemId, int itemCount)
{
    m_curItemId = itemId;
    CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(itemId);
    m_pItemLayer->removeChildByTag(66);
    iconSprite->setAnchorPoint(ccp(0.5, 0.5));
    iconSprite->setPosition(ccp(m_pItemLayer->getContentSize().width / 2, m_pItemLayer->getContentSize().height / 2));
    m_pItemLayer->addChild(iconSprite, 66, 66);

    // count
    m_pItemCount->setString(MainMenuLayer::GetNumByFormat(itemCount));
    if(itemCount > 1)
    {
        m_pItemCount->setVisible(true);
    }
    else
    {
        m_pItemCount->setVisible(false);
    }

	// name 
	if(idx > 0)
	{
		m_pItemName->setVisible(true);
		CCString* strName = CCString::createWithFormat(Localizatioin::getLocalization("M_TREASURE_NAME"), idx);
		m_pItemName->setString(strName->getCString());
	}
	else
	{
		m_pItemName->setVisible(false);
	}
}
void SingleTreasureLayer::showGetFlag(bool flag)
{
    if(flag == true)
    {
        m_pGetFlag->setVisible(true);
    }
    else
    {
        m_pGetFlag->setVisible(false);
    }
}
void SingleTreasureLayer::showCanGetEffect(bool flag)
{
     m_pEffectLayer->removeAllChildren();

     if(flag == true)
     {
         // add effect
         EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(445,"",false);
         effect1->setPosition(ccp(m_pEffectLayer->getContentSize().width * 0.5f, m_pEffectLayer->getContentSize().height * 0.5f));
         effect1->SetAnim(kType_Play,1,true);
         m_pEffectLayer->addChild(effect1);
     }    
}

void SingleTreasureLayer::showUpgradeEffect()
{
    m_pUpgradeEffectLayer->removeAllChildren();

    // add effect
    EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(442,"",false);
    effect1->setPosition(ccp(m_pUpgradeEffectLayer->getContentSize().width * 0.5f, m_pUpgradeEffectLayer->getContentSize().height * 0.5f));
    effect1->SetAnim(kType_Play,1,false);
    m_pUpgradeEffectLayer->addChild(effect1);
}
void SingleTreasureLayer::setNameGreen()
{
    m_pItemName->setColor(SYS_FONT_GREEN);
}

void SingleTreasureLayer::setNameGray()
{
    m_pItemName->setColor(SYS_FONT_WHITE);
}

void SingleTreasureLayer::onGetWayCallBack(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyGetwayLayer",m_curItemId);
}

void SingleTreasureLayer::OnItemClicked(CCObject* pSender, CCControlEvent event)
{
    if(m_curItemId == 0)
    {
        return;
    }
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(m_curItemId, pt);

	/*ItemBase * info = ItemManager::Get()->getItemInfoById(m_curItemId);
	EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleTreasureLayer::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/

    TutorialsManager::Get()->HandleOneEvent("item_click", 1);
}


/////////////////////////////////single treasure map///////////////////////////////////////////////////
SingleTreasureMapLayer::SingleTreasureMapLayer()
    : m_pMapSprite(NULL)
    , m_pBackSprite(NULL)
{

}
SingleTreasureMapLayer::~SingleTreasureMapLayer()
{
    setMapSprite(NULL);
    setBackSprite(NULL);
}

SEL_MenuHandler SingleTreasureMapLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
SEL_CCControlHandler SingleTreasureMapLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool SingleTreasureMapLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMapSprite",  CCSprite*, m_pMapSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackSprite", CCSprite*, m_pBackSprite);
    return true;
}
void SingleTreasureMapLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{

}
void SingleTreasureMapLayer::InitLayer()
{

}


TreasureExploreUILayer::TreasureExploreUILayer(CCNode* parent) :
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

TreasureExploreUILayer::~TreasureExploreUILayer()
{

}

CCNode* TreasureExploreUILayer::getTutorialNode(std::string name)
{
    if (strcmp("explore", name.c_str()) == 0)
    {
        return m_infoLayer->getExploreBtn();
    } else if (strcmp("item_click", name.c_str()) == 0)
    {
        return m_infoLayer->m_singleVec[0]->getItemBtn();
    }
    else
    {
        return NULL;
    }
}