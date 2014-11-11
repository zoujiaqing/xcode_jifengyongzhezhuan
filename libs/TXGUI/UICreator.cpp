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
#include <sstream>
#include "UICreator.h"
#include "NiStream.h"

// controls
#include "UIPicture.h"
#include "UILabel.h"
#include "UILabelAtlas.h"
#include "UILayout.h"
#include "UIButton.h"
#include "UIButtonToggle.h"
#include "UIContainer.h"
#include "UIManager.h"
#include "UIScrollList.h"
#include "UIScrollPage.h"
#include "UIAnimation.h"
#include "UITextInputField.h"
#include "UIList.h"
#include "UIEditBox.h"

/// resource manager
#include "GameResourceManager.h"

namespace TXGUI {
    
    ///-------------------------------------------------------------------
    // bonous init and shut donw!
    //
    
    void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
    {
        std::string::size_type pos = 0;
        std::string::size_type srclen = strsrc.size();
        std::string::size_type dstlen = strdst.size();
        
        while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
        {
            strBig.replace( pos, srclen, strdst );
            pos += dstlen;
        }
    }
    
    void string_tripleft(std::string& strBig)
    {
        std::string::iterator pos = strBig.begin();
        
        while(pos != strBig.end())
        {            
            if (*pos != ' ')
            {
                break;
            }              
            strBig.replace(0, 1, "");
            pos = strBig.begin();
        }
    }

	///-------------------------------------------------------------------
	class ResourceDotHFile
	{        
	public :
		ResourceDotHFile(const char * file)
		{          
			assert(file != NULL);        
			CCFileData data(GameResourceManager::sharedManager()->storedFullPathFromRelativePath(file), "rb");
			const char* buffer = (const char*)data.getBuffer();
			std::string strBuffer(buffer);
			std::stringstream ss(stringstream::in | stringstream::out);
			ss << strBuffer<<"\r\n";

			do 
			{
				// read the file from hardware
				char strTemp[1024];
				std::string strVal;   

				while(!ss.eof())
				{
					strTemp[0] = '\0';
					ss.getline(strTemp, 1024);
					//CCLOG("line : %s", strTemp);
					strVal = strTemp;
					string_replace(strVal, "\r\n", "");
					string_replace(strVal, "\t" ," ");
					string_tripleft(strVal);

					std::string split = " ";

					int i0 = strVal.find_first_of(split);
					if (-1 == i0)
						continue;

					std::string def = strVal.substr(0, i0);

					strVal = strVal.substr(i0, strVal.length() - i0);
					string_tripleft(strVal);                    
					int i1 = strVal.find_first_of(split);
					if (-1 == i1)
						continue;                    
					std::string macro = strVal.substr(0, i1);

					std::string val = strVal.substr(i1, strVal.length() - i1);    
					string_tripleft(val);

					if (def.empty() == false && 
						val.empty() == false &&
						def.compare("#define") == 0 && 
						val.length() > 0)
					{
						string_replace(macro, " ", "");
						string_replace(val, " ", "");

						m_mapMacro2Value[macro] = atoi(val.c_str());
					}

					//CCLOG("key : %s = Value %s\n", macro.c_str(), val.c_str());
				}

			} while (0);                                                
		}

		int GetValue(const char * key)
		{
			int i = -1;
			if (m_mapMacro2Value.find(key) != m_mapMacro2Value.end())
			{
				i = m_mapMacro2Value[key];    
			}else
			{
				CCLOGERROR("ResourceDotHFile:GetValue[%s] = -1", key);
			}
			return i;
		}

	protected:
		std::map<string, unsigned int >m_mapMacro2Value;
	};

	static ResourceDotHFile *ms_resourceheader;
    
    void TXMainSDM::Init()
    {
        NiRegisterStream(UIPicture);
        NiRegisterStream(UILabel);
		NiRegisterStream(UILabelAtlas);
        NiRegisterStream(UILayout);
        NiRegisterStream(UIButton);
        NiRegisterStream(UIButtonToggle);
		NiRegisterStream(UIContainer);
		NiRegisterStream(UIScrollList);
		NiRegisterStream(UIList);
		NiRegisterStream(UIScrollPage);
		NiRegisterStream(UIAnimation);
		NiRegisterStream(UITextInputField);
		NiRegisterStream(UIEditBox);
		
    }
    
    void TXMainSDM::ShutDown()
    {
        NiUnregisterStream(UIPicture);
        NiUnregisterStream(UILabel);
		NiUnregisterStream(UILabelAtlas);
        NiUnregisterStream(UILayout); 
        NiUnregisterStream(UIButton);
        NiUnregisterStream(UIButtonToggle);
		NiUnregisterStream(UIContainer);
		NiUnregisterStream(UIScrollList);
		NiUnregisterStream(UIList);
		NiUnregisterStream(UIScrollPage);
		NiUnregisterStream(UIAnimation);
		NiUnregisterStream(UITextInputField);
		NiUnregisterStream(UIEditBox);

		UIManager::Destroy();
		DragReceiverRegister::Destroy();
		if (ms_resourceheader)
		{
			delete ms_resourceheader;
		}
    }
    

    
    /**
     * @brief 创建一个UI图层
     * @param SceneType, 
     * @param pCustomScene, 父图层
     * @return UILayerBase, UILayer的对象
     * @
     */     
    UILayerBase* UICreator::CreateUILayer(SceneType _sceneType, CustomScene* pCustomScene)
    {
        UILayerBase* pBaseLayer = NULL;
        
        // Create Normal UILayer
        
        
        return pBaseLayer;
    }   
    
            
    UILayout*  UICreator::CreateUILayoutFromFile(const char* file, cocos2d::CCNode *pParent,
                                                     bool bShow )
    {
        // Load plist layout file
        UILayout* pLayer = NULL;    
        NiStream * pkStream = new NiStream();
        pLayer = (UILayout*)(pkStream->CreateControlFromFile(file, pParent));     
        delete pkStream;

		if(NULL == pLayer)
		{
			return NULL;
		}

		// update containers reference object
		std::list<UIControlBase*> childContainers = pLayer->getAllChildEndsWithName("Container");
		for(std::list<UIControlBase*>::iterator it = childContainers.begin();
			it != childContainers.end(); it++)
		{
			if(dynamic_cast<UIContainer*>(*it) != NULL)
			{
				UIContainer *tmp = static_cast<UIContainer*>(*it);

				if(tmp->getRefMode() == OBJECT_REFERENCE)
				{
					std::string refObjName = tmp->getRefObjName();
					UIControlBase *refObj = pLayer->FindChildObjectByName(refObjName);
					if(refObj != NULL && dynamic_cast<UIContainer*>(refObj) != NULL)
					{
						tmp->setRefObj(static_cast<UIContainer*>(refObj));
					}
				}
			}
		}

		// update scroll pages
		std::list<UIControlBase*> childScrollPages = pLayer->getAllChildEndsWithName("ScrollPage");
		for(std::list<UIControlBase*>::iterator it = childScrollPages.begin();
			it != childScrollPages.end(); it++)
		{
			if(dynamic_cast<UIScrollPage*>(*it) != NULL)
			{
				UIScrollPage *tmp = static_cast<UIScrollPage*>(*it);
				tmp->addPagesByChildren();
			}
		}

		float scaleFactor = UIManager::sharedManager()->getScaleFactor();
		//float scaleFactorX = UIManager::sharedManager()->getScaleFactorX();
		//float scaleFactorY = UIManager::sharedManager()->getScaleFactorY();
		pLayer->setScale(scaleFactor);
		//pLayer->setScale(scaleFactorX, scaleFactorY);

        return pLayer;
    }
    
	UILayout*  UICreator::CreateUILayoutFromFile(const char* file, bool bShow )
	{
		 cocos2d::CCNode *pParent = UIManager::sharedManager()->getUIDefaultRoot();
		 if (pParent)
		 {
			 return CreateUILayoutFromFile(file, pParent, bShow);
		 }else{
			 return NULL;
		 }
	}
    
    /**
     * @brief 解析resource.h，从中获取给定资源名字的编码值 
     * @param name, 资源名
     * @return int, 资源编码值
     * @
     */ 
    int getResourceIDByName(const char* name)
    {
        // to do...
        if (NULL == ms_resourceheader)
        {
            ms_resourceheader = new ResourceDotHFile("Data/resources.xml");
        }  
        
        return ms_resourceheader->GetValue(name);
    }

	void resetReloadResourceID()
	{
		static bool bLoad = false;
		if (NULL != ms_resourceheader && false == bLoad)
		{
			delete ms_resourceheader;
			ms_resourceheader = new ResourceDotHFile("Data/resources.xml");
			bLoad = true;
		}  		
	}
}