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

#include "DieState.h"
#include "SpriteSeer.h"
#include "GameAudioManager.h"
#include "HSJoystick.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "SpriteElfManager.h"
#include "LevelLayer.h"

#include "LevelLayer.h"
#include "MainLandManager.h"
#include "PvAIManager.h"

DieState::DieState(BaseAttackEvt *pEvt) :  FState(FState::ExclusiveStates, NULL)
	, m_CurfaceDir(DIRECTION_UNKNOW)
	, slowTimer(-2)
{
	m_Type = ToDie;
	m_pRole = (SpriteSeer *) pEvt->m_pRole;

	m_pEvt = pEvt;

	m_LowPriority.push_back(ToIdle);
}


DieState::~DieState( void )
{
	if(m_pEvt)
	{
		delete m_pEvt;
		m_pEvt = NULL;
	}
}


unsigned int DieState::PlayAnim(void)
{
	int actionID = 0;
	// 判断是否在主城中
	m_CurfaceDir = m_pRole->getDirection();
	switch(m_CurfaceDir)
	{
	case DIRECTION_LEFT:
		actionID = kTypeDie_FaceHor;
		m_pRole->SetAnim(actionID, 1, false);		 
		break;

	case DIRECTION_RIGHT:

		actionID = kTypeDie_FaceHor;
		m_pRole->SetAnim(actionID, 1, false);
		m_pRole->SetAnimFlipX(true);
		break;

	case DIRECTION_UP:
		
		actionID = kTypeDie_FaceUp;
		m_pRole->SetAnim(actionID, 1, false);
		break;

	case DIRECTION_BOTTOM:
		
		actionID = kTypeDie_FaceDown;
		m_pRole->SetAnim(actionID, 1, false);
		break;

	default:
		
		actionID = kTypeDie_FaceHor;
		m_pRole->SetAnim(actionID, 1, false);
		break;
	}

	m_pRole->SetAnimPauseOnOver(true);

	return actionID;
}

bool DieState::Entered()
{
	if (false == FState::Entered())
		return false;

	CCLOG("DieState::Entered()----------------------------------");

	// init the animation

	if (PvAIManager::Get()->getLastAIPlayer() == m_pRole)
	{
		slowTimer = 0;
		CCDirector::sharedDirector()->getScheduler()->setTimeScale(0.2f);
	}


	unsigned int actionID = PlayAnim();

	return true;
}

void DieState::Exited()
{
	FState::Exited();
}

void DieState::Update(float dt)
{
	FState::Update(dt);
	if (slowTimer > -1 && PvAIManager::Get()->getLastAIPlayer() == m_pRole)
	{
		slowTimer += dt;
		if (slowTimer > 0.2f)
		{
			CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
			slowTimer = -2;
		}		
	}
}

void DieState::Reset()
{

}    

