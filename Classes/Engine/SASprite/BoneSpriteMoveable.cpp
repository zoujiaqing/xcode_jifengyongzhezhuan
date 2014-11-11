#include "BoneSpriteMoveable.h"
#include "LevelDefine.h"

#include "GameEnginePlayer.h"
#include "BoneAnimSprite.h"
#include "HSJoystick.h"
#include "GameManager.h"
#include "GameDataManager.h"
#include "MainLandManager.h"

BoneSpriteMoveable::BoneSpriteMoveable(int type, bool isPlayerSet)
	:BoneSpriteBase(type)
	,m_fSpeed(1.0f)
	,m_isMoving(false)
	,m_map(NULL)
	,m_direction(DIRECTION_BOTTOM)
	,m_step(0)
	,m_Target(NULL)
	,m_func(NULL)
	,m_isPlayerSet(isPlayerSet)
	//,m_fMoveRate(210.0f)
	,m_fMoveRate(252.0f)
	,m_fAdditionMoveRate(0.0f)
	,m_eMoveType(MOVE_BY_MOUSE)
	,isPathFindPaused(false)
	,canMove(true)
	,m_colliderRect(CCRectZero)
	,m_oldDir(-1)
{
	m_eMoveType = GameDataManager::Get()->getHeroMoveType();

	//GameDataManager::Get()->setHeroMoveType(MOVE_BY_JOY);
	m_cellPoint = CCPointZero;
	resetPathFinding();	

	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if(!bMainLandLevel)
	{
		//m_fMoveRate = 200.0f;
		m_fMoveRate = 240.0f;
	}
	
}


BoneSpriteMoveable::~BoneSpriteMoveable()
{
	//CC_SAFE_DELETE(m_map);
}


void BoneSpriteMoveable::setPosition(const CCPoint &position)
{
	BoneSpriteBase::setPosition(position);
	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();
	if (m_map)
	{
		m_map->setStart(position);
	}

}


cocos2d::CCPoint BoneSpriteMoveable::getRandomPoint()
{
	return m_map->getRandomPoint();
}


//-----------------------------------------------------------------------
//		now prohibit other player touches
//-----------------------------------------------------------------------
bool BoneSpriteMoveable::canDealWithTouch(cocos2d::CCTouch* touch)
{
	return BoneSpriteBase::canDealWithTouch(touch);
}


//-----------------------------------------------------------------------
bool BoneSpriteMoveable::moveToTarget(cocos2d::CCPoint destPos,cocos2d::CCPoint startPos, 
	CCObject *target /* = NULL */, SEL_CallFunc finSelector /* = NULL */, 
	float additionMoveRate /* = 200.0f */, CCRect colliderRect /* = CCRectZero */)
{

	if (isPathFindPaused || !canMove)
	{
		return false;
	}

	m_Target = target;
	m_func = finSelector;
	//m_fMoveRate = moveRate;
	m_fAdditionMoveRate = additionMoveRate;
	m_colliderRect = colliderRect;

	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();
	if (m_map)
	{
		m_map->setStart(startPos);
		m_map->setEnd(destPos);
	}

	if (m_map && m_map->getStart() != -1 && m_map->getEnd() != -1) 
	{
		if( m_map->astarSearch())
		{
			m_step =1;
			if (m_step == m_map->getPathTop())
			{
				if (m_Target != 0 && m_func != 0)
				{
					(m_Target->*m_func)();
					m_Target = NULL;
					m_func = NULL;
				}
				return false;
			}
			else
			{
				m_isMoving = true;
				startMove();
				return true;
			}
		} 
		else
		{			     
			endMove();
			return false;
		}
	}
	return false;
}

float BoneSpriteMoveable::GetMoveSpeed()
{
	//if (m_fMoveRate != 200)
	//{
	//	CCLOG("value change");
	//}
	float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
	float moveRate = m_fMoveRate / deviceScale;
	float speed = 0;
	speed = moveRate * LevelMultiResolution::sGetLevelScale() * 1.0;
	speed += GetAdditionMoveSpeed();
	return speed;
}

float BoneSpriteMoveable::GetAdditionMoveSpeed()
{
	float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
	float moveRate = m_fAdditionMoveRate / deviceScale;
	float speed = 0;
	speed = moveRate * LevelMultiResolution::sGetLevelScale() * 1.0;
	return speed;
}

bool BoneSpriteMoveable::moveByJoystick(float dt)
{
	float speed = GetMoveSpeed();
 
	CCPoint velocity = HSJoystick::sharedJoystick()->getVelocity();
	if(velocity.x == 0 && velocity.y == 0)
	{
		m_isMoving = false;		
		updateWaitAnim();
		return false;
	}
	m_isMoving = true;
	float dx = speed * velocity.x * dt ;
	float dy = speed * velocity.y * dt ;

	CCPoint curPos = this->getPosition();
	CCPoint newPos = ccpAdd(curPos, ccp(dx,dy));
			
	updateDirection(newPos);
	updateRunAnim();

#if 1
	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();
	if(m_map && m_map->isPointReachable(newPos))
	{
		this->setPosition(newPos);
	}
	else if(m_map && m_map->isPointReachable(ccpAdd(curPos, ccp(dx,0))))
	{
		this->setPosition(ccpAdd(curPos, ccp(dx,0)));
	}
	else if(m_map && m_map->isPointReachable(ccpAdd(curPos, ccp(0,dy))))
	{
		this->setPosition(ccpAdd(curPos, ccp(0,dy)));
	}	
#else
	CCRect rect = getRelativeParentColliderRect();
	rect.origin.x += dx;
	rect.origin.y += dy;

	if(m_map && m_map->isPointReachable(rect))
	{
		this->setPosition(newPos);
	}
	else 
	{
		CCRect rect2(rect);
		rect2.origin.x += dx;
		if(m_map && m_map->isPointReachable(rect2))
		{
			this->setPosition(ccpAdd(curPos, ccp(dx,0)));
		}
		else 
		{
			CCRect rect3(rect);
			rect3.origin.y += dy;
			if(m_map && m_map->isPointReachable(rect3))
			{
				this->setPosition(ccpAdd(curPos, ccp(0,dy)));
			}
		}
	}
#endif
	return true;
}

void BoneSpriteMoveable::startMove()
{
	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();

	if (m_colliderRect.size.width != 0 && m_colliderRect.size.height != 0)
	{
		CCRect selfColliderRect = this->getRelativeParentColliderRect();
		if (selfColliderRect.intersectsRect(m_colliderRect))
		{
			m_rootSprite->stopAllActions();
			m_map->setStart(getPosition());
			endMove();

			return ;
		}
	}

	if (m_map && m_step < m_map->getPathTop()) 
	{
		float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();

		CCPoint pos = m_map->getPathPos(m_step);
		float Speed = GetMoveSpeed();

		updateDirection(pos);
		updateRunAnim();
		CCPoint pt1 = this->getPosition();
		CCPoint pt2 = pos;
		float distance =  sqrtf((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
		distance /= deviceScale;

		CCPoint destPoint = CCPoint(pos.x / deviceScale, pos.y / deviceScale);
		CCMoveTo *move2 = CCMoveTo::create( distance / Speed, destPoint);
		CCCallFunc *fun2 = CCCallFunc::create(this, callfunc_selector(BoneSpriteMoveable::startMove));
		CCFiniteTimeAction *sequence = CCSequence::create(move2,fun2,NULL);

		m_rootSprite->stopAllActions();
		m_rootSprite->runAction(sequence);        

		m_map->setStart(getPosition());
		m_step++;
	}
	else
	{  
		m_rootSprite->stopAllActions();
		m_map->setStart(getPosition());

		endMove();
	}
}

void BoneSpriteMoveable::endMove()
{
	m_isMoving = false;

	updateWaitAnim();

	//
	if (m_Target && m_func)
	{
		(m_Target->*m_func)();
	}

	m_Target = NULL;
	m_func = NULL;

	//m_fMoveRate = 2.0f;
	m_fAdditionMoveRate = 0.0f;
}


bool BoneSpriteMoveable::isMoving()
{
	return m_isMoving;
}

DIRECTION_ACTOR BoneSpriteMoveable::getDirection()
{	
	return DIRECTION_ACTOR(m_direction);
}

float BoneSpriteMoveable::GetSpeed()
{
	return m_fSpeed;
}

void  BoneSpriteMoveable::SetSpeed(float fValue)
{
	m_fSpeed = fValue;
}


void BoneSpriteMoveable::updateDirection(cocos2d::CCPoint destPos)
{
	cocos2d::CCPoint curPos = this->getPosition();

	float dy = destPos.y - curPos.y;
	float dx = destPos.x - curPos.x; 
	CCPoint dir(dx, dy);
	if (dir.x != 0 || dir.y != 0)
	{
		dir = ccpNormalize(dir);
	}
	setDirection(dir);
}



void BoneSpriteMoveable::setDirection( cocos2d::CCPoint normalVector, float rate )
{
	float dy = normalVector.y;
	float dx = normalVector.x; 
	
	if(dy == 0.0f && dx == 0.0f)
		return;

	m_direction =  DIRECTION_UNKNOW;

	if (fabs(dy ) > rate * fabs(dx))
	{
		if (dy > 0)
		{
			m_direction = DIRECTION_UP;
		}
		else
		{
			m_direction = DIRECTION_BOTTOM;
		}
	}
	else
	{
		if (dx > 0)
		{
			m_direction = DIRECTION_RIGHT;
		}
		else
		{
			m_direction = DIRECTION_LEFT;
		}
	}
}


void BoneSpriteMoveable::updateRunAnim()
{
	if(!m_animBone) return;
}


void BoneSpriteMoveable::updateWaitAnim()
{
	if(!m_animBone)	return;
}

cocos2d::CCPoint BoneSpriteMoveable::getDirectionVector()
{
	cocos2d::CCPoint dir;
	switch(m_direction)
	{
	case DIRECTION_LEFT:
		dir.setPoint(-1.0f, 0.0f);
		break;
	case DIRECTION_RIGHT:
		dir.setPoint(1.0f, 0.0f);
		break;
	case DIRECTION_UP:
		dir.setPoint(0.0f, 1.0f);
		break;
	case DIRECTION_BOTTOM:
		dir.setPoint(0.0f, -1.0f);
		break;
	default:
		dir.setPoint(-1.0f, 0.0f);
		break;
	}
	return dir;
}

bool BoneSpriteMoveable::isCellPosChange()
{
	CCPoint curCellPoint = CCPointZero;
	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();
	if (m_map)
	{
		curCellPoint = m_map->pointPixelToCell(getPosition());
		if ((int)m_cellPoint.x != (int)curCellPoint.x || 
			(int)m_cellPoint.y != (int)curCellPoint.y)
		{
			m_cellPoint = curCellPoint;
			return true;
		}
	}
	
	return false;
}

bool BoneSpriteMoveable::isPosChange()
{
	CCPoint curPos = CCPointZero;
	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();
	if (m_map)
	{
		curPos = getPosition();
		if ((int)m_prevPos.x != (int)curPos.x || 
			(int)m_prevPos.y != (int)curPos.y)
		{
			m_prevPos = curPos;
			return true;
		}
	}

	return false;
}

bool BoneSpriteMoveable::isDirectionChange()
{
	
	if (m_direction != m_oldDir)
	{
		m_oldDir = m_direction;
		return true;
	}
	

	return false;
}


BoneSpriteMoveable::MOVE_TO_RESULT BoneSpriteMoveable::moveTo( cocos2d::CCPoint destPos, float speed, CCCallFunc* endFunc /*= NULL */ )
{
	MOVE_TO_RESULT ret;

	CCPoint curPos = getPosition();
	CCPoint pInPixel = curPos;
	CCPoint oldPoint = calMoveToEndPoint(destPos);

	/*if (curPos.x != oldPoint.x ||
	curPos.y != oldPoint.y)*/
	{
		CCArray *arrayOfActions = CCArray::create();

		float distance = sqrtf((oldPoint.x - curPos.x)*(oldPoint.x - curPos.x)+(oldPoint.y - curPos.y)*(oldPoint.y - curPos.y));
		CCMoveTo *move2 = CCMoveTo::create( distance / speed, oldPoint);

		/*if(endFunc == NULL)
		{
		endFunc = CCCallFuncND::create(NULL, 
		callfuncND_selector(NULL),
		(void *)NULL);
		}*/

		CCCallFunc *fun2 = CCCallFunc::create(this, callfunc_selector(BoneSpriteMoveable::endMove));

		arrayOfActions->addObject(move2);
		arrayOfActions->addObject(endFunc);
		arrayOfActions->addObject(fun2);

		CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);
		ret.tag = *(unsigned int*)sequence;
		sequence->setTag(ret.tag);

		m_rootSprite->stopAllActions();
		m_rootSprite->runAction(sequence);
		ret.pAction = sequence;

		m_isMoving = true;
	}

	

	ret.destPos = oldPoint;
	return ret;
}

BoneSpriteMoveable::MOVE_TO_RESULT BoneSpriteMoveable::moveToNoPhysics( cocos2d::CCPoint destPos, float speed, CCCallFunc* endFunc /*= NULL */ )
{
	MOVE_TO_RESULT ret;

	CCPoint curPos = getPosition();
	CCPoint pInPixel = curPos;
	CCPoint oldPoint = destPos;	


		
	CCArray *arrayOfActions = CCArray::create();

	float distance = sqrtf((oldPoint.x - curPos.x)*(oldPoint.x - curPos.x)+(oldPoint.y - curPos.y)*(oldPoint.y - curPos.y));
	CCMoveTo *move2 = CCMoveTo::create( distance / speed, oldPoint);

	/*if(endFunc == NULL)
	{
	endFunc = CCCallFuncND::create(NULL, 
	callfuncND_selector(NULL),
	(void *)NULL);
	}*/

	CCCallFunc *fun2 = CCCallFunc::create(this, callfunc_selector(BoneSpriteMoveable::endMove));

	arrayOfActions->addObject(move2);
	if(endFunc)
		arrayOfActions->addObject(endFunc);
	arrayOfActions->addObject(fun2);

	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

	m_rootSprite->stopAllActions();
	m_rootSprite->runAction(sequence);
	ret.pAction = sequence;

	m_isMoving = true;

	ret.destPos = oldPoint;
	return ret;
}

void BoneSpriteMoveable::pausePathFinding()
{
	if (isPathFindPaused)
	{
		return;
	}
	LevelLayer * layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (layer)
	{
		CPhysicsLayer * mapLayer = layer->getPhysicsLayer();
		CCPoint endPos = mapLayer->getPathPos(mapLayer->getPathTop() - 1);

		pathFindInfo.destPos = endPos;
		pathFindInfo.target = m_Target;
		pathFindInfo.finSelector = m_func;
		pathFindInfo.moveRate = m_fMoveRate;
		moveToTarget(getPosition(), getPosition());
	}	
	isPathFindPaused = true;	
}

void BoneSpriteMoveable::resumePathFinding()
{
	if (pathFindInfo.destPos.x == 0 && pathFindInfo.destPos.y == 0)
	{
		return;
	}
	SetAdditionMoveRate(0.0f);
	moveToTarget(pathFindInfo.destPos, getPosition(), pathFindInfo.target, pathFindInfo.finSelector, pathFindInfo.moveRate - GetMoveRate());
	resetPathFinding();	
}

void BoneSpriteMoveable::resetPathFinding()
{
	pathFindInfo.destPos.x = 0;
	pathFindInfo.destPos.y = 0;
	pathFindInfo.target = NULL;
	pathFindInfo.finSelector = NULL;
	pathFindInfo.moveRate = 200.0f;
	isPathFindPaused = false;
}


void BoneSpriteMoveable::setCanMove(bool move)
{	
	if (!move)
	{
		moveToTarget(getPosition(), getPosition());
	}
	//后赋值保证上面的可以执行
	canMove = move;
}

cocos2d::CCPoint BoneSpriteMoveable::calMoveToEndPoint( cocos2d::CCPoint& destPos)
{
	CCPoint curPos = getPosition();
	CCPoint pInPixel = curPos;
	CCPoint oldPoint = pInPixel;	

	//m_map = LevelManager::sShareInstance()->getCurrentLevelLayer()->getPhysicsLayer();
	if (m_map)
	{
		CCPoint startCellPoint = m_map->pointPixelToCell(curPos);
		CCPoint endCellPoint = m_map->pointPixelToCell(destPos);

		float _distance = sqrtf((startCellPoint.x - endCellPoint.x)*(startCellPoint.x - endCellPoint.x)
			+(startCellPoint.y - endCellPoint.y)*(startCellPoint.y - endCellPoint.y));

		CCPoint dir = ccpSub(endCellPoint, startCellPoint);
		if (dir.x == 0 && dir.y == 0)
		{
			return oldPoint;
		}
		else
		{
			dir = ccpNormalize(dir);
		}		

		float newPosX = startCellPoint.x;
		float newPosY = startCellPoint.y;

		int index = 0;
		while(true)
		{
			float length = (index+1);
			newPosX = startCellPoint.x + dir.x * length;
			newPosY = startCellPoint.y + dir.y * length;

			if (_distance < (dir.x * length)*(dir.x * length) + (dir.y * length)*(dir.y * length))
			{
				newPosX = endCellPoint.x;
				newPosY = endCellPoint.y;

				pInPixel = m_map->pointCellToPixel(ccp(newPosX,newPosY));
				if (m_map->isPointReachable(pInPixel))
				{
					oldPoint = pInPixel;
				}
				break;
			}

			pInPixel = m_map->pointCellToPixel(ccp(newPosX,newPosY));
			if (false == m_map->isPointReachable(pInPixel))
			{
				break;
			}
			else
			{
				oldPoint = pInPixel;
			}

			index ++;
		}

	}

	return oldPoint;
}
