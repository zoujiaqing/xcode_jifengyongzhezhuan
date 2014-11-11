#include "OnlineNetworkManager.h"
#include "NickNameManager.h"

NickNameManager::NickNameManager()
{
	m_callTarget = NULL;
	m_callHandler = NULL;
	b_isNeedNickFlag = false;
}

NickNameManager::~NickNameManager()
{

}

const char* NickNameManager::getNickName(bool isMale)
{
	if(isMale)
	{
		if(m_maleNickVec.size() > 0)
		{
			std::vector<std::string>::iterator it = m_maleNickVec.begin();
			if(it != m_maleNickVec.end())
			{
				CCString* nick = CCString::create(*it);
				m_maleNickVec.erase(it);
				return nick->m_sString.c_str();
			}
		}
		else
		{
			OnlineNetworkManager::sShareInstance()->sendGetRandomName(0);
			b_isNeedNickFlag = true;
		}
		return NULL;
	}
	else
	{
		if(m_femaleNickVec.size() > 0)
		{
			std::vector<std::string>::iterator it = m_femaleNickVec.begin();
			if(it != m_femaleNickVec.end())
			{
				CCString* nick = CCString::create(*it);
				m_femaleNickVec.erase(it);
				return nick->m_sString.c_str();
			}
		}
		else
		{
			OnlineNetworkManager::sShareInstance()->sendGetRandomName(1);
			b_isNeedNickFlag = true;
		}
		return NULL;
	}
	
}

void NickNameManager::setNickNameCallback(CCNode* target,SEL_CallFuncND func)
{
	m_callHandler = func;
	m_callTarget = target;
}

void NickNameManager::onReceiveNewNickName(const char* nick,unsigned int sexual)
{
	if(b_isNeedNickFlag)
	{
		if(m_callTarget != NULL && m_callHandler != NULL)
		{
			(m_callTarget->*m_callHandler)(m_callTarget,(void*)nick);
		}
		b_isNeedNickFlag = false;
	}
	else
	{
		if(sexual == 0)
		{
			m_maleNickVec.push_back(nick);
		}
		else
		{
			m_femaleNickVec.push_back(nick);
		}
	}
	
}

void NickNameManager::unRegisterNickNameCallback()
{
	m_callHandler = NULL;
	m_callTarget = NULL;
}