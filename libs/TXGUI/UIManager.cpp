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
#include "CCDirector.h"

#include "UIManager.h"
#include "UICreator.h"

#include "CustomScene.h"
#include "LayerBase.h"

#include "NotificationCenter.h"

#include "TXAudioEngine.h"

#include "UILayerBase.h"
#include "UILayerColorBase.h"


#include "UIContainer.h"

#include <string>
using namespace std;


//这里定义的是UI制作时，参考的标准尺寸（像素)
#define SRC_WIDTH   REF_LEVEL_WIDTH
#define SRC_HEIGHT  REF_LEVEL_HEIGHT

namespace TXGUI {
    USING_NS_CC;
    
    UIManager* UIManager::pSharedManager = NULL;

	UIManager::UIManager()
		: m_pUIDefaultRoot(NULL)
	{
		m_UIMap.clear();
	}

	UIManager::~UIManager()
	{
		std::map<std::string, UILayout*>::iterator it = m_UIMap.begin();
		for(; it != m_UIMap.end(); it++)
		{
			UILayout* layer = it->second;
			delete layer;
		}
	}
    
    UIManager* UIManager::sharedManager()
    {
        if(!pSharedManager)
        {
            pSharedManager = new UIManager();
            
        }
        return pSharedManager;
    }

	void UIManager::Destroy()
	{
		if (pSharedManager)
		{
			delete pSharedManager;
			pSharedManager = NULL;
		}
	}

	/// set/get ui default root
	void	UIManager::setUIDefaultRoot(CCNode* pRoot)
	{
		m_pUIDefaultRoot = pRoot;
	}

	CCNode* UIManager::getUIDefaultRoot()
	{
		return m_pUIDefaultRoot;
	}
    
    void UIManager::DispatchMenuEvent(UIControlBase* pSender, string namepfn, CCObject* pObject)
    {
        assert(pSender);
        UILayout* layerout = NULL;
        
        if (dynamic_cast<UILayout*>(pSender) != NULL)
            layerout = dynamic_cast<UILayout*>(pSender);
        
		while (pSender->getParent() != NULL) 
        {
            pSender = pSender->getParent(); 
            if (dynamic_cast<UILayout*>(pSender) != NULL)
            {
                layerout = dynamic_cast<UILayout*>(pSender);
            }        
        }
        
        assert(layerout);
        if (layerout)
        {
            layerout->CallMenuHandler(namepfn, pObject);           
        }
    }

	float UIManager::getScaleFactor()
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
		float scaleX = winSize.width / SRC_WIDTH;
		float scaleY = winSize.height / SRC_HEIGHT;

		return (scaleX < scaleY)? scaleX : scaleY;
	}

	float UIManager::getScaleFactorX()
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
		float scaleX = winSize.width / SRC_WIDTH;

		return scaleX;
	}

	float UIManager::getScaleFactorY()
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
		float scaleY = winSize.height / SRC_HEIGHT;

		return scaleY;
	}

   	   /*
		*@prototype, CreateUILayoutFromFile()
		*@param, fileName - plist file name
		*@param, UIName - ui name
		*@param, bShow - visible
		*/
	UILayout * UIManager::CreateUILayoutFromFile(const char *fileName, cocos2d::CCNode *parent, std::string UIName, bool bShow)
	{
		CCLOG("UIManager::CreateUILayoutFromFile : %s", fileName);
		UILayout *layout = NULL;
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			CCAssert(true,"uilayout already exists");
			return NULL;
		}

		layout = UICreator::CreateUILayoutFromFile(fileName, parent, bShow);
		if (layout)
		{
			m_UIMap[UIName] = layout;
		}

		//execute lua script if it exists
#ifndef TXGUIUTIL
		if (layout)
		{
			layout->executeLuaScript();
		}
#endif

		return layout;
	}

	   /*
		*@prototype, CreateUILayoutFromFile()
		*@param, fileName - plist file name
		*@param, UIName - ui name
		*@param, bShow - visible
		*/
	UILayout * UIManager::CreateUILayoutFromFile(const char *fileName,  std::string UIName, bool bShow)
	{
		 cocos2d::CCNode *parent = UIManager::sharedManager()->getUIDefaultRoot();
		 if (parent)
		 {
			 return CreateUILayoutFromFile(fileName, parent, UIName, bShow);
		 }else
		 {
			 return NULL;
		 }
	}

    void UIManager::AddLayout(UILayout* layout, std::string UIName)
    {
        if (layout)
        {
            m_UIMap[UIName] = layout;
        }
    }

    void UIManager::RemoveLayout(std::string name)
    {
        if(m_UIMap.find(name) != m_UIMap.end())
        {
            m_UIMap.erase(name);
        }
    }

	UILayout *UIManager::getUILayout(std::string UIName)
	{
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			return m_UIMap[UIName];
		}

		return NULL;
	}

	void UIManager::RegisterMenuHandler(std::string UIName, std::string name, SEL_MenuHandler pfn, CCObject* pSelect)
	{
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			m_UIMap[UIName]->RegisterMenuHandler(name, pfn, pSelect);
		}else{
            CCError("UIManager::RegisterMenuHandler error, can't find the UI:%s", UIName.c_str());
        }
	}

	void UIManager::UnregisterMenuHandler(std::string UIName, std::string name)
	{
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			m_UIMap[UIName]->UnregisterMenuHandler(name);
		}
	}

	void UIManager::UnregisterAllMenuHandler(std::string UIName)
	{
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			m_UIMap[UIName]->UnregisterAllMenuHandler();
		}
	}

	void UIManager::RemoveUILayout(std::string UIName)
	{
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			CCLOG("remove ui layout %s",UIName.c_str());
			UILayout *layout = m_UIMap[UIName];
			layout->UnregisterAllMenuHandler();
			delete layout;

			m_UIMap.erase(UIName);
		}
	}

	void UIManager::UpdateLayout(std::string UIName)
	{
		if(m_UIMap.find(UIName) != m_UIMap.end())
		{
			m_UIMap[UIName]->UpdateWorldPosition();
		}
	}

	cocos2d::CCPoint UIManager::containerCenterPosition(UIContainer *container) 
	{
		static float baseWidth = 1.0f * SRC_WIDTH / CC_CONTENT_SCALE_FACTOR();
		static float baseHeight = 1.0f * SRC_HEIGHT / CC_CONTENT_SCALE_FACTOR();

		return container->containerCenterPosition(CCSizeMake(baseWidth, baseHeight));
	}

	void UIManager::SetOtherUILayoutTouchEnableExcept(bool value,std::string UIName)
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->clearAllTouches();
		std::map<std::string, UILayout*>::iterator it = m_UIMap.begin();
		for(; it != m_UIMap.end(); it++)
		{
			UILayout* layer = it->second;
			if(layer->getName() != UIName)
			{
				layer->setTouchEnabled(value);
			}
			else
			{
				layer->setTouchEnabled(true);
			}
		}
	}
}