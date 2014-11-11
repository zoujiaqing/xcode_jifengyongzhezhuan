
#ifndef __LUACLIENT_H_
#define __LUACLIENT_H_

extern "C" {
#include "tolua++.h"
#include "tolua_fix.h"
}
#include "UIControlsApearControler.h"
#include "ShopItemInfoFromServer.h"
#include <map>
#include <string>
#include "tolua_fix.h"
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"

//Note: Add Client .h Files Here
#include "SpriteFactory.h"
#include "SpriteElfDataCenter.h"
#include "LevelManager.h"
#include "UserData.h"
#include "GMessage.h"
#include "NotificationCenter.h"
#include "GameManager.h"
#include "SkillUIManager.h"
#include "ElfExploreManager.h"
#include "MainMenuLayer.h"
#include "GameAudioManager.h"
#include "particle_manager.h"
#include "ItemManager.h"
#include "TimeManager.h"
#include "GemStonesFromLua.h"
#include "PvAIManager.h"
#include "TutorialsManager.h"
#include "XLogicManager.h"
#include "InstanceManager.h"
#include "TipMgr.h"
#include "HeroRoleManager.h"
#include "CommonEnum.h"
#include "LoginManager.h"
#include "MenuItemExpand.h"
#include "FeedbackLayer.h"
#include "LevelLayer.h"
#include "HtmlNode.h"
#include "SpriteMonsterMgr.h"
#include "ParticleManager.h"
#include "BYGraySprite.h" 
#include "HSJoystick.h"
#include "GameDataManager.h"
#include "PayManager.h"
#include "SystemConditionManager.h"
#include "WaitingLayerManager.h"
#include "GameFontManager.h"
#include "CallNativeFuncManager.h"
#include "MessageBoxLayer.h"
#include "MonthCardManager.h"
#include "SkeletonAnimRcsManager.h"
#include "CityListLayer.h"
#include "DiamondWarningLayer.h"

using namespace cocos2d;
using namespace TXGUI;
using namespace CocosDenshion;

TOLUA_API int  tolua_LuaClient_open (lua_State* tolua_S);

#endif // __LUACLIENT_H_
