#pragma once

#include "cocos2d.h"
#include "vector"
#include "map"
#include "string"

using namespace cocos2d;

class StoryFrameData;

// Note: 剧情中角色数据
class StoryRoleBasicData
{
public:
	StoryRoleBasicData()
	{
		mRoleId = 0;
		mIsVisible = false;
		mRoleLogoIcon = "";
	}
	~StoryRoleBasicData()
	{
		mMapStoryFramesData.clear();
	}
public:
	CC_SYNTHESIZE(unsigned int, mRoleId, RoleId);
	CC_SYNTHESIZE(bool, mIsVisible, IsVisible);
	CC_SYNTHESIZE(std::string, mRoleLogoIcon, RoleLogoIcon);

	std::map<unsigned int,StoryFrameData> mMapStoryFramesData;
};

// Note: 剧情中帧事件数据
class StoryFrameData
{
public:
	StoryFrameData()
	{
		mRolePos = ccp(-1,-1);
		mActorId = -1;
		mFrameIndex = 0;
		mFlip = -1;
		mVisible = -1;
		mIsOver = false;		
		mIsActionPauseOnOver = false;
	}
	~StoryFrameData(){}

	static bool IsPosChanged(CCPoint p)
	{
		if (p.x == -1 && p.y == -1)
		{
			return false;
		}
		return true;
	}
public:
	CC_SYNTHESIZE(unsigned int, mFrameIndex, FrameIndex);
	CC_SYNTHESIZE(CCPoint, mRolePos, RolePos);
	CC_SYNTHESIZE(int, mActorId, ActorId);
	CC_SYNTHESIZE(int, mFlip, Flip);
	CC_SYNTHESIZE(bool, mIsOver, IsOver);
	CC_SYNTHESIZE(bool, mIsActionPauseOnOver, IsPauseOnOver);
	CC_SYNTHESIZE(int, mVisible, Visible);
};

// Note: 剧情对话数据
class StoryDialogData
{
public:
	StoryDialogData()
	{
		mFrameIndex = 0;
		mRoleIndex = 0;
		mTextID = "";
		mIsOver = false;
	}
	~StoryDialogData(){}

	void ResetValue()
	{
		mFrameIndex = 0;
		mRoleIndex = 0;
		mTextID = "";
		mIsOver = false;
	}
public:
	CC_SYNTHESIZE(unsigned int, mFrameIndex, FrameIndex);
	CC_SYNTHESIZE(std::string, mTextID, TextID);
	CC_SYNTHESIZE(unsigned int, mRoleIndex, RoleIndex);
	CC_SYNTHESIZE(bool, mIsOver, IsOver);
};

class StroyData
{
public:
	StroyData()
	{
		mID = 0;
		mTaskId = 0;
		mMapId = 0;
		mEndFrame = 0;
		mCameraPos = CCPointZero;
		mHappendAtWhen = 0;
	}
	~StroyData()
	{
		mMapStoryRolesBasicData.clear();
		mMapStroyDialogsData.clear();
	}

	// Note: 数据操作函数
	bool InsertRolePosAtFrame(unsigned int roleIndex,unsigned int frameIndex,CCPoint pos);
	bool InsertRoleActorAtFrame(unsigned int roleIndex,unsigned int frameIndex,unsigned int actorId,bool bAnimPauseOnOver);
	bool InsertDialogTextIdAtFrame(unsigned int roleIndex,unsigned int frameIndex,std::string textId);
	bool InsertIsFlipAtFrame(unsigned int roleIndex,unsigned int frameIndex,int nFlip);
	bool InsertIsVisibleAtFrame(unsigned int roleIndex,unsigned int frameIndex,int nVisible);
	bool SetRoleIdAtIndex(unsigned int roleIndex,unsigned int roleId);
	bool SetRoleLeftIcon(unsigned int roleIndex,std::string iconName);
	bool SetDisplayPicAtIndex(unsigned int index,std::string picName);

	// Note: 数据Parts
	
public:
	CC_SYNTHESIZE(unsigned int, mID, ID);
	CC_SYNTHESIZE(unsigned int, mTaskId, TaskId);
	CC_SYNTHESIZE(unsigned int, mMapId, MapId);
	CC_SYNTHESIZE(CCPoint, mCameraPos, CameraPos);
	CC_SYNTHESIZE(unsigned int, mEndFrame, EndFrame);
	// Note: 剧情触发的时机 0 进入副本 1 退出副本
	CC_SYNTHESIZE(unsigned int, mHappendAtWhen, HappendAtWhen);

	std::map<unsigned int,StoryRoleBasicData> mMapStoryRolesBasicData;
	std::map<unsigned int,StoryDialogData> mMapStroyDialogsData;
	std::map<unsigned int,std::string> mMapStoryDisplayPicData;
};