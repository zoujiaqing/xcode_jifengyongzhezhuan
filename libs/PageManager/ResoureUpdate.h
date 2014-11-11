//
//  ResoureUpdate.h
//  HelloLua
//
//  Created by 程小二 on 13-3-25.
//
//

#ifndef HelloLua_ResoureUpdate_h
#define HelloLua_ResoureUpdate_h

#include <string>
#include <curl/curl.h>

//#include <pthread.h>

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include "Win32Platform/thread_win32.h"
#endif

#include "cocos2d.h"
#include "UpdateResourceDelegate.h"

typedef enum
{
    RealTimeUpdateA,
    RealTimeUpdateB,
    RealTimeUpdateC,
    RealTimeUpdateD,
    RealTimeUpdateE,
    RealTimeUpdateNull,
}RealTimeUpdate;

class ResoureUpdate
{
public:
    static ResoureUpdate *sharedResoureUpdate();
	static void purgeInstance();
    ~ResoureUpdate();
    
    void realTimeUpdate(int infoId);
    
    static int _downloadPercent;
    
    bool getRealTimeUpdate(RealTimeUpdate realtimeUpdate);
    void saveRealTimeUpdate(RealTimeUpdate reawltimeUpdate, bool value);
    
    bool IsRunning() { return _running; }
    void setRunning(bool run) { _running = run; }
    
    void execute();
private:
    void init();
    void createDownloadedDir();
    void checkStoragePath();
    
    void realTimeUpdate(RealTimeUpdate realtimeUpdate);
    bool curl_downloaded();
    
    static void *startThread(void *operation);
    
private:
    static ResoureUpdate *s_pSharedResoureUpdate;
    
    //! The path to store downloaded resources.
    std::string _storagePath;
    std::string _zipfilename;
    std::string _url;
    
    CURL *_curl;
    
    // keeps track of the current operation status
    volatile NetworkOperationStatus _status;
    
    //mutex used for thread synchronization
    //pthread_mutex_t _mutex;
    pthread_t _thread;
    
    RealTimeUpdate _curRealTimeUpdate;
    bool _running;
};

#endif
