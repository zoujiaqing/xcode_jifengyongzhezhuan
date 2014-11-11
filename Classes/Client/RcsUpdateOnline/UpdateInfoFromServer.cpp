#include "NetWorkConfigInfoFromServer.h"
#include <sys/stat.h>
#include <string.h>
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "curl/curl.h"
#include "json_res.h"
#include "FileManager.h"
#include "GameResourceManager.h"
#include "UpdateInfoFromServer.h"
#include "LuaTinkerManager.h"
#include "ResourcesUpdateManager.h"
#include "AppDelegate.h"

#define RE_DOWNLOAD_TIMES 1

UpdateInfoFromServer *UpdateInfoFromServer::instance = NULL;

UpdateInfoFromServer *UpdateInfoFromServer::sharedInstance()
{
	if (instance == NULL) {
		instance = new UpdateInfoFromServer();
	}

	return instance;
}

UpdateInfoFromServer::UpdateInfoFromServer()
{
	m_nRepeatTimes = RE_DOWNLOAD_TIMES;
	m_apkUrl = "";
	m_cdnUrl = "";
}

UpdateInfoFromServer::~UpdateInfoFromServer()
{
}

size_t UpdateInfoFromServer::configFileDownLoadCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
	/** To append content to a string */
    UpdateInfoFromServer::sharedInstance()->m_strFileList = "";
	UpdateInfoFromServer::sharedInstance()->m_strFileList.append(ptr);
	return  size*nmemb;
}

string UpdateInfoFromServer::GetEndOfCdnUrlList()
{ 
	if(m_lstCdnUrl.size() == 0)
		return "";

	string url = m_lstCdnUrl.back(); 
	m_lstCdnUrl.pop_back(); 
	
	return url; 
}


bool UpdateInfoFromServer::downloadConfigFileListOneTime(const char* url,const char* postMessage)
{
	if (url == NULL)
	{
		return false;
	}

	std::string strHtml = "";
	std::string strRetData = "";    

	CURLcode curlRes;
	CURL* easyCurl = curl_easy_init();
	if (easyCurl == NULL)
	{
		return false;
	}
	curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &UpdateInfoFromServer::configFileDownLoadCallback);
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

bool UpdateInfoFromServer::downloadConfigFileList(const char* channelId,const char* mainVersion,const char* subVersion,int type)
{
	// Note: inner network
	//const char* url = "http://10.1.1.155/server/updateversion.php3";
	//const char* url = "http://192.168.24.170/version/updateversion.php";
	// Note: outer network
	//const char* url = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetResorceDownLoadURL");
	const char* url = NetWorkConfigInfoFromServer::sharedInstance()->GetRcsDownloadURL();
    const char* nextUrl = NetWorkConfigInfoFromServer::sharedInstance()->GetRcsDownloadURL2();

	char postMessage[256];
	memset(postMessage,'\0',sizeof(postMessage)/sizeof(char));
	//sprintf(postMessage,"channelid=%s&main_version=%s&sub_version=%s&type=%d&device_id=1",channelId,mainVersion,subVersion,type);
    string device_id = ResourceUpdateManager::sharedInstance()->GetUniquedDeviceID();
	const char* svn_version = AppDelegate::GetShareAppDelegate()->GetSvnVersion();
	sprintf(postMessage,"channelid=%s&main_version=%s&sub_version=%s&type=%d&device_id=%s&svn_version=%s",channelId,mainVersion,subVersion,type,device_id.c_str(),svn_version);

	while (m_nRepeatTimes > 0)
	{
		if (downloadConfigFileListOneTime(url,postMessage))
		{
			return true;
		}

		m_nRepeatTimes --;
	}
	//AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect ! UpdateInfoFromServer::downloadConfigFileList");
	//CCLOG("UpdateInfoFromServer::downloadConfigFileList() NetWork Error");
	return false;
}

bool UpdateInfoFromServer::parseRcvMessageFromServer()
{
	const char* pMessage = m_strFileList.c_str();

	cocos2d::CCLog("download step m_strFileList %s",pMessage);
	if (pMessage == NULL || pMessage == "" || pMessage == "error")
	{
		return false;
	}

	Json::Value jsonValue = Json::Value();
	if (!parseJsonStr(pMessage, jsonValue)) 
	{
		CCLOGERROR("UpdateInfoFromServer Error: %s",pMessage);
		return false;
	}

	Json::Value jsonUrlArry = jsonValue["cdn"];
	Json::Value apkArray = jsonValue["apk"];
    Json::Value serverArray = jsonValue["servers"];

	if (false == apkArray.isNull())
	{
		m_apkUrl = apkArray.asString();
	}

	for( int i = 0; i < jsonUrlArry.size(); i++ )
	{
		const Json::Value cdnArray = jsonUrlArry[i];

		if (false == cdnArray.isNull())
		{
			m_lstCdnUrl.push_back(cdnArray.asString());
			//removeSubStringFromString("\"",m_cdnUrl);
			//removeSubStringFromString("\n",m_cdnUrl);

			/*		if (false == this->parseServerInfo(serverArray))
			{
			return false;
			}	*/	
		}
	}

	return true;
}

void UpdateInfoFromServer::removeSubStringFromString(std::string subString,std::string &sourceString)
{
	int pos = sourceString.find(subString);
	while(pos != std::string::npos)
	{
		sourceString = sourceString.replace(pos,1,"");
		pos = sourceString.find(subString);
	}
}

bool UpdateInfoFromServer::parseServerInfo(const Json::Value &serverInfo)
{
	if (serverInfo.isNull())
	{
		return false;
	}
	const Json::Value serverSubInfo = serverInfo["server"];
	if (serverInfo.isNull())
	{
		return false;
	}
	if (m_vecServerInfo.size() > 0)
	{
		m_vecServerInfo.clear();
	}
	for (int i=0; i<serverSubInfo.size(); i++) 
	{
		const Json::Value attributeInfo = serverSubInfo[i]["@attributes"];
		if (attributeInfo.isNull())
		{
			m_vecServerInfo.clear();
			return false;
		}
		else
		{
			std::string serverName = attributeInfo["ServerName"].asString();
			std::string serverIp = attributeInfo["ServerIP"].asString();
			std::string serverPort = attributeInfo["ServerPort"].asString();

			ServerInfo oneServerInfo;
			oneServerInfo.tServerName = serverName;
			oneServerInfo.tServerIp = serverIp;
			oneServerInfo.tServerPort = serverPort;

			m_vecServerInfo.push_back(oneServerInfo);
		}		
	}
	return true;
}