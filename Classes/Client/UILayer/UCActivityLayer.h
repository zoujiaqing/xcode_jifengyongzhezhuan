#ifndef _UC_ACTIVITY_LAYER_H_
#define _UC_ACTIVITY_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"
#include "ObserverProtocol.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

class UCActivityLayer:
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public ObserverProtocol
{
public :
    UCActivityLayer();
    virtual ~UCActivityLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(UCActivityLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onClose(CCObject* sender, CCControlEvent e);
    void onBonusClick1(CCObject* sender, CCControlEvent e);
    void onBonusClick2(CCObject* sender, CCControlEvent e);
    void onBonusClick3(CCObject* sender, CCControlEvent e);

    void onOpenVip(CCObject* sender, CCControlEvent e);
    void onGetBonus(CCObject* sender, CCControlEvent e);

    void onItemClick1(CCObject* sender, CCControlEvent e);
    void onItemClick2(CCObject* sender, CCControlEvent e);
    void onItemClick3(CCObject* sender, CCControlEvent e);
    void onItemClick4(CCObject* sender, CCControlEvent e);
    void onItemClick5(CCObject* sender, CCControlEvent e);
    void onItemClick6(CCObject* sender, CCControlEvent e);
    void onItemClick7(CCObject* sender, CCControlEvent e);

    virtual void onBroadcastMessage(BroadcastMessage* msg);
    void RefreshLayer();
    void SendOpenUCVip();

private :
    void InitBonusItemLayer();
    void InitLayer(int type);
    void ShowItemTip(int i, CCObject* object);

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn,      CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_bonusButton1,  BonusButton1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_bonusButton2,  BonusButton2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_bonusButton3,  BonusButton3);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_openVipButton, OpenVipButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_getButton,     GetButton);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem1,   ButtonItem1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem2,   ButtonItem2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem3,   ButtonItem3);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem4,   ButtonItem4);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem5,   ButtonItem5);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem6,   ButtonItem6);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buttonItem7,   ButtonItem7);

    CC_SYNTHESIZE_RETAIN(CCSprite*, m_bonus1,    BonusSprite1);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_bonus2,    BonusSprite2);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_bonus3,    BonusSprite3);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_bonus1_h,  BonusSprite1H);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_bonus2_h,  BonusSprite2H);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_bonus3_h,  BonusSprite3H);

    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_itemLayer1, ItemLayer1);
    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_itemLayer2, ItemLayer2);
    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_itemLayer3, ItemLayer3);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label1,   Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label2,   Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label3,   Label3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label4,   Label4);

    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_itemLayer,  ItemLayer);

    CC_SYNTHESIZE(int, m_currentType, CurrentType);

    std::vector<CCSprite*> m_vtBonusItems;
};

class UCActivityLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UCActivityLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(UCActivityLayer);
};

#endif