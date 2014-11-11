#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "ServerInfoFromServer.h"
#include "XLogicManager.h"

#include "PKLayer.h"
#include "TXGUIHeader.h"
#include "GameAudioManager.h"

#include "GameResourceManager.h"
#include "GameFontManager.h"
#include "AspriteManager.h"
#include "ASprite.h"

#include "UICreator.h"
#include "AssetsManager.h"
#include "HelloWorldScene.h"
#include "Localization.h"
#include "GameConfigFromLuaManager.h"
#include "GMessage.h"
#include "ParticleManager.h"
#include "MessageBoxLayer.h"
#include "SkeletonAnimRcsManager.h"
#include "UserData.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"
#include "MainMenuLayer.h"
#include "TimeManager.h"
using namespace TXGUI;

#define LOADINGTIME 10

 bool   PVPLearnLayer::isShow  = false;
 bool  PVPLearnLayer::getShow() 
 {
	 return isShow ;
 }
PVPLearnLayer::PVPLearnLayer()
{
	
	m_layout = NULL;
	isShow = false ;
	delet_menu = 0 ;
}

PVPLearnLayer::~PVPLearnLayer()
{
	UIManager::sharedManager()->RemoveUILayout("pkDiolgLayout");
	TimeManager::Get()->stopTimer(TIMER_PK_LAYER_REMAIN);
	//TimeManager::Get()->unregisterTimer(this, TIMER_PK_LAYER_REMAIN);
	m_layout = NULL;
    isShow = false ;
	
	
}
void PVPLearnLayer::OnClose(CCObject* pSender)
{
    
	if(mLearnInfo.status == 0)
	{

	}
	else if (mLearnInfo.status ==1)
	{
		OnlineNetworkManager::sShareInstance()->SendPVPAckLearnReq(mLearnInfo.uid ,mLearnInfo.reg_tm ,mLearnInfo.channel_id ,false,1);
	}
	 removeFromParentAndCleanup(true);
}

void PVPLearnLayer::OnConfirm(CCObject* pSender)
{
	
	if(mLearnInfo.status == 0)
	{
	
		OnlineNetworkManager::sShareInstance()->SendPVPLearnReq(mLearnInfo.uid,mLearnInfo.reg_tm ,mLearnInfo.channel_id ,mLearnInfo.mandatory);

		
	}
	else if (mLearnInfo.status ==1)
	{
		OnlineNetworkManager::sShareInstance()->SendPVPAckLearnReq(mLearnInfo.uid ,mLearnInfo.reg_tm ,mLearnInfo.channel_id ,true ,0);
		

	}
	  removeFromParentAndCleanup(true);
}
void PVPLearnLayer::PKbuttonCb(CCObject* pSender)
{
	this->setVisblePKButton(false,mLearnInfo );
	
	
	
	this->setStyle(mLearnInfo);
	this->setVisible(true);
	
}
void PVPLearnLayer::setVisblePKButton(bool  val ,PVPLearnInfo LearnInfo)
{
	MainMenuLayer* layer = GetMainMenuLayer();
	 int tag = MainMenuLayer::getLayerTagByName("PKButtonUI");

	if(val)
	{
		ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
		int idx = TXGUI::getResourceIDByName("map_ui_FRAME_PK_ICON");
		CCPoint pt;
		CCSprite *pEmptyItem = as->getSpriteFromFrame_Middle( idx, 0, pt);

		CCSprite* baseSprite = CCSprite::create();
		baseSprite->setContentSize(pEmptyItem->getContentSize());
		baseSprite->addChild(pEmptyItem,20);
		pEmptyItem->setPosition(ccp(pEmptyItem->getContentSize().width * 0.5f,pEmptyItem->getContentSize().height * 0.5f));
		SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(441);
		EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(441,"",false);
        if(effect1)
        {
            baseSprite->addChild(effect1,10);
			effect1->setPosition(ccp(pEmptyItem->getContentSize().width * 0.5f,pEmptyItem->getContentSize().height * 0.5f));
            effect1->SetAnim(kType_Play,1,true);
        }
		delet_menu = 1 ;
		IconButton * pIcon = new IconButton(baseSprite, NULL, baseSprite, this, 
			menu_selector(PVPLearnLayer::PKbuttonCb));
		pIcon->autorelease();

		pIcon->setContentSize(baseSprite->getContentSize());
		//CCPoint statusPt;
		//CCSprite * buttonSprite = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_PK_ICON"),0,statusPt);
		float scale = UIManager::sharedManager()->getScaleFactor();
		float scalex = UIManager::sharedManager()->getScaleFactorX();
		float scaley = UIManager::sharedManager()->getScaleFactorY();
		pIcon->setScaleX(scalex);
		pIcon->setScaleY(scaley);
		pIcon->setPosition(ccp(pt.x*scalex,pt.y*scaley));
		layer->addChild(pIcon ,Z_Order_WaitingLayer ,tag);
		mLearnInfo.channel_id = LearnInfo.channel_id ;
		mLearnInfo.effectiveness = LearnInfo.effectiveness ;
		mLearnInfo.mandatory = LearnInfo.mandatory ;
		mLearnInfo.playName = LearnInfo.playName ;
		mLearnInfo.reg_tm = LearnInfo.reg_tm ;
		mLearnInfo.status = LearnInfo.status ;
		mLearnInfo.type = LearnInfo.type ;
		mLearnInfo.uid = LearnInfo.uid ;
		//PVPLearnLayer* layer = (PVPLearnLayer*)	PVPLearnLayer::create() ;
		GameAudioManager::sharedManager()->playEffect(350014,false);
	}
	else
	{
		delet_menu = 0 ;
		layer->removeChildByTag(tag);
	}
}


void PVPLearnLayer::onUpdateRemainTime(long remainTime)
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
	if(remainTime <= 0)
	{
		MainMenuLayer* layer = GetMainMenuLayer();
		int tag = MainMenuLayer::getLayerTagByName("PKButtonUI");
		if(delet_menu == 1)
			layer->removeChildByTag(tag);
		delet_menu =3 ;
		this->scheduleUpdate();
		
		
	}
	else 
	{
		//printf(timeString);
		//setUpCDTime(timeString);
	}
	
}
void PVPLearnLayer::updateLogin(float dt) 
{
	OnlineNetworkManager::sShareInstance()->SendPVPAckLearnReq(mLearnInfo.uid ,mLearnInfo.reg_tm ,mLearnInfo.channel_id ,true ,0);
	removeFromParentAndCleanup(true);
}
void PVPLearnLayer::mandSendAccept()
{
	scheduleOnce(schedule_selector(PVPLearnLayer::updateLogin),1.0f);
}
void PVPLearnLayer::setStyle(PVPLearnInfo LearnInfo)
{
	UILabel * pPvplernTitle= m_layout->FindChildObjectByName<UILabel>("pkInviteTitleLable");

	UILabel * pinvite_introduction_2= m_layout->FindChildObjectByName<UILabel>("pk_invite_introduction_2Lable");


	UILabel * pk_invite_text_1_1Lable= m_layout->FindChildObjectByName<UILabel>("pk_invite_text_1_1Lable");
	UILabel * pk_invite_text_1_nameLable= m_layout->FindChildObjectByName<UILabel>("pk_invite_text_1_nameLable");
	UILabel * pk_invite_text_1_2Lable= m_layout->FindChildObjectByName<UILabel>("pk_invite_text_1_2Lable");
	UILabel * pk_invite_introduction_4Lable= m_layout->FindChildObjectByName<UILabel>("pk_invite_introduction_4Lable");
	

	UILabel * pk_invite_text_2_nameLable= m_layout->FindChildObjectByName<UILabel>("pk_invite_text_2_nameLable");
	UILabel * pk_invite_text_2Lable= m_layout->FindChildObjectByName<UILabel>("pk_invite_text_2Lable");

	mLearnInfo.channel_id = LearnInfo.channel_id ;
	mLearnInfo.effectiveness = LearnInfo.effectiveness ;
	mLearnInfo.mandatory = LearnInfo.mandatory ;
	mLearnInfo.playName = LearnInfo.playName ;
	mLearnInfo.reg_tm = LearnInfo.reg_tm ;
	mLearnInfo.status = LearnInfo.status ;
	mLearnInfo.type = LearnInfo.type ;
	mLearnInfo.uid = LearnInfo.uid ;
	if(mLearnInfo.type <1)mLearnInfo.type =1 ;
	if(mLearnInfo.type>6)mLearnInfo.type =6 ;
	const char* rst = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/GameUILayer.lua", "getPKTypeCH", mLearnInfo.type);
	pinvite_introduction_2->setString(rst);
	if(LearnInfo.status == 0)
	{
		TimeManager::Get()->stopTimer(TIMER_PK_LAYER_REMAIN);

		
		pk_invite_text_2_nameLable->setVisible(false);
		pk_invite_text_2Lable->setVisible(false);
		pk_invite_text_1_nameLable->setString(mLearnInfo.playName.c_str() );

		if(!mLearnInfo.mandatory)
		{
			pk_invite_introduction_4Lable->setVisible(false);
			pPvplernTitle->setString(Localizatioin::getLocalization("M_PK_INVITE_TITLE_1"));
		}
		else
		{
			pPvplernTitle->setString(Localizatioin::getLocalization("M_PK_INVITE_TITLE_3"));
			pk_invite_text_1_1Lable->setString(Localizatioin::getLocalization("M_PK_INVITE_TEXT_3"));
	

		}
		
	}
	else if(LearnInfo.status == 1)
	{
		pPvplernTitle->setString(Localizatioin::getLocalization("M_PK_INVITE_TITLE_2"));
		pk_invite_text_1_1Lable->setVisible(false);
		pk_invite_text_1_2Lable->setVisible(false);
		pk_invite_text_1_nameLable->setVisible(false);
		pk_invite_introduction_4Lable->setVisible(false);
		pk_invite_text_2_nameLable->setString(mLearnInfo.playName.c_str() );
		
	}
}

bool PVPLearnLayer::init()
{	
	bool bRet = false;
	isShow = true  ;

	

	do {
		
		if(TimeManager::Get()->hasTimer(TIMER_PK_LAYER_REMAIN))
		{
			TimeManager::Get()->renewTimer(TIMER_PK_LAYER_REMAIN, TimeManager::Get()->getCurServerTime()+10);
			if (!TimeManager::Get()->hasObserver(this, TIMER_PK_LAYER_REMAIN))
			{
				TimeManager::Get()->attachTimer(this, TIMER_PK_LAYER_REMAIN);
			}
		}
		else
		{
			TimeManager::Get()->registerTimer(this, TIMER_PK_LAYER_REMAIN, TimeManager::Get()->getCurServerTime()+10);
		}

		m_layout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/pkDiolgUI.plist", this, "pkDiolgLayout");
		if(m_layout == NULL)
			return false ;
		UIManager::sharedManager()->RegisterMenuHandler("pkDiolgLayout","PVPLearnLayer::OnClose",
			menu_selector(PVPLearnLayer::OnClose), this);
		UIManager::sharedManager()->RegisterMenuHandler("pkDiolgLayout","PVPLearnLayer::OnConfirm",
			menu_selector(PVPLearnLayer::OnConfirm), this);

        /// response to keyboard
		this->setKeypadEnabled(true);
		
		bRet = true;
	}while (0);
	return bRet;
}
void PVPLearnLayer::update(float dt)
{
	if(delet_menu == 3)
	{
		delet_menu = 0 ;
		removeFromParentAndCleanup(true);
		
	}
	
	//   unschedule(schedule_selector(FrontCoverLayer::update));
	//   //schedule(schedule_selector(FrontCoverLayer::readyLogin_func), 0.5f);
	//ReadyToLogin();
}



 


