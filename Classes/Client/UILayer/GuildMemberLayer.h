#ifndef _GUILD_MEMBER_LAYER_H_
#define _GUILD_MEMBER_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"
#include "Controls/CCheckBox.h"
#include "GuildManager.h"
#include "ObserverProtocol.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

#define TAG_QUIT 0
#define TAG_JIESAN 1
#define TAG_APPOINT 2
#define TAG_KICK 3

class GuildMemberLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
    , public ObserverProtocol
{
public :
    GuildMemberLayer();
    virtual ~GuildMemberLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildMemberLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    virtual void onBroadcastMessage(BroadcastMessage* msg);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onQuitLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onKickLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onAppointLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onJieSanButtonClicked(CCObject* sender, CCControlEvent event);

    void onLevelButtonClickded(CCObject* sender, CCControlEvent pControlEvent);
    void onPowerButtonClickded(CCObject* sender, CCControlEvent pControlEvent);
    void onContributeButtonClickded(CCObject* sender, CCControlEvent pControlEvent);
    void onJobButtonClickded(CCObject* sender, CCControlEvent pControlEvent);
    void onStatusButtonClickded(CCObject* sender, CCControlEvent pControlEvent);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    
    int getClickIndex() { return m_clickIndex; }

private :
    void UpdateMemberInfo();
    void RefreshCell(CCTableViewCell* cell, unsigned int index);
    CCLayer* CreateSingleGuildMemberLayer();
    void AddTableView();

private :
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_roleButton,       RoleButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_levelButton,      LevelButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_powerButton,      PowerButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_contributeButton, ContributeButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_jobButton,        JobButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_statusButton,     StatusButton);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_quitButton,       QuitButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_jiesanButton,     jiesanButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_settingButton,    SettingButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_kickButton,       KickButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton,      CloseButton);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_contentLayer,     ContentLayer);
    CC_SYNTHESIZE_RETAIN(CCTableView*,     m_memberView,       MemberView);
    
    int m_clickIndex;
    std::vector<GuildMemberInfo> m_guildMembers;
    int m_curType;
};

class GuildMemberLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildMemberLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildMemberLayer);
};

class SingleGuildMemberLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    SingleGuildMemberLayer();
    virtual ~SingleGuildMemberLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleGuildMemberLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void InitLayer(unsigned int index, GuildMemberLayer* layer, GuildMemberInfo info);
    void onLine();
    void offLine();
    void onGLookBtnClicked(CCObject* object, CCControlEvent event);

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_nameLabel,       NameLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_levelLabel,      LevelLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_powerLabel,      PowerLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_contributeLabel, ContributeLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_jobLabel,        JobLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_statusLabel,     StatusLabel);

    CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m_lightSprite, LightSprite);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pLookGBtn, LookGBtn);

    GuildMemberLayer* m_parentLayer;
    unsigned int m_index;
    GuildMemberInfo m_curInfo;
};

class SingleGuildMemberLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleGuildMemberLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleGuildMemberLayer);
};

class GuildNoticeLayer : 
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildNoticeLayer();
    virtual ~GuildNoticeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildNoticeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();
    void initlayer();

    void onConfirmLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onRefuseLayer(CCObject* sender, CCControlEvent pControlEvent);

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_titleLabel,  TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_noticeLabel, NoticeLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pTipLabel,   TipLabel);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_confirmButton, ConfirmButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_refuseButton,  RefuseButton);
};

class GuildNoticeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildNoticeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildNoticeLayer);
};

class GuildAppointLayer : 
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildAppointLayer();
    virtual ~GuildAppointLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildAppointLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onConfirm(CCObject* sender, CCControlEvent pControlEvent);

    void onClickCheckBox1(bool flag);
    void onClickCheckBox2(bool flag);
    void onClickCheckBox3(bool flag);
    void onClickCheckBox4(bool flag);

    void initLayer(GuildMemberInfo info);
    void onCLoseBtn(CCObject* obj, CCControlEvent event);

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_titleLabel, TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_nameLabel,  NameLabel);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_type1Layer, Type1Layer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_type2Layer, Type2Layer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_type3Layer, Type3Layer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_type4Layer, Type4Layer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_confirmButton, ConfirmButton);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_tipLabel,   TipLabel);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*, m_checkBox1,  CheckBox1);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*, m_checkBox2,  CheckBox2);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*, m_checkBox3,  CheckBox3);
    CC_SYNTHESIZE_RETAIN(CCCheckBox*, m_checkBox4,  CheckBox4);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pCloseBtn,  CloseBtn);

    unsigned int m_selectType;
    GuildMemberInfo m_memberInfo;
};

class GuildAppointLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildAppointLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildAppointLayer);
};

#endif