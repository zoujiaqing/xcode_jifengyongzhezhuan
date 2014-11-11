#pragma once

#include "Singleton_t.h"
#include "map"
#include "set"
#include "string"

typedef struct OnePopupRewardData
{
	OnePopupRewardData(){
		nId = 0;
		nRewardId = 0;
		szRewardContent = "";
		szRewardOkBtnContent = "";
		bShowCloseBtn = true;
		bModelLayer = false;
		bStartAtBegin = false;
		nTaskId = 0;
		nWhen = 0;
		nTutorialId = 0;
		nUnLockHeroLevel = 0;
	}
	unsigned int nId;		// ±àºÅ
	unsigned int nRewardId;
	unsigned int nUnLockHeroLevel;
	std::string szRewardContent;
	std::string szRewardOkBtnContent;
	bool bShowCloseBtn;
	bool bModelLayer;
	bool bStartAtBegin;
	unsigned int nTaskId;
	unsigned int nWhen;
	unsigned int nTutorialId;

}ONEPOPREWARDDATA;

class PopupRewardManager : public TSingleton<PopupRewardManager>
{
public:
	PopupRewardManager();
	virtual ~PopupRewardManager();

	void ResetValue();

	void InitXMLData();
	void ShowOneLayerByTaskState(unsigned int taskId,unsigned int when);
	void ShowOneLayerByHeroLevel(unsigned int heroLevel);
	void OnPopupRewardLayerClose();
	void PushOneItem(unsigned int id)
	{
		mUsedItems.insert(id);
	}
	bool IsOneItemUsed(unsigned int id)
	{
		std::set<unsigned int>::iterator iter = mUsedItems.find(id);
		if (iter != mUsedItems.end())
		{
			return true;
		}
		return false;
	}
	void SendReqPopupItemsState();
	void SetConfigDataReGetOrNot(bool bGet);
private:
	bool mIsLoadData;
	bool mIsStartAtBegin;
	unsigned int mTutorialId;
	std::map<unsigned int,OnePopupRewardData> mIdAndPopupRewardData;

	std::set<unsigned int> mUsedItems;
	bool bSendData;
};