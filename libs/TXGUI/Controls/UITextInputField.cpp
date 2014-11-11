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
#include "UITextInputField.h"
#include "CrossPlatform.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"
#include "GameFontManager.h"
#include "AspriteManager.h"

namespace TXGUI {
	USING_NS_CC;

	NiImplementRTTI(UITextInputField, UIControlBase);
	NiImplementCreateObject(UITextInputField);

	UITextInputField::UITextInputField(const char *label, 
		cocos2d::CCSize dimension, 
		cocos2d::CCTextAlignment alignment,
		const char *fontName,
		float fontSize,
		cocos2d::CCNode* parent
		)  
		:UIControlBase(parent)
	{
		m_pTextInputField = TextInputField::textFieldWithPlaceHolder(label, dimension, alignment, fontName, fontSize);
		m_pControlNode = m_pTextInputField;

		CreateWhiteBack();

		if(m_pWhiteBack != NULL)
		{
			parent->addChild(m_pWhiteBack);
		}

		CCNode *pNode = getCurrentNode();
		parent->addChild(pNode);
	}

	UITextInputField::UITextInputField(const char *label, const char *fontName, float fontSize, cocos2d::CCNode* parent)  
		:UIControlBase(parent)
	{
		m_pTextInputField = TextInputField::textFieldWithPlaceHolder(label, fontName, fontSize);
		m_pControlNode = m_pTextInputField;

		CreateWhiteBack();

		if(m_pWhiteBack != NULL)
		{
			parent->addChild(m_pWhiteBack);
		}

		CCNode *pNode = getCurrentNode();
		parent->addChild(pNode);
	}

	UITextInputField::UITextInputField(TextInputField *pTextInputField, CCNode *parent)
		:UIControlBase(parent)
	{
		m_pTextInputField = pTextInputField;
		m_pControlNode = m_pTextInputField;

		CreateWhiteBack();

		if(m_pWhiteBack != NULL)
		{
			parent->addChild(m_pWhiteBack);
		}

		CCNode *pNode = getCurrentNode();
		parent->addChild(pNode);
	}

	UITextInputField::UITextInputField(std::string name,
		int tag,
		float z,
		UIControlBase *parent,
		TextInputField *textInputField
		)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pTextInputField = textInputField;
		m_pControlNode = textInputField;

		CreateWhiteBack();

		if(m_pWhiteBack != NULL)
		{
			parent->getCurrentNode()->addChild(m_pWhiteBack, m_fPosZ);
		}

		parent->getCurrentNode()->addChild(textInputField);
	}

	UITextInputField::~UITextInputField()
	{

	}   

	void UITextInputField::BindNode(CCNode* pParentNode)
	{
		if(m_pWhiteBack != NULL)
		{
			pParentNode->addChild(m_pWhiteBack, m_fPosZ);
		}

		UIControlBase::BindNode(pParentNode);
	}

	void UITextInputField::setString(const char *label)
	{
		if (m_pTextInputField)
			m_pTextInputField->resetInputText(label);
	}

	const char* UITextInputField::getString()
	{
		if (m_pTextInputField)
			return m_pTextInputField->getString();

		return "";
	}

	void UITextInputField::setPosition(CCPoint pt)
	{
		UIControlBase::setWorldPosition(pt);

		if(m_pWhiteBack)
		{
			m_pWhiteBack->setPosition(pt);
		}

		if (m_pTextInputField)
		{
			m_pTextInputField->setPosition(pt);
		}
	}

	void UITextInputField::setScale(float scale)
	{
		UIControlBase::setScale(scale);
		if(m_pWhiteBack != NULL)
		{
			m_pWhiteBack->setScale(scale);
		}

		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->setScale(scale);
		}
	}

	void UITextInputField::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);
		if(m_pWhiteBack != NULL)
		{
			m_pWhiteBack->setScaleX(xScale);
			m_pWhiteBack->setScaleY(yScale);
		}

		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->setScaleX(xScale);
			m_pTextInputField->setScaleY(yScale);
		}
	}

	void UITextInputField::setTextInputField(TextInputField *pTextInputField)
	{
		if(pTextInputField == NULL)
			return;

		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->removeFromParentAndCleanup(true);
			m_pTextInputField = NULL;
		}

		m_pTextInputField = pTextInputField;
		m_pControlNode = m_pTextInputField;
		m_pControlNode->setPosition(m_ptLocalPosition);

		CreateWhiteBack();

		if(m_pParentNode != NULL)
		{
			if(m_pWhiteBack != NULL)
			{
				m_pWhiteBack->setPosition(m_ptLocalPosition);
				m_pParentNode->addChild(m_pWhiteBack, m_fPosZ);
			}

			m_pParentNode->addChild(m_pTextInputField, m_fPosZ);
		}
	}

	void UITextInputField::setLengthLimit(int value)
	{
		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->setLengthLimit(value);
		}
	}

	void UITextInputField::setIsPassword(bool value)
	{
		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->setIsPassword(value);
		}
	}

	void UITextInputField::setIsVisible(bool isVisible)
	{
		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->setVisible(isVisible);
		}

		if(m_pWhiteBack != NULL)
		{
			m_pWhiteBack->setVisible(isVisible);
		}
	}

	void UITextInputField::clearString()
	{
		if(m_pTextInputField != NULL)
		{
			m_pTextInputField->clearString();
		}
	}
    
    void UITextInputField::becomeFirstResponder() {
        if (m_pTextInputField != NULL) {
            m_pTextInputField->attachWithIME();
        }
    }
    
    void UITextInputField::resignFirstResponder() {
        if (m_pTextInputField != NULL) {
            m_pTextInputField->detachWithIME();
        }
    }
    

	void UITextInputField::CreateWhiteBack()
	{
		if(m_pWhiteBack != NULL)
		{
			m_pWhiteBack->removeFromParentAndCleanup(true);
			m_pWhiteBack = NULL;
		}

		if(m_pTextInputField == NULL)
		{
			return;
		}

		CCSize dimension = m_pTextInputField->getDimensions();
		int dimensionWidth = dimension.width;
		int dimensionHeight = dimension.height;

		if(dimensionWidth != 0 && dimensionHeight != 0)
		{
			int *pixels = new int[(dimensionWidth + 8) * (dimensionHeight + 8)];
			for (int i=0; i<dimensionWidth+8; ++i) {
				for (int j=0; j<dimensionHeight+8; ++j) {
					//pixels[i][j] = 0xffffffff;
					pixels[i*4+j] = 0xffffffff;
				}
			}

			CCTexture2D *texture = new CCTexture2D();
			texture->initWithData(pixels,
				kCCTexture2DPixelFormat_RGB888,
				1,
				1,
				CCSizeMake(dimensionWidth + 8, dimensionHeight + 8)
				);

			delete[] pixels;

			m_pWhiteBack = CCSprite::createWithTexture(texture);
			texture->release();
		}
	}

	///------------------------------------------------------------------------
	//  streaming
	///------------------------------------------------------------------------
	UITextInputField::UITextInputField()
	{
		m_pTextInputField = NULL;
		m_pWhiteBack = NULL;
	}


	void UITextInputField::LoadBinary(NiStream &kStream)
	{
		UIControlBase::LoadBinary(kStream);    

		mutableDic* dic = kStream.GetStreamData();
        
		float contentScale = CCDirector::sharedDirector()->getContentScaleFactor();
		
		uint32_t bLocalizeString = 0;
		kStream.getIntAttributeValue(dic, "bLocalizeString", bLocalizeString);

		std::string strTemp = "";
		kStream.getStringattributeValue(dic, "placeholder", strTemp);
		const char* placeholder = strTemp.c_str();
		if (bLocalizeString > 0)
		{
			placeholder = CCLocalizedString(strTemp.c_str(), strTemp.c_str());
		}

		uint32_t lengthLimit = 0;
		kStream.getIntAttributeValue(dic, "lengthLimit", lengthLimit);

		uint32_t isPassword = 0;
		kStream.getIntAttributeValue(dic, "isPassword", isPassword);

		int priority = kCCMenuHandlerPriority;
		kStream.getSignedIntAttributeValue(dic,"priority",priority);

		uint32_t dimensionWidth = 0;
		uint32_t dimensionHeight = 0;
		kStream.getIntAttributeValue(dic, "dimensionWidth", dimensionWidth);
		kStream.getIntAttributeValue(dic, "dimensionHeight", dimensionHeight);
        
        dimensionWidth /= contentScale;
        dimensionHeight /= contentScale;

		uint32_t alignment = 1;
		kStream.getIntAttributeValue(dic, "alignment", alignment);

		mutableDic *colorDic = static_cast<mutableDic*>(dic->objectForKey("color"));
		uint32_t color[3] = {255, 255, 255};
		if(colorDic)
		{
			kStream.getIntAttributeValue(colorDic, "red", color[0]);
			kStream.getIntAttributeValue(colorDic, "green", color[1]);
			kStream.getIntAttributeValue(colorDic, "blue", color[2]);
		}

		string font = KJLinXin;
		kStream.getStringattributeValue(dic, "font", font);

        float largeFontSize = GameFontManager::largeFontSize();
		uint32_t fontSize = largeFontSize * contentScale;
		kStream.getIntAttributeValue(dic, "fontSize", fontSize);
		fontSize /= contentScale;

		CCSize dimension = CCSizeMake(dimensionWidth, dimensionHeight);
		if(dimensionWidth > 0 && dimensionHeight < fontSize)
		{
			dimension = CCSizeMake(dimensionWidth, fontSize);
		}
        
		CCSize touchSize = CCSizeZero;
		mutableDic *rectDic = static_cast<mutableDic*>(dic->objectForKey("rect"));
		if(rectDic)
		{
			uint32_t useASRect = 0;
			std::string ASRectName = "";
			std::string ASfile = KUI_BIN;
			uint32_t anchorPoint = 0;
			// if use Asprite Rect
			kStream.getIntAttributeValue(rectDic, "useASRect", useASRect);
			kStream.getIntAttributeValue(rectDic, "anchorPoint", anchorPoint);
			if(useASRect == 1)
			{
				kStream.getStringattributeValue(rectDic, "ASRectName", ASRectName);
				if(ASRectName != "")
				{
					mutableDic* dicAsMgr = static_cast<mutableDic*>(rectDic->objectForKey("AspriteManager"));
					// content attributes 
					if (dicAsMgr)
					{
						if (IsIpad())
						{
							kStream.getStringattributeValue(dicAsMgr, "ipad", ASfile); 
						}else
						{
							kStream.getStringattributeValue(dicAsMgr, "iphone", ASfile); 
						}
					}
					int ID = getResourceIDByName(ASRectName.c_str());
					// name not exists, use normal method
					if(ID != -1)
					{
						ASprite *as = AspriteManager::getInstance()->getAsprite(ASfile);
						CCRect asRect = as->getframeRect(ID);
						touchSize = asRect.size;
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
			}
		}

		m_pTextInputField = TextInputField::textFieldWithPlaceHolder(placeholder,
			dimension,
			(CCTextAlignment)alignment,
			font.c_str(),
			(float)fontSize
			);

		m_pTextInputField->setAnchorPoint(ccp(0.5, 0.5));
		m_pTextInputField->setPosition(m_ptLocalPosition);
		m_pTextInputField->setLengthLimit(lengthLimit);
		m_pTextInputField->setIsPassword(isPassword != 0);
		m_pTextInputField->setColor(ccc3(color[0], color[1], color[2]));
		m_pTextInputField->setInitPriority(priority);
		m_pTextInputField->setTouchInputSize(touchSize);
		// make background
		CreateWhiteBack();
		if(m_pWhiteBack != NULL)
		{
			m_pWhiteBack->setPosition(m_ptLocalPosition);
			m_pWhiteBack->setVisible(m_bIsVisible);
		}

		m_pControlNode = m_pTextInputField;
		m_pControlNode->setVisible(m_bIsVisible);
	}

	void UITextInputField::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UITextInputField::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
		{
			return false;           
		}

		// Register ExtraData

		return true;
	}

	void UITextInputField::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);

	}

	bool UITextInputField::IsEqual(NiObject *pObject)
	{
		if (! UIControlBase::IsEqual(pObject))
			return false;

		// Test for extra data equality, but ignore non-streamable data


		return true;
	}    
}
