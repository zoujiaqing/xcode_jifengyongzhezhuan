#pragma once

#include "BoneSpriteBase.h"
#include "../Sprite/PhysicsLayer.h"

using namespace cocos2d;

typedef struct PathFindInfo
{
	CCPoint destPos;
	CCObject * target;
	SEL_CallFunc finSelector;
	float moveRate;
}PathFindInfo;

//////////////////////////////////////////////////////////////////////////
//the base class for level moveable object
//////////////////////////////////////////////////////////////////////////
class BoneSpriteMoveable : public BoneSpriteBase
{
public:
	BoneSpriteMoveable(int type, bool isPlayerSet);
	virtual ~BoneSpriteMoveable();

public:
	struct MOVE_TO_RESULT
	{
		MOVE_TO_RESULT(void)
			:pAction(NULL)
			,tag(0)
		{

		}
		cocos2d::CCPoint destPos;
		cocos2d::CCAction* pAction;
		unsigned int tag;
	};

	//// 用的是A星算法
	virtual bool				moveToTarget(cocos2d::CCPoint destPos,cocos2d::CCPoint startPos,
		CCObject *target = NULL,
		SEL_CallFunc finSelector = NULL,
		float additionMoveRate = 0.0f,
		CCRect colliderRect = CCRectZero);
	
	// 没有用A星算法，可以沿着斜直线移动
	// 返回实际能到的点，这个点不一定等于destPos
	virtual MOVE_TO_RESULT moveTo(cocos2d::CCPoint destPos,
		float speed,	// pixels/s
		CCCallFunc* endFunc = NULL	// CCCallFuncND::create(classPointer, callfuncND_selector(class::fun), (void *)funParm);
		);
	virtual MOVE_TO_RESULT moveToNoPhysics(cocos2d::CCPoint destPos,
		float speed,	// pixels/s
		CCCallFunc* endFunc = NULL	// CCCallFuncND::create(classPointer, callfuncND_selector(class::fun), (void *)funParm);
		);

	CCPoint calMoveToEndPoint(cocos2d::CCPoint& destPos);

	virtual bool				moveByJoystick(float dt);

	/// get current move type value
	virtual Move_Type GetMoveType(){return m_eMoveType;}
	virtual void SetMoveType(Move_Type moveType){m_eMoveType = moveType;}

	//override base
	virtual void setPosition(const CCPoint &position);
	virtual cocos2d::CCPoint	getRandomPoint();

	virtual bool				canDealWithTouch(cocos2d::CCTouch* touch);

	virtual bool				isMoving();

	virtual bool				isCellPosChange();
	virtual bool				isDirectionChange();
	virtual bool				isPosChange();

	virtual DIRECTION_ACTOR     getDirection();

	virtual cocos2d::CCPoint     getDirectionVector();

	/// move speed , float 
	virtual float GetSpeed();
	virtual void  SetSpeed(float fValut);

	virtual void				updateDirection(cocos2d::CCPoint destPos);
	virtual void				setDirection(cocos2d::CCPoint normalVector, float rate = 1.19f /*dy / dx = 1.19*/);

	//暂停及继续寻路
	virtual void                pausePathFinding();
	virtual void				resumePathFinding();

	virtual bool                getIsPathFindPaused(){return isPathFindPaused;}

	void setCanMove(bool move);

	float GetMoveSpeed();
	float GetAdditionMoveSpeed();
	void  SetAdditionMoveRate(float rate){m_fAdditionMoveRate = rate;}
	float GetAdditionMoveRate(void){return m_fAdditionMoveRate;}
	float  GetMoveRate(void){return m_fMoveRate;}

protected:	
	virtual void				updateRunAnim();
	virtual void				updateWaitAnim();
	/**
	* Instruction : 
	* @param 
	* @result 根据返回结果决定是否进行寻路
	*/	
	virtual void				startMove();
	virtual void				endMove();

	virtual void                resetPathFinding();

protected:
	Move_Type  m_eMoveType;
	float m_fSpeed;
	float m_fMoveRate;
	float m_fAdditionMoveRate;
	//
	bool					m_isMoving;
	//
	int						m_direction;
	//
	int						m_step;

	// 
	CCPoint					m_cellPoint;
	CCPoint					m_prevPos;
	int						m_oldDir;
	//---------------------------------------------------------------------
	//			the flag is the diff with other players
	//---------------------------------------------------------------------
	bool					m_isPlayerSet;

	//
	CPhysicsLayer*				m_map;

	//
	CCObject*		m_Target;
	SEL_CallFunc			m_func;
	CCRect	m_colliderRect;

	PathFindInfo   pathFindInfo;
	bool isPathFindPaused;
	bool canMove;

};