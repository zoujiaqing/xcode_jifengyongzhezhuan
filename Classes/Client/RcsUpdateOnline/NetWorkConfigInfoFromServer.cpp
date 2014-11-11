#include "NetWorkConfigInfoFromServer.h"
#include <sys/stat.h>
#include <string.h>
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "curl/curl.h"
#include "json_res.h"
#include "FileManager.h"
#include "GameResourceManager.h"
#include "LuaTinkerManager.h"
#include "AppDelegate.h"
#include "ResourcesUpdateManager.h"
#include "util.h"

#define RE_DOWNLOAD_TIMES 3

NetWorkConfigInfoFromServer *NetWorkConfigInfoFromServer::instance = NULL;
 void NetWorkConfigInfoFromServer::puresharedInstance()
	{
	    if(instance)
		{
		   delete instance ;
		   instance = NULL ;

		}
	}
NetWorkConfigInfoFromServer *NetWorkConfigInfoFromServer::sharedInstance()
{
	if (instance == NULL) {
		instance = new NetWorkConfigInfoFromServer();
	}

	return instance;
}

NetWorkConfigInfoFromServer::NetWorkConfigInfoFromServer()
{
	m_nRepeatTimes = RE_DOWNLOAD_TIMES;
	mIsDownLoadContentFromServer = false;
}

NetWorkConfigInfoFromServer::~NetWorkConfigInfoFromServer()
{
}

size_t NetWorkConfigInfoFromServer::configFileDownLoadCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
	/** To append content to a string */
	NetWorkConfigInfoFromServer::sharedInstance()->m_strFileList.append(ptr);
	return  size*nmemb;
}


bool NetWorkConfigInfoFromServer::downloadConfigFileListOneTime(const char* url,const char* postMessage)
{
	if (url == NULL) {
		return false;
	}

	std::string strHtml = "";
	std::string strRetData = "";    

	CURLcode curlRes;
	CURL* easyCurl = curl_easy_init();
	if (easyCurl == NULL) {
		return false;
	}
	curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &NetWorkConfigInfoFromServer::configFileDownLoadCallback);
	curl_easy_setopt(easyCurl, CURLOPT_POSTFIELDS, postMessage);
	curl_easy_setopt(easyCurl, CURLOPT_WRITEDATA, &strHtml);
	curl_easy_setopt(easyCurl, CURLOPT_URL,url);
	curl_easy_setopt(easyCurl, CURLOPT_POST, 1);
	curl_easy_setopt(easyCurl, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,30);
	curlRes = curl_easy_perform(easyCurl);
	curl_easy_cleanup(easyCurl);
	bool result = (curlRes == CURLE_OK);
	CCLOG("%s result is %d", __FUNCTION__, result);
	return result;


}

bool NetWorkConfigInfoFromServer::downloadConfigFileList()
{	
	const char* net_flag = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetNetWorkFlag");
	const char* channelId = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetChannelID");
	const char* url = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetNetWorkServerConfigURL");
    const char* url2 = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetNetWorkServerConfigURL2");

	char postMessage[256];
	memset(postMessage,'\0',sizeof(postMessage)/sizeof(char));
	string device_id = ResourceUpdateManager::sharedInstance()->GetUniquedDeviceID();
	const char* svn_version = AppDelegate::GetShareAppDelegate()->GetSvnVersion();
	sprintf(postMessage,"channel_id=%s&net_flag=%s&device_id=%s&svn_version=%s",channelId,net_flag,device_id.c_str(),svn_version);

	while (m_nRepeatTimes > 0)
	{
		if (downloadConfigFileListOneTime(url,postMessage))
		{
			mIsDownLoadContentFromServer = true;
			return true;
		}

        if (downloadConfigFileListOneTime(url2, postMessage))
        {
            mIsDownLoadContentFromServer = true;
            return true;
        }
		m_nRepeatTimes --;
	}
	ShowToast("Sorry , NetWork is disconnect !NetWorkConfigInfoFromServer::downloadConfigFileList()");
	CCLOG("NetWorkConfigInfoFromServer::downloadConfigFileList() NetWork Error");
	return false;
}

bool NetWorkConfigInfoFromServer::parseRcvMessageFromServer()
{
	const char* pMessage = m_strFileList.c_str();

	CCLOG("download step m_strFileList %s",pMessage);
	if (pMessage == NULL)
	{
		return false;
	}

	Json::Value jsonValue;
	if (!parseJsonStr(pMessage, jsonValue)) 
	{
		CCLOGERROR("NetWorkConfigInfoFromServer Error: %s",pMessage);
		return false;
	}

	const Json::Value isUsePlatformArray = jsonValue["is_use_oth_platform"];
	const Json::Value ipArray = jsonValue["ip"];
	const Json::Value portArray = jsonValue["port"];
	const Json::Value gameidArray = jsonValue["gameid"];
	const Json::Value rcs_download_urlArray = jsonValue["rcs_download_url"];
	const Json::Value rcs_download_urlArray2 = jsonValue["rcs_download_url2"];
	const Json::Value server_list_urlArray = jsonValue["server_list_url"];
	const Json::Value front_state_urlArray = jsonValue["front_state_url"];
    const Json::Value notice_state_urlArray = jsonValue["notice_state_url"];
    
	const Json::Value serverArray = jsonValue["server_id"];
	const Json::Value appIdArray = jsonValue["app_id"];
	const Json::Value appKeyArray = jsonValue["app_Key"];
	const Json::Value cpIdArray = jsonValue["cp_id"];
	const Json::Value extraDataArray = jsonValue["extra_data"];
	const Json::Value thirdChannelArray = jsonValue["third_channel"];

	if (false == isUsePlatformArray.isNull())
	{
		mNetWorkConfigInfo.tPlatformFlag = atoi(isUsePlatformArray.asString().c_str());
	}
	if (false == ipArray.isNull())
	{
		mNetWorkConfigInfo.tIP = ipArray.asString();
	}
	if (false == portArray.isNull())
	{
		mNetWorkConfigInfo.tIpPort = portArray.asString();
	}
	if (false == gameidArray.isNull())
	{
		mNetWorkConfigInfo.tGameId = atoi(gameidArray.asString().c_str());
	}
	if (false == rcs_download_urlArray.isNull())
	{
		mNetWorkConfigInfo.tRcsDownloadUrl = rcs_download_urlArray.asString();
	}
    if (false == rcs_download_urlArray2.isNull())
    {
        mNetWorkConfigInfo.tRcsDownloadUrl2 = rcs_download_urlArray2.asString();
    }
	if (false == server_list_urlArray.isNull())
	{
		mNetWorkConfigInfo.tServerListUrl = server_list_urlArray.asString();
	}
	if (false == front_state_urlArray.isNull())
	{
		mNetWorkConfigInfo.tFrontStateUrl = front_state_urlArray.asString();
	}
    if(false == notice_state_urlArray.isNull())
    {
        mNetWorkConfigInfo.tNoticesUrl = notice_state_urlArray.asString();
    }
	if(!serverArray.isNull())
	{
		mNetWorkConfigInfo.tServerId = atoi(serverArray.asString().c_str());
	}
	if(!appIdArray.isNull())
	{
		std::string appId = appIdArray.asString();
		if(appId == "null")
		{
			appId = "";
		}
		mNetWorkConfigInfo.tAppId = appId;
	}
	if(!appKeyArray.isNull())
	{
		std::string appKey = appKeyArray.asString();
		if(appKey == "null")
		{
			appKey = "";
		}
		mNetWorkConfigInfo.tAppKey = appKey;
	}
	if(!cpIdArray.isNull())
	{
		std::string cpId = cpIdArray.asString();
		if(cpId == "null")
		{
			cpId = "";
		}
		mNetWorkConfigInfo.tCpId = cpId;
	}
	if(!extraDataArray.isNull())
	{
		std::string extraData = extraDataArray.asString();
		if(extraData == "null")
		{
			extraData = "";
		}
		mNetWorkConfigInfo.tExtraData = extraData;
	}
	if(!thirdChannelArray.isNull())
	{
		std::string thirdChannel = thirdChannelArray.asString();
		if(thirdChannel == "null")
		{
			thirdChannel = "";
		}
		mNetWorkConfigInfo.tThirdChannel = thirdChannel;
	}

	return true;
}
