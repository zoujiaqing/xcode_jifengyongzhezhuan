#include "LevelObjectCreator.h"
#include "ObjectDefine.h"

#include "UIDefine.h"
#include "UserData.h"
#include "MapDataManager.h"
#include "ATLMapInfo.h"
#include "ATLMapLayer.h"
#include "GameResourceManager.h"
#include "SpriteMonsterMgr.h"

#include "HeroRoleManager.h"
#include "MainLandManager.h"

CCSprite* LevelObjectCreator::sCreateMapBack(ASprite* asFile, int frame)
{	
	ASSERT(asFile != NULL, "error: level data is empty");

	CCPoint pt;
	CCSprite* back = CCSprite::create();
	
	CCSprite* sprite = asFile->getSpriteFromFrame_Middle(frame, 0, pt);
	if(sprite)
	{
        float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
        CCPoint mapOffsetPoint = CCPoint(IPHONE_HD_OFFSET_X / deviceScale, IPHONE_HD_OFFSET_Y / deviceScale);
        
		pt = LevelMultiResolution::sTransformPointToMap(ccpAdd(pt, mapOffsetPoint));
		sprite->setPosition(pt);
		sprite->setScale(LevelMultiResolution::sGetLevelScale());

		back->addChild(sprite);
	}
	else
	{
		printf("error: the level map back %d is empty", frame);
	}

	return back;
}

CCSprite* LevelObjectCreator::sCreateTileLayer(std::vector<STileLayerInfo>& layers, sLevelData* pLevelData)
{	
	CCSprite* back = CCSprite::create();
	for(size_t i = 0; i < layers.size(); i++)
	{
		const char* pFileName =GameResourceManager::sharedManager()->storedFullPathFromRelativePath(
																	(std::string("mapBin/") + layers[i].name).c_str());

		CATLMapInfo info;
		info.Load(pFileName);
		CATLMapLayer* pLayer = new CATLMapLayer();
		pLayer->autorelease();
		pLayer->ParseMap(&info);
		pLayer->setVisible(layers[i].isVisible);
		back->addChild(pLayer, layers[i].zorder);

		pLevelData->row = info.GetHeight();
		pLevelData->col = info.GetWidth();
		pLevelData->cellWidth = info.GetTileWidth();
		pLevelData->cellHeight = info.GetTileHeight();

	}
	return back;
}

MapObject* LevelObjectCreator::sCreateMapAnimations(int index, const char* spriteName)
{
	MapObject* object = new MapObject(spriteName, index, index * 2);				
	object->autorelease();

	return object;
}


SpriteTransPoint* LevelObjectCreator::sCreateTransPoint(const MAP_POINT_INFO& info)
{
	//const char* name = "transpoint";
	//const char* name = MapDataManager::sharedManager()->getMapInfoByMapId(info.ID)->mapName.c_str();
	SpriteTransPoint* transPoint = SpriteTransPoint::node(info.name, info.point, info.ID);
	//transPoint->setPosition(info.point);		

	return transPoint;
}


SpriteSeer* LevelObjectCreator::sCreateSeer(PLAYER_ID id,const char* name, int type, bool isPlayerSet)
{
	bool bCreateHeroAttachEffect = true;
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if(bMainLandLevel)
	{
		bCreateHeroAttachEffect = false;

	}
	SpriteSeer* seer = HeroRoleManager::Get()->createRole(id, name, type, isPlayerSet,bCreateHeroAttachEffect);	

	return seer;
}


SpriteMonster* LevelObjectCreator::sCreateSpriteMonster(MonsterInitInfo info)
{	
	SpriteMonster* elf = 
		SpriteMonsterMgr::Get()->CreateMonster(info);

	return elf;
}
