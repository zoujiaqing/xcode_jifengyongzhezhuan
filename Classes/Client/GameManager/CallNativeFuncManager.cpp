#include "CallNativeFuncManager.h"
#include "GameManager.h"
#include "CCBLoaderManager.h"
#include "RankListLayer.h"
#include "MainMenuLayer.h"
#include "TreasureExploreLayer.h"
#include "ItemExchangeShopLayer.h"
#include "CombatSetting.h"
#include "InstanceManager.h"
#include "LevelManager.h"

static CallNativeFuncManager* m_instance = NULL;
CallNativeFuncManager::CallNativeFuncManager()
{

}

CallNativeFuncManager::~CallNativeFuncManager()
{
    
}

CallNativeFuncManager* CallNativeFuncManager::getInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new CallNativeFuncManager();
	}

	return m_instance;
}
    
void CallNativeFuncManager::AddIndexLayerToMainScene(int whatLayer)
{
	switch((NewLayerFlag)whatLayer)
	{
	case RANK_LIST_LAYER:
		this->AddRankListLayer();
		break;

    case TREASURE_EXPLORE_LAYER:
        this->ShowTreasureExploreLayer(true);
        break;

    case ITEM_EXCHANGE_SHOP_LAYER:
        this->ShowItemExchangeShopLayer(true);
        break;

	default:
		break;
	}
}

void CallNativeFuncManager::AddRankListLayer()
{
    CCNode* pMenuLayerNode = (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    CCNodeLoader* loader = new RankListLayerNodeLoader();
    CCNode* rankNode = CCBLoaderManager::getInstance()->LoadFromCCBI(loader, "rank_list", "rank_list");
    int tag = dynamic_cast<MainMenuLayer*>(pMenuLayerNode)->getLayerTagByName("RankListLayer");
    if(pMenuLayerNode->getChildByTag(tag))
    {
        return;
    }
    CCSize size = pMenuLayerNode->getContentSize();
    rankNode->setPosition(ccp(size.width / 2, size.height / 2));
    float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
    rankNode->setScale(scale);
    pMenuLayerNode->addChild(rankNode, Z_Order_RankListLayer, tag);
}

void CallNativeFuncManager::RemoveRankListLayer()
{
    CCNode* pMenuLayerNode = (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    int tag = dynamic_cast<MainMenuLayer*>(pMenuLayerNode)->getLayerTagByName("RankListLayer");
    pMenuLayerNode->removeChildByTag(tag, true);
}

void CallNativeFuncManager::ShowRankListLayer(bool flag)
{
    if(flag == true)
    {
        AddRankListLayer();
    }
    else
    {
        RemoveRankListLayer();
    }
}

 void CallNativeFuncManager::ShowTreasureExploreLayer(bool flag,bool isPopLayer)
 {
     CCNode* pMenuLayerNode = (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
     int tag = dynamic_cast<MainMenuLayer*>(pMenuLayerNode)->getLayerTagByName("TreasureExploreLayer");
     if(flag == true)
     {
         CCNodeLoader* loader = new TreasureExploreLayerLoader();
         CCNodeLoader* sigleLoader = new SingleTreasureLayerLoader();
         std::vector<CCNodeLoader*> loaderVec;
         loaderVec.clear();
         loaderVec.push_back(loader);
         loaderVec.push_back(sigleLoader);

         std::vector<const char*> classVec;
         classVec.clear();
         classVec.push_back("TreasureExploreLayer");
         classVec.push_back("SingleTreasure");


         CCNode* treasureNode = CCBLoaderManager::getInstance()->LoadFromDoubleCCBI("TreasureExploreLayer", classVec, loaderVec);
         if(pMenuLayerNode->getChildByTag(tag))
         {
             return;
         }
         CCSize size = pMenuLayerNode->getContentSize();
         treasureNode->setPosition(ccp(size.width / 2, size.height / 2));
         float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
         treasureNode->setScale(scale);
         pMenuLayerNode->addChild(treasureNode, Z_Order_TreasureExploreLayer, tag);
		 MainMenuLayer::PushOneLayer(dynamic_cast<CCLayer*>(treasureNode));
     }
     else
     {
		 if(isPopLayer)
		 {
			 CCLayer* layer = dynamic_cast<CCLayer*>(pMenuLayerNode->getChildByTag(tag));
			 if(layer)
			 {
				 MainMenuLayer::RemoveOneLayer(layer);
			 }
		 }
         pMenuLayerNode->removeChildByTag(tag, true);
     }
 }

 void CallNativeFuncManager::ShowItemExchangeShopLayer(bool flag)
 {
     CCNode* pMenuLayerNode = (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
     int tag = dynamic_cast<MainMenuLayer*>(pMenuLayerNode)->getLayerTagByName("ItemExchangeShopLayer");
     if(flag == true)
     {
         CCNodeLoader* loader = new ItemExchangeShopLayerLoader();
         CCNode* Node = CCBLoaderManager::getInstance()->LoadFromCCBI(loader, "ItemExchangeShopLayer", "ItemExchangeShopLayer");
         if(pMenuLayerNode->getChildByTag(tag))
         {
             return;
         }
         CCSize size = pMenuLayerNode->getContentSize();
         Node->setPosition(ccp(size.width / 2, size.height / 2));
         float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
         Node->setScale(scale);
         pMenuLayerNode->addChild(Node, Z_Order_ItemExchangeShopLayer, tag);
     }
     else
     {
         pMenuLayerNode->removeChildByTag(tag, true);
     }
 }

 void CallNativeFuncManager::ShowCombatSettingLayer(bool flag)
 {
     CCNode* pMenuLayerNode = (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
     int tag = dynamic_cast<MainMenuLayer*>(pMenuLayerNode)->getLayerTagByName("CombatSetting");
     if(flag == true)
     {
         CCNodeLoader* loader = new CombatSettingLoader();
         CCNode* Node = CCBLoaderManager::getInstance()->LoadFromCCBI(loader, "CombatSetting", "CombatSetting");
         if(pMenuLayerNode->getChildByTag(tag))
         {
             return;
         }
         CCSize size = pMenuLayerNode->getContentSize();
         Node->setPosition(ccp(size.width / 2, size.height / 2));
         float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
         Node->setScale(scale);
         pMenuLayerNode->addChild(Node, Z_Order_CombatSetting, tag);
     }
     else
     {
         pMenuLayerNode->removeChildByTag(tag, true);
     }
 }

void CallNativeFuncManager::SetRankLayerVipFlgVisible(bool flag)
{
    CCNode* pMenuLayerNode = (CCNode*)GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    int tag = dynamic_cast<MainMenuLayer*>(pMenuLayerNode)->getLayerTagByName("RankListLayer");
    RankListLayer* pLayer = dynamic_cast<RankListLayer*>(pMenuLayerNode->getChildByTag(tag));
    if(pLayer == NULL)
    {
        return;
    }
}

void CallNativeFuncManager::ShowWBCombatSettingTip(bool flag)
{
    UIManager *manager = UIManager::sharedManager();
    UILayout* layout = manager->getUILayout("MainMenu");
    UIButton* guildBtn = layout->FindChildObjectByName<TXGUI::UIButton>("button_bossCombatSetting");
    if (!guildBtn)
    {
        return;
    }

    if(flag == true)
    {
        guildBtn->showTipIcon();
    }
    else
    {
        guildBtn->hideTipIcon();
    }
}

bool CallNativeFuncManager::isOwnGuildPlayer(int otherHighGid, int otherLowGid)
{
    bool flag = true;
    USERINFO info = UserData::GetUserInfo();
    if(info.m_guildId.getHighID() == otherHighGid && info.m_guildId.getLowID() == otherLowGid)
    {
        flag = true;
    }
    else
    {
        flag = false;
    }

    return flag;
}