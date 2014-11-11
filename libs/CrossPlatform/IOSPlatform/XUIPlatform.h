//
//  XIOSUIPlatform.h
//  iSeer
//
//  Created by razer tong on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_XIOSUIPlatform_h
#define iSeer_XIOSUIPlatform_h

#include "IPUIPlatform.h"

class XUIPlatform:public IPUIPlatform
{
public:
    XUIPlatform();
    
    ///
    //  InitPlatform
    //  init all the devices that needed!
    //
    void InitPlatform();
    
    ///
    //  ShutDown
    //  clean up all context used  
    //
    void ShutDown();
    
    void playVideo(const char* name,int tag);
    
    void addCreatePlayerWithTextField();
    
    const char *getPlayerName();
    
    void hidePlayerTextField();
    
    void showPlayerTextField();
    
    void removePlayerTextField();
    
    
    void showInitInput(float x, float y, float w, float h);
    
    void hideInitInput();
    
    void removeInitInput();
    
    int getInputOffsetY();
    
    void setInputFrame(float x, float y, float w, float h);
    
    int getInputNum();
    
    void setMaxExp(int num);
    
    
    void addActicityIndicator() ;
    
    void removeActicityIndicator();
    
    ///  Taomee account manager BEGIN
    void  onTaomeeAccountManagerTouched();
    
    void onTaomeeMoreTouched();
    
    void addTaomeeAdvertise();
    
    void setAdvertiseVisible(bool bVisible);
};
XUIPlatform* CreateXUIPlatform();
#endif
