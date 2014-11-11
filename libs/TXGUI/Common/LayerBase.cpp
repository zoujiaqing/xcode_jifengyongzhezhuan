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

#include "LayerBase.h"
#include "CustomScene.h"


namespace TXGUI {
    USING_NS_CC;
    LayerBase::LayerBase() {
        
    }
    
    LayerBase::~LayerBase() {
        
    }
    
    bool LayerBase::init() {
        CCLayer::init();
        
        onInit();
        return true;
    }
    
    void LayerBase::unInit() {
        onUnInit();
        
        this->removeAllChildrenWithCleanup(true);
        this->removeFromParentAndCleanup(true);
    }
    
    void LayerBase::onInit() {
        
    }
    
    void LayerBase::onUnInit() {
        
    }
      
    void LayerBase::setParentScene(CustomScene* parent) {
        _parentScene = parent;
    }
    
    
    // ColorLayerBase 
    ColorLayerBase::ColorLayerBase() {
        
    }
    
    ColorLayerBase::~ColorLayerBase() {
        
    }
    
    bool ColorLayerBase::init() {
        ColorLayerBase::initWithColor(getLayerColor());
        
        onInit();
        return true;
    }
    
    void ColorLayerBase::unInit() {
        onUnInit();
        
        this->removeAllChildrenWithCleanup(true);
        this->removeFromParentAndCleanup(true);
    }
    
    void ColorLayerBase::onInit() {
        
    }
    
    void ColorLayerBase::onUnInit() {
        
    }
    
    void ColorLayerBase::setParentScene(CustomScene* parent) {
        _parentScene = parent;
    }
       
    cocos2d::ccColor4B ColorLayerBase::getLayerColor() {
        return cocos2d::ccc4(0, 0, 0, 255);
    }    
}

