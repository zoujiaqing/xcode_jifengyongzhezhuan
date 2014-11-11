#ifndef LUA_GAMEAPI_H
#define LUA_GAMEAPI_H



#include "tolua++.h"

int  tolua_GameApi_open(lua_State* tolua_S);

extern long long millisecondNow();

#endif