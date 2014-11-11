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

#ifndef iSeer_UIScrollList_h
#define iSeer_UIScrollList_h

#include "TXGUI.h"
#include "UIControlBase.h"
#include "NiStream.h"
#include "UIScrollView.h"

namespace TXGUI
{
	USING_NS_CC;

	class UIButton;

	class UIScrollList : public UIControlBase
	{
		NiDeclareRTTI;
		NiDeclareStream;

	public:
		UIScrollList(CCNode *pParentNode);
		virtual ~UIScrollList();

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

		// 检查子节点是否在可见区域内，协助判定不可见区域内的按钮点击无效
		bool checkChildInView(UIControlBase* childControl);

		void setCanReceiveScrollCallback(bool isReceive);
		void setScrollMaxHandler(int handler);
		void onReceiveViewScrollMax();

		virtual void setTouchEnabled(bool value);
		/*
		*@prototype, AddNewItem()
		*@param, normalSprite	- normal display texture
		*@param, clickedSprite	- clicked display texture
		*@param, strCallbackFun - clicked callback function
		*@param, content	- text content displayed
		*@note, add one new item 
		*/
		virtual UIButton* AddNewItem(CCSprite * normalSprite, 
			CCSprite* clickedSprite,
			string strCallbackFun,
			string content,
			ccColor3B fontColor, 
			int priority = kCCScrollMenuPriority);

		virtual UIButton* AddNewItem(CCSprite * normalSprite, 
			CCSprite* clickedSprite,
			CCSprite* disableSprite,
			string strCallbackFun,
			string content,
			ccColor3B fontColor, 
			int priority = kCCScrollMenuPriority);

		virtual UIControlBase* AddBaseItem(CCSize size,CCNode* node);

		// direciton
		virtual void setDirection(SWUIScrollViewDirection value);

		virtual void setIsVisible(bool isVisible);

		virtual void clearChildrens();

		void removeItem(UIControlBase* item);
	protected:
		UIScrollList();
		
		/// add one button
		virtual void addItem(UIControlBase* item);

	protected:
		SWUIScrollViewDirection m_direction;
		UIScrollView *m_pScrollView;
		CCSprite *m_pBackground;
		CCPoint m_ptStartOffset;
		int m_childTag;
		unsigned int m_deltaDistance;
		//vector<UIButton*> m_arrButtonItems;
		int m_pPageChangeHandlerEntry;
	};
}

#endif //iSeer_UIScrollList_h