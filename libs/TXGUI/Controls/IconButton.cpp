#include "IconButton.h"
#include "UIDefine.h"
#include "UIMenu.h"
#include "ItemDragLayer.h"
#include "MainMenuLayer.h"
#include "DragReceiver.h"
const static float DELTA = 40.0f;
#define DELAY 1.0f

namespace TXGUI
{
	IconButton::IconButton(CCSprite* backgroundTex , CCSprite* normalTex, CCSprite* selectedTex, 
		cocos2d::CCObject * handler,SEL_MenuHandler pfnClicked,int priority)
	{
		m_backgroundTex = backgroundTex;
		m_normalTex = normalTex;
		m_selectedTex = selectedTex;
		m_pagePosition = -1;
		m_buttonOrder = 5;
		m_iconOrder = 10;
		m_priority = priority;
		m_pAnimTipParentNode = NULL;
		if (m_backgroundTex)
		{
			m_menuItem = CCMenuItemSprite::create(m_backgroundTex,
				m_selectedTex,
				handler,
				pfnClicked);
			//CCMenu* menus = CCMenu::create(m_menuItem, NULL);
			m_menu = UIMenu::create(m_menuItem, NULL);
			m_menu->SetNormalIcon(m_normalTex);
			//m_menu->SetDragMove(true);
			//menus->setNeedChangeTouchPt(true);
			m_menu->setInitPriority(m_priority);
			m_menu->setPosition(CCPointZero);
			this->addChild(m_menu,m_buttonOrder);
		}

		if (m_normalTex)
		{
			m_normalTex->setPosition(ccp(0,0));
			this->addChild(m_normalTex,m_iconOrder);
		}

		//if (m_selectedTex)
		//{
		//	m_selectedTex->setPosition(ccp(0,0));
		//	this->addChild(m_selectedTex);
		//}
		m_dragHandler = NULL;
		m_pButtonLabel = NULL;
		m_pFrameIcon = NULL;
	}

	IconButton::~IconButton()
	{
		if(m_dragHandler)
		{
			DragReceiverRegister::sharedManager()->unregisterDragReceiver(m_dragHandler);
			//GetItemDragLayer()->unregisterDragReceiver(m_dragHandler);
			delete m_dragHandler;
			m_dragHandler = NULL;
		}
	}

	IconButton * IconButton::create(CCSprite* backgroundTex , CCSprite* normalTex, CCSprite* selectedTex,
			cocos2d::CCObject * handler, SEL_MenuHandler pfnClicked,int priority)
	{
		IconButton * bpRet = new IconButton(backgroundTex,normalTex,selectedTex,handler,pfnClicked,priority);
		bpRet->autorelease();
		return bpRet;
	}

	IconButton * IconButton::create(CCSprite* backgroundTex , CCSprite* normalTex, CCSprite* selectedTex,
			int priority)
	{
		IconButton * bpRet = new IconButton(backgroundTex,normalTex,selectedTex,NULL,NULL,priority);
		bpRet->autorelease();
		return bpRet;
	}


	void IconButton::setDragMove(bool value)
	{
		m_menu->SetDragMove(value,m_dragHandler);
	}

	void IconButton::SetDragFlagTime(unsigned int ms)
	{
		if(m_menu)
		{
			m_menu->SetDragFlagTime(ms);
		}
	}

	void IconButton::SetNormalIcon(CCSprite* normalTex)
	{
		if (m_normalTex)
		{
			m_normalTex->removeFromParentAndCleanup(true);
		}

		m_normalTex = normalTex;
		if (m_normalTex)
		{
			m_normalTex->setPosition(ccp(0,0));
			this->addChild(m_normalTex,m_iconOrder);

			if(m_menu)
			{
				m_menu->SetNormalIcon(m_normalTex);
			}
		}

		if(m_dragHandler)
		{
			m_dragHandler->setDragItem(m_normalTex);
		}
	}

	void IconButton::SetSelectedIcon(CCSprite* selectedTex)
	{
		if (m_selectedTex)
		{
			m_selectedTex->removeFromParentAndCleanup(true);
		}

		m_selectedTex = selectedTex;
		if (m_selectedTex)
		{
			m_menuItem->setSelectedImage(m_selectedTex);
		}
	}

	void IconButton::AddExternalSprite(CCSprite* sprite, CCPoint pt)
	{
		if (sprite)
		{			
			sprite->setPosition(pt);
			this->addChild(sprite,m_iconOrder + 1);
		}
	}

	UILabelTTF *IconButton::getItemLabel()
	{
		return m_pButtonLabel;
	}

	void IconButton::setItemLabel(const char * text,float fontSize,const char* font)
	{
		if(m_pButtonLabel == NULL)
		{
			//CCSize nameLabelSize = CCSizeMake(150 / CC_CONTENT_SCALE_FACTOR(), fontSize * 2.0f);
			m_pButtonLabel = UILabelTTF::create(text, font, fontSize);
			m_pButtonLabel->setAnchorPoint(ccp(1,0));
			float x = getContentSize().width * 0.4f;
			float y = getContentSize().height * 0.4f;
			m_pButtonLabel->setPosition(ccp(x,-y));
			this->addChild(m_pButtonLabel, m_iconOrder + 2);
		}

		m_pButtonLabel->setString(text);
	}

	void IconButton::removeItemLabel()
	{
		if(m_pButtonLabel != NULL)
		{
			m_pButtonLabel->removeFromParentAndCleanup(true);
			m_pButtonLabel = NULL;
		}
	}

	// store user data at current node and it's children if there were
	void IconButton::SetUserData(int data)
	{
		this->setTag(data);
		CCArray* pChildren = this->getChildren();
		if (pChildren)
		{
			for(unsigned int i = 0; i < pChildren->count(); i++)
			{
				CCNode* pNode = (CCNode*) pChildren->objectAtIndex(i);
				if (pNode)
				{
					pNode->setTag(data);

					CCArray *menuChildren = pNode->getChildren();
					if (menuChildren)
					{
						for(unsigned int i = 0; i < menuChildren->count(); i++) 
						{
							CCNode* menuChildNode = (CCNode*) menuChildren->objectAtIndex(i);
							if (menuChildNode)
							{
								menuChildNode->setTag(data);
							}
						}
					}
				}
			}
		}
	}

	void IconButton::SetDoubleClickHandler(CCObject* listerner,SEL_MenuHandler handler)
	{
		m_menu->SetDoubleClickHandler(listerner,handler);
	}

	void IconButton::SetTappedHandler(CCObject* listerner,SEL_MenuHandler tapHandler,SEL_MenuHandler tapCancelHandler)
	{
		m_menu->SetTappedHandler(listerner,tapHandler,tapCancelHandler);
	}

	void IconButton::creatDragReceiver(DragHandlerType type)
	{
		if(m_dragHandler)
		{
			return;
		}
		
		m_dragHandler = new DragReceiver(kCCScrollMenuPriority,type,m_pagePosition);
		m_dragHandler->setCheckDragFunc(checkDrag_selector(IconButton::isReleaseInButton),this);
		DragReceiverRegister::sharedManager()->registerDragReceiver(m_dragHandler);
	}

	bool IconButton::isReleaseInButton(CCTouch* touch)
	{
		bool claim = m_menu->checkReleaseTouchInButton(touch);
		if(claim)
		{
			return true;
		}
		return false;
	}

	void IconButton::setFrameSprite(CCSprite* sprite,CCPoint pt)
	{
		if(m_pFrameIcon)
		{
			m_pFrameIcon->removeFromParentAndCleanup(true);
			m_pFrameIcon = NULL;
		}

		if(sprite)
		{
			m_pFrameIcon = sprite;
			addChild(m_pFrameIcon, m_iconOrder+1);
			m_pFrameIcon->setPosition(pt);
		}
	}

	void IconButton::setHandlerPriority(int newPriority)
	{
		if(m_menu)
		{
			m_priority = newPriority;
			m_menu->setHandlerPriority(newPriority);
		}
	}

	int IconButton::getHandlerPriority()
	{
		return m_priority;
	}

	void IconButton::showTipIcon(const CCPoint& anchorPos)
	{
		if (NULL != m_pAnimTipParentNode)
		{
			//m_pAnimTipParentNode->removeFromParentAndCleanup(true);
			m_pAnimTipParentNode->setVisible(true);
			return;
		}
		m_pAnimTipParentNode = CCNode::create();
		this->addChild(m_pAnimTipParentNode,10000);	

		CCSprite* iconSprite = AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY");
		m_pAnimTipParentNode->addChild(iconSprite,1);

		float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
		CCSize iconSize = iconSprite->getContentSize();		
		CCSize MenuSize = this->m_menuItem->getContentSize();
		CCPoint relativePt = ccp(anchorPos.x * (MenuSize.width/2 - iconSize.width/2),iconSize.height/2);
		m_pAnimTipParentNode->setPosition(relativePt);
		
		CCActionInterval* action = NULL;
		action = CCMoveBy::create(0.4,ccp(0,10));

		CCActionInterval* action_back = action->reverse();
		CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));

		CCRepeatForever* pAction = CCRepeatForever::create(seq);
		m_pAnimTipParentNode->runAction( pAction );
	}

	void IconButton::hideTipIcon()
	{
		if (NULL != m_pAnimTipParentNode)
		{
			m_pAnimTipParentNode->removeFromParentAndCleanup(true);
		}
		m_pAnimTipParentNode = NULL;
	}

	void IconButton::SetButtonName(string name)
	{
		m_name = name;
		if (m_menu)
		{
			m_menuItem->SetMenuName(name);
		}
	}


}