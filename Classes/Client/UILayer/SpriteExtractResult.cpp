#include "OnlineNetworkManager.h"
#include "SpriteExtractResult.h"
#include "UIDefine.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "LuaTinkerManager.h"
#include "InterfaceManager.h"
#include "ItemManager.h"
#include "SpriteElfManager.h"
#include "SkeletonAnimRcsManager.h"
#include "GameAudioManager.h"
#include "particle_manager.h"
#include "ParticleManager.h"
#include "TutorialsManager.h"
#include "MarketManager.h"
#include "SpriteFactory.h"
#include "LevelLayer.h"

SpriteExtractResultLayer::SpriteExtractResultLayer() :
    m_singleExtractLayer(NULL),
    m_mutipleExtractLayer(NULL),
    m_singlePosLayer(NULL),
    m_singleDetailBtn(NULL),
    m_singleReturnBtn(NULL),
    m_singleContinueBtn(NULL),
    m_mutipleReturnBtn(NULL),
    m_mutipleContinueBtn(NULL),
    m_currentType(0),
    m_costLayer(NULL),
    m_costDescLabel(NULL),
    m_costLabel(NULL),
    m_costSprite(NULL),
    m_singlePetName(NULL),
    m_currentId(0),
    m_singleLight(NULL),
    m_uiLayer(NULL)
{
    m_vtMutipleLayers.clear();
    m_vtMutipleLayers.resize(10);
    m_vtSprites.clear();
}

SpriteExtractResultLayer::~SpriteExtractResultLayer()
{
    setSingleExtractLayer(NULL);
    setMutipleExtractLayer(NULL);
    setSinglePosLayer(NULL);
    setSingleDetailBtn(NULL);
    setSingleReturnBtn(NULL);
    setSingleContinueBtn(NULL);
    setMutipleReturnBtn(NULL);
    setMutipleContinueBtn(NULL);
    setCostDescLabel(NULL);
    setCostLabel(NULL);
    setCostLayer(NULL);
    setCostSprite(NULL);
    setSinglePetName(NULL);

    UIManager::sharedManager()->RemoveLayout("sprite_extract_result");
    delete m_uiLayer;
}

SEL_MenuHandler SpriteExtractResultLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SpriteExtractResultLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "single_return",    SpriteExtractResultLayer::onSingleReturnClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "mutiple_return",   SpriteExtractResultLayer::onMutipleReturnClicked);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "single_continue",  SpriteExtractResultLayer::onSingleContinue);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "mutiple_continue", SpriteExtractResultLayer::onMutipleContinue);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "single_detail",    SpriteExtractResultLayer::onSingleDetail);

    return NULL;
}

bool SpriteExtractResultLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_extract",       CCLayer*,         m_singleExtractLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_extract",      CCLayer*,         m_mutipleExtractLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_pos_layer",     CCLayer*,         m_singlePosLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_detail_btn",    CCControlButton*, m_singleDetailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_return_btn",    CCControlButton*, m_singleReturnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_continue_btn",  CCControlButton*, m_singleContinueBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_return_btn",   CCControlButton*, m_mutipleReturnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_continue_btn", CCControlButton*, m_mutipleContinueBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_pet_name",      CCLabelTTF*,      m_singlePetName);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_light",         CCSprite*,        m_singleLight);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_desc",            CCLabelTTF*,      m_costDescLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost",                 CCLabelTTF*,      m_costLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_layer",           CCLayer*,         m_costLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_sprite",          CCSprite*,        m_costSprite);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_1",       CCLayer*,         m_vtMutipleLayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_2",       CCLayer*,         m_vtMutipleLayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_3",       CCLayer*,         m_vtMutipleLayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_4",       CCLayer*,         m_vtMutipleLayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_5",       CCLayer*,         m_vtMutipleLayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_6",       CCLayer*,         m_vtMutipleLayers[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_7",       CCLayer*,         m_vtMutipleLayers[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_8",       CCLayer*,         m_vtMutipleLayers[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_9",       CCLayer*,         m_vtMutipleLayers[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mutiple_pos_10",      CCLayer*,         m_vtMutipleLayers[9]);
    return false;
}

void SpriteExtractResultLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);
    setTouchEnabled(true);

    m_costLabel->setString("0");
    m_singleExtractLayer->setVisible(false);
    m_mutipleExtractLayer->setVisible(false);

    float scaleX = UIManager::sharedManager()->getScaleFactorX();
    float scaleY = UIManager::sharedManager()->getScaleFactorY();
    setScaleX(scaleX);
    setScaleY(scaleY);

    // 添加新手指引相关参数
    m_uiLayer = new SpriteExtractResultUILayer(this);
    m_uiLayer->setInfoLayer(this);
    UIManager::sharedManager()->AddLayout(m_uiLayer, "sprite_extract_result");
}

bool SpriteExtractResultLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void SpriteExtractResultLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint pt = pTouch->getLocation();

    // 检查10连抽再次招募按钮是否隐藏
    if (m_mutipleContinueBtn->isVisible() == false)
    {
        return;
    }

    // 计算相应的位置
    for (unsigned int i = 0; i < m_vtMutipleLayers.size(); ++i)
    {
        CCLayer* layer = m_vtMutipleLayers[i];
        CCSize size = layer->getContentSize();
        CCPoint dexPt = layer->convertToNodeSpace(pt);
        CCRect rect = CCRectMake(0, 0, size.width, size.height);
        if (rect.containsPoint(dexPt))
        {
            if (m_vtSprites.size() <= i)
            {
                return;
            }

            int id = m_vtSprites[i];
            LuaTinkerManager::Get()->callLuaFunc<void>("Script/FairyLayer/FairyInfoLayer.lua","ShowFairyInfoLayer", id);
            return;
        }
    }
}

void SpriteExtractResultLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
}

void SpriteExtractResultLayer::onEnter()
{
    CCLayer::onEnter();

    m_singleDetailBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_singleReturnBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_singleContinueBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_mutipleReturnBtn->setTouchPriority(kCCLayerMenuTouchPriority);
    m_mutipleContinueBtn->setTouchPriority(kCCLayerMenuTouchPriority);
}

void SpriteExtractResultLayer::SpriteOut(std::vector<int> vtSprite, int type)
{
    setCurrentType(type);
    m_vtSprites = vtSprite;

    GameAudioManager::sharedManager()->playEffect(350009,false);

    if (vtSprite.size() > 1 ||
        vtSprite.size() == 0)
    {
        SprietOutMutiple(vtSprite);
    } else
    {
        SpriteOutSingle(vtSprite[0]);
    }
}

void SpriteExtractResultLayer::SpriteOutSingle(int id)
{
    m_singleExtractLayer->setVisible(true);
    m_mutipleExtractLayer->setVisible(false);
    m_singleContinueBtn->setVisible(false);
    m_singleDetailBtn->setVisible(false);
    m_singleReturnBtn->setVisible(false);
    m_costLayer->setVisible(false);

    MainMenuLayer * layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    const char* iconStr = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/SpriteElf/ElfConfigParse.lua","GetOneElfIcon", id);
    if (iconStr == NULL)
    {
        if (layer != NULL)
        {
            layer->ShowSpriteExtractResultLayer(false);
        }

        return;
    }

    setCurrentId(id);

    // 添加宠物
    CCSize s = m_singleExtractLayer->getContentSize();
    SpriteFactory::sharedFactory()->ShowElfOnLayerAtPoint(m_singleExtractLayer, m_currentId, ccp(s.width / 2, s.height / 2 + 60));
    CCNode* elfNode = m_singleExtractLayer->getChildByTag(MAP_TAG_ELF);
    SpriteElf* elf = dynamic_cast<SpriteElf*>(elfNode);
    if (elf)
    {
        elf->getRoot()->setVisible(false);
    }

    // 设置光效
    SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(439);
    EffectSprite* effect = ParticleManager::Get()->createEffectSprite(439,"",false);
    if (effect)
    {
        effect->getRoot()->setScale(2.0f);
        m_singleExtractLayer->addChild(effect, 1, 300);
        CCSize s = m_singleExtractLayer->getContentSize();
        effect->setPosition(ccp(s.width / 2, s.height - 80));
        effect->SetAnim(kType_Play, 1, false);
    }

    m_singlePetName->setVisible(false);
    m_singleLight->setVisible(false);
    
    CCCallFuncN* func1 = CCCallFuncN::create(this, callfuncN_selector(SpriteExtractResultLayer::ShowPet));
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(SpriteExtractResultLayer::ShowBtn));
    this->runAction(CCSequence::create(CCDelayTime::create(EFFECT_TIME), func1, CCDelayTime::create(0.2f), func2, NULL));
}

void SpriteExtractResultLayer::SprietOutMutiple(std::vector<int> vtSprite)
{
    m_singleExtractLayer->setVisible(false);
    m_mutipleExtractLayer->setVisible(true);
    m_mutipleContinueBtn->setVisible(false);
    m_mutipleReturnBtn->setVisible(false);
    m_costLayer->setVisible(false);

    for (size_t i = 0; i < vtSprite.size(); ++i)
    {
        SkeletonAnimRcsManager::getInstance()->LoadOneSpriteElfRcs(vtSprite[i], true);
    }

    // 设置光效
    SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(439);
    EffectSprite* effect = ParticleManager::Get()->createEffectSprite(439,"",false);
    if (effect)
    {
        effect->getRoot()->setScale(2.0f);
        m_mutipleExtractLayer->addChild(effect, 1, 300);
        CCSize s = m_mutipleExtractLayer->getContentSize();
        effect->setPosition(ccp(s.width / 2, s.height - 80));
        effect->SetAnim(kType_Play, 1, false);
    }

    for (size_t i = 0; i < m_vtMutipleLayers.size(); ++i)
    {
        CCLayer* layer = m_vtMutipleLayers[i];
        layer->removeAllChildren();

        if (i >= vtSprite.size())
        {
            continue;
        }

        // 名字和框
        CCSprite* sprite = CCSprite::createWithSpriteFrame(InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", "name_back.png"));

        sprite->setAnchorPoint(ccp(0.5f, 0.5f));
        sprite->setPosition(ccp(60.0f, 60.0f));
        layer->addChild(sprite, 0, 100);
        sprite->setVisible(false);

        const char* name = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyNameByID", vtSprite[i]);

        CCLabelTTF* nameLabel = CCLabelTTF::create("", "Helvetica", 20.0f);
        if (name == NULL)
        {
            nameLabel->setString("");
        } else
        {
            nameLabel->setString(name);
        }

        // 设置颜色
        int quality = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyQualityByID", vtSprite[i]);
        ccColor3B color = ItemManager::Get()->getFairyLabelColorByQuality(quality);
        nameLabel->setColor(color);
        nameLabel->setVisible(false);

        nameLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        nameLabel->setPosition(ccp(60.0f, 60.0f));
        layer->addChild(nameLabel, 0, 101);

        CCSize s = layer->getContentSize();
        SpriteFactory::sharedFactory()->ShowElfOnLayerAtPoint(layer, vtSprite[i], ccp(s.width / 2, s.height / 2 + 40));
        SpriteElf* elf = dynamic_cast<SpriteElf*>(layer->getChildByTag(MAP_TAG_ELF));
        if (elf)
        {
            elf->getRoot()->setVisible(false);
        }

        CCDelayTime* delayAction1 = CCDelayTime::create(0.25f * (i) + EFFECT_TIME);
        CCDelayTime* delayAction2 = CCDelayTime::create(0.3f);
        CCFiniteTimeAction* scaleAction = CCScaleTo::create(0.15f, 1.0f);
        CCCallFuncN* showFun1 =  CCCallFuncN::create(this, callfuncN_selector(SpriteExtractResultLayer::ShowSprite));
        CCCallFuncN* showFun2 =  CCCallFuncN::create(this, callfuncN_selector(SpriteExtractResultLayer::ShowMutipleLight));

        layer->setScale(3.0f);
        layer->setVisible(true);
        layer->runAction(CCSequence::create(delayAction1, showFun2, delayAction2, showFun1, scaleAction, NULL));
    }

    CCCallFuncN* func1 = CCCallFuncN::create(this, callfuncN_selector(SpriteExtractResultLayer::ShowBtn));
    this->runAction(CCSequence::create(CCDelayTime::create(0.25f * vtSprite.size() + 0.25f + EFFECT_TIME), func1, NULL));
}

void SpriteExtractResultLayer::ShowPet(CCNode* node)
{
    const char* name = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyNameByID", m_currentId);

    if (name == NULL)
    {
        m_singlePetName->setString("");
    } else
    {
        m_singlePetName->setString(name);
    }

    // 设置颜色
    m_singlePetName->setVisible(true);
    int quality = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyQualityByID", m_currentId);
    ccColor3B color = ItemManager::Get()->getFairyLabelColorByQuality(quality);
    m_singlePetName->setColor(color);
    m_singlePetName->setScale(3.0f);
    m_singlePetName->runAction(CCScaleTo::create(0.15f, 1.0f));
    setCurrentId(0);

    CCNode* elfNode = m_singleExtractLayer->getChildByTag(MAP_TAG_ELF);
    SpriteElf* elf = dynamic_cast<SpriteElf*>(elfNode);
    if (elf != NULL)
    {
        elf->getRoot()->setVisible(true);
        elf->getRoot()->setScale(5.0f);
        elf->getRoot()->runAction(CCScaleTo::create(0.15f, 1.5f));
    }

    m_singleLight->setVisible(true);
    m_singleLight->stopAllActions();
    m_singleLight->runAction(CCRepeat::create(CCRotateBy::create(2.0f, 360.0f), kCCRepeatForever));
}

void SpriteExtractResultLayer::ShowMutipleLight(CCNode* node)
{
    CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect("particle_effect_upgradesuccess");
    node->addChild(particle, 2, 99);
    particle->setAnchorPoint(ccp(0.5, 0.5));
    particle->setPosition(ccp(60.0f, 140.0f));
    node->setVisible(true);
}

void SpriteExtractResultLayer::ShowBtn(CCNode* node)
{
    m_mutipleContinueBtn->setVisible(true);
    m_mutipleReturnBtn->setVisible(true);
    m_singleContinueBtn->setVisible(true);
    m_singleDetailBtn->setVisible(true);
    m_singleReturnBtn->setVisible(true);

    m_costLayer->setVisible(true);

    int cost = calcCost(m_currentType);
    m_costLabel->setString(CCString::createWithFormat("%d", cost)->getCString());

    // 检查类型
    if (m_currentType == 1)
    {
        CCSpriteFrame* frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", "4.png");
        m_costSprite->setDisplayFrame(frame);
    } else
    {
        CCSpriteFrame* frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/market.plist", "5.png");
        m_costSprite->setDisplayFrame(frame);
    }
}

int SpriteExtractResultLayer::calcCost(int type)
{
    // int greenCost  = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21070);
    // int blueCost   = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21071);
    // int purpleCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21072);
    // int yellowCost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21073);

    int greenCost = MarketManager::Get()->GetExchangePrice(21070);
    int blueCost = MarketManager::Get()->GetExchangePrice(21071);
    int purpleCost = MarketManager::Get()->GetExchangePrice(21072);
	int yellowCost = MarketManager::Get()->GetExchangePrice(21073);
	int redCost = MarketManager::Get()->GetExchangePrice(21074);

    switch (type)
    {
    case 1 :
        return greenCost;

    case 2 :
    case 3 :
        return blueCost;

    case 4 :
    case 5 :
        return purpleCost;

    case 6 :
        return yellowCost;

	case 7 :
		return redCost;
    default:
        return 0;
    }
}

void SpriteExtractResultLayer::ShowSprite(CCNode* node)
{
    if (node != NULL)
    {
        if (node->getChildByTag(100))
            node->getChildByTag(100)->setVisible(true);

        if (node->getChildByTag(101))
            node->getChildByTag(101)->setVisible(true);

        SpriteElf* elf = dynamic_cast<SpriteElf*>(node->getChildByTag(MAP_TAG_ELF));
        if (elf)
        {
            elf->getRoot()->setVisible(true);
        }
    }
}

void SpriteExtractResultLayer::onSingleReturnClicked(CCObject* sender, CCControlEvent event)
{
    TutorialsManager::Get()->HandleOneEvent("return", 1);

    MainMenuLayer * layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (layer != NULL)
    {
        layer->ShowSpriteExtractResultLayer(false);
    }
}

void SpriteExtractResultLayer::onMutipleReturnClicked(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (layer != NULL)
    {
        layer->ShowSpriteExtractResultLayer(false);
    }
}

void SpriteExtractResultLayer::onSingleDetail(CCObject* sender, CCControlEvent event)
{
    if (m_vtSprites.size() == 0)
    {
        return;
    }

    int id = m_vtSprites[0];

    LuaTinkerManager::Get()->callLuaFunc<void>("Script/FairyLayer/FairyInfoLayer.lua","ShowFairyInfoLayer", id);
}

void SpriteExtractResultLayer::onSingleContinue(CCObject* sender, CCControlEvent event)
{
    int type = m_currentType;
    if (m_currentType == 2)
    {
        type = 3;
    } else
    if (m_currentType == 4)
    {
        type = 5;
    }

    int cost = 0;
    if (type == 3)
    {
        cost = MarketManager::Get()->GetExchangePrice(21071);
        // cost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21071);
    } else if (type == 5)
    {
        cost = MarketManager::Get()->GetExchangePrice(21072);
        // cost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21072);
    } else
    if (type == 6)
    {
        cost = MarketManager::Get()->GetExchangePrice(21073);
        // cost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21073);
	} else
	if (type == 7)
	{
		cost = MarketManager::Get()->GetExchangePrice(21074);
		// cost = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", 21074);
	}

    if (UserData::GetDiamond() < cost)
    {
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua","SetRequireDiamondCount",cost);
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/UILayer/PayHintLayer.lua", "CheckPayHint");
        return;
    }

    OnlineNetworkManager::sShareInstance()->SendExtractSprite(type);
}

void SpriteExtractResultLayer::closeLayerCallBack(void)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
		mLayer->ShowSpriteExtractResultLayer(false);
    }
}

void SpriteExtractResultLayer::onMutipleContinue(CCObject* sender, CCControlEvent event)
{
    int type = m_currentType;
    if (m_currentType == 2)
    {
        type = 3;
    } else
    if (m_currentType == 4)
    {
        type = 5;
    }

    OnlineNetworkManager::sShareInstance()->SendExtractSprite(type);
}

SpriteExtractResultUILayer::SpriteExtractResultUILayer(CCNode* parent) :
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

SpriteExtractResultUILayer::~SpriteExtractResultUILayer()
{

}

CCNode* SpriteExtractResultUILayer::getTutorialNode(std::string name)
{
    if (strcmp("return", name.c_str()) == 0)
    {
        return m_infoLayer->getSingleReturnBtn();
    } else
    {
        return NULL;
    }
}