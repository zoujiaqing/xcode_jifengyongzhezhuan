#include "ForcePKAnimLayer.h"
#include "UIManager.h"
#include "GameResourceManager.h"
#include "Localization.h"
#include "MainMenuLayer.h"
#include "LuaTinkerManager.h"
#include "ParticleManager.h"
#include "GameAudioManager.h"
#include "SkeletonAnimRcsManager.h"
using namespace TXGUI;

ForcePKAnimLayer::ForcePKAnimLayer()
	: m_pPKEffect(NULL)
	, m_pBackGroundPic(NULL)
	, m_pAnimEndListerner(NULL)
	, m_pfnAnimEndSelector(NULL)
	, m_isAnimPlayOver(false)
	, m_isPlaySound(false)
	, m_nPlaySoundFrame(8)
{
}

ForcePKAnimLayer::~ForcePKAnimLayer()
{
	if(m_pPKEffect)
	{
		EnginePlayerManager::getInstance()->removeEffectAsprite(m_pPKEffect);
		m_pPKEffect = NULL;
	}

	UIManager::sharedManager()->RemoveUILayout("ForcePKAnimPanel");
}

bool ForcePKAnimLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();
	if (manager)
	{
		UILayout* pForcePKAnimLayout = manager->CreateUILayoutFromFile("UIplist/forcePkAnimLayer.plist", this, "ForcePKAnimPanel");
		if (pForcePKAnimLayout)
		{
			m_pBackGroundPic = pForcePKAnimLayout->FindChildObjectByName<UIPicture>("BackgroundPic");
			if (m_pBackGroundPic)
			{
				SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(247);
				m_pPKEffect = ParticleManager::Get()->createEffectSprite(247,"");
				if (m_pPKEffect)
				{					
					m_pPKEffect->SetAnim(kType_Play,1,true,this);
					m_pPKEffect->SetAnimPauseOnOver(true);
					m_pBackGroundPic->getCurrentNode()->addChild(m_pPKEffect,1);
					CCSize size = m_pBackGroundPic->getCurrentNode()->getContentSize();;					
					m_pPKEffect->setPosition(ccp(size.width / 2,size.height / 2));
				}
			}
		}
	}	
	GameAudioManager::sharedManager()->playEffect(350014,false);
	return true;
}

bool ForcePKAnimLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void ForcePKAnimLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void ForcePKAnimLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void ForcePKAnimLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void ForcePKAnimLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void ForcePKAnimLayer::SetAnimPauseOnEnd(bool bPauseOnEnd)
{
	if (m_pPKEffect)
	{
		m_pPKEffect->SetAnimPauseOnOver(bPauseOnEnd);
	}
}

void ForcePKAnimLayer::SetAnimEndCallBack(CCObject* pListerner,SEL_CallFunc callBack)
{
	m_pAnimEndListerner = pListerner;
	m_pfnAnimEndSelector = callBack;
}

void ForcePKAnimLayer::animationHandler(const char* _aniType, const char* _aniID, const char* _frameID)
{
	if (0 == strcmp(_aniType,"loopComplete") ||
		0 == strcmp(_aniType,"complete"))
	{
		if (m_pAnimEndListerner && m_pfnAnimEndSelector)
		{
			(m_pAnimEndListerner->*m_pfnAnimEndSelector)();
		}
	}
	else
	{
		if (_frameID && _frameID != "")
		{
			if (false == m_isPlaySound)
			{
				unsigned int nFrame = atoi(_frameID);
				if (nFrame >= m_nPlaySoundFrame)
				{
					GameAudioManager::sharedManager()->playEffect(350012,false);
					m_isPlaySound = true;
				}
			}
		}		
	}
}