#ifndef __ResouceVersionCheckFromServer__
#define __ResouceVersionCheckFromServer__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "json_res.h"
#include "vector"

class ResouceVersionCheckFromServer
{
public:
	static ResouceVersionCheckFromServer *sharedInstance();
	~ResouceVersionCheckFromServer();
	
	/**
	* Instruction : 下载配置信息表 多次尝试下载
	* @param 
	*/	
	bool downloadConfigFileList();
	const char* GetDownLoadInfoString(){return m_strFileList.c_str();}
	bool parseRcvMessageFromServer();

	bool IsNeedUpdateResources(){
		return m_isNeedUpdate;
	}
private:
	ResouceVersionCheckFromServer();
	bool downloadConfigFileListOneTime(const char* url,const char* postMessage);
	static size_t configFileDownLoadCallback(char* ptr, size_t size, size_t nmemb, void* context);
private:
	static ResouceVersionCheckFromServer* instance;
	Json::Value m_jvJsonValue;
	std::string m_strFileList;
	int m_nRepeatTimes;	
	
	bool m_isNeedUpdate;
};

#endif //__ResouceVersionCheckFromServer__
