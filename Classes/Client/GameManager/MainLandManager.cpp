#include "OnlineNetworkManager.h"
#include "GameResourceManager.h"
#include "tinystr.h"
#include "LevelManager.h"
#include "MainLandManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "CityListLayer.h"
#include "InstanceManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NcSDKManager.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "TmLoginManager.h"
#endif
#ifndef NULL
#define NULL 0
#endif

MainLandManager::MainLandManager()
{   
	mapCityIdAndBasicInfo = new std::map<unsigned int,CityBasicInfo*>();
	mIsDataLoad = false;	
	m_pAllLevelInfo = new CATLObjectInfo();
}

MainLandManager::~MainLandManager()
{
	for (std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
		 iter != mapCityIdAndBasicInfo->end();iter++)
	{
		CityBasicInfo * basic_info = (*iter).second;
		CC_SAFE_DELETE(basic_info);
	}
	mapCityIdAndBasicInfo->clear();
	CC_SAFE_DELETE(mapCityIdAndBasicInfo);

	CC_SAFE_DELETE(m_pAllLevelInfo);
}

bool MainLandManager::LoadData()
{
	// Note: 避免数据重复加载
	if (false == mIsDataLoad)
	{
		mIsDataLoad = true;
	}
	else
	{
		return true;
	}

	std::string xmlFilePath = "Data/CityBasicInformation.xml";
	std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(xmlFilePath.c_str());

	unsigned long	_size;
	char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "r", &_size);
	TiXmlDocument	*_document = new TiXmlDocument();
	_document->Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);

	CC_SAFE_DELETE_ARRAY(_pFileContent);

	TiXmlElement *RootElement = _document->RootElement();
	if (RootElement)
	{
		TiXmlElement *childElement = RootElement->FirstChildElement();
		while(childElement)
		{
			CityBasicInfo *cityBasicInfo = new CityBasicInfo();

			unsigned int city_id = 0;
			TiXmlAttribute* pAttribute = childElement->FirstAttribute();
			while(pAttribute)
			{
				std::string strName(pAttribute->Name());
				if (strName == "CityID")
				{
					const char* city_id_str = childElement->Attribute("CityID");
					city_id = atoi(city_id_str);
				}
				else if (strName == "MapID")
				{
					std::string mapIdStr(childElement->Attribute("MapID"));
					cityBasicInfo->map_id = atoi(mapIdStr.c_str());
				}
				else if (strName == "UnlockTask")
				{
					std::string taskIdStr(childElement->Attribute("UnlockTask"));
					cityBasicInfo->unlockTaskId = atoi(taskIdStr.c_str());
				}
				else if (strName == "CityName")
				{
					cityBasicInfo->city_name = childElement->Attribute("CityName");
				}
				else if (strName == "HeroBornCoordinate")
				{
					cityBasicInfo->heroBornPoint = getPointAttribute(childElement, "HeroBornCoordinate");
				}
				else if (strName == "NormalRaidTransmissionGate")
				{
					cityBasicInfo->normalRaidTransmissionGate = getPointAttribute(childElement, "NormalRaidTransmissionGate");
				}
				else if (strName == "EliteRaidTransmissionGate")
				{
					cityBasicInfo->eliteRaidTransmissionGate = getPointAttribute(childElement, "EliteRaidTransmissionGate");
				}
				else if (strName == "BGMID")
				{
					const char* city_bgm = childElement->Attribute("BGMID");
					cityBasicInfo->backgroundMusicID = atoi(city_bgm);
				}
				else if (strName == "type")
				{
					const char* typeStr = childElement->Attribute("type");
					cityBasicInfo->type = atoi(typeStr);
				}
				else
				{
					int pos = strName.find("TaskNPC");
					if (std::string::npos != pos)
					{
						pos = pos + 6;
						int endPos = strName.rfind("ID");
						int firstNpcId = atoi(strName.substr(pos+1,endPos-pos-1).c_str());

						char buffer[100];
						memset(buffer,0,sizeof(buffer)/sizeof(char));
						sprintf(buffer,"TaskNPC%dID",firstNpcId);
						int npc_id = atoi(childElement->Attribute(buffer));

						NpcBasicInfo npcBasicInfo;

						memset(buffer,0,sizeof(buffer)/sizeof(char));
						sprintf(buffer,"TaskNPC%dposition",firstNpcId);

						std::string npc_pos_str(childElement->Attribute(buffer));
						int midPos = npc_pos_str.find("/");
						float posX = atoi(npc_pos_str.substr(0,midPos).c_str()) * 16;
						float posY = atoi(npc_pos_str.substr(midPos+1,npc_pos_str.length()-midPos-1).c_str()) * 16;

						npcBasicInfo.pos.x = posX;
						npcBasicInfo.pos.y = posY;				

						memset(buffer,0,sizeof(buffer)/sizeof(char));
						sprintf(buffer,"TaskNPC%dflip",firstNpcId);
						const char* bFlip_str = childElement->Attribute(buffer);
						if (bFlip_str != 0)
						{
							int value = atoi(bFlip_str);
							if (value == 1)
							{
								npcBasicInfo.bFlip = true;
							}
						}

						cityBasicInfo->mapNpcIdAndPos->insert(std::make_pair(npc_id,npcBasicInfo));
					}
				}

				pAttribute = pAttribute->Next();
			}

			

			if (cityBasicInfo->unlockTaskId)
			{
				//添加到映射表
				taskCityMap[cityBasicInfo->unlockTaskId] = cityBasicInfo->map_id;
			}
			else
			{
				defaultOpenCityList.insert(cityBasicInfo->map_id);
			}

			if (cityBasicInfo->type == 1)
			{
				InstanceManager::Get()->setSpecialEnteranceMapId(ENTERANCE_UNDERGROUND,cityBasicInfo->map_id);
			}
			else if (cityBasicInfo->type == 2)
			{
				InstanceManager::Get()->setSpecialEnteranceMapId(ENTERANCE_CITYDEFEND,cityBasicInfo->map_id);
			}
			else if (cityBasicInfo->type == 3)
			{
				InstanceManager::Get()->setSpecialEnteranceMapId(ENTERANCE_WORLDBOSS,cityBasicInfo->map_id);
			}
			else if (cityBasicInfo->type == 4)
			{
				InstanceManager::Get()->setSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER,cityBasicInfo->map_id);
			}
            else if (cityBasicInfo->type == 5)
            {
                InstanceManager::Get()->setSpecialEnteranceMapId(ENTERANCE_GUILDSCENE,cityBasicInfo->map_id);
            }
		

			mapCityIdAndBasicInfo->insert(std::make_pair(city_id,cityBasicInfo));

			childElement = childElement->NextSiblingElement();
		}
	}

	delete _document;

	const char* pFileName = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(
		(std::string("GameBin/allLevel.bin")).c_str());

	m_pAllLevelInfo->Load(pFileName);

	return true;
}

std::map<unsigned int,NpcBasicInfo>* MainLandManager::GetOneCityNpcInfos(unsigned int city_id)
{
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->find(city_id);
	if (iter == mapCityIdAndBasicInfo->end())
	{
		return 0;
	}
	return (*iter).second->mapNpcIdAndPos;
}

CCPoint MainLandManager::getCityCell(int mapId, string typeName)
{
	CCPoint cell = ccp(0, 0);
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
	while (iter != mapCityIdAndBasicInfo->end())
	{
		if ((*iter).second->map_id == mapId)
		{
			if (typeName == "born")
			{
				cell = (*iter).second->heroBornPoint;
			}
			else if (typeName == "normalGate")
			{
				cell = (*iter).second->normalRaidTransmissionGate;
			}
			else if (typeName == "eliteGate")
			{
				cell = (*iter).second->eliteRaidTransmissionGate;
			}
			
			break;
		}
		iter++;
	}	
	
	return cell;
}

string MainLandManager::getCityName(int mapId)
{
	string name;
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
	while (iter != mapCityIdAndBasicInfo->end())
	{
		if ((*iter).second->map_id == mapId)
		{
			name = (*iter).second->city_name;
			break;
		}
		iter++;
	}
	return name;
}

bool MainLandManager::isMainLand(int mapId)
{
	bool isMainLand = false;
	if (!mapId)
	{
		isMainLand = true;
	}
	else
	{
		std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
		while (iter != mapCityIdAndBasicInfo->end())
		{
			if ((*iter).second->map_id == mapId)
			{
				isMainLand = true;
				break;
			}
			iter++;
		}
	}
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(mapId == 0)
    {
        if(NcSDKManager::isFloatButtonHiden() == false)
        {
            NcSDKManager::SetFloatButtonHidden(true);
        }
    }
    else
    {
        if(isMainLand == true)
        {
            if(NcSDKManager::isFloatButtonHiden() == true)
            {
                NcSDKManager::SetFloatButtonHidden(false);
            }
        }
        else
        {
            if(NcSDKManager::isFloatButtonHiden() == false)
            {
                NcSDKManager::SetFloatButtonHidden(true);
            }
        }
    }
    
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   
    {
    	PlantformId	channel = TmLoginManager::tmLoginManager()->getPlantformId();
        if(isMainLand == true)
        {
        	if(channel == YingYongHui) 
		    {
			    TmLoginManager::tmLoginManager()->setFloatButtonHidden(false);
		    }
           
        }
        else
        {
        	if(channel == YingYongHui) 
		    {
			    TmLoginManager::tmLoginManager()->setFloatButtonHidden(true);
		    }
        }
    }
    
#endif

	return isMainLand;
}
bool MainLandManager::isMainLandType(int mapId)
{
	bool isMainLand = false;
	if (!mapId)
	{
		isMainLand = true;
	}
	else
	{
		std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
		while (iter != mapCityIdAndBasicInfo->end())
		{
			if ((*iter).second->map_id == mapId)
			{
				if((*iter).second->type == 0)
					isMainLand = true;
				break;
			}
			iter++;
		}
	}

	return isMainLand;
}
bool MainLandManager::isCityUnlock(int mapId)
{
	bool ret = false;
	if (unlockCityList.find(mapId) != unlockCityList.end())
	{
		ret = true;
	}

	if (defaultOpenCityList.find(mapId) != defaultOpenCityList.end())
	{
		ret = true;
	}

	return ret;
}

void MainLandManager::onEventTaskUnlock(int taskId)
{
	if (taskCityMap.find(taskId) != taskCityMap.end())
	{
		if (unlockCityList.find(taskCityMap[taskId]) == unlockCityList.end())
		{
			unlockCityList.insert(taskCityMap[taskId]);
		}
	}	
}

void MainLandManager::sendGetTaskStatus()
{
	map<int, int>::iterator iter = taskCityMap.begin();
	for (; iter != taskCityMap.end(); iter++)
	{
		OnlineNetworkManager::sShareInstance()->sendGetTaskStatusMessage(iter->first);
	}
}

vector<int> MainLandManager::getCityIdList()
{
	vector<int> cityIdList;
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
	while (iter != mapCityIdAndBasicInfo->end())
	{
		if ((*iter).second->type == 0)
		{
			cityIdList.push_back((*iter).second->map_id);
		}		
		iter++;
	}
	return cityIdList;
}
	
CCPoint MainLandManager::getPointAttribute(TiXmlElement *element, string attStr)
{
	std::string pointStr(element->Attribute(attStr.c_str()));
	int midPos = pointStr.find("/");
	int cellX = atoi(pointStr.substr(0,midPos).c_str());
	int cellY = atoi(pointStr.substr(midPos+1,pointStr.length()-midPos-1).c_str());
	return ccp(cellX, cellY);
}

int MainLandManager::GetCityIdByMapId(int mapId)
{
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
	while(iter != mapCityIdAndBasicInfo->end())
	{
		if (mapId == (*iter).second->map_id)
		{
			return (*iter).first;
		}
		iter++;
	}
	return -1;
}

bool MainLandManager::GetCityIdByNpcId(int npc_id,int &city_Id,int &mapId)
{
	// Note: 首先在自己的城中查找NPC
	int CityIDValue = GetCityIdByMapId(this->curCityId);
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->find(CityIDValue);
	if (iter != mapCityIdAndBasicInfo->end())
	{
		CityBasicInfo* pCityInfo = (*iter).second;
		if (pCityInfo)
		{
			std::map<unsigned int,NpcBasicInfo>::iterator npcIter = pCityInfo->mapNpcIdAndPos->find(npc_id);
			if (npcIter != pCityInfo->mapNpcIdAndPos->end())
			{
				mapId = (*iter).second->map_id;
				city_Id = (*iter).first;
				return true;
			}
		}
	}

	//if (bFindNpc == false)
	{
		std::map<unsigned int,CityBasicInfo*>::reverse_iterator reversIter = mapCityIdAndBasicInfo->rbegin();
		while (reversIter != mapCityIdAndBasicInfo->rend())
		{
			int tmpCityId = (*reversIter).first;
			if (isCityUnlock((*reversIter).second->map_id))
			{
				CityBasicInfo* pCityInfo = (*reversIter).second;
				if (pCityInfo)
				{
					std::map<unsigned int,NpcBasicInfo>::iterator npcIter = pCityInfo->mapNpcIdAndPos->find(npc_id);
					if (npcIter != pCityInfo->mapNpcIdAndPos->end())
					{
						mapId = (*reversIter).second->map_id;
						city_Id = (*reversIter).first;
						return true;
					}
				}
			}	

			reversIter++;
		}
	}

	return false;
}

bool MainLandManager::GetOneCityNpcPos(int cityId,int npcId, CCPoint &targetPos)
{
	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->find(cityId);
	if (iter != mapCityIdAndBasicInfo->end())
	{
		CityBasicInfo* pCityInfo = (*iter).second;
		if (pCityInfo)
		{
			std::map<unsigned int,NpcBasicInfo>::iterator npcIter = pCityInfo->mapNpcIdAndPos->find(npcId);
			if (npcIter != pCityInfo->mapNpcIdAndPos->end())
			{
				targetPos = (*npcIter).second.pos;
				return true;
			}
		}
	}

	return false;
}

int MainLandManager::GetCityBGM(int cityId)
{
	int musicId = 0;

	std::map<unsigned int,CityBasicInfo*>::iterator iter = mapCityIdAndBasicInfo->begin();
	while (iter != mapCityIdAndBasicInfo->end())
	{
		if ((*iter).second->map_id == cityId)
		{
			musicId = (*iter).second->backgroundMusicID;		
			break;
		}
		iter++;
	}

	return musicId;
}

void MainLandManager::resetData()
{
	unlockCityList.clear();
}