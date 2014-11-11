#pragma once

#include "Singleton_t.h"
#include "cs_core.pb.h"
#include <vector>

using namespace std;

struct MonthCardInfo
{
	MonthCardInfo()
	{
		card_id = 0;
		is_received = false;
		day_count = 0;
		diamond_amount = 0;
		is_alarm = false;
	}
	unsigned int card_id;
	bool is_received;
	unsigned int day_count;
	unsigned int diamond_amount;
	bool is_alarm;
};

class MonthCardManager : public TSingleton<MonthCardManager>
{
public:
	MonthCardManager();
	~MonthCardManager();

	//void LoadData();
	void resetUserMonthCardInfo();
	bool hasMonthCard();
	MonthCardInfo getOneMonthCard();
	bool getMonthCardInfoByID(MonthCardInfo& info,unsigned int cardID);
	void setMonthCardInfo(CSMcardInfoRsp* msg);
	void onReceiveGetSuccess(unsigned int cardID);

private:
	void addNewCardInfo(MonthCardInfo info);

private:
	vector<MonthCardInfo> m_monthCardVec;
};