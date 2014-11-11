#include "ComboLayer.h"
#include "MainMenuLayer.h"
#include "UIManager.h"

extern long long millisecondNow();

ComboLayer::ComboLayer()
{
	m_comboNumber = 0;
	m_comboShow = 0;
	m_lastHitTime = 0;
	m_comboNode = NULL;
	m_comboLabel = NULL;
	m_comboFrontLabel = NULL;
	m_comboState = NONO_COMBO;
	m_hidingPoint = CCPointZero;
	m_showingPoint = CCPointZero;
	m_maxComboNumber = 0;
}

ComboLayer::~ComboLayer()
{
	unschedule(schedule_selector(ComboLayer::update));
}

bool ComboLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	float scale = UIManager::sharedManager()->getScaleFactor();
	m_comboNode = CCNode::create();
	m_comboNode->setScale(scale);
	CCSprite* sprite = CCSprite::create("UI/ui_combo.png");
	m_comboNode->addChild(sprite,0);
	sprite->setAnchorPoint(ccp(0.5f,1.0f));
	sprite->setPosition(CCPointZero);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float x =  visibleSize.width - 0.6f * sprite->getContentSize().width * scale;
	float y =  visibleSize.height * 0.68f;
	m_showingPoint = ccp(x,y);
	m_ExtraPoint = ccp(visibleSize.width - 0.8f * sprite->getContentSize().width * scale,y);
	m_hidingPoint = ccpAdd(m_showingPoint,ccp(sprite->getContentSize().width * scale * 1.1f,0));
	m_comboNode->setPosition(m_hidingPoint);
	m_comboNode->setVisible(false);

	this->addChild(m_comboNode);

	schedule(schedule_selector(ComboLayer::update), 0.08f);
	return true;
}

void ComboLayer::update(float time)
{
	switch(m_comboState)
	{
	case NONO_COMBO:
		if(m_comboNumber > 0)
		{
			comboComeIn();
		}
		break;
	case COMBO_SHOWING:
		long long currentTime = millisecondNow();
		if(currentTime - m_lastHitTime > 3000)
		{
			// Ê±¼ä 
			this->comboComeOut();
		}
		else
		{
			if(m_comboNumber != m_comboShow)
			{
				updateComboNum();
			}
		}
		break;
	}
}

void ComboLayer::comboComeIn()
{
	CCAssert(m_comboState != COMBO_COME_IN,"combo is showing");

	m_comboState = COMBO_COME_IN;
	m_comboNode->setVisible(true);
	float scale = UIManager::sharedManager()->getScaleFactor();
	m_comboNode->setScale(scale);
	if(m_comboLabel)
	{
		m_comboLabel->removeFromParentAndCleanup(true);
		m_comboLabel = NULL;
	}

	if(m_comboFrontLabel)
	{
		m_comboFrontLabel->removeFromParentAndCleanup(true);
		m_comboFrontLabel = NULL;
	}

	m_comboShow = m_comboNumber;
	char comboString[10];
	sprintf(comboString,"%d",m_comboShow);
	m_comboLabel = CCLabelAtlas::create(comboString,getFontName(m_comboShow),48,50,48);
	m_comboLabel->setAnchorPoint(ccp(0.5f,0));
/*
	m_comboFrontLabel = CCLabelAtlas::create(comboString,"UI/ui_combo_white.png",48,50,48);
	m_comboFrontLabel->setAnchorPoint(ccp(0.5f,0));
*/
	m_comboNode->addChild(m_comboLabel,1);
	//m_comboNode->addChild(m_comboFrontLabel,2);
	m_comboNode->setPosition(m_hidingPoint);
	CCMoveTo* m_move = CCMoveTo::create(0.15f,m_ExtraPoint);
	CCMoveTo* moveAction = CCMoveTo::create(0.05f,m_showingPoint);
	//CCCallFuncN *rc = CCCallFuncN::create(this,callfuncN_selector(ComboLayer::onComboMoveInOver));
	
	/*m_comboLabel->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCScaleTo::create(0.01f,2.0f,2.0f),CCScaleTo::create(0.1f,1.0f,1.0f),NULL));
	m_comboFrontLabel->runAction(CCSequence::create(CCDelayTime::create(0.4f),CCFadeIn::create(0.1f),CCFadeOut::create(0.1f),NULL));*/

	m_comboNode->runAction(CCSequence::create(m_move,moveAction,
		CCCallFuncN::create(this,callfuncN_selector(ComboLayer::onComboMoveInOver)),
		NULL));
}

void ComboLayer::comboComeOut()
{
	CCAssert(m_comboState != COMBO_COME_OUT,"combo is outing");

	m_comboState = COMBO_COME_OUT;
	m_comboNumber = 0;
	m_comboShow = 0;
	m_lastHitTime = 0;
	float originScaleX = m_comboNode->getScaleX();
	CCScaleTo* m_scaleTo = CCScaleTo::create(0.2f,originScaleX,0.1f);
	CCMoveTo* m_move = CCMoveTo::create(0.2f,m_hidingPoint);
	m_comboNode->runAction(CCSequence::create(m_scaleTo,m_move,
		CCCallFuncN::create(this,callfuncN_selector(ComboLayer::onComboMoveOutOver)),
		NULL));
}

void ComboLayer::updateComboNum()
{
	if(m_comboLabel)
	{
		m_comboLabel->stopAllActions();
		m_comboLabel->removeFromParentAndCleanup(true);
		m_comboLabel = NULL;
	}

	if(m_comboFrontLabel)
	{
		m_comboFrontLabel->stopAllActions();
		m_comboFrontLabel->removeFromParentAndCleanup(true);
		m_comboFrontLabel = NULL;
	}

	if(m_comboShow < m_comboNumber)
	{
		m_comboShow++;
		char comboString[10];
		sprintf(comboString,"%d",m_comboShow);
		m_comboLabel = CCLabelAtlas::create(comboString,getFontName(m_comboShow),48,50,48);
		m_comboLabel->setAnchorPoint(ccp(0.5f,0));
		m_comboLabel->setPosition(CCPointZero);
		m_comboNode->addChild(m_comboLabel,1);
		m_comboLabel->setScale(2.0f);

		m_comboFrontLabel = CCLabelAtlas::create(comboString,"UI/ui_combo_white.png",48,50,48);
		m_comboFrontLabel->setAnchorPoint(ccp(0.5f,0));
		m_comboFrontLabel->setOpacity(0);
		m_comboNode->addChild(m_comboFrontLabel,2);
		//CCScaleTo* action1 = CCScaleTo::create(0.05f,1.7f,1.7f);
		CCScaleTo* action = CCScaleTo::create(0.1f,1.0f,1.0f);
		m_comboLabel->runAction(action);
		m_comboFrontLabel->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.1f),CCFadeOut::create(0.1f),NULL));
		//m_comboLabel->runAction(CCSequence::create(action,CCCallFuncN::create(this,callfuncN_selector(ComboLayer::onComboScaleOver)),NULL));
		//m_comboFrontLabel->runAction(CCSequence::create(action,CCCallFuncN::create(this,callfuncN_selector(ComboLayer::onComboScaleOver)),NULL));
	}
}

void ComboLayer::onComboMoveInOver(CCNode* sender)
{
	CCAssert(m_comboState != COMBO_SHOWING,"combo is already showing");

	m_comboState = COMBO_SHOWING;
}

void ComboLayer::onComboMoveOutOver(CCNode* sender)
{
	CCAssert(m_comboState != NONO_COMBO,"combo is already hiding");

	m_comboState = NONO_COMBO;
	m_comboNode->setVisible(false);
}

void ComboLayer::onComboScaleOver(CCNode* sender)
{
	if(m_comboLabel)
	{
		m_comboLabel->removeFromParentAndCleanup(true);
		m_comboLabel = NULL;
	}

	char comboString[10];
	sprintf(comboString,"%d",m_comboShow);
	m_comboLabel = CCLabelAtlas::create(comboString,"UI/ui_combo_white.png",48,50,48);
	m_comboLabel->setAnchorPoint(ccp(0.5f,0));
	m_comboLabel->setPosition(CCPointZero);
	//m_comboLabel->setOpacity(150);
	//m_comboLabel->runAction(CCFadeIn::create(0.2f));
	m_comboNode->addChild(m_comboLabel,1);
}

void ComboLayer::HandleComboIncrease(unsigned int incremental)
{
	m_comboNumber += incremental;
	m_lastHitTime = millisecondNow();

	if(m_comboNumber > m_maxComboNumber)
	{
		m_maxComboNumber = m_comboNumber;
		if (m_maxComboNumber > MAX_COMBO_NUMBER)
		{
			m_maxComboNumber = MAX_COMBO_NUMBER;
		}
	}

	switch(m_comboState)
	{
	case NONO_COMBO:
		comboComeIn();
		break;
	//case COMBO_SHOWING:
	//	updateComboNum();
	//	break;
	}
}

const char* ComboLayer::getFontName(unsigned int num)
{
	char* fileName;
	if(num >= 50)
	{
		fileName = "UI/ui_combo_red.png";
	}
	else if(num >= 30)
	{
		fileName = "UI/ui_combo_orange.png";
	}
	else if(num >= 10)
	{
		fileName = "UI/ui_combo_yellow.png";
	}
	else
	{
		fileName = "UI/ui_combo_green.png";
	}
	CCString* pRet = CCString::create(fileName);
	return pRet->m_sString.c_str();
}