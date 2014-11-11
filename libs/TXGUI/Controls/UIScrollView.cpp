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

#include "UIScrollView.h"

#include <math.h>

#define TOUCH_DELTA 25

namespace TXGUI {
    using namespace cocos2d;  
   
    UIScrollView::UIScrollView(bool touchEnable, bool schedule, ccColor4B color) 
    : UILayerColorBase( touchEnable,  schedule,  color)
    {
        container_ = NULL;
        touchMoved_ = false;
		shouldSwallowTouch_ = false;
        isDragging_ = false;
        bounces_ =false;
        clipsToBounds_ = false;
        m_pDelegate = NULL;
		isScrollList = false ;
		m_ScrollMaxTarget = NULL;
		m_ScrollMaxHandler = NULL;
		m_isReceiveScrollMsg = false;
    }
    
    UIScrollView::UIScrollView() 
    : UILayerColorBase( true,  true,  ccc4(255,255,255,255))
    {
        container_ = NULL;
        touchMoved_ = false;
        isDragging_ = false;
        bounces_ =false;
        clipsToBounds_ = false;
        m_pDelegate = NULL;
		m_ScrollMaxTarget = NULL;
		m_ScrollMaxHandler = NULL;
		m_isReceiveScrollMsg = false;
    }
    
    UIScrollView::~UIScrollView()
    {
		CC_SAFE_DELETE(touches_);
    }    
    
    UIScrollView *UIScrollView::viewWithViewSize(cocos2d::CCSize size)
    {
        UIScrollView *pScroView = new UIScrollView();
        if (pScroView && pScroView->initWithViewSize(size)) 
        {
			pScroView->autorelease();
            return pScroView;
        }
        CC_SAFE_DELETE(pScroView);
        return NULL;
    }
    
    bool UIScrollView::initWithViewSize(CCSize size)
    {
        initWithViewSize(size, NULL);
        return true;
    }
    
    UIScrollView *UIScrollView::viewWithViewSize(CCSize size, CCNode* container)
    {
        UIScrollView *pScroView = new UIScrollView();
        if (pScroView && pScroView->initWithViewSize(size, container))
        {
            return pScroView;
        }
        CC_SAFE_DELETE(pScroView);
        return NULL;
    }
    
    bool UIScrollView::initWithViewSize(CCSize size, CCNode* container)
    {
        if(!CCLayer::init())
        {
            return false;
        }        
        
        setViewSize(size);
        container_ = container;
        if(!container_)
        {
            container_ = CCLayer::create();
        }
        
        setIsTouchEnabled(true);
        touches_ = new TouchArray();
        bounces_ = true;
        clipsToBounds_ = true;
        container_->setContentSize(CCSizeZero);
        direction_ = SWUIScrollViewDirectionBoth;
        container_->setPosition(CCPointZero);
        touchLength_ = 0;
        
        addChild(container_);
        
        minScale_ = maxScale_ =1.0f;
        m_bPriority = kCCScrollTouchPriority;
        
        return true;
    }
    

    CCPoint UIScrollView::getMaxContainerOffset()
    {
		CCSize newViewSize = CCSize(viewSize_.width, viewSize_.height);

		if (container_) {           
			CCPoint pt = CCPointZero;
			if(direction_ == SWUIScrollViewDirectionBoth || direction_ == SWUIScrollViewDirectionVertical)
			{
				if(container_->getContentSize().height > newViewSize.height)
				{
					pt.y = (container_->getContentSize().height - newViewSize.height) * container_->getScaleY();
				}
			}
			if(direction_ == SWUIScrollViewDirectionBoth || direction_ == SWUIScrollViewDirectionHorizontal)
			{
				if(container_->getContentSize().width > newViewSize.width)
				{
					pt.x = (container_->getContentSize().width - newViewSize.width) * container_->getScaleX();
				}
			}
			return pt;
		}else
		{
			return ccp(newViewSize.width ,newViewSize.height);
		}
    }
    
    CCPoint UIScrollView::getMinContainerOffset()
    {
        return ccp(0.0f,0.0f);
    }
    
    void UIScrollView::stoppedAnimatedScroll(CCNode *pSender)
    {
        unschedule(schedule_selector(UIScrollView::performedAnimatedScroll));
		//by benyang
		checkChildrenPos();
    }
    
    void UIScrollView::performedAnimatedScroll(float dt)
    {
        if (isDragging_)
        {
            unschedule(schedule_selector(UIScrollView::performedAnimatedScroll));
            return;
        }
        
        ////excute scroviewdidscroll
        if (m_pDelegate)
        {
            m_pDelegate->scrollViewDidScroll(this);
        }
    }
    
    void UIScrollView::deaccelerateScrolling(float dt)
    {
        if (isDragging_) {
            unschedule(schedule_selector(UIScrollView::deaccelerateScrolling));
            return;
        }
        
        float newX, newY;
        CCPoint maxInset, minInset;
        
        container_->setPosition (ccpAdd(container_->getPosition(), scrollDistance_));
        
        if (bounces_) {
            maxInset = maxInset_;
            minInset = minInset_;
        } else {
            maxInset = getMaxContainerOffset();
            minInset = getMinContainerOffset();
        }
        
        //check to see if offset lies within the inset bounds
        newX     = MIN(container_->getPosition().x, maxInset.x);
        newX     = MAX(newX, minInset.x);
        newY     = MIN(container_->getPosition().y, maxInset.y);
        newY     = MAX(newY, minInset.y);
        
        scrollDistance_     = ccpSub(scrollDistance_, ccp(newX - container_->getPosition().x, 
                                                          newY - container_->getPosition().y));
        scrollDistance_     = ccpMult(scrollDistance_, SCROLL_DEACCEL_RATE);
        setContentOffset(ccp(newX,newY));
        
        if ((fabsf(scrollDistance_.x) <= SCROLL_DEACCEL_DIST &&
             fabsf(scrollDistance_.y) <= SCROLL_DEACCEL_DIST) ||
            newX == maxInset.x || newX == minInset.x ||
            newY == maxInset.y || newY == minInset.y) {
            unschedule(schedule_selector(UIScrollView::deaccelerateScrolling));
            relocateContainer(true);
        }
    }
    
    void UIScrollView::relocateContainer(bool animated)
    {
        CCPoint oldPoint, min, max;
        float newX, newY;
        
        min = getMinContainerOffset();
        max =  getMaxContainerOffset();
        
        oldPoint = container_->getPosition();
        newX     = oldPoint.x;
        newY     = oldPoint.y;
        if (direction_ == SWUIScrollViewDirectionBoth || direction_ == SWUIScrollViewDirectionHorizontal) {
            newX     = MIN(newX, max.x);
            newX     = MAX(newX, min.x);
        }
        if (direction_ == SWUIScrollViewDirectionBoth || direction_ == SWUIScrollViewDirectionVertical) {
            newY     = MIN(newY, max.y);
            newY     = MAX(newY, min.y);
			//if(newY < 0)
			//{
			//	newY = min.y;
			//}
			//else
			//{
			//	newY = MIN(newY, max.y);
			//}
        }
        if (newY != oldPoint.y || newX != oldPoint.x) {
            setContentOffset(ccp(newX, newY),animated);
        }
		//by benyang
		else
		{
			checkChildrenPos();
		}
    }
    

    
    CCSize UIScrollView::getContentSize()
    {
        return  container_->getContentSize();
    }
    
    void UIScrollView::setContentSize(CCSize size)
    {
        if (container_) 
        {
            container_->setContentSize(size);
        }        
        //maxInset_ = getMaxContainerOffset();
        //maxInset_ = ccp(maxInset_.x + viewSize_.width * INSET_RATIO,
        //                maxInset_.y + viewSize_.height * INSET_RATIO);
        //
        //minInset_ = getMinContainerOffset();
        //minInset_ = ccp(minInset_.x - viewSize_.width * INSET_RATIO,
        //                minInset_.y - viewSize_.height * INSET_RATIO);
        
		maxInset_ = getMaxContainerOffset();
		maxInset_ = ccp(maxInset_.x + viewSize_.width * INSET_RATIO,
			maxInset_.y + viewSize_.height * INSET_RATIO);

		minInset_ = getMinContainerOffset();
		minInset_ = ccp(minInset_.x - viewSize_.width * INSET_RATIO,
			minInset_.y - viewSize_.height * INSET_RATIO);
    }

	void UIScrollView::setPosition(CCPoint pt)
	{
		CCNode::setPosition(pt);

		worldPosition_ = convertToWorldSpace(CCPointZero);
	}

	void UIScrollView::setContainerPos(CCPoint pt)
	{
		if(container_)
		{
			container_->setPosition(pt);
			checkChildrenPos();
		}
	}

	cocos2d::CCPoint UIScrollView::getContainerPos()
	{
		if(container_)
		{
			return container_->getPosition();
		}

		return CCPointZero;
	}

	void UIScrollView::setScale(float scale)
	{
		viewSize_ = CCSize(viewSize_.width * scale, viewSize_.height * scale);
		CCSize contentSize = container_->getContentSize();
		container_->setContentSize(CCSize(contentSize.width * scale, contentSize.height * scale));
		setZoomScale(scale);
	}

	void UIScrollView::setScale(float xScale, float yScale)
	{
		viewSize_ = CCSize(viewSize_.width * xScale, viewSize_.height * yScale);
		setZoomScale((xScale < yScale)? xScale : yScale);
	}
    
    void UIScrollView::addChild(CCNode *node,int z,int aTag)
    {
        //CCLOG("addChild");
        if (container_ != node) 
        {
            container_->addChild(node,z,aTag);
        } else {
			node->ignoreAnchorPointForPosition(false);
			node->setAnchorPoint (ccp(0.0f, 0.0f));
            CCLayer::addChild(node, z, aTag);
        }
    }
    
    void UIScrollView::beforeDraw()
    {
        if(clipsToBounds_)
        {
            glEnable(GL_SCISSOR_TEST);
            const float s = CCDirector::sharedDirector()->getContentScaleFactor();
			glScissor(worldPosition_.x * s - viewSize_.width / 2 * s,
                      worldPosition_.y * s - viewSize_.height / 2 * s,
                      viewSize_.width * s,
                      viewSize_.height * s);
			if(isScrollList)
			{
				ccArray *arrayData = m_pChildren->data;
                CCNode *childroot =  (CCNode *)arrayData->arr[0];
				bool   IsShow = false ;
                if(childroot->getChildrenCount()> 0)
                {
                //CCPoint offset = child->getPosition();
                arrayData = childroot->getChildren()->data;
				unsigned int i=0;
				for( ; i < arrayData->num; i++ ) 
				{
					CCNode * child =  (CCNode *)arrayData->arr[i];
					if(direction_ == SWUIScrollViewDirectionHorizontal)
					{
                         float dx = childroot->convertToNodeSpace(ccp(worldPosition_.x * s,worldPosition_.y * s)).x ;
						if((child->getPositionX()-(container_->getContentSize().width*0.5)/((float)(arrayData->num))>(dx+  viewSize_.width / 2 * s))||
							((child->getPositionX()+(container_->getContentSize().width*1.5)/((float)(arrayData->num)))<(dx - viewSize_.width / 2 * s) ))
							child->setVisible(false);
						else
							child->setVisible(true);
					}
					else
					{
                        float dy =childroot->convertToNodeSpace(ccp(worldPosition_.x * s,worldPosition_.y * s)).y  ;
						if((child->getPositionY()-(container_->getContentSize().height*0.5)/((float)(arrayData->num))>(dy+  viewSize_.height / 2 * s))||
							((child->getPositionY()+(container_->getContentSize().height*1.5)/((float)(arrayData->num)))<(dy - viewSize_.height / 2 * s) ))
						{
							child->setVisible(false);
							if(IsShow)
							{
								IsShow = false ;
								child->setVisible(true);
							}
							
							
						}
                        else
						{
							if(!IsShow)
							{
								IsShow = true ;
								if(i > 0)
								{
										CCNode * childLast =  (CCNode *)arrayData->arr[i-1];
										childLast->setVisible(true);
								}


							
							}
							child->setVisible(true);
						}

					}

				}
              }
			
			}
			
        }
    }
    
    void UIScrollView::afterDraw()
    {
        if(clipsToBounds_)
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }   
    
    
    void UIScrollView::visit()
    {
        if(!isVisible())
        {
            return;
        }
        
        //    CCLayer::visit();
        //CCNode
		//// Disable
        kmGLPushMatrix();		        
        
        if (m_pGrid&&m_pGrid->isActive() )
        {
            m_pGrid->beforeDraw();
            transformAncestors();
        }
        
        transform();
        beforeDraw();
        if (m_pChildren)
        {
            ccArray *arrayData = m_pChildren->data;
            unsigned int i=0;
            // draw children zOrder < 0
            for( ; i < arrayData->num; i++ ) {
                CCNode *child =(CCNode *)  arrayData->arr[i];
                if ( child->getZOrder() < 0 ) {
                    child->visit();
                } else
                    break;
            }
            //draw();
            
            // draw children zOrder >= 0
            for( ; i < arrayData->num; i++ ) {
                CCNode *child =  (CCNode *)arrayData->arr[i];
                child->visit();
            }
            
        } else
            draw();
        
        afterDraw();
        if (m_pGrid && m_pGrid->isActive() )
            m_pGrid->afterDraw(this);
        
        
        ////glPopMatrix();
		kmGLPopMatrix();
    }
    

    void UIScrollView::setIsTouchEnabled(bool e)
    {
        CCLayer::setTouchEnabled(e);
        if (!e)
        {
            isDragging_ = false;
            touchMoved_ = false;
            touches_->removeAllObjects();
        }
    }
    
    void UIScrollView::setContentOffset(CCPoint offset)
    {
        //    CCLOG("offset1=%f %f", offset.x,offset.y);
        setContentOffset(offset, false);
    }
    
    void UIScrollView::setContentOffset(CCPoint offset,bool animated)
    {
        //    CCLOG("offset2=%f %f", offset.x,offset.y);
        if(animated)
        {
            setContentOffset(offset, BOUNCE_DURATION);
        }else
        {
            if(!bounces_)
            {
                const CCPoint minOffset = getMinContainerOffset();
                const CCPoint maxOffset = getMaxContainerOffset();
                
                offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
                offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y)); 
            }
            container_->setPosition(offset);
            if (m_pDelegate)
            {
                m_pDelegate->scrollViewDidScroll(this);
            }
            
            ///excute scrollViewDidScroll
        }
    }
    
    void UIScrollView::setContentOffset(CCPoint offset, float dt )
    {
        //    CCLOG("offset3=%f %f", offset.x,offset.y);
        CCFiniteTimeAction *scroll, *expire;
        scroll = CCMoveTo::create(dt, offset);
        expire = CCCallFuncN::create(this, callfuncN_selector(UIScrollView::stoppedAnimatedScroll));
        container_->runAction(CCSequence::create(scroll,expire,NULL));
        schedule(schedule_selector(UIScrollView::performedAnimatedScroll));
    }
    
    
    void UIScrollView::setZoomScale(float s)
    {
        if(container_->getScale() != s)
        {
            CCPoint oldCenter, newCenter;
            CCPoint center;
            
            
            if (touchLength_ == 0.0f) {
                center = ccp(viewSize_.width*0.5f, viewSize_.height*0.5f);
                center = convertToWorldSpace(center);
            } else {
                center = touchPoint_;
            }
            
            oldCenter = container_->convertToNodeSpace(center);
            container_->setScale( MAX(minScale_, MIN(maxScale_, s)));
            newCenter = container_->convertToWorldSpace(oldCenter);
            
            //重置位移
			//maxInset_ = getMaxContainerOffset();
			//maxInset_ = ccp(maxInset_.x + viewSize_.width * INSET_RATIO,
			//                maxInset_.y + viewSize_.height * INSET_RATIO);
			//
			//minInset_ = getMinContainerOffset();
			//minInset_ = ccp(minInset_.x - viewSize_.width * INSET_RATIO,
			//                minInset_.y - viewSize_.height * INSET_RATIO);

			maxInset_ = getMaxContainerOffset();
			maxInset_ = ccp(maxInset_.x + container_->getContentSize().width * INSET_RATIO,
				maxInset_.y + container_->getContentSize().height * INSET_RATIO);

			minInset_ = getMinContainerOffset();
			minInset_ = ccp(minInset_.x - container_->getContentSize().width * INSET_RATIO,
				minInset_.y - container_->getContentSize().height * INSET_RATIO);
            
            const CCPoint offset = ccpSub(center, newCenter);
            
            //excute scrollViewDidZoom
            if (m_pDelegate) {
                m_pDelegate->scrollViewDidZoom(this);
            }
            
            setContentOffset(ccpAdd(container_->getPosition(), offset));
        }
    }
    
    float UIScrollView::getZoomScale()
    {
        return container_->getScale();
    }

     void UIScrollView::setZoomScale(float s,bool animated)
     {
         if (animated) {
             setZoomScale(s,BOUNCE_DURATION);
         } else {
             setZoomScale(s);
         }
     }
     
     void UIScrollView::setZoomScale(float s, float dt)
     {
         if (dt > 0) {
             if (container_->getScale() != s) {;
                 
                 CCScaleTo *scaleAction = CCScaleTo::create(dt, s, s);
                 runAction(scaleAction);
                 //            CCActionTween *scaleAction;
                 //            scaleAction = [CCActionTween actionWithDuration:dt
                 //                                                        key:@"zoomScale"
                 //                                                       from:container_.scale
                 //                                                         to:s];
                 //            [self runAction:scaleAction];
             }
         } else {
             setZoomScale(s);
         }
     }

    
    void UIScrollView::setViewSize(CCSize size)
    {
        
        if (!viewSize_.equals(size)) 
        {
            viewSize_ = size;
            maxInset_ = getMaxContainerOffset();
            maxInset_ = ccp(maxInset_.x + viewSize_.width * INSET_RATIO,
                            maxInset_.y + viewSize_.height * INSET_RATIO);
            
            minInset_ = getMinContainerOffset();
            minInset_ = ccp(minInset_.x - viewSize_.width * INSET_RATIO,
                            minInset_.y - viewSize_.height * INSET_RATIO);
        }
    }
    
    
    /** Hackish stuff - stole touches from other CCTouchDispatcher targeted delegates. 
     Used to claim touch without receiving ccTouchBegan. */
    void UIScrollView::claimTouch(CCTouch* pTouch)
    {
        CCTargetedTouchHandler* handler =
        (CCTargetedTouchHandler*)CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this);		
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
    
    void UIScrollView::cancelAndStoleTouch(CCTouch* pTouch, CCEvent* pEvent)
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
    
	void UIScrollView::sendTouchToOther(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
	{
		CCSet* touchSet = new CCSet();
		touchSet->addObject(pTouch);
		touchSet->autorelease();
		CCDirector::sharedDirector()->getTouchDispatcher()->touchesBegan(touchSet, pEvent);
	}

	void UIScrollView::endOtherTouch(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
	{
		CCTargetedTouchHandler* handler =
			(CCTargetedTouchHandler*)CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this);		
		if (handler)
		{
			// 先删掉自己的的列表，使优先级较低控件能收到消息
			bool claimed = false;
			CCSet* claimedTouches = handler->getClaimedTouches();
			if (claimedTouches->containsObject(pTouch))
			{
				claimed = true;
				claimedTouches->removeObject(pTouch);
			}

			CCSet* touchSet = new CCSet();
			touchSet->addObject(pTouch);
			touchSet->autorelease();
			CCDirector::sharedDirector()->getTouchDispatcher()->touchesEnded(touchSet, pEvent);

			claimedTouches = handler->getClaimedTouches();
			if(claimed)
			{
				if (!claimedTouches->containsObject(pTouch))
				{
					claimedTouches->addObject(pTouch);
				}
			}
		}

	}

    void UIScrollView::registerWithTouchDispatcher()
    {
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_bPriority, false);
        //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_bPriority, true);
    }
    
    // default implements are used to call script callback if exist
    bool UIScrollView::ccTouchBegan(CCTouch* touch, CCEvent* event)
    {
        if(!isVisible())
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
//        CCPoint temp =container_->convertToWorldSpace(container_->convertTouchToNodeSpace(touch));
        
		// 此touch事件判定为非滑动操作时自己发出的，由其它控件响应
		/*if(touches_->containsObject(touch))
		{
			touches_->removeObject(touch);
			return false;
		}*/

        CCRect frame = CCRectMake(worldPosition_.x - viewSize_.width / 2, worldPosition_.y - viewSize_.height / 2,
			viewSize_.width, viewSize_.height);
        //  CCLOG("%d",CCRect::CCRectContainsPoint(frame,container_->convertToWorldSpace(container_->convertTouchToNodeSpace(touch))));
        //CCLOG("%d",touchMoved_);
        if(touches_->count()>2 || 
           touchMoved_         ||
           !frame.containsPoint(container_->convertToWorldSpace(container_->convertTouchToNodeSpace(touch))))
        {
            return false; 
            
        }
        
        if (!touches_->containsObject(touch))
        {
            touches_->addObject(touch);
        }
        
        if(touches_->count() ==1)
        {
            touchBeginPoint_ = touchPoint_ =convertTouchToNodeSpace(touch);
            touchMoved_ = false;
			shouldSwallowTouch_ = false;
            isDragging_ = true;
            scrollDistance_ = CCPointZero;
            touchLength_ = 0.0f;
        }else if(touches_->count() ==2)
        {
            touchPoint_ = ccpMidpoint(convertTouchToNodeSpace((CCTouch*)touches_->objectAtIndex(0)),
                                      convertTouchToNodeSpace((CCTouch*)touches_->objectAtIndex(1)));
            
            touchLength_ = ccpDistance(container_->convertTouchToNodeSpace((CCTouch*)touches_->objectAtIndex(0)),
                                       container_->convertTouchToNodeSpace((CCTouch*)touches_->objectAtIndex(1)));
            isDragging_ = false;
        }

        return true;
    }
    
    void UIScrollView::ccTouchMoved(CCTouch* touch, CCEvent* event)
    {
        if(!isVisible())
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
        
        if(touches_->containsObject(touch))
        {
            if(touches_->count() ==1 && isDragging_)
            {
                CCPoint moveDistance, newPoint, maxInset, minInset;
                CCRect  frame;
                float newX, newY;

				touchMoved_ = true;
                
				frame = CCRectMake(worldPosition_.x - viewSize_.width / 2, worldPosition_.y - viewSize_.height / 2,
					viewSize_.width, viewSize_.height);
                newPoint = convertTouchToNodeSpace(touch);
                moveDistance = ccpSub(newPoint, touchPoint_);
                touchPoint_ = newPoint;
                
                if(frame.containsPoint( convertToWorldSpace(newPoint)))
                {
					CCPoint totalDistance = ccpSub(newPoint,touchBeginPoint_);
                    switch (direction_) {
                        case SWScrollViewDirectionVertical:
                            moveDistance = ccp(0.0f, moveDistance.y);
							if(abs((int)totalDistance.y) > TOUCH_DELTA)
							{
								shouldSwallowTouch_ = true;
							}
                            break;
                        case SWScrollViewDirectionHorizontal:
							moveDistance = ccp(moveDistance.x, 0.0f);
							if(abs((int)totalDistance.x) > TOUCH_DELTA)
							{
								shouldSwallowTouch_ = true;
							}
                            break;
                        default:
                            break;
                    }
                    
					container_->setPosition(ccpAdd(container_->getPosition(), moveDistance));
                    
                    maxInset = maxInset_;
                    minInset = minInset_;
                    
                    //CCLOG("scroll view container:%f, %f", container_->getPosition().x, container_->getPosition().y);
                    //CCLOG("maxInset:%f, %f", maxInset.x, maxInset.y);
                    //CCLOG("minInset:%f, %f", minInset.x, minInset.y);
                    
                    newX     = MIN(container_->getPosition().x, maxInset.x);
                    newX     = MAX(newX, minInset_.x);
                    newY     = MIN(container_->getPosition().y, maxInset.y);
                    newY     = MAX(newY, minInset.y);
                    
                    scrollDistance_     = ccpSub(moveDistance, ccp(newX - container_->getPosition().x, 
                                                                   newY - container_->getPosition().y));
                    //CCLOG("scrollDistance_ = (%f,%f)",scrollDistance_.x,scrollDistance_.y);
					//CCLOG("newX = %f,newY = %f",newX,newY);
                    setContentOffset(ccp(newX,newY));               
                    checkScrollMax(newY,minInset.y);
					
                }
            }else if(touches_->count() ==2 && !isDragging_)
            {
                const float len = ccpDistance(container_->convertTouchToNodeSpace((CCTouch*)touches_->objectAtIndex(0)),
                                              container_->convertTouchToNodeSpace((CCTouch*)touches_->objectAtIndex(1)));
                setZoomScale(getZoomScale()*len/touchLength_); 
            }
        }
    }
    
    void UIScrollView::ccTouchEnded(CCTouch* touch, CCEvent* event)
    {
        
        if(!isVisible())
        {
            return;
        }
        
        if(touches_->containsObject(touch))
		{
			if(touches_->count() ==1 && touchMoved_)
			{
				schedule(schedule_selector(UIScrollView::deaccelerateScrolling));
			}

			if(touches_->count() ==1 && shouldSwallowTouch_)
            {
                //把事件吃掉　不返回下一层
                cancelAndStoleTouch(touch, event);
            }
            touches_->removeObject(touch);

			//if(touches_->count() ==1 && !shouldSwallowTouch_)
			//{
			//	sendTouchToOther(touch,event);
			//	endOtherTouch(touch,event);
			//	isDragging_ = false;
			//	touchMoved_ = false;
			//}
			//else
			//{
			//	if(touches_->count() ==1 && shouldSwallowTouch_)
			//	{
			//		//把事件吃掉　不返回下一层
			//		cancelAndStoleTouch(touch, event);
			//	}
			//	touches_->removeObject(touch);
			//}

        }
        if(touches_->count() == 0)
        {
            isDragging_ = false;
            touchMoved_ = false;
			shouldSwallowTouch_ = false;
        }
		checkChildrenPos();
    }
    
    void UIScrollView::ccTouchCancelled(CCTouch *touch, CCEvent* event)
    {
        if(!isVisible())
        {
            return;
        } 
        touches_->removeObject(touch);
        if(touches_->count() == 0)
        {
            isDragging_ = false;
            touchMoved_ = false;
        }
    }

	void UIScrollView::checkScrollMax(float newY,float maxY)
	{
		if(!m_isReceiveScrollMsg)
		{
			return;
		}

		CCPoint offset = getContainerPos();
		CCSize viewSize = getViewSize();
		CCSize contentSize = getContentSize();
		if(direction_ == SWScrollViewDirectionVertical)
		{
			if(contentSize.height >= viewSize.height && contentSize.height - viewSize.height <= offset.y) 
			{
				if(m_ScrollMaxHandler != NULL && m_ScrollMaxTarget != NULL)
				{
					m_isReceiveScrollMsg = false;
					(m_ScrollMaxTarget->*m_ScrollMaxHandler)();
				}
			}
		}
	}

	void UIScrollView::setCanReceiveScrollCallback(bool isReceive)
	{
		m_isReceiveScrollMsg = isReceive;
	}

	void UIScrollView::registerScrollMaxCallback(CCObject* target,SEL_CallFunc handler)
	{
		m_ScrollMaxHandler = handler;
		m_ScrollMaxTarget = target;
	}

	void UIScrollView::checkChildrenPos()
	{
		if(container_->getChildren())
		{
			CCArray *containerChildren = container_->getChildren();
			CCPoint localPosition = convertToNodeSpace(worldPosition_);
			CCRect frame = CCRectMake(localPosition.x - viewSize_.width / 2, localPosition.y - viewSize_.height / 2, 
				viewSize_.width, viewSize_.height);

			for(int i = 0; i < (int)containerChildren->count(); i++)
			{
				CCObject *childObj = containerChildren->objectAtIndex(i);
				CCNode* buttonNode = dynamic_cast<CCNode*>(childObj);
				CCArray* menuArrar = buttonNode->getChildren();
				CCObject* menuObj;
				CCARRAY_FOREACH(menuArrar,menuObj)
				{
					CCMenu* menu = dynamic_cast<CCMenu*>(menuObj);
					if(menu)
					{
						if(menu->getChildren())
						{
							CCArray *menuChildren = menu->getChildren();
							for(int j = 0; j < (int)menuChildren->count(); j++)
							{
								CCMenuItem *childItem = (CCMenuItem*) menuChildren->objectAtIndex(j);

								CCPoint childPos = buttonNode->getPosition();
								CCPoint layerOffset = container_->getPosition();
								CCPoint worldPos = ccpAdd(childPos, layerOffset);

								if(frame.containsPoint( worldPos))
								{
									childItem->setEnabled(true);
								}
								else
								{
									childItem->setEnabled(false);
								}
							}

						}
					}
				}
			}
		}
	}

}
