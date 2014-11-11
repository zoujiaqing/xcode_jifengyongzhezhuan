#include "ParticleManager.h"
#include "HeroParticleListener.h"
#include "GameResourceManager.h"
#include "UILabelAtlas.h"
#include "UIManager.h"
#include "GameManager.h"
#include "CConnectionData.h"
#include "EnginePlayerManager.h"
#include "MainMenuLayer.h"
#include "BattleUILayer.h"
#include "LevelManager.h"
#include "LevelLayer.h"
#include "LuaTinkerManager.h"
using namespace TXGUI;

#define EFFECT_DIR "effect/"
static ParticleManager *g_sParticleMgr = NULL;

ParticleManager::ParticleManager()
{
	m_currentTarget.setPlayId(0,0,0);
	mEffectIndex = 0;
	HeroParticleListener::Create();
	mMaxRcsID = mMinRcsID = 0;			
}

void ParticleManager::GetEffectRcsIDRange(unsigned int &min,unsigned int &max)
{
	if (mMinRcsID == 0)
	{
		mMinRcsID = LuaTinkerManager::Get()->callLuaFunc<int>("Script/EffectDataConfig.lua", "GetEffectAnimRcsMinID");		
	}
	min = mMinRcsID;
	if (mMaxRcsID == 0)
	{
		mMaxRcsID = LuaTinkerManager::Get()->callLuaFunc<int>("Script/EffectDataConfig.lua", "GetEffectAnimRcsMaxID");		
	}
	max = mMaxRcsID;
}

bool ParticleManager::TellIsEffectByRcsID(unsigned int id)
{
	unsigned int min = 0;
	unsigned int max = 0;
	GetEffectRcsIDRange(min,max);
	if (id >= min && id <= max)
	{
		return true;
	}
	return false;
}

ParticleManager::~ParticleManager()
{
	mParticleList.clear();
	HeroParticleListener::Destroy();
}

AParticleNode* ParticleManager::CreateParticleNode(EffectType type)
{
	AParticleNode* particle = AParticleNode::CreateParticleNode(GetParticlePlist(type),++mEffectIndex);
	AddParticleNodeToList(mEffectIndex,particle);
	return particle; 
}

void ParticleManager::AddParticleNodeToList(unsigned int index,AParticleNode* particle)
{
	std::map<unsigned int, AParticleNode*>::iterator itor = mParticleList.find(index);
	if (itor == mParticleList.end())
	{
		mParticleList.insert(std::pair<unsigned int, AParticleNode*>(index,particle));
	}
}

void ParticleManager::HandleParticleRemoved(unsigned int index)
{
	std::map<unsigned int, AParticleNode*>::iterator itor = mParticleList.find(index);
	if (itor != mParticleList.end())
	{
		mParticleList.erase(itor);
	}
}

char* ParticleManager::GetParticlePlist(EffectType type)
{
	char* plist = NULL;
	switch(type)
	{
		//case kSnowParticle:
		//	plist = "snowTest.plist";
		//	break;
		case NormalAttackEffect:
			break;
		//case kSmokeOne:
		//	plist = "smoke_01.plist";
		//	break;
	}
	return plist;
}

void ParticleManager::DumpParticlesInfor()
{
	std::map<unsigned int, AParticleNode*>::iterator itor = mParticleList.begin();
	if (itor != mParticleList.end())
	{
		CCLOG("Particle: totalNum: %ld, texture wide: %ld, high: %d, count: %d",
			itor->second->GetParticle()->getTotalParticles(),
			itor->second->GetParticle()->getTexture()->getPixelsWide(),
			itor->second->GetParticle()->getTexture()->getPixelsHigh());
		itor++;
	}
}

EffectSprite* ParticleManager::createEffectSprite(int type,const char *name,bool isNeedRegister)
{
	EffectSprite *pBoneAnimSprite = EffectSprite::effectWithID(type,name);
	if(isNeedRegister)
	{
		EnginePlayerManager::getInstance()->addEffectAsprite(pBoneAnimSprite);
	}
	return pBoneAnimSprite;
}

CCNode * ParticleManager::createEffectSpriteAndPlay(int type,const char *name,int animId, int nloop, bool isLoop)
{
	EffectSprite * m_effect = createEffectSprite(type, name);
  
    m_effect->SetAnim(animId, nloop, isLoop);
    return (CCNode *)m_effect;
}

EffectType ParticleManager::getEffectType(const char* actionTypeStr)
{
	EffectType type = EUnkownEffect;
	if(0 == strcmp(actionTypeStr,"kTypeNormalAttacked"))
	{
		type = NormalAttackEffect;
	}
	else if(0 == strcmp(actionTypeStr,"kTypeMonsterAttacked"))
	{
		type = MonsterAttackEffect;
	}
	else if(0 == strcmp(actionTypeStr,"kTypeCritAttacked"))
	{
		type = CritAttackEffect;
	}
	else if(0 == strcmp(actionTypeStr,"kTypeNormalTranspoint"))
	{
		type = TranspointEffect;
	}
	return type;
}

void ParticleManager::SetEnemyHpLable(float currentHp,PLAYER_ID targetMonster,unsigned int type,unsigned int enemyType,bool isSelf)
{
	bool isPlayAni = true;
	if(!isSelf)
	{
		if(targetMonster != m_currentTarget || m_currentTarget.getID() == 0)
		{
			// // 其它人砍非当前目标，不处理
			return;
		}
	}
	if(targetMonster != m_currentTarget)
	{
		isPlayAni = false;
		m_currentTarget = targetMonster;
	}
	SceneLayer* layer = GameManager::Get()->GetSceneLayer();
	if(layer)
	{
		MainMenuLayer* uiLayer = layer->getMainMenuLayer();
		if(uiLayer)
		{
			int battleTag = uiLayer->getLayerTagByName("BattleUILayer");
			BattleUILayer* battleLayer = uiLayer->findUILayerByTag<BattleUILayer>(battleTag);
			if(battleLayer)
			{
				battleLayer->SetEnemyHpLable(currentHp,isPlayAni,targetMonster,type,enemyType);
			}
		}
	}
}

EffectSprite * ParticleManager::playEffectByInfoMonster(SkillInfo info, SpriteMonster * monster)
{
	EffectSprite * effectNode = playEffectByInfo(info);
	if (!effectNode)
	{
		return NULL;
	}
	if (monsterEffectMap.find(monster) != monsterEffectMap.end())
	{
		if (monsterEffectMap[monster].find(effectNode) == monsterEffectMap[monster].end())
		{
			monsterEffectMap[monster].insert(effectNode);
		}
		else
		{
			ASSERT(0, "reused effect , find why");
		}
		
	}
	else
	{
		set<EffectSprite *> effectSet;
		effectSet.insert(effectNode);
		monsterEffectMap[monster] = effectSet;
	}

	//弓箭手特效动态更新层级
	if (info.type == ATTACK_TYPE_FAR)
	{
		if (dynZOrderMap.find(effectNode) == dynZOrderMap.end())
		{
			dynZOrderMap.insert(effectNode);
		}
	}
	return NULL;
}

bool ParticleManager::stopEffectOfMonster(SpriteMonster * monster)
{
	bool ret = false;
    
	if (monsterEffectMap.find(monster) != monsterEffectMap.end())
	{
		set<EffectSprite *>::iterator iter = monsterEffectMap[monster].begin();
		for (; iter != monsterEffectMap[monster].end(); )
		{
			if (!(*iter)->isVisible())
			{
				ccCallRecycleEffect(*iter++);
			}
			else
			{
				iter++;
			}
		}
		ret = true;
	}
	return ret;
}


EffectSprite * ParticleManager::playEffectByInfo(SkillInfo info)
{
	EffectSprite * effectNode = NULL;
	switch (info.type)
	{
	case ATTACK_TYPE_STAND:
	case ATTACK_TYPE_CONTINUE:
		effectNode = playOneTimeEffect(info.effectId, info.endPosition, info.delayTime ,info.playType);
		break;

	case ATTACK_TYPE_FAR:
		effectNode = playArcherEffect(info.effectId, info.startPosition, info.endPosition, info.speed * 16, info.delayTime, info.playType);
	default:
		break;
	}	
	return effectNode;
}

EffectSprite * ParticleManager::playOneTimeEffect(int effectId, CCPoint position, float delayTime, int playType)
{
	EffectSprite * effectNode = getFreeEffectNode(effectId);
	if (!effectNode)
	{
		return NULL;
	}

	effectNode->setPreType((RoleActionType)(playType));

	CCPoint zPos = position;
	//需要显示在任务后面的特效（贴地特效）
	bool backEffect = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "BackEffect", effectId);
	if (backEffect)
	{
		zPos = ccpAdd(position, ccp(0, 70));
	}

	//使用前设置标志
	effectNodeMap[effectId][effectNode] = true;

	CCArray *arrayOfActions = CCArray::create();
	CCDelayTime * delayEffect = CCDelayTime::create(delayTime / 1000);
	CCCallFuncN *showFunc = CCCallFuncN::create(this, callfuncN_selector(ParticleManager::ccCallShowOneTimeEffect));
	CCDelayTime * delayRecycle = CCDelayTime::create(5.0f);
	CCCallFuncN *recycleFunc = CCCallFuncN::create(this, callfuncN_selector(ParticleManager::ccCallRecycleEffect));

	arrayOfActions->addObject(delayEffect);
	arrayOfActions->addObject(showFunc);
	arrayOfActions->addObject(delayRecycle);
	arrayOfActions->addObject(recycleFunc);
	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	effectNode->setPosition(position);
	effectNode->setZOrder(LevelLayer::sCalcZorder(zPos));

	effectNode->stopAllActions();
	effectNode->runAction(sequence);

	return effectNode;
}

EffectSprite * ParticleManager::playArcherEffect(int effectId, CCPoint startPosition, CCPoint endPosition, float speed, float delayTime, int playType)
{
	EffectSprite * effectNode = getFreeEffectNode(effectId);
	if (!effectNode)
	{
		return NULL;
	}

	effectNode->setPreType((RoleActionType)(playType));

	effectNodeMap[effectId][effectNode] = true;

	float distance = ccpDistance(startPosition, endPosition);
	CCPoint directVec = ccpSub(endPosition, startPosition);

	float degree;
	degree = atan(directVec.x / directVec.y) * 180 / 3.14;
	if (directVec.y < 0)
	{
		degree += 180;
	}

	CCPoint startCell = LevelManager::sShareInstance()->pointPixelToCell(startPosition);
	CCPoint endCell = LevelManager::sShareInstance()->pointPixelToCell(endPosition);


	effectNode->getRoot()->setRotation(degree);
	effectNode->setPosition(startPosition);	
	
	CCArray *arrayOfActions = CCArray::create();

	CCDelayTime * delayEffect = CCDelayTime::create(delayTime / 1000);
	CCCallFuncN *showEffect = CCCallFuncN::create(this, callfuncN_selector(ParticleManager::ccCallShowContinueEffect));
	CCMoveTo * moveEffect = CCMoveTo::create(distance / speed, endPosition);	
	CCHide * hideEffect = CCHide::create();
	CCCallFuncN *recycleFunc = CCCallFuncN::create(this, callfuncN_selector(ParticleManager::ccCallRecycleEffect));

	arrayOfActions->addObject(delayEffect);
	arrayOfActions->addObject(showEffect);
	arrayOfActions->addObject(moveEffect);
	arrayOfActions->addObject(hideEffect);
	arrayOfActions->addObject(recycleFunc);
	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	effectNode->stopAllActions();
	effectNode->runAction(sequence);

	return effectNode;
}

EffectSprite * ParticleManager::getFreeEffectNode(int effectId)
{
	EffectSprite * effectNode = NULL;
	LevelLayer * layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (!layer || !layer->getObjectLayer())
	{
		return NULL;
	}
    
	if (effectNodeMap.find(effectId) != effectNodeMap.end())
	{
		map<EffectSprite *, bool> spriteMap = effectNodeMap[effectId];
		map<EffectSprite *, bool>::iterator iter = spriteMap.begin();
		for (; iter != spriteMap.end(); iter++)
		{
			if (iter->second == false)
			{
				effectNode = iter->first;
				break;
			}
		}

		if (!effectNode)
		{
			effectNode = createEffectSprite(effectId,"");

			layer->getObjectLayer()->addChild(effectNode);
			spriteMap.insert(make_pair<EffectSprite *, bool>(effectNode, false));
			effectNodeMap[effectId] = spriteMap;
		}
	}
	else
	{
		map<EffectSprite *, bool> spriteMap;
		effectNode = createEffectSprite(effectId,"");

		layer->getObjectLayer()->addChild(effectNode);
		spriteMap.insert(make_pair<EffectSprite *, bool>(effectNode, false));

		effectNodeMap[effectId] = spriteMap;
	}

	//ASSERT(effectNode, "创建特效失败");

	return effectNode;
}

void ParticleManager::ccCallShowOneTimeEffect(CCNode * node)
{
	EffectSprite * effectNode = (EffectSprite *)node;
	effectNode->SetAnim(effectNode->getPreType(), 1, false);
	effectNode->setVisible(true);
}

void ParticleManager::ccCallShowContinueEffect(CCNode * node)
{
	EffectSprite * effectNode = (EffectSprite *)node;
	effectNode->SetAnim(effectNode->getPreType(), 1, true);
	effectNode->setVisible(true);
}

void ParticleManager::ccCallRecycleEffect(CCNode * node)
{
	EffectSprite * effectNode = (EffectSprite *)node;

	effectNode->stopAllActions();

	int effectId = effectNode->GetTypeId();
	//设置可重用
	if (effectNodeMap.find(effectId) != effectNodeMap.end())
	{
		if (effectNodeMap[effectId].find(effectNode) != effectNodeMap[effectId].end())
		{
			effectNodeMap[effectId][effectNode] = false;
		}
	}

	//从怪物特效表中删除
	map<SpriteMonster*, set<EffectSprite*> >::iterator iter = monsterEffectMap.begin();
	for (; iter != monsterEffectMap.end(); iter++)
	{
		if (iter->second.find(effectNode) != iter->second.end())
		{
			CCLOG("Stop monster effect.........");
			iter->second.erase(effectNode);
			break;
		}
	}
}

void ParticleManager::Update(float dt)
{
	LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (layer)
	{
		set<EffectSprite *>::iterator iter = dynZOrderMap.begin();
		for (; iter != dynZOrderMap.end(); iter++)
		{
			if ((*iter)->isVisible())
			{
				layer->getObjectLayer()->reorderChild(*iter, LevelLayer::sCalcZorder((*iter)->getPosition()));
			}
		}
	}

	// Note: 特效时间管理
	for (std::map<unsigned int ,ParticleConditionData>::iterator iter = ParticleConditionDatas.begin();
		 iter != ParticleConditionDatas.end(); iter++)
	{
		if ((*iter).second.attachParticleNodes.size() > 0)
		{
			for (std::map<CCNode*,float>::iterator _iter = (*iter).second.attachParticleNodes.begin();
				_iter != (*iter).second.attachParticleNodes.end(); )
			{
				(*_iter).second += dt;
				if (((*_iter).second) >= (*iter).second.lifeTime)
				{
					if ((*iter).second.playedNum > 0)
					{
						(*iter).second.playedNum --;
					}
					(*iter).second.attachParticleNodes.erase(_iter++);
				}
				else
				{
					_iter++;
				}				
			}
		}
	}
}

void ParticleManager::clearEffectBuff()
{
	effectNodeMap.clear();
	monsterEffectMap.clear();
	dynZOrderMap.clear();
}

void ParticleManager::AddParticleConditonData()
{
	AddOneParticleConditionData(101,4,0);
}

void ParticleManager::AddOneParticleConditionData(unsigned int type,unsigned int nums,float lifeTime)
{
	std::map<unsigned int ,ParticleConditionData>::iterator iter = ParticleConditionDatas.find(type);
	if (iter == ParticleConditionDatas.end())
	{
		ParticleConditionData data;
		int duration = SkeletonAnimRcsManager::getInstance()->getOneAnimationDurationTween(type,kType_Play);
		if (duration != 0)
		{
			data.type_id = type;
			data.sumNums = nums;
			data.lifeTime = ((float)duration)/FRAME_RATES;
			data.attachParticleNodes.clear();
			ParticleConditionDatas.insert(std::make_pair(type,data));
		}
	}
	else
	{
		CCLOGERROR("AddOneParticleConditionData repeat id %d",type);
	}
}

void ParticleManager::RemoveOneParticlePlayedNum(unsigned int type)
{
	std::map<unsigned int ,ParticleConditionData>::iterator iter = ParticleConditionDatas.find(type);
	if (iter != ParticleConditionDatas.end())
	{
		if ((*iter).second.playedNum > 0)
		{
			(*iter).second.playedNum --;
		}		
	}
}

bool ParticleManager::TellWhetherOneParticleCanPlay(unsigned int type,CCNode* attachParticle,bool & bAddOneParticle)
{
	if (attachParticle)
	{
		bAddOneParticle = false;

		std::map<unsigned int ,ParticleConditionData>::iterator iter = ParticleConditionDatas.find(type);
		if (iter == ParticleConditionDatas.end())
		{
			return true;
		}
		if ((*iter).second.playedNum < (*iter).second.sumNums)
		{
			std::map<CCNode*,float>::iterator particleIter = (*iter).second.attachParticleNodes.find(attachParticle);
			if (particleIter == (*iter).second.attachParticleNodes.end())
			{
				(*iter).second.playedNum ++;
				(*iter).second.attachParticleNodes.insert(std::make_pair(attachParticle,0));
				bAddOneParticle = true;
				return true;
			}			
			else
			{
				(*iter).second.attachParticleNodes[attachParticle] = 0;
				bAddOneParticle = true;
				return true;
			}
		}
	}
	return false;
}