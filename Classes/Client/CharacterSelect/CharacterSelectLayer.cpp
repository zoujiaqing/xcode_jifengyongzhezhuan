#include "AppDelegate.h"
#include "Localization.h"
#include "XLogicManager.h"
#include "CharacterSelectLayer.h"
#include "TXGUIHeader.h"
#include "SpriteSeer.h"
#include "SkeletonAnimRcsManager.h"
#include "GameConfigFromLuaManager.h"
#include "AspriteManager.h"
#include "HelloWorldScene.h"

#include "HeroRoleManager.h"
#include "UserData.h"

#include <Time.h>
#include "LuaTinkerManager.h"
#include "ItemManager.h"
#include "GameResourceManager.h"
#include "NickNameManager.h"

#define CHARACTER_MOVE_POS 0.06f

CharacterSelectLayer::CharacterSelectLayer()
{
	m_currentCharacterIndex = 0;
	m_currentButton = NULL;
	m_layout = NULL;
	m_characterButtonPosX = 0;
	m_currentModle = NULL;
	m_characterPic = NULL;
	m_picPoint = CCPointZero;
	m_lastRandomName = "";
}

CharacterSelectLayer::~CharacterSelectLayer()
{
	UIManager::sharedManager()->RemoveUILayout("CharacterCreate");
	NickNameManager::Get()->unRegisterNickNameCallback();
}

bool CharacterSelectLayer::init()
{
	bool bRet = false;
	do {
		srand((int)time(0));

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		const char* rcsPath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("game.png");
		CCSprite* pSprite = CCSprite::create(rcsPath);
		CCAssert(pSprite != NULL,"error picture");
		if(pSprite != NULL)
		{
			this->addChild(pSprite, 0);
		}
		

		float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
		float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
		float scale = (scaleX > scaleY) ? scaleX : scaleY;
		pSprite->setScaleX(scale);
		pSprite->setScaleY(scale);
		pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

		UIManager *manager = UIManager::sharedManager();

		manager->CreateUILayoutFromFile("UIplist/CharacterCreate.plist", this, "CharacterCreate",true);
		m_layout = manager->getUILayout("CharacterCreate");

		manager->RegisterMenuHandler("CharacterCreate","CharacterSelectLayer::onItemClicked",
			menu_selector(CharacterSelectLayer::onItemClicked), this);

		manager->RegisterMenuHandler("CharacterCreate","CharacterSelectLayer::menuCreateRoleCallback",
			menu_selector(CharacterSelectLayer::menuCreateRoleCallback), this);

		manager->RegisterMenuHandler("CharacterCreate","CharacterSelectLayer::setRandomName",
			menu_selector(CharacterSelectLayer::setRandomName), this);

		mTextField= m_layout->FindChildObjectByName<UIEditBox>("NameInput");
		//mTextField = (TextInputField*)uiField->getCurrentNode();
		m_container = m_layout->FindChildObjectByName<UIContainer>("characterInforContainer");

		m_occupationLabel = m_layout->FindChildObjectByName<UILabel>("characterOccupationLable");
		m_occupationDes = m_layout->FindChildObjectByName<UILabel>("characterIntroductionLable");
		UIPicture* uipic = m_layout->FindChildObjectByName<UIPicture>("CharacterPic");
		if(uipic)
		{
			m_picPoint = uipic->getSpritePos();
		}
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(1,false);
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(2,false);
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(3,false);
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(4,false);
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(5,false);
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(6,false);
		initCharacterButton();

		NickNameManager::Get()->setNickNameCallback(this,callfuncND_selector(CharacterSelectLayer::onReceiveNickName));
		bRet = true;
	}while (0);
	return bRet;
}

bool CharacterSelectLayer::checkNullString(const char* str)
{
	unsigned int length = strlen(str);
	if(length == 0)
	{
		return true;
	}
	for(unsigned int i=0;i<length;i++)
	{
		if(str[i] != ' ' 
			&& str[i] != '\0' 
			&& str[i] != '\n'
			&& str[i] != '\r')
		{
			return false;
		}
	}
	return true;
}

void CharacterSelectLayer::menuCreateRoleCallback(CCObject* pSender)
{
 	if(mTextField != NULL)
	{
		if(!checkNullString(mTextField->getString()))
		{
			XLogicManager::sharedManager()->createRole(mTextField->getString(),m_currentCharacterIndex);
		}
        else
        {
			const char* msg = Localizatioin::getLocalization("M_INSERTNAME");
            showMessage(msg);
        }

	}

}

void CharacterSelectLayer::onItemClicked(CCObject* pSender)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(pSender);
	if(item)
	{
		CCNode* node = item->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			UIControlBase* controller = menu->getControlNode();
			UIButton* button = dynamic_cast<UIButton*>(controller);
			if(button)
			{
				if(button != m_currentButton)
				{
					if(m_currentButton != NULL)
					{
						CCSprite* sprite = m_currentButton->getIconSprite();
						if(sprite)
						{
							sprite->setColor(ccc3(110,110,110));
						}
						if(m_currentButton->getAnimationNode())
						{
							CCPoint pt = m_currentButton->getPosition();
							pt.x = m_characterButtonPosX;
							CCAction* actionMove = CCMoveTo::create(0.2f,pt);
							m_currentButton->getAnimationNode()->runAction(actionMove);
						}
					}
					if(m_currentModle)
					{
						m_currentModle->removeFromParentAndCleanup(true);
						m_currentModle = NULL;
					}

					m_currentButton = button;
					CCSprite* sprite = m_currentButton->getIconSprite();
					if(sprite)
					{
						sprite->setColor(ccWHITE);
						if(m_currentButton->getAnimationNode())
						{
							CCPoint pt = m_currentButton->getPosition();
							pt.x = m_characterButtonPosX - m_currentButton->getButtonSize().width * CHARACTER_MOVE_POS;
							CCAction* actionMove = CCMoveTo::create(0.2f,pt);
							m_currentButton->getAnimationNode()->runAction(actionMove);
						}
					}
					unsigned int type = button->getButtonIndex();
					ShowCharacterInfor(type);
				}
			}
		}
	}
}

void CharacterSelectLayer::ShowCharacterInfor(unsigned int index)
{
	if(m_currentCharacterIndex == index)
	{
		return;
	}

	m_currentCharacterIndex = index;
	// to do 
	ShowCharacterModle(index);
	const char* name = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", index, "Name");
	setOccupationName(name);
	const char* des = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", index, "Description");
	setOccupationDes(des);
	ShowCharacterBigPicture(index);
	UIEditBox* uiField = m_layout->FindChildObjectByName<UIEditBox>("NameInput");
	if(uiField)
	{
		string currentName = uiField->getString();
		if(m_lastRandomName == currentName)
		{
			// 当前为随机名字则重拉随机名字，否则保留用户自己输入的名字
			setRandomName(NULL);
		}
	}
}

void CharacterSelectLayer::setOccupationName(const char* text)
{
	if(m_occupationLabel)
	{
		m_occupationLabel->setString(text);
	}
}
void CharacterSelectLayer::setOccupationDes(const char* text)
{
	if(m_occupationDes)
	{
		m_occupationDes->setString(text);
	}
}

void CharacterSelectLayer::ShowCharacterModle(unsigned int index)
{
	CCSprite* backPic = (CCSprite*)m_layout->FindChildObjectByName<UIPicture>("CharacterBgPic")->getCurrentNode();
	if(backPic)
	{
		m_currentModle = HeroRoleManager::Get()->createRole(UserData::getUserId(), "", index, true,false);

		if(m_currentModle)
		{
			m_currentModle->SetAnim(kTypeIdle_FaceDown_Stand_MAIN,0,true);
			unsigned int weaponId = ItemManager::Get()->getHeroDefautWeaponId(index);
			m_currentModle->SetEquipItemsData(weaponId);
			CCPoint pt = backPic->getPosition();
			float scale = UIManager::sharedManager()->getScaleFactor();
			m_currentModle->getRoot()->setScale(scale);
			CCPoint modePt = ccp(pt.x,pt.y - scale * 130);
			m_currentModle->getRoot()->setPosition(modePt);
			m_container->getCurrentNode()->addChild(m_currentModle,m_container->getCurrentNode()->getZOrder()+1);
		}
	}
}

void CharacterSelectLayer::ShowCharacterBigPicture(unsigned int index)
{
	if(m_characterPic)
	{
		m_characterPic->removeFromParentAndCleanup(true);
		m_characterPic = NULL;
	}
	const char* fileName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", index, "PictureFile");
	//const char* picName = GameConfigFromLuaManager::Get()->getCharacterPicById(index);
	//int iSpriteFrame = getResourceIDByName(picName);
	//ASprite *as = AspriteManager::getInstance()->getAsprite(fileName); 
	string file = std::string(fileName);
	if(file.compare("") != 0)
	{
		CCPoint pt;
		CCSprite* sprite = CCSprite::create(fileName);
		
		//CCSprite* sprite = as->getSpriteFromFrame_Middle(iSpriteFrame, 0, pt);
		if(sprite)
		{
            std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();
            
            float f = 1.0f;
            if (strcmp(clientVersion.c_str(), "hq") == 0)
            {
                f = 1.0f / HQ_CHARACTER_BODYICON;
            } else if (strcmp(clientVersion.c_str(), "nq") == 0)
            {
                f = 1.0f / NQ_CHARACTER_BODYICON;
            }

			sprite->setScale(UIManager::sharedManager()->getScaleFactor() * f);
			m_characterPic = sprite;
			this->addChild(m_characterPic,10);
			int width = m_characterPic->getContentSize().width;
			m_characterPic->setAnchorPoint(ccp(0,0));
			m_characterPic->setPosition(ccp(-width,0));
			CCAction* action = CCMoveTo::create(0.2f,ccp(0,0));
			m_characterPic->runAction(action);
		}
	}
}

void CharacterSelectLayer::setRandomName(CCObject* pSender)
{	
	/*const char* jobName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters", m_currentButton->getButtonIndex(), "Name");
	string file = std::string(jobName);
	int randomNum = rand() % 1000;
	char randName[20];
	sprintf(randName,"%s%d",jobName,randomNum);*/
	bool isMale = m_currentButton->getButtonIndex() % 2 == 1;
	const char* nick = NickNameManager::Get()->getNickName(isMale);
	if(nick != NULL)
	{
		//string name = nick;
		m_lastRandomName = nick;
		UIEditBox* uiField = m_layout->FindChildObjectByName<UIEditBox>("NameInput");
		if(uiField)
		{
			CCLOG("setNickName: %s",m_lastRandomName.c_str());
			uiField->setString(m_lastRandomName.c_str());
		}
	}
	
}

void CharacterSelectLayer::onReceiveNickName(CCNode* node,void* nick)
{
	const char* nickName = static_cast<const char*>(nick);
	if(nickName)
	{
		m_lastRandomName = nickName;
		UIEditBox* uiField = m_layout->FindChildObjectByName<UIEditBox>("NameInput");
		if(uiField)
		{
			CCLOG("setNickName: %s",m_lastRandomName.c_str());
			uiField->setString(m_lastRandomName.c_str());
		}
	}
}

void CharacterSelectLayer::showMessage(const char* msg)
{
	CCNode* parent = this->getParent();
	HelloWorld* helloworld = dynamic_cast<HelloWorld*>(parent);
	if(helloworld)
	{
		helloworld->showMessage(msg);
	}
}

void CharacterSelectLayer::initCharacterButton()
{
	if(m_layout)
	{
		UIScrollList* list = m_layout->FindChildObjectByName<UIScrollList>("characterScrollList");
		if(list)
		{
			//int randomNum = rand() % 6;
			// 选男刺客作默认角色
			int randomNum = 4;
			int i = 0;
			std::list<UIControlBase*> controls = list->getChildren();
			std::list<UIControlBase*>::iterator it = controls.begin();
			for(; it != controls.end(); it++)
			{
				UIButton* button = dynamic_cast<UIButton*>(*it);
				if(button)
				{
					if(m_characterButtonPosX == 0)
					{
						m_characterButtonPosX = button->getPosition().x;
					}
					CCSprite* sprite = button->getIconSprite();
					if(sprite)
					{
						sprite->setColor(ccc3(110,110,110));
					}

					if(randomNum == i++)
					{
						sprite->setColor(ccWHITE);
						m_currentButton = button;
						if(m_currentButton->getAnimationNode())
						{
							CCPoint pt = m_currentButton->getPosition();
							pt.x = m_characterButtonPosX - m_currentButton->getButtonSize().width * CHARACTER_MOVE_POS;
							m_currentButton->setPosition(pt);
						}
						ShowCharacterInfor(m_currentButton->getButtonIndex());
					}
					
				}
			}
		}

	}
}