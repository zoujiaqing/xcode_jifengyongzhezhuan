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

USING_NS_CC;


class ABox
{
public:
	ABox()
	{
		m_rect = CCRectZero;
		m_origio = CCPointZero ;
		m_worldRect.setRect(0, 0, 0, 0);
	}

	ABox(CCPoint& origio, CCRect& rect)
	{
		m_rect = rect;
		m_origio = origio ;
        float x = origio.x + rect.origin.x;
		float y = origio.y + rect.origin.y;
		m_worldRect.setRect(x, y, rect.size.width, rect.size.height);
	}

	bool ContainPointInPixel(CCPoint pt)
	{		
		return m_worldRect.containsPoint(pt);
	}

	bool IntersectABox(ABox & box)
	{
		if (this->m_worldRect.intersectsRect(box.m_worldRect))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetOrigin(CCPoint& pt)
	{
		m_origio = pt;
		float x = pt.x + m_rect.origin.x;
		float y = pt.y + m_rect.origin.y;
		m_worldRect.setRect(x, y, m_rect.size.width, m_rect.size.height);
	}

	CCPoint GetOrigin() {return m_origio;}

	ABox(const ABox& other)
	{
		m_rect = other.m_rect;
		m_origio = other.m_origio;
		m_worldRect = other.m_worldRect;
	}

	ABox& operator= (const ABox& other)
	{
		m_rect = other.m_rect;
		m_origio = other.m_origio;
		m_worldRect = other.m_worldRect;

		return *this;
	}

protected:
	CCRect  m_worldRect;
	CCPoint m_origio;
	CCRect  m_rect;
};