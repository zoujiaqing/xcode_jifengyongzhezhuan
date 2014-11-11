//
//  IPUIPlatform.h
//  iSeer
//
//  Created by razer tong on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_IPUIPlatform_h
#define iSeer_IPUIPlatform_h

class IPUIPlatform
{
public:
    
    virtual ~IPUIPlatform() {}
    
    ///
    //  InitPlatform
    //  init all the devices that needed!
    //
    virtual void InitPlatform() = 0;
    
    
    ///
    //  ShutDown
    //  clean up all context used  
    //
    virtual void ShutDown() = 0;
   
    //used by CreatePlayerLayer.cpp
    //创建角色时增加text框
    virtual void addCreatePlayerWithTextField()=0;
    
    //get the value of textfiled
    virtual const char *getPlayerName()=0;
    
    //hide textfiled
    virtual void hidePlayerTextField()=0;
    
    //show textfield
    virtual void showPlayerTextField()=0;
    
    //remove textfield from view
    virtual void removePlayerTextField()=0;
    
    
    //used by AssignExpLayer.mm
    //经验分配器输入框
    virtual void showInitInput(float x, float y, float w, float h) =0;
    
    virtual void hideInitInput() =0;
    
    virtual void removeInitInput() =0;
    
    virtual int getInputOffsetY() =0;
    
    virtual void setInputFrame(float x, float y, float w, float h) =0;
    
    virtual int getInputNum()=0;
    
    virtual void setMaxExp(int num)=0;
    
    
    //used by SceneIntro.cpp
    virtual void playVideo(const char* name,int tag)=0;
    
    virtual void addActicityIndicator() =0;
    
    virtual void removeActicityIndicator()=0;
    
    ///  Taomee account manager BEGIN
    virtual void  onTaomeeAccountManagerTouched() =0;
    
    virtual void onTaomeeMoreTouched()=0;
    
    virtual void addTaomeeAdvertise()=0;
    
    virtual void setAdvertiseVisible(bool bVisible)=0;
};

#endif
