#include "OnlineNetworkManager.h"
#include "GuildShopLayer.h"
#include "UIManager.h"
#include "MainMenuLayer.h"
#include "GlobalFunction.h"
#include "ItemManager.h"
#include "Localization.h"
#include "InterfaceManager.h"
#include "LuaTinkerManager.h"

GuildShopLayer::GuildShopLayer() :
    m_diamondLabel(NULL),
    m_presLabel(NULL),
    m_contentLayer(NULL),
    m_closeButton(NULL),
    m_shopList(NULL),
    m_pRefreshLabel(NULL)
{

}

GuildShopLayer::~GuildShopLayer()
{
    setDiamondLabel(NULL);
    setPresLabel(NULL);
    setContentLayer(NULL);
    setCloseButton(NULL);
    setShopList(NULL);
    setRefreshLabel(NULL);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GuildShopItemsUpdate");
}

SEL_MenuHandler GuildShopLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildShopLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", GuildShopLayer::onClose);
    return NULL;
}

bool GuildShopLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "diamond",       CCLabelTTF*,      m_diamondLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pres",          CCLabelTTF*,      m_presLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "content_layer", CCLayer*,         m_contentLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",     CCControlButton*, m_closeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelRefresh",     CCLabelTTF*, m_pRefreshLabel);
    return false;
}

void GuildShopLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);
    m_pRefreshLabel->setString(Localizatioin::getLocalization("G_SHOP_TEXT"));
    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuildShopLayer::OnMsgCallBack), "GuildShopItemsUpdate", NULL);
    GuildManager::Get()->SendGetGuildShopItems();
}

void GuildShopLayer::ConfigUI()
{
    bool needSetOffset = false;
    CCPoint offset = CCPointZero;
    if(this->m_shopList)
    {
        offset.x = m_shopList->getContentOffset().x;
        offset.y = m_shopList->getContentOffset().y;
        needSetOffset = true;
        this->m_shopList->removeFromParentAndCleanup(true);
        setShopList(NULL);
    }

    // 添加商品列表
    setShopList(CCTableView::create(this, m_contentLayer->getContentSize()));
    m_shopList->setDirection(kCCScrollViewDirectionVertical);
    m_shopList->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_contentLayer->addChild(m_shopList);
    m_shopList->setTouchPriority(kCCScrollMenuPriority);
    //m_shopList->setBounceable(false);
    m_shopList->setDelegate(this);
    m_shopList->reloadData();

    if(needSetOffset == true)
    {
        m_shopList->setContentOffset(offset);
    }
}

void GuildShopLayer::OnMsgCallBack(CCObject* obj)
{
    // cur diamond coin contibute
    USERINFO info = UserData::GetUserInfo();
    m_presLabel->setString(IntToStr(info.m_guild_contributes));
    m_diamondLabel->setString(IntToStr(UserData::GetDiamond()));

    ConfigUI();
}

bool GuildShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildShopLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void GuildShopLayer::onEnter()
{
    CCLayer::onEnter();

    m_closeButton->setTouchPriority(kCCScrollMenuPriority);
}

CCSize GuildShopLayer::cellSizeForTable(CCTableView *table)
{
    return CCSize(900, 116);
}

CCTableViewCell* GuildShopLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int GuildShopLayer::numberOfCellsInTableView(CCTableView *table)
{
    int size = GuildManager::Get()->getGuildShopItems().size();
    if(size == 0)
    {
        return 0;
    }
    if(size % 2 == 0)
    {
        return size / 2;
    }
    else
    {
        return size / 2 + 1;
    }
}

void GuildShopLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void GuildShopLayer::onClose(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildShopLayer(false);
        bool flag = GuildManager::Get()->getIsGuildInfoNeedOpen();
        if(flag == true)
        {
            mLayer->ShowGuildInfoLayer(true);
        }
    }
}
 
void GuildShopLayer::RefreshCell(CCTableViewCell* cell, unsigned int index)
{
    if (! cell->getChildByTag(111))
    {
        CCLayer* layer = CreateSingleGuildShopLayer();
        cell->addChild(layer, 0, 111);
        layer->setPosition(ccp(0, 5));
    }

    SingleGuildShopLayer* layer1 = dynamic_cast<SingleGuildShopLayer*>(cell->getChildByTag(111));
    if (layer1 != NULL && (index * 2) < GuildManager::Get()->getGuildShopItems().size())
    {
        layer1->InitLayer(index * 2, this, GuildManager::Get()->getGuildShopItems()[index * 2]);
    }

    // judge index is the biggest
    if((index * 2 + 1) >= GuildManager::Get()->getGuildShopItems().size())
    {
        cell->removeChildByTag(112);
        return;
    }

    if (! cell->getChildByTag(112))
    {
        CCLayer* layer = CreateSingleGuildShopLayer();
        cell->addChild(layer, 0, 112);
        layer->setPosition(ccp(450, 5));
    }

    SingleGuildShopLayer* layer2 = dynamic_cast<SingleGuildShopLayer*>(cell->getChildByTag(112));
    if (layer2 != NULL && (index * 2 + 1) < GuildManager::Get()->getGuildShopItems().size())
    {
        layer2->InitLayer(index * 2 + 1, this, GuildManager::Get()->getGuildShopItems()[index * 2 + 1]);
    }
}

CCLayer* GuildShopLayer::CreateSingleGuildShopLayer()
{
    CCNodeLoader* loader = new SingleGuildShopLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_guild_shop", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_guild_shop");
    return dynamic_cast<CCLayer*>(node);
}

void GuildShopLayer::closeLayerCallBack(void)
{
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			mLayer->ShowGuildShopLayer(false,false);
		}
	}
}

SingleGuildShopLayer::SingleGuildShopLayer() :
    m_buyButton(NULL),
    m_itemFrame(NULL),
    m_costItem(NULL),
    m_costAmount(NULL),
    m_tipLabel(NULL),
    m_pItemName(NULL),
    m_pItemAmount(NULL),
    m_pItemClickBtn(NULL),
    m_pNormalTag(NULL),
    m_pSpecialTag(NULL),
	m_curItemId(0)
{
    m_shopIndex = 1;
}

SingleGuildShopLayer::~SingleGuildShopLayer()
{
    setBuyButton(NULL);
    setItemName(NULL);
    setItemFrame(NULL);
    setCostAmount(NULL);
    setCostItem(NULL);
    setTispLabel(NULL);
    setItemAmount(NULL);
    setItemClickBtn(NULL);
    setNormalTag(NULL);
    setSpecialTag(NULL);
}

SEL_MenuHandler SingleGuildShopLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleGuildShopLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buy", SingleGuildShopLayer::onBuyClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "item_click", SingleGuildShopLayer::onItemsClicked);
    return NULL;
}

bool SingleGuildShopLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "buy_btn",     CCControlButton*, m_buyButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemClickBtn",     CCControlButton*, m_pItemClickBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_frame",  CCSprite*,        m_itemFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF*,      m_pItemName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_item",   CCLayer*,         m_costItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_amount", CCLabelTTF*,      m_costAmount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tip",         CCLabelTTF*,      m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCount",  CCLabelTTF*,    m_pItemAmount);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNormalTag",   CCScale9Sprite*, m_pNormalTag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpecialTag",  CCSprite*,       m_pSpecialTag);
    return false;
}

void SingleGuildShopLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{

}

void SingleGuildShopLayer::InitLayer(unsigned int index, GuildShopLayer* layer, GuildShopItems info)
{
    m_shopIndex = info.itemindex;


    // the four ahead is the special
    if(index < 4)
    {
        m_pNormalTag->setVisible(false);
        m_pSpecialTag->setVisible(true);
    }
    else
    {
        m_pNormalTag->setVisible(true);
        m_pSpecialTag->setVisible(false);
    }

    // to 
    int toItemId = info.toitemId;
    m_pItemClickBtn->setTag(toItemId);
    if(info.toamount <= 1)
    {
        m_pItemAmount->setVisible(false);
    }
    else
    {
        m_pItemAmount->setVisible(true);
        m_pItemAmount->setString(IntToStr(info.toamount));
    }
    CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(toItemId);
    m_itemFrame->removeChildByTag(110);
    m_itemFrame->addChild(iconSprite, 110, 110);
    iconSprite->setPosition(ccp(m_itemFrame->getContentSize().width / 2, m_itemFrame->getContentSize().height / 2));
    m_pItemName->setString(ItemManager::Get()->getItemInfoById(toItemId)->m_name);

    // from
    m_costAmount->setString(IntToStr(info.fromamount));
    CCSprite* fromIcon = InterfaceManager::Get()->getIconByItemid(info.fromitemId);
    m_costItem->removeAllChildren();
    m_costItem->addChild(fromIcon);
    fromIcon->setPosition(ccp(m_costItem->getContentSize().width / 2, m_costItem->getContentSize().height / 2));

    // judge
    int times = info.times;
    int timesMax = info.max_times;
    int limit = info.guildLvlimit;
    CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("G_SHOP_BUYNUMBER1"), (timesMax - times));
    m_tipLabel->setString(str->getCString());
    m_tipLabel->setColor(SYS_FONT_WHITE);
    if(index > 1 && limit != 0)
    {
        CCString* str2 = CCString::createWithFormat(Localizatioin::getLocalization("G_SHOP_LEVER"), limit);
        m_tipLabel->setString(str2->getCString());
        m_tipLabel->setColor(SYS_FONT_RED);
    }

    // button enable
    if((timesMax - times) == 0 || limit != 0)
    {
        m_buyButton->setEnabled(false);
    }
    else
    {
        m_buyButton->setEnabled(true);
    }
}

void SingleGuildShopLayer::onEnter()
{
    CCLayer::onEnter();
    m_buyButton->setTouchPriority(kCCLayerTouchPriority);
    m_pItemClickBtn->setTouchPriority(kCCLayerTouchPriority);
}

void SingleGuildShopLayer::onBuyClicked(CCObject* sender, CCControlEvent event)
{
    GuildManager::Get()->SendGuildBuy(31001, m_shopIndex);
}

void SingleGuildShopLayer::onGetWayCallBack(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyGetwayLayer",m_curItemId);
}

void SingleGuildShopLayer::onItemsClicked(CCObject* sender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(sender);
    int itemid = node->getTag();
    if(itemid == 0)
    {
        return;
    }
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(itemid, pt);

	/*ItemBase * info = ItemManager::Get()->getItemInfoById(itemid);
	EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		m_curItemId = itemid;
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleGuildShopLayer::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/
}
