#ifndef NORMAL_MONSTER_HP_LAYER_H
#define NORMAL_MONSTER_HP_LAYER_H

#include "HealthPointProtocol.h"
#include "cocos2d.h"
#include "TXGUIHeader.h"

using namespace cocos2d;
using namespace TXGUI;

class NormalMonsterHPLayer : public CCLayer,
							public HealthPointProtocol
{
public:
	NormalMonsterHPLayer();
	virtual ~NormalMonsterHPLayer();
	CREATE_FUNC(NormalMonsterHPLayer);

	virtual bool init();

	virtual void SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType = 0);

	virtual void updateHP(float dt);

	virtual void setActive(bool isActive);

protected:

	bool b_isActive;
	unsigned int m_targetMonsterId;
	UIContainer* m_container;
	UILabel* m_nameLabel;
	UILabel* m_levelLabel;
	CCSprite* m_hpPic;
	bool b_checkMonster;
};

#endif