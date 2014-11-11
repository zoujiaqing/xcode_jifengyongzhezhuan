#ifndef _SYSTEM_CONDITION_MANAGER_H_
#define _SYSTEM_CONDITION_MANAGER_H_

#include "OnlineNetworkManager.h"
#include "Singleton_t.h"
#include "SystemConditionDataCenter.h"

class SystemConditionManager : public TSingleton<SystemConditionManager>
{
public:
	SystemConditionManager();
	~SystemConditionManager();

	void Init();

	void SendSystemCheckMessage();
	void TackTaskStatus(unsigned int task_id,PB_TASK_STATUS_T status);
	int GetTaskStatus(unsigned int task_id);
private:
	bool mIsSendMessage;
	SystemConditionDataCenter *mDataCenter;
};

#endif	//_SYSTEM_CONDITION_MANAGER_H_