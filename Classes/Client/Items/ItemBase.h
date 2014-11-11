#ifndef ITEM_BASE_H
#define ITEM_BASE_H

#include "cocos2d.h"

class ItemBase : public cocos2d::CCNode
{
public:
	ItemBase();
	virtual ~ItemBase();

	virtual unsigned int getEquipLevel();
public:
	unsigned int	m_id;				//物品Id
	char			m_name[128];		//物品名
	bool			b_isPiledUp;		//是否可堆叠
	unsigned int	m_maxPiledNum;		//最大堆叠数量
	unsigned int	m_source[5];		//获取方式
	unsigned int	m_byeprice;			//购买价格
	unsigned int	m_sellprice;		//卖出价格
	unsigned int	m_requiredLevel;	//等级要求
	unsigned int	m_type;				//物品类型
	bool			b_Destroyed;		//是否可摧毁
	char            m_szDescription[512];	 //描述字段
	std::string			m_useDes;			// 使用描述
	unsigned int	m_quality;			//物品品质(用于边框和字体颜色)
	unsigned int	m_iconId;			//物品Id编号
	unsigned int	m_order;			//物品品质
	unsigned int	m_banch;			//是否可以一键使用
};

#endif