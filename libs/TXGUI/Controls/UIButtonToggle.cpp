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
#include "UIButtonToggle.h"
#include "AspriteManager.h"


#include "UIManager.h"
#include "UICreator.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"

#include "CrossPlatform.h"

namespace TXGUI {
    USING_NS_CC;
    
    NiImplementRTTI(UIButtonToggle, UIButton);
    NiImplementCreateObject(UIButtonToggle);
    
    
    UIButtonToggle::UIButtonToggle(cocos2d::CCNode* parent)  
    :UIButton(parent)
    {        
        m_pToggle = NULL;
    }

	UIButtonToggle::UIButtonToggle(std::string name, int tag, float z, UIControlBase *parent, CCMenuItemToggle *toggle)
		:UIButton(name, tag, z, parent, NULL, NULL,-128)
	{
		m_pToggle = NULL;
		setMenuItemToggle(toggle);
	}	
	
	UIButtonToggle::UIButtonToggle(
		std::string name,
		int tag,
		float z,
		UIControlBase *parent,
		CCMenuItemToggle *toggle,
		std::string clickedFunc
		)
		:UIButton(name, tag, z, parent, NULL, NULL,-128, clickedFunc)
	{
		m_pToggle = NULL;
		setMenuItemToggle(toggle);
	}
    
    UIButtonToggle::~UIButtonToggle()
    {
        
    }  
	    
	void UIButtonToggle::setPosition(CCPoint pt) 
    {
        UIButton::setPosition(pt);
        
    }

	void UIButtonToggle::setScale(float scale)
	{
		UIButton::setScale(scale);
	}

	void UIButtonToggle::setScale(float xScale, float yScale)
	{
		UIButton::setScale(xScale, yScale);
		if(m_pToggle != NULL)
		{
			m_pToggle->setScaleX(xScale);
			m_pToggle->setScaleY(yScale);
		}
	}

	void UIButtonToggle::setMenuItemToggle(CCMenuItemToggle *pToggle)
	{
		if(pToggle == NULL)
			return;

		if(m_pToggle != NULL)
		{
			m_pToggle->removeFromParentAndCleanup(true);
			m_pToggle = NULL;
		}

		if(m_strClickedFun != "")
		{
			pToggle->setTarget(this, menu_selector(UIButtonToggle::Clicked));
		}

		pToggle->setTag(m_iTag);
		m_pToggle = pToggle;
		m_pToggle->setPosition(m_ptLocalPosition);
		m_pBtnMenu->addChild(pToggle, 0);
	}

	void UIButtonToggle::setClickedFunc(std::string func)
	{
		UIButton::setClickedFunc(func);

		if(func != "")
		{
			if(m_pToggle != NULL)
			{
				m_pToggle->setTarget(this, menu_selector(UIButtonToggle::Clicked));
			}
		}
		else
		{			
			if(m_pToggle != NULL)
			{
				m_pToggle->setTarget(NULL, NULL);
			}
		}
	}

    void UIButtonToggle::Clicked(CCObject* pObject)
    {
        UIButton::Clicked(pObject);
    }
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UIButtonToggle::UIButtonToggle()
    {
        m_pToggle = NULL;
    }
    
    
    void UIButtonToggle::LoadBinary(NiStream &kStream)
    {
        // streaming  button
        UIButton::LoadBinary(kStream); 
        
        // streaming menu item toggle
		if(m_pButtonItem)
		{
			m_pButtonItem->setTarget(NULL, NULL);
			m_pBtnMenu->removeChild(m_pButtonItem, false);
			m_pButtonItem->setParent(NULL);
			m_pButtonItem->setPosition(ccp(m_pButtonItem->getContentSize().width / 2, 
			m_pButtonItem->getContentSize().height / 2));
		}
        
        
        m_pToggle = CCMenuItemToggle::createWithTarget(this, 
                                                     menu_selector(UIButtonToggle::Clicked), 
                                                     NULL, NULL);
		m_pToggle->setTag(m_iTag);
		//m_pToggle->setPosition(m_ptLocalPosition);
		m_pToggle->setPosition(CCPointZero);
        m_pBtnMenu->addChild(m_pToggle, 0);
        
        mutableDic* dic = kStream.GetStreamData();   
        mutableArray* optionbuttons = static_cast<mutableArray*>(dic->objectForKey("toggleButtons")); 
        
		uint32_t useASposition = 0;
		kStream.getIntAttributeValue(dic, "useASposition", useASposition);

        
        vector<string> images;

        for(int i = 0; i < (int)optionbuttons->count(); i ++)
        {
            // texture attributes of optional button 
            mutableDic* texDic =  static_cast<mutableDic*>(optionbuttons->objectAtIndex(i));        
            string normalTex = "", clickedTex = "", disableTex = "";
			string binName = "";
			bool hasDisableTex = false;
            kStream.getStringattributeValue(texDic, "normalTexture", normalTex);
            kStream.getStringattributeValue(texDic, "clickedTexture", clickedTex);
			kStream.getStringattributeValue(texDic, "disableTexture", disableTex);

			hasDisableTex = (disableTex != "");

			mutableDic* binarydic = static_cast<mutableDic*>(texDic->objectForKey("AspriteManager"));
			if(binarydic)
			{
				if(IsIpad())
				{
					kStream.getStringattributeValue(binarydic, "ipad", binName);
				}
				else
				{
					kStream.getStringattributeValue(binarydic, "iphone", binName);
				}
                
                /// add images for mutilple Images usage in AuroraGT Tools
				mutableDic* dicImages = static_cast<mutableDic*>(binarydic->objectForKey("images"));
				if (dicImages)
				{
					string imgVal ="";
					uint32_t nImages = 0;
					kStream.getIntAttributeValue(dicImages, "imagecount", nImages);
					for(uint32_t i = 0; i < nImages; i++)
					{
						char key[32] = "";
						sprintf(key, "%d", i);
						kStream.getStringattributeValue(dicImages, key, imgVal);
						images.push_back(imgVal);
					}
				}
			}
            
            

            CCPoint pt;
            ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);

			if(binName != "")
			{
				as = AspriteManager::getInstance()->getAsprite(binName);
                /// set up as sprite images
                for(size_t i = 0; i < images.size(); i++)
                {
                    if (as)
                    {
                        as->setImageName(i, images[i]);
                    }
                }
			}
            
			CCSprite * clickedSprite;
			CCSprite * normalSprite;
			CCSprite * disableSprite;
			CCMenuItemSprite* item;
			clickedSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(clickedTex.c_str()), 0, pt);
			normalSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(normalTex.c_str()), 0, pt);
			if(hasDisableTex)
			{
				disableSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(disableTex.c_str()), 0, pt);

				item = CCMenuItemSprite::create(
					normalSprite, 
					clickedSprite, 
					disableSprite, 
					NULL, 
					NULL
					);
			}
			else
			{
				item = CCMenuItemSprite::create(normalSprite, clickedSprite);
			}
			/*item->setPosition(ccp(clickedSprite->getContentSize().width / 2, 
			clickedSprite->getContentSize().height / 2));*/
			item->setPosition(CCPointZero);
            m_pToggle->addSubItem(item);

			if(useASposition == 1)
			{
				CCPoint parentWorldPosition = CCPointZero;
				if(m_pParent != NULL)
				{
					parentWorldPosition = m_pParent->getWorldPosition();
				}
                
				m_ptLocalPosition.x = pt.x - parentWorldPosition.x;
				m_ptLocalPosition.y = pt.y - parentWorldPosition.y;
			}
			else
			{
				// we modified it, so reset it
				UIControlBase::CalWorldPos();
			}
        } 
        
        m_pToggle->setSelectedIndex(0);
		//m_pToggle->setIsTouchEnabled(m_bIsTouchEnabled);
		m_pToggle->setEnabled(m_bIsEnabled);
    }
    
    void UIButtonToggle::LinkObject(NiStream &kStream)
    {
        UIButton::LinkObject(kStream);
    }
    
    bool UIButtonToggle::RegisterStreamables(NiStream &kStream)
    {
        if (! UIButton::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UIButtonToggle::SaveBinary(NiStream &kStream)
    {
        UIButton::SaveBinary(kStream);
        
    }
    
    bool UIButtonToggle::IsEqual(NiObject *pObject)
    {
        if (! UIButton::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }    

	void UIButtonToggle::BindNode(CCNode* pParentNode)
	{
		UIButton::BindNode(pParentNode);
	}

	void UIButtonToggle::selected()
	{
		if(m_pToggle)
		{
			unsigned int index = m_pToggle->getSelectedIndex();
			if(index  == 0)
			{
				m_pToggle->setSelectedIndex(1);
			}
		}
	}
	void UIButtonToggle::unselected()
	{
		if(m_pToggle)
		{
			unsigned int index = m_pToggle->getSelectedIndex();
			if(index  == 1)
			{
				m_pToggle->setSelectedIndex(0);
			}
		}
	}

	CCSize UIButtonToggle::getButtonSize()
	{
		if(m_pToggle)
		{
			CCSize size = m_pToggle->getContentSize();
			size.width /= CC_CONTENT_SCALE_FACTOR();
			size.height /= CC_CONTENT_SCALE_FACTOR();
			return size;
		}
		return CCSizeZero;
	}
}