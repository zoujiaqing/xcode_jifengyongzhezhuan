#include "SpriteSeer.h"
#include "SpriteSeerListener.h"
#include "LevelDefine.h"

//#include "Define.h"
#include "EnginePlayerManager.h"
#include "GameManager.h"
#include "ItemManager.h"

// UILayer
#include "MainMenuLayer.h"
#include "BattleUILayer.h"
#include "CrossPlatform.h"
#include "GameFontManager.h"
#include "BaseAttackState.h"
#include "EffectSprite.h"
#include "ParticleManager.h"
#include "SkillDispMgr.h"
#include "SpriteMonsterMgr.h"
#include "GameDataManager.h"
#include "InstanceManager.h"
#include "XLogicManager.h"
#include "GameAudioManager.h"
#include "SkeletonAnimRcsManager.h"
#include "GameResourceManager.h"
#include "SpriteFactory.h"
#include "MapDataManager.h"
#include "MainLandManager.h"
#include "UserData.h"
#include "CityListLayer.h"
#include "NpcSprite.h"
#include "BoneNPCManager.h"
#include "ItemUpdateManager.h"
#include "MathUtil.h"
#include "TimeManager.h"
#include "DieState.h"
#include "SpriteElfManager.h"
#include "Localization.h"
#include "LuaTinkerManager.h"
#include "PvAIManager.h"
#include "GMessage.h"
#include "../Map/ATLObjectLayer.h"
#include "UILayout.h"
#include "UserData.h"

#define AUTO_ATTACK_OFFSET 60.0f
#define AUTO_ATTACK_CD 0.2f
#define PORTAL_CHECK_CD 0.5f
#define PORTAL_CHECK_DISTANCE 100.0f

using namespace cocos2d;


SpriteSeer::SpriteSeer(int type, bool isPlayerSet)
	:BoneSpriteMoveable(type, isPlayerSet)	
	,m_nNormalAttackNum(0)
	,autoAttack(false)
	,autoAttackCDTime(0.0f)
	, m_bNormalAttack(true)
    ,attackedSoundCDTime(0)
	, m_acceleration(0.0f)
	, m_battleSide(0)
	, m_stateMachineRunning(true)
	,posInited(false)
	,portalCheckCDTime(0.0f)
	,m_nameLabel(NULL)
	, m_curByAttackVoiceTime(0)
	, m_curDieVoiceTime(0)
	,showActivePortalInfo(true)
	,m_vipPic(NULL)
	,touchStatus(false)
	,m_nAttachElfNum(0)
	,rageBasic1(-1)
	,rageBasic2(-1)
	,rageRate1(-1)
	,rageRate2(-1)
	,m_nameHeight(0)
	,m_wearTitle(NULL)
	,m_guildNameSprite(NULL)
{
     m_pStateMachine = new SkillStateMachine(); 
	 m_pStateFactory = new StateFactory(this);

	 ItemUpdateManager::Get()->addToUpdateManager(this);
}

void SpriteSeer::InitStateMachine()
{
	if(!m_pStateMachine->CheckStateRuning(ToIdle))
		m_pStateMachine->Push(m_pStateFactory->Create(ToIdle, 0));

	ItemUpdateManager::Get()->removeFromUpdateManager(this);
}

void SpriteSeer::ShutDownStateMachine()
{
	//m_pStateMachine->RemoveAllState();

}

SpriteSeer::~SpriteSeer()
{
	
	ShutDownStateMachine();
    delete m_pStateMachine;
	m_pStateMachine = NULL;

	if(m_pStateFactory)
	{
		delete m_pStateFactory;
		m_pStateFactory = NULL;
	}

	CC_SAFE_DELETE(m_map);
	// Note: 删除和角色绑定的精灵对象
	SpriteFactory::sharedFactory()->RemoveElfFromHero(m_ID,false);
	SpriteFactory::sharedFactory()->RemovePosElfFromHero(m_ID);
	// Note: 主角
	//if (m_ID == UserData::getUserId())
	{
		NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
	}
	
}

SpriteSeer* SpriteSeer::Clone(SpriteSeer* pSprite)
{
	SpriteSeer *seer = NULL;
	if (pSprite)
	{
		seer = new SpriteSeer(pSprite->m_TypeID, false);
		if (seer && seer->initWithId(pSprite->m_ID,pSprite->m_TypeID,pSprite->m_name.c_str(),false))
		{
			unsigned int weaponId = seer->GetEquipWeaponId();
			seer->SetEquipItemsData(weaponId);

			seer->autorelease();
			return  seer;
		}
		CC_SAFE_DELETE(seer);
		return NULL;
	}
}

SpriteSeer* SpriteSeer::seerWithID(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet,bool isCreateEffect)
{
	SpriteSeer *seer = new SpriteSeer(type, isPlayerSet);
	if (seer && seer->initWithId(s_id,type,name,isCreateEffect))
	{
		
		seer->autorelease();
		return  seer;
	}
	CC_SAFE_DELETE(seer);
	return NULL;
}

void SpriteSeer::ShowNameLabelColor(bool bRedName)
{
	if (m_nameLabel)
	{
		if (bRedName)
		{
			m_nameLabel->setColor(ccRED);
		}
		else
		{
			if(UserData::getUserId() == m_ID)
			{
				m_nameLabel->setColor(ccORANGE);
			}
			else
			{
				ccColor3B nameLabelColor = GetNameLabelColor();
				m_nameLabel->setColor(nameLabelColor);
			}
		}
	}
}

ccColor3B SpriteSeer::GetNameLabelColor()
{
	ccColor3B color = ccWHITE;

	if(UserData::getUserId() == m_ID)
	{
		long serverTime = TimeManager::Get()->getCurServerTime();
		if (UserData::GetUserInfo().m_pvpRedTimer - serverTime > 0)
		{
			color = ccRED;
		}
		else
		{
			color = ccORANGE;
		}
	}
	else
	{
		if(!MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId()) && 
			(LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel()|| LevelManager::sShareInstance()->isCurrentBestFighterLevel()))
		{
			USERINFO* info = UserData::Get()->GetUserInfo(m_ID);
			if(info)
			{
				USERINFO userInfo = UserData::GetUserInfo();
				if(abs((int)userInfo.battleSide - (int)info->battleSide) % 2 != 0)
				{
					color = ccRED;
				}
			}
		}
	}

	return color;
}

bool SpriteSeer::initWithId(PLAYER_ID id,int typeId,const char *name,bool isCreateEffect)
{
	m_name = std::string(name);
	m_ID = id;
	m_TypeID = typeId;
	
	m_animBone =  EnginePlayerManager::getInstance()->addBoneAnimSprite(typeId);
	if (m_animBone == 0)
	{
		return false;
	}
	if(isCreateEffect)
	{
		m_effectSprite = ParticleManager::Get()->createEffectSprite(103,"");
		m_rootSprite->addChild(m_effectSprite,m_rootSprite->getZOrder()+20);
		m_effectSprite->setPosition(getAttachPoint(kType_ATTACK_HURT));
		m_effectSprite->setVisible(false);
	}
	attachBoneAnimToRootSprite(1);

	float labelFontSize = GameFontManager::smallFontSize();
	//float labelFontSize = 20;
    //CCSize nameLabelSize = CCSizeMake(150 / CC_CONTENT_SCALE_FACTOR(), labelFontSize * 1.5);
    //m_nameLabel = UILabelTTF::create(name, KJLinXin, labelFontSize, nameLabelSize, kCCTextAlignmentCenter);
	CCString* str = CCString::createWithFormat(" %s",name);
	m_nameLabel = UILabelTTF::create(str->m_sString.c_str(),KJLinXin,labelFontSize);
    if(m_nameLabel)
    {
		m_nameLabel->SetDepthTest(false);
        m_nameLabel->setPosition(getAttachPoint(kType_PLAYER_NAME));
		m_nameLabel->setVertexZ(-LevelLayer::sCalcDepth(MAP_Z_OBJ_TOP));
        m_rootSprite->addChild(m_nameLabel);

		ccColor3B nameLabelColor = GetNameLabelColor();
		m_nameLabel->setColor(nameLabelColor);
		m_nameHeight = m_nameLabel->getContentSize().height;
    }

	resetVipMark();//add vip mark
	resetGuildName();
	resetWearTitle();

	// init state machine
	InitStateMachine();
	GameDataManager * mgr = GameDataManager::Get();
	HSJoystick * stick = HSJoystick::sharedJoystick();
	if (mgr)
	{		
		bool isMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
		SetMoveType(mgr->getHeroMoveType());
		if (isMainLandLevel)
		{
			stick->setIsEnable(false);
			m_eMoveType = MOVE_BY_MOUSE;

			//主城中停止自动攻击
			mgr->setHeroAutoAttack(false);
		}	
		else
		{
			stick->setIsEnable(m_eMoveType == MOVE_BY_MOUSE ? false : true);
		}
	}

	//初始化技能vector
	SkillDispMgr * skillMgr = SkillDispMgr::Get();

	vector<int> skillTmp = skillMgr->GetAllSkillID((RoleJobType)getType());

	//手动添加普通攻击id
	int normalAttackId = skillMgr->GetNormalAttackID(GetID(), getType());
	attackSkillVec.push_back(normalAttackId);
	for (size_t i = 0; i < skillTmp.size(); i++)
	{
		bool isUnlock = skillMgr->IsEquipSkill(skillTmp[i]);
		const SKILL_INFO * info = skillMgr->GetSkillInfo(skillTmp[i]);



		if (info->partID == 1)
		{
			if (isUnlock)
			{
				if (info->bBuff)
				{
					buffSkillVec.push_back(skillTmp[i]);
				}
				else
				{
					attackSkillVec.push_back(skillTmp[i]);
				}
			}
		}
	}
	

	//初始化自己的physicsLayer
    m_map = createSearchLayer();
	
	portalPosition = InstanceManager::Get()->getPortalPosition();

	// Note: 主角
	if (m_ID == UserData::getUserId())
	{
		unsigned int weaponId = ItemManager::Get()->getHeroWeaponId();
		this->SetEquipItemsData(weaponId);

		NotificationCenter::defaultCenter()->registerAllMsgObserver(this);//只要主角才监听
	}

	
    return  true;
}



void SpriteSeer::startMove()
{   
	switch(m_eMoveType)
	{
	case MOVE_BY_JOY:
		if (m_step < m_map->getPathTop()) 
		{
			//BoneSpriteMoveable::startMove();
			beginAStarSearch();
		}
		else
		{
			if(m_step != 0 && m_map->getPathTop() != 0)
			{
				//BoneSpriteMoveable::endMove();
				endAStarSearch();
			}
		}
		
		break;

	case MOVE_BY_MOUSE:
		//if (m_step < m_map->getPathTop()) 
		//{       
		//	//---------------------------------------------------------------------------------
		//	// diff logic for player seer and other seers
		//	//---------------------------------------------------------------------------------
		//	BaseListener * baseListener = LevelLayer::sGetListenerByTag(getTag());
		//	if (baseListener)
		//	{
		//		SpriteSeerListener* listener =  dynamic_cast<SpriteSeerListener*>(baseListener);
		//		if (listener)
		//		{
		//			listener->HandleObjectMoveEvent(this);
		//		}		 
		//	}
		//}

		////
		//BoneSpriteMoveable::startMove();
		beginAStarSearch();
		if (m_isMoving)
		{
			startPathFinding();
		}

		break;
	}
}


void SpriteSeer::endMove()
{
	switch(m_eMoveType)
	{
	case MOVE_BY_JOY:
		//BoneSpriteMoveable::endMove();
		endAStarSearch();
		break;

	case MOVE_BY_MOUSE:
		//---------------------------------------------------------------------------------
		// diff logic for player seer and other seers
		//---------------------------------------------------------------------------------
		BaseListener * baseListener = LevelLayer::sGetListenerByTag(getTag());
		if (baseListener)
		{
			SpriteSeerListener* listener =  dynamic_cast<SpriteSeerListener*>(baseListener);
			if (listener)
			{
				listener->HandleObjectEndMoveEvent(this);
			}
		}
		//
		//BoneSpriteMoveable::endMove();	
		endAStarSearch();

		if (!autoAttack)
		{
			endPathFinding();
		}
		
		break;
	}

}

void SpriteSeer::beginAStarSearch()
{
	if (m_colliderRect.size.width != 0 && m_colliderRect.size.height != 0)
	{
		CCRect selfColliderRect = this->getRelativeParentColliderRect();
		if (selfColliderRect.intersectsRect(m_colliderRect))
		{
			m_rootSprite->stopAllActions();
			m_map->setStart(getPosition());

			endMove();
			return ;
		}
	}
	if (m_map && m_step < m_map->getPathTop()) 
	{
		float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();

		CCPoint pos = m_map->getPathPos(m_step);
		float Speed = GetMoveSpeed();

		updateDirection(pos);
		updateRunAnim();
		CCPoint pt1 = this->getPosition();
		

		CCPoint destPoint = CCPoint(pos.x / deviceScale, pos.y / deviceScale);
		    
		m_rootSprite->stopAllActions();

		if(m_pStateMachine && m_pStateFactory)
		{
			VariableSpeedMoveToHandler* pMoveTo = dynamic_cast<VariableSpeedMoveToHandler*>(m_pStateMachine->GetState(ToVariableSpeedMoveToHandler));
			if(pMoveTo == NULL)
			{
				pMoveTo = dynamic_cast<VariableSpeedMoveToHandler*>(m_pStateFactory->Create(ToVariableSpeedMoveToHandler, -1));
				m_pStateMachine->Push(pMoveTo);
				pMoveTo->SetTarget(this, destPoint, Speed);
			}
			else
			{
				pMoveTo->ChangeDstPos(destPoint);
			}
		}
		
		

		m_map->setStart(getPosition());
		m_step++;
	}
	else
	{  
		m_rootSprite->stopAllActions();
		m_map->setStart(getPosition());

		endMove();
	}
}

void SpriteSeer::endAStarSearch()
{
	m_isMoving = false;

	updateWaitAnim();

	//
	if (m_Target && m_func)
	{
		(m_Target->*m_func)();
	}

	m_Target = NULL;
	m_func = NULL;

	//m_fMoveRate = 2.0f;
	m_fAdditionMoveRate = 0.0f;
}
//
//Machtimer machTimer;
//CCPoint g_p1;
//CCPoint g_p2;
//int g_count = 0;
//float speedCount = 0.0f;

int SpriteSeer::GetFairyBasicRage(int pos)
{
	return pos == 1 ? rageBasic1 : rageBasic2;
}

int SpriteSeer::GetFairyRageRate(int pos)
{
	return pos == 1 ? rageRate1 : rageRate2;
}

void SpriteSeer::Update(float fTime)
{
	//初始化精灵初始怒气及增长率
	if (rageBasic1 == -1)
	{
		rageBasic1 = SpriteElfManager::Get()->GetFairyRageBasicByPos(m_ID, SpriteElfManager::E_Fight_Left);
		rageBasic2 = SpriteElfManager::Get()->GetFairyRageBasicByPos(m_ID, SpriteElfManager::E_Fight_Right);
		rageRate1 = SpriteElfManager::Get()->GetFairyRageRateByPos(m_ID, SpriteElfManager::E_Fight_Left);
		rageRate2 = SpriteElfManager::Get()->GetFairyRageRateByPos(m_ID, SpriteElfManager::E_Fight_Right);
	}

	USERINFO * info = UserData::Get()->GetUserInfo(m_ID);
	bool isMainLand = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if (info && !isMainLand && !info->rageBasicInited && rageBasic1 > 0)
	{
		info->ragePoint += rageBasic1;
		info->secRagePoint += rageBasic2;
		info->rageBasicInited = true;
	}



	if (!posInited)
	{
		if (getPosition().x > 0)
		{
			posInited = true;
			TaskPosition taskPos = GameDataManager::Get()->getTaskPosition();
			PLAYER_ID heroId = UserData::Get()->getUserId();
			if (m_ID == heroId && taskPos.cityId)
			{
				//moveToCityPosition(taskPos.cityId, taskPos.position);
				moveToCityPosition(taskPos.cityId,taskPos.position,taskPos.npcId);
			}
		}
	}

    if (m_bInRedState)
	{
		m_InRedTime += fTime;
		if (m_InRedTime > 0.1f)
		{
			m_InRedTime = 0;
			revertToOriginColor();
		}
	}
	
	if (m_pStateMachine && m_stateMachineRunning)
	{
		m_pStateMachine->Update(fTime);

		VariableSpeedMoveToHandler* pMoveTo = dynamic_cast<VariableSpeedMoveToHandler*>(m_pStateMachine->GetState(ToVariableSpeedMoveToHandler));
		if(pMoveTo)
		{
			if(pMoveTo)
			{
				if(pMoveTo->IsFinish())
				{
					m_pStateMachine->Remove(ToVariableSpeedMoveToHandler);
					beginAStarSearch();
					
				}
			}
		}
	}

	//自动攻击更新
	if (autoAttackCDTime < AUTO_ATTACK_CD)
	{
		autoAttackCDTime += fTime;
	}
	else if (autoAttack)
	{
		bool isInPvAI = LevelManager::sShareInstance()->isCurrentPVAILevel();
		bool isPVP = LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel();
		bool isBestFighter = LevelManager::sShareInstance()->isCurrentBestFighterLevel();
		
		if (isInPvAI || isPVP || isBestFighter)
		{
			LevelLayer * levellayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
			SpriteSeer * aiPlayer = levellayer->getPlayer(PvAIManager::Get()->getAIPlayerId());
			
			if (aiPlayer && aiPlayer->GetRoleOpacity() > 0)
			{
				attackToTarget(aiPlayer->getPosition());
			}
		}
		else
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
		}

		autoAttackCDTime = 0.0f;
	}

	//传送点检测更新
	if (portalCheckCDTime < PORTAL_CHECK_CD)
	{
		portalCheckCDTime += fTime;
	}
	else if (m_eMoveType == MOVE_BY_JOY)
	{
		portalCheckCDTime = 0;
		int monsterCount = SpriteMonsterMgr::Get()->getMonsterCount();
		float distance = ccpDistance(portalPosition, getPosition());
		if (0 == monsterCount && distance < PORTAL_CHECK_DISTANCE)
		{
			activePortal();
		}
	}

    if(attackedSoundCDTime > 0)
    {
        attackedSoundCDTime = (attackedSoundCDTime > fTime) ? (attackedSoundCDTime - fTime) : 0 ;
    }

	if( GetID() == UserData::getUserId())
	{
		if(m_pStateMachine)
		{
			if(!m_pStateMachine->CheckStateRuning(ToNormalAttack))
			{
				tellIsAttackEnable();

			}

			//副本中传送门开启检测
			bool isMainLand = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
			if (!isMainLand)
			{
				if(LevelManager::sShareInstance()->isCurrentPPVELevel() ||
				LevelManager::sShareInstance()->isCurrentEliteLevel() ||
				LevelManager::sShareInstance()->isCurrentPVELevel() || 
				LevelManager::sShareInstance()->isCurrentUndergroundLevel())
				{
					if (SpriteMonsterMgr::Get()->getMonsterCount() == 0)
					{
						CCPoint portalPos = InstanceManager::Get()->getPortalPosition();
						if (portalPos.x != 0 || portalPos.y !=0)
						{
							if (showActivePortalInfo)
							{
								showActivePortalInfo = false;
								ShowMessage(Localizatioin::getLocalization("M_PORTAL_OPEN"));
							}	
						}	
					}
				}
			}			
		}
	}

	
	

}

void SpriteSeer::PauseStateMachine()
{
	m_stateMachineRunning = false;	
}

void SpriteSeer::ResumeStateMachine()
{
	m_stateMachineRunning = true;
}

void SpriteSeer::ExistStateMachine()
{
	if (m_pStateMachine)
	{
		m_pStateMachine->Exit();
	}	
}

bool SpriteSeer::tellIsAttackEnable()
{
	bool ret = false;
	SceneLayer* layer = GameManager::Get()->GetSceneLayer();
	if(layer)
	{
		MainMenuLayer* uiLayer = layer->getMainMenuLayer();
		if(uiLayer)
		{
			int battleTag = uiLayer->getLayerTagByName("BattleUILayer");
			BattleUILayer* battleUI = uiLayer->findUILayerByTag<BattleUILayer>(battleTag);
			if (battleUI && battleUI->getNormalAttackBtn() && battleUI->getNormalAttackBtn()->isSelected())
			{
				ret = true;

				SpriteSeer* hero = this;
				if (hero)
				{
					CCLOG("tellIsAttackEnable");
					;
					unsigned int skillID = 0;
					skillID = SkillDispMgr::Get()->GetNormalAttackID(GetID(), getType());
					if(skillID != 0)
						hero->DoNormalAttack(skillID);
				}
			}
		}
	}
	return ret;
}

bool SpriteSeer::DoNormalAttack(unsigned int skillId)
{
	if(IsDie())
	{
		return false;
	}

	//CCLOG("%s, skillID : %ld, Time : %u", __FUNCTION__, skillId, time(NULL));

	m_nNormalAttackNum++;
	//printf("m_nNormalAttackNum : %ld\n", m_nNormalAttackNum);
	if(m_pStateMachine->CheckStateRuning(ToNormalAttack))
	{
		return false;
	}
	if (!m_bNormalAttack)
	{
		return false;
	}

	m_pStateMachine->Remove(ToRun);
	m_pStateMachine->Remove(ToIdle);
    
	m_nNormalAttackNum = 1;
	m_pStateMachine->Push(m_pStateFactory->Create(ToNormalAttack, skillId));
	return true;
}

bool SpriteSeer::DoNormalAttack( void )
{
	//unsigned int skillID = 0;
	//switch(getType())
	//{
	//case kTypeFighter_Boy:
	//case kTypeFighter_Girl:
	//	skillID = 4011101;	// 战士
	//	break;
	//case kTypeMagician_Girl:
	//case kTypeMagician_Boy:
	//	skillID = 4041101;	// 法师
	//	break;
	//case kTypeAssassin_Boy:
	//case kTypeAssassin_Girl:
	//	skillID = 4051101;	// 刺客
	//	break;
	//default:
	//	break;
	//}

	unsigned int skillID = SkillDispMgr::Get()->GetNormalAttackID(this->GetID(), this->getType());

	if(skillID != 0)
		DoNormalAttack(skillID);

	return true;

}

bool SpriteSeer::DoSkillAttack(unsigned int skillId)
{
	if(IsDie())
	{
		return false;
	}

	//CCLOG("%s, skillID : %ld, Time : %u", __FUNCTION__, skillId, time(NULL));


	if(!SkillDispMgr::Get()->IsCD(GetID(), skillId))
	{
		SkillDispMgr::Get()->SetCD(GetID(), skillId);
        
		m_pStateMachine->Remove(ToRun);
		m_pStateMachine->Remove(ToIdle);
		m_pStateMachine->Remove(ToNormalAttack);

		const SKILL_INFO* pSkillInfo = SkillDispMgr::Get()->GetSkillInfo(skillId);
		/*if (m_pStateMachine->CheckStateRuning(pSkillInfo->type))
		{
			return false;
		}*/
		m_pStateMachine->Push(m_pStateFactory->Create(pSkillInfo->type, skillId));
        
        if(autoAttack)
        {
            /*MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
            BattleUILayer* battlayer = mLayer->GetBattleUILayer();*/
			int battleTag = MainMenuLayer::getLayerTagByName("BattleUILayer");
			BattleUILayer* battleLayer = MainMenuLayer::showUILayerByTag<BattleUILayer>(battleTag);
			if(battleLayer)
			{
				battleLayer->autoSkillAttack(skillId);
			}
        }
		return true;

	}
	
	return false;
}

void SpriteSeer::ByAttacked(unsigned int blood, unsigned int ragePoint, bool isCrit,bool isFairyAttack/* = false*/)
{	
	const unsigned int VOICE[7][3] =
	{
		{0, 0, 0},

		{3211010, 3211011, 3211012},
		{3211210, 3211211, 3211212},

		{3211310, 3211311, 3211312},
		{3211410, 3211411, 3211412},

		{3211510, 3211511, 3211512},
		{3211610, 3211611, 3211612},
	};

	if(m_effectSprite)
	{
		m_effectSprite->SetAnim(kType_Play,1,false);
	}

	bool isHarmful = true;

	USERINFO* info = UserData::Get()->GetUserInfo(m_ID);
	if(info)
	{
		USERINFO* pHeroInfo = UserData::Get()->GetUserInfo(GameManager::Get()->getHero()->GetID());
		if(info->battleSide % 2 == pHeroInfo->battleSide % 2)
		{
			isHarmful = false;
		}
		
		//if(this->getTag() == MAP_TAG_SEER)
		{
			if(ragePoint < 1)
				ragePoint = 1;
			int localRage1 = 1;
			int localRage2 = 1;
			if (rageRate1 > 0)
			{
				localRage1 = (1 + rageRate1 * 1.0f / 100);
			}

			if (rageRate2 > 0)
			{
				localRage2 = (1 + rageRate2 * 1.0f / 100);
			}

			info->ragePoint += ragePoint * localRage1;
			info->secRagePoint += ragePoint * localRage2;

			if(info->ragePoint > info->totalRangePoint)
			{
				info->ragePoint = info->totalRangePoint;
			}
			if (info->secRagePoint > info->secTotalRangePoint)
			{
				info->secRagePoint = info->secTotalRangePoint;
			}
		}
	}
	CCPoint damagePoint = ccpAdd(this->getPosition(),this->getAttachPoint(kType_PLAYER_NAME));
	GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleDamageNumberEffect(damagePoint, blood, isCrit, isHarmful,isFairyAttack);

	playBecomeRedEffect(false);
	
		unsigned int soundID = 321013;
		switch(getType())
		{
		case kTypeFighter_Boy:
		case kTypeFighter_Girl:
			//skillID = 4011101;	// 战士
			break;
		case kTypeMagician_Girl:
		case kTypeMagician_Boy:
			soundID = 323004;	// 法师
			break;
		case kTypeAssassin_Boy:
		case kTypeAssassin_Girl:
			//skillID = 4051101;	// 刺客
			break;
		default:
			break;
		}
	
    if(attackedSoundCDTime == 0)
    {
        GameAudioManager::sharedManager()->playEffect(soundID, false);
        attackedSoundCDTime = 0.5f;
    }
	
	if(getTag() == MAP_TAG_SEER)
	{
		if(TimeManager::Get()->getCurServerTime() - m_curByAttackVoiceTime > 1)
		{
		
			// 语音
			unsigned int voiceID = 0;

			unsigned int index = ranged_random(0, 4);
			if(index < 3)
			{
				voiceID = VOICE[getType()][index];
				GameAudioManager::sharedManager()->playEffect(voiceID, false);
				m_curByAttackVoiceTime = TimeManager::Get()->getCurServerTime();

			}
		}

	
		//if(UserData::GetUserInfo().health_point <= 0)
		//{
		//	Die();

		//	//fix BUG #101412
		//	GameDataManager::Get()->setHeroAutoAttack(false);

		//	MainMenuLayer* mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		//	int battleTag = mLayer->getLayerTagByName("BattleUILayer");
		//	BattleUILayer* layer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
		//	if(layer)
		//	{
		//		layer->clearSkillProgressTimer();
		//	}
		//}
		
	}
}

bool SpriteSeer::MoveToByJoystick(float dt)
{
	float speed = GetMoveSpeed();

	CCPoint velocity = HSJoystick::sharedJoystick()->getVelocity();
	if(velocity.x == 0 && velocity.y == 0)
	{
		m_isMoving = false;		
		updateWaitAnim();
		return false;
	}
	m_isMoving = true;
	float dx = speed * velocity.x * dt ;
	float dy = speed * velocity.y * dt ;

	CCPoint curPos = this->getPosition();
	CCPoint newPos = ccpAdd(curPos, ccp(dx,dy));

	updateDirection(newPos);
	updateRunAnim();

	//m_map = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getPhysicsLayer();	
	if(m_map && m_map->isPointReachable(newPos))
	{
		this->setPosition(newPos);
	}
	else if(m_map && m_map->isPointReachable(ccpAdd(curPos, ccp(dx,0))))
	{
		this->setPosition(ccpAdd(curPos, ccp(dx,0)));
	}
	else if(m_map && m_map->isPointReachable(ccpAdd(curPos, ccp(0,dy))))
	{
		this->setPosition(ccpAdd(curPos, ccp(0,dy)));
	}	

	return true;
}


bool SpriteSeer::moveByJoystick( float dt )
{
	if (m_eMoveType == MOVE_BY_JOY)
	{
		moveToTarget(getPosition(), getPosition());
	}
	
	if (m_pStateMachine->CheckStateRuning(ToNormalAttack) ||
		m_pStateMachine->CheckStateRuning(ToLinkTime))
	{
		return false;
	}

	int tag = MainMenuLayer::getLayerTagByName("BattleUILayer");
	BattleUILayer* battleUI = MainMenuLayer::showUILayerByTag<BattleUILayer>(tag,false);
	if (battleUI && battleUI->getNormalAttackBtn()->isSelected())
	{
		return false;
	}

	if(BoneSpriteMoveable::moveByJoystick(dt))
	{
		return true;
	}
	else
	{
		if (m_pStateMachine->CheckStateRuning(ToRun))
		{
			
			FState* pState = m_pStateMachine->GetState(ToRun);
			if(pState)
				m_pStateMachine->Remove(pState);

			if(m_pStateMachine->CheckStateRuning(ToIdle))
			{
				m_pStateMachine->Remove(m_pStateMachine->GetState(ToIdle));
			}
			m_pStateMachine->Push(m_pStateFactory->Create(ToIdle, 0));
		}

		return false;
	}

	return false;
	
}

void SpriteSeer::DoRun( void )
{
	if(IsDie())
	{
		return;
	}

    if(false == m_stateMachineRunning)
        return;
    
	CCPoint velocity = HSJoystick::sharedJoystick()->getVelocity();
	if(velocity.x == 0 && velocity.y == 0)
	{
		return;
	}

	int tag = MainMenuLayer::getLayerTagByName("BattleUILayer");
	BattleUILayer* battleUI = MainMenuLayer::showUILayerByTag<BattleUILayer>(tag,false);
	if (battleUI && battleUI->getNormalAttackBtn()->isSelected())
	{
		return;
	}

	if(m_pStateMachine->CheckStateRuning(ToIdle))
	{
		if (canMove)
		{
			m_pStateMachine->Remove(ToRun);
			m_pStateMachine->Remove(ToIdle);
			m_pStateMachine->Push(m_pStateFactory->Create(ToRun, 0));
		}
	}
	
}

void SpriteSeer::startPathFinding()
{
	if(IsDie())
	{
		return;
	}

	if (!m_pStateMachine->CheckStateRuning(ToRun) && m_pStateMachine->CheckStateRuning(ToIdle))
	{
		m_pStateMachine->Remove(ToIdle);
		m_pStateMachine->Push(m_pStateFactory->Create(ToRun, 0));
	}
}

void SpriteSeer::endPathFinding()
{
	if(m_pStateMachine->CheckStateRuning(ToRun))
	{
		m_pStateMachine->Remove(ToRun);
		m_pStateMachine->Push(m_pStateFactory->Create(ToIdle, 0));
	}
}

void SpriteSeer::startAutoAttack()
{
	if (autoAttack)
	{
		return;
	}
	autoAttack = true;
	m_eMoveType = MOVE_BY_MOUSE;
}

void SpriteSeer::endAutoAttack()
{
	if (!autoAttack)
	{
		return;
	}
	autoAttack = false;
	m_eMoveType = GameDataManager::Get()->getHeroMoveType();
	
	this->moveToTarget(getPosition(), getPosition());
}

void SpriteSeer::attackToTarget(CCPoint targetPos)
{	
	if(IsDie())
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
			CCPoint vec = ccpSub(targetPos, this->getPosition());
			if (vec.x == 0 && vec.y == 0)
			{
				setDirection(getDirectionVector());
			}
			else
			{
				vec = ccpNormalize(vec);
				setDirection(vec);
			}			
			
			//瞬移特殊处理
			if (4046101 == skillId)
			{
				setDirection(ccp(-vec.x, -vec.y));
			}
			if(!DoSkillAttack(skillId) && canMove)
			{
				this->moveToTarget(targetPos, this->getPosition());
			}
		}
		else
		{
			CCPoint vec = ccpSub(targetPos, this->getPosition());
			if (vec.x == 0 && vec.y == 0)
			{
				setDirection(getDirectionVector());
			}
			else
			{
				vec = ccpNormalize(vec);
				setDirection(vec);
			}	
			DoNormalAttack();
		}
	}
	else if (targetPos.x > 0 && targetPos.y > 0)
	{
		m_nNormalAttackNum = 0;
		CCPoint pt = targetPos;
		CCPoint pixPos = this->getPosition();
		if (m_map->isPointReachable(pt))
		{
			this->moveToTarget(pt, pixPos);
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
				} while (!m_map->isPointReachable(pt));

				if (m_map->isPointReachable(pt))
				{
					this->moveToTarget(pt, pixPos);
				}
			}
		}
	}

	return;

}

int SpriteSeer::getBestSkill(CCPoint targetPos)
{
	int skillId = 0;
	bool canDoSkill = false;
	int normalAttackId = SkillDispMgr::Get()->GetNormalAttackID(GetID(), getType());

	std::set<int> conflictSkillSet;
	std::vector<int> canDoSkillVec;

	//状态机为空时返回
	if (!m_pStateMachine)
	{
		return skillId;
	}

	//检测连线上两点是否为物理点
	CCPoint nowPoint = this->getPosition();
	CCPoint checkPoint1 = ccpAdd(nowPoint, ccp(ccpSub(targetPos, nowPoint).x / 3, ccpSub(targetPos, nowPoint).y / 3));
	CCPoint checkPoint2 = ccpAdd(nowPoint, ccp(ccpSub(targetPos, nowPoint).x * 2 / 3, ccpSub(targetPos, nowPoint).y * 2 / 3));
	if (m_map->isPointReachable(targetPos))
	{
		if (!m_map->isPointReachable(checkPoint1) || !m_map->isPointReachable(checkPoint2))
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
		if (m_pStateMachine->CheckStateRuning(info->type))
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

		if (ccpDistance(this->getPosition(), targetPos) < info->longDistanceAttackRange)
		{
			if (attackSkillVec[i] == normalAttackId || !skillMgr->IsCD(GetID(), attackSkillVec[i]))
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

		if (!skillMgr->IsCD(GetID(), buffSkillVec[i]))
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

	CCLOG("---- skillId : %d -----",skillId);

	return skillId;
}

void SpriteSeer::moveToPortal()
{
	CCPoint portalPos = InstanceManager::Get()->getPortalPosition();
	if (portalPos.x == 0 && portalPos.y ==0)
	{
		moveToTarget(getPosition(), getPosition());
		return;
	}
	SEL_CallFunc cfActivePortal = callfunc_selector(SpriteSeer::activePortal);
	this->moveToTarget(portalPos, this->getPosition(), this, cfActivePortal);
}

void SpriteSeer::activePortal()
{
	//世界boss中直接进入地图，若有更合适的地方考虑移动这段代码
	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS))
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/WorldBossMgr.lua", "OnPressWorldBossPortal");
		return;
	}

	if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE))
	{	
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			mLayer->ShowGuildInstanceListLayer(true);
		}
		return;
	}

	XLogicManager::sharedManager()->setMessageStatus(XLogicManager::E_NONE);

	int instanceId = InstanceManager::Get()->getCurInstanceId();
	int portalId = InstanceManager::Get()->getPortalId();

	//ASSERT(instanceId != 0, "SpriteSeer: Instance Id not init yet.");
	//ASSERT(portalId != 0, "SpriteSeer: Portal Id not init yet.");
	if (instanceId != 0 && portalId != 0)
	{
		XLogicManager::sharedManager()->prepareBattle(instanceId,portalId);
	}
}

float SpriteSeer::GetCurAnimTotalTime( void )
{
	return SkeletonAnimRcsManager::getInstance()->getOneAnimationDuration(m_TypeID, GetAnimID());
}

cocos2d::CCPoint SpriteSeer::GetJoystickVelocity( void )
{
	return HSJoystick::sharedJoystick()->getVelocity();
}

void SpriteSeer::setPosition( const CCPoint &position )
{
	// test only
	if(m_pStateMachine)
	{
		if(m_pStateMachine->CheckStateRuning(ToIdle))
		{
			int a;
			a = 10;
		}
	}

	BoneSpriteMoveable::setPosition(position);
}

void SpriteSeer::setLastPosition(CCPoint pt)
{
	m_ptLastServerPosition = pt;
}

CCPoint SpriteSeer::getLastPosition()
{
	return m_ptLastServerPosition;
}

void SpriteSeer::stopAllActions( void )
{
	BoneSpriteMoveable::stopAllActions();
	m_rootSprite->stopAllActions();
	if(m_pStateMachine)
	{
		m_pStateMachine->Remove(ToVariableSpeedMoveToHandler);
	}
}

CPhysicsLayer* SpriteSeer::createSearchLayer()
{
	if (GameManager::Get()->GetSceneLayer())
	{
		CPhysicsLayer* layer = new CPhysicsLayer();
		int id = GameManager::Get()->GetSceneLayer()->getCurMapId();
		BBYTE * file = (unsigned char*)MapDataManager::sharedManager()->getMapFileByMapID(id);

		int cellW = 0;
		memcpy(&cellW, file, sizeof(int));
		file += sizeof(int);
		int row = 0;
		memcpy(&row, file, sizeof(int));
		file += sizeof(int);
		int col = 0;
		memcpy(&col, file, sizeof(int));
		file += sizeof(int);

		layer->init(file, row, col, cellW, cellW);
		return layer;
	}else
	{
		return NULL;
	}
}

void SpriteSeer::moveToCityPosition(int cityId, CCPoint position,int npcId /* = -1 */)
{
	CCLOG("auto find road step 8 move to city pos");
	if (!MainLandManager::Get()->isMainLand(cityId))
	{
		CCLOG("auto find road step 9");
		return;
	}

	if (MainLandManager::Get()->getCurCityId() == cityId)
	{
		CCLOG("auto find road step 10 ");
		//在当前主城
		if (npcId == -1)
		{
			CCLOG("auto find road step 11 ");
			moveToTarget(position, getPosition());
		}
		else
		{
			CCLOG("auto find road step 12 ");
			// Note: 获取当前的Npc
			SpriteNPC* pNpc = BoneNPCManager::getInstance()->GetOneNpc(npcId);
			if (pNpc)
			{
				CCLOG("auto find road step 13 ");
				CCPoint targetPos = position;
				pNpc->MoveToTarget(targetPos,pNpc->getRelativeParentColliderRect());
			}

		}		

		TaskPosition taskPos;
		taskPos.cityId = 0;
		taskPos.npcId = 0;
		taskPos.position = ccp(0, 0);
		GameDataManager::Get()->setTaskPosition(taskPos);
	}
	else
	{
		CCLOG("auto find road step 15 ");
		//其他主城
		TaskPosition taskPos;
		taskPos.cityId = cityId;
		taskPos.position = position;
		taskPos.npcId = npcId;
		GameDataManager::Get()->setTaskPosition(taskPos);
		CityListLayer * layer = MainMenuLayer::getUILayerByTag<CityListLayer>(MainMenuLayer::getLayerTagByName("CityListLayer"));
		if (layer)
		{
			CCLOG("auto find road step 16 ");
			layer->openCityList();
			layer->moveToCity(cityId, 0.5f);
		}
	}
}

void SpriteSeer::resetNameLabelColor(const ccColor3B& color)
{
	if(m_nameLabel)
	{
		m_nameLabel->setColor(color);
	}
}

void SpriteSeer::Die( void )
{
	if(IsDie())
	{
		return;
	}
	

	const unsigned int VOICE[7] =
	{
		0,

		3211009,
		3211209,

		3211309,
		3211409,

		3211509,
		3211609,
	};

	if(getTag() == MAP_TAG_SEER)
	{
		if(TimeManager::Get()->getCurServerTime() - m_curDieVoiceTime > 1)
		{

			// 语音
			unsigned int voiceID = 0;

			voiceID = VOICE[getType()];
			GameAudioManager::sharedManager()->playEffect(voiceID, false);
			m_curDieVoiceTime = TimeManager::Get()->getCurServerTime();

		}

		//fix BUG #101412
		GameDataManager::Get()->setHeroAutoAttack(false);

		MainMenuLayer* mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		int battleTag = mLayer->getLayerTagByName("BattleUILayer");
		BattleUILayer* layer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
		if(layer)
		{
			layer->clearSkillProgressTimer();
		}
	}

	if(m_pStateMachine)
	{
		// 清除所有的其它状态
		m_pStateMachine->Exit();

		m_pStateMachine->Push(m_pStateFactory->Create(ToDie, 0));
	}
}

void SpriteSeer::Revival( void )
{
	USERINFO* pInfo = UserData::Get()->GetUserInfo(this->GetID());
	if(pInfo)
	{
		BATTLEINFO* pBattleInfo = (BATTLEINFO*)pInfo;
		pBattleInfo->health_point = pBattleInfo->total_hp;
	}

	if(m_pStateMachine)
	{
		// 清除所有的其它状态
		m_pStateMachine->Exit();

		m_pStateMachine->Push(m_pStateFactory->Create(ToReviveInvincible, 4999999));
	}
}

bool SpriteSeer::IsDie( void )
{
	if(m_pStateMachine)
	{
		return m_pStateMachine->CheckStateRuning(ToDie);
	}

	return false;
}

void SpriteSeer::MoveToSpriteTranspoint(bool bNormalTranspoint)
{
	SpriteTransPoint* tranpoint = NULL;
	if(bNormalTranspoint)
	{
		tranpoint = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getMainLandTranspoint();
	} 
	else
	{
		tranpoint = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getEliteTranspoint();
	}
	
	if (tranpoint)
	{
		tranpoint->MoveToSelfPosTarget();	
	}
}

bool SpriteSeer::IsHadFairy( void )
{
	bool bRet = false;

	std::map<unsigned int,SpriteElf*> elfs;
	if (SpriteElfManager::Get()->GetInstances(this->GetID(),elfs))
	{
		if (elfs.size() > 0)
		{
			bRet = true;
		}
	}

	return bRet;
}

bool SpriteSeer::IsHasFightElf(unsigned int pos)
{
	bool bRet = false;

	if (SpriteElfManager::Get()->IsHasElfAtPos(this->GetID(),pos))
	{
		bRet = true;
	}

	return bRet;	
}

bool SpriteSeer::CanHurt( void )
{
	if(m_pStateMachine)
	{
		for(unsigned int i = ToSkillBegin + 1; i < ToSkillEnd; i++)
		{
			EventType type = (EventType)i;

			FState* pState = m_pStateMachine->GetState(type);
			if(pState)
			{
				BaseAttackState* pBaseAttackState = dynamic_cast<BaseAttackState*>(pState);
				if(pBaseAttackState)
				{
					if(pBaseAttackState->GetSkillInfo())
					{
						if(pBaseAttackState->GetSkillInfo()->bInvincible)
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

bool SpriteSeer::CanAim( void )
{
	if(m_pStateMachine->CheckStateRuning(ToInvisibleKiller))
	{
		return false;
	}

	return true;
}

void SpriteSeer::SetRoleOpacity( GLubyte var )
{
	if(m_nameLabel)
	{
		m_nameLabel->setVisible(var != 0);
	}

	if(m_vipPic)
	{
		m_vipPic->setVisible(var !=0);
	}

	if(m_wearTitle)
	{
		m_wearTitle->setVisible(var != 0);
	}

	if(m_guildNameSprite)
	{
		m_guildNameSprite->setVisible(var != 0);
	}

	std::map<unsigned int,SpriteElf*> elfs;
	if (SpriteElfManager::Get()->GetInstances(this->GetID(),elfs))
	{
		for (std::map<unsigned int,SpriteElf*>::iterator iter = elfs.begin();
			iter != elfs.end(); iter++)
		{
			SpriteElf* pSpriteElf = (*iter).second;
			if(pSpriteElf)
			{
				pSpriteElf->SetRoleOpacity(var);
				pSpriteElf->getGradeLayer()->setVisible(var !=0);
			}
		}
	}
	BoneSpriteBase::SetRoleOpacity(var);
}

void SpriteSeer::onBroadcastMessage( BroadcastMessage* msg )
{
	switch(msg->message)
	{
		case GM_GET_PLAYER_CHANGE_NAME:
			if(m_nameLabel)
			{
				m_nameLabel->setString(UserData::getUserName());
				resetVipMark();
				resetGuildName();
				resetWearTitle();
			}
			break;
		case GM_ATTR_VIP_LEVEL_CHANGE:
			resetVipMark();
			resetGuildName();
			resetWearTitle();
			break;
		case GM_CHANGE_USER_GUILD:
			resetGuildName();
			resetWearTitle();
			break;
		case GM_UPDATE_USER_EQUIP_TITLE:
			resetWearTitle();
			break;
	}
}

void SpriteSeer::resetGuildName()
{
	USERINFO* info = UserData::Get()->GetUserInfo(m_ID);
	if (info == NULL)
	{
		return;
	}
	if(m_guildNameSprite)
	{
		m_guildNameSprite->removeFromParentAndCleanup(true);
		m_guildNameSprite = NULL;
	}
	if(info->guildName != "")
	{
		int vipLevel = UserData::Get()->GetVipLevel();
		CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("M_SOCIATY_NAMEMARK"),info->guildName.c_str());
		m_guildNameSprite = UILabelTTF::create(str->m_sString.c_str(),KJLinXin,20);
		if(m_guildNameSprite)
		{
			m_guildNameSprite->SetDepthTest(false);
			CCPoint pos = getAttachPoint(kType_PLAYER_NAME);
			pos.y += m_nameHeight * 0.5f+ m_guildNameSprite->getContentSize().height*0.5f;
			//pos.y += m_nameHeight;
			m_guildNameSprite->setPosition(pos);
			m_guildNameSprite->setVertexZ(-LevelLayer::sCalcDepth(MAP_Z_OBJ_TOP));
			m_rootSprite->addChild(m_guildNameSprite);
			/*
			if (vipLevel == 13)
			{
				m_guildNameSprite->setColor(ccc3(201,57,255));
			} 
			else if (vipLevel == 14)
			{
				m_guildNameSprite->setColor(ccc3(254,174,49));
			}
			else
			*/
			{
				m_guildNameSprite->setColor(ccc3(0,162,255));
			}
		}
	}

}

void SpriteSeer::resetWearTitle()
{
	//unsigned int titleId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Talent/TalentInterface.lua","getUserWearTitle");
	USERINFO* info = UserData::Get()->GetUserInfo(m_ID);
	if (info == NULL)
	{
		return;
	}
	if(info->current_title != 0)
	{
		CCString* str = CCString::createWithFormat("map_ui_title_FRAME_%u",info->current_title);
		CCSprite* titleSprite = AspriteManager::getInstance()->getOneFrame("UI/ui_title.bin",str->m_sString.c_str());
		if(titleSprite)
		{
			if(m_wearTitle)
			{
				m_wearTitle->removeFromParentAndCleanup(true);
				m_wearTitle = NULL;
			}
			CCPoint pos = getAttachPoint(kType_PLAYER_NAME);
			float guildNameHeight = 0;
			if(m_guildNameSprite)
			{
				guildNameHeight = m_guildNameSprite->getContentSize().height;
			}
			pos.y += m_nameHeight * 0.5f+ guildNameHeight * 0.5f+ titleSprite->getContentSize().height*0.5f;
			//pos.y += m_nameHeight+ guildNameHeight;
			titleSprite->setPosition(pos);
			m_rootSprite->addChild(titleSprite);
			m_wearTitle = titleSprite;
		}
	}
}

void SpriteSeer::resetVipMark()
{
	USERINFO* info = UserData::Get()->GetUserInfo(m_ID);
	if (info == NULL)
	{
		return;
	}

	if (m_vipPic)
	{//之前是vip
		m_vipPic->removeFromParentAndCleanup(true);
		m_vipPic = NULL;
	}
		
	unsigned int vip = info->m_vip;
	if (vip != 0)
	{
		char iconName[30] = {'\0'};
		sprintf(iconName, "map_ui_FRAME_ICON_HEADVIP%d", vip);
		CCSprite* pPic = AspriteManager::getInstance()->getOneFrame(KUI_BIN,iconName);
		if (pPic && m_nameLabel)
		{
			ASprite* pAsprite = AspriteManager::getInstance()->getAsprite(KUI_BIN);

			if(pAsprite)
			{
				m_vipPic = AlphaTestBatchNode::create(pAsprite->getImageName().c_str());
				if(m_vipPic)
				{
					m_vipPic->SetCoef(0.99f);
					pPic->setTexture(m_vipPic->getTexture());
					m_vipPic->addChild(pPic);
					//CCPoint pos = m_nameLabel->getPosition();
					//pos.y += m_nameLabel->getContentSize().height/2.0f + pPic->getContentSize().height/2.0f;
					CCPoint vipPos = getAttachPoint(kType_PLAYER_NAME);
					CCPoint namePos = getAttachPoint(kType_PLAYER_NAME);
					vipPos.x -=  m_nameLabel->getContentSize().width*0.5f;
					namePos.x += pPic->getContentSize().width*0.5f;
					m_vipPic->setPosition(vipPos);
					m_nameLabel->setPosition(namePos);
					m_vipPic->setVertexZ(-LevelLayer::sCalcDepth(MAP_Z_OBJ_TOP));
					m_rootSprite->addChild(m_vipPic);
					if(pPic->getContentSize().height > m_nameHeight)
					{
						m_nameHeight = pPic->getContentSize().height;
					}
				}
			}
		}
	}
}
void SpriteSeer::unRegisterWithTouchDispatcher()
{
	
	if(touchStatus )
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	touchStatus = false;
}
void SpriteSeer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	
	if(!touchStatus)
		pDirector->getTouchDispatcher()->addTargetedDelegate(this,133, true);
	touchStatus = true ;
}
bool SpriteSeer::isTouchSelf(cocos2d::CCPoint pt)
{
	cocos2d::CCRect rect = getRect();
	//// TODO: 为不同的Npc拖动不同的框
	//rect.size.width = 64;
	//rect.size.height = 96;

	if((pt.x >= rect.origin.x) && 
		(pt.x <= rect.origin.x + rect.size.width) &&
		(pt.y >= rect.origin.y) && 
		(pt.y <= rect.origin.y + rect.size.height))
	{
		return true;
	}

	return false;	
}
cocos2d::CCRect SpriteSeer::getRect()
{
	if (m_animBone)
	{
		return m_animBone->getCurViewRect();
	}

	return CCRectMake(0, 0, 0, 0);	
}


bool  SpriteSeer::isTouchInPop(cocos2d::CCPoint pt1 ,cocos2d::CCPoint pt2 ,cocos2d::CCSize sz) 
 {
	  bool ret =false ;

	  if(pt1.x>=(pt2.x-sz.width/2)&&pt1.x<=(pt2.x+sz.width/2)&&pt1.y>=(pt2.y-sz.height/2)&&pt1.y<=(pt2.y+sz.height/2))
		  ret = true ;
	  return ret ;
 }
 cocos2d::CCPoint  SpriteSeer::adaptShowDot(cocos2d::CCPoint pt1,cocos2d::CCSize sz1 ,cocos2d::CCSize winsz)
 {
	 cocos2d::CCPoint pt = ccp(0.0 ,0.0);
	 cocos2d::CCPoint beginPt = ccpSub(pt1 ,ccp(sz1.width/2 ,sz1.height/2));
	
	 cocos2d::CCPoint endPt =ccpAdd(pt1 ,ccp(sz1.width/2 ,sz1.height/2));

	 if(beginPt.x <0)
	 {
		 beginPt.x= 0 ;
		 endPt.x = beginPt.x + sz1.width ;
	 
	 }
	 else if(endPt.x > winsz.width)
	 {
		 endPt.x = winsz.width ;
		 beginPt.x = endPt.x -sz1.width ;
	 }
	 if(beginPt.y < 0)
	 {
		 beginPt.y = 0 ;
		 endPt.y = beginPt.y +sz1.height ;

	 
	 } 
	 else if(endPt.y >winsz.height )
	 {
		 endPt.y = winsz.height ;
		 beginPt.y = endPt.y -sz1.height ;
	 }
	 pt = ccpAdd(beginPt ,endPt);

	 pt = ccp(pt.x/2 ,pt.y/2);

	 return pt ;



 }
bool SpriteSeer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event)
{
	/*NPCData* pData = BoneNPCManager::getInstance()->GetOneNpcBasicData(m_TypeID);


	if (pData->mName == "")
	{

	return false;
	}*/


	bool ret = false;
	SpriteSeer * seer = GameManager::Get()->getHero();
	
	if(this == seer|| this->isVisible()== false)
		return false ;
	
	 bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	 if(!bMainLandLevel)
		 return false ;
	int openMenu =  LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getChatPlayerOpen");
	if(openMenu)
	{
		int width = LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getWinW");
		int hight =  LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getWinH");
		int  px =  LuaTinkerManager::Get()->callLuaFunc<int >("Script/Chat/ChatPlayerOption.lua","getWinPostionX");
		int  py =  LuaTinkerManager::Get()->callLuaFunc<int >("Script/Chat/ChatPlayerOption.lua","getWinPostionY");
		CCPoint oldPT = pTouch->getLocationInView();
		oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);
		if(! isTouchInPop(oldPT ,ccp(px ,py),CCSizeMake(width ,hight)))
			 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","closeChatPlayerOption");
		return ret ;
	}
	
	if(canDealWithTouch(pTouch))
	{
		
		CCPoint oldPT = pTouch->getLocationInView();
		oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);
	    CCSize cwin = CCDirector::sharedDirector()->getWinSizeInPixels();

		CCPoint winPT = ccp(cwin.width/2 ,cwin.height/2);
		USERINFO* info =UserData::Get()->GetUserInfo(this->m_ID) ;
		unsigned int strength = info->effectiveness ;
		//oldPT =this->convertToWorldSpace(oldPT);
		cocos2d::CCRect selfRect = getRelativeParentColliderRect();
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerId",this->m_ID.getID());
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setRegtime",this->m_ID.getRegTime());
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setChannel",this->m_ID.getChannel());
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerStrength",strength);
		 
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerName",this->m_name.c_str());
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setInitPlayer");
		 int width = LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getWinW");
		 int hight =  LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getWinH");
		oldPT = adaptShowDot(oldPT ,CCSizeMake(width ,hight),cwin);
		 oldPT = ccpSub(oldPT ,winPT);
		 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setLayerPostion",oldPT.x,oldPT.y);
         
		//SpriteNPCListener::sShareInstance()->HandleObjectTouchEvent(this, getPosition(),selfRect);
		ret = true;
	}
	
	  

	return ret;
}

void SpriteSeer::addAttackSkill(int skillId){
	attackSkillVec.push_back(skillId);
}

void SpriteSeer::addBuffSkill(int skillId){
	buffSkillVec.push_back(skillId);
}