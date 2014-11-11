#pragma once

#include "ASprite.h"
#include "UILayout.h"
#include "UIContainer.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UIPicture.h"
#include "TaskData.h"
#include "vector"
#include "UIScrollList.h"
#include "UIButtonToggle.h"
using namespace std;
using namespace cocos2d;

class TaskListLayer : public cocos2d::CCLayer
{
public:
	TaskListLayer();
	virtual ~TaskListLayer();

	CREATE_FUNC(TaskListLayer);
	virtual bool init();

	void closeBtnClick(CCObject* sender);
	void ShowTaskListLayer(bool bShow);
    
    void tabAcceptedTaskBtnClick(CCObject* sender);
    void tabAcceptableTaskBtnClick(CCObject* sender);

	void AddNewListItem(unsigned int task_id,UIScrollList* pList);
	CCSprite* GetSpriteFromSystemIconByName(const char *rcs_name);

	void ListItemClickEvent(CCObject* sender);

	virtual void closeLayerCallBack(void);

	/**
	* Instruction : 显示已接任务列表
	* @param 
	*/
	void ShowAccptedTaskList();
	/**
	* Instruction : 显示可接任务列表
	* @param 
	*/
	void ShowAcceptableTaskList();
	/**
	* Instruction : 插入ListItems
	* @param 
	*/
	void InitListWithElements();
protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
	TXGUI::UILayout *taskListLayout;
	UIScrollList* m_pAcceptedList;
	UIScrollList* m_pAcceptableList;

	UIButtonToggle* m_acceptedTaskToggle;
	UIButtonToggle* m_acceptableTaskToggle;
};