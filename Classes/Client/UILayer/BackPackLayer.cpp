#include "OnlineNetworkManager.h"
#include "PVPMessage.h"
#include "TXGUIHeader.h"
#include "BackPackLayer.h"
#include "AspriteManager.h"
#include "ASprite.h"
#include "ItemManager.h"
#include "SpriteSeer.h"
#include "DragReceiver.h"
#include "GameFontManager.h"
#include "UserData.h"
#include "MainMenuLayer.h"
#include "EquipMixLayer.h"
#include "HeroRoleManager.h"
#include "SpriteElfConfigFromLua.h"
#include "TipMgr.h"
#include "GMessage.h"
#include "GameAudioManager.h"
#include "ShopUILayer.h"
#include "Localization.h"
#include "LuaTinkerManager.h"
#include "DiamondWarningLayer.h"
#include "SpriteFactory.h"
#include "MainLandManager.h"
#include "LevelManager.h"
using namespace TXGUI;

BackPackLayer::BackPackLayer()
{
    this->m_pShopUILayer = NULL;
    this->m_shopUIReceiver = NULL;
	m_unLockExchangeId = 0;
	b_isUnlockBagFlag = true;
    m_DagPos = 0;
	m_tipPos = 0;
	m_currentEquipPage = 0;
	m_fragScrollPage = NULL;
	b_isBagPageFlag = true;
	m_bagIndexToggle = NULL;
	m_fragIndexToggle = NULL;
	m_fairyBtMap = NULL;
	m_lastClickedFairyIndex = -1;
	b_isInitModle = false;
	m_isFairySelectModle = false;
	m_showedVipLevel = 0;
}

BackPackLayer::~BackPackLayer()
{
	UIManager::sharedManager()->RemoveUILayout("BackPack");
    
    if(m_shopUIReceiver)
    {
        DragReceiverRegister::sharedManager()->unregisterDragReceiver(m_shopUIReceiver);
		delete m_shopUIReceiver;
		m_shopUIReceiver = NULL;
    }
    
	if(m_backPackReceiver)
	{
		DragReceiverRegister::sharedManager()->unregisterDragReceiver(m_backPackReceiver);
		delete m_backPackReceiver;
		m_backPackReceiver = NULL;
	}
	if(m_storageReceiver)
	{
		DragReceiverRegister::sharedManager()->unregisterDragReceiver(m_storageReceiver);
		delete m_storageReceiver;
		m_storageReceiver = NULL;
	}

	if(m_equipmentReceiver)
	{
		DragReceiverRegister::sharedManager()->unregisterDragReceiver(m_equipmentReceiver);
		delete m_equipmentReceiver;
		m_equipmentReceiver = NULL;
	}

	if(m_fairyBtMap != NULL)
	{
		m_fairyBtMap->clear();
		m_fairyBtMap = NULL;
	}

	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

bool BackPackLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
    //createShopLayer();	
    
	m_curUiAs = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
	UIManager *manager = UIManager::sharedManager();

	backPackLayout = manager->CreateUILayoutFromFile("UIplist/backPackUI.plist", this, "BackPack");

	//manager->CreateUILayoutFromFile("UIplist/equipmentMix.plist", "equipmentMixLayout");
	m_backPackContainer = backPackLayout->FindChildObjectByName<UIContainer>("backPackContainer");
	m_storageContainer = backPackLayout->FindChildObjectByName<UIContainer>("storageContainer");
    m_equipmentContainer = backPackLayout->FindChildObjectByName<UIContainer>("equipmentContainer");
    m_fragIndexToggle = backPackLayout->FindChildObjectByName<UIButtonToggle>("fragButtonToggle");
	m_bagIndexToggle = backPackLayout->FindChildObjectByName<UIButtonToggle>("bagButtonToggle");
	manager->RegisterMenuHandler("BackPack","BackPackLayer::closeBackPack",
		menu_selector(BackPackLayer::closeBackPack), this);

	manager->RegisterMenuHandler("BackPack","BackPackLayer::onItemClicked",
		menu_selector(BackPackLayer::onItemClicked), this);

	manager->RegisterMenuHandler("BackPack","BackPackLayer::openBackIndex",
		menu_selector(BackPackLayer::openBackIndex), this);

	/*manager->RegisterMenuHandler("BackPack","BackPackLayer::closeStorage",
		menu_selector(BackPackLayer::closeStorage), this);*/

	manager->RegisterMenuHandler("BackPack","BackPackLayer::reOrderBackPackItems",
		menu_selector(BackPackLayer::reOrderBackPackItems), this);
	
	/*manager->RegisterMenuHandler("BackPack","BackPackLayer::reOrderStorageItems",
		menu_selector(BackPackLayer::reOrderStorageItems), this);*/

	manager->RegisterMenuHandler("BackPack","BackPackLayer::onFragItemClicked",
		menu_selector(BackPackLayer::onFragItemClicked), this);
	
    manager->RegisterMenuHandler("BackPack","BackPackLayer::closeEquipment",
                                 menu_selector(BackPackLayer::closeEquipment), this);
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onEquipmentItemClicked",
		menu_selector(BackPackLayer::onEquipmentItemClicked), this);
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onEquipAllBtClicked",
		menu_selector(BackPackLayer::onEquipAllBtClicked), this);
	/*manager->RegisterMenuHandler("BackPack","BackPackLayer::onFetterInfoClicked",
		menu_selector(BackPackLayer::onFetterInfoClicked), this);
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onFairyInfoClicked",
		menu_selector(BackPackLayer::onFairyInfoClicked), this);*/
	
	/*manager->RegisterMenuHandler("BackPack","BackPackLayer::TrainButtonClicked",
		menu_selector(BackPackLayer::TrainButtonClicked), this);*/	
	
	manager->RegisterMenuHandler("BackPack","BackPackLayer::openAttributeBoard",
		menu_selector(BackPackLayer::openAttributeBoard), this);	

	manager->RegisterMenuHandler("BackPack","BackPackLayer::closeAttributeBoard",
		menu_selector(BackPackLayer::closeAttributeBoard), this);
 
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onAlchemyLeftBtClicked",
		menu_selector(BackPackLayer::onAlchemyLeftBtClicked), this);
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onAlchemyRightBtClicked",
		menu_selector(BackPackLayer::onAlchemyRightBtClicked), this);
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onAlchemyItemClicked",
		menu_selector(BackPackLayer::onAlchemyItemClicked), this);

    manager->RegisterMenuHandler("BackPack","BackPackLayer::openEquipFragIndex",
                                 menu_selector(BackPackLayer::openEquipFragIndex), this);
    manager->RegisterMenuHandler("BackPack","BackPackLayer::onFairyListItemClicked",
                                 menu_selector(BackPackLayer::onFairyListItemClicked), this);
	manager->RegisterMenuHandler("BackPack","BackPackLayer::onChangeFairyClicked",
                                 menu_selector(BackPackLayer::onChangeFairyClicked), this);
	m_backPackScrollPage = backPackLayout->FindChildObjectByName<UIScrollPage>("backPackScrollPage");
	m_storageScrollPage = backPackLayout->FindChildObjectByName<UIScrollPage>("storageScrollPage");
    m_fragScrollPage = backPackLayout->FindChildObjectByName<UIScrollPage>("fragScrollPage");

	UIPicture* fairyPic = backPackLayout->FindChildObjectByName<UIPicture>("fairyNonePic");
	if(fairyPic)
	{
		CCSprite* sprite = CCSprite::create("UI/ui_fairy_lock.png");
		if(sprite != NULL)
		{
			fairyPic->setSprite(sprite,false);
		}
	}
	//scrollLayer->setShowPagesIndicator(false);
	b_isBackPackOpened = false;
	b_isStorageOpened = false;
    b_isEquipmentOpened = false;
	b_isAttributeOpened = false;
	m_lockPicName = "map_ui_FRAME_STORAGE_GRID1_LOCKED";

	setTouchEnabled(true); 
	if(m_backPackContainer && m_backPackContainer->getCurrentNode())
	{
		m_backPackContainer->getCurrentNode()->setVisible(false);
	}
	if(m_storageContainer && m_storageContainer->getCurrentNode())
	{
		m_storageContainer->getCurrentNode()->setVisible(false);
	}
    if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
    {
        m_equipmentContainer->getCurrentNode()->setVisible(false);
    }

	//m_playerExpLable = (CCSprite*)backPackLayout->FindChildObjectByName<UIPicture>("equipmentExpBarPic")->getCurrentNode();
	//m_playerExpLable->setAnchorPoint(ccp(0,0.5f));
	//CCPoint oldPos = m_playerExpLable->getPosition();
	//float scale = UIManager::sharedManager()->getScaleFactor();
	//m_playerExpLable->setPosition(ccp(oldPos.x-scale * m_playerExpLable->getContentSize().width/2,oldPos.y));

	UILabelTTF* m_userName = (UILabelTTF*)backPackLayout->FindChildObjectByName<UILabel>("playerNameLabel")->getCurrentNode();
	m_userName->setString(UserData::getUserName());

	UIPicture* pic = backPackLayout->FindChildObjectByName<UIPicture>("fairyBgPic");
	CCSprite* sprite = CCSprite::create("UI/ui_fairy_friend.png");
	pic->setSprite(sprite,false);

	initBgTouchArea();
	initButtonPriority();
	initAlchemyButton();
	openBackIndex(NULL);
	showPlayerAlchemy(false);
	//m_playModleLayer = NULL;
	//m_elfModleLayer = NULL;
	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

	scheduleOnce(schedule_selector(BackPackLayer::initLuaNextFrame),0.01f);

	return true;
}

void BackPackLayer::initLuaNextFrame(float dt)
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/backPackLayer.lua","InitBackPack");
}

void BackPackLayer::setBagIndex(bool flag)
{
	b_isBagPageFlag = flag;
	if(m_backPackScrollPage)
	{
		m_backPackScrollPage->setVisible(flag);
	}
	if(m_fragScrollPage)
	{
		m_fragScrollPage->setVisible(!flag);
	}
}

void BackPackLayer::initBgTouchArea()
{
	UIPicture* bgPic = backPackLayout->FindChildObjectByName<UIPicture>("backPackPic");
	if(bgPic)
	{
		m_packbgPoint = bgPic->getSpritePos();
		m_packbgSize = bgPic->getSpriteSize();
	}
	else
	{
		m_packbgPoint = CCPointZero;
		m_packbgSize = CCSizeZero;
	}

	UIPicture* storagePic = backPackLayout->FindChildObjectByName<UIPicture>("storagePic");
	if(storagePic)
	{
		m_storagebgPoint = storagePic->getSpritePos();
		m_storagebgSize = storagePic->getSpriteSize();
	}
	else
	{
		m_storagebgPoint = CCPointZero;
		m_storagebgSize = CCSizeZero;
	}

	UIPicture* equipmentPic = backPackLayout->FindChildObjectByName<UIPicture>("equipmentBgPic");
	if(equipmentPic)
	{
		m_equipmentbgPoint = equipmentPic->getSpritePos();
		m_equipmentbgSize = equipmentPic->getSpriteSize();
	}
	else
	{
		m_equipmentbgPoint = CCPointZero;
		m_equipmentbgSize = CCSizeZero;
	}

	m_backPackReceiver = new DragReceiver(kCCScrollTouchBgPriority,EUIBg,0);
	m_backPackReceiver->setCheckDragFunc(checkDrag_selector(BackPackLayer::checkInBackPackBg),this);
	DragReceiverRegister::sharedManager()->registerDragReceiver(m_backPackReceiver);

	m_storageReceiver = new DragReceiver(kCCScrollTouchBgPriority,EUIBg,1);
	m_storageReceiver->setCheckDragFunc(checkDrag_selector(BackPackLayer::checkInStoragekBg),this);
	DragReceiverRegister::sharedManager()->registerDragReceiver(m_storageReceiver);

	m_equipmentReceiver = new DragReceiver(kCCScrollTouchBgPriority,EUIBg,2);
	m_equipmentReceiver->setCheckDragFunc(checkDrag_selector(BackPackLayer::checkInEquipmentBg),this);
	DragReceiverRegister::sharedManager()->registerDragReceiver(m_equipmentReceiver);
    
    m_shopUIReceiver = new DragReceiver(kCCScrollTouchBgPriority,EUIBg,3);
	m_shopUIReceiver->setCheckDragFunc(checkDrag_selector(BackPackLayer::checkInShopUIPackBg),this);
	DragReceiverRegister::sharedManager()->registerDragReceiver(m_shopUIReceiver);
}

void BackPackLayer::initButtonPriority()
{
	// 调整按钮响应优先级，使其高于背景面板和滑动面板
	UIButton* button = backPackLayout->FindChildObjectByName<UIButton>("SortItemsBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("StoreHouseBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("ShopBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("closeBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("storageCloseBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("StorageSortItemsBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("equipmentCloseBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	button = backPackLayout->FindChildObjectByName<UIButton>("playerAttributeBt");
	if(button)
	{
		button->setMenuInitPriority(kCCScrollMenuPriority);
	}

	m_equipmentHelmetButton = backPackLayout->FindChildObjectByName<UIButton>("helmetGrid");
	m_equipmentNecklaceButton = backPackLayout->FindChildObjectByName<UIButton>("necklaceGrid");
	m_equipmentWeaponButton = backPackLayout->FindChildObjectByName<UIButton>("weaponGrid");
	m_equipmentArmorButton = backPackLayout->FindChildObjectByName<UIButton>("armorGrid");
	m_equipmentRingButton = backPackLayout->FindChildObjectByName<UIButton>("ringGrid");
	m_equipmentShoeButton = backPackLayout->FindChildObjectByName<UIButton>("shoeGrid");
}

void BackPackLayer::initAlchemyButton()
{
	if(backPackLayout)
	{
		for(int i =1;i<9;i++)
		{
			UIButton* grid = backPackLayout->FindChildObjectByName<UIButton>(CCString::createWithFormat("alchemyGrid%d",i)->m_sString.c_str());
			if(grid)
			{
				CCSprite* normalSprite = AspriteManager::getInstance()->getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_FRAME_SOUL");
				CCSprite* clickedSprite = AspriteManager::getInstance()->getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_FRAME_SOULCLICK");
				float scale = grid->getMenuItemSprite()->getContentSize().width / normalSprite->getContentSize().width;
				normalSprite->setScale(scale);
				normalSprite->setContentSize(CCSizeMake(normalSprite->getContentSize().width * scale,normalSprite->getContentSize().height * scale));
				clickedSprite->setScale(scale);
				clickedSprite->setContentSize(CCSizeMake(clickedSprite->getContentSize().width * scale,clickedSprite->getContentSize().height * scale));
				grid->getMenuItemSprite()->setNormalImage(normalSprite);
				grid->getMenuItemSprite()->setSelectedImage(clickedSprite);
			}
		}
	}
}

void BackPackLayer::onChangeFairyClicked(CCObject* sender)
{
	if(m_lastClickedFairyIndex == 0)
	{
		onFetterInfoClicked(NULL);
	}
	else if (m_lastClickedFairyIndex == 5)
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/backPackLayer.lua","showUserAttributes");
		openAttributeBoard(NULL);
	}
	else if(m_lastClickedFairyIndex < 5 && m_lastClickedFairyIndex > 0)
	{
		unsigned int fairyPos = getFairyPosByListIndex(m_lastClickedFairyIndex);
		if(!isFairyPosLocked(fairyPos))
		{
			unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
			if(fairyId == 0)
			{
				LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_SelectFairyToEquip",false);
				//m_isFairySelectModle = true;
			}
			else
			{
				LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyInfoLayer",fairyPos,2);
			}
		}
	}
}

void BackPackLayer::onReceiveFairySelectEnd()
{
	if(m_isFairySelectModle)
	{
		unsigned int pos = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetSelectSingleFairyPos");
		unsigned int lastPos = getFairyPosByListIndex(m_lastClickedFairyIndex);
		if(m_lastClickedFairyIndex == 0)
		{
			lastPos = m_tipPos;
		}
		if(lastPos != 0)
		{
			OnlineNetworkManager::sShareInstance()->SendMoveFairyPosReq(pos,lastPos);
		}
		m_isFairySelectModle = false;
	}
}

void BackPackLayer::onReceiveFairyChanged(unsigned int fairyPos)
{
	int page = -1;
	if(fairyPos == FAIRY_POS_START_BTL)
	{
		setFairyIcon(4,FAIRY_POS_START_BTL);
		showFairyModle(FAIRY_POS_START_BTL,1);
		page = 1;
	}
	else if(fairyPos == FAIRY_POS_START_BTL + 1)
	{
		setFairyIcon(3,FAIRY_POS_START_BTL + 1);
		showFairyModle(FAIRY_POS_START_BTL + 1,2);
		page = 2;
	}
	else if(fairyPos == FAIRY_POS_START_AST )
	{
		setFairyIcon(2,FAIRY_POS_START_AST );
		showFairyModle(FAIRY_POS_START_AST,3);
		page = 3;
	}
	else if(fairyPos == FAIRY_POS_START_AST + 1)
	{
		setFairyIcon(1,FAIRY_POS_START_AST + 1);
		showFairyModle(FAIRY_POS_START_AST + 1,4);
		page = 4;
	}
	else if(fairyPos >= FAIRY_POS_START_PRT && fairyPos < FAIRY_POS_START_NOM)
	{
		setFriendlyFairyIcon(fairyPos - FAIRY_POS_START_PRT);
	}
	if(page == m_currentEquipPage)
	{
		//unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
		//setSelectedFairyInfo(fairyPos,fairyId);
		//onReceivedEquipPageChanged();
		setEquipPageInfo();
	}
}

void BackPackLayer::onFairyListItemClicked(CCObject* sender)
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
			UIScrollPage* modlePage = backPackLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
			if(modlePage != NULL)
			{
				modlePage->moveToPage(5 - m_lastClickedFairyIndex,false);
			}
		}
	}
}

void BackPackLayer::initEquipFairyList()
{
	UIScrollList* modlePage = backPackLayout->FindChildObjectByName<UIScrollList>("fairyList");
	if(NULL == m_fairyBtMap)
	{
		m_fairyBtMap = new std::map<unsigned int,TXGUI::UIButton*>();
		for(int i = 5; i >= 0 ; --i)
		{
			ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
			CCPoint pt;
			CCSprite* normal = pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_EQUIPMENT_FAIRY_FRAME1") ,0, pt);
			CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_EQUIPMENT_FAIRY_FRAME1_CLICKED"), 0, pt);
			UIButton* button = modlePage->AddNewItem(normal,
				clicked,
				"BackPackLayer::onFairyListItemClicked",
				"",
				ccWHITE,
				kCCScrollMenuPriority);
			char btName[50];
			sprintf(btName,"fairyList_%d",i);
			button->setName(btName);
			m_fairyBtMap->insert(make_pair(5 - i,button));
			if(i == 0)
			{
				m_lastClickedFairyIndex = 5;
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
		setFairyListIcon(5,playerPic);
		setFairyIcon(4,FAIRY_POS_START_BTL);
		setFairyIcon(3,FAIRY_POS_START_BTL + 1);
		setFairyIcon(2,FAIRY_POS_START_AST);
		setFairyIcon(1,FAIRY_POS_START_AST + 1);
		CCSprite* friendPic = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_system_icon_FRAME_ICON_FAIRY_FRIEND"), 0, pt);
		setFairyListIcon(0,friendPic);
	}
	
}

bool BackPackLayer::isFairyPosLocked(unsigned int pos)
{
	bool ret = true;
	unsigned int requiredLevel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyPosUnlockLevel",pos);
	if(UserData::GetUserLevel() >= requiredLevel)
	{
		ret = false;
	}
	return ret;
}

void BackPackLayer::setPlayerInfoVisible(bool isVisible)
{
	UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("playerLevelLabel");
	if(label)
	{
		label->setVisible(isVisible);
	}
	UILabel* m_userName = backPackLayout->FindChildObjectByName<UILabel>("playerNameLabel");
	if(m_userName)
	{
		m_userName->setVisible(isVisible);
	}
	/*UIButton* playerInfo = backPackLayout->FindChildObjectByName<UIButton>("playerAttributeBt");
	if(playerInfo)
	{
		playerInfo->setVisible(isVisible);
	}*/
	UIButton* bt = backPackLayout->FindChildObjectByName<UIButton>("allEquipBtn");
	if(bt)
	{
		bt->setVisible(isVisible);
	}
	UILabel* playerPowerLabel = backPackLayout->FindChildObjectByName<UILabel>("playerPowerLabel");
	if(playerPowerLabel)
	{
		playerPowerLabel->setVisible(isVisible);
	}
	UIPicture* playerVipPic = backPackLayout->FindChildObjectByName<UIPicture>("playerVipPic");
	if(playerVipPic)
	{
		playerVipPic->setVisible(isVisible);
	}
	UIPicture* iconTitle = backPackLayout->FindChildObjectByName<UIPicture>("iconTitle");
	if(iconTitle)
	{
		iconTitle->setVisible(isVisible);
	}
	UILabel* guildNameLabel = backPackLayout->FindChildObjectByName<UILabel>("guildNameLabel");
	if(guildNameLabel)
	{
		guildNameLabel->setVisible(isVisible);
	}
}

void BackPackLayer::setFairyInfoVisible(bool isVisible,bool isFightFairy,bool isLock,bool isEmpty)
{
	UIPicture* pic = backPackLayout->FindChildObjectByName<UIPicture>("fairyLockIcon");
	if(pic)
	{
		pic->setVisible(isVisible && isLock);
	}
	pic = backPackLayout->FindChildObjectByName<UIPicture>("fairyhelpIcon");
	if(pic)
	{
		pic->setVisible(isVisible && !isFightFairy && !isLock);
	}
	pic = backPackLayout->FindChildObjectByName<UIPicture>("fairyfightIcon");
	if(pic)
	{
		pic->setVisible(isVisible && isFightFairy && !isLock);
	}
	UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("fairyLevelLabel");
	if(label)
	{
		label->setVisible(isVisible && !isLock);
	}
	UILabel* m_userName = backPackLayout->FindChildObjectByName<UILabel>("fairyNameLabel");
	if(m_userName)
	{
		m_userName->setVisible(isVisible && !isLock);
	}
	UILabel* localLevelLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyLockLevelLabel");
	if(localLevelLabel)
	{
		localLevelLabel->setVisible(isVisible && isLock);
	}
	UIPicture* fairyPic = backPackLayout->FindChildObjectByName<UIPicture>("fairyNonePic");
	if(fairyPic)
	{
		fairyPic->setVisible(isVisible && isEmpty && !isLock);
	}
	UILabel* fairyPowerLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyPowerLabel");
	if(fairyPowerLabel)
	{
		fairyPowerLabel->setVisible(isVisible && !isEmpty && !isLock);
	}
	UILabel* fairyQualityLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyQualityLabel");
	if(fairyQualityLabel)
	{
		fairyQualityLabel->setVisible(isVisible && !isEmpty && !isLock);
	}
	UIContainer* chemicalContainer = backPackLayout->FindChildObjectByName<UIContainer>("chemicalContainer");
	if(chemicalContainer)
	{
		chemicalContainer->setVisible(isVisible && !isEmpty && !isLock);
	}
	/*UIButton* playerInfo = backPackLayout->FindChildObjectByName<UIButton>("fairyInfoBt");
	if(playerInfo)
	{
		playerInfo->setVisible(isVisible && !isLock && !isEmpty);
	}*/
	/*UIButton* fairyBt = backPackLayout->FindChildObjectByName<UIButton>("fairySetOnBt");
	if(fairyBt)
	{
		fairyBt->setVisible(isVisible && !isLock && isEmpty);
	}*/
	UIButton* bt = backPackLayout->FindChildObjectByName<UIButton>("allEquipBtn");
	if(bt)
	{
		bt->setVisible(isVisible && !isLock);
	}
}
	
void BackPackLayer::setFairyFriendInfoVisible(bool isVisible)
{
	UILabel* m_userName = backPackLayout->FindChildObjectByName<UILabel>("fairyFriendLabel");
	if(m_userName)
	{
		m_userName->setVisible(isVisible);
	}
	UIButton* bt = backPackLayout->FindChildObjectByName<UIButton>("fetterInfoBt");
	if(bt)
	{
		bt->setVisible(isVisible);
	}
	UIPicture* pic = backPackLayout->FindChildObjectByName<UIPicture>("fairyBgPic");
	if(pic)
	{
		pic->setVisible(isVisible);
	}
	UIContainer* container = backPackLayout->FindChildObjectByName<UIContainer>("alchemyControlContainer");
	if(container)
	{
		container->setVisible(!isVisible);
	}

	if(isVisible)
	{
		showPlayerAlchemy(false);
	}
}

void BackPackLayer::setFairyIcon(unsigned int index,unsigned int pos)
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

void BackPackLayer::setFairyListIcon(unsigned int index,CCSprite* icon,unsigned int lockLevel/* = 0*/)
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

void BackPackLayer::showPlayerModle(UIScrollPage* modlePage)
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

void BackPackLayer::showFairyModle(unsigned int fairyPos,unsigned int index)
{
	if(!b_isInitModle)
	{
		return;
	}
	UIScrollPage* modlePage = backPackLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
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

void BackPackLayer::showFairyFriend(UIScrollPage* modlePage,unsigned int index)
{
	if(modlePage)
	{
		CCLayer* fairyModleLayer = modlePage->getPageLayer(index);
		if (NULL == fairyModleLayer)
		{
			fairyModleLayer = CCLayer::create();
			modlePage->addPage(fairyModleLayer);
		}
	}
}

void BackPackLayer::showPlayerAlchemy(bool isAlchemyVisible)
{
	UIContainer* alchemyContainer = backPackLayout->FindChildObjectByName<UIContainer>("alchemyContainer");
	UIContainer* equipItemContainer = backPackLayout->FindChildObjectByName<UIContainer>("equipItemContainer");
	UIButton* alchemyLeftBt = backPackLayout->FindChildObjectByName<UIButton>("alchemyLeftBt");
	UIButton* alchemyRightBt = backPackLayout->FindChildObjectByName<UIButton>("alchemyRightBt");
	UILabel* alchemyLabel = backPackLayout->FindChildObjectByName<UILabel>("alchemyLabel");
	if(alchemyContainer)
	{
		alchemyContainer->setVisible(isAlchemyVisible);
	}
	if(equipItemContainer)
	{
		equipItemContainer->setVisible(!isAlchemyVisible);
	}
	if(alchemyLeftBt)
	{
		alchemyLeftBt->setEnabled(isAlchemyVisible);
	}
	if(alchemyRightBt)
	{
		alchemyRightBt->setEnabled(!isAlchemyVisible);
	}
	if(alchemyLabel)
	{
		if(isAlchemyVisible)
		{
			alchemyLabel->setString(Localizatioin::getLocalization("M_ALCHEMY_SOUL"));
		}
		else
		{
			alchemyLabel->setString(Localizatioin::getLocalization("M_FAIRY_EQUIPON"));
		}
	}
}

void BackPackLayer::checkPlayerModle()
{
	//if(NULL == m_playModleLayer && NULL == m_elfModleLayer)
	if(!b_isInitModle)
	{
		UIScrollPage* modlePage = backPackLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
		if(modlePage)
		{
			b_isInitModle = true;
			modlePage->setTouchStolenDistance(10);
			// 添加玩家人物模型
			showPlayerModle(modlePage);
			// 添加出战精灵
			showFairyModle(FAIRY_POS_START_BTL,1);
			showFairyModle(FAIRY_POS_START_BTL + 1,2);
			// 添加助战精灵
			showFairyModle(FAIRY_POS_START_AST,3);
			showFairyModle(FAIRY_POS_START_AST + 1,4);
			// 添加精灵伙伴页
			showFairyFriend(modlePage,5);
			modlePage->registerPageChangedCallBackFunc(this,callfunc_selector(BackPackLayer::onReceivedEquipPageChanged));
			modlePage->moveToPage(m_currentEquipPage,false);
		}
		initEquipFairyList();
		/*UIPicture* pic = backPackLayout->FindChildObjectByName<UIPicture>("fairyBgPic");
		if(pic)
		{
		}*/
	}
}

void BackPackLayer::showPlayerVip()
{
	UIPicture* playerVipPic = backPackLayout->FindChildObjectByName<UIPicture>("playerVipPic");
	USERINFO info = UserData::GetUserInfo();
	if(info.m_vip > 0 && playerVipPic && info.m_vip != m_showedVipLevel)
	{
		m_showedVipLevel = info.m_vip;
		char vipName[50];
		unsigned int vip = info.m_vip;
		sprintf(vipName,"map_ui_FRAME_ICON_HEADVIP%u",vip);
		CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KUI_BIN,vipName);
		playerVipPic->setSprite(sprite,false);
	}
}

void BackPackLayer::showGuildName()
{
	if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
	{
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("guildNameLabel");
		if(label)
		{
			USERINFO info = UserData::GetUserInfo();
			if(info.guildName != "")
			{
				CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("M_SOCIATY_NAMEMARK"),info.guildName.c_str());
				label->setString(str->m_sString.c_str());
			}
			else
			{
				label->setString("");
			}
		}
	}
}

void BackPackLayer::showPlayerLevel()
{
	if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
	{
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("playerLevelLabel");
		if(label)
		{
			char levelStr[10];
			sprintf(levelStr,"lv. %d", UserData::GetUserLevel());
			label->setString(levelStr);
		}
	}
}

void BackPackLayer::showPlayerEquipTitle()
{
	if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
	{
		UIPicture* iconTitle = backPackLayout->FindChildObjectByName<UIPicture>("iconTitle");
		if(iconTitle)
		{
			CCSprite* titleSprite = NULL;
			if(UserData::GetUserInfo().current_title != 0)
			{
				CCString* str = CCString::createWithFormat("map_ui_title_FRAME_%u",UserData::GetUserInfo().current_title);
				titleSprite = AspriteManager::getInstance()->getOneFrame("UI/ui_title.bin",str->m_sString.c_str());
			}
			
			iconTitle->setSprite(titleSprite,false);
			if(m_currentEquipPage != 0)
			{
				iconTitle->setVisible(false);
			}
		}
	}
}

void BackPackLayer::showPlayerEffectiveness()
{
	if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
	{
		UILabel* playerPowerLabel = backPackLayout->FindChildObjectByName<UILabel>("playerPowerLabel");
		if(playerPowerLabel)
		{
			USERINFO info = UserData::GetUserInfo();
			char str[100];
			unsigned int effectiveness = info.effectiveness;
			sprintf(str ,"%s%u",Localizatioin::getLocalization("M_BAG_POWERINALL"),effectiveness);
			playerPowerLabel->setString(str);
		}
	}
}

void BackPackLayer::onReceiveFairyFetter()
{
	if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
	{
		if(m_lastClickedFairyIndex < 5 && m_lastClickedFairyIndex > 0)
		{
			unsigned int fairyPos = getFairyPosByListIndex(m_lastClickedFairyIndex);
			unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
			if(fairyId != 0)
			{
				showCurrentFairyFetter(fairyPos,fairyId);
			}
		}
	}
}

void BackPackLayer::onRecieveFairyEffectiveness()
{
	if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
	{
		if(m_lastClickedFairyIndex < 5 && m_lastClickedFairyIndex > 0)
		{
			unsigned int fairyPos = getFairyPosByListIndex(m_lastClickedFairyIndex);
			unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
			if(fairyId != 0)
			{
				UILabel* fairyPowerLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyPowerLabel");
				if(fairyPowerLabel)
				{
					if(fairyId != 0)
					{
						unsigned int effectiveness = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyEffectivenessByPos",fairyPos);
						char str[100];
						sprintf(str ,"%s%u",Localizatioin::getLocalization("M_BAG_POWERSUPPLY"),effectiveness);
						fairyPowerLabel->setString(str);
					}
					else
					{
						fairyPowerLabel->setString("");
					}
				}
			}
		}
	}
}

//void BackPackLayer::showPlayerExp()
//{
//	USERINFO userInfo = UserData::GetUserInfo();
//	float percent = 1.0f;
//	if(userInfo.nextExp != 0)
//	{
//		percent = (1.0f * userInfo.exp) / userInfo.nextExp;
//	}
//	percent = percent > 1 ? 1 : percent;
//	if(m_playerExpLable)
//	{
//		float scale = UIManager::sharedManager()->getScaleFactor();
//		m_playerExpLable->setScaleX(percent * scale);
//		m_playerExpLable->setScaleY(scale);
//	}
//	if(backPackLayout)
//	{
//		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("equipExpLabel");
//		if(label)
//		{
//			char exp[20];
//			//sprintf(exp,"exp %.2f%%",percent*100);
//			sprintf(exp,"exp %u/%u",userInfo.exp,userInfo.nextExp);
//			label->setString(exp);
//		}
//	}
//}

void BackPackLayer::showEquipFragCanComposeIcon()
{
	if(backPackLayout)
	{
		UIButtonToggle* bt = backPackLayout->FindChildObjectByName<UIButtonToggle>("fragButtonToggle");
		if(bt)
		{
			USERINFO info = UserData::GetUserInfo();
			if(info.b_hasEnoughEquipFrag)
			{
				bt->showTipIcon();
			}
			else
			{
				bt->hideTipIcon();
			}
		}
	}
}

void BackPackLayer::showBagNewEquipIcon()
{
	if(backPackLayout)
	{
		UIButtonToggle* bt = backPackLayout->FindChildObjectByName<UIButtonToggle>("bagButtonToggle");
		if(bt)
		{
			bool hasNew = ItemManager::Get()->checkNewEquipInfo();
			if(hasNew)
			{
				bt->showTipIcon();
			}
			else
			{
				bt->hideTipIcon();
			}
		}
	}
}

void BackPackLayer::showCoinLabel()
{
	if(backPackLayout)
	{
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("bagGoldLable");
		if(label)
		{
			unsigned int gold = UserData::GetGold();
			const char* str = MainMenuLayer::GetNumByFormat(gold);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}

void BackPackLayer::showFragStoneLabel()
{
	if(backPackLayout)
	{
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("bagFragLable");
		if(label)
		{
			USERINFO info = UserData::GetUserInfo();
			const char* str = MainMenuLayer::GetNumByFormat(info.m_fragStone);
			if(str)
			{
				label->setString(str);
			}
		}
	}
}

void BackPackLayer::showPlayerAttri()
{
	USERINFO info = UserData::GetUserInfo();
	if(backPackLayout)
	{
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("LifeNumberLable");
		if(label)
		{
			char text[20];
			sprintf(text,"%d",(int)(info.total_hp));
			label->setString(text);
		}

		label = backPackLayout->FindChildObjectByName<UILabel>("PowerNumberLable");
		if(label)
		{
			char text[20];
			sprintf(text,"%d",(int)(info.courage));
			label->setString(text);
		}

		label = backPackLayout->FindChildObjectByName<UILabel>("SkillNumberLable");
		if(label)
		{
			char text[20];
			sprintf(text,"%d",(int)(info.trick));
			label->setString(text);
		}

		label = backPackLayout->FindChildObjectByName<UILabel>("IntellectNumberLable");
		if(label)
		{
			char text[20];
			sprintf(text,"%d",(int)(info.charm));
			label->setString(text);
		}
	}
}

void BackPackLayer::openExchangeShop()
{
    unsigned int unLockLevel = 8;
	if (UserData::Get()->GetUserLevel() < unLockLevel)
	{
		std::string flag = "M_LEVEL_NOT_ENOUGH";
		std::string content = Localizatioin::getLocalization(flag.c_str());
		ShowMessage(content.c_str());
		return ;
	}
    
    if(!isVisible())
	{
		setVisible(true);
	}
    
    if(!b_isBackPackOpened)
	{
		if(m_backPackContainer && m_backPackContainer->getCurrentNode())
		{
			GameAudioManager::sharedManager()->playEffect(350001,false);
			m_backPackContainer->getCurrentNode()->setVisible(true);
			b_isBackPackOpened = true;
			unsigned int maxPos = ItemManager::Get()->getBackPackMaxPos();
			for(unsigned int i=1;i<= maxPos;i++)
			{
				BackPackItem* item = ItemManager::Get()->findItemByPos(i);
				if(item)
				{
					setOneBackPackIcon(item,i);
				}
			}
		}
	}
    
    if(b_isStorageOpened)
	{
		if(m_storageContainer && m_storageContainer->getCurrentNode())
		{
			m_storageContainer->getCurrentNode()->setVisible(false);
		}
		b_isStorageOpened = false;
	}
    
	if(b_isAttributeOpened)
	{
		UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
		if(attributeContainer && attributeContainer->getCurrentNode())
		{
			attributeContainer->getCurrentNode()->setVisible(false);
		}
		b_isAttributeOpened = false;
	}
    
    openCoreExchangeShop();
	MainMenuLayer::PushOneLayer(this);
}

void BackPackLayer::setDefaultSelect()
{
	if(m_lastClickedFairyIndex == -1)
	{
		for(unsigned int i=1;i<= 6;i++)
		{
			if(m_currentEquipPage != 5)
			{
				unsigned int beginPos = (m_currentEquipPage == 0) ? kItemHeroEquip : (kItemFairyEquip + (m_currentEquipPage - 1) * 100);
				BackPackItem* item = ItemManager::Get()->findItemByPos(beginPos + i);
				if(item)
				{
					setEquipmentIcon(item,i);
				}
			}
			else
			{
				setFriendlyFairyIcon(i-1);
			}
		}
		for(unsigned int i=1;i<= 8;i++)
		{
			if(m_currentEquipPage != 5)
			{
				unsigned int beginPos = (m_currentEquipPage == 0) ? kAlchemyHeroEquip : (kAlchemyFairyEquip + (m_currentEquipPage - 1) * 100);
				BackPackItem* item = ItemManager::Get()->findItemByPos(beginPos + i);
				if(item)
				{
					setAlchemyIcon(item,i);
				}
			}
		}
		setFairyFriendInfoVisible(false);
		setFairyInfoVisible(false,false,false,false);
		setPlayerInfoVisible(true);
	}
}

void BackPackLayer::openBackPack(bool withEquipment)
{
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
    if(!bMainLandLevel)
        return;

	if(!isVisible())
	{
		setVisible(true);
	}
	showBackPackContainer();
    if(!b_isEquipmentOpened && withEquipment)
    {
		if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
        {
            m_equipmentContainer->getCurrentNode()->setVisible(true);
        }
		b_isEquipmentOpened = true;
		setDefaultSelect();
		checkPlayerModle();
		showPlayerLevel();
		showPlayerVip();
		showGuildName();
		showPlayerEquipTitle();
		showPlayerEffectiveness();
		//showPlayerExp();
		showPlayerAttri();
        
		GameAudioManager::sharedManager()->playEffect(350001,false);
    }

	if(b_isStorageOpened)
	{
		if(m_storageContainer && m_storageContainer->getCurrentNode())
		{
			m_storageContainer->getCurrentNode()->setVisible(false);
		}
		b_isStorageOpened = false;
	}
	if(b_isAttributeOpened)
	{
		UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
		if(attributeContainer && attributeContainer->getCurrentNode())
		{
			attributeContainer->getCurrentNode()->setVisible(false);
		}
		b_isAttributeOpened = false;
	}
	if(m_pShopUILayer)
	{
		m_pShopUILayer->closeShop();
		m_pShopUILayer->setVisible(false);
	}

	if (!withEquipment)
	{
		if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
        {
            m_equipmentContainer->getCurrentNode()->setVisible(false);
        }
		b_isEquipmentOpened = false;
	}
	SpriteElfConfigFromLuaManager::getInstance()->CloseElfPotientionPanel();
	MainMenuLayer::PushOneLayer(this);
}

void BackPackLayer::openAttributeBoard(CCObject* sender)
{
	if(b_isBackPackOpened)
	{
		b_isBackPackOpened = false;
		if(m_backPackContainer && m_backPackContainer->getCurrentNode())
		{
			m_backPackContainer->getCurrentNode()->setVisible(false);
		}
	}
	UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
	b_isAttributeOpened = true;
	GameAudioManager::sharedManager()->playEffect(350001,false);
	if(attributeContainer && attributeContainer->getCurrentNode())
	{
		attributeContainer->getCurrentNode()->setVisible(true);
	}
	SpriteElfConfigFromLuaManager::getInstance()->CloseElfPotientionPanel();
}

void BackPackLayer::closeAttributeBoard(CCObject* sender)
{
	hideAttributeBoard();
	showBackPackContainer(false);
}

void BackPackLayer::hideAttributeBoard()
{
	UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
	b_isAttributeOpened = false;
	if(attributeContainer && attributeContainer->getCurrentNode())
	{
		attributeContainer->getCurrentNode()->setVisible(false);
	}
}

void BackPackLayer::resetBackPackItemIcon(unsigned int index)
{
	ItemManager* manager = ItemManager::Get();
	BackPackItem* item = manager->findItemByPos(index);
	if(item)
	{
		setOneBackPackIcon(item,index);
	}
}

void BackPackLayer::resetStorageItemIcon(unsigned int index)
{
	ItemManager* manager = ItemManager::Get();
	BackPackItem* item = manager->findItemByPos(index+kItemStorage);
	if(item)
	{
		setOneStorageIconn(item,index);
	}
}

void BackPackLayer::setVisible(bool visible)
{
	CCLayer::setVisible(visible);
	if(!visible)
	{
		b_isBackPackOpened = false;
		b_isStorageOpened = false;
        b_isEquipmentOpened = false;
		b_isAttributeOpened = false;
		if(m_backPackContainer && m_backPackContainer->getCurrentNode())
		{
			m_backPackContainer->getCurrentNode()->setVisible(false);
		}
		if(m_storageContainer && m_storageContainer->getCurrentNode())
		{
			m_storageContainer->getCurrentNode()->setVisible(false);
		}
        if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
        {
            m_equipmentContainer->getCurrentNode()->setVisible(false);
        }
		UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
		if(attributeContainer && attributeContainer->getCurrentNode())
		{
			attributeContainer->getCurrentNode()->setVisible(false);
		}
		UIScrollPage* modlePage = backPackLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
		if(modlePage)
		{
			modlePage->removeAllPages();
			b_isInitModle = false;
		}
	}
	else
	{
		openBackPack(true);
		ShowChatBar(true);
	}
}

void BackPackLayer::resetItemIcon(unsigned int index)
{
}

void BackPackLayer::setFriendlyFairyIcon(unsigned int index)
{
	unsigned int fairyPos = index + FAIRY_POS_START_PRT;
	UIButton* button = getEquipmentButton(index+1);
	if(isFairyPosLocked(fairyPos))
	{
		if(button)
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
			int idx = getResourceIDByName(m_lockPicName.c_str());
			CCPoint pt;
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			button->setIconSprite(lockPic);
			button->setFrameSprite(NULL);
			unsigned int requiredLevel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyPosUnlockLevel",fairyPos);
			char levelStr[30];
			sprintf(levelStr,"Lv.%u",requiredLevel);
			CCSize size = button->getMenuItemSprite()->getContentSize();
			button->setTextLabel(levelStr,18,ccRED,KJLinXin, ccp(size.width * (0.4f),size.height * (-0.4f)),ccp(1,0));
			button->setDragMove(false);
		}
	}
	else
	{
		if(button)
		{
			button->setFrameSprite(NULL);
			button->setTextLabel("",18,ccORANGE,KJLinXin, CCPointZero);
			unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
			if(fairyId != 0)
			{
				const char* fairyIcon = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID",fairyId);
				CCSprite* sprite = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin",fairyIcon);
				button->setIconSprite(sprite);
				button->setDragMove(false);
			}
			else
			{
				CCSprite* icon = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_FAIRY_LOCK");
				button->setIconSprite(icon);
			}
			button->setDragMove(false);
		}
	}
	

}

CCSprite* BackPackLayer::getEquipEmptyIcon(unsigned int index)
{
	CCSprite* icon = NULL;
	string iconName = "";
	switch(index)
	{
	case 1:
		iconName = "map_ui_FRAME_EQUIPMENT_ICON_HELMET";
		break;
	case 2:
		iconName = "map_ui_FRAME_EQUIPMENT_ICON_NECKLACE";
		break;
	case 3:
		iconName = "map_ui_FRAME_EQUIPMENT_ICON_WEAPON";
		break;
	case 4:
		iconName = "map_ui_FRAME_EQUIPMENT_ICON_ARMOR";
		break;
	case 5:
		iconName = "map_ui_FRAME_EQUIPMENT_ICON_RING";
		break;
	case 6:
		iconName = "map_ui_FRAME_EQUIPMENT_ICON_SHOE";
		break;
	}
	if(iconName != "")
	{
		icon = AspriteManager::getInstance()->getOneFrame("UI/ui.bin",iconName);
	}
	return icon;
}

void BackPackLayer::setAlchemyIcon(BackPackItem* item,unsigned int index)
{
	if(backPackLayout)
	{
		UIButton* button = backPackLayout->FindChildObjectByName<UIButton>(CCString::createWithFormat("alchemyGrid%d",index)->m_sString.c_str());
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>(CCString::createWithFormat("alchemyLockLevel%d",index)->m_sString.c_str());
		if(label == NULL || button == NULL)
		{
			return;
		}
		
		if(item->isLocked)
		{
			label->setVisible(true);
			ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
			CCPoint pt;
			int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_STORAGE_GRID1_LOCKED");
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( iFrameId, 0, pt);
			float scale = button->getMenuItemSprite()->getContentSize().width / lockPic->getContentSize().width;
			lockPic->setScale(scale);
			lockPic->setContentSize(button->getMenuItemSprite()->getContentSize());
			if(lockPic && button)
			{
				button->setIconSprite(lockPic);
			}
		}
		else 
		{
			label->setVisible(false);
			if(item->isEmpty)
			{
				if(button)
				{
					button->setIconSprite(NULL);
					//button->setDragMove(false);
				}
			}
			else
			{
				CCSprite* pic = getIconSpriteById(item->getItemId());
				if(pic)
				{
					if(button)
					{
						button->setIconSprite(pic);
						ItemManager::Get()->showSpriteParticleEffect(pic,item->getItemId(),ccp(pic->getContentSize().width * 0.5,pic->getContentSize().height * 0.5));
					}
				}
			}
		}
	}
}

void BackPackLayer::setEquipmentIcon(BackPackItem* item,unsigned int index)
{
	if(b_isEquipmentOpened)
	{
		if(item->isEmpty)
		{
			UIButton* button = getEquipmentButton(index);
			if(button)
			{
				button->setIconSprite(getEquipEmptyIcon(index));
				//button->setFrameSprite(NULL);
				button->setTextLabel("",18,ccORANGE,KJLinXin, CCPointZero);
				button->setDragMove(false);
			}
		}
		else
		{
			CCSprite* pic = getIconSpriteById(item->getItemId());
			if(pic)
			{
				UIButton* button = getEquipmentButton(index);
				if(button)
				{
					button->setIconSprite(pic);
					ItemBase* itemInfo = item->getItemInfo();
					if(itemInfo && itemInfo > 0)
					{
						/*CCNode* node = CCNode::create();
						CCSprite* framePic = getIconFrame(itemInfo->m_quality);
						if(framePic)
						{
							node->addChild(framePic);
						}
						button->setFrameSprite(node);*/
						unsigned int itemLevel = item->itemLevel;
						unsigned int itemType = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Equipment/EquipConfigTable.lua",
							"getEquipTypeById",item->getItemId());
						char str[50];
						if(itemType == 2)
						{
							sprintf(str,"+%u",itemLevel);
						}
						else
						{
							sprintf(str,"%u",itemLevel);
						}
						CCSize size = button->getMenuItemSprite()->getContentSize();
						button->setTextLabel(str,18,ccORANGE,KJLinXin, ccp(size.width * (-0.4f),size.height * (0.4f)),ccp(0,1));
					}
					button->setDragMove(true);
				}
			}
		}
	}
}

CCSprite* BackPackLayer::getEquipmentUpgradeIcon(unsigned int itemId)
{
	ExchangeItem* exchangeItem = ItemManager::Get()->getEquipDrawingById(itemId);
	if(exchangeItem && exchangeItem->m_fromItems)
	{
		bool isDrawingExist = false;
		for(std::list<pair<unsigned int ,unsigned int> >::iterator it = exchangeItem->m_fromItems->begin();
			it != exchangeItem->m_fromItems->end(); ++it)
		{
			unsigned int equip = (*it).first;
			ItemBase* info = ItemManager::Get()->getItemInfoById(equip);
			if(info)
			{
				if(info->m_type == 2)
				{
					unsigned int drawingNum = ItemManager::Get()->getItemNumberById(equip);
					if(drawingNum > 0)
					{
						isDrawingExist = true;
					}
					else
					{
						isDrawingExist = false;
					}
					break;
				}
			}
		
		}
		
		if(isDrawingExist)
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
			int idx = TXGUI::getResourceIDByName("map_ui_system_icon_FRAME_ICON_UPGRADE");
			CCPoint pt;
			CCSprite * pic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			return pic;
		}
	}
	return NULL;
}

void BackPackLayer::setOneStorageIconn(BackPackItem* item,unsigned int index)
{
	CCAssert(index != 0, "Storage Pos Error");
	if(b_isStorageOpened)
	{
		unsigned int page = (index-1) / EACH_PAGE_NUMBER;
		unsigned int row = ((index-1) % EACH_PAGE_NUMBER) / EACH_ROW_NUMBER;
		unsigned int colmun = ((index-1) % EACH_PAGE_NUMBER) % EACH_ROW_NUMBER;
		if(item->isLocked)
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
			int idx = getResourceIDByName(m_lockPicName.c_str());
			CCPoint pt;
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(m_storageScrollPage)
			{
				m_storageScrollPage->SetPageItemIconWithSprite(page,colmun,row,lockPic);
			}
		}
		else if(item->isEmpty)
		{
			if(m_storageScrollPage)
			{
				m_storageScrollPage->SetPageItemIconWithSprite(page,colmun,row,NULL);
				IconButton* button = m_storageScrollPage->GetIconButton(page,colmun,row);
				if(button)
				{
					button->removeItemLabel();
                    //button->setFrameSprite(NULL);
				}
			}
		}
		else
		{
			CCSprite* pic = getIconSpriteById(item->getItemId());
			if(pic)
			{
				if(m_storageScrollPage)
				{
					m_storageScrollPage->SetPageItemIconWithSprite(page,colmun,row,pic);
					IconButton* button = m_storageScrollPage->GetIconButton(page,colmun,row);
					if(button)
					{
						if(item->amount > 1)
						{

							char amountString[10];
							sprintf(amountString,"%d",item->amount);
							float labelFontSize = GameFontManager::smallFontSize();  
							button->setItemLabel(amountString,labelFontSize,KJLinXin);
						}
						else
						{
							button->removeItemLabel();
						}
						button->creatDragReceiver(EStorageItem);
						/*ItemBase* itemInfo = item->getItemInfo();
						if(itemInfo && itemInfo > 0)
						{
							CCSprite* framePic = getIconFrame(itemInfo->m_quality);
							if(framePic)
							{
								button->setFrameSprite(framePic);
							}
							else
							{
								button->setFrameSprite(NULL);
							}
						}*/
						button->setDragMove(true);
					}
				}
			}
		}
	}

}

void BackPackLayer::setOneFragmentIcon(BackPackItem* item,unsigned int index)
{
	CCAssert(index != 0, "Storage Pos Error");
	if(b_isBackPackOpened)
	{
		unsigned int page = (index-1) / EACH_PAGE_NUMBER;
		unsigned int row = ((index-1) % EACH_PAGE_NUMBER) / EACH_ROW_NUMBER;
		unsigned int colmun = ((index-1) % EACH_PAGE_NUMBER) % EACH_ROW_NUMBER;
		if(item->isLocked)
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
			int idx = getResourceIDByName(m_lockPicName.c_str());
			CCPoint pt;
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(m_fragScrollPage)
			{
				m_fragScrollPage->SetPageItemIconWithSprite(page,colmun,row,lockPic);
			}
		}
		else if(item->isEmpty)
		{
			if(m_fragScrollPage)
			{
				m_fragScrollPage->SetPageItemIconWithSprite(page,colmun,row,NULL);
				IconButton* button = m_fragScrollPage->GetIconButton(page,colmun,row);
				if(button)
				{
					button->removeItemLabel();
					//button->setFrameSprite(NULL);
					button->hideTipIcon();
				}
			}
		}
		else
		{
			CCSprite* pic = getIconSpriteById(item->getItemId());
			if(pic)
			{
				if(m_fragScrollPage)
				{
					m_fragScrollPage->SetPageItemIconWithSprite(page,colmun,row,pic);
					IconButton* button = m_fragScrollPage->GetIconButton(page,colmun,row);
					if(item->amount > 1)
					{
						char amountString[10];
						sprintf(amountString,"%d",item->amount);
						float labelFontSize = GameFontManager::smallFontSize();  
						button->setItemLabel(amountString,labelFontSize,KJLinXin);
					}
					else
					{
						button->removeItemLabel();
					}
					/*ItemBase* itemInfo = item->getItemInfo();
					if(itemInfo && itemInfo > 0)
					{
						CCSprite* framePic = getIconFrame(itemInfo->m_quality);
						if(framePic)
						{
							button->setFrameSprite(framePic);
						}
						else
						{
							button->setFrameSprite(NULL);
						}
					}*/
					button->creatDragReceiver(EBackPackItem);
					button->setDragMove(true);

					bool ret = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/EquipConfigTable.lua","checkSingleFragEnough",
							item->getItemId(),item->amount);
					if(ret)
					{
						button->showTipIcon();
					}
					else
					{
						button->hideTipIcon();
					}
				}
			}
		}
	}
}

void BackPackLayer::setOneBackPackIcon(BackPackItem* item,unsigned int index)
{
	CCAssert(index != 0, "Storage Pos Error");
	if(b_isBackPackOpened)
	{
		unsigned int page = (index-1) / EACH_PAGE_NUMBER;
		unsigned int row = ((index-1) % EACH_PAGE_NUMBER) / EACH_ROW_NUMBER;
		unsigned int colmun = ((index-1) % EACH_PAGE_NUMBER) % EACH_ROW_NUMBER;
		if(item->isLocked)
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
			int idx = getResourceIDByName(m_lockPicName.c_str());
			CCPoint pt;
			CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
			if(m_backPackScrollPage)
			{
				m_backPackScrollPage->SetPageItemIconWithSprite(page,colmun,row,lockPic);
			}
		}
		else if(item->isEmpty)
		{
			if(m_backPackScrollPage)
			{
				m_backPackScrollPage->SetPageItemIconWithSprite(page,colmun,row,NULL);
				IconButton* button = m_backPackScrollPage->GetIconButton(page,colmun,row);
				if(button)
				{
					button->removeItemLabel();
					//button->setFrameSprite(NULL);
					button->hideTipIcon();
				}
			}
		}
		else
		{
			CCSprite* pic = getIconSpriteById(item->getItemId());
			if(pic)
			{
				if(m_backPackScrollPage)
				{
					m_backPackScrollPage->SetPageItemIconWithSprite(page,colmun,row,pic);
					IconButton* button = m_backPackScrollPage->GetIconButton(page,colmun,row);
					button->hideTipIcon();
					ItemBase* itemInfo = item->getItemInfo();
					EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(itemInfo);
					if(equipInfo)
					{ // equip show level
						if(item->itemLevel >= 1)
						{
							unsigned int itemType = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Equipment/EquipConfigTable.lua",
							"getEquipTypeById",item->getItemId());
							char str[50];
							if(itemType == 2)
							{
								sprintf(str,"+%u",item->itemLevel);
							}
							else
							{
								sprintf(str,"%u",item->itemLevel);
							}
							float labelFontSize = GameFontManager::smallFontSize();  
							button->setItemLabel(str,labelFontSize,KJLinXin);
							UILabelTTF* label = button->getItemLabel();
							if(label)
							{
								label->setAnchorPoint(ccp(0,1));
								float x = button->getContentSize().width * 0.4f;
								float y = button->getContentSize().height * 0.4f;
								label->setPosition(ccp(-x,y));
								label->setColor(ccORANGE);
							}
							if(item->b_isNew)
							{
								button->showTipIcon();
							}
						}
						else
						{
							button->removeItemLabel();
						}
					}
					else
					{// item show amount
						if(item->amount > 1)
						{
							char amountString[10];
							sprintf(amountString,"%d",item->amount);
							float labelFontSize = GameFontManager::smallFontSize();  
							button->setItemLabel(amountString,labelFontSize,KJLinXin);
						}
						else
						{
							button->removeItemLabel();
						}
					}
					/*if(itemInfo && itemInfo > 0)
					{
						CCSprite* framePic = getIconFrame(itemInfo->m_quality);
						if(framePic)
						{
							button->setFrameSprite(framePic);
						}
						else
						{
							button->setFrameSprite(NULL);
						}
					}*/
					button->creatDragReceiver(EBackPackItem);
					button->setDragMove(true);
					
				}
			}
		}
	}
}

void BackPackLayer::setOneItemIcon(BackPackItem* item,unsigned int index)
{
	/*if(index == 0)
	{
		CCAssert(index,"error item index");
		return;
	}*/
    
	unsigned int pos = index;
	if(pos >= kEquipFragmet && pos < kItemRecycle)
	{
		pos = pos - kEquipFragmet;
		setOneFragmentIcon(item,pos);
	}
	else if(pos >= kAlchemyHeroEquip && pos < kFairyStartId)
	{
		pos = pos - (kAlchemyHeroEquip + m_currentEquipPage * 100);
		setAlchemyIcon(item,pos);
	}
	else if(pos >= kItemFairyEquip && pos < kAlchemyBag)
	{
		// fairy equip show
		if(m_currentEquipPage != 0)
		{
			pos = pos - (kItemFairyEquip + (m_currentEquipPage - 1) * 100);
			setEquipmentIcon(item,pos);
		}
	}
	else if(pos >= kItemHeroEquip)
	{
		pos = pos - kItemHeroEquip;
		if(m_currentEquipPage == 0)
		{
			setEquipmentIcon(item,pos);
		}
	}
	else if(pos >= kItemStorage)
	{
		pos = pos - kItemStorage;
		setOneStorageIconn(item,pos);
	}
	else if(pos < kItemStorage)
	{
		setOneBackPackIcon(item,pos);
	}
}

CCSprite* BackPackLayer::getIconSpriteById(unsigned int ItemId)
{
	return ItemManager::Get()->getIconSpriteById(ItemId);
}

CCSprite* BackPackLayer::getIconFrame(unsigned int level)
{
	return ItemManager::Get()->getIconFrame(level);
}

void BackPackLayer::closeBackPack(CCObject* sender)
{
	if(b_isBackPackOpened)
	{
		if(m_backPackContainer && m_backPackContainer->getCurrentNode())
		{
			m_backPackContainer->getCurrentNode()->setVisible(false);
		}
		//UIManager::sharedManager()->SetOtherUILayoutTouchEnableExcept(true,"BackPack");
		b_isBackPackOpened = false;		
	}
	closeEquipment(NULL);
	if(m_pShopUILayer)
	{
		m_pShopUILayer->OnClose(NULL);
	}
	checkLayerHide();

}

void BackPackLayer::closeEquipment(cocos2d::CCObject *sender)
{
    if(b_isEquipmentOpened)
	{
		if(m_equipmentContainer && m_equipmentContainer->getCurrentNode())
		{
			m_equipmentContainer->getCurrentNode()->setVisible(false);
		}
		//UIManager::sharedManager()->SetOtherUILayoutTouchEnableExcept(true,"BackPack");
		b_isEquipmentOpened = false;

		UIScrollPage* modlePage = backPackLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
		if(modlePage)
		{
			modlePage->removeAllPages();
			b_isInitModle = false;
		}
		hideAttributeBoard();
	}
	checkLayerHide();
}

void BackPackLayer::openBackIndex(CCObject* sender)
{
	if(m_fragIndexToggle)
	{
		m_fragIndexToggle->unselected();
	}
	if(m_bagIndexToggle)
	{
		m_bagIndexToggle->selected();
	}
	setBagIndex(true);
}

void BackPackLayer::openEquipFragIndex(CCObject* sender)
{
	if(m_fragIndexToggle)
	{
		m_fragIndexToggle->selected();
	}
	if(m_bagIndexToggle)
	{
		m_bagIndexToggle->unselected();
	}
	setBagIndex(false);
}

void BackPackLayer::openShop(CCObject* sender)
{
	setBagIndex(true);
	//return;
	//unsigned int unLockLevel = 18;
	//if (UserData::Get()->GetUserLevel() < unLockLevel)
	//{
	//	std::string flag = "M_LEVEL_NOT_ENOUGH";
	//	std::string content = Localizatioin::getLocalization(flag.c_str());
	//	ShowMessage(content.c_str());
	//	return ;
	//}
    
    
    closeEquipment(NULL);
    closeStorage(NULL);

	if(m_pShopUILayer == NULL)
	{
		createShopLayer();
	}
	if(m_pShopUILayer != NULL)
	{
		m_pShopUILayer->setVisible(true);
		m_pShopUILayer->sendShopList();
		m_pShopUILayer->openShop();
	}
    
    ShowChatBar(false);
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "ClosePointShop");
}

void BackPackLayer::openCoreExchangeShop()
{
    closeEquipment(NULL);
    closeStorage(NULL);
	if(m_pShopUILayer == NULL)
	{
		createShopLayer();
	}
	if(m_pShopUILayer != NULL)
	{
		 m_pShopUILayer->setVisible(true);
		 m_pShopUILayer->showExchangeShopList();
	}
    ShowChatBar(false);
}

//void BackPackLayer::openStoreHouse(CCObject* sender)
//{
//	//
//	setBagIndex(true);
//	return;
//	if(!isVisible())
//	{
//		setVisible(true);
//	}
//	if(!b_isStorageOpened)
//	{
//        closeEquipment(NULL);
//		if(m_storageContainer && m_storageContainer->getCurrentNode())
//		{
//			m_storageContainer->getCurrentNode()->setVisible(true);
//		}
//		GameAudioManager::sharedManager()->playEffect(350001,false);
//		b_isStorageOpened = true;
//		unsigned int maxPos = ItemManager::Get()->getStorageMaxPos();
//		for(unsigned int i=1;i<=maxPos;i++)
//		{
//			BackPackItem* item = ItemManager::Get()->findItemByPos(i+kItemStorage);
//			if(item)
//			{
//				setOneStorageIconn(item,i);
//			}
//		}
//	}
//    
//    if(m_pShopUILayer)
//	{
//        m_pShopUILayer->setVisible(false);
//	}
//    
//    ShowChatBar(true, -1);
//	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "ClosePointShop");
//}

void BackPackLayer::closeStorage(CCObject* sender)
{
	if(b_isStorageOpened)
	{
		if(m_storageContainer && m_storageContainer->getCurrentNode())
		{
			m_storageContainer->getCurrentNode()->setVisible(false);
		}
		b_isStorageOpened = false;
	}
	checkLayerHide();
}

void BackPackLayer::createShopLayer()
{
	m_pShopUILayer = ShopUILayer::create();
	if(m_pShopUILayer != NULL)
	{
		addChild(m_pShopUILayer,10);
		m_pShopUILayer->setVisible(false);
	}
}

void BackPackLayer::onReceivedConfirmBagItemUnlock(CCObject* sender)
{
	unselectAllBagItem(b_isUnlockBagFlag);
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","CheckPayHint");
	OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(m_unLockExchangeId,NULL,0);
	m_unLockExchangeId = 0;
}

void BackPackLayer::onReceivedCancellBagItemUnlock(CCObject* sender)
{
	unselectAllBagItem(b_isUnlockBagFlag);
	m_unLockExchangeId = 0;
}

void BackPackLayer::setEquipPageInfo()
{
	if(m_currentEquipPage == 0)
	{
		setFairyInfoVisible(false,false,false,false);
		setPlayerInfoVisible(true);
		setFairyFriendInfoVisible(false);
	}
	else if(m_currentEquipPage >= 1 && m_currentEquipPage <= 4)
	{
		unsigned int pos = 0;
		if(m_currentEquipPage == 4 || m_currentEquipPage == 3)
		{
			pos = FAIRY_POS_START_AST + (m_currentEquipPage - 3);
		}
		else if(m_currentEquipPage == 2 || m_currentEquipPage == 1)
		{
			pos = FAIRY_POS_START_BTL + (m_currentEquipPage - 1);
		}
		unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",pos);
		setSelectedFairyInfo(pos,fairyId);
		setPlayerInfoVisible(false);
		setFairyFriendInfoVisible(false);
		setFairyInfoVisible(true,m_currentEquipPage <= 2,isFairyPosLocked(pos),fairyId == 0);
	}
	else
	{
		setPlayerInfoVisible(false);
		setFairyFriendInfoVisible(true);
		setFairyInfoVisible(false,false,false,false);
	}
	if(m_currentEquipPage != 5)
	{
		for(unsigned int i=1;i<= 6;i++)
		{
			unsigned int beginPos = (m_currentEquipPage == 0) ? kItemHeroEquip : (kItemFairyEquip + (m_currentEquipPage - 1) * 100);
			BackPackItem* item = ItemManager::Get()->findItemByPos(beginPos + i);
			if(item)
			{
				setEquipmentIcon(item,i);
			}
		}
		for(unsigned int i=1;i<= 8;i++)
		{
			unsigned int beginPos = (m_currentEquipPage == 0) ? kAlchemyHeroEquip : (kAlchemyFairyEquip + (m_currentEquipPage - 1) * 100);
			BackPackItem* item = ItemManager::Get()->findItemByPos(beginPos + i);
			if(item)
			{
				setAlchemyIcon(item,i);
			}
		}
	}
	else
	{
		for(unsigned int i=0;i<= 5;i++)
		{
			setFriendlyFairyIcon(i);
		}
	}
	resetFairyListPostion(m_currentEquipPage);
}

void BackPackLayer::onReceivedEquipPageChanged()
{
	UIScrollPage* modlePage = backPackLayout->FindChildObjectByName<UIScrollPage>("modelScrollPage");
	if(modlePage)
	{
		m_currentEquipPage = modlePage->getCurPage();
		setEquipPageInfo();
	}
}

void BackPackLayer::setSelectedFairyInfo(int pos,int fairyId)
{
	if(!isFairyPosLocked(pos))
	{
		UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("fairyLevelLabel");
		if(label)
		{
			if(fairyId != 0)
			{
				unsigned int fairyLevel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyLevelByPos",pos);
				char levelStr[20];
				sprintf(levelStr ,"Lv.%u",fairyLevel);
				label->setString(levelStr);
			}
			else
			{
				label->setString("");
			}
		}
		UILabel* m_userName = backPackLayout->FindChildObjectByName<UILabel>("fairyNameLabel");
		if(m_userName)
		{
			if(fairyId != 0)
			{
				const char* fairyName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyNameByPos",pos);
				unsigned int fairyQuality = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyQualityByPos",pos);
				ccColor3B color = ItemManager::Get()->getFairyLabelColorByQuality(fairyQuality);
				m_userName->setStringWithColor(fairyName,color);
			}
			else
			{
				m_userName->setString("");
			}
		}
		UILabel* fairyPowerLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyPowerLabel");
		if(fairyPowerLabel)
		{
			if(fairyId != 0)
			{
				unsigned int effectiveness = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyEffectivenessByPos",pos);
				char str[100];
				sprintf(str ,"%s%u",Localizatioin::getLocalization("M_BAG_POWERSUPPLY"),effectiveness);
				fairyPowerLabel->setString(str);
			}
			else
			{
				fairyPowerLabel->setString("");
			}
		}
		UILabel* fairyQualityLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyQualityLabel");
		if(fairyQualityLabel)
		{
			if(fairyId != 0)
			{
				unsigned int quality = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetPlayerFairyQulity",pos);
				char str[100];
				sprintf(str ,"%s%u",Localizatioin::getLocalization("M_FAIRY_QUALITY"),quality);
				fairyQualityLabel->setString(str);
			}
			else
			{
				fairyQualityLabel->setString("");
			}
		}
		showCurrentFairyFetter(pos,fairyId);
	}
	else
	{
		UILabel* localLevelLabel = backPackLayout->FindChildObjectByName<UILabel>("fairyLockLevelLabel");
		if(localLevelLabel)
		{
			char levelStr[30];
			unsigned int requiredLevel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyPosUnlockLevel",pos);
			const char* str = Localizatioin::getLocalization("M_ENCHASE_FAIRY_LOCK");
			sprintf(levelStr,"%u%s",requiredLevel,str);
			localLevelLabel->setString(levelStr);	
		}
	}
}

void BackPackLayer::showCurrentFairyFetter(int pos,int fairyId)
{
	UIScrollList* chemicalTextLabel = backPackLayout->FindChildObjectByName<UIScrollList>("chemicalTextList");
	if(chemicalTextLabel)
	{
		chemicalTextLabel->clearChildrens();
		for(int i=6;i>0;i--)
		{
			unsigned int fetterId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyFetterIdById",fairyId,i);
			if(fetterId != 0)
			{
				const char* name = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua", "G_GetFetterNameByFetterId",fetterId);
				ccColor3B color = KQualityOrangeColor;
				bool isActivation = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyFetterIsActivation",pos,fetterId);
				if(isActivation)
				{
					color = KQualityOrangeColor;
				}
				else
				{
					color = KQualityGrayColor;
				}
				CCSprite* sprite = AspriteManager::getInstance()->getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_TEXT_CHEMICAL1");
				UILabelTTF* label = UILabelTTF::create(name,KJLinXin,16,sprite->getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
				label->setColor(color);
				chemicalTextLabel->AddBaseItem(label->getContentSize(),label);
			}
		}
		CCSize size = chemicalTextLabel->getContentSize();
		CCSize viewSize = chemicalTextLabel->getViewSize();
		//CCPoint startPoint = chemicalTextLabel->getContentOffset();
		CCPoint startPoint = CCPointZero;
		CCPoint offset = ccp(startPoint.x + size.width - viewSize.width,startPoint.y);
		chemicalTextLabel->setStartOffset(offset);
	}

	/*for(int i=1;i<6;i++)
	{
		
		char chemicalLabelName[50];
		sprintf(chemicalLabelName,"chemicalTextLabel%u",i);
		UILabel* chemicalTextLabel = backPackLayout->FindChildObjectByName<UILabel>(chemicalLabelName);
		if(chemicalTextLabel)
		{
			unsigned int fetterId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyFetterIdById",fairyId,i);
			if(fetterId != 0)
			{
				const char* name = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua", "G_GetFetterNameByFetterId",fetterId);
				if(name)
				{
					chemicalTextLabel->setString(name);
				}
				bool isActivation = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Fairy/FairyLuaInterface.lua", "G_GetFairyFetterIsActivation",pos,fetterId);
				if(isActivation)
				{
					chemicalTextLabel->setColor(KQualityOrangeColor);
				}
				else
				{
					chemicalTextLabel->setColor(KQualityGrayColor);
				}
			}
			else
			{
				chemicalTextLabel->setString("");
			}
		}
	}*/
	 
}

void BackPackLayer::resetFairyListPostion(int currentPage)
{
	UIScrollList* uiList = backPackLayout->FindChildObjectByName<UIScrollList>("fairyList");
	if(uiList)
	{
		std::list<UIControlBase*>::iterator it = uiList->getChildren().begin();
		for(; it != uiList->getChildren().end(); it++)
		{
			if((*it) != NULL)
			{
				if((*it)->getTag() == 5 - currentPage)
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
						m_lastClickedFairyIndex = 5 - m_currentEquipPage;
						bt->getMenuItemSprite()->selected();
					}
				}
			}
		}
	}
}

void BackPackLayer::unselectAllBagItem(bool isBag)
{
	if(isBag)
	{
		unsigned int maxPos = ItemManager::Get()->getBackPackMaxPos();
		for(unsigned int i = 1;i <= maxPos;i++)
		{
			unsigned int page = (i-1) / EACH_PAGE_NUMBER;
			unsigned int row = ((i-1) % EACH_PAGE_NUMBER) / EACH_ROW_NUMBER;
			unsigned int colmun = ((i-1) % EACH_PAGE_NUMBER) % EACH_ROW_NUMBER;
			IconButton* button = m_backPackScrollPage->GetIconButton(page,colmun,row);
			if(button)
			{
				CCMenuItem* item = button->getMenuItem();
				item->unselected();
			}
		}
	}
	else
	{
		unsigned int maxPos = ItemManager::Get()->getStorageMaxPos();
		for(unsigned int i = 1;i <= maxPos;i++)
		{
			unsigned int page = (i-1) / EACH_PAGE_NUMBER;
			unsigned int row = ((i-1) % EACH_PAGE_NUMBER) / EACH_ROW_NUMBER;
			unsigned int colmun = ((i-1) % EACH_PAGE_NUMBER) % EACH_ROW_NUMBER;
			IconButton* button = m_storageScrollPage->GetIconButton(page,colmun,row);
			if(button)
			{
				CCMenuItem* item = button->getMenuItem();
				item->unselected();
			}
		}
	}
}

void BackPackLayer::reqToUnlockBagItem(unsigned int pos)
{
	b_isUnlockBagFlag = true;

	if(pos >= kItemStorage)
	{
		b_isUnlockBagFlag = false;
	}
	unsigned int startPos;
	UIScrollPage* itemPage = NULL;
	unsigned int startParamId;

	if(b_isUnlockBagFlag)
	{
		startPos = kItemBag;
		itemPage = m_backPackScrollPage;
		startParamId = 40250;
	}
	else
	{
		startPos = kItemStorage;
		itemPage = m_storageScrollPage;
		startParamId = 40700;
	}

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
			if(itemPage)
			{	
				// 增加选中效果
				unsigned int page = (index-1) / EACH_PAGE_NUMBER;
				unsigned int row = ((index-1) % EACH_PAGE_NUMBER) / EACH_ROW_NUMBER;
				unsigned int colmun = ((index-1) % EACH_PAGE_NUMBER) % EACH_ROW_NUMBER;
				IconButton* button = itemPage->GetIconButton(page,colmun,row);
				if(button)
				{
					CCMenuItem* item = button->getMenuItem();
					item->selected();
				}
			}

			if(index >= 63)
			{
				 int paramId = startParamId; // 钻石解锁花费ID
				 paramId += 10 * (index - 63);
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
			menu_selector(BackPackLayer::onReceivedConfirmBagItemUnlock),
			menu_selector(BackPackLayer::onReceivedCancellBagItemUnlock),this);	
		LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",diamondCost);
	}
}

void BackPackLayer::onFragItemClicked(CCObject* sender)
{
	if(m_fragScrollPage)
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
                    DragReceiver* receiver = button->getDragReceiver();
                    if(receiver)
                    {
                        unsigned int pos = receiver->GetHandlerPosition();
						DragHandlerType type = receiver->GetDragItemType();
						CCAssert(type == EFragBag,"error frag pos");
						m_tipPos = pos + kEquipFragmet;
						BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);
						if(item && !item->isEmpty)
						{
							ItemManager::Get()->showItemTipsByPos(m_tipPos,menu->getTouchPoint());
						}
						
					}
				}
			}
		}
	}
}

void BackPackLayer::onAlchemyItemClicked(CCObject* sender)
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
				unsigned int pos = button->getTag();
				pos += kAlchemyHeroEquip + m_currentEquipPage * 100;
				ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
			}
		}
	}
}

void BackPackLayer::onItemClicked(CCObject* sender)
{
	if(m_backPackScrollPage)
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
                    DragReceiver* receiver = button->getDragReceiver();
                    if(receiver)
                    {
                        unsigned int pos = receiver->GetHandlerPosition();
                        //m_DagPos = pos;
						
						DragHandlerType type = receiver->GetDragItemType();
						if(type == EStorageItem)
						{
							pos += kItemStorage;
						}
						else if(type != EBackPackItem)
						{
							return;
						}
						m_tipPos = pos;
						BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
						if(item)
						{
							if(item->isLocked)
							{
								// 已锁
								reqToUnlockBagItem(pos);
							}
							else if(!item->isEmpty)
							{
								ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
								ItemBase* info = item->getItemInfo();
								EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
								if(equipInfo)
								{
									const char* breakText = Localizatioin::getLocalization("M_DECOMPOSITION_BUTTON1");
									const char* equipText = Localizatioin::getLocalization("M_DECOMPOSITION_BUTTON2");
									ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onEquipBreakCallBack),breakText,true,
										this,menu_selector(BackPackLayer::onEquipCallBack),equipText,true);
									/*const char* getText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
									ItemManager::Get()->setTipMiddleDelegate(this,menu_selector(BackPackLayer::onGetWayCallBack),getText,true);*/
									if(item->b_isNew)
									{
										ItemManager::Get()->setEquipNewNote(pos,false);
									}
								}
								else if(info)
								{
									const char* sellText = Localizatioin::getLocalization("M_BUTTON_TIPS3");
									const char* useText = Localizatioin::getLocalization("M_BUTTON_TIPS1");
									const char* mixText = Localizatioin::getLocalization("M_BUTTON_TIPS2");
									const char* multiUseText = Localizatioin::getLocalization("M_AKEYUSE");
									const char* exchangeText = Localizatioin::getLocalization("M_DECOMPOSITION_BUTTON1");
									
									switch(info->m_type)
									{
									case 1:
									case 4:
									case 18:
									//case 20:
									case 22:
										{
											
											if(info->m_banch == 1)
											{
												ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onItemMultiUseCallBack),multiUseText,true,
													this,menu_selector(BackPackLayer::onItemUseCallBack),useText,true);
											}
											else
											{
												ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
													this,menu_selector(BackPackLayer::onItemUseCallBack),useText,true);
											}
											
											break;
										}
									case 20:
										{
											ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onItemSellCallBack),sellText,true,
												this,menu_selector(BackPackLayer::onItemUseCallBack),useText,true);
											break;
										}
									case 21:
										{
											ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onItemUseCallBack),exchangeText,true,
												this,menu_selector(BackPackLayer::onItemMixCallBack),mixText,true);
											break;
										}
									case 2:
										{
											/*ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onItemSellCallBack),sellText,true,
												this,menu_selector(BackPackLayer::onItemUseCallBack),mixText,true);*/
											ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
												this,menu_selector(BackPackLayer::onItemUseCallBack),mixText,true);
											break;
										}
									case 16:
										{
											ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
												this,menu_selector(BackPackLayer::onItemMixCallBack),mixText,true);
											break;
										}
									case 25:
										{
											ItemManager::Get()->setTipDelegate(NULL,NULL,"",false,
												this,menu_selector(BackPackLayer::onChangeNameCallBack),useText,true);
											break;
										}
									default:
										{
											//ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onItemSellCallBack),sellText,true,
											//	NULL,NULL,"",false);
											break;
										}
									}
								}
							}
							
						}
                    }
				}
			}
		}
	}
}

void BackPackLayer::onEquipmentItemClicked(CCObject* sender)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			for(int i =1;i<=6;i++)
			{
				UIButton* button = getEquipmentButton(i);
				if(button->getCurrentNode() == menu)
				{
					DragReceiver* receiver = button->getDragReceiver();
					if(receiver)
					{
						unsigned int pos = receiver->GetHandlerPosition();
						DragHandlerType type = receiver->GetDragItemType();
						if(type != EEquipmentItem)
						{
							return;
						}
						
						if(m_currentEquipPage != 5)
						{
							pos += kItemHeroEquip + m_currentEquipPage * 100;
							m_tipPos = pos;
							// 装备格
							BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
							if(item)
							{
								ItemManager::Get()->showItemTipsByPos(pos,menu->getTouchPoint());
								const char* mixText = Localizatioin::getLocalization("M_UPGRADE");
								const char* takeOffText = Localizatioin::getLocalization("M_STRENGTHENEQUIPAGE_TEXT_8");
								ItemManager::Get()->setTipDelegate(this,menu_selector(BackPackLayer::onEquipUpgradeCallBack),mixText,true,
									this,menu_selector(BackPackLayer::onEquipTakeOffCallBack),takeOffText,true);
								/*const char* getText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
								ItemManager::Get()->setTipMiddleDelegate(this,menu_selector(BackPackLayer::onGetWayCallBack),getText,true);*/

							}
						}
						else
						{
							// 精灵格
							unsigned int fairyPos = FAIRY_POS_START_PRT + i -1;
							m_tipPos = fairyPos;
							if(!isFairyPosLocked(fairyPos))
							{
								unsigned int fairyId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIdByPos",fairyPos);
								/*if(fairyId != 0)
								{
									const char* fairyIcon = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID",fairyId);
									CCSprite* sprite = AspriteManager::getInstance()->getOneFrame("UI/ui_system_icon.bin",fairyIcon);
									button->setIconSprite(sprite);
								}
								else
								{
									button->setIconSprite(NULL);
								}*/
								if(fairyId == 0)
								{
									LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_SelectFairyToEquip",false);
									//m_isFairySelectModle = true;
								}
								else
								{
									LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyInfoLayer",fairyPos,1);
								}
							}
						}
					}
					break;
				}
			}
		}
	}
}

void BackPackLayer::onItemMixCallBack(CCObject* sender)
{
	if(m_tipPos != 0)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);

		if(item && !item->isEmpty && !item->isLocked)
		{
			ItemBase* info = item->getItemInfo();
			if(info)
			{
				ExchangeItem* exchangeItem = ItemManager::Get()->getExchageItemByIdWithType(info->m_id,0);
				if(exchangeItem)
				{
					OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(exchangeItem->m_exchangeId,m_tipPos);
				}
			}
		}
	}
}

void BackPackLayer::onItemSellCallBack(CCObject* sender)
{
	 PVPMessage::Get()->sendToBag(m_tipPos);
}

void BackPackLayer::onItemMultiUseCallBack(CCObject* sender)
{
	if(m_tipPos != 0)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);

		if(item && !item->isEmpty && !item->isLocked)
		{
			ItemBase* info = item->getItemInfo();
			if(info->m_banch == 1)
			{
				OnlineNetworkManager::sShareInstance()->sendUseItem(m_tipPos,info->m_id,item->amount);
			}
		}
	}
}

void BackPackLayer::onEquipTakeOffCallBack(CCObject* sender)
{
	if(m_tipPos != 0)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);
		if(item && !item->isEmpty && !item->isLocked)
		{
			unsigned int pos = ItemManager::Get()->getBackPackEmptyGridIndex();
			if(pos != 0)
			{
				OnlineNetworkManager::sShareInstance()->sendBackPackMove(m_tipPos,pos);
			}
			else
			{
				const char* msg = Localizatioin::getLocalization("M_FINDSPIRIT_BACKPACKFULL");
				if(msg)
				{
					ShowMessage(msg);
				}
			}
		}
	}
}

void BackPackLayer::onGetWayCallBack(CCObject* sender)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);
	if(item && !item->isEmpty && !item->isLocked)
	{
		LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyGetwayLayer",item->getItemId());
	}
}

void BackPackLayer::onEquipUpgradeCallBack(CCObject* sender)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);

	if(item && !item->isEmpty && !item->isLocked)
	{
		// to do upgrade
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/equipUpgradeLayer.lua","setUpgradedEquipPos",m_tipPos);
		int tag = MainMenuLayer::getLayerTagByName("EquipUpgradeLayer");
		MainMenuLayer::showUILayerByTag<CCLayer>(tag);
	}
}

void BackPackLayer::onEquipCallBack(CCObject* sender)
{
	if(m_tipPos != 0)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);
		if(item && !item->isEmpty && !item->isLocked)
		{
			ItemBase* info = item->getItemInfo();
			EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
			if(equipInfo)
			{
				unsigned int toPos = equipInfo->m_equipPart + kItemHeroEquip + m_currentEquipPage * 100;
				OnlineNetworkManager::sShareInstance()->sendBackPackMove(m_tipPos,toPos);
			}
		}
		
	}
	
}

void BackPackLayer::onEquipBreakCallBack(CCObject* sender)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);
	if(item && !item->isEmpty && !item->isLocked)
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/EquipBreakLayer.lua","setBreakedEquipPos",m_tipPos);
		int tag = MainMenuLayer::getLayerTagByName("EquipBreakLayer");
		MainMenuLayer::showUILayerByTag<CCLayer>(tag);
	}
}

void BackPackLayer::onItemUseCallBack(CCObject* sender)
{
	if(m_tipPos != 0)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(m_tipPos);
		
		if(item && !item->isEmpty && !item->isLocked)
		{
			ItemBase* info = item->getItemInfo();
			if(info)
			{
				switch(info->m_type)
				{
				case 1:
				case 4:
				case 18:
				case 20:
				case 21:
				case 22:
					{
						OnlineNetworkManager::sShareInstance()->sendUseItem(m_tipPos,info->m_id);
						break;
					}
				}
			}
		}
	}
}

void BackPackLayer::onChangeNameCallBack(CCObject* sender)
{
	MainMenuLayer::showUILayerByTag<CCLayer>(MainMenuLayer::getLayerTagByName("ChangeNameLayer"),true);
}

//void BackPackLayer::onEquipMixCallBack(CCObject* sender)
//{
//
//}

bool BackPackLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(isVisible() && (b_isBackPackOpened || b_isStorageOpened || b_isEquipmentOpened || b_isAttributeOpened))
	{
		return true;
	}
	return false;
	/*bool isInBackPack = checkInBackPackBg(pTouch);
	bool isInEquipment = checkInEquipmentBg(pTouch);
	bool isInStorage = checkInStoragekBg(pTouch);
	return (isInBackPack || isInEquipment || isInStorage);*/
}

void BackPackLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void BackPackLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void BackPackLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void BackPackLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void BackPackLayer::closeLayerCallBack(void)
{
	this->setVisible(false);
}

void BackPackLayer::checkLayerHide()
{
	bool isHide = false;

	if(!this->isVisible())
	{
		isHide = true;
	}
	else if(!b_isBackPackOpened && !b_isStorageOpened && !b_isEquipmentOpened && !b_isAttributeOpened)
	{
		isHide = true;
	}

	if(isHide)
	{
		MainMenuLayer::RemoveOneLayer(this);
	}
}

void BackPackLayer::reOrderBackPackItems(CCObject* sender)
{
	unsigned int val = 1;
	if(b_isBagPageFlag)
	{
		val = 1;
	}
	else
	{
		val = 14;
	}
	OnlineNetworkManager::sShareInstance()->sendBackPackReOrder(val);
}

void BackPackLayer::reOrderStorageItems(CCObject* sender)
{
	OnlineNetworkManager::sShareInstance()->sendBackPackReOrder(2);
}

bool BackPackLayer::checkInShopUIPackBg(CCTouch *pTouch)
{
    CCLOG("call BackPackLayer::checkInShopUIPackBg(CCTouch *pTouch)\n");
    CCPoint point = pTouch->getLocation();
	bool ret = false;
	if(m_pShopUILayer != NULL)
	{
		m_pShopUILayer->setShopTouchEndPt(point);
		ret = m_pShopUILayer->IsInShop(point);
	}
	
	return ret;
}

bool BackPackLayer::checkInBackPackBg(CCTouch *pTouch)
{
	if(this->isVisible())
	{
		if(b_isBackPackOpened)
		{
			CCPoint point = pTouch->getLocation();
			if((point.x >= m_packbgPoint.x - m_packbgSize.width /2) && (point.x <= m_packbgPoint.x + m_packbgSize.width /2)
				&& (point.y >= m_packbgPoint.y - m_packbgSize.height /2) && (point.y <= m_packbgPoint.y + m_packbgSize.height /2))
			{
				return true;
			}
		}
	}
	return false;
}

bool BackPackLayer::checkInStoragekBg(CCTouch *pTouch)
{
	if(this->isVisible())
	{
		if(b_isStorageOpened)
		{
			CCPoint point = pTouch->getLocation();
			if((point.x >= m_storagebgPoint.x - m_storagebgSize.width /2) && (point.x <= m_storagebgPoint.x + m_storagebgSize.width /2)
				&& (point.y >= m_storagebgPoint.y - m_storagebgSize.height /2) && (point.y <= m_storagebgPoint.y + m_storagebgSize.height /2))
			{
				return true;
			}
		}
	}
	return false;
}

bool BackPackLayer::checkInEquipmentBg(CCTouch *pTouch)
{
	if(this->isVisible())
	{
		if(b_isEquipmentOpened)
		{
			CCPoint point = pTouch->getLocation();
			if((point.x >= m_equipmentbgPoint.x - m_equipmentbgSize.width /2) && (point.x <= m_equipmentbgPoint.x + m_equipmentbgSize.width /2)
				&& (point.y >= m_equipmentbgPoint.y - m_equipmentbgSize.height /2) && (point.y <= m_equipmentbgPoint.y + m_equipmentbgSize.height /2))
			{
				return true;
			}
		}

	}
	return false;
}

TXGUI::UIButton* BackPackLayer::getEquipmentButton(int index)
{
	UIButton* button;
	switch(index)
	{
		case 1:
			button = m_equipmentHelmetButton;
			break;
		case 2:
			button = m_equipmentNecklaceButton;
			break;
		case 3:
			button = m_equipmentWeaponButton;
			break;
		case 4:	
			button = m_equipmentArmorButton;
			break;
		case 5:
			button = m_equipmentRingButton;
			break;
		case 6:
			button = m_equipmentShoeButton;
			break;
		default:
			button = NULL;
			break;
	}
	return button;
}

void BackPackLayer::onBagButtonClicked()
{
	if(b_isBackPackOpened || b_isEquipmentOpened || b_isStorageOpened || b_isAttributeOpened)
	{
		if(b_isBackPackOpened && b_isEquipmentOpened)
		{
			setVisible(false);
			checkLayerHide();
		}
		else
		{
			openBackPack(true);
		}
	}
	else
	{
		openBackPack(true);
		//OnlineNetworkManager::sShareInstance()->sendBackPackMessage();
	}
}

void BackPackLayer::onAlchemyLeftBtClicked(CCObject* sender)
{
	showPlayerAlchemy(false);
}

void BackPackLayer::onAlchemyRightBtClicked(CCObject* sender)
{
	showPlayerAlchemy(true);
}

void BackPackLayer::OnNPCBagClicked()
{
    if(b_isBackPackOpened || b_isEquipmentOpened || b_isStorageOpened || b_isAttributeOpened)
	{
		if(b_isBackPackOpened && b_isEquipmentOpened)
		{
			setVisible(false);
			checkLayerHide();
		}
		else
		{
			openExchangeShop();
		}
	}
	else
	{
		openExchangeShop();
	}
}


void BackPackLayer::OnNPCBagMagicShopClicked()
{
//     int unLockLevel = 16;
// 	if (UserData::Get()->GetUserLevel() < unLockLevel)
// 	{
// 		std::string flag = "M_MYSTERYSHOP_LOCKED";
// 		std::string content = Localizatioin::getLocalization(flag.c_str());
// 		ShowMessage(content.c_str());
// 		return ;
// 	}
        
    if(b_isStorageOpened)
	{
		if(m_storageContainer && m_storageContainer->getCurrentNode())
		{
			m_storageContainer->getCurrentNode()->setVisible(false);
		}
		b_isStorageOpened = false;
	}
    
	if(b_isAttributeOpened)
	{
		UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
		if(attributeContainer && attributeContainer->getCurrentNode())
		{
			attributeContainer->getCurrentNode()->setVisible(false);
		}
		b_isAttributeOpened = false;
	}

	MainMenuLayer::PushOneLayer(this);

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(true, 3);
    }
}

void BackPackLayer::showBackPackContainer(bool isRetIcon/* = true*/)
{
	if(!b_isBackPackOpened)
	{
		if(m_backPackContainer && m_backPackContainer->getCurrentNode())
		{
			m_backPackContainer->getCurrentNode()->setVisible(true);
			b_isBackPackOpened = true;
			if(isRetIcon)
			{
				unsigned int maxPos = ItemManager::Get()->getBackPackMaxPos();
				for(unsigned int i=1;i<= maxPos;i++)
				{
					BackPackItem* item = ItemManager::Get()->findItemByPos(i);
					if(item)
					{
						setOneBackPackIcon(item,i);
					}
				}
				for(unsigned int i=1;i<= maxPos;i++)
				{
					BackPackItem* item = ItemManager::Get()->findItemByPos(i+kEquipFragmet);
					if(item)
					{
						setOneFragmentIcon(item,i);
					}
				}
			}
		}
		if(isRetIcon)
		{
			showCoinLabel();
			showFragStoneLabel();
			showEquipFragCanComposeIcon();
			showBagNewEquipIcon();
			setBagIndex(b_isBagPageFlag);
		}
	}
}

void BackPackLayer::onEquipAllBtClicked(CCObject* sender)
{
	CCLOG("BackPackLayer::onEquipAllBtClicked");
	if(m_lastClickedFairyIndex <= 5 && m_lastClickedFairyIndex > 0)
	{
		unsigned int fairyPos = getFairyPosByListIndex(m_lastClickedFairyIndex);
		if(m_lastClickedFairyIndex == 5)
		{
			fairyPos = 0;
		}
		if(!isFairyPosLocked(fairyPos) || m_lastClickedFairyIndex == 5)
		{
			OnlineNetworkManager::sShareInstance()->sendEquipAllMsg(fairyPos);
		}
	}
	
}

//void BackPackLayer::onFairyInfoClicked(CCObject* sender)
//{
//	CCLOG("BackPackLayer::onFairyInfoClicked");
//	if(m_lastClickedFairyIndex <= 5 && m_lastClickedFairyIndex > 0)
//	{
//		unsigned int fairyPos = getFairyPosByListIndex(m_lastClickedFairyIndex);
//		if(!isFairyPosLocked(fairyPos))
//		{
//			LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyInfoLayer",fairyPos,2);
//		}
//	}
//}


void BackPackLayer::onFetterInfoClicked(CCObject* sender)
{
	CCLOG("BackPackLayer::onFetterInfoClicked");
	int tag = MainMenuLayer::getLayerTagByName("FairyContactInfoLayer");
	MainMenuLayer::getUILayerByTag<CCLayer>(tag,true);
}

unsigned int BackPackLayer::getFairyPosByListIndex(int index)
{
	unsigned int fairyPos = 0;
	if(index == 4 || index == 3)
	{
		fairyPos = FAIRY_POS_START_BTL + (4 - m_lastClickedFairyIndex);
	}
	else if(index == 2 || index == 1)
	{
		fairyPos = FAIRY_POS_START_AST + (2 - m_lastClickedFairyIndex);
	}
	return fairyPos;
}

//void BackPackLayer::TrainButtonClicked(CCObject* sender)
//{
//	CCLOG("TrainButtonClicked");
//	// Note: 隐藏右侧的边框
//	HideRightPanel();
//	ShowElfStrengthenPanel();
//}

void BackPackLayer::HideRightPanel()
{
	b_isBackPackOpened = false;
	b_isStorageOpened = false;
	b_isAttributeOpened = false;
	if(m_backPackContainer && m_backPackContainer->getCurrentNode())
	{
		m_backPackContainer->getCurrentNode()->setVisible(false);
	}
	if(m_storageContainer && m_storageContainer->getCurrentNode())
	{
		m_storageContainer->getCurrentNode()->setVisible(false);
	}
	UIContainer* attributeContainer = backPackLayout->FindChildObjectByName<UIContainer>("attributeContainer");
	if(attributeContainer && attributeContainer->getCurrentNode())
	{
		attributeContainer->getCurrentNode()->setVisible(false);
	}
}

void BackPackLayer::ShowElfStrengthenPanel()
{
	SpriteElfConfigFromLuaManager::getInstance()->EquipElfPotientionPanelToBackPackLayer(this);
}

void BackPackLayer::ShowElfPanelWithBackPackLayer()
{
	openBackPack(true);
	//TrainButtonClicked(0);
}

void BackPackLayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_ATTR_BUY_BAG_CAPACITY:
		{
			if(m_backPackContainer && m_backPackContainer->getCurrentNode())
			{
				unsigned int maxPos = ItemManager::Get()->getBackPackMaxPos();
				for(unsigned int i=1;i<= maxPos;i++)
				{
					BackPackItem* item = ItemManager::Get()->findItemByPos(i);
					if(item)
					{
						setOneBackPackIcon(item,i);
					}
				}
			}	
		}
	case GM_ATTR_BUY_STORE_CAPACITY:
		{
			if(m_storageContainer && m_storageContainer->getCurrentNode())
			{
				unsigned int maxPos = ItemManager::Get()->getStorageMaxPos();
				for(unsigned int i=1;i<= maxPos;i++)
				{
					BackPackItem* item = ItemManager::Get()->findItemByPos(i + kItemStorage);
					if(item)
					{
						setOneStorageIconn(item,i);
					}
				}
			}	
		}
	case GM_ATTR_HEALTH_POINT_UPDATE:
		{
			USERINFO info = UserData::GetUserInfo();
			UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("LifeNumberLable");
			if(label)
			{
				char text[20];
				sprintf(text,"%d",int(info.total_hp));
				label->setString(text);
			}
			break;
		}
	case GM_ATTR_COURAGE_UPDATE:
		{
			USERINFO info = UserData::GetUserInfo();
			UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("PowerNumberLable");
			if(label)
			{
				char text[20];
				sprintf(text,"%d",int(info.courage));
				label->setString(text);
			}
			break;
		}
	case GM_ATTR_CHARM_UPDATE:
		{
			USERINFO info = UserData::GetUserInfo();
			UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("SkillNumberLable");
			if(label)
			{
				char text[20];
				sprintf(text,"%d",int(info.trick));
				label->setString(text);
			}
			break;
		}
	case GM_ATTR_TRICK_UPDATE:
		{
			USERINFO info = UserData::GetUserInfo();
			UILabel* label = backPackLayout->FindChildObjectByName<UILabel>("IntellectNumberLable");
			if(label)
			{
				char text[20];
				sprintf(text,"%d",int(info.charm));
				label->setString(text);
			}
			break;
		}
	case GM_ATTR_HERO_LEVEL_UPDATE:
		{
			showPlayerLevel();
			setFairyIcon(4,FAIRY_POS_START_BTL);
			setFairyIcon(3,FAIRY_POS_START_BTL + 1);
			setFairyIcon(2,FAIRY_POS_START_AST);
			setFairyIcon(1,FAIRY_POS_START_AST + 1);
			break;
		}
	case GM_ATTR_ABILITY_ALL_UPDATE:
		{
			showPlayerEffectiveness();
			break;
		}
	case GM_ATTR_FAIRY_EFFECTIVENESS:
		{
			onRecieveFairyEffectiveness();
			break;
		}
	case GM_ATTR_FAIRY_FETTER:
		{
			onReceiveFairyFetter();
			break;
		}
	/*case GM_ATTR_EXP_UPDATE:
		{
			showPlayerExp();
			break;
		}*/
    case GM_ATTR_COIN_UPDATE:
        {
			showCoinLabel();
			if(m_pShopUILayer != NULL)
			{
				m_pShopUILayer->updateCoinAndDiamand();
			}
			break;
        }
	case GM_ATTR_FRAG_STONE:
		{
			showFragStoneLabel();
			break;
		}
	case GM_ATTR_FRAG_CAN_CONPOSE:
		{
			showEquipFragCanComposeIcon();
			break;
		}
	case GM_FAIRY_SELECT_DONE:
		{
			onReceiveFairySelectEnd();
			break;
		}
	case GM_ATTR_NEW_EQUIP_CHANGE:
		{
			showBagNewEquipIcon();
			break;
		}
	case GM_ATTR_VIP_LEVEL_CHANGE:
		{
			showPlayerVip();
			break;
		}
	case GM_UPDATE_USER_EQUIP_TITLE:
		{
			showPlayerEquipTitle();
			break;
		}
	case GM_CHANGE_USER_GUILD:
		{
			showGuildName();
			break;
		}
	case GM_GET_PLAYER_CHANGE_NAME:
		{
			if(backPackLayout)
			{
				UILabelTTF* m_userName = (UILabelTTF*)backPackLayout->FindChildObjectByName<UILabel>("playerNameLabel")->getCurrentNode();
				if(m_userName)
				{
					m_userName->setString(UserData::getUserName());
				}
			}
			break;
		}
	}
}

void BackPackLayer::dragToEquipItem(unsigned int fromPos,unsigned int toPos)
{
	// to do m_currentEquipPage
	unsigned int equipPos = toPos + kItemHeroEquip + m_currentEquipPage * 100;
	OnlineNetworkManager::sShareInstance()->sendBackPackMove(fromPos,equipPos);
}

void BackPackLayer::ajustScrollPageByIndex(string pageName, int index)
{
	if (pageName == string("backPackScrollPage") && m_backPackScrollPage)
	{
		int pageIndex = index / EACH_PAGE_NUMBER;
		m_backPackScrollPage->moveToPage(pageIndex);
	}
}




