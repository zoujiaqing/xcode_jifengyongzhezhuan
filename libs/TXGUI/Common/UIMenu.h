#ifndef ADVENCE_MENU_H
#define ADVENCE_MENU_H

#include "cocos2d.h"
#include "ItemDragLayer.h"
#include "DragReceiver.h"
#include "UIControlBase.h"
using namespace cocos2d;

namespace TXGUI{
	class UIMenu : public cocos2d::CCMenu
	{
	public:
		UIMenu();
		virtual ~UIMenu();

		static UIMenu* create();
		static UIMenu* create(CCMenuItem* item, ...);
		static UIMenu* createWithItem(CCMenuItem* item);
		static UIMenu* createWithItems(CCMenuItem *firstItem, va_list args);
		static UIMenu* createWithArray(CCArray* pArrayOfItems);

		virtual void registerWithTouchDispatcher();

		virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
		virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);

		virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
		virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);


		virtual void SetDragMove(bool value, DragReceiver* m_dragHandler);
		virtual void SetDoubleClickHandler(CCObject* listerner,SEL_MenuHandler handler);
		virtual void SetTappedHandler(CCObject* listerner,SEL_MenuHandler tapHandler,SEL_MenuHandler tapCancelHandler);
		virtual void SetNormalIcon(CCSprite* normalTex);
		virtual bool checkReleaseTouchInButton(CCTouch* touch);

		virtual void setControlNode(UIControlBase* controller){m_relativeController = controller;m_name = controller->getName();}
		virtual UIControlBase* getControlNode(){return m_relativeController;}
		virtual void setTouchActivityTime(bool value){m_bTouchActivity = value;}
		virtual CCPoint getTouchPoint();
		virtual void SetDragFlagTime(unsigned int ms);
	protected:
		void StopAccumulateSleepTime();
		void StartAccumulateSleepTime();

		void Update(float ft);
	private:
		long long millisecondNow();
		void checkMenuTapped();
		void checkMenuTapCancel();
	private:
		CCSprite* m_btIconSprite;

		/// drag sleep time
		long m_dragSleepTime;
		bool m_bTouchActivity; // 是否在点击时触发按键响应 true:按下时触发 false:松开时触发
		bool m_bDoubleTouchFlag;
		bool m_bDefineDragMove;
		bool b_lastStatus;
		DragReceiver* m_pDragHandler;
		bool m_bMoveAway;
		CCPoint m_touchBeginPoint;
		long long m_lastTouchTime;
		unsigned int m_dragTimeFlag;
		CCMenuItem* m_lastTouchedItem;

		CCObject*       m_doubleTouchListener;
		SEL_MenuHandler    m_doubleTouchHandler;

		CCObject*       m_tappedTouchListener;
		SEL_MenuHandler    m_tappedTouchHandler;
		SEL_MenuHandler    m_tapCancelTouchHandler;
		bool			m_tappedFlag;
		UIControlBase*		m_relativeController;


		// for test
		string m_name;
	};
}
#endif