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

#include "TXGUI.h"
#include "UIControlBase.h"
#include "NiStream.h"
#include "UIScrollView.h"

#include "ListItemButton.h"

namespace TXGUI
{
	USING_NS_CC;
    
	class ListItemButton;

	class UIList : public UIControlBase
	{
		NiDeclareRTTI;
		NiDeclareStream;

	public:
		UIList(CCNode *pParentNode);
		virtual ~UIList();

	public:
		virtual void BindNode(CCNode *pNode);

		// position
		virtual void setPosition(CCPoint pt);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual void setScrollView(UIScrollView *pScrollView);

		virtual void setBackground(CCSprite *pBackground);

		virtual void setStartOffset(CCPoint ptOffset);

		virtual void setViewSize(CCSize bSize);

		virtual CCSize getViewSize();

		virtual void setContentSize(CCSize bSize);

		virtual CCSize getContentSize();

		virtual CCPoint getContentOffset();

		// direciton
		virtual void setDirection(SWUIScrollViewDirection value);

		virtual void	setIsVisible(bool isVisible);

		virtual void	clearAllListItem();

		//// add new item to current list
		ListItemButton*		AddItem(CCSprite* spriteBkg, CCSprite* spriteSelect);

		//// remove current list item 
		void	RemoveItem(ListItemButton* pItem, bool bCleanup = true);

	protected:
		UIList();
		
		//// reisze all items
		void	ReSizeAllItems();

		//// message calback of click item
		void  OnItemClicked(CCObject *pObject);

	protected:
		SWUIScrollViewDirection m_direction;
		UIScrollView *	m_pScrollView;
		CCSprite *		m_pBackground;
		CCPoint			m_ptStartOffset;
		CCSize	m_szItemContent;

		/// child list item
		ListItemButtonList m_lsItems;

		std::string m_strItemClickedFun;
		std::string m_strItemDoubleClickedFun;
		std::string m_strItemDragReleaseFun;
	};
}
