#ifndef _WAITING_LAYER_MANAGER_H_
#define _WAITING_LAYER_MANAGER_H_

#include "Singleton_t.h"
#include "string"
#include "map"

class WaitingLayerManager : public TSingleton<WaitingLayerManager>
{
public:
	typedef struct tWaitingLayerMessage
	{
		std::string t_message;
		float t_runningTime;
		unsigned int t_count;
		tWaitingLayerMessage()
		{
			t_message = "";
			t_runningTime = 0;			
			t_count = 0;
		}

	}WAITING_LAYER_MESSAGE;
public:
	WaitingLayerManager(){
		WaitingLayerQueneCount = 0;
		TotalWaitingTimer = 5.0;
	}
	void PushOneMessage(const char* pMessage);
	void CheckPopOneMessage(const char* pMessage);
	void RemoveOneByForce();
    void clearMessages();

	void update(float dt);
private:
	int	WaitingLayerQueneCount ;
	float TotalWaitingTimer;
	std::map<std::string,WAITING_LAYER_MESSAGE> m_messageAndValueMap;
};

#endif	//_WAITING_LAYER_MANAGER_H_