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

#ifndef iSkate_ObserverProtocol_h
#define iSkate_ObserverProtocol_h

#include <string>

namespace TXGUI{
    
    typedef struct stBroadcastMessage
    {
        int message;
        void* sender;
        std::string param1;
        std::string param2;
    }BroadcastMessage;

    
    class ObserverProtocol
    {
        friend class NotificationCenter;
    public:
        virtual void onBroadcastMessage(BroadcastMessage*msg) = 0;
    };
}


#endif
