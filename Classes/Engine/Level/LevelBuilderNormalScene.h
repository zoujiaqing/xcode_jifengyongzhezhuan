#ifndef LEVEL_BUILDER_NORMAL_SCENE_H
#define LEVEL_BUILDER_NORMAL_SCENE_H

#include "BaseLevelBuilder.h"



class LevelBuilderNormalScene : public BaseLevelBuilder
{

public:

	LevelBuilderNormalScene(LevelLayer* levelLayer);
	virtual ~LevelBuilderNormalScene();

	virtual bool		build(bool bStoryMode = false);

};



#endif