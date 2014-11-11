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
#include "BaseAttackState.h"

class EffectSprite;
class SpriteSeer;
namespace AttackChecker
{
	class CBase;
};

class NormalAttackState : public BaseAttackState, public StatePoller
{
public:
	NormalAttackState(BaseAttackEvt* pEvt);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:	

	cocos2d::CCPoint m_attackDir;
	float m_attackDisplacement;

	int m_attackBoxFrame;
};


// ----------------------------------------------------------------------------


class NormalMagicFireBallState : public BaseAttackState, public StatePoller
{
public:
	NormalMagicFireBallState(BaseAttackEvt * pEvt);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	void SetAttackDir( CCPoint dir, float displacement );

protected:
	

	cocos2d::CCPoint m_attackDir;
	float m_attackDisplacement;


};

