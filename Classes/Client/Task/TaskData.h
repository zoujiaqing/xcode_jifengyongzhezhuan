#pragma once

struct tTaskStepData
{
	unsigned int t_step;
	unsigned int t_step_value;
	tTaskStepData(){
		t_step = t_step_value = 0;
	}
};

enum eTaskState
{
	kType_TASK_NOT_ACCEPT = 0,
	kType_TASK_RUNNING,
	kType_TASK_WAIT_REWARD,
	kType_TASK_COMPLETE,
	kType_TASK_UNKNOW
};

enum eTaskType
{
	kType_TASK_TALK = 1,
	kType_TASK_CLEARANCE,
	kType_TASK_KILL_MONSTER,
	kType_TASK_COLLECT,
	kType_TASK_TYPE_UNKNOW
};

class TaskData
{
public:
	TaskData(unsigned int taskId){
		mTaskId = taskId;
		mTaskState = kType_TASK_UNKNOW;
	}

	void SetTaskStep(tTaskStepData data){
		mStepTask = data;
	}
	void SetTaskState(eTaskState task_state){
		mTaskState = task_state;
	}
	void GetTaskState();
	eTaskState GetCurTaskState(){return mTaskState;}
	unsigned int GetStepValue(){return mStepTask.t_step_value;}
	unsigned int GetStep(){return mStepTask.t_step;}
private:
	unsigned int mTaskId;
	eTaskState mTaskState;
	tTaskStepData mStepTask;
};