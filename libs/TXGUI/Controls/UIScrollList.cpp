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

#include "UIScrollList.h"
#include "NiObject.h"
#include "CrossPlatform.h"
#include "ASpriteManager.h"
#include "CCLuaEngine.h"
#include "UIButton.h"
#include "tolua++.h"
#include "GameResourceManager.h"
#include "UIManager.h"
#include "GameFontManager.h"

namespace TXGUI
{
	USING_NS_CC;

	NiImplementRTTI(UIScrollList, UIControlBase);
	NiImplementCreateObject(UIScrollList);

	UIScrollList::UIScrollList(CCNode *pParentNode)
		: UIControlBase(pParentNode)
	{
		m_pScrollView = new UIScrollView();
		m_pScrollView->autorelease();
		m_pScrollView->setPosition(CCPointZero);
		m_ptStartOffset = CCPointZero;
		m_pBackground = NULL;
		m_pControlNode = m_pScrollView;
		m_childTag = 0;
		m_deltaDistance = 0;
		m_direction = SWUIScrollViewDirectionHorizontal;
		m_pPageChangeHandlerEntry = 0;

		pParentNode->addChild(m_pScrollView);
	}

	UIScrollList::~UIScrollList()
	{

	}

	void UIScrollList::setPosition(CCPoint pt)
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

	void UIScrollList::setScale(float scale)
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

	void UIScrollList::setScale(float xScale, float yScale)
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

	void UIScrollList::setScrollView(UIScrollView *pScrollView)
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

	void UIScrollList::setBackground(CCSprite *pBackground)
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

	void UIScrollList::setStartOffset(CCPoint ptOffset)
	{
		m_ptStartOffset = ptOffset;

		if(m_pScrollView != NULL)
		{
			m_pScrollView->setContainerPos(ptOffset);
		}
	}

	void UIScrollList::setViewSize(CCSize bSize)
	{
		if(m_pScrollView != NULL)
		{
			m_pScrollView->setViewSize(bSize);
		}
	}

	CCSize UIScrollList::getViewSize()
	{
		if(m_pScrollView != NULL)
		{
			return m_pScrollView->getViewSize();
		}

		return CCSizeZero;
	}

	void UIScrollList::setContentSize(CCSize bSize)
	{
		if(m_pScrollView != NULL)
		{
			m_pScrollView->setContentSize(bSize);
		}
	}

	CCSize UIScrollList::getContentSize()
	{
		if(m_pScrollView != NULL)
		{
			return m_pScrollView->getContentSize();
		}

		return CCSizeZero;
	}

	CCPoint UIScrollList::getContentOffset()
	{
		if(m_pScrollView != NULL)
		{
			return m_pScrollView->getContainerPos();
		}

		return CCPointZero;
	}

	void UIScrollList::setDirection(SWUIScrollViewDirection value)
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

	void UIScrollList::setIsVisible(bool isVisible)
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

	void UIScrollList::BindNode(CCNode *pParentNode)
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
				float delta_y = m_deltaDistance;
				CCPoint itemPt = m_ptLocalPosition;
				itemPt.x = 0;
				itemPt.y += m_pScrollView->getViewSize().height / 2;

				std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				for(; it != m_ChildrenControls.end(); it++)
				{
					UIButton* button = dynamic_cast<UIButton*>(*it);
					if(button)
					{
						itemPt.y -= delta_y + button->getButtonSize().height / 2;
						(*it)->setPosition(itemPt);
						itemPt.y -= button->getButtonSize().height / 2;
						contentSize.height += button->getButtonSize().height + delta_y;
					}

				}
				setContentSize(contentSize);
			}
			else if(m_direction == SWUIScrollViewDirectionHorizontal)
			{
				contentSize.width = 0;
				float delta_x = m_deltaDistance;
				CCPoint itemPt = m_ptLocalPosition;
				itemPt.x += m_pScrollView->getViewSize().width / 2;
				itemPt.y = 0;

				std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				for(; it != m_ChildrenControls.end(); it++)
				{
					UIButton* button = dynamic_cast<UIButton*>(*it);
					if(button)
					{
						itemPt.x -= delta_x + button->getButtonSize().width / 2;
						(*it)->setPosition(itemPt);
						itemPt.x -= button->getButtonSize().width / 2;
						contentSize.width += button->getButtonSize().width + delta_x;
					}

				}
				setContentSize(contentSize);
			}
		}
	}

	bool UIScrollList::checkChildInView(UIControlBase* childControl)
	{
		if(childControl == NULL)
		{
			return false;
		}

		float scale = UIManager::sharedManager()->getScaleFactor();
		CCSize contentSize = getContentSize();
		CCSize viewSize = getViewSize();
		CCPoint currentOffset = getContentOffset();
		if(m_direction == SWUIScrollViewDirectionVertical)
		{
			if(getViewSize().height > getContentSize().height)
			{
				return true;
			}

			float buttonHeight = 0;
			CCPoint pt = CCPointZero;
			UIButton* button = dynamic_cast<UIButton*>(childControl);
			if(button != NULL)
			{
				buttonHeight = button->getButtonSize().height * scale;
				pt = button->getAnimationNode()->getPosition();
			}
			else
			{
				buttonHeight = childControl->getUIContentSize().height * scale;
				pt = childControl->getCurrentNode()->getPosition();
			}
			
			float btLeftEdge = contentSize.height - viewSize.height * 0.5f + pt.y/* - buttonHeight *0.5f*/;
			float btRightEdge = contentSize.height - viewSize.height * 0.5f + pt.y/* + buttonHeight *0.5f*/;
			float leftEdge = contentSize.height - currentOffset.y - viewSize.height;
			float rightEage = contentSize.height - currentOffset.y;
			if(btLeftEdge < leftEdge || btRightEdge > rightEage)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if(m_direction == SWUIScrollViewDirectionHorizontal)
		{
			if(getViewSize().width > getContentSize().width)
			{
				return true;
			}

			float buttonWidth = 0;
			CCPoint pt = CCPointZero;
			UIButton* button = dynamic_cast<UIButton*>(childControl);
			if(button != NULL)
			{
				buttonWidth = button->getButtonSize().width * scale;
				pt = button->getAnimationNode()->getPosition();
			}
			else
			{
				buttonWidth = childControl->getUIContentSize().width * scale;
				pt = childControl->getCurrentNode()->getPosition();
			}
				
			//CCPoint pt = childControl->getPosition();
			
			float btLeftEdge = contentSize.width - viewSize.width * 0.5f + pt.x/* - buttonWidth *0.5f*/;
			float btRightEdge = contentSize.width - viewSize.width * 0.5f + pt.x/* + buttonWidth *0.5f*/;
			float leftEdge = contentSize.width - currentOffset.x - viewSize.width;
			float rightEage = contentSize.width - currentOffset.x;
			if(btLeftEdge < leftEdge || btRightEdge > rightEage)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	UIButton* UIScrollList::AddNewItem(CCSprite * normalSprite, CCSprite* clickedSprite,  CCSprite* disableSprite,
		string strCallbackFun, string content, ccColor3B fontColor, int priority)
	{
		CCMenuItemSprite* button = CCMenuItemSprite::create(normalSprite, 
			clickedSprite, 
			disableSprite,
			this,                                                                          
			NULL);

		//int nTag = (int)(m_ChildrenControls.size());
		int nTag = m_childTag++;
		float z = 0;
		button->setTag(nTag);
		char szName[32] = "";
		sprintf(szName, "ListItem_%d", nTag);

		CCMenuItemLabel *label = NULL;

		if (content.empty() == false && content != "")
		{
			string font = KJLinXin;
			float largeFontSize = GameFontManager::largeFontSize();
			uint32_t fontSize = 1.5f *largeFontSize * CC_CONTENT_SCALE_FACTOR();
			fontSize /= CCDirector::sharedDirector()->getContentScaleFactor();

			CCLabelTTF* takeByLabel = CCLabelTTF::create(content.c_str(), font.c_str(), (float)fontSize);
			takeByLabel->setColor(fontColor);

			label = CCMenuItemLabel::create(
				takeByLabel, 
				this, 
				NULL
				);
			label->setTag(nTag);
		}

		UIButton* pItem = new UIButton(szName, nTag, z, this, button, label,priority, strCallbackFun);

		addItem(pItem);
		return pItem;
	}

	/*
	*@prototype, AddNewItem()
	*@param, normalSprite	- normal display texture
	*@param, clickedSprite	- clicked display texture
	*@param, content	- text content displayed
	*@note, add one new item 
	*/
	UIButton* UIScrollList::AddNewItem(CCSprite * normalSprite, CCSprite* clickedSprite, string strCallbackFun, 
		string content, ccColor3B fontColor, int priority)
	{
		//CCMenuItemSprite* button = CCMenuItemSprite::create(normalSprite, 
		//	clickedSprite, 
		//	this,                                                                          
		//	NULL);
		
		//int nTag = (int)(m_ChildrenControls.size());
		int nTag = m_childTag++;
		float z = 0;
		//button->setTag(nTag);
		char szName[32] = "";
		sprintf(szName, "ListItem_%d", nTag);

		CCMenuItemLabel *label = NULL;
				
		if (content.empty() == false && content != "")
		{
			string font = KJLinXin;
			float largeFontSize = GameFontManager::largeFontSize();
			uint32_t fontSize = 1.5f *largeFontSize * CC_CONTENT_SCALE_FACTOR();
			fontSize /= CCDirector::sharedDirector()->getContentScaleFactor();

			CCLabelTTF* takeByLabel = CCLabelTTF::create(content.c_str(), font.c_str(), (float)fontSize);
			takeByLabel->setColor(fontColor);

			label = CCMenuItemLabel::create(
				takeByLabel, 
				this, 
				NULL
				);
			label->setTag(nTag);
		}

 		UIButton* pItem = new UIButton(szName, nTag, z, this, normalSprite,clickedSprite, label,priority, strCallbackFun);

		addItem(pItem);
		return pItem;
	}

	void UIScrollList::removeItem(UIControlBase* item)
	{
		if(item != NULL)
		{
			float scale = UIManager::sharedManager()->getScaleFactor();
			CCSize contentSize = getContentSize();
			CCPoint itemPt = m_ptLocalPosition;

			int removeTag = item->getTag();
			CCSize itemSize = CCSizeZero;
			UIButton* button = dynamic_cast<UIButton*>(item);
			if(button)
			{
				itemSize = button->getButtonSize();
				itemSize.width = itemSize.width * scale;
				itemSize.height = itemSize.height * scale;
			}
			else
			{
				UIControlBase* control = dynamic_cast<UIControlBase*>(item);
				if(control)
				{
					itemSize = control->getUIContentSize();
				}
			}

			if(!itemSize.equals(CCSizeZero))
			{
				if(m_direction == SWUIScrollViewDirectionVertical)
				{
					float delta_y = m_deltaDistance * scale;
					for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
						it != m_ChildrenControls.end(); 
						it++)
					{
						UIControlBase* pChild = *it;
						if(pChild->getTag() > removeTag)
						{
							if(dynamic_cast<UIButton*>(pChild))
							{
								CCPoint oldPt = pChild->getPosition();
								pChild->setPosition(ccpAdd(oldPt,ccp(0,itemSize.height + delta_y)));
							}
							else
							{
								CCPoint oldPt = pChild->getCurrentNode()->getPosition();
								pChild->getCurrentNode()->setPosition(ccpAdd(oldPt,ccp(0,itemSize.height + delta_y)));
							}
							
						}
					} 
					contentSize.height -= itemSize.height + delta_y;
					setContentSize(contentSize);
				}
				else if(m_direction == SWUIScrollViewDirectionHorizontal)
				{
					float delta_x = m_deltaDistance * scale;
					for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
						it != m_ChildrenControls.end(); 
						it++)
					{
						UIControlBase* pChild = *it;
						if(pChild->getTag() > removeTag)
						{
							if(dynamic_cast<UIButton*>(pChild))
							{
								CCPoint oldPt = pChild->getPosition();
								pChild->setPosition(ccpAdd(oldPt,ccp(itemSize.width  +delta_x,0)));
							}
							else
							{
								CCPoint oldPt = pChild->getCurrentNode()->getPosition();
								pChild->getCurrentNode()->setPosition(ccpAdd(oldPt,ccp(itemSize.width +delta_x,0)));
							}

						}
					} 
					contentSize.width -= itemSize.width  + delta_x;
					setContentSize(contentSize);
				}
			}
			RemoveChild(item,true);
		}		
	}

	void UIScrollList::addItem(UIControlBase* item)
	{
		if(item != NULL)
		{
			if(m_direction == SWUIScrollViewDirectionVertical)
			{
				float scale = UIManager::sharedManager()->getScaleFactor();
				CCSize contentSize = getContentSize();
				float delta_y = m_deltaDistance;
				CCPoint itemPt = m_ptLocalPosition;
				itemPt.x = 0;
				itemPt.y = m_pScrollView->getViewSize().height / 2 - contentSize.height;
				UIButton* button = dynamic_cast<UIButton*>(item);
				if(button)
				{
					itemPt.y -= delta_y * scale + button->getButtonSize().height * scale / 2;
					button->setPosition(itemPt);
					itemPt.y -= button->getButtonSize().height * scale/ 2;
					contentSize.height += button->getButtonSize().height * scale + delta_y * scale;
					setContentSize(contentSize);
				}
				else
				{
					UIControlBase* control = dynamic_cast<UIControlBase*>(item);
					if(control)
					{
						itemPt.y -= delta_y * scale + control->getUIContentSize().height/ 2;
						if(control->getCurrentNode())
						{
							control->getCurrentNode()->setPosition(itemPt);
							itemPt.y -=control->getUIContentSize().height / 2;
							contentSize.height += control->getUIContentSize().height + delta_y * scale;
							setContentSize(contentSize);
						}
					}
				}
			}
			else if(m_direction == SWUIScrollViewDirectionHorizontal)
			{
				float scale = UIManager::sharedManager()->getScaleFactor();
				CCSize contentSize = getContentSize();
				float delta_x = m_deltaDistance;
				CCPoint itemPt = m_ptLocalPosition;
				itemPt.x = m_pScrollView->getViewSize().width / 2 - contentSize.width;
				itemPt.y = 0;
				UIButton* button = dynamic_cast<UIButton*>(item);
				if(button)
				{
					itemPt.x -= delta_x * scale + button->getButtonSize().width * scale / 2;
					button->setPosition(itemPt);
					itemPt.x -= button->getButtonSize().width * scale/ 2;
					contentSize.width += button->getButtonSize().width * scale + delta_x * scale;
					setContentSize(contentSize);
				}
				else
				{
					UIControlBase* control = dynamic_cast<UIControlBase*>(item);
					if(control)
					{
						itemPt.x -= delta_x * scale + control->getUIContentSize().width/ 2;
						if(control->getCurrentNode())
						{
							control->getCurrentNode()->setPosition(itemPt);
							itemPt.x -=control->getUIContentSize().width/ 2;
							contentSize.width += control->getUIContentSize().width + delta_x * scale;
							setContentSize(contentSize);
						}
					}
				}
			}
		}	
		m_pScrollView->checkChildrenPos();
	}

	UIControlBase* UIScrollList::AddBaseItem(CCSize size,CCNode* node)
	{
		//int nTag = (int)(m_ChildrenControls.size());
		int nTag = m_childTag++;
		float z = 0;
		char szName[32] = "";
		sprintf(szName, "ListItem_%d", nTag);
		UIControlBase* pControlBase = new UIControlBase(szName,nTag,0,this,size,node);
		addItem(pControlBase);
		return pControlBase;
	}

	void UIScrollList::clearChildrens()
	{
		m_childTag = 0;
		for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
			it != m_ChildrenControls.end(); 
			it++)
		{
			UIControlBase* pChild = *it;
			delete pChild;
		} 

		m_ChildrenControls.clear();
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
	}
	///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
	UIScrollList::UIScrollList()
	{
		m_childTag = 0;
		m_deltaDistance = 0;
		m_pControlNode = NULL;
		m_pScrollView = NULL;
		m_pBackground = NULL;
		m_direction = SWUIScrollViewDirectionHorizontal;
		m_pPageChangeHandlerEntry = 0;
	}

	void UIScrollList::LoadBinary(NiStream &kStream)
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
		m_deltaDistance = 5;
		int priority = kCCScrollTouchPriority;

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
        kStream.getSignedIntAttributeValue(dic,"priority",priority);
		kStream.getIntAttributeValue(dic,"deltaDistance",m_deltaDistance);

        viewWidth /= CC_CONTENT_SCALE_FACTOR();
        viewHeight /= CC_CONTENT_SCALE_FACTOR();
        contentWidth /= CC_CONTENT_SCALE_FACTOR();
        contentHeight /= CC_CONTENT_SCALE_FACTOR();
		m_ptLocalPosition = CCPointZero;
		mutableDic *bgDic = (mutableDic*)dic->objectForKey("background");
		if(bgDic)
		{
			// load ASpriteManger resource file
			ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
			mutableDic* dicAsMgr = static_cast<mutableDic*>(bgDic->objectForKey("AspriteManager"));
			// content attributes 
			if (dicAsMgr)
			{
				string ASfile = KUI_BIN;
				if (IsIpad())
				{
					kStream.getStringattributeValue(dicAsMgr, "ipad", ASfile); 
				}else
				{
					kStream.getStringattributeValue(dicAsMgr, "iphone", ASfile); 
				}
				as = AspriteManager::getInstance()->getAsprite(ASfile); 
			}          
               
			string bgName = "";
			kStream.getStringattributeValue(bgDic, "name", bgName);
			m_pBackground = as->getSpriteFromFrame_Middle(getResourceIDByName(bgName.c_str()), 0, m_ptWorldPosition);

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
		m_pScrollView->setisScrollList(true);
		m_pScrollView->setDirection(m_direction);
		m_pScrollView->setPosition(m_ptLocalPosition);
		m_pScrollView->setContentOffset(ccp((int)contentOffsetX / CC_CONTENT_SCALE_FACTOR(),
                                            (int)contentOffsetY / CC_CONTENT_SCALE_FACTOR()));
		m_pScrollView->setContentSize(CCSize(contentWidth, contentHeight));
		m_pScrollView->setPriority(priority);
		m_pScrollView->registerScrollMaxCallback(this,callfunc_selector(UIScrollList::onReceiveViewScrollMax));
		m_pControlNode = m_pScrollView;
		m_pControlNode->setVisible(m_bIsVisible);
	}

	void UIScrollList::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UIScrollList::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
	}

	void UIScrollList::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);
	}

	bool UIScrollList::IsEqual(NiObject *pObject)
	{
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
	}

	void UIScrollList::setTouchEnabled(bool value)
	{
		UIControlBase::setTouchEnabled(value);
		if(m_pScrollView)
		{
			m_pScrollView->setTouchEnabled(value);
		}
	}

	void UIScrollList::setCanReceiveScrollCallback(bool isReceive)
	{
		if(m_pScrollView)
		{
			m_pScrollView->setCanReceiveScrollCallback(isReceive);
		}
	}
	
	void UIScrollList::setScrollMaxHandler(int handler)
	{
		m_pPageChangeHandlerEntry = handler;
	}

	void UIScrollList::onReceiveViewScrollMax()
	{
		if(m_pPageChangeHandlerEntry != 0)
		{
			CCLuaEngine::defaultEngine()->executeFunctionByHandler(m_pPageChangeHandlerEntry,0);
		}
	}
}