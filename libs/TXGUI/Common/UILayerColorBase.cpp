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
#include "TXGUI.h"
#include "UILayerColorBase.h"

namespace TXGUI {
    // UILayerColorBase 
	UILayerColorBase::UILayerColorBase()
		:
		m_bSwallowTouchEvent(false)
		,m_touchPriority(kCCScrollTouchBgPriority)
	{
		ccColor4B c = {0, 0, 0, 0};
		 _layerColor = c;

		 setTouchEnabled(true);
	}


    UILayerColorBase::UILayerColorBase(std::string fileName, 
                                       bool touchEnable, 
                                       bool schedule,
                                       ccColor4B color)
    : _layerColor(color),
	m_bSwallowTouchEvent(false)
    {
        setTouchEnabled(touchEnable);
        if (schedule)
            this->scheduleUpdate();
    }
    
    UILayerColorBase::UILayerColorBase(bool touchEnable, bool schedule, ccColor4B color)
    : _layerColor(color),
	m_bSwallowTouchEvent(false)
    {
        setTouchEnabled(touchEnable);
        if (schedule)
            this->scheduleUpdate();
    }
    
	bool UILayerColorBase::init()
	{
		bool localTouchEnable = m_bTouchEnabled;
		//if(!ColorLayerBase::init())
		if(!LayerBase::init())
		{
			return false;
		}
		setTouchEnabled(localTouchEnable);
		return true;
	}

    UILayerColorBase::~UILayerColorBase() {
        
    }
    
    cocos2d::ccColor4B UILayerColorBase::getLayerColor() {
        return _layerColor;
    }
    
    void UILayerColorBase::onBroadcastMessage(BroadcastMessage* msg) {
        
    }
    
    void UILayerColorBase::notifyUIMsg(UILayerMsgId msgId, uint32_t param) {
        
    }
    
    void UILayerColorBase::clearAllChildren() {
        this->removeAllChildrenWithCleanup(true);
    }
    
    void UILayerColorBase::showLayer(bool show) {
        this->setVisible(show);
    }
    
    void UILayerColorBase::onInit() {
        //ColorLayerBase::onInit();  
		LayerBase::onInit();
    }
    
    void UILayerColorBase::onUnInit() {
        //ColorLayerBase::onUnInit();     
		LayerBase::onUnInit();
    }

	void UILayerColorBase::onExit()
	{
		CCLayer::onExit();
		/*if (m_bTouchEnabled)
		{
			/// unregister touch delegate
			CCDirector* pDirector = CCDirector::sharedDirector();
			pDirector->getTouchDispatcher()->removeDelegate(this);
		}*/
	}
    
    void UILayerColorBase::update(float deltaTime) {
        
    }
    
    void UILayerColorBase::onItemClickCallback(int tagId) {
        
    }
    
	void UILayerColorBase::onItemValueChanged(int tagId) {

	}

	void UILayerColorBase::EnableModeless(bool bModelLayer,int priority) 
	{ 
		if (m_bSwallowTouchEvent && !bModelLayer)
		{
			/// unregister touch delegate
			CCDirector* pDirector = CCDirector::sharedDirector();
			pDirector->getTouchDispatcher()->removeDelegate(this);
		}

		m_touchPriority = priority;
		m_bSwallowTouchEvent = bModelLayer;
		if (isRunning() == false && bModelLayer)
		{
			/// force to add the touch delegate
			setTouchEnabled(true);
		}
	}

	bool UILayerColorBase::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		// find parent visible ,if not , should not process the touch event 
		bool bVisible = isVisible();
		CCNode* pParent = getParent();
		while(pParent != NULL && bVisible)
		{
			if (pParent->isVisible() == false)
			{
				bVisible = false;
			}

			pParent = pParent->getParent();
		}

		if(bVisible && m_bSwallowTouchEvent)
		{
			return true;
		}

		return false;
	}

	void UILayerColorBase::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLayer::ccTouchMoved(pTouch,pEvent);
	}

	void UILayerColorBase::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLayer::ccTouchEnded(pTouch,pEvent);
	}

	void UILayerColorBase::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLayer::ccTouchCancelled(pTouch,pEvent);
	}

	void UILayerColorBase::registerWithTouchDispatcher()
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
	}
}

