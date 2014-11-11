#include "OnlineNetworkManager.h"
#include "NetStateChecker.h"
#include "cocos2d.h"


USING_NS_CC;

NetStateChecker::~NetStateChecker()
{

}

NetStateChecker::NetStateChecker()
	:counter(0)
	,timeout(false)
	,recStatus(false)
{
	mTimeInMs = GetTimeWithLua();
}


void NetStateChecker::Start()
{
	counter = 0 ;
	timeout = false ;
	recStatus = false ;
	SendRqsToServer();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&NetStateChecker::Update),this,0,false);
}

void NetStateChecker::Stop()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&NetStateChecker::Update), this);
}

void NetStateChecker::SendRqsToServer()
{
	//CCLOG("---Send Ping---");
	OnlineNetworkManager::sShareInstance()->sendReqNetStateMessage();
	mTimeInMs = GetTimeWithLua();	
	counter = -1;
	timeout = false ;
}

void NetStateChecker::RecieveRqsFromServer()
{	
	//CCLOG("Receive Pingbag Rsp time : %u", time(NULL));
	//CCLOG("---Recieve Ping---");
	long time = GetTimeWithLua();
	long mDeltaTime = abs(time - mTimeInMs);
	CCDirector::sharedDirector()->SetNetDelayTime(mDeltaTime);  
	counter = 0;
	timeout = false ;
	recStatus = true ;
}

void NetStateChecker::Update(float dt)
{
	if (counter >= 0)
	{
		if(!recStatus )
			counter += dt;
		else
			recStatus = false ;
		
		
		if (counter >= 10.0f)
		{
			//CCLOG("Send Pingbag system time : %u", time(NULL));
			SendRqsToServer();
		}

	}
	else
	{
		long time = GetTimeWithLua();
		long mDeltaTime = abs(time - mTimeInMs);
		if(mDeltaTime >= 30000 && !timeout)
		{
			mTimeInMs = GetTimeWithLua();	
			timeout = true ;
			OnlineNetworkManager::sShareInstance()->LostConnection2Server("ping time out");
			
			
			
		}
	}

}

long NetStateChecker::GetTimeWithLua()
{
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now,0);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}