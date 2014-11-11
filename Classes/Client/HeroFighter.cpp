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

#include "HeroFighter.h"
#include "BaseAttackState.h"
#include "SkillDispMgr.h"

HeroFight::HeroFight(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet, bool isCreateEffect)
	:RoleBase(s_id, name, type, isPlayerSet, isCreateEffect)
{

}

HeroFight::~HeroFight()
{

}

// --------------------------------------- OtherHeroFight ---------------------------------------------------------

OtherHeroFight::OtherHeroFight(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet, bool isCreateEffect /*= true*/ )
	:RoleBase(s_id, name, type, isPlayerSet, isCreateEffect)
{

}

OtherHeroFight::~OtherHeroFight()
{

}

cocos2d::CCPoint OtherHeroFight::GetJoystickVelocity( void )
{
	return m_JoystickVelocity;
}

bool OtherHeroFight::DoNormalAttack(unsigned int skillId , const CCPoint& rolePos,  const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster)
{
	m_pStateMachine->Remove(ToNormalAttack);
	m_pStateMachine->Remove(ToRun);
	m_pStateMachine->Remove(ToIdle);

	m_nNormalAttackNum = 1;

	FState* pState = m_pStateFactory->Create(ToNormalAttack, skillId);
	StateContainer* pContainer = dynamic_cast<StateContainer*>(pState);
	if(pContainer)
	{
		SkillBeginReceiverHandler* pRecevier = dynamic_cast<SkillBeginReceiverHandler*>(m_pStateFactory->Create(ToSkillBeginReceiverHandler, skillId));
		if(pRecevier)
		{
			pRecevier->SetAimMonster(bAnimMonster);
			pRecevier->SetDirection(aimDir);
			pRecevier->SetNearestMonsterPos(monsterPos);
            pRecevier->SetRolePos(rolePos);
			pRecevier->SetRoleDirection(roleDir);
            
			pContainer->Push(pRecevier);

		}
	}
	m_pStateMachine->Push(pState);

	return true;
}

bool OtherHeroFight::DoSkillAttack( unsigned int skillId , const CCPoint& rolePos,  const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster)
{
	m_pStateMachine->Remove(ToRun);
	m_pStateMachine->Remove(ToIdle);
	m_pStateMachine->Remove(ToNormalAttack);

	const SKILL_INFO* pSkillInfo = SkillDispMgr::Get()->GetSkillInfo(skillId);

	FState* pState = m_pStateFactory->Create(pSkillInfo->type, skillId);
	StateContainer* pContainer = dynamic_cast<StateContainer*>(pState);
	if(pContainer)
	{
		SkillBeginReceiverHandler* pRecevier = dynamic_cast<SkillBeginReceiverHandler*>(m_pStateFactory->Create(ToSkillBeginReceiverHandler, skillId));
		if(pRecevier)
		{
			pRecevier->SetAimMonster(bAnimMonster);
			pRecevier->SetDirection(aimDir);
			pRecevier->SetNearestMonsterPos(monsterPos);
            pRecevier->SetRolePos(rolePos);
            pRecevier->SetRoleDirection(roleDir);

			pContainer->Push(pRecevier);

		}
	}
	m_pStateMachine->Push(pState);

	return true;
}


