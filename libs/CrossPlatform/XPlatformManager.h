//
//  PlatformManager.h
//  iSeer
//
//  Created by b a on 12-5-11.
//  Copyright (c) 2012Äê __MyCompanyName__. All rights reserved.
//  @Author: jameshuang
//  @Date: 2012/5/12


#ifndef iSeer_Platform_h
#define iSeer_Platform_h
#include "IPlatform.h"
#include "IPUIPlatform.h"
#include "IExResourceMgr.h"

class XPlatformManager;


///
// class PlatformManager, create platform's device
//
class XPlatformManager 
{
public:   
    
    static XPlatformManager* m_sPlatformMgr; 
    
    // Init     
    static void _SMInit();
    
    // Shutdown
    static void _SMShutDown();
    
    static IPlatform* GetPlatformDevice();
    
    static IPUIPlatform *GetPlatformUI();

	static void SetExternalResourceMgr(IExResourceMgr * pResrouceMgr);
    
    ///
    // InitDevice
    // Init current platform's device, include audio device and vedio device, input device
    // 
    void InitDevice();
    
protected:
    
    XPlatformManager ();
    
    ~XPlatformManager();

    
    // Platform instance
    IPlatform * m_pPlatform;  
    
    //ui instance
    IPUIPlatform *m_pUIPlatform;

	// external resource manager
	IExResourceMgr * m_pResourceMgr;
};




#endif
