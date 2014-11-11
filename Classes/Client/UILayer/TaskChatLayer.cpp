#include "AppDelegate.h"
#include "TaskChatLayer.h"
#include "UIManager.h"
#include "GameResourceManager.h"
#include "TaskManager.h"
#include "BoneNPCManager.h"
#include "GameConfigFromLuaManager.h"
#include "TaskConfigFromLuaManager.h"
#include "Localization.h"
#include "MainMenuLayer.h"
#include "BackPackLayer.h"
#include "WaitingLayerManager.h"
#include "TutorialsManager.h"
#include "LevelManager.h"
#include "InstanceManager.h"
#include "LuaTinkerManager.h"
#include "CallNativeFuncManager.h"
#include "UserData.h"
#include "GuildManager.h"

using namespace TXGUI;

TaskChatLayer::TaskChatLayer()
{
	m_leftHeroLogoPic = 0;
	m_taskNameLabel = 0;
	m_npcTaskTalkLabel = 0;

	m_taskRewardContainer = 0;
	for (int index = 0;index<3;index++)
	{
		m_taskContainerItem[index] = 0;
	}

	m_bStartAcceptTaskWork = false;
	m_bStartAcceptRewardWork = false;
	m_bStartRunningTaskWork = false;

	mCurTaskId = 0;
}

TaskChatLayer::~TaskChatLayer()
{
	UIManager::sharedManager()->RemoveUILayout("TaskChatPanel");
}

bool TaskChatLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();

	taskChatLayout = manager->CreateUILayoutFromFile("UIplist/taskChat.plist", this, "TaskChatPanel");

	m_taskChatContainer = taskChatLayout->FindChildObjectByName<UIContainer>("taskChatContainer");

	manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::closeBtnClick",
		menu_selector(TaskChatLayer::closeBtnClick), this);
	manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::OnTaskItemClickEvent_01",
		menu_selector(TaskChatLayer::OnTaskItemClickEvent_01), this);
	manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::OnTaskItemClickEvent_02",
		menu_selector(TaskChatLayer::OnTaskItemClickEvent_02), this);
	manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::OnTaskItemClickEvent_03",
		menu_selector(TaskChatLayer::OnTaskItemClickEvent_03), this);
    manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::OnTaskshopExchangeBtnClick",
                                 menu_selector(TaskChatLayer::OnTaskshopExchangeBtnClick), this);
    manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::OnTaskCityDefendBtnClick",
        menu_selector(TaskChatLayer::OnTaskCityDefendBtnClick), this);
    manager->RegisterMenuHandler("TaskChatPanel","TaskChatLayer::OnTaskItemExchangeBtnClick",
        menu_selector(TaskChatLayer::OnTaskItemExchangeBtnClick), this);

	m_leftHeroLogoPic = taskChatLayout->FindChildObjectByName<UIPicture>("npcLogoIcon");
	m_taskNameLabel = taskChatLayout->FindChildObjectByName<UILabel>("TaskNameLable");
	m_npcTaskTalkLabel = taskChatLayout->FindChildObjectByName<UILabel>("NpcTaskTalkLabel");

	// Note: find container
	m_taskRewardContainer = taskChatLayout->FindChildObjectByName<UIContainer>("RewardContainer");
	m_taskContainerItem[0] = taskChatLayout->FindChildObjectByName<UIContainer>("taskItemContainer_01");
	m_taskContainerItem[1] = taskChatLayout->FindChildObjectByName<UIContainer>("taskItemContainer_02");
	m_taskContainerItem[2] = taskChatLayout->FindChildObjectByName<UIContainer>("taskItemContainer_03");

	m_rewardMoneyLabel = taskChatLayout->FindChildObjectByName<UILabel>("TaskRewardMoneyLabel");
	m_rewardExpLabel = taskChatLayout->FindChildObjectByName<UILabel>("TaskRewardExpLabel");

	//setTouchEnabled(true);
	return true;
}

void TaskChatLayer::closeLayerCallBack(void)
{
	this->setVisible(false);
	m_bStartAcceptTaskWork = false;
	m_bStartAcceptRewardWork = false;
	m_bStartRunningTaskWork = false;
}

void TaskChatLayer::closeBtnClick(CCObject* sender)
{
	this->setVisible(false);
	m_bStartAcceptTaskWork = false;
	m_bStartAcceptRewardWork = false;
	m_bStartRunningTaskWork = false;

	MainMenuLayer::RemoveOneLayer(this);
	TaskManager::getInstance()->setTaskState(E_TASK_STATE_UNKNOW);
}

bool TaskChatLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("TaskChatLayer::ccTouchBegan");
	return true;
}

void TaskChatLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void TaskChatLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void TaskChatLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void TaskChatLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void TaskChatLayer::ShowTaskChatLayer(unsigned int npc_id)
{
	NPCData* pData = BoneNPCManager::getInstance()->GetOneNpcBasicData(npc_id);
	std::string leftRoleIcon = std::string("UI/") + pData->mTaskIntroImageIcon + std::string(".png");
	ShowLeftRightNpcIcon(leftRoleIcon);
	std::string npcName = Localizatioin::getLocalization((pData->mName).c_str());
	ShowTaskNameOrNpc(npcName);
	std::string npcMotto = Localizatioin::getLocalization((pData->mMotto).c_str());
	ShowTaskTalkContentOrMotto(npcMotto);
	ShowTaskList(npc_id);
	this->setVisible(true);
    
    if((int)pData->mType == 3 || (int)pData->mType == 1 || (int)pData->mType == 7)
        processExchangeShopFunc((int)pData->mType, true);
    else
        processExchangeShopFunc((int)pData->mType, false);

    // item exchange
    if((int)pData->mType == 8)
    {
        processItemExchangeFunc(true);
    }
    else
    {
        processItemExchangeFunc(false);
    }

    // guild
    if((int)pData->mType == 10 || (int)pData->mType == 11 || (int)pData->mType == 12 || (int)pData->mType == 13)
    {
        processGuildChat((int)pData->mType, true);
    }
    else
    {
        processGuildChat((int)pData->mType, false);
    }

	MainMenuLayer::PushOneLayer(this);
}

void TaskChatLayer::ShowLeftRightNpcIcon(std::string iconName)
{
	if (m_leftHeroLogoPic != 0)
	{
		std::string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(iconName.c_str());
		CCSprite *npcSprite = CCSprite::create();
		npcSprite->initWithFile(path.c_str());
		CCPoint lastPt = CCPointZero;
		float widthOffset = 0;
		float heightOffset = 0;
		if(m_leftHeroLogoPic->getCurrentNode())
		{
			CCPoint anchorPoint = m_leftHeroLogoPic->getCurrentNode()->getAnchorPoint();
			widthOffset -= anchorPoint.x * m_leftHeroLogoPic->getSpriteSize().width;
			heightOffset -= anchorPoint.y * m_leftHeroLogoPic->getSpriteSize().height;
			lastPt = m_leftHeroLogoPic->getCurrentNode()->getPosition();
		}
		else
		{
			widthOffset -= 0.5f * m_leftHeroLogoPic->getSpriteSize().width;
			heightOffset -= 0.5f * m_leftHeroLogoPic->getSpriteSize().height;
			lastPt = m_leftHeroLogoPic->getSpritePos();
		}
		npcSprite->setAnchorPoint(ccp(0,0));
        std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();

        // client version
        if (strcmp(clientVersion.c_str(), "hq") == 0)
        {
            npcSprite->setScale((1.0f / HQ_NPC_UI));
        } else if (strcmp(clientVersion.c_str(), "nq") == 0)
        {
            npcSprite->setScale((1.0f / HQ_NPC_UI));
        }
    
        m_leftHeroLogoPic->setMySprite(npcSprite);

		npcSprite->setPosition(ccp(lastPt.x+widthOffset,lastPt.y+heightOffset));
	}
}

void TaskChatLayer::ShowTaskNameOrNpc(std::string task_name)
{
	if (m_taskNameLabel != 0)
	{
		m_taskNameLabel->setString(task_name.c_str());
	}
}

void TaskChatLayer::ShowTaskTalkContentOrMotto(std::string task_cotent)
{
	if (m_npcTaskTalkLabel)
	{
		m_npcTaskTalkLabel->setString(task_cotent.c_str());
	}
}

void TaskChatLayer::ShowHeroTalkContent(std::string task_cotent)
{
}

void TaskChatLayer::ShowTaskList(unsigned int npc_id)
{
	HideAllTaskList();
	if (TaskManager::getInstance()->GetOneNpcTaskIdsByPriority(npc_id,mVecTaskIds))
	{
		int count = 0;
		for (std::vector<unsigned int>::iterator iter = mVecTaskIds.begin();
			 iter != mVecTaskIds.end(); iter++)
		{
			if (count >= 3)
			{
				break;
			}

			ShowOneTaskItem((*iter),count);

			count ++;
		}
	}	
}

void TaskChatLayer::ShowRewardContent(unsigned int icon_reward,unsigned int exp_reward)
{

}

void TaskChatLayer::HideAllTaskList()
{
	for (int index = 0;index<3;index++)
	{
		ShowOneTaskItemOrHideOnly(index,false);
	}
	m_taskRewardContainer->getCurrentNode()->setVisible(false);
}

void TaskChatLayer::ShowOneTaskItem(unsigned int task_id,int index)
{
	eTaskState task_state = TaskManager::getInstance()->GetTaskState(task_id);
	std::string content_id(TaskConfigFromLuaManager::getInstance()->GetOneTaskName(task_id));
	std::string content(Localizatioin::getLocalization((content_id).c_str()));	
	ShowTaskItemWithFlagStateAndName(index,task_state,content.c_str());
	ShowOneTaskItemOrHideOnly(index,true);
}

void TaskChatLayer::ShowTaskItemWithFlagStateAndName(int itemIndex,eTaskState taskState,std::string content)
{
	UIPicture* newTaskIcon = 0;
	UIPicture* runingTaskIcon = 0;
	UIPicture* waitRewardIcon = 0;
	UILabel* taskNameLabel = 0;

	std::string newTaskIconName = "";
	std::string runingTaskIconName = "";
	std::string waitRewardIconName = "";
	std::string taskNameIconName = "";

	switch (itemIndex)
	{
	case 0:
		{
			newTaskIconName = "TaskNewFlagPic_01";
			runingTaskIconName = "TaskRuningFlagPic_01";
			waitRewardIconName = "TaskCompleteFlagPic_01";
			taskNameIconName = "HeroTaskTalkLabel_01";
		}
		break;
	case 1:
		{
			newTaskIconName = "TaskNewFlagPic_02";
			runingTaskIconName = "TaskRuningFlagPic_02";
			waitRewardIconName = "TaskCompleteFlagPic_02";
			taskNameIconName = "HeroTaskTalkLabel_02";
		}
		break;
	case 2:
		{
			newTaskIconName = "TaskNewFlagPic_03";
			runingTaskIconName = "TaskRuningFlagPic_03";
			waitRewardIconName = "TaskCompleteFlagPic_03";
			taskNameIconName = "HeroTaskTalkLabel_03";
		}
		break;
	}

	newTaskIcon = taskChatLayout->FindChildObjectByName<UIPicture>(newTaskIconName.c_str());
	runingTaskIcon = taskChatLayout->FindChildObjectByName<UIPicture>(runingTaskIconName.c_str());
	waitRewardIcon = taskChatLayout->FindChildObjectByName<UIPicture>(waitRewardIconName.c_str());
	taskNameLabel = taskChatLayout->FindChildObjectByName<UILabel>(taskNameIconName.c_str());

	newTaskIcon->getCurrentNode()->setVisible(false);
	runingTaskIcon->getCurrentNode()->setVisible(false);
	waitRewardIcon->getCurrentNode()->setVisible(false);

	bool bUseSpecialColor = false;

	switch(taskState)
	{
	case kType_TASK_NOT_ACCEPT:
		{
			newTaskIcon->getCurrentNode()->setVisible(true);
			bUseSpecialColor = true;
		}
		break;
	case kType_TASK_RUNNING:
		{
			runingTaskIcon->getCurrentNode()->setVisible(true);
		}
		break;
	case kType_TASK_WAIT_REWARD:
		{
			waitRewardIcon->getCurrentNode()->setVisible(true);
			bUseSpecialColor = true;
		}
		break;
	}

	taskNameLabel->setString(content.c_str());
	if (bUseSpecialColor)
	{
		taskNameLabel->setColor(ccc3(238,166,26));
	}	
	else
	{
		taskNameLabel->setColor(ccc3(255,255,255));
	}
}

void TaskChatLayer::OnTaskItemClickEvent_01(CCObject* sender)
{
	//unsigned int task_id = mVecTaskIds[0];

	OnOneTaskItemClick(0);

	if (m_bStartAcceptTaskWork)
	{
		TrackAcceptTaskWork(false,mCurTaskId);
	}
	else if (m_bStartAcceptRewardWork)
	{
		TrackAcceptRewardWork(false,mCurTaskId);
	}
	else if (m_bStartRunningTaskWork)
	{
		TrackAcceptRunningWork(false,mCurTaskId);
	}
}

void TaskChatLayer::OnTaskItemClickEvent_02(CCObject* sender)
{
	//unsigned int task_id = mVecTaskIds[1];

	OnOneTaskItemClick(1);

	if (m_bStartAcceptTaskWork)
	{
		TrackAcceptTaskWork(false,mCurTaskId);
	}
	else if (m_bStartAcceptRewardWork)
	{
		TrackAcceptRewardWork(false,mCurTaskId);
	}
	else if (m_bStartRunningTaskWork)
	{
		TrackAcceptRunningWork(false,mCurTaskId);
	}
}

void TaskChatLayer::ShowTaskReward(unsigned int money,unsigned int exp)
{
	m_taskRewardContainer->getCurrentNode()->setVisible(true);
	char buffer[100];
	sprintf(buffer,"%d",money);
	m_rewardMoneyLabel->setString(buffer);
	memset(buffer,0,sizeof(buffer)/sizeof(char));
	sprintf(buffer,"%d",exp);
	m_rewardExpLabel->setString(buffer);
}

void TaskChatLayer::OnTaskItemClickEvent_03(CCObject* sender)
{
	//unsigned int task_id = mVecTaskIds[2];

	OnOneTaskItemClick(2);

	if (m_bStartAcceptTaskWork)
	{
		TrackAcceptTaskWork(false,mCurTaskId);
	}
	else if (m_bStartAcceptRewardWork)
	{
		TrackAcceptRewardWork(false,mCurTaskId);
	}
	else if (m_bStartRunningTaskWork)
	{
		TrackAcceptRunningWork(false,mCurTaskId);
	}
}

void TaskChatLayer::OnOneTaskItemClick(unsigned int index)
{
	HideAllTaskList();

	if (m_bStartAcceptRewardWork || m_bStartAcceptTaskWork || m_bStartRunningTaskWork)
	{
		return ;
	}

	mCurTaskId = mVecTaskIds[index];

	// Note: 任务名称
	std::string taskFlagName = TaskConfigFromLuaManager::getInstance()->GetOneTaskName(mCurTaskId);
	std::string taskName = Localizatioin::getLocalization((taskFlagName).c_str());
	ShowTaskNameOrNpc(taskName);

	eTaskState task_state = TaskManager::getInstance()->GetTaskState(mCurTaskId);
	switch(task_state)
	{
	case kType_TASK_NOT_ACCEPT:
		{
			// Note: 显示金币奖励
			unsigned int moneyNum = 0;
			TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardMoney(mCurTaskId,moneyNum);

			unsigned int expNum = 0;
			TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardExp(mCurTaskId,expNum);

			ShowTaskReward(moneyNum,expNum);

			m_bStartAcceptTaskWork = true;
			TrackAcceptTaskWork(true,mCurTaskId);
		}
		break;
	case kType_TASK_WAIT_REWARD:
		{
			// Note: 显示金币奖励
			unsigned int moneyNum = 0;
			TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardMoney(mCurTaskId,moneyNum);

			unsigned int expNum = 0;
			TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardExp(mCurTaskId,expNum);

			ShowTaskReward(moneyNum,expNum);

			m_bStartAcceptRewardWork = true;
			TrackAcceptRewardWork(true,mCurTaskId);
		}
		break;
	case kType_TASK_RUNNING:
		{
			m_bStartRunningTaskWork = true;
			TrackAcceptRunningWork(true,mCurTaskId);
		}
		break;
	}

	// Note: 根据TaskId判断是否会有剧情
	if (task_state == kType_TASK_NOT_ACCEPT)
	{
		if (false == TutorialsManager::Get()->IsInTutorial())
		{
			//char buffer[100];
			//sprintf(buffer,"TaskAnswerBtn_0%d",index+1);
			TutorialsManager::Get()->UpdateByTaskState(mCurTaskId,task_state,"TaskAnswerBtn_01");
		}
	}	
}

void TaskChatLayer::OnTaskshopExchangeBtnClick(CCObject* sender)
{
	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER))
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "OnPressPointShopBtn");
		return;
	}

    CCLOG("OnTaskshopExchangeBtnClick\n");
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
        if(mShopType == 3)
           layer->OnNPCBagClicked();
        else if(mShopType == 1)
            layer->OnNPCBagMagicShopClicked();
	}
}
 
void TaskChatLayer::OnTaskCityDefendBtnClick(CCObject* sender)
{
    int userLv = UserData::GetUserInfo().level;
    int CanOpenLv = LuaTinkerManager::Get()->callLuaFunc<int>("Script/Activity/dailyActivityLayerDataCenter.lua", "getCityDefendOpenLv"); 
    if(userLv < CanOpenLv)
    {
        CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("M_CITYDEFEND_OPENLEVEL"), CanOpenLv);
        ShowMessage(str->getCString());
        return;
    }

    closeBtnClick(NULL);
    int tag = MainMenuLayer::getLayerTagByName("CityDefendRoomListLayer");
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/MainMenuUILayer.lua", "getLuaLayerByTag", tag);    
}
void TaskChatLayer::OnTaskItemExchangeBtnClick(CCObject* sender)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (!mLayer)
    {
       return;
    }

    if(mShopType == 10)
    {
         GuildManager::Get()->setIsGuildInfoNeedOpen(false);
        mLayer->ShowGuildShopLayer(true);
    }
    else if(mShopType == 11)
    {
        mLayer->ShowGuildBlessLayer(true);
    }
    else if(mShopType == 12)
    {
        mLayer->ShowGuildInstanceListLayer(true);
    }
    else if(mShopType == 13)
    {
        GuildManager::Get()->setIsRankNeedOpen(true);
        GuildManager::Get()->setIsGuildInfoNeedOpen(false);
        mLayer->ShowGuildListLayer(true);
    }
    else if(mShopType == 8)
    {
        int userLv = UserData::GetUserInfo().level;
        int CanOpenLv = LuaTinkerManager::Get()->callLuaFunc<int>("Script/Activity/dailyActivityLayerDataCenter.lua", "getCityDefendOpenLv"); 
        if(userLv < CanOpenLv)
        {
            CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("M_CITYDEFEND_OPENLEVEL"), CanOpenLv);
            ShowMessage(str->getCString());
            return;
        }
        CallNativeFuncManager::getInstance()->ShowItemExchangeShopLayer(true);
    }

    closeBtnClick(NULL);
}

void TaskChatLayer::TrackAcceptTaskWork(bool bClear /* = false */,unsigned int task_id /* = -1 */)
{
	static int content_index = 1;
	
	if (bClear == true)
	{
		content_index = 1;
		return ;
	}

	// Note: 判断对话是否已经结束
	bool bTalkOver = TaskConfigFromLuaManager::getInstance()->TellIsRecieveTalkOver(task_id,content_index);
	if (bTalkOver)
	{
		this->setVisible(false);
		// send message to server one step
		TaskManager::getInstance()->SetOneTaskStepToServer(task_id,1,1);
		WaitingLayerManager::Get()->PushOneMessage("CSTaskDoStepReq");
		m_bStartAcceptTaskWork = false;
		return ;
	}

	// Note: 显示Npc对话的内容
	std::string npcTalkFlag = TaskConfigFromLuaManager::getInstance()->GetRecieveTextFromNpc(task_id,(content_index));
	std::string npcTalkContent = Localizatioin::getLocalization(npcTalkFlag.c_str());
	ShowTaskTalkContentOrMotto(npcTalkContent);

	// Note: 显示Hero对话内容
	ShowOneTaskItemOrHideOnly(0,true);
	std::string heroTalkFlag = TaskConfigFromLuaManager::getInstance()->GetRecieveTextFromHero(task_id,(content_index));
	std::string heroTalkContent = Localizatioin::getLocalization(heroTalkFlag.c_str());
	ShowTaskItemWithFlagStateAndName(0,kType_TASK_RUNNING,heroTalkContent);

	content_index ++;
}

void TaskChatLayer::TrackAcceptRewardWork(bool bClear /* = false */,unsigned int task_id /* = -1 */)
{
	static int content_index = 1;

	if (bClear == true)
	{
		content_index = 1;
		return ;
	}

	// Note: 判断对话是否已经结束
	bool bTalkOver = TaskConfigFromLuaManager::getInstance()->TellIsCompleteTalkOver(task_id,content_index);
	if (bTalkOver)
	{
		this->setVisible(false);
		// send message to server one step
		unsigned int total_steps = TaskConfigFromLuaManager::getInstance()->GetOneTaskStepNums(task_id);
		TaskManager::getInstance()->SetOneTaskStepToServer(task_id,total_steps,1);
		WaitingLayerManager::Get()->PushOneMessage("CSTaskDoStepReq");
		m_bStartAcceptRewardWork = false;
		return ;
	}

	// Note: 显示Npc对话的内容
	std::string npcTalkFlag = TaskConfigFromLuaManager::getInstance()->GetCompleteTextFromNpc(task_id,(content_index));
	std::string npcTalkContent = Localizatioin::getLocalization(npcTalkFlag.c_str());
	ShowTaskTalkContentOrMotto(npcTalkContent);

	// Note: 显示Hero对话内容
	ShowOneTaskItemOrHideOnly(0,true);
	std::string heroTalkFlag = TaskConfigFromLuaManager::getInstance()->GetCompleteTextFromHero(task_id,(content_index));
	std::string heroTalkContent = Localizatioin::getLocalization(heroTalkFlag.c_str());
	ShowTaskItemWithFlagStateAndName(0,kType_TASK_RUNNING,heroTalkContent);

	content_index ++;
}

void TaskChatLayer::TrackAcceptRunningWork(bool bClear /* = false */,unsigned int task_id /* = -1 */)
{
	static int content_index = 1;

	if (bClear == true)
	{
		content_index = 1;
		return ;
	}

	// Note: 判断对话是否已经结束
	if (content_index > 1)
	{
		this->setVisible(false);
		m_bStartRunningTaskWork = false;
		return ;
	}

	// Note: 显示Npc对话的内容
	std::string npcTalkFlag = TaskConfigFromLuaManager::getInstance()->GetCommonNpcTalkFlagString();
	std::string npcTalkContent = Localizatioin::getLocalization(npcTalkFlag.c_str());
	ShowTaskTalkContentOrMotto(npcTalkContent);

	// Note: 显示Hero对话内容
	ShowOneTaskItemOrHideOnly(0,true);
	std::string heroTalkFlag = TaskConfigFromLuaManager::getInstance()->GetCommonHeroTalkFlagString();
	std::string heroTalkContent = Localizatioin::getLocalization(heroTalkFlag.c_str());
	ShowTaskItemWithFlagStateAndName(0,kType_TASK_RUNNING,heroTalkContent);

	content_index ++;
}


void TaskChatLayer::processExchangeShopFunc(int type, bool visible)
{
    UIButton* shop_btn = taskChatLayout->FindChildObjectByName<UIButton>("shopExchangBtn");
    UILabel* shop_label = taskChatLayout->FindChildObjectByName<UILabel>("shopExchangText");
    shop_btn->setVisible(visible);
    shop_label->setVisible(visible);
    
    if(type ==1 ) //magic shop
        shop_label->setString(Localizatioin::getLocalization("M_SHOP_TITLE2"));
    else if(type == 3 || type == 7)
        shop_label->setString(Localizatioin::getLocalization("M_EXCHANGESHOP_TITLE"));
    mShopType = type;
}

void TaskChatLayer::processItemExchangeFunc(bool visible)
{
    UIButton* btnLeft = taskChatLayout->FindChildObjectByName<UIButton>("ItemExchangShopBtn");
    UILabel* labelLeft = taskChatLayout->FindChildObjectByName<UILabel>("ItemExchangText");
    btnLeft->setVisible(visible);
    labelLeft->setVisible(visible);
    labelLeft->setString(Localizatioin::getLocalization("M_EXCHANGESHOP_TITLE"));
    UIButton* btnRight = taskChatLayout->FindChildObjectByName<UIButton>("CityDefendBtn");
    UILabel* labelRight = taskChatLayout->FindChildObjectByName<UILabel>("CityDefendText");
    btnRight->setVisible(visible);
    labelRight->setVisible(visible);
    labelRight->setString(Localizatioin::getLocalization("M_CITYDEFEND_TITLE"));
    mShopType = 8;
}

void TaskChatLayer::processGuildChat(int type, bool visible)
{
    UIButton* btnLeft = taskChatLayout->FindChildObjectByName<UIButton>("ItemExchangShopBtn");
    UILabel* labelLeft = taskChatLayout->FindChildObjectByName<UILabel>("ItemExchangText");
    btnLeft->setVisible(visible);
    labelLeft->setVisible(visible);
    
    // type 10 11 12 13 分别代表公会商人、祈福、活动、排行
    if(type == 10)
    {
        labelLeft->setString(Localizatioin::getLocalization("G_SHOP_TITLE"));
    }
    else if(type == 11)
    {
        labelLeft->setString(Localizatioin::getLocalization("G_BENEDICTION_TITLE"));
    }
    else if(type == 12)
    {
        labelLeft->setString(Localizatioin::getLocalization("G_INSTANCE"));
    }
    else if(type == 13)
    {
        labelLeft->setString(Localizatioin::getLocalization("G_RANK"));
    }
    mShopType = type;
}