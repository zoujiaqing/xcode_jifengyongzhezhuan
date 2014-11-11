#include "LuaTinkerManager.h"
#include "GuildMemberLayer.h"
#include "UIManager.h"
#include "MainMenuLayer.h"
#include "GuildManager.h"
#include "InterfaceManager.h"
#include "GMessage.h"
#include "ButtonPerformanceManager.h"
#include "Localization.h"
#include "GameAudioManager.h"

GuildMemberLayer::GuildMemberLayer() :
    m_roleButton(NULL),
    m_levelButton(NULL),
    m_powerButton(NULL),
    m_contributeButton(NULL),
    m_jobButton(NULL),
    m_statusButton(NULL),
    m_quitButton(NULL),
    m_settingButton(NULL),
    m_kickButton(NULL),
    m_contentLayer(NULL),
    m_memberView(NULL),
    m_closeButton(NULL),
    m_jiesanButton(NULL),
    m_clickIndex(0),
    m_curType(3)
{
    m_guildMembers.clear();
}

GuildMemberLayer::~GuildMemberLayer()
{
    setRoleButton(NULL);
    setLevelButton(NULL);
    setPowerButton(NULL);
    setContributeButton(NULL);
    setJobButton(NULL);
    setStatusButton(NULL);
    setQuitButton(NULL);
    setSettingButton(NULL);
    setKickButton(NULL);
    setContentLayer(NULL);
    setMemberView(NULL);
    setCloseButton(NULL);
    setjiesanButton(NULL);

    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
    ButtonPerformanceManager::Get()->removePerformanceButton(this);
}

SEL_MenuHandler GuildMemberLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildMemberLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",   GuildMemberLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "quit",    GuildMemberLayer::onQuitLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "drop",    GuildMemberLayer::onJieSanButtonClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "kick",    GuildMemberLayer::onKickLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "setting", GuildMemberLayer::onAppointLayer);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "level",        GuildMemberLayer::onLevelButtonClickded);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "power",        GuildMemberLayer::onPowerButtonClickded);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "contribute",   GuildMemberLayer::onContributeButtonClickded);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "job",          GuildMemberLayer::onJobButtonClickded);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "status",       GuildMemberLayer::onStatusButtonClickded);
    return NULL;
}

bool GuildMemberLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "role_button",        CCControlButton*, m_roleButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "level_button",       CCControlButton*, m_levelButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "power_button",       CCControlButton*, m_powerButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute_button",  CCControlButton*, m_contributeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "job_button",         CCControlButton*, m_jobButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "status_button",      CCControlButton*, m_statusButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "quit_button",        CCControlButton*, m_quitButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pjiesanButton",     CCControlButton*, m_jiesanButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "setting_button",     CCControlButton*, m_settingButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "kick_button",        CCControlButton*, m_kickButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",          CCControlButton*, m_closeButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "content_layer",      CCLayer*,         m_contentLayer);
    return false;
}

void GuildMemberLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    m_curType = 3;
    GuildManager::Get()->SendGetGuildMember();
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    // 首先获取玩家的公会信息
    CSQueryPlayerGuildInfoRsp info = GuildManager::Get()->getPlayerOwnerGuildInfo();
    
    if (info.playerinfo().status() == (int) GUILD_TO_OWNER)
    {
        m_quitButton->setVisible(false);
        m_jiesanButton->setVisible(true);
    }
    else
    {
        m_jiesanButton->setVisible(false);
    }

    if (info.playerinfo().status() == (int) GUILD_TO_MEMBER ||
        info.playerinfo().status() == (int) GUILD_TO_ELITE)
    {
        m_kickButton->setVisible(false);
        m_settingButton->setVisible(false);
    }

    // button perfomance
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_levelButton,      0);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_powerButton,     1);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_contributeButton,      2);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_jobButton, 3);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_statusButton,      4);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 3);

    GuildManager::Get()->setIsKickMemeber(false);
}

void GuildMemberLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_UPDATE_GUILD_MEMBER :
        {
            UpdateMemberInfo();
            break;
        }

    default:
        break;
    }
}

void GuildMemberLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildMemberLayer(false);
        mLayer->ShowGuildInfoLayer(true);
    }
}

void GuildMemberLayer::onQuitLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    GuildManager::Get()->setMemberOpenType(TAG_QUIT);
    GuildManager::Get()->setStr2(UserData::GetUserInfo().guildName);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildNoticeLayer(true);
    }
}

void GuildMemberLayer::onJieSanButtonClicked(CCObject* sender, CCControlEvent event)
{
    GuildManager::Get()->setMemberOpenType(TAG_JIESAN);
    GuildManager::Get()->setStr2(UserData::GetUserInfo().guildName);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildNoticeLayer(true);
    }
}

void GuildMemberLayer::onKickLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    GuildManager::Get()->setMemberOpenType(TAG_KICK);
    if (m_clickIndex == -1 || m_clickIndex >= m_guildMembers.size())
    {
        return;
    }

    GuildMemberInfo info = m_guildMembers[m_clickIndex];
    std::vector<StrGuildStatus> vtStatus;
    vtStatus.clear();
    StrGuildStatus guildStatus;
    guildStatus.user_id = info.userid;
    guildStatus.reg_tm = info.reg_tm;
    guildStatus.channel_id = info.channel_id;
    guildStatus.status = (int) GUILD_TO_NOT_MEMBER;
    vtStatus.push_back(guildStatus);
    GuildManager::Get()->setMemberStatus(vtStatus);
    GuildManager::Get()->setStr2(info.name);

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildNoticeLayer(true);
    }
}

void GuildMemberLayer::onAppointLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    if (m_clickIndex == -1 || m_clickIndex >= m_guildMembers.size())
    {
        return;
    }

    GuildMemberInfo info = m_guildMembers[m_clickIndex];
    if(info.userid == UserData::getUserId().getID() && info.reg_tm == UserData::getUserId().getRegTime())
    {
        ShowMessage(Localizatioin::getLocalization("G_MEMBERS_ERROR1"));
        return;
    }

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        GuildManager::Get()->setStr2(info.name);
        mLayer->ShowGuildAppointLayer(true);
        GuildAppointLayer* layer = dynamic_cast<GuildAppointLayer*>(mLayer->getGuildAppointLayer());
        if (layer != NULL)
        {
            layer->initLayer(info);
        }
    }
}

void GuildMemberLayer::onLevelButtonClickded(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);
    m_curType = 0;
    m_guildMembers.clear();
    std::vector<GuildMemberInfo> memberVec = GuildManager::Get()->getMemberList();
    std::sort(memberVec.begin(), memberVec.end(), GuildManager::LvBigger());

    for(int i = 0; i < memberVec.size(); i++)
    {
        m_guildMembers.push_back(memberVec[i]);
    }

    AddTableView();
}
void GuildMemberLayer::onPowerButtonClickded(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 1);
    m_curType = 1;
    m_guildMembers.clear();
    std::vector<GuildMemberInfo> memberVec = GuildManager::Get()->getMemberList();
    std::sort(memberVec.begin(), memberVec.end(), GuildManager::PowerBigger());

    for(int i = 0; i < memberVec.size(); i++)
    {
        m_guildMembers.push_back(memberVec[i]);
    }

    AddTableView();
}
void GuildMemberLayer::onContributeButtonClickded(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 2);
    m_curType = 2;
    m_guildMembers.clear();
    std::vector<GuildMemberInfo> memberVec = GuildManager::Get()->getMemberList();
    std::sort(memberVec.begin(), memberVec.end(), GuildManager::ContributeBigger());

    for(int i = 0; i < memberVec.size(); i++)
    {
        m_guildMembers.push_back(memberVec[i]);
    }

    AddTableView();
}
void GuildMemberLayer::onJobButtonClickded(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 3);
    m_curType = 3;
    m_guildMembers.clear();
    std::vector<GuildMemberInfo> memberVec = GuildManager::Get()->getMemberList();
    std::sort(memberVec.begin(), memberVec.end(), GuildManager::JobBigger());

    for(int i = 0; i < memberVec.size(); i++)
    {
        m_guildMembers.push_back(memberVec[i]);
    }

    AddTableView();
}
void GuildMemberLayer::onStatusButtonClickded(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 4);
    m_curType = 3;
    m_guildMembers.clear();
    std::vector<GuildMemberInfo> memberVec = GuildManager::Get()->getMemberList();
    std::sort(memberVec.begin(), memberVec.end(), GuildManager::StatusBigger());

    for(int i = 0; i < memberVec.size(); i++)
    {
        m_guildMembers.push_back(memberVec[i]);
    }

    AddTableView();
}

bool GuildMemberLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildMemberLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void GuildMemberLayer::onEnter()
{
    CCLayer::onEnter();

    m_roleButton->setTouchPriority(kCCScrollMenuPriority);
    m_levelButton->setTouchPriority(kCCScrollMenuPriority);
    m_powerButton->setTouchPriority(kCCScrollMenuPriority);
    m_contributeButton->setTouchPriority(kCCScrollMenuPriority);
    m_jobButton->setTouchPriority(kCCScrollMenuPriority);
    m_statusButton->setTouchPriority(kCCScrollMenuPriority);

    m_closeButton->setTouchPriority(kCCScrollMenuPriority);
    m_quitButton->setTouchPriority(kCCScrollMenuPriority);
    m_settingButton->setTouchPriority(kCCScrollMenuPriority);
    m_kickButton->setTouchPriority(kCCScrollMenuPriority);
    m_jiesanButton->setTouchPriority(kCCScrollMenuPriority);
}

CCSize GuildMemberLayer::cellSizeForTable(CCTableView *table)
{
    return CCSize(1040, 64);
}

CCTableViewCell* GuildMemberLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->dequeueCell();
    if(cell)
    {
        RefreshCell(cell, idx);
    }
    else
    {
        cell = new CCTableViewCell();
        cell->autorelease();
        RefreshCell(cell, idx);
    }

    return cell;
}

unsigned int GuildMemberLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_guildMembers.size();
}

void GuildMemberLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    for (unsigned int index = 0; index < numberOfCellsInTableView(table); ++index)
    {
        CCTableViewCell* pCell = table->cellAtIndex(index);
        if (pCell == NULL)
        {
            continue;
        }

        SingleGuildMemberLayer* layer = dynamic_cast<SingleGuildMemberLayer*>(pCell->getChildByTag(111));
        if (layer == NULL)
        {
            continue;
        }

        if (pCell == cell)
        {
            layer->getLightSprite()->setVisible(true);
            m_clickIndex = index;
        } else
        {
            layer->getLightSprite()->setVisible(false);
        }
    }
}

void GuildMemberLayer::UpdateMemberInfo()
{
    switch (m_curType)
    {
    case 0:
        onLevelButtonClickded(NULL, NULL);   
        break;
    case 1:
        onPowerButtonClickded(NULL, NULL);   
        break;
    case 2:
        onContributeButtonClickded(NULL, NULL);   
        break;
    case 3:
        onJobButtonClickded(NULL, NULL);    
        break;
    case 4:
        onStatusButtonClickded(NULL, NULL);   
        break;
    default:
        break;
    }
    
}

void GuildMemberLayer::AddTableView()
{
    bool needSetOffset = false;
    CCPoint offset = CCPointZero;
    if(this->m_memberView)
    {
        offset.x = m_memberView->getContentOffset().x;
        offset.y = m_memberView->getContentOffset().y;
        needSetOffset = true;
        this->m_memberView->removeFromParentAndCleanup(true);
        setMemberView(NULL);
    }

    setMemberView(CCTableView::create(this, m_contentLayer->getContentSize()));
    m_memberView->setDirection(kCCScrollViewDirectionVertical);
    m_memberView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_contentLayer->addChild(m_memberView);
    m_memberView->setTouchPriority(kCCScrollMenuPriority);
   // m_memberView->setBounceable(false);
    m_memberView->setDelegate(this);
    m_memberView->reloadData();

    bool isKickPlayerClicked = GuildManager::Get()->getIsKickMemeber();
    if(needSetOffset == true && isKickPlayerClicked == false)
    {
        m_memberView->setContentOffset(offset);
    }
}

void GuildMemberLayer::RefreshCell(CCTableViewCell* cell, unsigned int index)
{
    if (! cell->getChildByTag(111))
    {
        CCLayer* layer = CreateSingleGuildMemberLayer();
        cell->addChild(layer, 0, 111);
    }

    SingleGuildMemberLayer* sLayer = dynamic_cast<SingleGuildMemberLayer*>(cell->getChildByTag(111));
    if (sLayer != NULL)
    {
        sLayer->InitLayer(index, this, m_guildMembers[index]);
    }
}

CCLayer* GuildMemberLayer::CreateSingleGuildMemberLayer()
{
    CCNodeLoader* loader = new SingleGuildMemberLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_guild_member", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_guild_member");
    return dynamic_cast<CCLayer*>(node);
}

SingleGuildMemberLayer::SingleGuildMemberLayer() :
    m_nameLabel(NULL),
    m_levelLabel(NULL),
    m_powerLabel(NULL),
    m_contributeLabel(NULL),
    m_jobLabel(NULL),
    m_statusLabel(NULL),
    m_parentLayer(NULL),
    m_index(0),
    m_lightSprite(NULL),
    m_pLookGBtn(NULL)
{

}

SingleGuildMemberLayer::~SingleGuildMemberLayer()
{
    setNameLabel(NULL);
    setLevelLabel(NULL);
    setPowerLabel(NULL);
    setContributeLabel(NULL);
    setJobLabel(NULL);
    setStatusLabel(NULL);
    setLightSprite(NULL);
    setLookGBtn(NULL);
}

SEL_MenuHandler SingleGuildMemberLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleGuildMemberLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "glook", SingleGuildMemberLayer::onGLookBtnClicked);
    return NULL;
}

bool SingleGuildMemberLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",       CCLabelTTF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "level",      CCLabelTTF*, m_levelLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "power",      CCLabelTTF*, m_powerLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute", CCLabelTTF*, m_contributeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "job",        CCLabelTTF*, m_jobLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "status",     CCLabelTTF*, m_statusLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "light",      CCScale9Sprite*, m_lightSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLookGBtn",CCControlButton*, m_pLookGBtn);
    return false;
}

void SingleGuildMemberLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    m_pLookGBtn->setTouchPriority(kCCScrollMenuPriority);
}

void SingleGuildMemberLayer::InitLayer(unsigned int index, GuildMemberLayer* layer, GuildMemberInfo info)
{
    m_index = index;
    m_curInfo = info;
    m_parentLayer = layer;
    if (layer->getClickIndex() == index)
    {
        m_lightSprite->setVisible(true);
    } else
    {
        m_lightSprite->setVisible(false);
    }
    
    // 初始化界面
    m_nameLabel->setString(info.name.c_str());
    m_levelLabel->setString(CCString::createWithFormat("%d", info.lv)->getCString());
    m_jobLabel->setString(GuildManager::Get()->getGuildPersonTitle(info.status).c_str());
    m_powerLabel->setString(CCString::createWithFormat("%d", info.power_score)->getCString());
    m_contributeLabel->setString(CCString::createWithFormat("%d", info.total_contribute)->getCString());

    int onlinetime = info.online;
    if(onlinetime == 0)
    {
        // online
        m_statusLabel->setString(Localizatioin::getLocalization("M_FRIEND_ONLINE_1"));
        onLine();
    }
    else
    {
        //m_statusLabel->setString(Localizatioin::getLocalization("M_FRIEND_ONLINE_2"));
        m_statusLabel->setString(GuildManager::Get()->getMemOfflineTimeStr((time_t)onlinetime).c_str());
        offLine();
    }
     
    // is self or not
    int selfId = UserData::getUserId().getID();
    int selfRegTm = UserData::getUserId().getRegTime();
    int selfChannel = UserData::getUserId().getChannel();
    if(selfId == info.userid && selfRegTm == info.reg_tm && selfChannel == info.channel_id)
    {
        m_pLookGBtn->setVisible(false);
    }
    else
    {
        m_pLookGBtn->setVisible(true);
    }
}

void SingleGuildMemberLayer::onLine()
{
    m_nameLabel->setColor(ccGREEN);
    m_levelLabel->setColor(ccGREEN);
    m_powerLabel->setColor(ccGREEN);
    m_contributeLabel->setColor(ccGREEN);
    m_jobLabel->setColor(ccGREEN);
    m_statusLabel->setColor(ccGREEN);
}

void SingleGuildMemberLayer::offLine()
{
    m_nameLabel->setColor(ccGRAY);
    m_levelLabel->setColor(ccGRAY);
    m_powerLabel->setColor(ccGRAY);
    m_contributeLabel->setColor(ccGRAY);
    m_jobLabel->setColor(ccGRAY);
    m_statusLabel->setColor(ccGRAY);
}

void SingleGuildMemberLayer::onGLookBtnClicked(CCObject* object, CCControlEvent event)
{
    int openMenu =  LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getChatPlayerOpen");

    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerId",m_curInfo.userid);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setRegtime",m_curInfo.reg_tm);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setChannel",m_curInfo.channel_id);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerLevel",m_curInfo.lv);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerStrength",m_curInfo.power_score);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerName",m_curInfo.name.c_str());
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setInitPlayer");
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setModelLess", true);
}

GuildNoticeLayer::GuildNoticeLayer() :
    m_titleLabel(NULL),
    m_noticeLabel(NULL),
    m_confirmButton(NULL),
    m_refuseButton(NULL),
    m_pTipLabel(NULL)
{

}

GuildNoticeLayer::~GuildNoticeLayer()
{
    setTitleLabel(NULL);
    setNoticeLabel(NULL);
    setConfirmButton(NULL);
    setRefuseButton(NULL);
    setTipLabel(NULL);
}

SEL_MenuHandler GuildNoticeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildNoticeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "confirm", GuildNoticeLayer::onConfirmLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "refuse",  GuildNoticeLayer::onRefuseLayer);
    return NULL;
}

bool GuildNoticeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "title",       CCLabelTTF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTipLabel",       CCLabelTTF*, m_pTipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "notice",      CCLabelTTF*, m_noticeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "confirm_btn", CCControlButton*, m_confirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refuse_btn",  CCControlButton*, m_refuseButton);
    return false; 
}

void GuildNoticeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);
    m_pTipLabel->setString(Localizatioin::getLocalization("G_QUIT_EXPLANATION"));
    initlayer();
}

void GuildNoticeLayer::initlayer()
{
    int tag = GuildManager::Get()->getMemberOpenType();
    switch (tag)
    {
    case TAG_QUIT:
        {
            m_titleLabel->setString(Localizatioin::getLocalization("G_MEMBERS_BUTTON4"));
            CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_QUIT"), GuildManager::Get()->getStr2().c_str());
            m_noticeLabel->setString(str->getCString());
            m_pTipLabel->setVisible(true);
            m_pTipLabel->setString(Localizatioin::getLocalization("G_QUIT_EXPLANATION"));
        }
        break;

    case TAG_JIESAN:
        {
            m_titleLabel->setString(Localizatioin::getLocalization("G_MEMBERS_DISBAND"));
            CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_DISBAND"), GuildManager::Get()->getStr2().c_str());
            m_noticeLabel->setString(str->getCString());
            m_pTipLabel->setVisible(true);
            m_pTipLabel->setString(Localizatioin::getLocalization("G_QUIT_EXPLANATION2"));
        }
        break;

    case TAG_APPOINT:
        {
            m_titleLabel->setString(Localizatioin::getLocalization("G_MEMBERS_APPOINTMENT"));
            CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_APPOINTMENT_TIPS3"), GuildManager::Get()->getStr2().c_str());
            m_noticeLabel->setString(str->getCString());
            m_pTipLabel->setVisible(false);
        }
        break;

    case TAG_KICK:
        {
            m_titleLabel->setString(Localizatioin::getLocalization("G_MEMBERS_SACK"));
            CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_SACK_1"), GuildManager::Get()->getStr2().c_str());
            m_noticeLabel->setString(str->getCString());
            m_pTipLabel->setVisible(false);
        }
        break;
    default:
        break;
    }
}

bool GuildNoticeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildNoticeLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildNoticeLayer::onEnter()
{
    CCLayer::onEnter();
    m_confirmButton->setTouchPriority(kCCLayerTouchPriority);
    m_refuseButton->setTouchPriority(kCCLayerTouchPriority);
}


void GuildNoticeLayer::onConfirmLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildNoticeLayer(false);
    }

    int tag = GuildManager::Get()->getMemberOpenType();
    GuildManager::Get()->setIsKickMemeber(false);
    switch (tag)
    {
    case TAG_QUIT:
        GuildManager::Get()->SendQuitGuild();
        break;

    case TAG_JIESAN:
        GuildManager::Get()->SendJiesanGuld();
        break;

    case TAG_APPOINT:
        GuildManager::Get()->SendChangeGuildMemberStatus(GuildManager::Get()->getMemberStatus());
        break;

    case TAG_KICK:
        GuildManager::Get()->setIsKickMemeber(true);
        GuildManager::Get()->SendChangeGuildMemberStatus(GuildManager::Get()->getMemberStatus());
        break;
    default:
        break;
    }
    
}

void GuildNoticeLayer::onRefuseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildNoticeLayer(false);
    }
}

GuildAppointLayer::GuildAppointLayer() :
    m_titleLabel(NULL),
    m_nameLabel(NULL),
    m_type1Layer(NULL),
    m_type2Layer(NULL),
    m_type3Layer(NULL),
    m_type4Layer(NULL),
    m_confirmButton(NULL),
    m_tipLabel(NULL),
    m_checkBox1(NULL),
    m_checkBox2(NULL),
    m_checkBox3(NULL),
    m_checkBox4(NULL),
    m_pCloseBtn(NULL),
    m_selectType(0)
{

}

GuildAppointLayer::~GuildAppointLayer()
{
    setTipLabel(NULL);
    setNameLabel(NULL);
    setType1Layer(NULL);
    setType2Layer(NULL);
    setType3Layer(NULL);
    setType4Layer(NULL);
    setConfirmButton(NULL);
    setTipLabel(NULL);
    setCloseBtn(NULL);
}

SEL_MenuHandler GuildAppointLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildAppointLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "confirm", GuildAppointLayer::onConfirm);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",   GuildAppointLayer::onCLoseBtn);
    return NULL;
}

bool GuildAppointLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "title",          CCLabelTTF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",           CCLabelTTF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "type_1_layer",   CCLayer*, m_type1Layer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "type_2_layer",   CCLayer*, m_type2Layer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "type_3_layer",   CCLayer*, m_type3Layer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "type_4_layer",   CCLayer*, m_type4Layer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "confirm_btn",    CCControlButton*, m_confirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tip",            CCLabelTTF*, m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",      CCControlButton*, m_pCloseBtn);
    return false;
}

void GuildAppointLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    // 添加四个editBox
    CCSpriteFrame* norFrame1 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame1 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox1(CCCheckBox::create(norFrame1, selFrame1, checkbox_selector(GuildAppointLayer::onClickCheckBox1), this, kCCLayerTouchPriority));
    m_checkBox1->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox1->setPosition(ccp(m_type1Layer->getContentSize().width / 2, m_type1Layer->getContentSize().height / 2));
    m_type1Layer->addChild(m_checkBox1);

    CCSpriteFrame* norFrame2 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame2 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox2(CCCheckBox::create(norFrame2, selFrame2, checkbox_selector(GuildAppointLayer::onClickCheckBox2), this, kCCLayerTouchPriority));
    m_checkBox2->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox2->setPosition(ccp(m_type2Layer->getContentSize().width / 2, m_type2Layer->getContentSize().height / 2));
    m_type2Layer->addChild(m_checkBox2);

    CCSpriteFrame* norFrame3 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame3 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox3(CCCheckBox::create(norFrame3, selFrame3, checkbox_selector(GuildAppointLayer::onClickCheckBox3), this, kCCLayerTouchPriority));
    m_checkBox3->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox3->setPosition(ccp(m_type3Layer->getContentSize().width / 2, m_type3Layer->getContentSize().height / 2));
    m_type3Layer->addChild(m_checkBox3);

    CCSpriteFrame* norFrame4 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame4 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox4(CCCheckBox::create(norFrame4, selFrame4, checkbox_selector(GuildAppointLayer::onClickCheckBox4), this, kCCLayerTouchPriority));
    m_checkBox4->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox4->setPosition(ccp(m_type4Layer->getContentSize().width / 2, m_type4Layer->getContentSize().height / 2));
    m_type4Layer->addChild(m_checkBox4);

    m_pCloseBtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

bool GuildAppointLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildAppointLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildAppointLayer::onEnter()
{
    CCLayer::onEnter();
    m_confirmButton->setTouchPriority(kCCLayerTouchPriority);
}

void GuildAppointLayer::onConfirm(CCObject* sender, CCControlEvent pControlEvent)
{
    if (m_selectType == 0)
    {
        return;
    }

    

    std::vector<StrGuildStatus> vtInfos;
    vtInfos.clear();
    StrGuildStatus statusInfo;
    statusInfo.user_id = m_memberInfo.userid;
    statusInfo.reg_tm = m_memberInfo.reg_tm;
    statusInfo.channel_id = m_memberInfo.channel_id;
    statusInfo.status = m_selectType;
    vtInfos.push_back(statusInfo);


    // owner make a comfirm
    if(m_selectType == 4)
    {
        GuildManager::Get()->setMemberOpenType(TAG_APPOINT);
        GuildManager::Get()->setMemberStatus(vtInfos);
        MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
        if (mLayer)
        {
            mLayer->ShowGuildAppointLayer(false);
            mLayer->ShowGuildNoticeLayer(true);
        }
        return;
    }

    GuildManager::Get()->SendChangeGuildMemberStatus(vtInfos);

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildAppointLayer(false);
    }
}

void GuildAppointLayer::onClickCheckBox1(bool flag)
{
    if (flag == true)
    {
        m_checkBox2->setClickState(false);
        m_checkBox3->setClickState(false);
        m_checkBox4->setClickState(false);
    
        m_selectType = 1;
    } else
    {
        m_selectType = 0;
    }
}

void GuildAppointLayer::onClickCheckBox2(bool flag)
{
    if (flag == true)
    {
        m_checkBox1->setClickState(false);
        m_checkBox3->setClickState(false);
        m_checkBox4->setClickState(false);

        m_selectType = 2;
    } else
    {
        m_selectType = 0;
    }
}

void GuildAppointLayer::onClickCheckBox3(bool flag)
{
    if (flag == true)
    {
        m_checkBox1->setClickState(false);
        m_checkBox2->setClickState(false);
        m_checkBox4->setClickState(false);

        m_selectType = 3;
    } else
    {
        m_selectType = 0;
    }
}

void GuildAppointLayer::onClickCheckBox4(bool flag)
{
    if (flag == true)
    {
        m_checkBox1->setClickState(false);
        m_checkBox2->setClickState(false);
        m_checkBox3->setClickState(false);

        m_selectType = 4;
    } else
    {
        m_selectType = 0;
    }
}

void GuildAppointLayer::initLayer(GuildMemberInfo info)
{
    m_memberInfo = info;
    m_nameLabel->setString(info.name.c_str());

    int curGlevel = GuildManager::Get()->getPlayerOwnerGuildInfo().guildinfo().lv();
    int secondChariManNum = GuildManager::Get()->getAppointSecChariManNumByLevel(curGlevel);
    int elitNum = GuildManager::Get()->getAppointEliteNumByLevel(curGlevel);
    CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_APPOINTMENT_TEXT2"), elitNum, secondChariManNum);
    m_tipLabel->setString(str->getCString());
}

void GuildAppointLayer::onCLoseBtn(CCObject* obj, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildAppointLayer(false);
    }
}