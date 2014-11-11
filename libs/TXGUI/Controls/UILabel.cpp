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
#include "UILabel.h"
#include "CrossPlatform.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"
#include "AspriteManager.h"
//#include "Define.h"
#include "GameFontManager.h"

namespace TXGUI {
    USING_NS_CC;
    
    NiImplementRTTI(UILabel, UIControlBase);
    NiImplementCreateObject(UILabel);
    
    
    UILabel::UILabel(const char *label, const char *fontName, float fontSize, cocos2d::CCNode* parent)  
    :UIControlBase(parent)
    {
        m_pTTFLabel = UILabelTTF::create(label, fontName, fontSize);
        m_pControlNode = m_pTTFLabel;
        
        CCNode *pNode = getCurrentNode();
        parent->addChild(pNode);
    }

	UILabel::UILabel(UILabelTTF *pLabel, CCNode *parent)
		:UIControlBase(parent)
	{
		m_pTTFLabel = pLabel;
		m_pControlNode = m_pTTFLabel;

		parent->addChild(m_pTTFLabel);
	}

	UILabel::UILabel(std::string name, int tag, float z, UIControlBase *parent, UILabelTTF *label)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pTTFLabel = label;
		m_pControlNode = label;
		parent->getCurrentNode()->addChild(label, m_fPosZ);
	}
    
    UILabel::~UILabel()
    {
        // don't call this
        //delete m_pTTFLabel;
		m_pTTFLabel = NULL;
    }
    
    void UILabel::setString(const char *label)
    {
        if (m_pTTFLabel)
            m_pTTFLabel->setString(label);
    }
    
	void UILabel::setStringWithColor(const char *label,const ccColor3B& color)
	{
		if (m_pTTFLabel)
		{
			m_pTTFLabel->setStringWithColor(label,color);
		}
	}

    const char* UILabel::getString()
    {
        if (m_pTTFLabel)
            return m_pTTFLabel->getString();

		return "";
    }
    
    void UILabel::setPosition(CCPoint pt) 
    {
        UIControlBase::setPosition(pt);

        if (m_pTTFLabel)
		{
            m_pTTFLabel->setPosition(pt);
		}
    }

	void UILabel::setScale(float scale)
	{
		UIControlBase::setScale(scale);
		if(m_pTTFLabel != NULL)
		{
			m_pTTFLabel->setScale(scale);
		}
	}

	void UILabel::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);
		if(m_pTTFLabel != NULL)
		{
			m_pTTFLabel->setScaleX(xScale);
			m_pTTFLabel->setScaleY(yScale);
		}
	}

	void UILabel::setColor(const ccColor3B& color)
	{
		if (m_pTTFLabel)
		{
			m_pTTFLabel->setColor(color);
		}
	}

	void UILabel::setLabelTTF(UILabelTTF *pLabel)
	{
		if(pLabel == NULL)
			return;

		if(m_pTTFLabel != NULL)
		{
			m_pTTFLabel->removeFromParentAndCleanup(true);
			m_pTTFLabel = NULL;
		}

		m_pTTFLabel = pLabel;
		m_pControlNode = m_pTTFLabel;
		m_pControlNode->setPosition(m_ptLocalPosition);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pTTFLabel, m_fPosZ);
		}
	}
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UILabel::UILabel()
    {
        m_pTTFLabel = NULL;
    }
    
    
    void UILabel::LoadBinary(NiStream &kStream)
    {
		UIControlBase::LoadBinary(kStream);
                
        mutableDic* dic = kStream.GetStreamData();
        
		// content attributes         
        std::string strTemp = GetContent(dic, kStream);        
        const char* content = strTemp.c_str(); 
				
		uint32_t color[3] = {255, 255, 255};		
		GetColor(dic, kStream, color[0], color[1], color[2]);

		bool hasRect = false;
		uint32_t useASRect = 0;
		uint32_t rectWidth = 0;
		uint32_t rectHeight = 0;
		uint32_t alignment = kCCTextAlignmentCenter;
		uint32_t anchorPoint = 1;
		std::string ASRectName = "";
		ASprite* as = NULL;
		mutableDic *rectDic = static_cast<mutableDic*>(dic->objectForKey("rect"));
		if(rectDic)
		{
			// if use Asprite Rect
			kStream.getIntAttributeValue(rectDic, "useASRect", useASRect);
			if(useASRect == 1)
			{
				kStream.getStringattributeValue(rectDic, "ASRectName", ASRectName);				
				if(ASRectName != "")
				{
					as = GetASpriteCfg(kStream, rectDic);
					hasRect = true;
				}
			}
			else
			{
				kStream.getIntAttributeValue(rectDic, "width", rectWidth);
				kStream.getIntAttributeValue(rectDic, "height", rectHeight);
                rectWidth /= CC_CONTENT_SCALE_FACTOR();
                rectHeight /= CC_CONTENT_SCALE_FACTOR();
				if(rectWidth > 0 && rectHeight > 0)
				{
					hasRect = true;
				}
			}

			kStream.getIntAttributeValue(rectDic, "alignment", alignment);
			kStream.getIntAttributeValue(rectDic, "anchorPoint", anchorPoint);
			// check the alignment
			if(alignment < kCCTextAlignmentLeft || alignment > kCCTextAlignmentRight)
			{
				alignment = kCCTextAlignmentCenter;
			}
		}

		string font = KJLinXin;
		kStream.getStringattributeValue(dic, "font", font);

		float contentScale = CCDirector::sharedDirector()->getContentScaleFactor();

        float largeFontSize = GameFontManager::largeFontSize();
		uint32_t fontSize = largeFontSize * contentScale;
		kStream.getIntAttributeValue(dic, "fontSize", fontSize);
        fontSize /= contentScale;

		if(hasRect)
		{
			if(useASRect)
			{
				int ID = getResourceIDByName(ASRectName.c_str());
				// name not exists, use normal method
				if(ID == -1)
				{
					hasRect = false;
				}
				else
				{					
					CCRect asRect = as->getframeRect(ID);
					unsigned int defaultSize = 0;
					kStream.getIntAttributeValue(dic, "defaultSize", defaultSize);
					if(defaultSize > 0)
					{
						m_pTTFLabel = UILabelTTF::create(
							content,
							font.c_str(),
							fontSize,
							CCSizeZero,
							(CCTextAlignment)alignment,
							kCCVerticalTextAlignmentCenter
							);
					}
					else
					{
						m_pTTFLabel = UILabelTTF::create(
							content,
							font.c_str(),
							fontSize,
							asRect.size,
							(CCTextAlignment)alignment,
							kCCVerticalTextAlignmentCenter
							);
					}
					

					switch(anchorPoint)
					{
					case 0:
						{
							CCRect rect = as->getframeRect(ID);
							m_ptWorldPosition = rect.origin;
						}
						break;

					case 1:
						m_ptWorldPosition = as->GetFramePointMiddle(ID);
						break;

					default:
						break;
					}

					CCPoint parentWorldPosition = CCPointZero;
					if(m_pParent != NULL)
					{
						parentWorldPosition = m_pParent->getWorldPosition();
					}
					
					m_ptLocalPosition.x = m_ptWorldPosition.x - parentWorldPosition.x;
					m_ptLocalPosition.y = m_ptWorldPosition.y - parentWorldPosition.y;
				}
			}
			else
			{
				m_pTTFLabel = UILabelTTF::create(
					content,
					font.c_str(),
					fontSize,
					CCSizeMake(rectWidth, rectHeight),
					(CCTextAlignment)alignment,
					kCCVerticalTextAlignmentCenter
					);
			}
		}

		if(!hasRect)
		{
			m_pTTFLabel = UILabelTTF::create(content, font.c_str(), (float)fontSize);
		}

        
        // update CCLabel's position
        //m_pTTFLabel->setOpacityModifyRGB(false);
		m_pTTFLabel->setColor(ccc3(color[0], color[1], color[2]));
        // set control node to m_pTTFLabel;
		m_pControlNode = m_pTTFLabel;
		m_pControlNode->setPosition(m_ptLocalPosition);
		m_pControlNode->setVisible(m_bIsVisible);

#if _DEBUG
		m_pTTFLabel->m_name = m_strName;
#endif

    }
    
    void UILabel::LinkObject(NiStream &kStream)
    {
        UIControlBase::LinkObject(kStream);
    }
    
    bool UILabel::RegisterStreamables(NiStream &kStream)
    {
        if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UILabel::SaveBinary(NiStream &kStream)
    {
        UIControlBase::SaveBinary(kStream);
        
    }
    
    bool UILabel::IsEqual(NiObject *pObject)
    {
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }    
}
