#ifndef LOADING_LAYER_H
#define LOADING_LAYER_H


#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class LoadingLayer : public cocos2d::CCLayer
{
public:
	LoadingLayer();
	virtual ~LoadingLayer();

	CREATE_FUNC(LoadingLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	void update(float dt);

	void ShowLoading(bool isShowOnly = false);

	void HideLoading();

protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
private: 
	bool b_Hide;
};

#endif