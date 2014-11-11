#include "OnlineNetworkManager.h"
#include "InstanceManager.h"
#include "GameConfigFromLuaManager.h"
#include "CCLuaEngine.h"
#include "LuaTinkerManager.h"
#include "MainMenuLayer.h"
#include "MainLandManager.h"
#include "LevelManager.h"
#include "Localization.h"
#include "SpriteSeer.h"

#ifndef NULL
#define NULL 0
#endif

#include <algorithm>
using namespace std;

InstanceManager::InstanceManager()
	:portalPosition(ccp(0,0)),
	 portalId(0),
	 curInstanceId(0),
	 instanceTime(0),
	 preInstanceId(0),
	 preMapId(0),
	 playerCount(0),
	 m_max_normal_instance_id(0)
{   
	specialEnteranceMap[ENTERANCE_CITYDEFEND] = 0;
	specialEnteranceMap[ENTERANCE_UNDERGROUND] = 0;
	specialEnteranceMap[ENTERANCE_WORLDBOSS] = 0;
    specialEnteranceMap[ENTERANCE_GUILDSCENE] = 0;
	outCityMap[ENTERANCE_CITYDEFEND] = 0;
	outCityMap[ENTERANCE_UNDERGROUND] = 0;
	outCityMap[ENTERANCE_WORLDBOSS] = 0;
    outCityMap[ENTERANCE_GUILDSCENE] = 0;
}

InstanceManager::~InstanceManager()
{
	
}

void InstanceManager::UpdateData()
{
	// clear data
	m_vecPPveInstance.clear();

	// load instance script file
	//GameConfigFromLuaManager::Get()->checkConfigLoadFile("InstanceConfigTable");
	LuaTinkerManager::Get()->checkAnyLoadFile("Script/ppveUILayer.lua");

	// call InitAllInstance in InstanceConfigTable.lua
	CCLuaEngine::defaultEngine()->executeGlobalFunction("InitAllInstance");
}

void InstanceManager::UpdateDataForGuildInstance()
{
	// clear data
	m_vecGuildInstance.clear();

	// load instance script file
	LuaTinkerManager::Get()->checkAnyLoadFile("Script/guildInstanceUILayer.lua");

	// call InitGuildInstance in InstanceConfigTable.lua
	CCLuaEngine::defaultEngine()->executeGlobalFunction("InitGuildInstance");
}

void  InstanceManager::InsertPPVEInstanceId(int id, int reqLevel)
{
     MapInfoVector::iterator it = m_vecPPveInstance.begin();
	 for(; it != m_vecPPveInstance.end(); it++)
	 {
		 InstanceMapInfo& data = *it;
		 if (data.id == id)
		 {
			 return;
		 }
	 }

	 InstanceMapInfo data;
	 
	 data.id = id;
	 data.MinLevel = reqLevel;

	 m_vecPPveInstance.push_back(data);
}

void  InstanceManager::InsertGuildInstanceId(int id, int reqLevel)
{
	MapInfoVector::iterator it = m_vecGuildInstance.begin();
	for(; it != m_vecGuildInstance.end(); it++)
	{
		InstanceMapInfo& data = *it;
		if (data.id == id)
		{
			return;
		}
	}

	InstanceMapInfo data;

	data.id = id;
	data.MinLevel = reqLevel;

	m_vecGuildInstance.push_back(data);
}

MapInfoVector& InstanceManager::getPPVEInstances()
{
	return m_vecPPveInstance;
}

MapInfoVector& InstanceManager::getGuildInstances()
{
	return m_vecGuildInstance;
}

void InstanceManager::resetData()
{
	

	portalPosition = playerPosition = ccp(0, 0);
	portalId = 0;
	preInstanceId = 0 ;
	preMapId = 0 ;
	curInstanceId = 0;

	instanceTime = 0;

	playerCount = 0;

	m_vecPPveInstance.clear();
	curInstanceList.clear();
	totalInstanceList.clear();
	monsterBornPoints.clear();

	outCityMap[ENTERANCE_CITYDEFEND] = 0;
	outCityMap[ENTERANCE_UNDERGROUND] = 0;
	outCityMap[ENTERANCE_WORLDBOSS] = 0;
    outCityMap[ENTERANCE_GUILDSCENE] = 0;
}

void InstanceManager::addToTotalInstanceList(InstanceInfo info)
{
	totalInstanceList[info.id] = info;
	if (info.id < JING_YING_FU_BEN_MIN 
		&& m_max_normal_instance_id < info.id)
	{
		m_max_normal_instance_id = info.id;
	}
}
InstanceInfo * InstanceManager::getOneInstance(unsigned int instanceId)
{
	
	if (totalInstanceList.find(instanceId) != totalInstanceList.end())
	{
		return &totalInstanceList[instanceId];
	}
	return (InstanceInfo *)NULL;
}
int	InstanceManager::getOneInstanceStars(unsigned int instanceId)
{
	int ret = -1;
	if (totalInstanceList.find(instanceId) != totalInstanceList.end())
	{
	    if(instanceId < JING_YING_FU_BEN_MIN)
		    ret = totalInstanceList[instanceId].star;
        else if(instanceId <= JING_YING_FU_BEN_MAX &&
            totalInstanceList[instanceId].can_enter_flag)
            ret = totalInstanceList[instanceId].star;
            
	}
	return ret;
}

//used by auto fight to get all chapter name for jing ying fu ben
std::map<int, ChapterInfo>*	InstanceManager::getAllchapterName(INSTANCE_TYPE type)
{
    chapterNameMap.clear();
    std::map<int , InstanceInfo>::iterator instanceItor;
    for(instanceItor = totalInstanceList.begin(); instanceItor != totalInstanceList.end(); instanceItor++)
    {
        int instanceId = instanceItor->first;
        
        if(type == INSTANCE_TYPE_JINGYING && (instanceId < JING_YING_FU_BEN_MIN || instanceId > JING_YING_FU_BEN_MAX))
		{
			continue;
		}
		else if (type == INSTANCE_TYPE_DUNGEON && (instanceId < UNDERGROUND_INSTANCE_MIN || instanceId > UNDERGROUND_INSTANCE_MAX))
		{
			continue;
		} 
        
        int chapterId = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", instanceId, "Chapter");
        if(chapterNameMap.find(chapterId) == chapterNameMap.end())
        {
            char strid[10];

            snprintf(strid, 10, "%d", chapterId);  
            string msgKey=string("M_AUTOFIGHT_ELITE_SHEET")+string(strid);
 
            const char* msg = Localizatioin::getLocalization( msgKey.c_str());
            chapterNameMap[chapterId].name = string(msg);
            chapterNameMap[chapterId].active = false;             
        }
       
        //star must > 1
        if(instanceItor->second.star > 1 &&
           instanceItor->second.can_enter_flag)
        {
            chapterNameMap[chapterId].active = true;             
        }
    }
	return &chapterNameMap;
}

bool InstanceManager::isInstanceListChanged(const std::vector<InstanceInfo>& instanceInfos)
{
	bool ret = false;
	if (curInstanceList.size() != instanceInfos.size())
	{
		ret = true;
		return ret;
	}

	for (size_t i = 0; i < curInstanceList.size(); i++)
	{
		if (curInstanceList[i].id != instanceInfos[i].id
			|| curInstanceList[i].star != instanceInfos[i].star
			|| curInstanceList[i].can_enter_flag != instanceInfos[i].can_enter_flag
			|| curInstanceList[i].refresh_times != instanceInfos[i].refresh_times
			|| curInstanceList[i].battle_times != instanceInfos[i].battle_times)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

//进入特殊入口，地下城，主城防守，世界BOSS,公会场景
void InstanceManager::onEnterSpecialEntrance(ENTERANCE_TYPE type)
{
	CCPoint bornCell = MainLandManager::Get()->getCityCell(specialEnteranceMap[type], "born");
	CCPoint bornPoint = LevelManager::sShareInstance()->pointCellToPixel(bornCell);
	OnlineNetworkManager::sShareInstance()->sendPlayerEnterMapMessage(specialEnteranceMap[type], bornPoint.x, bornPoint.y, 0);
	LuaTinkerManager::Get()->callLuaFunc<void>("", "PushOneWaitingLayer", "CSEnterMapReq");
	outCityMap[type] = MainLandManager::Get()->getCurCityId();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "CancelBestFighter");
}

//离开特殊入口，地下城，主城防守，世界BOSS
void InstanceManager::onLeaveSpecialEntrance(ENTERANCE_TYPE type)
{
	CCPoint bornCell = MainLandManager::Get()->getCityCell(outCityMap[type], "born");
	CCPoint bornPoint = LevelManager::sShareInstance()->pointCellToPixel(bornCell);
	OnlineNetworkManager::sShareInstance()->sendPlayerEnterMapMessage(outCityMap[type], bornPoint.x, bornPoint.y, 0);
	LuaTinkerManager::Get()->callLuaFunc<void>("", "PushOneWaitingLayer", "CSEnterMapReq");
}

void InstanceManager::autoSearchPathToInstance(int instanceId)
{
	int star = getOneInstanceStars(instanceId);
	if(star != -1)
	{
		// 副本存在
		InstanceListLayer *pLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
		if(pLayer)
		{
			pLayer->setInstanceId(instanceId);
			SpriteSeer* hero = GameManager::Get()->getHero();
            if(instanceId < JING_YING_FU_BEN_MIN)
                hero->MoveToSpriteTranspoint(true);
            else if(instanceId < JING_YING_FU_BEN_MAX)
			    hero->MoveToSpriteTranspoint(false);
			
		}
	}
	else
	{
		const char* msg = Localizatioin::getLocalization("M_FINDMATERIAL_ERROR2");
		ShowMessage(msg);
	}
}

InstanceInfo InstanceManager::getInstanceInfo(int instanceId)
{
    InstanceInfo info;
    info.id = 0;
    for (size_t i = 0; i < curInstanceList.size(); ++i)
    {
        if (curInstanceList[i].id == instanceId)
        {
            return curInstanceList[i];
        }
    }

    return info;
}

void InstanceManager::RefreshInstanceTimes(int id)
{
    for (size_t i = 0; i < curInstanceList.size(); ++i)
    {
        if (curInstanceList[i].id == id)
        {
            curInstanceList[i].battle_times = 0;
        }
    }
}