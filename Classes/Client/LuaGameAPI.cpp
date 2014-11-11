#include  "PPVELayer.h"
#include "MessageHandler.h"
#include "MessageUtil.h"
#include "LuaGameAPI.h"
#include "MainMenuLayer.h"

#include "XLogicManager.h"
#include "GameManager.h"
#include "InstanceManager.h"
#include "UserData.h"
#include "GameUILayer.h"
#include "ItemManager.h"
#include "MessageBoxLayer.h"
#include "UIPopupReward.h"
#include "DiamondWarningLayer.h"
#include "WaitingLayerManager.h"
#include "GuildInfoLayer.h"

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"
#include "tolua_fix.h"
#include "CCLuaEngine.h"

TOLUA_API int tolua_GameApi_open(lua_State *tolua_S);



//register classes
static void tolua_reg_types(lua_State *tolua_S)
{
#ifndef Mtolua_typeid
#define Mtolua_typeid(L,TI,T)
#endif
	tolua_usertype(tolua_S, "GameApi");
	tolua_usertype(tolua_S,"EQUIPINFO");
	Mtolua_typeid(tolua_S,typeid(EQUIPINFO), "EQUIPINFO");
	tolua_usertype(tolua_S,"MessageBox");
	Mtolua_typeid(tolua_S,typeid(MessageBox), "MessageBox");
	tolua_usertype(tolua_S,"PopupRewardLayer");
	Mtolua_typeid(tolua_S,typeid(PopupRewardLayer), "PopupRewardLayer");
	tolua_usertype(tolua_S,"DiamondWarningLayer");
	Mtolua_typeid(tolua_S,typeid(PopupRewardLayer), "DiamondWarningLayer");
}

static int tolua_PPVELayer_InitMap(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::PPVELayer_InitMap'",&tolua_err);
		return 0;
	}
#endif

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		PPVELayer* ppveLayer = mLayer->GetPPVELayer();
		if (ppveLayer)
			ppveLayer->InitMaps();
	}
	return 1;
}

static int tolua_GuildInstance_InitMap(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::GuildInstance_InitMap'",&tolua_err);
		return 0;
	}
#endif

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		GuildInstanceListLayer* guildInstanceLayer = (GuildInstanceListLayer*)(mLayer->getGuildInstanceListLayer());
		if (guildInstanceLayer)
			guildInstanceLayer->InitMaps();
	}
	return 1;
}

static int tolua_GetPPVEAutoChatCD(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::GetPPVEAutoChatCD'",&tolua_err);
		return 0;
	}
#endif
	USERINFO info = UserData::GetUserInfo();
	long lastTime = info.m_lastPPVEChatTime;
	long currentTime = TimeManager::Get()->getCurServerTime();
	int ret = currentTime - lastTime;
	if(ret == 0)
	{
		ret = 1;
	}
	else if(ret >= 10 || ret < 0)
	{
		info.m_lastPPVEChatTime = currentTime;
		UserData::SetUserInfo(info);
		ret = 0;
	}
	tolua_pushnumber(tolua_S,(lua_Number)ret);
	return 1;
}

static int tolua_GetLastStaminaRestoreTime(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::GetLastStaminaRestoreTime'",&tolua_err);
		return 0;
	}
#endif
	USERINFO info = UserData::GetUserInfo();
	long lastTime = info.m_lastStaminaRestoreTime;
	long currentTime = TimeManager::Get()->getCurServerTime();
	long time = currentTime - lastTime;
	int ret = (int)time;
	tolua_pushnumber(tolua_S,(lua_Number)ret);
	return 1;
}

static int tolua_getTimeBySec(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::getTimeBySec'",&tolua_err);
		return 0;
	}
#endif
	unsigned int sec = (unsigned int)lua_tonumber(tolua_S,2);
	//sec += TimeManager::Get()->getTimeZone()*60;
	time_t sec_temp = sec;
	struct tm sec_tm;
	localtime_r(&sec_temp, &sec_tm);
	tolua_pushnumber(tolua_S,(lua_Number)(sec_tm.tm_year+1900));
	tolua_pushnumber(tolua_S,(lua_Number)(sec_tm.tm_mon+1));
	tolua_pushnumber(tolua_S,(lua_Number)sec_tm.tm_mday);
	tolua_pushnumber(tolua_S,(lua_Number)sec_tm.tm_hour);
	tolua_pushnumber(tolua_S,(lua_Number)sec_tm.tm_min);
	return 5;
}

static int tolua_ShowMessage(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::PPVELayer_InitMap'",&tolua_err);
		return 0;
	}
#endif

	string strVal = lua_tostring(tolua_S, 2);
	ShowMessage(strVal.c_str());
	return 1;
}

static int tolua_TestSystemTime(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::TestSystemTime'",&tolua_err);
		return 0;
	}
#endif

	string strVal = lua_tostring(tolua_S, 2);
	LogCurrentSystem(strVal.c_str());
	return 1;
}


static int tolua_CreateSliderCtrl(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"CCNode",0,&tolua_err) ||
       !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
       !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
       !tolua_isnumber(tolua_S, 4, 0, &tolua_err) ||
       !tolua_isnoobj(tolua_S, 5, &tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'PushOneLayer'",&tolua_err);
		return 0;
	}
#endif
    
	CCNode* node = (CCNode*)(tolua_tousertype(tolua_S, 1, NULL));
    int tag = tolua_tonumber(tolua_S, 2, 0);
    int width = tolua_tonumber(tolua_S, 3, 0);
    int heigh = tolua_tonumber(tolua_S, 4, 0);
	MainMenuLayer::CreateSliderCtrl(node,tag,width,heigh);
	return 0;
}

static int tolua_InsertPPVEInstance(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S, 4, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::PPVELayer_InitMap'",&tolua_err);
		return 0;
	}
#endif

	int id = tolua_tonumber(tolua_S, 2, 0);
	int reqLevel = tolua_tonumber(tolua_S, 3, 0);
    if (InstanceManager::Get())
	{
		InstanceManager::Get()->InsertPPVEInstanceId(id, reqLevel);
	}

	return 1;
}

static int tolua_InsertGuildInstance(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S, 4, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::PPVELayer_InitMap'",&tolua_err);
		return 0;
	}
#endif

	int id = tolua_tonumber(tolua_S, 2, 0);
	int reqLevel = tolua_tonumber(tolua_S, 3, 0);
	if (InstanceManager::Get())
	{
		InstanceManager::Get()->InsertGuildInstanceId(id, reqLevel);
	}

	return 1;
}

static int tolua_GetOtherPlayerInfo(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S, 2, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'GameApi::GetOtherPlayerInfo'",&tolua_err);
		return 0;
	}
#endif

	int i = 1;
	std::list<PLAYER_ID>* users = UserData::Get()->GetOtherUserId();
	lua_newtable(tolua_S);
	for(std::list<PLAYER_ID>::iterator iter = users->begin();
		iter != users->end();iter++)
	{
		USERINFO* info = UserData::Get()->GetUserInfo(*iter);

		if(info->type >= 0 && info->type <= 6)
		{
			lua_pushnumber(tolua_S,i++);
			lua_newtable(tolua_S);

			lua_pushstring(tolua_S,"uid");
			tolua_pushusertype(tolua_S,(void*)(&info->id),"PLAYER_ID");
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"name");
			lua_pushstring(tolua_S,info->szName);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"totalHp");
			lua_pushnumber(tolua_S,info->total_hp);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"currentHp");
			lua_pushnumber(tolua_S,info->health_point);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"type");
			lua_pushnumber(tolua_S,info->type);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"battleSide");
			lua_pushnumber(tolua_S,info->battleSide);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"currentRagePoint");
			lua_pushnumber(tolua_S,info->ragePoint);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"totalRagePoint");
			lua_pushnumber(tolua_S,info->totalRangePoint);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"secCurrentRagePoint");
			lua_pushnumber(tolua_S,info->secRagePoint);
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S,"secTotalRagePoint");
			lua_pushnumber(tolua_S,info->secTotalRangePoint);
			lua_settable(tolua_S, -3);

			lua_settable(tolua_S, -3);
		}

	}

    
	return 1;
}

static int tolua_EQUIPINFO_getSuitEquip(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(!tolua_isusertype(tolua_S,1,"EQUIPINFO",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S, 2, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'EQUIPINFO::getSuitEquip'",&tolua_err);
		return 0;
	}
#endif

	EQUIPINFO* self = (EQUIPINFO*)tolua_tousertype(tolua_S,1,0);

	lua_newtable(tolua_S);
	for(std::vector<unsigned int>::iterator it = self->m_suitAttribution.begin();
		it != self->m_suitAttribution.end(); ++it)
	{
		lua_pushnumber(tolua_S,*it);
		lua_pushboolean(tolua_S,true);
		lua_settable(tolua_S, -3);
	}
	return 1;
}

static int tolua_EQUIPINFO_getEquipHole(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(!tolua_isusertype(tolua_S,1,"EQUIPINFO",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S, 2, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'EQUIPINFO::getEquipHole'",&tolua_err);
		return 0;
	}
#endif
	EQUIPINFO* self = (EQUIPINFO*)tolua_tousertype(tolua_S,1,0);

	lua_newtable(tolua_S);
	for(std::map<unsigned int,EQUIPHOLE>::iterator it = self->m_equipHoles.begin();
		it != self->m_equipHoles.end(); ++it)
	{
		unsigned int holePos = it->first;
		EQUIPHOLE hole = it->second;

		lua_pushnumber(tolua_S,hole.hole_index);
		lua_newtable(tolua_S);

		lua_pushstring(tolua_S,"hole_index");
		lua_pushnumber(tolua_S,hole.hole_index);
		lua_settable(tolua_S, -3);

		lua_pushstring(tolua_S,"item_gem_id");
		lua_pushnumber(tolua_S,hole.item_gem_id);
		lua_settable(tolua_S, -3);

		lua_pushstring(tolua_S,"attr_key");
		lua_pushnumber(tolua_S,hole.attr_key);
		lua_settable(tolua_S, -3);

		lua_pushstring(tolua_S,"attr_value");
		lua_pushnumber(tolua_S,hole.attr_value);
		lua_settable(tolua_S, -3);

		lua_pushstring(tolua_S,"max_value");
		lua_pushnumber(tolua_S,hole.max_value);
		lua_settable(tolua_S, -3);

		lua_pushstring(tolua_S,"min_value");
		lua_pushnumber(tolua_S,hole.min_value);
		lua_settable(tolua_S, -3);

		lua_settable(tolua_S, -3);
	}

	return 1;
	
}

static int tolua_ShowWaitingLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	bool bHasBooleanParam = false;
	bool bHasFloatParam = false;
	bool bShowRightNow = true;
	float fHideTimer = 0;

	tolua_Error tolua_err;
	if (tolua_isboolean(tolua_S, 1, 0, &tolua_err))
	{
		bHasBooleanParam = true;
		bShowRightNow = tolua_toboolean(tolua_S, 1, 0);
		if (tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			bHasFloatParam = true;
			fHideTimer = tolua_tonumber(tolua_S, 2, 0);
		}
	}
	
	int lastIndex = 1;
	if (bHasBooleanParam)
	{
		lastIndex ++;
		if (bHasFloatParam)
		{
			lastIndex++;
		}
	}
	if(!tolua_isnoobj(tolua_S, lastIndex, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'ShowWaitingLayer'",&tolua_err);
		return 0;
	}
#endif

	if (false == bHasBooleanParam)
	{
		ShowWaitingLayer();
	}
	else
	{
		if (false == bHasFloatParam)
		{
			ShowWaitingLayer(bShowRightNow);
		}
		else
		{
			ShowWaitingLayer(bShowRightNow,fHideTimer);
		}
	}

	return 0;
}

static int tolua_HideWaitingLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		//!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S, 1, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'HideWaitingLayer'",&tolua_err);
		return 0;
	}
#endif

	HideWaitingLayer();

	return 0;
}

static int tolua_PushOneLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S, 4, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'PushOneLayer'",&tolua_err);
		return 0;
	}
#endif

	CCLayer* layer = (CCLayer*)(tolua_tousertype(tolua_S, 1, NULL));
	std::string param_1 = tolua_tostring(tolua_S,2,NULL);
	std::string param_2 = tolua_tostring(tolua_S,3,NULL);

	MainMenuLayer::PushOneLayer(layer,param_1,param_2);

	return 0;
}

static int tolua_PushOneWaitingLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isstring(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S, 2, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'PushOneWaitingLayer'",&tolua_err);
		return 0;
	}
#endif
	std::string message = tolua_tostring(tolua_S,1,NULL);
	WaitingLayerManager::Get()->PushOneMessage(message.c_str());

	return 0;
}

static int tolua_RemoveOneWaitingLayerByForce(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isnoobj(tolua_S, 1, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'RemoveOneWaitingLayerByForce'",&tolua_err);
		return 0;
	}
#endif
	WaitingLayerManager::Get()->RemoveOneByForce();

	return 0;
}

static int tolua_SetLayerOneTouch(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S, 2, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'SetLayerOneTouch'",&tolua_err);
		return 0;
	}
#endif
	
	CCLayer* layer = (CCLayer*)(tolua_tousertype(tolua_S, 1, NULL));
	layer->setTouchMode(kCCTouchesOneByOne);

	return 0;
}

static int tolua_RemoveOneLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'RemoveOneLayer'",&tolua_err);
		return 0;
	}
#endif

	CCLayer* layer = (CCLayer*)(tolua_tousertype(tolua_S, 1, NULL));

	MainMenuLayer::RemoveOneLayer(layer);

	return 0;
}

static int tolua_StopHeroMove(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		//!tolua_isusertable(tolua_S,1,"GameApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S, 1, &tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'stopHeroMove'",&tolua_err);
		return 0;
	}
#endif

	GameUILayer * layer = GetGameUILayer();
	if (layer)
	{
		layer->stopHeroMove();
	}

	return 0;
}

/* method: Show of class  MessageBox */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MessageBox_Show00
static int tolua_LuaClient_MessageBox_Show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"MessageBox",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		((!tolua_isvaluenil(tolua_S,3,&tolua_err) && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err))) ||
		((!tolua_isvaluenil(tolua_S,4,&tolua_err) && !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err))) ||
		!tolua_isnumber(tolua_S,5,0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"ccColor3B",0,&tolua_err)) ||
		!tolua_isnumber(tolua_S,7,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* message_body = ((const char*)  tolua_tostring(tolua_S,2,0));
		LUA_FUNCTION pfnOkHandler = 0;
		if(!tolua_isvaluenil(tolua_S,3,&tolua_err))
		{
			pfnOkHandler = (  toluafix_ref_function(tolua_S,3,0)); 
		}
		LUA_FUNCTION pfnCancelHandler = 0;
		if(!tolua_isvaluenil(tolua_S,4,&tolua_err))
		{
			pfnCancelHandler = (  toluafix_ref_function(tolua_S,4,0)); 
		}
		unsigned int uType = ((unsigned int)  tolua_tonumber(tolua_S,5,0));
		ccColor3B labelColor = *((ccColor3B*)  tolua_tousertype(tolua_S,6,0));
		int fontSize = ((int)  tolua_tonumber(tolua_S,7,0));
		{
			MessageBox::Show(message_body,pfnOkHandler,pfnCancelHandler,uType,labelColor,fontSize);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Show'.",&tolua_err);
	return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Show of class  MessageBox */
#ifndef TOLUA_DISABLE_tolua_LuaGameApi_PopupRewardLayer_Show00
	static int tolua_LuaGameApi_PopupRewardLayer_Show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"PopupRewardLayer",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||	// message_body
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||	// rewardId
		!tolua_isstring(tolua_S,4,0,&tolua_err) ||	// okBtnContent
		!tolua_isboolean(tolua_S,5,0,&tolua_err) ||	// bShowCloseBtn
		!tolua_isboolean(tolua_S,6,0,&tolua_err) ||		// bModelLayer
		((!tolua_isvaluenil(tolua_S,7,&tolua_err) && !toluafix_isfunction(tolua_S,7,"LUA_FUNCTION",0,&tolua_err))) ||	// nOkBtnHandler
		((!tolua_isvaluenil(tolua_S,8,&tolua_err) && !toluafix_isfunction(tolua_S,8,"LUA_FUNCTION",0,&tolua_err))) ||	// nCloseBtnHandler
		!tolua_isnoobj(tolua_S,9,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* message_body = ((const char*)  tolua_tostring(tolua_S,2,0));
		LUA_FUNCTION pfnOkHandler = 0;
		if(!tolua_isvaluenil(tolua_S,7,&tolua_err))
		{
			pfnOkHandler = (  toluafix_ref_function(tolua_S,7,0)); 
		}
		LUA_FUNCTION pfnCloseHandler = 0;
		if(!tolua_isvaluenil(tolua_S,8,&tolua_err))
		{
			pfnCloseHandler = (  toluafix_ref_function(tolua_S,8,0)); 
		}

		int nRewardId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
		const char* szBtnContent = ((const char*)  tolua_tostring(tolua_S,4,0));
		bool bShowCloseBtn = tolua_toboolean(tolua_S,5,0);
		bool bShowModelLayer = tolua_toboolean(tolua_S,6,0);

		TXGUI::PopupRewardLayer::Show(message_body,nRewardId,szBtnContent,bShowCloseBtn,bShowModelLayer,pfnOkHandler,pfnCloseHandler);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Show'.",&tolua_err);
	return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

	/* method: Show of class  MessageBox */
#ifndef TOLUA_DISABLE_tolua_LuaGameApi_PopupRewardLayer_Show01
	static int tolua_LuaGameApi_PopupRewardLayer_Show01(lua_State* tolua_S)
	{
#ifndef TOLUA_RELEASE
		tolua_Error tolua_err;
		if (
			!tolua_isusertable(tolua_S,1,"PopupRewardLayer",0,&tolua_err) ||
			!tolua_isstring(tolua_S,2,0,&tolua_err) ||	// message_body
			!tolua_isstring(tolua_S,3,0,&tolua_err) ||	// rewardIconName
			!tolua_isstring(tolua_S,4,0,&tolua_err) ||	// okBtnContent
			!tolua_isboolean(tolua_S,5,0,&tolua_err) ||	// bShowCloseBtn
			!tolua_isboolean(tolua_S,6,0,&tolua_err) ||		// bModelLayer
			((!tolua_isvaluenil(tolua_S,7,&tolua_err) && !toluafix_isfunction(tolua_S,7,"LUA_FUNCTION",0,&tolua_err))) ||	// nOkBtnHandler
			((!tolua_isvaluenil(tolua_S,8,&tolua_err) && !toluafix_isfunction(tolua_S,8,"LUA_FUNCTION",0,&tolua_err))) ||	// nCloseBtnHandler
			!tolua_isnumber(tolua_S,9,0,&tolua_err) ||	// rewardId
			!tolua_isnoobj(tolua_S,10,&tolua_err)
			)
			goto tolua_lerror;
		else
#endif
		{
			const char* message_body = ((const char*)  tolua_tostring(tolua_S,2,0));
			LUA_FUNCTION pfnOkHandler = 0;
			if(!tolua_isvaluenil(tolua_S,7,&tolua_err))
			{
				pfnOkHandler = (  toluafix_ref_function(tolua_S,7,0)); 
			}
			LUA_FUNCTION pfnCloseHandler = 0;
			if(!tolua_isvaluenil(tolua_S,8,&tolua_err))
			{
				pfnCloseHandler = (  toluafix_ref_function(tolua_S,8,0)); 
			}

			const char* szRewardIconName = ((const char*)  tolua_tostring(tolua_S,3,0));
			const char* szBtnContent = ((const char*)  tolua_tostring(tolua_S,4,0));
			bool bShowCloseBtn = tolua_toboolean(tolua_S,5,0);
			bool bShowModelLayer = tolua_toboolean(tolua_S,6,0);

			float displayTime = tolua_tonumber(tolua_S,9,0);

			TXGUI::PopupRewardLayer::Show(message_body,szRewardIconName,szBtnContent,bShowCloseBtn,bShowModelLayer,pfnOkHandler,pfnCloseHandler,displayTime);
		}
		return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
		tolua_error(tolua_S,"#ferror in function 'Show'.",&tolua_err);
		return 0;
#endif
	}
#endif //#ifndef TOLUA_DISABLE

/* method: Show of class  MessageBox */
#ifndef TOLUA_DISABLE_tolua_LuaGameApi_DiamondWarningLayer_Show00
	static int tolua_LuaGameApi_DiamondWarningLayer_Show00(lua_State* tolua_S)
	{
#ifndef TOLUA_RELEASE
		tolua_Error tolua_err;
		if (
			!tolua_isusertable(tolua_S,1,"DiamondWarningLayer",0,&tolua_err) ||
			!tolua_isstring(tolua_S,2,0,&tolua_err) ||	// message_body
			!tolua_isnumber(tolua_S,3,0,&tolua_err) ||	// diamondNum
			((!tolua_isvaluenil(tolua_S,4,&tolua_err) && !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err))) ||	// nOkBtnHandler
			((!tolua_isvaluenil(tolua_S,5,&tolua_err) && !toluafix_isfunction(tolua_S,5,"LUA_FUNCTION",0,&tolua_err))) ||	// nCloseBtnHandler
			(tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"ccColor3B",0,&tolua_err)) 		   ||	// color
			!tolua_isstring(tolua_S,7,0,&tolua_err) ||	// tip content
			!tolua_isstring(tolua_S,8,0,&tolua_err) ||	// tip content 2
			!tolua_isnoobj(tolua_S,9,&tolua_err)
			)
			goto tolua_lerror;
		else
#endif
		{
			const char* tip_content = ((const char*)  tolua_tostring(tolua_S,7,0));
			const char* top_tip_content = ((const char*)  tolua_tostring(tolua_S,8,0));
			const char* message_body = ((const char*)  tolua_tostring(tolua_S,2,0));
			LUA_FUNCTION pfnOkHandler = 0;
			if(!tolua_isvaluenil(tolua_S,4,&tolua_err))
			{
				pfnOkHandler = (  toluafix_ref_function(tolua_S,4,0)); 
			}
			LUA_FUNCTION pfnCloseHandler = 0;
			if(!tolua_isvaluenil(tolua_S,5,&tolua_err))
			{
				pfnCloseHandler = (  toluafix_ref_function(tolua_S,5,0)); 
			}

			int nDiamondNum = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
			ccColor3B labelColor = *((ccColor3B*)  tolua_tousertype(tolua_S,6,0));

			DiamondWarningLayer::Show(message_body,nDiamondNum,pfnOkHandler,pfnCloseHandler,labelColor,tip_content,top_tip_content);
		}
		return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
		tolua_error(tolua_S,"#ferror in function 'Show'.",&tolua_err);
		return 0;
#endif
	}
#endif //#ifndef TOLUA_DISABLE

//////////////////////////////////////////////////////////////////////////

//init
TOLUA_API int tolua_GameApi_open(lua_State *tolua_S)
{
	tolua_open(tolua_S);
	tolua_reg_types(tolua_S);
	tolua_module(tolua_S, NULL, 0);

	tolua_beginmodule(tolua_S, NULL);
	/// MessageHandler
	tolua_cclass(tolua_S, "GameApi", "GameApi", "", NULL);
	tolua_beginmodule(tolua_S, "GameApi");
	tolua_function(tolua_S, "PPVELayer_InitMap", tolua_PPVELayer_InitMap);
	tolua_function(tolua_S, "GuildInstance_InitMap", tolua_GuildInstance_InitMap);
	tolua_function(tolua_S, "GetPPVEAutoChatCD", tolua_GetPPVEAutoChatCD);
	tolua_function(tolua_S, "GetLastStaminaRestoreTime", tolua_GetLastStaminaRestoreTime);
	tolua_function(tolua_S, "GetTimeBySec", tolua_getTimeBySec);
	
	tolua_function(tolua_S, "showMessage", tolua_ShowMessage);
	tolua_function(tolua_S, "testSystemTime", tolua_TestSystemTime);

	tolua_function(tolua_S, "InsertPPVEInstance", tolua_InsertPPVEInstance);
	tolua_function(tolua_S, "InsertGuildInstance", tolua_InsertGuildInstance);
	tolua_function(tolua_S, "getOtherPlayerInfo", tolua_GetOtherPlayerInfo);
	tolua_endmodule(tolua_S);

	tolua_function(tolua_S, "ShowWaitingLayer", tolua_ShowWaitingLayer);
	tolua_function(tolua_S, "HideWaitingLayer", tolua_HideWaitingLayer);

    tolua_function(tolua_S, "CreateSliderCtrl", tolua_CreateSliderCtrl);
	tolua_function(tolua_S, "PushOneLayer", tolua_PushOneLayer);
	tolua_function(tolua_S, "RemoveOneLayer", tolua_RemoveOneLayer);
	tolua_function(tolua_S, "PushOneWaitingLayer", tolua_PushOneWaitingLayer);
	tolua_function(tolua_S, "RemoveOneWaitingLayerByForce", tolua_RemoveOneWaitingLayerByForce);	

	tolua_function(tolua_S, "SetLayerOneTouch", tolua_SetLayerOneTouch);

	tolua_function(tolua_S, "StopHeroMove", tolua_StopHeroMove);

	tolua_cclass(tolua_S, "EQUIPINFO", "EQUIPINFO", "", NULL);
	tolua_beginmodule(tolua_S,"EQUIPINFO");
	tolua_function(tolua_S, "getEquipHole", tolua_EQUIPINFO_getEquipHole);
	tolua_function(tolua_S, "getSuitEquip", tolua_EQUIPINFO_getSuitEquip);
	tolua_endmodule(tolua_S);

	tolua_constant(tolua_S,"MB_OK",MB_OK);
	tolua_constant(tolua_S,"MB_OKCANCEL",MB_OKCANCEL);
	tolua_constant(tolua_S,"MB_ABORTRETRYIGNORE",MB_ABORTRETRYIGNORE);
	tolua_constant(tolua_S,"MB_YESNOCANCEL",MB_YESNOCANCEL);
	tolua_constant(tolua_S,"MB_YESNO",MB_YESNO);
	tolua_constant(tolua_S,"MB_RETRYCANCEL",MB_RETRYCANCEL);
	tolua_constant(tolua_S,"MB_OKCANCELCLOSE",MB_OKCANCELCLOSE);
	tolua_cclass(tolua_S,"MessageBox","MessageBox","",NULL);
	tolua_beginmodule(tolua_S,"MessageBox");
	tolua_function(tolua_S,"Show",tolua_LuaClient_MessageBox_Show00);
	tolua_endmodule(tolua_S);

	tolua_cclass(tolua_S,"PopupRewardLayer","PopupRewardLayer","",NULL);
	tolua_beginmodule(tolua_S,"PopupRewardLayer");
	tolua_function(tolua_S,"Show",tolua_LuaGameApi_PopupRewardLayer_Show00);
	tolua_function(tolua_S,"ShowByIconName",tolua_LuaGameApi_PopupRewardLayer_Show01);
	tolua_endmodule(tolua_S);

	tolua_cclass(tolua_S,"DiamondWarningLayer","DiamondWarningLayer","",NULL);
	tolua_beginmodule(tolua_S,"DiamondWarningLayer");
	tolua_function(tolua_S,"Show",tolua_LuaGameApi_DiamondWarningLayer_Show00);
	tolua_endmodule(tolua_S);
    
	tolua_endmodule(tolua_S);

	return 1;
}