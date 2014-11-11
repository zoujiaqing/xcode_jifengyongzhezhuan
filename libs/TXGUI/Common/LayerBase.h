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

#ifndef iSkate_LayerBase_h
#define iSkate_LayerBase_h

#include "TXGUI.h"
#include "cocos2d.h"

namespace TXGUI {
        USING_NS_CC;
    class CustomScene;
    
    
    class LayerBase : public cocos2d::CCLayer
    {
    public:
        LayerBase();
        virtual ~LayerBase();
        virtual bool init();
        virtual void unInit();
        
        void setParentScene(CustomScene* parent);
        CustomScene* getParentScene() {return _parentScene;}
        
    protected:
        virtual void onInit();
        virtual void onUnInit();
               
    private:
        CustomScene* _parentScene;
        
    };
    
    class ColorLayerBase : public cocos2d::CCLayerColor {
    public:
        ColorLayerBase();
        virtual ~ColorLayerBase();
        virtual bool init();
        virtual void unInit();
        
        void setParentScene(CustomScene* parent);
        CustomScene* getParentScene() {return _parentScene;}
        
    protected:
        virtual void onInit();
        virtual void onUnInit();
        
        virtual cocos2d::ccColor4B getLayerColor();
    private:
        CustomScene* _parentScene;
        
    };   

}

#endif
