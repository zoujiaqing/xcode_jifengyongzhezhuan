#pragma once

#include "Singleton_t.h"
#include "cocos2d.h"
#include "TimeProtocol.h"

USING_NS_CC;

typedef set<TimeProtocol*> TimeObSet;
typedef map<int, TimeObSet> TimeObMap;

typedef set<int> LuaObSet;
typedef map<int, LuaObSet> LuaObMap;

typedef map<int, long> CounterMap;

class TimeManager: public TSingleton<TimeManager>,public CCObject
{
public:
	TimeManager();
	~TimeManager();
	
	//同步及获取服务器时间
	void syncServerTime(unsigned int serTime);
	long getCurServerTime();
	bool isInited(){return this->initFlag;}

	//辅助函数
	string secondsToString(long seconds);

	//C++计时器相关
	bool registerTimer(TimeProtocol * observer, int counterId, long endTime);
	bool unregisterTimer(TimeProtocol * observer, int counterId);
	bool attachTimer(TimeProtocol * observer, int counterId);

	//Lua计时器相关
	int registerLuaTimer(int handler, int counterId, long endTime);
	bool unregisterLuaTimer(int handler, int counterId);
	int attachLuaTimer(int handler, int counterId);
	void setTimeZone(int val){timeZone =val ;}
	int  getTimeZone(){return timeZone;}
	//计时器启动、暂停、更新
	bool startTimer(int counterId, long endTime);
	bool stopTimer(int counterId);	
	bool renewTimer(int counterId, long endTime);

	//检测计时器
	bool hasTimer(int counterId);
	bool hasObserver(TimeProtocol * observer, int counterId);
	bool hasLuaObserver(int handler, int counterId);

	virtual void update(float dt);

	void ResetData();
    
    //CC_SYNTHESIZE(long, m_shopFreshTime, shopFreshTime)
private:
	void updateObservers(long delta);
	void updateLuaObservers(long delta);
	#ifdef OUR_DEBUG
	void updateMem(float dt);
    #endif
	//用来同步系统时间
	long serverMinusLocalTime;

	//最新的本地时间，用来检测更新通知
	long latestLocalTime;

	//是否同步过系统时间
	bool initFlag;
	int  timeZone ;

	TimeObMap timeObMap;	
	LuaObMap luaObMap;

	CounterMap counterMap;

	//加速检测
	long lastSyncServerTime;
	int acceCount;
};
