#include "OnlineNetworkManager.h"


#include "GameManager.h"
#include "EnginePCH.h"
#include "TXGUIHeader.h"
#include "CrossPlatform.h"
#include "XPlatformManager.h"
#include "ILogicManager.h"
#include "GameAudioManager.h"
#include "GameResourceManager.h"

#include "CConnectionData.h"
#include "SkeletonAnimRcsManager.h"
#include "LevelManager.h"
#include "ParticleManager.h"
#include "CameraController.h"
#include "SkillDispMgr.h"
#include "SpriteMonsterMgr.h"
#include "HelloWorldScene.h"
#include "MainLandManager.h"
#include "XLogicManager.h"
#include "NickNameManager.h"
#include "UIControlsApearControler.h"
#include "TipMgr.h"
using namespace TXGUI;

// constructor
GameManager::GameManager() :
m_pLogicMgr(NULL),
m_pSpriteSeer(NULL),
m_spriteSeerTypeId(-1)
{

	// Init Platform device
	XPlatformManager::_SMInit();
	XPlatformManager::m_sPlatformMgr->SetExternalResourceMgr(GameResourceManager::sharedManager());
	XPlatformManager::m_sPlatformMgr->InitDevice();

	// init TXGUI Engine
	TXMainSDM::Init();

	Engine::_SDMInit();

	/// init audio device
	GameAudioManager::sharedManager()->InitDevice();

	SkillDispMgr::Create();
	SpriteMonsterMgr::Create();

}


GameManager::~GameManager()
{
	/// clear cameara controller
	CameraController::purgeInstance();

	SpriteMonsterMgr::Destroy();
	SkillDispMgr::Destroy();
	LevelManager::Destroy();
	ParticleManager::Destroy();
	MainLandManager::Destroy();

	/// un-init audio device
	GameAudioManager::sharedManager()->Destroy();

	Engine::_SDMShutDown();

	// shut down TXGUI Engine
	TXMainSDM::ShutDown();

	// Shut down Platform device
	XPlatformManager::_SMShutDown();
	m_pLogicMgr = NULL;	
	MainLandManager::Destroy();
	NickNameManager::Destroy();
	// ¹Ø±ÕÍøÂç²ã
	OnlineNetworkManager::Destroy();

	GameResourceManager::purgeInstance();
	UserData::Destroy();
	SpriteMonsterMgr::Destroy();
	TipMgr::Destroy();
	

}

SpriteSeer *GameManager::getHero()
{
	return m_pSpriteSeer;
}

void GameManager::setHero(SpriteSeer* hero)
{
	m_pSpriteSeer = hero;
}

int GameManager::getHeroTypeId()
{
	return m_spriteSeerTypeId;
}

void GameManager::setHeroTypeId(int typeId)
{
	m_spriteSeerTypeId = typeId;
}

void GameManager::SetLogicManager(ILogicManager *pILogicMgr)
{
	CCAssert(m_pLogicMgr == NULL, "m_pLogicMgr is not null!");
	m_pLogicMgr = pILogicMgr;
}

CCScene* GameManager::goToSplash()
{
	if(GetSceneLayer())
	{
		GetSceneLayer()->clearCurScene();
	}

	/*CCScene *pScene = HelloWorld::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);*/
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	return CCDirector::sharedDirector()->getRunningScene();
}

SceneLayer* GameManager::GetSceneLayer()
{
	return dynamic_cast<SceneLayer*>(XLogicManager::sharedManager()->GetCurLayer());
}

// process touch
void GameManager::processTouchBegan(CCPoint touchPoint)
{
	bool isReacted = false;
	// player processing..

	// NPC processing...	

}