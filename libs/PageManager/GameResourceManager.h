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
#pragma once

#include <string>
#include "cocos2d.h"
#include "IExResourceMgr.h"
#include "UpdateResourceDelegate.h"

class AssetsManager;

USING_NS_CC;

class GameResourceManager : public IExResourceMgr, public NetworkOperationDelegate

{
public:
	static GameResourceManager *sharedManager();
	static void purgeInstance();

	~GameResourceManager();

	void updateResource(const std::string& version = "1.0.0.0");

	/**need relative path!
	*absolute path passed in will give out wrong result!
	*/
	const char *writablePathFromRelativePath(const char *path);
	/**
	* Instruction : 
	* @param 
	*/
	const char* androidPathFromRelativePath(const char* path);
	/**need relative path!
	*absolute path passed in will give out wrong result!
	*/
	const char *storedFullPathFromRelativePath(const char *path);

	/**need relative path! relative path = additionalPath + path
	*absolute path passed in will give out wrong result!
	*/
	const char *storedFullPathFromRelativePath(const char *additionalPath, const char *path);

	/**need absolute path or it'll always fail to read data from cache*/
	CCArray *		arrayFromCache(const char *path);
	/**need absolute path or it'll always fail to read data from cache*/
	CCDictionary *	dictionaryFromCache(const char *path);

	/**need absolute path or it'll always fail to read data from cache*/
	void			addSpriteFrameWithDictionaryFromCache(const char *path);
	/**need absolute path or it'll always fail to read data from cache*/
	void			removeSpriteFrameWithDictionaryFromCache(const char *path);

	/*
	*@prototype, loadXml()
	*@fileName,
	*@isEncrypt,
	*@note,
	*		注意当调用这个函数后，需要自己对返回的指针进行释放
	*/
	const char* loadXml(const char *fileName,bool isEncrypt = true);
    
    bool uncompress(const std::string& storePath, const std::string& filename);
    
    virtual void operationDidFinish(AssetsManager *operation, const char* strVal);
    virtual void operationDidFail(AssetsManager *operation);
    const std::string& getStorepath() { return m_storagePath; }
    
    CCRenderTexture* createStroke(CCLabelTTF* label, float size, ccColor3B cor, GLubyte opacity);

	bool createDirectory(const char *path);

	
	 void    initSearchPath();
private:
	void init();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void loadSDCardInfo();
#endif
    
    void createDownloadedDir();        
private:
	static GameResourceManager *s_pSharedManager;
    
	unsigned char m_xmlEncryptKey[16];
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool m_bHasSDCard;
	std::string m_strSDCardPath;
#endif
    
    //! The path to store downloaded resources.
    std::string m_storagePath;
    AssetsManager *m_pAssetsManager;
	

};


