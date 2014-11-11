#include "OnlineNetworkManager.h"
#include "CombatSetting.h"
#include "CallNativeFuncManager.h"
#include "Localization.h"
#include "UserData.h"
#include "XLogicManager.h"
#include "MainMenuLayer.h"

CombatSetting::CombatSetting()
    : m_pCLoseBtn(NULL),    
      m_pTitleLabel(NULL),  
      m_pLabel1(NULL),      
      m_pLabel2(NULL),      
      m_pLabel3(NULL),      
      m_pLabel4(NULL),      
      m_pLabel5(NULL),      
      m_pLabel6(NULL),      
      m_pCheckOn1(NULL),     
      m_pCheckBtn1(NULL),    
      m_pCheckOn2(NULL),     
      m_pCheckBtn2(NULL),    
      m_pCheckOn3(NULL),     
      m_pCheckBtn3(NULL),    
      m_pConfirmBtn(NULL)
{

}
CombatSetting::~CombatSetting()
{
    setCloseBtn(NULL);
    setTitleLabel(NULL);
    setLabel1(NULL);
    setLabel2(NULL);
    setLabel3(NULL);
    setLabel4(NULL);
    setLabel5(NULL);
    setLabel6(NULL);
    setCheckOn1(NULL);
    setCheckBtn1(NULL);
    setCheckOn2(NULL);
    setCheckBtn2(NULL);
    setCheckOn3(NULL);
    setCheckBtn3(NULL);
    setConfirmBtn(NULL);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CombatSettingNotice");
}

SEL_MenuHandler CombatSetting::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler CombatSetting::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", CombatSetting::OnCloseBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "save", CombatSetting::OnSaveBtnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "check1", CombatSetting::OnCheckBtnClicked1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "check2", CombatSetting::OnCheckBtnClicked2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "check3", CombatSetting::OnCheckBtnClicked3);

    return NULL;
}

bool CombatSetting::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",     CCControlButton*, m_pCLoseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChechBtn1",   CCControlButton*, m_pCheckBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheckOn1",    CCSprite*,        m_pCheckOn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChechBtn2",   CCControlButton*, m_pCheckBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheckOn2",    CCSprite*,        m_pCheckOn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChechBtn3",   CCControlButton*, m_pCheckBtn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheckOn3",    CCSprite*,        m_pCheckOn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmBtn", CCControlButton*, m_pConfirmBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleLabel", CCLabelTTF*,      m_pTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable1",     CCLabelTTF*,      m_pLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable2",     CCLabelTTF*,      m_pLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable3",     CCLabelTTF*,      m_pLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable4",     CCLabelTTF*,      m_pLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable5",     CCLabelTTF*,      m_pLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLable6",     CCLabelTTF*,      m_pLabel6);
    

    return true;
}

void CombatSetting::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    setTouchEnabled(true);
    m_pCLoseBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pCheckBtn1->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pCheckBtn2->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pCheckBtn3->setTouchPriority(kCCLayerMenuTouchPriority);
    m_pConfirmBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CombatSetting::OnMsgCallBack), "CombatSettingNotice", NULL);

    XLogicManager::sharedManager()->reqPlayerAttributes(PB_ATTR_WBOSS_VIEW_NUMBER);
    
    InitLocalization();
}

void CombatSetting::OnMsgCallBack(CCObject* obj)
{
    USERINFO info = UserData::GetUserInfo();
    int value = info.m_worldboss_forbid_roles;
    // 0 表示不屏蔽 3表示屏蔽（其中3为可以看到别的玩家的数量）
    if(value == 0)
    {
        m_pCheckOn3->setVisible(false);
    }
    else if(value == 3)
    {
        m_pCheckOn3->setVisible(true);
    }
}

void CombatSetting::InitLocalization()
{
    m_pTitleLabel->setString(Localizatioin::getLocalization("M_WORLDBOSS_TITLE"));
    m_pLabel1->setString(Localizatioin::getLocalization("M_COSTDIAMOND_AUTO"));
    m_pLabel2->setString(Localizatioin::getLocalization("M_AUTO"));
    m_pLabel3->setString(Localizatioin::getLocalization("M_WORLDBOSS_TEXT3"));
    m_pLabel4->setString(Localizatioin::getLocalization("M_WORLDBOSS_TIPS_1"));
    m_pLabel5->setString(Localizatioin::getLocalization("M_WORLDBOSS_TEXT4"));
    m_pLabel6->setString(Localizatioin::getLocalization("M_WORLDBOSS_TEXT5"));
    m_pConfirmBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_WORLDBOSS_SCREEN")), CCControlStateNormal);
}

bool CombatSetting::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CombatSetting::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
}

void CombatSetting::onEnter()
{
    CCLayer::onEnter();
}

void CombatSetting::onExit()
{
    CCLayer::onExit();
    this->stopAllActions();
    this->unscheduleAllSelectors();
}

void CombatSetting::OnCloseBtnClicked(CCObject* pSender, CCControlEvent event)
{
    CallNativeFuncManager::getInstance()->ShowCombatSettingLayer(false);
}
void CombatSetting::OnCheckBtnClicked1(CCObject* pSender, CCControlEvent event)
{
    if(m_pCheckOn1->isVisible() == false)
    {
        m_pCheckOn1->setVisible(true);
        m_pCheckOn2->setVisible(false);
    }
    else
    {
        m_pCheckOn1->setVisible(false);
    }
}

void CombatSetting::OnCheckBtnClicked2(CCObject* pSender, CCControlEvent event)
{
    if(m_pCheckOn2->isVisible() == false)
    {
        m_pCheckOn2->setVisible(true);
        m_pCheckOn1->setVisible(false);
    }
    else
    {
        m_pCheckOn2->setVisible(false);
    }
}
void CombatSetting::OnCheckBtnClicked3(CCObject* pSender, CCControlEvent event)
{
    if(m_pCheckOn3->isVisible() == false)
    {
        m_pCheckOn3->setVisible(true);
    }
    else
    {
        m_pCheckOn3->setVisible(false);
    }
}

void CombatSetting::OnSaveBtnClicked(CCObject* pSender, CCControlEvent event)
{
//    // 1 select
//    bool flag1 = m_pCheckOn1->isVisible();
//    if(flag1 == true)
//    {
//        // 1200009
//        OnlineNetworkManager::sShareInstance()->sendSetSlotAttributeMessage(9, 1);
//    }
//    else
//    {
//        OnlineNetworkManager::sShareInstance()->sendSetSlotAttributeMessage(9, 0);
//    }
//
//    // 2 select
//    bool flag2 = m_pCheckOn2->isVisible();
//    if(flag2 == true)
//    {
//
//    }
//    else
//    {
//
//    }

    // 3 select
    bool flag3 = m_pCheckOn3->isVisible();
    if(flag3 == true)
    {
        // 3 forbid
        OnlineNetworkManager::sShareInstance()->sendSetSlotAttributeMessage(8, 3);
    }
    else
    {
        // send 0 open
        OnlineNetworkManager::sShareInstance()->sendSetSlotAttributeMessage(8, 0);
    }

    CallNativeFuncManager::getInstance()->ShowCombatSettingLayer(false);
    ShowMessage(Localizatioin::getLocalization("M_WORLDBOSS_SCREENHINT"));
}