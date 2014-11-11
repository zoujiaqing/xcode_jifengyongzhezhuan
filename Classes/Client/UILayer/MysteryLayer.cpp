#include "OnlineNetworkManager.h"
#include "PVPMessage.h"
#include "MysteryLayer.h"
#include "MarketLayer.h"
#include "UIDefine.h"
#include "ItemManager.h"
#include "MainMenuLayer.h"
#include "LuaTinkerManager.h"
#include "MarketManager.h"
#include "TXGUI.h"
#include "AspriteManager.h"
#include "TimeManager.h"
#include "Localization.h"
#include "GMessage.h"
#include "NotificationCenter.h"
#include "MessageBoxLayer.h"

MysteryLayer::MysteryLayer() :
    m_shopListLayer(NULL),
    m_shopList(NULL),
    m_diamond(NULL),
    m_timeLabel(NULL),
    m_hunLabel(NULL),
    m_cardLabel(NULL),
    m_refreshbtn(NULL),
    m_descLabel(NULL),
    m_desc2Label(NULL),
    m_mysteryNote(NULL)
{
    m_shopItems.clear();
}

MysteryLayer::~MysteryLayer()
{
    setShopList(NULL);
    setShopListLayer(NULL);
    setDiamond(NULL);
    setHunLabel(NULL);
    setCardLabel(NULL);
    setRefreshBtn(NULL);
    setTimeLabel(NULL);
    setDescLabel(NULL);
    setDesc2Label(NULL);
    setMysteryNote(NULL);
    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler MysteryLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MysteryLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "refresh", MysteryLayer::onRefresh);
    return NULL;
}

bool MysteryLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "shop_list_layer", CCLayer*,    m_shopListLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "time_label",      CCLabelTTF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "diamond",         CCLabelTTF*, m_diamond);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "hun",             CCLabelTTF*, m_hunLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "card_count",      CCLabelTTF*, m_cardLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refresh_btn",     CCControlButton*, m_refreshbtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "desc_label",      CCLabelTTF*, m_descLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "desc2_label",     CCLabelTTF*, m_desc2Label);
    return false;
}

void MysteryLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    InitNote();
    PVPMessage::Get()->sendShopItems();
    setTouchEnabled(true);
    InitAttrib();
    schedule(schedule_selector(MysteryLayer::InitTime), 1.0f, kCCRepeatForever, 0.0f);
    m_descLabel->setString(Localizatioin::getLocalization("M_SHOP_REFRESH"));
    m_desc2Label->setString(Localizatioin::getLocalization("M_SHOP_STONEHINT"));
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
}

void MysteryLayer::RefreshItemList()
{
    if (m_shopList == NULL)
    {
        setShopList(CCTableView::create(this, m_shopListLayer->getContentSize()));
        m_shopList->setDirection(kCCScrollViewDirectionVertical);
        m_shopList->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_shopListLayer->addChild(m_shopList);
        m_shopList->setTouchPriority(kCCLayerMenuTouchPriority);
        m_shopList->setDelegate(this);
        m_shopList->setBounceable(false);
    } else
    {
        m_shopList->reloadData();
    }
}

CCSize MysteryLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(920.0f, 135.0f);
}

CCTableViewCell* MysteryLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int MysteryLayer::numberOfCellsInTableView(CCTableView *table)
{
    return (m_shopItems.size() + 1) / 2;
}

void MysteryLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void MysteryLayer::RefreshCell(CCTableViewCell* cell, unsigned int idx)
{
    if (! cell->getChildByTag(111))
    {
        CCLayer* firstLayer = CreateSingleShopLayer(2 * idx);
        cell->addChild(firstLayer, 0, 111);
        firstLayer->setPosition(ccp(5, 0));
    } else
    {
        SingleShopListLayer* layer = dynamic_cast<SingleShopListLayer*>(cell->getChildByTag(111));
        if (layer != NULL)
        {
            layer->InitLayer(m_shopItems[2 * idx]);
        }
    }

    if (! cell->getChildByTag(112))
    {
        if (m_shopItems.size() >= 2 * idx + 2)
        {
            CCLayer* secondLayer = CreateSingleShopLayer(2 * idx + 1);
            cell->addChild(secondLayer, 0, 112);
            secondLayer->setPosition(ccp(465, 0));
        }
    } else
    {
        if (m_shopItems.size() >= 2 * idx + 2)
        {
            SingleShopListLayer* layer = dynamic_cast<SingleShopListLayer*>(cell->getChildByTag(112));
            if (layer != NULL)
            {
                layer->setVisible(true);
                layer->InitLayer(m_shopItems[2 * idx + 1]);
            }
        } else
        {
            cell->getChildByTag(112)->setVisible(false);
        }
    }
}

CCLayer* MysteryLayer::CreateSingleShopLayer(unsigned int index)
{
    CCNodeLoader* loader = new SingleShopListLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_shop_list", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_shop_list");
    SingleShopListLayer* layer = dynamic_cast<SingleShopListLayer*>(node);
    layer->InitLayer(m_shopItems[index]);
    return layer;
}

void MysteryLayer::InitNote()
{
    setMysteryNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));
    m_mysteryNote->setPosition(ccpAdd(m_refreshbtn->getPosition(), ccp(100, 0)));
    m_refreshbtn->getParent()->addChild(m_mysteryNote);

    CCActionInterval* action2 = NULL;
    action2 = CCMoveBy::create(0.4,ccp(0,10));

    CCActionInterval* action_back2 = action2->reverse();
    CCActionInterval* seq2 = (CCActionInterval*)(CCSequence::create( action2, action_back2, NULL ));

    CCRepeatForever* pAction2 = CCRepeatForever::create(seq2);
    m_mysteryNote->runAction(pAction2);
}

bool MysteryLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void MysteryLayer::onEnter()
{
    CCLayer::onEnter();
    m_refreshbtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

void MysteryLayer::TryRefreshList()
{
    PVPMessage::Get()->sendShopItems();
    InitAttrib();
}

void MysteryLayer::onRefresh(CCObject* object, CCControlEvent event)
{
    const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", 10800, "FromItems");
    string exchangeStr = string(fromItems);
    int dashPos = exchangeStr.find_first_of("/");
    if(dashPos != exchangeStr.npos)
    {
        unsigned int length = exchangeStr.size();
        std::string idStr = exchangeStr.substr(0,dashPos);
        std::string numStr = exchangeStr.substr(dashPos+1,length-1);
        int id = atoi(idStr.c_str());
        int num = atoi(numStr.c_str());
        ASSERT(id == 416002,"cost is not diamond!");

        USERINFO userInfo = UserData::GetUserInfo();
        unsigned int freeTimes = userInfo.m_refreshTimes;

        // 首先判断刷新卡数量
        // 再判断免费次数
		//1表示用免费次数
		//2表示用刷新卡
		//3表示用钻石

        int cardCount = LuaTinkerManager::Get()->callLuaFunc<int>("Script/main.lua", "GetRefreshCardCount");
		if (freeTimes > 0)
		{
			PVPMessage::Get()->sendSpeedUpReq(10800, 1);
		}
		else if (cardCount > 0)
		{
			PVPMessage::Get()->sendSpeedUpReq(10800, 2);
		}
        else if (cardCount < 1 && freeTimes == 0)
        {
            unsigned int diamond = num;
            int userDiamond = UserData::GetDiamond();
            if (userDiamond < diamond)
            {
                ShowMessage(Localizatioin::getLocalization("M_SHOP_DIAMOND"));
                return;
            }
			PVPMessage::Get()->sendSpeedUpReq(10800, 3);
        }
    }
}

void MysteryLayer::InitAttrib()
{
    int cardCount = LuaTinkerManager::Get()->callLuaFunc<int>("Script/main.lua", "GetRefreshCardCount");
    m_cardLabel->setString(CCString::createWithFormat("%d", cardCount)->getCString());

    USERINFO userInfo = UserData::GetUserInfo();
    m_hunLabel->setString(CCString::createWithFormat("%d", userInfo.m_soulStone)->getCString());
    m_diamond->setString(CCString::createWithFormat("%d", UserData::GetDiamond())->getCString());

    // 按钮上文字
    UpdateTimes();
}

void MysteryLayer::InitTime(float f)
{
    USERINFO userInfo = UserData::GetUserInfo();
    long time = userInfo.m_shopRefreshTime;
    long nowTime = TimeManager::Get()->getCurServerTime();
    if (nowTime >= time)
    {
        m_timeLabel->setString("");
    } else
    {
        CCString* pStr = CCString::createWithFormat("%s %s", Localizatioin::getLocalization("M_SHOP_UPDATETIME"), GetTimeStr(time - nowTime).c_str());
        m_timeLabel->setString(pStr->getCString());
    }

    int times = userInfo.m_refreshTimes;
    int maxTimes = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", UserData::GetVipLevel(), "Shop_Number2");
    if (times == maxTimes)
    {
        m_timeLabel->setString(CCString::create(Localizatioin::getLocalization("M_SHOP_UPDATETIME1"))->getCString());
    }
}

void MysteryLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_ATTR_SP_DIAMOND_UPDATE :
        {
            InitAttrib();
            break;
        }
    
    case GM_ATTR_SP_SOUL_STONE_UPDATE :
        {
            InitAttrib();
            break;
        }

    case GM_UPDATE_SHOP_REFRESH_TIMES :
        {
            UpdateTimes();
            break;
        }

    default:
        break;
    }
}

void MysteryLayer::UpdateTimes()
{
    USERINFO userInfo = UserData::GetUserInfo();
    int times = userInfo.m_refreshTimes;

    if (times == 0)
    {
        m_refreshbtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_SHOP_REFRESH2")), CCControlStateNormal);
    } else
    {
        m_refreshbtn->setTitleForState(CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_REFRESH1"), times), CCControlStateNormal);
    }

    int maxTimes = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", UserData::GetVipLevel(), "Shop_Number2");
    if (times == maxTimes)
    {
        m_timeLabel->setString(CCString::create(Localizatioin::getLocalization("M_SHOP_UPDATETIME1"))->getCString());
    }

    if (times < 10)
    {
        m_mysteryNote->setVisible(false);
    } else
    {
        m_mysteryNote->setVisible(true);
    }
}

std::string MysteryLayer::GetTimeStr(long _time)
{
    char timeString[30];
    unsigned int hour = _time / 3600;
    unsigned int min = (_time - hour * 3600) / 60;
    unsigned int second = _time % 60;
    
    sprintf(timeString,"%02d : %02d : %02d",hour,min,second);
    return std::string(timeString);
}

void MysteryLayer::InitLayer(std::vector<shopItem> vtItems)
{
    m_shopItems = vtItems;
    RefreshItemList();
}

void MysteryLayer::buyItem(int index, bool suc)
{
    if (suc == false)
    {
        return;
    }

    if (m_shopList == NULL)
    {
        return;
    }

    // 找到指定的商品
    int cellIndex = index / 2;
    CCTableViewCell* cell = m_shopList->cellAtIndex(cellIndex);
    if (cell == NULL)
    {
        return;
    }

    SingleShopListLayer* layer = dynamic_cast<SingleShopListLayer*>(cell->getChildByTag(111 + index % 2));
    if (layer)
    {
        layer->getBuyBtn()->setEnabled(false);
        layer->getBuyBtn()->setTitleForState(CCString::create(Localizatioin::getLocalization("M_SHOP_ALREADYBUY")), CCControlStateDisabled);
    }

    // 给予提示
    shopItem itemInfo = m_shopItems[index];
    ItemBase* itemBaseInfo = ItemManager::Get()->getItemInfoById(itemInfo.to_id);
    CCString* pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_BUY"), itemBaseInfo->m_name);
    ShowMessage(pStr->getCString());
}

SingleShopListLayer::SingleShopListLayer() :
    m_nameLabel(NULL),
    m_priceLabel(NULL),
    m_itemFrame(NULL),
    m_costItem(NULL),
    m_buyBtn(NULL),
    m_itemBtn(NULL)
{

}

SingleShopListLayer::~SingleShopListLayer()
{
    setNameLabel(NULL);
    setPriceLabel(NULL);
    setItemFrame(NULL);
    setCostItem(NULL);
    setBuyBtn(NULL);
    setItemBtn(NULL);
	m_fairyId = 0;
}

SEL_MenuHandler SingleShopListLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleShopListLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buy",        SingleShopListLayer::onClickBuyItem);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "click_item", SingleShopListLayer::onClickedItem);
    return NULL;
}

bool SingleShopListLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name",       CCLabelTTF*,      m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "price",      CCLabelTTF*,      m_priceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_item",  CCSprite*,        m_costItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_frame", CCSprite*,        m_itemFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "buy_btn",    CCControlButton*, m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_btn",   CCControlButton*, m_itemBtn);
    return false;
}

void SingleShopListLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{

}

void SingleShopListLayer::onEnter()
{
    CCLayer::onEnter();
    m_buyBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_itemBtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

void SingleShopListLayer::InitLayer(shopItem itemInfo)
{
    m_itemInfo = itemInfo;
    int itemId = itemInfo.to_id;
    int itemCount = itemInfo.to_num;

    // 创建图片
    CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(itemId);
    if (iconSprite != NULL)
    {
        if (m_itemFrame->getParent()->getChildByTag(111))
        {
            m_itemFrame->getParent()->removeChildByTag(111);
        }

        m_itemFrame->getParent()->addChild(iconSprite, 0, 111);
        iconSprite->setPosition(m_itemFrame->getPosition());
    }

    CCString* StrCount = CCString::createWithFormat("%d", itemCount);
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

    ItemBase* itemBaseInfo = ItemManager::Get()->getItemInfoById(itemId);
    if (itemBaseInfo)
    {
        m_nameLabel->setString(itemBaseInfo->m_name);
    }

    // 价格
    CCPoint pt;
    CCSprite* money = NULL;
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    if(itemInfo.from_id == 416001) //coin sprite
    {
        int idx = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_COIN");
        money = pAsprite->getSpriteFromFrame_Middle(idx, 0, pt);
    }
    else if(itemInfo.from_id == 416009)
    {
        int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_MEDAL");
        money = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    }
    else if(itemInfo.from_id == 416018)
    {
        int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_STONE");
        money = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    }
    else
    {
        int idx = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_DIAMOND");
        money = pAsprite->getSpriteFromFrame_Middle(idx, 0, pt);
    }

    money->setAnchorPoint(m_costItem->getAnchorPoint());
    money->setPosition(m_costItem->getPosition());

    if (m_costItem->getParent()->getChildByTag(113))
    {
        m_costItem->getParent()->removeChildByTag(113);
    }

    m_costItem->getParent()->addChild(money, 0, 113);

    m_priceLabel->setString(CCString::createWithFormat("%d", itemInfo.from_num)->getCString());

    // 检查是否已经购买过了
    if (itemInfo.isBuy == true)
    {
        m_buyBtn->setEnabled(false);
        m_buyBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_SHOP_ALREADYBUY")), CCControlStateDisabled);
    } else
    {
        m_buyBtn->setEnabled(true);
        m_buyBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_DIAMONDSHOP_BUY")), CCControlStateNormal);
    }
}

void SingleShopListLayer::onClickBuyItem(CCObject* object, CCControlEvent event)
{
    int cost = m_itemInfo.from_num;
    if (m_itemInfo.from_id == 416002 &&
        UserData::GetDiamond() < cost)
    {
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",cost);
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
        return;
    }

    // 检查是否是魂石
    if (m_itemInfo.from_id == 416018)
    {
        USERINFO userInfo = UserData::GetUserInfo();
        if (cost > userInfo.m_soulStone)
        {
            // 弹出提示框
            MessageBox::Show(Localizatioin::getLocalization("M_SHOP_STONE"), this, menu_selector(SingleShopListLayer::OnSelectDecompose), 
                menu_selector(SingleShopListLayer::OnSelectCancel),MB_OKCANCELCLOSE);
            MessageBox::AdjustStyle(Localizatioin::getLocalization("M_DECOMPOSE"), Localizatioin::getLocalization("M_CANCEL"));
            MessageBox::showCloseBtn();
        }
    }

    MarketManager::Get()->buyShopItem(m_itemInfo.index);
}

void SingleShopListLayer::OnSelectDecompose(CCObject* sender)
{
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua", "G_OpenFairyDecomposeLayer");

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(false);
    }
}

void SingleShopListLayer::OnSelectCancel(CCObject* sender)
{

}

void SingleShopListLayer::onGetWayCallBack(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyGetwayLayer",m_fairyId);
}

void SingleShopListLayer::onFairyDetailClicked(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyInfoLayerById",m_fairyId,0);
}

void SingleShopListLayer::onClickedItem(CCObject* object, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(object);
    if (node == NULL)
    {
        return;
    }

    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(m_itemInfo.to_id, pt);

	bool isFairy = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Fairy/FairyLuaInterface.lua", "G_GetIsFairyById", m_itemInfo.to_id);
	ItemBase * info = ItemManager::Get()->getItemInfoById(m_itemInfo.to_id);
	if (isFairy || info->m_type == 21)
	{
		m_fairyId = m_itemInfo.to_id % 10000;
		const char * detailText = Localizatioin::getLocalization("M_EQUIPMENT_INFORMATION");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleShopListLayer::onFairyDetailClicked),detailText,true,NULL,NULL,"",false);
	}

	/*EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(SingleShopListLayer::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
	}*/

}
