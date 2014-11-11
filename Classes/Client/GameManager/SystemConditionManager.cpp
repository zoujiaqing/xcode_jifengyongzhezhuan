#include "SystemConditionManager.h"

SystemConditionManager::SystemConditionManager():
	mDataCenter(NULL),
	mIsSendMessage(false)
{

}

SystemConditionManager::~SystemConditionManager()
{
	CC_SAFE_DELETE(mDataCenter);
}

void SystemConditionManager::Init()
{
	mDataCenter = new SystemConditionDataCenter();
	if (mDataCenter)
	{
		mDataCenter->Init();
	}
}

void SystemConditionManager::SendSystemCheckMessage()
{
	if (mDataCenter)
	{
		if (false == mIsSendMessage)
		{
			mDataCenter->SendCheckTaskStatuMessage();
			mIsSendMessage = true;
		}		
	}
}

void SystemConditionManager::TackTaskStatus(unsigned int task_id,PB_TASK_STATUS_T status)
{
	if (mDataCenter)
	{
		mDataCenter->TackTaskStatus(task_id,status);
	}
}

int SystemConditionManager::GetTaskStatus(unsigned int task_id)
{
	if(mDataCenter)
	{
		return mDataCenter->GetTaskStatus(task_id);
	}
	else
	{
		return PB_TASK_CANT_TOUCH;
	}
}