#include "ItemManager.h"
#include "BackPackLayer.h"
#include "AlchemyUILayer.h"
#include "CardUILayer.h"
#include "CardMixLayer.h"
#include "MainMenuLayer.h"
#include "GameConfigFromLuaManager.h"
#include "cs_core.pb.h"
#include "Localization.h"
#include "TipMgr.h"
#include "UserData.h"
#include "GMessage.h"
#include "LuaTinkerManager.h"
#include "ParticleManager.h"
#include "particle_manager.h"
#include "SkeletonAnimRcsManager.h"
ItemManager::ItemManager()
	: m_backPackList(NULL),
	m_storageList(NULL),
	m_equipmentList(NULL),
	m_exchangeItemList(NULL),
	m_maxPackPos(120),
	m_defaultManorPackNum(100),
	m_defaultPackPos(30),
	m_maxStoragePos(120),
	m_defaultStoragePos(30),
	b_isLoadExchangeItems(false),
	m_alchemyBagList(NULL),
	m_alchemyStorageList(NULL),
	m_alchemyEquipList(NULL)
{
	m_defaultAlchemyBagPos = AlCHEMYBAGCAPACITY;
	m_defaultAlchemyStoragePos = 10;
	m_alchemyFairyEquipList = NULL;
	m_searchNpcCostMap = NULL;
	m_fairyEquipmentList = NULL;
	m_gemStoneList = NULL;
	m_manorItemList = NULL;
	m_recycleItemList = NULL;
	m_cardBagList = NULL;
	m_cardEquipList = NULL;
	m_equipmentFragList = NULL;
	m_constellationCardNum = NULL;
	m_fairyFragList = NULL;
	m_equipEnchaseList = NULL;
	m_unlockPackPos = 0;
	m_unlockStoragePos = 0;
	m_unlockAlchemyBagPos = 0;
	m_unlockAlchemyStroragePos = 0;
	m_surplusGemRecastNum = 0;
	m_defaultCardEquipPos = 36;
	m_defaultCardBagPos = 100;
}

ItemManager::~ItemManager()
{
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_backPackList->clear();
		CC_SAFE_DELETE(m_backPackList);
	}
	if(m_storageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_storageList->begin();
			iter != m_storageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_storageList->clear();
		CC_SAFE_DELETE(m_storageList);
	}
	if(m_equipmentFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentFragList->begin();
			iter != m_equipmentFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_equipmentFragList->clear();
		CC_SAFE_DELETE(m_equipmentFragList);
	}
	if(m_equipmentList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentList->begin();
			iter != m_equipmentList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_equipmentList->clear();
		CC_SAFE_DELETE(m_equipmentList);
	}
	if(m_equipEnchaseList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipEnchaseList->begin();
			iter != m_equipEnchaseList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_equipEnchaseList->clear();
		CC_SAFE_DELETE(m_equipEnchaseList);
	}
	if(m_fairyEquipmentList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_fairyEquipmentList->begin();
			iter != m_fairyEquipmentList->end();++iter)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_fairyEquipmentList->clear();
		CC_SAFE_DELETE(m_fairyEquipmentList);
	}
	if(m_alchemyBagList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyBagList->begin();
			iter != m_alchemyBagList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_alchemyBagList->clear();
		CC_SAFE_DELETE(m_alchemyBagList);
	}
	if(m_alchemyStorageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyStorageList->begin();
			iter != m_alchemyStorageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_alchemyStorageList->clear();
		CC_SAFE_DELETE(m_alchemyStorageList);
	}
	if(m_alchemyEquipList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyEquipList->begin();
			iter != m_alchemyEquipList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_alchemyEquipList->clear();
		CC_SAFE_DELETE(m_alchemyEquipList);
	}
	if(m_alchemyFairyEquipList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyFairyEquipList->begin();
			iter != m_alchemyFairyEquipList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_alchemyFairyEquipList->clear();
		CC_SAFE_DELETE(m_alchemyFairyEquipList);
	}
	if(m_gemStoneList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_gemStoneList->begin();
			iter != m_gemStoneList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_gemStoneList->clear();
		CC_SAFE_DELETE(m_gemStoneList);
	}

	if (m_manorItemList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_manorItemList->begin();
			iter != m_manorItemList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			CC_SAFE_DELETE(item);
		}
		m_manorItemList->clear();
		CC_SAFE_DELETE(m_manorItemList);
	}
	
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator iter = m_exchangeItemList->begin();
			iter != m_exchangeItemList->end();iter++)
		{
			ExchangeItem *item = *iter;
			CC_SAFE_DELETE(item);
		}
		m_exchangeItemList->clear();
		CC_SAFE_DELETE(m_exchangeItemList);
	}

	if(m_equipmentList)
	{
		BACKPACK_VECTOR::iterator iter = m_equipmentList->begin();
		while(iter != m_equipmentList->end())
		{
			BackPackItem* item = (*iter).second;
			delete item;
			iter++;
		}

		m_equipmentList->clear();
		delete m_equipmentList;
		m_equipmentList = NULL;
	}
	if(m_equipEnchaseList)
	{
		BACKPACK_VECTOR::iterator iter = m_equipEnchaseList->begin();
		while(iter != m_equipEnchaseList->end())
		{
			BackPackItem* item = (*iter).second;
			delete item;
			iter++;
		}

		m_equipEnchaseList->clear();
		delete m_equipEnchaseList;
		m_equipEnchaseList = NULL;
	}
	if(m_recycleItemList)
	{
		BACKPACK_VECTOR::iterator iter = m_recycleItemList->begin();
		while(iter != m_recycleItemList->end())
		{
			BackPackItem* item = (*iter).second;
			delete item;
			iter++;
		}

		m_recycleItemList->clear();
		delete m_recycleItemList;
		m_recycleItemList = NULL;
	}

	if(m_cardBagList)
	{
		BACKPACK_VECTOR::iterator iter = m_cardBagList->begin();
		while(iter != m_cardBagList->end())
		{
			BackPackItem* item = (*iter).second;
			delete item;
			iter++;
		}

		m_cardBagList->clear();
		delete m_cardBagList;
		m_cardBagList = NULL;
	}

	if(m_cardEquipList)
	{
		BACKPACK_VECTOR::iterator iter = m_cardEquipList->begin();
		while(iter != m_cardEquipList->end())
		{
			BackPackItem* item = (*iter).second;
			delete item;
			iter++;
		}

		m_cardEquipList->clear();
		delete m_cardEquipList;
		m_cardEquipList = NULL;
	}

	if(m_fairyFragList)
	{
		BACKPACK_VECTOR::iterator iter = m_fairyFragList->begin();
		while(iter != m_fairyFragList->end())
		{
			BackPackItem* item = (*iter).second;
			delete item;
			iter++;
		}

		m_fairyFragList->clear();
		delete m_fairyFragList;
		m_fairyFragList = NULL;
	}

	if(m_searchNpcCostMap)
	{
		m_searchNpcCostMap->clear();
		delete m_searchNpcCostMap;
		m_searchNpcCostMap = NULL;
	}

	if(m_constellationCardNum)
	{
		m_constellationCardNum->clear();
		delete m_constellationCardNum;
		m_constellationCardNum = NULL;
	}
}

void ItemManager::init()
{
	if(!m_backPackList)
	{
		m_backPackList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= m_maxPackPos;i++)
		{
			BackPackItem* item = new BackPackItem(0);
			if(i <= m_defaultPackPos)
			{
				item->isLocked = false;
			}
			else
			{
				item->isLocked = true;
			}
			item->position = i;
			item->isEmpty = true;

			m_backPackList->insert(std::make_pair(i,item));
		}
	}
	if(!m_equipmentFragList)
	{
		m_equipmentFragList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= m_maxPackPos;i++)
		{
			BackPackItem* item = new BackPackItem(0);
			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;

			m_equipmentFragList->insert(std::make_pair(i,item));
		}
	}
	if(!m_storageList)
	{
		m_storageList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= m_maxStoragePos;i++)
		{
			BackPackItem* item = new BackPackItem(1);
			if(i <= m_defaultPackPos)
			{
				item->isLocked = false;
			}
			else
			{
				item->isLocked = true;
			}
			item->position = i;
			item->isEmpty = true;

			m_storageList->insert(std::make_pair(i,item));
		}
	}

	if(!m_equipmentList)
	{
		m_equipmentList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= 6;i++)
		{
			BackPackItem* item = new BackPackItem(2);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_equipmentList->insert(std::make_pair(i,item));
		}
	}
	if(!m_equipEnchaseList)
	{
		m_equipEnchaseList = new BACKPACK_VECTOR();
		for(unsigned int j =0; j<= 4;j++)
		{
			for(unsigned int i=1;i<=6;i++)
			{
				unsigned int pos = i + j * 100 + kItemEnchaseEquip - kItemHeroEquip;
				BackPackItem* item = new BackPackItem(6);
				item->isLocked = false;
				item->position = pos;
				item->isEmpty = true;
				m_equipEnchaseList->insert(std::make_pair(pos,item));
			}
		}
	}
	if(!m_fairyEquipmentList)
	{
		m_fairyEquipmentList = new BACKPACK_VECTOR();
		for(unsigned int j =1; j<= 4;j++)
		{
			for(unsigned int i=1;i<=6;i++)
			{
				unsigned int pos = i + j * 100;
				BackPackItem* item = new BackPackItem(6);
				item->isLocked = false;
				item->position = pos;
				item->isEmpty = true;
				m_fairyEquipmentList->insert(std::make_pair(pos,item));
			}
		}
		
	}

	if(!m_alchemyBagList)
	{
		m_alchemyBagList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= AlCHEMYBAGCAPACITY;i++)
		{
			BackPackItem* item = new BackPackItem(3);
			if(i <= m_defaultPackPos)
			{
				item->isLocked = false;
			}
			else
			{
				item->isLocked = true;
			}
			item->position = i;
			item->isEmpty = true;

			m_alchemyBagList->insert(std::make_pair(i,item));
		}
	}

	if(!m_alchemyStorageList)
	{
		m_alchemyStorageList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= AlCHEMYSTORAGECAPACITY;i++)
		{
			BackPackItem* item = new BackPackItem(4);
			if(i <= m_defaultPackPos)
			{
				item->isLocked = false;
			}
			else
			{
				item->isLocked = true;
			}
			item->position = i;
			item->isEmpty = true;

			m_alchemyStorageList->insert(std::make_pair(i,item));
		}
	}
	
	if(!m_alchemyEquipList)
	{
		m_alchemyEquipList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= 8;i++)
		{
			BackPackItem* item = new BackPackItem(5);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_alchemyEquipList->insert(std::make_pair(i,item));
		}
	}

	if(!m_alchemyFairyEquipList)
	{
		m_alchemyFairyEquipList = new BACKPACK_VECTOR();
		for(unsigned int j =0; j<= 3;j++)
		{
			for(unsigned int i=1;i<= 8;i++)
			{
				unsigned int pos = i + j * 100;
				BackPackItem* item = new BackPackItem(5);
				item->isLocked = false;
				item->position = pos;
				item->isEmpty = true;
				m_alchemyFairyEquipList->insert(std::make_pair(pos,item));
			}
		}
	}

	if(!m_gemStoneList)
	{
		m_gemStoneList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= 10;i++)
		{
			BackPackItem* item = new BackPackItem(5);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_gemStoneList->insert(std::make_pair(i,item));
		}
	}

	if(!m_fairyFragList)
	{
		m_fairyFragList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= FAIRYFRAGMENTBAGCAPACITY;i++)
		{
			BackPackItem* item = new BackPackItem(8);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_fairyFragList->insert(std::make_pair(i,item));
		}
	}

	if(!m_manorItemList)
	{
		m_manorItemList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= m_defaultManorPackNum;i++)
		{
			BackPackItem* item = new BackPackItem(7);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_manorItemList->insert(std::make_pair(i,item));
		}
	}
	
	if(!m_recycleItemList)
	{
		m_recycleItemList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= 20;i++)
		{
			BackPackItem* item = new BackPackItem(7);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_recycleItemList->insert(std::make_pair(i,item));
		}
	}

	if(!m_cardBagList)
	{
		m_cardBagList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= m_defaultCardBagPos;i++)
		{
			BackPackItem* item = new BackPackItem(7);

			item->isLocked = false;
			item->position = i;
			item->isEmpty = true;
			m_cardBagList->insert(std::make_pair(i,item));
		}
	}

	if(!m_cardEquipList)
	{
		m_cardEquipList = new BACKPACK_VECTOR();
		for(unsigned int i=1;i<= m_defaultCardEquipPos;i++)
		{
			BackPackItem* item = new BackPackItem(7);

			item->isLocked = (i % 3 == 1) ? false : true;
			item->position = i;
			item->isEmpty = true;
			m_cardEquipList->insert(std::make_pair(i,item));
		}
	}

	m_exchangeItemList = new std::list<ExchangeItem*>();
	m_constellationCardNum = new std::map<unsigned int ,unsigned int>();
}

BackPackItem* ItemManager::findItemByPos(unsigned int pos)
{
	unsigned int realPos = pos;
	if(realPos >= kItemRecycle)
	{
		realPos = realPos - kItemRecycle; 
		if(m_recycleItemList)
		{
			BACKPACK_VECTOR::iterator iter = m_recycleItemList->find(realPos);
			if(iter != m_recycleItemList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if (realPos >= kFairyFragment)
	{
		realPos = realPos - kFairyFragment; 
		if(m_fairyFragList)
		{
			BACKPACK_VECTOR::iterator iter = m_fairyFragList->find(realPos);
			if(iter != m_fairyFragList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
		
	}
	else if(realPos >= kEquipFragmet)
	{
		realPos = realPos - kEquipFragmet; 
		if(m_equipmentFragList)
		{
			BACKPACK_VECTOR::iterator iter = m_equipmentFragList->find(realPos);
			if(iter != m_equipmentFragList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kItemManorId)
	{
		realPos = realPos - kItemManorId; 
		if(m_manorItemList)
		{
			BACKPACK_VECTOR::iterator iter = m_manorItemList->find(realPos);
			if(iter != m_manorItemList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kCardEquip)
	{
		realPos = realPos - kCardEquip; 
		if(m_cardEquipList)
		{
			BACKPACK_VECTOR::iterator iter = m_cardEquipList->find(realPos);
			if(iter != m_cardEquipList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kCardBag)
	{
		realPos = realPos - kCardBag; 
		if(m_cardBagList)
		{
			BACKPACK_VECTOR::iterator iter = m_cardBagList->find(realPos);
			if(iter != m_cardBagList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kItemGemStone)
	{
		realPos = realPos - kItemGemStone; 
		if(m_gemStoneList)
		{
			BACKPACK_VECTOR::iterator iter = m_gemStoneList->find(realPos);
			if(iter != m_gemStoneList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kAlchemyFairyEquip && realPos < kFairyStartId)
	{
		realPos = realPos - kAlchemyFairyEquip; 
		if(m_alchemyEquipList)
		{
			BACKPACK_VECTOR::iterator iter = m_alchemyFairyEquipList->find(realPos);
			if(iter != m_alchemyFairyEquipList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kAlchemyHeroEquip)
	{
		realPos = realPos - kAlchemyHeroEquip; 
		if(m_alchemyEquipList)
		{
			BACKPACK_VECTOR::iterator iter = m_alchemyEquipList->find(realPos);
			if(iter != m_alchemyEquipList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kAlchemyStorage)
	{
		realPos = realPos - kAlchemyStorage; 
		if(m_alchemyStorageList)
		{
			BACKPACK_VECTOR::iterator iter = m_alchemyStorageList->find(realPos);
			if(iter != m_alchemyStorageList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kAlchemyBag)
	{
		realPos = realPos - kAlchemyBag; 
		if(m_alchemyBagList)
		{
			BACKPACK_VECTOR::iterator iter = m_alchemyBagList->find(realPos);
			if(iter != m_alchemyBagList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else if(realPos >= kItemFairyEquip)
	{
		realPos = realPos - kItemHeroEquip; 
		if(realPos % 100 < (kItemEnchaseEquip - kItemHeroEquip))
		{
			if(m_fairyEquipmentList)
			{
				BACKPACK_VECTOR::iterator iter = m_fairyEquipmentList->find(realPos);
				if(iter != m_fairyEquipmentList->end())
				{
					BackPackItem* item = (*iter).second;
					return item;
				}
			}
			return NULL;
		}
		else
		{
			if(m_equipEnchaseList)
			{
				BACKPACK_VECTOR::iterator iter = m_equipEnchaseList->find(realPos);
				if(iter != m_equipEnchaseList->end())
				{
					BackPackItem* item = (*iter).second;
					return item;
				}
			}
			return NULL;
		}
	}
	else if(realPos >= kItemHeroEquip)
	{
		realPos = realPos - kItemHeroEquip;
		if(realPos % 100 < (kItemEnchaseEquip - kItemHeroEquip))
		{
			if(m_equipmentList)
			{
				BACKPACK_VECTOR::iterator iter = m_equipmentList->find(realPos);
				if(iter != m_equipmentList->end())
				{
					BackPackItem* item = (*iter).second;
					return item;
				}
			}
			return NULL;
		}
		else
		{
			if(m_equipEnchaseList)
			{
				BACKPACK_VECTOR::iterator iter = m_equipEnchaseList->find(realPos);
				if(iter != m_equipEnchaseList->end())
				{
					BackPackItem* item = (*iter).second;
					return item;
				}
			}
			return NULL;
		}
	}
	else if(realPos >= kItemStorage)
	{
		realPos = realPos - kItemStorage; 
		if(m_storageList)
		{
			BACKPACK_VECTOR::iterator iter = m_storageList->find(realPos);
			if(iter != m_storageList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
	else
	{
		if(m_backPackList)
		{
			BACKPACK_VECTOR::iterator iter = m_backPackList->find(realPos);
			if(iter != m_backPackList->end())
			{
				BackPackItem* item = (*iter).second;
				return item;
			}
		}
		return NULL;
	}
}

void ItemManager::checkAlchemyEquipUnlock()
{
	for(unsigned int j=0;j<= 4;j++)
	{
		for(unsigned int i=1;i<= 8;i++)
		{
			unsigned index = kAlchemyHeroEquip + j * 100;
			BackPackItem* item = ItemManager::Get()->findItemByPos(index+i);
			if(item)
			{
				if(checkPlayerEquipUnlock(i))
				{
					item->isLocked = false;
				}
				else
				{
					item->isLocked = true;
				}
			}
		}
	}
}

bool ItemManager::checkPlayerEquipUnlock(int index)
{
	int playerLevel =  UserData::GetUserLevel();
	if((playerLevel - 40)/5 >= index)
	{
		return true;
	}
	return false;
}

void ItemManager::showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt)
{
	char particleName[30];
	sprintf(particleName,"particle_effect_spirit_%d",id);
	//if(ParticleManagerX::sharedManager()->load("particle/ccb/TestParticleSystems.ccbi"))
	//{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles(particleName);
		if(particle != NULL)
		{
			CCNode* child = parent->getChildByTag(100);
			if(child)
			{
				child->removeFromParentAndCleanup(true);
				child = NULL;
			}
			parent->addChild(particle,100,100);
			particle->setPosition(pt);
		}
	//}
}

void ItemManager::removeItem(unsigned int pos,unsigned int id)
{
	unsigned int realPos = pos;
	BackPackItem* item = findItemByPos(pos);
	if(!item || item->getItemId() != id || item->isLocked)
	{
		CCLOG("remove item error");
		return;
	}
	item->setItemId(0);
	item->isEmpty = true;
	item->amount = 0;
	item->itemLevel = 0;
	item->resetItem();

	if(item->b_isNew)
	{
		item->b_isNew = false;
		NotificationCenter::defaultCenter()->broadcast(GM_ATTR_NEW_EQUIP_CHANGE,this);
	}

	if(pos < kAlchemyBag || pos >= kEquipFragmet)
	{
		int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
		BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
		if(layer)
		{
			layer->setOneItemIcon(item,pos);
		}
	}
	else if(pos < kFairyStartId)
	{
		int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
		AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag,false);
		if(layer)
		{
			layer->setItemIcon(item,pos);
		}
		
		tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
		BackPackLayer* backLayer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
		if(backLayer)
		{
			backLayer->setOneItemIcon(item,pos);
		}
	}
	else if(pos >= kCardBag && pos <= kItemManorId)
	{
		//CardUILayer* layer = GetCurrentCardUILayer();
		int tag = MainMenuLayer::getLayerTagByName("CardUILayer");
		CardUILayer* layer = MainMenuLayer::getUILayerByTag<CardUILayer>(tag,false);
		if(layer)
		{
			layer->setCardItem(item,pos);
		}
	}
}

void ItemManager::setBackPackDefaultPos(unsigned int ownerPos)
{
	m_defaultPackPos = ownerPos;
	resetBagPacks();
}

void ItemManager::setBackPackUnlockPos(unsigned int pos)
{
	m_unlockPackPos = pos;
	resetBagPacks();
}

void ItemManager::setConstellationCardNum(unsigned int card,unsigned int num)
{
	if(m_constellationCardNum && m_cardEquipList)
	{
		bool needReset = false;
		std::map<unsigned int ,unsigned int>::iterator it = m_constellationCardNum->find(card);
		if(it == m_constellationCardNum->end())
		{
			// 第一次添加

			needReset = true;
			m_constellationCardNum->insert(make_pair(card,num));
		}
		else
		{
			if(num != it->second)
			{
				needReset = true;
				m_constellationCardNum->erase(it);
				m_constellationCardNum->insert(make_pair(card,num));
			}
		}

		if(needReset)
		{
			unsigned int start = (card - 1) * 3;
			for(unsigned int i = start + 1;i <= start + num + 1;++i)
			{
				BackPackItem* item = findItemByPos(i + kCardEquip);
				if(item)
				{
					item->isLocked = false;
					int tag = MainMenuLayer::getLayerTagByName("CardUILayer");
					CardUILayer* layer = MainMenuLayer::showUILayerByTag<CardUILayer>(tag,false);
					if(layer)
					{
						layer->setCardItem(item,i + kCardEquip);
					}
				}
				
			}
		}
	}
}

void ItemManager::resetBagPacks()
{
	unsigned int totalPackPos = m_defaultPackPos + m_unlockPackPos;
	if(m_backPackList)
	{
		for(unsigned int i=1;i<= m_maxPackPos;i++)
		{
			BackPackItem* item = findItemByPos(i);
			if(item)
			{
				if(i <= totalPackPos)
				{
					item->isLocked = false;
				}
				else
				{
					item->isLocked = true;
				}
				item->position = i;
			}
		}
	}
}

void ItemManager::resetStoragePacks()
{
	unsigned int totalPackPos = m_defaultStoragePos + m_unlockStoragePos;
	if(m_storageList)
	{
		for(unsigned int i=1;i<= m_maxStoragePos;i++)
		{
			BackPackItem* item = findItemByPos(i+kItemStorage);
			if(item)
			{
				if(i <= totalPackPos)
				{
					item->isLocked = false;
				}
				else
				{
					item->isLocked = true;
				}
				item->position = i;
			}
		}
	}
}

void ItemManager::setStorageUnlockPos(unsigned int pos)
{
	m_unlockStoragePos = pos;
	resetStoragePacks();
}

void ItemManager::setStorageDefaultPos(unsigned int ownerPos)
{
	m_defaultStoragePos = ownerPos;
	resetStoragePacks();
}

void ItemManager::resetAlchemyBagPacks(unsigned int ownerPos)
{
	if(m_alchemyBagList)
	{
		for(unsigned int i=1;i<= AlCHEMYBAGCAPACITY;i++)
		{
			BackPackItem* item = findItemByPos(i+kAlchemyBag);
			if(item)
			{
				if(i <= ownerPos)
				{
					item->isLocked = false;
				}
				else
				{
					item->isLocked = true;
				}
				item->position = i;
			}
		}
	}
}

void ItemManager::setAlchemyBagUnlockCapacity(unsigned int value)
{
	m_unlockAlchemyBagPos = value;
	unsigned int totalPackPos = m_defaultAlchemyBagPos + m_unlockAlchemyBagPos;
	resetAlchemyBagPacks(totalPackPos);
}

void ItemManager::setAlchemyBackDefaultCapacity(unsigned int pos)
{
	m_defaultAlchemyBagPos = pos;
	unsigned int totalPackPos = m_defaultAlchemyBagPos + m_unlockAlchemyBagPos;
	resetAlchemyBagPacks(totalPackPos);
}

void ItemManager::resetAlchemyStoragePacks(unsigned int ownerPos)
{
	if(m_alchemyStorageList)
	{
		for(unsigned int i=1;i<= AlCHEMYSTORAGECAPACITY;i++)
		{
			BackPackItem* item = findItemByPos(i+kAlchemyStorage);
			if(item)
			{
				if(i <= ownerPos)
				{
					item->isLocked = false;
				}
				else
				{
					item->isLocked = true;
				}
				item->position = i;
			}
		}
	}
}

void ItemManager::setAlchemyStorageDefaultCapacity(unsigned int pos)
{
	m_defaultAlchemyStoragePos = pos;
	unsigned int totalPackPos = m_defaultAlchemyStoragePos + m_unlockAlchemyStroragePos;
	resetAlchemyStoragePacks(totalPackPos);
}

void ItemManager::setAlchemyStorageUnlockCapacity(unsigned int value)
{
	m_unlockAlchemyStroragePos = value;
	unsigned int totalPackPos = m_defaultAlchemyStoragePos + m_unlockAlchemyStroragePos;
	resetAlchemyStoragePacks(totalPackPos);
}

void ItemManager::setItem(CSItemInfo packInfo)
{
	unsigned int realPos = packInfo.pos();
	BackPackItem* existItem = findItemByPos(realPos);
	if(existItem && !existItem->isLocked)
	{
		existItem->setItemId(packInfo.item_id());
		existItem->itemLevel = packInfo.item_level();
		existItem->amount = packInfo.item_num();
		existItem->isEmpty = false;
		existItem->b_isNew = false;
		existItem->resetEquipInfo();
        
		for(std::list<ITEMATTR*>::iterator iter = existItem->m_attribution->begin();
			iter != existItem->m_attribution->end(); ++iter)
		{
			ITEMATTR* attr = *iter;
			if(attr)
			{
				delete attr;
				attr = NULL;
			}
		}
		existItem->m_attribution->clear();

		unsigned int attrCount = packInfo.item_attrs_size();
		for(unsigned int j=0; j<attrCount; j++)
		{
			ITEMATTR* attr = new ITEMATTR();
			CSItemAttr cs_attr = packInfo.item_attrs(j);
			unsigned int temp = cs_attr.attr_basic_value();
			attr->attr_basic_value = temp;
			//attr->attr_basic_value = cs_attr.attr_basic_value();
			attr->attr_key = cs_attr.attr_key();
			if(cs_attr.has_attr_intensify_value())
			{
				attr->attr_intensify_value = cs_attr.attr_intensify_value();
			}
			if(cs_attr.has_attr_gem_value())
			{
				attr->attr_gem_value = cs_attr.attr_gem_value();
			}
			existItem->m_attribution->push_back(attr);
		}

		if(packInfo.has_equip_info())
		{
			existItem->setEquipInfo(packInfo.equip_info());
		}
		if(packInfo.has_ss_info())
		{
			existItem->setSpriteInfo(packInfo.ss_info());
		}

		if(packInfo.has_manor_info())
		{
			existItem->setManorInfo(packInfo.manor_info());
		}

		if(realPos > kFairyFragment && realPos < kItemRecycle)
		{
			// 精灵碎片

		}
		else if(realPos < kAlchemyBag || realPos > kEquipFragmet)
		{
			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
			if(layer)
			{
				layer->setOneItemIcon(existItem,realPos);
			}
		}
		else if(realPos < kCardBag)
		{
			int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
			AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag,false);
			if(layer)
			{
				layer->setItemIcon(existItem,realPos);
			}

			tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* backLayer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
			if(backLayer)
			{
				backLayer->setOneItemIcon(existItem,realPos);
			}
		}
		else
		{
			int tag = MainMenuLayer::getLayerTagByName("CardUILayer");
			CardUILayer* layer = MainMenuLayer::getUILayerByTag<CardUILayer>(tag,false);
			if(layer)
			{
				layer->setCardItem(existItem,realPos);
			}
		}
	}
}

void ItemManager::resetItems()
{
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_equipmentFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentFragList->begin();
			iter != m_equipmentFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_storageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_storageList->begin();
			iter != m_storageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_fairyFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_fairyFragList->begin();
			iter != m_fairyFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_manorItemList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_manorItemList->begin();
			iter != m_manorItemList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_equipmentList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentList->begin();
			iter != m_equipmentList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_equipEnchaseList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipEnchaseList->begin();
			iter != m_equipEnchaseList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_fairyEquipmentList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_fairyEquipmentList->begin();
			iter != m_fairyEquipmentList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}

	if(m_alchemyBagList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyBagList->begin();
			iter != m_alchemyBagList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}

	if(m_alchemyStorageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyStorageList->begin();
			iter != m_alchemyStorageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}

	if(m_alchemyEquipList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyEquipList->begin();
			iter != m_alchemyEquipList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			//item->isLocked = false;
			item->b_isNew = false;
		}
	}

	if(m_alchemyFairyEquipList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyFairyEquipList->begin();
			iter != m_alchemyFairyEquipList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			//item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_gemStoneList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_gemStoneList->begin();
			iter != m_gemStoneList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->amount = 0;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}
	if(m_recycleItemList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_recycleItemList->begin();
			iter != m_recycleItemList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}

	if(m_cardBagList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_cardBagList->begin();
			iter != m_cardBagList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->isLocked = false;
			item->b_isNew = false;
		}
	}

	if(m_cardEquipList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_cardEquipList->begin();
			iter != m_cardEquipList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			item->isEmpty = true;
			item->b_isNew = false;
			std::map<unsigned int ,unsigned int>::iterator it = m_constellationCardNum->find((item->position - 1)/ 3 + 1);
			if(it == m_constellationCardNum->end())
			{
				item->isLocked = (item->position % 3 == 1) ? false : true;
			}
			else
			{
				int index = item->position % 3;
				if(index == 0)
				{
					index = 3;
				}
				if(index <= it->second + 1)
				{
					item->isLocked = false;
				}
				else
				{
					item->isLocked = true;
				}
			}
		}
	}
}

unsigned int ItemManager::getBackPackEmptyGridIndex()
{
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item->isEmpty)
			{
				return item->position;
			}
		}
	}

	return 0;
}

unsigned int ItemManager::getStorageEmptyGridIndex()
{
	if(m_storageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_storageList->begin();
			iter != m_storageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item->isEmpty)
			{
				return item->position;
			}
		}
	}

	return 0;
}

unsigned int ItemManager::getAlchemyBagEmptyGridIndex()
{
	if(m_alchemyStorageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_alchemyStorageList->begin();
			iter != m_alchemyStorageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item->isEmpty)
			{
				return item->position;
			}
		}
	}

	return 0;
}

unsigned int ItemManager::getAlchemyEquipEmptyGridIndex(unsigned int index)
{
	if(index == 0)
	{
		if(m_alchemyEquipList)
		{
			for(BACKPACK_VECTOR::iterator iter = m_alchemyEquipList->begin();
			iter != m_alchemyEquipList->end();iter++)
			{
				BackPackItem *item = (*iter).second;
				if(item->isEmpty && !item->isLocked)
				{
					return item->position;
				}
			}
		}
	}
	else
	{
		if(m_alchemyFairyEquipList)
		{
			for(BACKPACK_VECTOR::iterator iter = m_alchemyFairyEquipList->begin();
			iter != m_alchemyFairyEquipList->end();iter++)
			{
				BackPackItem *item = (*iter).second;
				if(item->isEmpty && !item->isLocked && (item->position / 100 + 1 == index))
				{
					return item->position;
				}
			}
		}
	}

	return 0;
}

void ItemManager::openBackPack(bool withEquipment)
{
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
		layer->openBackPack(withEquipment);
	}
}

void ItemManager::closeEquipment()
{
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
		layer->closeEquipment(NULL);
	}
}

void ItemManager::exchangeItemPos(unsigned int from, unsigned int to)
{
	BackPackItem* itemFrom = findItemByPos(from);
	BackPackItem* itemTo = findItemByPos(to);
	if(itemFrom && itemTo && !itemFrom->isLocked && !itemTo->isLocked)
	{
		unsigned int tempId = itemFrom->getItemId();
		bool tempIsEmpty = itemFrom->isEmpty;
		bool tempIsNew = itemFrom->b_isNew;
		unsigned int tempNumber = itemFrom->amount;
		unsigned int tempLevel = itemFrom->itemLevel;

		std::list<ITEMATTR*>* tempAttr = itemFrom->m_attribution;
		EquipmetInfo* tempEquipInfo = itemFrom->m_equipInfo;
		SPRITEINFO* tempSpriteInfo = itemFrom->m_spriteInfo;

		itemFrom->setItemId(itemTo->getItemId());
		itemFrom->itemLevel = itemTo->itemLevel;
		itemFrom->isEmpty = itemTo->isEmpty;
		itemFrom->b_isNew = itemTo->b_isNew;
		itemFrom->amount = itemTo->amount;
		itemFrom->m_attribution = itemTo->m_attribution;
		itemFrom->m_equipInfo = itemTo->m_equipInfo;
		itemFrom->m_spriteInfo = itemTo->m_spriteInfo;
		if(itemFrom->packType == 2 || itemFrom->packType == 6)
		{
			itemFrom->b_isNew = false;
			NotificationCenter::defaultCenter()->broadcast(GM_ATTR_NEW_EQUIP_CHANGE,this);
		}
		else
		{
			itemFrom->b_isNew = itemTo->b_isNew;
			if(itemFrom->m_equipInfo)
			{
				itemFrom->m_equipInfo->m_suitAttribution.clear();
			}
		}

		itemTo->setItemId(tempId);
		itemTo->itemLevel = tempLevel;
		itemTo->isEmpty = tempIsEmpty;
		itemTo->amount = tempNumber;
		itemTo->m_attribution = tempAttr;
		itemTo->m_equipInfo = tempEquipInfo;
		itemTo->m_spriteInfo = tempSpriteInfo;
		if(itemTo->packType == 2 || itemTo->packType == 6)
		{
			itemTo->b_isNew = false;
			NotificationCenter::defaultCenter()->broadcast(GM_ATTR_NEW_EQUIP_CHANGE,this);
		}
		else
		{
			itemTo->b_isNew = tempIsNew;
			if(itemTo->m_equipInfo)
			{
				itemTo->m_equipInfo->m_suitAttribution.clear();
			}
		}

		if(from >= kEquipFragmet && to >= kEquipFragmet)
		{
			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
			if(layer)
			{
				layer->setOneItemIcon(itemFrom,from);
				layer->setOneItemIcon(itemTo,to);
			}
		}
		else if(from < kAlchemyBag && to < kAlchemyBag)
		{
			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
			if(layer)
			{
				layer->setOneItemIcon(itemFrom,from);
				layer->setOneItemIcon(itemTo,to);
			}
		}
		else if(from >= kCardBag && to >= kCardBag)
		{
			int tag = MainMenuLayer::getLayerTagByName("CardUILayer");
			CardUILayer* layer = MainMenuLayer::getUILayerByTag<CardUILayer>(tag,false);
			if(layer)
			{
				layer->setCardItem(itemFrom,from);
				layer->setCardItem(itemTo,to);
			}
		}
		else if(from >= kAlchemyBag && to >= kAlchemyBag)
		{
			int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
			AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag,false);
			if(layer)
			{
				layer->setItemIcon(itemFrom,from);
				layer->setItemIcon(itemTo,to);
			}

			tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* backPackLayer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
			if(backPackLayer)
			{
				backPackLayer->setOneItemIcon(itemFrom,from);
				backPackLayer->setOneItemIcon(itemTo,to);
			}
		}
		else
		{
			CCLOG("error exchange  from %d to %d !",from,to);
		}
	}
	else
	{
		CCLOG("exchange item pos error!");
	}
}

bool ItemManager::setEquipItemInfor(unsigned int pos,EquipmentBase equipmetInfo)
{
	BackPackItem* item = findItemByPos(pos);
	if(!item || item->isEmpty || item->isLocked)
	{
		CCLOG("Error Equip item: %d ",pos);
		return false;
	}
	if(!item->m_equipInfo)
	{
		item->m_equipInfo = new EquipmetInfo();
	}
	item->m_equipInfo->levelUpCost = equipmetInfo.cost_levelup();
	item->m_equipInfo->m_nextAttribution.clear();
	item->m_equipInfo->m_suitAttribution.clear();
	unsigned int count = equipmetInfo.incr_attr_next_lv_size();
	for(unsigned int i = 0; i< count; i++)
	{
		EquipmentBase_Item_Attr attr = equipmetInfo.incr_attr_next_lv(i);
		EQUIPNEXTATTR nextAttr;
		nextAttr.key = attr.key();
		nextAttr.value = attr.value();
		item->m_equipInfo->m_nextAttribution.push_back(nextAttr);
	}
	count = equipmetInfo.rel_ids_size();
	for(unsigned int i = 0; i<count; i++)
	{
		item->m_equipInfo->m_suitAttribution.push_back(equipmetInfo.rel_ids(i));
	}
	return true;
}

void ItemManager::loadExchangeItems()
{
	if(!b_isLoadExchangeItems)
	{
		GameConfigFromLuaManager::Get()->getExchangeItems();
		b_isLoadExchangeItems = true;
	}
}

void ItemManager::addExchangeItem(unsigned int exchangeId,string fromItems, string toItems, unsigned int type,unsigned int requiredLevel)
{
	ExchangeItem* item = new ExchangeItem();
	item->setFromItems(fromItems);
	item->setToItems(toItems);
	item->m_exchangeType = type;
	item->m_exchangeId = exchangeId;
	item->m_requiredLevel = requiredLevel;
	m_exchangeItemList->push_back(item);
}

std::list<ExchangeItem*>* ItemManager::getExchangeItemsByType(unsigned int type)
{
	loadExchangeItems();
	std::list<ExchangeItem*>* exchangeList = new std::list<ExchangeItem*>;
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator it = m_exchangeItemList->begin();
			it != m_exchangeItemList->end(); it++)
		{
			ExchangeItem *item = *it;
			if(item && item->m_exchangeType == type)
			{
				exchangeList->push_back(item);	
			}
		}
	}
	return exchangeList;
}

list<unsigned int>* ItemManager::getExchangeSpriteStones(int exchangeType)
{
	loadExchangeItems();
	list<unsigned int>* spriteList = new list<unsigned int>();
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator it = m_exchangeItemList->begin();
			it != m_exchangeItemList->end(); it++)
		{
			ExchangeItem *item = *it;
			if(item && item->m_exchangeType == exchangeType)
			{
				spriteList->push_back(item->m_exchangeId);	
			}
		}
	}
	return spriteList;
}

ExchangeItem* ItemManager::getExchageItemById(unsigned int exchangeId)
{
	loadExchangeItems();
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator it = m_exchangeItemList->begin();
			it != m_exchangeItemList->end(); it++)
		{
			ExchangeItem *item = *it;
			if(item->m_exchangeId == exchangeId)
			{
				return item;
			}
		}
	}
	return NULL;
}

ExchangeItem* ItemManager::getExchageItemByIdWithType(unsigned int itemID,unsigned int type)
{
	loadExchangeItems();
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator it = m_exchangeItemList->begin();
			it != m_exchangeItemList->end(); it++)
		{
			ExchangeItem *item = *it;
			if(item)
			{
				if(item->m_fromItems && (item->m_exchangeType == type || type == 0))
				{
					for (std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_fromItems->begin();
						iter != item->m_fromItems->end();iter++)
					{
						if((*iter).first == itemID)
						{
							return item;
						}
					}	
				}
			}
		}
	}
	return NULL;
}

/*bool ItemManager::checkEquipDrawingUseful(unsigned int drawingId)
{
	bool isUseFlag = true;
	loadExchangeItems();
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator it = m_exchangeItemList->begin();
			it != m_exchangeItemList->end(); it++)
		{
			ExchangeItem *item = *it;
			if(item && item->m_exchangeId == drawingId)
			{
				unsigned int equipId = getEquipIdInDrawing(item);
				ItemBase* info = ItemManager::Get()->getItemInfoById(equipId);
				EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
				if(equipInfo)
				{
					BackPackItem* backItem = getEquipmentByPos(equipInfo->m_ownerType,equipInfo->m_equipPart);
					if(backItem &&  backItem->getItemInfo())
					{
						 if(backItem->getItemInfo()->m_quality >= equipInfo->m_quality)
						 {
							 isUseFlag = false;		 
						 }
					}
				}
				break;
			}
		}
	}
	return isUseFlag;
}*/

unsigned int ItemManager::getEquipIdInDrawing(ExchangeItem* item)
{
	unsigned int equipId = 0;
	if(item)
	{
		for(std::list<pair<unsigned int ,unsigned int> >::iterator iter = item->m_toItems->begin();
			iter != item->m_toItems->end();iter++)
		{
			unsigned int matarialId = (*iter).first;
			ItemBase* info = ItemManager::Get()->getItemInfoById(matarialId);
			EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
			if(equipInfo)
			{
				equipId = equipInfo->m_id;
				//CC_SAFE_DELETE(info);
				break;
			}
		//	CC_SAFE_DELETE(info);
		}

	}
	return equipId;
}

/*BackPackItem* ItemManager::getEquipmentByPos(unsigned int owner,unsigned int part)
{
	BackPackItem* equipItem = NULL;
	if (owner == 0)
	{
		if(m_equipmentList)
		{
			for(BACKPACK_VECTOR::iterator iter = m_equipmentList->begin();
				iter != m_equipmentList->end();iter++)
			{
				BackPackItem *item = (*iter).second;
				if(item)
				{
					ItemBase* info = item->getItemInfo();
					EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
					if(equipInfo)
					{
						if(equipInfo->m_equipPart == part && equipInfo->m_ownerType == owner)
						{
							equipItem = item;
							break; 
						}
					}
				}
			}
		}
	}
	else if( owner == 1)
	{
		if(m_fairyEquipmentList)
		{
			for(BACKPACK_VECTOR::iterator iter = m_fairyEquipmentList->begin();
				iter != m_fairyEquipmentList->end();iter++)
			{
				BackPackItem *item = (*iter).second;
				if(item)
				{
					ItemBase* info = item->getItemInfo();
					EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
					if(equipInfo)
					{
						if(equipInfo->m_equipPart == part && equipInfo->m_ownerType == owner)
						{
							equipItem = item;
							break; 
						}
					}
				}
			}
		}
	}
	
	return equipItem;
}*/

ExchangeItem* ItemManager::getEquipDrawingById(unsigned int equipId)
{
	loadExchangeItems();
	if(m_exchangeItemList)
	{
		for(std::list<ExchangeItem*>::iterator it = m_exchangeItemList->begin();
			it != m_exchangeItemList->end(); it++)
		{
			ExchangeItem *item = *it;
			if(item)
			{
				if(item->m_fromItems)
				{
					for (std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_fromItems->begin();
					iter != item->m_fromItems->end();iter++)
					{
						if((*iter).first == equipId)
						{
							return item;
						}
					}	
				}
			}
		}
	}

	return NULL;
}

CCSprite* ItemManager::getIconSpriteById(unsigned int ItemId)
{
	ItemBase* info = getItemInfoById(ItemId);
	int index = 0;
	int quality = 0;
	bool isEquipSuit = false;
	if(info)
	{ 
		quality = info->m_quality;
		index = GameConfigFromLuaManager::Get()->getItemResourceId(info->m_iconId);
		if(dynamic_cast<EquipmentItem*>(info))
		{
			isEquipSuit = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/EquipConfigTable.lua","IsSuitEquip",ItemId);
		}
	}
	else
	{
		string iconName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyIconNameByID",ItemId);
		if(iconName != "")
		{
			index = TXGUI::getResourceIDByName(iconName.c_str());
		}
		else
		{
			index = GameConfigFromLuaManager::Get()->getItemResourceId(0);
		}
	}
	//CC_SAFE_DELETE(info);
	ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN);
	CCPoint pt;
	CCSprite * pic = as->getSpriteFromFrame_Middle( index, 0, pt);
	if(quality != 0)
	{
		CCSprite* framePic = getIconFrame(quality);
		if(framePic)
		{
			pic->addChild(framePic,10);
			framePic->setPosition(ccp(pic->getContentSize().width * 0.5f,pic->getContentSize().height * 0.5f));
		}
	}
	if(isEquipSuit)
	{
		//SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(445);
		EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(445,"",false);
        if(effect1)
        {
            pic->addChild(effect1,20);
			effect1->setPosition(ccp(pic->getContentSize().width * 0.5f,pic->getContentSize().height * 0.5f));
            effect1->SetAnim(kType_Play,1,true);
        }
	}
	return pic;
	
}

ItemBase* ItemManager::getItemInfoById(unsigned int id)
{
	ItemBase* info = NULL;
	EquipmentItem* equipmentInfo = new EquipmentItem();
	if(GameConfigFromLuaManager::Get()->getEquipmentItem(id,equipmentInfo))
	{
		info = equipmentInfo;
	}
	else
	{
		CC_SAFE_DELETE(equipmentInfo);
		ItemBase* itemInfo = new ItemBase();
		if(GameConfigFromLuaManager::Get()->getItem(id,itemInfo))
		{
			info = itemInfo;
			
		}
		else
		{
			CC_SAFE_DELETE(itemInfo);
		}
	}
	if(info)
		info->autorelease();
	return info;
}

unsigned int ItemManager::getItemNumberInOnlyInBag(unsigned int id)
{
	unsigned int sum = 0;
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked && item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_equipmentFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentFragList->begin();
			iter != m_equipmentFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked && item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_fairyFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_fairyFragList->begin();
			iter != m_fairyFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_gemStoneList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_gemStoneList->begin();
			iter != m_gemStoneList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	return sum;
}

unsigned int ItemManager::getItemNumberById(unsigned int id)
{
	unsigned int sum = 0;
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked && item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_equipmentFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentFragList->begin();
			iter != m_equipmentFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked && item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_storageList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_storageList->begin();
			iter != m_storageList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_fairyFragList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_fairyFragList->begin();
			iter != m_fairyFragList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_equipmentList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_equipmentList->begin();
			iter != m_equipmentList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_fairyEquipmentList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_fairyEquipmentList->begin();
			iter != m_fairyEquipmentList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_gemStoneList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_gemStoneList->begin();
			iter != m_gemStoneList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	if(m_cardBagList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_cardBagList->begin();
			iter != m_cardBagList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && !item->isEmpty && !item->isLocked &&  item->getItemId() == id)
			{
				sum += item->amount;
			}
		}
	}
	return sum;
}

string ItemManager::getItemButtonNameWithAjust(string turButtonName, bool ajust)
{
	if (turButtonName.find("bagItem_") != string::npos)
	{
		int itemId = 0;
		string idStr = turButtonName.substr(string("bagItem_").size(), string::npos);
		stringstream ss;
		ss << idStr;
		ss >> itemId;

		int itemIndex = 0;
		if(m_backPackList)
		{
			for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
				iter != m_backPackList->end();iter++)
			{
				BackPackItem *item = (*iter).second;
				if(item && item->getItemId() == itemId)
				{
					itemIndex = (*iter).first;
					break;
				}
			}
		}

		if (ajust)
		{
			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
			if(layer)
			{
				layer->ajustScrollPageByIndex("backPackScrollPage", itemIndex);
			}
		}

		ostringstream oss;
		oss << "backPackScrollPage_" << itemIndex;
		return oss.str();
	}


}

string ItemManager::getTurEventNameByButtonName(string buttonName)
{
	string turEventName = buttonName;
	if (buttonName.find("backPackScrollPage_") != string::npos)
	{
		int buttonIndex = 0;
		string indexStr = buttonName.substr(string("backPackScrollPage_").size(), string::npos);
		stringstream ss;
		ss << indexStr;
		ss >> buttonIndex;

		int itemId = 0;
		if(m_backPackList)
		{
			for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
				iter != m_backPackList->end();iter++)
			{
				if (buttonIndex == (*iter).first)
				{
					BackPackItem *item = (*iter).second;
					itemId = item->getItemId();
					break;
				}
			}
		}

		ostringstream oss;
		oss << "bagItem_" << itemId;
		turEventName = oss.str();
	}
	return turEventName;
}

void ItemManager::checkBagEmptyItemNum()
{
	unsigned int num = getBagEmptyNum();
	if(num <= 4)
	{
		const char* str = Localizatioin::getLocalization("M_BAGFULL_CLEAN");
		if(str != NULL)
		{
			ShowMessage(str);
		}
	}
}

unsigned int ItemManager::getBagEmptyNum()
{
	unsigned int emptyNum = 0;
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item && item->isEmpty && !item->isLocked)
			{
				emptyNum++;
			}
		}
	}
	return emptyNum;
}

void ItemManager::reqAlchemyUpgradeSelected(unsigned int pos)
{
	int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
	AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag);
	if(layer)
	{
		layer->selectUpgradeItem(pos);
	}
}

void ItemManager::reqAlchemyUpgradeUnselected()
{
	int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
	AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag);
	if(layer)
	{
		layer->unselectUpgradeItem();
	}
}

int ItemManager::getCurrentEquipmentIndex()
{
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
		return layer->getCurrentEquipPage();
	}
	return 0;
}

int ItemManager::getCurrentAlchemyIndex()
{
	int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
	AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag);
	if(layer)
	{
		return layer->getCurrentAlchemyIndex();
	}
	return 0;
}

unsigned int ItemManager::getAlchemySearchNpcCost(unsigned int npcID)
{
	if(!m_searchNpcCostMap)
	{
		m_searchNpcCostMap = new std::map<unsigned int ,unsigned int>();
	}

	map<unsigned int ,unsigned int>::iterator it = m_searchNpcCostMap->find(npcID);
	if(it != m_searchNpcCostMap->end())
	{
		return it->second;
	}
	else
	{
		int res = GameConfigFromLuaManager::Get()->getSearchSpiritNpcCost(npcID);

		CCAssert(res >= 0,"Alchemy Search Spirit Data Error");
		m_searchNpcCostMap->insert(make_pair(npcID,res));

		return res;
	}

}

ccColor3B ItemManager::getLabelColorByQuality(unsigned int quality)
{
	ccColor3B color;
	switch(quality)
	{
	
	case  1:
		color =  KQualityGreenColor;
		break;
	case  2:
		color =  KQualityBlueColor;
		break;
	case  3:
		color =  KQualityPurpleColor;
		break;
	case  4:
		color =  ccYELLOW;
		break;
	case  5:
		color =  KQualityOrangeColor;
		break;
	default:
		color =  KQualityWhiteColor;
		break;
	}
	return color;
}

ccColor3B ItemManager::getFairyLabelColorByQuality(unsigned int quality)
{
	ccColor3B color;
	switch(quality)
	{
	
	case  1:
		color =  KQualityWhiteColor;
		break;
	case  2:
		color =  KQualityGreenColor;
		break;
	case  3:
		color =  KQualityBlueColor;
		break;
	case  4:
		color =  KQualityPurpleColor;
		break;
	default:
		color =  KQualityPurpleColor;
		break;
	}
	return color;
}

CCSprite* ItemManager::getIconFrame(unsigned int level)
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
		picName = "map_ui_system_icon_FRAME_FRAME_YELLOW";
		break;
	case 5:
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

unsigned int ItemManager::getHeroWeaponId()
{
	BackPackItem* backItem = findItemByPos(kItemHeroEquip + 3);
	if(backItem && !backItem->isEmpty)
	{
		return backItem->getItemId();
	}
	else
	{
		return 0;
	}
}

unsigned int ItemManager::getHeroDefautWeaponId(unsigned int heroTypeId)
{
	// Note: 战士
	if (heroTypeId == 1 || heroTypeId == 2)
	{
		return 400600;
	}
	// Note: 法师
	else if (heroTypeId == 3 || heroTypeId == 4)
	{
		return 400700;
	}
	// Note: 刺客
	else if (heroTypeId == 5 || heroTypeId == 6)
	{
		return 400800;
	}
}

void ItemManager::setTipDelegate(cocos2d::CCObject*	leftTarget,cocos2d::SEL_MenuHandler leftHandler,const char* leftText,bool isLeftVisible,
	cocos2d::CCObject*	rightTarget,cocos2d::SEL_MenuHandler rightHandler,const char* rightText,bool isRithtVisible)
{
	TipMgr::Get()->setOKButton(leftTarget,leftHandler,leftText,isLeftVisible);
	TipMgr::Get()->setCancellButton(rightTarget,rightHandler,rightText,isRithtVisible);
}

void ItemManager::setTipMiddleDelegate(cocos2d::CCObject*	leftTarget,cocos2d::SEL_MenuHandler leftHandler,const char* leftText,bool isLeftVisible)
{
	TipMgr::Get()->setMiddleButton(leftTarget,leftHandler,leftText,isLeftVisible);
}

void ItemManager::setTipLuaHander(int leftHandler,const char* leftText,bool isLeftVisible,
	int rightHandler,const char* rightText,bool isRightVisible)
{
	TipMgr::Get()->setOKLuaHandle(leftHandler,leftText,isLeftVisible);
	TipMgr::Get()->setCancellLuaHandle(rightHandler,rightText,isRightVisible);
}

void ItemManager::setMiddleLuaHander(int leftHandler,const char* leftText,bool isLeftVisible)
{
	TipMgr::Get()->setMiddleLuaHandle(leftHandler,leftText,isLeftVisible);
}

void ItemManager::showItemTipsByPos(unsigned int pos,CCPoint pt)
{
	BackPackItem* item = ItemManager::Get()->findItemByPos(pos);
	if(item && !item->isEmpty && !item->isLocked)
	{
		CCSprite* icon = getIconSpriteById(item->getItemId());
		if(icon)
		{
			ItemBase* itemInfo = item->getItemInfo();
			if(itemInfo)
			{
				if(itemInfo->m_quality > 0)
				{
					CCSprite* framePic = getIconFrame(itemInfo->m_quality);
					if(framePic)
					{
						icon->addChild(framePic);
						framePic->setPosition(ccp(icon->getContentSize().width * 0.5f,icon->getContentSize().height * 0.5f));
					}
				}
				EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(itemInfo);
				if(equipInfo)
				{
					//showEquipmentTipByPos(item,icon,pt);
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/TipManager.lua","showEquipmentTipByPos",
							pos,pt.x,pt.y);
					return;
				}
				switch(itemInfo->m_type)
				{
				case 2:
					showEquipDrawingTipByPos(item,icon,pt);
					break;
				case 3:
					showUnusedItemTipByPos(item,icon,pt);
					break;
				case 7:
					showAlchemyItemTipByPos(item,icon,pt);
					break;
				case 10:
					showCardItemTipByPos(item,icon,pt);
					break;
				case 19:
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/TipManager.lua","showEquipFragTipByPos",
							pos,pt.x,pt.y);
					break;
				default:
					showusedItemTipByPos(item,icon,pt);
					break;
				}
			}
		}

	}
}

void ItemManager::showItemTipsById(unsigned int itemId,CCPoint pt)
{
	ItemBase* itemInfo = getItemInfoById(itemId);
	if(itemInfo)
	{
		CCSprite* icon = getIconSpriteById(itemId);
		if(icon)
		{
			if(itemInfo->m_quality > 0)
			{
				CCSprite* framePic = getIconFrame(itemInfo->m_quality);
				if(framePic)
				{
					icon->addChild(framePic);
					framePic->setPosition(ccp(icon->getContentSize().width * 0.5f,icon->getContentSize().height * 0.5f));
				}
			}
			EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(itemInfo);
			if(equipInfo)
			{
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/TipManager.lua","showEquipmentTipById",
							equipInfo->m_id,pt.x,pt.y);
				return;
			}
			else
			{
				switch(itemInfo->m_type)
				{
				case 2:
					showEquipDrawingTipByItemid(itemInfo,icon,pt);
					break;
				case 10:
					showCardTipByItemid(itemInfo,icon,pt);
					break;
				case 19:
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/TipManager.lua","showEquipFragTipById",
							itemId,pt.x,pt.y);
					break;
				default:
					showNormalItemTipByItemid(itemInfo,icon,pt);
					break;
				}
			}
		}
	}
	else
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Equipment/TipManager.lua","showTipNotItem",
							itemId,pt.x,pt.y);
	}
}

void ItemManager::showEquipmentTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt)
{
	if(item && !item->isEmpty && !item->isLocked)
	{
		ItemBase* itemInfo = item->getItemInfo();
		EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(itemInfo);
		if(equipInfo)
		{
			TipMgr::Get()->createTip(icon,pt);
			TipMgr::Get()->add(equipInfo->m_name,getLabelColorByQuality(equipInfo->m_quality),22);
			char order[30];
			//sprintf(order,"%s%u",Localizatioin::getLocalization("M_EQUIPMENT_UPGRADELEVEL"),equipInfo->m_order);
			sprintf(order,"%s%u","order",equipInfo->m_order);
			TipMgr::Get()->add(order,getLabelColorByQuality(equipInfo->m_quality),22);
			//TipMgr::Get()->add("",ccWHITE ,20);
			char levelStr[30];
			sprintf(levelStr,"%s%d",Localizatioin::getLocalization("M_EQUIPMENT_UPGRADELEVEL"),item->itemLevel);
			TipMgr::Get()->add(levelStr,ccORANGE ,20);
			if(item->m_attribution)
			{
				for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
					it != item->m_attribution->end(); ++it)
				{
					ITEMATTR* attr = *it;
					if(attr->attr_basic_value != 0)
					{
						const char* text = getAttributionText(attr->attr_key,(attr->attr_basic_value+attr->attr_intensify_value));
						if(text)
						{
							TipMgr::Get()->add(text,ccORANGE ,20);
						}
					}	
				}
			}
			TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_WEARREQUIREMENT"),ccWHITE ,20);
			char levelRequiredStr[30];
			sprintf(levelRequiredStr,"%s%d",Localizatioin::getLocalization("M_EQUIPMENT_ROLELEVEL"),equipInfo->m_requiredLevel);
			TipMgr::Get()->add(levelRequiredStr,ccWHITE ,20);
			char typeRequiredStr[40];
			if(equipInfo->m_type != 0)
			{
				sprintf(typeRequiredStr,"%s%s",Localizatioin::getLocalization("M_EQUIPMENT_OCCUPATION"),LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", equipInfo->m_type, "Name"));
			}
			else
			{
				sprintf(typeRequiredStr,"%s%s",Localizatioin::getLocalization("M_EQUIPMENT_OCCUPATION"),Localizatioin::getLocalization("M_ALLOCCUPTATION"));
			}
			TipMgr::Get()->add(typeRequiredStr,ccWHITE ,20);
			TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccORANGE ,20);
			//TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_METARIALNEED"),ccWHITE ,20);
			/*char priceStr[100];
			sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),equipInfo->m_sellprice);
			TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
			TipMgr::Get()->addNodeDone();
		}
	}
}

void ItemManager::showEquipDrawingTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt)
{
	if(item && !item->isEmpty && !item->isLocked)
	{
		ItemBase* itemInfo = item->getItemInfo();
		if(itemInfo && itemInfo->m_type == 2)
		{
			ExchangeItem* exchangeItem = getEquipDrawingById(itemInfo->m_id);
			if(exchangeItem)
			{
				TipMgr::Get()->createTip(icon,pt);
				TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
				//TipMgr::Get()->add(Localizatioin::getLocalization("M_DOUBLECLICK"),ccWHITE ,20);
				TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccORANGE ,20);
				TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_METARIALNEED"),ccWHITE ,20);
				for(std::list<pair<unsigned int ,unsigned int> >::iterator iter = exchangeItem->m_fromItems->begin();
					iter != exchangeItem->m_fromItems->end();iter++)
				{
					ccColor3B color;
					unsigned int matarialId = (*iter).first;
					unsigned int ownerNum = getItemNumberById(matarialId);
					if(ownerNum >= (*iter).second)
					{
						ownerNum = (*iter).second;
						color = ccGREEN;
					}
					else
					{
						color = ccRED;
					}
					ItemBase* info = getItemInfoById(matarialId);
					if(info)
					{
						char numStr[50];
						sprintf(numStr,"%s%d/%d",info->m_name,ownerNum,(*iter).second);
						TipMgr::Get()->add(numStr,color ,20);
					}
					
				}
				/*char priceStr[100];
				sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
				TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
				TipMgr::Get()->addNodeDone();
			}
			
		}
	}
}

void ItemManager::showUnusedItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt)
{
	if(item && !item->isEmpty && !item->isLocked)
	{
		ItemBase* itemInfo = item->getItemInfo();
		if(itemInfo)
		{
			TipMgr::Get()->createTip(icon,pt);
			TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
			//TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccWHITE ,20);
			TipMgr::Get()->add(itemInfo->m_szDescription,ccWHITE ,20);
			/*char priceStr[100];
			sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
			TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
			TipMgr::Get()->addNodeDone();
		}
	}
}

void ItemManager::showusedItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt)
{
	if(item && !item->isEmpty && !item->isLocked)
	{
		ItemBase* itemInfo = item->getItemInfo();
		if(itemInfo)
		{
			TipMgr::Get()->createTip(icon,pt);
			TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
			//TipMgr::Get()->add(Localizatioin::getLocalization("M_DOUBLECLICK"),ccWHITE ,20);
			//TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccWHITE ,20);
			TipMgr::Get()->add(itemInfo->m_szDescription,ccWHITE ,20);
			/*char priceStr[100];
			sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
			TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
			TipMgr::Get()->addNodeDone();
		}
	}
}

void ItemManager::showCardTipByItemid(ItemBase* itemInfo,CCSprite* icon,CCPoint pt)
{
	TipMgr::Get()->createTip(icon,pt);
	TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
	char levelStr[50];
	sprintf(levelStr,"%s%d",Localizatioin::getLocalization("M_EQUIPMENT_LEVEL"),CardMixLayer::getCardLevel(itemInfo->m_id));
	TipMgr::Get()->add(levelStr,ccORANGE ,20);
	unsigned int key = CardMixLayer::getCardAttrKey(itemInfo->m_id);
	unsigned int num = CardMixLayer::getCardAttrNum(itemInfo->m_id,key);
	const char* text = CardMixLayer::getCardAttrText(key,num);
	if(text)
	{
		TipMgr::Get()->add(text,ccORANGE ,20);
	}

	TipMgr::Get()->add(itemInfo->m_szDescription,ccWHITE ,20);
	/*char priceStr[20];
	sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
	TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
	TipMgr::Get()->addNodeDone();
}

void ItemManager::showEquipDrawingTipByItemid(ItemBase* itemInfo,CCSprite* icon,CCPoint pt)
{
	ExchangeItem* exchangeItem = getEquipDrawingById(itemInfo->m_id);
	if(exchangeItem)
	{
		TipMgr::Get()->createTip(icon,pt);
		TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
		//TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccORANGE ,20);
		TipMgr::Get()->add(itemInfo->m_szDescription,ccWHITE ,20);
		TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_METARIALNEED"),ccWHITE ,20);
		for(std::list<pair<unsigned int ,unsigned int> >::iterator iter = exchangeItem->m_fromItems->begin();
			iter != exchangeItem->m_fromItems->end();iter++)
		{
			ccColor3B color;
			unsigned int matarialId = (*iter).first;
			unsigned int ownerNum = getItemNumberById(matarialId);
			if(ownerNum >= (*iter).second)
			{
				ownerNum = (*iter).second;
				color = ccGREEN;
			}
			else
			{
				color = ccRED;
			}
			ItemBase* info = getItemInfoById(matarialId);
			if(info)
			{
				char numStr[50];
				sprintf(numStr,"%s%d/%d",info->m_name,ownerNum,(*iter).second);
				TipMgr::Get()->add(numStr,color ,20);
			}

		}
		/*char priceStr[100];
		sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
		TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
		TipMgr::Get()->addNodeDone();
	}
}

void ItemManager::showNormalItemTipByItemid(ItemBase* itemInfo,CCSprite* icon,CCPoint pt)
{
	TipMgr::Get()->createTip(icon,pt);
	TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
	//TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccWHITE ,20);
	TipMgr::Get()->add(itemInfo->m_szDescription,ccWHITE ,20);

	char priceStr[100];
	sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
	TipMgr::Get()->add(priceStr,ccYELLOW ,20);
	TipMgr::Get()->addNodeDone();
}

void ItemManager::showAlchemyItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt)
{
	if(item && !item->isEmpty && !item->isLocked)
	{
		ItemBase* itemInfo = item->getItemInfo();
		if(itemInfo)
		{
			TipMgr::Get()->createTip(icon,pt);
			TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
			char levelStr[50];
			sprintf(levelStr,"%s%d",Localizatioin::getLocalization("M_EQUIPMENT_LEVEL"),item->itemLevel);
			TipMgr::Get()->add(levelStr,ccORANGE ,20);
			if(item->m_attribution)
			{
				for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
					it != item->m_attribution->end(); ++it)
				{
					ITEMATTR* attr = *it;
					const char* text = getAttributionText(attr->attr_key,(attr->attr_basic_value+attr->attr_intensify_value));
                    if(text)
                    {
                        TipMgr::Get()->add(text,ccORANGE ,20);
                    }
				}
			}
			TipMgr::Get()->add(itemInfo->m_szDescription,ccWHITE ,20);
			if(item->m_spriteInfo)
			{
				char priceStr[50];
				if(item->m_spriteInfo != 0)
				{
					sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_ELITE_NEXTLEVEL"),item->m_spriteInfo->levelUpCost);
				}
				else
				{
					sprintf(priceStr,"%s",Localizatioin::getLocalization("M_REACH_HIGHESTLEVEL"));
				}
				TipMgr::Get()->add(priceStr,ccGREEN ,20);
				
			}
			TipMgr::Get()->addNodeDone();
		}
	}
}

void ItemManager::showCardItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt)
{
	ItemBase* itemInfo = item->getItemInfo();
	if(item && !item->isEmpty && !item->isLocked)
	{
		if(itemInfo)
		{
			TipMgr::Get()->createTip(icon,pt);
			TipMgr::Get()->add(itemInfo->m_name,getLabelColorByQuality(itemInfo->m_quality),22);
			char levelStr[50];
			sprintf(levelStr,"%s%d",Localizatioin::getLocalization("M_EQUIPMENT_LEVEL"),item->itemLevel);
			TipMgr::Get()->add(levelStr,ccORANGE ,20);
			if(item->m_attribution)
			{
				for(std::list<ITEMATTR*>::iterator it = item->m_attribution->begin();
					it != item->m_attribution->end(); ++it)
				{
					ITEMATTR* attr = *it;
					const char* text = getAttributionText(attr->attr_key,(attr->attr_basic_value+attr->attr_intensify_value));
					if(text)
					{
						TipMgr::Get()->add(text,ccORANGE ,20);
					}
				}
			}
			TipMgr::Get()->add(Localizatioin::getLocalization("M_EQUIPMENT_DESCRIPTION"),ccWHITE ,20);
			/*char priceStr[100];
			sprintf(priceStr,"%s%d",Localizatioin::getLocalization("M_PRICE"),itemInfo->m_sellprice);
			TipMgr::Get()->add(priceStr,ccYELLOW ,20);*/
			TipMgr::Get()->addNodeDone();
		}
	}
}

const char* ItemManager::getAttributionKeyText(unsigned int key)
{
	const char* str;
	switch(key)
	{
	case PB_ITEM_ATTR_PHYATK:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_PHYATTACK");
		break;
	case PB_ITEM_ATTR_MAGATK:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_MAGATTACK");
		break;
	case PB_ITEM_ATTR_SKIATK:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_SKIATTACK");
		break;
	case PB_ITEM_ATTR_PHYDEF:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_PHYDEFENCE");
		break;
	case PB_ITEM_ATTR_MAGDEF:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_MAGDEFENCE");
		break;
	case PB_ITEM_ATTR_SKIDEF:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_SKIDEFENCE");
		break;
	case PB_ITEM_ATTR_HP:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_LIFE");
		break;
	case PB_ITEM_ATTR_ACCU:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_ACCURATE");
		break;
	case PB_ITEM_ATTR_DODG:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_DODGE");
		break;
	case PB_ITEM_ATTR_WRK:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_WRECK");
		break;
	case PB_ITEM_ATTR_PARRY:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_BLOCK");
		break;
	case PB_ITEM_ATTR_CRIT:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_CRITICAL");
		break;
	case PB_ITEM_ATTR_TENA:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_TENACITY");
		break;
	case PB_ITEM_ATTR_SLY:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_SLAY");
		break;
	case PB_ITEM_ATTR_SPEED:
		str = Localizatioin::getLocalization("M_ATTRIBUTE_SPEED");
		break;
	default:
		str = "";
		break;
	}
	return str;
}

const char* ItemManager::getAttributionText(unsigned int key,unsigned int value)
{
	const char* str = getAttributionKeyText(key);
	if(str != NULL)
	{
		//char* text = new char[50];
		char text[50];
		sprintf(text,"%s+%d",str,value);
		CCString* pRet = CCString::create(text);
		return pRet->m_sString.c_str();
	}
	return NULL;
}

void ItemManager::setEquipMaxTimes(unsigned int maxTimes)
{
	USERINFO info = UserData::GetUserInfo();
	if(maxTimes != info.m_equipLvUpMaxTimes)
	{
		info.m_equipLvUpMaxTimes = maxTimes;
		UserData::SetUserInfo(info);
		NotificationCenter::defaultCenter()->broadcast(GM_ATTR_EQUIP_LVUP_CD,this);
	}
}

void ItemManager::clearItems()
{
	resetItems();
	m_defaultPackPos = 15;
	m_unlockPackPos  = 0;
	m_defaultStoragePos = 15;
	m_unlockStoragePos = 0;
	m_defaultAlchemyBagPos = AlCHEMYBAGCAPACITY;
	m_unlockAlchemyBagPos = 0;
	m_defaultAlchemyStoragePos = 10;
	m_unlockAlchemyStroragePos = 0;
	m_surplusGemRecastNum = 0;
}


void ItemManager::setEquipNewNote(unsigned int pos,bool isNew)
{
	BackPackItem* item = findItemByPos(pos);
	if(item)
	{
		bool isChanged = (item->b_isNew != isNew);
		item->b_isNew = isNew;
		if(pos < kItemStorage)
		{
			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
			BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag,false);
			if(layer)
			{
				layer->setOneItemIcon(item,pos);
				
			}
		}
		if(isChanged)
		{
			NotificationCenter::defaultCenter()->broadcast(GM_ATTR_NEW_EQUIP_CHANGE,this);
		}
	}
}

bool ItemManager::checkNewEquipInfo()
{
	bool ret = false;
	if(m_backPackList)
	{
		for(BACKPACK_VECTOR::iterator iter = m_backPackList->begin();
			iter != m_backPackList->end();iter++)
		{
			BackPackItem *item = (*iter).second;
			if(item->b_isNew)
			{
				ret = true;
				break;
			}
		}
	}
	return ret;
}

/*
背包格
*/
BackPackItem::BackPackItem(unsigned int type)
{
	packType = type;
	m_itemInfo = NULL;
	m_attribution = new std::list<ITEMATTR*>();
	m_equipInfo = NULL;
	m_spriteInfo = NULL;
	m_manorBaseInfo = NULL;
	itemLevel = 1;
	b_isNew = false;
}

BackPackItem::~BackPackItem()
{	
	if(m_attribution)
	{
		for(std::list<ITEMATTR*>::iterator iter = m_attribution->begin();
			iter != m_attribution->end(); ++iter)
		{
			ITEMATTR* attr = *iter;
			if(attr)
			{
				delete attr;
				attr = NULL;
			}
		}
		m_attribution->clear();
		delete m_attribution;
		m_attribution = NULL;
	}
	
	resetEquipInfo();
	CC_SAFE_DELETE(m_spriteInfo);
	//CC_SAFE_DELETE(m_itemInfo);
	CC_SAFE_RELEASE(m_itemInfo);
	m_itemInfo = NULL ;
	CC_SAFE_DELETE(m_manorBaseInfo);
}

ItemBase* BackPackItem::getItemInfo()
{
	if(NULL == m_itemInfo)
	{
		m_itemInfo = ItemManager::Get()->getItemInfoById(itemId);
		if(m_itemInfo)
			m_itemInfo->retain();
	}

	return m_itemInfo;

}

void BackPackItem::setItemId(unsigned int id)
{
	if(id != itemId)
	{
		if(m_itemInfo != NULL)
		{  m_itemInfo = NULL ;
		CC_SAFE_RELEASE(m_itemInfo);
			//CC_SAFE_DELETE(m_itemInfo);
		}
		itemId = id;
	}
}

void BackPackItem::resetEquipInfo()
{
	if(this->m_equipInfo)
	{
		this->m_equipInfo->m_equipHoles.clear();
		this->m_equipInfo->m_nextAttribution.clear();
		this->m_equipInfo->m_suitAttribution.clear();
		delete m_equipInfo;
		m_equipInfo = NULL;
	}
}

void BackPackItem::setEquipInfo(EquipmentBase equipmentInfo)
{
	resetEquipInfo();
	if(!m_equipInfo)
	{
		m_equipInfo = new EquipmetInfo();
	}
	m_equipInfo->levelUpCost = equipmentInfo.cost_levelup();
	unsigned int count = equipmentInfo.incr_attr_next_lv_size();
	for(unsigned int i = 0; i< count; i++)
	{
		EquipmentBase_Item_Attr attr = equipmentInfo.incr_attr_next_lv(i);
		EQUIPNEXTATTR nextAttr;
		nextAttr.key = attr.key();
		nextAttr.value = attr.value();
		m_equipInfo->m_nextAttribution.push_back(nextAttr);
	}
	if(this->packType == 2 || this->packType == 6)
	{
		count = equipmentInfo.rel_ids_size();
		for(unsigned int i = 0; i<count; i++)
		{
			m_equipInfo->m_suitAttribution.push_back(equipmentInfo.rel_ids(i));
		}
	}
	else
	{
		m_equipInfo->m_suitAttribution.clear();
	}
	unsigned int holeCount = equipmentInfo.equip_holes_size();
	for(unsigned int j=0; j<holeCount; j++)
	{
		EQUIPHOLE hole;
		CSEquipHole cs_hole = equipmentInfo.equip_holes(j);
		hole.attr_key = cs_hole.attr_key();
		hole.attr_value = cs_hole.attr_value();
		hole.hole_index = cs_hole.hole_index();
		hole.item_gem_id = cs_hole.item_gem_id();
		if(cs_hole.has_attr_max_v())
		{
			hole.max_value = cs_hole.attr_max_v();
		}
		else
		{
			hole.max_value = 0;
		}
		if(cs_hole.has_attr_min_v())
		{
			hole.min_value = cs_hole.attr_min_v();
		}
		else
		{
			hole.min_value = 0;
		}
		if(m_equipInfo->m_equipHoles.find(hole.hole_index) == m_equipInfo->m_equipHoles.end())
		{
			m_equipInfo->m_equipHoles.insert(std::make_pair(hole.hole_index,hole));
		}
	}
}

void BackPackItem::setSpriteInfo(SpiritStoneBase base)
{
	if(m_spriteInfo == NULL)
	{
		m_spriteInfo = new SPRITEINFO();
	}
	m_spriteInfo->levelUpCost = base.consume();
	m_spriteInfo->convertSp = base.convert();
}

void BackPackItem::setManorInfo(const ManorBase &base)
{
	if(m_manorBaseInfo == NULL)
	{
		m_manorBaseInfo = new MANORBASEINFO();
	}
	m_manorBaseInfo->hurtGetCoin = base.hunt_gain_coin();
	m_manorBaseInfo->pickGetExp = base.pick_gain_exp();
}

void BackPackItem::resetItem()
{
	resetEquipInfo();
	CC_SAFE_DELETE(m_spriteInfo);
	//CC_SAFE_DELETE(m_itemInfo);
	CC_SAFE_RELEASE(m_itemInfo);
	m_itemInfo = NULL ;
	if(m_attribution)
	{
		for(std::list<ITEMATTR*>::iterator iter = m_attribution->begin();
			iter != m_attribution->end(); ++iter)
		{
			ITEMATTR* attr = *iter;
			if(attr)
			{
				delete attr;
				attr = NULL;
			}
		}
		m_attribution->clear();
	}
}

/*
物品交换列表
*/
ExchangeItem::ExchangeItem()
{
	m_exchangeType = 0;
	m_fromItems = NULL;
	m_toItems = NULL;
}

ExchangeItem::~ExchangeItem()
{
	if(m_fromItems)
	{
		m_fromItems->clear();
		CC_SAFE_DELETE(m_fromItems);
	}

	if(m_toItems)
	{
		m_toItems->clear();
		CC_SAFE_DELETE(m_toItems);
	}
}

void ExchangeItem::setFromItems(std::string fromStr)
{
	m_fromItems = initItemsByString(fromStr);
}

void ExchangeItem::setToItems(std::string toStr)
{
	m_toItems = initItemsByString(toStr);
}

std::list<pair<unsigned int ,unsigned int> >* ExchangeItem::initItemsByString(std::string str)
{
	std::list<pair<unsigned int ,unsigned int> >* items = new std::list<pair<unsigned int ,unsigned int> >();
	int currentPos = 0;
	while(currentPos != str.npos)
	{
		int pos = str.find_first_of(';',currentPos);

			std::string itemStr = str.substr(currentPos,pos-currentPos);
			int dashPos = itemStr.find_first_of("/");
			if(dashPos != itemStr.npos)
			{
				std::string idStr = itemStr.substr(0,dashPos);
				std::string numStr = itemStr.substr(dashPos+1,pos-1);
				int id = atoi(idStr.c_str());
				int num = atoi(numStr.c_str());
				items->push_back(std::make_pair(id,num));
				//items->insert(std::make_pair(id,num));
			}		
			if(pos != str.npos)
			{
				currentPos = pos+1;
			}
			else
			{
				currentPos = pos;
			}
	}
	return items;
}

