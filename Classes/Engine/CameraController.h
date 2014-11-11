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
#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class CameraController : public CCLayer
{
public:

	enum CameraFollowType
	{
		E_FollowHero = 0,
		E_FixPoint,
		E_CameraFollowUnKnow
	};
    
    static CameraController *sharedCameraController();
	static void purgeInstance();

	void ResetValue();

	CameraController();
	virtual ~CameraController();

	void Update(float dt);
    
    void shakeAndScale(float shakeTime);
    
    void shakeOnly(float shakeTime, float magnitude);
    void levelScale();

	void SetFollowPoint(CCPoint p)
	{
		m_followPoint = p;
	}

	void SetFollowType(CameraFollowType followType)
	{
		m_eCameraFollowType = followType;
	}
private:
    void shakeUpdate(float dt);
	void followToOnePoint(cocos2d::CCPoint p,float dt,bool bFollowRightNow = false);
    
    void revertArch();
    void setarch();
protected:
    
    static CameraController *s_pSharedCameraController;

	float m_fLastUpdateTime;
    
    float mShakeTime;
    float mShakeMagnitude;
    float mShakeOffsetY;
    
    double m_accumulator;
    double m_lastTickTime;

	CCPoint m_followPoint;
	CameraFollowType m_eCameraFollowType;

	CCPoint m_curCameraPoint;
	bool m_isStartCameraFollowX;
	bool m_isStartCameraFollowY;
	bool m_isStartCameraMove;
};