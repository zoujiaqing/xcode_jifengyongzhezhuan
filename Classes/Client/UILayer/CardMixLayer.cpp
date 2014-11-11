#include "OnlineNetworkManager.h"
#include "CardMixLayer.h"
#include "ItemManager.h"
#include "LuaTinkerManager.h"
#include "cs_core.pb.h"
#include "Localization.h"

CardMixLayer::CardMixLayer()
{
	m_uiLayout = NULL;
	m_fromCardPic = NULL;
	m_fromCardAttr = NULL;
	m_fromCardName = NULL;
	m_toCardAttr = NULL;
	m_toCardName = NULL;
	m_toCardPic = NULL;
	m_CardNumLabel = NULL;
	m_MixCardNumLabel = NULL;
	m_carId = 0;
	m_exchangeId = 0;
	m_cardNum = 0;
	isComposeFlag = false;
}

CardMixLayer::~CardMixLayer()
{
	UIManager::sharedManager()->RemoveUILayout("cardMixLayout");
}

bool CardMixLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	m_uiLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/cardMixUI.plist", this, "cardMixLayout");

	m_fromCardPic = m_uiLayout->FindChildObjectByName<UIPicture>("cardFrame1");
	m_fromCardAttr = m_uiLayout->FindChildObjectByName<UILabel>("cardAttr1");
	m_fromCardName = m_uiLayout->FindChildObjectByName<UILabel>("cardType1");
	m_toCardAttr = m_uiLayout->FindChildObjectByName<UILabel>("cardAttr2");
	m_toCardName = m_uiLayout->FindChildObjectByName<UILabel>("cardType2");
	m_CardNumLabel = m_uiLayout->FindChildObjectByName<UILabel>("cardNum");
	m_MixCardNumLabel = m_uiLayout->FindChildObjectByName<UILabel>("mixCardNum");
	m_toCardPic = m_uiLayout->FindChildObjectByName<UIPicture>("cardFrame2");


	UIManager::sharedManager()->RegisterMenuHandler("cardMixLayout","CardMixLayer::onConfirmBtClicked",
		menu_selector(CardMixLayer::onConfirmBtClicked), this);

	UIManager::sharedManager()->RegisterMenuHandler("cardMixLayout","CardMixLayer::onCancellBtClicked",
		menu_selector(CardMixLayer::onCancellBtClicked), this);

	this->setVisible(false);
	return true;
}

void CardMixLayer::showCardMixLayer(unsigned int cardId,CCSprite* fromCardSprite,unsigned int mixCard,CCSprite* mixSprite,
									unsigned int exchangeId,unsigned int cardNum,unsigned int mixCardNum)
{
	m_carId = cardId;
	m_exchangeId = exchangeId;
	m_fromCardPic->setSprite(fromCardSprite,false);
	m_toCardPic->setSprite(mixSprite,false);
	m_cardNum = mixCardNum;
	if(m_CardNumLabel)
	{
		char text[10];
		sprintf(text,"x%u",cardNum);
		m_CardNumLabel->setString(text);
	}
	if(m_MixCardNumLabel)
	{
		char text[10];
		sprintf(text,"x%u",mixCardNum);
		m_MixCardNumLabel->setString(text);
	}

	const char* fromAttrText = getCardAttrText(getCardAttrKey(cardId),getCardAttrNum(cardId,getCardAttrKey(cardId)));
	if(fromAttrText != NULL)
	{
		m_fromCardAttr->setString(fromAttrText);
	}
	else
	{
		m_fromCardAttr->setString("");
	}

	const char* toAttrText = getCardAttrText(getCardAttrKey(mixCard),getCardAttrNum(mixCard,getCardAttrKey(mixCard)));
	if(toAttrText != NULL)
	{
		m_toCardAttr->setString(toAttrText);
	}
	else
	{
		m_toCardAttr->setString("");
	}
	
	ItemBase* fromCardInfo = ItemManager::Get()->getItemInfoById(m_carId);
	if(fromCardInfo)
	{
		m_fromCardName->setString(fromCardInfo->m_name);
		
	}

	ItemBase* toCardInfo = ItemManager::Get()->getItemInfoById(mixCard);
	if(toCardInfo)
	{
		m_toCardName->setString(toCardInfo->m_name);
		
	}

	this->setVisible(true);
}

void CardMixLayer::hideLayer()
{
	this->setVisible(false);
	m_cardNum = 0;
}

void CardMixLayer::onConfirmBtClicked(CCObject* obj)
{
	OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(m_exchangeId,0,m_cardNum);
}

void CardMixLayer::onCancellBtClicked(CCObject* obj)
{
	this->hideLayer();
}

void CardMixLayer::onReceivedCardComposeSuccess(int error)
{
	if(error == 0)
	{
		this->hideLayer();
	}
}

unsigned int CardMixLayer::getCardAttrKey(unsigned int id)
{
	unsigned int num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "PhyAttack");
	if(num != 0)
	{
		return PB_ITEM_ATTR_PHYATK;
	}

	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "MagAttack");
	if(num != 0)
	{
		return PB_ITEM_ATTR_MAGATK;
	}

	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "SkiAttack");
	if(num != 0)
	{
		return PB_ITEM_ATTR_SKIATK;
	}

	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "PhyDefense");
	if(num != 0)
	{
		return PB_ITEM_ATTR_PHYDEF;
	}

	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "MagDefense");
	if(num != 0)
	{
		return PB_ITEM_ATTR_MAGDEF;
	}

	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "SkiDefense");
	if(num != 0)
	{
		return PB_ITEM_ATTR_SKIDEF;
	}

	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "HP");
	if(num != 0)
	{
		return PB_ITEM_ATTR_HP;
	}

	return 0;
}

unsigned int CardMixLayer::getCardLevel(unsigned int id)
{
	unsigned int level = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "Level");
	return level;
}

unsigned int CardMixLayer::getCardAttrNum(unsigned int id,unsigned int key)
{
	const char* keyStr;
	switch(key)
	{
	case PB_ITEM_ATTR_PHYATK:
		keyStr = "PhyAttack";
		break;
	case PB_ITEM_ATTR_MAGATK:
		keyStr = "MagAttack";
		break;
	case PB_ITEM_ATTR_SKIATK:
		keyStr = "SkiAttack";
		break;
	case PB_ITEM_ATTR_PHYDEF:
		keyStr = "PhyDefense";
		break;
	case PB_ITEM_ATTR_MAGDEF:
		keyStr = "MagDefense";
		break;
	case PB_ITEM_ATTR_SKIDEF:
		keyStr = "SkiDefense";
		break;
	case PB_ITEM_ATTR_HP:
		keyStr = "HP";
		break;
	default:
		keyStr = NULL;
		break;
	}

	if(keyStr != NULL)
	{
		unsigned int num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, keyStr);
		return num;
	}
	else
	{
		return 0;
	}
}

const char* CardMixLayer::getCardAttrText(unsigned int key,unsigned int num)
{
	const char* str;
	switch(key)
	{
	case PB_ITEM_ATTR_PHYATK:
		str = Localizatioin::getLocalization("M_CARD_PHYATTACK");
		break;
	case PB_ITEM_ATTR_MAGATK:
		str = Localizatioin::getLocalization("M_CARD_MAGATTACK");
		break;
	case PB_ITEM_ATTR_SKIATK:
		str = Localizatioin::getLocalization("M_CARD_SKIATTACK");
		break;
	case PB_ITEM_ATTR_PHYDEF:
		str = Localizatioin::getLocalization("M_CARD_PHYDEFENCE");
		break;
	case PB_ITEM_ATTR_MAGDEF:
		str = Localizatioin::getLocalization("M_CARD_MAGDEFENCE");
		break;
	case PB_ITEM_ATTR_SKIDEF:
		str = Localizatioin::getLocalization("M_CARD_SKIDEFENCE");
		break;
	case PB_ITEM_ATTR_HP:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_LIFE");
		break;
	default:
		str = NULL;
		break;
	}
	if(str != NULL)
	{
		char* text = new char[50];
		sprintf(text,"%s+%d",str,num);
		CCString* pRet = CCString::create(text);
		return pRet->m_sString.c_str();
	}
	else
	{
		return "";
	}
}

//const char* CardMixLayer::getCardAttrById(unsigned int id)
//{
//	unsigned int num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "PhyAttack");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_PHYATK,num);
//	}
//
//	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "MagAttack");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_MAGATK,num);
//	}
//
//	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "SkiAttack");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_SKIATK,num);
//	}
//
//	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "PhyDefense");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_PHYDEF,num);
//	}
//
//	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "MagDefense");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_MAGDEF,num);
//	}
//
//	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "SkiDefense");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_SKIDEF,num);
//	}
//
//	num = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("Constellation", "Constellation", id, "HP");
//	if(num != 0)
//	{
//		return ItemManager::Get()->getAttributionText(PB_ITEM_ATTR_HP,num);
//	}
//
//	return NULL;
//}