#include "GuildInstanceManager.h"
#include "LevelManager.h"
#include "LevelLayer.h"
#include "SkillDispMgr.h"
#include "MainLandManager.h"
#include "SpriteElfManager.h"
#include "SpriteMonsterMgr.h"
#include "InstanceManager.h"

#define AUTO_ATTACK_CD 0.2f

// 公会副本助战好友自动战斗代码
// PAIVE 战斗模式
// added by authurwang at 2014/09/18
GuildInstanceManager::GuildInstanceManager()
	:aiPlayer(NULL)
	,autoAttack(false)
	,autoAttackCDTime(0.0f)
	,m_isEnterFromGuildScene(false)
{
	aiPlayerId.setPlayId(0,0,0);
}

GuildInstanceManager::~GuildInstanceManager()
{
}

void GuildInstanceManager::setAIAutoAttack(bool autoAttack)
{	
	this->autoAttack = autoAttack;
	if (this->autoAttack)
	{
		LevelLayer * layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer && aiPlayerId.getID() != 0)
		{
			aiPlayer = layer->getPlayer(aiPlayerId);
			//aiPlayer->startAutoAttack();
		}
	}else{

		if (aiPlayer){
			//aiPlayer->endAutoAttack();
			aiPlayer = NULL;
		}
	}
}

void GuildInstanceManager::resetData()
{
	aiPlayer = NULL;
	autoAttack = false;
	aiPlayerId.setPlayId(0,0,0);
}

//重新设置技能
void GuildInstanceManager::setAISkillsId(vector<int> equipSkills)
{
	attackSkillVec.clear();
	buffSkillVec.clear();

	SkillDispMgr * skillMgr = SkillDispMgr::Get();

	if (aiPlayer)
	{
		int normalAttackId = skillMgr->GetNormalAttackID(aiPlayer->GetID(), aiPlayer->getType());
		attackSkillVec.push_back(normalAttackId);

		for (size_t i = 0; i < equipSkills.size(); i++)
		{
			//bool isUnlock = skillMgr->IsEquipSkill(skillTmp[i]);
			const SKILL_INFO * info = skillMgr->GetSkillInfo(equipSkills[i]);

			//普通攻击默认未解锁


			if (info->partID == 1)
			{
				/* if (isUnlock)*/
				{
					if (info->bBuff)
					{
						buffSkillVec.push_back(equipSkills[i]);
						aiPlayer->addBuffSkill(equipSkills[i]);
					}
					else
					{
						attackSkillVec.push_back(equipSkills[i]);
						aiPlayer->addAttackSkill(equipSkills[i]);
					}
				}
			}
		}
	}
}

void GuildInstanceManager::Update(float fTime)
{
	//自动攻击更新
	if (autoAttackCDTime < AUTO_ATTACK_CD)
	{
		autoAttackCDTime += fTime;
	}
	else if(aiPlayer)
	{

        if (aiPlayer->m_pStateMachine && aiPlayer->m_stateMachineRunning)
        {
            aiPlayer->m_pStateMachine->Update(fTime);

            VariableSpeedMoveToHandler* pMoveTo = dynamic_cast<VariableSpeedMoveToHandler*>(aiPlayer->m_pStateMachine->GetState(ToVariableSpeedMoveToHandler));
            if(pMoveTo)
            {
                if(pMoveTo)
                {
                    if(pMoveTo->IsFinish())
                    {
                        aiPlayer->m_pStateMachine->Remove(ToVariableSpeedMoveToHandler);
                        aiPlayer->beginAStarSearch();

                    }
                }
            }
        }
        aiPlayer->SetMoveType(MOVE_BY_MOUSE);
		AutoAttack();
	}

}

void GuildInstanceManager::AutoAttack()
{


	SpriteMonster * monster = SpriteMonsterMgr::Get()->getNearestMonster();
	int monsterCount = SpriteMonsterMgr::Get()->getMonsterCount();
	CCPoint portalPos = InstanceManager::Get()->getPortalPosition();
	if (monster)
	{
		attackToTarget(monster->getPosition());
	}
	else if (monsterCount == 0)
	{
		if (portalPos.x != 0 || portalPos.y !=0)
		{
			moveToPortal();
		}				
	}

	autoAttackCDTime = 0.0f;
}

void GuildInstanceManager::attackToTarget(CCPoint targetPos)
{	
	if(aiPlayer->IsDie())
	{
		return;
	}

	SkillDispMgr * skillMgr = SkillDispMgr::Get();

	int skillId = getBestSkill(targetPos);
	//CCLOG("-------- skillId : %d --------", skillId);
	if (skillId)
	{
		for (size_t i = 0; i < attackSkillVec.size(); i++)
		{
			if (skillId == attackSkillVec[i])
			{
				attackSkillVec.erase(attackSkillVec.begin() + i);
				attackSkillVec.push_back(skillId);
				break;
			}
		}

		for (size_t i = 0; i < buffSkillVec.size(); i++)
		{
			if (skillId == buffSkillVec[i])
			{
				buffSkillVec.erase(buffSkillVec.begin() + i);
				buffSkillVec.push_back(skillId);
				break;
			}
		}

		if (skillMgr->GetSkillInfo(skillId)->type != ToNormalAttack)
		{
			CCPoint vec = ccpSub(targetPos, aiPlayer->getPosition());
			if (vec.x == 0 && vec.y == 0)
			{
				aiPlayer->setDirection(aiPlayer->getDirectionVector());
			}
			else
			{
				vec = ccpNormalize(vec);
				aiPlayer->setDirection(vec);
			}			

			//瞬移特殊处理
			if (4046101 == skillId)
			{
				aiPlayer->setDirection(ccp(-vec.x, -vec.y));
			}
			if(!aiPlayer->DoSkillAttack(skillId) && aiPlayer->canMove)
			{
				aiPlayer->moveToTarget(targetPos, aiPlayer->getPosition());
			}
		}
		else
		{
			CCPoint vec = ccpSub(targetPos, aiPlayer->getPosition());
			if (vec.x == 0 && vec.y == 0)
			{
				aiPlayer->setDirection(aiPlayer->getDirectionVector());
			}
			else
			{
				vec = ccpNormalize(vec);
				aiPlayer->setDirection(vec);
			}	
			aiPlayer->DoNormalAttack();
		}
	}
	else if (targetPos.x > 0 && targetPos.y > 0)
	{
		aiPlayer->m_nNormalAttackNum = 0;
		CCPoint pt = targetPos;
		CCPoint pixPos = aiPlayer->getPosition();
		if (LevelManager::sShareInstance()->isPointReachable(pt))
		{
			aiPlayer->moveToTarget(pt, pixPos);
		}
		else
		{
			//需要支持当怪物在物理层时跑向怪物方向
			CCPoint vec = ccpSub(pt, pixPos);
			if (vec.x != 0 || vec.y != 0)
			{
				vec = ccpNormalize(vec);

				int count = 0;
				do
				{
					pt = ccpSub(pt, vec);
					++count;
					if (count > 2000)
					{
						break;
					}
				} while (!LevelManager::sShareInstance()->isPointReachable(pt));

				if (LevelManager::sShareInstance()->isPointReachable(pt))
				{
					aiPlayer->moveToTarget(pt, pixPos);
				}
			}
		}
	}

	return;

}

int GuildInstanceManager::getBestSkill(CCPoint targetPos)
{
	int skillId = 0;
	bool canDoSkill = false;
	int normalAttackId = SkillDispMgr::Get()->GetNormalAttackID(aiPlayerId, aiPlayer->getType());

	std::set<int> conflictSkillSet;
	std::vector<int> canDoSkillVec;

	//状态机为空时返回
	if (!aiPlayer->m_pStateMachine)
	{
		return skillId;
	}

	//检测连线上两点是否为物理点
	CCPoint nowPoint = aiPlayer->getPosition();
	CCPoint checkPoint1 = ccpAdd(nowPoint, ccp(ccpSub(targetPos, nowPoint).x / 3, ccpSub(targetPos, nowPoint).y / 3));
	CCPoint checkPoint2 = ccpAdd(nowPoint, ccp(ccpSub(targetPos, nowPoint).x * 2 / 3, ccpSub(targetPos, nowPoint).y * 2 / 3));
	if (LevelManager::sShareInstance()->isPointReachable(targetPos))
	{
		if (!LevelManager::sShareInstance()->isPointReachable(checkPoint1) || !LevelManager::sShareInstance()->isPointReachable(checkPoint2))
		{
			return skillId;
		}
	}

	SkillDispMgr * skillMgr = SkillDispMgr::Get();

	//获得正在释放的技能列表，以及与当前技能冲突的技能列表，可能有多个技能
	for (size_t i = 0; i < attackSkillVec.size(); i++)
	{
		const SKILL_INFO * info = skillMgr->GetSkillInfo(attackSkillVec[i]);
		if (!info)
		{
			continue;
		}
		if (aiPlayer->m_pStateMachine->CheckStateRuning(info->type))
		{
			conflictSkillSet.insert(info->skillConflict);
		}
	}

	//获取可以释放并在攻击距离内且不冲突的技能，攻击技能
	for (size_t i = 0; i < attackSkillVec.size(); i++)
	{
		const SKILL_INFO * info = skillMgr->GetSkillInfo(attackSkillVec[i]);
		if (!info)
		{
			continue;
		}

		if (ccpDistance(aiPlayer->getPosition(), targetPos) < info->longDistanceAttackRange)
		{
			if (attackSkillVec[i] == normalAttackId || !skillMgr->IsCD(aiPlayerId, attackSkillVec[i]))
			{
				canDoSkillVec.push_back(attackSkillVec[i]);
			}
		}	
	}

	//buff技能
	for (size_t i = 0; i < buffSkillVec.size(); i++)
	{
		const SKILL_INFO * info = skillMgr->GetSkillInfo(buffSkillVec[i]);
		if (!info)
		{
			continue;
		}

		if (!skillMgr->IsCD(aiPlayerId, buffSkillVec[i]))
		{
			if (conflictSkillSet.find(buffSkillVec[i]) == conflictSkillSet.end())
			{
				canDoSkillVec.push_back(buffSkillVec[i]);
			}	
		}		
	}

	//获得最高优先级技能
	int maxPriority = 0;
	for (size_t i = 0; i < canDoSkillVec.size(); i++)
	{
		const SKILL_INFO * info = skillMgr->GetSkillInfo(canDoSkillVec[i]);
		if (!info)
		{
			continue;
		}

		int curPriority = info->skillPriority;
		if (curPriority > maxPriority)
		{
			maxPriority = curPriority;
			skillId = canDoSkillVec[i];
		}		
	}

	return skillId;
}

void GuildInstanceManager::moveToPortal()
{
	CCPoint portalPos = InstanceManager::Get()->getPortalPosition();
	if (portalPos.x == 0 && portalPos.y ==0)
	{
		aiPlayer->moveToTarget(aiPlayer->getPosition(), aiPlayer->getPosition());
		return;
	}
	SEL_CallFunc cfActivePortal = callfunc_selector(SpriteSeer::activePortal);
	aiPlayer->moveToTarget(portalPos, aiPlayer->getPosition(), aiPlayer, cfActivePortal);
}
