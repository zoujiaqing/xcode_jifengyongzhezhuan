#pragma once
//游戏数据，设置管理
#include "Singleton_t.h"
#include "PhysicsLayer.h"

typedef struct TaskPosition{
	int cityId;
	int npcId;
	CCPoint position;
	TaskPosition(){
		npcId = -1;
	}
}TaskPosition;

class GameDataManager : public TSingleton<GameDataManager>
{
public:
	GameDataManager();
	virtual ~GameDataManager();

	//操作方式
	Move_Type getHeroMoveType(){return heroMoveType;}
	void setHeroMoveType(Move_Type type);

	//自动攻击状态
	bool getHeroAutoAttack(){return heroAutoAttack;}
	void setHeroAutoAttack(bool autoAttack);

	//任务系统跨地图寻路
	TaskPosition getTaskPosition(){return taskPos;}
	void setTaskPosition(TaskPosition taskPosition){this->taskPos = taskPosition;}

	void sendEliteTranspointValidReq();
	void checkEliteTranspointValid(unsigned int id);
	bool IsEliteTranspointValid(){return mIsOpenEliteTranspoint;}
	void resetEliteTranspointValid(){mIsOpenEliteTranspoint=false;}
	void TrackEliteValid();
private:
	Move_Type heroMoveType;
	bool heroAutoAttack;
	TaskPosition taskPos;
	bool mIsOpenEliteTranspoint;
};