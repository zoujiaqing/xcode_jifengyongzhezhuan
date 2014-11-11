#include "BossHPLayer.h"
#include "LuaTinkerManager.h"
#include "Localization.h"
#include "SpriteMonster.h"
#include "SpriteMonsterMgr.h"

#define BOSS_HP_BAR_NUMBER 4

BossHPLayer::BossHPLayer()
{
	m_targetMonsterId = 0;
	b_isActive = false;
	m_container = NULL;
	m_nameLabel = NULL;
	m_levelLabel = NULL;
	m_magnificationLabel = NULL;
	b_isAnimFlag = false;
	m_targetHPLine = 0;
	m_targetHpPercent = 0;
	m_currentHPLine = 10000;
	m_currentHPPercent = 0;
	//m_hpPic = NULL;
}

BossHPLayer::~BossHPLayer()
{
	m_hpPicList.clear();

}

bool BossHPLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	bool success = false;

	UILayout* layout = UIManager::sharedManager()->getUILayout("BattleUI");
	if(layout != NULL)
	{
		m_container = layout->FindChildObjectByName<UIContainer>("BossHpContainer");
		m_nameLabel = layout->FindChildObjectByName<UILabel>("bossNameText");
		m_levelLabel = layout->FindChildObjectByName<UILabel>("bossLevelText");
		m_magnificationLabel = layout->FindChildObjectByName<UILabel>("bossMagnificationText");
		for(unsigned int i=1; i<= BOSS_HP_BAR_NUMBER;i++)
		{
			char spriteName[20];
			sprintf(spriteName,"bossHpBar%d",i);
			CCSprite* sprite = (CCSprite*)layout->FindChildObjectByName<UIPicture>(spriteName)->getCurrentNode();
			sprite->setAnchorPoint(ccp(0,0.5f));
			CCPoint oldPos = sprite->getPosition();
			float scale = UIManager::sharedManager()->getScaleFactor();
			sprite->setPosition(ccp(oldPos.x-scale * sprite->getContentSize().width/2,oldPos.y));
			sprite->setVisible(false);
			m_hpPicList.push_back(sprite);
		}

		m_container->setVisible(false);

		success = true;
	}

	return success;
}

void BossHPLayer::setActive(bool isActive)
{
	if(isActive)
	{
		m_container->setVisible(true);
	}
	else
	{
		//m_hpPic->stopAllActions();
		m_container->setVisible(false);
		//m_targetHPLine = 0;
		//m_targetHpPercent = 0;
		//m_currentHPPercent = 0;
		b_isAnimFlag = false;
	}

	b_isActive = isActive;
}

void BossHPLayer::updateHP(float dt)
{
	if(b_isActive)
	{
		if(!b_isAnimFlag)
		{
			if(m_targetHpPercent != m_currentHPPercent || m_targetHPLine != m_currentHPLine)
			{
				startHpAnim(m_targetHpPercent,m_targetHPLine);
			}
		}

		if(m_targetMonsterId != 0)
		{
			if(!SpriteMonsterMgr::Get()->GetMonster(m_targetMonsterId))
			{
				setActive(false);
			}
		}
	}
}

void BossHPLayer::setHpWithoutAnim(float percent,int hpCount)
{
	for(unsigned int i=0; i<= BOSS_HP_BAR_NUMBER - 1;i++)
	{
		m_hpPicList[i]->stopAllActions();
		m_hpPicList[i]->setVisible(false);
	}

	unsigned int currentIndex = hpCount % BOSS_HP_BAR_NUMBER;
	unsigned int nextIndex = (hpCount-1) % BOSS_HP_BAR_NUMBER;

	//float scale = UIManager::sharedManager()->getScaleFactor();

	// 最后一条血条没底框
	if(hpCount != 0)
	{
		m_hpPicList[nextIndex]->setVisible(true);
		m_hpPicList[nextIndex]->setScaleX(UIManager::sharedManager()->getScaleFactor());
		m_hpPicList[nextIndex]->setZOrder(2);
	}

	m_hpPicList[currentIndex]->setVisible(true);
	m_hpPicList[currentIndex]->setScaleX(percent);
	m_hpPicList[currentIndex]->setZOrder(3);

	m_currentHPLine = hpCount;
	m_currentHPPercent = percent;
	b_isAnimFlag = false;

	if(m_currentHPLine != 0)
	{
		char magStr[10];
		sprintf(magStr,"X %d",m_currentHPLine);
		m_magnificationLabel->setString(magStr);
	}
	else
	{
		m_magnificationLabel->setString("");
	}
	
}

void BossHPLayer::startHpAnim(float targetPercent,int targetHPCount)
{
	m_targetHpPercent = targetPercent;
	m_targetHPLine = targetHPCount;

	if(!b_isAnimFlag)
	{
		//CCAssert(m_currentHPLine >= m_targetHPLine,"target health point is larger");
		CCAssert((m_currentHPLine > m_targetHPLine) || ((m_currentHPLine == m_targetHPLine) && (m_currentHPPercent >= m_targetHpPercent)),"target health point is larger");
		if(m_currentHPLine == m_targetHPLine)
		{
			//同一段血条的递减
			unsigned int currentIndex = m_currentHPLine % BOSS_HP_BAR_NUMBER;
			float scale = UIManager::sharedManager()->getScaleFactor();
			CCActionInterval* actionScale = CCScaleTo::create(0.2f,m_targetHpPercent,scale);
			m_currentHPPercent = m_targetHpPercent;
			m_hpPicList[currentIndex]->runAction(
				CCSequence::create(actionScale,
				CCCallFuncN::create(this,callfuncN_selector(BossHPLayer::onHealthPointActionOver)),NULL)
				);
		}
		else
		{
			if(m_currentHPPercent == 0)
			{
				//当前血条已经减到底，切换血条
				if(m_currentHPLine == 0)
				{
					return;
				}
				m_currentHPPercent = UIManager::sharedManager()->getScaleFactor();
				--m_currentHPLine;

				setHpWithoutAnim(m_currentHPPercent,m_currentHPLine);
				startHpAnim(m_targetHpPercent,m_targetHPLine);
			}
			else
			{
				unsigned int currentIndex = m_currentHPLine % BOSS_HP_BAR_NUMBER;
				float scale = UIManager::sharedManager()->getScaleFactor();
				CCActionInterval* actionScale = CCScaleTo::create(0.2f,0,scale);
				m_currentHPPercent = 0;
				m_hpPicList[currentIndex]->runAction(
					CCSequence::create(actionScale,
					CCCallFuncN::create(this,callfuncN_selector(BossHPLayer::onHealthPointActionOver)),NULL)
					);
			}
		}


		b_isAnimFlag = true;
	}

}

void BossHPLayer::onHealthPointActionOver(CCNode* node)
{
	b_isAnimFlag = false;
}

void BossHPLayer::SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID enemyId,unsigned int monsterType)
{
	SpriteMonster* monster = SpriteMonsterMgr::Get()->GetMonster(enemyId.getID());
	if(monster == NULL)
	{
		return;
	}
	CCAssert(monster->getHPLine() != 0,"elite hp count is 0");

	float totalHp = monster->getTotalBlood();
	CCLOG("set boss hp: currentHp= %f, totalHp = %f",currentHp, totalHp);
	float eachHp = totalHp / monster->getHPLine();
	float reaminHpPercent = currentHp / eachHp;
	int remainHpNum = (int)reaminHpPercent;

	float scale = UIManager::sharedManager()->getScaleFactor();
	float percent = scale * (reaminHpPercent - remainHpNum);
	if(percent == 0 && remainHpNum != 0)
	{
		--remainHpNum;
		percent = scale;
	}

	bool isPlayAnim = true;
	if(m_targetMonsterId != enemyId.getID())
	{
		const char* mosterNameIndex = LuaTinkerManager::Get()->getLuaConfig<const char *>("MonsterBasicInfomation", "MonsterBasicInfomation", monsterType, "Name");
		CCAssert(mosterNameIndex,"monster name index is null");
		const char* monsterName = Localizatioin::getLocalization(mosterNameIndex);
		CCAssert(monsterName,"monster name is null");
		m_nameLabel->setString(monsterName);

		char levelStr[10];
		sprintf(levelStr,"Lv%d",monster->getLevel());
		m_levelLabel->setString(levelStr);

		m_currentHPLine = remainHpNum;
		m_targetHpPercent = percent;
		m_currentHPLine = remainHpNum;
		isPlayAnim = false;
	}

	if(isPlayAnim)
	{
		startHpAnim(percent,remainHpNum);
	}
	else
	{
		m_targetHpPercent = percent;
		m_targetHPLine = remainHpNum;
		setHpWithoutAnim(percent,remainHpNum);
		
	}

	

	m_targetMonsterId = enemyId.getID();
}