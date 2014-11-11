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
#include "ChopseriesState.h"
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
#include "actions/CCActionInstant.h"
#include "SpriteMonsterMgr.h"
#include "ParticleManager.h"

ChopseriesState::ChopseriesState(BaseAttackEvt * pEvt) : BaseAttackState(pEvt)
{
	m_Type = ToChopseries;
	m_HighPriority.push_back(ToSpecialAttack);
}

bool ChopseriesState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;

	Push(m_pStateFactory->Create(ToAnimHandler, m_skillID));
	Push(m_pStateFactory->Create(ToAttackCheckerHandler, m_skillID));
	Push(m_pStateFactory->Create(ToAttackSenderHandler, m_skillID));

	Push(m_pStateFactory->Create(ToSkillBeginSenderHandler, m_skillID));

	return true;
}

void ChopseriesState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();
}

void ChopseriesState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);
	
}


void ChopseriesState::Reset()
{

}