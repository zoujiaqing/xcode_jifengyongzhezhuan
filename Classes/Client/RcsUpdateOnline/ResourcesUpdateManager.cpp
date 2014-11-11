//
//  ResourcesUpateManager.cpp
//  UpateResourceDemo
//
//  Created by Delle  on 13-7-11.
//
//

#include <sys/stat.h>
#include <string.h>
#include "ResourcesUpdateManager.h"
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "curl/curl.h"
#include "json_res.h"
#include "FileManager.h"
#include "GameResourceManager.h"
#include "UpdateInfoFromServer.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
#include "AspriteManager.h"

#include "util.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <direct.h>
#include <io.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "JniHelper.h"

//#include <sys/stat.h>
//#include <sys/types.h>

//#include <errno.h>
//#include <string.h>
//#include <unistd.h>
//#include <netdb.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>


#define REQ_TIMES 5
#define FILE_LIST "fileList.data"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NcSDKManager.h"
#endif

using namespace cocos2d;

extern int CheckDownLoadFileNumsCallBack(int nums);

ResourceUpdateManager *ResourceUpdateManager::instance = NULL;

std::vector <std::string> split(std::string the_string,std::string delimiter)
{
    std::vector <std::string>  tempor_vector;
    size_t forwarder1 = 0;
    size_t forwarder2 = 0;
    forwarder1 = the_string.find(delimiter, forwarder1);
    while(forwarder1!=std::string::npos)
    {
        forwarder1 ++;
        tempor_vector.push_back(the_string.substr(forwarder2,forwarder1-forwarder2-1));
        forwarder2 = forwarder1;
        forwarder1 = the_string.find(delimiter, forwarder1);
    }
    if(forwarder1!=forwarder2)
        tempor_vector.push_back(the_string.substr(forwarder2,forwarder1-forwarder2-1));
    return tempor_vector;
}

ResourceUpdateManager *ResourceUpdateManager::sharedInstance()
{
    if (instance == NULL) {
        instance = new ResourceUpdateManager();
    }
    
    return instance;
}
void ResourceUpdateManager::pureInstance()
{
	if (instance ) {
		delete instance ;
		instance = NULL ;
	}
}
ResourceUpdateManager::ResourceUpdateManager():m_iRequestTimes(5),m_strVersion(""),m_jvJsonValue(NULL),
	m_bVerifyAll(false),m_iHasUpdateNum(0),m_toUpdateAmount(0),m_toCheckingCallback(NULL),m_updateSize(0),m_iUpdateNum(0)
{    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_updateResourceDirection = "UpdateFile";
#else
	m_updateResourceDirection = ".";
#endif
}

ResourceUpdateManager::~ResourceUpdateManager()
{
}

int ResourceUpdateManager::callbackRes(pResourceDownloadCallback callback, int hasUpdateNum, bool updateSuccess, bool isComplete)
{
    if (callback) {
        callback(hasUpdateNum,updateSuccess, isComplete);
    }
    return 0;
}

int ResourceUpdateManager::callbackPreDownload(pCheckingCallback callback, int updateNum)
{
    if (callback) {
        callback(updateNum);
    }
    return 0;
}


const char* ResourceUpdateManager::getVersion()
{
    return m_strVersion.c_str();
}

#pragma mark url request methods
bool ResourceUpdateManager::downloadResFileList(const char* url)
{
    if (url == NULL) 
	{
        return false;
    }
    
    std::string strHtml = "";
    std::string strRetData = "";    
	m_strFileList = "";
    
    CURLcode curlRes;
    CURL* easyCurl = curl_easy_init();
    if (easyCurl == NULL)
	{
        return false;
    }
    curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &ResourceUpdateManager::fileListCallback);
	curl_easy_setopt(easyCurl, CURLOPT_WRITEDATA, &strHtml);
	curl_easy_setopt(easyCurl, CURLOPT_URL,url);
	curl_easy_setopt(easyCurl, CURLOPT_FOLLOWLOCATION, true);
	//curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,30);
	curlRes = curl_easy_perform(easyCurl);
	curl_easy_cleanup(easyCurl);
	bool result = (curlRes == CURLE_OK);
    
	return result;
}

bool ResourceUpdateManager::downloadResource(const char* url,const char* fileName, const char* fileMode)
{
    std::string updateInfoFilePath = ResourceUpdateManager::sharedInstance()->writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    
    makeDir(updateInfoFilePath.c_str());
    
    updateInfoFilePath.append("/");
    std::string strFileName = fileName;
    std::vector<std::string> vFileName = split(strFileName, "/");
    size_t vectSize = vFileName.size();
    
    /** To check whether is directory */
    if (vFileName.size() > 1) {
        for (std::vector<std::string>::size_type i=0; i<vectSize-1; i++) {
            updateInfoFilePath.append(vFileName[i]);
            makeDir(updateInfoFilePath.c_str());
            updateInfoFilePath.append("/");
        }
        updateInfoFilePath.append(vFileName[vectSize-1]);
    }
    else{
        updateInfoFilePath.append(fileName);
    }
    
    //cocos2d::CCLog("RcsUpdate %s",fileName);
    
    if (NULL == updateInfoFilePath.c_str()) {
        return false;
    }
    if (url == NULL) {
        return false;
    }

    
    FILE *f = fopen(updateInfoFilePath.c_str(), fileMode);
    if (NULL == f) {
        return false;
    }
    std::string strRetData = "";
    
    CURLcode curlRes;
    CURL* easyCurl = curl_easy_init();
    if (easyCurl == NULL) {
        return false;
    }
    curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &ResourceUpdateManager::updateCallback);
	curl_easy_setopt(easyCurl, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(easyCurl, CURLOPT_URL, url);
	curl_easy_setopt(easyCurl, CURLOPT_FOLLOWLOCATION, true);
	//curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,30);
	curlRes = curl_easy_perform(easyCurl);
	curl_easy_cleanup(easyCurl);
    fclose(f);
	bool result = (curlRes == CURLE_OK);

    
	return result;
}

size_t ResourceUpdateManager::fileListCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
    /** To append content to a string */
    ResourceUpdateManager::sharedInstance()->m_strFileList.append(ptr);
    return  size*nmemb;
}

size_t ResourceUpdateManager::updateCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
    /** Write download content to file */
    if (NULL == context) 
	{
        return 0;
    }
    fwrite(ptr, size, nmemb, (FILE *)context);
    return size*nmemb;
}
 
#pragma mark file operations
void ResourceUpdateManager::removeFile(const char *fileName)
{
    if (fileName[strlen(fileName) - 1] == '/') {
        removeDir(fileName);
    }else{
        remove(fileName);
    }
}

int ResourceUpdateManager::CreateDir(const   char   *sPathName)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    char   DirName[256];
    strcpy(DirName,   sPathName);
    int   i,len   =   strlen(DirName);
    if(DirName[len-1]!='/')
        strcat(DirName,   "/");
    
    len   =   strlen(DirName);
    
    for(i=1;   i<len;   i++)
    {
        if(DirName[i]=='/')
        {
            DirName[i]   =   '\0';
            if(   access(DirName,   NULL)!=0   )
            {
                if(mkdir(DirName,0755)==-1)
                {
                    return   -1;
                }
            }
            DirName[i]   =   '/';
        }
    }
#endif
    
    return   0;
}

void ResourceUpdateManager::makeDir(const char *dirName)
{
    if(access(dirName, 0) == 0)
	{
		return;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	mkdir(dirName);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::CCLog("enter mkdir");
	//CreateDir(dirName);
    //LoginManager::Get()->createDirection(dirName);
    ResourceUpdateManager::CreateDir(dirName);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//mkdir(dirName, S_IRWXU);
    ResourceUpdateManager::CreateDir(dirName);
#endif
}


void ResourceUpdateManager::removeDir(const char *dirName)
{
	if(access(dirName, 0) != 0)
	{
		return;
	}
    
	rmdir(dirName);
}

const char* ResourceUpdateManager::writeablePathFromRelativePath(const char *path)
{
	return GameResourceManager::sharedManager()->writablePathFromRelativePath(path);
}

const char* ResourceUpdateManager::storedFullPathFromRelativePath(const char *path)
{
	return GameResourceManager::sharedManager()->storedFullPathFromRelativePath(path);
}

bool ResourceUpdateManager::loadFileList()
{
    std::string fileListPath = writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    fileListPath.append("/");
    fileListPath.append(FILE_LIST);
    
    FILE *f = fopen(fileListPath.c_str(), "rb");
    if (NULL == f ) {
        //fclose(f);
        return false;
    }
    fclose(f);
    
    unsigned long fileSize = 0;
    unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(fileListPath.c_str(), "rb", &fileSize);
    
    if (NULL == buffer) {
        return false;
    }
    
    char* pszFileList = new char[fileSize+1];
    memcpy(pszFileList, buffer, fileSize);
    pszFileList[fileSize] = 0;
    Json::Value jsonValue;
    if (!parseJsonStr(pszFileList, jsonValue)) {
        return false;
    }
    m_jvFileList = jsonValue;
    delete []pszFileList;
    
    CC_SAFE_DELETE_ARRAY(buffer);
    return true;
}

bool ResourceUpdateManager::saveFileList()
{
    std::string updateInfoFilePath = ResourceUpdateManager::sharedInstance()->writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    makeDir(updateInfoFilePath.c_str());
    updateInfoFilePath.append("/");
    updateInfoFilePath.append(FILE_LIST);
    
    FILE *f = fopen(updateInfoFilePath.c_str(), "wb");
    if (NULL == f) 
	{
        return  false;
    }

	m_jvJsonValue["version"] = m_strVersion;
	m_jvJsonValue["full_package"] = getUpdateSize();
	m_jvJsonValue["verify_files"] = m_bVerifyAll;
	Json::Value filelist = m_jvJsonValue;
	size_t size = filelist.toStyledString().length();
	fwrite(filelist.toStyledString().c_str(), 1, size, f);

    fclose(f);
    return true;
}

bool ResourceUpdateManager::loadTmpFileList()
{
    std::string fileListPath = writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    fileListPath.append("/");
    fileListPath.append("tmp.data");
    
    FILE *f = fopen(fileListPath.c_str(), "rb");
    if (NULL == f ) {
        //fclose(f);
        return false;
    }
    fclose(f);
    
    unsigned long fileSize = 0;
    unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(fileListPath.c_str(), "rb", &fileSize);
    
    if (NULL == buffer) {
        return false;
    }
    
    char* pszFileList = new char[fileSize+1];
    memcpy(pszFileList, buffer, fileSize);
    pszFileList[fileSize] = 0;
    Json::Value jsonValue;
    if (!parseJsonStr(pszFileList, jsonValue)) {
        return false;
    }
    m_jvTmpFileList = jsonValue;
    delete []pszFileList;
    
    CC_SAFE_DELETE_ARRAY(buffer);
    return true;

}

bool ResourceUpdateManager::saveFile(const char *fileName, const char *content)
{
    std::string updateInfoFilePath = ResourceUpdateManager::sharedInstance()->writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    makeDir(updateInfoFilePath.c_str());
    updateInfoFilePath.append("/");
    updateInfoFilePath.append(fileName);
    
    // To  do file operations.
    FILE *f = fopen(updateInfoFilePath.c_str(), "wb");
    if (NULL == f) {
        fclose(f);
        return false;
    }
    
    size_t size = strlen(content);
    fwrite(content, 1, size, f);
    fclose(f);
    return true;
}

bool ResourceUpdateManager::verifyFile(const char *fileName, const char *hash)
{
    std::string path = writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    path.append("/");
    path.append(fileName);
    
    std::string localHash;
    if (!getFileHash(path.c_str(), localHash)) 
	{
        return true;
    } 
    
    std::string strHash = hash;
    
    if (localHash == strHash) 
	{
        return false;
    }
        
    return true;
}

#pragma mark update operations

bool ResourceUpdateManager::isNeedUpdate(const char *url, pCheckingCallback callback)
{
    if (NULL == url || strlen(url) == 0)
	{
        return false;
    }
    
    m_strFileDownloadUrl = url;
    bool downloadFlag = false;
    int time = REQ_TIMES;
    std::string fileListUrl = url;
    fileListUrl.append("/data.json");
    
    //  Download file list.
    do 
	{
        downloadFlag = downloadResFileList(fileListUrl.c_str());
        time--;
    } while (!downloadFlag && time>0);
    
    if (!downloadFlag && time<=0) 
	{
        return false;
    }
    
    //  Load temp file.
    loadTmpFileList();
    
    //  Load local file list.
    bool ret = false;
    if (!loadFileList())
	{
        ret = true;
    }
    
    //  Parse file list.
    Json::Value jsonValue;
    if (!parseJsonStr(m_strFileList.c_str(), jsonValue)) 
	{
        return false;
    }
    const Json::Value fileArray = jsonValue["files"];
    ResourceUpdateManager::sharedInstance()->m_bVerifyAll = jsonValue["verify_files"].asBool();
	if ((ResourceUpdateManager::sharedInstance()->m_strVersion) == "")
		ResourceUpdateManager::sharedInstance()->m_strVersion = jsonValue["version"].asString();
	string s = m_strVersion;
    //  Check file list.
    unsigned int size = 0;
    if (checkFileList(m_jvFileList,m_jvTmpFileList, jsonValue, m_vUpdateFileList, m_vNeedCheckingFileList, size)) 
	{
        ret = true;
    }

	addUpdateSize(size);

    //  Finish to check file list.
    int updateNum = m_vUpdateFileList.size() + m_vNeedCheckingFileList.size();
	m_iUpdateNum = updateNum;
    
    return ret;
}
/* 将m_updateSize的值增加var*/
void ResourceUpdateManager::addUpdateSize( unsigned int var )
{
	m_updateSize += var;
}

bool ResourceUpdateManager::updateResource(pResourceDownloadCallback callback)
{
    size_t needListSize = m_vUpdateFileList.size();
    size_t checkingListSize = m_vNeedCheckingFileList.size();

    if (needListSize<1 && checkingListSize<1) 
	{
        return true;
    }
    
    bool downloadFlag = false;
    int time = REQ_TIMES;
    
    if (needListSize >= 1 && m_iHasUpdateNum < needListSize) 
	{   /** To load new files */
        Json::Value fileObj = m_vUpdateFileList[m_iHasUpdateNum];
        std::string str = fileObj["hash"].asString();
        //std::string url = ResourceUpdateManager::sharedInstance()->m_strFileDownloadUrl;
		std::string url = fileObj["fileDownloadUrl"].asString();
        url.append("/");
        url.append(str.c_str());
        std::string strFileName = fileObj["name"].asString();
        
        //  To download resource (5 times try, if not succ.)
        do 
		{
            downloadFlag = ResourceUpdateManager::sharedInstance()->downloadResource(url.c_str(),strFileName.c_str(),"wb");
            time--;
            if (ResourceUpdateManager::sharedInstance()->m_bVerifyAll) 
			{
                if (!verifyFile(fileObj["name"].asString().c_str(), fileObj["hash"].asString().c_str())) 
				{
                    m_jvTmpFileList.append(fileObj);
                    downloadFlag = true;
                    break;
                }
                else
				{
                    downloadFlag = false;
                }
            }
            else if (fileObj["verify"].asBool())
			{   /** Verify the verify needed files */
                if (!verifyFile(fileObj["name"].asString().c_str(), fileObj["hash"].asString().c_str())) 
				{
                    m_jvTmpFileList.append(fileObj);
                    downloadFlag = true;
                    break;
                }
                else
				{
                    downloadFlag = false;
                }
            }
//            cocos2d::CCLog("download url:%s",url.c_str());           
//            cocos2d::CCLog("download flag:%d, times:%d",downloadFlag, time);         
        } while (!downloadFlag && time>0);
        
        if (!downloadFlag && time<=0) 
		{
            callbackRes(callback, m_iHasUpdateNum, false, false);
            return false;
        }
		else
		{
            m_jvTmpFileList.append(fileObj);
            saveFile("tmp.data", m_jvTmpFileList.toStyledString().c_str());
            m_iHasUpdateNum++;
            if (m_iHasUpdateNum == (needListSize+checkingListSize)) 
			{   /** Update complete */
                saveFileList();
                std::string updateInfoFilePath = ResourceUpdateManager::sharedInstance()->writeablePathFromRelativePath(m_updateResourceDirection.c_str());
                makeDir(updateInfoFilePath.c_str());
                updateInfoFilePath.append("/");
                updateInfoFilePath.append("tmp.data");
                removeFile(updateInfoFilePath.c_str());
                callbackRes(callback, m_iHasUpdateNum, true, true);
            }
            else
			{
                callbackRes(callback, m_iHasUpdateNum, true, false);
            }
            return true;
        }
    }
    
    time = REQ_TIMES;
    if (checkingListSize>=1 && m_iHasUpdateNum<checkingListSize+needListSize)
	{   /** To load mod files */
        Json::Value fileObj = m_vNeedCheckingFileList[m_iHasUpdateNum-needListSize];
        std::string str = fileObj["hash"].asString();
        std::string url = ResourceUpdateManager::sharedInstance()->m_strFileDownloadUrl;
        url.append("/");
        url.append(str.c_str());
        std::string strFileName = fileObj["name"].asString();
        
        //  To download resource (5 times try, if not succ.)
        do 
		{
            downloadFlag = ResourceUpdateManager::sharedInstance()->downloadResource(url.c_str(),strFileName.c_str(),"wb");
            time--;
            if (ResourceUpdateManager::sharedInstance()->m_bVerifyAll) 
			{
                if (!verifyFile(fileObj["name"].asString().c_str(), fileObj["hash"].asString().c_str()))
				{
                    m_jvTmpFileList.append(fileObj);
                    downloadFlag = true;
                    break;
                }
                else
				{
                    downloadFlag = false;
                }
            }
            else if (fileObj["verify"].asBool()) 
			{   /** Verify the verify needed files */
                if (!verifyFile(fileObj["name"].asString().c_str(), fileObj["hash"].asString().c_str())) 
				{
                    m_jvTmpFileList.append(fileObj);
                    downloadFlag = true;
                    break;
                }
                else
				{
                    downloadFlag = false;
                }
            }
//             cocos2d::CCLog("download url:%s",url.c_str());
//             cocos2d::CCLog("download flag:%d, times:%d",downloadFlag, time);
            
        } while (!downloadFlag && time>=0);
        
        if (!downloadFlag && time<=0)
		{
            callbackRes(callback, m_iHasUpdateNum, false, false);
            return false;
        }
        else
		{
            m_jvTmpFileList.append(fileObj);
            saveFile("tmp.data", m_jvTmpFileList.toStyledString().c_str());
            m_iHasUpdateNum++;
            if (m_iHasUpdateNum == (needListSize+checkingListSize))
			{   /** Update complete */
                saveFileList();
                std::string updateInfoFilePath = ResourceUpdateManager::sharedInstance()->writeablePathFromRelativePath(m_updateResourceDirection.c_str());
                makeDir(updateInfoFilePath.c_str());
                updateInfoFilePath.append("/");
                updateInfoFilePath.append("tmp.data");
                removeFile(updateInfoFilePath.c_str());
                callbackRes(callback, m_iHasUpdateNum,true,true);
            }
            else
			{
                callbackRes(callback, m_iHasUpdateNum, true,false);
            }
            return true;
        }
    }

    return true;
}

bool ResourceUpdateManager::checkFileList(const Json::Value fileList, const Json::Value tmpFileList, const Json::Value downloadFileList, std::vector<Json::Value> &updateFileList, std::vector<Json::Value> &needCheckingFileList, unsigned int& size)
{
    bool ret = false;
    Json::Value jsonLocalArray = fileList["files"];
    Json::Value jsonDownloadArray = downloadFileList["files"];
    Json::Value jsonTmpArray = tmpFileList;

    size = 0;
    for (int i=0; i<jsonDownloadArray.size(); i++) 
	{
        bool flag = false;
            
		if (jsonDownloadArray[i]["verify"].asBool()) 
		{
			bool tmpFlag = false;

			//  To check whether exsist in tmp filelist.
			for (int k = 0; k < jsonTmpArray.size(); k++) 
			{
				if (jsonDownloadArray[i]["hash"].asString() == jsonTmpArray[k]["hash"].asString()) 
				{
					tmpFlag = true;
					break;
				}
			}                 
			//  The file had mod by user.
			if (!tmpFlag && verifyFile(jsonDownloadArray[i]["name"].asString().c_str(), jsonDownloadArray[i]["hash"].asString().c_str())) 
			{
				needCheckingFileList.push_back(jsonDownloadArray[i]);
				size += jsonDownloadArray[i]["size"].asInt();
				ret = true;
			}
			flag = true;
			break;
        }
        if (!flag) 
		{
            bool tmpFlag = false;          

            //  To check whether exist in tmp filelist.
            for (int k=0; k<jsonTmpArray.size(); k++) 
			{
                if (jsonDownloadArray[i]["hash"].asString() == jsonTmpArray[k]["hash"].asString())
				{
                    tmpFlag = true;
					break;
                }
            }
			// check whether exist in updateFileList 
			for (vector<Json::Value>::iterator iter = updateFileList.begin(); 
				iter != updateFileList.end(); ++iter)
			{
				if ((*iter)["name"].asString() == jsonDownloadArray[i]["name"].asString())
				{
					tmpFlag = true;
					break;
				}
			}
            if (!tmpFlag)
			{
				jsonDownloadArray[i]["fileDownloadUrl"] = m_strFileDownloadUrl;
                updateFileList.push_back(jsonDownloadArray[i]);
                size += jsonDownloadArray[i]["size"].asInt();
                ret = true;
				 
				if (m_jvJsonValue == NULL) 
				{
					ResourceUpdateManager::sharedInstance()->m_jvJsonValue = Json::Value();
				}
				ResourceUpdateManager::sharedInstance()->m_jvJsonValue["files"].append(jsonDownloadArray[i]);      
			}
        }
    }
    
    //  To remove tmp file.
    std::string updateInfoFilePath = ResourceUpdateManager::sharedInstance()->writeablePathFromRelativePath(m_updateResourceDirection.c_str());
    makeDir(updateInfoFilePath.c_str());
    updateInfoFilePath.append("/");
    updateInfoFilePath.append("tmp.data");
    removeFile(updateInfoFilePath.c_str());
    
    return ret;
}

void ResourceUpdateManager::onConfirmButtonClicked(CCObject* obj)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	extern void openWebUrl(std::string urlstring);
	std::string apk_url = UpdateInfoFromServer::sharedInstance()->GetApkUrl();
	if (apk_url != "")
	{
		openWebUrl(apk_url);
	}
	CCDirector::sharedDirector()->end();

#endif
}

bool ResourceUpdateManager::HandleApkDownLoad()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//extern void openWebUrl(std::string urlstring);
	std::string apk_url = UpdateInfoFromServer::sharedInstance()->GetApkUrl();
	if (apk_url == "")
	{
		return false;
	}

	cocos2d::CCLog("ResourceUpdateManager::HandleApkDownLoad");

	return true;
#else
	return false;
#endif
}

bool ResourceUpdateManager::HandleIosUpdate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string apk_url = UpdateInfoFromServer::sharedInstance()->GetApkUrl();
	if (apk_url == "")
	{
		return false;
	}
    NcSDKManager::checkAppstoreUpdate(apk_url.c_str());
    
	return true;
#endif
	return false;
}

bool ResourceUpdateManager::GetVersionInfoFromLocal(std::string &mainVersion,std::string &subVersion)
{
	std::string filePath = writeablePathFromRelativePath(m_updateResourceDirection.c_str());
	filePath.append("/");
	filePath.append("fileList.data");

	FILE *f = fopen(filePath.c_str(), "rb");
	if (NULL == f ) {
		return false;
	}
	fclose(f);

	unsigned long fileSize = 0;
	unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), "rb", &fileSize);

	if (NULL == buffer) {
		return false;
	}

	char* pszFileList = new char[fileSize+1];
	memcpy(pszFileList, buffer, fileSize);
	pszFileList[fileSize] = 0;
	Json::Value jsonValue;
	if (!parseJsonStr(pszFileList, jsonValue)) {
		return false;
	}

	const Json::Value version_value = jsonValue["version"];
	if (version_value.isNull())
	{
		return false;
	}
	std::string version_str = version_value.asString();
	//UpdateInfoFromServer::removeSubStringFromString("\"",version_str);
	int startPos = version_str.find(".");
	int length = version_str.length();
	if (startPos == std::string::npos)
	{
		return false;
	}	
	mainVersion = version_str.substr(0,startPos);
	subVersion = version_str.substr(startPos+1,length - startPos - 1);
	
	delete []pszFileList;
	CC_SAFE_DELETE_ARRAY(buffer);

	return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string ResourceUpdateManager::jstring2str(JNIEnv* env, jstring jstr)
{
    char*   rtn   =   NULL;
    jclass   clsstring   =   env->FindClass("java/lang/String");
    jstring   strencode   =   env->NewStringUTF("GB2312");
    jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
    jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
    jsize   alen   =   env->GetArrayLength(barr);
    jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);
    if(alen   >   0)
    {
        rtn   =   (char*)malloc(alen+1);
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements(barr,ba,0);
    std::string stemp(rtn);
    free(rtn);
    return   stemp;
}
#endif

std::string ResourceUpdateManager::GetChannelIdFromJava()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::CCLog("updateRcs get channel id");
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "GetChannelId", "()Ljava/lang/String;"))
	{
        jstring value = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return jstring2str(t.env,value);
	}
#endif
	return "";
}

std::string ResourceUpdateManager::GetMainVersionFromJava()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::CCLog("updateRcs get main version");
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "GetMainVersion", "()Ljava/lang/String;"))
	{
        jstring value = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return jstring2str(t.env,value);
	}
#endif
	return "";
}

std::string ResourceUpdateManager::GetSubVersionFromJava()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::CCLog("updateRcs get sub version");
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "GetSubVersion", "()Ljava/lang/String;"))
	{
        jstring value = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return jstring2str(t.env,value);
	}
#endif
	return "";
}

std::string ResourceUpdateManager::GetUniquedDeviceID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::CCLog("updateRcs get device id");
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "GetDeviceID", "()Ljava/lang/String;"))
	{
        jstring value = (jstring)t.env->CallStaticObjectMethod (t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return jstring2str(t.env,value); 
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "0";
#endif
	return "";
}

int ResourceUpdateManager::UpdateNeedResourceByType(int type)
{
	// Note: So
	if (0 == type)
	{
		m_updateResourceDirection = "So";
	}
	// Note: commond rcs
	else if (1 == type)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		m_updateResourceDirection = "UpdateFile";
#else
		m_updateResourceDirection = ".";
#endif
	}
	
	std::string str_channelID = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetChannelID");
	cocos2d::CCLog("download step channel id %s",str_channelID.c_str());

	int nResult = 1;	
	std::string str_main_version = "";
	std::string str_sub_version = "";
	if (false == ResourceUpdateManager::sharedInstance()->GetVersionInfoFromLocal(str_main_version,str_sub_version))
	{
		str_main_version = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua", "GetMainVersion");
		str_sub_version = "0";
	}
	if (str_channelID != "" && str_main_version != "" && str_sub_version != "")
	{
		cocos2d::CCLog("download step channel channel_id main_ver sub_version %s",str_channelID.c_str(),str_main_version.c_str(),str_sub_version.c_str());
		bool downloadFlag = UpdateInfoFromServer::sharedInstance()->downloadConfigFileList(str_channelID.c_str(),str_main_version.c_str(),str_sub_version.c_str(),type);
		if (downloadFlag)
		{				
			cocos2d::CCLog("download step down suc");
			//UpdateInfoFromServer::sharedInstance()->setFileList("{\"urls\":[{\"cdn\":\"http:\\/\\/mcdn.61.com\\/resman\\/test_for_update\\/1\\/\"},{\"cdn\":\"http:\\/\\/mcdn.61.com\\/resman\\/test_for_update\\/2\\/\"},{\"cdn\":\"http:\\/\\/mcdn.61.com\\/resman\\/test_for_update\\/3\\/\"},{\"cdn\":\"http:\\/\\/mcdn.61.com\\/resman\\/test_for_update\\/4\\/\"}]}");
			if (UpdateInfoFromServer::sharedInstance()->parseRcvMessageFromServer())
			{
				cocos2d::CCLog("download step parse suc"); 
				if (ResourceUpdateManager::sharedInstance()->HandleApkDownLoad())
				{
					nResult = -1;
					cocos2d::CCLog("download step apk load ");
					AspriteManager::getInstance()->OnlyLoadNoticeAsprite();
					std::string msg = Localizatioin::getLocalization("M_UPDATE_HINT");
					TXGUI::MessageBox::AdjustPriority(true);
					TXGUI::MessageBox::Show(msg.c_str(), this, SEL_MenuHandler(&ResourceUpdateManager::onConfirmButtonClicked),
						NULL, MB_OK,ccWHITE,24);
					TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
					TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
				}
				else if (ResourceUpdateManager::sharedInstance()->HandleIosUpdate())
				{
					cocos2d::CCLog("download step ios update ");
					nResult = -1;
					//CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
					//exit(0);
#endif
				}
				else
				{
					while(1)
					{
						std::string url = UpdateInfoFromServer::sharedInstance()->GetEndOfCdnUrlList();
						if (url != "")
						{
							cocos2d::CCLog("download step url %s",url.c_str());
							if (ResourceUpdateManager::sharedInstance()->isNeedUpdate(url.c_str(),CheckDownLoadFileNumsCallBack))
							{
								nResult = 0;
							}
						}
						else
						{
							break;      // url为空即视为urllist中的所有文件下载完毕，更新完成，跳出循环
						}
					}
				}
				// 统计需要更新的文件数量
				callbackPreDownload(CheckDownLoadFileNumsCallBack, m_iUpdateNum);
			}
		}
		else
		{
			nResult = 2;
		}
	}
	return nResult;
}
