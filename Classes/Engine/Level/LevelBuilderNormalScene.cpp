#include "LevelBuilderNormalScene.h"



LevelBuilderNormalScene::LevelBuilderNormalScene(LevelLayer* levelLayer)
	:BaseLevelBuilder(levelLayer)
{

}


LevelBuilderNormalScene::~LevelBuilderNormalScene()
{

}


bool LevelBuilderNormalScene::build(bool bStoryMode /* = false */)
{
	createBacks();
	createPhysicsLayer();
	createTransPoints();
	//if (false == bStoryMode)
	{
		createSeer();
	}	
	createNpcs();
	createMonster();
	createMonsterBornPoints();
	if (bStoryMode)
	{		
		showMonsterOrNot(false);
	}

	return true;
}