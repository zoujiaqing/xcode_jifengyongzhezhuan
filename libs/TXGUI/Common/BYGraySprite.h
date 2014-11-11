//
// BYGraySprite.h
//

#pragma once


#include "cocoa/CCGeometry.h"
#include "cocos2d.h"
USING_NS_CC;


class BYGraySprite : public CCSprite
{
public:
	BYGraySprite();
	virtual ~BYGraySprite();
	static BYGraySprite* create(const char* pszFileName);
	static BYGraySprite* create();
	static BYGraySprite* createWithTexture(CCTexture2D *pTexture);
	bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
	bool initWithTexture(CCTexture2D *pTexture);
	virtual void draw();
};

extern CCSprite* graylightWithCCSprite(CCSprite* oldSprite,bool isLight);

