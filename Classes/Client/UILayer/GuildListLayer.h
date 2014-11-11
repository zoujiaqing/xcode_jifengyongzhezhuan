#ifndef _GUILD_LIST_LAYER_H_
#define _GUILD_LIST_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"
#include "ObserverProtocol.h"
#include "GuildManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

#define MAX_COUNT_PER_PAGE       6

class GuildListLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public ObserverProtocol
    , CCEditBoxDelegate
{
public :
    GuildListLayer();
    virtual ~GuildListLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildListLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onCloseLayer(CCObject* sender, CCControlEvent pControlEvent);
    void onCreateClicked(CCObject* sender, CCControlEvent pControlEvent);
    void onButton1Clicked(CCObject* sender, CCControlEvent pControlEvent);
    void onButton2Clicked(CCObject* sender, CCControlEvent pControlEvent);
    void onButton3Clicked(CCObject* sender, CCControlEvent pControlEvent);
    void onButton4Clicked(CCObject* sender, CCControlEvent pControlEvent);
    void onButton5Clicked(CCObject* sender, CCControlEvent pControlEvent);
    void onButton6Clicked(CCObject* sender, CCControlEvent pControlEvent);
    
    void onLeftClicked(CCObject* sender, CCControlEvent event);
    void onRightClicked(CCObject* sender, CCControlEvent event);
    void onJoinClicked(CCObject* sender, CCControlEvent event);
    void onGLookBtnClicked(CCObject* sender, CCControlEvent event);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    virtual void onBroadcastMessage(BroadcastMessage* msg);
    void InitLayer();

    virtual void editBoxReturn(CCEditBox* editBox);

private :
    void InitSingleLayer(CCLayer* layer, GuildListInfo info, int index);
    void ShowGuildDetailInfo(unsigned int index);
    void ShowPageLabel();
    void CreateGuildSuccess(float f);
    void ProcessCDTime();
    void RefreshTime(float f);
    CCSpriteFrame* getRankSpriteFrame(int index);
    void AddRequireFlag(CCLayer* layer, GuildListInfo info);

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_titleLabel,     TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_searchLayer,    SearchLayer);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pageLabel,      PageLabel);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_leaderName,     LeaderName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_requireLabel,   RequireLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_guildWord,      GuildWord);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_leftBtn,   LeftBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_rightBtn,  RightBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_joinBtn,   JoinBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_createBtn, CreateBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn,  CloseBtn);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_button1,   Button1);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_button2,   Button2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_button3,   Button3);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_button4,   Button4);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_button5,   Button5);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_button6,   Button6);
    CC_SYNTHESIZE_RETAIN(CCEditBox*,       m_searchBox, SearchBox);
    
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_detailInfoLayer, DetailInfoLayer);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pFire1,    Fire1);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pFire2,    Fire2);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_pFire3,    Fire3);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pCDLabel,  CDLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_pCDTime,   CDTime);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_pLookGBtn, LookGBtn);

    std::vector<CCLayer*> m_contentLayers;
    std::vector<CCControlButton*> m_buttons;
    unsigned int m_pageId;
    unsigned int m_totalPage;
    bool m_bSearch;
    int m_selectId;
    long m_targetTimeStamp;
    GuildListInfo m_curInfo;
};

class GuildListLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildListLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildListLayer);
};

class GuildCreateLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildCreateLayer();
    virtual ~GuildCreateLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildCreateLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onConfirm(CCObject* sender, CCControlEvent pControlEvent);
    void onRefuse(CCObject* sender, CCControlEvent pControlEvent);

private :
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_editLayer,          EditLayer);
    CC_SYNTHESIZE_RETAIN(CCEditBox*,  m_editBox,            EditBox);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_costMoney,          CostMoney);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_confirmButton, ConfirmButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_refuseButton,  RefuseButton);
};

class GuildCreateLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildCreateLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildCreateLayer);
};

#endif