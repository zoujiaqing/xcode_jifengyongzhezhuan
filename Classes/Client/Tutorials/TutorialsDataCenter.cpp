#include <libxml/parser.h>
#include "TutorialsDataCenter.h"
#include "GameResourceManager.h"
#include "Localization.h"
#include "DragReceiver.h"
#include "UIManager.h"

TutorialDataCenter::TutorialDataCenter()
{
	mIsLoadData = false;
}

TutorialDataCenter::~TutorialDataCenter()
{
	mIdAndTutorialData.clear();
}

void TutorialDataCenter::InitXMLData()
{
	if (mIsLoadData)
	{
		return ;
	}
	mIsLoadData = true;

	const char* pFile = "Data/Tutorials.bin";
	char *xmlString =(char*) GameResourceManager::sharedManager()->loadXml(pFile);
	xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
	xmlNodePtr pRoot = xmlDocGetRootElement(pDoc);

	if(pRoot != NULL)
	{
		xmlNodePtr pChildrenNode = pRoot->children;
		while (pChildrenNode != NULL)
		{
			unsigned int id = 0;
			bool isExpansionMainMenu = false;
			bool isTaskHead = false;
			bool IsNewWnd = false;
			bool IsAllowInEmptyArea = false;
			unsigned int eventType = 1;
			OneTutorialData oneTutorialData;

			if(xmlStrcmp(pChildrenNode->name, BAD_CAST"Tutorial") == 0)
			{
				xmlAttrPtr attrPtr = pChildrenNode->properties;

				while (attrPtr != NULL)
				{
					if (!xmlStrcmp(attrPtr->name, BAD_CAST "ID"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ID");

						id = atoi((const char*)szAttr);
						oneTutorialData.nId = id;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "UILayout"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "UILayout");

						oneTutorialData.szUiLayoutName = (const char*)szAttr;
												
						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "DstUILayout"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "DstUILayout");

						oneTutorialData.szUiDstLayoutName = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlType"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlType");

						oneTutorialData.szControlType = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlName"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlName");

						oneTutorialData.szControlName = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlTipType"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlTipType");

						oneTutorialData.nTipType = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlTipContent"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlTipContent");

						oneTutorialData.szControlContent = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlTipRelativePt"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlTipRelativePt");

						CCPoint pt;
						std::string tmpStr((const char*)szAttr);
						int endPos = tmpStr.find("/");
						pt.x = atoi(tmpStr.substr(0,endPos).c_str());
						pt.y = atoi(tmpStr.substr(endPos+1,tmpStr.length()-endPos-1).c_str());

						oneTutorialData.relativePoint = pt;
						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "TutorialType"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "TutorialType");

						oneTutorialData.nType = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "nextTutorial"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "nextTutorial");

						oneTutorialData.nNextId = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "IsExpansionMainMenu"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "IsExpansionMainMenu");

						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							isExpansionMainMenu = true;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "IsTaskHeader"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "IsTaskHeader");

						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							isTaskHead = true;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "IsNewWnd"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "IsNewWnd");

						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							IsNewWnd = true;
						}

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlType_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlType_2");

						oneTutorialData.szControlType_2 = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlName_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlName_2");

						oneTutorialData.szControlName_2 = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlTipType_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlTipType_2");

						oneTutorialData.nTipType_2 = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlTipContent_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlTipContent_2");

						oneTutorialData.szControlContent_2 = (const char*)szAttr;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlTipRelativePt_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlTipRelativePt_2");

						CCPoint pt;
						std::string tmpStr((const char*)szAttr);
						int endPos = tmpStr.find("/");
						pt.x = atoi(tmpStr.substr(0,endPos).c_str());
						pt.y = atoi(tmpStr.substr(endPos+1,tmpStr.length()-endPos-1).c_str());

						oneTutorialData.relativePoint_2 = pt;
						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlPos"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlPos");

						oneTutorialData.nPos = atoi((const char*)szAttr);

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlPos_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlPos_2");

						int value = atoi((const char*)szAttr);
						if (-1 == value)
						{
							oneTutorialData.IsCareDstPos = false;
						}
						else
						{
							oneTutorialData.nPos_2 = value;
						}						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlUIType_2"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlUIType_2");

						int type = GetUITypeByString((const char*)szAttr);
						oneTutorialData.nUIType_2 = type;

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "ControlEventType"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "ControlEventType");

						int type = atoi((const char*)szAttr);
						eventType = type;						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "TaskId"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "TaskId");

						int taskId = atoi((const char*)szAttr);
						oneTutorialData.nTaskId = taskId;						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "When"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "When");

						int when = atoi((const char*)szAttr);
						oneTutorialData.nWhen = when;						

						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "IsAllowInEmptyArea"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "IsAllowInEmptyArea");

						if (strcmp((const char*)szAttr,"true") == 0 || strcmp((const char*)szAttr,"TRUE") == 0)
						{
							IsAllowInEmptyArea = true;
						}				

						xmlFree(szAttr);
					}

					attrPtr = attrPtr->next;
				}

			}

			if (id != 0)
			{
				oneTutorialData.isExpansionMainMenu = isExpansionMainMenu;
				oneTutorialData.IsTaskHeader = isTaskHead;
				oneTutorialData.IsNewWnd = IsNewWnd;
				oneTutorialData.nEventType = eventType;
				oneTutorialData.IsAllowInEmptyArea = IsAllowInEmptyArea;
				mIdAndTutorialData.insert(std::make_pair(id,oneTutorialData));
			}

			pChildrenNode = pChildrenNode->next;
		}
	}

	mCurIter = mIdAndTutorialData.end();

	free(xmlString);
	xmlFreeDoc(pDoc);

	return ;
}

void TutorialDataCenter::SetCurIterator(unsigned int id)
{
	mCurIter = mIdAndTutorialData.find(id);
}

unsigned int TutorialDataCenter::GetOneTutorialType()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return 0;
	}

	return (*mCurIter).second.nType;
}

std::string TutorialDataCenter::GetOneTutorialLayoutName()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return "";
	}

	return (*mCurIter).second.szUiLayoutName;
}

std::string TutorialDataCenter::GetOneTutorialDestLayoutName()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return "";
	}

	return (*mCurIter).second.szUiDstLayoutName;
}

std::string TutorialDataCenter::GetOneTutorialControlName(int id)
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return "";
	}

	std::string rst = "";
	if (id == 1)
	{
		rst = (*mCurIter).second.szControlName;
	}
	else if (id == 2)
	{
		rst = (*mCurIter).second.szControlName_2;
	}

	return rst;
}

unsigned int TutorialDataCenter::GetOneTutorialControlType(int id)
{
	unsigned int rst = 0;
	if (mCurIter == mIdAndTutorialData.end())
	{
		return rst;
	}

	std::string szRst = "";
	if (id == 1)
	{
		szRst = (*mCurIter).second.szControlType;
	}
	else if (id == 2)
	{
		szRst = (*mCurIter).second.szControlType_2;
	}

	if (szRst == "UIButton")
	{
		rst = 1;
	}
	else if(szRst == "IconButton")
	{
		rst = 2;
	} else if (szRst == "ControlButton")
    {
        rst = 3;
    }

	return rst;
}

unsigned int TutorialDataCenter::GetOneTutorialTipType(int id)
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return 0;
	}

	int tipType = 0;

	if (1 == id)
	{
		tipType = (*mCurIter).second.nTipType;
	}
	else if (2 == id)
	{
		tipType = (*mCurIter).second.nTipType_2;
	}

	return tipType;
}

int TutorialDataCenter::GetOneTutorialUIType(int id)
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return 0;
	}

	int tipType = 0;

	if (1 == id)
	{
		tipType = (*mCurIter).second.nUIType;
	}
	else if (2 == id)
	{
		tipType = (*mCurIter).second.nUIType_2;
	}

	return tipType;
}

unsigned int TutorialDataCenter::GetOneTutorialPos(int id)
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return 0;
	}

	int tipType = 0;

	if (1 == id)
	{
		tipType = (*mCurIter).second.nPos;
	}
	else if (2 == id)
	{
		tipType = (*mCurIter).second.nPos_2;
	}

	return tipType;
}

unsigned int TutorialDataCenter::GetOneTutorialEventType()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return 0;
	}

	return (*mCurIter).second.nEventType;
}

unsigned int TutorialDataCenter::GetTutorialIdByLeadinstanceIdAndWhen(unsigned int taskId,unsigned int when)
{
	for (std::map<unsigned int ,OneTutorialData>::iterator iter = mIdAndTutorialData.begin();
		 iter != mIdAndTutorialData.end(); iter++)
	{
		unsigned int type = (*iter).second.nType;
		if (3 == type)
		{
			unsigned int task_id = (*iter).second.nTaskId;
			unsigned int nWhen = (*iter).second.nWhen;
			if(task_id == taskId && nWhen == when)
			{
				return (*iter).first;
			}
		}
	}
	return 0;
}

CCPoint TutorialDataCenter::GetOneTutorialRelativePt(int id)
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return CCPointZero;
	}

	CCPoint p = CCPointZero;

	if (1 == id)
	{
		p = (*mCurIter).second.relativePoint;
	}
	else if (2 == id)
	{
		p = (*mCurIter).second.relativePoint_2;
	}

	p.x = p.x * TXGUI::UIManager::sharedManager()->getScaleFactor();
	p.y = p.y * TXGUI::UIManager::sharedManager()->getScaleFactor();

	return p;
}

unsigned int TutorialDataCenter::GetNextTutorialID()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return 0;
	}

	return (*mCurIter).second.nNextId;
}

const char* TutorialDataCenter::GetOneTutorialContent(int id)
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return "";
	}

	std::string rst = "";
	if (id == 1)
	{
		rst = (*mCurIter).second.szControlContent;
	}
	else if (id == 2)
	{
		rst = (*mCurIter).second.szControlContent_2;
	}

	if (rst == "")
	{
		return "";
	}

	return Localizatioin::getLocalization(rst.c_str());
}

bool TutorialDataCenter::GetOneTutorialIsHasTaskHead(unsigned int id,bool &bError)
{
	bError = false;
	std::map<unsigned int ,OneTutorialData>::iterator iter = mIdAndTutorialData.find(id);
	if (iter == mIdAndTutorialData.end())
	{
		bError = true;
		return false;
	}

	return (*iter).second.IsTaskHeader;
}

bool TutorialDataCenter::GetOneTutorialIsNewWnd()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return false;
	}

	return (*mCurIter).second.IsNewWnd;
}

bool TutorialDataCenter::GetOneTutorialIsAllowInEmptyArea()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return false;
	}

	return (*mCurIter).second.IsAllowInEmptyArea;
}

bool TutorialDataCenter::GetOneTutorialIsCareDstPos()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return true;
	}

	return (*mCurIter).second.IsCareDstPos;
}

bool TutorialDataCenter::GetOneTutorialIsExpandMainMenu()
{
	if (mCurIter == mIdAndTutorialData.end())
	{
		return false;
	}

	return (*mCurIter).second.isExpansionMainMenu;
}

void TutorialDataCenter::GetTutorialIds(std::vector< int> &vecOut)
{
	vecOut.clear();

	std::map<unsigned int ,OneTutorialData>::iterator iter = mIdAndTutorialData.begin();
	for (iter;iter != mIdAndTutorialData.end();iter++)
	{
		vecOut.push_back((*iter).first);
	}
}

bool TutorialDataCenter::IsOneTaskHasTutorial(unsigned int taskId)
{
	std::map<unsigned int ,OneTutorialData>::iterator iter = mIdAndTutorialData.begin();
	for (iter;iter != mIdAndTutorialData.end();iter++)
	{
		if (taskId == (*iter).second.nTaskId)
		{
			return true;
		}
	}
	return false;
}

int TutorialDataCenter::GetUITypeByString(std::string type)
{
	TXGUI::DragHandlerType eType = TXGUI::EUnKowned;
	if (type == "EUnKowned")
	{
		eType = TXGUI::EUnKowned;
	}
	else if (type == "EBackPackItem")
	{
		eType = TXGUI::EBackPackItem;
	}
	else if (type == "EStorageItem")
	{
		eType = TXGUI::EStorageItem;
	}
	else if (type == "EEquipmentItem")
	{
		eType = TXGUI::EEquipmentItem;
	}
	else if (type == "EUIBg")
	{
		eType = TXGUI::EUIBg;
	}
	else if (type == "EAlchemyButton")
	{
		eType = TXGUI::EAlchemyButton;
	}
	else if (type == "EAlchemyUpgrade")
	{
		eType = TXGUI::EAlchemyUpgrade;
	}
	else if (type == "EAlchemyBag")
	{
		eType = TXGUI::EAlchemyBag;
	}
	else if (type == "EAlchemyStorage")
	{
		eType = TXGUI::EAlchemyStorage;
	}
	else if (type == "EAlchemyEquip")
	{
		eType = TXGUI::EAlchemyEquip;
	}
	else if (type == "EALchemyToStorage")
	{
		eType = TXGUI::EALchemyToStorage;
	}
	else if (type == "ESkillEquipItem")
	{
		eType = TXGUI::ESkillEquipItem;
	}
	else if (type == "ESlotItem")
	{
		eType = TXGUI::ESlotItem;
	}
	else if (type == "ECardBagItem")
	{
		eType = TXGUI::ECardBagItem;
	}
	else if (type == "ECardEquipItem")
	{
		eType = TXGUI::ECardEquipItem;
	}
	return eType;
}