#include "OnlineNetworkManager.h"
#include "CardUILayer.h"
#include "MainMenuLayer.h"
#include "GameFontManager.h"
//#include "ItemManager.h"
#include "DiamondWarningLayer.h"
#include "UserData.h"
#include "GMessage.h"
#include "Localization.h"
#include "LuaTinkerManager.h"
#include "TipMgr.h"
#include "TutorialsManager.h"
#include "GameResourceManager.h"
#include "MainMenuLayer.h"
#include "CardAttributionLayer.h"
#include "MultiCardBuyLayer.h"

#define CARD_SHOP_COLUMN 4
#define CARD_SHOP_ROW 2
#define CARD_SHOP_PAGE (CARD_SHOP_COLUMN * CARD_SHOP_ROW)

CardUILayer::CardUILayer()
{
	m_uiLayout = NULL;
	m_constellationIndex = 0;
	m_selectedListButton = NULL;
	m_mixLayer = NULL;
	m_currentShopIndex = 0;
	initShopFlag = false;
	m_bagItemPos = 0;
	m_shopItemPos = 0;
}

CardUILayer::~CardUILayer()
{
	UIManager::sharedManager()->RemoveUILayout("cardUILayout");
	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

bool CardUILayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
	m_uiLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/cardUI.plist", this, "cardUILayout");
	m_bagScrollPage = m_uiLayout->FindChildObjectByName<UIScrollPage>("bagScrollPage");

	m_mixLayer = CardMixLayer::create();
	this->addChild(m_mixLayer,2);

	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::reOrderBagItems",
		menu_selector(CardUILayer::reOrderBagItems), this);

	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onCloseBtClicked",
		menu_selector(CardUILayer::onCloseBtClicked), this);

	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onConstellationItemClicked",
		menu_selector(CardUILayer::onConstellationItemClicked), this);
	/*UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onBagItemDoubleClicked",
		menu_selector(CardUILayer::onBagItemDoubleClicked), this);*/
	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onShopBtClicked",
		menu_selector(CardUILayer::onShopBtClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onEquipCardBtClicked",
		menu_selector(CardUILayer::onEquipCardBtClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onHelpButtonClicked",
		menu_selector(CardUILayer::onHelpButtonClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onBagItemClicked",
		menu_selector(CardUILayer::onBagItemClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onShopItemCLicked",
		menu_selector(CardUILayer::onShopItemCLicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("cardUILayout","CardUILayer::onCardAttrPanelBtClicked",
		menu_selector(CardUILayer::onCardAttrPanelBtClicked), this);
	
	UIPicture* starPic = m_uiLayout->FindChildObjectByName<UIPicture>("starPic");
	if(starPic)
	{
		const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("UI/ui_pic_card.png");
		CCSprite* sprite = CCSprite::create(path);
		if(sprite)
		{
			starPic->setSprite(sprite);
		}
	}

	showDiamondLabel();
	showGoldLabel();
	initConstellationItem();
	initCardBagItem();
	return true;
}

void CardUILayer::setVisible(bool visible)
{
	if(visible)
	{
		ShowChatBar(false);
	}
	CCLayer::setVisible(visible);
}

void CardUILayer::initCardBagItem()
{
	for(unsigned int i =1; i<= 72;i++)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(kCardBag + i);
		setCardBagItem(item,kCardBag + i,true);
	}
}

void CardUILayer::initCardShop()
{
	if(!initShopFlag)
	{
		std::list<ExchangeItem*>* exchangelist = ItemManager::Get()->getExchangeItemsByType(7);
		exchangelist->sort(ExchangeItem::smaller());
		for(std::list<ExchangeItem*>::iterator it = exchangelist->begin();
			it != exchangelist->end();++it)
		{
			if(*it != NULL)
			{
				addShopItem(*it);
			}
		}

		UIScrollPage* shopPage = m_uiLayout->FindChildObjectByName<UIScrollPage>("shopPage");
		shopPage->moveToPage(0);

		exchangelist->clear();
		CC_SAFE_DELETE(exchangelist);

		initShopFlag = true;
	}
	
}

void CardUILayer::addShopItem(ExchangeItem* item)
{
	float scale = UIManager::sharedManager()->getScaleFactor();
	UIScrollPage* shopPage = m_uiLayout->FindChildObjectByName<UIScrollPage>("shopPage");
	unsigned int page = m_currentShopIndex / CARD_SHOP_PAGE;
	unsigned int row = (m_currentShopIndex % CARD_SHOP_PAGE) / CARD_SHOP_COLUMN;
	unsigned int column = (m_currentShopIndex % CARD_SHOP_PAGE) % CARD_SHOP_COLUMN;
	if(page >= shopPage->getPageCount())
	{
		CCLayer* layer = CCLayer::create();
		layer->setAnchorPoint(ccp(0, 0));		
		shopPage->addPage(layer);
	}

	CCLayer * layer = shopPage->getPageLayer(page);
	ASSERT(layer != NULL, "CardUILayer shop page is null");

	// 添加商品节点
	ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
	unsigned int cellIndex = getResourceIDByName("map_ui_card_FRAME_CARDSHOP_ALL_CARD1");
	CCSize parentSize = as->getframeRect(cellIndex).size;
	CCSprite* parentNode = CCSprite::create();

	float cellOffsetX = 15;
	float cellOffsetY = 15;

	float deltaX = (shopPage->getContentSize().width / scale - 2 * cellOffsetX - parentSize.width) / (CARD_SHOP_COLUMN - 1);;
	float deltaY = (shopPage->getContentSize().height / scale - 2 * cellOffsetY - parentSize.height) / (CARD_SHOP_ROW - 1);

	CCPoint parentPt;
	CCSprite *pEmptyItem = as->getSpriteFromFrame_Middle( cellIndex, 0, parentPt);
	CCPoint pt;
	CCSprite *pSelectedItem = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_SELECTED") ,0,pt); 
	IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectedItem, this, 
		menu_selector(CardUILayer::onShopItemCLicked));
	CCSprite* selectedFrame = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARDSHOP_ICON_CARD1") ,0,pt); 
	float selectX = parentSize.width * 0.5f - pSelectedItem->getContentSize().width * 0.5f;
	float selectY =  2 * (pt.y - parentPt.y) - pSelectedItem->getContentSize().height * 0.5f + selectedFrame->getContentSize().height * 0.5f;
	pSelectedItem->setPosition(ccp(selectX,selectY));
	pIcon->setIconOrder(3);
	pIcon->autorelease();

	pIcon->SetButtonIndex(item->m_exchangeId); // 设置交换ID
	pIcon->setTag(m_currentShopIndex+1);
	pIcon->setContentSize(pEmptyItem->getContentSize());

	//pIcon->SetDoubleClickHandler(this,menu_selector(CardUILayer::onShopItemDoubleClicked));
	pIcon->SetDragFlagTime(150);

	int x = column * deltaX + cellOffsetX + parentSize.width / 2 - (shopPage->getContentSize().width / scale) / 2 ;
	int y = (shopPage->getContentSize().height / scale) / 2 - (row * deltaY + cellOffsetY + parentSize.height / 2);
	pIcon->setPosition(ccp(x, y));
		
	char name[50];
	sprintf(name,"%s_%d","shopPage",m_currentShopIndex+1);
	pIcon->SetButtonName(name);
	

	// 设置商品内容
	CCSprite* cardSprite = NULL;
	std::list<pair<unsigned int ,unsigned int> >::iterator it = item->m_toItems->begin();
	if(it != item->m_toItems->end())
	{
		unsigned int cardId = it->first;
		CCSprite* cardSprite = getCardSprite(cardId,m_mixLayer->getCardAttrKey(cardId),m_mixLayer->getCardLevel(cardId));
		CCPoint cardPt;
		CCSprite* cardFrame = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARDSHOP_ICON_CARD1") ,0, cardPt);
		cardSprite->setPosition(ccpSub(cardPt,parentPt));
		pIcon->addChild(cardSprite,0);

		CCPoint keyPt;
		cardFrame = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARDSHOP_TEXT_CARD1_ATTRIBUTE1") ,0, keyPt);
		//const char* keyText = ItemManager::Get()->getAttributionKeyText(CardMixLayer::getCardAttrKey(cardId));
		const char* keyText = CardMixLayer::getCardAttrText(CardMixLayer::getCardAttrKey(cardId),CardMixLayer::getCardAttrNum(cardId,CardMixLayer::getCardAttrKey(cardId)));
		UILabelTTF* keyLabel = UILabelTTF::create(keyText,KJLinXin,18);
		keyLabel->setPosition(ccpSub(keyPt,parentPt));
		pIcon->addChild(keyLabel,1);
	}

	it = item->m_fromItems->begin();
	if(it != item->m_fromItems->end())
	{
		CCPoint diamondPt;
		CCSprite* diamondSprite = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARDSHOP_ICON_CARD1_PRICE") ,0, diamondPt);
		diamondSprite->setPosition(ccpSub(diamondPt,parentPt));
		pIcon->addChild(diamondSprite,3);

		CCPoint pricePt;
		CCSprite* priceFrame = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARDSHOP_TEXT_CARD1_PRICE") ,0, pricePt);
		char priceText[10];
		sprintf(priceText,"%d",it->second);
		UILabelTTF* label = UILabelTTF::create(priceText,KJLinXin,20);
		label->setPosition(ccpSub(pricePt,parentPt));
		pIcon->addChild(label,4);
	}
	layer->addChild(pIcon);
	++m_currentShopIndex;
}

void CardUILayer::reOrderBagItems(CCObject* obj)
{
	OnlineNetworkManager::sShareInstance()->sendBackPackReOrder(11);
}

void CardUILayer::onCloseBtClicked(CCObject* obj)
{
	MainMenuLayer::RemoveOneLayer(this);
	//HideCardUILayer();
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			mLayer->hideUILayerByTag(this->getTag());
		}
	}
}

void CardUILayer::closeLayerCallBack(void)
{
	//HideCardUILayer();
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			mLayer->hideUILayerByTag(this->getTag());
		}
	}
}

void CardUILayer::onBagItemClicked(CCObject* obj)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(obj);
	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
				int buttonIndex = button->GetButtonIndex();
				unsigned int pos = kCardBag + buttonIndex;
				BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
				if(item && !item->isEmpty && !item->isLocked)
				{
					m_bagItemPos = pos;
					//const char* sellText = Localizatioin::getLocalization("M_BUTTON_TIPS3");
					const char* mixText = Localizatioin::getLocalization("M_MATERIAL_1");
					ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
					const char* multiMixText = Localizatioin::getLocalization("M_MATERIAL_ALL");
					ItemManager::Get()->setTipDelegate(this,menu_selector(CardUILayer::onCardMixCallBack),mixText,true,
						this,menu_selector(CardUILayer::onCardMultiMixCallBack),multiMixText,true);
					
				}
				TutorialsManager::Get()->HandleOneEvent(button->m_name.c_str(),1);
			}
		}
	}
}

void CardUILayer::onShopItemCLicked(CCObject* obj)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(obj);
	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
				int buttonIndex = button->GetButtonIndex();
				m_shopItemPos = buttonIndex;
				//reqBuyCard(exchangeId);
				ExchangeItem* item = ItemManager::Get()->getExchageItemById(buttonIndex);
				if(item)
				{
					std::list<std::pair<unsigned int ,unsigned int> >::iterator it = item->m_toItems->begin();
					if(it != item->m_toItems->end())
					{
						const char* byeText = Localizatioin::getLocalization("M_DIAMONDSHOP_BUY");
						unsigned int itemId = it->first;
						ItemManager::Get()->showItemTipsById(itemId,menu->getTouchPoint());
						ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
							this,menu_selector(CardUILayer::onCardByeClicked),byeText,true);
					}
				}
				
			}
		}
	}
}

void CardUILayer::onCardMixCallBack(CCObject* obj)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(m_bagItemPos);
	if(item && !item->isEmpty && !item->isLocked)
	{
		ExchangeItem* exchangeItem = ItemManager::Get()->getEquipDrawingById(item->getItemId());
		if(exchangeItem)
		{
			reqToMixCard(item->getItemId(),exchangeItem);
		}
		else
		{
			const char* text = Localizatioin::getLocalization("M_CARD_ERROR2");
			ShowMessage(text);
		}
	}
	
}

void CardUILayer::onCardMultiMixCallBack(CCObject* sender)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(m_bagItemPos);
	if(item && !item->isEmpty && !item->isLocked)
	{
		ExchangeItem* exchangeItem = ItemManager::Get()->getEquipDrawingById(item->getItemId());
		if(exchangeItem)
		{
			reqToMixCard(item->getItemId(),exchangeItem,true);
		}
		else
		{
			const char* text = Localizatioin::getLocalization("M_CARD_ERROR2");
			ShowMessage(text);
		}
	}
}

void CardUILayer::onCardByeClicked(CCObject* obj)
{
	//reqBuyCard(m_shopItemPos);
	int tag = MainMenuLayer::getLayerTagByName("MultiCardBuyLayer");
	MultiCardBuyLayer* layer = MainMenuLayer::showUILayerByTag<MultiCardBuyLayer>(tag,true);
	if(layer)
	{
		layer->setCardExchangeId(m_shopItemPos);
	}
}

void CardUILayer::onCardSellCallBack(CCObject* sender)
{

}

void CardUILayer::onShopBtClicked(CCObject* obj)
{
	UIContainer* shopContainer = m_uiLayout->FindChildObjectByName<UIContainer>("cardShopContainer");
	bool isVisible = shopContainer->getCurrentNode()->isVisible();
	showCardShop(!isVisible);
}

//void CardUILayer::onShopItemDoubleClicked(CCObject* obj)
//{
//	CCMenu* menu = dynamic_cast<CCMenu*>(obj);
//	if(menu)
//	{
//		IconButton* button = dynamic_cast<IconButton*>(menu->getParent());
//		{
//			unsigned int exchangeId = button->GetButtonIndex();
//			reqBuyCard(exchangeId);
//		}
//	}
//}

void CardUILayer::onEquipCardBtClicked(CCObject* obj)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(obj);
	if(item)
	{
		UIMenu* menu = dynamic_cast<UIMenu*>(item->getParent());
		if(menu)
		{
			UIControlBase* control = menu->getControlNode();
			UIButton* bt = dynamic_cast<UIButton*>(control);
			unsigned int index = bt->getButtonIndex();
			unsigned int pos = index + (m_constellationIndex - 1) * 3 + kCardEquip;
			BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
			if(item)
			{
				if(item->isLocked)
				{
					if(index == 3)
					{
						BackPackItem* lastItem = ItemManager::Get()->findItemByPos(pos - 1);
						if(lastItem && lastItem->isLocked)
						{
							reqUnlockEquipCard(m_constellationIndex,true,true);
						}
						else
						{
							reqUnlockEquipCard(m_constellationIndex,false,true);
						}
					}
					else if(index == 2)
					{
						reqUnlockEquipCard(m_constellationIndex,true,false);
					}
				}
				else if(!item->isEmpty)
				{
					ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
				}
			}
		}
	}
	
}

void CardUILayer::onConstellationItemClicked(CCObject* obj)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(obj);
	if(item)
	{
		CCNode* node = item->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			UIControlBase* controller = menu->getControlNode();
			UIButton* button = dynamic_cast<UIButton*>(controller);
			if(button)
			{				
				unsigned int index = button->getButtonIndex();
				showSelectConstellation(index);

				if(m_selectedListButton)
				{
					CCMenuItem* lastItem = m_selectedListButton->getMenuItemSprite();
					if(lastItem)
					{
						lastItem->unselected();
					}
				}
				m_selectedListButton = button;
				item->selected();
			}
		}
	}
}

void CardUILayer::onHelpButtonClicked(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/HelpUI/HelpManager.lua","setRecentHelpIndex","CardUILayer");
	int tag = MainMenuLayer::getLayerTagByName("HelpUILayer");
	MainMenuLayer::showUILayerByTag<CCLayer>(tag);
}

void CardUILayer::onCardAttrPanelBtClicked(CCObject* sender)
{
	int tag = MainMenuLayer::getLayerTagByName("CardAttributionLayer");
	CardAttributionLayer* layer = MainMenuLayer::showUILayerByTag<CardAttributionLayer>(tag);
	if(layer)
	{
		layer->showCardsAttr();
	}
}

//void CardUILayer::onBagItemTapped(CCObject* obj)
//{
//	UIMenu* menu = dynamic_cast<UIMenu*>(obj);
//	if(menu)
//	{
//		CCNode* parent = menu->getParent();
//		IconButton* button = dynamic_cast<IconButton*>(parent);
//		if(button)
//		{
//			DragReceiver* receiver = button->getDragReceiver();
//			if(receiver)
//			{
//				unsigned int pos = receiver->GetHandlerPosition();
//				pos += kCardBag;
//				ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
//				//showItemTip(pos,menu->getTouchPoint());
//			}
//		}
//	}
//}

//void CardUILayer::onEquipmentTapped(CCObject* obj)
//{
//	UIMenu* menu = dynamic_cast<UIMenu*>(obj);
//	if(menu)
//	{
//		UIControlBase* control = menu->getControlNode();
//		UIButton* bt = dynamic_cast<UIButton*>(control);
//		unsigned int index = bt->getButtonIndex();
//		unsigned int pos = index + (m_constellationIndex - 1) * 3 + kCardEquip;	
//		ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
//	}
//}

//void CardUILayer::onBagItemTapCancell(CCObject* obj)
//{
//	TipMgr::Get()->destroyTip();
//}

void CardUILayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_ATTR_SP_DIAMOND_UPDATE:
		showDiamondLabel();
		break;
	case GM_ATTR_COIN_UPDATE:
		showGoldLabel();
		break;
	}
}

void CardUILayer::reqToMixCard(unsigned int cardId,ExchangeItem* item,bool isMultiMix/* = false*/)
{
	unsigned int num = ItemManager::Get()->getItemNumberById(cardId);
	std::list<pair<unsigned int ,unsigned int> >::iterator it = item->m_fromItems->begin();
	if(it == item->m_fromItems->end())
	{
		return;
	}
	unsigned int mixNum = it->second;

	if(num < mixNum)
	{
		const char* text = Localizatioin::getLocalization("M_CARD_ERROR1");
		ShowMessage(text);
		return;
	}

	unsigned int mixCardNum = 1;
	unsigned int cardNum = mixNum * mixCardNum;
	if(isMultiMix)
	{
		mixCardNum  = num / mixNum;
		cardNum = mixNum * mixCardNum;
	}

	it = item->m_toItems->begin();
	if(it != item->m_toItems->end())
	{
		unsigned int mixCardId = it->first;

		CCSprite* cardSprite = getCardSprite(cardId,m_mixLayer->getCardAttrKey(cardId),m_mixLayer->getCardLevel(cardId));
		CCSprite* mixSprite = getCardSprite(mixCardId,m_mixLayer->getCardAttrKey(mixCardId),m_mixLayer->getCardLevel(mixCardId));
		m_mixLayer->showCardMixLayer(cardId,cardSprite,mixCardId,mixSprite,item->m_exchangeId,cardNum,mixCardNum);
	}
}

void CardUILayer::onReceivedCardComposeSuccess(int error)
{
	if(error != 0)
	{
		CCLOG("card compose error: %d",error);
	}
	m_mixLayer->onReceivedCardComposeSuccess(0);
}

void CardUILayer::reqUnlockEquipCard(unsigned int pos,bool firstLock,bool secondLock)
{
	if(!firstLock && !secondLock)
	{
		return;
	}
	m_unLockIndex = pos;
	unsigned int price = 0;
	
	if(firstLock)
	{
		m_unLockExchangeId = 40070; // 40070 解锁第二个格子，40080 解锁第三个格子
		price += getSingleUnlockCost(m_unLockExchangeId);
	}

	if(secondLock)
	{
		m_unLockExchangeId = 40080; // 40070 解锁第二个格子，40080 解锁第三个格子
		price += getSingleUnlockCost(m_unLockExchangeId);
	}
	
	const char* costLabel = Localizatioin::getLocalization("M_UNLOCKCOST");

	DiamondWarningLayer::Show(costLabel,price,menu_selector(CardUILayer::onReceiveConfirmUnlock),NULL,this);
}

unsigned int CardUILayer::getSingleUnlockCost(unsigned int index)
{
	unsigned int price = 0;
	const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", index, "FromItems");
	string exchangeStr = string(fromItems);
	int dashPos = exchangeStr.find_first_of("/");
	if(dashPos != exchangeStr.npos)
	{
		unsigned int length = exchangeStr.size();
		std::string idStr = exchangeStr.substr(0,dashPos);
		std::string numStr = exchangeStr.substr(dashPos+1,length-1);
		int id = atoi(idStr.c_str());
		price = atoi(numStr.c_str());
		ASSERT(id == 416002,"cost is not diamond!");
	}
	return price;
}

void CardUILayer::reqBuyCard(unsigned int exchangeId)
{
	ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
	if(item)
	{
		std::list<std::pair<unsigned int ,unsigned int> >::iterator it = item->m_fromItems->begin();
		if(it != item->m_fromItems->end())
		{
			unsigned int price = it->second;
			if(price > UserData::GetDiamond())
			{
				// 钻石不足
				const char* warning = Localizatioin::getLocalization("S_Gh_Diamond_Content");
				ShowMessage(warning);
			}
			else
			{
				m_shopSelectedItem = exchangeId;
				const char* costLabel = Localizatioin::getLocalization("M_SHOPCOST");
				DiamondWarningLayer::Show(costLabel,price,menu_selector(CardUILayer::onReceiveConfirmBuyCard),NULL,this);
			}
		}
	}
}

void CardUILayer::onReceiveConfirmBuyCard(CCObject* obj)
{
	OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(m_shopSelectedItem,0);
}

void CardUILayer::onReceiveConfirmUnlock(CCObject* obj)
{
	OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(m_unLockExchangeId,&m_unLockIndex,1);
}

void CardUILayer::showDiamondLabel()
{
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("diamondLabel");
		if(label)
		{
			unsigned int diamond = UserData::GetDiamond();
			//const char* str = MainMenuLayer::GetNumByFormat(diamond);
			char str[20];
			sprintf(str,"%u",diamond);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}

void CardUILayer::showGoldLabel()
{
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("coinLabel");
		if(label)
		{
			unsigned int coin = UserData::GetGold();
			const char* str = MainMenuLayer::GetNumByFormat(coin);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}

void CardUILayer::showCardShop(bool isShowShop)
{
	UIContainer* equipContainer = m_uiLayout->FindChildObjectByName<UIContainer>("cardEquipContainer");
	UIContainer* shopContainer = m_uiLayout->FindChildObjectByName<UIContainer>("cardShopContainer");
	UIButton* bagBt = m_uiLayout->FindChildObjectByName<UIButton>("cardBagBt");
	UIButton* shopBt = m_uiLayout->FindChildObjectByName<UIButton>("cardShopBt");
	UILabel* bagLabel = m_uiLayout->FindChildObjectByName<UILabel>("cardBagLabel");
	UILabel* shopLabel = m_uiLayout->FindChildObjectByName<UILabel>("cardShopLabel");

	equipContainer->setVisible(!isShowShop);
	shopContainer->setVisible(isShowShop);
	bagBt->setVisible(isShowShop);
	shopBt->setVisible(!isShowShop);
	bagLabel->setVisible(isShowShop);
	shopLabel->setVisible(!isShowShop);

	if(!initShopFlag)
	{
		initCardShop();
	}
}

void CardUILayer::setCardItem(BackPackItem* item, unsigned int pos)
{
	if(pos >= kCardBag && pos< kCardEquip)
	{
		setCardBagItem(item,pos);
	}
	else if(pos >= kCardEquip && pos <kItemManorId)
	{
		unsigned index = (pos - kCardEquip - 1) / 3 + 1;
		if(index == m_constellationIndex)
		{
			unsigned int btIndex = (pos - kCardEquip) % 3;
			btIndex = (btIndex != 0) ? btIndex : 3;
			showConstellationCard(item,btIndex);
		}
	}
	else
	{
		CCAssert(NULL,"error card item pos");
	}
}

void CardUILayer::setCardBagItem(BackPackItem* item, unsigned int pos,bool isInit)
{
	unsigned int index = pos - kCardBag;
	unsigned int page = (index-1) / 6;
	unsigned int row = ((index-1) % 6) / 3;
	unsigned int colmun = ((index-1) % 6) % 3;
	if(isInit)
	{
		// 修改控件显示效果
		IconButton* button = m_bagScrollPage->GetIconButton(page,colmun,row);
		if(button)
		{
			button->setIconOrder(3);
			CCMenuItemSprite* menuItem = button->getMenuItem();
			if(menuItem)
			{
				CCNode* selectedItem = menuItem->getSelectedImage();
				if(selectedItem)
				{
					ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
					CCSize parentSize = menuItem->getContentSize();
					CCPoint pt;
					CCSprite *pSelectedItem = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_SELECTED") ,0,pt); 
					CCSize cardSize = as->getframeRect(getResourceIDByName("map_ui_card_FRAME_CARD_ICON_INBAG1")).size;
					float selectX = parentSize.width * 0.5f - pSelectedItem->getContentSize().width * 0.5f;
					float selectY =  parentSize.height - cardSize.height - pSelectedItem->getContentSize().height * 0.5f + cardSize.height * 0.5f;
					menuItem->setSelectedImage(pSelectedItem);
					pSelectedItem->setPosition(ccp(selectX,selectY));

				}
			}
			
		}
	}

	if(item->isLocked)
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin"); 
		int idx = getResourceIDByName("map_ui_card_FRAME_CARD_LOCK_CARD2");
		CCPoint pt;
		CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
		if(m_bagScrollPage)
		{
			m_bagScrollPage->SetPageItemIconWithSprite(page,colmun,row,lockPic);
			IconButton* button = m_bagScrollPage->GetIconButton(page,colmun,row);
			if(button)
			{
				button->setDragMove(false);
			}
		}
	}
	else if(item->isEmpty)
	{
		CCSprite* icon = getCardSprite(0,0,100);
		CCSprite* card = getBagCardSprite(icon,0,NULL);
		m_bagScrollPage->SetPageItemIconWithSprite(page,colmun,row,card);
		IconButton* button = m_bagScrollPage->GetIconButton(page,colmun,row);
		if(button)
		{
			button->setDragMove(false);
		}
	}
	else
	{
		if(item->m_attribution)
		{
			for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
				it != item->m_attribution->end();++it)
			{
				ITEMATTR* attr = *it;
				if(attr && attr->attr_basic_value != 0)
				{
					CCSprite* pic = getCardSprite(item->getItemId(),attr->attr_key,item->itemLevel);
					char * name = "";
					if(item->getItemInfo())
					{
						name = item->getItemInfo()->m_name;
					}
					CCSprite* card = getBagCardSprite(pic,item->amount,name);
					m_bagScrollPage->SetPageItemIconWithSprite(page,colmun,row,card);
					IconButton* button = m_bagScrollPage->GetIconButton(page,colmun,row);
					button->creatDragReceiver(ECardBagItem);
					button->setDragMove(true);
					break;
				}
			}
		}
		
	}
}

void CardUILayer::showSelectConstellation(unsigned int index)
{
	m_constellationIndex = index;
	for(unsigned int i = 1;i<= 3;++i)
	{
		unsigned int pos = kCardEquip + 3 * (m_constellationIndex - 1) + i;
		BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
		CCAssert(item != NULL,"card back pack item is null");
		showConstellationCard(item,i,true);
	}
	showConstelationName(index);
}

void CardUILayer::showConstellationCard(BackPackItem* item,unsigned int index,bool isAnim)
{
	if(item->isLocked)
	{
		CCSprite* icon = getCardSprite(0,0,100);
		showConstellationItem(index,icon,true,false,"",isAnim);
	}
	else if(item->isEmpty)
	{
		CCSprite* icon = getCardSprite(0,0,100);
		showConstellationItem(index,icon,false,false,"",isAnim);
	}
	else
	{
		if(item->m_attribution)
		{
			for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
				it != item->m_attribution->end();++it)
			{
				ITEMATTR* attr = *it;
				if(attr && attr->attr_basic_value != 0)
				{
					CCSprite* icon = getCardSprite(item->getItemId(),attr->attr_key,item->itemLevel);
					const char* text = CardMixLayer::getCardAttrText(attr->attr_key,attr->attr_basic_value);
					//const char* keyText = ItemManager::Get()->getAttributionKeyText(attr->attr_key);
					/*char numberText[10];
					sprintf(numberText,"+%d",attr->attr_basic_value);*/
					showConstellationItem(index,icon,false,true,text,isAnim);
					break;
				}
			}
		}
	}
}

void CardUILayer::showConstellationItem(unsigned int index,CCSprite* icon,bool isLock,bool isDrag,const char* attrText,bool isAnim)
{
	char btName[50];
	sprintf(btName,"cardEquipBt%d",index);
	UIButton* button = m_uiLayout->FindChildObjectByName<UIButton>(btName);
	if(button != NULL)
	{
		button->setIconSprite(icon);
		button->setDragMove(isDrag);
		int pos = (m_constellationIndex - 1)  * 3 + index;
		button->resetDragHandlerPosition(pos);
	}
	
	char labelName[50];
	sprintf(labelName,"cardAttributeType%d",index);
	UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>(labelName);
	if(label != NULL)
	{
		label->setString(attrText);
	}

	if(isAnim)
	{
		if(button)
		{
			button->getAnimationNode()->stopAllActions();
            float scale = UIManager::sharedManager()->getScaleFactor();
			//float scale = button->getAnimationNode()->getScale();
			button->getAnimationNode()->setScale(0.8f);
			CCAction* action = CCScaleTo::create(0.5f,scale);
			button->getAnimationNode()->runAction(action);
		}
		
		if(label)
		{
			label->getCurrentNode()->stopAllActions();
			float scale = UIManager::sharedManager()->getScaleFactor();
            //float scale = label->getCurrentNode()->getScale();
			label->getCurrentNode()->setScale(0.8f);
			CCAction* action = CCScaleTo::create(0.5f,scale);
			label->getCurrentNode()->runAction(action);
		}
	}

	setConstellationCardLock(isLock,index,isAnim);
}

void CardUILayer::setConstellationCardLock(bool isLock,unsigned int index,bool isAnim)
{
	char picName[20];
	sprintf(picName,"carEquipLock%d",index);
	UIPicture* pic = m_uiLayout->FindChildObjectByName<UIPicture>(picName);
	if(pic != NULL)
	{
		pic->setVisible(isLock);
		if(isLock && isAnim)
		{
			pic->getCurrentNode()->stopAllActions();
			float scale = pic->getCurrentNode()->getScale();
			pic->getCurrentNode()->setScale(0.8f);
			CCAction* action = CCScaleTo::create(0.5f,scale);
			pic->getCurrentNode()->runAction(action);
		}
	}
}

CCSprite* CardUILayer::getBagCardSprite(CCSprite* icon,unsigned int cardNum,const char* cardName)
{
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
	CCSprite* parentSprite = CCSprite::create();
	parentSprite->setAnchorPoint(CCPointZero);
	CCPoint parentPt;
	CCSprite* parentFrame = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_ALL1") ,0, parentPt);

	if(icon != NULL)
	{
		CCPoint monsterPt;
		CCSprite* monsterFrame = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_CONTENT_INBAG1") ,0, monsterPt);
		icon->setPosition(ccpSub(monsterPt,parentPt));
		parentSprite->addChild(icon,0);

		if(cardNum != NULL)
		{
			char cardStr[10];
			sprintf(cardStr,"%d",cardNum);
			UILabelTTF* label = UILabelTTF::create(cardStr,KJLinXin,20);
			//label->setColor(ccORANGE);
			label->setColor(ccGREEN);
			label->setAnchorPoint(ccp(1,0));
			float x = monsterPt.x - parentPt.x + monsterFrame->getContentSize().width * 0.4f;
			float y = parentPt.y - monsterPt.y + monsterFrame->getContentSize().height * 0.4f;
			label->setPosition(ccp(x,-y));
			parentSprite->addChild(label, 1);
		}
	}

	if(cardName != NULL)
	{
		CCPoint monsterPt;
		CCSprite* labelFrame = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_TEXT_INBAG1") ,0, monsterPt);
		UILabelTTF* label = UILabelTTF::create(cardName,KJLinXin,20);
		label->setPosition(ccpSub(monsterPt,parentPt));
		parentSprite->addChild(label, 2);
	}

	return parentSprite;
}

CCSprite* CardUILayer::getCardSprite(unsigned int itemID,unsigned int key,unsigned int level)
{
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
	CCSprite* parentSprite = CCSprite::create();
	parentSprite->setAnchorPoint(CCPointZero);
	CCPoint parentPt;
	CCSprite* parentFrame = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_ICON_CARD1") ,0, parentPt);

	CCSprite* backPic = getCardBackPicByKey(key);
	parentSprite->addChild(backPic,0);

	CCSprite* monsterPic = getCardIconById(itemID);
	if(monsterPic != NULL)
	{
		parentSprite->addChild(monsterPic,1);
		CCPoint monsterPt;
		CCSprite* monsterFrame = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_FRAME_CARD1_CONTENT") ,0, monsterPt);
		monsterPic->setPosition(ccpSub(monsterPt,parentPt));
	}

	CCSprite* upPic = getCardFrontPicByKey(key);
	if(upPic != NULL)
	{
		parentSprite->addChild(upPic,2);
		upPic->setPosition(CCPointZero);
	}

	CCSprite* numSprite = getCardNumSprite(level);
	if(numSprite != NULL)
	{
		parentSprite->addChild(numSprite,3);
		CCPoint monsterPt;
		CCSprite* monsterFrame = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_CARD1_LEVEL") ,0, monsterPt);
		numSprite->setPosition(ccpSub(monsterPt,parentPt));
	}
	return parentSprite;
}

CCSprite* CardUILayer::getCardNumSprite(unsigned int num)
{
	if(num >= 10)
	{
		return NULL;
	}

	CCPoint pt;
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");

	char iconName[50];
	sprintf(iconName,"map_ui_card_FRAME_NUMBER_%d",num);
	CCSprite* sprite = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName(iconName) ,0, pt);
	return sprite;
}

CCSprite* CardUILayer::getCardIconById(unsigned int itemID)
{
	CCPoint pt;
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");

	char iconName[50];
	sprintf(iconName,"map_ui_card_FRAME_%d",itemID);
	CCSprite* sprite = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName(iconName) ,0, pt);
	return sprite;
}

CCSprite* CardUILayer::getCardFrontPicByKey(unsigned int key)
{
	char * picName = NULL;
	switch(key)
	{
	case PB_ITEM_ATTR_PHYATK:
		picName = "map_ui_card_FRAME_CARD_DECORATION_PHYATTACK_UP";
		break;
	case PB_ITEM_ATTR_MAGATK:
		picName = "map_ui_card_FRAME_CARD_DECORATION_MAGICATTACK_UP";
		break;
	case PB_ITEM_ATTR_SKIATK:
		picName = "map_ui_card_FRAME_CARD_DECORATION_SKIATTACK_UP";
		break;
	case PB_ITEM_ATTR_PHYDEF:
		picName = "map_ui_card_FRAME_CARD_DECORATION_PHYDEFENCE_UP";
		break;
	case PB_ITEM_ATTR_MAGDEF:
		picName = "map_ui_card_FRAME_CARD_DECORATION_MAGICDEFENCE_UP";
		break;
	case PB_ITEM_ATTR_SKIDEF:
		picName = "map_ui_card_FRAME_CARD_DECORATION_SKIDEFENCE_UP";
		break;
	case PB_ITEM_ATTR_HP:
		picName = "map_ui_card_FRAME_CARD_DECORATION_HP_UP";
		break;
	default:
		picName = NULL;
		break;
	}

	CCSprite* sprite = NULL;

	if(picName != NULL)
	{
		CCPoint pt;
		ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
		sprite = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName(picName) ,0, pt);
	}

	return sprite;
}

CCSprite* CardUILayer::getCardBackPicByKey(unsigned int key)
{
	char * picName = NULL;
	switch(key)
	{
	case PB_ITEM_ATTR_PHYATK:
		picName = "map_ui_card_FRAME_CARD_DECORATION_PHYATTACK_BACK";
		break;
	case PB_ITEM_ATTR_MAGATK:
		picName = "map_ui_card_FRAME_CARD_DECORATION_MAGICATTACK_BACK";
		break;
	case PB_ITEM_ATTR_SKIATK:
		picName = "map_ui_card_FRAME_CARD_DECORATION_SKIATTACK_BACK";
		break;
	case PB_ITEM_ATTR_PHYDEF:
		picName = "map_ui_card_FRAME_CARD_DECORATION_PHYDEFENCE_BACK";
		break;
	case PB_ITEM_ATTR_MAGDEF:
		picName = "map_ui_card_FRAME_CARD_DECORATION_MAGICDEFENCE_BACK";
		break;
	case PB_ITEM_ATTR_SKIDEF:
		picName = "map_ui_card_FRAME_CARD_DECORATION_SKIDEFENCE_BACK";
		break;
	case PB_ITEM_ATTR_HP:
		picName = "map_ui_card_FRAME_CARD_DECORATION_HP_BACK";
		break;
	default:
		picName = "map_ui_card_FRAME_CARD_NOCARD";
		break;
	}

	CCSprite* sprite = NULL;

	if(picName != NULL)
	{
		CCPoint pt;
		ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
		sprite = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName(picName) ,0, pt);
	}

	return sprite;
}

void CardUILayer::initConstellationItem()
{
	UIScrollList* list = m_uiLayout->FindChildObjectByName<UIScrollList>("constellationList");
	for(unsigned int i = CONTELLATION_NUMBER; i > 0; --i)
	{
		ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
		CCPoint pt;
		char contellationName[50];
		sprintf(contellationName,"map_ui_card_FRAME_CARD_ICON_CONSTELLATION%d",i);
		CCSprite* normal = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName(contellationName) ,0, pt);
		sprintf(contellationName,"map_ui_card_FRAME_CARD_ICON_CONSTELLATION%d_LIGHT",i);
		CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle(getResourceIDByName(contellationName), 0, pt);
		CCSprite* backPic = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_BUTTON_SHEET1"), 0, pt);
		UIButton* button = list->AddNewItem(normal,
			clicked,
			"CardUILayer::onConstellationItemClicked",
			"",
			ccWHITE,
			kCCScrollMenuPriority);
		button->setIconSprite(backPic);
		button->setButtonIndex(i);
	}

	/*for(unsigned int i =1; i< 3;i++)
	{
		char btName[50];
		sprintf(btName,"cardEquipBt%d",i);
		UIButton* button = m_uiLayout->FindChildObjectByName<UIButton>(btName);
		if(button)
		{
			CCMenuItemSprite* menuItem = button->getMenuItem();
			if(menuItem)
			{
				CCNode* 
				CCNode* selectedItem = menuItem->getSelectedImage();
				if(selectedItem)
				{
					ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui_card.bin");
					CCSize parentSize = menuItem->getContentSize();
					CCPoint pt;
					CCSprite *pSelectedItem = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_card_FRAME_CARD_SELECTED") ,0,pt); 
					CCSize cardSize = as->getframeRect(getResourceIDByName("map_ui_card_FRAME_CARD_ICON_INBAG1")).size;
					float selectX = parentSize.width * 0.5f - pSelectedItem->getContentSize().width * 0.5f;
					float selectY =  parentSize.height - cardSize.height - pSelectedItem->getContentSize().height * 0.5f + cardSize.height * 0.5f;
					menuItem->setSelectedImage(pSelectedItem);
					pSelectedItem->setPosition(ccp(selectX,selectY));

				}
			}
		}
	}*/

	setConstellationDefaultSelect();
}

void CardUILayer::setConstellationDefaultSelect()
{
	UIScrollList* list = m_uiLayout->FindChildObjectByName<UIScrollList>("constellationList");
	CCSize size = list->getContentSize();
	CCSize viewSize = list->getViewSize();
	CCPoint startPoint = list->getContentOffset();
	CCPoint offset = ccp(startPoint.x + size.width - viewSize.width,startPoint.y);
	list->setStartOffset(offset);

	UIButton* bt = m_uiLayout->FindChildObjectByName<UIButton>("ListItem_11");
	if(bt)
	{
		bt->getMenuItemSprite()->selected();
	}
	m_selectedListButton = bt;
	showSelectConstellation(1);
}

void CardUILayer::showConstelationName(unsigned int index)
{
	char name[30];
	sprintf(name,"M_CONSTELLATION_%d",index);
	const char* str = Localizatioin::getLocalization(name);
	if(str != NULL)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("decoration2Lable");
		if(label)
		{
			label->setString(str);
		}
	}
}
