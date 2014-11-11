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

#include "LinkTimeState.h"
#include "SpriteSeer.h"
#include "SkillDispMgr.h"
#include "IdleState.h"
#include "NormalAttackState.h"
#include "SpecialAttackState.h"

LinkTimeState::LinkTimeState(CCObject * actor) : FState(FState::ExclusiveStates, NULL)
{
	m_Type = ToLinkTime;
	m_pRole = (SpriteSeer *) actor;

	m_LowPriority.push_back(ToIdle);
	m_LowPriority.push_back(ToRun);
}

bool LinkTimeState::Entered()
{
	if (false == FState::Entered())
		return false;

	// do current skill 
	m_skillID = ((LinkTimeEvt* )(this->m_evtClone))->m_skillId;
	m_partID = ((LinkTimeEvt* )(this->m_evtClone))->m_partId;

	m_pSkill = SkillDispMgr::Get()->GetSkillInfo(m_skillID);
	CCAssert(m_pSkill != NULL, "m_pSkill is null");

	m_fSkillLinkTime = m_pSkill->linkTime;

	return true;
}

void LinkTimeState::Exited()
{
	FState::Exited();
}

void LinkTimeState::Update(float dt)
{
	m_fSkillLinkTime -= dt;

	bool bToIdle = true;

	if(m_fSkillLinkTime <= 0.0f)
	{
		if(m_pSkill->type == 1)
		{
			if(m_pSkill->linkSkillId != 0)
			{
				Exited();
				ChangeAttackState();
				bToIdle = false;
			}
		}
		else
		{
			int nextPartID = m_partID + 1;

			if(m_pSkill->type == 0 && nextPartID > m_pRole->GetNormalAttackNum())
			{
				m_pRole->tellIsAttackEnable();
			}

			if((nextPartID <= m_pRole->GetNormalAttackNum() || m_pSkill->type == 1 ) && m_pSkill->linkSkillId != 0)
			{
				bToIdle = false;
				Exited();
				ChangeAttackState();

				m_fSkillLinkTime = 0.0f;

			}
		}
		
	}
	else
	{
		bToIdle = false;
	}

	

	if (bToIdle)
	{
		// state over, exit normal attack state
		Exited();

		ChangeToIdleState();
	}
}

void LinkTimeState::Reset()
{

}

void LinkTimeState::ChangeAttackState( void )
{
	//int nextPartID = m_partID + 1;
	//if(m_pSkill->type == 0)
	//{
	//	NormalAttackState* pNormalAttackState =  (NormalAttackState*) (m_Machine->GetState(ToNormalAttack));
	//	NormalAttackEvt *pAttackevt = new NormalAttackEvt(pNormalAttackState, m_pSkill->linkSkillId, nextPartID);
	//	m_Machine->PostEvent(pAttackevt, FStateMachine::HighPriority);
	//}
	//else if(m_pSkill->type == 1)
	//{
	//	SpecialAttackState* pAttackState =  (SpecialAttackState*) (m_Machine->GetState(ToSpecialAttack));
	//	SpecialAttackEvt *pAttackevt = new SpecialAttackEvt(pAttackState, m_pSkill->linkSkillId, nextPartID);
	//	m_Machine->PostEvent(pAttackevt, FStateMachine::HighPriority);
	//}
	//else
	//{
	//	// TODO : 应该用 工厂模式来设计。
	//	ChangeToIdleState();
	//}
	
}

void LinkTimeState::ChangeToIdleState( void )
{
	// to idle state
	//IdleState* idle =  (IdleState*) (m_Machine->GetState(ToIdle));
	//IdleEvt *pIdleevt = new IdleEvt(idle);
	//m_Machine->PostEvent(pIdleevt, FStateMachine::HighPriority);
	//if(m_pSkill->type == 0)
	//	m_pRole->tellIsAttackEnable();
}
