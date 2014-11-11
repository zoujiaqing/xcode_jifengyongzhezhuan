#pragma once

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

#include "cocos2d.h"
#include "UserData.h"

class SpriteElfConfigFromLuaManager
{
public :	
	virtual ~SpriteElfConfigFromLuaManager();
	static SpriteElfConfigFromLuaManager* getInstance(void);
	static void Destroy();

	virtual void initLuaState();

	// Note: Check and Get Functions

	/**
	* Instruction : 获取精灵相对英雄的固定跟随偏移点
	* @param 
	*/	
	bool GetElfToHeroFixedDistance(float &posx,float &posy);

	/**
	* Instruction : 获取外判断区域
	* @param 
	*/	
	float GetOuterCircleRadius();

	/**
	* Instruction : 获取外判断区域
	* @param 
	*/	
	float GetInnerCircleRadius();

	/**
	* Instruction : 获取加速时间
	* @param 
	*/	
	float GetTotalAccTimer();
	/**
	* Instruction : 获取开始速度系数
	* @param 
	*/	
	float GetStartSpeedCoefficient();
	/**
	* Instruction : 获取最大靠近速度系数
	* @param 
	*/	
	float GetMaxMoveSpeedCoefficient();
	/**
	* Instruction : 
	* @param 
	*/
	//void PushElfIdToLuaTable();
	/**
	* Instruction : 将网络回调暴漏给Lua
	* @param 
	*/
	bool PushElfStateMessageToLua(int elfId,int stateId);
	/**
	* Instruction : 精灵列表信息暴漏给Lua
	* @param 
	*/
	bool PushElfListMessageToLua(PLAYER_ID roleId,int elfId,int stateId, int grade);
	/**
	* Instruction : 
	* @param 
	*/
	void GetElfListMessageOver();
	/**
	* Instruction : 
	* @param 
	*/
	bool CallLuaStartFunction();
	/**
	* Instruction : 获取主角战斗精灵ID
	* @param 
	*/
	unsigned int GetHeroFightElfID();
	/**
	* Instruction : 绑定精灵潜能面板到背包Layer
	* @param 
	*/
	bool EquipElfPotientionPanelToBackPackLayer(cocos2d::CCLayer* pLayer);
	/**
	* Instruction : 关闭精灵潜能面板
	* @param 
	*/
	bool CloseElfPotientionPanel();
	/**
	* Instruction : 
	* @param 
	*/
	void ResetValue();
	/**
	* Instruction : 
	* @param 
	*/
	bool ClearElfLayerAllData();
	/**
	* Instruction : 
	* @param 
	*/
	bool CreateSpriteElfLayer();

	void HandleSaveAwakeResultMessage();
	void HandleAwakeSuccessMessage(int courage,int deltaCharm,int deltaTrick,bool bSuc);

	void PushExploreMapIdToData(unsigned int type,unsigned int mapId);
	void PushExploreBasicRewardInfo(unsigned int map_id,unsigned int add_coin,unsigned int add_exp,unsigned int add_exploit);
	void PushExploreOtherRewardInfo(unsigned int item_id,unsigned int item_level,unsigned int item_num);
	void PushExploreLeftNums(unsigned int leftNums);
	void PushExploreNums(unsigned int Nums);
	void ExploreRewardCommit();

	//void PushElfStrengthData(unsigned int num_1,unsigned int num_2,unsigned int num_3,unsigned int num_4);

	bool TellIsElfId(unsigned int roleId);
	void PushExchangeElfRet(unsigned int elfId);

	void CheckWhetherNewElfReleaseByReputation();
protected:
	bool checkElfConfigCotent();
	bool checkElfLogicContent();
	bool checkElfExploreContent();
	bool checkElfStrengthContent();
private:
	SpriteElfConfigFromLuaManager();
	lua_State *m_configLuaState;

	bool mIsLoadElfConfigContent;
	bool mIsLoadElfLogicContent;
	bool mIsLoadElfExploreContent;
	bool mIsLoadElfStrengthContent;
};