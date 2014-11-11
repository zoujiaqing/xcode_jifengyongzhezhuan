#pragma once


#include <algorithm>
#include <list>
using namespace std;

class IEvent{};

class IEventHandler
{
public:
	virtual void RegisterEvent(IEvent* evtHandler) = 0;
	virtual void UnRegisterEvent(IEvent* evtHandler) = 0;
};


template<class E>
class EventProducer : public IEventHandler
{
public:
	virtual void RegisterEvent(IEvent* evtHandler)
	{
		IEvent* e = static_cast<IEvent*>(evtHandler);
		if (e)
		{
			if (find(m_lsHandler.begin(), m_lsHandler.end(), e) == m_lsHandler.end())
			{
				m_lsHandler.push_back(e);
			}
		}
	}

	virtual void UnRegisterEvent(IEvent* evtHandler)
	{
		IEvent* e = static_cast<IEvent*>(evtHandler);
		if (e)
		{
			list<IEvent*>::iterator it = find(m_lsHandler.begin(), m_lsHandler.end(), e);
			if (it != m_lsHandler.end())
			{
				m_lsHandler.erase(it);
			}
		}		
	}

	template<class Type> inline
		void FireEvent(void(Type::*EventName)())
	{
		list<IEvent*>temp;
		temp.insert(temp.begin(), m_lsHandler.begin(), m_lsHandler.end());

		list<IEvent*>::iterator it = temp.begin();
		for(; it != temp.end(); it++)
		{
			Type* e = (Type*)(*it);
			(e->*EventName)();
		}
	}

	template<class Type, class ArgT> inline
		void FireEvent(void(Type::*EventName)(ArgT), const ArgT& arg)
	{
		list<IEvent*>temp;
		temp.insert(temp.begin(), m_lsHandler.begin(), m_lsHandler.end());

		list<IEvent*>::iterator it = temp.begin();
		for(; it != temp.end(); it++)
		{
			Type* e = (Type*)(*it);
			if (e)
			{
				(e->*EventName)(arg);
			}			
		}
	}

	template<class Type, class Arg1T, class Arg2T> inline
		void FireEvent(void(Type::*EventName)(Arg1T, Arg2T), const Arg1T& arg1, const Arg2T& arg2)
	{
		list<IEvent*>temp;
		temp.insert(temp.begin(), m_lsHandler.begin(), m_lsHandler.end());

		list<IEvent*>::iterator it = temp.begin();
		for(; it != temp.end(); it++)
		{
			Type* e = (Type*)(*it);
			if (e)
			{
				(e->*EventName)(arg1, arg2);
			}
		}
	}

	template<class Type, class Arg1T, class Arg2T, class Arg3T> inline
		void FireEvent(void(Type::*EventName)(Arg1T, Arg2T, Arg3T), const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3)
	{
		list<IEvent*>temp;
		temp.insert(temp.begin(), m_lsHandler.begin(), m_lsHandler.end());

		list<IEvent*>::iterator it = temp.begin();
		for(; it != temp.end(); it++)
		{
			Type* e = (Type*)(*it);
			if (e)
			{
				(e->*EventName)(arg1, arg2, arg3);
			}
		}
	}

protected:
	list<IEvent*> m_lsHandler;
};


//// define socketEvent
class ISocketEvent : public IEvent
{
public:
	virtual void OnSocketError(int nError) = 0;
	virtual void OnConnected(int nResult) = 0;
	virtual void OnDisconnect() = 0;
};
