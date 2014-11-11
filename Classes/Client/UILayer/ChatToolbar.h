//
//  PPVELayer.h
//  iSeer
//
//  Created by arthurgong on 11-12-8.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;
using namespace TXGUI;
//
// 管理游戏中的快捷工具栏及聊天工具栏
//
class ChatToolbar : public CCLayer
{    
public:
	ChatToolbar();
	virtual ~ChatToolbar();

	CREATE_FUNC(ChatToolbar);
	virtual bool init();	

	void updateChatArea(float dt);

	void setChatBarVisible(bool isChatVisible,bool isFunctionVisible,bool isExtraVisible);
	// 显示或隐藏底部的聊天框，isChatVisible为左侧的聊天框，isFunctionVisible为右侧的工具栏,isExtraVisible为额外的功能键
protected:
	/// callback of close button

private:
	UILayout* m_layout;
	UIContainer* m_chatContainer;
	UIContainer* m_functionContainer;
	UIContainer* m_extraContainer;
};