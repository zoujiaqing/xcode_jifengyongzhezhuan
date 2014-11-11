#include "AppDelegate.h"
#include "InstanceDialogLayer.h"
#include "UIManager.h"
#include "Localization.h"
#include "GameConfigFromLuaManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "BattleUILayer.h"
#include "HSJoystick.h"
#include "StoryInstanceDirector.h"
#include "StoryDataCenter.h"
#include "GameResourceManager.h"

using namespace TXGUI;

InstanceDialogLayer::InstanceDialogLayer()
{
	m_pLayer = NULL;
	m_pContentLabel = NULL;
	m_pRoleNameLabel = NULL;
	m_leftHeroLogoPic = NULL;
	m_middleTipLogoPic = NULL;
	m_dialogBGPic = NULL;
	m_okBtn = NULL;
	m_okBtnLabel = NULL;
	m_areaPic = NULL;
	m_pNextAnimParentNode = NULL;
	m_pNextIcon = NULL;
	m_pNextIconPic = NULL;
}

InstanceDialogLayer::~InstanceDialogLayer()
{
	UIManager::sharedManager()->RemoveUILayout("InstanceDialogLayout");
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

bool InstanceDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{	
	bool bVisible = false;
	if (m_okBtn)
	{
		bVisible = m_okBtn->getCurrentNode()->isVisible();
	}
	if (false == bVisible)
	{
		StoryInstanceDirector::Get()->Resume();
	}	
	//return CCLayer::ccTouchBegan(pTouch,pEvent);
	return this->isVisible();
}

void InstanceDialogLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchMoved(pTouch, pEvent);
}

void InstanceDialogLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void InstanceDialogLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLayer::ccTouchEnded(pTouch, pEvent);
}

void InstanceDialogLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCMessageBoxPripority_2, true);
}

void InstanceDialogLayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

void InstanceDialogLayer::HideLayerAndClean()
{
	m_pContentLabel->setString("");
	m_pRoleNameLabel->setString("");
	if (m_leftHeroLogoPic)
	{
		m_leftHeroLogoPic->setSprite(NULL);
	}
	//CCNode* middleTipIcon = this->getChildByTag(MIDDLE_TIP_PIC_TAG);
	//if (middleTipIcon)
	//{
	//	middleTipIcon->removeFromParentAndCleanup(true);
	//}
	m_middleTipLogoPic->setSprite(NULL);

	mStoryDialogData.ResetValue();
	mDisplayPicName = "";

	this->setVisible(false);
}

bool InstanceDialogLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/InstanceDialogLayer.plist", this, "InstanceDialogLayout", true);
	
	manager->RegisterMenuHandler("InstanceDialogLayout","InstanceDialogLayer::okBtnClickEvent",
		menu_selector(InstanceDialogLayer::okBtnClickEvent), this);

	m_pContentLabel = m_pLayer->FindChildObjectByName<UILabel>("contentLable");
	m_pRoleNameLabel = m_pLayer->FindChildObjectByName<UILabel>("roleNameLable");
	m_leftHeroLogoPic = m_pLayer->FindChildObjectByName<UIPicture>("rolePic");
	m_middleTipLogoPic = m_pLayer->FindChildObjectByName<UIPicture>("midPic");
	m_dialogBGPic = m_pLayer->FindChildObjectByName<UIPicture>("backPic");
	m_okBtn = m_pLayer->FindChildObjectByName<UIButton>("okBtn");
	m_okBtnLabel = m_pLayer->FindChildObjectByName<UILabel>("okBtnLable");
	m_pNextIconPic = m_pLayer->FindChildObjectByName<UIPicture>("nextIconPic");

	m_areaPic = m_pLayer->FindChildObjectByName<UIPicture>("areaPic");
	m_okBtn->setMenuInitPriority(KCCMessageBoxContorlPripority_2);

	leftHeroLogoWidthOffset = 0.5f * m_leftHeroLogoPic->getSpriteSize().width;
	leftHeroLogoHeightOffset = 0.5f * m_leftHeroLogoPic->getSpriteSize().height;

	CreateNetIconAnimation();

	ShowDisplayPanelOrNot(false);
	ShowNextIconAnimation(false);

	this->setTouchEnabled(true);

	return true;
}

void InstanceDialogLayer::okBtnClickEvent(CCObject* pObj)
{
	StoryInstanceDirector::Get()->Resume();
	ShowDialogPanelOrNot(true);
	ShowDisplayPanelOrNot(false);
}

void InstanceDialogLayer::setStoryDialogData(StoryDialogData data)
{
	mStoryDialogData = data;
}

void InstanceDialogLayer::setStoryRoleBasicData(StoryRoleBasicData data)
{
	mStoryRoleBasicData = data;
}

void InstanceDialogLayer::SetStoryDisplayPicName(std::string picName)
{
	mDisplayPicName = picName;

}

void InstanceDialogLayer::ShowDialogPanel()
{
	std::string contentFlag = mStoryDialogData.getTextID();
	std::string content = Localizatioin::getLocalization(contentFlag.c_str());

	// Note: 显示对话内容
	if (m_pContentLabel)
	{
		m_pContentLabel->setString(content.c_str());
	}

	if (m_leftHeroLogoPic)
	{
		m_leftHeroLogoPic->setSprite(NULL);
	}

	m_areaPic->setVisible(true);
	// Note: 显示说话人的名字
	unsigned int roleId = mStoryRoleBasicData.getRoleId();
	ccColor3B colorName;
	if (0 == roleId)
	{
		colorName.r = 17;
		colorName.g = 179;
		colorName.b = 255;
	}else {
		colorName.r = 255;
		colorName.g = 195;
		colorName.b = 118;
	}
	m_pRoleNameLabel->setColor(colorName);
	if (m_pRoleNameLabel)
	{
		std::string roleName;
		StoryDataCenter::Get()->GetRoleNameByRoleId(roleId,roleName);
		m_pRoleNameLabel->setString(roleName.c_str());
	}
	// Note: 显示左侧的图像
	std::string picName = "";
	if (StoryDataCenter::Get()->GetOneRoleLeftIcon(mStoryDialogData.getRoleIndex(),picName))
	{
		if (picName == "")
		{
			return ;
		}
		else if (std::string::npos == picName.find(".png"))
		{
			picName = std::string("UI/") + picName + std::string(".png");
		}		
		const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(picName.c_str());
		CCSprite* sprite = CCSprite::create(path);
        
		if (sprite == NULL)
		{
            char buffer[100];
            sprintf(buffer,"Story Left Picture Not Exist %s",picName.c_str());
			std::string title = "Story Notification";
			CCMessageBox(buffer, title.c_str());

			return ;
		}
		sprite->setAnchorPoint(ccp(0,0));
        std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();

        // client version
        if (strcmp(clientVersion.c_str(), "hq") == 0)
        {
            sprite->setScale(1.0f / HQ_NPC_UI);
        } else if (strcmp(clientVersion.c_str(), "nq") == 0)
        {
            sprite->setScale(1.0f / NQ_NPC_UI);
        }

        m_leftHeroLogoPic->setMySprite(sprite);
        CCPoint newHeroLogoPt = sprite->getPosition();
        newHeroLogoPt.x -= leftHeroLogoWidthOffset;
        newHeroLogoPt.y -= leftHeroLogoHeightOffset;
        sprite->setPosition(newHeroLogoPt);		
	}

	// Note: 显示Skip动画
	ShowNextIconAnimation(true);
}

void InstanceDialogLayer::ShowDisplayPicPanel()
{
	if (mDisplayPicName != "")
	{
		//CCNode* middlePic = this->getChildByTag(MIDDLE_TIP_PIC_TAG);
		//if (middlePic)
		//{
		//	middlePic->removeFromParentAndCleanup(true);
		//}

		// Note: 显示中间的图像
		if (std::string::npos == mDisplayPicName.find(".png"))
		{
			mDisplayPicName = std::string("UI/") + mDisplayPicName + std::string(".png");
		}
		const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(mDisplayPicName.c_str());
		CCSprite* sprite = CCSprite::create(path);
		//float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
		//sprite->setScale(scale);

		if (sprite == NULL)
		{
			char buffer[100];
			sprintf(buffer,"Story Middle Picture Not Exist %s",mDisplayPicName.c_str());
			std::string title = "Story Notification";
			CCMessageBox(buffer, title.c_str());
		}
		else
		{		
			//this->addChild(sprite,0,MIDDLE_TIP_PIC_TAG);
			//CCPoint midPicPos = m_middleTipLogoPic->getWorldPosition();
			//sprite->setPosition(midPicPos);
			m_middleTipLogoPic->setSprite(sprite);
		}
	}
}

void InstanceDialogLayer::ClearMiddlePanel()
{
	mDisplayPicName = "";
	//CCNode* middleTipIcon = this->getChildByTag(MIDDLE_TIP_PIC_TAG);
	//if (middleTipIcon)
	//{
	//	middleTipIcon->removeFromParentAndCleanup(true);
	//}
	m_middleTipLogoPic->setSprite(NULL);
	ShowDisplayPanelOrNot(false);
}

void InstanceDialogLayer::ClearDialogPanel()
{
	mStoryDialogData.ResetValue();

	// Note: 显示对话内容
	if (m_pContentLabel)
	{
		m_pContentLabel->setString("");
	}
	if (m_pRoleNameLabel)
	{
		m_pRoleNameLabel->setString("");
	}

	if (m_leftHeroLogoPic)
	{
		m_leftHeroLogoPic->setSprite(NULL);
	}

	m_areaPic->setVisible(false);
	ShowNextIconAnimation(false);
}

void InstanceDialogLayer::ShowDialogPanelOrNot(bool bShow)
{
	if (m_pContentLabel)
	{
		m_pContentLabel->setVisible(bShow);
	}
	if (m_pRoleNameLabel)
	{
		m_pRoleNameLabel->setVisible(bShow);
	}
	m_leftHeroLogoPic->setVisible(bShow);
	if (m_dialogBGPic)
	{
		m_dialogBGPic->setVisible(bShow);
	}
	if (m_areaPic)
	{
		m_areaPic->setVisible(bShow);
	}
}

void InstanceDialogLayer::ShowDisplayPanelOrNot(bool bShow)
{
	//CCNode* middleTipIcon = this->getChildByTag(MIDDLE_TIP_PIC_TAG);
	if (m_middleTipLogoPic)
	{
		//middleTipIcon->setVisible(bShow);
		m_middleTipLogoPic->setVisible(bShow);
	}
	if (m_okBtn)
	{
		m_okBtn->setVisible(bShow);
	}
	if (m_okBtnLabel)
	{
		m_okBtnLabel->setVisible(bShow);
	}
}

void InstanceDialogLayer::ShowNextIconAnimation(bool bShow)
{
	if (m_pNextAnimParentNode)
	{
		m_pNextAnimParentNode->setVisible(bShow);
	}
}

void InstanceDialogLayer::CreateNetIconAnimation()
{
	const char* iconName = "map_ui_system_icon_FRAME_ICON_NEXT";
	m_pNextIcon = AspriteManager::getInstance()->getOneFrame(KICON_BIN,iconName);
	if (m_pNextIcon)
	{
		if (NULL == m_pNextAnimParentNode)
		{
			m_pNextAnimParentNode = m_pNextIconPic->getCurrentNode();
		}
		if (m_pNextAnimParentNode)
		{
			float scale = 1.0;
			m_pNextIcon->setPosition(ccp(m_pNextIcon->getContentSize().width/2 * scale,m_pNextIcon->getContentSize().height/2 * scale));
			m_pNextAnimParentNode->addChild(m_pNextIcon,100);

			float deltaDst = 10;
			deltaDst = deltaDst * scale;
			m_pNextIcon->setScale(scale);

			m_pNextIcon->stopAllActions();
			{
				CCActionInterval* action = NULL;
				action = CCMoveBy::create(0.4,ccp(deltaDst,0));

				CCActionInterval* action_back = action->reverse();
				CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));

				CCRepeatForever* pAction = CCRepeatForever::create(seq);
				m_pNextIcon->runAction( pAction );
			}
		}
	}
}