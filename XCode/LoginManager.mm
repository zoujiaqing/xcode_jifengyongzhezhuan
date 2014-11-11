#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "NetWorkConfigInfoFromServer.h"
#include "LoginController.h"
#include "LoginManager.h"
#include "LuaTinkerManager.h"
#include "XLogicManager.h"
#include "UserData.h"
#import "sys/utsname.h" 
#include <ifaddrs.h>
#include <arpa/inet.h>
#import "NcSDKManager.h"

/////
#ifdef TargetForKy
#import "KyLoginMan.h"
#import "KyPayMan.h"
#import "KYSDK.h"
#endif

#ifdef TargetForCoCoAppStore
#import "CocoLogin.h"
#include "IAPManager.h"
#endif

#ifdef TargetForPP
#import "PPLoginMan.h"
#import "PPPayMan.h"
#endif

#ifdef TargetForNd91
#import "NdLoginMan.h"
#import "OrderMan.h"
#import "NdPayMan.h"
#endif

#ifdef TargetForItools
#import "HXPaymentMan.h"
#import "HXLoginMan.h"
#endif

#ifdef TargetForTB
#import "TbLoginMan.h"
#import "TbPayMan.h"
#endif

#ifdef TargetForI4
#import "I4LoginMan.h"
#import "I4PayMan.h"
#endif

#ifdef TargetForXy
#import "XYPayMan.h"
#import "XYLoginMain.h"
#endif

LoginManager::LoginManager()
{
    m_registerName = "";
    m_registerPassword = "";
}

LoginManager::~LoginManager()
{

}

void LoginManager::doPlatformLogin()
{
    if(XLogicManager::sharedManager()->isIOSDisconnect == true)
    {
        // is in game scene disconnect to login layer
        NcSDKManager::doLastUserLogin();
        XLogicManager::sharedManager()->isIOSDisconnect = false;
    }
    else
    {
        NcSDKManager::doLogin();
    }
}

void LoginManager::onPlatformLoginSuccess()
{
	std::string sess_str(UserData::getUserSession());
	int platform = getChannelID();
	int serverId = 0;
	int userId = UserData::getUserId().getID();
	const char* appId = NetWorkConfigInfoFromServer::sharedInstance()->GetAppId();
	const char* appKey = NetWorkConfigInfoFromServer::sharedInstance()->GetAppKey();
	const char* cpId = NetWorkConfigInfoFromServer::sharedInstance()->GetCpId();
	const char* channel = NetWorkConfigInfoFromServer::sharedInstance()->GetThirdChannel();
    const char* extra = NetWorkConfigInfoFromServer::sharedInstance()->GetExtraData();
#ifdef TargetForNd91
    extra = [[[NdLoginMan sharedInstance] getUserInfoJson] UTF8String];
#endif
#ifdef TargetForItools
    extra = [[[HXLoginMan sharedInstance] getUserInfoJson] UTF8String];
#endif
#ifdef TargetForTB
    extra = [[[TbLoginMan sharedInstance] getUserInfoJson] UTF8String];
#endif
#ifdef TargetForI4
    extra = [[[I4LoginMan sharedInstance] getUserInfoJson] UTF8String];
#endif
#ifdef TargetForXy
    extra = [[[XYLoginMain sharedInstance] getUserInfoJson] UTF8String];
#endif
	unsigned int gameId = 0;
#ifdef TargetForNw
    gameId = 82;
#endif
	XLogicManager::sharedManager()->sendPlatformLoginMessage(sess_str.c_str(),platform,serverId,userId,appId,appKey,cpId,channel,extra,gameId);
}

void LoginManager::doLoginOut()
{
#ifdef TargetForNw
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","enterLoginLayer");
#else
    NcSDKManager::doLogout();
#endif
	
}

bool LoginManager::doRecentLogin()
{
    cocos2d::CCLog("LoginManager::doRecentLogin");
    bool ret = [[LoginController sharedInstance] doRecentLogin];
    return ret;
}

void LoginManager::loginWithUserName(const char* userName,const char* password)
{
    cocos2d::CCLog("LoginManager::loginWithUserName");
    [[LoginController sharedInstance] loginWithUserName:userName password:password];
}

void LoginManager::registerWithUserName(const char* userName,const char* password,const char* confirmPassword)
{
    cocos2d::CCLog("LoginManager::registerWithUserName");
    m_registerName = std::string(userName);
    m_registerPassword = std::string(password);
    [[LoginController sharedInstance] doRegisterWithUserName:userName password:password confirmPassword:confirmPassword];
}

void LoginManager::onLoginSuccess(unsigned int uid,const char* session,const char* userName)
{
    cocos2d::CCLog("LoginManager::onLoginSuccess: %d, %s, %s",uid,session,userName);
    
    UserData::Create();
    USERINFO mUserData = UserData::GetUserInfo();
    bool useDebugUser = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "UseDebugUser");
    if (useDebugUser)
    {
		uint32_t debugId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua", "GetDebugUserId");
		uint32_t channel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua","GetDebugChannel");
		mUserData.id.setPlayId(debugId,0,channel);
        string session = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfig/DebugConfig.lua", "GetDebugUserSession");
        strcpy(mUserData.userSession,session.c_str());
    }
    else
    {
        uint32_t channel = this->getChannelID();
		mUserData.id.setPlayId(uid,0,channel);
        strcpy(mUserData.userSession,session);
    }
    UserData::SetUserInfo(mUserData);
    XLogicManager::sharedManager()->connectToServer();
}

void LoginManager::onRegisterSuccess(unsigned int uid,const char* userName)
{
    cocos2d::CCLog("LoginManager::onRegisterSuccess: %d , %s",uid,userName);
    this->loginWithUserName(m_registerName.c_str(), m_registerPassword.c_str());
}

void LoginManager::onRegisterFailed(unsigned int error)
{
    cocos2d::CCLog("LoginManager::onRegisterFailed: %d ",error);
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","onReceiveRigisterError",error);
}

void LoginManager::onLoginFailed(unsigned int error)
{
    cocos2d::CCLog("LoginManager::onLoginFailed: %d", error);
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","onReceiveLoginError",error);
}

void LoginManager::onChangePasswordSuccess()
{
    cocos2d::CCLog("LoginManager::onChangePasswordSuccess");
}

void LoginManager::onChangePasswordFailed(unsigned int error)
{
    cocos2d::CCLog("LoginManager::onChangePasswordFailed");
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","onReceiveLoginError",error);
}

uint32_t LoginManager::getChannelID()
{
	return [[LoginController sharedInstance] getChannelId];
}

uint32_t LoginManager::getGameID()
{
    return [[LoginController sharedInstance] getGameId];
}

void LoginManager::update(float dt)
{
    
}

string LoginManager::getDeviceId()
{
    int userId = 0;
    CCString* key = CCString::createWithFormat("uuidstr%d", userId);
    std::string uuidStr = CCUserDefault::sharedUserDefault()->getStringForKey(key->getCString());
    if(std::strcmp(uuidStr.c_str(), "") == 0)
    {
        // if it is first time to set uuid
        CFUUIDRef puuid = CFUUIDCreate( nil );
        CFStringRef uuidString = CFUUIDCreateString( nil, puuid );
        NSString * result = (NSString *)CFStringCreateCopy( NULL, uuidString);
        CFRelease(puuid);
        CFRelease(uuidString);
        string uuid = [result UTF8String];
        [result autorelease];
        
        CCUserDefault::sharedUserDefault()->setStringForKey(key->getCString(), uuid);
        return uuid;
    }
    else
    {
        return uuidStr;
    }
}

/*
 * desc: get ios device type
 *
 * iPhone1,1   iPhone
 * iPhone1,2   iPhone 3G
 * iPhone2,1   iPhone 3GS
 * iPhone3,1   iPhone 4
 * iPhone3,3   iPhone 4 (Verizon)
 * iPhone3,3   iPhone 4 (CDMA)
 * iPhone4,1   iPhone 4S
 * iPhone5,1   iPhone 5 (not sure)
 *
 * iPod1,1     iPod Touch1
 * iPod2,1     iPod Touch2
 * iPod3,1     iPod Touch3
 * iPod4,1     iPod Touch4
 * iPod5,1     iPod Touch5 (not sure)
 *
 * iPad1,1     iPad1
 * iPad2,1     iPad2 (Wifi)
 * iPad2,2     iPad2 (3G GSM)
 * iPad2,3     iPad2 (3G CDMA)
 * iPad2,5     iPad Mini (may be)
 * iPad2,6     iPad Mini (may be)
 * iPad3,1     iPad3 (Wifi)
 * iPad3,2     iPad3 (4G)
 * iPad3,3     iPad3 (4G)
 * iPad3,6     iPad4 (may be)
 *
 * i386        Simulator
 * x86_64      Simulator
 *
 * AppleTV2,1 AppleTV(2G)
 */
string LoginManager::getDeviceType()
{
    /*NSString* str = [[UIDevice currentDevice] model];
    string type = [str UTF8String];
    //[str autorelease];
	return type;*/
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *machine = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    if(machine != NULL)
    {
        string type = [machine UTF8String];
        return type;
    }
    return "";
}

string LoginManager::getSoftVersion()
{
    NSString* str = [[UIDevice currentDevice] systemVersion];
    string version = [str UTF8String];
    //[str autorelease];
	return version;
}

string LoginManager::getNetworkType()
{
	return "";
}

string LoginManager::getProviderId()
{
	return "";
}

string LoginManager::getAppName()
{
	return "";
}

string LoginManager::getMacAddress()
{
    return "";
}

string LoginManager::getIpAddress()
{
    NSString *address = @"error";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0)
    {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL)
        {
            if(temp_addr->ifa_addr->sa_family == AF_INET)
            {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en1"])
                {
                    // Get NSString from C String
                    address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    string add = [address UTF8String];
    return add;
}

void LoginManager::doUserCenter()
{
#ifdef TargetForKy
    [[KyLoginMan sharedInstance] showUserCenter];
    XLogicManager::sharedManager()->isIOSInScene = true;
#endif
    
#ifdef TargetForPP
    [[PPLoginMan sharedInstance] showUserCenter];
    XLogicManager::sharedManager()->isIOSInScene = true;
#endif
    
#ifdef TargetForI4
    [[I4LoginMan sharedInstance] showUserCenter];
    XLogicManager::sharedManager()->isIOSInScene = true;
#endif

#ifdef TargetForItools
    [[HXLoginMan sharedInstance] showPlatformView];
    XLogicManager::sharedManager()->isIOSInScene = true;
#endif
}

#endif