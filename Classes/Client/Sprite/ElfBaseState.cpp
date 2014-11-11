#include "ElfCloseToState.h"
#include "SkillInfo.h"
#include "SkillDispMgr.h"
#include "BaseAttackState.h"
#include "LevelLayer.h"

ElfBaseState::ElfBaseState(BaseElfEvt *pEvt) : FState(FState::ExclusiveStates, NULL)
{
	m_Type = ToBaseElfAttack;

	if (pEvt)
	{
		m_pElf = pEvt->m_pElf;
		m_pRole = pEvt->m_pSeer;
		m_actionId = pEvt->m_skillId;
		m_normalActionId = pEvt->m_normalSkillId;
		m_assistActionId = pEvt->m_assistSkillId;
		m_pAttachData = pEvt->m_pAttachData;
		m_pEvt = pEvt;
	}

	m_tempAnimDirection = 0;		
	m_fMoveSpeed = 0;
	m_isFlip = false;

	m_pContainer = NULL;
	m_pSkillStateMachine = NULL;
	m_pStateFactory = new ElfStateFactory(m_pElf);
	m_pSkill = SkillDispMgr::Get()->GetFairySkillInfo(pEvt->m_skillId);
	m_pNormalSkill = SkillDispMgr::Get()->GetFairyNormalSkillInfo(pEvt->m_normalSkillId);
	m_pAssistSkill = SkillDispMgr::Get()->GetFairyAssistSkillInfo(pEvt->m_assistSkillId);
}

ElfBaseState::~ElfBaseState( void )
{
	if(m_pEvt)
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

bool ElfBaseState::Entered()
{
	FState::Entered();

	m_tempAnimDirection = 0;	
	m_fMoveSpeed = 0.0;
	m_isFlip = false;
	return true;
}

void ElfBaseState::Exited()
{
	FState::Exited();
}

void ElfBaseState::Update(float dt)
{
	FState::Update(dt);
}

void ElfBaseState::Reset()
{
	m_tempAnimDirection = 0;
	m_isFlip = false;	
}

int ElfBaseState::GetAnimDirection(CCPoint dir)
{
	unsigned int animDir = 0;

	float dir_x = dir.x;
	float dir_y = dir.y;

	if (dir_x == 0)
	{
		if (dir_y>0)
		{
			animDir = 3;
		}
		else
		{
			animDir = 0;
		}
	}
	else if (dir_y == 0)
	{
		if (dir_x>0)
		{
			animDir = 3;
		}
		else
		{
			animDir = 0;
		}
	}
	else if (dir_x >0 && dir_y > 0)
	{
		animDir = 3;
	}
	else if (dir_x >0 && dir_y < 0)
	{
		animDir = 1;
	}
	else if (dir_x <0 && dir_y > 0)
	{
		animDir = 2;
	}
	else if (dir_x <0 && dir_y < 0)
	{
		animDir = 0;
	}
	return animDir;
}

int ElfBaseState::GetAnimDirection(unsigned int skillId)
{
	unsigned int animDir = 0;

	bool bFlip = m_pRole->IsAnimFlipX();
	if (m_pElf)
	{
		switch(skillId)
		{
		case kTypeIdle_FaceHor_Stand_MAIN:
		case kTypeIdle_FaceHor_Stand_OTH:
		case kTypeRun_FaceHor_MAIN:
		case kTypeRun_FaceHor_OTH:
			{
				if (bFlip)
				{
					animDir = 1;
				}
				else
				{
					animDir = 0;
				}
			}
			break;
		case kTypeIdle_FaceDown_Stand_MAIN:
		case kTypeIdle_FaceDown_Stand_OTH:
		case kTypeRun_FaceDown_MAIN:
		case kTypeRun_FaceDown_OTH:
			{
				//if (m_pElf->IsOnlyMe())
				//{
				//	CCPoint elfPos = m_pElf->getPosition();
				//	CCPoint rolePos = m_pRole->getPosition();
				//	if (elfPos.x < rolePos.x)
				//	{
				//		animDir = 1;
				//	}
				//	else
				//	{
				//		animDir = 0;
				//	}
				//}
				//else
				{
					if (m_pElf->IsStandLeft())
					{
						animDir = 1;
					}
					else
					{
						animDir = 0;
					}
				}
			}
			break;
		case kTypeIdle_FaceUp_Stand_MAIN:
		case kTypeIdle_FaceUp_Stand_OTH:
		case kTypeRun_FaceUp_MAIN:
		case kTypeRun_FaceUp_OTH:
			{
				//if (m_pElf->IsOnlyMe())
				//{
				//	CCPoint elfPos = m_pElf->getPosition();
				//	CCPoint rolePos = m_pRole->getPosition();
				//	if (elfPos.x < rolePos.x)
				//	{
				//		animDir = 3;
				//	}
				//	else
				//	{
				//		animDir = 2;
				//	}
				//}
				//else
				{
					if (m_pElf->IsStandLeft())
					{
						animDir = 2;
					}
					else
					{
						animDir = 3;
					}
				}
			}
			break;
		}
	}

	return animDir;
}

unsigned int ElfBaseState::GetAnimId(int animDirectionID,bool &bFlip)
{
	switch(animDirectionID)
	{
	case 0:
		bFlip = false;
		return kTypeIdle_FaceDown_Stand_OTH;
		break;
	case 1:
		bFlip = true;
		return kTypeIdle_FaceDown_Stand_OTH;
		break;
	case 2:
		bFlip = true;
		return kTypeIdle_FaceUp_Stand_OTH;
		break;
	case 3:
		bFlip = false;
		return kTypeIdle_FaceUp_Stand_OTH;
		break;
	}
	return 0;
}

CCPoint ElfBaseState::GetCloseToHeroPoint(unsigned int followPointType)
{
	if (followPointType == 0)
	{
		return m_pElf->GetMoveTargetPos(true);
	}
	else if (followPointType == 1)
	{
		return m_pElf->GetMoveTargetPos(false);
	}
	return CCPointZero;
}

unsigned int ElfBaseState::GetFollowPointType(unsigned int skillId,bool bRelayVerDir /* = false */)
{
	switch(skillId)
	{
	case kTypeIdle_FaceHor_Stand_MAIN:
	case kTypeIdle_FaceHor_Stand_OTH:
	case kTypeRun_FaceHor_MAIN:
	case kTypeRun_FaceHor_OTH:
		{
			bool bFlipX = m_pRole->IsAnimFlipX();
			if (bFlipX)
			{
				return 0;
			}
			return 1;
		}
		break;
	case kTypeIdle_FaceDown_Stand_MAIN:
	case kTypeIdle_FaceDown_Stand_OTH:
	case kTypeIdle_FaceUp_Stand_MAIN:
	case kTypeIdle_FaceUp_Stand_OTH:
	case kTypeRun_FaceDown_MAIN:
	case kTypeRun_FaceDown_OTH:
	case kTypeRun_FaceUp_MAIN:
	case kTypeRun_FaceUp_OTH:
		{
			if (bRelayVerDir)
			{
				CCPoint elfPos = m_pElf->getPosition();
				CCPoint pointLeft = m_pElf->GetMoveTargetPos(true);
				CCPoint pointRight = m_pElf->GetMoveTargetPos(false);

				float disLeft = ccpLengthSQ(ccpSub(elfPos,pointLeft));
				float disRight = ccpLengthSQ(ccpSub(elfPos,pointRight));

				if(disLeft <= disRight)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
		}
		break;
	}
	return -1;
}

void ElfBaseState::SetContainer( StateContainer* pContainer )
{
	m_pContainer = pContainer; 
	ElfBaseState* pState = dynamic_cast<ElfBaseState*>(pContainer);
	if(pState)
	{
		if(pState->GetStateMachine())
		{
			SetStateMachine(pState->GetStateMachine());
		}
	}
}