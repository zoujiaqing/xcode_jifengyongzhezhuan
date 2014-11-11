#include "PlayerHPLayer.h"
#include "LevelLayer.h"
#include "LevelManager.h"
#include "UserData.h"
#include "LuaTinkerManager.h"
#include "Localization.h"

PlayerHPLayer::PlayerHPLayer()
{

}

PlayerHPLayer::~PlayerHPLayer()
{

}

void PlayerHPLayer::updateHP(float dt)
{
	if(b_isActive)
	{
		if(m_targetMonsterId != 0 && b_checkMonster)
		{
			LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
			if (layer)
			{
				std::map<PLAYER_ID, OthersPlayerInfo>::iterator iter = layer->m_othersPlayerInfo.find(m_targetPlayerId);
				if(iter == layer->m_othersPlayerInfo.end())
				{
					setActive(false);
					b_checkMonster = false;
				}
			}
		}
	}
}

void PlayerHPLayer::SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType)
{
	
	USERINFO* info = UserData::Get()->GetUserInfo(enemyId);

	CCAssert(info != NULL,"player id is error");

	float totalHp = info->total_hp;
	float scale = UIManager::sharedManager()->getScaleFactor();
	float percent= scale;
	if(totalHp != 0)
	{
		percent *= currentHp / totalHp;
	}
	if(m_hpPic)
	{
		m_hpPic->stopAllActions();
		if(isAnim)
		{
			float scale = UIManager::sharedManager()->getScaleFactor();
			CCActionInterval* actionScale = CCScaleTo::create(0.2f,percent,scale);
			m_hpPic->runAction(actionScale);
		}
		else
		{
			m_hpPic->setScaleX(percent);
		}
	}

	if(m_targetPlayerId != enemyId)
	{
		
		m_nameLabel->setString(info->szName);

		char levelStr[20];
		sprintf(levelStr,"Lv%d",info->level);
		m_levelLabel->setString(levelStr);
	}

	b_checkMonster = true;
	m_targetPlayerId = enemyId;
}