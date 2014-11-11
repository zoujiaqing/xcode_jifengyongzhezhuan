//
//  ResourcesUpdateManager.h
//  UpateResourceDemo
//
//  Created by Delle  on 13-7-11.
//
//

#ifndef __UpdateInfoFromServer__
#define __UpdateInfoFromServer__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "json_res.h"
#include "vector"

struct OnlineServerInfo
{
	std::string  tServerName;
	std::string  tServerId;
	std::string  tRecommend;
	std::string  tStatus;
    std::string  tIp;
    int          tPort;
	std::vector<std::string> mergedServerVec;

	OnlineServerInfo():tServerName(""),tServerId(""),tIp(""),tPort(0){}
};

class ServerInfoFromServer
{
public:
	static ServerInfoFromServer *sharedInstance();
	static void pureInstance();
	~ServerInfoFromServer();
	
	/**
	* Instruction : 下载配置信息表 多次尝试下载
	* @param 
	*/	
	bool downloadConfigFileList(const char* channelId);
	std::string GetDownLoadInfoString(){return m_strFileList;}
	bool parseRcvMessageFromServer();	
	std::string GetApkUrl(){return m_apkUrl;}
	std::string GetCdnUrl(){return m_cdnUrl;}
	const std::vector<OnlineServerInfo> & GetServerInfo() {return m_vecServerInfo;}
	static void removeSubStringFromString(std::string subString,std::string &sourceString);
private:
	ServerInfoFromServer();

	/**
	* Instruction : URL download. 单次下载
	* @param 
	*/	
	bool downloadConfigFileListOneTime(const char* url,const char* postMessage);
	bool parseServerInfo(const Json::Value &serverInfo);	
	static size_t configFileDownLoadCallback(char* ptr, size_t size, size_t nmemb, void* context);
private:
	static ServerInfoFromServer* instance;
	Json::Value m_jvJsonValue;
	std::string m_strFileList;
	int m_nRepeatTimes;
	std::string m_apkUrl;
	std::string m_cdnUrl;
	std::vector<OnlineServerInfo> m_vecServerInfo;
};

#endif //__UpdateInfoFromServer__
