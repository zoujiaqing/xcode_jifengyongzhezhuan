//
//  ResourcesUpdateManager.h
//  UpateResourceDemo
//
//  Created by Delle  on 13-7-11.
//
//

#ifndef __NetWorkConfigInfoFromServer__
#define __NetWorkConfigInfoFromServer__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "json_res.h"
#include "vector"

struct NetWorkConfigInfo
{
	unsigned int tGameId;
	unsigned int tPlatformFlag;
	int tServerId; // 第三方服务器编号
	std::string tIP;
	std::string tIpPort;
	std::string tRcsDownloadUrl;
    std::string tRcsDownloadUrl2;
	std::string tServerListUrl;
	std::string tFrontStateUrl;
    std::string tNoticesUrl;
	std::string tCpId;
	std::string tThirdChannel;
	std::string tExtraData;
	std::string tAppKey;
	std::string tAppId;

	NetWorkConfigInfo():tGameId(0),tIP(""),tIpPort(""),
		tRcsDownloadUrl(""),tRcsDownloadUrl2(""),tServerListUrl(""),tFrontStateUrl(""),tNoticesUrl(""),tPlatformFlag(0){}
};

class NetWorkConfigInfoFromServer
{
public:
	static NetWorkConfigInfoFromServer *sharedInstance();
	static void puresharedInstance();
	~NetWorkConfigInfoFromServer();
	
	/**
	* Instruction : 下载配置信息表 多次尝试下载
	* @param 
	*/	
	bool downloadConfigFileList();
	const char* GetDownLoadInfoString(){return m_strFileList.c_str();}
	bool parseRcvMessageFromServer();		

	unsigned int GetUsePlatformFlag()
	{
		return mNetWorkConfigInfo.tPlatformFlag;
	}

	const char* GetGetWayIP()
	{
		return mNetWorkConfigInfo.tIP.c_str();
	}

	unsigned int GetGetWayPort()
	{
		return atoi(mNetWorkConfigInfo.tIpPort.c_str());
	}

	unsigned int GetGameID()
	{
		return mNetWorkConfigInfo.tGameId;
	}

	const char* GetRcsDownloadURL()
	{
		return mNetWorkConfigInfo.tRcsDownloadUrl.c_str();
	}

    const char* GetRcsDownloadURL2()
    {
        return mNetWorkConfigInfo.tRcsDownloadUrl2.c_str();
    }

	const char* GetServerListURL()
	{
		return mNetWorkConfigInfo.tServerListUrl.c_str();
	}

	const char* GetFrontStateURL()
	{
		return mNetWorkConfigInfo.tFrontStateUrl.c_str();
	}
    
    const char* GetNoticeURL()
    {
        return mNetWorkConfigInfo.tNoticesUrl.c_str();
    }

	int GetOtherServerId()
	{
		return mNetWorkConfigInfo.tServerId;
	}

	const char* GetCpId()
	{
		return mNetWorkConfigInfo.tCpId.c_str();
	}

	const char* GetThirdChannel()
	{
		return mNetWorkConfigInfo.tThirdChannel.c_str();
	}

	const char* GetExtraData()
	{
		return mNetWorkConfigInfo.tExtraData.c_str();
	}

	const char* GetAppKey()
	{
		return mNetWorkConfigInfo.tAppKey.c_str();
	}

	const char* GetAppId()
	{
		return mNetWorkConfigInfo.tAppId.c_str();
	}

	bool IsDownLoadContentFromServer(){
		return mIsDownLoadContentFromServer;
	}
private:
	NetWorkConfigInfoFromServer();
	bool downloadConfigFileListOneTime(const char* url,const char* postMessage);
	static size_t configFileDownLoadCallback(char* ptr, size_t size, size_t nmemb, void* context);
private:
	static NetWorkConfigInfoFromServer* instance;
	Json::Value m_jvJsonValue;
	std::string m_strFileList;
	int m_nRepeatTimes;	

	NetWorkConfigInfo mNetWorkConfigInfo;
	bool mIsDownLoadContentFromServer;
};

#endif //__NetWorkConfigInfoFromServer__
