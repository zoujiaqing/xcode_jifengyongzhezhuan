#include "OnlineNetworkManager.h"
#include "AlchemyUILayer.h"
#include "MainMenuLayer.h"
#include "AspriteManager.h"
#include "ASprite.h"
#include "DragReceiver.h"
#include "GameFontManager.h"
#include "UserData.h"
#include "NotificationCenter.h"
#include "GMessage.h"
#include "particle_manager.h"
#include "TipMgr.h"
#include "Localization.h"
#include "GameAudioManager.h"
#include "TutorialsManager.h"
#include "LuaTinkerManager.h"
#include "DiamondWarningLayer.h"
#include "MessageBoxLayer.h"
#include "GameResourceManager.h"
#include "HeroRoleManager.h"
#include "SpriteFactory.h"

AlchemyUILayer::AlchemyUILayer()
{
	m_uiLayout = NULL;
	m_alchemistStoneLayer = NULL;
	m_alchemyEquipLayout = NULL;
	m_alchemistStorageLayer = NULL;
	m_alchemyExchangeLayout = NULL;
	m_exchangeItemList = NULL;
	m_selectedExchangeIndex = 0;
	m_selectedUpgradeItemPos = 0;
	m_selectedButton = NULL;
	//m_heroEquipToggle = NULL;
	//m_fairyEquipToggle = NULL;
	//m_equipLayoutIndex = true;
	m_heroEquipPic = NULL;
	m_fairyEquipPic = NULL;
	m_selectedNpcIndex = 0;
	m_unLockExchangeId = 0;
	autoSearchFlag = false;
	m_fairyBtMap = NULL;
	m_lastClickedFairyIndex = -1;
	m_currentEquipPage = 0;
	m_TopSearchCost = 0;
	m_tipPos = 0;
}

AlchemyUILayer::~AlchemyUILayer()
{
	UIManager::sharedManager()->RemoveUILayout("alchemyLayout");
	UIManager::sharedManager()->RemoveUILayout("alchemyEquipLayout");
	UIManager::sharedManager()->RemoveUILayout("alchemyExchangeLayout");
	if(m_exchangeItemList)
	{
		m_exchangeItemList->clear();
		delete m_exchangeItemList;
		m_exchangeItemList = NULL;
	}
	m_bagListButton.clear();
	m_storageListButton.clear();
	m_exchangeItemInfo.clear();

	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

bool AlchemyUILayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

	m_uiLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/alchemyUI.plist", this, "alchemyLayout");

	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onClosedBtClicked",
		menu_selector(AlchemyUILayer::onClosedBtClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onEnterAlchemyBagClicked",
		menu_selector(AlchemyUILayer::onEnterAlchemyBagClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onAlchemyExchangeClicked",
		menu_selector(AlchemyUILayer::onAlchemyExchangeClicked), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onClickFindSoul",
		menu_selector(AlchemyUILayer::onClickFindSoul), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onClickTopFind",
		menu_selector(AlchemyUILayer::onClickTopFind), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onClickAutoFind",
		menu_selector(AlchemyUILayer::onClickAutoFind), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onClickTransAllStoneEnergy",
		menu_selector(AlchemyUILayer::onClickTransAllStoneEnergy), this);
	UIManager::sharedManager()->RegisterMenuHandler("alchemyLayout","AlchemyUILayer::onHelpButtonClicked",
		menu_selector(AlchemyUILayer::onHelpButtonClicked), this);
	
	initAlchemistStone();
	initNpcIcon();
	initTopSearchCost();
	initAlchemyExItemInfo();
	updateBottomSpLabel();
	updateBottomDiamondLabel();
	updateBottomPieceLabel();
	updateBottomCoinLabel();

	
	return true;
}

void AlchemyUILayer::setVisible(bool visible)
{
	if(visible)
	{
		ShowChatBar(false);
	}
	CCLayer::setVisible(visible);
}

void AlchemyUILayer::initTopSearchCost()
{
	const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", 20310, "FromItems");
	string exchangeStr = string(fromItems);
	int dashPos = exchangeStr.find_first_of("/");
	if(dashPos != exchangeStr.npos)
	{
		unsigned int length = exchangeStr.size();
		std::string idStr = exchangeStr.substr(0,dashPos);
		std::string numStr = exchangeStr.substr(dashPos+1,length-1);
		int id = atoi(idStr.c_str());
		ASSERT(id == 416002,"cost is not diamond!");
		int price = atoi(numStr.c_str());
		m_TopSearchCost += price;
	}
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("costDiamondLable");
		if(label)
		{
			label->setString(CCString::stringWithFormat("%d",m_TopSearchCost)->m_sString.c_str());
			label->setVisible(false);
		}

		label = m_uiLayout->FindChildObjectByName<UILabel>("topFindLable");
		if(label)
		{
			label->setVisible(false);
		}

		UIPicture* costDiamondFrame = m_uiLayout->FindChildObjectByName<UIPicture>("costDiamondFrame");
		if(costDiamondFrame)
		{
			costDiamondFrame->setVisible(false);
		}
		
		UIButton* topFindBt = m_uiLayout->FindChildObjectByName<UIButton>("topFindBt");
		if(topFindBt)
		{
			topFindBt->setVisible(false);
		}
	}
}

void AlchemyUILayer::initAlchemyExItemInfo()
{
	if(m_exchangeItemInfo.size() == 0)
	{
		OnlineNetworkManager::sShareInstance()->sendSpriteExchangeMessage();
	}
}

bool AlchemyUILayer::initAlchemistStone()
{
	m_alchemistStoneLayer = CCLayer::create();
	if(m_uiLayout)
	{
		UIPicture* frame = m_uiLayout->FindChildObjectByName<UIPicture>("alchemyGridBg");
		if(frame)
		{
			int row = 5;
			int column = 6;
			CCSize size = frame->getSpriteSize();
			float width = size.width / column;
			float height = size.height / row;
			ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
			for(int i=0; i<row; i++)
			{
				for(int j=0; j<column; j++)
				{
					CCSprite* pEmptyItem = CCSprite::create();
					pEmptyItem->setContentSize(CCSize(width,height));
					CCPoint pt;
					int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_ICON_CLICKED");
					CCSprite* pSelectItem = as->getSpriteFromFrame_Middle(iFrameId,0,pt);
					float scale = UIManager::sharedManager()->getScaleFactor();
					pSelectItem->setScale(scale);
					IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, 
						menu_selector(AlchemyUILayer::onItemClicked));
					
					pIcon->autorelease();
					int index = i * column + j;
					pIcon->SetButtonIndex(index+1);
					pIcon->setContentSize(pEmptyItem->getContentSize());
					pIcon->creatDragReceiver(EAlchemyBag);
					//pIcon->SetTappedHandler(this,menu_selector(AlchemyUILayer::onItemTapped),menu_selector(AlchemyUILayer::onTapCancel));
					float x = (j - column /2 + 0.5f) * width;
					//float y = (-i + row /2 - 0.5f) * height;
					float y = (-i + row /2) * height;
					pIcon->setPosition(ccp(x, y));

					char name[20];
					sprintf(name,"alchemyBag_%d",index);
					pIcon->SetButtonName(name);

					m_bagListButton.push_back(pIcon);
					m_alchemistStoneLayer->addChild(pIcon,index+1);				
				}
				
			}
			m_alchemistStoneLayer->setPosition(frame->getWorldPosition());
			m_uiLayout->getCurrentNode()->addChild(m_alchemistStoneLayer);

			for(unsigned int i=1;i<=AlCHEMYBAGCAPACITY;i++)
			{
				unsigned int pos = i+kAlchemyBag;
				BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
				if(item)
				{
					setItemIcon(item,pos);
				}
			}

			return true;
		}
	}
	return false;
}

bool AlchemyUILayer::initAlchemistStorage()
{
	m_alchemistStorageLayer = CCLayer::create();
	if(m_alchemyEquipLayout)
	{
		UIPicture* frame = m_alchemyEquipLayout->FindChildObjectByName<UIPicture>("alchemyEquipGridBg");
		if(frame)
		{
			int row = 3;
			int column = 6;
			CCSize size = frame->getSpriteSize();
			float width = size.width / column;
			float height = size.height / row;
			ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
			for(int i=0; i<row; i++)
			{
				for(int j=0; j<column; j++)
				{
					CCSprite* pEmptyItem = CCSprite::create();
					pEmptyItem->setContentSize(CCSize(width,height));
					CCPoint pt;
					int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_ICON_CLICKED");
					CCSprite* pSelectItem = as->getSpriteFromFrame_Middle(iFrameId,0,pt);
					float scale = UIManager::sharedManager()->getScaleFactor();
					pSelectItem->setScale(scale);
					IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, 
						menu_selector(AlchemyUILayer::onItemClicked));
					pIcon->autorelease();
					int index = i * column + j;
					pIcon->SetButtonIndex(index+1);
					pIcon->setContentSize(pEmptyItem->getContentSize());
					pIcon->creatDragReceiver(EAlchemyStorage);
					/*pIcon->SetTappedHandler(this,menu_selector(AlchemyUILayer::onItemTapped),menu_selector(AlchemyUILayer::onTapCancel));
					pIcon->SetDoubleClickHandler(this,menu_selector(AlchemyUILayer::onItemDoubleClicked));*/
					float x = (j - column /2 + 0.5f) * width;
					float y = (-i + row /2) * height;
					pIcon->setPosition(ccp(x, y));

					char name[20];
					sprintf(name,"alchemyStorage_%d",index);
					pIcon->SetButtonName(name);

					m_storageListButton.push_back(pIcon);
					m_alchemistStorageLayer->addChild(pIcon,index+1);			
				}

			}
			m_alchemistStorageLayer->setPosition(frame->getWorldPosition());
			m_alchemyEquipLayout->getCurrentNode()->addChild(m_alchemistStorageLayer);

			for(unsigned int i=1;i<=AlCHEMYSTORAGECAPACITY;i++)
			{
				unsigned int pos = i+kAlchemyStorage;
				BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
				if(item)
				{
					setItemIcon(item,pos);
				}
			}

			//m_heroEquipToggle = m_alchemyEquipLayout->FindChildObjectByName<UIButtonToggle>("heroAlchemyButtonToggle");
			//m_fairyEquipToggle = m_alchemyEquipLayout->FindChildObjectByName<UIButtonToggle>("fairyAlchemyButtonToggle");
			m_heroEquipPic = m_alchemyEquipLayout->FindChildObjectByName<UIPicture>("alchemyDecorationHero");
			m_fairyEquipPic = m_alchemyEquipLayout->FindChildObjectByName<UIPicture>("alchemyDecorationFairy");

			if(m_fairyEquipPic)
			{
				m_fairyEquipPic->setVisible(false);
			}

			//m_equipLayoutIndex = true;
			showHeroEquipItems();
			setStoneEnergyLabel();
			UIScrollPage* modlePage = m_alchemyEquipLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
			if(modlePage)
			{
				modlePage->setTouchStolenDistance(10);
				// 添加玩家人物模型
				showPlayerModle(modlePage);
				// 添加出战精灵
				showFairyModle(FAIRY_POS_START_BTL,1);
				showFairyModle(FAIRY_POS_START_BTL + 1,2);
				// 添加助战精灵
				showFairyModle(FAIRY_POS_START_AST,3);
				showFairyModle(FAIRY_POS_START_AST + 1,4);

				modlePage->registerPageChangedCallBackFunc(this,callfunc_selector(AlchemyUILayer::onReceivedEquipPageChanged));
				modlePage->moveToPage(m_currentEquipPage,false);
			}
			initEquipFairyList();
			/*if(m_heroEquipToggle)
			{
				m_heroEquipToggle->selected();
			}*/
			return true;
		}
	}
	return false;
}

void AlchemyUILayer::initEquipFairyList()
{
	UIScrollList* modlePage = m_alchemyEquipLayout->FindChildObjectByName<UIScrollList>("fairyList");
	if(NULL == m_fairyBtMap)
	{
		m_fairyBtMap = new std::map<unsigned int,TXGUI::UIButton*>();
		for(int i = 4; i >= 0 ; --i)
		{
			ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
			CCPoint pt;
			CCSprite* normal = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_ALCHEMY_FRAME_ROLE") ,0, pt);
			CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_ALCHEMY_FRAME_ROLE_SELECTED"), 0, pt);
			UIButton* button = modlePage->AddNewItem(normal,
				clicked,
				"AlchemyUILayer::onFairyListItemClicked",
				"",
				ccWHITE,
				kCCScrollMenuPriority);
			char btName[50];
			sprintf(btName,"fairyList_%d",i);
			button->setName(btName);
			m_fairyBtMap->insert(make_pair(4 - i,button));
			if(i == 0)
			{
				m_lastClickedFairyIndex = 4;
				button->getMenuItemSprite()->selected();
			}
		}
		CCSize size = modlePage->getContentSize();
		CCSize viewSize = modlePage->getViewSize();
		CCPoint startPoint = modlePage->getContentOffset();
		CCPoint offset = ccp(startPoint.x + size.width - viewSize.width,startPoint.y);
		modlePage->setStartOffset(offset);

		const char* headIconName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", UserData::GetUserType(), "bagHeadPic");
		int idx = getResourceIDByName(headIconName);
		CCPoint pt;
		ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
		CCSprite * playerPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
		//playerPic->setScale(0.5f);
		setFairyListIcon(4,playerPic);
		setFairyIcon(3,FAIRY_POS_START_BTL);
		setFairyIcon(2,FAIRY_POS_START_BTL + 1);
		setFairyIcon(1,FAIRY_POS_START_AST);
		setFairyIcon(0,FAIRY_POS_START_AST + 1);
	}
	
}

void AlchemyUILayer::setFairyIcon(unsigned int index,unsigned int pos)
{
	if(isFairyPosLocked(pos))
	{
		CCSprite* sprite = AspriteManager::getInstance()->getOneFrame("UI/ui.bin","map_ui_FRAME_EQUIPMENT_FAIRY_FRAME1_LOCK");
		unsigned int requiredLevel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyPosUnlockLevel",pos);
		setFairyListIcon(index,sprite,requiredLevel);
	}
	else
	{
		unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",pos);
		if(fairyId != 0)
		{
			const char* fairyIcon = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID",fairyId);
			CCSprite* sprite = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin",fairyIcon);
			setFairyListIcon(index,sprite);
		}
		else
		{
			CCSprite* sprite = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_FAIRY_LOCK");
			setFairyListIcon(index,sprite);
		}
	}
}

bool AlchemyUILayer::isFairyPosLocked(unsigned int pos)
{
	bool ret = true;
	unsigned int requiredLevel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyPosUnlockLevel",pos);
	if(UserData::GetUserLevel() >= requiredLevel)
	{
		ret = false;
	}
	return ret;
}

void AlchemyUILayer::showPlayerModle(UIScrollPage* modlePage)
{
	if(modlePage)
	{
		CCLayer* playerModleLayer = modlePage->getPageLayer(0);
		if (NULL == playerModleLayer)
		{
			playerModleLayer = CCLayer::create();
			PLAYER_ID playerID;
			playerID.setPlayId(10000,0,0);
			SpriteSeer* showModle = HeroRoleManager::Get()->createRole(playerID, "", UserData::GetUserType(), false,false);
			showModle->SetAnim(kTypeIdle_FaceDown_Stand_MAIN,0,true);
			unsigned int weaponId = ItemManager::Get()->getHeroWeaponId();
			showModle->SetEquipItemsData(weaponId);
			float scale = UIManager::sharedManager()->getScaleFactor();
			showModle->getRoot()->setScale(1.5f);
			CCPoint modePt = ccp(0 ,modlePage->getContentSize().height * (-0.3f) / scale);
			showModle->setPosition(modePt);
			playerModleLayer->addChild(showModle);
			modlePage->addPage(playerModleLayer);
		}
	}
}

void AlchemyUILayer::showFairyModle(unsigned int fairyPos,unsigned int index)
{
	UIScrollPage* modlePage = m_alchemyEquipLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
	if(modlePage)
	{
		CCLayer* fairyModleLayer = modlePage->getPageLayer(index);
		if (NULL == fairyModleLayer)
		{
			fairyModleLayer = CCLayer::create();
			if(isFairyPosLocked(fairyPos))
			{

			}
			else
			{
				int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
				if(fairyId != 0)
				{
					SpriteFactory::sharedFactory()->ShowElfOnLayerAtPoint(fairyModleLayer,fairyId,ccp(0,0));
				}
			}
			
			modlePage->addPage(fairyModleLayer);
		}
		else
		{
			if(isFairyPosLocked(fairyPos))
			{
			}
			else
			{
				SpriteFactory::sharedFactory()->RemoveElfFromParent(fairyModleLayer);
				int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
				if(fairyId != 0)
				{
					SpriteFactory::sharedFactory()->ShowElfOnLayerAtPoint(fairyModleLayer,fairyId,ccp(0,0));
				}
			}
		}
	}
}

void AlchemyUILayer::onReceivedEquipPageChanged()
{
	UIScrollPage* modlePage = m_alchemyEquipLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
	if(modlePage)
	{
		m_currentEquipPage = modlePage->getCurPage();
		showHeroEquipItems();
	}
	resetFairyListPostion(m_currentEquipPage);
}

void AlchemyUILayer::resetFairyListPostion(int currentPage)
{
	UIScrollList* uiList = m_alchemyEquipLayout->FindChildObjectByName<UIScrollList>("fairyList");
	if(uiList)
	{
		std::list<UIControlBase*>::iterator it = uiList->getChildren().begin();
		for(; it != uiList->getChildren().end(); it++)
		{
			if((*it) != NULL)
			{
				if((*it)->getTag() == 4 - currentPage)
				{
					UIButton* bt = dynamic_cast<UIButton*>(*it);
					if(bt)
					{
						float scale = UIManager::sharedManager()->getScaleFactor();
						float buttonWidth = bt->getButtonSize().width * scale;
						CCSize contentSize = uiList->getContentSize();
						CCSize viewSize = uiList->getViewSize();
						CCPoint pt = bt->getPosition();
						CCPoint currentOffset = uiList->getContentOffset();
						float btLeftEdge = contentSize.width - viewSize.width * 0.5f + pt.x - buttonWidth *0.5f;
						float btRightEdge = contentSize.width - viewSize.width * 0.5f + pt.x + buttonWidth *0.5f;
						float leftEdge = contentSize.width - currentOffset.x - viewSize.width;
						float rightEage = contentSize.width - currentOffset.x;
						if(btLeftEdge < leftEdge)
						{
							CCPoint newOffset = ccp(buttonWidth *0.5f - viewSize.width * 0.5f - pt.x,currentOffset.y);
							uiList->setStartOffset(newOffset);
						}
						else if(btRightEdge > rightEage)
						{
							CCPoint newOffset = ccp(viewSize.width * 0.5f - buttonWidth *0.5f - pt.x,currentOffset.y);
							uiList->setStartOffset(newOffset);
						}

						std::map<unsigned int,TXGUI::UIButton*>::iterator lastIt = m_fairyBtMap->find(m_lastClickedFairyIndex);
						if(lastIt != m_fairyBtMap->end())
						{
							UIButton* button = lastIt->second;
							button->getMenuItemSprite()->unselected();
						}
						m_lastClickedFairyIndex = 4 - m_currentEquipPage;
						bt->getMenuItemSprite()->selected();
					}
				}
			}
		}
	}
}

bool AlchemyUILayer::initAlchemistExchange()
{
	if(m_alchemyExchangeLayout)
	{
		if(!m_exchangeItemList)
		{
			m_exchangeItemList = new std::map<unsigned int,unsigned int>();
			list<unsigned int>* exchangeIds = ItemManager::Get()->getExchangeSpriteStones(4);
			if(exchangeIds)
			{
				for(std::list<unsigned int>::iterator it = exchangeIds->begin();
					it != exchangeIds->end();it++)
				{
					unsigned int exchangeId = *it;
					ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
					if(item && item->m_toItems && item->m_toItems->size() == 1)
					{
						for (std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_toItems->begin();
							iter != item->m_toItems->end();iter++)
						{
							m_exchangeItemList->insert(make_pair((*iter).first,exchangeId));
						}	
					}
				}

				exchangeIds->clear();
				delete exchangeIds;
				exchangeIds = NULL;
			}		

		}

		unsigned int defaultIndex = 0;
		m_exchangeList = m_alchemyExchangeLayout->FindChildObjectByName<UIScrollList>("clchemyList");
		if(m_exchangeList)
		{
			m_exchangeList->clearChildrens();
			for(std::map<unsigned int,unsigned int>::iterator it = m_exchangeItemList->begin();
				it != m_exchangeItemList->end();it++)
			{
				addExchangeItem((*it).first,(*it).first);
				if(defaultIndex == 0)
				{
					defaultIndex = (*it).first;
				}
			}

		}
		showSelectecdExchangeItem(defaultIndex);
		setStoneEnergyLabel();
		setSpiritChipLabel();
	}
	return false;
}

void AlchemyUILayer::initNpcIcon()
{
	setNpcHeadIcon("alchemyNpcPic1","map_ui_system_icon_FRAME_ICON_ALCHEMY_1");
	setNpcHeadIcon("alchemyNpcPic2","map_ui_system_icon_FRAME_ICON_ALCHEMY_2");
	setNpcHeadIcon("alchemyNpcPic3","map_ui_system_icon_FRAME_ICON_ALCHEMY_3");
	setNpcHeadIcon("alchemyNpcPic4","map_ui_system_icon_FRAME_ICON_ALCHEMY_4");
	setNpcHeadIcon("alchemyNpcPic5","map_ui_system_icon_FRAME_ICON_ALCHEMY_5");
	USERINFO info = UserData::GetUserInfo();
	unsigned int npc = info.m_searchSpriteNpc;
	setNpcSelected(npc+1);
}

void AlchemyUILayer::onReceivedExchangeInfo(CSGetExchangeSSInfoRsp* msg)
{
	if(msg)
	{
		m_exchangeItemInfo.clear();
		unsigned int count = msg->ss_list_size();
		for(unsigned int i=0;i<count;++i)
		{
			ExchangeSSInfo ssInfo = msg->ss_list(i);
			stAlchemyExchangeInfo info;
			info.ss_id = ssInfo.ss_id();
			info.consume = ssInfo.consume();
			info.convert = ssInfo.convert();
			info.key = ssInfo.key();
			info.need_chip = ssInfo.need_chip();
			info.value = ssInfo.value();
			m_exchangeItemInfo.insert(make_pair(info.ss_id,info));
		}
	}
}

void AlchemyUILayer::setFairyListIcon(unsigned int index,CCSprite* icon,unsigned int lockLevel/* = 0*/)
{
	if(m_fairyBtMap != NULL)
	{
		std::map<unsigned int,TXGUI::UIButton*>::iterator it = m_fairyBtMap->find(index);
		if(it != m_fairyBtMap->end())
		{
			UIButton* button = it->second;
			button->setIconSprite(icon);
			if(lockLevel != 0)
			{
				char levelStr[30];
				sprintf(levelStr,"Lv.%u",lockLevel);
				CCSize size = button->getMenuItemSprite()->getContentSize();
				button->setTextLabel(levelStr,18,ccRED,KJLinXin, ccp(size.width * (0.4f),size.height * (-0.4f)),ccp(1,0));
			}
			else
			{
				button->setTextLabel("",18,ccORANGE,KJLinXin, CCPointZero);
			}
		}
	}
}

void AlchemyUILayer::setNpcHeadIcon(const char* name,const char* resoureName)
{
	UIButton* bt = m_uiLayout->FindChildObjectByName<UIButton>(name);
	if(bt)
	{
		CCMenuItemSprite*  item = bt->getMenuItemSprite();
		if(item)
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
			int idx = getResourceIDByName(resoureName);
			CCPoint pt;
			CCSprite * sprite = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(sprite)
			{
				float scale = 1;
				if(sprite->getContentSize().width != 0)
				{
					scale = item->getNormalImage()->getContentSize().width / sprite->getContentSize().width;
					sprite->setScale(scale);
				}

				bt->setIconSprite(sprite);

				ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
				int idx = getResourceIDByName("map_ui_FRAME_FRAME_HEADSHOT");
				CCPoint pt;
				CCSprite * frame = as->getSpriteFromFrame_Middle( idx, 0, pt);
				if(frame)
				{
					if(frame->getContentSize().width != 0)
					{
						CCSize imageSize = item->getNormalImage()->getContentSize();
						float frameScale = imageSize.width / frame->getContentSize().width;
						frame->setScale(1.2f*frameScale);
						frame->setContentSize(CCSize(imageSize.width * 1.2f,imageSize.height * 1.2f));
					}
					item->setNormalImage(frame);
				}

				idx = getResourceIDByName("map_ui_FRAME_ALCHEMY_CHARACTER_ICON_SELECTED");
				CCSprite * clickedFrame = as->getSpriteFromFrame_Middle( idx, 0, pt);
				if(clickedFrame)
				{
					if(clickedFrame->getContentSize().width != 0)
					{
						CCSize imageSize = item->getSelectedImage()->getContentSize();
						float frameScale = imageSize.width / clickedFrame->getContentSize().width;
						clickedFrame->setScale(1.2f*frameScale);
						clickedFrame->setContentSize(CCSize(imageSize.width * 1.2f,imageSize.height * 1.2f));
					}
					item->setSelectedImage(clickedFrame);
				}
			}	
		}
	}
	
}

void AlchemyUILayer::updateAutoSearch(float dt)
{
	OnlineNetworkManager::sShareInstance()->sendFindSoulStoneMessage();
}

void AlchemyUILayer::setAutoSearchStatus(bool status)
{
	if(autoSearchFlag == status)
	{
		return;
	}

	if(status)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("autoFindLable");
		if(label)
		{
			label->setString(Localizatioin::getLocalization("S_CANCEL"));
		}
		UIButton* findSoulBt = m_uiLayout->FindChildObjectByName<UIButton>("findSoulBt");
		if(findSoulBt)
		{
			findSoulBt->setEnabled(false);
		}
		UIButton* topFindBt = m_uiLayout->FindChildObjectByName<UIButton>("topFindBt");
		if(topFindBt)
		{
			topFindBt->setEnabled(false);
		}
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AlchemyUILayer::updateAutoSearch), this,0.2f,false);
	}
	else
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("autoFindLable");
		if(label)
		{
			label->setString(Localizatioin::getLocalization("M_ALCHEMY_AUTOFIND"));
		}
		UIButton* findSoulBt = m_uiLayout->FindChildObjectByName<UIButton>("findSoulBt");
		if(findSoulBt)
		{
			findSoulBt->setEnabled(true);
		}
		UIButton* topFindBt = m_uiLayout->FindChildObjectByName<UIButton>("topFindBt");
		if(topFindBt)
		{
			topFindBt->setEnabled(true);
		}
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AlchemyUILayer::updateAutoSearch), this);
	}

	autoSearchFlag = status;
}

void AlchemyUILayer::onFairyListItemClicked(CCObject* sender)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
	if(menuItem)
	{
		unsigned int tag = menuItem->getTag();
		if(m_lastClickedFairyIndex != -1 && m_lastClickedFairyIndex != tag)
		{
			std::map<unsigned int,TXGUI::UIButton*>::iterator it = m_fairyBtMap->find(m_lastClickedFairyIndex);
			if(it != m_fairyBtMap->end())
			{
				UIButton* button = it->second;
				button->getMenuItemSprite()->unselected();
			}
		}
		m_lastClickedFairyIndex = tag;
		std::map<unsigned int,TXGUI::UIButton*>::iterator it = m_fairyBtMap->find(m_lastClickedFairyIndex);
		if(it != m_fairyBtMap->end())
		{
			UIButton* button = it->second;
			button->getMenuItemSprite()->selected();
			UIScrollPage* modlePage = m_alchemyEquipLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
			if(modlePage != NULL)
			{
				modlePage->moveToPage(4 - m_lastClickedFairyIndex,false);
			}
		}
	}
}

//void AlchemyUILayer::onEquipmentTapped(CCObject* sender)
//{
//	UIMenu* menu = dynamic_cast<UIMenu*>(sender);
//	if(menu)
//	{
//		UIControlBase* controller = menu->getControlNode();
//		UIButton* button = dynamic_cast<UIButton*>(controller);
//		if(button)
//		{
//			unsigned int pos = button->getButtonIndex();
//			if(m_equipLayoutIndex)
//			{
//				pos += kAlchemyHeroEquip;
//			}
//			else
//			{
//				pos += kAlchemyFairyEquip;
//			}
//			ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
//		}
//	}
//}

//void AlchemyUILayer::onTapCancel(CCObject* sender)
//{
//	TipMgr::Get()->destroyTip();
//}
//
//void AlchemyUILayer::onItemTapped(CCObject* sender)
//{
//	UIMenu* menu = dynamic_cast<UIMenu*>(sender);
//	if(menu)
//	{
//		CCNode* parent = menu->getParent();
//		IconButton* button = dynamic_cast<IconButton*>(parent);
//		if(button)
//		{
//			DragReceiver* receiver = button->getDragReceiver();
//			if(receiver)
//			{
//				if (false == TutorialsManager::Get()->HandleOneEvent(button->m_name.c_str(),3))
//				{
//					return ;
//				}
//				unsigned int pos = receiver->GetHandlerPosition();
//				DragHandlerType type = receiver->GetDragItemType();
//				if(type == EAlchemyBag)
//				{
//					pos += kAlchemyBag;
//				}
//				else if(type == EAlchemyStorage)
//				{
//					pos += kAlchemyStorage;
//				}
//				ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());				
//			}
//		}
//	}
//}
//
//void AlchemyUILayer::onItemDoubleClicked(CCObject* sender)
//{
//	UIMenu* menu = dynamic_cast<UIMenu*>(sender);
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
//				DragHandlerType type = receiver->GetDragItemType();
//				if(type == EAlchemyBag)
//				{
//					pos += kAlchemyBag;
//				}
//				else if(type == EAlchemyStorage)
//				{
//					pos += kAlchemyStorage;
//				}
//				reqToUnlockBagItem(pos);
//			}
//		}
//	}
//}

void AlchemyUILayer::onClosedBtClicked(CCObject* sender)
{
	MainMenuLayer::RemoveOneLayer(this);
	//HideAlchemyUILayer();
	int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
	MainMenuLayer::hideUILayerByTag(tag);
}

void AlchemyUILayer::onClosedEquipBtClicked(CCObject* sender)
{
	hideAlchemyEquipLayer();
	showAlchemyLayer();
}

void AlchemyUILayer::onEnterAlchemyBagClicked(CCObject* sender)
{
	if(!autoSearchFlag)
	{
		hideAlchemyLayer();
		showAlchemyEquipLayer();
	}
}

void AlchemyUILayer::onClosedExchangeBtClicked(CCObject* sender)
{
	hideAlchemyExchangeLayer();
	showAlchemyLayer();
}

void AlchemyUILayer::onAlchemyExchangeClicked(CCObject* sender)
{
	if(!autoSearchFlag && m_exchangeItemInfo.size() > 0)
	{
		hideAlchemyLayer();
		showAlchemyExchangeLayer();
	}
}

void AlchemyUILayer::onEquipClicked(CCObject* sender)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			UIControlBase* controller = menu->getControlNode();
			UIButton* button = dynamic_cast<UIButton*>(controller);
			if(button)
			{
				unsigned int pos = button->getButtonIndex();
				pos += kAlchemyHeroEquip + m_currentEquipPage * 100;
				m_tipPos = pos;
				ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
				const char* text = Localizatioin::getLocalization("M_STRENGTHENEQUIPAGE_TEXT_8");
				ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
					this,menu_selector(AlchemyUILayer::onUnEquipCallback),text,true);
			}
		}
	}
}

void AlchemyUILayer::onItemClicked(CCObject* sender)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
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
				TutorialsManager::Get()->HandleOneEvent(button->m_name.c_str(),1);

				DragReceiver* receiver = button->getDragReceiver();
				if(receiver)
				{
					unsigned int pos = receiver->GetHandlerPosition();
					DragHandlerType type = receiver->GetDragItemType();
					if(type == EAlchemyBag)
					{
						pos += kAlchemyBag;
					}
					else if(type == EAlchemyStorage)
					{
						pos += kAlchemyStorage;
					}
					m_tipPos = pos;
					BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
					if(item)
					{
						if(item->isLocked)
						{
							reqToUnlockBagItem(pos);
						}
						else if(!item->isEmpty)
						{
							ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
							if(type == EAlchemyBag)
							{
								const char* text = Localizatioin::getLocalization("M_ALCHEMY_PUTINBAG");
								ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
									this,menu_selector(AlchemyUILayer::onMoveToBagCallback),text,true);
							}
							else
							{
								const char* text = Localizatioin::getLocalization("M_FAIRY_EQUIPON");
								ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
									this,menu_selector(AlchemyUILayer::onEquipOnCallback),text,true);
							}
						}						
					}
					
				}
				
			}
		}
	}
}



void AlchemyUILayer::onExchangeListItemClicked(CCObject* sender)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(sender);
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
				unsigned int type = button->getButtonIndex();
				showSelectecdExchangeItem(type);

				if(m_selectedButton)
				{
					CCMenuItem* lastItem = m_selectedButton->getMenuItemSprite();
					if(lastItem)
					{
						lastItem->unselected();
					}
				}
				m_selectedButton = button;
				item->selected();
			}
		}
	}
}

void AlchemyUILayer::onDragInAlchemyBtClicked(CCObject* sender)
{
	unselectUpgradeItem();
}

void AlchemyUILayer::onMoveToBagCallback(CCObject* sender)
{
	OnlineNetworkManager::sShareInstance()->sendSoulStoneToStorage(m_tipPos);
}

void AlchemyUILayer::onUnEquipCallback(CCObject* sender)
{
	unsigned int pos = ItemManager::Get()->getAlchemyBagEmptyGridIndex();
	if(pos != 0)
	{
		pos += kAlchemyStorage;
		OnlineNetworkManager::sShareInstance()->sendBackPackMove(m_tipPos,pos);
	}
	else
	{
		ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_BACKPACKFULL"));
	}
}

void AlchemyUILayer::onEquipOnCallback(CCObject* sender)
{
	unsigned int pos = ItemManager::Get()->getAlchemyEquipEmptyGridIndex(m_currentEquipPage);
	if(pos != 0)
	{
		if(m_currentEquipPage == 0)
		{
			pos += kAlchemyHeroEquip;
		}
		else
		{
			pos += kAlchemyFairyEquip;
		}
		OnlineNetworkManager::sShareInstance()->sendBackPackMove(m_tipPos,pos);
	}
	else
	{
		ShowMessage(Localizatioin::getLocalization("M_ALCHEMY_EQUIPFULL"));
	}
}

//void AlchemyUILayer::onClickFairyEquip(CCObject* sender)
//{
//	if(m_heroEquipToggle)
//	{
//		m_heroEquipToggle->unselected();
//	}
//	if(m_fairyEquipToggle)
//	{
//		m_fairyEquipToggle->selected();
//	}
//	if(m_equipLayoutIndex)
//	{
//		m_equipLayoutIndex = false;
//		showEquipItems(m_equipLayoutIndex);
//		if(m_heroEquipPic)
//		{
//			m_heroEquipPic->setVisible(false);
//		}
//		if(m_fairyEquipPic)
//		{
//			m_fairyEquipPic->setVisible(true);
//		}
//	}
//}

//void AlchemyUILayer::onClickHeroEquip(CCObject* sender)
//{
//	if(m_heroEquipToggle)
//	{
//		m_heroEquipToggle->selected();
//	}
//	if(m_fairyEquipToggle)
//	{
//		m_fairyEquipToggle->unselected();
//	}
//	if(!m_equipLayoutIndex)
//	{
//		m_equipLayoutIndex = true;
//		showEquipItems(m_equipLayoutIndex);
//		if(m_heroEquipPic)
//		{
//			m_heroEquipPic->setVisible(true);
//		}
//		if(m_fairyEquipPic)
//		{
//			m_fairyEquipPic->setVisible(false);
//		}
//	}
//}

void AlchemyUILayer::onClickFindSoul(CCObject* sender)
{
	if(!autoSearchFlag)
	{
		OnlineNetworkManager::sShareInstance()->sendFindSoulStoneMessage();
	}
	
}

void AlchemyUILayer::onClickTopFind(CCObject* sender)
{
	if(!autoSearchFlag)
	{
		onReceivedConfirmTopSearch(NULL);
		//const char* costLabel = Localizatioin::getLocalization("M_UNLOCKCOST");
		/*DiamondWarningLayer::Show("",m_TopSearchCost,
			menu_selector(AlchemyUILayer::onReceivedConfirmTopSearch),
			NULL,this);	*/
	}
}

void AlchemyUILayer::onClickAutoFind(CCObject* sender)
{
	setAutoSearchStatus(!autoSearchFlag);
}

void AlchemyUILayer::onClickTransAllStoneEnergy(CCObject* sender)
{
	if(!autoSearchFlag)
	{
		const char* costLabel = Localizatioin::getLocalization("M_CHANGEALL");
		MessageBox::Show(costLabel,this,menu_selector(AlchemyUILayer::onReceivedConfirmTransAllStones),NULL, MB_YESNO);
	}
}

void AlchemyUILayer::onClickUpgradeStone(CCObject* sender)
{
	if(m_selectedUpgradeItemPos == 0)
	{
		return;
	}

	BackPackItem* item = ItemManager::Get()->findItemByPos(m_selectedUpgradeItemPos);
	if(item)
	{
		OnlineNetworkManager::sShareInstance()->sendUpgradeSoulStone(m_selectedUpgradeItemPos);
	}
}

void AlchemyUILayer::onClickExchangeBt(CCObject* sender)
{
	ItemBase* info = ItemManager::Get()->getItemInfoById(m_selectedExchangeIndex);
	if(info && m_exchangeItemList)
	{
		unsigned int exchangeId = (*m_exchangeItemList)[m_selectedExchangeIndex];
		ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
		if(item && item->m_fromItems && item->m_fromItems->size() == 1)
		{
			std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_fromItems->begin();
			unsigned int count = iter->second;
			if(count <= UserData::GetUserChip())
			{
				OnlineNetworkManager::sShareInstance()->sendItemExchangeReq(exchangeId,NULL,0);
			}
			else
			{
				ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITPIECESTORE"));
			}
		}
		else
		{
			ShowMessage("Error exchange item id");
		}
		
	}
	else
	{
		ShowMessage("Error exchange item");
	}
}

void AlchemyUILayer::onReceivedConfirmTransAllStones(CCObject* sender)
{
	OnlineNetworkManager::sShareInstance()->sendTranStoneToEnergy(true);
}

void AlchemyUILayer::onReceivedConfirmBagItemUnlock(CCObject* sender)
{
	unselectAllBagItem();
	OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(m_unLockExchangeId,NULL,0);
	m_unLockExchangeId = 0;
}

void AlchemyUILayer::onReceivedCancellBagItemUnlock(CCObject* sender)
{
	unselectAllBagItem();
	m_unLockExchangeId = 0;
}

void AlchemyUILayer::onReceivedConfirmTopSearch(CCObject* sender)
{
	OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(20310,NULL,0);
}

void AlchemyUILayer::onHelpButtonClicked(CCObject* sender)
{
	if(!autoSearchFlag)
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/HelpUI/HelpManager.lua","setRecentHelpIndex","AlchemyUILayer");
		int tag = MainMenuLayer::getLayerTagByName("HelpUILayer");
		MainMenuLayer::showUILayerByTag<CCLayer>(tag);
	}
}

void AlchemyUILayer::unselectAllBagItem()
{
	USERINFO info = UserData::GetUserInfo();
	//unsigned int maxPos = info.m_alchemyStorageUnlockPos;
	for(unsigned int i = 0;i < AlCHEMYSTORAGECAPACITY;i++)
	{
		IconButton* button = m_storageListButton[i];
		if(button)
		{
			CCMenuItem* item = button->getMenuItem();
			item->unselected();
		}
	}
}

void AlchemyUILayer::addExchangeItem(unsigned int itemId,unsigned int index)
{
	ItemBase* info = ItemManager::Get()->getItemInfoById(itemId);
	if(info)
	{
		ASprite* pAsprite = AspriteManager::getInstance()->getAsprite(KUI_BIN);
		CCPoint parentPos;
		int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_EXCHANGE_SOUL1");
		CCSprite* normal = pAsprite->getSpriteFromFrame_Middle(iFrameId ,0, parentPos);//map_ui_FRAME_ALCHEMY_EXCHANGE_SOUL1
		iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_EXCHANGE_SOUL1_SELECTED");
		CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle(iFrameId, 0, parentPos);//map_ui_FRAME_ALCHEMY_EXCHANGE_SOUL1_SELECTED
		UIButton* button = m_exchangeList->AddNewItem(normal,
			clicked,
			"AlchemyUILayer::onExchangeListItemClicked",
			"",
			ccWHITE,
			kCCScrollMenuPriority);
		if(button)
		{			
			CCPoint pt;
			int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_EXCHANGE_TEXT_SOUL1NAME");
			CCSprite* sprite = pAsprite->getSpriteFromFrame_Middle(iFrameId,0,pt);
			CCSize labelSize = sprite->getContentSize();
			UILabelTTF* labelName = UILabelTTF::create(info->m_name,KJLinXin,28,CCSize(300,labelSize.height),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
			labelName->setPosition(ccpSub(pt,parentPos));
			labelName->setColor(ItemManager::Get()->getLabelColorByQuality(info->m_quality));
			button->getAnimationNode()->addChild(labelName,2);

			iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_EXCHANGE_TEXT_SOUL1ADD");
			CCSprite* sprite1 = pAsprite->getSpriteFromFrame_Middle(iFrameId,0,pt);
			labelSize = sprite1->getContentSize();


			std::map<unsigned int,stAlchemyExchangeInfo>::iterator it = m_exchangeItemInfo.find(itemId);
			if(it != m_exchangeItemInfo.end())
			{
				const char* text = ItemManager::Get()->getAttributionText(it->second.key,it->second.value);
				if(text)
				{
					UILabelTTF* labelAdd = UILabelTTF::create(text,KJLinXin,24,CCSize(300,labelSize.height),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
					labelAdd->setPosition(ccpSub(pt,parentPos));
					button->getAnimationNode()->addChild(labelAdd,3);
				}
			}
			
			iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_EXCHANGE_ICON_SOUL1");
			CCSprite* iconSprite = pAsprite->getSpriteFromFrame_Middle(iFrameId,0,pt);
			CCSprite* icon = ItemManager::Get()->getIconSpriteById(itemId);
			icon->setPosition(ccpSub(pt,parentPos));
			button->getAnimationNode()->addChild(icon,4);

			button->setButtonIndex(index);
			showSpriteParticleEffect(icon,itemId,ccp(icon->getContentSize().width * 0.5,icon->getContentSize().height * 0.5));
		}
	}
	
}

void AlchemyUILayer::showSelectecdExchangeItem(unsigned int index)
{
	m_selectedExchangeIndex = index;
	ItemBase* info = ItemManager::Get()->getItemInfoById(index);
	std::map<unsigned int,stAlchemyExchangeInfo>::iterator it = m_exchangeItemInfo.find(index);
	if(info && it != m_exchangeItemInfo.end())
	{
		if(m_alchemyExchangeLayout)
		{
			UILabel* nameLabel = m_alchemyExchangeLayout->FindChildObjectByName<UILabel>("alchemyExchangeSelectedLabel");
			if(nameLabel)
			{
				nameLabel->setString(info->m_name);
				nameLabel->setColor(ItemManager::Get()->getLabelColorByQuality(info->m_quality));
			}
			UILabel* numberLabel = m_alchemyExchangeLayout->FindChildObjectByName<UILabel>("alchemyExchangeSelectedNumLabel");
			if(numberLabel)
			{
				const char* text = ItemManager::Get()->getAttributionText(it->second.key,it->second.value);
				if(text)
				{
					numberLabel->setString(text);
				}
				else
				{
					numberLabel->setString("");
				}
			}

			UILabel* needLabel = m_alchemyExchangeLayout->FindChildObjectByName<UILabel>("alchemyExchangeEnergy2Label");
			if(needLabel)
			{
				char text[20];
				sprintf(text,"%d",it->second.consume);
				needLabel->setString(text);
			}

			UIPicture* pic = m_alchemyExchangeLayout->FindChildObjectByName<UIPicture>("alchemyExchangeSelectedIcon");
			if(pic)
			{
				CCSprite* sprite = ItemManager::Get()->getIconSpriteById(info->m_id);
				if(sprite)
				{
					pic->setSprite(sprite);
					showSpriteParticleEffect(sprite,index,ccp(sprite->getContentSize().width * 0.5,sprite->getContentSize().height * 0.5));
			
				}
			}
			//if(m_exchangeItemList)
			//{
			//	ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
			//	if(item && item->m_fromItems && item->m_fromItems->size() == 1)
			//	{
			//		std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_fromItems->begin();
			//		unsigned int count = iter->second;
					
					UILabel* spriteLabel = m_alchemyExchangeLayout->FindChildObjectByName<UILabel>("alchemyExchangePiece2NumberLabel");
					if(spriteLabel)
					{
						char text[10];
						sprintf(text,"%d",it->second.need_chip);
						spriteLabel->setString(text);
					}
			//	}
			//}
		}
	}
	
}

void AlchemyUILayer::closeLayerCallBack(void)
{
	int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
	MainMenuLayer::hideUILayerByTag(tag);
}


void AlchemyUILayer::showAlchemyLayer()
{
	if(m_uiLayout)
	{
		m_uiLayout->setVisible(true);
	}
	GameAudioManager::sharedManager()->playEffect(350001,false);
}

void AlchemyUILayer::hideAlchemyLayer()
{
	if(m_uiLayout)
	{
		m_uiLayout->setVisible(false);
	}
}

void AlchemyUILayer::showAlchemyEquipLayer()
{
	if(!m_alchemyEquipLayout)
	{
		m_alchemyEquipLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/alchemyEquipUI.plist", this, "alchemyEquipLayout");
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onClosedEquipBtClicked",
			menu_selector(AlchemyUILayer::onClosedEquipBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onDragInAlchemyBtClicked",
			menu_selector(AlchemyUILayer::onDragInAlchemyBtClicked), this);
		/*UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onClickFairyEquip",
			menu_selector(AlchemyUILayer::onClickFairyEquip), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onClickHeroEquip",
			menu_selector(AlchemyUILayer::onClickHeroEquip), this);*/
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onFairyListItemClicked",
                                 menu_selector(AlchemyUILayer::onFairyListItemClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onClickUpgradeStone",
			menu_selector(AlchemyUILayer::onClickUpgradeStone), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onClosedBtClicked",
			menu_selector(AlchemyUILayer::onClosedBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onEquipClicked",
			menu_selector(AlchemyUILayer::onEquipClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyEquipLayout","AlchemyUILayer::onHelpButtonClicked2",
		menu_selector(AlchemyUILayer::onHelpButtonClicked), this);
	
		initAlchemistStorage();
	}
	if(m_alchemyEquipLayout)
	{
		m_alchemyEquipLayout->setVisible(true);
	}
	GameAudioManager::sharedManager()->playEffect(350001,false);
}

void AlchemyUILayer::hideAlchemyEquipLayer()
{
	if(m_alchemyEquipLayout)
	{
		m_alchemyEquipLayout->setVisible(false);
	}
}

void AlchemyUILayer::showAlchemyExchangeLayer()
{
	if(!m_alchemyExchangeLayout)
	{
		m_alchemyExchangeLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/alchemyExchangeUI.plist", this, "alchemyExchangeLayout");
		UIManager::sharedManager()->RegisterMenuHandler("alchemyExchangeLayout","AlchemyUILayer::onClosedExchangeBtClicked",
			menu_selector(AlchemyUILayer::onClosedExchangeBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyExchangeLayout","AlchemyUILayer::onExchangeListItemClicked",
			menu_selector(AlchemyUILayer::onExchangeListItemClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyExchangeLayout","AlchemyUILayer::onClickExchangeBt",
			menu_selector(AlchemyUILayer::onClickExchangeBt), this);
		UIManager::sharedManager()->RegisterMenuHandler("alchemyExchangeLayout","AlchemyUILayer::onClosedBtClicked",
			menu_selector(AlchemyUILayer::onClosedBtClicked), this);
			UIManager::sharedManager()->RegisterMenuHandler("alchemyExchangeLayout","AlchemyUILayer::onHelpButtonExchangeClicked",
		menu_selector(AlchemyUILayer::onHelpButtonClicked), this);
		initAlchemistExchange();
	}
	if(m_alchemyExchangeLayout)
	{
		m_alchemyExchangeLayout->setVisible(true);
	}
	GameAudioManager::sharedManager()->playEffect(350001,false);
}

void AlchemyUILayer::hideAlchemyExchangeLayer()
{
	if(m_alchemyExchangeLayout)
	{
		m_alchemyExchangeLayout->setVisible(false);
	}
}

void AlchemyUILayer::setItemIcon(BackPackItem* item,unsigned int index)
{
	if(index >= kAlchemyFairyEquip)
	{
		if(m_currentEquipPage == (index - kAlchemyFairyEquip) / 100 + 1)
		{
			unsigned int pos = (index - kAlchemyFairyEquip) % 100;
			setEquipItemIcon(item,pos);
		}
	}
	if(index >= kAlchemyHeroEquip)
	{
		if(m_currentEquipPage == 0)
		{
			unsigned int pos = index - kAlchemyHeroEquip;
			setEquipItemIcon(item,pos);
		}
	}
	else if(index >= kAlchemyStorage)
	{
		unsigned int pos = index - kAlchemyStorage;
		setStorageItemIcon(item,pos);
	}
	else if(index >= kAlchemyBag)
	{
		unsigned int pos = index - kAlchemyBag;
		setBagItemIcon(item,pos);
	}
	else
	{
		CCLOG("AlchemyUILayer ,error item index :%d",index);
	}
}

void AlchemyUILayer::setBagItemIcon(BackPackItem* item,unsigned int position)
{
	if(item)
	{
		CCAssert(position != 0,"Error alchemy position");
		if(m_uiLayout && m_uiLayout->isVisible())
		{
			IconButton* button = m_bagListButton[position - 1];
			if(button)
			{
				if(item->isLocked)
				{
					ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
					CCPoint pt;
					int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_STORAGE_GRID1_LOCKED");
					CCSprite * lockPic = as->getSpriteFromFrame_Middle(iFrameId, 0, pt);
					if(lockPic)
					{
						lockPic->setScale(UIManager::sharedManager()->getScaleFactor());
						button->SetNormalIcon(lockPic);
						//button->setFrameSprite(NULL);
					}
				}
				else if(item->isEmpty)
				{
					button->SetNormalIcon(NULL);
					//button->setFrameSprite(NULL);
				}
				else
				{
					CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item->getItemId());
					/*ItemBase* itemInfo = item->getItemInfo();
					if(itemInfo && itemInfo->m_quality > 0)
					{
						CCSprite* framePic = ItemManager::Get()->getIconFrame(itemInfo->m_quality);
						if(framePic)
						{
							framePic->setScale(UIManager::sharedManager()->getScaleFactor());
							button->setFrameSprite(framePic);
						}
					}*/
					sprite->setScale(UIManager::sharedManager()->getScaleFactor());
					button->SetNormalIcon(sprite);
					button->creatDragReceiver(EAlchemyBag);
					button->setDragMove(true);
					showSpriteParticleEffect(sprite,item->getItemId(),ccp(sprite->getContentSize().width * 0.5,sprite->getContentSize().height * 0.5));
				}

			}
		}
	}
	
}

void AlchemyUILayer::setStorageItemIcon(BackPackItem* item,unsigned int position)
{
	if(item)
	{
		CCAssert(position != 0,"Error alchemy position");
		if(m_alchemyEquipLayout && m_alchemyEquipLayout->isVisible())
		{
			IconButton* button = m_storageListButton[position-1];
			if(button)
			{
				if(item->isLocked)
				{
					ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
					CCPoint pt;
					int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_STORAGE_GRID1_LOCKED");
					CCSprite * lockPic = as->getSpriteFromFrame_Middle(iFrameId, 0, pt);
					if(lockPic)
					{
						lockPic->setScale(UIManager::sharedManager()->getScaleFactor());
						button->SetNormalIcon(lockPic);
						//button->setFrameSprite(NULL);
					}
				}
				else if(item->isEmpty)
				{
					button->SetNormalIcon(NULL);
					//button->setFrameSprite(NULL);
				}
				else
				{
					CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item->getItemId());
					/*ItemBase* itemInfo = item->getItemInfo();
					if(itemInfo && itemInfo->m_quality > 0)
					{
						CCSprite* framePic = ItemManager::Get()->getIconFrame(itemInfo->m_quality);
						if(framePic)
						{
							framePic->setScale(UIManager::sharedManager()->getScaleFactor());
							button->setFrameSprite(framePic);
						}
					}*/
					sprite->setScale(UIManager::sharedManager()->getScaleFactor());
					button->SetNormalIcon(sprite);
					showSpriteParticleEffect(sprite,item->getItemId(),ccp(sprite->getContentSize().width * 0.5,sprite->getContentSize().height * 0.5));
					button->creatDragReceiver(EAlchemyBag);
					button->setDragMove(true);
					
				}

			}
		}
	}
}

void AlchemyUILayer::setEquipItemIcon(BackPackItem* item,unsigned int position)
{
	if(m_alchemyEquipLayout && m_alchemyEquipLayout->isVisible())
	{
		char buttonName[30];
		sprintf(buttonName,"alchemyEquipGrid%d",position);
		UIButton* button = m_alchemyEquipLayout->FindChildObjectByName<UIButton>(buttonName);
		if(!button)
		{
			return;
		}
		if(item)
		{
			if(item->isLocked)
			{
				ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
				CCPoint pt;
				int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_STORAGE_GRID1_LOCKED");
				CCSprite * lockPic = as->getSpriteFromFrame_Middle( iFrameId, 0, pt);
				if(lockPic)
				{
					button->setIconSprite(lockPic);
				}
				setUnlockEqiupLabel(position,true);
			}
			else 
			{
				setUnlockEqiupLabel(position,false);
				if(item->isEmpty)
				{
					button->setIconSprite(NULL);
				}
				else
				{		
					CCSprite* equipPic = ItemManager::Get()->getIconSpriteById(item->getItemId());
					if(equipPic)
					{	
						button->setIconSprite(equipPic);
						showSpriteParticleEffect(equipPic,item->getItemId(),ccp(equipPic->getContentSize().width * 0.5,equipPic->getContentSize().height * 0.5));
						button->setDragMove(true);
					}
				}
			}
		}
		else
		{
			if(button)
			{
				button->setIconSprite(NULL);
			}
		}
	}
}

void AlchemyUILayer::setUnlockEqiupLabel(int index,bool isVisible)
{
	char labelName[30];
	sprintf(labelName,"alchemyEquipGridLabel%d",index);
	UILabel* label = m_alchemyEquipLayout->FindChildObjectByName<UILabel>(labelName);
	if(label)
	{
		label->setVisible(isVisible);
	}
}

void AlchemyUILayer::selectUpgradeItem(unsigned int position,bool isEffect)
{
	if(position >= (kAlchemyFairyEquip + 900) || position < kAlchemyStorage)
	{
		CCLOG("error upgrade item pos : %d",position);
		return;
	}

	BackPackItem* item = ItemManager::Get()->findItemByPos(position);
	if(item)
	{
		UIButton* button = m_alchemyEquipLayout->FindChildObjectByName<UIButton>("alchemyDragInSoulIcon");
		CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item->getItemId());
		if(button && sprite)
		{
			m_selectedUpgradeItemPos = position;
			button->setIconSprite(sprite);

			if(isEffect)
			{
				if(button && button->getAnimationNode())
				{
					CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_soulupgrade");
					if(particle != NULL)
					{
						CCNode* child = button->getAnimationNode()->getChildByTag(100);
						if(child)
						{
							child->removeFromParentAndCleanup(true);
							child = NULL;
						}
						button->getAnimationNode()->addChild(particle,100,100);
						particle->setPosition(CCPointZero);
					}
				}
			}
		}
		if(item->m_spriteInfo)
		{
			UILabel* label = m_alchemyEquipLayout->FindChildObjectByName<UILabel>("alchemyEnergyNeedLabel");
			if(label)
			{
				if(item->m_spriteInfo->levelUpCost != 0)
				{
					USERINFO info = UserData::GetUserInfo();
					char spNeed[50];
					sprintf(spNeed,"%d",item->m_spriteInfo->levelUpCost);
					label->setString(spNeed);
					if(info.m_spriteEnergy >= item->m_spriteInfo->levelUpCost)
					{
						label->setColor(ccGREEN);	
					}
					else
					{
						label->setColor(ccRED);
					}
				}
				else
				{
					label->setColor(ccGREEN);
					label->setString(Localizatioin::getLocalization("M_REACH_HIGHESTLEVEL"));
				}
			}
			setEnergyIconVisible(true);
		}
	}
}

void AlchemyUILayer::unselectUpgradeItem()
{
	m_selectedUpgradeItemPos = 0;
	UIButton* button = m_alchemyEquipLayout->FindChildObjectByName<UIButton>("alchemyDragInSoulIcon");
	if(button)
	{
		button->setIconSprite(NULL);
		button->setDragMove(false);
	}
	UILabel* label = m_alchemyEquipLayout->FindChildObjectByName<UILabel>("alchemyEnergyNeedLabel");
	if(label)
	{
		label->setString("");
	}
	setEnergyIconVisible(false);
}

void AlchemyUILayer::showEquipItems(bool isHero)
{
	/*m_equipLayoutIndex = isHero;
	if(m_equipLayoutIndex)
	{
		showHeroEquipItems();
	}
	else
	{
		showFairyEquipItems();
	}*/
}

void AlchemyUILayer::showHeroEquipItems()
{
	for(unsigned int i=1;i<= 8;i++)
	{
		unsigned index = kAlchemyHeroEquip + m_currentEquipPage * 100;
		BackPackItem* item = ItemManager::Get()->findItemByPos(index+i);
		if(item)
		{
			setEquipItemIcon(item,i);
		}
	}
}

//void AlchemyUILayer::showFairyEquipItems()
//{
//	for(unsigned int i=1;i<= 8;i++)
//	{
//		BackPackItem* item = ItemManager::Get()->findItemByPos(kAlchemyFairyEquip+i);
//		if(checkPlayerEquipUnlock(i))
//		{
//			item->isLocked = false;
//		}
//		else
//		{
//			item->isLocked = true;
//		}
//		setEquipItemIcon(item,i);
//	}
//}

void AlchemyUILayer::onReceivedUpgradeSuccess()
{
	selectUpgradeItem(m_selectedUpgradeItemPos,true);
	GameAudioManager::sharedManager()->playEffect(350004,false);
}

void AlchemyUILayer::onReceivedSearchResult(unsigned int coin,unsigned int npcIndex,unsigned int itemId)
{
	unsigned int cost = ItemManager::Get()->getAlchemySearchNpcCost(m_selectedNpcIndex);
	if(cost != coin)
	{
		CCLOG("search sprite stone fail return  coin: %d, next npc: %d",(cost - coin),npcIndex);
		char text[50];
		sprintf(text,"%s%d",Localizatioin::getLocalization("M_FINDSPIRIT_FAIL_RESTORE"),cost - coin);
		ShowMessage(text,ccRED);
		GameAudioManager::sharedManager()->playEffect(350003,false);
	}
	else
	{
		ItemBase* info = ItemManager::Get()->getItemInfoById(itemId);
		if(info)
		{
			char text[50];
			sprintf(text,"%s%s",Localizatioin::getLocalization("M_FINDSPIRIT_SUCCESS_GET"),info->m_name);
			ShowMessage(text,ccGREEN);
			GameAudioManager::sharedManager()->playEffect(350002,false);
		}
		
		CCLOG("search sprite stone success cost: %d, next npc: %d,itemId: %d",coin,npcIndex,itemId);
	}
	bool isEffect = npcIndex > m_selectedNpcIndex ? true : false;
	setNpcSelected(npcIndex,isEffect);
}

void AlchemyUILayer::onReceivedTopSearch()
{
	setNpcSelected(4,true);
}

void AlchemyUILayer::onReceivedSearchError(unsigned err)
{
	setAutoSearchStatus(false);
}

void AlchemyUILayer::setNpcSelected(unsigned int index,bool isEffect)
{
	if(index == m_selectedNpcIndex)
	{
		return;
	}
	char lastNpcName[30];
	sprintf(lastNpcName,"alchemyNpcPic%d",m_selectedNpcIndex);
	UIButton* bt = m_uiLayout->FindChildObjectByName<UIButton>(lastNpcName);
	if(bt)
	{
		CCMenuItemSprite*  item = bt->getMenuItemSprite();
		if(item)
		{	
			item->unselected();
		}	

	}

	char iconName[30];
	sprintf(iconName,"alchemyNpcPic%d",index);
	bt = m_uiLayout->FindChildObjectByName<UIButton>(iconName);	
	if(bt)
	{
		CCMenuItemSprite*  item = bt->getMenuItemSprite();
		if(item)
		{		
			item->selected();
			if(isEffect)
			{
				CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_findsoulsuccess");
				if(particle != NULL)
				{
					CCNode* child = bt->getAnimationNode()->getChildByTag(100);
					if(child)
					{
						child->removeFromParentAndCleanup(true);
						child = NULL;
					}
					bt->getAnimationNode()->addChild(particle,100,100);
					particle->setPosition(CCPointZero);
				}
			}
		}		
		
	}

	UILabel* pLabel = m_uiLayout->FindChildObjectByName<UILabel>("costNumberLable");
	if(pLabel)
	{
		unsigned int cost = ItemManager::Get()->getAlchemySearchNpcCost(index);
		char costText[10];
		sprintf(costText,"%d",cost);
		pLabel->setString(costText);
	}
	setNpcBigPic(index);
	m_selectedNpcIndex = index;
}

void AlchemyUILayer::setNpcBigPic(unsigned int index)
{
	UIPicture* pic = m_uiLayout->FindChildObjectByName<UIPicture>("alchemyNpcFrame");
	if(pic)
	{
		char picName[40];
		sprintf(picName,"UI/ui_alchemy_pic%d.png",index);
		const char* new_picName = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(picName);
		CCSprite* sprite = CCSprite::create(new_picName);
		pic->setSprite(sprite,false);
	}
}

void AlchemyUILayer::setStoneEnergyLabel()
{
	if(m_alchemyEquipLayout && m_alchemyEquipLayout->isVisible())
	{
		UILabel* label = m_alchemyEquipLayout->FindChildObjectByName<UILabel>("alchemyEnergyInallLabel");
		if(label)
		{
			unsigned int sp = UserData::GetSpriteEnergy();
			char spNum[20];
			sprintf(spNum,"%d",sp);
			label->setString(spNum);
		}
	}
	if(m_alchemyExchangeLayout && m_alchemyExchangeLayout->isVisible())
	{
		UILabel* label = m_alchemyExchangeLayout->FindChildObjectByName<UILabel>("alchemyExchangeEnergyLabel");
		if(label)
		{
			unsigned int sp = UserData::GetSpriteEnergy();
			char spNum[20];
			sprintf(spNum,"%d",sp);
			label->setString(spNum);
		}
	}
	
}

void AlchemyUILayer::setSpiritChipLabel()
{
	if(m_alchemyExchangeLayout && m_alchemyExchangeLayout->isVisible())
	{
		UILabel* label = m_alchemyExchangeLayout->FindChildObjectByName<UILabel>("alchemyExchangePieceLabel");
		if(label)
		{
			unsigned int sp = UserData::GetUserChip();
			char spNum[20];
			sprintf(spNum,"%d",sp);
			label->setString(spNum);
		}
	}
}

void AlchemyUILayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_ATTR_SP_UPDATE:
		setStoneEnergyLabel();
		updateBottomSpLabel();
		break;
	case GM_ATTR_SP_CHIP_UPDATE:
		setSpiritChipLabel();
		updateBottomPieceLabel();
		break;
	case GM_ATTR_SP_DIAMOND_UPDATE:
		updateBottomDiamondLabel();
		break;
	case GM_ATTR_COIN_UPDATE:
		updateBottomCoinLabel();
		break;
	case GM_ATTR_BUY_SS_BAG_CAPACITY:
		{
			for(unsigned int i=1;i<=AlCHEMYSTORAGECAPACITY;i++)
			{
				unsigned int pos = i+kAlchemyStorage;
				BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
				if(item)
				{
					setItemIcon(item,pos);
				}
			}
			break;
		}
	}
}

void AlchemyUILayer::showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt)
{
	ItemManager::Get()->showSpriteParticleEffect(parent,id,pt);
}

void AlchemyUILayer::playTransEffect(UIButton* parentPt)
{
	if(parentPt && parentPt->getAnimationNode())
	{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_transformelite");
		if(particle != NULL)
		{
			parentPt->getAnimationNode()->addChild(particle);
			particle->setPosition(CCPointZero);
		}
	}
}

void AlchemyUILayer::onReceiveSpriteTrans()
{
	if(m_uiLayout && m_uiLayout->getCurrentNode() && m_uiLayout->getCurrentNode()->isVisible())
	{
		UIButton* bt = m_uiLayout->FindChildObjectByName<UIButton>("transformBt");
		if(bt)
		{
			playTransEffect(bt);
		}
	}
	if(m_alchemyEquipLayout && m_alchemyEquipLayout->getCurrentNode() && m_alchemyEquipLayout->getCurrentNode()->isVisible())
	{
		UIButton* bt = m_alchemyEquipLayout->FindChildObjectByName<UIButton>("transformBt");
		if(bt)
		{
			playTransEffect(bt);
		}
	}
	GameAudioManager::sharedManager()->playEffect(350009,false);
}

void AlchemyUILayer::setEnergyIconVisible(bool isVisible)
{
	if(m_alchemyEquipLayout)
	{
		UIPicture* pic = m_alchemyEquipLayout->FindChildObjectByName<UIPicture>("alchemyFrameDragInSoul1");
		if(pic)
		{
			pic->setVisible(isVisible);
		}
	}
}

void AlchemyUILayer::updateBottomDiamondLabel()
{
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("bottomDiamondLable");
		if(label)
		{
			unsigned int diamond = UserData::GetDiamond();
			const char* str = MainMenuLayer::GetNumByFormat(diamond);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}
void AlchemyUILayer::updateBottomCoinLabel()
{
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("bottomCoinLable");
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
void AlchemyUILayer::updateBottomPieceLabel()
{
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("bottomPieceLable");
		if(label)
		{
			unsigned int coin = UserData::GetUserChip();
			const char* str = MainMenuLayer::GetNumByFormat(coin);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}
void AlchemyUILayer::updateBottomSpLabel()
{
	if(m_uiLayout)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("bottomSoulLable");
		if(label)
		{
			unsigned int coin = UserData::GetSpriteEnergy();
			const char* str = MainMenuLayer::GetNumByFormat(coin);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}

void AlchemyUILayer::reqToUnlockBagItem(unsigned int pos)
{

	unsigned int startPos = kAlchemyStorage;
	unsigned int startParamId = 40130;

	std::list<unsigned int> bagLockedList;
	for(unsigned int i = startPos +1;i <= pos; i++)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(i);
		if(item && item->isLocked)
		{
			bagLockedList.push_back(i); // 按从小到大顺序排列
		}
	}

	if(bagLockedList.size() > 0)
	{
		unsigned int diamondCost = 0;
		for(std::list<unsigned int>::iterator it = bagLockedList.begin();
			it != bagLockedList.end();it++)
		{
			unsigned int index = *it - startPos;
	
			// 增加选中效果
			IconButton* button = m_storageListButton[index - 1];
			if(button)
			{
				CCMenuItem* item = button->getMenuItem();
				item->selected();
			}	
		
			if(index >= 7)
			{
				int paramId = startParamId; // 钻石解锁花费ID
				paramId += 10 * (index - 7);
				const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", paramId, "FromItems");
				string exchangeStr = string(fromItems);
				int dashPos = exchangeStr.find_first_of("/");
				if(dashPos != exchangeStr.npos)
				{
					unsigned int length = exchangeStr.size();
					std::string idStr = exchangeStr.substr(0,dashPos);
					std::string numStr = exchangeStr.substr(dashPos+1,length-1);
					int id = atoi(idStr.c_str());
					ASSERT(id == 416002,"cost is not diamond!");
					int price = atoi(numStr.c_str());
					diamondCost += price;
				}
				m_unLockExchangeId = paramId; // 解锁的ID取最大值
			}
		}
		const char* costLabel = Localizatioin::getLocalization("M_UNLOCKCOST");
		DiamondWarningLayer::Show(costLabel,diamondCost,
			menu_selector(AlchemyUILayer::onReceivedConfirmBagItemUnlock),
			menu_selector(AlchemyUILayer::onReceivedCancellBagItemUnlock),this);	
	}
}