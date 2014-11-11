#pragma once

#include "map"
#include "string"
#include "cocos2d.h"

USING_NS_CC;

enum ETutorialType
{
	kTutorialUI = 1,
	kTutorialPopupReward = 2,
	kTutorialTask = 3,
	kTutorialLeadTask = 4,
	kTutorialUnKnow
};

typedef struct OneTutorialData
{
	OneTutorialData(){
		nId = 0;
		nType = 0;
		nNextId = 0;
		nTipType = 0;
		nTipType_2 = 0;
		nPos = 0;
		nPos_2 = 0;
		nUIType = 0;
		nUIType_2 = 0;
		nTaskId = 0;
		nWhen = 0;
		nEventType = 0;
		IsTaskHeader = false;
		IsNewWnd = false;
		isExpansionMainMenu = false;
		IsAllowInEmptyArea = false;
		IsCareDstPos = true;
	}
	unsigned int nId;		// 编号
	unsigned int nType;
	unsigned int nNextId;
	unsigned int nPos;
	unsigned int nPos_2;
	unsigned int nUIType;
	unsigned int nUIType_2;
	unsigned int nEventType;
	unsigned int nTaskId;
	unsigned int nWhen;
	std::string szUiLayoutName;
	std::string szUiDstLayoutName;
	std::string szControlType;
	std::string szControlName;
	unsigned int nTipType;
	std::string szControlContent;
	CCPoint relativePoint;
	std::string szControlType_2;
	std::string szControlName_2;
	unsigned int nTipType_2;
	std::string szControlContent_2;
	CCPoint relativePoint_2;
	bool isExpansionMainMenu;
	bool IsTaskHeader;
	bool IsNewWnd;
	bool IsAllowInEmptyArea;
	bool IsCareDstPos;

}ONETUTORIALDATA;

class TutorialDataCenter
{
public:
	TutorialDataCenter();
	~TutorialDataCenter();

	void InitXMLData();
	
	/**
	* Instruction : 获取教程类型
	* @param 
	*/	
	unsigned int GetOneTutorialType();
	std::string GetOneTutorialLayoutName();
	std::string GetOneTutorialDestLayoutName();
	std::string GetOneTutorialControlName(int id);
	unsigned int GetOneTutorialControlType(int id);
	unsigned int GetOneTutorialTipType(int id);
	CCPoint GetOneTutorialRelativePt(int id);
	unsigned int GetNextTutorialID();
	const char* GetOneTutorialContent(int id);
	bool GetOneTutorialIsHasTaskHead(unsigned int id,bool &bError);
	bool GetOneTutorialIsExpandMainMenu();
	void GetTutorialIds(std::vector< int> &vecOut);
	bool GetOneTutorialIsNewWnd();
	int GetOneTutorialUIType(int id);
	unsigned int GetOneTutorialPos(int id);
	unsigned int GetOneTutorialEventType();
	unsigned int GetTutorialIdByLeadinstanceIdAndWhen(unsigned int taskId,unsigned int when);
	bool GetOneTutorialIsAllowInEmptyArea();
	bool GetOneTutorialIsCareDstPos();

	void SetCurIterator(unsigned int id);

	static int GetUITypeByString(std::string type);

	bool IsOneTaskHasTutorial(unsigned int taskId);

	const std::map<unsigned int ,OneTutorialData>& GetData()
	{
		return mIdAndTutorialData;
	}
private:
	bool mIsLoadData;
	std::map<unsigned int ,OneTutorialData> mIdAndTutorialData;
	std::map<unsigned int ,OneTutorialData>::iterator mCurIter;
};