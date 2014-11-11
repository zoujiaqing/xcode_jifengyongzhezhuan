#include <libxml/parser.h>
#include "OnlineNetworkManager.h"
#include "SystemConditionDataCenter.h"
#include "GameResourceManager.h"
#include "UserData.h"
#include "UIControlsApearControler.h"

SystemConditionDataCenter::SystemConditionDataCenter():
	mIsLoadData(false)
{

}

SystemConditionDataCenter::~SystemConditionDataCenter()
{
	mData.clear();
}

void SystemConditionDataCenter::Init()
{
	if (mIsLoadData)
	{
		return ;
	}
	mIsLoadData = true;

	const char* pFile = "Data/SystemConditions.bin";
	char *xmlString =(char*) GameResourceManager::sharedManager()->loadXml(pFile);
	xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
	xmlNodePtr pRoot = xmlDocGetRootElement(pDoc);

	if(pRoot != NULL)
	{
		xmlNodePtr pChildrenNode = pRoot->children;
		while (pChildrenNode != NULL)
		{
			unsigned int id = 0;
			
			SystemConditionData oneConditionData;

			if(xmlStrcmp(pChildrenNode->name, BAD_CAST"Item") == 0)
			{
				xmlAttrPtr attrPtr = pChildrenNode->properties;

				while (attrPtr != NULL)
				{
					if (!xmlStrcmp(attrPtr->name, BAD_CAST "ID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ID");

						if (strcmp((const char*)szAttr,"") != 0)
						{
							id = atoi((const char*)szAttr);
							oneConditionData.id = id;
						}						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "SystemName"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "SystemName");

						if (strcmp((const char*)szAttr,"") != 0)
						{
							oneConditionData.condtionName = (const char*)szAttr;
						}						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "UITutorialID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "UITutorialID");

						if (strcmp((const char*)szAttr,"") != 0)
						{
							oneConditionData.leadTutorialId = atoi((const char*)szAttr);
						}						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "HeroLevel"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "HeroLevel");

						if (strcmp((const char*)szAttr,"") != 0)
						{
							oneConditionData.heroLevel = atoi((const char*)szAttr);
						}						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "TaskID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "TaskID");

						if (strcmp((const char*)szAttr,"") != 0)
						{
							oneConditionData.taskId = atoi((const char*)szAttr);
						}						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlConfigID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlConfigID");

						if (strcmp((const char*)szAttr,"") != 0)
						{
							oneConditionData.controlAppearConfigId = atoi((const char*)szAttr);
						}						

						xmlFree(szAttr);
					}

					attrPtr = attrPtr->next;
				}

			}

			if (id != 0)
			{				
				mData.insert(std::make_pair(id,oneConditionData));
			}

			pChildrenNode = pChildrenNode->next;
		}
	}

	free(xmlString);
	xmlFreeDoc(pDoc);

	return ;
}

// Note: check all system open or not
void SystemConditionDataCenter::CheckSystemsOpenOrNot()
{
	for (std::map<unsigned int , SystemConditionData>::iterator iter = mData.begin();
		 iter != mData.end(); iter++)
	{
		if ((*iter).second.isSystemOpen == false)
		{
			bool bOpen = CheckOneSystemOpenOrNot((iter));
			if (bOpen)
			{
				int controlAppearConfigId = (*iter).second.controlAppearConfigId;
				if (controlAppearConfigId != -1)
				{
					UIControlAppearControler::Get()->TackRevControlState(controlAppearConfigId);
				}
			}
		}
	}
}

bool SystemConditionDataCenter::CheckOneSystemOpenOrNot(const std::map<unsigned int , SystemConditionData>::iterator &iter)
{
	SystemConditionData data = (*iter).second;
	if (data.isSystemOpen)
	{
		return true;
	}

	bool bCheckHeroLevel = true;
	bool bCheckTutorialId = true;
	bool bCheckTaskId = true;

	// Note: check hero level
	if (data.heroLevel != -1)
	{
		bCheckHeroLevel = CheckHeroLevelValidOrNot(data.heroLevel);
	}

	if (data.leadTutorialId != -1)
	{
		bCheckTutorialId = CheckTutorialIdValidOrNot(data.leadTutorialId);
	}

	if (data.taskId != -1)
	{
		bCheckTaskId = CheckTaskIdValidOrNot(data.taskId);
	}

	bool bSuc = bCheckHeroLevel && bCheckTutorialId && bCheckTaskId;
	if (bSuc)
	{
		(*iter).second.isSystemOpen = true;
	}

	return bSuc;
}

// Note: only check one system open or not
bool SystemConditionDataCenter::CheckOneSystemOpenOrNot(unsigned int id)
{
	std::map<unsigned int , SystemConditionData>::iterator iter = mData.find(id);
	if (iter != mData.end())
	{
		return CheckOneSystemOpenOrNot(iter);
	}
	return false;
}

bool SystemConditionDataCenter::CheckOneSystemOpenOrNot(const char* name)
{
	for (std::map<unsigned int , SystemConditionData>::iterator iter = mData.begin();
		iter != mData.end(); iter++)
	{
		std::string systemName = (*iter).second.condtionName;
		if (strcmp(systemName.c_str(),name) == 0)
		{
			return CheckOneSystemOpenOrNot(iter);
		}
	}

	return false;
}

bool SystemConditionDataCenter::CheckHeroLevelValidOrNot(unsigned int level)
{
	unsigned int heroLevel = UserData::Get()->GetUserLevel();
	if (heroLevel >= level)
	{
		return true;
	}
	return false;
}

bool SystemConditionDataCenter::CheckTutorialIdValidOrNot(unsigned int id)
{
	return true;
}

bool SystemConditionDataCenter::CheckTaskIdValidOrNot(unsigned int id)
{
	// Note: 发送判断任务是否完成的包
	
	return true;
}

void SystemConditionDataCenter::SendCheckTaskStatuMessage()
{
	for (std::map<unsigned int , SystemConditionData>::iterator iter = mData.begin();
		iter != mData.end(); iter++)
	{
		int task_id = (*iter).second.taskId;
		if (task_id != -1)
		{
			mTaskData[task_id] = PB_TASK_CANT_TOUCH;
			OnlineNetworkManager::sShareInstance()->sendGetTaskStatusMessage(task_id);
		}
	}
}

void SystemConditionDataCenter::TackTaskStatus(unsigned int task_id,PB_TASK_STATUS_T status)
{
	if (status != PB_TASK_CANT_TOUCH)
	{
		std::map<unsigned int , PB_TASK_STATUS_T>::iterator iter = mTaskData.find(task_id);
		if (iter != mTaskData.end())
		{
			(*iter).second = status;
			//CheckSystemsOpenOrNot();
		}
		else
		{
			mTaskData.insert(std::make_pair(task_id,status));
		}
	}	
}

int SystemConditionDataCenter::GetTaskStatus(unsigned int task_id)
{
	std::map<unsigned int , PB_TASK_STATUS_T>::iterator iter = mTaskData.find(task_id);
	if (iter != mTaskData.end())
	{
		return (*iter).second;
	}
	else
	{
		return PB_TASK_CANT_TOUCH;
	}
}