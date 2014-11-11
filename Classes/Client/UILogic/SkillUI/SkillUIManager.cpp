#include "OnlineNetworkManager.h"
#include "SkillUIManager.h"
#include "SkillDispMgr.h"

static SkillUIManager* _skillUIManager = 0;

SkillUIManager::SkillUIManager()
{
	mIsSendSkillReqMessage = false;
}

SkillUIManager::~SkillUIManager()
{

}

SkillUIManager* SkillUIManager::getInstance(void)
{
	if(_skillUIManager == 0)
	{
		_skillUIManager = new SkillUIManager();
	}
	return _skillUIManager;
}


void SkillUIManager::Destroy()
{
	if (_skillUIManager)
	{
		delete _skillUIManager;
		_skillUIManager = 0;
	}
}

void SkillUIManager::ResetValue()
{
	mIsSendSkillReqMessage = false;
}

void SkillUIManager::sendGetUserSkillReqMessage(bool bSender)
{
	if (bSender)
	{
		OnlineNetworkManager::sShareInstance()->sendGetUserSkillReqMessage(UserData::Get()->getUserId());
	}
	else
	{
		if (false == mIsSendSkillReqMessage)
		{
			OnlineNetworkManager::sShareInstance()->sendGetUserSkillReqMessage(UserData::Get()->getUserId());
			mIsSendSkillReqMessage = true;
		}
	}
}

void SkillUIManager::sendGetSlotReqMessage()
{
	OnlineNetworkManager::sShareInstance()->sendGetSlotReqMessage();
}

void SkillUIManager::sendAddUserSkillReqMessage(unsigned int skillId,unsigned int levelId)
{
	OnlineNetworkManager::sShareInstance()->sendAddUserSkillReqMessage(skillId,levelId);
}

void SkillUIManager::sendUserSkillDiffReqMessage(unsigned int skillId,unsigned int levelId)
{
	OnlineNetworkManager::sShareInstance()->sendUserSkillDiffReqMessage(skillId,levelId);
}

void SkillUIManager::InsertOneSkillItemToLua(unsigned int skillId,unsigned int skillLevel)
{

}

void SkillUIManager::sendSetSlotAttributeMessage(unsigned int slotIndex,unsigned int skillId)
{
	OnlineNetworkManager::sShareInstance()->sendSetSlotAttributeMessage(slotIndex,skillId);
	SkillDispMgr::Get()->PushOneSlotInfoToTable(slotIndex,skillId);
}