#include "ResourceCache.h"
#include "cocos2d.h"

USING_NS_CC;

#define MAX_CACHE_MEMORY 20*1024*1024

ResourceCache *ResourceCache::s_pSharedResourceCache = NULL;

ResourceCache::ResourceCache()
{
	m_mapResources.clear();
	m_iUsedMemory = 0;
}

ResourceCache::~ResourceCache()
{
	map<string, ResourceBase*>::iterator it = m_mapResources.begin();
	while( it != m_mapResources.end())
	{
		it->second->releaseResource();
		it = m_mapResources.begin();
	}
}

ResourceCache *ResourceCache::sharedResourceCache()
{
	if(s_pSharedResourceCache == NULL)
	{
		s_pSharedResourceCache = new ResourceCache();
	}

	return s_pSharedResourceCache;
}

void ResourceCache::purgeInstance()
{
	if(s_pSharedResourceCache)
	{
		delete s_pSharedResourceCache;
		s_pSharedResourceCache = NULL;
	}
}

void ResourceCache::addResource(string name, ResourceBase *resource)
{
	if(resource == NULL)
	{
		CCLOG("invalid resource of: %s", name.c_str());
		return;
	}

	if(m_mapResources.find(name) == m_mapResources.end())
	{
		m_mapResources[name] = resource;
		m_iUsedMemory += resource->getSize();
	}
	else
	{
		CCLOG("resource %s already loaded", name.c_str());
	}
}

void ResourceCache::removeResource(string name)
{
	map<string, ResourceBase*>::iterator it = m_mapResources.find(name);

	if(it != m_mapResources.end())
	{
		m_iUsedMemory -= it->second->getSize();
		m_mapResources.erase(it);
	}
	else
	{
		CCLOG("resource %s is not in the cache", name.c_str());
	}
}

ResourceBase *ResourceCache::getResource(string name)
{	
	map<string, ResourceBase*>::iterator it = m_mapResources.find(name);

	if(it != m_mapResources.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

bool ResourceCache::isMemorySufficient()
{
	if(m_iUsedMemory <= MAX_CACHE_MEMORY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ResourceCache::saveLog()
{
	// TODO
}