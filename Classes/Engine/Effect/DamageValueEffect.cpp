#include "DamageValueEffect.h"
#include "TXGUIHeader.h"
#include "AspriteManager.h"
#include "ASprite.h"
#include "SceneLayer.h"
#include "LevelLayer.h"
#include "GameManager.h"
#include "ParticleManager.h"

DamageValueEffect::DamageValueEffect()
{

}

DamageValueEffect::~DamageValueEffect()
{
	m_pDamageLabels->clear();
	delete m_pDamageLabels;
	m_pDamageLabels = NULL;
}

bool DamageValueEffect::init()
{
	bool bRet = false;
	do {
		m_pDamageLabels = new std::map<CCNode*,float>;
		schedule(schedule_selector(DamageValueEffect::update), 0.05f);
		bRet = true;
	}while (0);
	return bRet;
}

void DamageValueEffect::HandleDamageNumberEffect(CCPoint parentPt,unsigned int damage,bool isCrit,bool isHarmFul,bool isFairy)
{
	//CCPoint seerPos = parent->getPosition();
	if(isFairy)
	{
		if(isHarmFul)
		{
			// to do
			CreateFairyDamageLabel(damage,parentPt);
		}
		else
		{
			CreateDamageLable(damage,parentPt,isHarmFul);
		}
	}
	else
	{
		if(isCrit)
		{
			CreateCritDamageLable(damage,parentPt,isHarmFul);
		}
		else
		{
			CreateDamageLable(damage,parentPt,isHarmFul);
		}
	}
	
}

CCLabelAtlas* DamageValueEffect::CreateFairyDamageLabel(unsigned int damage,CCPoint parentPos)
{
	CCPoint uiPos = parentPos;
	char damageString[10];
	sprintf(damageString,"%d",damage);
	CCLabelAtlas* lable = CCLabelAtlas::create(damageString,"UI/ui_numble_5.png",41,43,48);

	lable->setPosition(uiPos.x,uiPos.y);
	lable->setAnchorPoint(ccp(0.5,0.5));
	lable->setScale(0);
	CCActionInterval* actionScale = CCScaleTo::create(0.01f,2);
	CCActionInterval* actionMove = CCMoveBy::create(0.01f,ccp(0,40));
	CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionScale,actionMove);
	CCActionInterval* actionFade = CCFadeOut::create(1.0f);
	CCActionInterval* bezierAction = CCMoveBy::create(1.0f,ccp(0,80));
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(bezierAction,actionFade);
	lable->runAction(CCSequence::create(FirstAction,CCScaleTo::create(0.05f,0.7f),CCDelayTime::create(0.2f),SecondAction,
		NULL));
	if(m_pDamageLabels->find(lable) == m_pDamageLabels->end())
	{
		m_pDamageLabels->insert(make_pair(lable,1.3f));
		if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
			&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
		{
			GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(lable,MAP_Z_OBJ_TOP);
		}
	}
	return lable;
}

CCLabelAtlas* DamageValueEffect::CreateAddBloodLabel(unsigned int blood,CCPoint parentPos)
{
	CCPoint uiPos = parentPos;
	char damageString[10];
	sprintf(damageString,"%d",blood);
	CCLabelAtlas* lable = CCLabelAtlas::create(damageString,"UI/ui_numble_4.png",41,43,48);

	lable->setPosition(uiPos.x,uiPos.y);
	lable->setAnchorPoint(ccp(0.5,0.5));
	lable->setScale(0);
	CCActionInterval* actionScale = CCScaleTo::create(0.01f,2);
	CCActionInterval* actionMove = CCMoveBy::create(0.01f,ccp(0,40));
	CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionScale,actionMove);
	CCActionInterval* actionFade = CCFadeOut::create(1.0f);
	CCActionInterval* bezierAction = CCMoveBy::create(1.0f,ccp(0,80));
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(bezierAction,actionFade);
	lable->runAction(CCSequence::create(FirstAction,CCScaleTo::create(0.05f,0.7f),CCDelayTime::create(0.2f),SecondAction,
		NULL));
	if(m_pDamageLabels->find(lable) == m_pDamageLabels->end())
	{
		m_pDamageLabels->insert(make_pair(lable,1.3f));
		if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
			&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
		{
			GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(lable,MAP_Z_OBJ_TOP);
		}
	}
	return lable;
}

CCLabelAtlas* DamageValueEffect::CreateDamageLable(unsigned int damage,CCPoint parentPos,bool isHarmFul)
{
	CCPoint uiPos = parentPos;
	char damageString[10];
	sprintf(damageString,"%d",damage);
	CCLabelAtlas* lable;
	if(isHarmFul)
	{
		lable = CCLabelAtlas::create(damageString,"UI/ui_numble_1.png",41,43,48);
	}
	else
	{
		lable = CCLabelAtlas::create(damageString,"UI/ui_numble_3.png",41,43,48);
	}
	
	lable->setPosition(uiPos.x,uiPos.y);
	lable->setAnchorPoint(ccp(0.5,0.5));
	lable->setScale(0);
	CCActionInterval* actionScale = CCScaleTo::create(0.01f,2);
	CCActionInterval* actionMove = CCMoveBy::create(0.01f,ccp(0,40));
	CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionScale,actionMove);
	CCActionInterval* actionFade = CCFadeOut::create(1.0f);
	CCActionInterval* bezierAction = CCMoveBy::create(1.0f,ccp(0,80));
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(bezierAction,actionFade);
	lable->runAction(CCSequence::create(FirstAction,CCScaleTo::create(0.05f,0.7f),CCDelayTime::create(0.2f),SecondAction,
		NULL));
	if(m_pDamageLabels->find(lable) == m_pDamageLabels->end())
	{
		m_pDamageLabels->insert(make_pair(lable,1.3f));
		if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
			&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
		{
			GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(lable,MAP_Z_OBJ_TOP);
		}
	}

	return lable;
}

CCNode* DamageValueEffect::CreateCritDamageLable(unsigned int damage,CCPoint parentPos,bool isHarmFul)
{
	CCPoint uiPos = parentPos;
	CCSprite* critNode = CCSprite::create();
	critNode->setPosition(ccp(uiPos.x,uiPos.y));
	critNode->setAnchorPoint(ccp(0.5,0.5));

	ASprite *as = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin"); 
	CCPoint pt;
	int iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_CRITICALBACK");
	CCSprite* critSprite = as->getSpriteFromFrame_Middle(iFrameID,0,pt);
	if(critSprite)
	{
		critSprite->setPosition(CCPointZero);
		critNode->addChild(critSprite,0);
	}

	char damageString[10];
	sprintf(damageString,"%d",damage);
	CCLabelAtlas* label;
	if(isHarmFul)
	{
		label = CCLabelAtlas::create(damageString,"UI/ui_numble_2.png",43,54,48);
	}
	else
	{
		label = CCLabelAtlas::create(damageString,"UI/ui_numble_3.png",41,43,48);
	}

	if(label)
	{
		label->setAnchorPoint(ccp(0.5f,0.5f));
		label->setPosition(CCPointZero);
		critNode->addChild(label,1);
		if(critSprite)
		{
			float scale_x = label->getContentSize().width / (0.8f * critSprite->getContentSize().width);
			float scale_y = label->getContentSize().height / (0.8f * critSprite->getContentSize().height);
			float scale = (scale_x > scale_y) ? scale_x : scale_y; 
			critSprite->setScale(scale);
			critSprite->setPosition(CCPointZero);
			critSprite->setAnchorPoint(ccp(0.5f,0.5f));
		}
	}

	critNode->setScale(0);
	critNode->runAction(getBattleUIAction());
	if(m_pDamageLabels->find(critNode) == m_pDamageLabels->end())
	{
		m_pDamageLabels->insert(make_pair(critNode,1.3f));
		//this->addChild(critNode);
		if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
			&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
		{
			GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(critNode,MAP_Z_OBJ_TOP);
		}
	}

	/*if(isHarmFul)
	{
		HandleCritBurst(uiPos,0);
	}*/

	return critNode;
}

CCSprite* DamageValueEffect::CreateMissLabel(CCPoint parentPos)
{
	//CCPoint visibleSize = CCDirector::sharedDirector()->getLevelRenderCameraOffset();

	//float pos_x = parentPos.x - visibleSize.x;
	//float pos_y = parentPos.y - visibleSize.y;
	//CCPoint uiPos = ccp(pos_x, pos_y);
	CCPoint uiPos = parentPos;
	ASprite *as = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin"); 
	CCPoint pt;
	int iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_MISS");
	CCSprite* missSprite = as->getSpriteFromFrame_Middle(iFrameID,0,pt);
	if(missSprite)
	{
		missSprite->setPosition(ccp(uiPos.x,uiPos.y+70));
		missSprite->setAnchorPoint(ccp(0.5,0.5));
		missSprite->setScale(0);

		missSprite->runAction(getBattleUIAction());
		if(m_pDamageLabels->find(missSprite) == m_pDamageLabels->end())
		{
			m_pDamageLabels->insert(make_pair(missSprite,1.3f));
			//this->addChild(missSprite);
			if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
				&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
			{
				GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(missSprite,MAP_Z_OBJ_TOP);
			}
		}
	}
	return missSprite;
}

CCSprite* DamageValueEffect::CreateBlockLabel(CCPoint parentPos)
{
	CCPoint uiPos = parentPos;
	ASprite *as = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin"); 
	CCPoint pt;
	int iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_BLOCK");
	CCSprite* missSprite = as->getSpriteFromFrame_Middle(iFrameID,0,pt);
	if(missSprite)
	{
		missSprite->setPosition(ccp(uiPos.x,uiPos.y+70));
		missSprite->setAnchorPoint(ccp(0.5,0.5));
		missSprite->setScale(0);
		
		missSprite->runAction(getBattleUIAction());
		if(m_pDamageLabels->find(missSprite) == m_pDamageLabels->end())
		{
			m_pDamageLabels->insert(make_pair(missSprite,1.3f));
			//this->addChild(missSprite);
			if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
				&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
			{
				GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(missSprite,MAP_Z_OBJ_TOP);
			}
		}
	}
	return missSprite;
}

CCAction* DamageValueEffect::getBattleUIAction()
{
	CCActionInterval* actionScale = CCScaleTo::create(0.01f,2.5f);
	CCActionInterval* actionMove = CCMoveBy::create(0.01f,ccp(0,40));
	CCSpawn* FirstAction = CCSpawn::createWithTwoActions(actionScale,actionMove);
	CCActionInterval* actionFade = CCFadeOut::create(2.0f);
	CCActionInterval* bezierAction = CCMoveBy::create(1.0f,ccp(0,20));
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(bezierAction,actionFade);
	CCFiniteTimeAction* action = CCSequence::create(FirstAction,CCScaleTo::create(0.05f,0.8f),CCDelayTime::create(0.2f),
		SecondAction, NULL);
	return action;
}

void DamageValueEffect::RemoveLable(CCNode* sender)
{
	//CCLOG("DamageValueEffect::RemoveLable");
	if(sender)
	{
		sender->stopAllActions();
		sender->removeFromParentAndCleanup(true);
	}
}

void DamageValueEffect::moveWithParabola(CCSprite* mSprite, CCPoint startPoint, CCPoint endPoint, float time)
{
	CCActionInterval* actionScale = CCScaleTo::create(1.5,1.0);
	CCActionInterval* actionMove = CCMoveBy::create(1.5,ccp(0,200));
	CCSpawn* action = CCSpawn::createWithTwoActions(actionScale,actionMove);
	CCActionInterval* actionFade = CCFadeOut::create(1.0);
	
}

void DamageValueEffect::HandleCritBurst(CCPoint pt, unsigned int effectType)
{
	unsigned int effectId = effectType;
	if(effectId == 0)
	{
		effectId = rand() % 3 + 152;
	}
	EffectSprite* effect = ParticleManager::Get()->createEffectSprite(effectId,"",false);
	if(effect)
	{
		effect->setPosition(pt);
		float scale = UIManager::sharedManager()->getScaleFactor();
		effect->getRoot()->setScale(scale);
		effect->SetAnim(kType_Play,1,false);

		if(m_pDamageLabels->find(effect) == m_pDamageLabels->end())
		{
			m_pDamageLabels->insert(make_pair(effect,1.0f));
			//this->addChild(critNode);
			if(GameManager::Get()->GetSceneLayer() != NULL && GameManager::Get()->GetSceneLayer()->getLevelLayer() != NULL
				&& GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer())
			{
				GameManager::Get()->GetSceneLayer()->getLevelLayer()->getObjectLayer()->addChild(effect,MAP_Z_OBJ_TOP);
			}
		}
	}
}

void DamageValueEffect::update(float dt)
{
	map<CCNode*,float>::iterator it= m_pDamageLabels->begin();
	while(it != m_pDamageLabels->end())
	{
		float time = it->second;
		time -= dt;
		if(time <= 0)
		{
			CCNode* label = it->first;
			if(label)
			{
				label->stopAllActions();
				label->removeFromParentAndCleanup(true);
				m_pDamageLabels->erase(it++);
				label = NULL;
			}
		}
		else
		{
			it->second = time;
			it++;
		}
	}

}

void DamageValueEffect::clearAllLabel()
{
	m_pDamageLabels->clear();
}