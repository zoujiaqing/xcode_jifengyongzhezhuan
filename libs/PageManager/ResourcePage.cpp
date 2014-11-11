#include "ResourcePage.h"
#include "ResourceCache.h"
#include "ResourcePageManager.h"
#include "GameResourceManager.h"
#include "ResourceTexture.h"
#include "ResourcePlistDictionary.h"
#include "ResourcePlistArray.h"
#include "cocos2d.h"

USING_NS_CC;

ResourcePage::ResourcePage()
{
	m_iID = -1;
	m_strName = "";
	m_iPriority = -1;
	m_iSize = 0;
	m_bCanDoMultiLoad = false;
	m_setChildren.clear();
	m_mapContents.clear();
	m_mapContentsType.clear();
}

ResourcePage::~ResourcePage()
{

}

ResourcePage *ResourcePage::createPage(stringstream &stream)
{
	ResourcePage *ret = new ResourcePage();

	bool hasError = false;

	do 
	{
		if(stream.eof())
		{
			hasError = true;
			break;
		}

		stream >> ret->m_iID;

		if(stream.eof())
		{
			hasError = true;
			break;
		}

		stream >> ret->m_strName;

		if(stream.eof())
		{
			hasError = true;
			break;
		}

		stream >> ret->m_iPriority;

		if(stream.eof())
		{
			hasError = true;
			break;
		}

		stream >> ret->m_bCanDoMultiLoad;

		if(stream.eof())
		{
			hasError = true;
			break;
		}

		int childrenCount = 0;
		stream >> childrenCount;
		
		for(int i = 0; i < childrenCount; i++)
		{
			if(stream.eof())
			{
				hasError = true;
				break;
			}

			int childrenID = 0;
			stream >> childrenID;
			ret->m_setChildren.insert(childrenID);
		}

		if(stream.eof())
		{
			hasError = true;
			break;
		}

		int contentCount = 0;
		stream >> contentCount;

		for(int i = 0; i < contentCount; i++)
		{
			if(stream.eof())
			{
				hasError = true;
				break;
			}

			string contentName = "";
			stream >> contentName;

			if(stream.eof())
			{
				hasError = true;
				break;
			}

			string typeName = "";
			stream >> typeName;

			int type = ResBase;

			contentName = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(contentName.c_str());

			if(typeName == "texture")
			{
				type = ResTexture;
			}
			else if(typeName == "plist_array")
			{
				type = ResPlistArray;
			}
			else if(typeName == "plist_dictionary")
			{
				type = ResPlistDictionary;
			}
			else
			{
				hasError = true;
			}

			ret->m_mapContentsType[contentName] = type;
			ret->m_mapContents[contentName] = NULL;
		}
	} while (0);

	if(hasError)
	{
		delete ret;
		ret = NULL;
	}

	return ret;
}

void ResourcePage::run()
{
	loadPage();
}

void ResourcePage::loadPage()
{
	map<string, ResourceBase*>::iterator it = m_mapContents.begin();
	for(; it != m_mapContents.end(); it++)
	{
		if(it->second == NULL)
		{
			string name = it->first;
			int type = m_mapContentsType[name];

			switch(type)
			{
			case ResTexture:
				{
					ResourceTexture *res = ResourceTexture::resourceWithFile(name.c_str());
					it->second = res;
				}
				break;

			case ResPlistDictionary:
				{
					ResourcePlistDictionary *res = ResourcePlistDictionary::resourceWithFile(name.c_str());
					it->second = res;
				}
				break;

			case ResPlistArray:
				{
					ResourcePlistArray *res = ResourcePlistArray::resourceWithFile(name.c_str());
					it->second = res;
				}
				break;

			default:
				{
					CCLOG("error resource type");
				}
				break;
			}
		}
	}
}

bool ResourcePage::loadPageInterval()
{
	map<string, ResourceBase*>::iterator it = m_mapContents.begin();
	for(; it != m_mapContents.end(); it++)
	{
		if(it->second == NULL)
		{
			string name = it->first;
			int type = m_mapContentsType[name];
			bool resourceLoaded = false;

			switch(type)
			{
			case ResTexture:
				{
					ResourceTexture *res = ResourceTexture::resourceWithFile(name.c_str());
					it->second = res;
					resourceLoaded = true;
				}
				break;

			case ResPlistDictionary:
				{
					ResourcePlistDictionary *res = ResourcePlistDictionary::resourceWithFile(name.c_str());
					it->second = res;
					resourceLoaded = true;
				}
				break;

			case ResPlistArray:
				{
					ResourcePlistArray *res = ResourcePlistArray::resourceWithFile(name.c_str());
					it->second = res;
					resourceLoaded = true;
				}
				break;

			default:
				{
					CCLOG("error resource type");
				}
				break;
			}

			if(resourceLoaded)
			{
				if(++it == m_mapContents.end())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
    
    return true;
}

void ResourcePage::unloadPage()
{	
	map<string, ResourceBase*>::iterator it = m_mapContents.begin();

	for(; it != m_mapContents.end(); it++)
	{
		if(it->second != NULL)
		{
			it->second->releaseResource();
			it->second = NULL;
		}
	}
}

bool ResourcePage::containsChild(int childID)
{
	return (m_setChildren.find(childID) != m_setChildren.end());
}

bool ResourcePage::containsResource(const char *name)
{
	return (m_mapContents.find(name) != m_mapContents.end());
}

bool ResourcePage::canDoMultiLoad()
{
	return m_bCanDoMultiLoad;
}

int ResourcePage::getID()
{
	return m_iID;
}

string ResourcePage::getName()
{
	return m_strName;
}

int ResourcePage::getPriority()
{
	return m_iPriority;
}

int ResourcePage::getSize()
{
	return m_iSize;
}

set<int> ResourcePage::getChildrenSet()
{
	return m_setChildren;
}