#include "OnlineNetworkManager.h"
#include "GameFontManager.h"

#include "MainMenuLayer.h"
#include "GameDataManager.h"
#include "Localization.h"
#include "BattleCoverLayer.h"
#include "SpriteSeer.h"
#include "GameManager.h"
#include "LevelLayer.h"
#include "BaseListener.h"
#include "GameResourceManager.h"

using namespace TXGUI;

BattleCoverLayer::BattleCoverLayer()	
{
	
}

BattleCoverLayer::~BattleCoverLayer()
{
	UIManager::sharedManager()->RemoveUILayout("BattleCover");
}

bool BattleCoverLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	setTouchEnabled(true);
	UIManager *manager = UIManager::sharedManager();

	manager->CreateUILayoutFromFile("UIplist/battleCover.plist", this, "BattleCover");

	//³õÊ¼»¯±êÇ©
	//int fontSize = 28;
	//float factor = manager->getScaleFactor();
	//CCSize nameLabelSize = CCSizeMake(500 / CC_CONTENT_SCALE_FACTOR(), fontSize * factor * 2.0f);

	//infoLabel = UILabelTTF::create("", KJLinXin, fontSize * factor, 
	//	nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);	

	m_autoFightInfoLabel = CCSprite::create("UI/ui_autofight.png");
	m_autoFightInfoLabel->setScale(manager->getScaleFactor());

	m_autoFindWayInfoLabel = CCSprite::create("UI/ui_autofindway.png");
	m_autoFindWayInfoLabel->setScale(manager->getScaleFactor());

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	m_autoFightInfoLabel->setPosition(ccp(visibleSize.width * 0.5f, visibleSize.height * 0.73f));
	m_autoFindWayInfoLabel->setPosition(ccp(visibleSize.width * 0.5f, visibleSize.height * 0.73f));

	this->addChild(m_autoFightInfoLabel);
	this->setVisible(false);

	this->addChild(m_autoFindWayInfoLabel);
	this->setVisible(false);

	return true;
}

void BattleCoverLayer::openBattleCoverLayer(COVER_TYPE coverType)
{
	this->setVisible(true);		
	this->layerType = coverType;

	m_autoFightInfoLabel->setVisible(false);
	m_autoFindWayInfoLabel->setVisible(false);

	switch (coverType)
	{
	case COVER_AUTOATTACK:
		//infoLabel->setString(autoAttackText.c_str());
		m_autoFightInfoLabel->setVisible(true);
		break;

	case COVER_WAITINGOTHER:
		//infoLabel->setString(waitText.c_str());
		m_autoFightInfoLabel->setVisible(false);
		break;

	case COVER_AUTOSEARCH:
		//infoLabel->setString(autoSearchText.c_str());
		m_autoFindWayInfoLabel->setVisible(true);
		break;

	case COVER_NONE:
		//infoLabel->setString("");
		m_autoFightInfoLabel->setVisible(false);
		break;

	default:
		//infoLabel->setString("");
		m_autoFightInfoLabel->setVisible(false);
		break;
	}
}

void BattleCoverLayer::closeBattleCoverLayer(CCObject* sender)
{
	this->setVisible(false);
	//infoLabel->setVisible(false);
	m_autoFightInfoLabel->setVisible(false);
	m_autoFindWayInfoLabel->setVisible(false);
	layerType = COVER_NONE;
}

void BattleCoverLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

bool BattleCoverLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	bool ret = false;
	SpriteSeer * hero = GameManager::Get()->getHero();
	if (this->isVisible())
	{
		switch (layerType)
		{
		case COVER_AUTOATTACK:
			
		
			GameDataManager::Get()->setHeroAutoAttack(false);
			closeBattleCoverLayer(this);
			ret = true;
			break;

		case COVER_WAITINGOTHER:
			ret = true;
			break;

		case COVER_AUTOSEARCH:			
			
			if (hero)
			{
				BaseListener* listener = LevelLayer::sGetListenerByTag(hero->getTag());
				if (listener)
				{
					listener->HandleLayerTouchBeginEvent(hero, hero->getPosition());
				}		
			}
			
			closeBattleCoverLayer(this);
			ret = true;
			break;

		case COVER_NONE:
			break;

		default:
			break;
		}		
	}
	return ret;
}

