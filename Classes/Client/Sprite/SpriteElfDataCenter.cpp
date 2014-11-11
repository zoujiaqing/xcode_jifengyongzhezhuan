#include "OnlineNetworkManager.h"
#include "SpriteElfDataCenter.h"
#include "SpriteElfManager.h"
#include "UserData.h"
#include "SpriteElfConfigFromLua.h"
#include "SpriteFactory.h"

//ElfAttachData::ElfAttachData()
//{
//	nElfId = 0;
//	nState = -1;
//}
//
//void ElfAttachData::SetElfId(int elfId)
//{
//	this->nElfId = elfId;
//}
//
//void ElfAttachData::SetStateId(int stateId)
//{
//	this->nState = stateId;
//}

static SpriteElfDataCenter* _elfDataManager = 0;

SpriteElfDataCenter::~SpriteElfDataCenter()
{
	//m_nEquipElfId = 0;
	//m_vecElfData.clear();
}

SpriteElfDataCenter::SpriteElfDataCenter()
{
	m_bHeroHasSendMessage = false;
}

SpriteElfDataCenter* SpriteElfDataCenter::Get()
{
	if(_elfDataManager == 0)
	{
		_elfDataManager = new SpriteElfDataCenter();
	}
	return _elfDataManager;
}

void SpriteElfDataCenter::Destroy()
{
	if (_elfDataManager)
	{
		delete _elfDataManager;
		_elfDataManager = NULL;
	}
}

//void SpriteElfDataCenter::AddOneAttachData(ElfAttachData data)
//{
//	unsigned int elfId = data.nElfId;
//
//	for (std::vector<ElfAttachData>::iterator iter = m_vecElfData.begin();
//		iter != m_vecElfData.end();iter++)
//	{
//		if (elfId == (*iter).nElfId)
//		{
//			return ;
//		}
//	}
//	m_vecElfData.push_back(data);
//}

void SpriteElfDataCenter::EquipOneElf(unsigned int elfId)
{
	SpriteElfManager::Get()->AttachElfToOneHero(elfId,UserData::Get()->getUserId(), 0);
}

//void SpriteElfDataCenter::SetOneElfState(unsigned int elfId,int state)
//{
//	for (std::vector<ElfAttachData>::iterator iter = m_vecElfData.begin();
//	iter != m_vecElfData.end();iter++)
//	{
//		if (elfId == (*iter).nElfId)
//		{
//			(*iter).nState = state;
//			return ;
//		}
//	}
//}
//
//unsigned int SpriteElfDataCenter::GetOneElfState(unsigned int elfId)
//{
//	for (std::vector<ElfAttachData>::iterator iter = m_vecElfData.begin();
//		iter != m_vecElfData.end();iter++)
//	{
//		if (elfId == (*iter).nElfId)
//		{
//			return (*iter).nState;
//		}		
//	}
//	// Note: 0 ±íÊ¾Lock×´Ì¬
//	return 0;
//}

void SpriteElfDataCenter::SendGetElfListMessage(PLAYER_ID roleId,const char* name)
{
	if (roleId == UserData::Get()->getUserId())
	{
		if (false == m_bHeroHasSendMessage)
		{
			OnlineNetworkManager::sShareInstance()->sendReqOneHeroElfMessage(roleId,name);
			m_bHeroHasSendMessage = true;
		}
		else
		{
			unsigned int elfId = SpriteElfConfigFromLuaManager::getInstance()->GetHeroFightElfID();
			SpriteFactory::sharedFactory()->AttachElfToOneHero(elfId, roleId);
		}
	}
	else
	{
		OnlineNetworkManager::sShareInstance()->sendReqOneHeroElfMessage(roleId,name);
	}
}

void SpriteElfDataCenter::SendSetElfStateMessage(unsigned int elfId,int state)
{
	OnlineNetworkManager::sShareInstance()->sendReqSetElfStateMessage(elfId,state);
}

PLAYER_ID SpriteElfDataCenter::GetHeroUserId()
{
	return UserData::Get()->getUserId();
}

void SpriteElfDataCenter::ResetValue()
{
	m_bHeroHasSendMessage = false;
}