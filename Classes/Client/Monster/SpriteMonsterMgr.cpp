#include "SpriteMonsterMgr.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "SpriteSeer.h"
#include "ItemUpdateManager.h"
#include "LevelManager.h"
#include "LevelLayer.h"
#include "LuaTinkerManager.h"
#include "GameAudioManager.h"
#include "detect.hpp"
#include "UIManager.h"
#include "Localization.h"
#include "InstanceManager.h"

using namespace cocos2d;
using namespace TXGUI;

SpriteMonsterMgr::SpriteMonsterMgr()
	:isPlayingDieSound(false)
	,nowPlayingSoundId(0)
	,specialMonsterHpInited(false)
	,inSlow(false)
	,hasSlowed(false)
{
     
}

SpriteMonsterMgr::~SpriteMonsterMgr()
{

}

void SpriteMonsterMgr::CreateCacheMonster(int typeId)
{
	//CCLOG("create monster cache typeId: %d", typeId);
	SpriteMonster * monster = NULL;

	MonsterInitInfo info;
	info.typeId = typeId;

	monster = new SpriteMonster(info);
	if (monster)
	{
		monster->createTexture(typeId);
		if (cacheMonsters.find(typeId) == cacheMonsters.end())
		{
			set<SpriteMonster*> monsterSet;
			monsterSet.insert(monster);
			cacheMonsters[typeId] = monsterSet;
		}
		else
		{
			cacheMonsters[typeId].insert(monster);
		}
	}
	else
	{
		CC_SAFE_DELETE(monster);
	}
}

SpriteMonster* SpriteMonsterMgr::CreateMonster(MonsterInitInfo info)
{
	SpriteMonster * monster = NULL;

	
	do
	{
		//try to use cache , if failed create a new one
		if (info.useCache)
		{
			if (cacheMonsters.find(info.typeId) == cacheMonsters.end())
			{
				CCLOG("can't find cache monster type : %d", info.typeId);
				break;
			}

			set<SpriteMonster*> monsterSet = cacheMonsters[info.typeId];
			if (monsterSet.size() == 0)
			{
				CCLOG("cache monster count none : type : %d", info.typeId);
				break;
			}

			set<SpriteMonster*>::iterator iter = monsterSet.begin();
			monster = *iter;
			if (monster->initWithInfoAndShow(info))
			{
				if (Add(monster) == false)
				{
					CCLOG("create monster id:%d failed, it's already existed" , info.uid);
					break;
				}
				cacheMonsters[info.typeId].erase(*iter);
				//CCLOG("use cache monster type : %d", info.typeId);
				return monster;
			}
		}
	} while (0);



	monster = new SpriteMonster(info);

	do
	{
		if (!monster)
		{
			break;
		}
		monster->autorelease();

		if (monster->createTexture(info.typeId) && monster->initWithInfoAndShow(info))
		{
			if (Add(monster) == false)
			{
				CCLOG("create monster id:%d failed, it's already existed" , info.uid);
				CC_SAFE_DELETE(monster);
				monster = NULL;
			}
			return  monster;
		}
		else
		{
			break;
		}
	} while (0);

	CC_SAFE_DELETE(monster);
	return NULL;
}

SpriteMonster* SpriteMonsterMgr::GetMonster(int s_id)
{
	SpriteMonster * monster = null;
	map<int, SpriteMonster*>::iterator it = m_tabMonsters.find(s_id) ;
	if (it != m_tabMonsters.end())
	{
		monster = m_tabMonsters[it->first];
	}
	return monster;
}


bool SpriteMonsterMgr::Add(SpriteMonster * monster)
{    
	if (m_tabMonsters.find(monster->GetID().getID()) != m_tabMonsters.end())
	{
		return false;
	}

	m_tabMonsters[monster->GetID().getID()] = monster;

	return true;
}

void SpriteMonsterMgr::Remove(SpriteMonster * monster)
{
	map<int, SpriteMonster*>::iterator it = m_tabMonsters.find(monster->GetID().getID()) ;
	if (it != m_tabMonsters.end())
	{	
		m_tabMonsters.erase(it);				
	}
}

void SpriteMonsterMgr::Update(float dt)
{
	map<int, SpriteMonster*>::iterator it = m_tabMonsters.begin();
	for(; it != m_tabMonsters.end(); )
	{
		SpriteMonster* monster = it->second;
		bool bDeleteSelf = false;
		bool isDead = false;
		monster->Update(dt, bDeleteSelf, isDead);
		
		//死亡检测是否为最后一个boss
		if (isDead && !inSlow && !hasSlowed)
		{
			if (isLastBoss(monster))
			{
				inSlow = true;
				hasSlowed = true;
				CCDirector::sharedDirector()->getScheduler()->setTimeScale(0.2f);
			}
		}

		if (inSlow && isLastBoss(monster) && monster->selfScheduleTimer >= 0)
		{
			CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
			inSlow = false;
		}

		if (bDeleteSelf)
		{
			m_tabMonsters.erase(it++);
			monster->removeFromParentAndCleanup(true);			
		}
		else
		{
			it++;
		}
	}

	//检测播放声音
	if (isPlayingDieSound)
	{
		//该接口在Android失效，如果效果不好考虑用CD控制
		isPlayingDieSound = GameAudioManager::sharedManager()->isEffectIsPlaying(nowPlayingSoundId);
	}


	map<int , SkillSoundInfo>::iterator iter = skillSoundMap.begin();
	for (; iter != skillSoundMap.end(); iter++)
	{
		//首先刷新延迟时间
		if (iter->second.delayTime >= 0)
		{
			iter->second.delayTime -= dt;
		}		

		//尚未播放的声音，检测延迟时间是否已到
		if (iter->second.waitingForPlay == true)
		{
			if (iter->second.delayTime <= 0)
			{
				//延迟时间已到，可以播放
				GameAudioManager::sharedManager()->playEffect(iter->second.soundId, iter->second.isLoop);
				iter->second.waitingForPlay = false;
			}
		}
		else
		{
			//启动持续时间计时
			if (iter->second.continueTime >= 0)
			{
				iter->second.continueTime -= dt;
			}

			//终止后持续调用保证停止
			if (iter->second.continueTime <= 0 && iter->second.isLoop)
			{
				GameAudioManager::sharedManager()->stopEffect(iter->second.soundId);
			}
		}
	}
}

SpriteMonster * SpriteMonsterMgr::getNearestMonster()
{
	SpriteMonster * nearestMonster = NULL;
	float distance = 10000.0f;
	SpriteSeer * hero = GameManager::Get()->getHero();
	if (!hero || m_tabMonsters.size() == 0)
	{
		return NULL;
	}

	map<int, SpriteMonster*>::iterator it = m_tabMonsters.begin();

	SpriteMonster * firstMonster = it->second;

	//能被锁定的怪物才记入数据，隐身不计入
	if (firstMonster->getCanBeFocus() && !firstMonster->getIsAlliance())
	{
		distance = ccpDistance(firstMonster->getPosition(), hero->getPosition());
		nearestMonster = firstMonster;
	}	

	for(; it != m_tabMonsters.end(); it++ )
	{
		SpriteMonster* monster = it->second;
		if (!monster->getCanBeFocus() || monster->getIsAlliance())
		{
			continue;
		}

		float dis = ccpDistance(monster->getPosition(), hero->getPosition());
		if (dis < distance && monster->fsm_rule.IsRunning())
		{
			distance = dis;
			nearestMonster = monster;
		}	
	}

	return nearestMonster;
}

bool SpriteMonsterMgr::isLastBoss(SpriteMonster * monster)
{
	bool isBoss = false;
	bool isLast = true;

	//传送地图为零说明为最后一张地图
	if (InstanceManager::Get()->getPortalId() == 0)
	{
		map<int, SpriteMonster*>::iterator it = m_tabMonsters.begin();
		for(; it != m_tabMonsters.end(); it++)
		{
			SpriteMonster* itMonster = it->second;
			if (itMonster->isBoss)
			{
				if (monster != itMonster)
				{
					isLast = false;
					break;
				}
				isBoss = true;
			}
		}
	}

	return isBoss && isLast;
}

bool SpriteMonsterMgr::playMonsterDieSound(int typeId)
{
	bool ret = false;

	if (false == isPlayingDieSound)
	{
		int soundId = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterBasicInfomation", "MonsterBasicInfomation", typeId, "SoundID_Death");
		GameAudioManager::sharedManager()->playEffect(soundId, false);

		isPlayingDieSound = true;
		nowPlayingSoundId = soundId;
		
		ret = true;
	}

	return ret;
}

bool SpriteMonsterMgr::playMonsterSkillSound(int skillId)
{
	bool ret = false;
	
	int soundId = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterSkill", "MonsterSkills", skillId, "sound_id");
	int delayTime =  LuaTinkerManager::Get()->getLuaConfig<int>("MonsterSkill", "MonsterSkills", skillId, "sound_delay_time");
	int continueTime = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterSkill", "MonsterSkills", skillId, "continue_time");
	int isLoop = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterSkill", "MonsterSkills", skillId, "sound_loop");

	SkillSoundInfo info;
	info.skillId = skillId;
	info.soundId = soundId;
	info.delayTime = delayTime / 1000.0f;
	info.continueTime = continueTime;
	info.isLoop = isLoop == 1;
	info.waitingForPlay = true;

	//加入播放列表，在update中播放
	skillSoundMap[skillId] = info;

	return ret;
}

//type : 1 世界boss 2 勇者试炼 3 主城防守
void SpriteMonsterMgr::updateSpecialMonsterHPLayer(int id, int nowHp, int totalHp, int type)
{
	UILayout * layout = UIManager::sharedManager()->getUILayout("WorldBossHPUI");
	SpriteMonster * monster = NULL;
	if (m_tabMonsters.find(id) != m_tabMonsters.end())
	{
		monster = m_tabMonsters[id];
	}
	if (layout && monster)
	{		
		if (!specialMonsterHpInited)
		{			
			const char* mosterNameIndex = LuaTinkerManager::Get()->getLuaConfig<const char *>("MonsterBasicInfomation", "MonsterBasicInfomation", monster->GetTypeId(), "Name");
			const char* monsterName = Localizatioin::getLocalization(mosterNameIndex);

			CCSprite * monsterHPBarNode = NULL;

			if (type != 3)
			{
				layout->FindChildObjectByName<UILabel>("bossName")->setString(monsterName);

				stringstream bossLevelStream;
				bossLevelStream << "Lv." << monster->getLevel();
				string bossLevel = bossLevelStream.str();

				//layout->FindChildObjectByName<UILabel>("bossLevel")->setString(bossLevel.c_str());
				layout->FindChildObjectByName<UILabel>("bossLevel")->setString("");

				monsterHPBarNode = (CCSprite *)layout->FindChildObjectByName<UIPicture>("bossHPBar")->getCurrentNode();
				
			}
			else
			{
				layout->FindChildObjectByName<UILabel>("cityName")->setString(monsterName);

				monsterHPBarNode = (CCSprite *)layout->FindChildObjectByName<UIPicture>("cityHPBar")->getCurrentNode();
			}



			monsterHPBarNode->setAnchorPoint(ccp(0, 0.5f));

			CCPoint oldPos = monsterHPBarNode->getPosition();
			float scale = UIManager::sharedManager()->getScaleFactor();
			monsterHPBarNode->setPosition(ccp(oldPos.x-scale * monsterHPBarNode->getContentSize().width/2, oldPos.y));

			specialMonsterHpInited = true;
		}

		stringstream hpStream;
		hpStream << nowHp << "/" << totalHp;
		string hpStr = hpStream.str();


		switch (type)
		{
		case 1:
			//layout->FindChildObjectByName<UILabel>("bossHPNum")->setString(hpStr.c_str());
			layout->FindChildObjectByName<UILabel>("bossHPNum")->setString(Localizatioin::getLocalization("M_WORLDBOSS_HP"));
			break;

		case 2:
			layout->FindChildObjectByName<UILabel>("bossHPNum")->setString("");
			break;

		default:
			break;
		}

		
		CCSprite * monsterHPBarNode = NULL;
		if (type != 3)
		{
			monsterHPBarNode = (CCSprite *)layout->FindChildObjectByName<UIPicture>("bossHPBar")->getCurrentNode();
		}
		else
		{
			monsterHPBarNode = (CCSprite *)layout->FindChildObjectByName<UIPicture>("cityHPBar")->getCurrentNode();
		}
		
		float scale = UIManager::sharedManager()->getScaleFactor();
		float percent= scale;
		if(totalHp != 0)
		{
			percent *= nowHp * 1.0f / totalHp;
		}

		CCActionInterval* actionScale = CCScaleTo::create(0.2f,percent,scale);
		monsterHPBarNode->runAction(actionScale);
	}
}

void SpriteMonsterMgr::resetData()
{
	m_tabMonsters.clear();
	cacheMonsters.clear();
	skillSoundMap.clear();

	isPlayingDieSound= false;
	nowPlayingSoundId = 0;
	specialMonsterHpInited = false;
	inSlow = false;
	hasSlowed = false;
}


