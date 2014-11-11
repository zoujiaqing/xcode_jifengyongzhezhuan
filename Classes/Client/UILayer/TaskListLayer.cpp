#include "TaskListLayer.h"
#include "UIManager.h"
#include "GameResourceManager.h"
#include "TaskManager.h"
#include "GameConfigFromLuaManager.h"
#include "TaskConfigFromLuaManager.h"
#include "Localization.h"
#include "AspriteManager.h"
#include "IconButton.h"
#include "GameFontManager.h"
#include "algorithm"
#include "MainMenuLayer.h"
using namespace TXGUI;

TaskListLayer::TaskListLayer()
{
	taskListLayout = 0;
	m_pAcceptedList = 0;
	m_pAcceptableList = 0;

	m_acceptedTaskToggle = 0;
	m_acceptableTaskToggle = 0;
}

TaskListLayer::~TaskListLayer()
{
	UIManager::sharedManager()->RemoveUILayout("taskListPanel");
}

bool TaskListLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();

	taskListLayout = manager->CreateUILayoutFromFile("UIplist/taskListPanel.plist", this, "taskListPanel");

	m_pAcceptedList = taskListLayout->FindChildObjectByName<UIScrollList>("taskAcceptedScrollList");
	m_pAcceptableList = taskListLayout->FindChildObjectByName<UIScrollList>("taskAcceptableScrollList");

	manager->RegisterMenuHandler("taskListPanel","TaskListLayer::closeBtnClick",
		menu_selector(TaskListLayer::closeBtnClick), this);
	manager->RegisterMenuHandler("taskListPanel","TaskListLayer::tabAcceptedTaskBtnClick",
		menu_selector(TaskListLayer::tabAcceptedTaskBtnClick), this);
	manager->RegisterMenuHandler("taskListPanel","TaskListLayer::tabAcceptableTaskBtnClick",
		menu_selector(TaskListLayer::tabAcceptableTaskBtnClick), this);

	m_acceptedTaskToggle = taskListLayout->FindChildObjectByName<UIButtonToggle>("tabAcceptedBtn");
	m_acceptableTaskToggle = taskListLayout->FindChildObjectByName<UIButtonToggle>("tabAcceptableBtn");

	return true;
}

void TaskListLayer::closeBtnClick(CCObject* sender)
{
	ShowTaskListLayer(false);

	MainMenuLayer::RemoveOneLayer(this);
}

bool TaskListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void TaskListLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void TaskListLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void TaskListLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void TaskListLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -127, true);
}

void TaskListLayer::ShowTaskListLayer(bool bShow)
{
	this->setVisible(bShow);
	if (bShow)
	{
		m_pAcceptedList->clearChildrens();
		m_pAcceptableList->clearChildrens();

		InitListWithElements();
		tabAcceptedTaskBtnClick(0);

		MainMenuLayer::PushOneLayer(this);
	}
	else
	{
		m_pAcceptedList->clearChildrens();
		m_pAcceptableList->clearChildrens();
	}
}

void TaskListLayer::closeLayerCallBack(void)
{
	this->setVisible(false);
	m_pAcceptedList->clearChildrens();
	m_pAcceptableList->clearChildrens();
}

void TaskListLayer::tabAcceptedTaskBtnClick(CCObject* sender)
{
	if (m_acceptedTaskToggle)
	{
		m_acceptedTaskToggle->selected();
	}
	if (m_acceptableTaskToggle)
	{
		m_acceptableTaskToggle->unselected();
	}    

	ShowAccptedTaskList();
}

void TaskListLayer::tabAcceptableTaskBtnClick(CCObject* sender)
{
    if (m_acceptedTaskToggle)
    {
		m_acceptedTaskToggle->unselected();
    }
	if (m_acceptableTaskToggle)
	{
		m_acceptableTaskToggle->selected();
	}

	ShowAcceptableTaskList();
}

CCSprite* TaskListLayer::GetSpriteFromSystemIconByName(const char *rcs_name)
{
	int iSpriteFrame = TXGUI::getResourceIDByName(rcs_name);
	ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
	if (as == 0)
	{
		return 0;
	}
	CCPoint pos;
	CCSprite * pSprite = as->getSpriteFromFrame_Middle(iSpriteFrame, 0,pos);
	return pSprite;
}

void TaskListLayer::ListItemClickEvent(CCObject* sender)
{
	CCNode* node = dynamic_cast<CCNode*>(sender);
	if (0 != node)
	{
		CCNode* pParentNode = node->getParent();
		if (pParentNode)
		{
			CCNode* iconNode = pParentNode->getParent();
			IconButton* button = dynamic_cast<IconButton*>(iconNode);
			if(button)
			{
				unsigned int task_id = button->GetButtonIndex();
				TaskManager::getInstance()->MoveToOneTargetByTaskId(task_id);
				ShowTaskListLayer(false);
			}
		}
	}	
}

void TaskListLayer::AddNewListItem(unsigned int task_id,UIScrollList* pList)
{
	if (pList)
	{
		std::string backGroundFrameName = "map_ui_FRAME_TASK_FRAME_TASKGRID1";
		CCSprite* pBGFrame = AspriteManager::getInstance()->getOneFrame(KUI_BIN,backGroundFrameName);
		CCSize bgFrameSize = pBGFrame->getContentSize();		

		CCNode* pBaseNode = CCNode::create();
		TXGUI::UIControlBase*pBase = pList->AddBaseItem(bgFrameSize,pBaseNode);

		CCPoint relativePoint;
		CCPoint anchorPoint(0.5,0.5);
		std::string childFrameName = "";

		// Note: Frame Task BackGround
		childFrameName = "map_ui_FRAME_TASK_FRAME_TASK1";
		CCSprite* pChildSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		pChildSprite->setPosition(relativePoint);
		pBaseNode->addChild(pChildSprite);

		// Note: 左侧地图Icon
		childFrameName = "map_ui_FRAME_TASK_FRAME_TASKICON1";
		CCSprite* pLeftCircleSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		pLeftCircleSprite->setPosition(relativePoint);
		pBaseNode->addChild(pLeftCircleSprite);

		std::string TaskIcon(TaskManager::getInstance()->GetOneTaskUpRightLogoImage(task_id));
		CCSprite* pTaskIconMap = AspriteManager::getInstance()->getOneFrame(KICON_BIN,TaskIcon);
		pTaskIconMap->setPosition(relativePoint);
		pBaseNode->addChild(pTaskIconMap);
		
		// Note: 自动寻路Btn
		childFrameName = "map_ui_FRAME_TASK_BUTTON_AUTOFIND";
		AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		CCSprite* pAutoTaskNormalIcon = AspriteManager::getInstance()->getOneFrame(KUI_BIN,"map_ui_FRAME_TASK_BUTTON_AUTOFIND");
		CCSprite* pAutoTaskClickIcon = AspriteManager::getInstance()->getOneFrame(KUI_BIN,"map_ui_FRAME_TASK_BUTTON_AUTOFIND_CLICKED");		

		IconButton* pbtn = new IconButton(pAutoTaskNormalIcon,0,pAutoTaskClickIcon,this,menu_selector(TaskListLayer::ListItemClickEvent));
		pbtn->autorelease();
		pbtn->SetButtonIndex(task_id);
		pbtn->setPosition(relativePoint);
		pBaseNode->addChild(pbtn);
		std::string autoBtnText = Localizatioin::getLocalization("T_common_text_auto_path");
		UILabelTTF * autoFindPathLabel = UILabelTTF::create(autoBtnText.c_str(), KJLinXin, 32);
		pBaseNode->addChild(autoFindPathLabel,1);
		autoFindPathLabel->setPosition(relativePoint);

		// Note: 显示任务名称
		childFrameName = "map_ui_FRAME_TASK_TEXT_TASKNAME";
		CCSprite* pTaskNameLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		std::string taskFlagName = TaskConfigFromLuaManager::getInstance()->GetOneTaskName(task_id);
		std::string taskName = Localizatioin::getLocalization((taskFlagName).c_str());
		CCSize labelSize = pTaskNameLabelSprite->getContentSize();
		UILabelTTF * taskNameLabel = UILabelTTF::create(taskName.c_str(), KJLinXin, 28,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
		taskNameLabel->setColor(ccc3(238,166,26));
		taskNameLabel->setPosition(relativePoint);
		pBaseNode->addChild(taskNameLabel);

		// Note: 任务描述
		childFrameName = "map_ui_FRAME_TASK_TEXT_TASKDESCRIPTION";
		CCSprite* pTaskDescriptionLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		std::string taskDesFlagName = TaskConfigFromLuaManager::getInstance()->GetOneTaskDescription(task_id);
		std::string taskDesName = Localizatioin::getLocalization((taskDesFlagName).c_str());

		labelSize = pTaskDescriptionLabelSprite->getContentSize();
		UILabelTTF * taskDescriptionLabel = UILabelTTF::create(taskDesName.c_str(), KJLinXin, 20,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
		taskDescriptionLabel->setPosition(relativePoint);
		pBaseNode->addChild(taskDescriptionLabel);

		// Note: 任务进度
		childFrameName = "map_ui_FRAME_TASK_TEXT_TASKPROGRESS";
		CCSprite* pTaskProgressLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		std::string progressName = TaskManager::getInstance()->GetOneTaskProgreess(task_id);
		labelSize = pTaskProgressLabelSprite->getContentSize();
		UILabelTTF * taskProgressLabel = UILabelTTF::create(progressName.c_str(), KJLinXin, 20,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
		taskProgressLabel->setPosition(relativePoint);
		pBaseNode->addChild(taskProgressLabel);

		// Note: 字体颜色显示
		eTaskState task_state =  TaskManager::getInstance()->GetTaskState(task_id);
		if (task_state == kType_TASK_WAIT_REWARD)
		{
			taskProgressLabel->setColor(ccc3(238,166,26));
		}

		// Note: 奖励文本
		childFrameName = "map_ui_FRAME_TASK_TEXT_REWARD";
		CCSprite* pTaskRewardLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		std::string rewardText = Localizatioin::getLocalization("T_common_text_reward");
		labelSize = pTaskRewardLabelSprite->getContentSize();
		UILabelTTF * taskRewardLabel = UILabelTTF::create(rewardText.c_str(), KJLinXin, 24,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		taskRewardLabel->setPosition(relativePoint);
		taskRewardLabel->setColor(ccc3(195, 114, 67));
		pBaseNode->addChild(taskRewardLabel);

		// Note: 奖励金币图标
		childFrameName = "map_ui_FRAME_TASK_ICON_COIN";
		CCSprite* pTaskIconIconLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		pTaskIconIconLabelSprite->setPosition(relativePoint);
		pBaseNode->addChild(pTaskIconIconLabelSprite);
		// Note: 奖励经验图标
		childFrameName = "map_ui_FRAME_TASK_ICON_EXP";
		CCSprite* pTaskIconExpLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		pTaskIconExpLabelSprite->setPosition(relativePoint);
		pBaseNode->addChild(pTaskIconExpLabelSprite);

		// Note: 奖励金币文本
		unsigned int moneyNum = 0;
		TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardMoney(task_id,moneyNum);
		unsigned int expNum = 0;
		TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardExp(task_id,expNum);

		char buffer[100];
		sprintf(buffer,"%d",moneyNum);

		childFrameName = "map_ui_FRAME_TASK_TEXT_COIN";
		CCSprite* pTaskTextIconLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		labelSize = pTaskTextIconLabelSprite->getContentSize();
		UILabelTTF * taskTextIconLabel = UILabelTTF::create(buffer, KJLinXin, 24,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		taskTextIconLabel->setPosition(relativePoint);
		pBaseNode->addChild(taskTextIconLabel);

		// Note: 奖励经验文本
		childFrameName = "map_ui_FRAME_TASK_TEXT_EXP";
		CCSprite* pTaskTextExpLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		memset(buffer,0,sizeof(buffer)/sizeof(char));
		sprintf(buffer,"%d",expNum);
		labelSize = pTaskTextExpLabelSprite->getContentSize();
		UILabelTTF * taskTextExpLabel = UILabelTTF::create(buffer, KJLinXin, 24,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		taskTextExpLabel->setPosition(relativePoint);
		pBaseNode->addChild(taskTextExpLabel);
	}
}

void TaskListLayer::ShowAccptedTaskList()
{
	if (m_pAcceptableList)
	{
		m_pAcceptableList->setIsVisible(false);
	}
	if (m_pAcceptedList)
	{
		m_pAcceptedList->setIsVisible(true);
	}
}

void TaskListLayer::ShowAcceptableTaskList()
{
	if (m_pAcceptableList)
	{
		m_pAcceptableList->setIsVisible(true);
	}
	if (m_pAcceptedList)
	{
		m_pAcceptedList->setIsVisible(false);
	}
}

void TaskListLayer::InitListWithElements()
{
	if (m_pAcceptableList)
	{
		std::vector<unsigned int> * pVec = TaskManager::getInstance()->GetAcceptableTasks();
		for (std::vector<unsigned int>::iterator iter = pVec->begin();
			iter != pVec->end(); iter++)
		{
			unsigned int task_id = (*iter);
			AddNewListItem(task_id,m_pAcceptableList);
		}
	}

	if (m_pAcceptedList)
	{
		std::map<unsigned int,tTaskStepData> * pMap = TaskManager::getInstance()->GetAcceptedTasks();
		for (std::map<unsigned int,tTaskStepData>::iterator iter = pMap->begin();
			iter != pMap->end(); iter++)
		{
			unsigned int task_id = (*iter).first;
			AddNewListItem(task_id,m_pAcceptedList);
		}
	}
}