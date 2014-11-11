#ifndef _MYSTERY_LAYER_H_
#define _MYSTERY_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ShopUILayer.h"
#include "TXGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MysteryLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
    , public TXGUI::ObserverProtocol
{
public :
    MysteryLayer();
    virtual ~MysteryLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MysteryLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    virtual void onEnter();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void InitLayer(std::vector<shopItem> vtItems);
    void buyItem(int index, bool suc);
    void InitAttrib();
    void InitTime(float f);
    virtual void onBroadcastMessage(BroadcastMessage* msg);

    void UpdateTimes();

    void onRefresh(CCObject* object, CCControlEvent event);
    void TryRefreshList();

private :
    std::string GetTimeStr(long _time);
    void RefreshCell(CCTableViewCell* cell, unsigned int idx);
    void RefreshItemList();
    CCLayer* CreateSingleShopLayer(unsigned int index);
    void InitNote();

    CC_SYNTHESIZE_RETAIN(CCTableView*, m_shopList,       ShopList);
    CC_SYNTHESIZE_RETAIN(CCLayer*,     m_shopListLayer,  ShopListLayer);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_timeLabel,      TimeLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_diamond,        Diamond);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_hunLabel,       HunLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_cardLabel,      CardLabel);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_refreshbtn, RefreshBtn);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_descLabel,      DescLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_desc2Label,     Desc2Label);
    CC_SYNTHESIZE_RETAIN(CCNode*,      m_mysteryNote,    MysteryNote);

    std::vector<shopItem> m_shopItems;
};

class MysteryLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MysteryLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MysteryLayer);
};

class SingleShopListLayer :
    public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    SingleShopListLayer();
    virtual ~SingleShopListLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleShopListLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onClickBuyItem(CCObject* object, CCControlEvent event);
    void onClickedItem(CCObject* object, CCControlEvent event);
	void onFairyDetailClicked(CCObject* object);
	void onGetWayCallBack(CCObject* object);

    void InitLayer(shopItem itemInfo);
    virtual void onEnter();

    void OnSelectDecompose(CCObject* sender);
    void OnSelectCancel(CCObject* sender);

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_nameLabel,   NameLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_priceLabel,  PriceLabel);
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_itemFrame,   ItemFrame);
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_costItem,    CostItem);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buyBtn, BuyBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_itemBtn, ItemBtn);

    shopItem m_itemInfo;
	unsigned int m_fairyId;
};

class SingleShopListLayerLoader : 
    public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleShopListLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleShopListLayer);
};

#endif