//
//  TriggerObject.cpp
//  iSeer
//
//  Created by b a on 12-5-21.
//  Copyright (c) 2012Äê __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "TriggerObject.h"

TriggerObject::TriggerObject() : m_bEnable(true)
{
    m_Triggerid = -1;
    m_pHandler = NULL;

}


TriggerObject::TriggerObject(unsigned int id, IEventHandler* pHandler, unsigned int uiEventID) 
: m_bEnable(true)
{
    m_Triggerid = id;
    m_pHandler = pHandler;
    
    Init(id, m_pHandler, uiEventID);
}

TriggerObject::~TriggerObject()
{
    if (m_pHandler)
    {
        RemoveEvent(m_uiEventID, m_pHandler);   
    }
    
    m_pHandler = NULL;
    m_Triggerid = -1;
}

void TriggerObject::Init(unsigned int id, IEventHandler* pHandler, unsigned int uiEventID) 
{
    m_Triggerid = id;
    m_pHandler = pHandler;
    m_uiEventID = uiEventID;
    
    if (m_pHandler)
    {
        RegisterEvent(uiEventID, m_pHandler);   
    }
}

void TriggerObject::EnableTrigger(bool bEnable)
{
    m_bEnable = bEnable;
}

void TriggerObject::DispatchEvent(EVENTID uiEvent, IEvent& pEvent)
{
    if (m_bEnable)
    {
        CEventCenter::DispatchEvent(uiEvent, pEvent);
    }
}