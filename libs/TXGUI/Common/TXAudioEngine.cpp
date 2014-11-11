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
#include "TXAudioEngine.h"

namespace TXGUI {    
           
    static TXAudioEngine *ms_Instance = NULL;
    
    TXAudioEngine* TXAudioEngine::Instance()
    {
        if(ms_Instance == NULL)
        {
            ms_Instance = new TXAudioEngine();
        }
        return ms_Instance;
    }
    
    TXAudioEngine::TXAudioEngine()
    {
        m_pAudio = NULL;
    }
    
    void TXAudioEngine::PlayAudio(int iAudioEffect)
    {
        // to do ...
        if (m_pAudio)
        {
            m_pAudio->PlayAudio(iAudioEffect);
        }
    }
    
    void TXAudioEngine::StopAudio(int iAudioEffect)
    {
        // to do ...
        if (m_pAudio)
        {
            m_pAudio->StopAudio(iAudioEffect);
        }   
    }
    
    void TXAudioEngine::PauseAudio(int iAudioEffect)
    {
        // to do ...
        if (m_pAudio)
        {
            m_pAudio->PauseAudio(iAudioEffect);
        } 
    }
}