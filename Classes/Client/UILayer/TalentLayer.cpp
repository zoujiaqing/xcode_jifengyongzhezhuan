//
//  TalentLayer.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-5-13.
//
//
#include "PVPMessage.h"
#include "TalentLayer.h"
#include "GameResourceManager.h"
#include "Localization.h"
#include "GameFontManager.h"
#include "GMessage.h"
#include "particle_manager.h"
#include "UserData.h"
#include "GameAudioManager.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "LuaTinkerManager.h"
#include "ItemManager.h"

using namespace TXGUI;

long TalentLayer::m_stopTimer = 0;
bool TalentLayer::m_TalentLevelupFlag = false;

TalentLayer::TalentLayer()
{
    this->m_pLayer = NULL;
    this->m_pList = NULL;
    this->m_pUseDiamand = NULL;
    this->m_plevelupBtn = NULL;
    this->m_bTimeLevelUp = true;
    
    this->m_goldCoinNum = 0;
    this->m_diamondNum = 0;
    this->m_gongxiNum = 0;
    this->m_talent_cnt = 0;
    
    this->m_bCloseLayerCallBack = false;
    
    this->m_dt = 0;
    this->m_cnt = 60;
    this->m_musicTime = 0;
    
    this->m_curTalentID = 0;
    this->m_curLevel = 0;
    
    this->m_eachplus = 0;
}

TalentLayer::~TalentLayer()
{
    UIManager::sharedManager()->RemoveUILayout("TalentUI");
	UIManager::sharedManager()->RemoveUILayout("UseDiamand_Talent");
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void TalentLayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

bool TalentLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    ///show ppve panel
	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/TalentUI.plist", this, "TalentUI", true);
    
    m_pUseDiamand = manager->CreateUILayoutFromFile("UIplist/UseDiamand_Talent.plist", this, "UseDiamand_Talent", true);
    manager->RegisterMenuHandler("UseDiamand_Talent","TalentLayer::OnConfirm",	menu_selector(TalentLayer::OnConfirm), this);
    manager->RegisterMenuHandler("UseDiamand_Talent","TalentLayer::OnCancle",	menu_selector(TalentLayer::OnCancle), this);
    m_pUseDiamand->setVisible(false);
    setDiamandTxt();
    
    manager->RegisterMenuHandler("TalentUI","TalentLayer::OnClose",	menu_selector(TalentLayer::OnClose), this);
	manager->RegisterMenuHandler("TalentUI","TalentLayer::OnClickListItem",	menu_selector(TalentLayer::OnClickListItem), this);
    manager->RegisterMenuHandler("TalentUI","TalentLayer::OnLevelUpTalent",	menu_selector(TalentLayer::OnLevelUpTalent), this);
    
	m_pList = m_pLayer->FindChildObjectByName<UIScrollList>("talent_characterScrollList");
    //m_pList->setTouchEnabled(false);
    
    m_plevelupBtn = m_pLayer->FindChildObjectByName<UIButton>("talent_updateBtn");
    std::string strname = getLocationTxt(101);
    UILabelTTF *label = UILabelTTF::create(strname.c_str(), KJLinXin, 30);
    m_plevelupBtn->getAnimationNode()->addChild(label,100, 100);
    
    showTalentDetail(false);
    PVPMessage::Get()->getTalentList();
    updateCoinAndDiamand();
	return true;
}

void TalentLayer::updateCoinAndDiamand()
{
    int goldnum,diamandnum;
    UILabel* gold_label = m_pLayer->FindChildObjectByName<UILabel>("talent_gold_number");
    UILabel* diamand_label = m_pLayer->FindChildObjectByName<UILabel>("talent_diamand_number");
    USERINFO info =  UserData::GetUserInfo();
    goldnum = info.m_diamond;
    diamandnum = info.m_player_exploit;
    
    char buf[10];
    sprintf(buf, "%d", goldnum);
    gold_label->setString(buf);
    /*if(strlen(buf) >= 7)
    {
        std::string str = buf;
        str += "k";
        gold_label->setString(str.c_str());
    }*/
    //sprintf(buf, "%d", diamandnum);
	const char* exploitStr = MainMenuLayer::GetNumByFormat(diamandnum);
    diamand_label->setString(exploitStr);
}

void TalentLayer::convertSecToMinute(int time)
{
    m_musicTime = time /60;
    m_cnt = time % 60;
    return;
}

void TalentLayer::updateLevel(int level, int plus, int exploit)
{
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui2.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    UILabel* levelLabel = m_pLayer->FindChildObjectByName<UILabel>("talent_level_sel");
    UILabel* plusLabel = m_pLayer->FindChildObjectByName<UILabel>("talent_add_sel");
    UILabel* needHonorLabel = m_pLayer->FindChildObjectByName<UILabel>("talent_number_needhonor");
    
    char buf[10];
    std::string strname = "";
    sprintf(buf, "%d\n", level);
    strname = "Lv";
    strname += buf;
    levelLabel->setString(strname.c_str());
    levelLabel->setColor(ccWHITE);
    
    sprintf(buf, "%d", plus);
    std::string strname2 = "+";
    strname2 += buf;
    plusLabel->setString(strname2.c_str());
    
    sprintf(buf, "%d", exploit);
    needHonorLabel->setString(buf);
    
    for(size_t i=0; i<m_TalentQueries.size(); i++)
    {
        if(m_curTalentID == m_TalentQueries[i].talent_id)
        {
            CCLabelTTF* leveltable = m_LevelLabels[i];
            leveltable->setString(strname.c_str());
            
            CCLabelTTF* plusLabel = m_PlusLabels[i];
            plusLabel->setString(strname2.c_str());
            
            m_TalentQueries[i].level = level;
        }
    }
    
    UILabel* nextlevel2 = m_pLayer->FindChildObjectByName<UILabel>("talent_nextlevel2_sel");
    sprintf(buf,"%d\n", level+1);
    strname = "Lv";
    strname += buf;
    nextlevel2->setString(strname.c_str());
    
    UIPicture *Icon = m_pLayer->FindChildObjectByName<UIPicture>("talent_icon_sel");
    CCNode* node = Icon->getCurrentNode();
    showSpriteParticleEffect(node,474014,ccp(node->getContentSize().width * 0.5,node->getContentSize().height * 0.5));
    GameAudioManager::sharedManager()->playEffect(350006,false);
    updateCoinAndDiamand();
}

void TalentLayer::startCD(int timeout)
{
    if(timeout != 0)
    {
        convertSecToMinute(timeout);
        
        char buf[10];
        sprintf(buf, "%d", m_musicTime);
        std::string strname = buf;
        
        strname += ":";
        char buf2[10];
        sprintf(buf2, "%d", m_cnt);
        
        if(m_cnt < 10)
        {
            strname += "0";
            strname += buf2;
        }
        else
            strname += buf2;
        
        UILabelTTF *label = UILabelTTF::create(strname.c_str(), KJLinXin, 30);
        m_plevelupBtn->getAnimationNode()->removeChildByTag(100);
        m_plevelupBtn->getAnimationNode()->addChild(label, 100, 100);
        
        unschedule(schedule_selector(TalentLayer::updateCDTime));
        schedule(schedule_selector(TalentLayer::updateCDTime),0);
        m_bTimeLevelUp = false;
    }
    else
    {
        unschedule(schedule_selector(TalentLayer::updateCDTime));
        std::string strname = getLocationTxt(101);
        UILabelTTF *label = UILabelTTF::create(strname.c_str(), KJLinXin, 30);
        m_plevelupBtn->getAnimationNode()->removeChildByTag(100);
        m_plevelupBtn->getAnimationNode()->addChild(label,100, 100);
        m_bTimeLevelUp = true;
    }
}

void TalentLayer::addNewTalent(int talent_id, int level_id, int plus)
{
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui2.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    
    CCPoint pt = CCPointZero;
	int idx = TXGUI::getResourceIDByName("map_ui2_FRAME_GENIUS_BUTTON_TALENT1");
    CCSprite* normal = pAsprite->getSpriteFromFrame_Middle(idx ,0, pt);
	idx = TXGUI::getResourceIDByName("map_ui2_FRAME_GENIUS_BUTTON_TALENT1_SELECTED");
    CCSprite* clicked= pAsprite->getSpriteFromFrame_Middle(idx, 0, pt);
    
    UIButton* pItem = m_pList->AddNewItem(normal,
                                          clicked,
                                          "TalentLayer::OnClickListItem",
                                          "",
                                          ccGREEN,
                                          kCCScrollMenuPriority);
   

    
  
    
    if(pItem)
    {
		 pItem->setTag(talent_id);
		if(m_talent_cnt == 0)
			pItem->getMenuItemSprite()->selected();
        m_talent_cnt += 1;
		int idx = TXGUI::getResourceIDByName("map_ui2_FRAME_GENIUS_ICONGRID_TALENT1");
        CCSprite* icon_frame = pAsprite->getSpriteFromFrame_Middle(idx ,0, pt);
        CCSprite* icon = ItemManager::Get()->getIconSpriteById(talent_id);
        icon_frame->addChild(icon,-1);
        icon->setPosition(ccp(icon_frame->getContentSize().width/2, icon_frame->getContentSize().height/2));
        
        std::string strname = getLocationTxt(talent_id);
        CCLabelTTF* name = CCLabelTTF::create(strname.c_str(), KJLinXin, 20);
        name->setColor(ccWHITE);
        
        char buf[10];
        sprintf(buf, "%d\n", level_id);
        strname = "Lv";
        strname += buf;
        
        CCLabelTTF* level = CCLabelTTF::create(strname.c_str(), KJLinXin, 14);
        level->setColor(ccWHITE);
        m_LevelLabels.push_back(level);
        
        sprintf(buf, "%d\n", plus);
        strname = "+";
        strname += buf;
        CCLabelTTF* addlabel = CCLabelTTF::create(strname.c_str(), KJLinXin, 14);
        addlabel->setColor(ccWHITE);
        m_PlusLabels.push_back(addlabel);
        
        pItem->getAnimationNode()->addChild(icon_frame,10);
        pItem->getAnimationNode()->addChild(name,10);
        pItem->getAnimationNode()->addChild(level,10);
        pItem->getAnimationNode()->addChild(addlabel,10);
        
        icon_frame->setPosition(ccp(icon_frame->getContentSize().width/2 - normal->getContentSize().width/2, 0));
        name->setPosition(ccp(0, normal->getContentSize().height/2-name->getContentSize().height-5));
        level->setPosition(ccp(0, -normal->getContentSize().height/2 + level->getContentSize().height+5));
        addlabel->setPosition(ccp(normal->getContentSize().width/2 - addlabel->getContentSize().width/2-20, -addlabel->getContentSize().height/2));
        
        TalentQuery qu;
        qu.talent_id = talent_id;
        qu.level = level_id;
        m_TalentQueries.push_back(qu);
	    m_UIButtonlist.push_back(pItem);
    }
}

void TalentLayer::showOneTalent(int index, int talent_id, int headIconId, int roleLevel, int addNumber, int attackLevel, int levelNeed, int GongXi)
{
    ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui2.bin");
    pAsprite->setImageName(0, "UI/ui.png");
    
    UIPicture *frame = m_pLayer->FindChildObjectByName<UIPicture>("talent_frame_sel");
    UILabel* name = m_pLayer->FindChildObjectByName<UILabel>("talent_name_sel");
    UILabel* level = m_pLayer->FindChildObjectByName<UILabel>("talent_level_sel");
    
    UILabel* add = m_pLayer->FindChildObjectByName<UILabel>("talent_add_sel");
    UIPicture *Icon = m_pLayer->FindChildObjectByName<UIPicture>("talent_icon_sel");
    
    UILabel* attr = m_pLayer->FindChildObjectByName<UILabel>("talent_attri_sel");
    UILabel* effect = m_pLayer->FindChildObjectByName<UILabel>("talent_attri_effect");

    UILabel* nextlevel = m_pLayer->FindChildObjectByName<UILabel>("talent_nextlevel_sel");
    UILabel* rolelevelTxt = m_pLayer->FindChildObjectByName<UILabel>("talent_roleLevel");

    UILabel* nextlevel2 = m_pLayer->FindChildObjectByName<UILabel>("talent_nextlevel2_sel");
    UILabel* updatedate = m_pLayer->FindChildObjectByName<UILabel>("talent_updatedate_sel");
    UILabel* number = m_pLayer->FindChildObjectByName<UILabel>("talent_number_needhonor");
    
    frame->setVisible(true);
    name->setVisible(true);
    level->setVisible(true);
    effect->setVisible(true);
    add->setVisible(true);
    Icon->setVisible(true);
    
    attr->setVisible(true);
    nextlevel->setVisible(true);
    rolelevelTxt->setVisible(true);
    nextlevel2->setVisible(true);
    
    updatedate->setVisible(true);
    number->setVisible(true);
    
    CCPoint pt;
    CCNode* node = Icon->getCurrentNode();
    node->removeChildByTag(10, true);
    
    CCSprite* icon = ItemManager::Get()->getIconSpriteById(talent_id);//pAsprite2->getSpriteFromFrame_Middle(frameId ,0, pt);
    node->addChild(icon,-1,10);
    icon->setPosition(ccp(node->getContentSize().width/2, node->getContentSize().height/2));
    
    std::string strname = "";
    char buf[10];
    
    index += 1;
    strname = getLocationTxt(talent_id);
    name->setString(strname.c_str());
    name->setColor(ccWHITE);
    
    sprintf(buf, "%d\n", roleLevel);
    strname = "Lv";
    strname += buf;
    level->setString(strname.c_str());
    level->setColor(ccWHITE);
    
    strname = "";
    strname += "+";
    sprintf(buf, "%d\n", addNumber);
    strname += buf;
    add->setString(strname.c_str());
    add->setColor(ccWHITE);
    
    sprintf(buf, "%d\n", attackLevel);
    strname = getLocationTxt(talent_id*10);
    strname += "+";
    strname += buf;
    attr->setString(strname.c_str());
    attr->setColor(ccWHITE);
    
    strname = Localizatioin::getLocalization("M_UPGRADE_EFFECT");
    effect->setString(strname.c_str());
    effect->setColor(ccORANGE);
    
    strname = getLocationTxt(100);
    strname += ":";
    nextlevel->setString(strname.c_str());
    nextlevel->setColor(ccORANGE);
    
    strname = Localizatioin::getLocalization("M_ROLELEVEL");
    rolelevelTxt->setString(strname.c_str());
    rolelevelTxt->setColor(ccWHITE);
    
    sprintf(buf,"%d\n", levelNeed+1);
    strname = "Lv";
    strname += buf;
    nextlevel2->setString(strname.c_str());
    nextlevel2->setColor(ccRED);

    int selflevel = UserData::GetUserLevel();
    if(selflevel >= levelNeed+1)
        nextlevel2->setColor(ccWHITE);
    
    strname = Localizatioin::getLocalization("M_UPGRADECOST");
    strname += ":";
    updatedate->setString(strname.c_str());
    updatedate->setColor(ccORANGE);
    
    sprintf(buf, "%d\n", GongXi);
    number->setString(buf);
    number->setColor(ccWHITE);
}

void TalentLayer::updateCDTime(float dt)
{
    if((m_musicTime == 0 && m_cnt == 0) || m_musicTime < 0)
    {
        unscheduleUpdate();
        std::string strname = getLocationTxt(101);
        UILabelTTF *label = UILabelTTF::create(strname.c_str(), KJLinXin, 30);
        m_plevelupBtn->getAnimationNode()->removeChildByTag(100);
        m_plevelupBtn->getAnimationNode()->addChild(label, 100, 100);
        m_bTimeLevelUp = true;
        return;
    }
    
    m_dt += dt;
    if(m_dt >= 1.0f)
    {
        m_dt = 0;
        m_cnt -= 1;
        if(m_cnt <= 0)
        {
            m_cnt = 59;
            m_musicTime -= 1;
            
            if((m_musicTime == 0 && m_cnt == 0) || m_musicTime < 0)
            {
                unscheduleUpdate();
                std::string strname = getLocationTxt(101);
                UILabelTTF *label = UILabelTTF::create(strname.c_str(), KJLinXin, 30);
                m_plevelupBtn->getAnimationNode()->removeChildByTag(100);
                m_plevelupBtn->getAnimationNode()->addChild(label, 100, 100);
                m_bTimeLevelUp = true;
                return;
            }
        }
        
        std::string strtime = "";
        char buf[20] = "";
        sprintf(buf, "%d", m_musicTime);
        
        strtime += buf;
        strtime += ":";
        
        char buf2[10];
        sprintf(buf2, "%d", m_cnt);
        
        if(m_cnt < 10)
        {
            strtime += "0";
            strtime += buf2;
        }
        else
            strtime += buf2;
        
        setUpCDTime(strtime.c_str());
    }
}

//点击天赋
void TalentLayer::OnClickListItem(CCObject* sender)
{
    CCNode* node = (CCNode*)sender;
    if(node)
    {
        UIMenu* menu = dynamic_cast<UIMenu*>(node->getParent());
        if(menu)
        {
            UIControlBase* base = menu->getControlNode();
            int tag = base->getTag();
            
            for(size_t i=0; i<m_TalentQueries.size(); i++)
            {
                TalentQuery qu = m_TalentQueries[i];
                if(qu.talent_id == tag)
                {
                    PVPMessage::Get()->getOneTalent(qu.talent_id, qu.level);
                    setcurTalentID(qu.talent_id);
                    setcurLevel(qu.level);
                    
                    UIButton* pbtn = m_UIButtonlist[i];
                    pbtn->getMenuItemSprite()->selected();
                    
                    for(size_t j=0; j<m_UIButtonlist.size(); j++)
                    {
                        if(j != i)
                            m_UIButtonlist[j]->getMenuItemSprite()->unselected();
                    }
                    
                    break;
                }
            }
        }
    }
}

//关闭天赋系统
void TalentLayer::OnClose(CCObject *sender)
{
    unschedule(schedule_selector(TalentLayer::updateCDTime));
    
    
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    //mLayer->HideTalentLayer(false);
	ShowChatBar(true,  -1);
	NotificationCenter::defaultCenter()->broadcast(GM_CLOSE_TALENTLAYER, this);
}

//升级天赋
void TalentLayer::OnLevelUpTalent(CCObject* sender)
{
    if(m_bTimeLevelUp)
    {
        PVPMessage::Get()->levelUpTalent(m_curTalentID);
    }
    else
    {
        m_pUseDiamand->setVisible(true);
        updateSpendDiamand();
    }
}

void TalentLayer::setDiamandTxt()
{
    UILabel* confirmLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_confirm");
    UILabel* cancleLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_cancel");
    UILabel* tileLabel = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_tile");
    
    std::string msg = "";
    msg = Localizatioin::getLocalization("M_CONFIRM");
    confirmLabel->setString(msg.c_str());

    msg = Localizatioin::getLocalization("M_CANCEL");
    cancleLabel->setString(msg.c_str());

    msg = Localizatioin::getLocalization("M_SKIP_CD");
    tileLabel->setString(msg.c_str());
    
    confirmLabel->setColor(ccWHITE);
    cancleLabel->setColor(ccWHITE);
    tileLabel->setColor(ccWHITE);
}

void TalentLayer::OnConfirm(CCObject *sender)
{
    CCLOG("TalentLayer OnConfirm\n");
    m_pUseDiamand->setVisible(false);
    PVPMessage::Get()->sendSpeedUpReq(10100);
}

void TalentLayer::OnCancle(CCObject *sender)
{
    CCLOG("TalentLayer OnCancle\n");
    m_pUseDiamand->setVisible(false);
}

void TalentLayer::clearScrollist()
{
	if (m_pList)
	{
		m_pList->clearChildrens();
        m_TalentQueries.clear();
		m_UIButtonlist.clear();
	}
}

void TalentLayer::setUpCDTime(const char* strtime)
{
    UILabelTTF *label = UILabelTTF::create(strtime, KJLinXin, 30);
    m_plevelupBtn->getAnimationNode()->removeChildByTag(100);
    m_plevelupBtn->getAnimationNode()->addChild(label,100,100);
}

std::string TalentLayer::getLocationTxt(int location_id)
{
    std::string msg = "";
    switch (location_id) {
            
        case 416901:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_PHYATTACK");
            break;
            
        case 416902:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_MAGATTACK");
            break;
            
        case 416903:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_SKIATTACK");
            break;
            
        case 416904:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_PHYDEFENCE");
            break;
            
        case 416905:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_MAGDEFENCE");
            break;
            
        case 416906:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_SKIDEFENCE");
            break;
            
        case 416907:
            msg = Localizatioin::getLocalization("M_ATTRIBUTE_LIFE");
            break;
            
        case 4169010:
            msg = Localizatioin::getLocalization("M_GENIUS_PHYATTACK");
            break;

        case 4169020:
            msg = Localizatioin::getLocalization("M_GENIUS_MAGATTACK");
            break;
        
        case 4169030:
            msg = Localizatioin::getLocalization("M_GENIUS_SKIATTACK");
            break;
        
        case 4169040:
            msg = Localizatioin::getLocalization("M_GENIUS_PHYDEFENCE");
            break;

        case 4169050:
            msg = Localizatioin::getLocalization("M_GENIUS_MAGDEFENCE");
            break;
            
        case 4169060:
            msg = Localizatioin::getLocalization("M_GENIUS_SKIDEFENCE");
            break;
            
        case 4169070:
            msg = Localizatioin::getLocalization("M_GENIUS_LIFE");
            break;
            
        case 100:
            msg = Localizatioin::getLocalization("M_UPGRADENEED");
            break;
            
        case 101:
            msg = Localizatioin::getLocalization("M_GENIUS_UPGRADE");
            break;
            
        default:
            break;
    }
    return msg;
}

void TalentLayer::closeLayerCallBack( void )
{
	m_bCloseLayerCallBack = true;
	OnClose(NULL);
	m_bCloseLayerCallBack = false;
}

void TalentLayer::showOne(unsigned int talent_id,
                          unsigned int level,
                          
                          unsigned int plus_add,
                          unsigned int next_exploit,
                          unsigned int next_time,
                          
                          unsigned int diff_physical_attack,
                          unsigned int diff_magic_attack,
                          unsigned int diff_skill_attack,
                                                    unsigned int diff_physical_defence,
                          unsigned int diff_magic_defence,
                          unsigned int diff_skill_defence,
                          
                          unsigned int diff_hp)
{
    for(size_t i = 0; i < m_TalentQueries.size(); i++)
    {
        if(m_TalentQueries[i].talent_id == talent_id)
        {
            
            if(talent_id == 416901)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_physical_attack, level, next_exploit);
            else if(talent_id == 416902)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_magic_attack, level, next_exploit);
            else if(talent_id == 416903)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_skill_attack, level, next_exploit);
            else if(talent_id == 416904)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_physical_defence, level, next_exploit);
            else if(talent_id == 416905)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_magic_defence, level, next_exploit);
            else if(talent_id == 416906)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_skill_defence, level, next_exploit);
            else if(talent_id == 416907)
                showOneTalent(i, talent_id, -1, level, plus_add, diff_hp, level, next_exploit);
            return;
        }
    }
}

void TalentLayer::showTalentDetail(bool visible)
{
	ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("UI/ui2.bin");
	pAsprite->setImageName(0, "UI/ui.png");

	UIPicture *frame = m_pLayer->FindChildObjectByName<UIPicture>("talent_frame_sel");
	UILabel* name = m_pLayer->FindChildObjectByName<UILabel>("talent_name_sel");
	UILabel* level = m_pLayer->FindChildObjectByName<UILabel>("talent_level_sel");

	UILabel* add = m_pLayer->FindChildObjectByName<UILabel>("talent_add_sel");
	UIPicture *Icon = m_pLayer->FindChildObjectByName<UIPicture>("talent_icon_sel");

	UILabel* attr = m_pLayer->FindChildObjectByName<UILabel>("talent_attri_sel");
    UILabel* effect = m_pLayer->FindChildObjectByName<UILabel>("talent_attri_effect");
	UILabel* nextlevel = m_pLayer->FindChildObjectByName<UILabel>("talent_nextlevel_sel");
	UILabel* nextlevel2 = m_pLayer->FindChildObjectByName<UILabel>("talent_nextlevel2_sel");

	UILabel* updatedate = m_pLayer->FindChildObjectByName<UILabel>("talent_updatedate_sel");
	UILabel* number = m_pLayer->FindChildObjectByName<UILabel>("talent_number_needhonor");

	frame->setVisible(visible);
	name->setVisible(visible);
	level->setVisible(visible);
    effect->setVisible(visible);
	add->setVisible(visible);
	Icon->setVisible(visible);

	attr->setVisible(visible);
	nextlevel->setVisible(visible);
	nextlevel2->setVisible(visible);

	updatedate->setVisible(visible);
	number->setVisible(visible);
}

void TalentLayer::showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt)
{
    std::string particleName = "particle_effect_upgradesuccess";
    CCNode* particle = ParticleManagerX::sharedManager()->getParticles_uieffect(particleName.c_str());
    if(particle != NULL)
    {
        CCNode* child = parent->getChildByTag(100);
        if(child)
        {
            child->removeFromParentAndCleanup(true);
            child = NULL;
        }
        parent->addChild(particle,100,100);
        particle->setPosition(pt);
    }
}

int TalentLayer::talentIDToFrameId(int talent_id)
{
    return 0;
}

void TalentLayer::updateSpendDiamand()
{
    const char* fromItems = LuaTinkerManager::Get()->getLuaConfig<const char *>("ExchangeParameter", "ExchangeParameter", 10100, "FromItems");
    string exchangeStr = string(fromItems);
    int dashPos = exchangeStr.find_first_of("/");
    if(dashPos != exchangeStr.npos)
    {
        unsigned int length = exchangeStr.size();
        std::string idStr = exchangeStr.substr(0,dashPos);
        std::string numStr = exchangeStr.substr(dashPos+1,length-1);
        int id = atoi(idStr.c_str());
        int num = atoi(numStr.c_str());
        ASSERT(id == 416002,"cost is not diamond!");
        
        unsigned int diamond = m_musicTime*60+m_cnt;
        float fd = 1.0f*diamond/60;
        diamond = (int)ceilf(fd);
        diamond *= num;
        
        UILabel* diamand_label = m_pUseDiamand->FindChildObjectByName<UILabel>("useDia_num");
        char buf[10];
        sprintf(buf, "%d", diamond);
        diamand_label->setString(buf);
    }
}





