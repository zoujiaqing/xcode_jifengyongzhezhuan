#ifndef ATLMAPLAYER_2012_12_26_H
#define ATLMAPLAYER_2012_12_26_H

#include "../../../cocos2dx/cocos2dx/sprite_nodes/CCSpriteBatchNode.h"
#include "../../../cocos2dx/cocos2dx/base_nodes/CCNode.h"
#include "ATLMapInfo.h"

class CATLMapLayer  : public cocos2d::CCSpriteBatchNode
{
public:
	CATLMapLayer();
	~CATLMapLayer();

	void ParseMap(CATLMapInfo* pMapInfo);

	virtual void draw(void);
	virtual void addChild(CCNode * child, int zOrder);
	virtual void visit(void);

private :
    float m_mapScale;
};

#endif