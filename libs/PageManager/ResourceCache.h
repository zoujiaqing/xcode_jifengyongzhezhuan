#ifndef _iSeer_ResourceCache_h_
#define _iSeer_ResourceCache_h_

#include <map>
#include <string>
#include "ResourceBase.h"

using namespace std;

class ResourceCache
{
public:
	static ResourceCache *			s_pSharedResourceCache;

private:
	map<string, ResourceBase*>		m_mapResources;
	map<string, ResourceBase*>		m_mapExResources;

	int								m_iUsedMemory;

protected:
	ResourceCache();

public:
	~ResourceCache();

	static ResourceCache *			sharedResourceCache();
	static void						purgeInstance();

	void							addResource(string name, ResourceBase *resource);
	void							removeResource(string name);
	ResourceBase*					getResource(string name);

	bool							isMemorySufficient();

	void							saveLog();
};

#endif