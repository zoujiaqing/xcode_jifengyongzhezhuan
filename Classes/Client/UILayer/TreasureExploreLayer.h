#pragma once 

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "TutorialBase.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define global_scale 0.7
#define MATERIAL_NUM 31010
#define DIAMOND_NUM 31011
#define UPGRADE_DIAMON 31012
#define REFRESH_DIAMON 31013
#define DURATION 0.5

class TreasureExploreUILayer;
class SingleTreasureMapLayerLoader;
class SingleTreasureMapLayer;
class SingleTreasureLayerLoader;
class SingleTreasureLayer;
class TreasureExploreLayerLoader;
class TreasureExploreLayer: public CCLayer
                          , public CCBMemberVariableAssigner
                          , public CCBSelectorResolver
                          , public CCNodeLoaderListener
                          , public CCTableViewDataSource
                          , public CCTableViewDelegate
{
public:
    TreasureExploreLayer();
    virtual ~TreasureExploreLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TreasureExploreLayer, create);
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

    void ConfigUI();
    void InitMultiLanguage();
    void InitItems();
    void OnReceiveTreasureInfoUpdate(CCObject* Data);
    void onRefreshItems(CCObject* Data);
    void onUpgradeSuccess(CCObject* Data);
    void InitRefresh();

	virtual void closeLayerCallBack(void);
private:
    SingleTreasureLayer* createSingleTreasureLayer();
    SingleTreasureMapLayer* createSingleTreasureMapLayer();
    void OnCloseBtnClicked(CCObject* pSender, CCControlEvent event);
    void OnExploreBtnClicked(CCObject* pSender, CCControlEvent event);
    void BackExploreBtnPrority(float dt);
    void OnGetRewardBtnClicked(CCObject* pSender, CCControlEvent event);
    void AddProgressBar();
    void SetSuccessRate(int rate, bool needEffect);
    void SetExploreRemainTimes(int times);
    void SetCurMap(int mapindex);
    void SetAlreadyGetIndex(int alreadyIndex, int toIndex);
    void AddTableView();
    void PutMapSpriteToTableViewCell(CCTableViewCell* cell);
	void MoveMapsToLeftAction();
	void MoveMapsToFirstAction();
    void DelayExacuteFunc(float dt);
    void ResetMapInitPostion(int mapindex);
    void ShowGetMessage(int alreadyIndex);
    void onBallClicked(CCObject* pSender, CCControlEvent event);

private:
    // num label
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pCurMapName,	    CurMapName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pSuccessNum,	    SuccessNum);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pMaterialNum,	    MaterialNum);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pDiamondNum,	    DiamondNum);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pRemainNum,	        RemainNum);

    // multi language
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTitleLabel,	    TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel1,	        Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel2,	        Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel3,	        Label3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel4,	        Label4);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel5,	        Label5);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel6,	        Label6);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel7,	        Label7);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel8,	        Label8);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel9,	        Label9);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pDiamondOwn,	    DiamondOwn);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pMaterialOwn,	    MaterialOwn);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel12,	        Label12);

    // refresh
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel10,	        Label10);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel11,	        Label11);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pRefreshDiamond,	RefreshDiamond);
	//CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pRefreshTimes,	    RefreshTimes);
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_pWordRefresh,       WordRefresh);
    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_pWordUpgrade,       WordUpgrade);
	CC_SYNTHESIZE_RETAIN(CCLayer*,    m_pBallEffectLayer,   BallEffectLayer);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pResetTimes,	    ResetTimes);

    // button
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCloseBtn,     CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pExploreBtn,   ExploreBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pGetRewardBtn, GetRewardBtn);
    CC_SYNTHESIZE(int, m_curMapIndex,     CurMapIndex);
    CC_SYNTHESIZE(int, m_curPercentage,   CurPercentage);
    CC_SYNTHESIZE(int, m_curExploreTimes, CurExploreTimes);
    CC_SYNTHESIZE(float, m_durationTime,  DurationTime);
    CC_SYNTHESIZE(bool, m_exploreClicked, ExploreClicked);
    CC_SYNTHESIZE(bool, m_rewardClicked,  RewardClicked);
    CC_SYNTHESIZE_RETAIN(CCProgressTimer*, m_pProgressTimer, ProgressTimer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_pRateLayer,      RateLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_pRateEffectLayer,      RateEffectLayer);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_pPercentSprite1, PercentSprite1);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_pPercentSprite2, PercentSprite2);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_pPercentSprite3, PercentSprite3);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m_pbgSprite, BgSprite);    

    CC_SYNTHESIZE_RETAIN(CCLayer*,      m_pTableViewLayer,	 TableViewLayer);
    CC_SYNTHESIZE_RETAIN(CCTableView*,  m_pTableView,	     TableView);

    CC_SYNTHESIZE_RETAIN(CCControlButton*,  m_pRefreshBtn,	     RefreshBtn);

    TreasureExploreUILayer* m_uiLayer;

public:
	std::vector<SingleTreasureLayer*> m_singleVec;
	std::vector<SingleTreasureLayer*> m_refreshAwardVec;
    std::vector<SingleTreasureMapLayer*> m_singleMapsVec;
};

class TreasureExploreLayerLoader
    : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TreasureExploreLayerLoader, loader);
private:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TreasureExploreLayer);
};

///////////////////////////////////////////////////////////////////
class SingleTreasureLayer
    : public CCLayer
    , public CCBMemberVariableAssigner
    , public CCBSelectorResolver
    , public CCNodeLoaderListener
{
public:
    SingleTreasureLayer();
    virtual ~SingleTreasureLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleTreasureLayer, create);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    void InitLayer(int idx, int itemId, int itemCount);
    void showGetFlag(bool flag);
    void showCanGetEffect(bool flag);
    void showUpgradeEffect();
    void setNameGreen();
    void setNameGray();

    void OnItemClicked(CCObject* pSender, CCControlEvent event);
	void onGetWayCallBack(CCObject* pSender);

private:
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pItemLayer,   ItemLayer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pItemBtn,     ItemBtn);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItemName,    ItemName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pItemCount,   ItemCount);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pEffectLayer, EffectLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pUpgradeEffectLayer, UpgradeEffectLayer);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pGetFlag,     GetFlag);
    CC_SYNTHESIZE(int, m_curItemId, ItemId);
};

class SingleTreasureLayerLoader
    : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleTreasureLayerLoader, loader);
private:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleTreasureLayer);
};

/////////////////////////////////////////////////////////
class SingleTreasureMapLayer
    : public CCLayer
    , public CCBMemberVariableAssigner
    , public CCBSelectorResolver
    , public CCNodeLoaderListener
{
public:
    SingleTreasureMapLayer();
    virtual ~SingleTreasureMapLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleTreasureMapLayer, create);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    void InitLayer();

private:
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pMapSprite,     MapSprite);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pBackSprite,    BackSprite);
};

class SingleTreasureMapLayerLoader
    : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleTreasureMapLayerLoader, loader);
private:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleTreasureMapLayer);
};


class TreasureExploreUILayer :
    public UILayout
    , public UITutorialBase
{
public :
    TreasureExploreUILayer(CCNode* parent);
    virtual ~TreasureExploreUILayer();

    virtual CCNode* getTutorialNode(std::string name);

private :
    CC_SYNTHESIZE(TreasureExploreLayer*, m_infoLayer, InfoLayer);
};