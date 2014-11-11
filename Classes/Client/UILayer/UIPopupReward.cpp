#include "UIPopupReward.h"
#include "UIManager.h"
#include "GameResourceManager.h"
#include "GameConfigFromLuaManager.h"
#include "TaskConfigFromLuaManager.h"
#include "Localization.h"
#include "AspriteManager.h"
#include "GameFontManager.h"
#include "algorithm"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "LevelLayer.h"
#include "ItemManager.h"
#include "PopupRewardManager.h"
#include "particle_manager.h"
#include "LuaTinkerManager.h"

using namespace TXGUI;

TXGUI::PopupRewardLayer* TXGUI::PopupRewardLayer::selfInstance = 0;

TXGUI::PopupRewardLayer::PopupRewardLayer()
{
	clickEventHander = 0;
	pfnOkClicked = 0;
	pfnCloseClicked = 0;
	messageBodyLabel = 0;
	okBtn = 0;
	okContentLabel = 0;
	mIconPic = NULL;
	mIconFramePic = NULL;
	mHitIconFramePic = NULL;
	mIsWndAnimOver = false;
	mIsModelLayer = false;
	okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));

	this->closeBtn = NULL;
}

TXGUI::PopupRewardLayer::~PopupRewardLayer()
{
	this->stopAllActions();
	selfInstance = 0;
	UIManager::sharedManager()->RemoveUILayout("PopupRewardPanel");
}

void TXGUI::PopupRewardLayer::Destroy()
{
	UIManager::sharedManager()->RemoveUILayout("PopupRewardPanel");
	this->removeFromParentAndCleanup(true);
	this->unscheduleAllSelectors();
	this->stopAllActions();
	selfInstance = 0;
}

void TXGUI::PopupRewardLayer::ResetValue()
{
	mIsWndAnimOver = false;
	std::string value = std::string(Localizatioin::getLocalization("S_OK"));
	okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
}

bool TXGUI::PopupRewardLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();

	messageboxLayer = manager->CreateUILayoutFromFile("UIplist/PopupReward.plist", this, "PopupRewardPanel");

	manager->RegisterMenuHandler("PopupRewardPanel","PopupRewardLayer::okBtnClickEvent",
		menu_selector(PopupRewardLayer::okBtnClickEvent), this);

	manager->RegisterMenuHandler("PopupRewardPanel","PopupRewardLayer::closeBtnClickEvent",
		menu_selector(PopupRewardLayer::closeBtnClickEvent), this);


	messageBodyLabel = messageboxLayer->FindChildObjectByName<UILabel>("message_body");
	okBtn = messageboxLayer->FindChildObjectByName<UIButton>("OkBtn");
	okContentLabel = messageboxLayer->FindChildObjectByName<UILabel>("ok_btn_label");
	okBtn->setMenuInitPriority(KCCMessageBoxContorlPripority);

	closeBtn = messageboxLayer->FindChildObjectByName<UIButton>("closeBtn");
	closeBtn->setVisible(false);
	closeBtn->setMenuInitPriority(KCCMessageBoxContorlPripority);

	mIconPic = messageboxLayer->FindChildObjectByName<UIPicture>("RewardIcon");
	mHitIconFramePic = messageboxLayer->FindChildObjectByName<UIPicture>("HitFrameIcon");
	mIconFramePic = messageboxLayer->FindChildObjectByName<UIPicture>("IconBg");;

	setTouchEnabled(true);

	return true;
}

bool TXGUI::PopupRewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void TXGUI::PopupRewardLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void TXGUI::PopupRewardLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void TXGUI::PopupRewardLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void TXGUI::PopupRewardLayer::registerWithTouchDispatcher()
{
	if (mIsModelLayer)
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCMessageBoxPripority, true);
	}
	else
	{
		CCLayer::registerWithTouchDispatcher();
	}	
}

void TXGUI::PopupRewardLayer::WndAnimEnd(CCNode* sender)
{	
	mIsWndAnimOver = true;
	playUpgradeEffect();
}

bool TXGUI::PopupRewardLayer::Show(const char* message_body,unsigned int rewardId,const char* okBtnContent /* = "" */,
	bool bShowCloseBtn /* = true */,bool bModelLayer /* = false*/,cocos2d::CCObject * handler /* = 0 */, 
	SEL_MenuHandler pfnOkClicked /* = 0 */,SEL_MenuHandler pfnCloselClicked /* = 0 */)
{
	CCSprite* iconSprite = selfInstance->GetSpriteByRewardID(rewardId);
	if (iconSprite == NULL)
	{
		return false;
	}

	if (selfInstance != 0)
	{
		// Note: 销毁原来窗口
		//selfInstance->Destroy();
		return false;
	}

	TXGUI::PopupRewardLayer* pLayer = TXGUI::PopupRewardLayer::create();
	pLayer->mIsModelLayer = bModelLayer;
	selfInstance = pLayer;
	selfInstance->ResetValue();

	MainMenuLayer * Layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (Layer == 0)
	{
		return false;
	}
	int tag = Layer->getLayerTagByName("UIPopupRewardLayer");
	Layer->addChild(pLayer,Z_Order_PopupDialogLayer,tag);
	pLayer->setVisible(true);

	float deltaDst = 200;
	CCPoint oldLayerPt = pLayer->getPosition();
	pLayer->setPosition(ccp(oldLayerPt.x,oldLayerPt.y - deltaDst));

	// Note: 设置动画效果
	selfInstance->mIsWndAnimOver = false;
	CCActionInterval* actionMove2 = CCMoveBy::create(0.6f,ccp(0,deltaDst));
	CCActionInterval* actionFade = CCFadeOut::create(0.6f);
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(actionMove2,actionFade);
	pLayer->runAction(CCSequence::create(SecondAction,CCCallFuncN::create(selfInstance,callfuncN_selector(PopupRewardLayer::WndAnimEnd)),NULL));

	std::string messageContent = Localizatioin::getLocalization(message_body);
	pLayer->messageBodyLabel->setString(messageContent.c_str());

	pLayer->clickEventHander = handler;
	pLayer->pfnOkClicked = pfnOkClicked;
	pLayer->pfnCloseClicked = pfnCloselClicked;

	pLayer->closeBtn->setVisible(bShowCloseBtn);

	if (okBtnContent == "")
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	}
	else
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization(okBtnContent));;
	}
	selfInstance->okContentLabel->setString(selfInstance->okBtnContent.c_str());

	if (selfInstance->mHitIconFramePic)
	{
		selfInstance->mHitIconFramePic->setVisible(false);
	}
	if (selfInstance->mIconPic)
	{
		selfInstance->mIconPic->setSprite(iconSprite);		
	}	

	return true;
}

bool TXGUI::PopupRewardLayer::Show(const char* message_body,unsigned int rewardId,const char* okBtnContent /* = "" */,
	bool bShowCloseBtn /* = true */,bool bModelLayer /* = false */, int nOkBtnHandler /* = 0 */,int nCloseBtnHandler /* = 0 */)
{
	CCSprite* iconSprite = selfInstance->GetSpriteByRewardID(rewardId);
	if (iconSprite == NULL)
	{
		return false;
	}

	if (selfInstance != 0)
	{
		// Note: 销毁原来窗口
		//selfInstance->Destroy();
		return false;
	}

	TXGUI::PopupRewardLayer* pLayer = TXGUI::PopupRewardLayer::create();
	pLayer->mIsModelLayer = bModelLayer;
	selfInstance = pLayer;
	selfInstance->ResetValue();

	MainMenuLayer * Layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (Layer == 0)
	{
		return false;
	}
	int tag = Layer->getLayerTagByName("UIPopupRewardLayer");
	Layer->addChild(pLayer,Z_Order_PopupDialogLayer,tag);
	pLayer->setVisible(true);

	float deltaDst = 200;
	CCPoint oldLayerPt = pLayer->getPosition();
	pLayer->setPosition(ccp(oldLayerPt.x,oldLayerPt.y - deltaDst));

	// Note: 设置动画效果
	selfInstance->mIsWndAnimOver = false;
	CCActionInterval* actionMove2 = CCMoveBy::create(0.6f,ccp(0,deltaDst));
	CCActionInterval* actionFade = CCFadeOut::create(0.6f);
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(actionMove2,actionFade);
	pLayer->runAction(CCSequence::create(SecondAction,CCCallFuncN::create(selfInstance,callfuncN_selector(PopupRewardLayer::WndAnimEnd)),NULL));

	std::string messageContent = Localizatioin::getLocalization(message_body);
	pLayer->messageBodyLabel->setString(messageContent.c_str());

	pLayer->clickEventHander = NULL;
	pLayer->pfnOkClicked = NULL;
	pLayer->pfnCloseClicked = NULL;

	pLayer->okBtn->getMenuItemSprite()->registerScriptTapHandler(nOkBtnHandler);
	pLayer->closeBtn->getMenuItemSprite()->registerScriptTapHandler(nCloseBtnHandler);

	pLayer->closeBtn->setVisible(bShowCloseBtn);

	if (okBtnContent == "")
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	}
	else
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization(okBtnContent));;
	}
	selfInstance->okContentLabel->setString(selfInstance->okBtnContent.c_str());

	if (selfInstance->mIconPic)
	{
		selfInstance->mIconPic->setSprite(iconSprite);		
	}
	if (selfInstance->mHitIconFramePic)
	{
		selfInstance->mHitIconFramePic->setVisible(false);
	}

	return true;
}

void TXGUI::PopupRewardLayer::DisplayOver(float dt)
{
	if (selfInstance != 0)
	{
		// Note: 销毁原来窗口
		selfInstance->Destroy();
	}
}

bool TXGUI::PopupRewardLayer::Show(const char* message_body,const char* rewardIcon,const char* okBtnContent /* = "" */,bool bShowCloseBtn /* = true */,
	bool bModelLayer /* = false */, int nOkBtnHandler /* = 0 */,int nCloseBtnHandler /* = 0 */,float displayTime)
{
	CCSprite* iconSprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,rewardIcon);
	if (iconSprite == NULL)
	{
		return false;
	}

	if (selfInstance != 0)
	{
		// Note: 销毁原来窗口
		//selfInstance->Destroy();
		return false;
	}

	TXGUI::PopupRewardLayer* pLayer = TXGUI::PopupRewardLayer::create();
	pLayer->mIsModelLayer = bModelLayer;
	selfInstance = pLayer;
	selfInstance->ResetValue();

	if (displayTime != 0.0)
	{
		selfInstance->scheduleOnce(schedule_selector(TXGUI::PopupRewardLayer::DisplayOver),displayTime);
	}

	MainMenuLayer * Layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (Layer == 0)
	{
		return false;
	}
	int tag = Layer->getLayerTagByName("UIPopupRewardLayer");
	Layer->addChild(pLayer,Z_Order_PopupDialogLayer,tag);
	pLayer->setVisible(true);

	float deltaDst = 200;
	CCPoint oldLayerPt = pLayer->getPosition();
	pLayer->setPosition(ccp(oldLayerPt.x,oldLayerPt.y - deltaDst));

	// Note: 设置动画效果
	selfInstance->mIsWndAnimOver = false;
	CCActionInterval* actionMove2 = CCMoveBy::create(0.6f,ccp(0,deltaDst));
	CCActionInterval* actionFade = CCFadeOut::create(0.6f);
	CCSpawn* SecondAction = CCSpawn::createWithTwoActions(actionMove2,actionFade);
	pLayer->runAction(CCSequence::create(SecondAction,CCCallFuncN::create(selfInstance,callfuncN_selector(PopupRewardLayer::WndAnimEnd)),NULL));

	std::string messageContent = Localizatioin::getLocalization(message_body);
	pLayer->messageBodyLabel->setString(messageContent.c_str());

	pLayer->clickEventHander = NULL;
	pLayer->pfnOkClicked = NULL;
	pLayer->pfnCloseClicked = NULL;

	pLayer->okBtn->getMenuItemSprite()->registerScriptTapHandler(nOkBtnHandler);
	pLayer->closeBtn->getMenuItemSprite()->registerScriptTapHandler(nCloseBtnHandler);

	pLayer->closeBtn->setVisible(bShowCloseBtn);

	if (okBtnContent == "")
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	}
	else
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization(okBtnContent));;
	}
	selfInstance->okContentLabel->setString(selfInstance->okBtnContent.c_str());

	if (selfInstance->mIconPic)
	{
		selfInstance->mIconPic->setSprite(iconSprite);		
	}
	if (selfInstance->mIconFramePic)
	{
		selfInstance->mIconFramePic->setVisible(false);
	}

	return true;
}

CCSprite* TXGUI::PopupRewardLayer::GetSpriteByRewardID(unsigned int rewardId)
{
	if (rewardId >= 4000000 && rewardId <= 4999999)
	{
		CCSprite* iconSprite = NULL;
		const char* iconName = GameConfigFromLuaManager::Get()->GetOneSkillIconName(rewardId);
		if (iconName)
		{
			iconSprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,iconName);
		}
		return iconSprite;
	}
	else
	{
		return ItemManager::Get()->getIconSpriteById(rewardId);
	}	
}

void TXGUI::PopupRewardLayer::AdjustStyle(string okBtnContent /* = "" */,unsigned int fontSize /* = 0 */,ccColor3B labelColor /* = ccc3 */)
{
	if (selfInstance != NULL && selfInstance->okContentLabel != NULL)
	{
	}
	if (okBtnContent != "")
	{
		selfInstance->okBtnContent = std::string(Localizatioin::getLocalization(okBtnContent.c_str()));
		selfInstance->okContentLabel->setString(selfInstance->okBtnContent.c_str());
	}

	if (fontSize != 0)
	{
		//selfInstance->okContentLabel->setContentSize(CCSize(fontSize,fontSize));
	}

	if (labelColor.r != ccWHITE.r || labelColor.g != ccWHITE.g || labelColor.b != ccWHITE.b)
	{
		selfInstance->okContentLabel->setColor(labelColor);
	}

}

void TXGUI::PopupRewardLayer::okBtnClickEvent(CCObject* sender)
{
	if (false == mIsWndAnimOver)
	{
		return ;
	}
	if (this->clickEventHander != 0 && pfnOkClicked != 0)
	{
		(this->clickEventHander->*pfnOkClicked)(sender);
	}
	//PopupRewardManager::Get()->OnPopupRewardLayerClose();
	Destroy();
}

void TXGUI::PopupRewardLayer::closeBtnClickEvent(CCObject* sender)
{
	if (false == mIsWndAnimOver)
	{
		return ;
	}

	if (this->clickEventHander != 0 && this->pfnCloseClicked != 0)
	{
		(this->clickEventHander->*pfnCloseClicked)(sender);
	}
	Destroy();
}

void TXGUI::PopupRewardLayer::playUpgradeEffect()
{
	if(mIconPic && mIconPic->getCurrentNode())
	{
		CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_upgradesuccess");
		if(particle != NULL)
		{
			CCNode* child = mIconPic->getCurrentNode()->getChildByTag(100);
			if(child)
			{
				child->removeFromParentAndCleanup(true);
				child = NULL;
			}
			mIconPic->getCurrentNode()->addChild(particle,100,100);
			particle->setPosition(ccp(mIconPic->getCurrentNode()->getContentSize().width * 0.5,mIconPic->getCurrentNode()->getContentSize().height * 0.5));
		}
	}
}