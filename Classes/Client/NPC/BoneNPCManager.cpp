#include "BoneNPCManager.h"
#include "SkeletonAnimRcsManager.h"
#include "GameConfigFromLuaManager.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "LevelLayer.h"
#include "TaskManager.h"
#include "MainLandManager.h"
#include "ATLObjectLayer.h"
#include "LuaTinkerManager.h"

static BoneNPCManager* _npcManager = 0;

BoneNPCManager* BoneNPCManager::getInstance(void)
{
	if(_npcManager == 0)
	{
		_npcManager = new BoneNPCManager();
		_npcManager->Init();
	}
	return _npcManager;
}

BoneNPCManager::~BoneNPCManager()
{
	mMapNpcTypeIdAndSprites->clear();
	delete mMapNpcTypeIdAndSprites ;
	mMapNpcTypeIdAndSprites = 0;
}

void BoneNPCManager::Destroy()
{
	if (_npcManager)
	{
		delete _npcManager;
		_npcManager = NULL;
	}
}

void BoneNPCManager::Init()
{
	mMapNpcTypeIdAndSprites = new std::map<int,SpriteNPC *>();

}


BoneNPCManager::BoneNPCManager()
{

}

void BoneNPCManager::ResetValue()
{
	mMapNpcTypeIdAndSprites->clear();
}

std::map<int,NPCData *>* BoneNPCManager::LoadNPCConfigInfoFromLua(int levelId)
{
	std::map<int,NPCData *>* pMapNpcTypeIdAndData = new std::map<int,NPCData *>();

	std::map<unsigned int,NpcBasicInfo>* pNpcBasicInfo = MainLandManager::Get()->GetOneCityNpcInfos(levelId);
	for (std::map<unsigned int,NpcBasicInfo>::iterator iter = pNpcBasicInfo->begin();
		iter != pNpcBasicInfo->end();iter++)
	{
		int typeId = (*iter).first;

		const char * name = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", typeId, "Name");
		const char * motto = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", typeId, "motto");
		int npcType = LuaTinkerManager::Get()->getLuaConfig<int>("npc", "npcs", typeId, "Type");
		const char * head_icon = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", typeId, "HeadIcon");
		const char * body_icon = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", typeId, "BodyIcon");
		const char * type_flag_icon = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", typeId, "TypeIcon");

		NPCData * pData = new NPCData();
		if (NULL != name)
		{
			pData->mName = std::string(name);
		}		
		if (NULL != motto)
		{
			pData->mMotto = std::string(motto);
		}		
		pData->mType = (NPCType)npcType;
		pData->mRcsTypeID = typeId;
		if (NULL != type_flag_icon)
		{
			pData->mHeadImageIcon = std::string(type_flag_icon);
		}		
		if (NULL != body_icon)
		{
			pData->mTaskIntroImageIcon = std::string(body_icon);
		}		
		if (NULL != head_icon)
		{
			pData->mTaskUpLogoImageIcon = std::string(head_icon);
		}		
		pData->posx = (*iter).second.pos.x;
		pData->posy = (*iter).second.pos.y;
		pData->bFlip = (*iter).second.bFlip;

		pMapNpcTypeIdAndData->insert(std::make_pair(typeId,pData));
	}

	return pMapNpcTypeIdAndData;
}

void BoneNPCManager::LoadOneLevelNPC(int levelID,CCNode *parentNode)
{
	int city_id = MainLandManager::Get()->GetCityIdByMapId(levelID);

	if(0 == parentNode)
	{
		return ;
	}

	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if (bMainLandLevel == false)
	{
		return ;
	}

	// Note: 加载npc动画资源
	SkeletonAnimRcsManager::getInstance()->LoadOneLevelNPCRcs(city_id);

	// Note: 创建Npc实例
	std::map<int,NPCData *>* pMapNpcTypeIdAndData = LoadNPCConfigInfoFromLua(city_id);
	for (std::map<int,NPCData *>::iterator iter = pMapNpcTypeIdAndData->begin();
		 iter != pMapNpcTypeIdAndData->end();iter++)
	{
		// TODO:临时放置的Npc资源ID
		SpriteNPC * pNpcSprite = SpriteNPC::NPCWithData((*iter).second);
		parentNode->addChild(pNpcSprite);

		CATLObjectLayer* pObjLayer = dynamic_cast<CATLObjectLayer*>(parentNode);
		if(pObjLayer)
		{
			pObjLayer->AddRoleShadow(pNpcSprite);
		}

		mMapNpcTypeIdAndSprites->insert(std::make_pair((*iter).first,pNpcSprite));

		CCPoint npcPos((*iter).second->posx,(*iter).second->posy);
		pNpcSprite->setPosition(npcPos);
		pNpcSprite->setZOrder(LevelLayer::sCalcZorder(npcPos));

		if ((*iter).second->bFlip)
		{
			pNpcSprite->SetAnimFlipX(true);
		}
	}

	pMapNpcTypeIdAndData->clear();
	delete pMapNpcTypeIdAndData;
	pMapNpcTypeIdAndData = 0;
}


void BoneNPCManager::UpdateOneNpcFlagIcon(unsigned int npcTypeId)
{
	std::map<int,SpriteNPC *>::iterator iter = mMapNpcTypeIdAndSprites->find(npcTypeId);
	if (iter != mMapNpcTypeIdAndSprites->end())
	{
		SpriteNPC * pNpc = (*iter).second;
		pNpc->ShowFlagSignIcon(TaskManager::getInstance()->GetNpcFlagIconState(npcTypeId));
	}
}

void BoneNPCManager::UpdateAllNpcFlagIcon()
{
	for (std::map<int,SpriteNPC *>::iterator iter = mMapNpcTypeIdAndSprites->begin();
		 iter != mMapNpcTypeIdAndSprites->end(); iter++)
	{
		SpriteNPC * pNpc = (*iter).second;
		pNpc->ShowFlagSignIcon(TaskManager::getInstance()->GetNpcFlagIconState((*iter).first));
	}
}

NPCData * BoneNPCManager::GetOneNpcBasicData(unsigned int npc_id,bool bFromLua /* = false */)
{
	if (false == bFromLua)
	{
		std::map<int,SpriteNPC *>::iterator iter = mMapNpcTypeIdAndSprites->find(npc_id);
		if (iter != mMapNpcTypeIdAndSprites->end())
		{
			return (*iter).second->GetData();
		}
	}
	else
	{
		const char * name = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", npc_id, "Name");
		const char * motto = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", npc_id, "motto");
		int npcType = LuaTinkerManager::Get()->getLuaConfig<int>("npc", "npcs", npc_id, "Type");
		const char * head_icon = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", npc_id, "HeadIcon");
		const char * body_icon = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", npc_id, "BodyIcon");
		const char * type_flag_icon = LuaTinkerManager::Get()->getLuaConfig<const char *>("npc", "npcs", npc_id, "TypeIcon");

		NPCData * pData = new NPCData();

		
		if (name != NULL)
		{
			pData->mName = std::string(name);
		}
		if (motto != NULL)
		{
			pData->mMotto = std::string(motto);
		}		
		pData->mType = (NPCType)npcType;
		pData->mRcsTypeID = npc_id;
		if (type_flag_icon != NULL)
		{
			pData->mHeadImageIcon = std::string(type_flag_icon);
		}
		if (body_icon != NULL)
		{
			pData->mTaskIntroImageIcon = std::string(body_icon);
		}
		if (head_icon != NULL)
		{
			pData->mTaskUpLogoImageIcon = std::string(head_icon);
		}		
		pData->autorelease();
		return pData;
	}

	return 0;
}

SpriteNPC* BoneNPCManager::GetOneNpc(unsigned int npc_id)
{
	std::map<int,SpriteNPC *>::iterator iter = mMapNpcTypeIdAndSprites->find(npc_id);
	if (iter != mMapNpcTypeIdAndSprites->end())
	{
		return (*iter).second;
	}
	return 0;
}

void BoneNPCManager::ShowAllNpc(bool bShow)
{
	for (std::map<int,SpriteNPC *>::iterator iter = mMapNpcTypeIdAndSprites->begin();
		iter != mMapNpcTypeIdAndSprites->end(); iter++)
	{
		SpriteNPC * pNpc = (*iter).second;
		pNpc->setVisible(bShow);
		pNpc->setShadowVisible(bShow);
	}
}