#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "NetWorkConfigInfoFromServer.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"
#include "XLogicManager.h"

LoginManager::LoginManager()
{

}

LoginManager::~LoginManager()
{

}

void LoginManager::doPlatformLogin()
{

}

void LoginManager::doUserCenter(){
	CCLOG("not use for now");
}

void LoginManager::onPlatformLoginSuccess()
{
	CCLOG("LoginManager::onPlatformLoginSuccess()");
	//const char* sess = TmLoginManager::tmLoginManager()->getSession();
	const char* sess = "123465";
	int platform = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua","GetDebugChannel");
	int serverId = 0;
	int userId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua","GetDebugUserId");
	const char* appId = "200857196";
	const char* appKey = "654321654";
	const char* cpID = "";
	const char* channelId = "";
	const char* extra = "";
	unsigned int gameId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua","GetDebugGameID");
	XLogicManager::sharedManager()->sendPlatformLoginMessage(sess,platform,serverId,userId,appId,appKey,cpID,channelId,extra,gameId);
}


void LoginManager::update(float dt)
{
}

bool LoginManager::doRecentLogin()
{
	return false;
}

void LoginManager::doLoginOut()
{
	//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Login/loginInterface.lua","enterLoginLayer");
}

void LoginManager::loginWithUserName(const char* userName,const char* password)
{
    //CCLOG("LoginManager::loginWithUserName");
}

void LoginManager::registerWithUserName(const char* userName,const char* password,const char* confirmPassword)
{
    //CCLOG("LoginManager::registerWithUserName");
}

void LoginManager::onLoginSuccess(unsigned int uid,const char* session,const char* userName)
{
    //CCLOG("LoginManager::onLoginSuccess");
}

void LoginManager::onRegisterSuccess(unsigned int uid,const char* userName)
{
    //CCLOG("LoginManager::onLoginSuccess");
}

void LoginManager::onRegisterFailed(unsigned int error)
{
    //CCLOG("LoginManager::onRegisterFailed");
}

void LoginManager::onLoginFailed(unsigned int error)
{
    //CCLOG("LoginManager::onLoginFailed");
}

void LoginManager::onChangePasswordSuccess()
{
    //CCLOG("LoginManager::onChangePasswordSuccess");
}

void LoginManager::onChangePasswordFailed(unsigned int error)
{
    //CCLOG("LoginManager::onChangePasswordFailed");
}

uint32_t LoginManager::getChannelID()
{
	uint32_t channel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");
	return channel;
}

uint32_t LoginManager::getGameID()
{
	//uint32_t gameID = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetGameID");
	uint32_t gameID = NetWorkConfigInfoFromServer::sharedInstance()->GetGameID();
	return gameID;
}

string LoginManager::getDeviceId()
{
	return "";
}

string LoginManager::getDeviceType()
{
	return "";
}

string LoginManager::getSoftVersion()
{
	return "";
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

std::string LoginManager::getMacAddress()
{
	return "";
}

std::string LoginManager::getIpAddress()
{
	return "";
}

#endif