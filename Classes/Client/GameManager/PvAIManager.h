#pragma once

#include "Singleton_t.h"
#include "SpriteSeer.h"
#include "UILayout.h"
#include "UserData.h"

typedef struct AIPlayerInfo
{
	string name;
	int level;
	int rank;
	int type;
	PLAYER_ID id;
	int aiStatus;
    AIPlayerInfo& operator = (const AIPlayerInfo& playId);
}AIPlayerInfo;

typedef struct AILogInfo
{
	string name;
	bool direct;//true 主动，false被动
	bool win;
}AILogInfo;

typedef struct AILayerBasicInfo
{
	int remainCount;
	long coolDownTime;
	int buyCount;
}AILayerBasicInfo;

class PvAIManager : public TSingleton<PvAIManager>
{
public:
	PvAIManager();
	~PvAIManager();

	//网络请求中转
	void sendPvAIReq(int buttonIndex);

	void onMsgPvAIReward(int accuCoin, int accuReputation, int oneCoin, int oneReputation);

	//设置主角信息
	void setHeroRank(int rank){this->heroRank = rank;}
    int getHeroRank(){ return this->heroRank;}

	void setAIPlayerId(PLAYER_ID uid){this->aiPlayerId = uid;}
	PLAYER_ID getAIPlayerId(){return aiPlayerId;}

	void setAIPlayerVec(vector<AIPlayerInfo> infos){this->aiPlayerVec = infos;}
	void setAILogInfoVec(vector<AILogInfo> logs){this->aiLogInfoVec = logs;}

	void setRemainCount(int count){this->remainCount = count;}
	int getRemainCount(){return this->remainCount;}

	void setBuyCount(int count){this->buyCount = count;}
	int getBuyCount(){return this->buyCount;}

	void setAISkillsId(vector<int> equipSkills);

	void setAIAutoAttack(bool autoAttack);

	void onEventLocalHurt(CSPlayerHitPlayerReq attackReq);

	//设置cd结束时间，首次创建Timer时使用
	void setCoolDownEndTime(long endTime){this->coolDownEndTime = endTime;}
	long getCoolDownEndTime(){return this->coolDownEndTime;}

	//判断SpriteSeer是否为ai
	bool isAIPlayer(SpriteSeer * player){return player == aiPlayer;}
	SpriteSeer * getAIPlayer(){return aiPlayer;}

	//获得上一个ai player ，在ai死亡后使用
	SpriteSeer * getLastAIPlayer(){return lastAIPlayer;}

	//辅助函数
	void refreshPvAILayer();
	void Update(float dt);

	string strWithNum(string str, int number)
	{
		stringstream strStream;
		strStream << str << number;
		return strStream.str();
	}

	void resetData();
private:
	void moveToHero();

	int getBestSkill();

private:
	//主角相关信息
	int heroRank;

	int accuCoin;
	int accuReputation;
	int oneCoin;
	int oneReputation;

	SpriteSeer * aiPlayer;
	SpriteSeer * lastAIPlayer;

	//std::vector<int> skills;
	std::vector<int> attackSkillVec;
	std::vector<int> buffSkillVec;

	float aiAutoAttackCDTime;
	bool aiAutoAttack;
	PLAYER_ID aiPlayerId;

	vector<AIPlayerInfo> aiPlayerVec;
	vector<AILogInfo> aiLogInfoVec;

	TXGUI::UILayout * pvaiLayout;
	bool inited;
	long coolDownEndTime;
	bool inPvAIBattle;

	//基本信息提出
	int remainCount;
	int buyCount;
};