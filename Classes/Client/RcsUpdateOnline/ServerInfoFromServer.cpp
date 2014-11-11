#include "NetWorkConfigInfoFromServer.h"
#include <sys/stat.h>
#include <string.h>
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "curl/curl.h"
#include "json_res.h"
#include "FileManager.h"
#include "GameResourceManager.h"
#include "ServerInfoFromServer.h"
#include "LuaTinkerManager.h"
#include "ResourcesUpdateManager.h"
#include "AppDelegate.h"

#define RE_DOWNLOAD_TIMES 5

ServerInfoFromServer *ServerInfoFromServer::instance = NULL;

ServerInfoFromServer *ServerInfoFromServer::sharedInstance()
{
	if (instance == NULL) {
		instance = new ServerInfoFromServer();
	}

	return instance;
}
void ServerInfoFromServer::pureInstance()
{
	if (instance ) {
		delete instance ;
		instance = NULL ;
	}
}
ServerInfoFromServer::ServerInfoFromServer()
{
	m_nRepeatTimes = RE_DOWNLOAD_TIMES;
	m_apkUrl = "";
	m_cdnUrl = "";
}

ServerInfoFromServer::~ServerInfoFromServer()
{
}

size_t ServerInfoFromServer::configFileDownLoadCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
	/** To append content to a string */
    ServerInfoFromServer::sharedInstance()->m_strFileList.append(ptr);
	return  size*nmemb;
}


bool ServerInfoFromServer::downloadConfigFileListOneTime(const char* url,const char* postMessage)
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

    ServerInfoFromServer::sharedInstance()->m_strFileList = "";
	curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &ServerInfoFromServer::configFileDownLoadCallback);
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

bool ServerInfoFromServer::downloadConfigFileList(const char* channelId)
{
	//const char* url = "http://211.151.105.176/ahero_svc_list/listserver.php";
	//const char* url = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetServerListURL");
	const char* url = NetWorkConfigInfoFromServer::sharedInstance()->GetServerListURL();
	//char postMessage[256];
	//string postMessage ("");
    CCString * postMessage = new CCString();
	postMessage->autorelease() ;
	//memset(postMessage,'\0',sizeof(postMessage)/sizeof(char));
	//sprintf(postMessage,"channelid=%s&device_id=1",channelId);
    string device_id = ResourceUpdateManager::sharedInstance()->GetUniquedDeviceID();
	const char* svn_version = AppDelegate::GetShareAppDelegate()->GetSvnVersion();
	postMessage->initWithFormat("channelid=%s&device_id=%s&svn_version=%s",channelId,device_id.c_str(),svn_version);
	//sprintf(postMessage,"channelid=%s&device_id=%s&svn_version=%s",channelId,device_id.c_str(),svn_version);

	while (m_nRepeatTimes > 0)
	{
		if (downloadConfigFileListOneTime(url,postMessage->getCString()))
		{
			return true;
		}
		m_nRepeatTimes --;
	}
	//AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect !ServerInfoFromServer::downloadConfigFileList()");
	CCLOG("ServerInfoFromServer::downloadConfigFileList() NetWork Error");
	return false;
}

bool ServerInfoFromServer::parseRcvMessageFromServer()
{
	const char* pMessage = m_strFileList.c_str();
	if (pMessage == NULL)
	{
		return false;
	}

	Json::Value jsonValue;
	if (!parseJsonStr(pMessage, jsonValue)) 
	{
		CCLOGERROR("UpdateInfoFromServer Error: %s",pMessage);
		return false;
	}

	const Json::Value serverArray = jsonValue["server"];

	if (false == this->parseServerInfo(serverArray))
	{
		return false;
	}

	return true;
}

void ServerInfoFromServer::removeSubStringFromString(std::string subString,std::string &sourceString)
{
	int pos = sourceString.find(subString);
	while(pos != std::string::npos)
	{
		sourceString = sourceString.replace(pos,1,"");
		pos = sourceString.find(subString);
	}
}

bool ServerInfoFromServer::parseServerInfo(const Json::Value &serverInfo)
{
	if (serverInfo.isNull())
	{
		return false;
	}
	const Json::Value serverSubInfo = serverInfo;
	if (serverInfo.isNull())
	{
		return false;
	}
	if (m_vecServerInfo.size() > 0)
	{
		m_vecServerInfo.clear();
	}
    if (serverSubInfo.size() == 1)
    {
        const Json::Value attributeInfo = serverSubInfo["@attributes"];
        if (attributeInfo.isNull())
		{
			m_vecServerInfo.clear();
			return false;
		}
		else
		{
			std::string serverName = attributeInfo["ServerName"].asString();
			std::string serverId = attributeInfo["ServerID"].asString();
            std::string recommend = attributeInfo["Recommend"].asString();
			std::string status = attributeInfo["Status"].asString();
            std::string serverIp = attributeInfo["ServerIP"].asString();
			std::string mergeInfo = attributeInfo["merge"].asString();
            int serverPort = atoi(attributeInfo["ServerPort"].asString().c_str());
			
			OnlineServerInfo oneServerInfo;
			oneServerInfo.tServerName = serverName;
			oneServerInfo.tServerId = serverId;
			oneServerInfo.tRecommend = recommend;
			oneServerInfo.tStatus = status;
            oneServerInfo.tIp = serverIp;
            oneServerInfo.tPort = serverPort;
            
			if(mergeInfo != "0" && mergeInfo != "")
			{
				int currentPos = 0;
				while(currentPos != mergeInfo.npos)
				{
					int pos = mergeInfo.find_first_of(';',currentPos);
					if(pos != mergeInfo.npos)
					{
						std::string serverStr = mergeInfo.substr(currentPos,pos-currentPos);
						oneServerInfo.mergedServerVec.push_back(serverStr);
						currentPos = pos+1;
					}
					else
					{
						std::string serverStr = mergeInfo.substr(currentPos,mergeInfo.length()-currentPos);
						oneServerInfo.mergedServerVec.push_back(serverStr);
						currentPos = pos;
					}
				}
			}
			
			m_vecServerInfo.push_back(oneServerInfo);
		}
    }
    else
    {
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
                std::string serverId = attributeInfo["ServerID"].asString();
				std::string recommend = attributeInfo["Recommend"].asString();
				std::string status = attributeInfo["Status"].asString();
                std::string serverIp = attributeInfo["ServerIP"].asString();
				std::string mergeInfo = attributeInfo["merge"].asString();
                int serverPort = atoi(attributeInfo["ServerPort"].asString().c_str());

                OnlineServerInfo oneServerInfo;
                oneServerInfo.tServerName = serverName;
                oneServerInfo.tServerId = serverId;
				oneServerInfo.tRecommend = recommend;
				oneServerInfo.tStatus = status;
                oneServerInfo.tIp = serverIp;
                oneServerInfo.tPort = serverPort;

				if(mergeInfo != "0" && mergeInfo != "")
				{
					int currentPos = 0;
					while(currentPos != mergeInfo.npos)
					{
						int pos = mergeInfo.find_first_of(';',currentPos);
						if(pos != mergeInfo.npos)
						{
							std::string serverStr = mergeInfo.substr(currentPos,pos-currentPos);
							oneServerInfo.mergedServerVec.push_back(serverStr);
							currentPos = pos+1;
						}
						else
						{
							std::string serverStr = mergeInfo.substr(currentPos,mergeInfo.length()-currentPos);
							oneServerInfo.mergedServerVec.push_back(serverStr);
							currentPos = pos;
						}
					}
				}

                m_vecServerInfo.push_back(oneServerInfo);
            }		
        }
    }
	
	return true;
}