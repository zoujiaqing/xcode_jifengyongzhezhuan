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

#ifndef iSeer_UIToggle_h
#define iSeer_UIToggle_h

#include "TXGUI.h"
#include "UIControlBase.h"
#include "UIButton.h"

namespace TXGUI {
    USING_NS_CC;
    
    class UIButtonToggle : public UIButton 
    {       
        NiDeclareRTTI;
        NiDeclareStream;  
        
    public:
        UIButtonToggle(CCNode* parent);

		UIButtonToggle(std::string name, int tag, float z, UIControlBase *parent, CCMenuItemToggle *toggle);

		UIButtonToggle(
			std::string name,
			int tag,
			float z,
			UIControlBase *parent,
			CCMenuItemToggle *toggle,
			std::string clickedFunc
			);
        
        virtual   ~UIButtonToggle();

		virtual void setPosition(CCPoint point);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);
        
        CCMenuItemToggle* getCCXToggle() {
            return m_pToggle;
        }

		virtual void setMenuItemToggle(CCMenuItemToggle *pToggle);

		virtual void setClickedFunc(std::string func);
        
		virtual void BindNode(CCNode* pParentNode);

		virtual void selected();
		virtual void unselected();

		virtual CCSize getButtonSize();
    protected:
        UIButtonToggle();        
        
        virtual void  Clicked(CCObject* pObject);
        
    protected:        
        CCMenuItemToggle *m_pToggle;
    };
}

#endif
