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
#include <list>
#include "ABox.h"
using namespace std;

class ABox;

class ABoxAnimation
{
public:
	struct ABoxFrame
	{
		float fTimeStamp;
		ABox  box;	

		ABoxFrame()
		{
			fTimeStamp = 0;
		}
	};
public:
	ABoxAnimation(ABox& collisonBox);

	~ABoxAnimation();

public:
	/// add one frame
	void AddFrame(float fTime, ABox & box);

	/// remove one frame
	void RemoveFrame(int key);

	/// get frame count
	int GetFrameCount();

	/// play the animation
	void Play(float fTime);
	
	/// test whether current animation intersect the given abox at a certain time
	bool TestIntersectABoxAtTime(ABox & box, float fTime);

	/// get collision box
	const ABox& GetCollisionBox();

	/// flag for flip , true if flipped current animation, then the collision box's position will be flipped 
	void SetAnimFlipX(bool bFlipX){m_bFlipX = bFlipX;};
	bool GetAnimFlipX() {return m_bFlipX;}

	/// origin of current animation 
	void    SetOrigin(CCPoint pt) {m_ptOrigin = pt;};
	CCPoint GetOrigin(){return m_ptOrigin;};

protected:
	/// draw current ABox
	void DrawABox(ABox & box);

protected:
	vector<ABoxFrame> m_lstBoxes;
	int  m_nTotalFrames;
	bool m_bFlipX;
	CCPoint m_ptOrigin;

	ABox m_collisionBox;
};