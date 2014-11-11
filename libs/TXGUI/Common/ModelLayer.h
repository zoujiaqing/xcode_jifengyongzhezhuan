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

#ifndef iSkate_ModelScene_h
#define iSkate_ModelScene_h
#include "TXGUI.h"
#include "cocos2d.h"

namespace TXGUI {
    class UIModelLayerBase;
    
    class ModelLayerDelegate {
    public:
        virtual void onModelLayerShow() {};
        virtual void onModelLayerHide() {};
        virtual void onLayerMsg(int tagId) = 0;
    };
    
    
    class ModelLayer : public cocos2d::CCObject, public ModelLayerDelegate {
    
        ModelLayer();
        bool init();
        
    public:
        virtual ~ModelLayer();
        static ModelLayer* shareInstance();
        static void pureModelLayer();
        
        bool showModelLayer(UIModelLayerBase* layer, ModelLayerDelegate* delegate);
        void hideModelLayer(UIModelLayerBase* layer);
        void hideModelLayer();
        
    protected:
        bool doShowModelLayer(UIModelLayerBase* layer, bool show);
        
        virtual void onModelLayerShow();
        virtual void onModelLayerHide();
        virtual void onLayerMsg(int tagId);      
    private:
        UIModelLayerBase*   _modelLayer;
        ModelLayerDelegate* _modelDelegate;
    };
}

#endif
