#include "OnlineNetworkManager.h"
#include "RankListLayer.h"
#include "CCBLoaderManager.h"
#include "CallNativeFuncManager.h"
#include "ButtonPerformanceManager.h"
#include "Localization.h"
#include "GlobalFunction.h"
#include "InterfaceManager.h"
#include "MainMenuLayer.h"
#include "BackPackLayer.h"
#include "LuaTinkerManager.h"
#include "CCLuaEngine.h"
#include "MathUtil.h"

RankListLayer::RankListLayer():
	  m_pLevelButton(NULL),    
	  m_pCombatButton(NULL),   
	  m_pArenaButton(NULL),   
	  m_pReputationButton(NULL),
	  m_pPartyButton(NULL),
      m_pCloseButton(NULL),
	  m_pTitleLabel(NULL),
	  m_pRankLabel(NULL),
	  m_pRiseLabel(NULL),
	  m_pPlayerLabel(NULL),
	  m_pPartyLabel(NULL),
	  m_pReputationLabel(NULL),
	  m_pTableViewLayer(NULL),
      m_pTableView(NULL),
      m_pRightLayer(NULL),
      m_pLookLabel(NULL),
      m_pCurRankLabel(NULL),
      m_pCurRankNum(NULL)
{
    m_pRankName = RANK_NAME_LV_SCORE;
}

RankListLayer::~RankListLayer()
{
	setLevelButton(NULL);
	setCombatButton(NULL);
	setArenaButton(NULL);
	setReputationButton(NULL);
	setPartyButton(NULL);
    setCloseButton(NULL);

	setTitleLabel(NULL);
	setRankLabel(NULL);
	setRiseLabel(NULL);
	setPlayerLabel(NULL);
	setPartyLabel(NULL);
	setReputationLabel(NULL);

	setTableViewLayer(NULL);
    setTableView(NULL);
    setRightLayer(NULL);
    setLookLabel(NULL);
    setCurRankLabel(NULL);
    setCurRankNum(NULL);

    ButtonPerformanceManager::Get()->removePerformanceButton(this);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, Msg_Refresh_RankList);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "lookplayer");
}

SEL_MenuHandler RankListLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}
 
SEL_CCControlHandler RankListLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",             RankListLayer::CloseLayer);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_level",      RankListLayer::onLabelButtonClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_combat",     RankListLayer::onLabelButtonClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_spriteCombat",     RankListLayer::onLabelButtonClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_reputation", RankListLayer::onLabelButtonClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_party",      RankListLayer::onLabelButtonClicked);
	return NULL;
}

bool RankListLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "level_btn",      CCControlButton*, this->m_pLevelButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "combat_btn",     CCControlButton*, this->m_pCombatButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spriteCombat_btn",      CCControlButton*, this->m_pArenaButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "reputation_btn", CCControlButton*, this->m_pReputationButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "party_btn",	    CCControlButton*, this->m_pPartyButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",	    CCControlButton*, this->m_pCloseButton);


	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleLabel",	       CCLabelTTF*, this->m_pTitleLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankLabel",	       CCLabelTTF*, this->m_pRankLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRiseLabel",	       CCLabelTTF*, this->m_pRiseLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPlayerLabel",	       CCLabelTTF*, this->m_pPlayerLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPartyLabel",	       CCLabelTTF*, this->m_pPartyLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReputationLabel",	   CCLabelTTF*, this->m_pReputationLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLookLabel",	       CCLabelTTF*, this->m_pLookLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurRankLabel",	   CCLabelTTF*, this->m_pCurRankLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurRankNum",	       CCLabelTTF*, this->m_pCurRankNum);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rank_member",           CCLayer*,  this->m_pTableViewLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRightLayer",         CCLayer*,  this->m_pRightLayer);

	return true;
}

void RankListLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
	this->setTouchEnabled(true);

    // button perfomance
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_pLevelButton,      0);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_pCombatButton,     1);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_pArenaButton,      2);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_pReputationButton, 3);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_pPartyButton,      4);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);

    // set touch priority
    m_pCloseButton->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pLevelButton->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pCombatButton->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pArenaButton->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pReputationButton->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pPartyButton->setTouchPriority(kCCLayerMenuTouchPriority);

    // register observer
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RankListLayer::OnRefreshRankList), Msg_Refresh_RankList, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RankListLayer::OnNoticeLookClicked), "lookplayer", NULL);

    // send msg
    int uid     = UserData::getUserId().getID();
    int regTime = UserData::getUserId().getRegTime();
    int channel = UserData::getUserId().getChannel();
    RankManager::getInstance()->setRankName(m_pRankName);
    OnlineNetworkManager::sShareInstance()->SendGetPlayerRankNum(m_pRankName, uid, regTime, channel);
    
    this->initMutilanguage();

    m_pReputationLabel->setString(Localizatioin::getLocalization("M_RANK_LEVEL"));
    m_pCurRankNum->setString("--");
    RankManager::getInstance()->setRankIndex(-1);
}

bool RankListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void RankListLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
}

void RankListLayer::onEnter()
{
	CCLayer::onEnter();
}

void RankListLayer::onExit()
{
	CCLayer::onExit();
    this->stopAllActions();
    this->unscheduleAllSelectors();
}

SingleRankListLayer* RankListLayer::createSingleRankListLayer()
{
    CCNodeLoader* pLoader = new SingleRankListLayerLoader();
    CCNode* pNode = CCBLoaderManager::getInstance()->LoadFromCCBI(pLoader, "single_rank", "single_rank");
    SingleRankListLayer* pLayer = dynamic_cast<SingleRankListLayer*>(pNode);
    return pLayer;
}

CCSize RankListLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(this->m_pTableViewLayer->getContentSize().width, this->m_pTableViewLayer->getContentSize().height / 5);
}
CCTableViewCell* RankListLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->dequeueCell();
    if(cell)
    {
        this->refreshCell(cell, idx);
    }
    else
    {
        cell = new CCTableViewCell();
        cell->autorelease();

        SingleRankListLayer* pLayer = this->createSingleRankListLayer();
        pLayer->setTag(111);
        cell->addChild(pLayer);
        this->refreshCell(cell, idx);
    }

    return cell;
}

void RankListLayer::refreshCell(CCTableViewCell* cell, int idx)
{
    if(!cell)
    {
        return;
    }
    SingleRankListLayer* pRankLayer = dynamic_cast<SingleRankListLayer*>(cell->getChildByTag(111));
    if(!pRankLayer)
    {
        return;
    }

    std::vector<RankInfoX> vec = RankManager::getInstance()->GetRankInfo();
    if(idx >= vec.size())
    {
        return;
    }
    pRankLayer->InitLayer(vec[idx], idx);
}

unsigned int RankListLayer::numberOfCellsInTableView(CCTableView *table)
{
    int size = RankManager::getInstance()->GetRankInfo().size();
    return size;
}

void RankListLayer::HighLightSelect(int index)
{
    int idx = index;
    RankManager::getInstance()->setRankIndex(idx);

    // high light
    for(int i = 0; i < this->numberOfCellsInTableView(m_pTableView); i++)
    {
        CCTableViewCell* cells = m_pTableView->cellAtIndex(i);
        if(cells == NULL)
        {
            continue;
        }
        SingleRankListLayer* layer = dynamic_cast<SingleRankListLayer*>(cells->getChildByTag(111));
        if(layer == NULL)
        {
            continue;
        }
        if(i == idx)
        {
            layer->getHighLightLayer()->setVisible(true);
        }
        else
        {
            layer->getHighLightLayer()->setVisible(false);
        }
    }
}

void RankListLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    int idx = cell->getIdx();
    HighLightSelect(idx);
}

void RankListLayer::OnNoticeLookClicked(CCObject* obj)
{
     int idx =  RankManager::getInstance()->getRankIndex();

    // show index info
    //this->RefreshAttri();
    this->ShowIndexPlayerInfo(idx);

    HighLightSelect(idx);
}

void RankListLayer::ShowIndexPlayerInfo(int idx)
{
    std::vector<RankInfoX> vec = RankManager::getInstance()->GetRankInfo();
    if(idx >= vec.size())
    {
        return;
    }
    RankInfoX info = vec[idx];
    //LuaTinkerManager::Get()->callLuaFunc<void>("Script/Friend/OtherPlayerEquipLayer.lua", "setPlayerIDInfo", info.userid, info.reg_time, info.channel_id);


    // open selector menu
    int openMenu =  LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getChatPlayerOpen");
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerId",info.userid);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setRegtime",info.reg_time);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setChannel",info.channel_id);

    // 精灵的所属玩家里面还包含了精灵等级信息，所有要解析出来
    std::string nameStr = "";
    if (RankManager::getInstance()->getRankName() == RANK_NAME_FAIRY_SCORE)
    {
        cs_fairy_info *fairyInfo = new cs_fairy_info;
        fairyInfo->ParseFromString(info.name);
        if (fairyInfo && fairyInfo->fairy_id() > 0)
        {
            nameStr = fairyInfo->username().c_str();
        }
        else
        {
            nameStr = "--";
        }
        delete fairyInfo;
    }
    else
    {
        nameStr = info.name.c_str();
    }

    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerName",nameStr.c_str());
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setInitPlayer");
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setModelLess", true);
}

void RankListLayer::OnRefreshRankList(CCObject* obj)
{
    m_pCurRankNum->setString(IntToStr(RankManager::getInstance()->getPlayerRank()));
    this->ConfigUI();
}

// layer ui logical
void RankListLayer::ConfigUI()
{
    if(this->m_pTableView)
    {
        this->m_pTableView->removeFromParentAndCleanup(true);
        setTableView(NULL);
    }

    float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
    CCSize size = CCSizeMake(this->m_pTableViewLayer->getContentSize().width, this->m_pTableViewLayer->getContentSize().height);
    CCTableView* tableview = CCTableView::create(this, size);
    tableview->setDirection(kCCScrollViewDirectionVertical);
    tableview->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableview->setDelegate(this);
    setTableView(tableview);
    this->m_pTableViewLayer->addChild(m_pTableView);
    m_pTableView->setTouchPriority(kCCLayerMenuTouchPriority);
    this->m_pTableView->reloadData();
}

// process mutilanguage
void RankListLayer::initMutilanguage()
{
    // labels 
	m_pTitleLabel->setString(Localizatioin::getLocalization("M_RANKING"));
	m_pRankLabel->setString(Localizatioin::getLocalization("M_PVAI_RANK2"));
	m_pRiseLabel->setString(Localizatioin::getLocalization("M_LIFT"));
	m_pPlayerLabel->setString(Localizatioin::getLocalization("M_PLAYER"));
	m_pPartyLabel->setString(Localizatioin::getLocalization("M_GUILD"));
	m_pReputationLabel->setString(Localizatioin::getLocalization("Name_411303"));
    m_pLookLabel->setString(Localizatioin::getLocalization("M_FIND"));
	m_pCurRankLabel->setString(Localizatioin::getLocalization("M_LATEST_RANK"));

    // buttons
    m_pLevelButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_RANK_LEVEL")), CCControlStateNormal);
    m_pCombatButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_COMBAT")), CCControlStateNormal);
    m_pArenaButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_FAIRY_FIGHT")), CCControlStateNormal);
    m_pReputationButton->setTitleForState(CCString::create(Localizatioin::getLocalization("Name_411303")), CCControlStateNormal);
}

void RankListLayer::RefreshAttri()
{ 
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Friend/OtherPlayerEquipLayer.lua", "callCreateFunc");
}

void RankListLayer::CloseLayer(CCObject* pSender, CCControlEvent pEvent)
{
    CallNativeFuncManager::getInstance()->RemoveRankListLayer(); 
}

void RankListLayer::onLabelButtonClicked(CCObject* pSender, CCControlEvent pEvent)
{
	m_pCurRankLabel->setString(Localizatioin::getLocalization("M_LATEST_RANK"));
	m_pPlayerLabel->setString(Localizatioin::getLocalization("M_PLAYER"));
	m_pPartyLabel->setString(Localizatioin::getLocalization("M_GUILD"));
    // button tag
    RankManager::getInstance()->setRankIndex(-1);
    int tag = dynamic_cast<CCControlButton*>(pSender)->getTag();
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, tag);
    switch(tag)
    {
    case 0:
        m_pRankName = RANK_NAME_LV_SCORE;
        m_pReputationLabel->setString(Localizatioin::getLocalization("M_RANK_LEVEL"));
        break;

    case 1:
        m_pRankName = RANK_NAME_POWER_SCORE;
        m_pReputationLabel->setString(Localizatioin::getLocalization("M_COMBAT"));
        break;

    case 2:
		m_pRankName = RANK_NAME_FAIRY_SCORE;
		m_pPlayerLabel->setString(Localizatioin::getLocalization("ELF_NAME"));
		m_pPartyLabel->setString(Localizatioin::getLocalization("M_USER"));
		m_pReputationLabel->setString(Localizatioin::getLocalization("M_FAIRY_FIGHT"));
		m_pCurRankLabel->setString(Localizatioin::getLocalization("M_FAIRY_FINAL_RANK"));
        break;

    case 3:
        m_pRankName = RANK_NAME_REPUTATION_SCORE;
        m_pReputationLabel->setString(Localizatioin::getLocalization("Name_411303"));
        break;

    case 4:
        break;

    default:
        break;
    }
    
    // user info
    int uid     = UserData::getUserId().getID();
    int regTime = UserData::getUserId().getRegTime();
    int channel = UserData::getUserId().getChannel();
    RankManager::getInstance()->setRankName(m_pRankName);
    OnlineNetworkManager::sShareInstance()->SendGetPlayerRankNum(m_pRankName, uid, regTime, channel);
}

// the down is SingleRankListLayer
#pragma mark -- SingleRankListLayer

SingleRankListLayer::SingleRankListLayer()
    : m_pRankSprite(NULL), 
      m_pRank(NULL),       
      m_pRankState(NULL),  
      m_pRankStateNo(NULL),
      m_pName(NULL),       
      m_pPartyName(NULL),  
      m_pReputation(NULL),
      m_pVip(NULL),
      m_pHighLightLayer(NULL),
      m_pLookBtn(NULL),
      m_pCircle(NULL),
	  m_pRect(NULL),
      m_pCurPos(NULL)
{
    
}
SingleRankListLayer::~SingleRankListLayer()
{
    setRankSprite(NULL);
    setRank(NULL);
    setRankState(NULL);
    setRankStateNo(NULL);
    setName(NULL);
    setPartyName(NULL);
    setReputation(NULL);
    setVip(NULL);
    setHighLightLayer(NULL);
    setLookBtn(NULL);
    setCircle(NULL);
    setCurPos(NULL);
}

SEL_MenuHandler SingleRankListLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler SingleRankListLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "look", SingleRankListLayer::OnLookButtonClicked);
	return NULL;
}
bool SingleRankListLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rank_sprite",                  CCSprite*,      this->m_pRankSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rank",                         CCLabelTTF*,    this->m_pRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rank_state",                   CCSprite*,      this->m_pRankState);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rank_state_no" ,               CCLabelTTF*,    this->m_pRankStateNo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",                         CCLabelTTF*,    this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "party_name",                   CCLabelTTF*,    this->m_pPartyName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "reputation",                   CCLabelTTF*,    this->m_pReputation);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip",                          CCLabelTTF*,    this->m_pVip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLightLayer",            CCLayer*,       this->m_pHighLightLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLookBtn",                   CCControlButton*, this->m_pLookBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurPos",                    CCSprite*,      this->m_pCurPos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCircle",                    CCSprite*,      this->m_pCircle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRect",                      CCSprite*,      this->m_pRect);

	return true;
}

void SingleRankListLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    this->InitLayerData();
    RankManager::getInstance()->setInitPos(this->m_pName->getPosition());
	RankManager::getInstance()->setPartyInitPos(this->m_pPartyName->getPosition());
    m_pLookBtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

void SingleRankListLayer::InitLayerData()
{
    this->m_pRank->setString("--");
    this->m_pRankStateNo->setString("--");
    this->m_pName->setString("--");
    this->m_pPartyName->setString("--");
    this->m_pReputation->setString("--");
}

void SingleRankListLayer::InitLayer(RankInfoX info, int index)
{
    // hight light
    if(index == RankManager::getInstance()->getRankIndex())
    {
        m_pHighLightLayer->setVisible(true);
    }
    else
    {
        m_pHighLightLayer->setVisible(false);
    }

    m_pLookBtn->setTag(index);

    // rank
    this->m_pRankSprite->setVisible(true);
    m_pRank->setVisible(false);
    if(info.rank == 1)
    {
        CCSpriteFrame* frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_first.png");
        this->m_pRankSprite->setDisplayFrame(frame);
        this->m_pName->setColor(SYS_FONT_ORANGE);
        this->m_pPartyName->setColor(SYS_FONT_ORANGE);
        this->m_pReputation->setColor(SYS_FONT_ORANGE);
    }
    else if(info.rank == 2)
    {
        CCSpriteFrame* frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_2.png");
        this->m_pRankSprite->setDisplayFrame(frame);
        this->m_pName->setColor(SYS_FONT_PURPLE);
        this->m_pPartyName->setColor(SYS_FONT_PURPLE);
        this->m_pReputation->setColor(SYS_FONT_PURPLE);
    }
    else if(info.rank == 3)
    {
        CCSpriteFrame* frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_third.png");
        this->m_pRankSprite->setDisplayFrame(frame);
        this->m_pName->setColor(SYS_FONT_BLUE);
        this->m_pPartyName->setColor(SYS_FONT_BLUE);
        this->m_pReputation->setColor(SYS_FONT_BLUE);
    }
    else
    {
        this->m_pRankSprite->setVisible(false);
        m_pRank->setVisible(true);
        this->m_pName->setColor(SYS_FONT_WHITE);
        this->m_pPartyName->setColor(SYS_FONT_WHITE);
        this->m_pReputation->setColor(SYS_FONT_WHITE);
    }
    if(strcmp(info.guild_name.c_str(), "") == 0)
    {
        m_pPartyName->setString("--");
    }
    else
    {
        m_pPartyName->setString(info.guild_name.c_str());
    }
    
    m_pRank->setString(IntToStr(info.rank));
    if(info.rank == RankManager::getInstance()->getPlayerRank())
    {
        this->m_pName->setColor(SYS_FONT_RED);
        this->m_pPartyName->setColor(SYS_FONT_RED);
        this->m_pReputation->setColor(SYS_FONT_RED);
        m_pLookBtn->setVisible(false);
        m_pCurPos->setVisible(true);
    }
    else
    {
        m_pLookBtn->setVisible(true);
        m_pCurPos->setVisible(false);
    }

    // updown
    int stateNum = info.updown;
    CCSpriteFrame* frame = NULL;
    if(stateNum < 0 )
    {
        frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_down.png");
        m_pRankStateNo->setColor(SYS_FONT_RED);
    }
    else if(stateNum > 0)
    {
        frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_up.png");
        m_pRankStateNo->setColor(SYS_FONT_GREEN);
    }
    else
    {
        frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_normal.png");
        m_pRankStateNo->setColor(SYS_FONT_YELLOW);
    }
    m_pRankState->setDisplayFrame(frame);
    m_pRankStateNo->setString(IntToStr((int)fabs((double)stateNum)));

    // name
	cs_fairy_info *fairyInfo = new cs_fairy_info;
	if (RankManager::getInstance()->getRankName() == RANK_NAME_FAIRY_SCORE)
	{
		fairyInfo->ParseFromString(info.name);
		if (fairyInfo && fairyInfo->fairy_id() > 0)
		{
			std::string fairyName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/fairyCombatRank.lua", "getFairyNameById", fairyInfo->fairy_id());
			if (fairyInfo->fairy_grade() > 0)
			{
				fairyName.append("+");
				fairyName.append(IntToStr(fairyInfo->fairy_grade()));
			}
			m_pName->setString(fairyName.c_str());
			m_pPartyName->setString(fairyInfo->username().c_str());
		}
		else
		{
			m_pName->setString("--");
			m_pPartyName->setString("--");
		}
	}
	else 
	{
		m_pName->setString(info.name.c_str());
	}
    
    // vip
    int vipLv = 0;
    if(RankManager::getInstance()->getRankName() == RANK_NAME_LV_SCORE)
    {
        int tempV = RankManager::getInstance()->Offset32Bit(info.score);
        vipLv = tempV % 100;
    }
    else
    {
        vipLv = info.score % 100;
    }
    std::string vipstring = "V";
    vipstring.append(IntToStr(vipLv));
    m_pVip->setString(vipstring.c_str());
    m_pName->setPosition(RankManager::getInstance()->getInitPos());
    m_pVip->setPosition(RankManager::getInstance()->getInitPos());
	m_pPartyName->setPosition(RankManager::getInstance()->getPartyInitPos());
    if(vipLv == 0)
    {
        m_pVip->setVisible(false);
        m_pName->setPosition(RankManager::getInstance()->getInitPos());
    }
    else
    {
        CCPoint p = RankManager::getInstance()->getInitPos();
        int vipSizeW = m_pVip->getContentSize().width;
        int nameSizeW = m_pName->getContentSize().width;
		int partySizeW = m_pPartyName->getContentSize().width;
		if (RankManager::getInstance()->getRankName() == RANK_NAME_FAIRY_SCORE)
		{
			m_pPartyName->setPosition(ccp(m_pPartyName->getPositionX()+vipSizeW/2 + 2,m_pPartyName->getPositionY()));
			m_pVip->setPosition(ccp(m_pPartyName->getPosition().x - (partySizeW/2) - 2 - vipSizeW/2, m_pVip->getPosition().y));
		}
		else
		{
			m_pVip->setPosition(ccp(m_pVip->getPosition().x - (nameSizeW / 2) - 2, m_pVip->getPosition().y));
			m_pName->setPosition(ccp(m_pName->getPosition().x + (vipSizeW / 2) + 2, m_pName->getPosition().y));
		}
        m_pVip->setVisible(true);
    }

    //m_pPartyName
    if(RankManager::getInstance()->getRankName() == RANK_NAME_LV_SCORE)
    {
        int tempV = RankManager::getInstance()->Offset32Bit(info.score);
        m_pReputation->setString(IntToStr(tempV / 100));
    }
    else
    {
        m_pReputation->setString(IntToStr(info.score / 100));
    }
    // head pic
    if(info.type == 0)
    {
        m_pCircle->setVisible(false);
        return;
    }
    else
    {
        m_pCircle->setVisible(true);
    }
    int carrerType = info.type;
    if(carrerType == 0)
    {
        m_pCircle->getParent()->removeChildByTag(55);
        return;
    }
	if (RankManager::getInstance()->getRankName() == RANK_NAME_FAIRY_SCORE)
	{
		m_pCircle->setVisible(false);
		m_pRect->setVisible(true);

		if (fairyInfo && fairyInfo->fairy_id() > 0)
		{
			const char* fairyIconName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/fairyCombatRank.lua", "getFairyIconById", fairyInfo->fairy_id());
			CCSprite* fairyIcon = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin", fairyIconName);
			if (fairyIcon)
			{
				fairyIcon->setScale(0.8);
				fairyIcon->setPosition(m_pRect->getPosition());
				m_pRect->getParent()->removeChildByTag(55);
				m_pRect->getParent()->addChild(fairyIcon, 55, 55);
			}
		}
	}
	else
	{
		m_pCircle->setVisible(true);
		m_pRect->setVisible(false);

		const char* headIconName =  LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/GameConfig/ItemConfigTab.lua", "getCharacterField", carrerType, "HeadPicture");
		CCSprite* playerIcon = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin",headIconName);
		playerIcon->setScale(0.5);
		playerIcon->setPosition(m_pCircle->getPosition());
		m_pCircle->getParent()->removeChildByTag(55);
		m_pCircle->getParent()->addChild(playerIcon, 55, 55);
	}
	delete fairyInfo;
}


void SingleRankListLayer::OnLookButtonClicked(CCObject* pSender, CCControlEvent event)
{
    RankManager::getInstance()->setRankIndex((dynamic_cast<CCControlButton*>(pSender))->getTag());
    CCNotificationCenter::sharedNotificationCenter()->postNotification("lookplayer");
}