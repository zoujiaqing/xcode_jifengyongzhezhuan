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


#include <libxml/parser.h>

#include "ItemManager.h"
#include "ItemBase.h"
#include "EquipmentItem.h"
#include "GameConfigFromLuaManager.h"
#include "GameResourceManager.h"
#include "NetWorkConfigure.h"
#include "LuaTinkerManager.h"
#include "Localization.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "tolua++.h"

static void EnumTableItem(lua_State *L, const char* lpszTableName,std::vector<unsigned int> &outVec)
{
	outVec.clear();

	lua_getglobal(L, lpszTableName); 
	int it = lua_gettop(L);  
	lua_pushnil(L);         
	while(lua_next(L, it))  
	{
		int value = (int)lua_tonumber(L, -2);
		printf("%d\n", value); 		
		outVec.push_back(value);
		lua_pop(L, 1);  
	}
	lua_pop(L, 1);
}

GameConfigFromLuaManager::GameConfigFromLuaManager()
{
	this->m_configLuaState = NULL;
	this->initLuaState();
	//loadedFiles.clear();

	this->m_isLoadMapMusicData = false;
	//this->m_isLoadOsCommonMethodData = false;
	this->m_isLoadToolBarTableData = false;
	this->m_isLoadSkillConfigData = false;
	this->m_isLoadCommonDefineData = false;
}

GameConfigFromLuaManager::~GameConfigFromLuaManager() 
{

}

void GameConfigFromLuaManager::initLuaState() {

	// get global luad state
	this->m_configLuaState = CCLuaEngine::defaultEngine()->getLuaState();
}

//bool GameConfigFromLuaManager::checkAnyLoadFile(string filePath)
//{
//	
//	if (loadedFiles.find(filePath) == loadedFiles.end())
//	{
//		const char *path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(filePath.c_str());
//
//		unsigned long fileSize = 0;
//		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rt", &fileSize);
//		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
//
//		if (ret != 0) {
//			CCLOG("load %s file error: %s", filePath.c_str(), lua_tostring(this->m_configLuaState, -1));
//			lua_pop(this->m_configLuaState, 1);
//			return false;
//		}
//
//		loadedFiles.insert(filePath);
//		return true;
//	}
//
//	return true;
//}

//bool GameConfigFromLuaManager::checkConfigLoadFile(string fileName)
//{
//	string filePath = "Script/GameConfig/" + fileName + ".lua";
//    #if 0
//	return checkAnyLoadFile(filePath);
//    #else
//    return LuaTinkerManager::Get()->checkAnyLoadFile(filePath);
//    #endif
//}

bool GameConfigFromLuaManager::getEquipmentItemFieldById(const int itemId, const char * field , const char* itemState, std::string& Val)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	const char *desc = "";

	lua_getglobal(this->m_configLuaState, itemState);
	lua_pushnumber(this->m_configLuaState, itemId);
	lua_pushstring(this->m_configLuaState, field);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		//CCLOG("call getItemDesc error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		desc = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	Val = desc;
    return true;
}

bool GameConfigFromLuaManager::getEquipmentItemFieldById(const int itemId, const char * field , const char* itemState, int& Val)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	lua_getglobal(this->m_configLuaState, itemState);
	lua_pushnumber(this->m_configLuaState, itemId);
	lua_pushstring(this->m_configLuaState, field);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		//CCLOG("call getItemDesc error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isnumber(this->m_configLuaState, -1)) {
		Val = lua_tointeger(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);
    return true;
}

bool GameConfigFromLuaManager::getEquipmentItemFieldById(const int itemId, const char * field , const char* itemState, double& Val)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	lua_getglobal(this->m_configLuaState, itemState);
	lua_pushnumber(this->m_configLuaState, itemId);
	lua_pushstring(this->m_configLuaState, field);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		//CCLOG("call getItemDesc error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isnumber(this->m_configLuaState, -1)) {
		Val = lua_tonumber(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);
    return true;
}

bool GameConfigFromLuaManager::getEquipmentItemFieldById(const int itemId, const char * field , const char* itemState, bool& Val)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	lua_getglobal(this->m_configLuaState, itemState);
	lua_pushnumber(this->m_configLuaState, itemId);
	lua_pushstring(this->m_configLuaState, field);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		//CCLOG("call getItemDesc error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isnumber(this->m_configLuaState, -1)) {
		Val = lua_tonumber(this->m_configLuaState, -1) != 0;
	}

	lua_pop(this->m_configLuaState, 1);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// get resource id
#include "TXGUIHeader.h"
#define UNKNOWN_ITEM_RESOURCE_ID   "map_ui_system_icon_FRAME_499999"

int GameConfigFromLuaManager::getItemResourceId(const int itemId) {
	char szKey[256] = "";
	sprintf(szKey, "map_ui_system_icon_FRAME_%d", itemId);
	int iVal = TXGUI::getResourceIDByName(szKey);
	if (iVal < 0)
	{
		/// can't find the resource from resources.h
		return TXGUI::getResourceIDByName(UNKNOWN_ITEM_RESOURCE_ID);
	}
	return iVal;
}

bool GameConfigFromLuaManager::getInstanceBonusInfo(int instanceId, InstanceInfoBonusInfo* info)
{
    if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/RaidMusic.lua"))
        return false;

    if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/Level.lua"))
        return false;

    if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/UserLevel.lua"))
        return false;

    if(! checkMapMusicContent())
        return false;

    std::string drop1, drop2, drop3, drop4, drop5;

    int id = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/MapMusicTable.lua","getInstanceId", instanceId);

    info->money = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "money");
    info->Exploit = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Exploit");
    info->command_power = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Fighting");
	info->max_number = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Number");
	info->power = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Power");
    info->exp   = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Power");

    int level = UserData::Get()->GetUserLevel();

    int type = LuaTinkerManager::Get()->getLuaConfig<int>("RaidMusic", "Map_Basic", id, "Type");
// 
//     if (type == 6)
//     {
//         info->exp = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/UserLevel.lua","getInstanceElseExp", level);
//     } else
//     {
//         info->exp = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/UserLevel.lua","getInstanceExp", level);
//     }

    drop1 = std::string(LuaTinkerManager::Get()->getLuaConfig<const char*>("RaidMusic", "Map_Basic", id, "RaidDrops1"));
    drop2 = std::string(LuaTinkerManager::Get()->getLuaConfig<const char*>("RaidMusic", "Map_Basic", id, "RaidDrops2"));
    drop3 = std::string(LuaTinkerManager::Get()->getLuaConfig<const char*>("RaidMusic", "Map_Basic", id, "RaidDrops3"));
    drop4 = std::string(LuaTinkerManager::Get()->getLuaConfig<const char*>("RaidMusic", "Map_Basic", id, "RaidDrops4"));
    drop5 = std::string(LuaTinkerManager::Get()->getLuaConfig<const char*>("RaidMusic", "Map_Basic", id, "RaidDrops5"));

    info->drops.clear();
    parseInstanceBonusInfo(drop1, info->drops);
    parseInstanceBonusInfo(drop2, info->drops);
    parseInstanceBonusInfo(drop3, info->drops);
    parseInstanceBonusInfo(drop4, info->drops);
    parseInstanceBonusInfo(drop5, info->drops);
    return true;
}

void GameConfigFromLuaManager::parseInstanceBonusInfo(std::string strBonus, std::vector<int>& vtDrops)
{
    unsigned int index = 0;
    do 
    {
        if (index == -1)
        {
            break;
        }

        int cent = strBonus.substr(index, strBonus.size() - index).find_first_of(";");
        std::string singleBonus;
        if (cent > 0)
        {
            singleBonus = strBonus.substr(index, cent - index);
            index = cent + 1;
        } else
        {
            singleBonus = strBonus.substr(index, strBonus.size() - index);
            index = -1;
        }

        int dot = singleBonus.find_first_of("/");
        int drop;
        if (dot == -1)
        {
            drop = atoi(singleBonus.c_str());
        } else
        {
            drop = atoi(singleBonus.substr(0, dot).c_str());
        }

        if (drop != 0)
        {
            // 策划会配置重复的，所以要去掉重复的
            if (std::find(vtDrops.begin(), vtDrops.end(), drop) == vtDrops.end())
            {
                vtDrops.push_back(drop);
            }
        }

    } while (1);
}

 bool GameConfigFromLuaManager::getEquipmentItem(const int itemId, EquipmentItem* item)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	string Name = "",  Description = "";
	int Owner, Part,Character, Level, GetPrice, SellPrice, RequiredLevel, Quality, iconId,order;

	/// get all the filed in the equipment table
	bool isExist = getEquipmentItemFieldById(itemId, "Name", "getEquipmentField", Name);
	isExist &= getEquipmentItemFieldById(itemId, "Description", "getEquipmentField", Description);
	isExist &= getEquipmentItemFieldById(itemId, "Owner", "getEquipmentField", Owner);
	isExist &= getEquipmentItemFieldById(itemId, "Part", "getEquipmentField", Part);	
	isExist &= getEquipmentItemFieldById(itemId, "Character", "getEquipmentField", Character);
	isExist &= getEquipmentItemFieldById(itemId, "Level", "getEquipmentField", Level);
	isExist &= getEquipmentItemFieldById(itemId, "Order", "getEquipmentField", order);
	isExist &= getEquipmentItemFieldById(itemId, "SellPrice", "getEquipmentField", SellPrice);
	isExist &= getEquipmentItemFieldById(itemId, "LookFace", "getEquipmentField", iconId);
	isExist &= getEquipmentItemFieldById(itemId, "Quality", "getEquipmentField", Quality);
    if(!isExist)
    {
        return false;
    }
    /// copy to the item
	item->m_ownerType = Owner;
	item->m_equipPart = Part;
	item->m_type = Character;
	item->m_sellprice = SellPrice;
	item->m_order = order;
	item->m_requiredLevel = Level;
	item->m_quality = Quality;
	item->m_iconId = iconId;
	item->m_id = itemId;
	const char* desStr = Localizatioin::getLocalization(Description.c_str());
	sprintf(item->m_szDescription, "%s", desStr);
	const char* nameStr = Localizatioin::getLocalization(Name.c_str());
	sprintf(item->m_name,  "%s", nameStr);

	return true;
}

bool  GameConfigFromLuaManager::getItem(const int itemId, ItemBase* item)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	string Name = "",  Description = "", UseDes = "";
	int  SellPrice, RequiredLevel, Quality,Type, iconId,order,banch;

	/// get all the filed in the equipment table
	bool isExist = getEquipmentItemFieldById(itemId, "Name", "getItemField", Name);
	isExist &= getEquipmentItemFieldById(itemId, "description", "getItemField", Description);
	isExist &= getEquipmentItemFieldById(itemId, "Order", "getItemField", order);
	isExist &= getEquipmentItemFieldById(itemId, "SellPrice", "getItemField", SellPrice);
	isExist &= getEquipmentItemFieldById(itemId, "RequiredLevel", "getItemField", RequiredLevel);
	isExist &= getEquipmentItemFieldById(itemId, "Quality", "getItemField", Quality);
	isExist &= getEquipmentItemFieldById(itemId, "Type", "getItemField", Type);
	isExist &= getEquipmentItemFieldById(itemId, "LookFace", "getItemField", iconId);
	isExist &= getEquipmentItemFieldById(itemId, "Tip", "getItemField", UseDes);
	isExist &= getEquipmentItemFieldById(itemId, "Batch", "getItemField", banch);
    if(!isExist)
    {
        return false;
    }
    /// copy to the item
	item->m_type = Type;
	item->m_sellprice = SellPrice;
	//item->m_byeprice = GetPrice;
	item->m_requiredLevel = RequiredLevel;
	item->m_quality = Quality;
	item->m_id = itemId;
	item->m_iconId = iconId;
	item->m_order = order;
	item->m_banch = banch;
	const char* desStr = Localizatioin::getLocalization(Description.c_str());
	sprintf(item->m_szDescription, "%s", desStr);
	const char* nameStr = Localizatioin::getLocalization(Name.c_str());
	sprintf(item->m_name,  "%s", nameStr);
	const char* useStr = Localizatioin::getLocalization(UseDes.c_str());
	item->m_useDes = useStr;

	return true;
}

bool GameConfigFromLuaManager::checkToolBarTableContent()
{
	if (!this->m_isLoadToolBarTableData) {
     #if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/ChatToolLayer.lua");

		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);

		if (ret != 0) {
			CCError("load ToolBar name config file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
        #else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/ChatToolLayer.lua");
        #endif
#ifndef RELOAD_LUA_FILES
		this->m_isLoadToolBarTableData = true;
#endif		
		return true;
	}

	return true;
}

bool GameConfigFromLuaManager::checkSkillUIContent()
{
	if (!this->m_isLoadSkillConfigData) {
     #if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/Skill/SkillDataCenter.lua");
		//#ifndef ANDROID
		//		int ret = luaL_dofile(this->m_configLuaState, path);
		//#else
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
		//#endif
		if (ret != 0) {
			CCError("load SkillUIContent name config file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
        #else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/Skill/SkillDataCenter.lua");
        #endif
#ifndef RELOAD_LUA_FILES
		this->m_isLoadSkillConfigData = true;
#endif		
		return true;
	}

	return true;
}

bool GameConfigFromLuaManager::checkCommonDefineContent()
{
	if (!this->m_isLoadCommonDefineData) {
     #if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/CommonDefine.lua");
		//#ifndef ANDROID
		//		int ret = luaL_dofile(this->m_configLuaState, path);
		//#else
		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
		//#endif
		if (ret != 0) {
			CCError("load checkCommonDefineContent name config file error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return false;
		}
        #else
         int ret =  LuaTinkerManager::Get()->checkAnyLoadFile("Script/CommonDefine.lua");
        #endif
        
#ifndef RELOAD_LUA_FILES
		this->m_isLoadCommonDefineData = true;
#endif		
		return true;
	}

	return true;
}

bool GameConfigFromLuaManager::checkMapMusicContent()
{
	if (!this->m_isLoadMapMusicData)
	{
#if 0
		const char *path = GameResourceManager::sharedManager()
			->storedFullPathFromRelativePath("Script/GameConfig/MapMusicTable.lua");

		unsigned long fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
		int ret = luaL_dostringEx(this->m_configLuaState, (char*)buffer, fileSize);
		if (ret != 0) {
			CCLOG("load MapMusicTable.lua error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);
		}
#else
        LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/MapMusicTable.lua");
#endif
		if(initLuaMusicTable())
		{
			this->m_isLoadMapMusicData = true;
			return true;
		}
	}
	return true;
}

lua_State* GameConfigFromLuaManager::getMainLuaState()
{
	lua_State* L_state = CCLuaEngine::defaultEngine()->getLuaState();
    #if 0
	std::string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Script/main.lua");

	unsigned long fileSize = 0;
	unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &fileSize);
	int ret = luaL_dostringEx(L_state, (char*)buffer, fileSize);

	if (ret != 0) {
		CCLOG("getMainLuaState error:%s", lua_tostring(L_state, -1));
		lua_pop(L_state, 1);
		return NULL;
	}
    #else
    LuaTinkerManager::Get()->checkAnyLoadFile("Script/main.lua");

    #endif
	return L_state;
}

void GameConfigFromLuaManager::getOneMapMonsterIds(unsigned int instance_id,unsigned int mapId,std::vector<unsigned int> &monsterIdVec)
{
	if (false == LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/MapDataTable.lua"))
		return ;

	monsterIdVec.clear();

	lua_getglobal(this->m_configLuaState, "GetOneMapMonsterIds");
	lua_pushnumber(this->m_configLuaState, instance_id);
	lua_pushnumber(this->m_configLuaState, mapId);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call getOneMapMonsterIds error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return ;
	}

	if (lua_isboolean(this->m_configLuaState, -1)) {
		bool bSuc = lua_toboolean(this->m_configLuaState, -1);
		if (bSuc == false)
		{
			lua_pop(m_configLuaState, 1);
			return;
		}
	}

	lua_pop(m_configLuaState, 1);

	lua_getglobal(this->m_configLuaState,"G_MonsterTable");
	int nIndex = lua_gettop( m_configLuaState );
	bool bTable = lua_istable(m_configLuaState, -1);
	if (!bTable)
	{
		lua_pop(m_configLuaState, 1);
		return ;
	}
	lua_pushnil( m_configLuaState );
	while( 0 != lua_next( m_configLuaState, -2 ) )
	{
		unsigned int monsterId = (unsigned int)lua_tonumber(m_configLuaState, -1);
		monsterIdVec.push_back(monsterId);
		lua_pop( m_configLuaState, 1 );
	}		
}

int GameConfigFromLuaManager::getExchangeId(int num)
{
	if (!LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return 0;

	lua_getglobal(this->m_configLuaState, "getExchangeId");
	lua_pushnumber(this->m_configLuaState, num);
	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call getExchangeId error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return 0;
	}

	int id = 0;
	if (lua_isnumber(this->m_configLuaState, -1)) {
		id = lua_tointeger(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);
	return id;
}

void GameConfigFromLuaManager::getExchangeItems()
{
	int i = 1;

	string fromItems,toItems;
	int type;
	int level;

	bool success = true;
	while(success)
	{
		int num = getExchangeId(i);
		success = getExchangeItemFieldById(num, "FromItems", fromItems);
		success &= getExchangeItemFieldById(num, "ToItems", toItems);
		success &= getExchangeItemFieldById(num, "Type",type);	
		success &= getExchangeItemFieldById(num, "Level",level);
		
		if(success)
		{
			ItemManager::Get()->addExchangeItem(num,fromItems,toItems,type,level);
		}
		i++;
	}
	
}

bool GameConfigFromLuaManager::getExchangeItemFieldById(const int itemId, const char* field ,std::string& Val)
{
	if (!LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	lua_getglobal(this->m_configLuaState, "getExchangeField");
	lua_pushnumber(this->m_configLuaState, itemId);
	lua_pushstring(this->m_configLuaState, field);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call getExchangeItemDesc error:%s,%s", lua_tostring(this->m_configLuaState, -1),field);
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isstring(this->m_configLuaState, -1)) {
		Val = lua_tostring(this->m_configLuaState, -1);
	}
	else
	{
		lua_pop(this->m_configLuaState, 1);
		return false;
	}

	lua_pop(this->m_configLuaState, 1);
	return true;
}

bool GameConfigFromLuaManager::getExchangeItemFieldById(const int itemId, const char* field , int& Val)
{
	if (!LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return false;

	lua_getglobal(this->m_configLuaState, "getExchangeField");
	lua_pushnumber(this->m_configLuaState, itemId);
	lua_pushstring(this->m_configLuaState, field);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call getExchangeItemDesc error:%s,%s", lua_tostring(this->m_configLuaState, -1),field);
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isnumber(this->m_configLuaState, -1)) {
		Val = lua_tointeger(this->m_configLuaState, -1);
	}
	else
	{
		lua_pop(this->m_configLuaState, 1);
		return false;
	}
	lua_pop(this->m_configLuaState, 1);
	return true;
}

TXGUI::UILayout* GameConfigFromLuaManager::InitLayer(CCNode* parentLayer,const char* initFunName)
{
	//if (!this->checkToolBarTableContent())
	//	return false;

    if(!LuaTinkerManager::Get()->checkAnyLoadFile("Script/ChatToolLayer.lua"))
    {
        return false;
    }
    
	TXGUI::UILayout* pResult = 0;

	lua_getglobal(this->m_configLuaState, initFunName);
	tolua_pushusertype(this->m_configLuaState,(void*)parentLayer,"CCNode");

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call InitLayer error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return false;
	}

	if (lua_isuserdata(this->m_configLuaState, -1)) {
		pResult = (TXGUI::UILayout*)lua_touserdata(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);
	return pResult;
}

int GameConfigFromLuaManager::getSearchSpiritNpcCost(int npcId)
{
	int result = -1;
	if (!LuaTinkerManager::Get()->checkAnyLoadFile("Script/GameConfig/ItemConfigTab.lua"))
		return result;
	lua_getglobal(this->m_configLuaState, "getSearchSpiritField");
	lua_pushnumber(this->m_configLuaState, npcId);
	lua_pushstring(this->m_configLuaState, "Money");

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call getSearchSpiritField error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return result;
	}

	if (lua_isnumber(this->m_configLuaState, -1)) {
		result = lua_tointeger(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);
	return result;
}

void GameConfigFromLuaManager::PushSkillItemToLua(unsigned int skillId,unsigned int levelId,unsigned int maxLevel)
{
	if (!this->checkSkillUIContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushSkillItemToDataCenter");
	lua_pushnumber(this->m_configLuaState, skillId);
	lua_pushnumber(this->m_configLuaState, levelId);
	lua_pushnumber(this->m_configLuaState, maxLevel);

	int ret = lua_pcall(this->m_configLuaState, 3, 1, 0);
	if (ret != 0) {
		CCLOG("call PushSkillItemToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::InitSkillUIDataCenter()
{
	if (!this->checkSkillUIContent())
		return ;

	lua_getglobal(this->m_configLuaState, "SkillDataCenterInitData");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) {
		CCLOG("call InitSkillUIDataCenter error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

const char* GameConfigFromLuaManager::GetOneSkillIconName(int rewardId)
{
	if (!this->checkSkillUIContent())
		return NULL;

	lua_getglobal(this->m_configLuaState, "GetOneSkillIconName");
	lua_pushnumber(this->m_configLuaState,rewardId);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) {
		CCLOG("call GetOneSkillIconName error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return NULL;
	}

	const char * result = NULL;

	if (lua_isstring(this->m_configLuaState, -1)) {
		result = lua_tostring(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return result;
}

void GameConfigFromLuaManager::PushDiffInfoToLua(unsigned int skill_id,unsigned int skill_level,
	unsigned int next_money,unsigned int next_exploit,unsigned int next_lv,float diff_own_cd,float continue_time_cd,unsigned int diff_hurt,
	unsigned int physical_attack,unsigned int magic_attack,unsigned int skill_attack,unsigned int physical_defence,
	unsigned int magic_defence,unsigned int skill_defence,unsigned int health_point,
	unsigned int accurate,unsigned int dodge,unsigned int wreck,unsigned int parry,unsigned int critical_strike,
	unsigned int tenacity,unsigned int slay,unsigned int speed,unsigned int proficiency)
{
	if (!this->checkSkillUIContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushDiffInfoToLua");
	lua_pushnumber(this->m_configLuaState, skill_id);
	lua_pushnumber(this->m_configLuaState, skill_level);

	lua_pushnumber(this->m_configLuaState, next_money);
	lua_pushnumber(this->m_configLuaState, next_exploit);
	lua_pushnumber(this->m_configLuaState, next_lv);
	lua_pushnumber(this->m_configLuaState, diff_own_cd);
	lua_pushnumber(this->m_configLuaState, continue_time_cd);
	lua_pushnumber(this->m_configLuaState, diff_hurt);

	lua_pushnumber(this->m_configLuaState, physical_attack);
	lua_pushnumber(this->m_configLuaState, magic_attack);
	lua_pushnumber(this->m_configLuaState, skill_attack);
	lua_pushnumber(this->m_configLuaState, physical_defence);
	lua_pushnumber(this->m_configLuaState, magic_defence);
	lua_pushnumber(this->m_configLuaState, skill_defence);
	lua_pushnumber(this->m_configLuaState, health_point);
	lua_pushnumber(this->m_configLuaState, accurate);
	lua_pushnumber(this->m_configLuaState, dodge);
	lua_pushnumber(this->m_configLuaState, wreck);
	lua_pushnumber(this->m_configLuaState, parry);
	lua_pushnumber(this->m_configLuaState, critical_strike);
	lua_pushnumber(this->m_configLuaState, tenacity);
	lua_pushnumber(this->m_configLuaState, slay);
	lua_pushnumber(this->m_configLuaState, speed);
	lua_pushnumber(this->m_configLuaState, proficiency);

	int ret = lua_pcall(this->m_configLuaState, 24, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call PushDiffInfoToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::AddUserSkillRspToLua(unsigned int skill_id,unsigned int skill_level,unsigned int maxLevel)
{
	if (!this->checkSkillUIContent())
		return ;

	lua_getglobal(this->m_configLuaState, "handleMsgAddUserSkillRsp");
	lua_pushnumber(this->m_configLuaState, skill_id);
	lua_pushnumber(this->m_configLuaState, skill_level);
	lua_pushnumber(this->m_configLuaState,maxLevel);

	int ret = lua_pcall(this->m_configLuaState, 3, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call AddUserSkillRspToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::PushSkillSlotInfoToLua(unsigned int slotIndex,unsigned int value)
{
	if (!this->checkSkillUIContent())
		return ;

	lua_getglobal(this->m_configLuaState, "PushSkillSlotInfoToLua");
	lua_pushnumber(this->m_configLuaState, slotIndex - SKILL_SLOT_1 + 1);
	lua_pushnumber(this->m_configLuaState, value);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call AddUserSkillRspToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::EquipSkillToLeftSlot(unsigned int skillId)
{
	if (!this->checkSkillUIContent())
		return ;

	lua_getglobal(this->m_configLuaState, "EquipSkillToLeftSlot");
	lua_pushnumber(this->m_configLuaState, skillId);

	int ret = lua_pcall(this->m_configLuaState, 1, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call EquipSkillToLeftSlot error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::ListItemDragToSLotItem(unsigned int skillItemIndex,unsigned int slotIndex)
{
	lua_getglobal(this->m_configLuaState, "ListItemDragToSLotItem");
	lua_pushnumber(this->m_configLuaState, skillItemIndex);
	lua_pushnumber(this->m_configLuaState, slotIndex);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call ListItemDragToSLotItem error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::SlotItemToSelf(unsigned int preSlotIndex,unsigned int toSlotIndex)
{
	lua_getglobal(this->m_configLuaState, "SlotItemToSelf");
	lua_pushnumber(this->m_configLuaState, preSlotIndex);
	lua_pushnumber(this->m_configLuaState, toSlotIndex);

	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call SlotItemToSelf error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

void GameConfigFromLuaManager::SkillLevelUpSucMessage()
{
	lua_getglobal(this->m_configLuaState, "SkillLevelUpSucMessage");

	int ret = lua_pcall(this->m_configLuaState, 0, 1, 0);
	if (ret != 0) 
	{
		CCLOG("call SkillLevelUpSucMessage error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
		return ;
	}

	lua_pop(this->m_configLuaState, 1);
}

int GameConfigFromLuaManager::getInstanceMusicId(const int raidId,const int mapId)
{
	if(!checkMapMusicContent())
		return 0;

	int musicId = 0;
	lua_getglobal(this->m_configLuaState, "getInstanceMusicId");
	lua_pushnumber(this->m_configLuaState, raidId);
	lua_pushnumber(this->m_configLuaState, mapId);
	int ret = lua_pcall(this->m_configLuaState, 2, 1, 0);
	if (ret != 0) {
		CCLOG("call getInstanceMusicId error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);

		return musicId;
	}

	if (lua_isnumber(this->m_configLuaState, -1)) {
		musicId = lua_tointeger(this->m_configLuaState, -1);
	}

	lua_pop(this->m_configLuaState, 1);

	return musicId;
}

void GameConfigFromLuaManager::PushLayerInstanceToLua(cocos2d::CCLayer* pLayer)
{
	if(!checkCommonDefineContent())
		return ;

	lua_getglobal(this->m_configLuaState, "SetLayerInstance");
	tolua_pushusertype(this->m_configLuaState,(void*)pLayer,"CCLayer");

	int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
	if (ret != 0) {
		CCLOG("call PushLayerInstanceToLua error:%s", lua_tostring(this->m_configLuaState, -1));
		lua_pop(this->m_configLuaState, 1);
	}
}

bool GameConfigFromLuaManager::initLuaMusicTable()
{
	lua_getglobal(this->m_configLuaState, "initInstanceMusic");
	int ret = lua_pcall(this->m_configLuaState, 0, 0, 0);
	if(ret != 0)
	{
		CCLOG("call initInstanceMusic error:%s", lua_tostring(this->m_configLuaState, -1));
		return false;
	}
	return true;

}

//int GameConfigFromLuaManager::table_next(lua_State *L, int i,char **k, char **v)
//{
//	if ( lua_next(L, i) !=0 )
//	{
//		*k = (char *)lua_tostring(L, -2);
//		*v = (char *)lua_tostring(L, -1);
//		lua_pop(L, 1);
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}	
//}
