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

#ifndef iSeer_NiStream_h
#define iSeer_NiStream_h

#include "TXGUI.h"
#include "NiStreamMacros.h"

#include <map>
#include <list>

namespace TXGUI {
    USING_NS_CC;
    
    class UIControlBase;
    class NiObject;

    
    ///
    // class NiStream
    //
    class NiStream
    {
    public:     
        NiStream();
        virtual  ~NiStream();
        
        cocos2d::ccColor3B convertToColor3B(uint32_t value);    
        
        /**
         * @brief 创建一个UI图层控件
         * @param const char * , 
         * @param cocos2d::CCNode*, 父图层
         * @return UIControlBase, UILayer的对象
         * @
         */        
        UIControlBase* CreateControlFromFile(const char * file, cocos2d::CCNode* parent);
        
        bool getIntAttributeValue(mutableDic* dic, 
                                  std::string key, 
                                  uint32_t& value);
        
		bool getSignedIntAttributeValue(mutableDic* dic, 
									std::string key, 
									int32_t& value);

        bool getStringattributeValue(mutableDic* dic,
                                     std::string key, 
                                     std::string& value);
        
        bool getDictionaryAttributeValue(mutableDic* dic,
                                         std::string key, 
                                         mutableDic*& value);
        
        bool getAttributePositive(mutableDic* dic,
                                  std::string key);
        
        bool RegisterSaveObject(NiObject* pkObject);
        
        mutableDic * GetStreamData();
        
        // Streaming operation
        typedef NiObject* (*createFunction)(void);
        static int RegisterLoader(const char* pcClassName, createFunction pfnFunc);
        static void UnregisterLoader(const char * pcClassName);
        
    protected:  
        
        UIControlBase* parseControls(cocos2d::CCNode* parent, mutableDic* dic);
        
        virtual void internalCreateControl(UIControlBase** pControl, CCObject* pData); 
        
    protected:
        std::string m_strFile;
        static std::map<int, UIControlBase*> _controlsDic;
        
        std::map<const NiObject*, unsigned int>m_kRegisterMap;
        std::list<NiObject*> m_kObjects;
        
        UIControlBase* m_pControl;
        CCPoint m_ptPosition;
        cocos2d::CCNode* _ParentNode;
        std::string m_strName;
        
        mutableDic * m_pCurrentLinkedDic;
        
        // Creators
        static std::map<string, createFunction>ms_kLoaders;
    };   
    
}

#endif
