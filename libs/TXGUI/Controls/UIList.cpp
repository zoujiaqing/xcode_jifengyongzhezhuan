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

#include <iostream>

#include "UIList.h"
#include "NiObject.h"
#include "CrossPlatform.h"
#include "ASpriteManager.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"
#include "UIManager.h"
#include "UICreator.h"

#include "GameFontManager.h"

namespace TXGUI
{
	USING_NS_CC;

	NiImplementRTTI(UIList, UIControlBase);
	NiImplementCreateObject(UIList);

	UIList::UIList(CCNode *pParentNode)
		: UIControlBase(pParentNode)
	{
		m_pScrollView = new UIScrollView();
		m_pScrollView->autorelease();
		m_pScrollView->setPosition(CCPointZero);
		m_ptStartOffset = CCPointZero;
		m_pBackground = NULL;
		m_pControlNode = m_pScrollView;
		m_direction = SWUIScrollViewDirectionHorizontal;

		pParentNode->addChild(m_pScrollView);

		/// click function
		m_strItemClickedFun = "";
		m_strItemDoubleClickedFun = "";
		m_strItemDragReleaseFun = "";
	}

	UIList::~UIList()
	{

	}

	void UIList::setPosition(CCPoint pt)
	{
		UIControlBase::setPosition(pt);

		if(m_pBackground != NULL)
		{
			m_pBackground->setPosition(pt);
		}

		if(m_pScrollView != NULL)
		{
			m_pScrollView->setPosition(pt);
		}
	}

	void UIList::setScale(float scale)
	{
		if(!m_bScaleSetted)
		{
			m_ptStartOffset = ccp(m_ptStartOffset.x * scale, m_ptStartOffset.y * scale);

			if(m_pScrollView != NULL)
			{
				m_pScrollView->setScale(scale);
			}
		}

		UIControlBase::setScale(scale);
        m_pScrollView->setContainerPos(m_ptStartOffset);

		if(m_pBackground != NULL)
		{
			m_pBackground->setScale(scale);
		}
	}

	void UIList::setScale(float xScale, float yScale)
	{
		if(!m_bScaleSetted)
		{
			m_ptStartOffset = ccp(m_ptStartOffset.x * xScale, m_ptStartOffset.y * yScale);

			if(m_pScrollView != NULL)
			{
				m_pScrollView->setScale(xScale, yScale);
			}
		}

		UIControlBase::setScale(xScale, yScale);
        m_pScrollView->setContainerPos(m_ptStartOffset);

		if(m_pBackground != NULL)
		{
			m_pBackground->setScaleX(xScale);
			m_pBackground->setScaleY(yScale);
		}
	}

	void UIList::setScrollView(UIScrollView *pScrollView)
	{
		if(pScrollView == NULL)
			return;

		if(m_pScrollView != NULL)
		{
			m_pScrollView->removeFromParentAndCleanup(true);
			m_pScrollView = NULL;
		}
        
		m_pScrollView = pScrollView;
		m_pControlNode = m_pScrollView;
		m_pControlNode->setPosition(m_ptLocalPosition);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pScrollView, m_fPosZ);
		}
	}

	void UIList::setBackground(CCSprite *pBackground)
	{
		if(pBackground == NULL)
			return;

		if(m_pBackground != NULL)
		{
			m_pBackground->removeFromParentAndCleanup(true);
			m_pBackground = NULL;
		}
        
		m_pBackground = pBackground;
		m_pBackground->setPosition(m_ptLocalPosition);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pBackground, m_fPosZ);
		}
	}

	void UIList::setStartOffset(CCPoint ptOffset)
	{
		m_ptStartOffset = ptOffset;

		if(m_pScrollView != NULL)
		{
			m_pScrollView->setContainerPos(ptOffset);
		}
	}

	void UIList::setViewSize(CCSize bSize)
	{
		if(m_pScrollView != NULL)
		{
			m_pScrollView->setViewSize(bSize);
		}
	}

	CCSize UIList::getViewSize()
	{
		if(m_pScrollView != NULL)
		{
			return m_pScrollView->getViewSize();
		}

		return CCSizeZero;
	}

	void UIList::setContentSize(CCSize bSize)
	{
		if(m_pScrollView != NULL)
		{
			m_pScrollView->setContentSize(bSize);
		}
	}

	CCSize UIList::getContentSize()
	{
		if(m_pScrollView != NULL)
		{
			return m_pScrollView->getContentSize();
		}

		return CCSizeZero;
	}

	CCPoint UIList::getContentOffset()
	{
		if(m_pScrollView != NULL)
		{
			return m_pScrollView->getContainerPos();
		}

		return CCPointZero;
	}

	void UIList::setDirection(SWUIScrollViewDirection value)
	{
		if(value < SWUIScrollViewDirectionHorizontal
			|| value > SWUIScrollViewDirectionBoth)
		{
			m_direction = SWUIScrollViewDirectionBoth;
		}
		else
		{
			m_direction = value;
		}

		if(m_pScrollView != NULL)
		{
			m_pScrollView->setDirection(m_direction);
		}
	}

	void UIList::setIsVisible(bool isVisible)
	{
		if(m_pScrollView != NULL)
		{
			m_pScrollView->setVisible(isVisible);
		}

		if(m_pBackground != NULL)
		{
			m_pBackground->setVisible(isVisible);
		}
	}

	void UIList::BindNode(CCNode *pParentNode)
	{
		UIControlBase::BindNode(pParentNode);

		if(m_pBackground)
		{
			pParentNode->addChild(m_pBackground, m_fPosZ);
			CCSize bgSize = m_pBackground->getContentSize();
			CCSize contentSize = getViewSize();

			if(m_direction == SWUIScrollViewDirectionVertical)
			{
				contentSize.height = 0;
				float delta_y = 5.0f;
				CCPoint itemPt = m_ptLocalPosition;
				itemPt.x = 0;
				itemPt.y += m_pScrollView->getViewSize().height / 2;

				/// bind list items
               
				setContentSize(contentSize);
			}
		}
	}

	void UIList::clearAllListItem()
	{
		CCSize contentSize = getViewSize();
		if(m_direction == SWUIScrollViewDirectionHorizontal)
		{
			contentSize.width = 0;
		}
		else if(m_direction == SWUIScrollViewDirectionVertical)
		{
			contentSize.height = 0;
		}
		setContentSize(contentSize);

		/// remove all list items
		ListItemButtonList::iterator itF = m_lsItems.begin();
		ListItemButtonList::iterator itE = m_lsItems.end();
		while(itF != itE)
		{
			ListItemButton* pitem = *itF;
			pitem->removeFromParentAndCleanup(true);
			itF++;
		}
		m_lsItems.clear();
	}

	ListItemButton* UIList::AddItem(CCSprite* spriteBkg, CCSprite* spriteSelect)
	{
		ListItemButton* pItem = new ListItemButton(spriteBkg, spriteBkg, spriteSelect, this,
			SEL_MenuHandler(&UIList::OnItemClicked) );

		pItem->autorelease();
		m_pControlNode->addChild(pItem);

		size_t n = m_lsItems.size();
		float y = m_szItemContent.height * n;
		float x = 0;
		pItem->setPosition(x, y);

		m_lsItems.push_back(pItem);
		return pItem;
	}

	void UIList::RemoveItem(ListItemButton* pItem, bool bCleanup /* = true */)
	{
		/// remove all list items
		ListItemButtonList::iterator itF = m_lsItems.begin();
		ListItemButtonList::iterator itE = m_lsItems.end();
		while(itF != itE)
		{			
			if (*itF == pItem)
			{
				m_lsItems.erase(itF);
				pItem->removeFromParentAndCleanup(bCleanup);				
				break;
			}
			itF++;
		}

		/// reorder all the items
		ReSizeAllItems();
	}

	void UIList::ReSizeAllItems()
	{
		ListItemButtonList::iterator itF = m_lsItems.begin();
		ListItemButtonList::iterator itE = m_lsItems.end();
		int i = 0;
		while(itF != itE)
		{
			ListItemButton * item = *itF;
			float y = m_szItemContent.height * i++;
			float x = 0;
			item->setPosition(x, y);
		}
	}

	// menu item clicked event callback
	void UIList::OnItemClicked(CCObject *pObject)
	{

		if(m_strItemClickedFun != "")
		{
			dispatchMenuEvent(m_strItemClickedFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
	UIList::UIList()
	{
		m_pControlNode = NULL;
		m_pScrollView = NULL;
		m_pBackground = NULL;
		m_direction = SWUIScrollViewDirectionHorizontal;
	}

	void UIList::LoadBinary(NiStream &kStream)
	{
		UIControlBase::LoadBinary(kStream);
		
		uint32_t viewWidth = 0;
		uint32_t viewHeight = 0;
		uint32_t contentOffsetX = 0;
		uint32_t contentOffsetY = 0;
		uint32_t contentWidth = 0;
		uint32_t contentHeight = 0;
		uint32_t direction = 0;
		uint32_t startX = 0;
		uint32_t startY = 0;

		mutableDic *dic = kStream.GetStreamData();
		
		kStream.getIntAttributeValue(dic, "viewWidth", viewWidth);
		kStream.getIntAttributeValue(dic, "viewHeight", viewHeight);
		kStream.getIntAttributeValue(dic, "contentOffsetX", contentOffsetX);
		kStream.getIntAttributeValue(dic, "contentOffsetY", contentOffsetY);
		kStream.getIntAttributeValue(dic, "contentWidth", contentWidth);
		kStream.getIntAttributeValue(dic, "contentHeight", contentHeight);
		kStream.getIntAttributeValue(dic, "direction", direction);
		kStream.getIntAttributeValue(dic, "startX", startX);
		kStream.getIntAttributeValue(dic, "startY", startY);
        
        viewWidth /= CC_CONTENT_SCALE_FACTOR();
        viewHeight /= CC_CONTENT_SCALE_FACTOR();
        contentWidth /= CC_CONTENT_SCALE_FACTOR();
        contentHeight /= CC_CONTENT_SCALE_FACTOR();
		m_ptLocalPosition = CCPointZero;
		mutableDic *bgDic = (mutableDic*)dic->objectForKey("background");
		if(bgDic)
		{
			// load ASpriteManger resource file
			ASprite* as = GetASpriteCfg(kStream, bgDic);               
			m_pBackground = GetASprite(kStream, bgDic, as, "name", m_ptWorldPosition);

			uint32_t useASposition = 0;
			if(kStream.getIntAttributeValue(bgDic, "useASposition", useASposition) && useASposition == 1)
			{
				CCPoint parentWorldPosition = CCPointZero;
				if(m_pParent != NULL)
				{
					parentWorldPosition = m_pParent->getWorldPosition();
				}

				m_ptLocalPosition.x = m_ptWorldPosition.x - parentWorldPosition.x;
				m_ptLocalPosition.y = m_ptWorldPosition.y - parentWorldPosition.y;
				viewWidth = m_pBackground->getContentSize().width / CC_CONTENT_SCALE_FACTOR();
				viewHeight = m_pBackground->getContentSize().height / CC_CONTENT_SCALE_FACTOR();
			}
			else
			{
				// we modified it, so reset it
				UIControlBase::CalWorldPos();
			}

			m_pBackground->setPosition(CCPointZero);
			m_pBackground->setVisible(m_bIsVisible);
		}
		
		setDirection((SWUIScrollViewDirection)direction);
		m_ptStartOffset = ccp((int)startX / CC_CONTENT_SCALE_FACTOR(), (int)startY / CC_CONTENT_SCALE_FACTOR());
        
		m_pScrollView = UIScrollView::viewWithViewSize(CCSize(viewWidth, viewHeight));
		m_pScrollView->setDirection(m_direction);
		m_pScrollView->setPosition(m_ptLocalPosition);
		m_pScrollView->setContentOffset(ccp((int)contentOffsetX / CC_CONTENT_SCALE_FACTOR(),
                                            (int)contentOffsetY / CC_CONTENT_SCALE_FACTOR()));
		m_pScrollView->setContentSize(CCSize(contentWidth, contentHeight));

		m_pControlNode = m_pScrollView;
		m_pControlNode->setVisible(m_bIsVisible);
		
		/// init list items template
		mutableDic *listDic = (mutableDic*)dic->objectForKey("ListItem");
		if (listDic)
		{
			// load ASpriteManger resource file
			ASprite *as =  GetASpriteCfg(kStream, listDic);		
			uint32_t nCount = 0;  kStream.getIntAttributeValue(listDic, "count", nCount);
			for(size_t i = 0; i < nCount; i++)
			{
				CCSprite* pSptNormal = GetASprite(kStream, listDic, as, "normal");
				CCSprite* pSptSel = GetASprite(kStream, listDic, as, "select");
				ListItemButton* item = AddItem(pSptNormal, pSptSel);
				item->setTag(i);
			}
		}
	}

	void UIList::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UIList::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
	}

	void UIList::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);
	}

	bool UIList::IsEqual(NiObject *pObject)
	{
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
	}
}