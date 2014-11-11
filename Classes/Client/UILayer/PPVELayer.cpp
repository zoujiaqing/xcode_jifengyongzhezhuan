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

#include <stdlib.h>
#include <stdio.h>
#include "OnlineNetworkManager.h"
#include "PPVELayer.h"
#include "tolua++.h"
#include "XLogicManager.h"
#include "GameManager.h"
#include "UserData.h"
#include "LevelDefine.h"
#include "UIManager.h"
#include "TXGUIHeader.h"
#include "Define.h"
#include "GameFontManager.h"
#include "CrossUtil.h"

#include "GameConfigFromLuaManager.h"

#include "MainMenuLayer.h"
#include "Localization.h"
#include "GameDataManager.h"

#include "ChatToolbar.h"

//// game message system header files here!
#include "GMessage.h"
#include "PPVEMessage.h"
#include "CCLuaEngine.h"

#include "GameResourceManager.h"
#include "LuaTinkerManager.h"
#include "ItemManager.h"
#include "MessageHandler.h"
using namespace TXGUI;

//////////////////////////////////////////////////////////////////////////
///  
///  
///

PPVELayer::PPVELayer()
	: m_bCloseLayerCallBack(false) 
{
    m_currentInstance = 0;
    m_FuBenLevel = 0;
}

PPVELayer::~PPVELayer()
{
	PPVEMessage::Get()->exitRoom();
	PPVEMessage::Get()->getRoomList();

	/// hide chat toolbar
	ShowChatBar(true);

	UIManager::sharedManager()->RemoveUILayout("ppveUI");
	UIManager::sharedManager()->RemoveUILayout("ppveRoom");

	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void PPVELayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

bool PPVELayer::init()
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
		
	GameConfigFromLuaManager::Get()->InitLayer((CCNode*)this,"InitPPVELayer");
	
	/// show ppve panel
	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/ppveUI.plist", this, "ppveUI", true);
	manager->RegisterMenuHandler("ppveUI","PPVELayer::OnClose",	menu_selector(PPVELayer::OnClose), this);
	manager->RegisterMenuHandler("ppveUI","PPVELayer::OnClickListItem",	menu_selector(PPVELayer::OnClickListItem), this);	

	m_pList = m_pLayer->FindChildObjectByName<UIScrollList>("characterScrollList");
	
	/// for test 
	InstanceManager::Get()->UpdateData();
	MapInfoVector& map = InstanceManager::Get()->getPPVEInstances();
	m_vecMaps.clear();
    
    std::vector<int> veclevels;
    for(size_t i=0; i<map.size(); i++)
    {
        veclevels.push_back(map[i].MinLevel);
    }
    std::sort(veclevels.begin(), veclevels.end());
    
    for(size_t i=0;i<veclevels.size();i++)
    {
        int level = veclevels[i];
        for(size_t j=0; j<map.size(); j++)
        {
            if(level == map[j].MinLevel)
            {
                m_vecMaps.push_back(map[j]);
                continue;
            }
        }
    }
    
	//m_vecMaps.insert(m_vecMaps.begin(), map.begin(), map.end());
	// TODO : PPVE中，后加入的玩家的副本ID？
	InstanceManager::Get()->setCurInstanceId(123456789);

    LuaTinkerManager::Get()->callLuaFunc<int>("Script/ppveUILayer.lua","addUIUpdateTimer");

	// check bag pack full
	ItemManager::Get()->checkBagEmptyItemNum();
	return true;
}


void PPVELayer::OnClose(CCObject *sender)
{
	CCLOG("PPVELayer::OnClose");
      //call lua to close the update function
   /* lua_State* L_state = CCLuaEngine::defaultEngine()->getLuaState();
    lua_getglobal(L_state, "onClose");
    tolua_pushnumber(L_state, 2);//2 is not used .just cause onclose need a parameter
    lua_pcall(L_state, 1, LUA_MULTRET, 0);
    */
    UIManager *manager = UIManager::sharedManager();
	manager->RemoveUILayout("ppveUI");

    /// send close PPVE message to global message system
	NotificationCenter::defaultCenter()->broadcast(GM_CLOSE_PPVELAYER, this);
}

bool PPVELayer::AddNewRoom(const PVERoomBase& room)
{
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_ppve.bin");
	pAsprite->setImageName(0, "UI/ui.png");
	pAsprite->setImageName(1, "UI/ui_ppve.png");
	CCPoint pt = CCPointZero;
	int iFrameId = TXGUI::getResourceIDByName("map_ui_ppve_FRAME_MS_GRID_ROOM");
	CCSprite* normal = pAsprite->getSpriteFromFrame_Middle( iFrameId ,0, pt);
	iFrameId = TXGUI::getResourceIDByName("map_ui_ppve_FRAME_MS_GRID_ROOM_CLICKED");
	CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle( iFrameId, 0, pt);
		
	std::string levelStr = Localizatioin::getLocalization("M_LEVEL");
	char szLevel[4] = "";
	sprintf(szLevel, "%d", room.top_level());
	levelStr = szLevel + levelStr;

	UIButton* pItem = m_pList->AddNewItem(normal, 
		clicked, 
		"PPVELayer::OnClickListItem", 
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

void PPVELayer::ClearAllRooms()
{
	if (m_pList)
	{
		m_pList->clearChildrens();
	}
}

void   PPVELayer::OnClickListItem(CCObject* sender)
{
	int Tag = ((CCNode*)(sender))->getTag();
	CCLOG("OnClick Room:%d", Tag);

	int stamina = UserData::GetUserInfo().m_stamina;
	if (stamina < 5)
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "onStaminaClicked");
		return;
	}
    
	PPVEMessage::Get()->JoinRoom(Tag);
}

void PPVELayer::InitMaps()
{
	int count = m_vecMaps.size();
	ScrollPagePosition position;
	for(int i=0;i < count;i++)
	{
		position = getScrollPagePos(i);
		addSingleInstance(m_vecMaps[i], position);
        
        CCLOG("vec maps level:%d\n", m_vecMaps[i].MinLevel);
	}

	UIManager *manager = UIManager::sharedManager();
	UILayout* instanceListLayout = manager->getUILayout("ppveMapListUI");
	UIScrollPage *page = instanceListLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
	page->moveToPage(0);
}

void PPVELayer::addSingleInstance(InstanceMapInfo info, ScrollPagePosition position)
{
	UIManager *manager = UIManager::sharedManager();
	UILayout* instanceListLayout = manager->getUILayout("ppveMapListUI");
	if (!instanceListLayout)	
		return;

	UIScrollPage *page = instanceListLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");

	page->setScale(manager->getScaleFactor());

	//创建足够的页数
	while(position.page >= page->getPageCount())
	{
		CCLayer* layer = CCLayer::create();
		layer->setAnchorPoint(ccp(0, 0));		
		page->addPage(layer);
	}

	CCLayer * layer = page->getPageLayer(position.page);
	CCNode * insNode = layer->getChildByTag(position.index);

	//读取资源
	ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);

	std::string instanceConfigName = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "Icon");
	std::transform(instanceConfigName.begin(), instanceConfigName.end(), instanceConfigName.begin(), towupper);
	instanceConfigName = std::string("map_ui_system_icon_FRAME_") + instanceConfigName;	

	int normalBtnId = getResourceIDByName("map_ui_FRAME_INSTANCE_GRID1");
	int selectBtnId = getResourceIDByName("map_ui_FRAME_INSTANCE_GRID1_CLICKED");
	int isStarId = getResourceIDByName("map_ui_FRAME_INSTANCE_STAR");
	int notStarId = getResourceIDByName("map_ui_FRAME_INSTANCE_STAR_BLANK");
	int instanceIconId = getResourceIDByName(instanceConfigName.c_str());

	CCPoint pt;
	CCSprite *pNormalBtn = as->getSpriteFromFrame_Middle(normalBtnId, 0, pt);
	CCSprite *pSelectBtn = as->getSpriteFromFrame_Middle(selectBtnId ,0,pt);

	//创建副本按钮
	IconButton * pBtn = new IconButton(pNormalBtn, NULL, pSelectBtn, this, 
		menu_selector(PPVELayer::OnPressInstanceBtn));					
	pBtn->autorelease();		
	pBtn->setContentSize(pNormalBtn->getContentSize());
	pBtn->setPosition(ccp(0,0));
	pBtn->SetButtonIndex(position.index);

	//创建副本名称标签
	float fontSize = GameFontManager::smallFontSize();
	CCSize nameLabelSize = CCSizeMake(150 / CC_CONTENT_SCALE_FACTOR(), fontSize * 1.5f);
	//std::string instanceName = GameConfigFromLuaManager::Get()->getInstanceName(info.id);
	const char * instanceText = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "RaidName");	
	string instanceName = Localizatioin::getLocalization(instanceText);

	CCLabelTTF * instanceLabel = CCLabelTTF::create(instanceName.c_str(), KJLinXin, fontSize,
		nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom);
	instanceLabel->setPosition(ccp(0, -70));

	//创建副本图标
	ASprite *as1 = AspriteManager::getInstance()->getAsprite(KICON_BIN);
	CCSprite *pIcon = as1->getSpriteFromFrame_Middle(instanceIconId, 0, pt);
	pIcon->setPosition(ccp(0, 0));

	//创建副本级别要求
	std::string levelStr = Localizatioin::getLocalization("M_FUBEN");
	char szLevel[4] = "";
	sprintf(szLevel, "%d", info.MinLevel);
	levelStr = szLevel + levelStr;
	CCLabelTTF * levelLabel = CCLabelTTF::create(levelStr.c_str(), KJLinXin, fontSize + 5,
		nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom);
	levelLabel->setPosition(ccp(0, -40));

	//创建副本结点
	CCNode * instanceNode = CCNode::create();

	instanceNode->setTag(position.index);	
	instanceNode->setPosition(scrollPosToPoint(position));

	instanceNode->addChild(pIcon);
	instanceNode->addChild(pBtn);	
	instanceNode->addChild(instanceLabel);	
	instanceNode->addChild(levelLabel);
    
	layer->addChild(instanceNode);	
}

#define MAX_ROW 2
#define MAX_COL 4

PPVELayer::ScrollPagePosition PPVELayer::getScrollPagePos(int index)
{
	ScrollPagePosition spPos;
	spPos.index = index;
	spPos.page = index / (MAX_ROW * MAX_COL);
	spPos.row  = index % (MAX_ROW * MAX_COL) / MAX_COL;
	spPos.col  = index % (MAX_ROW * MAX_COL) % MAX_COL;

	return spPos;
}

CCPoint PPVELayer::scrollPosToPoint(ScrollPagePosition position)
{	
	//横竖坐标间距的一半
	float halfH = 120.0f;
	float halfV = 100.0f;

	//横竖坐标偏移，左下角为原点
	float offsetH = 0.0f;
	float offsetV = 40.0f;
	float HDelta, VDelta;

	HDelta =   halfH * (2 * position.col + 1 - MAX_COL) + offsetH;
	VDelta = - halfV * (2 * position.row + 1 - MAX_ROW) + offsetV;

	return ccp(HDelta, VDelta);
}

void PPVELayer::OnPressInstanceBtn(CCObject *sender)
{	
	CCLOG("PPVELayer: onPressInstanceBtn");
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
	IconButton* button = dynamic_cast<IconButton*>(sender);
	UIMenu * menu = dynamic_cast<UIMenu*>(sender);
	if(menu)
	{
		CCNode* parent = menu->getParent();
		IconButton* button = dynamic_cast<IconButton*>(parent);
	}

	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		CCMenu* menu = dynamic_cast<CCMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
				int i = button->GetButtonIndex();
				if (i < (int)m_vecMaps.size())
				{
					m_currentInstance = m_vecMaps[i].id;
                    m_FuBenLevel = m_vecMaps[i].MinLevel;
                    int level = UserData::GetUserLevel();
                    if(level < m_FuBenLevel)
                    {
                        std::string str =  Localizatioin::getLocalization("M_LEVEL_NOT_ENOUGH");
                        ShowMessage(str.c_str());
                        return;
                    }
					else
					{
						InstanceManager::Get()->setCurInstanceId(m_currentInstance);

					}
				}
				else
				{
					m_currentInstance = 0;
					InstanceManager::Get()->setCurInstanceId(m_currentInstance);
				}

                int attributes[1];
                attributes[0] = m_currentInstance%10000 +  PB_ATTR_PPVE_TIMES_DAILY_BEGIN;
                CCLOG("map id key :%d\n", attributes[0]);
                OnlineNetworkManager::sShareInstance()->sendAttributesReq(attributes,1);
                
				/// call global script function
				//CCLuaEngine::defaultEngine()->executeGlobalFunction("gotoNewRoom");
			}
		}
	}
}

void PPVELayer::closeLayerCallBack( void )
{
	m_bCloseLayerCallBack = true;

	OnClose(NULL);

	m_bCloseLayerCallBack = false;
}
