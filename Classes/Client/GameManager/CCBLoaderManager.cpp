#include "CCBLoaderManager.h"

static CCBLoaderManager* m_instance = NULL;
CCBLoaderManager::CCBLoaderManager()
{

}

CCBLoaderManager::~CCBLoaderManager()
{

}

CCBLoaderManager* CCBLoaderManager::getInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new CCBLoaderManager();
	}
	return m_instance;
}

// load ccnode from ccbi
CCNode* CCBLoaderManager::LoadFromCCBI(CCNodeLoader* loader, const char* className, const char* ccbFileName)
{
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader(className, loader);
	CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
	std::string path = "ccbi/";
	path.append(ccbFileName);
	CCNode* node = ccbReader->readNodeGraphFromFile(path.c_str());
    delete ccbReader;
	return node;
}

//load the ccbi file
CCNode* CCBLoaderManager::LoadFromDoubleCCBI(const char* ccbFileName, std::vector<const char *>ccbLayerNames, std::vector<CCNodeLoader*> cclayerLoader)
{
    CCNodeLoaderLibrary* pNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CCBReader* ccbReader = new CCBReader(pNodeLoaderLibrary);

    for(int index = 0; index < cclayerLoader.size(); index++)
    {
        pNodeLoaderLibrary->registerCCNodeLoader(ccbLayerNames[index], cclayerLoader[index]);
    }

    std::string path = "ccbi/";
    path.append(ccbFileName);
    CCNode* node = ccbReader->readNodeGraphFromFile(path.c_str());

    delete ccbReader;
    return node;
}