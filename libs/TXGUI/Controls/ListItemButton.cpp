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
#include "ListItemButton.h"

#include "UIDefine.h"
#include "UIMenu.h"
#include "ItemDragLayer.h"
#include "MainMenuLayer.h"
#include "DragReceiver.h"


const static float DELTA = 40.0f;
#define DELAY 1.0f

namespace TXGUI
{
	ListItemButton::ListItemButton(CCSprite* backgroundTex ,
		CCSprite* normalTex, 
		CCSprite* selectedTex, 
		cocos2d::CCObject * handler,
		SEL_MenuHandler pfnClicked) : IconButton(backgroundTex, normalTex, selectedTex, handler, pfnClicked)
	{
		
	}

	ListItemButton::~ListItemButton()
	{
		//// delete all the list item control which attached to it
		ControlListInterator itF = m_lstChildren.begin();
		ControlListInterator itE = m_lstChildren.end();
		while(itF != itE)
		{
			delete *itF;
			itF++;
		}
		m_lstChildren.clear();
	}

	bool ListItemButton::AttachControl(UIControlBase* pControl)
	{
		if (pControl == NULL)
			return false;

		ControlListInterator itF = m_lstChildren.begin();
		ControlListInterator itE = m_lstChildren.end();
		while(itF != itE)
		{
			if (*itF == pControl)
				return false;

			itF++;
		}

		m_lstChildren.push_back(pControl);
		CCNode* pNode = pControl->getCurrentNode();
	    this->addChild(pNode);

		return true;
	}

	void ListItemButton::DetachControl(UIControlBase* pControl)
	{
		ControlListInterator itF = m_lstChildren.begin();
		ControlListInterator itE = m_lstChildren.end();
		while(itF != itE)
		{
			if (*itF == pControl)
			{
				CCNode* pNode = pControl->getCurrentNode();
				this->removeChild(pNode, false);
				m_lstChildren.erase(itF);
				return;
			}

			itF++;
		}
	}

}