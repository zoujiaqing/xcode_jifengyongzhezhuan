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
#include "IdleState.h"
#include "SpriteSeer.h"
#include "LevelManager.h"
#include "SpriteElfManager.h"
#include "MainLandManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"

IdleState::IdleState(BaseAttackEvt *pEvt) : FState(FState::ExclusiveStates, NULL)
{
	m_Type = ToIdle;
	m_pRole = (SpriteSeer *) pEvt->m_pRole;
	
	m_pEvt = pEvt;

	m_HighPriority.push_back(ToRun);
}



IdleState::~IdleState( void )
{
	if(m_pEvt)
	{
		delete m_pEvt;
		m_pEvt = NULL;
	}
	

}


void IdleState::SetActorDirAndAnimation(bool bInMainLandLevel /* = true */)
{
	int actionID = 0;
	// init the animation
	DIRECTION_ACTOR faceDir = m_pRole->getDirection();
	switch(faceDir)
	{
	case DIRECTION_LEFT:
		if (bInMainLandLevel)
		{
			actionID = kTypeIdle_FaceHor_Stand_MAIN;
		}
		else
		{
			actionID = kTypeIdle_FaceHor_Stand_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);		 
		break;

	case DIRECTION_RIGHT:
		if (bInMainLandLevel)
		{
			actionID = kTypeIdle_FaceHor_Stand_MAIN;
		}
		else
		{
			actionID = kTypeIdle_FaceHor_Stand_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		m_pRole->SetAnimFlipX(true);
		break;

	case DIRECTION_UP:
		if (bInMainLandLevel)
		{
			actionID = kTypeIdle_FaceUp_Stand_MAIN;
		}
		else
		{
			actionID = kTypeIdle_FaceUp_Stand_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		break;

	case DIRECTION_BOTTOM:
		if (bInMainLandLevel)
		{
			actionID = kTypeIdle_FaceDown_Stand_MAIN;
		}
		else
		{
			actionID = kTypeIdle_FaceDown_Stand_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);
		break;

	default:
		if (bInMainLandLevel)
		{
			actionID = kTypeIdle_FaceDown_Stand_MAIN;
		}
		else
		{
			actionID = kTypeIdle_FaceDown_Stand_OTH;
		}
		m_pRole->SetAnim(actionID, 1, true);


		break;
	}

	//SpriteElfManager::Get()->PushOneElfChaseState(m_pRole,actionID);

	//CCLOG("Idle anim Id : %ld", m_pRole->GetAnimID() );

}

bool IdleState::Entered()
{
	CCLOG("IdleState::Entered()----------------------------------");
	if (false == FState::Entered())
		return false;

	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	SetActorDirAndAnimation(bMainLandLevel);
	
	// TODO : 多人副本是，其它其它玩家还没有进来的时候，主角已经能移动了。
	/*if (MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer")))
	{
		MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"))->closeBattleCoverLayer(NULL);
	}*/

	return true;
}

void IdleState::Exited()
{
	FState::Exited();
}

void IdleState::Update(float dt)
{
	FState::Update(dt);
}

void IdleState::Reset()
{

}