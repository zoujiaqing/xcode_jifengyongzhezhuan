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

#ifndef iSeer_Animation_h
#define iSeer_Animation_h


#include "TXGUI.h"
#include "UIControlBase.h"
#include "EnginePlayerManager.h"

namespace TXGUI {
    USING_NS_CC;
    
    class UIAnimation : public UIControlBase 
    {       
        NiDeclareRTTI;
        NiDeclareStream;  
    public:
        UIAnimation(cocos2d::CCNode* parent);

		UIAnimation(std::string name, int tag, float z, UIControlBase *parent, AnimSprite *anim, float initScale);
        
        virtual   ~UIAnimation();        
        
        virtual void setPosition(CCPoint pt);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual void setInitScale(float scale);

		virtual float getInitScale();

		virtual void setAnimSprite(AnimSprite *pAnimSprite);

		virtual AnimSprite* getAnimSprite();
        
    protected:
        UIAnimation();
        
    protected:        
        AnimSprite* m_pAnimSprite;
		float m_fInitScale;
    };
}

#endif
