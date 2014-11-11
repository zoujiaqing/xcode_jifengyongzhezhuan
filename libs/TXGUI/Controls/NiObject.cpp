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
#include "NiObject.h"
#include "NiStream.h"

namespace TXGUI {
    
    USING_NS_CC;
    
    NiImplementRootRTTI(NiObject);
    
    NiObject::NiObject()
    {
        
    }
    
    ///------------------------------------------------------------------------
    //  streaming
    ///------------------------------------------------------------------------
    
    void NiObject::LoadBinary(NiStream &kStream)
    {
        
    }
    
    void NiObject::LinkObject(NiStream &kStream)
    {
        
    }
    
    bool NiObject::RegisterStreamables(NiStream &kStream)
    {
        return kStream.RegisterSaveObject(this);
    }
    
    void NiObject::SaveBinary(NiStream &kStream)
    {
        
    }
    
    bool NiObject::IsEqual(NiObject *pObject)
    {
        if (! pObject)
            return false;
        
        if (strcmp(GetRTTI()->GetName(), pObject->GetRTTI()->GetName()) != 0)
            return false;
        
        return true;
    }   
    
}