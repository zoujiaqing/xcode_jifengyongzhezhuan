#include "OnlineNetworkManager.h"
#include "SpecialMarketLayer.h"
#include "MainMenuLayer.h"
#include "BatchShoppingLayer.h"
#include "GameManager.h"
#include "UserData.h"
#include "UIDefine.h"
#include "Localization.h"
#include "ItemManager.h"
#include "MarketManager.h"
#include "AspriteManager.h"
#include "LuaTinkerManager.h"
#include "NotificationCenter.h"
#include "GMessage.h"

SpecialMarketLayer::SpecialMarketLayer() :
    m_diamondLabel(NULL),
    m_shopItemList(NULL),
    m_shopItemLayer(NULL)
{
    m_vtDatas.clear();
}

SpecialMarketLayer::~SpecialMarketLayer()
{
    setDiamondLabel(NULL);
    setShopItemList(NULL);
    setShopListLayer(NULL);
    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler SpecialMarketLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SpecialMarketLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool SpecialMarketLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "diamond",         CCLabelTTF*, m_diamondLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "shop_list_layer", CCLayer*,    m_shopItemLayer);
    return false;
}

void SpecialMarketLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    RefreshShopItemList();
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
    m_diamondLabel->setString(CCString::createWithFormat("%d", UserData::GetDiamond())->getCString());
}

void SpecialMarketLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void SpecialMarketLayer::RefreshShopItemList()
{
    int vip_lv = UserData::GetVipLevel();
    m_vtDatas = MarketManager::Get()->getIndexLevelItemList(19, vip_lv);

    // 请求物品次数
    SendGetShopTimes();
    
    // 请求物品价格
    SendGetShopPrice();

    if (m_shopItemList == NULL)
    {
        setShopItemList(CCTableView::create(this, m_shopItemLayer->getContentSize()));
        m_shopItemList->setDirection(kCCScrollViewDirectionVertical);
        m_shopItemList->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_shopItemLayer->addChild(m_shopItemList);
        m_shopItemList->setTouchPriority(kCCLayerMenuTouchPriority);
        m_shopItemList->setDelegate(this);
    } else
    {
        m_shopItemList->reloadData();
    }
}

CCSize SpecialMarketLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(920, 135);
}

CCTableViewCell* SpecialMarketLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int SpecialMarketLayer::numberOfCellsInTableView(CCTableView *table)
{
    return (m_vtDatas.size () + 1) / 2;
}

void SpecialMarketLayer::RefreshCell(CCTableViewCell* cell, unsigned int idx)
{
    if (! cell->getChildByTag(111))
    {
        CCLayer* firstLayer = CreateSingleSpecMarket(2 * idx);
        cell->addChild(firstLayer, 0, 111);
        firstLayer->setPosition(ccp(5, 0));
    } else
    {
        SingleSpecialMarketLayer* layer = dynamic_cast<SingleSpecialMarketLayer*>(cell->getChildByTag(111));
        if (layer != NULL)
        {
            layer->InitLayer(m_vtDatas[2 * idx]);
        }
    }

    if (! cell->getChildByTag(112))
    {
        if (m_vtDatas.size() >= 2 * idx + 2)
        {
            CCLayer* secondLayer = CreateSingleSpecMarket(2 * idx + 1);
            cell->addChild(secondLayer, 0, 112);
            secondLayer->setPosition(ccp(465, 0));
        }
    } else
    {
        if (m_vtDatas.size() >= 2 * idx + 2)
        {
            SingleSpecialMarketLayer* layer = dynamic_cast<SingleSpecialMarketLayer*>(cell->getChildByTag(112));
            if (layer != NULL)
            {
                layer->setVisible(true);
                layer->InitLayer(m_vtDatas[2 * idx + 1]);
            }
        } else
        {
            cell->getChildByTag(112)->setVisible(false);
        }
    }
}

// 请求商品购买次数
void SpecialMarketLayer::SendGetShopTimes()
{
    std::vector<int> itemsList;
    itemsList.clear();

    for (size_t i = 0; i < m_vtDatas.size(); ++i)
    {
        itemsList.push_back(m_vtDatas[i].market_id);
    }

    OnlineNetworkManager::sShareInstance()->SendGetShopTimes(itemsList);
}

// 请求商品价格
void SpecialMarketLayer::SendGetShopPrice()
{
    std::vector<int> itemsList;
    itemsList.clear();

    for (size_t i = 0; i < m_vtDatas.size(); ++i)
    {
        itemsList.push_back(m_vtDatas[i].market_id);
    }

    OnlineNetworkManager::sShareInstance()->SendGetExchangeParameters(itemsList);
}

CCLayer* SpecialMarketLayer::CreateSingleSpecMarket(unsigned int index)
{
    CCNodeLoader* loader = new SingleSpecialMarketLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_market", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_market");
    SingleSpecialMarketLayer* layer = dynamic_cast<SingleSpecialMarketLayer*>(node);
    layer->InitLayer(m_vtDatas[index]);
    return layer;
}

void SpecialMarketLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_ATTR_SP_DIAMOND_UPDATE :
        {
            m_diamondLabel->setString(CCString::createWithFormat("%d", UserData::GetDiamond())->getCString());
            break;
        }

    case GM_UPDATE_EXCHANGE_PARAMETER :
        {
            UpdateShoppingInfo();
            break;
        }

    default:
        break;
    }
}

void SpecialMarketLayer::UpdateShoppingInfo()
{
    for (int i = 0; i < numberOfCellsInTableView(m_shopItemList); ++i)
    {
        CCTableViewCell* cell = m_shopItemList->cellAtIndex(i);
        if (cell == NULL)
        {
            continue;
        }

        if (cell->getChildByTag(111))
        {
            SingleSpecialMarketLayer* layer = dynamic_cast<SingleSpecialMarketLayer*>(cell->getChildByTag(111));
            if (layer != NULL)
            {
                layer->UpdateShoppingInfo();
            }
        }

        if (cell->getChildByTag(112))
        {
            SingleSpecialMarketLayer* layer = dynamic_cast<SingleSpecialMarketLayer*>(cell->getChildByTag(112));
            if (layer != NULL)
            {
                layer->UpdateShoppingInfo();
            }
        }
    }
}

SingleSpecialMarketLayer::SingleSpecialMarketLayer() :
    m_itemFrame(NULL),
    m_nameLabel(NULL),
    m_costItemSprite(NULL),
    m_priceLabel(NULL),
    m_costItemNext(NULL),
    m_priceNextLabel(NULL),
    m_tipLabel(NULL),
    m_buyBtn(NULL),
    m_redLine(NULL),
    m_discountSprite(NULL),
    m_discountLabel(NULL),
    m_itemBtn(NULL)
{

}

SingleSpecialMarketLayer::~SingleSpecialMarketLayer()
{
    setItemFrame(NULL);
    setNameLabel(NULL);
    setCostItemSprite(NULL);
    setPrice(NULL);
    setCostItemNext(NULL);
    setPriceNextLabel(NULL);
    setTipLabel(NULL);
    setBuyBtn(NULL);
    setRedLine(NULL);
    setDiscountLabel(NULL);
    setDiscountSprite(NULL);
    setItemBtn(NULL);
}

SEL_MenuHandler SingleSpecialMarketLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleSpecialMarketLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "click_item", SingleSpecialMarketLayer::onClickedItem);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buy",        SingleSpecialMarketLayer::onBuyItem);
    return NULL;
}

bool SingleSpecialMarketLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_frame",      CCSprite*,        m_itemFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",            CCLabelTTF*,      m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_item",       CCSprite*,        m_costItemSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_item_next",  CCSprite*,        m_costItemNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "price",           CCLabelTTF*,      m_priceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "price_next",      CCLabelTTF*,      m_priceNextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tip",             CCLabelTTF*,      m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "red_line",        CCSprite*,        m_redLine);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "buy_btn",         CCControlButton*, m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "discount_label",  CCLabelTTF*,      m_discountLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "discount_tag",    CCSprite*,        m_discountSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_btn",        CCControlButton*, m_itemBtn);
    return false;
}

void SingleSpecialMarketLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    
}

void SingleSpecialMarketLayer::onEnter()
{
    CCLayer::onEnter();
    m_buyBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_itemBtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

void SingleSpecialMarketLayer::InitLayer(MarketData data)
{
    m_itemData = data;

    // 创建图片
    CCSprite* iconSprite = NULL;
    if (data.item_id > 100000)
    {
        iconSprite = ItemManager::Get()->getIconSpriteById(data.item_id);
    } else
    {
        const char* fairyIcon = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID",data.item_id);
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

    // 创建数量
    CCString* StrCount = CCString::createWithFormat("%d", data.item_amount);
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

    // 物品名字
    if (data.item_id > 100000)
    {
        ItemBase* itemBaseInfo = ItemManager::Get()->getItemInfoById(data.item_id);
        if (itemBaseInfo)
        {
            m_nameLabel->setString(itemBaseInfo->m_name);
        }    
    } else
    {
        const char* fairyName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyNameByID",data.item_id);
        m_nameLabel->setString(fairyName);
    }

    // 价格
    m_priceLabel->setString(CCString::createWithFormat("%d", data.cost_item_amount)->getCString());
    
    int price = MarketManager::Get()->GetIndexPrice(data.market_id);
    if (data.discount_amount == 0 && (price == 0 || price == data.cost_item_amount))
    {
        m_redLine->setVisible(false);
        m_costItemNext->setVisible(false);
        m_priceNextLabel->setVisible(false);
        m_discountLabel->setVisible(false);
        m_discountSprite->setVisible(false);
    } else
    {
        int discount_price = 0;
        if (price != 0)
        {
            discount_price = price;
        } else
        {
            discount_price = data.discount_amount;
        }

        m_redLine->setVisible(true);
        m_costItemNext->setVisible(true);
        m_priceNextLabel->setVisible(true);
        m_priceNextLabel->setString(CCString::createWithFormat("%d", discount_price)->getCString());
        m_discountLabel->setVisible(true);
        m_discountSprite->setVisible(true);

        m_discountLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_REBATE"), 
            discount_price * 10 / data.cost_item_amount)->getCString());
    }

    // tip
    if (data.restriction == 0)
    {
        m_tipLabel->setVisible(false);
    } else
    {
        m_tipLabel->setVisible(true);

        // 首先检查已经购买的次数
        int times = MarketManager::Get()->getIndexMarketItemShopTimes(data.market_id);
        m_tipLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_TODAYBUY"), data.restriction - times)->getCString());
    }
}

void SingleSpecialMarketLayer::UpdateShoppingInfo()
{
    // 已购买次数
    if (m_itemData.restriction == 0)
    {
        m_tipLabel->setVisible(false);
    } else
    {
        m_tipLabel->setVisible(true);

        // 首先检查已经购买的次数
        int times = MarketManager::Get()->getIndexMarketItemShopTimes(m_itemData.market_id);
        m_tipLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_TODAYBUY"), m_itemData.restriction - times)->getCString());
    }

    // 价格
    m_priceLabel->setString(CCString::createWithFormat("%d", m_itemData.cost_item_amount)->getCString());

    int price = MarketManager::Get()->GetIndexPrice(m_itemData.market_id);
    if (m_itemData.discount_amount == 0 && (price == 0 || price == m_itemData.cost_item_amount))
    {
        m_redLine->setVisible(false);
        m_costItemNext->setVisible(false);
        m_priceNextLabel->setVisible(false);
        m_discountLabel->setVisible(false);
        m_discountSprite->setVisible(false);
    } else
    {
        int discount_price = 0;
        if (price != 0)
        {
            discount_price = price;
        } else
        {
            discount_price = m_itemData.discount_amount;
        }

        m_redLine->setVisible(true);
        m_costItemNext->setVisible(true);
        m_priceNextLabel->setVisible(true);
        m_priceNextLabel->setString(CCString::createWithFormat("%d", discount_price)->getCString());
        m_discountLabel->setVisible(true);
        m_discountSprite->setVisible(true);

        m_discountLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_REBATE"), 
            discount_price * 10 / m_itemData.cost_item_amount)->getCString());
    }
}

void SingleSpecialMarketLayer::onClickedItem(CCObject* object, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(object);
    if (node == NULL)
    {
        return;
    }

    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(m_itemData.item_id, pt);
}

void SingleSpecialMarketLayer::onBuyItem(CCObject* object, CCControlEvent event)
{
    MainMenuLayer* mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowBatchShoppingLayer(true);
        dynamic_cast<BatchShoppingLayer*>(mLayer->getBatchShoppingLayer())->InitLayer(m_itemData, BatchShoppingType_SpecialMarket);
    }
}
