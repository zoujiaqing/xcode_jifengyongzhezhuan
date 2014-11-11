#ifndef LEVEL_OBJECT_CREATOR
#define LEVEL_OBJECT_CREATOR

#include "cocos2d.h"
#include "LevelDefine.h"
#include "MapDataManager.h"

class SpriteTransPoint;
class SpriteElf;
class SpriteSeer;
class SpriteMine;
class AnimSprite;
class SpriteRoomAnimation;
class MapObject;
class SpriteMonster;


class LevelObjectCreator
{

public:

	////////////////////////////////////map back//////////////////////////////////////
	static		CCSprite*							sCreateMapBack(ASprite* asFile, int frame);
	static		CCSprite*							sCreateTileLayer(std::vector<STileLayerInfo>& layers,  sLevelData* pLevelData);

	////////////////////////////////////map animations//////////////////////////////////////
	static		MapObject*							sCreateMapAnimations(int index, const char* spriteName);

	////////////////////////////////////map transpoint//////////////////////////////////////
	static		SpriteTransPoint*					sCreateTransPoint(const MAP_POINT_INFO& info);

	///////////////////////////////////map mine///////////////////////////////////////
	static		SpriteMine*							sCreateMine(const MAP_POINT_INFO& info, int levelId);

	//////////////////////////////////seer////////////////////////////////////////
	static		SpriteSeer*							sCreateSeer(PLAYER_ID id,const char* name, int type, bool isPlayerSet);

	/////////////////////////////////elf/////////////////////////////////////////
	static		SpriteElf*							sCreateSpriteElf(int id, int level, bool isFollow, int type, bool isPlayerSet);

	////////////////////////////////room animation//////////////////////////////////////////
	static		SpriteRoomAnimation*				sCreateRoomAnimation();

	//////////////////////////////////////////////////////////////////////////
	/// create monster
	static       SpriteMonster* sCreateSpriteMonster(MonsterInitInfo info);
};




#endif