// added by seanxiao at 2014/03/31
#ifndef _RANK_LIST_LAYER_H_
#define _RANK_LIST_LAYER_H_

#include "UILayout.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "RankManager.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SingleRankListLayerLoader;
class SingleRankListLayer;
class RankListLayerNodeLoader;
class RankListLayer
	: public CCLayer
	, public CCBMemberVariableAssigner
	, public CCBSelectorResolver
	, public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public:
	RankListLayer();
	virtual ~RankListLayer();

	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(RankListLayer, create);
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
    void refreshCell(CCTableViewCell* cell, int idx);

public:
	void ConfigUI();
	void CloseLayer(CCObject* pSender, CCControlEvent pEvent);
    void onLabelButtonClicked(CCObject* pSender, CCControlEvent pEvent);
	void initMutilanguage();
    void OnRefreshRankList(CCObject* obj);
    void OnNoticeLookClicked(CCObject* obj);
    void ShowIndexPlayerInfo(int idx);
    void HighLightSelect(int index);

private:
    SingleRankListLayer* createSingleRankListLayer();
    void RefreshAttri();

	// all button
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pLevelButton,      LevelButton);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCombatButton,     CombatButton);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pArenaButton,      ArenaButton);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pReputationButton, ReputationButton);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pPartyButton,      PartyButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCloseButton,      CloseButton);

	// mutilanguage
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTitleLabel,	    TitleLabel);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pRankLabel,	        RankLabel);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pRiseLabel,	        RiseLabel);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pPlayerLabel,	    PlayerLabel);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pPartyLabel,        PartyLabel);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pReputationLabel,   ReputationLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLookLabel,         LookLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pCurRankLabel,      CurRankLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pCurRankNum,        CurRankNum);

	// model and tabelview layer
    CC_SYNTHESIZE_RETAIN(CCLayer*,      m_pTableViewLayer,	 TableViewLayer);
    CC_SYNTHESIZE_RETAIN(CCTableView*,  m_pTableView,	     TableView);
    CC_SYNTHESIZE_RETAIN(CCLayer*,      m_pRightLayer,       RightLayer);

    CC_SYNTHESIZE(int,  m_pRankName,  RankName);

private:

};

class RankListLayerNodeLoader
	: public CCLayerLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(RankListLayerNodeLoader, loader);
private:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(RankListLayer);
};


class SingleRankListLayer
	: public CCLayer
	, public CCBMemberVariableAssigner
	, public CCBSelectorResolver
	, public CCNodeLoaderListener
{
public:
	SingleRankListLayer();
	virtual ~SingleRankListLayer();
	
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleRankListLayer, create);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
	virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    void InitLayer(RankInfoX info, int index);
    void InitLayerData();
    void OnLookButtonClicked(CCObject* pSender, CCControlEvent event);

private:
    CC_SYNTHESIZE_RETAIN(CCSprite*,           m_pRankSprite,        RankSprite);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_pRank,              Rank);
    CC_SYNTHESIZE_RETAIN(CCSprite*,           m_pRankState,         RankState);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_pRankStateNo,       RankStateNo);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_pVip,               Vip);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_pName,              Name);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_pPartyName,         PartyName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_pReputation,        Reputation);
    CC_SYNTHESIZE_RETAIN(CCLayer*,            m_pHighLightLayer,    HighLightLayer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*,    m_pLookBtn,           LookBtn);
    CC_SYNTHESIZE_RETAIN(CCSprite*,           m_pCurPos,            CurPos);
    CC_SYNTHESIZE_RETAIN(CCSprite*,           m_pCircle,            Circle);
	CC_SYNTHESIZE_RETAIN(CCSprite*,			  m_pRect,              Rect);
};

class SingleRankListLayerLoader
	: public CCLayerLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleRankListLayerLoader, loader);
private:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleRankListLayer);
};

#endif