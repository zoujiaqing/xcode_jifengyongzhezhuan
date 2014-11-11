#pragma once
//#include "OnlineNetworkManager.h"
#include "map"
#include "set"
#include "string"
#include "cocos2d.h"
#include "Singleton_t.h"
#include "TXGUI.h"
#include "UILayout.h"

USING_NS_CC;

typedef struct OneContorlAppearData
{
	OneContorlAppearData(){
		nId = 0;
		isControlFixed = false;
		isControlHor = false;
		isFirstInit = false;
		nOrderIndex = 0;
		nTaskId = 0;
		nHeroLevel = 0;
		nTutorialId = 0;
	}
	unsigned int nId;		// 编号
	unsigned int nTaskId;
	unsigned int nHeroLevel;
	unsigned int nTutorialId;
	std::string szControlType;
	std::string szLayoutName;
	std::string szControlName;
	bool isControlFixed;
	bool isControlHor;
	bool isFirstInit;
	unsigned int nOrderIndex;

}ONECONTROLAPPEARDATA;

class UIControlAppearControler : public TSingleton<UIControlAppearControler>
{
public:
	struct OneTaskState
	{
		unsigned int nTaskId;
		bool bRevMessage;
		PB_TASK_STATUS_T taskStatue;
		OneTaskState(){
			nTaskId = 0;
			bRevMessage = false;
			//taskStatue = PB_TASK_CANT_TOUCH;
		}
	};
public:
	UIControlAppearControler();
	~UIControlAppearControler();

	void InitXMLData();
	void FirstHideAllControls();
	void InitPushAllAutoControls();

	void SendGetControlsState();
	void TackRevControlState(unsigned int id,bool bOnlyRecordData = false);
	void AddOneControllReqMessage(unsigned int id);

	void CheckButtonStateAtFirst();

	void TackOneTutorialEvent(std::string controlType,std::string controlName,std::string layoutName,bool bOnlyRecordData = false);
	void TackTaskOverEvent(unsigned int taskId);
	void TackHeroLevelEvent();

	void ShowAutoButtons(bool bShow);

	void SetConfigDataReGetOrNot(bool bGet);

	// Note: 和第一次检测按钮是否需要打开相关
	void SetTutorialMessageFromServerOrNot(bool bRev){
		bRevTutorialMessageFromServer = bRev;
	}
	void SetTaskMessageFromServerOrNot(bool bRev){
		bRevTaskMessageFromServer = bRev;
	}
	void ResetTaskState();
	void ReCheckUIControlOpenOrNot();
	void SendTaskStateQueMessage();
	void TackTaskStatus(unsigned int task_id,PB_TASK_STATUS_T status);

	void GetAllTutorialAttachedTaskId();
	void InsertOneTutorialAndTask(unsigned int tutorialId,unsigned int taskId);
protected:
	void InsertOneControlToAutoItems(std::string controlName,std::string layoutName,bool bHor,unsigned int pos);
	void ShowOneControl(unsigned int id);

	void ShowOneFixControl(TXGUI::UILayout* pLayout,std::string controlName,std::string controlType,bool bShow);
private:
	bool mIsLoadData;
	std::map<unsigned int ,OneContorlAppearData> mIdAndControlData;
	std::set<unsigned int> mUnLockItems;
	bool bSendData;

	std::map<unsigned int, OneTaskState> mIdAndTaskStateData;

	unsigned int nTotalRecControllMessageNums;
	unsigned int nRecTaskStateMessageNum;
	unsigned int nRecControlMessageNum;
	bool bRevTutorialMessageFromServer;
	bool bRevTaskMessageFromServer;
	bool bRevControlQueMessageFromServer;
	bool bReCheckButtonState;

	std::map<unsigned int,unsigned int> mTutorialIdTaskIdData;
};