#include <map>
#include <set>
#include <string>
#ifndef _iSeer_ResourcePageManager_h_
#define _iSeer_ResourcePageManager_h_

#if (!defined ANDROID && !defined PATH_MAX)
#define PATH_MAX 512
#endif

#include "cocos2d.h"
#include "ResourcePage.h"
#include "Thread.h"

USING_NS_CC;

class ResourcePageManager : public CCObject
{
public:
	static ResourcePageManager *		s_pSharedManager;

private:
	int									m_iCurPage;
	int									m_iIntervalLoadingPage;
	int									m_iMultiLoadingPage;

	map<int, ResourcePage*>				m_mapAllPages;

	set<int>							m_setLoadedPages;
	set<int>							m_setLoadingPages;
	set<int>							m_setToBeIntervalLoadedPages;
	set<int>							m_setToBeMultiLoadedPages;

	Thread *							m_pThread;

protected:
	ResourcePageManager();

public:
	~ResourcePageManager();

	static ResourcePageManager *		sharedManager();
	static void							purgeInstance();

	void								onPageChanged(int pageID);

	ResourcePage *						getPage(int pageID);

	void								update(float dt);

	int									findPageContainesResource(const char *name);

private:
	bool								init();

	/** load the page at once*/
	void								loadPage(int pageID);

	/** load the page resource by resource
	* useful for pre-loading
	*/
	void								loadPageInterval(int pageID);

	/** load the page in a new thread
	* useful for pre-loading of page without texture resources
	*/
	void								loadPageMultiThread(int pageID);

	void								unloadPage(int pageID);

	void								onIntervalLoadingFinished(int pageID);
	void								onMultiLoadingFinished(int pageID);
};

#endif