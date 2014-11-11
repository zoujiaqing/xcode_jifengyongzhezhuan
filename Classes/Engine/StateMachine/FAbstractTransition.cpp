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

#include "FAbstractTransition.h"


FAbstractTransition::FAbstractTransition(FState* sourceState)
{
	m_Sourcestate = sourceState;
	m_Machine = sourceState->m_Machine;
}

FStateMachine*  FAbstractTransition::Machine () {
	return m_Machine;
}

void  FAbstractTransition::SetTargetState ( FAbstractState*  target ) {
	m_Targetstate = target;
}

void  FAbstractTransition::SetTargetStates ( StateArrayList& targets ) {
	m_Targetlist.clear();
	m_Targetlist.assign(targets.begin(), targets.end());
}

FState* FAbstractTransition::SourceState (){
	return m_Sourcestate;
}

FAbstractState*   FAbstractTransition::TargetState () {
	return m_Targetstate;
}

StateArrayList& FAbstractTransition::TargetStates (){
	return m_Targetlist;
}