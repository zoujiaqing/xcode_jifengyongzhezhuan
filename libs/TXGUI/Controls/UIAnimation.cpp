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
#include "UIAnimation.h"
#include "AspriteManager.h"
#include "CrossPlatform.h"

#include "UIManager.h"
#include "UICreator.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"

namespace TXGUI {
    USING_NS_CC;
    
    NiImplementRTTI(UIAnimation, UIControlBase);
    NiImplementCreateObject(UIAnimation);
    
    
    UIAnimation::UIAnimation(cocos2d::CCNode* parent)  
    :UIControlBase(parent)
    {   
		m_pAnimSprite = NULL;
		m_fInitScale = 1.0f;
    }

	UIAnimation::UIAnimation(std::string name, int tag, float z, UIControlBase *parent, 
		AnimSprite *anim, float initScale)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pAnimSprite = anim;
		m_pControlNode = anim->getBatchNode();
		m_fInitScale = initScale;
		parent->getCurrentNode()->addChild(m_pControlNode, m_fPosZ);
	}
    
    UIAnimation::~UIAnimation()
    {
		if(m_pAnimSprite != NULL)
		{
			EnginePlayerManager::getInstance()->removeAsprite(m_pAnimSprite, true);
			m_pAnimSprite = NULL;
		}
    }
   
    void UIAnimation::setPosition(CCPoint pt) 
    {
        UIControlBase::setPosition(pt);
        
		if(m_pAnimSprite != NULL)
		{
            m_pAnimSprite->getBatchNode()->setPosition(pt);
		}
    }

	void UIAnimation::setScale(float scale)
	{
		UIControlBase::setScale(scale);
		if(m_pAnimSprite != NULL)
		{
			m_pAnimSprite->getBatchNode()->setScale(m_fInitScale * scale);
		}
	}

	void UIAnimation::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);
		if(m_pAnimSprite != NULL)
		{
			m_pAnimSprite->getBatchNode()->setScaleX(m_fInitScale * xScale);
			m_pAnimSprite->getBatchNode()->setScaleY(m_fInitScale * yScale);
		}
	}

	void UIAnimation::setInitScale(float scale)
	{
		m_fInitScale = scale;
	}

	float UIAnimation::getInitScale()
	{
		return m_fInitScale;
	}

	AnimSprite *UIAnimation::getAnimSprite()
	{
		return m_pAnimSprite;
	}

	void UIAnimation::setAnimSprite(AnimSprite *pAnimSprite)
	{
		if(pAnimSprite == NULL)
			return;

		if(m_pAnimSprite != NULL)
		{
			m_pAnimSprite->getBatchNode()->removeFromParentAndCleanup(true);
			EnginePlayerManager::getInstance()->removeAsprite(m_pAnimSprite, true);
			m_pAnimSprite = NULL;
		}

		m_pAnimSprite = pAnimSprite;
		m_pControlNode = m_pAnimSprite->getBatchNode();
		m_pControlNode->setPosition(m_ptLocalPosition);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pControlNode, m_fPosZ);
		}
	}
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UIAnimation::UIAnimation()
    {
		m_pAnimSprite = NULL;
        m_pControlNode = NULL;
		m_fInitScale = 1.0f;
    }
    
    
    void UIAnimation::LoadBinary(NiStream &kStream)
    {
		UIControlBase::LoadBinary(kStream);
        
        mutableDic* dic = kStream.GetStreamData();

		uint32_t loadType = 0;
		kStream.getIntAttributeValue(dic, "loadType", loadType);

		uint32_t useASposition = 0;
		kStream.getIntAttributeValue(dic, "useASposition", useASposition);

		if(useASposition == 1)
		{
			string asName = "";
			kStream.getStringattributeValue(dic, "ASname", asName);
			int index = getResourceIDByName(asName.c_str());

			ASprite *asprite = AspriteManager::getInstance()->getAsprite(KUI_BIN);
			m_ptWorldPosition = asprite->GetFramePointMiddle(index);

			CCPoint parentWorldPosition = CCPointZero;
			if(m_pParent != NULL)
			{
				parentWorldPosition = m_pParent->getWorldPosition();
			}

			// Asprite gives point position but we need pixel position
			m_ptLocalPosition.x = m_ptWorldPosition.x - parentWorldPosition.x;
			m_ptLocalPosition.y = m_ptWorldPosition.y - parentWorldPosition.y;
		}

		if(loadType == 0)
		{
			uint32_t iInitScale = 100;
			uint32_t animIndex;
			uint32_t animType;
			uint32_t animID;
			uint32_t loopTimes = 1;
			uint32_t bLoop = 1;

			kStream.getIntAttributeValue(dic, "initScale", iInitScale);
			kStream.getIntAttributeValue(dic, "animIndex", animIndex);
			kStream.getIntAttributeValue(dic, "animType", animType);
			kStream.getIntAttributeValue(dic, "animID", animID);
			kStream.getIntAttributeValue(dic, "loopTimes", loopTimes);
			kStream.getIntAttributeValue(dic, "bLoop", bLoop);

			setInitScale(float(iInitScale) / 100);
			m_pAnimSprite = EnginePlayerManager::getInstance()->addAnimSprite((int)animIndex, (int)animType, 0, 0);
			m_pAnimSprite->SetAnim((int)animID, (int)loopTimes, (bool)(bLoop != 0));
		}
		else if(loadType == 1)
		{
			uint32_t iInitScale = 100;
			string filePath = "";
			uint32_t animID;
			uint32_t loopTimes = 1;
			uint32_t bLoop = 1;

			kStream.getIntAttributeValue(dic, "initScale", iInitScale);
			kStream.getStringattributeValue(dic, "filePath", filePath);
			kStream.getIntAttributeValue(dic, "animID", animID);
			kStream.getIntAttributeValue(dic, "loopTimes", loopTimes);
			kStream.getIntAttributeValue(dic, "bLoop", bLoop);

			setInitScale(float(iInitScale) / 100);
			m_pAnimSprite = EnginePlayerManager::getInstance()->addAnimSprite(filePath, 0, 0);
			m_pAnimSprite->SetAnim((int)animID, (int)loopTimes, (bool)(bLoop != 0));
		}

		m_pControlNode = m_pAnimSprite->getBatchNode();
		m_pControlNode->setPosition(m_ptLocalPosition);
		m_pControlNode->setVisible(m_bIsVisible);
    }
    
    void UIAnimation::LinkObject(NiStream &kStream)
    {
        UIControlBase::LinkObject(kStream);
    }
    
    bool UIAnimation::RegisterStreamables(NiStream &kStream)
    {
        if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UIAnimation::SaveBinary(NiStream &kStream)
    {
        UIControlBase::SaveBinary(kStream);
        
    }
    
    bool UIAnimation::IsEqual(NiObject *pObject)
    {
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }    
}
