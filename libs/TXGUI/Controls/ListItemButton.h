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

#pragma once

#include "IconButton.h"

namespace TXGUI
{
	class ListItemButton : public IconButton
	{
	public:
		ListItemButton(CCSprite* backgroundTex , CCSprite* normalTex, CCSprite* selectedTex,
			cocos2d::CCObject * handler, SEL_MenuHandler pfnClicked);		

		virtual~ListItemButton();

		/// attach one control to current ListItemButton
		bool	AttachControl(UIControlBase* pControl);

		/// detach one control from current ListItemButton
		void	DetachControl(UIControlBase* pControl);

	protected:
		ControlList m_lstChildren;
	};

	typedef list<ListItemButton*>ListItemButtonList;
}