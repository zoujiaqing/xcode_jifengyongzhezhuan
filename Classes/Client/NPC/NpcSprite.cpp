#include "TaskManager.h"
#include "NpcSprite.h"
#include "NPCDataDefine.h"
#include "EnginePlayerManager.h"
#include "AspriteManager.h"
#include "SpriteNpcPointListener.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "TaskChatLayer.h"
#include "TXGUI.h"
#include "GameManager.h"
#include "SpriteSeer.h"
#include "LevelLayer.h"
#include "GameAudioManager.h"
#include "ItemUpdateManager.h"
#include "BoneNPCManager.h"
#include "InstanceManager.h"
#include "LuaTinkerManager.h"
#include "algorithm"

SpriteNPC::SpriteNPC(int type)
	:BoneSpriteMoveable(type,false)
{
	mHeadImageIcon = 0;
	mShadowImageIcon = 0;
	mNpcDataInfo = 0;
	for (int index = 0;index < 3;index++)
	{
		mTaskFlagSignIcon[index] = 0;
	}
	mSelfColliderRect = CCRectZero;
	mSelfColliderRect.origin = ccp(-20,-20);
	mSelfColliderRect.size.width = 40;
	mSelfColliderRect.size.height = 40;

	ItemUpdateManager::Get()->addToUpdateManager(this);
}

SpriteNPC::~SpriteNPC()
{
	CC_SAFE_DELETE(mNpcDataInfo);

	ItemUpdateManager::Get()->removeFromUpdateManager(this);
}

SpriteNPC* SpriteNPC::NPCWithData(NPCData *pData)
{		
	SpriteNPC * npc = new SpriteNPC(pData->mType);
	if (npc && npc->InitWithData(pData))
	{
		npc->setTag(MAP_TAG_NPC);
		npc->autorelease();
		return  npc;
	}
	CC_SAFE_DELETE(npc);
	return NULL;
}

bool SpriteNPC::InitWithData(NPCData *pData)
{
	mNpcDataInfo = pData;
	m_ID.setPlayId(pData->mID,0,0);
	m_TypeID = pData->mRcsTypeID;

	m_animBone =  EnginePlayerManager::getInstance()->addBoneAnimSprite(pData->mRcsTypeID); 
	attachBoneAnimToRootSprite(1);

	GetTaskFlagSignIcon();
	GetShadowImageIcon();
	GetUpLogoImageIcon();

	AttachImageIcon();

	PlayIdleAnimation();

	return true;
}

void SpriteNPC::PlayIdleAnimation()
{
	if (m_animBone)
	{
		m_animBone->SetAnim(kType_Play,0,true);
	}
}

CCSprite* SpriteNPC::GetSpriteFromSystemIconByName(const char *rcs_name)
{
	int iSpriteFrame = TXGUI::getResourceIDByName(rcs_name);
	ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
	if (as == 0)
	{
		return 0;
	}
	CCPoint pos;
	CCSprite * pSprite = as->getSpriteFromFrame_Middle(iSpriteFrame, 0,pos);
	return pSprite;
}

void SpriteNPC::GetUpLogoImageIcon()
{
	if (mHeadImageIcon == NULL && mNpcDataInfo->mHeadImageIcon != "")
	{
		std::string npcLogoIconName = mNpcDataInfo->mHeadImageIcon;
		std::transform(npcLogoIconName.begin(), npcLogoIconName.end(), npcLogoIconName.begin(), towupper);
		npcLogoIconName = std::string("map_ui_system_icon_FRAME_") + npcLogoIconName;	

		mHeadImageIcon = GetSpriteFromSystemIconByName(npcLogoIconName.c_str());
		mHeadImageIcon->setAnchorPoint(ccp(0.5,0));
		mHeadImageIcon->setPosition(getAttachPoint(kType_PLAYER_NAME));
		mHeadImageIcon->setVisible(true);
	}
}

void SpriteNPC::GetTaskFlagSignIcon()
{
	// TODO: 为不同的角色添加绑定点

	CCSprite *pSprite = GetSpriteFromSystemIconByName("map_ui_system_icon_FRAME_ICON_EXCLAMATORYMARK");
	pSprite->setScale(0.5);
	pSprite->setAnchorPoint(ccp(0.5,0));
	pSprite->setPosition(getAttachPoint(kType_PLAYER_NAME));
	//pSprite->setPosition(ccp(0,80));
	pSprite->setVisible(false);
	mTaskFlagSignIcon[YELLOW_EXCLAMATION_MARK] = pSprite;

	pSprite = GetSpriteFromSystemIconByName("map_ui_system_icon_FRAME_ICON_QUESTIONMARK_GREY");
	pSprite->setScale(0.5);
	pSprite->setAnchorPoint(ccp(0.5,0));
	pSprite->setPosition(getAttachPoint(kType_PLAYER_NAME));
	//pSprite->setPosition(ccp(0,80));
	pSprite->setVisible(false);
	mTaskFlagSignIcon[GRAY_QUES_MARK] = pSprite;

	pSprite = GetSpriteFromSystemIconByName("map_ui_system_icon_FRAME_ICON_QUESTIONMARK_GOLD");
	pSprite->setScale(0.5);
	pSprite->setAnchorPoint(ccp(0.5,0));
	pSprite->setPosition(getAttachPoint(kType_PLAYER_NAME));
	//pSprite->setPosition(ccp(0,80));
	pSprite->setVisible(false);
	mTaskFlagSignIcon[YELLOW_QUES_MARK] = pSprite;	
}

void SpriteNPC::GetShadowImageIcon()
{
	mShadowImageIcon = AspriteManager::getInstance()->getAsprite("spriteBin/shadow.bin");
}

void SpriteNPC::AttachImageIcon()
{
	// Note: 绑定阴影 在NPCManager里添加阴影了。
	/*if (mShadowImageIcon != 0)
	{
		CCPoint cp;
		CCSprite* pCSprite = mShadowImageIcon->getSpriteFromFrame(0, 0, cp);
		if(pCSprite != NULL)
		{
			pCSprite->setPosition(ccp(0, 0));
			this->addChildToRoot(pCSprite);
		}
	}*/

	for (int index = 0;index < 3;index++)
	{
		this->addChildToRoot(mTaskFlagSignIcon[index]);
	}

	if (mHeadImageIcon)
	{
		this->addChildToRoot(mHeadImageIcon);
	}
}

void SpriteNPC::ShowFlagSignIcon(NPCFlagSign flag)
{
	for (int index = 0;index < 3;index++)
	{
		mTaskFlagSignIcon[index]->setVisible(false);
	}

	switch(flag)
	{
	case YELLOW_EXCLAMATION_MARK:
		{
			mTaskFlagSignIcon[YELLOW_EXCLAMATION_MARK]->setVisible(true);
		}
		break;
	case YELLOW_QUES_MARK:
		{
			mTaskFlagSignIcon[YELLOW_QUES_MARK]->setVisible(true);
		}
		break;
	case GRAY_QUES_MARK:
		{
			mTaskFlagSignIcon[GRAY_QUES_MARK]->setVisible(true);			
		}
		break;
	}
}

bool SpriteNPC::isTouchSelf(cocos2d::CCPoint pt)
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

cocos2d::CCRect SpriteNPC::getRect()
{
	if (m_animBone)
	{
		return m_animBone->getCurViewRect();
	}

	return CCRectMake(0, 0, 0, 0);	
}

void SpriteNPC::OneActorMoveEndEvent()
{
	TaskManager::getInstance()->setDialogNpcId(m_TypeID);
	GameAudioManager::sharedManager()->playEffect(350001,false);	
	MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"))->closeBattleCoverLayer(NULL);

	int npcType = LuaTinkerManager::Get()->getLuaConfig<int>("npc", "npcs", m_TypeID, "Type");
	if (5 == npcType)
	{
		//地下城副本中NPC的特殊处理
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/UndergroundInstance.lua", "OnPressUndergroundNpc", m_TypeID);
	}
	else if (6 == npcType)
	{
		//地下城副本中NPC的特殊处理
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/UndergroundInstance.lua", "OnPressCityDefendNpc", m_TypeID);
	}
	else
	{
		GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->GetTaskChatLayer()->ShowTaskChatLayer(m_TypeID);
	}	
}

bool SpriteNPC::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event)
{
	NPCData* pData = BoneNPCManager::getInstance()->GetOneNpcBasicData(m_TypeID);
	
	
	if (pData->mName == "")
	{
		
		return false;
	}
	
	bool ret = false;

	if(canDealWithTouch(pTouch))
	{
		cocos2d::CCRect selfRect = getRelativeParentColliderRect();
		SpriteNPCListener::sShareInstance()->HandleObjectTouchEvent(this, getPosition(),selfRect);
		ret = true;
	}

	return ret;
}

void SpriteNPC::MoveToTarget(CCPoint pos,CCRect colliderRect /* = CCRectZero */)
{
	CCLOG("auto find road step 14 ");
	SpriteNPCListener::sShareInstance()->HandleObjectTouchEvent(this, pos,colliderRect);
}

void SpriteNPC::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -127, true);
}