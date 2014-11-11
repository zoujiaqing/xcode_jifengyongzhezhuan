#include "OnlineNetworkManager.h"
#include "BatchShoppingLayer.h"
#include "MainMenuLayer.h"
#include "UIManager.h"
#include "Localization.h"
#include "LuaTinkerManager.h"
#include "ItemManager.h"
#include "AspriteManager.h"

BatchShoppingLayer::BatchShoppingLayer() :
    m_plus10Button(NULL),
    m_plus1Button(NULL),
    m_add10Button(NULL),
    m_add1Button(NULL),
    m_confirmButton(NULL),
    m_refuseButton(NULL),
    m_itemBtn(NULL),
    m_itemFrame(NULL),
    m_nameLabel(NULL),
    m_priceLabel(NULL),
    m_totalPrice(NULL),
    m_amountLabel(NULL),
    m_maxTimesLabel(NULL),
    m_currentAmount(0),
    m_type(BatchShoppingType_SpecialMarket)
{

}

BatchShoppingLayer::~BatchShoppingLayer()
{
    setPlus10Button(NULL);
    setPlus1Button(NULL);
    setAdd10Button(NULL);
    setAdd1Button(NULL);
    setConfirmButton(NULL);
    setRefuseButton(NULL);
    setItemBtn(NULL);
    setItemFrame(NULL);
    setNameLabel(NULL);
    setPriceLabel(NULL);
    setTotalPrice(NULL);
    setAmountLabel(NULL);
    setMaxTimesLabel(NULL);
}

SEL_MenuHandler BatchShoppingLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler BatchShoppingLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "confirm", BatchShoppingLayer::onConfirm);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "refuse",  BatchShoppingLayer::onRefuseLayer);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "plus_10", BatchShoppingLayer::onPlus10BtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "plus_1",  BatchShoppingLayer::onPlus1BtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "add_10",  BatchShoppingLayer::onAdd10BtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "add_1",   BatchShoppingLayer::onAdd1BtnClicked);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "click_item", BatchShoppingLayer::onClickItem);

    return NULL;
}

bool BatchShoppingLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "plus_10_btn", CCControlButton*, m_plus10Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "plus_1_btn",  CCControlButton*, m_plus1Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "add_10_btn",  CCControlButton*, m_add10Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "add_1_btn",   CCControlButton*, m_add1Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "confirm_btn", CCControlButton*, m_confirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refuse_btn",  CCControlButton*, m_refuseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_btn",    CCControlButton*, m_itemBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_frame",  CCSprite*,        m_itemFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_name",   CCLabelTTF*,      m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "price",       CCLabelTTF*,      m_priceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "total_price", CCLabelTTF*,      m_totalPrice);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "amount",      CCLabelTTF*,      m_amountLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "max_times",   CCLabelTTF*,      m_maxTimesLabel);
    return true;
}

void BatchShoppingLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);
}

bool BatchShoppingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void BatchShoppingLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerMenuTouchPriority - 1, true);
}

void BatchShoppingLayer::onEnter()
{
    CCLayer::onEnter();

    m_plus10Button->setTouchPriority(kCCLayerMenuTouchPriority - 2);
    m_plus1Button->setTouchPriority(kCCLayerMenuTouchPriority - 2);
    m_add10Button->setTouchPriority(kCCLayerMenuTouchPriority - 2);
    m_add1Button->setTouchPriority(kCCLayerMenuTouchPriority - 2);
    m_confirmButton->setTouchPriority(kCCLayerMenuTouchPriority - 2);
    m_refuseButton->setTouchPriority(kCCLayerMenuTouchPriority - 2);
    m_itemBtn->setTouchPriority(kCCLayerMenuTouchPriority - 2);
}

void BatchShoppingLayer::onConfirm(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer == NULL)
    {
        return;
    }

    int price = 0;
    if (m_marketData.discount_amount == 0)
    {
        price = m_marketData.cost_item_amount;
    } else
    {
        price = m_marketData.discount_amount;
    }

    int totalPrice = price * m_currentAmount;

    if ((int) UserData::GetDiamond() < totalPrice)
    {
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount", totalPrice);
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
        mLayer->ShowBatchShoppingLayer(false);
        return;
    }

    // 发送购买指令
    MarketManager::Get()->setLastShopItemId((int) m_marketData.item_id);
    
    int exchangeId = 0;
    if (m_type == BatchShoppingType_SpecialMarket)
    {
        exchangeId = 31001;
    } else if (m_type == BatchShoppingType_VipMarket)
    {
        exchangeId = 31000;
    }

    unsigned int* p = new unsigned int(2);
    p[0] = (unsigned int)(m_marketData.market_id);
    p[1] = (unsigned int)(m_currentAmount);
    OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(exchangeId, p, 2);
    mLayer->ShowBatchShoppingLayer(false);
}

void BatchShoppingLayer::onRefuseLayer(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowBatchShoppingLayer(false);
    }
}

void BatchShoppingLayer::InitLayer(MarketData mData, BatchShoppingType type)
{
    m_marketData = mData;
    m_type = type;

    // 首先初始化icon
    CCSprite* iconSprite = NULL;
    if (m_marketData.item_id > 100000)
    {
        iconSprite = ItemManager::Get()->getIconSpriteById(m_marketData.item_id);
    } else
    {
        const char* fairyIcon = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID", m_marketData.item_id);
        iconSprite = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin",fairyIcon);
    }

    if (iconSprite != NULL)
    {
        if (m_itemFrame->getParent()->getChildByTag(111))
        {
            m_itemFrame->getParent()->removeChildByTag(111);
        }

        m_itemFrame->getParent()->addChild(iconSprite, 0, 111);
        iconSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        iconSprite->setPosition(m_itemFrame->getPosition());
    }

    // 初始化数量
    CCString* StrCount = CCString::createWithFormat("%d", m_marketData.item_amount);
    CCLabelTTF* countName = CCLabelTTF::create(StrCount->getCString(), "Helvetica", 20.0f);
    if (countName != NULL)
    {
        if (m_itemFrame->getParent()->getChildByTag(112))
        {
            m_itemFrame->getParent()->removeChildByTag(112);
        }

        m_itemFrame->getParent()->addChild(countName, 0, 112);
        countName->setPosition(ccpAdd(m_itemFrame->getPosition(), ccp(40, - 40)));
        countName->setAnchorPoint(ccp(1, 0));
    }

    // 初始化名称
    if (m_marketData.item_id > 100000)
    {
        ItemBase* itemBaseInfo = ItemManager::Get()->getItemInfoById(m_marketData.item_id);
        if (itemBaseInfo)
        {
            m_nameLabel->setString(itemBaseInfo->m_name);
        }
    } else
    {
        const char* fairyName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyNameByID", m_marketData.item_id);
        m_nameLabel->setString(fairyName);
    }

    // 初始化还可以购买的数量
    if (m_marketData.restriction == 0)
    {
        m_maxTimesLabel->setVisible(false);
    } else
    {
        m_maxTimesLabel->setVisible(true);

        // 首先检查已经购买的次数
        int times = MarketManager::Get()->getIndexMarketItemShopTimes(m_marketData.market_id);
        m_maxTimesLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_TODAYBUY"), m_marketData.restriction - times)->getCString());
    }

    // 初始化当前购买的数量
    setCurrentAmount(1);
    m_amountLabel->setString(CCString::createWithFormat("%d", m_currentAmount)->getCString());

    // 初始化单价
    int cost = 0;
    int price = MarketManager::Get()->GetIndexPrice(m_marketData.market_id);
    if (m_marketData.discount_amount == 0 && price == 0)
    {
        cost = m_marketData.cost_item_amount;
    } else
    if (price != 0)
    {
        cost = price;
    } else
    {
        cost = m_marketData.discount_amount;
    }

    m_priceLabel->setString(CCString::createWithFormat("%d", cost)->getCString());

    // 初始化总价
    int totalPrice = cost * m_currentAmount;
    m_totalPrice->setString(CCString::createWithFormat("%d", totalPrice)->getCString());
}

void BatchShoppingLayer::onPlus10BtnClicked(CCObject* object, CCControlEvent event)
{
    RefreshCurrentAmount(m_currentAmount - 10);
}

void BatchShoppingLayer::onPlus1BtnClicked(CCObject* object, CCControlEvent event)
{
    RefreshCurrentAmount(m_currentAmount - 1);
}

void BatchShoppingLayer::onAdd10BtnClicked(CCObject* object, CCControlEvent event)
{
    RefreshCurrentAmount(m_currentAmount + 10);
}

void BatchShoppingLayer::onAdd1BtnClicked(CCObject* object, CCControlEvent event)
{
    RefreshCurrentAmount(m_currentAmount + 1);
}

void BatchShoppingLayer::onClickItem(CCObject* object, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(object);
    if (node == NULL)
    {
        return;
    }

    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(m_marketData.item_id, pt);
}

void BatchShoppingLayer::RefreshCurrentAmount(int amount)
{
    // 首先获取最大数量
    int max_amount = 0;
    if (m_marketData.restriction == 0)
    {
        max_amount = INT_MAX;
    } else
    {
        int times = MarketManager::Get()->getIndexMarketItemShopTimes(m_marketData.market_id);
        max_amount = m_marketData.restriction - times;
    }

    if (amount > max_amount)
    {
        amount = max_amount;
    }

    if (amount < 1)
    {
        amount = 1;
    }

    // 单价
    setCurrentAmount(amount);
    m_amountLabel->setString(CCString::createWithFormat("%d", m_currentAmount)->getCString());

    // 总价
    int price = 0;
    if (m_marketData.discount_amount == 0)
    {
        price = m_marketData.cost_item_amount;
    } else
    {
        price = m_marketData.discount_amount;
    }

    int totalPrice = price * m_currentAmount;
    m_totalPrice->setString(CCString::createWithFormat("%d", totalPrice)->getCString());
}
