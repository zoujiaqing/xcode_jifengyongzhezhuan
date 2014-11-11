#ifndef MARQUEE_UI_LAYER_H
#define MARQUEE_UI_LAYER_H

#include "cocos2d.h"
using namespace cocos2d;

class MarqueeUILayer : public CCLayer
{
public:
	MarqueeUILayer();
	virtual ~MarqueeUILayer();

	CREATE_FUNC(MarqueeUILayer);
	virtual bool init();

	//virtual void update(float fDelta);

	virtual void visit();

private:
	CCSize    m_viewSize;
	CCPoint   m_bgPoint;
};

#endif