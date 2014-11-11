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

#ifndef iSeer_NiObject_h
#define iSeer_NiObject_h

#include "TXGUI.h"
#include "NiRTTI.h"

#include "NiStream.h"

#include <list>
namespace TXGUI {
     
    USING_NS_CC;
    
    class NiObject : public CCObject
    {
        NiDeclareRootRTTI(NiObject);
        NiDeclareAbstractStream;
        
    public:
        //for streaming
        NiObject();
        
    };
    
}

#endif
