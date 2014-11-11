#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "TutorialBase.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define BG_LIGHT_TAG     111
#define EFFECT_TIME      0.6f

class SpriteExtractResultUILayer;
class SpriteExtractResultLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    SpriteExtractResultLayer();
    virtual ~SpriteExtractResultLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SpriteExtractResultLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    void onSingleReturnClicked(CCObject* sender, CCControlEvent event);
    void onMutipleReturnClicked(CCObject* sender, CCControlEvent event);

    void onSingleContinue(CCObject* sender,  CCControlEvent event);
    void onMutipleContinue(CCObject* sender, CCControlEvent event);
    
    void onSingleDetail(CCObject* sender, CCControlEvent event);

    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    void registerWithTouchDispatcher();
    void onEnter();

    void SpriteOut(std::vector<int> vtSprite, int type);
    void SpriteOutSingle(int id);
    void SprietOutMutiple(std::vector<int> vtSprite);
    void ShowSprite(CCNode* node);
    void ShowBtn(CCNode* node);
    void ShowMutipleLight(CCNode* node);
    void ShowPet(CCNode* node);
	virtual void closeLayerCallBack(void);
private :
    int calcCost(int type);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_singleExtractLayer,  SingleExtractLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_mutipleExtractLayer, MutipleExtractLayer);
    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_singlePosLayer,      SinglePosLayer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_singleDetailBtn,     SingleDetailBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_singleReturnBtn,     SingleReturnBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_singleContinueBtn,   SingleContinueBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_mutipleReturnBtn,    MutipleReturnBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_mutipleContinueBtn,  MutipleContinueBtn);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_singlePetName,       SinglePetName);

    CC_SYNTHESIZE_RETAIN(CCLayer*,         m_costLayer,           CostLayer);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_costDescLabel,       CostDescLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*,      m_costLabel,           CostLabel);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_costSprite,          CostSprite);

    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_singleLight,        SingleLight);
    CC_SYNTHESIZE_RETAIN(CCSprite*,        m_mutipleLight,       MutipleLight);

    SpriteExtractResultUILayer* m_uiLayer;

    CC_SYNTHESIZE(int, m_currentType, CurrentType);
    CC_SYNTHESIZE(int, m_currentId,   CurrentId);

    std::vector<CCLayer*> m_vtMutipleLayers;
    std::vector<int> m_vtSprites;
};

class SpriteExtractResultLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SpriteExtractResultLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SpriteExtractResultLayer);
};

class SpriteExtractResultUILayer :
      public UILayout
    , public UITutorialBase
{
public :
    SpriteExtractResultUILayer(CCNode* parent);
    virtual ~SpriteExtractResultUILayer();

    virtual CCNode* getTutorialNode(std::string name);

private :
    CC_SYNTHESIZE(SpriteExtractResultLayer*, m_infoLayer, InfoLayer);
};