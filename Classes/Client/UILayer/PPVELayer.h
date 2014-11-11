//
//  PPVELayer.h
//  iSeer
//
//  Created by arthurgong on 11-12-8.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "PPVEMessage.h"

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"

using namespace std;
using namespace cocos2d;


//
// 管理PPVE战斗逻辑模块
//
class PPVELayer : public CCLayer
{    
public:
	typedef struct ScrollPagePosition
	{
		int index;
		int page;
		int row;
		int col;
	}ScrollPagePosition;

public:
	PPVELayer();
	virtual ~PPVELayer();

	CREATE_FUNC(PPVELayer);
	virtual bool init();

    bool   AddNewRoom(const PVERoomBase& room);
	
	/// clear all rooms
	void   ClearAllRooms();

	void   InitMaps();

	int		GetCurrentInstance(){return m_currentInstance;};

	virtual void closeLayerCallBack(void);

	bool IsOnCloseLayerCallBack(void){return m_bCloseLayerCallBack;}
    
    CC_SYNTHESIZE(int, m_FuBenLevel, FuBenLevel)

public:
	virtual void setTouchEnabled(bool value);


protected:
	/// callback of close button
	void   OnClose(CCObject *sender);	

	void   OnClickListItem(CCObject* sender);

	void   OnPressInstanceBtn(CCObject *sender);

	/// add single instance
	void   addSingleInstance(InstanceMapInfo info, ScrollPagePosition position);

	//辅助计算函数
	ScrollPagePosition getScrollPagePos(int index);

	CCPoint scrollPosToPoint(ScrollPagePosition position);

protected:
	/// room list
	UIScrollList* m_pList;

	TXGUI::UILayout* m_pLayer;

	/// all the maps
	MapInfoVector  m_vecMaps;

	int m_currentInstance;

	bool m_bCloseLayerCallBack;
};