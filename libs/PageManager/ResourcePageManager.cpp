#include <stdio.h>
#include <sstream>
#include <string>
#include "ResourcePageManager.h"
#include "GameResourceManager.h"

using namespace std;

#define PAGE_CONFIG "PageConfig.txt"
#define UPDATE_DELTA_FRAMES 5

ResourcePageManager *ResourcePageManager::s_pSharedManager = NULL;

ResourcePageManager::ResourcePageManager()
{
	m_iCurPage = -1;
	m_iIntervalLoadingPage = -1;
	m_iMultiLoadingPage = -1;
	m_mapAllPages.clear();
	m_setLoadedPages.clear();
	m_setLoadingPages.clear();
	m_setToBeIntervalLoadedPages.clear();
	m_setToBeMultiLoadedPages.clear();

	m_pThread = NULL;

	float fps = CCDirector::sharedDirector()->getAnimationInterval();

	// register update function while initialization is completed
	if(init())
	{
		CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();

		pScheduler->scheduleSelector(
			schedule_selector(ResourcePageManager::update),
			this,
			(float)UPDATE_DELTA_FRAMES * fps,
			false
			);
	}
}

ResourcePageManager::~ResourcePageManager()
{
	map<int, ResourcePage*>::iterator it = m_mapAllPages.begin();
	for(; it != m_mapAllPages.end(); it++)
	{
		if(it->second != NULL)
		{
			it->second->unloadPage();
			delete it->second;
			it->second = NULL;
		}
	}
}

ResourcePageManager *ResourcePageManager::sharedManager()
{
	if(s_pSharedManager == NULL)
	{
		s_pSharedManager = new ResourcePageManager();
	}

	return s_pSharedManager;
}

void ResourcePageManager::purgeInstance()
{
	if(s_pSharedManager)
	{
		delete s_pSharedManager;
		s_pSharedManager = NULL;
	}
}

bool ResourcePageManager::init()
{
	string configFile = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(PAGE_CONFIG);
	CCFileData data(configFile.c_str(), "rb");

	int size = data.getSize();

	if(size == 0)
	{
		CCLOG("Can't find the page config file!");
		return false;
	}

	char *buff = new char[size + 1];
	strncpy(buff, (const char*)data.getBuffer(), size);
	buff[size] = 0;

	stringstream fileStream;
	fileStream << buff;
    
    delete [] buff;

	while(!fileStream.eof())
	{
		string str = "";
		fileStream >> str;

		if(str == "page")
		{
			ResourcePage *newPage = ResourcePage::createPage(fileStream);

			if(newPage == NULL)
			{
				CCLOG("Some error occurs when analyzing page config file! Stop init.");
				return false;
			}
			else
			{
				int ID = newPage->getID();

				if(m_mapAllPages.find(ID) != m_mapAllPages.end())
				{
					CCLOG("page #%d already exists", ID);
					delete newPage;
					newPage = NULL;
				}
				else
				{
					m_mapAllPages[ID] = newPage;
				}
			}
		}
	}
    
    return true;
}

void ResourcePageManager::onPageChanged(int pageID)
{
	// TODO
	// for test now
	m_setToBeMultiLoadedPages.insert(pageID);

	onMultiLoadingFinished(-1);
}

ResourcePage *ResourcePageManager::getPage(int pageID)
{
	if(m_mapAllPages.find(pageID) != m_mapAllPages.end())
	{
		return m_mapAllPages[pageID];
	}
	else
	{
		return NULL;
	}
}

void ResourcePageManager::update(float dt)
{
	// load interval loading page
	if(m_iIntervalLoadingPage != -1)
	{
		loadPageInterval(m_iIntervalLoadingPage);
	}

	// check multi thread loading page
	if(m_iMultiLoadingPage != -1)
	{
		// no thread is running means the first time we do multi loading
		if(m_pThread == NULL)
		{
			loadPageMultiThread(m_iMultiLoadingPage);
		}
		else
		{
			// multi thread loading finished
			if(m_pThread->getState() == Thread::THREAD_STATUS_EXIT)
			{
				onMultiLoadingFinished(m_iMultiLoadingPage);

				delete m_pThread;
				m_pThread = NULL;

				if(m_iMultiLoadingPage != -1)
				{
					loadPageMultiThread(m_iMultiLoadingPage);
				}
			}
		}
	}
}

int ResourcePageManager::findPageContainesResource(const char *name)
{
	map<int, ResourcePage*>::iterator iter = m_mapAllPages.begin();

	for(; iter != m_mapAllPages.end(); iter++)
	{
		if(iter->second->containsResource(name))
		{
			return iter->first;
		}
	}

	return -1;
}

void ResourcePageManager::loadPage(int pageID)
{
	if(m_mapAllPages.find(pageID) == m_mapAllPages.end())
	{
		CCLOG("unknown page #%d, ignore", pageID);
		return;
	}

	if(m_setLoadedPages.find(pageID) != m_setLoadedPages.end())
	{
		CCLOG("page #%d already loaded", pageID);
		return;
	}

	if(m_setLoadingPages.find(pageID) != m_setLoadingPages.end())
	{
		CCLOG("page #%d is currently loading", pageID);
		return;
	}

	m_mapAllPages[pageID]->loadPage();
	m_setLoadedPages.insert(pageID);
	
	set<int>::iterator it;

	it = m_setToBeIntervalLoadedPages.find(pageID);
	if(it != m_setToBeIntervalLoadedPages.end())
	{
		m_setToBeIntervalLoadedPages.erase(it);
	}

	it = m_setToBeMultiLoadedPages.find(pageID);
	if(it != m_setToBeMultiLoadedPages.end())
	{
		m_setToBeMultiLoadedPages.erase(it);
	}
}

void ResourcePageManager::loadPageInterval(int pageID)
{
	if(m_mapAllPages.find(pageID) == m_mapAllPages.end())
	{
		CCLOG("unknown page #%d, ignore", pageID);
		onIntervalLoadingFinished(-1);
		return;
	}

	if(m_setLoadedPages.find(pageID) != m_setLoadedPages.end())
	{
		CCLOG("page #%d already loaded", pageID);
		onIntervalLoadingFinished(-1);
		return;
	}

	bool finished = m_mapAllPages[pageID]->loadPageInterval();

	if(finished)
	{
		onIntervalLoadingFinished(pageID);
	}
}

void ResourcePageManager::loadPageMultiThread(int pageID)
{
	if(m_mapAllPages.find(pageID) == m_mapAllPages.end())
	{
		CCLOG("unknown page #%d, ignore", pageID);
		onMultiLoadingFinished(-1);
		return;
	}

	if(m_setLoadedPages.find(pageID) != m_setLoadedPages.end())
	{
		CCLOG("page #%d already loaded", pageID);
		onMultiLoadingFinished(-1);
		return;
	}

	if(m_setLoadingPages.find(pageID) != m_setLoadingPages.end())
	{
		CCLOG("page #%d is currently loading", pageID);
		onMultiLoadingFinished(-1);
		return;
	}

	ResourcePage *toBeLoaded = m_mapAllPages[pageID];

	if(!toBeLoaded->canDoMultiLoad())
	{
		CCLOG("page #%d can not be loaded by multi thread");
		onMultiLoadingFinished(-1);
		return;
	}

	if(m_pThread)
	{
		if(m_pThread->getState() == Thread::THREAD_STATUS_RUNNING)
		{
			CCLOG("last multi thread loading not finished. Can't start new loading.");
			return;
		}

		delete m_pThread;
	}

	m_pThread = new Thread(toBeLoaded);

	m_setLoadingPages.insert(pageID);

	m_pThread->start();
}

void ResourcePageManager::unloadPage(int pageID)
{
	if(m_mapAllPages.find(pageID) == m_mapAllPages.end())
	{
		CCLOG("unknown page #%d, ignore", pageID);
		return;
	}

	if(m_setLoadedPages.find(pageID) == m_setLoadedPages.end())
	{
		CCLOG("page #%d not loaded", pageID);
		return;
	}

	m_mapAllPages[pageID]->unloadPage();
	m_setLoadedPages.erase(m_setLoadedPages.find(pageID));
}

void ResourcePageManager::onIntervalLoadingFinished(int pageID)
{
	set<int>::iterator it;

	// m_iIntervalLoadingPage is always in the m_setLoadingPages unless it is -1
	it = m_setLoadingPages.find(m_iIntervalLoadingPage);
	if(it != m_setLoadingPages.end())
	{
		m_setLoadingPages.erase(it);
	}

	if(pageID != -1)
	{
		m_setLoadedPages.insert(pageID);

		m_iIntervalLoadingPage = -1;
	}

	if(!m_setToBeIntervalLoadedPages.empty())
	{
		it = m_setToBeIntervalLoadedPages.begin();
		m_iIntervalLoadingPage = *it;
		m_setToBeIntervalLoadedPages.erase(it);
		m_setLoadingPages.insert(m_iIntervalLoadingPage);
	}
}

void ResourcePageManager::onMultiLoadingFinished(int pageID)
{
	set<int>::iterator it;

	if(pageID != -1)
	{
		m_setLoadedPages.insert(pageID);
		
		it = m_setLoadingPages.find(pageID);
		if(it != m_setLoadingPages.end())
		{
			m_setLoadingPages.erase(it);
		}

		m_iMultiLoadingPage = -1;
	}

	if(!m_setToBeMultiLoadedPages.empty())
	{
		it = m_setToBeMultiLoadedPages.begin();
		m_iMultiLoadingPage = *it;
		m_setToBeMultiLoadedPages.erase(it);
	}
}