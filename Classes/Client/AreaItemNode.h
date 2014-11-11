#pragma once

#include "cocos2d.h"
USING_NS_CC;

class AreaItemNode
{
public:
	AreaItemNode();
	~AreaItemNode();

	virtual CCPoint GetItemPosition() = 0;
	//virtual float GetHideDistance() = 0;

	virtual CCRect GetABBox() = 0;
	virtual void SetUpdateOrNot(bool bUpdate){m_bUpdate = bUpdate;}
	virtual bool GetUpdateEnabled(){return m_bUpdate;}
	static  bool IsTwoRectsOverlapped(const CCRect &rect_1,const CCRect &rect_2);
private:
	bool m_bUpdate;
};