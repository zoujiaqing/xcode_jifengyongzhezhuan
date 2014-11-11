#include "OnlineNetworkManager.h"
#include "NetWorkConfigure.h"
#include "PopupRewardManager.h"
#include "GameResourceManager.h"
#include <libxml/parser.h>
#include "UIPopupReward.h"
#include "TutorialsManager.h"

PopupRewardManager::PopupRewardManager()
{	
	mTutorialId = 0;
	mIsLoadData = false;	
	mIsStartAtBegin = false;
	bSendData = true;
}

PopupRewardManager::~PopupRewardManager()
{
	mTutorialId = 0;
}

void PopupRewardManager::ResetValue()
{

}

void PopupRewardManager::InitXMLData()
{
	return;//jackniu 2015-5-2 not used the PopupReward xml file
	if (mIsLoadData)
	{
		return ;
	}
	mIsLoadData = true;

	const char* pFile = "Data/PopupReward.bin";
	char *xmlString =(char*) GameResourceManager::sharedManager()->loadXml(pFile);
	xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
	xmlNodePtr pRoot = xmlDocGetRootElement(pDoc);

	if(pRoot != NULL)
	{
		xmlNodePtr pChildrenNode = pRoot->children;
		while (pChildrenNode != NULL)
		{
			unsigned int id = 0;
			bool bShowCloseBtn = true;
			bool bModelLayer = false;
			bool bStartAtBegin = false;
			OnePopupRewardData data;

			if(xmlStrcmp(pChildrenNode->name, BAD_CAST"item") == 0)
			{
				xmlAttrPtr attrPtr = pChildrenNode->properties;

				while (attrPtr != NULL)
				{
					if (!xmlStrcmp(attrPtr->name, BAD_CAST "ID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ID");
						id = atoi((const char*)szAttr);
						data.nId = id;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "rewardID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "rewardID");
						unsigned int rewardId = atoi((const char*)szAttr);
						data.nRewardId = rewardId;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "rewardContent"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "rewardContent");
						data.szRewardContent = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "rewardBtnContent"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "rewardBtnContent");
						data.szRewardOkBtnContent = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "showCloseBtn"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "showCloseBtn");
						if (strcmp((const char*)szAttr,"false") == 0 || strcmp((const char*)szAttr,"FALSE") == 0)
						{
							bShowCloseBtn = false;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "taskID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "taskID");
						unsigned int taskId = atoi((const char*)szAttr);
						data.nTaskId = taskId;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "taskWhen"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "taskWhen");
						unsigned int when = atoi((const char*)szAttr);
						data.nWhen = when;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "TutorialID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "TutorialID");
						unsigned int tutorialId = atoi((const char*)szAttr);
						data.nTutorialId = tutorialId;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "modelLayer"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "modelLayer");
						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							bModelLayer = true;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "IsStartAtBegin"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "IsStartAtBegin");
						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							bStartAtBegin = true;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "HeroLevel"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "HeroLevel");
						unsigned int heroLevel = atoi((const char*)szAttr);
						data.nUnLockHeroLevel = heroLevel;

						xmlFree(szAttr);
					}

					attrPtr = attrPtr->next;
				}

			}

			if (id != 0)
			{
				data.bShowCloseBtn = bShowCloseBtn;
				data.bModelLayer = bModelLayer;
				data.bStartAtBegin = bStartAtBegin;
				mIdAndPopupRewardData.insert(std::make_pair(id,data));
			}

			pChildrenNode = pChildrenNode->next;
		}
	}

	free(xmlString);
	xmlFreeDoc(pDoc);

	return ;
}

void PopupRewardManager::ShowOneLayerByTaskState(unsigned int taskId,unsigned int when)
{
	for (std::map<unsigned int,OnePopupRewardData>::iterator iter = mIdAndPopupRewardData.begin();
		 iter != mIdAndPopupRewardData.end(); iter++)
	{
		unsigned int task_id = (*iter).second.nTaskId;
		unsigned int _when = (*iter).second.nWhen;
		if (task_id == taskId && _when == when)
		{
			//if (IsOneItemUsed((*iter).first))
			//{
			//	break;
			//}

			mTutorialId = (*iter).second.nTutorialId;
			mIsStartAtBegin = (*iter).second.bStartAtBegin;
			PopupRewardLayer::Show((*iter).second.szRewardContent.c_str(),(*iter).second.nRewardId,
								   (*iter).second.szRewardOkBtnContent.c_str(),(*iter).second.bShowCloseBtn,NULL,NULL);

			//PushOneItem((*iter).first);
			//OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage((*iter).first + kType_PopupReward_Key);

			if (mIsStartAtBegin)
			{
				if (0 != mTutorialId)
				{
					TutorialsManager::Get()->StartOneTutorial(mTutorialId,kTutorialPopupReward);	
					mTutorialId = 0;
				}
			}
			return ;
		}
	}	
}

void PopupRewardManager::ShowOneLayerByHeroLevel(unsigned int heroLevel)
{
	for (std::map<unsigned int,OnePopupRewardData>::iterator iter = mIdAndPopupRewardData.begin();
		iter != mIdAndPopupRewardData.end(); iter++)
	{
		unsigned int nHeroLevel = (*iter).second.nUnLockHeroLevel;
		if (nHeroLevel != 0 && nHeroLevel <= heroLevel)
		{
			if (IsOneItemUsed((*iter).first))
			{
				continue;;
			}

			mTutorialId = (*iter).second.nTutorialId;
			mIsStartAtBegin = (*iter).second.bStartAtBegin;
			bool bRst = PopupRewardLayer::Show((*iter).second.szRewardContent.c_str(),(*iter).second.nRewardId,
				(*iter).second.szRewardOkBtnContent.c_str(),(*iter).second.bShowCloseBtn,NULL,NULL);

			if (bRst)
			{
				PushOneItem((*iter).first);
				OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage((*iter).first + kType_PopupReward_Key);

				if (mIsStartAtBegin)
				{
					if (0 != mTutorialId)
					{
						TutorialsManager::Get()->StartOneTutorial(mTutorialId,kTutorialPopupReward);	
						mTutorialId = 0;
					}
				}
			}
		}
	}	
}

void PopupRewardManager::SendReqPopupItemsState()
{
	//static bool bSendData = false;
	if (bSendData)
	{
		std::vector<int> vec;
		for (std::map<unsigned int,OnePopupRewardData>::iterator iter = mIdAndPopupRewardData.begin();
			iter != mIdAndPopupRewardData.end(); iter++)
		{
			if ((*iter).second.nUnLockHeroLevel != 0)
			{
				vec.push_back((*iter).first + kType_PopupReward_Key);
			}		
		}
		OnlineNetworkManager::sShareInstance()->sendOnceReqMessage(vec);

		bSendData = false;
	}
}

void PopupRewardManager::OnPopupRewardLayerClose()
{
	if (false == mIsStartAtBegin)
	{
		if (0 != mTutorialId)
		{
			TutorialsManager::Get()->StartOneTutorial(mTutorialId,kTutorialPopupReward);	
			mTutorialId = 0;
		}
	}
}

void PopupRewardManager::SetConfigDataReGetOrNot(bool bGet)
{
	bSendData = bGet;
	if (bSendData)
	{
		mUsedItems.clear();
	}
}