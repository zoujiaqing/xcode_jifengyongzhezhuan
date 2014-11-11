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
#include "PhysicsLayer.h"
#include "BaseAttackState.h"

class SpriteSeer;

class RunState : public FState
{
public:
	RunState(BaseAttackEvt *pEvt);
	virtual ~RunState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

protected:
	/// set action run direction and animation
	unsigned int SetRunDir(bool bInMainLandLevel = true);

protected:

	BaseAttackEvt* m_pEvt;

	SpriteSeer *m_pRole;
	DIRECTION_ACTOR m_CurfaceDir;

	float m_fStartTime;
	bool  m_bPrepare;

	float m_fSoundTime;
};
