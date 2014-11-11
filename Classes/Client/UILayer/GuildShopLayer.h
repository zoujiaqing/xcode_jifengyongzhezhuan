#ifndef _GUILD_SHOP_LAYER_H_
#define _GUILD_SHOP_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"
#include "GuildManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

class GuildShopLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public :
    GuildShopLayer();
    virtual ~GuildShopLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildShopLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

    void onClose(CCObject* sender, CCControlEvent event);
    void OnMsgCallBack(CCObject* obj);
    void ConfigUI();
	virtual void closeLayerCallBack(void);
private :
    void RefreshCell(CCTableViewCell* cell, unsigned int index);
    CCLayer* CreateSingleGuildShopLayer();

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_diamondLabel, DiamondLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_presLabel,    PresLabel);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_contentLayer, ContentLayer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton,  CloseButton);
    CC_SYNTHESIZE_RETAIN(CCTableView*,     m_shopList,     ShopList);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pRefreshLabel,    RefreshLabel);
};

class GuildShopLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildShopLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildShopLayer);
};

class SingleGuildShopLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    SingleGuildShopLayer();
    virtual ~SingleGuildShopLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleGuildShopLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void InitLayer(unsigned int index, GuildShopLayer* layer, GuildShopItems info);

    void onEnter();
    void onBuyClicked(CCObject* sender, CCControlEvent event);
    void onItemsClicked(CCObject* sender, CCControlEvent event);
	void onGetWayCallBack(CCObject* sender);
private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buyButton,   BuyButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pItemClickBtn,   ItemClickBtn);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_itemFrame,   ItemFrame);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_costItem,    CostItem);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_costAmount,  CostAmount);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_tipLabel,    TispLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItemName,   ItemName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItemAmount, ItemAmount);
    CC_SYNTHESIZE_RETAIN(CCScale9Sprite*,  m_pNormalTag,  NormalTag);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pSpecialTag, SpecialTag);
	CC_SYNTHESIZE(int, m_curItemId, ItemId);
    int m_shopIndex;
};

class SingleGuildShopLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleGuildShopLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleGuildShopLayer);
};

#endif