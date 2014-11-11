#ifndef DRAG_RECEIVER_H
#define DRAG_RECEIVER_H

#include "cocos2d.h"
#include <List>
using namespace cocos2d;

namespace TXGUI{

	typedef bool (CCObject::*CheckDragFunc)(CCTouch *);
	#define checkDrag_selector(_SELECTOR) (CheckDragFunc)(&_SELECTOR)

	enum DragHandlerType
	{
		EUnKowned	   = 0,
		EBackPackItem  = 1,			// 背包格
		EStorageItem   = 2,			// 仓库格
		EEquipmentItem = 3,			// 装备格
		EUIBg		   = 4,			// UI面板
		EAlchemyButton = 5,			// 炼金熔灵石
		EAlchemyUpgrade = 6,		// 炼金石升级格
		EAlchemyBag	   = 7,			// 灵石背包格   
		EAlchemyStorage= 8,			// 灵石仓库格 
		EAlchemyEquip  = 9,			// 灵石装备格 
		EALchemyToStorage = 10,		// 灵石拖入仓库
		ESkillEquipItem	  = 11,		// 技能列表
		ESlotItem = 12,			// 技能槽格
		ECardBagItem	= 13,		// 卡牌背包
		ECardEquipItem	= 14,		// 卡牌装备
		EFragBag   = 15,			// 装备碎片格
	};

	class DragReceiver
	{


	public:
		//DragReceiver();
		DragReceiver(int priority,DragHandlerType type,int pos);
		~DragReceiver();
		void setPriority(int priority);
		void setHandlerType(DragHandlerType type,int pos){m_type =type;m_handlerPos = pos; }
		virtual bool checkDragIn(CCTouch* touch);
		void setCheckDragFunc(CheckDragFunc function,CCObject* target);
		int getPriority(){return m_priority;}

		void setDragItem(CCSprite* item){m_pDragItem = item;}
		CCSprite* getDragItem(){return m_pDragItem;}
		void SetTouchDelegate(CCTouchDelegate* handler){m_pTouchDelegate = handler;}
		CCTouchDelegate* getTouchDelegate(){return m_pTouchDelegate;}
		DragHandlerType GetDragItemType(){return m_type;}
		int GetHandlerPosition(){return m_handlerPos;}
		void setHandlerPosition(int pos){m_handlerPos = pos;}
	private:
		CCTouchDelegate* m_pTouchDelegate;
		CCSprite* m_pDragItem;
		int m_priority;
		DragHandlerType m_type;
		int m_handlerPos; 
		CheckDragFunc m_checkFunc;
		CCObject* m_target;
	
	};


	class DragReceiverRegister
	{
	public:
		DragReceiverRegister();
		~DragReceiverRegister();

		static DragReceiverRegister *sharedManager();
		static void Destroy();
		virtual void registerDragReceiver(DragReceiver* receiver);
		virtual void unregisterDragReceiver(DragReceiver* receiver);
		std::list<DragReceiver*>* getReceivers(){return m_pDragReceivers;}
	private:
		static DragReceiverRegister* pSharedManager;
		std::list<DragReceiver*>* m_pDragReceivers;
	};

}
#endif