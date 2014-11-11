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

#include "OnlineNetworkManager.h"
#include "ApocalypseState.h"
#include "SpriteSeer.h"
#include "SkillDispMgr.h"
#include "IdleState.h"
#include "LinkTimeState.h"
#include "AttackChecker.h"
#include "UserData.h"
#include "cs_basic.pb.h"
#include "cs_core.pb.h"
#include "cs_battle.pb.h"
#include "LevelManager.h"
#include "HSJoystick.h"
#include "GameManager.h"
#include "PhysicsLayer.h"
#include "LevelLayer.h"
#include "ParticleManager.h"

ApocalypseState::ApocalypseState( BaseAttackEvt * pEvt ) 
	: BaseAttackState(pEvt)
{
	m_Type = ToApocalypse;

	m_LowPriority.push_back(ToNormalAttack);
}



bool ApocalypseState::Entered()
{
	bool ret = false;
	ret = BaseAttackState::Entered();

	Push(m_pStateFactory->Create(ToAnimHandler, m_skillID));
	if(m_pSkill->effectId > 0)
	{
		Push(m_pStateFactory->Create(ToRoleEffectHandler, m_skillID));
	}

	Push(m_pStateFactory->Create(ToAttackCheckerHandler, m_skillID));
	Push(m_pStateFactory->Create(ToAttackSenderHandler, m_skillID));

	Push(m_pStateFactory->Create(ToSkillBeginSenderHandler, m_skillID));


	return ret;
}

void ApocalypseState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();
}

void ApocalypseState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);

	if(m_pSkill == NULL)
	{
		ChangeToIdleState();
		return;
	}
}


void ApocalypseState::Reset()
{
	BaseAttackState::Reset();
}

