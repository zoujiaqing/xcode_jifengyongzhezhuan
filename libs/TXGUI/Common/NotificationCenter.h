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

#pragma once
#include <set>
#include <queue>
#include "TXGUI.h"
#include "ObserverProtocol.h"
#include "cocos2d.h"
#include "string"

namespace TXGUI{
    USING_NS_CC;
    class ObserverProtocol;
    
    typedef std::set<ObserverProtocol*> ObserverSet;
    typedef std::queue<BroadcastMessage> MessageVector;
    
    typedef std::vector<BroadcastMessage> MessageData;
    typedef std::map<int, ObserverSet> ObserversMap;
    typedef std::set<int> ObserverHandlers;

	typedef std::map<int,std::string> ObserverHandlerAndDataMap;

    class NotificationCenter : public cocos2d::CCObject
    {
        NotificationCenter();
    public:
        virtual ~NotificationCenter();
        static NotificationCenter* defaultCenter();
		static void Destroy();

        virtual void update(float dt);
        
       
        void registerMsgObserver(ObserverProtocol*observer, int messageId);
        void unregisterMsgObserver(ObserverProtocol*observer, int messageId);
        void unregisterMsgObserver(ObserverProtocol*observer);
        
        void registerAllMsgObserver(ObserverProtocol* observer);
        void unregisterAllMsgObserver(ObserverProtocol* observer);
        
        void broadcast(int msg, void*sender, 
                       std::string param1 = "", std::string param2="");

        int registerScriptObserver(int handler);
		// 在notification中不能调unregister取消对消息的注册，可在通知回调函数中返回非零值表示取消对消息的注册
		void unregisterScriptObserver(int handler);
		void unregisterAllScriptObservers();

		//int registerScriptObserver(int handler,std::string param);

    protected:
        void notifyObservers();
        void notifyNormalObservers();
        void notifyBroadObservers();
		void notifyLuaScriptHandlers();
        void doNotifyObservers(ObserverSet& observer, BroadcastMessage& msg);
        
    private:        
        MessageData messageBump;
        ObserversMap observersMap;
        ObserverSet allMsgObserver;
		ObserverHandlers m_scriptHandler;
		ObserverHandlerAndDataMap m_scriptHandlerDatas;
    };
}
