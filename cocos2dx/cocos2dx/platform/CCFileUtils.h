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
#ifndef __CC_FILEUTILS_PLATFORM_H__
#define __CC_FILEUTILS_PLATFORM_H__

#include <string>
#include <vector>
#include <map>
#include "CCPlatformMacros.h"
#include "ccTypes.h"

NS_CC_BEGIN

	class CCDictionary;
	class CCArray;

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class CC_DLL CCFileUtils
{
public:
    static CCFileUtils* sharedFileUtils();
    static void purgeFileUtils();

    void purgeCachedEntries();
    /**
    @brief Get resource file data
    @param[in]  pszFileName The resource file name which contains the path.
    @param[in]  pszMode The read mode of the file.
    @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
    @return Upon success, a pointer to the data is returned, otherwise NULL.
    @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
    */
    unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);

    /**
    @brief Get resource file data from a zip file.
    @param[in]  pszFileName The resource file name which contains the relative path of the zip file.
    @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
    @return Upon success, a pointer to the data is returned, otherwise NULL.
    @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
    */
    unsigned char* getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize);

    /**
    @brief   Generate the absolute path of the file.
    @param   pszRelativePath     The relative path of the file.
    @return  The absolute path of the file.
    @warning We only add the ResourcePath before the relative path of the file.
    If you have not set the ResourcePath, the function appends "/NEWPLUS/TDA_DATA/UserData/" by default.
    You can set ResourcePath with void setResourcePath(const char *pszResourcePath);
    */
    const char* fullPathFromRelativePath(const char *pszRelativePath);

	/**
    @brief   Generate the absolute path of the (additionalPath + file).
    @param   pszRelativePath     The relative path of the file.
	@param   additionalPath      The additional path of before the file.
    @return  The absolute path of the file.
    @warning We only add the ResourcePath before the relative path of the file.
    If you have not set the ResourcePath, the function appends "/NEWPLUS/TDA_DATA/UserData/" by default.
    You can set ResourcePath with void setResourcePath(const char *pszResourcePath);
	The result is like : ResourcePath + additionalPath + pszRelativePath
    */
	const char* fullPathFromRelativePath(const char *pszRelativePath,const char * additionalPath);

    /// @cond
    const char* fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile);
    /// @endcond

    /**
    @brief  Set the resource directory; we will find resources relative to this directory.
    @param pszDirectoryName  Relative path to root.
    */
    void setResourceDirectory(const char *pszDirectoryName);

    /**
    @brief  Get the resource directory
    */
    const char* getResourceDirectory();

    /**
    @brief   Get the writeable path
    @return  The path that can write/read file
    */
    std::string getWriteablePath();

	/**
    @brief   Generate a CCDictionary pointer by file
    @param   pFileName  The file name of *.plist file
    @return  The CCDictionary pointer generated from the file
    */
    static CCDictionary *dictionaryWithContentsOfFile(const char *pFileName);
    
    /*
     @brief The same meaning as dictionaryWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     */
	static CCDictionary *dictionaryWithContentsOfFileThreadSafe(const char *pFileName);
    
    /*
    @brief   Generate a CCMutableArray pointer by file
    @param   pFileName  The file name of *.plist file
    @return  The CCMutableArray pointer generated from the file
    */
    static CCArray *arrayWithContentsOfFile(const char *pFileName);
    
    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     */
	bool isAbsolutePath(const std::string& strPath) ;
	static CCArray *arrayWithContentsOfFileThreadSafe(const char *pFileName);
	 const char *getResourcePath();
	///////////////////////////////////////////////////
	// check whether file exist
	///////////////////////////////////////////////////
	static bool isFileExist(const char *fullfilename);

    /**
    @brief Set/Get whether pop-up a message box when the image load failed
    */
    void setPopupNotify(bool bNotify);
    bool isPopupNotify();
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
    virtual void setSearchPaths(const std::vector<std::string>& searchPaths);
    
    /**
      * Add search path.
      *
      * @since v2.1
      */
     void addSearchPath(const char* path);
    
    /**
     *  Gets the array of search paths.
     *  
     *  @return The array of search paths.
     *  @see fullPathForFilename(const char*).
     */
    virtual const std::vector<std::string>& getSearchPaths();
	 std::string fullPathForFilename(const char* pszFileName) ;
	 std::string getPathForFilename(const std::string& filename, const std::string& searchPath);
	 
    
protected:
    CCFileUtils(void)
    {
    }
    
    std::string m_obDirectory;
    /**
     * The vector contains search paths.
     * The lower index of the element in this vector, the higher priority for this search path.
     */
    std::vector<std::string> m_searchPathArray;
	std::map<std::string, std::string> m_fullPathCache;
};

// end of platform group
/// @}

class CCFileData
{
public:
	CCFileData(const char* pszFileName, const char* pszMode)
		: m_pBuffer(0)
		, m_uSize(0)
	{
		m_pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pszFileName, pszMode, &m_uSize);
	}
	~CCFileData()
	{
		CC_SAFE_DELETE_ARRAY(m_pBuffer);
	}

	bool reset(const char* pszFileName, const char* pszMode)
	{
		CC_SAFE_DELETE_ARRAY(m_pBuffer);
		m_uSize = 0;
		m_pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pszFileName, pszMode, &m_uSize);
		return (m_pBuffer) ? true : false;
	}

	CC_SYNTHESIZE_READONLY(unsigned char *, m_pBuffer, Buffer);
	CC_SYNTHESIZE_READONLY(unsigned long ,  m_uSize,   Size);
};

NS_CC_END

#endif    // __CC_FILEUTILS_PLATFORM_H__
