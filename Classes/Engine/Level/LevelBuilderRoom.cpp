#include "LevelBuilderRoom.h"



LevelBuilderRoom::LevelBuilderRoom(LevelLayer* levelLayer)
	:BaseLevelBuilder(levelLayer)
{

}


LevelBuilderRoom::~LevelBuilderRoom()
{

}


bool LevelBuilderRoom::build()
{
	createBacks();	
	createPhysicsLayer();
	createNpcs();
	createSeer();

	return true;
}

