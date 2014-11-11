#include "Event.hpp"

namespace base {

Event::~Event() {
	if(m_cb) {
		delete m_cb;
		m_cb = NULL;
	}
	sig_event_del(this);
};

void Event::deactivate(EventableObject* obj) {
	if(m_cb) {
		delete m_cb;
		m_cb = NULL;
	}
};

EventHandler::EventHandler(Event* event):
	m_event(event) {
	if(m_event) {
		m_event->sig_event_del.connect(this, &EventHandler::unavailable);
	}
}

EventHandler& EventHandler::operator=(const EventHandler& other) {
	if(m_event) {
		m_event->sig_event_del.disconnect(this);
	}
	if(other.m_event) {
		other.m_event->sig_event_del.connect(this, &EventHandler::unavailable);
	}
	m_event = other.m_event;
	return (*this);
}

bool EventHandler::available() {
	return (m_event != NULL);
}

void EventHandler::deactivate() {
	if(m_event) {
		m_event->deactivate();
	}
}

bool EventHandler::is_activate() {
	if( m_event && m_event->is_active() ) {
		return true;
	}
	return false;
}

void EventHandler::unavailable(Event* event) {
	if(event == m_event) {
		m_event = NULL;
	}
};

};

