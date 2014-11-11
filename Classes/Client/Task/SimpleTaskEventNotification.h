#ifndef _SIMPLE_TASK_EVENT_NOTIFICATION_H_
#define _SIMPLE_TASK_EVENT_NOTIFICATION_H_

#include "set"
#include "map"
#include "SimpleTaskEventEnum.h"
#include "SimpleTaskObserver.h"

class SimpleTaskEventNotification
{
public:
	SimpleTaskEventNotification();
	~SimpleTaskEventNotification();

	void registerMsgObserver(SimpleTaskObserver*observer, ESimpleTaskEvent taskEvent);
	void unregisterMsgObserver(SimpleTaskObserver*observer, ESimpleTaskEvent taskEvent);

	void broadCastMessage(ESimpleTaskEvent taskEvent);
private:
	std::map<ESimpleTaskEvent,std::set<SimpleTaskObserver*> > m_mapEventAndObservers;
};

#endif	// _SIMPLE_TASK_EVENT_NOTIFICATION_H_