#include "OnlineNetworkManager.h"
#include "FirstChargeLayer.h"
#include "ShopItemInfoFromServer.h"
#include "UIDefine.h"
#include "TXGUI.h"
#include "UIManager.h"
#include "MainMenuLayer.h"
#include "PayManager.h"
#include "AspriteManager.h"
#include "LuaTinkerManager.h"
#include "ItemManager.h"
#include "Localization.h"

using namespace TXGUI;

#define WEAPON_BONUS_ID      400690

FirstChargeLayer::FirstChargeLayer() :
    m_closeBtn(NULL),
    m_extraBtn(NULL),
    m_chargeBtn1(NULL),
    m_chargeBtn2(NULL),
    m_chargeBtn3(NULL),
    m_label1(NULL),
    m_label2(NULL),
    m_label3(NULL),
    m_iconBtn(NULL),
    m_getBtn(NULL)
{
    m_vtChargeLayers.clear();
    m_vtChargeLayers.resize(3);
    m_productList.clear();
}

FirstChargeLayer::~FirstChargeLayer()
{
    setCloseBtn(NULL);
    setExtraBtn(NULL);
    setChargeBtn1(NULL);
    setChargeBtn2(NULL);
    setChargeBtn3(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
    setIconBtn(NULL);
    setGetBtn(NULL);
}

SEL_MenuHandler FirstChargeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler FirstChargeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", FirstChargeLayer::onCloseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "extra", FirstChargeLayer::onExtraBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btn_1", FirstChargeLayer::onClickedBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btn_2", FirstChargeLayer::onClickedBtn2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btn_3", FirstChargeLayer::onClickedBtn3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "icon_click", FirstChargeLayer::onIconBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "get",   FirstChargeLayer::onGetBonus);
    return NULL;
}

bool FirstChargeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",  CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "extra_btn",  CCControlButton*, m_extraBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "charge_1",   CCLayer*, m_vtChargeLayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "charge_2",   CCLayer*, m_vtChargeLayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "charge_3",   CCLayer*, m_vtChargeLayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_1",      CCControlButton*, m_chargeBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_2",      CCControlButton*, m_chargeBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_3",      CCControlButton*, m_chargeBtn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label1",     CCLabelTTF*,      m_label1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label2",     CCLabelTTF*,      m_label2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label3",     CCLabelTTF*,      m_label3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon_btn",   CCControlButton*, m_iconBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_btn",    CCControlButton*, m_getBtn);
    return false;
}

void FirstChargeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    setPosition(ccp(s.width / 2, s.height / 2));
    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    m_label1->setString(Localizatioin::getLocalization("M_FIRST_RECHARGE_1"));
    m_label2->setString(Localizatioin::getLocalization("M_FIRST_RECHARGE_2"));
    m_label3->setString(Localizatioin::getLocalization("M_FIRST_RECHARGE_3"));

    // 添加物品
    const char* fairyIcon = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID", 2119);
    CCSprite* iconSprite = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin",fairyIcon);

    iconSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    m_iconBtn->getParent()->addChild(iconSprite);
    iconSprite->setPosition(m_iconBtn->getPosition());

    // 确定按钮状态
    int state = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/UILayer/FirstPayRewardUI.lua", "getFirstPayStatus");
    if (state == DONE_TOPUP_AND_GET_AWARD)
    {
        m_getBtn->setEnabled(false);
    }
}

void FirstChargeLayer::setGetBtnEnable()
{
    m_getBtn->setEnabled(false);
}

bool FirstChargeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void FirstChargeLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
}

void FirstChargeLayer::onEnter()
{
    CCLayer::onEnter();

    m_closeBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_extraBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_chargeBtn1->setTouchPriority(kCCLayerMenuTouchPriority);
    m_chargeBtn2->setTouchPriority(kCCLayerMenuTouchPriority);
    m_chargeBtn3->setTouchPriority(kCCLayerMenuTouchPriority);
    m_iconBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_getBtn->setTouchPriority(kCCLayerMenuTouchPriority);

    ShopItemInfoFromServer::sharedInstance()->PrepardData();
    InitChargeList();
}

void FirstChargeLayer::onCloseBtn(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowFirstChargeLayer(false);
    }
}

void FirstChargeLayer::onExtraBtn(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowFirstChargeLayer(false);
        mLayer->ShowChargeLayer(true);
    }
}

void FirstChargeLayer::onClickedBtn1(CCObject* object, CCControlEvent event)
{
    ClickBtn(0);
}

void FirstChargeLayer::onClickedBtn2(CCObject* object, CCControlEvent event)
{
    ClickBtn(1);
}

void FirstChargeLayer::onClickedBtn3(CCObject* object, CCControlEvent event)
{
    ClickBtn(2);
}

void FirstChargeLayer::onIconBtnClicked(CCObject* object, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(object);
    if (node == NULL)
    {
        return;
    }

    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(2119, pt);
}

void FirstChargeLayer::onGetBonus(CCObject* object, CCControlEvent event)
{
    int state = LuaTinkerManager::Get()->callLuaFunc<int>("Script/UILayer/FirstPayRewardUI.lua", "getFirstPayStatus");
    int awardId = LuaTinkerManager::Get()->callLuaFunc<int>("Script/UILayer/FirstPayRewardUI.lua", "getFirstPayAwardId");
//     if (state != DONE_TOPUP_NOT_GET_AWARD)
//     {
//         ShowMessage(Localizatioin::getLocalization("M_UNFINISH_FRISTCHARGE"));
//         return;
//     }

    OnlineNetworkManager::sShareInstance()->SendGetTopupAwardReq(awardId);
}

void FirstChargeLayer::ClickBtn(unsigned int index)
{
    if (index >= m_productList.size())
    {
        return;
    }

    PayManager::Get()->payForProduct(index, m_productList[index]);
}

void FirstChargeLayer::InitChargeList()
{
    std::map<unsigned int, OneShopItemInfo> totalShopInfo = ShopItemInfoFromServer::sharedInstance()->getShopInfo();
    std::vector<OneShopItemInfo> vtChargeList;
    std::vector<int> vtIndexList;
    vtChargeList.clear();
    vtIndexList.clear();
    m_productList.clear();

    int index = 0;
    for (std::map<unsigned int, OneShopItemInfo>::iterator it = totalShopInfo.begin();
         it != totalShopInfo.end(); ++it)
    {
        if (it->second.is_command == 1)
        {
            vtChargeList.push_back(it->second);
            m_productList.push_back(it->second.product_id);
            vtIndexList.push_back(index);
        }

        index++;
    }

    for (size_t i = 0; i < m_vtChargeLayers.size(); ++i)
    {
        CCLayer* layer = m_vtChargeLayers[i];

        if (i >= vtChargeList.size())
        {
            layer->setVisible(false);
            continue;
        } else
        {
            layer->setVisible(true);
        }

        // 初始化其中的数值
        OneShopItemInfo info = vtChargeList[i];
        if (layer->getChildByTag(1))
        {
            CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(1));
            int giftAmount = info.item_base_num * (((float)info.add_times / 100) - 1);
            label->setString(CCString::createWithFormat("%d", giftAmount)->getCString());
        }

        if (layer->getChildByTag(2))
        {
            CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(2));
            label->setString(CCString::createWithFormat("%.2f%s", ((float) info.product_price) / 100, Localizatioin::getLocalization("M_YUAN"))->getCString());
        }

        if (layer->getChildByTag(3))
        {
            CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(3));
            label->setString(CCString::createWithFormat("%d", info.item_base_num)->getCString());
        }

        // 创建图标
        std::string IconName = CCString::createWithFormat("map_ui_system_icon_FRAME_ICON_DIAMOND%d", vtIndexList[i] + 1)->m_sString;
        CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN, IconName);

        if (layer->getChildByTag(4))
        {
            layer->getChildByTag(4)->addChild(sprite);
            CCSize s = layer->getChildByTag(4)->getContentSize();
            sprite->setAnchorPoint(ccp(0.5f, 0.5f));
            sprite->setPosition(ccp(s.width / 2, s.height / 2));
        }
    }
}
