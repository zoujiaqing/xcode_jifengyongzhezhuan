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
#include "UILayout.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"

namespace TXGUI {
    USING_NS_CC;
    
    NiImplementRTTI(UILayout, UIControlBase);
    NiImplementCreateObject(UILayout);
    
    UILayout::UILayout(cocos2d::CCNode* parent) 
    :UIControlBase(parent)
    {
        m_pLayer =  new UILayerColorBase( true,  true,  ccc4(255,255,255,0));
		m_pLayer->init();
		m_pLayer->autorelease();
		m_pLayer->setPosition(CCPointZero);
        m_pControlNode = m_pLayer;
        
        CCNode *pNode = getCurrentNode();
        parent->addChild(pNode);
    }

	UILayout::UILayout(std::string name, int tag, float z, UIControlBase *parent)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pLayer =  new UILayerColorBase( true,  true,  ccc4(255,255,255,0));
		m_pLayer->init();
		m_pLayer->autorelease();
		m_pLayer->setPosition(CCPointZero);
		m_pControlNode = m_pLayer;

		parent->getCurrentNode()->addChild(m_pLayer);
	}

	UILayout::UILayout(std::string name, int tag, float z, UIControlBase *parent, ccColor4B color)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pLayer =  new UILayerColorBase( true,  true,  color);
		m_pLayer->init();
		m_pLayer->autorelease();
		m_pLayer->setPosition(CCPointZero);
		m_pControlNode = m_pLayer;

		parent->getCurrentNode()->addChild(m_pLayer);
	}
    
    UILayout::~UILayout()
    {   
        // don't call this

    }
    
    CCNode* UILayout::getBindNode()
    {
        // 当前的bind节点应该是新创建的Layer节点
        return getCurrentNode();
        //return UIControlBase::getBindNode();
    }
    
    void UILayout::setPosition(CCPoint pt) 
    {
        UIControlBase::setPosition(pt);

		if(m_pLayer != NULL)
		{
			m_pLayer->setPosition(pt);
		}
    }
    
    /**
     * @brief 注册一个MenuHandler
     * @param SEL_MenuHandler, pfn
     * @return void
     * @
     */
    void UILayout::RegisterMenuHandler(string name, SEL_MenuHandler pfn, CCObject* pSelect)
    {
        MenuHandler st;
        assert(pSelect);
        st.pfn = pfn;
        st.pSelect = pSelect;
        m_MenuCallback[name] = st;
    }
    
    /**
     * @brief 反注册一个MenuHandler
     * @param SEL_MenuHandler, pfn
     * @return void
     * @
     */
    void UILayout::UnregisterMenuHandler(string name)
    {
        if (m_MenuCallback.find(name) != m_MenuCallback.end())
        {
            m_MenuCallback.erase(name);
        }
    }

	/**
     * @brief 反注册所有MenuHandler
     * @return void
     * @
     */
	void UILayout::UnregisterAllMenuHandler()
	{
		m_MenuCallback.clear();
	}

	void UILayout::setVisible(bool visible)
	{
		if (m_pLayer)
		{
			m_pLayer->setVisible(visible);
		}
	}

	void UILayout::setVisibleInStyle(bool visible)
	{
		if (m_pLayer )
		{
			m_pLayer->setVisible(visible);

			/// 显示时，从小到大显示 ,有一个通用的动画效果
			if (visible)
			{
				CCPoint pt = m_pLayer->getPosition();
				CCMoveTo* mov = CCMoveTo::create(0.5f, pt);
				pt.x += CCDirector::sharedDirector()->getWinSize().width;
				m_pLayer->setPosition(pt);				
				m_pLayer->runAction(mov);
			}
		}
	}
    
    /**
     * @brief 调用MenuHandler
     * @param namepfn, Function's Name with class name prefix
     * @return void
     * @
     */
    void UILayout::CallMenuHandler(string namepfn, CCObject* pObject)
    {
        if (m_MenuCallback.find(namepfn) != m_MenuCallback.end())
        {
            MenuHandler mh = m_MenuCallback[namepfn];
            CCObject* pSelect = mh.pSelect;
            SEL_MenuHandler pfn = mh.pfn; 
            if (pSelect)
            {
                (pSelect->*pfn)(pObject);
            }        
        }
    }

    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UILayout::UILayout() 
    {
		m_pLayer = NULL;
		m_pControlNode = NULL;
    }    
    
    void UILayout::LoadBinary(NiStream &kStream)
    {
        UIControlBase::LoadBinary(kStream);

		mutableDic *dic = kStream.GetStreamData();

		uint32_t color[4] = {255, 255, 255, 0};

		mutableDic *colorDic = static_cast<mutableDic*>(dic->objectForKey("color"));
		if(colorDic)
		{
			kStream.getIntAttributeValue(colorDic, "red", color[0]);
			kStream.getIntAttributeValue(colorDic, "green", color[1]);
			kStream.getIntAttributeValue(colorDic, "blue", color[2]);
			kStream.getIntAttributeValue(colorDic, "alpha", color[3]);
		}

		uint32_t bTouchEnable = 0;
		int touchPriority = kCCScrollTouchBgPriority;
		kStream.getIntAttributeValue(dic, "bModelLayer", bTouchEnable);
		kStream.getSignedIntAttributeValue(dic,"bModelPriorty",touchPriority);
		m_pLayer =  new UILayerColorBase( true,  true,  ccc4(color[0],color[1],color[2],color[3]));
		m_pLayer->init();
		m_pLayer->autorelease();

		m_pLayer->EnableModeless(bTouchEnable != 0,touchPriority);
		m_touchPriority = touchPriority;

		m_pControlNode = m_pLayer;
		m_pControlNode->setPosition(m_ptLocalPosition);
		m_pControlNode->setVisible(m_bIsVisible);
    }

	void UILayout::EnableModeless(bool bModelLayer)
	{
		if (m_pLayer)
		{
			m_pLayer->EnableModeless(bModelLayer,m_touchPriority);
		}
	}

    void UILayout::SetNewPriority(int priority)
    {
        m_touchPriority = priority;
    }
    
    void UILayout::LinkObject(NiStream &kStream)
    {
        UIControlBase::LinkObject(kStream);
    }
    
    bool UILayout::RegisterStreamables(NiStream &kStream)
    {
        if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UILayout::SaveBinary(NiStream &kStream)
    {
        UIControlBase::SaveBinary(kStream);
        
    }
    
    bool UILayout::IsEqual(NiObject *pObject)
    {
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }   
}