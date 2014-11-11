#include "OnlineNetworkManager.h"
#include "SpriteElfLayer.h"
#include <stdlib.h>
#include <stdio.h>
#include "UIManager.h"
#include "TXGUIHeader.h"
#include "GameFontManager.h"
#include "CrossUtil.h"
#include "MainMenuLayer.h"
#include "Localization.h"
#include "ChatToolbar.h"
#include "CCLuaEngine.h"
#include "GameConfigFromLuaManager.h"
#include "SpriteFactory.h"
#include "SpriteElfConfigFromLua.h"

using namespace TXGUI;

SpriteElfLayer::SpriteElfLayer()
{

}

SpriteElfLayer::~SpriteElfLayer()
{
	/// hide chat toolbar
	//HideChatToolbar();

	UIManager::sharedManager()->RemoveUILayout("SpriteElfPanel");
	//UIManager::sharedManager()->RemoveUILayout("chatToolbar");

	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void SpriteElfLayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

bool SpriteElfLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	/// 设置成模态的图层

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	/// SHOW chat toolbar here
	//ChatToolbar* chatbar = GetChatToolbar();
	//if (chatbar)
	//{
	//	chatbar->setVisible(true);
	//	this->addChild(chatbar);
	//}
	GameConfigFromLuaManager::Get()->InitLayer((CCNode*)this,"InitElfLayer");
	//CCLuaEngine::defaultEngine()->executeGlobalFunction("InitElfLayer");

	/// show ppve panel
	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/SpriteElfPanel.plist", this, "SpriteElfPanel", true);
	manager->RegisterMenuHandler("SpriteElfPanel","SpriteElfLayer::OnClose",	menu_selector(SpriteElfLayer::OnClose), this);

	// Note: 显示精灵
	//SpriteElfConfigFromLuaManager::getInstance()->CallLuaStartFunction();
	//CCLuaEngine::defaultEngine()->executeGlobalFunction("InitSpriteElfLayer");
	CCLuaEngine::defaultEngine()->executeGlobalFunction("InitSpriteElfLayer");

	return true;
}


void SpriteElfLayer::OnClose(CCObject *sender)
{	
	this->removeFromParentAndCleanup(true);
	SpriteFactory::sharedFactory()->ResetValue();
}

void   SpriteElfLayer::OnClickListItem(CCObject* sender)
{
	int Tag = ((CCNode*)(sender))->getTag();
	CCLOG("OnClick Room:%d", Tag);
}