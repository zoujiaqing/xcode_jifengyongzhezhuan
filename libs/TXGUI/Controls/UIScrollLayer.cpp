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

#include "AspriteManager.h"
#include <iostream>
#include "UIScrollLayer.h"
#include <cmath>
#include "UIDefine.h"
#include "UICreator.h"
#include "UIManager.h"

#define MIN_MOVE_DELTA 40
#define CHANGE_PAGE_LENGTH_RATE 4

namespace TXGUI
{
	USING_NS_CC;

	// ´¥ÃþÎó²î
	const int TOUCH_DELTA = 20;

	UIScrollLayer::UIScrollLayer()
	{
		m_Page = 0;
		m_CurPage = 0;
		m_showPage = 0;
		m_fMinimumTouchLengthToChangePage = 20.0f;
		m_bShouldStolenTouch = false;
		m_ScrollStatus = ESS_IDLE;
		m_localScale = 1;
		m_pageChangeTarget = NULL;
		m_pageChangeHandler = NULL;
		m_bPriority = kCCScrollTouchPriority;
		m_bTouchStolenDistance = TOUCH_DELTA;
	}

	UIScrollLayer::~UIScrollLayer()
	{
		if (m_PageLayer)
		{
			m_PageLayer->release();
		}
	}


	UIScrollLayer* UIScrollLayer::nodeWithNoLayer(CCSize pageSize, EScrollDirection dir)
	{
		UIScrollLayer* pRet = new UIScrollLayer();

		if(pRet && pRet->initWithNoLayer(pageSize, dir))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return pRet;
		}
	}

	UIScrollLayer* UIScrollLayer::nodeWithLayers(CCArray* layers, CCSize pageSize, EScrollDirection dir)
	{
		UIScrollLayer* pRet = new UIScrollLayer();
		if (pRet && pRet->initWithLayers(layers, pageSize, dir))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

	bool UIScrollLayer::initWithNoLayer(CCSize pageSize, EScrollDirection dir)
	{
		bool bRet = false;

		do {
			CC_BREAK_IF(!CCLayer::init());
			m_ScrollDir = dir;

			m_PageLayer = CCArray::create();
			m_PageLayer->init();
			m_PageLayer->retain();

			m_baseLayer = CCLayer::create();
			m_baseLayer->setAnchorPoint(CCPointZero);
			this->addChild(m_baseLayer);

			setTouchEnabled(true);

			this->setContentSize(pageSize);
			
			if(m_ScrollDir == ESD_Horizontal)
			{
				m_fMinimumTouchLengthToChangePage = m_obContentSize.width / CHANGE_PAGE_LENGTH_RATE;
			}
			else if (m_ScrollDir == ESD_Vertical)
			{
				m_fMinimumTouchLengthToChangePage = m_obContentSize.height / CHANGE_PAGE_LENGTH_RATE;
			}

			// Show indicator by default.
			m_bShowPagesIndicator = true;
			m_tPagesIndicatorPosition = ccp(0.5f * m_obContentSize.width, ceilf(m_obContentSize.height / 8.0f));
		
			bRet = true;
		} while(0);

		return bRet;
	}

	bool UIScrollLayer::initWithLayers(CCArray* layers, CCSize pageSize, EScrollDirection dir)
	{
		bool bRet = false;
    
		do {
			CC_BREAK_IF(!CCLayer::init());
			CC_ASSERT(layers && layers->count());
        
			m_ScrollDir = dir;
        
			m_PageLayer = CCArray::createWithCapacity(layers->count());
			m_PageLayer->retain();
        
			m_baseLayer = CCLayer::create();
			m_baseLayer->setAnchorPoint(CCPointZero);
			this->addChild(m_baseLayer);
        
			setTouchEnabled(true);
        
			this->setContentSize(pageSize);
			for (int i = 0; i < (int)layers->count(); i++)
			{
				CCLayer* layer = (CCLayer*)layers->objectAtIndex(i);
				addPage(layer);
			}

			if(m_ScrollDir == ESD_Horizontal)
			{
				m_fMinimumTouchLengthToChangePage = m_obContentSize.width / CHANGE_PAGE_LENGTH_RATE;
			}
			else if (m_ScrollDir == ESD_Vertical)
			{
				m_fMinimumTouchLengthToChangePage = m_obContentSize.height / CHANGE_PAGE_LENGTH_RATE;
			}
        
			// Show indicator by default.
			m_bShowPagesIndicator = true;
			m_tPagesIndicatorPosition = ccp(0.5f * m_obContentSize.width, ceilf(m_obContentSize.height / 8.0f));
			 
        
			bRet = true;
		} while (0);
    
		return bRet;
	}

	void UIScrollLayer::visit()
	{
		checkLayersVisiable();
		glEnable(GL_SCISSOR_TEST);
		
        float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
        
		glScissor(m_CenterWorldPos.x * deviceScale - m_obContentSize.width / 2 * deviceScale,
                  m_CenterWorldPos.y * deviceScale - m_obContentSize.height / 2 * deviceScale,
                  m_obContentSize.width * deviceScale,
                  m_obContentSize.height * deviceScale);


		CCLayer::visit();
		glDisable(GL_SCISSOR_TEST);
	}

	// Register with more priority than CCMenu's but don't swallow touches
	void UIScrollLayer::registerWithTouchDispatcher()
	{	
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_bPriority, false);
	}

	bool UIScrollLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		CCPoint touchPoint = pTouch->getLocationInView();
		m_TouchDownPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

		if(!touchIsInContent(m_TouchDownPoint) || !m_bVisible)
		{
			return false;
		}		
		
		for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
		{
			if (c->isVisible() == false)
			{
				return false;
			}
		}

		m_TouchCurPoint = m_TouchDownPoint;
		m_ScrollStatus = ESS_TOUCHED;
		//CCLuaLog("UIScrollLayer::ccTouchBegan: true");
		return true;
	}

	void UIScrollLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		// ÒÆ¶¯
		CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());

		if(!touchIsInContent(touchPoint) || !m_bVisible)
		{
			return;
		}		

		for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
		{
			if (c->isVisible() == false)
			{
				return;
			}
		}

		CCPoint ptbaselayer = m_baseLayer->getPosition();

		if (m_ScrollDir == ESD_Horizontal)
		{
			CCPoint posPoint = CCPointMake(m_baseLayer->getPosition().x + touchPoint.x - m_TouchCurPoint.x, ptbaselayer.y);

			if(abs(touchPoint.x - m_TouchCurPoint.x) > m_bTouchStolenDistance * m_localScale)
			{
				m_bShouldStolenTouch = true;
			}
			if(abs(touchPoint.x - m_TouchDownPoint.x) > MIN_MOVE_DELTA * m_localScale)
			{
				m_baseLayer->setPosition(posPoint);
			}
		}
		else
		{
			CCPoint posPoint = CCPointMake(ptbaselayer.x, m_baseLayer->getPosition().y + touchPoint.y - m_TouchCurPoint.y);

			if(abs(touchPoint.y - m_TouchCurPoint.y) > m_bTouchStolenDistance * m_localScale)
			{
				m_bShouldStolenTouch = true;
			}
			if(abs(touchPoint.y - m_TouchDownPoint.y) > MIN_MOVE_DELTA * m_localScale)
			{
				m_baseLayer->setPosition(posPoint);
			}
		}
    
		m_TouchCurPoint = touchPoint;
		m_ScrollStatus = ESS_DRAGGING;
	}

	void UIScrollLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{ 
		//CCLuaLog("UIScrollLayer::ccTouchEnded");
		    //if (offset >= (TOUCH_DELTA * TOUCH_DELTA)) {
			int offset = 0;
			if (m_ScrollDir == ESD_Horizontal)
			{
				offset = m_baseLayer->getPosition().x + m_CurPage * m_obContentSize.width - m_ptCenter.x;
			}
			else
			{
				//offset =  m_ptCenter.y - (m_baseLayer->getPosition().y + m_CurPage * m_obContentSize.height);
				offset = m_ptCenter.y + m_CurPage * m_obContentSize.height - m_baseLayer->getPosition().y;
			}

			if (offset > m_fMinimumTouchLengthToChangePage) {
				if (m_CurPage > 0) {
					--m_CurPage;
				}
			}
			else if (offset <- m_fMinimumTouchLengthToChangePage) {
				if (m_CurPage < (m_Page - 1)) {
					++m_CurPage;
				}
			}

			m_ScrollStatus = ESS_IDLE;
        
			goToPage();

			if(m_bShouldStolenTouch)
			{
				cancelAndStoleTouch(pTouch, pEvent);
				m_bShouldStolenTouch = false;
			}
		//}
	}

	void UIScrollLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		//CCLuaLog("UIScrollLayer::ccTouchCancelled");
		m_ScrollStatus = ESS_IDLE;
	}

	/** Hackish stuff - stole touches from other CCTouchDispatcher targeted delegates. 
	 Used to claim touch without receiving ccTouchBegan. */
	void UIScrollLayer::claimTouch(CCTouch* pTouch)
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		CCTargetedTouchHandler* handler = (CCTargetedTouchHandler*)pDispatcher->findHandler(this);		
		if (handler)
		{
			CCSet* claimedTouches = handler->getClaimedTouches();
			if (!claimedTouches->containsObject(pTouch))
			{
				claimedTouches->addObject(pTouch);
			}
			else 
			{
				CCLOGERROR("CCScrollLayer::claimTouch is already claimed!");
			}
		}
	}

	void UIScrollLayer::cancelAndStoleTouch(CCTouch* pTouch, CCEvent* pEvent)
	{
		// Throw Cancel message for everybody in TouchDispatcher.
		CCSet* touchSet = new CCSet();
		touchSet->addObject(pTouch);
		touchSet->autorelease();
		CCDirector::sharedDirector()->getTouchDispatcher()->touchesCancelled(touchSet, pEvent);
    
		//< after doing this touch is already removed from all targeted handlers
    
		// Squirrel away the touch
		claimTouch(pTouch);
	}

	int UIScrollLayer::getCurPage()
	{
		if(m_Page == 0)
		{
			return -1;
		}

		return m_CurPage;
	}

	int UIScrollLayer::getPageCount()
	{
		return m_Page;
	}

	CCLayer * UIScrollLayer::getPageLayer(int page)
	{
		if(page < 0 || page >= m_Page)
		{
			return NULL;
		}		
		return (CCLayer *)m_PageLayer->objectAtIndex(page);		
	}

	void UIScrollLayer::moveToPage(int page, bool animated)
	{
		if(page < 0 || page >= m_Page)
		{
			return;
		}

		if(m_ScrollStatus != ESS_IDLE)
		{
			return;
		}

		CCSize contentSize = getContentSize();

		if(animated)
		{
			if (m_ScrollDir == ESD_Horizontal)
			{				
				CCMoveTo *moveTo = CCMoveTo::create(0.2f, CCPointMake(-page * contentSize.width  + m_ptCenter.x, m_ptCenter.y));
				//m_baseLayer->runAction(moveTo);
				m_baseLayer->runAction(CCSequence::create(moveTo,CCCallFuncN::create(this,callfuncN_selector(UIScrollLayer::onMoveEnd)),NULL));
			}
			else
			{
				CCMoveTo *moveTo = CCMoveTo::create(0.2f, CCPointMake(m_ptCenter.x , page * contentSize.height  + m_ptCenter.y));
				//m_baseLayer->runAction(moveTo);
				m_baseLayer->runAction(CCSequence::create(moveTo,CCCallFuncN::create(this,callfuncN_selector(UIScrollLayer::onMoveEnd)),NULL));
			}
		}
		else
		{
			if (m_ScrollDir == ESD_Horizontal)
			{
				m_baseLayer->setPosition(CCPointMake(-page * contentSize.width + m_ptCenter.x, m_ptCenter.y));
			}
			else
			{
				m_baseLayer->setPosition(CCPointMake(m_ptCenter.x, page * contentSize.height + m_ptCenter.y));
			}
		}

		m_CurPage = page;
		checkChildrenPos();
		clearIndicators();
		addIndicators(m_ScrollDir);

		if(m_showPage != m_CurPage)
		{
			if(m_pageChangeTarget != NULL && m_pageChangeHandler != NULL)
			{
				(m_pageChangeTarget->*m_pageChangeHandler)();
			}
			m_showPage = m_CurPage;
		}
	}

	void UIScrollLayer::goToPage(bool animated)
	{
		//CCSize pageSize = this->getContentSize();
		CCSize contentSize = getContentSize();

		if(animated)
		{
			if (m_ScrollDir == ESD_Horizontal)
			{				
				CCMoveTo *moveTo = CCMoveTo::create(0.2f, CCPointMake(-m_CurPage * contentSize.width  + m_ptCenter.x, m_ptCenter.y));
				//m_baseLayer->runAction(moveTo);
				m_baseLayer->runAction(CCSequence::create(moveTo,CCCallFuncN::create(this,callfuncN_selector(UIScrollLayer::onMoveEnd)),NULL));
			}
			else
			{
				CCMoveTo *moveTo = CCMoveTo::create(0.2f, CCPointMake(m_ptCenter.x , m_CurPage * contentSize.height  + m_ptCenter.y));
				//m_baseLayer->runAction(moveTo);
				m_baseLayer->runAction(CCSequence::create(moveTo,CCCallFuncN::create(this,callfuncN_selector(UIScrollLayer::onMoveEnd)),NULL));
			}
		}
		else
		{
			if (m_ScrollDir == ESD_Horizontal)
			{
				m_baseLayer->setPosition(CCPointMake(-m_CurPage * contentSize.width + m_ptCenter.x, m_ptCenter.y));
			}
			else
			{
				m_baseLayer->setPosition(CCPointMake(m_ptCenter.x, m_CurPage * contentSize.height + m_ptCenter.y));
			}
		}
		checkChildrenPos();
		clearIndicators();
		addIndicators(m_ScrollDir);

		if(m_showPage != m_CurPage)
		{
			if(m_pageChangeTarget != NULL && m_pageChangeHandler != NULL)
			{
				(m_pageChangeTarget->*m_pageChangeHandler)();
			}
			m_showPage = m_CurPage;
		}
	}

	void UIScrollLayer::addPage(cocos2d::CCLayer *pPageLayer, bool changeScale)
	{
		if (pPageLayer) {
			CCSize pageSize = this->getContentSize();
			if (changeScale)
			{
				pageSize.width /= m_localScale;
				pageSize.height /= m_localScale;
			}
			
			//CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
			pPageLayer->setContentSize(pageSize);
			pPageLayer->setPosition(CCPointZero);
			if (m_ScrollDir == ESD_Horizontal)
			{
			    pPageLayer->setPosition(ccp(pageSize.width * m_Page, 0));
			}
			else
			{
			    pPageLayer->setPosition(ccp(0, -pageSize.height * m_Page));
			}

			//by benyang: avoid re-add in some situation
			if(pPageLayer->getParent() != NULL && pPageLayer->getParent() != m_baseLayer)
			{
				pPageLayer->removeFromParentAndCleanup(true);
			}

			if(pPageLayer->getParent() == NULL)
			{
				m_baseLayer->addChild(pPageLayer);
			}

			m_PageLayer->addObject(pPageLayer);
			m_Page = m_PageLayer->count();
			checkChildrenPos();
		}
	}

	void UIScrollLayer::addPages(cocos2d::CCArray *layers)
	{
		if(layers)
		{
			for(int i = 0; i < (int)layers->count(); i++)
			{
				CCLayer* layer = (CCLayer*)layers->objectAtIndex(i);
				addPage(layer);
			}
		}
	}

	void UIScrollLayer::removeAllPages()
	{
		if(m_baseLayer)
		{
			m_baseLayer->removeAllChildrenWithCleanup(true);
			m_PageLayer->removeAllObjects();
			m_Page = 0;
			m_CurPage = 0;
			m_showPage = 0;
			goToPage(false);
		}
	}
    
	bool UIScrollLayer::touchIsInContent(CCPoint point)
	{
     
		if(point.x > m_CenterWorldPos.x - m_obContentSize.width / 2
				&& point.x < m_CenterWorldPos.x + m_obContentSize.width / 2
				&& point.y > m_CenterWorldPos.y - m_obContentSize.height / 2
				&& point.y < m_CenterWorldPos.y + m_obContentSize.height / 2
			)
			{
				return true;
			}
		else
		{
			return false;
		}
	}

	void UIScrollLayer::setDirection(EScrollDirection direction)
	{
		m_ScrollDir = direction;
	}

	CCLayer *UIScrollLayer::getBaseLayer()
	{
		return m_baseLayer;
	}

	void UIScrollLayer::setPosition(CCPoint point)
	{
	   // set base layer's position 
		CCSize size = getContentSize();
		m_CenterWorldPos = convertToWorldSpace(point);
		m_ptCenter = point;
		m_ptOrigio.x = point.x - size.width / 2;
		m_ptOrigio.y = point.y - size.height / 2;

		m_baseLayer->setPosition(m_ptCenter);
	}
	
    CCPoint UIScrollLayer::getPosition()
    {
        return m_baseLayer->getPosition();
    }

	void UIScrollLayer::setScale(float scale)
	{
		m_localScale = scale;
		m_CenterWorldPos = ccp(m_CenterWorldPos.x * scale, m_CenterWorldPos.y * scale);
		CCSize size = getContentSize();
		setContentSize(CCSize(size.width * scale, size.height * scale));			

		if(m_ScrollDir == ESD_Horizontal)
		{
			m_fMinimumTouchLengthToChangePage = m_obContentSize.width / CHANGE_PAGE_LENGTH_RATE;
		}
		else if (m_ScrollDir == ESD_Vertical)
		{
			m_fMinimumTouchLengthToChangePage = m_obContentSize.height / CHANGE_PAGE_LENGTH_RATE;
		}
	}

	void UIScrollLayer::setScale(float xScale, float yScale)
	{
		m_CenterWorldPos = ccp(m_CenterWorldPos.x * xScale, m_CenterWorldPos.y * yScale);
		CCSize size = getContentSize();
		setContentSize(CCSize(size.width * xScale, size.height * yScale));			

		if(m_ScrollDir == ESD_Horizontal)
		{
			m_fMinimumTouchLengthToChangePage = m_obContentSize.width / CHANGE_PAGE_LENGTH_RATE;
		}
		else if (m_ScrollDir == ESD_Vertical)
		{
			m_fMinimumTouchLengthToChangePage = m_obContentSize.height / CHANGE_PAGE_LENGTH_RATE;
		}
	}

	void UIScrollLayer::checkLayersVisiable()
	{
		CCSize contentSize = getContentSize();
		bool isMoving = false;
		float offset = 0;
		if (m_ScrollDir == ESD_Horizontal)
		{
			offset = m_baseLayer->getPosition().x + m_CurPage * m_obContentSize.width - m_ptCenter.x;
		}
		else
		{
			offset = m_baseLayer->getPosition().y + m_CurPage * m_obContentSize.height - m_ptCenter.y;
		}

		if(std::abs(offset) < 1.0)
		{
			isMoving = false;
		}
		else
		{
			isMoving = true;
		}
		/*if((ptbaselayer.x - m_ptCenter.x) % contentSize.width != 0)
		{
			isMoving = true;
		}
		if((ptbaselayer.y - m_ptCenter.y) % contentSize.width != 0)
		{
			isMoving = true;
		}*/

		if(m_PageLayer)
		{
			for(int i = 0; i < (int)m_PageLayer->count(); i++)
			{
				CCLayer* layer = (CCLayer*)m_PageLayer->objectAtIndex(i);
				if(layer)
				{
					if(isMoving)
					{
						if(i >= m_CurPage-1 && i<= m_CurPage +1)
						{
							layer->setVisible(true);
						}
						else
						{
							layer->setVisible(false);
						}
					}
					else
					{
						if(i == m_CurPage)
						{
							layer->setVisible(true);
						}
						else
						{
							layer->setVisible(false);
						}
					}
				}
			}
		}
	}

	void UIScrollLayer::onMoveEnd(CCNode* obj)
	{
		checkChildrenPos();
	}

	void UIScrollLayer::checkChildrenPos()
	{
		//CCArray* array = m_PageLayer->getChildren();
		CCObject* obj;
		CCARRAY_FOREACH(m_PageLayer,obj)
		{		
			CCLayer* layer = dynamic_cast<CCLayer*>(obj);
			if(layer)
			{
				CCArray* itemArray = layer->getChildren();
				CCObject* itemObj;
				CCARRAY_FOREACH(itemArray,itemObj)
				{
					CCNode* iconButton = dynamic_cast<CCNode*>(itemObj);
					if(iconButton)
					{
						CCArray* menuArray = iconButton->getChildren();
						CCObject* menuObj;
						CCARRAY_FOREACH(menuArray,menuObj)
						{
							CCMenu* menu = dynamic_cast<CCMenu*>(menuObj);
							if(menu)
							{
								CCArray* menuItemArray = menu->getChildren();
								CCObject* menuItmeObj;
								bool claim = false;
								CCARRAY_FOREACH(menuItemArray,menuItmeObj)
								{
									CCMenuItem* item = dynamic_cast<CCMenuItem*>(menuItmeObj);
									if(item)
									{
										CCPoint pt = layer->convertToWorldSpace(menu->getPosition());
										claim = touchIsInContent(pt);
										if(claim)
										{
											break;
										}
									}
								}
								menu->setEnabled(claim);
							}
						}
					}
				}
			}
		}
	}

	void UIScrollLayer::clearIndicators()
	{
		for (size_t i = 0; i < indicators.size(); i++)
		{
			indicators[i]->removeFromParent();
		}

		indicators.clear();
	}

	void UIScrollLayer::registerPageChangeCallback(CCObject* target,SEL_CallFunc handler)
	{
		this->m_pageChangeTarget = target;
		this->m_pageChangeHandler = handler;
	}

	void UIScrollLayer::addIndicators(EScrollDirection dir)
	{
		if (m_Page <= 1)
		{
			return;
		}
		if (m_bShowPagesIndicator)
		{
			float scale = UIManager::sharedManager()->getScaleFactor();
        
			int totalScreens = m_Page;
            
			// Prepare Points Array
			vector<CCPoint> indiPoints;

			//·ÖºáÏò×ÝÏò»­µã
			if(m_ScrollDir == ESD_Horizontal)
			{
				CGFloat n = (CGFloat)totalScreens; //< Total points count in CGFloat.
				CGFloat pY = m_tPagesIndicatorPosition.y; //< Points y-coord in parent coord sys.
				CGFloat d = 16.0f * scale; //< Distance between points.
				CCPoint* points = new CCPoint[totalScreens];
				int pointX = m_tPagesIndicatorPosition.x;
				for (int i = 0; i < totalScreens; ++i)
				{                
					CGFloat pX = pointX + d * ((CGFloat)i - 0.5f*(n-1.0f));
					points[i] = ccp(pX, pY);
					indiPoints.push_back(points[i]);
				}
				delete [] points;
			}
			else if (m_ScrollDir == ESD_Vertical)
			{
				CGFloat n = (CGFloat)totalScreens; //< Total points count in CGFloat.
				CGFloat pX = m_tPagesIndicatorPosition.x; //< Points y-coord in parent coord sys.
				CGFloat d = 16.0f * scale; //< Distance between points.
				CCPoint* points = new CCPoint[totalScreens];
				int pointY = m_tPagesIndicatorPosition.y;
				for (int i = 0; i < totalScreens; ++i)
				{                
					CGFloat pY = pointY - d * ((CGFloat)i - 0.5f*(n-1.0f));
					points[i] = ccp(pX, pY);
					indiPoints.push_back(points[i]);
				}
				delete [] points;
			}

			

			CCPoint pt;
			ASprite *as = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
			int sheetSelectId = getResourceIDByName("map_ui_FRAME_SHEET_SELECT");
			int sheetunSelectId = getResourceIDByName("map_ui_FRAME_SHEET_UNSELECT");
			for (size_t i = 0; i < indiPoints.size(); i++)
			{
				CCLayer * curLayer = (CCLayer *)m_PageLayer->objectAtIndex(i);	
				if (m_CurPage != i)
				{
					CCSprite * unselectIcon = as->getSpriteFromFrame_Middle(sheetunSelectId, 0, pt);	
					unselectIcon->setScale(scale);
					unselectIcon->setPosition(indiPoints[i]);
					this->addChild(unselectIcon);
					indicators.push_back(unselectIcon);
				}
				else
				{
					CCSprite * selectIcon = as->getSpriteFromFrame_Middle(sheetSelectId, 0, pt);
					selectIcon->setScale(scale);
					selectIcon->setPosition(indiPoints[i]);
					this->addChild(selectIcon);
					indicators.push_back(selectIcon);
				}
			}
        
			
		}
		
	}
}

