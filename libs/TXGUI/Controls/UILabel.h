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
#ifndef iSeer_UILabel_h
#define iSeer_UILabel_h

#include "TXGUI.h"
#include "UILayerColorBase.h"
#include "UIControlBase.h"
#include "UILabelTTF.h"

namespace TXGUI {
    USING_NS_CC;
    
    class UILabel : public UIControlBase
    {       
        NiDeclareRTTI;
        NiDeclareStream;  
    public:
        UILabel(const char *label,
                const char *fontName,
                float fontSize, 
                cocos2d::CCNode* parent);

		UILabel(UILabelTTF *pLabel, cocos2d::CCNode *parent);

		UILabel(std::string name, int tag, float z, UIControlBase *parent, UILabelTTF *label);
        
        virtual   ~UILabel();
        
        void setString(const char *label);
        
        const char* getString();
        
        virtual void setPosition(CCPoint pt) ;

		virtual void setLabelTTF(UILabelTTF *pLabel);
		virtual UILabelTTF * getLabelTTF(){return m_pTTFLabel;}

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual	void setColor(const ccColor3B& color);
        
		virtual void setStringWithColor(const char *label,const ccColor3B& color);
    protected:
        UILabel();
        
    protected:        
        UILabelTTF *m_pTTFLabel;
    };
}


#endif
