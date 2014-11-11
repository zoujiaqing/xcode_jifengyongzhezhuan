#include "DeferEvent.hpp"

namespace base {

DeferEvent::DeferEvent(EventableObject* ev_owner,CallbackBase* cb):
	Event(ev_owner, cb)
{

}
	
DeferManager::~DeferManager() {
	process_events();
}
	
void DeferManager::clear() {
    EventList::iterator it = m_allevents.begin();
    while (it != m_allevents.end()) {
		it = m_allevents.erase(it);
	}
}

bool DeferManager::empty() {
	EventList::iterator it = m_allevents.begin();
    while (it != m_allevents.end()) {
		if((*it)->is_active()) {
			return true;
		}
		++it;
	}
	return false;
}

void DeferManager::process_events() {
    EventList::iterator it = m_allevents.begin();
    while (it != m_allevents.end()) {
		DeferEvent* ev = *it;
		m_allevents.erase(it);
		if (ev->is_active()) {
			ev->m_cb->execute();
        }
        if (ev->is_active()) {
			ev->deactivate();
        }
        delete ev;
        it = m_allevents.begin();
	}
}

}

