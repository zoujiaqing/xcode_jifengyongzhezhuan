#include "ResourcePlistDictionary.h"
#include "ResourceCache.h"

ResourcePlistDictionary::ResourcePlistDictionary()
	: ResourceBase()
{
	m_iType = ResPlistDictionary;
	m_pData = NULL;
}

ResourcePlistDictionary::~ResourcePlistDictionary()
{

}

ResourcePlistDictionary *ResourcePlistDictionary::resourceWithFile(const char *filePath)
{
	ResourcePlistDictionary *ret = new ResourcePlistDictionary();

	ret->m_strResourceName = filePath;
	ret->m_pData = CCFileUtils::dictionaryWithContentsOfFileThreadSafe(filePath);

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

void ResourcePlistDictionary::releaseResource()
{
	if(m_pData != NULL)
	{
		// remove from resource cache
		ResourceCache::sharedResourceCache()->removeResource(m_strResourceName);

		// call this to release the dictionary
		m_pData->release();
	}

	delete this;
}

CCDictionary *ResourcePlistDictionary::getData()
{
	return m_pData;
}

void ResourcePlistDictionary::setData(CCDictionary *data)
{
	if(m_pData != NULL)
	{
		m_pData->release();
	}

	m_pData = data;
}