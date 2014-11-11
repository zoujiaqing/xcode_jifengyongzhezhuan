
#ifndef __LUAENGINE_H_
#define __LUAENGINE_H_

extern "C" {
#include "tolua++.h"
#include "tolua_fix.h"
}

#include <map>
#include <string>
#include "tolua_fix.h"
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"

//Note: Add Client .h Files Here

#include "GameResourceManager.h"
#include "UILabelTTF.h"
//#include "IconButton.h"

using namespace cocos2d;
using namespace CocosDenshion;

TOLUA_API int  tolua_LuaEngine_open (lua_State* tolua_S);

#endif // __LUAENGINE_H_