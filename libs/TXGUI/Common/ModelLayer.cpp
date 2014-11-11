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
#include "UIModelLayerBase.h"


namespace TXGUI {
    static ModelLayer* s_modelLayer = NULL;
    
    
    ModelLayer::ModelLayer()
    : _modelLayer(NULL)
    , _modelDelegate(NULL) {
        
    }
    
    ModelLayer::~ModelLayer() {
    
    }
    
    ModelLayer* ModelLayer::shareInstance() {
        if (s_modelLayer == NULL) {
            s_modelLayer = new ModelLayer();
            s_modelLayer->init();
        }
        
        return s_modelLayer;
    }
    
    void ModelLayer::pureModelLayer() {
        if (s_modelLayer == NULL)
            return ;
        
        s_modelLayer->release();
    }
    
    bool ModelLayer::init() {
        
        return true;
    }
    
    bool ModelLayer::showModelLayer(UIModelLayerBase* layer, 
                                    ModelLayerDelegate* delegate) {
        if (layer == NULL)
            return false;
        
        hideModelLayer();
        
        if (!doShowModelLayer(layer, true)) {
            return false;
        }
        
        _modelDelegate = delegate;
        _modelLayer = layer;
        onModelLayerShow();
        
        return true;
    }
    
    void ModelLayer::hideModelLayer(UIModelLayerBase* layer) {
        if ((layer != NULL) && (layer == _modelLayer) )
            hideModelLayer();
    }
    
    void ModelLayer::hideModelLayer() {
        if (doShowModelLayer(_modelLayer, false)) {
            onModelLayerHide();
        }
    }
    
    bool ModelLayer::doShowModelLayer(UIModelLayerBase* layer, bool show) {
        cocos2d::CCScene* curScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
        if (curScene == NULL) 
            return false;
        
        if (show) {
            curScene->addChild(layer, Model_Layer, Model_Tag);
            layer->setModelDelegate(this);
        } else {
            if (layer == NULL) 
                return false;
            
            layer->setVisible(false);
            layer->setModelDelegate(NULL);
            curScene->removeChild(layer, true);
        }
        
        return true;
    }
    
    void ModelLayer::onModelLayerShow() {
        if (_modelDelegate != NULL) {
            _modelDelegate->onModelLayerShow();
        }
    }
    
    void ModelLayer::onModelLayerHide() {
        if (_modelDelegate != NULL) {
            _modelDelegate->onModelLayerHide();
            _modelDelegate = NULL;
        }
        _modelLayer = NULL;
    }
    
    void ModelLayer::onLayerMsg(int tagId) {
        if (_modelDelegate != NULL) {
            _modelDelegate->onLayerMsg(tagId);
        }
    }
}

