
#ifndef __AssetsManager__
#define __AssetsManager__

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


/*
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 */

class AssetsManager
{
public:
    enum ErrorCode
    {
        ServerNotAvailable,    /** server address error or timeout */
        TimeOut,
    };
    
    AssetsManager(const char* reslistUrl, const char* storagePath);
        
    /* @brief Gets storage path.
     */
    const char* getStoragePath() const;
    
    /* @brief Sets storage path.
     *
     * @param storagePath The path to store downloaded resources.
     * @warm The path should be a valid path.
     */
    void setStoragePath(const char* storagePath);
    
    void start(const std::string& newVersion);
    
    std::string getVersion();
    void setVersion(const char* verison);
    void deleteVersion();
        
    /**
     Getter for the current delegate
     */
    NetworkOperationDelegate* getDelegate();
    
    /**
     Setter for the delegate to be called when the operation completes
    */
    void setDelegate(NetworkOperationDelegate *delegate);
    
    static int _downloadPercent;
    /**
     Starts the operation when called from startThread
    */
    void execute();
    
protected:
    
    bool downloadReslist();
    bool anlaysReslistMD();
    bool downloadZip(std::stringstream& strStream);
    
    bool downLoad(const char* packageUrl);
    void checkStoragePath();
    
    bool checkZipMD5(const char *srcMD5, const char *zipPath);
    bool md5Check(const char *srcMD5, const char *buffer, size_t bufSize);
    
    /**
     Static method that is called by pthread_create as the entry point
     for the new thread.
     @param operation   The operation that we need to start
     */
    static void *startThread(void *operation);
     
private:
    //! The path to store downloaded resources.
    std::string _storagePath;
    
    //! The version of downloaded resources.
    std::string _version;
    std::string _newVersion;
    
    CURL *_curl;
    std::string _reslist;
    std::string _reslistUrl;
    
    // keeps track of the current operation status
    volatile NetworkOperationStatus _status;
    
    // the delegate to be notified when the operation completes
    NetworkOperationDelegate *_delegate;
    
    // mutex used for thread synchronization
    //pthread_mutex_t _mutex;
    
    // thread handle
    //pthread_t _thread;
    
    pthread_t _thread;//µ±«∞œﬂ≥Ãµƒœﬂ≥ÃID
};

#endif /* defined(__AssetsManager__) */


