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

#ifndef iSkate_CustomScene_h
#define iSkate_CustomScene_h


#include "TXGUI.h"
#include "ObserverProtocol.h"
#include "cocos2d.h"


namespace TXGUI {
    
    class UILayerBase;

    class ModelLayerDelegate;

    
    class CustomScene : public cocos2d::CCScene, public ObserverProtocol
    {
    public:
        CustomScene(SceneType type, bool schedule = false);
        virtual ~CustomScene();
        
        SceneType sceneType() {
            return _sceneType;
        }
        
        bool init();
        void unInit();
        void assemble();
        
        void RegisterMsgObserver(int messageId);
        virtual void onBroadcastMessage(BroadcastMessage*msg);
        virtual void update(float deltaTime);
        
    protected:
        void doAssembleUI();
        void UnRegisterAllMsgObserver();
        
        virtual void onInit();
        virtual void onUnInit();
        virtual void onAssemble();
        
        
        
        UILayerBase* getUILayer() {return _uiLayer;}
        
        
    private:
        SceneType _sceneType;
        UILayerBase* _uiLayer;

        cocos2d::CCLayer*_uiContainer;
        bool _shouldSchedule;
        
        std::set<int> _msgsObserver;      
        
    };

}

#endif
