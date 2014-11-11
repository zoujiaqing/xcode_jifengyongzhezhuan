#include "WaitingLayerManager.h"
#include "WaitingLayer.h"

extern void ShowWaitingLayer(bool bShowRightNow  = true ,float sumTimer  = 0.3 );
extern void HideWaitingLayer();

void WaitingLayerManager::PushOneMessage(const char* pMessage)
{
	if (strcmp(pMessage,"") == 0)
	{
		WaitingLayerQueneCount ++;
		return ;
	}

	std::string strMessage(pMessage);
	int length = strMessage.length();
	if (length <= 3)
	{
		CCError("PushOneMessage not valid param -> %s ",pMessage);
		return ;
	}
	strMessage = strMessage.substr(0,length-3);

	std::map<std::string,WAITING_LAYER_MESSAGE>::iterator iter = m_messageAndValueMap.find(strMessage.c_str());
	if (iter == m_messageAndValueMap.end())
	{
		WAITING_LAYER_MESSAGE messageData;
		messageData.t_message = strMessage;
		messageData.t_count = 1;
		m_messageAndValueMap.insert(std::make_pair(strMessage,messageData));

		WaitingLayerQueneCount ++;
	}
	else
	{
		(*iter).second.t_count ++;
	}
}

void WaitingLayerManager::CheckPopOneMessage(const char* pMessage)
{
	std::string strMessage(pMessage);
	int length = strMessage.length();
	if (length <= 3)
	{
		CCError("CheckPopOneMessage not valid param -> %s ",pMessage);
		return ;
	}

	if(strcmp(strMessage.c_str(),"ErrorCode") == 0)
	{
		m_messageAndValueMap.clear();
		WaitingLayerQueneCount = 0;
		return;
	}

	strMessage = strMessage.substr(0,length-3);

	std::map<std::string,WAITING_LAYER_MESSAGE>::iterator iter = m_messageAndValueMap.find(strMessage.c_str());
	if (iter != m_messageAndValueMap.end())
	{
		(*iter).second.t_count --;
		if ((*iter).second.t_count == 0)
		{
			m_messageAndValueMap.erase(iter);
			WaitingLayerQueneCount --;
		}
	}
}

void WaitingLayerManager::RemoveOneByForce()
{
	if (WaitingLayerQueneCount > 0)
	{
		WaitingLayerQueneCount --;
	}
}

void WaitingLayerManager::clearMessages()
{
    m_messageAndValueMap.clear();
    WaitingLayerQueneCount = 0;
}

void WaitingLayerManager::update(float dt)
{
	size_t size = m_messageAndValueMap.size();
	bool needShow = false;
	if (size != 0)
	{
		for (std::map<std::string,WAITING_LAYER_MESSAGE>::iterator iter = m_messageAndValueMap.begin();
			iter != m_messageAndValueMap.end(); )
		{
			(*iter).second.t_runningTime += dt;

			if ((*iter).second.t_runningTime > 1)
			{
				needShow = true;
			}
			if ((*iter).second.t_runningTime > TotalWaitingTimer)
			{
				m_messageAndValueMap.erase(iter++);
				WaitingLayerQueneCount --;
			}
			else
			{
				iter++;
			}
		}
	}

	if (WaitingLayerQueneCount > 0)
	{
		if (needShow)
		{
			ShowWaitingLayer(true);
		}
		else
		{
			ShowWaitingLayer(false);
		}
		
	}
	else
	{
		HideWaitingLayer();
	}
}