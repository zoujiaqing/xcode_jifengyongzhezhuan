#pragma once

#include "string"
#include "cocos2d.h"
USING_NS_CC;

class ArmatureSpritePool:public CCNode
{
public:
	static ArmatureSpritePool *create();
public:
	ArmatureSpritePool();
	~ArmatureSpritePool();

	virtual bool init();
	CCSprite* GetOneSprite(std::string _boneName,std::string _imgName);
protected:
	bool CreateOneSprite(std::string _boneName,std::string _imgName);
private:
	CCDictionary *mSpriteDic;
};