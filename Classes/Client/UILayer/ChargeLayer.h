#ifndef  _CHARGE_LAYER_H_
#define _CHARGE_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ObserverProtocol.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

struct GiftBonusInfo
{
    int item_id;
    int amount;

    GiftBonusInfo()
    {
        item_id = 0;
        amount = 0;
    }
};

class OneShopItemInfo;
class ChargeLayer:
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
    , public ObserverProtocol
{
public :
    ChargeLayer();
    virtual ~ChargeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ChargeLayer, create);

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

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onClose(CCObject* object, CCControlEvent event);
    void onClickedBtn(CCObject* object, CCControlEvent event);
    void onBuyBtnClicked(CCObject* object, CCControlEvent event);

    void onSelectItem1(CCObject* object, CCControlEvent event);
    void onSelectItem2(CCObject* object, CCControlEvent event);
    void onSelectItem3(CCObject* object, CCControlEvent event);
    void onSelectItem4(CCObject* object, CCControlEvent event);

    void initLayerOpen(int openTag);
    void updateChargeList();

    void onClickLeft(CCObject* object,  CCControlEvent event);
    void onClickRight(CCObject* object, CCControlEvent event);

    virtual void onBroadcastMessage(BroadcastMessage* msg);

private :
    void selectItem(int index, CCObject* object);
    void AdjustWindow(CCTouch* pTouch);
    void AdjustWindowTo(int index, bool flag = true);
    void InitLayer();
    void RefreshChargeList();
    void RefreshVipList();
    void ParseShopData();
    void RefreshCell(CCTableViewCell* cell, unsigned int idx);
    CCLayer* CreateSingleChargeLayer(unsigned int index);
    void ParseGiftItem(std::string strItems, std::vector<GiftBonusInfo>& vtGifts);

    CC_SYNTHESIZE_RETAIN(CCTableView*, m_chargeList,      ChargeList);
    CC_SYNTHESIZE_RETAIN(CCLayer*,     m_viewLayer,      ViewLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,     m_nextLayer,      NextLayer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn,   CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_btn,        Btn);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_currentDiamond, CurrentDiamond);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_nextDiamond,    NextDiamond);
    CC_SYNTHESIZE_RETAIN(CCSprite*,    m_currentVipLv,   CurrentVipLv);
    CC_SYNTHESIZE_RETAIN(CCSprite*,    m_nextVipLv,      NextVipLv);
    CC_SYNTHESIZE_RETAIN(CCSprite*,    m_currentVipLv1,  CurrentVipLv1);
    CC_SYNTHESIZE_RETAIN(CCSprite*,    m_nextVipLv1,     NextVipLv1);
    CC_SYNTHESIZE_RETAIN(CCSprite*,    m_processSprite,  ProcessSprite);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_buyBtn,     BuyBtn);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_giftLabel,      GiftLabel);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_leftBtn,    LeftBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_rightBtn,   RightBtn);

    CC_SYNTHESIZE_RETAIN(CCLayer*,     m_vipContentLayer, VipContentLayer);
    CC_SYNTHESIZE_RETAIN(CCTableView*, m_vipContentList,  VipContentList);

    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_sonVipLayer,     SonVipLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_sonChargeLayer,  SonChargeLayer);

    CC_SYNTHESIZE(int, m_currentVipIndex, CurrentVipIndex);
    CC_SYNTHESIZE(int, m_currentOpenTag,  CurrentOpenTag);
    CCPoint m_startPt;

    std::vector<CCLayer*> m_vtGiftLayers;
    std::vector<OneShopItemInfo> m_vtShopInfo;
    std::vector<GiftBonusInfo> m_vtItemsInfo;
};

class ChargeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ChargeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ChargeLayer);
};

class SingleChargeLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
	, public TXGUI::ObserverProtocol
{
public :
    SingleChargeLayer();
    virtual ~SingleChargeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleChargeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    virtual void onEnter();
    void InitLayer(OneShopItemInfo info, unsigned int index);
	void showNormalProduct(OneShopItemInfo info);
	void showMouthCardProduct(OneShopItemInfo info);
	void refreshMouthCardInfo();
    void pay();
protected:
	virtual void onBroadcastMessage(BroadcastMessage* msg);

private :
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_recommandSprite, RecommandSprite);
	CC_SYNTHESIZE_RETAIN(CCSprite*,   m_hotSprite, HotSprite);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label1, Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label2, Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label3, Label3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label4, Label4);
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_itemFrame, ItemFrame);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_btn, Btn);

    CC_SYNTHESIZE(int, m_productId, ProductId);
    CC_SYNTHESIZE(int, m_index,     Index);
	CC_SYNTHESIZE(int, m_productType,     ProductType);
	CC_SYNTHESIZE(int, m_MCardTotalDiamond,     MCardTotalDiamond);
};

class SingleChargeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleChargeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleChargeLayer);
};

class VipContentLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    VipContentLayer();
    virtual ~VipContentLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(VipContentLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    
    void InitLayer(unsigned int index);

private :
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_vipLv1, VipLv1);
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_vipLv2, VipLv2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label,  Label);
};

class VipContentLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(VipContentLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(VipContentLayer);
};

class VipContentDataSource :
    public CCTableViewDataSource
{
public :
    VipContentDataSource();
    virtual ~VipContentDataSource();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    void RefreshCell(CCTableViewCell* cell, unsigned int index);
    VipContentLayer* CreateSingleVipContentLayer(unsigned int index);
};

#endif