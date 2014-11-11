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

#ifndef iSeer_UIPicture_h
#define iSeer_UIPicture_h

#include "TXGUI.h"
#include "UIControlBase.h"
#include "NiStream.h"

namespace TXGUI {
    USING_NS_CC;
    
    
    class UIPicture : public UIControlBase
    {           
        NiDeclareRTTI;
        NiDeclareStream;   
        
    public:
        UIPicture(CCSprite* pSprite, cocos2d::CCNode* pParentNode);
		UIPicture(std::string name, int tag, float z, UIControlBase *parent, CCSprite *sprite);
        virtual~UIPicture();
        
        // position        
		virtual void setPosition(CCPoint pt) ;

		virtual void setUseStretch(bool useStretch);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual void setInitScale(float scale);

		virtual void setSprite(CCSprite *pSprite,bool isMerge = true);
        void setMySprite(CCSprite* pSprite);

		virtual CCPoint getSpritePos();

		virtual CCSize getSpriteSize();

		virtual void addExtraPic(CCSprite *sprite,CCPoint pt = CCPointZero);
    protected:
        // for streaming
        UIPicture();

		CCNode*  MergeSmallASprite(CCSprite* sprite);
        
    protected:
        CCNode * m_pSpriteNode;
		bool m_bUseStretch;
		float m_fInitScale;
    };
}



#endif
