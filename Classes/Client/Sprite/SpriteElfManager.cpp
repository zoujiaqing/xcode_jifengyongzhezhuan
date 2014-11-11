#include "SpriteElfManager.h"
#include "cocos2d.h"
#include "SpriteElf.h"
#include "LevelManager.h"
#include "SkeletonAnimRcsManager.h"
#include "LevelLayer.h"
#include "SpriteElfConfigFromLua.h"
#include "LevelManager.h"
#include "LuaTinkerManager.h"
#include "AssistElfFightPanel.h"
#include "GameManager.h"
#include "MainLandManager.h"
#include "BattleUILayer.h"
#include "AssistElfFightPanel.h"
#include "MainMenuLayer.h"
USING_NS_CC;

cocos2d::CCPoint SpriteElfManager::s_fixDistanceToHero = CCPointZero;

SpriteElfManager::SpriteElfManager()
{
	// Note: Get s_fixDistanceToHero
	SpriteElfConfigFromLuaManager::getInstance()->GetElfToHeroFixedDistance(s_fixDistanceToHero.x,s_fixDistanceToHero.y);
	 m_mapElfIdAndInstances.clear();
     m_mapElfIdAndPos.clear();

	 m_vecAssistSkillEventObserver.clear();	

	 m_mapAssistElfIdAndInstances.clear();

}

SpriteElfManager::~SpriteElfManager()
{
	m_mapElfIdAndInstances.clear();
}

void SpriteElfManager::ClearAllData()
{
	m_mapElfIdAndInstances.clear();
}

SpriteElf* SpriteElfManager::CreateInstance(int type, int grade)
{
	// Note: 检测动画资源是否已经加载
	SkeletonAnimRcsManager::getInstance()->LoadOneSpriteElfRcs(type,true);

	SpriteElf * instance = NULL;

	instance = new SpriteElf();
	if (instance && instance->initWithId(type, grade))
	{
		instance->autorelease();
		return  instance;
	}
	CC_SAFE_DELETE(instance);
	return NULL;
}

void SpriteElfManager::RemoveAllElfInstance()
{
	//m_mapElfIdAndPos.clear();
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapElfIdAndInstances.begin();
	if (iter != m_mapElfIdAndInstances.end())
	{
		for (std::map<unsigned int,SpriteElf*>::iterator _iter = (*iter).second.begin();
			_iter != (*iter).second.end(); _iter++)
		{
			SpriteElf* pElf = (*_iter).second;
			pElf->removeFromParentAndCleanup(true);
		}
		
		iter++;
	}
	m_mapElfIdAndInstances.clear();

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter1 = m_mapAssistElfIdAndInstances.begin();
	if (iter1 != m_mapAssistElfIdAndInstances.end())
	{
		for (std::map<unsigned int,SpriteElf*>::iterator _iter = (*iter1).second.begin();
			_iter != (*iter1).second.end(); _iter++)
		{
			SpriteElf* pElf = (*_iter).second;
			pElf->removeFromParentAndCleanup(true);
		}

		iter1++;
	}
	m_mapAssistElfIdAndInstances.clear();
}

SpriteElf* SpriteElfManager::CreateAssistInstance(PLAYER_ID s_id,int type)
{
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapAssistElfIdAndInstances.find(s_id);
	if (iter != m_mapAssistElfIdAndInstances.end())
	{
		std::map<unsigned int,SpriteElf*>::iterator _iter = (*iter).second.find(type);
		if (_iter != (*iter).second.end())
		{
			SpriteElf* pElf = (*_iter).second;
			if (pElf)
			{
				pElf->removeFromParentAndCleanup(true);
				(*iter).second.erase(_iter);
			}			
		}		
	}

	SpriteElf * instance = CreateInstance(type);
	if (instance)
	{
		if (AddAssistElf(s_id,instance,type) == false)
		{
			CC_SAFE_DELETE(instance);
			instance = NULL;
		}
		else
		{
			instance->SetUpdateOrNot(false);
		}
	}
	return instance;
}

SpriteElf* SpriteElfManager::CreateInstance(PLAYER_ID s_id,int type, int grade)
{
	EFairyPos pos  ;
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapElfIdAndInstances.find(s_id);
	if (iter != m_mapElfIdAndInstances.end())
	{
		std::map<unsigned int,SpriteElf*>::iterator _iter = (*iter).second.find(type);
		if (_iter != (*iter).second.end())
		{
			SpriteElf* pElf = (*_iter).second;
			
			if (pElf)
			{
				pElf->removeFromParentAndCleanup(true);
				(*iter).second.erase(_iter);
			}	

		}		
	}

	std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >::iterator iter_pos = m_mapElfIdAndPos.find(s_id);
	if(iter_pos !=  m_mapElfIdAndPos.end())
	{
		std::map<unsigned int ,EFairyPos>::iterator _iter_pos2 = (*iter_pos).second.find(type) ;

		pos = (*_iter_pos2).second ;
	}
	else
		return NULL ;
	SpriteElf * instance = CreateInstance(type, grade);
	if (instance)
	{
        instance->getGradeLayer()->setVisible(true);

		if (Add(s_id,instance,type) == false)
		{
			//CCLOGERROR("create SpriteElf id:%d failed, it's already existed" , s_id);
			CC_SAFE_DELETE(instance);
			instance = NULL;
		}
		else
		{
			std::map<unsigned int,SpriteElf*> elfs;
			if (GetInstances(s_id,elfs))
			{
				if (pos == E_Fight_Left)
				{
					instance->SetStandLeft(true);
					SetPlayerElfPos(s_id,type,E_Fight_Left);
					if(s_id == UserData::Get()->getUserId())
					{
						unsigned int attackVal =LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerAttackFairyByPos",1);
						instance->setSkAttack(attackVal);
					}
				}
				else if(pos == E_Fight_Right)
				{
					instance->SetStandLeft(false);
					SetPlayerElfPos(s_id,type,E_Fight_Right);
					if(s_id == UserData::Get()->getUserId())
					{
						unsigned int attackVal =LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerAttackFairyByPos",2);
						instance->setSkAttack(attackVal);
					}
				}
				
			}
		}
	}
	return instance;
}

SpriteElf* SpriteElfManager::GetOneInstance(PLAYER_ID s_id,unsigned int elfTypeId)
{
	SpriteElf * instance = null;
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
	if (it != m_mapElfIdAndInstances.end())
	{
		std::map<unsigned int,SpriteElf*>& elfs = (*it).second;
		std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.find(elfTypeId);
		if (elf_iter != elfs.end())
		{
			instance = elf_iter->second;
		}
	}
	return instance;
}

bool SpriteElfManager::GetInstances(PLAYER_ID s_id,std::map<unsigned int,SpriteElf*>& elfs)
{
	bool bRet = false;

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
	if (it != m_mapElfIdAndInstances.end())
	{		
		bRet = true;
		elfs = (*it).second;
	}

	return bRet;
}

bool SpriteElfManager::IsHasElf(PLAYER_ID s_id,unsigned int elfTypeId)
{
	bool bRet = false;

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
	if (it != m_mapElfIdAndInstances.end())
	{		
		std::map<unsigned int,SpriteElf*>& elfs = (*it).second;
		if (elfs.find(elfTypeId) != elfs.end())
		{
			bRet = true;
		}		
	}

	return bRet;
}
bool SpriteElfManager::IsHasElfPlay(PLAYER_ID s_id)
{
	bool bRet = false;

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
	if (it != m_mapElfIdAndInstances.end())
	{		
		
		bRet = true;
				
	}

	return bRet;
}
bool SpriteElfManager::IsHasElfAtPos(PLAYER_ID s_id,unsigned int pos)
{
	bool bRet = false;

	std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >::iterator iter = m_mapElfIdAndPos.find(s_id);
	if (iter != m_mapElfIdAndPos.end())
	{
		const std::map<unsigned int,EFairyPos>& elfInfos = (*iter).second;
		for (std::map<unsigned int,EFairyPos>::const_iterator c_iter = elfInfos.begin();
			c_iter != elfInfos.end();c_iter++)
		{
			if (pos == (*c_iter).second)
			{
				bRet = true;
				break;
			}
		}		
	}

	return bRet;
}

bool SpriteElfManager::AddAssistElf(PLAYER_ID id,SpriteElf * instance,unsigned int elfTypeId)
{
	bool bRet = false;

	if (instance != null)
	{
		std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapAssistElfIdAndInstances.find(id);
		if (iter == m_mapAssistElfIdAndInstances.end())
		{
			std::map<unsigned int,SpriteElf*> elfs;
			elfs.insert(std::make_pair(elfTypeId,instance));
			m_mapAssistElfIdAndInstances.insert(std::make_pair(id,elfs));
			bRet = true;
		}
		else
		{
			std::map<unsigned int,SpriteElf*> &elfs = (*iter).second;
			elfs.insert(std::make_pair(elfTypeId,instance));
			bRet = true;
		}
	}

	return bRet;
}

bool SpriteElfManager::Add(PLAYER_ID id,SpriteElf * instance,unsigned int elfTypeId)
{    
	bool bRet = false;

	if (instance != null)
	{
		std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapElfIdAndInstances.find(id);
		if (iter == m_mapElfIdAndInstances.end())
		{
			std::map<unsigned int,SpriteElf*> elfs;
			elfs.insert(std::make_pair(elfTypeId,instance));
			m_mapElfIdAndInstances.insert(std::make_pair(id,elfs));
			//instance->SetIsOnlyMe(true);
			bRet = true;
		}
		else
		{
			std::map<unsigned int,SpriteElf*> &elfs = (*iter).second;
			elfs.insert(std::make_pair(elfTypeId,instance));
			//if (elfs.size() == 1)
			//{
			//	instance->SetIsOnlyMe(true);
			//}
			//else
			//{
			//	for (std::map<unsigned int,SpriteElf*>::iterator iter = elfs.begin();
			//		 iter != elfs.end(); iter++)
			//	{
			//		(*iter).second->SetIsOnlyMe(false);
			//		
			//	}
			//}
			bRet = true;
		}
	}

	return bRet;
}
 bool SpriteElfManager::RemovePosElfFromHero(PLAYER_ID s_id)
 {
	 std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> > ::iterator it1 = m_mapElfIdAndPos.find(s_id) ;

	 if (it1 != m_mapElfIdAndPos.end())
	 {	
		m_mapElfIdAndPos.erase(it1);

	 }
	 return true ;
 }
 SpriteElf* SpriteElfManager::GetOneInstanceFromPos(PLAYER_ID s_id,unsigned int pos)
 {
	   SpriteElf* pSpriteElf = NULL;
	   if(pos >0 && pos <20)
	   {
		   std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
		   if (it != m_mapElfIdAndInstances.end())
		   {
			   std::map<unsigned int,SpriteElf*>& elfs = (*it).second;
			   for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();elf_iter != elfs.end();elf_iter++)
			   {
				   SpriteElf* instance = (*elf_iter).second;
				   if(pos == 1&& instance->IsStandLeft())
				   {
					   pSpriteElf = instance ;
					   break;
				   }
				   else if(pos == 2 && !instance->IsStandLeft())
				   {
					    pSpriteElf = instance ;
						break;
				   }
				   //elf_iter = elfs.erase(elf_iter);
			   }
		   }
	   }
	   else if(pos >= 20)
	   {
		   std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator _it = m_mapAssistElfIdAndInstances.find(s_id) ;
		   if (_it != m_mapAssistElfIdAndInstances.end())
		   {	
			   std::map<unsigned int,SpriteElf*>& elfs = (*_it).second;
			   for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();elf_iter != elfs.end();elf_iter++)
			   {
				   SpriteElf* instance = (*elf_iter).second;
				   if(pos ==instance->getSpriteCurPos() )
				   {
					   pSpriteElf = instance ;
					   break;
				   }
				   //elf_iter = elfs.erase(elf_iter);
			   }
		   }
	   }
	   return pSpriteElf ;
		    
 }
bool SpriteElfManager::RemoveElfFromHero(PLAYER_ID s_id,bool bDeleteElf /* = true */)
{
	bool bRet = true;
	
	

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
	if (it != m_mapElfIdAndInstances.end())
	{	
		std::map<unsigned int,SpriteElf*>& elfs = (*it).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();elf_iter != elfs.end();elf_iter++)
		{
			SpriteElf* instance = (*elf_iter).second;
			if (bDeleteElf)
			{
				instance->SpriteDelSkill();
				instance->removeFromParentAndCleanup(true);
			}
			//elf_iter = elfs.erase(elf_iter);
		}
		elfs.clear() ;
		m_mapElfIdAndInstances.erase(it);
	}
	
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator _it = m_mapAssistElfIdAndInstances.find(s_id) ;
	if (_it != m_mapAssistElfIdAndInstances.end())
	{	
		std::map<unsigned int,SpriteElf*>& elfs = (*_it).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin(); elf_iter != elfs.end();elf_iter++)
		{
			SpriteElf* instance = (*elf_iter).second;
			if (bDeleteElf)
			{
				instance->SpriteDelSkill();
				instance->removeFromParentAndCleanup(true);
			}

			//elf_iter = elfs.erase(elf_iter);
		}	
		elfs.clear();
		m_mapAssistElfIdAndInstances.erase(_it);
	}

	return bRet;
}

bool SpriteElfManager::RemoveOneElf(PLAYER_ID s_id,unsigned int elfTypeId,bool bDeleteElf /* = true */)
{
	bool bRet = false;

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.find(s_id) ;
	if (it != m_mapElfIdAndInstances.end())
	{	
		std::map<unsigned int,SpriteElf*>& elfs = (*it).second;
		std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.find(elfTypeId);
		if (elf_iter != elfs.end())
		{
			SpriteElf* instance = (*elf_iter).second;
			if (bDeleteElf)
			{
				instance->removeFromParentAndCleanup(true);
			}
			elfs.erase(elf_iter);

			bRet = true;
		}		
	}

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator _it = m_mapAssistElfIdAndInstances.find(s_id) ;
	if (_it != m_mapAssistElfIdAndInstances.end())
	{	
		std::map<unsigned int,SpriteElf*>& elfs = (*_it).second;
		std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.find(elfTypeId);
		if (elf_iter != elfs.end())
		{
			SpriteElf* instance = (*elf_iter).second;
			if (bDeleteElf)
			{
				instance->removeFromParentAndCleanup(true);
			}
			elfs.erase(elf_iter);

			bRet = true;
		}		
	}

	return bRet;
}

void SpriteElfManager::Update(float dt)
{
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator it = m_mapElfIdAndInstances.begin();
	for(; it != m_mapElfIdAndInstances.end(); )
	{
		std::map<unsigned int,SpriteElf*>& elfs = (*it).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();
			elf_iter != elfs.end(); elf_iter++)
		{
			SpriteElf* instance = elf_iter->second;
			if (instance)
			{
				instance->Update(dt);
			}
		}
		
		it++;
	}

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator _it = m_mapAssistElfIdAndInstances.begin();
	for(; _it != m_mapAssistElfIdAndInstances.end(); )
	{
		std::map<unsigned int,SpriteElf*>& elfs = (*_it).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();
			elf_iter != elfs.end(); elf_iter++)
		{
			SpriteElf* instance = elf_iter->second;
			if (instance)
			{
				instance->Update(dt);
			}
		}

		_it++;
	}
}

void SpriteElfManager::AttachElfToOneHero(unsigned int elfTypeId, PLAYER_ID heroId, unsigned int grade, SpriteSeer* parent)
{
	if (elfTypeId == 0)
	{
		return ;
	}
	// Note: 找到需要绑定的 spriteseer
	SpriteSeer * seer = parent;
	if (seer == NULL)
	{
		LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			seer = layer->getPlayer(heroId);
		}
	}

	if (seer != NULL)
	{
		if (false == IsHasElf(heroId,elfTypeId))
		{
			SpriteElf* pElf = CreateInstance(heroId, elfTypeId, grade);
			if (pElf)
			{
				seer->AddElfNum();
				pElf->AttachToOneHero(seer);
			}
		}
	}
}


void SpriteElfManager::PushOneElfChaseState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData /* = 0 */)
{
	PLAYER_ID heroId = pHero->GetID();
	
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapElfIdAndInstances.find(heroId);
	if (iter != m_mapElfIdAndInstances.end())
	{
		std::map<unsigned int,SpriteElf*>& elfs = (*iter).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();
			elf_iter != elfs.end(); elf_iter++)
		{
			SpriteElf* pElf = (*elf_iter).second;
			if (pElf)
			{
				pElf->PushOneElfChaseState(pHero,skillId,pData);
			}
		}		
	}
}

void SpriteElfManager::PushOneElfCloseToState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData /* = 0 */)
{
	PLAYER_ID heroId = pHero->GetID();

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapElfIdAndInstances.find(heroId);
	if (iter != m_mapElfIdAndInstances.end())
	{
		std::map<unsigned int,SpriteElf*>& elfs = (*iter).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();
			elf_iter != elfs.end(); elf_iter++)
		{
			SpriteElf* pElf = (*elf_iter).second;
			if (pElf)
			{
				pElf->PushOneElfCloseToState(pHero,skillId,pData);
			}
		}		
	}
}

void SpriteElfManager::PushOneElfAttackState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData /* = 0 */)
{
	PLAYER_ID heroId = pHero->GetID();

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::iterator iter = m_mapElfIdAndInstances.find(heroId);
	if (iter != m_mapElfIdAndInstances.end())
	{
		std::map<unsigned int,SpriteElf*>& elfs = (*iter).second;
		for (std::map<unsigned int,SpriteElf*>::iterator elf_iter = elfs.begin();
			elf_iter != elfs.end(); elf_iter++)
		{
			SpriteElf* pElf = (*elf_iter).second;
			if (pElf)
			{
				pElf->PushOneElfAttackState(pHero,skillId,pData);
			}
		}		
	}
}

void SpriteElfManager::CheckWhetherNewElfReleaseByReputation()
{
	unsigned int reputaion = UserData::GetUserInfo().m_reputation;
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/SpriteElf/SpriteElfLayer.lua", "CheckWhetherNewElfReleaseByReputation",reputaion);
}

void SpriteElfManager::SetPlayerElfPos(PLAYER_ID id,unsigned int elfId,EFairyPos pos)
{
	std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >::iterator iter = m_mapElfIdAndPos.find(id);
	if (iter != m_mapElfIdAndPos.end())
	{
		std::map<unsigned int,EFairyPos>& elfs = (*iter).second;
		std::map<unsigned int,EFairyPos>::iterator elfIter = elfs.find(elfId);
		if (elfIter != elfs.end())
		{
			(*elfIter).second = pos;
		}
		else
		{
			elfs.insert(std::make_pair(elfId,pos));
		}
	}
	else
	{
		std::map<unsigned int,EFairyPos> elfPosInfos;
		elfPosInfos.insert(std::make_pair(elfId,pos));
		m_mapElfIdAndPos.insert(std::make_pair(id,elfPosInfos));
	}
}

bool SpriteElfManager::GetPlayerElfPos(PLAYER_ID id,unsigned int elfId,EFairyPos& pos)
{
	bool bRet = false;

	std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >::iterator iter = m_mapElfIdAndPos.find(id);
	if (iter != m_mapElfIdAndPos.end())
	{
		std::map<unsigned int,EFairyPos>& elfs = (*iter).second;
		std::map<unsigned int,EFairyPos>::iterator elfIter = elfs.find(elfId);
		if (elfIter != elfs.end())
		{
			pos = (*elfIter).second;
			bRet = true;
		}
	}

	return bRet;
}

unsigned int SpriteElfManager::GetFairyIdByPos(PLAYER_ID id, EFairyPos pos)
{
	unsigned int fairyId = 0;
	std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >::iterator iter = m_mapElfIdAndPos.find(id);
	if (iter != m_mapElfIdAndPos.end())
	{
		std::map<unsigned int,EFairyPos>& elfs = (*iter).second;
		std::map<unsigned int,EFairyPos>::iterator elfIter = elfs.begin();
		for (; elfIter != elfs.end(); elfIter++)
		{
			if (elfIter->second == pos)
			{
				fairyId = elfIter->first;
				break;
			}
		}
	}
	return fairyId;
}

int SpriteElfManager::GetFairyRageBasicByPos(PLAYER_ID id, EFairyPos pos)
{
	int ret = -1;
	unsigned int fairyId = GetFairyIdByPos(id, pos);
	if (fairyId)
	{
		ret = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","GetFairyRageBasic",fairyId);
	}
	return ret;
}

int SpriteElfManager::GetFairyRageRateByPos(PLAYER_ID id, EFairyPos pos)
{
	int ret = -1;
	unsigned int fairyId = GetFairyIdByPos(id, pos);
	if (fairyId)
	{
		ret = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","GetFairyRageRate",fairyId);
	}
	return ret;
}

void SpriteElfManager::AddAssistSkillStartObserver(ElfAssistSkillObserver* pObserver)
{
	m_vecAssistSkillEventObserver.push_back(pObserver);
}

void SpriteElfManager::RemoveAssistSkillObserver(ElfAssistSkillObserver* pObserver)
{
	for (std::vector<ElfAssistSkillObserver*>::iterator iter = m_vecAssistSkillEventObserver.begin();
		iter != m_vecAssistSkillEventObserver.end(); iter++)
	{
		if ((*iter) == pObserver)
		{
			m_vecAssistSkillEventObserver.erase(iter);
			break;
		}
	}	
}

void SpriteElfManager::DoObserverEvent(PLAYER_ID id,unsigned int elfId,unsigned int type)
{
	EFairyPos pos;
	if (GetPlayerElfPos(id,elfId,pos))
	{
		for (std::vector<ElfAssistSkillObserver*>::iterator iter = m_vecAssistSkillEventObserver.begin();
			iter != m_vecAssistSkillEventObserver.end(); iter++)
		{
			(*iter)->OnSkillTrigger(pos,type);
		}
	}
}

void SpriteElfManager::CreateAssistElfs()
{
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());

	if (false == bMainLandLevel)
	{
		for (std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >::iterator iter = m_mapElfIdAndPos.begin();
			iter != m_mapElfIdAndPos.end(); iter++)
		{
			PLAYER_ID id = (*iter).first;
			const std::map<unsigned int,EFairyPos>& elfInfos = (*iter).second;
			for (std::map<unsigned int,EFairyPos>::const_iterator c_iter = elfInfos.begin();
				c_iter != elfInfos.end();c_iter++)
			{
				// Note: 找到需要绑定的英雄
				//SpriteSeer * seer = GameManager::Get()->getHero();
				SpriteSeer * seer = NULL ;
				LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
				if (layer)
				{
					seer = layer->getPlayer(id);
				}
				if (seer != NULL)
				{
					unsigned int elfTypeId = (*c_iter).first;
					if ((*c_iter).second == E_Assist_Pos_1)
					{
						SpriteElf* pElf = CreateAssistInstance(id,elfTypeId);
						if (pElf)
						{
							pElf->SetAssistFightPos(0);
							pElf->AttachToHeroAssistType(seer);
							if(id == UserData::Get()->getUserId())
							{
								unsigned int attackVal =LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerAttackFairyByPos",20);
							    pElf->setSkAttack(attackVal);
							}
						}
						
					
					}
					else if ((*c_iter).second == E_Assist_Pos_2)
					{
						SpriteElf* pElf = CreateAssistInstance(id,elfTypeId);
						if (pElf)
						{
							pElf->SetAssistFightPos(1);
							pElf->AttachToHeroAssistType(seer);
							if(id == UserData::Get()->getUserId())
							{
								unsigned int attackVal =LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerAttackFairyByPos",21);
								pElf->setSkAttack(attackVal);
							}
						}
					}
				}
			}
		    SpriteSeer * seer1 = GameManager::Get()->getHero();

			
			if( seer1 != NULL && id == seer1->GetID())
			{
				if( m_vecAssistSkillEventObserver.size()==0)
				{
					int tag = MainMenuLayer::getLayerTagByName("BattleUILayer");
					BattleUILayer* layer = MainMenuLayer::getUILayerByTag<BattleUILayer>(tag,false);
					if(layer)
					{
						// Note: 创建辅助战斗精灵的Panel
						AssistElfFightPanel* pAssistElfFightPanel = dynamic_cast<AssistElfFightPanel*>(AssistElfFightPanel::create(layer,layer->get_layout()));
						if (pAssistElfFightPanel)
						{
							layer->addChild(pAssistElfFightPanel);
							SpriteElfManager::Get()->AddAssistSkillStartObserver(pAssistElfFightPanel);
						}
					}
				}
				
				
			}
		}
		
	}
}