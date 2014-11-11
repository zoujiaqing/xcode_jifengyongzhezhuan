#pragma once
#include "Singleton_t.h"

class NetStateChecker :public TSingleton<NetStateChecker>, public CCObject
{
public:	
	NetStateChecker();
	~NetStateChecker();

	/// start say hello
	void Start();

	/// stop say hello
	void Stop();    
	 
	bool GetTimeout(){return timeout;}
	void RecieveRqsFromServer();

private:
	void SendRqsToServer();  

	void Update(float dt);

	long GetTimeWithLua();

	long int mTimeInMs;
	bool  timeout ;
	bool  recStatus ;
	float counter;
};