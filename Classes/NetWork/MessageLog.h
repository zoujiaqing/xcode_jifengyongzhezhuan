// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#ifndef MESSAGE_LOG_H
#define MESSAGE_LOG_H
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if COCOS2D_DEBUG == 1
#define _DEBUG_LOG
#endif

#endif


#ifdef _DEBUG_LOG
//	MessageLog, a debug log file wrapper for print the message file to message.log file
//  you can find the log file in source/Win32 
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 
#include <time.h>
#include "Singleton_t.h"

class MessageLog : public TSingleton<MessageLog>
{
public:
	//void CreateDirectories(const char * filename);
	//	Constructor, open the logfile
	MessageLog();

	//	Destructor, close if logfile if opened
	~MessageLog();
	
    //The first parameter is the file name. It could include the absolute path, 
    //or just a file name. 
	void OpenFile(const char * strFile, bool bAppend = true, long lTruncate = 2*1024*1024);
	//void ChangeFile(const char * strFile, bool bAppend = TRUE, long lTruncate = 2*1024*1024);
	void CloseFile();

	//	Write log info into the logfile, with printf like parameters support
	void Write(const char * pszFormat, ...);
	

private:
	FILE*	m_pLogFile;
	long	m_lTruncate;
	const char *	m_filename;
};


#endif //ifdef _DEBUG_LOG

#endif
