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

#include <iostream>
#include "UIModelLayerBase.h"
#include "ModelLayer.h"

namespace TXGUI {
    
    // UIModelLayerBase members
    
    UIModelLayerBase::UIModelLayerBase(std::string fileName, bool touchEnable, 
                                       bool schedule, ccColor4B color) 
    : UILayerColorBase(fileName, touchEnable, schedule, color) {
        
    }
    
    void UIModelLayerBase::registerWithTouchDispatcher() {
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        pDispatcher->addTargetedDelegate(this, MODEL_LAYER_PRIORITY, true);
    }
    
    void UIModelLayerBase::onItemClickCallback(int tagId){
        if (_modelLayerDelegate != NULL)
            _modelLayerDelegate->onLayerMsg(tagId);
    }
    
    void UIModelLayerBase::onItemValueChanged(int tagId) {
        if (_modelLayerDelegate != NULL)
            _modelLayerDelegate->onLayerMsg(tagId);
    }
    
    void UIModelLayerBase::hideModelView() {
        ModelLayer::shareInstance()->hideModelLayer(this);
    }
    
    bool UIModelLayerBase::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
        return true;
    }
    
    void UIModelLayerBase::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
        return ;
    }
    
    void UIModelLayerBase::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
        return ;
    }
    
    void UIModelLayerBase::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
        return ;
    }
    
    void UIModelLayerBase::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
        return ;
    }
    
    void UIModelLayerBase::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
        return ;
    }
    
    void UIModelLayerBase::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
        return ;
    }
    
    void UIModelLayerBase::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent) {
        return ;
    }

}