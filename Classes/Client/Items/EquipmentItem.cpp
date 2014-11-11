#include "EquipmentItem.h"

EquipmentItem::EquipmentItem()
{
	m_id = 0;
	b_isPiledUp = false;
	m_maxPiledNum = 0;
	m_byeprice = 0;
	m_sellprice = 0;
	m_requiredLevel = 0;
	m_type = 0;
	b_Destroyed = 0;
	m_quality = 0;
	m_ownerType = 0;
	m_equipPart = 0;
	m_equipLevel = 0; 
}

EquipmentItem::~EquipmentItem()
{

}

unsigned int EquipmentItem::getEquipLevel()
{
	return m_equipLevel;
}