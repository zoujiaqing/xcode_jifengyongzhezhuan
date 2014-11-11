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
#include "UILabelAtlas.h"
#include "CrossPlatform.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"
#include "AspriteManager.h"
#include "UIDefine.h"

namespace TXGUI {
	USING_NS_CC;

	NiImplementRTTI(UILabelAtlas, UIControlBase);
	NiImplementCreateObject(UILabelAtlas);


	UILabelAtlas::UILabelAtlas(const char *label,
		const char *atlasFile,
		int itemWith,
		int itemHeight,
		unsigned int startChar, 
		cocos2d::CCNode* parent)
		:UIControlBase(parent)
	{
		m_pAtlasLabel = CCLabelAtlas::create(
			label,
			atlasFile,
			itemWith,
			itemHeight,
			startChar);
		m_pControlNode = m_pAtlasLabel;

		CCNode *pNode = getCurrentNode();
		parent->addChild(pNode);
	}

	UILabelAtlas::UILabelAtlas(CCLabelAtlas *pLabel, CCNode *parent)
		:UIControlBase(parent)
	{
		m_pAtlasLabel = pLabel;
		m_pControlNode = m_pAtlasLabel;

		parent->addChild(m_pAtlasLabel);
	}

	UILabelAtlas::UILabelAtlas(std::string name, int tag, float z, UIControlBase *parent, CCLabelAtlas *label)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pAtlasLabel = label;
		m_pControlNode = label;
		parent->getCurrentNode()->addChild(label, m_fPosZ);
	}

	UILabelAtlas::~UILabelAtlas()
	{
		// don't call this
		//delete m_pTTFLabel;
	}

	void UILabelAtlas::setString(const char *label)
	{
		if (m_pAtlasLabel)
			m_pAtlasLabel->setString(label);
	}

	const char* UILabelAtlas::getString()
	{
		if (m_pAtlasLabel)
			return m_pAtlasLabel->getString();

		return "";
	}

	void UILabelAtlas::setPosition(CCPoint pt) 
	{
		UIControlBase::setPosition(pt);

		if (m_pAtlasLabel)
		{
			m_pAtlasLabel->setPosition(pt);
		}
	}

	void UILabelAtlas::setScale(float scale)
	{
		UIControlBase::setScale(scale);
		if(m_pAtlasLabel != NULL)
		{
			m_pAtlasLabel->setScale(scale);
		}
	}

	void UILabelAtlas::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);
		if(m_pAtlasLabel != NULL)
		{
			m_pAtlasLabel->setScaleX(xScale);
			m_pAtlasLabel->setScaleY(yScale);
		}
	}

	void UILabelAtlas::setLabelAtlas(CCLabelAtlas *pLabel)
	{
		if(pLabel == NULL)
			return;

		if(m_pAtlasLabel != NULL)
		{
			m_pAtlasLabel->removeFromParentAndCleanup(true);
			m_pAtlasLabel = NULL;
		}

		m_pAtlasLabel = pLabel;
		m_pControlNode = m_pAtlasLabel;
		m_pControlNode->setPosition(m_ptLocalPosition);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pAtlasLabel, m_fPosZ);
		}
	}

	///------------------------------------------------------------------------
	//  streaming
	///------------------------------------------------------------------------
	UILabelAtlas::UILabelAtlas()
	{
		m_pAtlasLabel = NULL;
	}


	void UILabelAtlas::LoadBinary(NiStream &kStream)
	{
		UIControlBase::LoadBinary(kStream);

		mutableDic* dic = kStream.GetStreamData();
		// content attributes 		
		string strTemp = GetContent(dic, kStream);
		const char * content = strTemp.c_str();

		/// color attributes
		uint32_t color[3] = {255, 255, 255};
		GetColor(dic, kStream, color[0], color[1], color[2]);

		//bool hasRect = false;
		uint32_t useASposition = 0;
		std::string ASPositionName = "";
		kStream.getStringattributeValue(dic, "ASPositionName", ASPositionName);
		kStream.getIntAttributeValue(dic, "useASposition", useASposition);

		std::string ASfile = KUI_BIN;

		// if use Asprite Rect
		ASprite* as = GetASpriteCfg(kStream, dic);

		string fontFile = "";
		kStream.getStringattributeValue(dic, "fontFile", fontFile);
		//get stored full path
		fontFile = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(fontFile.c_str());

		uint32_t itemWidth = 0;
		uint32_t itemHeight = 0;
		kStream.getIntAttributeValue(dic, "itemWidth", itemWidth);
		kStream.getIntAttributeValue(dic, "itemHeight", itemHeight);
		itemWidth /= CCDirector::sharedDirector()->getContentScaleFactor();
		itemHeight /= CCDirector::sharedDirector()->getContentScaleFactor();

		uint32_t startChar = '0';
		kStream.getIntAttributeValue(dic, "startChar", startChar);

		if(useASposition)
		{
			int ID = getResourceIDByName(ASPositionName.c_str());

			ASprite *as = AspriteManager::getInstance()->getAsprite(ASfile);
			m_ptWorldPosition = as->GetFramePoint(ID);

			CCPoint parentWorldPosition = CCPointZero;
			if(m_pParent != NULL)
			{
				parentWorldPosition = m_pParent->getWorldPosition();
			}

			m_ptLocalPosition.x = m_ptWorldPosition.x - parentWorldPosition.x;
			m_ptLocalPosition.y = m_ptWorldPosition.y - parentWorldPosition.y;
		}

		// update CCLabel's position
		m_pAtlasLabel = CCLabelAtlas::create(
			content,
			fontFile.c_str(),
			itemWidth,
			itemHeight,
			(unsigned int)startChar
			);
		m_pAtlasLabel->setColor(ccc3(color[0], color[1], color[2]));

		// set control node to m_pTTFLabel;
		m_pControlNode = m_pAtlasLabel;
		m_pControlNode->setAnchorPoint(ccp(0,1.0f));
		m_pControlNode->setPosition(m_ptLocalPosition);
		m_pControlNode->setVisible(m_bIsVisible);
	}

	void UILabelAtlas::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UILabelAtlas::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
		{
			return false;           
		}

		// Register ExtraData

		return true;
	}

	void UILabelAtlas::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);

	}

	bool UILabelAtlas::IsEqual(NiObject *pObject)
	{
		if (! UIControlBase::IsEqual(pObject))
			return false;

		// Test for extra data equality, but ignore non-streamable data


		return true;
	}    
}
