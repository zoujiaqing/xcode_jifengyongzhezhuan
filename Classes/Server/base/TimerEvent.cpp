#include "TimerEvent.hpp"

namespace base {

TimerEvent::TimerEvent(EventableObject* ev_owner,CallbackBase* cb, uint32_t interval, int repeat):
	Event(ev_owner, cb), m_interval(interval), m_repeat_times(repeat)
{

}
	
TimerManager::~TimerManager() {
    EventMap::iterator it = m_allevents.begin();
    while (it != m_allevents.end()) {
        delete it->second;    
    	++it;
	}
	m_allevents.clear();
}

void TimerManager::process_events()
{
    Time tv;
    EventMap::iterator it = m_allevents.begin();
    while (it != m_allevents.end()) {
        if (it->first <= tv) {
            TimerEvent* ev = it->second;
            m_allevents.erase(it);

            if (ev->is_active()) {
                if ((ev->m_cb->execute() == 0) && ((ev->m_repeat_times == -1) || (ev->m_repeat_times > 0))) {
                    ev->m_expired_tv.now_ms_timeadd(ev->m_interval);
                    // add this event to the global event list, updating will all be done later on...
                    m_allevents.insert(std::make_pair(ev->m_expired_tv, ev));
                    if (ev->m_repeat_times > 0) {
                        --(ev->m_repeat_times);
                    }

                    it = m_allevents.begin();
                    continue;
                }
            }

            if (ev->is_active()) {
                remove_event(ev);
            }
            delete ev;
            it = m_allevents.begin();
        } else {
            break;
        }
    }
}

}
