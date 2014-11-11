#include "OnlineNetworkManager.h"
#include "ChargeLayer.h"
#include "ShopItemInfoFromServer.h"
#include "UIDefine.h"
#include "Localization.h"
#include "AspriteManager.h"
#include "TXGUI.h"
#include "MainMenuLayer.h"
#include "UserData.h"
#include "InterfaceManager.h"
#include "WaitingLayerManager.h"
#include "ItemManager.h"
#include "LuaTinkerManager.h"
#include "PayManager.h"
#include "UIManager.h"
#include "GMessage.h"
#include "MonthCardManager.h"
#include <algorithm>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NcSDKManager.h"
#include "IAPManager.h"
#endif

#define MAX_VIP_LEVEL          15

ChargeLayer::ChargeLayer() :
    m_chargeList(NULL),
    m_viewLayer(NULL),
    m_closeBtn(NULL),
    m_currentDiamond(NULL),
    m_currentVipLv(NULL),
    m_nextVipLv(NULL),
    m_currentVipLv1(NULL),
    m_nextVipLv1(NULL),
    m_nextDiamond(NULL),
    m_nextLayer(NULL),
    m_processSprite(NULL),
    m_vipContentLayer(NULL),
    m_vipContentList(NULL),
    m_sonVipLayer(NULL),
    m_sonChargeLayer(NULL),
    m_currentVipIndex(0),
    m_startPt(CCPointZero),
    m_btn(NULL),
    m_buyBtn(NULL),
    m_giftLabel(NULL),
    m_leftBtn(NULL),
    m_rightBtn(NULL),
    m_currentOpenTag(0)
{
    m_vtShopInfo.clear();
    m_vtGiftLayers.clear();
    m_vtGiftLayers.resize(4);
}

ChargeLayer::~ChargeLayer()
{
    setChargeList(NULL);
    setCloseBtn(NULL);
    setViewLayer(NULL);
    setCurrentDiamond(NULL);
    setCurrentVipLv(NULL);
    setNextVipLv(NULL);
    setCurrentVipLv1(NULL);
    setNextVipLv1(NULL);
    setNextDiamond(NULL);
    setNextLayer(NULL);
    setProcessSprite(NULL);
    setVipContentLayer(NULL);
    setVipContentList(NULL);
    setSonChargeLayer(NULL);
    setSonVipLayer(NULL);
    setBtn(NULL);
    setBuyBtn(NULL);
    setGiftLabel(NULL);
    setLeftBtn(NULL);
    setRightBtn(NULL);

    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler ChargeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler ChargeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",    ChargeLayer::onClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btn",      ChargeLayer::onClickedBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "buy_gift", ChargeLayer::onBuyBtnClicked);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_1", ChargeLayer::onSelectItem1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_2", ChargeLayer::onSelectItem2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_3", ChargeLayer::onSelectItem3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_4", ChargeLayer::onSelectItem4);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "vip_left",  ChargeLayer::onClickLeft);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "vip_right", ChargeLayer::onClickRight);

    return NULL;
}

bool ChargeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "view_layer",      CCLayer*,     m_viewLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",       CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn",             CCControlButton*, m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "current_diamond", CCLabelTTF*,  m_currentDiamond);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_amount",     CCLabelTTF*,  m_nextDiamond);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_level",       CCSprite*,    m_currentVipLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_level1",      CCSprite*,    m_currentVipLv1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_vip_level",  CCSprite*,    m_nextVipLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_vip_level1", CCSprite*,    m_nextVipLv1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_layer",      CCLayer*,     m_nextLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "process",         CCSprite*,    m_processSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_content",     CCLayer*,     m_vipContentLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "buy_btn",         CCControlButton*, m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gift_label",      CCLabelTTF*,  m_giftLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_left_btn",    CCControlButton*, m_leftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_right_btn",   CCControlButton*, m_rightBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "son_vip",         CCLayer*,     m_sonVipLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "son_charge",      CCLayer*,     m_sonChargeLayer);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item1",           CCLayer*,     m_vtGiftLayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item2",           CCLayer*,     m_vtGiftLayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item3",           CCLayer*,     m_vtGiftLayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item4",           CCLayer*,     m_vtGiftLayers[3]);
    return false;
}

void ChargeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    setPosition(ccp(s.width / 2, s.height / 2));
    setTouchEnabled(true);

    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
    InitLayer();

    initLayerOpen(m_currentVipIndex);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    m_rightBtn->setRotationY(180.0f);
    
#ifdef TargetForCoCoAppStore
    IAPManager::CloseUnConfirmTransactions();
#endif
}

CCSize ChargeLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(920.0f, 130.0f);
}

CCTableViewCell* ChargeLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int ChargeLayer::numberOfCellsInTableView(CCTableView *table)
{
    return (m_vtShopInfo.size() + 1) / 2;
}

void ChargeLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

bool ChargeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_startPt = pTouch->getLocation();
    return true;
}

void ChargeLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint pt = pTouch->getLocation();

    if (m_sonVipLayer->isVisible() == true)
    {
        // 尝试刷新界面
        AdjustWindow(pTouch);

    } else if (m_sonChargeLayer->isVisible() == true)
    {
        if (m_chargeList == NULL)
        {
            return;
        }

        // 查询当前点击的是哪个cell
        for (unsigned int i = 0; i < numberOfCellsInTableView(m_chargeList); ++i)
        {
            CCTableViewCell* cell = m_chargeList->cellAtIndex(i);

            if (cell == NULL)
            {
                continue;
            }

            SingleChargeLayer* layer = dynamic_cast<SingleChargeLayer*>(cell->getChildByTag(111));
            if (layer != NULL)
            {
                CCSize size = layer->getContentSize();
                CCPoint dexPt = layer->convertToNodeSpace(pt);
                CCRect rect = CCRectMake(0, 0, size.width, size.height);
                if (rect.containsPoint(dexPt))
                {
                    layer->pay();
                }
            }

            layer = dynamic_cast<SingleChargeLayer*>(cell->getChildByTag(112));
            if (layer != NULL)
            {
                CCSize size = layer->getContentSize();
                CCPoint dexPt = layer->convertToNodeSpace(pt);
                CCRect rect = CCRectMake(0, 0, size.width, size.height);
                if (rect.containsPoint(dexPt))
                {
                    layer->pay();
                }
            }
        }
    }
}

void ChargeLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerMenuTouchPriority - 1, true);
}

void ChargeLayer::onEnter()
{
    CCLayer::onEnter();

    m_closeBtn->setTouchPriority(kCCLayerMenuTouchPriority - 3);
    m_btn->setTouchPriority(kCCLayerMenuTouchPriority - 3);
    m_buyBtn->setTouchPriority(kCCLayerMenuTouchPriority - 3);
    m_leftBtn->setTouchPriority(kCCLayerMenuTouchPriority - 3);
    m_rightBtn->setTouchPriority(kCCLayerMenuTouchPriority - 3);

    for (size_t i = 0; i < m_vtGiftLayers.size(); ++i)
    {
        CCNode* node = m_vtGiftLayers[i]->getChildByTag(1);
        if (node == NULL)
        {
            continue;
        }

        CCControlButton* button = dynamic_cast<CCControlButton*>(node);
        if (button == NULL)
        {
            continue;
        }

        button->setTouchPriority(kCCLayerMenuTouchPriority - 3);
    }
}

void ChargeLayer::onClose(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowChargeLayer(false);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::setIsInPurchasing(false);
#endif
}

void ChargeLayer::onClickedBtn(CCObject* object, CCControlEvent event)
{
    if (m_currentOpenTag == 0)
    {
        m_currentOpenTag = 1;
    } else
    {
        m_currentOpenTag = 0;
    }

    initLayerOpen(m_currentOpenTag);
}

void ChargeLayer::onBuyBtnClicked(CCObject* object, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(true, 2);
        mLayer->ShowChargeLayer(false);
    }
}

void ChargeLayer::onSelectItem1(CCObject* object, CCControlEvent event)
{
    selectItem(0, object);
}

void ChargeLayer::onSelectItem2(CCObject* object, CCControlEvent event)
{
    selectItem(1, object);
}

void ChargeLayer::onSelectItem3(CCObject* object, CCControlEvent event)
{
    selectItem(2, object);
}

void ChargeLayer::onSelectItem4(CCObject* object, CCControlEvent event)
{
    selectItem(3, object);
}

void ChargeLayer::onClickLeft(CCObject* object, CCControlEvent event)
{
    if (m_vipContentList == NULL)
    {
        return;
    }

    AdjustWindowTo(m_currentVipIndex - 1);
}

void ChargeLayer::onClickRight(CCObject* object, CCControlEvent event)
{
    if (m_vipContentList == NULL)
    {
        return;
    }

    AdjustWindowTo(m_currentVipIndex + 1);
}

void ChargeLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
        case GM_ATTR_DIAMOND_ACCU :
            {
                InitLayer();
                break;
            }

        case GM_ATTR_SP_DIAMOND_UPDATE :
        case GM_ATTR_VIP_LEVEL_CHANGE :
            {
                InitLayer();
                updateChargeList();
                break;
            }

        default:
            break;
    }
}

void ChargeLayer::updateChargeList()
{
    if (m_chargeList != NULL)
    {
        ShopItemInfoFromServer::sharedInstance()->PrepardData();
        ShopItemInfoFromServer::sharedInstance()->PrepareShopData();
        ParseShopData();
        RefreshChargeList();
    }
}

void ChargeLayer::initLayerOpen(int openTag)
{
    m_currentOpenTag = openTag;
    if (openTag == 0)
    {
        m_sonVipLayer->setVisible(false);
        m_sonChargeLayer->setVisible(true);
        m_btn->setTitleForState(CCString::createWithFormat(Localizatioin::getLocalization("M_VIP_RIGHT")), CCControlStateNormal);

        if (m_chargeList == NULL)
        {
            ShopItemInfoFromServer::sharedInstance()->PrepardData();
            ShopItemInfoFromServer::sharedInstance()->PrepareShopData();
            ParseShopData();
            RefreshChargeList();
        }
    } else
    {
        m_sonVipLayer->setVisible(true);
        m_sonChargeLayer->setVisible(false);
        m_btn->setTitleForState(CCString::createWithFormat(Localizatioin::getLocalization("M_RECHARGE")), CCControlStateNormal);

        if (m_vipContentList == NULL)
        {
            RefreshVipList();
        }
    }
}

void ChargeLayer::selectItem(int index, CCObject* object)
{
    CCControlButton* button = dynamic_cast<CCControlButton*>(object);
    if (button == NULL)
    {
        return;
    }

    if (m_sonVipLayer->isVisible() == false)
    {
        return;
    }

    if (m_vtItemsInfo.size() <= index)
    {
        return;
    }

    int itemId = m_vtItemsInfo[index].item_id;
    CCPoint pt = button->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(itemId, pt);
}

void ChargeLayer::InitLayer()
{
    m_currentDiamond->setString(CCString::createWithFormat("%d", UserData::GetDiamond())->getCString());

    int vip_lv = UserData::GetVipLevel();
    int next_vip_lv = vip_lv + 1;

    if (vip_lv < 10)
    {
        m_currentVipLv->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", 
            CCString::createWithFormat("rank_level%d.png", vip_lv)->getCString()));
    } else
    {
        m_currentVipLv->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", 
            CCString::createWithFormat("rank_level%d.png", vip_lv / 10)->getCString()));
        m_currentVipLv1->setVisible(true);
        m_currentVipLv1->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", 
            CCString::createWithFormat("rank_level%d.png", vip_lv % 10)->getCString()));
    }

    if (next_vip_lv < 10)
    {
        m_nextVipLv->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", 
            CCString::createWithFormat("rank_level%d.png", next_vip_lv)->getCString()));
    } else
    {
        m_nextVipLv->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", 
            CCString::createWithFormat("rank_level%d.png", next_vip_lv / 10)->getCString()));
        m_nextVipLv1->setVisible(true);
        m_nextVipLv1->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", 
            CCString::createWithFormat("rank_level%d.png", next_vip_lv % 10)->getCString()));
    }

    if (next_vip_lv > MAX_VIP_LEVEL)
    {
        m_nextLayer->setVisible(false);
        m_processSprite->setScaleX(0.0f);
    } else
    {
        // 已充值金额
        USERINFO info = UserData::GetUserInfo();
        int diamond_acc = info.m_diamond_acc;

        // 获取下一级VIP需要的充值金额
        int totalAmount = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", next_vip_lv, "Diamond");
        int currentAmount = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", vip_lv, "Diamond");

        m_nextDiamond->setString(CCString::createWithFormat("%d", totalAmount - diamond_acc)->getCString());

        // 设置比例
        float f = ((float) diamond_acc - (float) currentAmount) / (totalAmount - currentAmount);
        if (f >= 1.0f)
        {
            f = 1.0f;
        }

        if (f <= 0.0f)
        {
            f = 0.0f;
        }

        m_processSprite->setScaleX(f);
    }
}

void ChargeLayer::RefreshChargeList()
{
    if (m_chargeList == NULL)
    {
        setChargeList(CCTableView::create(this, m_viewLayer->getContentSize()));
        m_chargeList->setDirection(kCCScrollViewDirectionVertical);
        m_chargeList->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_viewLayer->addChild(m_chargeList);
        m_chargeList->setTouchPriority(kCCLayerMenuTouchPriority - 2);
        m_chargeList->setDelegate(this);
		m_chargeList->setForbidTouched(true);
        m_chargeList->setBounceable(false);
    } else
    {
        m_chargeList->reloadData();
    }
}

void ChargeLayer::RefreshVipList()
{
    if (m_vipContentList == NULL)
    {
        VipContentDataSource* data = new VipContentDataSource();
        setVipContentList(CCTableView::create(data, CCSizeMake(500.0f, 350.0f)));
        m_vipContentList->setDirection(kCCScrollViewDirectionHorizontal);
        m_vipContentLayer->addChild(m_vipContentList);
        m_vipContentList->setTouchPriority(kCCLayerMenuTouchPriority - 2);
        m_vipContentList->setDelegate(this);
        m_vipContentList->setBounceable(false);
        m_vipContentList->reloadData();
    } else
    {
        m_vipContentList->reloadData();
    }

    int userLevel = UserData::GetVipLevel();
    if (userLevel < 1)
    {
        userLevel = 1;
    }

    AdjustWindowTo(userLevel - 1, false);
}

void ChargeLayer::AdjustWindow(CCTouch* pTouch)
{
    if (m_vipContentList == NULL)
    {
        return;
    }

    CCPoint postPt = pTouch->getLocation();
    if (postPt.x - m_startPt.x > 120)
    {
        AdjustWindowTo(m_currentVipIndex - 1);
    } else if (postPt.x - m_startPt.x < -120)
    {
        AdjustWindowTo(m_currentVipIndex + 1);
    } else
    {
        AdjustWindowTo(m_currentVipIndex);
    }
}

void ChargeLayer::AdjustWindowTo(int index, bool flag)
{
    if (index < 0 ||
        index > MAX_VIP_LEVEL - 1)
    {
        return;
    }

    m_vipContentList->unscheduleAllSelectors();
    m_vipContentList->stopAllActions();
    m_vipContentList->getContainer()->stopAllActions();
    setCurrentVipIndex(index);
    if (index == MAX_VIP_LEVEL - 1)
    {
        m_rightBtn->setEnabled(false);
    } else
    {
        m_rightBtn->setEnabled(true);
    }

    if (index == 0)
    {
        m_leftBtn->setEnabled(false);
    } else
    {
        m_leftBtn->setEnabled(true);
    }
    
    float fx = - 500.0f * index;
    
    m_vipContentList->setContentOffset(ccp(fx, 0), flag);

    // 更改VIP礼包配置
    m_giftLabel->setString(CCString::createWithFormat("VIP%d - %s", index + 1, Localizatioin::getLocalization("M_VIP_GIFT"))->getCString());
    std::string strItems = std::string(LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/GameConfig/ItemConfigTab.lua","getVipBagItemList", index + 1));
    m_vtItemsInfo.clear();
    ParseGiftItem(strItems, m_vtItemsInfo);

    for (size_t i = 0; i < m_vtGiftLayers.size(); ++i)
    {
        if (m_vtGiftLayers[i]->getChildByTag(111))
            m_vtGiftLayers[i]->removeChildByTag(111);

        if (m_vtGiftLayers[i]->getChildByTag(112))
            m_vtGiftLayers[i]->removeChildByTag(112);

        if (i < m_vtItemsInfo.size())
        {
            m_vtGiftLayers[i]->setVisible(true);
            CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(m_vtItemsInfo[i].item_id);
            m_vtGiftLayers[i]->addChild(iconSprite, 0, 111);
            iconSprite->setAnchorPoint(ccp(0.5f, 0.5f));
            CCSize s = m_vtGiftLayers[i]->getContentSize();
            iconSprite->setPosition(ccp(s.width / 2, s.height / 2));

            CCLabelTTF* amountLabel = CCLabelTTF::create(CCString::createWithFormat("%d", m_vtItemsInfo[i].amount)->getCString(),
                "Helvetica", 20.0f);

            amountLabel->setAnchorPoint(ccp(1.0f, 0.0f));
            m_vtGiftLayers[i]->addChild(amountLabel, 0, 112);
            amountLabel->setPosition(ccp(s.width - 10, 10));

        } else
        {
            m_vtGiftLayers[i]->setVisible(false);
        }
    }
}

void ChargeLayer::ParseGiftItem(std::string strItems, std::vector<GiftBonusInfo>& vtGifts)
{
    vtGifts.clear();
    if (strItems.size() < 6)
    {
        return;
    }

    unsigned int index = 0;
    do 
    {
        std::string itemStr;
        unsigned newIndex = strItems.substr(index, strItems.size() - index).find_first_of(';');
        if (newIndex == -1)
        {
            itemStr = strItems.substr(index, strItems.size() - index);
        } else
        {
            itemStr = strItems.substr(index, newIndex);
        }

        unsigned int pot = itemStr.find_first_of('/');
        GiftBonusInfo info;
        if (pot == -1)
        {
            info.item_id = atoi(itemStr.c_str());
            info.amount = 1;
        } else
        {
            info.item_id = atoi(itemStr.substr(0, pot).c_str());
            info.amount = atoi(itemStr.substr(pot + 1, itemStr.size() - pot - 1).c_str());
        }

        vtGifts.push_back(info);

        if (newIndex == -1)
        {
            break;
        } else
        {
            index = newIndex + 1 + index;
        }


    } while (1);
}

void ChargeLayer::ParseShopData()
{
    m_vtShopInfo.clear();
    std::map<unsigned int, OneShopItemInfo> totalShopInfo = ShopItemInfoFromServer::sharedInstance()->getShopInfo();

    for (std::map<unsigned int, OneShopItemInfo>::iterator it = totalShopInfo.begin();
         it != totalShopInfo.end(); ++it)
    {
        m_vtShopInfo.push_back(it->second);
    }

	std::sort(m_vtShopInfo.begin(),m_vtShopInfo.end(),ShopItemSortFunc);
}

void ChargeLayer::RefreshCell(CCTableViewCell* cell, unsigned int idx)
{
    if (cell->getChildByTag(111))
        cell->removeChildByTag(111);

    if (cell->getChildByTag(112))
        cell->removeChildByTag(112);

    CCLayer* firstLayer = CreateSingleChargeLayer(2 * idx);
    cell->addChild(firstLayer, 0, 111);
    firstLayer->setPosition(ccp(5, 8));

    if (m_vtShopInfo.size() >= 2 * idx + 2)
    {
        CCLayer* secondLayer = CreateSingleChargeLayer(2 * idx + 1);
        cell->addChild(secondLayer, 0, 112);
        secondLayer->setPosition(ccp(465, 8));
    }
}

CCLayer* ChargeLayer::CreateSingleChargeLayer(unsigned int index)
{
    if (m_vtShopInfo.size() <= index)
    {
        return NULL;
    }

    CCNodeLoader* loader = new SingleChargeLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_charge", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_charge");
    SingleChargeLayer* layer = dynamic_cast<SingleChargeLayer*>(node);
    layer->InitLayer(m_vtShopInfo[index], index);
    return layer;
}

SingleChargeLayer::SingleChargeLayer() :
    m_recommandSprite(NULL),
	m_hotSprite(NULL),
    m_label1(NULL),
    m_label2(NULL),
    m_label3(NULL),
	m_label4(NULL),
    m_itemFrame(NULL),
    m_btn(NULL),
    m_productId(0),
    m_index(-1),
	m_productType(0),
	m_MCardTotalDiamond(0)
{
	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
}

SingleChargeLayer::~SingleChargeLayer()
{
    setRecommandSprite(NULL);
	setHotSprite(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
	setLabel4(NULL);
    setItemFrame(NULL);
    setBtn(NULL);
	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler SingleChargeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleChargeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool SingleChargeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "recommand_sprite", CCSprite*,        m_recommandSprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "hot_sprite",		  CCSprite*,        m_hotSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label1",           CCLabelTTF*,      m_label1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label2",           CCLabelTTF*,      m_label2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label3",           CCLabelTTF*,      m_label3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label4",           CCLabelTTF*,      m_label4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item_frame",       CCSprite*,        m_itemFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "click_btn",        CCControlButton*, m_btn);
    return false;
}

void SingleChargeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{

}

void SingleChargeLayer::onEnter()
{
    m_btn->setTouchPriority(kCCLayerMenuTouchPriority - 3);
}

void SingleChargeLayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_MONTHCARD_GET_SUCCESS:
		if(this->m_productType == 1)
		{
			refreshMouthCardInfo();
		}
		break;
	default:
		break;
	}
}

void SingleChargeLayer::refreshMouthCardInfo()
{
	bool hasBuyCard = false;

	MonthCardInfo cardInfo;
	if(MonthCardManager::Get()->getMonthCardInfoByID(cardInfo,m_productId))
	{
		if(!cardInfo.is_alarm)
		{
			hasBuyCard = true;
			if(cardInfo.is_received)
			{
				m_label4->setVisible(true);
				m_label4->setString(Localizatioin::getLocalization("M_LOGINREWARD_ALREADYGOT"));
				m_label4->setColor(ccc3(93,93,93));
			}
			else
			{
				m_label4->setVisible(true);
				m_label4->setString(Localizatioin::getLocalization("M_MONYHCARD_GET"));
				m_label4->setColor(ccc3(187,12,181));
			}
		}
	}
	if(hasBuyCard)
	{
		
		m_label2->setString(CCString::createWithFormat("%s%d",Localizatioin::getLocalization("M_MONTHCARD_LASTDAY"), cardInfo.day_count)->getCString());
		m_label2->setColor(ccc3(187,12,181));
	}
	else
	{
		m_label2->setString(CCString::createWithFormat(Localizatioin::getLocalization("M_MONTHCARD_TEXT"),m_MCardTotalDiamond)->getCString());
		m_label2->setColor(ccc3(187,12,181));
	}
	
}

void SingleChargeLayer::showNormalProduct(OneShopItemInfo info)
{
	m_label1->setString(CCString::createWithFormat("%d%s", info.item_base_num, Localizatioin::getLocalization("S_Diamond_content"))->getCString());
    
	
	USERINFO userInfo = UserData::GetUserInfo();
	std::map<int, int> mapShopBuyInfo = ShopItemInfoFromServer::sharedInstance()->getShopBuyInfo();
    bool result = false;
    if (info.is_command == true)
    {
        if (mapShopBuyInfo.find(info.product_id) == mapShopBuyInfo.end() ||
            mapShopBuyInfo[info.product_id] == 0)
        {
            result = true;
        }
    }

    // 再计算送的金币数
    int giftAmount = 0;
    CCString* pStrWords = NULL;
    if (result == true)
    {
        // 多倍
        giftAmount = info.item_base_num * (((float)info.add_times / 100) - 1);
        CCString* strWords1 = CCString::createWithFormat(Localizatioin::getLocalization("M_GIVE_DIAMONDS"), giftAmount);
        CCString* strWords2 = CCString::createWithFormat(Localizatioin::getLocalization("M_FIRSTSPEND_TEXT"), ((float)info.add_times / 100));
    
        pStrWords = CCString::createWithFormat("%s%s", strWords1->getCString(), strWords2->getCString());
    } else
    {
        giftAmount = info.item_gift_num;
        pStrWords = CCString::createWithFormat(Localizatioin::getLocalization("M_GIVE_DIAMONDS"), giftAmount);
    }

    m_label2->setString(pStrWords->getCString());
}

void SingleChargeLayer::showMouthCardProduct(OneShopItemInfo info)
{
	/*MonthCardInfo cardInfo;
	if(MonthCardManager::Get()->getMonthCardInfoByID(cardInfo,m_productId))
	{*/
		unsigned int per_day_diamond = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("monthcard","month_card",m_productId,"per_day_diamond");
		unsigned int days = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("monthcard","month_card",m_productId,"days");
		//unsigned int total_diamond = per_day_diamond * days + info.item_base_num;
		//setMCardTotalDiamond(total_diamond);
		setMCardTotalDiamond(info.item_base_num);
	//}
	
	m_label1->setString(info.product_name.c_str());
	refreshMouthCardInfo();
	
}

void SingleChargeLayer::InitLayer(OneShopItemInfo info, unsigned int index)
{
    setIndex(index);
    setProductId(info.product_id);
	setProductType(info.product_type);
	
    m_recommandSprite->setVisible(false);
	m_hotSprite->setVisible(false);
	m_label4->setVisible(false);

    if (index > 5)
    {
        index = 5;
    }
    
	m_label3->setString(CCString::createWithFormat("%.2f%s", ((float) info.product_price) / 100, Localizatioin::getLocalization("M_YUAN"))->getCString());
	std::string str = info.resource_name;
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	std::string IconName = CCString::createWithFormat("map_ui_system_icon_FRAME_%s", str.c_str())->m_sString;
	//std::string IconName = CCString::createWithFormat("map_ui_system_icon_FRAME_ICON_DIAMOND%d", index + 1)->m_sString;
    CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN, IconName);

	if(sprite)
	{
		 addChild(sprite);
		 sprite->setAnchorPoint(ccp(0.5, 0.5));
		 sprite->setPosition(m_itemFrame->getPosition());
	}

    // 决定是否为推荐
    
    if (info.is_command == 1)
    {
        m_recommandSprite->setVisible(true);
		m_hotSprite->setVisible(false);
		if(info.product_type == 0)
		{
			m_label2->setColor(ccc3(187, 12, 181));
		}
    } 
	else if(info.is_command == 2)
	{
		m_recommandSprite->setVisible(false);
		m_hotSprite->setVisible(true);
	}
	else
    {
        m_recommandSprite->setVisible(false);
		m_hotSprite->setVisible(false);
    }

	if(m_productType == 0)
	{
		showNormalProduct(info);
	}
	else
	{
		showMouthCardProduct(info);
	}
    
}

void SingleChargeLayer::pay()
{
	if(m_productType == 1)
	{
		MonthCardInfo cardInfo;
		if(MonthCardManager::Get()->getMonthCardInfoByID(cardInfo,m_productId) && !cardInfo.is_alarm)
		{
			if(!cardInfo.is_received)
			{
				OnlineNetworkManager::sShareInstance()->sendMonthCardGetReq(m_productId);
			}
		}
		else
		{
			// to buy
			PayManager::Get()->payForProduct(m_index, m_productId);
		}
	}
	else
	{
		PayManager::Get()->payForProduct(m_index, m_productId);
	}
    
}

VipContentLayer::VipContentLayer() :
    m_vipLv1(NULL),
    m_vipLv2(NULL),
    m_label(NULL)
{

}

VipContentLayer::~VipContentLayer()
{
    setVipLv1(NULL);
    setVipLv2(NULL);
    setLabel(NULL);
}

SEL_MenuHandler VipContentLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler VipContentLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool VipContentLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_lv_1", CCSprite*,   m_vipLv1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_lv_2", CCSprite*,   m_vipLv2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label",    CCLabelTTF*, m_label);
    return false;
}

void VipContentLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{

}

void VipContentLayer::InitLayer(unsigned int index)
{
    m_label->setVisible(false);
    
    int id = 1;
    do 
    {
        CCString* pStr = CCString::createWithFormat("description_vip%d_%d", index + 1, id);
        std::string desc = std::string(Localizatioin::getLocalization(pStr->getCString()));
        if (strcmp(desc.c_str(), "") == 0)
        {
            break;
        }

        CCString* descStr = CCString::createWithFormat("%d.%s", id, desc.c_str());

        CCLabelTTF* label = CCLabelTTF::create(descStr->getCString(), m_label->getFontName(), m_label->getFontSize());
        addChild(label);
        label->setAnchorPoint(ccp(0.0, 1.0f));
        label->setPosition(ccp(m_label->getPosition().x, m_label->getPosition().y - (id - 1) * 22));
        id++;
    } while (1);

    if (index < 9)
    {
        m_vipLv1->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/vip.plist", CCString::createWithFormat("vip_%d.png", index + 1)->getCString()));
        m_vipLv2->setVisible(false);
    } else
    {
        m_vipLv1->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/vip.plist", CCString::createWithFormat("vip_%d.png", 1)->getCString()));
        m_vipLv2->setDisplayFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/vip.plist", CCString::createWithFormat("vip_%d.png", index - 9)->getCString()));
        m_vipLv2->setVisible(true);
    }
}

VipContentDataSource::VipContentDataSource()
{

}

VipContentDataSource::~VipContentDataSource()
{

}

CCSize VipContentDataSource::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(500, 350);
}

CCTableViewCell* VipContentDataSource::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int VipContentDataSource::numberOfCellsInTableView(CCTableView *table)
{
    return MAX_VIP_LEVEL;
}

void VipContentDataSource::RefreshCell(CCTableViewCell* cell, unsigned int index)
{
    if (cell->getChildByTag(111))
        cell->removeChildByTag(111);

    VipContentLayer* layer = CreateSingleVipContentLayer(index);

    cell->addChild(layer, 0, 111);
}

VipContentLayer* VipContentDataSource::CreateSingleVipContentLayer(unsigned int index)
{
    CCNodeLoader* loader = new VipContentLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("vip_content", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/vip_content");
    VipContentLayer* layer = dynamic_cast<VipContentLayer*>(node);
    layer->InitLayer(index);
    return layer;
}