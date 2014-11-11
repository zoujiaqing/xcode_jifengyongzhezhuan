//
//  ResoureUpdate.cpp
//  HelloLua
//
//  Created by 程小二 on 13-3-25.
//
//

#include "ResoureUpdate.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <vector>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <direct.h>
#include <io.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "SystemInfoJni.h"
#include <sys/stat.h>
#include <sys/types.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "GameResourceManager.h"

using namespace cocos2d;
using namespace std;

#define KEY_OF_REALTIMEUPDATE_A   "RealTimeUpdateA1"
#define KEY_OF_REALTIMEUPDATE_B   "RealTimeUpdateB"
#define KEY_OF_REALTIMEUPDATE_C   "RealTimeUpdateC"
#define KEY_OF_REALTIMEUPDATE_D   "RealTimeUpdateD"
#define KEY_OF_REALTIMEUPDATE_E   "RealTimeUpdateE"
#define KEY_OF_REALTIMEUPDATE_F   "RealTimeUpdateF"

#define FILENAME_REALTIMEUPDATE_A "A.zip"
#define FILENAME_REALTIMEUPDATE_B "B.zip"
#define FILENAME_REALTIMEUPDATE_C "C.zip"
#define FILENAME_REALTIMEUPDATE_D "D.zip"
#define FILENAME_REALTIMEUPDATE_E "E.zip"
#define FILENAME_REALTIMEUPDATE_F "F.zip"


ResoureUpdate* ResoureUpdate::s_pSharedResoureUpdate = NULL;
int ResoureUpdate::_downloadPercent = 0;

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    ResoureUpdate::_downloadPercent = (int)(nowDownloaded/totalToDownload*100);
    CCLog("downloading... %d%%", ResoureUpdate::_downloadPercent);
    return 0;
}

#ifdef WIN32
static DWORD WINAPI mHandler(LPVOID sId) {
    // make a copy of the parameter for convenience
    reinterpret_cast<ResoureUpdate*>(sId)->execute();
	return 0;
    // now do something with id
}
#endif

void * ResoureUpdate::startThread(void *operation)
{
    reinterpret_cast<ResoureUpdate *>(operation)->execute();
    return NULL;
}

ResoureUpdate *ResoureUpdate::sharedResoureUpdate()
{
	if(s_pSharedResoureUpdate == NULL)
	{
		s_pSharedResoureUpdate = new ResoureUpdate();
		s_pSharedResoureUpdate->init();
	}
	return s_pSharedResoureUpdate;
}

void ResoureUpdate::purgeInstance()
{
	if (s_pSharedResoureUpdate)
	{
		delete s_pSharedResoureUpdate;
		s_pSharedResoureUpdate = NULL;
	}
}

ResoureUpdate::~ResoureUpdate()
{
}

void ResoureUpdate::init()
{
    createDownloadedDir();
    checkStoragePath();
    //pthread_mutex_init(&_mutex, NULL);
    _status = NetworkOperationStatusReady;
    _thread = NULL;
    _url = "";
    _zipfilename = "";
    _curRealTimeUpdate = RealTimeUpdateNull;
    _running = false;
}

void ResoureUpdate::realTimeUpdate(int infoId)
{
    realTimeUpdate(RealTimeUpdateA);
}

void ResoureUpdate::realTimeUpdate(RealTimeUpdate realtimeUpdate)
{
    if(getRealTimeUpdate(realtimeUpdate))
        return;
    
    _downloadPercent = 0;
    _curRealTimeUpdate = realtimeUpdate;
    _running = true;
    
    switch (realtimeUpdate) {
        case RealTimeUpdateA:
            _url = "http://10.1.1.106/res.ihero.61.com/1.0.0.0/add.zip";
            _zipfilename = FILENAME_REALTIMEUPDATE_A;
            break;
            
        case RealTimeUpdateB:
            _url = "http://10.1.1.106/res.ihero.61.com/1.0.0.0/add.zip";
            _zipfilename = FILENAME_REALTIMEUPDATE_B;
            break;
            
        case RealTimeUpdateC:
            _url = "http://10.1.1.106/res.ihero.61.com/1.0.0.0/add.zip";
            _zipfilename = FILENAME_REALTIMEUPDATE_C;
            break;
            
        case RealTimeUpdateD:
            _url = "http://10.1.1.106/res.ihero.61.com/1.0.0.0/add.zip";
            _zipfilename = FILENAME_REALTIMEUPDATE_D;
            break;
            
        case RealTimeUpdateE:
            _url = "http://10.1.1.106/res.ihero.61.com/1.0.0.0/add.zip";
            _zipfilename = FILENAME_REALTIMEUPDATE_E;
            break;
            
        default:
            break;
    }
    
    //assert(_status == NetworkOperationStatusReady);
    _status = NetworkOperationStatusRunning;

#ifndef WIN32
    pthread_create(&_thread, 0,  &ResoureUpdate::startThread, this);
#else
	_thread = CreateThread(NULL, NULL, mHandler, this, NULL, NULL);
#endif
}

void ResoureUpdate::execute()
{
    curl_downloaded();
    GameResourceManager::sharedManager()->uncompress(_storagePath, _zipfilename);
}

bool ResoureUpdate::curl_downloaded()
{
    //Create a file to save package.
    string outFileName = _storagePath + _zipfilename;
    CCLog("outfilename:%s\n", outFileName.c_str());
    FILE *fp = fopen(outFileName.c_str(), "wb");
    if (! fp)
    {
        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
    
    _curl = curl_easy_init();
    if (! _curl)
    {
        CCLOG("can not init curl");
        return false;
    }
    
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    if (res != 0)
    {
        CCLOG("error when download package");
        _status = NetworkOperationStatusFailed;
        fclose(fp);
        return false;
    }
    
    if (_status == NetworkOperationStatusRunning)
    {
        _status = NetworkOperationStatusFinished;
        CCLOG("succeed downloading package %s", _url.c_str());
        saveRealTimeUpdate(_curRealTimeUpdate, true);
    }
    
    fclose(fp);
    return true;
}

bool ResoureUpdate::getRealTimeUpdate(RealTimeUpdate realtimeUpdate)
{
    switch (realtimeUpdate) {
        case RealTimeUpdateA:
            return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_OF_REALTIMEUPDATE_A, false);
            
        case RealTimeUpdateB:
            return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_OF_REALTIMEUPDATE_B, false);
            
        case RealTimeUpdateC:
            return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_OF_REALTIMEUPDATE_C, false);
            
        case RealTimeUpdateD:
            return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_OF_REALTIMEUPDATE_D, false);
        
        case RealTimeUpdateE:
            return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_OF_REALTIMEUPDATE_E, false);
        default:
            break;
    }
    return false;
}

void ResoureUpdate::saveRealTimeUpdate(RealTimeUpdate realtimeUpdate, bool value)
{
    switch (realtimeUpdate) {
        case RealTimeUpdateA:
            CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_OF_REALTIMEUPDATE_A, value);
            CCUserDefault::sharedUserDefault()->flush();
            break;
            
        case RealTimeUpdateB:
            CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_OF_REALTIMEUPDATE_B, value);
            CCUserDefault::sharedUserDefault()->flush();
            return;
            
        case RealTimeUpdateC:
            CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_OF_REALTIMEUPDATE_C, value);
            CCUserDefault::sharedUserDefault()->flush();
            return;
            
        case RealTimeUpdateD:
            CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_OF_REALTIMEUPDATE_D, value);
            CCUserDefault::sharedUserDefault()->flush();
            return;
            
        case RealTimeUpdateE:
            CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_OF_REALTIMEUPDATE_E, value);
            CCUserDefault::sharedUserDefault()->flush();
            return;
            
        default:
            break;
    }
}

void ResoureUpdate::createDownloadedDir()
{
    _storagePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    _storagePath += "RealTimeUpdate";
    
    //Create the folder if it doesn't exist
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    
    pDir = opendir (_storagePath.c_str());
    if (! pDir)
    {
        mkdir(_storagePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
	if ((GetFileAttributesA(_storagePath.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_storagePath.c_str(), 0);
	}
#endif
}

void ResoureUpdate::checkStoragePath()
{
    if (_storagePath.size() > 0 && _storagePath[_storagePath.size() - 1] != '/')
    {
        _storagePath.append("/");
        CCLog("storagePath:%s\n", _storagePath.c_str());
    }
}

