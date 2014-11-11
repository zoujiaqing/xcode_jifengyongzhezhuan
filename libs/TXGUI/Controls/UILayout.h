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

#ifndef iSeer_UILayerOut_h
#define iSeer_UILayerOut_h

#include "TXGUI.h"
#include "UILayerColorBase.h"
#include "UIControlBase.h"

namespace TXGUI {
    USING_NS_CC;
    
    class UILayout :  public UIControlBase
    {   
        NiDeclareRTTI;
        NiDeclareStream;   
        
        friend class UIManager;
        
        typedef struct stMenuHandler
        {
            SEL_MenuHandler pfn;
            CCObject* pSelect;
        }MenuHandler;
        
    public:
        UILayout(cocos2d::CCNode* parent);
		UILayout(std::string name, int tag, float z, UIControlBase *parent);
		UILayout(std::string name, int tag, float z, UIControlBase *parent, cocos2d::ccColor4B color);
        virtual ~UILayout();
                
        virtual void setAudioEffect(int iAudio){ m_iAudioEffect = iAudio;}
        
        virtual CCNode* getBindNode();
        
        virtual void setPosition(CCPoint pt);
        
        /**
         * @brief 注册一个MenuHandler
         * @param SEL_MenuHandler, pfn
         * @return void
         * @
         */
        virtual void RegisterMenuHandler(string name, SEL_MenuHandler pfn, CCObject* pSelect);
        
        /**
         * @brief 反注册一个MenuHandler
         * @param SEL_MenuHandler, pfn
         * @return void
         * @
         */
        virtual void UnregisterMenuHandler(string name);

		virtual void UnregisterAllMenuHandler();
        
		/// show /hide ui layeout
		virtual void setVisible(bool visible);
        /// show /hide the ui layout in animation 
		virtual void setVisibleInStyle(bool visible);
        
		virtual void EnableModeless(bool bModelLayer);
        virtual void SetNewPriority(int priority);
    protected:
        UILayout();
        
        /**
         * @brief 调用MenuHandler
         * @param namepfn, Function's Name with class name prefix
         * @param pObject, node to owner of the menu handler
         * @return void
         * @
         */
        void CallMenuHandler(string namepfn, CCObject* pObject);
        
    protected:
        int m_iAudioEffect;
        UILayerColorBase * m_pLayer;

		int m_touchPriority;
        
        // menu callback 
        std::map<string, MenuHandler> m_MenuCallback;
    };
}


#endif
