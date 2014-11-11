#pragma once

#include "ASprite.h"
#include "UILayout.h"
#include "UIContainer.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UIPicture.h"
#include "TaskData.h"
#include "vector"
using namespace std;
using namespace cocos2d;

class TaskChatLayer : public cocos2d::CCLayer
{
public:
	TaskChatLayer();
	virtual ~TaskChatLayer();

	CREATE_FUNC(TaskChatLayer);
	virtual bool init();

	void closeBtnClick(CCObject* sender);
	void ShowTaskChatLayer(unsigned int npc_id);
	void closeLayerCallBack(void);
protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	/**
	* Instruction : 显示左侧头像
	* @param 
	*/	
	void ShowLeftRightNpcIcon(std::string iconName);
	/**
	* Instruction : 显示任务的名字
	* @param 
	*/	
	void ShowTaskNameOrNpc(std::string task_name);
	/**
	* Instruction : 显示Npc说话的内容（座右铭+对话内容）
	* @param 
	*/	
	void ShowTaskTalkContentOrMotto(std::string task_cotent);
	/**
	* Instruction : 玩家对话的内容
	* @param 
	*/	
	void ShowHeroTalkContent(std::string task_cotent);
	/**
	* Instruction : 显示任务列表
	* @param 
	*/
	void ShowTaskList(unsigned int npc_id);
	/**
	* Instruction : 
	* @param 
	*/
	void HideAllTaskList();
	/**
	* Instruction : 显示一条任务信息
	* @param index 0-2 代表 1-3 task item
	*/
	void ShowOneTaskItem(unsigned int task_id,int index);
	/**
	* Instruction : 显示奖励内容
	* @param 
	*/	
	void ShowRewardContent(unsigned int icon_reward,unsigned int exp_reward);
	/**
	* Instruction : 显示任务状态 未接 进行中 领取奖励
	* @param 
	*/
	void ShowTaskItemWithFlagStateAndName(int itemIndex,eTaskState taskState,std::string content);

	/**
	* Instruction : 设置任务的奖励
	* @param 
	*/
	void ShowTaskReward(unsigned int money,unsigned int exp);

	// Note: 任务Item点击后的回调函数
	void OnTaskItemClickEvent_01(CCObject* sender);
	void OnTaskItemClickEvent_02(CCObject* sender);
	void OnTaskItemClickEvent_03(CCObject* sender);
    void OnTaskshopExchangeBtnClick(CCObject* sender);
	void OnOneTaskItemClick(unsigned int index);
    void OnTaskCityDefendBtnClick(CCObject* sender);
    void OnTaskItemExchangeBtnClick(CCObject* sender);

	// Note: 开始对话任务
	/**
	* Instruction : 接受任务时的对话
	* @param 
	*/	
	void TrackAcceptTaskWork(bool bClear = false,unsigned int task_id = 0);
	/**
	* Instruction : 完成任务时的对话
	* @param 
	*/
	void TrackAcceptRewardWork(bool bClear = false,unsigned int task_id = 0);
	/**
	* Instruction : 进行中任务时的对话
	* @param 
	*/
	void TrackAcceptRunningWork(bool bClear = false,unsigned int task_id = 0);

	/**
	* Instruction : 仅仅显示ListItem
	* @param 
	*/
	void ShowOneTaskItemOrHideOnly(int index,bool bShow){
		m_taskContainerItem[index]->getCurrentNode()->setVisible(bShow);
	}
    
    void processExchangeShopFunc(int type, bool visible);
    void processItemExchangeFunc(bool visible);
    void processGuildChat(int type, bool visible);
private:
	std::vector<unsigned int> mVecTaskIds;

	TXGUI::UILayout *taskChatLayout;
	TXGUI::UIContainer* m_taskChatContainer;
	TXGUI::UIContainer* m_taskRewardContainer;
	TXGUI::UIPicture* m_leftHeroLogoPic;
	TXGUI::UILabel* m_taskNameLabel;
	TXGUI::UILabel* m_npcTaskTalkLabel;

	// Note: 3个 task item 相关的容器
	TXGUI::UIContainer* m_taskContainerItem[3];

	// Note: 奖励相关
	TXGUI::UILabel* m_rewardMoneyLabel;
	TXGUI::UILabel* m_rewardExpLabel;

	// Note: 任务对话相关
	bool m_bStartAcceptTaskWork;
	bool m_bStartAcceptRewardWork;
	bool m_bStartRunningTaskWork;

	unsigned int mCurTaskId;
    
    int mShopType;
};