#include <libxml/parser.h>

#include "MapDataManager.h"
#include "GameResourceManager.h"
#include "Define.h"

using namespace std;

MapDataManager* MapDataManager::s_pSharedManager = NULL;

MapDataManager::MapDataManager()
{
}

MapDataManager::~MapDataManager()
{
	for(std::map<int, unsigned char*>::iterator iter = m_physicsFileBuffer.begin(); iter != m_physicsFileBuffer.end(); iter++)
	{
		delete[] iter->second;
		iter->second = NULL;
	}

	m_physicsFileBuffer.clear();
}



unsigned char* MapDataManager::getMapFileByMapID(const int mapID) 
{
	// TODO : 
	if(m_physicsFileBuffer.find(mapID) != m_physicsFileBuffer.end())
	{
		return m_physicsFileBuffer[mapID];
	}
	else
	{
		for(std::map<int, unsigned char*>::iterator iter = m_physicsFileBuffer.begin(); iter != m_physicsFileBuffer.end(); iter++)
		{
			delete[] iter->second;
			iter->second = NULL;
		}

		m_physicsFileBuffer.clear();

		char buff[32];
		
		memset(buff, 0, 32);
		sprintf(buff, "%s%diPad.map","spriteBin/", mapID);

		const char * fullpath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(buff);

		CCFileData data(fullpath, "rb");
		unsigned char* pData = new unsigned char[data.getSize()];
		memcpy(pData, data.getBuffer(), data.getSize());
		

		this->m_physicsFileBuffer.insert(make_pair(mapID, pData));

		return pData;
		
	}
	return NULL;
}

MapDataManager * MapDataManager::sharedManager()
{
	if(s_pSharedManager == NULL)
	{
		s_pSharedManager = new MapDataManager();
	}

	return s_pSharedManager;
}

void MapDataManager::Destroy()
{
	delete s_pSharedManager;
	s_pSharedManager = NULL;
}







