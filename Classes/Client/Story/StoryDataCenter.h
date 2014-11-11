#pragma once

/// <summary>
//	剧情系统数据管理模块
/// </summary>

#include "StoryData.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "map"
#include "vector"

#define INSTANCE_ID_HEADER (200000)

// Note: 剧情的类型分类
enum EStoryType
{
	kType_MainLand_PreTask = 0,
	kType_MainLand_AcceptTask = 1,
	kType_MainLand_RewardTask = 2,
	kType_MainLand_OverTask = 3,
	kType_FB_BeginMap = 4,
	kType_FB_EndMap = 5,
	kType_Story_UnKnow
};

class StoryDataCenter
{
public:    	
	StoryDataCenter();
	virtual ~StoryDataCenter(); 

	static StoryDataCenter* Get();
	static void Destroy();

	/**
	* Instruction : 加载副本相关剧情数据
	* @param 
	*/	
	bool LoadOneDataItem(unsigned int id,unsigned int map_id,unsigned int nWhen,int &_ID);
	/**
	* Instruction : 加载主城相关剧情数据
	* @param 
	*/	
	bool LoadOneDataItem(unsigned int id,unsigned int when,int &_ID);

	bool LoadDataFromXML();

	// Note: 依赖于id
	bool IsOneItemHasStory(unsigned int Id,unsigned int map_id,unsigned int when,unsigned int &type);
	bool IsOneMainLandStoryPlayed(unsigned int mapId);
	unsigned int GetTotalFrames(unsigned int Id);
	bool IsOneFrameHasDialog(unsigned int taskId,unsigned int frameIndex,StoryDialogData & dialogData);
	bool GetOneItemOwnRoleIds(unsigned int taskId,std::vector<unsigned int> &vecOut);
	bool GetOneItemOneFrameRoleInfo(unsigned int taskId,unsigned int frameIndex,
		std::vector<unsigned int> &vecPlayerId,std::vector<StoryFrameData>& vecStoryData);
	bool IsOneRoleVisible(unsigned int taskId,unsigned int roleId);
	bool SetOneRoleVisibleOrNot(unsigned int taskId,unsigned int roleId,bool bVisible);
	bool GetRoleNameByRoleId(unsigned int roleId, std::string& roleName);

	// Note: 依赖iter
	unsigned int GetTotalFrames();
	bool IsOneFrameHasDialog(unsigned int frameIndex,StoryDialogData & dialogData);
	bool IsOneFrameHasDisplayPic(unsigned int frameIndex,std::string & picName);
	bool GetOneItemOwnRoleIds(std::vector<unsigned int> &vecOut);
	bool GetOneItemAllRoleIds(std::vector<unsigned int> &vecOut);
	bool GetOneItemOneFrameRoleInfo(unsigned int frameIndex,
		std::vector<unsigned int> &vecPlayerId,std::vector<StoryFrameData>& vecStoryData);
	bool IsOneRoleVisible(unsigned int roleId);
	bool SetOneRoleVisibleOrNot(unsigned int roleId,bool bVisible);
	bool GetNextEndPointFrameAndPos(unsigned int roleIndex,unsigned int curFrameIndex,unsigned int& nextFrameIndex,CCPoint &nextPoint);
	bool GetPreAndNextPointFrameData(unsigned int roleIndex,unsigned int curFrameIndex,unsigned int &preFrameIndex,unsigned int &nextFrameIndex,CCPoint &prePoint,CCPoint &nextPoint);
	bool GetOneRoleLeftIcon(unsigned int roleIndex,std::string &iconName);
	CCPoint GetCameraPoint();
	std::map<unsigned int,StoryRoleBasicData>& GetStoryRoleBasicData();

	void SendReqCheckPlayedStory();
protected:
	TiXmlElement* initXML(const char * filePath);
	bool ReadOneXmlItem(TiXmlElement* pElement);
	bool SplitPosFromContent(std::string content,float &posx,float &posy);
	bool SplitFrameAndPosFromContent(std::string content,unsigned int &frame,float &posx,float &posy);
	bool SplitRoleID(std::string content,unsigned int &roleId);
	bool SplitFrameAndActorIdFromContent(std::string content,unsigned int &frame,int &actorId,bool &bPauseOnOver);
	bool SplitFrameAndTextIdFromContent(std::string content,std::vector<unsigned int> &vecFrameIndex,std::vector<std::string> &vecTextId);
	bool SplitContent(std::string content,std::vector<std::string> &vecOutContent);
private:
	bool mIsLoadAllData;
	std::vector<StroyData> mVecTaskIdAndStoryData;
	std::vector<StroyData>::iterator mCurIterator;

	std::vector<StroyData> mMainLandVecTaskStoryData;
};