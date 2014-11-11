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
#include "BaseAttackState.h"

class SpriteSeer;

class IdleState : public FState
{
public:
	IdleState(BaseAttackEvt *pEvt);
	virtual ~IdleState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	/// set actor direction and animation
	void SetActorDirAndAnimation(bool bInMainLandLevel = true);
	
protected:
	SpriteSeer *m_pRole;

	BaseAttackEvt* m_pEvt;
};