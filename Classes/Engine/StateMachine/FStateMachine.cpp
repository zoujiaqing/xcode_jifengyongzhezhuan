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
#include "FStateMachine.h"

bool FState::Reject()
{
	for (list<EventType>::iterator it = m_HighPriority.begin(); it != m_HighPriority.end(); it++)
	{
		EventType type = *it;
		if (m_Machine->GetState(type)->m_StateFlag == UPDATING)
		{
			m_ForceExit = true;
			m_ForceExitState = m_Machine->GetState(type);
			m_Machine->GetState(type)->m_ForceEnterState = this;
			return true;
		}
	}

	for (list<EventType>::iterator it = m_LowPriority.begin(); it != m_LowPriority.end(); it++)	   
	{
		EventType type = *it;
		if (m_Machine->GetState(type)->m_StateFlag == UPDATING)
		{
			m_Machine->GetState(type)->m_StateFlag = OVER;
			m_Machine->GetState(type)->m_ForceExit = true;
			m_ForceEnterState = m_Machine->GetState(type);
			m_Machine->GetState(type)->m_ForceExitState = this;
			m_ForceEnter = true;
		}
	}

	return false;
}