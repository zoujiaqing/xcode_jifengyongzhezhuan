// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#pragma once
#include "FAbstractTransition.h"
#include "SkillInfo.h"
#include "cs_battle.pb.h"
#include "BoneSpriteMoveable.h"
#include "SpriteElf.h"
#include <stack>
#include "AttackChecker.h"

class EffectSprite;
class SpriteSeer;
class SpriteMonster;
class SpriteElf;

namespace AttackChecker
{
	class CBase;
};
class BaseAttackEvt;
class BaseElfAttachData;

// ---------------------------------------------------------------------------------------------
class StateContainer
{
public:
	virtual ~StateContainer(void);

	virtual void Push(FState* pState);
	virtual void Remove(FState* pState);

	virtual FState* GetState(EventType type);

protected:
	std::list<FState*> m_states;

};

class StatePoller : public StateContainer
{
public:
	virtual ~StatePoller(void);
	virtual void Push(FState* pState);
	virtual void Remove(FState* pState);
	virtual void Remove(EventType type);
	virtual void Update(float dt);

	virtual void Exit(void);
protected:
	
};

class SkillStateMachine : public StatePoller
{
public:
	virtual ~SkillStateMachine(void){}

	virtual void Push(FState* pState);

	bool CheckStateRuning(FState* state)
	{
		bool bRet = false;
		if (state)
		{
			bRet = state->m_StateFlag != STOP;
		}
		return bRet;
	}

	bool CheckStateRuning(EventType type)
	{
		bool bRet = false;
		for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); iter++)
		{
			FState* state = (*iter);
			if (state)
			{
				if((*iter)->m_Type == type)
				{
					bRet = state->m_StateFlag == UPDATING;
					if(bRet)
						break;

				}
			}
			
		}

		
		return bRet;
	}

};

class StateFactory
{
public:
	StateFactory(SpriteSeer* pRole);

	FState* Create(EventType type, unsigned int skillID);
	FState* Create(BaseAttackEvt* pEvt);

protected:
	SpriteSeer* m_pRole;
};

class ElfStateFactory
{
public:
	ElfStateFactory(SpriteElf* pRole);

	FState* Create(EventType type, SpriteSeer* pHero,unsigned int skillID,BaseElfAttachData* pData = 0);
	FState* Create(BaseAttackEvt* pEvt);

protected:
	SpriteElf* m_pRole;
};
// ---------------------------------------------------------------------------------------------


class BaseAttackEvt : public FEvent
{
	friend class BaseAttackState;
public :

	BaseAttackEvt(SpriteSeer* pRole, unsigned int skillID)
	{
		m_EvtType = ToBaseAttack;
		m_pRole = pRole;
		m_skillId = skillID;
	}

	///// clone current normal attack event
	//virtual FEvent* Clone()
	//{
	//	BaseAttackEvt* pCloneEvent = new BaseAttackEvt(NULL, 0, 1);
	//	pCloneEvent->m_Sender = this->m_Sender;
	//	pCloneEvent->m_EvtType = this->m_EvtType;
	//	pCloneEvent->m_skillId = this->m_skillId;
	//	pCloneEvent->m_partId = this->m_partId;
	//	return pCloneEvent;
	//}

	SpriteSeer* m_pRole;

	unsigned int m_skillId;
	unsigned int m_partId;
};

class BaseElfAttachData
{
public:
	enum EFollowType
	{
		EFollowLeft = 0,
		EFollowRight,
	};
public:
	bool bStartWithAcc;
	EFollowType m_eFollowType;

	BaseElfAttachData()
		: bStartWithAcc(false)
		, m_eFollowType(EFollowLeft)
	{
	}
};

class BaseElfEvt : public FEvent
{
public :
	BaseElfEvt(SpriteElf* pElf, SpriteSeer* pSeer,unsigned int skillID,unsigned int normalSkillId,unsigned int assistSkillId,BaseElfAttachData* pAttachData = 0)
	{
		m_pElf = pElf;
		m_pSeer = pSeer;
		m_skillId = skillID;
		m_pAttachData = pAttachData;
		m_normalSkillId = normalSkillId;
		m_assistSkillId = assistSkillId;
	}
	SpriteSeer* m_pSeer;
	SpriteElf* m_pElf;
	unsigned int m_skillId;
	BaseElfAttachData* m_pAttachData;
	// Note: 添加普通攻击+助战技能
	unsigned int m_normalSkillId;
	unsigned int m_assistSkillId;
};

class BaseAttackState : public FState
{
public:
	BaseAttackState(BaseAttackEvt* pEvt);

	virtual ~BaseAttackState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	virtual void ChangeToIdleState( void );

	SpriteSeer* GetRole(void){return m_pRole;}
	const SKILL_INFO* GetSkillInfo(void){return m_pSkill;}

	void SetStateMachine(SkillStateMachine* pStateMachine){m_pSkillStateMachine = pStateMachine;}
	SkillStateMachine* GetStateMachine(void){return m_pSkillStateMachine;}

	void SetContainer(StateContainer* pContainer);

	void SetEventType(EventType type){m_Type = type;}
	void setElfAttack(SpriteElf * pElf ,int type )
	{
	   m_Elf = pElf ;
	   attackType =type ;
	}
	SpriteElf * getElfAttack(){ return m_Elf ;}
	int getAttackType(){return attackType ;}
protected:
	bool CanIdleState(void);
protected:
	SpriteSeer *m_pRole;
	SpriteElf *m_Elf ;
	int attackType ;
	const SKILL_INFO* m_pSkill;

	unsigned int m_skillID;
	unsigned int m_partID;

	BaseAttackEvt* m_pEvt;

	StateContainer* m_pContainer;
	SkillStateMachine* m_pSkillStateMachine;
	StateFactory* m_pStateFactory;
	
};


// ---------------------------------------------------------------------------
class AnimHandler : public CCObject, public BaseAttackState
{
public:
	AnimHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	void EndMove(CCNode*, void*);

	CCPoint GetAttackDir(void){return m_aimDir;}
	float GetEffectDisplacement(void){return m_effectDisplacement;}

protected:
	void SetNormalAttackDir();

	virtual void ChangeToLinkTime(void);

	virtual void UpdateDirectionByJoystick(float dt);

protected:

	int m_animationID;

	bool m_bStartLinkTime;
	bool m_bMoving;

	float m_displacement;
	float m_effectDisplacement;

	float m_fContinueTime;
	float m_fSoundTime;

	BoneSpriteMoveable::MOVE_TO_RESULT m_moveToResult;

	cocos2d::CCPoint m_nearestMonsterPos;
	cocos2d::CCPoint m_aimDir;

	bool m_bAnimMonster;

	BaseAttackState* m_pMoveToHandler;
};


// ---------------------------------------------------------------------------
class MagicAnimHandler : public AnimHandler
{
public:
	MagicAnimHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

protected:
	
};

// -----------------------------------------------------------

class LinkTimeHandler : public BaseAttackState
{
public:
	LinkTimeHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	float m_fSkillLinkTime;
};

// -----------------------------------------------------------

class AttackCheckerHandler : public BaseAttackState
{
public:
	AttackCheckerHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	// 必须在状态机的push函数之后调用
	void SetAttackAction(AttackChecker::CBase::ACTION action);
	AttackChecker::CBase* getAttackChecker(){return m_pAttackChecker ;}

protected:
	AttackChecker::CBase* m_pAttackChecker;
};

// ------------------------------------------------------
class MagicCheckerEvt : public BaseAttackEvt
{
public:
	MagicCheckerEvt(SpriteSeer* pRole, unsigned int skillID, EffectSprite* pEffect)
		:BaseAttackEvt(pRole, skillID)
	{
		m_EvtType = ToMagicCheckHandler;
		m_pRole = pRole;
		m_skillId = skillID;
		m_pEffect = pEffect;
	}

	EffectSprite* m_pEffect;
};

class MagicCheckerHandler : public AttackCheckerHandler
{
public:
	MagicCheckerHandler(BaseAttackEvt* pEvt );
	virtual bool Entered();

};

// -------------------------------------------------------

class AttackSenderHandler : public BaseAttackState
{
public:
	AttackSenderHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	CSPlayerAttackReq m_attackReq;
};

// -----------------------------------------------------------

class EffectAnimHandler : public BaseAttackState
{
public:
	EffectAnimHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	void SetInitDirection(CCPoint dir);
	void SetAttackDir( CCPoint dir, float displacement );

	void SetLinkEffectPos(const CCPoint& pos){m_linkEffectPos = pos; m_bLinkEffect = true;}

protected:

	cocos2d::CCPoint GetAnimDirVector(void);
	DIRECTION_ACTOR GetAnimDir(void);
	
	EffectSprite* GetEffect(int id);

	void SetEffectZOrder( CCPoint pos );

protected:

	EffectSprite* m_pEffect;
	CCPoint m_dir;
	float m_displacement;
	float m_speed;

	CCPoint m_initDir;
	bool m_bInitDir;

	cocos2d::CCPoint m_attackDir;
	float m_attackDisplacement;

	bool m_bLinkEffect;
	cocos2d::CCPoint m_linkEffectPos;
};

// -----------------------------------------------------------
class RoleEffectHandler : public BaseAttackState
{
public:
	RoleEffectHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();
	
protected:
	EffectSprite* m_pEffect;
	
};

// -----------------------------------------------------------
class AimDirectionHandler : public BaseAttackState
{
public:
	AimDirectionHandler(BaseAttackEvt* pEvt );

	AimDirectionHandler(const AimDirectionHandler& rhs);

	virtual bool Entered();

	/*virtual void Update(float dt);

	virtual void Exited();*/

	CCPoint GetDirection(void);
	CCPoint GetNearestMonsterPos(void);
	bool IsAimMonster(void);

protected:
	cocos2d::CCPoint GetAimDirection();

	SpriteMonster* GetNearestMonster(void);

protected:

	cocos2d::CCPoint m_nearestMonsterPos;
	cocos2d::CCPoint m_aimDir;
	bool m_bAimMonster;

	bool m_bEntered;

};

// -----------------------------------------------------------
class SkillBeginSenderHandler : public BaseAttackState
{
public:
	SkillBeginSenderHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	/*virtual void Update(float dt);

	virtual void Exited();*/

protected:
	
};

// -----------------------------------------------------------
class SkillBeginReceiverHandler : public BaseAttackState
{
public:
	SkillBeginReceiverHandler(BaseAttackEvt* pEvt );

	//virtual bool Entered();

	/*virtual void Update(float dt);

	virtual void Exited();*/

	CCPoint GetDirection(void){return m_aimDir;}
	CCPoint GetNearestMonsterPos(void){return m_nearestMonsterPos;}
	CCPoint GetRolePos(void){return m_rolePos;}
	CCPoint GetRoleDirection(void){return m_roleDir;}

	bool IsAimMonster(void){return m_bAimMonster;}

	void SetAimMonster(bool b){m_bAimMonster = b;}
	void SetNearestMonsterPos(const CCPoint& pos){m_nearestMonsterPos = pos;}
	void SetDirection(const CCPoint& dir){m_aimDir = dir;}
	void SetRolePos(const CCPoint& pos){m_rolePos = pos;}
	void SetRoleDirection(const CCPoint& dir){m_roleDir = dir;}

protected:
	cocos2d::CCPoint m_rolePos;
	cocos2d::CCPoint m_roleDir;
	
	cocos2d::CCPoint m_nearestMonsterPos;
	cocos2d::CCPoint m_aimDir;
	bool m_bAimMonster;
	
};

// -----------------------------------------------------------
class MoveToHandler : public BaseAttackState
{
public:
	MoveToHandler(BaseAttackEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	//virtual void Exited();

	void SetTarget(cocos2d::CCNode* pTarget, cocos2d::CCPoint& dstPos, float speed);

	bool IsFinish(void){return m_displacement <= 0.0f;}

protected:

	float m_displacement;
	CCPoint m_dir;
	float m_speed;
	cocos2d::CCNode* m_pTarget;

	
};

// -----------------------------------------------------------
class VariableSpeedMoveToHandler : public MoveToHandler
{
public:
	VariableSpeedMoveToHandler(BaseAttackEvt* pEvt );

	virtual void Update(float dt);

	void ChangeDstPos(const cocos2d::CCPoint& dstPos);

};
