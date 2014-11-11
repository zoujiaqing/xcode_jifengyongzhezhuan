#ifndef  ITEM_DRAG_LAYER_H
#define  ITEM_DRAG_LAYER_H

#include "cocos2d.h"
#include "DragReceiver.h"
#include "UIDragLayer.h"
#include <list>

using namespace cocos2d;
using namespace TXGUI;

class ItemDragLayer : public CCLayer
{
public:
	ItemDragLayer();
	~ItemDragLayer();

	CREATE_FUNC(ItemDragLayer);
	virtual bool init();

	void onClickConfigDeleteItem(CCObject* sender);
	void onClickCancellDeleteItem(CCObject* sender);

	virtual void registerDragReceiver(DragReceiver* receiver);
	virtual void unregisterDragReceiver(DragReceiver* receiver);
	virtual void startDrag(DragReceiver* handler,CCTouch* pTouch,CCEvent *pEvent);

	virtual void dealWithDragAction(DragReceiver* from,DragReceiver* to);
	virtual void dragEndWithoutUI(DragReceiver* from);
    
private:
	virtual int getReceiverIndex(DragHandlerType type, unsigned int pos);
    
private:
	UIDragLayer* m_pDragUILayer;
	int	m_currentDeletePos;
};
#endif