#ifndef PLAYER_HP_LAYER_H
#define PLAYER_HP_LAYER_H

#include "NormalMonsterHPLayer.h"
#include "UserData.h"

class PlayerHPLayer : public NormalMonsterHPLayer
{
public:
	PlayerHPLayer();
	virtual ~PlayerHPLayer();
	CREATE_FUNC(PlayerHPLayer);

	virtual void SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType = 0);
	virtual void updateHP(float dt);
private:
	PLAYER_ID m_targetPlayerId;
};

#endif