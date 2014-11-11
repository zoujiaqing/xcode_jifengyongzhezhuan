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

#ifndef iSeer_TXAudioEngine_h
#define iSeer_TXAudioEngine_h

namespace TXGUI {
    
    class IAudioEffect
    {
    public:
        virtual void PlayAudio (int iAudioEffect) = 0;
        
        virtual void StopAudio (int iAudioEffect) = 0;
         
        virtual void PauseAudio(int iAudioEffect) = 0;
    };
    
    class TXAudioEngine
    {        
    public:
        TXAudioEngine();
        
        static TXAudioEngine* Instance();
        
        virtual void PlayAudio (int iAudioEffect);
        
        virtual void StopAudio (int iAudioEffect);
        
        virtual void PauseAudio(int iAudioEffect);
        
    protected:
        IAudioEffect * m_pAudio;
    };
}


#endif
