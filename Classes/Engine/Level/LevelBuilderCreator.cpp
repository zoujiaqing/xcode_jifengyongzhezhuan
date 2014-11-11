#include "LevelBuilderCreator.h"
#include "LevelBuilderNormalScene.h"
#include "LevelBuilderRoom.h"
#include "BaseLevelBuilder.h"

#include "LevelDefine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <direct.h>
#include <io.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "SystemInfoJni.h"
#include <sys/stat.h>
#include <sys/types.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

//////////////////////////////////////////////////////////////////////////
//Now hack the builder type by level id, later will update by config file
//////////////////////////////////////////////////////////////////////////

BaseLevelBuilder*	LevelBuilderCreator::sCreateLevelBuilder(int levelId, LevelLayer* levelLayer)
{
	return new LevelBuilderNormalScene(levelLayer);
}