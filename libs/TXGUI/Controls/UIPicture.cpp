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
#include "UIPicture.h"

#include "NiObject.h"
#include "AspriteManager.h"
#include "CrossPlatform.h"
#include "CCLuaEngine.h"

#include "tolua++.h"
#include "GameResourceManager.h"
//#include "CCScale9Sprite.h"
#include "UIManager.h"

namespace TXGUI{
    USING_NS_CC;
    
    NiImplementRTTI(UIPicture, UIControlBase);
    NiImplementCreateObject(UIPicture);
    
    UIPicture::UIPicture(CCSprite* pSprite, cocos2d::CCNode* pParentNode)  
    : UIControlBase(pParentNode)
    {
        m_pSpriteNode = MergeSmallASprite(pSprite);

        m_pControlNode = m_pSpriteNode;
		m_bUseStretch = false;
		m_fInitScale = 1.0f;
        
        CCNode *pNode = getCurrentNode();
        pParentNode->addChild(pNode);
    }

	UIPicture::UIPicture(std::string name, int tag, float z, UIControlBase *parent, CCSprite *pSprite)
		:UIControlBase(name, tag, z, parent,CCSizeZero)
	{
		m_pSpriteNode = MergeSmallASprite(pSprite);

		m_pControlNode = m_pSpriteNode;
		m_bUseStretch = false;
		m_fInitScale = 1.0f;
		parent->getCurrentNode()->addChild(m_pSpriteNode, m_fPosZ);
	}
    
    UIPicture::~UIPicture()
    {
        
    }
    
    void UIPicture::setPosition(CCPoint pt) 
    {
        UIControlBase::setPosition(pt);
        
        if (m_pSpriteNode)
		{
            m_pSpriteNode->setPosition(pt);
        }
    }

	void UIPicture::setUseStretch(bool useStretch)
	{
		m_bUseStretch = useStretch;
	}

	void UIPicture::setScale(float scale)
	{
		UIControlBase::setScale(scale);

		if(m_pSpriteNode != NULL)
		{
			if(m_bUseStretch)
			{
				// currently only support background
				float scaleX = UIManager::sharedManager()->getScaleFactorX();
				float scaleY = UIManager::sharedManager()->getScaleFactorY();

				m_pSpriteNode->setScaleX(scaleX);
				m_pSpriteNode->setScaleY(scaleY);
			}
			else
			{
				m_pSpriteNode->setScale(m_fInitScale * scale);
			}
		}
	}

	void UIPicture::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);

		if(m_pSpriteNode != NULL)
		{
			if(m_bUseStretch)
			{
				// currently only support background
				float scaleX = UIManager::sharedManager()->getScaleFactorX();
				float scaleY = UIManager::sharedManager()->getScaleFactorY();

				m_pSpriteNode->setScaleX(scaleX);
				m_pSpriteNode->setScaleY(scaleY);
			}
			else
			{
				m_pSpriteNode->setScaleX(m_fInitScale * xScale);
				m_pSpriteNode->setScaleY(m_fInitScale * yScale);
			}
		}
	}

	void UIPicture::setInitScale(float scale)
	{
		m_fInitScale  = scale;
	}

	void UIPicture::setSprite(CCSprite *pSprite,bool isMerge)
	{
		if(pSprite == NULL)
		{
			if(m_pSpriteNode == NULL)
			{
				return;
			}
			else
			{
				m_pSpriteNode->removeFromParentAndCleanup(true);
				m_pSpriteNode = NULL;
				m_pControlNode = NULL;
				return;
			}
		}

		float scale = UIManager::sharedManager()->getScaleFactor();

		if(m_pSpriteNode != NULL)
		{
			scale = m_pSpriteNode->getScale();
			m_pSpriteNode->removeFromParentAndCleanup(true);
			m_pSpriteNode = NULL;
		}
				
		float orientSclae = pSprite->getScale();

		if(isMerge)
		{
			m_pSpriteNode = MergeSmallASprite(pSprite);
		}
		else
		{
			m_pSpriteNode = pSprite;
		}
		
		m_pControlNode = m_pSpriteNode;
		m_pControlNode->setPosition(m_ptLocalPosition);
		m_pSpriteNode->setScale(scale*orientSclae);

		if(m_pParentNode != NULL)
		{
			m_pParentNode->addChild(m_pSpriteNode, m_fPosZ);
		}
	}

    void UIPicture::setMySprite(CCSprite* pSprite)
    {
        if(pSprite == NULL)
        {
            if(m_pSpriteNode == NULL)
            {
                return;
            }
            else
            {
                m_pSpriteNode->removeFromParentAndCleanup(true);
                m_pSpriteNode = NULL;
                m_pControlNode = NULL;
                return;
            }
        }

        float scale = UIManager::sharedManager()->getScaleFactor();

        if(m_pSpriteNode != NULL)
        {
            m_pSpriteNode->removeFromParentAndCleanup(true);
            m_pSpriteNode = NULL;
        }

        float orientSclae = pSprite->getScale();
        m_pSpriteNode = pSprite;

        m_pControlNode = m_pSpriteNode;
        m_pControlNode->setPosition(m_ptLocalPosition);
        m_pSpriteNode->setScale(scale * orientSclae);

        if(m_pParentNode != NULL)
        {
            m_pParentNode->addChild(m_pSpriteNode, m_fPosZ);
        }
    }

	//// merge small piece asprite to a big one
	CCNode* UIPicture::MergeSmallASprite(CCSprite* sprite)
	{
		if (!sprite)
			return NULL;

		CCTexture2D* ptexture = RecursiveFindTexture(sprite);
		if (ptexture == NULL)
			return sprite;

		if(sprite->getChildrenCount() == 0)
		{
			return sprite;
		}

		CCArray* secondTextureSprite = CCArray::create();
		CCArray* otherChildren = CCArray::create();
		while (sprite->getChildrenCount() > 0)
		{
			CCTexture2D* ptexture = RecursiveFindTexture(sprite);

			/// create new batch node which used to store a series of sprites
			CCSpriteBatchNode * pNode = CCSpriteBatchNode::createWithTexture(ptexture);

			CCSize size = sprite->getContentSize();
			CCPoint pt = sprite->getPosition();

			pNode->setContentSize(size);
			pNode->setAnchorPoint(ccp(0.0, 0.0));

			secondTextureSprite->removeAllObjects();
			int i = 0;
			while(sprite->getChildrenCount() > 0)
			{
				CCArray* children = sprite->getChildren();
				CCObject* hastObj = children->lastObject();
				CCSprite* s1 = dynamic_cast<CCSprite*>(hastObj);	
				if(s1)
				{
					sprite->removeChild(s1, false);

					if (s1->getTexture() == ptexture)
					{
						pNode->addChild(s1, i--);
					}else
					{
						if(s1->getTexture() == NULL)
						{
							CCArray* children = s1->getChildren();
							CCObject* pTemp;
							CCARRAY_FOREACH(children, pTemp)
							{
								CCSprite* pPst = dynamic_cast<CCSprite*>(pTemp);
								if(pPst)
								{
									s1->removeChild(pPst, false);
									secondTextureSprite->addObject(pPst);
								}
							}
						}
						else
						{
							secondTextureSprite->addObject(s1);
						}
					}
				}
				else
				{
					CCNode* tempNode = dynamic_cast<CCNode*>(hastObj);
					CCAssert(tempNode != NULL);
					sprite->removeChild(tempNode, false);
					otherChildren->addObject(tempNode);
				}
				
			}

			/// copy second texture sprite to sprite
			CCObject* pTemp;
			CCARRAY_FOREACH(secondTextureSprite, pTemp)
			{
				//CCSprite* pPst = (CCSprite*) pTemp;
				CCNode* pPst = dynamic_cast<CCNode*>(pTemp);
				if(pPst)
				{
					sprite->addChild(pPst);
				}
			}

			if (!m_pSpriteNode)
			{
				/// 第一次创建时，取第一个sprite位置为当前m_pSpriteNode的位置
				m_pSpriteNode = CCNode::create();
				m_pSpriteNode->setPosition(pt);
				m_pSpriteNode->setContentSize(size);
				m_pSpriteNode->setAnchorPoint(sprite->getAnchorPoint());

				pNode->setPosition(CCPointZero);
			}else
			{
				/// convert other sprite to local position 
				CCPoint ptOrgio = m_pSpriteNode->getPosition();
				pt = ccpSub(pt , ptOrgio);
				pNode->setPosition(pt);
			}

			m_pSpriteNode->addChild(pNode);	
		}	
		CCObject* pTemp;
		CCARRAY_FOREACH(otherChildren, pTemp)
		{
			//CCSprite* pPst = (CCSprite*) pTemp;
			CCNode* pPst = dynamic_cast<CCNode*>(pTemp);
			if(pPst)
			{
				m_pSpriteNode->addChild(pPst);
			}
		}

		return m_pSpriteNode;
	}
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UIPicture::UIPicture()
    {
        m_pSpriteNode = NULL;
        m_pControlNode = NULL;
		m_bUseStretch = false;
		m_fInitScale = 1.0f;
    }
    
    
    void UIPicture::LoadBinary(NiStream &kStream)
    {
        UIControlBase::LoadBinary(kStream);
        
        mutableDic* dic = kStream.GetStreamData();

		uint32_t directLoad = 0;
		string directPath = "";
		kStream.getIntAttributeValue(dic, "directLoad", directLoad);
		if(directLoad)
		{
			kStream.getStringattributeValue(dic, "directPath", directPath);
		}

		uint32_t directFrame = 0;
		kStream.getIntAttributeValue(dic, "directFrame", directFrame);

		uint32_t useStretch = 0;
		kStream.getIntAttributeValue(dic, "useStretch", useStretch);
		m_bUseStretch = (useStretch == 1);

		uint32_t initScale = 100;
		kStream.getIntAttributeValue(dic, "initScale", initScale);
		m_fInitScale = (float)initScale / 100.0f;

		std::string resourceName = "";
		kStream.getStringattributeValue(dic, "resourceName", resourceName);

		std::string iconName = "";
		kStream.getStringattributeValue(dic, "iconTexture", iconName);

		std::string iconBinFile = "";
		kStream.getStringattributeValue(dic, "iconBinFile", iconBinFile);

		CCSprite* pSprite = NULL;

		//load from file directly
		if(directLoad)
		{
			directPath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(directPath.c_str());
			pSprite = CCSprite::create(directPath.c_str());
		}
		else
		{
			if(directFrame == 1)
			{
				string frameFile = "";
				kStream.getStringattributeValue(dic, "frameFile", frameFile);
				if(frameFile != "")
				{
					string frameFileFullPath = GameResourceManager::sharedManager()
						->storedFullPathFromRelativePath(frameFile.c_str());
					GameResourceManager::sharedManager()
						->addSpriteFrameWithDictionaryFromCache(frameFileFullPath.c_str());
				}
				pSprite = CCSprite::createWithSpriteFrameName(resourceName.c_str());
			}
			else
			{
				// read sprite id first
				int iSpriteFrame = getResourceIDByName(resourceName.c_str());
        
				// load ASpriteManger resource file
				ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
				mutableDic* dicAsMgr = static_cast<mutableDic*>(dic->objectForKey("AspriteManager"));
				// content attributes 
				if (dicAsMgr)
				{
					string ASfile = KUI_BIN;
					if (IsIpad())
					{
						kStream.getStringattributeValue(dicAsMgr, "ipad", ASfile); 
					}else
					{
						kStream.getStringattributeValue(dicAsMgr, "iphone", ASfile); 
					}

					/// add images for mutilple Images usage in AuroraGT Tools
					vector<string> images;
					mutableDic* dicImages = static_cast<mutableDic*>(dicAsMgr->objectForKey("images"));
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
					
					as = AspriteManager::getInstance()->getAsprite(ASfile); 
					/// set up as sprite images
					for(size_t i = 0; i < images.size(); i++)
					{
						if (as)
						{
							as->setImageName(i, images[i]);
						}
					}
				}

				pSprite = as->getSpriteFromFrame_Middle(iSpriteFrame, 0, m_ptWorldPosition);

				uint32_t useASposition = 0;
				if(kStream.getIntAttributeValue(dic, "useASposition", useASposition) && useASposition == 1)
				{
					/** if we use AS_bin file's frame position
					 * local position should reset from parent's world position
					 */
					CCPoint parentWorldPosition = CCPointZero;
					if(m_pParent != NULL)
					{
						parentWorldPosition = m_pParent->getWorldPosition();
					}

					m_ptLocalPosition.x = m_ptWorldPosition.x - parentWorldPosition.x;
					m_ptLocalPosition.y = m_ptWorldPosition.y - parentWorldPosition.y;
				}
				else
				{
					// we modified it, so reset it
					UIControlBase::CalWorldPos();
				}
			}
		}

		if(iconName != "")
		{
			ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
			if (iconBinFile != "")
			{
				as = AspriteManager::getInstance()->getAsprite(iconBinFile); 
			}
			int iSpriteFrame = getResourceIDByName(iconName.c_str());
			pSprite = as->getSpriteFromFrame_Middle(iSpriteFrame, 0, m_ptWorldPosition);
		}
		//if(m_bUseStretch)
		//{
		//	uint32_t widthCut = DEFAULT_CUT;
		//	uint32_t heightCut = DEFAULT_CUT;
		//	kStream.getIntAttributeValue(dic, "widthCut", widthCut);
		//	kStream.getIntAttributeValue(dic, "heightCut", heightCut);

		//	pSprite = CCScale9Sprite::scale9SpriteWithSprite(pSprite, widthCut, heightCut);
		//}
        
        if (pSprite == NULL)
        {
			CCLOG("error! Can't find UIPicture's Sprite! Create default node!");
			int width = 16 / CC_CONTENT_SCALE_FACTOR();
			int height = 16 / CC_CONTENT_SCALE_FACTOR();
			int *pixels = new int[width * height];
			for (int i=0; i<width; ++i) {
				for (int j=0; j<height; ++j) {
					//pixels[i][j] = 0xffffffff;
					pixels[i*height+j] = 0xff0000ff;
				}
			}

			CCTexture2D *texture = new CCTexture2D();
			texture->initWithData(pixels,
				kCCTexture2DPixelFormat_RGB888,
				1,
				1,
				CCSizeMake(width, height)
				);

			delete[] pixels;

			pSprite = CCSprite::createWithTexture(texture);
			texture->release();
        }

		m_pSpriteNode = MergeSmallASprite(pSprite);

		m_pControlNode = m_pSpriteNode;
		m_pControlNode->setVisible(m_bIsVisible);
		m_pControlNode->setScale(m_fInitScale);
		m_pControlNode->setPosition(m_ptLocalPosition);
    }
    
    void UIPicture::LinkObject(NiStream &kStream)
    {
        UIControlBase::LinkObject(kStream);
    }
    
    bool UIPicture::RegisterStreamables(NiStream &kStream)
    {
        if (! UIControlBase::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UIPicture::SaveBinary(NiStream &kStream)
    {
        UIControlBase::SaveBinary(kStream);        
    }
    
    bool UIPicture::IsEqual(NiObject *pObject)
    {
        if (! UIControlBase::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }    

	CCPoint UIPicture::getSpritePos()
	{
		if(m_pSpriteNode)
		{
			CCPoint localPoint = m_pSpriteNode->getPosition();
			if(m_pSpriteNode->getParent())
			{
				return m_pSpriteNode->getParent()->convertToWorldSpace(localPoint);
			}
			//return m_pSprite->convertToWorldSpace(CCPointZero);
		}
		return CCPointZero;
	}

	CCSize UIPicture::getSpriteSize()
	{
		if(m_pSpriteNode)
		{
			CCSize size = m_pSpriteNode->getContentSize();
			float scaleX = m_pSpriteNode->getScaleX();
			float scaleY = m_pSpriteNode->getScaleY();
			return CCSize(size.width * scaleX, size.height * scaleY);
		}
		return CCSizeZero;
	}

	void UIPicture::addExtraPic(CCSprite *sprite, CCPoint pt)
	{
		if(!sprite)
		{
			return;
		}

		if(m_pSpriteNode)
		{
			sprite->setAnchorPoint(ccp(0.5f,0.5f));
			m_pSpriteNode->addChild(sprite);
			CCSize size = m_pSpriteNode->getContentSize();
			sprite->setPosition(ccpAdd(pt,ccp(size.width /2,size.height /2)));
		}
	}
}

