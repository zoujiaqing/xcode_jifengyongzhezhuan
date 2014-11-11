/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "platform/CCCommon.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <android/log.h>
#include <stdio.h>
#include <jni.h>
#include "../CCFileUtils.h"
#include <string>
#include <stdio.h>


NS_CC_BEGIN

#define MAX_LEN         (cocos2d::kMaxLogLen + 1)
void writeLog(const char * log)
{
#if COCOS2D_DEBUG == 1 
	static FILE* logfile=NULL;

	if(!logfile) {
		 static const char file1name[] = "/sdcard/adventureLog.txt"; 

		 static const char file2name[] = "/sdcard/adventureLog1.txt"; 
		 FILE *file;
		 FILE *write;
		 char line [ 256 ]; /* or other suitable maximum line size */
		 char linec [256]; // copy of line
		
					 
		 file = fopen ( file1name, "r" );
		 write = fopen (file2name, "w" );
		
		 if ( file != NULL && write != NULL)
		 {
			while ( fgets ( line, sizeof(line), file ) != NULL ) /* read a line */
			{
			  fprintf (write , line);
			}
			fclose (write);
			fclose (file );
		 }
		 
		 
		
		  if (!(logfile = fopen(file1name, "w")))
		  {
			  printf("Failed to open message log file\n");
		  }

	}
	if(logfile) {
		fprintf(logfile , "%s\n", log);
		fflush(logfile);
	}
#if 0
	//
	static std::string romPath = CCFileUtils::sharedFileUtils()->getWriteablePath().append("files/adventureLog.txt");
	static std::string sdPath;
	static std::string sdPath1;
	static FILE* outf = NULL;
	static FILE* outf2 = NULL;
	static FILE* outf3 = NULL;
	static bool init = false;
	if (!init)
	{//init sd path
		init = true;
		sdPath = romPath;
		sdPath1 = romPath;
		sdPath.replace(sdPath.find("/data/data"), std::string("/data/data").length(), "/sdcard/Android/data");//sd路径
		sdPath1.replace(sdPath1.find("/data/data"), std::string("/data/data").length(), "/storage/sdcard0/Android/data");//sd1路径
		CCLog("adventure log path: %s", romPath.c_str());
	}
	
	if(outf == NULL && outf2 == NULL && outf3 == NULL)
	{//try open sd1 file
		rename(sdPath1.c_str(), std::string(sdPath1 + std::string(".txt")).c_str());
		outf3 = fopen(sdPath1.c_str(), "w");
		if (outf3)
			CCLog("sd1 log opened");
	}

	if(outf == NULL && outf2 == NULL && outf3 == NULL)
	{//try open sd file
		rename(sdPath.c_str(), std::string(sdPath + std::string(".txt")).c_str());
		outf2 = fopen(sdPath.c_str(), "w");
		if (outf2)
			CCLog("sd log opened");
	}

	/*
	if(outf == NULL && outf2 == NULL && outf3 == NULL)
	{//sd out still NULL, try open rom file
		rename(romPath.c_str(), std::string(romPath + std::string(".txt")).c_str());
		outf = fopen(romPath.c_str(), "w");
		if (outf)
			CCLog("rom log opened");
	}
	*/
	
	if(outf3)
	{//sd
		fputs(log, outf3);
		fputs("\n", outf3);
		fflush(outf3);
	}
	
	if(outf2)
	{//sd
		fputs(log, outf2);
		fputs("\n", outf2);
		fflush(outf2);
	}

	if(outf)
	{//rom
		fputs(log, outf);
		fputs("\n", outf);
		fflush(outf);
	}
#endif
#endif
}


void CCLog(const char * pszFormat, ...)
{
    char buf[MAX_LEN];

    va_list args;
    va_start(args, pszFormat);        
    vsnprintf(buf, MAX_LEN, pszFormat, args);
    va_end(args);

    __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info",  buf);

	writeLog(buf);
}

void CCMessageBox(const char * pszMsg, const char * pszTitle)
{
    showDialogJNI(pszMsg, pszTitle);
}

void CCLuaLog(const char * pszFormat)
{
    CCLog(pszFormat);
}

NS_CC_END
