#include "ResourceVersionCheckFromServer.h"
#include <sys/stat.h>
#include <string.h>
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "curl/curl.h"
#include "json_res.h"
#include "FileManager.h"
#include "GameResourceManager.h"
#include "LuaTinkerManager.h"
#include "ResourcesUpdateManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "AppDelegate.h"

#define RE_DOWNLOAD_TIMES 5

ResouceVersionCheckFromServer *ResouceVersionCheckFromServer::instance = NULL;

ResouceVersionCheckFromServer *ResouceVersionCheckFromServer::sharedInstance()
{
	if (instance == NULL) {
		instance = new ResouceVersionCheckFromServer();
	}

	return instance;
}

ResouceVersionCheckFromServer::ResouceVersionCheckFromServer()
{
	m_nRepeatTimes = RE_DOWNLOAD_TIMES;
	m_isNeedUpdate = false;
}

ResouceVersionCheckFromServer::~ResouceVersionCheckFromServer()
{
}

size_t ResouceVersionCheckFromServer::configFileDownLoadCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
	/** To append content to a string */
	ResouceVersionCheckFromServer::sharedInstance()->m_strFileList.append(ptr);
	return  size*nmemb;
}


bool ResouceVersionCheckFromServer::downloadConfigFileListOneTime(const char* url,const char* postMessage)
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
	curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &ResouceVersionCheckFromServer::configFileDownLoadCallback);
	curl_easy_setopt(easyCurl, CURLOPT_POSTFIELDS, postMessage);
	curl_easy_setopt(easyCurl, CURLOPT_WRITEDATA, &strHtml);
	curl_easy_setopt(easyCurl, CURLOPT_URL,url);
	curl_easy_setopt(easyCurl, CURLOPT_POST, 1);
	curl_easy_setopt(easyCurl, CURLOPT_FOLLOWLOCATION, true);
	//curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,30);
	curlRes = curl_easy_perform(easyCurl);
	curl_easy_cleanup(easyCurl);
	bool result = (curlRes == CURLE_OK);

	return result;
}

bool ResouceVersionCheckFromServer::downloadConfigFileList()
{	
	const char* channelId = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetChannelID");
	const char* url = NetWorkConfigInfoFromServer::sharedInstance()->GetRcsDownloadURL();

	std::string str_so_main_version = ResourceUpdateManager::sharedInstance()->GetMainVersionFromJava();
	CCLOG("rcsCheck so main version %s",str_so_main_version.c_str());
	std::string str_so_sub_version = ResourceUpdateManager::sharedInstance()->GetSubVersionFromJava();
	CCLOG("rcsCheck so sub version %s",str_so_sub_version.c_str());

	std::string str_main_version = "";
	std::string str_sub_version = "";
	if (false == ResourceUpdateManager::sharedInstance()->GetVersionInfoFromLocal(str_main_version,str_sub_version))
	{
		str_main_version = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua", "GetMainVersion");
		str_sub_version = "0";
	}

	CCLOG("rcsCheck rcs main version %s",str_main_version.c_str());
	CCLOG("rcsCheck rcs sub version %s",str_sub_version.c_str());

	char postMessage[256];
	memset(postMessage,'\0',sizeof(postMessage)/sizeof(char));
	string device_id = ResourceUpdateManager::sharedInstance()->GetUniquedDeviceID();
	const char* svn_version = AppDelegate::GetShareAppDelegate()->GetSvnVersion();
	sprintf(postMessage,"channelid=%s&main_version=%s&sub_version=%s&type=2&s_main_version=%s&s_sub_version=%s&device_id=%s&svn_version=%s",
		channelId,str_main_version.c_str(),str_sub_version.c_str(),str_so_main_version.c_str(),str_so_sub_version.c_str(),device_id.c_str(),svn_version);

	while (m_nRepeatTimes > 0)
	{
		if (downloadConfigFileListOneTime(url,postMessage))
		{
			return true;
		}
		m_nRepeatTimes --;
	}
	//AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect !ResouceVersionCheckFromServer::downloadConfigFileList()");
	CCLOG("ResouceVersionCheckFromServer::downloadConfigFileList() NetWork Error");
	return false;
}

bool ResouceVersionCheckFromServer::parseRcvMessageFromServer()
{
	// Note: 返回1表示是最新的版本 0表示不是最新版本
	const char* pMessage = m_strFileList.c_str();

	CCLOG("download step m_strFileList %s",pMessage);
	if (pMessage == NULL)
	{
		return false;
	}

	if (strcmp(pMessage,"0") == 0)
	{
		m_isNeedUpdate = true;
	}
	else if (strcmp(pMessage,"1") == 0)
	{
		m_isNeedUpdate = false;
	}
	else
	{
		CCLOGERROR("ResouceVersionCheckFromServer Error: %s",pMessage);
		m_isNeedUpdate = false;
		return false;
	}
	return true;
}