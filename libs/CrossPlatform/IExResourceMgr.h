//
//  IExResourceMgr.h
//  iSeer
//
//  Created by razer tong on 12-5-15.
//  Copyright (c) 2012Äê __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_IExResourceMgr_h
#define iSeer_IExResourceMgr_h
#include <string>
#include "cocos2d.h"

USING_NS_CC;


class IExResourceMgr
{
public:
     	/**need relative path!
	*absolute path passed in will give out wrong result!
	*/
	virtual const char *writablePathFromRelativePath(const char *path) = 0;
	/**need relative path!
	*absolute path passed in will give out wrong result!
	*/
	virtual const char *storedFullPathFromRelativePath(const char *path) = 0;

	/**need absolute path or it'll always fail to read data from cache*/
	virtual CCArray *	arrayFromCache(const char *path) = 0;

	/**need absolute path or it'll always fail to read data from cache*/
	virtual CCDictionary*	dictionaryFromCache(const char *path) = 0;
};

#endif
