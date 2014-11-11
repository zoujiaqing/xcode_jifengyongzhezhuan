/************************************************************************/
/* 文件描述：精灵追逐主角状态                                           */
/************************************************************************/

#pragma once
#include "FAbstractTransition.h"
#include "BaseAttackState.h"
#include "ElfBaseState.h"

class SpriteElf;
class SpriteSeer;

class ElfCloseToState : public ElfBaseState
{
public:
	ElfCloseToState(BaseElfEvt *pEvt);
	virtual ~ElfCloseToState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();

	float GetGoTime()
	{
		return m_fGoTime;
	}

	bool IsInOuterCircle(float distance);
protected:
	float m_fGoTime;
	float m_accTotalTime;
	float m_accelateRate;
	float m_maxSpeed;
	// Note: m_followPointType精灵的跟随点 0 左侧 1 右侧
	unsigned int	  m_followPointType;
	// Note: 精灵加速跟随圈
	float m_outerCircleRadius;
	float m_startSpeedCoefficient;
};