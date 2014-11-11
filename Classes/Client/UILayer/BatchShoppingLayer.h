#ifndef _BATCH_SHOPPING_LAYER_H_
#define _BATCH_SHOPPING_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MarketManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum BatchShoppingType
{
    BatchShoppingType_SpecialMarket = 1,
    BatchShoppingType_VipMarket = 2,
};

class BatchShoppingLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    BatchShoppingLayer();
    virtual ~BatchShoppingLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BatchShoppingLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onConfirm(CCObject* object, CCControlEvent event);
    void onRefuseLayer(CCObject* object, CCControlEvent event);

    void InitLayer(MarketData mData, BatchShoppingType type);

    void onPlus10BtnClicked(CCObject* object, CCControlEvent event);
    void onPlus1BtnClicked(CCObject* object, CCControlEvent event);
    void onAdd10BtnClicked(CCObject* object, CCControlEvent event);
    void onAdd1BtnClicked(CCObject* object, CCControlEvent event);

    void onClickItem(CCObject* object, CCControlEvent event);

private :
    void RefreshCurrentAmount(int amount);

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_plus10Button,  Plus10Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_plus1Button,   Plus1Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_add10Button,   Add10Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_add1Button,    Add1Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_confirmButton, ConfirmButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_refuseButton,  RefuseButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_itemBtn,       ItemBtn);

    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_itemFrame,     ItemFrame);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_nameLabel,     NameLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_priceLabel,    PriceLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_totalPrice,    TotalPrice);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_amountLabel,   AmountLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_maxTimesLabel, MaxTimesLabel);

    MarketData m_marketData;
    BatchShoppingType m_type;

    CC_SYNTHESIZE(int, m_currentAmount, CurrentAmount);
};

class BatchShoppingLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BatchShoppingLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BatchShoppingLayer);
};

#endif