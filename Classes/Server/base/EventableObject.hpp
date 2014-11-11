#ifndef EVENTABLEOBJECT_HPP_
#define EVENTABLEOBJECT_HPP_

#include <cassert>
#include <set>
#include "sigslot.hpp"

namespace base {

class EventableObject {

public:
	sigslot::signal1<EventableObject*> sig_event_disable;

	virtual ~EventableObject()
	{
		sig_event_disable(this);
	}

	EventableObject(){};

private:
	EventableObject(const EventableObject& obj);

	EventableObject& operator=(const EventableObject& obj);

};


};

#endif
