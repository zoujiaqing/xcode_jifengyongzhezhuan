#ifndef TIMEREVENT_HPP_
#define TIMEREVENT_HPP_
#include "Event.hpp"
#include "EventableObject.hpp"
#include "Singleton.hpp"

namespace base {

class TimerManager;
class ClockInterface;

class TimerEvent: public Event {
public:
	TimerEvent(EventableObject* ev_owner, CallbackBase* cb, uint32_t interval, int repeat);

private:
	uint32_t	m_interval;
	int			m_repeat_times;
	Time		m_expired_tv;
friend class TimerManager;
friend class ClockInterface;
};

/**
  * @brief 事件管理器。使用它可以管理继承自EventableObject的子类的定时事件。
  */
class TimerManager {
private:
	typedef std::multimap<Time, TimerEvent*> EventMap;

public:
	virtual ~TimerManager();
	
	EventHandler add_event(int (*method)(), const Time& first_expired_tv, uint32_t interval =0, int repeats = 0 )
	{
		Event::CallbackBase* cb = new Event::CallbackM0(method);
		
		TimerEvent* pEvent = do_add_static_event(cb,first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
	}

	template <typename P1>
	EventHandler add_event(int (*method)(P1&), const P1& p1, const Time& first_expired_tv, uint32_t interval =0, int repeats = 0 )
    {
        Event::CallbackBase* cb = new Event::CallbackM1<P1>(method, p1);
        
		TimerEvent* pEvent = do_add_static_event(cb,first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
    }

	template <typename P1, typename P2>
	EventHandler add_event(int (*method)(P1&, P2&), const P1& p1, const P2& p2, const Time& first_expired_tv, uint32_t interval =0, int repeats = 0 )
    {
        Event::CallbackBase* cb = new Event::CallbackM2<P1, P2>(method, p1, p2);
        
		TimerEvent* pEvent = do_add_static_event(cb,first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
    }

	template <typename P1, typename P2, typename P3>
	EventHandler add_event(int (*method)(P1&, P2&, P3&), const P1& p1, const P2& p2, const P3& p3, const Time& first_expired_tv, uint32_t interval =0, int repeats = 0 )
    {
        Event::CallbackBase* cb = new Event::CallbackM3<P1, P2, P3>(method, p1, p2, p3);
        
		TimerEvent* pEvent = do_add_static_event(cb,first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
    }

	template <typename P1, typename P2, typename P3, typename P4>
	EventHandler add_event(int (*method)(P1&, P2&, P3&, P4&), const P1& p1, const P2& p2, const P3& p3, const P4& p4, const Time& first_expired_tv, uint32_t interval =0, int repeats = 0 )
    {
        Event::CallbackBase* cb = new Event::CallbackM4<P1, P2, P3, P4>(method, p1, p2, p3, p4);
        
		TimerEvent* pEvent = do_add_static_event(cb,first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
    }
	
	/**
	 * @brief 添加一件定时事件
	 * @param obj 事件拥有者
	 * @param method 事件发生时调用的回调函数。该回调函数返回0则一切正常，如果返回非0，则事件管理器会立刻把该定时事件删除。
	 * @param first_expired_tv 事件第一次发生的时间
	 * @param interval 事件第一次发生后，再次发生的间隔时间（单位是毫秒），配合repeats使用。如果不需要重复，则不用传递任何参数。
	 * @param repeats 事件重复次数，配合interval使用。如果希望事件第一次发生后，还要再重复发生一次，则传递参数1；
	 *                如果希望定时事件一直不断重复发生，则传递-1；如果不需要重复，则不用传递任何参数。
	 * @return 刚刚添加的定时事件指针。
	 */
	template <typename Class>
	EventHandler add_event(Class& obj, int (Class::*method)(), const Time& first_expired_tv, uint32_t interval = 0, int repeats = 0)
	{
		Event::CallbackBase* cb = new Event::CallbackP0<Class>(&obj, method);

		TimerEvent* pEvent = do_add_event(&obj, cb, first_expired_tv, interval, repeats);
	
		return EventHandler(pEvent);
	}

	/**
	 * @brief 添加一件定时事件
	 * @param obj 事件拥有者
	 * @param method 事件发生时调用的回调函数。该回调函数返回0则一切正常，如果返回非0，则事件管理器会立刻把该定时事件删除。
	 * @param p1 传递给回调函数method的参数
	 * @param first_expired_tv 事件第一次发生的时间
	 * @param interval 事件第一次发生后，再次发生的间隔时间（单位是毫秒），配合repeats使用。如果不需要重复，则不用传递任何参数。
	 * @param repeats 事件重复次数，配合interval使用。如果希望事件第一次发生后，还要再重复发生一次，则传递参数1；
	 *                如果希望定时事件一直不断重复发生，则传递-1；如果不需要重复，则不用传递任何参数。
	 * @return 刚刚添加的定时事件指针。
	 */
	template <typename Class, typename P1>
	EventHandler add_event(Class& obj, int (Class::*method)(P1&), const P1& p1, const Time& first_expired_tv, uint32_t interval = 0, int repeats = 0)
	{
		Event::CallbackBase* cb = new Event::CallbackP1<Class, P1>(&obj, method, p1);

		TimerEvent* pEvent = do_add_event(&obj, cb, first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
	}

	/**
	 * @brief 添加一件定时事件
	 * @param obj 事件拥有者
	 * @param method 事件发生时调用的回调函数。该回调函数返回0则一切正常，如果返回非0，则事件管理器会立刻把该定时事件删除。
	 * @param p1 传递给回调函数method的参数
	 * @param p2 传递给回调函数method的参数
	 * @param first_expired_tv 事件第一次发生的时间
	 * @param interval 事件第一次发生后，再次发生的间隔时间（单位是毫秒），配合repeats使用。如果不需要重复，则不用传递任何参数。
	 * @param repeats 事件重复次数，配合interval使用。如果希望事件第一次发生后，还要再重复发生一次，则传递参数1；
	 *                如果希望定时事件一直不断重复发生，则传递-1；如果不需要重复，则不用传递任何参数。
	 * @return 刚刚添加的定时事件指针。
	 */
	template <typename Class, typename P1, typename P2>
	EventHandler add_event(Class& obj, int (Class::*method)(P1&, P2&), const P1& p1, const P2& p2, const Time& first_expired_tv, uint32_t interval = 0, int repeats = 0)
	{
		Event::CallbackBase* cb = new Event::CallbackP2<Class, P1, P2>(&obj, method, p1, p2);

		TimerEvent* pEvent = do_add_event(&obj, cb, first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
	}

	/**
	 * @brief 添加一件定时事件
	 * @param obj 事件拥有者
	 * @param method 事件发生时调用的回调函数。该回调函数返回0则一切正常，如果返回非0，则事件管理器会立刻把该定时事件删除。
	 * @param p1 传递给回调函数method的参数
	 * @param p2 传递给回调函数method的参数
	 * @param p3 传递给回调函数method的参数
	 * @param first_expired_tv 事件第一次发生的时间
	 * @param interval 事件第一次发生后，再次发生的间隔时间（单位是毫秒），配合repeats使用。如果不需要重复，则不用传递任何参数。
	 * @param repeats 事件重复次数，配合interval使用。如果希望事件第一次发生后，还要再重复发生一次，则传递参数1；
	 *                如果希望定时事件一直不断重复发生，则传递-1；如果不需要重复，则不用传递任何参数。
	 * @return 刚刚添加的定时事件指针。
	 */
	template <typename Class, typename P1, typename P2, typename P3>
	EventHandler add_event(Class& obj, int (Class::*method)(P1&, P2&, P3&), const P1& p1, const P2& p2, const P3& p3, const Time& first_expired_tv, uint32_t interval = 0, int repeats = 0)
	{
		Event::CallbackBase* cb = new Event::CallbackP3<Class, P1, P2, P3>(&obj, method, p1, p2, p3);

		TimerEvent* pEvent = do_add_event(&obj, cb, first_expired_tv, interval, repeats);

		return EventHandler(pEvent);
	}

	/**
	 * @brief 添加一件定时事件
	 * @param obj 事件拥有者
	 * @param method 事件发生时调用的回调函数。该回调函数返回0则一切正常，如果返回非0，则事件管理器会立刻把该定时事件删除。
	 * @param p1 传递给回调函数method的参数
	 * @param p2 传递给回调函数method的参数
	 * @param p3 传递给回调函数method的参数
	 * @param p4 传递给回调函数method的参数
	 * @param first_expired_tv 事件第一次发生的时间
	 * @param interval 事件第一次发生后，再次发生的间隔时间（单位是毫秒），配合repeats使用。如果不需要重复，则不用传递任何参数。
	 * @param repeats 事件重复次数，配合interval使用。如果希望事件第一次发生后，还要再重复发生一次，则传递参数1；
	 *                如果希望定时事件一直不断重复发生，则传递-1；如果不需要重复，则不用传递任何参数。
	 * @return 刚刚添加的定时事件指针。
	 */
	template <typename Class, typename P1, typename P2, typename P3, typename P4>
	EventHandler add_event(Class& obj, int (Class::*method)(P1&, P2&, P3&, P4&), const P1& p1, const P2& p2, const P3& p3, const P4& p4, const Time& first_expired_tv, uint32_t interval = 0, int repeats = 0)
	{
		Event::CallbackBase* cb = new Event::CallbackP4<Class, P1, P2, P3, P4>(&obj, method, p1, p2, p3, p4);

		TimerEvent* pEvent = do_add_event(&obj, cb, first_expired_tv, interval, repeats);
		
		return EventHandler(pEvent);
	}

	/**
	 * @brief 删除一个定时事件
	 * @param ev 需要删除的定时事件
	 */
	void remove_event(TimerEvent* ev)
	{
		if(ev)
		{
			ev->deactivate();
		}
	}

	/**
	 * @brief 删除obj的所有定时事件
	 * @param obj obj的所有定时事件将被删除
	 */
	void remove_events(EventableObject& obj)
	{
		obj.sig_event_disable(&obj);
	}

	/**
	 * @brief 修改定时事件ev的发生时间。注意，在ev发生时的回调函数中调用该函数是不起作用的！
	 * @param ev 需要修改发生时间的定时事件
	 * @param tv 发生时间
	 */
	void modify_expired_tv(TimerEvent* ev, const Time& tv)
	{
		assert(m_allevents.count(ev->m_expired_tv));

		m_allevents.erase(ev->m_expired_tv);

		ev->m_expired_tv = tv;
		// add this event to the global event list, updating will all be done later on...
		m_allevents.insert(std::make_pair(ev->m_expired_tv, ev));
	}

	/**
	 * @brief 修改定时事件发生的间隔时间，单位是毫秒
	 * @param ev 定时事件
	 * @param interval 间隔时间，单位是毫秒
	 */
	void modify_interval(TimerEvent* ev, uint32_t interval)
	{
		ev->m_interval = interval;
	}

	/**
	 * @brief 处理所有定时事件。该函数必须被定期调用，否则所有的定时事件都不会发生。
	 *        定期调用的时间间隔精度决定了定时事件发生的时间精度。
	 */
	void process_events();

private:
	TimerEvent* do_add_event(EventableObject* obj, Event::CallbackBase* cb, const Time& first_expired_tv, uint32_t interval, int repeats)
	{
		// create a timed event
		TimerEvent* event = new TimerEvent(obj, cb, interval, repeats);
		event->m_expired_tv = first_expired_tv;
		// add this event to the global event list, updating will all be done later on...
		m_allevents.insert(std::make_pair(event->m_expired_tv, event));
	
		// add this event to the object's list

		return event;
	}

	TimerEvent* do_add_static_event(TimerEvent::CallbackBase* cb, const Time& first_expired_tv, uint32_t interval, int repeats)
	{
		// create a timed event
		TimerEvent* event = new TimerEvent(NULL, cb, interval, repeats);
		event->m_expired_tv = first_expired_tv;
		// add this event to the global event list, updating will all be done later on...
		m_allevents.insert(std::make_pair(event->m_expired_tv, event));
	
		return event;
	}

private:
	EventMap	m_allevents;
};

} // end of namespace base

#endif 
