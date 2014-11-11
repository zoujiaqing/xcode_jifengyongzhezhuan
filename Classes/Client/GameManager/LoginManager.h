#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_
#include "Singleton_t.h"
#include "cocos2d.h"

USING_NS_CC;
enum
{
	ENone = 0,
	ETaomeeLoginSuccess = 1,
	EPlatformLoginSuccess = 2,
};

class LoginManager : public TSingleton<LoginManager>,public CCObject
{
public:
	LoginManager();
	~LoginManager();

	// 用户登录
	void loginWithUserName(const char* userName,const char* password);

	// 用户注册
	void registerWithUserName(const char* userName,const char* password,const char* confirmPassword);
    
    bool doRecentLogin();
    
    void onLoginSuccess(unsigned int uid,const char* session,const char* userName);
    
    void onRegisterSuccess(unsigned int uid,const char* userName);
    
    void onRegisterFailed(unsigned int error);
    
    void onLoginFailed(unsigned int error);
    
    void onChangePasswordSuccess();
    
    void onChangePasswordFailed(unsigned int error);

	void doPlatformLogin();

	void onPlatformLoginSuccess();

	void doLoginOut();
	void doUserCenter();
	virtual void update(float dt);

	// 得到用户渠道号
	uint32_t getChannelID();

	uint32_t getGameID();

	std::string getDeviceId();

	std::string getDeviceType();

	std::string getSoftVersion();

	std::string getNetworkType();

	std::string getProviderId();

	std::string getAppName();

	std::string getMacAddress();

	std::string getIpAddress();

	std::string getDeviceOS()
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		return "IOS";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		return "andorid";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		return "win32";
#else 
		return "unknown";
#endif
	}
private:
    
    std::string m_registerName;
    
    std::string m_registerPassword;

	int loginStatus;
};

#endif