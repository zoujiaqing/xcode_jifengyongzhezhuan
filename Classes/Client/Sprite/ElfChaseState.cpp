#include "ElfChaseState.h"
#include "SpriteElfManager.h"
#include "SpriteElfConfigFromLua.h"

ElfChaseState::ElfChaseState(BaseElfEvt *pEvt) : ElfBaseState(pEvt)
{
	m_Type = ToElfChase;
	m_innerCircleRaidus = SpriteElfConfigFromLuaManager::getInstance()->GetInnerCircleRadius();
	m_maxSpeedCoefficient = SpriteElfConfigFromLuaManager::getInstance()->GetMaxMoveSpeedCoefficient();
}

ElfChaseState::~ElfChaseState( void )
{
}

bool ElfChaseState::Entered()
{
	ElfBaseState::Entered();
	//CCLOG("ElfChaseState::Entered()----------------------------------");
	return true;
}

void ElfChaseState::Exited()
{	
	ElfBaseState::Exited();
	//CCLOG("ElfChaseState::Exited()----------------------------------");
}

void ElfChaseState::Update(float dt)
{
	ElfBaseState::Update(dt);
	
	CCPoint targetPoint = ccpAdd(m_pRole->getPosition(),m_pRole->getAttachPoint(kType_PLAYER_NAME));
	CCPoint elfPos = m_pElf->getPosition();

	CCPoint dir = ccpSub(targetPoint, elfPos);
	float dirLength = sqrt(ccpLengthSQ(dir));
	if (IsInInnerCircle(dirLength))
	{
		BaseElfAttachData *pAttachData = new BaseElfAttachData();
		pAttachData->bStartWithAcc = true;
		SpriteElfManager::Get()->PushOneElfCloseToState(m_pRole,0,pAttachData);
		return ;
	}

	if(dir.x != 0 || dir.y != 0)
	{
		dir = ccpNormalize(dir);
	}
	else
	{
		return ;
	}

	float speed = m_pRole->GetMoveSpeed() * m_maxSpeedCoefficient;

	float newPosX = elfPos.x + dt*speed*dir.x;
	float newPosY = elfPos.y + dt*speed*dir.y;

	m_pElf->setPosition(ccp(newPosX,newPosY));

	// Note: 更新方向
	bool bFlip = false;
	unsigned int animDirectionId = GetAnimDirection(dir);
	unsigned int animID = GetAnimId(animDirectionId,bFlip);
	if (animID != m_tempAnimDirection)
	{
		m_pElf->SetAnim(animID,0);
		m_pElf->SetAnimFlipX(bFlip);

		m_tempAnimDirection = animID;
		m_isFlip = bFlip;
	}
	else if(bFlip != m_isFlip)
	{
		m_pElf->SetAnimFlipX(bFlip);
		m_isFlip = bFlip;
	}
}

void ElfChaseState::Reset()
{
	ElfBaseState::Reset();
}

bool ElfChaseState::IsInInnerCircle(float distance)
{
	if (distance <= m_innerCircleRaidus)
	{
		return true;
	}
	return false;
}