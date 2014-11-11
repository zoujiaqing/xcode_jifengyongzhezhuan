#ifndef _SYSTEM_CONDITION_DATA_CENTER_H_
#define _SYSTEM_CONDITION_DATA_CENTER_H_

#include "string"
#include "map"

class SystemConditionDataCenter
{
private:
	struct SystemConditionData
	{		
		int id;
		std::string condtionName;		
		int leadTutorialId;
		int heroLevel;
		int taskId;
		int controlAppearConfigId;
		bool isSystemOpen;
		SystemConditionData(){
			id = -1;
			leadTutorialId = -1;
			heroLevel = -1;
			taskId = -1;
			controlAppearConfigId = -1;
			isSystemOpen = false;
		}
	};
public:
	SystemConditionDataCenter();
	~SystemConditionDataCenter();

	// Note: read data from xml file
	void Init();

	void SendCheckTaskStatuMessage();

	// Note: first check system open or not
	void CheckSystemsOpenOrNot();

	// Note: only check one system open or not
	bool CheckOneSystemOpenOrNot(unsigned int id);
	bool CheckOneSystemOpenOrNot(const char* name);

	void TackTaskStatus(unsigned int task_id,PB_TASK_STATUS_T status);

	int GetTaskStatus(unsigned int task_id);
protected:
	bool CheckOneSystemOpenOrNot(const std::map<unsigned int , SystemConditionData>::iterator &iter);
	bool CheckHeroLevelValidOrNot(unsigned int level);
	bool CheckTutorialIdValidOrNot(unsigned int level);
	bool CheckTaskIdValidOrNot(unsigned int level);
private:
	bool mIsLoadData;
	std::map<unsigned int , SystemConditionData> mData;
	// Note: Tell whether task done or not
	std::map<unsigned int , PB_TASK_STATUS_T> mTaskData;
};

#endif	//_SYSTEM_CONDITION_DATA_CENTER_H_