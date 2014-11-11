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
#include <Windows.h>
#include "platform/CCCommon.h"
#include "CCStdC.h"
#include "../CCFileUtils.h"
#include <string>
#include <stdio.h>

NS_CC_BEGIN

#define MAX_LEN         (cocos2d::kMaxLogLen + 1)

void writeLog(const char * log)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || COCOS2D_DEBUG == 1 
	static std::string writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath().append("/adventureLog.txt");
	static FILE* outf = NULL;
	static int init = 1;
	if (init == 1){//初始化时检测长度，
		init = 2;
		CCLog("adventure log path: %s", writablePath.c_str());
	}
	if(outf == NULL)
	{//try open file
		rename(writablePath.c_str(), std::string(writablePath + std::string(".txt")).c_str());
		outf = fopen(writablePath.c_str(), "a");
		if (!outf)
		{
			return;
		}
		fseek(outf, 0, SEEK_END);
		long len = ftell(outf);
		if (len > 5 * 1024 * 1024)//长度大于5M就删除
		{
			rewind(outf);
		}
	}
	if(outf)
	{
		fputs(log, outf);
		fputs("\n", outf);
		fflush(outf);
	}
#endif
}

	
void SetColor(unsigned short ForeColor=4,unsigned short BackGroundColor=0)                                                                                                   //可以接受0/1/2个参数
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon,ForeColor|BackGroundColor);
}



void CCLog(const char * pszFormat, ...)
{
	SetColor(8);

    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
    va_end(ap);

    WCHAR wszBuf[MAX_LEN] = {0};
    MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");

    WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
    printf("%s\n", szBuf);

	writeLog(szBuf);
}

void CCError(const char * pszFormat, ...)
{
	SetColor(4);

	char szBuf[MAX_LEN];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[MAX_LEN] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	printf("%s\n", szBuf);
	
	writeLog(szBuf);
}

void CCMessageBox(const char * pszMsg, const char * pszTitle)
{
    MessageBoxA(NULL, pszMsg, pszTitle, MB_OK);
}

void CCLuaLog(const char *pszMsg)
{
    int bufflen = MultiByteToWideChar(CP_UTF8, 0, pszMsg, -1, NULL, 0);
    WCHAR* widebuff = new WCHAR[bufflen + 1];
    memset(widebuff, 0, sizeof(WCHAR) * (bufflen + 1));
    MultiByteToWideChar(CP_UTF8, 0, pszMsg, -1, widebuff, bufflen);

    OutputDebugStringW(widebuff);
    OutputDebugStringA("\n");

	bufflen = WideCharToMultiByte(CP_ACP, 0, widebuff, -1, NULL, 0, NULL, NULL);
	char* buff = new char[bufflen + 1];
	memset(buff, 0, sizeof(char) * (bufflen + 1));
	WideCharToMultiByte(CP_ACP, 0, widebuff, -1, buff, bufflen, NULL, NULL);
	puts(buff);

	writeLog(buff);

	delete[] widebuff;
	delete[] buff;
}

NS_CC_END
