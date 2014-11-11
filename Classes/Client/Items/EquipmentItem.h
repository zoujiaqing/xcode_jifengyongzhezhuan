#ifndef EQUIPMENT_ITEM_H
#define EQUIPMENT_ITEM_H

#include "ItemBase.h"

class EquipmentItem : public ItemBase
{
public:
	EquipmentItem();
	virtual ~EquipmentItem();
	virtual unsigned int getEquipLevel();
public:
	unsigned int		m_ownerType;		//使用者类型
	unsigned int		m_equipPart;		//装备位置
	unsigned int		m_equipLevel;		//装备等级
};

#endif