#include "SpriteElf.h"
#include "EnginePlayerManager.h"
#include "SpriteElfManager.h"
#include "LevelLayer.h"
#include "LevelManager.h"
#include "ElfChaseState.h"
#include "ElfCloseToState.h"
#include "ElfSkillAttackState.h"
#include "MainLandManager.h"
#include "SkillDispMgr.h"
#include "GameManager.h"
#include "ItemManager.h"
#include "LuaTinkerManager.h"
#include "GameResourceManager.h"
#include "GameFontManager.h"
#include "InterfaceManager.h"
#include "AspriteManager.h"
#include "ASprite.h"

SpriteElf::~SpriteElf()
{
	ShutDownStateMachine();
	if(pBaseElfAttachData)
	{
		delete pBaseElfAttachData ;
		pBaseElfAttachData = NULL ;
	}
	if(m_pStateMachine)
	{
		delete m_pStateMachine;
		m_pStateMachine = NULL;
	}
	

	if(m_pStateFactory)
	{
		delete m_pStateFactory;
		m_pStateFactory = NULL;
	}

    setGradeLayer(NULL);
}
void SpriteElf::SpriteDelSkill()
{
	ShutDownStateMachine();
	if(pBaseElfAttachData)
	{
		delete pBaseElfAttachData ;
		pBaseElfAttachData = NULL ;
	}
	if(m_pStateMachine)
	{
		delete m_pStateMachine;
		m_pStateMachine = NULL;
	}
	if(m_pStateFactory)
	{
		delete m_pStateFactory;
		m_pStateFactory = NULL;
	}
}
SpriteElf::SpriteElf()
	: BoneSpriteMoveable(0, 0)
	, m_isStandLeft(false)
	, m_nAssistFightPos(-1)
	, m_LevelLabel(NULL)
    , m_gradeLayer(NULL)
{
	attachedHero = 0;
	elfTempPos = CCPointZero;
	isStartMove = false;
	relativePosToHero = CCPointZero;
	targetPos = CCPointZero;
	SkiAttack = 0 ;

	pBaseElfAttachData =  new BaseElfAttachData()  ;
	m_pStateMachine = new SkillStateMachine(); 
	m_pStateFactory = new ElfStateFactory(this);
}

void SpriteElf::InitStateMachine()
{
	/*if(!m_pStateMachine->CheckStateRuning(ToElfCloseTo))
		m_pStateMachine->Push(m_pStateFactory->Create(ToElfCloseTo,0, 0));*/
}

void SpriteElf::ShutDownStateMachine()
{
	//m_pStateMachine->RemoveAllState();
}

bool SpriteElf::initWithId(int typeId, int grade)
{
	m_TypeID = typeId;

	m_animBone =  EnginePlayerManager::getInstance()->addBoneAnimSprite(typeId);
	if (m_animBone == 0)
	{
		return false;
	}
	attachBoneAnimToRootSprite(1);
	
	// init state machine
	InitStateMachine();

    InitGradeLayer(grade);
	return true;
}

void SpriteElf::InitGradeLayer(int grade)
{
    setGradeLayer(CCLayer::create());
    
    int quality = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyQualityByID", m_TypeID);
    if (quality <= 2)
    {
        return;
    }

    CCSprite* plusSprite = NULL;
    if (quality == 3)
    {
        plusSprite = CCSprite::createWithSpriteFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", "blue_plus.png"));
    } else
    {
        plusSprite = CCSprite::createWithSpriteFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", "purple_plus.png"));
    }

    CCString* pStrGrade = NULL;
    if (quality == 3)
    {
        pStrGrade = CCString::createWithFormat("blue_%d.png", grade % 10);
    } else
    {
        pStrGrade = CCString::createWithFormat("purple_%d.png", grade % 10);
    }

    CCSprite* gradeSprite = CCSprite::createWithSpriteFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", pStrGrade->getCString()));
    m_gradeLayer->addChild(plusSprite);
    m_gradeLayer->addChild(gradeSprite);
    plusSprite->setPosition(ccp(-11, 0));
    gradeSprite->setPosition(ccp(11, 0));

    m_rootSprite->addChild(m_gradeLayer, 1, 0);
    m_gradeLayer->setPosition(getAttachPoint(kType_PLAYER_NAME));
    m_gradeLayer->setVisible(false);
}

void SpriteElf::Update(float fTime)
{
	//this->setVertexZ(LevelLayer::sCalcZorder(this->getPosition()));
	if (m_pStateMachine)
	{
		m_pStateMachine->Update(fTime);
	}
}

void SpriteElf::ElfMoveToTarget()
{
	isStartMove = false;
	elfTempPos = targetPos;
}

bool SpriteElf::IsElfStartMove(SpriteSeer* pHero,CCPoint &targetPoint)
{
	if (pHero == 0)
	{
		return false;
	}
	targetPoint = GetMoveTargetPos(pHero->IsAnimFlipX());
	CCPoint disPos = ccpSub(targetPoint,elfTempPos);
	float distance = 4;
	if(ccpLengthSQ(disPos) >= distance)
	{
		return true;
	}
	return false;
}

CCPoint SpriteElf::GetMoveTargetPos(bool bRight)
{
	if (attachedHero == 0)
	{
		return CCPointZero;
	}
	CCPoint distPoint = relativePosToHero;
	if (false == bRight)
	{
		distPoint.x = -distPoint.x;
	}
	return ccpAdd(attachedHero->getPosition(),distPoint);
}

void SpriteElf::AttachToOneHero(SpriteSeer* hero)
{
	attachedHero = hero;

	this->m_name = hero->m_name;

	// Note: Add To Obj Layer
	LevelLayer* pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (pLayer)
	{
		CCNode* pObjLayer = pLayer->getObjectLayer();
		if (pObjLayer)
		{

			//const char* name = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyNameByID", getSpriteCurPos());
			//int quality = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyQualityByID", getSpriteCurPos());
			//ccColor3B color = ItemManager::Get()->getFairyLabelColorByQuality(quality);
			//float labelFontSize = GameFontManager::smallFontSize();
			//
		
			//m_LevelLabel = UILabelTTF::create(name,KJLinXin,labelFontSize);
			//if(m_LevelLabel)
			//{
			//	m_LevelLabel->SetDepthTest(false);
			//	m_LevelLabel->setPosition(getAttachPoint(kType_PLAYER_NAME));
			//	//m_nameLabel->setVertexZ(LevelLayer::sCalcDepth(MAP_Z_OBJ_TOP)+100);
			//	m_rootSprite->addChild(m_LevelLabel);
			//	//m_nameLabel->setVisible(false);
			//	//ccColor3B nameLabelColor = GetNameLabelColor();
			//	m_LevelLabel->setColor(color);
			//}


			// Note: 设置精灵的起始位置
			relativePosToHero = ccpAdd(attachedHero->getAttachPoint(kType_PLAYER_NAME),SpriteElfManager::s_fixDistanceToHero);
			//relativePosToHero = SpriteElfManager::s_fixDistanceToHero ;
			elfTempPos = GetMoveTargetPos(true);
			this->setPosition(elfTempPos);
			//this->setVertexZ(-LevelLayer::sCalcDepth(MAP_Z_OBJ_TOP));
			pObjLayer->addChild(this ,LevelLayer::sCalcZorder(attachedHero->getPosition())-1);
			
			this->SetAnim(kTypeIdle_FaceDown_Stand_OTH,1);
			//this->getRoot()->setScale(0.5);
			
			PushOneElfCloseToState(attachedHero, 0,pBaseElfAttachData );
		}
	}
}

void SpriteElf::AttachToHeroAssistType(SpriteSeer* hero)
{
	if (hero)
	{
		attachedHero = hero;
		this->m_name = hero->m_name;

		// Note: Add To Obj Layer
		LevelLayer* pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (pLayer)
		{
			CCNode* pObjLayer = pLayer->getObjectLayer();
			if (pObjLayer)
			{
				// Note: 设置精灵的起始位置
				this->setPosition(CCPointZero);
				pObjLayer->addChild(this,MAP_Z_OBJ_TOP);
				this->SetAnim(kTypeIdle_FaceDown_Stand_OTH,1);

				if(!MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId()))
				{
					if(!m_pStateMachine->CheckStateRuning(ToAssistElfAttackTrigger))
					{
						unsigned int skillId = SkillDispMgr::Get()->GetFairyAssistSkillID(GetTypeId());
						if(skillId != 0 && !GetBattlePVP(hero))
							m_pStateMachine->Push(m_pStateFactory->Create(ToAssistElfAttackTrigger, attachedHero, skillId,pBaseElfAttachData));
					}			
				}
			}
		}
	}
}


void SpriteElf::PushOneElfChaseState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData /* = 0 */)
{
	float goTime = 0;
	if (m_pStateMachine->CheckStateRuning(ToElfChase))
	{
		m_pStateMachine->Remove(m_pStateMachine->GetState(ToElfChase));
	}
	if (m_pStateMachine->CheckStateRuning(ToElfSkillAttack))
	{
		m_pStateMachine->Remove(m_pStateMachine->GetState(ToElfSkillAttack));
	}
	if (m_pStateMachine->CheckStateRuning(ToElfCloseTo))
	{
		ElfCloseToState* pCloseToState = (ElfCloseToState*)m_pStateMachine->GetState(ToElfCloseTo);
		m_pStateMachine->Remove((FState*)pCloseToState);
	}
	ElfChaseState* pChaseState = (ElfChaseState*)m_pStateFactory->Create(ToElfChase, pHero,skillId,pData);
	m_pStateMachine->Push((FState*)pChaseState);
}
bool SpriteElf::GetBattlePVP( SpriteSeer* pHero )
{
	if(LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel()
		|| LevelManager::sShareInstance()->isCurrentBestFighterLevel())
	{
		if(pHero->GetID() != GameManager::Get()->getHero()->GetID())
			return true ;
	}
	return false ;
}
void SpriteElf::PushOneElfCloseToState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData /* = 0 */)
{
	if (m_pStateMachine->CheckStateRuning(ToElfChase))
	{
		m_pStateMachine->Remove(ToElfChase);
	}
	if (m_pStateMachine->CheckStateRuning(ToElfCloseTo))
	{
		m_pStateMachine->Remove(ToElfCloseTo);
	}

	if(!m_pStateMachine->CheckStateRuning(ToElfSkillAttack))
		m_pStateMachine->Push(m_pStateFactory->Create(ToElfCloseTo, pHero,skillId,pData));
	
	if(!MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId()))
	{
		if(!m_pStateMachine->CheckStateRuning(ToElfAttackTrigger))
		{
			unsigned int skillId = SkillDispMgr::Get()->GetFairySkillID(GetTypeId());
			if(skillId != 0 && !GetBattlePVP(pHero))
			m_pStateMachine->Push(m_pStateFactory->Create(ToElfAttackTrigger, pHero, skillId, pData));
		}			
	}
}

void SpriteElf::PushOneElfAttackState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData /* = 0 */)
{
	if (m_pStateMachine->CheckStateRuning(ToElfCloseTo))
	{
		m_pStateMachine->Remove(ToElfCloseTo);
	}
	if (m_pStateMachine->CheckStateRuning(ToElfChase))
	{
		m_pStateMachine->Remove(ToElfChase);
	}
	if (m_pStateMachine->CheckStateRuning(ToElfSkillAttack))
	{
		m_pStateMachine->Remove(ToElfSkillAttack);
	}
	m_pStateMachine->Push(m_pStateFactory->Create(ToElfSkillAttack, pHero,skillId,pData));
}
int SpriteElf::getSpriteCurPos()
{
	if(m_nAssistFightPos >= 0)
		return m_nAssistFightPos +20 ;
	else if(m_nAssistFightPos == -1)
	{
		if(m_isStandLeft)
			return 1 ;
		else
			return 2 ;
	}
}
bool SpriteElf::DoSkillAttack( unsigned int skillId,const CCPoint& rolePos, const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster )
{
	m_pStateMachine->Remove(ToElfSkillAttack);
	m_pStateMachine->Remove(ToElfCloseTo);
	const FAIRY_SKILL_INFO* pSkillInfo = SkillDispMgr::Get()->GetFairySkillInfo(skillId);

	if(pSkillInfo == NULL)
	{
		ASSERT(false, "Can't find elf skill!!!");
		return false;
	}
	EventType stateEvent = ToAssistElfAttack;
	if(IsAssistElf()&& pSkillInfo->aim ==FAIRY_SKILL_INFO::E_HERO )
	{
		stateEvent = ToAssistElfAttack ;
	//	m_pStateMachine->Push(m_pStateFactory->Create(ToAssistElfAttack, attachedHero, skillId, NULL));

		
	}
	else
		stateEvent = ToElfSkillAttack ;
	FState* pState = m_pStateFactory->Create(stateEvent, attachedHero, skillId, NULL);
	StateContainer* pContainer = dynamic_cast<StateContainer*>(pState);
	if(pContainer)
	{
		SkillBeginReceiverHandler* pRecevier = new SkillBeginReceiverHandler( new BaseAttackEvt( attachedHero, skillId ) );;
		if(pRecevier)
		{
			pRecevier->SetAimMonster(bAnimMonster);
			pRecevier->SetDirection(aimDir);
			pRecevier->SetNearestMonsterPos(monsterPos);
			pRecevier->SetRolePos(rolePos);
			pRecevier->SetRoleDirection(roleDir);

			pContainer->Push(pRecevier);

		}
	}
	m_pStateMachine->Push(pState);

	return true;
}
