#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "TutorialBase.h"
#include "ObserverProtocol.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SpriteExtractUILayer;
class SpriteExtractLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
    , public ObserverProtocol
{
public :
    SpriteExtractLayer();
    virtual ~SpriteExtractLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SpriteExtractLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();

	void InitLayer();
	void InitPositionAndVisible();
    void InitPurpleDesc();

    void onExtract1(CCObject* sender, CCControlEvent pControlEvent);
    //void onExtract2(CCObject* sender, CCControlEvent pControlEvent);
    void onExtract3(CCObject* sender, CCControlEvent pControlEvent);
	void onExtract4(CCObject* sender, CCControlEvent pControlEvent);
	void onExtract5(CCObject* sender, CCControlEvent pControlEvent);

    void onClickSpriteList(CCObject* sender, CCControlEvent event);

    void InitTime();
    void SendGetPrices();

    virtual void onBroadcastMessage(BroadcastMessage* msg);

private :
    void InitNote();
    void updateTime(float f);
    std::string GetTimeStr(long _time);
    void RefreshPrice();
    
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_extractBtn1, ExtractBtn1);
    //CC_SYNTHESIZE_RETAIN(CCControlButton*, m_extractBtn2, ExtractBtn2);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_extractBtn3, ExtractBtn3);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_extractBtn4, ExtractBtn4);
	CC_SYNTHESIZE_RETAIN(CCControlButton*, m_extractBtn5, ExtractBtn5);

	CC_SYNTHESIZE_RETAIN(CCLayer*, m_extractLayer1, ExtractLayer1);
	CC_SYNTHESIZE_RETAIN(CCLayer*, m_extractLayer3, ExtractLayer3);
	CC_SYNTHESIZE_RETAIN(CCLayer*, m_extractLayer4, ExtractLayer4);
	CC_SYNTHESIZE_RETAIN(CCLayer*, m_extractLayer5, ExtractLayer5);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_spriteListBtn, SpriteListBtn);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_greenLabel,  GreenLabel);
    //CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_blueLabel,   BlueLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_purpleLabel, PurpleLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_purpleDesc,  PurpleDesc);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_label5,      Label5);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_greenCost,   GreenCost);
    //CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_blueCost,    BlueCost);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_purpleCost,  PurpleCost);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_yellowCost,  YellowCost);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_redCost,  RedCost);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_diamondAmount, DiamondAmount);

    CC_SYNTHESIZE(int, m_xxx, XXX);
    CC_SYNTHESIZE(int, m_yyy, YYY);

    CC_SYNTHESIZE_RETAIN(CCSprite*,   m_greenNote,  GreenNote);
    //CC_SYNTHESIZE_RETAIN(CCSprite*,   m_blueNote,   BlueNote);
	CC_SYNTHESIZE_RETAIN(CCSprite*,   m_purpleNote, PurpleNote);

    SpriteExtractUILayer* m_uiLayer;
};

class SpriteExtractLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SpriteExtractLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SpriteExtractLayer);
};

class SpriteExtractUILayer :
      public UILayout
    , public UITutorialBase
{
public :
    SpriteExtractUILayer(CCNode* parent);
    virtual ~SpriteExtractUILayer();

    virtual CCNode* getTutorialNode(std::string name);

private :
    CC_SYNTHESIZE(SpriteExtractLayer*, m_infoLayer, InfoLayer);
};