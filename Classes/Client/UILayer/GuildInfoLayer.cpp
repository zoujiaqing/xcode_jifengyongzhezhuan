#include "GuildInfoLayer.h"
#include "MainMenuLayer.h"
#include "UIManager.h"
#include "ButtonPerformanceManager.h"
#include "InterfaceManager.h"
#include "GMessage.h"
#include "UserData.h"
#include "GlobalFunction.h"
#include "ItemManager.h"
#include "Localization.h"
#include "SkeletonAnimRcsManager.h"
#include "GameAudioManager.h"
#include "SoundEnum.h"
#include "ParticleManager.h"
#include "InterfaceManager.h"
#include "InstanceManager.h"
#include "GameFontManager.h"
#include "LuaTinkerManager.h"
#include "GameConfigFromLuaManager.h"
#include "UserData.h"
#include "BYGraySprite.h"
#include "LevelManager.h"
#include "GuildInstanceManager.h"

GuildInfoLayer::GuildInfoLayer() :
    m_titleLable(NULL),
    m_allInfoLabel1(NULL),
    m_allInfoLabel2(NULL),
    m_allInfoLabel3(NULL),
    m_allInfoLabel4(NULL),
    m_allInfoName(NULL),
    m_allInfoMemberNo(NULL),
    m_allInfoLv(NULL),
    m_allInfoExp(NULL),
    m_singleInfoLabel1(NULL),
    m_singleInfoLabel2(NULL),
    m_singleInfoLabel3(NULL),
    m_singleInfoName(NULL),
    m_singleInfoPlace(NULL),
    m_singleInfoAllContributon(NULL),
    m_singleInfoLesContribution(NULL),
    m_label1(NULL),
    m_label2(NULL),
    m_announcement(NULL),
    m_allInfoLayer(NULL),
    m_singleInfoLayer(NULL),
    m_allInfoButton(NULL),
    m_singleInfoButton(NULL),
    m_editButton(NULL),
    m_requestButton(NULL),
    m_historyButton(NULL),
    m_rankButton(NULL),
    m_memberButton(NULL),
    m_enterButton(NULL),
    m_closeButton(NULL),
    m_contributeButton(NULL),
    m_blessButton(NULL),
    m_shopButton(NULL),
    m_instanceButton(NULL),
    m_pRequestTip(NULL),
    m_pContributeTip(NULL),
    m_pBlessTip(NULL),
	m_redEnvoBtn(NULL),
	m_pRedEnvoTip(NULL)
{

}

GuildInfoLayer::~GuildInfoLayer()
{
    setTitleLabel(NULL);
    setAllInfoLabel1(NULL);
    setAllInfoLabel2(NULL);
    setAllInfoLabel3(NULL);
    setAllInfoLabel4(NULL);
    setAllInfoName(NULL);
    setAllInfoMemberNo(NULL);
    setAllInfoLv(NULL);
    setALLInfoExp(NULL);
    setSingleInfoLabel1(NULL);
    setSingleInfoLabel2(NULL);
    setSingleInfoLabel3(NULL);
    setSingleInfoName(NULL);
    setSingleInfoPlace(NULL);
    setSingleInfoAllContribution(NULL);
    setSingleInfoLesContribution(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setAnnouncement(NULL);
    setAllInfoLayer(NULL);
    setSingleInfoLayer(NULL);
    setAllInfoButton(NULL);
    setSingleInfoButton(NULL);
    setEditButton(NULL);
    setRequestButton(NULL);
    setHistoryButton(NULL);
    setRankButton(NULL);
    setMemberButton(NULL);
    setEnterButton(NULL);
    setCloseButton(NULL);
    setContributeButton(NULL);
    setBlessButton(NULL);
    setShopButton(NULL);
    setInstanceButton(NULL);
    setBlessTip(NULL);
    setContributeTip(NULL);
    setRequestTip(NULL);
	setRedEnvoBtn(NULL);
	setRedEnvoTip(NULL);

    ButtonPerformanceManager::Get()->removePerformanceButton(this);
    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler GuildInfoLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildInfoLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",       GuildInfoLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "all_info",    GuildInfoLayer::onAllInfoClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "single_info", GuildInfoLayer::onSingleInfoClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "edit",        GuildInfoLayer::onEdit);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "enter",       GuildInfoLayer::onEnterClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "contribute",  GuildInfoLayer::onContributeClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bless",       GuildInfoLayer::onBlessClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "request",     GuildInfoLayer::onRequestClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "member",      GuildInfoLayer::onMemberClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "shop",        GuildInfoLayer::onShopClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "history",     GuildInfoLayer::onHistoryClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "rank",        GuildInfoLayer::onRankButtonClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "instance",    GuildInfoLayer::onGuildInstanceClicked);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "red_envo",    GuildInfoLayer::onGuildRedEnvoClicked);
    return NULL;
}

bool GuildInfoLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "title",           CCLabelTTF*, m_titleLable);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_label1", CCLabelTTF*, m_allInfoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_label2", CCLabelTTF*, m_allInfoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_label3", CCLabelTTF*, m_allInfoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_label4", CCLabelTTF*, m_allInfoLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_name",   CCLabelTTF*, m_allInfoName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_member_no", CCLabelTTF*, m_allInfoMemberNo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_lv",     CCLabelTTF*, m_allInfoLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_exp",    CCLabelTTF*, m_allInfoExp);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_label1", CCLabelTTF*, m_singleInfoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_label2", CCLabelTTF*, m_singleInfoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_label3", CCLabelTTF*, m_singleInfoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_name",   CCLabelTTF*, m_singleInfoName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_place",  CCLabelTTF*, m_singleInfoPlace);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_all_contribution", CCLabelTTF*, m_singleInfoAllContributon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_les_contribution", CCLabelTTF*, m_singleInfoLesContribution);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label1",          CCLabelTTF*, m_label1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label2",          CCLabelTTF*, m_label2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "announcement",    CCLabelTTF*, m_announcement);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_layer",  CCLayer*, m_allInfoLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_layer", CCLayer*, m_singleInfoLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_info_btn",    CCControlButton*, m_allInfoButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_info_btn", CCControlButton*, m_singleInfoButton);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "edit_btn",    CCControlButton*, m_editButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "request_btn", CCControlButton*, m_requestButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "history_btn", CCControlButton*, m_historyButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rank_btn",    CCControlButton*, m_rankButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "member_btn",  CCControlButton*, m_memberButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "enter_btn",   CCControlButton*, m_enterButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",   CCControlButton*, m_closeButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute_button", CCControlButton*, m_contributeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bless_button",      CCControlButton*, m_blessButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "shop_button",       CCControlButton*, m_shopButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "instance_button",   CCControlButton*, m_instanceButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "red_envo_btn",   CCControlButton*, m_redEnvoBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRequestTip",      CCLayer*, m_pRequestTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContributeTip",   CCLayer*, m_pContributeTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBlessTip",        CCLayer*, m_pBlessTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRedEnvoTip",        CCLayer*, m_pRedEnvoTip);

    return false;
}

void GuildInfoLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    m_allInfoLayer->setVisible(true);
    m_singleInfoLayer->setVisible(false);

    // 向服务器请求帮会基本信息
    GuildManager::Get()->SendGetGuildInfo();
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    m_titleLable->setString(UserData::GetUserInfo().guildName.c_str());
}

bool GuildInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildInfoLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void GuildInfoLayer::onEnter()
{
    CCLayer::onEnter();

    m_closeButton->setTouchPriority(kCCScrollMenuPriority);
    m_allInfoButton->setTouchPriority(kCCScrollMenuPriority);
    m_singleInfoButton->setTouchPriority(kCCScrollMenuPriority);
    m_editButton->setTouchPriority(kCCScrollMenuPriority);
    m_requestButton->setTouchPriority(kCCScrollMenuPriority);
    m_historyButton->setTouchPriority(kCCScrollMenuPriority);
    m_rankButton->setTouchPriority(kCCScrollMenuPriority);
    m_memberButton->setTouchPriority(kCCScrollMenuPriority);
    m_enterButton->setTouchPriority(kCCScrollMenuPriority);

    m_contributeButton->setTouchPriority(kCCScrollMenuPriority);
    m_blessButton->setTouchPriority(kCCScrollMenuPriority);
    m_shopButton->setTouchPriority(kCCScrollMenuPriority);
    m_instanceButton->setTouchPriority(kCCScrollMenuPriority);
	m_redEnvoBtn->setTouchPriority(kCCScrollMenuPriority);

    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_allInfoButton,     0);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_singleInfoButton,  1);

    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);
}

void GuildInfoLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_UPDATE_GUILD_INFO :
        {
            InitGuildInfo();
            break;
        }

    default:
        break;
    }
}

// 初始化公会信息
void GuildInfoLayer::InitGuildInfo()
{
    CSQueryPlayerGuildInfoRsp playerGuildInfo = GuildManager::Get()->getPlayerOwnerGuildInfo();

    // 公会信息
    m_allInfoName->setString(playerGuildInfo.guildinfo().oname().c_str());
    m_allInfoMemberNo->setString(CCString::createWithFormat("%d / %d", playerGuildInfo.guildinfo().member_num(), playerGuildInfo.guildinfo().member_num_limit())->getCString());
    m_allInfoLv->setString(CCString::createWithFormat("Lv %d", playerGuildInfo.guildinfo().lv())->getCString());
    m_allInfoExp->setString(CCString::createWithFormat("%d / %d", playerGuildInfo.guildinfo().exp(), playerGuildInfo.guildinfo().exp_limit())->getCString());

    // 个人信息
    m_singleInfoName->setString(UserData::getUserName());
    int idx = playerGuildInfo.playerinfo().status();
    m_singleInfoPlace->setString(GuildManager::Get()->getGuildPersonTitle(idx).c_str());
    m_singleInfoAllContributon->setString(CCString::createWithFormat("%d", playerGuildInfo.playerinfo().total_contribute())->getCString());
    m_singleInfoLesContribution->setString(CCString::createWithFormat("%d", playerGuildInfo.playerinfo().contribute())->getCString());

    // 公告
    m_announcement->setString(playerGuildInfo.guildinfo().notice().c_str());

    int status = GuildManager::Get()->getPlayerOwnerGuildInfo().playerinfo().status();
    if(status == 1)
    {
        m_requestButton->setEnabled(false);
    }
    else
    {
        m_requestButton->setEnabled(true);
    }

    if(status == 4)
    {
        m_editButton->setVisible(true);
    }
    else
    {
        m_editButton->setVisible(false);
    }
}

void GuildInfoLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoLayer(false);
    }
}

void GuildInfoLayer::onEdit(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoEditLayer(true);
    }
}

void GuildInfoLayer::onEnterClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    InstanceManager::Get()->onEnterSpecialEntrance(ENTERANCE_GUILDSCENE);
}

void GuildInfoLayer::onGuildInstanceClicked(CCObject * sender, CCControlEvent event)
{
	GameAudioManager::sharedManager()->playEffect(350000,false);

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowGuildInstanceListLayer(true);
		//mLayer->ShowGuildInfoLayer(false);
	}
}

void GuildInfoLayer::onContributeClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildContributeLayer(true);
    }
}

void GuildInfoLayer::onBlessClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildBlessLayer(true);
    }
}

void GuildInfoLayer::onGuildRedEnvoClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoLayer(false);
		mLayer->ShowGuildRedEnvolopeLayer(true);
    }
}

void GuildInfoLayer::onRequestClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildRequestLayer(true);
    }
}

void GuildInfoLayer::onMemberClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoLayer(false);
        mLayer->ShowGuildMemberLayer(true);
    }
}

void GuildInfoLayer::onShopClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        GuildManager::Get()->setIsGuildInfoNeedOpen(true);
        mLayer->ShowGuildInfoLayer(false);
        mLayer->ShowGuildShopLayer(true);
    }
}

void GuildInfoLayer::onHistoryClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoLayer(false);
        mLayer->ShowGuildRecordLayer(true);
    }
}

void GuildInfoLayer::onRankButtonClicked(CCObject* sender, CCControlEvent event)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoLayer(false);
        GuildManager::Get()->setIsRankNeedOpen(true);
        GuildManager::Get()->setIsGuildInfoNeedOpen(true);
        mLayer->ShowGuildListLayer(true);
    }
}

void GuildInfoLayer::showRequestTip(bool flag)
{
    m_pRequestTip->removeAllChildren();
    if(flag == true)
    {
        CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY");
        sprite->setPosition(ccp(m_pRequestTip->getContentSize().width / 2, m_pRequestTip->getContentSize().height / 2));
        m_pRequestTip->addChild(sprite);

        CCActionInterval* action1 = NULL;
        action1 = CCMoveBy::create(0.4,ccp(0,10));

        CCActionInterval* action_back1 = action1->reverse();
        CCActionInterval* seq1 = (CCActionInterval*)(CCSequence::create( action1, action_back1, NULL ));

        CCRepeatForever* pAction1 = CCRepeatForever::create(seq1);
        sprite->runAction(pAction1);
    }
} 
void GuildInfoLayer::showContributeTip(bool flag)
{
    m_pContributeTip->removeAllChildren();
    if(flag == true)
    {
        CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY");
        sprite->setPosition(ccp(m_pContributeTip->getContentSize().width / 2, m_pContributeTip->getContentSize().height / 2));
        m_pContributeTip->addChild(sprite);

        CCActionInterval* action1 = NULL;
        action1 = CCMoveBy::create(0.4,ccp(0,10));

        CCActionInterval* action_back1 = action1->reverse();
        CCActionInterval* seq1 = (CCActionInterval*)(CCSequence::create( action1, action_back1, NULL ));

        CCRepeatForever* pAction1 = CCRepeatForever::create(seq1);
        sprite->runAction(pAction1);
    }
}
void GuildInfoLayer::showBlessTIp(bool flag)
{
    m_pBlessTip->removeAllChildren();
    if(flag == true)
    {
        CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY");
        sprite->setPosition(ccp(m_pBlessTip->getContentSize().width / 2, m_pBlessTip->getContentSize().height / 2));
        m_pBlessTip->addChild(sprite);

        CCActionInterval* action1 = NULL;
        action1 = CCMoveBy::create(0.4,ccp(0,10));

        CCActionInterval* action_back1 = action1->reverse();
        CCActionInterval* seq1 = (CCActionInterval*)(CCSequence::create( action1, action_back1, NULL ));

        CCRepeatForever* pAction1 = CCRepeatForever::create(seq1);
        sprite->runAction(pAction1);
    }
}

void GuildInfoLayer::showRedEnvoTip(bool flag)
{
	m_pRedEnvoTip->removeAllChildren();
    if(flag == true)
    {
        CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY");
        sprite->setPosition(ccp(m_pRedEnvoTip->getContentSize().width / 2, m_pRedEnvoTip->getContentSize().height / 2));
        m_pRedEnvoTip->addChild(sprite);

        CCActionInterval* action1 = NULL;
        action1 = CCMoveBy::create(0.4,ccp(0,10));

        CCActionInterval* action_back1 = action1->reverse();
        CCActionInterval* seq1 = (CCActionInterval*)(CCSequence::create( action1, action_back1, NULL ));

        CCRepeatForever* pAction1 = CCRepeatForever::create(seq1);
        sprite->runAction(pAction1);
    }
}

void GuildInfoLayer::onAllInfoClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    m_allInfoLayer->setVisible(true);
    m_singleInfoLayer->setVisible(false);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);
}

void GuildInfoLayer::onSingleInfoClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    m_allInfoLayer->setVisible(false);
    m_singleInfoLayer->setVisible(true);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 1);
}

GuildInfoEditLayer::GuildInfoEditLayer() :
    m_closeBtn(NULL),
    m_confirmBtn(NULL),
    m_backLayer(NULL),
    m_noticeEditBox(NULL),
    m_announceEditBox(NULL),
    m_noticeLayer(NULL),
    m_announceLayer(NULL),
    m_pText1(NULL),
    m_pText2(NULL)
{

}

GuildInfoEditLayer::~GuildInfoEditLayer()
{
    setCloseBtn(NULL);
    setConfirmBtn(NULL);
    setBackLayer(NULL);
    setNoticeEditBox(NULL);
    setAnnounceEditBox(NULL);
    setNoticeLayer(NULL);
    setAnnounceLayer(NULL);
    setText1(NULL);
    setText2(NULL);
}

SEL_MenuHandler GuildInfoEditLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildInfoEditLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",   GuildInfoEditLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "confirm", GuildInfoEditLayer::onConfirm);
    return NULL;
}

bool GuildInfoEditLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",      CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "confirm_btn",    CCControlButton*, m_confirmBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back_layer",     CCLayer*,         m_backLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "notice_layer",   CCLayer*,         m_noticeLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "announce_layer", CCLayer*,         m_announceLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "text1",       CCLabelTTF*,         m_pText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "text2",       CCLabelTTF*,         m_pText2);
    return false;
}

void GuildInfoEditLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);
    
    // for backlayer
    m_backLayer->setContentSize(s);
    m_backLayer->setScale(m_backLayer->getScale() / scale);

    // 添加公告editBox
    CCScale9Sprite* back1 = CCScale9Sprite::createWithSpriteFrame(
        InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/vip.plist", "empty_button.png"));
    setNoticeEditBox(CCEditBox::create(m_noticeLayer->getContentSize(), back1));
    m_noticeLayer->addChild(m_noticeEditBox);
    m_noticeEditBox->setAnchorPoint(ccp(0, 0));
    m_noticeEditBox->setPlaceHolder(Localizatioin::getLocalization("G_ANNOUNCEMENT_TEXT4"));
    m_noticeEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_noticeEditBox->setFont("Helvetica", 20.0f);
    m_noticeEditBox->setTouchPriority(kCCScrollTouchPriority - 1);
    m_noticeEditBox->setMaxLength(60);
    m_noticeEditBox->setDelegate(this);
    m_noticeEditBox->setTag(1);

    CCScale9Sprite* back2 = CCScale9Sprite::createWithSpriteFrame(
        InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/vip.plist", "empty_button.png"));
    setAnnounceEditBox(CCEditBox::create(m_announceLayer->getContentSize(), back2));
    m_announceLayer->addChild(m_announceEditBox);
    m_announceEditBox->setAnchorPoint(ccp(0, 0));
    m_announceEditBox->setPlaceHolder(Localizatioin::getLocalization("G_ANNOUNCEMENT_TEXT2"));
    m_announceEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_announceEditBox->setFont("Helvetica", 20.0f);
    m_announceEditBox->setTouchPriority(kCCScrollTouchPriority - 1);
    m_announceEditBox->setMaxLength(60);
    m_announceEditBox->setDelegate(this);
    m_announceEditBox->setTag(2);


    //   
    CSQueryPlayerGuildInfoRsp playerGuildInfo = GuildManager::Get()->getPlayerOwnerGuildInfo();
     // 公告
    if(strcmp(playerGuildInfo.guildinfo().notice().c_str(), "") != 0)
    {
        m_pText2->setString(playerGuildInfo.guildinfo().notice().c_str());
        m_announceEditBox->setPlaceHolder("");
        m_announceEditBox->setText("");
    }
    // 宣言
    if(strcmp(playerGuildInfo.guildinfo().note().c_str(), "") != 0)
    {
        m_pText1->setString(playerGuildInfo.guildinfo().note().c_str());
        m_noticeEditBox->setPlaceHolder("");
        m_noticeEditBox->setText("");
    }
}

bool GuildInfoEditLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildInfoEditLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildInfoEditLayer::onEnter()
{
    CCLayer::onEnter();

    m_closeBtn->setTouchPriority(kCCLayerTouchPriority);
    m_confirmBtn->setTouchPriority(kCCLayerTouchPriority);
}

void GuildInfoEditLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoEditLayer(false);
    }
}

void GuildInfoEditLayer::onConfirm(CCObject* sender, CCControlEvent pControlEvent)
{
    GuildManager::Get()->SendModifyNotice(m_pText2->getString(), m_pText1->getString());

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoEditLayer(false);
    }
}

void GuildInfoEditLayer::editBoxEditingDidBegin(CCEditBox* editBox)
{

}
void GuildInfoEditLayer::editBoxEditingDidEnd(CCEditBox* editBox)
{
    int tag = editBox->getTag();
    switch (tag)
    {
    case 1:
        if(strcmp(editBox->getText(), "") == 0)
        {
            editBox->setPlaceHolder(Localizatioin::getLocalization("G_ANNOUNCEMENT_TEXT4"));
            m_pText1->setString("");
        }
        else
        {
            m_pText1->setString(editBox->getText());
            editBox->setPlaceHolder("");
            editBox->setText("");
        }
        break;

    case 2:
        if(strcmp(editBox->getText(), "") == 0)
        {
            editBox->setPlaceHolder(Localizatioin::getLocalization("G_ANNOUNCEMENT_TEXT2"));
            m_pText2->setString("");
        }
        else
        {
            m_pText2->setString(editBox->getText());
            editBox->setPlaceHolder("");
            editBox->setText("");
        }
        break;

    default:
        break;
    }
}

GuildContributeLayer::GuildContributeLayer() :
    m_guildLv(NULL),
    m_guildExp(NULL),
    m_contribute(NULL),
    m_moneyLabel(NULL),
    m_diamondLabel(NULL),
    m_contributeButton1(NULL),
    m_contributeButton2(NULL),
    m_contributeButton3(NULL),
    m_contributeButton4(NULL),
    m_closeButton(NULL),
    m_pTitleLabel(NULL),
    m_pLabel1(NULL),    
    m_pLabel2(NULL),    
    m_pLabel3(NULL),    
    m_pLabel4(NULL),    
    m_pLabel5(NULL),    
    m_pLabel6(NULL),    
    m_pLabel7(NULL),    
    m_pLabel8(NULL),    
    m_pLabel9(NULL),    
    m_pLabel10(NULL),
    m_pChuConsume(NULL),  
    m_pZhongConsume(NULL),
    m_pGaoConsume(NULL),  
    m_pTeConsume(NULL),
    m_pChuExp(NULL),         
    m_pZhongExp(NULL),       
    m_pGaoExp(NULL),         
    m_pTeExp(NULL),          
    m_pChuContribute(NULL),  
    m_pZhongContribute(NULL),
    m_pGaoContribute(NULL),  
    m_pTeContribute(NULL),
    m_backLayer(NULL)
{
    m_pOldGuildLv = 1;
}

GuildContributeLayer::~GuildContributeLayer()
{
    setGuildLv(NULL);
    setGuildExp(NULL);
    setContributeLabel(NULL);
    setMoneyLabel(NULL);
    setDiamondLabel(NULL);
    setContributeButton1(NULL);
    setContributeButton2(NULL);
    setContributeButton3(NULL);
    setContributeButton4(NULL);
    setCloseButton(NULL);
    setTitleLabel(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
    setLabel4(NULL);
    setLabel5(NULL);
    setLabel6(NULL);
    setLabel7(NULL);
    setLabel8(NULL);
    setLabel9(NULL);
    setLabel10(NULL);
    setChuConsume(NULL);
    setZhongConsume(NULL);
    setGaoConsume(NULL);
    setTeConsume(NULL);
    setBackLayer(NULL);

    setChuExp(NULL);
    setZhongExp(NULL);
    setGaoExp(NULL);
    setTeExp(NULL);
    setChuContribute(NULL);
    setZhongContribute(NULL);
    setGaoContribute(NULL);
    setTeContribute(NULL);

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GuildContributeInfo_update");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "update_guild_contribute_times");    
}

SEL_MenuHandler GuildContributeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildContributeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", GuildContributeLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "contribute1", GuildContributeLayer::OnContribute1Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "contribute2", GuildContributeLayer::OnContribute2Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "contribute3", GuildContributeLayer::OnContribute3Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "contribute4", GuildContributeLayer::OnContribute4Clicked);
    return NULL;
}

bool GuildContributeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_lv",   CCLabelTTF*, m_guildLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp",  CCLabelTTF*, m_guildExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute", CCLabelTTF*, m_contribute);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "money",      CCLabelTTF*, m_moneyLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "diamond",    CCLabelTTF*, m_diamondLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back_layer", CCLayer*,         m_backLayer);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute_button1", CCControlButton*, m_contributeButton1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute_button2", CCControlButton*, m_contributeButton2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute_button3", CCControlButton*, m_contributeButton3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "contribute_button4", CCControlButton*, m_contributeButton4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",          CCControlButton*, m_closeButton);

    // mutil language
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleLabel",   CCLabelTTF*, m_pTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label1",          CCLabelTTF*, m_pLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label2",          CCLabelTTF*, m_pLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label3",          CCLabelTTF*, m_pLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label4",          CCLabelTTF*, m_pLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label5",          CCLabelTTF*, m_pLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label6",          CCLabelTTF*, m_pLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label7",          CCLabelTTF*, m_pLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label8",          CCLabelTTF*, m_pLabel8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label9",          CCLabelTTF*, m_pLabel9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label10",          CCLabelTTF*, m_pLabel10);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChuConsume",     CCLabelTTF*, m_pChuConsume);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZhongConsume",   CCLabelTTF*, m_pZhongConsume);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGaoConsume",     CCLabelTTF*, m_pGaoConsume);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTeConsume",      CCLabelTTF*, m_pTeConsume);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "chu_exp",          CCLabelTTF*, m_pChuExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "zhong_exp",        CCLabelTTF*, m_pZhongExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gao_exp",          CCLabelTTF*, m_pGaoExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "te_exp",           CCLabelTTF*, m_pTeExp);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "chu_contribute",     CCLabelTTF*, m_pChuContribute);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "zhong_contribute",   CCLabelTTF*, m_pZhongContribute);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gao_contribute",     CCLabelTTF*, m_pGaoContribute);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "te_contribute",      CCLabelTTF*, m_pTeContribute);

    return false;
}

void GuildContributeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    // for backlayer
    m_backLayer->setContentSize(s);
    m_backLayer->setScale(m_backLayer->getScale() / scale);

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                        callfuncO_selector(GuildContributeLayer::onContributeInfoCallBack), "GuildContributeInfo_update", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
        callfuncO_selector(GuildContributeLayer::onContributeTimesUpdate), "update_guild_contribute_times", NULL);

    // init
    CSQueryPlayerGuildInfoRsp playerGuildInfo = GuildManager::Get()->getPlayerOwnerGuildInfo();
    int glv = playerGuildInfo.guildinfo().lv();
    m_pOldGuildLv = glv;

    // GET INFO
    std::vector<int> tempVec;
    tempVec.clear();
    tempVec.push_back(21000);
    tempVec.push_back(21001);
    tempVec.push_back(21002);
    tempVec.push_back(21003);
    GuildManager::Get()->SendGetGuildContributeInfo(tempVec);

    InitMutilanguage();
    onContributeTimesUpdate(NULL);
}

void GuildContributeLayer::onContributeInfoCallBack(CCObject* obj)
{
    // cur diamond coin contibute
    USERINFO info = UserData::GetUserInfo();
    m_contribute->setString(IntToStr(info.m_guild_contributes));
    m_moneyLabel->setString(IntToStr(UserData::GetGold()));
    m_diamondLabel->setString(IntToStr(UserData::GetDiamond()));

    // tips
    int contributeTiems = UserData::GetUserInfo().m_guild_contribute_times;
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    int tag = mLayer->getLayerTagByName("GuildInfoLayer");
    if(1 - contributeTiems == 0)
    {
        dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag))->showContributeTip(false);
    }
    else
    {
        dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag))->showContributeTip(true);
    }

    int contributeTimes = UserData::GetUserInfo().m_guild_contribute_times;
    int blessTiems = UserData::GetUserInfo().m_guild_bless_times;
    int requestListnum = GuildManager::Get()->getRequestMemberList().size();
	if((1 - contributeTimes == 0) && blessTiems > 0 && requestListnum == 0 && !GuildManager::Get()->getIsHasRedEnvo())
    {
        GuildManager::Get()->HideMainTipIcon();
    }
    else
    {
        GuildManager::Get()->ShowMainTipIcon();
    }

    // guild lv exp
    CSQueryPlayerGuildInfoRsp playerGuildInfo = GuildManager::Get()->getPlayerOwnerGuildInfo();
    int glv = playerGuildInfo.guildinfo().lv();
    CCString* guildStr = CCString::createWithFormat(Localizatioin::getLocalization("G_LEVEL"),glv);
    m_guildLv->setString(guildStr->getCString());
    if(glv - m_pOldGuildLv > 0)
    {
        CCString* toLVStr = CCString::createWithFormat(Localizatioin::getLocalization("G_RECORD_TEXT8"), glv);
        ShowMessage(toLVStr->getCString());
    }
    m_pOldGuildLv = glv;
    
    int exp = playerGuildInfo.guildinfo().exp();
    int explimit = playerGuildInfo.guildinfo().exp_limit();
    if(exp == explimit)
    {
        m_guildExp->setString(Localizatioin::getLocalization("G_DONATE_EXPERIENCE2"));
    }
    else
    {
        CCString* tempStr = CCString::createWithFormat("%d/%d", exp, explimit);
        m_guildExp->setString(tempStr->getCString());
    }

    std::vector<GuildContributeInfo> vecInfo = GuildManager::Get()->getGuildContributeInfo();
    if(vecInfo.size() != 4)
    {
        CCLOG("guild contribute num is not equal to 4 !");
        return;
    }

    // chu
    m_pChuConsume->setString(IntToStr(vecInfo[0].fromItemAmount));
    m_pChuExp->setString(CCString::createWithFormat(
                    Localizatioin::getLocalization("G_DONATE_ADD_EXPERIENCE"),vecInfo[0].toItemAmount)->getCString());
    m_pChuContribute->setString(CCString::createWithFormat(
                    Localizatioin::getLocalization("G_DONATE_ADD_CONTRIBUTION"),vecInfo[0].toItemAmount2)->getCString());

    // zhong
    m_pZhongConsume->setString(IntToStr(vecInfo[1].fromItemAmount));
    m_pZhongExp->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("G_DONATE_ADD_EXPERIENCE"),vecInfo[1].toItemAmount)->getCString());
    m_pZhongContribute->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("G_DONATE_ADD_CONTRIBUTION"),vecInfo[1].toItemAmount2)->getCString());

    // gao
    m_pGaoConsume->setString(IntToStr(vecInfo[2].fromItemAmount));
    m_pGaoExp->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("G_DONATE_ADD_EXPERIENCE"),vecInfo[2].toItemAmount)->getCString());
    m_pGaoContribute->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("G_DONATE_ADD_CONTRIBUTION"),vecInfo[2].toItemAmount2)->getCString());

    // te
    m_pTeConsume->setString(IntToStr(vecInfo[3].fromItemAmount));
    m_pTeExp->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("G_DONATE_ADD_EXPERIENCE"),vecInfo[3].toItemAmount)->getCString());
    m_pTeContribute->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("G_DONATE_ADD_CONTRIBUTION"),vecInfo[3].toItemAmount2)->getCString());
}
 
void GuildContributeLayer::onContributeTimesUpdate(CCObject* obj)
{
    USERINFO info = UserData::GetUserInfo();
    int times = info.m_guild_contribute_times;
    if(times != 0)
    {
        // 按钮全灰掉
        m_contributeButton1->setEnabled(false);
        m_contributeButton2->setEnabled(false);
        m_contributeButton3->setEnabled(false);
        m_contributeButton4->setEnabled(false);
    }
    else
    {
        m_contributeButton1->setEnabled(true);
        m_contributeButton2->setEnabled(true);
        m_contributeButton3->setEnabled(true);
        m_contributeButton4->setEnabled(true);
    }
}

void GuildContributeLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildContributeLayer(false);
    }
}

void GuildContributeLayer::InitMutilanguage()
{
    m_pTitleLabel->setString(Localizatioin::getLocalization("G_DONATE_TITLE"));
    m_pLabel1->setString(Localizatioin::getLocalization("G_DONATE_LEVEL"));
    m_pLabel2->setString(Localizatioin::getLocalization("G_DONATE_EXPERIENCE1"));
    m_pLabel3->setString(Localizatioin::getLocalization("G_DONATE_TEXT1"));
    m_pLabel4->setString(Localizatioin::getLocalization("G_DONATE_TEXT2"));
    m_pLabel5->setString(Localizatioin::getLocalization("G_COST"));
    m_pLabel6->setString(Localizatioin::getLocalization("G_COST"));
    m_pLabel7->setString(Localizatioin::getLocalization("G_COST"));
    m_pLabel8->setString(Localizatioin::getLocalization("G_COST"));
    m_pLabel9->setString(Localizatioin::getLocalization("G_DONATE_CONTRIBUTION"));
    m_pLabel10->setString(Localizatioin::getLocalization("G_NOW"));
}

bool GuildContributeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildContributeLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildContributeLayer::onEnter()
{
    CCLayer::onEnter();

    m_contributeButton1->setTouchPriority(kCCLayerTouchPriority);
    m_contributeButton2->setTouchPriority(kCCLayerTouchPriority);
    m_contributeButton3->setTouchPriority(kCCLayerTouchPriority);
    m_contributeButton4->setTouchPriority(kCCLayerTouchPriority);
    m_closeButton->setTouchPriority(kCCLayerTouchPriority);
}

void GuildContributeLayer::OnContribute1Clicked(CCObject* pSender, CCControlEvent event)
{
    GuildManager::Get()->SendGuildContribute(21000);
}
void GuildContributeLayer::OnContribute2Clicked(CCObject* pSender, CCControlEvent event)
{
    GuildManager::Get()->SendGuildContribute(21001);
}
void GuildContributeLayer::OnContribute3Clicked(CCObject* pSender, CCControlEvent event)
{
    GuildManager::Get()->SendGuildContribute(21002);
}
void GuildContributeLayer::OnContribute4Clicked(CCObject* pSender, CCControlEvent event)
{
    GuildManager::Get()->SendGuildContribute(21003);
}

GuildRequestLayer::GuildRequestLayer() :
    m_settingButton(NULL),
    m_refuseButton(NULL),
    m_agreeButton(NULL),
    m_closeButton(NULL),
    m_checkBox(NULL),
    m_checkLayer(NULL),
    m_backLayer(NULL),
    m_tableView(NULL),
    m_contentLayer(NULL)
{
    m_vtStates.clear();
    m_vtGuildRequest.clear();
}

GuildRequestLayer::~GuildRequestLayer()
{
    setSettingButton(NULL);
    setRefuseButton(NULL);
    setAgreeButton(NULL);
    setCloseButton(NULL);
    setCheckBox(NULL);
    setCheckLayer(NULL);
    setBackLayer(NULL);
    setTableView(NULL);
    setContentLayer(NULL);

    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler GuildRequestLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildRequestLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",   GuildRequestLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "setting", GuildRequestLayer::onSettingClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "agree",   GuildRequestLayer::onConfirmClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "refuse",  GuildRequestLayer::onRefuseClicked);
    return NULL;
}

bool GuildRequestLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "setting_button", CCControlButton*, m_settingButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refuse_button",  CCControlButton*, m_refuseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "agree_button",   CCControlButton*, m_agreeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",      CCControlButton*, m_closeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "all_choose",     CCLayer*,         m_checkLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back_layer",     CCLayer*,         m_backLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "content_layer",  CCLayer*,         m_contentLayer);
    return false;
}

void GuildRequestLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    // for backlayer
    m_backLayer->setContentSize(s);
    m_backLayer->setScale(m_backLayer->getScale() / scale);

    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    // 添加一个checkBox
    CCSpriteFrame* norFrame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox(CCCheckBox::create(norFrame, selFrame, checkbox_selector(GuildRequestLayer::onClickCheckBox), this, kCCLayerTouchPriority));
    m_checkBox->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox->setPosition(ccp(m_checkLayer->getContentSize().width / 2, m_checkLayer->getContentSize().height / 2));
    m_checkLayer->addChild(m_checkBox);

    // 发送请求帮会成员信息
    GuildManager::Get()->SendGetGuildMember();
}

void GuildRequestLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_UPDATE_GUILD_MEMBER :
        {
            UpdateRequestInfo();
            break;
        }

    default:
        break;
    }
}

void GuildRequestLayer::UpdateRequestInfo()
{
    m_vtGuildRequest = GuildManager::Get()->getRequestMemberList();
    if (m_tableView == NULL)
    {
        setTableView(CCTableView::create(this, m_contentLayer->getContentSize()));
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_contentLayer->addChild(m_tableView);
        m_tableView->setTouchPriority(kCCScrollTouchPriority - 1);
        m_tableView->setDelegate(this);
    } else
    {
        m_tableView->reloadData();
    }

    m_vtStates.clear();
    m_vtStates.resize(numberOfCellsInTableView(m_tableView), false);
    onClickCheckBox(false);
}

void GuildRequestLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildRequestLayer(false);
    }
}

void GuildRequestLayer::onSettingClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildRequestLayer(false);
        mLayer->ShowGuildRequestSettingLayer(true);
    }
}

void GuildRequestLayer::onConfirmClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    std::vector<StrGuildStatus> vtGuildStatus;
    vtGuildStatus.clear();
    for (size_t i = 0; i < m_vtGuildRequest.size(); ++i)
    {
        if (m_vtStates[i] == false)
        {
            continue;
        }

        StrGuildStatus info;
        info.user_id = m_vtGuildRequest[i].userid;
        info.reg_tm = m_vtGuildRequest[i].reg_tm;
        info.channel_id = m_vtGuildRequest[i].channel_id;
        info.status = (int) GUILD_TO_MEMBER;
        vtGuildStatus.push_back(info);
    }

    if (vtGuildStatus.size() == 0)
    {
        return;
    }

    GuildManager::Get()->SendChangeGuildMemberStatus(vtGuildStatus);
}

void GuildRequestLayer::onRefuseClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    std::vector<StrGuildStatus> vtGuildStatus;
    vtGuildStatus.clear();
    for (size_t i = 0; i < m_vtGuildRequest.size(); ++i)
    {
        if (m_vtStates[i] == false)
        {
            continue;
        }

        StrGuildStatus info;
        info.user_id = m_vtGuildRequest[i].userid;
        info.reg_tm = m_vtGuildRequest[i].reg_tm;
        info.channel_id = m_vtGuildRequest[i].channel_id;
        info.status = (int) GUILD_TO_REFUSE;
        vtGuildStatus.push_back(info);
    }

    if (vtGuildStatus.size() == 0)
    {
        return;
    }

    GuildManager::Get()->SendChangeGuildMemberStatus(vtGuildStatus);
}

bool GuildRequestLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRequestLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildRequestLayer::onEnter()
{
    CCLayer::onEnter();

    m_settingButton->setTouchPriority(kCCLayerTouchPriority);
    m_refuseButton->setTouchPriority(kCCLayerTouchPriority);
    m_agreeButton->setTouchPriority(kCCLayerTouchPriority);
    m_closeButton->setTouchPriority(kCCLayerTouchPriority);
}

void GuildRequestLayer::onClickCheckBox(bool flag)
{
    for (unsigned int i = 0; i < m_vtStates.size(); ++i)
    {
        m_vtStates[i] = flag;
    }

    for (unsigned int i = 0; i < numberOfCellsInTableView(m_tableView); ++i)
    {
        CCTableViewCell* cell = m_tableView->cellAtIndex(i);
        
        if (cell == NULL)
        {
            continue;
        }

        SingleGuildRequestLayer* layer = dynamic_cast<SingleGuildRequestLayer*>(cell->getChildByTag(111));
        if (layer == NULL)
        {
            continue;
        }

        layer->getCheckBox()->setClickState(flag);
    }
}

void GuildRequestLayer::setClickState(unsigned int index, bool state)
{
    if (m_vtStates.size() <= index)
    {
        return;
    }

    m_vtStates[index] = state;
}

bool GuildRequestLayer::getClickState(unsigned int index)
{
    if (m_vtStates.size() <= index)
    {
        return false;
    }

    return m_vtStates[index];
}

void GuildRequestLayer::RefreshCell(CCTableViewCell* cell, unsigned int index)
{
    if (! cell->getChildByTag(111))
    {
        CCLayer* layer = CreateSingleRequestLayer();
        cell->addChild(layer, 0, 111);
    }

    SingleGuildRequestLayer* sLayer = dynamic_cast<SingleGuildRequestLayer*>(cell->getChildByTag(111));
    if (sLayer != NULL)
    {
        sLayer->InitLayer(index, this, m_vtGuildRequest[index]);
    }
}

CCSize GuildRequestLayer::cellSizeForTable(CCTableView *table)
{
    return CCSize(620.0f, 56.0f);
}

CCTableViewCell* GuildRequestLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int GuildRequestLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_vtGuildRequest.size();
}

void GuildRequestLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

CCLayer* GuildRequestLayer::CreateSingleRequestLayer()
{
    CCNodeLoader* loader = new SingleGuildRequestLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_guild_request", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_guild_request");
    SingleGuildRequestLayer* layer = dynamic_cast<SingleGuildRequestLayer*>(node);
    return layer;
}

SingleGuildRequestLayer::SingleGuildRequestLayer() :
    m_chooseLayer(NULL),
    m_checkBox(NULL),
    m_parentLayer(NULL),
    m_index(0),
    m_nameLabel(NULL),
    m_lvLabel(NULL),
    m_powerLabel(NULL)
{

}

SingleGuildRequestLayer::~SingleGuildRequestLayer()
{
    setChooseLayer(NULL);
    setCheckBox(NULL);
    setNameLabel(NULL);
    setLvLabel(NULL);
    setPowerLabel(NULL);
}

SEL_MenuHandler SingleGuildRequestLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleGuildRequestLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool SingleGuildRequestLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "choose", CCLayer*,    m_chooseLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",   CCLabelTTF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lv",     CCLabelTTF*, m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "power",  CCLabelTTF*, m_powerLabel);
    return false;
}

void SingleGuildRequestLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    // 添加一个checkBox
    CCSpriteFrame* norFrame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox(CCCheckBox::create(norFrame, selFrame, checkbox_selector(SingleGuildRequestLayer::onClickCheckBox), this, kCCLayerTouchPriority));
    m_checkBox->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox->setPosition(ccp(m_chooseLayer->getContentSize().width / 2, m_chooseLayer->getContentSize().height / 2));
    m_chooseLayer->addChild(m_checkBox);
}

void SingleGuildRequestLayer::onClickCheckBox(bool status)
{
    if (m_parentLayer == NULL)
    {
        return;
    }

    m_parentLayer->setClickState(m_index, status);
}

void SingleGuildRequestLayer::InitLayer(unsigned int index, GuildRequestLayer* parentLayer, GuildMemberInfo info)
{
    m_parentLayer = parentLayer;
    m_index = index;

    bool state = m_parentLayer->getClickState(index);
    m_checkBox->setClickState(state);

    // 初始化界面
    m_nameLabel->setString(info.name.c_str());
    m_lvLabel->setString(CCString::createWithFormat("Lv %d", info.lv)->getCString());
    m_powerLabel->setString(CCString::createWithFormat("%d", info.power_score)->getCString());
}

GuildRequestSettingLayer::GuildRequestSettingLayer() :
    m_plus_10Button(NULL),
    m_plus_1Button(NULL),
    m_add_10Button(NULL),
    m_add_1Button(NULL),
    m_confirmButton(NULL),
    m_refuseButton(NULL),
    m_backLayer(NULL),
    m_powerLabel(NULL),
    m_power(1),
    m_checkBox1(NULL),
    m_checkBox2(NULL),
    m_checkBox3(NULL),
    m_checkLayer1(NULL),
    m_checkLayer2(NULL),
    m_checkLayer3(NULL),
    m_selectType(0)
{

}

GuildRequestSettingLayer::~GuildRequestSettingLayer()
{
    setPlus_10_Button(NULL);
    setPlus_1_Button(NULL);
    setAdd_10_Button(NULL);
    setAdd_1_Button(NULL);
    setConfirmButton(NULL);
    setRefuseButton(NULL);
    setBackLayer(NULL);
    setPowerLabel(NULL);
    setCheckBox1(NULL);
    setCheckBox2(NULL);
    setCheckBox3(NULL);
    setCheckLayer1(NULL);
    setCheckLayer2(NULL);
    setCheckLayer3(NULL);
}

SEL_MenuHandler GuildRequestSettingLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildRequestSettingLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "confirm", GuildRequestSettingLayer::onConfirm);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "refuse",  GuildRequestSettingLayer::onRefuse);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "plus_10", GuildRequestSettingLayer::onPlus10);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "plus_1",  GuildRequestSettingLayer::onPlus1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "add_10",  GuildRequestSettingLayer::onAdd10);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "add_1",   GuildRequestSettingLayer::onAdd1);
    return NULL;
}

bool GuildRequestSettingLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "plus_10_button",  CCControlButton*, m_plus_10Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "plus_1_button",   CCControlButton*, m_plus_1Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "add_10_button",   CCControlButton*, m_add_10Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "add_1_button",    CCControlButton*, m_add_1Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "confirm_button",  CCControlButton*, m_confirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refuse_button",   CCControlButton*, m_refuseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back_layer",      CCLayer*,         m_backLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "power",           CCLabelTTF*,      m_powerLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "checkbox_layer1", CCLayer*,         m_checkLayer1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "checkbox_layer2", CCLayer*,         m_checkLayer2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "checkbox_layer3", CCLayer*,         m_checkLayer3);
    return false;
}

void GuildRequestSettingLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    // for backlayer
    m_backLayer->setContentSize(s);
    m_backLayer->setScale(m_backLayer->getScale() / scale);

    m_power = 1;
    RefreshPowerLabel();

    // 添加三个checkBox
    CCSpriteFrame* norFrame1 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame1 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox1(CCCheckBox::create(norFrame1, selFrame1, checkbox_selector(GuildRequestSettingLayer::onClickCheckBox1), this, kCCLayerTouchPriority));
    m_checkBox1->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox1->setPosition(ccp(m_checkLayer1->getContentSize().width / 2, m_checkLayer1->getContentSize().height / 2));
    m_checkLayer1->addChild(m_checkBox1);

    CCSpriteFrame* norFrame2 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame2 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox2(CCCheckBox::create(norFrame2, selFrame2, checkbox_selector(GuildRequestSettingLayer::onClickCheckBox2), this, kCCLayerTouchPriority));
    m_checkBox2->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox2->setPosition(ccp(m_checkLayer2->getContentSize().width / 2, m_checkLayer2->getContentSize().height / 2));
    m_checkLayer2->addChild(m_checkBox2);

    CCSpriteFrame* norFrame3 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_off.png");
    CCSpriteFrame* selFrame3 = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "guild_check_on.png");

    setCheckBox3(CCCheckBox::create(norFrame3, selFrame3, checkbox_selector(GuildRequestSettingLayer::onClickCheckBox3), this, kCCLayerTouchPriority));
    m_checkBox3->setAnchorPoint(ccp(0.5, 0.5));
    m_checkBox3->setPosition(ccp(m_checkLayer3->getContentSize().width / 2, m_checkLayer3->getContentSize().height / 2));
    m_checkLayer3->addChild(m_checkBox3);


    // set Old setting status
    CSQueryPlayerGuildInfoRsp info = GuildManager::Get()->getPlayerOwnerGuildInfo();
    int joinType = info.guildinfo().join_type();
    if(joinType == 1)
    {
        m_checkBox1->setClickState(true);
    }
    else if(joinType == 3)
    {
        m_checkBox2->setClickState(true);
    }
    else if(joinType == 2)
    {
        m_checkBox3->setClickState(true);
        m_power = info.guildinfo().fightcondition() / 1000;
        if(m_power == 0)
        {
            m_power = 1;
        }
        m_powerLabel->setString(CCString::createWithFormat("%d K", m_power)->getCString());
    }
}

bool GuildRequestSettingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRequestSettingLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildRequestSettingLayer::onEnter()
{
    CCLayer::onEnter();
    
    m_plus_10Button->setTouchPriority(kCCLayerTouchPriority);
    m_plus_1Button->setTouchPriority(kCCLayerTouchPriority);
    m_add_10Button->setTouchPriority(kCCLayerTouchPriority);
    m_add_1Button->setTouchPriority(kCCLayerTouchPriority);
    m_confirmButton->setTouchPriority(kCCLayerTouchPriority);
    m_refuseButton->setTouchPriority(kCCLayerTouchPriority);
}
 
void GuildRequestSettingLayer::onConfirm(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildRequestLayer(true);
        mLayer->ShowGuildRequestSettingLayer(false);
    }

    if(m_selectType == 0)
    {
        return;
    }
    GuildManager::Get()->SenderGuildLApplySettting(m_selectType, m_power * 1000);

    ShowMessage(Localizatioin::getLocalization("G_CONDITION_TEXT4"));
}

void GuildRequestSettingLayer::onRefuse(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildRequestLayer(true);
        mLayer->ShowGuildRequestSettingLayer(false);
    }
}

void GuildRequestSettingLayer::onPlus10(CCObject* object, CCControlEvent event)
{
    if (m_power > 10)
    {
        m_power -= 10;
    } else
    {
        m_power = 0;
    }

    if(m_power == 0)
    {
        m_power = 1;
    }

    RefreshPowerLabel();
}

void GuildRequestSettingLayer::onPlus1(CCObject* objecet, CCControlEvent event)
{
    if (m_power > 1)
    {
        m_power -= 1;
    } else
    {
        m_power = 0;
    }

    if(m_power == 0)
    {
        m_power = 1;
    }

    RefreshPowerLabel();
}

void GuildRequestSettingLayer::onAdd10(CCObject* object, CCControlEvent event)
{
    m_power += 10;

    RefreshPowerLabel();
}

void GuildRequestSettingLayer::onAdd1(CCObject* object, CCControlEvent event)
{
    m_power += 1;

    RefreshPowerLabel();
}

void GuildRequestSettingLayer::RefreshPowerLabel()
{
    m_powerLabel->setString(CCString::createWithFormat("%d K", m_power)->getCString());
}

void GuildRequestSettingLayer::onClickCheckBox1(bool flag)
{
    if (flag)
    {
        m_selectType = 1;
        m_checkBox2->setClickState(false);
        m_checkBox3->setClickState(false);
    } else
    {
        if(m_selectType == 1)
        {
            m_checkBox1->setClickState(true);
            return;
        }
    }
}

void GuildRequestSettingLayer::onClickCheckBox2(bool flag)
{
    if (flag)
    {
        m_selectType = 3;
        m_checkBox1->setClickState(false);
        m_checkBox3->setClickState(false);
    } else
    {
        if(m_selectType == 3)
        {
            m_checkBox2->setClickState(true);
            return;
        }
    }
}

void GuildRequestSettingLayer::onClickCheckBox3(bool flag)
{
    if (flag)
    {
        m_selectType = 2;
        m_checkBox1->setClickState(false);
        m_checkBox2->setClickState(false);
    } else
    {
        if(m_selectType == 2)
        {
            m_checkBox3->setClickState(true);
            return;
        }
    }
}

GuildBlessLayer::GuildBlessLayer() :
    m_currentLife(NULL),
    m_nextLife(NULL),
    m_currentPres(NULL),
    m_nextPres(NULL),
    m_currentMoney(NULL),
    m_nextMoney(NULL),
    m_closeButton(NULL),
    m_blessButton(NULL),
    m_backLayer(NULL),
    m_currentStep(NULL),
    m_nextNeed(NULL),
    m_costDiamond(NULL),
    m_restTimes(NULL),
    m_pSpriteMaterial(NULL),
    m_pTitleLabel(NULL),
    m_pLabel1(NULL),    
    m_pLabel2(NULL),    
    m_pLabel3(NULL),    
    m_pLabel4(NULL),    
    m_pLabel5(NULL),    
    m_pLabel6(NULL),    
    m_pLabel7(NULL),
    m_pCurContribute(NULL),
    m_pCurDiamond(NULL),
    m_pEffectLayer(NULL)
{

}

GuildBlessLayer::~GuildBlessLayer()
{
    setCurrentLife(NULL);
    setNextLife(NULL);
    setCurrentPres(NULL);
    setNextPres(NULL);
    setCurrentMoney(NULL);
    setNextMoney(NULL);
    setCloseButton(NULL);
    setBlessButton(NULL);
    setBackLayer(NULL);
    setCurrentStep(NULL);
    setNextNeed(NULL);
    setCostDiamond(NULL);
    setRestTimes(NULL);
    setSpriteMaterial(NULL);
    setTitleLabel(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
    setLabel4(NULL);
    setLabel5(NULL);
    setLabel6(NULL);
    setLabel7(NULL);
    setCurDiamond(NULL);
    setEffectLayer(NULL);
    setCurContribute(NULL);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GuildBlessInfo_update");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "update_guild_blesstimes");
}

SEL_MenuHandler GuildBlessLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildBlessLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", GuildBlessLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "get", GuildBlessLayer::onBlessClicked);
    return NULL;
}

bool GuildBlessLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",     CCControlButton*, m_closeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_button",    CCControlButton*, m_blessButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "current_life",  CCLabelTTF*, m_currentLife);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_life",     CCLabelTTF*, m_nextLife);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "current_pres",  CCLabelTTF*, m_currentPres);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_pres",     CCLabelTTF*, m_nextPres);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "current_money", CCLabelTTF*, m_currentMoney);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_money",    CCLabelTTF*, m_nextMoney);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back_layer",    CCLayer*,    m_backLayer);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "current_step",  CCLabelTTF*, m_currentStep);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_need",     CCLabelTTF*, m_nextNeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_diamond",  CCLabelTTF*, m_costDiamond);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rest_times",    CCLabelTTF*, m_restTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteMaterial",    CCLayer*, m_pSpriteMaterial);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleLabel",   CCLabelTTF*, m_pTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label1",          CCLabelTTF*, m_pLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label2",          CCLabelTTF*, m_pLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label3",          CCLabelTTF*, m_pLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label4",          CCLabelTTF*, m_pLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label5",          CCLabelTTF*, m_pLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label6",          CCLabelTTF*, m_pLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label7",          CCLabelTTF*, m_pLabel7);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurDiamond",          CCLabelTTF*, m_pCurDiamond);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurContribute",          CCLabelTTF*, m_pCurContribute);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectLayer",         CCLayer*, m_pEffectLayer);
    return true;
}

void GuildBlessLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    float scaleX = UIManager::sharedManager()->getScaleFactorX();
    float scaleY = UIManager::sharedManager()->getScaleFactorY();

    // for backlayer
    m_backLayer->setContentSize(s);
    m_backLayer->setScale(m_backLayer->getScale() / scale);

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                            callfuncO_selector(GuildBlessLayer::onMsgCallBack), "GuildBlessInfo_update", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                            callfuncO_selector(GuildBlessLayer::onMsgTimesCallBack), "update_guild_blesstimes", NULL);
    GuildManager::Get()->SendGetGuildBlessInfo(true);

    // init mutilanguge
    m_pTitleLabel->setString(Localizatioin::getLocalization("G_BENEDICTION_TITLE"));
    m_pLabel1->setString(Localizatioin::getLocalization("G_BENEDICTION_NOW"));
    m_pLabel2->setString(Localizatioin::getLocalization("G_BENEDICTION_GET"));
    m_pLabel3->setString(Localizatioin::getLocalization("G_BENEDICTION_NEXT"));
    m_pLabel4->setString(Localizatioin::getLocalization("G_BENEDICTION_GET"));
    m_pLabel5->setString(Localizatioin::getLocalization("G_BENEDICTION_COST"));
    m_pLabel6->setString(Localizatioin::getLocalization("G_BENEDICTION_NUMBER"));
    m_pLabel7->setString(Localizatioin::getLocalization("G_NOW"));

    onMsgTimesCallBack(NULL);
}

void GuildBlessLayer::onMsgCallBack(CCObject* obj)
{
    InitBlessLayer();
}

void GuildBlessLayer::onMsgTimesCallBack(CCObject* obj)
{
    int retTimes = UserData::GetUserInfo().m_guild_bless_times;
    int retTimesSub = 2 - retTimes;
    if(retTimesSub < 0)
    {
        retTimesSub = 0;
    }
    CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_NUMBER"), retTimesSub);
    m_restTimes->setString(str->getCString());
}
 
void GuildBlessLayer::InitBlessLayer()
{
    // cur diamond glv new lv
    m_pCurDiamond->setString(IntToStr(UserData::GetDiamond()));
    m_pCurContribute->setString(IntToStr(UserData::GetUserInfo().m_guild_contributes));
    CSQueryPlayerGuildInfoRsp playerGuildInfo = GuildManager::Get()->getPlayerOwnerGuildInfo();
    int glv = playerGuildInfo.guildinfo().lv();
    CCString* guildStr = CCString::createWithFormat(Localizatioin::getLocalization("G_BENEDICTION_LEVER"),glv);
    m_currentStep->setString(guildStr->getCString());
   
    // data
    std::vector<GuildBlessInfo> blessInfo = GuildManager::Get()->getGuildBlessInfo();
    if(blessInfo.size() == 0)
    {
        return;
    }

    CCString* guildnextStr = CCString::createWithFormat(Localizatioin::getLocalization("G_BENEDICTION_LEVER"),blessInfo[0].nextLevel);
    m_nextNeed->setString(guildnextStr->getCString());

    // tips
    int blessTiems = UserData::GetUserInfo().m_guild_bless_times;
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    int tag = mLayer->getLayerTagByName("GuildInfoLayer");
    if(dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag)))
    {
        if(blessTiems > 0)
        {
            dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag))->showBlessTIp(false);
        }
        else
        {
            dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag))->showBlessTIp(true);
        }
    }

    int contributeTimes = UserData::GetUserInfo().m_guild_contribute_times;
    int blessTiemss = UserData::GetUserInfo().m_guild_bless_times;
    int requestListnum = GuildManager::Get()->getRequestMemberList().size();
    if((1 - contributeTimes == 0) && (2 - blessTiemss == 0) && requestListnum == 0 && !GuildManager::Get()->getIsHasRedEnvo())
    {
        GuildManager::Get()->HideMainTipIcon();
    }
    else
    {
        GuildManager::Get()->ShowMainTipIcon();
    }

    std::vector< std::vector<int> > fromItem = blessInfo[0].fromItems;
    if(blessInfo[0].fromItems.size() != 0)
    {
        int fromItemId = fromItem[0][0];
        int fromItemAmount = fromItem[0][1];
        CCSprite* fromIcon = InterfaceManager::Get()->getIconByItemid(fromItemId);
        m_pSpriteMaterial->removeAllChildren();
        m_pSpriteMaterial->addChild(fromIcon);
        fromIcon->setPosition(ccp(m_pSpriteMaterial->getContentSize().width / 2, m_pSpriteMaterial->getContentSize().height / 2));
        m_costDiamond->setString(IntToStr(fromItemAmount));
    }

    // to item 
    if(blessInfo[0].toItems.size() != 0)
    {
        m_currentLife->setString(IntToStr(blessInfo[0].toItems[0][1]));
        m_currentPres->setString(IntToStr(blessInfo[0].toItems[1][1]));
        m_currentMoney->setString(IntToStr(blessInfo[0].toItems[2][1]));
    }    

    // effect
    if(blessInfo[0].res == 0)
    {
        PlayerEffect();
        ShowMessage(Localizatioin::getLocalization("G_BENEDICTION_SUCCESS"));

        std::vector<std::string> strVec;
        strVec.clear();
        strVec.push_back(Localizatioin::getLocalization("G_CONTRIBUTION"));
        strVec.push_back(Localizatioin::getLocalization("G_GOLD"));
        strVec.push_back(Localizatioin::getLocalization("Name_416000"));
        for(int i = 0; i < 3; i++)
        {
            int itemCount = blessInfo[0].toItems[i][1];
            std::string itemName = strVec[i];
            std::string tempStr = Localizatioin::getLocalization("M_PK_INTRODUCTION_2");
            tempStr.append(" ").append(itemName.c_str()).append(" x ").append(IntToStr(itemCount));
            ShowMessage(tempStr.c_str());
        }
    }

    // to next item
    if(blessInfo[0].toNextItems.size() == 0)
    {
        m_nextLife->setString("-");
        m_nextPres->setString("-");
        m_nextMoney->setString("-");
        m_nextNeed->setString(Localizatioin::getLocalization("G_BENEDICTION_NOT"));
    }
    else
    {
        m_nextLife->setString(IntToStr(blessInfo[0].toNextItems[0][1]));
        m_nextPres->setString(IntToStr(blessInfo[0].toNextItems[1][1]));
        m_nextMoney->setString(IntToStr(blessInfo[0].toNextItems[2][1]));
    }
}

bool GuildBlessLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildBlessLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildBlessLayer::onEnter()
{
    CCLayer::onEnter();

    m_closeButton->setTouchPriority(kCCLayerTouchPriority);
    m_blessButton->setTouchPriority(kCCLayerTouchPriority);
}

void GuildBlessLayer::onCloseLayer(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildBlessLayer(false);
    }
}

void GuildBlessLayer::onBlessClicked(CCObject* object, CCControlEvent event)
{
    GuildManager::Get()->SendGetGuildBlessInfo(false);    
}

void GuildBlessLayer::PlayerEffect()
{
    m_pEffectLayer->removeAllChildren();
    SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(450);
    EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(450,"",false);
    effect1->setPosition(ccp(m_pEffectLayer->getContentSize().width * 0.5f, m_pEffectLayer->getContentSize().height * 0.5f));
    effect1->SetAnim(kType_Play,1,false);
    m_pEffectLayer->addChild(effect1);
    GameAudioManager::sharedManager()->playEffect(320000, false);
}

int GuildInstanceListLayer::guildInstancePlayTimes = 0;
GuildUnionInfo GuildInstanceListLayer::guildUnionInfo;
GuildInstanceListLayer::GuildInstanceListLayer():
m_pLayer(NULL)
{

}

GuildInstanceListLayer::~GuildInstanceListLayer()
{
	UIManager::sharedManager()->RemoveUILayout("guildInstanceUI");
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetGuildUnionInfo");
}

bool GuildInstanceListLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/guildInstanceUI.plist", this, "guildInstanceUI", true);
	manager->RegisterMenuHandler("guildInstanceUI","closeInstanceList",	menu_selector(GuildInstanceListLayer::onClose), this);
	//manager->RegisterMenuHandler("ppveUI","PPVELayer::OnClickListItem",	menu_selector(PPVELayer::OnClickListItem), this);	

	UILabel *hintLabel = m_pLayer->FindChildObjectByName<UILabel>("hintLabel");
	hintLabel->setString(Localizatioin::getLocalization("M_GUILD_FUBEN_TIPS"));

	InstanceManager::Get()->UpdateDataForGuildInstance();
	MapInfoVector& map = InstanceManager::Get()->getGuildInstances();
	m_vecMaps.clear();

	std::vector<int> vecids;
	for(size_t i=0; i<map.size(); i++)
	{
		vecids.push_back(map[i].id);
	}
	std::sort(vecids.begin(), vecids.end());

	for(size_t i=0;i<vecids.size();i++)
	{
		int id = vecids[i];
		for(size_t j=0; j<map.size(); j++)
		{
			if(id == map[j].id)
			{
				m_vecMaps.push_back(map[j]);
				continue;
			}
		}
	}

	GuildManager::Get()->SendGetGuildUnionInfoReq();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuildInstanceListLayer::onGetGuildUnionInfoCallBack), "GetGuildUnionInfo", NULL);

	return true;
}

int GuildInstanceListLayer::getCouldPlayTimes(){

	int couldPlayTime = 0;
	int idx = GuildManager::Get()->getPlayerOwnerGuildInfo().playerinfo().status();
	switch (idx)
	{
	case 1:
		couldPlayTime = 2 - guildInstancePlayTimes;
		break;
	case 2:
	case 3:
	case 4:
		couldPlayTime = 3 - guildInstancePlayTimes;
		break;
	default:
		break;
	}

	return couldPlayTime;
}

void GuildInstanceListLayer::onGetGuildUnionInfoCallBack(CCObject* obj)
{
	InitMaps();
}

void GuildInstanceListLayer::InitMaps()
{
	int guildLv = GuildManager::Get()->getPlayerOwnerGuildInfo().guildinfo().lv();

	std::vector<GuildUnionInfo> vecInfos = GuildManager::Get()->getUnionInfo();
	for(int i=0; i< vecInfos.size(); i++){
		if(guildLv == vecInfos[i].level){
			guildUnionInfo = vecInfos[i];
			break;
		}
	}

	int count = m_vecMaps.size();
	ScrollPagePosition position;
	for(int i=0;i < count;i++)
	{
		position = getScrollPagePos(i);
		addSingleInstance(m_vecMaps[i], position);

		CCLOG("vec maps level:%d\n", m_vecMaps[i].MinLevel);
	}

	UIManager *manager = UIManager::sharedManager();
	UIScrollPage *page = m_pLayer->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
	page->moveToPage(0);
}

void GuildInstanceListLayer::addSingleInstance(InstanceMapInfo info, ScrollPagePosition position)
{
	UIManager *manager = UIManager::sharedManager();
	if (!m_pLayer)	
		return;

	UIScrollPage *page = m_pLayer->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");

	page->setScale(manager->getScaleFactor());

	//创建足够的页数
	while(position.page >= page->getPageCount())
	{
		CCLayer* layer = CCLayer::create();
		layer->setAnchorPoint(ccp(0, 0));		
		page->addPage(layer);
	}

	CCLayer * layer = page->getPageLayer(position.page);
	CCNode * insNode = layer->getChildByTag(position.index);

	//读取资源
	ASprite *as = AspriteManager::getInstance()->getAsprite(KPPVE_BIN);

	std::string instanceConfigName = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "Icon");
	std::transform(instanceConfigName.begin(), instanceConfigName.end(), instanceConfigName.begin(), towupper);
	instanceConfigName = std::string("map_ui_system_icon_FRAME_") + instanceConfigName;	

	int normalBtnId = getResourceIDByName("map_ui_ppve_FRAME_GMS_BUTTON_INSTANCE");
	int selectBtnId = getResourceIDByName("map_ui_ppve_FRAME_GMS_BUTTON_INSTANCE_CLICKED");
	int instanceLevelId = getResourceIDByName("map_ui_ppve_FRAME_GMS_TEXT_INSTANCELEVEL");
	int instanceNameId = getResourceIDByName("map_ui_ppve_FRAME_GMS_TEXT_INSTANCENAME");
	int instanceIconId = getResourceIDByName(instanceConfigName.c_str());

	CCPoint ptNormalBtn,ptSelectBtn,ptLevelText,ptNameText,ptIcon;
	CCSprite *pNormalBtn = as->getSpriteFromFrame_Middle(normalBtnId, 0, ptNormalBtn);
	CCSprite *pNormalBtn2 = as->getSpriteFromFrame_Middle(normalBtnId, 0, ptNormalBtn);
	CCSprite *pSelectBtn = as->getSpriteFromFrame_Middle(selectBtnId, 0, ptSelectBtn);
	CCSprite *pLevelText = as->getSpriteFromFrame_Middle(instanceLevelId, 0, ptLevelText);
	CCSprite *pNameText = as->getSpriteFromFrame_Middle(instanceNameId, 0, ptNameText);

	//创建副本按钮
	IconButton * pBtn = new IconButton(pNormalBtn, NULL, pSelectBtn, this, 
		menu_selector(GuildInstanceListLayer::onPressInstanceBtn),-130);					
	pBtn->autorelease();		
	pBtn->setContentSize(pNormalBtn->getContentSize());
	pBtn->setPosition(ccp(0,0));
	pBtn->SetButtonIndex(position.index);

	InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
	GameConfigFromLuaManager::Get()->getInstanceBonusInfo(info.id, bonusInfo);

	float fontSize = 20;
	CCSize nameLabelSize = CCSizeMake(200 / CC_CONTENT_SCALE_FACTOR(), fontSize * 1.5f);

	//创建副本图标
	CCPoint pt;
	CCSprite *pIcon = AspriteManager::getInstance()->getAsprite(KICON_BIN)->getSpriteFromFrame_Middle(instanceIconId, 0, pt);
	pIcon->setPosition(ccp(0,0));

	//创建副本名称标签
	const char * instanceText = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "RaidName");	
	string instanceName = Localizatioin::getLocalization(instanceText);

	CCLabelTTF * nameLabel = CCLabelTTF::create(instanceName.c_str(), KJLinXin, fontSize,
		nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom);
	nameLabel->setPosition(ccpSub(ptLevelText, ptNormalBtn));

	//创建副本推荐战斗力
	CCLabelTTF * powerLabel;
	if(info.id <= guildUnionInfo.transcript){
		powerLabel = CCLabelTTF::create(CCString::createWithFormat(Localizatioin::getLocalization("M_GUILD_FUBEN_POWER"), bonusInfo->command_power)->getCString(), KJLinXin, fontSize,
			nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom);
		powerLabel->setColor(ccc3(255,184,0));
		nameLabel->setColor(ccc3(255,184,0));
	}else{
		powerLabel = CCLabelTTF::create(Localizatioin::getLocalization("M_GUILD_FUBEN_CLOSE"), KJLinXin, fontSize,
			nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom);
		powerLabel->setColor(ccc3(255,84,0));
		nameLabel->setColor(ccc3(255,84,0));

		//pIcon = graylightWithCCSprite(pIcon, false);
	}

	powerLabel->setPosition(ccpSub(ptNameText, ptNormalBtn));

	delete bonusInfo;


	//创建副本结点
	CCNode * instanceNode = CCNode::create();

	instanceNode->setTag(position.index);	
	instanceNode->setPosition(scrollPosToPoint(position));

	instanceNode->addChild(pIcon);
	instanceNode->addChild(pNormalBtn2);
	instanceNode->addChild(pBtn);	
	instanceNode->addChild(powerLabel);	
	instanceNode->addChild(nameLabel);

	layer->addChild(instanceNode);	
}

void GuildInstanceListLayer::onPressInstanceBtn(CCObject *sender)
{	
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);

	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		CCMenu* menu = dynamic_cast<CCMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
				int i = button->GetButtonIndex();
				if (i < (int)m_vecMaps.size())
				{
					
					InstanceManager::Get()->setCurInstanceId(m_vecMaps[i].id);

					MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
					if (mLayer)
					{
						mLayer->ShowGuildInstanceHelpLayer(true);
						//mLayer->ShowGuildInstanceListLayer(false);
					}
				}
			}
		}
	}
}

GuildInstanceListLayer::ScrollPagePosition GuildInstanceListLayer::getScrollPagePos(int index)
{
	ScrollPagePosition spPos;
	spPos.index = index;
	spPos.page = index / (2 * 4);
	spPos.row  = index % (2 * 4) / 4;
	spPos.col  = index % (2 * 4) % 4;

	return spPos;
}

CCPoint GuildInstanceListLayer::scrollPosToPoint(ScrollPagePosition position)
{	
	float halfH = 120.0f;
	float halfV = 120.0f;

	float offsetH = 0.0f;
	float offsetV = 40.0f;
	float HDelta, VDelta;

	HDelta =   halfH * (2 * position.col + 1 - 4) + offsetH;
	VDelta = - halfV * (2 * position.row + 1 - 2) + offsetV;

	return ccp(HDelta, VDelta);
}

void GuildInstanceListLayer::onClose(CCObject* sender)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowGuildInstanceListLayer(false);
		/*if (LevelManager::sShareInstance()->getCurMapId() != InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE)){
			mLayer->ShowGuildInfoLayer(true);
		}*/
	}
}

void GuildInstanceListLayer::closeLayerCallBack(void)
{
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			mLayer->ShowGuildInstanceListLayer(false,false);
		}
	}
}


#define CHOOSENODE_CLICK 10100
GuildInstanceHelpLayer::GuildInstanceHelpLayer():
m_pLayer(NULL),
m_pSelectedChoose(NULL)
{

}

GuildInstanceHelpLayer::~GuildInstanceHelpLayer()
{
	m_pSelectedChoose = NULL;
	UIManager::sharedManager()->RemoveUILayout("guildInstanceHelpUI");
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetGuildChooseInfo");
}

bool GuildInstanceHelpLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/guildInstanceHelpUI.plist", this, "guildInstanceHelpUI", true);
	manager->RegisterMenuHandler("guildInstanceHelpUI","closeInstanceHelp",	menu_selector(GuildInstanceHelpLayer::onClose), this);
	manager->RegisterMenuHandler("guildInstanceHelpUI","startOneself",	menu_selector(GuildInstanceHelpLayer::onStartOneself), this);
	manager->RegisterMenuHandler("guildInstanceHelpUI","startWithHelp",	menu_selector(GuildInstanceHelpLayer::onStartWithHelp), this);

	m_pLayer->FindChildObjectByName<UILabel>("GHELP_REMAINDER")->setString(CCString::createWithFormat("%s%d",Localizatioin::getLocalization("M_HEROTREASURE_REMAINTIME"),GuildInstanceListLayer::getCouldPlayTimes())->getCString());

	this->showOutput();
	this->showCoinAndDiamond();

	UIScrollPage *outputPage = m_pLayer->FindChildObjectByName<UIScrollPage>("OUTPUT_FRAME");
	outputPage->getScrollLayer()->setShowPagesIndicator(false);
	outputPage->setScale(manager->getScaleFactor());

	UIScrollPage *choosePage = m_pLayer->FindChildObjectByName<UIScrollPage>("CHOOSE_FRAME");
	choosePage->setScale(manager->getScaleFactor());
	choosePage->getScrollLayer()->setShowPagesIndicator(false);

	GuildManager::Get()->SendGetGuildChooseInfoReq(InstanceManager::Get()->getCurInstanceId());
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuildInstanceHelpLayer::onGetInstanceChooseInfoCallBack), "GetGuildChooseInfo", NULL);

	return true;
}

void GuildInstanceHelpLayer::showOutput()
{
	int instanceId = InstanceManager::Get()->getCurInstanceId();
	InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
	GameConfigFromLuaManager::Get()->getInstanceBonusInfo(instanceId, bonusInfo);

	//金币
	m_pLayer->FindChildObjectByName<UILabel>("OUTPUT_GOLD_NUMBER")->setString(CCString::createWithFormat("%d", bonusInfo->money)->getCString());

	//贡献
	int id = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/MapMusicTable.lua","getInstanceId", instanceId);
	string contributionsDrop = string(LuaTinkerManager::Get()->getLuaConfig<const char*>("RaidMusic", "Map_Basic", id, "RaidDrops1"));
	int cent =	(contributionsDrop + ";").find_first_of(";");
	string tmp = contributionsDrop.substr(0,cent);
	int begInd = tmp.find_first_of("/") + 1;
	int endInd = tmp.find_last_of("/");
	if(endInd > begInd){
		m_pLayer->FindChildObjectByName<UILabel>("OUTPUT_CONTRIBUTIONS_NUMBER")->setString(tmp.substr(begInd,endInd-begInd).c_str());
	}else{
		m_pLayer->FindChildObjectByName<UILabel>("OUTPUT_CONTRIBUTIONS_NUMBER")->setString("0");
	}

	//掉落
	UIScrollPage *page = m_pLayer->FindChildObjectByName<UIScrollPage>("OUTPUT_FRAME");

	int index = 0;
	ASprite *as = AspriteManager::getInstance()->getAsprite(KPPVE_BIN);
	for(int i = 0;i<bonusInfo->drops.size();i++)
	{
		if(bonusInfo->drops[i] != 416025){
			ScrollPagePosition position = getScrollPagePos(index++, 2, 2);
			while(position.page >= page->getPageCount())
			{
				CCLayer* layer = CCLayer::create();
				layer->setAnchorPoint(ccp(0, 0));		
				page->addPage(layer);
			}

			CCLayer * layer = page->getPageLayer(position.page);
			CCNode * insNode = layer->getChildByTag(position.index);

			CCPoint pt;
			int normalBtnId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_OUTPUT_FRAME1");
			int selectBtnId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_OUTPUT_FRAME1_CLICKED");
			CCSprite *pNormalBtn = as->getSpriteFromFrame_Middle(normalBtnId, 0, pt);
			CCSprite *pSelectBtn = as->getSpriteFromFrame_Middle(selectBtnId ,0, pt);

			IconButton * pBtn = new IconButton(pNormalBtn, NULL, pSelectBtn, this, menu_selector(GuildInstanceHelpLayer::onPressOutputBtn),-136);					
			pBtn->autorelease();		
			pBtn->setContentSize(pNormalBtn->getContentSize());
			pBtn->setPosition(ccp(0,0));
			pBtn->SetButtonIndex(i);

			CCSprite* pIcon = ItemManager::Get()->getIconSpriteById(bonusInfo->drops[i]);
			pIcon->setPosition(ccp(0,0));

			CCNode * outputNode = CCNode::create();
			outputNode->setTag(position.index);	
			outputNode->setPosition(scrollPosToPoint(position, pNormalBtn->getContentSize(), 0.0f, 0.0f, 2, 2));
			outputNode->addChild(pBtn);	
			outputNode->addChild(pIcon);
			layer->addChild(outputNode);
		}
	}

	delete bonusInfo;
}

void GuildInstanceHelpLayer::showCoinAndDiamond()
{
	USERINFO info =  UserData::GetUserInfo();

	char coinNum[100];
	sprintf(coinNum,"%d",(int)info.m_gold);

	char diamondNum[100];
	sprintf(diamondNum,"%d",(int)info.m_diamond);

	m_pLayer->FindChildObjectByName<UILabel>("NOW_GOLD_NUMBER")->setString(coinNum);
	m_pLayer->FindChildObjectByName<UILabel>("NOW_DIAMOND_NUMBER")->setString(diamondNum);
}

void GuildInstanceHelpLayer::onClose(CCObject* sender)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowGuildInstanceHelpLayer(false);
		//mLayer->ShowGuildInstanceListLayer(true);
	}
}

bool GuildInstanceHelpLayer::checkCouldPlay()
{
	CSQueryPlayerGuildInfoRsp playerGuildInfo = GuildManager::Get()->getPlayerOwnerGuildInfo();
	if(InstanceManager::Get()->getCurInstanceId() > GuildInstanceListLayer::guildUnionInfo.transcript)
	{
		std::string str =  Localizatioin::getLocalization("M_GUILD_FUBEN_ERROR1");
		ShowMessage(str.c_str());
		return false;
	}
	else if(GuildInstanceListLayer::getCouldPlayTimes() <= 0)
	{
		std::string str =  Localizatioin::getLocalization("M_GUILD_FUBEN_ERROR2");
		ShowMessage(str.c_str());
		return false;
	}
	return true;
}

void GuildInstanceHelpLayer::onStartOneself(CCObject* sender)
{
	if(checkCouldPlay()){

		bool inGuildScene = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE);
		if(inGuildScene)
		{
			GuildInstanceManager::Get()->setIsEnterFromGuildScene(inGuildScene);
		}

		CSPlayerGuildInfo info;
		GuildManager::Get()->SendCreateGuildInstanceReq(InstanceManager::Get()->getCurInstanceId(),false,info);
	}
}

void GuildInstanceHelpLayer::onStartWithHelp(CCObject* sender)
{
	if(checkCouldPlay()){
		if(m_pSelectedChoose == NULL){
			std::string str =  Localizatioin::getLocalization("M_GUILD_FUBEN_ERROR3");
			ShowMessage(str.c_str());
			return; 
		}

		bool inGuildScene = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE);
		if(inGuildScene)
		{
			GuildInstanceManager::Get()->setIsEnterFromGuildScene(inGuildScene);
		}

		CSPlayerGuildInfo chooseInfo = GuildManager::Get()->getChooseInfo()[m_pSelectedChoose->GetButtonIndex()];
		GuildManager::Get()->SendCreateGuildInstanceReq(InstanceManager::Get()->getCurInstanceId(),true,chooseInfo);
	}
}

void GuildInstanceHelpLayer::onGetInstanceChooseInfoCallBack(CCObject* obj)
{
	InitChoose();
}

void GuildInstanceHelpLayer::InitChoose(){

	std::vector<CSPlayerGuildInfo> vecInfos = GuildManager::Get()->getChooseInfo();

	if(vecInfos.size() > 0){
		m_pLayer->FindChildObjectByName<UILabel>("NOCHOOSE_TEXT")->setVisible(false);

		//读取资源
		ASprite *as = AspriteManager::getInstance()->getAsprite(KPPVE_BIN);
		ASprite *asIcon = AspriteManager::getInstance()->getAsprite(KICON_BIN); 

		int cBgId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_BACKGROUND");
		int cClickId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_CLICKED");
		int cIconId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_ICON");
		int cPowerId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_POWER");
		int cNameId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_NAME");
		int cPostId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_POST");
		int cCostId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_COST");
		int cCostDiamondIconId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_COST_ICON");
		int cCostGoldIconId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_COST_ICON2");
		int cCostTextId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_COST_TEXT");
		int cCostLevelId = getResourceIDByName("map_ui_ppve_FRAME_GHELP_CHOOSE_COST_LEVEL");

		CCPoint ptBg,ptClick,ptIcon,ptPower,ptName,ptPost,ptCost,ptCostDiamondIcon,ptCostGoldIcon,ptCostText,ptCostLevel,ptHeadIcon;
		CCSprite *pIcon = as->getSpriteFromFrame_Middle(cIconId, 0, ptIcon);
		CCSprite *pPower = as->getSpriteFromFrame_Middle(cPowerId, 0, ptPower);
		CCSprite *pName = as->getSpriteFromFrame_Middle(cNameId, 0, ptName);
		CCSprite *pPost = as->getSpriteFromFrame_Middle(cPostId, 0, ptPost);
		CCSprite *pCost = as->getSpriteFromFrame_Middle(cCostId, 0, ptCost);
		CCSprite *pCostText = as->getSpriteFromFrame_Middle(cCostTextId, 0, ptCostText);
		CCSprite *pCostLevel = as->getSpriteFromFrame_Middle(cCostLevelId, 0, ptCostLevel);

		UIScrollPage *page = m_pLayer->FindChildObjectByName<UIScrollPage>("CHOOSE_FRAME");

		CCSize size = page->getContentSize();

		for(int i=0; i< vecInfos.size(); i++)
		{
			CSPlayerGuildInfo info = vecInfos[i];

			CCSprite *pBg = as->getSpriteFromFrame_Middle(cBgId, 0, ptBg);

			ScrollPagePosition position = getScrollPagePos(i, 3, 2);
			while(position.page >= page->getPageCount())
			{
				CCLayer* layer = CCLayer::create();
				layer->setAnchorPoint(ccp(0, 0));		
				page->addPage(layer);
			}

			CCLayer * layer = page->getPageLayer(position.page);
			CCNode * insNode = layer->getChildByTag(position.index);

			IconButton * pBtn = new IconButton(pBg, NULL, NULL, this, 
				menu_selector(GuildInstanceHelpLayer::onPressChooseBtn),-136);					
			pBtn->autorelease();		
			pBtn->setContentSize(pBg->getContentSize());
			pBtn->setPosition(ccp(0,0));
			pBtn->SetButtonIndex(position.index);

			float fontSize = 18;
			ccColor3B fontColor = ccc3(41,25,7);
			CCSize nameLabelSize = CCSizeMake(120 / CC_CONTENT_SCALE_FACTOR(), fontSize * 1.5f);



			CCLabelTTF * powerLabel = CCLabelTTF::create(CCString::createWithFormat("%s:%d",Localizatioin::getLocalization("M_COMBAT"),info.power_score())->getCString(), KJLinXin, fontSize,
				pPower->getContentSize(), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
			powerLabel->setPosition(ccpSub(ptPower, ptBg));
			powerLabel->setColor(fontColor);

			CCLabelTTF * nameLabel = CCLabelTTF::create(info.name().c_str(), KJLinXin, fontSize,
				pName->getContentSize(), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
			nameLabel->setPosition(ccpSub(ptName, ptBg));
			nameLabel->setColor(fontColor);

			CCLabelTTF * postLabel = CCLabelTTF::create(CCString::createWithFormat("%s:%s",Localizatioin::getLocalization("G_MEMBERS_TITLE5"),GuildManager::Get()->getGuildPersonTitle(info.status()).c_str())->getCString(), KJLinXin, fontSize,
				pPost->getContentSize(), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
			postLabel->setPosition(ccpSub(ptPost, ptBg));
			postLabel->setColor(fontColor);

			CCLabelTTF * costLabel = CCLabelTTF::create(Localizatioin::getLocalization("G_COST"), KJLinXin, fontSize,
				pCost->getContentSize(), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
			costLabel->setPosition(ccpSub(ptCost, ptBg));
			costLabel->setColor(fontColor);

			CCLabelTTF * costTextLabel = CCLabelTTF::create("0", KJLinXin, fontSize,
				pCostText->getContentSize(), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
			costTextLabel->setPosition(ccpSub(ptCostText, ptBg));
			costTextLabel->setColor(ccc3(255,255,255));

			CCLabelTTF * costLevelLabel = CCLabelTTF::create(CCString::createWithFormat("LV.%d",info.lv())->getCString(), KJLinXin, fontSize,
				pCostLevel->getContentSize(), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
			costLevelLabel->setPosition(ccpSub(ptCostLevel, ptBg));
			costLevelLabel->setColor(fontColor);

			CCSprite *pClick = as->getSpriteFromFrame_Middle(cClickId, 0, ptClick);
			pClick->setPosition(ccpSub(ptClick,ptBg));
			pClick->setVisible(false);
			pClick->setTag(CHOOSENODE_CLICK);
			pBtn->addChild(pClick,10);

			CCSprite *pCostDiamondIcon = as->getSpriteFromFrame_Middle(cCostDiamondIconId, 0, ptCostDiamondIcon);
			pCostDiamondIcon->setPosition(ccpSub(ptCostDiamondIcon,ptBg));
			CCSprite *pCostGoldIcon = as->getSpriteFromFrame_Middle(cCostGoldIconId, 0, ptCostGoldIcon);
			pCostGoldIcon->setPosition(ccpSub(ptCostGoldIcon,ptBg));

			const char* headIconName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", info.type(), "HeadPicture");
			int idx = getResourceIDByName(headIconName);
			CCSprite * headIcon = asIcon->getSpriteFromFrame_Middle(idx , 0, ptHeadIcon);
			headIcon->setPosition(ccpSub(ptIcon, ptBg));
			headIcon->setScale((pIcon->getContentSize().width - 5.0f) / headIcon->getContentSize().width);

			CCNode * chooseNode = CCNode::create();
			chooseNode->setTag(position.index);	
			chooseNode->setPosition(scrollPosToPoint(position, pBg->getContentSize(), 5.0f, 5.0f, 3, 2));
			chooseNode->addChild(pBtn);

			for(int j = 0; j< info.cost_size(); j++){
				CSPlayerGuildInfo_Item costItem = info.cost(j);
				if( 416001 == costItem.item_id()){
					chooseNode->addChild(pCostGoldIcon);
				}else{
					chooseNode->addChild(pCostDiamondIcon);
				} 
				costTextLabel->setString(CCString::createWithFormat("%d", costItem.item_num())->getCString());
			}

			chooseNode->addChild(powerLabel);
			chooseNode->addChild(nameLabel);
			chooseNode->addChild(postLabel);
			chooseNode->addChild(costLabel);
			chooseNode->addChild(costTextLabel);
			chooseNode->addChild(costLevelLabel);
			chooseNode->addChild(headIcon);
			layer->addChild(chooseNode);
		}
	}
}

GuildInstanceHelpLayer::ScrollPagePosition GuildInstanceHelpLayer::getScrollPagePos(int index, int maxRow, int maxCol)
{
	ScrollPagePosition spPos;
	spPos.index = index;
	spPos.page = index / (maxRow * maxCol);
	spPos.row  = index % (maxRow * maxCol) / maxCol;
	spPos.col  = index % (maxRow * maxCol) % maxCol;

	return spPos;
}

CCPoint GuildInstanceHelpLayer::scrollPosToPoint(ScrollPagePosition position, CCSize size, float offsetH, float offsetW, int maxRow, int maxCol)
{	
	return ccp( (size.width + offsetW) / 2 * (2 * position.col + 1 - maxCol) ,
		-(size.height + offsetH)  / 2 * (2 * position.row + 1 - maxRow));
}

void GuildInstanceHelpLayer::onPressOutputBtn(CCObject *sender)
{	
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);

	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		CCMenu* menu = dynamic_cast<CCMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
				InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
				GameConfigFromLuaManager::Get()->getInstanceBonusInfo(InstanceManager::Get()->getCurInstanceId(), bonusInfo);

				ItemManager::Get()->showItemTipsById(bonusInfo->drops[button->GetButtonIndex()], button->getMenuNode()->getTouchPoint());

				delete bonusInfo;
			}
		}
	}
}

void GuildInstanceHelpLayer::onPressChooseBtn(CCObject *sender)
{	
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);

	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		CCMenu* menu = dynamic_cast<CCMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
				button->getChildByTag(CHOOSENODE_CLICK)->setVisible(true);

				if(m_pSelectedChoose != NULL){
					m_pSelectedChoose->getChildByTag(CHOOSENODE_CLICK)->setVisible(false);
				}

				m_pSelectedChoose = button;
			}
		}
	}

}