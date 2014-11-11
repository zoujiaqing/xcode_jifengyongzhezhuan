#include "MarketLayer.h"
#include "MainMenuLayer.h"
#include "SpriteExtractLayer.h"
#include "MysteryLayer.h"
#include "SpecialMarketLayer.h"
#include "VipMarketLayer.h"
#include "ButtonPerformanceManager.h"
#include "LuaTinkerManager.h"
#include "UIManager.h"
#include "TutorialsManager.h"
#include "AspriteManager.h"
#include "TXGUI.h"
#include "UIDefine.h"
#include "Localization.h"
#include "GMessage.h"

MarketLayer::MarketLayer() :
    m_spriteExtractBtn(NULL),
    m_specialPriceBtn(NULL),
    m_vipBtn(NULL),
    m_mysteryBtn(NULL),
    m_currentLayer(NULL),
    m_closeBtn(NULL),
    m_chargeBtn(NULL),
    m_currenTag(Tag_Sprite),
    m_sonLayer(NULL),
    m_mysteryNote(NULL),
	m_vipNote(NULL)
{

}

MarketLayer::~MarketLayer()
{
    ButtonPerformanceManager::Get()->removePerformanceButton(this);

    setSpriteExtractBtn(NULL);
    setSpecialPriceBtn(NULL);
    setVipBtn(NULL);
    setMysteryBtn(NULL);
    setCloseBtn(NULL);
    setChargeBtn(NULL);
    setSonLayer(NULL);

    setCurrentLayer(NULL);

    UIManager::sharedManager()->RemoveLayout("market_layer");
    delete m_uiLayer;

    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
}

SEL_MenuHandler MarketLayer::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MarketLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",   MarketLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "sprite",  MarketLayer::onSpriteClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "special", MarketLayer::onSpecialClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "vip",     MarketLayer::onVipClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "mystery", MarketLayer::onMysteryClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "charge", MarketLayer::onChargeClicked);
    
    return NULL;
}

bool MarketLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_btn",  CCControlButton*, m_spriteExtractBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "special_btn", CCControlButton*, m_specialPriceBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vip_btn",     CCControlButton*, m_vipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mystery_btn", CCControlButton*, m_mysteryBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",   CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "charge_btn",  CCControlButton*, m_chargeBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "son_layer",   CCLayer*,         m_sonLayer);

    return false;
}

void MarketLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);
    
    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    // 初始化一些label
    m_spriteExtractBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_FAIRYSHOP_TITLE")), CCControlStateNormal);
    m_specialPriceBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_ONSALESHOP_TITLE")), CCControlStateNormal);
    m_vipBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_VIPSHOP_TITLE")), CCControlStateNormal);
    m_mysteryBtn->setTitleForState(CCString::create(Localizatioin::getLocalization("M_SHOP_TITLE")), CCControlStateNormal);

    // 添加新手指引相关参数
    m_uiLayer = new MarketUILayer(this);
    m_uiLayer->setInfoLayer(this);
    UIManager::sharedManager()->AddLayout(m_uiLayer, "market_layer");

    InitNote();
}

void MarketLayer::InitNote()
{
    setMysteryNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));
	setVipNote(AspriteManager::getInstance()->getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_EXCALMATORY"));

	float scale = UIManager::sharedManager()->getScaleFactor();
	m_vipNote->setPosition(ccpAdd(m_vipBtn->getPosition(), ccp(60, 0)));
	m_vipBtn->getParent()->addChild(m_vipNote);

    m_mysteryNote->setPosition(ccpAdd(m_mysteryBtn->getPosition(), ccp(60, 0)));
    m_mysteryBtn->getParent()->addChild(m_mysteryNote);

    CCActionInterval* action1 = NULL;
    action1 = CCMoveBy::create(0.4, ccp(0,10));
    CCActionInterval* action_back1 = action1->reverse();
    CCActionInterval* seq1 = (CCActionInterval*)(CCSequence::create( action1, action_back1, NULL ));
    CCRepeatForever* pAction1 = CCRepeatForever::create(seq1);

    CCActionInterval* action2 = NULL;
    action2 = CCMoveBy::create(0.4,ccp(0,10));
    CCActionInterval* action_back2 = action2->reverse();
    CCActionInterval* seq2 = (CCActionInterval*)(CCSequence::create( action2, action_back2, NULL ));
    CCRepeatForever* pAction2 = CCRepeatForever::create(seq2);

    m_mysteryNote->runAction(pAction1);
	m_vipNote->runAction(pAction2);

    USERINFO userInfo = UserData::GetUserInfo();
    int times = userInfo.m_refreshTimes;
    if (times < 10)
    {
        m_mysteryNote->setVisible(false);
    } 
	else
    {
        m_mysteryNote->setVisible(true);
    }

	bool bIsBuyCurrentVipPack = MarketManager::Get()->isShopedCurrentVipPacks();
	if (bIsBuyCurrentVipPack)
	{
		m_vipNote->setVisible(false);
	}
	else
	{
		m_vipNote->setVisible(true);
	}
}

void MarketLayer::RefreshNote()
{
    USERINFO userInfo = UserData::GetUserInfo();
    int times = userInfo.m_refreshTimes;
    if (times < 10)
    {
        m_mysteryNote->setVisible(false);
    } 
	else
    {
        m_mysteryNote->setVisible(true);
    }

	bool bIsBuyCurrentVipPack = MarketManager::Get()->isShopedCurrentVipPacks();
	if (bIsBuyCurrentVipPack)
	{
		m_vipNote->setVisible(false);
	}
	else
	{
		m_vipNote->setVisible(true);
	}
}

bool MarketLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void MarketLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    TutorialsManager::Get()->HandleOneEvent("close", 1);

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(false);
    }
}

void MarketLayer::onSpriteClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    setCurrentTag(Tag_Sprite);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, (int) Tag_Sprite);
    InitLayer(m_currenTag);
}

void MarketLayer::onSpecialClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, (int) Tag_Special);
    setCurrentTag(Tag_Special);
    InitLayer(m_currenTag);
}

void MarketLayer::onVipClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, (int) Tag_Vip);
	//m_vipBtn->setEnabled(true);

    setCurrentTag(Tag_Vip);
    InitLayer(m_currenTag);
}

void MarketLayer::onMysteryClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    setCurrentTag(Tag_Mystery);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, (int) Tag_Mystery);
    InitLayer(m_currenTag);

    TutorialsManager::Get()->HandleOneEvent("mystery_btn", 1);
}

void MarketLayer::onChargeClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer* mainMenuLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    mainMenuLayer->ShowMarketLayer(false);
    mainMenuLayer->ShowChargeLayer(true);
}

void MarketLayer::onEnter()
{
    CCLayer::onEnter();

    m_spriteExtractBtn->setTouchPriority(kCCScrollMenuPriority);
    m_specialPriceBtn->setTouchPriority(kCCScrollMenuPriority);
    m_vipBtn->setTouchPriority(kCCScrollMenuPriority);
    m_mysteryBtn->setTouchPriority(kCCScrollMenuPriority);
    m_closeBtn->setTouchPriority(kCCScrollMenuPriority);
    m_chargeBtn->setTouchPriority(kCCScrollMenuPriority);

    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_spriteExtractBtn, (int) Tag_Sprite);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_specialPriceBtn,  (int) Tag_Special);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_vipBtn,           (int) Tag_Vip);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_mysteryBtn,       (int) Tag_Mystery);
	//this->removeChild(m_vipBtn);
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, m_currenTag);
}

void MarketLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void MarketLayer::InitLayer(MarketLayerTag tag)
{
    if (m_currentLayer != NULL)
    {
        m_sonLayer->removeChild(m_currentLayer, true);
        setCurrentLayer(NULL);
    }

    switch (tag)
    {
    case Tag_Sprite :
        setCurrentLayer(CreateSpriteExtractLayer());
        break;

    case Tag_Mystery :
        setCurrentLayer(CreateMysteryLayer());
        break;

    case Tag_Special :
        setCurrentLayer(CreateSpecialMarketLayer());
        break;

    case Tag_Vip :
        setCurrentLayer(CreateVipMarketLayer());
        break;

    default:
        setCurrentLayer(NULL);
    }

    m_currentLayer->setPosition(CCPointZero);
    m_currentLayer->setAnchorPoint(CCPointZero);

    m_sonLayer->addChild(m_currentLayer);
}

CCLayer* MarketLayer::CreateSpriteExtractLayer()
{
    CCNodeLoader* loader = new SpriteExtractLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("sprite_extract", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/sprite_extract");

    return dynamic_cast<CCLayer*>(node);
}

CCLayer* MarketLayer::CreateMysteryLayer()
{
    CCNodeLoader* loader = new MysteryLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("mystery", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/mystery");

    return dynamic_cast<CCLayer*>(node);
}

CCLayer* MarketLayer::CreateSpecialMarketLayer()
{
    CCNodeLoader* loader = new SpecialMarketLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("spec_market", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/spec_market");

    return dynamic_cast<CCLayer*>(node);
}

CCLayer* MarketLayer::CreateVipMarketLayer()
{
    CCNodeLoader* loader = new VipMarketLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("vip_market", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/vip_market");

    return dynamic_cast<CCLayer*>(node);
}

void MarketLayer::InitOpenLayer(int tag)
{
    setCurrentTag((MarketLayerTag) tag);
    InitLayer(m_currenTag);
}

void MarketLayer::UpdateShoppingInfo()
{
    if (dynamic_cast<SpecialMarketLayer*>(m_currentLayer) != NULL)
    {
        dynamic_cast<SpecialMarketLayer*>(m_currentLayer)->UpdateShoppingInfo();
    }

    if (dynamic_cast<VipMarketLayer*>(m_currentLayer) != NULL)
    {
        dynamic_cast<VipMarketLayer*>(m_currentLayer)->UpdateShoppingInfo();
    }
}

void MarketLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_UPDATE_SHOP_REFRESH_TIMES :
        {
            RefreshNote();
            break;
        }
	case GM_UPDATE_MARKET_TIME_INFO:
		{
            RefreshNote();
            break;
		}

    default:
        break;
    }
}

void MarketLayer::closeLayerCallBack(void)
{
	TutorialsManager::Get()->HandleOneEvent("close", 1);

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowMarketLayer(false,0,false);
    }
}

MarketUILayer::MarketUILayer(CCNode* parent) :
m_infoLayer(NULL)
{
    UILayerColorBase* pLayer =  new UILayerColorBase(true, true, ccc4(255,255,255,0));
    pLayer->init();
    pLayer->autorelease();
    pLayer->setPosition(CCPointZero);
    UILayout::m_pControlNode = pLayer;

    CCNode *pNode = getCurrentNode();
    parent->addChild(pNode);
}

MarketUILayer::~MarketUILayer()
{

}

CCNode* MarketUILayer::getTutorialNode(std::string name)
{
    if (strcmp("mystery_btn", name.c_str()) == 0)
    {
        return m_infoLayer->getMysteryBtn();
    } else
    {
        return NULL;
    }
}