#include "WaitingLayer.h"
#include "GameManager.h"
#include "SceneLayer.h"
#include "UIDefine.h"

#include "AspriteManager.h"

WaitingLayer::WaitingLayer():
m_pSprite(NULL)
,m_bShow(false)
{
	m_sumHideTimer = 0;
	m_hidingTimer = 0 ;
}

WaitingLayer::~WaitingLayer()
{
	
}

bool WaitingLayer::init()
{
	bool bRet = false;
	do {
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		m_pSprite = AspriteManager::getInstance()->getOneFrame("UI/ui.bin", "map_ui_FRAME_ICON_LOAD");
		m_pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		m_pSprite->setAnchorPoint(ccp(0.5,0.5));
		this->addChild(m_pSprite, 0);


		setTouchEnabled(true);
		bRet = true;
	}while (0);
	return true;
}

void WaitingLayer::update(float dt)
{	
	if (m_sumHideTimer < 1.0f)
	{
		float fDelta = dt * 300;
		float fRotate = m_pSprite->getRotation();
		m_pSprite->setRotation(fRotate + fDelta);
		m_pSprite->setVisible(true);
	}
	else
	{
		m_pSprite->setVisible(false);
		m_hidingTimer += dt;
	}
}

void WaitingLayer::ShowWaiting(bool bShowRightNow /* = true */,float sumTimer /* = 1.0 */)
{
	if (!m_bShow)
	{
		this->setVisible(true);
		m_pSprite->setRotation(0);
		scheduleUpdate();
		m_bShow = true;

		//m_hidingTimer = 0;

	}

	if (bShowRightNow)
	{
		m_sumHideTimer = 0;
	}
	else
	{
		m_sumHideTimer = 2.0f;
	}
}

void WaitingLayer::HideWaiting()
{
	this->setVisible(false);	
	unscheduleUpdate();
    m_bShow = false;
	m_sumHideTimer = 0;
	m_hidingTimer = 0 ;
}

bool WaitingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(isVisible())
	{
		return true;
	}
  
	return false;
}

void WaitingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchMoved(pTouch,pEvent);
}

void WaitingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchEnded(pTouch,pEvent);
}

void WaitingLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchCancelled(pTouch,pEvent);
}

void WaitingLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCWaitingLayerPripority, true);
}


