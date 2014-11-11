#include "AreaItemNode.h"

AreaItemNode::AreaItemNode()
{
    m_bUpdate = true;
}

AreaItemNode::~AreaItemNode()
{

}

bool AreaItemNode::IsTwoRectsOverlapped(const CCRect &rect_1,const CCRect &rect_2)
{
	return rect_1.intersectsRect(rect_2);
}