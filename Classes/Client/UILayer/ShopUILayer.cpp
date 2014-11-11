//
//  ShopUILayer.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-6-18.
//
//
#include "PVPMessage.h"
#include "GMessage.h"
#include "ShopUILayer.h"
#include "Localization.h"
#include "GameFontManager.h"
#include "particle_manager.h"
#include "UserData.h"
#include "GameAudioManager.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "ItemManager.h"
#include "TipMgr.h"
#include "TimeManager.h"
#include "LuaTinkerManager.h"
#include "TutorialsManager.h"
#include "ItemManager.h"
#include "OnlineNetworkManager.h"
#include "MainMenuLayer.h"
#include "TalentLayer.h"

using namespace TXGUI;

ShopUILayer::ShopUILayer()
{
    this->m_pLayer = NULL;
    this->m_pShopList = NULL;
    this->m_pUseDiamand = NULL;
    this->m_shopSate = kBuyBack;
    this->m_bInitRemainTime = false;
    this->m_bOpenDiamand = false;
    this->m_remainTime = 0;
    this->m_exchangeItemList = NULL;
	this->m_ShopTouchEndPt = CCPoint(0,0);
	this->refreshDiamond = 0;
}

ShopUILayer::~ShopUILayer()
{
    UIManager::sharedManager()->RemoveUILayout("ShopUI");
    UIManager::sharedManager()->RemoveUILayout("UseDiamand_Shop");
    TimeManager::Get()->unregisterTimer(this, PB_ATTR_SHOP_CD);
}

void ShopUILayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

bool ShopUILayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/ShopUI.plist", this, "ShopUI", true);
	CCAssert(m_pLayer != NULL,"Shop ui is null");
	if(m_pLayer == NULL)
	{
		return false;
	}
    //manager->RegisterMenuHandler("ShopUI","ShopUILayer::OnClose", menu_selector(ShopUILayer::OnClose), this);
    manager->RegisterMenuHandler("ShopUI","ShopUILayer::OnFreshShoplist",	menu_selector(ShopUILayer::OnFreshShoplist), this);
    manager->RegisterMenuHandler("ShopUI","ShopUILayer::OnCallbackBuylist",	menu_selector(ShopUILayer::OnCallbackBuylist), this);
	if(m_pLayer)
		m_pShopList = m_pLayer->FindChildObjectByName<UIScrollList>("shop_scrolllist");
    const char* str_shoptile = Localizatioin::getLocalization("M_SHOP_TITLE2");
	if(m_pLayer)
	{
		UILabel* shoptile_label = m_pLayer->FindChildObjectByName<UILabel>("shoptile");
		shoptile_label->setString(str_shoptile);
	
		UILabel* shophint_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_hint");
		str_shoptile = Localizatioin::getLocalization("M_SHOP_HINT");
		if(shophint_label)
			shophint_label->setString(str_shoptile);
	}

    setupIconSide();
	return true;
}

void ShopUILayer::openShop()
{
    if(!m_bOpenDiamand)
    {
        UIManager *manager = UIManager::sharedManager();
        m_pUseDiamand = manager->CreateUILayoutFromFile("UIplist/UseDiamand_Shop.plist", this, "UseDiamand_Shop", true);
        manager->RegisterMenuHandler("UseDiamand_Shop","ShopUILayer::OnConfirm",	menu_selector(ShopUILayer::OnConfirm), this);
        manager->RegisterMenuHandler("UseDiamand_Shop","ShopUILayer::OnCancle",	menu_selector(ShopUILayer::OnCancle), this);
        m_pUseDiamand->setVisible(false);
        setDiamandTxt();

        m_bOpenDiamand = true;
    }
    processBottomBoard(true);

	//renew timer when open shop ui.
	initRemainTimer(TalentLayer::m_stopTimer);
}

void ShopUILayer::closeShop()
{
    m_bOpenDiamand = false;
    UIManager::sharedManager()->RemoveUILayout("UseDiamand_Shop");
}

void ShopUILayer::setDiamandTxt()
{
    UILabel* confirmLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_confirm");
    UILabel* cancleLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_cancel");
    UILabel* tileLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_tile");
    
    std::string msg = "";
    msg = Localizatioin::getLocalization("M_CONFIRM");
    confirmLabel->setString(msg.c_str());
    
    msg = Localizatioin::getLocalization("M_CANCEL");
    cancleLabel->setString(msg.c_str());
    
    msg = Localizatioin::getLocalization("M_RENOVATESHOP_COSTDIAMOND");
    tileLabel->setString(msg.c_str());
    
    confirmLabel->setColor(ccWHITE);
    cancleLabel->setColor(ccWHITE);
    tileLabel->setColor(ccWHITE);
}

void ShopUILayer::setupNobuyBack(bool isvisible)
{
    UIPicture* nobuyPicture = m_pLayer->FindChildObjectByName<UIPicture>("shop_frame_nobuyback");
    UILabel* nobuytext = m_pLayer->FindChildObjectByName<UILabel>("shop_text_nobuyback");
    string str_cost = Localizatioin::getLocalization("M_SHOP_NOBUYBACK_HINT");
    nobuytext->setString(str_cost.c_str());
    nobuyPicture->setVisible(isvisible);
    nobuytext->setVisible(isvisible);
}

void ShopUILayer::setupIconSide()
{
    UIPicture* iconside = m_pLayer->FindChildObjectByName<UIPicture>("shop_side_icon");
    CCPoint pt;
    ASprite* pAsprite2 = AspriteManager::getInstance()->getAsprite("UI/ui_system_icon.bin");
    
    int frame;
    switch (m_shopSate) {
        case kBuyBack:
            frame = TXGUI::getResourceIDByName("map_ui_system_icon_FRAME_ICON_SHOP");
            break;
            
        case kShop:
            frame = TXGUI::getResourceIDByName("map_ui_system_icon_FRAME_ICON_STORAGE");
            break;
            
        case kExchange:
            frame = TXGUI::getResourceIDByName("map_ui_system_icon_FRAME_ICON_STORAGE");
            break;
            
        default:
            break;
    }
    CCSprite* icon = pAsprite2->getSpriteFromFrame_Middle(frame ,0, pt);
    iconside->setSprite(icon);
}

void ShopUILayer::sendShopList()
{
    PVPMessage::Get()->sendShopItems();
    setShopBoard();
    
    const char* str_shoptile = Localizatioin::getLocalization("M_SHOP_TITLE2");
    UILabel* shoptile_label = m_pLayer->FindChildObjectByName<UILabel>("shoptile");
    shoptile_label->setString(str_shoptile);
    
    UILabel* shophint_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_hint");
    str_shoptile = Localizatioin::getLocalization("M_SHOP_HINT");
    shophint_label->setString(str_shoptile);

    
    if(!m_bInitRemainTime)
        initRemainTimer(TalentLayer::m_stopTimer);
    
    m_shopSate = kBuyBack;
    setupIconSide();
    
    UIButton* btnfresh = m_pLayer->FindChildObjectByName<UIButton>("shop_update_btn");
    UILabel* btnfresh_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_update");
    btnfresh->setVisible(true);
    btnfresh_label->setVisible(true);
    
    UILabel* btncallbackbuy_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_ready");
    const char* str_callback = Localizatioin::getLocalization("M_SHOP_BUYBACK");
    btncallbackbuy_label->setString(str_callback);
    setupNobuyBack(false);
}

void ShopUILayer::setupShopList(std::vector<shopItem>& shoplist)
{
    m_pShopList->clearChildrens();
    m_BtnList.clear();
    m_shopPriceList.clear();
	fromIdList.clear();
    
	int btnIndex = 0;
    std::vector<shopItem>::iterator it = shoplist.begin();
    for(; it != shoplist.end(); ++it)
    {
        int  to_id    = it->to_id;
        int  to_num   = it->to_num;
        
        int  from_id  = it->from_id;
        int  from_num = it->from_num;
        
        bool isBuy    = it->isBuy;
        int  index    = it->index;
        
        m_shopPriceList.push_back(from_num);
        createButtonNode(to_id, to_num, from_id, from_num, isBuy, index ,btnIndex, true);
		btnIndex ++;
    }
}

void ShopUILayer::showExchangeShopList()
{
    m_BtnList.clear();
    m_shopSate = kExchange;
    processBottomBoard(false);
    m_pricelist.clear();
    setExchangeBoard();
    
    const char* str_shoptile = Localizatioin::getLocalization("M_EXCHANGESHOP_TITLE");
    UILabel* shoptile_label = m_pLayer->FindChildObjectByName<UILabel>("shoptile");
    shoptile_label->setString(str_shoptile);
    
    if(!m_exchangeItemList)
    {
        m_exchangeItemList = new std::map<unsigned int,unsigned int>();
        list<unsigned int>* exchangeIds = ItemManager::Get()->getExchangeSpriteStones(8);
        if(exchangeIds)
        {
            for(std::list<unsigned int>::iterator it = exchangeIds->begin();
                it != exchangeIds->end();it++)
            {
                unsigned int exchangeId = *it;
                ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
                if(item && item->m_toItems && item->m_toItems->size() == 1)
                {
                    for (std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_toItems->begin();
                         iter != item->m_toItems->end();iter++)
                    {
                        m_exchangeItemList->insert(make_pair((*iter).first,exchangeId));
                    }
                }
                
                if(item && item->m_fromItems && item->m_fromItems->size() == 1)
                {
                    for (std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_fromItems->begin();
                         iter != item->m_fromItems->end();iter++)
                    {
                        m_pricelist.push_back((*iter).second);
                    }
                }
            }
            
            exchangeIds->clear();
            delete exchangeIds;
            exchangeIds = NULL;
        }
    }
    
    if(m_pShopList)
    {
        int index = 0;
        m_pShopList->clearChildrens();
        for(std::map<unsigned int,unsigned int>::iterator it = m_exchangeItemList->begin();
            it != m_exchangeItemList->end();it++)
        {
            int item_id = (*it).first;
            int buyindex = (*it).second;
            CCLOG("shop item_id:%d, buyindex:%d\n", item_id, buyindex);
            ItemBase* base = ItemManager::Get()->getItemInfoById(item_id);
            if(base)
            {
                int from_num = m_pricelist[index];
                createButtonNode(item_id, 0, 416009, from_num, false, buyindex, index, false);
                index += 1;
            }
		
        }
    }
    setupNobuyBack(false);
}

void ShopUILayer::createButtonNode(int item_id, int item_num, int from_id, int from_num, bool isbuy, int buyIndex,int btnIndex, bool shownum)
{
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    
    std::string backGroundFrameName = "map_ui_FRAME_SHOP_FRAME_ITEM";
    CCSprite* pBGFrame = AspriteManager::getInstance()->getOneFrame(KUI_BIN,backGroundFrameName);
    CCSize bgFrameSize = pBGFrame->getContentSize();
    
    CCNode* pBaseNode = CCNode::create();
    CCPoint relativePoint;
    CCPoint anchorPoint(0.5,0.5);
    std::string childFrameName = "";
    
    childFrameName = "map_ui_FRAME_SHOP_FRAME_ITEM";
    CCSprite* pChildSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
    pBaseNode->addChild(pChildSprite);
    pChildSprite->setPosition(ccp(pChildSprite->getPosition().x + 20, pChildSprite->getPosition().y));
    m_pShopList->AddBaseItem(pChildSprite->getContentSize(),pBaseNode);
    

    childFrameName = "map_ui_FRAME_SHOP_FRAME_ITEM1";
    CCSprite* pLeftCircleSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
    pLeftCircleSprite->setPosition(ccp(relativePoint.x + 24, relativePoint.y));
    pBaseNode->addChild(pLeftCircleSprite);
    
    CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item_id);
    pLeftCircleSprite->addChild(sprite,10);
    ItemBase* info = ItemManager::Get()->getItemInfoById(item_id);
    if(info)
    {
        CCSprite* framePic = ItemManager::Get()->getIconFrame(info->m_quality);
        if(framePic)
        {
            sprite->addChild(framePic);
            framePic->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
        }
       
    }
    sprite->setPosition(ccp(pLeftCircleSprite->getContentSize().width/2, pLeftCircleSprite->getContentSize().height/2));
    
    ///////////////////////////add  tip handler///////////////
    //add tip handler
    ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
    CCSprite* pEmptyItem = CCSprite::create();
    CCSize uiSize = pLeftCircleSprite->getContentSize();
    pEmptyItem->setContentSize(CCSize(uiSize.width,uiSize.height));
    CCPoint pt;
    int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_ALCHEMY_ICON_CLICKED");
    CCSprite* pSelectItem = as->getSpriteFromFrame_Middle(iFrameId,0,pt);
    IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, menu_selector(ShopUILayer::onShopTip));
    pIcon->autorelease();
    pIcon->setTag(item_id);
    pIcon->setContentSize(pEmptyItem->getContentSize());
    pLeftCircleSprite->addChild(pIcon,10);
    
    pIcon->setPosition(ccp(pLeftCircleSprite->getContentSize().width/2,
                           pLeftCircleSprite->getContentSize().height/2));
    ///////////////////////////////////////////////////////////
    
    if(shownum)
    {
        //char buf[10];
        //sprintf(buf, "%d", item_num);
        //UILabelTTF* label = UILabelTTF::create(buf, KJLinXin, 20);
        //sprite->addChild(label,0);
        //label->setPosition(ccp(sprite->getContentSize().width - label->getContentSize().width/2+5,
        //                       label->getContentSize().height/2-5));
		char buf[10];
		sprintf(buf, "%d", item_num);
		pIcon->setItemLabel(buf, 20, KJLinXin);
    }
    
    //add btn
    childFrameName = "map_ui_FRAME_SHOP_BUTTON_BUY";
    const char* str_normal = "";
    const char* str_sel = "";
    const char* btn_text = "M_DIAMONDSHOP_BUY";
    if(isbuy)
    {
        btn_text = "M_SHOP_ALREADYBUY";
        str_normal = "map_ui_FRAME_SHOP_BUTTON_CANTBUY";
        str_sel = "map_ui_FRAME_SHOP_BUTTON_CANTBUY";
    }
    else
    {
        btn_text = "M_DIAMONDSHOP_BUY";
        str_normal = "map_ui_FRAME_SHOP_BUTTON_BUY";
        str_sel = "map_ui_FRAME_SHOP_BUTTON_BUY_CLICKED";
    }
    AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
    CCSprite* pAutoTaskNormalIcon = AspriteManager::getInstance()->getOneFrame(KUI_BIN, str_normal);
    CCSprite* pAutoTaskClickIcon = AspriteManager::getInstance()->getOneFrame(KUI_BIN, str_sel);
    
    char buffer[100];
	sprintf(buffer,"btn_%d",btnIndex);

    IconButton* pbtn = new IconButton(pAutoTaskNormalIcon,0,pAutoTaskClickIcon,this,menu_selector(ShopUILayer::OnDoubleClick));
    pbtn->autorelease();
    pbtn->SetButtonName(buffer);
    pbtn->SetButtonIndex(buyIndex);
    pbtn->setPosition(relativePoint);
    pBaseNode->addChild(pbtn,0,9);
    if(isbuy)
        pbtn->getMenuItem()->setEnabled(false);
    
    std::string autoBtnText = Localizatioin::getLocalization(btn_text);
    UILabelTTF * autoFindPathLabel = UILabelTTF::create(autoBtnText.c_str(), KJLinXin, 20);
    pBaseNode->addChild(autoFindPathLabel,1);
    autoFindPathLabel->setPosition(relativePoint);
    
    //label icon name
    childFrameName = "map_ui_FRAME_SHOP_TEXT_ITEMNAME1";
    AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
    ItemBase* base  = ItemManager::Get()->getItemInfoById(item_id);
    if (base == NULL)
    {
        return;
    }
    const char* icon_name = base->m_name;
    CCLabelTTF* label_iconname = CCLabelTTF::create(icon_name, KJLinXin, 17);
    pBaseNode->addChild(label_iconname, 0, 10);
    label_iconname->setPosition(relativePoint);
  
    if(isbuy)
    {
        childFrameName = "map_ui_FRAME_SHOP_TEXT_ALREADYBUY1";
        AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
        
        const char* str_buy = Localizatioin::getLocalization(btn_text);
        UILabelTTF* label_buy = UILabelTTF::create(str_buy, KJLinXin, 20);
        pBaseNode->addChild(label_buy,0,11);
        label_buy->setColor(ccc3(20,255,0));
        label_buy->setPosition(relativePoint);
    }
    else
    {
        childFrameName = "map_ui_FRAME_SHOP_ICON_COST1";
        AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);

        CCPoint pt;
        CCSprite* money = NULL;
        if(from_id == 416001) //coin sprite
        {
            int idx = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_COIN");
            money = pAsprite->getSpriteFromFrame_Middle(idx, 0, pt);
        }
        else if(from_id == 416009)
        {
            int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_MEDAL");
            money = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        }
		else if(from_id == 416018)
        {
            int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_STONE");
            money = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        }
        else
        {
            int idx = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_DIAMOND");
            money = pAsprite->getSpriteFromFrame_Middle(idx, 0, pt);
        }
        
        pBaseNode->addChild(money,0,12);
        money->setPosition(ccp(relativePoint.x + 20, relativePoint.y));
        
        childFrameName = "map_ui_FRAME_SHOP_TEXT_ITEMCOST1";
        AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
        
        char buf[10];
        sprintf(buf, "%d", from_num);
        UILabelTTF* label_money = UILabelTTF::create(buf, KJLinXin, 20);
        pBaseNode->addChild(label_money,0,13);
        label_money->setPosition(relativePoint);
    }
    m_BtnList.push_back(pBaseNode);
	fromIdList.push_back(from_id);
}

void ShopUILayer::processBottomBoard(bool isvisible)
{
    UIPicture *diamond = m_pLayer->FindChildObjectByName<UIPicture>("shop_icon_diamond_inall");
    UILabel* diamond_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_diamond_inall");
    
    UIPicture *coin = m_pLayer->FindChildObjectByName<UIPicture>("shop_icon_icon_inall");
    UILabel* coin_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_coin_inall");
    
    UILabel* fresh_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_updatetime");
    UILabel* freshtime_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_updatetime");
    
    UIButton* btnfresh = m_pLayer->FindChildObjectByName<UIButton>("shop_update_btn");
    UILabel* btnfresh_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_update");
    
    UIButton* btncallbackbuy = m_pLayer->FindChildObjectByName<UIButton>("shop_update_ready_btn");
    UILabel* btncallbackbuy_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_ready");
    
    diamond->setVisible(true);
    diamond_label->setVisible(true);
    
    coin->setVisible(true);
    coin_label->setVisible(true);
    
    fresh_label->setVisible(isvisible);
    freshtime_label->setVisible(isvisible);
    
    btnfresh->setVisible(isvisible);
    btnfresh_label->setVisible(isvisible);
    
    btncallbackbuy->setVisible(isvisible);
    btncallbackbuy_label->setVisible(isvisible);
    
    int goldnum, diamandnum;
    if(m_shopSate == kExchange)
    {
        unsigned int drawingNum = ItemManager::Get()->getItemNumberById(416009);
        USERINFO info =  UserData::GetUserInfo();
        goldnum = drawingNum;
        diamandnum = info.m_diamond;
        
        ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
        pAsprite->setImageName(0, "UI/ui.png");
        CCPoint pt = CCPointZero;
        
        int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_COIN_INALL");
        CCSprite* pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        
        iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_DIAMOND_INALL");
        CCSprite* pdiamand = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        
        diamond->setSprite(pcoin);
        coin->setSprite(pdiamand);
        
        const char* buf = MainMenuLayer::GetNumByFormat(info.m_gold);
        if(buf)
           diamond_label->setString(buf);
        
        buf = MainMenuLayer::GetNumByFormat(goldnum);
        if(buf)
          coin_label->setString(buf);
    }
    else
    {
        USERINFO info =  UserData::GetUserInfo();
        goldnum = info.m_gold;
        diamandnum = info.m_diamond;
        
        ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
        pAsprite->setImageName(0, "UI/ui.png");
        CCPoint pt = CCPointZero;
        
        int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_COIN_INALL");
        CCSprite* pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        
        iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_DIAMOND_INALL");
        CCSprite* pdiamand = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);

		//将金币图标替换为魂石
		iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_STONE");
        CCSprite* pstone = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        
        diamond->setSprite(pdiamand);
        coin->setSprite(pstone);
        
        //const char* buf = MainMenuLayer::GetNumByFormat(goldnum);
        //if(buf)
        //   coin_label->setString(buf);
		updateSoulStoneCount();
        
		char buffer[100];
		sprintf(buffer,"%d",diamandnum);
        //buf = MainMenuLayer::GetNumByFormat(diamandnum);
        if(buffer)
          diamond_label->setString(buffer);
    } 


    if(!isvisible)
        return;
    
    const char* str_freshtime = Localizatioin::getLocalization("M_SHOP_UPDATETIME");
    const char* str_fresh = Localizatioin::getLocalization("M_SHOP_UPDATE");
    const char* str_callback = Localizatioin::getLocalization("M_SHOP_BUYBACK");
    
    freshtime_label->setString("00:00");
    fresh_label->setString(str_freshtime);
    
    btnfresh_label->setString(str_fresh);
    btncallbackbuy_label->setString(str_callback);

	//更新刷新卡数量
	int cardCount = LuaTinkerManager::Get()->callLuaFunc<int>("Script/main.lua", "GetRefreshCardCount");

	ostringstream ss;
	ss << cardCount;

	if (m_pLayer)
	{
		UILabel * label = m_pLayer->FindChildObjectByName<UILabel>("cardNum");
		if (label)
		{
			label->setString(ss.str().c_str());
		}
	}


}

void ShopUILayer::floatCoin(int coinnum)
{
    string str_cost = "";
    if(m_shopSate == kShop)
       str_cost= Localizatioin::getLocalization("M_SHOP_COST");
    else if(m_shopSate == kBuyBack)
       str_cost= Localizatioin::getLocalization("M_SHOP_GETCOIN");
    else if(m_shopSate == kExchange)
        str_cost= Localizatioin::getLocalization("M_SHOP_GETCOIN");

    char buf[10];
    sprintf(buf, "%d", coinnum);
    str_cost += " ";
    str_cost += buf;
    
	MainMenuLayer* pMainMenuLayer = GetMainMenuLayer();
	if (pMainMenuLayer == NULL)
	{
		return ;
	}
	unsigned int labelTag = 31010;
	CCNode* pOldLabel = pMainMenuLayer->getChildByTag(labelTag);
	if (pOldLabel)
	{
		pOldLabel->removeFromParentAndCleanup(true);
		pOldLabel = NULL;
	}	
	
	UILabelTTF* label = UILabelTTF::create(str_cost.c_str(), KJLinXin, 25);
	label->setColor(ccORANGE);
	CCMoveBy* moveby = CCMoveBy::create(0.8f, ccp(0,40));
	CCCallFuncN* f = CCCallFuncN::create(this, callfuncN_selector(ShopUILayer::removeLabel));
	CCSequence* seq = (CCSequence*)CCSequence::create(moveby, f, NULL);
	label->runAction(seq);
	pMainMenuLayer->addChild(label,1000, labelTag);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    label->setPosition(ccp(size.width/2, size.height/2));
    
    if(m_shopSate == kExchange)
    {
        UILabel* diamond_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_diamond_inall");
        USERINFO info =  UserData::GetUserInfo();
        int goldnum = info.m_gold;
        goldnum += coinnum;
        
        const char* buf = MainMenuLayer::GetNumByFormat(goldnum);
        if(buf)
           diamond_label->setString(buf);
    }
    
    else
    {
        UILabel* coin_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_coin_inall");
        USERINFO info =  UserData::GetUserInfo();
        int goldnum = info.m_gold;
        goldnum += coinnum;
        
        const char* buf = MainMenuLayer::GetNumByFormat(goldnum);
        if(buf)
           coin_label->setString(buf);
    }
}

void ShopUILayer::removeLabel(CCNode* sender)
{
    sender->removeFromParentAndCleanup(true);
}

void ShopUILayer::SendToBag(int DragPos)
{
    if(m_shopSate == kShop || !isVisible())
        return;
    
    UIPicture *framePic = m_pLayer->FindChildObjectByName<UIPicture>("shop_background");
	if(framePic)
	{
		CCPoint pos = framePic->getSpritePos();
		CCSize size = framePic->getSpriteSize();
        
        if((m_ShopTouchEndPt.x >= pos.x - size.width /2) && (m_ShopTouchEndPt.x <= pos.x + size.width /2)
           && (m_ShopTouchEndPt.y >= pos.y - size.height /2) && (m_ShopTouchEndPt.y <= pos.y + size.height /2))
        {
            PVPMessage::Get()->sendToBag(DragPos);
            return;
        }
	}
    return;
}

bool ShopUILayer::IsInShop(CCPoint location)
{
    if(m_shopSate == kShop || !isVisible())
        return false;
    
    UIPicture *framePic = m_pLayer->FindChildObjectByName<UIPicture>("shop_background");
	if(framePic)
	{
		CCPoint pos = framePic->getSpritePos();
		CCSize size = framePic->getSpriteSize();
        
        if((location.x >= pos.x - size.width /2) && (location.x <= pos.x + size.width /2)
           && (location.y >= pos.y - size.height /2) && (location.y <= pos.y + size.height /2))
        {
            //PVPMessage::Get()->sendToBag(DagPos);
            return true;
        }
	}
    return false;
}

void ShopUILayer::buyshop(int index, bool succ)
{
    CCNode* pBaseNode = m_BtnList[index];
    if(pBaseNode && succ)
    {
        CCPoint relativePoint;
        CCPoint anchorPoint(0.5,0.5);

        ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
        pAsprite->setImageName(0, "UI/ui.png");
        
        std::string backGroundFrameName = "map_ui_FRAME_SHOP_FRAME_ITEM";
        const char* childFrameName = "map_ui_FRAME_SHOP_TEXT_ALREADYBUY1";
        AspriteManager::getInstance()->getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
        
        const char* str_buy = Localizatioin::getLocalization("M_SHOP_ALREADYBUY");
        UILabelTTF* label_buy = UILabelTTF::create(str_buy, KJLinXin, 20);
        pBaseNode->addChild(label_buy,0,11);
        label_buy->setColor(ccc3(20,255,0));
        label_buy->setPosition(relativePoint);
        
        CCSprite* money = (CCSprite*)pBaseNode->getChildByTag(12);
        UILabelTTF* money_label = (UILabelTTF*)pBaseNode->getChildByTag(13);
        if(money)
            money->setVisible(false);
        if(money_label)
            money_label->setVisible(false);
        
        IconButton* pbtn = (IconButton*)pBaseNode->getChildByTag(9);
        const char* str_normal = "map_ui_FRAME_SHOP_BUTTON_CANTBUY";
        const char* str_sel = "map_ui_FRAME_SHOP_BUTTON_CANTBUY";
        CCSprite* pAutoTaskNormalIcon = AspriteManager::getInstance()->getOneFrame(KUI_BIN, str_normal);
        CCSprite* pAutoTaskClickIcon = AspriteManager::getInstance()->getOneFrame(KUI_BIN, str_sel);
        if(pbtn)
        {
            pbtn->getMenuItem()->setNormalImage(pAutoTaskNormalIcon);
            pbtn->getMenuItem()->setSelectedImage(pAutoTaskClickIcon);
            pbtn->getMenuItem()->setEnabled(false);
        }
    }
    
    string str_cost = "";
    if(succ)
    {
        int cost = 0;
        if(index < m_shopPriceList.size())
           cost = m_shopPriceList[index];
        str_cost = Localizatioin::getLocalization("M_SHOP_COST");


		if (index < fromIdList.size())
		{
			if (fromIdList[index] == 416018)
			{
				str_cost = Localizatioin::getLocalization("M_SHOP_COST_STONE");
			}
			else
			{
				str_cost = Localizatioin::getLocalization("M_SHOP_COST_DIAMOND");
			}
		}
		
		ostringstream oss;
		oss << str_cost << cost;

        //UILabel* coin_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_coin_inall");
        //USERINFO info =  UserData::GetUserInfo();
        //int goldnum = info.m_gold;
        //goldnum -= cost;
        //
        ////sprintf(buf, "%d", goldnum);
        //const char* strbuf = MainMenuLayer::GetNumByFormat(goldnum);
        //if(strbuf)
        //   coin_label->setString(strbuf);
		updateSoulStoneCount();
        

        ShowMessage(oss.str().c_str());
    }
    //else
    //str_cost = Localizatioin::getLocalization("M_SHOP_ALREADYBUY_HINT");
}

void ShopUILayer::freshShopList()
{
    sendShopList();
    updateCoinAndDiamand();
}

void ShopUILayer::reconstructCallBuyScrollist()
{
    m_pShopList->clearChildrens();
    m_BtnList.clear();
    
    int startpos = 1;
    int endpos = 20;
    
    /*
    int  ss = 50012;
    int index = (ss - 50000);
    int index_end = (index-1)%20;
     
    if(index_end == 0)
        index_end = 20;*/
    
    //12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11
    
    bool isHave = false;
	int  btnIndex = 0;
    for(int start = startpos; start <= endpos; start++)
    {
        BackPackItem* item = ItemManager::Get()->findItemByPos(50000+start);
        if(item && !item->isEmpty && !item->isLocked){
            
            int item_id = item->getItemId();
            ItemBase* base  = ItemManager::Get()->getItemInfoById(item_id);
            
            int item_num = item->amount;
            
            int from_id = 416001;
            int from_num = base->m_sellprice;
            
            bool isbuy = false;
            int buyindex = start+50000;
            createButtonNode(item_id, item_num, from_id, from_num, isbuy, buyindex,btnIndex, true);
         
			btnIndex ++;
            isHave = true;
        }
    }
    if(!isHave)
        setupNobuyBack(true);
    else
        setupNobuyBack(false);
}

void ShopUILayer::updateCoinAndDiamand()
{
    UILabel* diamond_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_diamond_inall");
    UILabel* coin_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_coin_inall");
    
    USERINFO info =  UserData::GetUserInfo();
    int goldnum = info.m_gold;
    int diamandnum = info.m_diamond;
    
    if(m_shopSate == kExchange)
    {
        const char* buf = MainMenuLayer::GetNumByFormat(goldnum);
        if(buf)
            diamond_label->setString(buf);
        
        unsigned int drawingNum = ItemManager::Get()->getItemNumberById(416009);
        buf = MainMenuLayer::GetNumByFormat(drawingNum);
        if(buf)
            coin_label->setString(buf);
    }
    else
    {
        //const char* buf = MainMenuLayer::GetNumByFormat(goldnum);
        //if(buf)
        //    coin_label->setString(buf);
		updateSoulStoneCount();

		char buffer[100];
		sprintf(buffer,"%d",diamandnum);
        //buf = MainMenuLayer::GetNumByFormat(diamandnum);
        if(buffer)
            diamond_label->setString(buffer);
    }
}

void ShopUILayer::updateSpendDiamand()
{
     const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", 10800, "FromItems");
     string exchangeStr = string(fromItems);
     int dashPos = exchangeStr.find_first_of("/");
     if(dashPos != exchangeStr.npos)
     {
       unsigned int length = exchangeStr.size();
       std::string idStr = exchangeStr.substr(0,dashPos);
       std::string numStr = exchangeStr.substr(dashPos+1,length-1);
       int id = atoi(idStr.c_str());
       int num = atoi(numStr.c_str());
       ASSERT(id == 416002,"cost is not diamond!");
         
       unsigned int diamond = (m_remainTime / 3600) * num;
       if(m_remainTime % 60 != 0)
       {
            diamond += num;
       }
       UILabel* diamand_label = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_num");
       char buf[10];
       sprintf(buf, "%d", diamond);
       diamand_label->setString(buf);
	   this->refreshDiamond = diamond;
    }
}

void ShopUILayer::OnClose(CCObject *sender)
{
	if (m_pShopList)
	{
		m_pShopList->clearChildrens();
	}
    
    setVisible(false);
    closeShop();
    ShowChatBar(true, -1);
}

//press fresh button
void ShopUILayer::OnFreshShoplist(CCObject* sender)
{
	int cardCount = LuaTinkerManager::Get()->callLuaFunc<int>("Script/main.lua", "GetRefreshCardCount");
	
	//检查是否已经提示过花钻
	bool shopRemindCost = CCUserDefault::sharedUserDefault()->getBoolForKey("shopRemindCost");
	if (cardCount <= 0 && !shopRemindCost)
	{
		updateSpendDiamand();

		USERINFO info =  UserData::GetUserInfo();
        int nowDiamond = info.m_diamond;
		if (this->refreshDiamond > nowDiamond)
		{
			const char * notEnoughDiamond =  Localizatioin::getLocalization("M_SHOP_DIAMOND");
			ShowMessage(notEnoughDiamond);
			return;
		}

		m_pUseDiamand->setVisible(true);


		CCUserDefault::sharedUserDefault()->setBoolForKey("shopRemindCost", true);
		CCUserDefault::sharedUserDefault()->flush();
	}
	else
	{
		OnConfirm(NULL);
	}
    
}

void ShopUILayer::OnCallbackBuylist(CCObject* sender)
{
    if(m_shopSate == kBuyBack)
    {
        reconstructCallBuyScrollist();
        m_shopSate = kShop;
        
        UIButton* btnfresh = m_pLayer->FindChildObjectByName<UIButton>("shop_update_btn");
        UILabel* btnfresh_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_update");
        btnfresh->setVisible(false);
        btnfresh_label->setVisible(false);
        
        UILabel* btncallbackbuy_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_ready");
        const char* str_callback = Localizatioin::getLocalization("M_BACK");
        btncallbackbuy_label->setString(str_callback);
        
        const char* str_shoptile = Localizatioin::getLocalization("M_SHOP_BUYBACK");
        UILabel* shoptile_label = m_pLayer->FindChildObjectByName<UILabel>("shoptile");
        shoptile_label->setString(str_shoptile);
        
        UILabel* shophint_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_hint");
        str_shoptile = Localizatioin::getLocalization("M_SHOP_HINT");
        shophint_label->setString("");
    }
    else
    {
        sendShopList();
        m_shopSate = kBuyBack;
        UIButton* btnfresh = m_pLayer->FindChildObjectByName<UIButton>("shop_update_btn");
        UILabel* btnfresh_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_update");
        btnfresh->setVisible(true);
        btnfresh_label->setVisible(true);
        
        UILabel* btncallbackbuy_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_ready");
        const char* str_callback = Localizatioin::getLocalization("M_SHOP_BUYBACK");
        btncallbackbuy_label->setString(str_callback);
        
        const char* str_shoptile = Localizatioin::getLocalization("M_SHOP_TITLE2");
        UILabel* shoptile_label = m_pLayer->FindChildObjectByName<UILabel>("shoptile");
        shoptile_label->setString(str_shoptile);
        
        UILabel* shophint_label = m_pLayer->FindChildObjectByName<UILabel>("shop_text_hint");
        str_shoptile = Localizatioin::getLocalization("M_SHOP_HINT");
        shophint_label->setString(str_shoptile);
    }
    setupIconSide();
}

void ShopUILayer::OnDoubleClick(cocos2d::CCObject *sender)
{
    
    CCNode* node = dynamic_cast<CCNode*>(sender);
	if (0 != node)
	{        
		CCNode* pParentNode = node->getParent();
		if (pParentNode)
		{
			CCNode* iconNode = pParentNode->getParent();
			IconButton* button = dynamic_cast<IconButton*>(iconNode);
			if(button)
			{
                if (false == TutorialsManager::Get()->HandleOneEvent(button->getButtonName().c_str(),1))
                {
                    return ;
                }
				unsigned int buyindex = button->GetButtonIndex();
                switch (m_shopSate) {
                    case kBuyBack:
                        PVPMessage::Get()->buyShopItem(buyindex);
                        break;
                        
                    case kShop:
                        PVPMessage::Get()->sendToShop(buyindex);
                        break;
                        
                    case kExchange:
                        OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(buyindex,0);
                        break;
                    default:
                        break;
                }
			}
		}
	}
}

void ShopUILayer::OnConfirm(CCObject *sender)
{
	const char * refreshSuccess =  Localizatioin::getLocalization("M_SHOP_RESET");
	ShowMessage(refreshSuccess);

    int diamand_id = 10800;
    PVPMessage::Get()->sendSpeedUpReq(diamand_id);
    m_pUseDiamand->setVisible(false);
}

void ShopUILayer::OnCancle(CCObject *sender)
{
    m_pUseDiamand->setVisible(false);
}

void ShopUILayer::onShopTip(CCObject* sender)
{
    CCLOG("shopUILayer on ShopTip\n");
    
    CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
	if(menuItem)
	{
		CCNode* node = menuItem->getParent();
		UIMenu* menu = dynamic_cast<UIMenu*>(node);
		if(menu)
		{
			CCNode* parent = menu->getParent();
			IconButton* button = dynamic_cast<IconButton*>(parent);
			if(button)
			{
                int item_id = button->getTag();
                ItemManager::Get()->showItemTipsById(item_id, menu->getTouchPoint());
            }
        }
    }
}


void ShopUILayer::onUpdateRemainTime(long remainTime)
{
    char timeString[30];
    unsigned int hour = remainTime / 3600;
    unsigned int min = (remainTime - hour * 3600) / 60;
    unsigned int second = remainTime % 60;
    if(hour > 0)
    {
        sprintf(timeString,"%02d : %02d : %02d",hour,min,second);
    }
    else
    {
        sprintf(timeString,"%02d : %02d",min,second);
    }
    UILabel* freshtime_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_updatetime");
    freshtime_label->setString(timeString);
    m_remainTime = remainTime;
}

void ShopUILayer::initRemainTimer(long seconds)
{
    //long nowTime = TimeManager::Get()->getCurServerTime();
	if(TimeManager::Get()->hasTimer(PB_ATTR_SHOP_CD))
	{
		TimeManager::Get()->renewTimer(PB_ATTR_SHOP_CD, seconds);
		if (!TimeManager::Get()->hasObserver(this, PB_ATTR_SHOP_CD))
		{
			TimeManager::Get()->attachTimer(this, PB_ATTR_SHOP_CD);
		}
	}
	else
	{
        CCLOG("initRemainTimer second:%ld\n", seconds);
		TimeManager::Get()->registerTimer(this, PB_ATTR_SHOP_CD, seconds);
	}
    m_bInitRemainTime = true;
}

void ShopUILayer::freshExchangeCoinVal()
{
    if(m_shopSate == kExchange && isVisible())
    {
        UILabel* coin_label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_coin_inall");
        unsigned int drawingNum = ItemManager::Get()->getItemNumberById(416009);
        const char* buf = MainMenuLayer::GetNumByFormat(drawingNum);
        if(buf)
           coin_label->setString(buf);
    }
}

void ShopUILayer::setExchangeBoard()
{
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    CCPoint pt = CCPointZero;

    UIPicture *coin = m_pLayer->FindChildObjectByName<UIPicture>("shop_icon_icon_inall");
    UIPicture *diamond = m_pLayer->FindChildObjectByName<UIPicture>("shop_icon_diamond_inall");
    int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_MEDAL");
    CCSprite* psprite = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    coin->setSprite(psprite);
}

void ShopUILayer::setShopBoard()
{
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    CCPoint pt = CCPointZero;

    UIPicture *coin = m_pLayer->FindChildObjectByName<UIPicture>("shop_icon_icon_inall");
    UIPicture *diamond = m_pLayer->FindChildObjectByName<UIPicture>("shop_icon_diamond_inall");
    int iFrameID = TXGUI::getResourceIDByName("map_ui_FRAME_SHOP_ICON_DIAMOND");
    CCSprite* psprite = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    diamond->setSprite(psprite);
}

void ShopUILayer::updateRefreshCardCount()
{
	int count = LuaTinkerManager::Get()->callLuaFunc<int>("Script/main.lua", "GetRefreshCardCount");

	ostringstream ss;
	ss << count;

	if (m_pLayer)
	{
		UILabel * label = m_pLayer->FindChildObjectByName<UILabel>("cardNum");
		if (label)
		{
			label->setString(ss.str().c_str());
		}
	}
}
void ShopUILayer::updateSoulStoneCount()
{
	int count = LuaTinkerManager::Get()->callLuaFunc<int>("Script/main.lua", "GetSoulStoneCount");

	ostringstream ss;
	ss << count;

	if (m_pLayer)
	{
		UILabel * label = m_pLayer->FindChildObjectByName<UILabel>("shop_number_coin_inall");
		if (label)
		{
			label->setString(ss.str().c_str());
		}
	}
}

