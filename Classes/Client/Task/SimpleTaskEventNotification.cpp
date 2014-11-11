#include "SimpleTaskEventNotification.h"

SimpleTaskEventNotification::SimpleTaskEventNotification()
{

}

SimpleTaskEventNotification::~SimpleTaskEventNotification()
{

}

void SimpleTaskEventNotification::registerMsgObserver(SimpleTaskObserver*observer, ESimpleTaskEvent taskEvent)
{
	std::map<ESimpleTaskEvent,std::set<SimpleTaskObserver*> >::iterator iter = m_mapEventAndObservers.find(taskEvent);
	if (iter == m_mapEventAndObservers.end())
	{
		std::set<SimpleTaskObserver*> observerSet;
		observerSet.insert(observer);

		m_mapEventAndObservers.insert(std::make_pair(taskEvent,observerSet));
	}
	else
	{
		std::set<SimpleTaskObserver*> &observerSet = (*iter).second;
		observerSet.insert(observer);
	}
}

void SimpleTaskEventNotification::unregisterMsgObserver(SimpleTaskObserver*observer, ESimpleTaskEvent taskEvent)
{
	std::map<ESimpleTaskEvent,std::set<SimpleTaskObserver*> >::iterator iter = m_mapEventAndObservers.find(taskEvent);
	if (iter == m_mapEventAndObservers.end())
	{
		return ;
	}
	std::set<SimpleTaskObserver*> &observerSet = (*iter).second;
	std::set<SimpleTaskObserver*>::iterator iterSet = observerSet.find(observer);
	if (iterSet != (observerSet).end())
	{
		observerSet.erase(iterSet);
	}	
}

void SimpleTaskEventNotification::broadCastMessage(ESimpleTaskEvent taskEvent)
{
	std::map<ESimpleTaskEvent,std::set<SimpleTaskObserver*> >::iterator iter = m_mapEventAndObservers.find(taskEvent);
	if (iter == m_mapEventAndObservers.end())
	{
		return ;
	}

	for (std::set<SimpleTaskObserver*>::iterator _iter = ((*iter).second).begin();
		_iter != ((*iter).second).end(); _iter++)
	{
		SimpleTaskObserver *observer = (*_iter);
		if (observer)
		{
			observer->HandleOneEvent(taskEvent);
		}
	}
}