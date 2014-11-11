#ifndef DEFEREVENT_HPP_
#define DEFEREVENT_HPP_

#include "Event.hpp"
#include "EventableObject.hpp"
#include "Singleton.hpp"
#include <list>

namespace base {

class DeferManager;

class DeferEvent: public Event {
public:
	DeferEvent(EventableObject* ev_owner, CallbackBase* cb);

friend class DeferManager;
};

/**
  * @brief 事件管理器。使用它可以管理继承自EventableObject的子类的定时事件。
  */
class DeferManager {
private:
	typedef std::list<DeferEvent*> EventList;

public:
	virtual ~DeferManager();

	void clear();

	bool empty();
	
	EventHandler add_event(int (*method)())
	{
		Event::CallbackBase* cb = new Event::CallbackM0(method);
		
		DeferEvent* pEvent = do_add_static_event(cb);
		
		return EventHandler(pEvent);
	}

	template <typename P1>
	EventHandler add_event(int (*method)(P1&), const P1& p1)
    {
        Event::CallbackBase* cb = new Event::CallbackM1<P1>(method, p1);
        
		DeferEvent* pEvent = do_add_static_event(cb);
		
		return EventHandler(pEvent);
    }

	template <typename P1, typename P2>
	EventHandler add_event(int (*method)(P1&, P2&), const P1& p1, const P2& p2)
    {
        Event::CallbackBase* cb = new Event::CallbackM2<P1, P2>(method, p1, p2);
        
		DeferEvent* pEvent = do_add_static_event(cb);
		
		return EventHandler(pEvent);
    }

	template <typename P1, typename P2, typename P3>
	EventHandler add_event(int (*method)(P1&, P2&, P3&), const P1& p1, const P2& p2, const P3& p3)
    {
        Event::CallbackBase* cb = new Event::CallbackM3<P1, P2, P3>(method, p1, p2, p3);
        
		DeferEvent* pEvent = do_add_static_event(cb);
		
		return EventHandler(pEvent);
    }

	template <typename P1, typename P2, typename P3, typename P4>
	EventHandler add_event(int (*method)(P1&, P2&, P3&, P4&), const P1& p1, const P2& p2, const P3& p3, const P4& p4)
    {
        Event::CallbackBase* cb = new Event::CallbackM4<P1, P2, P3, P4>(method, p1, p2, p3, p4);
        
		DeferEvent* pEvent = do_add_static_event(cb);
		
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
	EventHandler add_event(Class& obj, int (Class::*method)())
	{
		Event::CallbackBase* cb = new Event::CallbackP0<Class>(&obj, method);

		DeferEvent* pEvent = do_add_event(&obj, cb);
	
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
	EventHandler add_event(Class& obj, int (Class::*method)(P1&), const P1& p1)
	{
		Event::CallbackBase* cb = new Event::CallbackP1<Class, P1>(&obj, method, p1);

		DeferEvent* pEvent = do_add_event(&obj, cb);
		
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
	EventHandler add_event(Class& obj, int (Class::*method)(P1&, P2&), const P1& p1, const P2& p2)
	{
		Event::CallbackBase* cb = new Event::CallbackP2<Class, P1, P2>(&obj, method, p1, p2);

		DeferEvent* pEvent = do_add_event(&obj, cb);
		
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
	EventHandler add_event(Class& obj, int (Class::*method)(P1&, P2&, P3&), const P1& p1, const P2& p2, const P3& p3)
	{
		Event::CallbackBase* cb = new Event::CallbackP3<Class, P1, P2, P3>(&obj, method, p1, p2, p3);

		DeferEvent* pEvent = do_add_event(&obj, cb);

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
	EventHandler add_event(Class& obj, int (Class::*method)(P1&, P2&, P3&, P4&), const P1& p1, const P2& p2, const P3& p3, const P4& p4)
	{
		Event::CallbackBase* cb = new Event::CallbackP4<Class, P1, P2, P3, P4>(&obj, method, p1, p2, p3, p4);

		DeferEvent* pEvent = do_add_event(&obj, cb);
		
		return EventHandler(pEvent);
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
	 * @brief 处理所有定时事件。该函数必须被定期调用，否则所有的定时事件都不会发生。
	 *        定期调用的时间间隔精度决定了定时事件发生的时间精度。
	 */
	void process_events();

private:
	DeferEvent* do_add_event(EventableObject* obj, Event::CallbackBase* cb)
	{
		// create a timed event
		DeferEvent* event = new DeferEvent(obj, cb);
		// add this event to the global event list, updating will all be done later on...
		m_allevents.push_back(event);
	
		// add this event to the object's list

		return event;
	}

	DeferEvent* do_add_static_event(Event::CallbackBase* cb)
	{
		// create a timed event
		DeferEvent* event = new DeferEvent(NULL, cb);
		// add this event to the global event list, updating will all be done later on...
		m_allevents.push_back(event);
	
		return event;
	}

private:
	EventList	m_allevents;
};

} // end of namespace base



#endif
