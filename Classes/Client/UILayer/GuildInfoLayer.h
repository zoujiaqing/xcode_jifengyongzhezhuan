#ifndef _GUILD_INFO_LAYER_H_
#define _GUILD_INFO_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"
#include "Controls/CCheckBox.h"
#include "ObserverProtocol.h"
#include "GuildManager.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

class GuildInfoLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public ObserverProtocol
{
public :
    GuildInfoLayer();
    virtual ~GuildInfoLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildInfoLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onEdit(CCObject* sender, CCControlEvent pControlEvent);
    void onEnterClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onContributeClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onBlessClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onAllInfoClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onSingleInfoClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onRequestClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onMemberClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onShopClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onHistoryClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onRankButtonClicked(CCObject* sender, CCControlEvent event);
    void onGuildInstanceClicked(CCObject * sender, CCControlEvent event);
	void onGuildRedEnvoClicked(CCObject * sender, CCControlEvent event);
    void showRequestTip(bool flag);
    void showContributeTip(bool flag);
    void showBlessTIp(bool flag);
	void showRedEnvoTip(bool flag);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();
    virtual void onBroadcastMessage(BroadcastMessage* msg);

private :
    void InitGuildInfo();

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_titleLable,      TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoLabel1,   AllInfoLabel1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoLabel2,   AllInfoLabel2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoLabel3,   AllInfoLabel3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoLabel4,   AllInfoLabel4);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoName,     AllInfoName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoMemberNo, AllInfoMemberNo);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoLv,       AllInfoLv);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_allInfoExp,      ALLInfoExp);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoLabel1, SingleInfoLabel1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoLabel2, SingleInfoLabel2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoLabel3, SingleInfoLabel3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoName,   SingleInfoName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoPlace,  SingleInfoPlace);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoAllContributon,  SingleInfoAllContribution);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_singleInfoLesContribution, SingleInfoLesContribution);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label1, Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_label2, Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_announcement, Announcement);

    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_allInfoLayer,    AllInfoLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_singleInfoLayer, SingleInfoLayer);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_allInfoButton,    AllInfoButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_singleInfoButton, SingleInfoButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_editButton,       EditButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_requestButton,    RequestButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_historyButton,    HistoryButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_rankButton,       RankButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_memberButton,     MemberButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_enterButton,      EnterButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton,      CloseButton);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_contributeButton, ContributeButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_blessButton,      BlessButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_shopButton,       ShopButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_instanceButton,   InstanceButton);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_redEnvoBtn,	   RedEnvoBtn);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pRequestTip,      RequestTip);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pContributeTip,   ContributeTip);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pBlessTip,        BlessTip);
	CC_SYNTHESIZE_RETAIN(CCLayer*,         m_pRedEnvoTip,      RedEnvoTip);
};

class GuildInfoLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildInfoLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildInfoLayer);
};

class GuildInfoEditLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCEditBoxDelegate
{
public :
    GuildInfoEditLayer();
    virtual ~GuildInfoEditLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildInfoEditLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onConfirm(CCObject* sender, CCControlEvent pControlEvent);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();
    void editBoxReturn(CCEditBox* editBox) {}
    void editBoxEditingDidBegin(CCEditBox* editBox);
    void editBoxEditingDidEnd(CCEditBox* editBox);

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn,   CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_confirmBtn, ConfirmBtn);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_backLayer,  BackLayer);

    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_noticeLayer,   NoticeLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,  m_announceLayer, AnnounceLayer);

    CC_SYNTHESIZE_RETAIN(CCEditBox*,       m_noticeEditBox, NoticeEditBox);
    CC_SYNTHESIZE_RETAIN(CCEditBox*,       m_announceEditBox, AnnounceEditBox);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pText1,        Text1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pText2,        Text2);
};

class GuildInfoEditLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildInfoEditLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildInfoEditLayer);
};

class GuildContributeLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildContributeLayer();
    virtual ~GuildContributeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildContributeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void InitMutilanguage();

    void onContributeInfoCallBack(CCObject* obj);
    void onContributeTimesUpdate(CCObject* obj);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void OnContribute1Clicked(CCObject* pSender, CCControlEvent event);
    void OnContribute2Clicked(CCObject* pSender, CCControlEvent event);
    void OnContribute3Clicked(CCObject* pSender, CCControlEvent event);
    void OnContribute4Clicked(CCObject* pSender, CCControlEvent event);

private:
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_guildLv,      GuildLv);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_guildExp,     GuildExp);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_contribute,   ContributeLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_moneyLabel,   MoneyLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_diamondLabel, DiamondLabel);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_contributeButton1, ContributeButton1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_contributeButton2, ContributeButton2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_contributeButton3, ContributeButton3);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_contributeButton4, ContributeButton4);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton,       CloseButton);
    CC_SYNTHESIZE_RETAIN(CCLayer*, m_backLayer,         BackLayer);


    // mutil language
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTitleLabel,      TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel1,          Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel2,          Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel3,          Label3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel4,          Label4);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel5,          Label5);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel6,          Label6);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel7,          Label7);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel8,          Label8);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel9,          Label9);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel10,         Label10);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pChuConsume,      ChuConsume);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pZhongConsume,    ZhongConsume);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pGaoConsume,      GaoConsume);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTeConsume,       TeConsume);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pChuExp,          ChuExp);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pZhongExp,        ZhongExp);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pGaoExp,          GaoExp);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTeExp,           TeExp);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pChuContribute,   ChuContribute);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pZhongContribute, ZhongContribute);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pGaoContribute,   GaoContribute);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTeContribute,    TeContribute);

    int m_pOldGuildLv;
};

class GuildContributeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildContributeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildContributeLayer);
};

class GuildRequestLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
    , public ObserverProtocol
{
public :
    GuildRequestLayer();
    virtual ~GuildRequestLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRequestLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    virtual void onBroadcastMessage(BroadcastMessage* msg);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onSettingClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onConfirmClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onRefuseClicked(CCObject* sender, CCControlEvent pControlEvent);

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onClickCheckBox(bool flag);
    void setClickState(unsigned int index, bool state);
    bool getClickState(unsigned int index);

private :
    void UpdateRequestInfo();
    void RefreshCell(CCTableViewCell* cell, unsigned int index);
    CCLayer* CreateSingleRequestLayer();

    std::vector<bool> m_vtStates;
    std::vector<GuildMemberInfo> m_vtGuildRequest;

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_settingButton, SettingButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_refuseButton,  RefuseButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_agreeButton,   AgreeButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton,   CloseButton);

    CC_SYNTHESIZE_RETAIN(CCCheckBox*,      m_checkBox,      CheckBox);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_checkLayer,    CheckLayer);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_backLayer,     BackLayer);
    CC_SYNTHESIZE_RETAIN(CCTableView*,     m_tableView,     TableView);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_contentLayer,  ContentLayer);
};

class GuildRequestLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRequestLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRequestLayer);
};

class SingleGuildRequestLayer:
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    SingleGuildRequestLayer();
    virtual ~SingleGuildRequestLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleGuildRequestLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onClickCheckBox(bool status);
    void InitLayer(unsigned int index, GuildRequestLayer* parentLayer, GuildMemberInfo info);

private :
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_chooseLayer, ChooseLayer);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*, m_checkBox,    CheckBox);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_nameLabel,   NameLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_lvLabel,     LvLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_powerLabel,  PowerLabel);

    GuildRequestLayer* m_parentLayer;
    unsigned int m_index;
};

class SingleGuildRequestLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleGuildRequestLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleGuildRequestLayer);
};

class GuildRequestSettingLayer : 
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildRequestSettingLayer();
    virtual ~GuildRequestSettingLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRequestSettingLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onConfirm(CCObject* object, CCControlEvent event);
    void onRefuse(CCObject* object, CCControlEvent event);

    void onPlus10(CCObject* object, CCControlEvent event);
    void onPlus1(CCObject* objecet, CCControlEvent event);
    void onAdd10(CCObject* object, CCControlEvent event);
    void onAdd1(CCObject* object, CCControlEvent event);

    void onClickCheckBox1(bool flag);
    void onClickCheckBox2(bool flag);
    void onClickCheckBox3(bool flag);

private :
    void RefreshPowerLabel();

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_plus_10Button, Plus_10_Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_plus_1Button,  Plus_1_Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_add_10Button,  Add_10_Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_add_1Button,   Add_1_Button);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_confirmButton, ConfirmButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_refuseButton,  RefuseButton);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_backLayer,     BackLayer);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_powerLabel,    PowerLabel);

    CC_SYNTHESIZE_RETAIN(CCCheckBox*,      m_checkBox1,     CheckBox1);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*,      m_checkBox2,     CheckBox2);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*,      m_checkBox3,     CheckBox3);

    CC_SYNTHESIZE_RETAIN(CCLayer*, m_checkLayer1, CheckLayer1);
    CC_SYNTHESIZE_RETAIN(CCLayer*, m_checkLayer2, CheckLayer2);
    CC_SYNTHESIZE_RETAIN(CCLayer*, m_checkLayer3, CheckLayer3);

    int m_power;
    unsigned int m_selectType;
};

class GuildRequestSettingLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRequestSettingLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRequestSettingLayer);
};

class GuildBlessLayer:
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildBlessLayer();
    virtual ~GuildBlessLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildBlessLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onCloseLayer(CCObject* object, CCControlEvent event);
    void onBlessClicked(CCObject* object, CCControlEvent event);
    void onMsgCallBack(CCObject* obj);
    void onMsgTimesCallBack(CCObject* obj);
    void InitBlessLayer();
    void PlayerEffect();

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton,  CloseButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_blessButton,  BlessButton);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_currentLife,  CurrentLife);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_nextLife,     NextLife);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_currentPres,  CurrentPres);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_nextPres,     NextPres);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_currentMoney, CurrentMoney);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_nextMoney,    NextMoney);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_currentStep,  CurrentStep);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_nextNeed,     NextNeed);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_costDiamond,  CostDiamond);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_restTimes,    RestTimes);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_backLayer,    BackLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,        m_pSpriteMaterial, SpriteMaterial)

    // muti language
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTitleLabel,      TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel1,          Label1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel2,          Label2);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel3,          Label3);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel4,          Label4);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel5,          Label5);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel6,          Label6);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pLabel7,          Label7);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_pCurDiamond,      CurDiamond);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,  m_pCurContribute,      CurContribute);
    CC_SYNTHESIZE_RETAIN(CCLayer*,     m_pEffectLayer,     EffectLayer);
};

class GuildBlessLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildBlessLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildBlessLayer);
};

class GuildInstanceListLayer : public CCLayer{

public:
	typedef struct ScrollPagePosition
	{
		int index;
		int page;
		int row;
		int col;
	}ScrollPagePosition;

public:
	GuildInstanceListLayer();
	virtual ~GuildInstanceListLayer();

	static int guildInstancePlayTimes;
	static int getCouldPlayTimes();
	static GuildUnionInfo guildUnionInfo;

	virtual bool init();

	void onGetGuildUnionInfoCallBack(CCObject* obj);
	void InitMaps();

	void onClose(CCObject* sender);
	void onPressInstanceBtn(CCObject *sender);
	virtual void closeLayerCallBack(void);
private:

protected:
	TXGUI::UILayout* m_pLayer;
	MapInfoVector m_vecMaps;

	/// add single instance
	void   addSingleInstance(InstanceMapInfo info, ScrollPagePosition position);

	ScrollPagePosition getScrollPagePos(int index);
	CCPoint scrollPosToPoint(ScrollPagePosition position);

};

class GuildInstanceHelpLayer : public CCLayer{

public:
	typedef struct ScrollPagePosition
	{
		int index;
		int page;
		int row;
		int col;
	}ScrollPagePosition;

	GuildInstanceHelpLayer();
	virtual ~GuildInstanceHelpLayer();

	virtual bool init();
	void showCoinAndDiamond();
	void showOutput();

	void onGetInstanceChooseInfoCallBack(CCObject* obj);
	void InitChoose();

	bool checkCouldPlay();

	void onPressOutputBtn(CCObject *sender);
	void onPressChooseBtn(CCObject *sender);

	void onClose(CCObject* sender);
	void onStartOneself(CCObject* sender);
	void onStartWithHelp(CCObject* sender);
protected:
	TXGUI::UILayout* m_pLayer;
	TXGUI::IconButton* m_pSelectedChoose;

	ScrollPagePosition getScrollPagePos(int index, int maxRow, int maxCol);
	CCPoint scrollPosToPoint(ScrollPagePosition position, CCSize size, float offsetH, float offsetW, int maxRow, int maxCol);
};

#endif