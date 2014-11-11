#include <stdlib.h>
#include <stdio.h>
#include "TileMap.h"
#include "ATLMapInfo.h"
#include "ATLMapLayer.h"

#include "../../../cocos2dx/cocos2dx/sprite_nodes/CCSprite.h"
#include "../../../cocos2dx/cocos2dx/include/ccMacros.h"

CTileMap::CTileMap()
{
	//this->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));
	//this->setRotationX(180);
}

CTileMap::~CTileMap()
{

}

void CTileMap::LoadLayer(const char* pFileName, int zOrder)
{
	CATLMapInfo info;
	info.Load(pFileName);
	CATLMapLayer* pLayer = new CATLMapLayer();
	pLayer->ParseMap(&info);
	//pLayer->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
	//pLayer->setRotationX(180);
	
	addChild(pLayer, zOrder);
}

void CTileMap::LoadBGLayer(const char* pFileName)
{
	LoadLayer(pFileName, 0);
}

void CTileMap::LoadDcorateLayer(const char* pFileName)
{
	LoadLayer(pFileName, 1);
}

void CTileMap::LoadPhysicsLayer(const char* pFileName)
{
	LoadLayer(pFileName, 2);
}


void CTileMap::draw(void)
{
	cocos2d::CCNode::draw();
}

void CTileMap::visit(void)
{
	cocos2d::CCNode::visit();
}