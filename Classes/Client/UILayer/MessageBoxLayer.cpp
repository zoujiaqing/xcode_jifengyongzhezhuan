#include "MessageBoxLayer.h"
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
#include "HelloWorldScene.h"
#include "UpdateResourceLayer.h"
#include "XLogicManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "NcSDKManager.h"
#endif

using namespace TXGUI;

TXGUI::MessageBox* TXGUI::MessageBox::selfInstance = 0;
bool TXGUI::MessageBox::sUseHightPriotiry = false;

TXGUI::MessageBox::MessageBox()
{
	clickEventHander = 0;
	pfnMiddleClicked = 0;
	pfnOkClicked = 0;
	pfnCancelClicked = 0;
	messageBodyLabel = 0;
	messageBodyLabelExt = 0;
	middleBtn = 0;
	okBtn = 0;
	cancelBtn = 0;
	middleContentLabel = 0;
	okContentLabel = 0;
	cancelContentLabel = 0;
	m_nMessagePriority = -1;
	middleBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	cancelBtnContent = std::string(Localizatioin::getLocalization("S_CANCEL"));
    isOkBtnDefaultBehavior = false;
    //this->closeBtn = NULL;
}

TXGUI::MessageBox::~MessageBox()
{
	//selfInstance = 0;
	//UIManager::sharedManager()->RemoveUILayout("MessageBoxPanel");
}

void TXGUI::MessageBox::Destroy()
{
	UIManager::sharedManager()->RemoveUILayout("MessageBoxPanel");
	selfInstance = 0;
	m_nMessagePriority = -1;
	this->removeFromParentAndCleanup(true);
}

void TXGUI::MessageBox::ResetValue()
{
	std::string value = std::string(Localizatioin::getLocalization("S_OK"));
	middleBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	okBtnContent = std::string(Localizatioin::getLocalization("S_OK"));
	cancelBtnContent = std::string(Localizatioin::getLocalization("S_CANCEL"));
}

void TXGUI::MessageBox::setMessagePriority(int var)
{
	m_nMessagePriority = var;
}

int TXGUI::MessageBox::getMessagePriority()
{
	return m_nMessagePriority;
}

bool TXGUI::MessageBox::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();

	messageboxLayer = manager->CreateUILayoutFromFile("UIplist/msgBox.plist", this, "MessageBoxPanel");

	manager->RegisterMenuHandler("MessageBoxPanel","MessageBox::middleBtnClickEvent",
		menu_selector(MessageBox::middleBtnClickEvent), this);

	manager->RegisterMenuHandler("MessageBoxPanel","MessageBox::okBtnClickEvent",
		menu_selector(MessageBox::okBtnClickEvent), this);

	manager->RegisterMenuHandler("MessageBoxPanel","MessageBox::cancelBtnClickEvent",
		menu_selector(MessageBox::cancelBtnClickEvent), this);
    
    manager->RegisterMenuHandler("MessageBoxPanel","MessageBox::closeBtnClickEvent",
                                 menu_selector(MessageBox::closeBtnClickEvent), this);
    
	messageBodyLabel = messageboxLayer->FindChildObjectByName<UILabel>("message_body");
	messageBodyLabelExt = messageboxLayer->FindChildObjectByName<UILabel>("message_body1");
	middleBtn = messageboxLayer->FindChildObjectByName<UIButton>("MiddleBtn");
	okBtn = messageboxLayer->FindChildObjectByName<UIButton>("OkBtn");
	cancelBtn = messageboxLayer->FindChildObjectByName<UIButton>("CancelBtn");

	middleContentLabel = messageboxLayer->FindChildObjectByName<UILabel>("middle_btn_label");
	okContentLabel = messageboxLayer->FindChildObjectByName<UILabel>("ok_btn_label");
	cancelContentLabel = messageboxLayer->FindChildObjectByName<UILabel>("cancel_btn_label");
    
    closeBtn = messageboxLayer->FindChildObjectByName<UIButton>("closeBtn");
    closeBtn->setVisible(false);

	if (sUseHightPriotiry)
	{
		middleBtn->setMenuInitPriority(KCCMessageBoxContorlPripority_2);
		okBtn->setMenuInitPriority(KCCMessageBoxContorlPripority_2);
		cancelBtn->setMenuInitPriority(KCCMessageBoxContorlPripority_2);
		closeBtn->setMenuInitPriority(KCCMessageBoxContorlPripority_2);
	}
	else
	{
		middleBtn->setMenuInitPriority(KCCMessageBoxContorlPripority);
		okBtn->setMenuInitPriority(KCCMessageBoxContorlPripority);
		cancelBtn->setMenuInitPriority(KCCMessageBoxContorlPripority);
		closeBtn->setMenuInitPriority(KCCMessageBoxContorlPripority);
	}

    setTouchEnabled(true);
    
	return true;
}

bool TXGUI::MessageBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void TXGUI::MessageBox::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void TXGUI::MessageBox::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void TXGUI::MessageBox::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void TXGUI::MessageBox::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	if (sUseHightPriotiry)
	{
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCMessageBoxPripority_2, true);
	}
	else
	{
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCMessageBoxPripority, true);
	}
}

void TXGUI::MessageBox::Show(const char* message_body,cocos2d::CCObject * handler /* = 0 */, 
	SEL_MenuHandler pfnOkClicked /* = 0 */,SEL_MenuHandler pfnCancelClicked /* = 0 */, 
	unsigned int  uType /* = MB_YESNO */,ccColor3B labelColor /* = ccc3 */,int fontSize /* = 32 */,
	int priority /* = KMessageBoxDefaultPriority*/)
{
	if (selfInstance != 0)
	{
		if(priority < selfInstance->getMessagePriority())
		{
			// 比当前优先级低，不接收新的消息弹框
			return;
		}
		// Note: 销毁原来窗口
		selfInstance->Destroy();
	}

	TXGUI::MessageBox* msgLayer = TXGUI::MessageBox::create();
	selfInstance = msgLayer;
	selfInstance->ResetValue();
	selfInstance->setMessagePriority(priority);

	int tag = MainMenuLayer::getLayerTagByName("MessageBox");
	if(GameManager::Get()->GetSceneLayer() != NULL)
	{
		MainMenuLayer * Layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (Layer != NULL)
		{
			Layer->addChild(msgLayer,Z_Order_MessageBox,tag);
		}
	}
	else
	{
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCNode* node = scene->getChildByTag(XLogicManager::E_SELECT_ZONE);
		HelloWorld* layer = dynamic_cast<HelloWorld*>(node);
		if(layer == NULL)
		{
			CCNode* pNode = scene->getChildByTag(XLogicManager::E_UPDATE_RESOURCE);
			if (pNode)
			{
				UpdateResourceLayer* pLayer = dynamic_cast<UpdateResourceLayer*>(pNode);
				if (pLayer)
				{
					pLayer->addChild(msgLayer,Z_Order_MessageBox,tag);
				}
			}
			else
			{
				sUseHightPriotiry = false;
				return ;
			}
		}
		else
		{
			layer->addChild(msgLayer,Z_Order_MessageBox,tag);
		}
	}
		
	msgLayer->setVisible(true);

	msgLayer->messageBodyLabel->setString(message_body);
	msgLayer->messageBodyLabel->setColor(labelColor);
	if(msgLayer->messageBodyLabel->getLabelTTF() != NULL && msgLayer->messageBodyLabel->getLabelTTF()->GetTTF() != NULL)
	{
		msgLayer->messageBodyLabel->getLabelTTF()->GetTTF()->setFontSize(fontSize);
	}

	msgLayer->clickEventHander = handler;
	msgLayer->pfnMiddleClicked = pfnOkClicked;
	msgLayer->pfnOkClicked = pfnOkClicked;
	msgLayer->pfnCancelClicked = pfnCancelClicked;

	msgLayer->middleBtn->setVisible(false);
	msgLayer->okBtn->setVisible(true);
	msgLayer->cancelBtn->setVisible(true);

	msgLayer->middleContentLabel->setVisible(false);
	msgLayer->okContentLabel->setVisible(true);
	msgLayer->cancelContentLabel->setVisible(true);

	if (uType == MB_OK)
	{
		CCPoint okBtnPos = msgLayer->okBtn->getPosition();
		CCPoint cancelBtnPos = msgLayer->cancelBtn->getPosition();
		CCPoint midPos = ccpAdd(okBtnPos,cancelBtnPos);

		msgLayer->cancelBtn->setVisible(false);
		msgLayer->cancelContentLabel->setVisible(false);
		msgLayer->okBtn->setPosition(ccp(midPos.x/2,midPos.y/2));
		msgLayer->okContentLabel->setPosition(ccp(midPos.x/2,midPos.y/2));
	}
	if(uType == MB_OKCANCELCLOSE)
	{
		msgLayer->closeBtn->setVisible(true);
	}
	if(uType == MB_MIDDLECLOSE)
	{
		msgLayer->closeBtn->setVisible(true);

		msgLayer->middleBtn->setVisible(true);
		msgLayer->okBtn->setVisible(false);
		msgLayer->cancelBtn->setVisible(false);

		msgLayer->middleContentLabel->setVisible(true);
		msgLayer->okContentLabel->setVisible(false);
		msgLayer->cancelContentLabel->setVisible(false);
	}

	selfInstance->middleContentLabel->setString(selfInstance->middleBtnContent.c_str());
	selfInstance->okContentLabel->setString(selfInstance->okBtnContent.c_str());
	selfInstance->cancelContentLabel->setString(selfInstance->cancelBtnContent.c_str());
	
	sUseHightPriotiry = false;
}

void TXGUI::MessageBox::Show(const char* message_body,int pfnOkHandler,int pfnCancelHandler,
	unsigned int uType/* = MB_YESNO*/,ccColor3B labelColor/* = ccc3(255,255,255)*/,int fontSize/* = 24*/,
	int priority /*= KMessageBoxDefaultPriority*/)
{
	if (selfInstance != 0)
	{
		if(priority < selfInstance->getMessagePriority())
		{
			// 比当前优先级低，不接收新的消息弹框
			return;
		}
		// Note: 销毁原来窗口
		selfInstance->Destroy();
	}
	
	TXGUI::MessageBox* msgLayer = TXGUI::MessageBox::create();
	selfInstance = msgLayer;
	selfInstance->ResetValue();
	selfInstance->setMessagePriority(priority);

	int tag = MainMenuLayer::getLayerTagByName("MessageBox");
	if(GameManager::Get()->GetSceneLayer() != NULL)
	{
		MainMenuLayer * Layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (Layer != NULL)
		{
			Layer->addChild(msgLayer,Z_Order_MessageBox,tag);
		}
	}
	else
	{
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCNode* node = scene->getChildByTag(XLogicManager::E_SELECT_ZONE);
		HelloWorld* layer = dynamic_cast<HelloWorld*>(node);
		if(layer == NULL)
		{
			sUseHightPriotiry = false;
			return ;
		}
		else
		{
			layer->addChild(msgLayer,Z_Order_MessageBox,tag);
		}
	}
	msgLayer->setVisible(true);

	msgLayer->messageBodyLabel->setString(message_body);
	msgLayer->messageBodyLabel->setColor(labelColor);
	msgLayer->clickEventHander = NULL;
	msgLayer->pfnOkClicked = NULL;
	msgLayer->pfnCancelClicked = NULL;
	msgLayer->pfnMiddleClicked = NULL;

	msgLayer->middleBtn->setVisible(false);
	msgLayer->okBtn->setVisible(true);
	msgLayer->cancelBtn->setVisible(true);

	msgLayer->middleContentLabel->setVisible(false);
	msgLayer->okContentLabel->setVisible(true);
	msgLayer->cancelContentLabel->setVisible(true);

	msgLayer->middleBtn->getMenuItemSprite()->registerScriptTapHandler(pfnOkHandler);
	msgLayer->okBtn->getMenuItemSprite()->registerScriptTapHandler(pfnOkHandler);
	msgLayer->cancelBtn->getMenuItemSprite()->registerScriptTapHandler(pfnCancelHandler);

	if (uType == MB_OK)
	{
		CCPoint okBtnPos = msgLayer->okBtn->getPosition();
		CCPoint cancelBtnPos = msgLayer->cancelBtn->getPosition();
		CCPoint midPos = ccpAdd(okBtnPos,cancelBtnPos);

		msgLayer->cancelBtn->setVisible(false);
		msgLayer->cancelContentLabel->setVisible(false);
		msgLayer->okBtn->setPosition(ccp(midPos.x/2,midPos.y/2));
		msgLayer->okContentLabel->setPosition(ccp(midPos.x/2,midPos.y/2));
	}
	if(uType == MB_OKCANCELCLOSE)
	{
		msgLayer->closeBtn->setVisible(true);
	}
	if(uType == MB_MIDDLECLOSE)
	{
		msgLayer->closeBtn->setVisible(true);

		msgLayer->middleBtn->setVisible(true);
		msgLayer->okBtn->setVisible(false);
		msgLayer->cancelBtn->setVisible(false);

		msgLayer->middleContentLabel->setVisible(true);
		msgLayer->okContentLabel->setVisible(false);
		msgLayer->cancelContentLabel->setVisible(false);
	}

	selfInstance->middleContentLabel->setString(selfInstance->middleBtnContent.c_str());
	selfInstance->okContentLabel->setString(selfInstance->okBtnContent.c_str());
	selfInstance->cancelContentLabel->setString(selfInstance->cancelBtnContent.c_str());

	sUseHightPriotiry = false;
}

void TXGUI::MessageBox::ShowTextExt(string contentExt,ccColor3B labelColor,int fontSize)
{
	selfInstance->messageBodyLabelExt->setString(contentExt.c_str());
	selfInstance->messageBodyLabelExt->setColor(labelColor);
	if(selfInstance->messageBodyLabelExt->getLabelTTF() != NULL && selfInstance->messageBodyLabelExt->getLabelTTF()->GetTTF() != NULL)
	{
		selfInstance->messageBodyLabelExt->getLabelTTF()->GetTTF()->setFontSize(fontSize);
	}
}

void TXGUI::MessageBox::clearMessageBox()
{
	if(selfInstance != NULL)
	{
		selfInstance->Destroy();
	}
}

void TXGUI::MessageBox::AdjustStyle(std::string okBtnContent,std::string cancelBtnContent)
{
	if (selfInstance)
	{
		if (selfInstance->okContentLabel)
		{
			selfInstance->okContentLabel->setString(okBtnContent.c_str());
		}
		if (selfInstance->cancelContentLabel)
		{
			selfInstance->cancelContentLabel->setString(cancelBtnContent.c_str());
		}		
	}
}

void TXGUI::MessageBox::AdjustMiddleStyle(std::string middleBtnContent)
{
	if (selfInstance)
	{
		if (selfInstance->middleContentLabel)
		{
			selfInstance->middleContentLabel->setString(middleBtnContent.c_str());
		}
	}
}

void TXGUI::MessageBox::AdjustZOrder(unsigned int zOrder)
{
	if (selfInstance)
	{
		selfInstance->setZOrder(zOrder);
	}
}

void TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(bool bDefault)
{
	if (selfInstance)
	{
		selfInstance->isOkBtnDefaultBehavior = bDefault;
	}
}

void TXGUI::MessageBox::AdjustPriority(bool bUseHightPriority)
{
	sUseHightPriotiry = bUseHightPriority;
}

void TXGUI::MessageBox::okBtnClickEvent(CCObject* sender)
{
	if (isOkBtnDefaultBehavior)
	{
        CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        exit(0);
#endif
	}
	else
	{
		if (this->clickEventHander != 0 && pfnOkClicked != 0)
		{
			(this->clickEventHander->*pfnOkClicked)(sender);
		}
		Destroy();
	}
}

void TXGUI::MessageBox::middleBtnClickEvent(CCObject* sender)
{
	if (isOkBtnDefaultBehavior)
	{
		CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		exit(0);
#endif
	}
	else
	{
		if (this->clickEventHander != 0 && pfnMiddleClicked != 0)
		{
			(this->clickEventHander->*pfnMiddleClicked)(sender);
		}
		Destroy();
	}
}

void TXGUI::MessageBox::cancelBtnClickEvent(CCObject* sender)
{
	if (this->clickEventHander != 0 && pfnCancelClicked != 0)
	{
		(this->clickEventHander->*pfnCancelClicked)(sender);
	}
	Destroy();
}

void TXGUI::MessageBox::closeBtnClickEvent(CCObject* sender)
{
	Destroy();
}

void TXGUI::MessageBox::showCloseBtn()
{
    //if(selfInstance->closeBtn)
      //  selfInstance->closeBtn->setVisible(true);
}

