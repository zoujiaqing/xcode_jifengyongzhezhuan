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


class InvisibleKillerState : public BaseAttackState, public StatePoller
{
public:
	InvisibleKillerState(BaseAttackEvt * pEvt);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();


};

// ------------------------------------------------------------------

class ReviveInvincibleState : public InvisibleKillerState
{
public:
	ReviveInvincibleState(BaseAttackEvt * pEvt);

	//virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	EffectSprite* m_pEffect;

};
