#include "OnlineNetworkManager.h"
#include "LevelManager.h"
#include "LevelLayer.h"
#include "MapDataManager.h"
#include "GameManager.h"
#include "SpriteSeer.h"
#include "BaseListener.h"
#include "MonsterData.h"
#include "BoneNPCManager.h"
#include "NpcSprite.h"
#include "SpriteElfManager.h"
#include "GameConfigFromLuaManager.h"
#include "UserData.h"
#include "SpriteElfDataCenter.h"
#include "SkillUIManager.h"
#include "ElfExploreManager.h"
#include "LuaTinkerManager.h"
#include "StoryDataCenter.h"
#include "TutorialsManager.h"

LevelManager* LevelManager::sInstance = NULL;

LevelManager::LevelManager()
	:mCurrentLevel(NULL),
	mLevelStatus(EUninit)
	,m_BattlelevelID(0)
{
	
}


LevelManager::~LevelManager()
{
	for (LEVEL_MAP::iterator itor = mLevelList.begin(); itor != mLevelList.end();)
	{
		int levelId = (*itor).first;
		freeLevelData(levelId);

		//clean up
		sLevelPack*	pack = (*itor).second;
		SAFE_DEL(pack->levelData);
		SAFE_DEL(pack);
		mLevelList.erase(itor++);
	}

	// destroy all the map data
	MapDataManager::Destroy();
	MonsterData::Destroy();
}

LevelManager* LevelManager::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new LevelManager();
		BaseListener::_SDM_init_Listener();
	}

	return sInstance;
}

void LevelManager::Destroy()
{
	if (sInstance)
	{
		delete sInstance;
		sInstance = NULL;
		BaseListener::_SDM_shutdown_Listener();
	}
}

void LevelManager::ShowMonsterOrNot(bool bShow)
{
	if (mCurrentLevel)
	{
		mCurrentLevel->ShowMonsterOrNot(bShow);
	}
}

LevelLayer* LevelManager::loadLevel(unsigned int levelId,bool bStoryMode /* = false */)
{
	LEVEL_MAP::iterator itor = mLevelList.find(levelId);
	//no record before
	if (itor == mLevelList.end())
	{
		sLevelPack* pack = new sLevelPack();

		//load level data 
		pack->levelData = loadLevelData(levelId);

		//build level layer
		pack->levelLayer = LevelLayer::node(levelId, pack->levelData,bStoryMode);

		//record to list		
		mLevelList.insert(std::make_pair<unsigned int, sLevelPack*>(levelId, pack));

		return pack->levelLayer;		
	}
	else
	{
		sLevelPack* pack = (*itor).second;
		if (pack->levelData == NULL)
		{
			pack->levelData = loadLevelData(levelId);
		}

		if (pack->levelLayer == NULL)
		{
			//build level layer
			pack->levelLayer = LevelLayer::node(levelId, pack->levelData);			
		}

		return pack->levelLayer;
	}
}

void LevelManager::freeLevel(unsigned int levelId, bool freeData)
{
	LEVEL_MAP::iterator itor = mLevelList.find(levelId);	
	if (itor != mLevelList.end())
	{
		sLevelPack* pack = (*itor).second;
		if (pack->levelLayer != NULL)
		{					
			//reset current level
			if (pack->levelLayer == mCurrentLevel)
			{
				removeLevelFromParent(mCurrentLevel);
				mCurrentLevel = NULL;
			}

			pack->levelLayer->destroyLayer();			
			pack->levelLayer = NULL;
			
		}

		if (freeData)
		{
			freeLevelData(levelId);

			//clean up
			SAFE_DEL(pack->levelData);
			SAFE_DEL(pack);
			mLevelList.erase(itor);
		}
	}
}

void LevelManager::freeLevel(LevelLayer* levelLayer, bool freeData)
{
	if (levelLayer == NULL)
	{
		return;
	}

	for (LEVEL_MAP::iterator itor = mLevelList.begin(); 
		itor != mLevelList.end(); ++itor)
	{
		if (levelLayer == (*itor).second->levelLayer)
		{
			freeLevel((*itor).first, freeData);
			break;
		}
	}
}

void LevelManager::freeAllLevels(bool freeData)
{
	for (LEVEL_MAP::iterator itor = mLevelList.begin(); 
		itor != mLevelList.end();)
	{

		freeLevel((*itor).first, freeData);

		if (freeData)
		{
			itor = mLevelList.begin();
		}
		else
		{
			++itor;
		}
	}
}

LevelLayer* LevelManager::activeLevel(unsigned int levelId, sActiveLevelConfig& cofig,bool bStoryMode /* = false */)
{
	mCurrentLevel = loadLevel(levelId,bStoryMode);	
	mCurrentLevel->onActiveLayer(cofig);

	//挂载精灵
	//SpriteElfManager::Get()->AttachElfToOneHero(1001,UserData::Get()->getUserId());
	// Note: 发送获取精灵信息的消息
	SpriteSeer * seer = GameManager::Get()->getHero();
	if (seer)
	{
		USERINFO mUserData = UserData::GetUserInfo();
		if(!mUserData.first_login)
			SpriteElfDataCenter::Get()->SendGetElfListMessage(UserData::Get()->getUserId(),seer->m_name.c_str());
	}
	
	SkillUIManager::getInstance()->sendGetUserSkillReqMessage(true);
	SkillUIManager::getInstance()->sendGetSlotReqMessage();
	ElfExploreManger::Get()->SendExploreReq(1);		
	OnlineNetworkManager::sShareInstance()->SendPlayerRedNameTimerReq();

	return mCurrentLevel;
}

//bool LevelManager::isMainLandLevel()
//{
//	if(!GameManager::Get()->GetSceneLayer())
//	{
//		return true;
//	}
//	int levelId = GameManager::Get()->GetSceneLayer()->getCurMapId();
//	return isHomeTownId(levelId);
//}

int LevelManager::getCurMapId()
{
	int mapId = 0;
	if (GameManager::Get()->GetSceneLayer())
	{
		mapId = GameManager::Get()->GetSceneLayer()->getCurMapId();
	}
	return mapId;
}

//////////////////////add by jameshuang , check current level whether PPVE or not
bool LevelManager::isCurrentPPVELevel()
{
	return isPPVELevel(m_BattlelevelID);
}

bool LevelManager::isCurrentPVELevel()
{
	return isPVELevel(m_BattlelevelID);
}

bool LevelManager::isCurrentPVPLevel()
{
	return isPVPLevel(m_BattlelevelID);
}
bool    LevelManager::isCurrentCommonPKLevel()
{
	return isCommonPKLevel(m_BattlelevelID);
}
bool    LevelManager::isCurrentMandPKLevel()
{
	return isMandPKLevel(m_BattlelevelID);
}
bool LevelManager::isCurrentEliteLevel()
{
	return isEliteLevel(m_BattlelevelID);
}


bool LevelManager::isCurrentPVAILevel()
{
	return isPVAILevel(m_BattlelevelID);
}

bool LevelManager::isCurrentUndergroundLevel()
{
	return isUndergroundLevel(m_BattlelevelID);
}

bool LevelManager::isCurrentCityDefendLevel()
{
	return isCityDefendLevel(m_BattlelevelID);
}

bool LevelManager::isCurrentWorldBossLevel()
{
	return isWorldBossLevel(m_BattlelevelID);
}

bool LevelManager::isCurrentBestFighterLevel()
{
	return isBestFighterLevel(m_BattlelevelID);
}

bool LevelManager::isCurrentHeroInstanceLevel()
{
	return isHeroInstanceLevel(m_BattlelevelID);
}

bool LevelManager::isCurrentGuildInstanceLevel()
{
	return isGuildInstanceLevel(m_BattlelevelID);
}

/////////////////////end by jameshuang , check current battle map type


void LevelManager::deactiveCurrentLevel(bool free, bool freeData)
{
	SpriteSeer* player = GameManager::Get()->getHero();
	if(player != NULL)
	{
		CCTargetedTouchDelegate* target = dynamic_cast<CCTargetedTouchDelegate*>(player);
		if (target)
		{
			CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(target);
		}
	}
	if (mCurrentLevel)
	{
		
			mCurrentLevel->removeAllOtherPlayer();
		
	}

	// Note: 清空Npc实例及数据
	std::map<int,SpriteNPC *> * pNpcTypeAndSprites = BoneNPCManager::getInstance()->GetNpcTypeIdAndSprites();
	if (pNpcTypeAndSprites)
	{
		for (std::map<int,SpriteNPC *>::iterator iter = pNpcTypeAndSprites->begin();
			iter != pNpcTypeAndSprites->end();iter++)
		{
			SpriteNPC *pNpc = (*iter).second;
			CCTargetedTouchDelegate* target = dynamic_cast<CCTargetedTouchDelegate*>(pNpc);
			if (target)
			{
				CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(target);
			}
		}
		pNpcTypeAndSprites->clear();
	}

	// Note: 清空精灵数据
	SpriteElfManager::Get()->RemoveAllElfInstance();

	//remove from parent node
	removeLevelFromParent(mCurrentLevel);

	//free level or data
	if (free == true && mCurrentLevel)
	{
		freeLevel(mCurrentLevel, freeData);
	}

	//CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	//reset current active level
	mCurrentLevel = NULL;
}


void LevelManager::removeLevelFromParent(LevelLayer* levelLayer)
{
	if (levelLayer && levelLayer->getParent() != NULL)
	{
		levelLayer->getParent()->removeChild(levelLayer, false);

		//stop all actions & remove delegates
		levelLayer->onDeactiveLayer();
	}
}


sLevelData* LevelManager::loadLevelData(unsigned int levelId)
{
	sLevelData* levelData = new sLevelData();

	//now the map file is controlled by game data manager, later can move to level manager
	levelData->mapFilePointer = (unsigned char*)MapDataManager::sharedManager()->getMapFileByMapID(levelId);
	//ASSERT(levelData->mapFilePointer != NULL, "map file is missing");

	//now the object sprite file is controlled by AspriteManager, later can move to level manager
	//char buff[20];
	//sprintf(buff, "spriteBin/%d.bin", levelId);
	//levelData->objectsSpriteFilePointer = AspriteManager::getInstance()->getAsprite(buff);

	//ASSERT(levelData->objectsSpriteFilePointer != NULL, "object sprites file is missing");

	levelData->levelID = levelId;

	return levelData;
}

void LevelManager::freeLevelData(unsigned int levelId)
{
	LEVEL_MAP::iterator itor = mLevelList.find(levelId);	
	if (itor != mLevelList.end())
	{
		sLevelPack* levelPack = (*itor).second;
		ASSERT(levelPack->levelData != NULL, "level data is empty");

		//now the data is controlled by others, so fake release
		levelPack->levelData->mapFilePointer = NULL;
		levelPack->levelData->objectsSpriteFilePointer = NULL;	
	}
}


LevelLayer* LevelManager::getCurrentLevelLayer()
{
	return mCurrentLevel;
}

cocos2d::CCPoint LevelManager::pointCellToPixel(CCPoint pointCell)
{
	CCPoint pixPoint;
	CPhysicsLayer* pPhysicsLayer = NULL;
	if(mCurrentLevel)
	{
		pPhysicsLayer = mCurrentLevel->getPhysicsLayer();
	}	
	if(pPhysicsLayer != NULL)
	{
		pixPoint = pPhysicsLayer->pointCellToPixel(pointCell);
	}

	return pixPoint;
}

cocos2d::CCPoint LevelManager::pointPixelToCell(CCPoint pointPixel)
{
	CCPoint cellPoint;

	CPhysicsLayer* pPhysicsLayer = NULL;
	if(mCurrentLevel)
	{
		pPhysicsLayer = mCurrentLevel->getPhysicsLayer();
	}	
	if(pPhysicsLayer != NULL)
	{
		cellPoint = pPhysicsLayer->pointPixelToCell(pointPixel);
	}

	return cellPoint;
}

bool LevelManager::isPointReachable(CCPoint pointPixel)
{
	bool ret = false;

	CPhysicsLayer* pPhysicsLayer = NULL;
	if(mCurrentLevel)
	{
		pPhysicsLayer = mCurrentLevel->getPhysicsLayer();
	}	
	if(pPhysicsLayer != NULL)
	{
		ret = pPhysicsLayer->isPointReachable(pointPixel);
	}

	return ret;
}

bool LevelManager::isPPVELevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 2;
}


bool LevelManager::isPVELevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 1 || type==9;
}

bool LevelManager::isPVPLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 3;
}

bool LevelManager::isPVAILevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return (type == 4 || type == 8);
}
bool    LevelManager::isCommonPKLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return(type == 12);
}
bool    LevelManager::isMandPKLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return(type == 13) ;
}
bool LevelManager::isEliteLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 5;
}

bool LevelManager::isUndergroundLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 6;
}

bool LevelManager::isCityDefendLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 7;
}

bool LevelManager::isWorldBossLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 10;
}

bool LevelManager::isBestFighterLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 11;
}

bool LevelManager::isHeroInstanceLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 14;
}

bool LevelManager::isGuildInstanceLevel(unsigned int level)
{
	int type = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", level, "Type");
	return type == 15;
}

void LevelManager::addOnePlayerTypeToBattle(unsigned int roleType)
{
	battlePlayerTypes.insert(roleType);
}

const std::set<unsigned int>& LevelManager::GetPlayerTypes()
{
	return battlePlayerTypes;
}

void LevelManager::ClearAllPlayerTypes()
{
	battlePlayerTypes.clear();
}

// ----------------------------------------------------------------------------------------------------

void Machtimer::initialize() {
}

void Machtimer::init() {
	t0 = currentTimeInMS();
}

void Machtimer::start() {
	t0 = currentTimeInMS();
}

long Machtimer::elapsed() {
	return currentTimeInMS()- t0;
}

long Machtimer::elapsedSec() {
	//return elapsed() * 10000;
	return elapsed() / 10000;

}
long Machtimer::elapsedMS() {

	return elapsed();
}

long Machtimer::currentTimeInMS() {

	struct cocos2d::cc_timeval now;

	cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);    
	//return (now.tv_sec * 1000 + now.tv_usec /1000.0)/10;
	return (now.tv_sec * 1000 + now.tv_usec /1000.0);

}