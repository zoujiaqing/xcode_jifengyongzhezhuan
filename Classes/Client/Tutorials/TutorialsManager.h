#pragma once
#include "cocos2d.h"
#include "TutorialsDataCenter.h"
#include "TutorialBase.h"
#include "vector"
#include "TaskData.h"

USING_NS_CC;

class TutorialsManager
{
public :
	TutorialsManager();
	~TutorialsManager();

	static TutorialsManager* Get();
	static void Destroy();

	void LoadData();
	/**
	* Instruction : 重置变量
	* @param 
	*/
	void ResetValue();

	/**
	* Instruction : 启动一个教程
	* @param 
	*/
	bool StartOneTutorial(unsigned int id,unsigned int type = kTutorialUnKnow,bool isPlayBegin = true,const char *attachStartControlName = "");
	
	/**
	* Instruction : 处理控件事件
	* @param type 0 单击 1 双击
	*/	
	bool HandleOneEvent(const char* name,unsigned int type);
	bool HandleDragEventOver(int type = 0,unsigned int pos = 0,bool bInEmptyUI = false);

	bool IsInDragTutorial();
	bool IsInTutorial();

	void Update(float dt);

	void ResetTutorialInstance(){
		mTutorialInstance = NULL;
		mCurTutorialId = 0;
	}
	void InsertOneCompelteTutorial(unsigned int id){
		m_vecCompelteTutorials.push_back(id);
	}
	void RevOneTutorialMessage(unsigned int id);
	bool IsOneTutorialExistInCompelteVec(unsigned int id);
	TutorialDataCenter* GetTutorialDataCenter(){return mDataCenter;}

	/**
	* Instruction : 发送设置ID
	* @param 
	*/
	void sendOnceSetReqMessage(unsigned int id);

	/**
	* Instruction : 
	* @param 
	*/
	void SendTutorialStateRqsMessage();
	/**
	* Instruction : 
	* @param 
	*/
	bool IsOneTaskWithOneTutorial(unsigned int task_id);
	/**
	* Instruction : 
	* @param 
	*/
	bool IsLeadTaskWithOneTutorial(unsigned int task_id);
	/**
	* Instruction : 
	* @param 
	*/
	void UpdateByTaskState(unsigned int taskId,eTaskState taskState,const char *attachStartControlName = "");
	/**
	* Instruction : 
	* @param 
	*/
	TutorialDataCenter* GetTutorialData()
	{
		return mDataCenter;
	}

	void SetConfigDataReGetOrNot(bool bGet);

	bool IsOneTutorialOver(unsigned int id);
protected:
	/**
	* Instruction : 启动一个UI教程
	* @param 
	*/
	bool StartOneUiTutorial(unsigned int id,unsigned int type = kTutorialUnKnow,const char *attachStartControlName = "");
	/**
	* Instruction : 启动一个任务教程
	* @param 
	*/
	bool StartOneTaskTutorial(unsigned int taskId,bool isPlayBegin = true,const char *attachStartControlName = "");
	/**
	* Instruction : 启动一个通过副本的UI引导
	* @param 
	*/
	bool StartOneLeadInstaceTaskTutorial(unsigned int taskId,unsigned int when);
private:
	static TutorialsManager* mInstance;

	TutorialDataCenter* mDataCenter;
	TutorialBase* mTutorialInstance;	

	unsigned int mCurTutorialId;
	std::vector<unsigned int> m_vecCompelteTutorials;

	bool bSendData;
	std::vector<int> m_vecTutorialIds;
	unsigned int m_nRecTutorialStateNum;
};