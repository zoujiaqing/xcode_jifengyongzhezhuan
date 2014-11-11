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
#include "IceGroundState.h"
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


IceGroundState::IceGroundState(BaseAttackEvt * pEvt) : BaseAttackState(pEvt)

{
	m_Type = ToIceGround;
	m_HighPriority.push_back(ToSpecialAttack);
}

bool IceGroundState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;

	m_attackBoxFrame = -1;

	AnimHandler* pAnimHandler = (MagicAnimHandler*)m_pStateFactory->Create(ToAnimHandler, m_skillID);
	Push(pAnimHandler);

	m_attackDir = pAnimHandler->GetAttackDir();
	m_attackDisplacement = pAnimHandler->GetEffectDisplacement();

	Push(m_pStateFactory->Create(ToAttackSenderHandler, m_skillID));

	Push(m_pStateFactory->Create(ToSkillBeginSenderHandler, m_skillID));


	return true;
}

void IceGroundState::Exited()
{
	BaseAttackState::Exited();

}

void IceGroundState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);

	if(m_pSkill->effectId > 0 && m_pSkill->longDistanceAttackRange)
	{
		int animID = m_pRole->GetAnimID();
		if(animID == m_pSkill->animation[DIRECTION_LEFT].animationID ||
			animID == m_pSkill->animation[DIRECTION_RIGHT].animationID ||
			animID == m_pSkill->animation[DIRECTION_UP].animationID ||
			animID == m_pSkill->animation[DIRECTION_BOTTOM].animationID
			)
		{
			cocos2d::CCRect heroAttackBox = m_pRole->getRelativeParentAttackBox();
			if( heroAttackBox.size.width > 0 || heroAttackBox.size.height > 0)
			{
				int curFrame = m_pRole->getCurAnimationFrameID();
				if(m_attackBoxFrame == -1)
				{
					m_attackBoxFrame = curFrame;
					IceGroundMagicState* pIceGroundMagic = dynamic_cast<IceGroundMagicState*>(m_pStateFactory->Create(ToIceGroundMagic, m_skillID));
					CCAssert(pIceGroundMagic != NULL, "Can't create IceGroundMagicState!");
					pIceGroundMagic->SetAttackDir(m_attackDir, m_attackDisplacement);
					m_pSkillStateMachine->Push(pIceGroundMagic);
				}
			}
		}
	}
}


void IceGroundState::Reset()
{

}

// ----------------------------------------------------------------------------------------------------------------

IceGroundMagicState::IceGroundMagicState(BaseAttackEvt * pEvt) : BaseAttackState(pEvt)
{
	m_Type = ToIceGroundMagic;
	m_HighPriority.push_back(ToSpecialAttack);
}


bool IceGroundMagicState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;


	EffectAnimHandler* pEffectAnimHandler = (EffectAnimHandler*)m_pStateFactory->Create(ToEffectAnimHandler, m_skillID);
	pEffectAnimHandler->SetAttackDir( m_attackDir, m_attackDisplacement);
	Push(pEffectAnimHandler);

	return true;
}

void IceGroundMagicState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();

}

void IceGroundMagicState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);

}


void IceGroundMagicState::Reset()
{

}

void IceGroundMagicState::SetAttackDir( CCPoint dir, float displacement )
{
	m_attackDir = dir;
	m_attackDisplacement = displacement;

}