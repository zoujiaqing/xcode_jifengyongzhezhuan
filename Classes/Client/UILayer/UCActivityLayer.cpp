#include "OnlineNetworkManager.h"
#include "UCActivityLayer.h"
#include "MainMenuLayer.h"
#include "UIManager.h"
#include "AspriteManager.h"
#include "UCActivityManager.h"
#include "ItemManager.h"
#include "Localization.h"
#include "GMessage.h"
#include "LoginManager.h"
#include "md5.h"
#include "curl/curl.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "TmLoginManager.h"
#endif

UCActivityLayer::UCActivityLayer() :
    m_closeBtn(NULL),
    m_bonusButton1(NULL),
    m_bonusButton2(NULL),
    m_bonusButton3(NULL),
    m_openVipButton(NULL),
    m_getButton(NULL),
    m_bonus1(NULL),
    m_bonus2(NULL),
    m_bonus3(NULL),
    m_bonus1_h(NULL),
    m_bonus2_h(NULL),
    m_bonus3_h(NULL),
    m_itemLayer1(NULL),
    m_itemLayer2(NULL),
    m_itemLayer3(NULL),
    m_currentType(0),
    m_itemLayer(NULL),
    m_buttonItem1(NULL),
    m_buttonItem2(NULL),
    m_buttonItem3(NULL),
    m_buttonItem4(NULL),
    m_buttonItem5(NULL),
    m_buttonItem6(NULL),
    m_buttonItem7(NULL), 
    m_label1(NULL),
    m_label2(NULL),
    m_label3(NULL),
    m_label4(NULL)
{
    m_vtBonusItems.clear();
    m_vtBonusItems.resize(7);
}

UCActivityLayer::~UCActivityLayer()
{
    setCloseBtn(NULL);
    setBonusButton1(NULL);
    setBonusButton2(NULL);
    setBonusButton3(NULL);
    setOpenVipButton(NULL);
    setGetButton(NULL);
    setBonusSprite1(NULL);
    setBonusSprite2(NULL);
    setBonusSprite3(NULL);
    setBonusSprite1H(NULL);
    setBonusSprite2H(NULL);
    setBonusSprite3H(NULL);

    setButtonItem1(NULL);
    setButtonItem2(NULL);
    setButtonItem3(NULL);
    setButtonItem4(NULL);
    setButtonItem5(NULL);
    setButtonItem6(NULL);
    setButtonItem7(NULL);

    setItemLayer(NULL);
    setItemLayer1(NULL);
    setItemLayer2(NULL);
    setItemLayer3(NULL);

    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
    setLabel4(NULL);

    for (size_t i = 0; i < m_vtBonusItems.size(); ++i)
    {
        CC_SAFE_RELEASE(m_vtBonusItems[i]);
    }

    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler UCActivityLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler UCActivityLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",         UCActivityLayer::onClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_click_1", UCActivityLayer::onBonusClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_click_2", UCActivityLayer::onBonusClick2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_click_3", UCActivityLayer::onBonusClick3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "open_vip",      UCActivityLayer::onOpenVip);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "get",           UCActivityLayer::onGetBonus);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_1", UCActivityLayer::onItemClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_2", UCActivityLayer::onItemClick2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_3", UCActivityLayer::onItemClick3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_4", UCActivityLayer::onItemClick4);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_5", UCActivityLayer::onItemClick5);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_6", UCActivityLayer::onItemClick6);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "bonus_item_click_7", UCActivityLayer::onItemClick7);
    return NULL;
}

bool UCActivityLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",     CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_1",      CCControlButton*, m_bonusButton1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_2",      CCControlButton*, m_bonusButton2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_3",      CCControlButton*, m_bonusButton3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "open_vip_btn",  CCControlButton*, m_openVipButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_get",       CCControlButton*, m_getButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_1", CCControlButton*, m_buttonItem1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_2", CCControlButton*, m_buttonItem2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_3", CCControlButton*, m_buttonItem3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_4", CCControlButton*, m_buttonItem4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_5", CCControlButton*, m_buttonItem5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_6", CCControlButton*, m_buttonItem6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item_7", CCControlButton*, m_buttonItem7);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_1",      CCSprite*, m_bonus1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_2",      CCSprite*, m_bonus2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_3",      CCSprite*, m_bonus3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_1_h",    CCSprite*, m_bonus1_h);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_2_h",    CCSprite*, m_bonus2_h);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_3_h",    CCSprite*, m_bonus3_h);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_layer_1", CCLayer*,  m_itemLayer1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_layer_2", CCLayer*,  m_itemLayer2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_layer_3", CCLayer*,  m_itemLayer3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_layer",   CCLayer*,  m_itemLayer);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_1", CCSprite*, m_vtBonusItems[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_2", CCSprite*, m_vtBonusItems[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_3", CCSprite*, m_vtBonusItems[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_4", CCSprite*, m_vtBonusItems[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_5", CCSprite*, m_vtBonusItems[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_6", CCSprite*, m_vtBonusItems[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_item_7", CCSprite*, m_vtBonusItems[6]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_1", CCLabelTTF*, m_label1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_2", CCLabelTTF*, m_label2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_3", CCLabelTTF*, m_label3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_4", CCLabelTTF*, m_label4);
    return false;
}

void UCActivityLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    setPosition(ccp(s.width / 2, s.height / 2));
    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    InitBonusItemLayer();
    InitLayer(1);

    // 给label上加文字
    m_label1->setString(Localizatioin::getLocalization("M_9YOU_REWARD1"));
    m_label2->setString(Localizatioin::getLocalization("M_9YOU_REWARD2"));
    m_label3->setString(Localizatioin::getLocalization("M_9YOU_REWARD3"));

    m_label4->setString(Localizatioin::getLocalization("M_9YOU_DESCRIPTION"));

    // 如果玩家不是UC会员，则将本地的sid发给平台主动去进行校验
    USERINFO userInfo = UserData::GetUserInfo();
    if (userInfo.m_91Vip == 0)
    {
        SendOpenUCVip();
    }
}

void UCActivityLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerMenuTouchPriority - 1, true);
}

void UCActivityLayer::onEnter()
{
    CCLayer::onEnter();
    m_closeBtn->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_bonusButton1->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_bonusButton2->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_bonusButton3->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_getButton->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_openVipButton->setTouchPriority(kCCLayerMenuTouchPriority - 1);

    m_buttonItem1->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_buttonItem2->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_buttonItem3->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_buttonItem4->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_buttonItem5->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_buttonItem6->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    m_buttonItem7->setTouchPriority(kCCLayerMenuTouchPriority - 1);
}

bool UCActivityLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void UCActivityLayer::onClose(CCObject* sender, CCControlEvent e)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowPlatActivityLayer(false);
    }
}

void UCActivityLayer::onBonusClick1(CCObject* sender, CCControlEvent e)
{
    InitLayer(1);
}

void UCActivityLayer::onBonusClick2(CCObject* sender, CCControlEvent e)
{
    InitLayer(2);
}

void UCActivityLayer::onBonusClick3(CCObject* sender, CCControlEvent e)
{
    InitLayer(3);
}


void UCActivityLayer::onOpenVip(CCObject* sender, CCControlEvent e)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    TmLoginManager::tmLoginManager()->doAccountManager();
#endif
}

void UCActivityLayer::onGetBonus(CCObject* sender, CCControlEvent e)
{
    // 领取奖励
    int pbId;
    if (m_currentType == 1)
    {
        pbId = PB_ATTR_91_VIP_DAILY_GIFT_GET_TIME;
    } else
    if (m_currentType == 2)
    {
        pbId = PB_ATTR_91_VIP_GIFT_GET_TIME;
    } else
    if (m_currentType == 3)
    {
        pbId = PB_ATTR_91_VIP_TOPUS_STATUS;
    } else
    {
        return;
    }

    OnlineNetworkManager::sShareInstance()->SendGetDailyReward(pbId);
}

void UCActivityLayer::onItemClick1(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(1, sender);
}

void UCActivityLayer::onItemClick2(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(2, sender);
}

void UCActivityLayer::onItemClick3(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(3, sender);
}

void UCActivityLayer::onItemClick4(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(4, sender);
}

void UCActivityLayer::onItemClick5(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(5, sender);
}

void UCActivityLayer::onItemClick6(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(6, sender);
}

void UCActivityLayer::onItemClick7(CCObject* sender, CCControlEvent e)
{
    ShowItemTip(7, sender);
}

void UCActivityLayer::InitBonusItemLayer()
{
    UCAwardData data1 = UCActivityManager::Get()->getIndexUCAwardBonus(1);
    UCAwardData data2 = UCActivityManager::Get()->getIndexUCAwardBonus(2);
    UCAwardData data3 = UCActivityManager::Get()->getIndexUCAwardBonus(3);
    if (data1.id == -1 || data2.id == -1 || data3.id == -3)
    {
        return;
    }

    CCSprite* bonus1 = AspriteManager::getInstance()->getOneFrame(KICON_BIN, "map_ui_system_icon_FRAME_ICON_INVITEREWARD");
    CCSprite* bonus2 = AspriteManager::getInstance()->getOneFrame(KICON_BIN, "map_ui_system_icon_FRAME_ICON_INVITEREWARD");
    CCSprite* bonus3 = AspriteManager::getInstance()->getOneFrame(KICON_BIN, "map_ui_system_icon_FRAME_ICON_INVITEREWARD");

    m_itemLayer1->addChild(bonus1);
    bonus1->setAnchorPoint(ccp(0.5, 0.5));
    bonus1->setPosition(ccp(m_itemLayer1->getContentSize().width / 2, m_itemLayer1->getContentSize().height / 2));

    m_itemLayer2->addChild(bonus2);
    bonus2->setAnchorPoint(ccp(0.5, 0.5));
    bonus2->setPosition(ccp(m_itemLayer2->getContentSize().width / 2, m_itemLayer2->getContentSize().height / 2));

    m_itemLayer3->addChild(bonus3);
    bonus3->setAnchorPoint(ccp(0.5, 0.5));
    bonus3->setPosition(ccp(m_itemLayer3->getContentSize().width / 2, m_itemLayer3->getContentSize().height / 2));
}

void UCActivityLayer::ShowItemTip(int i, CCObject* object)
{
    UCAwardData data = UCActivityManager::Get()->getIndexUCAwardBonus(m_currentType);
    std::vector<UCBonusItemData> bonusList = data.bonusList;
    if ((int) bonusList.size() < i)
    {
        return;
    }

    int itemId = bonusList[i - 1].itemId;
    CCNode* node = dynamic_cast<CCNode*>(object);
    if (node == NULL)
    {
        return;
    }

    ItemManager::Get()->showItemTipsById(itemId, node->convertToWorldSpace(CCPointZero));
}

void UCActivityLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_ATTR_PLAY_UCVIP_REFRESH :
        {
            RefreshLayer();
            break;
        }

    case GM_OPEN_UCVIP :
        {
            SendOpenUCVip();
            break;
        }

    default:
        break;
    }
}

void UCActivityLayer::RefreshLayer()
{
    // 更新按钮状态
    USERINFO userInfo = UserData::GetUserInfo();
    if (userInfo.m_91Vip != 0)
    {
        m_openVipButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_9YOU_BUTTON2")), CCControlStateNormal);
        m_label4->setString(Localizatioin::getLocalization("M_9YOU_DESCRIPTION1"));
    } else
    {
        m_openVipButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_9YOU_BEMEMBER")), CCControlStateNormal);
        m_label4->setString(Localizatioin::getLocalization("M_9YOU_DESCRIPTION"));
    }

    unsigned int getFlag = 0;
    if (m_currentType == 1)
    {
        getFlag = userInfo.m_get91_dailyBonus;
    } else
    if (m_currentType == 2)
    {
        getFlag = userInfo.m_get91_vipBonus;
    } else
    if (m_currentType == 3)
    {
        getFlag = userInfo.m_get91_firstChargeBonus;
    }

    if (getFlag != 0)
    {
        m_getButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_LOGINREWARD_ALREADYGOT")), CCControlStateDisabled);
        m_getButton->setEnabled(false);
    } else
    {
        m_getButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_UNDERGROUND_GETREWARD")), CCControlStateNormal);
        m_getButton->setEnabled(true);
    }
}

void UCActivityLayer::InitLayer(int type)
{
    setCurrentType(type);

    switch (type)
    {
    case 1 :
        m_bonus1->setVisible(false);
        m_bonus1_h->setVisible(true);
        m_bonus2->setVisible(true);
        m_bonus2_h->setVisible(false);
        m_bonus3->setVisible(true);
        m_bonus3_h->setVisible(false);
        break;

    case 2 :
        m_bonus1->setVisible(true);
        m_bonus1_h->setVisible(false);
        m_bonus2->setVisible(false);
        m_bonus2_h->setVisible(true);
        m_bonus3->setVisible(true);
        m_bonus3_h->setVisible(false);
        break;

    case 3 :
        m_bonus1->setVisible(true);
        m_bonus1_h->setVisible(false);
        m_bonus2->setVisible(true);
        m_bonus2_h->setVisible(false);
        m_bonus3->setVisible(false);
        m_bonus3_h->setVisible(true);
        break;

    default:
        break;
    }

    UCAwardData data = UCActivityManager::Get()->getIndexUCAwardBonus(type);
    if (data.id == -1)
    {
        return;
    }

    std::vector<UCBonusItemData> bonusList = data.bonusList;
    for (unsigned int i = 1; i <= 7; ++i)
    {
        if (m_itemLayer->getChildByTag(1000 + i))
            m_itemLayer->removeChildByTag(1000 + i);

        if (m_itemLayer->getChildByTag(1100 + i))
            m_itemLayer->removeChildByTag(1100 + i);
    }

    // 奖励物品图片
    for (unsigned int i = 1; i <=7; ++i)
    {
        if (i > bonusList.size())
            break;

        UCBonusItemData bonusData = bonusList[i - 1];
        CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(bonusData.itemId);
        m_itemLayer->addChild(iconSprite, 0, 1000 + i);
        iconSprite->setAnchorPoint(ccp(0.5, 0.5));
        iconSprite->setPosition(m_vtBonusItems[i - 1]->getPosition());

        CCString* strAmount = CCString::createWithFormat("%d", bonusData.itemAmount);
        CCLabelTTF* nameLabel = CCLabelTTF::create(strAmount->getCString(), "Helvetica", 20.0f);
        nameLabel->setPosition(ccpAdd(m_vtBonusItems[i - 1]->getPosition(), ccp(40, - 40)));
        nameLabel->setAnchorPoint(ccp(1, 0));
        m_itemLayer->addChild(nameLabel, 0, 1100 + i);
    }

    // 决定按钮显示
    USERINFO userInfo = UserData::GetUserInfo();
    if (userInfo.m_91Vip != 0)
    {
        m_openVipButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_9YOU_BUTTON2")), CCControlStateNormal);
        m_label4->setString(Localizatioin::getLocalization("M_9YOU_DESCRIPTION1"));
    } else
    {
        m_openVipButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_9YOU_BEMEMBER")), CCControlStateNormal);
        m_label4->setString(Localizatioin::getLocalization("M_9YOU_DESCRIPTION"));
    }

    unsigned int getFlag = 0;
    if (m_currentType == 1)
    {
        getFlag = userInfo.m_get91_dailyBonus;
    } else
    if (m_currentType == 2)
    {
        getFlag = userInfo.m_get91_vipBonus;
    } else
    if (m_currentType == 3)
    {
        unsigned int canGet = userInfo.m_get91_firstChargeBonus;
        if (canGet == 2)
        {
            getFlag = 1;
        } else
        {
            getFlag = 0;
        }
    }

    if (getFlag != 0)
    {
        m_getButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_LOGINREWARD_ALREADYGOT")), CCControlStateDisabled);
        m_getButton->setEnabled(false);
    } else
    {
        m_getButton->setTitleForState(CCString::create(Localizatioin::getLocalization("M_UNDERGROUND_GETREWARD")), CCControlStateNormal);
        m_getButton->setEnabled(true);
    }
}

void UCActivityLayer::SendOpenUCVip()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // 去平台验证UCVIP
    // http://wlpay.shootao.com/paycheck/mpay/uc_vip.php?game=ahero&sess_id=xxx&user_id=xxx&reg_time=xxx&server_id=xxx&sign=xxxx
    USERINFO mUserData = UserData::GetUserInfo();
    int user_id = mUserData.id.getID();
    int reg_time = mUserData.id.getRegTime();
    int server_id = mUserData.serverId;
    std::string session_id = TmLoginManager::tmLoginManager()->getSession();
    // std::string session_id = "dqidqhdq";
    int game_id = 82;

    std::string strSrc = CCString::createWithFormat("game=ahero&sess_id=%s&user_id=%d&reg_time=%d&server_id=%d&key=db1e56a76833320a60437b0133afee5f", 
        session_id.c_str(), user_id, reg_time, server_id)->m_sString;
    std::string MD5Str = MD5(strSrc.c_str(), strSrc.size()).toString();

    CCString* strUrl = CCString::createWithFormat(
        "http://wlpay.shootao.com/paycheck/mpay/uc_vip.php?game=ahero&sess_id=%s&user_id=%d&reg_time=%d&server_id=%d&sign=%s",
        session_id.c_str(), user_id, reg_time, server_id, MD5Str.c_str());

    CCLog("uc activity url %s", strUrl->getCString());

    CURLcode curlRes;
    CURL* easyCurl = curl_easy_init();
    if (easyCurl == NULL) {
        return;
    }

    curl_easy_setopt(easyCurl,CURLOPT_URL, strUrl->getCString());
    curlRes = curl_easy_perform(easyCurl);  
    curl_easy_cleanup(easyCurl);  
#endif
}
