#include "TaskData.h"
#include "TaskConfigFromLuaManager.h"

void TaskData::GetTaskState()
{
	if (mStepTask.t_step == 0) // 任务未接受
	{
		mTaskState = kType_TASK_NOT_ACCEPT;
	}
	else
	{
		eTaskType task_type = (eTaskType)TaskConfigFromLuaManager::getInstance()->GetOneTaskType(mTaskId);
		if (task_type == kType_TASK_TALK)
		{
			mTaskState = kType_TASK_WAIT_REWARD;
		}
		else
		{
			if (TaskConfigFromLuaManager::getInstance()->IsTaskWaitForGetReward(mTaskId,mStepTask.t_step))
			{
				mTaskState = kType_TASK_WAIT_REWARD;
			}
			else
			{
				mTaskState = kType_TASK_RUNNING;
			}
		}
	}
}