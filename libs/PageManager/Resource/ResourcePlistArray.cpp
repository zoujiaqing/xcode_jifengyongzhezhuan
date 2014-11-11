#include "ResourcePlistArray.h"
#include "ResourceCache.h"

ResourcePlistArray::ResourcePlistArray()
	: ResourceBase()
{
	m_iType = ResPlistArray;
	m_pData = NULL;
}

ResourcePlistArray::~ResourcePlistArray()
{

}

ResourcePlistArray *ResourcePlistArray::resourceWithFile(const char *filePath)
{
	ResourcePlistArray *ret = new ResourcePlistArray();

	ret->m_strResourceName = filePath;
	ret->m_pData = CCFileUtils::arrayWithContentsOfFileThreadSafe(filePath);

	if(ret->m_pData != NULL)
	{
		CCFileData data(filePath, "rb");
		ret->m_iSize = data.getSize();

		ResourceCache::sharedResourceCache()->addResource(filePath, ret);
	}
	else
	{
		delete ret;
		ret = NULL;
	}

	return ret;
}

void ResourcePlistArray::releaseResource()
{
	if(m_pData != NULL)
	{
		// remove from resource cache
		ResourceCache::sharedResourceCache()->removeResource(m_strResourceName);

		// call release to free the array
		m_pData->release();
	}

	delete this;
}

CCArray *ResourcePlistArray::getData()
{
	return m_pData;
}

void ResourcePlistArray::setData(CCArray *data)
{
	if(m_pData != NULL)
	{
		m_pData->release();
	}

	m_pData = data;
}