#include "LuaNetwork.h"
#include "MessageHandler.h"
#include "MessageUtil.h"


#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "PPVEMessage.h"
#include "PVPMessage.h"

TOLUA_API int tolua_Network_open(lua_State *tolua_S);



//register classes
static void tolua_reg_types(lua_State *tolua_S)
{
	tolua_usertype(tolua_S, "MessageHandler");
	tolua_usertype(tolua_S, "WILD_MSG_PACKAGE");
}



//////////////////////////////////////////////////////////////////////////
//MessageHandler
//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_sShareInstance(lua_State *tolua_S)
{
#ifndef TOLUA_ISEER_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::sShareInstance'",&tolua_err);
		return 0;
	}
#endif
	MessageHandler *tolua_ret = MessageHandler::sShareInstance();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"MessageHandler");

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_GetCurWildMsgPackage(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::getCurWildMsgPackage'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	WILD_MSG_PACKAGE* tolua_ret = (WILD_MSG_PACKAGE*) self->getWildMsgPackage();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"WILD_MSG_PACKAGE");

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_GetPPVERooms(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::GetPPVERooms'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		/// send get room list request
		PPVEMessage::Get()->getRoomList();
	}
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_createRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::createRoom'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		/// send get room list request
		PPVEMessage::Get()->createRoom();
	}

	return 1;
}
//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_getRoomList(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S, "#ferror in function 'MessageHandler::getRoomList'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		/// send get room list request
		PPVEMessage::Get()->getRoomList();
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_getRoomData(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::getRoomList'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		lua_newtable(tolua_S);

		/// send get room list request
		vector<PVERoomBase>& arrRooms = PPVEMessage::Get()->getRoomData();
		for(size_t i = 0; i < arrRooms.size(); i++)
		{
			PVERoomBase & inf = arrRooms[i];

			lua_pushnumber(tolua_S, i);

			lua_newtable(tolua_S);
			lua_pushstring(tolua_S, "room_id");//压入subtable的key
			lua_pushnumber(tolua_S, inf.room_id());
			lua_settable(tolua_S,-3);//弹出key,value，并设置到table里面去
			lua_pushstring(tolua_S, "top_level");//压入subtable的key
			lua_pushnumber(tolua_S, inf.top_level());
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S, "current_online");//压入subtable的key
			lua_pushnumber(tolua_S, inf.current_online());
			lua_settable(tolua_S, -3);

			lua_pushstring(tolua_S, "max_online");//压入subtable的key
			lua_pushnumber(tolua_S, inf.max_online());
			lua_settable(tolua_S, -3);

			lua_settable(tolua_S, -3);
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_exitRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::exitRoom'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		/// send get room list request
		PPVEMessage::Get()->exitRoom();
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_setReady(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isboolean(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::setReady'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		bool bReady = tolua_toboolean(tolua_S, 2, 0) != 0;

		/// send get room list request
		PPVEMessage::Get()->setReady(bReady);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_joinRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::setReady'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		int room = tolua_tonumber(tolua_S, 2, 0);

		/// send get room list request
		PPVEMessage::Get()->JoinRoom(room);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_startGame(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::startGame'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		/// send get room list request
		PPVEMessage::Get()->startGame();
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_kickOffFromRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::kickOffFromRoom'",&tolua_err);
		return 0;
	}
#endif

	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);	
	if (self)
	{
		int seat = tolua_tonumber(tolua_S, 2, 0);
		PPVEMessage::Get()->kickOffGuest(seat);	
	}

	return 1;
}

//////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_createRoom_1vs1(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::createRoom'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		/// send get room list request
		PVPMessage::Get()->pvp_createRoom_1vs1();
	}
	return 1;
}

//////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_createRoom_2vs2(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::createRoom'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		/// send get room list request
		PVPMessage::Get()->pvp_createRoom_2vs2();
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_getRoomList(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::GetPPVERooms'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		/// send get room list request
		PVPMessage::Get()->pvp_getRoomList();
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_getRoomData(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::getRoomList'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		lua_newtable(tolua_S);
        
		/// send get room list request
		vector<PVPRoomBase>& arrRooms = PVPMessage::Get()->pvp_getRoomData();
		for(size_t i = 0; i < arrRooms.size(); i++)
		{
			PVPRoomBase & inf = arrRooms[i];
            
			lua_pushnumber(tolua_S, i);
            
			lua_newtable(tolua_S);
            
			lua_pushstring(tolua_S, "room_id");//压入subtable的key
			lua_pushnumber(tolua_S, inf.room_id());
            
			lua_settable(tolua_S,-3);//弹出key,value，并设置到table里面去
			lua_pushstring(tolua_S, "top_level");//压入subtable的key
			lua_pushnumber(tolua_S, inf.top_level());
            
			lua_settable(tolua_S, -3);
			lua_pushstring(tolua_S, "current_online");//压入subtable的key
			lua_pushnumber(tolua_S, inf.current_online());
            
			lua_settable(tolua_S, -3);
			lua_pushstring(tolua_S, "max_online");//压入subtable的key
			lua_pushnumber(tolua_S, inf.max_online());
			lua_settable(tolua_S, -3);
            
			lua_settable(tolua_S, -3);
		}
	}
    
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_exitRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::exitRoom'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		/// send get room list request
		PVPMessage::Get()->pvp_exitRoom();
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_setReady(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isboolean(tolua_S, 2, 0, &tolua_err) ||
       !tolua_isnoobj(tolua_S,3,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::setReady'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		bool bReady = tolua_toboolean(tolua_S, 2, 0) != 0;
		/// send get room list request
		PVPMessage::Get()->pvp_setReady(bReady);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_joinRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
       !tolua_isnoobj(tolua_S,3,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::setReady'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		int room = tolua_tonumber(tolua_S, 2, 0);
        
		/// send get room list request
		PVPMessage::Get()->pvp_JoinRoom(room);
	}
    
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_startGame(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::startGame'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		///send get room list request
		PVPMessage::Get()->pvp_startGame();
	}
    
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_kickOffFromRoom(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
       !tolua_isnoobj(tolua_S,3,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::kickOffFromRoom'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		int seat = tolua_tonumber(tolua_S, 2, 0);
		PVPMessage::Get()->pvp_kickOffGuest(seat);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_MessageHandler_pvp_changeSeat(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"MessageHandler",0,&tolua_err) ||
       !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
       !tolua_isnoobj(tolua_S,3,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'MessageHandler::kickOffFromRoom'",&tolua_err);
		return 0;
	}
#endif
    
	MessageHandler* self = (MessageHandler*)tolua_tousertype(tolua_S,1,0);
	if (self)
	{
		int seat = tolua_tonumber(tolua_S, 2, 0);
		PVPMessage::Get()->pvp_changeSeat(seat);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
static int tolua_WildMsg_GetCMD(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_WildMsg_GetCMD'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
	string tolua_ret = self->header.cmd;
	tolua_pushstring(tolua_S, tolua_ret.c_str());

	return 1;
}

/// decode WildMessageBody
static int tolua_ReadBody_Int32(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_ReadBody_Int32'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
	int tolua_ret = self->ReadInt32();
	tolua_pushnumber(tolua_S, tolua_ret);

	return 1;
}

static int tolua_ReadBody_Short16(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_ReadBody_Short16'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
	int tolua_ret = self->ReadInt16();
	tolua_pushnumber(tolua_S, tolua_ret);

	return 1;
}

static int tolua_ReadBody_Number32(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_ReadBody_Number32'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
	double tolua_ret = self->ReadNumber32();
	tolua_pushnumber(tolua_S, tolua_ret);

	return 1;
}

static int tolua_ReadBody_String(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_WildMsg_GetCMD'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
	string tolua_ret = self->ReadString();
	tolua_pushstring(tolua_S, tolua_ret.c_str());

	return 1;
}

static int tolua_ReadBody_Char(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_WildMsg_GetCMD'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
	string tolua_ret = self->ReadChar();
	tolua_pushstring(tolua_S, tolua_ret.c_str());

	return 1;
}

static int tolua_ReadBody_readbuff(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"WILD_MSG_PACKAGE",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'WILD_MSG_PACKAGE::tolua_WildMsg_GetCMD'",&tolua_err);
		return 0;
	}
#endif

	WILD_MSG_PACKAGE* self = (WILD_MSG_PACKAGE*)tolua_tousertype(tolua_S,1,0);	
  
	lua_pushlstring(tolua_S, (const char *)(self->buffer ), self->length);
	return 1;
}

//////////////////////////////////////////////////////////////////////////

//init
TOLUA_API int tolua_Network_open(lua_State *tolua_S)
{
	tolua_open(tolua_S);
	tolua_reg_types(tolua_S);
	tolua_module(tolua_S, NULL, 0);

	tolua_beginmodule(tolua_S, NULL);
	    /// MessageHandler
		tolua_cclass(tolua_S, "MessageHandler", "MessageHandler", "", NULL);
		tolua_beginmodule(tolua_S, "MessageHandler");
			tolua_function(tolua_S, "sShareInstance", tolua_MessageHandler_sShareInstance);
			tolua_function(tolua_S, "getWildMsgPackage", tolua_MessageHandler_GetCurWildMsgPackage);
    
			tolua_function(tolua_S, "getPPVERooms", tolua_MessageHandler_GetPPVERooms);
    
			tolua_function(tolua_S, "createRoom", tolua_MessageHandler_createRoom);	
			tolua_function(tolua_S, "getRoomList", tolua_MessageHandler_getRoomList);
			tolua_function(tolua_S, "getRoomData", tolua_MessageHandler_getRoomData);
			tolua_function(tolua_S, "exitRoom", tolua_MessageHandler_exitRoom);
			tolua_function(tolua_S, "setReady", tolua_MessageHandler_setReady);
			tolua_function(tolua_S, "joinRoom", tolua_MessageHandler_joinRoom);
			tolua_function(tolua_S, "startGame", tolua_MessageHandler_startGame);
            tolua_function(tolua_S, "kickOffFromRoom", tolua_MessageHandler_kickOffFromRoom);
        
            tolua_function(tolua_S, "pvp_createRoom_1vs1", tolua_MessageHandler_pvp_createRoom_1vs1);
            tolua_function(tolua_S, "pvp_createRoom_2vs2", tolua_MessageHandler_pvp_createRoom_2vs2);
            tolua_function(tolua_S, "pvp_getRoomList",     tolua_MessageHandler_pvp_getRoomList);
            tolua_function(tolua_S, "pvp_getRoomData",     tolua_MessageHandler_pvp_getRoomData);
            tolua_function(tolua_S, "pvp_exitRoom",        tolua_MessageHandler_pvp_exitRoom);
            tolua_function(tolua_S, "pvp_setReady",        tolua_MessageHandler_pvp_setReady);
            tolua_function(tolua_S, "pvp_joinRoom",        tolua_MessageHandler_pvp_joinRoom);
            tolua_function(tolua_S, "pvp_startGame",       tolua_MessageHandler_pvp_startGame);
            tolua_function(tolua_S, "pvp_kickOffFromRoom", tolua_MessageHandler_pvp_kickOffFromRoom);
            tolua_function(tolua_S, "pvp_changeSeat",      tolua_MessageHandler_pvp_changeSeat);

    
		tolua_endmodule(tolua_S);

		/// WILD_MSG_PACKAGE decode operation
		tolua_cclass(tolua_S, "WILD_MSG_PACKAGE", "WILD_MSG_PACKAGE", "", NULL);
		tolua_beginmodule(tolua_S, "WILD_MSG_PACKAGE");
		    tolua_function(tolua_S, "GetCmd", tolua_WildMsg_GetCMD);
			tolua_function(tolua_S, "ReadInt32", tolua_ReadBody_Int32);
			tolua_function(tolua_S, "ReadShort16", tolua_ReadBody_Short16);
			tolua_function(tolua_S, "ReadNumber32", tolua_ReadBody_Number32);
			tolua_function(tolua_S, "ReadString", tolua_ReadBody_String);
			tolua_function(tolua_S, "ReadChar", tolua_ReadBody_Char);
            tolua_function(tolua_S, "ReadBuff", tolua_ReadBody_readbuff);
		tolua_endmodule(tolua_S);

		///WILD_MSG_PACKAGE encode operation
    
	tolua_endmodule(tolua_S);

	return 1;
}
