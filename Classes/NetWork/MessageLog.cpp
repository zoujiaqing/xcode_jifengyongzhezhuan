#include "MessageLog.h"
#include "GameResourceManager.h"
#ifdef _DEBUG_LOG
USING_NS_CC;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <direct.h>
#endif
#include <stdio.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "adventure"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

void MessageLog::OpenFile(const char * strFile, bool bAppend, long lTruncate)
{

	m_lTruncate = lTruncate;
	m_filename = strFile;
#if 0
	int result = rename(m_filename, std::string(std::string(m_filename) + std::string(".txt")).c_str());
    if (result == 0 )
        CCLOG( "File successfully renamed" );
    else
        CCLOG( "Error renaming file" );
	#endif

   static const char file2name[] = "/sdcard/messageLog1.txt"; 
   FILE *file;
   FILE *write;
   char line [ 256 ]; /* or other suitable maximum line size */
   char linec [256]; // copy of line

               
   file = fopen ( strFile, "r" );
   write = fopen (file2name, "w" );

   if ( file != NULL && write != NULL)
   {
      while ( fgets ( line, sizeof(line), file ) != NULL ) /* read a line */
      {
     	fprintf (write , line);
      }
      fclose (write);
      fclose ( file );
   }
   
   

	if (!(m_pLogFile = fopen(m_filename, "w")))
	{
		printf("Failed to open message log file\n");
	}
}

MessageLog::MessageLog()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || COCOS2D_DEBUG == 1 
    std::string aa = GameResourceManager::sharedManager()->getStorepath();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//aa = CCFileUtils::sharedFileUtils()->getWriteablePath().append("/files");
	//aa.replace(aa.find("/data/data"), std::string("/data/data").length(), "/sdcard/Android/data");//Ð´Èësd
	aa="/sdcard";
#endif
    CCLOG("storage path is %s", aa.c_str());
	OpenFile((aa+"/messageLog.txt").c_str(), false);
#endif
}

/////////////////////////////////
//	Destructor, close if logfile if opened
MessageLog::~MessageLog()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || COCOS2D_DEBUG == 1 
	CloseFile();
#endif
}
	
void MessageLog::Write(const char * pszFormat, ...)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || COCOS2D_DEBUG == 1 

		if (!m_pLogFile)
			return;

		//Trancate if the file grow too large
		long lLength = ftell(m_pLogFile);
		if (lLength > m_lTruncate)
			rewind(m_pLogFile);

		//write the formated log string to szLog
		char szLog[1024];
		va_list argList;
		va_start( argList, pszFormat );
		vsnprintf( szLog, 1024, pszFormat, argList );
		va_end( argList );

		//Get current time
		time_t timep;
		time (&timep);
		char szLine[1500];
		snprintf(szLine, 1500, "\n****************************\n%s %s", ctime(&timep), szLog );		
		fputs(szLine, m_pLogFile);
		fflush(m_pLogFile);

#endif
/* jackniu disable this 
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	{	char szLog[1024];
		va_list argList;
		va_start( argList, pszFormat );
		vsnprintf( szLog, 1024, pszFormat, argList );
		va_end( argList );
        
        struct cc_timeval now;

        if (CCTime::gettimeofdayCocos2d(&now, NULL) != 0)
        {
            CCLOG("error in gettimeofday");
            
        }
        long t = now.tv_sec*1000+now.tv_usec/1000;
        LOGI("****************************");
        LOGI("time %ld %s ", t,  szLog);
    }

#endif
   */     

}

void MessageLog::CloseFile()
{
	if (m_pLogFile)
	{
		fputs("\n===============Finish Loging================\n\n", m_pLogFile);

		fclose(m_pLogFile);
	}

	
}

#endif //end of _DEBUG_LOG
