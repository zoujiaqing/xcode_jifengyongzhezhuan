#include "LuaLevelCreator.h"
#include "BaseLevelBuilder.h"
#include "LevelDefine.h"



#ifndef __cplusplus
#	include "stdlib.h"
#endif

#include "string.h"


TOLUA_API int tolua_LevelCreator_open(lua_State *tolua_S);



//register classes
static void tolua_reg_types(lua_State *tolua_S)
{
	tolua_usertype(tolua_S, "BaseLevelBuilder");

	//add more

}



//////////////////////////////////////////////////////////////////////////
//BaseLevelBuilder
//////////////////////////////////////////////////////////////////////////

//constructor
static int tolua_Level_BaseLevelBuilder_new00(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertable(tolua_S,1,"BaseLevelBuilder",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"LevelLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'BaseLevelBuilder::new'",&tolua_err);
		return 0;
	}
#endif

	LevelLayer* levelLayer = (LevelLayer*) tolua_tousertype(tolua_S,2,0);
	BaseLevelBuilder* tolua_ret = (BaseLevelBuilder*) new BaseLevelBuilder(levelLayer);
	tolua_pushusertype(tolua_S,(void*)tolua_ret,"BaseLevelBuilder");

	return 1;
}


//destructor
static int tolua_Level_BaseLevelBuilder_delete(lua_State *tolua_S)
{
#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"BaseLevelBuilder",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'TXGUI::UIControlBase::delete'",&tolua_err);
		return 0;
	}
#endif
	BaseLevelBuilder *self = (BaseLevelBuilder*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);

	return 0;
}


//createBacks
static int tolua_BaseLevelBuilder_CreateBacks(lua_State *tolua_S)
{

#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"BaseLevelBuilder",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'BaseLevelBuilder::createBacks'",&tolua_err);
		return 0;
	}
#endif

	BaseLevelBuilder* self = (BaseLevelBuilder*)tolua_tousertype(tolua_S,1,0);
	self->createBacks();
    
    return 0;
}




//createTransPoints
static int tolua_BaseLevelBuilder_CreateTransPoints(lua_State *tolua_S)
{

#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"BaseLevelBuilder",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'BaseLevelBuilder::createTransPoints'",&tolua_err);
		return 0;
	}
#endif

	BaseLevelBuilder* self = (BaseLevelBuilder*)tolua_tousertype(tolua_S,1,0);
	self->createTransPoints();
    
    return 0;
}



//createNpcs
static int tolua_BaseLevelBuilder_CreateNpcs(lua_State *tolua_S)
{

#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"BaseLevelBuilder",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'BaseLevelBuilder::createNpcs'",&tolua_err);
		return 0;
	}
#endif

	BaseLevelBuilder* self = (BaseLevelBuilder*)tolua_tousertype(tolua_S,1,0);
	self->createNpcs();
    
    return 0;
}


//createSeer
static int tolua_BaseLevelBuilder_CreateSeer(lua_State *tolua_S)
{

#ifndef TOLUA_TXGUI_RELEASE
	tolua_Error tolua_err;
	if(
		!tolua_isusertype(tolua_S,1,"BaseLevelBuilder",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'BaseLevelBuilder::createSeer'",&tolua_err);
		return 0;
	}
#endif

	BaseLevelBuilder* self = (BaseLevelBuilder*)tolua_tousertype(tolua_S,1,0);
	self->createSeer();
    
    return 0;
}


//init
TOLUA_API int tolua_LevelCreator_open(lua_State *tolua_S)
{
	tolua_open(tolua_S);
	tolua_reg_types(tolua_S);
	tolua_module(tolua_S,NULL,0);
	tolua_beginmodule(tolua_S,NULL);

	tolua_cclass(tolua_S,"BaseLevelBuilder","BaseLevelBuilder","",NULL);
		tolua_beginmodule(tolua_S,"BaseLevelBuilder");
			tolua_function(tolua_S,"new",tolua_Level_BaseLevelBuilder_new00);
			tolua_function(tolua_S,"delete",tolua_Level_BaseLevelBuilder_delete);
			tolua_function(tolua_S,"createBacks",tolua_BaseLevelBuilder_CreateBacks);		

			tolua_function(tolua_S,"createTransPoints",tolua_BaseLevelBuilder_CreateTransPoints);	
	
			tolua_function(tolua_S,"createNpcs",tolua_BaseLevelBuilder_CreateNpcs);	
			tolua_function(tolua_S,"createSeer",tolua_BaseLevelBuilder_CreateSeer);	

		tolua_endmodule(tolua_S);

	tolua_endmodule(tolua_S);

	return 1;
}