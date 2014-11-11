#ifndef _FIRST_CHARGE_LAYER_H_
#define _FIRST_CHARGE_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class FirstChargeLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    FirstChargeLayer();
    virtual ~FirstChargeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(FirstChargeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onCloseBtn(CCObject* object, CCControlEvent event);
    void onExtraBtn(CCObject* object, CCControlEvent event);

    void onClickedBtn1(CCObject* object, CCControlEvent event);
    void onClickedBtn2(CCObject* object, CCControlEvent event);
    void onClickedBtn3(CCObject* object, CCControlEvent event);
    void onIconBtnClicked(CCObject* object, CCControlEvent event);
    void onGetBonus(CCObject* object, CCControlEvent event);
    void InitChargeList();

    void setGetBtnEnable();

private :
    void ParseShopData();
    void ClickBtn(unsigned int index);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn, CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_extraBtn, ExtraBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_iconBtn,  IconBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_getBtn,   GetBtn);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_chargeBtn1, ChargeBtn1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_chargeBtn2, ChargeBtn2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_chargeBtn3, ChargeBtn3);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_label1,     Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_label2,     Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_label3,     Label3);

    std::vector<CCLayer*> m_vtChargeLayers;
    std::vector<int>      m_productList;
};

class FirstChargeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(FirstChargeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(FirstChargeLayer);
};

#endif