#ifndef BOSS_HP_LAYER_H
#define BOSS_HP_LAYER_H

#include "HealthPointProtocol.h"
#include "TXGUIHeader.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace TXGUI;

class BossHPLayer : public CCLayer,
					public HealthPointProtocol
{
public:
	BossHPLayer();
	virtual ~BossHPLayer();
	CREATE_FUNC(BossHPLayer);

	virtual bool init();

	virtual void SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType = 0);

	virtual void updateHP(float dt);

	virtual void setActive(bool isActive);

	void onHealthPointActionOver(CCNode* node);
private:
	virtual void setHpWithoutAnim(float percent,int hpCount);
	virtual void startHpAnim(float targetPercent,int targetHPCount);

private:
	bool b_isAnimFlag;
	bool b_isActive;
	unsigned int m_targetMonsterId;
	UIContainer* m_container;
	UILabel* m_nameLabel;
	UILabel* m_levelLabel;
	UILabel* m_magnificationLabel;
	//CCSprite* m_hpPic;
	std::vector<CCSprite*> m_hpPicList;

	int m_targetHPLine;
	float m_targetHpPercent;
	int m_currentHPLine;
	float m_currentHPPercent;
};

#endif