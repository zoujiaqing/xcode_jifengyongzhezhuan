//
//  XPlatform.h
//  iSeer
//
//  Created by b a on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_XAndroidPlatform_h
#define iSeer_XAndroidPlatform_h

#include <map>
#include <string>
#include "IPlatform.h"

class IExResourceMgr;


class XWin32Platform : public IPlatform
{
public:
    XWin32Platform(IExResourceMgr * pResourceMgr);
    
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
    
    
    ///
    //  @prototype, playAudioEffect
    //
    //  @param 
    //         name,  audio file's name
    //  @param
    //         delay, delay a time to play sound
    //
	void PlayAudioEffect(const char*name, float delay);

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
    
    ///
    //  @prototype, getCurLanguage
    //
    //  @param 
    //         void
    //  @note, get current lanaguage from the device
    int getCurLanguage();
    
    // 获取设备的UUID
    const char *getDeviceUUID();
    
    //把0.0.1.6 游戏版本转换为16进制
    unsigned int getVersion();
    
    //获取info.plist中的bundle version
    //格式0.0.1.6
    const char *getVersionA();
    
    ///
    //  @prototype, getSystemVersion
    //  @param 
    //         void
    //获取ios操作系统的version
    float getSystemVersion();
    
    
    //判断设备的网络链接状态
    bool IsConnected();
    
    
    //判断设备是iphone　or　ipad
    UIUserInterfaceType getUserInterface();
    
    
    bool IsRetinaDisplay();
    
    void initUserInterface();
    
    bool IsIpad();
    
    const char *CCLocalizedString(const char *key,const char *comment);
    
    //store password for user
    const char* getPasswordForUsername(const char* username,const char* serviceName) ;
    
    //get user's password
    bool storeUsername(const char* username ,const char* password,const char* serviceName ,bool updateExisting ) ;
    
protected:
    
    std::map<std::string, std::string> m_localizableStringMap;

	/// External resource manager
	IExResourceMgr * m_pResourceMgr;
};

IPlatform* CreateXPlatform(IExResourceMgr * pResrouceMgr);


#endif
