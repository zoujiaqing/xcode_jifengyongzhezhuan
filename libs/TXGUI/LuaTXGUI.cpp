//
//  LuaTXGUI.h
//  iSeer
//
//  Created by benyang on 12-8-1.
//  Copyright (c) 2012ƒÍ __MyCompanyName__. All rights reserved.
//

#include "LuaTXGUI.h"
#include "AnimSprite.h"
#include "AspriteManager.h"

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

TOLUA_API int tolua_TXGUI_open(lua_State *tolua_S);

#ifdef __cplusplus

//destructor
static int tolua_collect_TXGUI__UIControlBase(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIAnimation(lua_State *tolua_S)
{
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIButton(lua_State *tolua_S)
{
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIButtonToggle(lua_State *tolua_S)
{
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIContainer(lua_State *tolua_S)
{
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UILabel(lua_State *tolua_S)
{
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UILabelAtlas(lua_State *tolua_S)
{
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UILayout(lua_State *tolua_S)
{
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIPicture(lua_State *tolua_S)
{
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIScrollList(lua_State *tolua_S)
{
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIScrollView(lua_State *tolua_S)
{
	TXGUI::UIScrollView *self = (TXGUI::UIScrollView*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIScrollPage(lua_State *tolua_S)
{
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIScrollLayer(lua_State *tolua_S)
{
	TXGUI::UIScrollLayer *self = (TXGUI::UIScrollLayer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__TextInputField(lua_State *tolua_S)
{
	TXGUI::TextInputField *self = (TXGUI::TextInputField*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UITextInputField(lua_State *tolua_S)
{
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_TXGUI__UIEditBox(lua_State *tolua_S)
{
	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

#endif

//register classes
static void tolua_reg_types(lua_State *tolua_S)
{
	tolua_usertype(tolua_S, "TXGUI::NiObject");
	tolua_usertype(tolua_S, "TXGUI::UIControlBase");
	tolua_usertype(tolua_S, "TXGUI::UIAnimation");
	tolua_usertype(tolua_S, "TXGUI::UIButton");
	tolua_usertype(tolua_S, "TXGUI::UIButtonToggle");
	tolua_usertype(tolua_S, "TXGUI::UIContainer");
	tolua_usertype(tolua_S, "TXGUI::UILabel");
	tolua_usertype(tolua_S, "TXGUI::UILabelAtlas");
	tolua_usertype(tolua_S, "TXGUI::UILayout");
	tolua_usertype(tolua_S, "TXGUI::UIPicture");
	tolua_usertype(tolua_S, "TXGUI::UIScrollList");
	tolua_usertype(tolua_S, "TXGUI::UIScrollView");
	tolua_usertype(tolua_S, "TXGUI::UIScrollPage");
	tolua_usertype(tolua_S, "TXGUI::UIScrollLayer");
	tolua_usertype(tolua_S, "TXGUI::TextInputField");
	tolua_usertype(tolua_S, "TXGUI::UITextInputField");
	tolua_usertype(tolua_S, "TXGUI::UIEditBox");
	tolua_usertype(tolua_S, "TXGUI::UIManager");

	/// AspriteManager
	tolua_usertype(tolua_S, "AspriteManager");
}

//UIControlBase
static int tolua_TXGUI_UIControlBase_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parentNode = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*) new TXGUI::UIControlBase(parentNode);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

static int tolua_TXGUI_UIControlBase_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint tolua_ret = (cocos2d::CCPoint) self->getPosition();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIControlBase_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getWorldPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getWorldPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint tolua_ret = (cocos2d::CCPoint) self->getWorldPosition();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIControlBase_setWorldPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setWorldPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setWorldPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getZOrder(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getZOrder'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getZOrder();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIControlBase_setZOrder(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setZOrder'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	float posZ = (float) tolua_tonumber(tolua_S,2,0);
	self->setZOrder(posZ);

	return 0;
}

static int tolua_TXGUI_UIControlBase_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIControlBase_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIControlBase_setScale00(tolua_S);
	}

	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale, yScale);

	return 0;
}

static int tolua_TXGUI_UIControlBase_UpdateWorldPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::UpdateWorldPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	self->UpdateWorldPosition();

	return 0;
}

static int tolua_TXGUI_UIControlBase_LocalPt2World(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::LocalPt2World'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCPoint tolua_ret = (cocos2d::CCPoint) self->LocalPt2World(*point);
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIControlBase_getLocal2WorldTransform(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getLocal2WorldTransform'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCAffineTransform tolua_ret = self->getLocal2WorldTransform();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCAffineTransform)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCAffineTransform");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCAffineTransform));
		tolua_pushusertype(tolua_S,tolua_obj,"CCAffineTransform");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIControlBase_setAudioEffect(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setAudioEffect'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	int iAudio = (int) tolua_tonumber(tolua_S,2,0);
	self->setAudioEffect(iAudio);

	return 0;
}

static int tolua_TXGUI_UIControlBase_PlayAudio(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::PlayAudio'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	self->PlayAudio();

	return 0;
}

static int tolua_TXGUI_UIControlBase_StopAudio(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::StopAudio'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	self->StopAudio();

	return 0;
}

static int tolua_TXGUI_UIControlBase_PauseAudio(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::PauseAudio'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	self->PauseAudio();

	return 0;
}

static int tolua_TXGUI_UIControlBase_getTag(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getTag'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	int tolua_ret = (int) self->getTag();
	tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIControlBase_setTag(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setTag'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	int tag = (int) tolua_tonumber(tolua_S,2,0);
	self->setTag(tag);

	return 0;
}

static int tolua_TXGUI_UIControlBase_setName(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setName'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	std::string name = tolua_tocppstring(tolua_S,2,0);
	self->setName(name);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getName(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getName'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	std::string tolua_ret = self->getName();
	tolua_pushcppstring(tolua_S,tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIControlBase_getCurrentNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getCurrentNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *tolua_ret = self->getCurrentNode();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");

	return 1;
}

static int tolua_TXGUI_UIControlBase_setParent(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setParent'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,2,0);
	self->setParent(parent);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getParent(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getParent'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*) self->getParent();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

static int tolua_TXGUI_UIControlBase_AddChild(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::AddChild'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *child = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,2,0);
	self->AddChild(child);

	return 1;
}

static int tolua_TXGUI_UIControlBase_RemoveChild00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::RemoveChild'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *child = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,2,0);
	self->RemoveChild(child);

	return 0;
}

static int tolua_TXGUI_UIControlBase_RemoveChild01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIControlBase_RemoveChild00(tolua_S);
	}
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *child = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,2,0);
	bool clean =  tolua_toboolean(tolua_S,3,0) != 0;
	self->RemoveChild(child, clean);

	return 0;
}

static int tolua_TXGUI_UIControlBase_RemoveAllChildrenWithCleanup(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::RemoveAllChildrenWithCleanup'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	self->RemoveAllChildrenWithCleanup();

	return 0;
}

static int tolua_TXGUI_UIControlBase_RemoveChildTreeByName(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::RemoveChildTreeByName'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	self->RemoveChildTreeByName(name);

	return 0;
}

static int tolua_TXGUI_UIControlBase_FindChildObjectByName00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::FindChildObjectByName'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*) self->FindChildObjectByName(name);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

static int tolua_TXGUI_UIControlBase_BindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::BindNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	self->BindNode(parent);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getBindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getBindNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *tolua_ret = (cocos2d::CCNode*) self->getBindNode();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");

	return 1;
}

static int tolua_TXGUI_UIControlBase_setLuaScriptFile(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setLuaScriptFile'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	std::string strFile = tolua_tocppstring(tolua_S,2,0);
	self->setLuaScriptFile(strFile);

	return 0;
}

static int tolua_TXGUI_UIControlBase_getLuaScriptFile(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::getLuaScriptFile'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	std::string tolua_ret = self->getLuaScriptFile();
	tolua_pushcppstring(tolua_S,tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIControlBase_executeLuaScript(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::executeLuaScript'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	self->executeLuaScript();

	return 0;
}

static int tolua_TXGUI_UIControlBase_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIControlBase_setVisible(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2, 0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::setVisible'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	bool bVisible = (bool) (tolua_toboolean(tolua_S, 2, 0) != 0);
	
	if (self)
	{
		self->setVisible(bVisible);
	}

	return 1;
}


static int tolua_TXGUI_UIControlBase_isVisible(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::isVisible'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
	bool tolua_ret = false;

	if (self->getCurrentNode())
	{
		tolua_ret = self->getCurrentNode()->isVisible();
	}

	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}


static int tolua_TXGUI_UIControlBase___NiObject__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::NiObject *tolua_ret = static_cast<TXGUI::NiObject*>(self);
#else
	TXGUI::NiObject *tolua_ret = (TXGUI::NiObject*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::NiObject");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIAnimation__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIAnimation *tolua_ret = static_cast<TXGUI::UIAnimation*>(self);
#else
	TXGUI::UIAnimation *tolua_ret = (TXGUI::UIAnimation*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIAnimation");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIButton__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIButton *tolua_ret = static_cast<TXGUI::UIButton*>(self);
#else
	TXGUI::UIButton *tolua_ret = (TXGUI::UIButton*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIButtonToggle__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIButtonToggle *tolua_ret = static_cast<TXGUI::UIButtonToggle*>(self);
#else
	TXGUI::UIButtonToggle *tolua_ret = (TXGUI::UIButtonToggle*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButtonToggle");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIContainer__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIContainer *tolua_ret = static_cast<TXGUI::UIContainer*>(self);
#else
	TXGUI::UIContainer *tolua_ret = (TXGUI::UIContainer*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIContainer");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UILabel__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UILabel *tolua_ret = static_cast<TXGUI::UILabel*>(self);
#else
	TXGUI::UILabel *tolua_ret = (TXGUI::UILabel*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabel");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UILabelAtlas__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UILabelAtlas *tolua_ret = static_cast<TXGUI::UILabelAtlas*>(self);
#else
	TXGUI::UILabelAtlas *tolua_ret = (TXGUI::UILabelAtlas*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelAtlas");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UILayout__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UILayout *tolua_ret = static_cast<TXGUI::UILayout*>(self);
#else
	TXGUI::UILayout *tolua_ret = (TXGUI::UILayout*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIPicture__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIPicture *tolua_ret = static_cast<TXGUI::UIPicture*>(self);
#else
	TXGUI::UIPicture *tolua_ret = (TXGUI::UIPicture*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIPicture");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIScrollList__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIScrollList *tolua_ret = static_cast<TXGUI::UIScrollList*>(self);
#else
	TXGUI::UIScrollList *tolua_ret = (TXGUI::UIScrollList*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollList");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIScrollPage__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIScrollPage *tolua_ret = static_cast<TXGUI::UIScrollPage*>(self);
#else
	TXGUI::UIScrollPage *tolua_ret = (TXGUI::UIScrollPage*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollPage");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UITextInputField__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UITextInputField *tolua_ret = static_cast<TXGUI::UITextInputField*>(self);
#else
	TXGUI::UITextInputField *tolua_ret = (TXGUI::UITextInputField*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UITextInputField");

	return 1;
}

static int tolua_TXGUI_UIControlBase___UIEditBox__(lua_State *tolua_S)
{
	TXGUI::UIControlBase *self = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIEditBox *tolua_ret = static_cast<TXGUI::UIEditBox*>(self);
#else
	TXGUI::UIEditBox *tolua_ret = (TXGUI::UIEditBox*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIEditBox");

	return 1;
}



//UIAnimation
static int tolua_TXGUI_UIAnimation_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIAnimation *tolua_ret = (TXGUI::UIAnimation*) new TXGUI::UIAnimation(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIAnimation");

	return 1;
}

static int tolua_TXGUI_UIAnimation_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"AnimSprite",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,7,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		return tolua_TXGUI_UIAnimation_new00(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	AnimSprite *anim = (AnimSprite*) tolua_tousertype(tolua_S,6,0);
	float initScale = (float) tolua_tonumber(tolua_S,7,0);

	TXGUI::UIAnimation *tolua_ret = (TXGUI::UIAnimation*) new TXGUI::UIAnimation(name, tag, z, parent, anim, initScale);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIAnimation");

	return 1;
}

static int tolua_TXGUI_UIAnimation_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIAnimation_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIAnimation_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIAnimation_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIAnimation_setScale00(tolua_S);
	}
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale, yScale);

	return 0;
}

static int tolua_TXGUI_UIAnimation_setInitScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::setInitScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setInitScale(scale);

	return 0;
}

static int tolua_TXGUI_UIAnimation_getInitScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::getInitScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getInitScale();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIAnimation_setAnimSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"AnimSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::setAnimSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	AnimSprite *pAnimSprite = (AnimSprite*) tolua_tousertype(tolua_S,2,0);
	self->setAnimSprite(pAnimSprite);

	return 0;
}

static int tolua_TXGUI_UIAnimation_getAnimSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::getAnimSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	AnimSprite *tolua_ret = self->getAnimSprite();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"AnimSprite");

	return 1;
}

static int tolua_TXGUI_UIAnimation_BindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::BindNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	self->BindNode(parent);

	return 0;
}

static int tolua_TXGUI_UIAnimation_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIAnimation",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIAnimation::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIAnimation___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIAnimation *self = (TXGUI::UIAnimation*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

// UIButton
static int tolua_TXGUI_UIButton_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIButton *tolua_ret = (TXGUI::UIButton*) new TXGUI::UIButton(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

static int tolua_TXGUI_UIButton_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCMenuItemSprite",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,7,"CCMenuItemLabel",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButton_new00(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::CCMenuItemSprite *sprite = (cocos2d::CCMenuItemSprite*) tolua_tousertype(tolua_S,6,0);
	cocos2d::CCMenuItemLabel *label = (cocos2d::CCMenuItemLabel*) tolua_tousertype(tolua_S,7,0);

	TXGUI::UIButton *tolua_ret = (TXGUI::UIButton*) new TXGUI::UIButton(name, tag, z, parent, sprite, label,-128);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

static int tolua_TXGUI_UIButton_new02(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCMenuItemSprite",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,7,"CCMenuItemLabel",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,8,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,9,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,10,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButton_new01(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::CCMenuItemSprite *sprite = (cocos2d::CCMenuItemSprite*) tolua_tousertype(tolua_S,6,0);
	cocos2d::CCMenuItemLabel *label = (cocos2d::CCMenuItemLabel*) tolua_tousertype(tolua_S,7,0);
	std::string func = (std::string) tolua_tocppstring(tolua_S,9,0);
	int priorty = (int) tolua_tonumber(tolua_S,8,0);
	TXGUI::UIButton *tolua_ret = (TXGUI::UIButton*) new TXGUI::UIButton(name, tag, z, parent, sprite, label,priorty, func);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

static int tolua_TXGUI_UIButton_new03(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"MenuItemExpand",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,7,"CCMenuItemLabel",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,8,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,9,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,10,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButton_new01(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	MenuItemExpand *sprite = (MenuItemExpand*) tolua_tousertype(tolua_S,6,0);
	cocos2d::CCMenuItemLabel *label = (cocos2d::CCMenuItemLabel*) tolua_tousertype(tolua_S,7,0);
	std::string func = (std::string) tolua_tocppstring(tolua_S,9,0);
	int priorty = (int) tolua_tonumber(tolua_S,8,0);
	TXGUI::UIButton *tolua_ret = (TXGUI::UIButton*) new TXGUI::UIButton(name, tag, z, parent, sprite, label,priorty, func);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

static int tolua_TXGUI_UIButton_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIButton_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIButton_setMenuInitPriority(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
       !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,3,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setMenuInitPriority'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	int priority = (int) tolua_tonumber(tolua_S,2,0);
	self->setMenuInitPriority(priority);
	return 0;
}
static int tolua_TXGUI_UIButton_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIButton_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButton_setScale00(tolua_S);
	}

	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale, yScale);

	return 0;
}

static int tolua_TXGUI_UIButton_getMenuItemSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::getMenuItemSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCMenuItemSprite *tolua_ret = self->getMenuItemSprite();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemSprite");

	return 1;
}

static int tolua_TXGUI_UIButton_getMenuItemSpriteExpand(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::tolua_TXGUI_UIButton_getMenuItemSpriteExpand'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	MenuItemExpand *tolua_ret = self->getMenuItemSpriteExpand();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"MenuItemExpand");

	return 1;
}

static int tolua_TXGUI_UIButton_setMenuItemSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCMenuItemSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setMenuItemSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCMenuItemSprite *pSprite = (cocos2d::CCMenuItemSprite*) tolua_tousertype(tolua_S,2,0);
	self->setMenuItemSprite(pSprite);

	return 0;
}

static int tolua_TXGUI_UIButton_getMenuItemLabel(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::getMenuItemLabel'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCMenuItemLabel *tolua_ret = self->getMenuItemLabel();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemLabel");

	return 1;
}

static int tolua_TXGUI_UIButton_setMenuItemLabel(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCMenuItemLabel",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setMenuItemLabel'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCMenuItemLabel *pLabel = (cocos2d::CCMenuItemLabel*) tolua_tousertype(tolua_S,2,0);
	self->setMenuItemLabel(pLabel);

	return 0;
}

static int tolua_TXGUI_UIButton_setClickedFunc(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setClickedFunc'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	std::string func = (std::string) tolua_tocppstring(tolua_S,2,0);
	self->setClickedFunc(func);

	return 0;
}

static int tolua_TXGUI_UIButton_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}


static int tolua_TXGUI_UIButton_setEnabled(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,1,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setEnabled'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	bool isEnable = ((bool)  tolua_toboolean(tolua_S,2,true));
	self->setEnabled(isEnable);

	return 0;
}

static int tolua_TXGUI_UIButton_setMenuItemDispalyState(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setMenuItemDispalyState'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	int state = ((int)  tolua_tonumber(tolua_S,2,0));
	self->setMenuItemDispalyState(state);

	return 0;
}

static int tolua_TXGUI_UIButton_isEnabled(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::isEnabled'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	bool tolua_ret = self->isEnabled();
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIButton_getAnimationNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::getAnimationNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	CCNode* tolua_ret = (CCNode*) self->getAnimationNode();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");

	return 1;
}

static int tolua_TXGUI_UIButton_setIconSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setIconSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	CCSprite* pSprite = (CCSprite*) tolua_tousertype(tolua_S,2,0);
	
	self->setIconSprite(pSprite);

	return 0;
}

static int tolua_TXGUI_UIButton_setDragMove(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,1,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setDragMove'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	bool isDrag = ((bool)  tolua_toboolean(tolua_S,2,true));
	self->setDragMove(isDrag);

	return 0;
}

static int tolua_TXGUI_UIButton_setFrameSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setFrameSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	CCSprite* pSprite = (CCSprite*) tolua_tousertype(tolua_S,2,0);
	self->setFrameSprite(pSprite);

	return 0;
}

static int tolua_TXGUI_UIButton_registerTapHandler(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		TXGUI::UIButton* self = (TXGUI::UIButton*)  tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerTapHandler'", NULL);
#endif
		{
			self->registerTapHandler(funcID);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerTapHandler'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIButton_unregisterTapHandler(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		TXGUI::UIButton* self = (TXGUI::UIButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterTapHandler'", NULL);
#endif
		{
			self->unregisterTapHandler();
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unregisterTapHandler'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIButton_registerTapCancellHandler(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		TXGUI::UIButton* self = (TXGUI::UIButton*)  tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerTapCancellHandler'", NULL);
#endif
		{
			self->registerTapCancellHandler(funcID);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerTapCancellHandler'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIButton_unregisterTapCancellHandler(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		TXGUI::UIButton* self = (TXGUI::UIButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterTapCancellHandler'", NULL);
#endif
		{
			self->unregisterTapCancellHandler();
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unregisterTapCancellHandler'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIButton_getMenuNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::getMenuNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	UIMenu* tolua_ret = self->getMenuNode();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIMenu");
	return 1;
}

static int tolua_TXGUI_UIButton_setTextLabel(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,4,"ccColor3B",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCPoint",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,7,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::setTextLabel'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	const char* pram1 = (const char*) tolua_tostring(tolua_S,2,0);
	float pram2 = (float)tolua_tonumber(tolua_S,3,0);
	ccColor3B* pram3 = (ccColor3B*) tolua_tousertype(tolua_S,4,0);
	const char* pram4 = (const char*) tolua_tostring(tolua_S,5,0);
	cocos2d::CCPoint* pram5 = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,6,0);
	cocos2d::CCPoint* pram6 = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,7,0);
	self->setTextLabel(pram1,pram2,*pram3,pram4,*pram5,*pram6);

	return 0;
}

static int tolua_TXGUI_UIButton_getButtonSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::getButtonSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	CCSize tolua_ret = self->getButtonSize();
		{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCSize)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCSize));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}



static int tolua_TXGUI_UIButton_showTipIcon(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::showTipIcon'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	self->showTipIcon();

	return 0;
}

static int tolua_TXGUI_UIButton_hideTipIcon(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::showTipIcon'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
	self->hideTipIcon();

	return 0;
}

static int tolua_TXGUI_UIButton_ShowRotateEffect(lua_State* tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
    tolua_Error tolua_err;
    if(
        !tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
    {
        tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::ShowRotateEffect'",&tolua_err);
        return 0;
    }
#endif
    TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
    self->ShowRotateEffect();

    return 0;
}

static int tolua_TXGUI_UIButton_HideRotateEffect(lua_State* tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
    tolua_Error tolua_err;
    if(
        !tolua_isusertype(tolua_S,1,"TXGUI::UIButton",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
    {
        tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButton::HideRotateEffect'",&tolua_err);
        return 0;
    }
#endif
    TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
    self->HideRotateEffect();

    return 0;
}

static int tolua_TXGUI_UIButton___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIButton *self = (TXGUI::UIButton*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UIButtonToggle
static int tolua_TXGUI_UIButtonToggle_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIButtonToggle *tolua_ret = (TXGUI::UIButtonToggle*) new TXGUI::UIButtonToggle(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButtonToggle");

	return 1;
}

static int tolua_TXGUI_UIButtonToggle_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCMenuItemToggle",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButtonToggle_new00(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::CCMenuItemToggle *toggle = (cocos2d::CCMenuItemToggle*) tolua_tousertype(tolua_S,6,0);

	TXGUI::UIButtonToggle *tolua_ret = (TXGUI::UIButtonToggle*) new TXGUI::UIButtonToggle(name, tag, z, parent, toggle);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButtonToggle");

	return 1;
}

static int tolua_TXGUI_UIButtonToggle_new02(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCMenuItemToggle",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButtonToggle_new01(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::CCMenuItemToggle *toggle = (cocos2d::CCMenuItemToggle*) tolua_tousertype(tolua_S,6,0);
	std::string func = (std::string) tolua_tocppstring(tolua_S,7,0);

	TXGUI::UIButtonToggle *tolua_ret = 
		(TXGUI::UIButtonToggle*) new TXGUI::UIButtonToggle(name, tag, z, parent, toggle, func);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButtonToggle");

	return 1;
}

static int tolua_TXGUI_UIButtonToggle_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIButtonToggle_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIButtonToggle_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIButtonToggle_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIButtonToggle_setScale00(tolua_S);
	}

	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UIButtonToggle_getMenuItemToggle(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::getMenuItemToggle'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCMenuItemToggle *tolua_ret = (cocos2d::CCMenuItemToggle*) self->getCCXToggle();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemToggle");

	return 1;
}

static int tolua_TXGUI_UIButtonToggle_setMenuItemToggle(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCMenuItemToggle",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::setMenuItemToggle'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCMenuItemToggle *pToggle = (cocos2d::CCMenuItemToggle*) tolua_tousertype(tolua_S,2,0);
	self->setMenuItemToggle(pToggle);

	return 0;
}

static int tolua_TXGUI_UIButtonToggle_setClickedFunc(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::setClickedFunc'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	std::string func = (std::string) tolua_tocppstring(tolua_S,2,0);
	self->setClickedFunc(func);

	return 0;
}

static int tolua_TXGUI_UIButtonToggle_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIButtonToggle_selected(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::selected'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	self->selected();
	return 0;
}

static int tolua_TXGUI_UIButtonToggle_unselected(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIButtonToggle",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIButtonToggle::selected'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
	self->unselected();
	return 0;
}

static int tolua_TXGUI_UIButtonToggle___UIButton__(lua_State *tolua_S)
{
	TXGUI::UIButtonToggle *self = (TXGUI::UIButtonToggle*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIButton *tolua_ret = static_cast<TXGUI::UIButton*>(self);
#else
	TXGUI::UIButton *tolua_ret = (TXGUI::UIButton*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

//UIContainer
static int tolua_TXGUI_UIContainer_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIContainer *tolua_ret = (TXGUI::UIContainer*) new TXGUI::UIContainer(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIContainer");

	return 1;
}

static int tolua_TXGUI_UIContainer_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIContainer_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIContainer_UpdateWorldPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::UpdateWorldPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	self->UpdateWorldPosition();

	return 0;
}

static int tolua_TXGUI_UIContainer_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIContainer_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIContainer_setScale00(tolua_S);
	}

	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale, yScale);
		
	return 0;
}

static int tolua_TXGUI_UIContainer_setRefMode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setRefMode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::ReferenceMode mode = (TXGUI::ReferenceMode) (int) tolua_tonumber(tolua_S,2,0);
	self->setRefMode(mode);

	return 0;
}

static int tolua_TXGUI_UIContainer_getRefMode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getRefMode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::ReferenceMode tolua_ret = self->getRefMode();
	tolua_pushnumber(tolua_S,(int)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer_setRefPointMode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setRefPointMode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::ReferencePoint mode = (TXGUI::ReferencePoint) (int) tolua_tonumber(tolua_S,2,0);
	self->setRefPointMode(mode);

	return 0;
}

static int tolua_TXGUI_UIContainer_getRefPointMode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getRefPointMode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::ReferencePoint tolua_ret = self->getRefPointMode();
	tolua_pushnumber(tolua_S,(int)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer_setWidthScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setWidthScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float scale = (float)tolua_tonumber(tolua_S,2,0);
	self->setWidthScale(scale);

	return 0;
}

static int tolua_TXGUI_UIContainer_getWidthScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getWidthScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getWidthScale();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer_setWidthOffset(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setWidthOffset'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float value = (float) tolua_tonumber(tolua_S,2,0);
	self->setWidthOffset(value);

	return 0;
}

static int tolua_TXGUI_UIContainer_getWidthOffset(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getWidthOffset'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getWidthOffset();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer_setHeightScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setHeightScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float scale = (float)tolua_tonumber(tolua_S,2,0);
	self->setHeightScale(scale);

	return 0;
}

static int tolua_TXGUI_UIContainer_getHeightScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getHeightScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getHeightScale();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer_setHeightOffset(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setHeightOffset'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float value = (float) tolua_tonumber(tolua_S,2,0);
	self->setHeightOffset(value);

	return 0;
}

static int tolua_TXGUI_UIContainer_getHeightOffset(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getHeightOffset'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getHeightOffset();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer_setSize00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize *size = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	self->setSize(*size);

	return 0;
}

static int tolua_TXGUI_UIContainer_setSize01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIContainer_setSize00(tolua_S);
	}

	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	int width = (int) tolua_tonumber(tolua_S,2,0);
	int height = (int) tolua_tonumber(tolua_S,3,0);
	self->setSize(width, height);

	return 0;
}

static int tolua_TXGUI_UIContainer_getSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize tolua_ret = self->getSize();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCSize)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCSize));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIContainer_setRefObjName(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setRefObjName'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	self->setRefObjName(name);

	return 0;
}

static int tolua_TXGUI_UIContainer_getRefObjName(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getRefObjName'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	std::string name = self->getRefObjName();
	tolua_pushcppstring(tolua_S,name);

	return 1;
}

static int tolua_TXGUI_UIContainer_setRefObj(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::setRefObj'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIContainer *refObj = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,2,0);
	self->setRefObj(refObj);

	return 0;
}

static int tolua_TXGUI_UIContainer_getRefObj(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::getRefObj'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIContainer *tolua_ret = self->getRefObj();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIContainer");
	
	return 1;
}

static int tolua_TXGUI_UIContainer_CalWorldPos(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::CalWorldPos'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	self->CalWorldPos();

	return 0;
}

static int tolua_TXGUI_UIContainer_CalLocalPos(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::CalLocalPos'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	self->CalLocalPos();

	return 0;
}

static int tolua_TXGUI_UIContainer_containerCenterPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::containerCenterPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize *targetWinSize = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCPoint tolua_ret = self->containerCenterPosition(*targetWinSize);
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIContainer_UpdateChildrenPos(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::UpdateChildrenPos'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	self->UpdateChildrenPos();

	return 0;
}

static int tolua_TXGUI_UIContainer_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIContainer::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIContainer___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIContainer *self = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UILabel
static int tolua_TXGUI_UILabel_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::new'",&tolua_err);
		return 0;
	}
#endif

	std::string label = tolua_tocppstring(tolua_S,2,0);
	std::string fontName = tolua_tocppstring(tolua_S,3,0);
	float fontSize = (float) tolua_tonumber(tolua_S,4,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,5,0);
	TXGUI::UILabel *tolua_ret = (TXGUI::UILabel*) new TXGUI::UILabel(label.c_str(), fontName.c_str(), fontSize, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabel");

	return 1;
}

static int tolua_TXGUI_UILabel_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLabelTTF",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabel_new00(tolua_S);
	}

	TXGUI::UILabelTTF *pLabel = (TXGUI::UILabelTTF*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	TXGUI::UILabel *tolua_ret = (TXGUI::UILabel*) new TXGUI::UILabel(pLabel, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabel");

	return 1;
}

static int tolua_TXGUI_UILabel_new02(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCLabelTTF",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabel_new01(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	TXGUI::UILabelTTF *label = (TXGUI::UILabelTTF*) tolua_tousertype(tolua_S,6,0);

	TXGUI::UILabel *tolua_ret = (TXGUI::UILabel*) new TXGUI::UILabel(name, tag, z, parent, label);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabel");

	return 1;
}

static int tolua_TXGUI_UILabel_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UILabel_setString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::setString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	std::string label = lua_tostring(tolua_S,2);
	self->setString(label.c_str());

	return 0;
}

static int tolua_TXGUI_UILabel_getString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::getString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	const char *label = self->getString();
	tolua_pushstring(tolua_S,label);

	return 1;
}

static int tolua_TXGUI_UILabel_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UILabel_setColor(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"ccColor3B",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::setColor'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	cocos2d::ccColor3B *color = (cocos2d::ccColor3B*) tolua_tousertype(tolua_S,2,0);
	self->setColor(*color);
	
	return 0;
}

static int tolua_TXGUI_UILabel_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UILabel_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabel_setScale00(tolua_S);
	}
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UILabel_getLabelTTF(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::getLabelTTF'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	if(self->getCurrentNode() != NULL)
	{
#ifdef __cplusplus
		cocos2d::CCLabelTTF *tolua_ret = static_cast<cocos2d::CCLabelTTF*>(self->getCurrentNode());
#else
		cocos2d::CCLabelTTF *tolua_ret = (cocos2d::CCLabelTTF*)(self->getCurrentNode());
#endif
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLabelTTF");
	}
	else
	{
		tolua_pushusertype(tolua_S,NULL,"CCLabelTTF");
	}

	return 1;
}

static int tolua_TXGUI_UILabel_setLabelTTF(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLabelTTF",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::setLabelTTF'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UILabelTTF *pLabel = (TXGUI::UILabelTTF*) tolua_tousertype(tolua_S,2,0);
	self->setLabelTTF(pLabel);

	return 0;
}

static int tolua_TXGUI_UILabel_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabel",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabel::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UILabel___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UILabel *self = (TXGUI::UILabel*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UILabelAtlas
static int tolua_TXGUI_UILabelAtlas_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,5,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,6,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,7,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::new'",&tolua_err);
		return 0;
	}
#endif

	std::string label = tolua_tocppstring(tolua_S,2,0);
	std::string fontFile = tolua_tocppstring(tolua_S,3,0);
	int itemWidth = (int) tolua_tonumber(tolua_S,4,0);
	int itemHeight = (int) tolua_tonumber(tolua_S,5,0);
	unsigned int startChar = (unsigned int) tolua_tonumber(tolua_S,6,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,7,0);
	TXGUI::UILabelAtlas *tolua_ret = (TXGUI::UILabelAtlas*) new TXGUI::UILabelAtlas(
		label.c_str(),
		fontFile.c_str(),
		itemWidth,
		itemHeight,
		startChar,
		parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelAtlas");

	return 1;
}

static int tolua_TXGUI_UILabelAtlas_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLabelAtlas",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabelAtlas_new00(tolua_S);
	}

	cocos2d::CCLabelAtlas *pLabel = (cocos2d::CCLabelAtlas*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	TXGUI::UILabelAtlas *tolua_ret = (TXGUI::UILabelAtlas*) new TXGUI::UILabelAtlas(pLabel, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelAtlas");

	return 1;
}

static int tolua_TXGUI_UILabelAtlas_new02(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCLabelAtlas",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabelAtlas_new01(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::CCLabelAtlas *label = (cocos2d::CCLabelAtlas*) tolua_tousertype(tolua_S,6,0);

	TXGUI::UILabelAtlas *tolua_ret = (TXGUI::UILabelAtlas*) new TXGUI::UILabelAtlas(name, tag, z, parent, label);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILabelAtlas");

	return 1;
}

static int tolua_TXGUI_UILabelAtlas_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UILabelAtlas_setString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::setString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	std::string label = tolua_tocppstring(tolua_S,2,0);
	self->setString(label.c_str());

	return 0;
}

static int tolua_TXGUI_UILabelAtlas_getString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::getString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	const char *label = self->getString();
	tolua_pushstring(tolua_S,label);

	return 1;
}

static int tolua_TXGUI_UILabelAtlas_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UILabelAtlas_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UILabelAtlas_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabelAtlas_setScale00(tolua_S);
	}
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UILabelAtlas_getLabelAtlas(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::getLabelAtlas'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	if(self->getCurrentNode() != NULL)
	{
#ifdef __cplusplus
		cocos2d::CCLabelAtlas *tolua_ret = static_cast<cocos2d::CCLabelAtlas*>(self->getCurrentNode());
#else
		cocos2d::CCLabelAtlas *tolua_ret = (cocos2d::CCLabelAtlas*)(self->getCurrentNode());
#endif
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLabelAtlas");
	}
	else
	{
		tolua_pushusertype(tolua_S,NULL,"CCLabelAtlas");
	}

	return 1;
}

static int tolua_TXGUI_UILabelAtlas_setLabelAtlas(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLabelAtlas",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::setLabelAtlas'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCLabelAtlas *pLabel = (cocos2d::CCLabelAtlas*) tolua_tousertype(tolua_S,2,0);
	self->setLabelAtlas(pLabel);

	return 0;
}

static int tolua_TXGUI_UILabelAtlas_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILabelAtlas",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILabelAtlas::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UILabelAtlas___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UILabelAtlas *self = (TXGUI::UILabelAtlas*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UILayout
static int tolua_TXGUI_UILayout_new00(lua_State* tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UILayout *tolua_ret = new TXGUI::UILayout(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");

	return 1;
}

static int tolua_TXGUI_UILayout_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
	{
		return tolua_TXGUI_UILayout_new00(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);

	TXGUI::UILayout *tolua_ret = (TXGUI::UILayout*) new TXGUI::UILayout(name, tag, z, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");

	return 1;
}

static int tolua_TXGUI_UILayout_new02(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"ccColor4B",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_UILayout_new01(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::ccColor4B *color = (cocos2d::ccColor4B*) tolua_tousertype(tolua_S,6,0);

	TXGUI::UILayout *tolua_ret = (TXGUI::UILayout*) new TXGUI::UILayout(name, tag, z, parent, *color);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");

	return 1;
}

static int tolua_TXGUI_UILayout_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UILayout_setAudioEffect(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::setAudioEffect'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	int iAudio = (int) tolua_tonumber(tolua_S,2,0);
	self->setAudioEffect(iAudio);

	return 0;
}

static int tolua_TXGUI_UILayout_getBindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::getBindNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *tolua_ret = self->getBindNode();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");

	return 1;
}

static int tolua_TXGUI_UILayout_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UILayout_RegisterMenuHandler(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"SEL_MenuHandler",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,4,"SelectorProtocol",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::RegisterMenuHandler'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	std::string name = tolua_tocppstring(tolua_S,2,0);
	cocos2d::SEL_MenuHandler *pfn = (cocos2d::SEL_MenuHandler*) tolua_tousertype(tolua_S,3,0);
	cocos2d::CCObject *pSelect = (cocos2d::CCObject*) tolua_tousertype(tolua_S,4,0);
	self->RegisterMenuHandler(name, *pfn, pSelect);

	return 0;
}

static int tolua_TXGUI_UILayout_UnregisterMenuHandler(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::UnregisterMenuHandler'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	std::string name = tolua_tocppstring(tolua_S,2,0);
	self->UnregisterMenuHandler(name);

	return 0;
}

static int tolua_TXGUI_UILayout_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UILayout_setVisible(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::setVisible'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	bool bVisible  = (bool ) (tolua_toboolean(tolua_S,2,0) != 0);
	self->setVisible(bVisible);
	return 1;
}

static int tolua_TXGUI_UILayout_setVisibleInStyle(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::setVisibleInStyle'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	bool bVisible  = (bool ) (tolua_toboolean(tolua_S,2,0) != 0);
	self->setVisibleInStyle(bVisible);
	return 1;
}

static int tolua_TXGUI_UILayout_EnableModeless(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::EnableModeless'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
	bool bModelView  = (bool ) (tolua_toboolean(tolua_S,2,0) != 0);
	self->EnableModeless(bModelView);
	return 1;
}

static int tolua_TXGUI_UILayout_SetNewPriority(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
    tolua_Error tolua_err;
    if(
        !tolua_isusertype(tolua_S,1,"TXGUI::UILayout",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
    {
        tolua_error(tolua_S,"#ferror in function 'TXGUI::UILayout::SetNewPriority'",&tolua_err);
        return 0;
    }
#endif

    TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
    int priorityView  = (int ) (tolua_tonumber(tolua_S,2,0));
    self->SetNewPriority(priorityView);
    return 1;
}

static int tolua_TXGUI_UILayout___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UILayout *self = (TXGUI::UILayout*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UIPicture
static int tolua_TXGUI_UIPicture_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCSprite *pSprite = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	TXGUI::UIPicture *tolua_ret = new TXGUI::UIPicture(pSprite,parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIPicture");

	return 1;
}

static int tolua_TXGUI_UIPicture_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"CCSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_UIPicture_new00(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	cocos2d::CCSprite *sprite = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,6,0);

	TXGUI::UIPicture *tolua_ret = (TXGUI::UIPicture*) new TXGUI::UIPicture(name, tag, z, parent, sprite);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIPicture");

	return 1;
}

static int tolua_TXGUI_UIPicture_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIPicture_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIPicture_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIPicture_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIPicture_setScale00(tolua_S);
	}

	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UIPicture_setInitScale(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::setInitScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setInitScale(scale);

	return 0;
}

static int tolua_TXGUI_UIPicture_setUseStretch(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::setUseStretch'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	bool useStretch =  tolua_toboolean(tolua_S,2,0) != 0;
	self->setUseStretch(useStretch);

	return 0;
}

static int tolua_TXGUI_UIPicture_getSprite(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::getSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	if(self->getCurrentNode() != NULL)
	{
#ifdef __cplusplus
		cocos2d::CCSprite *tolua_ret = static_cast<cocos2d::CCSprite*>(self->getCurrentNode());
#else
		cocos2d::CCSprite *tolua_ret = (cocos2d::CCSprite*)(self->getCurrentNode());
#endif
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
	}
	else
	{
		tolua_pushusertype(tolua_S,NULL,"CCSprite");
	}

	return 1;
}

static int tolua_TXGUI_UIPicture_setMySprite(lua_State* tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
    tolua_Error tolua_err;
    if(
        !tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
        !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
    {
        tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::setSprite'",&tolua_err);
        return 0;
    }
#endif
    TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
    cocos2d::CCSprite *pSprite = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
    self->setMySprite(pSprite);

    return 0;
}

static int tolua_TXGUI_UIPicture_setSprite00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::setSprite'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSprite *pSprite = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
	self->setSprite(pSprite);

	return 0;
}

static int tolua_TXGUI_UIPicture_setSprite01(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIPicture_setSprite00(tolua_S);
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSprite *pSprite = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
	bool isMerge = tolua_toboolean(tolua_S,3,0);
	self->setSprite(pSprite,isMerge);

	return 0;
}

static int tolua_TXGUI_UIPicture_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIPicture_addExtraPic(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIPicture::addExtraPic'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSprite *pSprite = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,3,0);
	self->addExtraPic(pSprite,*point);

	return 0;
}

static int tolua_TXGUI_UIPicture_getSpritePos(lua_State *tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UIPicture* self = (UIPicture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSpritePos'", NULL);
#endif
		{
			CCPoint tolua_ret = (CCPoint)  self->getSpritePos();
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
	tolua_error(tolua_S,"#ferror in function 'getSpritePos'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIPicture_getSpriteSize(lua_State *tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIPicture",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UIPicture* self = (UIPicture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSpriteSize'", NULL);
#endif
		{
			CCSize tolua_ret = (CCSize)  self->getSpriteSize();
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
	tolua_error(tolua_S,"#ferror in function 'getContentSize'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIPicture___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIPicture *self = (TXGUI::UIPicture*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UIScrollList
static int tolua_TXGUI_UIScrollList_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIScrollList *tolua_ret = new TXGUI::UIScrollList(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollList");

	return 1;
}

static int tolua_TXGUI_UIScrollList_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIScrollList_BindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::BindNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *node = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	self->BindNode(node);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIScrollList_setScale00(tolua_S);
	}

	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setScrollView(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIScrollView",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setScrollView'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIScrollView *pScrollView = (TXGUI::UIScrollView*) tolua_tousertype(tolua_S,2,0);
	self->setScrollView(pScrollView);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setBackground(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setBackground'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSprite *pBackground = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
	self->setBackground(pBackground);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setStartOffset(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setStartOffset'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *ptOffset = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setStartOffset(*ptOffset);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setViewSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setViewSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize *bSize = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	self->setViewSize(*bSize);

	return 0;
}

static int tolua_TXGUI_UIScrollList_getViewSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::getViewSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize tolua_ret = self->getViewSize();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCSize)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCSize));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIScrollList_setContentSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setContentSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize *bSize = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	self->setContentSize(*bSize);

	return 0;
}

static int tolua_TXGUI_UIScrollList_getContentSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::getContentSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize tolua_ret = self->getContentSize();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCSize)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCSize));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIScrollList_getContentOffset(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::getContentOffset'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint tolua_ret = self->getContentOffset();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIScrollList_setDirection(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setDirection'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	TXGUI::SWUIScrollViewDirection direction = (TXGUI::SWUIScrollViewDirection) (int) tolua_tonumber(tolua_S,2,0);
	self->setDirection(direction);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setIsVisible(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setIsVisible'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	bool isVisible =  tolua_toboolean(tolua_S,2,0) != 0;
	self->setIsVisible(isVisible);

	return 0;
}

static int tolua_TXGUI_UIScrollList_AddBaseItem(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::AddBaseItem'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize* pSize = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCNode* pNode = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	TXGUI::UIControlBase *tolua_ret = self->AddBaseItem(*pSize,pNode);

	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");
	return 1;
}

static int tolua_TXGUI_UIScrollList_AddNewItem(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;

	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCSprite",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"ccColor3B",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,7,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::AddNewItem'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSprite* pNormal = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCSprite* pClick = (cocos2d::CCSprite*) tolua_tousertype(tolua_S,3,0);
	const char* callBackFun = ((const char*)  tolua_tostring(tolua_S,4,0));
	const char* content = ((const char*)  tolua_tostring(tolua_S,5,0));
	ccColor3B* color = (ccColor3B*) tolua_tousertype(tolua_S,6,0);
	int number = (int) tolua_tonumber(tolua_S,7,0);

	UIButton* tolua_ret = self->AddNewItem(pNormal,pClick,std::string(callBackFun),content,*color,number);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIButton");

	return 1;
}

static int tolua_TXGUI_UIScrollList_removeItem(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;

	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::removeItem'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *obj = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,2,0);
	self->removeItem(obj);

	return 0;
}

static int tolua_TXGUI_UIScrollList_checkChildInView(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;

	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::removeItem'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIControlBase *obj = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->checkChildInView(obj);

	tolua_pushboolean(tolua_S,(bool)tolua_ret);
	return 1;
}

static int tolua_TXGUI_UIScrollList_clearChildrens(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;

	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::clearChildrens'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	self->clearChildrens();

	return 0;
}

static int tolua_TXGUI_UIScrollList_setCanReceiveScrollCallback(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;

	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setCanReceiveScrollCallback'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	bool ret = (bool) tolua_toboolean(tolua_S,2,0);
	self->setCanReceiveScrollCallback(ret);

	return 0;
}

static int tolua_TXGUI_UIScrollList_setScrollMaxHandler(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;

	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::setCanReceiveScrollCallback'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	LUA_FUNCTION nHandler = (toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScrollMaxHandler'", NULL);
#endif
		{
			self->setScrollMaxHandler(nHandler);
		}

	return 0;
}


static int tolua_TXGUI_UIScrollList_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollList",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollList::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIScrollList___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIScrollList *self = (TXGUI::UIScrollList*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::NiObject");

	return 1;
}

//UIScrollView
static int tolua_TXGUI_UIScrollView_viewWithViewSize00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIScrollView",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollLayer::nodeWithNoLayer'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCSize *size = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIScrollView *tolua_ret = TXGUI::UIScrollView::viewWithViewSize(*size);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollView");

	return 1;
}

static int tolua_TXGUI_UIScrollView_viewWithViewSize01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIScrollView",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIScrollView_viewWithViewSize00(tolua_S);
	}

	cocos2d::CCSize *size = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCNode *container = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	TXGUI::UIScrollView *tolua_ret = TXGUI::UIScrollView::viewWithViewSize(*size, container);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollView");

	return 1;
}

//UIScrollPage
static int tolua_TXGUI_UIScrollPage_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::new'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	TXGUI::UIScrollPage *tolua_ret = new TXGUI::UIScrollPage(parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollPage");

	return 1;
}

static int tolua_TXGUI_UIScrollPage_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_getScrollLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::UIScrollPage'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIScrollLayer *tolua_ret = self->getScrollLayer();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollLayer");

	return 1;
}

static int tolua_TXGUI_UIScrollPage_getScrollLayerPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
    tolua_Error tolua_err;
    if(
        !tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
    {
        tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::getScrollLayerPosition'",&tolua_err);
        return 0;
    }
#endif

    TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
    cocos2d::CCPoint tolua_ret = self->getScrollLayerPosition();
#ifdef __cplusplus
        void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
        tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
        void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
        tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif

   return 1;
}
//
//static int tolua_TXGUI_UIScrollPage_getScrollLayerPosition(lua_State* tolua_S)
//{
//#ifndef TOLUA_RELEASE
//    tolua_Error tolua_err;
//    if (
//        !tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
//        !tolua_isnoobj(tolua_S,2,&tolua_err)
//        )
//        goto tolua_lerror;
//    else
//#endif
//    {
//        TXGUI::UIScrollPage* self = (TXGUI::UIScrollPage*)  tolua_tousertype(tolua_S,1,0);
//#ifndef TOLUA_RELEASE
//        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScrollLayerPosition'", NULL);
//#endif
//        {
//            CCPoint tolua_ret = (CCPoint)  self->getScrollLayerPosition();
//            {
//#ifdef __cplusplus
//                void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
//                tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
//                tolua_register_gc(tolua_S,lua_gettop(tolua_S));
//#else
//                void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
//                tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
//                tolua_register_gc(tolua_S,lua_gettop(tolua_S));
//#endif
//            }
//        }
//    }
//    return 1;
//#ifndef TOLUA_RELEASE
//tolua_lerror:
//    tolua_error(tolua_S,"#ferror in function 'getScrollLayerPosition'.",&tolua_err);
//    return 0;
//#endif
//}

static int tolua_TXGUI_UIScrollPage_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIScrollPage_setScale00(tolua_S);
	}

	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale, yScale);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_setScrollLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIScrollLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::setScrollLayer'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIScrollLayer *pScrollLayer = (TXGUI::UIScrollLayer*) tolua_tousertype(tolua_S,2,0);
	self->setScrollLayer(pScrollLayer);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_setDirection(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::setDirection'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	TXGUI::EScrollDirection value = (TXGUI::EScrollDirection) (int) tolua_tonumber(tolua_S,2,0);
	self->setDirection(value);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_addPage00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::addPage'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCLayer *layer = (cocos2d::CCLayer*) tolua_tousertype(tolua_S,2,0);
	self->addPage(layer);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_addPage01(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLayer",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIScrollPage_addPage00(tolua_S);
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCLayer *layer = (cocos2d::CCLayer*) tolua_tousertype(tolua_S,2,0);
	bool changeScale = (bool)tolua_toboolean(tolua_S,3,0);
	self->addPage(layer, changeScale);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_addPages(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCArray",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::addPage'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCArray *layers = (cocos2d::CCArray*) tolua_tousertype(tolua_S,2,0);
	self->addPages(layers);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_addPagesByChildren(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::addPagesByChildren'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	self->addPagesByChildren();

	return 0;
}

static int tolua_TXGUI_UIScrollPage_removeAllPages(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::removeAllPages'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	self->removeAllPages();

	return 0;
}

static int tolua_TXGUI_UIScrollPage_setContentSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::setContentSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize *size = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	self->setContentSize(*size);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_getContentSize(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::getContentSize'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCSize tolua_ret = self->getContentSize();
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCSize)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCSize));
		tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

static int tolua_TXGUI_UIScrollPage_getCurPage(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::getCurPage'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	int tolua_ret = self->getCurPage();
	tolua_pushnumber(tolua_S,(int)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIScrollPage_getPageCount(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,2,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::getPageCount'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	int tolua_ret = self->getPageCount();
	tolua_pushnumber(tolua_S,(int)tolua_ret);
	return 1;
}

static int tolua_TXGUI_UIScrollPage_getPageLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
       !tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
       !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
       !tolua_isnoobj(tolua_S,3,&tolua_err)
       )
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::getPageLayer'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
    int pageIndex = (int) tolua_tonumber(tolua_S,2,0);
    CCLayer* tolua_ret = self->getPageLayer(pageIndex);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLayer");
	return 1;
}

static int tolua_TXGUI_UIScrollPage_moveToPage00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::moveToPage'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	int pageIndex = (int) tolua_tonumber(tolua_S,2,0);
	self->moveToPage(pageIndex);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_moveToPage01(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isboolean(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIScrollPage_moveToPage00(tolua_S);
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	int pageIndex = (int) tolua_tonumber(tolua_S,2,0);
	bool isAnimation = (bool) tolua_toboolean(tolua_S,3,0);
	self->moveToPage(pageIndex,isAnimation);

	return 0;
}

static int tolua_TXGUI_UIScrollPage_setTouchStolenDistance(lua_State *tolua_S)
{
	#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::setTouchStolenDistance'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	int val = (int) tolua_tonumber(tolua_S,2,0);
	self->setTouchStolenDistance(val);
	return 0;
}

static int tolua_TXGUI_UIScrollPage_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollPage::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIScrollPage_registerPageChangedCallbackFunc(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		TXGUI::UIScrollPage* self = (TXGUI::UIScrollPage*)  tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerPageChangedCallbackFunc'", NULL);
#endif
		{
			self->registerPageChangedCallbackFunc(nHandler);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerScriptAccelerateHandler'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIScrollPage_unregisterPageChangedCallbackFunc(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollPage",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		TXGUI::UIScrollPage* self = (TXGUI::UIScrollPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterPageChangedCallbackFunc'", NULL);
#endif
		{
			self->unregisterPageChangedCallbackFunc();
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unregisterPageChangedCallbackFunc'.",&tolua_err);
	return 0;
#endif
}

static int tolua_TXGUI_UIScrollPage___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIScrollPage *self = (TXGUI::UIScrollPage*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UIScrollLayer
static int tolua_TXGUI_UIScrollLayer_nodeWithNoLayer(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIScrollLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollLayer::nodeWithNoLayer'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCSize *pageSize = (cocos2d::CCSize*) tolua_tousertype(tolua_S,2,0);
	TXGUI::EScrollDirection dir = (TXGUI::EScrollDirection) (int) tolua_tonumber(tolua_S,3,0);
	TXGUI::UIScrollLayer *tolua_ret = TXGUI::UIScrollLayer::nodeWithNoLayer(*pageSize, dir);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollLayer");

	return 1;
}

static int tolua_TXGUI_UIScrollLayer_nodeWithLayers(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIScrollLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCArray",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCSize",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollLayer::nodeWithLayers'",&tolua_err);
		return 0;
	}
#endif
	cocos2d::CCArray *layers = (cocos2d::CCArray*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCSize *pageSize = (cocos2d::CCSize*) tolua_tousertype(tolua_S,3,0);
	TXGUI::EScrollDirection dir = (TXGUI::EScrollDirection) (int) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIScrollLayer *tolua_ret = TXGUI::UIScrollLayer::nodeWithLayers(layers, *pageSize, dir);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIScrollLayer");

	return 1;
}

static int tolua_TXGUI_UIScrollLayer_addPage(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCLayer",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollLayer::addPage'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollLayer *self = (TXGUI::UIScrollLayer*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCLayer *layer = (cocos2d::CCLayer*) tolua_tousertype(tolua_S,2,0);
	bool changeScale = (bool)tolua_toboolean(tolua_S,3,0);
	self->addPage(layer, changeScale);
	return 0;
}

static int tolua_TXGUI_UIScrollLayer_setTouchEnabled(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollLayer",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollLayer::setTouchEnabled'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollLayer *self = (TXGUI::UIScrollLayer*) tolua_tousertype(tolua_S,1,0);
	bool isEnable = (bool)tolua_toboolean(tolua_S,2,0);
	self->setTouchEnabled(isEnable);
	return 0;
}

static int tolua_TXGUI_UIScrollLayer_setShowPagesIndicator(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIScrollLayer",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIScrollLayer::setShowPagesIndicator'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIScrollLayer *self = (TXGUI::UIScrollLayer*) tolua_tousertype(tolua_S,1,0);
	bool isShow = (bool)tolua_toboolean(tolua_S,2,0);
	self->setShowPagesIndicator(isShow);
	return 0;
}

//TextInputField
static int tolua_TXGUI_TextInputField_textFieldWithPlaceHolder00(lua_State *tolua_S)
{
#ifndef TOLUA_ISEER_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TextInputField",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TextInputField::textFieldWithPlaceHolder'",&tolua_err);
		return 0;
	}
#endif
	const char *placeholder = tolua_tocppstring(tolua_S,2,0);
	const char *fontName = tolua_tocppstring(tolua_S,3,0);
	float fontSize = (float)tolua_tonumber(tolua_S,4,0);
	TXGUI::TextInputField *tolua_ret = TXGUI::TextInputField::textFieldWithPlaceHolder(placeholder, fontName, fontSize);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TextInputField");

	return 1;
}

static int tolua_TXGUI_TextInputField_textFieldWithPlaceHolder01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TextInputField",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCSize",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,6,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_TextInputField_textFieldWithPlaceHolder00(tolua_S);
	}

	const char *placeholder = tolua_tocppstring(tolua_S,2,0);
	cocos2d::CCSize *dimensions = (cocos2d::CCSize*) tolua_tousertype(tolua_S,3,0);
	cocos2d::CCTextAlignment alignment = (cocos2d::CCTextAlignment)((int)tolua_tonumber(tolua_S,4,0));
	const char *fontName = tolua_tocppstring(tolua_S,5,0);
	float fontSize = (float)tolua_tonumber(tolua_S,6,0);
	TXGUI::TextInputField *tolua_ret = TXGUI::TextInputField::textFieldWithPlaceHolder(placeholder, *dimensions, alignment, fontName, fontSize);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TextInputField");

	return 1;
}

//UITextInputField
static int tolua_TXGUI_UITextInputField_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::new'",&tolua_err);
		return 0;
	}
#endif

	const char *placeholder = tolua_tocppstring(tolua_S,2,0);
	const char *fontName = tolua_tocppstring(tolua_S,3,0);
	float fontSize = (float) tolua_tonumber(tolua_S,4,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,5,0);
	TXGUI::UITextInputField *tolua_ret = (TXGUI::UITextInputField*) new TXGUI::UITextInputField(placeholder, fontName, fontSize, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UITextInputField");

	return 1;
}

static int tolua_TXGUI_UITextInputField_new01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::TextInputField",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabel_new00(tolua_S);
	}

	TXGUI::TextInputField *pTextInputField = (TXGUI::TextInputField*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	TXGUI::UITextInputField *tolua_ret = (TXGUI::UITextInputField*) new TXGUI::UITextInputField(pTextInputField, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UITextInputField");

	return 1;
}

static int tolua_TXGUI_UITextInputField_new02(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCSize",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,6,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,7,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
	{
		return tolua_TXGUI_UILabel_new01(tolua_S);
	}

	const char *placeholder = tolua_tocppstring(tolua_S,2,0);
	cocos2d::CCSize *dimension = (cocos2d::CCSize*)tolua_tousertype(tolua_S,3,0);
	cocos2d::CCTextAlignment alignment = (cocos2d::CCTextAlignment)((int)tolua_tonumber(tolua_S,4,0));
	const char *fontName = tolua_tocppstring(tolua_S,5,0);
	float fontSize = (float) tolua_tonumber(tolua_S,6,0);
	cocos2d::CCNode *parent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,7,0);
	TXGUI::UITextInputField *tolua_ret = (TXGUI::UITextInputField*) new TXGUI::UITextInputField(placeholder, *dimension, alignment, fontName, fontSize, parent);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UITextInputField");

	return 1;
}

static int tolua_TXGUI_UITextInputField_new03(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,5,"TXGUI::UIControlBase",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,6,"TXGUI::TextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
	{
		return tolua_TXGUI_UITextInputField_new02(tolua_S);
	}

	std::string name = (std::string) tolua_tocppstring(tolua_S,2,0);
	int tag = (int) tolua_tonumber(tolua_S,3,0);
	float z = (float) tolua_tonumber(tolua_S,4,0);
	TXGUI::UIControlBase *parent = (TXGUI::UIControlBase*) tolua_tousertype(tolua_S,5,0);
	TXGUI::TextInputField *textInputField = (TXGUI::TextInputField*) tolua_tousertype(tolua_S,6,0);

	TXGUI::UITextInputField *tolua_ret = (TXGUI::UITextInputField*) new TXGUI::UITextInputField(name, tag, z, parent, textInputField);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UITextInputField");

	return 1;
}

static int tolua_TXGUI_UITextInputField_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::delete'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}

static int tolua_TXGUI_UITextInputField_BindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::BindNode'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *node = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	self->BindNode(node);

	return 0;
}

static int tolua_TXGUI_UITextInputField_setString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	std::string label = tolua_tocppstring(tolua_S,2,0);
	self->setString(label.c_str());

	return 0;
}

static int tolua_TXGUI_UITextInputField_getString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::getString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	const char *label = self->getString();
	tolua_pushstring(tolua_S,label);

	return 1;
}

static int tolua_TXGUI_UITextInputField_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UITextInputField_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setScale'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UITextInputField_setScale01(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UITextInputField_setScale00(tolua_S);
	}
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UITextInputField_getTextInputField(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::getTextInputField'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	if(self->getCurrentNode() != NULL)
	{
#ifdef __cplusplus
		TXGUI::TextInputField *tolua_ret = static_cast<TXGUI::TextInputField*>(self->getCurrentNode());
#else
		TXGUI::TextInputField *tolua_ret = (TXGUI::TextInputField*)(self->getCurrentNode());
#endif
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::TextInputField");
	}
	else
	{
		tolua_pushusertype(tolua_S,NULL,"TXGUI::TextInputField");
	}

	return 1;
}

static int tolua_TXGUI_UITextInputField_setTextInputField(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::TextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setTextInputField'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	TXGUI::TextInputField *pTextInputField = (TXGUI::TextInputField*) tolua_tousertype(tolua_S,2,0);
	self->setTextInputField(pTextInputField);

	return 0;
}

static int tolua_TXGUI_UITextInputField_setLengthLimit(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setLengthLimit'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	int lengthLimit = (int) tolua_tonumber(tolua_S,2,0);
	self->setLengthLimit(lengthLimit);

	return 0;
}

static int tolua_TXGUI_UITextInputField_setIsPassword(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setIsPassword'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	bool isPassword =  tolua_toboolean(tolua_S,2,0) != 0;
	self->setIsPassword(isPassword);

	return 0;
}

static int tolua_TXGUI_UITextInputField_setIsVisible(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::setIsVisible'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	bool isVisible = tolua_toboolean(tolua_S,2,0) != 0;
	self->setIsVisible(isVisible);

	return 0;
}

static int tolua_TXGUI_UITextInputField_clearString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::clearString'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	self->clearString();

	return 0;
}

static int tolua_TXGUI_UITextInputField_IsEqual(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::NiObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::IsEqual'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	TXGUI::NiObject *object = (TXGUI::NiObject*) tolua_tousertype(tolua_S,2,0);
	bool tolua_ret = self->IsEqual(object);
	tolua_pushboolean(tolua_S,(bool)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UITextInputField_becomeFirstResponder(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::becomeFirstResponder'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	self->becomeFirstResponder();

	return 0;
}

static int tolua_TXGUI_UITextInputField_resignFirstResponder(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UITextInputField",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UITextInputField::resignFirstResponder'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
	self->resignFirstResponder();

	return 0;
}

static int tolua_TXGUI_UITextInputField___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UITextInputField *self = (TXGUI::UITextInputField*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UIEditBox
//static int tolua_TXGUI_UIEditBox_new00(lua_State *tolua_S)
//{
//#ifndef TOLUA_TXGUI_RELEASE
//	tolua_Error tolua_err;
//	if(
//		!tolua_isusertable(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
//		!tolua_isnoobj(tolua_S,2,&tolua_err)
//		)
//	{
//		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::new'",&tolua_err);
//		return 0;
//	}
//#endif
//
//	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) new TXGUI::UIEditBox();
//	Mtolua_delete(self);
//	return 0;
//}

static int tolua_UIEditBox_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::delete'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *tolua_ret = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIEditBox");
	return 1;
}

static int tolua_TXGUI_UIEditBox_BindNode(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::BindNode'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCNode *node = (cocos2d::CCNode*) tolua_tousertype(tolua_S,2,0);
	self->BindNode(node);

	return 0;
}

static int tolua_TXGUI_UIEditBox_setString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::setString'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	std::string label = tolua_tocppstring(tolua_S,2,0);
	self->setString(label.c_str());

	return 0;
}

static int tolua_TXGUI_UIEditBox_getString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::getString'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	const char *label = self->getString();
	tolua_pushstring(tolua_S,label);

	return 1;
}

static int tolua_TXGUI_UIEditBox_setPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::setPosition'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	cocos2d::CCPoint *point = (cocos2d::CCPoint*) tolua_tousertype(tolua_S,2,0);
	self->setPosition(*point);

	return 0;
}

static int tolua_TXGUI_UIEditBox_setScale00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::setScale'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	float scale = (float) tolua_tonumber(tolua_S,2,0);
	self->setScale(scale);

	return 0;
}

static int tolua_TXGUI_UIEditBox_setScale01(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		return tolua_TXGUI_UIEditBox_setScale00(tolua_S);
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	int lengthLimit = (int) tolua_tonumber(tolua_S,2,0);
	self->setLengthLimit(lengthLimit);

	return 0;
}

static int tolua_TXGUI_UIEditBox_setLengthLimit(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::setLengthLimit'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	float xScale = (float) tolua_tonumber(tolua_S,2,0);
	float yScale = (float) tolua_tonumber(tolua_S,3,0);
	self->setScale(xScale,yScale);

	return 0;
}

static int tolua_TXGUI_UIEditBox_setIsVisible(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::setIsVisible'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	bool isVisible = tolua_toboolean(tolua_S,2,0) != 0;
	self->setIsVisible(isVisible);

	return 0;
}

static int tolua_TXGUI_UIEditBox_clearString(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::clearString'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	self->clearString();

	return 0;
}

static int tolua_TXGUI_UIEditBox_becomeFirstResponder(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::becomeFirstResponder'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	self->becomeFirstResponder();

	return 0;
}

static int tolua_TXGUI_UIEditBox_resignFirstResponder(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIEditBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIEditBox::resignFirstResponder'",&tolua_err);
		return 0;
	}
#endif

	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
	self->resignFirstResponder();

	return 0;
}

static int tolua_TXGUI_UIEditBox___UIControlBase__(lua_State *tolua_S)
{
	TXGUI::UIEditBox *self = (TXGUI::UIEditBox*) tolua_tousertype(tolua_S,1,0);
#ifdef __cplusplus
	TXGUI::UIControlBase *tolua_ret = static_cast<TXGUI::UIControlBase*>(self);
#else
	TXGUI::UIControlBase *tolua_ret = (TXGUI::UIControlBase*)self;
#endif
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIControlBase");

	return 1;
}

//UIManager
static int tolua_TXGUI_UIManager_sharedManager(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::sharedManager'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *tolua_ret = TXGUI::UIManager::sharedManager();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UIManager");

	return 1;
}

static int tolua_TXGUI_UIManager_getScaleFactor(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::getScaleFactor'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getScaleFactor();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIManager_getScaleFactorX(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::getScaleFactorX'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getScaleFactorX();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIManager_getScaleFactorY(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::getScaleFactorY'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	float tolua_ret = self->getScaleFactorY();
	tolua_pushnumber(tolua_S,(float)tolua_ret);

	return 1;
}

static int tolua_TXGUI_UIManager_createUILayoutFromFile(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCNode",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
		!tolua_isboolean(tolua_S,5,0,&tolua_err)	||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::createUILayoutFromFile'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	std::string fileName = (std::string) tolua_tocppstring(tolua_S,2,0);
	cocos2d::CCNode* pParent = (cocos2d::CCNode*) tolua_tousertype(tolua_S,3,0);
	std::string UIName = (std::string) tolua_tocppstring(tolua_S,4,0);
	bool bShow = tolua_toboolean(tolua_S, 5, 0) != 0;
	TXGUI::UILayout *tolua_ret = self->CreateUILayoutFromFile(fileName.c_str(), pParent,UIName, bShow);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");

	return 1;
}

static int tolua_TXGUI_UIManager_createUILayoutFromParent(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"TXGUI::UILayout",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
		!tolua_isboolean(tolua_S,5,0,&tolua_err)	||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::createUILayoutFromFile'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	std::string fileName = (std::string) tolua_tocppstring(tolua_S,2,0);

	//’‚¿Ô≤È’“µƒ «∏¯∂®µƒ≤ºæ÷µƒŒƒº˛µƒ∞Û∂®µƒΩ⁄µ„µƒ∏∏Ω⁄µ„
	TXGUI::UILayout *parent = (TXGUI::UILayout*) tolua_tousertype(tolua_S, 3,0);
	if (parent)
	{
		CCNode* bindNode = parent->getBindNode();
		if (bindNode)
		{
			bindNode = bindNode->getParent();
		}

		std::string UIName = (std::string) tolua_tocppstring(tolua_S,4,0);
		bool bShow = tolua_toboolean(tolua_S, 5, 0) != 0;
		TXGUI::UILayout *tolua_ret = self->CreateUILayoutFromFile(fileName.c_str(), bindNode, UIName, bShow);
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");
	}

	return 1;
}


static int tolua_TXGUI_UIManager_getUILayout(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::getUILayout'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	std::string UIName = (std::string) tolua_tocppstring(tolua_S,2,0);
	TXGUI::UILayout *tolua_ret = self->getUILayout(UIName);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"TXGUI::UILayout");

	return 1;
}

static int tolua_TXGUI_UIManager_getUIDefaultRoot(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::getUILayout'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	CCNode *tolua_ret = self->getUIDefaultRoot();
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");

	return 1;
}

static int tolua_TXGUI_UIManager_removeUILayout(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::removeUILayout'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	std::string UIName = (std::string) tolua_tocppstring(tolua_S,2,0);
	self->RemoveUILayout(UIName);

	return 1;
}

static int tolua_TXGUI_UIManager_containerCenterPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"TXGUI::UIManager",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"TXGUI::UIContainer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIManager::containerCenterPosition'",&tolua_err);
		return 0;
	}
#endif
	TXGUI::UIManager *self = (TXGUI::UIManager*) tolua_tousertype(tolua_S,1,0);
	TXGUI::UIContainer *container = (TXGUI::UIContainer*) tolua_tousertype(tolua_S,2,0);
	cocos2d::CCPoint tolua_ret = self->containerCenterPosition(container);
	{
#ifdef __cplusplus
		void *tolua_obj = Mtolua_new((cocos2d::CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void *tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cocos2d::CCPoint));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}

	return 1;
}

//getResourceIDByName
static int tolua_TXGUI_getResourceIDByName(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::getResourceIDByName'",&tolua_err);
		return 0;
	}
#endif
	std::string name = (std::string) tolua_tocppstring(tolua_S,1,0);
	int tolua_ret = TXGUI::getResourceIDByName(name.c_str());
	tolua_pushnumber(tolua_S,(int)tolua_ret);

	return 1;
}


static int tolua_collect_AspriteManager (lua_State* tolua_S)
{
	AspriteManager* self = (AspriteManager*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

//AspriteManager
static int tolua_AspriteManager_getInstance(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"AspriteManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'AspriteManager::getInstance'",&tolua_err);
		return 0;
	}
#endif
	
	AspriteManager* asMgr = AspriteManager::getInstance();
	tolua_pushusertype(tolua_S, asMgr,"AspriteManager");
	return 1;
}

/* method: getOneFrame of class  AspriteManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_AspriteManager_getOneFrame00
static int tolua_AspriteManager_getOneFrame(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"AspriteManager",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		AspriteManager* self = (AspriteManager*)  tolua_tousertype(tolua_S,1,0);
		std::string key = ((std::string)  tolua_tocppstring(tolua_S,2,0));
		std::string frameName = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOneFrame'", NULL);
#endif
		{
			CCSprite* tolua_ret = (CCSprite*)  self->getOneFrame(key,frameName);
			tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getOneFrame'.",&tolua_err);
	return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

static int tolua_AspriteManager_getOneFrameAndPosition(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"AspriteManager",0,&tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'AspriteManager::getOneFrame'",&tolua_err);
		return 0;
	}
#endif

	AspriteManager* asMgr = (AspriteManager*)(tolua_tousertype(tolua_S, 1, NULL));

	CCSprite* sprite = NULL;
	CCPoint pt = CCPointZero;
	string binfile =   tolua_tocppstring(tolua_S, 2, 0);
	string frame = tolua_tocppstring(tolua_S, 3, 0);
	if (asMgr)
	{
		ASprite* as = asMgr->getInstance()->getAsprite(binfile);
		if(as)
		{
			int iSpriteFrame = TXGUI::getResourceIDByName(frame.c_str());
			sprite = as->getSpriteFromFrame_Middle(iSpriteFrame,0,pt);
		}
	}

	tolua_pushusertype(tolua_S, sprite, "CCSprite");
#ifdef __cplusplus
	void* tolua_obj = Mtolua_new((CCPoint)(pt));
	tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
	tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
	void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
	tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
	tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	return 2;
}

/* method: getFramesRelativePos of class  AspriteManager */
#ifndef TOLUA_DISABLE_tolua_LuaEngine_AspriteManager_getFramesRelativePos00
static int tolua_LuaEngine_AspriteManager_getFramesRelativePos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"AspriteManager",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"CCPoint",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		AspriteManager* self = (AspriteManager*)  tolua_tousertype(tolua_S,1,0);
		std::string key = ((std::string)  tolua_tocppstring(tolua_S,2,0));
		std::string parentFrame = ((std::string)  tolua_tocppstring(tolua_S,3,0));
		std::string childFrame = ((std::string)  tolua_tocppstring(tolua_S,4,0));
		CCPoint childFrameAnchorPoint = *((CCPoint*)  tolua_tousertype(tolua_S,5,0));
		CCPoint* pos = ((CCPoint*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFramesRelativePos'", NULL);
#endif
		{
			CCSprite* tolua_ret = (CCSprite*)  self->getFramesRelativePos(key,parentFrame,childFrame,childFrameAnchorPoint,*pos);
			tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getFramesRelativePos'.",&tolua_err);
	return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

//init
TOLUA_API int tolua_TXGUI_open(lua_State *tolua_S)
{
	tolua_open(tolua_S);
	tolua_reg_types(tolua_S);
	tolua_module(tolua_S,NULL,0);
	tolua_beginmodule(tolua_S,NULL);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIControlBase","TXGUI::UIControlBase","",tolua_collect_TXGUI__UIControlBase);
#else
			tolua_cclass(tolua_S,"UIControlBase","TXGUI::UIControlBase","",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIControlBase");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIControlBase_new00);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIControlBase_delete);
				tolua_function(tolua_S,"getPosition",tolua_TXGUI_UIControlBase_getPosition);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIControlBase_setPosition);
				tolua_function(tolua_S,"getWorldPosition",tolua_TXGUI_UIControlBase_getWorldPosition);
				tolua_function(tolua_S,"setWorldPosition",tolua_TXGUI_UIControlBase_setWorldPosition);
				tolua_function(tolua_S,"getZOrder",tolua_TXGUI_UIControlBase_getZOrder);
				tolua_function(tolua_S,"setZOrder",tolua_TXGUI_UIControlBase_setZOrder);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIControlBase_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIControlBase_setScale01);
				tolua_function(tolua_S,"UpdateWorldPosition",tolua_TXGUI_UIControlBase_UpdateWorldPosition);
				tolua_function(tolua_S,"LocalPt2World",tolua_TXGUI_UIControlBase_LocalPt2World);
				tolua_function(tolua_S,"getLocal2WorldTransform",tolua_TXGUI_UIControlBase_getLocal2WorldTransform);
				tolua_function(tolua_S,"setAudioEffect",tolua_TXGUI_UIControlBase_setAudioEffect);
				tolua_function(tolua_S,"PlayAudio",tolua_TXGUI_UIControlBase_PlayAudio);
				tolua_function(tolua_S,"StopAudio",tolua_TXGUI_UIControlBase_StopAudio);
				tolua_function(tolua_S,"PauseAudio",tolua_TXGUI_UIControlBase_PauseAudio);
				tolua_function(tolua_S,"getTag",tolua_TXGUI_UIControlBase_getTag);
				tolua_function(tolua_S,"setTag",tolua_TXGUI_UIControlBase_setTag);
				tolua_function(tolua_S,"setName",tolua_TXGUI_UIControlBase_setName);
				tolua_function(tolua_S,"getName",tolua_TXGUI_UIControlBase_getName);
				tolua_function(tolua_S,"getCurrentNode",tolua_TXGUI_UIControlBase_getCurrentNode);
				tolua_function(tolua_S,"setParent",tolua_TXGUI_UIControlBase_setParent);
				tolua_function(tolua_S,"getParent",tolua_TXGUI_UIControlBase_getParent);
				tolua_function(tolua_S,"AddChild",tolua_TXGUI_UIControlBase_AddChild);
				tolua_function(tolua_S,"RemoveChild",tolua_TXGUI_UIControlBase_RemoveChild00);
				tolua_function(tolua_S,"RemoveChild",tolua_TXGUI_UIControlBase_RemoveChild01);
				tolua_function(tolua_S,"RemoveAllChildrenWithCleanup",tolua_TXGUI_UIControlBase_RemoveAllChildrenWithCleanup);
				tolua_function(tolua_S,"RemoveChildTreeByName",tolua_TXGUI_UIControlBase_RemoveChildTreeByName);
				tolua_function(tolua_S,"FindChildObjectByName",tolua_TXGUI_UIControlBase_FindChildObjectByName00);
				//tolua_function(tolua_S,"getAllChildEndsWithName",tolua_TXGUI_UIControlBase_getAllChildEndsWithName);
				tolua_function(tolua_S,"BindNode",tolua_TXGUI_UIControlBase_BindNode);
				tolua_function(tolua_S,"getBindNode",tolua_TXGUI_UIControlBase_getBindNode);
				//tolua_function(tolua_S,"setRoot",tolua_TXGUI_UIControlBase_setRoot);
				//tolua_function(tolua_S,"getRoot",tolua_TXGUI_UIControlBase_getRoot);
				tolua_function(tolua_S,"setLuaScriptFile",tolua_TXGUI_UIControlBase_setLuaScriptFile);
				tolua_function(tolua_S,"getLuaScriptFile",tolua_TXGUI_UIControlBase_getLuaScriptFile);
				tolua_function(tolua_S,"executeLuaScript",tolua_TXGUI_UIControlBase_executeLuaScript);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIControlBase_IsEqual);
				tolua_function(tolua_S, "setVisible", tolua_TXGUI_UIControlBase_setVisible);
				tolua_function(tolua_S, "isVisible", tolua_TXGUI_UIControlBase_isVisible);

				tolua_variable(tolua_S,"__NiObject__",tolua_TXGUI_UIControlBase___NiObject__,NULL);
				tolua_variable(tolua_S,"__UIAnimation__",tolua_TXGUI_UIControlBase___UIAnimation__,NULL);
				tolua_variable(tolua_S,"__UIButton__",tolua_TXGUI_UIControlBase___UIButton__,NULL);
				tolua_variable(tolua_S,"__UIButtonToggle__",tolua_TXGUI_UIControlBase___UIButtonToggle__,NULL);
				tolua_variable(tolua_S,"__UIContainer__",tolua_TXGUI_UIControlBase___UIContainer__,NULL);
				tolua_variable(tolua_S,"__UILabel__",tolua_TXGUI_UIControlBase___UILabel__,NULL);
				tolua_variable(tolua_S,"__UILabelAtlas__",tolua_TXGUI_UIControlBase___UILabelAtlas__,NULL);
				tolua_variable(tolua_S,"__UILayout__",tolua_TXGUI_UIControlBase___UILayout__,NULL);
				tolua_variable(tolua_S,"__UIPicture__",tolua_TXGUI_UIControlBase___UIPicture__,NULL);
				tolua_variable(tolua_S,"__UIScrollList__",tolua_TXGUI_UIControlBase___UIScrollList__,NULL);
				tolua_variable(tolua_S,"__UIScrollPage__",tolua_TXGUI_UIControlBase___UIScrollPage__,NULL);
				tolua_variable(tolua_S,"__UITextInputField__",tolua_TXGUI_UIControlBase___UITextInputField__,NULL);
				tolua_variable(tolua_S,"__UIEditBox__",tolua_TXGUI_UIControlBase___UIEditBox__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIAnimation","TXGUI::UIAnimation","TXGUI::UIControlBase",tolua_collect_TXGUI__UIAnimation);
#else
			tolua_cclass(tolua_S,"UIAnimation","TXGUI::UIAnimation","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIAnimation");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIAnimation_new00);
				tolua_function(tolua_S,"new", tolua_TXGUI_UIAnimation_new01);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIAnimation_delete);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIAnimation_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIAnimation_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIAnimation_setScale01);
				tolua_function(tolua_S,"getInitScale",tolua_TXGUI_UIAnimation_getInitScale);
				tolua_function(tolua_S,"setInitScale",tolua_TXGUI_UIAnimation_setInitScale);
				tolua_function(tolua_S,"getAnimSprite",tolua_TXGUI_UIAnimation_getAnimSprite);
				tolua_function(tolua_S,"setAnimSprite",tolua_TXGUI_UIAnimation_setAnimSprite);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIAnimation_IsEqual);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIAnimation___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIButton","TXGUI::UIButton","TXGUI::UIControlBase",tolua_collect_TXGUI__UIButton);
#else
			tolua_cclass(tolua_S,"UIButton","TXGUI::UIButton","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIButton");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIButton_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UIButton_new01);
				tolua_function(tolua_S,"new",tolua_TXGUI_UIButton_new02);
				tolua_function(tolua_S,"newExpand",tolua_TXGUI_UIButton_new03);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIButton_delete);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIButton_setPosition);
                tolua_function(tolua_S,"setMenuInitPriority",tolua_TXGUI_UIButton_setMenuInitPriority);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIButton_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIButton_setScale01);
				tolua_function(tolua_S,"getMenuItemSprite",tolua_TXGUI_UIButton_getMenuItemSprite);
				tolua_function(tolua_S,"getMenuItemSpriteExpand",tolua_TXGUI_UIButton_getMenuItemSpriteExpand);
				tolua_function(tolua_S,"setMenuItemSprite",tolua_TXGUI_UIButton_setMenuItemSprite);
				tolua_function(tolua_S,"getMenuItemLabel",tolua_TXGUI_UIButton_getMenuItemLabel);
				tolua_function(tolua_S,"setMenuItemLabel",tolua_TXGUI_UIButton_setMenuItemLabel);
				tolua_function(tolua_S,"setClickedFunc",tolua_TXGUI_UIButton_setClickedFunc);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIButton_IsEqual);
				tolua_function(tolua_S,"setEnabled",tolua_TXGUI_UIButton_setEnabled);
				tolua_function(tolua_S,"setMenuItemDispalyState",tolua_TXGUI_UIButton_setMenuItemDispalyState);
				tolua_function(tolua_S,"isEnabled",tolua_TXGUI_UIButton_isEnabled);
				tolua_function(tolua_S,"getAnimationNode",tolua_TXGUI_UIButton_getAnimationNode);
				tolua_function(tolua_S,"setIconSprite",tolua_TXGUI_UIButton_setIconSprite);
				tolua_function(tolua_S,"setDragMove",tolua_TXGUI_UIButton_setDragMove);
				tolua_function(tolua_S,"setFrameSprite",tolua_TXGUI_UIButton_setFrameSprite);
				tolua_function(tolua_S,"registerTapHandler",tolua_TXGUI_UIButton_registerTapHandler);
				tolua_function(tolua_S,"unregisterTapHandler",tolua_TXGUI_UIButton_unregisterTapHandler);
				tolua_function(tolua_S,"registerTapCancellHandler",tolua_TXGUI_UIButton_registerTapCancellHandler);
				tolua_function(tolua_S,"unregisterTapCancellHandler",tolua_TXGUI_UIButton_unregisterTapCancellHandler);
				tolua_function(tolua_S,"showTipIcon",tolua_TXGUI_UIButton_showTipIcon);
				tolua_function(tolua_S,"hideTipIcon",tolua_TXGUI_UIButton_hideTipIcon);
                tolua_function(tolua_S,"ShowRotateEffect",tolua_TXGUI_UIButton_ShowRotateEffect);
                tolua_function(tolua_S,"HideRotateEffect",tolua_TXGUI_UIButton_HideRotateEffect);
				tolua_function(tolua_S,"getMenuNode",tolua_TXGUI_UIButton_getMenuNode);
				tolua_function(tolua_S,"setTextLabel",tolua_TXGUI_UIButton_setTextLabel);
				tolua_function(tolua_S,"getButtonSize",tolua_TXGUI_UIButton_getButtonSize);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIButton___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIButtonToggle","TXGUI::UIButtonToggle","TXGUI::UIButton",tolua_collect_TXGUI__UIButtonToggle);
#else
			tolua_cclass(tolua_S,"UIButtonToggle","TXGUI::UIButtonToggle","TXGUI::UIButton",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIButtonToggle");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIButtonToggle_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UIButtonToggle_new01);
				tolua_function(tolua_S,"new",tolua_TXGUI_UIButtonToggle_new02);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIButtonToggle_delete);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIButtonToggle_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIButtonToggle_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIButtonToggle_setScale01);
				tolua_function(tolua_S,"getMenuItemToggle",tolua_TXGUI_UIButtonToggle_getMenuItemToggle);
				tolua_function(tolua_S,"setMenuItemToggle",tolua_TXGUI_UIButtonToggle_setMenuItemToggle);
				tolua_function(tolua_S,"setClickedFunc",tolua_TXGUI_UIButtonToggle_setClickedFunc);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIButtonToggle_IsEqual);
				tolua_function(tolua_S,"selected",tolua_TXGUI_UIButtonToggle_selected);
				tolua_function(tolua_S,"unselected",tolua_TXGUI_UIButtonToggle_unselected);
				tolua_variable(tolua_S,"__UIButton__",tolua_TXGUI_UIButtonToggle___UIButton__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
			tolua_constant(tolua_S,"RefPointNONE",TXGUI::NONE);
			tolua_constant(tolua_S,"RefPointLEFT_TOP",TXGUI::LEFT_TOP);
			tolua_constant(tolua_S,"RefPointLEFT_MIDDLE",TXGUI::LEFT_MIDDLE);
			tolua_constant(tolua_S,"RefPointLEFT_BOTTOM",TXGUI::LEFT_BOTTOM);
			tolua_constant(tolua_S,"RefPointMIDDLE_TOP",TXGUI::MIDDLE_TOP);
			tolua_constant(tolua_S,"RefPointMIDDLE_MIDDLE",TXGUI::MIDDLE_MIDDLE);
			tolua_constant(tolua_S,"RefPointMIDDLE_BOTTOM",TXGUI::MIDDLE_BOTTOM);
			tolua_constant(tolua_S,"RefPointRIGHT_TOP",TXGUI::RIGHT_TOP);
			tolua_constant(tolua_S,"RefPointRIGHT_MIDDLE",TXGUI::RIGHT_MIDDLE);
			tolua_constant(tolua_S,"RefPointRIGHT_BOTTOM",TXGUI::RIGHT_BOTTOM);

			tolua_constant(tolua_S,"RefModeNO_REFERENCE",TXGUI::NO_REFERENCE);
			tolua_constant(tolua_S,"RefModeNORMAL_REFERENCE",TXGUI::NORMAL_REFERENCE);
			tolua_constant(tolua_S,"RefModeOBJECT_REFERENCE",TXGUI::OBJECT_REFERENCE);

#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIContainer","TXGUI::UIContainer","TXGUI::UIControlBase",tolua_collect_TXGUI__UIContainer);
#else
			tolua_cclass(tolua_S,"UIContainer","TXGUI::UIContainer","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIContainer");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIContainer_new00);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIContainer_delete);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIContainer_setPosition);
				tolua_function(tolua_S,"UpdateWorldPosition",tolua_TXGUI_UIContainer_UpdateWorldPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIContainer_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIContainer_setScale01);
				tolua_function(tolua_S,"setRefMode",tolua_TXGUI_UIContainer_setRefMode);
				tolua_function(tolua_S,"getRefMode",tolua_TXGUI_UIContainer_getRefMode);
				tolua_function(tolua_S,"setRefPointMode",tolua_TXGUI_UIContainer_setRefPointMode);
				tolua_function(tolua_S,"getRefPointMode",tolua_TXGUI_UIContainer_getRefPointMode);
				tolua_function(tolua_S,"setWidthScale",tolua_TXGUI_UIContainer_setWidthScale);
				tolua_function(tolua_S,"getWidthScale",tolua_TXGUI_UIContainer_getWidthScale);
				tolua_function(tolua_S,"setWidthOffset",tolua_TXGUI_UIContainer_setWidthOffset);
				tolua_function(tolua_S,"getWidthOffset",tolua_TXGUI_UIContainer_getWidthOffset);
				tolua_function(tolua_S,"setHeightScale",tolua_TXGUI_UIContainer_setHeightScale);
				tolua_function(tolua_S,"getHeightScale",tolua_TXGUI_UIContainer_getHeightScale);
				tolua_function(tolua_S,"setHeightOffset",tolua_TXGUI_UIContainer_setHeightOffset);
				tolua_function(tolua_S,"getHeightOffset",tolua_TXGUI_UIContainer_getHeightOffset);
				tolua_function(tolua_S,"setSize",tolua_TXGUI_UIContainer_setSize00);
				tolua_function(tolua_S,"setSize",tolua_TXGUI_UIContainer_setSize01);
				tolua_function(tolua_S,"getSize",tolua_TXGUI_UIContainer_getSize);
				tolua_function(tolua_S,"setRefObjName",tolua_TXGUI_UIContainer_setRefObjName);
				tolua_function(tolua_S,"getRefObjName",tolua_TXGUI_UIContainer_getRefObjName);
				tolua_function(tolua_S,"setRefObj",tolua_TXGUI_UIContainer_setRefObj);
				tolua_function(tolua_S,"getRefObj",tolua_TXGUI_UIContainer_getRefObj);
				tolua_function(tolua_S,"CalWorldPos",tolua_TXGUI_UIContainer_CalWorldPos);
				tolua_function(tolua_S,"CalLocalPos",tolua_TXGUI_UIContainer_CalLocalPos);
				tolua_function(tolua_S,"containerCenterPosition",tolua_TXGUI_UIContainer_containerCenterPosition);
				tolua_function(tolua_S,"UpdateChildrenPos",tolua_TXGUI_UIContainer_UpdateChildrenPos);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIContainer_IsEqual);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIContainer___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UILabel","TXGUI::UILabel","TXGUI::UIControlBase",tolua_collect_TXGUI__UILabel);
#else
			tolua_cclass(tolua_S,"UILabel","TXGUI::UILabel","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UILabel");
				tolua_function(tolua_S,"new",tolua_TXGUI_UILabel_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UILabel_new01);
				tolua_function(tolua_S,"new",tolua_TXGUI_UILabel_new02);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UILabel_delete);
				tolua_function(tolua_S,"setString",tolua_TXGUI_UILabel_setString);
				tolua_function(tolua_S,"getString",tolua_TXGUI_UILabel_getString);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UILabel_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UILabel_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UILabel_setScale01);
				tolua_function(tolua_S,"getLabelTTF",tolua_TXGUI_UILabel_getLabelTTF);
				tolua_function(tolua_S,"setLabelTTF",tolua_TXGUI_UILabel_setLabelTTF);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UILabel_IsEqual);
				tolua_function(tolua_S,"setColor",tolua_TXGUI_UILabel_setColor);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UILabel___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UILabelAtlas","TXGUI::UILabelAtlas","TXGUI::UIControlBase",tolua_collect_TXGUI__UILabelAtlas);
#else
			tolua_cclass(tolua_S,"UILabelAtlas","TXGUI::UILabelAtlas","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UILabelAtlas");
				tolua_function(tolua_S,"new",tolua_TXGUI_UILabelAtlas_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UILabelAtlas_new01);
				tolua_function(tolua_S,"new",tolua_TXGUI_UILabelAtlas_new02);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UILabelAtlas_delete);
				tolua_function(tolua_S,"setString",tolua_TXGUI_UILabelAtlas_setString);
				tolua_function(tolua_S,"getString",tolua_TXGUI_UILabelAtlas_getString);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UILabelAtlas_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UILabelAtlas_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UILabelAtlas_setScale01);
				tolua_function(tolua_S,"getLabelTTF",tolua_TXGUI_UILabelAtlas_getLabelAtlas);
				tolua_function(tolua_S,"setLabelTTF",tolua_TXGUI_UILabelAtlas_setLabelAtlas);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UILabelAtlas_IsEqual);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UILabelAtlas___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UILayout","TXGUI::UILayout","TXGUI::UIControlBase",tolua_collect_TXGUI__UILayout);
#else
			tolua_cclass(tolua_S,"UILayout","TXGUI::UILayout","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UILayout");
				tolua_function(tolua_S,"new",tolua_TXGUI_UILayout_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UILayout_new01);
				tolua_function(tolua_S,"new",tolua_TXGUI_UILayout_new02);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UILayout_delete);
				tolua_function(tolua_S,"setAudioEffect",tolua_TXGUI_UILayout_setAudioEffect);
				tolua_function(tolua_S,"getBindNode",tolua_TXGUI_UILayout_getBindNode);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UILayout_setPosition);
				tolua_function(tolua_S,"RegisterMenuHandler",tolua_TXGUI_UILayout_RegisterMenuHandler);
				tolua_function(tolua_S,"UnregisterMenuHandler",tolua_TXGUI_UILayout_UnregisterMenuHandler);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UILayout_IsEqual);
				tolua_function(tolua_S,"setVisible",tolua_TXGUI_UILayout_setVisible);
				tolua_function(tolua_S,"setVisibleInStyle",tolua_TXGUI_UILayout_setVisibleInStyle);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UILayout___UIControlBase__,NULL);
				tolua_function(tolua_S,"EnableModeless",tolua_TXGUI_UILayout_EnableModeless);
                tolua_function(tolua_S,"SetNewPriority",tolua_TXGUI_UILayout_SetNewPriority);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIPicture","TXGUI::UIPicture","TXGUI::UIControlBase",tolua_collect_TXGUI__UIPicture);
#else
			tolua_cclass(tolua_S,"UIPicture","TXGUI::UIPicture","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIPicture");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIPicture_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UIPicture_new01);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIPicture_delete);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIPicture_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIPicture_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIPicture_setScale01);
				tolua_function(tolua_S,"setInitScale",tolua_TXGUI_UIPicture_setInitScale);
				tolua_function(tolua_S,"setUseStretch",tolua_TXGUI_UIPicture_setUseStretch);
				tolua_function(tolua_S,"getSprite",tolua_TXGUI_UIPicture_getSprite);
				tolua_function(tolua_S,"setSprite",tolua_TXGUI_UIPicture_setSprite00);
				tolua_function(tolua_S,"setSprite",tolua_TXGUI_UIPicture_setSprite01);
                tolua_function(tolua_S,"setMySprite",tolua_TXGUI_UIPicture_setMySprite);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIPicture_IsEqual);
				tolua_function(tolua_S,"addExtraPic",tolua_TXGUI_UIPicture_addExtraPic);
				tolua_function(tolua_S,"getSpriteSize",tolua_TXGUI_UIPicture_getSpriteSize);
				tolua_function(tolua_S,"getSpritePos",tolua_TXGUI_UIPicture_getSpritePos);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIPicture___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
			tolua_constant(tolua_S,"SWUIScrollViewDirectionHorizontal",TXGUI::SWUIScrollViewDirectionHorizontal);
			tolua_constant(tolua_S,"SWUIScrollViewDirectionVertical",TXGUI::SWUIScrollViewDirectionVertical);
			tolua_constant(tolua_S,"SWUIScrollViewDirectionBoth",TXGUI::SWUIScrollViewDirectionBoth);

#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIScrollList","TXGUI::UIScrollList","TXGUI::UIControlBase",tolua_collect_TXGUI__UIScrollList);
#else
			tolua_cclass(tolua_S,"UIScrollList","TXGUI::UIScrollList","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIScrollList");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIScrollList_new00);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIScrollList_delete);
				tolua_function(tolua_S,"BindNode",tolua_TXGUI_UIScrollList_BindNode);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIScrollList_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIScrollList_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIScrollList_setScale01);
				tolua_function(tolua_S,"setScrollView",tolua_TXGUI_UIScrollList_setScrollView);
				tolua_function(tolua_S,"setBackground",tolua_TXGUI_UIScrollList_setBackground);
				tolua_function(tolua_S,"setStartOffset",tolua_TXGUI_UIScrollList_setStartOffset);
				tolua_function(tolua_S,"setViewSize",tolua_TXGUI_UIScrollList_setViewSize);
				tolua_function(tolua_S,"getViewSize",tolua_TXGUI_UIScrollList_getViewSize);
				tolua_function(tolua_S,"setContentSize",tolua_TXGUI_UIScrollList_setContentSize);
				tolua_function(tolua_S,"getContentSize",tolua_TXGUI_UIScrollList_getContentSize);
				tolua_function(tolua_S,"getContentOffset",tolua_TXGUI_UIScrollList_getContentOffset);
				tolua_function(tolua_S,"setDirection",tolua_TXGUI_UIScrollList_setDirection);
				tolua_function(tolua_S,"setIsVisible",tolua_TXGUI_UIScrollList_setIsVisible);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIScrollList_IsEqual);
				tolua_function(tolua_S,"AddBaseItem",tolua_TXGUI_UIScrollList_AddBaseItem);
				tolua_function(tolua_S,"AddNewItem",tolua_TXGUI_UIScrollList_AddNewItem);
				tolua_function(tolua_S,"removeItem",tolua_TXGUI_UIScrollList_removeItem);
				tolua_function(tolua_S,"checkChildInView",tolua_TXGUI_UIScrollList_checkChildInView);
				tolua_function(tolua_S,"clearChildrens",tolua_TXGUI_UIScrollList_clearChildrens);
				tolua_function(tolua_S,"setCanReceiveScrollCallback",tolua_TXGUI_UIScrollList_setCanReceiveScrollCallback);
				tolua_function(tolua_S,"setScrollMaxHandler",tolua_TXGUI_UIScrollList_setScrollMaxHandler);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIScrollList___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIScrollView","TXGUI::UIScrollView","",tolua_collect_TXGUI__UIScrollView);
#else
			tolua_cclass(tolua_S,"UIScrollView","TXGUI::UIScrollView","",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIScrollView");
				tolua_function(tolua_S,"viewWithViewSize",tolua_TXGUI_UIScrollView_viewWithViewSize00);
				tolua_function(tolua_S,"viewWithViewSize",tolua_TXGUI_UIScrollView_viewWithViewSize01);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
			tolua_constant(tolua_S,"ESD_Horizontal",TXGUI::ESD_Horizontal);
			tolua_constant(tolua_S,"ESD_Vertical",TXGUI::ESD_Vertical);

#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIScrollPage","TXGUI::UIScrollPage","TXGUI::UIControlBase",tolua_collect_TXGUI__UIScrollPage);
#else
			tolua_cclass(tolua_S,"UIScrollPage","TXGUI::UIScrollPage","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIScrollPage");
				tolua_function(tolua_S,"new",tolua_TXGUI_UIScrollPage_new00);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UIScrollPage_delete);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIScrollPage_setPosition);
				tolua_function(tolua_S,"getScrollLayer",tolua_TXGUI_UIScrollPage_getScrollLayer);
                tolua_function(tolua_S,"getScrollLayerPosition",tolua_TXGUI_UIScrollPage_getScrollLayerPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIScrollPage_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UIScrollPage_setScale01);
				tolua_function(tolua_S,"setScrollLayer",tolua_TXGUI_UIScrollPage_setScrollLayer);
				tolua_function(tolua_S,"setDirection",tolua_TXGUI_UIScrollPage_setDirection);
				tolua_function(tolua_S,"addPage",tolua_TXGUI_UIScrollPage_addPage00);
				tolua_function(tolua_S,"addPage",tolua_TXGUI_UIScrollPage_addPage01);
				tolua_function(tolua_S,"addPages",tolua_TXGUI_UIScrollPage_addPages);
				tolua_function(tolua_S,"addPagesByChildren",tolua_TXGUI_UIScrollPage_addPagesByChildren);
				tolua_function(tolua_S,"removeAllPages",tolua_TXGUI_UIScrollPage_removeAllPages);
				tolua_function(tolua_S,"setContentSize",tolua_TXGUI_UIScrollPage_setContentSize);
				tolua_function(tolua_S,"getContentSize",tolua_TXGUI_UIScrollPage_getContentSize);
				tolua_function(tolua_S,"getCurPage",tolua_TXGUI_UIScrollPage_getCurPage);
                tolua_function(tolua_S,"getPageCount",tolua_TXGUI_UIScrollPage_getPageCount);
                tolua_function(tolua_S,"getPageLayer",tolua_TXGUI_UIScrollPage_getPageLayer);
				tolua_function(tolua_S,"moveToPage",tolua_TXGUI_UIScrollPage_moveToPage00);
				tolua_function(tolua_S,"moveToPage",tolua_TXGUI_UIScrollPage_moveToPage01);
				tolua_function(tolua_S,"setTouchStolenDistance",tolua_TXGUI_UIScrollPage_setTouchStolenDistance);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UIScrollPage_IsEqual);
				tolua_function(tolua_S,"registerPageChangedCallbackFunc",tolua_TXGUI_UIScrollPage_registerPageChangedCallbackFunc);
				tolua_function(tolua_S,"unregisterPageChangedCallbackFunc",tolua_TXGUI_UIScrollPage_unregisterPageChangedCallbackFunc);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIScrollPage___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIScrollLayer","TXGUI::UIScrollLayer","CCLayer",tolua_collect_TXGUI__UIScrollLayer);
#else
			tolua_cclass(tolua_S,"UIScrollLayer","TXGUI::UIScrollLayer","CCLayer",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIScrollLayer");
				tolua_function(tolua_S,"nodeWithNoLayer",tolua_TXGUI_UIScrollLayer_nodeWithNoLayer);
				tolua_function(tolua_S,"nodeWithLayers",tolua_TXGUI_UIScrollLayer_nodeWithLayers);
				tolua_function(tolua_S,"addPage",tolua_TXGUI_UIScrollLayer_addPage);
				tolua_function(tolua_S,"setTouchEnabled",tolua_TXGUI_UIScrollLayer_setTouchEnabled);
				tolua_function(tolua_S,"setShowPagesIndicator",tolua_TXGUI_UIScrollLayer_setShowPagesIndicator);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
#ifdef __cplusplus
			tolua_cclass(tolua_S,"TextInputField","TXGUI::TextInputField","CCTextFieldTTF",tolua_collect_TXGUI__TextInputField);
#else
			tolua_cclass(tolua_S,"TextInputField","TXGUI::TextInputField","CCTextFieldTTF",NULL);
#endif
			tolua_beginmodule(tolua_S,"TextInputField");
				tolua_function(tolua_S,"textFieldWithPlaceHolder",tolua_TXGUI_TextInputField_textFieldWithPlaceHolder00);
				tolua_function(tolua_S,"textFieldWithPlaceHolder",tolua_TXGUI_TextInputField_textFieldWithPlaceHolder01);
			tolua_endmodule(tolua_S);

#ifdef __cplusplus
			tolua_cclass(tolua_S,"UITextInputField","TXGUI::UITextInputField","TXGUI::UIControlBase",tolua_collect_TXGUI__UITextInputField);
#else
			tolua_cclass(tolua_S,"UITextInputField","TXGUI::UITextInputField","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UITextInputField");
				tolua_function(tolua_S,"new",tolua_TXGUI_UITextInputField_new00);
				tolua_function(tolua_S,"new",tolua_TXGUI_UITextInputField_new01);
				tolua_function(tolua_S,"new",tolua_TXGUI_UITextInputField_new02);
				tolua_function(tolua_S,"new",tolua_TXGUI_UITextInputField_new03);
				tolua_function(tolua_S,"delete",tolua_TXGUI_UITextInputField_delete);
				tolua_function(tolua_S,"BindNode",tolua_TXGUI_UITextInputField_BindNode);
				tolua_function(tolua_S,"setString",tolua_TXGUI_UITextInputField_setString);
				tolua_function(tolua_S,"getString",tolua_TXGUI_UITextInputField_getString);
				tolua_function(tolua_S,"setPosition",tolua_TXGUI_UITextInputField_setPosition);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UITextInputField_setScale00);
				tolua_function(tolua_S,"setScale",tolua_TXGUI_UITextInputField_setScale01);
				tolua_function(tolua_S,"getTextInputField",tolua_TXGUI_UITextInputField_getTextInputField);
				tolua_function(tolua_S,"setTextInputField",tolua_TXGUI_UITextInputField_setTextInputField);
				tolua_function(tolua_S,"setLengthLimit",tolua_TXGUI_UITextInputField_setLengthLimit);
				tolua_function(tolua_S,"setIsPassword",tolua_TXGUI_UITextInputField_setIsPassword);
				tolua_function(tolua_S,"setIsVisible",tolua_TXGUI_UITextInputField_setIsVisible);
				tolua_function(tolua_S,"clearString",tolua_TXGUI_UITextInputField_clearString);
				tolua_function(tolua_S,"IsEqual",tolua_TXGUI_UITextInputField_IsEqual);
				tolua_function(tolua_S,"becomeFirstResponder",tolua_TXGUI_UITextInputField_becomeFirstResponder);
				tolua_function(tolua_S,"resignFirstResponder",tolua_TXGUI_UITextInputField_resignFirstResponder);
				tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UITextInputField___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);

#ifdef __cplusplus
			tolua_cclass(tolua_S,"UIEditBox","TXGUI::UIEditBox","TXGUI::UIControlBase",tolua_collect_TXGUI__UIEditBox);
#else
			tolua_cclass(tolua_S,"UIEditBox","TXGUI::UIEditBox","TXGUI::UIControlBase",NULL);
#endif
			tolua_beginmodule(tolua_S,"UIEditBox");
			//tolua_function(tolua_S,"new",tolua_TXGUI_UIEditBox_new00);
			tolua_function(tolua_S,"delete",tolua_UIEditBox_delete);
			tolua_function(tolua_S,"BindNode",tolua_TXGUI_UIEditBox_BindNode);
			tolua_function(tolua_S,"setString",tolua_TXGUI_UIEditBox_setString);
			tolua_function(tolua_S,"getString",tolua_TXGUI_UIEditBox_getString);
			tolua_function(tolua_S,"setPosition",tolua_TXGUI_UIEditBox_setPosition);
			tolua_function(tolua_S,"setScale",tolua_TXGUI_UIEditBox_setScale00);
			tolua_function(tolua_S,"setScale",tolua_TXGUI_UIEditBox_setScale01);
			tolua_function(tolua_S,"setLengthLimit",tolua_TXGUI_UIEditBox_setLengthLimit);
			tolua_function(tolua_S,"setIsVisible",tolua_TXGUI_UIEditBox_setIsVisible);
			tolua_function(tolua_S,"clearString",tolua_TXGUI_UIEditBox_clearString);
			tolua_function(tolua_S,"becomeFirstResponder",tolua_TXGUI_UIEditBox_becomeFirstResponder);
			tolua_function(tolua_S,"resignFirstResponder",tolua_TXGUI_UIEditBox_resignFirstResponder);
			tolua_variable(tolua_S,"__UIControlBase__",tolua_TXGUI_UIEditBox___UIControlBase__,NULL);
			tolua_endmodule(tolua_S);

		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
			tolua_cclass(tolua_S,"UIManager","TXGUI::UIManager","",NULL);
			tolua_beginmodule(tolua_S,"UIManager");
				tolua_function(tolua_S,"sharedManager",tolua_TXGUI_UIManager_sharedManager);
				tolua_function(tolua_S,"getScaleFactor",tolua_TXGUI_UIManager_getScaleFactor);
				tolua_function(tolua_S,"getScaleFactorX",tolua_TXGUI_UIManager_getScaleFactorX);
				tolua_function(tolua_S,"getScaleFactorY",tolua_TXGUI_UIManager_getScaleFactorY);
				tolua_function(tolua_S,"getUILayout",tolua_TXGUI_UIManager_getUILayout);
				tolua_function(tolua_S,"createUILayoutFromFile",tolua_TXGUI_UIManager_createUILayoutFromFile);
				tolua_function(tolua_S,"createUILayoutFromParent",tolua_TXGUI_UIManager_createUILayoutFromParent);
				tolua_function(tolua_S,"removeUILayout",tolua_TXGUI_UIManager_removeUILayout);
				tolua_function(tolua_S,"getUIDefaultRoot",tolua_TXGUI_UIManager_getUIDefaultRoot);
				tolua_function(tolua_S,"containerCenterPosition",tolua_TXGUI_UIManager_containerCenterPosition);
			tolua_endmodule(tolua_S);
		tolua_endmodule(tolua_S);
		tolua_module(tolua_S,"TXGUI",0);
		tolua_beginmodule(tolua_S,"TXGUI");
			tolua_function(tolua_S,"getResourceIDByName",tolua_TXGUI_getResourceIDByName);
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"AspriteManager","AspriteManager","",tolua_collect_AspriteManager);
#else
		tolua_cclass(tolua_S,"AspriteManager","AspriteManager","",NULL);
#endif
		tolua_beginmodule(tolua_S,"AspriteManager");
		tolua_function(tolua_S,"getInstance",tolua_AspriteManager_getInstance);
		tolua_function(tolua_S, "getOneFrame", tolua_AspriteManager_getOneFrame);
		tolua_function(tolua_S,"getOneFrameAndPosition",tolua_AspriteManager_getOneFrameAndPosition);
		tolua_function(tolua_S,"getFramesRelativePos",tolua_LuaEngine_AspriteManager_getFramesRelativePos00);
		tolua_endmodule(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}