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
#include "GreatFireBallState.h"
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



GreatFireBallState::GreatFireBallState(BaseAttackEvt * pEvt) : BaseAttackState(pEvt)
{
	m_Type = ToGreatFireBall;
	m_HighPriority.push_back(ToSpecialAttack);
}


bool GreatFireBallState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;

	m_attackBoxFrame = -1;

	MagicAnimHandler* pAnimHandler = (MagicAnimHandler*)m_pStateFactory->Create(ToMagicAnimHandler, m_skillID);
	
	Push(pAnimHandler);

	m_attackDir = pAnimHandler->GetAttackDir();
	m_attackDisplacement = pAnimHandler->GetEffectDisplacement();

	Push(m_pStateFactory->Create(ToAttackSenderHandler, m_skillID));
	
	Push(m_pStateFactory->Create(ToSkillBeginSenderHandler, m_skillID));

	return true;
}

void GreatFireBallState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();
}

void GreatFireBallState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);

	if(m_pSkill->effectId > 0 && m_pSkill->longDistanceAttackRange)
	{
		int animID = m_pRole->GetAnimID();
		//CCLOG("animID : %ld , Cur Fram : %ld", animID, m_pRole->getCurAnimationFrameID());
		if(animID == m_pSkill->animation[DIRECTION_LEFT].animationID ||
			animID == m_pSkill->animation[DIRECTION_RIGHT].animationID ||
			animID == m_pSkill->animation[DIRECTION_UP].animationID ||
			animID == m_pSkill->animation[DIRECTION_BOTTOM].animationID 
			
			)
		{
			cocos2d::CCRect heroAttackBox = m_pRole->getRelativeParentAttackBox();
			if(heroAttackBox.size.width > 0 || heroAttackBox.size.height > 0 )
			{
				int curFrame = m_pRole->getCurAnimationFrameID();
				if(m_attackBoxFrame == -1)
				{
					m_attackBoxFrame = curFrame;
					GreatMagicFireBallState* pFireBall = (GreatMagicFireBallState*)m_pStateFactory->Create(ToGreatMagicFireBall, m_skillID);
					pFireBall->SetAttackDir(m_attackDir, m_attackDisplacement);
					m_pSkillStateMachine->Push(pFireBall);
				}

			}
		}
		
	}
		
	
}


void GreatFireBallState::Reset()
{

}

// ----------------------------------------------------------------------------------------------------------------

GreatMagicFireBallState::GreatMagicFireBallState(BaseAttackEvt * pEvt) : BaseAttackState(pEvt)
{
	m_Type = ToGreatMagicFireBall;
	m_HighPriority.push_back(ToSpecialAttack);
}


bool GreatMagicFireBallState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;

	EffectAnimHandler* pAnimState = (EffectAnimHandler*)m_pStateFactory->Create(ToEffectAnimHandler, m_skillID);
	pAnimState->SetInitDirection(CCPoint(0.0f, 1.0f));
	pAnimState->SetAttackDir( m_attackDir, m_attackDisplacement);
	Push(pAnimState);

	return true;
}

void GreatMagicFireBallState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();


}

void GreatMagicFireBallState::Update(float dt)
{
	BaseAttackState::Update(dt);
	StatePoller::Update(dt);
}


void GreatMagicFireBallState::Reset()
{

}

void GreatMagicFireBallState::SetAttackDir( CCPoint dir, float displacement )
{
	m_attackDir = dir;
	m_attackDisplacement = displacement;

}