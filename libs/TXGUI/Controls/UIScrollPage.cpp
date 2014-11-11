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

#include "UIScrollPage.h"
#include "UILayout.h"
#include "NiObject.h"
#include "CrossPlatform.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "AspriteManager.h"
#include "IconButton.h"
#include "UIManager.h"
#include "UICreator.h"
#include "TutorialsManager.h"
#include "script_support/CCScriptSupport.h"
#include "ItemManager.h"
 
namespace TXGUI
{
	USING_NS_CC;

	NiImplementRTTI(UIScrollPage, UIControlBase);
	NiImplementCreateObject(UIScrollPage);

	UIScrollPage::UIScrollPage(CCNode *pParentNode)
		: UIControlBase(pParentNode)
	{
		m_pScrollLayer = new UIScrollLayer();
		m_pScrollLayer->autorelease();
		m_pScrollLayer->setPosition(CCPointZero);
		m_pControlNode = m_pScrollLayer;
		m_direction = ESD_Horizontal;
		m_indicatorOffsetX = 0;
		m_indicatorOffsetY = 0;
		m_contentSize = CCSizeZero;

		// page item cell
		m_irow = m_icolumn = 1;
		m_ipageCount = 0;
		m_irow = m_icolumn = 1;
		m_celloffsetX = m_celloffsetY = 0;
		m_bIsConfiged = false;
		m_strItemClickedFun = "";
		m_strItemDoubleClickedFun = "";
		m_strItemDragReleaseFun = "";
		m_strTappedFun = "";
		m_strTapCancelFun = "";
		m_cellBinFile = "";
		m_pPageChangeHandlerEntry = 0;
		m_pPageChangedTarget = NULL;
		m_pPageChangedHander = NULL;

		pParentNode->addChild(m_pScrollLayer);
	}

	UIScrollPage::~UIScrollPage()
	{
		m_lstButton.clear();
		unregisterPageChangedCallbackFunc();
	}

	void UIScrollPage::setPosition(CCPoint pt)
	{
		UIControlBase::setPosition(pt);

		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->setPosition(pt);

			CCSize size = m_pScrollLayer->getContentSize();
			if(m_direction == ESD_Horizontal)
			{
				CCPoint indicatorPos = ccp(pt.x, pt.y - size.height / 2 - m_indicatorOffsetY);
				m_pScrollLayer->setPagesIndicatorPosition(indicatorPos);
				moveToPage(0,false);
			}
			else if(m_direction == ESD_Vertical)
			{
				CCPoint indicatorPos = ccp(pt.x - size.width / 2 - m_indicatorOffsetX, pt.y);
				m_pScrollLayer->setPagesIndicatorPosition(indicatorPos);
				moveToPage(0,false);
			}
		}
	}

    CCPoint UIScrollPage::getScrollLayerPosition()
    {
        CCPoint pos = this->getScrollLayer()->getPosition();
        return pos;
    }

	void UIScrollPage::BindNode(CCNode* pParentNode)
	{
		m_pBindNode = pParentNode;
		m_pParentNode = pParentNode;

		m_pScrollLayer->registerPageChangeCallback(this,callfunc_selector(UIScrollPage::onReceivedPageChanged));
		CCNode *pNode = m_pScrollLayer;
		pParentNode->addChild(pNode, m_fPosZ);

		std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
		for(; it != m_ChildrenControls.end(); it++)
		{
			(*it)->BindNode(pNode);
		}
	}

	void UIScrollPage::setScale(float scale)
	{
		if(!m_bScaleSetted)
		{
			m_indicatorOffsetX = m_indicatorOffsetX * scale;
			m_indicatorOffsetY = m_indicatorOffsetY * scale;
			m_contentSize = CCSize(m_contentSize.width * scale, m_contentSize.height * scale);

			if(m_pScrollLayer != NULL)
			{
				m_pScrollLayer->setScale(scale);

				if(m_bIsConfiged)
				{
					m_pControlNode->setScale(scale);
				}
			}
		}

		UIControlBase::setScale(scale);
	}

	void UIScrollPage::setScale(float xScale, float yScale)
	{
		if(!m_bScaleSetted)
		{
			m_indicatorOffsetX = m_indicatorOffsetX * xScale;
			m_indicatorOffsetY = m_indicatorOffsetY * yScale;
			m_contentSize = CCSize(m_contentSize.width * xScale, m_contentSize.height * yScale);

			if(m_pScrollLayer != NULL)
			{
				m_pScrollLayer->setScale(xScale, yScale);

				if(m_bIsConfiged)
				{
					m_pControlNode->setScaleX(xScale);
					m_pControlNode->setScaleY(yScale);
				}
			}
		}

		UIControlBase::setScale(xScale, yScale);
	}

	void UIScrollPage::setScrollLayer(UIScrollLayer *pScrollLayer)
	{
		if(pScrollLayer == NULL)
			return;

		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->removeFromParentAndCleanup(true);
			m_pScrollLayer = NULL;
		}
        
		m_pScrollLayer = pScrollLayer;
		m_pControlNode = m_pScrollLayer->getBaseLayer();
		m_pControlNode->setPosition(m_ptLocalPosition);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pScrollLayer, m_fPosZ);
		}
	}

	UIScrollLayer *UIScrollPage::getScrollLayer()
	{
		return m_pScrollLayer;
	}

	void UIScrollPage::setDirection(EScrollDirection value)
	{
		if(value < ESD_Horizontal || value > ESD_Vertical)
		{
			m_direction = ESD_Horizontal;
		}
		else
		{
			m_direction = value;
		}

		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->setDirection(m_direction);
		}
	}

	void UIScrollPage::setContentSize(CCSize size)
	{
		m_contentSize = size;

		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->setContentSize(size);
		}
	}

	CCSize UIScrollPage::getContentSize()
	{
		return m_contentSize;
	}

	// page item icon set/get function
	void UIScrollPage::SetPageItemIcon(int ipage, int icolumn, int irow, string iconFile)
	{
		if (ipage > (int) m_ipageCount || ipage < 0)
			return;

		if (icolumn >(int) m_icolumn || icolumn < 0)
			return;

		if (irow > (int) m_irow || irow < 0)
			return;

		CCSprite* elfIcon1 = CCSprite::createWithSpriteFrameName(iconFile.c_str()); 
		IconButton* pButton = m_lstButton[ipage * m_irow * m_icolumn + irow * m_icolumn + icolumn];		
		if (pButton)
		{
			pButton->SetNormalIcon(elfIcon1);
		}
	}

	void UIScrollPage::SetPageItemIconWithSprite(int ipage, int icolmun, int irow, CCSprite* isprite)
	{
		if (ipage > (int) m_ipageCount || ipage < 0)
			return;

		if (icolmun >(int) m_icolumn || icolmun < 0)
			return;

		if (irow > (int) m_irow || irow < 0)
			return;

		size_t index = ipage * m_irow * m_icolumn + irow * m_icolumn + icolmun;
		if(index < m_lstButton.size())
		{
			IconButton* pButton = m_lstButton[ipage * m_irow * m_icolumn + irow * m_icolumn + icolmun];		
			if (pButton)
			{
				pButton->SetNormalIcon(isprite);
			}
		}

	}

	IconButton* UIScrollPage::GetIconButton(int ipage, int icolumn, int irow)
	{
		if (ipage >= (int) m_ipageCount || ipage < 0)
			return NULL;

		if (icolumn >= (int) m_icolumn || icolumn < 0)
			return NULL;

		if (irow >= (int) m_irow || irow < 0)
			return NULL;

		IconButton* pButton = m_lstButton[ipage * m_irow * m_icolumn + irow * m_icolumn + icolumn];	
		return pButton;
	}

	void UIScrollPage::addPages(CCArray *layers)
	{
		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->addPages(layers);
		}
	}

	void UIScrollPage::addPage(CCLayer* page, bool changeScale)
	{
		if (m_pScrollLayer)
		{
			m_pScrollLayer->addPage(page, changeScale);
		}
	}

	void UIScrollPage::addOneEmptyPage(int pageIndex)
	{
		CCLayer* layer = CCLayer::create();
		layer->setAnchorPoint(ccp(0, 0));
		{
			int deltaX, deltaY;
			if(m_cellBinFile == "")
			{
				m_cellBinFile = KUI_BIN;
			}
			ASprite *as = AspriteManager::getInstance()->getAsprite(m_cellBinFile);
			int idx = getResourceIDByName(m_cellAsfile.c_str());
			int selectedId = getResourceIDByName(m_cellSelectedAsfile.c_str());
			CCSize buttonSize = as->getframeRect(idx).size;
//			float contentScale = CCDirector::sharedDirector()->getContentScaleFactor();
//			buttonSize = CCSizeMake(buttonSize.width * contentScale, buttonSize.height * contentScale);

			if (m_icolumn <= 1)
				deltaX = 0;
			else
				deltaX = (getContentSize().width - 2 * m_celloffsetX - buttonSize.width) / (m_icolumn - 1);

			if (m_irow <= 1)
				deltaY = 0;
			else
				deltaY = (getContentSize().height - 2 * m_celloffsetY - buttonSize.height) / (m_irow - 1);
			
			/////////////////////////////////////////////////////////////////////////////
			// row 
			// row
			for(int r = 0; r < (int)m_irow; r++)
			{
				for(int l = 0; l < (int) m_icolumn; l++)
				{
					// clone one ccsprite 
					CCPoint pt;
					CCSprite *pEmptyItem = as->getSpriteFromFrame_Middle( idx, 0, pt);
					CCSprite *pSelectedItem = as->getSpriteFromFrame_Middle(selectedId ,0,pt); 
					IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectedItem, this, 
						menu_selector(UIScrollPage::OnItemClicked));
					pIcon->autorelease();
					int index = pageIndex * m_icolumn * m_irow;
					index += r * m_icolumn + l;
					pIcon->SetButtonIndex(index+1);
					pIcon->setContentSize(pEmptyItem->getContentSize());
					if(m_itemType != 0)
					{
						pIcon->creatDragReceiver((DragHandlerType)m_itemType);
					}
					pIcon->SetDoubleClickHandler(this,menu_selector(UIScrollPage::OnItemDoubleClicked));
					pIcon->SetTappedHandler(this,menu_selector(UIScrollPage::OnTapped),menu_selector(UIScrollPage::OnTapCancel));
					pIcon->SetDragFlagTime(150);
					m_lstButton.push_back(pIcon);

					int x = l * deltaX + m_celloffsetX + buttonSize.width / 2 - getContentSize().width / 2 ;
					int y = getContentSize().height / 2 - (r * deltaY + m_celloffsetY + buttonSize.height / 2);

//					x /= contentScale;
//					y /= contentScale;

					pIcon->setPosition(ccp(x, y));

					char name[50];
					sprintf(name,"%s_%d",this->m_strName.c_str(),index+1);
					pIcon->SetButtonName(name);

					layer->addChild(pIcon);
				}
			}
		}

		addPage(layer);		
	}

	void UIScrollPage::addPagesByChildren()
	{
		if(!m_ChildrenControls.empty())
		{
			CCArray *layers = CCArray::createWithCapacity(m_ChildrenControls.size());

			for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				it != m_ChildrenControls.end(); it++)
			{
				if(dynamic_cast<UILayout*>(*it))
				{
					layers->addObject(static_cast<UILayout*>(*it)->getCurrentNode());
				}
			}

			addPages(layers);
		}
	}

	void UIScrollPage::removeAllPages()
	{
		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->removeAllPages();
		}

		if(!m_ChildrenControls.empty())
		{
			for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				it != m_ChildrenControls.end(); it++)
			{
				delete *it;
			}

			m_ChildrenControls.clear();
		}
	}

	int UIScrollPage::getCurPage()
	{
		if(m_pScrollLayer != NULL)
		{
			return m_pScrollLayer->getCurPage();
		}

		return -1;
	}

	int UIScrollPage::getPageCount()
	{
		if(m_pScrollLayer != NULL)
		{
			return m_pScrollLayer->getPageCount();
		}

		return -1;
	}

	CCLayer * UIScrollPage::getPageLayer(int page)
	{
		if(m_pScrollLayer != NULL)
		{
			return m_pScrollLayer->getPageLayer(page);
		}

		return NULL;
	}

	void UIScrollPage::moveToPage(int pageIndex, bool animate)
	{
		if(m_pScrollLayer != NULL)
		{
			m_pScrollLayer->moveToPage(pageIndex, animate);
		}
	}

	// menu item clicked event callback
    void UIScrollPage::OnItemClicked(CCObject *pObject)
	{
		// 转发新手教学消息
		if (false == dispatchTutorialMsg(pObject,1))
		{
			return ;
		}

		if(m_strItemClickedFun != "")
        {
			dispatchMenuEvent(m_strItemClickedFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	void UIScrollPage::OnItemDoubleClicked(CCObject *pObject)
	{
		// 转发新手教学消息
		if (false == dispatchTutorialMsg(pObject,2))
		{
			return ;
		}

		if(m_strItemDoubleClickedFun != "")
		{
			dispatchMenuEvent(m_strItemDoubleClickedFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	void UIScrollPage::OnItemDragRelease(CCObject *pObject)
	{
		if(m_strItemDragReleaseFun != "")
		{
			dispatchMenuEvent(m_strItemDragReleaseFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	void UIScrollPage::OnTapped(CCObject *pObject)
	{
		if (false == dispatchTutorialMsg(pObject,3))
		{
			return ;
		}

		if(m_strTappedFun != "")
		{
			dispatchMenuEvent(m_strTappedFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	void UIScrollPage::OnTapCancel(CCObject *pObject)
	{
		if(m_strTapCancelFun != "")
		{
			dispatchMenuEvent(m_strTapCancelFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	bool UIScrollPage::dispatchTutorialMsg(CCObject* pObject,unsigned int type)
	{
		UIMenu* menu = NULL;
		CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(pObject);
		if(menuItem)
		{
			CCNode* node = menuItem->getParent();
			menu = dynamic_cast<UIMenu*>(node);			
		}
		else
		{
			menu = dynamic_cast<UIMenu*>(pObject);
		}

		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			char name[50];
			sprintf(name,"%s_%d",this->m_strName.c_str(),button->GetButtonIndex());

			string eventName = ItemManager::Get()->getTurEventNameByButtonName(name);

			return TutorialsManager::Get()->HandleOneEvent(eventName.c_str(),type);
		}

		return true;
	}

	///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
	UIScrollPage::UIScrollPage()
	{
		m_pControlNode = NULL;
		m_pScrollLayer = NULL;
		m_direction = ESD_Horizontal;
		m_indicatorOffsetX = 0;
		m_indicatorOffsetY = 0;

		// page item cell
		m_strItemClickedFun = "";
		m_strItemDoubleClickedFun = "";
		m_strItemDragReleaseFun = "";
		m_irow = m_icolumn = 1;
		m_ipageCount = 0;
		m_celloffsetX = m_celloffsetY = 0;

		m_bIsConfiged = false;
		m_itemType = 0;
		m_pPageChangeHandlerEntry = 0;
		m_pPageChangedTarget = NULL;
		m_pPageChangedHander = NULL;
	}

	void UIScrollPage::LoadBinary(NiStream &kStream)
	{
		UIControlBase::LoadBinary(kStream);

		uint32_t contentWidth = 0;
		uint32_t contentHeight = 0;
		uint32_t direction = 0;
		int priority = kCCScrollTouchPriority;

		mutableDic *dic = kStream.GetStreamData();

		// get button click callback function
		kStream.getStringattributeValue(dic, "itemClickedCallback", m_strItemClickedFun);
		kStream.getStringattributeValue(dic, "itemDoubleClickedCallback", m_strItemDoubleClickedFun);
		kStream.getStringattributeValue(dic, "itemDragReleaseCallback", m_strItemDragReleaseFun);
		kStream.getStringattributeValue(dic, "tappedCallback", m_strTappedFun);
		kStream.getStringattributeValue(dic, "tapCancelCallback", m_strTapCancelFun);

		kStream.getIntAttributeValue(dic, "itemType", m_itemType);
		kStream.getIntAttributeValue(dic, "contentWidth", contentWidth);
		kStream.getIntAttributeValue(dic, "contentHeight", contentHeight);
		kStream.getIntAttributeValue(dic, "direction", direction);
		kStream.getSignedIntAttributeValue(dic, "indicatorOffsetX", m_indicatorOffsetX);
		kStream.getSignedIntAttributeValue(dic, "indicatorOffsetY", m_indicatorOffsetY);
        kStream.getSignedIntAttributeValue(dic,"priority",priority);

        contentWidth /= CC_CONTENT_SCALE_FACTOR();
        contentHeight /= CC_CONTENT_SCALE_FACTOR();
        m_indicatorOffsetX /= CC_CONTENT_SCALE_FACTOR();
        m_indicatorOffsetY /= CC_CONTENT_SCALE_FACTOR();
        
		setDirection((EScrollDirection)direction);

		// query frameRect property
		mutableDic* framedic = static_cast<mutableDic*>(dic->objectForKey("frameRect"));
		if( framedic )
		{
			string rectAsIndex = "";			
			kStream.getStringattributeValue(framedic, "rectAsIndex", rectAsIndex);	
			uint32_t  useAscontentRect = 0;
			kStream.getIntAttributeValue(framedic, "useAscontentRect", useAscontentRect);
			if (useAscontentRect)
			{
				CCPoint pt;
				string ASfile = KUI_BIN;
				kStream.getStringattributeValue(framedic, "binFile", ASfile); 
				ASprite *as = AspriteManager::getInstance()->getAsprite(ASfile);   

				int idx = getResourceIDByName(rectAsIndex.c_str());
				CCRect scrollRect = as->getframeRect(idx);
				CCPoint scrollCenter = as->GetFramePointMiddle(idx);

				// re-calculate the local position 
				CCPoint parentWorldPosition = CCPointZero;
				if(m_pParent != NULL)
				{
					parentWorldPosition = m_pParent->getWorldPosition();
				}

				m_ptLocalPosition.x = scrollCenter.x - parentWorldPosition.x;
				m_ptLocalPosition.y = scrollCenter.y - parentWorldPosition.y;

				setContentSize(scrollRect.size);
			}
		}
		else
		{
			setContentSize(CCSize(contentWidth, contentHeight));
		}

		m_pScrollLayer = UIScrollLayer::nodeWithNoLayer(m_contentSize, m_direction);
		m_pScrollLayer->setPosition(m_ptLocalPosition);
		m_pScrollLayer->setPagesIndicatorPosition(ccp(m_indicatorOffsetX, m_indicatorOffsetY));
		m_pScrollLayer->setPriority(priority);
		m_pScrollLayer->setVisible(m_bIsVisible);
		m_pControlNode = m_pScrollLayer->getBaseLayer();

		// add pages
		mutableDic* pageDic = static_cast<mutableDic*>(dic->objectForKey("page"));
		if( pageDic )
		{
			kStream.getIntAttributeValue(pageDic, "count", m_ipageCount); 
			kStream.getIntAttributeValue(pageDic, "column", m_icolumn); 
			kStream.getIntAttributeValue(pageDic, "row", m_irow); 
			kStream.getIntAttributeValue(pageDic, "cellOffsetX", m_celloffsetX); 
			kStream.getIntAttributeValue(pageDic, "cellOffsetY", m_celloffsetY);
            
            m_celloffsetX /= CC_CONTENT_SCALE_FACTOR();
            m_celloffsetY /= CC_CONTENT_SCALE_FACTOR();
            
			m_bIsConfiged = true;
		}

		m_cellAsfile = "";
		kStream.getStringattributeValue(dic, "cellAsIndex", m_cellAsfile);
		m_cellSelectedAsfile = "";
		kStream.getStringattributeValue(dic, "cellSelectedAsIndex", m_cellSelectedAsfile);
		if (m_cellAsfile.empty() == false)
		{
			CCPoint pt;
			m_cellBinFile = KUI_BIN;
			kStream.getStringattributeValue(dic, "binFile", m_cellBinFile); 
			ASprite *as = AspriteManager::getInstance()->getAsprite(m_cellBinFile);   
			int idx = getResourceIDByName(m_cellAsfile.c_str());
			m_cellRect = as->getframeRect(idx);
		}

		for(int i = 0 ; i < (int) m_ipageCount ; i ++)
		{
			addOneEmptyPage(i);
		}
	}

	void UIScrollPage::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UIScrollPage::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
	}

	void UIScrollPage::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);
	}

	bool UIScrollPage::IsEqual(NiObject *pObject)
	{
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
	}

	void UIScrollPage::registerPageChangedCallbackFunc(int handler)
	{
		unregisterPageChangedCallbackFunc();
		m_pPageChangeHandlerEntry = handler;
	}

	void UIScrollPage::unregisterPageChangedCallbackFunc()
	{
		m_pPageChangeHandlerEntry = 0;
	}

	void UIScrollPage::registerPageChangedCallBackFunc(CCObject* target,SEL_CallFunc handler)
	{
		m_pPageChangedTarget = target;
		m_pPageChangedHander = handler;
	}

	void UIScrollPage::onReceivedPageChanged()
	{
		if(m_pPageChangeHandlerEntry != 0)
		{
			CCLuaEngine::defaultEngine()->executeFunctionByHandler(m_pPageChangeHandlerEntry,0);
		}
		if(m_pPageChangedTarget != NULL && m_pPageChangedHander != NULL)
		{
			(m_pPageChangedTarget->*m_pPageChangedHander)();
		}
	}

	void UIScrollPage::setVisible(bool value)
	{
		if(m_pControlNode)
		{
			m_pControlNode->setVisible(value);
			//CCArray* array = m_pControlNode->getChildren();
			//CCObject* obj;
			//CCARRAY_FOREACH(array,obj)
			//{
			//	CCNode* node = dynamic_cast<CCNode*>(obj);
			//	if(node)
			//	{
			//		node->setVisible(false);
			//	}
			//}
		}
		if(m_pScrollLayer)
		{
			m_pScrollLayer->setVisible(value);
		}
	}

	void UIScrollPage::setTouchStolenDistance(int var)
	{
		if(m_pScrollLayer)
		{
			m_pScrollLayer->setTouchStolenDistance(var);
		}
	}
}