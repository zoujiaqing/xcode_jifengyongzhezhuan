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

#include "UILayerBase.h"
#include "ModelLayer.h"


namespace TXGUI{
    
    UILayerBase::UILayerBase(std::string fileName, bool touchEnable, bool schedule)
    {
        setTouchEnabled(touchEnable);
        if (schedule)
            this->scheduleUpdate();
    }
    
    UILayerBase::~UILayerBase() {
        
    }
       
    void UILayerBase::onBroadcastMessage(BroadcastMessage* msg) {
        
    }
    
    void UILayerBase::notifyUIMsg(UILayerMsgId msgId, uint32_t param) {
        
    }
       
    void UILayerBase::clearAllChildren() {
        this->removeAllChildrenWithCleanup(true);
    }
    
    void UILayerBase::showLayer(bool show) {
        this->setVisible(show);
    }
    
    void UILayerBase::onInit() {
        LayerBase::onInit();        
    }
    
    void UILayerBase::onUnInit() {
        LayerBase::onUnInit();
    }
        
   
    void UILayerBase::update(float deltaTime) {
        
    }
    
    void UILayerBase::onItemClickCallback(int tagId) {
        
    }
    
    void UILayerBase::onItemValueChanged(int tagId) {
        
    }  
}

