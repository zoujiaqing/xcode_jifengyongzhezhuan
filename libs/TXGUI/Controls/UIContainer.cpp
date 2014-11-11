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
// Created by benyang on 12-7-20.
//

#include <iostream>

#include "CCDirector.h"
#include "UIContainer.h"
#include "NiObject.h"
#include "CrossPlatform.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"

namespace TXGUI
{
	USING_NS_CC;

	NiImplementRTTI(UIContainer, UIControlBase);
	NiImplementCreateObject(UIContainer);

	UIContainer::UIContainer(CCNode *pParentNode)
		: UIControlBase(pParentNode)
	{
		m_refMode = NO_REFERENCE;
		m_refPointMode = NONE;
		m_widthScale = m_widthOffset = m_heightScale = m_heightOffset = 0.0f;
		m_size = CCSizeZero;
		m_refObj = NULL;
		m_refObjName = "";

		m_pControlNode = CCNode::create();
		m_pControlNode->setAnchorPoint(CCPointZero);
		m_pControlNode->setPosition(CCPointZero);
		
		CCNode *pNode = getCurrentNode();
		pParentNode->addChild(pNode);
	}

	UIContainer::~UIContainer()
	{

	}

	void UIContainer::setPosition(CCPoint pt)
	{
		UIControlBase::setPosition(pt);

		if(m_pControlNode != NULL)
		{
			m_pControlNode->setPosition(pt);
		}
	}

	void UIContainer::UpdateWorldPosition()
	{
		if(m_refMode == NO_REFERENCE)
		{
			UIControlBase::UpdateWorldPosition();
		}
		else
		{
			UpdateChildrenPos();
		}
	}

	void UIContainer::setScale(float scale)
	{
		if(!m_bScaleSetted)
		{
			m_widthOffset = m_widthOffset * scale;
			m_heightOffset = m_heightOffset * scale;
			m_size = CCSize(m_size.width * scale, m_size.height * scale);
		}

		UIControlBase::setScale(scale);
	}

	void UIContainer::setScale(float xScale, float yScale)
	{
		if(!m_bScaleSetted)
		{
			m_widthOffset = m_widthOffset * xScale;
			m_heightOffset = m_heightOffset * yScale;
			m_size = CCSize(m_size.width * xScale, m_size.height * yScale);
		}

		UIControlBase::setScale(xScale, yScale);
	}

	void UIContainer::setRefMode(ReferenceMode refMode)
	{
		m_refMode = refMode;

		if(NO_REFERENCE > m_refMode || m_refMode > OBJECT_REFERENCE)
		{
			m_refMode = NO_REFERENCE;
		}
	}

	ReferenceMode UIContainer::getRefMode()
	{
		return m_refMode;
	}

	void UIContainer::setRefPointMode(ReferencePoint refPtMode)
	{
		m_refPointMode = refPtMode;
		if(NONE > refPtMode || refPtMode > RIGHT_BOTTOM)
		{
			m_refPointMode = NONE;
		}
	}

	ReferencePoint UIContainer::getRefPointMode()
	{
		return m_refPointMode;
	}

	void UIContainer::setWidthScale(float scale)
	{
		if(0.0f <= scale && scale <= 1.0f)
		{
			m_widthScale = scale;
		}
	}

	float UIContainer::getWidthScale()
	{
		return m_widthScale;
	}

	void UIContainer::setWidthOffset(float value)
	{
		m_widthOffset = value;
	}

	float UIContainer::getWidthOffset()
	{
		return m_widthOffset;
	}

	void UIContainer::setHeightScale(float scale)
	{
		if(0.0f <= scale && scale <= 1.0f)
		{
			m_heightScale = scale;
		}
	}

	float UIContainer::getHeightScale()
	{
		return m_heightScale;
	}

	void UIContainer::setHeightOffset(float value)
	{
		m_heightOffset = value;
	}

	float UIContainer::getHeightOffset()
	{
		return m_heightOffset;
	}

	void UIContainer::setSize(CCSize size)
	{
		m_size = size;
	}

	void UIContainer::setSize(int width, int height)
	{
		m_size = CCSize(width, height);
	}

	CCSize UIContainer::getSize()
	{
		return m_size;
	}

	void UIContainer::setRefObjName(std::string name)
	{
		m_refObjName = name;
	}

	std::string UIContainer::getRefObjName()
	{
		return m_refObjName;
	}

	void UIContainer::setRefObj(UIContainer *refObj)
	{
		m_refObj = refObj;
	}

	UIContainer *UIContainer::getRefObj()
	{
		return m_refObj;
	}

	void UIContainer::CalWorldPos()
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CalWorldPosWithSize(winSize);
	}

	void UIContainer::preCalWorldPos()
	{
        float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
		float refWidth = REF_LEVEL_WIDTH;
		float refHeight= REF_LEVEL_HEIGHT;
        CCSize winSize = CCSizeMake(refWidth / deviceScale, refHeight / deviceScale);
		CalWorldPosWithSize(winSize);
	}

	void UIContainer::CalWorldPosWithSize(CCSize size)
	{
		CCPoint centerWorldPosition = containerCenterPosition(size);
		setWorldPosition(centerWorldPosition);
		CalLocalPos();
	}

	void UIContainer::CalLocalPos()
	{
		if(m_pParent != NULL)
		{
			CCPoint parentWorldPos = m_pParent->getWorldPosition();
			CCPoint selfWorldPos = getWorldPosition();

			setPosition(ccp(selfWorldPos.x - parentWorldPos.x, selfWorldPos.y - parentWorldPos.y));
		}
		else
		{
			setPosition(getWorldPosition());
		}
	}

	CCPoint UIContainer::containerCenterPosition(CCSize targetWinSize)
	{
		CCPoint ret = CCPointZero;

		if (m_refMode == NORMAL_REFERENCE)
		{
			switch (m_refPointMode)
			{
			case NONE:
				{
					CCLOG("wrong reference point mode for %s", getName().c_str());
					return CCPointZero;
				}
				break;

			case LEFT_TOP:
			case MIDDLE_TOP:
				{
					ret.x = targetWinSize.width * m_widthScale + m_widthOffset;
					ret.y = targetWinSize.height * (1 - m_heightScale) - m_heightOffset;
				}
				break;

			case LEFT_MIDDLE:
			case LEFT_BOTTOM:
			case MIDDLE_MIDDLE:
			case MIDDLE_BOTTOM:
				{
					ret.x = targetWinSize.width * m_widthScale + m_widthOffset;
					ret.y = targetWinSize.height * m_heightScale + m_heightOffset;
				}
				break;

			case RIGHT_TOP:
				{
					ret.x = targetWinSize.width * (1 - m_widthScale) - m_widthOffset;
					ret.y = targetWinSize.height * (1 - m_heightScale) - m_heightOffset;
				}
				break;

			case RIGHT_MIDDLE:
			case RIGHT_BOTTOM:
				{
					ret.x = targetWinSize.width * (1 - m_widthScale) - m_widthOffset;
					ret.y = targetWinSize.height * m_heightScale + m_heightOffset;
				}
				break;

			default:
				{
					CCLOG("wrong reference point mode for %s", getName().c_str());
					return CCPointZero;
				}
				break;
			}
		}
		else if(m_refMode == OBJECT_REFERENCE)
		{
			if(m_refObj == NULL)
			{
				CCLOG("need an object to be referenced to for %s", getName().c_str());
				return CCPointZero;
			}

			m_refObj->CalWorldPos();

			CCPoint objWorldPos = m_refObj->getWorldPosition();
			CCSize objSize = m_refObj->getSize();
			CCSize selfSize = getSize();

			switch (m_refPointMode)
			{
			case NONE:
				{
					CCLOG("wrong reference point mode for %s", getName().c_str());
					return CCPointZero;
				}
				break;

			case LEFT_TOP:
				{
					ret.x = objWorldPos.x - objSize.width / 2 - selfSize.width * m_widthScale
						- m_widthOffset - selfSize.width / 2;
					ret.y = objWorldPos.y + objSize.height / 2 + selfSize.height * m_heightScale
						+ m_heightOffset + selfSize.height / 2;
				}
				break;

			case LEFT_MIDDLE:
				{
					ret.x = objWorldPos.x - objSize.width / 2 - selfSize.width * m_widthScale
						- m_widthOffset - selfSize.width / 2;
					ret.y = objWorldPos.y - objSize.height / 2 + objSize.height * m_heightScale
						+ m_heightOffset;
				}
				break;

			case LEFT_BOTTOM:
				{
					ret.x = objWorldPos.x - objSize.width / 2 - selfSize.width * m_widthScale
						- m_widthOffset - selfSize.width / 2;
					ret.y = objWorldPos.y - objSize.height / 2 - selfSize.height * m_heightScale
						- m_heightOffset - selfSize.height / 2;
				}
				break;

			case MIDDLE_TOP:
				{
					ret.x = objWorldPos.x - objSize.width / 2 + objSize.width * m_widthScale
						+ m_widthOffset;
					ret.y = objWorldPos.y + objSize.height / 2 + selfSize.height * m_heightScale
						+ m_heightOffset + selfSize.height / 2;
				}
				break;

			case MIDDLE_MIDDLE:
				{
					ret.x = objWorldPos.x - objSize.width / 2 + objSize.width * m_widthScale + m_widthOffset;
					ret.y = objWorldPos.y - objSize.height / 2 + objSize.height * m_heightScale + m_heightOffset;
				}
				break;

			case MIDDLE_BOTTOM:
				{
					ret.x = objWorldPos.x - objSize.width / 2 + objSize.width * m_widthScale + m_widthOffset;
					ret.y = objWorldPos.y - objSize.height / 2 - selfSize.height * m_heightScale
						- m_heightOffset - selfSize.height / 2;
				}
				break;

			case RIGHT_TOP:
				{
					ret.x = objWorldPos.x + objSize.width / 2 + selfSize.width * m_widthScale
						+ m_widthOffset + selfSize.width / 2;
					ret.y = objWorldPos.y + objSize.height / 2 + selfSize.height * m_heightScale
						+ m_heightOffset + selfSize.height / 2;
				}
				break;

			case RIGHT_MIDDLE:
				{
					ret.x = objWorldPos.x + objSize.width / 2 + selfSize.width * m_widthScale
						+ m_widthOffset + selfSize.width / 2;
					ret.y = objWorldPos.y - objSize.height / 2 + objSize.height * m_heightScale + m_heightOffset;
				}
				break;

			case RIGHT_BOTTOM:
				{
					ret.x = objWorldPos.x + objSize.width / 2 + selfSize.width * m_widthScale
						+ m_widthOffset + selfSize.width / 2;
					ret.y = objWorldPos.y - objSize.height / 2 - selfSize.height * m_heightScale
						- m_heightOffset - selfSize.height / 2;
				}
				break;

			default:
				{
					CCLOG("wrong reference point mode for %s", getName().c_str());
					return CCPointZero;
				}
				break;
			}
		}
		else
		{
			CCPoint localPt = getPosition();
			ret = LocalPt2World(localPt);
		}

		return ret;
	}

	// set children position according to the reference point
	void UIContainer::UpdateChildrenPos()
	{
		CalWorldPos();

		for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
			it != m_ChildrenControls.end(); it++)
		{
			(*it)->UpdateWorldPosition();
		}
	}

    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
	UIContainer::UIContainer()
	{
		m_pControlNode = NULL;
		m_refMode = NO_REFERENCE;
		m_refPointMode = NONE;
		m_widthScale = m_widthOffset = m_heightScale = m_heightOffset = 0.0f;
		m_size = CCSizeZero;
		m_refObj = NULL;
		m_refObjName = "";
	}

	void UIContainer::LoadBinary(NiStream &kStream)
	{
		UIControlBase::LoadBinary(kStream);

		uint32_t refMode = 0;
		uint32_t refPoint = 0;

		uint32_t widthScale = 0;
		uint32_t widthOffset = 0;
		uint32_t heightScale = 0;
		uint32_t heightOffset = 0;

		uint32_t width = 0;
		uint32_t height = 0;

		mutableDic *dic = kStream.GetStreamData();

		kStream.getIntAttributeValue(dic, "refMode", refMode);
		kStream.getIntAttributeValue(dic, "refPointMode", refPoint);
		kStream.getIntAttributeValue(dic, "widthScale", widthScale);
		kStream.getIntAttributeValue(dic, "widthOffset", widthOffset);
		kStream.getIntAttributeValue(dic, "heightScale", heightScale);
		kStream.getIntAttributeValue(dic, "heightOffset", heightOffset);
		kStream.getIntAttributeValue(dic, "width", width);
		kStream.getIntAttributeValue(dic, "height", height);
		kStream.getStringattributeValue(dic, "refObjName", m_refObjName);

		setRefMode((ReferenceMode)refMode);
		setRefPointMode((ReferencePoint)refPoint);
		setWidthScale(float(widthScale) / 10000.0f);
		setWidthOffset(widthOffset / CC_CONTENT_SCALE_FACTOR());
		setHeightScale(float(heightScale) / 10000.0f);
		setHeightOffset(heightOffset / CC_CONTENT_SCALE_FACTOR());
		setSize(width / CC_CONTENT_SCALE_FACTOR(), height / CC_CONTENT_SCALE_FACTOR());
		setWorldPosition(m_ptLocalPosition);

		m_refObj = NULL;
		m_pControlNode = CCNode::create();
		m_pControlNode->setAnchorPoint(CCPointZero);
		m_pControlNode->setPosition(m_ptLocalPosition);
		m_pControlNode->setVisible(m_bIsVisible);
		/** calculate world position in 1024x768
		* this is important to enable Asprite position calculation to get correct local position
		*/
		preCalWorldPos();
	}

	void UIContainer::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UIContainer::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
	}

	void UIContainer::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);
	}

	bool UIContainer::IsEqual(NiObject *pObject)
	{
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
	}
}