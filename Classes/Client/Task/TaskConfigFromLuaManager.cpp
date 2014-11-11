#include "TaskConfigFromLuaManager.h"
#include "GameResourceManager.h"
#include "CCLuaEngine.h"
#include "GameConfigFromLuaManager.h"

#include "LuaTinkerManager.h"

static TaskConfigFromLuaManager* _taskConfigFromLuaManager = 0;

TaskConfigFromLuaManager* TaskConfigFromLuaManager::getInstance(void)
{
	if(_taskConfigFromLuaManager == 0)
	{
		_taskConfigFromLuaManager = new TaskConfigFromLuaManager();
	}
	return _taskConfigFromLuaManager;
}

void TaskConfigFromLuaManager::Destroy()
{
	if (_taskConfigFromLuaManager)
	{
		delete _taskConfigFromLuaManager;
		_taskConfigFromLuaManager = NULL;
	}
}

TaskConfigFromLuaManager::TaskConfigFromLuaManager()
{
	m_isLoadTaskData = false;
	m_configLuaState = 0;

	this->initLuaState();
}

TaskConfigFromLuaManager::~TaskConfigFromLuaManager()
{

}

void TaskConfigFromLuaManager::initLuaState()
{
	//GameConfigFromLuaManager::Get()->getNpcBodyIconName(501);
	this->m_configLuaState = CCLuaEngine::defaultEngine()->getLuaState();
}

bool TaskConfigFromLuaManager::checkTaskDataCotent()
{
	if (!this->m_isLoadTaskData) {
#if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/Task/TaskState.lua");
//#ifndef ANDROID
		//int ret = luaL_dofile(this->m_configLuaState, path);
//#else
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
//#endif
		if (ret != 0) {
			CCLOG("load task def name config file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
#else
    LuaTinkerManager::Get()->checkAnyLoadFile("Script/Task/TaskState.lua");

#endif
		this->m_isLoadTaskData = true;
		return true;
	}

	return true;
}

bool TaskConfigFromLuaManager::IsTaskWaitForGetReward(unsigned int task_id,unsigned int task_step)
{
	if (false == this->checkTaskDataCotent())
		return false;

	bool bSuc = false;

	lua_getglobal(this->m_configLuaState, "IsTaskWaitForGetReward");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, task_step);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call IsTaskWaitForGetReward error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isboolean(this->m_configLuaState, -1)) {
		bSuc = lua_toboolean(this->m_configLuaState, -1);
	}
	
	lua_pop(this->m_configLuaState, 1);

	return bSuc;
}

unsigned int TaskConfigFromLuaManager::GetTaskAttachNpcId(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	unsigned int npcId = 0;

	lua_getglobal(this->m_configLuaState, "GetOneTaskAttachNpcIds");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetTaskAttachNpcId error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		npcId = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return npcId;
}

const char*  TaskConfigFromLuaManager::GetOneTaskName(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetOneTaskName");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskName error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

bool TaskConfigFromLuaManager::GetOneTaskRewardMoney(unsigned int task_id,unsigned int &moneyNum)
{
	if (false == this->checkTaskDataCotent())
		return false;

	lua_getglobal(this->m_configLuaState, "GetOneTaskRewardMoneyTable");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskRewardMoney error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	moneyNum = (int)lua_tonumber(m_configLuaState, -1);
	lua_pop(m_configLuaState, 1);

	return true;
}

bool TaskConfigFromLuaManager::GetOneTaskRewardExp(unsigned int task_id,unsigned int &expNum)
{
	if (false == this->checkTaskDataCotent())
		return false;

	lua_getglobal(this->m_configLuaState, "GetOneTaskRewardExpTable");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskRewardMoney error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	expNum = (int)lua_tonumber(m_configLuaState, -1);
	lua_pop(m_configLuaState, 1);

	return true;
}

// Note: 领取任务相关对话数据
const char* TaskConfigFromLuaManager::GetRecieveTextFromNpc(unsigned int task_id,unsigned int talk_index)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetRecieveTextNpc");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, talk_index);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call GetRecieveTextFromNpc error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

const char* TaskConfigFromLuaManager::GetCommonCompleteContent()
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetCommonCompleteContent");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetCommonCompleteContent error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

const char* TaskConfigFromLuaManager::GetRecieveTextFromHero(unsigned int task_id,unsigned int talk_index)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetRecieveTextHero");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, talk_index);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call GetRecieveTextHero error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

bool TaskConfigFromLuaManager::TellIsRecieveTalkOver(unsigned int task_id,unsigned int talk_index)
{
	if (false == this->checkTaskDataCotent())
		return false;

	bool bSuc = false;

	lua_getglobal(this->m_configLuaState, "TellIsRecieveTalkOver");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, talk_index);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call TellIsRecieveTalkOver error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isboolean(this->m_configLuaState, -1)) {
		bSuc = lua_toboolean(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return bSuc;
}

// Note: 领取奖励相关对话数据
const char* TaskConfigFromLuaManager::GetCompleteTextFromNpc(unsigned int task_id,unsigned int talk_index)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetCompleteTextNpc");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, talk_index);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call GetRecieveTextFromNpc error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

const char* TaskConfigFromLuaManager::GetCompleteTextFromHero(unsigned int task_id,unsigned int talk_index)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetCompleteTextHero");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, talk_index);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call GetRecieveTextHero error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

bool TaskConfigFromLuaManager::TellIsCompleteTalkOver(unsigned int task_id,unsigned int talk_index)
{
	if (false == this->checkTaskDataCotent())
		return false;

	bool bSuc = false;

	lua_getglobal(this->m_configLuaState, "TellIsCompleteTalkOver");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, talk_index);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call TellIsRecieveTalkOver error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isboolean(this->m_configLuaState, -1)) {
		bSuc = lua_toboolean(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return bSuc;
}

unsigned int TaskConfigFromLuaManager::GetOneTaskStepNums(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetOneTaskStepNums");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskStepNums error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

const char* TaskConfigFromLuaManager::GetCommonNpcTalkFlagString()
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetCommonNpcTalk");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetRecieveTextHero error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

const char* TaskConfigFromLuaManager::GetCommonHeroTalkFlagString()
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetCommonHeroTalk");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetRecieveTextHero error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

unsigned int TaskConfigFromLuaManager::GetOneTaskType(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetTaskType");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskType error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

unsigned int TaskConfigFromLuaManager::GetOneTaskTalkToNpcId(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetOneTaskTalkToNpcId");
	lua_pushnumber(this->m_configLuaState, task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskTalkToNpcId error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

const char* TaskConfigFromLuaManager::GetOneTaskClearanceIconName(unsigned int task_id,unsigned int step)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetOneTaskClearanceIconName");
	lua_pushnumber(this->m_configLuaState,(int)task_id);
	lua_pushnumber(this->m_configLuaState,step);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskClearanceIconName error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

unsigned int TaskConfigFromLuaManager::GetOneTaskInstanceId(unsigned int task_id,unsigned int step)
{
	if (false == this->checkTaskDataCotent())
		return false;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetTaskInstanceId");
	lua_pushnumber(this->m_configLuaState, task_id);
	lua_pushnumber(this->m_configLuaState, step);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call GetTaskInstanceId error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

const char* TaskConfigFromLuaManager::GetOneTaskDescription(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetOneTaskDescription");
	lua_pushnumber(this->m_configLuaState,(int)task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskDescription error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

const char* TaskConfigFromLuaManager::GetOneTaskProgress(unsigned int task_id,int value,bool bWaitForReward)
{
	if (false == this->checkTaskDataCotent())
		return false;

	const char *name = "";

	lua_getglobal(this->m_configLuaState, "GetOneTaskProgress");
	lua_pushnumber(this->m_configLuaState,(unsigned int)task_id);
	lua_pushnumber(this->m_configLuaState,(int)value);
	lua_pushboolean(this->m_configLuaState,bWaitForReward);

	int ret = lua_pcall(this->m_configLuaState, 3, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskProgress error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		name = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return name;
}

unsigned int TaskConfigFromLuaManager::GetOneTaskRewardElfID(unsigned int task_id)
{
	if (false == this->checkTaskDataCotent())
		return false;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetOneTaskRewardElfID");
	lua_pushnumber(this->m_configLuaState,(unsigned int)task_id);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneTaskRewardElfID error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}
