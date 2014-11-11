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

#include "BattleUILayer.h"
#include <stdlib.h>
#include <stdio.h>

#include "XLogicManager.h"
#include "GameManager.h"
#include "UserData.h"
#include "LevelDefine.h"
#include "UIManager.h"
#include "TXGUIHeader.h"
#include "Define.h"
#include "GameFontManager.h"
#include "CrossUtil.h"

#include "SpriteSeer.h"
#include "ParticleManager.h"
#include "GameAudioManager.h"
#include "SpriteMonsterMgr.h"

#include "ItemBase.h"
#include "EquipmentItem.h"
#include "GameConfigFromLuaManager.h"
#include "SkillDispMgr.h"
#include "SkillInfo.h"
#include "GameDataManager.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
#include "SkillDispMgr.h"
#include "TimeManager.h"
#include "MainMenuLayer.h"
#include "BattleCoverLayer.h"
#include "InstanceManager.h"
#include "StoryInstanceDirector.h"
//#include "ResoureUpdate.h"
#include "LuaTinkerManager.h"
#include "ComboLayer.h"
#include "NormalMonsterHPLayer.h"
#include "BossHPLayer.h"
#include "EliteHPLayer.h"
#include "PlayerHPLayer.h"
#include "AccountsLayer.h"
#include "GameResourceManager.h"
#include "AssistElfFightPanel.h"
#include "SpriteElfManager.h"

using namespace TXGUI;

//////////////////////////////////////////////////////////////////////////
///  
///  
///

BattleUILayer::BattleUILayer()
	:m_headPic(NULL),
	m_layout(NULL),
	m_comboLayer(NULL)
{
    this->m_isSKill1Run = false;
    this->m_isSkill2Run = false;
    this->m_isSkill3Run = false;
    this->m_isSkill4Run = false;
    this->m_isSkill5Run = false;
    
    this->m_skillId1 = 0;
    this->m_skillId2 = 0;
    this->m_skillId3 = 0;
    this->m_skillId4 = 0;
    this->m_skillId5 = 0;

	m_enemyNameLabel = NULL;
	m_hpLabelMap = NULL;
	m_currentHPLable = NULL;
    
#if WIN32
	CCDirector::sharedDirector()->getOpenGLView()->AddWin32KeyLayer(this);
#endif
}

BattleUILayer::~BattleUILayer()
{
	clearSkillProgressTimer();
    TimeManager::Get()->unregisterTimer(this, TIMER_INSTANCE_COUNTDOWN);
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/battleLayer.lua", "unRegisterBattleHandlers");
	UIManager::sharedManager()->RemoveUILayout("BattleUI");

	if(m_hpLabelMap != NULL)
	{
		m_hpLabelMap->clear();
		delete m_hpLabelMap;
		m_hpLabelMap = NULL;
	}	
	

#if WIN32
	CCDirector::sharedDirector()->getOpenGLView()->RemovWin32KeyLayer(this);
#endif
}

bool BattleUILayer::init()
{
    if(!CCLayer::init())
    {
       return false;
    }
    
	setTouchEnabled(true);
	//CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0 ,false);
    
	m_curUiAs = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
	
	UIManager *manager = UIManager::sharedManager();

	manager->CreateUILayoutFromFile("UIplist/battleUI.plist", this, "BattleUI");

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::normalAttack",
		menu_selector(BattleUILayer::normalAttack), this);

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::skillOneAttack",
		menu_selector(BattleUILayer::skillTwoAttack), this);

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::skillTwoAttack",
		menu_selector(BattleUILayer::skillOneAttack), this);

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::skillThreeAttack",
		menu_selector(BattleUILayer::skillThreeAttack), this);
	manager->RegisterMenuHandler("BattleUI","BattleUILayer::skillFourAttack",
		menu_selector(BattleUILayer::skillFiveAttack), this);

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::skillFiveAttack",
		menu_selector(BattleUILayer::skillFourAttack), this);

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::findEnemy",
		menu_selector(BattleUILayer::findEnemy), this);

	manager->RegisterMenuHandler("BattleUI","BattleUILayer::backToTown",
		menu_selector(BattleUILayer::backToTown), this);

	m_layout = manager->getUILayout("BattleUI");

	m_userName = (UILabelTTF*)m_layout->FindChildObjectByName<UILabel>("playerName")->getCurrentNode();
	m_userName->setString(UserData::getUserName());
    
    remainText = (UILabelTTF*)m_layout->FindChildObjectByName<UILabel>("remainText")->getCurrentNode();
    remainNum = (UILabelTTF*)m_layout->FindChildObjectByName<UILabel>("remainNum")->getCurrentNode();
    
    remainText->setVisible(false);
    remainNum->setVisible(false);
    
	//m_enemyHpLable = (CCSprite*)m_layout->FindChildObjectByName<UIPicture>("enemyHpBar")->getCurrentNode();
	//m_enemyHpFrame = (CCSprite*)m_layout->FindChildObjectByName<UIPicture>("enemyHpFrame")->getCurrentNode();
	//m_enemyHpLable->setAnchorPoint(ccp(0,0.5f));
	//CCPoint oldPos = m_enemyHpLable->getPosition();
	//float scale = UIManager::sharedManager()->getScaleFactor();
	//m_enemyHpLable->setPosition(ccp(oldPos.x-scale * m_enemyHpLable->getContentSize().width/2,oldPos.y));
	m_normalAttackBtn = m_layout->FindChildObjectByName<TXGUI::UIButton>("normalAttack");		
	//m_enemyHpLable->setVisible(false);
	//m_enemyHpFrame->setVisible(false);
	//M_targetMonsterId = 0;
	//b_checkMonster = false;

	m_headPic = m_layout->FindChildObjectByName<UIPicture>("iconHeadShot");
	if(m_headPic)
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
		const char* headIconName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", UserData::GetUserType(), "HeadPicture");
		
		int idx = getResourceIDByName(headIconName);
		CCPoint pt;
		CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
		if(lockPic)
		{
			lockPic->setScale(0.65f);
			m_headPic->setSprite(lockPic);
		}	
	}    
    
	UIPicture* framepic = m_layout->FindChildObjectByName<UIPicture>("frameHeadShot");
	if(framepic)
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
		int idx = getResourceIDByName("map_ui_FRAME_FRAME_HEADSHOT");
		CCPoint pt;
		CCSprite * lockPic = as->getSpriteFromFrame_Middle( idx, 0, pt);
		if(lockPic)
		{
			lockPic->setScale(0.65f);
			framepic->setSprite(lockPic);
		}	
	}
    
	m_enemyNameLabel = m_layout->FindChildObjectByName<UILabel>("enemyNameText");

    initSkillIcon();
	setPlayerLevel();
	schedule(schedule_selector(BattleUILayer::update), 1.0f);
    
    bool isPVP = LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel();
	bool isPVAI = LevelManager::sShareInstance()->isCurrentPVAILevel();
	bool isBestFighter = LevelManager::sShareInstance()->isCurrentBestFighterLevel();
    UIButton *b1 = m_layout->FindChildObjectByName<UIButton>("backToTown");
    UIButton *b2 = m_layout->FindChildObjectByName<UIButton>("findenemy");
	UIPicture *p1 = m_layout->FindChildObjectByName<UIPicture>("autoFightPic");
    if(isPVP || isPVAI || isBestFighter)
    {
        b1->setVisible(false);
		if (isPVP || isBestFighter)
		{
			b2->setVisible(false);
			p1->setVisible(false);
		}        
    }

    {
        USERINFO mUserData = UserData::GetUserInfo();       
    	
        if(mUserData.first_login){
            b1->setVisible(false);
        }

    }
	m_hpLabelMap = new std::map<unsigned int,HealthPointProtocol*>();    

// Note: 创建辅助战斗精灵的Panel
AssistElfFightPanel* pAssistElfFightPanel = dynamic_cast<AssistElfFightPanel*>(AssistElfFightPanel::create(this,m_layout));
//if (pAssistElfFightPanel)
//{
//	this->addChild(pAssistElfFightPanel);
//	SpriteElfManager::Get()->AddAssistSkillStartObserver(pAssistElfFightPanel);
//}
	bool isPveMode = LevelManager::sShareInstance()->isCurrentPVELevel();
	if(isPveMode && CCUserDefault::sharedUserDefault()->getBoolForKey("autofight", false))
	{
		CCLOG("auto fight is up");
		CCObject temp;
		findEnemy(&temp);
	}

    return true;
}

void BattleUILayer::clearSkillProgressTimer()
{
    SkillDispMgr::Get()->clearCD();
    UIButton* button1 = m_layout->FindChildObjectByName<UIButton>("skillOneAttack");
    UIButton* button2 = m_layout->FindChildObjectByName<UIButton>("skillTwoAttack");
    UIButton* button3 = m_layout->FindChildObjectByName<UIButton>("skillThreeAttack");
    UIButton* button4 = m_layout->FindChildObjectByName<UIButton>("skillFourAttack");
    UIButton* button5 = m_layout->FindChildObjectByName<UIButton>("skillFiveAttack");
    
    CCNode* parentnd1 = button1->getAnimationNode();
    CCNode* parentnd2 = button2->getAnimationNode();
    CCNode* parentnd3 = button3->getAnimationNode();
    CCNode* parentnd4 = button4->getAnimationNode();
    CCNode* parentnd5 = button5->getAnimationNode();
    
    CCArray* pChildren = parentnd1->getChildren();
    if(pChildren && pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(pChildren, child)
        {
            bool ret = dynamic_cast<CCProgressTimer*>(child) ? true : false;
            if(ret)
            {
                CCProgressTimer* pp = dynamic_cast<CCProgressTimer*>(child);
                if(pp)
                    pp->removeAllChildrenWithCleanup(true);
            }
        }
    }
    
    
    pChildren = parentnd2->getChildren();
    if(pChildren && pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(pChildren, child)
        {
            bool ret = dynamic_cast<CCProgressTimer*>(child) ? true : false;
            if(ret)
            {
                CCProgressTimer* pp = dynamic_cast<CCProgressTimer*>(child);
                if(pp)
                    pp->removeAllChildrenWithCleanup(true);
            }
        }
    }
    
    pChildren = parentnd3->getChildren();
    if(pChildren && pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(pChildren, child)
        {
            bool ret = dynamic_cast<CCProgressTimer*>(child) ? true : false;
            if(ret)
            {
                CCProgressTimer* pp = dynamic_cast<CCProgressTimer*>(child);
                if(pp)
                    pp->removeAllChildrenWithCleanup(true);
            }
        }
    }
   
    pChildren = parentnd4->getChildren();
    if(pChildren && pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(pChildren, child)
        {
            bool ret = dynamic_cast<CCProgressTimer*>(child) ? true : false;
            if(ret)
            {
                CCProgressTimer* pp = dynamic_cast<CCProgressTimer*>(child);
                if(pp)
                    pp->removeAllChildrenWithCleanup(true);
            }
        }
    }
    
    pChildren = parentnd5->getChildren();
    if(pChildren && pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(pChildren, child)
        {
            bool ret = dynamic_cast<CCProgressTimer*>(child) ? true : false;
            if(ret)
            {
                CCProgressTimer* pp = dynamic_cast<CCProgressTimer*>(child);
                if(pp)
                    pp->removeAllChildrenWithCleanup(true);
            }
        }
    }
    
    CCNode* child1 = parentnd1->getChildByTag(1000);
    CCNode* child2 = parentnd2->getChildByTag(1000);
    CCNode* child3 = parentnd3->getChildByTag(1000);
    CCNode* child4 = parentnd4->getChildByTag(1000);
    CCNode* child5 = parentnd5->getChildByTag(1000);
    
    if(child1)
    {
        child1->stopAllActions();
        child1->removeFromParentAndCleanup(true);
    }
    
    if(child2)
    {
        child2->stopAllActions();
        child2->removeFromParentAndCleanup(true);
    }
    
    if(child3)
    {
        child3->stopAllActions();
        child3->removeFromParentAndCleanup(true);
    }
    
    if(child4)
    {
        child4->stopAllActions();
        child4->removeFromParentAndCleanup(true);
    }
    
    if(child5)
    {
        child5->stopAllActions();
        child5->removeFromParentAndCleanup(true);
    }
}

void BattleUILayer::update(float dt)
{
	if(m_currentHPLable != NULL)
	{
		m_currentHPLable->updateHP(dt);
	}
	
	/*if(M_targetMonsterId != 0 && b_checkMonster)
	{
		if(LevelManager::sShareInstance()->isCurrentPVPLevel())
		{
			LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
			if (layer)
			{
				std::map<uint32_t, OthersPlayerInfo>::iterator iter = layer->m_othersPlayerInfo.find(M_targetMonsterId);
				if(iter == layer->m_othersPlayerInfo.end())
				{
					m_enemyHpLable->setVisible(false);
					m_enemyHpFrame->setVisible(false);
					m_enemyNameLabel->setVisible(false);
					b_checkMonster = false;
				}
			}
		}
		else
		{
			if(!SpriteMonsterMgr::Get()->GetMonster(M_targetMonsterId))
			{
				m_enemyHpLable->setVisible(false);
				m_enemyHpFrame->setVisible(false);
				m_enemyNameLabel->setVisible(false);
				b_checkMonster = false;
			}
		}
	}*/
}

void BattleUILayer::updateMapName()
{

}


void BattleUILayer::selectSpritePack(CCObject *sender)
{

}

void BattleUILayer::selectMap(CCObject *sender)
{ 

}


void BattleUILayer::selectItemPack(CCObject *sender)
{

}


void BattleUILayer::selectShop(CCObject *sender)
{

}

void BattleUILayer::selectTask(CCObject *sender)
{

}

void BattleUILayer::selectSet(CCObject *sender)
{

}


void BattleUILayer::selectHelp(CCObject *sender)
{

}

void BattleUILayer::onClickedAffirmBt(CCObject* sender)
{
	XLogicManager::sharedManager()->LeaveBattle();

	if (GameManager::Get()->GetSceneLayer())
	{
		GameManager::Get()->GetSceneLayer()->showLoading(true);
	}
	int tag = GetMainMenuLayer()->getLayerTagByName("AccountsLayer");
    AccountsLayer* layer = (AccountsLayer*)GetMainMenuLayer()->getChildByTag(tag);
    if(layer)
        layer->closeAccountsLayer();
    
}
void BattleUILayer::onClickedCancellBt(CCObject* sender)
{
}

void BattleUILayer::onPressTimeoutOK(CCObject* sender)
{
	XLogicManager::sharedManager()->LeaveBattle();
}

void BattleUILayer::onPressPlayerDieOK(CCObject* sender)
{
	XLogicManager::sharedManager()->LeaveBattle();
	if (GameManager::Get()->GetSceneLayer())
	{
		GameManager::Get()->GetSceneLayer()->showLoading(true);
	}
}

bool BattleUILayer::isProgressRunning(const char* uibuttonname)
{
    UIButton* button = m_layout->FindChildObjectByName<UIButton>(uibuttonname);
    CCNode* parent = button->getAnimationNode();
    
    CCArray* pChildren = parent->getChildren();
    if(pChildren && pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(pChildren, child)
        {
            bool ret = dynamic_cast<CCProgressTimer*>(child) ? true : false;
            if(ret)
                return true;
        }
    }
    return false;
}

void BattleUILayer::setupOwnerCD(int skillID, const char* buttonname)
{
    float skillCD = SkillDispMgr::Get()->getOwnCDTime(GameManager::Get()->getHero()->GetID(), skillID);    
    if(skillCD <= 0)
        return;
    
    UIButton* button = m_layout->FindChildObjectByName<UIButton>(buttonname);
	const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("UI/own_cd.png");
    CCSprite* sp_own = CCSprite::create(path);
    runSkillProgress(button, sp_own, skillCD);
}

void BattleUILayer::setupShareCD(const char* buttonname)
{
    float shareCD = SkillDispMgr::Get()->getShareCDTime(GameManager::Get()->getHero()->GetID());
    if(shareCD <= 0)
        return;
    
    UIButton* button = m_layout->FindChildObjectByName<UIButton>(buttonname);
	const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("UI/share_cd.png");
    CCSprite* sp_share = CCSprite::create(path);
    runSkillProgress(button, sp_share, shareCD);
}

void BattleUILayer::runSkillProgress(TXGUI::UIButton* button, CCSprite*sprite, float time)
{
    CCProgressTimer *protime = CCProgressTimer::create(sprite);
 //   button->getAnimationNode()->removeChildByTag(1000, true); // fix BUG #100556
    button->getAnimationNode()->addChild(protime,INT_MAX, 1000);
    protime->setType(kCCProgressTimerTypeRadial);
    protime->setReverseProgress(true);
    
    protime->setPosition(ccp(button->getAnimationNode()->getContentSize().width/2, button->getAnimationNode()->getContentSize().height/2));
    CCProgressFromTo *progressFromTo = CCProgressFromTo::create(time, 100, 0);
    
    CCCallFuncN * proccessOver = CCCallFuncN::create(this, callfuncN_selector(BattleUILayer::progressDone));
    CCSequence *seq = (CCSequence*)CCSequence::create(progressFromTo,proccessOver,NULL);
    protime->runAction(seq);
}

void BattleUILayer::progressDone(cocos2d::CCNode *sender)
{
    CCProgressTimer *protime = (CCProgressTimer*)sender;
    protime->removeFromParentAndCleanup(true);
}

void BattleUILayer::normalAttack(CCObject *sender)
{
	CCLOG("BattleUILayer::normalAttack");

	SpriteSeer* hero = GameManager::Get()->getHero();
	if (hero)
	{
		//unsigned int skillID = 0;
		//switch(hero->getType())
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
        
		unsigned int skillID = SkillDispMgr::Get()->GetNormalAttackID(hero->GetID(), hero->getType());
		if(skillID != 0)
			hero->DoNormalAttack(skillID);
	}
}

void BattleUILayer::skillOneAttack(CCObject *sender)
{
	CCLOG("BattleUILayer::skillOneAttack");
    
	// for test
	SpriteSeer* hero = GameManager::Get()->getHero();
	if(hero)
	{
        unsigned int skillID;
        
        if(!hero->isAutoAttacking())
        {
            if(isProgressRunning("skillTwoAttack"))
                return;
        }
        
        if(hero->isAutoAttacking())
            skillID = m_skillId2;
        else
        {
            skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(1);
            if(skillID != 0)
                hero->DoSkillAttack(skillID);
        }

        if(!hero->isAutoAttacking())
        {
            if(isProgressRunning("skillTwoAttack"))
                return;
        }
        
        setupOwnerCD(skillID, "skillTwoAttack");
        setupShareCD("skillOneAttack");
        setupShareCD("skillThreeAttack");
        setupShareCD("skillFourAttack");
        setupShareCD("skillFiveAttack");
	}
}

void BattleUILayer::skillTwoAttack(CCObject *sender)
{
	SpriteSeer* hero = GameManager::Get()->getHero();
	if(hero)
	{
        if(!hero->isAutoAttacking())
        {
            if(isProgressRunning("skillOneAttack"))
                return;
        }

		unsigned int skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(2);
        
        if(hero->isAutoAttacking())
            skillID = m_skillId1;
        
        else if(skillID != 0)
			hero->DoSkillAttack(skillID);
        
        setupOwnerCD(skillID, "skillOneAttack");
        setupShareCD("skillTwoAttack");
        setupShareCD("skillThreeAttack");
        setupShareCD("skillFourAttack");
        setupShareCD("skillFiveAttack");
	}
	// GameAudioManager::sharedManager()->playEffect(0, false);
}

void BattleUILayer::skillThreeAttack(CCObject *sender)
{
	//CCLOG("BattleUILayer::skillThreeAttack");
	SpriteSeer* hero = GameManager::Get()->getHero();
	if(hero)
	{
		unsigned int skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(3);
        
        if(!hero->isAutoAttacking())
        {
            if(isProgressRunning("skillThreeAttack"))
                return;
        }
        
        if(hero->isAutoAttacking())
            skillID = m_skillId3;
        
		else if(skillID != 0)
			hero->DoSkillAttack(skillID);
        
        setupOwnerCD(skillID, "skillThreeAttack");
        setupShareCD("skillOneAttack");
        setupShareCD("skillTwoAttack");
        setupShareCD("skillFourAttack");
        setupShareCD("skillFiveAttack");
	}
}

void BattleUILayer::skillFourAttack(CCObject *sender)
{
	SpriteSeer* hero = GameManager::Get()->getHero();
	if(hero)
	{
		unsigned int skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(4);

        if(!hero->isAutoAttacking())
        {
            if(isProgressRunning("skillFiveAttack"))
                return;
        }
        
        if(hero->isAutoAttacking())
            skillID = m_skillId5;
        
		else if(skillID != 0)
			hero->DoSkillAttack(skillID);

        setupOwnerCD(skillID, "skillFiveAttack");
        setupShareCD("skillOneAttack");
        setupShareCD("skillTwoAttack");
        setupShareCD("skillThreeAttack");
        setupShareCD("skillFourAttack");
	}
}

void BattleUILayer::skillFiveAttack(CCObject *sender)
{
	SpriteSeer* hero = GameManager::Get()->getHero();
	if(hero)
	{
		unsigned int skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(5);
        
        if(!hero->isAutoAttacking())
        {
            if(isProgressRunning("skillFourAttack"))
                return;
        }
        
        if(hero->isAutoAttacking())
            skillID = m_skillId4;
        
		else if(skillID != 0)
			hero->DoSkillAttack(skillID);

        setupOwnerCD(skillID, "skillFourAttack");
        setupShareCD("skillOneAttack");
        setupShareCD("skillTwoAttack");
        setupShareCD("skillThreeAttack");
        setupShareCD("skillFiveAttack");
        CCLog("skillFiveAttack end\n");
	}
}

void BattleUILayer::autoSkillAttack(int skillId)
{
    //2 1 3 5 4
    if(skillId == m_skillId1)
    {
        CCLog("skillId == m_skillId1");
        skillTwoAttack(NULL);
    }
    
    else if(skillId == m_skillId2)
    {
        CCLog("skillId == m_skillId2");
        skillOneAttack(NULL);
    }
    
    else if(skillId == m_skillId3)
    {
        CCLog("skillId == m_skillId3");
        skillThreeAttack(NULL);
    }
    
    else if(skillId == m_skillId4)
    {
        CCLog("skillId == m_skillId4");
        skillFiveAttack(NULL);
    }
    
    else if(skillId == m_skillId5)
    {
        CCLog("skillId == m_skillId5");
        skillFourAttack(NULL);
    }
}

void BattleUILayer::initSkillIcon()
{
    unsigned int userType = UserData::Get()->GetUserType();
	ASprite* iconAsprite = AspriteManager::getInstance()->getAsprite(KICON_BIN); 
	if (!iconAsprite)
	{
		return;
	}

	unsigned int skillID = 0;
	// 技能攻击1
	skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(1);
    m_skillId2 = skillID;
	if(SkillDispMgr::Get()->GetSkillInfo(skillID))
	{
		string iconName = SkillDispMgr::Get()->GetSkillInfo(skillID)->actionIcon;
		int idx = getResourceIDByName(iconName.c_str());
		CCPoint pt;
		CCSprite * iconPic = iconAsprite->getSpriteFromFrame_Middle( idx, 0, pt);
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillTwoAttack");
		if(button && iconPic)
		{
			button->setVisible(true);
			button->setIconSprite(iconPic);
		}
	}
	else
	{
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillTwoAttack");
		if(button)
		{
			button->setVisible(false);
		}
	}

	// 技能攻击2
	skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(2);
    m_skillId1 = skillID;
	if(SkillDispMgr::Get()->GetSkillInfo(skillID))
	{
		string iconName = SkillDispMgr::Get()->GetSkillInfo(skillID)->actionIcon;
		int idx = getResourceIDByName(iconName.c_str());
		CCPoint pt;
		CCSprite * iconPic = iconAsprite->getSpriteFromFrame_Middle( idx, 0, pt);
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillOneAttack");
		if(button && iconPic)
		{
			button->setVisible(true);
			button->setIconSprite(iconPic);
		}
	}
	else
	{
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillOneAttack");
		if(button)
		{
			button->setVisible(false);
		}
	}

	// 技能攻击3
	skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(3);
    m_skillId3 = skillID;
	if(SkillDispMgr::Get()->GetSkillInfo(skillID))
	{
		string iconName = SkillDispMgr::Get()->GetSkillInfo(skillID)->actionIcon;
		int idx = getResourceIDByName(iconName.c_str());
		CCPoint pt;
		CCSprite * iconPic = iconAsprite->getSpriteFromFrame_Middle( idx, 0, pt);
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillThreeAttack");
		if(button && iconPic)
		{
			button->setVisible(true);
			button->setIconSprite(iconPic);
		}
	}
	else
	{
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillThreeAttack");
		if(button)
		{
			button->setVisible(false);
		}
	}

	// 技能攻击4
	skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(4);
    m_skillId5 = skillID;
	if(SkillDispMgr::Get()->GetSkillInfo(skillID))
	{
		string iconName = SkillDispMgr::Get()->GetSkillInfo(skillID)->actionIcon;
		int idx = getResourceIDByName(iconName.c_str());
		CCPoint pt;
		CCSprite * iconPic = iconAsprite->getSpriteFromFrame_Middle( idx, 0, pt);
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillFiveAttack");
		if(button && iconPic)
		{
			button->setVisible(true);
			button->setIconSprite(iconPic);
		}
	}
	else
	{
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillFiveAttack");
		if(button)
		{
			button->setVisible(false);
		}
	}

	// 技能攻击5
	skillID = SkillDispMgr::Get()->GetSkillIDFromSlot(5);
    m_skillId4 = skillID;
	if(SkillDispMgr::Get()->GetSkillInfo(skillID))
	{
		string iconName = SkillDispMgr::Get()->GetSkillInfo(skillID)->actionIcon;
		int idx = getResourceIDByName(iconName.c_str());
		CCPoint pt;
		CCSprite * iconPic = iconAsprite->getSpriteFromFrame_Middle( idx, 0, pt);
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillFourAttack");
		if(button && iconPic)
		{
			button->setVisible(true);
			button->setIconSprite(iconPic);
		}
	}
	else
	{
		UIButton* button = (UIButton*)m_layout->FindChildObjectByName<UIButton>("skillFourAttack");
		if(button)
		{
			button->setVisible(false);
		}
	}
}

void BattleUILayer::backToTown(CCObject *sender)
{
    USERINFO mUserData = UserData::GetUserInfo();       
	
    if(mUserData.first_login){
        return;
    }
    
	CCLOG("BattleUILayer::backToTown");
	const char* msg = Localizatioin::getLocalization("M_CONFIRM_BACKTOTOWN");
	MessageBox::Show(msg,this,menu_selector(BattleUILayer::onClickedAffirmBt),menu_selector(BattleUILayer::onClickedCancellBt));
}

void BattleUILayer::findEnemy(CCObject *sender)
{
	CCLOG("BattleUILayer::findEnemy");
	bool canAutoFight = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "GetAutoFightState");
	if (!canAutoFight)
	{
		const char* msg = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/main.lua", "GetAutoFightText");
		ShowMessage(msg);
		return;
	}
	//jackniu task#135719 after user use autofight the first time. always turn on autofight
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("autofight", false))
	{
		CCLOG("turn on auto fight");
		CCUserDefault::sharedUserDefault()->setBoolForKey("autofight", true);
    	CCUserDefault::sharedUserDefault()->flush();
	}
	SpriteSeer* hero = GameManager::Get()->getHero();
	if(hero)
	{
		GameDataManager::Get()->setHeroAutoAttack(true);
	}
}

void BattleUILayer::SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID targetMonster,unsigned int type,unsigned int enemyType)
{
	for(std::map<unsigned int,HealthPointProtocol*>::iterator it = m_hpLabelMap->begin();
		it != m_hpLabelMap->end(); ++it)
	{
		if(it->first != enemyType)
		{
			it->second->setActive(false);
		}
	}

	m_currentHPLable = getHealthPointContainer(type);
	m_currentHPLable->setActive(true);
	m_currentHPLable->SetEnemyHpLable(currentHp,isAnim,targetMonster,enemyType);
}

HealthPointProtocol* BattleUILayer::getHealthPointContainer(unsigned int monsterType)
{
	std::map<unsigned int,HealthPointProtocol*>::iterator it = m_hpLabelMap->find(monsterType);
	if(it != m_hpLabelMap->end())
	{
		HealthPointProtocol* lable = it->second;
		CCAssert(lable != NULL,"HealthPointProtocol is null");
		return lable;
	}
	else
	{
		HealthPointProtocol* lable = NULL;
		switch(monsterType)
		{
		case 1:
			{
				NormalMonsterHPLayer* layer = NormalMonsterHPLayer::create();
				this->addChild(layer);
				lable  = layer;
				break;
			}
			// 普通小怪
		case 2:
			{
				// 精英怪
				EliteHPLayer* layer = EliteHPLayer::create();
				this->addChild(layer);
				lable = layer;
				break;
			}
		case 3:
			{
				// boss
				BossHPLayer* layer = BossHPLayer::create();
				this->addChild(layer);
				lable = layer;
				break;
			}
			
		case 4:
			{
				PlayerHPLayer* layer = PlayerHPLayer::create();
				this->addChild(layer);
				lable = layer;
				break;
			}
			// 玩家
			
			break;
		default:
			break;
		}
		CCAssert(lable != NULL,"HealthPointProtocol can not create");
		m_hpLabelMap->insert(std::make_pair(monsterType,lable));
		return lable;
	}
}

//void BattleUILayer::SetEnemyHpLable(float currentHp,float totalHp,bool isAnim,unsigned int targetMonster,const char* targetName,unsigned int nameType,float dt)
//{
//	if(M_targetMonsterId != targetMonster)
//	{
//		if(m_enemyNameLabel)
//		{
//			m_enemyNameLabel->setVisible(true);
//			m_enemyNameLabel->setString(targetName);
//
//			ccColor3B color = ccWHITE;
//			switch(nameType)
//			{
//			case 1:
//				color = ccYELLOW;
//				break;
//			case 2:
//				color = ccRED;
//				break;
//			default:
//				color = ccWHITE;
//				break;
//			}
//			m_enemyNameLabel->setColor(color);
//		}
//	}
//	M_targetMonsterId = targetMonster;
//	b_checkMonster = true;
//	float scale = UIManager::sharedManager()->getScaleFactor();
//	float percent = scale * currentHp / totalHp;
//	if(m_enemyHpLable)
//	{
//		m_enemyHpLable->setVisible(true);
//		m_enemyHpFrame->setVisible(true);
//		m_enemyHpLable->stopAllActions();
//		if(isAnim)
//		{
//			CCActionInterval* actionScale = CCScaleTo::create(dt,percent,scale);
//			m_enemyHpLable->runAction(actionScale);
//		}
//		else
//		{
//			m_enemyHpLable->setScaleX(percent);
//		}
//	}
//
//	
//}

void BattleUILayer::initRemainTime(long seconds)
{
	long nowTime = TimeManager::Get()->getCurServerTime();
	if (TimeManager::Get()->hasTimer(TIMER_INSTANCE_COUNTDOWN))
	{
		TimeManager::Get()->renewTimer(TIMER_INSTANCE_COUNTDOWN, nowTime + seconds);
		if (!TimeManager::Get()->hasObserver(this, TIMER_INSTANCE_COUNTDOWN))
		{
			TimeManager::Get()->attachTimer(this, TIMER_INSTANCE_COUNTDOWN);
		}
	}
	else
	{
		TimeManager::Get()->registerTimer(this, TIMER_INSTANCE_COUNTDOWN, nowTime + seconds);
	}

    string remainStr = TimeManager::Get()->secondsToString(seconds);
    this->remainNum->setVisible(true);
    this->remainText->setVisible(true);
    
    this->remainNum->setString(remainStr.c_str());
}

void BattleUILayer::onUpdateRemainTime(long remainTime)
{
	string remainStr = TimeManager::Get()->secondsToString(remainTime);
    this->remainNum->setString(remainStr.c_str());
}

void BattleUILayer::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCLayer::ccTouchesBegan(pTouches, pEvent);
}

void BattleUILayer::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void BattleUILayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void BattleUILayer::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void BattleUILayer::setPlayerLevel()
{
	if(m_layout)
	{
		UILabel* label = m_layout->FindChildObjectByName<UILabel>("playerlevel");
		if(label)
		{
			unsigned int level = UserData::GetUserLevel();
			char levelStr[10];
			sprintf(levelStr,"Lv%d",level);
			label->setString(levelStr);
		}
	}
}

void BattleUILayer::HandleComboIncrease(unsigned int count)
{
	if (LevelManager::sShareInstance()->isCurrentPVAILevel())
	{
		return;
	}
	if(!m_comboLayer)
	{
		m_comboLayer = ComboLayer::create();
		this->addChild(m_comboLayer);
	}

	m_comboLayer->HandleComboIncrease(count);
}

unsigned int BattleUILayer::GetMaxComboNumber()
{
	if(m_comboLayer)
	{
		return m_comboLayer->GetMaxComboNumber();
	}
	else
	{
		return 0;
	}
}

#if WIN32
void BattleUILayer::processWin32KeyPress( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_NUMPAD0:
			{
				normalAttack(NULL);
			}
			break;
		case VK_NUMPAD1:
			{
				skillOneAttack(NULL);
			}
			break;
		case VK_NUMPAD2:
			{
				skillTwoAttack(NULL);
			}
			break;
		case VK_NUMPAD3:
			{
				skillThreeAttack(NULL);
			}
			break;
		case VK_NUMPAD4:
			{
				skillFourAttack(NULL);
			}
			break;
		case VK_NUMPAD5:
			{
				skillFiveAttack(NULL);
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch( wParam )
		{
		case VK_NUMPAD0:
			{
				
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
#endif
