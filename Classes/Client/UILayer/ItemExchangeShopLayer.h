#pragma once 

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayout.h"
#include "ItemManager.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define ITEM_EXCHANGE_TYPE 16

class SingleItemExchangeShopLoader;
class SingleItemExchangeShop;
class ItemExchangeShopLayerLoader;
class ItemExchangeShopLayer: public CCLayer
    , public CCBMemberVariableAssigner
    , public CCBSelectorResolver
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public:
    ItemExchangeShopLayer();
    virtual ~ItemExchangeShopLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ItemExchangeShopLayer, create);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();
    virtual void onExit();

    // tableview source
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view){};
    void scrollViewDidZoom(CCScrollView* view){};

    void InitData();
    void InitMultiLanguage();
private:
    SingleItemExchangeShop* createSingleItemExchangeShop();
    void OnCloseBtnClicked(CCObject* pSender, CCControlEvent event);
    void AddTableView(CCObject* obj);
    void refreshCell(CCTableViewCell* cell, int idx);

private:
    // multi language
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTitleLabel,	    TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel1,	        Label1);

    // button
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCloseBtn,     CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCLayer*,      m_pTableViewLayer,	 TableViewLayer);
    CC_SYNTHESIZE_RETAIN(CCTableView*,  m_pTableView,	     TableView);
    CC_SYNTHESIZE_RETAIN(CCLayer*,      m_pSingleContent,    SingleContent);
	
    std::vector<ExchangeItem*> m_vecExchangeItems;
};

class ItemExchangeShopLayerLoader
    : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ItemExchangeShopLayerLoader, loader);
private:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ItemExchangeShopLayer);
};

///////////////////////////////////////////////////////////////////
class SingleItemExchangeShop
    : public CCLayer
    , public CCBMemberVariableAssigner
    , public CCBSelectorResolver
    , public CCNodeLoaderListener
{
public:
    SingleItemExchangeShop();
    virtual ~SingleItemExchangeShop();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleItemExchangeShop, create);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    void InitLayer(ExchangeItem* item);

	void onGetWayCallBack(CCObject* pSender);
    void OnExchangeBtnClicked(CCObject* pSender, CCControlEvent event);
    void OnItem1Clicked(CCObject* pSender, CCControlEvent event);
    void OnItem2Clicked(CCObject* pSender, CCControlEvent event);
    void OnItem3Clicked(CCObject* pSender, CCControlEvent event);
    void OnItem4Clicked(CCObject* pSender, CCControlEvent event);

    int getItemId(std::string Itemstr);
    int getItemCount(std::string Itemstr);

    void AddItemIconAndCount(int itemId, CCLayer* layer, int Count, int tag);
private:
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel1,	           Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pLabel2,	           Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItem2Num,	           Item2Num);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItem3Num,	           Item3Num);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItem4Num,	           Item4Num);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pExchangeBtn,         ExchangeBtn);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pItemLayer1,          ItemLayer1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pItemBtn1,            ItemBtn1);
	CC_SYNTHESIZE(int, m_curItemId, ItemId);
    CC_SYNTHESIZE(int, m_curExchangeId, CurExchangeId);

    std::vector<CCLayer*> m_vecItemLayer;
    std::vector<CCControlButton*> m_vecItemBtn;
    std::vector<CCLabelTTF*> m_vecItemNum;
};

class SingleItemExchangeShopLoader
    : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleItemExchangeShopLoader, loader);
private:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleItemExchangeShop);
};