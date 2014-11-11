#ifndef TIP_TOUCH_LAYER_H
#define TIP_TOUCH_LAYER_H

#include "cocos2d.h"

class TipTouchLayer : public cocos2d::CCLayer
{
public:
	TipTouchLayer();
	~TipTouchLayer();
	CREATE_FUNC(TipTouchLayer);
	virtual bool init();

protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

private:
	void onTouchClicked();
};

#endif