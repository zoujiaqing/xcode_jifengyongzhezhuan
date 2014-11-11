//
//  device-c-interface.h
//  idou
//
//  Created by razer tong on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef idou_device_c_interface_h
#define idou_device_c_interface_h


#include "cocos2d.h"
#include <iostream>



//主要为了把解密后的字符串转换为ccdictionary
//cocos2d::CCDictionary<std::string, cocos2d::CCObject*> *dictionaryWithString(const char *pString);

//国际化
//const char *CCLocalizedString(const char *key,const char *comment);

void c_performFunc();


//used by CreatePlayerLayer.cpp
//创建角色时增加text框
//void addCreatePlayerWithTextField();
//const char *getPlayerName();
//void hidePlayerTextField();
//void showPlayerTextField();
//void removePlayerTextField();


//used by AssignExpLayer.mm
//经验分配器输入框
//void showInitInput(float x, float y, float w, float h);
//void hideInitInput();
//void removeInitInput();
//int getInputOffsetY();
//void setInputFrame(float x, float y, float w, float h);
//int getInputNum();
//void setMaxExp(int num);


//used by SceneIntro.cpp
//void playVideo(const char* name,int tag);

//used by GameAudioManager.cpp
//void playEffectWithDealy(const char*name,float delay);

//kechain
//const char* getPasswordForUsername(const char* username,const char* serviceName);
//
//bool storeUsername(const char* username ,const char* password,const char* serviceName ,bool updateExisting );



extern int m_cmd;


#endif
