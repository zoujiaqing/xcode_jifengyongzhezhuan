//
//  CrossPlatform.h
//  iSeer
//
//  Created by razer tong on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_CrossPlatform_h
#define iSeer_CrossPlatform_h

#include "IPlatform.h"
//////////////////////////////////////////////////////////////////////////////////////////
//                                   Platform API
//////////////////////////////////////////////////////////////////////////////////////////

///
//  @prototype, getCurLanguage
//
//  @param 
//         void
//  @note, get current lanaguage from the device
int getCurLanguage();

///
//  @prototype, UTF8Length
//
//  @param
//         str,   source string
//
int UTF8Length(const char* str);

///
//  @prototype, UTF8SubString
//
//  @param  inStr,  source string
//  @param  length, sub-string length
//  @param  outStr, destination string
void UTF8SubString(const char* inStr, int length, char* outStr);

// 获取设备的UUID
const char *getDeviceUUID();

//把0.0.1.6 游戏版本转换为16进制
unsigned int getVersion();

//获取info.plist中的bundle version
//格式0.0.1.6
const char *getVersionA();

//获取ios操作系统的version
float getSystemVersion();

//设备判断
bool IsIpad();

//判断设备的网络链接状态
bool IsConnected();


//判断设备是iphone　or　ipad
UIUserInterfaceType getUserInterface();


bool IsRetinaDisplay();

//国际化
const char *CCLocalizedString(const char *key,const char *comment);

const char* getPasswordForUsername(const char* username,const char* serviceName);

bool storeUsername(const char* username ,const char* password,const char* serviceName ,bool updateExisting );

/////////////////////////////////////////////////////////////////////////////////////////////
//Platform UI//
/////////////////////////////////////////////////////////////////////////////////////////////

//play mp4
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

void  onTaomeeAccountManagerTouched();

void onTaomeeMoreTouched();

void addTaomeeAdvertise();

void setAdvertiseVisible(bool bVisible);

#endif
