#ifndef HEALTH_POINT_PROTOCOL_H
#define HEALTH_POINT_PROTOCOL_H

#include "UserData.h"

class HealthPointProtocol
{
public:
	//virtual void SetEnemyHpLable(float currentHp,float totalHp,bool isAnim,unsigned int targetMonster,const char* targetName,unsigned int nameType,float dt = 0.2f) = 0;
	virtual void SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType) = 0;

	virtual void updateHP(float dt) = 0;

	virtual void setActive(bool isActive) = 0;

};

#endif