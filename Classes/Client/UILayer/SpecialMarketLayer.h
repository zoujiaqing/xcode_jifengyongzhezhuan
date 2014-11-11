#ifndef _SPECIAL_MARKET_LAYER_H_
#define _SPECIAL_MARKET_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MarketManager.h"
#include "ObserverProtocol.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

class SpecialMarketLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
    , public TXGUI::ObserverProtocol
{
public :
    SpecialMarketLayer();
    virtual ~SpecialMarketLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SpecialMarketLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

    virtual void onBroadcastMessage(BroadcastMessage* msg);
    void UpdateShoppingInfo();

private :
    void RefreshCell(CCTableViewCell* cell, unsigned int idx);
    void RefreshShopItemList();
    CCLayer* CreateSingleSpecMarket(unsigned int index);
    void SendGetShopTimes();
    void SendGetShopPrice();

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_diamondLabel,  DiamondLabel);
    CC_SYNTHESIZE_RETAIN(CCTableView*, m_shopItemList,  ShopItemList);
    CC_SYNTHESIZE_RETAIN(CCLayer*,     m_shopItemLayer, ShopListLayer);

    std::vector<MarketData> m_vtDatas;
};

class SpecialMarketLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SpecialMarketLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SpecialMarketLayer);
};

class SingleSpecialMarketLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    SingleSpecialMarketLayer();
    virtual ~SingleSpecialMarketLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleSpecialMarketLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    virtual void onEnter();

    void InitLayer(MarketData data);
    void onClickedItem(CCObject* object, CCControlEvent event);
    void onBuyItem(CCObject* object, CCControlEvent event);

    void UpdateShoppingInfo();

private :
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_itemFrame,          ItemFrame);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_nameLabel,          NameLabel);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_costItemSprite,     CostItemSprite);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_priceLabel,         Price);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_costItemNext,       CostItemNext);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_priceNextLabel,     PriceNextLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_tipLabel,           TipLabel);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buyBtn,             BuyBtn);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_redLine,            RedLine);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_discountSprite,     DiscountSprite);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_discountLabel,      DiscountLabel);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_itemBtn,            ItemBtn);

    MarketData m_itemData;
};

class SingleSpecialMarketLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleSpecialMarketLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleSpecialMarketLayer);
};

#endif