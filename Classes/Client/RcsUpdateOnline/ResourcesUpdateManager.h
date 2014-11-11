//
//  ResourcesUpdateManager.h
//  UpateResourceDemo
//
//  Created by Delle  on 13-7-11.
//
//

#ifndef __UpateResourceDemo__ResourcesUpdateManager__
#define __UpateResourceDemo__ResourcesUpdateManager__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "json_res.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "JniHelper.h"
#endif

//
//  int  检测文件列表结束，返回需要更新的文件个数。
//
typedef int(*pCheckingCallback)(int);

//
//  int     更新第N个文件.
//  bool    更新第N个文件是否成功.
//  bool    是否更新完成.
//
typedef int(*pResourceDownloadCallback)(int, bool, bool);

class ResourceUpdateManager:public cocos2d::CCLayer
{
public:
    static ResourceUpdateManager *sharedInstance();
	static void pureInstance();
    ~ResourceUpdateManager();
    bool isNeedUpdate(const char* url, pCheckingCallback callback);
    bool updateResource(pResourceDownloadCallback callback);
    const char* getVersion();
    static int CreateDir(const char* path);	
	bool HandleApkDownLoad();
	bool HandleIosUpdate();
	bool GetVersionInfoFromLocal(std::string &mainVersion,std::string &subVersion);
	/**
	* Instruction : 根据类型决定下载资源类型 0 so文件 1 资源文件
	* @param 
	* @return -1 Error 直接进游戏 0 开始下载 1成功
	*/	
	int UpdateNeedResourceByType(int type);

    std::string GetUniquedDeviceID();

	std::string GetChannelIdFromJava();
	std::string GetMainVersionFromJava();
	std::string GetSubVersionFromJava();

	std::string GetResourceUpdateDir()
	{
		return m_updateResourceDirection;
	}
	
protected:

	void onConfirmButtonClicked(CCObject* obj);

private:
    //
    //  Do checking.
    //
    void checkUpate();
    ResourceUpdateManager();
    
    //
    //  URL download.
    //
    bool loadFileList();
    bool loadTmpFileList();
    bool downloadResFileList(const char* url);
    bool downloadResource(const char* url, const char* fileName, const char* fileMode);
    static size_t fileListCallback(char* ptr, size_t size, size_t nmemb, void* context);
    static size_t updateCallback(char* ptr, size_t size, size_t nmemb, void* context);
    
    // 
    //  Callback.
    //
    int callbackRes(pResourceDownloadCallback callback, int hasUpdateNum,bool updateSuccess, bool isComplete);
    int callbackPreDownload(pCheckingCallback callback, int updateNum);
    
    //
    //  File operations.
    //
    void removeFile(const char *fileName);
	void makeDir(const char *dirName);
	void removeDir(const char *dirName);
    const char* writeablePathFromRelativePath(const char* path);
    const char*storedFullPathFromRelativePath(const char *path);
	//* 将m_updateSize的值增加var*//
	void addUpdateSize( unsigned int var );     

    bool saveFileList();
    bool saveFile(const char* fileName, const char *content);
    bool verifyFile(const char* fileName, const char* hash);
    bool checkFileList(const Json::Value fileList, const Json::Value tmpFileList, const Json::Value downloadFileList, std::vector<Json::Value> &updateFileList, std::vector<Json::Value> &needCheckingFileList, unsigned int& size);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string jstring2str(JNIEnv* env, jstring jstr);
#endif

public:
    CC_SYNTHESIZE(unsigned int, m_updateSize, UpdateSize);

private:
    static ResourceUpdateManager* instance;
    unsigned int m_iRequestTimes;
    std::string m_strVersion;
    std::string m_strSDCardPath;
    bool        m_bHasSDCard;
    bool        m_bVerifyAll;
    Json::Value m_jvJsonValue;
    std::string m_strUpdatePath;
    std::string m_strFileDownloadUrl;
    Json::Value m_jvFileList;
    std::string m_strFileList;
    std::string m_strServerUrl;
    Json::Value m_jvTmpFileList;      
    unsigned int m_iHasUpdateNum;
    std::vector<Json::Value> m_vUpdateFileList;
    std::vector<Json::Value> m_vNeedCheckingFileList;
	std::string m_updateResourceDirection;

	int m_iUpdateNum;   //更新的文件数

    int m_toUpdateAmount;
    pCheckingCallback m_toCheckingCallback;
};

#endif /* defined(__UpateResourceDemo__ResourcesUpdateManager__) */
