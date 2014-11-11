//
//  XIOSPlatform.cpp
//  iSeer
//
//  Created by b a on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#import "SFHFKeychainUtils.h"
#include <iostream>
#include "XPlatform.h"      //by Stanley
#include "CrossUtil.h"
#include "IOSUtil.h"
#import "TaomeeFeedback.h"

//by Stanley
IPlatform* CreateXPlatform(IExResourceMgr * pResrouceMgr)
{    
    IPlatform *pf = new XIOSPlatform(pResrouceMgr);
    return pf;
}


XIOSPlatform::XIOSPlatform(IExResourceMgr * pResrouceMgr)
{
    m_pResourceMgr = pResrouceMgr;
    InitPlatform();
}


///
//  InitPlatform
//  init all the devices that needed!
//
void XIOSPlatform::InitPlatform()
{
    
}

///
//  ShutDown
//  clean up all context used  
//
void XIOSPlatform::ShutDown()
{

}

///
//  @prototype, playAudioEffect
//
//  @param 
//         name,  audio file's name
//  @param
//         delay, delay a time to play sound
//
void XIOSPlatform::PlayAudioEffect(const char*name, float delay)
{
    // play audio effect
    [device playEffect:name delay:delay];
}


///
//  @prototype, getCurLanguage
//
//  @param 
//         void
//  @note, get current lanaguage from the device
// get cur language
int XIOSPlatform::getCurLanguage()
{
    NSUserDefaults *defs = [NSUserDefaults  standardUserDefaults];
    NSArray *languages = [defs objectForKey:@"AppleLanguages"];
    NSString *preferredLang = [languages objectAtIndex:0];
    NSLog(@"%@", preferredLang);
    
    if(([preferredLang isEqualToString:@"zh-Hans"]))
    {
        return LANGUAGE_HS; //简体中文
    }
    else if(([preferredLang isEqualToString:@"zh-Hant"]))
    {
        return LANGUAGE_HT; //繁体中文
    }
    else
    {
        return LANGUAGE_EN; //其他为英语
    }    
}

/*
 *@brief 获取设备的uuid
 */
const char *XIOSPlatform::getDeviceUUID()
{
    return [[TaomeeFeedback openUDID] UTF8String];
}

uint32_t XIOSPlatform::getVersion()
{
    
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
    NSArray *versionNumbers = [version componentsSeparatedByString:@"."];
    int index = 0;
    uint32_t fullVersion = 0;
    for (NSString *number in versionNumbers) {
        if (index <= 3) {
          	fullVersion += [number intValue]<<8*(3-index);  
        }
        else {
            break;
        }
        index++;
    }
    return fullVersion;
}

const char *XIOSPlatform::getVersionA()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] UTF8String];
}

float XIOSPlatform::getSystemVersion()
{
    return [[[UIDevice currentDevice] systemVersion] floatValue];
}


/*
 *@brief 判断网络是否链接
 */
bool XIOSPlatform::IsConnected()
{
    // Create zero addy
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    
    // Recover reachability flags
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags)
    {
        return NO;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    return (isReachable && !needsConnection) ? YES : NO;
    
    
}

/*
 *@brief 判断设备是ipad　or　iphone
 *
 */


UIUserInterfaceType XIOSPlatform::getUserInterface()
{
    
    BOOL iPad = NO;
    
#ifdef UI_USER_INTERFACE_IDIOM
    iPad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#endif
    
    if (iPad) {
        return UIUserInterfacePad;
    } else {
        if(([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640,960), [[UIScreen mainScreen] currentMode].size) : NO))
        {
            return UIUserInterfaceRetinaPhone;
        }
        return UIUserInterfacePhone;
    }  
}



bool XIOSPlatform::IsRetinaDisplay()
{
    if(([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640,960), [[UIScreen mainScreen] currentMode].size) : NO))
    {
        return true;
    }
    return false;
}


bool isipad = false;
void XIOSPlatform::initUserInterface()
{
    printf("XIOSPlatform::initUserInterface()");
    isipad = false;
    
#ifdef UI_USER_INTERFACE_IDIOM
    isipad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#endif 
}

bool XIOSPlatform::IsIpad()
{
    return isipad;
}

const char *XIOSPlatform::CCLocalizedString(const char *key,const char *comment)
{
    NSString *bKey = [NSString stringWithUTF8String:key];
    return  [NSLocalizedString(bKey, @"") UTF8String];
}

const char* XIOSPlatform::getPasswordForUsername(const char* username,const char* serviceName)
{
    return [[SFHFKeychainUtils getPasswordForUsername:[NSString stringWithUTF8String:username] andServiceName:[NSString stringWithUTF8String:serviceName] error:nil]UTF8String];
}

bool XIOSPlatform::storeUsername(const char* username ,const char* password,const char* serviceName ,bool updateExisting )
{
    return [SFHFKeychainUtils storeUsername:[NSString stringWithUTF8String:username] andPassword:[NSString stringWithUTF8String:password] forServiceName:[NSString stringWithUTF8String:serviceName] updateExisting:(BOOL)updateExisting error:nil];
}

int XIOSPlatform::UTF8Length(const char* str) {
    NSString *strString = [NSString stringWithCString:str encoding:NSUTF8StringEncoding];
    
    return strString.length;
}

void XIOSPlatform::UTF8SubString(const char* inStr, int length, char* outStr) {
    NSString *inString = [NSString stringWithCString:inStr encoding:NSUTF8StringEncoding];
    
    NSString *subString = [inString substringWithRange:NSMakeRange(0, length)];
    
    sprintf(outStr, "%s", [subString UTF8String]);
}
