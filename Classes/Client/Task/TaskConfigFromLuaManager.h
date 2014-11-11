#pragma once

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

class TaskConfigFromLuaManager
{
public :	
	virtual ~TaskConfigFromLuaManager();
	static TaskConfigFromLuaManager* getInstance(void);
	static void Destroy();

	virtual void initLuaState();

	bool checkTaskDataCotent();
	bool IsTaskWaitForGetReward(unsigned int task_id,unsigned int task_step);
	unsigned int GetTaskAttachNpcId(unsigned int task_id);
	const char*  GetOneTaskName(unsigned int task_id);
	bool GetOneTaskRewardMoney(unsigned int task_id,unsigned int &moneyNum);
	bool GetOneTaskRewardExp(unsigned int task_id,unsigned int &expNum);
	/**
	* Instruction : 
	* @param 
	*/
	unsigned int GetOneTaskStepNums(unsigned int task_id);
	/**
	* Instruction : 获取接受任务时NPC对话时说话内容
	* @param 
	*/
	const char* GetRecieveTextFromNpc(unsigned int task_id,unsigned int talk_index);
	/**
	* Instruction : 获取接受任务时Hero对话时说话内容
	* @param 
	*/
	const char* GetRecieveTextFromHero(unsigned int task_id,unsigned int talk_index);
	/**
	* Instruction : 获取完成文本
	* @param 
	*/
	const char* GetCommonCompleteContent();
	/**
	* Instruction : 判断接受任务对话是否完成
	* @param 
	*/
	bool TellIsRecieveTalkOver(unsigned int task_id,unsigned int talk_index);
	/**
	* Instruction : 获取领取奖励时NPC对话时说话内容
	* @param 
	*/
	const char* GetCompleteTextFromNpc(unsigned int task_id,unsigned int talk_index);
	/**
	* Instruction : 获取领取奖励时Hero对话时说话内容
	* @param 
	*/
	const char* GetCompleteTextFromHero(unsigned int task_id,unsigned int talk_index);
	/**
	* Instruction : 判断领取奖励对话是否完成
	* @param 
	*/
	bool TellIsCompleteTalkOver(unsigned int task_id,unsigned int talk_index);
	/**
	* Instruction : 获取Npc公用的对话
	* @param 
	*/
	const char* GetCommonNpcTalkFlagString();
	/**
	* Instruction : 获取Hero公用的对话
	* @param 
	*/
	const char* GetCommonHeroTalkFlagString();
	/**
	* Instruction : 获取任务的类型 1 对话 2通关 3杀怪 4收集
	* @param 
	*/
	unsigned int GetOneTaskType(unsigned int task_id);
	/**
	* Instruction : 获取某任务需要对话的Npc的id
	* @param 
	*/
	unsigned int GetOneTaskTalkToNpcId(unsigned int task_id);
	/**
	* Instruction : 获取某一个副本的icon
	* @param 
	*/
	const char* GetOneTaskClearanceIconName(unsigned int task_id,unsigned int step);
	/**
	* Instruction : 获取任务相关的副本入口id
	* @param 
	*/
	unsigned int GetOneTaskInstanceId(unsigned int task_id,unsigned int step);
	/**
	* Instruction : 获取任务描述
	* @param 
	*/
	const char* GetOneTaskDescription(unsigned int task_id);
	/**
	* Instruction : 获取任务进程
	* @param 
	*/
	const char* GetOneTaskProgress(unsigned int task_id,int value,bool bWaitForReward);
	/**
	* Instruction : 获取某任务获得的奖励精灵ID
	* @param 
	*/
	unsigned int GetOneTaskRewardElfID(unsigned int task_id);
private:
	TaskConfigFromLuaManager();

	lua_State *m_configLuaState;

	bool m_isLoadTaskData;
};