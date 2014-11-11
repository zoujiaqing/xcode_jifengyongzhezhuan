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
#include "NormalAttackState.h"

class SpriteSeer;
class EffectSprite;

namespace AttackChecker
{
	class CBase;
};




// ------------------------------ SpecialAttackState ---------------------------------------------
class SpecialAttackState : public BaseAttackState, public StatePoller
{
public:
	SpecialAttackState(BaseAttackEvt * pEvt);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:


protected:

	float m_fContinueTime;

};