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

#include "CameraController.h"
#include "cocos2d.h"
#include "LevelManager.h"
#include "SpriteSeer.h"
#include "SceneLayer.h"
#include "GameManager.h"
#include "LevelLayer.h"
#include "GameManager.h"
#include "CCShake.h"

USING_NS_CC;

#define SHAKE_FREQUENCY 45

CameraController *CameraController::s_pSharedCameraController = NULL;

CameraController *CameraController::sharedCameraController()
{
	if(s_pSharedCameraController == NULL)
	{
		s_pSharedCameraController = new CameraController();
	}
	return s_pSharedCameraController;
}

void CameraController::purgeInstance()
{
	if (s_pSharedCameraController)
	{
		delete s_pSharedCameraController;
		s_pSharedCameraController = NULL;
	}
}

CameraController::CameraController()
{
	m_fLastUpdateTime = 0;
    
    mShakeTime = 0.0f;
    mShakeMagnitude = 0.0f;
    mShakeOffsetY = 0.0f;

	m_eCameraFollowType = E_FollowHero;
	m_followPoint = CCPointZero;
	m_curCameraPoint = CCPointZero;
	m_isStartCameraFollowX = false;
	m_isStartCameraFollowY = false;
	m_isStartCameraMove = false;
}

CameraController::~CameraController()
{
}

void CameraController::ResetValue()
{
	m_eCameraFollowType = E_FollowHero;
}


void CameraController::revertArch()
{
    LevelLayer* level =  GameManager::Get()->GetSceneLayer()->getLevelLayer();
    level->setAnchorPoint(ccp(0,0));
}

void CameraController::setarch()
{
}


void CameraController::shakeAndScale(float shakeTime)
{
    mShakeTime = shakeTime;
    mShakeMagnitude = 0;
    
    LevelLayer* level =  GameManager::Get()->GetSceneLayer()->getLevelLayer();
    level->stopAllActions();
    level->setScale(1.0f);
    level->setAnchorPoint(ccp(0,0));
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCSize mapsize = level->getMapSizeInPixel();
    CCPoint heroPos = GameManager::Get()->getHero()->getPosition();
    
    float px = heroPos.x / winsize.width;
    float py = heroPos.y / winsize.height;
    
    CCPoint newar = ccp(px, py);
    
    level->setAnchorPoint(ccp(px,py));
    
    CCActionInterval* s1_out = CCScaleTo::create(0.09, 1.35);
    CCActionInterval* s1_in = CCScaleTo::create(0.11, 1.05);
    CCActionInterval* s2_out = CCScaleTo::create(0.06, 1.2);
    CCActionInterval* s2_in = CCScaleTo::create(0.07, 1.0);
    CCCallFunc* f1 = CCCallFunc::create(this, callfunc_selector(CameraController::revertArch));
    
    CCSequence* seq = (CCSequence*)CCSequence::create(s1_out,s1_in,
                                                      s2_out, s2_in,
                                                      f1, NULL);
    level->runAction(seq);
    
    /*
    else{
     
        CCActionInterval* s1_out = CCScaleTo::create(0.09, 1.2);
        CCActionInterval* s1_in = CCScaleTo::create(0.11, 1.0);
        CCActionInterval* revert = CCScaleTo::create(0.11, 1.0f);
        
        CCCallFunc* f1 = CCCallFunc::create(this, callfunc_selector(CameraController::revertArch));
        
        CCSequence* seq = (CCSequence*)CCSequence::create(s1_out,s1_in,revert,
                                                          f1, NULL);
        level->runAction(seq);
    }*/
   
}

void CameraController::shakeOnly(float shakeTime, float magnitude)
{
    mShakeTime = shakeTime;
    mShakeMagnitude = magnitude;  
}

void CameraController::levelScale()
{
    LevelLayer* level =  GameManager::Get()->GetSceneLayer()->getLevelLayer();
    level->setScale(1.0f);
    CCActionInterval*  actionBy = CCScaleBy::create(0.1, 1.2f, 1.2f);
    level->runAction( CCSequence::create(actionBy, actionBy->reverse(), NULL));
}

void CameraController::Update(float dt)
{
    //CCLOG("cameraController update:%f\n", dt);
	if (m_eCameraFollowType == E_FollowHero)
	{
		// update position
		SpriteSeer *seer = GameManager::Get()->getHero();
		if (seer == NULL)
			return;

		followToOnePoint(seer->getPosition(),dt);
	}
	else if(m_eCameraFollowType == E_FixPoint)
	{
		followToOnePoint(m_followPoint,dt,true);
	}
}

void CameraController::followToOnePoint(cocos2d::CCPoint p,float dt,bool bFollowRightNow /* = false */)
{
	const float PERIOD_TIME = 0.1f;
	const float PERIOD_CONST = 2.0f * 3.1415926f / PERIOD_TIME;	

	mShakeOffsetY = 0.0f;
	if (mShakeTime > 0.0f) {
		mShakeTime -= dt;
        
		mShakeOffsetY = (float) (sin(mShakeTime * PERIOD_CONST) * mShakeMagnitude);

	}

	// update position

	//CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCSize screenSize = CCDirector::sharedDirector()->getRenderSize();

	CCPoint center = ccp(screenSize.width / 2 , screenSize.height / 2);
	CCRect mapRect = LevelManager::sShareInstance()->getCurrentLevelLayer()->getMapRect();

	float ymin = mapRect.getMinY() + screenSize.height / 2;
	float ymax = mapRect.getMaxY() - screenSize.height / 2;
	float xmin = mapRect.getMinX() + screenSize.width / 2;
	float xmax = mapRect.getMaxX() - screenSize.width / 2; 

	CCPoint seerPos = p;
	seerPos.x = int(seerPos.x);
	seerPos.y = int(seerPos.y);
	CCPoint targetPos = ccpSub(seerPos, center);

	if(seerPos.x < xmin)
	{
		targetPos.x = 0;
	}

	if(seerPos.x > xmax)
	{
		targetPos.x = xmax - xmin;
	}

	if(seerPos.y < ymin)
	{
		targetPos.y = 0;
	}

	if(seerPos.y > ymax)
	{
		targetPos.y = ymax - ymin;
	}

	targetPos.y += mShakeOffsetY;

	if (bFollowRightNow)
	{
		m_curCameraPoint = targetPos;
	}
	else
	{
		//float speed = 180;
		////dt = 1.0 / 60;
		//
		//CCPoint destPt = ccpSub(targetPos,m_curCameraPoint);
		//if (m_isStartCameraMove)
		//{
		//	CCPoint direction = ccpNormalize(destPt);
		//	float moveX = dt * direction.x * speed;
		//	float moveY = dt * direction.y * speed;
		//	CCPoint newDestPt(m_curCameraPoint.x+moveX,m_curCameraPoint.y+moveY);
		//	float deltaX1 = targetPos.x - m_curCameraPoint.x;
		//	float deltaY1 = targetPos.y - m_curCameraPoint.y;
		//	float deltaX2 = targetPos.x - newDestPt.x;
		//	float deltaY2 = targetPos.y - newDestPt.y;
		//	if (deltaX1*deltaX2 <= 0 && deltaY1*deltaY2<=0)
		//	{
		//		// Note: 移动超过了目标点
		//		m_curCameraPoint = targetPos;
		//		m_isStartCameraMove = false;
		//	}
		//	else
		//	{
		//		m_curCameraPoint = newDestPt;
		//	}
		//}
		//else
		//{			
		//	float destLength = ccpLength(destPt);
		//	if (destLength > 8)
		//	{
		//		// Note: 开始靠近
		//		m_isStartCameraMove = true;
		//	}
		//}

		//if (m_isStartCameraFollowX)
		//{
		//	// Note: 负方向
		//	int direction = -1;
		//	if (targetPos.x - m_curCameraPoint.x >0)
		//	{
		//		// Note: 正方向
		//		direction = 1;
		//	}
		//	float disX = dt * 180 * direction;
		//	float deltaX1 = targetPos.x - m_curCameraPoint.x;
		//	float deltaX2 = targetPos.x - (m_curCameraPoint.x + disX);
		//	if (deltaX1 * deltaX2 <= 0)
		//	{
		//		m_curCameraPoint.x = targetPos.x;
		//		m_isStartCameraFollowX = false;
		//	}
		//	else
		//	{
		//		if (fabs(deltaX1) < 1)
		//		{
		//			m_curCameraPoint.x = targetPos.x;
		//			m_isStartCameraFollowX = false;
		//		}
		//		else
		//		{
		//			m_curCameraPoint.x += disX;
		//		}
		//	}
		//}
		//else
		//{
		//	float deltaX = targetPos.x - m_curCameraPoint.x;
		//	if (fabs(deltaX) > 1)
		//	{
		//		m_isStartCameraFollowX = true;
		//	}			
		//}

		//if (m_isStartCameraFollowY)
		//{
		//	// Note: 负方向
		//	int direction = -1;
		//	if (targetPos.y - m_curCameraPoint.y >0)
		//	{
		//		// Note: 正方向
		//		direction = 1;
		//	}
		//	float disY = dt * 180 * direction;
		//	float deltaY1 = targetPos.y - m_curCameraPoint.y;
		//	float deltaY2 = targetPos.y - (m_curCameraPoint.x + disY);
		//	if (deltaY1 * deltaY2 <= 0)
		//	{
		//		m_curCameraPoint.y = targetPos.y;
		//		m_isStartCameraFollowY = false;
		//	}
		//	else
		//	{
		//		if (fabs(deltaY1) < 1)
		//		{
		//			m_curCameraPoint.y = targetPos.y;
		//			m_isStartCameraFollowY = false;
		//		}
		//		else
		//		{
		//			m_curCameraPoint.y += disY;
		//		}
		//	}
		//}
		//else
		//{
		//	float deltaY = targetPos.y - m_curCameraPoint.y;
		//	if (fabs(deltaY) > 1)
		//	{
		//		m_isStartCameraFollowY = true;
		//	}
		//}

		if (m_curCameraPoint.x == 0 && m_curCameraPoint.y == 0)
		{
			m_curCameraPoint = targetPos;
		}
		else
		{
			float deltaX = targetPos.x - m_curCameraPoint.x;
			float deltaY = targetPos.y - m_curCameraPoint.y;

			float k = 0.2;
			float disX = k*deltaX;
			float disY = k*deltaY;

			if (fabs(deltaX) < 0.1)
			{
				m_curCameraPoint.x = targetPos.x;
			}		
			else
			{
				m_curCameraPoint.x = m_curCameraPoint.x + disX;
			}

			if (fabs(deltaY) < 0.1)
			{
				m_curCameraPoint.y = targetPos.y;
			}		
			else
			{
				m_curCameraPoint.y = m_curCameraPoint.y + disY;
			}
		}
	}
	CCDirector::sharedDirector()->setLevelRenderCameraOffset(ccp(int(m_curCameraPoint.x),int(m_curCameraPoint.y)));
}
