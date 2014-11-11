#include "BaseLevelBuilder.h"
#include "LevelObjectCreator.h"
#include "ObjectDefine.h"

#include "tolua++.h"
#include "CCLuaEngine.h"

#include "GameManager.h"
#include "UserData.h"
#include "MapDataManager.h"
#include "UIDefine.h"
#include "PhysicsLayer.h"
#include "GameResourceManager.h"
#include "ATLMapInfo.h"
#include "ATLObjectInfo.h"
#include "ATLObjectLayer.h"
#include "SpriteMonster.h"
#include "SkillDispMgr.h"
#include "cs_battle.pb.h"
#include "MonsterData.h"
#include "SpriteMonsterMgr.h"
#include "InstanceManager.h"
#include "BoneNPCManager.h"

#include "Engine.h"
#include "SpriteFactory.h"
#include "MainLandManager.h"
#include "ParticleManager.h"

#include "ATLObjectLayer.h"
#include "LuaTinkerManager.h"
//----------------------------------------------------------------------------------------
BaseLevelBuilder::BaseLevelBuilder(LevelLayer* levelLayer)
{
	ASSERT(levelLayer != NULL, "level layer is empty");
	m_level = levelLayer;
	m_mainLandTranspoint = 0;
}


BaseLevelBuilder::~BaseLevelBuilder()
{

}

//----------------------------------------------------------------------------------------
bool BaseLevelBuilder::build(bool bStoryMode /* = false */)
{
	excuteScript();

	return true;
}

//----------------------------------------------------------------------------------------
bool BaseLevelBuilder::excuteScript()
{
	char buffer[64];
	int levelId = m_level->getID();	
	sprintf(buffer, "Script\\Level\\%d.lua", levelId);
	
	CCLuaEngine* luaEngine = (CCLuaEngine*)CCScriptEngineManager::sharedManager()->getScriptEngine();
	if (luaEngine)
	{
		lua_State *lua_S = luaEngine->getLuaState();
		tolua_pushusertype(lua_S,(void*)this,"BaseLevelBuilder");	
		lua_setglobal(lua_S,"baseLevelBuilderObject");
	}

	//execute the script
	std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(buffer);
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeScriptFile(path.c_str());
	
	return true;
}


//----------------------------------------------------------------------------------------
void BaseLevelBuilder::createBacks()
{
	ASSERT(m_level->m_levelDataReference != NULL, "error； level data is NULL");

	//const char* pFileName =GameResourceManager::sharedManager()->storedFullPathFromRelativePath(
	//	(std::string("GameBin/allLevel.bin")).c_str());

	//CATLObjectInfo* info = NULL;

	// info.Load(pFileName);
	CATLObjectInfo* info = MainLandManager::Get()->getAllLevelInfo();
	if (info == NULL)
	{
		return;
	}

	std::vector<STileLayerInfo>  aa;
	sLevelData *bb = m_level->m_levelDataReference;

#if _WIN32
	#if _DEBUG
	{
		bool bHadMap = false;
		for(size_t i = 0; i < (*info).m_levels.size(); i++)
		{
			int id = atoi((*info).m_levels[i]->name.c_str());
			if(id == m_level->m_levelDataReference->levelID)
			{
				bHadMap = true;
				break;
			}
		}
		if(!bHadMap)
		{
			ASSERT(0, "Can't find the map!!!");
		}
	}
	
	#endif

#endif

	for(size_t i = 0; i < (*info).m_levels.size(); i++)
	{
		int id = atoi((*info).m_levels[i]->name.c_str());
		if(id == m_level->m_levelDataReference->levelID)
		{
			for(size_t j = 0; j < (*info).m_levels[i]->layers.size(); j++)
			{
				CATLObjectInfo::LAYER::TILED* pTiled = dynamic_cast<CATLObjectInfo::LAYER::TILED*>((*info).m_levels[i]->layers[j]);
				if(pTiled != NULL)
				{
					STileLayerInfo tileInfo;
					tileInfo.isVisible = true;
					tileInfo.name = pTiled->mapFile;
					tileInfo.zorder = j;
					aa.push_back(tileInfo);
				}
			}
		}		
	}

	CCSprite* tileLayer = LevelObjectCreator::sCreateTileLayer(aa, bb);
	tileLayer->setPosition(ccp(0, 0));
	tileLayer->setVertexZ(LevelLayer::sCalcDepth(MAP_Z_TILE_BACKGROUND));
	m_level->addChild(tileLayer, MAP_Z_TILE_BACKGROUND);

	createObjects((*info));
}

//----------------------------------------------------------------------------------------
void BaseLevelBuilder::createObjects(CATLObjectInfo &info)
{
	CATLObjectLayer* pLayer = new CATLObjectLayer();
	pLayer->ParseObject(&info, m_level->m_levelDataReference->levelID, m_level);
	pLayer->setPosition(ccp(0, 0));
	m_level->m_pObjectsLayer = pLayer;
	m_level->addChild(pLayer, MAP_Z_OBJECT);
	pLayer->autorelease();	

	/// for debug information
	Engine::GetDebugInf().AtlObj_Node_Num = pLayer->getBatchNodeNumber();
}

//---------------------------------------------------------------------------------------- 
void BaseLevelBuilder::createPhysicsLayer()
{
	//
	ASSERT(m_level->m_pPhysicsLayer == NULL , "Physics Layer != NULL !");
	
	int num = 0;
	MAP_POINT_INFO info;

	unsigned char* file = (unsigned char*)MapDataManager::sharedManager()->getMapFileByMapID(m_level->m_levelDataReference->levelID);;

	int cellW = 0;
	memcpy(&cellW, file, sizeof(int));
	file += sizeof(int);
	int row = 0;
	memcpy(&row, file, sizeof(int));
	file += sizeof(int);
	int col = 0;
	memcpy(&col, file, sizeof(int));
	file += sizeof(int);
	

	m_level->m_pPhysicsLayer = new CPhysicsLayer();
	m_level->m_pPhysicsLayer->init(file,
		row,
		col,
		cellW,
		cellW);	

	
	for(int j = 0; j < row; j++)
	{
		for(int i = 0; i < col; i++)
		{
			unsigned char flag = 0;
			memcpy(&flag, file, sizeof(unsigned char));
			file += sizeof(unsigned char);
			float x = (float)(i * cellW);
			float y = (float)(j * cellW);
			if(flag & (0x01 << 1))
			{
				MAP_POINT_INFO info;
				info.point.x = x;
				info.point.y = y;
				m_level->m_bornPointList.push_back(info);
			}
		}
	}

	int count = m_level->m_bornPointList.size();
	for(int i = 0; i < count; i++)
	{
		m_level->m_bornPointList[i].ID = *(int*)file;
		file += sizeof(int);
	}

	file = NULL;
}

//----------------------------------------------------------------------------------------
void BaseLevelBuilder::createTransPoints()
{
	if(MainLandManager::Get()->isMainLand(m_level->m_id))
	{

		MAP_POINT_INFO info;
        {
            CCPoint normalGateCell = MainLandManager::Get()->getCityCell(m_level->m_id, "normalGate");
		    CCPoint normalGatePoint = m_level->m_pPhysicsLayer->pointCellToPixel(normalGateCell);

            //create normal fuben
    		info.name="transpoint";
    		info.point.x = normalGatePoint.x;
    		info.point.y = normalGatePoint.y;
    		
    		info.ID = 200101;

    		SpriteTransPoint* transPoint = LevelObjectCreator::sCreateTransPoint(info);
    		transPoint->setTag(MAP_TAG_TRANSPOINT);
    		m_level->m_pObjectsLayer->addChild(transPoint, LevelLayer::sCalcZorder(info.point));

    		m_mainLandTranspoint = transPoint;
        }

		{
			//create jing ying fu ben icon on main land by jackniu
			CCPoint eliteGateCell = MainLandManager::Get()->getCityCell(m_level->m_id, "eliteGate");
			CCPoint eliteGatePoint = m_level->m_pPhysicsLayer->pointCellToPixel(eliteGateCell);

			MAP_POINT_INFO info;
			info.name="elite_transpoint";
			info.point.x = eliteGatePoint.x;
			info.point.y = eliteGatePoint.y;
			info.ID = 200102;

			SpriteTransPoint* elitetransPoint = LevelObjectCreator::sCreateTransPoint(info);
			elitetransPoint->setTag(MAP_TAG_TRANSPOINT);
			m_level->m_pObjectsLayer->addChild(elitetransPoint, LevelLayer::sCalcZorder(info.point));

			m_eliteTranspoint = elitetransPoint;
		}
		
	}
	else
	{
		MAP_POINT_INFO info;
		info.point = InstanceManager::Get()->getPortalPosition();
		info.ID = InstanceManager::Get()->getPortalId();
		if ((info.point.x == 0 && info.point.y) || info.ID == 0)
		{
			return;
		}
		info.name="transpoint";
		SpriteTransPoint* transPoint = LevelObjectCreator::sCreateTransPoint(info);
		
		transPoint->setPosition(info.point);
		transPoint->setMapId(info.ID);

		transPoint->setTag(MAP_TAG_TRANSPOINT);
		m_level->m_pObjectsLayer->addChild(transPoint, LevelLayer::sCalcZorder(info.point));
	}

}

void BaseLevelBuilder::createMonsterBornPoints()
{
	vector<CCPoint> points = InstanceManager::Get()->getMonsterBornPoints();
	for (size_t i = 0; i < points.size(); i++)
	{
		EffectSprite* monsterBornPoint = ParticleManager::Get()->createEffectSprite(205,"");
		monsterBornPoint->SetAnim(kType_Play,1,true);

		CCNode * bornNode = CCNode::create();
		bornNode->addChild(monsterBornPoint);
		bornNode->setPosition(points[i]);

		m_level->m_pObjectsLayer->addChild(bornNode, MAP_Z_TRANSPOINT);		
	}	
	InstanceManager::Get()->clearMonsterBornPoints();
}


//----------------------------------------------------------------------------------------
void BaseLevelBuilder::createNpcs()
{
	//NPCManager::sharedManager()->showNpcOnMap(m_level->m_id, m_level);
	BoneNPCManager::getInstance()->LoadOneLevelNPC(m_level->m_id, m_level->m_pObjectsLayer);
}

//----------------------------------------------------------------------------------------
void BaseLevelBuilder::createSeer()
{
	CCLOG("BaseLevelBuilder::createSeer %s", UserData::Get()->getUserName());

	// TODO : tile physics
	if(m_level->m_levelDataReference->mapFilePointer == NULL)
		return;

	unsigned int playerType = UserData::GetUserType();
	ASSERT(m_level->m_levelDataReference->mapFilePointer != NULL, "map file is empty");	

	SpriteSeer* seer = LevelObjectCreator::sCreateSeer(UserData::Get()->getUserId(),UserData::Get()->getUserName(),
		playerType ,
		true
		);

	seer->SetID(UserData::Get()->getUserId());
	seer->setTag(MAP_TAG_SEER);
	m_level->m_pObjectsLayer->addChild(seer, LevelLayer::sCalcZorder(seer->getPosition()));

	GameManager::Get()->setHero(seer);
	GameManager::Get()->setHeroTypeId(seer->GetTypeId());

	SkillDispMgr::Get()->Load("Data/skill.bin", "Data/animationMap.bin");
	SkillDispMgr::Get()->LoadFairy("Data/fairyskill.bin", "Data/fairyanimation.bin");


	// TODO : 实际上，影子可以放到LevelLayer里，不一定要放到ObjectLayer里。
	CATLObjectLayer* pObjLayer = dynamic_cast<CATLObjectLayer*>(m_level->m_pObjectsLayer);
	if(pObjLayer)
	{
		pObjLayer->AddRoleShadow(seer);
	}
}

//----------------------------------------------------------------------------------------
//			others function means other players'
//----------------------------------------------------------------------------------------
void BaseLevelBuilder::removeAllOtherPlayer()
{
	if(m_level == NULL)
	{
		return;
	}
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_level->m_othersPlayerInfo.begin();
    for (;itor!= m_level->m_othersPlayerInfo.end() ;itor++)
	{
		PLAYER_ID uid = itor->first ;
		 SpriteFactory::sharedFactory()->RemoveElfFromHero(uid);

		 std::map<PLAYER_ID, LevelLayer::WAKL_INFO>::iterator iterWalk = m_level->m_walkInfo.find(uid);
		 if(iterWalk != m_level->m_walkInfo.end())
		 {
			 m_level->m_walkInfo.erase(iterWalk);
		 }

		 OthersPlayerInfo playerInfo = (*itor).second;

		 //
		

		 //remove seer
		 SpriteSeer* seer = playerInfo.seer;
		 bool isMainLand = MainLandManager::Get()->isMainLand(m_level->m_id);
		 if(isMainLand)
			 seer->unRegisterWithTouchDispatcher();
		 if (seer && m_level->m_pObjectsLayer)
		 {
			 seer->getRoot()->stopAllActions();


			 m_level->m_pObjectsLayer->removeChild(seer, true);
		 }
	}
}
void BaseLevelBuilder::addOtherPlayer(PLAYER_ID uid, const char* userName,unsigned int type, cocos2d::CCPoint pt, unsigned int orient, unsigned int battleSide)
{
	// 添加对可能的crash的保护
	CCAssert(m_level != NULL,"BaseLevelBuilder.m_level is null");
	if(m_level == NULL)
	{
		return;
	}
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_level->m_othersPlayerInfo.find(uid);
	if (itor == m_level->m_othersPlayerInfo.end())
	{
		//
		OthersPlayerInfo playerInfo;
		playerInfo.seer = NULL;
		playerInfo.pet = NULL;
#if _DEBUG
		playerInfo.name = userName;
#endif
		//add seer
		ASSERT(m_level->m_levelDataReference->mapFilePointer != NULL, "map file is empty");
		SpriteSeer* seer = LevelObjectCreator::sCreateSeer(uid,userName,
			type, 
			false);

		if(NULL == seer)
		{
			return;
		}

		

		seer->SetID(uid);
		seer->setTag(MAP_TAG_OTHER_SEER);
		seer->setPosition(pt);

		if (m_level->m_pObjectsLayer)
		{
			m_level->m_pObjectsLayer->addChild(seer, LevelLayer::sCalcZorder(seer->getPosition()));
			
			
			
			bool isMainLand = MainLandManager::Get()->isMainLand(m_level->m_id);
			if(isMainLand)
				seer->registerWithTouchDispatcher();
			int	animID;
			if (isMainLand)
			{
				animID = kTypeIdle_FaceDown_Stand_MAIN;
			}
			else
			{
				animID = kTypeIdle_FaceDown_Stand_OTH;
			}
			
			/*switch(orient)
			{
			case DIRECTION_UP:
			animID = kTypeIdle_FaceUp_Stand_MAIN;
			break;
			case DIRECTION_LEFT:
			break;
			case DIRECTION_BOTTOM:
			break;
			case DIRECTION_RIGHT:
			break;
			default:
			break;
			}*/
			seer->SetAnim(animID,1,true);
		}

		playerInfo.seer = seer;

		//add pet
		//if (petType != 0)
		//{
		//	SpriteElf* elf = LevelObjectCreator::sCreateSpriteElf(petType, 0, true,
		//		MAP_TAG_OTHER_FOLLOW_SPRITE ,
		//		true);

		//	elf->setTag(MAP_TAG_OTHER_FOLLOW_SPRITE);
		//	elf->followSeerPosition(pt, true);
		//	m_level->addChild(elf, LevelLayer::sCalcZorder(elf->getPosition()));

		//	playerInfo.pet = elf;
		//}

		//
		m_level->m_othersPlayerInfo.insert(std::pair<PLAYER_ID, OthersPlayerInfo>(uid, playerInfo));

		CATLObjectLayer* pObjLayer = dynamic_cast<CATLObjectLayer*>(m_level->m_pObjectsLayer);
		if(pObjLayer)
		{
			pObjLayer->AddRoleShadow(seer);
		}

	}
	else
	{
		CCLOG("error, the player has add before");
	}
}

void BaseLevelBuilder::removeOtherPlayer(PLAYER_ID uid)
{
	// Note: Remove Attach Elf 
	SpriteFactory::sharedFactory()->RemoveElfFromHero(uid);

	std::map<PLAYER_ID, LevelLayer::WAKL_INFO>::iterator iterWalk = m_level->m_walkInfo.find(uid);
	if(iterWalk != m_level->m_walkInfo.end())
	{
		m_level->m_walkInfo.erase(iterWalk);
	}

	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_level->m_othersPlayerInfo.find(uid);
	if (itor != m_level->m_othersPlayerInfo.end())
	{
		OthersPlayerInfo playerInfo = (*itor).second;

		//
		m_level->m_othersPlayerInfo.erase(itor);
		
		//remove seer
		SpriteSeer* seer = playerInfo.seer;
		bool isMainLand = MainLandManager::Get()->isMainLand(m_level->m_id);
		if(isMainLand)
			seer->unRegisterWithTouchDispatcher();
		if (seer && m_level->m_pObjectsLayer)
		{
			seer->getRoot()->stopAllActions();
			
			
			m_level->m_pObjectsLayer->removeChild(seer, true);
		}
	}
	else
	{
		CCLOG("error, the player has not add before");
	}
}

SpriteSeer* BaseLevelBuilder::getPlayer(PLAYER_ID uid)
{
	SpriteSeer* seer = null;
	SpriteSeer* hero = GameManager::Get()->getHero();
	PLAYER_ID heroId = UserData::getUserId();
	if(hero && heroId == uid)
	{
		return hero;
	}
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_level->m_othersPlayerInfo.find(uid);
	if (itor != m_level->m_othersPlayerInfo.end())
	{
		OthersPlayerInfo playerInfo = (*itor).second;

		//remove seer
		seer = playerInfo.seer;		
	}
	return seer;
}

//----------------------------------------------------------------------------------------
//			others function means other players'
//----------------------------------------------------------------------------------------
void BaseLevelBuilder::addMonster(MonsterInitInfo info)
{
	std::map<uint32_t, SpriteMonster*>::iterator itor = m_level->m_MonsterInfo.find(info.uid);
	if (itor == m_level->m_MonsterInfo.end())
	{
		//add monster
		ASSERT(m_level->m_levelDataReference->mapFilePointer != NULL, "map file is empty");
		SpriteMonster* monster = LevelObjectCreator::sCreateSpriteMonster(info);

		PLAYER_ID playerId;
		playerId.setPlayId(info.uid,0,0);
		monster->SetID(playerId);
		monster->setTag(MAP_TAG_MONSTER);
		monster->setPosition(info.pos);

		//printf("addMonster %f, %f\n", info.pos.x, info.pos.y);

		//添加阴影
		/*ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("spriteBin/shadow.bin");
		if(pAsprite != NULL)
		{
			CCPoint cp;
			CCSprite* pCSprite = pAsprite->getSpriteFromFrame(0, 0, cp);
			if(pCSprite != NULL)
			{
				pCSprite->setPosition(ccp(0, 0));
				pCSprite->setTag(SHADOW_TAG);
				monster->addChildToRoot(pCSprite);
			}
		}*/

		CATLObjectLayer* pObjLayer = dynamic_cast<CATLObjectLayer*>(m_level->m_pObjectsLayer);
		if(pObjLayer)
		{
			pObjLayer->AddRoleShadow(monster);
		}

		//为精英怪添加特效
		if(info.isElite)
		{
			EffectSprite * eliteEffect = ParticleManager::Get()->createEffectSprite(206,"");
			eliteEffect->SetAnim(kType_Play, 1, true);
			eliteEffect->setTag(ELITE_TAG);
			monster->addChildToRoot(eliteEffect);
		}

		if (m_level->m_pObjectsLayer)
		{
			m_level->m_pObjectsLayer->addChild(monster, LevelLayer::sCalcZorder(monster->getPosition()));			
		}

		m_level->m_MonsterInfo.insert(std::pair<uint32_t, SpriteMonster*>(info.uid, monster));
	}
	else
	{
		CCLOG("error, the player has add before");
	}
}

void BaseLevelBuilder::showMonsterOrNot(bool bShow)
{
	if (m_level)
	{
		std::map<uint32_t, SpriteMonster*>::iterator iter = m_level->m_MonsterInfo.begin();
		while(iter != m_level->m_MonsterInfo.end())
		{
			SpriteMonster* pMonster = (*iter).second;
			if (pMonster)
			{
				pMonster->getRoot()->setVisible(bShow);
				pMonster->setShadowVisible(bShow);
			}

			iter ++;
		}
	}
}

void BaseLevelBuilder::removeMonster(unsigned int uid)
{
	std::map<uint32_t, SpriteMonster*>::iterator itor = m_level->m_MonsterInfo.find(uid);
	if (itor != m_level->m_MonsterInfo.end())
	{
		SpriteMonster* monster = (*itor).second;
		m_level->m_MonsterInfo.erase(itor);
		m_level->m_pObjectsLayer->removeChild(monster, true);
	}
	else
	{
		CCLOG("error, the player has not add before");
	}
}

void BaseLevelBuilder::createMonster()
{
	bool isMainLand = MainLandManager::Get()->isMainLand(m_level->m_id);
	if (!isMainLand)
	{
		int instanceId = InstanceManager::Get()->getCurInstanceId();
		//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Monster.lua", "CreateRaidCacheMonster", instanceId);
	}

	std::vector<MonsterInfo>* monsterList = MonsterData::GetMonsterInfo()->getMonsterList();
	if(monsterList != NULL)
	{
		for(std::vector<MonsterInfo>::iterator it= monsterList->begin();
			it != monsterList->end();it++)
		{
			MonsterInfo info = *it;

			MonsterInitInfo mInitInfo;

			mInitInfo.uid = info.id();
			mInitInfo.typeId = info.type_id();
			CSPoint position = info.point();
			mInitInfo.totalHp = info.blood();
			mInitInfo.speed = info.speed();
			mInitInfo.isBoss = info.boss();
			mInitInfo.pos = m_level->m_pPhysicsLayer->pointCellToPixel(ccp(position.xpos(), position.ypos()));
			mInitInfo.monsterName = "monster";
			mInitInfo.isElite = info.elite();
			mInitInfo.hp_line = info.hp_line();
			mInitInfo.m_level = info.level();
			mInitInfo.isAlliance = info.city();
			mInitInfo.useCache = false;

			addMonster(mInitInfo);

			MonsterMoveValue moves = info.moves();
			int pathCount = moves.paths_size();
			if(pathCount > 0)
			{
				std::vector<CCPoint> points;
				for(int j = 0; j < pathCount; ++j)
				{
					CSPoint path = moves.paths(j);	
					CCPoint pixPoint = LevelManager::sShareInstance()->pointCellToPixel(ccp(path.xpos(), path.ypos()));
					points.push_back(pixPoint);
				}
				SpriteMonster * monster = SpriteMonsterMgr::Get()->GetMonster(mInitInfo.uid);			
			}
		}
	}
}

//----------------------------------------------------------------------------------------

SpriteTransPoint* BaseLevelBuilder::GetMainLandTransPointInstance()
{
	return m_mainLandTranspoint;
}

SpriteTransPoint* BaseLevelBuilder::GetEliteTransPointInstance()
{
	return m_eliteTranspoint;
}
//----------------------------------------------------------------------------------------
