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

#include <list>
#include <algorithm>
#include "cocos2d.h"

#include "StateEventType.h"
#include "FStateMachine.h"

using namespace std;
USING_NS_CC;

class FStateMachine;
class FState;
class FAbstractState;


class FAbstractTransition
{
public:
	FAbstractTransition(FState* sourceState);

	FStateMachine*  Machine ();


	void  SetTargetState ( FAbstractState*  target );

	void  SetTargetStates ( StateArrayList& targets );

	FState* SourceState ();

	FAbstractState*   TargetState ();

	StateArrayList& TargetStates ();

protected:
	FState* m_Sourcestate;
	FAbstractState* m_Targetstate;
	StateArrayList m_Targetlist;
	FStateMachine* m_Machine;
};