#include "NetWorkConfigInfoFromServer.h"
#include <string>
#include "cocos2d.h"
#include "UserLoginStatistics.h"
#include "curl/curl.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"
#include "UserData.h"
#include "AppDelegate.h"

std::string UrlEncode(const std::string& szToEncode);

UserLoginStatistics::UserLoginStatistics()
{

}

UserLoginStatistics::~UserLoginStatistics()
{
	
}
	
void UserLoginStatistics::sendStatInfoToServer(const char* key)
{
    //return;//jackniu xiaomi crash when login. disable this according to wesly
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	int num = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(key);
	if(num == 0)
	{
		++num;
		std::string url = "";
		//const char* address = "211.151.105.169/server/frontstat.php";
		//const char* address = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetStateURL");
		const char* address = NetWorkConfigInfoFromServer::sharedInstance()->GetFrontStateURL();
		//std::string msg = "";
		url.append(address);
		url.append("?key=");
		url.append(key);
		url.append("&value=");
		char numStr[10];
		sprintf(numStr,"%d",num);
		url.append(numStr);

		string appName = LoginManager::Get()->getAppName();
		if(appName != "")
		{
			url.append("&appName=");
			url.append(appName);
		}

		// deviceId
		string deviceId = LoginManager::Get()->getDeviceId(); 
		if(deviceId != "")
		{
			url.append("&deviceId=");
			url.append(deviceId);
		}
		// deviceType
		string deviceType = LoginManager::Get()->getDeviceType(); 
		if(deviceType != "")
		{
			url.append("&deviceType=");
			url.append(deviceType);
		}
		// softVersion
		string softVersion = LoginManager::Get()->getSoftVersion(); 
		if(softVersion != "")
		{
			url.append("&softVersion=");
			url.append(softVersion);
		}
		// networkType
		string networkType = LoginManager::Get()->getNetworkType(); 
		if(networkType != "")
		{
			url.append("&networkType=");
			url.append(networkType);
		}
		// SubscriberId
		string subscriberId = LoginManager::Get()->getProviderId(); 
		if(subscriberId != "")
		{
			url.append("&subscriberId=");
			url.append(subscriberId);
		}

		PLAYER_ID playerId = UserData::getUserId();
		if(playerId.getID() != 0)
		{
			url.append("&userId=");
			char userId[20];
			sprintf(userId,"%d",playerId.getID());
			url.append(userId);
			url.append("&regTime=");
			char regTime[20];
			sprintf(regTime,"%d",playerId.getRegTime());
			url.append(regTime);
		}

		unsigned int channelId = LoginManager::Get()->getChannelID();
		char channel[20];
		sprintf(channel,"%d",channelId);
		url.append("&channel=");
		url.append(channel);

		USERINFO info = UserData::GetUserInfo();
		unsigned int serverId = info.serverId;
		if(serverId != 0)
		{
			char server[20];
			sprintf(server,"%d",serverId);
			url.append("&server=");
			url.append(server);
		}

		/*if(info.szName != '\0' && info.szName != 0x00)
		{
			url.append("&userName=");
			url.append(info.szName);
		}*/

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		char resolution[20];
		sprintf(resolution,"%d*%d",(int)winSize.width,(int)winSize.height);
		url.append("&resolution=");
		url.append(resolution);

		CCLOG("url: %s",url.c_str());

		std::string urlEncoded = UrlEncode(url);

		CCLOG("urlEncoded: %s",urlEncoded.c_str());

	
		std::string strRetData = "";

		CURLcode curlRes;
		CURL* easyCurl = curl_easy_init();
		if (easyCurl == NULL) {
			return;
		}
		//strHtml.clear();
		
		//curl_easy_setopt(easyCurl, CURLOPT_POSTFIELDS, msg.c_str());
	
		curl_easy_setopt(easyCurl, CURLOPT_URL,urlEncoded.c_str());
		curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,3);
		 //curl_easy_setopt(easyCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		//curl_easy_setopt(easyCurl, CURLOPT_POST, 1);
		//curl_easy_setopt(easyCurl, CURLOPT_TIMEOUT, 10);  
	//	curl_easy_setopt(easyCurl, CURLOPT_FOLLOWLOCATION, true);
		 //curl_easy_setopt(easyCurl, CURLOPT_WRITEDATA, &strHtml);
		//curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,120);
		curlRes = curl_easy_perform(easyCurl);

		curl_easy_cleanup(easyCurl);

		if(curlRes == CURLE_OK)
		{
			cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(key,num);
			cocos2d::CCUserDefault::sharedUserDefault()->flush();
		}
		//else
		//{
		//	AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect !UserLoginStatistics::sendStatInfoToServer");
		//}
	}
//#endif
}

void UserLoginStatistics::sendLaunchStat(unsigned int channelId,const char* version,const char* platform,
                                        const char* equdid,const char* eqtype,const char* mac)
{
    const char* netUrl = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/GameConfigParse.lua","GetLaunchStatUrl");
    if(netUrl == NULL)
    {
        return;
    }
    char channel[20];
    sprintf(channel,"%u",channelId);
    std::string channelStr = UrlEncode(channel);
    std::string versionStr = UrlEncode(version);
    std::string platformStr = UrlEncode(platform);
    std::string equdidStr = UrlEncode(equdid);
    std::string eqtypeStr = UrlEncode(eqtype);
    std::string macStr = UrlEncode(mac);
    std::string url = netUrl;
    url.append("?");
    url.append("&channel=");
    url.append(channelStr);
    url.append("&version=");
    url.append(version);
    url.append("&platform=");
    url.append(platformStr);
    url.append("&equdid=");
    url.append(equdidStr);
    url.append("&eqtype=");
    url.append(eqtypeStr);
    url.append("&mac=");
    url.append(mac);
    
    CURLcode curlRes;
    CURL* easyCurl = curl_easy_init();
    if (easyCurl == NULL) {
        return;
    }
    
    curl_easy_setopt(easyCurl, CURLOPT_URL,url.c_str());
	curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,3);
    curlRes = curl_easy_perform(easyCurl);
    curl_easy_cleanup(easyCurl);

	if(curlRes != CURLE_OK)
	{
		//AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect ! UserLoginStatistics::sendLaunchStat");
	}
}

std::string UrlEncode(const std::string& szToEncode)
{  
	std::string src = szToEncode;  
	char hex[] = "0123456789ABCDEF";  
	string dst;  
	for (size_t i = 0; i < src.size(); ++i)
	{  
		unsigned char cc = src[i];  
		if (isascii(cc))  
		{
			if (cc == ' ')  
			{  
				dst += "%20";  
			}  
			else 
				dst += cc;  
		}  
		else 
		{  
			unsigned char c = static_cast<unsigned char>(cc);
			dst += '%';  
			dst += hex[c / 16];  
			dst += hex[c % 16];
		}  
	}  
	return dst;  
}  
