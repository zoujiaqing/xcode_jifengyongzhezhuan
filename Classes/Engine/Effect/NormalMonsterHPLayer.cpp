#include "NormalMonsterHPLayer.h"
#include "LuaTinkerManager.h"
#include "Localization.h"
#include "SpriteMonster.h"
#include "SpriteMonsterMgr.h"

NormalMonsterHPLayer::NormalMonsterHPLayer()
{
	m_targetMonsterId = 0;
	b_isActive = false;
	m_container = NULL;
	m_nameLabel = NULL;
	m_levelLabel = NULL;
	m_hpPic = NULL;
	b_checkMonster = false;
}

NormalMonsterHPLayer::~NormalMonsterHPLayer()
{

}

bool NormalMonsterHPLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	bool success = false;

	UILayout* layout = UIManager::sharedManager()->getUILayout("BattleUI");
	if(layout != NULL)
	{
		m_container = layout->FindChildObjectByName<UIContainer>("NormalMonsterHpContainer");
		m_nameLabel = layout->FindChildObjectByName<UILabel>("normalMonsterNameText");
		m_levelLabel = layout->FindChildObjectByName<UILabel>("normalMonsterLevelText");
		
		m_hpPic = (CCSprite*)layout->FindChildObjectByName<UIPicture>("normalMonsterHpBar")->getCurrentNode();

		m_hpPic->setAnchorPoint(ccp(0,0.5f));
		CCPoint oldPos = m_hpPic->getPosition();
		float scale = UIManager::sharedManager()->getScaleFactor();
		m_hpPic->setPosition(ccp(oldPos.x-scale * m_hpPic->getContentSize().width/2,oldPos.y));

		m_container->setVisible(false);

		success = true;
	}

	return success;
}

void NormalMonsterHPLayer::setActive(bool isActive)
{
	if(isActive)
	{
		m_container->setVisible(true);
	}
	else
	{
		m_hpPic->stopAllActions();
		m_container->setVisible(false);
	}

	b_isActive = isActive;
}

void NormalMonsterHPLayer::updateHP(float dt)
{
	if(b_isActive)
	{
		if(m_targetMonsterId != 0 && b_checkMonster)
		{
			if(!SpriteMonsterMgr::Get()->GetMonster(m_targetMonsterId))
			{
				setActive(false);
				b_checkMonster = false;
			}
		}
	}
}

void NormalMonsterHPLayer::SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType)
{
	SpriteMonster* monster = SpriteMonsterMgr::Get()->GetMonster(enemyId.getID());
	if(monster == NULL)
	{
		return;
	}

	float totalHp = monster->getTotalBlood();
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
			CCActionInterval* actionScale = CCScaleTo::create(0.2f,percent,scale);
			m_hpPic->runAction(actionScale);
		}
		else
		{
			m_hpPic->setScaleX(percent);
		}
	}

	if(m_targetMonsterId != enemyId.getID())
	{
		const char* mosterNameIndex = LuaTinkerManager::Get()->getLuaConfig<const char *>("MonsterBasicInfomation", "MonsterBasicInfomation", monsterType, "Name");
		CCAssert(mosterNameIndex,"monster name index is null");
		const char* monsterName = Localizatioin::getLocalization(mosterNameIndex);
		CCAssert(monsterName,"monster name is null");
		m_nameLabel->setString(monsterName);

		char levelStr[20];
		sprintf(levelStr,"Lv%d",monster->getLevel());
		m_levelLabel->setString(levelStr);
	}

	b_checkMonster = true;
	m_targetMonsterId = enemyId.getID();
}