#include "AutoTaskSystem.h"
#include "TaskManager.h"
#include <libxml/parser.h>
#include "GameResourceManager.h"
#include "TutorialsManager.h"
#include "StoryInstanceDirector.h"
#include "MainMenuLayer.h"
#include "BattleUILayer.h"
#include "LevelManager.h"
#include "InstanceManager.h"
#include "MainLandManager.h"
#include "GameManager.h"

#include "NotificationCenter.h"
#include "GMessage.h"

AutoTaskSystem::AutoTaskSystem()
{
	mIsLoadData = false;
	ReadXML();

	NotificationCenter::defaultCenter()->registerMsgObserver(this,GM_E_TASK_FIRST_ID_STATE);
}

AutoTaskSystem::~AutoTaskSystem()
{

}

void AutoTaskSystem::ReadXML()
{
	if (mIsLoadData)
	{
		return ;
	}
	mIsLoadData = true;

	const char* pFile = "Data/pathfind.bin";
	char *xmlString =(char*) GameResourceManager::sharedManager()->loadXml(pFile);
	xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
	xmlNodePtr pRoot = xmlDocGetRootElement(pDoc);

	if(pRoot != NULL)
	{
		xmlNodePtr pChildrenNode = pRoot->children;
		while (pChildrenNode != NULL)
		{
			unsigned int id = 0;
			unsigned int task_id = 0;

			if(xmlStrcmp(pChildrenNode->name, BAD_CAST"pathfind") == 0)
			{
				xmlAttrPtr attrPtr = pChildrenNode->properties;

				while (attrPtr != NULL)
				{
					if (!xmlStrcmp(attrPtr->name, BAD_CAST "ID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ID");
						id = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "TaskId"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "TaskId");
						task_id = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}

					attrPtr = attrPtr->next;
				}

			}

			if (id != 0 && task_id != 0)
			{
				mTaskSet.insert(task_id);
			}

			pChildrenNode = pChildrenNode->next;
		}
	}

	free(xmlString);
	xmlFreeDoc(pDoc);
}

bool AutoTaskSystem::IsTaskBelongAutoTaskSystem(unsigned int taskId)
{
	std::set<unsigned int >::iterator iter = mTaskSet.find(taskId);
	if (iter != mTaskSet.end())
	{
		return true;
	}
	return false;
}

bool AutoTaskSystem::IsAutoSystemOpen()
{
	bool bOpen = true;
	// Note: 不在主城中
	if (false == MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId()))
	{
		bOpen = false;
	}
	else if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_UNDERGROUND)
		|| LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_CITYDEFEND)
		|| LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS)
		|| LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER))
	{
		bOpen  = false;
	}	
	else
	{
		SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
		if (pSceneLayer)
		{
			MainMenuLayer* pMainMenuLayer = pSceneLayer->getMainMenuLayer();
			if (pMainMenuLayer)
			{
				if (pMainMenuLayer->IsInAutoOnHookModule() ||
					pMainMenuLayer->IsNotice2Load()||
					pMainMenuLayer->IsAutoAppear())
				{
					bOpen = false;
				}
			}
		}
	}

	return bOpen;
}

void AutoTaskSystem::onBroadcastMessage(BroadcastMessage*msg)
{
	if (msg)
	{
		if (msg->message == GM_E_TASK_FIRST_ID_STATE)
		{
			bool isAutoTaskSystemOpen = IsAutoSystemOpen();						
			if (isAutoTaskSystemOpen)
			{
				unsigned int firstLevel = TaskManager::getInstance()->OnlyGetFirstLevelTaskID();
				if (firstLevel != 0)
				{
					std::set<unsigned int >::iterator iter = mTaskSet.find(firstLevel);
					if (iter != mTaskSet.end())
					{	
						if (false == TutorialsManager::Get()->IsInTutorial() && false == StoryInstanceDirector::Get()->IsStart())
						{
							BattleCoverLayer * layer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
							layer->openBattleCoverLayer(COVER_AUTOSEARCH);

							TaskManager::getInstance()->MoveToOneTargetByTaskId(firstLevel);
						}
					}
				}	
			}
		}
	}
}

//void AutoTaskSystem::HandleOneEvent(ESimpleTaskEvent event)
//{
//	if (E_SIMPLE_TASK_EVENT_GET_FIRSRT_ID == event)
//	{
//		unsigned int firstLevel = TaskManager::getInstance()->OnlyGetFirstLevelTaskID();
//		if (firstLevel != 0)
//		{
//			std::set<unsigned int >::iterator iter = mTaskSet.find(firstLevel);
//			if (iter != mTaskSet.end())
//			{	
//				if (false == TutorialsManager::Get()->IsInTutorial() && false == StoryInstanceDirector::Get()->IsStart())
//				{
//					BattleCoverLayer * layer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
//					layer->openBattleCoverLayer(COVER_AUTOSEARCH);
//
//					TaskManager::getInstance()->MoveToOneTargetByTaskId(firstLevel);
//				}
//			}
//		}	
//	}
//}