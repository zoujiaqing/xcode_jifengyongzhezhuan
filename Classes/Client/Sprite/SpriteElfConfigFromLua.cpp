#include "SpriteElfConfigFromLua.h"
#include "GameResourceManager.h"
#include "CCLuaEngine.h"
#include "SpriteElfDataCenter.h"
#include "tolua++.h"
#include "UserData.h"
#include "LuaTinkerManager.h"

static SpriteElfConfigFromLuaManager* _spriteElfConfigFromLuaManager = 0;

SpriteElfConfigFromLuaManager* SpriteElfConfigFromLuaManager::getInstance(void)
{
	if(_spriteElfConfigFromLuaManager == 0)
	{
		_spriteElfConfigFromLuaManager = new SpriteElfConfigFromLuaManager();
	}
	return _spriteElfConfigFromLuaManager;
}

void SpriteElfConfigFromLuaManager::Destroy()
{
	if (_spriteElfConfigFromLuaManager)
	{
		delete _spriteElfConfigFromLuaManager;
		_spriteElfConfigFromLuaManager = NULL;
	}
}

SpriteElfConfigFromLuaManager::SpriteElfConfigFromLuaManager()
{
	m_configLuaState = 0;
	mIsLoadElfConfigContent = false;
	mIsLoadElfLogicContent = false;
	mIsLoadElfExploreContent = false;
	mIsLoadElfStrengthContent = false;

	this->initLuaState();
}

SpriteElfConfigFromLuaManager::~SpriteElfConfigFromLuaManager()
{

}

void SpriteElfConfigFromLuaManager::initLuaState()
{
	this->m_configLuaState = CCLuaEngine::defaultEngine()->getLuaState();
}

bool SpriteElfConfigFromLuaManager::checkElfConfigCotent()
{
	if (!this->mIsLoadElfConfigContent) {
#if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/SpriteElf/ElfConfigParse.lua");
//#ifndef ANDROID
//		int ret = luaL_dofile(this->m_configLuaState, path);
//#else
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
//#endif
		if (ret != 0) {
			CCLOG("load kElfConfigCotent def name config file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
#else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/SpriteElf/ElfConfigParse.lua");
#endif
#ifndef RELOAD_LUA_FILES
		this->mIsLoadElfConfigContent = true;
#endif				
		return true;
	}

	return true;
}

bool SpriteElfConfigFromLuaManager::checkElfLogicContent()
{
	if (!this->mIsLoadElfLogicContent) {
     #if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/SpriteElf/SpriteElfLayer.lua");
//#ifndef ANDROID
//		int ret = luaL_dofile(this->m_configLuaState, path);
//#else
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
//#endif
		if (ret != 0) {
			CCLOG("SpriteElfConfigFromLuaManager::checkElfLogicContent file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
        #else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/SpriteElf/SpriteElfLayer.lua");

        #endif
#ifndef RELOAD_LUA_FILES
		this->mIsLoadElfLogicContent = true;
#endif	
		return true;
	}

	return true;
}

bool SpriteElfConfigFromLuaManager::checkElfExploreContent()
{
	if (!this->mIsLoadElfExploreContent) {
#if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/SpriteElfExplore/ElfExploreLayer.lua");
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
		if (ret != 0) {
			CCLOG("SpriteElfConfigFromLuaManager::checkElfExploreContent file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
#else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/SpriteElfExplore/ElfExploreLayer.lua");

#endif
#ifndef RELOAD_LUA_FILES
		this->mIsLoadElfExploreContent = true;
#endif	
		return true;
	}

	return true;
}

bool SpriteElfConfigFromLuaManager::checkElfStrengthContent()
{
	if (!this->mIsLoadElfStrengthContent) {
#if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/SpriteElf/SpriteElfStrengthenLayer.lua");
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
		if (ret != 0) {
			CCLOG("SpriteElfConfigFromLuaManager::checkElfStrengthContent file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
#else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/SpriteElf/SpriteElfStrengthenLayer.lua");

#endif
#ifndef RELOAD_LUA_FILES
		this->mIsLoadElfStrengthContent = true;
#endif	
		return true;
	}

	return true;
}

bool SpriteElfConfigFromLuaManager::GetElfToHeroFixedDistance(float &posx,float &posy)
{
	if (false == this->checkElfConfigCotent())
		return false;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetElfToHeroFixedDistance");

	int ret = lua_pcall(this->m_configLuaState, 0, 2, 0);
	if (ret != 0) {
		CCLOG("call GetElfToHeroFixedDistance error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		posy = lua_tonumber(this->m_configLuaState, -1);
	}

	if (lua_isstring(this->m_configLuaState, -2)) {
		posx = lua_tonumber(this->m_configLuaState, -2);
	}

	lua_pop(this->m_configLuaState, 2);

	return true;
}

float SpriteElfConfigFromLuaManager::GetOuterCircleRadius()
{
	if (false == this->checkElfConfigCotent())
		return false;

	float result = 0;

	lua_getglobal(this->m_configLuaState, "GetOuterCircleRadius");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOuterCircleRadius error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

float SpriteElfConfigFromLuaManager::GetInnerCircleRadius()
{
	if (false == this->checkElfConfigCotent())
		return false;

	float result = 0;

	lua_getglobal(this->m_configLuaState, "GetInnerCircleRadius");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetInnerCircleRadius error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

float SpriteElfConfigFromLuaManager::GetTotalAccTimer()
{
	if (false == this->checkElfConfigCotent())
		return false;

	float result = 0;

	lua_getglobal(this->m_configLuaState, "GetTotalAccTimer");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetTotalAccTimer error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

float SpriteElfConfigFromLuaManager::GetStartSpeedCoefficient()
{
	if (false == this->checkElfConfigCotent())
		return false;

	float result = 0;

	lua_getglobal(this->m_configLuaState, "GetStartSpeedCoefficient");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetStartSpeedCoefficient error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

float SpriteElfConfigFromLuaManager::GetMaxMoveSpeedCoefficient()
{
	if (false == this->checkElfConfigCotent())
		return false;

	float result = 0;

	lua_getglobal(this->m_configLuaState, "GetMaxMoveSpeedCoefficient");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetMaxMoveSpeedCoefficient error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

//void SpriteElfConfigFromLuaManager::PushElfIdToLuaTable()
//{
//	std::vector<ElfAttachData> vecData = SpriteElfDataCenter::Get()->GetElfData();
//	lua_newtable(this->m_configLuaState);
//	int count = 1;
//	for (std::vector<ElfAttachData>::iterator iter = vecData.begin();
//		iter != vecData.end();iter++)
//	{
//		lua_pushnumber(this->m_configLuaState,(count));
//		lua_pushnumber(this->m_configLuaState,(*iter).nElfId);
//		lua_settable(this->m_configLuaState,-3);
//		count++;
//	}
//
//	lua_setglobal(this->m_configLuaState, "ElfIdTable" );
//}

bool SpriteElfConfigFromLuaManager::PushElfStateMessageToLua(int elfId,int stateId)
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "HandleRsqStateMessageFromServer");
	lua_pushnumber(this->m_configLuaState,elfId);
	lua_pushnumber(this->m_configLuaState,stateId);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call PushElfStateMessageToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	lua_pop(this->m_configLuaState, 1);

	return true;
}

bool SpriteElfConfigFromLuaManager::PushElfListMessageToLua(PLAYER_ID roleId,int elfId,int stateId, int grade)
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "HandleRsqListMessageFromServer");
	tolua_pushusertype(this->m_configLuaState,(void*)(&roleId),"PLAYER_ID");
	lua_pushnumber(this->m_configLuaState,elfId);
	lua_pushnumber(this->m_configLuaState,stateId);
    lua_pushnumber(this->m_configLuaState,grade);

	int ret = lua_pcall(this->m_configLuaState, 4, 1, 0);
	if (ret != 0) {
		CCLOG("call PushElfListMessageToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	lua_pop(this->m_configLuaState, 1);

	return true;
}

void SpriteElfConfigFromLuaManager::GetElfListMessageOver()
{
	if (false == this->checkElfLogicContent())
		return ;

	lua_getglobal(this->m_configLuaState, "HandleGetElfListMessageOver");
	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetElfListMessageOver error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
	}

	lua_pop(this->m_configLuaState, 1);
}

bool SpriteElfConfigFromLuaManager::CallLuaStartFunction()
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "InitSpriteElfLayer");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call InitSpriteElfLayer error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	lua_pop(this->m_configLuaState, 1);

	return true;
}

unsigned int SpriteElfConfigFromLuaManager::GetHeroFightElfID()
{
	if (false == this->checkElfLogicContent())
		return 0;

	unsigned int result = 0;

	lua_getglobal(this->m_configLuaState, "GetHeroFightElfID");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call GetHeroFightElfID error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return 0;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

bool SpriteElfConfigFromLuaManager::EquipElfPotientionPanelToBackPackLayer(cocos2d::CCLayer* pLayer)
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "GlobalCreateElfStengthenLayer");
	tolua_pushusertype(this->m_configLuaState,(void*)pLayer,"CCLayer");

	int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
	if (ret != 0) {
		CCLOG("call EquipElfPotientionPanelToBackPackLayer error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	return true;
}

bool SpriteElfConfigFromLuaManager::CloseElfPotientionPanel()
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "GlobalCloseLayer");

	int ret = lua_pcall(this->m_configLuaState, 0, 0, 0);
	if (ret != 0) {
		CCLOG("call CloseElfPotientionPanel error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	return true;
}

void SpriteElfConfigFromLuaManager::ResetValue()
{
	mIsLoadElfConfigContent = false;
	mIsLoadElfLogicContent = false;
	mIsLoadElfExploreContent = false;
	mIsLoadElfStrengthContent = false;
}

bool SpriteElfConfigFromLuaManager::ClearElfLayerAllData()
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "ClearElfLayerAllData");

	int ret = lua_pcall(this->m_configLuaState, 0, 0, 0);
	if (ret != 0) {
		CCLOG("call ClearElfLayerAllData error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	return true;
}

bool SpriteElfConfigFromLuaManager::CreateSpriteElfLayer()
{
	if (false == this->checkElfLogicContent())
		return false;

	lua_getglobal(this->m_configLuaState, "CreateSpriteElfLayer");

	int ret = lua_pcall(this->m_configLuaState, 0, 0, 0);
	if (ret != 0) {
		CCLOG("call CreateSpriteElfLayer error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	return true;
}

void SpriteElfConfigFromLuaManager::HandleSaveAwakeResultMessage()
{
	lua_getglobal(this->m_configLuaState, "HandleSaveAwakeResultMessage");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call HandleSaveAwakeResultMessage error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void SpriteElfConfigFromLuaManager::HandleAwakeSuccessMessage(int deltaCourage,int deltaCharm,int deltaTrick,bool bSuc)
{
	lua_getglobal(this->m_configLuaState, "HandleAwakeSuccessMessage");
	lua_pushnumber(this->m_configLuaState,deltaCourage);
	lua_pushnumber(this->m_configLuaState,deltaCharm);
	lua_pushnumber(this->m_configLuaState,deltaTrick);
	lua_pushboolean(this->m_configLuaState,bSuc);

	int ret = lua_pcall(this->m_configLuaState, 4, 1, 0);
	if (ret != 0) {
		CCLOG("call HandleAwakeSuccessMessage error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}

void SpriteElfConfigFromLuaManager::PushExploreMapIdToData(unsigned int type,unsigned int mapId)
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushExploreMapIdToData");
	lua_pushnumber(this->m_configLuaState, type);
	lua_pushnumber(this->m_configLuaState, mapId);	

	int ret = lua_pcall(this->m_configLuaState, 2, 0, 0);
	if (ret != 0) {
		CCLOG("call PushExploreMapIdToData error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}

void SpriteElfConfigFromLuaManager::PushExploreBasicRewardInfo(unsigned int map_id,unsigned int add_coin,unsigned int add_exp,unsigned int add_exploit)
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushExploreBasicRewardInfo");
	lua_pushnumber(this->m_configLuaState,map_id);
	lua_pushnumber(this->m_configLuaState, add_coin);
	lua_pushnumber(this->m_configLuaState, add_exp);
	lua_pushnumber(this->m_configLuaState, add_exploit);

	int ret = lua_pcall(this->m_configLuaState, 4, 0, 0);
	if (ret != 0) {
		CCLOG("call PushExploreBasicRewardInfo error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}

}

void SpriteElfConfigFromLuaManager::PushExploreOtherRewardInfo(unsigned int item_id,unsigned int item_level,unsigned int item_num)
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushExploreOtherRewardInfo");
	lua_pushnumber(this->m_configLuaState, item_id);
	lua_pushnumber(this->m_configLuaState, item_level);
	lua_pushnumber(this->m_configLuaState, item_num);

	int ret = lua_pcall(this->m_configLuaState, 3, 0, 0);
	if (ret != 0) {
		CCLOG("call PushExploreOtherRewardInfo error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}

void SpriteElfConfigFromLuaManager::PushExploreLeftNums(unsigned int leftNums)
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushExploreLeftNums");
	lua_pushnumber(this->m_configLuaState, leftNums);

	int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
	if (ret != 0) {
		CCLOG("call PushExploreLeftNums error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}
void SpriteElfConfigFromLuaManager::PushExploreNums(unsigned int Nums)
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushExploreNums");
	lua_pushnumber(this->m_configLuaState, Nums);

	int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
	if (ret != 0) {
		CCLOG("call PushExploreNums error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}

void SpriteElfConfigFromLuaManager::ExploreRewardCommit()
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "ExploreRewardCommit");

	int ret = lua_pcall(this->m_configLuaState, 0, 0, 0);
	if (ret != 0) {
		CCLOG("call ExploreRewardCommit error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}

//void SpriteElfConfigFromLuaManager::PushElfStrengthData(unsigned int num_1,unsigned int num_2,unsigned int num_3,unsigned int num_4)
//{
//	if (false == this->checkElfStrengthContent())
//		return ;
//
//	lua_getglobal(this->m_configLuaState, "SetConsumeItemNumInfo");
//	lua_pushnumber(this->m_configLuaState,num_1);
//	lua_pushnumber(this->m_configLuaState,num_2);
//	lua_pushnumber(this->m_configLuaState,num_3);
//	lua_pushnumber(this->m_configLuaState,num_4);
//
//	int ret = lua_pcall(this->m_configLuaState, 4, 0, 0);
//	if (ret != 0) {
//		CCLOG("call PushElfStrengthData error:%s", lua_tostring(this->m_configLuaState, -1));
//		lua_pop(this->m_configLuaState, 1);
//
//		return ;
//	}
//}

void SpriteElfConfigFromLuaManager::PushExchangeElfRet(unsigned int elfId)
{
	if (false == this->checkElfExploreContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushExchangeElfRet");
	lua_pushnumber(this->m_configLuaState,elfId);

	int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
	if (ret != 0) {
		CCLOG("call PushExchangeElfRet error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}
}

bool SpriteElfConfigFromLuaManager::TellIsElfId(unsigned int roleId)
{
	if (false == this->checkElfConfigCotent())
		return 0;

	bool result = false;

	lua_getglobal(this->m_configLuaState, "TellIsElfId");
	lua_pushnumber(this->m_configLuaState,roleId);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call TellIsElfId error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isboolean(this->m_configLuaState, -1)) {
		result = lua_toboolean(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

void SpriteElfConfigFromLuaManager::CheckWhetherNewElfReleaseByReputation()
{
	if (false == this->checkElfLogicContent())
		return ;

	unsigned int reputaion = UserData::GetUserInfo().m_reputation;
	lua_getglobal(this->m_configLuaState, "CheckWhetherNewElfReleaseByReputation");
	lua_pushnumber(this->m_configLuaState,reputaion);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call CheckWhetherNewElfReleaseByReputation error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}
