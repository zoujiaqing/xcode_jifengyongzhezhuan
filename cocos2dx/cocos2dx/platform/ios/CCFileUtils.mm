/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.

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
#import <Foundation/Foundation.h>
#import <UIKit/UIDevice.h>

#include <string>
#include <stack>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include "CCString.h"
#include "CCFileUtils.h"
#include "CCDirector.h"
#include "CCSAXParser.h"
#include "CCDictionary.h"
#include "support/zip_support/unzip.h"

#define MAX_PATH 260

USING_NS_CC;

static void static_addValueToCCDict(id key, id value, CCDictionary* pDict);
static void static_addItemToCCArray(id item, CCArray* pArray);

static void static_addItemToCCArray(id item, CCArray *pArray)
{
    // add string value into array
    if ([item isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([item UTF8String]);
        
        pArray->addObject(pValue);
        pValue->release();
        return;
    }

    // add number value into array(such as int, float, bool and so on)
    if ([item isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [item stringValue];
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pArray->addObject(pValue);
        pValue->release();
        return;
    }
    
    // add dictionary value into array
    if ([item isKindOfClass:[NSDictionary class]]) {
        CCDictionary* pDictItem = new CCDictionary();
        for (id subKey in [item allKeys]) {
            id subValue = [item objectForKey:subKey];
            static_addValueToCCDict(subKey, subValue, pDictItem);
        }
        pArray->addObject(pDictItem);
        pDictItem->release();
        return;
    }
    
    // add array value into array
    if ([item isKindOfClass:[NSArray class]]) {
        CCArray *pArrayItem = new CCArray();
        pArrayItem->init();
        for (id subItem in item) {
            static_addItemToCCArray(subItem, pArrayItem);
        }
        pArray->addObject(pArrayItem);
        pArrayItem->release();
        return;
    }
}

static void static_addValueToCCDict(id key, id value, CCDictionary* pDict)
{
    // the key must be a string
    CCAssert([key isKindOfClass:[NSString class]], "The key should be a string!");
    std::string pKey = [key UTF8String];

    // the value is a new dictionary
    if ([value isKindOfClass:[NSDictionary class]]) {
        CCDictionary* pSubDict = new CCDictionary();
        for (id subKey in [value allKeys]) {
            id subValue = [value objectForKey:subKey];
            static_addValueToCCDict(subKey, subValue, pSubDict);
        }
        pDict->setObject(pSubDict, pKey.c_str());
        pSubDict->release();
        return;
    }

    // the value is a string
    if ([value isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([value UTF8String]);

        pDict->setObject(pValue, pKey.c_str());
        pValue->release();
        return;
    }

    // the value is a number
    if ([value isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [value stringValue];
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pDict->setObject(pValue, pKey.c_str());
        pValue->release();
        return;
    }

    // the value is a array
    if ([value isKindOfClass:[NSArray class]]) {
        CCArray *pArray = new CCArray();
        pArray->init();
        for (id item in value) {
            static_addItemToCCArray(item, pArray);
        }
        pDict->setObject(pArray, pKey.c_str());
        pArray->release();
        return;
    }
}

NS_CC_BEGIN

static CCFileUtils* s_pFileUtils = NULL;

CCFileUtils* CCFileUtils::sharedFileUtils()
{
    if (s_pFileUtils == NULL)
    {
        s_pFileUtils = new CCFileUtils();
    }
    return s_pFileUtils;
}

void CCFileUtils::purgeFileUtils()
{
    if (s_pFileUtils != NULL)
    {
        s_pFileUtils->purgeCachedEntries();
    }

    CC_SAFE_DELETE(s_pFileUtils);
}

void CCFileUtils::purgeCachedEntries()
{

}

void CCFileUtils::setResourceDirectory(const char *pszDirectoryName)
{
    m_obDirectory = pszDirectoryName;
    if (m_obDirectory.size() > 0 && m_obDirectory[m_obDirectory.size() - 1] != '/')
    {
        m_obDirectory.append("/");
    }
}

const char* CCFileUtils::getResourceDirectory()
{
    return m_obDirectory.c_str();
}
const char*  CCFileUtils::getResourcePath()
{
    NSString *respath = nil;
    
    respath = [[NSBundle mainBundle] bundlePath];
    
//    std::string file = "snow.pvr.ccz";
//    std::string path = ".";
//    
//    respath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file.c_str()]
//                                                         ofType:nil
//                                                    inDirectory:[NSString stringWithUTF8String:path.c_str()]];
    
    //NSString *resDirectory = [respath stringByDeletingLastPathComponent];
    return [respath UTF8String] ;
}
/*
const char* CCFileUtils::fullPathFromRelativePath(const char *pszRelativePath)
{
    CCAssert(pszRelativePath != NULL, "CCFileUtils: Invalid path");
    
    NSString *fullpath = nil;
    NSString *relPath = [NSString stringWithUTF8String:pszRelativePath];
    
    // only if it is not an absolute path
    if( ! [relPath isAbsolutePath] ) {
        
        // pathForResource also searches in .lproj directories. issue #1230
        NSString *lastPathComponent = [relPath lastPathComponent];
        
        NSString *imageDirectory = [relPath stringByDeletingLastPathComponent];
        NSMutableString *imageDirectoryByAppendDirectory = [NSMutableString stringWithUTF8String:m_obDirectory.c_str()];
        [imageDirectoryByAppendDirectory appendString:imageDirectory];
        
        // search path from directory set by setResourceDirectory
        fullpath = [[NSBundle mainBundle] pathForResource:lastPathComponent
                                                   ofType:nil
                                              inDirectory:imageDirectoryByAppendDirectory];
        if (fullpath == nil)
        {
            // search from root directory
            fullpath = [[NSBundle mainBundle] pathForResource:lastPathComponent
                                                       ofType:nil
                                                  inDirectory:imageDirectory];
        }
    }
    
    if (fullpath == nil)
    {
        fullpath = relPath;
    }
    
    return [fullpath UTF8String];
}
*/
const char *CCFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
{
    std::string relativeFile = fullPathFromRelativePath(pszRelativeFile);
    CCString *pRet = new CCString();
    pRet->autorelease();
    pRet->m_sString = relativeFile.substr(0, relativeFile.rfind('/')+1);
    pRet->m_sString += pszFilename;
    return pRet->m_sString.c_str();
}

CCDictionary* ccFileUtils_dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
{
    const char* pszFullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName);
    NSString* pPath = [NSString stringWithUTF8String:pszFullPath];
    NSDictionary* pDict = [NSDictionary dictionaryWithContentsOfFile:pPath];
    
    CCDictionary* pRet = new CCDictionary();
    for (id key in [pDict allKeys]) {
        id value = [pDict objectForKey:key];
        static_addValueToCCDict(key, value, pRet);
    }
    
    return pRet;
}

CCArray* ccFileUtils_arrayWithContentsOfFileThreadSafe(const char* pFileName)
{
//    NSString* pPath = [NSString stringWithUTF8String:pFileName];
//    NSString* pathExtension= [pPath pathExtension];
//    pPath = [pPath stringByDeletingPathExtension];
//    pPath = [[NSBundle mainBundle] pathForResource:pPath ofType:pathExtension];
//    fixing cannot read data using CCArray::createWithContentsOfFile
    const char* pszFullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName);
    NSString* pPath = [NSString stringWithUTF8String:pszFullPath];
    NSArray* pArray = [NSArray arrayWithContentsOfFile:pPath];
    
    CCArray* pRet = new CCArray();
    for (id value in pArray) {
        static_addItemToCCArray(value, pRet);
    }

    return pRet;
}

unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    CCAssert(pszFileName != NULL && pSize != NULL && pszMode != NULL, "Invalid parameters.");
    *pSize = 0;
    do 
    {
        // read the file from hardware
        FILE *fp = fopen(pszFileName, pszMode);
        CC_BREAK_IF(!fp);

        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);

    if (! pBuffer && isPopupNotify()) 
    {
        std::string title = "Notification";
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");

        CCMessageBox(msg.c_str(), title.c_str());
    }
    return pBuffer;
}

// notification support when getFileData from a invalid file
static bool s_bPopupNotify = true;

void CCFileUtils::setPopupNotify(bool bNotify)
{
    s_bPopupNotify = bNotify;
}

bool CCFileUtils::isPopupNotify()
{
    return s_bPopupNotify;
}

std::string CCFileUtils::getWriteablePath()
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
}

unsigned char* CCFileUtils::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
{
        unsigned char * pBuffer = NULL;
        unzFile pFile = NULL;
        *pSize = 0;
    
        do 
        {
            CC_BREAK_IF(!pszZipFilePath || !pszFileName);
            CC_BREAK_IF(strlen(pszZipFilePath) == 0);
    
            pFile = unzOpen(pszZipFilePath);
            CC_BREAK_IF(!pFile);
    
            int nRet = unzLocateFile(pFile, pszFileName, 1);
            CC_BREAK_IF(UNZ_OK != nRet);
    
            char szFilePathA[260];
            unz_file_info FileInfo;
            nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
            CC_BREAK_IF(UNZ_OK != nRet);
    
            nRet = unzOpenCurrentFile(pFile);
            CC_BREAK_IF(UNZ_OK != nRet);
    
            pBuffer = new unsigned char[FileInfo.uncompressed_size];
            int nSize = 0;
            nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
            CCAssert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size, "the file size is wrong");
    
            *pSize = FileInfo.uncompressed_size;
            unzCloseCurrentFile(pFile);
        } while (0);
    
        if (pFile)
        {
            unzClose(pFile);
        }

            return pBuffer;
}

/**
 @brief   Generate a CCDictionary pointer by file
 @param   pFileName  The file name of *.plist file
 @return  The CCDictionary pointer generated from the file
 */
CCDictionary *CCFileUtils::dictionaryWithContentsOfFile(const char *pFileName)
{
    return ccFileUtils_dictionaryWithContentsOfFileThreadSafe(pFileName);
}

/*
 @brief The same meaning as dictionaryWithContentsOfFile(), but it doesn't call autorelease, so the
 invoker should call release().
 */
CCDictionary *CCFileUtils::dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
{
    return ccFileUtils_dictionaryWithContentsOfFileThreadSafe(pFileName);
}

/*
 @brief   Generate a CCMutableArray pointer by file
 @param   pFileName  The file name of *.plist file
 @return  The CCMutableArray pointer generated from the file
 */
CCArray *CCFileUtils::arrayWithContentsOfFile(const char *pFileName)
{
    return ccFileUtils_arrayWithContentsOfFileThreadSafe(pFileName);
}

/*
 @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
 invoker should call release().
 */
CCArray *CCFileUtils::arrayWithContentsOfFileThreadSafe(const char *pFileName)
{
    return ccFileUtils_arrayWithContentsOfFileThreadSafe(pFileName);
}
bool CCFileUtils::isFileExist(const char *fullfilename)
{
	FILE *fp = fopen(fullfilename, "r");
	if (fp == NULL)		return false;
	//
	fseek(fp,0,SEEK_END);
	int size = ftell(fp);
	fclose(fp);
    
	if (size == 0)	return false;
    
	return true;
}
bool CCFileUtils::isAbsolutePath(const std::string& strPath)
{
    if ((strlen(strPath.c_str()) > 1 && strPath.c_str()[1] == ':'))
        return true ;
    
	return strPath[0] == '/' ? true : false;
}/**
 *  Sets the array of search paths.
 *
 *  You can use this array to modify the search path of the resources.
 *  If you want to use "themes" or search resources in the "cache", you can do it easily by adding new entries in this array.
 *
 *  @note This method could access relative path and absolute path.
 *        If the relative path was passed to the vector, CCFileUtils will add the default resource directory before the relative path.
 *        For instance:
 *        	On Android, the default resource root path is "assets/".
 *        	If "/mnt/sdcard/" and "resources-large" were set to the search paths vector,
 *        	"resources-large" will be converted to "assets/resources-large" since it was a relative path.
 *
 *  @param searchPaths The array contains search paths.
 *  @see fullPathForFilename(const char*)
 *  @since v2.1
 */
void CCFileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool bExistDefaultRootPath = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string resourceRootPath = getResourcePath();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string resourceRootPath = getResourcePath();
    resourceRootPath.append("/");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string resourceRootPath ("./");
#endif
    m_searchPathArray.clear();
    m_fullPathCache.clear();
    for (std::vector<std::string>::const_iterator iter = searchPaths.begin(); iter != searchPaths.end(); ++iter)
    {
        std::string strPrefix;
        std::string path;
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            //strPrefix = resourceRootPath;
        }
        path = strPrefix+(*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!bExistDefaultRootPath && path == resourceRootPath)
        {
            bExistDefaultRootPath = true;
        }
        m_searchPathArray.push_back(path);
    }
    
    if (!bExistDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_searchPathArray.push_back(resourceRootPath);
    }
    
}
/**
 * Add search path.
 *
 * @since v2.1
 */
void CCFileUtils::addSearchPath(const char* path_)
{
    std::string strPrefix;
    std::string path(path_);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string resourceRootPath = getResourcePath();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string resourceRootPath =  getResourcePath();
    resourceRootPath.append("/");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string resourceRootPath ("./");
#endif
    if (!isAbsolutePath(path))
    { // Not an absolute path
        strPrefix = resourceRootPath;
    }
    path = strPrefix + path;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    m_searchPathArray.push_back(path);
}

/**
 *  Gets the array of search paths.
 *
 *  @return The array of search paths.
 *  @see fullPathForFilename(const char*).
 */
const std::vector<std::string>& CCFileUtils::getSearchPaths()
{
    return m_searchPathArray;
}
const char* CCFileUtils::fullPathFromRelativePath(const char *pszFileName)
//std::string CCFileUtils::fullPathForFilename(const char* pszFileName)
{
    CCAssert(pszFileName != NULL, "CCFileUtils: Invalid path");
    CCString* pRet = CCString::create("");
    std::string strFileName = pszFileName;
    if ( isAbsolutePath(pszFileName))
    {
        //CCLOG("Return absolute path( %s ) directly.", pszFileName);
        pRet->m_sString = pszFileName;
        return pRet->m_sString.c_str();
    }
    
    // Already Cached ?
    std::map<std::string, std::string>::iterator cacheIter = m_fullPathCache.find(pszFileName);
    if (cacheIter != m_fullPathCache.end())
    {
        //CCLOG("Return full path from cache: %s", cacheIter->second.c_str());
        pRet->m_sString = cacheIter->second.c_str();
        return pRet->m_sString.c_str();
        
    }
    
    // Get the new file name.
    std::string newFilename (pszFileName);
    
    std::string fullpath = "";
    
    for (std::vector<std::string>::iterator searchPathsIter = m_searchPathArray.begin();
         searchPathsIter != m_searchPathArray.end(); ++searchPathsIter) {
        
        
        //CCLOG("\n\nSEARCHING: %s, %s, %s", newFilename.c_str(), resOrderIter->c_str(), searchPathsIter->c_str());
        
        fullpath = this->getPathForFilename(newFilename, *searchPathsIter);
        
        if (fullpath.length() > 0)
        {
            // Using the filename passed in as key.
            m_fullPathCache.insert(std::pair<std::string, std::string>(pszFileName, fullpath));
            //CCLOG("Returning path: %s", fullpath.c_str());
            pRet->m_sString =  fullpath.c_str();
            return pRet->m_sString.c_str();
            
        }
        
    }
    // CCLOG("fullPathFromRelativePath :%s",pRet->m_sString.c_str());
    // The file wasn't found, return the file name passed in.
    pRet->m_sString = pszFileName;
    return pRet->m_sString.c_str();
}
std::string CCFileUtils::getPathForFilename(const std::string& filename, const std::string& searchPath)
{
    std::string path = searchPath;
    path +=filename ;
    //CCLOG("getPathForFilename, fullPath start = %s", path.c_str());
    if (!isFileExist(path.c_str())) {
        path = "";
    }
    
    // CCLOG("getPathForFilename, fullPath = %s", path.c_str());
    return path;
}

NS_CC_END

