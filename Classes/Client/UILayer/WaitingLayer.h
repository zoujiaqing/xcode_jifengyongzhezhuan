#pragma once

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class WaitingLayer : public cocos2d::CCLayer
{
public:
	WaitingLayer();
	virtual ~WaitingLayer();

	CREATE_FUNC(WaitingLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	void update(float dt);

	void ShowWaiting(bool bShowRightNow = true,float sumTimer = 1.0);

	void HideWaiting();

protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

private: 
	CCSprite* m_pSprite;
	bool m_bShow;
	float m_sumHideTimer;
	float m_hidingTimer;
};
