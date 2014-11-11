//
//  IEvent.h
//  iSeer
//
//  Created by b a on 12-5-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_IEvent_h
#define iSeer_IEvent_h

#include <vector>
#include <map>

#include "EventID.h"

/// 
// class IEvent
// 
class IEvent
{
public:
    virtual unsigned int GetID() = 0;
    virtual void*GetData() = 0;     
};

///
// class IEventHandler
//
class IEventHandler
{
public:
    virtual void HandleEvent(EVENTID id, IEvent* pkEvent) = 0;
    
};


///
// class IEventCenter
// 
class IEventCenter
{
public:
    /**
     * @brief 注册事件处理者
     * @param uiEvent,   事件编号
     * @param pkHandler, 事件处理者
     * @return bool, true: 注册成功, false : 失败
     * @
     */
    virtual bool RegisterEvent(EVENTID uiEvent, IEventHandler* pkHandler) = 0;
    
    
    /**
     * @brief 反注册事件处理者
     * @param uiEvent,   事件编号
     * @param pkHandler, 事件处理者
     * @return bool, true: 注册成功, false : 失败
     * @
     */
    virtual void RemoveEvent(EVENTID uiEvent, IEventHandler* pkHandler) = 0;
    
    /**
     * @brief 注册Root事件处理者
     * @param pkHandler, 事件处理者
     * @return void, 
     * @note  会把所有的事件透传root Handlerer
     */
    virtual void SetRootEvent(IEventHandler* pkRootHandler) = 0; 
};



typedef std::vector<IEventHandler*> VecEventHandler;
typedef std::map<EVENTID, VecEventHandler>EventID2Handlers;



class CEventCenter : public IEventCenter
{
   
public: 
    CEventCenter();
    
    
    /**
     * @brief 注册事件处理者
     * @param uiEvent,   事件编号
     * @param pkHandler, 事件处理者
     * @return bool, true: 注册成功, false : 失败
     * @
     */
    virtual bool RegisterEvent(EVENTID uiEvent, IEventHandler* pkHandler);
    
    
    /**
     * @brief 反注册事件处理者
     * @param uiEvent,   事件编号
     * @param pkHandler, 事件处理者
     * @return bool, true: 注册成功, false : 失败
     * @
     */
    virtual void RemoveEvent(EVENTID uiEvent, IEventHandler* pkHandler); 
    
    
    /**
     * @brief 注册Root事件处理者
     * @param pkHandler, 事件处理者
     * @return void, 
     * @note  会把所有的事件透传root Handlerer
     */
    virtual void SetRootEvent(IEventHandler* pkRootHandler); 
    
    
    /**
     * @brief 清除注册事件处理者
     * @return void,
     * @
     */ 
    virtual void ClearAllHandlers();
    
protected:
    virtual void DispatchEvent(EVENTID uiEvent, IEvent& pEvent);
    
    
protected:
    EventID2Handlers  m_mapGameEventID2Handlers;  
    
    IEventHandler* m_pRootHandler;
};



class PkgEvent : public IEvent
{
public :
    PkgEvent(unsigned int ID, void* data)
    {
        m_pPkgData = data;    
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
    virtual void* GetData() {return m_pPkgData;};
    
protected:
    void* m_pPkgData;
    unsigned int m_uiCMD;
};

#endif
