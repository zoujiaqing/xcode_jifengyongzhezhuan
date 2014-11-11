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

#include "NiStream.h"
#include "UIControlBase.h"
#include "GameResourceManager.h"

namespace TXGUI {
    
    std::map<int, UIControlBase*> NiStream::_controlsDic;
    std::map<string, NiStream::createFunction> NiStream::ms_kLoaders;
    
    cocos2d::ccColor3B NiStream::convertToColor3B(uint32_t value)
    {
        GLubyte r = 0, g = 0, b = 0;
        uint32_t bitValue = 0;
        
        bitValue = value & 0x00ff0000;
        r = (GLubyte)(bitValue >> 16);
        
        bitValue = value & 0x0000ff00;
        g = (GLubyte)(bitValue >> 8);
        
        bitValue = value & 0x000000ff;
        b = (GLubyte)bitValue;
        
        return ccc3(r, g, b);
    }    
    
    NiStream::NiStream()
    {
        m_strFile = "";
        m_pControl= NULL;
        m_pCurrentLinkedDic = NULL;
    }
    
    NiStream::~NiStream()
    {
        if (m_pCurrentLinkedDic)
        {           
            m_pCurrentLinkedDic = NULL;
        }
    }
    
    UIControlBase* NiStream::CreateControlFromFile(const char * file, cocos2d::CCNode* parent)
    {
        assert(file != NULL);
        m_strFile = file;
        
#ifdef TXGUIUTIL
        const char* relativePath = file;
#else
        const char* relativePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(file);
#endif
        if (NULL == relativePath) 
            return NULL;

		mutableArray* level = GameResourceManager::sharedManager()->arrayFromCache(relativePath);
        
        if (level == NULL || level->count() < 1)
            return NULL;
        
        mutableDic* root = static_cast<mutableDic*>(level->objectAtIndex(0));
        if (root == NULL)
            return NULL;  
        
        return parseControls(parent, root);
    }
    
    UIControlBase* NiStream::parseControls(cocos2d::CCNode* parent, mutableDic* dic) 
    {
        if (dic == NULL) 
            return NULL;
        _ParentNode = parent;        
        m_pCurrentLinkedDic = dic;
        
        cocos2d::CCString* ccType = static_cast<cocos2d::CCString*>(dic->objectForKey("type"));
        std::string type = ccType->toStdString();
        
        delete m_pControl;
        m_pControl = NULL;
        
        // create current type creator ,and create control 
        createFunction pfnCreate = NULL;
        std::map<string, createFunction>::iterator itor = ms_kLoaders.find(type);
        if (itor != ms_kLoaders.end())
        {
            pfnCreate = (*itor).second;
        }
        
        if (pfnCreate == NULL)
        {
            CCLOG("Error!, NiStream::parseControls() ! type £½ %s", type.c_str());
            return NULL;
        }

        m_pControl = (UIControlBase*)(*pfnCreate)();
        m_pControl->LoadBinary(*this);

        if (m_pControl != NULL && _controlsDic.find(m_pControl->getTag()) == _controlsDic.end()) {
            _controlsDic[m_pControl->getTag()] = m_pControl;
//            CCLOG("Add control id:%i into controls dic.", m_pControl->getTag());
        }
        
        mutableArray* children = static_cast<mutableArray*>(dic->objectForKey("children"));
        if (children == NULL)
        {
            m_pControl->BindNode(_ParentNode);
			//m_pControl->setRoot(m_pControl);
            return m_pControl;
        }

        //create control from control data 
        internalCreateControl(&m_pControl, children);   
        
        m_pControl->BindNode(_ParentNode);
		//m_pControl->setRoot(m_pControl);
        
        return m_pControl;
    }
    
    
    void NiStream::internalCreateControl(UIControlBase** pControl, CCObject* pData)
    {
        mutableArray* children = static_cast<mutableArray*>(pData);
        if (children == NULL)
            return;
        
        UIControlBase* pParent = *pControl;
        
        for (size_t i = 0; i < children->count(); i++) 
        {
            mutableDic* dic = (mutableDic*)(children->objectAtIndex(i));           
            
            m_pCurrentLinkedDic = dic;
            
            cocos2d::CCString* ccType = static_cast<cocos2d::CCString*>(dic->objectForKey("type"));
            std::string type = ccType->toStdString();

            // create current type creator ,and create control 
            createFunction pfnCreate = NULL;
            std::map<string, createFunction>::iterator itor = ms_kLoaders.find(type);
            if (itor != ms_kLoaders.end())
            {
                pfnCreate = (*itor).second;
            }
            
            if (pfnCreate == NULL)
            {
                CCLOG("Error!, NiStream::parseControls() ! child type = %s", type.c_str());
                continue ;
            }
            
            UIControlBase* pChildControl = (UIControlBase*)(*pfnCreate)();

			// set parent first
            pChildControl->setParent(pParent);    
			
            pChildControl->LoadBinary(*this);
            
            // get the transform
            //CCPoint ptWorld = pChildControl->GetWorldPosition();
            
            // need to transform world position to relative position 
            // call subclass's function and transform world position to local
            //pChildControl->SetWorldPosition(ptWorld);
            
            if (pChildControl != NULL && _controlsDic.find(pChildControl->getTag()) == _controlsDic.end())
            {
                _controlsDic[pChildControl->getTag()] = pChildControl;
//                CCLOG("Add control id:%i into controls dic.", pChildControl->getTag());
            }
            
            // parse chilren node
            mutableArray* children = static_cast<mutableArray*>(dic->objectForKey("children"));
           
            //create control from control data
			if(children != NULL)
			{
				internalCreateControl(&pChildControl, children);
			}
        }
        
        return;
    }
    
    bool NiStream::getIntAttributeValue(mutableDic* dic, 
                                             std::string key, 
                                             uint32_t& value) 
    {
        if (dic == NULL)
            return false;
        
        CCString* keyValue = static_cast<CCString*>(dic->objectForKey(key.c_str()));
        if (keyValue == NULL)
            return false;
        
        value = keyValue->toUInt();
        
        return true;
    }

	bool NiStream::getSignedIntAttributeValue(mutableDic* dic, 
		std::string key, 
		int32_t& value)
	{
		if (dic == NULL)
			return false;

		CCString* keyValue = static_cast<CCString*>(dic->objectForKey(key.c_str()));
		if (keyValue == NULL)
			return false;

		value = keyValue->toInt();

		return true;
	}
    
    bool NiStream::getStringattributeValue(mutableDic* dic,
                                                std::string key, 
                                                std::string& value)
    {
        if (dic == NULL)
            return false;
        
        CCString* keyValue = static_cast<CCString*>(dic->objectForKey(key.c_str()));
        if (keyValue == NULL)
            return false;
        
        value = keyValue->toStdString();
        
        return true;
    }
    
    bool NiStream::getDictionaryAttributeValue(mutableDic* dic,
                                                    std::string key, 
                                                    mutableDic*& value) 
    {
        if (dic == NULL)
            return false;
        
        mutableDic* keyValue = static_cast<mutableDic*>(dic->objectForKey(key.c_str()));
        if (keyValue == NULL)
            return false;
        
        value = keyValue;
        return true;
    }
    
    bool NiStream::getAttributePositive(mutableDic* dic, std::string key)
    {
        if (dic == NULL)
            return false;
        
        CCString* keyValue = static_cast<CCString*>(dic->objectForKey(key.c_str()));
        if (keyValue == NULL)
            return false;
        
        return keyValue->toInt() != 0;
    }
    
    bool NiStream::RegisterSaveObject(NiObject *pkObject)
    {
        assert(pkObject);
        
        unsigned int uiLinkID;
        
        if (m_kRegisterMap.find(pkObject) != m_kRegisterMap.end())
            return false;
        
        uiLinkID = m_kObjects.size();
        m_kRegisterMap[pkObject] = uiLinkID;
        m_kObjects.push_back(pkObject);        
        return true;
    }
        
    mutableDic * NiStream::GetStreamData()
    {        
        return m_pCurrentLinkedDic;
    }
    
    int NiStream::RegisterLoader(const char* pcClassName, createFunction pfnFunc)
    {
        ms_kLoaders[pcClassName] = pfnFunc;        
        return 0;
    }
    
    void NiStream::UnregisterLoader(const char * pcClassName)
    {
        if (ms_kLoaders.find(pcClassName) != ms_kLoaders.end())
        {
            ms_kLoaders.erase(pcClassName);
        }
    }
}