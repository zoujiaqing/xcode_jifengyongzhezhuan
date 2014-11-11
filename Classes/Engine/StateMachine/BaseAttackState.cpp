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

#include "OnlineNetworkManager.h"
#include "BaseAttackState.h"
#include "SpriteSeer.h"
#include "SkillDispMgr.h"
#include "IdleState.h"
#include "LinkTimeState.h"
#include "AttackChecker.h"
#include "UserData.h"
#include "cs_basic.pb.h"
#include "cs_core.pb.h"
#include "cs_battle.pb.h"
#include "LevelManager.h"
#include "HSJoystick.h"
#include "GameManager.h"
#include "PhysicsLayer.h"
#include "LevelLayer.h"
#include "ParticleManager.h"
#include "NormalAttackState.h"
#include "SpriteMonsterMgr.h"
#include "GameAudioManager.h"
#include "SpriteMonster.h"

#include "IdleState.h"
#include "RunState.h"
#include "DieState.h"
#include "NormalAttackState.h"
#include "EffectSprite.h"
#include "ParticleManager.h"
#include "LinkTimeState.h"
#include "SpecialAttackState.h"
#include "IronDefenseState.h"
#include "SkillDispMgr.h"
#include "InvincibilityState.h"
#include "ApocalypseState.h"
#include "TeleportState.h"
#include "GreatFireBallState.h"
#include "CalvingState.h"
#include "ChopseriesState.h"
#include "FirehalfmoonState.h"
#include "IceGroundState.h"
#include "WhirlstormState.h"
#include "ExtremeSpeedState.h"
#include "AssassinationState.h"
#include "FatalDarkKnivesState.h"
#include "InvisibleKillerState.h"
#include "ElfChaseState.h"
#include "ElfCloseToState.h"
#include "ElfSkillAttackState.h"
#include "EnginePlayerManager.h"
#include "ElfBaseState.h"
#include "CameraController.h"
#include "MathUtil.h"
#include "PvAIManager.h"

BaseAttackState::BaseAttackState(BaseAttackEvt* pEvt) : FState(FState::ExclusiveStates, NULL)
	, m_pEvt(pEvt)
	, m_pContainer(NULL)
	, m_pSkillStateMachine(NULL)
{
	m_Type = ToBaseAttack;
	
	m_skillID = pEvt->m_skillId;
	
	m_pRole = (SpriteSeer *)pEvt->m_pRole;
	m_pSkill = SkillDispMgr::Get()->GetSkillInfo(pEvt->m_skillId);
	m_partID = 1;
	if(m_pSkill)
		m_partID = m_pSkill->partID;
	 m_Elf = (SpriteElf *)NULL;
	 attackType = 0  ;
	m_pStateFactory = new StateFactory(m_pRole);

}


BaseAttackState::~BaseAttackState( void )
{
	if(m_pEvt != NULL)
	{
		delete m_pEvt;
		m_pEvt = NULL;
	}

	if(m_pStateFactory != NULL)
	{
		delete m_pStateFactory;
		m_pStateFactory = NULL;
	}
}


bool BaseAttackState::Entered()
{
	if (false == FState::Entered())
		return false;

	// init the animation
	if(m_evtClone)
	{
		// do current skill 
		m_skillID = ((BaseAttackEvt* )(this->m_evtClone))->m_skillId;
		m_partID = ((BaseAttackEvt* )(this->m_evtClone))->m_partId;
	

		m_pSkill = SkillDispMgr::Get()->GetSkillInfo(m_skillID);
	}

	//CCAssert(m_pSkill != NULL, "m_pSkill is null");
	

	return true;
}

void BaseAttackState::Exited()
{
	FState::Exited();
}

void BaseAttackState::Update(float dt)
{
	FState::Update(dt);
}


void BaseAttackState::Reset()
{
}

void BaseAttackState::ChangeToIdleState( void )
{
	if(!m_pSkillStateMachine->CheckStateRuning(ToIdle))
	{
		m_pSkillStateMachine->Push(m_pStateFactory->Create(ToIdle, m_skillID));
		m_pSkillStateMachine->Remove(this);
	}
	
}

void BaseAttackState::SetContainer( StateContainer* pContainer )
{
	m_pContainer = pContainer; 
	BaseAttackState* pState = dynamic_cast<BaseAttackState*>(pContainer);
	if(pState)
	{
		if(pState->GetStateMachine())
		{
			SetStateMachine(pState->GetStateMachine());
		}
	}
}

bool BaseAttackState::CanIdleState(void)
{
	bool ret = true;
	for(unsigned int i = (unsigned int)ToNormalAttack; i < (unsigned int)ToLinkTimeHandler; i++)
	{
		if(m_pSkillStateMachine)
		{
			EventType t = (EventType)i;
			if(t != m_pSkill->type)
			{

				if(m_pSkillStateMachine->CheckStateRuning(t))
				{
					ret = false;
					break;
				}
			}
		}
	}

	return ret;
}

// -----------------------------------------------------------------------------------------------------------

void StateContainer::Push( FState* pState )
{ 
	m_states.push_back(pState);
}

void StateContainer::Remove( FState* pState )
{
	m_states.remove(pState);
	delete pState;

}

StateContainer::~StateContainer( void )
{
	for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); iter++ )
	{
		FState* pState = *iter;
		if(pState)
		{
			delete pState;
		}
	}

	m_states.clear();
}

FState* StateContainer::GetState( EventType type )
{
	for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); iter++)
	{
		FState* pState = (*iter);
		if(pState)
		{
			if(pState->m_Type == type)
			{
				if(pState->m_StateFlag == STOP || pState->m_StateFlag == UPDATING)
					return pState;
			}
		}
	}

	
	return NULL;
}

void StatePoller::Update( float dt )
{
	for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); iter++)
	{
		FState* pState = (*iter);
		if(pState)
		{
			switch(pState->m_StateFlag)
			{
			case STOP:
				pState->Entered();
				break;
			case UPDATING:
				pState->Update(dt);
				break;
			//case OVER:
				//pState->Exited();
				//break;
			default:
				break;
			}
		}
		
	}

	for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); )
	{
		FState* pState = *iter;
		if(pState)
		{
			if(pState->m_StateFlag == OVER)
			{
				m_states.erase(iter++);
				delete pState;
				pState = NULL;
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}

}

StatePoller::~StatePoller( void )
{
	Exit();
}

void StatePoller::Exit( void )
{
	for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); iter++)
	{
		FState* pState = (*iter);
		if(pState)
		{
			Remove(pState);
		}
		
	}
}

void StatePoller::Push( FState* pState )
{
	if(pState == NULL)
		return;

	StateContainer::Push(pState);
	BaseAttackState* pBaseAttackState = dynamic_cast<BaseAttackState*>(pState);
	if(pBaseAttackState != NULL)
	{
		pBaseAttackState->SetContainer(this);
	}

	ElfBaseState* pElfBaseAttackState = dynamic_cast<ElfBaseState*>(pState);
	if(pElfBaseAttackState != NULL)
	{
		pElfBaseAttackState->SetContainer(this);
	}

	if(pState->m_StateFlag == STOP)
	{
		pState->Entered();
	}
}

void StatePoller::Remove( FState* pState )
{
	if(pState == NULL)
		return;

	switch(pState->m_StateFlag)
	{
	case UPDATING:
		pState->Exited();
		
		break;
	default:
		break;
	}

	pState->m_StateFlag = OVER;
}

void StatePoller::Remove( EventType type )
{
	for(std::list<FState*>::iterator iter = m_states.begin(); iter != m_states.end(); iter++)
	{
		FState* pState = (*iter);
		if(pState)
		{
			if(pState->m_Type == type)
			{
				Remove(pState);
			}
		}

	}
}


void SkillStateMachine::Push( FState* pState )
{
	if(pState == NULL)
		return;
	BaseAttackState* pBaseAttackState = dynamic_cast<BaseAttackState*>(pState);
	if(pBaseAttackState != NULL)
	{
		pBaseAttackState->SetStateMachine(this);
	}

	ElfBaseState* pElfBaseAttackState = dynamic_cast<ElfBaseState*>(pState);
	if(pElfBaseAttackState != NULL)
	{
		pElfBaseAttackState->SetStateMachine(this);
	}

	StatePoller::Push(pState);
	
}

FState* StateFactory::Create( EventType type, unsigned int skillID )
{
	FState* pState = NULL;

	switch(type)
	{
	case ToNormalAttack:
		pState = new NormalAttackState( new BaseAttackEvt(m_pRole, skillID ) );
		break;
	case ToTeleport:
		pState = new TeleportState( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToAnimHandler:
		pState = new AnimHandler( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToAimDirectionHandler:
		pState = new AimDirectionHandler( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToMagicAnimHandler:
		pState = new MagicAnimHandler( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToAttackCheckerHandler:
		pState = new AttackCheckerHandler( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToMagicCheckHandler:
		pState = new MagicCheckerHandler( new BaseAttackEvt( m_pRole, skillID ) );
		//CCAssert(0);
		break;
	case ToAttackSenderHandler:
		pState = new AttackSenderHandler( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToEffectAnimHandler:
		pState = new EffectAnimHandler( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToRoleEffectHandler:
		pState = new RoleEffectHandler( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToLinkTimeHandler:
		pState = new LinkTimeHandler( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToNormalMagicFireBall:
		pState = new NormalMagicFireBallState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToSpecialAttack:
		pState = new SpecialAttackState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToIronDefense:
		pState = new IronDefenseState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToInvincibility:
		pState = new InvincibilityState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToApocalypse:
		pState = new ApocalypseState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToBlackHole:
		pState = new ApocalypseState( new BaseAttackEvt( m_pRole, skillID ) );	
		if(dynamic_cast<BaseAttackState*>(pState))
		{
			dynamic_cast<BaseAttackState*>(pState)->SetEventType(ToBlackHole);
		}
		break;
	case ToGreatFireBall:
		pState = new GreatFireBallState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToGreatMagicFireBall:
		pState = new GreatMagicFireBallState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToCalving:
		pState = new CalvingState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToCalvingMagic:
		pState = new CalvingMagicState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToChopseries:
		pState = new ChopseriesState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToExtremeSpeed:
		pState = new ExtremeSpeedState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToAssassination:
		pState = new AssassinationState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToFatalDarkKnives:
		pState = new FatalDarkKnivesState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToInvisibleKiller:
		pState = new InvisibleKillerState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToReviveInvincible:
		pState = new ReviveInvincibleState( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToWhirlwind:
	case ToStepSword:
	case ToEarthSplitter:
		{
			pState = new SpecialAttackState( new BaseAttackEvt( m_pRole, skillID ) );
			BaseAttackState* pBaseAttackState = dynamic_cast<BaseAttackState*>(pState);
			if(pBaseAttackState)
			{
				pBaseAttackState->SetEventType(type);
			}
		}
		
		break;
	case ToSkillBeginSenderHandler:
		pState = new SkillBeginSenderHandler( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToSkillBeginReceiverHandler:
		pState = new SkillBeginReceiverHandler( new BaseAttackEvt( m_pRole, skillID ) );		
		break;
	case ToMoveToHandler:
		pState = new MoveToHandler( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToVariableSpeedMoveToHandler:
		pState = new VariableSpeedMoveToHandler( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToWhirlstorm:
		pState = new WhirlstormState( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToIceGround:
		pState = new IceGroundState( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToIceGroundMagic:
		pState = new IceGroundMagicState( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToFirehalfmoon:
		pState = new FirehalfmoonState( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	case ToIdle:
		pState = new IdleState( new BaseAttackEvt( m_pRole, skillID ) );

		break;
	case ToRun:
		pState = new RunState( new BaseAttackEvt( m_pRole, skillID ) );

		break;
	case ToDie:
		pState = new DieState( new BaseAttackEvt( m_pRole, skillID ) );
		break;
	default:
		pState = new IdleState( new BaseAttackEvt( m_pRole, skillID ) );

		break;
	}

	return pState;
}

FState* StateFactory::Create( BaseAttackEvt* pEvt )
{
	FState* pState = NULL;

	switch(pEvt->GetEventType())
	{
	case ToMagicCheckHandler:
		pState = new MagicCheckerHandler( pEvt );
		break;
	}

	return pState;
}

StateFactory::StateFactory( SpriteSeer* pRole )
	: m_pRole(pRole)
{

}


FState* ElfStateFactory::Create(EventType type, SpriteSeer* pHero,unsigned int skillID,BaseElfAttachData* pData /* = 0 */)
{
	FState* pState = NULL;

	unsigned int normalSkillId = SkillDispMgr::Get()->GetFairyNormalSkillID(m_pRole->GetTypeId());
	unsigned int assistSkillId = SkillDispMgr::Get()->GetFairyAssistSkillID(m_pRole->GetTypeId());

	switch(type)
	{
	case ToElfChase:
		pState = new ElfChaseState(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToElfCloseTo:
		pState = new ElfCloseToState(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToElfSkillAttack:
		pState = new ElfSkillAttackState(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToElfAnimHandler:
		pState = new ElfAnimHandler(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToElfAimDirectionHandler:
		pState = new ElfAimDirectionHandler(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToElfEffectAnimHandler:
		pState = new ElfEffectAnimHandler(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToElfAttackTrigger:
		pState = new ElfAttackTrigger(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToAssistElfAttackTrigger:
		pState = new AssistElfAttackTrigger(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	case ToAssistElfAttack:
		pState = new AssistElfSkillAttackState(new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData));
		break;
	default:
		pState = new ElfChaseState( new BaseElfEvt( m_pRole, pHero,skillID ,normalSkillId,assistSkillId,pData) );

		break;
	}

	return pState;
}

FState* ElfStateFactory::Create( BaseAttackEvt* pEvt )
{
	FState* pState = NULL;

	return pState;
}

ElfStateFactory::ElfStateFactory( SpriteElf* pRole )
	: m_pRole(pRole)
{

}

// --------------------------------------------------------------------------------------------------------
AnimHandler::AnimHandler( BaseAttackEvt* pEvt  )
	:BaseAttackState(pEvt)
	, m_bAnimMonster(false)
{
	m_Type = ToAnimHandler;
}

bool AnimHandler::Entered()
{
	BaseAttackState::Entered();

	const unsigned int VOICE[7][3] =
	{
		{0, 0, 0},

		{3211001, 3211002, 3211003},
		{3211201, 3211202, 3211203},

		{3211301, 3211302, 3211303},
		{3211401, 3211402, 3211403},

		{3211501, 3211502, 3211503},
		{3211601, 3211602, 3211603},
	};

	m_pMoveToHandler = NULL;
	m_bAnimMonster = false;
	m_effectDisplacement = m_pSkill->longDistanceAttackRange;
	m_displacement = m_pSkill->displacement;

	if(m_pSkill->SoundId > 0)
	{
		//if(!GameAudioManager::sharedManager()->isEffectIsPlaying(m_pSkill->SoundId))
			GameAudioManager::sharedManager()->playEffect(m_pSkill->SoundId, false);
			/*if(m_pSkill->bSoundLoop && m_pSkill->SoundId == 321012)
			{
				m_fSoundTime = 0.3f;
			}*/
	}

	if(m_pRole->getTag() == MAP_TAG_SEER)
	{
		if(m_pSkill->type == ToNormalAttack)
		{
			unsigned int voiceID = 0;
			unsigned int index = ranged_random(0, 5);
			if(index < 3)
				voiceID = VOICE[m_pRole->getType()][index];
			if(voiceID > 0)
				GameAudioManager::sharedManager()->playEffect(voiceID, false);
		}
		else
		{
			unsigned int voiceID = 0;
			if(m_pRole->getType() % 2 == 1)
			{
				voiceID = m_pSkill->boyVoiceID;
			}
			else
			{
				voiceID = m_pSkill->girleVoiceID;
			}

			if(voiceID > 0)
				GameAudioManager::sharedManager()->playEffect(voiceID, false);
			
		}
	}
	
	

	m_moveToResult.destPos.setPoint(0.0f, 0.0f);
	m_moveToResult.pAction = NULL;

	if(m_pSkill->bBuff)
	{
		if(m_pSkill->canControl)
		{
			m_pRole->setCanMove(true);
//			m_pRole->SetCanNormalAttack(true); //fix BUG #100250
		}
		else
		{
			m_pRole->setCanMove(false);
//			m_pRole->SetCanNormalAttack(false); //fix BUG #100250
		}
		
	}
	else
	{
		m_pRole->setCanMove(false);
		m_pRole->SetCanNormalAttack(false);
	}
    
	m_pRole->setVisible(true);
	m_bStartLinkTime = false;
	m_bMoving = false;
    
	if(m_pSkill->iShakeCamera == 1 && m_pRole->getTag() == MAP_TAG_SEER)
	{
		float scaleTime = 0.0f;
		CameraController::sharedCameraController()->shakeAndScale(scaleTime);
	}
	
	m_fContinueTime = 0.0f;
	
	ClientUserSkillInfo info;
	if(SkillDispMgr::Get()->GetOneSkillInfoById(m_pRole->GetID(), m_skillID, info))
	{
		if(m_pSkill->partID == info.parts[0].part)
			m_fContinueTime = info.parts[0].continue_time;
	}

	if(m_pSkill->bBuff && m_pSkill->roleType == kType_ALL_ROLE)
	{
		m_fContinueTime = m_pSkill->continueTime;
	}

	FState* pState = m_pContainer->GetState(ToAimDirectionHandler);

	if(m_pSkill->canControl)
	{
		if(pState)
		{
			m_pContainer->Remove(pState);
			pState = NULL;
		}
	}

	if(!pState)
	{
		pState = m_pStateFactory->Create(ToAimDirectionHandler, m_skillID);
		m_pContainer->Push(pState);
	}

	AimDirectionHandler* pAimDirection = dynamic_cast<AimDirectionHandler*>(pState);
	if(pAimDirection)
	{
		m_bAnimMonster = pAimDirection->IsAimMonster();
		m_aimDir = pAimDirection->GetDirection();
		m_nearestMonsterPos = pAimDirection->GetNearestMonsterPos();

	}

	SetNormalAttackDir();

	//CCLOG("AnimHandler anim Id : %ld , skillID : %ld, time : %u", m_pRole->GetAnimID(), m_skillID, time(NULL) );

	return true;
}



void AnimHandler::Update( float dt )
{
	BaseAttackState::Update(dt);

	if(m_pSkill == NULL)
	{
		ChangeToLinkTime();
		return;
	}

	/*if(m_pSkill->bSoundLoop && m_pSkill->SoundId == 321012)
	{
		m_fSoundTime -= dt;
		if(m_fSoundTime <= 0.0f)
		{
			GameAudioManager::sharedManager()->playEffect(m_pSkill->SoundId, false);
			m_fSoundTime = 0.3f;
		}
	}*/

	/*if(m_bMoving)
	{
	if(m_moveToResult.pAction != NULL && m_moveToResult.tag != 0)
	{
	if(!m_pRole->getRoot()->getActionByTag(m_moveToResult.tag))
	{
	m_bMoving = false;
	}
	}
	}*/
	
	if(m_pSkill->canControl)
		UpdateDirectionByJoystick(dt);


	m_pRole->setVisible(!m_pSkill->bDisappear);




	if(m_pSkill->animation[DIRECTION_LEFT].animationID != (unsigned int)-1 &&
		m_pSkill->animation[DIRECTION_LEFT].animationID != 0)
	{
		ANIMATION_STATE aniState = m_pRole->getCurAnimationState();
		switch(aniState)
		{
		case kType_COMPLETE:
			m_bStartLinkTime = true;
			break;
		default:
			break;
		}
	}


	m_fContinueTime -= dt;

	float fContinueTime = 0.0f;
	ClientUserSkillInfo info;
	if(SkillDispMgr::Get()->GetOneSkillInfoById(m_pRole->GetID(), m_skillID, info))
	{
		if(m_pSkill->partID == info.parts[0].part)
			fContinueTime = info.parts[0].continue_time;
	}

	if(m_pSkill->roleType == kType_ALL_ROLE && m_fContinueTime <= 0.0f && m_pSkill->bBuff)
	{
		ChangeToLinkTime();
		return;
	}
	else if(fContinueTime > 0.0f && m_fContinueTime <= 0.0f)
	{
		if(!m_bMoving)
		{
			ChangeToLinkTime();
			return;
		}
	
	}
	else if(m_bStartLinkTime)
	{
		if(m_fContinueTime > 0.0f)
		{
			m_bStartLinkTime = false;
			SetNormalAttackDir();
		}
		else
		{
			if(m_bMoving)
			{
				m_bStartLinkTime = false;
				SetNormalAttackDir();
			}
			else
			{
				ChangeToLinkTime();
				return;
			}
		}
	}




	if(m_pSkill->canControl)
	{	
		if(m_animationID != -1 && m_animationID != 0)
		{
			m_pSkillStateMachine->Remove(ToRun);
			switch(m_pRole->GetMoveType())
			{
			case MOVE_BY_JOY:
				m_pRole->moveByJoystick(dt);
				break;
			case MOVE_BY_MOUSE:
				m_pRole->setCanMove(true);
				break;
			}
		}
			
	}


	if(m_pMoveToHandler)
	{
		MoveToHandler* pMoveTo = dynamic_cast<MoveToHandler*>(m_pMoveToHandler);
		if(pMoveTo)
		{
			if(pMoveTo->IsFinish())
			{
				m_bMoving = false;

				m_pContainer->Remove(pMoveTo);
				m_pMoveToHandler = NULL;
			}
		}
	}
}

void AnimHandler::Exited()
{
	BaseAttackState::Exited();
	if(m_moveToResult.pAction != NULL)
	{
		m_pRole->getRoot()->stopAction(m_moveToResult.pAction);
		m_moveToResult.pAction = NULL;
	}


	m_pRole->setCanMove(true);
	//fix BUG #100250, not buff skill
	if(!m_pSkill->bBuff)
	{
		m_pRole->SetCanNormalAttack(true);
	}
	m_pRole->setVisible(true);

	if (m_pSkill->bSoundLoop)
	{
		//if(GameAudioManager::sharedManager()->isEffectIsPlaying(m_pSkill->SoundId))
			GameAudioManager::sharedManager()->stopEffect(m_pSkill->SoundId);
	}
}

void AnimHandler::SetNormalAttackDir()
{
	if(m_pSkill == NULL)
		return;


	m_displacement = m_pSkill->displacement;

	if(m_pSkill->aim == SKILL_INFO::E_AIM_ENEMY && !m_bMoving)
	{	
		m_pRole->setDirection(m_aimDir, 1.0f);
	}

	DIRECTION_ACTOR faceDir = m_pRole->getDirection();

	const tagSkillInfo::ANIMATION_INFO* pAnimation = NULL;

	pAnimation = &(m_pSkill->animation[faceDir]);

	m_animationID = pAnimation->animationID;

	if(m_animationID > 0)
	{
		m_pRole->SetAnim(pAnimation->animationID, 1, true);
		m_pRole->SetAnimFlipX(pAnimation->isFlipX);
		m_pRole->SetAnimPauseOnOver(true);
	}
	else
	{	
		if(CanIdleState())
		{
			if(m_pSkillStateMachine->CheckStateRuning(ToMoveToHandler) || m_pSkillStateMachine->CheckStateRuning(ToVariableSpeedMoveToHandler))
			{
				if(m_pSkillStateMachine->CheckStateRuning(ToIdle))
				{
					m_pSkillStateMachine->Remove(ToIdle);
				}

				m_pSkillStateMachine->Push(m_pStateFactory->Create(ToRun, m_skillID));

			}
			else
			{

				m_pSkillStateMachine->Push(m_pStateFactory->Create(ToIdle, m_skillID));

			}
		}
		
	}
	
	
	if(m_pSkill->bAdaptationDistance)
	{
		float dis = ccpDistance(m_nearestMonsterPos, m_pRole->getPosition());

		if(dis < m_displacement)
		{
			m_displacement = dis;
		}

		if(dis < m_effectDisplacement)
		{
			m_effectDisplacement = dis;
		}
	}
	

	if(m_pSkill->displacement != 0)
	{
		if(m_bMoving == false)
		{

			CCPoint dir = m_pRole->getDirectionVector();

			bool bMagician = false;
			if(m_pRole->getType() == kTypeMagician_Boy || m_pRole->getType() == kTypeMagician_Girl)
			{
				bMagician = true;
			}

			if(m_pSkill->aim == SKILL_INFO::E_AIM_ENEMY && !bMagician)
			{
				dir = m_aimDir;
			}

			
			

			dir = ccpMult(dir, (float)m_displacement);
			dir = ccpAdd(m_pRole->getPosition(), dir);
			float speed = 200.0f;

			ClientUserSkillInfo info;
			if(SkillDispMgr::Get()->GetOneSkillInfoById(m_pRole->GetID(), m_skillID, info))
			{
				if(m_pSkill->partID == info.parts[0].part)
					m_fContinueTime = info.parts[0].continue_time;
			}

			if(m_fContinueTime > 0.0f && m_displacement != 0.0f)
			{
				speed = (float)fabs((float)m_displacement) / m_fContinueTime;

			}
			/*else
			{
				float animTotalTime = m_pRole->GetCurAnimTotalTime();
				if(animTotalTime > 0.0f)
					speed = (float)fabs((float)m_displacement) / animTotalTime;
			}*/

			if(m_pRole != NULL)
			{

				/*CCCallFunc* endFunc = CCCallFuncND::create(this, 
					callfuncND_selector(AnimHandler::EndMove),
					(void *)this);
				m_moveToResult = m_pRole->moveTo(dir, speed, endFunc);*/

				if(m_pMoveToHandler == NULL && m_displacement != 0.0f)
				{
					MoveToHandler* pMoveTo = dynamic_cast<MoveToHandler*>(m_pStateFactory->Create(ToMoveToHandler, m_skillID));
					if(pMoveTo)
					{
						m_pMoveToHandler = pMoveTo;
						m_pContainer->Push(pMoveTo);
                        CCPoint dstPoint = m_pRole->calMoveToEndPoint(dir);
						// CCLOG("m_displacement : %f , m_skillID : %ld, curPos(%f, %f), oldDst(%f, %f), Dst(%f, %f)", m_displacement, m_skillID, m_pRole->getPosition().x, m_pRole->getPosition().y, dir.x, dir.y, dstPoint.x, dstPoint.y);

#if _DEBUG
						if(ccpDistance(dstPoint, m_pRole->getPosition()) > fabs(m_displacement) * 1.15f)
						{
							//assert(0);
						}
#endif
						m_bMoving = true;

						pMoveTo->SetTarget(m_pRole, dstPoint, speed);
						
						
					}
				}
				
			}
		}

	}

}

void AnimHandler::ChangeToLinkTime( void )
{
	if(m_pContainer != NULL)
	{
		m_pContainer->Push( m_pStateFactory->Create(ToLinkTimeHandler, m_pSkill->Id));
		m_pContainer->Remove(this);
	}
}



void AnimHandler::UpdateDirectionByJoystick( float dt )
{
	CCPoint velocity = HSJoystick::sharedJoystick()->getVelocity();
	m_pRole->setDirection(velocity);
}


void AnimHandler::EndMove( CCNode*, void*)
{
	m_bMoving = false;
}

// -----------------------------------------------------------------------

MagicAnimHandler::MagicAnimHandler( BaseAttackEvt* pEvt )
	: AnimHandler(pEvt)
{
	m_Type = ToMagicAnimHandler;
}

bool MagicAnimHandler::Entered()
{
	AnimHandler::Entered();


	ATTACH_POINT_TYPE apt = kType_SHOOT_HOR;
	switch(m_pRole->getDirection())
	{
	case DIRECTION_UP:
		apt = kType_SHOOT_UP;
		break;
	case DIRECTION_BOTTOM:
		apt = kType_SHOOT_DOWN;
		break;
	case DIRECTION_LEFT:
	case DIRECTION_RIGHT:
		apt = kType_SHOOT_HOR;
		break;
	default:
		apt = kType_SHOOT_HOR;
		break;
	}


	CCPoint pos = m_pRole->getAttachPoint(apt);
	pos = ccpAdd( m_pRole->getPosition(),  pos );
	
	if(m_bAnimMonster)
	{
	 	CCPoint oldDir = m_aimDir;
		m_aimDir = ccpSub(m_nearestMonsterPos, pos);
		if(ccpDistanceSQ(m_nearestMonsterPos, pos) < 0.00001f)
		{
			m_aimDir = m_pRole->getDirectionVector();
		}

		m_aimDir = ccpNormalize(m_aimDir);

		if(ccpDot(oldDir, m_aimDir) < 0.0f)
		{
			m_aimDir = oldDir;
		}
		
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------

LinkTimeHandler::LinkTimeHandler( BaseAttackEvt* pEvt )
	: BaseAttackState( pEvt)
{
	m_Type = ToLinkTimeHandler;
}

bool LinkTimeHandler::Entered()
{
	BaseAttackState::Entered();

	m_fSkillLinkTime = m_pSkill->linkTime;

	return true;
}

void LinkTimeHandler::Update( float dt )
{
	BaseAttackState::Update(dt);

	if(m_pContainer != NULL)
	{
		m_fSkillLinkTime -= dt;

		bool bToIdle = true;

		if(m_fSkillLinkTime <= 0.0f)
		{
			if(m_pSkill->type == ToNormalAttack)
			{
				int nextPartID = m_partID + 1;

				if( nextPartID > m_pRole->GetNormalAttackNum())
				{
					m_pRole->tellIsAttackEnable();
				}

				if(nextPartID <= m_pRole->GetNormalAttackNum()  && m_pSkill->linkSkillId != 0)
				{
                    if(m_pRole->getTag() == MAP_TAG_SEER || PvAIManager::Get()->isAIPlayer(m_pRole))
                    {
                        bToIdle = false;
                       
                        FState* pState = dynamic_cast<FState*>(m_pContainer);
                        if(pState != NULL)
                        {
                            m_pSkillStateMachine->Remove(pState);
                        }
                        
                        m_fSkillLinkTime = 0.0f;

						const SKILL_INFO* pLinkSkillInfo = SkillDispMgr::Get()->GetSkillInfo(m_pSkill->linkSkillId);
						m_pSkillStateMachine->Push(m_pStateFactory->Create(pLinkSkillInfo->type,  m_pSkill->linkSkillId));

                    }

				}
			}
			else
			{
				if(m_pSkill->linkSkillId != 0)
				{
					const SKILL_INFO* pLinkSkillInfo = SkillDispMgr::Get()->GetSkillInfo(m_pSkill->linkSkillId);
					FState* pLinkState = m_pStateFactory->Create(pLinkSkillInfo->type,  m_pSkill->linkSkillId);
					AimDirectionHandler* pAimDirState = dynamic_cast<AimDirectionHandler*>(m_pContainer->GetState(ToAimDirectionHandler));
					if(pAimDirState)
					{
						StateContainer* pContainer = dynamic_cast<StateContainer*>(pLinkState);
						if(pContainer)
						{
							pContainer->Push( new AimDirectionHandler(*pAimDirState));
						}
					}

					

					FState* pState = dynamic_cast<FState*>(m_pContainer);
					if(pState != NULL)
					{
						m_pSkillStateMachine->Remove(pState);
					}

					bToIdle = false;
					
					m_pSkillStateMachine->Push(pLinkState);
                    
				}
				else
				{
					if(m_pSkill->animation[DIRECTION_LEFT].animationID == (unsigned int) -1 ||
						m_pSkill->animation[DIRECTION_LEFT].animationID == 0)
					{
						//if(m_pSkillStateMachine->CheckStateRuning(ToIdle) ||
						//	m_pSkillStateMachine->CheckStateRuning(ToRun))
						{
							bToIdle = false;

							FState* pState = dynamic_cast<FState*>(m_pContainer);
							if(pState != NULL)
							{
								m_pSkillStateMachine->Remove(pState);
							}
						}
						
					}
				}
			}
			

		}
		else
		{
			bToIdle = false;
		}



		if (bToIdle)
		{
			m_pSkillStateMachine->Remove(ToRun);
			m_pSkillStateMachine->Remove(ToIdle);
			if(CanIdleState())
				m_pSkillStateMachine->Push(m_pStateFactory->Create(ToIdle, m_pSkill->Id));
			
			FState* pState = dynamic_cast<FState*>(m_pContainer);
			if(pState != NULL)
			{
				m_pSkillStateMachine->Remove(pState);
			}

			if(m_pSkill->type == ToNormalAttack)
			{
				int nextPartID = m_partID + 1;

				if( nextPartID >= m_pRole->GetNormalAttackNum())
				{
					m_pRole->tellIsAttackEnable();
				}

			}
			
		}

		
	}


	
	
}

void LinkTimeHandler::Exited()
{
	BaseAttackState::Exited();
}

// -----------------------------------------------------------------------------


AttackCheckerHandler::AttackCheckerHandler( BaseAttackEvt* pEvt )
	:BaseAttackState(pEvt)
{
	m_Type = ToAttackCheckerHandler;
}


bool AttackCheckerHandler::Entered()
{
	BaseAttackState::Entered();

	m_pAttackChecker = NULL;

	if(LevelManager::sShareInstance()->isCurrentPVPLevel() || LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel()|| LevelManager::sShareInstance()->isCurrentBestFighterLevel())
	{
		if(m_pRole->getTag() == MAP_TAG_SEER)
		{
			m_pAttackChecker = new AttackChecker::CNormalAttack(m_pRole);
			m_pAttackChecker->SetCanAttackOtherPlayer(true);
		}
	}
	else if (LevelManager::sShareInstance()->isCurrentPVAILevel())
	{
		m_pAttackChecker = new AttackChecker::CNormalAttack(m_pRole);
		m_pAttackChecker->SetCanAttackOtherPlayer(true);
	}
	else
	{
		m_pAttackChecker = new AttackChecker::CNormalAttack(m_pRole);
	}


	if(m_pAttackChecker)
	{
		m_pAttackChecker->SetSkillID(m_skillID);
		m_pAttackChecker->SetAttackerID(m_pRole->GetID());
	}
	return true;
}

void AttackCheckerHandler::Update( float dt )
{
	BaseAttackState::Update(dt);
	if(m_pAttackChecker)
		m_pAttackChecker->Update(dt);
}

void AttackCheckerHandler::Exited()
{
	BaseAttackState::Exited();
	if(m_pAttackChecker)
	{
		delete m_pAttackChecker;
		m_pAttackChecker = NULL;
	}
}

void AttackCheckerHandler::SetAttackAction( AttackChecker::CBase::ACTION action )
{
	//CCAssert(m_pAttackChecker != NULL);
	if(m_pAttackChecker)
	{
		m_pAttackChecker->SetAction(action);
	}
}

// ----------------------------------------------------------------

MagicCheckerHandler::MagicCheckerHandler( BaseAttackEvt* pEvt )
	: AttackCheckerHandler(pEvt)
{
	m_Type = ToMagicCheckHandler;
}

bool MagicCheckerHandler::Entered()
{
	BaseAttackState::Entered();

	m_pAttackChecker = NULL;

	MagicCheckerEvt* pEvt = dynamic_cast<MagicCheckerEvt*>(m_pEvt);
	if(pEvt)
	{
		if(LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel() || LevelManager::sShareInstance()->isCurrentBestFighterLevel())
		{
			//if(m_pRole->getTag() == MAP_TAG_SEER)
			{
				m_pAttackChecker = new AttackChecker::CNormalAttack(pEvt->m_pEffect);
				m_pAttackChecker->SetCanAttackOtherPlayer(true);
			}
		}
		else if (LevelManager::sShareInstance()->isCurrentPVAILevel())
		{
			m_pAttackChecker = new AttackChecker::CNormalAttack(pEvt->m_pEffect);
			m_pAttackChecker->SetCanAttackOtherPlayer(true);
		}
		else
		{
			m_pAttackChecker = new AttackChecker::CNormalAttack(pEvt->m_pEffect);
		}
		
		if(m_pAttackChecker)
		{
			m_pAttackChecker->SetSkillID(m_skillID);
			m_pAttackChecker->SetAttackerID(m_pRole->GetID());
			m_pAttackChecker->setElfAttack(m_Elf ,attackType);

			const FAIRY_SKILL_INFO* pFairySkill = SkillDispMgr::Get()->GetFairySkillInfo(m_skillID);
			if(pFairySkill)
			{
				if(pFairySkill->aim == FAIRY_SKILL_INFO::E_FULL_SCREEN)
				{
					m_pAttackChecker->SetFullScreenAttack(true);
				}
			}
		}
	}
	return true;
}

// ----------------------------------------------------------------

AttackSenderHandler::AttackSenderHandler( BaseAttackEvt* pEvt )
	: BaseAttackState(pEvt)
{
	m_Type = ToAttackSenderHandler;
}

bool AttackSenderHandler::Entered()
{
	BaseAttackState::Entered();
	
	if(m_pRole->getTag() != MAP_TAG_SEER)
		return true;

	m_attackReq.set_userid(m_pRole->GetID().getID());
	m_attackReq.set_reg_time(m_pRole->GetID().getRegTime());
	CSPoint *pCSPoint = m_attackReq.mutable_start_pos();
	CCPoint pos = m_pRole->getPosition();
	pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
	pCSPoint->set_xpos(pos.x);
	pCSPoint->set_ypos(pos.y);
	pCSPoint = m_attackReq.mutable_start_direct();
	CCPoint dir = m_pRole->getDirectionVector();
	pCSPoint->set_xpos(dir.x);
	pCSPoint->set_ypos(dir.y);

	m_attackReq.set_action_id(m_skillID);


	
	pCSPoint = m_attackReq.mutable_end_direct();
	pCSPoint->set_xpos(dir.x);
	pCSPoint->set_ypos(dir.y);

	pCSPoint = m_attackReq.mutable_end_pos();
	pCSPoint->set_xpos(pos.x);
	pCSPoint->set_ypos(pos.y);

	m_attackReq.clear_monsters();

	//OnlineNetworkManager::sShareInstance()->sendPlayerAttackMessage(m_attackReq);

	return true;

}

void AttackSenderHandler::Update( float dt )
{
	BaseAttackState::Update(dt);

	if(UserData::Get()->getUserId() != m_pRole->GetID())
		return;

}

void AttackSenderHandler::Exited()
{
	BaseAttackState::Exited();
	
	if(UserData::Get()->getUserId() != m_pRole->GetID())
		return;

	if(m_pContainer != NULL)
	{
		CCPoint dir = m_pRole->getDirectionVector();
		CSPoint* pCSPoint = m_attackReq.mutable_end_direct();
		pCSPoint->set_xpos(dir.x);
		pCSPoint->set_ypos(dir.y);

		CCPoint pos = m_pRole->getPosition();
		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		pCSPoint = m_attackReq.mutable_end_pos();
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);

		AttackChecker::CNormalAttack* pNormalAttack = dynamic_cast<AttackChecker::CNormalAttack*>(m_pContainer->GetState(ToAttackCheckerHandler));
		if(pNormalAttack != NULL)
		{

			if(pNormalAttack->m_attackMessage.size() > 0)
			{
				m_attackReq.clear_monsters();

				for(unsigned int i = 0; i < pNormalAttack->m_attackMessage.size(); i++)
				{

					*m_attackReq.add_monsters() = pNormalAttack->m_attackMessage[i];

				}

				// OnlineNetworkManager::sShareInstance()->sendPlayerAttackMessage(m_attackReq);

			}

		}


	}
}

// ------------------------------------------------------------------------

EffectAnimHandler::EffectAnimHandler( BaseAttackEvt* pEvt )
	: BaseAttackState(pEvt)
	, m_bInitDir(false)
	, m_bLinkEffect(false)
{
	m_Type = ToEffectAnimHandler;
}

bool EffectAnimHandler::Entered()
{
	BaseAttackState::Entered();

	m_pEffect = NULL;

	if(m_pSkill->effectId > 0)
	{
		if(m_pSkill->effectsoundID > 0)
		{
			if(!GameAudioManager::sharedManager()->isEffectIsPlaying(m_pSkill->effectsoundID))
				GameAudioManager::sharedManager()->playEffect(m_pSkill->effectsoundID, false);
		}

		if(m_bLinkEffect)
		{
			m_pEffect = GetEffect(m_pSkill->linkEffectId);
		}
		else
		{
			m_pEffect = GetEffect(m_pSkill->effectId);
		}
		
		m_pContainer->Push(m_pStateFactory->Create(new MagicCheckerEvt(m_pRole, m_skillID, m_pEffect)));


		ATTACH_POINT_TYPE apt = kType_SHOOT_HOR;
		switch(GetAnimDir())
		{
		case DIRECTION_UP:
			apt = kType_SHOOT_UP;
			break;
		case DIRECTION_BOTTOM:
			apt = kType_SHOOT_DOWN;
			break;
		case DIRECTION_LEFT:
		case DIRECTION_RIGHT:
			apt = kType_SHOOT_HOR;
			break;
		default:
			apt = kType_SHOOT_HOR;
			break;
		}

		m_displacement = m_pSkill->longDistanceAttackRange;
		if(m_pSkill->bAdaptationDistance)
		{
			if(m_displacement > m_attackDisplacement)
				m_displacement = m_attackDisplacement;
		}

		m_dir = m_attackDir;

		if(m_bInitDir)
		{
			float degree = acosf(ccpDot(m_initDir, m_dir)) / 3.1415926 * 180.0f;
			if(m_dir.x < 0)
				degree = -degree;
			if(m_pEffect && m_pEffect->getRoot())
				m_pEffect->getRoot()->setRotation(degree);
		}
		
		bool bLoopAnim = true;
		if(m_bLinkEffect)
		{
			bLoopAnim = false;
		}

		if(m_pSkill->bShowFarWay)
		{
			m_speed = m_pSkill->longDistanceEffectSpeed;

			CCPoint pos = m_pRole->getPosition();
			pos = ccpAdd(pos, ccpMult(m_dir, m_displacement));
			if(m_pEffect)
			{
				m_pEffect->setPosition( pos );
				m_pEffect->SetAnim(kType_Play,1,bLoopAnim);
			}
			
		}
		else
		{
			m_speed = m_pSkill->longDistanceEffectSpeed;
	
			CCPoint pos = m_pRole->getAttachPoint(apt);
			if(m_bLinkEffect)
			{
				if(m_pEffect)
					m_pEffect->setPosition( m_linkEffectPos );

			}
			else
			{
				if(m_pEffect)
					m_pEffect->setPosition( m_pRole->getPosition() + pos );
			}
			
			if(m_pEffect)
				m_pEffect->SetAnim(kType_Play,1,bLoopAnim);
		}
		
		if(m_pEffect && m_pEffect->getParent() != LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer())
		{
			LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer()->addChild(m_pEffect, LevelLayer::sCalcZorder(m_pEffect->getPosition()));
			SetEffectZOrder(m_pEffect->getPosition());
		}
		else
		{
			if(m_pEffect)
				m_pEffect->setZOrder(LevelLayer::sCalcZorder(m_pEffect->getPosition()));
		}

	}

	return true;
}

void EffectAnimHandler::Update( float dt )
{
	BaseAttackState::Update(dt);

	if(m_pEffect == NULL)
	{
		m_pContainer->Remove(this);

		FState* pState = dynamic_cast<FState*>(m_pContainer);
		m_pSkillStateMachine->Remove(pState);

	}
	else
	{
		
		m_pEffect->setVisible(m_pRole->GetRoleOpacity() != 0);
		

		if(m_pSkill->bShowFarWay || m_pSkill->aim == SKILL_INFO::E_AIM_HERO)
		{
			bool bComplete = false;
			ANIMATION_STATE aniState = kType_START;
			if(m_pEffect)
				aniState = m_pEffect->getCurAnimationState();
			switch(aniState)
			{
			case kType_COMPLETE:
				bComplete = true;
				break;
			default:
				break;
			}

			if(bComplete)
			{
				m_pContainer->Remove(this);
				if(m_pSkill->linkEffectId == 0 || (m_pSkill->linkEffectId > 0 && m_bLinkEffect == true))
				{
					FState* pState = dynamic_cast<FState*>(m_pContainer);
					m_pSkillStateMachine->Remove(pState);
				}
				
			}
		}
		else
		{
			if(m_displacement > 0.0f)
			{
				
				
				float d = m_speed * dt;

				if(m_displacement < d)
				{
					d = m_displacement;
				}

				m_displacement -= d;
				CCPoint pos;
				if(m_pEffect)
				{
					pos = m_pEffect->getPosition();
					pos = ccpAdd(pos, ccpMult(m_dir, d));
					m_pEffect->setPosition(pos);
				}

				SetEffectZOrder(pos);
				
				if(m_bLinkEffect)
				{
					bool bComplete = false;
					ANIMATION_STATE aniState = kType_START;
					if(m_pEffect)
						m_pEffect->getCurAnimationState();
					switch(aniState)
					{
					case kType_COMPLETE:
						bComplete = true;
						break;
					default:
						break;
					}

					if(bComplete)
					{
						m_displacement = 0.0f;
					}
				}
				
				
			}
			else
			{
				m_pContainer->Remove(this);
				if(m_pSkill->linkEffectId == 0 || (m_pSkill->linkEffectId > 0 && m_bLinkEffect == true))
				{
					FState* pState = dynamic_cast<FState*>(m_pContainer);
					m_pSkillStateMachine->Remove(pState);
				}
				
			}
		}
		
	}
}

void EffectAnimHandler::Exited()
{
	BaseAttackState::Exited();
	CCPoint effedtPos;
	if(LevelManager::sShareInstance()->getCurrentLevelLayer() &&
		LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer())
	{

		if(m_pEffect)
		{
			effedtPos = m_pEffect->getPosition();
		}

		cocos2d::CCArray* pArray = LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer()->getChildren();
		if(pArray)
		{
			for(unsigned int i = 0; i < pArray->data->num; i++)
			{
				EffectSprite* pEffect = dynamic_cast<EffectSprite*>(pArray->data->arr[i]);
				if(pEffect)
				{
					if(m_pEffect == pEffect)
					{
						if(m_pEffect)
							m_pEffect->setVisible(false);
						m_pEffect = NULL;
					}
				}
			}
		}

		if(m_pSkill)
		{
			if(m_pSkill->linkEffectId > 0 && m_bLinkEffect == false)
			{
				if(m_pContainer)
				{
					EffectAnimHandler* pEffectAnimHandler = (EffectAnimHandler*)m_pStateFactory->Create(ToEffectAnimHandler, m_skillID);
					if(m_pSkill->bRotateEffect)
					{
						pEffectAnimHandler->SetInitDirection(m_initDir);
					}

					pEffectAnimHandler->SetAttackDir( m_attackDir, m_attackDisplacement);
					pEffectAnimHandler->SetLinkEffectPos(effedtPos);
					m_pContainer->Push(pEffectAnimHandler );
				}
			}
		}
	}

	
	
}

void EffectAnimHandler::SetInitDirection( CCPoint dir )
{
	if(m_pSkill)
		m_bInitDir = m_pSkill->bRotateEffect;
	m_initDir = dir;
}

cocos2d::CCPoint EffectAnimHandler::GetAnimDirVector( void )
{
	CCPoint ret;
	int animID = m_pRole->GetAnimID();
	if(m_pSkill->animation[DIRECTION_LEFT].animationID == animID)
	{
		if(m_pRole->IsAnimFlipX())
		{
			ret.setPoint(1.0f, 0.0f);
		}
		else
		{
			ret.setPoint(-1.0f, 0.0f);
		}
		
	}
	else if(m_pSkill->animation[DIRECTION_RIGHT].animationID == animID)
	{
		ret.setPoint(1.0f, 0.0f);
	}
	else if(m_pSkill->animation[DIRECTION_UP].animationID == animID)
	{
		ret.setPoint(0.0f, 1.0f);
	}
	else if(m_pSkill->animation[DIRECTION_BOTTOM].animationID == animID)
	{
		ret.setPoint(0.0f, -1.0f);
	}

	// CCLOG("EffectAnimHandler animID : %ld, Dir : (%f, %f)", animID, ret.x, ret.y );

	return ret;
}

DIRECTION_ACTOR EffectAnimHandler::GetAnimDir(void)
{
	DIRECTION_ACTOR ret;
	int animID = m_pRole->GetAnimID();
	

	if(m_pSkill->animation[DIRECTION_LEFT].animationID == animID)
	{
		if(m_pRole->IsAnimFlipX())
		{
			ret = DIRECTION_RIGHT;
		}
		else
		{
			ret = DIRECTION_LEFT;
		}
	}
	else if(m_pSkill->animation[DIRECTION_UP].animationID == animID)
	{
		ret = DIRECTION_UP;

	}
	else if(m_pSkill->animation[DIRECTION_BOTTOM].animationID == animID)
	{
		ret = DIRECTION_BOTTOM;

	}
	else
	{
		ret = DIRECTION_UNKNOW;
	}

	CCLOG("EffectAnimHandler animID : %ld, Dir : %ld", animID, ret );

	return ret;
}


void EffectAnimHandler::SetAttackDir( CCPoint dir, float displacement )
{
	m_attackDir = dir;
	m_attackDisplacement = displacement;
	
}

EffectSprite* EffectAnimHandler::GetEffect( int id )
{
	EffectSprite* pRet = NULL;
	if(LevelManager::sShareInstance()->getCurrentLevelLayer() && LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer())
	{
		cocos2d::CCArray* pArray = LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer()->getChildren();
		if(pArray)
		{
			for(unsigned int i = 0; i < pArray->data->num; i++)
			{
				EffectSprite* pEffect = dynamic_cast<EffectSprite*>(pArray->data->arr[i]);
				if(pEffect)
				{
					if(pEffect->getType() == id)
					{
						if(!pEffect->isVisible())
						{
							pRet = pEffect;
							break;
						}
					}
				}
			}
		}
	}

	if(pRet == NULL)
	{
		pRet = ParticleManager::Get()->createEffectSprite(id,"");

	}

	pRet->setVisible(true);

	return pRet;
}

void EffectAnimHandler::SetEffectZOrder( CCPoint pos )
{
	if(m_pSkill)
	{
		switch(m_pSkill->effectZOrder)
		{
		case FAIRY_SKILL_INFO::E_BEHIDE_HERO:
			pos.y += 16.0f;
			break;
		case FAIRY_SKILL_INFO::E_FRONT_HERO:
			pos.y -= 16.0f;
			break;
		default:

			break;

		}
	}

	if(LevelManager::sShareInstance()->getCurrentLevelLayer() && LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer())
		LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer()->reorderChild(m_pEffect, LevelLayer::sCalcZorder(pos));
}

// ----------------------------------------------------------------------------------

RoleEffectHandler::RoleEffectHandler( BaseAttackEvt* pEvt )
	: BaseAttackState(pEvt)
{
	m_Type = ToRoleEffectHandler;
}

bool RoleEffectHandler::Entered()
{
	BaseAttackState::Entered();

	if(m_pSkill->effectId > 0)
	{
		m_pEffect = ParticleManager::Get()->createEffectSprite(m_pSkill->effectId,"");
		m_pRole->getRoot()->addChild(m_pEffect, MAP_Z_OBJ_TOP);
		if(m_pEffect)
		{
			m_pEffect->SetAnim(kType_Play,1,true);
		}

		CCPoint pos(0.0f, 0.0f);
		if(m_pEffect)
			m_pEffect->setPosition(pos);
	}
	return true;
}

void RoleEffectHandler::Update( float dt )
{
	BaseAttackState::Update(dt);

	if(m_pEffect)
	{
		m_pEffect->setVisible(m_pRole->GetRoleOpacity() != 0);
	}
}

void RoleEffectHandler::Exited()
{
	BaseAttackState::Exited();

	if(m_pEffect)
	{
		m_pEffect->removeFromParent();
		m_pEffect = NULL;
	}

}

// ---------------------------------------------------------------------------------

AimDirectionHandler::AimDirectionHandler( BaseAttackEvt* pEvt )
	:BaseAttackState(pEvt)
	, m_bEntered(false)
	
{
	m_Type = ToAimDirectionHandler;
}

AimDirectionHandler::AimDirectionHandler( const AimDirectionHandler& rhs )
	: BaseAttackState(new BaseAttackEvt(rhs.m_pRole, rhs.m_skillID) )
	, m_bEntered(true)
{
	m_Type = ToAimDirectionHandler;

	m_aimDir = rhs.m_aimDir;
	m_nearestMonsterPos = rhs.m_nearestMonsterPos;
	m_bAimMonster = rhs.m_bAimMonster;

}

bool AimDirectionHandler::Entered()
{
	BaseAttackState::Entered();
	if(m_bEntered)
		return false;

	m_bAimMonster = false;

	SkillBeginReceiverHandler* pReceiver = dynamic_cast<SkillBeginReceiverHandler*>(m_pContainer->GetState(ToSkillBeginReceiverHandler));
	if(pReceiver)
	{
		m_aimDir = pReceiver->GetDirection();
		m_bAimMonster = pReceiver->IsAimMonster();
		m_nearestMonsterPos = pReceiver->GetNearestMonsterPos();
        CCLOG("cur Pos(%f, %f), receiver Pos(%f, %f)", m_pRole->getPosition().x, m_pRole->getPosition().y, pReceiver->GetRolePos().x, pReceiver->GetRolePos().y);
		m_pRole->getRoot()->stopAllActions();
		//m_pRole->setPosition(pReceiver->GetRolePos());
		m_pRole->setDirection(pReceiver->GetRoleDirection());
	}
	else
	{
		m_aimDir = GetAimDirection();

	}
	return true;
}

cocos2d::CCPoint AimDirectionHandler::GetNearestMonsterPos( void )
{
	return m_nearestMonsterPos;
}

bool AimDirectionHandler::IsAimMonster( void )
{
	return m_bAimMonster;
}

cocos2d::CCPoint AimDirectionHandler::GetDirection( void )
{
	return m_aimDir;
}


cocos2d::CCPoint AimDirectionHandler::GetAimDirection()
{
	CCPoint ret = m_pRole->getDirectionVector();
	CCPoint velocity = m_pRole->GetJoystickVelocity();
	CCPoint rolePos = m_pRole->getPosition();

	if(velocity.x == 0.0f && velocity.y == 0.0f)
	{
		if(LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel() || LevelManager::sShareInstance()->isCurrentBestFighterLevel())
		{
			if(LevelManager::sShareInstance()->getCurrentLevelLayer())
			{
				float minDis = m_pSkill->displacement;
				if(minDis < m_pSkill->longDistanceAttackRange)
					minDis = m_pSkill->longDistanceAttackRange;

				if(minDis <= 0.0f)
					minDis = 320.0f;

				minDis *= minDis;

				SpriteSeer* pNearestHero = NULL;

				USERINFO* pHeroInfo = UserData::Get()->GetUserInfo(m_pRole->GetID());
		

				std::map<PLAYER_ID, OthersPlayerInfo>& otherPlayer = LevelManager::sShareInstance()->getCurrentLevelLayer()->m_othersPlayerInfo;
				for(std::map<PLAYER_ID, OthersPlayerInfo>::const_iterator iter = otherPlayer.begin(); iter != otherPlayer.end(); iter++)
				{
					SpriteSeer* pOhterHero = iter->second.seer; 

					if(m_pRole == pOhterHero || pOhterHero == NULL)
						continue;

					if(!pOhterHero->CanAim())
						continue;

					USERINFO* pOtherInfo = UserData::Get()->GetUserInfo(pOhterHero->GetID());
					if(pHeroInfo && pOhterHero)
					{
						if(pHeroInfo->battleSide % 2 == pOtherInfo->battleSide % 2)
							continue;
					}

					float dis = ccpDistanceSQ(pOhterHero->getPosition(), m_pRole->getPosition());
					if( dis <= minDis)
					{
						pNearestHero = pOhterHero;
						minDis = dis;

					}
				}
				
		
				if(pNearestHero)
				{
					m_bAimMonster = true;
					m_nearestMonsterPos = pNearestHero->getPosition();
					ret = ccpSub(m_nearestMonsterPos, rolePos);
					if(ret.x == 0.0f && ret.y == 0.0f)
					{
						ret = m_pRole->getDirectionVector();
					}
				}
			}
		}
		else if(LevelManager::sShareInstance()->isCurrentPVAILevel())
		{
			if(LevelManager::sShareInstance()->getCurrentLevelLayer())
			{
				float minDis = m_pSkill->displacement;
				if(minDis < m_pSkill->longDistanceAttackRange)
					minDis = m_pSkill->longDistanceAttackRange;

				if(minDis <= 0.0f)
					minDis = 320.0f;

				minDis *= minDis;

				SpriteSeer* pNearestHero = NULL;

				USERINFO* pHeroInfo = UserData::Get()->GetUserInfo(m_pRole->GetID());

				if(m_pRole->GetID() == GameManager::Get()->getHero()->GetID())
				{
					std::map<PLAYER_ID, OthersPlayerInfo>& otherPlayer = LevelManager::sShareInstance()->getCurrentLevelLayer()->m_othersPlayerInfo;
					for(std::map<PLAYER_ID, OthersPlayerInfo>::const_iterator iter = otherPlayer.begin(); iter != otherPlayer.end(); iter++)
					{
						SpriteSeer* pOhterHero = iter->second.seer; 

						if(m_pRole == pOhterHero || pOhterHero == NULL)
							continue;

						if(!pOhterHero->CanAim())
							continue;

						USERINFO* pOtherInfo = UserData::Get()->GetUserInfo(pOhterHero->GetID());
						if(pHeroInfo && pOhterHero)
						{
							if(pHeroInfo->battleSide % 2 == pOtherInfo->battleSide % 2)
								continue;
						}

						float dis = ccpDistanceSQ(pOhterHero->getPosition(), m_pRole->getPosition());
						if( dis <= minDis)
						{
							pNearestHero = pOhterHero;
							minDis = dis;

						}
					}
				}
				else
				{
					float dis = ccpDistanceSQ(GameManager::Get()->getHero()->getPosition(), m_pRole->getPosition());
					if( dis <= minDis)
					{
						pNearestHero = GameManager::Get()->getHero();
						minDis = dis;

					}
				}


				if(pNearestHero)
				{
					m_bAimMonster = true;
					m_nearestMonsterPos = pNearestHero->getPosition();
					ret = ccpSub(m_nearestMonsterPos, rolePos);
					if(ret.x == 0.0f && ret.y == 0.0f)
					{
						ret = m_pRole->getDirectionVector();
					}
				}
			}
		}
		else
		{
			SpriteMonster* pNearestMonster = GetNearestMonster();

			if(pNearestMonster)
			{
				m_bAimMonster = true;
				m_nearestMonsterPos = pNearestMonster->getPosition();
				ret = ccpSub(m_nearestMonsterPos, rolePos);

				if(ret.x == 0.0f && ret.y == 0.0f)
				{
					ret = m_pRole->getDirectionVector();
				}
			}
		}
		
	}
	else
	{
		if(m_pSkill->bAdaptationDistance)
		{
			float minDis = m_pSkill->longDistanceAttackRange;

			minDis *= minDis;

			const map<int, SpriteMonster*>& allMonster = SpriteMonsterMgr::Get()->GetAllMonster();
			for(map<int, SpriteMonster*>::const_iterator iter = allMonster.begin(); iter != allMonster.end(); iter++)
			{
				CCPoint dir = ccpSub(iter->second->getPosition() , rolePos);
				if(dir.x == 0.0f && dir.y == 0.0f)
				{
					m_bAimMonster = true;
					m_nearestMonsterPos = iter->second->getPosition();
					ret = velocity;
					break;
				}

				dir = ccpNormalize(dir);

				float fov = cosf(90.0f / 180.0f * 3.1415926f);
				if(ccpDot(dir, velocity) > fov)
				{
					float dis = ccpDistanceSQ(iter->second->getPosition(), m_pRole->getPosition());
					if( dis <= minDis)
					{
						m_bAimMonster = true;
						minDis = dis;
						m_nearestMonsterPos = iter->second->getPosition();
						ret = dir;
					}
				}

			}
		}
		else
		{
			ret = velocity;
		}

	}

	if (ret.x != 0 || ret.y != 0)
	{
		ret = ccpNormalize(ret);
	}

	return ret;
}

SpriteMonster* AimDirectionHandler::GetNearestMonster( void )
{
	SpriteMonster* pRet = NULL;
	float minDis = m_pSkill->displacement;
	if(minDis < m_pSkill->longDistanceAttackRange)
		minDis = m_pSkill->longDistanceAttackRange;

	if(minDis <= 0.0f)
		minDis = 320.0f;

	minDis *= minDis;

	const map<int, SpriteMonster*>& allMonster = SpriteMonsterMgr::Get()->GetAllMonster();
	for(map<int, SpriteMonster*>::const_iterator iter = allMonster.begin(); iter != allMonster.end(); iter++)
	{
		if(iter->second->getCanBeFocus())
		{
			float dis = ccpDistanceSQ(iter->second->getPosition(), m_pRole->getPosition());
			if( dis <= minDis)
			{
				pRet = iter->second;
				minDis = dis;

			}
		}
		
	}

	return pRet;
}

// ----------------------------------------------------------------------------------------------------

SkillBeginSenderHandler::SkillBeginSenderHandler( BaseAttackEvt* pEvt )
	:BaseAttackState(pEvt)
{
	m_Type = ToSkillBeginSenderHandler;
}

bool SkillBeginSenderHandler::Entered()
{
	BaseAttackState::Entered();

	if(m_pRole->getTag() != MAP_TAG_SEER)
		return true;

	if(m_pSkill != NULL && m_pSkill->type != ToNormalAttack && m_pSkill->partID != 1)
		return true;

	CSSkillBeginReq req;

	req.set_userid(m_pRole->GetID().getID());
	req.set_reg_time(m_pRole->GetID().getRegTime());
	req.set_channel_id(m_pRole->GetID().getChannel());
	CSPoint *pCSPoint = req.mutable_pos();
	CCPoint pos = m_pRole->getPosition();
	pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
	pCSPoint->set_xpos(pos.x);
	pCSPoint->set_ypos(pos.y);
	
	CCPoint roleDir = m_pRole->getDirectionVector();
	req.set_start_dir_x(roleDir.x);
	req.set_start_dir_y(roleDir.y);

	req.set_action_id(m_skillID);

	AimDirectionHandler* pAim = dynamic_cast<AimDirectionHandler*>(m_pContainer->GetState(ToAimDirectionHandler));
	if(pAim)
	{
		pos = pAim->GetNearestMonsterPos();
		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		CCPoint dir = pAim->GetDirection();

		req.set_aim(pAim->IsAimMonster());

		pCSPoint = req.mutable_target_pos();
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);

		req.set_direct_x(dir.x);
		req.set_direct_y(dir.y);
	    req.set_entity_pos(0);

		OnlineNetworkManager::sShareInstance()->sendSkillBeginMessage(req);

	}

	ElfAimDirectionHandler* pElfAim = dynamic_cast<ElfAimDirectionHandler*>(m_pContainer->GetState(ToElfAimDirectionHandler));
	if(pElfAim)
	{
		pos = pElfAim->GetNearestMonsterPos();
		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		CCPoint dir = pElfAim->GetDirection();

		req.set_aim(pElfAim->IsAimMonster());

		pCSPoint = req.mutable_target_pos();
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);

		req.set_direct_x(dir.x);
		req.set_direct_y(dir.y);
		req.set_entity_pos(this->getElfAttack()->getSpriteCurPos());

		OnlineNetworkManager::sShareInstance()->sendSkillBeginMessage(req);

	}
	
	return true;
}

// ----------------------------------------------------------------------

SkillBeginReceiverHandler::SkillBeginReceiverHandler( BaseAttackEvt* pEvt )
	: BaseAttackState(pEvt)
{
	m_Type = ToSkillBeginReceiverHandler;
}

// ----------------------------------------------------------------------

MoveToHandler::MoveToHandler( BaseAttackEvt* pEvt )
	: BaseAttackState(pEvt)
{
	m_Type = ToMoveToHandler;
}

bool MoveToHandler::Entered()
{
	BaseAttackState::Entered();

	m_dir.setPoint(0.0f, 0.0f);
	m_pTarget = NULL;
	m_displacement = 0.0f;

	return true;
}

void MoveToHandler::Update( float dt )
{
	BaseAttackState::Update(dt);

	//CCLOG("MoveToHandler Speed : %f", m_speed);

	if(m_pTarget)
	{

		if(m_displacement > 0.0f)
		{
			float d = m_speed * dt;
			if(m_displacement < d)
			{
				d = m_displacement;
			}

			m_displacement -= d;
			CCPoint pos = m_pTarget->getPosition();
			pos = ccpAdd(pos, ccpMult(m_dir, d));
			
			
			SpriteSeer* pHero = dynamic_cast<SpriteSeer*>(m_pTarget);
			if(pHero)
			{
				pHero->getRoot()->stopAllActions();
			}

			m_pTarget->setPosition(pos);

			// CCLOG("Move To (%f, %f)", pos.x, pos.y);

			LevelManager::sShareInstance()->getCurrentLevelLayer()->getObjectLayer()->reorderChild(m_pTarget, LevelLayer::sCalcZorder(pos));
		}
	}
	else
	{
		m_displacement = 0.0f;
	}
	
}

void MoveToHandler::SetTarget( cocos2d::CCNode* pTarget, cocos2d::CCPoint& dstPos, float speed )
{
	m_speed = speed;
	m_pTarget = pTarget;
	if(pTarget)
	{
		CCPoint dir = ccpSub(dstPos, pTarget->getPosition());
		m_displacement = ccpLength(dir);
		if(m_displacement != 0.0f)
			m_dir = ccpNormalize(dir);
	}
}

// -------------------------------------------------------------------------------------------
VariableSpeedMoveToHandler::VariableSpeedMoveToHandler( BaseAttackEvt* pEvt )
	: MoveToHandler(pEvt)
{
	m_Type = ToVariableSpeedMoveToHandler;
}

void VariableSpeedMoveToHandler::Update( float dt )
{
	//if(m_a > 0.0f)
	//{
	//	if(m_speed >= m_topSpeed)
	//	{
	//		m_speed -= (dt * m_a);
	//	}
	//	else
	//	{
	//		m_speed += (dt * m_a);

	//	}
	//}
	//else
	//{
	//	if(m_speed >= m_topSpeed)
	//	{
	//		m_speed += (dt * m_a);
	//	}
	//	else
	//	{
	//		m_speed -= (dt * m_a);

	//	}
	//}
	
	if(m_pTarget)
	{
		SpriteSeer* pSeer = dynamic_cast<SpriteSeer*>(m_pTarget);
		if(pSeer)
		{
			pSeer->SetAdditionMoveRate(pSeer->GetAdditionMoveRate() + pSeer->GetAcceleration() * dt);
			m_speed = pSeer->GetMoveSpeed();
		}
	}
	MoveToHandler::Update(dt);
}

void VariableSpeedMoveToHandler::ChangeDstPos( const cocos2d::CCPoint& dstPos )
{
	if(m_pTarget)
	{
		CCPoint dir = ccpSub(dstPos, m_pTarget->getPosition());
		m_displacement = ccpLength(dir);
		if (m_displacement != 0.0)
		{
			m_dir = ccpNormalize(dir);
		}		
	}
}

