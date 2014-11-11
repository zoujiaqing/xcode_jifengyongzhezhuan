//
//  PlatformManager.cpp
//  iSeer
//
//  Created by b a on 12-5-11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
//  @Author: jameshuang
//  @Date: 2012/5/12
#include <string.h>
#include <stdio.h>
#include "XPlatformManager.h"

#ifndef WIN32
#include "AndroidPlatform/XPlatform.h"          //by Stanley
#include "AndroidPlatform/XUIPlatform.h"
#else
#include "Win32Platform/XPlatform.h"
#include "Win32Platform/XUIPlatform.h"
#endif


  
XPlatformManager* XPlatformManager::m_sPlatformMgr = NULL; 

XPlatformManager::XPlatformManager() : 
m_pPlatform(NULL),
m_pUIPlatform(NULL),
m_pResourceMgr(NULL)
{
    
}

XPlatformManager::~XPlatformManager() {
    delete this->m_pUIPlatform;
    this->m_pUIPlatform = NULL;

	m_pResourceMgr = NULL;
}

void XPlatformManager::_SMInit()
{    
    if (m_sPlatformMgr == NULL)
    {
        m_sPlatformMgr = new XPlatformManager();        
    }
}

void XPlatformManager::_SMShutDown()
{
	if (m_sPlatformMgr)
	{
		//clear XIOSPlatform
		if ( XPlatformManager::m_sPlatformMgr->m_pPlatform)
		{
			XPlatformManager::m_sPlatformMgr->m_pPlatform->ShutDown();
			delete XPlatformManager::m_sPlatformMgr->m_pPlatform;
			XPlatformManager::m_sPlatformMgr->m_pPlatform = NULL;
		}

		//clear CreateXUIPlatform
		if ( XPlatformManager::m_sPlatformMgr->m_pUIPlatform)
		{
			XPlatformManager::m_sPlatformMgr->m_pUIPlatform->ShutDown();
			delete XPlatformManager::m_sPlatformMgr->m_pUIPlatform;
			XPlatformManager::m_sPlatformMgr->m_pUIPlatform = NULL;
		}

		delete m_sPlatformMgr;
		m_sPlatformMgr = NULL; 
	}   
}

IPlatform* XPlatformManager::GetPlatformDevice()
{   
    if (XPlatformManager::m_sPlatformMgr == NULL)
        return NULL;
    
    return XPlatformManager::m_sPlatformMgr->m_pPlatform;
}

IPUIPlatform *XPlatformManager::GetPlatformUI()
{
    if (XPlatformManager::m_sPlatformMgr == NULL)
        return NULL;

    return XPlatformManager::m_sPlatformMgr->m_pUIPlatform;
}

void XPlatformManager::SetExternalResourceMgr(IExResourceMgr * pResrouceMgr)
{
	if (XPlatformManager::m_sPlatformMgr == NULL)
		return ;

	XPlatformManager::m_sPlatformMgr->m_pResourceMgr = pResrouceMgr;
}

///
//  void InitDevice()
//  create device from current platform configuration
//
void XPlatformManager::InitDevice()
{    
    //by Stanley
     m_pPlatform =  CreateXPlatform(m_pResourceMgr);    
    
    //create IOS UI Platform
    m_pUIPlatform = CreateXUIPlatform();

    m_pPlatform->initUserInterface();
}


//////////////////////////////////////////////////////////////////////////////////////////
//                                   Platform API
//////////////////////////////////////////////////////////////////////////////////////////
//  @prototype, getCurLanguage
//
//  @param 
//         void
//  @note, get current lanaguage from the device
int getCurLanguage()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getCurLanguage();
}

///
//  @prototype, UTF8Length
//
//  @param
//         str,   source string
//
int UTF8Length(const char* str)
{
	IPlatform* platform = XPlatformManager::GetPlatformDevice();
	return platform->UTF8Length(str);
}

///
//  @prototype, UTF8SubString
//
//  @param  inStr,  source string
//  @param  length, sub-string length
//  @param  outStr, destination string
void UTF8SubString(const char* inStr, int length, char* outStr)
{
	IPlatform* platform = XPlatformManager::GetPlatformDevice();
	platform->UTF8SubString(inStr, length, outStr);
}

// 获取设备的UUID
const char *getDeviceUUID()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getDeviceUUID();
}

//把0.0.1.6 游戏版本转换为16进制
unsigned int getVersion()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getVersion();
}

//获取info.plist中的bundle version
//格式0.0.1.6
const char *getVersionA()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getVersionA();    
}

//获取ios操作系统的version
float getSystemVersion()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getSystemVersion();  
}

//判断设备的网络链接状态
bool IsConnected()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->IsConnected();   
}


//判断设备是iphone　or　ipad
UIUserInterfaceType getUserInterface()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getUserInterface();   
}


bool IsRetinaDisplay()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->IsRetinaDisplay();     
}

//设备判断
bool IsIpad()
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    if (platform == NULL)
    {
        return false;
    }
    return platform->IsIpad();   
}


const char *CCLocalizedString(const char *key,const char *comment)
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->CCLocalizedString(key, comment);
}

const char* getPasswordForUsername(const char* username,const char* serviceName)
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return platform->getPasswordForUsername(username, serviceName);
}

bool storeUsername(const char* username ,const char* password,const char* serviceName ,bool updateExisting )
{
    IPlatform* platform = XPlatformManager::GetPlatformDevice();
    return  platform->storeUsername(username, password, serviceName, updateExisting);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Platform UI//
/////////////////////////////////////////////////////////////////////////////////////////////
//play mp4
void playVideo(const char* name,int tag)
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->playVideo(name, tag);
}

void addCreatePlayerWithTextField()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->addCreatePlayerWithTextField();
    
}

const char *getPlayerName()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    return platform->getPlayerName();
}

void hidePlayerTextField()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->hidePlayerTextField();
}

void showPlayerTextField()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->showPlayerTextField();
}

void removePlayerTextField()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->removePlayerTextField();
}

void showInitInput(float x, float y, float w, float h)
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->showInitInput(x,y, w, h);
}

void hideInitInput()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->hideInitInput();
}

void removeInitInput()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->removeInitInput();
}

int getInputOffsetY()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    return platform->getInputOffsetY();
}

void setInputFrame(float x, float y, float w, float h)
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->setInputFrame(x, y, w, h);
}

int getInputNum()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    return platform->getInputNum();
}

void setMaxExp(int num)
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->setMaxExp(num);
}

void addActicityIndicator() 
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->addActicityIndicator();
}

void removeActicityIndicator()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->removeActicityIndicator();
}

void  onTaomeeAccountManagerTouched()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->onTaomeeAccountManagerTouched();
}

void onTaomeeMoreTouched()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->onTaomeeMoreTouched();
}

void addTaomeeAdvertise()
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->addTaomeeAdvertise();
}

void setAdvertiseVisible(bool bVisible)
{
    IPUIPlatform *platform = XPlatformManager::GetPlatformUI();
    platform->setAdvertiseVisible(bVisible);
}