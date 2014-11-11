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

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <errno.h>
#include "GameResourceManager.h"

#include "CCCommon.h"
#include "CCFileUtils.h"

#include "support/data_support/uthash.h"
#include "curl/curl.h"
#include "AES.h"

#include "ResourceCache.h"
#include "ResourcePlistArray.h"
#include "ResourcePlistDictionary.h"
#include "AssetsManager.h"

//#include "ResoureUpdate.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <direct.h>
#include <io.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "SystemInfoJni.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "CocosPlayClient.h"

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

#include "support/zip_support/unzip.h"


#define MAX_SIZE 1024

#if (!defined ANDROID && !defined PATH_MAX)
#define PATH_MAX 512
#endif

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512


using namespace cocos2d;
using namespace std;

GameResourceManager* GameResourceManager::s_pSharedManager = NULL;

static int debug_callback(CURL *curl, curl_infotype type, char *data, size_t size, void *pt)
{
	CCLOG("data type: %d", type);
	CCLOG("data size: %d", size);
	char *str = new char[size + 1];
	strncpy(str, data, size);
	str[size] = 0;
	if(size < 256)
		CCLOG("data: %s", str);
	delete[] str;

	return 0;
}

GameResourceManager *GameResourceManager::sharedManager()
{
	if(s_pSharedManager == NULL)
	{
		s_pSharedManager = new GameResourceManager();
		s_pSharedManager->init();
	}
    
	return s_pSharedManager;
}

void GameResourceManager::purgeInstance()
{
	if (s_pSharedManager)
	{
		delete s_pSharedManager;
		s_pSharedManager = NULL;
	}
}

GameResourceManager::~GameResourceManager()
{
	curl_global_cleanup();
	ResourceCache::purgeInstance();
    
    delete  m_pAssetsManager;
    m_pAssetsManager = NULL;

	//// must call this!
	//ResoureUpdate::purgeInstance();
}

void GameResourceManager::updateResource(const std::string& version)
{
    //m_pAssetsManager->start("1.0.0.0", "machine id");
}

const char *GameResourceManager::writablePathFromRelativePath(const char *path)
{
	string writablePath;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//on android check sd card first
	if(m_bHasSDCard)
	{
		writablePath = m_strSDCardPath;
		writablePath.append("/");
		writablePath.append(path);
	}
	else
	{
		writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
		writablePath.append(path);
	}
#else
	writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
	writablePath.append(path);
#endif
	CCString *pRet = new CCString();
	pRet->autorelease();
	pRet->m_sString = writablePath;
	return pRet->m_sString.c_str();
}

const char* GameResourceManager::androidPathFromRelativePath(const char* path)
{
	string writablePath = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
	writablePath.append(path);
	if(access(writablePath.c_str(), 0) != 0)
	{
		writablePath = "";
	}
#endif
	CCString *pRet = new CCString();
	pRet->autorelease();
	pRet->m_sString = writablePath;
	return pRet->m_sString.c_str();
}

const char *GameResourceManager::storedFullPathFromRelativePath(const char *additionalPath,const char *path)
{
	return storedFullPathFromRelativePath((std::string(additionalPath)+std::string(path)).c_str());
}

const char *GameResourceManager::storedFullPathFromRelativePath(const char *path)
{
	CCString *pRet = new CCString();
	pRet->autorelease();
    
	//check writable space first
//	std::string resPath = writablePathFromRelativePath(path);
//	if(access(resPath.c_str(), 0) == 0)
//	{
//		pRet->m_sString = resPath;
//		return pRet->m_sString.c_str();
//	}
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	//on android then check /data/data
//	resPath = CCFileUtils::sharedFileUtils()->getWriteablePath();
//	resPath.append(path);
//
//	if(access(resPath.c_str(), 0) == 0)
//	{
//
//		pRet->m_sString = resPath;
//		return pRet->m_sString.c_str();
//	}
//#endif
std::string	resPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path);
  // std::string resPath =fullPathForFilename(path);
	pRet->m_sString = resPath;
	return pRet->m_sString.c_str();
}

CCArray *GameResourceManager::arrayFromCache(const char *path)
{
	CCArray* ret;
    
	ResourcePlistArray *cache = 
		(ResourcePlistArray*)ResourceCache::sharedResourceCache()->getResource(path);

	if(cache != NULL)
	{
		ret = cache->getData();
	}
	else
	{
		cache = ResourcePlistArray::resourceWithFile(path);
		ret = cache->getData();
	}

	return ret;
}

CCDictionary *GameResourceManager::dictionaryFromCache(const char *path)
{
	CCDictionary* ret;

	ResourcePlistDictionary *cache = 
		(ResourcePlistDictionary*)ResourceCache::sharedResourceCache()->getResource(path);

	if(cache != NULL)
	{
		ret = cache->getData();
	}
	else
	{
		cache = ResourcePlistDictionary::resourceWithFile(path);
		ret = cache->getData();
	}

	return ret;
}

void GameResourceManager::addSpriteFrameWithDictionaryFromCache(const char *path)
{
	CCDictionary* dict = dictionaryFromCache(path);

	string texturePath("");

	CCDictionary* metadataDict
		= (CCDictionary*)dict->objectForKey(string("metadata"));
	if (metadataDict)
	{
		// try to read  texture file name from meta data
		if (dict)
		{
			CCString *pString = (CCString*)dict->objectForKey("textureFileName");
			texturePath = pString ? pString->m_sString.c_str() : "";
		}
		else
		{
			texturePath = "";
		}
	}

	if (! texturePath.empty())
	{
		// build texture path relative to plist file
		texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), path);
	}
	else
	{
		// build texture path by replacing file extension
		texturePath = path;

		// remove .xxx
		size_t startPos = texturePath.find_last_of("."); 
		texturePath = texturePath.erase(startPos);

		// append .png
		texturePath = texturePath.append(".png");
	}

	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(dict, pTexture);
}

void GameResourceManager::removeSpriteFrameWithDictionaryFromCache(const char *path)
{
	CCDictionary* dict = dictionaryFromCache(path);

	if(dict != NULL)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromDictionary(dict);
	}
	else
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(path);
	}
}

const char* GameResourceManager::loadXml(const char *fileName,bool isEncrypt)
{    
    CCFileData data(this->storedFullPathFromRelativePath(fileName), "rb");
    size_t size = data.getSize();
    char* out = (char*)malloc(size);
    memcpy(out, data.getBuffer(),size);
    if (isEncrypt) 
    {
        AES aes(m_xmlEncryptKey);  
        aes.InvCipher((void *)out,size);
    } 
    return out;
}

void GameResourceManager::init()
{
	curl_global_init(CURL_GLOBAL_ALL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	loadSDCardInfo();
#endif

	m_xmlEncryptKey[0] = 0x2b;
	m_xmlEncryptKey[1] = 0x7e;
	m_xmlEncryptKey[2] = 0x15;
	m_xmlEncryptKey[3] = 0x16; 
	m_xmlEncryptKey[4] = 0x28;
	m_xmlEncryptKey[5] = 0xae;
	m_xmlEncryptKey[6] = 0xd2;
	m_xmlEncryptKey[7] = 0xa6; 
	m_xmlEncryptKey[8] = 0xab;
	m_xmlEncryptKey[9] = 0xf7;
	m_xmlEncryptKey[10] = 0x15;
	m_xmlEncryptKey[11] = 0x88; 
	m_xmlEncryptKey[12] = 0x09;
	m_xmlEncryptKey[13] = 0xcf;
	m_xmlEncryptKey[14] = 0x4f;
	m_xmlEncryptKey[15] = 0x3c;
    
    createDownloadedDir();
 
    m_pAssetsManager = new AssetsManager("", m_storagePath.c_str());
    m_pAssetsManager->setDelegate(this);
	
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GameResourceManager::loadSDCardInfo()
{
	m_bHasSDCard = hasSDCard();
	m_strSDCardPath = getSDCardPath();
	CCLOG("sd path: %s", m_strSDCardPath.c_str());
}
#endif

bool GameResourceManager::uncompress(const std::string& storePath, const std::string& filename)
{
    // Open the zip file
    string outFileName = storePath + filename;
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = storePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    return true;
}

void GameResourceManager::operationDidFinish(AssetsManager *operation, const char* strVal)
{
    CCLOG("GameResoureceManager: operationDidFinish\n");
}

void GameResourceManager::operationDidFail(AssetsManager *operation)
{
    CCLOG("GameResoureceManager: operationDidFail\n");
}

void GameResourceManager::createDownloadedDir()
{
    m_storagePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    m_storagePath += "tmpdir";
    
    // Create the folder if it doesn't exist
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    
    pDir = opendir (m_storagePath.c_str());
    if (! pDir)
    {
        mkdir(m_storagePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
	if ((GetFileAttributesA(m_storagePath.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(m_storagePath.c_str(), 0);
	}
#endif
}

/*
 * Create a direcotry is platform depended.
 */
bool GameResourceManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

CCRenderTexture* GameResourceManager::createStroke(CCLabelTTF* label, float size, ccColor3B cor, GLubyte opacity)
{
    CCRenderTexture* rt = CCRenderTexture::renderTextureWithWidthAndHeight(
                                                                           label->getTexture()->getContentSize().width + size * 2,
                                                                           label->getTexture()->getContentSize().height+size * 2
                                                                           );
    
    CCPoint originalPos = label->getPosition();
    
    ccColor3B originalColor = label->getColor();
    
    GLubyte originalOpacity = label->getOpacity();
    
    bool originalVisibility = label->isVisible();
    
    label->setColor(cor);
    
    label->setOpacity(opacity);
    
    label->setVisible(true);
    
    ccBlendFunc originalBlend = label->getBlendFunc();
    
    ccBlendFunc bf = {GL_SRC_ALPHA, GL_ONE};
    
    label->setBlendFunc(bf);
    
    CCPoint bottomLeft = ccp(
                             label->getTexture()->getContentSize().width * label->getAnchorPoint().x + size,
                             label->getTexture()->getContentSize().height * label->getAnchorPoint().y + size);
    
    CCPoint positionOffset= ccp(
                                - label->getTexture()->getContentSize().width / 2,
                                - label->getTexture()->getContentSize().height / 2);
    
    CCPoint position = ccpSub(originalPos, positionOffset);
    

	rt->clear(0, 0, 0, 0);
    rt->begin();
    
    for (int i=0; i<360; i+= 15) // you should optimize that for your needs
    {
        label->setPosition(ccp(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*size, bottomLeft.y + cos(CC_DEGREES_TO_RADIANS(i))*size));
        label->visit();
    }
    rt->end();
    
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    label->setVisible(originalVisibility);
    label->setOpacity(originalOpacity);
    rt->setPosition(originalPos);
    
    return rt;
}
void    GameResourceManager::initSearchPath()
{
	std::vector<std::string> searchPaths ;
	searchPaths.clear();
	std::string writablePath ;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//on android check sd card first
	//1 check /sdcard/Android/data/package name /files/
	//if(m_bHasSDCard)
	{
		writablePath = m_strSDCardPath;
		writablePath.append("/");
		CCLOG("andriod root path : %s",m_strSDCardPath.c_str());
		
	}
	searchPaths.push_back(writablePath);
	//2 check coco play sdcard dir
	if (cocosplay::isEnabled()) {
		searchPaths.push_back(cocosplay::getGameRoot());
	}
	//3 check /data/data/packagename/
	{
		writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
		
	}
#else
	 writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
#endif
	
	//writablePath.append("Resources");
	searchPaths.push_back(writablePath);
	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    std::vector<std::string> str = CCFileUtils::sharedFileUtils()->getSearchPaths();
}