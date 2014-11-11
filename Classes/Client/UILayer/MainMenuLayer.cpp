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

#include "PPVELayer.h"
#include "PVPLayer.h"
#include "XLogicManager.h"
#include "MainMenuLayer.h"
#include <stdlib.h>
#include <stdio.h>

#include "GameManager.h"
#include "ItemDragLayer.h"
#include "BattleUILayer.h"
#include "GameUILayer.h"
#include "BackPackLayer.h"
#include "InstanceListLayer.h"
#include "CityListLayer.h"
#include "EquipmentUpgradeUILayer.h"
#include "TXGUIHeader.h"
#include "LevelManager.h"

#include "WaitingLayer.h"
#include "WaitingLayerManager.h"
/// ppve 
#include "GMessage.h"
#include "ChatToolbar.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
#include "EquipMixLayer.h"
#include "AlchemyUILayer.h"
#include "MainLandManager.h"
#include "GameAudioManager.h"
#include "StoryInstanceDirector.h"
#include "BattleCoverLayer.h"
#include "TalentLayer.h"
#include "AutoOnHook.h"
#include "ShopUILayer.h"
#include "CardUILayer.h"
#include "MarketLayer.h"
#include "ChargeLayer.h"
#include "FirstChargeLayer.h"
#include "UCActivityLayer.h"
#include "GuildListLayer.h"
#include "GuildRedEnvolopeLayer.h"
#include "GuildInfoLayer.h"
#include "GuildShopLayer.h"
#include "GuildMemberLayer.h"
#include "GuildRecordLayer.h"
#include "BatchShoppingLayer.h"
#include "SpriteExtractResult.h"
#include "LuaTinkerManager.h"
#include "UIControlsApearControler.h"
#include "LuaEngine.h"
#include "HelloWorldScene.h"
#include "LoadingLayer.h"
#include "PVPMessage.h"
#include "MarqueeUILayer.h"
#include "CardAttributionLayer.h"
#include "MultiCardBuyLayer.h"
#include "UCActivityLayer.h"
#include "TipMgr.h"
#include "ForcePKAnimLayer.h"
#include "CallNativeFuncManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "TmLoginManager.h"
#endif

using namespace TXGUI;

//////////////////////////////////////////////////////////////////////////
///  管理所有的UI图层，这里控制游戏中的所有的UI层
///  如装备层，
/*     战斗界面图层，
       背包图层...
	   PPVE图层...


*/   
///  本显示层，在游戏过程中，是不会被释放掉的，相当于游戏界面的总的管理器
///

MainMenuLayer::MainMenuLayer():
	damageValueLable(NULL),
    m_marketLayer(NULL),
    m_spriteExtractResultLayer(NULL),
    m_chargeLayer(NULL),
    m_firstChargeLayer(NULL),
    m_batchShoppingLayer(NULL),
    m_ucActivityLayer(NULL),
    m_guildListLayer(NULL),
    m_guildInfoLayer(NULL),
    m_guildInfoEditLayer(NULL),
    m_guildCreateLayer(NULL),
    m_guildContributeLayer(NULL),
    m_guildRequestLayer(NULL),
    m_guildRequestSettingLayer(NULL),
    m_guildBlessLayer(NULL),
    m_guildMemberLayer(NULL),
    m_guildNoticeLayer(NULL),
    m_guildAppointLayer(NULL),
    m_guildShopLayer(NULL),
	m_guildRecordLayer(NULL),
	m_guildInstanceListLayer(NULL),
	m_guildInstanceHelpLayer(NULL),
	m_guildRedEnvolopeLayer(NULL),
	m_guildRedEnvolopeNoteLayer(NULL),
	m_guildSendRedEnvoLayer(NULL),
	m_guildGetRedEnvoLayer(NULL),
	m_guildRedEnvoRecordLayer(NULL)
{
	m_pCityListUI = NULL;
	m_pBattleCoverUI = NULL;
	m_pInstanceListUI = NULL;
	m_pBackPackUI = NULL;
	m_pTaskChatUI = NULL;
	m_pTaskListUI = NULL;
	m_pBattleUI = NULL;
	m_pGameUI = NULL;
	m_equipmentUpgradeLayer = NULL;
	m_pUIMessage = NULL;
	m_pUIWaiting = NULL;
    
	//// PPVE , but don't create the ppve layer when init MainMenuLayer!!!!!
	m_pPPVELayer = NULL;
    m_pPVPLayer = NULL;
    m_pTalentLayer = NULL;
    m_pAutoOnHook = NULL;

	m_pChatToolbar = NULL;
	m_equipMixLayer = NULL;
	m_pAlchemyUILayer = NULL;
	m_cardUILayer = NULL;
    
    m_pLoadingLayer = NULL;
}

MainMenuLayer::~MainMenuLayer()
{
	CCLOG("***************************MainMenuLayer::~MainMenuLayer()************************");
	HidePVPLayer(false);
	HidePPVELayer(false);
    HideTalentLayer(false);
	HideAutoOnHook(false);
	HideGuildInstanceLayer(false);
    
	UIManager::sharedManager()->setUIDefaultRoot(NULL);

	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
	MessageBox::clearMessageBox();
    WaitingLayerManager::Get()->clearMessages();

    if (sVecLayersInQuene)
        sVecLayersInQuene->clear();
	CC_SAFE_DELETE(sVecLayersInQuene);

	TipMgr::Get()->removeTipAndTouchLayer();
}

void MainMenuLayer::addChild(CCNode * child)
{
	this->addChild(child,child->getZOrder(),child->getTag());
}
void MainMenuLayer::addChild(CCNode * child, int zOrder)
{
	this->addChild(child,zOrder,child->getTag());
}

void MainMenuLayer::addChild(CCNode * child, int zOrder, int tag)
{
	CCAssert((tag > 10000) && (tag < 40000),"error tag");
	CCNode* node = this->getChildByTag(tag);
	// CCAssert(node == NULL,"same tag"); //恢复，用于检测重复创建
    if(node != NULL)//fix bug 91897
    {
        CCLOG("failed to add repeated child which tag is %d", tag);
        return;
    }
    CCLayer::addChild(child,zOrder,tag);
}

CCLayer* MainMenuLayer::findUILayerByTag(int tag)
{
	CCNode* node = this->getChildByTag(tag);
	if (dynamic_cast<CCLayer*>(node) != NULL)
	{
		return static_cast<CCLayer*>(node);
	}

	return NULL;
}

CCLayer* MainMenuLayer::showUILayerByTag(int tag,bool isAutoCreate/* = true*/)
{
	CCLayer* layer = getUILayerByTag(tag,isAutoCreate);
	if(layer)
	{
		layer->setVisible(true);
	}
	return layer;
}

CCLayer* MainMenuLayer::getUILayerByTag(int tag,bool isAutoCreate/* = true*/)
{
	// 当前layer为空NULL并且不需要创建
	if(!isAutoCreate && findUILayerByTag(tag) == NULL)
	{
		return NULL;
	}

	CCLayer* layer = findUILayerByTag(tag);

	if(tag == getLayerTagByName("GameUILayer") && NULL == layer)
	{
		if(!m_pGameUI)
		{
			m_pGameUI = GameUILayer::create();
			this->addChild(m_pGameUI, Z_Order_Normal,tag);

		}
		layer = m_pGameUI;
	}
	else if(tag == getLayerTagByName("ItemDragLayer") && NULL == layer)
	{
		if(!m_pDragLayer)
		{
			m_pDragLayer = ItemDragLayer::create();
			this->addChild(m_pDragLayer, Z_Order_DragLayer,tag);
		}
		layer = m_pDragLayer;
	}
	else if(tag == getLayerTagByName("BattleUILayer") && NULL == layer)
	{
		if(!m_pBattleUI)
		{
			m_pBattleUI = BattleUILayer::create();
			this->addChild(m_pBattleUI,1,tag);
		}
		layer = m_pBattleUI;
	}
	else if(tag == getLayerTagByName("CardUILayer") && NULL == layer)
	{
		if(!m_cardUILayer)
		{
			m_cardUILayer = CardUILayer::create();
			addChild(m_cardUILayer,9,tag);
			GameAudioManager::sharedManager()->playEffect(350001,false);
			PushOneLayer(m_cardUILayer);
		}
		//ShowChatBar(false);
		layer = m_cardUILayer;
	}
	else if (tag == getLayerTagByName("InstanceListLayer") && NULL == layer)
	{
		if (!m_pInstanceListUI)
		{
			m_pInstanceListUI = InstanceListLayer::create();
			this->addChild(m_pInstanceListUI,4,tag);
		}
		layer = m_pInstanceListUI;
	}
	else if (tag == getLayerTagByName("CityListLayer") && NULL == layer)
	{
		if (!m_pCityListUI)
		{
			m_pCityListUI = CityListLayer::create();
			this->addChild(m_pCityListUI,5,tag);
		}
		layer = m_pCityListUI;
	}
	else if (tag == getLayerTagByName("BattleCoverLayer") && NULL == layer)
	{
		if (!m_pBattleCoverUI)
		{
			m_pBattleCoverUI = BattleCoverLayer::create();
			this->addChild(m_pBattleCoverUI, 100,tag);
		}
		layer = m_pBattleCoverUI;
	}
	else if(tag == getLayerTagByName("BackPackLayer") && NULL == layer)
	{
		if(!m_pBackPackUI)
		{
			m_pBackPackUI = BackPackLayer::create();
			this->addChild(m_pBackPackUI,3,tag);
		}
		layer = m_pBackPackUI;
	}
	else if(tag == getLayerTagByName("EquipMixLayer") && NULL == layer)
	{
		if(!m_equipMixLayer)
		{
			m_equipMixLayer = EquipMixLayer::create();
			addChild(m_equipMixLayer,6,tag);
			GameAudioManager::sharedManager()->playEffect(350001,false);
			PushOneLayer(m_equipMixLayer);
		}
		layer = m_equipMixLayer;
	}
	else if(tag == getLayerTagByName("AlchemyUILayer") && NULL == layer)
	{
		if(!m_pAlchemyUILayer)
		{
			m_pAlchemyUILayer = AlchemyUILayer::create();
			addChild(m_pAlchemyUILayer,8,tag);
			GameAudioManager::sharedManager()->playEffect(350001,false);
			PushOneLayer(m_pAlchemyUILayer);
		}
		//ShowChatBar(false);
		layer = m_pAlchemyUILayer;
	}
	else if(tag == getLayerTagByName("EquipmentUpgradeUILayer") && NULL == layer)
	{
		//if(NULL == m_equipmentUpgradeLayer)
		{
			//m_equipmentUpgradeLayer = EquipmentUpgradeUILayer::create();
			//this->addChild(m_equipmentUpgradeLayer,5,tag);
			//GameAudioManager::sharedManager()->playEffect(350001,false);
			//MainMenuLayer::PushOneLayer(m_equipmentUpgradeLayer);

			if(!m_pBackPackUI)
			{
				m_pBackPackUI = BackPackLayer::create();
				this->addChild(m_pBackPackUI,3,tag);
			}
			layer = m_pBackPackUI;
		}
		//layer = m_equipmentUpgradeLayer;
	}
    else if(tag == getLayerTagByName("TalentLayer") && NULL == layer)
    {
        //if(NULL == m_pTalentLayer)
        //{
        //    GameAudioManager::sharedManager()->playEffect(350001,false);
        //    ShowTalentLayer();
        //}
    }
    
    else if(tag == getLayerTagByName("ManorUI") && NULL == layer)
    {
        //CCLOG("getLayerTagByName \n");
        //LuaTinkerManager::Get()->callLuaFunc<void>("Script/GameUILayer.lua", "onGardenButton");
    }
    
    else if(tag == getLayerTagByName("PPVELayer") && NULL == layer)
    {
        if(NULL == m_pPPVELayer)
        {
            GameAudioManager::sharedManager()->playEffect(350001,false);
            ShowPPVELayer();
        }
    }
	else if(tag == getLayerTagByName("MarqueeUILayer") && NULL == layer)
	{
		layer = MarqueeUILayer::create();
		this->addChild(layer,85,tag);
	}
	else if(tag == getLayerTagByName("CardAttributionLayer") && NULL == layer)
	{
		layer = CardAttributionLayer::create();
		this->addChild(layer,10,tag);
	}
	else if(tag == getLayerTagByName("MultiCardBuyLayer") && NULL == layer)
	{
		layer = MultiCardBuyLayer::create();
		this->addChild(layer,10,tag);
	}
	else if (tag == getLayerTagByName("ForcePKAnimLayer") && NULL == layer)
	{
		layer = ForcePKAnimLayer::create();
		this->addChild(layer,90,tag);
	}
	else if (tag == getLayerTagByName("MarketLayer") && NULL == layer)
	{
		ShowMarketLayer(true);
		layer = m_marketLayer;
	}
	else if (tag == getLayerTagByName("GuildShopLayer") && NULL == layer)
	{
		ShowGuildShopLayer(true);
		layer = m_guildShopLayer;
	}
	else if(tag == getLayerTagByName("ChargeLayer") && NULL == layer)
	{
		ShowChargeLayer(true);
		layer = m_chargeLayer;
	} 
	else if (tag == getLayerTagByName("FirstChargeLayer") && NULL == layer)
    {
        ShowFirstChargeLayer(true);
        layer = m_firstChargeLayer;
    }
    else if (tag == getLayerTagByName("TreasureExploreLayer") && NULL == layer)
    {
         CallNativeFuncManager::getInstance()->ShowTreasureExploreLayer(true);
         layer = dynamic_cast<CCLayer*>(getChildByTag(tag));
    }
	else if (tag == getLayerTagByName("GuildInstanceListLayer") && NULL == layer)
	{
		ShowGuildInstanceListLayer(true);
		layer = m_guildInstanceListLayer;
	}
	else
	{
		if(LuaTinkerManager::Get()->checkAnyLoadFile("Script/MainMenuUILayer.lua"))
		{
			lua_State* LuaState = CCLuaEngine::defaultEngine()->getLuaState();
			lua_getglobal(LuaState, "getLuaLayerByTag");
			lua_pushinteger(LuaState,tag);
			lua_pushboolean(LuaState,isAutoCreate);
			int ret = lua_pcall(LuaState, 2, 0, 0);
			if (ret != 0) {
				CCLOG("call getLuaLayerByTag error:%s", lua_tostring(LuaState, -1));
				lua_pop(LuaState, 1);
				return NULL;
			}

			layer = findUILayerByTag(tag);
			/*if (lua_isuserdata(LuaState, -1)) {
			layer = (CCLayer*)lua_touserdata(LuaState, -1);
			}
			lua_pop(LuaState, 1);*/
		}
	}

	return layer;
}

void MainMenuLayer::hideLayerByTag(int tag)
{
	if(tag == getLayerTagByName("CardUILayer"))
	{
		ShowChatBar(true);
		if(m_cardUILayer)
		{
			m_cardUILayer->removeFromParentAndCleanup(true);
			m_cardUILayer = NULL;
		}
	}
	else if(tag == getLayerTagByName("EquipMixLayer"))
	{
		ShowChatBar(true);
		if(m_equipMixLayer)
		{
			m_equipMixLayer->removeFromParentAndCleanup(true);
			m_equipMixLayer = NULL;
		}
	}
	else if(tag == getLayerTagByName("AlchemyUILayer"))
	{
		ShowChatBar(true);
		if(m_pAlchemyUILayer)
		{
			m_pAlchemyUILayer->removeFromParentAndCleanup(true);
			m_pAlchemyUILayer = NULL;
		}
	}
	else if(tag == getLayerTagByName("EquipmentUpgradeUILayer"))
	{
		if(m_equipmentUpgradeLayer)
		{
			m_equipmentUpgradeLayer->removeFromParentAndCleanup(true);
			m_equipmentUpgradeLayer = NULL;
		}
	}
	else if(tag == getLayerTagByName("ForcePKAnimLayer"))
	{
		CCLayer* pLayer = dynamic_cast<CCLayer*>(this->getChildByTag(tag));
		if(pLayer)
		{
			pLayer->removeFromParentAndCleanup(true);
		}
	}
	else
	{
		if(LuaTinkerManager::Get()->checkAnyLoadFile("Script/MainMenuUILayer.lua"))
		{
			lua_State* LuaState = CCLuaEngine::defaultEngine()->getLuaState();
			lua_getglobal(LuaState, "HideOneLayerByTag");
			lua_pushinteger(LuaState,tag);
			int ret = lua_pcall(LuaState, 1, 0, 0);
			if (ret != 0) {
				CCLOG("call showLuaLayerByTag error:%s", lua_tostring(LuaState, -1));
				lua_pop(LuaState, 1);
				return;
			}
		}
	}
}

void MainMenuLayer::CreateTaskListLayer()
{
	//if (m_pTaskListUI == 0)
	{
		int tag = getLayerTagByName("TaskListLayer");
		m_pTaskListUI = TaskListLayer::create();
		this->addChild(m_pTaskListUI, Z_Order_Task,tag);
		m_pTaskListUI->ShowTaskListLayer(true);
	}
}

void MainMenuLayer::DestroyTaskListLayer()
{
	m_pTaskListUI->removeFromParentAndCleanup(true);
	m_pTaskListUI = 0;
}

bool MainMenuLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }

	/// Init UI Manager Default Root Node
	UIManager::sharedManager()->setUIDefaultRoot(this);
    
	setTouchEnabled(true);
	/// only show Game UI
	m_pGameUI = GameUILayer::create();
	int tag = getLayerTagByName("GameUILayer");
	this->addChild(m_pGameUI, Z_Order_Normal,tag);

	tag = getLayerTagByName("ItemDragLayer");
	m_pDragLayer = ItemDragLayer::create();
	this->addChild(m_pDragLayer, Z_Order_DragLayer,tag);

	tag = getLayerTagByName("BackPackLayer");
	m_pBackPackUI = BackPackLayer::create();
	this->addChild(m_pBackPackUI,Z_Order_BackPackUI,tag);

	tag = getLayerTagByName("TaskChatLayer");
	m_pTaskChatUI = TaskChatLayer::create();
	this->addChild(m_pTaskChatUI,Z_Order_TaskChatLayer,tag);
    
    tag = getLayerTagByName("LoadingLayer");
	m_pLoadingLayer = LoadingLayer::create();
	this->addChild(m_pLoadingLayer,Z_Order_LoadingLayer,tag);

	//EquipmentUpgradeUILayer::setUpgradeColdTime(0);
    
	/// add more!
	NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

	this->setKeypadEnabled(true);

	setChatToolbar(true,true,true);

	//发送主城依赖地图的taskId请求
	MainLandManager::Get()->sendGetTaskStatus();
    return true;
}

void MainMenuLayer::showPreOpenLayer()
{
	const char* layerName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/DeathTip/preOpenLayer.lua","getPreOpenLayerName");
	if(layerName != NULL && layerName[0] != '\0')
	{
		int tag = getLayerTagByName(layerName);
        if(getLayerTagByName("MarketLayer") == tag)
        {
             ShowMarketLayer(true,0);
        }
        else
        {
            showUILayerByTag(tag,true);
        }		
	}
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","");
}

/// 设备不同的关卡上的UI应该如何配置
void MainMenuLayer::ChangeMapUI(int mapID)
{	
	/*if(m_pBackPackUI)
	{
		m_pBackPackUI->setVisible(false);
	}
	if(damageValueLable)
	{
		damageValueLable->clearAllLabel();
	}*/
	if (MainLandManager::Get()->isMainLand(mapID))
	{
		/// 是主城
		if (m_pBattleUI)
		{
			m_pBattleUI->removeFromParentAndCleanup(true);
			m_pBattleUI = NULL;
		}

		if (m_pGameUI)
			m_pGameUI->setVisible(true);

		if(m_pChatToolbar)
		{
			m_pChatToolbar->setVisible(true);
			setChatToolbar(true,true,true);
		}
	}
	else
	{
		if(!m_pBattleUI)
		{
			int tag = getLayerTagByName("BattleUILayer");
			m_pBattleUI = BattleUILayer::create();
			this->addChild(m_pBattleUI,1,tag);
		}

		if (m_pGameUI)
			m_pGameUI->setVisible(false);

		if(m_pChatToolbar)
		{
			m_pChatToolbar->setVisible(false);
		}

		int tag = getLayerTagByName("UIPopupRewardLayer");
		CCLayer* pPopupRewardLayer = (CCLayer*)this->getChildByTag(tag);
		if (pPopupRewardLayer)
		{
			pPopupRewardLayer->removeFromParentAndCleanup(true);
		}
	}
	if(m_pTaskChatUI)
	{
		m_pTaskChatUI->setVisible(false);
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///          API
MainMenuLayer*  GetMainMenuLayer()
{
    if (GameManager::Get()->GetSceneLayer() == NULL)
        return NULL;
    SceneLayer* pScene = dynamic_cast<SceneLayer*>(GameManager::Get()->GetSceneLayer());
	if(pScene)
	{
		return pScene->getMainMenuLayer();
	}
	
	return NULL;
}


//ItemDragLayer* GetItemDragLayer()
//{
//    if (GameManager::Get()->GetSceneLayer() == NULL)
//        return NULL;
//    
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		return mLayer->GetItemDragLayer();
//	}
//
//	return NULL;
//}

//ItemDragLayer* MainMenuLayer::GetItemDragLayer(){
//	return m_pDragLayer;
//}

/// get battle ui layer
//BattleUILayer* GetBattleUILayer()
//{
//    if (GameManager::Get()->GetSceneLayer() == NULL)
//        return NULL;
//    
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		return mLayer->GetBattleUILayer();
//	}
//
//	return NULL;
//}


//BattleUILayer* MainMenuLayer::GetBattleUILayer(){
//	return m_pBattleUI;
//}

void MainMenuLayer::HandleDamageNumberEffect(CCPoint parentPt,unsigned int damage,bool isCrit,bool isHarmful,bool isFairyAttack)
{
	if(!damageValueLable)
	{
		int tag = getLayerTagByName("DamageValueEffect");
		damageValueLable = DamageValueEffect::create();
		this->addChild(damageValueLable,0,tag);
	}
	damageValueLable->HandleDamageNumberEffect(parentPt,damage,isCrit,isHarmful,isFairyAttack);
}

void MainMenuLayer::HandleAddBloodNumberEffect(CCPoint parentPt,unsigned int blood)
{
	if(!damageValueLable)
	{
		int tag = getLayerTagByName("DamageValueEffect");
		damageValueLable = DamageValueEffect::create();
		this->addChild(damageValueLable,0,tag);
	}
	damageValueLable->CreateAddBloodLabel(blood,parentPt);
}

void MainMenuLayer::HandleCritBurst(CCPoint pt,unsigned int effectType)
{
	if(!damageValueLable)
	{
		int tag = getLayerTagByName("DamageValueEffect");
		damageValueLable = DamageValueEffect::create();
		this->addChild(damageValueLable,0,tag);
	}
	damageValueLable->HandleCritBurst(pt,effectType);
}

void MainMenuLayer::HandleAttackMissEffect(CCPoint parentPt)
{
	if(!damageValueLable)
	{
		int tag = getLayerTagByName("DamageValueEffect");
		damageValueLable = DamageValueEffect::create();
		this->addChild(damageValueLable,0,tag);
	}
	damageValueLable->CreateMissLabel(parentPt);
}

void MainMenuLayer::HandleAttackBlockEffect(CCPoint parentPt)
{
	if(!damageValueLable)
	{
		int tag = getLayerTagByName("DamageValueEffect");
		damageValueLable = DamageValueEffect::create();
		this->addChild(damageValueLable,0,tag);
	}
	damageValueLable->CreateBlockLabel(parentPt);
}

void MainMenuLayer::HandleComboIncrease(unsigned int incremental)
{
	if(m_pBattleUI)
	{
		m_pBattleUI->HandleComboIncrease(incremental);
	}
}

/// get game ui layer
GameUILayer* GetGameUILayer()
{
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			return mLayer->GetGameUILayer();
		}
	}

	return NULL;
}

GameUILayer* MainMenuLayer::GetGameUILayer()
{
	return m_pGameUI;
}

//BackPackLayer* MainMenuLayer::GetBackPackLayer()
//{
//	return m_pBackPackUI;
//}

TaskChatLayer* MainMenuLayer::GetTaskChatLayer()
{
	return m_pTaskChatUI;
}

TaskListLayer* MainMenuLayer::GetTaskListLayer()
{
	return m_pTaskListUI;
}

//BackPackLayer* GetBackPackLayer()
//{
//	if(GameManager::Get()->GetSceneLayer())
//	{
//		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//		if (mLayer)
//		{
//			return mLayer->GetBackPackLayer();
//		}
//	}
//	
//	return NULL;
//}

//InstanceListLayer* MainMenuLayer::GetInstanceListLayer()
//{
//	if (NULL == m_pInstanceListUI)
//	{
//		int tag = getLayerTagByName("InstanceListLayer");
//		m_pInstanceListUI = InstanceListLayer::create();
//		this->addChild(m_pInstanceListUI,4,tag);
//	}
//
//	return m_pInstanceListUI;
//}

//InstanceListLayer* GetInstanceListLayer()
//{
//	if(GameManager::Get()->GetSceneLayer())
//	{
//		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//		if (mLayer)
//		{
//			return mLayer->GetInstanceListLayer();
//		}
//	}
//	return NULL;
//}

//CityListLayer* MainMenuLayer::GetCityListLayer()
//{
//	if (NULL == m_pCityListUI)
//	{
//		int tag = getLayerTagByName("CityListLayer");
//		m_pCityListUI = CityListLayer::create();
//		this->addChild(m_pCityListUI,5,tag);
//	}
//
//	return m_pCityListUI;
//}
//
//CityListLayer* GetCityListLayer()
//{
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		return mLayer->GetCityListLayer();
//	}
//	return NULL;
//}

//BattleCoverLayer* MainMenuLayer::GetBattleCoverLayer()
//{
//	if (NULL == m_pBattleCoverUI)
//	{
//		int tag = getLayerTagByName("BattleCoverLayer");
//		m_pBattleCoverUI = BattleCoverLayer::create();
//		this->addChild(m_pBattleCoverUI, 1000,tag);
//	}
//
//	return m_pBattleCoverUI;
//}
//
//BattleCoverLayer* GetBattleCoverLayer()
//{
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		return mLayer->GetBattleCoverLayer();
//	}
//	return NULL;
//}

//void MainMenuLayer::ShowEquipmentUpgradeUI(unsigned int coldTime)
//{
//	if(NULL == m_equipmentUpgradeLayer)
//	{
//		int tag = getLayerTagByName("EquipmentUpgradeUILayer");
//		m_equipmentUpgradeLayer = EquipmentUpgradeUILayer::create();
//		this->addChild(m_equipmentUpgradeLayer,5,tag);
//	}
//	m_equipmentUpgradeLayer->setUpgradeColdTime(coldTime);
//	if(!m_equipmentUpgradeLayer->isVisible())
//	{
//		m_equipmentUpgradeLayer->setVisible(true);
//	}
//	GameAudioManager::sharedManager()->playEffect(350001,false);
//	MainMenuLayer::PushOneLayer(m_equipmentUpgradeLayer);
//}
//
//EquipmentUpgradeUILayer* MainMenuLayer::GetEquipmentUpgradeLayer()
//{
//	return m_equipmentUpgradeLayer;
//}
//
//EquipmentUpgradeUILayer* GetEquipmentUpgradeLayer()
//{
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		return mLayer->GetEquipmentUpgradeLayer();
//	}
//
//	return NULL;
//}
//
//void MainMenuLayer::HideEquipmentUpgradeUI()
//{
//	if(m_equipmentUpgradeLayer)
//	{
//		//m_equipmentUpgradeLayer->setVisible(false);
//		m_equipmentUpgradeLayer->removeFromParentAndCleanup(true);
//		m_equipmentUpgradeLayer = NULL;
//	}
//}

/////////////////////////////pvp///////////////////////////////////
PVPLayer* GetPVPLayer()
{
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			return mLayer->ShowPVPLayer();
		}
	}
	
    
	return NULL;
}

void  RemovePVPLayer(bool showGameUI)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->HidePVPLayer(showGameUI);
	}
}

PVPLayer* MainMenuLayer::ShowPVPLayer()
{
	if (NULL == m_pPVPLayer)
	{
		int tag = getLayerTagByName("PVPLayer");
        m_pPVPLayer = new PVPLayer();
        m_pPVPLayer->init();
        this->addChild(m_pPVPLayer, 5,tag);
		MainMenuLayer::PushOneLayer(m_pPVPLayer);
	}
    
	/// hide gaming ui
	if (m_pGameUI)
		m_pGameUI->setVisible(false);
    
	return m_pPVPLayer;
}

SpriteElfLayer* MainMenuLayer::ShowSpriteElfLayer()
{
	SpriteElfLayer* pLayer = SpriteElfLayer::create();
	if (NULL != pLayer)
	{
		int tag = getLayerTagByName("SpriteElfLayer");
		this->addChild(pLayer, 5,tag);
		return pLayer;
	}
	return pLayer;
}

void MainMenuLayer::HidePVPLayer(bool showGameUI)
{
	if(m_pPVPLayer)
	{
		if(!m_pPVPLayer->IsOnCloseLayerCallBack())
			MainMenuLayer::RemoveOneLayer(m_pPVPLayer);
		m_pPVPLayer->removeFromParentAndCleanup(true);
        
		delete m_pPVPLayer;
		m_pPVPLayer = NULL;
        
		//m_pPVPLayer->removeFromParentAndCleanup(true);
		//m_pPVPLayer = NULL;
	}
    
	/// show gaming ui
	if (m_pGameUI)
		m_pGameUI->setVisible(showGameUI);
}

bool MainMenuLayer::IsInPVPModule()
{
	if (m_pPVPLayer != NULL)
		return true;
	return false;
}

/////////////////////////////Talent Layer//////////////////////////
TalentLayer* GetTalentLayer()
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		return mLayer->ShowTalentLayer();
	}
    
	return NULL;
}

void  RemoveTalentLayer(bool showGameUI)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->HideTalentLayer(showGameUI);
	}
}

TalentLayer* MainMenuLayer::ShowTalentLayer()
{
	//if (NULL == m_pTalentLayer)
	//{
	//	int tag = getLayerTagByName("TalentLayer");
 //       m_pTalentLayer = TalentLayer::create();
 //       this->addChild(m_pTalentLayer, 5,tag);
	//	MainMenuLayer::PushOneLayer(m_pTalentLayer);
	//}
 //   
 //   ShowChatBar(true, -1);
 //   
	///// hide gaming ui
	////if (m_pGameUI)
	//	//m_pGameUI->setVisible(false);
 //   
	//return m_pTalentLayer;

	return NULL;
}

void MainMenuLayer::HideTalentLayer(bool showGameUI)
{
	if(m_pTalentLayer)
	{
		if(!m_pTalentLayer->IsOnCloseLayerCallBack())
			MainMenuLayer::RemoveOneLayer(m_pTalentLayer);
		m_pTalentLayer->removeFromParentAndCleanup(true);
        
		//delete m_pTalentLayer;
		m_pTalentLayer = NULL;
	}
    
    //ShowChatBar(true,  -1);
	/// show gaming ui
	//if (m_pGameUI)
		//m_pGameUI->setVisible(showGameUI);
}

bool MainMenuLayer::IsInTalentModule()
{
	if (m_pTalentLayer != NULL)
		return true;
	return false;
}


void MainMenuLayer::ShowMarketLayer(bool flag, int layerTag,bool popFlag)
{
    int tag = getLayerTagByName("MarketLayer");

    if (flag == true && m_marketLayer == NULL)
    {
        CCNodeLoader* loader = new MarketLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("market", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/market", this);
        setMarketLayer(dynamic_cast<CCLayer*>(node));
        dynamic_cast<MarketLayer*>(m_marketLayer)->InitOpenLayer(layerTag);
        this->addChild(m_marketLayer, Z_Order_MarketLayer, tag);
		PushOneLayer(m_marketLayer);
    } 
	else if (flag == false && m_marketLayer != NULL)
    {
		if(popFlag)
		{
			RemoveOneLayer(m_marketLayer);
		}
        this->removeChildByTag(tag);
        setMarketLayer(NULL);
		
    }
}

void MainMenuLayer::ShowFirstChargeLayer(bool flag)
{
    int tag = getLayerTagByName("FirstChargeLayer");

    if (flag == true && m_firstChargeLayer == NULL)
    {
        CCNodeLoader* loader = new FirstChargeLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("first_charge", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/first_charge", this);
        setFirstChargeLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_firstChargeLayer, Z_Order_FirstChargeLayer, tag);
    } 
	else if (flag == false && m_firstChargeLayer != NULL)
    {
        this->removeChildByTag(tag);
        setFirstChargeLayer(NULL);
    }
}

void MainMenuLayer::ShowBatchShoppingLayer(bool flag)
{
    int tag = getLayerTagByName("BatchShoppingLayer");

    if (flag == true && m_batchShoppingLayer == NULL)
    {
        CCNodeLoader* loader = new BatchShoppingLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("batch_shopping", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/batch_shopping", this);
        setBatchShoppingLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_batchShoppingLayer, Z_Order_BatchShoppingLayer, tag);
    }
	else if (flag == false && m_batchShoppingLayer != NULL)
    {
        this->removeChildByTag(tag);
        setBatchShoppingLayer(NULL);
    }
}

void MainMenuLayer::ShowPlatActivityLayer(bool flag)
{
    int tag = getLayerTagByName("PlatActivityLayer");

    if (flag == true && m_ucActivityLayer == NULL)
    {
        CCNodeLoader* loader = new UCActivityLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("uc_activity", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/uc_activity", this);
        setUCActivityLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_ucActivityLayer, Z_Order_UCActivity, tag);
    }
	else if (flag == false && m_ucActivityLayer != NULL)
    {
        this->removeChildByTag(tag);
        setUCActivityLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildListLayer(bool flag)
{
    int tag = getLayerTagByName("GuildListLayer");

    if (flag == true && m_guildListLayer == NULL)
    {
        CCNodeLoader* loader = new GuildListLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_list", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_list", this);
        setGuildListLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildListLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildListLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildListLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildInfoLayer(bool flag)
{
    int tag = getLayerTagByName("GuildInfoLayer");

    if (flag == true && m_guildInfoLayer == NULL)
    {
        CCNodeLoader* loader = new GuildInfoLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_info", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_info", this);
        setGuildInfoLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildInfoLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildInfoLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildInfoLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildInfoEditLayer(bool flag)
{
    int tag = getLayerTagByName("GuildInfoEditLayer");

    if (flag == true && m_guildInfoEditLayer == NULL)
    {
        CCNodeLoader* loader = new GuildInfoEditLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_edit", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_edit", this);
        setGuildInfoEditLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildInfoEditLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildInfoEditLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildInfoEditLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildCreateLayer(bool flag)
{
    int tag = getLayerTagByName("GuildCreateLayer");

    if (flag == true && m_guildCreateLayer == NULL)
    {
        CCNodeLoader* loader = new GuildCreateLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_create", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_create", this);
        setGuildCreateLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildCreateLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildCreateLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildCreateLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildContributeLayer(bool flag)
{
    int tag = getLayerTagByName("GuildContributeLayer");

    if (flag == true && m_guildContributeLayer == NULL)
    {
        CCNodeLoader* loader = new GuildContributeLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_contribute", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_contribute", this);
        setGuildContributeLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildContributeLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildContributeLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildContributeLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildRequestLayer(bool flag)
{
    int tag = getLayerTagByName("GuildRequestLayer");

    if (flag == true && m_guildRequestLayer == NULL)
    {
        CCNodeLoader* loader = new GuildRequestLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_request", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_request", this);
        setGuildRequestLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildRequestLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildRequestLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildRequestLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildRequestSettingLayer(bool flag)
{
    int tag = getLayerTagByName("GuildRequestSettingLayer");

    if (flag == true && m_guildRequestSettingLayer == NULL)
    {
        CCNodeLoader* loader = new GuildRequestSettingLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_request_setting", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_request_setting", this);
        setGuildRequestSettingLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildRequestSettingLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildRequestSettingLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildRequestSettingLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildBlessLayer(bool flag)
{
    int tag = getLayerTagByName("GuildBlessLayer");

    if (flag == true && m_guildBlessLayer == NULL)
    {
        CCNodeLoader* loader = new GuildBlessLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_bless", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_bless", this);
        setGuildBlessLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildBlessLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildBlessLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildBlessLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildMemberLayer(bool flag)
{
    int tag = getLayerTagByName("GuildMemberLayer");

    if (flag == true && m_guildMemberLayer == NULL)
    {
        CCNodeLoader* loader = new GuildMemberLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_member", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_member", this);
        setGuildMemberLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildMemberLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildMemberLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildMemberLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildNoticeLayer(bool flag)
{
    int tag = getLayerTagByName("GuildNoticeLayer");

    if (flag == true && m_guildNoticeLayer == NULL)
    {
        CCNodeLoader* loader = new GuildNoticeLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_notice", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_notice", this);
        setGuildNoticeLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildNoticeLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildNoticeLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildNoticeLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildAppointLayer(bool flag)
{
    int tag = getLayerTagByName("GuildAppointLayer");

    if (flag == true && m_guildAppointLayer == NULL)
    {
        CCNodeLoader* loader = new GuildAppointLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_appoint", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_appoint", this);
        setGuildAppointLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildAppointLayer, Z_Order_GuildInfoEditLayer, tag);
    } else if (flag == false && m_guildAppointLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildAppointLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildShopLayer(bool flag,bool popFlag)
{
    int tag = getLayerTagByName("GuildShopLayer");

    if (flag == true && m_guildShopLayer == NULL)
    {
        CCNodeLoader* loader = new GuildShopLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_shop", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_shop", this);
        setGuildShopLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildShopLayer, Z_Order_GuildListLayer, tag);
		PushOneLayer(m_marketLayer);
    } else if (flag == false && m_guildShopLayer != NULL)
    {
		if(popFlag)
		{
			RemoveOneLayer(m_marketLayer);
		}
        this->removeChildByTag(tag);
        setGuildShopLayer(NULL);
    }
}

void MainMenuLayer::HideFirstChargeEffect()
{
    GameUILayer* gLayer = GetGameUILayer();

    if (gLayer && gLayer->getFirstPlayEffect())
    {
        gLayer->getFirstPlayEffect()->setVisible(false);
    }
}

void MainMenuLayer::ShowGuildRedEnvolopeLayer(bool flag)
{
	int tag = getLayerTagByName("GuildRedEnvolopeLayer");

	if (flag == true && m_guildRedEnvolopeLayer == NULL)
	{
		CCNodeLoader* loader = new GuildRedEnvolopeLayerLoader();
		CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("guild_red_envolope", loader);
		
		CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
		ccbReader->autorelease();

		CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_red_envo", this);
	    setGuildRedEnvolopeLayer(dynamic_cast<GuildRedEnvolopeLayer*>(node));

		this->addChild(m_guildRedEnvolopeLayer, Z_Order_GuildListLayer, tag);
	}
	else if(flag == false && m_guildRedEnvolopeLayer != NULL)
	{
		this->removeChildByTag(tag);
		setGuildRedEnvolopeLayer(NULL);
	}
}

void MainMenuLayer::ShowGuildRedEnvolopeNoteLayer(bool flag)
{
    int tag = getLayerTagByName("GuildRedEnvolopeNoteLayer");

    if (flag == true && m_guildRedEnvolopeNoteLayer == NULL)
    {
        CCNodeLoader* loader = new GuildRedEnvolopeNoteLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_red_envo_note", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_red_envo_note", this);
        setGuildRedEnvolopeNoteLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildRedEnvolopeNoteLayer, Z_Order_GuildListLayer, tag);
    } 
	else if (flag == false && m_guildRedEnvolopeNoteLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildRedEnvolopeNoteLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildRecordLayer(bool flag)
{
    int tag = getLayerTagByName("GuildRecordLayer");

    if (flag == true && m_guildRecordLayer == NULL)
    {
        CCNodeLoader* loader = new GuildRecordLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_record", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_record", this);
        setGuildRecordLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_guildRecordLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildRecordLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildRecordLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildSendRedEnvoLayer(bool flag)
{
    int tag = getLayerTagByName("GuildSendRedEnvoLayer");

    if (flag == true && m_guildSendRedEnvoLayer == NULL)
    {
        CCNodeLoader* loader = new GuildRedEnvolopeSendLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("guild_send_red_layer", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_send_red_suc", this);
		setGuildRedEnvolopeSendLayer(dynamic_cast<CCLayer*>(node));

		this->addChild(m_guildSendRedEnvoLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildSendRedEnvoLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildRedEnvolopeSendLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildGetRedEnvoLayer(bool flag)
{
    int tag = getLayerTagByName("GuildGetRedEnvoLayer");

    if (flag == true && m_guildGetRedEnvoLayer == NULL)
    {
        CCNodeLoader* loader = new GuildGetRedEnvolopeLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("get_red_suc_layer", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_get_red_suc", this);
		setGuildGetRedEnvolopeLayer(dynamic_cast<CCLayer*>(node));

		this->addChild(m_guildGetRedEnvoLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildGetRedEnvoLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildGetRedEnvolopeLayer(NULL);
    }
}

void MainMenuLayer::ShowGuildRedEnvoRecordLayer(bool flag)
{
	int tag = getLayerTagByName("GuildRedEnvoRecordLayer");

    if (flag == true && m_guildRedEnvoRecordLayer == NULL)
    {
        CCNodeLoader* loader = new GuildRedEnvolopeRecordLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("red_envo_record", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/guild_red_envo_record", this);
		setGuildRedEnvolopeRecordLayer(dynamic_cast<CCLayer*>(node));

		this->addChild(m_guildRedEnvoRecordLayer, Z_Order_GuildListLayer, tag);
    } else if (flag == false && m_guildRedEnvoRecordLayer != NULL)
    {
        this->removeChildByTag(tag);
        setGuildRedEnvolopeRecordLayer(NULL);
    }
}

void MainMenuLayer::ShowChargeLayer(bool flag, int layerTag)
{
    int tag = getLayerTagByName("ChargeLayer");

    if (flag == true && m_chargeLayer == NULL)
    {
        CCNodeLoader* loader = new ChargeLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("charge", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/charge", this);
        setChargeLayer(dynamic_cast<CCLayer*>(node));
        dynamic_cast<ChargeLayer*>(m_chargeLayer)->initLayerOpen(layerTag);

        this->addChild(m_chargeLayer, Z_Order_ChargeLayer, tag);
    }
	else if (flag == false && m_chargeLayer != NULL)
    {
        this->removeChildByTag(tag);
        setChargeLayer(NULL);
    }
}


void MainMenuLayer::ShowSpriteExtractResultLayer(bool flag,bool popFlag)
{
    int tag = getLayerTagByName("SpriteExtractResultLayer");

    if (flag == true && m_spriteExtractResultLayer == NULL)
    {
        ShowMarketLayer(false);

        CCNodeLoader* loader = new SpriteExtractResultLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("sprite_extract_result", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/sprite_extract_result", this);
        setSpriteExtractResultLayer(dynamic_cast<CCLayer*>(node));

        this->addChild(m_spriteExtractResultLayer, Z_Order_SpriteExtractResultLayer, tag);
		PushOneLayer(m_marketLayer);
    } 
	else if (flag == false && m_spriteExtractResultLayer != NULL)
    {
		if(popFlag)
		{
			RemoveOneLayer(m_marketLayer);
			ShowMarketLayer(true);
		}
        this->removeChildByTag(tag);
        setSpriteExtractResultLayer(NULL);
    }
}

/////////////////////////////AutoOnHook////////////////////////////
AutoOnHook* GetAutoOnHook()
{
    if(GameManager::Get()->GetSceneLayer())
    {
        MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
        if (mLayer)
        {
            return mLayer->ShowAutoOnHook();
        }
    }
	return NULL;
}

void  RemoveAutoOnHook(bool showGameUI)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->HideAutoOnHook(showGameUI);
	}
}

AutoOnHook* MainMenuLayer::ShowAutoOnHook(bool fromInstancePanel)
{
	if (NULL == m_pAutoOnHook)
	{
		int tag = getLayerTagByName("AutoOnHook");
        m_pAutoOnHook = new AutoOnHook(fromInstancePanel);

        m_pAutoOnHook->init();
        this->addChild(m_pAutoOnHook, 5,tag);
		//if(!fromInstancePanel)
			MainMenuLayer::PushOneLayer(m_pAutoOnHook);
	}
    
	/// hide gaming ui
	if (m_pGameUI)
		m_pGameUI->setVisible(false);
    
	return m_pAutoOnHook;
}

void MainMenuLayer::HideAutoOnHook(bool showGameUI)
{
	if(m_pAutoOnHook)
	{
		if(!m_pAutoOnHook->IsOnCloseLayerCallBack())
			MainMenuLayer::RemoveOneLayer(m_pAutoOnHook);
		m_pAutoOnHook->removeFromParentAndCleanup(true);
        
		delete m_pAutoOnHook;
		m_pAutoOnHook = NULL;
	}
    
	/// show gaming ui
	if (m_pGameUI)
		m_pGameUI->setVisible(showGameUI);
}

bool MainMenuLayer::IsInAutoOnHookModule()
{
	if (m_pAutoOnHook != NULL)
		return true;
	return false;
}

   
/////////////////////////////ppve//////////////////////////////////
PPVELayer* GetPPVELayer()
{
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			return mLayer->ShowPPVELayer();
		}
	}
	
	return NULL;
}

void  RemovePPVELayer(bool showGameUI)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->HidePPVELayer(showGameUI);
	}
}

PPVELayer* MainMenuLayer::ShowPPVELayer()
{
	if (NULL == m_pPPVELayer)
	{
		int tag = getLayerTagByName("PPVELayer");
		//m_pPPVELayer = PPVELayer::create();
		m_pPPVELayer = new PPVELayer();
		m_pPPVELayer->init();
		this->addChild(m_pPPVELayer, 5,tag);
		MainMenuLayer::PushOneLayer(m_pPPVELayer);
	}

	/// hide gaming ui
	if (m_pGameUI)
		m_pGameUI->setVisible(false);
    
	return m_pPPVELayer;
}

void MainMenuLayer::resetMailUIMsg()
{
	//if(MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId()))
	{
		USERINFO mUserData = UserData::GetUserInfo();
		bool isMailVisible = (mUserData.m_newMailNum > 0) || (mUserData.m_attachMailNum > 0);
		UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
		if(layout)
		{
			UIButton* button = layout->FindChildObjectByName<UIButton>("mailBtn");
			if(button)
			{
				if(isMailVisible)
				{
					button->showTipIcon();
					button->setVisible(true);
				}
				else
				{
					button->setVisible(false);
				}
				
			}
		}
	}
}

void MainMenuLayer::showMagicShop()
{
	int tag = getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
	if(layer)
	{
        layer->OnNPCBagMagicShopClicked();
	}
}

void MainMenuLayer::HidePPVELayer(bool showGameUI)
{
	if(m_pPPVELayer)
	{
		if(!m_pPPVELayer->IsOnCloseLayerCallBack())
			MainMenuLayer::RemoveOneLayer(m_pPPVELayer);
		//m_pPPVELayer->setVisible(false);
		m_pPPVELayer->removeFromParentAndCleanup(true);
		delete m_pPPVELayer;
		m_pPPVELayer = NULL;
	}
	/// show gaming ui
	if (m_pGameUI)
		m_pGameUI->setVisible(showGameUI);
}

bool MainMenuLayer::IsInPPVEModule()
{
	if (m_pPPVELayer != NULL)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////    toolbar

ChatToolbar* GetChatToolbar()
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		return mLayer->GetChatToolbar();
	}

	return NULL;
}

//void SetChatToolLayer(bool isChatHide,bool isFunctionHide,bool isExtraHide,int order)
//{
//	if (NULL == GameManager::Get()->GetSceneLayer())
//		return;
//
//	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//	if (mLayer)
//	{
//		 mLayer->setChatToolbar(isChatHide,isFunctionHide,isExtraHide,order);
//	}
//}

void ShowChatBar(bool isExtrVisible,int order)
{
	if (NULL == GameManager::Get()->GetSceneLayer())
		return;

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->setChatToolbar(true,true,isExtrVisible,order);
	}
}

MainMenuLayer* MainMenuLayer::getMainMenuLayer()
{
	if (GameManager::Get()->GetSceneLayer() != NULL)
	{
		return GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	}
	return NULL;
}

ChatToolbar* MainMenuLayer::GetChatToolbar()
{
	if (NULL == m_pChatToolbar)
	{
		int tag = getLayerTagByName("ChatToolbar");
		m_pChatToolbar = ChatToolbar::create();		
		this->addChild(m_pChatToolbar,-1,tag);
	}

	return m_pChatToolbar;
}

void MainMenuLayer::setChatToolbar(bool isChatVisible,bool isFunctionVisible,bool isExtraVisible,int order)
{
	if(!m_pChatToolbar)
	{
		int tag = getLayerTagByName("ChatToolbar");
		m_pChatToolbar = ChatToolbar::create();
		this->addChild(m_pChatToolbar,order,tag);
	}
	if (m_pChatToolbar)
	{
		m_pChatToolbar->setChatBarVisible(isChatVisible,isFunctionVisible,isExtraVisible);
		if(m_pGameUI)
		{
			UIControlAppearControler::Get()->ShowAutoButtons(isExtraVisible);
		}
		this->reorderChild(m_pChatToolbar,order);
	}
}

void MainMenuLayer::HideGuildInstanceLayer(bool showGameUI)
{
	if(m_guildInstanceHelpLayer)
	{
		m_guildInstanceHelpLayer->removeFromParentAndCleanup(true);
		delete m_guildInstanceHelpLayer;
		m_guildInstanceHelpLayer = NULL;
	}

	if(m_guildInstanceListLayer)
	{
		m_guildInstanceListLayer->removeFromParentAndCleanup(true);
		delete m_guildInstanceListLayer;
		m_guildInstanceListLayer = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////
/////                消息响应函数, 这里可以处理一些系统消息
///////
void MainMenuLayer::onBroadcastMessage(BroadcastMessage* msg)
{
	switch(msg->message)
	{
	case GM_CLOSE_PPVELAYER:
		HidePPVELayer(true);
		break;
            
    case GM_CLOSE_PVPLAYER:
        HidePVPLayer(true);
        break;
            
    case GM_CLOSE_TALENTLAYER:
        HideTalentLayer(true);
        break;
            
    case GM_CLOSE_AUTOONHOOK:
        HideAutoOnHook(true);
        //GetInstanceListLayer()->openInstanceList();
        break;
            
	case GM_APPLICATION_ENTER_BACKGROUND:
		/// game enter background now!
		/// check whether player is in mainland or not
		break;

	case GM_E_ENTER_MAINLAND:
		{
			//bool flag = TalentLayer::m_TalentLevelupFlag;
			//UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
			//if(layout)
			//{
			//	UIButton* button = layout->FindChildObjectByName<UIButton>("button_talent");
			//	if(button)
			//	{
			//		if(flag)
			//			button->showTipIcon();
			//		else
			//			button->hideTipIcon();
			//	}               
			//}
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////

int MainMenuLayer::getLayerTagByName(const char* name)
{
	int tag = LuaTinkerManager::Get()->getLuaConfig<int>("uiTagDefine", "UITagTable", name, "tag");
	CCAssert(tag != 0,"get a zero tag");
	return tag;
}

UIMessageLayer* MainMenuLayer::getUIMessageLayer()
{
	if(NULL == m_pUIMessage)
	{
		int tag = getLayerTagByName("UIMessageLayer");
		m_pUIMessage = UIMessageLayer::create();
		addChild(m_pUIMessage,90,tag);
	}

	return m_pUIMessage;
}

void ShowMessage(const char* message,const ccColor3B& color ,unsigned int  type )
{
	if(GameManager::Get()->GetSceneLayer())
	{
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if (mLayer)
		{
			UIMessageLayer* uiMessageLayer = mLayer->getUIMessageLayer();
			if(uiMessageLayer)
			{
				uiMessageLayer->ShowMessage(message,color ,type);
			}
		}
	}
	else
	{
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCNode* node = scene->getChildByTag(XLogicManager::E_SELECT_ZONE);
		HelloWorld* layer = dynamic_cast<HelloWorld*>(node);
		if(layer)
		{
			layer->showMessage(message);
		}
	}
}

void MainMenuLayer::CloseAllInStackLayer()
{
	if (sVecLayersInQuene == NULL)
	{
		return ;
	}
	for (std::vector<CCLayer*>::iterator iter = sVecLayersInQuene->begin();
		iter != sVecLayersInQuene->end();iter++)
	{
		CCLayer* layer = (*iter);
		if (layer->getTag() != kCCNodeTagInvalid)
		{
			LuaTinkerManager::Get()->callLuaFunc<bool>("Script/MainMenuUILayer.lua", "HideOneLayerByTag", layer->getTag());
		}	
		layer->closeLayerCallBack();
	}

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	NotificationCenter::defaultCenter()->broadcast(GM_LUA_LAYER_CLOSE,mLayer,"","");
	GameConfigFromLuaManager::Get()->PushLayerInstanceToLua(NULL);
	sVecLayersInQuene->clear();
}

void MainMenuLayer::PushOneLayer(CCLayer *pLayer,std::string param_1 /* = "" */,std::string param_2 /* = "" */)
{
	 LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","closeChatPlayerOption");
	if (sVecLayersInQuene == 0)
	{
		sVecLayersInQuene = new std::vector<CCLayer*>();
	}
	for (std::vector<CCLayer*>::iterator iter = sVecLayersInQuene->begin();
		iter != sVecLayersInQuene->end();iter++)
	{
		CCLayer* layer = (*iter);
		if (layer == pLayer)
		{
			return ;
		}
	}
	if (param_1 != "remainExist")
	{
		for (std::vector<CCLayer*>::iterator iter = sVecLayersInQuene->begin();
		 iter != sVecLayersInQuene->end();iter++)
		{
			CCLayer* layer = (*iter);
			if (layer->getTag() != kCCNodeTagInvalid)
			{
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/MainMenuUILayer.lua", "HideOneLayerByTag", layer->getTag());
			}	
			layer->closeLayerCallBack();		
		}
	}


	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	NotificationCenter::defaultCenter()->broadcast(GM_LUA_LAYER_CLOSE,mLayer,param_1,param_2);

	GameConfigFromLuaManager::Get()->PushLayerInstanceToLua(pLayer);

	sVecLayersInQuene->clear();
	if (pLayer != NULL)
	{
		sVecLayersInQuene->push_back(pLayer);
	}	
}

void MainMenuLayer::CreateSliderCtrl(CCNode* layer, int tag, int width, int height)
{
    float scale = UIManager::sharedManager()->getScaleFactor();
    ASprite* pAsprite2 = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    pAsprite2->setImageName(0, "UI/ui.png");
    
    CCPoint relativePoint_sfx, relativePoint_music;
    CCPoint anchorPoint(0.5,0.5);
    
    std::string backGroundFrameName = "map_ui_FRAME_SETTING_BUTTONAREA_SFX"; //"map_ui_FRAME_SETTING_BACKGROUND";  //map_ui_FRAME_SETTING_BUTTONAREA_MUSIC
    const char* childFrameName = "map_ui_FRAME_SETTING_BARFRAME_SFX";
    AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint_sfx);
    
    backGroundFrameName = "map_ui_FRAME_SETTING_BUTTONAREA_MUSIC"; //"map_ui_FRAME_SETTING_BACKGROUND";  //map_ui_FRAME_SETTING_BUTTONAREA_MUSIC
    childFrameName = "map_ui_FRAME_SETTING_BAR_MUSIC";
    AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint_music);
    
	const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("load0001.png");
    CCSprite* bkground = CCSprite::create(path);
	path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("load0002.png");
    CCSprite* progress = CCSprite::create(path);
    
	path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("load0001.png");
    CCSprite* bkslider = CCSprite::create(path);
    CCSize bkslider_size = bkslider->getContentSize();
    
    CCPoint pt;
    int frame = TXGUI::getResourceIDByName("map_ui_FRAME_SETTING_BUTTON_SFX");
    CCSprite* sfx = pAsprite2->getSpriteFromFrame_Middle(frame ,0, pt);
    
    CCPoint anotherPt;
    CCSprite* anotherSp = pAsprite2->getSpriteFromFrame_Middle(TXGUI::getResourceIDByName("map_ui_FRAME_SETTING_FRAME_MUSIC") ,0, anotherPt);
    
    UILayout* player = UIManager::sharedManager()->getUILayout("GameSetting");
    UIPicture* area_music = player->FindChildObjectByName<UIPicture>("SETTING_BUTTONAREA_MUSIC");
    UIPicture* area_sfx = player->FindChildObjectByName<UIPicture>("SETTING_BUTTONAREA_SFX");
    
    
    //Add the slider
    CCControlSlider *slider = CCControlSlider::create(bkground, progress, sfx);
    slider->setTag(tag);
    slider->setAnchorPoint(ccp(0.f, 0.f));
    slider->setMinimumValue(0.0f); //Sets the min value of range
    slider->setMaximumValue(100.0f); //Sets the max value of range
    slider->setTouchPriority(kCCScrollMenuPriority);
    slider->addTargetWithActionForControlEvents(layer, cccontrol_selector(MainMenuLayer::valueChanged), CCControlEventValueChanged);
    //layer->addChild(slider);
    //slider->setScale(scale);
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCLOG("slider width:%f, height:%f\n", winsize.width, winsize.height);
    if(tag == 1)
    {
        //slider->setPosition(ccp(relativePoint_music.x, relativePoint_music.y));
        
        //if(winsize.width == 1794 && winsize.height == 1080)
       // {
         //   CCPoint pt = slider->getPosition();
           // slider->setPosition(ccp(pt.x + 30,  pt.y + anotherSp->getContentSize().height));
       // }
        
        area_music->getCurrentNode()->addChild(slider);
        bool isFirst = CCUserDefault::sharedUserDefault()->getBoolForKey("GameSettingSliderUpperKey_First");
        int val = CCUserDefault::sharedUserDefault()->getIntegerForKey("GameSettingSliderUpperKey");
        if(isFirst == false)
        {
            val = 100;
            CCUserDefault::sharedUserDefault()->setBoolForKey("GameSettingSliderUpperKey_First", true);
            CCUserDefault::sharedUserDefault()->flush();
        }
        slider->setValue(val);
    }
    else if(tag ==2)
    {
        //slider->setPosition(ccp(relativePoint_sfx.x, relativePoint_sfx.y));
        //if(winsize.width == 1794 && winsize.height == 1080)
       // {
         //   CCPoint pt = slider->getPosition();
           // slider->setPosition(ccp(pt.x + 30,  pt.y + anotherSp->getContentSize().height+30));
        //}

        area_sfx->getCurrentNode()->addChild(slider);
        bool isFirst = CCUserDefault::sharedUserDefault()->getBoolForKey("GameSettingSliderDownKey_First");
        int val = CCUserDefault::sharedUserDefault()->getIntegerForKey("GameSettingSliderDownKey");
        if(isFirst == false)
        {
            val = 100;
            CCUserDefault::sharedUserDefault()->setBoolForKey("GameSettingSliderDownKey_First", true);
            CCUserDefault::sharedUserDefault()->flush();
        }
        slider->setValue(val);
    }
}

void MainMenuLayer::valueChanged(CCObject *sender, CCControlEvent controlEvent)
{
    CCControlSlider* pSlider = (CCControlSlider*)sender;
    //Change value of label.
	if(pSlider->getTag() == 1)
    {
        CCLog(CCString::createWithFormat("Upper slider value = %.02f", pSlider->getValue())->getCString());
        CCUserDefault::sharedUserDefault()->setIntegerForKey("GameSettingSliderUpperKey", pSlider->getValue());
        CCUserDefault::sharedUserDefault()->flush();
        GameAudioManager::sharedManager()->setBackgroundMusicVolume(pSlider->getValue());
        //pSlider->setValue(float val);
    }
    
    else if(pSlider->getTag() == 2)
    {
        CCLog(CCString::createWithFormat("Down slider value = %.02f", pSlider->getValue())->getCString());
        CCUserDefault::sharedUserDefault()->setIntegerForKey("GameSettingSliderDownKey", pSlider->getValue());
        CCUserDefault::sharedUserDefault()->flush();
         GameAudioManager::sharedManager()->setEffectsVolume(pSlider->getValue());
    }
}

void MainMenuLayer::ClearLayersInQueneData()
{
	if (sVecLayersInQuene == 0 || sVecLayersInQuene->size() == 0)
	{
		return ;
	}	
	sVecLayersInQuene->clear();
}

void MainMenuLayer::RemoveOneLayer(CCLayer *pLayer)
{
	if (sVecLayersInQuene == 0 || sVecLayersInQuene->size() == 0)
	{
		return ;
	}	
	for (std::vector<CCLayer*>::iterator iter = sVecLayersInQuene->begin();
		iter != sVecLayersInQuene->end();iter++)
	{
		CCLayer* layer = (*iter);
		if (pLayer == layer)
		{
			sVecLayersInQuene->erase(iter);
			return ;
		}
	}
}

std::vector<CCLayer*>* MainMenuLayer::sVecLayersInQuene = 0;
WaitingLayer* MainMenuLayer::getWaitingLayer()
{
	if(NULL == m_pUIWaiting)
	{
		int tag = getLayerTagByName("WaitingLayer");
		m_pUIWaiting = WaitingLayer::create();
		addChild(m_pUIWaiting, Z_Order_WaitingLayer,tag);
	}

	return m_pUIWaiting;
}


void ShowWaitingLayer(bool bShowRightNow /* = true */,float sumTimer /* = 0.3 */)
{
	if (GetMainMenuLayer())
	{
		GetMainMenuLayer()->getWaitingLayer()->ShowWaiting(bShowRightNow,sumTimer);
	}
	else
	{
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCNode* node = scene->getChildByTag(XLogicManager::E_SELECT_ZONE);
		HelloWorld* helloLayer = dynamic_cast<HelloWorld*>(node);
		if(helloLayer != NULL)
		{
			int tag = MainMenuLayer::getLayerTagByName("WaitingLayer");
			CCNode* layer = helloLayer->getChildByTag(tag);
			WaitingLayer* waitingLayer = dynamic_cast<WaitingLayer*>(layer);
			if(waitingLayer == NULL)
			{
				waitingLayer = WaitingLayer::create();
				helloLayer->addChild(waitingLayer, Z_Order_WaitingLayer,tag);
			}
			waitingLayer->ShowWaiting(bShowRightNow,sumTimer);
		}
	}
}


void HideWaitingLayer()
{
	if (GetMainMenuLayer())
	{
		GetMainMenuLayer()->getWaitingLayer()->HideWaiting();
	}
	else
	{
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCNode* node = scene->getChildByTag(XLogicManager::E_SELECT_ZONE);
		HelloWorld* helloLayer = dynamic_cast<HelloWorld*>(node);
		if(helloLayer != NULL)
		{
			int tag = MainMenuLayer::getLayerTagByName("WaitingLayer");
			CCNode* layer = helloLayer->getChildByTag(tag);
			WaitingLayer* waitingLayer = dynamic_cast<WaitingLayer*>(layer);
			if(waitingLayer != NULL)
			{
				waitingLayer->HideWaiting();
			}
		}
	}
}
static bool is_backkey_pressed = false;

void MainMenuLayer::OkBtnClick(CCObject* obj)
{
    is_backkey_pressed = false;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TmLoginManager::tmLoginManager()->doSdkExit();
#endif
	CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

void MainMenuLayer::CancelBtnClick(CCObject* obj)
{
	CCLOG("MainMenuLayer::CancelBtnClick");
     is_backkey_pressed = false;
}
void MainMenuLayer::keyBackClicked()
{
    CCLOG("key back is pressed");
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");
#endif
	if (bMainLandLevel
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		|| channel== Nd91
#endif
		)
	{
	    if(is_backkey_pressed)
         return;
	    
		//91 is shit. add it's exit ui,not our's
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		//PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");

		if(channel == Nd91 ) 
		{
			TmLoginManager::tmLoginManager()->doSdkExit();
		}
		else
#endif
		{
			is_backkey_pressed = true;
			std::string content = Localizatioin::getLocalization("S_Message_common_content");
			TXGUI::MessageBox::AdjustPriority(true);
			TXGUI::MessageBox::Show(content.c_str(),this,menu_selector(MainMenuLayer::OkBtnClick),
				menu_selector(MainMenuLayer::CancelBtnClick),MB_YESNO,ccWHITE,24,KMessageBoxExitApp);
			TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
		}
	}
}

void MainMenuLayer::keyMenuClicked()
{
	CCLOG("keyMenuClicked");
}

void MainMenuLayer::DarkEffectOver()
{
	StoryInstanceDirector::Get()->DarkEffectOver();	
}

const char* MainMenuLayer::GetNumByFormat(unsigned int num)
{
	char numStr[20];
	/*if(num >= 100000000)
	{
	unsigned int m = num / 1000000;
	sprintf(numStr,"%dm",m);
	}
	else if(num >= 100000)
	{
	unsigned int k = num / 1000;
	sprintf(numStr,"%dk",k);
	}
	else
	{
	sprintf(numStr,"%d",num);
	}*/
	if(num >= 100000000)
	{
		const char* text = Localizatioin::getLocalization("M_ONE_HUNDRED_MILLION");
		unsigned int tenTh = num / 100000000;
		sprintf(numStr,"%u%s",tenTh,text);
	}
	else if(num >= 10000)
	{
		const char* text = Localizatioin::getLocalization("M_TENTHOUSAND");
		unsigned int tenTh = num / 10000;
		sprintf(numStr,"%u%s",tenTh,text);
	}
	else
	{
		sprintf(numStr,"%u",num);
	}
	CCString* pRet = CCString::create(numStr);
	return pRet->m_sString.c_str();
}

const char* MainMenuLayer::GetNumByFormatK(unsigned int num)
{
	char numStr[20];
	if(num >= 100000000)
	{
	unsigned int m = num / 1000000;
	sprintf(numStr,"%dm",m);
	}
	else if(num >= 100000)
	{
	unsigned int k = num / 1000;
	sprintf(numStr,"%dk",k);
	}
	else
	{
	sprintf(numStr,"%d",num);
	}
	CCString* pRet = CCString::create(numStr);
	return pRet->m_sString.c_str();
}

bool MainMenuLayer::IsNotice2Load()
{
    bool isLoad = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "IsLoadNotice2");
    return isLoad;
}
bool MainMenuLayer::IsAutoAppear()
{
	
	bool isLoad = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "IsLoadAutoAppear");
	return isLoad;

}

void MainMenuLayer::setFairyMovedIcon(unsigned int changedPos)
{
	if(m_pBackPackUI != NULL)
	{
		m_pBackPackUI->onReceiveFairyChanged(changedPos);
	}
}

void MainMenuLayer::setFairySelectStatus(bool isSelected)
{
	if(m_pBackPackUI != NULL)
	{
		m_pBackPackUI->setFairySelectStatus(isSelected);
	}
}

void MainMenuLayer::onReceivedElfStrengthLayerClosed()
{
	if(m_pBackPackUI != NULL)
	{
		m_pBackPackUI->closeEquipment(NULL);
	}
}

static long long lastSystem = 0;

void   LogCurrentSystem(const char* msg)
{
	long long curTime = millisecondNow();
	char text[100];
	long long deltaTime = curTime - lastSystem;
	sprintf(text,"system time (%s) is: %lld, %lld",msg,curTime,deltaTime);
	CCLOG(text);
	lastSystem = curTime;
}

void MainMenuLayer::ShowGuildInstanceListLayer(bool flag,bool popFlag/* = true*/)
{
	int tag = getLayerTagByName("GuildInstanceListLayer");

	if (flag == true && m_guildInstanceListLayer == NULL)
	{
		m_guildInstanceListLayer = new GuildInstanceListLayer();
		m_guildInstanceListLayer->init();

		this->addChild(m_guildInstanceListLayer, Z_Order_GuildInstanceListLayer, tag);
		PushOneLayer(m_guildInstanceListLayer);
	} else if (flag == false && m_guildInstanceListLayer != NULL)
	{
		if(popFlag)
		{
			RemoveOneLayer(m_guildInstanceListLayer);
		}
		this->removeChildByTag(tag);
		setGuildInstanceListLayer(NULL);
	}
}

void MainMenuLayer::ShowGuildInstanceHelpLayer(bool flag)
{
	int tag = getLayerTagByName("GuildInstanceHelpLayer");

	if (flag == true && m_guildInstanceHelpLayer == NULL)
	{
		m_guildInstanceHelpLayer = new GuildInstanceHelpLayer();
		m_guildInstanceHelpLayer->init();

		this->addChild(m_guildInstanceHelpLayer, Z_Order_GuildInstanceHelpLayer, tag);
	} else if (flag == false && m_guildInstanceHelpLayer != NULL)
	{
		this->removeChildByTag(tag);
		setGuildInstanceHelpLayer(NULL);
	}
}

int MainMenuLayer::getSelectedCellOfLeft()
{
	GuildRedEnvolopeLayer* layer = dynamic_cast<GuildRedEnvolopeLayer *>(m_guildRedEnvolopeLayer);

	return layer->m_leftSelectedIndex;
}