// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#include "OnlineNetworkManager.h"
#include "ChatToolbar.h"
#include <stdlib.h>
#include <stdio.h>

#include "XLogicManager.h"
#include "GameManager.h"
#include "UserData.h"
#include "LevelDefine.h"
#include "UIManager.h"
#include "TXGUIHeader.h"
#include "Define.h"
#include "GameFontManager.h"
#include "CrossUtil.h"
#include "LuaTinkerManager.h"

//// game message system header files here!
#include "GMessage.h"

using namespace TXGUI;

//////////////////////////////////////////////////////////////////////////
///  
///  
///

ChatToolbar::ChatToolbar()
{
	m_layout = NULL;
	m_chatContainer = NULL;
	m_functionContainer = NULL;
	m_extraContainer = NULL;
}

ChatToolbar::~ChatToolbar()
{
	UIManager::sharedManager()->RemoveUILayout("chatToolbar");
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


bool ChatToolbar::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();
	m_layout = manager->CreateUILayoutFromFile("UIplist/chatToolbar.plist",this, "chatToolbar", true);
	if(m_layout)
	{
		m_chatContainer = m_layout->FindChildObjectByName<UIContainer>("chatContainer");
		m_functionContainer = m_layout->FindChildObjectByName<UIContainer>("functionContainer");
		m_extraContainer = m_layout->FindChildObjectByName<UIContainer>("extraContainer");
	}

	scheduleOnce(schedule_selector(ChatToolbar::updateChatArea),1.0f);
	return true;
}

void ChatToolbar::updateChatArea(float dt)
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/ChatToolLayer.lua","InitSmallChatArea");
}

void ChatToolbar::setChatBarVisible(bool isChatVisible,bool isFunctionVisible,bool isExtraVisible)
{
	if(m_chatContainer)
	{
		m_chatContainer->setVisible(isChatVisible);
	}
	if(m_functionContainer)
	{
		m_functionContainer->setVisible(isFunctionVisible);
	}
	if(m_extraContainer)
	{
		m_extraContainer->setVisible(!isExtraVisible);
		if(isExtraVisible)
		{
			for(std::list<UIControlBase*>::iterator it = m_extraContainer->getChildren().begin();
				it != m_extraContainer->getChildren().end(); 
				it++)
			{
				 UIControlBase* pChild = *it;
				 if(pChild)
				 {
					 pChild->setVisible(false);
				 }
			} 
		}
	}
}

