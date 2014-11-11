#ifndef _SIMPLE_TASK_OBSERVER_H_
#define _SIMPLE_TASK_OBSERVER_H_

#include "TaskData.h"
#include "SimpleTaskEventEnum.h"

class SimpleTaskObserver
{
public:
	SimpleTaskObserver();
	~SimpleTaskObserver();
	virtual void HandleOneEvent(ESimpleTaskEvent event) = 0;
};

#endif	//_SIMPLE_TASK_OBSERVER_H_