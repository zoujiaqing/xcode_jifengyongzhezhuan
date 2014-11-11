#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "string"
#include "UILayout.h"
#include "TutorialBase.h"
#include "TutorialLayer.h"
#include "TutorialsDataCenter.h"
#include "UIButton.h"
#include "IconButton.h"

USING_NS_CC;
USING_NS_CC_EXT;

class UITutorial : public TutorialBase
{
public:
	enum
	{
		kType_Tutorail_UnKnow = 0,
		kType_Tutorial_Normal ,
		kType_Tutorial_Drag,
		kType_Tutorial_TaskLead,
	};

	enum
	{
		kType_Event_UnKnow = 0,
		kType_Event_Click ,
		kType_Event_DoubleClick,
		kType_Event_ContinueClick,
	};
public:
	UITutorial();
	UITutorial(unsigned int id,TutorialDataCenter* pDataCenter);
	virtual ~UITutorial();

	void Reset();

	void Start(const char * attachStartControlName = "");
	void End();
	void Update(float dt);

	// Note: 处理单击 双击 长摁事件
	bool HandleOneEvent(const char* name,unsigned int type);
	// Note: 处理用户拖拽事件
	bool HandleDragEventOver(int type = 0,int pos = 0,bool bInEmptyUI = false);
	// Note: 判断当前是否处于拖拽教程中
	bool IsInDragTutorial();
	// Note: 判断是否在教程中
	bool IsInTutorial();
protected:
	void HandleOneTutorialItem();
	void HandleOneTutorialItemWithDelay(unsigned int numFrames = 2);
	void HightLightControls();
	void HightLightOneControl(TXGUI::UIButton *pBtn,unsigned int id);
	void HightLightOneControl(TXGUI::IconButton *pBtn,unsigned int id);
    void HightLightOneControl(CCControlButton*, unsigned int id);
	void ResetOneControl(TXGUI::UIButton* pBtn);
	void ResetOneControl(TXGUI::IconButton* pBtn);
	void ResetAllControl();

	void HandleOneTutorialItemWithExpandMenuOver();
	void ShowFlagIconWithOneContorl(TXGUI::UIButton *pBtn,unsigned int id);
	void ShowFlagIconWithOneContorl(TXGUI::IconButton *pBtn,unsigned int id);
    void ShowFlagIconWithOneContorl(CCControlButton* pBtn, unsigned int id);

	void InitNextTutorial();
	void MoveNextTutorial();	

	bool IsLayoutVisiable(TXGUI::UILayout* pLayout);
	void ResetTutorialValue();

	void GetLayerControls(std::string name,int id,unsigned int type);
	/**
	* Instruction : 开始一个教学的时候 检测所有按钮的状态 
	* @param 
	*/	
	void FirstCheckAllReleaseControls(const char * attachStartControlName = "");

	void SetTutorailType(unsigned int type);
private:
	TutorialDataCenter* mDataCenter;
	TutorialLayer* mTutorialLayer;
	TXGUI::UILayout* mCurLayout;
	TXGUI::UILayout* mDstLayout;

	std::string mCurLayoutName;
	std::string mDstLayoutName;
	unsigned int mDelayFrames;
	unsigned int mRunningFrames;	
	bool mIsStartFrameCount;
	unsigned int mControlItemOldPripority;

	// Note: 当前教程类型 1 点击 2 拖拽 3 任务引导
	unsigned int mCurUITutorialType;

	// Note: 拖拽UI类型
	int mDragUIType;
	// Note: 拖拽的目标pos
	unsigned int mDragToUIPos;
	// Note: 1 单击 2 双击 3 长摁
	unsigned int mEventType;
	// Note: 当前按钮名称
	std::string mCurActiveControlName;

	// Note: 用于计算Flag图标的位置
	bool mIsStartCalControlPos;
	CCPoint mOldButtonPoint;
	CCPoint mOldButtonPoint_2;

	// Note: 当前控件
	TXGUI::UIButton* mCurActiveUIButton;
	TXGUI::IconButton* mCurActiveIconButton;
	TXGUI::UIButton* mCurActiveUIButton_2;
	TXGUI::IconButton* mCurActiveIconButton_2;
    CCControlButton* mCurActiveControlButton;
    CCControlButton* mCurActiveControlButton_2;

	// Note: 控件名称
	std::string mControlName;
	std::string mControlName_2;
	unsigned int mControlType;
	unsigned int mControlType_2;

	bool mIsNextTutorialNewWnd;
	bool mIsAllowInEmptyArea;
	bool mIsCareDstPos;

	enum ETutorialLayout {FIRTST_LAYOUT = 1,SECOND_LAYOUT = 2};
	enum EControlType {UIBUTTON_TYPE = 1,ICONBUTTON_TYPE = 2};	

	bool mIsEndWithSpecialControl;
};