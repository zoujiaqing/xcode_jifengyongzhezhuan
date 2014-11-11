#include "OnlineNetworkManager.h"
#include "VipMarketLayer.h"
#include "UserData.h"
#include "UIDefine.h"
#include "ItemManager.h"
#include "Localization.h"
#include "MainMenuLayer.h"
#include "GMessage.h"
#include "LuaTinkerManager.h"
#include "AspriteManager.h"
#include "NotificationCenter.h"
#include "BatchShoppingLayer.h"

VipMarketLayer::VipMarketLayer() :
    m_diamondLabel(NULL),
    m_shopListLayer(NULL),
    m_vipItemList(NULL)
{
    m_vtDatas.clear();
}

VipMarketLayer::~VipMarketLayer()
{
    setDiamondLabel(NULL);
    setShopListLayer(NULL);
    setVipItemList(NULL);
    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler VipMarketLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler VipMarketLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool VipMarketLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "shop_list_layer", CCLayer*,    m_shopListLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "diamond",         CCLabelTTF*, m_diamondLabel);
    return false;
}

void VipMarketLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    RefreshShopItemList();
    m_diamondLabel->setString(CCString::createWithFormat("%d", UserData::GetDiamond())->getCString());
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
}

void VipMarketLayer::RefreshShopItemList()
{
    // 策划现在要求礼包都显示出来
    m_vtDatas = MarketManager::Get()->getVipShopItemList(0, 15);

    // 请求物品次数
    SendGetShopTimes();

    // 请求物品价格
    SendGetShopPrice();

    if (m_vipItemList == NULL)
    {
        setVipItemList(CCTableView::create(this, m_shopListLayer->getContentSize()));
        m_vipItemList->setDirection(kCCScrollViewDirectionVertical);
        m_vipItemList->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_shopListLayer->addChild(m_vipItemList);
        m_vipItemList->setTouchPriority(kCCLayerMenuTouchPriority);
        m_vipItemList->setDelegate(this);
    } 
	else
    {
        m_vipItemList->reloadData();
    }
}


// 请求商品购买次数
void VipMarketLayer::SendGetShopTimes()
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
void VipMarketLayer::SendGetShopPrice()
{
    std::vector<int> itemsList;
    itemsList.clear();

    for (size_t i = 0; i < m_vtDatas.size(); ++i)
    {
        itemsList.push_back(m_vtDatas[i].market_id);
    }

    OnlineNetworkManager::sShareInstance()->SendGetExchangeParameters(itemsList);
}

CCLayer* VipMarketLayer::CreateSingleVipMarket(unsigned int index)
{
    CCNodeLoader* loader = new SingleVipMarketLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_vip_market", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_vip_market");
    SingleVipMarketLayer* layer = dynamic_cast<SingleVipMarketLayer*>(node);
    layer->InitLayer(m_vtDatas[index]);
    return layer;
}

CCSize VipMarketLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(920.0f, 135.0f);
}

CCTableViewCell* VipMarketLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int VipMarketLayer::numberOfCellsInTableView(CCTableView *table)
{
    return (m_vtDatas.size() + 1) / 2;
}

void VipMarketLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void VipMarketLayer::RefreshCell(CCTableViewCell* cell, unsigned int index)
{
    if (! cell->getChildByTag(111))
    {
        CCLayer* firstLayer = CreateSingleVipMarket(2 * index);
        cell->addChild(firstLayer, 0, 111);
        firstLayer->setPosition(ccp(5, 0));
    } else
    {
        SingleVipMarketLayer* layer = dynamic_cast<SingleVipMarketLayer*>(cell->getChildByTag(111));
        if (layer != NULL)
        {
            layer->InitLayer(m_vtDatas[2 * index]);
        }
    }

    if (! cell->getChildByTag(112))
    {
        if (m_vtDatas.size() >= 2 * index + 2)
        {
            CCLayer* secondLayer = CreateSingleVipMarket(2 * index + 1);
            cell->addChild(secondLayer, 0, 112);
            secondLayer->setPosition(ccp(465, 0));
        }
    } else
    {
        if (m_vtDatas.size() >= 2 * index + 2)
        {
            SingleVipMarketLayer* layer = dynamic_cast<SingleVipMarketLayer*>(cell->getChildByTag(112));
            if (layer != NULL)
            {
                layer->setVisible(true);
                layer->InitLayer(m_vtDatas[2 * index + 1]);
            }
        } else
        {
            cell->getChildByTag(112)->setVisible(false);
        }
    }
}   

void VipMarketLayer::onBroadcastMessage(BroadcastMessage* msg)
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
            RefreshPrice();
            break;
        }

    default:
        break;
    }
}

// 刷新价格
void VipMarketLayer::RefreshPrice()
{
    UpdateShoppingInfo();
}

void VipMarketLayer::UpdateShoppingInfo()
{
    for (size_t i = 0; i < numberOfCellsInTableView(m_vipItemList); ++i)
    {
        CCTableViewCell* cell = m_vipItemList->cellAtIndex(i);
        if (cell == NULL)
        {
            continue;
        }

        if (cell->getChildByTag(111))
        {
            SingleVipMarketLayer* layer = dynamic_cast<SingleVipMarketLayer*>(cell->getChildByTag(111));
            if (layer != NULL)
            {
                layer->UpdateShoppingInfo();
            }
        }

        if (cell->getChildByTag(112))
        {
            SingleVipMarketLayer* layer = dynamic_cast<SingleVipMarketLayer*>(cell->getChildByTag(112));
            if (layer != NULL)
            {
                layer->UpdateShoppingInfo();
            }
        }
    }
}

SingleVipMarketLayer::SingleVipMarketLayer() :
    m_itemFrame(NULL),
    m_nameLabel(NULL),
    m_costItemSprite(NULL),
    m_priceLabel(NULL),
    m_costItemNext(NULL),
    m_priceNextLabel(NULL),
    m_vipTipLabel(NULL),
    m_buyBtn(NULL),
    m_redLine(NULL),
    m_discountSprite(NULL),
    m_discountLabel(NULL),
    m_itemBtn(NULL),
	m_buyBtnNote(NULL)
{

}

SingleVipMarketLayer::~SingleVipMarketLayer()
{
    setItemFrame(NULL);
    setNameLabel(NULL);
    setCostItemSprite(NULL);
    setPrice(NULL);
    setCostItemNext(NULL);
    setPriceNextLabel(NULL);
    setVipTipLabel(NULL);
    setBuyBtn(NULL);
    setRedLine(NULL);
    setDiscountLabel(NULL);
    setDiscountSprite(NULL);
    setItemBtn(NULL);
}

SEL_MenuHandler SingleVipMarketLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleVipMarketLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "click_item", SingleVipMarketLayer::onClickedItem);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buy",        SingleVipMarketLayer::onBuyItem);
    return NULL;
}

bool SingleVipMarketLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_frame",      CCSprite*,        m_itemFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",            CCLabelTTF*,      m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_item",       CCSprite*,        m_costItemSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_item_next",  CCSprite*,        m_costItemNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "price",           CCLabelTTF*,      m_priceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "price_next",      CCLabelTTF*,      m_priceNextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_tip",         CCLabelTTF*,      m_vipTipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "red_line",        CCSprite*,        m_redLine);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "buy_btn",         CCControlButton*, m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "discount_label",  CCLabelTTF*,      m_discountLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "discount_tag",    CCSprite*,        m_discountSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_btn",        CCControlButton*, m_itemBtn);
    return false;
}

void SingleVipMarketLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    
}

void SingleVipMarketLayer::onEnter()
{
    CCLayer::onEnter();
    m_buyBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_itemBtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

void SingleVipMarketLayer::onClickedItem(CCObject* object, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(object);
    if (node == NULL)
    {
        return;
    }

    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(m_itemData.item_id, pt);
}

void SingleVipMarketLayer::onBuyItem(CCObject* object, CCControlEvent event)
{
    if (m_itemData.restriction == 1)
    {
        int cost = 0;
        int price = MarketManager::Get()->GetIndexPrice(m_itemData.market_id);
        if (m_itemData.discount_amount == 0 && (price == 0 || price == m_itemData.cost_item_amount))
        {
            cost = m_itemData.cost_item_amount;
        }
		else if (price != 0)
        {
            cost = price;
        }
		else
        {
            cost = m_itemData.discount_amount;
        }

        if (UserData::GetDiamond() < cost)
        {
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",cost);
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
            return;
        }

        MarketManager::Get()->setLastShopItemId((int) m_itemData.item_id);
        unsigned int i = m_itemData.market_id;
        OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(31000, &i, 1);
    } 
	else
    {
        MainMenuLayer* mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
        if (mLayer)
        {
            mLayer->ShowBatchShoppingLayer(true);
            dynamic_cast<BatchShoppingLayer*>(mLayer->getBatchShoppingLayer())->InitLayer(m_itemData, BatchShoppingType_VipMarket);
        }
    }
}

void SingleVipMarketLayer::InitLayer(MarketData data)
{
    m_itemData = data;

    // 创建图片
    CCSprite* iconSprite = NULL;
    if (data.item_id > 100000)
    {
        iconSprite = ItemManager::Get()->getIconSpriteById(data.item_id);
    } 
	else
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

        m_itemFrame->getParent()->addChild(iconSprite, 0 , 111);
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
    } 
	else
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
    }
	else
    {
        int discount_price = 0;
        if (price != 0)
        {
            discount_price = price;
        } 
		else
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

    m_vipTipLabel->setString(CCString::createWithFormat(
        Localizatioin::getLocalization("M_SHOP_VIP"), data.vip_level)->getCString());

	if (m_buyBtnNote == NULL)
	{
		setBuyBtnNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));
		float scale = UIManager::sharedManager()->getScaleFactor();

		m_buyBtnNote->setPosition(ccpAdd(m_buyBtn->getPosition(), ccp(60, 0)));
		m_buyBtn->getParent()->addChild(m_buyBtnNote);
	}
	CCActionInterval* action = NULL;
	action = CCMoveBy::create(0.4,ccp(0,10));
	CCActionInterval* action_back = action->reverse();
	CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));
	CCRepeatForever* pAction = CCRepeatForever::create(seq);
	m_buyBtnNote->runAction(pAction);

    // 检查按钮是否需要变化
    int buyTimes = MarketManager::Get()->getIndexMarketItemShopTimes(data.market_id);
    if (buyTimes >= data.restriction && data.restriction != 0)
    {
        m_buyBtn->setEnabled(false);
        m_buyBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_SHOP_ALREADYBUY")), CCControlStateDisabled);
    }
	else
    {
        m_buyBtn->setEnabled(true);
    }

	if (buyTimes == 0 && data.market_id <= (UserData::GetVipLevel() + MarketManager::Get()->getVip0PackId()))
		m_buyBtnNote->setVisible(true);
	else
		m_buyBtnNote->setVisible(false);
}

void SingleVipMarketLayer::UpdateShoppingInfo()
{
    int buyTimes = MarketManager::Get()->getIndexMarketItemShopTimes(m_itemData.market_id);

    if (buyTimes >= m_itemData.restriction &&
        m_itemData.restriction != 0)
    {
        m_buyBtn->setEnabled(false);
        m_buyBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_SHOP_ALREADYBUY")), CCControlStateDisabled);
    }

	if (buyTimes == 0 && m_itemData.market_id <= (UserData::GetVipLevel() + MarketManager::Get()->getVip0PackId()))
		m_buyBtnNote->setVisible(true);
	else
		m_buyBtnNote->setVisible(false);

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
    } 
	else
    {
        int discount_price = 0;
        if (price != 0)
        {
            discount_price = price;
        } 
		else
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
