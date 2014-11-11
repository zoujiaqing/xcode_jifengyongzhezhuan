#include "CardAttributionLayer.h"
#include "ItemManager.h"
#include "CardUILayer.h"
//#include "CardMixLayer.h"

CardAttributionLayer::CardAttributionLayer()
{
	m_uiLayout = NULL;
}

CardAttributionLayer::~CardAttributionLayer()
{
	UIManager::sharedManager()->RemoveUILayout("cardAttrLayout");
}

bool CardAttributionLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	bool bRet = false;
	do 
	{
		m_uiLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/cardAttrPannel.plist", this, "cardAttrLayout");
		UIManager::sharedManager()->RegisterMenuHandler("cardAttrLayout","CardAttributionLayer::onCloseBtClicked",
			menu_selector(CardAttributionLayer::onCloseBtClicked), this);
		bRet = true;
	} while (0);

	return bRet;
}

void CardAttributionLayer::onCloseBtClicked(CCObject* obj)
{
	//this->setVisible(false);
	this->removeFromParentAndCleanup(true);
}

void CardAttributionLayer::showCardsAttr()
{
	checkUserCardsAttr();
	for(unsigned int i = PB_ITEM_ATTR_PHYATK;i <= PB_ITEM_ATTR_HP; ++i)
	{
		std::map<unsigned int,unsigned int>::iterator it = m_cardAttrMap.find(i);
		unsigned int value = 0;
		if(it != m_cardAttrMap.end())
		{
			value = it->second;
		}
		setAttrLabel(i,value);
	}
}

void CardAttributionLayer::setAttrLabel(unsigned int attrKey,unsigned int value)
{
	if(m_uiLayout != NULL)
	{
		char labelName[30];
		sprintf(labelName,"AttrLabel%d",attrKey);
		UILabel* label = m_uiLayout->FindChildObjectByName<UILabel>(labelName);
		if(label != NULL)
		{
			//const char* text = CardMixLayer::getCardAttrText(attrKey,value);
			const char* text = ItemManager::Get()->getAttributionText(attrKey,value);
			if(text != NULL)
			{
				label->setString(text);
			}
		}
	}
}

void CardAttributionLayer::checkUserCardsAttr()
{
	m_cardAttrMap.clear();
	for(int i =1;i<= CONTELLATION_NUMBER * 3; i++)
	{
		BackPackItem* item = ItemManager::Get()->findItemByPos(i+kCardEquip);
		if(item && !item->isEmpty && !item->isLocked)
		{
			if(item->m_attribution)
			{
				for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
					it != item->m_attribution->end();++it)
				{
					ITEMATTR* attr = *it;
					if(attr && attr->attr_basic_value != 0)
					{
						std::map<unsigned int,unsigned int>::iterator itor = m_cardAttrMap.find(attr->attr_key);
						if(itor != m_cardAttrMap.end())
						{
							itor->second += attr->attr_basic_value;
						}
						else
						{
							m_cardAttrMap.insert(make_pair(attr->attr_key,attr->attr_basic_value));
						}
					}
				}
			}
		}
	}
}