#ifndef BASE_LISTENER_H
#define BASE_LISTENER_H

#include "cocos2d.h"
#include "LevelManager.h"
#include <list>
using namespace std;

class SpriteBase;


class BaseListener
{

public:

	BaseListener();
	virtual ~BaseListener();

	static BaseListener*		sShareInstance();

	// init all the listeners or shutdown all the listeners
	static void _SDM_init_Listener();
	static void _SDM_shutdown_Listener();

public:

	//////////////////////////////////////////////////////////////////////////
	//event from level layer
	//////////////////////////////////////////////////////////////////////////	

	virtual void				HandleLayerTouchBeginEvent(cocos2d::CCNode* obj, cocos2d::CCPoint pt){}
	virtual void				HandleLayerTouchEndEvent(cocos2d::CCNode* obj, cocos2d::CCPoint pt){}
	virtual void				HandleLayerActiveEvent(cocos2d::CCNode* obj, sActiveLevelConfig* config);
	virtual void				HandleLayerDeactiveEvent(cocos2d::CCNode* obj);
	virtual void				HandleLayerUpdateEvent(cocos2d::CCNode* obj, float dt){}

	//////////////////////////////////////////////////////////////////////////
	//event from object
	//////////////////////////////////////////////////////////////////////////
	virtual void				HandleObjectTouchEvent(SpriteBase* obj, cocos2d::CCPoint pt){}	

private:

	static BaseListener*		sInstance;

};

typedef list<BaseListener*> ArrayBaseListener;



#endif