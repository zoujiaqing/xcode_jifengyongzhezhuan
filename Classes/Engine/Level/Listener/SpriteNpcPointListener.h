
#ifndef SPRITE_TRANSPOINT_LISTENER_H
#define SPRITE_TRANSPOINT_LISTENER_H

#include "BaseListener.h"



class SpriteNPCListener : public BaseListener
{

public:

	SpriteNPCListener();
	virtual ~SpriteNPCListener();

	static SpriteNPCListener*	sShareInstance();


public:

	virtual void					HandleObjectTouchEvent(SpriteBase* obj, cocos2d::CCPoint pt,CCRect colliderRect = CCRectZero);
	void							HandleLockedEvent(SpriteBase* obj);
	void							HandleTransformEvent(SpriteBase* obj);

private:

	static SpriteNPCListener*	sInstance;

};





#endif