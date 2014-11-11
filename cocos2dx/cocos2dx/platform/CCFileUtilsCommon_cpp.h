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
#ifndef __CC_PLATFORM_FILEUTILS_CPP__
#error "CCFileUtilsCommon_cpp.h can only be included for CCFileUtils.cpp in platform/win32(android,...)"
#endif /* __CC_PLATFORM_FILEUTILS_CPP__ */

#include "CCFileUtils.h"
#include "CCDirector.h"
#include "cocoa/CCDictionary.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/CocosPlayClient.h"

#endif
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

#include "cocoa/CCString.h"
#include "CCSAXParser.h"
#include "support/zip_support/unzip.h"

#include <stack>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

NS_CC_BEGIN

typedef enum 
{
    SAX_NONE = 0,
    SAX_KEY,
    SAX_DICT,
    SAX_INT,
    SAX_REAL,
    SAX_STRING,
    SAX_ARRAY
}CCSAXState;

typedef enum
{
    SAX_RESULT_NONE = 0,
    SAX_RESULT_DICT,
    SAX_RESULT_ARRAY
}CCSAXResult;

class CCDictMaker : public CCSAXDelegator
{
public:
    CCSAXResult m_eResultType;
    CCArray* m_pRootArray;
    CCDictionary *m_pRootDict;
    CCDictionary *m_pCurDict;
    std::stack<CCDictionary*> m_tDictStack;
    std::string m_sCurKey;   ///< parsed key
    std::string m_sCurValue; // parsed value
    CCSAXState m_tState;
    CCArray* m_pArray;

    std::stack<CCArray*> m_tArrayStack;
    std::stack<CCSAXState>  m_tStateStack;

public:
    CCDictMaker()        
        : m_eResultType(SAX_RESULT_NONE),
          m_pRootArray(NULL), 
          m_pRootDict(NULL),
          m_pCurDict(NULL),
          m_tState(SAX_NONE),
          m_pArray(NULL)
    {
    }

    ~CCDictMaker()
    {
    }

    CCDictionary* dictionaryWithContentsOfFile(const char *pFileName)
    {
        m_eResultType = SAX_RESULT_DICT;
        CCSAXParser parser;

        if (false == parser.init("UTF-8"))
        {
            return NULL;
        }
        parser.setDelegator(this);

        parser.parse(pFileName);
        return m_pRootDict;
    }

    CCArray* arrayWithContentsOfFile(const char* pFileName)
    {
        m_eResultType = SAX_RESULT_ARRAY;
        CCSAXParser parser;

        if (false == parser.init("UTF-8"))
        {
            return NULL;
        }
        parser.setDelegator(this);

        parser.parse(pFileName);
        return m_pArray;
    }

    void startElement(void *ctx, const char *name, const char **atts)
    {
        CC_UNUSED_PARAM(ctx);
        CC_UNUSED_PARAM(atts);
        std::string sName((char*)name);
        if( sName == "dict" )
        {
            m_pCurDict = new CCDictionary();
            if(m_eResultType == SAX_RESULT_DICT && m_pRootDict == NULL)
            {
                // Because it will call m_pCurDict->release() later, so retain here.
                m_pRootDict = m_pCurDict;
                m_pRootDict->retain();
            }
            m_tState = SAX_DICT;

            CCSAXState preState = SAX_NONE;
            if (! m_tStateStack.empty())
            {
                preState = m_tStateStack.top();
            }

            if (SAX_ARRAY == preState)
            {
                // add the dictionary into the array
                m_pArray->addObject(m_pCurDict);
            }
            else if (SAX_DICT == preState)
            {
                // add the dictionary into the pre dictionary
                CCAssert(! m_tDictStack.empty(), "The state is wrong!");
                CCDictionary* pPreDict = m_tDictStack.top();
                pPreDict->setObject(m_pCurDict, m_sCurKey.c_str());
            }

            m_pCurDict->release();

            // record the dict state
            m_tStateStack.push(m_tState);
            m_tDictStack.push(m_pCurDict);
        }
        else if(sName == "key")
        {
            m_tState = SAX_KEY;
        }
        else if(sName == "integer")
        {
            m_tState = SAX_INT;
        }
        else if(sName == "real")
        {
            m_tState = SAX_REAL;
        }
        else if(sName == "string")
        {
            m_tState = SAX_STRING;
        }
        else if (sName == "array")
        {
            m_tState = SAX_ARRAY;
            m_pArray = new CCArray();
            if (m_eResultType == SAX_RESULT_ARRAY && m_pRootArray == NULL)
            {
                m_pRootArray = m_pArray;
                m_pRootArray->retain();
            }
            CCSAXState preState = SAX_NONE;
            if (! m_tStateStack.empty())
            {
                preState = m_tStateStack.top();
            }

            if (preState == SAX_DICT)
            {
                m_pCurDict->setObject(m_pArray, m_sCurKey.c_str());
            }
            else if (preState == SAX_ARRAY)
            {
                CCAssert(! m_tArrayStack.empty(), "The state is wrong!");
                CCArray* pPreArray = m_tArrayStack.top();
                pPreArray->addObject(m_pArray);
            }
            m_pArray->release();
            // record the array state
            m_tStateStack.push(m_tState);
            m_tArrayStack.push(m_pArray);
        }
        else
        {
            m_tState = SAX_NONE;
        }
    }

    void endElement(void *ctx, const char *name)
    {
        CC_UNUSED_PARAM(ctx);
        CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
        std::string sName((char*)name);
        if( sName == "dict" )
        {
            m_tStateStack.pop();
            m_tDictStack.pop();
            if ( !m_tDictStack.empty())
            {
                m_pCurDict = m_tDictStack.top();
            }
        }
        else if (sName == "array")
        {
            m_tStateStack.pop();
            m_tArrayStack.pop();
            if (! m_tArrayStack.empty())
            {
                m_pArray = m_tArrayStack.top();
            }
        }
        else if (sName == "true")
        {
            CCString *str = new CCString("1");
            if (SAX_ARRAY == curState)
            {
                m_pArray->addObject(str);
            }
            else if (SAX_DICT == curState)
            {
                m_pCurDict->setObject(str, m_sCurKey.c_str());
            }
            str->release();
        }
        else if (sName == "false")
        {
            CCString *str = new CCString("0");
            if (SAX_ARRAY == curState)
            {
                m_pArray->addObject(str);
            }
            else if (SAX_DICT == curState)
            {
                m_pCurDict->setObject(str, m_sCurKey.c_str());
            }
            str->release();
        }
        else if (sName == "string" || sName == "integer" || sName == "real")
        {
            CCString* pStrValue = new CCString(m_sCurValue);

            if (SAX_ARRAY == curState)
            {
                m_pArray->addObject(pStrValue);
            }
            else if (SAX_DICT == curState)
            {
                m_pCurDict->setObject(pStrValue, m_sCurKey.c_str());
            }

            pStrValue->release();
            m_sCurValue.clear();
        }
        
        m_tState = SAX_NONE;
    }

    void textHandler(void *ctx, const char *ch, int len)
    {
        CC_UNUSED_PARAM(ctx);
        if (m_tState == SAX_NONE)
        {
            return;
        }

        CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
        CCString *pText = new CCString(std::string((char*)ch,0,len));

        switch(m_tState)
        {
        case SAX_KEY:
            m_sCurKey = pText->getCString();
            break;
        case SAX_INT:
        case SAX_REAL:
        case SAX_STRING:
            {
                if (curState == SAX_DICT)
                {
                    CCAssert(!m_sCurKey.empty(), "key not found : <integer/real>");
                }
                
                m_sCurValue.append(pText->getCString());
            }
            break;
        default:
            break;
        }
        pText->release();
    }
};

CCDictionary* ccFileUtils_dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
{
    CCDictMaker tMaker;
    return tMaker.dictionaryWithContentsOfFile(pFileName);
}

CCArray* ccFileUtils_arrayWithContentsOfFileThreadSafe(const char* pFileName)
{
    CCDictMaker tMaker;
    return tMaker.arrayWithContentsOfFile(pFileName);
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
bool CCFileUtils::isAbsolutePath(const std::string& strPath)
{
	 if ((strlen(strPath.c_str()) > 1 && strPath.c_str()[1] == ':'))
		 return true ;

	return strPath[0] == '/' ? true : false;
}
void CCFileUtils::setResourceDirectory(const char* pszResourceDirectory)
{
    m_obDirectory = pszResourceDirectory;
    if (m_obDirectory.size() > 0 && m_obDirectory[m_obDirectory.size() - 1] != '/')
    {
        m_obDirectory.append("/");
    }
}

const char* CCFileUtils::getResourceDirectory()
{
    return m_obDirectory.c_str();
}


//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_bPopupNotify = true;

void CCFileUtils::setPopupNotify(bool bNotify)
{
    s_bPopupNotify = bNotify;
}

bool CCFileUtils::isPopupNotify()
{
    return s_bPopupNotify;
}
bool CCFileUtils::isFileExist(const char *fullfilename)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	if (cocosplay::isEnabled() && !cocosplay::isDemo())
	{
	
		  return cocosplay::fileExists(fullfilename);
	
	}
	else
#endif
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

	
}
/** 
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
		    std::string resourceRootPath =  getResourcePath();
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
				 strPrefix = resourceRootPath;
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


	 std::string CCFileUtils::fullPathForFilename(const char* pszFileName)
	 {
		 CCAssert(pszFileName != NULL, "CCFileUtils: Invalid path");

		 std::string strFileName = pszFileName;
		 if (isAbsolutePath(pszFileName))
		 {
			 //CCLOG("Return absolute path( %s ) directly.", pszFileName);
			 return pszFileName;
		 }

		 // Already Cached ?
		 std::map<std::string, std::string>::iterator cacheIter = m_fullPathCache.find(pszFileName);
		 if (cacheIter != m_fullPathCache.end())
		 {
			 //CCLOG("Return full path from cache: %s", cacheIter->second.c_str());
			 return cacheIter->second;
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
							 return fullpath;
						 }
				 
		 }

		 //CCLOG("cocos2d: fullPathForFilename: No file found at %s. Possible missing file.", pszFileName);

		 // The file wasn't found, return the file name passed in.
		 return pszFileName;
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

#endif // (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
