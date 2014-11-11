#pragma  once
#include "cocos2d.h"
#include "Singleton_t.h"
#include "vector"
#include "map"
#include "TaskData.h"
#include "NPCDataDefine.h"
#include "vector"
#include "SimpleTaskEventNotification.h"

#include "AutoTaskSystem.h"

class TaskManager : public SimpleTaskEventNotification
{
public :	
	virtual ~TaskManager();
	static TaskManager* getInstance(void);
	static void Destroy();

	void RqsTaskFromServer();
	void RqsTaskLogFromServer();
	void SetOneTaskStepToServer(unsigned int task_id,unsigned int step_id,unsigned int step_value);

	void AddOneAcceptableTask(unsigned int task_id){m_vecAcceptableTask->push_back(task_id);}
	void AddOneAcceptedTask(unsigned int task_id,tTaskStepData data){
		m_mapAcceptedTask->insert(std::make_pair(task_id,data));
	}

	void ClearTaskData();
	/**
	* Instruction : 得到任务的状态 保存至m_mapTaskData
	* @param 
	*/	
	void GetTasksMoreData();
	/**
	* Instruction : 获取优先级最高的任务
	* @param 
	*/	
	void GetFirstLevelTaskID();
	/**
	* Instruction : 
	* @param 
	*/
	unsigned int OnlyGetFirstLevelTaskID(){return mCurFirstLevelTaskID;}
	/**
	* Instruction : 得到任务的状态
	* @param 
	*/	
	eTaskState GetTaskState(unsigned int task_id);
	/**
	* Instruction : 进入主城后得到任务数据
	* @param 
	*/
	void UpdateTaskData();

	void SendAutoTaskSystemState();
	bool IsTaskBelongAutoTaskSystem(unsigned int taskId);
	/**
	* Instruction : 显示奖励任务消息
	* @param 
	*/
	void ShowRewardTaskMessage(std::vector<unsigned int> &vecTaskIds);
	/**
	* Instruction : 获取NPC相关的任务
	* @param 
	*/
	void GetNpcTaskData();
	/**
	* Instruction : 得到NPC头上符号
	* @param 
	*/
	NPCFlagSign GetNpcFlagIconState(unsigned int npcId);
	/**
	* Instruction : 更新UI面板
	* @param 
	*/
	void UpdateUI();
	/**
	* Instruction : 更新右上图标
	* @param 
	*/
	void UpdateUpRightLogoIcon();
	/**
	* Instruction : 处理点击屏幕右上角图标事件
	* @param 
	*/
	void TackInstanceIconClickEvent();
	/**
	* Instruction : 根据任务id移到至Npc或副本入口
	* @param 
	*/
	void MoveToOneTargetByTaskId(unsigned int task_id);
	/**
	* Instruction : 按照优先级获取某NPC所有的任务
	* @param 
	*/
	bool GetOneNpcTaskIdsByPriority(unsigned int npc_id,std::vector<unsigned int> &vecOut);
	/**
	* Instruction : 获取和某Npc有对话任务的任务id列表
	* @param 
	*/
	std::vector<unsigned int> GetTalkTaskWithOneNpc(unsigned int npc_id);
	/**
	* Instruction : 获取任务相关的Npc图标（任务会发生转移）
	* @param 
	*/
	unsigned int GetTaskAttachNpcId(unsigned int task_id);
	/**
	* Instruction : 在副本列表上显示感叹号图标
	* @param 
	*/
	void ShowOneInstanceIconOnLayer(unsigned int instance_id,bool bShowTip = false);
	/**
	* Instruction : 打断自动寻路到副本
	* @param 
	*/
	void InterruptAutoGoToInstanceEvent();
	/**
	* Instruction : 获取某任务右上显示Logo函数
	* @param 
	*/
	std::string GetOneTaskUpRightLogoImage(unsigned int task_id);
	/**
	* Instruction : 获取可接任务
	* @param 
	*/
	std::vector<unsigned int> * GetAcceptableTasks(){
		return m_vecAcceptableTask;
	}
	/**
	* Instruction : 获取已接任务
	* @param 
	*/
	std::map<unsigned int,tTaskStepData> * GetAcceptedTasks(){
		return m_mapAcceptedTask;
	}
	/**
	* Instruction : 获取某任务的进程
	* @param 
	*/
	std::string GetOneTaskProgreess(unsigned int task_id);

	void DisplayOneNpcChatLayer(int npcId);
	bool IsNpcHasWaitAcceptTask(int npcId);
	bool IsNpcHasWaitRewardTask(int npcId);
	void ResetValue();
private:
	TaskManager();

	// Note: First Data from server
	std::vector<unsigned int> *m_vecAcceptableTask;
	std::map<unsigned int,tTaskStepData> *m_mapAcceptedTask;

	// Note: 处理过的任务数据
	std::map<unsigned int,TaskData> *m_mapTaskData;

	// Note: 每个NPC相关的任务数据
	std::map<unsigned int,std::vector<unsigned int> > *m_mapNpcIdAndTaskIds;

	unsigned int mCurFirstLevelTaskID;

	CC_SYNTHESIZE(unsigned int, mDialogNpcId, DialogNpcId);
	CC_SYNTHESIZE(eTaskGlobleState, mTaskState, TaskState);

	AutoTaskSystem* m_autoTaskSystem;
};