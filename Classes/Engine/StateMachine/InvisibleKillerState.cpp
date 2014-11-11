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
#include "InvisibleKillerState.h"
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

InvisibleKillerState::InvisibleKillerState(BaseAttackEvt * pEvt) : BaseAttackState(pEvt)
{
	m_Type = ToInvisibleKiller;
	m_HighPriority.push_back(ToSpecialAttack);
}

bool InvisibleKillerState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;


	Push(m_pStateFactory->Create(ToAnimHandler, m_skillID));

	Push(m_pStateFactory->Create(ToAttackSenderHandler, m_skillID));

	Push(m_pStateFactory->Create(ToSkillBeginSenderHandler, m_skillID));

	if(m_pRole->getTag() == MAP_TAG_SEER)
	{
		m_pRole->SetRoleOpacity(255 - m_pSkill->alpha * 255);
	}
	else
	{
		m_pRole->SetRoleOpacity(0);
	}

	return true;
}

void InvisibleKillerState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();

	m_pRole->SetRoleOpacity(255);
	
}

void InvisibleKillerState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);

	if(m_pSkill == NULL)
	{
		Exited();

		ChangeToIdleState();
		return;
	}

}


void InvisibleKillerState::Reset()
{
	BaseAttackState::Reset();
}

// ----------------------------------------------------------------------
ReviveInvincibleState::ReviveInvincibleState( BaseAttackEvt * pEvt )
	: InvisibleKillerState(pEvt)
{
	m_Type = ToReviveInvincible;
	m_HighPriority.push_back(ToSpecialAttack);
}

bool ReviveInvincibleState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;

	Push(m_pStateFactory->Create(ToAnimHandler, m_skillID));
	
	m_pEffect = ParticleManager::Get()->createEffectSprite(190,"");

	m_pRole->getRoot()->addChild(m_pEffect, MAP_Z_OBJ_TOP);
	m_pEffect->SetAnim(kType_Play,1,false);

	CCPoint pos(0.0f, 0.0f);
	m_pEffect->setPosition(pos);

	if(m_pRole->getTag() == MAP_TAG_SEER)
	{
		m_pRole->SetRoleOpacity(255 - m_pSkill->alpha * 255);
	}
	else
	{
		m_pRole->SetRoleOpacity(0);
	}

	return true;
}

void ReviveInvincibleState::Update(float dt)
{
	InvisibleKillerState::Update(dt);
}

void ReviveInvincibleState::Exited()
{
	InvisibleKillerState::Exited();

	if(m_pEffect)
	{
		m_pEffect->removeFromParent();
		m_pEffect = NULL;
	}

	m_pRole->SetRoleOpacity(255);

}
