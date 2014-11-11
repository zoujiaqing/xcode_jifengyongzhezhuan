
#ifndef SPRITE_SEER_LISTENER_H
#define SPRITE_SEER_LISTENER_H

#include "BaseListener.h"



class SpriteSeerListener : public BaseListener
{

public:

	SpriteSeerListener();
	virtual ~SpriteSeerListener();

	static SpriteSeerListener*	sShareInstance();


public:

	virtual void				HandleLayerTouchBeginEvent(cocos2d::CCNode* obj, cocos2d::CCPoint pt);
	virtual void				HandleLayerActiveEvent(cocos2d::CCNode* obj, sActiveLevelConfig* data);
	virtual void				HandleLayerDeactiveEvent(cocos2d::CCNode* obj);
	virtual void				HandleLayerUpdateEvent(cocos2d::CCNode* obj, float dt);

	virtual void				HandleObjectMoveEvent(SpriteBase* obj);
	virtual void				HandleObjectEndMoveEvent(SpriteBase* obj);

private:

	static SpriteSeerListener*	sInstance;
	float m_lastMoveSyn;
	CCPoint m_lastSendPos;
	bool	m_bSendMoveMesage;

	bool	m_bSendEndMoveMesage;
	float	m_endMoveTime;

	unsigned int m_oldAnimID;
};





#endif