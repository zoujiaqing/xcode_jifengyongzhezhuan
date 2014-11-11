// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not
// be copied or disclosed except in accordance with the terms of that
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies.
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#include "OnlineNetworkManager.h"
#include "PVPLayer.h"
#include <stdlib.h>
#include <stdio.h>

#include "XLogicManager.h"
#include "GameManager.h"
#include "UserData.h"
#include "LevelDefine.h"
#include "UIManager.h"
#include "TXGUIHeader.h"
#include "Define.h"
#include "GameFontManager.h"
#include "CrossUtil.h"

#include "MainMenuLayer.h"
#include "Localization.h"
#include "GameDataManager.h"

#include "ChatToolbar.h"
#include "GameConfigFromLuaManager.h"

//// game message system header files here!
#include "GMessage.h"
#include "PVPMessage.h"
#include "CCLuaEngine.h"
#include "GameResourceManager.h"

using namespace TXGUI;

//////////////////////////////////////////////////////////////////////////
///
///
///

PVPLayer::PVPLayer()
	: m_bCloseLayerCallBack(false)
{
    this->m_b1vs1 = false;
    this->m_b2vs2 = false;
    this->m_bprotect = false;
    this->m_bfree = false;
    
    this->m_sp = true;
    this->m_sf = false;
}

PVPLayer::~PVPLayer()
{
	///hide chat toolbar
	ShowChatBar(true);
	UIManager::sharedManager()->RemoveUILayout("pvp_1vs1");
	UIManager::sharedManager()->RemoveUILayout("pvp_2vs2");
    UIManager::sharedManager()->RemoveUILayout("pvp_s1");
    UIManager::sharedManager()->RemoveUILayout("pvpUI");
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void PVPLayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

bool PVPLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	const char *path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("game.png");
	CCSprite* pSprite = CCSprite::create(path);
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pSprite->setAnchorPoint(ccp(0.5,0.5));
    
	this->addChild(pSprite, 0);
    
	float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
	float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
	float scale = (scaleX > scaleY) ? scaleX : scaleY;
	pSprite->setScaleX(scale);
	pSprite->setScaleY(scale);
    
	/// SHOW chat toolbar here
	GameConfigFromLuaManager::Get()->InitLayer((CCNode*)this,"InitPPVELayer");
    
	/// show ppve panel
	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/pvpUI.plist", this, "pvpUI", true);
    
	manager->RegisterMenuHandler("pvpUI","PVPLayer::OnClose",	menu_selector(PVPLayer::OnClose), this);
	manager->RegisterMenuHandler("pvpUI","PVPLayer::OnClickListItem",	menu_selector(PVPLayer::OnClickListItem), this);
    
	m_pList = m_pLayer->FindChildObjectByName<UIScrollList>("characterScrollList");
    
    TXGUI::UILayout *s1_layer = manager->CreateUILayoutFromFile("UIplist/pvp_s1.plist", this, "pvp_s1", true);
    manager->RegisterMenuHandler("pvp_s1","PVPLayer::onSel_1vs1",menu_selector(PVPLayer::onSel_1vs1), this);
    manager->RegisterMenuHandler("pvp_s1","PVPLayer::onSel_2vs2",menu_selector(PVPLayer::onSel_2vs2), this);
    manager->RegisterMenuHandler("pvp_s1","PVPLayer::onSel_protect",menu_selector(PVPLayer::onSel_protect), this);
    manager->RegisterMenuHandler("pvp_s1","PVPLayer::onSel_free",menu_selector(PVPLayer::onSel_free), this);
    s1_layer->setVisible(false);
    
    m_pList->setTouchEnabled(false);
    
    m_btn1vs1 = s1_layer->FindChildObjectByName<UIButtonToggle>("1vs1_btn");
    m_btn2vs2 = s1_layer->FindChildObjectByName<UIButtonToggle>("2vs2_btn");
    m_btnProtect = s1_layer->FindChildObjectByName<UIButtonToggle>("protect_btn");
    m_btnFree = s1_layer->FindChildObjectByName<UIButtonToggle>("free_btn");
    
    if(m_btn2vs2)
		m_btn2vs2->selected();
    
    if(m_btnFree)
		m_btnFree->selected();
    
    this->m_b1vs1 = false;
    this->m_b2vs2 = true;
    this->m_bprotect = false;
    this->m_bfree = true;
    
    UIPicture* pic = m_pLayer->FindChildObjectByName<UIPicture>("IconCharacter");
    UILabel* label = m_pLayer->FindChildObjectByName<UILabel>("PlayName");
    UILabel* label_level = m_pLayer->FindChildObjectByName<UILabel>("PlayLevel");

    if(label_level)
    {
        int level = UserData::GetUserLevel();
        char buf[10];
        sprintf(buf, "%d\n", level);
        string str = "lv";
        str += buf;
        label_level->setString(str.c_str());
    }
    
    if(label)
    {
        const char* name = UserData::getUserName();
        label->setString(name);
    }

    if(pic)
    {
       unsigned int type = UserData::GetUserType();
       
        const char* frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSMAN";
        if(type == 1)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSMAN";
        else if(type == 2)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSWOMAN";
        else if(type == 3)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDMAN";
        else if(type == 4)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDWOMAN";
        else if(type == 5)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINMAN";
        else if(type == 6)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINWOMAN";
        else if(type == 7)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERMAN";
        else if(type == 8)
            frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERWOMAN";
        
        CCSprite * headIcon = AspriteManager::getInstance()->getOneFrame("UI/ui_character_select.bin", frameRes);
        if(headIcon)
        {
            pic->setSprite(headIcon);
        }
    }
	return true;
}

void PVPLayer::OnClose(CCObject *sender)
{
	CCLOG("PVPLayer::OnClose");
    UIManager *manager = UIManager::sharedManager();
	manager->RemoveUILayout("pvpUI");
	NotificationCenter::defaultCenter()->broadcast(GM_CLOSE_PVPLAYER, this);
}

bool PVPLayer::AddNewRoom(const PVPRoomBase& room)
{
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_ppve.bin");
	pAsprite->setImageName(0, "UI/ui.png");
	pAsprite->setImageName(1, "UI/ui_ppve.png");
	CCPoint pt = CCPointZero;
	int iFrameId = TXGUI::getResourceIDByName("map_ui_ppve_FRAME_PVP_GRID_ROOM");
	CCSprite* normal = pAsprite->getSpriteFromFrame_Middle( iFrameId ,0, pt);
	iFrameId = TXGUI::getResourceIDByName("map_ui_ppve_FRAME_PVP_GRID_ROOM_CLICKED");
	CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle( iFrameId, 0, pt);
    
	std::string levelStr = Localizatioin::getLocalization("M_LEVEL");
    std::string strval = "";
    
    if(room.top_level() != 0)
    {
        int toplevel = room.top_level();
        int toplevel1 = toplevel/10*10;
        int toplevel2 = toplevel/10*10+9;
        
        char buf[10];
        sprintf(buf, "%d", toplevel1);
        strval = "[";
        strval += buf;
        sprintf(buf, "%d", toplevel2);
        strval += ",";
        strval += buf;
        strval += "]";
    }
    else
        strval = Localizatioin::getLocalization("M_NO_LEVEL_LIMIT");
    
    levelStr = strval;
    
	UIButton* pItem = m_pList->AddNewItem(normal,
                                          clicked,
                                          "PVPLayer::OnClickListItem",
                                          levelStr,
                                          ccGREEN,
                                          kCCScrollMenuPriority );
	if (pItem)
	{
		char szTemp[16] = "";
		sprintf(szTemp, "%d", room.room_id());
		UILabelTTF * index = UILabelTTF::create(szTemp, KJLinXin, 32);
		index->setPosition(-250, 0);
		pItem->getAnimationNode()->addChild(index);
        
		sprintf(szTemp, "%d/%d", room.current_online(), room.max_online());
		UILabelTTF * onlineMax = UILabelTTF::create(szTemp, KJLinXin, 32);
		onlineMax->setPosition(250, 0);
        
		pItem->getAnimationNode()->addChild(onlineMax);
        
		pItem->setTag(room.room_id());
		
		return true;
	}
	else
	{
		return false;
	}
}

void PVPLayer::ClearAllRooms()
{
	if (m_pList)
	{
		m_pList->clearChildrens();
	}
}

void   PVPLayer::OnClickListItem(CCObject* sender)
{
	int Tag = ((CCNode*)(sender))->getTag();
	CCLOG("OnClick Room:%d", Tag);
	PVPMessage::Get()->pvp_JoinRoom(Tag);
}

void   PVPLayer::onSel_1vs1(CCObject *sender)
{
    CCLOG("onSel_1vs1\n");
    if(m_b1vs1 == false)
    {
        CCLuaEngine::defaultEngine()->executeGlobalFunction("onBtn1vs1");
        m_btn1vs1->unselected();
        m_btn2vs2->selected();
    }
    else
    {
        m_b2vs2 = true;
        m_btn2vs2->unselected();
    }
}

void   PVPLayer::onSel_2vs2(CCObject *sender)
{
    if(m_b2vs2 == true)
    {
        CCLuaEngine::defaultEngine()->executeGlobalFunction("onBtn2vs2");
        m_btn2vs2->unselected();
        m_btn1vs1->selected();
    }
    else
    {
        m_b1vs1 = false;
        m_btn1vs1->unselected();
    }
}

void  PVPLayer::onSel_protect(CCObject* sender)
{
    CCLOG("onSel_protect\n");
    
    if(m_bprotect == false)
    {
        CCLuaEngine::defaultEngine()->executeGlobalFunction("onBtnProtect");
        m_btnProtect->unselected();
        m_btnFree->selected();
    }
    else
    {
        m_bfree = true;
        m_btnFree->unselected();
    }
    
    m_sp = true;
    m_sf = false;
}

void PVPLayer::onSel_free(CCObject* sender)
{
    CCLOG("onSel_free\n");
    
    if(m_bfree == true)
    {
        CCLuaEngine::defaultEngine()->executeGlobalFunction("onBtnFree");
        m_btnFree->unselected();
        m_btnProtect->selected();
    }
    else
    {
        m_bprotect = false;
        m_btnProtect->unselected();
    }
    
    m_sp = false;
    m_sf = true;
}

void PVPLayer::closeLayerCallBack( void )
{
	m_bCloseLayerCallBack = true;
	OnClose(NULL);
	m_bCloseLayerCallBack = false;
}
