#include <iostream>
#include <sstream>

#include "XPlatform.h"
#include "CrossUtil.h"
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "SimpleAudioEngine.h"
#include "GameResourceManager.h"
#include "IExResourceMgr.h"

using namespace CocosDenshion;
using namespace cocos2d;

#define LOCALIZABLE_STRING_FILE "Data/Localizable.strings"
#define MAX_MBS_SIZE 512

IPlatform* CreateXPlatform(IExResourceMgr * pResrouceMgr)
{    
    IPlatform *pf = new XAndroidPlatform(pResrouceMgr);
    return pf;
}

XAndroidPlatform::XAndroidPlatform(IExResourceMgr * pResrouceMgr)
{    
	m_pResourceMgr = pResrouceMgr;
    InitPlatform();
}

///
//  InitPlatform
//  init all the devices that needed!
//
void XAndroidPlatform::InitPlatform()
{
	if(!m_localizableStringMap.empty())
	{
		return;
	}

	CCAssert(m_pResourceMgr != NULL, "XAndroidPlatform::m_pResourceMgr == NULL");

	CCFileData data(m_pResourceMgr->storedFullPathFromRelativePath(LOCALIZABLE_STRING_FILE), "r");
	const char* buffer = (const char*)data.getBuffer();
	std:string strBuffer(buffer);
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	ss << strBuffer;

	do
	{
		char strTemp[1024];
		std::string strVal;

		//by benyang:not very strong
		//need add "//" & "/*" detection
		while(!ss.eof())
		{
			strTemp[0] = '\0';
			ss.getline(strTemp, 1024);
			strVal = strTemp;

			string key;
			string value;

			int quotePos1, quotePos2, quotePos3, quotePos4;
			
			quotePos1 = strVal.find('\"');
			if(quotePos1 != std::string::npos)
			{
				quotePos2 = strVal.find('\"', quotePos1 + 1);
				if(quotePos2 == std::string::npos)
				{
					CCLog("wrong format of file %s", LOCALIZABLE_STRING_FILE);
					//m_localizableStringMap.clear();
					return;
				}

				quotePos3 = strVal.find('\"', quotePos2 + 1);
				if(quotePos3 == std::string::npos)
				{
					CCLog("wrong format of file %s", LOCALIZABLE_STRING_FILE);
					//m_localizableStringMap.clear();
					return;
				}

				quotePos4 = strVal.find('\"', quotePos3 + 1);
				if(quotePos4 == std::string::npos)
				{
					CCLog("wrong format of file %s", LOCALIZABLE_STRING_FILE);
					//m_localizableStringMap.clear();
					return;
				}

				key = strVal.substr(quotePos1 + 1, quotePos2 - quotePos1 - 1);
				value = strVal.substr(quotePos3 + 1, quotePos4 - quotePos3 - 1);

				int matchPos = value.find("\\n");
				while(matchPos != std::string::npos)
				{
					value.replace(matchPos, 2, "\n");
					matchPos = value.find("\\n");
				}

				m_localizableStringMap.insert(make_pair(key, value));
			}
		}
	} while (0);
}

///
//  ShutDown
//  clean up all context used  
//
void XAndroidPlatform::ShutDown()
{

}

///
//  @prototype, playAudioEffect
//
//  @param 
//         name,  audio file's name
//  @param
//         delay, delay a time to play sound
//
void XAndroidPlatform::PlayAudioEffect(const char*name, float delay)
{
	SimpleAudioEngine::sharedEngine()->playEffect(name);
}

///
//  @prototype, UTF8Length
//
//  @param
//         str,   source string
//
int XAndroidPlatform::UTF8Length(const char* str)
{
	int index = 0;
	int count = 0;
	while (str[index])
	{
		if ((str[index] & 0xC0) != 0x80)
		{
			count++;
		}
		index++;
	}
	return count;
}

///
//  @prototype, UTF8SubString
//
//  @param  inStr,  source string
//  @param  length, sub-string length
//  @param  outStr, destination string
void XAndroidPlatform::UTF8SubString(const char* inStr, int length, char* outStr)
{
	int srcIndex = 0;
	int outIndex = 0;
	int count = 0;
	while (inStr[srcIndex] && (count < length))
	{
		size_t bytesOfOneChar = 0;

		//0xxxxxxx
		if((inStr[srcIndex] & 0x80) == 0x00)
		{
			bytesOfOneChar = 1;
		}
		//110xxxxx
		else if((inStr[srcIndex] & 0xE0) == 0xC0)
		{
			bytesOfOneChar = 2;
		}
		//1110xxxx
		else if((inStr[srcIndex] & 0xF0) == 0xE0)
		{
			bytesOfOneChar = 3;
		}
		//11110xxx
		else if((inStr[srcIndex] & 0xF8) == 0xF0)
		{
			bytesOfOneChar = 4;
		}
		//111110xx
		else if((inStr[srcIndex] & 0xFC) == 0xF8)
		{
			bytesOfOneChar = 5;
		}
		//1111110x
		else if((inStr[srcIndex] & 0xFE) == 0xFC)
		{
			bytesOfOneChar = 6;
		}
		// should not happen
		else
		{
			bytesOfOneChar = 1;
		}

		if((outIndex + bytesOfOneChar) >= MAX_MBS_SIZE - 1)
		{
			break;
		}

		memcpy(outStr + outIndex, inStr + srcIndex, bytesOfOneChar);
		outIndex += bytesOfOneChar;
		srcIndex += bytesOfOneChar;
		count++;
	}

	outStr[outIndex] = 0;
}


///
//  @prototype, getCurLanguage
//
//  @param 
//         void
//  @note, get current lanaguage from the device
// get cur language
int XAndroidPlatform::getCurLanguage()
{
    return LANGUAGE_HS; //简体中文
}

/*
 *@brief 获取设备的uuid
 */
const char *XAndroidPlatform::getDeviceUUID()
{
    return "Unknown UUID";
}

unsigned int  XAndroidPlatform::getVersion()
{
    return 100;
}

const char *XAndroidPlatform::getVersionA()
{
    return "1.0.0";
}

float XAndroidPlatform::getSystemVersion()
{
    return 100.0f;
}

/*
 *@brief 判断网络是否链接
 */
bool XAndroidPlatform::IsConnected()
{
	// DISABLE 
	//KSocketStatus socketStatus = OnlineNetworkManager::sShareInstance()->getSocketStatus();
	//if(socketStatus == KSocketConnecting)
	//{
	//	return true;
	//}
	//else
	{
		return false;
	}
}

/*
 *@brief 判断设备是ipad　or　iphone
 *
 */

UIUserInterfaceType XAndroidPlatform::getUserInterface()
{
    return UIUserInterfacePad;
}



bool XAndroidPlatform::IsRetinaDisplay()
{
    return false;
}


void XAndroidPlatform::initUserInterface()
{

}

bool XAndroidPlatform::IsIpad()
{
    return true;
}

const char *XAndroidPlatform::CCLocalizedString(const char *key,const char *comment)
{
	std::string strKey = key;
	std::map<std::string, std::string>::iterator it = m_localizableStringMap.find(strKey);
	if(it != m_localizableStringMap.end())
	{
		return it->second.c_str();
	}
	else
	{
		return key;
	}
}

const char* XAndroidPlatform::getPasswordForUsername(const char* username,const char* serviceName)
{
    return "Unknown";
}

bool XAndroidPlatform::storeUsername(const char* username ,const char* password,const char* serviceName ,bool updateExisting )
{
    return true;
}