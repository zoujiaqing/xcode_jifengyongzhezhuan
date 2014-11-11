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
#include "ABoxAnimation.h"
#include <algorithm>
using namespace std;


bool compare_time(const ABoxAnimation::ABoxFrame& t1,const ABoxAnimation::ABoxFrame& t2)
{  
	return t1.fTimeStamp < t2.fTimeStamp;  
}

ABoxAnimation::ABoxAnimation(ABox& collisionBox)
	: m_nTotalFrames(0)
	,m_bFlipX(false)
	,m_ptOrigin(CCPointZero)
{
	m_collisionBox = collisionBox;
}

ABoxAnimation::~ABoxAnimation()
{

}

void ABoxAnimation::AddFrame(float fTime, ABox & box)
{
	ABoxFrame frame;
	frame.box = box;
	frame.fTimeStamp = fTime;	

	m_lstBoxes.push_back(frame);
	sort(m_lstBoxes.begin(), m_lstBoxes.end(), compare_time);
	m_nTotalFrames++;
}

void ABoxAnimation::RemoveFrame(int key)
{
	if (key < (int) m_lstBoxes.size())
	{
		vector<ABoxFrame>::iterator it = m_lstBoxes.begin();
		m_lstBoxes.erase(it + key);
		m_nTotalFrames ++;
	}
}

int ABoxAnimation::GetFrameCount()
{
	return m_nTotalFrames;
}

void ABoxAnimation::Play(float fTime)
{
	/// draw debug information
	int k = int(fTime * 24);
	if (m_nTotalFrames == 0)
		return ;

	k = k % m_nTotalFrames;

	ABoxFrame& frame = m_lstBoxes[k];
	DrawABox(frame.box);	
}

bool ABoxAnimation::TestIntersectABoxAtTime(ABox & box, float fTime)
{
	int k = int(fTime * 24);
	if (m_nTotalFrames == 0)
		return false;
	k = k % m_nTotalFrames;

	ABoxFrame& frame = m_lstBoxes[k];

	/// move the animation box to the new origin point
	CCPoint pt = frame.box.GetOrigin();
	frame.box.SetOrigin(this->m_ptOrigin);

	/// test the two box
	if (frame.box.IntersectABox(box))
	{
		frame.box.SetOrigin(pt);
		return true;
	}

	/// restore the old origin point
	frame.box.SetOrigin(pt);
	return false;
}

void ABoxAnimation::DrawABox(ABox & box)
{
	/// draw debug information

}

const ABox& ABoxAnimation::GetCollisionBox()
{
	return m_collisionBox;
}