#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <map>
#include <vector>
#include "Time.hpp"
#include "EventableObject.hpp"
#include "sigslot.hpp"

namespace base {

class EventHandler;

class Event : public sigslot::has_slots<> {
public:	
	class CallbackBase {
	public:
		virtual int execute() = 0;
		virtual ~CallbackBase() { };
	};

	class CallbackM0 : public CallbackBase {
	public:
		typedef int (*Method)();
	public:
		CallbackM0(Method method)
			{ m_func = method; }

		int execute()
			{ return m_func(); }


	private:
		Method m_func;
	};
	
	template < typename P1 >
	class CallbackM1 : public CallbackBase {
	public:
		typedef int (*Method)(P1&);
	public:
		CallbackM1(Method method, const P1& p1)
			{ m_func = method; m_p1 = p1; }

		int execute()
			{ return m_func(m_p1); }
	

	private:
		Method	m_func;
		P1		m_p1;
	};
	
	template < typename P1, typename P2 >
	class CallbackM2 : public CallbackBase {
	public:
		typedef int (*Method)(P1&, P2&);
	public:
		CallbackM2(Method method, const P1& p1, const P2& p2)
			{ m_func = method; m_p1 = p1; m_p2 = p2; }

		int execute()
			{ return m_func(m_p1, m_p2); }
		

	private:
		Method	m_func;
		P1		m_p1;
		P2		m_p2;
	};

	template < typename P1, typename P2, typename P3 >
	class CallbackM3 : public CallbackBase {
	public:
		typedef int (*Method)(P1&, P2&, P3&);
	public:
		CallbackM3(Method method, const P1& p1, const P2& p2, const P3& p3)
			{ m_func = method; m_p1 = p1; m_p2 = p2; m_p3 = p3; }

		int execute()
			{ return m_func(m_p1, m_p2, m_p3); }
		

	private:
		Method	m_func;
		P1		m_p1;
		P2		m_p2;
		P3		m_p3;
	};

	template < typename P1, typename P2, typename P3, typename P4 >
	class CallbackM4 : public CallbackBase {
	public:
		typedef int (*Method)(P1&, P2&, P3&, P4&);
	public:
		CallbackM4(Method method, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
			{ m_func = method; m_p1 = p1; m_p2 = p2; m_p3 = p3; m_p4 = p4;}

		int execute()
			{ return m_func(m_p1, m_p2, m_p3, m_p4); }
		

	private:
		Method	m_func;
		P1		m_p1;
		P2		m_p2;
		P3		m_p3;
		P4		m_p4;
	};


	template < typename Class >
	class CallbackP0 : public CallbackBase {
	public:
		typedef int (Class::*Method)();

	public:
		CallbackP0(Class* class_instance, Method method)
			{ m_obj  = class_instance; m_func = method; }

		int execute()
			{ return (m_obj->*m_func)(); }


	private:	
		Class*	m_obj;
		Method	m_func;
	};

	template < typename Class, typename P1 >
	class CallbackP1 : public CallbackBase {
	public:
		typedef int (Class::*Method)(P1&);

	public:
		CallbackP1(Class* class_instance, Method method, const P1& p1)
			{ m_obj = class_instance; m_func = method; m_p1 = p1; }

		int execute()
			{ return (m_obj->*m_func)(m_p1); }
		

	private:	
		Class*	m_obj;
		Method	m_func;
		P1		m_p1;
	};

	template < typename Class, typename P1, typename P2 >
	class CallbackP2 : public CallbackBase {
	public:
		typedef int (Class::*Method)(P1&, P2&);

	public:
		CallbackP2(Class* class_instance, Method method, const P1& p1, const P2& p2)
			{ m_obj = class_instance; m_func = method; m_p1 = p1; m_p2 = p2; }

		int execute()
			{ return (m_obj->*m_func)(m_p1, m_p2); }
		

	private:	
		Class*	m_obj;
		Method	m_func;
		P1		m_p1;
		P2		m_p2;
	};

	template < class Class, typename P1, typename P2, typename P3 >
	class CallbackP3 : public CallbackBase {
	public:	
		typedef int (Class::*Method)(P1&, P2&, P3&);

	public:
		CallbackP3(Class* class_instance, Method method, const P1& p1, const P2& p2, const P3& p3)
			{ m_obj = class_instance; m_func = method; m_p1 = p1; m_p2 = p2; m_p3 = p3; }

		int execute()
			 {return (m_obj->*m_func)(m_p1, m_p2, m_p3); }
		

	private:	
		Class*	m_obj;
		Method	m_func;
		P1		m_p1;
		P2		m_p2;
		P3		m_p3;
	};

	template < class Class, typename P1, typename P2, typename P3, typename P4 >
	class CallbackP4 : public CallbackBase {
	public:
		typedef int (Class::*Method)(P1&, P2&, P3&, P4&);

	public:
		CallbackP4(Class* class_instance, Method method, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
			{ m_obj = class_instance; m_func = method; m_p1 = p1; m_p2 = p2; m_p3 = p3; m_p4 = p4; }
	
		int execute()
			{ return (m_obj->*m_func)(m_p1, m_p2, m_p3, m_p4); }
		

	private:	
		Class*	m_obj;
		Method	m_func;
		P1		m_p1;
		P2		m_p2;
		P3		m_p3;
		P4		m_p4;
	};

private:
	sigslot::signal1<Event*> sig_event_del;

public:
	
	~Event();

	EventableObject* ower() {
		return m_ev_owner;
	}

	Event(EventableObject* ev_owner,CallbackBase* cb):
		m_ev_owner(ev_owner), m_cb(cb) {
		if(ev_owner) {
			ev_owner->sig_event_disable.connect(this, &Event::deactivate);
		}
		assert(m_cb);
	}

	int execute() {
		if(m_cb) {
			return m_cb->execute();
		}
		return 0;
	}
	
	bool is_active() {
		return m_cb  != NULL;
	}

	void deactivate(EventableObject* object=NULL);

protected:
	
	EventableObject*    m_ev_owner;

    CallbackBase*       m_cb;

friend class EventHandler;
};

class EventHandler : public sigslot::has_slots<> {
public:
	EventHandler(Event* event=NULL);

	EventHandler& operator=(const EventHandler& other);

	bool available();

	void deactivate();

	bool is_activate();

	void unavailable(Event* event);

protected:
	Event *m_event;
};

};

#endif
