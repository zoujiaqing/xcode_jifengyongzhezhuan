#pragma once 

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayout.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define BOSS_FORBID_OTHER_ROLES "BossForbidOherRoles"

class CombatSettingLoader;
class CombatSetting: public CCLayer
    , public CCBMemberVariableAssigner
    , public CCBSelectorResolver
    , public CCNodeLoaderListener
{
public:
    CombatSetting();
    virtual ~CombatSetting();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CombatSetting, create);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();
    virtual void onExit();

    void OnCloseBtnClicked(CCObject* pSender, CCControlEvent event);
    void OnCheckBtnClicked1(CCObject* pSender, CCControlEvent event);
    void OnCheckBtnClicked2(CCObject* pSender, CCControlEvent event);
    void OnCheckBtnClicked3(CCObject* pSender, CCControlEvent event);
    void OnSaveBtnClicked(CCObject* pSender, CCControlEvent event);
    void InitLocalization();

    void OnMsgCallBack(CCObject* obj);

private:
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCLoseBtn,       CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pTitleLabel,     TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel1,         Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel2,         Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel3,         Label3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel4,         Label4);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel5,         Label5);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel6,         Label6);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pCheckOn1,        CheckOn1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCheckBtn1,       CheckBtn1);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pCheckOn2,        CheckOn2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCheckBtn2,       CheckBtn2);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pCheckOn3,        CheckOn3);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCheckBtn3,       CheckBtn3);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pConfirmBtn,     ConfirmBtn);
};

class CombatSettingLoader
    : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CombatSettingLoader, loader);
private:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CombatSetting);
};