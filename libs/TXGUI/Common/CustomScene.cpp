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

#include "CustomScene.h"
#include "UILayerBase.h"
#include "NotificationCenter.h"
#include "ModelLayer.h"
#include "UICreator.h"

namespace TXGUI 
{
    
    CustomScene::CustomScene(SceneType type, bool schedule) 
    : _sceneType(type)
    , _uiLayer(NULL)
    , _uiContainer(NULL)
    , _shouldSchedule(schedule) 
    {
        _uiContainer = cocos2d::CCLayer::create();
        this->addChild(_uiContainer, UI_Layer, UI_Tag);
        if (_shouldSchedule) {
            this->scheduleUpdate();
        }
    }
    
    CustomScene::~CustomScene() {
        unInit();
    }
     
    bool CustomScene::init() {
        _uiLayer = NULL;
        
        onInit();
        
        return true;
    }
    
    void CustomScene::unInit() {
        onUnInit();
        
       
        if (_uiLayer != NULL) {
            _uiLayer->unInit();
            _uiLayer = NULL;
        }
       
        if (_shouldSchedule) {
            this->unscheduleUpdate();
            _shouldSchedule = false;
        }
        
        UnRegisterAllMsgObserver();
        
        this->removeAllChildrenWithCleanup(true);
        
        
    }
    
    void CustomScene::assemble() {
        doAssembleUI();
        
        onAssemble();   
        
    }
    
    
    void CustomScene::doAssembleUI() 
    {
        UILayerBase* layer = UICreator::CreateUILayer(_sceneType, this);
       
        if (layer == NULL)
            return;
        
        _uiLayer = layer;
        if (_uiContainer != NULL)
            _uiContainer->addChild((cocos2d::CCNode*)_uiLayer);
    }
    
    void CustomScene::RegisterMsgObserver(int messageId) {
        if (_msgsObserver.end() == _msgsObserver.find(messageId)) {
            _msgsObserver.insert(messageId);
            
            NotificationCenter::defaultCenter()->registerMsgObserver(this, messageId);
        }
    }
    
    void CustomScene::UnRegisterAllMsgObserver() {
        if (_msgsObserver.size() > 0) {
            _msgsObserver.clear();
            NotificationCenter::defaultCenter()->unregisterMsgObserver(this);  
        }
    }
    
    void CustomScene::update(float deltaTime) {
        
    }
    
    void CustomScene::onBroadcastMessage(BroadcastMessage* msg) {
        if (_uiLayer != NULL)
            _uiLayer->onBroadcastMessage(msg);
    }
    
    void CustomScene::onInit() {
        
    }    
    
    void CustomScene::onUnInit() {
        
    }

    void CustomScene::onAssemble() {
        
    }
}


