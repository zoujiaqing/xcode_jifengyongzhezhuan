//
//  AccountsLayer.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-7-2.
//
//

#include "OnlineNetworkManager.h"
#include "AccountsLayer.h"
#include "AspriteManager.h"
#include "Localization.h"
#include "GameAudioManager.h"
#include "ItemManager.h"
#include "GameFontManager.h"
#include "XLogicManager.h"
#include "GameManager.h"
#include "SpriteSeer.h"
#include "LuaTinkerManager.h"
#include "MainMenuLayer.h"
#include "LevelManager.h"
#include "InstanceManager.h"
//#include "ResoureUpdate.h"
#include "PVPMessage.h"
#include "TipMgr.h"
#include "MainMenuLayer.h"
#include "BattleUILayer.h"
#include "WaitingLayerManager.h"
#include "GameConfigFromLuaManager.h"
#include "InterfaceManager.h"

using namespace TXGUI;

accountRewardDetailData AccountsLayer::s_accountDetailData;

AccountsLayer::AccountsLayer()
{
    this->m_pLayer = NULL;
    
    this->winBackGround = NULL;
    this->winDecoration = NULL;
    this->winDecorationCityDefence = NULL;
    this->win_icon_star1 = NULL;
    this->win_icon_star2 = NULL;
    this->win_icon_star3 = NULL;
    
    this->win_text_estimate = NULL;   
    this->win_text_finishtime = NULL;
    this->win_number_finishtime = NULL;
    
    this->win_text_killnumber = NULL;
    this->win_number_killnumber = NULL;

    this->win_text_combo = NULL;
    this->win_number_combo = NULL;
    
    this->win_text_basicReward = NULL;
    
    this->win_icon_reward1 = NULL;
    this->win_number_reward1 = NULL;
    
    this->win_icon_reward2 = NULL;
    this->win_number_reward2 = NULL;
    
    this->win_icon_reward3 = NULL;
    this->win_number_reward3 = NULL;
    
    this->win_icon_reward4 = NULL;
    this->win_number_reward4 = NULL;
    
    this->win_text_specialReward = NULL;
    
    this->win_frame_specialReward1 = NULL;
    this->win_frame_icon_specialReward1 = NULL;
    
    this->win_frame_specialReward2 = NULL;
    this->win_frame_icon_specialReward2 = NULL;
    
    this->win_frame_specialReward3 = NULL;
    this->win_frame_icon_specialReward3 = NULL;
    
    this->win_frame_specialReward4 = NULL;
    this->win_frame_icon_specialReward4 = NULL;
    
    this->win_text_backToTown = NULL;    
    this->win_text_leftBackToTown = NULL;
    this->win_text_next = NULL;
    this->win_text_onceMore = NULL;
    
    this->win_onback = NULL;
    this->win_onleftback = NULL;
    this->win_onnext = NULL;
    this->win_onceMore = NULL;
    
    this->m_star = 0;
    
    this->m_killtime = 0;
    this->m_killnum = 0;
    this->m_killcombo = 0;
    
    this->add_coin = 0;
    this->add_domiand = 0;
    this->add_exp = 0;
    this->add_exploit = 0;
	this->add_contributions = 0;
    
    this->m_item1_id=0;
    this->m_item1_num=0;
    
    this->m_item2_id=0;
    this->m_item2_num=0;
    
    this->m_item3_id=0;
    this->m_item3_num=0;
    
    this->m_item4_id=0;
    this->m_item4_num=0;
    
    this->m_itemcnt=0;
}

AccountsLayer::~AccountsLayer()
{
    UIManager::sharedManager()->RemoveUILayout("FightAccounts");
    UIManager::sharedManager()->RemoveUILayout("FightWinPVP");
    UIManager::sharedManager()->RemoveUILayout("FightFailure");
}

bool AccountsLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    this->setTouchPriority(kCCMenuHandlerPriority);

    //send the data to server 
    //1002 is to check if we lauch the game the first time
        
    USERINFO mUserData = UserData::GetUserInfo();       
    
    if(mUserData.first_login){
        OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(1002);
        mUserData.first_login=false;
        UserData::SetUserInfo(mUserData);
        SceneLayer* sl = (SceneLayer*)XLogicManager::sharedManager()->GetCurLayer();
        sl->onFirstEnterMainland();
        
    }
    
    return true;
}

void AccountsLayer::openFailureBattleTimeOut()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc* f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openFailureBattle_core));
    CCSequence* seq = (CCSequence*)CCSequence::create(delay,f,NULL);
    this->runAction(seq);
}

void AccountsLayer::openFailureBattleDie(unsigned int index)
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc* f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openFailureBattle_core));
    CCSequence* seq = (CCSequence*)CCSequence::create(delay,f,NULL);
    this->runAction(seq);
}

void AccountsLayer::openFailureBattle_core()
{
    UIManager* manager = UIManager::sharedManager();
    m_pLayer = manager->CreateUILayoutFromFile("UIplist/FightFailure.plist", this, "FightFailure", true);
    PVPMessage::Get()->sendAccountBar();
    manager->RegisterMenuHandler("FightFailure","AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
    manager->RegisterMenuHandler("FightFailure","AccountsLayer::onGetFairy", menu_selector(AccountsLayer::onGetFairy), this);
    manager->RegisterMenuHandler("FightFailure","AccountsLayer::onTrainFairy", menu_selector(AccountsLayer::onTrainFairy), this);
    manager->RegisterMenuHandler("FightFailure","AccountsLayer::onStrenthEquip", menu_selector(AccountsLayer::onStrenthEquip), this);
    manager->RegisterMenuHandler("FightFailure","AccountsLayer::onBuyDiamond", menu_selector(AccountsLayer::onBuyDiamond), this);

    resetButtonPic("attackBt");
    resetButtonPic("defenceBt");
    resetButtonPic("lifeBt");

    USERINFO info = UserData::GetUserInfo();
    unsigned int vipLv = info.m_vip;
    char hit2LabelName[50];
    sprintf(hit2LabelName,"M_DEAD_VIP%u_TEXT",vipLv);
    const char* hit2Text = Localizatioin::getLocalization(hit2LabelName);
    const char* hitText = Localizatioin::getLocalization("M_DEAD_VIPHINT");
    
    if(vipLv > 0)
    {
        UILabel* hint2Label = m_pLayer->FindChildObjectByName<UILabel>("hint2Label");
        if(hit2Text)
        {
            hint2Label->setString(hit2Text);
            hint2Label->setVisible(true);
        }
        UILabel* hint1Label = m_pLayer->FindChildObjectByName<UILabel>("hint1Label");
        if(hitText)
        {
            hint1Label->setString(hitText);
            hint1Label->setVisible(true);
        }
        UILabel* hintVipLabel = m_pLayer->FindChildObjectByName<UILabel>("hintVipLabel");
        char vipText[50];
        sprintf(vipText,"vip%u",vipLv+1);
        hintVipLabel->setString(vipText);
        hintVipLabel->setVisible(true);
    }
    else
    {
        UILabel* hit2 = m_pLayer->FindChildObjectByName<UILabel>("hint2Label");
        if(hit2Text)
        {
            hit2->setString(hit2Text);
            hit2->setVisible(true);
        }
        UILabel* hit0Label = m_pLayer->FindChildObjectByName<UILabel>("hit0Label");
        hit0Label->setVisible(true);
    }
}

//void AccountsLayer::openFailureBattleTimeOut_core()
//{
//    UIManager* manager = UIManager::sharedManager();
//    m_pLayer = manager->CreateUILayoutFromFile("UIplist/FightFailure.plist", this, "FightFailure", true);
//    manager->RegisterMenuHandler("FightFailure","AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
//    manager->RegisterMenuHandler("FightFailure","AccountsLayer::OnLive", menu_selector(AccountsLayer::OnLive), this);
//    
//    PVPMessage::Get()->sendAccountBar();
//
//    //UILabel* failspecial_text_fail1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_fail1");
//    //UILabel* failspecial_text_fail2 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_fail2");
//    UILabel* failspecial_text_need = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_need");
//    
//    UIPicture* failspecial_icon_diamond = m_pLayer->FindChildObjectByName<UIPicture>("failspecial_icon_diamond");
//    UILabel* failspecial_number_diamond = m_pLayer->FindChildObjectByName<UILabel>("failspecial_number_diamond");
//    UILabel* failspecial_text_can1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_can1");
//    UILabel* failspecial_number_can1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_number_can1");
//    UILabel* failspecial_number_can2 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_number_can2");
//    
//    UIButton* win_onlive = m_pLayer->FindChildObjectByName<UIButton>("win_onlive");
//    UIButton* win_onback2 = m_pLayer->FindChildObjectByName<UIButton>("win_onback2");
//    
//    UILabel* win_text_live = m_pLayer->FindChildObjectByName<UILabel>("win_text_live");
//    UILabel* win_text_backToTown2 = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown2");
//    
//    //failspecial_text_fail1->setVisible(false);
//    //failspecial_text_fail2->setVisible(false);
//    failspecial_text_need->setVisible(false);
//    
//    failspecial_icon_diamond->setVisible(false);
//    failspecial_number_diamond->setVisible(false);
//    failspecial_text_can1->setVisible(false);
//    failspecial_number_can1->setVisible(false);
//    failspecial_number_can2->setVisible(false);
//    
//    win_onlive->setVisible(false);
//    win_onback2->setVisible(false);
//    
//    win_text_live->setVisible(false);
//    win_text_backToTown2->setVisible(false);
//    
//    //UILabel* failspecial_text_timeup1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_timeup1");
//    //UILabel* failspecial_text_timeup2 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_timeup2");
//    //UILabel* failspecial_text_timeup3 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_timeup3");
//    
//    UILabel* win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
//
//    if (LevelManager::sShareInstance()->isCurrentCityDefendLevel())
//    {
//        const char* msg = Localizatioin::getLocalization("M_CITYDEFEND_BACKTOTOWN");
//        win_text_backToTown->setString(msg);
//    }
//    else
//    {
//        const char* msg = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
//        win_text_backToTown->setString(msg);
//    }
//    
//    //msg = Localizatioin::getLocalization("M_ESTIMATE_PVP_FAILTEXT1");
//    //failspecial_text_timeup1->setString(msg);
//    
//    //msg = Localizatioin::getLocalization("M_ESTIMATE_PVP_FAILTEXT2");
//    //failspecial_text_timeup2->setString(msg);
//    
//    //msg = Localizatioin::getLocalization("M_ESTIMATE_PVP_FAILTEXT3");
//    //failspecial_text_timeup3->setString(msg);
//    
//    GameAudioManager::sharedManager()->stopPlayBGM();
//    GameAudioManager::sharedManager()->playEffect(350008,false);
//}
//
//void AccountsLayer::openFailureBattleDie_core()
//{
//    UIManager* manager = UIManager::sharedManager();
//    m_pLayer = manager->CreateUILayoutFromFile("UIplist/FightFailure.plist", this, "FightFailure", true);
//    manager->RegisterMenuHandler("FightFailure","AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
//    manager->RegisterMenuHandler("FightFailure","AccountsLayer::OnLive", menu_selector(AccountsLayer::OnLive), this);
//    
//    PVPMessage::Get()->sendAccountBar();
//    
//    UILabel* failspecial_text_timeup1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_timeup1");
//    UILabel* failspecial_text_timeup2 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_timeup2");
//    UILabel* failspecial_text_timeup3 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_timeup3");
//    
//    UIButton* win_onback = m_pLayer->FindChildObjectByName<UIButton>("win_onback");
//    UILabel* win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
//    
//    failspecial_text_timeup1->setVisible(false);
//    failspecial_text_timeup2->setVisible(false);
//    failspecial_text_timeup3->setVisible(false);
//    win_onback->setVisible(false);
//    win_text_backToTown->setVisible(false);
//    
//    UILabel* failspecial_text_fail1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_fail1");
//    UILabel* failspecial_text_fail2 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_fail2");
//    UILabel* failspecial_text_need = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_need");
//    
//    //const char* msg = Localizatioin::getLocalization("M_ESTIMATE_FAILTEXT1");
//    //failspecial_text_fail1->setString(msg);
//    
//    //msg = Localizatioin::getLocalization("M_ESTIMATE_FAILTEXT2");
//    //failspecial_text_fail2->setString(msg);
//    
//    const char* msg = Localizatioin::getLocalization("M_ESTIMATE_NEED");
//    failspecial_text_need->setString(msg);
//    failspecial_text_need->setColor(ccORANGE);
//    
//    UIPicture* failspecial_icon_diamond = m_pLayer->FindChildObjectByName<UIPicture>("failspecial_icon_diamond");
//    UILabel* failspecial_number_diamond = m_pLayer->FindChildObjectByName<UILabel>("failspecial_number_diamond");
//    UILabel* failspecial_text_can1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_text_can1");
//    UILabel* failspecial_number_can1 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_number_can1");
//    UILabel* failspecial_number_can2 = m_pLayer->FindChildObjectByName<UILabel>("failspecial_number_can2");
//    
//    if(m_revivalDiamond.empty())
//        failspecial_number_diamond->setString("10");
//    else
//        failspecial_number_diamond->setString(m_revivalDiamond.c_str());
//
//    msg = Localizatioin::getLocalization("M_ESTIMATE_RELIVE");
//    failspecial_text_can1->setString(msg);
//    failspecial_text_can1->setColor(ccORANGE);
//    
//    if(m_revivalNum.empty())
//        failspecial_number_can1->setString("0");
//    else
//        failspecial_number_can1->setString(m_revivalNum.c_str());
//
//    failspecial_number_can1->setColor(ccORANGE);
//    
//    msg = Localizatioin::getLocalization("M_ESTIMATE_REMAINTIME");
//    failspecial_number_can2->setString(msg);
//    failspecial_number_can2->setColor(ccORANGE);
//    
//    UILabel* win_text_live = m_pLayer->FindChildObjectByName<UILabel>("win_text_live");
//    UILabel* win_text_backToTown2 = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown2");
//    
//    msg = Localizatioin::getLocalization("M_ESTIMATE_RELIVENOW");
//    win_text_live->setString(msg);
//
//    if (LevelManager::sShareInstance()->isCurrentCityDefendLevel())
//    {
//        const char* msg = Localizatioin::getLocalization("M_CITYDEFEND_BACKTOTOWN");
//        win_text_backToTown2->setString(msg);
//    }
//    else
//    {
//        const char* msg = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
//        win_text_backToTown2->setString(msg);
//    }
//    
//    UIButton* win_onlive = m_pLayer->FindChildObjectByName<UIButton>("win_onlive");
//    if(win_onlive)
//    {
//        if(m_revivalDiamond.empty() && m_revivalNum.empty())
//        {
//                // TODO : gray
//        }
//    }
//    
//    GameAudioManager::sharedManager()->stopPlayBGM();
//    GameAudioManager::sharedManager()->playEffect(350008,false);
//}

void AccountsLayer::openWinBattle()
{
     UIManager* manager = UIManager::sharedManager();
     m_pLayer = manager->CreateUILayoutFromFile("UIplist/FightAccounts.plist", this, "FightAccounts", true);
     manager->RegisterMenuHandler("FightAccounts", "AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
     manager->RegisterMenuHandler("FightAccounts", "AccountsLayer::OnOnceMore", menu_selector(AccountsLayer::onOnceMore), this);
     manager->RegisterMenuHandler("FightAccounts", "AccountsLayer::OnLeftBack", menu_selector(AccountsLayer::OnBack), this);
     manager->RegisterMenuHandler("FightAccounts", "AccountsLayer::OnNext", menu_selector(AccountsLayer::OnNext), this);
    
     winBackGround = m_pLayer->FindChildObjectByName<UIPicture>("win_background");
     winDecoration = m_pLayer->FindChildObjectByName<UIPicture>("win_decoration");
     winDecorationCityDefence = m_pLayer->FindChildObjectByName<UIPicture>("win_decoration_citydefence");
     win_icon_star1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_star1");
     win_icon_star2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_star2");
     win_icon_star3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_star3");
    
     winBackGround->setVisible(false);
     winDecoration->setVisible(false);
     winDecorationCityDefence->setVisible(false);
     win_icon_star1->setVisible(false);
     win_icon_star2->setVisible(false);
     win_icon_star3->setVisible(false);
     
     win_text_estimate = m_pLayer->FindChildObjectByName<UILabel>("win_text_estimate");
     
     win_text_finishtime = m_pLayer->FindChildObjectByName<UILabel>("win_text_finishtime");
     win_number_finishtime = m_pLayer->FindChildObjectByName<UILabel>("win_number_finishtime");
     
     win_text_killnumber = m_pLayer->FindChildObjectByName<UILabel>("win_text_killnumber");
     win_number_killnumber = m_pLayer->FindChildObjectByName<UILabel>("win_number_killnumber");
     
     win_text_combo = m_pLayer->FindChildObjectByName<UILabel>("win_text_combo");
     win_number_combo = m_pLayer->FindChildObjectByName<UILabel>("win_number_combo");
     
     win_text_estimate->setVisible(false);
    
     win_text_finishtime->setVisible(false);
     win_number_finishtime->setVisible(false);
     
     win_text_killnumber->setVisible(false);
     win_number_killnumber->setVisible(false);
     
     win_text_combo->setVisible(false);
     win_number_combo->setVisible(false);
     
     const char* msg = Localizatioin::getLocalization("M_ESTIMATE_ESTIMATE");
     win_text_estimate->setString(msg);
     win_text_estimate->setColor(ccGREEN);
     
     msg = Localizatioin::getLocalization("M_ESTIMATE_FINISHTIME");
     win_text_finishtime->setString(msg);
     
     msg = Localizatioin::getLocalization("M_ESTIMATE_KILLNUMBER");
     win_text_killnumber->setString(msg);
     
     msg = Localizatioin::getLocalization("M_ESTIMATE_COMBO");
     win_text_combo->setString(msg);
     
     win_text_basicReward = m_pLayer->FindChildObjectByName<UILabel>("win_text_basicReward");
     
     win_icon_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward1");
     win_number_reward1 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward1");
     
     win_icon_reward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward2");
     win_number_reward2 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward2");
     
     win_icon_reward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward3");
     win_number_reward3 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward3");
     //jackniu gongxun icon not used 2014-5-2
     //win_icon_reward4 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward4");
     //win_number_reward4 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward4");
     
     win_text_basicReward->setVisible(false);
     
     msg = Localizatioin::getLocalization("M_ESTIMATE_BASICREWARD");
     win_text_basicReward->setString(msg);
     win_text_basicReward->setColor(ccGREEN);
     
     
     win_icon_reward1->setVisible(false);
     win_number_reward1->setVisible(false);
     
     win_icon_reward2->setVisible(false);
     win_number_reward2->setVisible(false);
     
     win_icon_reward3->setVisible(false);
     win_number_reward3->setVisible(false);
     
    // win_icon_reward4->setVisible(false);
     //win_number_reward4->setVisible(false);
     
     win_text_specialReward = m_pLayer->FindChildObjectByName<UILabel>("win_text_specialReward");
     
     win_frame_specialReward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_specialReward1");
     win_frame_icon_specialReward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_icon_specialReward1");
     
     win_frame_specialReward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_specialReward2");
     win_frame_icon_specialReward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_icon_specialReward2");
     
     win_frame_specialReward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_specialReward3");
     win_frame_icon_specialReward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_icon_specialReward3");
     
     win_frame_specialReward4 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_specialReward4");
     win_frame_icon_specialReward4 = m_pLayer->FindChildObjectByName<UIPicture>("win_frame_icon_specialReward4");
     
     win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
     win_text_leftBackToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_leftBackToTown");
     win_text_next = m_pLayer->FindChildObjectByName<UILabel>("win_text_next");
     win_text_onceMore = m_pLayer->FindChildObjectByName<UILabel>("win_text_onceMore");

     win_onback = m_pLayer->FindChildObjectByName<UIButton>("win_onback");
     win_onleftback = m_pLayer->FindChildObjectByName<UIButton>("win_onleftback");
     win_onnext = m_pLayer->FindChildObjectByName<UIButton>("win_onnext");
     win_onceMore = m_pLayer->FindChildObjectByName<UIButton>("win_onOnceMore");
     
     win_text_specialReward->setVisible(false);
  
     msg = Localizatioin::getLocalization("M_ESTIMATE_SPECIALREWARD");
     win_text_specialReward->setString(msg);
     win_text_specialReward->setColor(ccGREEN);
     
     win_frame_specialReward1->setVisible(false);
     win_frame_icon_specialReward1->setVisible(false);
     
     win_frame_specialReward2->setVisible(false);
     win_frame_icon_specialReward2->setVisible(false);
     
     win_frame_specialReward3->setVisible(false);
     win_frame_icon_specialReward3->setVisible(false);
     
     win_frame_specialReward4->setVisible(false);
     win_frame_icon_specialReward4->setVisible(false);
     
     win_text_backToTown->setVisible(false);
     win_text_leftBackToTown->setVisible(false);
     win_text_next->setVisible(false);
     win_text_onceMore->setVisible(false);

     win_onback->setVisible(false);
     win_onleftback->setVisible(false);
     win_onnext->setVisible(false);
     win_onceMore->setVisible(false);
    
     doOne();
}

void AccountsLayer::openFailurePVP()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  playfailurewav = (CCCallFunc*)CCCallFunc::create(this, callfunc_selector(AccountsLayer::playFailureWav));
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openFailurePVP_core));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,playfailurewav,CCDelayTime::create(1.0f),f,NULL);
    this->runAction(seq);
}
void AccountsLayer::openWinPK()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  playwinwav = (CCCallFunc*)CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinWav));
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openWinPK_core));
    CCCallFunc*  playwinsound = CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinSound));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,playwinwav,CCDelayTime::create(1.0f),f,CCDelayTime::create(1.0f),playwinsound,NULL);
    this->runAction(seq);
}
void AccountsLayer::openFailurePK()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  playfailurewav = (CCCallFunc*)CCCallFunc::create(this, callfunc_selector(AccountsLayer::playFailureWav));
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openFailurePK_core));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,playfailurewav,CCDelayTime::create(1.0f),f,NULL);
    this->runAction(seq);
}
void AccountsLayer::openWinPVP()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  playwinwav = (CCCallFunc*)CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinWav));
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openWinPVP_core));
    CCCallFunc*  playwinsound = CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinSound));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,playwinwav,CCDelayTime::create(1.0f),f,CCDelayTime::create(1.0f),playwinsound,NULL);
    this->runAction(seq);
    
}
void AccountsLayer::openWinPVP2()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  playwinwav = (CCCallFunc*)CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinWav));
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openWinPVP2_core));
    CCCallFunc*  playwinsound = CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinSound));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,playwinwav,CCDelayTime::create(1.0f),f,CCDelayTime::create(1.0f),playwinsound,NULL);
    this->runAction(seq);
    
}
void AccountsLayer::accountWinPvP2(unsigned int bcoin, 
    unsigned int bpvp2_score, unsigned int ccoin, 
    unsigned int cpvp2_score
    )
{
    
    this->bcoin = bcoin;
    this->bpvp2_score = bpvp2_score;
    this->ccoin = ccoin;
    this->cpvp2_score = cpvp2_score;
  
}

void AccountsLayer::accountWinPvP(unsigned int add_coin, unsigned int add_diomand, unsigned int add_exp, unsigned int add_exploit
                                  ,const std::vector<accountReward>& rewards)
{
    
    this->add_coin = add_coin;
    this->add_domiand = add_diomand;
    this->add_exp = add_exp;
    this->add_exploit = add_exploit;

    if(rewards.size() == 1)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        m_itemcnt = 1;
    }
    
    else if(rewards.size() == 2)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        
        m_item2_id = rewards[1].item_id;
        m_item2_num = rewards[1].item_num;
        m_itemcnt = 2;
    }
    
    else if(rewards.size() == 3)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        
        m_item2_id = rewards[1].item_id;
        m_item2_num = rewards[1].item_num;
        
        m_item3_id = rewards[2].item_id;
        m_item3_num = rewards[2].item_num;
        m_itemcnt = 3;
    }
    
    else if(rewards.size() == 4)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        
        m_item2_id = rewards[1].item_id;
        m_item2_num = rewards[1].item_num;
        
        m_item3_id = rewards[2].item_id;
        m_item3_num = rewards[2].item_num;
        
        m_item4_id = rewards[3].item_id;
        m_item4_num = rewards[3].item_num;
        m_itemcnt = 4;
    }
}

void AccountsLayer::accountBattle(unsigned int star,
                                  unsigned int killtime,
                                  unsigned int killnum,
                                  unsigned int killcombo,
                                  unsigned int add_coin, unsigned int add_diamond, unsigned int add_exp, unsigned int add_exploit, unsigned int add_contributions,
                                  const std::vector<accountReward>& rewards)
{
    this->m_star = star;
    this->m_killtime = killtime;
    this->m_killnum = killnum;
    this->m_killcombo = killcombo;
    this->add_coin = add_coin;
    this->add_domiand = add_diamond;
    this->add_exp = add_exp;
    this->add_exploit = add_exploit;
    this->add_contributions = add_contributions;

    if(rewards.size() == 1)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        m_itemcnt = 1;
    }
    else if(rewards.size() == 2)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        
        m_item2_id = rewards[1].item_id;
        m_item2_num = rewards[1].item_num;
        m_itemcnt = 2;
    }
    else if(rewards.size() == 3)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        
        m_item2_id = rewards[1].item_id;
        m_item2_num = rewards[1].item_num;
        
        m_item3_id = rewards[2].item_id;
        m_item3_num = rewards[2].item_num;
        m_itemcnt = 3;
    }
    else if(rewards.size() >= 4)
    {
        m_item1_id = rewards[0].item_id;
        m_item1_num = rewards[0].item_num;
        
        m_item2_id = rewards[1].item_id;
        m_item2_num = rewards[1].item_num;
        
        m_item3_id = rewards[2].item_id;
        m_item3_num = rewards[2].item_num;
        
        m_item4_id = rewards[3].item_id;
        m_item4_num = rewards[3].item_num;
        m_itemcnt = 4;
    }
}


void AccountsLayer::doOne()
{
    CCCallFunc *f1 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::one_1));
    CCCallFunc *f2 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::one_2));
    CCCallFunc *f3 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::one_3));
    CCCallFunc *f4 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::one_4));
    
    CCCallFunc *star1 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::star_1));
    CCCallFunc *star2 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::star_2));
    CCCallFunc *star3 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::star_3));
    
    float delayTime = 0.6f;
    float startime1 = 1.0, startime2 = 0.5, startime3 = 0.5;
    if(m_star == 0)
    {
        startime1 = 0.01f;
        startime2 = 0.01f;
        startime3 = 0.01f;
    }
    if(m_star == 1)
    {
        startime1 = delayTime;
        startime2 = 0.01f;
        startime3 = 0.01f;
    }
    else if(m_star == 2)
    {
        startime1 = delayTime;
        startime2 = delayTime;
        startime3 = 0.01f;
    }
    
    CCCallFunc *baseReward = CCCallFunc::create(this, callfunc_selector(AccountsLayer::basereWard));
    
    CCCallFunc *specialReward1 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::specialReWard1));
    CCCallFunc *specialReward2 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::specialReWard2));
    CCCallFunc *specialReward3 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::specialReWard3));
    CCCallFunc *specialReward4 = CCCallFunc::create(this, callfunc_selector(AccountsLayer::specialReWard4));
    
    float spectime1 = 0.8f, spectime2 = 0.6f, spectime3 = 0.6f, spectime4 = 0.6f;
    if(m_itemcnt ==0)
    {
        spectime1 = 0.01f;
        spectime2 = 0.01f;
        spectime3 = 0.01f;
        spectime4 = 0.01f;
    }
    if(m_itemcnt == 1)
    {
        //spectime1 = delayTime;
        spectime2 = 0.01f;
        spectime3 = 0.01f;
        spectime4 = 0.01f;
    }
    else if(m_itemcnt == 2)
    {
        //spectime1 = delayTime;
        //spectime2 = delayTime;
        spectime3 = 0.01f;
        spectime4 = 0.01f;
    }
    else if(m_itemcnt == 3)
    {
        //spectime1 = delayTime;
        //spectime2 = delayTime;
        //spectime3 = delayTime;
        spectime4 = 0.01f;
    }
    
    CCCallFunc* showbackBtn = CCCallFunc::create(this,  callfunc_selector(AccountsLayer::showBackBtn));
    CCCallFunc* playwinsound = CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinSound));
    CCSequence* seqsound = (CCSequence*)CCSequence::create(CCDelayTime::create(2.0f), playwinsound, NULL);
    
    CCSequence* seqcore = NULL;
    if (LevelManager::sShareInstance()->isCurrentCityDefendLevel() || LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
    {
        seqcore = (CCSequence*)CCSequence::create(f1,
                                                CCDelayTime::create(0.8f),f2,
                                                CCDelayTime::create(0.2f),f3,
                                                CCDelayTime::create(0.2f),f4,                                                          
                                                         
                                                CCDelayTime::create(1.0f),baseReward,
                                                          
                                                CCDelayTime::create(spectime1),specialReward1,
                                                CCDelayTime::create(spectime2),specialReward2,
                                                CCDelayTime::create(spectime3),specialReward3,
                                                CCDelayTime::create(spectime4),specialReward4,
                                                          
                                                CCDelayTime::create(0.8f),showbackBtn,
                                                NULL);
    }
    else
    {
        seqcore = (CCSequence*)CCSequence::create(f1,
                                                CCDelayTime::create(0.8f),f2,
                                                CCDelayTime::create(0.2f),f3,
                                                CCDelayTime::create(0.2f),f4,
                                                          
                                                CCDelayTime::create(startime1),star1,
                                                CCDelayTime::create(startime2),star2,
                                                CCDelayTime::create(startime3),star3,
                                                          
                                                CCDelayTime::create(1.0f),baseReward,
                                                          
                                                CCDelayTime::create(spectime1),specialReward1,
                                                CCDelayTime::create(spectime2),specialReward2,
                                                CCDelayTime::create(spectime3),specialReward3,
                                                CCDelayTime::create(spectime4),specialReward4,
                                                          
                                                CCDelayTime::create(0.8f),showbackBtn,
                                                NULL);
    }
    
    CCSpawn* spawn = (CCSpawn*)CCSpawn::create(seqsound, seqcore,NULL);
    CCCallFunc* playWinBattleWav = (CCCallFunc*)CCCallFunc::create(this, callfunc_selector(AccountsLayer::playWinBattleWav));

    //原始方式
    //CCSequence *seq = (CCSequence*)CCSequence::create(CCDelayTime::create(1.0f),playWinBattleWav,CCDelayTime::create(2.4f), spawn,NULL);

    //应该正确的方式
    //CCSequence *seq = (CCSequence*)CCSequence::create(CCDelayTime::create(2.0f), playWinBattleWav, spawn,NULL);    

    //当前方式
    CCSequence *seq = (CCSequence*)CCSequence::create(playWinBattleWav,CCDelayTime::create(2.4f), spawn,NULL);

    this->runAction(seq);
}

void AccountsLayer::one_1()
{
    winBackGround->setVisible(true);
    if (LevelManager::sShareInstance()->isCurrentCityDefendLevel() || LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
    {
        winDecoration->setVisible(false);
        winDecorationCityDefence->setVisible(true);
    }
    else
    {
        winDecoration->setVisible(true);
        winDecorationCityDefence->setVisible(false);
    }
    
    
    win_text_estimate->setVisible(true);
    win_text_basicReward->setVisible(true);
    if(m_itemcnt>0)
         win_text_specialReward->setVisible(true);
    
    CCNode* s_winBackGround = winBackGround->getCurrentNode();
    float scale = s_winBackGround->getScale();
    s_winBackGround->setScale(0);
    s_winBackGround->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));

    CCNode* s_winDecoration = NULL;
    if (LevelManager::sShareInstance()->isCurrentCityDefendLevel() || LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
    {
        s_winDecoration = winDecorationCityDefence->getCurrentNode();
    }
    else
    {
        s_winDecoration = winDecoration->getCurrentNode();
    }
    scale = s_winDecoration->getScale();
    s_winDecoration->setScale(0);
    s_winDecoration->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));

    
    scale = win_text_estimate->getLabelTTF()->getScale();
    win_text_estimate->getLabelTTF()->setScale(0);
    win_text_estimate->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    
    scale = win_text_basicReward->getLabelTTF()->getScale();
    win_text_basicReward->getLabelTTF()->setScale(0);
    win_text_basicReward->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));

    scale = win_text_specialReward->getLabelTTF()->getScale();
    win_text_specialReward->getLabelTTF()->setScale(0);
    win_text_specialReward->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    
    GameAudioManager::sharedManager()->stopPlayBGM();
    GameAudioManager::sharedManager()->playEffect(350010,false);
}

void AccountsLayer::one_2()
{
    win_text_finishtime->setVisible(true);
    win_number_finishtime->setVisible(true);
    
    char buf[10];
    sprintf(buf, "%d", m_killtime);
    win_number_finishtime->setString(buf);
    
    float scale = win_text_finishtime->getLabelTTF()->getScale();
    win_text_finishtime->getLabelTTF()->setScale(0);
    win_text_finishtime->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    
    scale = win_number_finishtime->getLabelTTF()->getScale();
    win_number_finishtime->getLabelTTF()->setScale(0);
    win_number_finishtime->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    GameAudioManager::sharedManager()->playEffect(350011,false);
}

void AccountsLayer::one_3()
{
    win_text_killnumber->setVisible(true);
    win_number_killnumber->setVisible(true);
    
    char buf[10];
    sprintf(buf, "%d", m_killnum);
    win_number_killnumber->setString(buf);

    float scale = win_text_killnumber->getLabelTTF()->getScale();
    win_text_killnumber->getLabelTTF()->setScale(0);
    win_text_killnumber->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    
    scale = win_number_killnumber->getLabelTTF()->getScale();
    win_number_killnumber->getLabelTTF()->setScale(0);
    win_number_killnumber->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    GameAudioManager::sharedManager()->playEffect(350011,false);
}

void AccountsLayer::one_4()
{
    win_text_combo->setVisible(true);
    win_number_combo->setVisible(true);
    
    char buf[10];
    sprintf(buf, "%d", m_killcombo);
    win_number_combo->setString(buf);

    float scale = win_text_combo->getLabelTTF()->getScale();
    win_text_combo->getLabelTTF()->setScale(0);
    win_text_combo->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    
    scale = win_number_combo->getLabelTTF()->getScale();
    win_number_combo->getLabelTTF()->setScale(0);
    win_number_combo->getLabelTTF()->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    GameAudioManager::sharedManager()->playEffect(350011,false);
}

void AccountsLayer::star_1()
{
    if(m_star <= 0)
        return;
    
    win_icon_star1->setVisible(true);
    CCNode* s_win_icon_star1 = win_icon_star1->getCurrentNode();
    float scale = s_win_icon_star1->getScale();
    s_win_icon_star1->setScale(0);
    s_win_icon_star1->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    GameAudioManager::sharedManager()->playEffect(350012,false);
}

void AccountsLayer::star_2()
{
    if(m_star <= 1)
        return;
    
    win_icon_star2->setVisible(true);
    CCNode* s_win_icon_star2 = win_icon_star2->getCurrentNode();
    float scale = s_win_icon_star2->getScale();
    s_win_icon_star2->setScale(0);
    s_win_icon_star2->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    GameAudioManager::sharedManager()->playEffect(350012,false);
}

void AccountsLayer::star_3()
{
    if(m_star <= 2)
        return;
    win_icon_star3->setVisible(true);
    CCNode* s_win_icon_star3 = win_icon_star3->getCurrentNode();
    float scale = s_win_icon_star3->getScale();
    s_win_icon_star3->setScale(0);
    s_win_icon_star3->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, scale)));
    GameAudioManager::sharedManager()->playEffect(350012,false);
}

void AccountsLayer::basereWard()
{
	bool isPAIvEMode = LevelManager::sShareInstance()->isCurrentGuildInstanceLevel();

	win_icon_reward1->setVisible(true);
	win_number_reward1->setVisible(true);

	win_icon_reward2->setVisible(true);
	win_number_reward2->setVisible(true);

	//jackniu 2014-5-2
	//win_icon_reward4->setVisible(true);
	//win_number_reward4->setVisible(true);
	if(m_itemcnt> 1)
		win_text_specialReward->setVisible(true);
	char buf[10];
	sprintf(buf, "%d", add_coin);
	win_number_reward1->setString(buf);


	if(isPAIvEMode){
		sprintf(buf, "%d", add_contributions);
		win_number_reward2->setString(buf);
	}else{
		sprintf(buf, "%d", add_domiand);
		win_number_reward2->setString(buf);

		win_icon_reward3->setVisible(true);
		win_number_reward3->setVisible(true);
		sprintf(buf, "%d", add_exp);
		win_number_reward3->setString(buf);
	}

	//sprintf(buf, "%d", add_exploit);
	//win_number_reward4->setString(buf);

	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin");
	pAsprite->setImageName(0, "UI/ui_inbattle.png");
	CCPoint pt = CCPointZero;

	int iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_COIN");
	CCSprite* pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
	win_icon_reward1->setSprite(pcoin);

	if(isPAIvEMode)
	{
		iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_CONTRIBUTIONS");
		pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
		win_icon_reward2->setSprite(pcoin);
	}
	else
	{
		iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_FAIRYEXP");
		pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
		win_icon_reward2->setSprite(pcoin);

		iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_EXP");
		pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
		win_icon_reward3->setSprite(pcoin);
	}

	// 多人副本隐藏经验数值
	if (LevelManager::sShareInstance()->isCurrentPPVELevel())
	{
		win_number_reward3->setVisible(false);
		win_icon_reward3->setVisible(false);
	}

	//jackniu gong xun not used
    //iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_EXPLOIT");
   // pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    //win_icon_reward4->setSprite(pcoin);
    
    CCLabelTTF *number = (CCLabelTTF*)win_number_reward1->getLabelTTF();
    labelScale(number, 0, number->getScale());
    
    
    number = (CCLabelTTF*)win_number_reward2->getLabelTTF();
    labelScale(number, 0, number->getScale());
    
    number = (CCLabelTTF*)win_number_reward3->getLabelTTF();
    labelScale(number, 0, number->getScale());

    //number = (CCLabelTTF*)win_number_reward4->getLabelTTF();
    //labelScale(number, 0, number->getScale());
    
    CCNode* node = win_icon_reward1->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_icon_reward2->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_icon_reward3->getCurrentNode();
    nodeScale(node,0,node->getScale());

    //node = win_icon_reward4->getCurrentNode();
    //nodeScale(node,0,node->getScale());
    
    GameAudioManager::sharedManager()->playEffect(350013,false);
}

void AccountsLayer::specialReWard1()
{
    if(m_itemcnt < 1)
        return;
    if(win_frame_specialReward1!= NULL && win_frame_icon_specialReward1 != NULL)
    {
        win_frame_specialReward1->setVisible(true);
        win_frame_icon_specialReward1->setVisible(true);
    
        setSpecialReWardSprite(win_frame_icon_specialReward1, 1);
        CCNode* node = win_frame_icon_specialReward1->getCurrentNode();
        nodeScale(node,0,node->getScale());
    
        node = win_frame_specialReward1->getCurrentNode();
        nodeScale(node,0,node->getScale());
        GameAudioManager::sharedManager()->playEffect(350014,false);
    }
}

void AccountsLayer::specialReWard2()
{
    if(m_itemcnt < 2)
        return;
    
    setSpecialReWardSprite(win_frame_icon_specialReward2, 2);
    win_frame_specialReward2->setVisible(true);
    win_frame_icon_specialReward2->setVisible(true);
    
    CCNode* node = win_frame_icon_specialReward2->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_frame_specialReward2->getCurrentNode();
    nodeScale(node,0,node->getScale());
    GameAudioManager::sharedManager()->playEffect(350014,false);
}

void AccountsLayer::specialReWard3()
{
    if(m_itemcnt < 3)
        return;
    
    setSpecialReWardSprite(win_frame_icon_specialReward3, 3);
    win_frame_specialReward3->setVisible(true);
    win_frame_icon_specialReward3->setVisible(true);
    
    CCNode* node = win_frame_icon_specialReward3->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_frame_specialReward3->getCurrentNode();
    nodeScale(node,0,node->getScale());
    GameAudioManager::sharedManager()->playEffect(350014,false);
}

void AccountsLayer::specialReWard4()
{
    if(m_itemcnt < 4)
        return;
    
    setSpecialReWardSprite(win_frame_icon_specialReward4, 4);
    win_frame_specialReward4->setVisible(true);
    win_frame_icon_specialReward4->setVisible(true);
    
    CCNode* node = win_frame_icon_specialReward4->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_frame_specialReward4->getCurrentNode();
    nodeScale(node,0,node->getScale());
    GameAudioManager::sharedManager()->playEffect(350014,false);
}

void AccountsLayer::showBackBtn()
{
    if (LevelManager::sShareInstance()->isCurrentUndergroundLevel())
    {
        //地下城副本
        int nextUndergroundId = LuaTinkerManager::Get()->callLuaFunc<int>("Script/Instance/UndergroundInstance.lua", "GetNextUnderInstanceId");
        
        if (nextUndergroundId)
        {
            const char* backStr = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
            win_text_leftBackToTown->setString(backStr);

            const char* nextStr = Localizatioin::getLocalization("M_UNDERGROUND_NEXT");
            win_text_next->setString(nextStr);

            win_text_leftBackToTown->setVisible(true);
            win_text_next->setVisible(true);

            win_onleftback->setVisible(true);
            win_onnext->setVisible(true);
        }
        else
        {
            const char* msg = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
            win_text_backToTown->setString(msg);
    
            win_text_backToTown->setVisible(true);
            win_onback->setVisible(true);
        }
        
    }
    else if (LevelManager::sShareInstance()->isCurrentCityDefendLevel())
    {
        const char* msg = Localizatioin::getLocalization("M_CITYDEFEND_BACKTOTOWN");
        win_text_backToTown->setString(msg);
    
        win_text_backToTown->setVisible(true);
        win_onback->setVisible(true);
    }
    else
    {
        const char* msg = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
        win_text_backToTown->setString(msg);
    
        win_text_backToTown->setVisible(true);
        win_onback->setVisible(true);
        // win_onceMore->setVisible(true);

        const char* againMsg = Localizatioin::getLocalization("M_HEROINSTANCE_FIGHTAGAIN");
        win_text_onceMore->setString(againMsg);
        // win_text_onceMore->setVisible(true);
    }
}

void AccountsLayer::labelScale(CCLabelTTF* label, float startscael, float endscale)
{
    label->setScale(startscael);
    label->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, endscale)));
}

void AccountsLayer::nodeScale(CCNode* node, float startscale, float endscale)
{
    node->setScale(startscale);
    node->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.2f, endscale)));
}
void AccountsLayer::setRandItem( unsigned int item_id ,unsigned int num)
{
    m_item1_id = item_id ;
    m_item1_num = num ;

}
void AccountsLayer::setSpecialReWardSprite(UIPicture* pUIPicture, int index)
{
    int item_id, item_num;
    switch (index) {
        case 1:
            item_id = m_item1_id;
            item_num = m_item1_num;
            break;
            
        case 2:
            item_id = m_item2_id;
            item_num= m_item2_num;
            break;
            
        case 3:
            item_id = m_item3_id;
            item_num = m_item3_num;
            break;
            
        case 4:
            item_id = m_item4_id;
            item_num = m_item4_num;
            break;
        case 5 :
            item_id = m_item1_id;
            item_num = m_item1_num;
            break;
        default:
            break;
    }
    
    
       
    CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item_id);           
    //pUIPicture->setSprite(sprite,false);
    
    
    //add tip handler
    ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
    CCSprite* pEmptyItem = CCSprite::create();
    CCSize uiSize = pUIPicture->getCurrentNode()->getContentSize();
    pEmptyItem->setContentSize(CCSize(uiSize.width,uiSize.height));
    CCPoint pt;
    //
     int iFrameId = TXGUI::getResourceIDByName("map_ui_FRAME_BAG_GRID1");
     if(index == 5)
      iFrameId = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_CITYPVP_FIGHTREWARD_FRAME_SPECIALREWARD1_CLICKED");
    CCSprite* pSelectItem = as->getSpriteFromFrame_Middle(iFrameId,0,pt);
    //IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, menu_selector(AccountsLayer::onItemTapped));
    IconButton * pIcon = new IconButton(pSelectItem, NULL, pEmptyItem, this, menu_selector(AccountsLayer::onItemTapped));

    pIcon->autorelease();
    pIcon->SetButtonIndex(index);
    pIcon->setTag(item_id);
    pIcon->setContentSize(pEmptyItem->getContentSize());
    
    pIcon->SetNormalIcon(sprite);
    pUIPicture->getCurrentNode()->addChild(pIcon,10);
    CCSize uiPicutreSize = pUIPicture->getSpriteSize();
    pIcon->setPosition(ccp(pUIPicture->getCurrentNode()->getContentSize().width/2-2,
                           pUIPicture->getCurrentNode()->getContentSize().height/2-2));

    char buf[10];
    sprintf(buf, "%d", item_num);
    pIcon->setItemLabel(buf,20,KJLinXin);    
    
    /////////////////add framePic////////////
    /*CCSprite* framePic = NULL;
    ItemBase* itemInfo = ItemManager::Get()->getItemInfoById(item_id);
    if(itemInfo)
    {
        framePic = ItemManager::Get()->getIconFrame(itemInfo->m_quality);
    }
  
    if(framePic)
    {
        pIcon->setFrameSprite(framePic);
    }
    else
    {
        pIcon->setFrameSprite(NULL);
    }*/
}

void AccountsLayer::closeAccountsLayer()
{
    UIManager::sharedManager()->RemoveUILayout("FightAccounts");
    UIManager::sharedManager()->RemoveUILayout("FightWinPVP");
    UIManager::sharedManager()->RemoveUILayout("FightFailure");
    this->removeFromParentAndCleanup(true);
}

void AccountsLayer::OnBack(CCObject *sender)
{
    CCLOG("on back\n");
    UIManager::sharedManager()->RemoveUILayout("FightAccounts");
    UIManager::sharedManager()->RemoveUILayout("FightWinPVP");
    UIManager::sharedManager()->RemoveUILayout("FightFailure");
     UIManager::sharedManager()->RemoveUILayout("PKFinishWin");
    
    this->removeFromParentAndCleanup(true);
    XLogicManager::sharedManager()->LeaveBattle();
}

void AccountsLayer::onOnceMore(CCObject* sender)
{
    int instanceId = InstanceManager::Get()->getCurInstanceId();
    InstanceInfo* info = InstanceManager::Get()->getOneInstance(instanceId);
    if (info == NULL)
    {
        OnBack(NULL);
        return;
    }
	
    InstanceInfoBonusInfo* bonusInfo = new InstanceInfoBonusInfo();
    GameConfigFromLuaManager::Get()->getInstanceBonusInfo(instanceId, bonusInfo);

    // 检查体力和次数
    USERINFO userInfo = UserData::GetUserInfo();
    int staming = userInfo.m_stamina;

	if(staming < bonusInfo->power)
    {
       // ShowMessage(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"));
        InterfaceManager::Get()->ShowNOEnoughStaminaBox(this, menu_selector(AccountsLayer::enterBuyStaminaPage));
        return;
    }

    if (bonusInfo->max_number <= info->battle_times)
    {
        // 弹出确认提示框
        ShowMessage(Localizatioin::getLocalization("M_CHALLENGETIMESOUT"));
        return;
    }

    XLogicManager::sharedManager()->prepareBattle(instanceId, 0);
}

void AccountsLayer::onGetFairy(CCObject *sender)
{
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","MarketLayer");
    OnBack(NULL);
}

void AccountsLayer::onTrainFairy(CCObject *sender)
{
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","FairyBagLayer");
    OnBack(NULL);
}

void AccountsLayer::onStrenthEquip(CCObject *sender)
{
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","BackPackLayer");
    OnBack(NULL);
}

void AccountsLayer::onBuyDiamond(CCObject *sender)
{
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","ChargeLayer");
    OnBack(NULL);
}

void AccountsLayer::OnPvpItemClicked(CCObject* sender)
{
    CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(sender);
    if(menuItem)
    {
        CCNode* node = menuItem->getParent();
        UIMenu* menu = dynamic_cast<UIMenu*>(node);
        if(menu)
        {
            int item_id = m_item1_id;
            ItemManager::Get()->showItemTipsById(item_id, menu->getTouchPoint());
        }
    }

}

void AccountsLayer::OnLive(CCObject* sender)
{    
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    int battleTag = mLayer->getLayerTagByName("BattleUILayer");
    BattleUILayer* battleUILayer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
    battleUILayer->clearSkillProgressTimer();
    
    if(m_revivalDiamond.empty() && m_revivalNum.empty())
    {
        std::string content = Localizatioin::getLocalization("M_ESTIMATE_NORELIVETIME");;
        if (content != "")
        {
            ShowMessage(content.c_str());
        }
    }
    else
    {
        int revivalDiamand = atoi(m_revivalDiamond.c_str());
        USERINFO user = UserData::GetUserInfo();
        if (revivalDiamand > user.m_diamond)
        {
            std::string content = Localizatioin::getLocalization("M_ESTIMATE_NO_ENOUGHDIAMOND");
            if (content != "")
            {
                ShowMessage(content.c_str());
            }        
            return;
        }
        
        OnlineNetworkManager::sShareInstance()->SendBattlePlayerRevival();
        WaitingLayerManager::Get()->PushOneMessage("CSBattlePlayerRevivalReq");
    }
}

void AccountsLayer::OnNext(CCObject* sender)
{
    int nextUndergroundId = LuaTinkerManager::Get()->callLuaFunc<int>("Script/Instance/UndergroundInstance.lua", "GetNextUnderInstanceId");
        
    if (nextUndergroundId)
    {
        InstanceManager::Get()->setCurInstanceId(nextUndergroundId);
        int instanceId = InstanceManager::Get()->getCurInstanceId();
    
        XLogicManager::sharedManager()->prepareBattle(instanceId,0);
        
        //ResoureUpdate::sharedResoureUpdate()->realTimeUpdate(instanceId);
    }
}

void AccountsLayer::pvpCoinEffect()
{
    char buf[10];
    sprintf(buf, "%d", add_coin);
    win_number_reward1->setString(buf);
    
    sprintf(buf, "%d", add_domiand);
    win_number_reward2->setString(buf);
    
    sprintf(buf, "%d", add_exp);
    win_number_reward3->setString(buf);
    
   // sprintf(buf, "%d", add_exploit);
    //win_number_reward4->setString(buf);
    
    CCLabelTTF *number = (CCLabelTTF*)win_number_reward1->getLabelTTF();
    labelScale(number, 0, number->getScale());
    
    number = (CCLabelTTF*)win_number_reward2->getLabelTTF();
    labelScale(number, 0, number->getScale());
    
    number = (CCLabelTTF*)win_number_reward3->getLabelTTF();
    labelScale(number, 0, number->getScale());
    
    //number = (CCLabelTTF*)win_number_reward4->getLabelTTF();
    //labelScale(number, 0, number->getScale());
    
    CCNode* node = win_icon_reward1->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_icon_reward2->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    node = win_icon_reward3->getCurrentNode();
    nodeScale(node,0,node->getScale());
    
    //node = win_icon_reward4->getCurrentNode();
    //nodeScale(node,0,node->getScale());
}

void AccountsLayer::openFailurePVP_core()
{
    UIManager* manager = UIManager::sharedManager();
    m_pLayer = manager->CreateUILayoutFromFile("UIplist/FightWinPVP.plist", this, "FightWinPVP", true);
    manager->RegisterMenuHandler("FightWinPVP","AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
    
    winBackGround = m_pLayer->FindChildObjectByName<UIPicture>("win_background");
    winDecoration = m_pLayer->FindChildObjectByName<UIPicture>("win_decoration");
    //winDecorationCityDefence = m_pLayer->FindChildObjectByName<UIPicture>("win_decoration_citydefence");
    win_text_basicReward = m_pLayer->FindChildObjectByName<UILabel>("win_text_basicReward");
    
    winDecoration->setVisible(false);

    UILabel * sl_win_special_reward1_text = m_pLayer->FindChildObjectByName<UILabel>("win_special_reward1_text");
    //winDecorationCityDefence->setVisible(false);
    sl_win_special_reward1_text->setVisible(false);
    win_icon_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward1");
    win_number_reward1 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward1");
    
    win_icon_reward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward2");
    win_icon_reward2->setVisible(false);

    win_number_reward2 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward2");
    win_number_reward2->setVisible(false);
    win_icon_reward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward3");
    win_number_reward3 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward3");
    
   // win_icon_reward4 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward4");
   // win_number_reward4 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward4");
    
    const char* msg = Localizatioin::getLocalization("M_ESTIMATE_BASICREWARD");
    win_text_basicReward->setString(msg);
    win_text_basicReward->setColor(ccGREEN);
    
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin");
    pAsprite->setImageName(0, "UI/ui_inbattle.png");
    CCPoint pt = CCPointZero;
    
    int iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_COIN");
    CCSprite* pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    win_icon_reward1->setSprite(pcoin);
    iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_DIAMOND");
    pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    win_icon_reward2->setSprite(pcoin);
    iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_EXP");
    pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
   // win_icon_reward3->setSprite(pcoin);
    //iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_REPUTATION");
   // pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
   // win_icon_reward4->setSprite(pcoin);
     { char buf[10];
    sprintf(buf, "%d", add_coin);
    win_number_reward1->setString(buf);}
    
    {//show REPUTATION
        iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_REPUTATION");
        pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        win_icon_reward3->setSprite(pcoin);
        
        {
            char buf[10];
            snprintf(buf,10, "%d", add_exploit);
            win_number_reward3->setString(buf);
        }
    }
    msg = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
    win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
    win_text_backToTown->setString(msg);
    
    if(m_pLayer)
    {
        UIButton* bt = m_pLayer->FindChildObjectByName<UIButton>("pvpReward1Bt");
        UILabel* label = m_pLayer->FindChildObjectByName<UILabel>("pvpRewardsLabel");
        if(bt)
        {
            bt->setVisible(false);
        }
        if(label)
        {
            label->setVisible(false);
        }
    }

    //pvpCoinEffect();
    GameAudioManager::sharedManager()->playEffect(350008,false);
}
void AccountsLayer::openFailurePK_core()
{
    UIManager* manager = UIManager::sharedManager();
    m_pLayer = manager->CreateUILayoutFromFile("UIplist/PKFinishWin.plist", this, "PKFinishWin", true);
    manager->RegisterMenuHandler("PKFinishWin","PKFinish::OnBack", menu_selector(AccountsLayer::OnBack), this);
    manager->RegisterMenuHandler("PKFinishWin","PKFinish::onFightReward", menu_selector(AccountsLayer::OnBack), this);

    UIPicture * sl_win_pic = m_pLayer->FindChildObjectByName<UIPicture>("win_pic");
    UIPicture * sl_fail_pic = m_pLayer->FindChildObjectByName<UIPicture>("fail_pic");
    //UILabel * sl_text_award = m_pLayer->FindChildObjectByName<UILabel>("text_award");
    UILabel * sl_fightRewardNumCoin = m_pLayer->FindChildObjectByName<UILabel>("fightRewardNumCoin");
    UILabel * sl_win_special_reward1_text = m_pLayer->FindChildObjectByName<UILabel>("win_special_reward1_text");
    UIPicture * sl_win_icon_special_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward1");
    //UIButton *  sl_win_fightReward_bt  =  m_pLayer->FindChildObjectByName<UIButton>("win_fightReward_bt");
    sl_win_pic->setVisible(false);
    //sl_text_award->setString( Localizatioin::getLocalization("M_PVPTOP_FIGHTREWARD_2"));
    sl_win_special_reward1_text->setVisible(false);
    sl_win_icon_special_reward1->setVisible(false);
    //sl_win_fightReward_bt->setVisible(false);
    char strCoin[20]={0};
    snprintf(strCoin,20,"%d",add_coin);
    sl_fightRewardNumCoin->setString(strCoin);
        win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
        win_text_backToTown->setVisible(true);
}

void AccountsLayer::onWinPkAddIcon(CCNode* pUIPicture, int item_id, int item_num, int index)
{
    
    CCSprite* sprite = ItemManager::Get()->getIconSpriteById(item_id);           
    //pUIPicture->setSprite(sprite,false);
    
    
    //add tip handler
    ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin");
    //CCSprite* pEmptyItem = CCSprite::create();
   // CCSize uiSize = pUIPicture->getContentSize();
   // pEmptyItem->setContentSize(CCSize(uiSize.width,uiSize.height));
    CCPoint pt;
    //
    int iFrameId = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_CITYPVP_FIGHTREWARD_FRAME_SPECIALREWARD1_CLICKED");
    CCSprite* pSelectItem = as->getSpriteFromFrame_Middle(iFrameId,0,pt);
    iFrameId = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_CITYPVP_FIGHTREWARD_FRAME_SPECIALREWARD1");
    CCSprite* pBackground = as->getSpriteFromFrame_Middle(iFrameId,0,pt);

    //IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectItem, this, menu_selector(AccountsLayer::onItemTapped));
    IconButton * pIcon = new IconButton(pBackground, sprite, pSelectItem, this, menu_selector(AccountsLayer::onItemTapped));

    pIcon->autorelease();
    pIcon->SetButtonIndex(index);
    pIcon->setTag(item_id);
    pIcon->setContentSize(pUIPicture->getContentSize());
    
  //  pIcon->SetNormalIcon(sprite);
    pUIPicture->addChild(pIcon,10);
    
    pIcon->setPosition(ccp(pUIPicture->getContentSize().width/2,
                           pUIPicture->getContentSize().height/2));

    char buf[10];
    sprintf(buf, "%d", item_num);
    pIcon->setItemLabel(buf,20,KJLinXin);    
    pIcon->setScaleX(-1);  
    /////////////////add framePic////////////
    /*CCSprite* framePic = NULL;
    ItemBase* itemInfo = ItemManager::Get()->getItemInfoById(item_id);
    if(itemInfo)
    {
        framePic = ItemManager::Get()->getIconFrame(itemInfo->m_quality);
    }
  
    if(framePic)
    {
        pIcon->setFrameSprite(framePic);
    }
    else
    {
        pIcon->setFrameSprite(NULL);
    }*/
}

UIPicture * sl_win_icon_special_reward1;
UIPicture * sl_win_icon_special_reward2;
UIPicture * sl_win_icon_special_reward3;

void AccountsLayer::openWinPK_core()
{
    userChoosenCardId = -1;

    UIManager* manager = UIManager::sharedManager();
    m_pLayer = manager->CreateUILayoutFromFile("UIplist/PKFinishWin.plist", this, "PKFinishWin", true);
    manager->RegisterMenuHandler("PKFinishWin","PKFinish::OnBack", menu_selector(AccountsLayer::OnBack), this);
    //manager->RegisterMenuHandler("PKFinishWin","PKFinish::onFightReward", menu_selector(AccountsLayer::OnBack), this);

    UIPicture * sl_win_pic = m_pLayer->FindChildObjectByName<UIPicture>("win_pic");
    UIPicture * sl_fail_pic = m_pLayer->FindChildObjectByName<UIPicture>("fail_pic");
    //UILabel * sl_text_award = m_pLayer->FindChildObjectByName<UILabel>("text_basicReward");
    UILabel * sl_fightRewardNumCoin = m_pLayer->FindChildObjectByName<UILabel>("fightRewardNumCoin");
    UILabel * sl_win_special_reward1_text = m_pLayer->FindChildObjectByName<UILabel>("win_special_reward1_text");
    sl_win_special_reward1_text->setVisible(false);
    
    sl_win_icon_special_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward1");
    sl_win_icon_special_reward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward2");
    sl_win_icon_special_reward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward3");

    UIButton *  sl_win_fightReward_bt  =  m_pLayer->FindChildObjectByName<UIButton>("win_onback");
    sl_win_fightReward_bt->setVisible(false);

    sl_fail_pic->setVisible(false);
    //sl_text_award->setString( Localizatioin::getLocalization("M_PVPTOP_FIGHTREWARD_1"));
    //sl_text_award->setVisible(true);

    char strCoin[20]={0};
    snprintf(strCoin,20,"%d",add_coin);
    sl_fightRewardNumCoin->setString(strCoin);

    
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openWinPK_core_step2));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,f,NULL);
    this->runAction(seq);
    GameAudioManager::sharedManager()->playEffect(350007,false);
}

void AccountsLayer::openWinPK_core_step2()
{
    
    UILabel * sl_win_special_reward1_text = m_pLayer->FindChildObjectByName<UILabel>("win_special_reward1_text");
    sl_win_special_reward1_text->setVisible(true);
    
    addCardPic(sl_win_icon_special_reward1, 1);
    addCardPic(sl_win_icon_special_reward2, 2);
    addCardPic(sl_win_icon_special_reward3, 3);

}
//the real reward is on the first slot. so switch from the first to the
//toIndex slot
void AccountsLayer::switchItemReward(int toIndex)
{
    int tempItemId; 
    int tempItemNum;
    switch(toIndex){
    case 2:
    {
        tempItemId = m_item2_id;
        tempItemNum =m_item2_num;
        m_item2_id = m_item1_id;
        m_item2_num = m_item1_num;
        break;
    }
    case 3:
    {
        tempItemId = m_item3_id;
        tempItemNum =m_item3_num;
        m_item3_id = m_item1_id;
        m_item3_num = m_item1_num;
        break;
    }
    default:
        CCLOG("no need to do the to index is %d", toIndex);
        return;
    }
    m_item1_id = tempItemId;
    m_item1_num = tempItemNum; 
    
}
void AccountsLayer::OnCardClick(CCObject *sender)
{
    CCMenuItemSprite *m_menuItem=dynamic_cast<CCMenuItemSprite *>(sender);
    int id = m_menuItem->getTag();
    userChoosenCardId = id;
    flipTheCard(id, true);
    
}


void AccountsLayer::flipTheCard(int cardId, bool needSwitch)
{
    CCLOG("id is %d",cardId);
    CCNode *actionNode;
    switch(cardId){
    case 1:
        actionNode=sl_win_icon_special_reward1->getCurrentNode();
        actionNode->setUserData(sl_win_icon_special_reward1);
        break;
    case 2:
        actionNode=sl_win_icon_special_reward2->getCurrentNode();
        actionNode->setUserData(sl_win_icon_special_reward2);
        break;
    case 3:
        actionNode=sl_win_icon_special_reward3->getCurrentNode();
        actionNode->setUserData(sl_win_icon_special_reward3);
        break;
    default:
        CCAssert(0, "can not bee here");
        break;
    }
    if(needSwitch)
    switchItemReward(cardId);
    actionNode->removeChildByTag(100);
    CCSprite* cardPic = CCSprite::create("UI/ui_turncard.png");
    actionNode->addChild(cardPic, 10, 10);
    
    cardPic->setPosition(ccp(actionNode->getContentSize().width/2-2,
                          actionNode->getContentSize().height/2-2));
    cardPic->setTag(cardId);
    float duration = 0.4f;
    //fan pai
    CCOrbitCamera * orbit = CCOrbitCamera::create(duration,1, 0, 0, 180, 0, 0);
    //翻转到一半时，调用一个函数
       CCDelayTime *delay = CCDelayTime::create(duration/2);
    //在这里改变CCSprite的displayersprite
    CCCallFuncN *call = CCCallFuncN::create(this,callfuncN_selector(AccountsLayer::picCardHalfCallback));
    CCFiniteTimeAction *seq = CCSequence::create(delay,call,NULL);
    
    CCCallFuncN *call2 = CCCallFuncN::create(this,callfuncN_selector(AccountsLayer::picCardDoneCallback));
    CCFiniteTimeAction *seq2 = CCSequence::create(orbit,call2,NULL);
    CCFiniteTimeAction *spawn = CCSpawn::create(seq2,seq,NULL);
    CCDirector::sharedDirector()->setDepthTest(false);
    cardPic->runAction(spawn);


}

void AccountsLayer::addCardPic(UIPicture *parent, int cardId)
{
    CCSprite* cardPic = CCSprite::create("UI/ui_turncard.png");
    CCMenuItemSprite* m_menuItem = CCMenuItemSprite::create(cardPic,
        cardPic,
        this,
        menu_selector(AccountsLayer::OnCardClick));
    m_menuItem->setTag(cardId);
    UIMenu *m_menu = UIMenu::create(m_menuItem, NULL);
    cardPic->setPosition(CCPointZero);
    
    m_menu->setInitPriority(kCCScrollMenuPriority);
    //m_menu->setPosition(CCPointZero);
    
    m_menu->setPosition(ccp(parent->getCurrentNode()->getContentSize().width/2-2,
                           parent->getCurrentNode()->getContentSize().height/2-2));
    parent->getCurrentNode()->addChild(m_menu,1,100);

    m_menuItem->setScale(1.0);
    //m_menuItem->runAction(CCScaleTo::create(0.2, 1));
    CCDelayTime *delay2;
    CCSequence* seq1;
    CCSequence* seq2;
    if(cardId > 1)
        delay2 = CCDelayTime::create(0.6f*(cardId-1));
    CCCallFuncN *call2 = CCCallFuncN::create(this,
        callfuncN_selector(AccountsLayer::picCardBeat));
    #if 0
    //CCDelayTime *delay = CCScaleTo::create(0.1, 1);
    CCActionInterval*  actionBy = CCScaleBy::create(0.3f, 1.1f);
    //CCActionInterval* move_ease_in = CCEaseIn::create((CCActionInterval*)(actionBy->copy()->autorelease()), 2.5f);
    //CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    CCDelayTime *delay = CCDelayTime::create(2.1f);
    CCActionInterval* scale_ease_out = CCEaseOut::create((CCActionInterval*)(actionBy->copy()->autorelease()), 1.5f);
    CCActionInterval* scale_ease_out_back = scale_ease_out->reverse();
    if(cardId > 1)
        seq1 =(CCSequence*) CCSequence::create(delay2,scale_ease_out,  scale_ease_out_back, delay,NULL);
    else
        seq1 =(CCSequence*) CCSequence::create(scale_ease_out,  scale_ease_out_back, delay,NULL);
            
    //在这里改变CCSprite的displayersprite
    CCActionInterval* rep = CCRepeatForever::create(seq1);
    //sender->runAction(rep);
#endif
    if(cardId > 1)
        seq2 =(CCSequence*) CCSequence::create(delay2, call2  ,NULL);
    if(cardId == 1)
        m_menuItem->runAction(call2);
    else
        m_menuItem->runAction(seq2);
}

//when turn 90 degren, change card pic. add card a item award 
void AccountsLayer::picCardHalfCallback(CCNode* sender)
{

    //UIPicture * parent =(UIPicture *)sender->getUserData();
    // sender->removeAllChildrenWithCleanup(true);
    //setSpecialReWardSprite(parent ,1);
    
    int tag = sender->getTag();
    CCLOG("%s, tag is %d", __FUNCTION__, tag);
    int item_id, item_num;
    
    switch (tag) {
        case 1:
            item_id = m_item1_id;
            item_num = m_item1_num;
            break;
            
        case 2:
            item_id = m_item2_id;
            item_num= m_item2_num;
            break;
            
        case 3:
            item_id = m_item3_id;
            item_num = m_item3_num;
            break;
            
        default:
            break;
    }
    
    onWinPkAddIcon(sender, item_id, item_num, tag);

    //sender->getChildByTag(11)->setScaleX(-1);
    //sender->addChild(CCNode * child, int zOrder)
}

void AccountsLayer::picCardBeat(CCNode* sender)
{
        
       //CCDelayTime *delay = CCScaleTo::create(0.1, 1);
    CCActionInterval*  actionBy = CCScaleBy::create(0.2f, 1.1f);
    //CCActionInterval* move_ease_in = CCEaseIn::create((CCActionInterval*)(actionBy->copy()->autorelease()), 2.5f);
    //CCActionInterval* move_ease_in_back = move_ease_in->reverse();
    CCDelayTime *delay = CCDelayTime::create(1.2f);
    CCActionInterval* scale_ease_out = CCEaseOut::create((CCActionInterval*)(actionBy->copy()->autorelease()), 1.5f);
    CCActionInterval* scale_ease_out_back = scale_ease_out->reverse();
    
    CCSequence* seq1 =(CCSequence*) CCSequence::create(scale_ease_out,  scale_ease_out_back, delay,NULL);
    //在这里改变CCSprite的displayersprite
    CCActionInterval* rep = CCRepeatForever::create(seq1);
    sender->runAction(rep);
}
void AccountsLayer::picCardDoneCallback(CCNode* sender)
{
    
    int tag = sender->getTag();
    CCLOG("%s, tag is %d,userChoosenCardId %d", __FUNCTION__, tag,
        userChoosenCardId);
    
      if(tag == userChoosenCardId)
    {
        //add a flash pic
        CCSprite* flashPic = CCSprite::create("UI/ui_turncard_back.png");
            
        sender->addChild(flashPic, -1, 100);
        
        flashPic->setPosition(ccp(sender->getContentSize().width/2,
                           sender->getContentSize().height/2));
        CCFiniteTimeAction* fade = CCFadeOut::create(1.0f);
           CCSequence*    action2 = (CCSequence*)CCSequence::create(fade,fade->reverse(), NULL);
    

        flashPic->runAction(CCRepeatForever::create(action2));
        
        switch (tag) {
            case 1:
            {
                flipTheCard(2);
                flipTheCard(3);
                break;
            }
            case 2:
            {
                flipTheCard(1);
                flipTheCard(3);
                break;
            }
            case 3:
            {
                flipTheCard(1);
                flipTheCard(2);
                break;
            }
            default:
                break;
        }
    }
    else
    {
        CCDirector::sharedDirector()->setDepthTest(true);
        UIButton *  sl_win_fightReward_bt  =  m_pLayer->FindChildObjectByName<UIButton>("win_onback");
        sl_win_fightReward_bt->setVisible(true);
        
        win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
        win_text_backToTown->setVisible(true);
    }

}


void AccountsLayer::openWinPVP_core()
{
    
    userChoosenCardId = -1;

    UIManager* manager = UIManager::sharedManager();
    m_pLayer = manager->CreateUILayoutFromFile("UIplist/FightWinPVP.plist", this, "FightWinPVP", true);
    manager->RegisterMenuHandler("FightWinPVP","AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
    manager->RegisterMenuHandler("FightWinPVP","AccountsLayer::OnPvpItemClicked", menu_selector(AccountsLayer::OnPvpItemClicked), this);
    
    winBackGround = m_pLayer->FindChildObjectByName<UIPicture>("win_background");
    winDecoration = m_pLayer->FindChildObjectByName<UIPicture>("win_decoration");
    
    //winDecorationCityDefence = m_pLayer->FindChildObjectByName<UIPicture>("win_decoration_citydefence");
    //winDecorationCityDefence->setVisible(false);
    
    win_text_basicReward = m_pLayer->FindChildObjectByName<UILabel>("win_text_basicReward");
    
    UIPicture* failureDecoration = m_pLayer->FindChildObjectByName<UIPicture>("fail_decoration");
    failureDecoration->setVisible(false);
    
    if(m_pLayer)
    {
        UIButton* bt = m_pLayer->FindChildObjectByName<UIButton>("pvpReward1Bt");
        UILabel* label = m_pLayer->FindChildObjectByName<UILabel>("win_special_reward1_text");
        
        UILabel* btLabel = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");

        bt->setVisible(false);
        label->setVisible(false);
        btLabel->setVisible(false);
        #if 0
        if(m_itemcnt > 0)
        {
            bt->setVisible(true);
            label->setVisible(true);
            CCSprite* sprite = ItemManager::Get()->getIconSpriteById(m_item1_id);           
            bt->setIconSprite(sprite);
            if(m_item1_num > 1)
            {
                char amountString[10];
                sprintf(amountString,"%d",m_item1_num);
                CCSize size = bt->getButtonSize();
                bt->setTextLabel(amountString,18,ccWHITE,KJLinXin, ccp(size.width * (0.4f),size.height * (-0.4f)),ccp(1,0));
            }
            /*CCNode* node = CCNode::create();
            ItemBase* itemInfo = ItemManager::Get()->getItemInfoById(m_item1_id);
            if(itemInfo && itemInfo > 0)
            {
                CCSprite* framePic = ItemManager::Get()->getIconFrame(itemInfo->m_quality);
                if(framePic)
                {
                    node->addChild(framePic);
                }
                bt->setFrameSprite(node);
            }*/
        }
        #endif
    }

    win_icon_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward1");
    win_number_reward1 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward1");
    
    win_icon_reward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward2");
    win_number_reward2 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward2");
    
    win_icon_reward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward3");
    win_number_reward3 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward3");
    //win_icon_reward3->setVisible(false);
    //win_number_reward3->setVisible(false);
   // win_icon_reward4 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_reward4");
   // win_number_reward4 = m_pLayer->FindChildObjectByName<UILabel>("win_number_reward4");
    
    const char* msg = Localizatioin::getLocalization("M_ESTIMATE_BASICREWARD");
    win_text_basicReward->setString(msg);
    win_text_basicReward->setColor(ccGREEN);
    
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin");
    pAsprite->setImageName(0, "UI/ui_inbattle.png");
    CCPoint pt = CCPointZero;
    
    int iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_COIN");
    CCSprite* pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    win_icon_reward1->setSprite(pcoin);
    
    
    
    iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_EXP");
    pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
    //win_icon_reward3->setSprite(pcoin);
//    iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_REPUTATION");
  //  pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
  //  win_icon_reward4->setSprite(pcoin);
    
    msg = Localizatioin::getLocalization("M_ESTIMATE_BACKTOTOWN");
    win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
    win_text_backToTown->setString(msg);
    
    //pvpCoinEffect();
    { char buf[10];
    sprintf(buf, "%d", add_coin);
    win_number_reward1->setString(buf);}
    specialReWard1();
    {//show REPUTATION
        iFrameID = TXGUI::getResourceIDByName("map_ui_inbattle_FRAME_ICON_REPUTATION");
        pcoin = pAsprite->getSpriteFromFrame_Middle(iFrameID, 0, pt);
        win_icon_reward3->setSprite(pcoin);
        
        {
            char buf[10];
            snprintf(buf,10, "%d", add_exploit);
            win_number_reward3->setString(buf);
        }
    }
     
    UIButton *    sl_win_fightReward_bt  =  m_pLayer->FindChildObjectByName<UIButton>("win_onback");
    sl_win_fightReward_bt->setVisible(false);
    
    win_text_backToTown = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");
    win_text_backToTown->setVisible(false);
    
    sl_win_icon_special_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward1");
    sl_win_icon_special_reward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward2");
    sl_win_icon_special_reward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward3");
    GameAudioManager::sharedManager()->playEffect(350007,false);

    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openWinPK_core_step2));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,f,NULL);
    this->runAction(seq);
}

void AccountsLayer::openWinPVP2_core()
{
    
    userChoosenCardId = -1;

    UIManager* manager = UIManager::sharedManager();
    m_pLayer = manager->CreateUILayoutFromFile("UIplist/BestFighterOverRewardUICPlusPlus.plist", this, "FightWinPVP", true);
    manager->RegisterMenuHandler("FightWinPVP","AccountsLayer::OnBack", menu_selector(AccountsLayer::OnBack), this);
    manager->RegisterMenuHandler("FightWinPVP","AccountsLayer::OnPvpItemClicked", menu_selector(AccountsLayer::OnPvpItemClicked), this);
    
    if(m_pLayer)
    {
        UIButton* bt = m_pLayer->FindChildObjectByName<UIButton>("win_onback");
        
        UILabel* btLabel = m_pLayer->FindChildObjectByName<UILabel>("win_text_backToTown");

        bt->setVisible(false);
        
        btLabel->setVisible(false);
        
        UILabel* label = m_pLayer->FindChildObjectByName<UILabel>("win_special_reward1_text");
        label->setVisible(false);
    }
    m_pLayer->FindChildObjectByName<UIPicture>("winCoinIcon")->setVisible(ccoin != 0);
    m_pLayer->FindChildObjectByName<UIPicture>("winPointIcon")->setVisible(ccoin != 0);
    m_pLayer->FindChildObjectByName<UILabel>("winRewardText")->setVisible(ccoin != 0);

 
 m_pLayer->FindChildObjectByName<UILabel>("winCoinNum")->setVisible(ccoin != 0);
 m_pLayer->FindChildObjectByName<UILabel>("winPointNum")->setVisible(ccoin != 0);

    
    m_pLayer->FindChildObjectByName<UILabel>("commonCoinNum")->setString(
    MainMenuLayer::GetNumByFormat(bcoin)
    );
    
    m_pLayer->FindChildObjectByName<UILabel>("commonPointNum")->setString(
    MainMenuLayer::GetNumByFormat(bpvp2_score)
    );
   
    m_pLayer->FindChildObjectByName<UILabel>("winCoinNum")->setString(
    MainMenuLayer::GetNumByFormat(ccoin)
    );
                    m_pLayer->FindChildObjectByName<UILabel>("winPointNum")->setString(
    MainMenuLayer::GetNumByFormat(cpvp2_score)
    );

    sl_win_icon_special_reward1 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward1");
    sl_win_icon_special_reward2 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward2");
    sl_win_icon_special_reward3 = m_pLayer->FindChildObjectByName<UIPicture>("win_icon_special_reward3");
    GameAudioManager::sharedManager()->playEffect(350007,false);

    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCCallFunc*  f = CCCallFunc::create(this, callfunc_selector(AccountsLayer::openWinPK_core_step2));
    CCSequence*  seq = (CCSequence*)CCSequence::create(delay,f,NULL);
    this->runAction(seq);
}

void AccountsLayer::onItemTapped(CCObject* sender)
{
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

void AccountsLayer::playWinSound()
{
    const unsigned int VOICE[7] ={0, 3211013, 3211213,  3211313, 3211413, 3211513, 3211613};
    GameAudioManager::sharedManager()->playEffect(VOICE[GameManager::Get()->getHero()->getType()], false);
}

void AccountsLayer::playWinBattleWav()
{
    GameAudioManager::sharedManager()->stopPlayBGM();
    GameAudioManager::sharedManager()->playEffect(350007, false);
}

void AccountsLayer::playWinWav()
{
    GameAudioManager::sharedManager()->stopPlayBGM();
    //GameAudioManager::sharedManager()->playEffect(350007, false);
}

void AccountsLayer::playFailureWav()
{
    GameAudioManager::sharedManager()->stopPlayBGM();
    //GameAudioManager::sharedManager()->playEffect(350008, false);
}

void AccountsLayer::DoRevival()
{ 
    UIManager::sharedManager()->RemoveUILayout("FightAccounts");
    UIManager::sharedManager()->RemoveUILayout("FightWinPVP");
    UIManager::sharedManager()->RemoveUILayout("FightFailure");
    this->removeFromParentAndCleanup(true);
}

void AccountsLayer::getDiamondAndRevivalNum(unsigned int index)
{
    // 去掉功能
    return;
    if(index != 0)
    {
        const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", index, "FromItems");
        string exchangeStr = string(fromItems);
        int dashPos = exchangeStr.find_first_of("/");
        if(dashPos != exchangeStr.npos)
        {
            unsigned int length = exchangeStr.size();
            std::string idStr = exchangeStr.substr(0,dashPos);
            m_revivalDiamond = exchangeStr.substr(dashPos+1,length-1);
            int id = atoi(idStr.c_str());
            ASSERT(id == 416002,"cost is not diamond!");
        }

        const char* pstrType = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", index, "Type2");
        if(pstrType)
        {
            m_revivalNum = pstrType;
        }
    }
    else
    {
        m_revivalNum.clear();
        m_revivalDiamond.clear();
    }
}

void AccountsLayer::setBarValue(int attack, int defence, int live)
{
    if(attack > 100)
        attack = 100;
    
    if(defence > 100)
        defence = 100;
    
    if(live > 100)
        live = 100;
    
    float f_attack = 1.0f*attack/100;
    float f_defence = 1.0f*defence/100;
    float f_live = 1.0f*live/100;
    float scale = UIManager::sharedManager()->getScaleFactor();
    
    const char* attackBarFrame = checkBarFrame(attack);
    const char* defenceBarFrame = checkBarFrame(defence);
    const char* lifeBarFrame = checkBarFrame(live);
    
    //////attack/////
    if(m_pLayer == NULL)
        return;
    
    UIPicture *bar_attack = m_pLayer->FindChildObjectByName<UIPicture>("attackBar");
    //ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui_inbattle.bin");
    //pAsprite->setImageName(0, "UI/ui_inbattle.png");
    
    CCSprite* psprite = AspriteManager::getInstance()->getOneFrame("UI/ui_inbattle.bin",attackBarFrame);
    bar_attack->setSprite(psprite);
    
    bar_attack->getCurrentNode()->setAnchorPoint(ccp(0,0.5));
    bar_attack->setPosition(ccpSub(bar_attack->getPosition(),ccp(bar_attack->getSpriteSize().width/2, 0)));
    bar_attack->setScale(f_attack*scale, scale);

    /////defence/////
    UIPicture *defence_attack = m_pLayer->FindChildObjectByName<UIPicture>("defenceBar");
    psprite = AspriteManager::getInstance()->getOneFrame("UI/ui_inbattle.bin",defenceBarFrame);
    defence_attack->setSprite(psprite);
    
    defence_attack->getCurrentNode()->setAnchorPoint(ccp(0,0.5));
    defence_attack->setPosition(ccpSub(defence_attack->getPosition(),ccp(defence_attack->getSpriteSize().width/2, 0)));
    defence_attack->setScale(f_defence*scale, scale);

    /////live/////
    UIPicture *live_attack = m_pLayer->FindChildObjectByName<UIPicture>("lifeBar");
    psprite = AspriteManager::getInstance()->getOneFrame("UI/ui_inbattle.bin",lifeBarFrame);
    live_attack->setSprite(psprite);

    live_attack->getCurrentNode()->setAnchorPoint(ccp(0,0.5));
    live_attack->setPosition(ccpSub(live_attack->getPosition(),ccp(live_attack->getSpriteSize().width/2, 0)));
    live_attack->setScale(f_live*scale, scale);

    UILabel* attackPercent = m_pLayer->FindChildObjectByName<UILabel>("attackPercent");
    UILabel* defencePercent = m_pLayer->FindChildObjectByName<UILabel>("defencePercent");
    UILabel* lifePercent = m_pLayer->FindChildObjectByName<UILabel>("lifePercent");
    
    char buf[20];
    sprintf(buf, "%d", attack);
    std::string strattack = buf;
    strattack += "%";
    
    sprintf(buf, "%d", defence);
    std::string strdefence = buf;
    strdefence += "%";
    
    sprintf(buf, "%d", live);
    std::string strlife = buf;
    strlife += "%";
    
    attackPercent->setString(strattack.c_str()); 
    defencePercent->setString(strdefence.c_str());
    lifePercent->setString(strlife.c_str());
    
    ccColor3B color_attack = checkBarColor(attack);
    ccColor3B color_defence = checkBarColor(defence);
    ccColor3B color_life = checkBarColor(live);
    
    const char* msg1 = checkBarName(attack);
    const char* msg2 = checkBarName(defence);
    const char* msg3 = checkBarName(live);
    
    UILabel* attack_txt = m_pLayer->FindChildObjectByName<UILabel>("attackLevel");
    UILabel* defence_txt = m_pLayer->FindChildObjectByName<UILabel>("defenceLevel");
    UILabel* life_txt = m_pLayer->FindChildObjectByName<UILabel>("lifeLevel");
    
    attack_txt->setString(msg1);
    defence_txt->setString(msg2);
    life_txt->setString(msg3);
    
    attack_txt->setColor(color_attack);
    defence_txt->setColor(color_defence);
    life_txt->setColor(color_life);
    
   /* UILabel* attackName = m_pLayer->FindChildObjectByName<UILabel>("ui_inbattle_FRAME_FAILSPECIAL_TEXT_ATTACK");
    UILabel* defenceName = m_pLayer->FindChildObjectByName<UILabel>("inbattle_FRAME_FAILSPECIAL_TEXT_DEFENCE");
    UILabel* lifeName = m_pLayer->FindChildObjectByName<UILabel>("inbattle_FRAME_FAILSPECIAL_TEXT_LIFE");
    
    const char* msg = Localizatioin::getLocalization("M_ESTIMATE_ATTACK");
    attackName->setString(msg);
    msg = Localizatioin::getLocalization("M_ESTIMATE_DEFENCE");
    defenceName->setString(msg);
    msg = Localizatioin::getLocalization("M_ESTIMATE_LIFE");
    lifeName->setString(msg);*/
}

// for stamina
void AccountsLayer::enterBagLayer(CCObject* sender)
{
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","BackPackLayer");
    OnBack(NULL);
}

void AccountsLayer::enterMallChipPage(CCObject* sender)
{
    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DeathTip/preOpenLayer.lua","setPreOpenLayerName","MarketLayer");
    OnBack(NULL);
}

ccColor3B AccountsLayer::checkBarColor(int percent)
{
    if(percent < 50)
        return ccc3(255,0,0);
    else if(percent >= 50 && percent < 70)
        return ccc3(254,211,52);
    else
        return ccc3(96,255,0);
}

const char* AccountsLayer::checkBarName(int percent)
{
    if(percent < 50)
        return Localizatioin::getLocalization("M_ESTIMATE_LOW");
    else if(percent >= 50 && percent < 70)
        return Localizatioin::getLocalization("M_ESTIMATE_MIDDLE");
    else
        return Localizatioin::getLocalization("M_ESTIMATE_HIGH");
}

void AccountsLayer::enterBuyStaminaPage(CCObject* sender)
{
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "onStaminaClicked");
}

const char* AccountsLayer::checkBarFrame(int percent)
{
    if(percent < 50)
        return "map_ui_inbattle_FRAME_FAILSPECIAL_BAR_DEFENCE";
    else if(percent >= 50 && percent < 70)
        return "map_ui_inbattle_FRAME_FAILSPECIAL_BAR_ATTACK";
    else
        return "map_ui_inbattle_FRAME_FAILSPECIAL_BAR_LIFE";
}

void AccountsLayer::resetButtonPic(const char* buttonName)
{
    UIButton* attackBt = m_pLayer->FindChildObjectByName<UIButton>(buttonName);
    if(attackBt)
    {
         CCSprite* normalSprite = AspriteManager::getInstance()->getOneFrame("UI/ui_inbattle.bin","map_ui_inbattle_FRAME_FAILSPECIAL_BUTTON_GO");
         CCSprite* clickedSprite = AspriteManager::getInstance()->getOneFrame("UI/ui_inbattle.bin","map_ui_inbattle_FRAME_FAILSPECIAL_BUTTON_GO_CLICKED");
         float scale_x = attackBt->getMenuItemSprite()->getNormalImage()->getContentSize().width / normalSprite->getContentSize().width;
         float scale_y = attackBt->getMenuItemSprite()->getNormalImage()->getContentSize().height / normalSprite->getContentSize().height;
         normalSprite->setScaleX(scale_x);
         normalSprite->setScaleY(scale_y);
         clickedSprite->setScaleX(scale_x);
         clickedSprite->setScaleY(scale_y);
         attackBt->getMenuItemSprite()->setNormalImage(normalSprite);
         attackBt->getMenuItemSprite()->setSelectedImage(clickedSprite);
         attackBt->getMenuItemSprite()->setContentSize(CCSizeMake(normalSprite->getContentSize().width * scale_x,normalSprite->getContentSize().height * scale_y));
    }
}
