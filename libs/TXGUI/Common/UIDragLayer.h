#ifndef UI_DRAG_LAYER_H
#define UI_DRAG_LAYER_H

#include "cocos2d.h"
#include "DragReceiver.h"
#include "UIDragLayer.h"
using namespace cocos2d;

namespace TXGUI
{
	typedef void (CCObject::*DragMoveFunc)(DragReceiver* ,DragReceiver*);
	typedef void (CCObject::*DragDeleteFunc)(DragReceiver*);

	#define dragMove_selector(_SELECTOR) (DragMoveFunc)(&_SELECTOR)
	#define dragDelete_selector(_SELECTOR) (DragDeleteFunc)(&_SELECTOR)

	class UIDragLayer : public cocos2d::CCLayer
	{
	public:
		UIDragLayer();
		virtual ~UIDragLayer();
		CREATE_FUNC(UIDragLayer);
		virtual bool init();

		virtual void registerDragReceiver(DragReceiver* receiver);
		virtual void unregisterDragReceiver(DragReceiver* receiver);
		virtual void startDrag(DragReceiver* handler,CCTouch* pTouch,CCEvent *pEvent);
		virtual void setDragDealTarget(CCObject* obj,DragMoveFunc moveFunc,DragDeleteFunc deleteFunc);
	protected:
		virtual void registerWithTouchDispatcher();
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	private:
		CCSprite* deepClone(CCSprite* target);
	private:
		bool b_active;
		std::list<DragReceiver*>* m_pDragReceivers;
		DragReceiver* m_currentDragHandler;
		CCSprite* m_pDragItem;
		CCTouch* m_currentTouch;
		DragMoveFunc m_dragMoveFunc;
		DragDeleteFunc m_dragDeleteFunc;
		CCObject* m_target;
	};
}

#endif