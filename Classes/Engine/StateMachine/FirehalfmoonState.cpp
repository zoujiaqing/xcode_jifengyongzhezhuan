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
#include "FirehalfmoonState.h"
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
#include "GreatFireBallState.h"

FirehalfmoonState::FirehalfmoonState(BaseAttackEvt* pEvt) : BaseAttackState(pEvt)
{
	m_Type = ToFirehalfmoon;
}

bool FirehalfmoonState::Entered()
{
	if (false == BaseAttackState::Entered())
		return false;
	
	m_attackBoxFrame = -1;

	MagicAnimHandler* pAnimHandler = (MagicAnimHandler*)m_pStateFactory->Create(ToMagicAnimHandler, m_skillID);

	Push(pAnimHandler);
	
	m_attackDir = pAnimHandler->GetAttackDir();
	m_attackDisplacement = pAnimHandler->GetEffectDisplacement();

	bool bAttackChecker = true;
	if(m_pRole->getType() == kTypeMagician_Girl || m_pRole->getType() == kTypeMagician_Boy)
	{
		bAttackChecker = false;
	}

	if(bAttackChecker)
	{
		Push(m_pStateFactory->Create(ToAttackCheckerHandler, m_pEvt->m_skillId));

	}
	

	Push(m_pStateFactory->Create(ToAttackSenderHandler, m_skillID));
	
	Push(m_pStateFactory->Create(ToSkillBeginSenderHandler, m_skillID));
	

	return true;
}

void FirehalfmoonState::Exited()
{
	BaseAttackState::Exited();
	StatePoller::Exit();
}

void FirehalfmoonState::Update(float dt)
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
			if(heroAttackBox.size.width > 0 || heroAttackBox.size.height > 0)
			{
				if(m_pSkillStateMachine)
				{
					int curFrame = m_pRole->getCurAnimationFrameID();
					if(m_attackBoxFrame == -1)
					{
						m_attackBoxFrame = curFrame;
						GreatMagicFireBallState* pNormalMagicFireBall = (GreatMagicFireBallState*)m_pStateFactory->Create(ToGreatMagicFireBall, m_skillID);
						pNormalMagicFireBall->SetAttackDir(m_attackDir, m_attackDisplacement);

						m_pSkillStateMachine->Push(pNormalMagicFireBall);
					}


				}
			}
		}
	}
}


void FirehalfmoonState::Reset()
{

}
