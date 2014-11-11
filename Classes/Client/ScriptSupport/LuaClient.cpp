/*
** Lua binding: LuaClient
** Generated automatically by tolua++-1.0.92 on 10/29/14 15:19:27.
*/

/****************************************************************************
 Copyright (c) 2011 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

extern "C" {
#include "tolua_fix.h"
}

#include "OnlineNetworkManager.h"
#include <map>
#include <string>
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

/* Exported function */
TOLUA_API int  tolua_LuaClient_open (lua_State* tolua_S);

#include "LuaCocos2d.h"
#include "LuaClient.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CCPoint (lua_State* tolua_S)
{
 CCPoint* self = (CCPoint*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_GUILD_ID (lua_State* tolua_S)
{
 GUILD_ID* self = (GUILD_ID*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MonthCardInfo (lua_State* tolua_S)
{
 MonthCardInfo* self = (MonthCardInfo*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCClippingNode (lua_State* tolua_S)
{
 CCClippingNode* self = (CCClippingNode*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_GLfloat (lua_State* tolua_S)
{
 GLfloat* self = (GLfloat*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_OneShopItemInfo (lua_State* tolua_S)
{
 OneShopItemInfo* self = (OneShopItemInfo*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCRect (lua_State* tolua_S)
{
 CCRect* self = (CCRect*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_ElfExploreManger (lua_State* tolua_S)
{
 ElfExploreManger* self = (ElfExploreManger*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_USERINFO (lua_State* tolua_S)
{
 USERINFO* self = (USERINFO*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_ccColor3B (lua_State* tolua_S)
{
 ccColor3B* self = (ccColor3B*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCSize (lua_State* tolua_S)
{
 CCSize* self = (CCSize*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_SpriteElfDataCenter (lua_State* tolua_S)
{
 SpriteElfDataCenter* self = (SpriteElfDataCenter*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_PLAYER_ID (lua_State* tolua_S)
{
 PLAYER_ID* self = (PLAYER_ID*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_IconButton (lua_State* tolua_S)
{
 IconButton* self = (IconButton*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
#ifndef Mtolua_typeid
#define Mtolua_typeid(L,TI,T)
#endif
 tolua_usertype(tolua_S,"UILabelTTF");
 Mtolua_typeid(tolua_S,typeid(UILabelTTF), "UILabelTTF");
 tolua_usertype(tolua_S,"BackPackItem");
 Mtolua_typeid(tolua_S,typeid(BackPackItem), "BackPackItem");
 tolua_usertype(tolua_S,"GUILD_ID");
 Mtolua_typeid(tolua_S,typeid(GUILD_ID), "GUILD_ID");
 tolua_usertype(tolua_S,"DragReceiver");
 Mtolua_typeid(tolua_S,typeid(DragReceiver), "DragReceiver");
 tolua_usertype(tolua_S,"ElfExploreManger");
 Mtolua_typeid(tolua_S,typeid(ElfExploreManger), "ElfExploreManger");
 tolua_usertype(tolua_S,"FeedbackLayer");
 Mtolua_typeid(tolua_S,typeid(FeedbackLayer), "FeedbackLayer");
 tolua_usertype(tolua_S,"SkeletonAnimRcsManager");
 Mtolua_typeid(tolua_S,typeid(SkeletonAnimRcsManager), "SkeletonAnimRcsManager");
 tolua_usertype(tolua_S,"CCNode");
 Mtolua_typeid(tolua_S,typeid(CCNode), "CCNode");
 tolua_usertype(tolua_S,"LevelLayer");
 Mtolua_typeid(tolua_S,typeid(LevelLayer), "LevelLayer");
 tolua_usertype(tolua_S,"MonthCardManager");
 Mtolua_typeid(tolua_S,typeid(MonthCardManager), "MonthCardManager");
 tolua_usertype(tolua_S,"MonsterInitInfo");
 Mtolua_typeid(tolua_S,typeid(MonsterInitInfo), "MonsterInitInfo");
 tolua_usertype(tolua_S,"InstanceInfo");
 Mtolua_typeid(tolua_S,typeid(InstanceInfo), "InstanceInfo");
 tolua_usertype(tolua_S,"WaitingLayerManager");
 Mtolua_typeid(tolua_S,typeid(WaitingLayerManager), "WaitingLayerManager");
 tolua_usertype(tolua_S,"USERINFO");
 Mtolua_typeid(tolua_S,typeid(USERINFO), "USERINFO");
 tolua_usertype(tolua_S,"ccColor3B");
 Mtolua_typeid(tolua_S,typeid(ccColor3B), "ccColor3B");
 tolua_usertype(tolua_S,"SpriteElfDataCenter");
 Mtolua_typeid(tolua_S,typeid(SpriteElfDataCenter), "SpriteElfDataCenter");
 tolua_usertype(tolua_S,"GameDataManager");
 Mtolua_typeid(tolua_S,typeid(GameDataManager), "GameDataManager");
 tolua_usertype(tolua_S,"UIControlBase");
 Mtolua_typeid(tolua_S,typeid(UIControlBase), "UIControlBase");
 tolua_usertype(tolua_S,"MenuItemExpand");
 Mtolua_typeid(tolua_S,typeid(MenuItemExpand), "MenuItemExpand");
 tolua_usertype(tolua_S,"SpriteElf");
 Mtolua_typeid(tolua_S,typeid(SpriteElf), "SpriteElf");
 tolua_usertype(tolua_S,"XLogicManager");
 Mtolua_typeid(tolua_S,typeid(XLogicManager), "XLogicManager");
 tolua_usertype(tolua_S,"CCClippingNode");
 Mtolua_typeid(tolua_S,typeid(CCClippingNode), "CCClippingNode");
 tolua_usertype(tolua_S,"TimeManager");
 Mtolua_typeid(tolua_S,typeid(TimeManager), "TimeManager");
 tolua_usertype(tolua_S,"NotificationCenter");
 Mtolua_typeid(tolua_S,typeid(NotificationCenter), "NotificationCenter");
 tolua_usertype(tolua_S,"PVPLayer");
 Mtolua_typeid(tolua_S,typeid(PVPLayer), "PVPLayer");
 tolua_usertype(tolua_S,"GameAudioManager");
 Mtolua_typeid(tolua_S,typeid(GameAudioManager), "GameAudioManager");
 tolua_usertype(tolua_S,"sLevelData");
 Mtolua_typeid(tolua_S,typeid(sLevelData), "sLevelData");
 tolua_usertype(tolua_S,"GameManager");
 Mtolua_typeid(tolua_S,typeid(GameManager), "GameManager");
 tolua_usertype(tolua_S,"PayManager");
 Mtolua_typeid(tolua_S,typeid(PayManager), "PayManager");
 tolua_usertype(tolua_S,"ItemBase");
 Mtolua_typeid(tolua_S,typeid(ItemBase), "ItemBase");
 tolua_usertype(tolua_S,"CCSprite");
 Mtolua_typeid(tolua_S,typeid(CCSprite), "CCSprite");
 tolua_usertype(tolua_S,"InstanceListLayer");
 Mtolua_typeid(tolua_S,typeid(InstanceListLayer), "InstanceListLayer");
 tolua_usertype(tolua_S,"PPVELayer");
 Mtolua_typeid(tolua_S,typeid(PPVELayer), "PPVELayer");
 tolua_usertype(tolua_S,"CCPoint");
 Mtolua_typeid(tolua_S,typeid(CCPoint), "CCPoint");
 tolua_usertype(tolua_S,"CityListLayer");
 Mtolua_typeid(tolua_S,typeid(CityListLayer), "CityListLayer");
 tolua_usertype(tolua_S,"CCLayer");
 Mtolua_typeid(tolua_S,typeid(CCLayer), "CCLayer");
 tolua_usertype(tolua_S,"CCTouch");
 Mtolua_typeid(tolua_S,typeid(CCTouch), "CCTouch");
 tolua_usertype(tolua_S,"DiamondWarningLayer");
 Mtolua_typeid(tolua_S,typeid(DiamondWarningLayer), "DiamondWarningLayer");
 tolua_usertype(tolua_S,"CCMenu");
 Mtolua_typeid(tolua_S,typeid(CCMenu), "CCMenu");
 tolua_usertype(tolua_S,"GLfloat");
 Mtolua_typeid(tolua_S,typeid(GLfloat), "GLfloat");
 tolua_usertype(tolua_S,"ItemManager");
 Mtolua_typeid(tolua_S,typeid(ItemManager), "ItemManager");
 tolua_usertype(tolua_S,"OnlineNetworkManager");
 Mtolua_typeid(tolua_S,typeid(OnlineNetworkManager), "OnlineNetworkManager");
 tolua_usertype(tolua_S,"MessageBox");
 Mtolua_typeid(tolua_S,typeid(MessageBox), "MessageBox");
 tolua_usertype(tolua_S,"CallNativeFuncManager");
 Mtolua_typeid(tolua_S,typeid(CallNativeFuncManager), "CallNativeFuncManager");
 tolua_usertype(tolua_S,"ParticleManager");
 Mtolua_typeid(tolua_S,typeid(ParticleManager), "ParticleManager");
 tolua_usertype(tolua_S,"GameFontManager");
 Mtolua_typeid(tolua_S,typeid(GameFontManager), "GameFontManager");
 tolua_usertype(tolua_S,"PLAYER_ID");
 Mtolua_typeid(tolua_S,typeid(PLAYER_ID), "PLAYER_ID");
 tolua_usertype(tolua_S,"SystemConditionManager");
 Mtolua_typeid(tolua_S,typeid(SystemConditionManager), "SystemConditionManager");
 tolua_usertype(tolua_S,"ShopItemInfoFromServer");
 Mtolua_typeid(tolua_S,typeid(ShopItemInfoFromServer), "ShopItemInfoFromServer");
 tolua_usertype(tolua_S,"SkillUIManager");
 Mtolua_typeid(tolua_S,typeid(SkillUIManager), "SkillUIManager");
 tolua_usertype(tolua_S,"OneShopItemInfo");
 Mtolua_typeid(tolua_S,typeid(OneShopItemInfo), "OneShopItemInfo");
 tolua_usertype(tolua_S,"CCEvent");
 Mtolua_typeid(tolua_S,typeid(CCEvent), "CCEvent");
 tolua_usertype(tolua_S,"PvAIManager");
 Mtolua_typeid(tolua_S,typeid(PvAIManager), "PvAIManager");
 tolua_usertype(tolua_S,"HSJoystick");
 Mtolua_typeid(tolua_S,typeid(HSJoystick), "HSJoystick");
 tolua_usertype(tolua_S,"SpriteMonsterMgr");
 Mtolua_typeid(tolua_S,typeid(SpriteMonsterMgr), "SpriteMonsterMgr");
 tolua_usertype(tolua_S,"CCScene");
 Mtolua_typeid(tolua_S,typeid(CCScene), "CCScene");
 tolua_usertype(tolua_S,"CPhysicsLayer");
 Mtolua_typeid(tolua_S,typeid(CPhysicsLayer), "CPhysicsLayer");
 tolua_usertype(tolua_S,"InstanceManager");
 Mtolua_typeid(tolua_S,typeid(InstanceManager), "InstanceManager");
 tolua_usertype(tolua_S,"MonthCardInfo");
 Mtolua_typeid(tolua_S,typeid(MonthCardInfo), "MonthCardInfo");
 tolua_usertype(tolua_S,"LevelManager");
 Mtolua_typeid(tolua_S,typeid(LevelManager), "LevelManager");
 tolua_usertype(tolua_S,"HtmlNode");
 Mtolua_typeid(tolua_S,typeid(HtmlNode), "HtmlNode");
 
 Mtolua_typeid(tolua_S,typeid(LUA_FUNCTION), "LUA_FUNCTION");
 tolua_usertype(tolua_S,"CCObject");
 Mtolua_typeid(tolua_S,typeid(CCObject), "CCObject");
 tolua_usertype(tolua_S,"SpriteTransPoint");
 Mtolua_typeid(tolua_S,typeid(SpriteTransPoint), "SpriteTransPoint");
 tolua_usertype(tolua_S,"SpriteFactory");
 Mtolua_typeid(tolua_S,typeid(SpriteFactory), "SpriteFactory");
 tolua_usertype(tolua_S,"cocos2d::CCPoint");
 Mtolua_typeid(tolua_S,typeid(cocos2d::CCPoint), "cocos2d::CCPoint");
 tolua_usertype(tolua_S,"SPRITEINFO");
 Mtolua_typeid(tolua_S,typeid(SPRITEINFO), "SPRITEINFO");
 tolua_usertype(tolua_S,"CCRect");
 Mtolua_typeid(tolua_S,typeid(CCRect), "CCRect");
 tolua_usertype(tolua_S,"TutorialsManager");
 Mtolua_typeid(tolua_S,typeid(TutorialsManager), "TutorialsManager");
 tolua_usertype(tolua_S,"TipMgr");
 Mtolua_typeid(tolua_S,typeid(TipMgr), "TipMgr");
 tolua_usertype(tolua_S,"HeroRoleManager");
 Mtolua_typeid(tolua_S,typeid(HeroRoleManager), "HeroRoleManager");
 tolua_usertype(tolua_S,"sActiveLevelConfig");
 Mtolua_typeid(tolua_S,typeid(sActiveLevelConfig), "sActiveLevelConfig");
 tolua_usertype(tolua_S,"BaseListener");
 Mtolua_typeid(tolua_S,typeid(BaseListener), "BaseListener");
 tolua_usertype(tolua_S,"cocos2d::CCLayer");
 Mtolua_typeid(tolua_S,typeid(cocos2d::CCLayer), "cocos2d::CCLayer");
 tolua_usertype(tolua_S,"MANORBASEINFO");
 Mtolua_typeid(tolua_S,typeid(MANORBASEINFO), "MANORBASEINFO");
 tolua_usertype(tolua_S,"CCSize");
 Mtolua_typeid(tolua_S,typeid(CCSize), "CCSize");
 tolua_usertype(tolua_S,"ParticleManagerX");
 Mtolua_typeid(tolua_S,typeid(ParticleManagerX), "ParticleManagerX");
 tolua_usertype(tolua_S,"SEL_MenuHandler");
 Mtolua_typeid(tolua_S,typeid(SEL_MenuHandler), "SEL_MenuHandler");
 tolua_usertype(tolua_S,"UIControlAppearControler");
 Mtolua_typeid(tolua_S,typeid(UIControlAppearControler), "UIControlAppearControler");
 tolua_usertype(tolua_S,"LoginManager");
 Mtolua_typeid(tolua_S,typeid(LoginManager), "LoginManager");
 tolua_usertype(tolua_S,"IconButton");
 Mtolua_typeid(tolua_S,typeid(IconButton), "IconButton");
 tolua_usertype(tolua_S,"UserData");
 Mtolua_typeid(tolua_S,typeid(UserData), "UserData");
 tolua_usertype(tolua_S,"MainMenuLayer");
 Mtolua_typeid(tolua_S,typeid(MainMenuLayer), "MainMenuLayer");
 tolua_usertype(tolua_S,"EffectSprite");
 Mtolua_typeid(tolua_S,typeid(EffectSprite), "EffectSprite");
 tolua_usertype(tolua_S,"RoleBase");
 Mtolua_typeid(tolua_S,typeid(RoleBase), "RoleBase");
 tolua_usertype(tolua_S,"SceneLayer");
 Mtolua_typeid(tolua_S,typeid(SceneLayer), "SceneLayer");
 tolua_usertype(tolua_S,"CCArray");
 Mtolua_typeid(tolua_S,typeid(CCArray), "CCArray");
 tolua_usertype(tolua_S,"EQUIPINFO");
 Mtolua_typeid(tolua_S,typeid(EQUIPINFO), "EQUIPINFO");
 tolua_usertype(tolua_S,"CCMenuItem");
 Mtolua_typeid(tolua_S,typeid(CCMenuItem), "CCMenuItem");
 tolua_usertype(tolua_S,"va_list");
 Mtolua_typeid(tolua_S,typeid(va_list), "va_list");
 tolua_usertype(tolua_S,"UIMenu");
 Mtolua_typeid(tolua_S,typeid(UIMenu), "UIMenu");
 tolua_usertype(tolua_S,"ITEMATTR");
 Mtolua_typeid(tolua_S,typeid(ITEMATTR), "ITEMATTR");
 tolua_usertype(tolua_S,"GemStonesFromLua");
 Mtolua_typeid(tolua_S,typeid(GemStonesFromLua), "GemStonesFromLua");
 tolua_usertype(tolua_S,"CCMenuItemSprite");
 Mtolua_typeid(tolua_S,typeid(CCMenuItemSprite), "CCMenuItemSprite");
 tolua_usertype(tolua_S,"EquipmentItem");
 Mtolua_typeid(tolua_S,typeid(EquipmentItem), "EquipmentItem");
 tolua_usertype(tolua_S,"cocos2d::CCSprite");
 Mtolua_typeid(tolua_S,typeid(cocos2d::CCSprite), "cocos2d::CCSprite");
 tolua_usertype(tolua_S,"SpriteSeer");
 Mtolua_typeid(tolua_S,typeid(SpriteSeer), "SpriteSeer");
}

/* method: create of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_create00
static int tolua_LuaClient_CCClippingNode_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCClippingNode* tolua_ret = (CCClippingNode*)  CCClippingNode::create();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCClippingNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_create01
static int tolua_LuaClient_CCClippingNode_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCNode* pStencil = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  {
   CCClippingNode* tolua_ret = (CCClippingNode*)  CCClippingNode::create(pStencil);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCClippingNode");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_CCClippingNode_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_delete00
static int tolua_LuaClient_CCClippingNode_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_init00
static int tolua_LuaClient_CCClippingNode_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_init01
static int tolua_LuaClient_CCClippingNode_init01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
  CCNode* pStencil = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(pStencil);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_CCClippingNode_init00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: onEnter of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_onEnter00
static int tolua_LuaClient_CCClippingNode_onEnter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onEnter'", NULL);
#endif
  {
   self->onEnter();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onEnter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onEnterTransitionDidFinish of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_onEnterTransitionDidFinish00
static int tolua_LuaClient_CCClippingNode_onEnterTransitionDidFinish00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onEnterTransitionDidFinish'", NULL);
#endif
  {
   self->onEnterTransitionDidFinish();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onEnterTransitionDidFinish'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onExitTransitionDidStart of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_onExitTransitionDidStart00
static int tolua_LuaClient_CCClippingNode_onExitTransitionDidStart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onExitTransitionDidStart'", NULL);
#endif
  {
   self->onExitTransitionDidStart();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onExitTransitionDidStart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onExit of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_onExit00
static int tolua_LuaClient_CCClippingNode_onExit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onExit'", NULL);
#endif
  {
   self->onExit();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onExit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: visit of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_visit00
static int tolua_LuaClient_CCClippingNode_visit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'visit'", NULL);
#endif
  {
   self->visit();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'visit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStencil of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_getStencil00
static int tolua_LuaClient_CCClippingNode_getStencil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CCClippingNode* self = (const CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStencil'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getStencil();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStencil'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setStencil of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_setStencil00
static int tolua_LuaClient_CCClippingNode_setStencil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
  CCNode* pStencil = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setStencil'", NULL);
#endif
  {
   self->setStencil(pStencil);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setStencil'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAlphaThreshold of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_getAlphaThreshold00
static int tolua_LuaClient_CCClippingNode_getAlphaThreshold00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CCClippingNode* self = (const CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlphaThreshold'", NULL);
#endif
  {
   GLfloat tolua_ret = (GLfloat)  self->getAlphaThreshold();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((GLfloat)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"GLfloat");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(GLfloat));
     tolua_pushusertype(tolua_S,tolua_obj,"GLfloat");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlphaThreshold'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAlphaThreshold of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_setAlphaThreshold00
static int tolua_LuaClient_CCClippingNode_setAlphaThreshold00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
  float fAlphaThreshold = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlphaThreshold'", NULL);
#endif
  {
   self->setAlphaThreshold(fAlphaThreshold);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlphaThreshold'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInverted of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_isInverted00
static int tolua_LuaClient_CCClippingNode_isInverted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CCClippingNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CCClippingNode* self = (const CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInverted'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInverted();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInverted'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInverted of class  CCClippingNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CCClippingNode_setInverted00
static int tolua_LuaClient_CCClippingNode_setInverted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
  bool bInverted = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInverted'", NULL);
#endif
  {
   self->setInverted(bInverted);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInverted'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PrintLog of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_PrintLog00
static int tolua_LuaClient_SpriteFactory_PrintLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PrintLog'", NULL);
#endif
  {
   self->PrintLog();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PrintLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedFactory of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_sharedFactory00
static int tolua_LuaClient_SpriteFactory_sharedFactory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SpriteFactory* tolua_ret = (SpriteFactory*)  SpriteFactory::sharedFactory();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteFactory");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedFactory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValue of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_GetValue00
static int tolua_LuaClient_SpriteFactory_GetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowElfOnLayerAtPoint of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_ShowElfOnLayerAtPoint00
static int tolua_LuaClient_SpriteFactory_ShowElfOnLayerAtPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
  CCNode* parentNode = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  int type = ((int)  tolua_tonumber(tolua_S,3,0));
  CCPoint pos = *((CCPoint*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowElfOnLayerAtPoint'", NULL);
#endif
  {
   self->ShowElfOnLayerAtPoint(parentNode,type,pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowElfOnLayerAtPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveElfFromHero of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_RemoveElfFromHero00
static int tolua_LuaClient_SpriteFactory_RemoveElfFromHero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID roleId = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveElfFromHero'", NULL);
#endif
  {
   self->RemoveElfFromHero(roleId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveElfFromHero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AttachElfToOneHero of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_AttachElfToOneHero00
static int tolua_LuaClient_SpriteFactory_AttachElfToOneHero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
  unsigned int elfTypeId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  PLAYER_ID heroId = *((PLAYER_ID*)  tolua_tousertype(tolua_S,3,0));
  unsigned int grade = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AttachElfToOneHero'", NULL);
#endif
  {
   self->AttachElfToOneHero(elfTypeId,heroId,grade);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AttachElfToOneHero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMainMenuLayerNode of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_GetMainMenuLayerNode00
static int tolua_LuaClient_SpriteFactory_GetMainMenuLayerNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMainMenuLayerNode'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->GetMainMenuLayerNode();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMainMenuLayerNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetValue of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_ResetValue00
static int tolua_LuaClient_SpriteFactory_ResetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetValue'", NULL);
#endif
  {
   self->ResetValue();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowElfPanelWithBackPackLayer of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_ShowElfPanelWithBackPackLayer00
static int tolua_LuaClient_SpriteFactory_ShowElfPanelWithBackPackLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowElfPanelWithBackPackLayer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ShowElfPanelWithBackPackLayer();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowElfPanelWithBackPackLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendElfAwakeMessage of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_SendElfAwakeMessage00
static int tolua_LuaClient_SpriteFactory_SendElfAwakeMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
  unsigned int awakeType = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendElfAwakeMessage'", NULL);
#endif
  {
   self->SendElfAwakeMessage(awakeType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendElfAwakeMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendAwakeConfigMessage of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_SendAwakeConfigMessage00
static int tolua_LuaClient_SpriteFactory_SendAwakeConfigMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendAwakeConfigMessage'", NULL);
#endif
  {
   self->SendAwakeConfigMessage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendAwakeConfigMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveElfFromParent of class  SpriteFactory */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteFactory_RemoveElfFromParent00
static int tolua_LuaClient_SpriteFactory_RemoveElfFromParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteFactory* self = (SpriteFactory*)  tolua_tousertype(tolua_S,1,0);
  CCNode* parentNode = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveElfFromParent'", NULL);
#endif
  {
   self->RemoveElfFromParent(parentNode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveElfFromParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: graylightWithCCSprite */
#ifndef TOLUA_DISABLE_tolua_LuaClient_graylightWithCCSprite00
static int tolua_LuaClient_graylightWithCCSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCSprite",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* oldSprite = ((CCSprite*)  tolua_tousertype(tolua_S,1,0));
  bool isLight = ((bool)  tolua_toboolean(tolua_S,2,0));
  {
   CCSprite* tolua_ret = (CCSprite*)  graylightWithCCSprite(oldSprite,isLight);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'graylightWithCCSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_new00
static int tolua_LuaClient_SpriteElfDataCenter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SpriteElfDataCenter* tolua_ret = (SpriteElfDataCenter*)  Mtolua_new((SpriteElfDataCenter)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteElfDataCenter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_new00_local
static int tolua_LuaClient_SpriteElfDataCenter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SpriteElfDataCenter* tolua_ret = (SpriteElfDataCenter*)  Mtolua_new((SpriteElfDataCenter)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteElfDataCenter");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_delete00
static int tolua_LuaClient_SpriteElfDataCenter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteElfDataCenter* self = (SpriteElfDataCenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_Get00
static int tolua_LuaClient_SpriteElfDataCenter_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SpriteElfDataCenter* tolua_ret = (SpriteElfDataCenter*)  SpriteElfDataCenter::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteElfDataCenter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Destroy of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_Destroy00
static int tolua_LuaClient_SpriteElfDataCenter_Destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SpriteElfDataCenter::Destroy();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EquipOneElf of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_EquipOneElf00
static int tolua_LuaClient_SpriteElfDataCenter_EquipOneElf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteElfDataCenter* self = (SpriteElfDataCenter*)  tolua_tousertype(tolua_S,1,0);
  unsigned int elfId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EquipOneElf'", NULL);
#endif
  {
   self->EquipOneElf(elfId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EquipOneElf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendGetElfListMessage of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_SendGetElfListMessage00
static int tolua_LuaClient_SpriteElfDataCenter_SendGetElfListMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteElfDataCenter* self = (SpriteElfDataCenter*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID roleId = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendGetElfListMessage'", NULL);
#endif
  {
   self->SendGetElfListMessage(roleId,name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendGetElfListMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendSetElfStateMessage of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_SendSetElfStateMessage00
static int tolua_LuaClient_SpriteElfDataCenter_SendSetElfStateMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteElfDataCenter* self = (SpriteElfDataCenter*)  tolua_tousertype(tolua_S,1,0);
  unsigned int elfId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  int state = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendSetElfStateMessage'", NULL);
#endif
  {
   self->SendSetElfStateMessage(elfId,state);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendSetElfStateMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeroUserId of class  SpriteElfDataCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteElfDataCenter_GetHeroUserId00
static int tolua_LuaClient_SpriteElfDataCenter_GetHeroUserId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteElfDataCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteElfDataCenter* self = (SpriteElfDataCenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeroUserId'", NULL);
#endif
  {
   PLAYER_ID tolua_ret = (PLAYER_ID)  self->GetHeroUserId();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((PLAYER_ID)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(PLAYER_ID));
     tolua_pushusertype(tolua_S,tolua_obj,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeroUserId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sShareInstance of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_sShareInstance00
static int tolua_LuaClient_LevelManager_sShareInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LevelManager* tolua_ret = (LevelManager*)  LevelManager::sShareInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LevelManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sShareInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurMapId of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_getCurMapId00
static int tolua_LuaClient_LevelManager_getCurMapId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurMapId'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getCurMapId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurMapId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentPPVELevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentPPVELevel00
static int tolua_LuaClient_LevelManager_isCurrentPPVELevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentPPVELevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentPPVELevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentPPVELevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentPVELevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentPVELevel00
static int tolua_LuaClient_LevelManager_isCurrentPVELevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentPVELevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentPVELevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentPVELevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentPVPLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentPVPLevel00
static int tolua_LuaClient_LevelManager_isCurrentPVPLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentPVPLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentPVPLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentPVPLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentCommonPKLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentCommonPKLevel00
static int tolua_LuaClient_LevelManager_isCurrentCommonPKLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentCommonPKLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentCommonPKLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentCommonPKLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentMandPKLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentMandPKLevel00
static int tolua_LuaClient_LevelManager_isCurrentMandPKLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentMandPKLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentMandPKLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentMandPKLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentEliteLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentEliteLevel00
static int tolua_LuaClient_LevelManager_isCurrentEliteLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentEliteLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentEliteLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentEliteLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentUndergroundLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentUndergroundLevel00
static int tolua_LuaClient_LevelManager_isCurrentUndergroundLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentUndergroundLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentUndergroundLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentUndergroundLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentCityDefendLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentCityDefendLevel00
static int tolua_LuaClient_LevelManager_isCurrentCityDefendLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentCityDefendLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentCityDefendLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentCityDefendLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentWorldBossLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentWorldBossLevel00
static int tolua_LuaClient_LevelManager_isCurrentWorldBossLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentWorldBossLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentWorldBossLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentWorldBossLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentBestFighterLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentBestFighterLevel00
static int tolua_LuaClient_LevelManager_isCurrentBestFighterLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentBestFighterLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentBestFighterLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentBestFighterLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCurrentGuildInstanceLevel of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_isCurrentGuildInstanceLevel00
static int tolua_LuaClient_LevelManager_isCurrentGuildInstanceLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCurrentGuildInstanceLevel'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isCurrentGuildInstanceLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCurrentGuildInstanceLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentLevelLayer of class  LevelManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelManager_getCurrentLevelLayer00
static int tolua_LuaClient_LevelManager_getCurrentLevelLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelManager* self = (LevelManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentLevelLayer'", NULL);
#endif
  {
   LevelLayer* tolua_ret = (LevelLayer*)  self->getCurrentLevelLayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LevelLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentLevelLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_LuaClient_USERINFO_new00
static int tolua_LuaClient_USERINFO_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"USERINFO",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   USERINFO* tolua_ret = (USERINFO*)  Mtolua_new((USERINFO)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"USERINFO");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_LuaClient_USERINFO_new00_local
static int tolua_LuaClient_USERINFO_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"USERINFO",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   USERINFO* tolua_ret = (USERINFO*)  Mtolua_new((USERINFO)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"USERINFO");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: id of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_id
static int tolua_get_USERINFO_id(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->id,"PLAYER_ID");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_id
static int tolua_set_USERINFO_id(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: userSession of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_userSession
static int tolua_get_USERINFO_userSession(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'userSession'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->userSession);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: userSession of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_userSession
static int tolua_set_USERINFO_userSession(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'userSession'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->userSession,(const char*)tolua_tostring(tolua_S,2,0),SESSION_LENGTH-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szName of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_szName
static int tolua_get_USERINFO_szName(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szName'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->szName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szName of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_szName
static int tolua_set_USERINFO_szName(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szName'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->szName,(const char*)tolua_tostring(tolua_S,2,0),20-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ip of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_ip
static int tolua_get_USERINFO_ip(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_ip
static int tolua_set_USERINFO_ip(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->ip,(const char*)tolua_tostring(tolua_S,2,0),30-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lastServerName of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_lastServerName
static int tolua_get_USERINFO_lastServerName(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastServerName'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->lastServerName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lastServerName of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_lastServerName
static int tolua_set_USERINFO_lastServerName(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastServerName'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->lastServerName,(const char*)tolua_tostring(tolua_S,2,0),30-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: port of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_port
static int tolua_get_USERINFO_unsigned_port(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_port
static int tolua_set_USERINFO_unsigned_port(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->port = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: createdTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_createdTime
static int tolua_get_USERINFO_createdTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'createdTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->createdTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: createdTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_createdTime
static int tolua_set_USERINFO_createdTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'createdTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->createdTime = ((long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tokenId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_tokenId
static int tolua_get_USERINFO_unsigned_tokenId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tokenId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tokenId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tokenId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_tokenId
static int tolua_set_USERINFO_unsigned_tokenId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tokenId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tokenId = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: level of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_level
static int tolua_get_USERINFO_unsigned_level(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'level'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->level);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: level of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_level
static int tolua_set_USERINFO_unsigned_level(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'level'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->level = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: exp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_exp
static int tolua_get_USERINFO_unsigned_exp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'exp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->exp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: exp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_exp
static int tolua_set_USERINFO_unsigned_exp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'exp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->exp = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: nextExp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_nextExp
static int tolua_get_USERINFO_unsigned_nextExp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'nextExp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->nextExp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: nextExp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_nextExp
static int tolua_set_USERINFO_unsigned_nextExp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'nextExp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->nextExp = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mapId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_mapId
static int tolua_get_USERINFO_unsigned_mapId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mapId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mapId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mapId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_mapId
static int tolua_set_USERINFO_unsigned_mapId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mapId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mapId = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_xPos
static int tolua_get_USERINFO_unsigned_xPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xPos'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->xPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_xPos
static int tolua_set_USERINFO_unsigned_xPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xPos'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_yPos
static int tolua_get_USERINFO_unsigned_yPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yPos'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->yPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_yPos
static int tolua_set_USERINFO_unsigned_yPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yPos'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->yPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: orient of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_orient
static int tolua_get_USERINFO_unsigned_orient(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'orient'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->orient);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: orient of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_orient
static int tolua_set_USERINFO_unsigned_orient(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'orient'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->orient = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: type of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_type
static int tolua_get_USERINFO_unsigned_type(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: type of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_type
static int tolua_set_USERINFO_unsigned_type(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->type = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: battleId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_battleId
static int tolua_get_USERINFO_unsigned_battleId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'battleId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->battleId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: battleId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_battleId
static int tolua_set_USERINFO_unsigned_battleId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'battleId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->battleId = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: current_title of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_current_title
static int tolua_get_USERINFO_unsigned_current_title(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'current_title'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->current_title);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: current_title of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_current_title
static int tolua_set_USERINFO_unsigned_current_title(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'current_title'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->current_title = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: battleSide of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_battleSide
static int tolua_get_USERINFO_unsigned_battleSide(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'battleSide'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->battleSide);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: battleSide of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_battleSide
static int tolua_set_USERINFO_unsigned_battleSide(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'battleSide'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->battleSide = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_spriteEnergy of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_spriteEnergy
static int tolua_get_USERINFO_unsigned_m_spriteEnergy(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_spriteEnergy'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_spriteEnergy);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_spriteEnergy of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_spriteEnergy
static int tolua_set_USERINFO_unsigned_m_spriteEnergy(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_spriteEnergy'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_spriteEnergy = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_gold of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_gold
static int tolua_get_USERINFO_unsigned_m_gold(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_gold'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_gold);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_gold of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_gold
static int tolua_set_USERINFO_unsigned_m_gold(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_gold'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_gold = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_diamond of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_diamond
static int tolua_get_USERINFO_unsigned_m_diamond(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_diamond'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_diamond);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_diamond of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_diamond
static int tolua_set_USERINFO_unsigned_m_diamond(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_diamond'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_diamond = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_stamina of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_stamina
static int tolua_get_USERINFO_unsigned_m_stamina(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_stamina'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_stamina);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_stamina of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_stamina
static int tolua_set_USERINFO_unsigned_m_stamina(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_stamina'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_stamina = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_spriteChip of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_spriteChip
static int tolua_get_USERINFO_unsigned_m_spriteChip(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_spriteChip'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_spriteChip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_spriteChip of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_spriteChip
static int tolua_set_USERINFO_unsigned_m_spriteChip(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_spriteChip'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_spriteChip = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_alchemyBagUnlockPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_alchemyBagUnlockPos
static int tolua_get_USERINFO_unsigned_m_alchemyBagUnlockPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_alchemyBagUnlockPos'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_alchemyBagUnlockPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_alchemyBagUnlockPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_alchemyBagUnlockPos
static int tolua_set_USERINFO_unsigned_m_alchemyBagUnlockPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_alchemyBagUnlockPos'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_alchemyBagUnlockPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_alchemyStorageUnlockPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_alchemyStorageUnlockPos
static int tolua_get_USERINFO_unsigned_m_alchemyStorageUnlockPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_alchemyStorageUnlockPos'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_alchemyStorageUnlockPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_alchemyStorageUnlockPos of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_alchemyStorageUnlockPos
static int tolua_set_USERINFO_unsigned_m_alchemyStorageUnlockPos(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_alchemyStorageUnlockPos'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_alchemyStorageUnlockPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_gemRecastTimes of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_gemRecastTimes
static int tolua_get_USERINFO_unsigned_m_gemRecastTimes(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_gemRecastTimes'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_gemRecastTimes);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_gemRecastTimes of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_gemRecastTimes
static int tolua_set_USERINFO_unsigned_m_gemRecastTimes(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_gemRecastTimes'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_gemRecastTimes = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_reputation of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_reputation
static int tolua_get_USERINFO_unsigned_m_reputation(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_reputation'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_reputation);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_reputation of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_reputation
static int tolua_set_USERINFO_unsigned_m_reputation(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_reputation'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_reputation = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_soulStone of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_soulStone
static int tolua_get_USERINFO_unsigned_m_soulStone(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_soulStone'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_soulStone);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_soulStone of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_soulStone
static int tolua_set_USERINFO_unsigned_m_soulStone(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_soulStone'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_soulStone = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: physical_attack of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_physical_attack
static int tolua_get_USERINFO_unsigned_physical_attack(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'physical_attack'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->physical_attack);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: physical_attack of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_physical_attack
static int tolua_set_USERINFO_unsigned_physical_attack(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'physical_attack'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->physical_attack = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: magic_attack of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_magic_attack
static int tolua_get_USERINFO_unsigned_magic_attack(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'magic_attack'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->magic_attack);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: magic_attack of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_magic_attack
static int tolua_set_USERINFO_unsigned_magic_attack(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'magic_attack'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->magic_attack = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: skill_attack of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_skill_attack
static int tolua_get_USERINFO_unsigned_skill_attack(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skill_attack'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skill_attack);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skill_attack of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_skill_attack
static int tolua_set_USERINFO_unsigned_skill_attack(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skill_attack'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skill_attack = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: physical_defence of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_physical_defence
static int tolua_get_USERINFO_unsigned_physical_defence(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'physical_defence'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->physical_defence);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: physical_defence of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_physical_defence
static int tolua_set_USERINFO_unsigned_physical_defence(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'physical_defence'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->physical_defence = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: magic_defence of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_magic_defence
static int tolua_get_USERINFO_unsigned_magic_defence(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'magic_defence'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->magic_defence);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: magic_defence of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_magic_defence
static int tolua_set_USERINFO_unsigned_magic_defence(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'magic_defence'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->magic_defence = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: skill_defence of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_skill_defence
static int tolua_get_USERINFO_unsigned_skill_defence(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skill_defence'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skill_defence);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skill_defence of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_skill_defence
static int tolua_set_USERINFO_unsigned_skill_defence(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skill_defence'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skill_defence = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: health_point of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_health_point
static int tolua_get_USERINFO_unsigned_health_point(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'health_point'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->health_point);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: health_point of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_health_point
static int tolua_set_USERINFO_unsigned_health_point(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'health_point'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->health_point = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: accurate of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_accurate
static int tolua_get_USERINFO_unsigned_accurate(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'accurate'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->accurate);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: accurate of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_accurate
static int tolua_set_USERINFO_unsigned_accurate(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'accurate'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->accurate = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dodge of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_dodge
static int tolua_get_USERINFO_unsigned_dodge(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dodge'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->dodge);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dodge of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_dodge
static int tolua_set_USERINFO_unsigned_dodge(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dodge'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dodge = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: wreck of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_wreck
static int tolua_get_USERINFO_unsigned_wreck(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wreck'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->wreck);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: wreck of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_wreck
static int tolua_set_USERINFO_unsigned_wreck(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wreck'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wreck = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: parry of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_parry
static int tolua_get_USERINFO_unsigned_parry(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parry'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->parry);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: parry of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_parry
static int tolua_set_USERINFO_unsigned_parry(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parry'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->parry = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: critical_strike of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_critical_strike
static int tolua_get_USERINFO_unsigned_critical_strike(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'critical_strike'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->critical_strike);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: critical_strike of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_critical_strike
static int tolua_set_USERINFO_unsigned_critical_strike(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'critical_strike'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->critical_strike = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tenacity of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_tenacity
static int tolua_get_USERINFO_unsigned_tenacity(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tenacity'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tenacity);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tenacity of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_tenacity
static int tolua_set_USERINFO_unsigned_tenacity(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tenacity'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tenacity = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: slay of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_slay
static int tolua_get_USERINFO_unsigned_slay(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'slay'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->slay);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: slay of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_slay
static int tolua_set_USERINFO_unsigned_slay(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'slay'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->slay = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: proficiency of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_proficiency
static int tolua_get_USERINFO_unsigned_proficiency(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proficiency'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->proficiency);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: proficiency of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_proficiency
static int tolua_set_USERINFO_unsigned_proficiency(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proficiency'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->proficiency = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: speed of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_speed
static int tolua_get_USERINFO_unsigned_speed(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->speed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: speed of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_speed
static int tolua_set_USERINFO_unsigned_speed(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->speed = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: total_hp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_total_hp
static int tolua_get_USERINFO_unsigned_total_hp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'total_hp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->total_hp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: total_hp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_total_hp
static int tolua_set_USERINFO_unsigned_total_hp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'total_hp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->total_hp = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: courage of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_courage
static int tolua_get_USERINFO_unsigned_courage(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'courage'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->courage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: courage of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_courage
static int tolua_set_USERINFO_unsigned_courage(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'courage'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->courage = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: charm of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_charm
static int tolua_get_USERINFO_unsigned_charm(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'charm'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->charm);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: charm of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_charm
static int tolua_set_USERINFO_unsigned_charm(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'charm'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->charm = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: trick of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_trick
static int tolua_get_USERINFO_unsigned_trick(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'trick'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->trick);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: trick of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_trick
static int tolua_set_USERINFO_unsigned_trick(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'trick'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->trick = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: effectiveness of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_effectiveness
static int tolua_get_USERINFO_unsigned_effectiveness(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'effectiveness'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->effectiveness);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: effectiveness of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_effectiveness
static int tolua_set_USERINFO_unsigned_effectiveness(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'effectiveness'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->effectiveness = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_player_exploit of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_player_exploit
static int tolua_get_USERINFO_unsigned_m_player_exploit(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_player_exploit'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_player_exploit);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_player_exploit of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_player_exploit
static int tolua_set_USERINFO_unsigned_m_player_exploit(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_player_exploit'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_player_exploit = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_playerExploerExperience of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_playerExploerExperience
static int tolua_get_USERINFO_unsigned_m_playerExploerExperience(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_playerExploerExperience'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_playerExploerExperience);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_playerExploerExperience of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_playerExploerExperience
static int tolua_set_USERINFO_unsigned_m_playerExploerExperience(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_playerExploerExperience'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_playerExploerExperience = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_playerGemAnima of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_playerGemAnima
static int tolua_get_USERINFO_unsigned_m_playerGemAnima(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_playerGemAnima'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_playerGemAnima);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_playerGemAnima of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_playerGemAnima
static int tolua_set_USERINFO_unsigned_m_playerGemAnima(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_playerGemAnima'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_playerGemAnima = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_enterManorTimes of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_enterManorTimes
static int tolua_get_USERINFO_unsigned_m_enterManorTimes(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_enterManorTimes'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_enterManorTimes);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_enterManorTimes of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_enterManorTimes
static int tolua_set_USERINFO_unsigned_m_enterManorTimes(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_enterManorTimes'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_enterManorTimes = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_continueloginTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_continueloginTime
static int tolua_get_USERINFO_unsigned_m_continueloginTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_continueloginTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_continueloginTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_continueloginTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_continueloginTime
static int tolua_set_USERINFO_unsigned_m_continueloginTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_continueloginTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_continueloginTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_loginRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_loginRewardTime
static int tolua_get_USERINFO_unsigned_m_loginRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_loginRewardTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_loginRewardTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_loginRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_loginRewardTime
static int tolua_set_USERINFO_unsigned_m_loginRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_loginRewardTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_loginRewardTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_vipRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_vipRewardTime
static int tolua_get_USERINFO_unsigned_m_vipRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_vipRewardTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_vipRewardTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_vipRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_vipRewardTime
static int tolua_set_USERINFO_unsigned_m_vipRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_vipRewardTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_vipRewardTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_payRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_payRewardTime
static int tolua_get_USERINFO_unsigned_m_payRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_payRewardTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_payRewardTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_payRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_payRewardTime
static int tolua_set_USERINFO_unsigned_m_payRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_payRewardTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_payRewardTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_buyDiamondRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_buyDiamondRewardTime
static int tolua_get_USERINFO_unsigned_m_buyDiamondRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_buyDiamondRewardTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_buyDiamondRewardTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_buyDiamondRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_buyDiamondRewardTime
static int tolua_set_USERINFO_unsigned_m_buyDiamondRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_buyDiamondRewardTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_buyDiamondRewardTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_buyManorTimes of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_buyManorTimes
static int tolua_get_USERINFO_unsigned_m_buyManorTimes(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_buyManorTimes'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_buyManorTimes);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_buyManorTimes of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_buyManorTimes
static int tolua_set_USERINFO_unsigned_m_buyManorTimes(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_buyManorTimes'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_buyManorTimes = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_pvaiRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_pvaiRewardTime
static int tolua_get_USERINFO_unsigned_m_pvaiRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvaiRewardTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_pvaiRewardTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_pvaiRewardTime of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_pvaiRewardTime
static int tolua_set_USERINFO_unsigned_m_pvaiRewardTime(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvaiRewardTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_pvaiRewardTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_palyHoldExp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_palyHoldExp
static int tolua_get_USERINFO_unsigned_m_palyHoldExp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_palyHoldExp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_palyHoldExp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_palyHoldExp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_palyHoldExp
static int tolua_set_USERINFO_unsigned_m_palyHoldExp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_palyHoldExp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_palyHoldExp = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_newMailNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_newMailNum
static int tolua_get_USERINFO_unsigned_m_newMailNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_newMailNum'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_newMailNum);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_newMailNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_newMailNum
static int tolua_set_USERINFO_unsigned_m_newMailNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_newMailNum'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_newMailNum = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_totalMailNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_totalMailNum
static int tolua_get_USERINFO_unsigned_m_totalMailNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_totalMailNum'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_totalMailNum);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_totalMailNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_totalMailNum
static int tolua_set_USERINFO_unsigned_m_totalMailNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_totalMailNum'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_totalMailNum = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_autofightNormalCd of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_autofightNormalCd
static int tolua_get_USERINFO_unsigned_m_autofightNormalCd(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_autofightNormalCd'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_autofightNormalCd);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_autofightNormalCd of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_autofightNormalCd
static int tolua_set_USERINFO_unsigned_m_autofightNormalCd(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_autofightNormalCd'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_autofightNormalCd = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_autofightJingYingCd of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_autofightJingYingCd
static int tolua_get_USERINFO_unsigned_m_autofightJingYingCd(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_autofightJingYingCd'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_autofightJingYingCd);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_autofightJingYingCd of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_autofightJingYingCd
static int tolua_set_USERINFO_unsigned_m_autofightJingYingCd(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_autofightJingYingCd'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_autofightJingYingCd = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pay_money of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_pay_money
static int tolua_get_USERINFO_unsigned_pay_money(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pay_money'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pay_money);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pay_money of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_pay_money
static int tolua_set_USERINFO_unsigned_pay_money(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pay_money'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pay_money = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mine_money of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_mine_money
static int tolua_get_USERINFO_unsigned_mine_money(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mine_money'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mine_money);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mine_money of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_mine_money
static int tolua_set_USERINFO_unsigned_mine_money(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mine_money'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mine_money = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: train_money of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_train_money
static int tolua_get_USERINFO_unsigned_train_money(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'train_money'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->train_money);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: train_money of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_train_money
static int tolua_set_USERINFO_unsigned_train_money(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'train_money'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->train_money = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: suspend_limit_exp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_suspend_limit_exp
static int tolua_get_USERINFO_unsigned_suspend_limit_exp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'suspend_limit_exp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->suspend_limit_exp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: suspend_limit_exp of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_suspend_limit_exp
static int tolua_set_USERINFO_unsigned_suspend_limit_exp(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'suspend_limit_exp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->suspend_limit_exp = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_vip of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_vip
static int tolua_get_USERINFO_unsigned_m_vip(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_vip'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_vip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_vip of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_vip
static int tolua_set_USERINFO_unsigned_m_vip(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_vip'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_vip = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_fragStone of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_fragStone
static int tolua_get_USERINFO_unsigned_m_fragStone(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fragStone'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_fragStone);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_fragStone of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_fragStone
static int tolua_set_USERINFO_unsigned_m_fragStone(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fragStone'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fragStone = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_pvpDuelTimesNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_pvpDuelTimesNum
static int tolua_get_USERINFO_unsigned_m_pvpDuelTimesNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvpDuelTimesNum'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_pvpDuelTimesNum);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_pvpDuelTimesNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_pvpDuelTimesNum
static int tolua_set_USERINFO_unsigned_m_pvpDuelTimesNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvpDuelTimesNum'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_pvpDuelTimesNum = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_pvpDueledTimesNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_pvpDueledTimesNum
static int tolua_get_USERINFO_unsigned_m_pvpDueledTimesNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvpDueledTimesNum'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_pvpDueledTimesNum);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_pvpDueledTimesNum of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_pvpDueledTimesNum
static int tolua_set_USERINFO_unsigned_m_pvpDueledTimesNum(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvpDueledTimesNum'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_pvpDueledTimesNum = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_pvpRed of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_pvpRed
static int tolua_get_USERINFO_unsigned_m_pvpRed(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvpRed'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_pvpRed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_pvpRed of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_pvpRed
static int tolua_set_USERINFO_unsigned_m_pvpRed(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_pvpRed'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_pvpRed = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b_hasEnouchFairyFlag of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_b_hasEnouchFairyFlag
static int tolua_get_USERINFO_b_hasEnouchFairyFlag(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_hasEnouchFairyFlag'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->b_hasEnouchFairyFlag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b_hasEnouchFairyFlag of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_b_hasEnouchFairyFlag
static int tolua_set_USERINFO_b_hasEnouchFairyFlag(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_hasEnouchFairyFlag'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b_hasEnouchFairyFlag = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b_reputationExchangeFlag of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_b_reputationExchangeFlag
static int tolua_get_USERINFO_b_reputationExchangeFlag(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_reputationExchangeFlag'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->b_reputationExchangeFlag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b_reputationExchangeFlag of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_b_reputationExchangeFlag
static int tolua_set_USERINFO_b_reputationExchangeFlag(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_reputationExchangeFlag'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b_reputationExchangeFlag = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_diamond_acc of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_unsigned_m_diamond_acc
static int tolua_get_USERINFO_unsigned_m_diamond_acc(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_diamond_acc'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_diamond_acc);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_diamond_acc of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_unsigned_m_diamond_acc
static int tolua_set_USERINFO_unsigned_m_diamond_acc(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_diamond_acc'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_diamond_acc = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: guildName of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_guildName
static int tolua_get_USERINFO_guildName(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'guildName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->guildName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: guildName of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_guildName
static int tolua_set_USERINFO_guildName(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'guildName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->guildName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_guildId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_get_USERINFO_m_guildId
static int tolua_get_USERINFO_m_guildId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_guildId'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_guildId,"GUILD_ID");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_guildId of class  USERINFO */
#ifndef TOLUA_DISABLE_tolua_set_USERINFO_m_guildId
static int tolua_set_USERINFO_m_guildId(lua_State* tolua_S)
{
  USERINFO* self = (USERINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_guildId'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"GUILD_ID",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_guildId = *((GUILD_ID*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_new00
static int tolua_LuaClient_PLAYER_ID_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PLAYER_ID* tolua_ret = (PLAYER_ID*)  Mtolua_new((PLAYER_ID)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PLAYER_ID");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_new00_local
static int tolua_LuaClient_PLAYER_ID_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PLAYER_ID* tolua_ret = (PLAYER_ID*)  Mtolua_new((PLAYER_ID)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPlayId of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_setPlayId00
static int tolua_LuaClient_PLAYER_ID_setPlayId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PLAYER_ID* self = (PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int regTime = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int channel = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlayId'", NULL);
#endif
  {
   self->setPlayId(id,regTime,channel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPlayId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPlayId of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_setPlayId01
static int tolua_LuaClient_PLAYER_ID_setPlayId01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  PLAYER_ID* self = (PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID* playId = ((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlayId'", NULL);
#endif
  {
   self->setPlayId(*playId);
  }
 }
 return 0;
tolua_lerror:
 return tolua_LuaClient_PLAYER_ID_setPlayId00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: isEqual of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_isEqual00
static int tolua_LuaClient_PLAYER_ID_isEqual00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PLAYER_ID* self = (PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID* playId = ((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEqual'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isEqual(*playId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEqual'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getID of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_getID00
static int tolua_LuaClient_PLAYER_ID_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const PLAYER_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const PLAYER_ID* self = (const PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRegTime of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_getRegTime00
static int tolua_LuaClient_PLAYER_ID_getRegTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PLAYER_ID* self = (PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRegTime'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getRegTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRegTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChannel of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_getChannel00
static int tolua_LuaClient_PLAYER_ID_getChannel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PLAYER_ID* self = (PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChannel'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getChannel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChannel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRegTime of class  PLAYER_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PLAYER_ID_setRegTime00
static int tolua_LuaClient_PLAYER_ID_setRegTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PLAYER_ID",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PLAYER_ID* self = (PLAYER_ID*)  tolua_tousertype(tolua_S,1,0);
  unsigned int reg_time = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRegTime'", NULL);
#endif
  {
   self->setRegTime(reg_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRegTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_new00
static int tolua_LuaClient_GUILD_ID_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GUILD_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GUILD_ID* tolua_ret = (GUILD_ID*)  Mtolua_new((GUILD_ID)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUILD_ID");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_new00_local
static int tolua_LuaClient_GUILD_ID_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GUILD_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GUILD_ID* tolua_ret = (GUILD_ID*)  Mtolua_new((GUILD_ID)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUILD_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGuildId of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_setGuildId00
static int tolua_LuaClient_GUILD_ID_setGuildId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GUILD_ID",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GUILD_ID* self = (GUILD_ID*)  tolua_tousertype(tolua_S,1,0);
  unsigned int high_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int low_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGuildId'", NULL);
#endif
  {
   self->setGuildId(high_id,low_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGuildId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGuildId of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_setGuildId01
static int tolua_LuaClient_GUILD_ID_setGuildId01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GUILD_ID",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const GUILD_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  GUILD_ID* self = (GUILD_ID*)  tolua_tousertype(tolua_S,1,0);
  const GUILD_ID* guildId = ((const GUILD_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGuildId'", NULL);
#endif
  {
   self->setGuildId(*guildId);
  }
 }
 return 0;
tolua_lerror:
 return tolua_LuaClient_GUILD_ID_setGuildId00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: isEqual of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_isEqual00
static int tolua_LuaClient_GUILD_ID_isEqual00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GUILD_ID",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const GUILD_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GUILD_ID* self = (GUILD_ID*)  tolua_tousertype(tolua_S,1,0);
  const GUILD_ID* guildId = ((const GUILD_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEqual'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isEqual(*guildId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEqual'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHighID of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_getHighID00
static int tolua_LuaClient_GUILD_ID_getHighID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GUILD_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GUILD_ID* self = (const GUILD_ID*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHighID'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getHighID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHighID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLowID of class  GUILD_ID */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GUILD_ID_getLowID00
static int tolua_LuaClient_GUILD_ID_getLowID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GUILD_ID",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GUILD_ID* self = (const GUILD_ID*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLowID'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getLowID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLowID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_Get00
static int tolua_LuaClient_UserData_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   UserData* tolua_ret = (UserData*)  UserData::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UserData");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUserId of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getUserId00
static int tolua_LuaClient_UserData_getUserId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PLAYER_ID tolua_ret = (PLAYER_ID)  UserData::getUserId();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((PLAYER_ID)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(PLAYER_ID));
     tolua_pushusertype(tolua_S,tolua_obj,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUserId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUserName of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getUserName00
static int tolua_LuaClient_UserData_getUserName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  UserData::getUserName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUserName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setUserName of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_setUserName00
static int tolua_LuaClient_UserData_setUserName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* newName = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   UserData::setUserName(newName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setUserName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHeroHp of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getHeroHp00
static int tolua_LuaClient_UserData_getHeroHp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::getHeroHp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeroHp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHeroTotalHp of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getHeroTotalHp00
static int tolua_LuaClient_UserData_getHeroTotalHp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::getHeroTotalHp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeroTotalHp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUserType of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_GetUserType00
static int tolua_LuaClient_UserData_GetUserType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::GetUserType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUserType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUserInfo of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_GetUserInfo00
static int tolua_LuaClient_UserData_GetUserInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   USERINFO& tolua_ret = (USERINFO&)  UserData::GetUserInfo();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"USERINFO");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUserInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUserLevel of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_GetUserLevel00
static int tolua_LuaClient_UserData_GetUserLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::GetUserLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUserLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUserInfo of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_SetUserInfo00
static int tolua_LuaClient_UserData_SetUserInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"USERINFO",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  USERINFO info = *((USERINFO*)  tolua_tousertype(tolua_S,2,0));
  {
   UserData::SetUserInfo(info);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUserInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUserInfo of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_GetUserInfo01
static int tolua_LuaClient_UserData_GetUserInfo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UserData",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  UserData* self = (UserData*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID id = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetUserInfo'", NULL);
#endif
  {
   USERINFO* tolua_ret = (USERINFO*)  self->GetUserInfo(id);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"USERINFO");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_UserData_GetUserInfo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUserInfo of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_SetUserInfo01
static int tolua_LuaClient_UserData_SetUserInfo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UserData",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"USERINFO",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  UserData* self = (UserData*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID id = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  USERINFO* info = ((USERINFO*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUserInfo'", NULL);
#endif
  {
   self->SetUserInfo(id,*info);
  }
 }
 return 0;
tolua_lerror:
 return tolua_LuaClient_UserData_SetUserInfo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeOtherUser of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_removeOtherUser00
static int tolua_LuaClient_UserData_removeOtherUser00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UserData",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UserData* self = (UserData*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID id = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeOtherUser'", NULL);
#endif
  {
   self->removeOtherUser(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeOtherUser'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRagePoint of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getRagePoint00
static int tolua_LuaClient_UserData_getRagePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::getRagePoint();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRagePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTotalRagePoint of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getTotalRagePoint00
static int tolua_LuaClient_UserData_getTotalRagePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::getTotalRagePoint();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTotalRagePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSecRagePoint of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getSecRagePoint00
static int tolua_LuaClient_UserData_getSecRagePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::getSecRagePoint();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSecRagePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSecTotalRagePoint of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_getSecTotalRagePoint00
static int tolua_LuaClient_UserData_getSecTotalRagePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::getSecTotalRagePoint();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSecTotalRagePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDiamond of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_GetDiamond00
static int tolua_LuaClient_UserData_GetDiamond00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::GetDiamond();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDiamond'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetVipLevel of class  UserData */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UserData_GetVipLevel00
static int tolua_LuaClient_UserData_GetVipLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UserData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  UserData::GetVipLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetVipLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: PlayerIdMake */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PlayerIdMake00
static int tolua_LuaClient_PlayerIdMake00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int regTime = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int channel = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  {
   PLAYER_ID tolua_ret = (PLAYER_ID)  PlayerIdMake(id,regTime,channel);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((PLAYER_ID)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(PLAYER_ID));
     tolua_pushusertype(tolua_S,tolua_obj,"PLAYER_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PlayerIdMake'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GuildIdMake */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GuildIdMake00
static int tolua_LuaClient_GuildIdMake00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int high_id = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned int low_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   GUILD_ID tolua_ret = (GUILD_ID)  GuildIdMake(high_id,low_id);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((GUILD_ID)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"GUILD_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(GUILD_ID));
     tolua_pushusertype(tolua_S,tolua_obj,"GUILD_ID");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GuildIdMake'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: defaultCenter of class  NotificationCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_NotificationCenter_defaultCenter00
static int tolua_LuaClient_NotificationCenter_defaultCenter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"NotificationCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   NotificationCenter* tolua_ret = (NotificationCenter*)  NotificationCenter::defaultCenter();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"NotificationCenter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'defaultCenter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerScriptObserver of class  NotificationCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_NotificationCenter_registerScriptObserver00
static int tolua_LuaClient_NotificationCenter_registerScriptObserver00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NotificationCenter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NotificationCenter* self = (NotificationCenter*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerScriptObserver'", NULL);
#endif
  {
   int tolua_ret = (int)  self->registerScriptObserver(handler);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerScriptObserver'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: broadcast of class  NotificationCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_NotificationCenter_broadcast00
static int tolua_LuaClient_NotificationCenter_broadcast00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NotificationCenter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NotificationCenter* self = (NotificationCenter*)  tolua_tousertype(tolua_S,1,0);
  int msg = ((int)  tolua_tonumber(tolua_S,2,0));
  void* sender = ((void*)  tolua_touserdata(tolua_S,3,0));
  std::string param1 = ((std::string)  tolua_tocppstring(tolua_S,4,""));
  std::string param2 = ((std::string)  tolua_tocppstring(tolua_S,5,""));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'broadcast'", NULL);
#endif
  {
   self->broadcast(msg,sender,param1,param2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'broadcast'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterScriptObserver of class  NotificationCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_NotificationCenter_unregisterScriptObserver00
static int tolua_LuaClient_NotificationCenter_unregisterScriptObserver00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NotificationCenter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NotificationCenter* self = (NotificationCenter*)  tolua_tousertype(tolua_S,1,0);
  int handler = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterScriptObserver'", NULL);
#endif
  {
   self->unregisterScriptObserver(handler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterScriptObserver'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterAllScriptObservers of class  NotificationCenter */
#ifndef TOLUA_DISABLE_tolua_LuaClient_NotificationCenter_unregisterAllScriptObservers00
static int tolua_LuaClient_NotificationCenter_unregisterAllScriptObservers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NotificationCenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NotificationCenter* self = (NotificationCenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterAllScriptObservers'", NULL);
#endif
  {
   self->unregisterAllScriptObservers();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterAllScriptObservers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_Get00
static int tolua_LuaClient_GameManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameManager* tolua_ret = (GameManager*)  GameManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSceneLayer of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_GetSceneLayer00
static int tolua_LuaClient_GameManager_GetSceneLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameManager* self = (GameManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSceneLayer'", NULL);
#endif
  {
   SceneLayer* tolua_ret = (SceneLayer*)  self->GetSceneLayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SceneLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSceneLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHero of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_getHero00
static int tolua_LuaClient_GameManager_getHero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameManager* self = (GameManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHero'", NULL);
#endif
  {
   SpriteSeer* tolua_ret = (SpriteSeer*)  self->getHero();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteSeer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHero of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_setHero00
static int tolua_LuaClient_GameManager_setHero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SpriteSeer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameManager* self = (GameManager*)  tolua_tousertype(tolua_S,1,0);
  SpriteSeer* hero = ((SpriteSeer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHero'", NULL);
#endif
  {
   self->setHero(hero);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHeroTypeId of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_getHeroTypeId00
static int tolua_LuaClient_GameManager_getHeroTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameManager* self = (GameManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeroTypeId'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getHeroTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeroTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHeroTypeId of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_setHeroTypeId00
static int tolua_LuaClient_GameManager_setHeroTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameManager* self = (GameManager*)  tolua_tousertype(tolua_S,1,0);
  int typeId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHeroTypeId'", NULL);
#endif
  {
   self->setHeroTypeId(typeId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHeroTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: goToSplash of class  GameManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameManager_goToSplash00
static int tolua_LuaClient_GameManager_goToSplash00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameManager* self = (GameManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'goToSplash'", NULL);
#endif
  {
   CCScene* tolua_ret = (CCScene*)  self->goToSplash();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCScene");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'goToSplash'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  SkillUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkillUIManager_getInstance00
static int tolua_LuaClient_SkillUIManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SkillUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SkillUIManager* tolua_ret = (SkillUIManager*)  SkillUIManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SkillUIManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetValue of class  SkillUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkillUIManager_ResetValue00
static int tolua_LuaClient_SkillUIManager_ResetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkillUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkillUIManager* self = (SkillUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetValue'", NULL);
#endif
  {
   self->ResetValue();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendGetUserSkillReqMessage of class  SkillUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkillUIManager_sendGetUserSkillReqMessage00
static int tolua_LuaClient_SkillUIManager_sendGetUserSkillReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkillUIManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkillUIManager* self = (SkillUIManager*)  tolua_tousertype(tolua_S,1,0);
  bool bSender = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendGetUserSkillReqMessage'", NULL);
#endif
  {
   self->sendGetUserSkillReqMessage(bSender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendGetUserSkillReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendAddUserSkillReqMessage of class  SkillUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkillUIManager_sendAddUserSkillReqMessage00
static int tolua_LuaClient_SkillUIManager_sendAddUserSkillReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkillUIManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkillUIManager* self = (SkillUIManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int skillId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int levelId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendAddUserSkillReqMessage'", NULL);
#endif
  {
   self->sendAddUserSkillReqMessage(skillId,levelId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendAddUserSkillReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendUserSkillDiffReqMessage of class  SkillUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkillUIManager_sendUserSkillDiffReqMessage00
static int tolua_LuaClient_SkillUIManager_sendUserSkillDiffReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkillUIManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkillUIManager* self = (SkillUIManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int skillId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int levelId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendUserSkillDiffReqMessage'", NULL);
#endif
  {
   self->sendUserSkillDiffReqMessage(skillId,levelId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendUserSkillDiffReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendSetSlotAttributeMessage of class  SkillUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkillUIManager_sendSetSlotAttributeMessage00
static int tolua_LuaClient_SkillUIManager_sendSetSlotAttributeMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkillUIManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkillUIManager* self = (SkillUIManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int slotIndex = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int skillId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendSetSlotAttributeMessage'", NULL);
#endif
  {
   self->sendSetSlotAttributeMessage(slotIndex,skillId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendSetSlotAttributeMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_new00
static int tolua_LuaClient_ElfExploreManger_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ElfExploreManger* tolua_ret = (ElfExploreManger*)  Mtolua_new((ElfExploreManger)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ElfExploreManger");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_new00_local
static int tolua_LuaClient_ElfExploreManger_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ElfExploreManger* tolua_ret = (ElfExploreManger*)  Mtolua_new((ElfExploreManger)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ElfExploreManger");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_Get00
static int tolua_LuaClient_ElfExploreManger_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ElfExploreManger* tolua_ret = (ElfExploreManger*)  ElfExploreManger::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ElfExploreManger");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Destroy of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_Destroy00
static int tolua_LuaClient_ElfExploreManger_Destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ElfExploreManger::Destroy();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetValue of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_ResetValue00
static int tolua_LuaClient_ElfExploreManger_ResetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ElfExploreManger* self = (ElfExploreManger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetValue'", NULL);
#endif
  {
   self->ResetValue();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendExchangeElfReq of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_SendExchangeElfReq00
static int tolua_LuaClient_ElfExploreManger_SendExchangeElfReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ElfExploreManger* self = (ElfExploreManger*)  tolua_tousertype(tolua_S,1,0);
  unsigned int elfId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendExchangeElfReq'", NULL);
#endif
  {
   self->SendExchangeElfReq(elfId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendExchangeElfReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendExploreReq of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_SendExploreReq00
static int tolua_LuaClient_ElfExploreManger_SendExploreReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ElfExploreManger* self = (ElfExploreManger*)  tolua_tousertype(tolua_S,1,0);
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendExploreReq'", NULL);
#endif
  {
   self->SendExploreReq(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendExploreReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendExploreDetermineReq of class  ElfExploreManger */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ElfExploreManger_SendExploreDetermineReq00
static int tolua_LuaClient_ElfExploreManger_SendExploreDetermineReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ElfExploreManger",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ElfExploreManger* self = (ElfExploreManger*)  tolua_tousertype(tolua_S,1,0);
  unsigned int optionId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendExploreDetermineReq'", NULL);
#endif
  {
   self->SendExploreDetermineReq(optionId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendExploreDetermineReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setChatToolbar of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_setChatToolbar00
static int tolua_LuaClient_MainMenuLayer_setChatToolbar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  bool isChatVisible = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool isFunctionVisible = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool isExtraVisible = ((bool)  tolua_toboolean(tolua_S,4,false));
  int order = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setChatToolbar'", NULL);
#endif
  {
   self->setChatToolbar(isChatVisible,isFunctionVisible,isExtraVisible,order);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setChatToolbar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMainMenuLayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_getMainMenuLayer00
static int tolua_LuaClient_MainMenuLayer_getMainMenuLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MainMenuLayer* tolua_ret = (MainMenuLayer*)  MainMenuLayer::getMainMenuLayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MainMenuLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainMenuLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumByFormat of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_GetNumByFormat00
static int tolua_LuaClient_MainMenuLayer_GetNumByFormat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int num = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   const char* tolua_ret = (const char*)  MainMenuLayer::GetNumByFormat(num);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumByFormat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumByFormatK of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_GetNumByFormatK00
static int tolua_LuaClient_MainMenuLayer_GetNumByFormatK00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int num = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   const char* tolua_ret = (const char*)  MainMenuLayer::GetNumByFormatK(num);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumByFormatK'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showUILayerByTag of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_showUILayerByTag00
static int tolua_LuaClient_MainMenuLayer_showUILayerByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
  bool isAutoCreate = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showUILayerByTag'", NULL);
#endif
  {
   CCLayer* tolua_ret = (CCLayer*)  self->showUILayerByTag(tag,isAutoCreate);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showUILayerByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findUILayerByTag of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_findUILayerByTag00
static int tolua_LuaClient_MainMenuLayer_findUILayerByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findUILayerByTag'", NULL);
#endif
  {
   CCLayer* tolua_ret = (CCLayer*)  self->findUILayerByTag(tag);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findUILayerByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hideUILayerByTag of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_hideUILayerByTag00
static int tolua_LuaClient_MainMenuLayer_hideUILayerByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hideUILayerByTag'", NULL);
#endif
  {
   self->hideUILayerByTag(tag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hideUILayerByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUILayerByTag of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_getUILayerByTag00
static int tolua_LuaClient_MainMenuLayer_getUILayerByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
  bool isAutoCreate = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUILayerByTag'", NULL);
#endif
  {
   CCLayer* tolua_ret = (CCLayer*)  self->getUILayerByTag(tag,isAutoCreate);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUILayerByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowPPVELayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_ShowPPVELayer00
static int tolua_LuaClient_MainMenuLayer_ShowPPVELayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowPPVELayer'", NULL);
#endif
  {
   PPVELayer* tolua_ret = (PPVELayer*)  self->ShowPPVELayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PPVELayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowPPVELayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HidePPVELayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_HidePPVELayer00
static int tolua_LuaClient_MainMenuLayer_HidePPVELayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  bool bShowGameUI = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HidePPVELayer'", NULL);
#endif
  {
   self->HidePPVELayer(bShowGameUI);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HidePPVELayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowPVPLayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_ShowPVPLayer00
static int tolua_LuaClient_MainMenuLayer_ShowPVPLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowPVPLayer'", NULL);
#endif
  {
   PVPLayer* tolua_ret = (PVPLayer*)  self->ShowPVPLayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PVPLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowPVPLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resetMailUIMsg of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_resetMailUIMsg00
static int tolua_LuaClient_MainMenuLayer_resetMailUIMsg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resetMailUIMsg'", NULL);
#endif
  {
   self->resetMailUIMsg();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetMailUIMsg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HidePVPLayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_HidePVPLayer00
static int tolua_LuaClient_MainMenuLayer_HidePVPLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  bool bShowGameUI = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HidePVPLayer'", NULL);
#endif
  {
   self->HidePVPLayer(bShowGameUI);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HidePVPLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showMagicShop of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_showMagicShop00
static int tolua_LuaClient_MainMenuLayer_showMagicShop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showMagicShop'", NULL);
#endif
  {
   self->showMagicShop();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showMagicShop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onReceivedElfStrengthLayerClosed of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_onReceivedElfStrengthLayerClosed00
static int tolua_LuaClient_MainMenuLayer_onReceivedElfStrengthLayerClosed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onReceivedElfStrengthLayerClosed'", NULL);
#endif
  {
   self->onReceivedElfStrengthLayerClosed();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onReceivedElfStrengthLayerClosed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFairyMovedIcon of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_setFairyMovedIcon00
static int tolua_LuaClient_MainMenuLayer_setFairyMovedIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int changedPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFairyMovedIcon'", NULL);
#endif
  {
   self->setFairyMovedIcon(changedPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFairyMovedIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFairySelectStatus of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_setFairySelectStatus00
static int tolua_LuaClient_MainMenuLayer_setFairySelectStatus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  bool isSelected = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFairySelectStatus'", NULL);
#endif
  {
   self->setFairySelectStatus(isSelected);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFairySelectStatus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowMarketLayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_ShowMarketLayer00
static int tolua_LuaClient_MainMenuLayer_ShowMarketLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
  int layerTag = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowMarketLayer'", NULL);
#endif
  {
   self->ShowMarketLayer(flag,layerTag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowMarketLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowChargeLayer of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_ShowChargeLayer00
static int tolua_LuaClient_MainMenuLayer_ShowChargeLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowChargeLayer'", NULL);
#endif
  {
   self->ShowChargeLayer(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowChargeLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HideFirstChargeEffect of class  MainMenuLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MainMenuLayer_HideFirstChargeEffect00
static int tolua_LuaClient_MainMenuLayer_HideFirstChargeEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainMenuLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainMenuLayer* self = (MainMenuLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HideFirstChargeEffect'", NULL);
#endif
  {
   self->HideFirstChargeEffect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HideFirstChargeEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedManager of class  GameAudioManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameAudioManager_sharedManager00
static int tolua_LuaClient_GameAudioManager_sharedManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameAudioManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameAudioManager* tolua_ret = (GameAudioManager*)  GameAudioManager::sharedManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameAudioManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playEffect of class  GameAudioManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameAudioManager_playEffect00
static int tolua_LuaClient_GameAudioManager_playEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameAudioManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameAudioManager* self = (GameAudioManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int soundID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  bool isLoop = ((bool)  tolua_toboolean(tolua_S,3,0));
  const float delay = ((const float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playEffect'", NULL);
#endif
  {
   self->playEffect(soundID,isLoop,delay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopAllEffect of class  GameAudioManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameAudioManager_stopAllEffect00
static int tolua_LuaClient_GameAudioManager_stopAllEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameAudioManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameAudioManager* self = (GameAudioManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopAllEffect'", NULL);
#endif
  {
   self->stopAllEffect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopAllEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopPlayBGM of class  GameAudioManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameAudioManager_stopPlayBGM00
static int tolua_LuaClient_GameAudioManager_stopPlayBGM00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameAudioManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameAudioManager* self = (GameAudioManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopPlayBGM'", NULL);
#endif
  {
   self->stopPlayBGM();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopPlayBGM'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: preLoadEffect of class  GameAudioManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameAudioManager_preLoadEffect00
static int tolua_LuaClient_GameAudioManager_preLoadEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameAudioManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameAudioManager* self = (GameAudioManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int soundID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'preLoadEffect'", NULL);
#endif
  {
   self->preLoadEffect(soundID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'preLoadEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedManager of class  ParticleManagerX */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ParticleManagerX_sharedManager00
static int tolua_LuaClient_ParticleManagerX_sharedManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ParticleManagerX",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ParticleManagerX* tolua_ret = (ParticleManagerX*)  ParticleManagerX::sharedManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ParticleManagerX");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParticles of class  ParticleManagerX */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ParticleManagerX_getParticles00
static int tolua_LuaClient_ParticleManagerX_getParticles00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ParticleManagerX",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ParticleManagerX* self = (ParticleManagerX*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParticles'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getParticles(name);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParticles'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParticles_uieffect of class  ParticleManagerX */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ParticleManagerX_getParticles_uieffect00
static int tolua_LuaClient_ParticleManagerX_getParticles_uieffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ParticleManagerX",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ParticleManagerX* self = (ParticleManagerX*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParticles_uieffect'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getParticles_uieffect(name);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParticles_uieffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  ParticleManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ParticleManager_Get00
static int tolua_LuaClient_ParticleManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ParticleManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ParticleManager* tolua_ret = (ParticleManager*)  ParticleManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ParticleManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createEffectSpriteAndPlay of class  ParticleManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ParticleManager_createEffectSpriteAndPlay00
static int tolua_LuaClient_ParticleManager_createEffectSpriteAndPlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ParticleManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ParticleManager* self = (ParticleManager*)  tolua_tousertype(tolua_S,1,0);
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  int animId = ((int)  tolua_tonumber(tolua_S,4,0));
  int nloop = ((int)  tolua_tonumber(tolua_S,5,0));
  bool isLoop = ((bool)  tolua_toboolean(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createEffectSpriteAndPlay'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->createEffectSpriteAndPlay(type,name,animId,nloop,isLoop);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createEffectSpriteAndPlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playOneTimeEffect of class  ParticleManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ParticleManager_playOneTimeEffect00
static int tolua_LuaClient_ParticleManager_playOneTimeEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ParticleManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ParticleManager* self = (ParticleManager*)  tolua_tousertype(tolua_S,1,0);
  int effectId = ((int)  tolua_tonumber(tolua_S,2,0));
  CCPoint position = *((CCPoint*)  tolua_tousertype(tolua_S,3,0));
  float delayTime = ((float)  tolua_tonumber(tolua_S,4,0));
  int playType = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playOneTimeEffect'", NULL);
#endif
  {
   EffectSprite* tolua_ret = (EffectSprite*)  self->playOneTimeEffect(effectId,position,delayTime,playType);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"EffectSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playOneTimeEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attr_key of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_get_ITEMATTR_unsigned_attr_key
static int tolua_get_ITEMATTR_unsigned_attr_key(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_key'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->attr_key);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attr_key of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_set_ITEMATTR_unsigned_attr_key
static int tolua_set_ITEMATTR_unsigned_attr_key(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_key'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attr_key = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attr_basic_value of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_get_ITEMATTR_unsigned_attr_basic_value
static int tolua_get_ITEMATTR_unsigned_attr_basic_value(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_basic_value'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->attr_basic_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attr_basic_value of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_set_ITEMATTR_unsigned_attr_basic_value
static int tolua_set_ITEMATTR_unsigned_attr_basic_value(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_basic_value'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attr_basic_value = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attr_intensify_value of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_get_ITEMATTR_unsigned_attr_intensify_value
static int tolua_get_ITEMATTR_unsigned_attr_intensify_value(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_intensify_value'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->attr_intensify_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attr_intensify_value of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_set_ITEMATTR_unsigned_attr_intensify_value
static int tolua_set_ITEMATTR_unsigned_attr_intensify_value(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_intensify_value'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attr_intensify_value = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attr_gem_value of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_get_ITEMATTR_unsigned_attr_gem_value
static int tolua_get_ITEMATTR_unsigned_attr_gem_value(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_gem_value'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->attr_gem_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attr_gem_value of class  ITEMATTR */
#ifndef TOLUA_DISABLE_tolua_set_ITEMATTR_unsigned_attr_gem_value
static int tolua_set_ITEMATTR_unsigned_attr_gem_value(lua_State* tolua_S)
{
  ITEMATTR* self = (ITEMATTR*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attr_gem_value'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attr_gem_value = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: hurtGetCoin of class  MANORBASEINFO */
#ifndef TOLUA_DISABLE_tolua_get_MANORBASEINFO_unsigned_hurtGetCoin
static int tolua_get_MANORBASEINFO_unsigned_hurtGetCoin(lua_State* tolua_S)
{
  MANORBASEINFO* self = (MANORBASEINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hurtGetCoin'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->hurtGetCoin);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: hurtGetCoin of class  MANORBASEINFO */
#ifndef TOLUA_DISABLE_tolua_set_MANORBASEINFO_unsigned_hurtGetCoin
static int tolua_set_MANORBASEINFO_unsigned_hurtGetCoin(lua_State* tolua_S)
{
  MANORBASEINFO* self = (MANORBASEINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hurtGetCoin'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->hurtGetCoin = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pickGetExp of class  MANORBASEINFO */
#ifndef TOLUA_DISABLE_tolua_get_MANORBASEINFO_unsigned_pickGetExp
static int tolua_get_MANORBASEINFO_unsigned_pickGetExp(lua_State* tolua_S)
{
  MANORBASEINFO* self = (MANORBASEINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pickGetExp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pickGetExp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pickGetExp of class  MANORBASEINFO */
#ifndef TOLUA_DISABLE_tolua_set_MANORBASEINFO_unsigned_pickGetExp
static int tolua_set_MANORBASEINFO_unsigned_pickGetExp(lua_State* tolua_S)
{
  MANORBASEINFO* self = (MANORBASEINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pickGetExp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pickGetExp = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_LuaClient_BackPackItem_getItemInfo00
static int tolua_LuaClient_BackPackItem_getItemInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BackPackItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemInfo'", NULL);
#endif
  {
   ItemBase* tolua_ret = (ItemBase*)  self->getItemInfo();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ItemBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemId of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_LuaClient_BackPackItem_getItemId00
static int tolua_LuaClient_BackPackItem_getItemId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BackPackItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemId'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getItemId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isLocked of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_isLocked
static int tolua_get_BackPackItem_isLocked(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isLocked'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isLocked);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isLocked of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_isLocked
static int tolua_set_BackPackItem_isLocked(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isLocked'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isLocked = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isEmpty of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_isEmpty
static int tolua_get_BackPackItem_isEmpty(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isEmpty'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isEmpty);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isEmpty of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_isEmpty
static int tolua_set_BackPackItem_isEmpty(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isEmpty'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isEmpty = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_unsigned_position
static int tolua_get_BackPackItem_unsigned_position(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->position);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_unsigned_position
static int tolua_set_BackPackItem_unsigned_position(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: amount of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_unsigned_amount
static int tolua_get_BackPackItem_unsigned_amount(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'amount'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->amount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: amount of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_unsigned_amount
static int tolua_set_BackPackItem_unsigned_amount(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'amount'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->amount = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: itemLevel of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_unsigned_itemLevel
static int tolua_get_BackPackItem_unsigned_itemLevel(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'itemLevel'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->itemLevel);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: itemLevel of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_unsigned_itemLevel
static int tolua_set_BackPackItem_unsigned_itemLevel(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'itemLevel'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->itemLevel = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_equipInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_m_equipInfo_ptr
static int tolua_get_BackPackItem_m_equipInfo_ptr(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_equipInfo'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->m_equipInfo,"EQUIPINFO");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_equipInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_m_equipInfo_ptr
static int tolua_set_BackPackItem_m_equipInfo_ptr(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_equipInfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"EQUIPINFO",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_equipInfo = ((EQUIPINFO*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: packType of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_unsigned_packType
static int tolua_get_BackPackItem_unsigned_packType(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'packType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->packType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: packType of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_unsigned_packType
static int tolua_set_BackPackItem_unsigned_packType(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'packType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->packType = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_spriteInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_m_spriteInfo_ptr
static int tolua_get_BackPackItem_m_spriteInfo_ptr(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_spriteInfo'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->m_spriteInfo,"SPRITEINFO");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_spriteInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_m_spriteInfo_ptr
static int tolua_set_BackPackItem_m_spriteInfo_ptr(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_spriteInfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"SPRITEINFO",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_spriteInfo = ((SPRITEINFO*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_manorBaseInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_get_BackPackItem_m_manorBaseInfo_ptr
static int tolua_get_BackPackItem_m_manorBaseInfo_ptr(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_manorBaseInfo'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->m_manorBaseInfo,"MANORBASEINFO");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_manorBaseInfo of class  BackPackItem */
#ifndef TOLUA_DISABLE_tolua_set_BackPackItem_m_manorBaseInfo_ptr
static int tolua_set_BackPackItem_m_manorBaseInfo_ptr(lua_State* tolua_S)
{
  BackPackItem* self = (BackPackItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_manorBaseInfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"MANORBASEINFO",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_manorBaseInfo = ((MANORBASEINFO*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_Get00
static int tolua_LuaClient_ItemManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ItemManager* tolua_ret = (ItemManager*)  ItemManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ItemManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findItemByPos of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_findItemByPos00
static int tolua_LuaClient_ItemManager_findItemByPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int pos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findItemByPos'", NULL);
#endif
  {
   BackPackItem* tolua_ret = (BackPackItem*)  self->findItemByPos(pos);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BackPackItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findItemByPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIconSpriteById of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getIconSpriteById00
static int tolua_LuaClient_ItemManager_getIconSpriteById00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ItemId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIconSpriteById'", NULL);
#endif
  {
   CCSprite* tolua_ret = (CCSprite*)  self->getIconSpriteById(ItemId);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIconSpriteById'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIconFrame of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getIconFrame00
static int tolua_LuaClient_ItemManager_getIconFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int level = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIconFrame'", NULL);
#endif
  {
   CCSprite* tolua_ret = (CCSprite*)  self->getIconFrame(level);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIconFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLabelColorByQuality of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getLabelColorByQuality00
static int tolua_LuaClient_ItemManager_getLabelColorByQuality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLabelColorByQuality'", NULL);
#endif
  {
   ccColor3B tolua_ret = (ccColor3B)  self->getLabelColorByQuality(quality);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((ccColor3B)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(ccColor3B));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLabelColorByQuality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFairyLabelColorByQuality of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getFairyLabelColorByQuality00
static int tolua_LuaClient_ItemManager_getFairyLabelColorByQuality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFairyLabelColorByQuality'", NULL);
#endif
  {
   ccColor3B tolua_ret = (ccColor3B)  self->getFairyLabelColorByQuality(quality);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((ccColor3B)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(ccColor3B));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFairyLabelColorByQuality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemNumberById of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getItemNumberById00
static int tolua_LuaClient_ItemManager_getItemNumberById00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemNumberById'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getItemNumberById(id);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemNumberById'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttributionText of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getAttributionText00
static int tolua_LuaClient_ItemManager_getAttributionText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int key = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int value = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttributionText'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getAttributionText(key,value);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttributionText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showItemTipsById of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_showItemTipsById00
static int tolua_LuaClient_ItemManager_showItemTipsById00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int itemId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  CCPoint pt = *((CCPoint*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showItemTipsById'", NULL);
#endif
  {
   self->showItemTipsById(itemId,pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showItemTipsById'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTipLuaHander of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_setTipLuaHander00
static int tolua_LuaClient_ItemManager_setTipLuaHander00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !toluafix_isfunction(tolua_S,5,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION leftHandler = (  toluafix_ref_function(tolua_S,2,0));
  const char* leftText = ((const char*)  tolua_tostring(tolua_S,3,0));
  bool isLeftVisible = ((bool)  tolua_toboolean(tolua_S,4,0));
  LUA_FUNCTION rightHandler = (  toluafix_ref_function(tolua_S,5,0));
  const char* rightText = ((const char*)  tolua_tostring(tolua_S,6,0));
  bool isRightVisible = ((bool)  tolua_toboolean(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTipLuaHander'", NULL);
#endif
  {
   self->setTipLuaHander(leftHandler,leftText,isLeftVisible,rightHandler,rightText,isRightVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTipLuaHander'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMiddleLuaHander of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_setMiddleLuaHander00
static int tolua_LuaClient_ItemManager_setMiddleLuaHander00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION leftHandler = (  toluafix_ref_function(tolua_S,2,0));
  const char* leftText = ((const char*)  tolua_tostring(tolua_S,3,0));
  bool isLeftVisible = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMiddleLuaHander'", NULL);
#endif
  {
   self->setMiddleLuaHander(leftHandler,leftText,isLeftVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMiddleLuaHander'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemInfoById of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_getItemInfoById00
static int tolua_LuaClient_ItemManager_getItemInfoById00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemInfoById'", NULL);
#endif
  {
   ItemBase* tolua_ret = (ItemBase*)  self->getItemInfoById(id);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ItemBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemInfoById'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openBackPack of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_openBackPack00
static int tolua_LuaClient_ItemManager_openBackPack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  bool withEquipment = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openBackPack'", NULL);
#endif
  {
   self->openBackPack(withEquipment);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openBackPack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeEquipment of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_closeEquipment00
static int tolua_LuaClient_ItemManager_closeEquipment00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeEquipment'", NULL);
#endif
  {
   self->closeEquipment();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeEquipment'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: checkBagEmptyItemNum of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_checkBagEmptyItemNum00
static int tolua_LuaClient_ItemManager_checkBagEmptyItemNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'checkBagEmptyItemNum'", NULL);
#endif
  {
   self->checkBagEmptyItemNum();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'checkBagEmptyItemNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showSpriteParticleEffect of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemManager_showSpriteParticleEffect00
static int tolua_LuaClient_ItemManager_showSpriteParticleEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  CCNode* parent = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  CCPoint pt = *((CCPoint*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showSpriteParticleEffect'", NULL);
#endif
  {
   self->showSpriteParticleEffect(parent,id,pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showSpriteParticleEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEquipLevel of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ItemBase_getEquipLevel00
static int tolua_LuaClient_ItemBase_getEquipLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEquipLevel'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getEquipLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEquipLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_id of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_id
static int tolua_get_ItemBase_unsigned_m_id(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_id of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_id
static int tolua_set_ItemBase_unsigned_m_id(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_name of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_m_name
static int tolua_get_ItemBase_m_name(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->m_name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_name of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_m_name
static int tolua_set_ItemBase_m_name(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_name'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->m_name,(const char*)tolua_tostring(tolua_S,2,0),128-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b_isPiledUp of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_b_isPiledUp
static int tolua_get_ItemBase_b_isPiledUp(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_isPiledUp'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->b_isPiledUp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b_isPiledUp of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_b_isPiledUp
static int tolua_set_ItemBase_b_isPiledUp(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_isPiledUp'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b_isPiledUp = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_maxPiledNum of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_maxPiledNum
static int tolua_get_ItemBase_unsigned_m_maxPiledNum(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_maxPiledNum'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_maxPiledNum);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_maxPiledNum of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_maxPiledNum
static int tolua_set_ItemBase_unsigned_m_maxPiledNum(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_maxPiledNum'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_maxPiledNum = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_source of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_LuaClient_ItemBase_m_source
static int tolua_get_LuaClient_ItemBase_m_source(lua_State* tolua_S)
{
 int tolua_index;
  ItemBase* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ItemBase*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=5)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_source[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_source of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_LuaClient_ItemBase_m_source
static int tolua_set_LuaClient_ItemBase_m_source(lua_State* tolua_S)
{
 int tolua_index;
  ItemBase* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ItemBase*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=5)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->m_source[tolua_index] = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_byeprice of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_byeprice
static int tolua_get_ItemBase_unsigned_m_byeprice(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_byeprice'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_byeprice);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_byeprice of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_byeprice
static int tolua_set_ItemBase_unsigned_m_byeprice(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_byeprice'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_byeprice = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_sellprice of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_sellprice
static int tolua_get_ItemBase_unsigned_m_sellprice(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_sellprice'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_sellprice);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_sellprice of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_sellprice
static int tolua_set_ItemBase_unsigned_m_sellprice(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_sellprice'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_sellprice = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_requiredLevel of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_requiredLevel
static int tolua_get_ItemBase_unsigned_m_requiredLevel(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_requiredLevel'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_requiredLevel);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_requiredLevel of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_requiredLevel
static int tolua_set_ItemBase_unsigned_m_requiredLevel(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_requiredLevel'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_requiredLevel = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_type of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_type
static int tolua_get_ItemBase_unsigned_m_type(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_type of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_type
static int tolua_set_ItemBase_unsigned_m_type(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_type = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b_Destroyed of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_b_Destroyed
static int tolua_get_ItemBase_b_Destroyed(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_Destroyed'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->b_Destroyed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b_Destroyed of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_b_Destroyed
static int tolua_set_ItemBase_b_Destroyed(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b_Destroyed'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b_Destroyed = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_szDescription of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_m_szDescription
static int tolua_get_ItemBase_m_szDescription(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_szDescription'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->m_szDescription);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_szDescription of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_m_szDescription
static int tolua_set_ItemBase_m_szDescription(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_szDescription'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->m_szDescription,(const char*)tolua_tostring(tolua_S,2,0),128-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_quality of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_quality
static int tolua_get_ItemBase_unsigned_m_quality(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_quality'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_quality);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_quality of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_quality
static int tolua_set_ItemBase_unsigned_m_quality(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_quality'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_quality = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_iconId of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_iconId
static int tolua_get_ItemBase_unsigned_m_iconId(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iconId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_iconId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_iconId of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_iconId
static int tolua_set_ItemBase_unsigned_m_iconId(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iconId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_iconId = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_order of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_get_ItemBase_unsigned_m_order
static int tolua_get_ItemBase_unsigned_m_order(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_order'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_order);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_order of class  ItemBase */
#ifndef TOLUA_DISABLE_tolua_set_ItemBase_unsigned_m_order
static int tolua_set_ItemBase_unsigned_m_order(lua_State* tolua_S)
{
  ItemBase* self = (ItemBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_order'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_order = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ownerType of class  EquipmentItem */
#ifndef TOLUA_DISABLE_tolua_get_EquipmentItem_unsigned_m_ownerType
static int tolua_get_EquipmentItem_unsigned_m_ownerType(lua_State* tolua_S)
{
  EquipmentItem* self = (EquipmentItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ownerType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ownerType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ownerType of class  EquipmentItem */
#ifndef TOLUA_DISABLE_tolua_set_EquipmentItem_unsigned_m_ownerType
static int tolua_set_EquipmentItem_unsigned_m_ownerType(lua_State* tolua_S)
{
  EquipmentItem* self = (EquipmentItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ownerType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ownerType = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_equipPart of class  EquipmentItem */
#ifndef TOLUA_DISABLE_tolua_get_EquipmentItem_unsigned_m_equipPart
static int tolua_get_EquipmentItem_unsigned_m_equipPart(lua_State* tolua_S)
{
  EquipmentItem* self = (EquipmentItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_equipPart'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_equipPart);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_equipPart of class  EquipmentItem */
#ifndef TOLUA_DISABLE_tolua_set_EquipmentItem_unsigned_m_equipPart
static int tolua_set_EquipmentItem_unsigned_m_equipPart(lua_State* tolua_S)
{
  EquipmentItem* self = (EquipmentItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_equipPart'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_equipPart = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_equipLevel of class  EquipmentItem */
#ifndef TOLUA_DISABLE_tolua_get_EquipmentItem_unsigned_m_equipLevel
static int tolua_get_EquipmentItem_unsigned_m_equipLevel(lua_State* tolua_S)
{
  EquipmentItem* self = (EquipmentItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_equipLevel'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_equipLevel);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_equipLevel of class  EquipmentItem */
#ifndef TOLUA_DISABLE_tolua_set_EquipmentItem_unsigned_m_equipLevel
static int tolua_set_EquipmentItem_unsigned_m_equipLevel(lua_State* tolua_S)
{
  EquipmentItem* self = (EquipmentItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_equipLevel'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_equipLevel = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_Get00
static int tolua_LuaClient_TimeManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TimeManager* tolua_ret = (TimeManager*)  TimeManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TimeManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurServerTime of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_getCurServerTime00
static int tolua_LuaClient_TimeManager_getCurServerTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurServerTime'", NULL);
#endif
  {
   long tolua_ret = (long)  self->getCurServerTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurServerTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInited of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_isInited00
static int tolua_LuaClient_TimeManager_isInited00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInited'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInited();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInited'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTimeZone of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_getTimeZone00
static int tolua_LuaClient_TimeManager_getTimeZone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTimeZone'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getTimeZone();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimeZone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: secondsToString of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_secondsToString00
static int tolua_LuaClient_TimeManager_secondsToString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  long seconds = ((long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'secondsToString'", NULL);
#endif
  {
   string tolua_ret = (string)  self->secondsToString(seconds);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'secondsToString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerLuaTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_registerLuaTimer00
static int tolua_LuaClient_TimeManager_registerLuaTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
  int counterId = ((int)  tolua_tonumber(tolua_S,3,0));
  long endTime = ((long)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerLuaTimer'", NULL);
#endif
  {
   int tolua_ret = (int)  self->registerLuaTimer(handler,counterId,endTime);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerLuaTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterLuaTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_unregisterLuaTimer00
static int tolua_LuaClient_TimeManager_unregisterLuaTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  int handler = ((int)  tolua_tonumber(tolua_S,2,0));
  int counterId = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterLuaTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->unregisterLuaTimer(handler,counterId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterLuaTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachLuaTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_attachLuaTimer00
static int tolua_LuaClient_TimeManager_attachLuaTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
  int counterId = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachLuaTimer'", NULL);
#endif
  {
   int tolua_ret = (int)  self->attachLuaTimer(handler,counterId);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachLuaTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: startTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_startTimer00
static int tolua_LuaClient_TimeManager_startTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  int counterId = ((int)  tolua_tonumber(tolua_S,2,0));
  long endTime = ((long)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'startTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->startTimer(counterId,endTime);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'startTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_stopTimer00
static int tolua_LuaClient_TimeManager_stopTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  int counterId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->stopTimer(counterId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: renewTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_renewTimer00
static int tolua_LuaClient_TimeManager_renewTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  int counterId = ((int)  tolua_tonumber(tolua_S,2,0));
  long endTime = ((long)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'renewTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->renewTimer(counterId,endTime);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'renewTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasTimer of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_hasTimer00
static int tolua_LuaClient_TimeManager_hasTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  int counterId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasTimer(counterId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasLuaObserver of class  TimeManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TimeManager_hasLuaObserver00
static int tolua_LuaClient_TimeManager_hasLuaObserver00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeManager* self = (TimeManager*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
  int counterId = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasLuaObserver'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasLuaObserver(handler,counterId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasLuaObserver'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  GemStonesFromLua */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GemStonesFromLua_getInstance00
static int tolua_LuaClient_GemStonesFromLua_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GemStonesFromLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GemStonesFromLua* tolua_ret = (GemStonesFromLua*)  GemStonesFromLua::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GemStonesFromLua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EquipInlayGemReq of class  GemStonesFromLua */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GemStonesFromLua_EquipInlayGemReq00
static int tolua_LuaClient_GemStonesFromLua_EquipInlayGemReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GemStonesFromLua",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GemStonesFromLua* self = (GemStonesFromLua*)  tolua_tousertype(tolua_S,1,0);
  unsigned int equip = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int hole = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int gemId = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EquipInlayGemReq'", NULL);
#endif
  {
   self->EquipInlayGemReq(equip,hole,gemId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EquipInlayGemReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GemRecastReq of class  GemStonesFromLua */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GemStonesFromLua_GemRecastReq00
static int tolua_LuaClient_GemStonesFromLua_GemRecastReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GemStonesFromLua",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GemStonesFromLua* self = (GemStonesFromLua*)  tolua_tousertype(tolua_S,1,0);
  unsigned int equipPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int hole1 = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int hole2 = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  unsigned int hole3 = ((unsigned int)  tolua_tonumber(tolua_S,5,0));
  unsigned int hole4 = ((unsigned int)  tolua_tonumber(tolua_S,6,0));
  unsigned int currencyKey = ((unsigned int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GemRecastReq'", NULL);
#endif
  {
   self->GemRecastReq(equipPos,hole1,hole2,hole3,hole4,currencyKey);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GemRecastReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GemAnimaExchangeReq of class  GemStonesFromLua */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GemStonesFromLua_GemAnimaExchangeReq00
static int tolua_LuaClient_GemStonesFromLua_GemAnimaExchangeReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GemStonesFromLua",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GemStonesFromLua* self = (GemStonesFromLua*)  tolua_tousertype(tolua_S,1,0);
  unsigned int gemId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int multi = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GemAnimaExchangeReq'", NULL);
#endif
  {
   self->GemAnimaExchangeReq(gemId,multi);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GemAnimaExchangeReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GemHoleUnlockReq of class  GemStonesFromLua */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GemStonesFromLua_GemHoleUnlockReq00
static int tolua_LuaClient_GemStonesFromLua_GemHoleUnlockReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GemStonesFromLua",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GemStonesFromLua* self = (GemStonesFromLua*)  tolua_tousertype(tolua_S,1,0);
  unsigned int exchangeId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int equipPos = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GemHoleUnlockReq'", NULL);
#endif
  {
   self->GemHoleUnlockReq(exchangeId,equipPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GemHoleUnlockReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  PvAIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PvAIManager_Get00
static int tolua_LuaClient_PvAIManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PvAIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PvAIManager* tolua_ret = (PvAIManager*)  PvAIManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PvAIManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendPvAIReq of class  PvAIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PvAIManager_sendPvAIReq00
static int tolua_LuaClient_PvAIManager_sendPvAIReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PvAIManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PvAIManager* self = (PvAIManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int userId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendPvAIReq'", NULL);
#endif
  {
   self->sendPvAIReq(userId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendPvAIReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onMsgPvAIReward of class  PvAIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PvAIManager_onMsgPvAIReward00
static int tolua_LuaClient_PvAIManager_onMsgPvAIReward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PvAIManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PvAIManager* self = (PvAIManager*)  tolua_tousertype(tolua_S,1,0);
  int accuCoin = ((int)  tolua_tonumber(tolua_S,2,0));
  int accuReputation = ((int)  tolua_tonumber(tolua_S,3,0));
  int oneCoin = ((int)  tolua_tonumber(tolua_S,4,0));
  int oneReputation = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onMsgPvAIReward'", NULL);
#endif
  {
   self->onMsgPvAIReward(accuCoin,accuReputation,oneCoin,oneReputation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onMsgPvAIReward'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCoolDownEndTime of class  PvAIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PvAIManager_getCoolDownEndTime00
static int tolua_LuaClient_PvAIManager_getCoolDownEndTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PvAIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PvAIManager* self = (PvAIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCoolDownEndTime'", NULL);
#endif
  {
   long tolua_ret = (long)  self->getCoolDownEndTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCoolDownEndTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRemainCount of class  PvAIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PvAIManager_getRemainCount00
static int tolua_LuaClient_PvAIManager_getRemainCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PvAIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PvAIManager* self = (PvAIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRemainCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getRemainCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRemainCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBuyCount of class  PvAIManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PvAIManager_getBuyCount00
static int tolua_LuaClient_PvAIManager_getBuyCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PvAIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PvAIManager* self = (PvAIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBuyCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getBuyCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBuyCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  TutorialsManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TutorialsManager_Get00
static int tolua_LuaClient_TutorialsManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TutorialsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TutorialsManager* tolua_ret = (TutorialsManager*)  TutorialsManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TutorialsManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StartOneTutorial of class  TutorialsManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TutorialsManager_StartOneTutorial00
static int tolua_LuaClient_TutorialsManager_StartOneTutorial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TutorialsManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TutorialsManager* self = (TutorialsManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StartOneTutorial'", NULL);
#endif
  {
   self->StartOneTutorial(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StartOneTutorial'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HandleOneEvent of class  TutorialsManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TutorialsManager_HandleOneEvent00
static int tolua_LuaClient_TutorialsManager_HandleOneEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TutorialsManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TutorialsManager* self = (TutorialsManager*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HandleOneEvent'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HandleOneEvent(name,type);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HandleOneEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sShareInstance of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sShareInstance00
static int tolua_LuaClient_OnlineNetworkManager_sShareInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   OnlineNetworkManager* tolua_ret = (OnlineNetworkManager*)  OnlineNetworkManager::sShareInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"OnlineNetworkManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sShareInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendDailyTaskReqMessage of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendDailyTaskReqMessage00
static int tolua_LuaClient_OnlineNetworkManager_sendDailyTaskReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendDailyTaskReqMessage'", NULL);
#endif
  {
   self->sendDailyTaskReqMessage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendDailyTaskReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendDailyTaskAcceptReqMessage of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendDailyTaskAcceptReqMessage00
static int tolua_LuaClient_OnlineNetworkManager_sendDailyTaskAcceptReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int taskId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendDailyTaskAcceptReqMessage'", NULL);
#endif
  {
   self->sendDailyTaskAcceptReqMessage(taskId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendDailyTaskAcceptReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendGiveUpTaskReqMessage of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendGiveUpTaskReqMessage00
static int tolua_LuaClient_OnlineNetworkManager_sendGiveUpTaskReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendGiveUpTaskReqMessage'", NULL);
#endif
  {
   self->sendGiveUpTaskReqMessage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendGiveUpTaskReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendExchangeParameterReq of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendExchangeParameterReq00
static int tolua_LuaClient_OnlineNetworkManager_sendExchangeParameterReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int exchangeId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int params = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  int len = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendExchangeParameterReq'", NULL);
#endif
  {
   self->sendExchangeParameterReq(exchangeId,&params,len);
   tolua_pushnumber(tolua_S,(lua_Number)params);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendExchangeParameterReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendGetDailyTaskRewardReqMessage of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendGetDailyTaskRewardReqMessage00
static int tolua_LuaClient_OnlineNetworkManager_sendGetDailyTaskRewardReqMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendGetDailyTaskRewardReqMessage'", NULL);
#endif
  {
   self->sendGetDailyTaskRewardReqMessage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendGetDailyTaskRewardReqMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendGetTaskStatusMessage of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendGetTaskStatusMessage00
static int tolua_LuaClient_OnlineNetworkManager_sendGetTaskStatusMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  int taskId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendGetTaskStatusMessage'", NULL);
#endif
  {
   self->sendGetTaskStatusMessage(taskId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendGetTaskStatusMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendUseItem of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendUseItem00
static int tolua_LuaClient_OnlineNetworkManager_sendUseItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int pos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int itemId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int num = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendUseItem'", NULL);
#endif
  {
   self->sendUseItem(pos,itemId,num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendUseItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendEquipComposeReq of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendEquipComposeReq00
static int tolua_LuaClient_OnlineNetworkManager_sendEquipComposeReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int exchangeId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int pos = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int multi = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendEquipComposeReq'", NULL);
#endif
  {
   self->sendEquipComposeReq(exchangeId,pos,multi);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendEquipComposeReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMoveFairyPosReq of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_SendMoveFairyPosReq00
static int tolua_LuaClient_OnlineNetworkManager_SendMoveFairyPosReq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int fromPos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int toPos = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMoveFairyPosReq'", NULL);
#endif
  {
   self->SendMoveFairyPosReq(fromPos,toPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMoveFairyPosReq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendMessage of class  OnlineNetworkManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_OnlineNetworkManager_sendMessage00
static int tolua_LuaClient_OnlineNetworkManager_sendMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OnlineNetworkManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OnlineNetworkManager* self = (OnlineNetworkManager*)  tolua_tousertype(tolua_S,1,0);
  char* msgName = ((char*)  tolua_tostring(tolua_S,2,0));
  void* bodyBuff = ((void*)  tolua_touserdata(tolua_S,3,0));
  int bodylen = ((int)  tolua_tonumber(tolua_S,4,0));
  bool isFailedCache = ((bool)  tolua_toboolean(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendMessage'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->sendMessage(msgName,bodyBuff,bodylen,isFailedCache);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_Get00
static int tolua_LuaClient_InstanceManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   InstanceManager* tolua_ret = (InstanceManager*)  InstanceManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"InstanceManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSpecialEnteranceMapId of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_getSpecialEnteranceMapId00
static int tolua_LuaClient_InstanceManager_getSpecialEnteranceMapId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InstanceManager* self = (InstanceManager*)  tolua_tousertype(tolua_S,1,0);
  ENTERANCE_TYPE type = ((ENTERANCE_TYPE) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSpecialEnteranceMapId'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getSpecialEnteranceMapId(type);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSpecialEnteranceMapId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onEnterSpecialEntrance of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_onEnterSpecialEntrance00
static int tolua_LuaClient_InstanceManager_onEnterSpecialEntrance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InstanceManager* self = (InstanceManager*)  tolua_tousertype(tolua_S,1,0);
  ENTERANCE_TYPE type = ((ENTERANCE_TYPE) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onEnterSpecialEntrance'", NULL);
#endif
  {
   self->onEnterSpecialEntrance(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onEnterSpecialEntrance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onLeaveSpecialEntrance of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_onLeaveSpecialEntrance00
static int tolua_LuaClient_InstanceManager_onLeaveSpecialEntrance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InstanceManager* self = (InstanceManager*)  tolua_tousertype(tolua_S,1,0);
  ENTERANCE_TYPE type = ((ENTERANCE_TYPE) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onLeaveSpecialEntrance'", NULL);
#endif
  {
   self->onLeaveSpecialEntrance(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onLeaveSpecialEntrance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: autoSearchPathToInstance of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_autoSearchPathToInstance00
static int tolua_LuaClient_InstanceManager_autoSearchPathToInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InstanceManager* self = (InstanceManager*)  tolua_tousertype(tolua_S,1,0);
  int instanceId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'autoSearchPathToInstance'", NULL);
#endif
  {
   self->autoSearchPathToInstance(instanceId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'autoSearchPathToInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOneInstanceStars of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_getOneInstanceStars00
static int tolua_LuaClient_InstanceManager_getOneInstanceStars00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InstanceManager* self = (InstanceManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int instanceId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOneInstanceStars'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getOneInstanceStars(instanceId);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOneInstanceStars'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOneInstance of class  InstanceManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceManager_getOneInstance00
static int tolua_LuaClient_InstanceManager_getOneInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InstanceManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InstanceManager* self = (InstanceManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int instanceId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOneInstance'", NULL);
#endif
  {
   InstanceInfo* tolua_ret = (InstanceInfo*)  self->getOneInstance(instanceId);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"InstanceInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOneInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_create00
static int tolua_LuaClient_IconButton_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCSprite",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* backgroundTex = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCSprite* normalTex = ((CCSprite*)  tolua_tousertype(tolua_S,3,0));
  CCSprite* selectedTex = ((CCSprite*)  tolua_tousertype(tolua_S,4,0));
  int priority = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   IconButton* tolua_ret = (IconButton*)  IconButton::create(backgroundTex,normalTex,selectedTex,priority);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"IconButton");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_new00
static int tolua_LuaClient_IconButton_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* backgroundTex = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCSprite* normalTex = ((CCSprite*)  tolua_tousertype(tolua_S,3,0));
  CCSprite* selectedTex = ((CCSprite*)  tolua_tousertype(tolua_S,4,0));
  {
   IconButton* tolua_ret = (IconButton*)  Mtolua_new((IconButton)(backgroundTex,normalTex,selectedTex));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"IconButton");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_new00_local
static int tolua_LuaClient_IconButton_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* backgroundTex = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCSprite* normalTex = ((CCSprite*)  tolua_tousertype(tolua_S,3,0));
  CCSprite* selectedTex = ((CCSprite*)  tolua_tousertype(tolua_S,4,0));
  {
   IconButton* tolua_ret = (IconButton*)  Mtolua_new((IconButton)(backgroundTex,normalTex,selectedTex));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"IconButton");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetButtonName of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetButtonName00
static int tolua_LuaClient_IconButton_SetButtonName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetButtonName'", NULL);
#endif
  {
   self->SetButtonName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetButtonName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetButtonIndex of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetButtonIndex00
static int tolua_LuaClient_IconButton_SetButtonIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetButtonIndex'", NULL);
#endif
  {
   self->SetButtonIndex(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetButtonIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetButtonIndex of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_GetButtonIndex00
static int tolua_LuaClient_IconButton_GetButtonIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetButtonIndex'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetButtonIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetButtonIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalIcon of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetNormalIcon00
static int tolua_LuaClient_IconButton_SetNormalIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* normalTex = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalIcon'", NULL);
#endif
  {
   self->SetNormalIcon(normalTex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSelectedIcon of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetSelectedIcon00
static int tolua_LuaClient_IconButton_SetSelectedIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* selectedTex = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSelectedIcon'", NULL);
#endif
  {
   self->SetSelectedIcon(selectedTex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSelectedIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddExternalSprite of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_AddExternalSprite00
static int tolua_LuaClient_IconButton_AddExternalSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* sprite = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCPoint pt = *((CCPoint*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddExternalSprite'", NULL);
#endif
  {
   self->AddExternalSprite(sprite,pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddExternalSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemLabel of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_getItemLabel00
static int tolua_LuaClient_IconButton_getItemLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemLabel'", NULL);
#endif
  {
   UILabelTTF* tolua_ret = (UILabelTTF*)  self->getItemLabel();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UILabelTTF");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setItemLabel of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_setItemLabel00
static int tolua_LuaClient_IconButton_setItemLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  const char* text = ((const char*)  tolua_tostring(tolua_S,2,0));
  float fontSize = ((float)  tolua_tonumber(tolua_S,3,0));
  const char* font = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setItemLabel'", NULL);
#endif
  {
   self->setItemLabel(text,fontSize,font);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setItemLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeItemLabel of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_removeItemLabel00
static int tolua_LuaClient_IconButton_removeItemLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeItemLabel'", NULL);
#endif
  {
   self->removeItemLabel();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeItemLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUserData of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetUserData00
static int tolua_LuaClient_IconButton_SetUserData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  int data = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUserData'", NULL);
#endif
  {
   self->SetUserData(data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUserData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDragFlagTime of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetDragFlagTime00
static int tolua_LuaClient_IconButton_SetDragFlagTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ms = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDragFlagTime'", NULL);
#endif
  {
   self->SetDragFlagTime(ms);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDragFlagTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDoubleClickHandler of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetDoubleClickHandler00
static int tolua_LuaClient_IconButton_SetDoubleClickHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SEL_MenuHandler",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCObject* listerner = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
  SEL_MenuHandler handler = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDoubleClickHandler'", NULL);
#endif
  {
   self->SetDoubleClickHandler(listerner,handler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDoubleClickHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTappedHandler of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_SetTappedHandler00
static int tolua_LuaClient_IconButton_SetTappedHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SEL_MenuHandler",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"SEL_MenuHandler",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCObject* listerner = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
  SEL_MenuHandler tapHandler = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,3,0));
  SEL_MenuHandler tapCancelHandler = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTappedHandler'", NULL);
#endif
  {
   self->SetTappedHandler(listerner,tapHandler,tapCancelHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTappedHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isReleaseInButton of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_isReleaseInButton00
static int tolua_LuaClient_IconButton_isReleaseInButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isReleaseInButton'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isReleaseInButton(touch);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isReleaseInButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDragMove of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_setDragMove00
static int tolua_LuaClient_IconButton_setDragMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  bool tolua_var_1 = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDragMove'", NULL);
#endif
  {
   self->setDragMove(tolua_var_1);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDragMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFrameSprite of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_setFrameSprite00
static int tolua_LuaClient_IconButton_setFrameSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* sprite = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCPoint pt = *((CCPoint*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFrameSprite'", NULL);
#endif
  {
   self->setFrameSprite(sprite,pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFrameSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMenuItem of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_getMenuItem00
static int tolua_LuaClient_IconButton_getMenuItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenuItem'", NULL);
#endif
  {
   CCMenuItemSprite* tolua_ret = (CCMenuItemSprite*)  self->getMenuItem();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCMenuItemSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMenuItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIconOrder of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_setIconOrder00
static int tolua_LuaClient_IconButton_setIconOrder00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  int order = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIconOrder'", NULL);
#endif
  {
   self->setIconOrder(order);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIconOrder'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHandlerPriority of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_setHandlerPriority00
static int tolua_LuaClient_IconButton_setHandlerPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  int newPriority = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHandlerPriority'", NULL);
#endif
  {
   self->setHandlerPriority(newPriority);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHandlerPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHandlerPriority of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_getHandlerPriority00
static int tolua_LuaClient_IconButton_getHandlerPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHandlerPriority'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getHandlerPriority();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHandlerPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: creatDragReceiver of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_creatDragReceiver00
static int tolua_LuaClient_IconButton_creatDragReceiver00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  DragHandlerType type = ((DragHandlerType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'creatDragReceiver'", NULL);
#endif
  {
   self->creatDragReceiver(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'creatDragReceiver'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMenuNode of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_getMenuNode00
static int tolua_LuaClient_IconButton_getMenuNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMenuNode'", NULL);
#endif
  {
   UIMenu* tolua_ret = (UIMenu*)  self->getMenuNode();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIMenu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMenuNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showTipIcon of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_showTipIcon00
static int tolua_LuaClient_IconButton_showTipIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
  const CCPoint* anchorPos = ((const CCPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showTipIcon'", NULL);
#endif
  {
   self->showTipIcon(*anchorPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showTipIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hideTipIcon of class  IconButton */
#ifndef TOLUA_DISABLE_tolua_LuaClient_IconButton_hideTipIcon00
static int tolua_LuaClient_IconButton_hideTipIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IconButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IconButton* self = (IconButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hideTipIcon'", NULL);
#endif
  {
   self->hideTipIcon();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hideTipIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_create00
static int tolua_LuaClient_UIMenu_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   UIMenu* tolua_ret = (UIMenu*)  UIMenu::create();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIMenu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_create01
static int tolua_LuaClient_UIMenu_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCMenuItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCMenuItem* item = ((CCMenuItem*)  tolua_tousertype(tolua_S,2,0));
  {
   UIMenu* tolua_ret = (UIMenu*)  UIMenu::create(item);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIMenu");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_UIMenu_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWithItems of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_createWithItems00
static int tolua_LuaClient_UIMenu_createWithItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCMenuItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"va_list",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCMenuItem* firstItem = ((CCMenuItem*)  tolua_tousertype(tolua_S,2,0));
  va_list args = *((va_list*)  tolua_tousertype(tolua_S,3,0));
  {
   UIMenu* tolua_ret = (UIMenu*)  UIMenu::createWithItems(firstItem,args);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIMenu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWithItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWithArray of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_createWithArray00
static int tolua_LuaClient_UIMenu_createWithArray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCArray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCArray* pArrayOfItems = ((CCArray*)  tolua_tousertype(tolua_S,2,0));
  {
   UIMenu* tolua_ret = (UIMenu*)  UIMenu::createWithArray(pArrayOfItems);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIMenu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWithArray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerWithTouchDispatcher of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_registerWithTouchDispatcher00
static int tolua_LuaClient_UIMenu_registerWithTouchDispatcher00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerWithTouchDispatcher'", NULL);
#endif
  {
   self->registerWithTouchDispatcher();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerWithTouchDispatcher'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ccTouchBegan of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_ccTouchBegan00
static int tolua_LuaClient_UIMenu_ccTouchBegan00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
  CCEvent* event = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ccTouchBegan'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ccTouchBegan(touch,event);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ccTouchBegan'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ccTouchMoved of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_ccTouchMoved00
static int tolua_LuaClient_UIMenu_ccTouchMoved00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
  CCEvent* event = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ccTouchMoved'", NULL);
#endif
  {
   self->ccTouchMoved(touch,event);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ccTouchMoved'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ccTouchEnded of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_ccTouchEnded00
static int tolua_LuaClient_UIMenu_ccTouchEnded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
  CCEvent* event = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ccTouchEnded'", NULL);
#endif
  {
   self->ccTouchEnded(touch,event);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ccTouchEnded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ccTouchCancelled of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_ccTouchCancelled00
static int tolua_LuaClient_UIMenu_ccTouchCancelled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
  CCEvent* event = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ccTouchCancelled'", NULL);
#endif
  {
   self->ccTouchCancelled(touch,event);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ccTouchCancelled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDragMove of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_SetDragMove00
static int tolua_LuaClient_UIMenu_SetDragMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"DragReceiver",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
  DragReceiver* m_dragHandler = ((DragReceiver*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDragMove'", NULL);
#endif
  {
   self->SetDragMove(value,m_dragHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDragMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDoubleClickHandler of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_SetDoubleClickHandler00
static int tolua_LuaClient_UIMenu_SetDoubleClickHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SEL_MenuHandler",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCObject* listerner = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
  SEL_MenuHandler handler = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDoubleClickHandler'", NULL);
#endif
  {
   self->SetDoubleClickHandler(listerner,handler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDoubleClickHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTappedHandler of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_SetTappedHandler00
static int tolua_LuaClient_UIMenu_SetTappedHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"SEL_MenuHandler",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"SEL_MenuHandler",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCObject* listerner = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
  SEL_MenuHandler tapHandler = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,3,0));
  SEL_MenuHandler tapCancelHandler = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTappedHandler'", NULL);
#endif
  {
   self->SetTappedHandler(listerner,tapHandler,tapCancelHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTappedHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalIcon of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_SetNormalIcon00
static int tolua_LuaClient_UIMenu_SetNormalIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* normalTex = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalIcon'", NULL);
#endif
  {
   self->SetNormalIcon(normalTex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: checkReleaseTouchInButton of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_checkReleaseTouchInButton00
static int tolua_LuaClient_UIMenu_checkReleaseTouchInButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'checkReleaseTouchInButton'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->checkReleaseTouchInButton(touch);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'checkReleaseTouchInButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setControlNode of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_setControlNode00
static int tolua_LuaClient_UIMenu_setControlNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"UIControlBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  UIControlBase* controller = ((UIControlBase*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setControlNode'", NULL);
#endif
  {
   self->setControlNode(controller);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setControlNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getControlNode of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_getControlNode00
static int tolua_LuaClient_UIMenu_getControlNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getControlNode'", NULL);
#endif
  {
   UIControlBase* tolua_ret = (UIControlBase*)  self->getControlNode();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIControlBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getControlNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTouchActivityTime of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_setTouchActivityTime00
static int tolua_LuaClient_UIMenu_setTouchActivityTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTouchActivityTime'", NULL);
#endif
  {
   self->setTouchActivityTime(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTouchActivityTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTouchPoint of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_getTouchPoint00
static int tolua_LuaClient_UIMenu_getTouchPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTouchPoint'", NULL);
#endif
  {
   CCPoint tolua_ret = (CCPoint)  self->getTouchPoint();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTouchPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDragFlagTime of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_SetDragFlagTime00
static int tolua_LuaClient_UIMenu_SetDragFlagTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ms = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDragFlagTime'", NULL);
#endif
  {
   self->SetDragFlagTime(ms);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDragFlagTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInitPriority of class  UIMenu */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIMenu_setInitPriority00
static int tolua_LuaClient_UIMenu_setInitPriority00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIMenu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIMenu* self = (UIMenu*)  tolua_tousertype(tolua_S,1,0);
  int priority = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInitPriority'", NULL);
#endif
  {
   self->setInitPriority(priority);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInitPriority'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveToSpriteTranspoint of class  SpriteSeer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteSeer_MoveToSpriteTranspoint00
static int tolua_LuaClient_SpriteSeer_MoveToSpriteTranspoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteSeer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteSeer* self = (SpriteSeer*)  tolua_tousertype(tolua_S,1,0);
  bool bNormalTranspoint = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveToSpriteTranspoint'", NULL);
#endif
  {
   self->MoveToSpriteTranspoint(bNormalTranspoint);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveToSpriteTranspoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAnim of class  SpriteSeer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteSeer_SetAnim00
static int tolua_LuaClient_SpriteSeer_SetAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteSeer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteSeer* self = (SpriteSeer*)  tolua_tousertype(tolua_S,1,0);
  int animId = ((int)  tolua_tonumber(tolua_S,2,0));
  int nloop = ((int)  tolua_tonumber(tolua_S,3,0));
  bool isLoop = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAnim'", NULL);
#endif
  {
   self->SetAnim(animId,nloop,isLoop);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEquipItemsData of class  SpriteSeer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteSeer_SetEquipItemsData00
static int tolua_LuaClient_SpriteSeer_SetEquipItemsData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteSeer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteSeer* self = (SpriteSeer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEquipItemsData'", NULL);
#endif
  {
   self->SetEquipItemsData(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEquipItemsData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoot of class  SpriteSeer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteSeer_getRoot00
static int tolua_LuaClient_SpriteSeer_getRoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteSeer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteSeer* self = (SpriteSeer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoot'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getRoot();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsHadFairy of class  SpriteSeer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteSeer_IsHadFairy00
static int tolua_LuaClient_SpriteSeer_IsHadFairy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteSeer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteSeer* self = (SpriteSeer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsHadFairy'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsHadFairy();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHadFairy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsHasFightElf of class  SpriteSeer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteSeer_IsHasFightElf00
static int tolua_LuaClient_SpriteSeer_IsHasFightElf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteSeer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteSeer* self = (SpriteSeer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int pos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsHasFightElf'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsHasFightElf(pos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHasFightElf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_Get00
static int tolua_LuaClient_TipMgr_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TipMgr* tolua_ret = (TipMgr*)  TipMgr::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TipMgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createTip of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_createTip00
static int tolua_LuaClient_TipMgr_createTip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* icon = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCPoint pt = *((CCPoint*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createTip'", NULL);
#endif
  {
   self->createTip(icon,pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createTip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_add00
static int tolua_LuaClient_TipMgr_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
  CCSprite* labelIcon = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  const char* message = ((const char*)  tolua_tostring(tolua_S,3,0));
  ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,4,0));
  int fontsize = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'", NULL);
#endif
  {
   self->add(labelIcon,message,color,fontsize);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_add01
static int tolua_LuaClient_TipMgr_add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
  const char* message = ((const char*)  tolua_tostring(tolua_S,2,0));
  ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,3,0));
  int fontsize = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'", NULL);
#endif
  {
   self->add(message,color,fontsize);
  }
 }
 return 0;
tolua_lerror:
 return tolua_LuaClient_TipMgr_add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: addPart of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_addPart00
static int tolua_LuaClient_TipMgr_addPart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
  const char* message = ((const char*)  tolua_tostring(tolua_S,2,0));
  ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,3,0));
  int fontsize = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addPart'", NULL);
#endif
  {
   HtmlNode* tolua_ret = (HtmlNode*)  self->addPart(message,color,fontsize);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HtmlNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addPart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addPart of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_addPart01
static int tolua_LuaClient_TipMgr_addPart01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"HtmlNode",0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
  HtmlNode* node = ((HtmlNode*)  tolua_tousertype(tolua_S,2,0));
  const char* message = ((const char*)  tolua_tostring(tolua_S,3,0));
  ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,4,0));
  int fontsize = ((int)  tolua_tonumber(tolua_S,5,0));
  bool isFinish = ((bool)  tolua_toboolean(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addPart'", NULL);
#endif
  {
   self->addPart(node,message,color,fontsize,isFinish);
  }
 }
 return 0;
tolua_lerror:
 return tolua_LuaClient_TipMgr_addPart00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: addNodeDone of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_addNodeDone00
static int tolua_LuaClient_TipMgr_addNodeDone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addNodeDone'", NULL);
#endif
  {
   self->addNodeDone();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addNodeDone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroyTip of class  TipMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_TipMgr_destroyTip00
static int tolua_LuaClient_TipMgr_destroyTip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TipMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TipMgr* self = (TipMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyTip'", NULL);
#endif
  {
   self->destroyTip();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyTip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  HeroRoleManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HeroRoleManager_Get00
static int tolua_LuaClient_HeroRoleManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HeroRoleManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   HeroRoleManager* tolua_ret = (HeroRoleManager*)  HeroRoleManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HeroRoleManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createRole of class  HeroRoleManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HeroRoleManager_createRole00
static int tolua_LuaClient_HeroRoleManager_createRole00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HeroRoleManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HeroRoleManager* self = (HeroRoleManager*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID s_id = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  int type = ((int)  tolua_tonumber(tolua_S,4,0));
  bool isPlayerSet = ((bool)  tolua_toboolean(tolua_S,5,0));
  bool isCreateEffect = ((bool)  tolua_toboolean(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createRole'", NULL);
#endif
  {
   RoleBase* tolua_ret = (RoleBase*)  self->createRole(s_id,name,type,isPlayerSet,isCreateEffect);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RoleBase");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createRole'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedManager of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_sharedManager00
static int tolua_LuaClient_XLogicManager_sharedManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XLogicManager* tolua_ret = (XLogicManager*)  XLogicManager::sharedManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XLogicManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reqPlayerAttributes of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_reqPlayerAttributes00
static int tolua_LuaClient_XLogicManager_reqPlayerAttributes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
  int attributeId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reqPlayerAttributes'", NULL);
#endif
  {
   self->reqPlayerAttributes(attributeId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reqPlayerAttributes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LeaveBattle of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_LeaveBattle00
static int tolua_LuaClient_XLogicManager_LeaveBattle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LeaveBattle'", NULL);
#endif
  {
   self->LeaveBattle();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LeaveBattle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHasStarReward of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_setHasStarReward00
static int tolua_LuaClient_XLogicManager_setHasStarReward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHasStarReward'", NULL);
#endif
  {
   self->setHasStarReward(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHasStarReward'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: networkDisConnected of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_networkDisConnected00
static int tolua_LuaClient_XLogicManager_networkDisConnected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'networkDisConnected'", NULL);
#endif
  {
   self->networkDisConnected();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'networkDisConnected'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: notifyShowNextAppearUI of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_notifyShowNextAppearUI00
static int tolua_LuaClient_XLogicManager_notifyShowNextAppearUI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyShowNextAppearUI'", NULL);
#endif
  {
   self->notifyShowNextAppearUI();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyShowNextAppearUI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fromGameBackToLogin of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_fromGameBackToLogin00
static int tolua_LuaClient_XLogicManager_fromGameBackToLogin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fromGameBackToLogin'", NULL);
#endif
  {
   self->fromGameBackToLogin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fromGameBackToLogin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAutoLogin of class  XLogicManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_XLogicManager_getAutoLogin00
static int tolua_LuaClient_XLogicManager_getAutoLogin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XLogicManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XLogicManager* self = (XLogicManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoLogin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getAutoLogin();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoLogin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  LoginManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LoginManager_Get00
static int tolua_LuaClient_LoginManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LoginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LoginManager* tolua_ret = (LoginManager*)  LoginManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LoginManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loginWithUserName of class  LoginManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LoginManager_loginWithUserName00
static int tolua_LuaClient_LoginManager_loginWithUserName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LoginManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LoginManager* self = (LoginManager*)  tolua_tousertype(tolua_S,1,0);
  const char* userName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* password = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loginWithUserName'", NULL);
#endif
  {
   self->loginWithUserName(userName,password);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loginWithUserName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerWithUserName of class  LoginManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LoginManager_registerWithUserName00
static int tolua_LuaClient_LoginManager_registerWithUserName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LoginManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LoginManager* self = (LoginManager*)  tolua_tousertype(tolua_S,1,0);
  const char* userName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* password = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* confirmPassword = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerWithUserName'", NULL);
#endif
  {
   self->registerWithUserName(userName,password,confirmPassword);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerWithUserName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doUserCenter of class  LoginManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LoginManager_doUserCenter00
static int tolua_LuaClient_LoginManager_doUserCenter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LoginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LoginManager* self = (LoginManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doUserCenter'", NULL);
#endif
  {
   self->doUserCenter();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doUserCenter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doRecentLogin of class  LoginManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LoginManager_doRecentLogin00
static int tolua_LuaClient_LoginManager_doRecentLogin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LoginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LoginManager* self = (LoginManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doRecentLogin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->doRecentLogin();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doRecentLogin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMenuItemDispalyState of class  MenuItemExpand */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MenuItemExpand_setMenuItemDispalyState00
static int tolua_LuaClient_MenuItemExpand_setMenuItemDispalyState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MenuItemExpand",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MenuItemExpand* self = (MenuItemExpand*)  tolua_tousertype(tolua_S,1,0);
  int state = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMenuItemDispalyState'", NULL);
#endif
  {
   self->setMenuItemDispalyState(state);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMenuItemDispalyState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEnabled of class  MenuItemExpand */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MenuItemExpand_setEnabled00
static int tolua_LuaClient_MenuItemExpand_setEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MenuItemExpand",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MenuItemExpand* self = (MenuItemExpand*)  tolua_tousertype(tolua_S,1,0);
  bool bEnabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEnabled'", NULL);
#endif
  {
   self->setEnabled(bEnabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  MenuItemExpand */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MenuItemExpand_create00
static int tolua_LuaClient_MenuItemExpand_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MenuItemExpand",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCNode* normalSprite = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  CCNode* selectedSprite = ((CCNode*)  tolua_tousertype(tolua_S,3,0));
  CCNode* disabledSprite = ((CCNode*)  tolua_tousertype(tolua_S,4,NULL));
  {
   MenuItemExpand* tolua_ret = (MenuItemExpand*)  MenuItemExpand::create(normalSprite,selectedSprite,disabledSprite);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MenuItemExpand");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  MenuItemExpand */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MenuItemExpand_create01
static int tolua_LuaClient_MenuItemExpand_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MenuItemExpand",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCObject",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"SEL_MenuHandler",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCNode* normalSprite = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  CCNode* selectedSprite = ((CCNode*)  tolua_tousertype(tolua_S,3,0));
  CCObject* target = ((CCObject*)  tolua_tousertype(tolua_S,4,0));
  SEL_MenuHandler selector = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,5,0));
  {
   MenuItemExpand* tolua_ret = (MenuItemExpand*)  MenuItemExpand::create(normalSprite,selectedSprite,target,selector);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MenuItemExpand");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_MenuItemExpand_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  MenuItemExpand */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MenuItemExpand_create02
static int tolua_LuaClient_MenuItemExpand_create02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MenuItemExpand",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CCObject",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"SEL_MenuHandler",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCNode* normalSprite = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  CCNode* selectedSprite = ((CCNode*)  tolua_tousertype(tolua_S,3,0));
  CCNode* disabledSprite = ((CCNode*)  tolua_tousertype(tolua_S,4,0));
  CCObject* target = ((CCObject*)  tolua_tousertype(tolua_S,5,0));
  SEL_MenuHandler selector = *((SEL_MenuHandler*)  tolua_tousertype(tolua_S,6,0));
  {
   MenuItemExpand* tolua_ret = (MenuItemExpand*)  MenuItemExpand::create(normalSprite,selectedSprite,disabledSprite,target,selector);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MenuItemExpand");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_MenuItemExpand_create01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: sCalcZorder of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_sCalcZorder00
static int tolua_LuaClient_LevelLayer_sCalcZorder00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCPoint pos = *((CCPoint*)  tolua_tousertype(tolua_S,2,0));
  {
   int tolua_ret = (int)  LevelLayer::sCalcZorder(pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sCalcZorder'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sCalcDepth of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_sCalcDepth00
static int tolua_LuaClient_LevelLayer_sCalcDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int z = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  LevelLayer::sCalcDepth(z);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sCalcDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sGetListenerByTag of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_sGetListenerByTag00
static int tolua_LuaClient_LevelLayer_sGetListenerByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   BaseListener* tolua_ret = (BaseListener*)  LevelLayer::sGetListenerByTag(tag);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BaseListener");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sGetListenerByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSpriteSeer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getSpriteSeer00
static int tolua_LuaClient_LevelLayer_getSpriteSeer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSpriteSeer'", NULL);
#endif
  {
   SpriteSeer* tolua_ret = (SpriteSeer*)  self->getSpriteSeer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteSeer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSpriteSeer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFollowSprite of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getFollowSprite00
static int tolua_LuaClient_LevelLayer_getFollowSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFollowSprite'", NULL);
#endif
  {
   SpriteElf* tolua_ret = (SpriteElf*)  self->getFollowSprite();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteElf");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFollowSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getObjectCount of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getObjectCount00
static int tolua_LuaClient_LevelLayer_getObjectCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getObjectCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getObjectCount(tag);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getObjectCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRandomPoint of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getRandomPoint00
static int tolua_LuaClient_LevelLayer_getRandomPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRandomPoint'", NULL);
#endif
  {
   CCPoint tolua_ret = (CCPoint)  self->getRandomPoint();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRandomPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSeerBornPoint of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getSeerBornPoint00
static int tolua_LuaClient_LevelLayer_getSeerBornPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"sActiveLevelConfig",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  sActiveLevelConfig* config = ((sActiveLevelConfig*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSeerBornPoint'", NULL);
#endif
  {
   CCPoint tolua_ret = (CCPoint)  self->getSeerBornPoint(*config);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSeerBornPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getID of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getID00
static int tolua_LuaClient_LevelLayer_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPhysicsLayer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getPhysicsLayer00
static int tolua_LuaClient_LevelLayer_getPhysicsLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPhysicsLayer'", NULL);
#endif
  {
   CPhysicsLayer* tolua_ret = (CPhysicsLayer*)  self->getPhysicsLayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CPhysicsLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPhysicsLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetData of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_GetData00
static int tolua_LuaClient_LevelLayer_GetData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetData'", NULL);
#endif
  {
   sLevelData* tolua_ret = (sLevelData*)  self->GetData();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"sLevelData");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMapSizeInPixel of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getMapSizeInPixel00
static int tolua_LuaClient_LevelLayer_getMapSizeInPixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMapSizeInPixel'", NULL);
#endif
  {
   CCSize tolua_ret = (CCSize)  self->getMapSizeInPixel();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCSize)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCSize));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMapSizeInPixel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMapTileSizeInPixel of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getMapTileSizeInPixel00
static int tolua_LuaClient_LevelLayer_getMapTileSizeInPixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMapTileSizeInPixel'", NULL);
#endif
  {
   CCSize tolua_ret = (CCSize)  self->getMapTileSizeInPixel();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCSize)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCSize));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMapTileSizeInPixel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMapRect of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getMapRect00
static int tolua_LuaClient_LevelLayer_getMapRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMapRect'", NULL);
#endif
  {
   CCRect tolua_ret = (CCRect)  self->getMapRect();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCRect)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCRect");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCRect));
     tolua_pushusertype(tolua_S,tolua_obj,"CCRect");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMapRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getObjectLayer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getObjectLayer00
static int tolua_LuaClient_LevelLayer_getObjectLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getObjectLayer'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getObjectLayer();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getObjectLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMainLandTranspoint of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getMainLandTranspoint00
static int tolua_LuaClient_LevelLayer_getMainLandTranspoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMainLandTranspoint'", NULL);
#endif
  {
   SpriteTransPoint* tolua_ret = (SpriteTransPoint*)  self->getMainLandTranspoint();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteTransPoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainLandTranspoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEliteTranspoint of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getEliteTranspoint00
static int tolua_LuaClient_LevelLayer_getEliteTranspoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEliteTranspoint'", NULL);
#endif
  {
   SpriteTransPoint* tolua_ret = (SpriteTransPoint*)  self->getEliteTranspoint();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteTransPoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEliteTranspoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addOtherPlayer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_addOtherPlayer00
static int tolua_LuaClient_LevelLayer_addOtherPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"cocos2d::CCPoint",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID uid = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  const char* userName = ((const char*)  tolua_tostring(tolua_S,3,0));
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  cocos2d::CCPoint pt = *((cocos2d::CCPoint*)  tolua_tousertype(tolua_S,5,0));
  unsigned int orient = ((unsigned int)  tolua_tonumber(tolua_S,6,0));
  unsigned int battleSide = ((unsigned int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addOtherPlayer'", NULL);
#endif
  {
   self->addOtherPlayer(uid,userName,type,pt,orient,battleSide);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addOtherPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeOtherPlayer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_removeOtherPlayer00
static int tolua_LuaClient_LevelLayer_removeOtherPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID uid = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeOtherPlayer'", NULL);
#endif
  {
   self->removeOtherPlayer(uid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeOtherPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateOtherPlayer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_updateOtherPlayer00
static int tolua_LuaClient_LevelLayer_updateOtherPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"cocos2d::CCPoint",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID uid = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  cocos2d::CCPoint pt = *((cocos2d::CCPoint*)  tolua_tousertype(tolua_S,3,0));
  unsigned int animID = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  bool bFlip = ((bool)  tolua_toboolean(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateOtherPlayer'", NULL);
#endif
  {
   self->updateOtherPlayer(uid,pt,animID,bFlip);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateOtherPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DoOtherPlayerSkill of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_DoOtherPlayerSkill00
static int tolua_LuaClient_LevelLayer_DoOtherPlayerSkill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"const CCPoint",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID uid = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  unsigned int skillID = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  const CCPoint* rolePos = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCPoint* roleDir = ((const CCPoint*)  tolua_tousertype(tolua_S,5,0));
  const CCPoint* aimDir = ((const CCPoint*)  tolua_tousertype(tolua_S,6,0));
  const CCPoint* monsterPos = ((const CCPoint*)  tolua_tousertype(tolua_S,7,0));
  bool bAnimMonster = ((bool)  tolua_toboolean(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DoOtherPlayerSkill'", NULL);
#endif
  {
   self->DoOtherPlayerSkill(uid,skillID,*rolePos,*roleDir,*aimDir,*monsterPos,bAnimMonster);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DoOtherPlayerSkill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DoElfSkill of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_DoElfSkill00
static int tolua_LuaClient_LevelLayer_DoElfSkill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"const CCPoint",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID uid = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
  unsigned int skillID = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  const CCPoint* rolePos = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCPoint* roleDir = ((const CCPoint*)  tolua_tousertype(tolua_S,5,0));
  const CCPoint* aimDir = ((const CCPoint*)  tolua_tousertype(tolua_S,6,0));
  const CCPoint* monsterPos = ((const CCPoint*)  tolua_tousertype(tolua_S,7,0));
  bool bAnimMonster = ((bool)  tolua_toboolean(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DoElfSkill'", NULL);
#endif
  {
   self->DoElfSkill(uid,skillID,*rolePos,*roleDir,*aimDir,*monsterPos,bAnimMonster);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DoElfSkill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlayer of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_getPlayer00
static int tolua_LuaClient_LevelLayer_getPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"PLAYER_ID",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  PLAYER_ID uid = *((PLAYER_ID*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlayer'", NULL);
#endif
  {
   SpriteSeer* tolua_ret = (SpriteSeer*)  self->getPlayer(uid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteSeer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EquipOnePlayerWeapon of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_EquipOnePlayerWeapon00
static int tolua_LuaClient_LevelLayer_EquipOnePlayerWeapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  int playerId = ((int)  tolua_tonumber(tolua_S,2,0));
  int weaponId = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EquipOnePlayerWeapon'", NULL);
#endif
  {
   self->EquipOnePlayerWeapon(playerId,weaponId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EquipOnePlayerWeapon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addMonster of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_addMonster00
static int tolua_LuaClient_LevelLayer_addMonster00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"MonsterInitInfo",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  MonsterInitInfo info = *((MonsterInitInfo*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addMonster'", NULL);
#endif
  {
   self->addMonster(info);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addMonster'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeMonster of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_removeMonster00
static int tolua_LuaClient_LevelLayer_removeMonster00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int uid = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeMonster'", NULL);
#endif
  {
   self->removeMonster(uid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeMonster'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowMonsterOrNot of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_ShowMonsterOrNot00
static int tolua_LuaClient_LevelLayer_ShowMonsterOrNot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  bool bShow = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowMonsterOrNot'", NULL);
#endif
  {
   self->ShowMonsterOrNot(bShow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowMonsterOrNot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showTouchEffect of class  LevelLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_LevelLayer_showTouchEffect00
static int tolua_LuaClient_LevelLayer_showTouchEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LevelLayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LevelLayer* self = (LevelLayer*)  tolua_tousertype(tolua_S,1,0);
  CCPoint point = *((CCPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showTouchEffect'", NULL);
#endif
  {
   self->showTouchEffect(point);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showTouchEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  FeedbackLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_FeedbackLayer_Get00
static int tolua_LuaClient_FeedbackLayer_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"FeedbackLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   FeedbackLayer* tolua_ret = (FeedbackLayer*)  FeedbackLayer::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"FeedbackLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callbackHotSpot of class  FeedbackLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_FeedbackLayer_callbackHotSpot00
static int tolua_LuaClient_FeedbackLayer_callbackHotSpot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FeedbackLayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FeedbackLayer* self = (FeedbackLayer*)  tolua_tousertype(tolua_S,1,0);
  CCObject* sender = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callbackHotSpot'", NULL);
#endif
  {
   self->callbackHotSpot(sender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callbackHotSpot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callbackOnline of class  FeedbackLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_FeedbackLayer_callbackOnline00
static int tolua_LuaClient_FeedbackLayer_callbackOnline00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FeedbackLayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FeedbackLayer* self = (FeedbackLayer*)  tolua_tousertype(tolua_S,1,0);
  CCObject* sender = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callbackOnline'", NULL);
#endif
  {
   self->callbackOnline(sender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callbackOnline'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callbackForum of class  FeedbackLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_FeedbackLayer_callbackForum00
static int tolua_LuaClient_FeedbackLayer_callbackForum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FeedbackLayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FeedbackLayer* self = (FeedbackLayer*)  tolua_tousertype(tolua_S,1,0);
  CCObject* sender = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callbackForum'", NULL);
#endif
  {
   self->callbackForum(sender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callbackForum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: callbackWeb of class  FeedbackLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_FeedbackLayer_callbackWeb00
static int tolua_LuaClient_FeedbackLayer_callbackWeb00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FeedbackLayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FeedbackLayer* self = (FeedbackLayer*)  tolua_tousertype(tolua_S,1,0);
  char* url = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'callbackWeb'", NULL);
#endif
  {
   self->callbackWeb(url);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'callbackWeb'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  HtmlNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HtmlNode_create00
static int tolua_LuaClient_HtmlNode_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HtmlNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float nodeWidth = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   HtmlNode* tolua_ret = (HtmlNode*)  HtmlNode::create(nodeWidth);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HtmlNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: begin of class  HtmlNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HtmlNode_begin00
static int tolua_LuaClient_HtmlNode_begin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HtmlNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HtmlNode* self = (HtmlNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'begin'", NULL);
#endif
  {
   self->begin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'begin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addLabel of class  HtmlNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HtmlNode_addLabel00
static int tolua_LuaClient_HtmlNode_addLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HtmlNode",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HtmlNode* self = (HtmlNode*)  tolua_tousertype(tolua_S,1,0);
  const char* strContent = ((const char*)  tolua_tostring(tolua_S,2,0));
  int fontsize = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* fontname = ((const char*)  tolua_tostring(tolua_S,4,0));
  ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addLabel'", NULL);
#endif
  {
   self->addLabel(strContent,fontsize,fontname,color);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addImage of class  HtmlNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HtmlNode_addImage00
static int tolua_LuaClient_HtmlNode_addImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HtmlNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cocos2d::CCSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HtmlNode* self = (HtmlNode*)  tolua_tousertype(tolua_S,1,0);
  cocos2d::CCSprite* sprite = ((cocos2d::CCSprite*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addImage'", NULL);
#endif
  {
   self->addImage(sprite);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addHyperLink of class  HtmlNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HtmlNode_addHyperLink00
static int tolua_LuaClient_HtmlNode_addHyperLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HtmlNode",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"ccColor3B",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !toluafix_isfunction(tolua_S,6,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HtmlNode* self = (HtmlNode*)  tolua_tousertype(tolua_S,1,0);
  const char* strContent = ((const char*)  tolua_tostring(tolua_S,2,0));
  int fontsize = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* fontname = ((const char*)  tolua_tostring(tolua_S,4,0));
  ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,5,0));
  LUA_FUNCTION touchLuaFunc = (  toluafix_ref_function(tolua_S,6,0));
  int priority = ((int)  tolua_tonumber(tolua_S,7,0));
  bool isButtonEnable = ((bool)  tolua_toboolean(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addHyperLink'", NULL);
#endif
  {
   int tolua_ret = (int)  self->addHyperLink(strContent,fontsize,fontname,color,touchLuaFunc,priority,isButtonEnable);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addHyperLink'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: finish of class  HtmlNode */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HtmlNode_finish00
static int tolua_LuaClient_HtmlNode_finish00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HtmlNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HtmlNode* self = (HtmlNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'finish'", NULL);
#endif
  {
   self->finish();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'finish'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  SpriteMonsterMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteMonsterMgr_Get00
static int tolua_LuaClient_SpriteMonsterMgr_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpriteMonsterMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SpriteMonsterMgr* tolua_ret = (SpriteMonsterMgr*)  SpriteMonsterMgr::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpriteMonsterMgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateCacheMonster of class  SpriteMonsterMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteMonsterMgr_CreateCacheMonster00
static int tolua_LuaClient_SpriteMonsterMgr_CreateCacheMonster00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteMonsterMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteMonsterMgr* self = (SpriteMonsterMgr*)  tolua_tousertype(tolua_S,1,0);
  int typeId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateCacheMonster'", NULL);
#endif
  {
   self->CreateCacheMonster(typeId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateCacheMonster'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateSpecialMonsterHPLayer of class  SpriteMonsterMgr */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SpriteMonsterMgr_updateSpecialMonsterHPLayer00
static int tolua_LuaClient_SpriteMonsterMgr_updateSpecialMonsterHPLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpriteMonsterMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpriteMonsterMgr* self = (SpriteMonsterMgr*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  int nowHp = ((int)  tolua_tonumber(tolua_S,3,0));
  int totalHp = ((int)  tolua_tonumber(tolua_S,4,0));
  int type = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateSpecialMonsterHPLayer'", NULL);
#endif
  {
   self->updateSpecialMonsterHPLayer(id,nowHp,totalHp,type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateSpecialMonsterHPLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedJoystick of class  HSJoystick */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HSJoystick_sharedJoystick00
static int tolua_LuaClient_HSJoystick_sharedJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HSJoystick",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   HSJoystick* tolua_ret = (HSJoystick*)  HSJoystick::sharedJoystick();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HSJoystick");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIsEnable of class  HSJoystick */
#ifndef TOLUA_DISABLE_tolua_LuaClient_HSJoystick_setIsEnable00
static int tolua_LuaClient_HSJoystick_setIsEnable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSJoystick",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSJoystick* self = (HSJoystick*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIsEnable'", NULL);
#endif
  {
   self->setIsEnable(enable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIsEnable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  GameDataManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameDataManager_Get00
static int tolua_LuaClient_GameDataManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameDataManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameDataManager* tolua_ret = (GameDataManager*)  GameDataManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameDataManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHeroAutoAttack of class  GameDataManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameDataManager_setHeroAutoAttack00
static int tolua_LuaClient_GameDataManager_setHeroAutoAttack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameDataManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameDataManager* self = (GameDataManager*)  tolua_tousertype(tolua_S,1,0);
  bool autoAttack = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHeroAutoAttack'", NULL);
#endif
  {
   self->setHeroAutoAttack(autoAttack);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHeroAutoAttack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: product_id of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_unsigned_product_id
static int tolua_get_OneShopItemInfo_unsigned_product_id(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->product_id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: product_id of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_unsigned_product_id
static int tolua_set_OneShopItemInfo_unsigned_product_id(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->product_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: item_id of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_unsigned_item_id
static int tolua_get_OneShopItemInfo_unsigned_item_id(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->item_id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: item_id of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_unsigned_item_id
static int tolua_set_OneShopItemInfo_unsigned_item_id(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->item_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: item_base_num of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_unsigned_item_base_num
static int tolua_get_OneShopItemInfo_unsigned_item_base_num(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_base_num'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->item_base_num);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: item_base_num of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_unsigned_item_base_num
static int tolua_set_OneShopItemInfo_unsigned_item_base_num(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_base_num'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->item_base_num = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: item_gift_num of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_unsigned_item_gift_num
static int tolua_get_OneShopItemInfo_unsigned_item_gift_num(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_gift_num'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->item_gift_num);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: item_gift_num of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_unsigned_item_gift_num
static int tolua_set_OneShopItemInfo_unsigned_item_gift_num(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_gift_num'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->item_gift_num = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: product_name of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_product_name
static int tolua_get_OneShopItemInfo_product_name(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->product_name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: product_name of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_product_name
static int tolua_set_OneShopItemInfo_product_name(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->product_name = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: product_price of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_unsigned_product_price
static int tolua_get_OneShopItemInfo_unsigned_product_price(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_price'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->product_price);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: product_price of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_unsigned_product_price
static int tolua_set_OneShopItemInfo_unsigned_product_price(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_price'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->product_price = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: product_orig_price of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_get_OneShopItemInfo_unsigned_product_orig_price
static int tolua_get_OneShopItemInfo_unsigned_product_orig_price(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_orig_price'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->product_orig_price);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: product_orig_price of class  OneShopItemInfo */
#ifndef TOLUA_DISABLE_tolua_set_OneShopItemInfo_unsigned_product_orig_price
static int tolua_set_OneShopItemInfo_unsigned_product_orig_price(lua_State* tolua_S)
{
  OneShopItemInfo* self = (OneShopItemInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'product_orig_price'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->product_orig_price = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedInstance of class  ShopItemInfoFromServer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ShopItemInfoFromServer_sharedInstance00
static int tolua_LuaClient_ShopItemInfoFromServer_sharedInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShopItemInfoFromServer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ShopItemInfoFromServer* tolua_ret = (ShopItemInfoFromServer*)  ShopItemInfoFromServer::sharedInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ShopItemInfoFromServer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOneShopItemInfo of class  ShopItemInfoFromServer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ShopItemInfoFromServer_GetOneShopItemInfo00
static int tolua_LuaClient_ShopItemInfoFromServer_GetOneShopItemInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ShopItemInfoFromServer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ShopItemInfoFromServer* self = (ShopItemInfoFromServer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int productId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOneShopItemInfo'", NULL);
#endif
  {
   OneShopItemInfo tolua_ret = (OneShopItemInfo)  self->GetOneShopItemInfo(productId);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((OneShopItemInfo)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"OneShopItemInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(OneShopItemInfo));
     tolua_pushusertype(tolua_S,tolua_obj,"OneShopItemInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOneShopItemInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PrepareNeedDataForLua of class  ShopItemInfoFromServer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_ShopItemInfoFromServer_PrepareNeedDataForLua00
static int tolua_LuaClient_ShopItemInfoFromServer_PrepareNeedDataForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ShopItemInfoFromServer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ShopItemInfoFromServer* self = (ShopItemInfoFromServer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PrepareNeedDataForLua'", NULL);
#endif
  {
   self->PrepareNeedDataForLua();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PrepareNeedDataForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  SystemConditionManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SystemConditionManager_Get00
static int tolua_LuaClient_SystemConditionManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SystemConditionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SystemConditionManager* tolua_ret = (SystemConditionManager*)  SystemConditionManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SystemConditionManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTaskStatus of class  SystemConditionManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SystemConditionManager_GetTaskStatus00
static int tolua_LuaClient_SystemConditionManager_GetTaskStatus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SystemConditionManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SystemConditionManager* self = (SystemConditionManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int task_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTaskStatus'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTaskStatus(task_id);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTaskStatus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  PayManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PayManager_Get00
static int tolua_LuaClient_PayManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PayManager* tolua_ret = (PayManager*)  PayManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PayManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadPlugin of class  PayManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PayManager_loadPlugin00
static int tolua_LuaClient_PayManager_loadPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PayManager* self = (PayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadPlugin'", NULL);
#endif
  {
   self->loadPlugin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unloadPlugin of class  PayManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PayManager_unloadPlugin00
static int tolua_LuaClient_PayManager_unloadPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PayManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PayManager* self = (PayManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadPlugin'", NULL);
#endif
  {
   self->unloadPlugin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unloadPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: payForProduct of class  PayManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_PayManager_payForProduct00
static int tolua_LuaClient_PayManager_payForProduct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PayManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PayManager* self = (PayManager*)  tolua_tousertype(tolua_S,1,0);
  int tolua_var_2 = ((int)  tolua_tonumber(tolua_S,2,0));
  int tolua_var_3 = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'payForProduct'", NULL);
#endif
  {
   self->payForProduct(tolua_var_2,tolua_var_3);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'payForProduct'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  UIControlAppearControler */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIControlAppearControler_Get00
static int tolua_LuaClient_UIControlAppearControler_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"UIControlAppearControler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   UIControlAppearControler* tolua_ret = (UIControlAppearControler*)  UIControlAppearControler::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIControlAppearControler");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InsertOneTutorialAndTask of class  UIControlAppearControler */
#ifndef TOLUA_DISABLE_tolua_LuaClient_UIControlAppearControler_InsertOneTutorialAndTask00
static int tolua_LuaClient_UIControlAppearControler_InsertOneTutorialAndTask00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIControlAppearControler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIControlAppearControler* self = (UIControlAppearControler*)  tolua_tousertype(tolua_S,1,0);
  unsigned int tutorialId = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int taskId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InsertOneTutorialAndTask'", NULL);
#endif
  {
   self->InsertOneTutorialAndTask(tutorialId,taskId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InsertOneTutorialAndTask'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  WaitingLayerManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_WaitingLayerManager_Get00
static int tolua_LuaClient_WaitingLayerManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"WaitingLayerManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   WaitingLayerManager* tolua_ret = (WaitingLayerManager*)  WaitingLayerManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"WaitingLayerManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PushOneMessage of class  WaitingLayerManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_WaitingLayerManager_PushOneMessage00
static int tolua_LuaClient_WaitingLayerManager_PushOneMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WaitingLayerManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WaitingLayerManager* self = (WaitingLayerManager*)  tolua_tousertype(tolua_S,1,0);
  const char* pMessage = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PushOneMessage'", NULL);
#endif
  {
   self->PushOneMessage(pMessage);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PushOneMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearMessages of class  WaitingLayerManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_WaitingLayerManager_clearMessages00
static int tolua_LuaClient_WaitingLayerManager_clearMessages00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WaitingLayerManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WaitingLayerManager* self = (WaitingLayerManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearMessages'", NULL);
#endif
  {
   self->clearMessages();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearMessages'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: superFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_superFontSize00
static int tolua_LuaClient_GameFontManager_superFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::superFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'superFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: largeFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_largeFontSize00
static int tolua_LuaClient_GameFontManager_largeFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::largeFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'largeFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: smallFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_smallFontSize00
static int tolua_LuaClient_GameFontManager_smallFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::smallFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'smallFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: miniFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_miniFontSize00
static int tolua_LuaClient_GameFontManager_miniFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::miniFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'miniFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createPlayerLabelFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_createPlayerLabelFontSize00
static int tolua_LuaClient_GameFontManager_createPlayerLabelFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::createPlayerLabelFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createPlayerLabelFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: smallTitleFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_smallTitleFontSize00
static int tolua_LuaClient_GameFontManager_smallTitleFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::smallTitleFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'smallTitleFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: smallTipFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_smallTipFontSize00
static int tolua_LuaClient_GameFontManager_smallTipFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::smallTipFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'smallTipFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: captureItemTipNameFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_captureItemTipNameFontSize00
static int tolua_LuaClient_GameFontManager_captureItemTipNameFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::captureItemTipNameFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'captureItemTipNameFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: captureItemTipCountFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_captureItemTipCountFontSize00
static int tolua_LuaClient_GameFontManager_captureItemTipCountFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::captureItemTipCountFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'captureItemTipCountFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: captureLayerFontSize of class  GameFontManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_GameFontManager_captureLayerFontSize00
static int tolua_LuaClient_GameFontManager_captureLayerFontSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameFontManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GameFontManager::captureLayerFontSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'captureLayerFontSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  CallNativeFuncManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CallNativeFuncManager_getInstance00
static int tolua_LuaClient_CallNativeFuncManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CallNativeFuncManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CallNativeFuncManager* tolua_ret = (CallNativeFuncManager*)  CallNativeFuncManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CallNativeFuncManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddIndexLayerToMainScene of class  CallNativeFuncManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CallNativeFuncManager_AddIndexLayerToMainScene00
static int tolua_LuaClient_CallNativeFuncManager_AddIndexLayerToMainScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CallNativeFuncManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CallNativeFuncManager* self = (CallNativeFuncManager*)  tolua_tousertype(tolua_S,1,0);
  int whatLayer = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddIndexLayerToMainScene'", NULL);
#endif
  {
   self->AddIndexLayerToMainScene(whatLayer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddIndexLayerToMainScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRankLayerVipFlgVisible of class  CallNativeFuncManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CallNativeFuncManager_SetRankLayerVipFlgVisible00
static int tolua_LuaClient_CallNativeFuncManager_SetRankLayerVipFlgVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CallNativeFuncManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CallNativeFuncManager* self = (CallNativeFuncManager*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRankLayerVipFlgVisible'", NULL);
#endif
  {
   self->SetRankLayerVipFlgVisible(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRankLayerVipFlgVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowCombatSettingLayer of class  CallNativeFuncManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CallNativeFuncManager_ShowCombatSettingLayer00
static int tolua_LuaClient_CallNativeFuncManager_ShowCombatSettingLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CallNativeFuncManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CallNativeFuncManager* self = (CallNativeFuncManager*)  tolua_tousertype(tolua_S,1,0);
  bool flag = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowCombatSettingLayer'", NULL);
#endif
  {
   self->ShowCombatSettingLayer(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowCombatSettingLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AdjustStyle of class  MessageBox */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MessageBox_AdjustStyle00
static int tolua_LuaClient_MessageBox_AdjustStyle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MessageBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string okBtnContent = ((string)  tolua_tocppstring(tolua_S,2,0));
  string cancelBtnContent = ((string)  tolua_tocppstring(tolua_S,3,0));
  {
   MessageBox::AdjustStyle(okBtnContent,cancelBtnContent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AdjustStyle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: card_id of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_get_MonthCardInfo_unsigned_card_id
static int tolua_get_MonthCardInfo_unsigned_card_id(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'card_id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->card_id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: card_id of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_set_MonthCardInfo_unsigned_card_id
static int tolua_set_MonthCardInfo_unsigned_card_id(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'card_id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->card_id = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: is_received of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_get_MonthCardInfo_is_received
static int tolua_get_MonthCardInfo_is_received(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'is_received'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->is_received);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: is_received of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_set_MonthCardInfo_is_received
static int tolua_set_MonthCardInfo_is_received(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'is_received'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->is_received = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: day_count of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_get_MonthCardInfo_unsigned_day_count
static int tolua_get_MonthCardInfo_unsigned_day_count(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'day_count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->day_count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: day_count of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_set_MonthCardInfo_unsigned_day_count
static int tolua_set_MonthCardInfo_unsigned_day_count(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'day_count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->day_count = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: diamond_amount of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_get_MonthCardInfo_unsigned_diamond_amount
static int tolua_get_MonthCardInfo_unsigned_diamond_amount(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'diamond_amount'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->diamond_amount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: diamond_amount of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_set_MonthCardInfo_unsigned_diamond_amount
static int tolua_set_MonthCardInfo_unsigned_diamond_amount(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'diamond_amount'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->diamond_amount = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: is_alarm of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_get_MonthCardInfo_is_alarm
static int tolua_get_MonthCardInfo_is_alarm(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'is_alarm'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->is_alarm);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: is_alarm of class  MonthCardInfo */
#ifndef TOLUA_DISABLE_tolua_set_MonthCardInfo_is_alarm
static int tolua_set_MonthCardInfo_is_alarm(lua_State* tolua_S)
{
  MonthCardInfo* self = (MonthCardInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'is_alarm'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->is_alarm = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  MonthCardManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MonthCardManager_Get00
static int tolua_LuaClient_MonthCardManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MonthCardManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MonthCardManager* tolua_ret = (MonthCardManager*)  MonthCardManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MonthCardManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasMonthCard of class  MonthCardManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MonthCardManager_hasMonthCard00
static int tolua_LuaClient_MonthCardManager_hasMonthCard00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MonthCardManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MonthCardManager* self = (MonthCardManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasMonthCard'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasMonthCard();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasMonthCard'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOneMonthCard of class  MonthCardManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_MonthCardManager_getOneMonthCard00
static int tolua_LuaClient_MonthCardManager_getOneMonthCard00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MonthCardManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MonthCardManager* self = (MonthCardManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOneMonthCard'", NULL);
#endif
  {
   MonthCardInfo tolua_ret = (MonthCardInfo)  self->getOneMonthCard();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((MonthCardInfo)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"MonthCardInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(MonthCardInfo));
     tolua_pushusertype(tolua_S,tolua_obj,"MonthCardInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOneMonthCard'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  SkeletonAnimRcsManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkeletonAnimRcsManager_getInstance00
static int tolua_LuaClient_SkeletonAnimRcsManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SkeletonAnimRcsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SkeletonAnimRcsManager* tolua_ret = (SkeletonAnimRcsManager*)  SkeletonAnimRcsManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SkeletonAnimRcsManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadOneRoleRcsOnly of class  SkeletonAnimRcsManager */
#ifndef TOLUA_DISABLE_tolua_LuaClient_SkeletonAnimRcsManager_LoadOneRoleRcsOnly00
static int tolua_LuaClient_SkeletonAnimRcsManager_LoadOneRoleRcsOnly00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimRcsManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimRcsManager* self = (SkeletonAnimRcsManager*)  tolua_tousertype(tolua_S,1,0);
  int roleId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadOneRoleRcsOnly'", NULL);
#endif
  {
   self->LoadOneRoleRcsOnly(roleId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadOneRoleRcsOnly'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTipContentColor of class  DiamondWarningLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_DiamondWarningLayer_SetTipContentColor00
static int tolua_LuaClient_DiamondWarningLayer_SetTipContentColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DiamondWarningLayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* tipContent = ((const char*)  tolua_tostring(tolua_S,2,0));
  ccColor3B tipColor = *((ccColor3B*)  tolua_tousertype(tolua_S,3,0));
  {
   DiamondWarningLayer::SetTipContentColor(tipContent,tipColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTipContentColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openCityList of class  CityListLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_CityListLayer_openCityList00
static int tolua_LuaClient_CityListLayer_openCityList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CityListLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CityListLayer* self = (CityListLayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openCityList'", NULL);
#endif
  {
   self->openCityList();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openCityList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: id of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_get_InstanceInfo_unsigned_id
static int tolua_get_InstanceInfo_unsigned_id(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_set_InstanceInfo_unsigned_id
static int tolua_set_InstanceInfo_unsigned_id(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: star of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_get_InstanceInfo_unsigned_star
static int tolua_get_InstanceInfo_unsigned_star(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'star'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->star);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: star of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_set_InstanceInfo_unsigned_star
static int tolua_set_InstanceInfo_unsigned_star(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'star'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->star = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: can_enter_flag of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_get_InstanceInfo_can_enter_flag
static int tolua_get_InstanceInfo_can_enter_flag(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'can_enter_flag'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->can_enter_flag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: can_enter_flag of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_set_InstanceInfo_can_enter_flag
static int tolua_set_InstanceInfo_can_enter_flag(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'can_enter_flag'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->can_enter_flag = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: refresh_times of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_get_InstanceInfo_unsigned_refresh_times
static int tolua_get_InstanceInfo_unsigned_refresh_times(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'refresh_times'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->refresh_times);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: refresh_times of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_set_InstanceInfo_unsigned_refresh_times
static int tolua_set_InstanceInfo_unsigned_refresh_times(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'refresh_times'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->refresh_times = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isgray of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_get_InstanceInfo_isgray
static int tolua_get_InstanceInfo_isgray(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isgray'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isgray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isgray of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_set_InstanceInfo_isgray
static int tolua_set_InstanceInfo_isgray(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isgray'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isgray = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: battle_times of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_get_InstanceInfo_unsigned_battle_times
static int tolua_get_InstanceInfo_unsigned_battle_times(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'battle_times'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->battle_times);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: battle_times of class  InstanceInfo */
#ifndef TOLUA_DISABLE_tolua_set_InstanceInfo_unsigned_battle_times
static int tolua_set_InstanceInfo_unsigned_battle_times(lua_State* tolua_S)
{
  InstanceInfo* self = (InstanceInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'battle_times'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->battle_times = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: showInstanceInfo of class  InstanceListLayer */
#ifndef TOLUA_DISABLE_tolua_LuaClient_InstanceListLayer_showInstanceInfo00
static int tolua_LuaClient_InstanceListLayer_showInstanceInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"InstanceListLayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCNode* parent = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  unsigned int instanceId = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int instanceType = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  CCPoint pt = *((CCPoint*)  tolua_tousertype(tolua_S,5,0));
  {
   InstanceListLayer::showInstanceInfo(parent,instanceId,instanceType,pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showInstanceInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: graylightWithCCSprite */
#ifndef TOLUA_DISABLE_tolua_LuaClient_graylightWithCCSprite01
static int tolua_LuaClient_graylightWithCCSprite01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCSprite",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCSprite* oldSprite = ((CCSprite*)  tolua_tousertype(tolua_S,1,0));
  bool isLight = ((bool)  tolua_toboolean(tolua_S,2,0));
  {
   CCSprite* tolua_ret = (CCSprite*)  graylightWithCCSprite(oldSprite,isLight);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaClient_graylightWithCCSprite00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LuaClient_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"SOUND_EFFECT_WALK",SOUND_EFFECT_WALK);
  tolua_constant(tolua_S,"SOUND_EFFECT_WALK_ASSASSIN",SOUND_EFFECT_WALK_ASSASSIN);
  tolua_constant(tolua_S,"SOUND_EFFECT_CLICK",SOUND_EFFECT_CLICK);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CCClippingNode","CCClippingNode","CCNode",tolua_collect_CCClippingNode);
  #else
  tolua_cclass(tolua_S,"CCClippingNode","CCClippingNode","CCNode",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CCClippingNode");
   tolua_function(tolua_S,"create",tolua_LuaClient_CCClippingNode_create00);
   tolua_function(tolua_S,"create",tolua_LuaClient_CCClippingNode_create01);
   tolua_function(tolua_S,"delete",tolua_LuaClient_CCClippingNode_delete00);
   tolua_function(tolua_S,"init",tolua_LuaClient_CCClippingNode_init00);
   tolua_function(tolua_S,"init",tolua_LuaClient_CCClippingNode_init01);
   tolua_function(tolua_S,"onEnter",tolua_LuaClient_CCClippingNode_onEnter00);
   tolua_function(tolua_S,"onEnterTransitionDidFinish",tolua_LuaClient_CCClippingNode_onEnterTransitionDidFinish00);
   tolua_function(tolua_S,"onExitTransitionDidStart",tolua_LuaClient_CCClippingNode_onExitTransitionDidStart00);
   tolua_function(tolua_S,"onExit",tolua_LuaClient_CCClippingNode_onExit00);
   tolua_function(tolua_S,"visit",tolua_LuaClient_CCClippingNode_visit00);
   tolua_function(tolua_S,"getStencil",tolua_LuaClient_CCClippingNode_getStencil00);
   tolua_function(tolua_S,"setStencil",tolua_LuaClient_CCClippingNode_setStencil00);
   tolua_function(tolua_S,"getAlphaThreshold",tolua_LuaClient_CCClippingNode_getAlphaThreshold00);
   tolua_function(tolua_S,"setAlphaThreshold",tolua_LuaClient_CCClippingNode_setAlphaThreshold00);
   tolua_function(tolua_S,"isInverted",tolua_LuaClient_CCClippingNode_isInverted00);
   tolua_function(tolua_S,"setInverted",tolua_LuaClient_CCClippingNode_setInverted00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SpriteFactory","SpriteFactory","",NULL);
  tolua_beginmodule(tolua_S,"SpriteFactory");
   tolua_function(tolua_S,"PrintLog",tolua_LuaClient_SpriteFactory_PrintLog00);
   tolua_function(tolua_S,"sharedFactory",tolua_LuaClient_SpriteFactory_sharedFactory00);
   tolua_function(tolua_S,"GetValue",tolua_LuaClient_SpriteFactory_GetValue00);
   tolua_function(tolua_S,"ShowElfOnLayerAtPoint",tolua_LuaClient_SpriteFactory_ShowElfOnLayerAtPoint00);
   tolua_function(tolua_S,"RemoveElfFromHero",tolua_LuaClient_SpriteFactory_RemoveElfFromHero00);
   tolua_function(tolua_S,"AttachElfToOneHero",tolua_LuaClient_SpriteFactory_AttachElfToOneHero00);
   tolua_function(tolua_S,"GetMainMenuLayerNode",tolua_LuaClient_SpriteFactory_GetMainMenuLayerNode00);
   tolua_function(tolua_S,"ResetValue",tolua_LuaClient_SpriteFactory_ResetValue00);
   tolua_function(tolua_S,"ShowElfPanelWithBackPackLayer",tolua_LuaClient_SpriteFactory_ShowElfPanelWithBackPackLayer00);
   tolua_function(tolua_S,"SendElfAwakeMessage",tolua_LuaClient_SpriteFactory_SendElfAwakeMessage00);
   tolua_function(tolua_S,"SendAwakeConfigMessage",tolua_LuaClient_SpriteFactory_SendAwakeConfigMessage00);
   tolua_function(tolua_S,"RemoveElfFromParent",tolua_LuaClient_SpriteFactory_RemoveElfFromParent00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"graylightWithCCSprite",tolua_LuaClient_graylightWithCCSprite00);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"SpriteElfDataCenter","SpriteElfDataCenter","",tolua_collect_SpriteElfDataCenter);
  #else
  tolua_cclass(tolua_S,"SpriteElfDataCenter","SpriteElfDataCenter","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"SpriteElfDataCenter");
   tolua_function(tolua_S,"new",tolua_LuaClient_SpriteElfDataCenter_new00);
   tolua_function(tolua_S,"new_local",tolua_LuaClient_SpriteElfDataCenter_new00_local);
   tolua_function(tolua_S,".call",tolua_LuaClient_SpriteElfDataCenter_new00_local);
   tolua_function(tolua_S,"delete",tolua_LuaClient_SpriteElfDataCenter_delete00);
   tolua_function(tolua_S,"Get",tolua_LuaClient_SpriteElfDataCenter_Get00);
   tolua_function(tolua_S,"Destroy",tolua_LuaClient_SpriteElfDataCenter_Destroy00);
   tolua_function(tolua_S,"EquipOneElf",tolua_LuaClient_SpriteElfDataCenter_EquipOneElf00);
   tolua_function(tolua_S,"SendGetElfListMessage",tolua_LuaClient_SpriteElfDataCenter_SendGetElfListMessage00);
   tolua_function(tolua_S,"SendSetElfStateMessage",tolua_LuaClient_SpriteElfDataCenter_SendSetElfStateMessage00);
   tolua_function(tolua_S,"GetHeroUserId",tolua_LuaClient_SpriteElfDataCenter_GetHeroUserId00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"LevelManager","LevelManager","",NULL);
  tolua_beginmodule(tolua_S,"LevelManager");
   tolua_function(tolua_S,"sShareInstance",tolua_LuaClient_LevelManager_sShareInstance00);
   tolua_function(tolua_S,"getCurMapId",tolua_LuaClient_LevelManager_getCurMapId00);
   tolua_function(tolua_S,"isCurrentPPVELevel",tolua_LuaClient_LevelManager_isCurrentPPVELevel00);
   tolua_function(tolua_S,"isCurrentPVELevel",tolua_LuaClient_LevelManager_isCurrentPVELevel00);
   tolua_function(tolua_S,"isCurrentPVPLevel",tolua_LuaClient_LevelManager_isCurrentPVPLevel00);
   tolua_function(tolua_S,"isCurrentCommonPKLevel",tolua_LuaClient_LevelManager_isCurrentCommonPKLevel00);
   tolua_function(tolua_S,"isCurrentMandPKLevel",tolua_LuaClient_LevelManager_isCurrentMandPKLevel00);
   tolua_function(tolua_S,"isCurrentEliteLevel",tolua_LuaClient_LevelManager_isCurrentEliteLevel00);
   tolua_function(tolua_S,"isCurrentUndergroundLevel",tolua_LuaClient_LevelManager_isCurrentUndergroundLevel00);
   tolua_function(tolua_S,"isCurrentCityDefendLevel",tolua_LuaClient_LevelManager_isCurrentCityDefendLevel00);
   tolua_function(tolua_S,"isCurrentWorldBossLevel",tolua_LuaClient_LevelManager_isCurrentWorldBossLevel00);
   tolua_function(tolua_S,"isCurrentBestFighterLevel",tolua_LuaClient_LevelManager_isCurrentBestFighterLevel00);
   tolua_function(tolua_S,"isCurrentGuildInstanceLevel",tolua_LuaClient_LevelManager_isCurrentGuildInstanceLevel00);
   tolua_function(tolua_S,"getCurrentLevelLayer",tolua_LuaClient_LevelManager_getCurrentLevelLayer00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"GM_E_ENTER_MAINLAND_COLOR_LAYER_OVER",GM_E_ENTER_MAINLAND_COLOR_LAYER_OVER);
  tolua_constant(tolua_S,"GM_ATTR_COIN_UPDATE",GM_ATTR_COIN_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SP_UPDATE",GM_ATTR_SP_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SP_CHIP_UPDATE",GM_ATTR_SP_CHIP_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SP_DIAMOND_UPDATE",GM_ATTR_SP_DIAMOND_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_HERO_LEVEL_UPDATE",GM_ATTR_HERO_LEVEL_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_PHYSICAL_ATTACK_UPDATE",GM_ATTR_PHYSICAL_ATTACK_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_MAGIC_ATTACK_UPDATE",GM_ATTR_MAGIC_ATTACK_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SKILL_ATTACK_UPDATE",GM_ATTR_SKILL_ATTACK_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_PHYSICAL_DEFENCE_UPDATE",GM_ATTR_PHYSICAL_DEFENCE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_MAGIC_DEFENCE_UPDATE",GM_ATTR_MAGIC_DEFENCE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SKILL_DEFENCE_UPDATE",GM_ATTR_SKILL_DEFENCE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_HEALTH_POINT_UPDATE",GM_ATTR_HEALTH_POINT_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_ACCURATE_UPDATE",GM_ATTR_ACCURATE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_DODGE_UPDATE",GM_ATTR_DODGE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_WRECK_UPDATE",GM_ATTR_WRECK_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_PARRY_UPDATE",GM_ATTR_PARRY_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_CRITICAL_STRIKE_UPDATE",GM_ATTR_CRITICAL_STRIKE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_TENACITY_UPDATE",GM_ATTR_TENACITY_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SLAY_UPDATE",GM_ATTR_SLAY_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_SPEED_UPDATE",GM_ATTR_SPEED_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_PROFICIENCY_UPDATE",GM_ATTR_PROFICIENCY_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_ABILITY_ALL_UPDATE",GM_ATTR_ABILITY_ALL_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_COURAGE_UPDATE",GM_ATTR_COURAGE_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_CHARM_UPDATE",GM_ATTR_CHARM_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_TRICK_UPDATE",GM_ATTR_TRICK_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_STAMINA_UPDATE",GM_ATTR_STAMINA_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_REPUTATION_UPDATE",GM_ATTR_REPUTATION_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_EXP_UPDATE",GM_ATTR_EXP_UPDATE);
  tolua_constant(tolua_S,"GM_ATTR_EQUIP_LVUP_UESED_TIMES",GM_ATTR_EQUIP_LVUP_UESED_TIMES);
  tolua_constant(tolua_S,"GM_ATTR_EQUIP_LVUP_CD",GM_ATTR_EQUIP_LVUP_CD);
  tolua_constant(tolua_S,"GM_NETWORK_DISCONNCT",GM_NETWORK_DISCONNCT);
  tolua_constant(tolua_S,"GM_CLOSE_TALENTLAYER",GM_CLOSE_TALENTLAYER);
  tolua_constant(tolua_S,"GM_CLOSE_AUTOONHOOK",GM_CLOSE_AUTOONHOOK);
  tolua_constant(tolua_S,"GM_ATTR_PLAYER_EXPOLIT",GM_ATTR_PLAYER_EXPOLIT);
  tolua_constant(tolua_S,"GM_LUA_LAYER_CLOSE",GM_LUA_LAYER_CLOSE);
  tolua_constant(tolua_S,"GM_ATTR_PLAYER_EXPLOER_EXP",GM_ATTR_PLAYER_EXPLOER_EXP);
  tolua_constant(tolua_S,"GM_ATTR_GEM_RECAST",GM_ATTR_GEM_RECAST);
  tolua_constant(tolua_S,"GM_ATTR_PLAYER_GEM_ANIMA",GM_ATTR_PLAYER_GEM_ANIMA);
  tolua_constant(tolua_S,"GM_E_ENTER_MAINLAND",GM_E_ENTER_MAINLAND);
  tolua_constant(tolua_S,"GM_E_ENTER_FB",GM_E_ENTER_FB);
  tolua_constant(tolua_S,"GM_ATTR_BUY_BAG_CAPACITY",GM_ATTR_BUY_BAG_CAPACITY);
  tolua_constant(tolua_S,"GM_ATTR_BUY_STORE_CAPACITY",GM_ATTR_BUY_STORE_CAPACITY);
  tolua_constant(tolua_S,"GM_ATTR_BUY_SS_BAG_CAPACITY",GM_ATTR_BUY_SS_BAG_CAPACITY);
  tolua_constant(tolua_S,"GM_ATTR_AUTO_FIGHT_CD",GM_ATTR_AUTO_FIGHT_CD);
  tolua_constant(tolua_S,"GM_ATTR_BAO_ZHANG_COIN_CHANGE",GM_ATTR_BAO_ZHANG_COIN_CHANGE);
  tolua_constant(tolua_S,"GM_ATTR_VIP_LEVEL_CHANGE",GM_ATTR_VIP_LEVEL_CHANGE);
  tolua_constant(tolua_S,"GM_ATTR_SERVER_TIME_SYNCED",GM_ATTR_SERVER_TIME_SYNCED);
  tolua_constant(tolua_S,"GM_E_ENTER_FIRST_LOGIN_STATE",GM_E_ENTER_FIRST_LOGIN_STATE);
  tolua_constant(tolua_S,"GM_ATTR_FRAG_STONE",GM_ATTR_FRAG_STONE);
  tolua_constant(tolua_S,"GM_ATTR_FRAG_CAN_CONPOSE",GM_ATTR_FRAG_CAN_CONPOSE);
  tolua_constant(tolua_S,"GM_FAIRY_SELECT_DONE",GM_FAIRY_SELECT_DONE);
  tolua_constant(tolua_S,"GM_ATTR_FAIRY_EFFECTIVENESS",GM_ATTR_FAIRY_EFFECTIVENESS);
  tolua_constant(tolua_S,"GM_ATTR_FAIRY_FETTER",GM_ATTR_FAIRY_FETTER);
  tolua_constant(tolua_S,"GM_ATTR_FARIY_FLAG_ENOUGH",GM_ATTR_FARIY_FLAG_ENOUGH);
  tolua_constant(tolua_S,"GM_ATTR_NEW_FAIRY_INFO",GM_ATTR_NEW_FAIRY_INFO);
  tolua_constant(tolua_S,"GM_MONTHCARD_GET_SUCCESS",GM_MONTHCARD_GET_SUCCESS);
  tolua_constant(tolua_S,"GM_GET_PLAYER_CHANGE_NAME",GM_GET_PLAYER_CHANGE_NAME);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"USERINFO","USERINFO","",tolua_collect_USERINFO);
  #else
  tolua_cclass(tolua_S,"USERINFO","USERINFO","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"USERINFO");
   tolua_function(tolua_S,"new",tolua_LuaClient_USERINFO_new00);
   tolua_function(tolua_S,"new_local",tolua_LuaClient_USERINFO_new00_local);
   tolua_function(tolua_S,".call",tolua_LuaClient_USERINFO_new00_local);
   tolua_variable(tolua_S,"id",tolua_get_USERINFO_id,tolua_set_USERINFO_id);
   tolua_variable(tolua_S,"userSession",tolua_get_USERINFO_userSession,tolua_set_USERINFO_userSession);
   tolua_variable(tolua_S,"szName",tolua_get_USERINFO_szName,tolua_set_USERINFO_szName);
   tolua_variable(tolua_S,"ip",tolua_get_USERINFO_ip,tolua_set_USERINFO_ip);
   tolua_variable(tolua_S,"lastServerName",tolua_get_USERINFO_lastServerName,tolua_set_USERINFO_lastServerName);
   tolua_variable(tolua_S,"port",tolua_get_USERINFO_unsigned_port,tolua_set_USERINFO_unsigned_port);
   tolua_variable(tolua_S,"createdTime",tolua_get_USERINFO_createdTime,tolua_set_USERINFO_createdTime);
   tolua_variable(tolua_S,"tokenId",tolua_get_USERINFO_unsigned_tokenId,tolua_set_USERINFO_unsigned_tokenId);
   tolua_variable(tolua_S,"level",tolua_get_USERINFO_unsigned_level,tolua_set_USERINFO_unsigned_level);
   tolua_variable(tolua_S,"exp",tolua_get_USERINFO_unsigned_exp,tolua_set_USERINFO_unsigned_exp);
   tolua_variable(tolua_S,"nextExp",tolua_get_USERINFO_unsigned_nextExp,tolua_set_USERINFO_unsigned_nextExp);
   tolua_variable(tolua_S,"mapId",tolua_get_USERINFO_unsigned_mapId,tolua_set_USERINFO_unsigned_mapId);
   tolua_variable(tolua_S,"xPos",tolua_get_USERINFO_unsigned_xPos,tolua_set_USERINFO_unsigned_xPos);
   tolua_variable(tolua_S,"yPos",tolua_get_USERINFO_unsigned_yPos,tolua_set_USERINFO_unsigned_yPos);
   tolua_variable(tolua_S,"orient",tolua_get_USERINFO_unsigned_orient,tolua_set_USERINFO_unsigned_orient);
   tolua_variable(tolua_S,"type",tolua_get_USERINFO_unsigned_type,tolua_set_USERINFO_unsigned_type);
   tolua_variable(tolua_S,"battleId",tolua_get_USERINFO_unsigned_battleId,tolua_set_USERINFO_unsigned_battleId);
   tolua_variable(tolua_S,"current_title",tolua_get_USERINFO_unsigned_current_title,tolua_set_USERINFO_unsigned_current_title);
   tolua_variable(tolua_S,"battleSide",tolua_get_USERINFO_unsigned_battleSide,tolua_set_USERINFO_unsigned_battleSide);
   tolua_variable(tolua_S,"m_spriteEnergy",tolua_get_USERINFO_unsigned_m_spriteEnergy,tolua_set_USERINFO_unsigned_m_spriteEnergy);
   tolua_variable(tolua_S,"m_gold",tolua_get_USERINFO_unsigned_m_gold,tolua_set_USERINFO_unsigned_m_gold);
   tolua_variable(tolua_S,"m_diamond",tolua_get_USERINFO_unsigned_m_diamond,tolua_set_USERINFO_unsigned_m_diamond);
   tolua_variable(tolua_S,"m_stamina",tolua_get_USERINFO_unsigned_m_stamina,tolua_set_USERINFO_unsigned_m_stamina);
   tolua_variable(tolua_S,"m_spriteChip",tolua_get_USERINFO_unsigned_m_spriteChip,tolua_set_USERINFO_unsigned_m_spriteChip);
   tolua_variable(tolua_S,"m_alchemyBagUnlockPos",tolua_get_USERINFO_unsigned_m_alchemyBagUnlockPos,tolua_set_USERINFO_unsigned_m_alchemyBagUnlockPos);
   tolua_variable(tolua_S,"m_alchemyStorageUnlockPos",tolua_get_USERINFO_unsigned_m_alchemyStorageUnlockPos,tolua_set_USERINFO_unsigned_m_alchemyStorageUnlockPos);
   tolua_variable(tolua_S,"m_gemRecastTimes",tolua_get_USERINFO_unsigned_m_gemRecastTimes,tolua_set_USERINFO_unsigned_m_gemRecastTimes);
   tolua_variable(tolua_S,"m_reputation",tolua_get_USERINFO_unsigned_m_reputation,tolua_set_USERINFO_unsigned_m_reputation);
   tolua_variable(tolua_S,"m_soulStone",tolua_get_USERINFO_unsigned_m_soulStone,tolua_set_USERINFO_unsigned_m_soulStone);
   tolua_variable(tolua_S,"physical_attack",tolua_get_USERINFO_unsigned_physical_attack,tolua_set_USERINFO_unsigned_physical_attack);
   tolua_variable(tolua_S,"magic_attack",tolua_get_USERINFO_unsigned_magic_attack,tolua_set_USERINFO_unsigned_magic_attack);
   tolua_variable(tolua_S,"skill_attack",tolua_get_USERINFO_unsigned_skill_attack,tolua_set_USERINFO_unsigned_skill_attack);
   tolua_variable(tolua_S,"physical_defence",tolua_get_USERINFO_unsigned_physical_defence,tolua_set_USERINFO_unsigned_physical_defence);
   tolua_variable(tolua_S,"magic_defence",tolua_get_USERINFO_unsigned_magic_defence,tolua_set_USERINFO_unsigned_magic_defence);
   tolua_variable(tolua_S,"skill_defence",tolua_get_USERINFO_unsigned_skill_defence,tolua_set_USERINFO_unsigned_skill_defence);
   tolua_variable(tolua_S,"health_point",tolua_get_USERINFO_unsigned_health_point,tolua_set_USERINFO_unsigned_health_point);
   tolua_variable(tolua_S,"accurate",tolua_get_USERINFO_unsigned_accurate,tolua_set_USERINFO_unsigned_accurate);
   tolua_variable(tolua_S,"dodge",tolua_get_USERINFO_unsigned_dodge,tolua_set_USERINFO_unsigned_dodge);
   tolua_variable(tolua_S,"wreck",tolua_get_USERINFO_unsigned_wreck,tolua_set_USERINFO_unsigned_wreck);
   tolua_variable(tolua_S,"parry",tolua_get_USERINFO_unsigned_parry,tolua_set_USERINFO_unsigned_parry);
   tolua_variable(tolua_S,"critical_strike",tolua_get_USERINFO_unsigned_critical_strike,tolua_set_USERINFO_unsigned_critical_strike);
   tolua_variable(tolua_S,"tenacity",tolua_get_USERINFO_unsigned_tenacity,tolua_set_USERINFO_unsigned_tenacity);
   tolua_variable(tolua_S,"slay",tolua_get_USERINFO_unsigned_slay,tolua_set_USERINFO_unsigned_slay);
   tolua_variable(tolua_S,"proficiency",tolua_get_USERINFO_unsigned_proficiency,tolua_set_USERINFO_unsigned_proficiency);
   tolua_variable(tolua_S,"speed",tolua_get_USERINFO_unsigned_speed,tolua_set_USERINFO_unsigned_speed);
   tolua_variable(tolua_S,"total_hp",tolua_get_USERINFO_unsigned_total_hp,tolua_set_USERINFO_unsigned_total_hp);
   tolua_variable(tolua_S,"courage",tolua_get_USERINFO_unsigned_courage,tolua_set_USERINFO_unsigned_courage);
   tolua_variable(tolua_S,"charm",tolua_get_USERINFO_unsigned_charm,tolua_set_USERINFO_unsigned_charm);
   tolua_variable(tolua_S,"trick",tolua_get_USERINFO_unsigned_trick,tolua_set_USERINFO_unsigned_trick);
   tolua_variable(tolua_S,"effectiveness",tolua_get_USERINFO_unsigned_effectiveness,tolua_set_USERINFO_unsigned_effectiveness);
   tolua_variable(tolua_S,"m_player_exploit",tolua_get_USERINFO_unsigned_m_player_exploit,tolua_set_USERINFO_unsigned_m_player_exploit);
   tolua_variable(tolua_S,"m_playerExploerExperience",tolua_get_USERINFO_unsigned_m_playerExploerExperience,tolua_set_USERINFO_unsigned_m_playerExploerExperience);
   tolua_variable(tolua_S,"m_playerGemAnima",tolua_get_USERINFO_unsigned_m_playerGemAnima,tolua_set_USERINFO_unsigned_m_playerGemAnima);
   tolua_variable(tolua_S,"m_enterManorTimes",tolua_get_USERINFO_unsigned_m_enterManorTimes,tolua_set_USERINFO_unsigned_m_enterManorTimes);
   tolua_variable(tolua_S,"m_continueloginTime",tolua_get_USERINFO_unsigned_m_continueloginTime,tolua_set_USERINFO_unsigned_m_continueloginTime);
   tolua_variable(tolua_S,"m_loginRewardTime",tolua_get_USERINFO_unsigned_m_loginRewardTime,tolua_set_USERINFO_unsigned_m_loginRewardTime);
   tolua_variable(tolua_S,"m_vipRewardTime",tolua_get_USERINFO_unsigned_m_vipRewardTime,tolua_set_USERINFO_unsigned_m_vipRewardTime);
   tolua_variable(tolua_S,"m_payRewardTime",tolua_get_USERINFO_unsigned_m_payRewardTime,tolua_set_USERINFO_unsigned_m_payRewardTime);
   tolua_variable(tolua_S,"m_buyDiamondRewardTime",tolua_get_USERINFO_unsigned_m_buyDiamondRewardTime,tolua_set_USERINFO_unsigned_m_buyDiamondRewardTime);
   tolua_variable(tolua_S,"m_buyManorTimes",tolua_get_USERINFO_unsigned_m_buyManorTimes,tolua_set_USERINFO_unsigned_m_buyManorTimes);
   tolua_variable(tolua_S,"m_pvaiRewardTime",tolua_get_USERINFO_unsigned_m_pvaiRewardTime,tolua_set_USERINFO_unsigned_m_pvaiRewardTime);
   tolua_variable(tolua_S,"m_palyHoldExp",tolua_get_USERINFO_unsigned_m_palyHoldExp,tolua_set_USERINFO_unsigned_m_palyHoldExp);
   tolua_variable(tolua_S,"m_newMailNum",tolua_get_USERINFO_unsigned_m_newMailNum,tolua_set_USERINFO_unsigned_m_newMailNum);
   tolua_variable(tolua_S,"m_totalMailNum",tolua_get_USERINFO_unsigned_m_totalMailNum,tolua_set_USERINFO_unsigned_m_totalMailNum);
   tolua_variable(tolua_S,"m_autofightNormalCd",tolua_get_USERINFO_unsigned_m_autofightNormalCd,tolua_set_USERINFO_unsigned_m_autofightNormalCd);
   tolua_variable(tolua_S,"m_autofightJingYingCd",tolua_get_USERINFO_unsigned_m_autofightJingYingCd,tolua_set_USERINFO_unsigned_m_autofightJingYingCd);
   tolua_variable(tolua_S,"pay_money",tolua_get_USERINFO_unsigned_pay_money,tolua_set_USERINFO_unsigned_pay_money);
   tolua_variable(tolua_S,"mine_money",tolua_get_USERINFO_unsigned_mine_money,tolua_set_USERINFO_unsigned_mine_money);
   tolua_variable(tolua_S,"train_money",tolua_get_USERINFO_unsigned_train_money,tolua_set_USERINFO_unsigned_train_money);
   tolua_variable(tolua_S,"suspend_limit_exp",tolua_get_USERINFO_unsigned_suspend_limit_exp,tolua_set_USERINFO_unsigned_suspend_limit_exp);
   tolua_variable(tolua_S,"m_vip",tolua_get_USERINFO_unsigned_m_vip,tolua_set_USERINFO_unsigned_m_vip);
   tolua_variable(tolua_S,"m_fragStone",tolua_get_USERINFO_unsigned_m_fragStone,tolua_set_USERINFO_unsigned_m_fragStone);
   tolua_variable(tolua_S,"m_pvpDuelTimesNum",tolua_get_USERINFO_unsigned_m_pvpDuelTimesNum,tolua_set_USERINFO_unsigned_m_pvpDuelTimesNum);
   tolua_variable(tolua_S,"m_pvpDueledTimesNum",tolua_get_USERINFO_unsigned_m_pvpDueledTimesNum,tolua_set_USERINFO_unsigned_m_pvpDueledTimesNum);
   tolua_variable(tolua_S,"m_pvpRed",tolua_get_USERINFO_unsigned_m_pvpRed,tolua_set_USERINFO_unsigned_m_pvpRed);
   tolua_variable(tolua_S,"b_hasEnouchFairyFlag",tolua_get_USERINFO_b_hasEnouchFairyFlag,tolua_set_USERINFO_b_hasEnouchFairyFlag);
   tolua_variable(tolua_S,"b_reputationExchangeFlag",tolua_get_USERINFO_b_reputationExchangeFlag,tolua_set_USERINFO_b_reputationExchangeFlag);
   tolua_variable(tolua_S,"m_diamond_acc",tolua_get_USERINFO_unsigned_m_diamond_acc,tolua_set_USERINFO_unsigned_m_diamond_acc);
   tolua_variable(tolua_S,"guildName",tolua_get_USERINFO_guildName,tolua_set_USERINFO_guildName);
   tolua_variable(tolua_S,"m_guildId",tolua_get_USERINFO_m_guildId,tolua_set_USERINFO_m_guildId);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"PLAYER_ID","PLAYER_ID","",tolua_collect_PLAYER_ID);
  #else
  tolua_cclass(tolua_S,"PLAYER_ID","PLAYER_ID","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"PLAYER_ID");
   tolua_function(tolua_S,"new",tolua_LuaClient_PLAYER_ID_new00);
   tolua_function(tolua_S,"new_local",tolua_LuaClient_PLAYER_ID_new00_local);
   tolua_function(tolua_S,".call",tolua_LuaClient_PLAYER_ID_new00_local);
   tolua_function(tolua_S,"setPlayId",tolua_LuaClient_PLAYER_ID_setPlayId00);
   tolua_function(tolua_S,"setPlayId",tolua_LuaClient_PLAYER_ID_setPlayId01);
   tolua_function(tolua_S,"isEqual",tolua_LuaClient_PLAYER_ID_isEqual00);
   tolua_function(tolua_S,"getID",tolua_LuaClient_PLAYER_ID_getID00);
   tolua_function(tolua_S,"getRegTime",tolua_LuaClient_PLAYER_ID_getRegTime00);
   tolua_function(tolua_S,"getChannel",tolua_LuaClient_PLAYER_ID_getChannel00);
   tolua_function(tolua_S,"setRegTime",tolua_LuaClient_PLAYER_ID_setRegTime00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GUILD_ID","GUILD_ID","",tolua_collect_GUILD_ID);
  #else
  tolua_cclass(tolua_S,"GUILD_ID","GUILD_ID","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GUILD_ID");
   tolua_function(tolua_S,"new",tolua_LuaClient_GUILD_ID_new00);
   tolua_function(tolua_S,"new_local",tolua_LuaClient_GUILD_ID_new00_local);
   tolua_function(tolua_S,".call",tolua_LuaClient_GUILD_ID_new00_local);
   tolua_function(tolua_S,"setGuildId",tolua_LuaClient_GUILD_ID_setGuildId00);
   tolua_function(tolua_S,"setGuildId",tolua_LuaClient_GUILD_ID_setGuildId01);
   tolua_function(tolua_S,"isEqual",tolua_LuaClient_GUILD_ID_isEqual00);
   tolua_function(tolua_S,"getHighID",tolua_LuaClient_GUILD_ID_getHighID00);
   tolua_function(tolua_S,"getLowID",tolua_LuaClient_GUILD_ID_getLowID00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"UserData","UserData","",NULL);
  tolua_beginmodule(tolua_S,"UserData");
   tolua_function(tolua_S,"Get",tolua_LuaClient_UserData_Get00);
   tolua_function(tolua_S,"getUserId",tolua_LuaClient_UserData_getUserId00);
   tolua_function(tolua_S,"getUserName",tolua_LuaClient_UserData_getUserName00);
   tolua_function(tolua_S,"setUserName",tolua_LuaClient_UserData_setUserName00);
   tolua_function(tolua_S,"getHeroHp",tolua_LuaClient_UserData_getHeroHp00);
   tolua_function(tolua_S,"getHeroTotalHp",tolua_LuaClient_UserData_getHeroTotalHp00);
   tolua_function(tolua_S,"GetUserType",tolua_LuaClient_UserData_GetUserType00);
   tolua_function(tolua_S,"GetUserInfo",tolua_LuaClient_UserData_GetUserInfo00);
   tolua_function(tolua_S,"GetUserLevel",tolua_LuaClient_UserData_GetUserLevel00);
   tolua_function(tolua_S,"SetUserInfo",tolua_LuaClient_UserData_SetUserInfo00);
   tolua_function(tolua_S,"GetUserInfo",tolua_LuaClient_UserData_GetUserInfo01);
   tolua_function(tolua_S,"SetUserInfo",tolua_LuaClient_UserData_SetUserInfo01);
   tolua_function(tolua_S,"removeOtherUser",tolua_LuaClient_UserData_removeOtherUser00);
   tolua_function(tolua_S,"getRagePoint",tolua_LuaClient_UserData_getRagePoint00);
   tolua_function(tolua_S,"getTotalRagePoint",tolua_LuaClient_UserData_getTotalRagePoint00);
   tolua_function(tolua_S,"getSecRagePoint",tolua_LuaClient_UserData_getSecRagePoint00);
   tolua_function(tolua_S,"getSecTotalRagePoint",tolua_LuaClient_UserData_getSecTotalRagePoint00);
   tolua_function(tolua_S,"GetDiamond",tolua_LuaClient_UserData_GetDiamond00);
   tolua_function(tolua_S,"GetVipLevel",tolua_LuaClient_UserData_GetVipLevel00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"PlayerIdMake",tolua_LuaClient_PlayerIdMake00);
  tolua_function(tolua_S,"GuildIdMake",tolua_LuaClient_GuildIdMake00);
  tolua_cclass(tolua_S,"NotificationCenter","NotificationCenter","",NULL);
  tolua_beginmodule(tolua_S,"NotificationCenter");
   tolua_function(tolua_S,"defaultCenter",tolua_LuaClient_NotificationCenter_defaultCenter00);
   tolua_function(tolua_S,"registerScriptObserver",tolua_LuaClient_NotificationCenter_registerScriptObserver00);
   tolua_function(tolua_S,"broadcast",tolua_LuaClient_NotificationCenter_broadcast00);
   tolua_function(tolua_S,"unregisterScriptObserver",tolua_LuaClient_NotificationCenter_unregisterScriptObserver00);
   tolua_function(tolua_S,"unregisterAllScriptObservers",tolua_LuaClient_NotificationCenter_unregisterAllScriptObservers00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameManager","GameManager","",NULL);
  tolua_beginmodule(tolua_S,"GameManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_GameManager_Get00);
   tolua_function(tolua_S,"GetSceneLayer",tolua_LuaClient_GameManager_GetSceneLayer00);
   tolua_function(tolua_S,"getHero",tolua_LuaClient_GameManager_getHero00);
   tolua_function(tolua_S,"setHero",tolua_LuaClient_GameManager_setHero00);
   tolua_function(tolua_S,"getHeroTypeId",tolua_LuaClient_GameManager_getHeroTypeId00);
   tolua_function(tolua_S,"setHeroTypeId",tolua_LuaClient_GameManager_setHeroTypeId00);
   tolua_function(tolua_S,"goToSplash",tolua_LuaClient_GameManager_goToSplash00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SkillUIManager","SkillUIManager","",NULL);
  tolua_beginmodule(tolua_S,"SkillUIManager");
   tolua_function(tolua_S,"getInstance",tolua_LuaClient_SkillUIManager_getInstance00);
   tolua_function(tolua_S,"ResetValue",tolua_LuaClient_SkillUIManager_ResetValue00);
   tolua_function(tolua_S,"sendGetUserSkillReqMessage",tolua_LuaClient_SkillUIManager_sendGetUserSkillReqMessage00);
   tolua_function(tolua_S,"sendAddUserSkillReqMessage",tolua_LuaClient_SkillUIManager_sendAddUserSkillReqMessage00);
   tolua_function(tolua_S,"sendUserSkillDiffReqMessage",tolua_LuaClient_SkillUIManager_sendUserSkillDiffReqMessage00);
   tolua_function(tolua_S,"sendSetSlotAttributeMessage",tolua_LuaClient_SkillUIManager_sendSetSlotAttributeMessage00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ElfExploreManger","ElfExploreManger","",tolua_collect_ElfExploreManger);
  #else
  tolua_cclass(tolua_S,"ElfExploreManger","ElfExploreManger","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ElfExploreManger");
   tolua_function(tolua_S,"new",tolua_LuaClient_ElfExploreManger_new00);
   tolua_function(tolua_S,"new_local",tolua_LuaClient_ElfExploreManger_new00_local);
   tolua_function(tolua_S,".call",tolua_LuaClient_ElfExploreManger_new00_local);
   tolua_function(tolua_S,"Get",tolua_LuaClient_ElfExploreManger_Get00);
   tolua_function(tolua_S,"Destroy",tolua_LuaClient_ElfExploreManger_Destroy00);
   tolua_function(tolua_S,"ResetValue",tolua_LuaClient_ElfExploreManger_ResetValue00);
   tolua_function(tolua_S,"SendExchangeElfReq",tolua_LuaClient_ElfExploreManger_SendExchangeElfReq00);
   tolua_function(tolua_S,"SendExploreReq",tolua_LuaClient_ElfExploreManger_SendExploreReq00);
   tolua_function(tolua_S,"SendExploreDetermineReq",tolua_LuaClient_ElfExploreManger_SendExploreDetermineReq00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MainMenuLayer","MainMenuLayer","CCLayer",NULL);
  tolua_beginmodule(tolua_S,"MainMenuLayer");
   tolua_function(tolua_S,"setChatToolbar",tolua_LuaClient_MainMenuLayer_setChatToolbar00);
   tolua_function(tolua_S,"getMainMenuLayer",tolua_LuaClient_MainMenuLayer_getMainMenuLayer00);
   tolua_function(tolua_S,"GetNumByFormat",tolua_LuaClient_MainMenuLayer_GetNumByFormat00);
   tolua_function(tolua_S,"GetNumByFormatK",tolua_LuaClient_MainMenuLayer_GetNumByFormatK00);
   tolua_function(tolua_S,"showUILayerByTag",tolua_LuaClient_MainMenuLayer_showUILayerByTag00);
   tolua_function(tolua_S,"findUILayerByTag",tolua_LuaClient_MainMenuLayer_findUILayerByTag00);
   tolua_function(tolua_S,"hideUILayerByTag",tolua_LuaClient_MainMenuLayer_hideUILayerByTag00);
   tolua_function(tolua_S,"getUILayerByTag",tolua_LuaClient_MainMenuLayer_getUILayerByTag00);
   tolua_function(tolua_S,"ShowPPVELayer",tolua_LuaClient_MainMenuLayer_ShowPPVELayer00);
   tolua_function(tolua_S,"HidePPVELayer",tolua_LuaClient_MainMenuLayer_HidePPVELayer00);
   tolua_function(tolua_S,"ShowPVPLayer",tolua_LuaClient_MainMenuLayer_ShowPVPLayer00);
   tolua_function(tolua_S,"resetMailUIMsg",tolua_LuaClient_MainMenuLayer_resetMailUIMsg00);
   tolua_function(tolua_S,"HidePVPLayer",tolua_LuaClient_MainMenuLayer_HidePVPLayer00);
   tolua_function(tolua_S,"showMagicShop",tolua_LuaClient_MainMenuLayer_showMagicShop00);
   tolua_function(tolua_S,"onReceivedElfStrengthLayerClosed",tolua_LuaClient_MainMenuLayer_onReceivedElfStrengthLayerClosed00);
   tolua_function(tolua_S,"setFairyMovedIcon",tolua_LuaClient_MainMenuLayer_setFairyMovedIcon00);
   tolua_function(tolua_S,"setFairySelectStatus",tolua_LuaClient_MainMenuLayer_setFairySelectStatus00);
   tolua_function(tolua_S,"ShowMarketLayer",tolua_LuaClient_MainMenuLayer_ShowMarketLayer00);
   tolua_function(tolua_S,"ShowChargeLayer",tolua_LuaClient_MainMenuLayer_ShowChargeLayer00);
   tolua_function(tolua_S,"HideFirstChargeEffect",tolua_LuaClient_MainMenuLayer_HideFirstChargeEffect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameAudioManager","GameAudioManager","",NULL);
  tolua_beginmodule(tolua_S,"GameAudioManager");
   tolua_function(tolua_S,"sharedManager",tolua_LuaClient_GameAudioManager_sharedManager00);
   tolua_function(tolua_S,"playEffect",tolua_LuaClient_GameAudioManager_playEffect00);
   tolua_function(tolua_S,"stopAllEffect",tolua_LuaClient_GameAudioManager_stopAllEffect00);
   tolua_function(tolua_S,"stopPlayBGM",tolua_LuaClient_GameAudioManager_stopPlayBGM00);
   tolua_function(tolua_S,"preLoadEffect",tolua_LuaClient_GameAudioManager_preLoadEffect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ParticleManagerX","ParticleManagerX","",NULL);
  tolua_beginmodule(tolua_S,"ParticleManagerX");
   tolua_function(tolua_S,"sharedManager",tolua_LuaClient_ParticleManagerX_sharedManager00);
   tolua_function(tolua_S,"getParticles",tolua_LuaClient_ParticleManagerX_getParticles00);
   tolua_function(tolua_S,"getParticles_uieffect",tolua_LuaClient_ParticleManagerX_getParticles_uieffect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ParticleManager","ParticleManager","",NULL);
  tolua_beginmodule(tolua_S,"ParticleManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_ParticleManager_Get00);
   tolua_function(tolua_S,"createEffectSpriteAndPlay",tolua_LuaClient_ParticleManager_createEffectSpriteAndPlay00);
   tolua_function(tolua_S,"playOneTimeEffect",tolua_LuaClient_ParticleManager_playOneTimeEffect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ITEMATTR","ITEMATTR","",NULL);
  tolua_beginmodule(tolua_S,"ITEMATTR");
   tolua_variable(tolua_S,"attr_key",tolua_get_ITEMATTR_unsigned_attr_key,tolua_set_ITEMATTR_unsigned_attr_key);
   tolua_variable(tolua_S,"attr_basic_value",tolua_get_ITEMATTR_unsigned_attr_basic_value,tolua_set_ITEMATTR_unsigned_attr_basic_value);
   tolua_variable(tolua_S,"attr_intensify_value",tolua_get_ITEMATTR_unsigned_attr_intensify_value,tolua_set_ITEMATTR_unsigned_attr_intensify_value);
   tolua_variable(tolua_S,"attr_gem_value",tolua_get_ITEMATTR_unsigned_attr_gem_value,tolua_set_ITEMATTR_unsigned_attr_gem_value);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MANORBASEINFO","MANORBASEINFO","",NULL);
  tolua_beginmodule(tolua_S,"MANORBASEINFO");
   tolua_variable(tolua_S,"hurtGetCoin",tolua_get_MANORBASEINFO_unsigned_hurtGetCoin,tolua_set_MANORBASEINFO_unsigned_hurtGetCoin);
   tolua_variable(tolua_S,"pickGetExp",tolua_get_MANORBASEINFO_unsigned_pickGetExp,tolua_set_MANORBASEINFO_unsigned_pickGetExp);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"FAIRYFRAGMENTBAGCAPACITY",FAIRYFRAGMENTBAGCAPACITY);
  tolua_constant(tolua_S,"kItemBag",kItemBag);
  tolua_constant(tolua_S,"kItemStorage",kItemStorage);
  tolua_constant(tolua_S,"kItemHeroEquip",kItemHeroEquip);
  tolua_constant(tolua_S,"kItemEnchaseEquip",kItemEnchaseEquip);
  tolua_constant(tolua_S,"kItemFairyEquip",kItemFairyEquip);
  tolua_constant(tolua_S,"kAlchemyBag",kAlchemyBag);
  tolua_constant(tolua_S,"kAlchemyStorage",kAlchemyStorage);
  tolua_constant(tolua_S,"kAlchemyHeroEquip",kAlchemyHeroEquip);
  tolua_constant(tolua_S,"kAlchemyFairyEquip",kAlchemyFairyEquip);
  tolua_constant(tolua_S,"kFairyStartId",kFairyStartId);
  tolua_constant(tolua_S,"kItemGemStone",kItemGemStone);
  tolua_constant(tolua_S,"kCardBag",kCardBag);
  tolua_constant(tolua_S,"kCardEquip",kCardEquip);
  tolua_constant(tolua_S,"kItemManorId",kItemManorId);
  tolua_constant(tolua_S,"kEquipFragmet",kEquipFragmet);
  tolua_constant(tolua_S,"kFairyFragment",kFairyFragment);
  tolua_constant(tolua_S,"kItemRecycle",kItemRecycle);
  tolua_cclass(tolua_S,"BackPackItem","BackPackItem","",NULL);
  tolua_beginmodule(tolua_S,"BackPackItem");
   tolua_function(tolua_S,"getItemInfo",tolua_LuaClient_BackPackItem_getItemInfo00);
   tolua_function(tolua_S,"getItemId",tolua_LuaClient_BackPackItem_getItemId00);
   tolua_variable(tolua_S,"isLocked",tolua_get_BackPackItem_isLocked,tolua_set_BackPackItem_isLocked);
   tolua_variable(tolua_S,"isEmpty",tolua_get_BackPackItem_isEmpty,tolua_set_BackPackItem_isEmpty);
   tolua_variable(tolua_S,"position",tolua_get_BackPackItem_unsigned_position,tolua_set_BackPackItem_unsigned_position);
   tolua_variable(tolua_S,"amount",tolua_get_BackPackItem_unsigned_amount,tolua_set_BackPackItem_unsigned_amount);
   tolua_variable(tolua_S,"itemLevel",tolua_get_BackPackItem_unsigned_itemLevel,tolua_set_BackPackItem_unsigned_itemLevel);
   tolua_variable(tolua_S,"m_equipInfo",tolua_get_BackPackItem_m_equipInfo_ptr,tolua_set_BackPackItem_m_equipInfo_ptr);
   tolua_variable(tolua_S,"packType",tolua_get_BackPackItem_unsigned_packType,tolua_set_BackPackItem_unsigned_packType);
   tolua_variable(tolua_S,"m_spriteInfo",tolua_get_BackPackItem_m_spriteInfo_ptr,tolua_set_BackPackItem_m_spriteInfo_ptr);
   tolua_variable(tolua_S,"m_manorBaseInfo",tolua_get_BackPackItem_m_manorBaseInfo_ptr,tolua_set_BackPackItem_m_manorBaseInfo_ptr);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ItemManager","ItemManager","",NULL);
  tolua_beginmodule(tolua_S,"ItemManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_ItemManager_Get00);
   tolua_function(tolua_S,"findItemByPos",tolua_LuaClient_ItemManager_findItemByPos00);
   tolua_function(tolua_S,"getIconSpriteById",tolua_LuaClient_ItemManager_getIconSpriteById00);
   tolua_function(tolua_S,"getIconFrame",tolua_LuaClient_ItemManager_getIconFrame00);
   tolua_function(tolua_S,"getLabelColorByQuality",tolua_LuaClient_ItemManager_getLabelColorByQuality00);
   tolua_function(tolua_S,"getFairyLabelColorByQuality",tolua_LuaClient_ItemManager_getFairyLabelColorByQuality00);
   tolua_function(tolua_S,"getItemNumberById",tolua_LuaClient_ItemManager_getItemNumberById00);
   tolua_function(tolua_S,"getAttributionText",tolua_LuaClient_ItemManager_getAttributionText00);
   tolua_function(tolua_S,"showItemTipsById",tolua_LuaClient_ItemManager_showItemTipsById00);
   tolua_function(tolua_S,"setTipLuaHander",tolua_LuaClient_ItemManager_setTipLuaHander00);
   tolua_function(tolua_S,"setMiddleLuaHander",tolua_LuaClient_ItemManager_setMiddleLuaHander00);
   tolua_function(tolua_S,"getItemInfoById",tolua_LuaClient_ItemManager_getItemInfoById00);
   tolua_function(tolua_S,"openBackPack",tolua_LuaClient_ItemManager_openBackPack00);
   tolua_function(tolua_S,"closeEquipment",tolua_LuaClient_ItemManager_closeEquipment00);
   tolua_function(tolua_S,"checkBagEmptyItemNum",tolua_LuaClient_ItemManager_checkBagEmptyItemNum00);
   tolua_function(tolua_S,"showSpriteParticleEffect",tolua_LuaClient_ItemManager_showSpriteParticleEffect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ItemBase","ItemBase","",NULL);
  tolua_beginmodule(tolua_S,"ItemBase");
   tolua_function(tolua_S,"getEquipLevel",tolua_LuaClient_ItemBase_getEquipLevel00);
   tolua_variable(tolua_S,"m_id",tolua_get_ItemBase_unsigned_m_id,tolua_set_ItemBase_unsigned_m_id);
   tolua_variable(tolua_S,"m_name",tolua_get_ItemBase_m_name,tolua_set_ItemBase_m_name);
   tolua_variable(tolua_S,"b_isPiledUp",tolua_get_ItemBase_b_isPiledUp,tolua_set_ItemBase_b_isPiledUp);
   tolua_variable(tolua_S,"m_maxPiledNum",tolua_get_ItemBase_unsigned_m_maxPiledNum,tolua_set_ItemBase_unsigned_m_maxPiledNum);
   tolua_array(tolua_S,"m_source",tolua_get_LuaClient_ItemBase_m_source,tolua_set_LuaClient_ItemBase_m_source);
   tolua_variable(tolua_S,"m_byeprice",tolua_get_ItemBase_unsigned_m_byeprice,tolua_set_ItemBase_unsigned_m_byeprice);
   tolua_variable(tolua_S,"m_sellprice",tolua_get_ItemBase_unsigned_m_sellprice,tolua_set_ItemBase_unsigned_m_sellprice);
   tolua_variable(tolua_S,"m_requiredLevel",tolua_get_ItemBase_unsigned_m_requiredLevel,tolua_set_ItemBase_unsigned_m_requiredLevel);
   tolua_variable(tolua_S,"m_type",tolua_get_ItemBase_unsigned_m_type,tolua_set_ItemBase_unsigned_m_type);
   tolua_variable(tolua_S,"b_Destroyed",tolua_get_ItemBase_b_Destroyed,tolua_set_ItemBase_b_Destroyed);
   tolua_variable(tolua_S,"m_szDescription",tolua_get_ItemBase_m_szDescription,tolua_set_ItemBase_m_szDescription);
   tolua_variable(tolua_S,"m_quality",tolua_get_ItemBase_unsigned_m_quality,tolua_set_ItemBase_unsigned_m_quality);
   tolua_variable(tolua_S,"m_iconId",tolua_get_ItemBase_unsigned_m_iconId,tolua_set_ItemBase_unsigned_m_iconId);
   tolua_variable(tolua_S,"m_order",tolua_get_ItemBase_unsigned_m_order,tolua_set_ItemBase_unsigned_m_order);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"EquipmentItem","EquipmentItem","ItemBase",NULL);
  tolua_beginmodule(tolua_S,"EquipmentItem");
   tolua_variable(tolua_S,"m_ownerType",tolua_get_EquipmentItem_unsigned_m_ownerType,tolua_set_EquipmentItem_unsigned_m_ownerType);
   tolua_variable(tolua_S,"m_equipPart",tolua_get_EquipmentItem_unsigned_m_equipPart,tolua_set_EquipmentItem_unsigned_m_equipPart);
   tolua_variable(tolua_S,"m_equipLevel",tolua_get_EquipmentItem_unsigned_m_equipLevel,tolua_set_EquipmentItem_unsigned_m_equipLevel);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"TIMER_INSTANCE_COUNTDOWN",TIMER_INSTANCE_COUNTDOWN);
  tolua_constant(tolua_S,"TIMER_PVAI_COOLDOWN",TIMER_PVAI_COOLDOWN);
  tolua_constant(tolua_S,"TIMER_PVAI_REWARD",TIMER_PVAI_REWARD);
  tolua_constant(tolua_S,"TIMER_OFFLINE_EXP_COUNTDOWN",TIMER_OFFLINE_EXP_COUNTDOWN);
  tolua_constant(tolua_S,"TIMER_MANOUR_1",TIMER_MANOUR_1);
  tolua_constant(tolua_S,"TIMER_MANOUR_2",TIMER_MANOUR_2);
  tolua_constant(tolua_S,"TIMER_MANOUR_3",TIMER_MANOUR_3);
  tolua_constant(tolua_S,"TIMER_MANOUR_4",TIMER_MANOUR_4);
  tolua_constant(tolua_S,"TIMER_MANOUR_5",TIMER_MANOUR_5);
  tolua_constant(tolua_S,"TIMER_CITYDEFEND_REMAIN",TIMER_CITYDEFEND_REMAIN);
  tolua_constant(tolua_S,"TIMER_ACTIVITY_UPDATE",TIMER_ACTIVITY_UPDATE);
  tolua_constant(tolua_S,"TIMER_WORLDBOSS_START_REMAIN",TIMER_WORLDBOSS_START_REMAIN);
  tolua_constant(tolua_S,"TIMER_WORLDBOSS_END_REMAIN",TIMER_WORLDBOSS_END_REMAIN);
  tolua_constant(tolua_S,"TIMER_WORLDBOSS_REVIVE_COUNTDOWN",TIMER_WORLDBOSS_REVIVE_COUNTDOWN);
  tolua_constant(tolua_S,"TIMER_ONLINEREWARD_COUNTDOWN",TIMER_ONLINEREWARD_COUNTDOWN);
  tolua_constant(tolua_S,"TIMER_BESTFIGHTER_REMAIN",TIMER_BESTFIGHTER_REMAIN);
  tolua_constant(tolua_S,"TIMER_BESTFIGHTER_QUEUE_REMAIN",TIMER_BESTFIGHTER_QUEUE_REMAIN);
  tolua_constant(tolua_S,"TIMER_REWARD_STAMINA_REMAIN",TIMER_REWARD_STAMINA_REMAIN);
  tolua_constant(tolua_S,"TIMER_RED_NAME_REMAIN",TIMER_RED_NAME_REMAIN);
  tolua_constant(tolua_S,"TIMER_COIN_BOSS_REMAIN",TIMER_COIN_BOSS_REMAIN);
  tolua_cclass(tolua_S,"TimeManager","TimeManager","",NULL);
  tolua_beginmodule(tolua_S,"TimeManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_TimeManager_Get00);
   tolua_function(tolua_S,"getCurServerTime",tolua_LuaClient_TimeManager_getCurServerTime00);
   tolua_function(tolua_S,"isInited",tolua_LuaClient_TimeManager_isInited00);
   tolua_function(tolua_S,"getTimeZone",tolua_LuaClient_TimeManager_getTimeZone00);
   tolua_function(tolua_S,"secondsToString",tolua_LuaClient_TimeManager_secondsToString00);
   tolua_function(tolua_S,"registerLuaTimer",tolua_LuaClient_TimeManager_registerLuaTimer00);
   tolua_function(tolua_S,"unregisterLuaTimer",tolua_LuaClient_TimeManager_unregisterLuaTimer00);
   tolua_function(tolua_S,"attachLuaTimer",tolua_LuaClient_TimeManager_attachLuaTimer00);
   tolua_function(tolua_S,"startTimer",tolua_LuaClient_TimeManager_startTimer00);
   tolua_function(tolua_S,"stopTimer",tolua_LuaClient_TimeManager_stopTimer00);
   tolua_function(tolua_S,"renewTimer",tolua_LuaClient_TimeManager_renewTimer00);
   tolua_function(tolua_S,"hasTimer",tolua_LuaClient_TimeManager_hasTimer00);
   tolua_function(tolua_S,"hasLuaObserver",tolua_LuaClient_TimeManager_hasLuaObserver00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GemStonesFromLua","GemStonesFromLua","",NULL);
  tolua_beginmodule(tolua_S,"GemStonesFromLua");
   tolua_function(tolua_S,"getInstance",tolua_LuaClient_GemStonesFromLua_getInstance00);
   tolua_function(tolua_S,"EquipInlayGemReq",tolua_LuaClient_GemStonesFromLua_EquipInlayGemReq00);
   tolua_function(tolua_S,"GemRecastReq",tolua_LuaClient_GemStonesFromLua_GemRecastReq00);
   tolua_function(tolua_S,"GemAnimaExchangeReq",tolua_LuaClient_GemStonesFromLua_GemAnimaExchangeReq00);
   tolua_function(tolua_S,"GemHoleUnlockReq",tolua_LuaClient_GemStonesFromLua_GemHoleUnlockReq00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PvAIManager","PvAIManager","",NULL);
  tolua_beginmodule(tolua_S,"PvAIManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_PvAIManager_Get00);
   tolua_function(tolua_S,"sendPvAIReq",tolua_LuaClient_PvAIManager_sendPvAIReq00);
   tolua_function(tolua_S,"onMsgPvAIReward",tolua_LuaClient_PvAIManager_onMsgPvAIReward00);
   tolua_function(tolua_S,"getCoolDownEndTime",tolua_LuaClient_PvAIManager_getCoolDownEndTime00);
   tolua_function(tolua_S,"getRemainCount",tolua_LuaClient_PvAIManager_getRemainCount00);
   tolua_function(tolua_S,"getBuyCount",tolua_LuaClient_PvAIManager_getBuyCount00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TutorialsManager","TutorialsManager","",NULL);
  tolua_beginmodule(tolua_S,"TutorialsManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_TutorialsManager_Get00);
   tolua_function(tolua_S,"StartOneTutorial",tolua_LuaClient_TutorialsManager_StartOneTutorial00);
   tolua_function(tolua_S,"HandleOneEvent",tolua_LuaClient_TutorialsManager_HandleOneEvent00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"OnlineNetworkManager","OnlineNetworkManager","",NULL);
  tolua_beginmodule(tolua_S,"OnlineNetworkManager");
   tolua_function(tolua_S,"sShareInstance",tolua_LuaClient_OnlineNetworkManager_sShareInstance00);
   tolua_function(tolua_S,"sendDailyTaskReqMessage",tolua_LuaClient_OnlineNetworkManager_sendDailyTaskReqMessage00);
   tolua_function(tolua_S,"sendDailyTaskAcceptReqMessage",tolua_LuaClient_OnlineNetworkManager_sendDailyTaskAcceptReqMessage00);
   tolua_function(tolua_S,"sendGiveUpTaskReqMessage",tolua_LuaClient_OnlineNetworkManager_sendGiveUpTaskReqMessage00);
   tolua_function(tolua_S,"sendExchangeParameterReq",tolua_LuaClient_OnlineNetworkManager_sendExchangeParameterReq00);
   tolua_function(tolua_S,"sendGetDailyTaskRewardReqMessage",tolua_LuaClient_OnlineNetworkManager_sendGetDailyTaskRewardReqMessage00);
   tolua_function(tolua_S,"sendGetTaskStatusMessage",tolua_LuaClient_OnlineNetworkManager_sendGetTaskStatusMessage00);
   tolua_function(tolua_S,"sendUseItem",tolua_LuaClient_OnlineNetworkManager_sendUseItem00);
   tolua_function(tolua_S,"sendEquipComposeReq",tolua_LuaClient_OnlineNetworkManager_sendEquipComposeReq00);
   tolua_function(tolua_S,"SendMoveFairyPosReq",tolua_LuaClient_OnlineNetworkManager_SendMoveFairyPosReq00);
   tolua_function(tolua_S,"sendMessage",tolua_LuaClient_OnlineNetworkManager_sendMessage00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"ENTERANCE_UNDERGROUND",ENTERANCE_UNDERGROUND);
  tolua_constant(tolua_S,"ENTERANCE_CITYDEFEND",ENTERANCE_CITYDEFEND);
  tolua_constant(tolua_S,"ENTERANCE_WORLDBOSS",ENTERANCE_WORLDBOSS);
  tolua_constant(tolua_S,"ENTERANCE_BESTFIGHTER",ENTERANCE_BESTFIGHTER);
  tolua_cclass(tolua_S,"InstanceManager","InstanceManager","",NULL);
  tolua_beginmodule(tolua_S,"InstanceManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_InstanceManager_Get00);
   tolua_function(tolua_S,"getSpecialEnteranceMapId",tolua_LuaClient_InstanceManager_getSpecialEnteranceMapId00);
   tolua_function(tolua_S,"onEnterSpecialEntrance",tolua_LuaClient_InstanceManager_onEnterSpecialEntrance00);
   tolua_function(tolua_S,"onLeaveSpecialEntrance",tolua_LuaClient_InstanceManager_onLeaveSpecialEntrance00);
   tolua_function(tolua_S,"autoSearchPathToInstance",tolua_LuaClient_InstanceManager_autoSearchPathToInstance00);
   tolua_function(tolua_S,"getOneInstanceStars",tolua_LuaClient_InstanceManager_getOneInstanceStars00);
   tolua_function(tolua_S,"getOneInstance",tolua_LuaClient_InstanceManager_getOneInstance00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"EUnKowned",EUnKowned);
  tolua_constant(tolua_S,"EBackPackItem",EBackPackItem);
  tolua_constant(tolua_S,"EStorageItem",EStorageItem);
  tolua_constant(tolua_S,"EEquipmentItem",EEquipmentItem);
  tolua_constant(tolua_S,"EUIBg",EUIBg);
  tolua_constant(tolua_S,"EAlchemyButton",EAlchemyButton);
  tolua_constant(tolua_S,"EAlchemyUpgrade",EAlchemyUpgrade);
  tolua_constant(tolua_S,"EAlchemyBag",EAlchemyBag);
  tolua_constant(tolua_S,"EAlchemyStorage",EAlchemyStorage);
  tolua_constant(tolua_S,"EAlchemyEquip",EAlchemyEquip);
  tolua_constant(tolua_S,"EALchemyToStorage",EALchemyToStorage);
  tolua_constant(tolua_S,"ESkillEquipItem",ESkillEquipItem);
  tolua_constant(tolua_S,"ESlotItem",ESlotItem);
  tolua_constant(tolua_S,"ECardBagItem",ECardBagItem);
  tolua_constant(tolua_S,"ECardEquipItem",ECardEquipItem);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"IconButton","IconButton","CCNode",tolua_collect_IconButton);
  #else
  tolua_cclass(tolua_S,"IconButton","IconButton","CCNode",NULL);
  #endif
  tolua_beginmodule(tolua_S,"IconButton");
   tolua_function(tolua_S,"create",tolua_LuaClient_IconButton_create00);
   tolua_function(tolua_S,"new",tolua_LuaClient_IconButton_new00);
   tolua_function(tolua_S,"new_local",tolua_LuaClient_IconButton_new00_local);
   tolua_function(tolua_S,".call",tolua_LuaClient_IconButton_new00_local);
   tolua_function(tolua_S,"SetButtonName",tolua_LuaClient_IconButton_SetButtonName00);
   tolua_function(tolua_S,"SetButtonIndex",tolua_LuaClient_IconButton_SetButtonIndex00);
   tolua_function(tolua_S,"GetButtonIndex",tolua_LuaClient_IconButton_GetButtonIndex00);
   tolua_function(tolua_S,"SetNormalIcon",tolua_LuaClient_IconButton_SetNormalIcon00);
   tolua_function(tolua_S,"SetSelectedIcon",tolua_LuaClient_IconButton_SetSelectedIcon00);
   tolua_function(tolua_S,"AddExternalSprite",tolua_LuaClient_IconButton_AddExternalSprite00);
   tolua_function(tolua_S,"getItemLabel",tolua_LuaClient_IconButton_getItemLabel00);
   tolua_function(tolua_S,"setItemLabel",tolua_LuaClient_IconButton_setItemLabel00);
   tolua_function(tolua_S,"removeItemLabel",tolua_LuaClient_IconButton_removeItemLabel00);
   tolua_function(tolua_S,"SetUserData",tolua_LuaClient_IconButton_SetUserData00);
   tolua_function(tolua_S,"SetDragFlagTime",tolua_LuaClient_IconButton_SetDragFlagTime00);
   tolua_function(tolua_S,"SetDoubleClickHandler",tolua_LuaClient_IconButton_SetDoubleClickHandler00);
   tolua_function(tolua_S,"SetTappedHandler",tolua_LuaClient_IconButton_SetTappedHandler00);
   tolua_function(tolua_S,"isReleaseInButton",tolua_LuaClient_IconButton_isReleaseInButton00);
   tolua_function(tolua_S,"setDragMove",tolua_LuaClient_IconButton_setDragMove00);
   tolua_function(tolua_S,"setFrameSprite",tolua_LuaClient_IconButton_setFrameSprite00);
   tolua_function(tolua_S,"getMenuItem",tolua_LuaClient_IconButton_getMenuItem00);
   tolua_function(tolua_S,"setIconOrder",tolua_LuaClient_IconButton_setIconOrder00);
   tolua_function(tolua_S,"setHandlerPriority",tolua_LuaClient_IconButton_setHandlerPriority00);
   tolua_function(tolua_S,"getHandlerPriority",tolua_LuaClient_IconButton_getHandlerPriority00);
   tolua_function(tolua_S,"creatDragReceiver",tolua_LuaClient_IconButton_creatDragReceiver00);
   tolua_function(tolua_S,"getMenuNode",tolua_LuaClient_IconButton_getMenuNode00);
   tolua_function(tolua_S,"showTipIcon",tolua_LuaClient_IconButton_showTipIcon00);
   tolua_function(tolua_S,"hideTipIcon",tolua_LuaClient_IconButton_hideTipIcon00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"UIMenu","UIMenu","CCMenu",NULL);
  tolua_beginmodule(tolua_S,"UIMenu");
   tolua_function(tolua_S,"create",tolua_LuaClient_UIMenu_create00);
   tolua_function(tolua_S,"create",tolua_LuaClient_UIMenu_create01);
   tolua_function(tolua_S,"createWithItems",tolua_LuaClient_UIMenu_createWithItems00);
   tolua_function(tolua_S,"createWithArray",tolua_LuaClient_UIMenu_createWithArray00);
   tolua_function(tolua_S,"registerWithTouchDispatcher",tolua_LuaClient_UIMenu_registerWithTouchDispatcher00);
   tolua_function(tolua_S,"ccTouchBegan",tolua_LuaClient_UIMenu_ccTouchBegan00);
   tolua_function(tolua_S,"ccTouchMoved",tolua_LuaClient_UIMenu_ccTouchMoved00);
   tolua_function(tolua_S,"ccTouchEnded",tolua_LuaClient_UIMenu_ccTouchEnded00);
   tolua_function(tolua_S,"ccTouchCancelled",tolua_LuaClient_UIMenu_ccTouchCancelled00);
   tolua_function(tolua_S,"SetDragMove",tolua_LuaClient_UIMenu_SetDragMove00);
   tolua_function(tolua_S,"SetDoubleClickHandler",tolua_LuaClient_UIMenu_SetDoubleClickHandler00);
   tolua_function(tolua_S,"SetTappedHandler",tolua_LuaClient_UIMenu_SetTappedHandler00);
   tolua_function(tolua_S,"SetNormalIcon",tolua_LuaClient_UIMenu_SetNormalIcon00);
   tolua_function(tolua_S,"checkReleaseTouchInButton",tolua_LuaClient_UIMenu_checkReleaseTouchInButton00);
   tolua_function(tolua_S,"setControlNode",tolua_LuaClient_UIMenu_setControlNode00);
   tolua_function(tolua_S,"getControlNode",tolua_LuaClient_UIMenu_getControlNode00);
   tolua_function(tolua_S,"setTouchActivityTime",tolua_LuaClient_UIMenu_setTouchActivityTime00);
   tolua_function(tolua_S,"getTouchPoint",tolua_LuaClient_UIMenu_getTouchPoint00);
   tolua_function(tolua_S,"SetDragFlagTime",tolua_LuaClient_UIMenu_SetDragFlagTime00);
   tolua_function(tolua_S,"setInitPriority",tolua_LuaClient_UIMenu_setInitPriority00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SpriteSeer","SpriteSeer","CCNode",NULL);
  tolua_beginmodule(tolua_S,"SpriteSeer");
   tolua_function(tolua_S,"MoveToSpriteTranspoint",tolua_LuaClient_SpriteSeer_MoveToSpriteTranspoint00);
   tolua_function(tolua_S,"SetAnim",tolua_LuaClient_SpriteSeer_SetAnim00);
   tolua_function(tolua_S,"SetEquipItemsData",tolua_LuaClient_SpriteSeer_SetEquipItemsData00);
   tolua_function(tolua_S,"getRoot",tolua_LuaClient_SpriteSeer_getRoot00);
   tolua_function(tolua_S,"IsHadFairy",tolua_LuaClient_SpriteSeer_IsHadFairy00);
   tolua_function(tolua_S,"IsHasFightElf",tolua_LuaClient_SpriteSeer_IsHasFightElf00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"RoleBase","RoleBase","SpriteSeer",NULL);
  tolua_beginmodule(tolua_S,"RoleBase");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TipMgr","TipMgr","",NULL);
  tolua_beginmodule(tolua_S,"TipMgr");
   tolua_function(tolua_S,"Get",tolua_LuaClient_TipMgr_Get00);
   tolua_function(tolua_S,"createTip",tolua_LuaClient_TipMgr_createTip00);
   tolua_function(tolua_S,"add",tolua_LuaClient_TipMgr_add00);
   tolua_function(tolua_S,"add",tolua_LuaClient_TipMgr_add01);
   tolua_function(tolua_S,"addPart",tolua_LuaClient_TipMgr_addPart00);
   tolua_function(tolua_S,"addPart",tolua_LuaClient_TipMgr_addPart01);
   tolua_function(tolua_S,"addNodeDone",tolua_LuaClient_TipMgr_addNodeDone00);
   tolua_function(tolua_S,"destroyTip",tolua_LuaClient_TipMgr_destroyTip00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HeroRoleManager","HeroRoleManager","",NULL);
  tolua_beginmodule(tolua_S,"HeroRoleManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_HeroRoleManager_Get00);
   tolua_function(tolua_S,"createRole",tolua_LuaClient_HeroRoleManager_createRole00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"XLogicManager","XLogicManager","",NULL);
  tolua_beginmodule(tolua_S,"XLogicManager");
   tolua_function(tolua_S,"sharedManager",tolua_LuaClient_XLogicManager_sharedManager00);
   tolua_function(tolua_S,"reqPlayerAttributes",tolua_LuaClient_XLogicManager_reqPlayerAttributes00);
   tolua_function(tolua_S,"LeaveBattle",tolua_LuaClient_XLogicManager_LeaveBattle00);
   tolua_function(tolua_S,"setHasStarReward",tolua_LuaClient_XLogicManager_setHasStarReward00);
   tolua_function(tolua_S,"networkDisConnected",tolua_LuaClient_XLogicManager_networkDisConnected00);
   tolua_function(tolua_S,"notifyShowNextAppearUI",tolua_LuaClient_XLogicManager_notifyShowNextAppearUI00);
   tolua_function(tolua_S,"fromGameBackToLogin",tolua_LuaClient_XLogicManager_fromGameBackToLogin00);
   tolua_function(tolua_S,"getAutoLogin",tolua_LuaClient_XLogicManager_getAutoLogin00);
   tolua_constant(tolua_S,"E_NOE",XLogicManager::E_NOE);
   tolua_constant(tolua_S,"E_SELECT_ZONE",XLogicManager::E_SELECT_ZONE);
   tolua_constant(tolua_S,"E_CREATE_PLAYER_LAYER",XLogicManager::E_CREATE_PLAYER_LAYER);
   tolua_constant(tolua_S,"E_SELECT_CHARACTER",XLogicManager::E_SELECT_CHARACTER);
   tolua_constant(tolua_S,"E_ENTER_MAINLAND",XLogicManager::E_ENTER_MAINLAND);
   tolua_constant(tolua_S,"E_ENTER_FB",XLogicManager::E_ENTER_FB);
   tolua_constant(tolua_S,"E_MAIN_GAME_LAYER",XLogicManager::E_MAIN_GAME_LAYER);
   tolua_constant(tolua_S,"E_GAME_HELP_LAYER",XLogicManager::E_GAME_HELP_LAYER);
   tolua_constant(tolua_S,"E_PVP_LAYER",XLogicManager::E_PVP_LAYER);
   tolua_constant(tolua_S,"E_PVE_LAYER",XLogicManager::E_PVE_LAYER);
   tolua_constant(tolua_S,"E_TASK_PANEL_LAYER",XLogicManager::E_TASK_PANEL_LAYER);
   tolua_constant(tolua_S,"E_XLOGICMANAGER",XLogicManager::E_XLOGICMANAGER);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"LoginManager","LoginManager","",NULL);
  tolua_beginmodule(tolua_S,"LoginManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_LoginManager_Get00);
   tolua_function(tolua_S,"loginWithUserName",tolua_LuaClient_LoginManager_loginWithUserName00);
   tolua_function(tolua_S,"registerWithUserName",tolua_LuaClient_LoginManager_registerWithUserName00);
   tolua_function(tolua_S,"doUserCenter",tolua_LuaClient_LoginManager_doUserCenter00);
   tolua_function(tolua_S,"doRecentLogin",tolua_LuaClient_LoginManager_doRecentLogin00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MenuItemExpand","MenuItemExpand","CCMenuItemSprite",NULL);
  tolua_beginmodule(tolua_S,"MenuItemExpand");
   tolua_function(tolua_S,"setMenuItemDispalyState",tolua_LuaClient_MenuItemExpand_setMenuItemDispalyState00);
   tolua_function(tolua_S,"setEnabled",tolua_LuaClient_MenuItemExpand_setEnabled00);
   tolua_function(tolua_S,"create",tolua_LuaClient_MenuItemExpand_create00);
   tolua_function(tolua_S,"create",tolua_LuaClient_MenuItemExpand_create01);
   tolua_function(tolua_S,"create",tolua_LuaClient_MenuItemExpand_create02);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"PB_ATTR_SVC_TIME_ZONE",PB_ATTR_SVC_TIME_ZONE);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_SS_MOD_ID",PB_ATTR_PLAYER_SS_MOD_ID);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_GEM_BACKPACK_CAPACITY",PB_ATTR_PLAYER_GEM_BACKPACK_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_RESET_GEM_TIMES",PB_ATTR_PLAYER_RESET_GEM_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_EQUIP_LVUP_FLAG",PB_ATTR_EQUIP_LVUP_FLAG);
  tolua_constant(tolua_S,"PB_ATTR_EFFECTIVENESS",PB_ATTR_EFFECTIVENESS);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_ENTER_MANOR_TIMES",PB_ATTR_PLAYER_ENTER_MANOR_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_EXPLORE_TIMES",PB_ATTR_PLAYER_EXPLORE_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_RECYCLE_BIN_START_POS",PB_ATTR_RECYCLE_BIN_START_POS);
  tolua_constant(tolua_S,"PB_ATTR_HOLD_ON_LINE_ACCU_TIME",PB_ATTR_HOLD_ON_LINE_ACCU_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_GOALS_REWARD_TIMES",PB_ATTR_GET_DAILY_GOALS_REWARD_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_UGCITY_BOXES_STATUS",PB_ATTR_UGCITY_BOXES_STATUS);
  tolua_constant(tolua_S,"PB_ATTR_UGCITY_BOXES_RESET_TIMES",PB_ATTR_UGCITY_BOXES_RESET_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_UGCITY_BOXES_OPEN_NUM",PB_ATTR_UGCITY_BOXES_OPEN_NUM);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVAI_TIMES",PB_ATTR_PLAYER_PVAI_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVP_TIMES",PB_ATTR_PLAYER_PVP_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_CDEF_TIMES",PB_ATTR_PLAYER_CDEF_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_WINNING_STREAK",PB_ATTR_PLAYER_WINNING_STREAK);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_SPEC_INSTANCE_BTL_TIME",PB_ATTR_PLAYER_SPEC_INSTANCE_BTL_TIME);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVP_RED",PB_ATTR_PLAYER_PVP_RED);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVP_RED_TIME",PB_ATTR_PLAYER_PVP_RED_TIME);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_VIP_LV",PB_ATTR_PLAYER_VIP_LV);
  tolua_constant(tolua_S,"PB_ATTR_BUY_DIAMOND_ACCU",PB_ATTR_BUY_DIAMOND_ACCU);
  tolua_constant(tolua_S,"PB_ATTR_CONTINUE_LOGIN_TIMES",PB_ATTR_CONTINUE_LOGIN_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_LOGIN_COUNT",PB_ATTR_DAILY_LOGIN_COUNT);
  tolua_constant(tolua_S,"PB_ATTR_DIAMOND_EX_POWER_TIMES",PB_ATTR_DIAMOND_EX_POWER_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_FAIRY_LOTTERY_ACCU_TIMES",PB_ATTR_FAIRY_LOTTERY_ACCU_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_MIN_DEF",PB_ATTR_CONSTEL_MIN_DEF);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_1_CAPACIT",PB_ATTR_CONSTEL_1_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_2_CAPACIT",PB_ATTR_CONSTEL_2_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_3_CAPACIT",PB_ATTR_CONSTEL_3_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_4_CAPACIT",PB_ATTR_CONSTEL_4_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_5_CAPACIT",PB_ATTR_CONSTEL_5_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_6_CAPACIT",PB_ATTR_CONSTEL_6_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_7_CAPACIT",PB_ATTR_CONSTEL_7_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_8_CAPACIT",PB_ATTR_CONSTEL_8_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_9_CAPACIT",PB_ATTR_CONSTEL_9_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_10_CAPACIT",PB_ATTR_CONSTEL_10_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_11_CAPACIT",PB_ATTR_CONSTEL_11_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_CONSTEL_12_CAPACIT",PB_ATTR_CONSTEL_12_CAPACIT);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_BEGIN",PB_ATTR_DAILY_TIMES_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_LGION",PB_ATTR_DAILY_TIMES_LGION);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_PAY",PB_ATTR_DAILY_TIMES_PAY);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_HERO_TREASURE",PB_ATTR_DAILY_TIMES_HERO_TREASURE);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_EXPLORE",PB_ATTR_DAILY_TIMES_EXPLORE);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_EQUIP_LVUP",PB_ATTR_DAILY_TIMES_EQUIP_LVUP);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_PVE_WIN",PB_ATTR_DAILY_TIMES_PVE_WIN);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_PVAI_BTL",PB_ATTR_DAILY_TIMES_PVAI_BTL);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_CARD_COMPOSE",PB_ATTR_DAILY_TIMES_CARD_COMPOSE);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_PVE2_WIN",PB_ATTR_DAILY_TIMES_PVE2_WIN);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_PPVE_WIN",PB_ATTR_DAILY_TIMES_PPVE_WIN);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_TRAINING",PB_ATTR_DAILY_TIMES_TRAINING);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_DAILY_TASK",PB_ATTR_DAILY_TIMES_DAILY_TASK);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_CDEF",PB_ATTR_DAILY_TIMES_CDEF);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_SEARCH_SS",PB_ATTR_DAILY_TIMES_SEARCH_SS);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_WBOSS",PB_ATTR_DAILY_TIMES_WBOSS);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_ENTER_MANOR",PB_ATTR_DAILY_TIMES_ENTER_MANOR);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_GEM_RESET",PB_ATTR_DAILY_TIMES_GEM_RESET);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_PVE3",PB_ATTR_DAILY_TIMES_PVE3);
  tolua_constant(tolua_S,"PB_ATTR_KAKAO_DEL_NUM_EACH_DAY",PB_ATTR_KAKAO_DEL_NUM_EACH_DAY);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_CONSUME_DIAMOND",PB_ATTR_DAILY_TIMES_CONSUME_DIAMOND);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_SKILL_LVUP",PB_ATTR_DAILY_TIMES_SKILL_LVUP);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_DOWER_LVUP",PB_ATTR_DAILY_TIMES_DOWER_LVUP);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_BABEL_INSTANCE_ID",PB_ATTR_DAILY_BABEL_INSTANCE_ID);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_TIMES_END",PB_ATTR_DAILY_TIMES_END);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_REFRESH_SHOP_TIMES",PB_ATTR_DAILY_REFRESH_SHOP_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_ACC_REFRESH_SHOP_TIMES",PB_ATTR_DAILY_ACC_REFRESH_SHOP_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_ACC_USED_REFRESH_SHOP_TIMES",PB_ATTR_DAILY_ACC_USED_REFRESH_SHOP_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_REWARD_BEGIN",PB_ATTR_GET_DAILY_REWARD_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_PAY_REWARD_TIME",PB_ATTR_GET_DAILY_PAY_REWARD_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_LOGIN_UI_FLAG",PB_ATTR_GET_DAILY_LOGIN_UI_FLAG);
  tolua_constant(tolua_S,"PB_ATTR_GET_HERO_TREASURE",PB_ATTR_GET_HERO_TREASURE);
  tolua_constant(tolua_S,"PB_ATTR_GET_PVAI_REWARD_TIME",PB_ATTR_GET_PVAI_REWARD_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_HOLD_EXP_TIME",PB_ATTR_GET_HOLD_EXP_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_REWARD_BY_VIP_LV",PB_ATTR_GET_REWARD_BY_VIP_LV);
  tolua_constant(tolua_S,"PB_ATTR_GET_FIRST_BUY_DIAMOND_REWARD_TIME",PB_ATTR_GET_FIRST_BUY_DIAMOND_REWARD_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_THREE_DAY_REWARD_ENABLE_FLAG",PB_ATTR_GET_THREE_DAY_REWARD_ENABLE_FLAG);
  tolua_constant(tolua_S,"PB_ATTR_GET_STAMINA_RESTORE_TIME",PB_ATTR_GET_STAMINA_RESTORE_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_LOGIN_REWARD_TIME",PB_ATTR_GET_DAILY_LOGIN_REWARD_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_VIP_REWARD_TIME",PB_ATTR_GET_DAILY_VIP_REWARD_TIME);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_LOGIN_TIMES",PB_ATTR_GET_DAILY_LOGIN_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK",PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_VIP_LOGIN_REWARD_MARK",PB_ATTR_GET_DAILY_VIP_LOGIN_REWARD_MARK);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_FACEBOOK_SHARE_MARK",PB_ATTR_GET_DAILY_FACEBOOK_SHARE_MARK);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_REP_EXCHANGE_FLAG",PB_ATTR_GET_DAILY_REP_EXCHANGE_FLAG);
  tolua_constant(tolua_S,"PB_ATTR_GET_DAILY_REWARD_END",PB_ATTR_GET_DAILY_REWARD_END);
  tolua_constant(tolua_S,"PB_ATTR_GET_SPEC_FAIRY_BEIGN",PB_ATTR_GET_SPEC_FAIRY_BEIGN);
  tolua_constant(tolua_S,"PB_ATTR_GET_SPEC_P_FAIRY_LEFT_TIMES",PB_ATTR_GET_SPEC_P_FAIRY_LEFT_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_FAIRY_COLLECTION_BEGIN",PB_ATTR_FAIRY_COLLECTION_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_FAIRY_COLLECTION_END",PB_ATTR_FAIRY_COLLECTION_END);
  tolua_constant(tolua_S,"PB_ATTR_SPECIAL_VIP_GIFT_BEGIN",PB_ATTR_SPECIAL_VIP_GIFT_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_91_VIP_DAILY_GIFT_GET_TIME",PB_ATTR_91_VIP_DAILY_GIFT_GET_TIME);
  tolua_constant(tolua_S,"PB_ATTR_91_VIP_GIFT_GET_TIME",PB_ATTR_91_VIP_GIFT_GET_TIME);
  tolua_constant(tolua_S,"PB_ATTR_91_VIP_TOPUS_STATUS",PB_ATTR_91_VIP_TOPUS_STATUS);
  tolua_constant(tolua_S,"PB_ATTR_91_VIP_STATUS",PB_ATTR_91_VIP_STATUS);
  tolua_constant(tolua_S,"PB_ATTR_SPECIAL_VIP_GIFT_END",PB_ATTR_SPECIAL_VIP_GIFT_END);
  tolua_constant(tolua_S,"PB_ATTR_EXPLORE_DAILY_TIMES",PB_ATTR_EXPLORE_DAILY_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_EXPLORE_MAP_INDEX",PB_ATTR_EXPLORE_MAP_INDEX);
  tolua_constant(tolua_S,"PB_ATTR_EXPLORE_SUCCESS_RATE",PB_ATTR_EXPLORE_SUCCESS_RATE);
  tolua_constant(tolua_S,"PB_ATTR_EXPLORE_HAVE_GOT_MAP_INDEX",PB_ATTR_EXPLORE_HAVE_GOT_MAP_INDEX);
  tolua_constant(tolua_S,"PB_ATTR_EXPLORE_REWARD_LEVELUP_FLAG",PB_ATTR_EXPLORE_REWARD_LEVELUP_FLAG);
  tolua_constant(tolua_S,"PB_ATTR_JUNIOR_EXPLORE_TIME",PB_ATTR_JUNIOR_EXPLORE_TIME);
  tolua_constant(tolua_S,"PB_ATTR_SENIOR_EXPLORE_TIME",PB_ATTR_SENIOR_EXPLORE_TIME);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MIN_CD",PB_ATTR_DEF_MIN_CD);
  tolua_constant(tolua_S,"PB_ATTR_EQUIP_LVUP_CD",PB_ATTR_EQUIP_LVUP_CD);
  tolua_constant(tolua_S,"PB_ATTR_DOWER_LVUP_CD",PB_ATTR_DOWER_LVUP_CD);
  tolua_constant(tolua_S,"PB_ATTR_SKILL_LVUP_CD",PB_ATTR_SKILL_LVUP_CD);
  tolua_constant(tolua_S,"PB_ATTR_PVAI_CD",PB_ATTR_PVAI_CD);
  tolua_constant(tolua_S,"PB_ATTR_AB_NORMAL_MAP_CD",PB_ATTR_AB_NORMAL_MAP_CD);
  tolua_constant(tolua_S,"PB_ATTR_AB_SPECIAL_MAP_CD",PB_ATTR_AB_SPECIAL_MAP_CD);
  tolua_constant(tolua_S,"PB_ATTR_AB_UGCITY_MAP_CD",PB_ATTR_AB_UGCITY_MAP_CD);
  tolua_constant(tolua_S,"PB_ATTR_WOELD_BOSS_CD",PB_ATTR_WOELD_BOSS_CD);
  tolua_constant(tolua_S,"PB_ATTR_MANOR_CD",PB_ATTR_MANOR_CD);
  tolua_constant(tolua_S,"PB_ATTR_SHOP_CD",PB_ATTR_SHOP_CD);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MAX_CD",PB_ATTR_DEF_MAX_CD);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MIN_OTHER",PB_ATTR_DEF_MIN_OTHER);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MAX_OTHER",PB_ATTR_DEF_MAX_OTHER);
  tolua_constant(tolua_S,"PB_ATTR_RESET_GEM_BUY_TIMES",PB_ATTR_RESET_GEM_BUY_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_MANOR_BUY_TIMES",PB_ATTR_MANOR_BUY_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PVAI_BUY_TIMES",PB_ATTR_PVAI_BUY_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MAX_BUY_TIMES",PB_ATTR_DEF_MAX_BUY_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MIN_CAPACITY",PB_ATTR_DEF_MIN_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_MANOR_CAPACITY",PB_ATTR_BUY_MANOR_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_SKILL_CAPACITY",PB_ATTR_BUY_SKILL_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_CONSTEL_CAPACITY",PB_ATTR_BUY_CONSTEL_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_EQUIP_HOLE_CAPACITY",PB_ATTR_BUY_EQUIP_HOLE_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_SS_BAG_CAPACITY",PB_ATTR_BUY_SS_BAG_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_BAG_CAPACITY",PB_ATTR_BUY_BAG_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_BUY_STORE_CAPACITY",PB_ATTR_BUY_STORE_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_DEF_MAX_CAPACITY",PB_ATTR_DEF_MAX_CAPACITY);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_STAMINA",PB_ATTR_PLAYER_STAMINA);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_COIN",PB_ATTR_PLAYER_COIN);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_DIAMOND",PB_ATTR_PLAYER_DIAMOND);
  tolua_constant(tolua_S,"PB_ATTR_EXP",PB_ATTR_EXP);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PRESTIGE",PB_ATTR_PLAYER_PRESTIGE);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_GEM_ANIMA",PB_ATTR_PLAYER_GEM_ANIMA);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_SS_CHIP",PB_ATTR_PLAYER_SS_CHIP);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_EXPLOIT",PB_ATTR_PLAYER_EXPLOIT);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_EXPLORE_EXP",PB_ATTR_PLAYER_EXPLORE_EXP);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_HERALDRY",PB_ATTR_PLAYER_HERALDRY);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_SP",PB_ATTR_PLAYER_SP);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVP2_SCORE",PB_ATTR_PLAYER_PVP2_SCORE);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_REFINE_STONE",PB_ATTR_PLAYER_REFINE_STONE);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_POTENTIAL_DRUG",PB_ATTR_PLAYER_POTENTIAL_DRUG);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_SHOP_REF_TOKEN",PB_ATTR_PLAYER_SHOP_REF_TOKEN);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_SOUL_STONE",PB_ATTR_PLAYER_SOUL_STONE);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_FAIRY_ELEM",PB_ATTR_PLAYER_FAIRY_ELEM);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_EPLORE_TICKET",PB_ATTR_PLAYER_EPLORE_TICKET);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_CONTRIBUTE",PB_ATTR_PLAYER_CONTRIBUTE);
  tolua_constant(tolua_S,"PB_ATTR_DOWER_LIGHT",PB_ATTR_DOWER_LIGHT);
  tolua_constant(tolua_S,"PB_ATTR_TITLE_WEAR",PB_ATTR_TITLE_WEAR);
  tolua_constant(tolua_S,"PB_ATTR_STAR_NUM",PB_ATTR_STAR_NUM);
  tolua_constant(tolua_S,"PB_ATTR_STAR_CONSUME",PB_ATTR_STAR_CONSUME);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_HOLD_EXP",PB_ATTR_PLAYER_HOLD_EXP);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_STORE_EXP",PB_ATTR_PLAYER_STORE_EXP);
  tolua_constant(tolua_S,"PB_ATTR_DAILY_GOALS_ACTIVITY",PB_ATTR_DAILY_GOALS_ACTIVITY);
  tolua_constant(tolua_S,"PB_ATTR_GROWUP_STATUS",PB_ATTR_GROWUP_STATUS);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_LAST_RESTORE_POWER_TIME",PB_ATTR_PLAYER_LAST_RESTORE_POWER_TIME);
  tolua_constant(tolua_S,"PB_ATTR_EXCHANGE_TIMES_BEGIN",PB_ATTR_EXCHANGE_TIMES_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_EXCHANGE_TIMES_END",PB_ATTR_EXCHANGE_TIMES_END);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_GUILD_LAST_LEAVE_TIME",PB_ATTR_PLAYER_GUILD_LAST_LEAVE_TIME);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_GUILD_DIALY_GOLD_BLESS",PB_ATTR_PLAYER_GUILD_DIALY_GOLD_BLESS);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_GUILD_CONTRIBUTE_TIMES",PB_ATTR_PLAYER_GUILD_CONTRIBUTE_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_GUILD_INSTANCE_TIMES",PB_ATTR_PLAYER_GUILD_INSTANCE_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_SEGMENT_DEF",PB_ATTR_SEGMENT_DEF);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_LAST_LOGIN_MAP",PB_ATTR_PLAYER_LAST_LOGIN_MAP);
  tolua_constant(tolua_S,"PB_ATTR_LAST_EXPLORE_ID",PB_ATTR_LAST_EXPLORE_ID);
  tolua_constant(tolua_S,"PB_ATTR_SET_ONCE_BEGIN",PB_ATTR_SET_ONCE_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_SET_ONCE_END",PB_ATTR_SET_ONCE_END);
  tolua_constant(tolua_S,"PB_ATTR_SET_BEGIN",PB_ATTR_SET_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_READY_SKILL_START",PB_ATTR_READY_SKILL_START);
  tolua_constant(tolua_S,"PB_ATTR_READY_SKILL_END",PB_ATTR_READY_SKILL_END);
  tolua_constant(tolua_S,"PB_ATTR_MXA_VIEW_PLAYER",PB_ATTR_MXA_VIEW_PLAYER);
  tolua_constant(tolua_S,"PB_ATTR_GAME_PIC_QUALITY",PB_ATTR_GAME_PIC_QUALITY);
  tolua_constant(tolua_S,"PB_ATTR_WBOSS_VIEW_NUMBER",PB_ATTR_WBOSS_VIEW_NUMBER);
  tolua_constant(tolua_S,"PB_ATTR_AUTO_COST_DIAMOND",PB_ATTR_AUTO_COST_DIAMOND);
  tolua_constant(tolua_S,"PB_ATTR_AUTO_WAIT_TIMEOUT",PB_ATTR_AUTO_WAIT_TIMEOUT);
  tolua_constant(tolua_S,"PB_ATTR_SET_END",PB_ATTR_SET_END);
  tolua_constant(tolua_S,"PB_ATTR_DOWER_START",PB_ATTR_DOWER_START);
  tolua_constant(tolua_S,"PB_ATTR_DOWER_END",PB_ATTR_DOWER_END);
  tolua_constant(tolua_S,"PB_ATTR_SHOP_ITEM_START",PB_ATTR_SHOP_ITEM_START);
  tolua_constant(tolua_S,"PB_ATTR_SHOP_ITEM_END",PB_ATTR_SHOP_ITEM_END);
  tolua_constant(tolua_S,"PB_ATTR_SHOP_ITEM_BUY_START",PB_ATTR_SHOP_ITEM_BUY_START);
  tolua_constant(tolua_S,"PB_ATTR_SHOP_ITEM_BUY_END",PB_ATTR_SHOP_ITEM_BUY_END);
  tolua_constant(tolua_S,"PB_ATTR_SPIRIT_SYS_NOT_FIRST_USED",PB_ATTR_SPIRIT_SYS_NOT_FIRST_USED);
  tolua_constant(tolua_S,"PB_ATTR_SPOP_SYS_NOT_FIRST_USED",PB_ATTR_SPOP_SYS_NOT_FIRST_USED);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_LAST_LOGIN_OUT_TIME",PB_ATTR_PLAYER_LAST_LOGIN_OUT_TIME);
  tolua_constant(tolua_S,"PB_ATTR_SET_ONCE_DAILY_BEGIN",PB_ATTR_SET_ONCE_DAILY_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_SET_ONCE_DAILY_END",PB_ATTR_SET_ONCE_DAILY_END);
  tolua_constant(tolua_S,"PB_ATTR_PPVE_TIMES_DAILY_BEGIN",PB_ATTR_PPVE_TIMES_DAILY_BEGIN);
  tolua_constant(tolua_S,"PB_ATTR_PPVE_TIMES_DAILY_END",PB_ATTR_PPVE_TIMES_DAILY_END);
  tolua_constant(tolua_S,"PB_ATTR_PHYSICAL_ATTACK_ADD",PB_ATTR_PHYSICAL_ATTACK_ADD);
  tolua_constant(tolua_S,"PB_ATTR_MAGIC_ATTACK_ADD",PB_ATTR_MAGIC_ATTACK_ADD);
  tolua_constant(tolua_S,"PB_ATTR_SKILL_ATTACK_ADD",PB_ATTR_SKILL_ATTACK_ADD);
  tolua_constant(tolua_S,"PB_ATTR_PHYSICAL_DEFENCE_ADD",PB_ATTR_PHYSICAL_DEFENCE_ADD);
  tolua_constant(tolua_S,"PB_ATTR_MAGIC_DEFENCE_ADD",PB_ATTR_MAGIC_DEFENCE_ADD);
  tolua_constant(tolua_S,"PB_ATTR_SKILL_DEFENCE_ADD",PB_ATTR_SKILL_DEFENCE_ADD);
  tolua_constant(tolua_S,"PB_ATTR_HP_ADD",PB_ATTR_HP_ADD);
  tolua_constant(tolua_S,"PB_ATTR_COURAGE_ADD",PB_ATTR_COURAGE_ADD);
  tolua_constant(tolua_S,"PB_ATTR_CHARM_ADD",PB_ATTR_CHARM_ADD);
  tolua_constant(tolua_S,"PB_ATTR_TRICK_ADD",PB_ATTR_TRICK_ADD);
  tolua_constant(tolua_S,"PB_ATTR_AUTO_BTL_INSTANCE_ID",PB_ATTR_AUTO_BTL_INSTANCE_ID);
  tolua_constant(tolua_S,"PB_ATTR_AUTO_BTL_REMAIN_TURN",PB_ATTR_AUTO_BTL_REMAIN_TURN);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVP_DUEL_TIMES",PB_ATTR_PLAYER_PVP_DUEL_TIMES);
  tolua_constant(tolua_S,"PB_ATTR_PLAYER_PVP_DUELED_TIMES",PB_ATTR_PLAYER_PVP_DUELED_TIMES);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_PHYATK",PB_ITEM_ATTR_PHYATK);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_MAGATK",PB_ITEM_ATTR_MAGATK);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_SKIATK",PB_ITEM_ATTR_SKIATK);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_PHYDEF",PB_ITEM_ATTR_PHYDEF);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_MAGDEF",PB_ITEM_ATTR_MAGDEF);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_SKIDEF",PB_ITEM_ATTR_SKIDEF);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_HP",PB_ITEM_ATTR_HP);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_ACCU",PB_ITEM_ATTR_ACCU);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_DODG",PB_ITEM_ATTR_DODG);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_WRK",PB_ITEM_ATTR_WRK);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_PARRY",PB_ITEM_ATTR_PARRY);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_CRIT",PB_ITEM_ATTR_CRIT);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_TENA",PB_ITEM_ATTR_TENA);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_SLY",PB_ITEM_ATTR_SLY);
  tolua_constant(tolua_S,"PB_ITEM_ATTR_SPEED",PB_ITEM_ATTR_SPEED);
  tolua_constant(tolua_S,"PB_TASK_CANT_TOUCH",PB_TASK_CANT_TOUCH);
  tolua_constant(tolua_S,"PB_TASK_TOUCH",PB_TASK_TOUCH);
  tolua_constant(tolua_S,"PB_TASK_FINISH",PB_TASK_FINISH);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVE",PB_INSTANCE_TYPE_PVE);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PPVE",PB_INSTANCE_TYPE_PPVE);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVP",PB_INSTANCE_TYPE_PVP);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVAI",PB_INSTANCE_TYPE_PVAI);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVE2",PB_INSTANCE_TYPE_PVE2);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVE3",PB_INSTANCE_TYPE_PVE3);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVAI2",PB_INSTANCE_TYPE_PVAI2);
  tolua_constant(tolua_S,"PB_INSTANCE_TYPE_PVP2",PB_INSTANCE_TYPE_PVP2);
  tolua_constant(tolua_S,"PB_SA_CDEF_ID",PB_SA_CDEF_ID);
  tolua_constant(tolua_S,"PB_SA_WBOSS_ID",PB_SA_WBOSS_ID);
  tolua_constant(tolua_S,"PB_SA_PVP2_ID",PB_SA_PVP2_ID);
  tolua_constant(tolua_S,"ITEM_TARGET_BAG",ITEM_TARGET_BAG);
  tolua_constant(tolua_S,"ITEM_TARGET_STORE",ITEM_TARGET_STORE);
  tolua_constant(tolua_S,"ITEM_TARGET_CARD",ITEM_TARGET_CARD);
  tolua_constant(tolua_S,"ITEM_TARGET_EQUIP_CHIP",ITEM_TARGET_EQUIP_CHIP);
  tolua_constant(tolua_S,"ITEM_TARGET_MONSTER_CHIP",ITEM_TARGET_MONSTER_CHIP);
  tolua_constant(tolua_S,"PB_AI_STATUS_TOP10",PB_AI_STATUS_TOP10);
  tolua_constant(tolua_S,"PB_AI_STATUS_FRONT",PB_AI_STATUS_FRONT);
  tolua_constant(tolua_S,"PB_AI_STATUS_SELF",PB_AI_STATUS_SELF);
  tolua_constant(tolua_S,"PB_AI_STATUS_BACK",PB_AI_STATUS_BACK);
  tolua_constant(tolua_S,"WORLD",WORLD);
  tolua_constant(tolua_S,"PARTY",PARTY);
  tolua_constant(tolua_S,"PM",PM);
  tolua_constant(tolua_S,"SUCC",SUCC);
  tolua_constant(tolua_S,"FAIL",FAIL);
  tolua_constant(tolua_S,"NOTALLOW",NOTALLOW);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_NOT_START",PB_ACTIVITY_STATUS_NOT_START);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_VALID",PB_ACTIVITY_STATUS_VALID);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_UNFINISH",PB_ACTIVITY_STATUS_UNFINISH);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_FINISH",PB_ACTIVITY_STATUS_FINISH);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_REWARD_REVC",PB_ACTIVITY_STATUS_REWARD_REVC);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_END",PB_ACTIVITY_STATUS_END);
  tolua_constant(tolua_S,"PB_ACTIVITY_STATUS_INVALID",PB_ACTIVITY_STATUS_INVALID);
  tolua_constant(tolua_S,"PB_ACTIVITY_RANK_QUERY_SELF",PB_ACTIVITY_RANK_QUERY_SELF);
  tolua_constant(tolua_S,"PB_ACTIVITY_RANK_QUERY_TOTLE",PB_ACTIVITY_RANK_QUERY_TOTLE);
  tolua_constant(tolua_S,"GET_ON_GIFT_SUCC",GET_ON_GIFT_SUCC);
  tolua_constant(tolua_S,"ON_GIFT_TIME_NOT_CLEAR",ON_GIFT_TIME_NOT_CLEAR);
  tolua_constant(tolua_S,"NONE_TOPUP_AWARD",NONE_TOPUP_AWARD);
  tolua_constant(tolua_S,"DONE_TOPUP_NOT_GET_AWARD",DONE_TOPUP_NOT_GET_AWARD);
  tolua_constant(tolua_S,"DONE_TOPUP_AND_GET_AWARD",DONE_TOPUP_AND_GET_AWARD);
  tolua_constant(tolua_S,"WAITTING_FOR_TOPUP",WAITTING_FOR_TOPUP);
  tolua_constant(tolua_S,"GET_TOPUP_AWARD_SUCCESS",GET_TOPUP_AWARD_SUCCESS);
  tolua_constant(tolua_S,"PB_PVP2_BTL_RET_WIN",PB_PVP2_BTL_RET_WIN);
  tolua_constant(tolua_S,"PB_PVP2_BTL_RET_FAIL",PB_PVP2_BTL_RET_FAIL);
  tolua_constant(tolua_S,"PB_PVP2_BTL_RET_UNMATCH",PB_PVP2_BTL_RET_UNMATCH);
  tolua_constant(tolua_S,"RANK_NAME_PVP2_SCORE",RANK_NAME_PVP2_SCORE);
  tolua_constant(tolua_S,"RANK_NAME_LV_SCORE",RANK_NAME_LV_SCORE);
  tolua_constant(tolua_S,"RANK_NAME_POWER_SCORE",RANK_NAME_POWER_SCORE);
  tolua_constant(tolua_S,"RANK_NAME_REPUTATION_SCORE",RANK_NAME_REPUTATION_SCORE);
  tolua_constant(tolua_S,"RANK_NAME_FAIRY_SCORE",RANK_NAME_FAIRY_SCORE);
  tolua_constant(tolua_S,"RANK_NAME_WB_HURT_SCORE",RANK_NAME_WB_HURT_SCORE);
  tolua_constant(tolua_S,"RANK_NAME_ACTIVITY_MIN",RANK_NAME_ACTIVITY_MIN);
  tolua_constant(tolua_S,"RANK_NAME_ACTIVITY_MAX",RANK_NAME_ACTIVITY_MAX);
  tolua_constant(tolua_S,"RANK_TYPE_TOTAL",RANK_TYPE_TOTAL);
  tolua_constant(tolua_S,"RANK_TYPE_DAILY",RANK_TYPE_DAILY);
  tolua_constant(tolua_S,"RANK_TYPE_WEEK",RANK_TYPE_WEEK);
  tolua_constant(tolua_S,"RANK_TYPE_MONTH",RANK_TYPE_MONTH);
  tolua_constant(tolua_S,"PB_MAIL_STATUS_NEW",PB_MAIL_STATUS_NEW);
  tolua_constant(tolua_S,"PB_MAIL_STATUS_UNREAD",PB_MAIL_STATUS_UNREAD);
  tolua_constant(tolua_S,"PB_MAIL_STATUS_READ",PB_MAIL_STATUS_READ);
  tolua_constant(tolua_S,"PB_MAIL_STATUS_GOT",PB_MAIL_STATUS_GOT);
  tolua_constant(tolua_S,"PB_MAIL_STATUS_DEL",PB_MAIL_STATUS_DEL);
  tolua_constant(tolua_S,"EXCHANGE",EXCHANGE);
  tolua_constant(tolua_S,"INVITE",INVITE);
  tolua_constant(tolua_S,"FAIRY_POS_START_BTL",FAIRY_POS_START_BTL);
  tolua_constant(tolua_S,"FAIRY_POS_START_AST",FAIRY_POS_START_AST);
  tolua_constant(tolua_S,"FAIRY_POS_START_PRT",FAIRY_POS_START_PRT);
  tolua_constant(tolua_S,"FAIRY_POS_START_NOM",FAIRY_POS_START_NOM);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_NOT_START",PB_TARGET_STATUS_NOT_START);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_VALID",PB_TARGET_STATUS_VALID);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_UNFINISH",PB_TARGET_STATUS_UNFINISH);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_FINISH",PB_TARGET_STATUS_FINISH);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_REWARD_REVC",PB_TARGET_STATUS_REWARD_REVC);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_END",PB_TARGET_STATUS_END);
  tolua_constant(tolua_S,"PB_TARGET_STATUS_INVALID",PB_TARGET_STATUS_INVALID);
  tolua_constant(tolua_S,"GUILD_TO_MEMBER",GUILD_TO_MEMBER);
  tolua_constant(tolua_S,"GUILD_TO_ELITE",GUILD_TO_ELITE);
  tolua_constant(tolua_S,"GUILD_TO_VICE",GUILD_TO_VICE);
  tolua_constant(tolua_S,"GUILD_TO_OWNER",GUILD_TO_OWNER);
  tolua_constant(tolua_S,"GUILD_TO_NOT_MEMBER",GUILD_TO_NOT_MEMBER);
  tolua_constant(tolua_S,"GUILD_TO_REFUSE",GUILD_TO_REFUSE);
  tolua_cclass(tolua_S,"LevelLayer","LevelLayer","cocos2d::CCLayer",NULL);
  tolua_beginmodule(tolua_S,"LevelLayer");
   tolua_function(tolua_S,"sCalcZorder",tolua_LuaClient_LevelLayer_sCalcZorder00);
   tolua_function(tolua_S,"sCalcDepth",tolua_LuaClient_LevelLayer_sCalcDepth00);
   tolua_function(tolua_S,"sGetListenerByTag",tolua_LuaClient_LevelLayer_sGetListenerByTag00);
   tolua_function(tolua_S,"getSpriteSeer",tolua_LuaClient_LevelLayer_getSpriteSeer00);
   tolua_function(tolua_S,"getFollowSprite",tolua_LuaClient_LevelLayer_getFollowSprite00);
   tolua_function(tolua_S,"getObjectCount",tolua_LuaClient_LevelLayer_getObjectCount00);
   tolua_function(tolua_S,"getRandomPoint",tolua_LuaClient_LevelLayer_getRandomPoint00);
   tolua_function(tolua_S,"getSeerBornPoint",tolua_LuaClient_LevelLayer_getSeerBornPoint00);
   tolua_function(tolua_S,"getID",tolua_LuaClient_LevelLayer_getID00);
   tolua_function(tolua_S,"getPhysicsLayer",tolua_LuaClient_LevelLayer_getPhysicsLayer00);
   tolua_function(tolua_S,"GetData",tolua_LuaClient_LevelLayer_GetData00);
   tolua_function(tolua_S,"getMapSizeInPixel",tolua_LuaClient_LevelLayer_getMapSizeInPixel00);
   tolua_function(tolua_S,"getMapTileSizeInPixel",tolua_LuaClient_LevelLayer_getMapTileSizeInPixel00);
   tolua_function(tolua_S,"getMapRect",tolua_LuaClient_LevelLayer_getMapRect00);
   tolua_function(tolua_S,"getObjectLayer",tolua_LuaClient_LevelLayer_getObjectLayer00);
   tolua_function(tolua_S,"getMainLandTranspoint",tolua_LuaClient_LevelLayer_getMainLandTranspoint00);
   tolua_function(tolua_S,"getEliteTranspoint",tolua_LuaClient_LevelLayer_getEliteTranspoint00);
   tolua_function(tolua_S,"addOtherPlayer",tolua_LuaClient_LevelLayer_addOtherPlayer00);
   tolua_function(tolua_S,"removeOtherPlayer",tolua_LuaClient_LevelLayer_removeOtherPlayer00);
   tolua_function(tolua_S,"updateOtherPlayer",tolua_LuaClient_LevelLayer_updateOtherPlayer00);
   tolua_function(tolua_S,"DoOtherPlayerSkill",tolua_LuaClient_LevelLayer_DoOtherPlayerSkill00);
   tolua_function(tolua_S,"DoElfSkill",tolua_LuaClient_LevelLayer_DoElfSkill00);
   tolua_function(tolua_S,"getPlayer",tolua_LuaClient_LevelLayer_getPlayer00);
   tolua_function(tolua_S,"EquipOnePlayerWeapon",tolua_LuaClient_LevelLayer_EquipOnePlayerWeapon00);
   tolua_function(tolua_S,"addMonster",tolua_LuaClient_LevelLayer_addMonster00);
   tolua_function(tolua_S,"removeMonster",tolua_LuaClient_LevelLayer_removeMonster00);
   tolua_function(tolua_S,"ShowMonsterOrNot",tolua_LuaClient_LevelLayer_ShowMonsterOrNot00);
   tolua_function(tolua_S,"showTouchEffect",tolua_LuaClient_LevelLayer_showTouchEffect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"FeedbackLayer","FeedbackLayer","CCLayer",NULL);
  tolua_beginmodule(tolua_S,"FeedbackLayer");
   tolua_function(tolua_S,"Get",tolua_LuaClient_FeedbackLayer_Get00);
   tolua_function(tolua_S,"callbackHotSpot",tolua_LuaClient_FeedbackLayer_callbackHotSpot00);
   tolua_function(tolua_S,"callbackOnline",tolua_LuaClient_FeedbackLayer_callbackOnline00);
   tolua_function(tolua_S,"callbackForum",tolua_LuaClient_FeedbackLayer_callbackForum00);
   tolua_function(tolua_S,"callbackWeb",tolua_LuaClient_FeedbackLayer_callbackWeb00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HtmlNode","HtmlNode","CCNode",NULL);
  tolua_beginmodule(tolua_S,"HtmlNode");
   tolua_function(tolua_S,"create",tolua_LuaClient_HtmlNode_create00);
   tolua_function(tolua_S,"begin",tolua_LuaClient_HtmlNode_begin00);
   tolua_function(tolua_S,"addLabel",tolua_LuaClient_HtmlNode_addLabel00);
   tolua_function(tolua_S,"addImage",tolua_LuaClient_HtmlNode_addImage00);
   tolua_function(tolua_S,"addHyperLink",tolua_LuaClient_HtmlNode_addHyperLink00);
   tolua_function(tolua_S,"finish",tolua_LuaClient_HtmlNode_finish00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SpriteMonsterMgr","SpriteMonsterMgr","",NULL);
  tolua_beginmodule(tolua_S,"SpriteMonsterMgr");
   tolua_function(tolua_S,"Get",tolua_LuaClient_SpriteMonsterMgr_Get00);
   tolua_function(tolua_S,"CreateCacheMonster",tolua_LuaClient_SpriteMonsterMgr_CreateCacheMonster00);
   tolua_function(tolua_S,"updateSpecialMonsterHPLayer",tolua_LuaClient_SpriteMonsterMgr_updateSpecialMonsterHPLayer00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HSJoystick","HSJoystick","",NULL);
  tolua_beginmodule(tolua_S,"HSJoystick");
   tolua_function(tolua_S,"sharedJoystick",tolua_LuaClient_HSJoystick_sharedJoystick00);
   tolua_function(tolua_S,"setIsEnable",tolua_LuaClient_HSJoystick_setIsEnable00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameDataManager","GameDataManager","",NULL);
  tolua_beginmodule(tolua_S,"GameDataManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_GameDataManager_Get00);
   tolua_function(tolua_S,"setHeroAutoAttack",tolua_LuaClient_GameDataManager_setHeroAutoAttack00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"OneShopItemInfo","OneShopItemInfo","",tolua_collect_OneShopItemInfo);
  #else
  tolua_cclass(tolua_S,"OneShopItemInfo","OneShopItemInfo","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"OneShopItemInfo");
   tolua_variable(tolua_S,"product_id",tolua_get_OneShopItemInfo_unsigned_product_id,tolua_set_OneShopItemInfo_unsigned_product_id);
   tolua_variable(tolua_S,"item_id",tolua_get_OneShopItemInfo_unsigned_item_id,tolua_set_OneShopItemInfo_unsigned_item_id);
   tolua_variable(tolua_S,"item_base_num",tolua_get_OneShopItemInfo_unsigned_item_base_num,tolua_set_OneShopItemInfo_unsigned_item_base_num);
   tolua_variable(tolua_S,"item_gift_num",tolua_get_OneShopItemInfo_unsigned_item_gift_num,tolua_set_OneShopItemInfo_unsigned_item_gift_num);
   tolua_variable(tolua_S,"product_name",tolua_get_OneShopItemInfo_product_name,tolua_set_OneShopItemInfo_product_name);
   tolua_variable(tolua_S,"product_price",tolua_get_OneShopItemInfo_unsigned_product_price,tolua_set_OneShopItemInfo_unsigned_product_price);
   tolua_variable(tolua_S,"product_orig_price",tolua_get_OneShopItemInfo_unsigned_product_orig_price,tolua_set_OneShopItemInfo_unsigned_product_orig_price);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ShopItemInfoFromServer","ShopItemInfoFromServer","",NULL);
  tolua_beginmodule(tolua_S,"ShopItemInfoFromServer");
   tolua_function(tolua_S,"sharedInstance",tolua_LuaClient_ShopItemInfoFromServer_sharedInstance00);
   tolua_function(tolua_S,"GetOneShopItemInfo",tolua_LuaClient_ShopItemInfoFromServer_GetOneShopItemInfo00);
   tolua_function(tolua_S,"PrepareNeedDataForLua",tolua_LuaClient_ShopItemInfoFromServer_PrepareNeedDataForLua00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SystemConditionManager","SystemConditionManager","",NULL);
  tolua_beginmodule(tolua_S,"SystemConditionManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_SystemConditionManager_Get00);
   tolua_function(tolua_S,"GetTaskStatus",tolua_LuaClient_SystemConditionManager_GetTaskStatus00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PayManager","PayManager","",NULL);
  tolua_beginmodule(tolua_S,"PayManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_PayManager_Get00);
   tolua_function(tolua_S,"loadPlugin",tolua_LuaClient_PayManager_loadPlugin00);
   tolua_function(tolua_S,"unloadPlugin",tolua_LuaClient_PayManager_unloadPlugin00);
   tolua_function(tolua_S,"payForProduct",tolua_LuaClient_PayManager_payForProduct00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"UIControlAppearControler","UIControlAppearControler","",NULL);
  tolua_beginmodule(tolua_S,"UIControlAppearControler");
   tolua_function(tolua_S,"Get",tolua_LuaClient_UIControlAppearControler_Get00);
   tolua_function(tolua_S,"InsertOneTutorialAndTask",tolua_LuaClient_UIControlAppearControler_InsertOneTutorialAndTask00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"WaitingLayerManager","WaitingLayerManager","",NULL);
  tolua_beginmodule(tolua_S,"WaitingLayerManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_WaitingLayerManager_Get00);
   tolua_function(tolua_S,"PushOneMessage",tolua_LuaClient_WaitingLayerManager_PushOneMessage00);
   tolua_function(tolua_S,"clearMessages",tolua_LuaClient_WaitingLayerManager_clearMessages00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameFontManager","GameFontManager","",NULL);
  tolua_beginmodule(tolua_S,"GameFontManager");
   tolua_function(tolua_S,"superFontSize",tolua_LuaClient_GameFontManager_superFontSize00);
   tolua_function(tolua_S,"largeFontSize",tolua_LuaClient_GameFontManager_largeFontSize00);
   tolua_function(tolua_S,"smallFontSize",tolua_LuaClient_GameFontManager_smallFontSize00);
   tolua_function(tolua_S,"miniFontSize",tolua_LuaClient_GameFontManager_miniFontSize00);
   tolua_function(tolua_S,"createPlayerLabelFontSize",tolua_LuaClient_GameFontManager_createPlayerLabelFontSize00);
   tolua_function(tolua_S,"smallTitleFontSize",tolua_LuaClient_GameFontManager_smallTitleFontSize00);
   tolua_function(tolua_S,"smallTipFontSize",tolua_LuaClient_GameFontManager_smallTipFontSize00);
   tolua_function(tolua_S,"captureItemTipNameFontSize",tolua_LuaClient_GameFontManager_captureItemTipNameFontSize00);
   tolua_function(tolua_S,"captureItemTipCountFontSize",tolua_LuaClient_GameFontManager_captureItemTipCountFontSize00);
   tolua_function(tolua_S,"captureLayerFontSize",tolua_LuaClient_GameFontManager_captureLayerFontSize00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CallNativeFuncManager","CallNativeFuncManager","",NULL);
  tolua_beginmodule(tolua_S,"CallNativeFuncManager");
   tolua_function(tolua_S,"getInstance",tolua_LuaClient_CallNativeFuncManager_getInstance00);
   tolua_function(tolua_S,"AddIndexLayerToMainScene",tolua_LuaClient_CallNativeFuncManager_AddIndexLayerToMainScene00);
   tolua_function(tolua_S,"SetRankLayerVipFlgVisible",tolua_LuaClient_CallNativeFuncManager_SetRankLayerVipFlgVisible00);
   tolua_function(tolua_S,"ShowCombatSettingLayer",tolua_LuaClient_CallNativeFuncManager_ShowCombatSettingLayer00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MessageBox","MessageBox","",NULL);
  tolua_beginmodule(tolua_S,"MessageBox");
   tolua_function(tolua_S,"AdjustStyle",tolua_LuaClient_MessageBox_AdjustStyle00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MonthCardInfo","MonthCardInfo","",tolua_collect_MonthCardInfo);
  #else
  tolua_cclass(tolua_S,"MonthCardInfo","MonthCardInfo","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MonthCardInfo");
   tolua_variable(tolua_S,"card_id",tolua_get_MonthCardInfo_unsigned_card_id,tolua_set_MonthCardInfo_unsigned_card_id);
   tolua_variable(tolua_S,"is_received",tolua_get_MonthCardInfo_is_received,tolua_set_MonthCardInfo_is_received);
   tolua_variable(tolua_S,"day_count",tolua_get_MonthCardInfo_unsigned_day_count,tolua_set_MonthCardInfo_unsigned_day_count);
   tolua_variable(tolua_S,"diamond_amount",tolua_get_MonthCardInfo_unsigned_diamond_amount,tolua_set_MonthCardInfo_unsigned_diamond_amount);
   tolua_variable(tolua_S,"is_alarm",tolua_get_MonthCardInfo_is_alarm,tolua_set_MonthCardInfo_is_alarm);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MonthCardManager","MonthCardManager","",NULL);
  tolua_beginmodule(tolua_S,"MonthCardManager");
   tolua_function(tolua_S,"Get",tolua_LuaClient_MonthCardManager_Get00);
   tolua_function(tolua_S,"hasMonthCard",tolua_LuaClient_MonthCardManager_hasMonthCard00);
   tolua_function(tolua_S,"getOneMonthCard",tolua_LuaClient_MonthCardManager_getOneMonthCard00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SkeletonAnimRcsManager","SkeletonAnimRcsManager","",NULL);
  tolua_beginmodule(tolua_S,"SkeletonAnimRcsManager");
   tolua_function(tolua_S,"getInstance",tolua_LuaClient_SkeletonAnimRcsManager_getInstance00);
   tolua_function(tolua_S,"LoadOneRoleRcsOnly",tolua_LuaClient_SkeletonAnimRcsManager_LoadOneRoleRcsOnly00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"DiamondWarningLayer","DiamondWarningLayer","",NULL);
  tolua_beginmodule(tolua_S,"DiamondWarningLayer");
   tolua_function(tolua_S,"SetTipContentColor",tolua_LuaClient_DiamondWarningLayer_SetTipContentColor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CityListLayer","CityListLayer","CCLayer",NULL);
  tolua_beginmodule(tolua_S,"CityListLayer");
   tolua_function(tolua_S,"openCityList",tolua_LuaClient_CityListLayer_openCityList00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"InstanceInfo","InstanceInfo","",NULL);
  tolua_beginmodule(tolua_S,"InstanceInfo");
   tolua_variable(tolua_S,"id",tolua_get_InstanceInfo_unsigned_id,tolua_set_InstanceInfo_unsigned_id);
   tolua_variable(tolua_S,"star",tolua_get_InstanceInfo_unsigned_star,tolua_set_InstanceInfo_unsigned_star);
   tolua_variable(tolua_S,"can_enter_flag",tolua_get_InstanceInfo_can_enter_flag,tolua_set_InstanceInfo_can_enter_flag);
   tolua_variable(tolua_S,"refresh_times",tolua_get_InstanceInfo_unsigned_refresh_times,tolua_set_InstanceInfo_unsigned_refresh_times);
   tolua_variable(tolua_S,"isgray",tolua_get_InstanceInfo_isgray,tolua_set_InstanceInfo_isgray);
   tolua_variable(tolua_S,"battle_times",tolua_get_InstanceInfo_unsigned_battle_times,tolua_set_InstanceInfo_unsigned_battle_times);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"InstanceListLayer","InstanceListLayer","cocos2d::CCLayer",NULL);
  tolua_beginmodule(tolua_S,"InstanceListLayer");
   tolua_function(tolua_S,"showInstanceInfo",tolua_LuaClient_InstanceListLayer_showInstanceInfo00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"graylightWithCCSprite",tolua_LuaClient_graylightWithCCSprite01);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LuaClient (lua_State* tolua_S) {
 return tolua_LuaClient_open(tolua_S);
};
#endif

