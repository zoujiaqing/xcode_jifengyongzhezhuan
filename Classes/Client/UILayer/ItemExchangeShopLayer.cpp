#include "OnlineNetworkManager.h"
#include "ItemExchangeShopLayer.h"
#include "CallNativeFuncManager.h"
#include "Localization.h"
#include "CCBLoaderManager.h"
#include "ItemManager.h"
#include "GlobalFunction.h"
#include "ItemExchangeShopManager.h"
#include "GameAudioManager.h"
#include "SoundEnum.h"
#include "LuaTinkerManager.h"

ItemExchangeShopLayer::ItemExchangeShopLayer()
    : m_pTitleLabel(NULL)
    , m_pLabel1(NULL)
    , m_pCloseBtn(NULL)
    , m_pTableViewLayer(NULL)
    , m_pTableView(NULL)
    , m_pSingleContent(NULL)
{
    m_vecExchangeItems.clear();
}
ItemExchangeShopLayer::~ItemExchangeShopLayer()
{
    setTitleLabel(NULL);
    setLabel1(NULL);
    setCloseBtn(NULL);
    setTableView(NULL);
    setTableViewLayer(NULL);
    setSingleContent(NULL);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "updateItemExchangeInfo");
}

SEL_MenuHandler ItemExchangeShopLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
SEL_CCControlHandler ItemExchangeShopLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",  ItemExchangeShopLayer::OnCloseBtnClicked);
    return NULL;
}
bool ItemExchangeShopLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleLabel",       CCLabelTTF*,         m_pTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable1",           CCLabelTTF*,         m_pLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",         CCControlButton*,    m_pCloseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableViewLayer",   CCLayer*,            m_pTableViewLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableView",        CCTableView*,        m_pTableView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSinlgeContent",        CCLayer*,        m_pSingleContent)

    return true;
}
void ItemExchangeShopLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    setTouchEnabled(true);
    m_pCloseBtn->setTouchPriority(kCCLayerMenuTouchPriority);

    InitData();
    InitMultiLanguage();
    AddTableView(NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ItemExchangeShopLayer::AddTableView), "updateItemExchangeInfo", NULL);
}
bool ItemExchangeShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void ItemExchangeShopLayer::registerWithTouchDispatcher()
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
}
void ItemExchangeShopLayer::onEnter()
{
     CCLayer::onEnter();
}
void ItemExchangeShopLayer::onExit()
{
    CCLayer::onExit();
    this->stopAllActions();
    this->unscheduleAllSelectors();
}

// tableview source
CCSize ItemExchangeShopLayer::cellSizeForTable(CCTableView *table)
{
    return m_pSingleContent->getContentSize();
}

void ItemExchangeShopLayer::refreshCell(CCTableViewCell* cell, int idx)
{
    if(!cell)
    {
        return;
    }
    SingleItemExchangeShop* pLayer = dynamic_cast<SingleItemExchangeShop*>(cell->getChildByTag(111));
    if(!pLayer)
    {
        return;
    }

    if(idx >= m_vecExchangeItems.size())
    {
        return;
    }
    pLayer->InitLayer(m_vecExchangeItems[idx]);
}
CCTableViewCell* ItemExchangeShopLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

        SingleItemExchangeShop* pLayer = this->createSingleItemExchangeShop();
        pLayer->setTag(111);
        cell->addChild(pLayer);
        this->refreshCell(cell, idx);
    }

    return cell;
}
unsigned int ItemExchangeShopLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_vecExchangeItems.size();
}
void ItemExchangeShopLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void ItemExchangeShopLayer::InitData()
{
    std::list<ExchangeItem*>* exchangelist = ItemManager::Get()->getExchangeItemsByType(16);
    exchangelist->sort(ExchangeItem::smaller());
    for(std::list<ExchangeItem*>::iterator it = exchangelist->begin();
        it != exchangelist->end();++it)
    {
        if(*it != NULL)
        {
            m_vecExchangeItems.push_back(*it);
        }
    }
}

void ItemExchangeShopLayer::InitMultiLanguage()
{
    m_pTitleLabel->setString(Localizatioin::getLocalization("M_EXCHANGESHOP_TITLE"));
    m_pLabel1->setString(Localizatioin::getLocalization("M_EXCHANGESTORE_3"));
}
SingleItemExchangeShop* ItemExchangeShopLayer::createSingleItemExchangeShop()
{
    CCNodeLoader* pLoader = new SingleItemExchangeShopLoader();
    CCNode* pNode = CCBLoaderManager::getInstance()->LoadFromCCBI(pLoader, "SingleItemExchangeShop", "SingleItemExchangeShop");
    SingleItemExchangeShop* pLayer = dynamic_cast<SingleItemExchangeShop*>(pNode);
    return pLayer;
}
void ItemExchangeShopLayer::OnCloseBtnClicked(CCObject* pSender, CCControlEvent event)
{
    CallNativeFuncManager::getInstance()->ShowItemExchangeShopLayer(false);
}
void ItemExchangeShopLayer::AddTableView(CCObject* obj)
{
    bool needSetOffset = false;
    if(this->m_pTableView)
    {
        ItemExchangeShopManager::Get()->setHeadScrollPos(m_pTableView->getContentOffset());
        this->m_pTableView->removeFromParentAndCleanup(true);
        setTableView(NULL);
        needSetOffset = true;
    }

    CCSize size = CCSizeMake(this->m_pTableViewLayer->getContentSize().width, this->m_pTableViewLayer->getContentSize().height);
    CCTableView* tableview = CCTableView::create(this, size);
    tableview->setDirection(kCCScrollViewDirectionVertical);
    tableview->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableview->setDelegate(this);
    tableview->setTouchPriority(kCCLayerMenuTouchPriority - 1);
    tableview->setForbidTouched(true);
    setTableView(tableview);
    this->m_pTableViewLayer->addChild(m_pTableView);
    m_pTableView->setTouchPriority(kCCLayerMenuTouchPriority);
    this->m_pTableView->reloadData();

    if(needSetOffset == true)
    {
        m_pTableView->setContentOffset(ItemExchangeShopManager::Get()->getHeadScrollPos());
    }
}


/////////////////////////////////////////single /////////////////////////////////////////////
SingleItemExchangeShop::SingleItemExchangeShop()
    : m_pLabel1(NULL),	   
      m_pLabel2(NULL),	   
      m_pExchangeBtn(NULL), 
      m_pItemLayer1(NULL),  
      m_pItemBtn1(NULL)
{
    m_vecItemLayer.clear();
    m_vecItemLayer.resize(3);
    m_vecItemBtn.clear();
    m_vecItemBtn.resize(3);
    m_vecItemNum.clear();
    m_vecItemNum.resize(3);
    m_curExchangeId = 0;
	m_curItemId = 0;
}
SingleItemExchangeShop::~SingleItemExchangeShop()
{
    setLabel1(NULL);
    setLabel2(NULL);
    setExchangeBtn(NULL);
    setItemLayer1(NULL);
    setItemBtn1(NULL);
}

SEL_MenuHandler SingleItemExchangeShop::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
SEL_CCControlHandler SingleItemExchangeShop::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "exchange",    SingleItemExchangeShop::OnExchangeBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "item_click1", SingleItemExchangeShop::OnItem1Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "item_click2", SingleItemExchangeShop::OnItem2Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "item_click3", SingleItemExchangeShop::OnItem3Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "item_click4", SingleItemExchangeShop::OnItem4Clicked);
    return NULL;
}
bool SingleItemExchangeShop::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable1",          CCLabelTTF*,        m_pLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable2",          CCLabelTTF*,        m_pLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem2Num",        CCLabelTTF*,        m_vecItemNum[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem3Num",        CCLabelTTF*,        m_vecItemNum[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItem4Num",        CCLabelTTF*,        m_vecItemNum[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExchangeBtn",     CCControlButton*,   m_pExchangeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemLayer1",      CCLayer*,           m_pItemLayer1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBtn1",        CCControlButton*,   m_pItemBtn1);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemLayer2",        CCLayer*,         m_vecItemLayer[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemLayer3",        CCLayer*,         m_vecItemLayer[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemLayer4",        CCLayer*,         m_vecItemLayer[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBtn2",        CCControlButton*,   m_vecItemBtn[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBtn3",        CCControlButton*,   m_vecItemBtn[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBtn4",        CCControlButton*,   m_vecItemBtn[2]);
    return true;
}
void SingleItemExchangeShop::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    m_pLabel1->setString(Localizatioin::getLocalization("M_EXCHANGESTORE_1"));
    m_pLabel2->setString(Localizatioin::getLocalization("M_EXCHANGESTORE_2"));
    m_pExchangeBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_EXCHANGE_BUTTON")), CCControlStateNormal);
    m_pExchangeBtn->setTouchPriority(kCCLayerTouchPriority);
    m_pItemBtn1->setTouchPriority(kCCLayerTouchPriority);
    for(int i = 0; i < m_vecItemBtn.size(); i++)
    {
        m_vecItemBtn[i]->setTouchPriority(kCCLayerTouchPriority);
    }
}
void SingleItemExchangeShop::InitLayer(ExchangeItem* item)
{
    m_curExchangeId = item->m_exchangeId;

    // from items
    std::list<std::pair<unsigned int ,unsigned int> >* listToItems = item->m_toItems;
    std::list<std::pair<unsigned int ,unsigned int> >::iterator rt = listToItems->begin();
    int toItemid = rt->first;
    m_pItemBtn1->setTag(toItemid);
    CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(toItemid);
    iconSprite->setAnchorPoint(ccp(0.5, 0.5));
    iconSprite->setPosition(ccp(m_pItemLayer1->getContentSize().width / 2, m_pItemLayer1->getContentSize().height / 2));
    m_pItemLayer1->removeChildByTag(66);
    m_pItemLayer1->addChild(iconSprite, 66, 66);

    // to items
    std::list<std::pair<unsigned int ,unsigned int> >* listFromItems = item->m_fromItems;
    std::list<std::pair<unsigned int ,unsigned int> >::iterator it = listFromItems->begin();
    int i = 0;
    for(; it != listFromItems->end(); it++)
    {
        int fromItemid = it->first;
        int fromItemCount = it->second;
        m_vecItemBtn[i]->setTag(fromItemid);
        AddItemIconAndCount(fromItemid, m_vecItemLayer[i], fromItemCount, i);
        i ++;
    }

    // visiable
    for(int j = 0; j < i; j++)
    {
        m_vecItemLayer[j]->setVisible(true);
        m_vecItemNum[j]->setVisible(true);
    }
    for(int m = i; m < 3; m++)
    {
        m_vecItemLayer[m]->setVisible(false);
        m_vecItemNum[m]->setVisible(false);
    }
}

void SingleItemExchangeShop::OnExchangeBtnClicked(CCObject* pSender, CCControlEvent event)
{
    OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(m_curExchangeId, 0);
    GameAudioManager::sharedManager()->playEffect(SOUND_EFFECT_CLICK, false);
}
void SingleItemExchangeShop::OnItem1Clicked(CCObject* pSender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(node->getTag(), pt);

	/*ItemBase * info = ItemManager::Get()->getItemInfoById(m_curItemId);
	EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleItemExchangeShop::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/
}
void SingleItemExchangeShop::OnItem2Clicked(CCObject* pSender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(node->getTag(), pt);

	/*ItemBase * info = ItemManager::Get()->getItemInfoById(m_curItemId);
	EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleItemExchangeShop::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/
}
void SingleItemExchangeShop::OnItem3Clicked(CCObject* pSender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(node->getTag(), pt);

	/*ItemBase * info = ItemManager::Get()->getItemInfoById(m_curItemId);
	EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleItemExchangeShop::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/
}
void SingleItemExchangeShop::OnItem4Clicked(CCObject* pSender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(node->getTag(), pt);

	/*ItemBase * info = ItemManager::Get()->getItemInfoById(m_curItemId);
	EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleItemExchangeShop::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/
}

void SingleItemExchangeShop::onGetWayCallBack(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyGetwayLayer",m_curItemId);
}

int SingleItemExchangeShop::getItemId(std::string Itemstr)
{
    int index = Itemstr.find_first_of("/");
    int itemid = atoi(Itemstr.substr(0, index).c_str());
    return itemid;
}
int SingleItemExchangeShop::getItemCount(std::string Itemstr)
{
    int index = Itemstr.find_first_of("/");
    int count = atoi(Itemstr.substr(index + 1, Itemstr.length() - index - 1).c_str());
    return count;
}

void SingleItemExchangeShop::AddItemIconAndCount(int itemId, CCLayer* layer, int Count, int tag)
{
    CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(itemId);
    iconSprite->setAnchorPoint(ccp(0.5, 0.5));
    iconSprite->setPosition(ccp(layer->getContentSize().width / 2, layer->getContentSize().height / 2));
    layer->removeChildByTag(tag);
    layer->addChild(iconSprite, tag, tag);

    int curCount = ItemManager::Get()->getItemNumberById(itemId);
    if((curCount - Count) >= 0)
    {
        m_vecItemNum[tag]->setColor(SYS_FONT_GREEN);
    }
    else
    {
        m_vecItemNum[tag]->setColor(SYS_FONT_RED);
    }

    std::string tempStr = IntToStr(curCount);
    tempStr.append(" / ").append(IntToStr(Count));
    m_vecItemNum[tag]->setString(tempStr.c_str());
}