#include <cmath>

#include "OnlineNetworkManager.h"

#include "LevelDefine.h"
#include "ObjectDefine.h"
#include "ListenerDefine.h"
#include "LevelBuilderCreator.h"


#include "CrossUtil.h"
#include "UIDefine.h"
#include "UserData.h"
#include "EnginePlayerManager.h"
#include "GameManager.h"
#include "MapDataManager.h"
#include "ParticleManager.h"
#include "AttackChecker.h"
#include "SpriteMonsterMgr.h"
#include "cs_battle.pb.h"
#include "MonsterData.h"
#include "TaskManager.h"
#include "SkillDispMgr.h"
#include "BoneNPCManager.h" 
#include "CameraController.h"
#include "SpriteElfManager.h"
#include "HeroFighter.h"
#include "GameAudioManager.h"
#include "StoryInstanceDirector.h"
#include "ItemUpdateManager.h"
#include "PvAIManager.h"
#include "TutorialsManager.h"
#include "WaitingLayerManager.h"
#include "LuaTinkerManager.h"
#include "MainLandManager.h"
#include "GuildInstanceManager.h"

LevelLayer::LevelLayer()
:m_levelDataReference(NULL)
,m_levelBuilder(NULL)
,m_pPhysicsLayer(NULL)
,m_pObjectsLayer(NULL)
,pressTime(0.0f)
,touchEffect(NULL)
,m_id(0)
,m_pathOffset(0)
{
}


LevelLayer::~LevelLayer()
{		
	GameAudioManager::sharedManager()->stopAllEffect();
	//
	SAFE_DEL(m_levelBuilder);


	m_othersPlayerInfo.clear();

	SAFE_DEL(m_pPhysicsLayer);

	ItemUpdateManager::Get()->resetData();
	m_levelDataReference = NULL;
}


LevelLayer* LevelLayer::node(unsigned int levelId, sLevelData* data,bool bStoryMode /* = false */)
{
	LevelLayer* pRet = LevelLayer::create();
	if (pRet && pRet->initLevel(levelId, data,bStoryMode))
	{
		return pRet;
	}
	else
	{
		pRet->release();
		pRet = NULL;
		return NULL;
	}
}

// on "init" you need to initialize your instance
bool LevelLayer::init()
{
	bool bRet = false;
	do {
		CC_BREAK_IF(! cocos2d::CCLayer::init());
		bRet = true;
	}while(0);

	return true;
}

bool LevelLayer::initLevel(unsigned int levelId, sLevelData* data,bool bStoryMode /* = false */)
{
	m_id = levelId;
	m_levelDataReference = data;


	//build level
	m_levelBuilder = LevelBuilderCreator::sCreateLevelBuilder(levelId, this);
	m_levelBuilder->build(bStoryMode);

	
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0 ,false);
	//debugDrawPath();
	
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if(bMainLandLevel)
	{
		
		touchEffect = ParticleManager::Get()->createEffectSprite(204,"",false);
		this->m_pObjectsLayer->addChild(touchEffect,  MAP_Z_TILE_BACKGROUND);
	}
	

	return true;
}

void LevelLayer::onlyCreateMonsters()
{
	if (m_levelBuilder)
	{
		m_levelBuilder->createMonster();
	}
}

SpriteTransPoint* LevelLayer::getMainLandTranspoint()
{
	return m_levelBuilder->GetMainLandTransPointInstance();
}

SpriteTransPoint* LevelLayer::getEliteTranspoint()
{
	return m_levelBuilder->GetEliteTransPointInstance();
}
//////////////////////////////////////////////////////////////////////////
//get kind objects
//////////////////////////////////////////////////////////////////////////
std::vector<CCNode*> LevelLayer::getChildrenByTag(int tag)
{
	std::vector<CCNode*> result;

	CCArray* children = this->getChildren();
	for (size_t i = 0; i < children->count(); ++i)
	{
		CCNode* node = (CCNode*)children->objectAtIndex(i);
		if(node && node->getTag() == tag)
		{
			result.push_back(node);
		}
	}

	if(m_pObjectsLayer != NULL)
	{
		children = m_pObjectsLayer->getChildren();
		for (size_t i = 0; children && i < children->count(); ++i)
		{
			CCNode* node = (CCNode*)children->objectAtIndex(i);
			if(node && node->getTag() == tag)
			{
				result.push_back(node);
			}
		}
	
	}
	return result;
}


//////////////////////////////////////////////////////////////////////////
//get kind objects count
//////////////////////////////////////////////////////////////////////////
int LevelLayer::getObjectCount(int tag)
{
	//std::vector<CCNode*> result;
	int count = 0;

	CCArray* children = this->getChildren();
	for (size_t i = 0; i < children->count(); ++i)
	{
		CCNode* node = (CCNode*)children->objectAtIndex(i);
		if(node && node->getTag() == tag)
		{
			//result.push_back(node);
			count++;
		}
	}

	if(m_pObjectsLayer != NULL)
	{
		children = m_pObjectsLayer->getChildren();
		for (size_t i = 0; i < children->count(); ++i)
		{
			CCNode* node = (CCNode*)children->objectAtIndex(i);
			if(node && node->getTag() == tag)
			{
				//result.push_back(node);
				count++;
			}
		}
	}

	return count;
	//return result.size();
}


//////////////////////////////////////////////////////////////////////////
//seer on map
//////////////////////////////////////////////////////////////////////////
SpriteSeer* LevelLayer::getSpriteSeer()
{
	std::vector<CCNode*> children = getChildrenByTag(MAP_TAG_SEER);
	if (children.size() > 0)
	{
		ASSERT(children.size() == 1, "more than 1 seer created");
		return (SpriteSeer*)children[0];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//follow sprite on map
//////////////////////////////////////////////////////////////////////////
SpriteElf* LevelLayer::getFollowSprite()
{
	std::vector<CCNode*> children = getChildrenByTag(MAP_TAG_FOLLOW_SPRITE);
	if (children.size() > 0)
	{
		ASSERT(children.size() == 1, "more than 1 follow sprite created");
		return (SpriteElf*)children[0];
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//level id
//////////////////////////////////////////////////////////////////////////
int LevelLayer::getID()
{
	return m_id;
}

//////////////////////////////////////////////////////////////////////////
//update movable objects z order 
//////////////////////////////////////////////////////////////////////////
void LevelLayer::update(float dt)
{   
	for(std::map<PLAYER_ID, WAKL_INFO>::iterator iter = m_walkInfo.begin(); iter != m_walkInfo.end(); iter++)
	{
		if(iter->second.idleTime.time <= iter->second.idleTime.curTime && !iter->second.walk.bUsed)
		{
			iter->second.walk.bUsed = true;
			updateOtherPlayer(iter->second.walk.uid, iter->second.walk.pt, iter->second.walk.animID, iter->second.walk.bFlip);
		}
		else
		{
			iter->second.idleTime.curTime += dt;
			
		}
	}

	while(!LevelManager::sShareInstance()->m_otherPlayerInfo.empty())
	{
		const LevelManager::OTHER_PLAYER_INFO& info = LevelManager::sShareInstance()->m_otherPlayerInfo.top();

		m_levelBuilder->addOtherPlayer(info.uid, info.userName.c_str(), info.type, info.pos, info.orient, info.battleSide);
		LevelManager::sShareInstance()->m_otherPlayerInfo.pop();
	}

	CCArray* children = this->getChildren();
	if (children)
	{
		int childrenCount = children->count();
		for (int i = 0; i < childrenCount; ++i)
		{
			CCNode* child = (CCNode* )children->objectAtIndex(i);
			ASSERT(child != NULL, "child is NULL");

			BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
			if (listener)
			{
				listener->HandleLayerUpdateEvent(child, dt);
			}			
		}
	}

	if(m_pObjectsLayer != NULL)
	{
		children = m_pObjectsLayer->getChildren();
		if (children)
		{
			int childrenCount = children->count();
			for (int i = 0; i < childrenCount; ++i)
			{
				CCNode* child = (CCNode* )children->objectAtIndex(i);
				ASSERT(child != NULL, "child is NULL");

				BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
				if (listener)
				{
					listener->HandleLayerUpdateEvent(child, dt);
				}				
			}
		}
	}

	// update Hero 
	SpriteSeer* hero = GameManager::Get()->getHero();
	if (hero)
	{
		hero->Update(dt);
	}

	//更新其他玩家
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_othersPlayerInfo.begin();
	while (itor != m_othersPlayerInfo.end())
	{
		OthersPlayerInfo playerInfo = (*itor).second;

		SpriteSeer * seer = playerInfo.seer;
		if (seer)
		{
			seer->Update(dt);
		}		

		itor++;
	}

	// update ItemUpdatManager
	ItemUpdateManager::Get()->Update(dt);

	// update monster
	SpriteMonsterMgr::Get()->Update(dt);

	// update actor
	EnginePlayerManager::getInstance()->update(dt);

	// Note: Update SpriteElf
	SpriteElfManager::Get()->Update(dt);

	// Note: 剧情Update
	StoryInstanceDirector::Get()->Update(dt);

	// Note: 教程Update
	TutorialsManager::Get()->Update(dt);

	WaitingLayerManager::Get()->update(dt);

	//按住屏幕移动持续寻路
	if (pressTime < 0.1f)
	{
		pressTime += dt;
	}
	else if (m_touchWinPoint.x != 0 && m_touchWinPoint.y != 0)
	{
		float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
		m_touchWinPoint = CCPoint(m_touchWinPoint.x * deviceScale, m_touchWinPoint.y * deviceScale);
    
		CCPoint newPT = LevelMultiResolution::sTransformWindowPointToMap(m_touchWinPoint);

		SpriteSeer * hero = GameManager::Get()->getHero();
		if (hero)
		{
			BaseListener* listener = LevelLayer::sGetListenerByTag(hero->getTag());
			if (listener)
			{
				listener->HandleLayerTouchBeginEvent(hero, newPT);
			}			
		}
		pressTime = 0.0f;
	}

	SkillDispMgr::Get()->Update(dt);

	PvAIManager::Get()->Update(dt);

	ParticleManager::Get()->Update(dt);

	GuildInstanceManager::Get()->Update(dt);

	//m_cameraController.Update(dt);
	CameraController::sharedCameraController()->Update(dt);
}


//////////////////////////////////////////////////////////////////////////
//rendering
//////////////////////////////////////////////////////////////////////////
void LevelLayer::visit()
{
	CCDirector::sharedDirector()->setRenderCameraForLevel();
	CCLayer::visit();
	CCDirector::sharedDirector()->setRenderCameraForUI();
}

//////////////////////////////////////////////////////////////////////////
//called when touch layer
//////////////////////////////////////////////////////////////////////////
void LevelLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	CCPoint oldPT = touch->getLocationInView();
	oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);	

	m_touchWinPoint = oldPT;

    float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
    oldPT = CCPoint(oldPT.x * deviceScale, oldPT.y * deviceScale);
    
	CCPoint newPT = LevelMultiResolution::sTransformWindowPointToMap(oldPT);

	// Note: 打断自动寻路到副本显示感叹号
	TaskManager::getInstance()->InterruptAutoGoToInstanceEvent();

	//上面的listener未命中，直接调用SpriteSeerListener
	SpriteSeer * seer = GameManager::Get()->getHero();
	if (seer)
	{
		BaseListener* listener = LevelLayer::sGetListenerByTag(seer->getTag());
		if (listener)
		{
			listener->HandleLayerTouchBeginEvent(seer, newPT);
		}		
	}

	//record touch point in map
	m_touchWinPoint = oldPT;		
}


//////////////////////////////////////////////////////////////////////////
//called when touch move layer
//////////////////////////////////////////////////////////////////////////
void LevelLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	CCPoint oldPT = touch->getLocationInView();
	oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);	

	m_touchWinPoint = oldPT;
	
}


//////////////////////////////////////////////////////////////////////////
//called when touch end
//////////////////////////////////////////////////////////////////////////
void LevelLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	CCPoint oldPT = touch->getLocationInView();
	oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);	

	CCPoint newPT = LevelMultiResolution::sTransformWindowPointToMap(oldPT);

	//send touch end event to children
	CCArray* children = this->getChildren();
	if (children)
	{
		int childrenCount = children->count();
		for (int i = 0; i < childrenCount; ++i)
		{
			CCNode* child = (CCNode* )children->objectAtIndex(i);
			ASSERT(child != NULL, "child is NULL");

			BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
			if (listener)
			{
				listener->HandleLayerTouchEndEvent(child, newPT);
			}			
		}
	}

	//reset touch point
	m_touchWinPoint = ccp(0, 0);
}


//////////////////////////////////////////////////////////////////////////
//called when level active
//////////////////////////////////////////////////////////////////////////
void LevelLayer::onActiveLayer(sActiveLevelConfig& config)
{
	//
	this->setTouchEnabled(true);
	//
	CCDirector::sharedDirector()->setLevelRenderCameraOffset(ccp(0,0));

	//send active event to children
	CCArray* children = this->getChildren();
	if (children)
	{
		int childrenCount = children->count();
		for (int i = 0; i < childrenCount; ++i)
		{
			CCNode* child = (CCNode* )children->objectAtIndex(i);
			ASSERT(child != NULL, "child is NULL");

			BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
			if (listener)
			{
				listener->HandleLayerActiveEvent(child, &config);
			}			
		}
	}

	if(m_pObjectsLayer != NULL)
	{
		/*CCNode* pNode = getSpriteSeer();
		if(pNode != NULL)
		{
		BaseListener* listener = LevelLayer::sGetListenerByTag(pNode->getTag());
		listener->HandleLayerActiveEvent(pNode, &config);
		}*/

		CCArray* children = m_pObjectsLayer->getChildren();
		if (children)
		{
			int childrenCount = children->count();
			for (int i = 0; i < childrenCount; ++i)
			{
				CCNode* child = (CCNode* )children->objectAtIndex(i);
				ASSERT(child != NULL, "child is NULL");

				BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
				if (listener)
				{
					listener->HandleLayerActiveEvent(child, &config);
				}				
			}
		}

	}

	//get other players
	//OnlineNetworkManager::sShareInstance()->sendGetOtherPlayersMessage();
}

//////////////////////////////////////////////////////////////////////////
//called when level de-active
//////////////////////////////////////////////////////////////////////////
void LevelLayer::onDeactiveLayer()
{
	this->setTouchEnabled(false);	

	//send de-active event to children
	CCArray* children = this->getChildren();
	if (children)
	{
		int childrenCount = children->count();
		for (int i = 0; i < childrenCount; ++i)
		{
			CCNode* child = (CCNode* )children->objectAtIndex(i);
			ASSERT(child != NULL, "child is NULL");

			BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
			if (listener)
			{
				listener->HandleLayerDeactiveEvent(child);
			}			
		}
	}	

	//send de-active event to children
	if(getObjectLayer())
	{
		CCArray* children = this->getObjectLayer()->getChildren();
		if (children)
		{
			int childrenCount = children->count();
			for (int i = 0; i < childrenCount; ++i)
			{
				CCNode* child = (CCNode* )children->objectAtIndex(i);
				ASSERT(child != NULL, "child is NULL");

				BaseListener* listener = LevelLayer::sGetListenerByTag(child->getTag());
				if (listener)
				{
					listener->HandleLayerDeactiveEvent(child);
				}				
			}
		}	
	}
	
}

//////////////////////////////////////////////////////////////////////////
//called when layer destroy
//////////////////////////////////////////////////////////////////////////
void LevelLayer::destroyLayer()
{
	onDeactiveLayer();	

	CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);

	if (this->getParent() != NULL)
	{
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		this->removeAllChildrenWithCleanup(true);
	}	

	m_mineInfoList.clear();

	SAFE_DEL(m_pPhysicsLayer);
}


//////////////////////////////////////////////////////////////////////////
//calculate z based on position in world
//////////////////////////////////////////////////////////////////////////
int LevelLayer::sCalcZorder(CCPoint pos)
{
	//CCLOG("pos.y : %f", pos.y);
	int dynamicZ = MAP_Z_DYNAMIC - pos.y * 10;
	//CCAssert(dynamicZ <= MAP_Z_DYNAMIC_END);
	return dynamicZ;
}

float LevelLayer::sCalcDepth( unsigned int z )
{
	// 换成相机坐标系 注意 CCDirector::setProjection() , CCDirector::setRenderCameraForLevel()
	int nFar = 0 + (int)CCDirector::sharedDirector()->getZEye();
	int nNear = -65535 + (int)CCDirector::sharedDirector()->getZEye();

	// 映射到相机坐标系下的Z区间
	float fz = (float)z / (float)MAP_Z_MAX;
	float nZ = fz * (float)(nFar - nNear) * 0.6f + (float)nNear + (float)(nFar - nNear) * 0.2f;
	//CCLOG("zOrder : %u , nz : %f, D : %ld", z, nZ, nFar - nNear);
	return (float)nZ;

}


//////////////////////////////////////////////////////////////////////////
//get listener by tag
//////////////////////////////////////////////////////////////////////////
BaseListener* LevelLayer::sGetListenerByTag(int tag)
{	
	if (tag == MAP_TAG_SEER)
	{
		return SpriteSeerListener::sShareInstance();
	}
	else if (tag == MAP_TAG_OTHER_SEER)
	{
		return SpriteOtherSeerListener::sShareInstance();
	}

	else if (tag == MAP_TAG_TRANSPOINT)
	{
		return SpriteTranspointListener::sShareInstance();
	}
	else if (tag == MAP_TAG_NPC)
	{
		return BaseListener::sShareInstance();
	}

	//add more here

	//return BaseListener::sShareInstance();	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//random  walkable point at map
//////////////////////////////////////////////////////////////////////////

CCPoint LevelLayer::getRandomPoint()
{
	return m_pPhysicsLayer->getRandomPoint();
}


CCPoint LevelLayer::getSeerBornPoint(sActiveLevelConfig& config)
{
	int x = config.pos.x;
	int y = config.pos.y;
	if(m_pPhysicsLayer->isPointReachable(ccp(x, y)))
	{
		return config.pos;
	}

	if(m_bornPointList.size() > 0)
	{

		if(m_pPhysicsLayer->isPointReachable(m_bornPointList[0].point))
		{
			return m_bornPointList[0].point;
		}
	}


	return getRandomPoint();
}

//////////////////////////////////////////////////////////////////////////
//scroll map when touch the map edge
//////////////////////////////////////////////////////////////////////////
void LevelLayer::handleMapScrollEvent(CCPoint winPT)
{
    ///	don't support scroll map by hand
	CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCSize mapSize = getMapSizeInPixel();

	CCPoint curCameraOffset = CCDirector::sharedDirector()->getLevelRenderCameraOffset();	
	CCPoint curCameraPos = CCDirector::sharedDirector()->getLevelRenderCameraPos();

	//
	CCPoint deltaPos = ccpSub(winPT, m_touchWinPoint);
	float offsetX = deltaPos.x;
	float offsetY = deltaPos.y;

	curCameraOffset.x -= offsetX;
	curCameraOffset.y -= offsetY;		

	//
	curCameraOffset.x = clampf(curCameraOffset.x, -(mapSize.width - winSize.width) * 0.5, (mapSize.width - winSize.width) * 0.5);
	curCameraOffset.y = clampf(curCameraOffset.y, -(mapSize.height - winSize.height) * 0.5, (mapSize.height - winSize.height) * 0.5);
	

	CCDirector::sharedDirector()->setLevelRenderCameraOffset(curCameraOffset);	
}


//------------------------------------------------------------------------
//		add/remove/update other player
//------------------------------------------------------------------------
void LevelLayer::addOtherPlayer(PLAYER_ID uid, const char* userName,unsigned int petType, cocos2d::CCPoint pt, unsigned int orient, unsigned int battleSide)
{
	m_levelBuilder->addOtherPlayer(uid, userName, petType, pt, orient, battleSide);

	OnlineNetworkManager::sShareInstance()->sendReqOneHeroElfMessage(uid,userName);

    // 检查当前地图是否为世界地图
    // 如果是世界地图，就不需要去请求别的玩家的技能信息
    bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
    if (! bMainLandLevel)
    {
        OnlineNetworkManager::sShareInstance()->sendGetUserSkillReqMessage(uid);
    }

	// Note: 拉取玩家的武器信息
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Friend/OtherPlayerWeapon.lua","reqPlayerWeapon",uid.getID(),uid.getChannel(),uid.getRegTime());
}

void LevelLayer::removeOtherPlayer(PLAYER_ID uid)
{
	m_levelBuilder->removeOtherPlayer(uid);
}
void LevelLayer::removeAllOtherPlayer()
{
	m_levelBuilder->removeAllOtherPlayer();
}
SpriteSeer* LevelLayer::getPlayer(PLAYER_ID uid)
{
	SpriteSeer * seer = m_levelBuilder->getPlayer(uid);
	return seer;
}

void LevelLayer::EquipOnePlayerWeapon(int playerId,int weaponId)
{
	std::map<PLAYER_ID, OthersPlayerInfo>& otherPlayer = m_othersPlayerInfo;
	for(std::map<PLAYER_ID, OthersPlayerInfo>::const_iterator iter = otherPlayer.begin(); iter != otherPlayer.end(); iter++)
	{
		if ((*iter).first.getID() == playerId)
		{
			BoneSpriteBase* pOtherHero = dynamic_cast<BoneSpriteBase*>(iter->second.seer);
			pOtherHero->SetEquipItemsData(weaponId);
			return ;
		}				
	}
}

//------------------------------------------------------------------------
//		add/remove/update monsters
//------------------------------------------------------------------------
void LevelLayer::addMonster(MonsterInitInfo info)
{
	m_levelBuilder->addMonster(info);
}

void LevelLayer::removeMonster(unsigned int uid)
{
	m_levelBuilder->removeMonster(uid);
}

void LevelLayer::ShowMonsterOrNot(bool bShow)
{
	m_levelBuilder->showMonsterOrNot(bShow);
}

void LevelLayer::updateOtherPlayer(PLAYER_ID uid, cocos2d::CCPoint pt, unsigned int animID, bool bFlip)
{
	if(UserData::Get()->getUserId() == uid)
	{
		return;
	}
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_othersPlayerInfo.find(uid);
	if (itor != m_othersPlayerInfo.end())
	{
		OthersPlayerInfo playerInfo = (*itor).second;

		//update seer
		SpriteSeer* seer = playerInfo.seer;
		if (seer)
		{
			SpriteOtherSeerListener::sShareInstance()->HandleNetWorkUpdate(seer, uid, pt, animID, bFlip);
		}
		else
		{
			CCLOG("error, seer is null");
		}
	}
	else
	{
		CCLOG("can't find the player id %d", uid.getID());
	}
}


void LevelLayer::DoOtherPlayerSkill( PLAYER_ID uid, unsigned int skillID , const CCPoint& rolePos,  const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster)
{
	if(UserData::Get()->getUserId() == uid)
	{
		return;
	}
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_othersPlayerInfo.find(uid);
	if (itor != m_othersPlayerInfo.end())
	{
		OthersPlayerInfo playerInfo = (*itor).second;

		//update seer
		OtherHeroFight* pOtherHero = dynamic_cast<OtherHeroFight*>(playerInfo.seer);
		if (pOtherHero)
		{
			const SKILL_INFO* pSkillInfo = SkillDispMgr::Get()->GetSkillInfo(skillID);
			if(pSkillInfo)
			{
				if(pSkillInfo->type == ToNormalAttack)
					pOtherHero->DoNormalAttack(skillID, rolePos, roleDir, aimDir, monsterPos, bAnimMonster );
				else
					pOtherHero->DoSkillAttack(skillID, rolePos, roleDir, aimDir, monsterPos, bAnimMonster );
				
			}
		}
		else
		{
			CCLOG("error, seer is null");
		}
	}
	else
	{
		CCLOG("can't find the player id %d", uid.getID());
	}
}

void LevelLayer::DoElfSkill( PLAYER_ID uid, unsigned int skillID , const CCPoint& rolePos,  const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster,unsigned int pos)
{
	if(UserData::Get()->getUserId() == uid)
	{
		return;
	}
	std::map<PLAYER_ID, OthersPlayerInfo>::iterator itor = m_othersPlayerInfo.find(uid);
	if (itor != m_othersPlayerInfo.end())
	{
		OthersPlayerInfo playerInfo = (*itor).second;

		if (playerInfo.seer)
		{
			SpriteElf* pSpriteElf = SpriteElfManager::Get()->GetOneInstanceFromPos(uid ,pos);
			//const std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >& elfMap = SpriteElfManager::Get()->GetAllInstance();
			//const std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator  playerElfMap = elfMap.find(uid);


			/*for(std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::const_iterator iter = elfMap.begin(); iter != elfMap.end(); iter++)
			{
			bool bFind = false;
			for (std::map<unsigned int,SpriteElf*>::const_iterator elfIter = (*iter).second.begin();
			elfIter != (*iter).second.end(); elfIter++)
			{
			if(elfIter->second->GetAttackedHero() == playerInfo.seer)
			{
			pSpriteElf = elfIter->second;
			bFind = true;
			break;
			}
			}
			if (bFind)
			{
			break;
			}
			}*/

			if(pSpriteElf)
			{
				
				pSpriteElf->DoSkillAttack(skillID, rolePos, roleDir, aimDir, monsterPos, bAnimMonster );
			}

		}
		else
		{
			CCLOG("error, seer is null");
		}
	}
	else
	{
		CCLOG("can't find the player id %d", uid.getID());
	}
}


//////////////////////////////////////////////////////////////////////////
//Debug draw move path
//////////////////////////////////////////////////////////////////////////
void LevelLayer::debugDrawPath()
{	
#ifdef COCOS2D_DEBUG		 
		
	int row = REF_LEVEL_HEIGHT / CELL_W;
	int col = REF_LEVEL_WIDTH / CELL_W;	

	//int offset = m_pathOffset;
	
	for (int y=0; y<row; y++)
	{
		for (int x=0; x<col; x++) 
		{
			if(m_pPhysicsLayer->isPointReachable(ccp(x, y)) )
			{
				CCSprite *sp = CCSprite::create("UI/Icon-Small@2x.png", CCRectMake(0,0, CELL_W, CELL_W));
				sp->setAnchorPoint(ccp(0,0));
				sp->setPosition(LevelMultiResolution::sTransformPointToMap(ccp(x*CELL_W, y*CELL_W)));
				sp->setScale(LevelMultiResolution::sGetLevelScale());

				sp->setOpacity(100);
				this->addChild(sp,10);
			}		
		}
	}  

#endif
}

cocos2d::CCSize LevelLayer::getMapSizeInPixel( void )
{
	return cocos2d::CCSize(m_levelDataReference->cellWidth * m_levelDataReference->col, m_levelDataReference->cellHeight * m_levelDataReference->row);
}

cocos2d::CCSize LevelLayer::getMapTileSizeInPixel( void )
{
	return cocos2d::CCSize(m_levelDataReference->cellWidth, m_levelDataReference->cellHeight);
}

cocos2d::CCRect LevelLayer::getMapRect( void )
{
	CCSize size = getMapSizeInPixel();
	return CCRect(0.0f, 0.0f, size.width, size.height);
}

void  LevelLayer::showTouchEffect(CCPoint point)
{
	touchEffect->SetAnim(kType_Play,1,false);
	touchEffect->setPosition(point);
}


//////////////////////////////////////////////////////////////////////////