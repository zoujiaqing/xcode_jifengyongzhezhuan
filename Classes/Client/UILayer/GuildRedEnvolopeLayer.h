#ifndef _GUILD_RED_ENVOLOPE_LAYER_H_
#define _GUILD_RED_ENVOLOPE_LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"
#include "GuildManager.h"
#include "EffectSprite.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

class GuildRedEnvolopeLayer :
	  public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public:
	GuildRedEnvolopeLayer(void);
	virtual ~GuildRedEnvolopeLayer(void);

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRedEnvolopeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

	void onClose(CCObject* sender, CCControlEvent event);
	void onNote(CCObject* sender, CCControlEvent event);

	void onTuhaoBtn(CCObject* sender, CCControlEvent event);
	void onChongzhiBtn(CCObject* sender, CCControlEvent event);

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

	void OnRedEnvoInfoMsgCallBack(CCObject* obj);
	void onRedEnvoConfigMsgCallBack(CCObject* obj);

	void ConfigUI();

	void RefreshCell(CCTableViewCell* cell, unsigned int index, bool isLeft);

	CCLayer* CreateSingleGuildRedEnvolopeLayer();

	CCLayer* CreateRightSingleGuildRedEnvolopeLayer();

	std::vector<std::string> split(std::string str,std::string pattern);

	void onIconClicked(CCObject* sender, CCControlEvent event);
	void onChargeBtn(CCObject* sender, CCControlEvent event);

	void onSendRed(CCObject* sender, CCControlEvent event);

	void onFirstSendRedEnvoMsgCallBack(CCObject* obj);

	unsigned int getSizeOfTuhaoRedEnvo();

private:
	CC_SYNTHESIZE_RETAIN(CCLayer*,           m_leftViewTableLayer,    LeftViewTableLayer);
	CC_SYNTHESIZE_RETAIN(CCTableView*,       m_leftViewTable,         LeftViewTable);
	CC_SYNTHESIZE_RETAIN(CCLayer*,           m_rightViewTableLayer,   RightViewTableLayer);
	CC_SYNTHESIZE_RETAIN(CCTableView*,       m_rightViewTable,        RightViewTable);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnClose,              BtnClose);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnNote,               BtnNote);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnCharge,             BtnCharge);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnTuhao,              BtnTuhao);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnChongzhi,           BtnChongZhi);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_label1,                Label1);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_label2,                Label2);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_label3,                Label3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_label4,                Label4);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_label5,                Label5);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelIcon1,            LabelIcon1);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelIcon2,            LabelIcon2);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelIcon3,            LabelIcon3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelIcon4,            LabelIcon4);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnIcon1,              BtnIcon1);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnIcon2,              BtnIcon2);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnIcon3,              BtnIcon3);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnIcon4,              BtnIcon4);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,   m_btnSendRed,            BtnSendRed);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelDiaNum,           LabelDiaNum);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelTitle,            LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelGetNote,          LabelGetNote);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,        m_labelGetAwardsNote,    LabelGetAwardsNote);


	unsigned int m_leftSelectedIndex;
	unsigned int m_rightSelectedIndex;
	CCLayer* m_layerSendRedEnvo;
};

class GuildRedEnvolopeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRedEnvolopeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRedEnvolopeLayer);
};

class SingleLeftGuildRedEnvolopeLayer :
	public CCLayer, 
	public CCBSelectorResolver, 
	public CCBMemberVariableAssigner, 
	public CCNodeLoaderListener
{
public:
	SingleLeftGuildRedEnvolopeLayer();
	virtual ~SingleLeftGuildRedEnvolopeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleLeftGuildRedEnvolopeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void InitLayer(unsigned int index, GuildRedEnvolopeLayer* layer, GuildRedEnvolopeInfo* info);

    virtual void onEnter();

	bool getIsShowHighRedEnvo();
	void setIsShowHighRedEnvo(bool flag);

	std::vector<std::string> split(std::string str,std::string pattern);

	CCSprite* get() { return m_spriteHighRedEnvo; }

	void onIconClicked(CCObject* sender, CCControlEvent event);

	//virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	//virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

private:
    CC_SYNTHESIZE_RETAIN(CCSprite*,               m_iconFrame,           IconFrame);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelRedName,        LabelRedName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,			  m_labelRestTime,       LabelRestTime);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteHighRedEnvo,   SpriteHighRedEnvo);
	CC_SYNTHESIZE_RETAIN(CCSprite*,               m_spriteNorRedEnvo,    SpriteNorRedEnvo);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnIcon,             BtnIcon);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,			  m_labelAward1Count,    LabelAward1Count);

	bool m_showHighRedEnvo;
};

class SingleGuildRedEnvoLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleGuildRedEnvoLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleLeftGuildRedEnvolopeLayer);
};

class GuildRedEnvolopeNoteLayer :
	public CCLayer, 
	public CCBSelectorResolver, 
	public CCBMemberVariableAssigner, 
	public CCNodeLoaderListener
{
public:
	GuildRedEnvolopeNoteLayer();
	virtual ~GuildRedEnvolopeNoteLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRedEnvolopeNoteLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void onEnter();

	void onClose(CCObject* sender, CCControlEvent event);
	void onBtnOk(CCObject* sender, CCControlEvent event);

private:
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnClose,            BtnClose);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnNoteOk,           BtnNoteOK);
	CC_SYNTHESIZE_RETAIN(CCLayer*,                m_layerContent,        LayerContent);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelNoteContent1,   LabelNoteContent1);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelNoteContent2,   LabelNoteContent2);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelNoteContent3,   LabelNoteContent3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelNoteContent4,   LabelNoteContent4);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelNoteContent5,   LabelNoteContent5);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelNoteContent6,   LabelNoteContent6);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelTitle,			 LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLayer*,				  m_layerBg,			 LayerBg);
};

class GuildRedEnvolopeSendLayer :
	public CCLayer, 
	public CCBSelectorResolver, 
	public CCBMemberVariableAssigner, 
	public CCNodeLoaderListener,
	public CCAnimationEventListener
{
public:
	GuildRedEnvolopeSendLayer();
	virtual ~GuildRedEnvolopeSendLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRedEnvolopeSendLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID);

	void SetAnimEndCallBack(CCObject* pListerner,SEL_CallFunc callBack);

	void onFirtstAnimEnd();

	void onSecondAnimEnd();

	virtual void onEnter();

	std::vector<std::string> split(std::string str,std::string pattern);

	void onCancel(CCObject* sender, CCControlEvent event);
	void onBtnOk(CCObject* sender, CCControlEvent event);

private:
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnCancel,              BtnCancel);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnOk,					BtnOK);
	CC_SYNTHESIZE_RETAIN(CCLayer*,                m_layerEffect,            LayerEffect);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelRedEnvoName,       LabelRedEnvoName);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelSendLeft,			LabelSendLeft);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteAward1,			SpriteAward1);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteAward2,			SpriteAward2);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteAward3,			SpriteAward3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelAward1Count,       LabelAward1Count);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelAward2Count,       LabelAward2Count);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelAward3Count,       LabelAward3Count);
	CC_SYNTHESIZE_RETAIN(CCLayer*,				  m_layerBg,				LayerBg);

	CCObject* m_pAnimEndListerner;
	SEL_CallFunc m_pfnAnimEndSelector;
	EffectSprite* m_effectSend;
	bool m_bIsPlayingAnim;
};

class GuildRedEnvolopeNoteLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRedEnvolopeNoteLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRedEnvolopeNoteLayer);
};

class SingleRightGuildRedEnvolopeLayer :
	public CCLayer, 
	public CCBSelectorResolver, 
	public CCBMemberVariableAssigner, 
	public CCNodeLoaderListener
{
public:
	SingleRightGuildRedEnvolopeLayer();
	virtual ~SingleRightGuildRedEnvolopeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleRightGuildRedEnvolopeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

	void InitLayer(unsigned int index, GuildRedEnvolopeLayer* layer, GuildGetRedEnvolopeInfo info);

    virtual void onEnter();

	std::vector<std::string> split(std::string str,std::string pattern);

	void onGet(CCObject* sender, CCControlEvent event);
	void onCheckout(CCObject* sender, CCControlEvent event);

	void onGetRedEnvoMsgCallBack(CCObject* obj);

	void onGetRedEnvoRecordMsgCallBack(CCObject* obj);

	void updateRestTime(float dt);

	void setRestTimeLabel();


private:
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_labelRestTime,       LabelRestTime);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_labelPlayerName,     LabelPlayerName);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_labelCount,          LabelCount);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_labelRestRed,        LabelRestRed);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,         m_labelRest,			 LabelRest);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,    m_btnGet,				 BtnGet);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,    m_btnCheckout,		 BtnCheckout);
	CC_SYNTHESIZE_RETAIN(CCLayer*,            m_layerGetRedEnvo,     LayerGetRedEnvo);

	unsigned int m_sendTime;
	SingleRedEnvoInfo m_singleInfo;
};

class GuildRedEnvolopeSendLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRedEnvolopeSendLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRedEnvolopeSendLayer);
};

class SingleRightGuildRedEnvoLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleRightGuildRedEnvoLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleRightGuildRedEnvolopeLayer);
};

class GuildGetRedEnvolopeLayer :
	public CCLayer, 
	public CCBSelectorResolver, 
	public CCBMemberVariableAssigner, 
	public CCNodeLoaderListener,
	public CCAnimationEventListener
{
public:          
	GuildGetRedEnvolopeLayer();
	virtual ~GuildGetRedEnvolopeLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildGetRedEnvolopeLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID);

	void SetAnimEndCallBack(CCObject* pListerner,SEL_CallFunc callBack);

	void onFirtstAnimEnd();

	void onSecondAnimEnd();

	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void onEnter();

	std::vector<std::string> split(std::string str,std::string pattern);

	void onBtnOk(CCObject* sender, CCControlEvent event);

private:
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnOk,					BtnOK);
	CC_SYNTHESIZE_RETAIN(CCLayer*,                m_layerEffect,            LayerEffect);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelGetSuc,		    LabelGetSuc);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteAward1,			SpriteAward1);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteAward2,			SpriteAward2);
	CC_SYNTHESIZE_RETAIN(CCSprite*,				  m_spriteAward3,			SpriteAward3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelGetLeft,		    LabelGetLeft);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelGetAward1,		    LabelGetAward1);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelGetAward2,		    LabelGetAward2);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelGetAward3,		    LabelGetAward3);
	CC_SYNTHESIZE_RETAIN(CCLayerColor*,			  m_layerBg,				LayerBg);

	CCObject* m_pAnimEndListerner;
	SEL_CallFunc m_pfnAnimEndSelector;
	EffectSprite* m_effectGet;
	bool m_bIsPlayingAnim;
};

class GuildGetRedEnvolopeLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildGetRedEnvolopeLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildGetRedEnvolopeLayer);
};

class GuildRedEnvolopeRecordLayer :
	  public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
public:
	GuildRedEnvolopeRecordLayer();
	virtual ~GuildRedEnvolopeRecordLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRedEnvolopeRecordLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void onEnter();

	std::vector<std::string> split(std::string str,std::string pattern);

	void onBtnOk(CCObject* sender, CCControlEvent event);
	void onBtnClose(CCObject* sender, CCControlEvent event);

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

	void ConfigUI();

	void RefreshCell(CCTableViewCell* cell, unsigned int index);

	CCLayer* CreateSingleGuildRedEnvoRecordLayer();

private:
	CC_SYNTHESIZE_RETAIN(CCControlButton*,        m_btnOk,					BtnOK);
	CC_SYNTHESIZE_RETAIN(CCLayer*,                m_layerRecordContent,     LayerRecordContent);
	CC_SYNTHESIZE_RETAIN(CCTableView*,			  m_layerTableView,			LayerTableView);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelSenderName,		LabelSenderName);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelTitle,				LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCControlButton*,		  m_btnClose,				BtnClose);
	CC_SYNTHESIZE_RETAIN(CCLayerColor*,			  m_layerBg,				LayerBg);
};

class GuildRedEnvolopeRecordLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRedEnvolopeRecordLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRedEnvolopeRecordLayer);
};

class SingleGuildRedEnvoRecordLayer :
	public CCLayer, 
	public CCBSelectorResolver, 
	public CCBMemberVariableAssigner, 
	public CCNodeLoaderListener
{
public:
	SingleGuildRedEnvoRecordLayer();
	virtual ~SingleGuildRedEnvoRecordLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SingleGuildRedEnvoRecordLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void InitLayer(unsigned int index, GuildRedEnvolopeRecordLayer* layer, RedEnvoRecordInfo info);

    virtual void onEnter();

	std::vector<std::string> split(std::string str,std::string pattern);

private:
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,             m_labelDate,        LabelDate);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,			  m_labelTime,        LabelTime);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,			  m_labelInfo,        LabelInfo);
};

class SingleGuildRedEnvoRecordLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SingleGuildRedEnvoRecordLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SingleGuildRedEnvoRecordLayer);
};
#endif

