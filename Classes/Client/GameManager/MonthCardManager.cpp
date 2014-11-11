#include "MonthCardManager.h"
#include "NotificationCenter.h"
#include "tolua_fix.h"
#include "CCLuaEngine.h"
#include "GMessage.h"

MonthCardManager::MonthCardManager()
{
	/*MonthCardInfo info;
	info.card_id = 89007;
	info.day_count = 0;
	info.is_alarm = false;
	info.is_received = false;
	info.diamond_amount = 100;
	m_monthCardVec.push_back(info);

	info.card_id = 89008;
	info.day_count = 5;
	info.is_alarm = false;
	info.is_received = false;
	info.diamond_amount = 100;
	m_monthCardVec.push_back(info);*/

}

MonthCardManager::~MonthCardManager()
{
	m_monthCardVec.clear();
}

//void MonthCardManager::LoadData()
//{
//}

void MonthCardManager::resetUserMonthCardInfo()
{
	m_monthCardVec.clear();
}

bool MonthCardManager::getMonthCardInfoByID(MonthCardInfo& info,unsigned int cardID)
{
	bool hasMCard = false;
	for(vector<MonthCardInfo>::iterator iter = m_monthCardVec.begin();iter != m_monthCardVec.end();iter++)
	{
		if((*iter).card_id == cardID)
		{
			info = *iter;
			hasMCard = true;
			break;
		}
	}
	return hasMCard;
}

void MonthCardManager::addNewCardInfo(MonthCardInfo info)
{
	bool isUpdateData = false;
	for(vector<MonthCardInfo>::iterator iter = m_monthCardVec.begin();iter != m_monthCardVec.end();iter++)
	{
		if((*iter).card_id == info.card_id)
		{
			(*iter).day_count = info.day_count;
			(*iter).is_alarm = info.is_alarm;
			(*iter).is_received = info.is_received;
			(*iter).diamond_amount = info.diamond_amount;
			isUpdateData = true;
			break;
		}
	}
	if(!isUpdateData)
	{
		m_monthCardVec.push_back(info);
	}
}

void MonthCardManager::onReceiveGetSuccess(unsigned int cardID)
{
	TXGUI::NotificationCenter::defaultCenter()->broadcast(GM_MONTHCARD_GET_SUCCESS,this);
	//
}

MonthCardInfo MonthCardManager::getOneMonthCard()
{
	MonthCardInfo cardInfo;
	cardInfo.card_id = 0;
	for(vector<MonthCardInfo>::iterator iter = m_monthCardVec.begin();iter != m_monthCardVec.end();iter++)
	{
		if((!(*iter).is_received) || (*iter).is_alarm)
		{
			cardInfo = (*iter);
			break;
		}
	}
	return cardInfo;
}

bool MonthCardManager::hasMonthCard()
{
	bool hasMonthCard = false;
	for(vector<MonthCardInfo>::iterator iter = m_monthCardVec.begin();iter != m_monthCardVec.end();iter++)
	{
		if((!(*iter).is_received) || (*iter).is_alarm)
		{
			hasMonthCard = true;
			break;
		}
	}
	return hasMonthCard;
}

void MonthCardManager::setMonthCardInfo(CSMcardInfoRsp* msg)
{
	if(msg != NULL)
	{
		unsigned int count = msg->info_size();
		for(unsigned int i =0;i < count;i++)
		{
			McardInfo cardInfo = msg->info(i);
			MonthCardInfo info;
			info.card_id = cardInfo.card_id();
			info.day_count = cardInfo.day_cnt();
			info.is_alarm = cardInfo.is_alarm();
			info.is_received = cardInfo.is_rev();
			info.diamond_amount = cardInfo.ret_diamond();
			//m_monthCardVec.push_back(info);
			addNewCardInfo(info);
		}
		TXGUI::NotificationCenter::defaultCenter()->broadcast(GM_MONTHCARD_GET_SUCCESS,this);
	}
}