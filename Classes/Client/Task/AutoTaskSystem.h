#ifndef _AUTO_TASK_SYSTEM_H_
#define _AUTO_TASK_SYSTEM_H_

#include "set"
//#include "SimpleTaskObserver.h"
#include "ObserverProtocol.h"

using namespace TXGUI;

class AutoTaskSystem : public TXGUI::ObserverProtocol
{
public:
	struct OneTaskInfo
	{
		unsigned int id;
		unsigned int task_id;

		OneTaskInfo()
		{
			id = task_id = 0;
		}
	};
public:
	AutoTaskSystem();
	virtual ~AutoTaskSystem();

	void ReadXML();

	virtual void onBroadcastMessage(BroadcastMessage *msg);
	//virtual void HandleOneEvent(ESimpleTaskEvent event);

	bool IsTaskBelongAutoTaskSystem(unsigned int taskId);
private:
	bool IsAutoSystemOpen();
private:
	std::set<unsigned int > mTaskSet;
	bool mIsLoadData;
};

#endif	//_AUTO_TASK_SYSTEM_H_