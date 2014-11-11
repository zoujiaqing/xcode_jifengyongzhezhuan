// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//

#include <iostream>
#include "NotificationCenter.h"
#include "ObserverProtocol.h"
#include "CCLuaEngine.h"

namespace TXGUI {
    USING_NS_CC;
    
    NotificationCenter::NotificationCenter()
    {
        CCLOG("NotificationCenter::NotificationCenter()");
		CCScheduler * pSchedule = CCDirector::sharedDirector()->getScheduler();
        pSchedule->scheduleSelector(schedule_selector(NotificationCenter::update), 
                                                         this, 
                                                         0,
                                                         false);
    }
    
    NotificationCenter::~NotificationCenter()
    {
        CCLOG("NotificationCenter::~NotificationCenter()");
		CCScheduler * pSchedule = CCDirector::sharedDirector()->getScheduler();
		pSchedule->unscheduleSelector(schedule_selector(NotificationCenter::update), 
			this);
    }
    
    static NotificationCenter* defaultNotificationCenter = NULL;
    
    NotificationCenter*NotificationCenter::defaultCenter()
    {
        if(NULL == defaultNotificationCenter)
		{
			defaultNotificationCenter = new NotificationCenter();
		}
		
		return defaultNotificationCenter;
    }

	void NotificationCenter::Destroy()
	{
		if (defaultNotificationCenter)
		{
			delete defaultNotificationCenter;
			defaultNotificationCenter = 0;
		}		
	}
    
    void NotificationCenter::update(float deltaTime)
    {
        notifyObservers();
    }
    

    void NotificationCenter::registerMsgObserver(ObserverProtocol*observer, int messageId) {
        if (observer == NULL)
            return ;
        
        if (observersMap.end() == observersMap.find(messageId)) {
            ObserverSet observerSet;
            observersMap[messageId] = observerSet; 
        }
        
        if (observersMap[messageId].end() == observersMap[messageId].find(observer))
            observersMap[messageId].insert(observer);
    }
    
    void NotificationCenter::unregisterMsgObserver(ObserverProtocol*observer, int messageId) {
        if (observer == NULL ||
            observersMap.end() == observersMap.find(messageId)) 
            return ;
        
        observersMap[messageId].erase(observer);
    }
    
    void NotificationCenter::unregisterMsgObserver(ObserverProtocol *observer) {
        if (observer == NULL)
            return ;
        
        for (ObserversMap::iterator iter = observersMap.begin();
             iter != observersMap.end(); ++iter) {
            (*iter).second.erase(observer);
        }
    }
    
    void NotificationCenter::registerAllMsgObserver(ObserverProtocol* observer) {
        if (observer != NULL && 
            allMsgObserver.end() == allMsgObserver.find(observer)) {
                allMsgObserver.insert(observer);
            }
    }
    
    void NotificationCenter::unregisterAllMsgObserver(ObserverProtocol* observer) {
        if (observer != NULL) {
            allMsgObserver.erase(observer);
        }
    }

    int NotificationCenter::registerScriptObserver(int handler)
	{
		if(m_scriptHandler.find(handler) == m_scriptHandler.end())
		{
			m_scriptHandler.insert(handler);
		}
		return handler;
	}

	void NotificationCenter::unregisterScriptObserver(int handler)
	{
		if(m_scriptHandler.find(handler) != m_scriptHandler.end())
		{
			m_scriptHandler.erase(handler);
			CCLuaEngine::defaultEngine()->removeScriptHandler(handler);
		}
	}

	void NotificationCenter::unregisterAllScriptObservers()
	{
		for(ObserverHandlers::iterator it = m_scriptHandler.begin();
			it != m_scriptHandler.end();++it)
		{
			if(*it)
			{
				CCLuaEngine::defaultEngine()->removeScriptHandler(*it);
			}
		}
		m_scriptHandler.clear();
	}

    void NotificationCenter::broadcast(int msg, void*sender, 
                                       std::string param1, std::string param2)
    {
        BroadcastMessage kMsg;
        kMsg.message = msg;
        kMsg.sender = sender;
        kMsg.param1 = param1;
        kMsg.param2 = param2;
        
        messageBump.push_back(kMsg);
        //CCLOG("NotificationCenter::broadcast(%s)", szObserverMessage[msg]);
    }
    
    
    void NotificationCenter::notifyObservers() {
        if (messageBump.empty()) {
            return;
        }
        
        notifyNormalObservers();
        notifyBroadObservers();
        notifyLuaScriptHandlers();
        messageBump.clear();
    }
    
    void NotificationCenter::notifyNormalObservers() {
        if (messageBump.empty()) {
            return;
        }
        
        for (MessageData::iterator iter = messageBump.begin(); 
             iter != messageBump.end(); ++iter) {
            BroadcastMessage& curMsg = *iter;
            if (observersMap.end() != observersMap.find(curMsg.message)) {
                doNotifyObservers(observersMap[curMsg.message], curMsg);
            }
        }
    }
    
    void NotificationCenter::notifyBroadObservers() {
        if (messageBump.empty()) {
            return;
        }
        
        for (MessageData::iterator iter = messageBump.begin(); 
             iter != messageBump.end(); ++iter) {
            BroadcastMessage& curMsg = *iter;
            doNotifyObservers(allMsgObserver, curMsg);
        }
    }
    
	void NotificationCenter::notifyLuaScriptHandlers()
	{
		for (MessageData::iterator iter = messageBump.begin(); 
			iter != messageBump.end(); ++iter) {
				BroadcastMessage& curMsg = *iter;
				CCLOG("NotificationCenter.cpp line:199 msg:0x%x", curMsg.message);
				for(ObserverHandlers::iterator it = m_scriptHandler.begin();
					it != m_scriptHandler.end();/*++it*/)
				{
					CCLuaEngine* engine = CCLuaEngine::defaultEngine();
					engine->cleanStack();
					engine->pushInt(curMsg.message);
					ObserverHandlers::iterator tempIter = it;
					tempIter++;
					int ret = engine->executeFunctionByHandler(*it,1);
					if(ret != 0)
					{
						// ÒÑ±»É¾³ý
						if(tempIter == m_scriptHandler.end())
						{
							break;
						}
						else
						{
							it = tempIter;
						}
					}
					else
					{
						++it;
					}
				}
		}
	}

    void NotificationCenter::doNotifyObservers(ObserverSet& observer,
                                               BroadcastMessage& msg) {
        ObserverSet::iterator it = observer.begin();
        for (; it != observer.end(); it++) 
        {
            if(NULL != *it)
            {
                (*it)->onBroadcastMessage(&msg);
            }
        }
    }
    
    
}