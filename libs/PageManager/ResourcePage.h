#ifndef _iSeer_ResourcePage_h_
#define _iSeer_ResourcePage_h_

#include <map>
#include <set>
#include <string>
#include <sstream>
#include "ResourceBase.h"
#include "Thread.h"

using namespace std;

class ResourcePage :public Runnable
{
private:
	int							m_iID;
	string						m_strName;
	int							m_iPriority;
	int							m_iSize;
	bool						m_bCanDoMultiLoad;
	set<int>					m_setChildren;
	map<string, ResourceBase*>	m_mapContents;
	map<string, int>			m_mapContentsType;

protected:
	ResourcePage();

public:
	~ResourcePage();

	static ResourcePage*		createPage(stringstream &stream);

	void						run();

	/** load the whole page at once*/
	void						loadPage();

	/** load the page resource by resource
	* useful for pre-loading
	* return true if the page is completedly loaded
	* otherwise return false
	*/
	bool						loadPageInterval();
	void						unloadPage();

	bool						containsChild(int childID);

	bool						containsResource(const char *name);

	bool						canDoMultiLoad();

	int							getID();

	string						getName();

	int							getPriority();

	int							getSize();

	set<int>					getChildrenSet();
};

#endif