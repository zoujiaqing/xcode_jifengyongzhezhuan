#include "DiamondWarningLayer.h"
#include "UIManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "SceneLayer.h"

static DiamondWarningLayer* selfInstance = NULL;

DiamondWarningLayer::DiamondWarningLayer()
{
	m_uiLayout = NULL;
	m_okBtn = NULL;
	m_cancelBtn = NULL;
	m_clickEventHander = NULL;
	m_pfnOkClicked = NULL;
	m_pfnCancelClicked = NULL;
	m_tipContentLabel = NULL;
	m_topTipContentLabel = NULL;
}

DiamondWarningLayer::~DiamondWarningLayer()
{
	UIManager::sharedManager()->RemoveUILayout("DiamondWarningLayout");
}

bool DiamondWarningLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	m_uiLayout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/diamondWarning.plist", this, "DiamondWarningLayout");

	UIManager::sharedManager()->RegisterMenuHandler("DiamondWarningLayout","DiamondWarningLayer::onConfirmBtClicked",
		menu_selector(DiamondWarningLayer::onConfirmBtClicked), this);

	UIManager::sharedManager()->RegisterMenuHandler("DiamondWarningLayout","DiamondWarningLayer::onCancelBtClicked",
		menu_selector(DiamondWarningLayer::onCancelBtClicked), this);

	m_diamondLabel = m_uiLayout->FindChildObjectByName<UILabel>("diamondLable");
	m_warninglabel = m_uiLayout->FindChildObjectByName<UILabel>("warningLable");
	m_tipContentLabel = m_uiLayout->FindChildObjectByName<UILabel>("tipContentLable");
	m_topTipContentLabel = m_uiLayout->FindChildObjectByName<UILabel>("topTipContentLable");

	m_okBtn = m_uiLayout->FindChildObjectByName<UIButton>("OkBtn");
	m_cancelBtn = m_uiLayout->FindChildObjectByName<UIButton>("CancelBtn");

	return true;
}

DiamondWarningLayer* DiamondWarningLayer::getInstance()
{
	selfInstance = NULL;

	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (layer != NULL)
		{
			int tag = layer->getLayerTagByName("DiamondWarningLayer");
			selfInstance = (DiamondWarningLayer*)layer->getChildByTag(tag);
			if (selfInstance == NULL)
			{
				selfInstance = DiamondWarningLayer::create();
				layer->addChild(selfInstance,Z_Order_DiamondWarningLayer,tag);
			}
		}
	}

	return selfInstance;
}

void DiamondWarningLayer::onConfirmBtClicked(CCObject* obj)
{
	if (m_clickEventHander != NULL && m_pfnOkClicked != NULL)
	{
		(m_clickEventHander->*m_pfnOkClicked)(obj);
	}
	this->removeFromParentAndCleanup(true);
	selfInstance = NULL;
}

void DiamondWarningLayer::onCancelBtClicked(CCObject* obj)
{
	if (m_clickEventHander != NULL && m_pfnCancelClicked != NULL)
	{
		(m_clickEventHander->*m_pfnCancelClicked)(obj);
	}
	this->removeFromParentAndCleanup(true);
	selfInstance = NULL;
}

void DiamondWarningLayer::Show(const char* message_body,unsigned int costDiamond, SEL_MenuHandler pfnOkClicked,
	SEL_MenuHandler pfnCancelClicked,CCObject * handler, ccColor3B labelColor,const char* tipContent,const char* topTipContent)
{
	DiamondWarningLayer* layer = getInstance();
	if(layer)
	{
		layer->setVisible(true);
		layer->m_clickEventHander = handler;
		layer->m_pfnOkClicked = pfnOkClicked;
		layer->m_pfnCancelClicked = pfnCancelClicked;
		layer->m_warninglabel->setStringWithColor(message_body,labelColor);
		char diamondText[10];
		sprintf(diamondText,"%d",costDiamond);
		layer->m_diamondLabel->setStringWithColor(diamondText,labelColor);
		layer->m_tipContentLabel->setStringWithColor(tipContent,ccWHITE);
		layer->m_topTipContentLabel->setStringWithColor(topTipContent,ccc3(255,144,0));
	}	
}

void DiamondWarningLayer::Show(const char* message_body,unsigned int costDiamond,int pfnOkHandler, 
	int pfnCancelClicked,ccColor3B labelColor ,const char* tipContent,const char* topTipContent)
{
	DiamondWarningLayer* layer = getInstance();
	if(layer)
	{
		layer->m_warninglabel->setStringWithColor(message_body,labelColor);
		char diamondText[10];
		sprintf(diamondText,"%d",costDiamond);
		layer->m_diamondLabel->setStringWithColor(diamondText,labelColor);
		layer->m_tipContentLabel->setStringWithColor(tipContent,ccWHITE);

		layer->m_okBtn->getMenuItemSprite()->registerScriptTapHandler(pfnOkHandler);
		layer->m_cancelBtn->getMenuItemSprite()->registerScriptTapHandler(pfnCancelClicked);
		layer->m_topTipContentLabel->setStringWithColor(topTipContent,ccc3(255,144,0));
	}
}

void DiamondWarningLayer::SetTipContentColor(const char* tipContent, ccColor3B tipColor)
{
	DiamondWarningLayer* layer = getInstance();
	if(layer)
	{
		layer->m_tipContentLabel->setStringWithColor(tipContent,tipColor);
	}
}