#include "GuildListLayer.h"
#include "UIManager.h"
#include "MainMenuLayer.h"
#include "InterfaceManager.h"
#include "GMessage.h"
#include "ButtonPerformanceManager.h"
#include "Localization.h"
#include "TimeManager.h"
#include "GlobalFunction.h"
#include "GameAudioManager.h"
#include "LuaTinkerManager.h"

GuildListLayer::GuildListLayer() :
    m_titleLabel(NULL),
    m_searchLayer(NULL),
    m_pageLabel(NULL),
    m_leaderName(NULL),
    m_requireLabel(NULL),
    m_guildWord(NULL),
    m_leftBtn(NULL),
    m_rightBtn(NULL),
    m_joinBtn(NULL),
    m_createBtn(NULL),
    m_closeBtn(NULL),
    m_searchBox(NULL),
    m_button1(NULL),
    m_button2(NULL),
    m_button3(NULL),
    m_button4(NULL),
    m_button5(NULL),
    m_button6(NULL),
    m_pFire1(NULL),
    m_pFire2(NULL),
    m_pFire3(NULL),
    m_pCDLabel(NULL),
    m_pCDTime(NULL),
    m_pLookGBtn(NULL),
    m_totalPage(10),
    m_pageId(0),
    m_detailInfoLayer(NULL),
    m_bSearch(false),
    m_selectId(-1)
{
    m_contentLayers.clear();
    m_contentLayers.resize(6);
    m_buttons.clear();
    m_targetTimeStamp = 0;
}

GuildListLayer::~GuildListLayer()
{
    setTitleLabel(NULL);
    setSearchBox(NULL);
    setSearchLayer(NULL);
    setPageLabel(NULL);
    setLeaderName(NULL);
    setRequireLabel(NULL);
    setGuildWord(NULL);
    setLeftBtn(NULL);
    setRightBtn(NULL);
    setJoinBtn(NULL);
    setCreateBtn(NULL);
    setCloseBtn(NULL);
    setButton1(NULL);
    setButton2(NULL);
    setButton3(NULL);
    setButton4(NULL);
    setButton5(NULL);
    setButton6(NULL);
    setDetailInfoLayer(NULL);
    setFire1(NULL);
    setFire2(NULL);
    setFire3(NULL);
    setCDLabel(NULL);
    setCDTime(NULL);
    setLookGBtn(NULL);

    ButtonPerformanceManager::Get()->removePerformanceButton(this);
    NotificationCenter::defaultCenter()->unregisterAllMsgObserver(this);
    GuildManager::Get()->setIsRankNeedOpen(false);
}

SEL_MenuHandler GuildListLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildListLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close",   GuildListLayer::onCloseLayer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "create",  GuildListLayer::onCreateClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "button1_click", GuildListLayer::onButton1Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "button2_click", GuildListLayer::onButton2Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "button3_click", GuildListLayer::onButton3Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "button4_click", GuildListLayer::onButton4Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "button5_click", GuildListLayer::onButton5Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "button6_click", GuildListLayer::onButton6Clicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "left",          GuildListLayer::onLeftClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "right",         GuildListLayer::onRightClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "join",          GuildListLayer::onJoinClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "glook",         GuildListLayer::onGLookBtnClicked);
    return NULL;
}

bool GuildListLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "title",          CCLabelTTF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "search_layer",   CCLayer*,    m_searchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page_label",     CCLabelTTF*, m_pageLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "leader_name",    CCLabelTTF*, m_leaderName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "require",        CCLabelTTF*, m_requireLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_word",     CCLabelTTF*, m_guildWord);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "left_btn",       CCControlButton*, m_leftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "right_btn",      CCControlButton*, m_rightBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "join_btn",       CCControlButton*, m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "create_btn",     CCControlButton*, m_createBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",      CCControlButton*, m_closeBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ele_layer_1",    CCLayer*, m_contentLayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ele_layer_2",    CCLayer*, m_contentLayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ele_layer_3",    CCLayer*, m_contentLayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ele_layer_4",    CCLayer*, m_contentLayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ele_layer_5",    CCLayer*, m_contentLayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ele_layer_6",    CCLayer*, m_contentLayers[5]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button1", CCControlButton*, m_button1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button2", CCControlButton*, m_button2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button3", CCControlButton*, m_button3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button4", CCControlButton*, m_button4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button5", CCControlButton*, m_button5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button6", CCControlButton*, m_button6);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLookGBtn", CCControlButton*, m_pLookGBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "detail_info_layer", CCLayer*, m_detailInfoLayer);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFire1", CCSprite*, m_pFire1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFire2", CCSprite*, m_pFire2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFire3", CCSprite*, m_pFire3);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCDLabel", CCLabelTTF*, m_pCDLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCDTime",  CCLabelTTF*, m_pCDTime);
    return false;
}
  
void GuildListLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    if(GuildManager::Get()->getIsRankNeedOpen() == true)
    {
        m_joinBtn->setVisible(false);
        m_createBtn->setVisible(false);
    }

    NotificationCenter::defaultCenter()->registerAllMsgObserver(this);

    // 添加一个editBox
    CCScale9Sprite* back = CCScale9Sprite::createWithSpriteFrame(
        InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "g_input_frame.png"));
    setSearchBox(CCEditBox::create(m_searchLayer->getContentSize(), back));
    m_searchLayer->addChild(m_searchBox);
    m_searchBox->setAnchorPoint(ccp(0, 0));
    //m_searchBox->setPlaceHolder("hahahahahaa");
    m_searchBox->setReturnType(kKeyboardReturnTypeDone);
    m_searchBox->setFont("Helvetica", 20.0f);
    m_searchBox->setTouchPriority(kCCScrollMenuPriority - 1);
    m_searchBox->setMaxLength(12);
    m_searchBox->setDelegate(this);

    m_buttons.push_back(m_button1);
    m_buttons.push_back(m_button2);
    m_buttons.push_back(m_button3);
    m_buttons.push_back(m_button4);
    m_buttons.push_back(m_button5);
    m_buttons.push_back(m_button6);

    for (size_t i = 0; i < m_buttons.size(); ++i)
    {
        CCControlButton* button = m_buttons[i];
        ButtonPerformanceManager::Get()->addPerformanceButton(this, m_buttons[i], i);
    }

    // 先隐藏所有帮派
    for (size_t i = 0; i < m_contentLayers.size(); ++i)
    {
        CCLayer* layer = m_contentLayers[i];
        layer->setVisible(false);
    }

    // 像服务器请求帮派列表
    m_pageId = 1;
    GuildManager::Get()->SendGetGuildList(0, MAX_COUNT_PER_PAGE - 1);
    ShowGuildDetailInfo(0);

    m_totalPage = 10;
    ShowPageLabel();
}

void GuildListLayer::onBroadcastMessage(BroadcastMessage* msg)
{
    switch(msg->message)
    {
    case GM_UPDATE_GUILD_LIST :
        {
            InitLayer();
            break;
        }

    case GM_CREATE_GUILD_SUCCESS :
        {
            schedule(schedule_selector(GuildListLayer::CreateGuildSuccess), 0.0f);
            break;
        }

    default:
        break;
    }
}

void GuildListLayer::InitLayer()
{
    if (m_pageId == 0)
    {
        return;
    }

    if (m_bSearch == false)
    {
        unsigned int beginId = (m_pageId - 1) * MAX_COUNT_PER_PAGE;
        for (int i = 0; i < MAX_COUNT_PER_PAGE; ++i)
        {
            CCLayer* layer = m_contentLayers[i];
            int id = beginId + i;

            GuildListInfo info = GuildManager::Get()->getIndexGuildInfo(id);
            if (info.hightGid == 0 && info.lowGid == 0)
            {
                layer->setVisible(false);
            } else
            {
                layer->setVisible(true);
                InitSingleLayer(layer, info, id + 1);
            }
        }

        // 获取最大数量
        int totalAmount = GuildManager::Get()->getTotalGuildAmount();
        unsigned int totalPage = 0;
        if (totalAmount <= 6)
        {
            totalPage = 1;
        } else
        {
            totalPage = (totalAmount - 1) / 6 + 1;
        }
        m_totalPage = totalPage;

        m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_pageId, totalPage)->getCString());
    } else
    {
        unsigned int beginId = (m_pageId - 1) * MAX_COUNT_PER_PAGE;
        for (int i = 0; i < MAX_COUNT_PER_PAGE; ++i)
        {
            CCLayer* layer = m_contentLayers[i];
            int id = beginId + i;

            GuildListInfo info = GuildManager::Get()->getIndexSearchInfo(id);
            if (info.hightGid == 0 && info.lowGid ==0 )
            {
                layer->setVisible(false);
            } else
            {
                layer->setVisible(true);
                InitSingleLayer(layer, info, id + 1);
            }
        }

        // 获取最大数量
        int totalAmount = GuildManager::Get()->getTotalSearchCount();
        unsigned int totalPage = 0;
        if (totalAmount <= 6)
        {
            totalPage = 1;
        } else
        {
            totalPage = (totalAmount - 1) / 6 + 1;
        }
        m_totalPage = totalPage;

        m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_pageId, totalPage)->getCString());
    }

    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);
    ShowGuildDetailInfo(0);

    ProcessCDTime();
}

void GuildListLayer::ProcessCDTime()
{
    int cdTime = UserData::GetUserInfo().m_guild_join_cd_time;
    m_targetTimeStamp = cdTime;
    long nowTime = TimeManager::Get()->getCurServerTime();
    time_t delt = m_targetTimeStamp - nowTime;
    if(delt < 0)
    {
        m_pCDTime->setVisible(false);
        m_pCDLabel->setVisible(false);
    }

    schedule(schedule_selector(GuildListLayer::RefreshTime), 1.0f, kCCRepeatForever, 0.001f);
}

void GuildListLayer::RefreshTime(float f)
{
    long nowTime = TimeManager::Get()->getCurServerTime();
    time_t delt = m_targetTimeStamp - nowTime;
    if(delt < 0)
    {
        unschedule(schedule_selector(GuildListLayer::RefreshTime));
        m_pCDTime->setVisible(false);
        m_pCDLabel->setVisible(false);
    }
    else
    {
        m_pCDTime->setVisible(true);
        m_pCDLabel->setVisible(true);
        const char* strTemp = GetTimeStr(delt);
        m_pCDTime->setString(strTemp);
    }
}

void GuildListLayer::CreateGuildSuccess(float f)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildInfoLayer(true);
        mLayer->ShowGuildListLayer(false);
    }
}

void GuildListLayer::editBoxReturn(CCEditBox* editBox)
{
    // 请求服务器查询列表
    if (strcmp(editBox->getText(), "") == 0)
    {
        // 请求所有列表
        // 理论上已经有了，直接显示即可
        m_pageId = 1;
        m_bSearch = false;
        InitLayer();
    } else
    {
        m_bSearch = true;
        m_pageId = 1;
        GuildManager::Get()->SendSearchIndexGuild(std::string(editBox->getText()));
    }
}

void GuildListLayer::InitSingleLayer(CCLayer* layer, GuildListInfo info, int index)
{
    // 名次
    CCSprite* rankSprite = dynamic_cast<CCSprite*>(layer->getChildByTag(1));
    CCLabelTTF* rankLabel = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(7));
    if(index > 6)
    {
        m_pFire1->setVisible(false);
        m_pFire2->setVisible(false);
        m_pFire3->setVisible(false);
    }
    
    if (index <= 3)
    {
        CCSpriteFrame* frame = getRankSpriteFrame(index);
        if(frame)
        {
            rankSprite->setDisplayFrame(frame);
        }
        rankSprite->setVisible(true);
        rankLabel->setVisible(false);
        m_pFire1->setVisible(true);
        m_pFire2->setVisible(true);
        m_pFire3->setVisible(true);
    } else
    {
        rankSprite->setVisible(false);
        rankLabel->setVisible(true);
        rankLabel->setString(CCString::createWithFormat("%d", index)->getCString());
    }

    // 公会名称
    CCLabelTTF* nameLabel = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(2));
    nameLabel->setString(info.gname.c_str());

    // 公会等级
    CCLabelTTF* lvLabel = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(3));
    lvLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("G_LEVEL"), info.lv)->getCString());

    // 公会总战斗力
    CCLabelTTF* powerLabel = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(4));
    powerLabel->setString(CCString::createWithFormat("%d", info.total_power)->getCString());

    // 公会人数
    CCLabelTTF* memberLabel = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(5));
    CCString* tempStr = CCString::createWithFormat("%d/%d", info.member_num, info.member_num_limit);
    memberLabel->setString(tempStr->getCString());
    if(info.member_num == info.member_num_limit)
    {
        nameLabel->setColor(SYS_FONT_RED);
        lvLabel->setColor(SYS_FONT_RED);
        powerLabel->setColor(SYS_FONT_RED);
        memberLabel->setColor(SYS_FONT_RED);
    }
    else
    {
        if(info.hightGid == UserData::GetUserInfo().m_guildId.getHighID() && info.lowGid == UserData::GetUserInfo().m_guildId.getLowID())
        {
            nameLabel->setColor(SYS_FONT_GREEN);
            lvLabel->setColor(SYS_FONT_GREEN);
            powerLabel->setColor(SYS_FONT_GREEN);
            memberLabel->setColor(SYS_FONT_GREEN);
        }
        else
        {
            nameLabel->setColor(SYS_FONT_WHITE);
            lvLabel->setColor(SYS_FONT_WHITE);
            powerLabel->setColor(SYS_FONT_WHITE);
            memberLabel->setColor(SYS_FONT_WHITE);
        }
    }

    AddRequireFlag(layer, info);
}

void GuildListLayer::AddRequireFlag(CCLayer* layer, GuildListInfo info)
{
    // 公会申请标记
    CCSprite* gRequiredFlag = dynamic_cast<CCSprite*>(layer->getChildByTag(8));
    if(!gRequiredFlag)
    {
        return;
    }
    bool isRequire = info.isRequired;
    if(isRequire == false)
    {
        gRequiredFlag->setVisible(false);
    }
    else
    {
        gRequiredFlag->setVisible(true);
    }
}

CCSpriteFrame* GuildListLayer::getRankSpriteFrame(int index)
{
    switch (index)
    {
    case 1:
        return InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_first.png");
        break;

    case 2:
        return InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_2.png");
        break;

    case 3:
        return InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/rank.plist", "rank_third.png");
        break;

    default:
        return NULL;
        break;
    }
}

void GuildListLayer::ShowGuildDetailInfo(unsigned int index)
{
    m_selectId = index;

    // 首先取得指定的公会信息
    GuildListInfo info;
    unsigned int id = (m_pageId - 1) * MAX_COUNT_PER_PAGE + index;
    if (m_bSearch == false)
    {
        info = GuildManager::Get()->getIndexGuildInfo(id);
    } else
    {
        info = GuildManager::Get()->getIndexSearchInfo(id);
    }
    m_curInfo = info;

    if (info.hightGid == 0 && info.lowGid == 0)
    {
        m_detailInfoLayer->setVisible(false);
    } else
    {
        m_detailInfoLayer->setVisible(true);
        m_leaderName->setString(info.leader_name.c_str());
        if(info.conditionType == 1)
        {
            m_requireLabel->setString(Localizatioin::getLocalization("G_LIST_CONDITION_3"));
        }
        else if(info.conditionType == 2)
        {
            m_requireLabel->setString(CCString::createWithFormat(Localizatioin::getLocalization("G_LIST_CONDITION_1"), 
                                      info.fightcondition)->getCString());
        }
        else
        {
            m_requireLabel->setString(Localizatioin::getLocalization("G_LIST_CONDITION_2"));
        }

        if(strcmp(info.g_notice.c_str(), "") == 0)
        {
            m_guildWord->setString(Localizatioin::getLocalization("G_ANNOUNCEMENT_TEXT5"));
        }
        else
        {
            m_guildWord->setString(info.g_notice.c_str());
        }
    }
     
    // is self or not
    if(info.lead_id == 0)
    {
        return;
    }
    int selfId = UserData::getUserId().getID();
    int selfRegTm = UserData::getUserId().getRegTime();
    int selfChannel = UserData::getUserId().getChannel();
    if(selfId == info.lead_id && selfRegTm == info.lead_reg_time && selfChannel == info.lead_channel)
    {
        m_pLookGBtn->setVisible(false);
    }
    else
    {
        m_pLookGBtn->setVisible(true);
    }

    // get detail attributes of the guild leader
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Friend/playerfunccall.lua",
        "sendGetOtherUserinfo", info.lead_id, info.lead_reg_time, info.lead_channel);
}

void GuildListLayer::ShowPageLabel()
{
    m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_pageId, m_totalPage)->getCString());
}

void GuildListLayer::onCloseLayer(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildListLayer(false);
        if(GuildManager::Get()->getIsGuildInfoNeedOpen() == true)
        {
            mLayer->ShowGuildInfoLayer(true);
        }
    }
}

void GuildListLayer::onCreateClicked(CCObject* sender, CCControlEvent pControlEvent)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    int cdTime = UserData::GetUserInfo().m_guild_join_cd_time;
    long nowTime = TimeManager::Get()->getCurServerTime();
    time_t delt = cdTime - nowTime;
    if(delt > 0)
    {
        ShowMessage(Localizatioin::getLocalization("G_LIST_COOLING"));
        return;
    }

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildCreateLayer(true);
    }
}

void GuildListLayer::onButton1Clicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);
    ShowGuildDetailInfo(0);
}

void GuildListLayer::onButton2Clicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 1);
    ShowGuildDetailInfo(1);
}

void GuildListLayer::onButton3Clicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 2);
    ShowGuildDetailInfo(2);
}

void GuildListLayer::onButton4Clicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 3);
    ShowGuildDetailInfo(3);
}

void GuildListLayer::onButton5Clicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 4);
    ShowGuildDetailInfo(4);
}

void GuildListLayer::onButton6Clicked(CCObject* sender, CCControlEvent pControlEvent)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 5);
    ShowGuildDetailInfo(5);
}

void GuildListLayer::onLeftClicked(CCObject* sender, CCControlEvent event)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    if (m_pageId <= 1)
    {
        return;
    }

    m_pageId--;
    if (m_bSearch == false)
    {
        GuildManager::Get()->SendGetGuildList((m_pageId - 1) * MAX_COUNT_PER_PAGE, m_pageId * MAX_COUNT_PER_PAGE - 1);
    } else
    {
        InitLayer();
    }

    ShowPageLabel();
}

void GuildListLayer::onRightClicked(CCObject* sender, CCControlEvent event)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    if (m_pageId >= m_totalPage)
    {
        return;
    }

    m_pageId++;
    if (m_bSearch == false)
    {
        GuildManager::Get()->SendGetGuildList((m_pageId - 1) * MAX_COUNT_PER_PAGE, m_pageId * MAX_COUNT_PER_PAGE - 1);
    } else
    {
        InitLayer();
    }

    ShowPageLabel();
}

void GuildListLayer::onJoinClicked(CCObject* sender, CCControlEvent event)
{
    GameAudioManager::sharedManager()->playEffect(350000,false);
    if (m_selectId == -1)
    {
        return;
    }

    int cdTime = UserData::GetUserInfo().m_guild_join_cd_time;
    long nowTime = TimeManager::Get()->getCurServerTime();
    time_t delt = cdTime - nowTime;
    if(delt > 0)
    {
        ShowMessage(Localizatioin::getLocalization("G_LIST_COOLING"));
        return;
    }

    unsigned int id = (m_pageId - 1) * MAX_COUNT_PER_PAGE + m_selectId;
    GuildListInfo info;
    if (m_bSearch)
    {
        info = GuildManager::Get()->getIndexSearchInfo(id);
    } else
    {
        info = GuildManager::Get()->getIndexGuildInfo(id);
    }

    if (info.hightGid == 0 && info.lowGid == 0)
    {
        return;
    }

    GuildManager::Get()->SendJoinGuild(info.hightGid, info.lowGid);

    // 添加申请标记
    if(m_selectId >= m_contentLayers.size())
    {
        return;
    }
    CCLayer* layer = m_contentLayers[m_selectId];
    CCSprite* gRequiredFlag = dynamic_cast<CCSprite*>(layer->getChildByTag(8));
    if(gRequiredFlag)
    {
        gRequiredFlag->setVisible(true);
    }
}

void GuildListLayer::onGLookBtnClicked(CCObject* sender, CCControlEvent event)
{
    int openMenu =  LuaTinkerManager::Get()->callLuaFunc<int>("Script/Chat/ChatPlayerOption.lua","getChatPlayerOpen");

    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerId",m_curInfo.lead_id);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setRegtime",m_curInfo.lead_reg_time);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setChannel",m_curInfo.lead_channel);
    //LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerStrength",m_curInfo.);
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setPlayerName",m_curInfo.leader_name.c_str());
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setInitPlayer");
    LuaTinkerManager::Get()->callLuaFunc<void>("Script/Chat/ChatPlayerOption.lua","setModelLess", true);
}

void GuildListLayer::onEnter()
{
    CCLayer::onEnter();

    m_leftBtn->setTouchPriority(kCCScrollMenuPriority);
    m_rightBtn->setTouchPriority(kCCScrollMenuPriority);
    m_closeBtn->setTouchPriority(kCCScrollMenuPriority);
    m_createBtn->setTouchPriority(kCCScrollMenuPriority);
    m_joinBtn->setTouchPriority(kCCScrollMenuPriority);
    m_pLookGBtn->setTouchPriority(kCCScrollMenuPriority);

    for (size_t i = 0; i < m_buttons.size(); ++i)
    {
        m_buttons[i]->setTouchPriority(kCCScrollMenuPriority);
    }
}

void GuildListLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

bool GuildListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

GuildCreateLayer::GuildCreateLayer() :
    m_editLayer(NULL),
    m_costMoney(NULL),
    m_confirmButton(NULL),
    m_refuseButton(NULL),
    m_editBox(NULL)
{

}

GuildCreateLayer::~GuildCreateLayer()
{
    setEditLayer(NULL);
    setCostMoney(NULL);
    setConfirmButton(NULL);
    setRefuseButton(NULL);
    setEditBox(NULL);
}

SEL_MenuHandler GuildCreateLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildCreateLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "confirm", GuildCreateLayer::onConfirm);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "refuse",  GuildCreateLayer::onRefuse);
    return NULL;
}

bool GuildCreateLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "edit_layer",     CCLayer*,    m_editLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cost_money",     CCLabelTTF*, m_costMoney);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "confirm_btn", CCControlButton*, m_confirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "refuse_btn",  CCControlButton*, m_refuseButton);
    return false;
}

void GuildCreateLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    // 添加一个editBox
    CCScale9Sprite* back = CCScale9Sprite::createWithSpriteFrame(
        InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/guild.plist", "g_input_frame.png"));
    setEditBox(CCEditBox::create(m_editLayer->getContentSize(), back));
    m_editLayer->addChild(m_editBox);
    m_editBox->setAnchorPoint(ccp(0, 0));
    m_editBox->setPlaceHolder(Localizatioin::getLocalization("G_ESTABLISH_TEXT"));
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setFont("Helvetica", 20.0f);
    m_editBox->setTouchPriority(kCCLayerTouchPriority);
    m_editBox->setMaxLength(6);
}

bool GuildCreateLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildCreateLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildCreateLayer::onEnter()
{
    CCLayer::onEnter();
    m_confirmButton->setTouchPriority(kCCLayerTouchPriority);
    m_refuseButton->setTouchPriority(kCCLayerTouchPriority);
}

void GuildCreateLayer::onConfirm(CCObject* sender, CCControlEvent pControlEvent)
{
    if (strcmp(m_editBox->getText(), "") == 0)
    {
        ShowMessage(Localizatioin::getLocalization("G_ESTABLISH_TEXT"));
        return;
    }
    
    string textStr = m_editBox->getText();
    int len = strlen(textStr.c_str());
    if(len > 18)
    {
        textStr = textStr.substr(0, 18);
    }

    GuildManager::Get()->SendCreateGuild(textStr);
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildCreateLayer(false);
    }
}

void GuildCreateLayer::onRefuse(CCObject* sender, CCControlEvent pControlEvent)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildCreateLayer(false);
    }
}
