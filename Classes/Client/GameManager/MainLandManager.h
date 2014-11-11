#pragma once

#include "Singleton_t.h"
#include "string"
#include "map"
#include "cocos2d.h"
#include "tinyxml.h"
#include "ATLObjectInfo.h"
#include "ATLObjectLayer.h"

USING_NS_CC;

struct NpcBasicInfo
{
	CCPoint pos;
	bool bFlip;

	NpcBasicInfo(){
		pos = CCPointZero;
		bFlip = false;
	}
};

struct CityBasicInfo
{
	std::string city_name;
	unsigned int map_id;
	CCPoint heroBornPoint;
	unsigned int backgroundMusicID;
	CCPoint normalRaidTransmissionGate;
	CCPoint eliteRaidTransmissionGate;
	int unlockTaskId;
	int type;

	std::map<unsigned int,NpcBasicInfo> *mapNpcIdAndPos;

	CityBasicInfo(){
		city_name = "";
		map_id = 0;
		backgroundMusicID = 0;
		heroBornPoint = normalRaidTransmissionGate = eliteRaidTransmissionGate = CCPointZero;

		mapNpcIdAndPos = new std::map<unsigned int,NpcBasicInfo>();
	}

	~CityBasicInfo(){
		mapNpcIdAndPos->clear();
		CC_SAFE_DELETE(mapNpcIdAndPos);
	}
};

/*
*   管理主城信息
*/
class MainLandManager : public TSingleton<MainLandManager>
{
public:
	MainLandManager();
	virtual ~MainLandManager();

	/**
	* Instruction : 从配置文件中读取主城数据
	* @param 
	*/	
	bool LoadData();
	/**
	* Instruction : 获取某主城中npc的信息
	* @param 
	*/	
	std::map<unsigned int,NpcBasicInfo>* GetOneCityNpcInfos(unsigned int city_id);

	//当前主城
	void setCurCityId(int id){
		this->curCityId = id;
	}
	int getCurCityId(){return this->curCityId;}

	//获取某主城的出生点
	//born : 出生点
	//normalGate: 普通传送门
	//eliteGate: 精英传送门
	CCPoint getCityCell(int mapId, std::string typeName);

	//主城名称
	std::string getCityName(int mapId);

	vector<int> getCityIdList();
	bool isCityUnlock(int mapId);

	//判断一个地图是不是主城
	bool isMainLand(int mapId);
	bool isMainLandType(int mapId);
	void onEventTaskUnlock(int taskId);

	void sendGetTaskStatus();

	/**
	* Instruction : 
	* @param 
	*/
	int GetCityIdByMapId(int map_id);

	bool GetCityIdByNpcId(int npc_id,int &cityId,int &mapId);
	bool GetOneCityNpcPos(int cityId,int npcId, CCPoint &targetPos);
	
	//得到主城的背景音乐
	int GetCityBGM(int cityId);

	//bool IsCityCanGo(int cityId);
	void resetData();

	CATLObjectInfo* getAllLevelInfo() { return m_pAllLevelInfo; }

private:
	CCPoint getPointAttribute(TiXmlElement *element, std::string attStr);

	std::map<unsigned int,CityBasicInfo*> *mapCityIdAndBasicInfo;
	map<int, int> taskCityMap;
	set<int> unlockCityList;
	set<int> defaultOpenCityList;

	int curCityId;
	bool mIsDataLoad;

	CATLObjectInfo* m_pAllLevelInfo;
};