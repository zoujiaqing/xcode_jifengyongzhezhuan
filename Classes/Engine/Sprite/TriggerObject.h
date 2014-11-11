//
//  TriggerObject.h
//  iSeer
//
//  Created by b a on 12-5-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_TriggerObject_h
#define iSeer_TriggerObject_h

#include "Event.h"

class TriggerEvent;
class TriggerObject;

/// 
// class Trigger
//
class TriggerEvent : public IEvent
{
public:
    enum TriggerPhase
    {
        Phase_None  = -1,
        
        // touched by user
        Phase_Touched = 0,
        
        // begin to function
        Phase_Begin = 1,
        
        // finished
        Phase_End   = 2,
        
        // new loop begin!
        Phase_Loop  = 3,
    };
    
    // Trigger Data, store trigger property data
    class TriggerData
    {       
        public :
        // data   
        TriggerPhase typeOfPhase; 
        unsigned int id ;
        TriggerObject* trigger;
    };
    
    
public:
    
    TriggerEvent(unsigned int ID, void* data)
    {
        m_pPkgData = (TriggerData*)data;    
        m_uiCMD = ID;
    }
    
    
    TriggerEvent(unsigned int ID, TriggerEvent::TriggerPhase phase, TriggerObject* trigger)
    {
        m_pPkgData = &m_objDefault; 
        
        m_objDefault.typeOfPhase = phase;
        m_objDefault.id = ID;
        m_objDefault.trigger = trigger;
        
        m_uiCMD = ID;
    }
    
    /** 
     * @brief: 获取事件编号
     * @param: void
     * @return:unsigned int
     */     
    virtual unsigned int GetID(){return m_uiCMD;};
    
    /** 
     * @brief: 获取事件编号
     * @param: void
     * @return:void* , 数据对象
     */ 
    virtual void* GetData() {return (void*)(m_pPkgData);};
    
    TriggerData& GetTriggerData() {return *m_pPkgData;}
    
protected:
    TriggerData* m_pPkgData;
    unsigned int m_uiCMD;
    
private:    
    TriggerData  m_objDefault;
};


///
// class TriggerObject
//
class TriggerObject : protected CEventCenter
{
public :
    // construtor 
    TriggerObject();
    TriggerObject(unsigned int id, IEventHandler* pHandler, unsigned int uiEventID);
    ~TriggerObject();  

    
    void EnableTrigger(bool bEnable);
    
    
protected:
    
    void   Init(unsigned int id, IEventHandler* pHandler, unsigned int uiEventID);
    
    virtual void DispatchEvent(EVENTID uiEvent, IEvent& pEvent);
    
protected:
    bool m_bEnable; 
    IEventHandler* m_pHandler;

    unsigned int m_Triggerid;
    unsigned int m_uiEventID;
};


#endif
