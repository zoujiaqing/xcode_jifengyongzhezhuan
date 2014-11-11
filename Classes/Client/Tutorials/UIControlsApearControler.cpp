#include "OnlineNetworkManager.h"
#include <libxml/parser.h>
#include "UIControlsApearControler.h"
#include "GameResourceManager.h"
#include "Localization.h"
#include "UIManager.h"
#include "TXGUI.h"
#include "UIButton.h"
#include "IconButton.h"
#include "GameManager.h"
#include "SceneLayer.h"
#include "MainMenuLayer.h"
#include "GameUILayer.h"
#include "UserData.h"
#include "LuaTinkerManager.h"
#include "TutorialsManager.h"

UIControlAppearControler::UIControlAppearControler()
{
	bSendData = true;
	mIsLoadData = false;

	bRevTutorialMessageFromServer = false;
	bRevTaskMessageFromServer = false;
	bRevControlQueMessageFromServer = false;
	bReCheckButtonState = false;
	nRecTaskStateMessageNum = 0;
	nRecControlMessageNum = 0;
	nTotalRecControllMessageNums = 0;
}

UIControlAppearControler::~UIControlAppearControler()
{
	mIdAndControlData.clear();
}

void UIControlAppearControler::InitXMLData()
{
	if (mIsLoadData)
	{
		return ;
	}
	mIsLoadData = true;

	const char* pFile = "Data/ControlsAppearTypeConfig.bin";
	char *xmlString =(char*) GameResourceManager::sharedManager()->loadXml(pFile);
	xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
	xmlNodePtr pRoot = xmlDocGetRootElement(pDoc);

	if(pRoot != NULL)
	{
		xmlNodePtr pChildrenNode = pRoot->children;
		while (pChildrenNode != NULL)
		{
			unsigned int id = 0;
			OneContorlAppearData oneControlData;

			if(xmlStrcmp(pChildrenNode->name, BAD_CAST"Item") == 0)
			{
				xmlAttrPtr attrPtr = pChildrenNode->properties;

				while (attrPtr != NULL)
				{
					if (!xmlStrcmp(attrPtr->name, BAD_CAST "ID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ID");

						id = atoi((const char*)szAttr);
						oneControlData.nId = id;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlType"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlType");

						oneControlData.szControlType = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "Layout"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "Layout");

						oneControlData.szLayoutName = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlName"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlName");

						oneControlData.szControlName = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlPosStyle"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlPosStyle");

						if (strcmp((const char*)szAttr,"auto") == 0)
						{
							oneControlData.isControlFixed = false;
						}
						else
						{
							oneControlData.isControlFixed = true;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "HOR"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "HOR");

						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							oneControlData.isControlHor = true;
						}
						else
						{
							oneControlData.isControlHor = false;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "IsFirstInit"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "IsFirstInit");

						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							oneControlData.isFirstInit = true;
						}
						else
						{
							oneControlData.isFirstInit = false;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "OrderIndex"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "OrderIndex");

						oneControlData.nOrderIndex = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "TaskId"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "TaskId");

						oneControlData.nTaskId = atoi((const char*)szAttr);

						OneTaskState taskState;
						taskState.nTaskId = oneControlData.nTaskId;
						taskState.bRevMessage = false;
						taskState.taskStatue = PB_TASK_CANT_TOUCH;
						mIdAndTaskStateData.insert(std::make_pair(taskState.nTaskId,taskState));

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "SystemId"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "SystemId");
						oneControlData.nTutorialId = atoi((const char*)szAttr);
						xmlFree(szAttr);						
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "HeroLevel"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "HeroLevel");

						oneControlData.nHeroLevel = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}

					attrPtr = attrPtr->next;
				}

			}

			if (id != 0)
			{
				bool disableMainMenuControlsManager = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "IsDisableMainMenuControlsManager");
				if (disableMainMenuControlsManager)
				{
					oneControlData.isFirstInit = true;
				}
				mIdAndControlData.insert(std::make_pair(id,oneControlData));
			}

			pChildrenNode = pChildrenNode->next;
		}
	}

	free(xmlString);
	xmlFreeDoc(pDoc);

	return ;
}

void UIControlAppearControler::ShowOneFixControl(TXGUI::UILayout* pLayout,std::string controlName,std::string controlType,bool bShow)
{
	if (pLayout)
	{		
		if (controlType == "UIButton")
		{
			TXGUI::UIButton* pBtn = pLayout->FindChildObjectByName<TXGUI::UIButton>(controlName.c_str());
			if (pBtn)
			{
				pBtn->setVisible(bShow);
			}
		}
	}
}

void UIControlAppearControler::FirstHideAllControls()
{
	TXGUI::UILayout *pLayout = NULL;
	std::string tmpLayoutName = "";
	for (std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
		 iter != mIdAndControlData.end(); iter++)
	{
		if ((*iter).second.isFirstInit == false)
		{
			std::string layOutName = (*iter).second.szLayoutName;
			if (layOutName != tmpLayoutName)
			{
				tmpLayoutName = layOutName;
				pLayout = TXGUI::UIManager::sharedManager()->getUILayout(layOutName.c_str());
			}
			std::string controlType = (*iter).second.szControlType;
			ShowOneFixControl(pLayout,(*iter).second.szControlName,(*iter).second.szControlType,false);
		}		
	}
}

void UIControlAppearControler::InitPushAllAutoControls()
{
	for (std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
		iter != mIdAndControlData.end(); iter++)
	{
		if ((*iter).second.isControlFixed == false && (*iter).second.isFirstInit == true)
		{
			bool bHor = (*iter).second.isControlHor;
			unsigned int insertPos = (*iter).second.nOrderIndex;
			InsertOneControlToAutoItems((*iter).second.szControlName,(*iter).second.szLayoutName,bHor,insertPos);
		}
	}
}

void UIControlAppearControler::InsertOneControlToAutoItems(std::string controlName,std::string layoutName,bool bHor,unsigned int pos)
{
	// TODO: 添加对Pos的处理
	if (layoutName == "MainMenu")
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer != NULL)
		{
			GameUILayer* pGameUILayer = mLayer->GetGameUILayer();
			if (pGameUILayer)
			{
				if (bHor)
				{
					pGameUILayer->MapButtonPointsH(controlName.c_str(),pos);
				}
				else
				{
					pGameUILayer->MapButtonPointsW(controlName.c_str(),pos);
				}
			}
		}
	}
}

void UIControlAppearControler::SendGetControlsState()
{
	if (bSendData)
	{
		std::vector<int> vec;
		for (std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
			iter != mIdAndControlData.end(); iter++)
		{
			if ((*iter).second.isFirstInit == false)
			{
				vec.push_back((*iter).second.nId);					
			}
		}
		nTotalRecControllMessageNums = vec.size();
		OnlineNetworkManager::sShareInstance()->sendOnceReqMessage(vec);

		bSendData = false;
	}
}

void UIControlAppearControler::TackRevControlState(unsigned int id,bool bOnlyRecordData /* = false */)
{
	mUnLockItems.insert(id);
	if (false == bOnlyRecordData)
	{
		ShowOneControl(id);
	}	
}

void UIControlAppearControler::CheckButtonStateAtFirst()
{
	for (std::set<unsigned int>::iterator iter = mUnLockItems.begin();
		 iter != mUnLockItems.end(); iter++)
	{
		ShowOneControl((*iter));
	}	
}

void UIControlAppearControler::ShowOneControl(unsigned int id)
{
	std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.find(id);
	if (iter != mIdAndControlData.end())
	{
		if ((*iter).second.isControlFixed)
		{
			UILayout* pLayout = TXGUI::UIManager::sharedManager()->getUILayout((*iter).second.szLayoutName.c_str());
			if (pLayout)
			{
				ShowOneFixControl(pLayout,(*iter).second.szControlName,(*iter).second.szControlType,true);
			}
		}
		else
		{
			InsertOneControlToAutoItems((*iter).second.szControlName,(*iter).second.szLayoutName,(*iter).second.isControlHor,(*iter).second.nOrderIndex);
		}
	}
}

void UIControlAppearControler::TackOneTutorialEvent(std::string controlType,std::string controlName,std::string layoutName,bool bOnlyRecordData /* = false */)
{
	std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
	for (iter; iter != mIdAndControlData.end(); iter++)
	{
		if ((*iter).second.szControlName == controlName && (*iter).second.szControlType == controlType && (*iter).second.szLayoutName == layoutName)
		{
			if ((*iter).second.isFirstInit == false)
			{
				unsigned int id = (*iter).second.nId;
				std::set<unsigned int>::iterator _iter = mUnLockItems.find(id);
				if (_iter == mUnLockItems.end())
				{
					OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(id);
					TackRevControlState(id,bOnlyRecordData);
				}				
				else
				{
					ShowOneControl(id);
				}
			}
			break;
		}
	}
}

void UIControlAppearControler::TackTaskOverEvent(unsigned int taskId)
{
	std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
	for (iter; iter != mIdAndControlData.end(); iter++)
	{
		if (taskId == (*iter).second.nTaskId)
		{
			unsigned int id = (*iter).second.nId;
			std::set<unsigned int>::iterator _iter = mUnLockItems.find(id);
			if (_iter == mUnLockItems.end())
			{
				OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(id);
				TackRevControlState(id);
			}
			break;
		}
	}
}

void UIControlAppearControler::TackHeroLevelEvent()
{
	unsigned int level = UserData::GetUserInfo().level;

	std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
	for (iter; iter != mIdAndControlData.end(); iter++)
	{
		unsigned int id = (*iter).first;
		unsigned int heroLevel = (*iter).second.nHeroLevel;
		if (heroLevel != 0)
		{
			if (level >= heroLevel)
			{
				std::set<unsigned int>::iterator _iter = mUnLockItems.find(id);
				if (_iter == mUnLockItems.end())
				{
					OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(id);
					TackRevControlState(id);
				}
			}
		}
	}
}

void UIControlAppearControler::ShowAutoButtons(bool bShow)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		GameUILayer* pGameUILayer = mLayer->GetGameUILayer();
		if (pGameUILayer)
		{
			pGameUILayer->ShowAutoButtons(bShow);
		}
	}
}

void UIControlAppearControler::SetConfigDataReGetOrNot(bool bGet)
{
	bSendData = bGet;
	if (bSendData)
	{
		mUnLockItems.clear();
		bReCheckButtonState = false;
		ResetTaskState();
	}
}

void UIControlAppearControler::SendTaskStateQueMessage()
{
	if (false == bReCheckButtonState && bRevTutorialMessageFromServer && bRevTaskMessageFromServer && bRevControlQueMessageFromServer)
	{
		GetAllTutorialAttachedTaskId();

		if (mIdAndTaskStateData.size() == 0)
		{
			ReCheckUIControlOpenOrNot();
			ResetTaskState();
		}
		else
		{
			for (std::map<unsigned int, OneTaskState>::iterator iter = mIdAndTaskStateData.begin();
				iter != mIdAndTaskStateData.end(); iter++)
			{
				OnlineNetworkManager::sShareInstance()->sendGetTaskStatusMessage((*iter).first);
			}
			bReCheckButtonState = true;
		}
	}
}

void UIControlAppearControler::TackTaskStatus(unsigned int task_id,PB_TASK_STATUS_T status) 
{
	std::map<unsigned int, OneTaskState>::iterator iter = mIdAndTaskStateData.find(task_id);
	if (iter != mIdAndTaskStateData.end())
	{
		nRecTaskStateMessageNum ++;
		(*iter).second.bRevMessage = true;
		(*iter).second.taskStatue = status;
	}

	if (nRecTaskStateMessageNum != 0 && (nRecTaskStateMessageNum >= mIdAndTaskStateData.size()))
	{
		ReCheckUIControlOpenOrNot();
		ResetTaskState();
	}
}

void UIControlAppearControler::ResetTaskState()
{
	for (std::map<unsigned int, OneTaskState>::iterator iter = mIdAndTaskStateData.begin();
		iter != mIdAndTaskStateData.end(); iter++)
	{
		(*iter).second.bRevMessage = false;
		(*iter).second.taskStatue = PB_TASK_CANT_TOUCH;
	}
	nRecTaskStateMessageNum = 0;
	nRecControlMessageNum = 0;
	bRevTutorialMessageFromServer = false;
	bRevTaskMessageFromServer = false;
	bRevControlQueMessageFromServer = false;
}

void UIControlAppearControler::ReCheckUIControlOpenOrNot()
{
	if (bRevTutorialMessageFromServer && bRevTaskMessageFromServer && bRevControlQueMessageFromServer)
	{
		for (std::map<unsigned int ,OneContorlAppearData>::iterator iter = mIdAndControlData.begin();
			 iter != mIdAndControlData.end(); iter++)
		{
			if ((*iter).second.isFirstInit)
			{
				continue;
			}
			std::set<unsigned int>::iterator _iter = mUnLockItems.find((*iter).first);
			// Note: 按钮没有解锁
			if (_iter == mUnLockItems.end())
			{
				unsigned int unLockHeroLevel = (*iter).second.nHeroLevel;
				unsigned int unLockTaskId = (*iter).second.nTaskId;
				unsigned int unLockTutorialId = (*iter).second.nTutorialId;

				unsigned int heroLevel = UserData::Get()->GetUserLevel();
				// Note: 检测人物等级相关的按钮
				bool bLockHero = false;
				if (unLockHeroLevel != 0)
				{
					if (heroLevel < unLockHeroLevel)
					{
						bLockHero = true;
					}
				}

				bool bLockTask = false;
				if (unLockTaskId != 0)
				{
					std::map<unsigned int, OneTaskState>::iterator taskIter = mIdAndTaskStateData.find((*iter).first);
					if (taskIter != mIdAndTaskStateData.end())
					{
						if ((*taskIter).second.taskStatue == PB_TASK_CANT_TOUCH)
						{
							bLockTask = true;
						}
					}
				}

				bool bLockTutorial = false;
				if (unLockTutorialId != 0)
				{
					if (false == TutorialsManager::Get()->IsOneTutorialOver(unLockTutorialId))
					{
						std::map<unsigned int,unsigned int>::iterator tutorialIter = mTutorialIdTaskIdData.find(unLockTutorialId);
						if (tutorialIter == mTutorialIdTaskIdData.end())
						{
							bLockTutorial = true;
						}
						else
						{
							std::map<unsigned int, OneTaskState>::iterator taskIter = mIdAndTaskStateData.find((*tutorialIter).second);
							if (taskIter == mIdAndTaskStateData.end())
							{
								bLockTutorial = true;
							}
							else
							{
								if ((*taskIter).second.taskStatue == PB_TASK_CANT_TOUCH)
								{
									bLockTutorial = true;
								}
							}
						}						
					}
				}

				if (bLockHero == false && bLockTask == false && bLockTutorial == false)
				{
					unsigned int id = (*iter).first;
					OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(id);
					TackRevControlState(id);
				}
			}
		}				
	}
}

void UIControlAppearControler::AddOneControllReqMessage(unsigned int id)
{
	nRecControlMessageNum ++;
	if (nRecControlMessageNum >= nTotalRecControllMessageNums)
	{
		bRevControlQueMessageFromServer = true;
		SendTaskStateQueMessage();
	}
}

void UIControlAppearControler::GetAllTutorialAttachedTaskId()
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Task/TaskState.lua", "GetAllTutorialAttachedTaskId");	
}

void UIControlAppearControler::InsertOneTutorialAndTask(unsigned int tutorialId,unsigned int taskId)
{
	mTutorialIdTaskIdData.insert(std::make_pair(tutorialId,taskId));

	std::map<unsigned int, OneTaskState>::iterator iter = mIdAndTaskStateData.find(taskId);
	if (iter == mIdAndTaskStateData.end())
	{
		OneTaskState taskState;
		taskState.nTaskId = taskId;
		taskState.bRevMessage = false;
		taskState.taskStatue = PB_TASK_CANT_TOUCH;
		mIdAndTaskStateData.insert(std::make_pair(taskState.nTaskId,taskState));
	}
}