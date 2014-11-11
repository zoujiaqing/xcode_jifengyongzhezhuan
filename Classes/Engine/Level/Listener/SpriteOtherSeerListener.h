
#ifndef SPRITE_OTHER_SEER_LISTENER_H
#define SPRITE_OTHER_SEER_LISTENER_H

#include "SpriteSeerListener.h"
#include "PhysicsLayer.h"
#include "UserData.h"

//-----------------------------------------------------------------------
//			the listener is to deal other players seer logic
//			the class is based on spriteSeerListener
//			any logic diff with player seer,
//			please override the interface
//-----------------------------------------------------------------------

class SpriteOtherSeerListener : public CCObject, public SpriteSeerListener
{

public:

	SpriteOtherSeerListener();
	virtual ~SpriteOtherSeerListener();

	static SpriteOtherSeerListener*	sShareInstance();


public:

	virtual void				HandleLayerTouchBeginEvent(cocos2d::CCNode* obj, cocos2d::CCPoint pt);
	virtual void				HandleLayerActiveEvent(cocos2d::CCNode* obj, sActiveLevelConfig* data);
	virtual void				HandleLayerUpdateEvent(cocos2d::CCNode* obj, float dt);

	virtual void				HandleObjectMoveEvent(SpriteBase* obj);
	virtual void				HandleObjectEndMoveEvent(SpriteBase* obj);
	virtual void				HandleLayerDeactiveEvent(cocos2d::CCNode* obj);

	void						HandleNetWorkUpdate(SpriteBase* obj, PLAYER_ID uid, cocos2d::CCPoint pt, unsigned int animID, bool bFlip);

	void EndMove(CCNode*, void*);

protected:
	DIRECTION_ACTOR		m_curfaceDir;
private:

	static SpriteOtherSeerListener*	sInstance;

	bool	m_bSendEndMoveMesage;
	float	m_endMoveTime;
};





#endif