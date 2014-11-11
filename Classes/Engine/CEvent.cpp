//
//  Event.cpp
//  iSeer
//
//  Created by b a on 12-5-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Event.h"


CEventCenter::CEventCenter()
{
    m_pRootHandler = NULL;
}

/**
 * @brief 注册事件处理者
 * @param uiEvent,   事件编号
 * @param pkHandler, 事件处理者
 * @return bool, true: 注册成功, false : 失败
 * @
 */
bool CEventCenter::RegisterEvent(EVENTID uiEvent, IEventHandler* pkHandler)
{
   	// 一个事件ID对应的hander列表中，相同的指针要过滤掉
	EventID2Handlers::iterator mitEvent2Handlers = 
    m_mapGameEventID2Handlers.find(uiEvent);
	if(mitEvent2Handlers != m_mapGameEventID2Handlers.end())
	{
		VecEventHandler::iterator vitHandler = 
        mitEvent2Handlers->second.begin();
		while(vitHandler != mitEvent2Handlers->second.end())
		{
			if(*vitHandler == pkHandler)
			{
                return false;
				break;
			}
			++vitHandler;
		}
        
		if(vitHandler == mitEvent2Handlers->second.end())
		{
			mitEvent2Handlers->second.push_back(pkHandler);
		}
	}
	else
	{
		VecEventHandler vecEventHandler;
		vecEventHandler.push_back(pkHandler);
		m_mapGameEventID2Handlers[uiEvent] = vecEventHandler;
	}
    
    return true;
}

/**
 * @brief 注册Root事件处理者
 * @param pkHandler, 事件处理者
 * @return void, 
 * @note  会把所有的事件透传root Handlerer
 */
void CEventCenter::SetRootEvent(IEventHandler* pkRootHandler)
{
    m_pRootHandler = pkRootHandler;
}


/**
 * @brief 反注册事件处理者
 * @param uiEvent,   事件编号
 * @param pkHandler, 事件处理者
 * @return bool, true: 注册成功, false : 失败
 * @
 */
void CEventCenter::RemoveEvent(EVENTID uiEvent, IEventHandler* pkHandler)
{
    EventID2Handlers::iterator mitEvent2Handlers = 
    m_mapGameEventID2Handlers.find(uiEvent);
	if(mitEvent2Handlers != m_mapGameEventID2Handlers.end())
	{
		VecEventHandler::iterator vitHandler = 
        mitEvent2Handlers->second.begin();
		while(vitHandler != mitEvent2Handlers->second.end())
		{
			if(*vitHandler == pkHandler)
			{
				mitEvent2Handlers->second.erase(vitHandler);
				break;
			}
			++vitHandler;
		}
	}
}


void CEventCenter::DispatchEvent(EVENTID uiEvent, IEvent& pEvent)
{
    if (m_pRootHandler != NULL)
        m_pRootHandler->HandleEvent(uiEvent, &pEvent);
    
    EventID2Handlers::iterator mitEvent2Handlers = m_mapGameEventID2Handlers.find(uiEvent); 
    if(mitEvent2Handlers != m_mapGameEventID2Handlers.end())
	{
		VecEventHandler::iterator vitHandler = 
        mitEvent2Handlers->second.begin();
		while(vitHandler != mitEvent2Handlers->second.end())
		{
			IEventHandler* pkHandler = (*vitHandler);
            pkHandler->HandleEvent(uiEvent, &pEvent);
			++vitHandler;
		}
	}
}

void CEventCenter::ClearAllHandlers()
{
    m_mapGameEventID2Handlers.clear();
}
