#ifndef LEVEL_BUILDER_ROOM_H
#define LEVEL_BUILDER_ROOM_H

#include "BaseLevelBuilder.h"



class LevelBuilderRoom : public BaseLevelBuilder
{

public:

	LevelBuilderRoom(LevelLayer* levelLayer);
	virtual ~LevelBuilderRoom();

	virtual bool		build();
};



#endif