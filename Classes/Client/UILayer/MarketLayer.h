#ifndef _MARKET_LAYER_H_
#define _MARKET_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "TutorialBase.h"
#include "ObserverProtocol.h"
#include "MarketManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum MarketLayerTag
{
    Tag_Sprite  = 0,
    Tag_Special = 1,
    Tag_Vip     = 2,
    Tag_Mystery = 3,
};

class MarketUILayer;
class MarketLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public ObserverProtocol
{
public :
    MarketLayer();
    virtual ~MarketLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MarketLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onSpriteClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onSpecialClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onVipClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onMysteryClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onChargeClicked(CCObject* sender, CCControlEvent pControlEvent);

    void UpdateShoppingInfo();
    void InitOpenLayer(int tag);

    virtual void onBroadcastMessage(BroadcastMessage* msg);
	virtual void closeLayerCallBack(void);
private :
    void InitNote();
    void RefreshNote();
    void InitLayer(MarketLayerTag tag);
    CCLayer* CreateSpriteExtractLayer();
    CCLayer* CreateMysteryLayer();
    CCLayer* CreateSpecialMarketLayer();
    CCLayer* CreateVipMarketLayer();

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_spriteExtractBtn, SpriteExtractBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_specialPriceBtn,  SpecialPriceBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_vipBtn,           VipBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_mysteryBtn,       MysteryBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn,         CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_chargeBtn,        ChargeBtn);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_sonLayer,         SonLayer);
    CC_SYNTHESIZE_RETAIN(CCNode*,          m_mysteryNote,      MysteryNote);
	CC_SYNTHESIZE_RETAIN(CCNode*,          m_vipNote,          VipNote);

    CC_SYNTHESIZE(MarketLayerTag, m_currenTag, CurrentTag);

    CC_SYNTHESIZE_RETAIN(CCLayer*, m_currentLayer, CurrentLayer);
    MarketUILayer* m_uiLayer;
};

class MarketLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MarketLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MarketLayer);
};

class MarketUILayer :
      public UILayout
    , public UITutorialBase
{
public :
    MarketUILayer(CCNode* parent);
    virtual ~MarketUILayer();

    virtual CCNode* getTutorialNode(std::string name);

private :
    CC_SYNTHESIZE(MarketLayer*, m_infoLayer, InfoLayer);
};

#endif