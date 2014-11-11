#include "SpriteMonster.h"
#include "SpriteSeerListener.h"
#include "LevelDefine.h"

//#include "Define.h"
#include "EnginePlayerManager.h"
#include "GameManager.h"
#include "CrossPlatform.h"
#include "GameFontManager.h"
#include "SpriteMonsterMgr.h"
#include "GameAudioManager.h"
#include "ParticleManager.h"
#include "SkillDispMgr.h"
#include "LevelManager.h"
#include "simplefsm_macro.h"
#include "ItemUpdateManager.h"
#include "CameraController.h"
#include "LuaTinkerManager.h"
#include "MonsterData.h"


using namespace cocos2d;

//状态切换
bool SpriteMonster::Rule::States(StateMachineEvent event, int state)
{
	BeginStateMachine

	State(MONSTER_STATE_INITIALIZE)
	OnEnter
	OnUpdate
	OnExit

	State(MONSTER_STATE_IDLE)
	OnEnter
	_this->m_rootSprite->stopAllActions();
	_this->setAnimFromStateAndDirection(MONSTER_STATE_IDLE, _this->direction);
	OnUpdate
	OnExit

	State(MONSTER_STATE_RUN)
	OnEnter
	_this->setAnimFromStateAndDirection(MONSTER_STATE_RUN, _this->direction);
	OnUpdate
	OnExit

	State(MONSTER_STATE_ATTACK)
	OnEnter
	OnUpdate
	OnExit

	State(MONSTER_STATE_HURT)
	OnEnter
	_this->setAnimFromStateAndDirection(MONSTER_STATE_HURT, _this->direction);
	OnUpdate
	OnExit

	State(MONSTER_STATE_DEAD)
	OnEnter
	OnUpdate
	OnExit

	EndStateMachine
}

SpriteMonster::SpriteMonster(MonsterInitInfo info)
	:BoneSpriteMoveable(info.typeId, false),
	totalScheduleTimer(0.7f),
	selfScheduleTimer(-1.0f),
	m_attackedEffectSprite(NULL),
	buffEffectSprite(NULL),
	//lastAnimId(0),
	//lastFlip(false),
	showAnim(true),
	attackContinueTime(0.0f),
	attackSpeed(0.0f),
	canBeAttacked(true),
	canBeFocus(true),
	blinkPoint(ccp(0, 0)),
	attackContinueNode(NULL),
	inBuff(false),
	isDead(false)
{

}

SpriteMonster::~SpriteMonster()
{
	if (SpriteMonsterMgr::Get())
	{
		SpriteMonsterMgr::Get()->Remove(this);
	}

	if(m_attackedEffectSprite)
	{
		EnginePlayerManager::getInstance()->removeEffectAsprite(m_attackedEffectSprite);
		m_attackedEffectSprite = NULL;
	}

	if(buffEffectSprite)
	{
		EnginePlayerManager::getInstance()->removeEffectAsprite(buffEffectSprite);
		buffEffectSprite = NULL;
	}

	ItemUpdateManager::Get()->removeFromUpdateManager(this);

}

bool SpriteMonster::createTexture(unsigned int typeId)
{
	m_animBone =  EnginePlayerManager::getInstance()->addBoneAnimSprite(typeId); 
	resetParentRole(this);
	attachBoneAnimToRootSprite(1);

	//暴击特效
	m_effectSprite = ParticleManager::Get()->createEffectSprite(102,"");
	m_rootSprite->addChild(m_effectSprite,m_rootSprite->getZOrder()+20);	
	m_effectSprite->setPosition(getAttachPoint(kType_ATTACK_HURT)); 
	//m_effectSprite->setVisible(false);

	//普通攻击特效
	m_attackedEffectSprite = ParticleManager::Get()->createEffectSprite(101,"");
	m_rootSprite->addChild(m_attackedEffectSprite,m_rootSprite->getZOrder()+20);
	m_attackedEffectSprite->setPosition(getAttachPoint(kType_ATTACK_HURT));
	//m_attackedEffectSprite->setVisible(false);

	//攻击类型特效
	int attackType = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterBasicInfomation", "MonsterBasicInfomation", m_TypeID, "AttackType");

	attackTypeEffectSprite = ParticleManager::Get()->createEffectSprite(101,"");
	this->addChildToRoot(attackTypeEffectSprite);	
	//attackTypeEffectSprite->SetAnim(kType_Play,1,true);
	//attackTypeEffectSprite->setVisible(false);

	attackContinueNode = CCNode::create();
	this->addChild(attackContinueNode);

	shakeNode = CCNode::create();
	this->addChild(shakeNode);

	jumpNode = CCNode::create();
	this->addChild(jumpNode);

	buffNode = CCNode::create();
	this->addChild(buffNode);

	rushNode = CCNode::create();
	this->addChild(rushNode);

	rushEndNode = CCNode::create();
	this->addChild(rushEndNode);

	this->getRoot()->setVisible(false);

    return  true;
}

bool SpriteMonster::initWithInfoAndShow(MonsterInitInfo info)
{
	m_TotalHp = info.totalHp;
	blood = info.totalHp;
	speed = info.speed * 16;
	isBoss = info.isBoss;
	isElite = info.isElite;
	isAlliance = info.isAlliance;
	m_level = info.m_level;
	hp_line = info.hp_line;
	m_ID.setPlayId(info.uid,0,0);
	m_TypeID = info.typeId;
	m_name = std::string(info.monsterName);

	ItemUpdateManager::Get()->addToUpdateManager(this);


	//初始化状态机
	fsm_rule._this = this;

	fsm_rule.Initialize();

	//设置启动方向及状态
	this->direction = DIRECTION_DOWN_LEFT;

	fsm_rule.SetState(MONSTER_STATE_IDLE);

	this->getRoot()->setVisible(true);

	return true;
}

void SpriteMonster::Update(float fTime,bool &bDeleteSelf, bool &isDead)
{
	
	bDeleteSelf = false;
	isDead = false;

	//冰封巨人跳入屏幕时执行显示操作会导致动画错乱
	//if (!isBoss)
	//{
	//	if (GetUpdateEnabled() && !showAnim)
	//	{
	//		this->SetAnim(lastAnimId,1,true,this);
	//		this->SetAnimFlipX(lastFlip);
	//		showAnim = true;
	//	}
	//
	//	if (!GetUpdateEnabled() && showAnim)
	//	{
	//		showAnim = false;
	//	}
	//}

	//状态机更新	
	fsm_rule.Update();

	//怪物渐隐消失操作
	if (!isAlliance && selfScheduleTimer != -1)
	{
		selfScheduleTimer += fTime;
		if (selfScheduleTimer >= (totalScheduleTimer))
		{
			selfScheduleTimer = -1;
			m_animBone->getCurRoleActorBase()->setArmaturetOpacity(0);

			bDeleteSelf = true;
		}
		else
		{
			m_animBone->getCurRoleActorBase()->setArmaturetOpacity(255 * (1.0 - ((float)selfScheduleTimer)/totalScheduleTimer));

		}
	}

	//变红还原
	if (m_bInRedState)
	{
		m_InRedTime += fTime;
		if (m_InRedTime > 0.2f)
		{
			m_InRedTime = 0;
			revertToOriginColor();
		}
	}

	//层级更新
	LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (layer)
	{
		//怪物层级更新	
		CCPoint pos = getPosition();
		if(pos.x != m_oldPos.x || pos.y != m_oldPos.y)
		{
			m_oldPos = pos;	
		
			if (layer)
			{
				layer->getObjectLayer()->reorderChild(this, LevelLayer::sCalcZorder(pos));
			}		
		}
	}

	if (this->isDead)
	{
		isDead = true;
	}
}

CCPoint SpriteMonster::moveToPoint(CCPoint desPoint, float speed)
{
	CCPoint curPoint = this->getPosition();

	CPhysicsLayer * pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer()->getPhysicsLayer();

	if (!pLayer->isPointReachable(desPoint))
	{
		desPoint = curPoint;
	}	

	if (desPoint.x == curPoint.x && desPoint.y == curPoint.y)
	{
		return curPoint;
	}

	if (speed == 0)
	{
		return curPoint;
	}

	CCArray *arrayOfActions = CCArray::create();

	float distance = ccpDistance(curPoint, desPoint);
	CCMoveTo *move = CCMoveTo::create( distance / speed, desPoint);

	//被击退结束时切换回idle状态
	this->mtpState = int(MONSTER_STATE_IDLE);
	CCCallFunc *endFunc = CCCallFuncND::create(this, 
								callfuncND_selector(SpriteMonster::ccCallChangeState),
								(void *)&mtpState);

	arrayOfActions->addObject(move);
	//arrayOfActions->addObject(endFunc);
				
	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	m_rootSprite->stopAllActions();
	m_rootSprite->runAction(sequence);		
	

	return desPoint;
}

void SpriteMonster::moveByPoints(const std::vector<CCPoint>& points)
{
	CCPoint nowPoint = this->getPosition();	
	float distance1 = ccpDistance(points[0], nowPoint);

	if(points.size() == 1 && distance1 < 1.0f)
	{
		//CCLOG("no move");
		return;
	}
	//使用格子点判断方向，同格子不改方向
	CCPoint endCell = LevelManager::sShareInstance()->pointPixelToCell(points[0]);
	CCPoint startCell = LevelManager::sShareInstance()->pointPixelToCell(nowPoint);

	this->mbpDir1 = CCPoint(ccpSub(endCell, startCell));
	//CCLOG("monster: %d  position: %f, %f, delay distance: %f", (int)this->m_ID, startCell.x, startCell.y, distance1);

	//启动和结束时的动画转换
	this->mbpState1 = int(MONSTER_STATE_IDLE);
	this->mbpState2 = int(MONSTER_STATE_RUN);
	
	CCCallFunc *startFunc = CCCallFuncND::create(this, 
										callfuncND_selector(SpriteMonster::ccCallChangeState),
										(void *)&mbpState2);
	CCCallFunc *endFunc = CCCallFuncND::create(this, 
										callfuncND_selector(SpriteMonster::ccCallChangeState),
										(void *)&mbpState1);


	CCArray *arrayOfActions = CCArray::create();

	if(points.size() > 1)
	{
		float distance2 = ccpDistance(points[1], points[0]);
		this->mbpDir2 = CCPoint(ccpSub(points[1], points[0]));

		//平滑加速
		float time1;
		if (distance1 == 0 && distance2 == 0)
		{
			time1 = 0;
		}
		else
		{
			time1 = (distance1 * distance2 / (distance1 + distance2)) / (speed);
		}
		float totalTime = (distance2) / speed;

		//偏差过大时闪烁
		//if (distance1 > 32.0f)
		{
			//time1 = 0;
		}		
		CCMoveTo *move1 = CCMoveTo::create( time1, points[0]);
		CCMoveTo *move2 = CCMoveTo::create( totalTime - time1 , points[1]);

		//func1和func2为调整方向
		CCCallFunc *func1 = CCCallFuncND::create(this, 
										callfuncND_selector(SpriteMonster::ccCallChangeDirection),
										(void *)&mbpDir1);
		CCCallFunc *func2 = CCCallFuncND::create(this, 
										callfuncND_selector(SpriteMonster::ccCallChangeDirection),
										(void *)&mbpDir2);
		arrayOfActions->addObject(startFunc);

		arrayOfActions->addObject(func1);
		arrayOfActions->addObject(move1);

		arrayOfActions->addObject(func2);
		arrayOfActions->addObject(move2);
	}
	else
	{
		CCMoveTo *move = CCMoveTo::create( distance1 / (speed), points[0]);	
		CCCallFunc *func = CCCallFuncND::create(this, 
										callfuncND_selector(SpriteMonster::ccCallChangeDirection),
										(void *)&mbpDir1);

		arrayOfActions->addObject(startFunc);

		arrayOfActions->addObject(func);
		arrayOfActions->addObject(move);
	}

	for(size_t i = 1; i < points.size() - 1; i++)
	{
		// 无视过多的寻路点
		if (i >= 30)
		{
			break;
		}

		float distance = ccpDistance(points[i+1], points[i]);
		this->mbpDir3[i] = CCPoint(ccpSub(points[i+1], points[i]));

		CCMoveTo *move = CCMoveTo::create( distance / speed, points[i+1]);
		CCCallFunc *func = CCCallFuncND::create(this, 
										callfuncND_selector(SpriteMonster::ccCallChangeDirection),
										(void *)(mbpDir3 + i));
		arrayOfActions->addObject(func);
		arrayOfActions->addObject(move);
	}

	arrayOfActions->addObject(endFunc);

	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	m_rootSprite->stopAllActions();
	m_rootSprite->runAction(sequence);

}

void SpriteMonster::PlayAttackedEffect(bool isCrit)
{
	EffectSprite * effectNode = isCrit ? m_effectSprite : m_attackedEffectSprite;
	effectNode->SetAnim(kType_Play,1,false);
}

void SpriteMonster::ccCallChangeDirection(CCNode * node, void * data)
{
	CCPoint *pdirect = (CCPoint *)data;
	this->changeDirection(*pdirect);
}

void SpriteMonster::ccCallChangeState(CCNode * node, void * data)
{
	int * pstate = (int *)data;
	if (MONSTER_STATE_DEAD != this->fsm_rule.GetState())
	{
		this->fsm_rule.SetState(*pstate);
	}	
}

void SpriteMonster::ccCallShake(CCNode * node,void * data)
{
	float scaleTime = 0.5f;
	CameraController::sharedCameraController()->shakeAndScale(scaleTime);
}

void SpriteMonster::changeDirection(CCPoint direct)
{
	if((direct.x == 0 && direct.y == 0) || 
		MONSTER_STATE_DEAD == this->fsm_rule.GetState() ||
		this->pointToDirection(direct) == this->direction)
	{
		return;
	}

	//更新方向
	this->direction = this->pointToDirection(direct);
	
	//设置动画
	setAnimFromStateAndDirection(this->fsm_rule.GetState(), this->direction);
	return;
	
}

DIRECTION_MONSTER SpriteMonster::pointToDirection(CCPoint direct)
{
	return direct.y > 0 ? (direct.x > 0 ? DIRECTION_UP_RIGHT : DIRECTION_UP_LEFT):(direct.x > 0 ? DIRECTION_DOWN_RIGHT : DIRECTION_DOWN_LEFT);
}

void SpriteMonster::setAnimFromStateAndDirection(MONSTER_STATE state, DIRECTION_MONSTER direction)
{
	int animId = kTypeAction_UnKnow;
	bool isFlipX = false;
	bool isLoop = true;
	bool isPauseOnOver = false;

	//根据怪物状态确定基准animId
	switch(state){
		case MONSTER_STATE_IDLE:
			animId = kTypeIdle_FaceDown_Stand_OTH;
			break;

		case MONSTER_STATE_RUN:
			animId = kTypeRun_FaceDown_OTH;
			break;

		case MONSTER_STATE_ATTACK:
			animId = kTypeAttack_FaceDown_1;
			isLoop = false;
			break;

		case MONSTER_STATE_HURT:
			animId = kTypeHurt_FaceDown;
			isLoop = false;
			break;

		case MONSTER_STATE_DEAD:
			animId = kTypeDie_FaceDown;
			isLoop = true;		
			isPauseOnOver = true;
			if (!isBoss)
			{
				//isPauseOnOver = true;
				//isLoop = true;
				selfScheduleTimer = 0;
			}			
			
			break;

		default:
			break;
	}

	//根据方向修正animId及水平翻转
	switch(direction)
	{
		case DIRECTION_UP_LEFT:
			isFlipX = true;
			animId += 1;
			break;

		case DIRECTION_UP_RIGHT:
			isFlipX = false;
			animId += 1;
			break;

		case DIRECTION_DOWN_LEFT:
			isFlipX = false;
			break;

		case DIRECTION_DOWN_RIGHT:
			isFlipX = true;
			break;

		default:
			break;
	}

	this->SetAnim(animId,1,isLoop,this);
	this->SetAnimFlipX(isFlipX);

	//lastAnimId = animId;
	//lastFlip = isFlipX;

	if (isPauseOnOver)
	{
		this->SetAnimPauseOnOver(true);
	}	
	else
	{
		this->SetAnimPauseOnOver(false);
	}
	return;
}

void SpriteMonster::setAnimFromStateAndDirection(int state, DIRECTION_MONSTER direction)
{
	setAnimFromStateAndDirection((MONSTER_STATE)state, direction);
}

void SpriteMonster::setAttackAnimation(int animId, DIRECTION_MONSTER direction)
{
	int i;
	for (i = 0; i < extraAnims.size(); i++)
	{
		if (extraAnims[i] == animId)
		{
			break;
		}
	}

	if (i >= extraAnims.size())
	{
		extraAnims.push_back(animId);
	}


	bool isFlipX = false;
	switch (direction)
	{
		case DIRECTION_UP_LEFT:
		case DIRECTION_DOWN_RIGHT:
			isFlipX = true;
			break;

		case DIRECTION_UP_RIGHT:
		case DIRECTION_DOWN_LEFT:
			isFlipX = false;
			break;

		default:
			break;
	}

	this->SetAnim(animId,1,true,this);
	this->SetAnimFlipX(isFlipX);

	//lastAnimId = animId;
	//lastFlip = isFlipX;

	return;
}

void SpriteMonster::onEventMsgMove(const std::vector<CCPoint>& points)
{
	if (this->speed == 0)
	{
		return;
	}
	//CCLOG("monster %d receive move msg............", m_ID);
	//CCLOG("onEventMsgMove : now state %d", fsm_rule.GetState());
	if(MONSTER_STATE_IDLE != fsm_rule.GetState() && MONSTER_STATE_RUN != fsm_rule.GetState())
	{
		bool attackCanMove = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "AttackCanMove", m_TypeID);
		if (attackSpeed != 0 && attackCanMove || !GetUpdateEnabled())
		{
			//仙人掌攻击中可以移动
			CCArray *arrayOfActions = CCArray::create();

			for (size_t i = 0; i < points.size(); i++)
			{
				float distance;
				if (i == 0)
				{
					distance = ccpDistance(getPosition(), points[0]);
				}
				else
				{
					distance = ccpDistance(points[i-1], points[i]);
				}

				CCMoveTo * move = CCMoveTo::create(distance / attackSpeed, points[i]);
				arrayOfActions->addObject(move);
			}
						
			CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

			m_rootSprite->stopAllActions();
			m_rootSprite->runAction(sequence);	
		}
		return;
	}
	moveByPoints(points);
}

void SpriteMonster::onEventMsgAttack(CCPoint serverPos, CCPoint direction, int animId, SkillInfo info)
{
	CCLOG("onEventMsgAttack : animId : %d", animId);
	CCLOG("onEventMsgAttack : effectId : %d", info.effectId);
	if (this->fsm_rule.GetState() == MONSTER_STATE_DEAD || this->fsm_rule.GetState() == MONSTER_STATE_ATTACK)
	{
		//尝试解决冲刺时卡在攻击状态的BUG
		if (ATTACK_TYPE_RUSH != info.type)
		{
			return;
		}		
	}
	
	this->setPosition(serverPos);
	this->m_rootSprite->stopAllActions();	
	this->changeDirection(direction);
	setAttackAnimation(animId, this->direction);

	this->fsm_rule.SetState(MONSTER_STATE_ATTACK);

	if (isBoss)
	{
		SpriteMonsterMgr::Get()->playMonsterSkillSound(info.skillId);
	}

	//普通攻击时，隐形怪现形
	if( (int)kTypeAttack_FaceDown_1 == animId || (int)kTypeAttack_FaceUp_1   == animId )			
	{
		this->getRoot()->setVisible(true);
		this->setShadowVisible(true);
		this->setCanBeFocus(true);
	}

	//魔法长老瞬移不可攻击
	//隐形技能开始不可攻击
	bool blinkStart = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "BlinkStart", animId);
	bool hideStart = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "HideStart", animId);
	if (blinkStart || hideStart)
	{
		this->setCanBeAttacked(false);
	}	

	//巨石怪震屏
	bool shakeScreen = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "ShakeStart", animId);
	if (shakeScreen)
	{
		size_t shakeCount = info.shakeTimeVec.size();
		if (shakeCount > 0)
		{
			CCArray *arrayOfActions = CCArray::create();
			for (size_t i = 0; i < shakeCount; i++)
			{
				if (i == 0)
				{
					CCDelayTime * delayTime = CCDelayTime::create(info.shakeTimeVec[i] / 1000);
					CCCallFunc *shakeFunc = CCCallFuncND::create(this, 
													callfuncND_selector(SpriteMonster::ccCallShake),
													NULL);
					arrayOfActions->addObject(delayTime);
					arrayOfActions->addObject(shakeFunc);
					continue;
				}

				CCDelayTime * delayTime = CCDelayTime::create((info.shakeTimeVec[i] - info.shakeTimeVec[i-1])/ 1000);
				CCCallFunc *shakeFunc = CCCallFuncND::create(this, 
													callfuncND_selector(SpriteMonster::ccCallShake),
													NULL);
				arrayOfActions->addObject(delayTime);
				arrayOfActions->addObject(shakeFunc);
			}

			CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

			shakeNode->stopAllActions();
			shakeNode->runAction(sequence);	
		}
	}

	bool delayIdle = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "DelayIdle", animId);
	if (delayIdle)
	{
		CCArray *arrayOfActions = CCArray::create();

		CCDelayTime * delayTime = CCDelayTime::create(attackContinueTime);
		this->dtiState = int(MONSTER_STATE_IDLE);
		CCCallFunc *endFunc = CCCallFuncND::create(this, 
									callfuncND_selector(SpriteMonster::ccCallChangeState),
									(void *)&dtiState);

		arrayOfActions->addObject(delayTime);
		arrayOfActions->addObject(endFunc);
				
		CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

		attackContinueNode->stopAllActions();
		attackContinueNode->runAction(sequence);
	}

	//野猪冲刺
	if (ATTACK_TYPE_RUSH == info.type)
	{
		rushInfo = info;
	
		CCArray *arrayOfActions = CCArray::create();

		CCDelayTime * delay = CCDelayTime::create(info.delayTime / 1000);
		CCCallFunc * func = CCCallFunc::create(this, callfunc_selector(SpriteMonster::ccCallRushByInfo));

		arrayOfActions->addObject(delay);
		arrayOfActions->addObject(func);

		CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

		rushNode->stopAllActions();
		rushNode->runAction(sequence);
	}
	else if (ATTACK_TYPE_JUMP == info.type)
	{
		this->setCanBeAttacked(false);
		
		jumpInfo = info;
		CCArray *arrayOfActions = CCArray::create();

		CCDelayTime * delay = CCDelayTime::create(info.delayTime / 1000);
		CCCallFunc * func = CCCallFunc::create(this, callfunc_selector(SpriteMonster::ccCallJumpByInfo));

		arrayOfActions->addObject(delay);
		arrayOfActions->addObject(func);

		CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

		jumpNode->stopAllActions();
		jumpNode->runAction(sequence);
	}
}

void SpriteMonster::ccCallRushByInfo()
{
	//若已死亡直接返回
	if (!this->fsm_rule.IsRunning())
	{
		return;
	}

	bool isFlipX = false;
	switch (direction)
	{
		case DIRECTION_UP_LEFT:
		case DIRECTION_DOWN_RIGHT:
			isFlipX = true;
			break;

		case DIRECTION_UP_RIGHT:
		case DIRECTION_DOWN_LEFT:
			isFlipX = false;
			break;

		default:
			break;
	}

	this->SetAnim(rushInfo.effectId,1,true,this);
	this->SetAnimFlipX(isFlipX);

	CCPoint nowPoint = this->getPosition();
	float distance = ccpDistance(getPosition(), rushInfo.endPosition);
	float virDistance = ccpDistance(getPosition(), rushInfo.virPosition);

	CCAssert(distance <= virDistance);

	float speed = rushInfo.speed * 16;

	this->dtiState = int(MONSTER_STATE_IDLE);

	CCMoveTo * move = CCMoveTo::create(distance / speed, rushInfo.endPosition);	
	m_rootSprite->stopAllActions();
	m_rootSprite->runAction(move);

	//结束时的状态转换用单独的node进行处理，防止中断
	CCArray *arrayOfActions = CCArray::create();

	CCDelayTime * delay = CCDelayTime::create(virDistance / speed);
	CCCallFunc *endFunc = CCCallFuncND::create(this, 
									callfuncND_selector(SpriteMonster::ccCallChangeState),
									(void *)&dtiState);

	arrayOfActions->addObject(delay);
	arrayOfActions->addObject(endFunc);

	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	rushEndNode->stopAllActions();
	rushEndNode->runAction(sequence);	
}

void SpriteMonster::ccCallJumpByInfo()
{
	this->setPosition(jumpInfo.endPosition);

	this->getRoot()->setVisible(true);
	this->setShadowVisible(true);

	this->SetAnim(jumpInfo.effectId,1,false,this);
}

void SpriteMonster::onEventMsgBuff(int animId, BuffInfo info, MonsterBattleInfo battleInfo)
{
	//CCLOG("onEventMsgBuff..........");
	buffBattleInfo = battleInfo;
	//播放自身动画
	bool isFlipX = false;
	switch (direction)
	{
		case DIRECTION_UP_LEFT:
		case DIRECTION_DOWN_RIGHT:
			isFlipX = true;
			break;

		case DIRECTION_UP_RIGHT:
		case DIRECTION_DOWN_LEFT:
			isFlipX = false;
			break;

		default:
			break;
	}

	this->SetAnim(animId,1,true,this);
	this->SetAnimFlipX(isFlipX);

	//添加并显示buff特效
	if (!buffEffectSprite)
	{
		buffEffectSprite = ParticleManager::Get()->createEffectSprite(info.effectId,"");
		bool isFrontEffect = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "FrontBuffEffect", info.effectId);
		if (isFrontEffect)
		{
			m_rootSprite->addChild(buffEffectSprite,m_rootSprite->getZOrder()+20);
		}
		else
		{
			this->addChildToRoot(buffEffectSprite);
		}
		
	}

	if (isBoss)
	{
		SpriteMonsterMgr::Get()->playMonsterSkillSound(info.skillId);
	}
	
	buffEffectSprite->SetAnim(kType_Play,1,true);
	buffEffectSprite->setVisible(false);

	//延迟生效buff属性，结束buff属性
	CCArray *arrayOfActions = CCArray::create();

	CCDelayTime * delayStartBuff = CCDelayTime::create(info.delayTime / 1000.0f);
	CCCallFunc * startBuff = CCCallFunc::create(this, callfunc_selector(SpriteMonster::ccCallBuffStart));
	CCDelayTime * delayEndBuff = CCDelayTime::create((info.deadTime - info.delayTime) / 1000.0f);
	CCCallFunc * endBuff = CCCallFunc::create(this, callfunc_selector(SpriteMonster::ccCallBuffEnd));

	arrayOfActions->addObject(delayStartBuff);
	arrayOfActions->addObject(startBuff);
	arrayOfActions->addObject(delayEndBuff);
	arrayOfActions->addObject(endBuff);

	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	buffNode->stopAllActions();
	buffNode->runAction(sequence);
}

void SpriteMonster::ccCallBuffStart()
{
	//CCLOG("Monster %d Buff Start.........", (int)m_ID);	
	BATTLEINFO * battleInfo =(BATTLEINFO*)(MONSTERINFO*)(EntityInfo::Get()->GetInfo(m_ID));
	normalBattleInfo = battleInfo->GetMonsterBattleInfo();
	battleInfo->Set(buffBattleInfo);
	buffEffectSprite->setVisible(true);
	inBuff = true;
}

void SpriteMonster::ccCallBuffEnd()
{
	//CCLOG("Monster %d Buff End.........", (int)m_ID);
	BATTLEINFO * battleInfo =(BATTLEINFO*)(MONSTERINFO*)(EntityInfo::Get()->GetInfo(m_ID));
	battleInfo->Set(normalBattleInfo);
	buffEffectSprite->setVisible(false);
	inBuff = false;
}


void SpriteMonster::onEventMsgDead()
{
	this->fsm_rule.SetState(MONSTER_STATE_DEAD);
	this->fsm_rule.Terminate();
	this->m_rootSprite->stopAllActions();
	this->setAnimFromStateAndDirection(MONSTER_STATE_DEAD, this->direction);
	
	//停止震屏
	this->stopAllActions();

	//普通怪物播放死亡声音
	SpriteMonsterMgr::Get()->playMonsterDieSound(m_TypeID);

	CCNode * eliteEffect = this->m_rootSprite->getChildByTag(ELITE_TAG);
	if (eliteEffect)
	{
		eliteEffect->removeFromParentAndCleanup(true);
	}
	//attackTypeEffectSprite->setVisible(false);
	this->isDead = true;
}

CCPoint SpriteMonster::onEventAttacked(CCPoint desPoint)
{	
	//被攻击时现形
	this->getRoot()->setVisible(true);
	this->setShadowVisible(true);
	this->setCanBeFocus(true);

	if (attackTypeEffectSprite)
	{
		attackTypeEffectSprite->SetAnim(kType_Play,1,false);
	}	

	//boss被攻击变红
	if (this->isBoss)
	{		
		playBecomeRedEffect(true);
	}

	CCPoint nowPos = this->getPosition();
	CCPoint endPos = nowPos;

	if (!LevelManager::sShareInstance()->getCurrentLevelLayer()->getPhysicsLayer()->isPointReachable(desPoint))
	{
		desPoint = nowPos;
	}	

	//防止未接收到死亡包时怪物不消失
	if (this->fsm_rule.GetState() == MONSTER_STATE_DEAD)
	{
		return endPos;
	}

	if(!this->isBoss && !this->isElite)
	{
		ParticleManager::Get()->stopEffectOfMonster(this);
		m_rootSprite->stopAllActions();
		endPos = this->moveToPoint(desPoint, 192.0f);	
		this->fsm_rule.SetState(MONSTER_STATE_HURT);
	}	

	return endPos;
}

void SpriteMonster::removeSelf()
{
	SpriteMonsterMgr::Get()->Remove(this);
	this->removeFromParentAndCleanup(true);
}

void SpriteMonster::animationHandler(const char* _aniType, const char* _aniID, const char* _frameID)
{
	if (0 == strcmp(_aniType,"loopComplete") ||
		0 == strcmp(_aniType,"complete"))
	{
		//攻击状态结束切回idle
		int animId = m_animBone->getAnimId();
		bool rushOnlyType = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "RushOnly", m_TypeID);
		if( (int)kTypeAttack_FaceDown_1 == animId ||
			(int)kTypeAttack_FaceUp_1   == animId)			
		{			
			if (!rushOnlyType)
			{
				this->fsm_rule.SetState(MONSTER_STATE_IDLE);			
			}			
		}

		if(	(int)kTypeHurt_FaceDown == animId ||
			(int)kTypeHurt_FaceUp   == animId)			
		{
			this->fsm_rule.SetState(MONSTER_STATE_IDLE);
		}

		if(	(int)kTypeDie_FaceDown == animId ||
			(int)kTypeDie_FaceUp   == animId)			
		{
			if (isBoss)
			{
				//m_animBone->getCurRoleActorBase()->setArmaturetOpacity(0);
				this->setShadowVisible(false);

				//死亡结束后开始渐隐消失操作
				if (selfScheduleTimer < 0)
				{
					selfScheduleTimer = 0;
				}

				return;
			}
		}

		if (!rushOnlyType)
		{
			handleAttackEnd(animId);
		}		

		//魔法长老瞬移第二段
		bool blinkEnd = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "BlinkEnd", animId);
		if (blinkEnd)
		{
			this->setCanBeAttacked(true);
			this->fsm_rule.SetState(MONSTER_STATE_IDLE);
		}

		bool jumpDown = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "JumpDown", animId);
		if (jumpDown)
		{
			this->setCanBeAttacked(true);
			this->fsm_rule.SetState(MONSTER_STATE_IDLE);

			if (buffEffectSprite && inBuff)
			{
				buffEffectSprite->setVisible(true);
			}

			this->m_rootSprite->stopAllActions();
			this->setAnimFromStateAndDirection(MONSTER_STATE_IDLE, this->direction);
		}

		bool buffAnimEnd = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "BuffAnimEnd", animId);
		if (buffAnimEnd)
		{
			this->fsm_rule.SetState(MONSTER_STATE_IDLE);

			this->m_rootSprite->stopAllActions();
			this->setAnimFromStateAndDirection(MONSTER_STATE_IDLE, this->direction);
		}
	}
}

void SpriteMonster::handleAttackEnd(int animId)
{
	for (int i = 0; i < extraAnims.size(); i++)
	{
		if (extraAnims[i] == animId)
		{
			bool blinkStart = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "BlinkStart", animId);
			if (blinkStart)
			{
				this->setPosition(blinkPoint);
			}
			
			bool hideStart = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "HideStart", animId);
			if (hideStart)
			{
				this->setPosition(blinkPoint);
				this->setCanBeAttacked(true);	
				this->getRoot()->setVisible(false);
				this->setShadowVisible(false);
				this->setCanBeFocus(false);
			}

			bool jumpStart = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "JumpStart", animId);
			if (jumpStart)
			{
				this->getRoot()->setVisible(false);
				this->setShadowVisible(false);
				
				if (buffEffectSprite && inBuff)
				{
					buffEffectSprite->setVisible(false);
				}
			}

			bool animLinkStart = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MonsterInfo.lua", "isIdInTable", "AnimLinkStart", animId);
			if (animLinkStart)
			{
				int downAnim = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterInfo", "AnimLinkTable", animId, "Down");
				int upAnim   = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterInfo", "AnimLinkTable", animId, "Up");
				switch (direction)
				{
				case DIRECTION_UP_LEFT:
					this->SetAnim(upAnim,1,true,this);
					this->SetAnimFlipX(true);
					break;
				case DIRECTION_UP_RIGHT:
					this->SetAnim(upAnim,1,true,this);
					this->SetAnimFlipX(false);
					break;
				case DIRECTION_DOWN_LEFT:
					this->SetAnim(downAnim,1,true,this);
					this->SetAnimFlipX(false);
					break;
				case DIRECTION_DOWN_RIGHT:
					this->SetAnim(downAnim,1,true,this);
					this->SetAnimFlipX(true);
					break;
				default:
					break;
				}	
			}		
			else
			{
				this->fsm_rule.SetState(MONSTER_STATE_IDLE);
			}		
			break;
		}
	}
}