#ifndef LEVEL_BUILDER_CREATOR_H
#define LEVEL_BUILDER_CREATOR_H

#include "BaseLevelBuilder.h"


//////////////////////////////////////////////////////////////////////////
enum BUILDER_TYPE
{
	LEVEL_BUILDER_NORMAL_SCENE			= 0,
	LEVEL_BUILDER_ROOM					,

	//add more here

};

//////////////////////////////////////////////////////////////////////////

class LevelBuilderCreator
{

public:

	//
	static		BaseLevelBuilder*			sCreateLevelBuilder(int levelId, LevelLayer* levelLayer);

};



#endif