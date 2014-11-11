#include "ItemBase.h"

ItemBase::ItemBase()
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
	m_useDes = "";
	m_order = 0;
	m_banch = 0;
}

ItemBase::~ItemBase()
{

}

unsigned int ItemBase::getEquipLevel()
{
	return 0;
}