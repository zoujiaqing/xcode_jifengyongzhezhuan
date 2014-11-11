#include "OnlineNetworkManager.h"
#include "EquipmentUpgradeUILayer.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "ItemManager.h"
#include "AspriteManager.h"
#include "GameConfigFromLuaManager.h"
#include "cs_core.pb.h"
#include "Localization.h"
#include "UserData.h"
#include "GMessage.h"
#include "GameAudioManager.h"
#include "particle_manager.h"
#include "LuaTinkerManager.h"
#include "MessageBoxLayer.h"
#include "TimeManager.h"
//static unsigned int m_coldTime;
//static long long m_laststengthTime;

EquipmentUpgradeUILayer::EquipmentUpgradeUILayer()
{
	m_equipmentLayout = NULL;
	m_choosedEquipIndex = 0;
	m_equipLayoutIndex = true;
	m_upTimesBar = NULL;
	m_choosedEquipBt = NULL;
	m_choosedEquipmentPic = NULL;
	m_remainTime = 0;
}

EquipmentUpgradeUILayer::~EquipmentUpgradeUILayer()
{
	UIManager::sharedManager()->RemoveUILayout("equipmentUpgradeLayout");
	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
	TimeManager::Get()->unregisterTimer(this, PB_ATTR_EQUIP_LVUP_CD);
}

bool EquipmentUpgradeUILayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();
	m_equipmentLayout = manager->CreateUILayoutFromFile("UIplist/equipmentUpgrade.plist", this, "equipmentUpgradeLayout");

	manager->RegisterMenuHandler("equipmentUpgradeLayout","EquipmentUpgradeUILayer::closeEquipUpgradeBt",
		menu_selector(EquipmentUpgradeUILayer::closeEquipUpgradeBt), this);
	manager->RegisterMenuHandler("equipmentUpgradeLayout","EquipmentUpgradeUILayer::chooseOneEquipmentBt",
		menu_selector(EquipmentUpgradeUILayer::chooseOneEquipmentBt), this);
	manager->RegisterMenuHandler("equipmentUpgradeLayout","EquipmentUpgradeUILayer::onClickHeroEquip",
		menu_selector(EquipmentUpgradeUILayer::onClickHeroEquip), this);
	manager->RegisterMenuHandler("equipmentUpgradeLayout","EquipmentUpgradeUILayer::onClickFairyEquip",
		menu_selector(EquipmentUpgradeUILayer::onClickFairyEquip), this);
	manager->RegisterMenuHandler("equipmentUpgradeLayout","EquipmentUpgradeUILayer::onClickStrength",
		menu_selector(EquipmentUpgradeUILayer::onClickStrength), this);
	
	m_equipmentHelmetPic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeHelmetIcon");
	m_equipmentNecklacePic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeNicklaceIcon");
	m_equipmentWeaponPic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeWeaponIcon");
	m_equipmentArmorPic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeArmorIcon");
	m_equipmentRingPic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeRingIcon");
	m_equipmentShoePic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeShoeIcon");
	m_choosedEquipmentPic = m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradechoosedIcon");
	m_heroEquipToggle = m_equipmentLayout->FindChildObjectByName<UIButtonToggle>("heroEquipButtonToggle");
	m_fairyEquipToggle = m_equipmentLayout->FindChildObjectByName<UIButtonToggle>("fairyEquipButtonToggle");
	m_levelUpCostLabel = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedCostNumber");
	m_levelLabel = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedLevel");
	m_coldTimeLabel = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeCDText");
    
	showPlayerEquipment();
    
	if(m_heroEquipToggle)
	{
		m_heroEquipToggle->selected();
	}

	m_upTimesBar = (CCSprite*)m_equipmentLayout->FindChildObjectByName<UIPicture>("equipUpgradeBarTime")->getCurrentNode();
	if(m_upTimesBar)
	{
		m_upTimesBar->setAnchorPoint(ccp(0,0.5f));
		CCPoint oldPos = m_upTimesBar->getPosition();
		float scale = UIManager::sharedManager()->getScaleFactor();
		m_upTimesBar->setPosition(ccp(oldPos.x-scale * m_upTimesBar->getContentSize().width/2,oldPos.y));
	}
	setUpgradeColdTime();
	showLvUpRemainTimes();
	setDefaultSelectedEquip();
	updateBottomCoinLabel();
	updateBottomDiamondLabel();



	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
	return true;
}

void EquipmentUpgradeUILayer::closeEquipUpgradeBt(CCObject* sender)
{
	/*MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
	mLayer->HideEquipmentUpgradeUI();
	}*/
	MainMenuLayer::hideUILayerByTag(this->getTag());
	MainMenuLayer::RemoveOneLayer(this);
}

void EquipmentUpgradeUILayer::closeLayerCallBack(void)
{
	/*MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
	mLayer->HideEquipmentUpgradeUI();
	}*/
	MainMenuLayer::hideUILayerByTag(this->getTag());
}

void EquipmentUpgradeUILayer::showEquipLevel(unsigned int index,unsigned int level)
{
	if(m_equipmentLayout)
	{
		char lableName[30];
		sprintf(lableName,"equipUpgradeLevelLabel%d",index);
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>(lableName);
		if(label)
		{
			if(level != 0)
			{
				char text[10];
				sprintf(text,"lv %d",level);
				label->setString(text);
			}
			else
			{
				label->setString("");
			}
		}
	}
}

void EquipmentUpgradeUILayer::showEquipName(unsigned int index,const char* name,const ccColor3B& color)
{
	if(m_equipmentLayout)
	{
		char lableName[30];
		sprintf(lableName,"equipUpgradeLabel%d",index);
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>(lableName);
		if(label)
		{
			label->setString(name);
			label->setColor(color);
		}
	}
}

void EquipmentUpgradeUILayer::showSingleEquip(unsigned int pos,unsigned int index)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
	if(item)
	{
		ItemBase* info = item->getItemInfo();
		if(info)
		{
			if(item->isEmpty)
			{
				showEquipLevel(index,0);
				showEquipName(index,"");
				UIPicture* pic = getEquipmentPic(index);
				if(pic)
				{
					pic->setSprite(NULL);
				}
				return;
			}
			else
			{
				UIPicture* pic = getEquipmentPic(index);
				if(pic)
				{
					CCSprite* equipPic = getIconSpriteById(item->getItemId());
					if(equipPic)
					{	
						pic->setSprite(equipPic);
						showEquipLevel(index,item->itemLevel);
						ItemBase* itemInfo = item->getItemInfo();
						if(itemInfo)
						{		
							showEquipName(index,itemInfo->m_name,ItemManager::Get()->getLabelColorByQuality(info->m_quality));
							if(itemInfo->m_quality > 0)
							{
								CCSprite* framePic = getIconFrame(itemInfo->m_quality);
								if(framePic)
								{
									pic->addExtraPic(framePic);
								}
							}	
						}
					}
				}
			}
		}
		else
		{
			showEquipLevel(index,0);
			showEquipName(index,"");
			UIPicture* pic = getEquipmentPic(index);
			if(pic)
			{
				pic->setSprite(NULL);
			}
		}
	}
	else
	{
		showEquipLevel(index,0);
		showEquipName(index,"");
		UIPicture* pic = getEquipmentPic(index);
		if(pic)
		{
			pic->setSprite(NULL);
		}
	}
}

void EquipmentUpgradeUILayer::showPlayerEquipment()
{
	for(unsigned int i=1;i<= 6;i++)
	{
		showSingleEquip(kItemHeroEquip + i,i);
	}
}

void EquipmentUpgradeUILayer::showFairyEquipment()
{
	for(unsigned int i=1;i<= 6;i++)
	{
		showSingleEquip(kItemFairyEquip +i,i);
	}
}

CCSprite* EquipmentUpgradeUILayer::getIconFrame(unsigned int level)
{
	string picName = "";
	switch(level)
	{
	case 1:
		picName = "map_ui_system_icon_FRAME_FRAME_GREEN";
		break;
	case 2:
		picName = "map_ui_system_icon_FRAME_FRAME_BLUE";
		break;
	case 3:
		picName = "map_ui_system_icon_FRAME_FRAME_PURPLE";
		break;
	case 4:
		picName = "map_ui_system_icon_FRAME_FRAME_ORANGE";
		break;
	}
	if(picName != "")
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
		int idx = getResourceIDByName(picName.c_str());
		CCPoint pt;
		CCSprite * pic = as->getSpriteFromFrame_Middle( idx, 0, pt);
		if(pic != NULL)
		{
			return pic;
		}
	}
	return NULL;
}

CCSprite* EquipmentUpgradeUILayer::getIconSpriteById(unsigned int ItemId)
{
	ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
	int idx = GameConfigFromLuaManager::Get()->getItemResourceId(ItemId);
	CCPoint pt;
	CCSprite * pic = as->getSpriteFromFrame_Middle( idx, 0, pt);
	return pic;
}

TXGUI::UIPicture* EquipmentUpgradeUILayer::getEquipmentPic(int index)
{
	UIPicture* pic;
	switch(index)
	{
	case 1:
		pic = m_equipmentHelmetPic;
		break;
	case 2:
		pic = m_equipmentNecklacePic;
		break;
	case 3:
		pic = m_equipmentWeaponPic;
		break;
	case 4:	
		pic = m_equipmentArmorPic;
		break;
	case 5:
		pic = m_equipmentRingPic;
		break;
	case 6:
		pic = m_equipmentShoePic;
		break;
	default:
		pic = NULL;
		break;
	}
	return pic;
}

TXGUI::UIButton* EquipmentUpgradeUILayer::getEquipmentButton(int index)
{
	if(m_equipmentLayout)
	{
		char btName[30];
		sprintf(btName,"equipUpgradeBt%d",index);
		UIButton* bt = m_equipmentLayout->FindChildObjectByName<UIButton>(btName);
		return bt;
	}
	return NULL;
}

void EquipmentUpgradeUILayer::chooseOneEquipmentBt(CCObject* sender)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(sender);
	if(item && item->getParent())
	{
		CCNode* parent = item->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(parent);
		if(menu)
		{
			UIControlBase* control = menu->getControlNode();
			UIButton* button = dynamic_cast<UIButton*>(control);
			if(button)
			{
				unsigned int index = button->getButtonIndex();
				setChoosedEquipment(index);
			}
		}
	}
}

void EquipmentUpgradeUILayer::setChoosedEquipment(unsigned int index)
{
	if(m_choosedEquipIndex == index)
	{
		if(m_choosedEquipBt && m_choosedEquipBt->getMenuItemSprite())
		{
			m_choosedEquipBt->getMenuItemSprite()->selected();
		}
		return;
	}
	
	m_choosedEquipIndex = index;
	if(m_choosedEquipBt && m_choosedEquipBt->getMenuItemSprite())
	{
		m_choosedEquipBt->getMenuItemSprite()->unselected();
	}
	m_choosedEquipBt = getEquipmentButton(index);
	if(m_choosedEquipBt && m_choosedEquipBt->getMenuItemSprite())
	{
		m_choosedEquipBt->getMenuItemSprite()->selected();
	}

	

	if(m_choosedEquipmentPic)
	{
		unsigned int pos = index;
		if(m_equipLayoutIndex)
		{
			pos += kItemHeroEquip;
		}
		else
		{
			pos += kItemFairyEquip;
		}
		BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
		if(item)
		{
			if(item->isEmpty)
			{
				m_choosedEquipmentPic->setSprite(NULL);
				showLevelUpCost(0);
				showCurrentLevel(0);
				showCurrentName("");
				showCurrentAttributeLabel("");
				showSecondAttributeLabel("");
				showCurrentAttributeNum(0);
				showSecondAttributeNum(0);
				showNextLevelAttributionLabel("");
				showNextLevelAttributionNum("");
				showSecondNextLevelAttributionLabel("");
				showSecondNextLevelAttributionNum("");
				return;
			}
			if(item->m_attribution)
			{
				std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
				int index = 0;
				while(it != item->m_attribution->end())
				{
					ITEMATTR* attr = *it;
					if(attr && (attr->attr_basic_value != 0 || attr->attr_intensify_value != 0))
					{
						if(index == 0)
						{
							showCurrentAttribute(attr->attr_key,attr->attr_basic_value,attr->attr_intensify_value);
							index++;
							it++;
						}
						else if(index == 1)
						{
							showSecondAttribute(attr->attr_key,attr->attr_basic_value,attr->attr_intensify_value);
							index++;
							break;
						}
					}
					else
					{
						it++;
					}
				}

				if(index == 1)
				{
					showSecondAttributeLabel("");
					showSecondAttributeNum(0);
				}
				
			}
			if(item->m_equipInfo)
			{
				std::list<EQUIPNEXTATTR>::iterator it = item->m_equipInfo->m_nextAttribution.begin();
				if(it != item->m_equipInfo->m_nextAttribution.end())
				{
					EQUIPNEXTATTR attr = *it;
					unsigned int num = attr.value;
					for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
						it != item->m_attribution->end();it++)
					{
						ITEMATTR* itemAttr = *it;
						if(itemAttr && (itemAttr->attr_key == attr.key))
						{
							num += itemAttr->attr_basic_value + itemAttr->attr_intensify_value;
							break;
						}
					}
					
					showNextLevelAttribution(attr.key,num);
				}
				if(++it != item->m_equipInfo->m_nextAttribution.end())
				{
					EQUIPNEXTATTR attr = *it;
					unsigned int num = attr.value;
					for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
						it != item->m_attribution->end();it++)
					{
						ITEMATTR* itemAttr = *it;
						if(itemAttr && (itemAttr->attr_key == attr.key))
						{
							num += itemAttr->attr_basic_value + itemAttr->attr_intensify_value;
							break;
						}
					}
					showSecondNextLevelAttribution(attr.key,num);
				}
				else
				{
					showSecondNextLevelAttributionLabel("");
					showSecondNextLevelAttributionNum("");
				}
				showLevelUpCost(item->m_equipInfo->levelUpCost);
			}
			
			CCSprite* equipPic = getIconSpriteById(item->getItemId());
			if(equipPic)
			{	
				
				showCurrentLevel(item->itemLevel);

				m_choosedEquipmentPic->setSprite(equipPic);
				ItemBase* itemInfo = item->getItemInfo();
				if(itemInfo && itemInfo > 0)
				{
					showCurrentName(itemInfo->m_name,ItemManager::Get()->getLabelColorByQuality(itemInfo->m_quality));

					if(itemInfo->m_quality > 0)
					{
						CCSprite* framePic = getIconFrame(itemInfo->m_quality);
						if(framePic)
						{
							m_choosedEquipmentPic->addExtraPic(framePic);
						}
					}
					
				}
			}
		}
		
	}
}

long EquipmentUpgradeUILayer::GetUpgradeColdTime()
{
	//return m_coldTime;
	return m_remainTime;
}

void EquipmentUpgradeUILayer::setUpgradeColdTime()
{
	//m_coldTime = value;
	//m_laststengthTime = millisecondNow();

	long nowTime = TimeManager::Get()->getCurServerTime();

	USERINFO info = UserData::GetUserInfo();
	m_remainTime = info.m_equipLvUpCd - nowTime;

	m_remainTime = (m_remainTime > 0) ? m_remainTime : 0;

	if(m_remainTime > 0)
	{
		initRemainTimer(m_remainTime);
	}

	showColdTime(m_remainTime);
}

void EquipmentUpgradeUILayer::onClickHeroEquip(CCObject* sender)
{
	if(m_heroEquipToggle)
	{
		m_heroEquipToggle->selected();
	}
	if(m_fairyEquipToggle)
	{
		m_fairyEquipToggle->unselected();
	}
	if(!m_equipLayoutIndex)
	{
		m_equipLayoutIndex = true;
		showPlayerEquipment();
		unSelectChoosedEquip();
		setDefaultSelectedEquip();
	}
}

void EquipmentUpgradeUILayer::onClickFairyEquip(CCObject* sender)
{
	if(checkFairyEquip())
	{
		if(m_fairyEquipToggle)
		{
			m_fairyEquipToggle->unselected();
		}
		const char* text = Localizatioin::getLocalization("M_FAIRY_LOCK");
		ShowMessage(text);
	}
	else
	{
		if(m_heroEquipToggle)
		{
			m_heroEquipToggle->unselected();
		}
		if(m_fairyEquipToggle)
		{
			m_fairyEquipToggle->selected();
		}
		if(m_equipLayoutIndex)
		{
			m_equipLayoutIndex = false;
			showFairyEquipment();
			unSelectChoosedEquip();
			setDefaultSelectedEquip();
		}
	}
	
}

void EquipmentUpgradeUILayer::onClickStrength(CCObject* sender)
{
	if(checkLvUp())
	{
		unsigned int pos = m_choosedEquipIndex;
		if(m_equipLayoutIndex)
		{
			pos += kItemHeroEquip;
		}
		else
		{
			pos += kItemFairyEquip;
		}
		OnlineNetworkManager::sShareInstance()->sentEquipUpgradeStrength(pos);
	}
	else
	{
		const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", 10000, "FromItems");
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
			long long currentTime = millisecondNow();
			//long long deltaTime = (currentTime - m_laststengthTime)/1000;
			//unsigned int time = (deltaTime > m_coldTime ) ?  0 : (m_coldTime - deltaTime);

			unsigned int diamond = (m_remainTime / 60) * num;
			if(m_remainTime % 60 != 0)
			{
				diamond += num;
			}

			const char* str = Localizatioin::getLocalization("M_SKIP_CD");
			const char* diamondStr = Localizatioin::getLocalization("S_Diamond_content");
			char costStr[100];
			sprintf(costStr,"%s%d%s",str,diamond,diamondStr);
			TXGUI::MessageBox::Show(costStr,this,menu_selector(EquipmentUpgradeUILayer::onClickCleanCDBt),NULL);
		}
	}
}

void EquipmentUpgradeUILayer::onClickCleanCDBt(CCObject* sender)
{
	OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(10000);
}

void EquipmentUpgradeUILayer::unSelectChoosedEquip()
{
	m_choosedEquipIndex = 0;
	if(m_choosedEquipmentPic)
	{
		m_choosedEquipmentPic->setSprite(NULL);
	}
	showCurrentLevel(0);
	showCurrentName("");
	showCurrentAttributeLabel("");
	showSecondAttributeLabel("");
	showCurrentAttributeNum(0);
	showSecondAttributeNum(0);
	showLevelUpCost(0);
	showNextLevelAttributionLabel("");
	showNextLevelAttributionNum("");
	showSecondNextLevelAttributionLabel("");
	showSecondNextLevelAttributionNum("");
}

void EquipmentUpgradeUILayer::successToStrength(unsigned int pos)
{
	GameAudioManager::sharedManager()->playEffect(350006,false);
     
	BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
	playUpgradeEffect();
	bool bHeroEquip = true;
	unsigned int index = pos;
	if(index >= kItemFairyEquip)
	{
		bHeroEquip = false; 
		index -= kItemFairyEquip;
	}
	else
	{
		index -= kItemHeroEquip;
	}

	if(m_equipLayoutIndex == bHeroEquip && m_choosedEquipIndex == index)
	{
		if(item)
		{
			showColdTime(m_remainTime);
			showCurrentLevel(item->itemLevel);

			showSingleEquip(pos,index);
			if(item->m_attribution)
			{
				std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
				int index = 0;
				while(it != item->m_attribution->end())
				{
					ITEMATTR* attr = *it;
					if(attr && (attr->attr_basic_value != 0 || attr->attr_intensify_value != 0))
					{
						if(index == 0)
						{
							showCurrentAttribute(attr->attr_key,attr->attr_basic_value,attr->attr_intensify_value);
							index++;
							it++;
						}
						else if(index == 1)
						{
							showSecondAttribute(attr->attr_key,attr->attr_basic_value,attr->attr_intensify_value);
							index++;
							break;
						}
					}
					else
					{
						it++;
					}
				}

				if(index == 1)
				{
					showSecondAttributeLabel("");
					showSecondAttributeNum(0);
				}

			}

			if(item->m_equipInfo)
			{
				std::list<EQUIPNEXTATTR>::iterator it = item->m_equipInfo->m_nextAttribution.begin();
				if(it != item->m_equipInfo->m_nextAttribution.end())
				{
					EQUIPNEXTATTR attr = *it;
					unsigned int num = attr.value;
					for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
						it != item->m_attribution->end();it++)
					{
						ITEMATTR* itemAttr = *it;
						if(itemAttr && (itemAttr->attr_key == attr.key))
						{
							num += itemAttr->attr_basic_value + itemAttr->attr_intensify_value;
							break;
						}
					}
					showNextLevelAttribution(attr.key,num);
				}
				if(++it != item->m_equipInfo->m_nextAttribution.end())
				{
					EQUIPNEXTATTR attr = *it;
					unsigned int num = attr.value;
					for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
						it != item->m_attribution->end();it++)
					{
						ITEMATTR* itemAttr = *it;
						if(itemAttr && (itemAttr->attr_key == attr.key))
						{
							num += itemAttr->attr_basic_value + itemAttr->attr_intensify_value;
							break;
						}
					}
					showSecondNextLevelAttribution(attr.key,num);
				}
				else
				{
					showSecondNextLevelAttributionLabel("");
				}
				showLevelUpCost(item->m_equipInfo->levelUpCost);
			}
		}
		
	}
}

void EquipmentUpgradeUILayer::showLevelUpCost(unsigned int cost)
{
	if(m_levelUpCostLabel)
	{
		char costString[10];
		sprintf(costString,"%d",cost);
		m_levelUpCostLabel->setString(costString);
	}
}

void EquipmentUpgradeUILayer::showCurrentLevel(unsigned int level)
{
	if(m_levelLabel)
	{
		if(level != 0)
		{
			char levelString[10];
			sprintf(levelString,"lv %d",level);
			m_levelLabel->setString(levelString);
		}
		else
		{
			m_levelLabel->setString("");
		}
	}
}

void EquipmentUpgradeUILayer::showCurrentName(const char* name,const ccColor3B& color)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedName");
		if(label)
		{
			label->setString(name);
			label->setColor(color);
		}
	}
}

void EquipmentUpgradeUILayer::showCurrentAttribute(unsigned int key,unsigned int basicValue,unsigned int strenghValue)
{
	unsigned int value = basicValue + strenghValue;
	//const char* text = ItemManager::Get()->getAttributionText(key,value);
	const char* text = ItemManager::Get()->getAttributionKeyText(key);
	if(text)
	{
		showCurrentAttributeLabel(text);
		showCurrentAttributeNum(value);
	}
}

void EquipmentUpgradeUILayer::showCurrentAttributeNum(unsigned int num)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpSkillNum");
		if(label)
		{
			if(num != 0)
			{
				char numStr[20];
				sprintf(numStr,"+%u",num);
				label->setString(numStr);
			}
			else
			{
				label->setString("");
			}
			
		}
	}
}

void EquipmentUpgradeUILayer::showCurrentAttributeLabel(const char* text)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedAddSkill");
		if(label)
		{
			label->setString(text);
		}
	}
}

void EquipmentUpgradeUILayer::showSecondAttribute(unsigned int key,unsigned int basicValue,unsigned int strenghValue)
{
	unsigned int value = basicValue + strenghValue;
	//const char* text = ItemManager::Get()->getAttributionText(key,value);
	const char* text = ItemManager::Get()->getAttributionKeyText(key);
	if(text)
	{
		showSecondAttributeLabel(text);
		showSecondAttributeNum(value);
	}
}

void EquipmentUpgradeUILayer::showSecondAttributeLabel(const char* text)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedAddSkill2");
		if(label)
		{
			label->setString(text);
		}
	}
}

void EquipmentUpgradeUILayer::showSecondAttributeNum(unsigned int num)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpSkillNum2");
		if(label)
		{
			if(num != 0)
			{
				char numStr[20];
				sprintf(numStr,"+%u",num);
				label->setString(numStr);
			}
			else
			{
				label->setString("");
			}
		}
	}
}

void EquipmentUpgradeUILayer::showNextLevelAttribution(unsigned int key,unsigned int strenghValue)
{
	//const char* text = ItemManager::Get()->getAttributionText(key,strenghValue);
	const char* text = ItemManager::Get()->getAttributionKeyText(key);
	if(text)
	{
		if(m_equipmentLayout)
		{
			UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedAddNextLevel");
			if(label)
			{
				char nextAttr[80];
				sprintf(nextAttr,"%s%s",Localizatioin::getLocalization("M_NEXTLEVEL"),text);
				label->setString(nextAttr);
			}

			label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpNextLevelNum");
			if(label)
			{
				char numStr[20];
				sprintf(numStr,"+%u",strenghValue);
				label->setString(numStr);
			}
		}
	}
}

void EquipmentUpgradeUILayer::showSecondNextLevelAttribution(unsigned int key,unsigned int strenghValue)
{
	//const char* text = ItemManager::Get()->getAttributionText(key,strenghValue);
	const char* text = ItemManager::Get()->getAttributionKeyText(key);
	if(text)
	{
		if(m_equipmentLayout)
		{
			UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedAddNextLevel2");
			if(label)
			{
				char nextAttr[80];
				sprintf(nextAttr,"%s%s",Localizatioin::getLocalization("M_NEXTLEVEL"),text);
				label->setString(nextAttr);
			}

			label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpNextLevelNum2");
			if(label)
			{
				char numStr[20];
				sprintf(numStr,"+%u",strenghValue);
				label->setString(numStr);
			}
		}
	}
}

void EquipmentUpgradeUILayer::showNextLevelAttributionLabel(const char* text)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedAddNextLevel");
		if(label)
		{
			label->setString(text);
		}
	}
}

void EquipmentUpgradeUILayer::showSecondNextLevelAttributionLabel(const char* text)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpgradeChoosedAddNextLevel2");
		if(label)
		{
			label->setString(text);
		}
	}
}

void EquipmentUpgradeUILayer::showSecondNextLevelAttributionNum(const char* text)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpNextLevelNum2");
		if(label)
		{
			label->setString(text);
		}
	}
}

void EquipmentUpgradeUILayer::showNextLevelAttributionNum(const char* text)
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("equipUpNextLevelNum");
		if(label)
		{
			label->setString(text);
		}
	}
}

void EquipmentUpgradeUILayer::showColdTime(long time)
{
	if(m_coldTimeLabel)
	{
		char timeString[30];
		unsigned int hour = time / 3600;
		unsigned int min = (time - hour * 3600) / 60;
		unsigned int second = time % 60;
		if(hour > 0)
		{
			sprintf(timeString,"%02d : %02d : %02d",hour,min,second);
		}
		else 
		{
			sprintf(timeString,"%02d : %02d",min,second);
		}
		m_coldTimeLabel->setString(timeString);
	}
}

void EquipmentUpgradeUILayer::onUpdateRemainTime(long remainTime)
{
	m_remainTime = remainTime;
	showLvUpRemainTimes();
	showColdTime(m_remainTime);
}

void EquipmentUpgradeUILayer::showLvUpRemainTimes()
{
	USERINFO userInfo = UserData::GetUserInfo();
	if(m_remainTime == 0 && userInfo.m_equipLvFlag != 0)
	{
		userInfo.m_equipLvFlag = 0;
		UserData::SetUserInfo(userInfo);
	}

	float percent = 0.0f;
	if(userInfo.m_equipLvFlag == 1)
	{
		percent = 1.0f;
	}
	else if(0 != userInfo.m_equipLvUpMaxTimes)
	{
		percent = (1.0f * m_remainTime) / userInfo.m_equipLvUpMaxTimes;
	}

	percent = (percent > 1.0f) ? 1.0f : percent;
	if(m_upTimesBar)
	{
		float scale = UIManager::sharedManager()->getScaleFactor();
		m_upTimesBar->setScaleX(percent * scale);
		m_upTimesBar->setScaleY(scale);
	}

	if(m_equipmentLayout)
	{
		UIButton* button = m_equipmentLayout->FindChildObjectByName<UIButton>("strengthEquipBt");
		if(button != NULL && button->getTextLabel() != NULL)
		{
			if(percent == 1.0f)
			{
				const char* text = Localizatioin::getLocalization("M_AUTOFIGHT_SPEEDUP");
				button->getTextLabel()->setString(text);
			}
			else
			{
				const char* text = Localizatioin::getLocalization("M_UPGRADE");
				button->getTextLabel()->setString(text);
			}
		}
	}
}
bool EquipmentUpgradeUILayer::checkLvUp()
{
	USERINFO info = UserData::GetUserInfo();
	if(info.m_equipLvFlag == 0 || m_remainTime == 0)
	{
		return true;
	}
	/*if(info.m_equipLvUpMaxTimes - info.m_equipLvFlag > 0)
	{
		return true;
	}*/
	return false;
}

void EquipmentUpgradeUILayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_ATTR_EQUIP_LVUP_UESED_TIMES:
	case GM_ATTR_EQUIP_LVUP_CD:
		setUpgradeColdTime();
		showLvUpRemainTimes();
		break;
	case GM_ATTR_SP_DIAMOND_UPDATE:
		updateBottomDiamondLabel();
		break;
	case GM_ATTR_COIN_UPDATE:
		updateBottomCoinLabel();
		break;
	}
}

void EquipmentUpgradeUILayer::setDefaultSelectedEquip()
{
	unsigned int pos;
	if(m_equipLayoutIndex)
	{
		pos = kItemHeroEquip;
	}
	else
	{
		pos = kItemFairyEquip;
	}
	for(unsigned int i =0;i < 6;i++)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(pos+i+1);
		if(item && !item->isEmpty && !item->isLocked)
		{
			setChoosedEquipment(i+1);
			break;
		}
	}

}

void EquipmentUpgradeUILayer::updateBottomDiamondLabel()
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("bottomDiamondLable");
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
void EquipmentUpgradeUILayer::updateBottomCoinLabel()
{
	if(m_equipmentLayout)
	{
		UILabel* label = m_equipmentLayout->FindChildObjectByName<UILabel>("bottomCoinLable");
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

void EquipmentUpgradeUILayer::playUpgradeEffect()
{
	if(m_choosedEquipmentPic && m_choosedEquipmentPic->getCurrentNode())
	{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_upgradesuccess");
		if(particle != NULL)
		{
			CCNode* child = m_choosedEquipmentPic->getCurrentNode()->getChildByTag(100);
			if(child)
			{
				child->removeFromParentAndCleanup(true);
				child = NULL;
			}
			m_choosedEquipmentPic->getCurrentNode()->addChild(particle,100,100);
			particle->setPosition(ccp(m_choosedEquipmentPic->getCurrentNode()->getContentSize().width * 0.5,m_choosedEquipmentPic->getCurrentNode()->getContentSize().height * 0.5));
		}
	}
}

void EquipmentUpgradeUILayer::initRemainTimer(long seconds)
{
	long nowTime = TimeManager::Get()->getCurServerTime();
	if(TimeManager::Get()->hasTimer(PB_ATTR_EQUIP_LVUP_CD))
	{
		TimeManager::Get()->renewTimer(PB_ATTR_EQUIP_LVUP_CD, seconds + nowTime);
		if (!TimeManager::Get()->hasObserver(this, PB_ATTR_EQUIP_LVUP_CD))
		{
			TimeManager::Get()->attachTimer(this, PB_ATTR_EQUIP_LVUP_CD);
		}
	}
	else
	{
		TimeManager::Get()->registerTimer(this, PB_ATTR_EQUIP_LVUP_CD, seconds + nowTime);
	}
}

bool EquipmentUpgradeUILayer::checkFairyEquip()
{
	bool isLockFairy = false;
	BackPackItem* item = ItemManager::Get()->findItemByPos(kItemFairyEquip+1);
	if(item && item->isEmpty)
	{
		isLockFairy = true;
	}
	return isLockFairy;
}

long long millisecondNow()
{ 
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL);
    long long vTime = now.tv_sec;
    vTime  = vTime * 1000;
    vTime = vTime + now.tv_usec / 1000;
	return vTime;
} 