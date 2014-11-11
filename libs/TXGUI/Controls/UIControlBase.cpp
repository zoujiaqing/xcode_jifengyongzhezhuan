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
//by Stanley
#include <algorithm>
#include "UIControlBase.h"
#include "IconButton.h"
#include "TXAudioEngine.h"
#include "CCLuaEngine.h"
#include "UIManager.h"
#include "tolua++.h"
#include "GameResourceManager.h"

#include "AspriteManager.h"
#include "CrossPlatform.h"
#include "Localization.h"

namespace TXGUI { 
    USING_NS_CC;
    
    NiImplementRTTI(UIControlBase, NiObject);
    
    UIControlBase::UIControlBase(cocos2d::CCNode* parent)
    {
        m_iTag = -1;
        m_iAudioEffect = -1;
        m_ptLocalPosition = m_ptWorldPosition = CCPointZero;
		m_fPosZ = 0.0f;
		m_bScaleSetted = false;
		m_bIsVisible = true;
        
        m_pParentNode = parent;
        m_pParent = NULL;
		//m_pRoot = NULL;
        m_pBindNode = NULL;
        m_pControlNode = NULL;
		m_strLuaScriptFile = "";
		m_uiContentSize = CCSizeZero;
    }

	UIControlBase::UIControlBase(std::string name, int tag, float z, UIControlBase *parent, CCSize size)
	{
		m_strName = name;
		m_iTag = tag;
		m_iAudioEffect = -1;
		m_ptLocalPosition = m_ptWorldPosition = CCPointZero;
		m_fPosZ = z;
		m_bScaleSetted = false;
        m_bIsVisible = true;

		m_pParentNode = parent->getCurrentNode();
		m_pParent = NULL;
		//m_pRoot = NULL;
		m_pBindNode = NULL;
		m_pControlNode = NULL;
		m_strLuaScriptFile = "";
		m_uiContentSize = size;
		setParent(parent);
	}
    
	UIControlBase::UIControlBase(std::string name, int tag, float z, UIControlBase *parent, CCSize size, CCNode* controlNode)
	{
		m_strName = name;
		m_iTag = tag;
		m_iAudioEffect = -1;
		m_ptLocalPosition = m_ptWorldPosition = CCPointZero;
		m_fPosZ = z;
		m_bScaleSetted = false;
        m_bIsVisible = true;
        
		m_pParentNode = parent->getCurrentNode();
		m_pParent = NULL;
		//m_pRoot = NULL;
		m_pBindNode = NULL;
		m_pControlNode = controlNode;
		controlNode->setAnchorPoint(ccp(0.5,0.5));
		m_strLuaScriptFile = "";
		m_uiContentSize = size;
		setParent(parent);
		BindNode(m_pParentNode);
		float scale = UIManager::sharedManager()->getScaleFactor();
		controlNode->setScale(scale);
		setScale(scale);
	}

    UIControlBase::~UIControlBase()
    {        
        
        if (m_iAudioEffect > 0)
        {
            StopAudio();
        }
        
        // compare children name according to the given parameters name
        for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
            it != m_ChildrenControls.end(); 
            it++)
        {
            UIControlBase* pChild = *it;
            delete pChild;
        } 
        
        m_ChildrenControls.clear();        
        
        if (m_pParentNode)
        {
			if (m_pControlNode && m_pControlNode->getParent())
            {
                m_pControlNode->removeFromParentAndCleanup(true);
            }
        }
    } 
    
    CCPoint UIControlBase::LocalPt2World(CCPoint pt)
    {
        CCAffineTransform local2world = getLocal2WorldTransform();
        CCPoint p1 = CCPointApplyAffineTransform(CCPointZero, local2world);        
        return p1;
    }
    
    void UIControlBase::PlayAudio()
    {
        TXAudioEngine::Instance()->PlayAudio(m_iAudioEffect);
    }
    
    void UIControlBase::StopAudio()
    {
        TXAudioEngine::Instance()->StopAudio(m_iAudioEffect);            
    }
    
    void UIControlBase::PauseAudio()
    {
        TXAudioEngine::Instance()->PauseAudio(m_iAudioEffect);  
    }    
    
    void UIControlBase::setParent(UIControlBase* pParent)
    {
        //CCLOG("Name = %s", m_strName.c_str());
        // removed from old parent
        if (m_pParent != NULL)
            m_pParent->RemoveChild(this);
        
        // add to new parent
        m_pParent = pParent;
        if (m_pParent != NULL)
        {
            //CCLOG("Parent = %s", m_pParent->getName().c_str());
            m_pParent->AddChild(this);
        }
    }
    
    /**
     * @brief 增加一个子结点
     * @param pChild, 子结点
     * @return void, 
     * @
     */
    void UIControlBase::AddChild(UIControlBase* pChild)
    {
        if (find(m_ChildrenControls.begin(), m_ChildrenControls.end(), pChild)
            == m_ChildrenControls.end())
        {
            m_ChildrenControls.push_back(pChild);
        }        
    }
    
    /**
     * @brief 删除一个子结点
     * @param pChild, 子结点
     * @return void, 
     * @
     */
    void UIControlBase::RemoveChild(UIControlBase* pChild, bool clean)
    {
        if (find(m_ChildrenControls.begin(), m_ChildrenControls.end(), pChild)
            != m_ChildrenControls.end())
        {
            m_ChildrenControls.remove(pChild);
			if(clean)
			{
				delete pChild;
			}
        } 
    }

	void UIControlBase::RemoveAllChildrenWithCleanup()
	{
		std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
		for(; it != m_ChildrenControls.end(); it++)
		{
			if((*it) != NULL)
			{
				delete *it;
			}
		}
		m_ChildrenControls.clear();
	}

	void UIControlBase::RemoveChildTreeByName(string name)
	{
		UIControlBase *childNode = this->FindChildObjectByName(name);
		if(childNode != NULL)
		{
			childNode->getParent()->RemoveChild(childNode);
			delete childNode;
		}
	}
    
    /**
     * @brief 根据名字，查找一个子节点对象
     * @param name, 子结点名字
     * @return UIControlBase*, 节点对象
     * @
     */        
    UIControlBase* UIControlBase::FindChildObjectByName(string name)
    {        
        // compare children name according to the given parameters name
        for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
            it != m_ChildrenControls.end(); 
            it++)
        {
            UIControlBase* pChild = *it;
            string val = pChild->getName();
            if (val.compare(name) == 0 && val.length() == name.length())
            {
                return pChild;
            }
        }   
        
        // search children node
        for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
            it != m_ChildrenControls.end(); 
            it++)
        {
            UIControlBase* pChild = (*it)->FindChildObjectByName(name);
            if (NULL != pChild)
            {
                return pChild;
            }
        } 
        
        return NULL;
    }    
    
    void UIControlBase::BindNode(CCNode* pParentNode)
    {
        m_pBindNode = pParentNode;
		m_pParentNode = pParentNode;

		CCNode *pNode = getCurrentNode();
		pParentNode->addChild(pNode, m_fPosZ);
        
        std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
        for(; it != m_ChildrenControls.end(); it++)
        {
            (*it)->BindNode(pNode);
        }
    }
    
    CCNode* UIControlBase::getBindNode()
    {
        return m_pBindNode;
    }

	/** Override synthesized setOpacity to recurse items */
	void UIControlBase::setOpacity(GLubyte var)
	{
		CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_pControlNode);
		if (pRGBAProtocol)
		{
			pRGBAProtocol->setOpacity(var);
		}
	}

	void UIControlBase::setVisible(bool visible)
	{
		if (getCurrentNode())
		{
			getCurrentNode()->setVisible(visible);
		}
	}

	void UIControlBase::setLuaScriptFile(std::string strFile)
	{
		m_strLuaScriptFile = strFile;
	}

	std::string UIControlBase::getLuaScriptFile()
	{
		return m_strLuaScriptFile;
	}

	void UIControlBase::executeLuaScript()
	{
		if(m_strLuaScriptFile == "")
		{
			return;
		}
#if 0
		std::string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(m_strLuaScriptFile.c_str());
		CCString* pstrFileContent = CCString::createWithContentsOfFile(path.c_str());

        if (pstrFileContent)
		{
			CCScriptEngineManager::sharedManager()->getScriptEngine()->executeString(pstrFileContent->getCString());
		}
#else
        string filePath(m_strLuaScriptFile);
		const char *path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(filePath.c_str());
      
		unsigned long  fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);

      //  CCLOG("%s, file: %s", __FUNCTION__, path);
        lua_State *curLuaState =  CCLuaEngine::defaultEngine()->getLuaState();
         int ret = luaL_loadbuffer(curLuaState, (char*)buffer,fileSize,filePath.c_str())||lua_pcall(curLuaState,0,LUA_MULTRET,0);
        delete(buffer);
        if (ret != 0) {

			CCLOG("load %s file error: %s", filePath.c_str(), lua_tostring(curLuaState, -1));
            
            lua_pop(curLuaState, 1);
			
		}
#endif
    

	}
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    UIControlBase::UIControlBase()
    {
        m_pParent = NULL;
		//m_pRoot = NULL;
        m_pBindNode = NULL;
		m_pControlNode = NULL;
		m_bScaleSetted = false;
		m_bIsVisible = true;
		m_strName = "";
    }
    
    void UIControlBase::LoadBinary(NiStream &kStream)
    {
        NiObject::LoadBinary(kStream);
        
        mutableDic* dic = kStream.GetStreamData();
        
        float deviceScale = CC_CONTENT_SCALE_FACTOR();
        
        // transform parts attributes
        uint32_t posX = 0;
        uint32_t posY = 0;
        uint32_t posZ = 0;
        mutableDic* transfrom = static_cast<mutableDic*>(dic->objectForKey("transform"));
        if (transfrom != NULL) {
            kStream.getIntAttributeValue(transfrom, "x", posX);
            kStream.getIntAttributeValue(transfrom, "y", posY);
            kStream.getIntAttributeValue(transfrom, "z", posZ);
        }        
        m_ptLocalPosition.x = (int)posX / deviceScale;
        m_ptLocalPosition.y = (int)posY / deviceScale;
		m_fPosZ = (int)posZ;
        
        // name attributes
        kStream.getStringattributeValue(dic, "name", m_strName);

        uint32_t tag = -1;
        kStream.getIntAttributeValue(dic, "tag", tag);

		std::string strFile = "";
		kStream.getStringattributeValue(dic, "luaScript", strFile);

		uint32_t isVisible = 1;
		kStream.getIntAttributeValue(dic, "isVisible", isVisible);
		m_bIsVisible = (isVisible == 1)? true : false;
        
        setPosition(m_ptLocalPosition);
        setTag(tag);
		setLuaScriptFile(strFile);
		CalWorldPos();
    }
    
    void UIControlBase::LinkObject(NiStream &kStream)
    {
        NiObject::LinkObject(kStream);
    }
    
    bool UIControlBase::RegisterStreamables(NiStream &kStream)
    {
        if (! NiObject::RegisterStreamables(kStream))            
        {
            return false;           
        }
        
        // Register ExtraData
        
        return true;
    }
    
    void UIControlBase::SaveBinary(NiStream &kStream)
    {
        NiObject::SaveBinary(kStream);        
    }
    
    bool UIControlBase::IsEqual(NiObject *pObject)
    {
        if (! NiObject::IsEqual(pObject))
            return false;
        
        // Test for extra data equality, but ignore non-streamable data
        
        
        return true;
    }   

	void UIControlBase::setTouchEnabled(bool value)
	{
		for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
			it != m_ChildrenControls.end(); 
			it++)
		{
			UIControlBase* pChild = *it;
			pChild->setTouchEnabled(value);

			CCLayer* pLayer = dynamic_cast<CCLayer*>(pChild->getCurrentNode());
			if(pLayer)
			{
				pLayer->setTouchEnabled(value);
			}
		}   
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// for optimization 
	CCTexture2D* UIControlBase::RecursiveFindTexture(CCSprite* sprite)
	{
		if (sprite->getTexture())
		{
			return sprite->getTexture();
		}
		else
		{
			CCObject* pObj;
			CCARRAY_FOREACH(sprite->getChildren(), pObj)
			{
				CCSprite* child = dynamic_cast<CCSprite*>(pObj);
				if(child)
				{
					CCTexture2D* tex = RecursiveFindTexture(child);
					if (tex)
						return tex;
				}
			}
		}

		return NULL;
	}

	void  UIControlBase::GetColor(mutableDic* dic, NiStream &kStream, uint32_t& r, uint32_t& g, uint32_t & b)
	{
		mutableDic *colorDic = static_cast<mutableDic*>(dic->objectForKey("color"));
		if(colorDic)
		{
			kStream.getIntAttributeValue(colorDic, "red", r);
			kStream.getIntAttributeValue(colorDic, "green", g);
			kStream.getIntAttributeValue(colorDic, "blue", b);
		}
	}

	string  UIControlBase::GetContent(mutableDic* dic, NiStream &kStream)
	{
		// content attributes 
		uint32_t bLocalizeString = 0;
		kStream.getIntAttributeValue(dic, "bLocalizeString", bLocalizeString);

		std::string strTemp = "";
		kStream.getStringattributeValue(dic, "content", strTemp);
		const char* content = strTemp.c_str();
		if (bLocalizeString > 0)
		{
			content = Localizatioin::getLocalization(content);
		}
		strTemp = content;

		return strTemp;
	}
	
	ASprite* UIControlBase::GetASpriteCfg(NiStream &kStream, mutableDic *bgDic)
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
		mutableDic* dicAsMgr = static_cast<mutableDic*>(bgDic->objectForKey("AspriteManager"));
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
			as = AspriteManager::getInstance()->getAsprite(ASfile); 
		} 

		return as;
	}

	CCSprite* UIControlBase::GetASprite(NiStream &kStream, mutableDic *bgDic, ASprite* as, string key)
	{
		string texname;
		kStream.getStringattributeValue(bgDic, key, texname);
		if (texname.empty())
			return NULL;
		CCPoint pt;
		CCSprite* pSpriteNormal = as->getSpriteFromFrame_Middle(getResourceIDByName(texname.c_str()), 0, pt);
		return pSpriteNormal;
	}

	CCSprite* UIControlBase::GetASprite(NiStream &kStream, mutableDic *bgDic, ASprite* as, string key, CCPoint& pt)
	{
		string bgName = "";
		kStream.getStringattributeValue(bgDic, key, bgName);
		CCSprite* pSprite = as->getSpriteFromFrame_Middle(getResourceIDByName(bgName.c_str()), 0, pt);
		return pSprite;
	}

	IconButton* UIControlBase::FindIconButtonChildByName(string name)
	{
		CCNode* root = this->getCurrentNode();
		return FindIconButtonInLayer(root,name);
	}

	IconButton* UIControlBase::FindIconButtonInLayer(CCNode* root,string name)
	{
		//CCLayer* layer = dynamic_cast<CCLayer*>(root);
		CCNode* layer = dynamic_cast<CCNode*>(root);
		if(layer == NULL)
		{
			return NULL;
		}
		CCArray* children = layer->getChildren();
		if ( children && children->count() > 0 )
		{
			CCObject* child;
			CCARRAY_FOREACH(children, child)
			{
				IconButton* button = dynamic_cast<IconButton*>(child);
				if(button)
				{
					if(button->m_name.compare(name) == 0 && button->m_name.length() == name.length())
					{
						return button;
					}
				}
				else
				{
					//CCLayer* childLayer = dynamic_cast<CCLayer*>(child);
					CCNode* childLayer = dynamic_cast<CCNode*>(child);
					if(childLayer)
					{
						button = FindIconButtonInLayer(childLayer,name);
						if(button)
						{
							return button;
						}
					}
				}
			}
		}

		return NULL;
	}

	//// merge small piece asprite to a big one
	CCSprite* UIControlBase::MergeSmallASprite(CCSprite* sprite)
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

		CCSprite* pSprite = NULL;

		CCArray* secondTextureSprite = CCArray::create();

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
				CCSprite* s1 = (CCSprite*)(children->lastObject());			
				sprite->removeChild(s1, false);

				if (s1->getTexture() == ptexture)
				{
					pNode->addChild(s1, i--);
				}else
				{
					secondTextureSprite->addObject(s1);
				}
			}

			/// copy second texture sprite to sprite
			CCObject* pTemp;
			CCARRAY_FOREACH(secondTextureSprite, pTemp)
			{
				CCSprite* pPst = (CCSprite*) pTemp;
				sprite->addChild(pPst);
			}

			if (!pSprite)
			{
				/// 第一次创建时，取第一个sprite位置为当前m_pSpriteNode的位置
				pSprite = CCSprite::create();
				pSprite->setPosition(pt);
				pSprite->setContentSize(size);
				pSprite->setAnchorPoint(sprite->getAnchorPoint());

				pNode->setPosition(CCPointZero);
			}else
			{
				/// convert other sprite to local position 
				CCPoint ptOrgio = pSprite->getPosition();
				pt = ccpSub(pt , ptOrgio);
				pNode->setPosition(pt);
			}

			pSprite->addChild(pNode);	
		}	

		return pSprite;
	}
}
