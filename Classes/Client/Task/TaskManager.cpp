#include "OnlineNetworkManager.h"
#include "TaskManager.h"
#include "cocos2d.h"
#include "TaskConfigFromLuaManager.h"
#include "BoneNPCManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "GameUILayer.h"
#include "SpriteTranspointListener.h"
#include "LevelLayer.h"
#include "SpriteTransPoint.h"
#include "InstanceListLayer.h"
#include "Localization.h"
#include "SpriteSeer.h"
#include "MainLandManager.h"
#include "TutorialsManager.h"
#include "StoryInstanceDirector.h"
#include "LuaTinkerManager.h"
#include "GameDataManager.h"
#include "WaitingLayerManager.h"
#include "GMessage.h"

USING_NS_CC;

static TaskManager* _taskManager = 0;

TaskManager* TaskManager::getInstance(void)
{
	if(_taskManager == 0)
	{
		_taskManager = new TaskManager();
	}
	return _taskManager;
}

void TaskManager::Destroy()
{
	if (_taskManager)
	{
		delete _taskManager;
		_taskManager = NULL;
	}
}

TaskManager::TaskManager()
{
	m_vecAcceptableTask = new std::vector<unsigned int>();
	m_mapAcceptedTask = new std::map<unsigned int,tTaskStepData>();
	m_mapTaskData = new std::map<unsigned int,TaskData>();
	m_mapNpcIdAndTaskIds = new std::map<unsigned int,std::vector<unsigned int> >();

	mCurFirstLevelTaskID = 0;
	mDialogNpcId = 0;
	mTaskState = E_TASK_STATE_UNKNOW;

	m_autoTaskSystem = new AutoTaskSystem();
	//this->registerMsgObserver(m_autoTaskSystem,E_SIMPLE_TASK_EVENT_GET_FIRSRT_ID);
}

TaskManager::~TaskManager()
{
	if (0 != m_vecAcceptableTask)
	{
		m_vecAcceptableTask->clear();	
		CC_SAFE_DELETE(m_vecAcceptableTask);
	}
	if (0 != m_mapAcceptedTask)
	{
		m_mapAcceptedTask->clear();
		CC_SAFE_DELETE(m_mapAcceptedTask);
	}	
	if (0 != m_mapTaskData)
	{
		m_mapTaskData->clear();
		CC_SAFE_DELETE(m_mapTaskData);
	}
	if (0 != m_mapNpcIdAndTaskIds)
	{
		m_mapNpcIdAndTaskIds->clear();
		CC_SAFE_DELETE(m_mapNpcIdAndTaskIds);
	}

	if (m_autoTaskSystem)
	{
		//this->unregisterMsgObserver(m_autoTaskSystem,E_SIMPLE_TASK_EVENT_GET_FIRSRT_ID);
		NotificationCenter::defaultCenter()->unregisterMsgObserver(m_autoTaskSystem);
		m_autoTaskSystem = NULL;
	}	
}


void TaskManager::ResetValue()
{
	m_mapTaskData->clear();
	m_mapNpcIdAndTaskIds->clear();
	m_mapAcceptedTask->clear();
	m_vecAcceptableTask->clear();
	mDialogNpcId = 0;
	mTaskState = E_TASK_STATE_UNKNOW;
}

bool TaskManager::IsTaskBelongAutoTaskSystem(unsigned int taskId)
{
	if (m_autoTaskSystem)
	{
		return m_autoTaskSystem->IsTaskBelongAutoTaskSystem(taskId);
	}
	return false;
}

void TaskManager::SendAutoTaskSystemState()
{
	if (m_autoTaskSystem)
	{
		NotificationCenter::defaultCenter()->broadcast(GM_E_TASK_FIRST_ID_STATE,m_autoTaskSystem);
	}	
}

void TaskManager::UpdateTaskData()
{
	GetTasksMoreData();
	GetFirstLevelTaskID();
	GetNpcTaskData();

	//this->broadCastMessage(E_SIMPLE_TASK_EVENT_GET_FIRSRT_ID);
	if (m_autoTaskSystem)
	{
		NotificationCenter::defaultCenter()->broadcast(GM_E_TASK_FIRST_ID_STATE,m_autoTaskSystem);
	}	
}

void TaskManager::ClearTaskData()
{
	// Note: 清空任务
	m_mapTaskData->clear();
	m_mapNpcIdAndTaskIds->clear();
	m_mapAcceptedTask->clear();
	m_vecAcceptableTask->clear();
}

void TaskManager::RqsTaskFromServer()
{	
	USERINFO mUserData = UserData::GetUserInfo();
	if(mUserData.first_login)
		return;
	ClearTaskData();

	OnlineNetworkManager::sShareInstance()->sendRqsTaskInfoMessage();
	WaitingLayerManager::Get()->PushOneMessage("CSTaskInfoReq");
}

void TaskManager::RqsTaskLogFromServer()
{
	OnlineNetworkManager::sShareInstance()->sendRqsTaskLogMessage();
	WaitingLayerManager::Get()->PushOneMessage("CSTaskLogReq");	
}

void TaskManager::SetOneTaskStepToServer(unsigned int task_id,unsigned int step_id,unsigned int step_value)
{
	OnlineNetworkManager::sShareInstance()->sendRqsTaskDoStepMessage(task_id,step_id,step_value);
}

void TaskManager::GetTasksMoreData()
{
	size_t acceptableTaskVecSize = m_vecAcceptableTask->size();
	for (size_t index = 0;index < acceptableTaskVecSize;index++)
	{
		unsigned int task_id = (*m_vecAcceptableTask)[index];
		TaskData taskData(task_id);
		taskData.GetTaskState();
		m_mapTaskData->insert(std::make_pair(task_id,taskData));
	}

	for (std::map<unsigned int,tTaskStepData>::iterator iter = m_mapAcceptedTask->begin();
		iter != m_mapAcceptedTask->end(); iter++)
	{
		unsigned int task_id = (*iter).first;
		TaskData taskData(task_id);
		taskData.SetTaskStep((*iter).second);
		taskData.GetTaskState();
		m_mapTaskData->insert(std::make_pair(task_id,taskData));
	}
}

void TaskManager::GetFirstLevelTaskID()
{
	unsigned int task_id = 0;
	eTaskState task_state = kType_TASK_UNKNOW;
	for (std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->begin();
		 iter != m_mapTaskData->end(); iter++)
	{
		unsigned int taskId = (*iter).first;
		TaskData taskData = (*iter).second;
		if (taskData.GetCurTaskState() == kType_TASK_WAIT_REWARD)
		{
			if (task_id == 0)
			{
				task_id = taskId;
				task_state = kType_TASK_WAIT_REWARD;
			}
			else
			{
				if (task_state == kType_TASK_WAIT_REWARD)
				{
					if (taskId < task_id)
					{
						task_id = taskId;
					}
				}
				else
				{
					task_id = taskId;
					task_state = kType_TASK_WAIT_REWARD;
				}
			}	
		}
		else if (taskData.GetCurTaskState() == kType_TASK_NOT_ACCEPT)
		{
			if (task_id == 0)
			{
				task_id = taskId;
				task_state = kType_TASK_NOT_ACCEPT;
			}
			else
			{
				if (task_state == kType_TASK_NOT_ACCEPT)
				{
					if (taskId < task_id)
					{
						task_id = taskId;
					}
				}
				else if(task_state == kType_TASK_RUNNING)
				{
					task_id = taskId;
					task_state = kType_TASK_NOT_ACCEPT;
				}
			}
		}
		else if (taskData.GetCurTaskState() == kType_TASK_RUNNING)
		{
			if (task_id == 0)
			{
				task_id = taskId;
				task_state = kType_TASK_RUNNING;
			}
			else
			{
				if (task_state == kType_TASK_RUNNING)
				{
					if (taskId < task_id)
					{
						task_id = taskId;
					}
				}
			}
		}
	}

	mCurFirstLevelTaskID = task_id;

	StoryInstanceDirector::Get()->UpdateByTaskState(mCurFirstLevelTaskID , task_state);
	TutorialsManager::Get()->UpdateByTaskState(mCurFirstLevelTaskID,task_state);
}

eTaskState TaskManager::GetTaskState(unsigned int task_id)
{
	eTaskState task_state = kType_TASK_UNKNOW;
	if (m_mapTaskData == 0)
	{
		return task_state;
	}
	std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->find(task_id);
	if (iter == m_mapTaskData->end())
	{
		return task_state;
	}
	task_state = (*iter).second.GetCurTaskState();
	return task_state;
}

void TaskManager::GetNpcTaskData()
{
	// TODO: 获取当前地图所有的NPC ID
	std::vector<unsigned int> vecNpcId;
	vecNpcId.push_back(501);	

	for (std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->begin();
		iter != m_mapTaskData->end(); iter++)
	{
		unsigned int taskId = (*iter).first;
		TaskData taskData = (*iter).second;

		unsigned int npcId = TaskConfigFromLuaManager::getInstance()->GetTaskAttachNpcId(taskId);
		unsigned int talkToNpcId = TaskConfigFromLuaManager::getInstance()->GetOneTaskTalkToNpcId(taskId);

		eTaskState task_state = GetTaskState(taskId);
		if (task_state != kType_TASK_NOT_ACCEPT && task_state != kType_TASK_UNKNOW)
		{
			if (talkToNpcId != 0)
			{
				npcId = talkToNpcId;
			}
		}

		std::map<unsigned int,std::vector<unsigned int> >::iterator npcTaskIter = m_mapNpcIdAndTaskIds->find(npcId);
		if (npcTaskIter == m_mapNpcIdAndTaskIds->end())
		{
			std::vector<unsigned int> taskVec;
			taskVec.push_back(taskId);
			m_mapNpcIdAndTaskIds->insert(std::make_pair(npcId,taskVec));
		}
		else
		{
			(*npcTaskIter).second.push_back(taskId);
		}		
	}
}

// Note: 得到NPC头上标示符的意义
NPCFlagSign TaskManager::GetNpcFlagIconState(unsigned int npcId)
{
	unsigned int task_id = 0;
	eTaskState task_state = kType_TASK_UNKNOW;

	std::map<unsigned int,std::vector<unsigned int> >::iterator npcTaskIter = m_mapNpcIdAndTaskIds->find(npcId);
	if (npcTaskIter != m_mapNpcIdAndTaskIds->end())
	{
		std::vector<unsigned int> taskIds = (*npcTaskIter).second;
		size_t vecTaskSize = taskIds.size();
		for (size_t index = 0;index<vecTaskSize;index++)
		{
			unsigned int taskId = taskIds[index];
			eTaskState taskState = GetTaskState(taskId);
			// Note: 等待领取奖励
			if (taskState == kType_TASK_WAIT_REWARD)
			{
				if (task_id == 0)
				{
					task_id = taskId;
					task_state = kType_TASK_WAIT_REWARD;
				}
				else
				{
					if (task_state == kType_TASK_WAIT_REWARD)
					{
						if (taskId < task_id)
						{
							task_id = taskId;
						}
					}
					else
					{
						task_id = taskId;
						task_state = kType_TASK_WAIT_REWARD;
					}
				}
			}
			else if (taskState == kType_TASK_NOT_ACCEPT)
			{
				if (task_id == 0)
				{
					task_id = taskId;
					task_state = kType_TASK_NOT_ACCEPT;
				}
				else
				{
					if (task_state == kType_TASK_NOT_ACCEPT)
					{
						if (taskId < task_id)
						{
							task_id = taskId;
						}
					}
					else if(task_state == kType_TASK_RUNNING)
					{
						task_id = taskId;
						task_state = kType_TASK_NOT_ACCEPT;
					}
				}
			}
			else if (taskState == kType_TASK_RUNNING)
			{
				if (task_id == 0)
				{
					task_id = taskId;
					task_state = kType_TASK_RUNNING;
				}
				else
				{
					if (task_state == kType_TASK_RUNNING)
					{
						if (taskId < task_id)
						{
							task_id = taskId;
						}
					}
				}
			}
		}
	}

	if (task_state == kType_TASK_WAIT_REWARD)
	{
		return YELLOW_QUES_MARK;
	}
	else if (task_state == kType_TASK_NOT_ACCEPT)
	{
		return YELLOW_EXCLAMATION_MARK;
	}
	else if (task_state == kType_TASK_RUNNING)
	{
		return GRAY_QUES_MARK;
	}

	return NPCFlagSign_UNKNOW;
}


void TaskManager::UpdateUI()
{
	BoneNPCManager::getInstance()->UpdateAllNpcFlagIcon();
	UpdateUpRightLogoIcon();
}

unsigned int TaskManager::GetTaskAttachNpcId(unsigned int task_id)
{
	unsigned int npc_id = 0;

	for (std::map<unsigned int,std::vector<unsigned int> >::iterator iter = m_mapNpcIdAndTaskIds->begin();
		 iter != m_mapNpcIdAndTaskIds->end();iter++)
	{
		npc_id = (*iter).first;
		std::vector<unsigned int> vecTasks = (*iter).second;
		for (std::vector<unsigned int>::iterator vecIter = vecTasks.begin();
			 vecIter != vecTasks.end();vecIter++)
		{
			unsigned int taskId = (*vecIter);
			if (task_id == taskId)
			{
				return npc_id;
			}
		}
	}
	
	return npc_id;
}

void TaskManager::UpdateUpRightLogoIcon()
{
	std::string taskIconName = GetOneTaskUpRightLogoImage(mCurFirstLevelTaskID);

	if(GetGameUILayer())
	{
		GetGameUILayer()->UpdateTaskUpRightIcon(taskIconName,kType_TASK_UNKNOW);
	}
}

void TaskManager::MoveToOneTargetByTaskId(unsigned int task_id)
{
	if (task_id == 0)
	{
		CCLOG("auto find road step 3 task_id == 0");
		BattleCoverLayer * layer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
		layer->closeBattleCoverLayer(NULL);
		return ;
	}
	SpriteSeer* pHero = GameManager::Get()->getHero();
	if (pHero == NULL)
	{
		CCLOG("auto find road step 4 hero null");
		return ;
	}

	eTaskState taskState = GetTaskState(task_id);
	if (taskState == kType_TASK_NOT_ACCEPT)
	{
		CCLOG("auto find road step 5");
		unsigned int npcId = TaskConfigFromLuaManager::getInstance()->GetTaskAttachNpcId(task_id);
		CCLOG("auto find road step 6 npc id %d",npcId);
		//SpriteNPC* pNpc = BoneNPCManager::getInstance()->GetOneNpc(npcId);
		//CCPoint targetPos = pNpc->getPosition();
		//pNpc->MoveToTarget(targetPos,pNpc->getRelativeParentColliderRect());

		int cityId = 0;
		int mapId = 0;
		CCPoint targetPos = CCPointZero;
		if (MainLandManager::Get()->GetCityIdByNpcId(npcId,cityId,mapId))
		{
			CCLOG("auto find road step 7 get city and map id %d,%d ",cityId,mapId);
			if (MainLandManager::Get()->GetOneCityNpcPos(cityId,npcId,targetPos))
			{
				CCLOG("auto find road step 8 npc pos %f,%f ",targetPos.x,targetPos.y);
				pHero->moveToCityPosition(mapId,targetPos,npcId);
			}			
		}
	}
	else if (taskState == kType_TASK_WAIT_REWARD)
	{
		CCLOG("auto find road step 17 ");
		unsigned int npcId = GetTaskAttachNpcId(task_id);
		CCLOG("auto find road step 18 npc id ",npcId);
		//SpriteNPC* pNpc = BoneNPCManager::getInstance()->GetOneNpc(npcId);
		//CCPoint targetPos = pNpc->getPosition();
		//pNpc->MoveToTarget(targetPos,pNpc->getRelativeParentColliderRect());

		int cityId = 0;
		int mapId = 0;
		CCPoint targetPos = CCPointZero;
		if (MainLandManager::Get()->GetCityIdByNpcId(npcId,cityId,mapId))
		{
			CCLOG("auto find road step 18 get city and map id %d,%d ",cityId,mapId);
			if (MainLandManager::Get()->GetOneCityNpcPos(cityId,npcId,targetPos))
			{
				CCLOG("auto find road step 19 npc pos %f,%f ",targetPos.x,targetPos.y);
				pHero->moveToCityPosition(mapId,targetPos,npcId);
			}			
		}
	}
	else if (taskState == kType_TASK_RUNNING)
	{
		CCLOG("auto find road step 20 taskid ",task_id);
		ShowOneInstanceIconOnLayer(task_id);
		SpriteSeer* pHero = GameManager::Get()->getHero();
		if (pHero)
		{
			CCLOG("auto find road step 21");
			if(task_id < 700000) //task id bellow 700000 is normal fu ben 
			{
				CCLOG("auto find road step 22");
				pHero->MoveToSpriteTranspoint(true);
			}
			else //jing ying fu ben added by jack niu
			{
				CCLOG("auto find road step 23");
				pHero->MoveToSpriteTranspoint(false);
			}	
		}
	}
	else
	{
		CCLOG("auto find road step 30 not rechable code type %d",(int)taskState);
	}
}

void TaskManager::TackInstanceIconClickEvent()
{
	CCLOG("auto find road step 1");
	if (mCurFirstLevelTaskID == 0)
	{
		const char* flag = "M_LEVELUP_TO_GETNEWTASK";
		const char* cotent = Localizatioin::getLocalization(flag);
		ShowMessage(cotent);
		CCLOG("auto find road step 2");
	}
	MoveToOneTargetByTaskId(mCurFirstLevelTaskID);
}

std::vector<unsigned int> TaskManager::GetTalkTaskWithOneNpc(unsigned int npc_id)
{
	std::vector<unsigned int> vecOut;

	for(std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->begin();
		iter != m_mapTaskData->end(); iter++)
	{
		unsigned int task_id = (*iter).first;
		eTaskType task_type = (eTaskType)TaskConfigFromLuaManager::getInstance()->GetOneTaskType(task_id);
		if (task_type == kType_TASK_TALK)
		{
			eTaskState task_state = (eTaskState)GetTaskState(task_id);
			if (task_state == kType_TASK_RUNNING || task_state == kType_TASK_WAIT_REWARD)
			{
				unsigned int npcId = TaskConfigFromLuaManager::getInstance()->GetOneTaskTalkToNpcId(task_id);
				if (npc_id == npcId)
				{
					vecOut.push_back(task_id);
					tTaskStepData stepData;
					stepData.t_step = 2;
					stepData.t_step_value = 0;
					(*iter).second.SetTaskStep(stepData);
					(*iter).second.SetTaskState(kType_TASK_WAIT_REWARD);
				}
			}
		}
	}

	return vecOut;
}

bool TaskManager::GetOneNpcTaskIdsByPriority(unsigned int npc_id,std::vector<unsigned int> &vecOut)
{
	vecOut.clear();

	std::map<unsigned int,int> notAcceptTaskIdMap;
	std::map<unsigned int,int> runningTaskIdMap;
	std::map<unsigned int,int> waitReardTaskIdMap;

	// Note: 判断有没有和自身绑定的对话任务
	//std::vector<unsigned int> vecTalkTask = GetTalkTaskWithOneNpc(npc_id);
	//size_t vecTalkSize = vecTalkTask.size();
	//for (size_t index = 0;index<vecTalkSize;index++)
	//{
	//	waitReardTaskIdMap.insert(std::make_pair(vecTalkTask[index],0));
	//}
	

	std::map<unsigned int,std::vector<unsigned int> >::iterator iter = m_mapNpcIdAndTaskIds->find(npc_id);
	if (iter != m_mapNpcIdAndTaskIds->end())
	{
		std::vector<unsigned int> vecTasks = (*iter).second;
		size_t vecSize = vecTasks.size();
		for (size_t index = 0;index < vecSize;index++)
		{
			unsigned int task_id = vecTasks[index];
			eTaskState task_state = GetTaskState(task_id);
			if (task_state == kType_TASK_NOT_ACCEPT)
			{
				notAcceptTaskIdMap.insert(std::make_pair(task_id,0));
			}
			else if (task_state == kType_TASK_RUNNING)
			{
				runningTaskIdMap.insert(std::make_pair(task_id,0));
			}
			else if (task_state == kType_TASK_WAIT_REWARD)
			{
				waitReardTaskIdMap.insert(std::make_pair(task_id,0));
			}
		}		
	}

	for (std::map<unsigned int,int>::iterator iter = waitReardTaskIdMap.begin();
		iter != waitReardTaskIdMap.end();iter++)
	{
		vecOut.push_back((*iter).first);
	}

	for (std::map<unsigned int,int>::iterator iter = notAcceptTaskIdMap.begin();
		iter != notAcceptTaskIdMap.end();iter++)
	{
		vecOut.push_back((*iter).first);
	}

	for (std::map<unsigned int,int>::iterator iter = runningTaskIdMap.begin();
		iter != runningTaskIdMap.end();iter++)
	{
		vecOut.push_back((*iter).first);
	}

	if (vecOut.size() > 0)
	{
		return true;
	}

	return false;
}

//show fu ben layer. and set current task id in the fu ben layer
void TaskManager::ShowOneInstanceIconOnLayer(unsigned int task_id,bool bShowTip /* = false */)
{
	//MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	//if (mLayer)
	//{
		CCLOG("auto find road step 28 taskid ",task_id);
		InstanceListLayer *pLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
		if (pLayer)
		{
			CCLOG("auto find road step 29 ");
			std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->find(task_id);
			if (iter != m_mapTaskData->end())
			{
				CCLOG("auto find road step 30 ");
				unsigned int task_step = (*iter).second.GetStep();
				CCLOG("auto find road step 31 task_step %d ",task_step);
				unsigned int instance_id = TaskConfigFromLuaManager::getInstance()->GetOneTaskInstanceId(task_id,task_step);
				CCLOG("auto find road step 32 instance_id %d ",instance_id);
				pLayer->setInstanceId(instance_id);
				//pLayer->setTipIconShowOrNot(bShowTip);
			}
		}
	//}
}

void TaskManager::InterruptAutoGoToInstanceEvent()
{
	//MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	//if (mLayer)
	//{
		InstanceListLayer *pLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
		if (pLayer)
		{
			pLayer->setInstanceId(0);
		}
	//}
}

void TaskManager::ShowRewardTaskMessage(std::vector<unsigned int> &vecTaskIds)
{
	for (std::vector<unsigned int>::iterator iter = vecTaskIds.begin();
		 iter != vecTaskIds.end(); iter++)
	{
		unsigned int task_id = (*iter);
		std::map<unsigned int,TaskData>::iterator _iter = m_mapTaskData->find(task_id);
		if (_iter != m_mapTaskData->end())
		{
			eTaskType task_type = (eTaskType)TaskConfigFromLuaManager::getInstance()->GetOneTaskType(task_id);
			eTaskState task_state = GetTaskState(task_id);

			if (task_state == kType_TASK_WAIT_REWARD)
			{
				if (task_type != kType_TASK_TALK && task_type != kType_TASK_TYPE_UNKNOW)
				{
					// Note: 任务名称
					std::string taskFlagName = TaskConfigFromLuaManager::getInstance()->GetOneTaskName(task_id);
					std::string taskName = Localizatioin::getLocalization((taskFlagName).c_str());

					std::string commonTalkFlag = TaskConfigFromLuaManager::getInstance()->GetCommonCompleteContent();
					std::string commonTalkContent = Localizatioin::getLocalization(commonTalkFlag.c_str());

					taskName += commonTalkContent;

					ShowMessage(taskName.c_str());
				}
			}
		}
	}
}

std::string TaskManager::GetOneTaskUpRightLogoImage(unsigned int task_id)
{
	std::string taskIconName = "";
	eTaskState taskState = kType_TASK_UNKNOW;

	// Note: 获取任务相关的Npc头像
	if (task_id == 0)
	{
		// Note: 没有任务可接
		std::string iconName("ICON_INSTANCE");
		taskIconName = std::string("map_ui_system_icon_FRAME_") + iconName;	
	}
	else
	{
		unsigned int npc_id = GetTaskAttachNpcId(task_id);
		NPCData* npcData = BoneNPCManager::getInstance()->GetOneNpcBasicData(npc_id,true);
		if (!npcData)
			return "";

		std::string npcTaskIcon = npcData ->mTaskUpLogoImageIcon;
		transform(npcTaskIcon.begin(), npcTaskIcon.end(), npcTaskIcon.begin(), towupper);
		npcTaskIcon = std::string("map_ui_system_icon_FRAME_") + npcTaskIcon;	

		taskState = GetTaskState(task_id);
		switch(taskState)
		{
		case kType_TASK_NOT_ACCEPT:
			{
				taskIconName = npcTaskIcon;
			}
			break;
		case kType_TASK_RUNNING:
			{
				std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->find(task_id);
				if (iter != m_mapTaskData->end())
				{
					unsigned int task_step = (*iter).second.GetStep();
					const char * taskname = TaskConfigFromLuaManager::getInstance()->GetOneTaskClearanceIconName(task_id,task_step);
					if(taskname != NULL){
						std::string npcTaskIcon(taskname);
						transform(npcTaskIcon.begin(), npcTaskIcon.end(), npcTaskIcon.begin(), towupper);
						npcTaskIcon = std::string("map_ui_system_icon_FRAME_") + npcTaskIcon;
						taskIconName = npcTaskIcon;
					}else{
						CCLOG("taskname is null for task id %d", task_id);
					}
				}
			}
			break;
		case kType_TASK_WAIT_REWARD:
			{
				taskIconName = npcTaskIcon;
			}
			break;
		}
	}
	return taskIconName;
}

std::string TaskManager::GetOneTaskProgreess(unsigned int task_id)
{
	unsigned int step_value = 0;
	bool bWaitForReward = false;
	std::map<unsigned int,TaskData>::iterator iter = m_mapTaskData->find(task_id);
	if (iter != m_mapTaskData->end())
	{
		eTaskState task_state =  GetTaskState((*iter).first);
		if (task_state == kType_TASK_WAIT_REWARD)
		{
			bWaitForReward = true;
		}
		step_value = (*iter).second.GetStepValue();
	}
	return TaskConfigFromLuaManager::getInstance()->GetOneTaskProgress(task_id,step_value,bWaitForReward);
}

void TaskManager::DisplayOneNpcChatLayer(int npcId)
{
	if (IsNpcHasWaitAcceptTask(npcId) || IsNpcHasWaitRewardTask(npcId))
	{
		SpriteNPC* pNpc = BoneNPCManager::getInstance()->GetOneNpc(npcId);
		if (pNpc != 0)
		{
			GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->GetTaskChatLayer()->ShowTaskChatLayer(npcId);
		}
	}
}

bool TaskManager::IsNpcHasWaitAcceptTask(int npcId)
{
	if (m_mapTaskData == 0 || m_mapNpcIdAndTaskIds == 0)
	{
		return false;
	}

	std::map<unsigned int,std::vector<unsigned int> >::iterator iter = m_mapNpcIdAndTaskIds->find(npcId);
	if (iter == m_mapNpcIdAndTaskIds->end())
	{
		return false;
	}
	for (std::vector<unsigned int>::iterator _iter = (*iter).second.begin(); _iter != (*iter).second.end(); _iter++)
	{
		unsigned int taskId = (*_iter);
		std::map<unsigned int,TaskData>::iterator taskIter = m_mapTaskData->find(taskId);
		if (taskIter != m_mapTaskData->end())
		{
			eTaskState taskState = (*taskIter).second.GetCurTaskState();
			if (taskState == kType_TASK_NOT_ACCEPT)
			{
				return true;
			}
		}
	}

	return false;
}

bool TaskManager::IsNpcHasWaitRewardTask(int npcId)
{
	if (m_mapTaskData == 0 || m_mapNpcIdAndTaskIds == 0)
	{
		return false;
	}

	std::map<unsigned int,std::vector<unsigned int> >::iterator iter = m_mapNpcIdAndTaskIds->find(npcId);
	if (iter == m_mapNpcIdAndTaskIds->end())
	{
		return false;
	}
	for (std::vector<unsigned int>::iterator _iter = (*iter).second.begin(); _iter != (*iter).second.end(); _iter++)
	{
		unsigned int taskId = (*_iter);
		std::map<unsigned int,TaskData>::iterator taskIter = m_mapTaskData->find(taskId);
		if (taskIter != m_mapTaskData->end())
		{
			eTaskState taskState = (*taskIter).second.GetCurTaskState();
			if (taskState == kType_TASK_WAIT_REWARD)
			{
				return true;
			}
		}
	}

	return false;
}