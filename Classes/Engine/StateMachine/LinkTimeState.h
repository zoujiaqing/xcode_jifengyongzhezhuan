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

class SpriteSeer;

class LinkTimeEvt : public FEvent
{
	friend class LinkTimeState;
public :
	LinkTimeEvt( FState * state, unsigned int SkillId, int partId)
	{
		this->m_Sender = state;
		this->m_EvtType = ToLinkTime;
		m_skillId = SkillId;
		m_partId = partId;
	}

	/// clone current normal attack event
	virtual FEvent* Clone()
	{
		LinkTimeEvt* pCloneEvent = new LinkTimeEvt(NULL, 0, 1);
		pCloneEvent->m_Sender = this->m_Sender;
		pCloneEvent->m_EvtType = this->m_EvtType;
		pCloneEvent->m_skillId = this->m_skillId;
		pCloneEvent->m_partId = this->m_partId;
		return pCloneEvent;
	}

protected:
	unsigned int m_skillId;
	unsigned int m_partId;
};


class LinkTimeState : public FState
{
public:
	LinkTimeState(CCObject * actor);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();
protected:
	virtual void ChangeAttackState(void);
	virtual void ChangeToIdleState(void);

protected:
	SpriteSeer *m_pRole;
	const SKILL_INFO* m_pSkill;
	float m_fSkillLinkTime;
	unsigned int m_skillID;
	unsigned int m_partID;
};