#ifndef _CCBLOADER_MANAGER_H_
#define _CCBLOADER_MANAGER_H_
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class CCBLoaderManager : public CCObject
{
public:
	CCBLoaderManager();
	virtual ~CCBLoaderManager();

	static CCBLoaderManager* getInstance();
	CCNode* LoadFromCCBI(CCNodeLoader* loader, const char* className, const char* ccbFileName);
    CCNode* LoadFromDoubleCCBI(const char* ccbFileName, std::vector<const char *>ccbLayerNames, std::vector<CCNodeLoader*> cclayerLoader);
private:
	
};
#endif