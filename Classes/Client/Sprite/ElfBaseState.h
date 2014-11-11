/************************************************************************/
/* 文件描述：精灵追逐主角状态                                           */
/************************************************************************/

#pragma once
#include "FAbstractTransition.h"
#include "BaseAttackState.h"

class SpriteElf;
class SpriteSeer;
struct FAIRY_SKILL_INFO;
class StateContainer;
class SkillStateMachine;
class ElfStateFactory;

class ElfBaseState : public FState
{
public:
	ElfBaseState(BaseElfEvt *pEvt);
	virtual ~ElfBaseState(void);

	virtual void Reset();

	virtual bool Entered();

	virtual void Update(float dt);

	virtual void Exited();


	const FAIRY_SKILL_INFO* GetSkillInfo(void){return m_pSkill;}

	void SetStateMachine(SkillStateMachine* pStateMachine){m_pSkillStateMachine = pStateMachine;}
	SkillStateMachine* GetStateMachine(void){return m_pSkillStateMachine;}

	void SetContainer(StateContainer* pContainer);

	void SetEventType(EventType type){m_Type = type;}

protected:
	/**
	* Instruction : 根据跟随点的类型获取当前的靠近点
	* @param 
	*/	
	virtual CCPoint GetCloseToHeroPoint(unsigned int followPointType);

	/**
	* Instruction : 获取跟随点的类型 0 左侧 1 右侧
	* @param 
	*/
	virtual unsigned int GetFollowPointType(unsigned int skillId,bool bRelayVerDir = false);

	/**
	* Instruction : 获取动画的方向，根据方向
	* @param 
	* @return 0 left_down 1 rihgt_down 2 left_up 3 right_up
	*/
	virtual int GetAnimDirection(CCPoint dir);

	/**
	* Instruction : 根据角色ID得到方向
	* @param 
	* @return 0 left_down 1 rihgt_down 2 left_up 3 right_up
	*/
	virtual int GetAnimDirection(unsigned int skillId);

	/**
	* Instruction : 获取动画的ID
	* @param 
	*/
	virtual unsigned int GetAnimId(int animDirectionID,bool &bFlip);



protected:
	SpriteSeer* m_pRole;
	SpriteElf* m_pElf;
	unsigned int m_actionId;
	// Note: 添加技能
	unsigned int m_normalActionId;
	unsigned int m_assistActionId;
	BaseElfAttachData *m_pAttachData;

	BaseElfEvt* m_pEvt;
	float m_fMoveSpeed;

	unsigned int m_tempAnimDirection;
	bool m_isFlip;

	const FAIRY_SKILL_INFO* m_pSkill;
	const FAIRY_SKILL_INFO* m_pNormalSkill;
	const FAIRY_SKILL_INFO* m_pAssistSkill;
	StateContainer* m_pContainer;
	int linktime ;
	SkillStateMachine* m_pSkillStateMachine;
	ElfStateFactory* m_pStateFactory;
};