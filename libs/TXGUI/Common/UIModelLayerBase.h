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

#ifndef iSeer_Header_h
#define iSeer_Header_h

#include "UILayerColorBase.h"

namespace TXGUI{
    class ModelLayerDelegate;
    
    class UIModelLayerBase : public UILayerColorBase 
    {
    public:
        UIModelLayerBase(std::string fileName, bool touchEnable = true, 
                         bool schedule = false, ccColor4B color = ccc4(0, 0, 0, 128));
        
        virtual int getMenuPriority() {
            return MODEL_MENU_PRIORITY;
        }
        
        void setModelDelegate(ModelLayerDelegate* delegate) {
            _modelLayerDelegate = delegate;
        }
        
        virtual void registerWithTouchDispatcher(void);
        
        virtual void onItemClickCallback(int tagId);
        virtual void onItemValueChanged(int tagId);
        
    protected:
        void hideModelView();
        
        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
        
        virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
        
    private:
        ModelLayerDelegate* _modelLayerDelegate;
    };
    
}



#endif
