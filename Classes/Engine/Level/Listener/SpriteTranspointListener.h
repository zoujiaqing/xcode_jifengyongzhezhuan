
#ifndef SPRITE_TRANSPOINT_LISTENER_H
#define SPRITE_TRANSPOINT_LISTENER_H

#include "BaseListener.h"


class InstanceListLayer;
class SpriteTransPoint;
class SpriteTranspointListener : public BaseListener
{

public:

	SpriteTranspointListener();
	virtual ~SpriteTranspointListener();

	static SpriteTranspointListener*	sShareInstance();


public:

	virtual void					HandleObjectTouchEvent(SpriteBase* obj, cocos2d::CCPoint pt);
	void							HandleLayerActiveEvent(cocos2d::CCNode* obj, sActiveLevelConfig* data);
private:
	void moveAndOpenInstanceList(SpriteTransPoint * transpoint, InstanceListLayer * layer);
	static SpriteTranspointListener*	sInstance;

};





#endif