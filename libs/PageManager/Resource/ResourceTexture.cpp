#include "ResourceTexture.h"
#include "ResourceCache.h"

ResourceTexture::ResourceTexture()
	: ResourceBase()
{
	m_iType = ResTexture;
	m_pData = NULL;
}

ResourceTexture::~ResourceTexture()
{

}

ResourceTexture *ResourceTexture::resourceWithFile(const char *filePath)
{
	ResourceTexture *ret = new ResourceTexture();

	ret->m_strResourceName = filePath;
	ret->m_pData = CCTextureCache::sharedTextureCache()->addImage(filePath);

	if(ret->m_pData != NULL)
	{
		int contentWidth = ret->m_pData->getPixelsWide();
		int contentHeight = ret->m_pData->getPixelsHigh();
		ret->m_iSize = contentWidth * contentHeight * (ret->m_pData->bitsPerPixelForFormat() / 8);

		//ensure that the texture won't be removed by CCTextureCache::removeUnusedTexture()
		ret->m_pData->retain();

		ResourceCache::sharedResourceCache()->addResource(filePath, ret);
	}
	else
	{
		delete ret;
		ret = NULL;
	}

	return ret;
}

void ResourceTexture::releaseResource()
{
	if(m_pData != NULL)
	{
		// remove from resource cache
		ResourceCache::sharedResourceCache()->removeResource(m_strResourceName);
	
		// remove potential sprite frames associated with the texture
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromTexture(m_pData);

		// remove from texture cache
		CCTextureCache::sharedTextureCache()->removeTexture(m_pData);

		// need this because we retained the texture
		m_pData->release();
	}

	delete this;
}

CCTexture2D *ResourceTexture::getData()
{
	return m_pData;
}

void ResourceTexture::setData(CCTexture2D* data)
{
	if(m_pData != NULL)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromTexture(m_pData);
		CCTextureCache::sharedTextureCache()->removeTexture(m_pData);
		m_pData->release();
	}

	m_pData = data;
}