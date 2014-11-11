// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#include "OnlineNetworkManager.h"
#include "AttackChecker.h"
#include "SpriteSeer.h"
#include "SpriteElf.h"
#include "SpriteMonsterMgr.h"
#include "SpriteMonster.h"
#include "GameManager.h"
#include "SceneLayer.h"
#include "MainMenuLayer.h"
#include "ParticleManager.h"
#include "SkillDispMgr.h"
#include "SkillInfo.h"
#include "GameAudioManager.h"
#include "UserData.h"
#include "LevelManager.h"
#include "LevelLayer.h"
#include "CameraController.h"
#include "MathUtil.h"
#include "PvAIManager.h"
#include "MonsterData.h"

AttackChecker::CBase::CBase(BoneSpriteBase* pAttacker) 
	:m_skillID(0)
	,m_attackPeriod(0.0f)
	, m_pAttacker(pAttacker)
	, m_bAttackOtherPlayer(false)
	, m_attackNumber(0)
	, m_bBasedOnFrame(false)
	, m_beatBackDistance(0)
	, m_type(ToNormalAttack)
	, m_attackType(tagSkillInfo::E_NORMAL)
	, m_attackBoxFrame(-1)
	, m_bFullScreenAttack(false)
	, m_bShakeCamera(false)
	, m_action(E_PLAYER_ATTACK_MONSTER)
	, m_uRagePoint(0)
	, m_hurtAddValue(0)
	, m_hurtPercent(100)
{
	m_defenderID.setPlayId(0,0,0);
	m_bPlayCritSound = false;
	m_bPlayUncritSound = false;
	m_fDeltaTimeCrit = 0;
	m_fDeltaTimeUncrit = 0;
	m_Elf = (SpriteElf *)NULL ;
	attackTypeCheck =0 ;
	if(m_pAttacker)
	{
		if(dynamic_cast<EffectSprite*>(m_pAttacker))
		{
			if(GameManager::Get()->getHero())
				m_attackerID = GameManager::Get()->getHero()->GetID();
		}
		else
		{
			m_attackerID = m_pAttacker->GetID();
		}
	}
}

AttackChecker::CBase::~CBase()
{

}

void AttackChecker::CBase::Update( float dt )
{
	// reset crit sound playing flag
	m_bPlayCritSound = false;
	m_bPlayUncritSound = false;
	m_curAttackTime -= dt;

	if(m_curAttackTime > 0.0f)
	{
		return;
	}

	m_curAttackTime = m_attackPeriod;

	if (m_pAttacker)
	{
		cocos2d::CCRect heroABox = m_pAttacker->getRelativeParentAttackBox();

		if(m_bFullScreenAttack)
		{
			CCPoint cameraOffset = CCDirector::sharedDirector()->getLevelRenderCameraOffset();
			CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
			heroABox = CCRectMake(cameraOffset.x,cameraOffset.y,visibleSize.width,visibleSize.height);	
		}

		int curFrame = m_pAttacker->getCurAnimationFrameID();
		if(m_attackBoxFrame != curFrame)
		{
			m_attackBoxFrame = curFrame;
			if(heroABox.size.width > 0.0f || heroABox.size.height > 0.0f)
			{
				//CCLOG("Attacker : frame : %ld, rect(%f, %f) ", curFrame, heroABox.size.width, heroABox.size.height);

				if(m_bAttackOtherPlayer)//攻击框 和 敌人位置 
				{
					if(LevelManager::sShareInstance()->getCurrentLevelLayer())
					{
						if(GameManager::Get()->getHero() && GameManager::Get()->getHero()->GetID() != m_attackerID)
						{
							if(!IsBattleSide(GameManager::Get()->getHero()))
							{
								IntersectsRect(heroABox, GameManager::Get()->getHero());
							}
						}

						std::map<PLAYER_ID, OthersPlayerInfo>& otherPlayer = LevelManager::sShareInstance()->getCurrentLevelLayer()->m_othersPlayerInfo;
						for(std::map<PLAYER_ID, OthersPlayerInfo>::const_iterator iter = otherPlayer.begin(); iter != otherPlayer.end(); iter++)
						{
							BoneSpriteBase* pOtherHero = dynamic_cast<BoneSpriteBase*>(iter->second.seer); 
							if(pOtherHero->GetID() == m_attackerID)
								continue;
							if(!IsBattleSide(pOtherHero))
								IntersectsRect(heroABox, pOtherHero);
						}
					}
				}
				else
				{
					const map<int, SpriteMonster*>& allMonster = SpriteMonsterMgr::Get()->GetAllMonster();
					for(map<int, SpriteMonster*>::const_iterator iter = allMonster.begin(); iter != allMonster.end(); iter++)
					{
						SpriteMonster* pMonster = iter->second; 
						if(pMonster->getCanBeAttacked())
							IntersectsRect(heroABox, pMonster);
						
					}
				} // if(m_bAttackOtherPlayer)
			}
			

		}
		
	}

	// 播放攻击的声音
	if (m_bPlayCritSound )
	{
		PlayCritSound(dt);
	}

	if (m_bPlayUncritSound)
	{
		PlayUncritSound(dt);
	}

}

bool AttackChecker::CBase::IsBattleSide(BoneSpriteBase* pOther)
{
	if(m_bAttackOtherPlayer)
	{
		
		USERINFO* pAttackerUserInfo = UserData::Get()->GetUserInfo(m_attackerID);
		USERINFO* pDefenUserInfo = UserData::Get()->GetUserInfo(pOther->GetID());
		if(pAttackerUserInfo && pDefenUserInfo)
		{
			if(pAttackerUserInfo->battleSide % 2 == pDefenUserInfo->battleSide % 2)
			{
				return true;
			}
			return false;
		}
	}
	return true;
}
void AttackChecker::CBase::IntersectsRect(cocos2d::CCRect& attackBox, BoneSpriteBase* pOther)
{
	cocos2d::CCRect monsterRect = pOther->getRelativeParentColliderRect();
	

	if(monsterRect.size.width > 0.0f && 
		monsterRect.size.height > 0.0f &&
		attackBox.intersectsRect(monsterRect)
		)
	{
		m_defenderID = pOther->GetID();

		int attackedCount = 0; 
		map<PLAYER_ID, int>::iterator attackIter = m_attackedMonsterList.find(m_defenderID);

		if(attackIter != m_attackedMonsterList.end())
		{
			attackIter->second++;
			attackedCount = attackIter->second;
		}
		else
		{
			attackedCount = 1;
			m_attackedMonsterList[m_defenderID] = attackedCount;
		}

		DoAttack(pOther, attackedCount);
	}
}

/// 申请播放暴击声音
void AttackChecker::CBase::PlayCritSound(float dt)
{
	//m_fDeltaTimeCrit -= dt;

	//if (m_fDeltaTimeCrit < 0)
	//{
	//	GameAudioManager::sharedManager()->playEffect(GetSkillInfo()->SoundId,	false);
	//	m_fDeltaTimeCrit = 1.0f;
	//}

	if (false == GameAudioManager::sharedManager()->isEffectIsPlaying(321003))
	{
		GameAudioManager::sharedManager()->playEffect(321003,	false);
	}
}

/// 申请播放非暴击声音
void AttackChecker::CBase::PlayUncritSound(float dt)
{
	//m_fDeltaTimeUncrit -= dt;

	//if (m_fDeltaTimeUncrit < 0)
	//{
	//	m_fDeltaTimeUncrit = 0.5f;
	//	GameAudioManager::sharedManager()->playEffect(GetSkillInfo()->SoundId,	false);
	//}

	if (false == GameAudioManager::sharedManager()->isEffectIsPlaying(321004))
	{
		GameAudioManager::sharedManager()->playEffect(321004,	false);
	}
}

void AttackChecker::CBase::SetSkillID( unsigned int id )
{
	m_skillID = id;
	m_attackedMonsterList.clear();
	
	const SKILL_INFO* pSkill = SkillDispMgr::Get()->GetSkillInfo(id);
	m_attackNumber = 1;
	float fContinueTime = 0.0f;

	m_bBasedOnFrame = false;
	m_beatBackDistance = 0.0f;

	if(pSkill)
	{
		ClientUserSkillInfo info;
		if(SkillDispMgr::Get()->GetOneSkillInfoById(m_attackerID, m_skillID, info))
		{
			//if(SkillDispMgr::Get()->GetOneSkillInfoById(m_skillID, info))
			{
				if(pSkill->partID == info.parts[0].part)
				{
					fContinueTime = info.parts[0].continue_time;
				}
			}
		}
		else
		{
			
			fContinueTime = pSkill->continueTime;
		}

		m_bBasedOnFrame = pSkill->bBasedOnFrame;
		m_beatBackDistance = pSkill->beatBackDistance;

		m_type = pSkill->type;
		m_attackType = pSkill->attackType;
		m_attackNumber = pSkill->attackNumber;
		if( pSkill->attackNumber > 0)
		{
			m_uRagePoint = pSkill->ragePoint / pSkill->attackNumber;
		}			
		else
			m_uRagePoint = pSkill->ragePoint;

		if(pSkill->iShakeCamera == 2)
		{
			m_bShakeCamera = true;
		}
	}
	else
	{
		const FAIRY_SKILL_INFO* pFairySkill = SkillDispMgr::Get()->GetFairySkillInfo(id);
		if(pFairySkill)
		{
			m_attackNumber = pFairySkill->attack_number;
			fContinueTime = pFairySkill->continueTime;
			m_hurtAddValue = pFairySkill->hurtAddValue;
			m_hurtPercent = pFairySkill->hurt;
			m_attackType = tagSkillInfo::E_FAIRY_SKILL;
		}
	}

	if( m_attackNumber > 1)
	{
		float time = 0.0f;

		if(fContinueTime > 0.0f)
		{
			time = fContinueTime;
		}
		else
		{
			// TODO: Animation Time
			//time = 
		}
		m_attackPeriod = time / (float)m_attackNumber;
	}
		
		


	m_curAttackTime = 0.0f;
}

void AttackChecker::CBase::DoAttack( BoneSpriteBase* pDefender, int count )
{

}

AttackChecker::CBase::INJURE_DATA AttackChecker::CBase::CalInjure(tagSkillInfo::ATTACK_TYPE attackType, unsigned int damage, const BATTLEINFO* pAttackInfo, const BATTLEINFO* pDefenerInfo)
{
	static const unsigned int ALL_CONSTANT = 10000;
	static const unsigned int DODGE_CONSTANT = 0;
	static const unsigned int PARRY_REDUCE_CONSTANT = 5000;
	static const unsigned int CRIT_MULT_CONSTANT = 10000;
	static const float MIN_HURT_CONSTANT = 0.5f;

	INJURE_DATA ret;

	if(!pAttackInfo || !pDefenerInfo)
	{
		return ret;
	}

	int hurt = 0;
	tagSkillInfo::ATTACK_TYPE type = tagSkillInfo::E_NORMAL;

	unsigned int typeProbability = ranged_random(0, 10000);
	
	unsigned int probability_max = 8000;
	unsigned int current_dodge = DODGE_CONSTANT + pDefenerInfo->dodge * 10 > pAttackInfo->accurate *10 ? 
		DODGE_CONSTANT + pDefenerInfo->dodge * 10 - pAttackInfo->accurate *10 : 0 ;

	current_dodge = current_dodge > probability_max? probability_max : current_dodge;
	if(typeProbability < probability_max && typeProbability >= probability_max - current_dodge)
	{
		type = tagSkillInfo::E_DODGE;
	}
	probability_max -= current_dodge;
	
	unsigned int current_parry = pDefenerInfo->parry * 10 > pAttackInfo->wreck * 10 ?
		pDefenerInfo->parry * 10 - pAttackInfo->wreck * 10 : 0;

	current_parry = current_parry > probability_max? probability_max : current_parry;
	if(typeProbability < probability_max && typeProbability >= probability_max - current_parry)
	{
		type = tagSkillInfo::E_PARRY;

	}
	probability_max -= current_parry;
	
	unsigned int  current_crit = pAttackInfo->critical_strike * 10 > pDefenerInfo->tenacity * 10 ?
		pAttackInfo->critical_strike * 10 - pDefenerInfo->tenacity * 10 : 0;

	current_crit = current_crit > probability_max? probability_max : current_crit;

	if(typeProbability < probability_max && typeProbability >= probability_max - current_crit)
	{
		type = tagSkillInfo::E_STRIKE;

	}

	probability_max -= current_crit;

	switch(attackType)
	{
	case tagSkillInfo::E_NORMAL:	// 普通攻击
		{
			unsigned int attackValue = pAttackInfo->physical_attack;
			unsigned int defenceValue = pDefenerInfo->physical_defence;
			unsigned int hurt1 = (attackValue > defenceValue ? ((attackValue - defenceValue ) / m_attackNumber): 1) * damage /100 * ranged_random(95, 105) / 100;
			unsigned int hurt2 = attackValue * MIN_HURT_CONSTANT / 100;
			hurt = hurt1 > hurt2? hurt1: hurt2;

			if(hurt < 1)
				hurt = 1;
		}

		break;
	case tagSkillInfo::E_MAGIC: // 魔法攻击
		{
			unsigned int attackValue = pAttackInfo->magic_attack;
			unsigned int defenceValue = pDefenerInfo->magic_defence;
			unsigned int hurt1 = (attackValue > defenceValue ? (attackValue - defenceValue) / m_attackNumber: 0) * damage /100 * ranged_random(95, 105) / 100;
			unsigned int hurt2 = attackValue * MIN_HURT_CONSTANT / 100;
			hurt = hurt1 > hurt2? hurt1: hurt2;

			if(hurt < 1)
				hurt = 1;
		}
		break;
	case tagSkillInfo::E_SKILL: // 技能攻击
		{
			int hurtPercent = 100;
			int skillAttackValue = 0;
			ClientUserSkillInfo info;
			if(SkillDispMgr::Get()->GetOneSkillInfoById(m_attackerID, m_skillID, info))
			{
				hurtPercent = info.parts[0].hurt;
				skillAttackValue = info.parts[0].exp_battle_info.skill_attack;
			}

			int attackValue = pAttackInfo->skill_attack;
			int defenceValue = pDefenerInfo->skill_defence;

			int hurt1 = 0;
			hurt1 = (attackValue  + skillAttackValue - defenceValue) / m_attackNumber * hurtPercent / 100 ;
			hurt1 = hurt1 * ranged_random(95, 105) / 100;
			int hurt2 = attackValue * MIN_HURT_CONSTANT / 100;
			hurt = hurt1 > hurt2? hurt1: hurt2;

			if(hurt < 1)
				hurt = 1;
		}
		break;
	case tagSkillInfo::E_FAIRY_SKILL: // 精灵技能攻击
		{
			int hurtPercent = m_hurtPercent;
			int skillAttackValue = m_hurtAddValue;
			int attackValue =m_Elf->getSkAttack();// pAttackInfo->skill_attack; 
			int playerValue = pAttackInfo->skill_attack;
			int defenceValue = pDefenerInfo->skill_defence;

			int hurt1 = 0;
			//hurt1 = (attackValue   + skillAttackValue - defenceValue) / m_attackNumber* hurtPercent / 100;
			hurt1 = (playerValue   + skillAttackValue - defenceValue)/(playerValue / (double)attackValue) / m_attackNumber* hurtPercent / 100;
			hurt1 = hurt1 * ranged_random(95, 105) / 100;
			int hurt2 = playerValue * MIN_HURT_CONSTANT / 100;
			hurt = hurt1 > hurt2? hurt1: hurt2;

			if(hurt < 1)
				hurt = 1;
		}
		break;
	default:
		break;
	}

	switch (type)
	{
	case tagSkillInfo::E_DODGE:
		{
			hurt = 0;
		}
		break;
	case tagSkillInfo::E_PARRY:
		{
			hurt = hurt * (ALL_CONSTANT - PARRY_REDUCE_CONSTANT) / ALL_CONSTANT ;

			if(hurt < 1)
				hurt = 1;

		}
		break;
	case tagSkillInfo::E_STRIKE:
		{
			hurt = hurt * (CRIT_MULT_CONSTANT + pAttackInfo->slay * 10 ) / ALL_CONSTANT;

			if(hurt < 1)
				hurt = 1;
		}
		break;
	default:
		break;
	}

	ret.hurt = hurt;
	ret.attackType = type;
	return ret;
}

int AttackChecker::CBase::GetAttackCoef( )
{
	int attackCoef = 100;
	ClientUserSkillInfo info;
	if(SkillDispMgr::Get()->GetOneSkillInfoById(m_attackerID, m_skillID, info))
	{
		attackCoef = (int)info.parts[0].hurt;
	}

	// CCLOG("skillID : %u, attackCoef : %ld", m_skillID, attackCoef);

	return attackCoef;
}

// -----------------------------------------------------------------------------------------------------------

void AttackChecker::CNormalAttack::DoAttack( BoneSpriteBase* pDefender, int count )
{
	//友方（主城防守）直接返回
	if(dynamic_cast<SpriteMonster*>(pDefender))
	{
		if (dynamic_cast<SpriteMonster*>(pDefender)->getIsAlliance())
		{
			return;
		}
	}

	if(!m_bBasedOnFrame)
	{
		if(count > m_attackNumber)
			return;
	}

	BoneSpriteBase* pMonster = dynamic_cast<BoneSpriteBase*>(pDefender);
	SpriteSeer* pHero = LevelManager::sShareInstance()->getCurrentLevelLayer()->getPlayer(m_attackerID);

	if(pHero == NULL || pMonster == NULL)
		return;

	int attackCoef = GetAttackCoef();

	BATTLEINFO* pAttackInfo = dynamic_cast<BATTLEINFO*>(UserData::Get()->GetUserInfo(pHero->GetID()));
	BATTLEINFO* pDefenerInfo = NULL;
	if(m_bAttackOtherPlayer)
	{
		pDefenerInfo = dynamic_cast<BATTLEINFO*>(UserData::Get()->GetUserInfo(pMonster->GetID()));
	}
	else
	{
		//pDefenerInfo = new BATTLEINFO(MonsterData::GetMonsterInfo()->getAttackBattleInfo(pMonster->GetID()));

		//pDefenerInfo = &(MonsterData::GetMonsterInfo()->getAttackBattleInfo(pMonster->GetID()));
		pDefenerInfo = (BATTLEINFO*)(USERINFO*)(EntityInfo::Get()->GetInfo(pMonster->GetID()));
	}

	BATTLEINFO fairyBattleInfo;
	switch(m_action)
	{
	case CBase::E_FAIRY_ATTACK_MONSTER:
	case CBase::E_FAIRY_ATTACK_PLAYER:
		{
			const FAIRY_SKILL_INFO* pFairySkillInfo = SkillDispMgr::Get()->GetFairySkillInfo(m_skillID);
			if(pFairySkillInfo)
			{
				fairyBattleInfo = *dynamic_cast<const BATTLEINFO*>(pFairySkillInfo);
				fairyBattleInfo = fairyBattleInfo + *pAttackInfo;
				pAttackInfo = &fairyBattleInfo;
			}
		}
		break;
	default:
		{
			//if(pHero->getTag() == MAP_TAG_SEER)
			{
				USERINFO* pUserInfo = (USERINFO*)(pAttackInfo);
				if(pUserInfo)
				{
					int rageRate1 = pHero->GetFairyRageRate(1);
					int rageRate2 = pHero->GetFairyRageRate(2);
					if(dynamic_cast<SpriteSeer*>(pDefender))
					{
						if (rageRate1 > 0)
						{
							pUserInfo->ragePoint += m_uRagePoint * (2 + rageRate1 * 1.0f / 100);
						}
						
						if (rageRate2 > 0)
						{
							pUserInfo->secRagePoint += m_uRagePoint * (2 + rageRate2 * 1.0f / 100);
						}						
					}
					else
					{
						if (rageRate1 > 0)
						{
							pUserInfo->ragePoint += m_uRagePoint * (1 + rageRate1 * 1.0f / 100);
						}
						
						if (rageRate2 > 0)
						{
							pUserInfo->secRagePoint += m_uRagePoint * (1 + rageRate2 * 1.0f / 100);
						}
					}
					
					if(pUserInfo->ragePoint > pUserInfo->totalRangePoint)
					{
						pUserInfo->ragePoint = pUserInfo->totalRangePoint;
					}
					if(pUserInfo->secRagePoint > pUserInfo->secTotalRangePoint)
					{
						pUserInfo->secRagePoint = pUserInfo->secTotalRangePoint;
					}
				}
			}
		}
		break;
	}

	INJURE_DATA injure = CalInjure( m_attackType, attackCoef, pAttackInfo, pDefenerInfo);//伤害计算

	if(dynamic_cast<SpriteSeer*>(pDefender))
	{
		if(!dynamic_cast<SpriteSeer*>(pDefender)->CanHurt())
		{
			injure.hurt = 0;
		}
	}

	CCPoint dir = ccpSub(pMonster->getPosition(), pHero->getPosition());

	//当主角和怪物位置重合时，不能直接Normalize
	if (dir.x == 0 && dir.y == 0)
	{
		dir = pHero->getDirectionVector();
	}
	dir = ccpNormalize(dir);
	dir = ccpMult(dir, m_beatBackDistance );
	dir = ccpAdd(dir, pMonster->getPosition());
	CCPoint pos = pMonster->getPosition();
	if(injure.attackType != tagSkillInfo::E_DODGE)
	{
		if(dynamic_cast<SpriteMonster*>(pMonster))
		{
			pos = dynamic_cast<SpriteMonster*>(pMonster)->onEventAttacked(dir);

		}

		if(GameManager::Get()->getHero() && m_attackerID == GameManager::Get()->getHero()->GetID())
		{
			GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleComboIncrease();
		}

	}

	AttackMonsterInfo attackMonsterInfo;
	attackMonsterInfo.set_id(GetDefenderID().getID());
	CSPoint* pCSPoint = attackMonsterInfo.mutable_start_pos();
	CCPoint startPos = LevelManager::sShareInstance()->pointPixelToCell(pMonster->getPosition());
	pCSPoint->set_xpos(startPos.x);
	pCSPoint->set_ypos(startPos.y);

	pCSPoint = attackMonsterInfo.mutable_pos();
	pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
	pCSPoint->set_xpos(pos.x);
	pCSPoint->set_ypos(pos.y);
	
	pCSPoint = attackMonsterInfo.mutable_direct();
	dir.setPoint(1.0f, 0.0f);
	if(dynamic_cast<BoneSpriteMoveable*>(pMonster))
	{
		dir = dynamic_cast<BoneSpriteMoveable*>(pMonster)->getDirectionVector();

	}
	pCSPoint->set_xpos(dir.x);
	pCSPoint->set_ypos(dir.y);

	attackMonsterInfo.set_blood( injure.hurt );
	
	attackMonsterInfo.set_status(MONSTER_HURT);
	
	m_attackMessage.push_back(attackMonsterInfo);

	bool isCrit = false;
	bool isFairy = (m_attackType == tagSkillInfo::E_FAIRY_SKILL);
	CCPoint damagePoint = ccpAdd(pMonster->getPosition(),pMonster->getAttachPoint(kType_PLAYER_NAME));
	// 主角的伤害都在handlePVPPlayerHitPlayer里处理。这个不用处理了，要不就是二次处理了，现在这个攻击包会发送到服务器，服务器会再次返回来。
	if(!m_bAttackOtherPlayer)
	{
		switch(injure.attackType)
		{
		case tagSkillInfo::E_PARRY:
			GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackBlockEffect(damagePoint);
			if(m_bAttackOtherPlayer)
			{
				if(dynamic_cast<SpriteSeer*>(m_pAttacker))
				{
					dynamic_cast<SpriteSeer*>(m_pAttacker)->ByAttacked(injure.hurt, m_uRagePoint,false,isFairy);
				}
			}
			else
			{
				
				GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleDamageNumberEffect(damagePoint, injure.hurt, false,true,isFairy);
				if(dynamic_cast<SpriteMonster*>(pMonster))
				{
					dynamic_cast<SpriteMonster*>(pMonster)->PlayAttackedEffect(false);
				}
			}
			break;
		case tagSkillInfo::E_STRIKE:
			{
				isCrit = true;
				if(m_bAttackOtherPlayer)
				{
					if(dynamic_cast<SpriteSeer*>(pMonster))
					{
						dynamic_cast<SpriteSeer*>(pMonster)->ByAttacked(injure.hurt, m_uRagePoint, true,isFairy);
					}
				}
				else
				{
					GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleDamageNumberEffect(damagePoint, injure.hurt, true,true,isFairy);
					if(dynamic_cast<SpriteMonster*>(pMonster))
					{
						dynamic_cast<SpriteMonster*>(pMonster)->PlayAttackedEffect(true);
					}
				}

				if(GameManager::Get()->getHero() && m_attackerID == GameManager::Get()->getHero()->GetID())
				{
					GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleCritBurst(damagePoint);
				}

			}
			break;
		case tagSkillInfo::E_DODGE:
			{
				GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackMissEffect(damagePoint);
				break;
			}
			
		default:
			if(m_bAttackOtherPlayer)
			{
				if(dynamic_cast<SpriteSeer*>(pMonster))
				{
					dynamic_cast<SpriteSeer*>(pMonster)->ByAttacked(injure.hurt, m_uRagePoint,false,isFairy);
				}
			}
			else
			{
				GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleDamageNumberEffect(damagePoint, injure.hurt, false,true,isFairy);
				if(dynamic_cast<SpriteMonster*>(pMonster))
				{
					dynamic_cast<SpriteMonster*>(pMonster)->PlayAttackedEffect(false);
				}

			}
			break;
		}
	}
	else
	{
		switch(injure.attackType)
		{

		case tagSkillInfo::E_DODGE:	// 因为在这个状态的时候，是不会发包到服务器了，收不到 handlePVPPlayerHitPlayer
			{
				GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackMissEffect(damagePoint);
				break;
			}
			
		default:
			break;
		}
	}
	
	if(isCrit)
	{
		m_bPlayCritSound = true;

		if(GameManager::Get()->getHero() && m_attackerID == GameManager::Get()->getHero()->GetID())
		{
			float scaleTime = 0.1f;
			CameraController::sharedCameraController()->shakeOnly(scaleTime, 20.0f);
		}
		
	}
	else
	{
		if(injure.attackType == tagSkillInfo::E_DODGE)
		{
			m_bPlayUncritSound = false;
		}
		else
		{
			// 击中了怪物，
			m_bPlayUncritSound = true;
		}
		
		if(m_bShakeCamera)
		{
			if(GameManager::Get()->getHero() && m_attackerID == GameManager::Get()->getHero()->GetID())
			{
				float scaleTime = 0.5f;
				CameraController::sharedCameraController()->shakeAndScale(scaleTime);
			}
			
		}
	}
	
	if(m_bAttackOtherPlayer)
	{

		AttackPlayerInfo attackOtherHeroInfo;
		attackOtherHeroInfo.set_userid(pMonster->GetID().getID());
		attackOtherHeroInfo.set_reg_time(pMonster->GetID().getRegTime());
		attackOtherHeroInfo.set_channel_id(pMonster->GetID().getChannel());
		attackOtherHeroInfo.set_blood(injure.hurt);
		switch(injure.attackType)
		{
		case tagSkillInfo::E_DODGE:
			attackOtherHeroInfo.set_hurt_type(1);
			break;
		case tagSkillInfo::E_PARRY:
			attackOtherHeroInfo.set_hurt_type(2);
			break;
		case tagSkillInfo::E_STRIKE:
			attackOtherHeroInfo.set_hurt_type(3);
			break;
		default:
			attackOtherHeroInfo.set_hurt_type(4);
			break;
		}

		CSPlayerHitPlayerReq attackReq;
		attackReq.set_userid(pHero->GetID().getID());
		attackReq.set_reg_time(pHero->GetID().getRegTime());
		attackReq.set_channel_id(pHero->GetID().getChannel());
		CSPoint* pCSPoint = attackReq.mutable_start_pos();

		// TODO : user hero or effect?
		CCPoint pos = pHero->getPosition();

		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);
		pCSPoint = attackReq.mutable_start_direct();
		CCPoint dir = pHero->getDirectionVector();
		pCSPoint->set_xpos(dir.x);
		pCSPoint->set_ypos(dir.y);

		dir = pHero->getDirectionVector();
		pCSPoint = attackReq.mutable_end_direct();
		pCSPoint->set_xpos(dir.x);
		pCSPoint->set_ypos(dir.y);

		pos = pHero->getPosition();
		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		pCSPoint = attackReq.mutable_end_pos();
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);

		attackReq.set_action_id(m_skillID);
		attackReq.clear_users();
		*attackReq.add_users() = attackOtherHeroInfo;

		if(pHero)
		{
			if(injure.attackType != tagSkillInfo::E_DODGE)
			{
				if(LevelManager::sShareInstance()->isCurrentPVAILevel())
				{
					OnlineNetworkManager::sShareInstance()->sendPlayerAttackOtherPlayerMessage(attackReq);
					PvAIManager::Get()->onEventLocalHurt(attackReq);
				}
				else
				{
					if(GameManager::Get()->getHero() && !GameManager::Get()->getHero()->IsDie()&&m_attackerID == GameManager::Get()->getHero()->GetID())//判断玩家是否死
						OnlineNetworkManager::sShareInstance()->sendPlayerAttackOtherPlayerMessage(attackReq);//伤害同步
				}
				
			}
		}
	}
	else
	{
		CSPlayerAttackReq attackReq;
		attackReq.set_userid(m_attackerID.getID());
		attackReq.set_reg_time(m_attackerID.getRegTime());
		attackReq.set_channel_id(m_attackerID.getChannel());
		pCSPoint = attackReq.mutable_start_pos();
		pos = pHero->getPosition();
		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);
		pCSPoint = attackReq.mutable_start_direct();
		dir = pHero->getDirectionVector();
		pCSPoint->set_xpos(dir.x);
		pCSPoint->set_ypos(dir.y);

		dir = pHero->getDirectionVector();
		pCSPoint = attackReq.mutable_end_direct();
		pCSPoint->set_xpos(dir.x);
		pCSPoint->set_ypos(dir.y);

		pos = pHero->getPosition();
		pos = LevelManager::sShareInstance()->pointPixelToCell(pos);
		pCSPoint = attackReq.mutable_end_pos();
		pCSPoint->set_xpos(pos.x);
		pCSPoint->set_ypos(pos.y);

		attackReq.set_action_id(m_skillID);
		attackReq.clear_monsters();
		*attackReq.add_monsters() = attackMonsterInfo;

		//if (m_skillID == 4001 || m_skillID == 3001)
		//{
		//	attackReq.set_buffer_id(1);
		//	attackReq.set_buffer_time(2000);
		//}
		//else
		//{
		//	attackReq.set_buffer_id(0);
		//	attackReq.set_buffer_time(0);
		//}

		if(injure.attackType != tagSkillInfo::E_DODGE)
		{
			if(LevelManager::sShareInstance()->isCurrentPVAILevel())
			{
				OnlineNetworkManager::sShareInstance()->sendPlayerAttackMessage(attackReq);
			}
			else if(LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
			{
				OnlineNetworkManager::sShareInstance()->sendPlayerAttackMessage(attackReq);
			}
			else
			{
				if(GameManager::Get()->getHero() && !GameManager::Get()->getHero()->IsDie()&&m_attackerID == GameManager::Get()->getHero()->GetID())//判断玩家是否死
					OnlineNetworkManager::sShareInstance()->sendPlayerAttackMessage(attackReq);
			}
			
		}
		
	}
	
	
}
