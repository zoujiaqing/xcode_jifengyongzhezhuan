#include "AspriteManager.h"
#include "XLogicManager.h"
#include "TXGUIHeader.h"
#include "BattleWinLayer.h"
#include "GameManager.h"

using namespace TXGUI;

BattleWinLayer::BattleWinLayer()
{

}

BattleWinLayer::~BattleWinLayer()
{
	UIManager::sharedManager()->RemoveUILayout("BattleWin");
}

bool BattleWinLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	m_curUiAs = AspriteManager::getInstance()->getAsprite("UI/ui_win.bin");
	UIManager *manager = UIManager::sharedManager();

	manager->CreateUILayoutFromFile("UIplist/battleWin.plist", this, "BattleWin");

	manager->RegisterMenuHandler("BattleWin","BattleWinLayer::clickOk",
		menu_selector(BattleWinLayer::clickOk), this);
	UILayout *layout = manager->getUILayout("BattleWin");
    
	//UIPicture *pic = layout->FindChildObjectByName<UIPicture>("winBackPic");
	//UIButton *bu = layout->FindChildObjectByName<UIButton>("backToTown");

	return true;
}

void BattleWinLayer::clickOk(CCObject *sender)
{
	XLogicManager::sharedManager()->backToTown();
	if (GameManager::Get()->GetSceneLayer())
	{
		GameManager::Get()->GetSceneLayer()->showLoading(true);
	}	
}

void BattleWinLayer::ShowLayout()
{
	this->setVisible(true);
	UIManager::sharedManager()->SetOtherUILayoutTouchEnableExcept(false,"BattleWin");
}

void BattleWinLayer::HideLayout()
{
	this->setVisible(false);
	UIManager::sharedManager()->SetOtherUILayoutTouchEnableExcept(true,"BattleWin");
}