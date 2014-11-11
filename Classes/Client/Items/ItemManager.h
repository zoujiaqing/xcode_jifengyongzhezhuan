#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "Singleton_t.h"
#include "cocos2d.h"
#include "EncodeValue.hpp"
#include <map>
#include <list>
#include "ItemBase.h"
#include "EquipmentItem.h"
#include "cs_core.pb.h"
using namespace cocos2d;
using namespace std;

#define AlCHEMYBAGCAPACITY	30
#define AlCHEMYSTORAGECAPACITY	18
#define FAIRYFRAGMENTBAGCAPACITY 200

/*
// id 0~999 背包格 1000~1999 仓库格 2000~2999 装备格 3000~3999 灵石包 4000~4999 灵石仓库 5000~5999 灵石装备格 7000 ~ 7999 宝石格 8000 ~ 8999 卡牌背包 9000 ~ 9999 卡牌装备
	10001 ~ 10100 庄园格 50000+ 回收站
*/
enum
{
	kItemBag = 0,
	kItemStorage = 1000,
	kItemHeroEquip = 2000,
	kItemEnchaseEquip = 2050, //精灵镶嵌位为装备位+50
	kItemFairyEquip = 2100,
	kAlchemyBag = 3000,
	kAlchemyStorage = 4000,
	kAlchemyHeroEquip = 5000,
	kAlchemyFairyEquip = 5100,
	kFairyStartId = 6000,
	kItemGemStone = 7000,
	kCardBag = 8000,
	kCardEquip = 9000,
	kItemManorId = 10000,
	kEquipFragmet = 11000,
	kFairyFragment = 12000,
	kItemRecycle = 50000,
};

typedef struct EquipmetHole
{
	// Note:  need
	EquipmetHole(){
		hole_index = 0;
		item_gem_id = 0;
		attr_key = 0;
		attr_value = 0;
		max_value = 0;
		min_value = 0;
	}
	EncodeValue<unsigned int> hole_index;		// 孔编号
	EncodeValue<unsigned int> item_gem_id;		// 宝石id
	EncodeValue<unsigned int> attr_key;			// 影响属性的key
	EncodeValue<unsigned int> attr_value;		// 影响属性的value
	EncodeValue<unsigned int> max_value;			// 属性可能最小值
	EncodeValue<unsigned int> min_value;			// 属性可能最大值
}EQUIPHOLE;

typedef struct ItemAttribution
{
	ItemAttribution(){
		attr_key = 0;
		attr_basic_value = 0;
		attr_intensify_value = 0;
		attr_gem_value = 0;
	}
	EncodeValue<unsigned int> attr_key;					// 物品属性key
	EncodeValue<unsigned int> attr_basic_value;			// 物品基本属性对应的value
	EncodeValue<unsigned int> attr_intensify_value;		// 强化影响物品属性value增量
	EncodeValue<unsigned int> attr_gem_value;			// 宝石影响物品属性value增量
}ITEMATTR;

typedef struct EquipNextAttribution
{
	EquipNextAttribution(){}
	unsigned int key;					// 装备强化下一级属性key
	unsigned int value;					// 装备强化下一级属性value
}EQUIPNEXTATTR;

typedef struct EquipmetInfo
{
	EquipmetInfo(){levelUpCost = 0;}
	unsigned int levelUpCost; // 装备升级花费
	std::list<EQUIPNEXTATTR>	m_nextAttribution; // 装备强化下一级属性
	std::vector<unsigned int> m_suitAttribution; // 激活的套装属性
	std::map<unsigned int,EQUIPHOLE> m_equipHoles;	// 装备孔
}EQUIPINFO;

typedef struct SpriteStoneInfo
{
	SpriteStoneInfo()
	: levelUpCost(0)
	, convertSp(0)
	{}
	unsigned int levelUpCost; // 升级需要的灵能
	unsigned int convertSp;	  // 转化的灵能
}SPRITEINFO;

typedef struct ManorBaseInfo
{
	ManorBaseInfo()
		: hurtGetCoin(0)
		, pickGetExp(0)
	{
	}
	unsigned int hurtGetCoin; // 打猎获取金币
	unsigned int pickGetExp;  // 采集获取经验
}MANORBASEINFO;

class BackPackItem
{
	// 背包格
public: 
	BackPackItem(unsigned int packType);
	~BackPackItem();
	ItemBase* getItemInfo();
	void setItemId(unsigned int id);
	void resetEquipInfo();
	void setEquipInfo(EquipmentBase base);
	unsigned int getItemId(){return itemId;}
	void setSpriteInfo(SpiritStoneBase base);
	void setManorInfo(const ManorBase &base);
	void resetItem();
public:
	bool isLocked;			// 是否解锁
	bool isEmpty;			// 是否为空	
	bool b_isNew;				// 是否为新添加
	unsigned int position;	// 物品位置
	unsigned int amount;	// 物品数量
	unsigned int itemLevel;	// 物品等级
	EQUIPINFO* m_equipInfo;	// 装备强化属性
	unsigned int packType; // 格子类型： 0 背包; 1仓库 ; 2装备栏 ;3 灵石背包;4 灵石仓库 ; 5灵石装备栏 ;6 精灵装备 7 庄园 , 8精灵碎片
	std::list<ITEMATTR*>*	m_attribution;
	SPRITEINFO* m_spriteInfo; // 灵石特有属性
	MANORBASEINFO* m_manorBaseInfo; // 庄园信息

private:
	ItemBase* m_itemInfo;	    // 物品基本配置属性
	unsigned int itemId;		// 物品Id	

};

class ExchangeItem
{
// 物品交换表格

public:
	ExchangeItem();
	~ExchangeItem();
	void setFromItems(std::string fromStr);
	void setToItems(std::string toStr);

	struct smaller
	{
		bool operator()(ExchangeItem* a,ExchangeItem* b)
		{
			return a->m_exchangeId < b->m_exchangeId;
		}
	};

private:
	std::list<std::pair<unsigned int ,unsigned int> >* initItemsByString(std::string str);
public:
	unsigned int m_exchangeType;
	unsigned int m_exchangeId;
	unsigned int m_requiredLevel;
	std::list<std::pair<unsigned int ,unsigned int> >* m_fromItems;
	std::list<std::pair<unsigned int ,unsigned int> >* m_toItems;
	/*std::map<unsigned int ,unsigned int>* m_fromItems;
	std::map<unsigned int ,unsigned int>* m_toItems;*/
};

typedef std::map<unsigned int ,BackPackItem*> BACKPACK_VECTOR;

class ItemManager : public TSingleton<ItemManager>
{
public:
	ItemManager();
	virtual ~ItemManager();

	void init();

	BackPackItem* findItemByPos(unsigned int pos);
	//bool addItem()	
	void resetItems();
	unsigned int getBackPackEmptyGridIndex();
	void setBackPackDefaultPos(unsigned int ownerPos);
	void setBackPackUnlockPos(unsigned int pos);
	void setStorageUnlockPos(unsigned int pos);
	void setEquipMaxTimes(unsigned int maxTimes);

	unsigned int getBackPackMaxPos(){return m_maxPackPos;}

	void openBackPack(bool withEquipment);
	void closeEquipment();

	void exchangeItemPos(unsigned int from, unsigned int to);
	void setItem(CSItemInfo info);

	void setStorageDefaultPos(unsigned int ownerPos);
	unsigned int getStorageEmptyGridIndex();
	unsigned int getAlchemyBagEmptyGridIndex();
	unsigned int getAlchemyEquipEmptyGridIndex(unsigned int index);

	unsigned int getStorageMaxPos(){return m_maxStoragePos;}

	void removeItem(unsigned int pos, unsigned int id);
	bool setEquipItemInfor(unsigned int pos,EquipmentBase equipmetInfo);
	void addExchangeItem(unsigned int exchangeId,std::string fromItems, std::string toItems,
						unsigned int type,unsigned int requiredLevel);

	bool checkEquipDrawingUseful(unsigned int drawingId);
	ItemBase* getItemInfoById(unsigned int id);
	ExchangeItem* getEquipDrawingById(unsigned int equipId); // 基于查找到from item查找交换表格
	ExchangeItem* getExchageItemById(unsigned int exchangeId);						 // 基于表格id查找表格
	ExchangeItem* getExchageItemByIdWithType(unsigned int itemID,unsigned int type = 0);  // 基于查找到from item查找指定类型交换表格,type 为0表示所有类型
	std::list<unsigned int>* getExchangeSpriteStones(int exchangeType);
	std::list<ExchangeItem*>* getExchangeItemsByType(unsigned int type); // 得到类型为type的所有交换信息

	unsigned int getItemNumberInOnlyInBag(unsigned int id);
	unsigned int getItemNumberById(unsigned int id);

	// 根据引导配置的名称获得背包中真实button名称，并翻页
	string getItemButtonNameWithAjust(string turButtonName, bool ajust = true);
	string getTurEventNameByButtonName(string buttonName);

	CCSprite* getIconSpriteById(unsigned int ItemId);
	void loadExchangeItems();
	
	// 设置星座中解锁的卡牌数
	void setConstellationCardNum(unsigned int card,unsigned int num);

	// alchemy
	
	void setAlchemyBagUnlockCapacity(unsigned int value);
	void setAlchemyStorageUnlockCapacity(unsigned int value);
	void setAlchemyBackDefaultCapacity(unsigned int pos);
	void setAlchemyStorageDefaultCapacity(unsigned int pos);
	void reqAlchemyUpgradeSelected(unsigned int pos);
	void reqAlchemyUpgradeUnselected();	
	int getCurrentAlchemyIndex();
	int getCurrentEquipmentIndex();
	bool checkPlayerEquipUnlock(int index);
	void checkAlchemyEquipUnlock();
	void showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt);

	unsigned int getAlchemySearchNpcCost(unsigned int npcID);
	void showItemTipsByPos(unsigned int pos,CCPoint pt);
	void showItemTipsById(unsigned int itemId,CCPoint pt);
	void setTipDelegate(cocos2d::CCObject*	leftTarget,cocos2d::SEL_MenuHandler leftHandler,const char* leftText,bool isLeftVisible,
						cocos2d::CCObject*	rightTarget,cocos2d::SEL_MenuHandler rightHandler,const char* rightText,bool isRithtVisible);
	void setTipMiddleDelegate(cocos2d::CCObject*	leftTarget,cocos2d::SEL_MenuHandler leftHandler,const char* leftText,bool isLeftVisible);
	void setTipLuaHander(int leftHandler,const char* leftText,bool isLeftVisible,
						int rightHandler,const char* rightText,bool isRightVisible);
	void setMiddleLuaHander(int leftHandler,const char* leftText,bool isLeftVisible);
	// 属性文件 格式 ： 属性+数字
	const char* getAttributionText(unsigned int key,unsigned int value);

	const char* getAttributionKeyText(unsigned int key);

	CCSprite* getIconFrame(unsigned int level);
	ccColor3B getLabelColorByQuality(unsigned int quality);
	ccColor3B getFairyLabelColorByQuality(unsigned int quality);

	// weapon id 
	unsigned int getHeroWeaponId();
	unsigned int getHeroDefautWeaponId(unsigned int heroTypeId);

	void checkBagEmptyItemNum();

	void clearItems();

	void setEquipNewNote(unsigned int pos,bool isNew);
	bool checkNewEquipInfo();
private: 
	// 显示物品信息
	void showEquipmentTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt);
	void showEquipDrawingTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt);
	void showUnusedItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt);
	void showusedItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt);
	void showAlchemyItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt);
	void showCardItemTipByPos(BackPackItem* item,CCSprite* icon,CCPoint pt);

	void showEquipDrawingTipByItemid(ItemBase* item,CCSprite* icon,CCPoint pt);
	void showNormalItemTipByItemid(ItemBase* item,CCSprite* icon,CCPoint pt);
	void showCardTipByItemid(ItemBase* item,CCSprite* icon,CCPoint pt);
	void resetAlchemyBagPacks(unsigned int ownerPos);
	void resetAlchemyStoragePacks(unsigned int ownerPos);
	void resetBagPacks();
	void resetStoragePacks();

	unsigned int getEquipIdInDrawing(ExchangeItem* item);
	BackPackItem* getEquipmentByPos(unsigned int owner,unsigned int part);

	unsigned int getBagEmptyNum();
private:
	BACKPACK_VECTOR *m_backPackList;
	BACKPACK_VECTOR *m_storageList;
	BACKPACK_VECTOR *m_equipmentList;
	BACKPACK_VECTOR *m_equipEnchaseList;
	BACKPACK_VECTOR *m_fairyEquipmentList;
	BACKPACK_VECTOR *m_alchemyBagList;
	BACKPACK_VECTOR *m_alchemyStorageList;
	BACKPACK_VECTOR *m_alchemyEquipList;
	BACKPACK_VECTOR *m_alchemyFairyEquipList;
	BACKPACK_VECTOR *m_gemStoneList;
	BACKPACK_VECTOR *m_manorItemList;
	BACKPACK_VECTOR *m_recycleItemList;
	BACKPACK_VECTOR *m_cardBagList;
	BACKPACK_VECTOR *m_cardEquipList;
	BACKPACK_VECTOR *m_equipmentFragList;
	BACKPACK_VECTOR *m_fairyFragList;
	unsigned int m_maxPackPos;					// 背包格子总数
	unsigned int m_defaultPackPos;				//玩家默认的背包格子数
	unsigned int m_unlockPackPos;				//已解锁的背包格子数
	unsigned int m_maxStoragePos;				//仓库格子总数
	unsigned int m_defaultStoragePos;			//默认的仓库格子数
	unsigned int m_unlockStoragePos;			//已解锁的仓库的格子数
	unsigned int m_defaultAlchemyBagPos;		//默认的炼金桌格子数
	unsigned int m_unlockAlchemyBagPos;			//已解锁的炼金桌格子数
	unsigned int m_defaultAlchemyStoragePos;	//默认的炼金背包格子数
	unsigned int m_unlockAlchemyStroragePos;	//已解锁的炼金背包格子数
	unsigned int m_surplusGemRecastNum;			//默认宝石重铸次数
	unsigned int m_defaultManorPackNum;			//默认庄园个数
	unsigned int m_defaultCardBagPos;			//默认卡牌背包格子数
	unsigned int m_defaultCardEquipPos;			//默认卡牌装备格子数
	std::map<unsigned int ,unsigned int>* m_constellationCardNum;  // 星座解锁卡牌数
	std::map<unsigned int ,unsigned int>* m_searchNpcCostMap;
	std::list<ExchangeItem*>* m_exchangeItemList;
	bool b_isLoadExchangeItems;
};

#endif	