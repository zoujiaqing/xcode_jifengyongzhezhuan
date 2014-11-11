#include "OnlineNetworkManager.h"
#include "UIControlsApearControler.h"
#include "TutorialsManager.h"
#include "UITutorial.h"
#include "TaskConfigFromLuaManager.h"
#include "TaskManager.h"
#include "LuaTinkerManager.h"
#include "WaitingLayerManager.h"
#include "MainLandManager.h"
#include "LevelManager.h"

TutorialsManager* TutorialsManager::mInstance = NULL;

TutorialsManager::TutorialsManager()
{
	mTutorialInstance = NULL;	
	mCurTutorialId = 0;
	m_nRecTutorialStateNum = 0;
	bSendData = true;
	mDataCenter = new TutorialDataCenter();
}

TutorialsManager::~TutorialsManager()
{
	if (mDataCenter)
	{
		delete mDataCenter;
		mDataCenter = NULL;
	}	
	if (mTutorialInstance)
	{
		mTutorialInstance->release();
		mTutorialInstance = NULL;
	}
}

void TutorialsManager::ResetValue()
{
	mCurTutorialId = 0;
	m_nRecTutorialStateNum = 0;
	if (mTutorialInstance)
	{
		mTutorialInstance->release();
		mTutorialInstance = NULL;
	}
}

TutorialsManager* TutorialsManager::Get()
{
	if (mInstance == 0)
	{
		mInstance = new TutorialsManager();
	}
	return mInstance;
}

void TutorialsManager::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

void TutorialsManager::LoadData()
{
	if (mDataCenter)
	{
		mDataCenter->InitXMLData();
	}
}

void TutorialsManager::Update(float dt)
{
	if (mTutorialInstance)
	{
		mTutorialInstance->Update(dt);
	}
}


bool TutorialsManager::StartOneTutorial(unsigned int id,unsigned int type /* = kTutorialUnKnow */,bool isPlayBegin /* = true */,
	const char *attachStartControlName /* = "" */)
{
	if (NULL == mDataCenter)
	{
		return false;
	}

	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if(false == bMainLandLevel)
	{
		return false;	
	}
	


	switch(type)
	{
	case kTutorialPopupReward:
		{
			if (false == StartOneUiTutorial(id,kTutorialPopupReward))
			{
				return false;
			}
		}
		break;
	case kTutorialUI:
		{
			if (false == StartOneUiTutorial(id,kTutorialUI))
			{
				return false;
			}
		}
		break;
	case kTutorialTask:
		{
			if (false == StartOneTaskTutorial(id,isPlayBegin,attachStartControlName))
			{
				return false;
			}
		}
		break;
	case kTutorialLeadTask:
		{
			unsigned int when = 2;
			if (isPlayBegin)
			{
				when = 1;
			}
			if (false == StartOneLeadInstaceTaskTutorial(id,when))
			{
				return false;
			}
		}
		break;
	case kTutorialUnKnow:
		{
			mDataCenter->SetCurIterator(id);
			ETutorialType tutorialType = (ETutorialType)mDataCenter->GetOneTutorialType();
			if (tutorialType == kTutorialUI)
			{
				StartOneUiTutorial(id);
			}
		}
		break;
	};

	return true;
}

bool TutorialsManager::IsOneTutorialOver(unsigned int id)
{
	for (std::vector<unsigned int>::reverse_iterator iter = m_vecCompelteTutorials.rbegin();
		iter != m_vecCompelteTutorials.rend(); iter++)
	{
		if (id <= (*iter))
		{
			return true;
		}
	}	
	return false;
}

bool TutorialsManager::StartOneUiTutorial(unsigned int id,unsigned int type /* = kTutorialUnKnow */,const char * attachStartControlName /* = "" */)
{
	//m_vecCompelteTutorials;
	if (mCurTutorialId == id)
	{
		return false;
	}
	else
	{
		if (false == IsOneTutorialExistInCompelteVec(id))
		{
			sendOnceSetReqMessage(id);
			InsertOneCompelteTutorial(id);
		}
		else
		{
			return false;
		}

		mTutorialInstance = new UITutorial(id,this->mDataCenter);
		mTutorialInstance->Start(attachStartControlName);
		mTutorialInstance->SetTutorailType(type);
		
		mCurTutorialId = id;
		return true;
	}
}

bool TutorialsManager::IsLeadTaskWithOneTutorial(unsigned int task_id)
{
	if (mDataCenter == NULL)
	{
		return false;
	}
	return mDataCenter->IsOneTaskHasTutorial(task_id);
}

bool TutorialsManager::IsOneTaskWithOneTutorial(unsigned int taskId)
{
	unsigned int taskType = TaskConfigFromLuaManager::getInstance()->GetOneTaskType(taskId);
	// Note: 任务类型开始的UI
	if (5 == taskType)
	{
		unsigned int tutorialID = TaskConfigFromLuaManager::getInstance()->GetOneTaskInstanceId(taskId,2);
		if (0 == tutorialID)
		{
			return false;
		}
		bool bError = false;
		bool isTaskHeader = TutorialsManager::Get()->GetTutorialDataCenter()->GetOneTutorialIsHasTaskHead(tutorialID,bError);
		if (bError)
		{
			return false;
		}
		return true;
	}

	return false;
}

bool TutorialsManager::StartOneTaskTutorial(unsigned int taskId,bool isPlayBegin /* = true */,const char * attachStartControlName /* = "" */)
{
	unsigned int taskType = TaskConfigFromLuaManager::getInstance()->GetOneTaskType(taskId);
	// Note: 任务类型开始的UI
	if (5 == taskType)
	{
		unsigned int tutorialID = TaskConfigFromLuaManager::getInstance()->GetOneTaskInstanceId(taskId,2);
		if (0 == tutorialID)
		{
			return false;
		}
		//TutorialsManager::Get()->GetTutorialDataCenter()->SetCurIterator(tutorialID);
		bool bError = false;
		bool isTaskHeader = TutorialsManager::Get()->GetTutorialDataCenter()->GetOneTutorialIsHasTaskHead(tutorialID,bError);
		if (bError)
		{
			return false;
		}
		if (isPlayBegin == isTaskHeader)
		{
			if (mCurTutorialId == tutorialID)
			{
				return false;
			}
			else
			{
				if (StartOneUiTutorial(tutorialID,kTutorialTask,attachStartControlName))
				{
					mCurTutorialId = tutorialID;
				}								
				return true;
			}			
		}
	}
	return false;
}

bool TutorialsManager::StartOneLeadInstaceTaskTutorial(unsigned int taskId,unsigned int when)
{
	if (mDataCenter)
	{
		unsigned int tutorialId = mDataCenter->GetTutorialIdByLeadinstanceIdAndWhen(taskId,when);
		if (0 != tutorialId)
		{
			if (mCurTutorialId == tutorialId)
			{
				return false;
			}
			if (false == StartOneUiTutorial(tutorialId))
			{
				return false;
			}
			mCurTutorialId = tutorialId;
			return true;
		}
	}
	return false;
}

bool TutorialsManager::HandleOneEvent(const char* name,unsigned int type)
{
	if (mTutorialInstance)
	{
		UITutorial *tutorialInstance = dynamic_cast<UITutorial*> (mTutorialInstance);
		if (tutorialInstance)
		{
			return tutorialInstance->HandleOneEvent(name,type);
		}
	}
	return true;
}

bool TutorialsManager::HandleDragEventOver(int type /* = 0 */,unsigned int pos /* = 0 */,bool bInEmptyUI /* = false */)
{
	if (mTutorialInstance)
	{		
		UITutorial *tutorialInstance = dynamic_cast<UITutorial*> (mTutorialInstance);
		if (tutorialInstance)
		{
			return tutorialInstance->HandleDragEventOver(type,pos,bInEmptyUI);
		}
	}

	return false;
}

bool TutorialsManager::IsInDragTutorial()
{
	if (mTutorialInstance)
	{		
		UITutorial *tutorialInstance = dynamic_cast<UITutorial*> (mTutorialInstance);
		if (tutorialInstance)
		{
			return tutorialInstance->IsInDragTutorial();
		}
	}
	return false;
}

bool TutorialsManager::IsInTutorial()
{
	if (mTutorialInstance)
	{		
		UITutorial *tutorialInstance = dynamic_cast<UITutorial*> (mTutorialInstance);
		if (tutorialInstance)
		{
			return tutorialInstance->IsInTutorial();
		}
	}
	return false;
}

bool TutorialsManager::IsOneTutorialExistInCompelteVec(unsigned int id)
{
	for (std::vector<unsigned int>::iterator iter = m_vecCompelteTutorials.begin();
		 iter != m_vecCompelteTutorials.end(); iter++)
	{
		if ((*iter) == id)
		{
			return true;
		}
	}	
	return false;
}

void TutorialsManager::sendOnceSetReqMessage(unsigned int id)
{
	OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(id+10000);
}

void TutorialsManager::SendTutorialStateRqsMessage()
{
	if (bSendData)
	{
		if (mDataCenter)
		{
			mDataCenter->GetTutorialIds(m_vecTutorialIds);

			for (std::vector<int>::iterator iter = m_vecTutorialIds.begin();
				iter != m_vecTutorialIds.end(); iter++)
			{
				(*iter) += 10000;
			}

			OnlineNetworkManager::sShareInstance()->sendOnceReqMessage(m_vecTutorialIds);
		}

		bSendData = false;
	}
}

void TutorialsManager::UpdateByTaskState(unsigned int taskId,eTaskState taskState,const char* attachStartControlName /* = "" */)
{
	switch(taskState)
	{
	case kType_TASK_WAIT_REWARD:
		{
			// Note: 主城中任务，等待领取奖励
			StartOneTutorial(taskId,kTutorialLeadTask,false);
			break;
		}
	case kType_TASK_NOT_ACCEPT:
		{
			// Note: 主城中任务,，任务等待接受
			TutorialsManager::Get()->StartOneTutorial(taskId,kTutorialTask,true,attachStartControlName);
			TutorialsManager::Get()->StartOneTutorial(taskId,kTutorialLeadTask,true);

			// Note: 处理上次已经完成的任务，但是剧情可能没有触发
			unsigned int preTaskID = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Task/TaskState.lua", "GetPreBuildTaskID", taskId);
			TutorialsManager::Get()->StartOneTutorial(preTaskID,kTutorialTask,true,attachStartControlName);
			TutorialsManager::Get()->StartOneTutorial(preTaskID,kTutorialLeadTask,true);			
			break;
		}
	case kType_TASK_RUNNING:
		{
			// Note: 主城中任务,，处理任务接受完成后，
			if (TutorialsManager::Get()->StartOneTutorial(taskId,kTutorialTask,false))
			{
				if (TutorialsManager::Get()->IsOneTaskWithOneTutorial(taskId))
				{
					TaskManager::getInstance()->SetOneTaskStepToServer(taskId,2,1);
					WaitingLayerManager::Get()->PushOneMessage("CSTaskDoStepReq");
				}
			}
			else
			{
				if (TutorialsManager::Get()->StartOneTutorial(taskId,kTutorialTask,true))
				{
					if (TutorialsManager::Get()->IsOneTaskWithOneTutorial(taskId))
					{
						TaskManager::getInstance()->SetOneTaskStepToServer(taskId,2,1);
						WaitingLayerManager::Get()->PushOneMessage("CSTaskDoStepReq");
					}
				}
			}
			break;
		}
	}
}

void TutorialsManager::SetConfigDataReGetOrNot(bool bGet)
{
	bSendData = bGet;
	if (bSendData)
	{
		mCurTutorialId = 0;
		m_vecCompelteTutorials.clear();
	}
}

void TutorialsManager::RevOneTutorialMessage(unsigned int id)
{
	m_nRecTutorialStateNum ++;
	if (m_nRecTutorialStateNum >= m_vecTutorialIds.size())
	{
		UIControlAppearControler::Get()->SetTutorialMessageFromServerOrNot(true);
		UIControlAppearControler::Get()->SendTaskStateQueMessage();
	}
}