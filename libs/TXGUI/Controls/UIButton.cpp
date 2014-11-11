
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
#include "UIButton.h"
#include "AspriteManager.h"
#include "UIManager.h"

#include "UIManager.h"
#include "UICreator.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"
#include "SkeletonAnimRcsManager.h"
#include "ParticleManager.h"
#include "GameFontManager.h"
#include "GameAudioManager.h"
#include "TutorialsManager.h"
#include "InstanceListLayer.h"
#include "BYGraySprite.h"
#include "EffectSprite.h"
#include "CommonEnum.h"

//extern BYGraySprite* graylightWithCCSprite(CCSprite* oldSprite,bool isLight);

#include "CrossPlatform.h"

namespace TXGUI {
    USING_NS_CC;
    
    NiImplementRTTI(UIButton, UIControlBase);
    NiImplementCreateObject(UIButton);
    
    
    UIButton::UIButton(cocos2d::CCNode* parent)  
    :UIControlBase(parent)
	,m_pAnimationNode(NULL)
    {
		m_strClickedFun = "";
		m_strTappedFun = "";
		m_strTapCancelFun = "";
		m_strDoubleClickedFun = "";
		m_luaTapHandler = 0;
		m_luaTapCancellHandler = 0;
		m_bIsEnabled = true;
		m_bIsTouchEnabled = true;
		m_bSoundEnabled = true;
        m_pBtnMenu = UIMenu::create();
		m_pBtnMenu->setPosition(CCPointZero);		
		m_pBtnMenu->setControlNode(this);
        m_pControlNode = m_pBtnMenu;
        
        // menu button item
        m_pButtonItem = NULL; 
		m_pButtonLabel = NULL;
		m_pBtIcon = NULL;
		m_pTextLabel = NULL;
		m_dragHandler = NULL;
		m_pFrameNode = NULL;		
		m_pAnimTipParentNode = NULL;
		m_fPosZ = 0;
		m_iconOrder = 1;
		m_anmationPosition = CCPointZero;
		m_pButtonPosZ = 1;
		m_priority = -128;
		CCNode *pNode = getCurrentNode();
		this->BindNode(parent);	
		float scale = UIManager::sharedManager()->getScaleFactor();
		setScale(scale);
		
		m_pDarkSprite = NULL;
        m_rotateEffect = NULL;
	}

	UIButton::UIButton(std::string name,
		int tag,
		float z,
		UIControlBase* parent,
		CCMenuItemSprite *sprite,
		CCMenuItemLabel *label,
		int priority
		)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
		,m_pAnimationNode(NULL)
	{
		m_strClickedFun = "";
		m_strTappedFun = "";
		m_strTapCancelFun = "";
		m_strDoubleClickedFun = "";
		m_luaTapHandler = 0;
		m_luaTapCancellHandler = 0;
		m_bIsEnabled = true;
		m_bIsTouchEnabled = true;
		m_bSoundEnabled = true;
		m_pBtnMenu = UIMenu::create();
		m_pBtnMenu->setPosition(CCPointZero);
		m_priority = priority;
		m_pBtnMenu->setInitPriority(priority);
		m_pControlNode = m_pBtnMenu;
		m_pBtnMenu->setControlNode(this);
		
		m_pButtonItem = NULL;
		m_pButtonLabel = NULL;
		m_pBtIcon = NULL;
		m_pAnimationNode = NULL;
		m_pTextLabel = NULL;
		m_dragHandler = NULL;
		m_pFrameNode = NULL;
		m_pAnimTipParentNode = NULL;
		m_anmationPosition = CCPointZero;
		m_pButtonPosZ = 1;
		m_fPosZ = 0;
		m_iconOrder = 1;
		setMenuItemSprite(sprite);
		setMenuItemLabel(label);

		BindNode(parent->getCurrentNode());
		float scale = UIManager::sharedManager()->getScaleFactor();
		setScale(scale);

		m_pDarkSprite = NULL;
        m_rotateEffect = NULL;
	}

	UIButton::UIButton(std::string name,
		int tag,
		float z,
		UIControlBase* parent,
		CCMenuItemSprite *sprite,
		CCMenuItemLabel *label,
		int priority,
		std::string clickedFunc
		)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_strClickedFun = clickedFunc;
		m_strTappedFun = "";
		m_strTapCancelFun = "";
		m_strDoubleClickedFun = "";
		m_luaTapHandler = 0;
		m_luaTapCancellHandler = 0;
		m_bIsEnabled = true;
		m_bIsTouchEnabled = true;
		m_bSoundEnabled = true;
		m_pBtnMenu = UIMenu::create();
		m_pBtnMenu->setPosition(CCPointZero);
		m_priority = priority;
		m_pBtnMenu->setInitPriority(priority);
		m_pControlNode = m_pBtnMenu;
		m_pBtnMenu->setControlNode(this);
		m_pButtonPosZ = 1;
		m_pButtonItem = NULL;
		m_pButtonLabel = NULL;
		m_pBtIcon = NULL;
		m_pAnimationNode= NULL;
		m_pTextLabel = NULL;
		m_dragHandler = NULL;
		m_pFrameNode = NULL;
		m_pAnimTipParentNode = NULL;
		m_anmationPosition = CCPointZero;
		m_fPosZ = 0;
		m_iconOrder = 1;
		setMenuItemSprite(sprite);
		setMenuItemLabel(label);


		BindNode(parent->getCurrentNode());
		
		float scale = UIManager::sharedManager()->getScaleFactor();
		setScale(scale);

		m_pDarkSprite = NULL;
        m_rotateEffect = NULL;
	}

	UIButton::UIButton(
		std::string name,
		int tag,
		float z,
		UIControlBase* parent,
		CCSprite *normalPic,
		CCSprite *clickedPic,
		CCMenuItemLabel *label,
		int priority,
		std::string clickedFunc
		)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_strClickedFun = clickedFunc;
		m_strTappedFun = "";
		m_strTapCancelFun = "";
		m_strDoubleClickedFun = "";
		m_luaTapHandler = 0;
		m_luaTapCancellHandler = 0;
		m_bIsEnabled = true;
		m_bIsTouchEnabled = true;
		m_bSoundEnabled = true;
		m_pBtnMenu = UIMenu::create();
		m_pBtnMenu->setPosition(CCPointZero);
		m_priority = priority;
		m_pBtnMenu->setInitPriority(priority);
		m_pControlNode = m_pBtnMenu;
		m_pBtnMenu->setControlNode(this);

		m_pButtonItem = NULL;
		m_pButtonLabel = NULL;
		m_pBtIcon = NULL;
		m_pAnimationNode = NULL;
		m_pTextLabel = NULL;
		m_dragHandler = NULL;
		m_pFrameNode = NULL;
		m_pAnimTipParentNode = NULL;
		m_anmationPosition = CCPointZero;
		m_pButtonPosZ = 0;
		m_fPosZ = 0;
		m_iconOrder = 1;

		CCMenuItemSprite* button = CCMenuItemSprite::create(normalPic, 
			clickedPic, 
			this,                                                                          
			menu_selector(UIButton::Clicked)
			);
		button->setTag(tag);
		setMenuItemSprite(button);
		setMenuItemLabel(label);

		BindNode(parent->getCurrentNode());
		float scale = UIManager::sharedManager()->getScaleFactor();
		setScale(scale);

		m_pDarkSprite = NULL;
        m_rotateEffect = NULL;
	}
    
    UIButton::~UIButton()
    {
		if(m_pAnimationNode)
		{
			m_pAnimationNode->stopAllActions();
			m_pAnimationNode->removeFromParentAndCleanup(true);
			m_pAnimationNode = NULL;
		}
		if(m_dragHandler)
		{
			DragReceiverRegister::sharedManager()->unregisterDragReceiver(m_dragHandler);
			delete m_dragHandler;
			m_dragHandler = NULL;
		}
		m_pControlNode = NULL;
		m_pAnimTipParentNode = NULL;
		unregisterTapHandler();
		unregisterTapCancellHandler();

        if (m_rotateEffect != NULL)
        {
            m_rotateEffect->stopAllActions();
            m_rotateEffect->removeFromParentAndCleanup(true);
            setRotateEffect(NULL);
        }
    }
   
    void UIButton::setPosition(CCPoint pt) 
    {
        UIControlBase::setPosition(pt);
        
		if(m_pAnimationNode)
		{
			m_pAnimationNode->setPosition(pt);
		}
    }

	void UIButton::setScale(float scale)
	{
		UIControlBase::setScale(scale);
		if(m_pAnimationNode != NULL)
		{
			m_pAnimationNode->setScale(scale);
		}
	}

	void UIButton::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);
		if(m_pButtonItem != NULL && m_pButtonItem->getParent() == m_pBtnMenu)
		{
			m_pButtonItem->setScaleX(xScale);
			m_pButtonItem->setScaleY(yScale);
		}

		if(m_pButtonLabel != NULL)
		{
			m_pButtonLabel->setScaleX(xScale);
			m_pButtonLabel->setScaleY(yScale);
		}

		if(m_pBtIcon != NULL)
		{
			m_pBtIcon->setScaleX(xScale);
			m_pBtIcon->setScaleY(yScale);
		}

		if(m_pTextLabel != NULL)
		{
			m_pTextLabel->setScaleX(xScale);
			m_pTextLabel->setScaleY(yScale);
		}
	}

	CCMenuItemSprite *UIButton::getMenuItemSprite()
	{
		return m_pButtonItem;
	}

	MenuItemExpand* UIButton::getMenuItemSpriteExpand()
	{
		if (m_pButtonItem)
		{
			return dynamic_cast<MenuItemExpand*>(m_pButtonItem);
		}
		return NULL;
	}

	void UIButton::setMenuItemSprite(CCMenuItemSprite *pSprite)
	{
		if(pSprite == NULL)
			return;

		if(m_pButtonItem != NULL)
		{
			m_pButtonItem->removeFromParentAndCleanup(true);
			m_pButtonItem = NULL;
		}

		//if(m_strClickedFun != "")
		{
			pSprite->setTarget(this, menu_selector(UIButton::Clicked));
		}

		pSprite->setTag(m_iTag);
		m_pButtonItem = pSprite;
		m_pButtonItem->setPosition(m_ptLocalPosition);
		m_pBtnMenu->addChild(pSprite, 0);
	}

	CCMenuItemLabel *UIButton::getMenuItemLabel()
	{
		return m_pButtonLabel;
	}

	void UIButton::setMenuItemLabel(CCMenuItemLabel *pLabel)
	{
		if(pLabel == NULL)
			return;

		if(m_pButtonLabel != NULL)
		{
			m_pButtonLabel->removeFromParentAndCleanup(true);
			m_pButtonLabel = NULL;
		}

		if(m_strClickedFun != "")
		{
			pLabel->setTarget(this, menu_selector(UIButton::Clicked));
		}
        
		pLabel->setTag(m_iTag);
		m_pButtonLabel = pLabel;
		m_pButtonLabel->setPosition(m_ptLocalPosition);
		m_pBtnMenu->addChild(m_pButtonLabel, 1);
	}

	UILabelTTF* UIButton::getTextLabel()
	{
		return m_pTextLabel;
	}

	void UIButton::setTextLabel(const char * text,float fontSize,ccColor3B color, const char* font,CCPoint pt,CCPoint anchorPt/* = ccp(0.5f,0.5f)*/)
	{
		if(m_pTextLabel == NULL)
		{
			//CCSize nameLabelSize = CCSizeMake(150 / CC_CONTENT_SCALE_FACTOR(), fontSize * 1.5);
			m_pTextLabel = UILabelTTF::create(text, font, fontSize);
			m_pTextLabel->setPosition(pt);
			if(m_pAnimationNode)
			{
				m_pAnimationNode->addChild(m_pTextLabel,m_ExtraSpriteOrder+1);
			}
		}

		m_pTextLabel->setString(text);
		m_pTextLabel->setColor(color);
		m_pTextLabel->setAnchorPoint(anchorPt);
		m_pTextLabel->setPosition(pt);
	}

	void UIButton::setClickedFunc(std::string func)
	{
		m_strClickedFun = func;

		if(func != "")
		{
			if(m_pButtonItem != NULL)
			{
				m_pButtonItem->setTarget(this, menu_selector(UIButton::Clicked));
			}		

			if(m_pButtonLabel != NULL)
			{
				m_pButtonLabel->setTarget(this, menu_selector(UIButton::Clicked));
			}
		}
		else
		{
			if(m_pButtonItem != NULL)
			{
				m_pButtonItem->setTarget(NULL, NULL);
			}		

			if(m_pButtonLabel != NULL)
			{
				m_pButtonLabel->setTarget(NULL, NULL);
			}
		}
	}
    
    void UIButton::Clicked(CCObject* pObject)
    {
		// 转发新手教学消息
		if (false == TutorialsManager::Get()->HandleOneEvent(this->m_strName.c_str(),1))
		{
			return ;
		}

		if(m_strClickedFun != "")
        {
			dispatchMenuEvent(m_strClickedFun, pObject);
			if(m_bSoundEnabled)
			{
				GameAudioManager::sharedManager()->playEffect(SOUND_EFFECT_CLICK,false);
			}
		}
		else
		{
			executeLuaScript();
		}
    }
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UIButton::UIButton()
    {
        m_pBtnMenu = UIMenu::create();
		m_pBtnMenu->setControlNode(this);
		m_pBtIcon = NULL;
        m_pButtonItem = NULL;
		m_pButtonLabel = NULL;
		m_pAnimationNode = NULL;
		m_pTextLabel = NULL;
		m_dragHandler = NULL;
		m_pFrameNode = NULL;
		m_pAnimTipParentNode = NULL;
        m_pControlNode = m_pBtnMenu;
		m_strClickedFun = "";
		m_strTappedFun = "";
		m_strTapCancelFun = "";
		m_strDoubleClickedFun = "";
		m_luaTapHandler = 0;
		m_luaTapCancellHandler = 0;
		m_bIsEnabled = true;
		m_bIsTouchEnabled = true;
		m_bSoundEnabled = true;
		m_iconOrder = 0;
		m_ExtraSpriteOrder = 0;
		m_anmationPosition = CCPointZero;
		m_fPosZ = 0;
		m_pButtonPosZ = 1;
		m_priority = -128;

		m_pDarkSprite = NULL;
        m_rotateEffect = NULL;
    }
    
    void UIButton::LoadBinary(NiStream &kStream)
    {
		UIControlBase::LoadBinary(kStream);
        m_pButtonPosZ = 1;
		m_iconOrder = 2;

		mutableDic* dic = kStream.GetStreamData();

		m_pBtnMenu->setPosition(CCPointZero);

		uint32_t enabled = 1;
		uint32_t touchEnabled = 1;
		uint32_t soundEnabled = 1;
		kStream.getIntAttributeValue(dic, "isEnabled", enabled);
		kStream.getIntAttributeValue(dic, "touchEnabled", touchEnabled);
		kStream.getIntAttributeValue(dic, "soundEnabled", soundEnabled);

		m_bIsEnabled = (enabled >= 1);
		m_bIsTouchEnabled = (touchEnabled >= 1);
		m_bSoundEnabled = (soundEnabled >= 1);

		m_strDoubleClickedFun = "";
		m_strTapCancelFun = "";
		m_strTappedFun = "";
		// get button click callback function
		kStream.getStringattributeValue(dic, "clickedCallback", m_strClickedFun);
		mutableDic* dragDic = static_cast<mutableDic*>(dic->objectForKey("drag"));
		if(dragDic)
		{
			if(m_pBtnMenu)
			{
				unsigned int type = 0;
				m_buttonIndex = 0;
				m_priority = - 128;
				
				unsigned int selectTime = 0; // button响应时间 0 表示 翻译时触发, 1点下时触发
				kStream.getSignedIntAttributeValue(dragDic, "priority", m_priority);
				kStream.getIntAttributeValue(dragDic, "itemType", type);
				kStream.getIntAttributeValue(dragDic, "itemIndex", m_buttonIndex);
				kStream.getIntAttributeValue(dragDic, "selectTime", selectTime);
				kStream.getStringattributeValue(dragDic, "doubleClickedCallback", m_strDoubleClickedFun);
				kStream.getStringattributeValue(dragDic, "tappedCallback", m_strTappedFun);
				kStream.getStringattributeValue(dragDic, "tapCancelCallback", m_strTapCancelFun);
				if(selectTime == 1)
				{
					m_pBtnMenu->setTouchActivityTime(true);
				}

				m_pBtnMenu->setInitPriority(m_priority);
				if(type != 0)
				{
					creatDragReceiver((DragHandlerType)type);
					//m_pBtnMenu->SetDoubleClickHandler(this,menu_selector(UIButton::OnDoubleClicked)); // 取消了双击操作
				}
				if(m_strTappedFun != "" && m_strTapCancelFun != "")
				{
					//m_pBtnMenu->SetTappedHandler(this,menu_selector(UIButton::OnTapped),menu_selector(UIButton::OnTapCancel)); // 取消了长按操作
				}
			}
		}

		// text on the button
		mutableDic* textDic =  static_cast<mutableDic*>(dic->objectForKey("text"));
		
		if(textDic)
		{
			std::string strTemp = GetContent(textDic, kStream);			
			const char* content = strTemp.c_str();
					
			uint32_t color[3] = {255, 255, 255};			
			GetColor(textDic, kStream, color[0], color[1], color[2]);

			string font = KJLinXin;
			kStream.getStringattributeValue(textDic, "font", font);

            float largeFontSize = GameFontManager::largeFontSize();
			uint32_t fontSize = 1.5f *largeFontSize * CC_CONTENT_SCALE_FACTOR();
			kStream.getIntAttributeValue(textDic, "fontSize", fontSize);
			fontSize /= CCDirector::sharedDirector()->getContentScaleFactor();

			CCLabelTTF* takeByLabel = CCLabelTTF::create(content, font.c_str(), (float)fontSize);
			takeByLabel->setColor(ccc3(color[0], color[1], color[2]));

			CCMenuItemLabel *takeByItem = CCMenuItemLabel::create(
				takeByLabel, 
				this, 
				menu_selector(UIButton::Clicked)
				);
			takeByItem->setTag(m_iTag);

			m_pBtnMenu->addChild(takeByItem, 1);
			m_pButtonLabel = takeByItem;
			m_pButtonLabel->setPosition(m_ptLocalPosition);

			uint32_t textOnly = 0;
			kStream.getIntAttributeValue(textDic, "textOnly", textOnly);
			if(textOnly)
			{
				takeByItem->setEnabled(m_bIsEnabled);
				//takeByItem->setIsTouchEnabled(m_bIsTouchEnabled);
				return;
			}
			else
			{
				takeByItem->setDisabledColor(ccc3(color[0], color[1], color[2]));
				takeByItem->setEnabled(false);
			}
		}

        mutableDic* textureDic =  static_cast<mutableDic*>(dic->objectForKey("texture"));
		// texture attributes of button    
		string normalTex = "", clickedTex = "", disableTex = "", iconTex = "";
		string textureBinName = "";
		bool hasDisableTex = false;
		uint32_t directTextureFrame = 0;
		string directTextureFrameFile = "";
		uint32_t isGray = 0;

		/// ASprite Manager images configuration if existed
		vector<string> images;

		if(textureDic)
		{
			kStream.getStringattributeValue(textureDic, "normalTexture", normalTex);
			kStream.getStringattributeValue(textureDic, "clickedTexture", clickedTex);
			kStream.getStringattributeValue(textureDic, "iconTexture", iconTex);
			kStream.getStringattributeValue(textureDic, "disableTexture", disableTex);
			kStream.getIntAttributeValue(textureDic, "directFrame", directTextureFrame);
			kStream.getIntAttributeValue(textureDic, "isGray", isGray);
			kStream.getSignedIntAttributeValue(textureDic, "iconOrder", m_iconOrder);
			if(directTextureFrame != 0)
			{
				kStream.getStringattributeValue(textureDic, "frameFile", directTextureFrameFile);
			}

			hasDisableTex = (disableTex != "");

			/// ASprite Manager Parameters			
			mutableDic* binarydic = static_cast<mutableDic*>(textureDic->objectForKey("AspriteManager"));
			if(binarydic)
			{
				if(IsIpad())
				{
					kStream.getStringattributeValue(binarydic, "ipad", textureBinName);
				}
				else
				{
					kStream.getStringattributeValue(binarydic, "iphone", textureBinName);
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
		}

		// other texture on the button
		mutableDic* otherTextureDic = static_cast<mutableDic*>(dic->objectForKey("textSprite"));

		// text texture of button
		string otherTextureName = "", otherTextureBinName = "";
		uint32_t directOtherTextureFrame = 0;
		string directOtherTextureFrameFile = "";

		if(otherTextureDic)
		{
			kStream.getIntAttributeValue(otherTextureDic, "directFrame", directOtherTextureFrame);
			if(directOtherTextureFrame == 1)
			{
				kStream.getStringattributeValue(otherTextureDic, "frameFile", directOtherTextureFrameFile);
			}
			kStream.getStringattributeValue(otherTextureDic, "textSpriteName", otherTextureName);
			mutableDic* binarydic = static_cast<mutableDic*>(otherTextureDic->objectForKey("AspriteManager"));
			if(binarydic)
			{
				if(IsIpad())
				{
					kStream.getStringattributeValue(binarydic, "ipad", otherTextureBinName);
				}
				else
				{
					kStream.getStringattributeValue(binarydic, "iphone", otherTextureBinName);
				}
			}
		}
        
        CCPoint pt, ptButton;
        ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);       

        CCSprite * clickedSprite = NULL;       
        CCSprite * normalSprite = NULL;
		CCSprite * disableSprite = NULL;
		CCSprite * iconSprite = NULL;

		CCSprite * otherTexture = NULL;
		CCSprite * otherTextureDump = NULL;
		CCSprite * otherTextureDisable = NULL;


		if(textureBinName != "")
		{
			as = AspriteManager::getInstance()->getAsprite(textureBinName);
			/// set up as sprite images
			for(size_t i = 0; i < images.size(); i++)
			{
				if (as)
				{
					as->setImageName(i, images[i]);
				}
			}
		}

		if(directTextureFrame == 1)
		{
			if(directTextureFrameFile != "")
			{
				string frameFileFullPath = GameResourceManager::sharedManager()
					->storedFullPathFromRelativePath(directTextureFrameFile.c_str());
				GameResourceManager::sharedManager()
					->addSpriteFrameWithDictionaryFromCache(directTextureFrameFile.c_str());
			}

			clickedSprite = CCSprite::createWithSpriteFrameName(clickedTex.c_str()); 
			normalSprite = CCSprite::createWithSpriteFrameName(normalTex.c_str());
			if(hasDisableTex)
			{
				disableSprite = CCSprite::createWithSpriteFrameName(disableTex.c_str());
			}
		}
		else
		{
			clickedSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(clickedTex.c_str()), 0, pt);
			clickedSprite = MergeSmallASprite(clickedSprite);
			normalSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(normalTex.c_str()), 0, pt);
			normalSprite = MergeSmallASprite(normalSprite);

			ptButton = pt;
			if(hasDisableTex)
			{
				disableSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(disableTex.c_str()), 0, pt);
				disableSprite = MergeSmallASprite(disableSprite);
				
			}
		}

		if(otherTextureName != "")
		{
			if(directOtherTextureFrame == 1)
			{
				if(directOtherTextureFrameFile != "")
				{
					string frameFileFullPath = GameResourceManager::sharedManager()
						->storedFullPathFromRelativePath(directOtherTextureFrameFile.c_str());
					GameResourceManager::sharedManager()
						->addSpriteFrameWithDictionaryFromCache(frameFileFullPath.c_str());
				}
				
				otherTexture = CCSprite::createWithSpriteFrameName(otherTextureName.c_str());
				otherTextureDump = CCSprite::createWithSpriteFrameName(otherTextureName.c_str());
				if(hasDisableTex)
				{
					otherTextureDisable = CCSprite::createWithSpriteFrameName(otherTextureName.c_str());
				}
			}
			else
			{
				as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
				if(otherTextureBinName != "")
				{
					as = AspriteManager::getInstance()->getAsprite(otherTextureBinName);
				}
				otherTexture = as->getSpriteFromFrame_Middle(getResourceIDByName(otherTextureName.c_str()), 0, pt);
				otherTexture = MergeSmallASprite(otherTexture);
				otherTextureDump = as->getSpriteFromFrame_Middle(getResourceIDByName(otherTextureName.c_str()), 0, pt);
				otherTextureDump = MergeSmallASprite(otherTextureDump);
				if(hasDisableTex)
				{
					otherTextureDisable = 
						as->getSpriteFromFrame_Middle(getResourceIDByName(otherTextureName.c_str()), 0, pt);
					otherTextureDisable = MergeSmallASprite(otherTextureDisable);
				}
			}
		}

        // if button sprite
        if (clickedSprite != NULL && normalSprite != NULL)
        {
			if(otherTexture != NULL && otherTextureDump != NULL)
			{
				otherTexture->setPosition(ccp(normalSprite->getContentSize().width / 2, 
					normalSprite->getContentSize().height / 2));
				normalSprite->addChild(otherTexture);
				otherTextureDump->setPosition(ccp(clickedSprite->getContentSize().width / 2, 
					clickedSprite->getContentSize().height / 2));
				clickedSprite->addChild(otherTextureDump);
				if(hasDisableTex && otherTextureDisable != NULL)
				{
					otherTextureDisable->setPosition(ccp(disableSprite->getContentSize().width / 2, 
						disableSprite->getContentSize().height / 2));
					disableSprite->addChild(otherTextureDisable);
				}
			}

            CCMenuItemSprite* button;
			if(hasDisableTex)
			{
				button = CCMenuItemSprite::create(normalSprite, 
					clickedSprite,
					disableSprite,
					this,                                                                          
					menu_selector(UIButton::Clicked));
			}
			else
			{
				button = CCMenuItemSprite::create(normalSprite, 
					clickedSprite, 
					this,                                                                          
					menu_selector(UIButton::Clicked));
			}
			button->setTag(m_iTag);
			button->setEnabled(m_bIsEnabled);
			//button->setTouchEnabled(m_bIsTouchEnabled);

			// calculate button position from ASspriteManager
			uint32_t useASposition = 0;
			if(kStream.getIntAttributeValue(dic, "useASposition", useASposition)
				&& useASposition == 1
				&& directTextureFrame == 0)
			{
				/** if we use AS_bin file's frame position
				* local position should reset from parent's world position
				*/
				CCPoint parentWorldPosition = CCPointZero;
				if(m_pParent != NULL)
				{
					parentWorldPosition = m_pParent->getWorldPosition();
				}
                
				m_ptLocalPosition.x = ptButton.x - parentWorldPosition.x;
				m_ptLocalPosition.y = ptButton.y - parentWorldPosition.y;
			}
			else
			{
				// we modified it, so reset it
				UIControlBase::CalWorldPos();
			}

			m_pBtnMenu->addChild(button,0);
			m_pButtonItem = button;
			m_pButtonItem->setPosition(CCPointZero);
            
			m_anmationPosition = m_ptLocalPosition;
			if(iconTex != "")
			{
				string ASfile = KICON_BIN;
				kStream.getStringattributeValue(textureDic, "iconBinFile", ASfile);
				ASprite * as = AspriteManager::getInstance()->getAsprite(ASfile);
				if(as)
				{
					iconSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(iconTex.c_str()), 0, pt);
					if(iconSprite)
					{
						m_pBtIcon = iconSprite;
					}
				}
			}
			m_pControlNode->setVisible(m_bIsVisible);
        }

		// text on the button
		mutableDic* lableDic =  static_cast<mutableDic*>(dic->objectForKey("lable"));

		if(lableDic)
		{			
			std::string strTemp = GetContent(lableDic, kStream);			
			const char* content = strTemp.c_str();			
			uint32_t color[3] = {255, 255, 255};			
			GetColor(lableDic, kStream, color[0], color[1], color[2]);

			string font = KJLinXin;
			kStream.getStringattributeValue(lableDic, "font", font);

			float largeFontSize = GameFontManager::largeFontSize();
			uint32_t fontSize = largeFontSize * CC_CONTENT_SCALE_FACTOR();
			kStream.getIntAttributeValue(lableDic, "fontSize", fontSize);
			fontSize /= CCDirector::sharedDirector()->getContentScaleFactor();
			int m_lableOffsetX = 0;
			int m_lableOffsetY = 0;
			kStream.getSignedIntAttributeValue(lableDic, "offsetX", m_lableOffsetX);
			kStream.getSignedIntAttributeValue(lableDic, "offsetY", m_lableOffsetY);
			if(normalSprite)
			{
				CCSize size = normalSprite->getContentSize();
				m_pTextLabel = UILabelTTF::create(content, font.c_str(), (float)fontSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
			}
			else
			{
				m_pTextLabel = UILabelTTF::create(content, font.c_str(), (float)fontSize);
			}
			
			m_pTextLabel->setColor(ccc3(color[0], color[1], color[2]));
			m_pTextLabel->setPosition(ccp(m_lableOffsetX,m_lableOffsetY));
		}
    }
    
	CCPoint UIButton::getWorldPosition()
	{
		if(m_pAnimationNode)
		{
			if(m_pAnimationNode->getParent())
			{
				return m_pAnimationNode->getParent()->convertToWorldSpace(m_pAnimationNode->getPosition());
			}
			else
			{
				return m_pAnimationNode->getPosition();
			}
		}
		return CCPointZero;
	}
	
    void UIButton::LinkObject(NiStream &kStream)
    {
        UIControlBase::LinkObject(kStream);
    }
    
    bool UIButton::RegisterStreamables(NiStream &kStream)
    {
        if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UIButton::SaveBinary(NiStream &kStream)
    {
        UIControlBase::SaveBinary(kStream);
        
    }
    
    bool UIButton::IsEqual(NiObject *pObject)
    {
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }    

	bool UIButton::isSelected()
	{
		if (0 == m_pButtonItem)
		{
			return false;
		}

		return m_pButtonItem->isSelected();	
	}

	void UIButton::setTouchEnabled(bool value)
	{
		UIControlBase::setTouchEnabled(value);
		if(m_pBtnMenu)
		{
			m_pBtnMenu->resetState();
		}
	}

	void UIButton::setMenuInitPriority(int priority)
	{
		if(m_pBtnMenu)
		{
			m_priority = priority;
			m_pBtnMenu->setInitPriority(priority);
		}
	}

	void UIButton::setHandlerPriority(int newPriority)
	{
		if(m_pBtnMenu)
		{
			m_priority = newPriority;
			m_pBtnMenu->setHandlerPriority(newPriority);
		}
	}

	int UIButton::getHandlerPriority()
	{
		return m_priority;
	}

	void UIButton::BindNode(CCNode* pParentNode)
	{
		m_pBindNode = pParentNode;
		m_pParentNode = pParentNode;

		if(!m_pAnimationNode)
		{
			m_pAnimationNode = CCNode::create();
			m_pAnimationNode->setPosition(m_ptLocalPosition);
			m_pAnimationNode->setAnchorPoint(ccp(0.5,0.5));
			m_pAnimationNode->setVisible(m_bIsVisible);
			pParentNode->addChild(m_pAnimationNode,m_fPosZ);
		}

		CCNode *pNode = getCurrentNode();
		m_pAnimationNode->addChild(pNode, m_pButtonPosZ);
		pNode->setPosition(CCPointZero);
		if(m_pBtIcon)
		{
			m_pAnimationNode->addChild(m_pBtIcon,m_iconOrder);
			m_pBtIcon->setPosition(CCPointZero);
		}

		m_ExtraSpriteOrder = m_pButtonPosZ > m_iconOrder ? (m_pButtonPosZ +1) : (m_iconOrder +1);
		if(m_pTextLabel)
		{
			m_pAnimationNode->addChild(m_pTextLabel,m_ExtraSpriteOrder+1);
		}

		std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
		for(; it != m_ChildrenControls.end(); it++)
		{
			(*it)->BindNode(pNode);
		}

		// for test
		m_pBtnMenu->setControlNode(this);
	}

	void UIButton::setEnabled(bool value)
	{
		if (m_pBtIcon)
		{ 
			if (m_pDarkSprite == NULL && false == value)
			{
				m_pDarkSprite = graylightWithCCSprite(m_pBtIcon,false);
				m_pAnimationNode->addChild(m_pDarkSprite,m_iconOrder);				
			}

			if (m_pDarkSprite)
			{
				m_pDarkSprite->setVisible(!value);
			}
			m_pBtIcon->setVisible(value);
		}		

		if (false == value)
		{
			CCNode* pDisableNode = m_pButtonItem->getDisabledImage();
			if (pDisableNode == NULL)
			{
				CCSprite* pNormalNode = dynamic_cast<CCSprite*> (m_pButtonItem->getNormalImage());
				if (pNormalNode)
				{
					CCSprite* pDisableSprite = graylightWithCCSprite(pNormalNode,false);
					m_pButtonItem->setDisabledImage((CCNode*)pDisableSprite);
				}
			}
		}
		m_pButtonItem->setEnabled(value);
	}

	bool UIButton::isEnabled()
	{
		if(m_pButtonItem)
		{
			return m_pButtonItem->isEnabled();
		}
		return false;
	}

	void UIButton::setIconSprite(CCSprite* sprite)
	{
		if(m_pBtIcon)
		{
			m_pBtIcon->removeFromParentAndCleanup(true);
			m_pBtIcon = NULL;
		}
		m_pBtIcon = sprite;

		if(m_pBtIcon)
		{
			if(m_pAnimationNode)
			{	
				m_pAnimationNode->addChild(m_pBtIcon,m_iconOrder);
				m_pBtIcon->setPosition(CCPointZero);
			}
		}
		if(m_dragHandler)
		{
			m_dragHandler->setDragItem(m_pBtIcon);
		}
		if(m_pBtnMenu)
		{
			m_pBtnMenu->SetNormalIcon(m_pBtIcon);
		}
					
	}

	void UIButton::SetDoubleClickHandler(CCObject* listerner,SEL_MenuHandler handler)
	{
		m_pBtnMenu->SetDoubleClickHandler(listerner,handler);
	}

	void UIButton::SetTappedHandler(CCObject* listerner,SEL_MenuHandler tapHandler,SEL_MenuHandler tapCancelHandler)
	{
		m_pBtnMenu->SetTappedHandler(listerner,tapHandler,tapCancelHandler);
	}

	void UIButton::creatDragReceiver(DragHandlerType type)
	{
		if(m_dragHandler)
		{
			return;
		}

		m_dragHandler = new DragReceiver(kCCScrollMenuPriority,type,m_buttonIndex);
		m_dragHandler->setCheckDragFunc(checkDrag_selector(UIButton::isReleaseInButton),this);
		DragReceiverRegister::sharedManager()->registerDragReceiver(m_dragHandler);
	}

	bool UIButton::isReleaseInButton(CCTouch* touch)
	{
		bool claim = m_pBtnMenu->checkReleaseTouchInButton(touch);
		if(claim)
		{
			return true;
		}
		return false;
	}

	void UIButton::setDragMove(bool value)
	{
		m_pBtnMenu->SetDragMove(value,m_dragHandler);
	}

	void UIButton::registerTapHandler(int nHandler)
	{
		unregisterTapHandler();
		m_pBtnMenu->SetTappedHandler(this,menu_selector(UIButton::OnTapped),menu_selector(UIButton::OnTapCancel));
		m_luaTapHandler = nHandler;
	}

	void UIButton::unregisterTapHandler(void)
	{
		if (m_luaTapHandler)
		{
			CCLuaEngine::defaultEngine()->removeScriptHandler(m_luaTapHandler);
			m_luaTapHandler = 0;
		}
	}

	void UIButton::registerTapCancellHandler(int nHandler)
	{
		unregisterTapCancellHandler();
		m_pBtnMenu->SetTappedHandler(this,menu_selector(UIButton::OnTapped),menu_selector(UIButton::OnTapCancel));
		m_luaTapCancellHandler = nHandler;
	}

	void UIButton::unregisterTapCancellHandler(void)
	{
		if (m_luaTapCancellHandler)
		{
			CCLuaEngine::defaultEngine()->removeScriptHandler(m_luaTapCancellHandler);
			m_luaTapCancellHandler = 0;
		}
	}

	void UIButton::OnTapped(CCObject *pObject)
	{		
		if (false == TutorialsManager::Get()->HandleOneEvent(this->m_strName.c_str(),3))
		{
			return ;
		}

		if(m_strTappedFun != "")
		{
			dispatchMenuEvent(m_strTappedFun, pObject);
		}
		else
		{
			if(m_luaTapHandler != 0)
			{
				CCLuaEngine* engine = CCLuaEngine::defaultEngine();
				engine->cleanStack();
				engine->pushInt(this->getTag());
				engine->executeFunctionByHandler(m_luaTapHandler,1);
			}
		}
	}

	void UIButton::OnTapCancel(CCObject *pObject)
	{
		if(m_strTapCancelFun != "")
		{
			dispatchMenuEvent(m_strTapCancelFun, pObject);
		}
		else
		{
			if(m_luaTapCancellHandler != 0)
			{
				CCLuaEngine* engine = CCLuaEngine::defaultEngine();
				engine->cleanStack();
				engine->pushInt(this->getTag());
				engine->executeFunctionByHandler(m_luaTapCancellHandler,1);
			}
		}
	}

	void UIButton::OnDoubleClicked(CCObject *pObject)
	{
		// 转发新手教学消息
		if (false == TutorialsManager::Get()->HandleOneEvent(this->m_strName.c_str(),2))
		{
			return ;
		}

		if(m_strDoubleClickedFun != "")
		{
			dispatchMenuEvent(m_strDoubleClickedFun, pObject);
		}
		else
		{
			executeLuaScript();
		}
	}

	void UIButton::setFrameSprite(CCNode* node,CCPoint pt)
	{
		if(m_pFrameNode)
		{
			m_pFrameNode->removeFromParentAndCleanup(true);
			m_pFrameNode = NULL;
		}

		if(node)
		{
			if(m_pAnimationNode)
			{
				m_pFrameNode = node;
				m_pAnimationNode->addChild(m_pFrameNode,m_ExtraSpriteOrder);
				m_pFrameNode->setPosition(pt);
			}
		}
	}

	CCSize UIButton::getButtonSize()
	{
		if(m_pButtonItem && m_pButtonItem->getNormalImage())
		{
			CCSize size = m_pButtonItem->getNormalImage()->getContentSize();
			size.width /= CC_CONTENT_SCALE_FACTOR();
			size.height /= CC_CONTENT_SCALE_FACTOR();
			return size;
		}
		return CCSizeZero;
	}


	void UIButton::setVisible(bool visible)
	{
		if (m_pControlNode)
		{
			m_pControlNode->setVisible(visible);
		}

		if(m_pAnimationNode)
		{	
			m_pAnimationNode->setVisible(visible);
		}
	}

    bool UIButton::isVisible()
    {
        if (m_pControlNode)
        {
            return m_pControlNode->isVisible();
        }

        if(m_pAnimationNode)
        {	
            return m_pAnimationNode->isVisible();
        }

        return false;
    }

	void UIButton::setTag(int iTag)
	{
		UIControlBase::setTag(iTag);
		if (m_pBtnMenu && m_pButtonItem)
		{
			m_pBtnMenu->setTag(iTag);
			m_pButtonItem->setTag(iTag);
		}
	}

	void UIButton::resetDragHandlerPosition(int pos)
	{
		if(m_dragHandler != NULL)
		{
			m_dragHandler->setHandlerPosition(pos);
		}
	}

	void UIButton::showTipIcon()
	{
		if (NULL != m_pAnimTipParentNode)
		{
			//m_pAnimTipParentNode->removeFromParentAndCleanup(true);
			m_pAnimTipParentNode->setVisible(true);
			return;
		}
		m_pAnimTipParentNode = CCNode::create();
		this->getAnimationNode()->addChild(m_pAnimTipParentNode,10000);	

		CCSprite* iconSprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY");
		m_pAnimTipParentNode->addChild(iconSprite,1);

		float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
		CCSize iconSize = iconSprite->getContentSize();		
		CCSize MenuSize = this->getButtonSize();
		CCPoint relativePt = ccp((MenuSize.width/2 - iconSize.width/2),iconSize.height/2);
		m_pAnimTipParentNode->setPosition(relativePt);
		
		CCActionInterval* action = NULL;
		action = CCMoveBy::create(0.4,ccp(0,10));

		CCActionInterval* action_back = action->reverse();
		CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));

		CCRepeatForever* pAction = CCRepeatForever::create(seq);
		m_pAnimTipParentNode->runAction( pAction );
	}

	void UIButton::hideTipIcon()
	{
		if (NULL != m_pAnimTipParentNode)
		{
			m_pAnimTipParentNode->removeFromParentAndCleanup(true);
		}
		m_pAnimTipParentNode = NULL;
	}

    void UIButton::ShowRotateEffect()
    {
        if (m_rotateEffect != NULL)
        {
            m_rotateEffect->setVisible(true);
        } else
        {
            SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(444);
            EffectSprite* effect = ParticleManager::Get()->createEffectSprite(444,"",false);
            if (effect)
            {
                this->getAnimationNode()->addChild(effect, 1);
                effect->setPosition(ccp(0, 0));
                effect->SetAnim(kType_Play, 1, true);
                setRotateEffect(effect);
            }
        }
    }

    void UIButton::HideRotateEffect()
    {
        if (m_rotateEffect != NULL)
        {
            m_rotateEffect->setVisible(false);
        }
    }

	void UIButton::setMenuItemDispalyState(int state)
	{
		if (m_pButtonItem)
		{
			MenuItemExpand *menuItemExpand = dynamic_cast<MenuItemExpand*>(m_pButtonItem);
			if (menuItemExpand)
			{
				menuItemExpand->setMenuItemDispalyState(state);
			}
		}		
	}
}
