/************************************************************************/
/* ÎÄ¼þÃèÊö£º¾«Áé×·ÖðÖ÷½Ç×´Ì¬                                           */
/************************************************************************/

#pragma once
#include "FAbstractTransition.h"
#include "BaseAttackState.h"
#include "ElfBaseState.h"

class SpriteElf;
class SpriteSeer;

class ElfChaseState : public ElfBaseState
{
public:
	ElfChaseState(BaseElfEvt *pEvt);
	virtual ~ElfChaseState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	bool IsInInnerCircle(float distance);
protected:
	float m_innerCircleRaidus;
	float m_maxSpeedCoefficient;
};