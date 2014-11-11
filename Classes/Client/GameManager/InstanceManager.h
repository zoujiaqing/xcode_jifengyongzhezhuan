// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//

#pragma once

#include "SpriteTransPoint.h"
#include "Singleton_t.h"
#include "InstanceListLayer.h"
#include "BYGraySprite.h"
/*
*   管理所有的副本的信息，存储当前所在的副本的信息，包括多人副本及单人副本
*   and 精英副本
*
*/

/// ppve instance data struct
typedef struct stMapInfo
{
	unsigned int id;
	bool active;
	int  MinLevel;
}InstanceMapInfo;

typedef vector<InstanceMapInfo>MapInfoVector;

typedef struct _ChapterInfo
{
	string name;
	bool active;
}ChapterInfo;

typedef enum
{
	ENTERANCE_UNDERGROUND,
	ENTERANCE_CITYDEFEND,
	ENTERANCE_WORLDBOSS,
	ENTERANCE_BESTFIGHTER,
    ENTERANCE_GUILDSCENE,
}ENTERANCE_TYPE;

class InstanceManager : public TSingleton<InstanceManager>
{
public:
	InstanceManager();
	virtual ~InstanceManager();

	void setMonsterBornPoints(vector<CCPoint> points){this->monsterBornPoints = points;}
	vector<CCPoint> getMonsterBornPoints(){return this->monsterBornPoints;}
	void clearMonsterBornPoints(){this->monsterBornPoints.clear();}

	void setPortalPosition(CCPoint pos){this->portalPosition = pos;}
	CCPoint getPortalPosition(){return this->portalPosition;}

	void setPortalId(unsigned int id){this->portalId = id;}
	unsigned int getPortalId(){return this->portalId;}

	void setCurInstanceId(int id){this->curInstanceId = id;}
	int getCurInstanceId(){return this->curInstanceId;}

	//服务器获得的副本玩家位置
	void setPlayerPosition(CCPoint pos){this->playerPosition = pos;}
	CCPoint getPlayerPosition(){return this->playerPosition;}

	//副本所需通关时间
	void setInstanceTime(int instanceTime){this->instanceTime = instanceTime;}
	int getInstanceTime(){return this->instanceTime;}

	//// ppve instance 
	void  InsertPPVEInstanceId(int id , int reqLevel);

	//// guild instance 
	void  InsertGuildInstanceId(int id , int reqLevel);

	MapInfoVector& getPPVEInstances();

	MapInfoVector& getGuildInstances();

	void addToTotalInstanceList(InstanceInfo info);

	void UpdateData();
	void UpdateDataForGuildInstance();

	void resetData();

	// Note: 获取某副本的星星数
	int	getOneInstanceStars(unsigned int instanceId);
	//get instance info for one instance
	 InstanceInfo * getOneInstance(unsigned int instanceId);
	//return all chapter id and name
	std::map<int, ChapterInfo>	*getAllchapterName(INSTANCE_TYPE type);

	//特殊入口
	void setSpecialEnteranceMapId(ENTERANCE_TYPE type, int id){specialEnteranceMap[type] = id;}
	int getSpecialEnteranceMapId(ENTERANCE_TYPE type){return specialEnteranceMap[type];}

	void onEnterSpecialEntrance(ENTERANCE_TYPE type);
	void onLeaveSpecialEntrance(ENTERANCE_TYPE type);

	//pve local server cache
	void setPreInstanceId(int id){this->preInstanceId = id;}
	int getPreInstanceId(){return preInstanceId;}

	void setPreMapId(int id){this->preMapId = id;}
	int getPreMapId(){return preMapId;}
	void autoSearchPathToInstance(int instanceId);

	//判断多人副本玩家数量
	void setPlayerCount(int count){this->playerCount = count;}
	int getPlayerCount() const {return this->playerCount;}
    
    InstanceInfo getInstanceInfo(int instanceId);
    void RefreshInstanceTimes(int id);
	unsigned int getMaxNormalInstanceId(){return m_max_normal_instance_id;}
public:

	bool isInstanceListChanged(const std::vector<InstanceInfo>& instanceInfos);
	std::vector<InstanceInfo> curInstanceList;

private:

	std::map<int , InstanceInfo> totalInstanceList;
	std::map<int, ChapterInfo> chapterNameMap;
	CCPoint portalPosition;
	unsigned int portalId;
	unsigned int m_max_normal_instance_id;

	int curInstanceId;
	CCPoint playerPosition;

	int instanceTime;

	//// add ppve instance
	MapInfoVector m_vecPPveInstance;	

	//// add guild instance
	MapInfoVector m_vecGuildInstance;	

	vector<CCPoint> monsterBornPoints;

	//特殊入口
	map<ENTERANCE_TYPE, int> specialEnteranceMap;
	map<ENTERANCE_TYPE, int> outCityMap;

	//pve local server cache
	int preInstanceId;
	int preMapId;

	int playerCount;
};
