#include "OnlineNetworkManager.h"
#include "InstanceListLayer.h"
#include "AspriteManager.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"
#include "GameFontManager.h"
#include "XLogicManager.h"
//#include "ResoureUpdate.h"
#include "GameConfigFromLuaManager.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "StoryInstanceDirector.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
#include "AutoOnHook.h"
#include "GameAudioManager.h"
#include "LuaTinkerManager.h"
#include "UserData.h"
#include "TutorialsManager.h"
#include "LevelLayer.h"
#include "SceneLayer.h"
#include "BYGraySprite.h"
#include "ItemManager.h"
#include "TipMgr.h"
#include "InterfaceManager.h"
#include "GMessage.h"

#define MAX_ROW 2
#define MAX_COL 4
#define INSTANCE_NODE_TAG 10000
#define TASK_MARK_TAG     12345
#define TASK_MARK_ZORDER  10000
#define BUTTON_TAG 10
using namespace TXGUI;

  
InstanceListLayer::InstanceListLayer() :
    m_instanceInfoLayer(NULL)
{
	
	this->instanceId =  0;
	this->instanceType = INSTANCE_TYPE_NORMAL;
	instanceListLayout = NULL;
	jingYingLayout = NULL;
	instanceDungeonLayout = NULL;
    showInstance = false;
    page_reset = -1;
	
}

InstanceListLayer::~InstanceListLayer()
{
	ShowChatBar(true);

	UIManager::sharedManager()->RemoveUILayout("InstanceList");
	UIManager::sharedManager()->RemoveUILayout("InstanceElite");
	UIManager::sharedManager()->RemoveUILayout("InstanceDungeon");
	InstanceManager::Get()->curInstanceList.clear();
}

void InstanceListLayer::setInstanceType(INSTANCE_TYPE v)
{
		if(this->instanceType != v)
		{
			UIManager::sharedManager()->RemoveUILayout("InstanceList");
			UIManager::sharedManager()->RemoveUILayout("InstanceElite");
			UIManager::sharedManager()->RemoveUILayout("InstanceDungeon");
			jingYingLayout = NULL;
			instanceListLayout = NULL;
			instanceDungeonLayout = NULL;
		}
        showInstance = true;
		instanceType = v;
}

bool InstanceListLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	m_curUiAs = AspriteManager::getInstance()->getAsprite(KUI_BIN);

	this->setVisible(false);

	taskInstanceId = 0;

	return true;
}

void InstanceListLayer::createLayout()
{
	UIManager *manager = UIManager::sharedManager();

	switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		if (!instanceListLayout)
		{
			instanceListLayout = manager->CreateUILayoutFromFile("UIplist/instanceListUI.plist", this, "InstanceList");	
			manager->RegisterMenuHandler("InstanceList","closeInstanceList",
			menu_selector(InstanceListLayer::closeInstanceList), this);	
			manager->RegisterMenuHandler("InstanceList","showStarReward",
			menu_selector(InstanceListLayer::showStarReward), this);	
			
		}
		break;

	case INSTANCE_TYPE_JINGYING:
		if (!jingYingLayout)
		{
			jingYingLayout = manager->CreateUILayoutFromFile("UIplist/instanceEliteUI.plist", this, "InstanceElite");	

			manager->RegisterMenuHandler("InstanceElite","closeInstanceList",
			menu_selector(InstanceListLayer::closeInstanceList), this);	
#if 0
			manager->RegisterMenuHandler("InstanceElite","clickAutoFight",
				menu_selector(InstanceListLayer::clickAutoFight), this);
			manager->RegisterMenuHandler("InstanceElite","clickResetJingYingFuBen",
				menu_selector(InstanceListLayer::clickResetJingYingFuBen), this);
#endif
			jingYingPlayTimesText = jingYingLayout->FindChildObjectByName<UILabel>("texthint");
			jingYingPlayTimes = 0;

			int attributes[1];
			
			attributes[0] = PB_ATTR_PLAYER_SPEC_INSTANCE_BTL_TIME;
			OnlineNetworkManager::sShareInstance()->sendAttributesReq(attributes, 1);
		}
		break;

	case INSTANCE_TYPE_DUNGEON:
		if (!instanceDungeonLayout)
		{
			instanceDungeonLayout = manager->CreateUILayoutFromFile("UIplist/instanceDungeonUI.plist", this, "InstanceDungeon");	

			manager->RegisterMenuHandler("InstanceDungeon","closeInstanceList",
			menu_selector(InstanceListLayer::closeInstanceList), this);	

			manager->RegisterMenuHandler("InstanceDungeon","clickAutoFight",
				menu_selector(InstanceListLayer::clickAutoFight), this);
			manager->RegisterMenuHandler("InstanceDungeon","clickResetJingYingFuBen",
				menu_selector(InstanceListLayer::clickResetJingYingFuBen), this);
		}
		break;

	default:
		break;
	}
}

void InstanceListLayer::openInstanceList()
{
	// check bag pack full
	ItemManager::Get()->checkBagEmptyItemNum();

    if (getInstanceType() == INSTANCE_TYPE_JINGYING)
    {
        OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(JING_YING_FU_BEN_MIN, JING_YING_FU_BEN_MAX);
    } else if (getInstanceType() == INSTANCE_TYPE_DUNGEON)
    {
        OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(UNDERGROUND_INSTANCE_MIN, UNDERGROUND_INSTANCE_MAX);
    } else if (getInstanceType() == INSTANCE_TYPE_NORMAL)
    {
        OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(NORMAL_INSTANCE_MIN, NORMAL_INSTANCE_MAX);
    }

	this->createLayout();
	GameAudioManager::sharedManager()->playEffect(350001,false);
	MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"))->closeBattleCoverLayer(NULL);
	this->setVisible(true);	
	MainMenuLayer::PushOneLayer(this);
	this->setScale(0);
	CCArray *arrayOfActions = CCArray::create();

	CCActionInterval *scaleIn = CCScaleTo::create(0.1f, 1.0f); 
	CCCallFunc *fun2 = CCCallFunc::create(this, callfunc_selector(InstanceListLayer::refreshInstanceListLayer));
	
	arrayOfActions->addObject(fun2);
	arrayOfActions->addObject(scaleIn);

	CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);
	this->runAction(sequence);
	if(instanceType == INSTANCE_TYPE_NORMAL)
	{
		ShowChatBar(false);
		LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/starRewardUI.lua", "sendCheckInstanceStarRewardReq");
	}
    //test crash
   //InstanceListLayer *xxxd = NULL;
   //xxxd->runAction(sequence);

    // set mainStarRewardBtn visible false
    UILayout * layout = UIManager::sharedManager()->getUILayout("MainMenu");	
    if (!layout)
    {
        return;
    }
    UIButton * mainRewardBtn = layout->FindChildObjectByName<UIButton>("button_starReward");
    if(mainRewardBtn)
    {
        mainRewardBtn->setVisible(false);
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/starRewardUI.lua", "setIsOpenRewardBtn", false);
    }
}

void InstanceListLayer::closeInstanceList(CCObject* sender)
{
	this->setVisible(false);	
	MainMenuLayer::RemoveOneLayer(this);

	ShowChatBar(true,-1);

    if (m_instanceInfoLayer != NULL)
    {
        removeChild(m_instanceInfoLayer, true);
        setInstanceInfoLayer(NULL);
    }

    // set mainStarRewardBtn visible true or not
    UILayout * layout = UIManager::sharedManager()->getUILayout("MainMenu");	
    if (!layout)
    {
        return;
    }
    UIButton * mainRewardBtn = layout->FindChildObjectByName<UIButton>("button_starReward");
    bool hasStarReward = XLogicManager::sharedManager()->hasStarReward;
    if(mainRewardBtn && (hasStarReward == true))
    {
        mainRewardBtn->setVisible(true);
    }
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/starRewardUI.lua", "setIsOpenRewardBtn", true);
}

void InstanceListLayer::showStarReward(CCObject* sender)
{
	int size = InstanceManager::Get()->curInstanceList.size();
    int maxInstanceId = 0;
    if (size != 0)
    {
        maxInstanceId = InstanceManager::Get()->curInstanceList[size - 1].id;
    }

    LuaTinkerManager::Get()->callLuaFunc<void,int>("Script/UILayer/starRewardUI.lua", "showStarRewardUI", maxInstanceId);
	//ShowChatBar(true,-1);	
}

void InstanceListLayer::refreshInstanceListLayer()
{	
	UIScrollPage *page;
	switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		page = instanceListLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		//ShowChatBar(false);
		break;

	case INSTANCE_TYPE_JINGYING:
		page = jingYingLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	case INSTANCE_TYPE_DUNGEON:
		page = instanceDungeonLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	default:
		break;
	}
	
	//清除已有的任务标记
	for (int i = 0; i < page->getPageCount(); i++)
	{
		CCLayer * layer = page->getPageLayer(i);
		CCNode * tMark = layer->getChildByTag(TASK_MARK_TAG);
		if (tMark)
		{
			layer->removeChildByTag(TASK_MARK_TAG);
		}
	}


	if (taskInstanceId)
	{
		vector<InstanceInfo> instanceList = InstanceManager::Get()->curInstanceList;
		int count = instanceList.size();
		ScrollPagePosition position;
		for(int i = 0;i < count; i++)
		{
			if (instanceList[i].id == taskInstanceId)
			{
				position = getScrollPagePos(i);
				
				CCLayer * layer = page->getPageLayer(position.page);
				if (!layer)
				{
					break;
				}

				CCPoint pt;
				ASprite * as = AspriteManager::getInstance()->getAsprite(KICON_BIN);
				int taskMarkId = getResourceIDByName("map_ui_system_icon_FRAME_ICON_EXCLAMATORYMARK");
				CCSprite * taskMark = as->getSpriteFromFrame_Middle(taskMarkId, 0, pt);

				taskMark->setPosition(scrollPosToPoint(position));
				taskMark->setTag(TASK_MARK_TAG);
				taskMark->setVisible(true);
				

				//任务标记跳动特效
				CCPoint nowPos = taskMark->getPosition();
				CCPoint desPos = ccpAdd(nowPos, ccp(0, 10));

				CCArray *arrayOfActions = CCArray::create();
			
				CCMoveTo * move1 = CCMoveTo::create(0.3f, desPos);
				CCMoveTo * move2 = CCMoveTo::create(0.3f, nowPos);			

				arrayOfActions->addObject(move1);
				arrayOfActions->addObject(move2);

				CCFiniteTimeAction *sequence = CCSequence::create(arrayOfActions);

				CCRepeatForever * repeat = CCRepeatForever::create(static_cast<CCSequence *>(sequence));
				taskMark->runAction(repeat);

				layer->addChild(taskMark);


				page->moveToPage(position.page, false);
				break;
			}	
		}		
	}
	else
	{
		page->moveToPage(page->getPageCount() - 1, false);
	}	
}

void InstanceListLayer::addInstancesFromList(const std::vector<InstanceInfo>& instanceList)
{
	UIScrollPage *page;
	
	//创建layout
	this->createLayout();
	switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		page = instanceListLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	case INSTANCE_TYPE_JINGYING:
		page = jingYingLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
        this->showCoinAndDiamond(jingYingLayout);
		break;

	case INSTANCE_TYPE_DUNGEON:
		page = instanceDungeonLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
        this->showCoinAndDiamond(instanceDungeonLayout);
		break;

	default:
		break;
	}

	//删除所有页面并重建
	page->removeAllPages();	
	int count = instanceList.size();
	ScrollPagePosition position;

	for(int i = 0;i < count; i++)
	{
		position = getScrollPagePos(i);
		addSigleInstance(instanceList[i], position);		
	}	


	refreshInstanceListLayer();
     if(page_reset >  0){
        page->moveToPage(page_reset, false);
        page_reset = -1;
    }
}

void InstanceListLayer::addSigleInstance(InstanceInfo info, ScrollPagePosition position)
{
	UIScrollPage *page;
	
	switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		page = instanceListLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		//check if we should show star reward button
		if(info.id == 200104 && info.star > 0)
		{
			//user already play the 4th instance show the star button
			CCLOG("show star button");
			UIButton *starBtn =
			instanceListLayout->FindChildObjectByName<UIButton>("BUTTON_SHOW_STAR_REWARD");
			starBtn->setVisible(true);
		}
		break;

	case INSTANCE_TYPE_JINGYING:
		page = jingYingLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	case INSTANCE_TYPE_DUNGEON:
		page = instanceDungeonLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	default:
		break;
	}

	//创建足够的页数
	while(position.page >= page->getPageCount())
	{
		CCLayer* layer = CCLayer::create();
		layer->setAnchorPoint(ccp(0, 0));		
		page->addPage(layer);
	}
	CCLayer * layer = page->getPageLayer(position.page);
	ASSERT(layer != NULL, "InstanceListLayer : addSigleInstance : null layer");

	CCNode * insNode = layer->getChildByTag(position.index);
	ASSERT(insNode == NULL, "InstanceListLayer : addSigleInstance : the instance already added!");

	//读取资源
	ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);

	std::string instanceConfigName;
	const char *pConfigName = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "Icon");
	if(pConfigName)
	{
		instanceConfigName = pConfigName;
	}	

	std::transform(instanceConfigName.begin(), instanceConfigName.end(), instanceConfigName.begin(), towupper);
	instanceConfigName = std::string("map_ui_system_icon_FRAME_") + instanceConfigName;

	int normalBtnId, selectBtnId, isStarId, notStarId;
	switch (instanceType)
	{
	case INSTANCE_TYPE_NORMAL:
		normalBtnId = getResourceIDByName("map_ui_FRAME_INSTANCE_GRID1");
		selectBtnId = getResourceIDByName("map_ui_FRAME_INSTANCE_GRID1_CLICKED");
		isStarId = getResourceIDByName("map_ui_FRAME_INSTANCE_STAR");
		notStarId = getResourceIDByName("map_ui_FRAME_INSTANCE_STAR_BLANK");
		break;

	case INSTANCE_TYPE_JINGYING:
		normalBtnId = getResourceIDByName("map_ui_FRAME_ELITEINSTANCE_GRID1");
		selectBtnId = getResourceIDByName("map_ui_FRAME_ELITEINSTANCE_GRID1_CLICKED");
		isStarId = getResourceIDByName("map_ui_FRAME_ELITEINSTANCE_STAR");
		notStarId = getResourceIDByName("map_ui_FRAME_ELITEINSTANCE_STAR_BLANK");
		break;

	case INSTANCE_TYPE_DUNGEON:
		normalBtnId = getResourceIDByName("map_ui_FRAME_UNDERGROUND_GRID1");
		selectBtnId = getResourceIDByName("map_ui_FRAME_UNDERGROUND_GRID1_CLICKED");
		isStarId = getResourceIDByName("map_ui_FRAME_UNDERGROUND_STAR");
		notStarId = getResourceIDByName("map_ui_FRAME_UNDERGROUND_STAR_BLANK");
		break;

	default:
		break;
	}
	
	int instanceIconId = getResourceIDByName(instanceConfigName.c_str());
	
	CCPoint pt;
	CCSprite *pNormalBtn = as->getSpriteFromFrame_Middle(normalBtnId, 0, pt);
	CCSprite *pSelectBtn = as->getSpriteFromFrame_Middle(selectBtnId ,0,pt);
    
	//创建副本按钮
	IconButton * pBtn;

	pBtn = new IconButton(pNormalBtn, NULL, pSelectBtn, this, menu_selector(InstanceListLayer::onPressInstanceBtn));
	
	char buffer[100];
	sprintf(buffer,"instanceListScrollPage_%d",(position.index+1));
	pBtn->SetButtonName(buffer);
	pBtn->autorelease();		
	pBtn->setContentSize(pNormalBtn->getContentSize());
	pBtn->setPosition(ccp(0,0));
	pBtn->SetButtonIndex(position.index);
    
	//创建副本名称标签
	float fontSize = GameFontManager::smallFontSize();
	CCSize nameLabelSize = CCSizeMake(200 / CC_CONTENT_SCALE_FACTOR(), fontSize * 2.0f);

	const char * instanceText = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "RaidName");	
	const char * pInstacneName = Localizatioin::getLocalization(instanceText);
	if(pInstacneName == NULL)
		return;
//get chapter id ,enter time for debug
   // const char* chapterid = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "Chapter");
   //char buff[150];
   //snprintf(buff, 150, "%s:%s:%d", pInstacneName,chapterid, info.refresh_times);
	UILabelTTF * instanceLabel = UILabelTTF::create(pInstacneName, KJLinXin, fontSize * 1.5f,
				nameLabelSize, kCCTextAlignmentCenter,kCCVerticalTextAlignmentBottom);
    
	instanceLabel->setPosition(ccp(10, -50));

	//创建副本图标
	ASprite *as1 = AspriteManager::getInstance()->getAsprite(KICON_BIN);
    
	CCSprite *pIcon = as1->getSpriteFromFrame_Middle(instanceIconId, 0, pt);
    if(!info.can_enter_flag && instanceType != INSTANCE_TYPE_NORMAL)
    {
        pIcon = graylightWithCCSprite(pIcon, false);     
        info.isgray = true;
    }

    if(pIcon){
		pIcon->setPosition(ccp(0, 0));
        pIcon->setTag(BUTTON_TAG);
	}

	//创建副本星星，位置偏移分别为绕圆周上偏左30度，正上，上偏右30度
	CCSprite * star1 = as->getSpriteFromFrame_Middle(info.star>=2 ? isStarId : notStarId ,0,pt);
	CCSprite * star2 = as->getSpriteFromFrame_Middle(info.star>=3 ? isStarId : notStarId ,0,pt);
	CCSprite * star3 = as->getSpriteFromFrame_Middle(info.star>=4 ? isStarId : notStarId ,0,pt);

	CCPoint offsetStar1 = ccp(-45,77.9);
	CCPoint offsetStar2 = ccp(0,90);
	CCPoint offsetStar3 = ccp(45,77.9);
	if (instanceType == INSTANCE_TYPE_DUNGEON)
	{
		float offsetFactor = 0.7f;
		//地下城副本星星位置更靠内
		offsetStar1 = ccp(-65 * offsetFactor,77.9 * offsetFactor);
		offsetStar2 = ccp(0, 90 * offsetFactor);
		offsetStar3 = ccp( 65 * offsetFactor,77.9 * offsetFactor);
	}	

	star1->setPosition(offsetStar1);
	star2->setPosition(offsetStar2);
	star3->setPosition(offsetStar3);
    
	//创建副本结点
	CCNode * instanceNode = CCNode::create();

	instanceNode->setTag(INSTANCE_NODE_TAG + position.index);	
	instanceNode->setPosition(scrollPosToPoint(position));
	if(pIcon)
	{
		instanceNode->addChild(pIcon);
	}	
    
    instanceNode->addChild(pBtn);	
    
	instanceNode->addChild(instanceLabel, 1000);	

	instanceNode->addChild(star1, 100);
	instanceNode->addChild(star2, 100);
	instanceNode->addChild(star3, 100);

	layer->addChild(instanceNode);	
}

void InstanceListLayer::onExit()
{
    closeInstanceInfoLayer();
    CCLayer::onExit();
}

void InstanceListLayer::showInstanceInfo(CCNode* parent,unsigned int instanceId,unsigned int instanceType,CCPoint pt)
{
	InstanceInfo* info = InstanceManager::Get()->getOneInstance(instanceId);
	if(info->id == 0)
	{
		return;
	}

	INSTANCE_TYPE type = INSTANCE_TYPE_NORMAL;
	if(instanceType == 5)
	{
		type = INSTANCE_TYPE_JINGYING;
	}

	/*InstanceListLayer* listLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"),false);
	if(listLayer)
	{
		listLayer->OpenInstanceInfoLayer(info,type,0,false);
	}
	else*/
	{
		CCNodeLoader* loader = new NormalInstanceInfoLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("normal_instance", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();
		CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/normal_instance");
        dynamic_cast<NormalInstanceInfoLayer*>(node)->InitLayer(*info, type, 0, false, false);
        //setInstanceInfoLayer(dynamic_cast<CCLayer*>(node));
        //CCSize s = this->getContentSize();
        node->setPosition(pt);
        float scale =  TXGUI::UIManager::sharedManager()->getScaleFactor();
        node->setScale(scale);
        parent->addChild(node);
	}
}

void InstanceListLayer::OpenInstanceInfoLayer(InstanceInfo info, INSTANCE_TYPE type, int jingYingPlayTimes, bool tutorials)
{
    if (m_instanceInfoLayer == NULL)
    {
        CCNodeLoader* loader = new NormalInstanceInfoLayerLoader();
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("normal_instance", loader);

        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();

        CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/normal_instance");
        dynamic_cast<NormalInstanceInfoLayer*>(node)->InitLayer(info, type, jingYingPlayTimes, tutorials);
        setInstanceInfoLayer(dynamic_cast<CCLayer*>(node));
        CCSize s = this->getContentSize();
        node->setPosition(ccp(s.width / 2, s.height / 2));
        float scale =  TXGUI::UIManager::sharedManager()->getScaleFactor();
        node->setScale(scale);
        this->addChild(node);
    } else
    {
        dynamic_cast<NormalInstanceInfoLayer*>(m_instanceInfoLayer)->InitLayer(info, type, jingYingPlayTimes);
    }
}

void InstanceListLayer::closeInstanceInfoLayer()
{
    if (m_instanceInfoLayer)
    {
        this->removeChild(m_instanceInfoLayer);
    }

    setInstanceInfoLayer(NULL);
}

void InstanceListLayer::onPressInstanceBtn(CCObject *sender)
{
    bool flag = TutorialsManager::Get()->IsInTutorial();

    CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
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
                //closeInstanceList(NULL);
                int buttonIndex = button->GetButtonIndex();
                InstanceInfo Dailyinfo =  InstanceManager::Get()->curInstanceList[buttonIndex];

                char buffer[100];
                sprintf(buffer,"instanceListScrollPage_%d",(buttonIndex+1));
                TutorialsManager::Get()->HandleOneEvent(buffer,1);

                OpenInstanceInfoLayer(Dailyinfo, instanceType, jingYingPlayTimes, flag);
                return;
            }
        }
    }
// 
// 	CCLOG("InstanceListLayer: onPressInstanceBtn");
// 	
// 	if(instanceType == INSTANCE_TYPE_JINGYING &&
// 		jingYingPlayTimes <= 0) {
// 		ShowMessage(Localizatioin::getLocalization("M_PVAI_CHELLENGTIMES_OVER"));
//         return;
// 	}
// 	
// 	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
// 	if(menuItem)
// 	{
// 		CCNode* node = menuItem->getParent();
// 		CCMenu* menu = dynamic_cast<CCMenu*>(node);
// 		if(menu)
// 		{
// 			CCNode* parent = menu->getParent();
// 			IconButton* button = dynamic_cast<IconButton*>(parent);
// 			if(button)
// 			{
// 				//closeInstanceList(NULL);
// 				int buttonIndex = button->GetButtonIndex();
// 
// 				char buffer[100];
// 				sprintf(buffer,"instanceListScrollPage_%d",(buttonIndex+1));
// 				TutorialsManager::Get()->HandleOneEvent(buffer,1);
// 				InstanceInfo info = InstanceManager::Get()->curInstanceList[buttonIndex];
//                 instanceId = info.id;
//                 int star = info.star;
// 
// 				USERINFO userInfo = UserData::GetUserInfo();
//                 int staming = userInfo.m_stamina;
//                     
//                 if(staming < 5)
// 				{
// 					ShowMessage(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"));
//                     return;
// 				}
// 				
// 
// 				//CCLOG("length of jing ying daily info is %d", InstanceManager::Get()->instanceList.size());
// 				//int listsize = InstanceManager::Get()->instanceList.size();
// //               if(buttonIndex > listsize) {
// //                   CCLOG("failed to get the daily info of jing ling fu ben");
// //                   //did not get the daily info from server. 
// //                   //wait.. to get
// //                   return;
// //               }
// 				InstanceInfo Dailyinfo =  InstanceManager::Get()->curInstanceList[buttonIndex];
// 
// 				switch (instanceType)
// 				{
// 				case INSTANCE_TYPE_NORMAL:
// 				case INSTANCE_TYPE_JINGYING:	
// 					if (star > 1)
// 					{
// 						char buf[10];
// 						sprintf(buf, "%d", staming);
//                     
// 						std::string instanceConfigName = Localizatioin::getLocalization("M_AUTOFIGHT_HINT");
// 						const char* msg1 = Localizatioin::getLocalization("M_AUTOFIGHT_FIGHT");
// 						const char* msg2 = Localizatioin::getLocalization("M_AUTOFIGHT_AUTOFIGHT");
//                     
// 						instanceConfigName += " ";
// 						instanceConfigName += buf;
// 						MessageBox::Show(instanceConfigName.c_str(),this,menu_selector(InstanceListLayer::OnFight), 
// 							menu_selector(InstanceListLayer::OnAutoOnHook),MB_OKCANCELCLOSE);
// 						MessageBox::AdjustStyle(msg1, msg2);
// 						MessageBox::showCloseBtn();
// 					}
// 					else
// 					{
// 						Fight(info.id);
// 					}
// 					break;
// #if 0
// 
// 				case INSTANCE_TYPE_JINGYING:				
// 					if(Dailyinfo.can_enter_flag== false)
// 					{
// 		                ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISH_THIS_INSTANCE"));
// 					}
// 					else 
// 					{
// 						Fight(info.id);
// 					}
// 					break;
// #endif
// 
// 				case INSTANCE_TYPE_DUNGEON:
// 					if(Dailyinfo.can_enter_flag== false)
// 					{
// 		                ShowMessage(Localizatioin::getLocalization("M_UNDERGROUND_ALREADYFINISH"));
// 					}
// 					else 
// 					{
// 						Fight(info.id);
// 					}
// 					break;
// 					
// 
// 				default:
// 					break;
// 				}
// 			}
// 		}
// 	}
}

void InstanceListLayer::onEventMsgInstanceList(const std::vector<InstanceInfo>& instanceInfos)
{
    showInstance = false;
	this->addInstancesFromList(InstanceManager::Get()->curInstanceList);
    updateIconColor();
	
}

ScrollPagePosition InstanceListLayer::getScrollPagePos(int index)
{
	//int row = instanceType !=  INSTANCE_TYPE_JINGYING ? 2 : 1;
	int row = 2;
	ScrollPagePosition spPos;
	spPos.index = index;
	spPos.page = index / (row * MAX_COL);
	spPos.row  = index % (row * MAX_COL) / MAX_COL;
	spPos.col  = index % (row * MAX_COL) % MAX_COL;

	return spPos;
}

CCPoint InstanceListLayer::scrollPosToPoint(ScrollPagePosition position)
{	
	//横竖坐标间距的一半
	float halfH = 120.0f;
	float halfV = 100.0f;

	//横竖坐标偏移，左下角为原点
	float offsetH = 0.0f;
	float offsetV = -35.0f;

	if (instanceType == INSTANCE_TYPE_DUNGEON)
	{
		halfV = 80;
		offsetV = 20;
	}
	
	float HDelta, VDelta;
	HDelta =   halfH * (2 * position.col + 1 - MAX_COL) + offsetH;
	VDelta = - halfV * (2 * position.row + 1 - MAX_ROW) + offsetV;

	return ccp(HDelta, VDelta);
}



void InstanceListLayer::closeLayerCallBack( void )
{
	this->setVisible(false);	
}

void InstanceListLayer::OnAutoOnHook(CCObject* sender)
{
#if 0
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
        closeInstanceInfoLayer();
        mLayer->ShowAutoOnHook(true);
        GetAutoOnHook()->setInstanceType(this->instanceId, this->instanceType);
        GetAutoOnHook()->sendCSABMonsterReq(instanceId);
	}
#endif
}

void InstanceListLayer::OnFight(CCObject* sender)
{
    Fight(instanceId);
}

void InstanceListLayer::Fight(int id)
{
    InstanceManager::Get()->setCurInstanceId(id);
    int instanceId = InstanceManager::Get()->getCurInstanceId();
    
    XLogicManager::sharedManager()->prepareBattle(instanceId,0);
    
    //ResoureUpdate::sharedResoureUpdate()->realTimeUpdate(id);
}


///////////////////////////////////////////////////////////////
//functions only used for jing ying fu ben 
//////////////////////////////////////////////////////////////

void InstanceListLayer::clickAutoFight(CCObject *sender)
{
	CCLOG("%s %d auto fight", __FUNCTION__, __LINE__);
	//ShowMessage("Coming Soon ...");
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
	    //int cId = getCurChapterId();
        std::map<int, ChapterInfo>* values = InstanceManager::Get()->getAllchapterName(instanceType);
        if(values->size() == 0)
        {
               CCLOG("no chaper can auto fight");
               ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISHALL"));
               
               return;
        }
        std::map<int, ChapterInfo>::iterator it = values->begin();
        bool all_disable = true;
        for(; it != values->end(); ++it)
        {
            if(it->second.active)
            {
                all_disable = false;
                break;
            }
        }
        if(all_disable)
        {
               ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_FINISHALL"));
               return;
        }
        GetAutoOnHook()->setInstanceType(this->instanceId, this->instanceType);
        GetAutoOnHook()->setchapterList(values);
	}
}

int InstanceListLayer::getResetTimes(int chapterId) 
{
    	InstanceInfo *info = getCurInstanceWithBiggestRefrechTimes(chapterId);
        if(info == NULL)
        {
            return -1;
        }
	    USERINFO userinfo = UserData::GetUserInfo();
        int vip = userinfo.m_vip;
        int resettime;
        switch (instanceType)
        {
        case INSTANCE_TYPE_JINGYING:
        {
            resettime = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", vip, "Elite");           
            break;
        }
		case INSTANCE_TYPE_DUNGEON:
        {
            resettime = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", vip, "Underground");           
            break;
        }
        default:
            break;
        }
        CCLOG("reset time is %d", resettime);
        return resettime - info->refresh_times;
}
void InstanceListLayer::clickResetJingYingFuBen(CCObject *sender)
{    

	unsigned  int chapId = getCurChapterId();
    int left_time = getResetTimes(chapId);
    InstanceInfo *info = getCurInstanceWithBiggestRefrechTimes(chapId);
    if(info == NULL)
    {
       return;
    }
    int baseid=0;
    switch (instanceType)
    {
    case INSTANCE_TYPE_JINGYING:
    {
        baseid = 20000;           
        break;
    }
	case INSTANCE_TYPE_DUNGEON:
    {
        baseid = 20010;           
        break;
    }
    default:
        break;
    }

    
    if(left_time <= 0)
    {
         ShowMessage(Localizatioin::getLocalization("M_ELITEINSTANCE_NORESET_TIME"));
    }
    else
    { //we can reset the game. 
		std::string strResetGameConsume = Localizatioin::getLocalization("M_RESET_ELITEINSTANCE_QUESTION");
   		std::string strResetGameTime = Localizatioin::getLocalization("M_RESET_ELITEINSTANCE_TIME");

		char buf[10];
        int num = LuaTinkerManager::Get()->callLuaFunc<int>("Script/GameConfig/ExchangeParameterControl.lua", 
    "getExchangeParameterCostNum",baseid+info->refresh_times);
	    sprintf(buf, "%d", num);
	    strResetGameConsume += " ";
	    strResetGameConsume += buf;
        strResetGameConsume += "\n"+strResetGameTime;
       
      
        sprintf(buf, " %d", left_time);
        strResetGameConsume += buf;
	    MessageBox::Show(strResetGameConsume.c_str(),this,menu_selector(InstanceListLayer::resetJingYingList),
			NULL, MB_OKCANCEL);
	    
	  	MessageBox::showCloseBtn();

	}
}

//get current chapter with biggist refrech time
InstanceInfo * InstanceListLayer::getCurInstanceWithBiggestRefrechTimes(int chapId)
{
	int count = InstanceManager::Get()->curInstanceList.size();
	unsigned int max = 0;
    int index = -1;
	for(int i = 0; i < count; i++)
	{

		 InstanceInfo info =  (InstanceManager::Get()->curInstanceList[i]);
		 int chapter = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", info.id, "Chapter");
        if(chapter == chapId)
		{			 
			if(info.refresh_times >= max)
            {
            max = info.refresh_times;
            index = i;
            }			
		}
	}
    CCLOG("max refresh time is %d", max);
    if(index > -1)
        return &(InstanceManager::Get()->curInstanceList[index]);
    return NULL;
}

int InstanceListLayer::getCurChapterId()
{
	UIScrollPage *page = NULL;
	switch (instanceType)
	{
	case INSTANCE_TYPE_JINGYING:
		page = jingYingLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	case INSTANCE_TYPE_DUNGEON:
		page = instanceDungeonLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	default:
		break;
	}

	int curPage = page->getCurPage();
    
	int count = InstanceManager::Get()->curInstanceList.size();
	unsigned int chapId;
	
	for(int i = 0; i < count; i++)
	{
	    ScrollPagePosition position = getScrollPagePos(i);
		//FIX ME... now we only get the first instance chapter id of current page
		//then send to server. need todo: show all bu ben of one charpter in one page
		if(position.page == curPage)
		{
		    
			InstanceInfo info = InstanceManager::Get()->curInstanceList[i];
			
	        int chapter = LuaTinkerManager::Get()->getLuaConfig<int>("InstanceList", "instances", info.id, "Chapter");
            chapId=chapter;
			break;
		}
	}
    return chapId;
}

//when pressed the reset button , call this function
void InstanceListLayer::resetJingYingList(CCObject *sender)
{
    unsigned  int chapId = getCurChapterId();
	InstanceInfo *info = getCurInstanceWithBiggestRefrechTimes(chapId);
    UIScrollPage *page = NULL;
	if(info == NULL)
		return;
    // we reset chapter. we need send the reset times to server.use   getCurInstanceWithBiggestRefrechTimes
    //to get the current reset time of current chapter
	switch (instanceType)
	{
	case INSTANCE_TYPE_JINGYING:
     	page = jingYingLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");

		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/InstanceListLayer.lua", "sendExchangeParameterReq",
											20000+info->refresh_times,chapId,PB_INSTANCE_TYPE_PVE2);
		break;

	case INSTANCE_TYPE_DUNGEON:
     	page = instanceDungeonLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");

		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/InstanceListLayer.lua", "sendExchangeParameterReq",
											20010+info->refresh_times,chapId,PB_INSTANCE_TYPE_PVE3);
		break;

	default:
		break;
	}
    

	page_reset = page->getCurPage();
}


void InstanceListLayer::onExchangeParameterRsp(bool suc)
{    
	if(suc) 
	{
	    //success to reset 
		const char* msg = Localizatioin::getLocalization("M_ELITEINSTANCE_RESETSUCCESS");
		ShowMessage(msg);	

		switch (instanceType)
		{
		case INSTANCE_TYPE_JINGYING:
			showCoinAndDiamond(jingYingLayout);

			OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(JING_YING_FU_BEN_MIN, JING_YING_FU_BEN_MAX);
			break;

		case INSTANCE_TYPE_DUNGEON:
			showCoinAndDiamond(instanceDungeonLayout);
			OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(UNDERGROUND_INSTANCE_MIN, UNDERGROUND_INSTANCE_MAX);
			break;

		default:
			break;
		}
        
    } 
 
}

void InstanceListLayer::updateIconColor()
{    
	if (instanceType != INSTANCE_TYPE_JINGYING && instanceType != INSTANCE_TYPE_DUNGEON)
	{
		return;
	}

    UIScrollPage *page = NULL;
    // showInstance=true; //set this.when the back msg come, the onEventMsgInstanceList called
	switch (instanceType)
	{
	case INSTANCE_TYPE_JINGYING:
        
		page = jingYingLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	case INSTANCE_TYPE_DUNGEON:
		page = instanceDungeonLayout->FindChildObjectByName<UIScrollPage>("instanceListScrollPage");
		break;

	default:
		break;
	}	
	
    //if enable , chagned color to gray of the icon 
    int pageCount = page->getPageCount();
    int changepage=-1;
    for(int i = 0; i < pageCount; i++)
    {
		CCLayer* layer =  page->getPageLayer(i);
        
		if(layer)
		{
			CCArray* itemArray = layer->getChildren();
			CCObject* itemObj;
			CCARRAY_FOREACH(itemArray,itemObj)
			{
			    //GET one button
				CCNode* Button = dynamic_cast<CCNode*>(itemObj);
                if(Button->getTag() == TASK_MARK_TAG)
                    continue;
                int index = Button->getTag()-INSTANCE_NODE_TAG;
                
				if(index > InstanceManager::Get()->curInstanceList.size())
                    continue;
				//get the icon sprite
				CCSprite* icon =  dynamic_cast<CCSprite*>(Button->getChildByTag(BUTTON_TAG));
                InstanceInfo &modelinfo = InstanceManager::Get()->curInstanceList[index];
        		bool changeToGray = !modelinfo.can_enter_flag;
                int zorder = icon->getZOrder();
                if(changeToGray)
                {
                    //change to gray    
			        //get the icon button
			        if(modelinfo.isgray == true)
                       return;
                    modelinfo.isgray = true;
			        CCSprite *newsprite = graylightWithCCSprite(icon, false);
                    if(newsprite){
                		newsprite->setPosition(ccp(0, 0));
                        newsprite->setTag(BUTTON_TAG);
                        
                        Button->addChild(newsprite);
                        newsprite->setZOrder(zorder);
                        icon->removeFromParentAndCleanup(true);
                	}
                } 
                else 
                {
                    //change from gray to normal image
                    if(modelinfo.isgray == false)
                       return;
                    modelinfo.isgray = false;
                    changepage = i;
                    InstanceInfo info = InstanceManager::Get()->curInstanceList[index];
                    std::string instanceConfigName;
                	const char *pConfigName = LuaTinkerManager::Get()->getLuaConfig<const char *>("InstanceList", "instances", info.id, "Icon");
                	if(pConfigName)
                		instanceConfigName = pConfigName;

                	std::transform(instanceConfigName.begin(), instanceConfigName.end(), instanceConfigName.begin(), towupper);
                	instanceConfigName = std::string("map_ui_system_icon_FRAME_") + instanceConfigName;
	                int instanceIconId = getResourceIDByName(instanceConfigName.c_str());
                    ASprite *as1 = AspriteManager::getInstance()->getAsprite(KICON_BIN);
					CCPoint pt;
                    CCSprite *newsprite = as1->getSpriteFromFrame_Middle(instanceIconId, 0, pt);
                	if(newsprite){
                		newsprite->setPosition(ccp(0, 0));
                        newsprite->setTag(BUTTON_TAG);
                        icon->removeFromParentAndCleanup(true);
                        Button->addChild(newsprite, zorder);
                	}
                }
			}
		}
	
    }
	if(changepage >= 0)
        page->moveToPage(changepage, false);
	
}

void InstanceListLayer::showCoinAndDiamond(UILayout * layout)
{
    UILabel* diamond_label = layout->FindChildObjectByName<UILabel>("bottomDiamondLable");
    UILabel* coin_label = layout->FindChildObjectByName<UILabel>("bottomCoinLable");

    USERINFO info =  UserData::GetUserInfo();
    
    int coinnum = info.m_gold;
    int diamandnum = info.m_diamond;
  
    const char* str = MainMenuLayer::GetNumByFormat(coinnum);
    if(str)
    coin_label->setString(str);

	char buffer[100];
	sprintf(buffer,"%d",diamandnum);
    //str = MainMenuLayer::GetNumByFormat(diamandnum);
    diamond_label->setString(buffer);
}

void InstanceListLayer::showJingYingPlayTimes(int haveplaytime){
	if(instanceType != INSTANCE_TYPE_JINGYING)
		return;
	jingYingPlayTimes = 3-haveplaytime;
	const char * str = LuaTinkerManager::Get()->callLuaFunc<const char *, int>("Script/UILayer/InstanceListLayer.lua", 
		"showJingYingPlayTimes", haveplaytime);
	jingYingPlayTimesText->setString(str);
}

NormalInstanceInfoLayer::NormalInstanceInfoLayer() :
    m_timesLabel(NULL),
    m_powerLabel(NULL),
    m_bonusCash(NULL),
    m_bonusHun(NULL),
    m_bonusLayer1(NULL),
    m_bonusLayer2(NULL),
    m_bonusLayer3(NULL),
    m_bonusLayer4(NULL),
    m_closeBtn(NULL),
    m_autoBtn(NULL),
    m_enterBtn(NULL),
    m_instanceType(INSTANCE_TYPE_NOTHING),
    m_jingYingPlayTimes(0),
    m_currentCombat(NULL),
    m_desc1(NULL),
    m_desc2(NULL),
	m_desc3(NULL),
	m_desc4(NULL),
    m_uiLayer(NULL),
	m_labelLife(NULL),
	m_labelCost(NULL),
	m_isFromInstancePanel(true)
{
    m_vtStars.clear();
    m_vtStars.resize(3);

    m_vtLayers.clear();
    m_vtLayers.resize(12);
}

NormalInstanceInfoLayer::~NormalInstanceInfoLayer()
{
    setTimesLabel(NULL);
    setPowerLabel(NULL);
    setBonusCash(NULL);
    setBonusHun(NULL);
    setBonusLayer1(NULL);
    setBonusLayer2(NULL);
    setBonusLayer3(NULL);
    setBonusLayer4(NULL);
    setCloseBtn(NULL);
    setAutoBtn(NULL);
    setEnterBtn(NULL);
    setCurrentCombat(NULL);
    setDesc1(NULL);
    setDesc2(NULL);
    setDesc3(NULL);
	setLabelLife(NULL);
	setLabelCost(NULL);

	setIsFromInstancePanel(true);

    m_vtStars.clear();

	NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);

    UIManager::sharedManager()->RemoveLayout("instance_info");
    delete m_uiLayer;

}

SEL_MenuHandler NormalInstanceInfoLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler NormalInstanceInfoLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", NormalInstanceInfoLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "enter", NormalInstanceInfoLayer::onEnterInstance);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "auto",  NormalInstanceInfoLayer::onSelectAuto);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_1", NormalInstanceInfoLayer::onSelectItem1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_2", NormalInstanceInfoLayer::onSelectItem2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_3", NormalInstanceInfoLayer::onSelectItem3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_4", NormalInstanceInfoLayer::onSelectItem4);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_5", NormalInstanceInfoLayer::onSelectItem5);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_6", NormalInstanceInfoLayer::onSelectItem6);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_7", NormalInstanceInfoLayer::onSelectItem7);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_8", NormalInstanceInfoLayer::onSelectItem8);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_9", NormalInstanceInfoLayer::onSelectItem9);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_10", NormalInstanceInfoLayer::onSelectItem10);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_11", NormalInstanceInfoLayer::onSelectItem11);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "select_12", NormalInstanceInfoLayer::onSelectItem12);
    return NULL;
}

bool NormalInstanceInfoLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "times",      CCLabelTTF*, m_timesLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "power",      CCLabelTTF*, m_powerLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_cash", CCLabelTTF*, m_bonusCash);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bonus_hun",  CCLabelTTF*, m_bonusHun);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "current_combat", CCLabelTTF*, m_currentCombat);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item1",      CCLayer*,    m_vtLayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item2",      CCLayer*,    m_vtLayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item3",      CCLayer*,    m_vtLayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item4",      CCLayer*,    m_vtLayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item5",      CCLayer*,    m_vtLayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item6",      CCLayer*,    m_vtLayers[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item7",      CCLayer*,    m_vtLayers[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item8",      CCLayer*,    m_vtLayers[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item9",      CCLayer*,    m_vtLayers[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item10",     CCLayer*,    m_vtLayers[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item11",     CCLayer*,    m_vtLayers[10]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "item12",     CCLayer*,    m_vtLayers[11]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "desc1",      CCLabelTTF*, m_desc1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "desc2",      CCLabelTTF*, m_desc2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "desc3",      CCLabelTTF*, m_desc3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "desc4",      CCLabelTTF*, m_desc4);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",  CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "enter_btn",  CCControlButton*, m_enterBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "auto_btn",   CCControlButton*, m_autoBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star1", CCSprite*, m_vtStars[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star2", CCSprite*, m_vtStars[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star3", CCSprite*, m_vtStars[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "life", CCLabelTTF*, m_labelLife);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost", CCLabelTTF*, m_labelCost);

    return false;
}

void NormalInstanceInfoLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    setTouchEnabled(true);
    m_uiLayer = new NormalInstanceInfoUILayer(this);
    m_uiLayer->setInfoLayer(this);
    UIManager::sharedManager()->AddLayout(m_uiLayer, "instance_info");

    m_desc3->setString(Localizatioin::getLocalization("M_REMAIN_STAMINA"));

    m_autoBtn->setTitleForState(CCString::createWithFormat(Localizatioin::getLocalization("M_AUTOFIGHT_AUTOFIGHT")), CCControlStateNormal);
    m_desc1->setString(Localizatioin::getLocalization("M_AUTOFIGHT_HINT2"));
	m_desc2->setString(Localizatioin::getLocalization("M_AUTOFIGHT_REMAINTIME"));
	m_desc4->setString(Localizatioin::getLocalization("M_ONE_CONSUME_POWER"));
	
    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);
}

void NormalInstanceInfoLayer::onCloseLayer(CCObject* sender, CCControlEvent event)
{
    InstanceListLayer* parentLayer = dynamic_cast<InstanceListLayer*>(getParent());
    if (parentLayer == NULL)
    {
		this->removeFromParentAndCleanup(true);
    }
	else
	{
		parentLayer->closeInstanceInfoLayer();
	}

    
}

void NormalInstanceInfoLayer::enterBagLayer(CCObject* sender)
{
    InstanceListLayer* layer = dynamic_cast<InstanceListLayer*>(getParent());
    if (layer)
    {
        layer->closeInstanceInfoLayer();
        layer->setVisible(false);
    }
	else
	{
		this->removeFromParentAndCleanup(true);
	}

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        int tag = mLayer->getLayerTagByName("BackPackLayer");
        mLayer->showUILayerByTag(tag,true);
    }
}
void NormalInstanceInfoLayer::enterMallChipPage(CCObject* sender)
{
    InstanceListLayer* layer = dynamic_cast<InstanceListLayer*>(getParent());
    if (layer)
    {
        layer->closeInstanceInfoLayer();
        layer->setVisible(false);
    }
	else
	{
		this->removeFromParentAndCleanup(true);
	}
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(true,1);
        ShowChatBar(true);
    }
}

void NormalInstanceInfoLayer::enterBuyStaminaPage(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "onStaminaClicked");
}

void NormalInstanceInfoLayer::onEnterInstance(CCObject* sender, CCControlEvent event)
{
    CCLOG("InstanceListLayer: onPressInstanceBtn");
	
    InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
    GameConfigFromLuaManager::Get()->getInstanceBonusInfo(m_instanceInfo.id, bonusInfo);

    // 检查体力
    USERINFO userInfo = UserData::GetUserInfo();
    int staming = userInfo.m_stamina;

	if(staming < bonusInfo->power && m_instanceType != INSTANCE_TYPE_DUNGEON)
    {
       // ShowMessage(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"));
        InterfaceManager::Get()->ShowNOEnoughStaminaBox(this, menu_selector(NormalInstanceInfoLayer::enterBuyStaminaPage));
        return;
    }

    if (bonusInfo->max_number <= m_instanceInfo.battle_times && m_instanceType == INSTANCE_TYPE_NORMAL)
    {
        // 弹出确认提示框
        ShowConfirmBox();
        return;
    }

	//update by arthurwang 2014/9/22
	//精英副本修改成独立可战斗次数
	//if (m_instanceType == INSTANCE_TYPE_JINGYING && m_jingYingPlayTimes <= 0) 
	if (bonusInfo->max_number <= m_instanceInfo.battle_times && m_instanceType == INSTANCE_TYPE_JINGYING)
    {
        ShowJYConfirmBox();
		//ShowMessage(Localizatioin::getLocalization("M_PVAI_CHELLENGTIMES_OVER"));
        return;
    }

    if(m_instanceInfo.can_enter_flag== false && m_instanceType == INSTANCE_TYPE_DUNGEON)
    {
        ShowMessage(Localizatioin::getLocalization("M_UNDERGROUND_ALREADYFINISH"));
        return;
    }

    InstanceManager::Get()->setCurInstanceId(m_instanceInfo.id);
    XLogicManager::sharedManager()->prepareBattle(m_instanceInfo.id, 0);

}

void NormalInstanceInfoLayer::onSelectAuto(CCObject* sender, CCControlEvent event)
{
    InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
    GameConfigFromLuaManager::Get()->getInstanceBonusInfo(m_instanceInfo.id, bonusInfo);
    if (m_instanceInfo.star <= 1)
    {
        ShowMessage(Localizatioin::getLocalization("M_CANNOTHANGUP"));
        return;
    }

    if (m_instanceType == INSTANCE_TYPE_DUNGEON)
    {
        ShowMessage(Localizatioin::getLocalization("M_CANNOTHANGUP"));
        return;
    }

    if (bonusInfo->max_number <= m_instanceInfo.battle_times && m_instanceType == INSTANCE_TYPE_NORMAL)
    {
        // 弹出确认提示框
        ShowConfirmBox();
        return;
    }

	//if (m_instanceType == INSTANCE_TYPE_JINGYING && m_jingYingPlayTimes <= 0)
	if (bonusInfo->max_number <= m_instanceInfo.battle_times && m_instanceType == INSTANCE_TYPE_JINGYING)
    {
        ShowJYConfirmBox();
		//ShowMessage(Localizatioin::getLocalization("M_PVAI_CHELLENGTIMES_OVER"));
        return;
    }

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowAutoOnHook(m_isFromInstancePanel);
        GetAutoOnHook()->setInstanceType(m_instanceInfo.id, m_instanceType);
        GetAutoOnHook()->sendCSABMonsterReq(m_instanceInfo.id);
    }

	InstanceListLayer* layer = dynamic_cast<InstanceListLayer*>(getParent());
	if (layer)
	{
		layer->closeInstanceInfoLayer();
		layer->setVisible(false);
	}
	else
	{
		this->removeFromParentAndCleanup(true);
	}

}

void NormalInstanceInfoLayer::onSelectItem1(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(1, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem2(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(2, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem3(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(3, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem4(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(4, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem5(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(1, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem6(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(2, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem7(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(3, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem8(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(4, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem9(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(5, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem10(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(6, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem11(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(7, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onSelectItem12(CCObject* sender, CCControlEvent event)
{
    ShowItemTip(8, dynamic_cast<CCNode*>(sender));
}

void NormalInstanceInfoLayer::onExit()
{
    TipMgr::Get()->destroyTip();
    CCLayer::onExit();
}

void NormalInstanceInfoLayer::onEnter()
{
    CCLayer::onEnter();

    m_autoBtn->setTouchPriority(kNormalInstanceLayerMenuTouchPriority);
    m_closeBtn->setTouchPriority(kNormalInstanceLayerMenuTouchPriority);
    m_enterBtn->setTouchPriority(kNormalInstanceLayerMenuTouchPriority);

    for (unsigned int i = 0; i < m_vtLayers.size(); ++i)
    {
        CCLayer* layer = m_vtLayers[i];
        if (layer == NULL)
        {
            continue;
        }

        CCControlButton* button = dynamic_cast<CCControlButton*>(layer->getChildByTag(1));
        if (button == NULL)
        {
            continue;
        }
        
        button->setTouchPriority(kNormalInstanceLayerMenuTouchPriority);
    }
}

void NormalInstanceInfoLayer::InitLayer(InstanceInfo info, INSTANCE_TYPE type, int jingYingPlayTimes, bool tutorials, bool isFromInstancePanel)
{
    //m_instanceInfo = info;
    m_instanceType = type;
    m_jingYingPlayTimes = jingYingPlayTimes;
	m_isFromInstancePanel = isFromInstancePanel;
    int id = info.id;
    InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
    GameConfigFromLuaManager::Get()->getInstanceBonusInfo(id, bonusInfo);

    // 显示星星
    int starCount = info.star - 1;
    if (starCount < 0)
    {
        starCount = 0;
    }

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
    cache->addSpriteFramesWithFile("ccbResources/instance.plist", "ccbResources/instance.png");  

    for (int i = 0; i < 3; ++i)
    {
        if (i < starCount)
        {
            CCSpriteFrame* frame = cache->spriteFrameByName("instance_star.png");
            m_vtStars[i]->setDisplayFrame(frame);
        } else
        {
            CCSpriteFrame* frame = cache->spriteFrameByName("instance_star_black.png");
            m_vtStars[i]->setDisplayFrame(frame);
        }
    }

    // 显示剩余挂机次数
    InstanceInfo* pInfo = InstanceManager::Get()->getOneInstance(info.id);
	m_instanceInfo = *pInfo;
    if (m_instanceType == INSTANCE_TYPE_NORMAL)
    {
        m_timesLabel->setString(CCString::createWithFormat("%d/%d", bonusInfo->max_number - pInfo->battle_times, bonusInfo->max_number)->getCString());
    } 
	else if (m_instanceType == INSTANCE_TYPE_JINGYING)
	{
		//m_timesLabel->setString(CCString::createWithFormat("%d/3", m_jingYingPlayTimes)->getCString());
		m_timesLabel->setString(CCString::createWithFormat("%d/%d", bonusInfo->max_number - pInfo->battle_times, bonusInfo->max_number)->getCString());
    } 
	else
    {
        m_timesLabel->setString("");
    }

    // 显示需要战斗力
    m_powerLabel->setString(CCString::createWithFormat("%d", bonusInfo->command_power)->getCString());

    // 显示剩余体力
    USERINFO userInfo = UserData::GetUserInfo();
    int staming = userInfo.m_stamina;
    m_labelLife->setString(CCString::createWithFormat("%d", staming)->getCString());

    // 显示属性奖励
    m_bonusCash->setString(CCString::createWithFormat("%d", bonusInfo->money)->getCString());
    m_bonusHun->setString(CCString::createWithFormat("%d", bonusInfo->exp)->getCString());

	// 显示进入副本消耗体力值
	m_labelCost->setString(CCString::createWithFormat("%d", bonusInfo->power)->getCString());

    for (int i = 0; i < 12; ++i)
    {
        if (i < 4 && bonusInfo->drops.size() <= 4 ||
            i >= 4 && bonusInfo->drops.size() > 4)
        {
            m_vtLayers[i]->setVisible(true);
        } else
        {
            m_vtLayers[i]->setVisible(false);
        }
    }

    // 显示物品奖励
    int min, max;
    if (bonusInfo->drops.size() <= 4)
    {
        min = 0;
        max = 4;
    } else
    {
        min = 4;
        max = 12;
    }

    for (int i = min; i < max; ++i)
    {
        if (i - min >= bonusInfo->drops.size())
        {
            m_vtLayers[i]->setVisible(false);
        } else
        {
            m_vtLayers[i]->setVisible(true);

            CCSprite* iconSprite = ItemManager::Get()->getIconSpriteById(bonusInfo->drops[i - min]);
            CCSize s = m_vtLayers[i]->getContentSize();
            iconSprite->setPosition(ccp(s.width / 2, s.height / 2));
            m_vtLayers[i]->addChild(iconSprite);
        }
    }

    delete bonusInfo;

    if (tutorials == true)
    {
        m_autoBtn->setEnabled(false);
        m_closeBtn->setEnabled(false);

        // AddAnimation();
    }

    // 显示当前战斗力
    CCString* pString = CCString::createWithFormat("(%s %d)", Localizatioin::getLocalization("M_CURRENT"), UserData::getUserEffectiveness());
    m_currentCombat->setString(pString->getCString());

    // 检查如果是地下城副本，则隐藏挂机按钮
    if (type == INSTANCE_TYPE_DUNGEON)
    {
        m_autoBtn->setVisible(false);
        m_enterBtn->setPosition(ccp(m_enterBtn->getParent()->getContentSize().width / 2, m_enterBtn->getPosition().y));

        m_desc1->setVisible(false);
        m_desc2->setVisible(false);
    }
}


void NormalInstanceInfoLayer::onBroadcastMessage(BroadcastMessage* msg)
{
	if(msg->message == GM_ATTR_STAMINA_UPDATE){
		m_labelLife->setString(CCString::createWithFormat("%d", UserData::GetUserInfo().m_stamina)->getCString());
	}else if(msg->message == GM_UPDATE_AUTO_BATTLE_TIMES){
		RefreshTimes(NULL);
	}
}

void NormalInstanceInfoLayer::RefreshTimes(CCObject* obj)
{
    int id = m_instanceInfo.id;
    InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
    GameConfigFromLuaManager::Get()->getInstanceBonusInfo(id, bonusInfo);

    m_instanceInfo.battle_times = 0;

    // 显示剩余挂机次数
    if (m_instanceType == INSTANCE_TYPE_NORMAL)
    {
        m_timesLabel->setString(CCString::createWithFormat("%d/%d", bonusInfo->max_number, bonusInfo->max_number)->getCString());
        InstanceManager::Get()->RefreshInstanceTimes(id);
    }
	else if (m_instanceType == INSTANCE_TYPE_JINGYING)
	{
		m_timesLabel->setString(CCString::createWithFormat("%d/%d", bonusInfo->max_number, bonusInfo->max_number)->getCString());
		InstanceManager::Get()->RefreshInstanceTimes(id);
	} 
}

void NormalInstanceInfoLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kNormalInstanceLayerTouchPriority, true);
}

bool NormalInstanceInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    // `ItemManager::Get()->showItemTipsById(drops[0], pTouch->getLocation());
    return true;
}

void NormalInstanceInfoLayer::ShowItemTip(int itemPos, CCNode* node)
{
    InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
    GameConfigFromLuaManager::Get()->getInstanceBonusInfo(m_instanceInfo.id, bonusInfo);

    if (itemPos > bonusInfo->drops.size())
    {
        return;
    }

    int itemId = bonusInfo->drops[itemPos -1];
    
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(itemId, pt);

	bool isFairy = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Fairy/FairyLuaInterface.lua", "G_GetIsFairyById", itemId);
	ItemBase * info = ItemManager::Get()->getItemInfoById(itemId);
	if (isFairy || info->m_type == 21)
	{
		m_fairyId = itemId % 10000;
		const char * detailText = Localizatioin::getLocalization("M_EQUIPMENT_INFORMATION");
		ItemManager::Get()->setTipDelegate(this,menu_selector(NormalInstanceInfoLayer::onFairyDetailClicked),detailText,true,NULL,NULL,"",false);
	}
	/*EquipmentItem* equipInfo = dynamic_cast<EquipmentItem*>(info);
	if(equipInfo != NULL || info->m_type == 19)
	{
		m_fairyId = itemId;
		const char * detailText = Localizatioin::getLocalization("M_FAIRY_GET_WAY");
		ItemManager::Get()->setTipDelegate(this,menu_selector(NormalInstanceInfoLayer::onGetWayCallBack),detailText,true,NULL,NULL,"",false);
		this->onCloseLayer(NULL,NULL);
	}*/
	
}

void NormalInstanceInfoLayer::onFairyDetailClicked(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyInfoLayerById",m_fairyId,0);
	InstanceListLayer* layer = dynamic_cast<InstanceListLayer*>(getParent());
    if (layer)
    {
        layer->closeInstanceInfoLayer();
        layer->setVisible(false);
    }
	else
	{
		this->removeFromParentAndCleanup(true);
	}
}

void NormalInstanceInfoLayer::onGetWayCallBack(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<void>("Script/Fairy/FairyLuaInterface.lua","G_ShowFairyGetwayLayer",m_fairyId);
}

void NormalInstanceInfoLayer::AddAnimation()
{
    CCSprite* pTmpSprite = NULL;
    pTmpSprite = createOneFlag("map_ui_lead_FRAME_BUTTON_LEAD_DOWN","map_ui_lead_FRAME_TEXT_LEAD_DOWN");

    if (pTmpSprite)
    {
        this->addChild(pTmpSprite);
        pTmpSprite->setVisible(true);

        CCSize size = pTmpSprite->getContentSize();

        pTmpSprite->setAnchorPoint(ccp(0.5,0.0));
        pTmpSprite->setPosition(ccpAdd(m_enterBtn->getPosition(), ccp(0, 60)));
        pTmpSprite->stopAllActions();
        CCActionInterval* action = NULL;
        action = CCMoveBy::create(0.8,ccp(0, 20));

        CCActionInterval* action_back = action->reverse();
        CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));

        CCRepeatForever* pAction = CCRepeatForever::create(seq);
        pTmpSprite->runAction( pAction );

        TXGUI::UILabelTTF* pLabelNode = dynamic_cast<TXGUI::UILabelTTF*>(pTmpSprite->getChildByTag(10001));
        if (pLabelNode)
        {
            pLabelNode->setString(Localizatioin::getLocalization("M_CLICK_ENTERTHEBATTLE"));
        }
    }
}

CCSprite* NormalInstanceInfoLayer::createOneFlag(std::string bgName,std::string txtName)
{
    CCSprite* pSprite = AspriteManager::getInstance()->getOneFrame(KUI_LEAD_BIN,bgName.c_str());
    if (pSprite)
    {
        //m_pFlagParentNode->addChild(pSprite,1);
        pSprite->setVisible(false);
        pSprite->setAnchorPoint(ccp(0.5,0.5));

        CCPoint relativePoint;
        CCSprite* pTaskTextExpLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_LEAD_BIN,bgName,txtName,ccp(0.5,0.5),relativePoint);
        CCSize labelSize = pTaskTextExpLabelSprite->getContentSize();
        TXGUI::UILabelTTF * taskTextExpLabel = TXGUI::UILabelTTF::create("", KJLinXin, 24,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        CCSize  picSize = pSprite->getContentSize();
        taskTextExpLabel->setPosition(ccp(relativePoint.x+picSize.width/2,relativePoint.y+picSize.height/2));
        taskTextExpLabel->setColor(ccc3(65,15,15));
        pSprite->addChild(taskTextExpLabel,1,10001);		
    }
    return pSprite;
}

void NormalInstanceInfoLayer::ShowConfirmBox()
{
    int times = m_instanceInfo.refresh_times;
    if (times >= 3)
    {
        MessageBox::Show(Localizatioin::getLocalization("M_RESET_INSTANCE_FULL"), this, NULL, NULL, MB_OK);
        return;
    }

    int basicCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 20520);
    int addedCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 20521);

    CCString* pString = CCString::createWithFormat(Localizatioin::getLocalization("M_RESET_INSTANCE"), basicCost + addedCost * (times));

    MessageBox::Show(pString->getCString(), this, menu_selector(NormalInstanceInfoLayer::OnResetTimes), 
        menu_selector(NormalInstanceInfoLayer::OnCancel),MB_OKCANCELCLOSE);
    MessageBox::AdjustStyle(Localizatioin::getLocalization("M_CONFIRM"), Localizatioin::getLocalization("M_CANCEL"));
    MessageBox::showCloseBtn();
}
  
void NormalInstanceInfoLayer::ShowJYConfirmBox()
{
	int maxTimes = LuaTinkerManager::Get()->getLuaConfig<int>("vip", "Vip", UserData::GetVipLevel(), "Elite");
	int times = m_instanceInfo.refresh_times;
	if (times >= maxTimes)
	{
		MessageBox::Show(Localizatioin::getLocalization("M_RESET_NOTIME"), this, NULL, NULL, MB_OK);
		return;
	}

	int cost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 20000 + m_instanceInfo.refresh_times);

	CCString* pString = CCString::createWithFormat(Localizatioin::getLocalization("M_RESET_INSTANCE"), cost);
	CCString* pString1 = CCString::createWithFormat("%s%d/%d",Localizatioin::getLocalization("M_RESET_NUMBER"), maxTimes-m_instanceInfo.refresh_times, maxTimes);

	MessageBox::Show(pString->getCString(), this, menu_selector(NormalInstanceInfoLayer::OnResetTimes), 
		menu_selector(NormalInstanceInfoLayer::OnCancel),MB_OKCANCELCLOSE);
	MessageBox::ShowTextExt(pString1->getCString());
	MessageBox::AdjustStyle(Localizatioin::getLocalization("M_CONFIRM"), Localizatioin::getLocalization("M_CANCEL"));
	MessageBox::showCloseBtn();
}
 
void NormalInstanceInfoLayer::OnResetTimes(CCObject* sender)
{
    OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(20520, &(m_instanceInfo.id), 1);
}

void NormalInstanceInfoLayer::OnCancel(CCObject* sender)
{

}

NormalInstanceInfoUILayer::NormalInstanceInfoUILayer(CCNode* parent) :
    m_infoLayer(NULL)
{
    UILayerColorBase* pLayer =  new UILayerColorBase( true,  true,  ccc4(255,255,255,0));
    pLayer->init();
    pLayer->autorelease();
    pLayer->setPosition(CCPointZero);
    UILayout::m_pControlNode = pLayer;

    CCNode *pNode = getCurrentNode();
    parent->addChild(pNode);
}

NormalInstanceInfoUILayer::~NormalInstanceInfoUILayer()
{

}

CCNode* NormalInstanceInfoUILayer::getTutorialNode(std::string name)
{
    return m_infoLayer->getEnterBtn();
}
