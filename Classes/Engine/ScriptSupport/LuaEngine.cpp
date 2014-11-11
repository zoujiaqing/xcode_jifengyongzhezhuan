/*
** Lua binding: LuaEngine
** Generated automatically by tolua++-1.0.92 on 08/06/13 17:08:13.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_LuaEngine_open (lua_State* tolua_S);

#include "LuaEngine.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_GameResourceManager (lua_State* tolua_S)
{
 GameResourceManager* self = (GameResourceManager*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_GLubyte (lua_State* tolua_S)
{
 GLubyte* self = (GLubyte*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UILabelTTF (lua_State* tolua_S)
{
 TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_ccColor3B (lua_State* tolua_S)
{
 ccColor3B* self = (ccColor3B*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"CCPoint");
 Mtolua_typeid(tolua_S,typeid(CCPoint), "CCPoint");
 tolua_usertype(tolua_S,"IExResourceMgr");
 Mtolua_typeid(tolua_S,typeid(IExResourceMgr), "IExResourceMgr");
 tolua_usertype(tolua_S,"CCTouch");
 Mtolua_typeid(tolua_S,typeid(CCTouch), "CCTouch");
 tolua_usertype(tolua_S,"NetworkOperationDelegate");
 Mtolua_typeid(tolua_S,typeid(NetworkOperationDelegate), "NetworkOperationDelegate");
 tolua_usertype(tolua_S,"CCTextAlignment");
 Mtolua_typeid(tolua_S,typeid(CCTextAlignment), "CCTextAlignment");
 tolua_usertype(tolua_S,"CCSize");
 Mtolua_typeid(tolua_S,typeid(CCSize), "CCSize");
 tolua_usertype(tolua_S,"AssetsManager");
 Mtolua_typeid(tolua_S,typeid(AssetsManager), "AssetsManager");
 tolua_usertype(tolua_S,"CCNode");
 Mtolua_typeid(tolua_S,typeid(CCNode), "CCNode");
 tolua_usertype(tolua_S,"CCDictionary");
 Mtolua_typeid(tolua_S,typeid(CCDictionary), "CCDictionary");
 tolua_usertype(tolua_S,"CCArray");
 Mtolua_typeid(tolua_S,typeid(CCArray), "CCArray");
 tolua_usertype(tolua_S,"TXGUI::UILabelTTF");
 Mtolua_typeid(tolua_S,typeid(TXGUI::UILabelTTF), "TXGUI::UILabelTTF");
 tolua_usertype(tolua_S,"CCLabelTTF");
 Mtolua_typeid(tolua_S,typeid(CCLabelTTF), "CCLabelTTF");
 tolua_usertype(tolua_S,"GameResourceManager");
 Mtolua_typeid(tolua_S,typeid(GameResourceManager), "GameResourceManager");
 tolua_usertype(tolua_S,"CCSprite");
 Mtolua_typeid(tolua_S,typeid(CCSprite), "CCSprite");
 tolua_usertype(tolua_S,"CCVerticalTextAlignment");
 Mtolua_typeid(tolua_S,typeid(CCVerticalTextAlignment), "CCVerticalTextAlignment");
 tolua_usertype(tolua_S,"ccColor3B");
 Mtolua_typeid(tolua_S,typeid(ccColor3B), "ccColor3B");
 tolua_usertype(tolua_S,"CCRenderTexture");
 Mtolua_typeid(tolua_S,typeid(CCRenderTexture), "CCRenderTexture");
 tolua_usertype(tolua_S,"CCObject");
 Mtolua_typeid(tolua_S,typeid(CCObject), "CCObject");
 tolua_usertype(tolua_S,"GLubyte");
 Mtolua_typeid(tolua_S,typeid(GLubyte), "GLubyte");
 tolua_usertype(tolua_S,"CCRGBAProtocol");
 Mtolua_typeid(tolua_S,typeid(CCRGBAProtocol), "CCRGBAProtocol");
}

/* method: sharedManager of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_sharedManager00
static int tolua_LuaEngine_GameResourceManager_sharedManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameResourceManager* tolua_ret = (GameResourceManager*)  GameResourceManager::sharedManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameResourceManager");
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

/* method: purgeInstance of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_purgeInstance00
static int tolua_LuaEngine_GameResourceManager_purgeInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameResourceManager::purgeInstance();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'purgeInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_delete00
static int tolua_LuaEngine_GameResourceManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
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

/* method: updateResource of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_updateResource00
static int tolua_LuaEngine_GameResourceManager_updateResource00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateResource'", NULL);
#endif
  {
   self->updateResource();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateResource'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateResource of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_updateResource01
static int tolua_LuaEngine_GameResourceManager_updateResource01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string version = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateResource'", NULL);
#endif
  {
   self->updateResource(version);
   tolua_pushcppstring(tolua_S,(const char*)version);
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaEngine_GameResourceManager_updateResource00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: writablePathFromRelativePath of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_writablePathFromRelativePath00
static int tolua_LuaEngine_GameResourceManager_writablePathFromRelativePath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writablePathFromRelativePath'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->writablePathFromRelativePath(path);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writablePathFromRelativePath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: storedFullPathFromRelativePath of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath00
static int tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'storedFullPathFromRelativePath'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->storedFullPathFromRelativePath(path);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'storedFullPathFromRelativePath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: storedFullPathFromRelativePath of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath01
static int tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* additionalPath = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* path = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'storedFullPathFromRelativePath'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->storedFullPathFromRelativePath(additionalPath,path);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: arrayFromCache of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_arrayFromCache00
static int tolua_LuaEngine_GameResourceManager_arrayFromCache00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'arrayFromCache'", NULL);
#endif
  {
   CCArray* tolua_ret = (CCArray*)  self->arrayFromCache(path);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCArray");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'arrayFromCache'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dictionaryFromCache of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_dictionaryFromCache00
static int tolua_LuaEngine_GameResourceManager_dictionaryFromCache00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dictionaryFromCache'", NULL);
#endif
  {
   CCDictionary* tolua_ret = (CCDictionary*)  self->dictionaryFromCache(path);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCDictionary");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dictionaryFromCache'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addSpriteFrameWithDictionaryFromCache of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_addSpriteFrameWithDictionaryFromCache00
static int tolua_LuaEngine_GameResourceManager_addSpriteFrameWithDictionaryFromCache00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSpriteFrameWithDictionaryFromCache'", NULL);
#endif
  {
   self->addSpriteFrameWithDictionaryFromCache(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSpriteFrameWithDictionaryFromCache'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeSpriteFrameWithDictionaryFromCache of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_removeSpriteFrameWithDictionaryFromCache00
static int tolua_LuaEngine_GameResourceManager_removeSpriteFrameWithDictionaryFromCache00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSpriteFrameWithDictionaryFromCache'", NULL);
#endif
  {
   self->removeSpriteFrameWithDictionaryFromCache(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSpriteFrameWithDictionaryFromCache'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadXml of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_loadXml00
static int tolua_LuaEngine_GameResourceManager_loadXml00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const char* fileName = ((const char*)  tolua_tostring(tolua_S,2,0));
  bool isEncrypt = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadXml'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->loadXml(fileName,isEncrypt);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadXml'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: uncompress of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_uncompress00
static int tolua_LuaEngine_GameResourceManager_uncompress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string storePath = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'uncompress'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->uncompress(storePath,filename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)storePath);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'uncompress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operationDidFinish of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_operationDidFinish00
static int tolua_LuaEngine_GameResourceManager_operationDidFinish00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"AssetsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  AssetsManager* operation = ((AssetsManager*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operationDidFinish'", NULL);
#endif
  {
   self->operationDidFinish(operation, "");
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'operationDidFinish'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operationDidFail of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_operationDidFail00
static int tolua_LuaEngine_GameResourceManager_operationDidFail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"AssetsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  AssetsManager* operation = ((AssetsManager*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operationDidFail'", NULL);
#endif
  {
   self->operationDidFail(operation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'operationDidFail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStorepath of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_getStorepath00
static int tolua_LuaEngine_GameResourceManager_getStorepath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStorepath'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getStorepath();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStorepath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createStroke of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_GameResourceManager_createStroke00
static int tolua_LuaEngine_GameResourceManager_createStroke00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCLabelTTF",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"ccColor3B",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"GLubyte",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
  CCLabelTTF* label = ((CCLabelTTF*)  tolua_tousertype(tolua_S,2,0));
  float size = ((float)  tolua_tonumber(tolua_S,3,0));
  ccColor3B cor = *((ccColor3B*)  tolua_tousertype(tolua_S,4,0));
  GLubyte opacity = *((GLubyte*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createStroke'", NULL);
#endif
  {
   CCRenderTexture* tolua_ret = (CCRenderTexture*)  self->createStroke(label,size,cor,opacity);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCRenderTexture");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createStroke'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __NetworkOperationDelegate__ of class  GameResourceManager */
#ifndef TOLUA_DISABLE_tolua_get_GameResourceManager___NetworkOperationDelegate__
static int tolua_get_GameResourceManager___NetworkOperationDelegate__(lua_State* tolua_S)
{
  GameResourceManager* self = (GameResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__NetworkOperationDelegate__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<NetworkOperationDelegate*>(self), "NetworkOperationDelegate");
#else
   tolua_pushusertype(tolua_S,(void*)((NetworkOperationDelegate*)self), "NetworkOperationDelegate");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_new00
static int tolua_LuaEngine_TXGUI_UILabelTTF_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TXGUI::UILabelTTF* tolua_ret = (TXGUI::UILabelTTF*)  Mtolua_new((TXGUI::UILabelTTF)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelTTF");
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

/* method: new_local of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_new00_local
static int tolua_LuaEngine_TXGUI_UILabelTTF_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TXGUI::UILabelTTF* tolua_ret = (TXGUI::UILabelTTF*)  Mtolua_new((TXGUI::UILabelTTF)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelTTF");
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

/* method: delete of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_delete00
static int tolua_LuaEngine_TXGUI_UILabelTTF_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
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

/* method: create of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_create00
static int tolua_LuaEngine_TXGUI_UILabelTTF_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCSize",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* content = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* fontName = ((const char*)  tolua_tostring(tolua_S,3,0));
  float fontSize = ((float)  tolua_tonumber(tolua_S,4,0));
  const CCSize* dimensions = ((const CCSize*)  tolua_tousertype(tolua_S,5,0));
  int hAlignment = ((int)  tolua_tonumber(tolua_S,6,0));
  int vAlignment = ((int)  tolua_tonumber(tolua_S,7,0));
  {
   TXGUI::UILabelTTF* tolua_ret = (TXGUI::UILabelTTF*)  TXGUI::UILabelTTF::create(content,fontName,fontSize,*dimensions,(CCTextAlignment)hAlignment,(CCVerticalTextAlignment)vAlignment);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelTTF");
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

/* method: create of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_create01
static int tolua_LuaEngine_TXGUI_UILabelTTF_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const char* content = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* fontName = ((const char*)  tolua_tostring(tolua_S,3,0));
  float fontSize = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   TXGUI::UILabelTTF* tolua_ret = (TXGUI::UILabelTTF*)  TXGUI::UILabelTTF::create(content,fontName,fontSize);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelTTF");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaEngine_TXGUI_UILabelTTF_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_create02
static int tolua_LuaEngine_TXGUI_UILabelTTF_create02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCSize",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"CCTextAlignment",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const char* content = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* fontName = ((const char*)  tolua_tostring(tolua_S,3,0));
  float fontSize = ((float)  tolua_tonumber(tolua_S,4,0));
  const CCSize* dimensions = ((const CCSize*)  tolua_tousertype(tolua_S,5,0));
  CCTextAlignment hAlignment = *((CCTextAlignment*)  tolua_tousertype(tolua_S,6,0));
  {
   TXGUI::UILabelTTF* tolua_ret = (TXGUI::UILabelTTF*)  TXGUI::UILabelTTF::create(content,fontName,fontSize,*dimensions,hAlignment);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelTTF");
  }
 }
 return 1;
tolua_lerror:
 return tolua_LuaEngine_TXGUI_UILabelTTF_create01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithString of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_initWithString00
static int tolua_LuaEngine_TXGUI_UILabelTTF_initWithString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCSize",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"CCTextAlignment",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"CCVerticalTextAlignment",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  const char* content = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* fontName = ((const char*)  tolua_tostring(tolua_S,3,0));
  float fontSize = ((float)  tolua_tonumber(tolua_S,4,0));
  const CCSize* dimensions = ((const CCSize*)  tolua_tousertype(tolua_S,5,0));
  CCTextAlignment hAlignment = *((CCTextAlignment*)  tolua_tousertype(tolua_S,6,0));
  CCVerticalTextAlignment vAlignment = *((CCVerticalTextAlignment*)  tolua_tousertype(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithString'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithString(content,fontName,fontSize,*dimensions,hAlignment,vAlignment);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setString of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_setString00
static int tolua_LuaEngine_TXGUI_UILabelTTF_setString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  const char* label = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setString'", NULL);
#endif
  {
   self->setString(label);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getString of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_getString00
static int tolua_LuaEngine_TXGUI_UILabelTTF_getString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getString();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOpacity of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_setOpacity00
static int tolua_LuaEngine_TXGUI_UILabelTTF_setOpacity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"GLubyte",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  GLubyte opacity = *((GLubyte*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOpacity'", NULL);
#endif
  {
   self->setOpacity(opacity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOpacity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOpacity of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_getOpacity00
static int tolua_LuaEngine_TXGUI_UILabelTTF_getOpacity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOpacity'", NULL);
#endif
  {
   GLubyte tolua_ret = (GLubyte)  self->getOpacity();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((GLubyte)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"GLubyte");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(GLubyte));
     tolua_pushusertype(tolua_S,tolua_obj,"GLubyte");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOpacity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOpacityModifyRGB of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_setOpacityModifyRGB00
static int tolua_LuaEngine_TXGUI_UILabelTTF_setOpacityModifyRGB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  bool bValue = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOpacityModifyRGB'", NULL);
#endif
  {
   self->setOpacityModifyRGB(bValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOpacityModifyRGB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isOpacityModifyRGB of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_isOpacityModifyRGB00
static int tolua_LuaEngine_TXGUI_UILabelTTF_isOpacityModifyRGB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isOpacityModifyRGB'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isOpacityModifyRGB();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isOpacityModifyRGB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setColor of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_setColor00
static int tolua_LuaEngine_TXGUI_UILabelTTF_setColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const ccColor3B",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  const ccColor3B* color = ((const ccColor3B*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setColor'", NULL);
#endif
  {
   self->setColor(*color);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getColor of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_getColor00
static int tolua_LuaEngine_TXGUI_UILabelTTF_getColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getColor'", NULL);
#endif
  {
   ccColor3B tolua_ret = (ccColor3B)  self->getColor();
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
 tolua_error(tolua_S,"#ferror in function 'getColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: visit of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_visit00
static int tolua_LuaEngine_TXGUI_UILabelTTF_visit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setAnchorPoint of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_setAnchorPoint00
static int tolua_LuaEngine_TXGUI_UILabelTTF_setAnchorPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  const CCPoint* anchorPoint = ((const CCPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAnchorPoint'", NULL);
#endif
  {
   self->setAnchorPoint(*anchorPoint);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAnchorPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createStroke of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_createStroke00
static int tolua_LuaEngine_TXGUI_UILabelTTF_createStroke00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCLabelTTF",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"ccColor3B",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  CCLabelTTF* label = ((CCLabelTTF*)  tolua_tousertype(tolua_S,2,0));
  float size = ((float)  tolua_tonumber(tolua_S,3,0));
  ccColor3B cor = *((ccColor3B*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createStroke'", NULL);
#endif
  {
   CCRenderTexture* tolua_ret = (CCRenderTexture*)  self->createStroke(label,size,cor);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCRenderTexture");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createStroke'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: listenToBackground of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_TXGUI_UILabelTTF_listenToBackground00
static int tolua_LuaEngine_TXGUI_UILabelTTF_listenToBackground00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TXGUI::UILabelTTF",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
  CCObject* obj = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'listenToBackground'", NULL);
#endif
  {
   self->listenToBackground(obj);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'listenToBackground'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __CCRGBAProtocol__ of class  TXGUI::UILabelTTF */
#ifndef TOLUA_DISABLE_tolua_get_TXGUI__UILabelTTF___CCRGBAProtocol__
static int tolua_get_TXGUI__UILabelTTF___CCRGBAProtocol__(lua_State* tolua_S)
{
  TXGUI::UILabelTTF* self = (TXGUI::UILabelTTF*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__CCRGBAProtocol__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<CCRGBAProtocol*>(self), "CCRGBAProtocol");
#else
   tolua_pushusertype(tolua_S,(void*)((CCRGBAProtocol*)self), "CCRGBAProtocol");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LuaEngine_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GameResourceManager","GameResourceManager","IExResourceMgr",tolua_collect_GameResourceManager);
  #else
  tolua_cclass(tolua_S,"GameResourceManager","GameResourceManager","IExResourceMgr",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GameResourceManager");
   tolua_function(tolua_S,"sharedManager",tolua_LuaEngine_GameResourceManager_sharedManager00);
   tolua_function(tolua_S,"purgeInstance",tolua_LuaEngine_GameResourceManager_purgeInstance00);
   tolua_function(tolua_S,"delete",tolua_LuaEngine_GameResourceManager_delete00);
   tolua_function(tolua_S,"updateResource",tolua_LuaEngine_GameResourceManager_updateResource00);
   tolua_function(tolua_S,"updateResource",tolua_LuaEngine_GameResourceManager_updateResource01);
   tolua_function(tolua_S,"writablePathFromRelativePath",tolua_LuaEngine_GameResourceManager_writablePathFromRelativePath00);
   tolua_function(tolua_S,"storedFullPathFromRelativePath",tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath00);
   tolua_function(tolua_S,"storedFullPathFromRelativePath",tolua_LuaEngine_GameResourceManager_storedFullPathFromRelativePath01);
   tolua_function(tolua_S,"arrayFromCache",tolua_LuaEngine_GameResourceManager_arrayFromCache00);
   tolua_function(tolua_S,"dictionaryFromCache",tolua_LuaEngine_GameResourceManager_dictionaryFromCache00);
   tolua_function(tolua_S,"addSpriteFrameWithDictionaryFromCache",tolua_LuaEngine_GameResourceManager_addSpriteFrameWithDictionaryFromCache00);
   tolua_function(tolua_S,"removeSpriteFrameWithDictionaryFromCache",tolua_LuaEngine_GameResourceManager_removeSpriteFrameWithDictionaryFromCache00);
   tolua_function(tolua_S,"loadXml",tolua_LuaEngine_GameResourceManager_loadXml00);
   tolua_function(tolua_S,"uncompress",tolua_LuaEngine_GameResourceManager_uncompress00);
   tolua_function(tolua_S,"operationDidFinish",tolua_LuaEngine_GameResourceManager_operationDidFinish00);
   tolua_function(tolua_S,"operationDidFail",tolua_LuaEngine_GameResourceManager_operationDidFail00);
   tolua_function(tolua_S,"getStorepath",tolua_LuaEngine_GameResourceManager_getStorepath00);
   tolua_function(tolua_S,"createStroke",tolua_LuaEngine_GameResourceManager_createStroke00);
   tolua_variable(tolua_S,"__NetworkOperationDelegate__",tolua_get_GameResourceManager___NetworkOperationDelegate__,NULL);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"TXGUI",0);
  tolua_beginmodule(tolua_S,"TXGUI");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"UILabelTTF","TXGUI::UILabelTTF","CCNode",tolua_collect_TXGUI__UILabelTTF);
   #else
   tolua_cclass(tolua_S,"UILabelTTF","TXGUI::UILabelTTF","CCNode",NULL);
   #endif
   tolua_beginmodule(tolua_S,"UILabelTTF");
    tolua_function(tolua_S,"new",tolua_LuaEngine_TXGUI_UILabelTTF_new00);
    tolua_function(tolua_S,"new_local",tolua_LuaEngine_TXGUI_UILabelTTF_new00_local);
    tolua_function(tolua_S,".call",tolua_LuaEngine_TXGUI_UILabelTTF_new00_local);
    tolua_function(tolua_S,"delete",tolua_LuaEngine_TXGUI_UILabelTTF_delete00);
    tolua_function(tolua_S,"create",tolua_LuaEngine_TXGUI_UILabelTTF_create00);
    tolua_function(tolua_S,"create",tolua_LuaEngine_TXGUI_UILabelTTF_create01);
    tolua_function(tolua_S,"create",tolua_LuaEngine_TXGUI_UILabelTTF_create02);
    tolua_function(tolua_S,"initWithString",tolua_LuaEngine_TXGUI_UILabelTTF_initWithString00);
    tolua_function(tolua_S,"setString",tolua_LuaEngine_TXGUI_UILabelTTF_setString00);
    tolua_function(tolua_S,"getString",tolua_LuaEngine_TXGUI_UILabelTTF_getString00);
    tolua_function(tolua_S,"setOpacity",tolua_LuaEngine_TXGUI_UILabelTTF_setOpacity00);
    tolua_function(tolua_S,"getOpacity",tolua_LuaEngine_TXGUI_UILabelTTF_getOpacity00);
    tolua_function(tolua_S,"setOpacityModifyRGB",tolua_LuaEngine_TXGUI_UILabelTTF_setOpacityModifyRGB00);
    tolua_function(tolua_S,"isOpacityModifyRGB",tolua_LuaEngine_TXGUI_UILabelTTF_isOpacityModifyRGB00);
    tolua_function(tolua_S,"setColor",tolua_LuaEngine_TXGUI_UILabelTTF_setColor00);
    tolua_function(tolua_S,"getColor",tolua_LuaEngine_TXGUI_UILabelTTF_getColor00);
    tolua_function(tolua_S,"visit",tolua_LuaEngine_TXGUI_UILabelTTF_visit00);
    tolua_function(tolua_S,"setAnchorPoint",tolua_LuaEngine_TXGUI_UILabelTTF_setAnchorPoint00);
    tolua_function(tolua_S,"createStroke",tolua_LuaEngine_TXGUI_UILabelTTF_createStroke00);
    tolua_function(tolua_S,"listenToBackground",tolua_LuaEngine_TXGUI_UILabelTTF_listenToBackground00);
    tolua_variable(tolua_S,"__CCRGBAProtocol__",tolua_get_TXGUI__UILabelTTF___CCRGBAProtocol__,NULL);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LuaEngine (lua_State* tolua_S) {
 return tolua_LuaEngine_open(tolua_S);
};
#endif

