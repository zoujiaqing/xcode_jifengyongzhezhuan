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

#include "RunState.h"
#include "SpriteSeer.h"
#include "GameAudioManager.h"
#include "HSJoystick.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "SpriteElfManager.h"
#include "LevelLayer.h"

#include "LevelLayer.h"
#include "MainLandManager.h"
#include "CommonEnum.h"

static float g_soudTime = 1.0;
static int g_soundId = SOUND_EFFECT_WALK;
RunState::RunState(BaseAttackEvt *pEvt) :  FState(FState::ExclusiveStates, NULL)
	, m_fStartTime(0)
	, m_bPrepare(false)
	, m_CurfaceDir(DIRECTION_UNKNOW)
{
	m_Type = ToRun;
	m_pRole = (SpriteSeer *) pEvt->m_pRole;

	m_pEvt = pEvt;

	m_LowPriority.push_back(ToIdle);
}


RunState::~RunState( void )
{
	if(m_pEvt)
	{
		delete m_pEvt;
		m_pEvt = NULL;
	}
}


unsigned int RunState::SetRunDir(bool bInMainLandLevel /* = true */)
{
	int actionID = 0;
	// 判断是否在主城中
	m_CurfaceDir = m_pRole->getDirection();
	switch(m_CurfaceDir)
	{
	case DIRECTION_LEFT:
		if (bInMainLandLevel)
		{
			actionID = kTypeRun_FaceHor_MAIN;
		}
		else
		{
			actionID = kTypeRun_FaceHor_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);		 
		break;

	case DIRECTION_RIGHT:
		if (bInMainLandLevel)
		{
			actionID = kTypeRun_FaceHor_MAIN;
		}
		else
		{
			actionID = kTypeRun_FaceHor_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		m_pRole->SetAnimFlipX(true);
		break;

	case DIRECTION_UP:
		if (bInMainLandLevel)
		{
			actionID = kTypeRun_FaceUp_MAIN;
		}
		else
		{
			actionID = kTypeRun_FaceUp_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		break;

	case DIRECTION_BOTTOM:
		if (bInMainLandLevel)
		{
			actionID = kTypeRun_FaceDown_MAIN;
		}
		else
		{
			actionID = kTypeRun_FaceDown_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		break;

	default:
		if (bInMainLandLevel)
		{
			actionID = kTypeRun_FaceHor_MAIN;
		}
		else
		{
			actionID = kTypeRun_FaceHor_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		break;
	}
	return actionID;
	//SpriteElfManager::Get()->PushOneElfCloseToState(m_pRole,actionID);
}

bool RunState::Entered()
{
	if (false == FState::Entered()) 
		return false;

	//CCLOG("RunState::Entered()----------------------------------");
	// init the animation
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	unsigned int actionID = SetRunDir(bMainLandLevel);
	m_fStartTime = 0.0f;
	m_bPrepare = true;

    int roleType = GameManager::Get()->getHero()->getType();
    if(roleType == kTypeAssassin_Boy || roleType == kTypeAssassin_Girl)
    {
        g_soundId = SOUND_EFFECT_WALK_ASSASSIN;
        g_soudTime = 2.1f;
    }
    else
    {
        g_soundId = SOUND_EFFECT_WALK;
        g_soudTime = 2.2f;
    }
    
	m_fSoundTime = g_soudTime;

	m_pRole->SetSpeed(0.01f);
	if(m_pRole->getTag() == MAP_TAG_SEER)
	{
		//if(!GameAudioManager::sharedManager()->isEffectIsPlaying(321009))
			GameAudioManager::sharedManager()->playEffect(g_soundId, false);

	}
	
	SpriteElfManager::Get()->PushOneElfCloseToState(m_pRole,actionID);

	return true;
}

void RunState::Exited()
{
	FState::Exited();
	m_pRole->SetSpeed(1.0f);
	m_bPrepare = true;
	if(m_pRole->getTag() == MAP_TAG_SEER)
	{
		//if(GameAudioManager::sharedManager()->isEffectIsPlaying(321009))
			GameAudioManager::sharedManager()->stopEffect(g_soundId);
	}
	//CCLOG("RunState::Exited()");
}

void RunState::Update(float dt)
{
	FState::Update(dt);

	if(m_pRole->getTag() == MAP_TAG_SEER)
	{
		m_fSoundTime -= dt;
		if(m_fSoundTime <= 0.0f)
		{
			GameAudioManager::sharedManager()->playEffect(g_soundId, false);
			m_fSoundTime = g_soudTime;
		}
	}
	

	// accumulate time 
	m_fStartTime += dt;

	if (m_fStartTime > 0.1f && m_bPrepare)
	{
		m_pRole->SetSpeed(1.0f);
		m_bPrepare = false;
	}
	else
	{
		if (m_pRole->getDirection() != m_CurfaceDir)
		{
			bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());

			SetRunDir(bMainLandLevel);
		}
	}
		
	if (MOVE_BY_JOY == m_pRole->GetMoveType())
	{
		m_pRole->moveByJoystick(dt);
	}
	
}

void RunState::Reset()
{

}

