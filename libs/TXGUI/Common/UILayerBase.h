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

#ifndef iSkate_UILayerBase_h
#define iSkate_UILayerBase_h

#include "ObserverProtocol.h"
#include "LayerBase.h"



namespace TXGUI{
    
    class ModelLayerDelegate;

    
    class UILayerBase : public LayerBase
    {
    public:
        UILayerBase(std::string fileName, bool touchEnable, bool schedule);
        virtual ~UILayerBase();
        
        virtual void onBroadcastMessage(BroadcastMessage*msg);
        virtual void notifyUIMsg(UILayerMsgId msgId, uint32_t param);
        
        void clearAllChildren();
        void showLayer(bool show);
                            
    protected:
        virtual void onInit();
        virtual void onUnInit();
        
    protected:
        virtual void update(float deltaTime);
        virtual void onItemClickCallback(int tagId);
        virtual void onItemValueChanged(int tagId);
    };   
    
}


#endif

