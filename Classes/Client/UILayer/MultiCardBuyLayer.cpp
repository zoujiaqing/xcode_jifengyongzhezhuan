#include "OnlineNetworkManager.h"
#include "MultiCardBuyLayer.h"
#include "MainMenuLayer.h"
#include "ItemManager.h"
#include "Localization.h"

#define LAGREST_CARD_MOUNT 999

MultiCardBuyLayer::MultiCardBuyLayer()
{
	m_uiLayout = NULL;
	m_currentExchageId = 0;
	m_numToBuy = 1;
	m_singlePrice = 0;
}

MultiCardBuyLayer::~MultiCardBuyLayer()
{
	UIManager::sharedManager()->RemoveUILayout("MultiCard");
}

bool MultiCardBuyLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	bool ret = false;

	do 
	{
		m_uiLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/MultiCard.plist", this, "MultiCard");

		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onAddOneBtClicked",
			menu_selector(MultiCardBuyLayer::onAddOneBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onAddTwoBtClicked",
			menu_selector(MultiCardBuyLayer::onAddTwoBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onAddThreeBtClicked",
			menu_selector(MultiCardBuyLayer::onAddThreeBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onMinusOneBtClicked",
			menu_selector(MultiCardBuyLayer::onMinusOneBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onMinusTwoBtClicked",
			menu_selector(MultiCardBuyLayer::onMinusTwoBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onMinusThreeBtClicked",
			menu_selector(MultiCardBuyLayer::onMinusThreeBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onConfirmBtClicked",
			menu_selector(MultiCardBuyLayer::onConfirmBtClicked), this);
		UIManager::sharedManager()->RegisterMenuHandler("MultiCard","MultiCardBuyLayer::onCancellBtClicked",
			menu_selector(MultiCardBuyLayer::onCancellBtClicked), this);
		ret = true;
	} while (0);

	return ret;
}

void MultiCardBuyLayer::onAddOneBtClicked(CCObject* obj)
{
	addCardNumToBuy(1);
}

void MultiCardBuyLayer::onAddTwoBtClicked(CCObject* obj)
{
	addCardNumToBuy(10);
}

void MultiCardBuyLayer::onAddThreeBtClicked(CCObject* obj)
{
	addCardNumToBuy(100);
}

void MultiCardBuyLayer::onMinusOneBtClicked(CCObject* obj)
{
	minusCardNumToBuy(1);
}

void MultiCardBuyLayer::onMinusTwoBtClicked(CCObject* obj)
{
	minusCardNumToBuy(10);
}

void MultiCardBuyLayer::onMinusThreeBtClicked(CCObject* obj)
{
	minusCardNumToBuy(100);
}

void MultiCardBuyLayer::onConfirmBtClicked(CCObject* obj)
{
	unsigned int diamond = m_numToBuy * m_singlePrice;
	if(diamond > UserData::GetDiamond())
	{
		// ×êÊ¯²»×ã
		const char* warning = Localizatioin::getLocalization("S_Gh_Diamond_Content");
		ShowMessage(warning);
	}
	else
	{
		OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(m_currentExchageId,0,m_numToBuy);
		this->setVisible(false);
	}
	
}

void MultiCardBuyLayer::onCancellBtClicked(CCObject* obj)
{
	this->setVisible(false);
}

void MultiCardBuyLayer::setCardExchangeId(unsigned int exchangeId)
{
	m_currentExchageId = exchangeId;
	ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
	if(item != NULL)
	{
		CCAssert(item->m_exchangeType == 7,"not card exchange");
		unsigned int itemId = 0;
		m_singlePrice = 0;
		m_numToBuy = 1;
		std::list<std::pair<unsigned int ,unsigned int> >::iterator it = item->m_toItems->begin();
		if(it != item->m_toItems->end())
		{
			itemId = it->first;
		}
		it = item->m_fromItems->begin();
		if(it != item->m_fromItems->end())
		{
			unsigned int diamondId  = it->first;
			CCAssert(diamondId == 416002, "not spend diamond");
			m_singlePrice = it->second;
		}

		showSpendDiamond(m_numToBuy,m_singlePrice);
		showCardNum(m_numToBuy);
		this->setVisible(true);
	}
}

void MultiCardBuyLayer::addCardNumToBuy(unsigned int num)
{
	m_numToBuy += num;
	if(m_numToBuy > LAGREST_CARD_MOUNT)
	{
		m_numToBuy = LAGREST_CARD_MOUNT;
	}
	showSpendDiamond(m_numToBuy,m_singlePrice);
	showCardNum(m_numToBuy);
}

void MultiCardBuyLayer::minusCardNumToBuy(unsigned int num)
{
	if(m_numToBuy <= num)
	{
		m_numToBuy = 1;
	}
	else
	{
		m_numToBuy -= num;
	}
	showSpendDiamond(m_numToBuy,m_singlePrice);
	showCardNum(m_numToBuy);
}

void MultiCardBuyLayer::showSpendDiamond(unsigned int cardNum,unsigned int price)
{
	if(m_uiLayout != NULL)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("diamondNumber");
		unsigned int diamond = cardNum * price;
		char diamondStr[10];
		sprintf(diamondStr,"%u",diamond);
		label->setString(diamondStr);
	}
}

void MultiCardBuyLayer::showCardNum(unsigned int cardNum)
{
	if(m_uiLayout != NULL)
	{
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>("buyNumber");
		char diamondStr[10];
		sprintf(diamondStr,"%u",cardNum);
		label->setString(diamondStr);
	}
}