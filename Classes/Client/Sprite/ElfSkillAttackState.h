/************************************************************************/
/* ÎÄ¼þÃèÊö£º¾«Áé×·ÖðÖ÷½Ç×´Ì¬                                           */
/************************************************************************/

#pragma once
#include "FAbstractTransition.h"
#include "BaseAttackState.h"
#include "ElfBaseState.h"

class SpriteElf;
class SpriteSeer;

class  ElfAttackTrigger : public ElfBaseState
{
public:
	ElfAttackTrigger(BaseElfEvt* pEvt);

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	cocos2d::CCNode* GetNearestMonster( void );
	bool GetBattlePVP( void );
protected:
	float m_cd;
	float m_normalSkillCd;
	float m_outerCircleRadius;
};

// -----------------------------------------------------------
class AssistElfAttackTrigger : public ElfBaseState
{
public:
	AssistElfAttackTrigger(BaseElfEvt* pEvt);
	virtual bool Entered();
	virtual void Update(float dt);
	virtual void Exited();
	
protected:
	bool GetBattlePVP( void );
	cocos2d::CCNode* GetNearestMonster( void );
	bool IsHasSomeEnemyAtArea(unsigned int area,unsigned int enemyNum);
protected:
	float m_cd;
};

// -----------------------------------------------------------
class ElfAimDirectionHandler : public ElfBaseState
{
public:
	ElfAimDirectionHandler(BaseElfEvt* pEvt );

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

// ---------------------------------------------------------------------------
class ElfAnimHandler : public ElfBaseState
{
public:
	ElfAnimHandler(BaseElfEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	CCPoint GetAttackDir(void){return m_aimDir;}
	float GetEffectDisplacement(void){return m_effectDisplacement;}

protected:
	void SetNormalAttackDir();

	virtual void ChangeToLinkTime(void);

protected:

	int m_animationID;

	bool m_bStartLinkTime;

	float m_fContinueTime;

	cocos2d::CCPoint m_nearestMonsterPos;
	cocos2d::CCPoint m_aimDir;
	float m_effectDisplacement;

	bool m_bAnimMonster;
};


// -----------------------------------------------------------

class ElfEffectAnimHandler : public ElfBaseState, public StatePoller
{
public:
	ElfEffectAnimHandler(BaseElfEvt* pEvt );

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	void SetInitDirection(CCPoint dir);
	void SetAttackDir( CCPoint dir, float displacement );

protected:

	cocos2d::CCPoint GetAnimDirVector(void);
	DIRECTION_ACTOR GetAnimDir(void);

	EffectSprite* GetEffect(int id);

	void SetEffectZOrder(CCPoint pos);

protected:

	EffectSprite* m_pEffect;
	CCPoint m_dir;
	float m_displacement;
	float m_speed;
	float m_continueTime;

	CCPoint m_initDir;
	bool m_bInitDir;
	AttackChecker::CBase*  pAttackChecker ;
	cocos2d::CCPoint m_attackDir;
	float m_attackDisplacement;
};


// ---------------------------------------------------------------------------------

class ElfSkillAttackState : public ElfBaseState, public StatePoller
{
public:
	ElfSkillAttackState(BaseElfEvt *pEvt);
	virtual ~ElfSkillAttackState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	CCPoint m_attackDir;
	float m_attackDisplacement;
	int m_attackBoxFrame;
};

class AssistElfSkillAttackState : public ElfBaseState, public StatePoller
{
public:
	AssistElfSkillAttackState(BaseElfEvt *pEvt);
	virtual ~AssistElfSkillAttackState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	CCPoint m_attackDir;
	float m_attackDisplacement;
	int m_attackBoxFrame;
};