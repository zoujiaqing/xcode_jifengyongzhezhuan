
#include "AssetsManager.h"
#include "cocos2d.h"
#include "GameResourceManager.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <vector>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif

#include "md5.h"

#include "LoginManager.h"

using namespace cocos2d;
using namespace std;

#define KEY_OF_VERSION   "current-version-code"
#define KEY_OF_DOWNLOADED_VERSION    "downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME    "updatePackage.zip"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

//#define NETWORKADDRESS "http://100.1.1.106/res.ihero.61.com/"

#define NETWORKADDRESS "http://img.mama.61.com/uploadfile/ihero_file/"

int AssetsManager::_downloadPercent = 0;

AssetsManager::AssetsManager(const char* reslistUrl, const char* storagePath)
: _version("")
, _reslist("")
, _reslistUrl(reslistUrl)
, _curl(NULL)
{
    _storagePath = storagePath;
    //checkStoragePath();
    
    //pthread_mutex_init(&_mutex, NULL);
    _status = NetworkOperationStatusReady;
    _delegate = NULL;
    
    _thread = NULL;
}

void AssetsManager::checkStoragePath()
{
    if (_storagePath.size() > 0 && _storagePath[_storagePath.size() - 1] != '/')
    {
        _storagePath.append("/");
        CCLOG("storagePath:%s\n", _storagePath.c_str());
    }
}

NetworkOperationDelegate *AssetsManager::getDelegate()
{
    return _delegate;
}

void AssetsManager::setDelegate(NetworkOperationDelegate *delegate)
{
    _delegate = delegate;
}

static size_t getVersionCode(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    string *version = (string*)userdata;
    version->append((char*)ptr, size * nmemb);
    return (size * nmemb);
}

static size_t getReslist(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    string *reslist = (string*)userdata;
    reslist->append((char*)ptr, size * nmemb);
    return (size * nmemb);
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    AssetsManager::_downloadPercent = (int)(nowDownloaded/totalToDownload*100);
    CCLOG("downloading... %d%%", AssetsManager::_downloadPercent);
    return 0;
}

#ifdef WIN32
static DWORD WINAPI mHandler(LPVOID sId) {
    // make a copy of the parameter for convenience
    reinterpret_cast<AssetsManager *>(sId)->execute();
	return 0;
    // now do something with id
}
#endif

void AssetsManager::start(const std::string& newVersion)
{
    //assert(_status == NetworkOperationStatusReady);
    _status = NetworkOperationStatusRunning;
    _newVersion = newVersion;
    
    _reslistUrl = newVersion;
    
    downloadReslist();
    
    /*
#ifndef WIN32
    pthread_create(&_thread, 0,  &AssetsManager::startThread, this);
#else
	_thread = CreateThread(NULL, NULL, mHandler, this, NULL, NULL);
#endif
*/
    //pthread_create(&_thread, 0, &AssetsManager::startThread, this);
}

void * AssetsManager::startThread(void *operation)
{
    reinterpret_cast<AssetsManager *>(operation)->execute();
    return NULL;
}

void AssetsManager::execute()
{
    // pthread_mutex_lock(&_mutex);
    
     _version = _newVersion;
     downloadReslist();
        return;
    
     //anlaysReslistMD();
    
     //pthread_mutex_unlock(&_mutex);
}

bool AssetsManager::downloadReslist()
{
    //_reslistUrl =  NETWORKADDRESS;  //"http://100.1.1.106/res.ihero.61.com/";
    //_reslistUrl.append(_version);
    //_reslistUrl.append("/");
    //_reslistUrl.append("reslist");
    
    _curl = curl_easy_init();
    if (! _curl)
    {
        CCLOG("can not init curl");
		if (_delegate)
		{
			_delegate->operationDidFail(this);
		}
        return false;
    }
    
    unsigned int channelId = LoginManager::Get()->getChannelID();
    char channelStr[20];
    sprintf(channelStr,"%d",channelId);
    
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _reslistUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, channelStr);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, getReslist);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_reslist);
    res = curl_easy_perform(_curl);
    
    if (res != 0)
    {
        CCLOG("can not get version file content, error code is %d", res);
        curl_easy_cleanup(_curl);
		if (_delegate)
		{
			_delegate->operationDidFail(this);
		}
		
        return false;
    }
    
    if (_status == NetworkOperationStatusRunning)
    {
        _status = NetworkOperationStatusFinished;
        if(_delegate)
            _delegate->operationDidFinish(this, _reslist.c_str());
    }
    
    return true;
}

bool AssetsManager::downLoad(const char* packageUrl)
{
    // Create a file to save package.
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    FILE *fp = fopen(outFileName.c_str(), "wb");
    if (! fp)
    {
        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
    
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, packageUrl);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    if (res != 0)
    {
        CCLOG("error when download package");
        fclose(fp);
        _status = NetworkOperationStatusFailed;
        if(_delegate)
            _delegate->operationDidFail(this);
        return false;
    }
    
    CCLOG("succeed downloading package %s", packageUrl);
    
    if (_status == NetworkOperationStatusRunning)
    {
        _status = NetworkOperationStatusFinished;
        if(_delegate)
            _delegate->operationDidFinish(this, "");
    }
    fclose(fp);
    return true;
}

bool AssetsManager::anlaysReslistMD()
{
    if(_reslist.size() == 0)
        return false;
    
    std::string reslist = _storagePath;
    reslist += "/reslist";
    
	FILE *f = fopen(reslist.c_str(), "wb");
	if(f == NULL)
	{
		CCLOG("Save version failed");
		CCLog(strerror(errno));
		return false;
	}
    
	size_t size = _reslist.length();
	fwrite(_reslist.c_str(), 1, size, f);
	fclose(f);
    
    f = fopen(reslist.c_str(), "rb");
	if(f == NULL)
	{
		CCLOG("ERROR! UpdateInfoFile not exists!");
		CCLog(strerror(errno));
		return false;
	}
    
	size = 0;
    
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
    
	//empty file, no update
	if(size == 0)
	{
		CCLOG("Don't need to update.");
		fclose(f);
		return false;
	}
    
	char fileMD5[33];
	fread(fileMD5, 1, 33, f);
	fileMD5[32] = 0;
    
	char* data = new char[size - 33 + 1];
	fread(data, 1, size - 33, f);
	data[size - 33] = 0;
	fclose(f);
    
	// check MD5
	if(!md5Check(fileMD5, data, size - 33))
	{
		CCLOG("MD5 not matches!");
		return false;
	}
    
    stringstream strStream;
	strStream << data;
	delete[] data;
    
    return downloadZip(strStream);
}

bool AssetsManager::downloadZip(stringstream& strStream)
{
	//get the version
    std::string curversion ="";
	strStream >> curversion;
    _version = getVersion();
    if(curversion == _version)
    {
        AssetsManager::_downloadPercent = 100;
        return false;
    }

    _version = curversion;
    setVersion(_version.c_str());
    
    while(!strStream.eof())
    {
        string op;
        strStream >> op;
        if(op == "add")
        {
			string resMD5;
			string url;
			strStream >> resMD5;
			strStream >> url;
            
            string zipfileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
            //url = "http://10.1.1.106/res.ihero.61.com/" + url;
            url = NETWORKADDRESS + url;
            
			int times = 10;
			bool downloadDone = false;
			bool md5Match = false;
            
            do {
                downloadDone = downLoad(url.c_str());
                md5Match = checkZipMD5(resMD5.c_str(), zipfileName.c_str());
                times--;
            } while ((!downloadDone || !md5Match) && times);
            
            if((!downloadDone || !md5Match) && !times)
			{
				CCLOG("Timeout when downloading zip file");
                return false;
			}
            
            GameResourceManager::sharedManager()->uncompress(_storagePath, TEMP_PACKAGE_FILE_NAME);
            // Delete unloaded zip file.
            if (remove(zipfileName.c_str()) != 0)
            {
                CCLOG("can not remove downloaded zip file");
            }
        }
        
        else if(op == "del")
		{
			string relativePath;
			strStream >> relativePath;
            std::string storedPath = _storagePath + relativePath;
			remove(storedPath.c_str());
		}
		else
		{
			continue;
		}
    }
    return true;
}

const char* AssetsManager::getStoragePath() const
{
    return _storagePath.c_str();
}

void AssetsManager::setStoragePath(const char *storagePath)
{
    _storagePath = storagePath;
    checkStoragePath();
}

string AssetsManager::getVersion()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
}

void AssetsManager::deleteVersion()
{
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, "");
}

void AssetsManager::setVersion(const char *verison)
{
    _version = verison;
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, _version);
    CCUserDefault::sharedUserDefault()->flush();
}

bool AssetsManager::checkZipMD5(const char *srcMD5, const char *zipPath)
{
	FILE *f = fopen(zipPath, "rb");
	if(f == NULL)
	{
		CCLOG("Error!Cannot find zip file!");
		return false;
	}
    
	size_t size = 0;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
    
	char *data = new char[size];
	fread(data, 1, size, f);
	fclose(f);
    
	bool result = md5Check(srcMD5, data, size);
	delete[] data;
    
    if(result)
        CCLOG("checkZipMD5 zip file succedd!\n");
    
	return result;
}

bool AssetsManager::md5Check(const char *srcMD5, const char *buffer, size_t bufSize)
{
	std::string MD5Value = MD5(buffer, bufSize).toString();
	bool result = (strcmp(srcMD5, MD5Value.c_str()) == 0)? true : false;
    
	return result;
}


