#include "OnlineNetworkManager.h"
#include "SpriteFactory.h"
#include "SpriteElfManager.h"
#include "UserData.h"
#include "SpriteElfConfigFromLua.h"
#include "SpriteElfDataCenter.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "BackPackLayer.h"
#include "LevelLayer.h"

SpriteFactory* SpriteFactory::mFactory = NULL;

SpriteFactory::SpriteFactory()
{
	mAttachElf = NULL;
}

void SpriteFactory::PrintLog()
{
	CCLOG("SpriteFactory::PrintLog");
}

void SpriteFactory::ResetValue()
{
	mAttachElf = NULL;
}

SpriteFactory* SpriteFactory::sharedFactory()
{
	if (mFactory == NULL)
	{
		mFactory = new SpriteFactory();
	}
	return mFactory;
}

void SpriteFactory::Destroy()
{
	if (mFactory)
	{
		delete mFactory;
		mFactory = NULL;
	}
}

int SpriteFactory::GetValue()
{
	return 2;
}

void SpriteFactory::RemoveElfFromParent(CCNode *parentNode)
{
	if (parentNode)
	{
		mAttachElf = dynamic_cast<SpriteElf*>(parentNode->getChildByTag(MAP_TAG_ELF));
		if (mAttachElf != NULL)
		{
			mAttachElf->removeFromParentAndCleanup(true);
			mAttachElf = NULL;
		}
	}
}

void SpriteFactory::ShowElfOnLayerAtPoint(CCNode* parentNode,int type,CCPoint pos)
{
	if (parentNode == NULL)
	{
		return;
	}
	mAttachElf = (SpriteElf*)parentNode->getChildByTag(MAP_TAG_ELF);
	if (mAttachElf != NULL)
	{
		mAttachElf->removeFromParentAndCleanup(true);
		mAttachElf = NULL;
	}
	SpriteElf* pElf = SpriteElfManager::Get()->CreateInstance(type);
	if (pElf)
	{
		parentNode->addChild(pElf,1,MAP_TAG_ELF);
		pElf->setPosition(pos);
		pElf->SetAnim(kTypeIdle_FaceDown_Stand_OTH,0);
		
		mAttachElf = pElf;
	}
}

void SpriteFactory::RemoveElfFromHero(PLAYER_ID roleId,bool bDeleteElf /* = true */)
{
	//SpriteElfManager::Get()->RemoveOneElf(roleId,bDeleteElf);
	SpriteElfManager::Get()->RemoveElfFromHero(roleId,bDeleteElf);
}
void SpriteFactory::RemovePosElfFromHero(PLAYER_ID s_id)
{
	 SpriteElfManager::Get()->RemovePosElfFromHero( s_id);
}
//void SpriteFactory::PushElfIdToLuaTable()
//{
//	SpriteElfConfigFromLuaManager::getInstance()->PushElfIdToLuaTable();
//}

//int SpriteFactory::GetOneElfState(int elfId)
//{
//	return (int)SpriteElfDataCenter::Get()->GetOneElfState(elfId);
//}

void SpriteFactory::AttachElfToOneHero(unsigned int elfTypeId,PLAYER_ID heroId, unsigned int grade)
{
	SpriteElfManager::Get()->AttachElfToOneHero(elfTypeId, heroId, grade, 0);
}

CCNode* SpriteFactory::GetMainMenuLayerNode()
{
	if(GameManager::Get()->GetSceneLayer() == NULL)
	{
		return NULL;
	}
	return (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
}

bool SpriteFactory::ShowElfPanelWithBackPackLayer()
{
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
		layer->ShowElfPanelWithBackPackLayer();
	}
	return true;
}

void SpriteFactory::SendElfAwakeMessage(unsigned int awakeType)
{
	OnlineNetworkManager::sShareInstance()->SendElfAwakeMessage(awakeType);
}

void SpriteFactory::SendAwakeConfigMessage()
{
	OnlineNetworkManager::sShareInstance()->SendAwakeConfigMessage();
}