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

#ifndef iSeer_UIScrollPage_h
#define iSeer_UIScrollPage_h

#include "TXGUI.h"
#include "UIControlBase.h"
#include "NiStream.h"
#include "UIScrollLayer.h"
//#include "CCSprite.h"

namespace TXGUI
{
	USING_NS_CC;

	class IconButton;

	class UIScrollPage : public UIControlBase
	{
		NiDeclareRTTI;
		NiDeclareStream;

	public:
		UIScrollPage(CCNode *pParentNode);
		virtual ~UIScrollPage();

		// position
		virtual void setPosition(CCPoint pt);

        virtual CCPoint getScrollLayerPosition();

		virtual void BindNode(CCNode* pParentNode);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual void setScrollLayer(UIScrollLayer *pScrollLayer);

		virtual UIScrollLayer *getScrollLayer();

		// direction
		virtual void setDirection(EScrollDirection value);

		// add pages
		virtual void addPages(CCArray* layers);
		virtual void addPage(CCLayer* page, bool changeScale = true);
		virtual void addPagesByChildren();
		virtual void addOneEmptyPage(int pageIndex);		

		// remove pages
		virtual void removeAllPages();
		
		// get current page index
		virtual int getCurPage();
		virtual int getPageCount();
		CCLayer *   getPageLayer(int page);

		virtual int getPageColumn(){return m_icolumn;}
		virtual int getPageRow(){return m_irow;}

		// move to page
		virtual void moveToPage(int pageIndex, bool animate = true);

		virtual void setContentSize(CCSize size);
		virtual CCSize getContentSize();

		virtual void setVisible(bool value);

		// register notification func while current page changed
		void registerPageChangedCallbackFunc(int handler);

		void unregisterPageChangedCallbackFunc();

		void registerPageChangedCallBackFunc(CCObject* target,SEL_CallFunc handler);

		void onReceivedPageChanged();
		// page item icon set/get function
		virtual void SetPageItemIcon(int iPage, int icolmun, int irow, string iconFile);

		void SetPageItemIconWithSprite(int iPage, int icolmun, int irow, CCSprite* isprite);
		/*
		* @function GetIconButton()
		* @param, iPage
		* @param, icolumn
		* @param, irow
		*/
		virtual IconButton* GetIconButton(int iPage, int icolmun, int irow);

		const std::vector<IconButton*>& GetIconButton(){return m_lstButton;}

		void setTouchStolenDistance(int var);
	protected:
		UIScrollPage();

		void  OnItemClicked(CCObject *pObject);

		void OnItemDoubleClicked(CCObject *pObject);

		void OnItemDragRelease(CCObject *pObject);

		void OnTapped(CCObject *pObject);
		void OnTapCancel(CCObject *pObject);

		bool dispatchTutorialMsg(CCObject* pObject,unsigned int type);
	protected:
		EScrollDirection m_direction;
		UIScrollLayer *m_pScrollLayer;

		/**determine if the page is wholy configed
		* that is to say using column and row
		* using this bool value in set scale
		*/
		bool m_bIsConfiged;
		
		string m_cellAsfile;
		string m_cellSelectedAsfile;
		uint32_t  m_icolumn;
		uint32_t  m_irow;
		uint32_t  m_ipageCount;
		CCRect    m_cellRect;
		uint32_t  m_celloffsetX;
		uint32_t  m_celloffsetY;

		int32_t m_indicatorOffsetX;
		int32_t m_indicatorOffsetY;
		CCSize m_contentSize;

		std::vector<IconButton*>m_lstButton;

		std::string m_strItemClickedFun;
		std::string m_strItemDoubleClickedFun;
		std::string m_strItemDragReleaseFun;
		std::string m_strTappedFun;
		std::string m_strTapCancelFun;
		unsigned int m_itemType;
		std::string m_cellBinFile;

		int m_pPageChangeHandlerEntry;
		CCObject* m_pPageChangedTarget;
		SEL_CallFunc m_pPageChangedHander;
	};
}

#endif //iSeer_UIScrollPage_h