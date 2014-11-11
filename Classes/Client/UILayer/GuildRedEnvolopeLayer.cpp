#include "GuildRedEnvolopeLayer.h"
#include "UIManager.h"
#include "ItemManager.h"
#include "MainMenuLayer.h"
#include "ButtonPerformanceManager.h"
#include "Localization.h"
#include "InterfaceManager.h"
#include "SkeletonAnimRcsManager.h"
#include "ParticleManager.h"
#include "TimeManager.h"

#include <time.h>

GuildRedEnvolopeLayer::GuildRedEnvolopeLayer(void) :
	m_leftViewTable(NULL),
	m_leftViewTableLayer(NULL),
	m_btnClose(NULL),
	m_btnNote(NULL),
	m_btnCharge(NULL),
	m_btnChongzhi(NULL),
	m_btnTuhao(NULL),
	m_btnIcon1(NULL),
	m_btnIcon2(NULL),
	m_btnIcon3(NULL),
	m_btnIcon4(NULL),
	m_btnSendRed(NULL),
	m_label1(NULL),
	m_label2(NULL),
	m_label3(NULL),
	m_labelDiaNum(NULL),
	m_labelIcon1(NULL),
	m_labelIcon2(NULL),
	m_labelIcon3(NULL),
	m_labelIcon4(NULL),
	m_rightViewTable(NULL),
	m_rightViewTableLayer(NULL),
	m_leftSelectedIndex(0),
	m_rightSelectedIndex(0),
	m_label4(NULL),
	m_label5(NULL),
	m_layerSendRedEnvo(NULL),
	m_labelTitle(NULL),
	m_labelGetAwardsNote(NULL),
	m_labelGetNote(NULL)
{
}

GuildRedEnvolopeLayer::~GuildRedEnvolopeLayer(void)
{
	setLeftViewTable(NULL);
	setRightViewTable(NULL);
	setLeftViewTableLayer(NULL);
	setRightViewTableLayer(NULL);
	setBtnClose(NULL);
	setBtnNote(NULL);
	setBtnCharge(NULL);
	setBtnChongZhi(NULL);
	setBtnSendRed(NULL);
	setBtnIcon1(NULL);
	setBtnIcon2(NULL);
	setBtnIcon3(NULL);
	setBtnIcon4(NULL);
	setBtnTuhao(NULL);
	setLabel1(NULL);
	setLabel2(NULL);
	setLabel3(NULL);
	setLabel4(NULL);
	setLabel5(NULL);
	setLabelDiaNum(NULL);
	setLabelIcon1(NULL);
	setLabelIcon2(NULL);
	setLabelIcon3(NULL);
	setLabelIcon4(NULL);
	setLabelGetAwardsNote(NULL);
	setLabelGetNote(NULL);
	setLabelTitle(NULL);
	m_layerSendRedEnvo = NULL;

    ButtonPerformanceManager::Get()->removePerformanceButton(this);

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetGuildRedEnvoInfo");
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetRedEnvoConfigRsp");
}

SEL_MenuHandler GuildRedEnvolopeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler GuildRedEnvolopeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", GuildRedEnvolopeLayer::onClose);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "note", GuildRedEnvolopeLayer::onNote);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "chongzhi_selector", GuildRedEnvolopeLayer::onChongzhiBtn);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "tuhao_selector", GuildRedEnvolopeLayer::onTuhaoBtn);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "icon1_selector", GuildRedEnvolopeLayer::onIconClicked);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "icon2_selector", GuildRedEnvolopeLayer::onIconClicked);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "icon3_selector", GuildRedEnvolopeLayer::onIconClicked);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "icon4_selector", GuildRedEnvolopeLayer::onIconClicked);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "charge",			GuildRedEnvolopeLayer::onChargeBtn);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "send_red",GuildRedEnvolopeLayer::onSendRed);

	return NULL;
}

bool GuildRedEnvolopeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tableview_left_layer",   CCLayer*,            m_leftViewTableLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tableview_right_layer",  CCLayer*,            m_rightViewTableLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_close",           CCControlButton*,    m_btnClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_note",			CCControlButton*,	 m_btnNote);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_dia_num",			CCLabelTTF*,	     m_labelDiaNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_tuhao",			CCControlButton*,	 m_btnTuhao);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_chongzhi",		CCControlButton*,	 m_btnChongzhi);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_charge",			CCControlButton*,	 m_btnCharge);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_1",				CCLabelTTF*,	     m_label1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_2",				CCLabelTTF*,	     m_label2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_3",				CCLabelTTF*,	     m_label3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_4",				CCLabelTTF*,	     m_label4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_5",				CCLabelTTF*,	     m_label5);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon1_btn",			    CCControlButton*,	 m_btnIcon1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon2_btn",				CCControlButton*,	 m_btnIcon2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon3_btn",				CCControlButton*,	 m_btnIcon3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon4_btn",				CCControlButton*,	 m_btnIcon4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon1_label",			CCLabelTTF*,	     m_labelIcon1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon2_label",			CCLabelTTF*,	     m_labelIcon2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon3_label",			CCLabelTTF*,	     m_labelIcon3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon4_label",			CCLabelTTF*,	     m_labelIcon4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_send_red",		CCControlButton*,	 m_btnSendRed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_red_envo_title",	CCLabelTTF*,	     m_labelTitle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_red_note",		CCLabelTTF*,	     m_labelGetNote);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_awards_note",	CCLabelTTF*,	     m_labelGetAwardsNote);


	return true;
}

void GuildRedEnvolopeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuildRedEnvolopeLayer::OnRedEnvoInfoMsgCallBack), "GetGuildRedEnvoInfo", NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuildRedEnvolopeLayer::onRedEnvoConfigMsgCallBack), "GetRedEnvoConfigRsp", NULL);

	GuildManager::Get()->SendGetRedEnvoConfigReq();      //请求红包XML，显示在左侧tableview
	GuildManager::Get()->SendGetGuildRedEnvoInfoReq();   //请求显示在右侧tableview的红包信息

    setTouchEnabled(true);
}

void GuildRedEnvolopeLayer::OnRedEnvoInfoMsgCallBack(CCObject* obj)
{
	ConfigUI();
}

void GuildRedEnvolopeLayer::onRedEnvoConfigMsgCallBack(CCObject* obj)
{
	ConfigUI();
}

bool GuildRedEnvolopeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRedEnvolopeLayer::onIconClicked(CCObject* sender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(sender);
    int itemid = node->getTag();
    if(itemid == 0)
    { 
        return;
    }
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(itemid, pt);
}

void GuildRedEnvolopeLayer::onChargeBtn(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
		mLayer->ShowGuildRedEnvolopeLayer(false);
		mLayer->ShowChargeLayer(true);
    }
}

void GuildRedEnvolopeLayer::onSendRed(CCObject* sender, CCControlEvent event)
{
	if (GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom().size() == 0)
		return;

	GuildRedEnvolopeInfo info = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[m_leftSelectedIndex];

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuildRedEnvolopeLayer::onFirstSendRedEnvoMsgCallBack), "FirstSendRedEnvo", NULL);

	GuildManager::Get()->SendFirstSendRedEnvoReq(info.envo_id, info.envo_type);
}

void GuildRedEnvolopeLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void GuildRedEnvolopeLayer::onEnter()
{
    CCLayer::onEnter();

	m_btnClose->setTouchPriority(kCCScrollMenuPriority);
	m_btnNote->setTouchPriority(kCCScrollMenuPriority);
	m_btnTuhao->setTouchPriority(kCCScrollMenuPriority);
	m_btnChongzhi->setTouchPriority(kCCScrollMenuPriority);
	m_btnSendRed->setTouchPriority(kCCScrollMenuPriority);
	m_btnCharge->setTouchPriority(kCCScrollMenuPriority);
	m_btnIcon1->setTouchPriority(kCCScrollMenuPriority);
	m_btnIcon2->setTouchPriority(kCCScrollMenuPriority);
	m_btnIcon3->setTouchPriority(kCCScrollMenuPriority);
	m_btnIcon4->setTouchPriority(kCCScrollMenuPriority);

	m_btnIcon1->getParent()->setVisible(false);
	m_btnIcon2->getParent()->setVisible(false);
	m_btnIcon3->getParent()->setVisible(false);
	m_btnIcon4->getParent()->setVisible(false);

	m_btnTuhao->setTitleForState(CCString::create(Localizatioin::getLocalization("G_REDENVELOPE_SORT1")), CCControlStateNormal);
	m_btnChongzhi->setTitleForState(CCString::create(Localizatioin::getLocalization("G_REDENVELOPE_SORT2")), CCControlStateNormal);
	m_btnSendRed->setTitleForState(CCString::create(Localizatioin::getLocalization("G_REDENVELOPE_BUTTON1")), CCControlStateNormal);

	m_labelGetAwardsNote->setString(Localizatioin::getLocalization("G_REDENVELOPE_GET1"));
	m_labelGetNote->setString(Localizatioin::getLocalization("G_REDENVELOPE_SORT3"));
	m_labelTitle->setString(Localizatioin::getLocalization("G_REDENVELOPE_TITLE"));

    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_btnTuhao,     0);
    ButtonPerformanceManager::Get()->addPerformanceButton(this, m_btnChongzhi,  1);

    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);

	ConfigUI();
}

void GuildRedEnvolopeLayer::onClose(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        //mLayer->ShowGuildShopLayer(false);
		mLayer->ShowGuildRedEnvolopeLayer(false);
        mLayer->ShowGuildInfoLayer(true);
    }
}

CCSize GuildRedEnvolopeLayer::cellSizeForTable(CCTableView *table)
{
	if (m_leftViewTable == NULL || m_leftViewTable == table)
		return CCSize(190, 220);

	if (m_rightViewTable == NULL || m_rightViewTable == table)
		return CCSize(390, 110);


}

CCTableViewCell* GuildRedEnvolopeLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
 	if (m_leftViewTable == table || m_leftViewTable == NULL)
	{
		CCTableViewCell* cell = table->dequeueCell();

		if(cell)
		{
			RefreshCell(cell, idx, true);
		}
		else
		{
			cell = new CCTableViewCell();
			cell->autorelease();
			RefreshCell(cell, idx, true);
		}

		return cell;
	}

	if (m_rightViewTable == table || m_rightViewTable == NULL)
	{
		CCTableViewCell* cell = table->dequeueCell();

		if(cell)
		{
			RefreshCell(cell, idx, false);
		}
		else
		{
			cell = new CCTableViewCell();
			cell->autorelease();
			RefreshCell(cell, idx, false);
		}

		return cell;
	}
}

void GuildRedEnvolopeLayer::RefreshCell(CCTableViewCell* cell, unsigned int index, bool isLeft)
{
	if (isLeft)
	{
		if (! cell->getChildByTag(111))
		{
			CCLayer* layer = CreateSingleGuildRedEnvolopeLayer();
			cell->addChild(layer, 0, 111);
			layer->setPosition(ccp(0, 0));
		}

		int size = getSizeOfTuhaoRedEnvo();
		SingleLeftGuildRedEnvolopeLayer* layer1 = dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(cell->getChildByTag(111));
		if (layer1 != NULL && (index) < size)
		{
			//GuildRedEnvolopeInfo temp = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[0];
			layer1->InitLayer(index, this, &GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[index]);

			if (index == m_leftSelectedIndex)
			{
				dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(cell->getChildByTag(111))->getSpriteHighRedEnvo()->setVisible(true);
				dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(cell->getChildByTag(111))->getSpriteNorRedEnvo()->setVisible(false);
			}
			else
			{
				dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(cell->getChildByTag(111))->getSpriteHighRedEnvo()->setVisible(false);
				dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(cell->getChildByTag(111))->getSpriteNorRedEnvo()->setVisible(true);
			}
		}
	}
	else
	{
		if (! cell->getChildByTag(111))
		{
			CCLayer* layer = CreateRightSingleGuildRedEnvolopeLayer();
			cell->addChild(layer, 0, 111);
			layer->setPosition(ccp(0, 0));
		}

		SingleRightGuildRedEnvolopeLayer* layer1 = dynamic_cast<SingleRightGuildRedEnvolopeLayer*>(cell->getChildByTag(111));
		if (layer1 != NULL && (index) < GuildManager::Get()->getRedEnvolopeInfo().size())
		{
			layer1->InitLayer(index, this, GuildManager::Get()->getRedEnvolopeInfo()[index]);
		}
	}
}

unsigned int GuildRedEnvolopeLayer::numberOfCellsInTableView(CCTableView *table)
{
	if (m_leftViewTable == table || m_leftViewTable == NULL)
	{
		return getSizeOfTuhaoRedEnvo();
	}

	if (m_rightViewTable == table || m_rightViewTable == NULL)
	{
		return GuildManager::Get()->getRedEnvolopeInfo().size();
	}

	return 0;
}

void GuildRedEnvolopeLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if (m_leftViewTable == table)
	{
		SingleLeftGuildRedEnvolopeLayer* layer = dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(cell->getChildByTag(111));

		if (layer == NULL)
			return;

		for (int i = 0; i < getSizeOfTuhaoRedEnvo(); i++)
		{
			CCTableViewCell* temCell = table->cellAtIndex(i);

			if (temCell == NULL)
			{
				continue;
			}
			
			if (cell->getIdx() == m_leftSelectedIndex)
			{
				return;
			}
			else
			{
				dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(temCell->getChildByTag(111))->getSpriteHighRedEnvo()->setVisible(false);
				dynamic_cast<SingleLeftGuildRedEnvolopeLayer*>(temCell->getChildByTag(111))->getSpriteNorRedEnvo()->setVisible(true);
			}
		}

		m_leftSelectedIndex = cell->getIdx();

		layer->getSpriteHighRedEnvo()->setVisible(true);
		layer->getSpriteNorRedEnvo()->setVisible(false);

		GuildRedEnvolopeInfo info = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[cell->getIdx()];

		m_btnSendRed->setVisible(true);

		vector<string> award2Str = split(info.award2, "/");

		if (award2Str.size() != 2)
			return;

		CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(atoi(award2Str[0].c_str()));
		if (iconFrame != NULL)
		{
			m_btnIcon1->getParent()->setVisible(true);
			m_btnIcon1->setTag(atoi(award2Str[0].c_str()));
			m_btnIcon1->getParent()->removeChildByTag(110);
			m_btnIcon1->getParent()->addChild(iconFrame, 1, 110);
			iconFrame->setPosition(m_btnIcon1->getPosition());
			m_labelIcon1->setString(award2Str[1].c_str());
			m_labelIcon1->setZOrder(2);
		}

		vector<string> award3Str = split(info.award3, ";");
		for (int i = 0; i < award3Str.size(); i++)
		{
			vector<string> award = split(award3Str[i].c_str(), "/");

			if (award.size() != 2)
				return;

			unsigned int award_id = atoi(award[0].c_str());
			const char * award_num = award[1].c_str();

			if (i == 0)
			{
				CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(award_id);
				if (iconFrame != NULL)
				{
					m_btnIcon2->getParent()->setVisible(true);
					m_btnIcon2->setTag(award_id);
					m_btnIcon2->getParent()->removeChildByTag(110);
					m_btnIcon2->getParent()->addChild(iconFrame, 1, 110);
					iconFrame->setPosition(m_btnIcon2->getPosition());
					m_labelIcon2->setString(award_num);
					m_labelIcon2->setZOrder(2);
				}
			}
			else if (i == 1)
			{
				CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(award_id);
				if (iconFrame != NULL)
				{
					m_btnIcon3->getParent()->setVisible(true);
					m_btnIcon3->setTag(award_id);
					m_btnIcon3->getParent()->removeChildByTag(110);
					m_btnIcon3->getParent()->addChild(iconFrame, 1, 110);
					iconFrame->setPosition(m_btnIcon3->getPosition());
					m_labelIcon3->setString(award_num);
					m_labelIcon3->setZOrder(2);
				}
			}
			else if (i == 2)
			{
				CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(award_id);
				if (iconFrame != NULL)
				{
					m_btnIcon4->getParent()->setVisible(true);
					m_btnIcon4->setTag(award_id);
					m_btnIcon4->getParent()->removeChildByTag(110);
					m_btnIcon4->getParent()->addChild(iconFrame, 1, 110);
					iconFrame->setPosition(m_btnIcon4->getPosition());
					m_labelIcon4->setString(award_num);
					m_labelIcon4->setZOrder(2);
				}
			}
		}
	}
}

void GuildRedEnvolopeLayer::ConfigUI()
{
	char dia_count_str[50] = {0};
	unsigned int dia_count = UserData::Get()->GetDiamond();

	sprintf(dia_count_str, "%d", dia_count);
	m_labelDiaNum->setString(dia_count_str);

	if (getSizeOfTuhaoRedEnvo() == 0)
		return;

	onTuhaoBtn(NULL, NULL);

	GuildRedEnvolopeInfo info = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[0];
	vector<string> award2Str = split(info.award2, "/");

	if (award2Str.size() != 2)
		return;

	CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(atoi(award2Str[0].c_str()));
	if (iconFrame != NULL)
	{
		m_btnIcon1->getParent()->setVisible(true);
		m_btnIcon1->setTag(atoi(award2Str[0].c_str()));
		m_btnIcon1->getParent()->removeChildByTag(110);
		m_btnIcon1->getParent()->addChild(iconFrame, 1, 110);
		iconFrame->setPosition(m_btnIcon1->getPosition());
		m_labelIcon1->setString(award2Str[1].c_str());
		m_labelIcon1->setZOrder(2);
	}

	vector<string> award3Str = split(info.award3, ";");
	for (int i = 0; i < award3Str.size(); i++)
	{
		vector<string> award = split(award3Str[i].c_str(), "/");

		if (award.size() != 2)
			return;

		unsigned int award_id = atoi(award[0].c_str());
		const char * award_num = award[1].c_str();

		if (i == 0)
		{
			CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(award_id);
			if (iconFrame != NULL)
			{
				m_btnIcon2->getParent()->setVisible(true);
				m_btnIcon2->setTag(award_id);
				m_btnIcon2->getParent()->removeChildByTag(110);
				m_btnIcon2->getParent()->addChild(iconFrame, 1, 110);
				iconFrame->setPosition(m_btnIcon2->getPosition());
				m_labelIcon2->setString(award_num);
				m_labelIcon2->setZOrder(2);
			}
		}
		else if (i == 1)
		{
			CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(award_id);
			if (iconFrame != NULL)
			{
				m_btnIcon3->getParent()->setVisible(true);
				m_btnIcon3->setTag(award_id);
				m_btnIcon3->getParent()->removeChildByTag(110);
				m_btnIcon3->getParent()->addChild(iconFrame, 1, 110);
				iconFrame->setPosition(m_btnIcon3->getPosition());
				m_labelIcon3->setString(award_num);
				m_labelIcon3->setZOrder(2);
			}
		}
		else if (i == 2)
		{
			CCSprite* iconFrame = ItemManager::Get()->getIconSpriteById(award_id);
			if (iconFrame != NULL)
			{
				m_btnIcon4->getParent()->setVisible(true);
				m_btnIcon4->setTag(award_id);
				m_btnIcon4->getParent()->removeChildByTag(110);
				m_btnIcon4->getParent()->addChild(iconFrame, 1, 110);
				iconFrame->setPosition(m_btnIcon4->getPosition());
				m_labelIcon4->setString(award_num);
				m_labelIcon4->setZOrder(2);
			}
		}
	}


    bool needSetOffset = false;
    CCPoint offset = CCPointZero;
    if(this->m_leftViewTable)
    {
        offset.x = m_leftViewTable->getContentOffset().x;
        offset.y = m_leftViewTable->getContentOffset().y;
        needSetOffset = true; 
        this->m_leftViewTable->removeFromParentAndCleanup(true);
        setLeftViewTable(NULL);
    }

    // 添加商品列表
    setLeftViewTable(CCTableView::create(this, m_leftViewTableLayer->getContentSize()));
    m_leftViewTable->setDirection(kCCScrollViewDirectionHorizontal);

    m_leftViewTableLayer->addChild(m_leftViewTable);

    m_leftViewTable->setTouchPriority(kCCScrollMenuPriority);
    m_leftViewTable->setDelegate(this);
    m_leftViewTable->reloadData();

	if(this->m_rightViewTable)
    {
        offset.x = m_rightViewTable->getContentOffset().x;
        offset.y = m_rightViewTable->getContentOffset().y;
        needSetOffset = true; 
        this->m_rightViewTable->removeFromParentAndCleanup(true);
        setRightViewTable(NULL);
    }

    // 添加商品列表
    setRightViewTable(CCTableView::create(this, m_rightViewTableLayer->getContentSize()));
	m_rightViewTable->setDirection(kCCScrollViewDirectionVertical);
	m_rightViewTable->setVerticalFillOrder(kCCTableViewFillTopDown);

    m_rightViewTableLayer->addChild(m_rightViewTable);

    m_rightViewTable->setTouchPriority(kCCScrollMenuPriority);
    m_rightViewTable->setDelegate(this);
    m_rightViewTable->reloadData();
}

//字符串分割函数
std::vector<std::string> GuildRedEnvolopeLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}


void GuildRedEnvolopeLayer::onNote(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
		mLayer->ShowGuildRedEnvolopeNoteLayer(true);
    }
}

void GuildRedEnvolopeLayer::onTuhaoBtn(CCObject* sender, CCControlEvent event)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 0);

	if (getSizeOfTuhaoRedEnvo() == 0)
		return;

	m_label1->setVisible(true);
	m_label2->setVisible(true);
	m_label3->setVisible(true);
	m_label4->setVisible(false);
	m_label5->setVisible(false);

	GuildRedEnvolopeInfo info = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[0];

	vector<string> date = split(info.activity_date, "-");
	vector<string> beginDate = split(date[0], "/");
	vector<string> endDate = split(date[1], "/");

	if (date.size() == 0 || beginDate.size() == 0 || endDate.size() == 0)
		return;

	char activity_date[100] = {0};
	sprintf(activity_date, Localizatioin::getLocalization("G_REDENVELOPE_TIME"), beginDate[0].c_str(), beginDate[1].c_str(), beginDate[2].c_str(), endDate[0].c_str(), endDate[1].c_str(), endDate[2].c_str());
	m_label1->setString(activity_date);

	vector<string> labelContent = split(info.labelContent, "$");

	if (labelContent.size() == 2)
	{
		m_label2->setString(labelContent[1].c_str());
	}
	else if(labelContent.size() == 4)
	{
		m_label2->setString(labelContent[1].c_str());
		m_label3->setString(labelContent[2].c_str());
	}
}

void GuildRedEnvolopeLayer::onChongzhiBtn(CCObject* sender, CCControlEvent event)
{
    ButtonPerformanceManager::Get()->PerformanceIndexButton(this, 1);

	m_label1->setVisible(false);
	m_label2->setVisible(false);
	m_label3->setVisible(false);
	m_label4->setVisible(true);
	m_label5->setVisible(true);

	vector<GuildRedEnvolopeInfo> info = GuildManager::Get()->getRedEnvolopeInfoFromXML();
	string extend;
	vector<string> strs;

	for (vector<GuildRedEnvolopeInfo>::iterator iter = info.begin();
		iter != info.end(); ++iter)
	{
		if (iter->envo_type == 2)
		{
			extend = iter->labelContent;
			break;
		}
	}

	strs = split(extend, "$");

	if (strs.size() == 2)
	{
		m_label4->setString(strs[1].c_str());
	}
	else if (strs.size() == 4)
	{
		m_label4->setString(strs[1].c_str());
		m_label5->setString(strs[2].c_str());
	}
}

CCLayer* GuildRedEnvolopeLayer::CreateSingleGuildRedEnvolopeLayer()
{
    CCNodeLoader* loader = new SingleGuildRedEnvoLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_red_envo_info", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_guild_red_envo");

    return dynamic_cast<CCLayer*>(node);
}

CCLayer* GuildRedEnvolopeLayer::CreateRightSingleGuildRedEnvolopeLayer()
{
    CCNodeLoader* loader = new SingleRightGuildRedEnvoLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_red_envo_info2", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_guild_red_envo2");

    return dynamic_cast<CCLayer*>(node);
}

void GuildRedEnvolopeLayer::onFirstSendRedEnvoMsgCallBack(CCObject* obj)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "FirstSendRedEnvo");

	FirstSendRedEnvoRspStruct info = GuildManager::Get()->getFirstSendRedEnvoRsp();

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer && info.suc)
	{
		mLayer->ShowGuildSendRedEnvoLayer(true);
	}  
}

unsigned int GuildRedEnvolopeLayer::getSizeOfTuhaoRedEnvo()
{
	return GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom().size();
}

SingleLeftGuildRedEnvolopeLayer::SingleLeftGuildRedEnvolopeLayer() :
	m_iconFrame(NULL),
	m_labelRedName(NULL),
	m_labelRestTime(NULL),
	m_spriteHighRedEnvo(NULL),
	m_spriteNorRedEnvo(NULL),
	m_showHighRedEnvo(false),
	m_btnIcon(NULL),
	m_labelAward1Count(NULL)
{
}

SingleLeftGuildRedEnvolopeLayer::~SingleLeftGuildRedEnvolopeLayer()
{
	setIconFrame(NULL);
	setLabelRedName(NULL);
	setLabelRestTime(NULL);
	setSpriteHighRedEnvo(NULL);
	setSpriteNorRedEnvo(NULL);
	setBtnIcon(NULL);
	setLabelAward1Count(NULL);
}

bool SingleLeftGuildRedEnvolopeLayer::getIsShowHighRedEnvo()
{
	return m_showHighRedEnvo;
}

void SingleLeftGuildRedEnvolopeLayer::setIsShowHighRedEnvo(bool flag)
{
	m_showHighRedEnvo = flag;
}

SEL_MenuHandler SingleLeftGuildRedEnvolopeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler SingleLeftGuildRedEnvolopeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "icon", SingleLeftGuildRedEnvolopeLayer::onIconClicked);

	return NULL;
}

bool SingleLeftGuildRedEnvolopeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon_frame",                      CCSprite*,         m_iconFrame);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_red_name",                  CCLabelTTF*,       m_labelRedName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_rest_time",                 CCLabelTTF*,       m_labelRestTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_high_red_envo_sprite",     CCSprite*,         m_spriteHighRedEnvo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "single_nor_red_envo_sprite",      CCSprite*,         m_spriteNorRedEnvo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "icon_pos_btn",					 CCControlButton*,	m_btnIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_award1_count",              CCLabelTTF*,       m_labelAward1Count);

	return false;
}

void SingleLeftGuildRedEnvolopeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
	m_spriteHighRedEnvo->setVisible(m_showHighRedEnvo);
	m_spriteNorRedEnvo->setVisible(!m_showHighRedEnvo); 
}

//字符串分割函数
std::vector<std::string> SingleLeftGuildRedEnvolopeLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

void SingleLeftGuildRedEnvolopeLayer::InitLayer(unsigned int index, GuildRedEnvolopeLayer* layer, GuildRedEnvolopeInfo* info)
{
	vector<string> aqStr = split(info->activity_qualification.c_str(), "/");
	if (aqStr.size() != 3)   
		return;

	char restTime[50] = {0};
	char restTimeTemp[50] = {0};
	char redName[50] = {0};

	sprintf(restTimeTemp, "%d", info->iRestTimes);
	sprintf(restTime, Localizatioin::getLocalization("G_REDENVELOPE_NUMBER"), restTimeTemp);
	sprintf(redName, Localizatioin::getLocalization("G_REDENVELOPE_DIAMOND"), aqStr[2].c_str());

	m_labelRestTime->setString(restTime);
	m_labelRedName->setString(redName);

	vector<string> award1Str = split(info->award1.c_str(), "/");
	m_btnIcon->setTag(atoi(award1Str[0].c_str()));
	m_labelAward1Count->setString(award1Str[1].c_str());
	CCSprite* award1Icon = ItemManager::Get()->getIconSpriteById(atoi(award1Str[0].c_str()));
	m_iconFrame->removeAllChildren();
	m_iconFrame->addChild(award1Icon);
	award1Icon->setAnchorPoint(ccp(0.5, 0.5));
	award1Icon->setPosition(ccp(m_iconFrame->getContentSize().width / 2, m_iconFrame->getContentSize().height / 2));
}

void SingleLeftGuildRedEnvolopeLayer::onIconClicked(CCObject* sender, CCControlEvent event)
{
    CCNode* node = dynamic_cast<CCNode*>(sender);
    int itemid = node->getTag();
    if(itemid == 0)
    {
        return;
    }
    CCPoint pt = node->convertToWorldSpace(CCPointZero);
    ItemManager::Get()->showItemTipsById(itemid, pt);
}

void SingleLeftGuildRedEnvolopeLayer::onEnter()
{
	CCLayer::onEnter();

	m_btnIcon->setTouchPriority(kCCLayerTouchPriority);
}

GuildRedEnvolopeNoteLayer::GuildRedEnvolopeNoteLayer() :
	m_btnClose(NULL),
	m_btnNoteOk(NULL),
	m_layerContent(NULL),
	m_labelNoteContent1(NULL),
	m_labelNoteContent2(NULL),
	m_labelNoteContent3(NULL),
	m_labelNoteContent4(NULL),
	m_labelNoteContent5(NULL),
	m_labelNoteContent6(NULL),
	m_labelTitle(NULL),
	m_layerBg(NULL)
{
}

GuildRedEnvolopeNoteLayer::~GuildRedEnvolopeNoteLayer()
{
	setBtnClose(NULL);
	setBtnNoteOK(NULL);
	setLayerContent(NULL);
	setLabelNoteContent1(NULL);
	setLabelNoteContent2(NULL);
	setLabelNoteContent3(NULL);
	setLabelNoteContent4(NULL);
	setLabelNoteContent5(NULL);
	setLabelNoteContent6(NULL);
	setLabelTitle(NULL);
	setLayerBg(NULL);
}

SEL_MenuHandler GuildRedEnvolopeNoteLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler GuildRedEnvolopeNoteLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close_note",   GuildRedEnvolopeNoteLayer::onClose);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "note_selector_ok", GuildRedEnvolopeNoteLayer::onBtnOk);

	return NULL;
}

bool GuildRedEnvolopeNoteLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_layer",              CCLayer*,               m_layerContent);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",						 CCControlButton*,       m_btnClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_btn_ok",                     CCControlButton*,       m_btnNoteOk);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_label1",             CCLabelTTF*,			 m_labelNoteContent1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_label2",             CCLabelTTF*,			 m_labelNoteContent2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_label3",             CCLabelTTF*,			 m_labelNoteContent3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_label4",             CCLabelTTF*,			 m_labelNoteContent4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_label5",             CCLabelTTF*,			 m_labelNoteContent5);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_content_label6",             CCLabelTTF*,			 m_labelNoteContent6);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_note_title",				 CCLabelTTF*,			 m_labelTitle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "note_black_bg",					 CCLayer*,				 m_layerBg);

	return false;
}

void GuildRedEnvolopeNoteLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

	float scale = UIManager::sharedManager()->getScaleFactor();
	setScale(scale);

	m_layerBg->setContentSize(s);
	m_layerBg->setScale(m_layerBg->getScale() / scale);
}

bool GuildRedEnvolopeNoteLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRedEnvolopeNoteLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildRedEnvolopeNoteLayer::onEnter()
{
	CCLayer::onEnter();

	m_btnClose->setTouchPriority(kCCLayerMenuTouchPriority);
	m_btnNoteOk->setTouchPriority(kCCLayerMenuTouchPriority);

	m_labelNoteContent1->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TEXT1"));
	m_labelNoteContent2->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TEXT2"));
	m_labelNoteContent3->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TEXT3"));
	m_labelNoteContent4->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TEXT4"));
	m_labelNoteContent5->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TEXT5"));
	m_labelNoteContent6->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TEXT6"));

	m_labelTitle->setString(Localizatioin::getLocalization("G_RINTRODUCTION_TITLE"));

	m_btnNoteOk->setTitleForState(CCString::create(Localizatioin::getLocalization("M_CONFIRM")), CCControlStateNormal);
}

void GuildRedEnvolopeNoteLayer::onClose(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        //mLayer->ShowGuildShopLayer(false);
		mLayer->ShowGuildRedEnvolopeNoteLayer(false);
    }
}

void GuildRedEnvolopeNoteLayer::onBtnOk(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        //mLayer->ShowGuildShopLayer(false);
		mLayer->ShowGuildRedEnvolopeNoteLayer(false);
    }
}

GuildRedEnvolopeSendLayer::GuildRedEnvolopeSendLayer() :
	m_layerEffect(NULL),
	m_spriteAward1(NULL),
	m_spriteAward2(NULL),
	m_spriteAward3(NULL),
	m_labelRedEnvoName(NULL),
	m_labelSendLeft(NULL),
	m_btnCancel(NULL),
	m_btnOk(NULL),
	m_labelAward1Count(NULL),
	m_labelAward2Count(NULL),
	m_labelAward3Count(NULL),
	m_effectSend(NULL),
	m_pAnimEndListerner(NULL),
	m_pfnAnimEndSelector(NULL),
	m_layerBg(NULL),
	m_bIsPlayingAnim(false)
{
}

GuildRedEnvolopeSendLayer::~GuildRedEnvolopeSendLayer()
{
	setLayerEffect(NULL);
	setSpriteAward1(NULL);
	setSpriteAward2(NULL);
	setSpriteAward3(NULL);
	setLabelRedEnvoName(NULL);
	setLabelSendLeft(NULL);
	setBtnCancel(NULL);
	setBtnOK(NULL);
	setLabelAward1Count(NULL);
	setLabelAward2Count(NULL);
	setLabelAward3Count(NULL);
	setLayerBg(NULL);

	if(m_effectSend)
	{
		EnginePlayerManager::getInstance()->removeEffectAsprite(m_effectSend);
		m_effectSend = NULL;
	}
}

SEL_MenuHandler GuildRedEnvolopeSendLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler GuildRedEnvolopeSendLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "cancel_send_red",		  GuildRedEnvolopeSendLayer::onCancel);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "send_red",				  GuildRedEnvolopeSendLayer::onBtnOk);

	return NULL;
}

bool GuildRedEnvolopeSendLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "effect_layer",					 CCLayer*,               m_layerEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cancel_btn",						 CCControlButton*,       m_btnCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ok_btn",							 CCControlButton*,       m_btnOk);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_red_envo_name",		 CCLabelTTF*,			 m_labelRedEnvoName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_awards_note",           CCLabelTTF*,			 m_labelSendLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "award1",							 CCSprite*,			     m_spriteAward1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "award2",							 CCSprite*,				 m_spriteAward2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "award3",							 CCSprite*,				 m_spriteAward3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_award1_count",				 CCLabelTTF*,			 m_labelAward1Count);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_award2_count",				 CCLabelTTF*,			 m_labelAward2Count);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_award3_count",				 CCLabelTTF*,			 m_labelAward3Count);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "send_balck_bg",					 CCLayer*,				 m_layerBg);

	return false;
}

void GuildRedEnvolopeSendLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

	float scale = UIManager::sharedManager()->getScaleFactor();
	setScale(scale);

	m_layerBg->setContentSize(s);
	m_layerBg->setScale(m_layerBg->getScale() / scale);
}

bool GuildRedEnvolopeSendLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRedEnvolopeSendLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildRedEnvolopeSendLayer::onEnter()
{
	CCLayer::onEnter();

	m_spriteAward1->setVisible(false);
	m_spriteAward2->setVisible(false);
	m_spriteAward3->setVisible(false);

	m_btnCancel->setTouchPriority(kCCLayerMenuTouchPriority);
	m_btnOk->setTouchPriority(kCCLayerMenuTouchPriority);

	m_btnOk->setTitleForState(CCString::create(Localizatioin::getLocalization("M_CONFIRM")), CCControlStateNormal);
	m_btnCancel->setTitleForState(CCString::create(Localizatioin::getLocalization("M_CANCEL")), CCControlStateNormal);

	char strAllSendRedEnvoCount[10] = {0};
	vector<string> award2;
	vector<string> award1;

	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	GuildRedEnvolopeInfo info = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[mLayer->getSelectedCellOfLeft()];

	string activity_qualification = info.activity_qualification;

	if (activity_qualification != "")
	{
		string redEnvoName = split(activity_qualification, "/")[2];

		char note[100] = {0};

		sprintf(note, Localizatioin::getLocalization("G_RSEND1"), redEnvoName.c_str());
		m_labelRedEnvoName->setString(note);
	}

	m_labelSendLeft->setString(Localizatioin::getLocalization("G_RWILLBE"));
	
	award1 = split(info.award1, ";");
	for (int i = 0; i < award1.size(); i++)
	{
		vector<string> temp;

		temp = split(award1[i], "/");
		
		if (i == 0)
		{
			m_spriteAward1->setVisible(true);
			CCSprite* award1Icon = ItemManager::Get()->getIconSpriteById(atoi(temp[0].c_str()));
			m_spriteAward1->addChild(award1Icon, 1, 110);
			award1Icon->setAnchorPoint(ccp(0.5, 0.5));
			award1Icon->setPosition(ccp(m_spriteAward1->getContentSize().width / 2, m_spriteAward1->getContentSize().height / 2));
			m_labelAward1Count->setString(temp[1].c_str());
			m_labelAward1Count->setZOrder(2);
		}
		else if (i == 1)
		{
			m_spriteAward2->setVisible(true);
			CCSprite* award2Icon = ItemManager::Get()->getIconSpriteById(atoi(temp[0].c_str()));
			m_spriteAward2->addChild(award2Icon, 1, 110);
			award2Icon->setAnchorPoint(ccp(0.5, 0.5));
			award2Icon->setPosition(ccp(m_spriteAward2->getContentSize().width / 2, m_spriteAward2->getContentSize().height / 2));
			m_labelAward2Count->setString(temp[1].c_str());
			m_labelAward2Count->setZOrder(2);
		}
		else if (i == 2)
		{
			m_spriteAward3->setVisible(true);
			CCSprite* award3Icon = ItemManager::Get()->getIconSpriteById(atoi(temp[0].c_str()));
			m_spriteAward3->addChild(award3Icon, 1, 110);
			award3Icon->setAnchorPoint(ccp(0.5, 0.5));
			award3Icon->setPosition(ccp(m_spriteAward3->getContentSize().width / 2, m_spriteAward3->getContentSize().height / 2));
			m_labelAward3Count->setString(temp[1].c_str());
			m_labelAward3Count->setZOrder(2);
		}
	}

    SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(453);
	m_effectSend = ParticleManager::Get()->createEffectSprite(453,"",false);
    if (m_effectSend)
    {
		SetAnimEndCallBack(this, callfunc_selector(GuildRedEnvolopeSendLayer::onFirtstAnimEnd));
		//m_effectSend->getRoot()->setScale(UIManager::sharedManager()->getScaleFactor());
		m_layerEffect->addChild(m_effectSend, 0, 300);
        CCSize s = m_layerEffect->getContentSize();
        m_effectSend->setPosition(ccp(s.width / 2, s.height / 2));
        m_effectSend->SetAnim(kType_Play, 1, true, this);
		m_effectSend->SetAnimPauseOnOver(true);
    }
}

void GuildRedEnvolopeSendLayer::onFirtstAnimEnd()
{
	m_effectSend->SetAnim(kType_Play1, 1000000, true);
}

void GuildRedEnvolopeSendLayer::onSecondAnimEnd()
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowGuildSendRedEnvoLayer(false);
	}  
}

void GuildRedEnvolopeSendLayer::SetAnimEndCallBack(CCObject* pListerner,SEL_CallFunc callBack)
{
	m_pAnimEndListerner = pListerner;
	m_pfnAnimEndSelector = callBack;
}

void GuildRedEnvolopeSendLayer::animationHandler(const char* _aniType, const char* _aniID, const char* _frameID)
{
	if (0 == strcmp(_aniType,"loopComplete") ||
		0 == strcmp(_aniType,"complete"))
	{
		if (m_pAnimEndListerner && m_pfnAnimEndSelector)
		{
			(m_pAnimEndListerner->*m_pfnAnimEndSelector)();
		}
	}
}

void GuildRedEnvolopeSendLayer::onCancel(CCObject* sender, CCControlEvent event)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowGuildSendRedEnvoLayer(false);
	}  
}

void GuildRedEnvolopeSendLayer::onBtnOk(CCObject* sender, CCControlEvent event)
{
	if (!m_bIsPlayingAnim)
	{
		m_bIsPlayingAnim = true;

		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		GuildRedEnvolopeInfo info = GuildManager::Get()->getTuHaoRedEnvolopeInfoFrom()[mLayer->getSelectedCellOfLeft()];

		GuildManager::Get()->SendSecondSendRedEnvoReq(info.envo_id, info.envo_type);	

		m_effectSend->PauseAllArmatureAnim();

		SetAnimEndCallBack(this, callfunc_selector(GuildRedEnvolopeSendLayer::onSecondAnimEnd));
		m_effectSend->SetAnim(kType_Play2, 1, true);
	}
}

//字符串分割函数
std::vector<std::string> GuildRedEnvolopeSendLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }

    return result;
}

SingleRightGuildRedEnvolopeLayer::SingleRightGuildRedEnvolopeLayer() :
	m_labelRestTime(NULL),
	m_labelCount(NULL),
	m_labelPlayerName(NULL),
	m_labelRestRed(NULL),
	m_btnGet(NULL),
	m_btnCheckout(NULL),
	m_layerGetRedEnvo(NULL),
	m_labelRest(NULL)
{
}

SingleRightGuildRedEnvolopeLayer::~SingleRightGuildRedEnvolopeLayer()
{
	setLabelCount(NULL);
	setLabelPlayerName(NULL);
	setLabelRestRed(NULL);
	setLabelRestTime(NULL);
	setBtnGet(NULL);
	setBtnCheckout(NULL);
	setLayerGetRedEnvo(NULL);
	setLabelRest(NULL);

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetRedEnvoRsp");
	unschedule(schedule_selector(SingleRightGuildRedEnvolopeLayer::updateRestTime));
}

SEL_MenuHandler SingleRightGuildRedEnvolopeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler SingleRightGuildRedEnvolopeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "get",		SingleRightGuildRedEnvolopeLayer::onGet);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "checkout",   SingleRightGuildRedEnvolopeLayer::onCheckout);

	return NULL;
}

bool SingleRightGuildRedEnvolopeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_right_tableview",					 CCControlButton*,       m_btnGet);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "checkout_btn",							 CCControlButton*,       m_btnCheckout);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_player_name",						 CCLabelTTF*,			 m_labelPlayerName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_count",							 CCLabelTTF*,			 m_labelCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_rest_red",							 CCLabelTTF*,			 m_labelRestRed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_rest_time",						 CCLabelTTF*,			 m_labelRestTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_right_rest",						 CCLabelTTF*,			 m_labelRest);

	return true;
}

void SingleRightGuildRedEnvolopeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SingleRightGuildRedEnvolopeLayer::onGetRedEnvoMsgCallBack), "GetRedEnvoRsp", NULL);
}

std::vector<std::string> SingleRightGuildRedEnvolopeLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }

    return result;
}

void SingleRightGuildRedEnvolopeLayer::InitLayer(unsigned int index, GuildRedEnvolopeLayer* layer, GuildGetRedEnvolopeInfo info)
{
	char strRestRed[10] = {0};
	char strName[50] = {0};

	sprintf(strRestRed, "%d", info.rest_times);
	m_labelRestRed->setString(strRestRed); 

	sprintf(strName, "%s", info.sender_name.c_str());
	m_labelPlayerName->setString(strName);

	m_btnGet->setTag(info.envo_id);

	char readDiaStr[10] = {0};
	char label[100] = {0};
	sprintf(readDiaStr, "%d", info.real_dia);
	sprintf(label, Localizatioin::getLocalization("G_REDENVELOPE_DIAMOND"), readDiaStr);
	m_labelCount->setString(label);
	 
	if (info.rest_times > 0 && info.isGeted == false)
	{
		m_btnGet->setVisible(true);
		m_btnCheckout->setVisible(false);
	}
	else
	{
		m_btnCheckout->setVisible(true);
		m_btnGet->setVisible(false);
	}

	m_sendTime = info.key.send_tm;

	m_singleInfo.envo_id = info.envo_id;
	m_singleInfo.send_time = info.key.send_tm;
	m_singleInfo.user_id = info.key.user_id;

	setRestTimeLabel();
	schedule(schedule_selector(SingleRightGuildRedEnvolopeLayer::updateRestTime), 1.0f);
}

void SingleRightGuildRedEnvolopeLayer::updateRestTime(float dt)
{
	setRestTimeLabel();
}

void SingleRightGuildRedEnvolopeLayer::setRestTimeLabel()
{
    long now; 
	struct tm* time1;
	time_t long_time;
	char timebuffer[30] = {0};
	char timebuffer1[100] = {0};

	now = TimeManager::Get()->getCurServerTime();

	long_time = m_sendTime + 3600 * 24 - now;
	//time1 = localtime(&long_time);

	long hour = long_time / 3600;
	long min = (long_time - hour * 3600) / 60;
	long sec = long_time - hour * 3600 - min * 60;

	sprintf(timebuffer, "%02d : %02d : %02d", hour, min, sec);
	sprintf(timebuffer1, Localizatioin::getLocalization("G_REDENVELOPE_GET2"), timebuffer);

	m_labelRestTime->setString(timebuffer1);
	m_labelRest->setString(Localizatioin::getLocalization("G_REDENVELOPE_LAST"));
}

void SingleRightGuildRedEnvolopeLayer::onEnter()
{
	CCLayer::onEnter();

	m_btnGet->setTouchPriority(kCCScrollMenuPriority);
	m_btnCheckout->setTouchPriority(kCCScrollMenuPriority);

	m_btnGet->setTitleForState(CCString::create(Localizatioin::getLocalization("M_UNDERGROUND_GETREWARD")), CCControlStateNormal);
	m_btnCheckout->setTitleForState(CCString::create(Localizatioin::getLocalization("M_FIND")), CCControlStateNormal);
}

void SingleRightGuildRedEnvolopeLayer::onGet(CCObject* sender, CCControlEvent event)
{
	int tag = m_btnGet->getTag();
	CSGuildRedPacketKey key;
	vector<GuildGetRedEnvolopeInfo> info = GuildManager::Get()->getRedEnvolopeInfo();
	vector<GuildGetRedEnvolopeInfo>::iterator iter;

	for (iter = info.begin();
		iter != info.end(); ++iter)
	{
		if (iter->envo_id == tag)
		{
			key.set_channelid(iter->key.channel_id);
			key.set_reg_tm(iter->key.reg_tm);
			key.set_send_tm(iter->key.send_tm);
			key.set_userid(iter->key.user_id);
			key.set_zone_id(iter->key.zone_id);

			break;
		}
	}

	if (iter == info.end())
		return;

	GuildManager::Get()->SendGetRedEnvoReq(key);
}

void SingleRightGuildRedEnvolopeLayer::onCheckout(CCObject* sender, CCControlEvent event)
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SingleRightGuildRedEnvolopeLayer::onGetRedEnvoRecordMsgCallBack), "GetRedEnvoRecordRsp", NULL);

	CSGuildRedPacketKey key;
	vector<GuildGetRedEnvolopeInfo> info = GuildManager::Get()->getRedEnvolopeInfo();
	vector<GuildGetRedEnvolopeInfo>::iterator iter;

	for (iter = info.begin();
		iter != info.end(); ++iter)
	{
		if (iter->envo_id == m_singleInfo.envo_id && iter->key.send_tm == m_singleInfo.send_time && iter->key.user_id == m_singleInfo.user_id)
		{
			key.set_channelid(iter->key.channel_id);
			key.set_reg_tm(iter->key.reg_tm);
			key.set_send_tm(iter->key.send_tm);
			key.set_userid(iter->key.user_id);
			key.set_zone_id(iter->key.zone_id);

			break;
		}
	}

	if (iter == info.end())
		return;

	GuildManager::Get()->SendGetRedEnvoRecordReq(key);
}

void SingleRightGuildRedEnvolopeLayer::onGetRedEnvoMsgCallBack(CCObject* obj)
{
	GetRedEnvoRspStruct rsp = GuildManager::Get()->getGetRedEnvoRsp();

	if (rsp.suc)
	{
		MainMenuLayer* layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();

		GuildManager::Get()->setGetRedEnvoRspTag(m_btnGet->getTag());

		if (layer)
		{
			layer->ShowGuildGetRedEnvoLayer(true);
		}
	}
}

void SingleRightGuildRedEnvolopeLayer::onGetRedEnvoRecordMsgCallBack(CCObject* obj)
{
	MainMenuLayer* layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	vector<RedEnvoRecordInfo> info = GuildManager::Get()->getRedEnvoRecordInfo();
	
	GuildManager::Get()->setRedEnvoRecordTag(m_btnGet->getTag());
	GuildManager::Get()->setCurrentRedEnvoSenderName(m_labelPlayerName->getString());

	if (layer)
	{
		layer->ShowGuildRedEnvoRecordLayer(true);
	}

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetRedEnvoRecordRsp");
}

GuildGetRedEnvolopeLayer::GuildGetRedEnvolopeLayer() :
	m_labelGetSuc(NULL),
	m_layerEffect(NULL),
	m_spriteAward1(NULL),
	m_spriteAward2(NULL),
	m_spriteAward3(NULL),
	m_btnOk(NULL),
	m_labelGetLeft(NULL),
	m_labelGetAward1(NULL),
	m_labelGetAward2(NULL),
	m_labelGetAward3(NULL),
	m_layerBg(NULL),
	m_bIsPlayingAnim(false)
{
}

GuildGetRedEnvolopeLayer::~GuildGetRedEnvolopeLayer()
{
	setLabelGetSuc(NULL);
	setLayerEffect(NULL);
	setSpriteAward1(NULL);
	setSpriteAward2(NULL);
	setSpriteAward3(NULL);
	setBtnOK(NULL);
	setLabelGetLeft(NULL);
	setLabelGetAward1(NULL);
	setLabelGetAward2(NULL);
	setLabelGetAward3(NULL);
	setLayerBg(NULL);
}

SEL_MenuHandler GuildGetRedEnvolopeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler GuildGetRedEnvolopeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "ok",		GuildGetRedEnvolopeLayer::onBtnOk);

	return NULL;
}

bool GuildGetRedEnvolopeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ok_btn",						 CCControlButton*,       m_btnOk);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_award1",					 CCSprite*,				 m_spriteAward1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_award2",					 CCSprite*,				 m_spriteAward2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_award3",					 CCSprite*,				 m_spriteAward3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_suc",			     CCLabelTTF*,			 m_labelGetSuc);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_red_envo_effect_layer",	 CCLayer*,				 m_layerEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_left",			     CCLabelTTF*,			 m_labelGetLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_award1",			 CCLabelTTF*,			 m_labelGetAward1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_award2",			 CCLabelTTF*,			 m_labelGetAward2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_get_award3",			 CCLabelTTF*,			 m_labelGetAward3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "get_black_bg",				 CCLayerColor*,			 m_layerBg);

	return true;
}

void GuildGetRedEnvolopeLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

	float scale = UIManager::sharedManager()->getScaleFactor();
	setScale(scale);

	m_layerBg->setContentSize(s);
	m_layerBg->setScale(m_layerBg->getScale() / scale);
}

bool GuildGetRedEnvolopeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildGetRedEnvolopeLayer::onFirtstAnimEnd()
{
	m_effectGet->SetAnim(kType_Play1, 1000000, true);
}

void GuildGetRedEnvolopeLayer::onSecondAnimEnd()
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		mLayer->ShowGuildGetRedEnvoLayer(false);
	}  
}

void GuildGetRedEnvolopeLayer::SetAnimEndCallBack(CCObject* pListerner,SEL_CallFunc callBack)
{
	m_pAnimEndListerner = pListerner;
	m_pfnAnimEndSelector = callBack;
}

void GuildGetRedEnvolopeLayer::animationHandler(const char* _aniType, const char* _aniID, const char* _frameID)
{
	if (0 == strcmp(_aniType,"loopComplete") ||
		0 == strcmp(_aniType,"complete"))
	{
		if (m_pAnimEndListerner && m_pfnAnimEndSelector)
		{
			(m_pAnimEndListerner->*m_pfnAnimEndSelector)();
		}
	}
}

void GuildGetRedEnvolopeLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildGetRedEnvolopeLayer::onEnter()
{
	CCLayer::onEnter();

	m_spriteAward1->setVisible(false);
	m_spriteAward2->setVisible(false);
	m_spriteAward3->setVisible(false);

	m_btnOk->setTouchPriority(kCCLayerMenuTouchPriority);

	m_btnOk->setTitleForState(CCString::create(Localizatioin::getLocalization("M_CONFIRM")), CCControlStateNormal);

	m_labelGetLeft->setString(Localizatioin::getLocalization("G_RGET2"));

	GetRedEnvoRspStruct rsp = GuildManager::Get()->getGetRedEnvoRsp();

	vector<GuildGetRedEnvolopeInfo> info = GuildManager::Get()->getRedEnvolopeInfo();
	string sender_name;
	vector<GuildGetRedEnvolopeInfo>::iterator iter;

	for ( iter = info.begin(); iter != info.end(); ++iter)
	{
		if (iter->envo_id == rsp.envo_id)
		{
			sender_name = iter->sender_name;
			break;
		}
	}

	if (iter == info.end())
		sender_name = "";

	char getSucStr[100] = {0};

	sprintf(getSucStr, Localizatioin::getLocalization("G_RGET1"), sender_name.c_str());

	m_labelGetSuc->setString(getSucStr);

	vector<RedEnvoAwardStruct> awards = rsp.awards;

	m_spriteAward1->setVisible(true);
	CCSprite* icon = ItemManager::Get()->getIconSpriteById(416002);
	m_spriteAward1->addChild(icon, 1, 110);
	icon->setAnchorPoint(ccp(0.5, 0.5));
	icon->setPosition(ccp(m_spriteAward1->getContentSize().width / 2, m_spriteAward1->getContentSize().height / 2));
	
	char awardCount[20] = {0};
	sprintf(awardCount, "%d", rsp.dia_count);
	m_labelGetAward1->setString(awardCount);
	m_labelGetAward1->setZOrder(2);

	for (int i = 0; i < awards.size(); i++)
	{
		if (i == 0)
		{
			m_spriteAward2->setVisible(true);
			CCSprite* icon = ItemManager::Get()->getIconSpriteById(awards[i].item_id);
			m_spriteAward2->addChild(icon, 1, 110);
			icon->setAnchorPoint(ccp(0.5, 0.5));
			icon->setPosition(ccp(m_spriteAward2->getContentSize().width / 2, m_spriteAward2->getContentSize().height / 2));
	
			char awardCount[20] = {0};
			sprintf(awardCount, "%d", awards[i].item_count);
			m_labelGetAward2->setString(awardCount);
			m_labelGetAward2->setZOrder(2);
		}
		else if (i == 1)
		{
			m_spriteAward3->setVisible(true);
			CCSprite* icon = ItemManager::Get()->getIconSpriteById(awards[i].item_id);
			m_spriteAward3->addChild(icon, 1, 110);
			icon->setAnchorPoint(ccp(0.5, 0.5));
			icon->setPosition(ccp(m_spriteAward3->getContentSize().width / 2, m_spriteAward3->getContentSize().height / 2));
	
			char awardCount[20] = {0};
			sprintf(awardCount, "%d", awards[i].item_count);
			m_labelGetAward3->setString(awardCount);
			m_labelGetAward3->setZOrder(2);
		}
	}

    SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(454);
    m_effectGet = ParticleManager::Get()->createEffectSprite(454,"",false);
    if (m_effectGet)
    {
        //m_effectGet->getRoot()->setScale(UIManager::sharedManager()->getScaleFactor());
		SetAnimEndCallBack(this, callfunc_selector(GuildGetRedEnvolopeLayer::onFirtstAnimEnd));
		m_layerEffect->addChild(m_effectGet, 1, 300);
        CCSize s = m_layerEffect->getContentSize();
        m_effectGet->setPosition(ccp(s.width / 2, s.height / 2));
        m_effectGet->SetAnim(kType_Play, 1, true, this);
		m_effectGet->SetAnimPauseOnOver(true);
    }
}

std::vector<std::string> GuildGetRedEnvolopeLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }

    return result;
}

void GuildGetRedEnvolopeLayer::onBtnOk(CCObject* sender, CCControlEvent event)
{
	if (!m_bIsPlayingAnim)
	{
		m_bIsPlayingAnim = true;

		m_effectGet->PauseAllArmatureAnim();
		SetAnimEndCallBack(this, callfunc_selector(GuildGetRedEnvolopeLayer::onSecondAnimEnd));
		m_effectGet->SetAnim(kType_Play2, 1, true);
	}
}

GuildRedEnvolopeRecordLayer::GuildRedEnvolopeRecordLayer() :
	m_labelSenderName(NULL),
	m_btnClose(NULL),
	m_btnOk(NULL),
	m_layerRecordContent(NULL),
	m_layerTableView(NULL),
	m_labelTitle(NULL),
	m_layerBg(NULL)
{
}

GuildRedEnvolopeRecordLayer::~GuildRedEnvolopeRecordLayer()
{
	setLabelSenderName(NULL);
	setLayerRecordContent(NULL);
	setBtnClose(NULL);
	setBtnOK(NULL);
	setLayerTableView(NULL);
	setLabelTitle(NULL);
	setLayerBg(NULL);
}

SEL_MenuHandler GuildRedEnvolopeRecordLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler GuildRedEnvolopeRecordLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "record_selector_ok",		GuildRedEnvolopeRecordLayer::onBtnOk);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close_record",			GuildRedEnvolopeRecordLayer::onBtnClose);

	return NULL;
}

 bool GuildRedEnvolopeRecordLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
 {
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "record_btn_ok",						 CCControlButton*,       m_btnOk);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "record_close_btn",					 CCControlButton*,       m_btnClose);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sender_name",						 CCLabelTTF*,			 m_labelSenderName);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "record_content_layer",				 CCLayer*,				 m_layerRecordContent);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_record_title",				 CCLabelTTF*,			 m_labelTitle);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "record_black_bg",					 CCLayerColor*,		     m_layerBg);

	 return true;
 }

void GuildRedEnvolopeRecordLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

	float scale = UIManager::sharedManager()->getScaleFactor();
	setScale(scale);

	m_layerBg->setContentSize(s);
	m_layerBg->setScale(m_layerBg->getScale() / scale);
}

bool GuildRedEnvolopeRecordLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRedEnvolopeRecordLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchPriority, true);
}

void GuildRedEnvolopeRecordLayer::onEnter()
{
	CCLayer::onEnter();

	m_btnClose->setTouchPriority(kCCLayerMenuTouchPriority);
	m_btnOk->setTouchPriority(kCCLayerMenuTouchPriority);

	m_btnOk->setTitleForState(CCString::create(Localizatioin::getLocalization("M_CONFIRM")), CCControlStateNormal);

	m_labelSenderName->setString(GuildManager::Get()->getCurrentRedEnvoSenderName().c_str());

	m_labelTitle->setString(Localizatioin::getLocalization("G_RRECORD_TITLE"));

	m_btnOk->setTitleForState(CCString::create(Localizatioin::getLocalization("M_ENSURE")), CCControlStateNormal);

	if (GuildManager::Get()->getRedEnvoRecordInfo().size() == 0)
		return;

	ConfigUI();
}

std::vector<std::string> GuildRedEnvolopeRecordLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }

    return result;
}

void GuildRedEnvolopeRecordLayer::onBtnOk(CCObject* sender, CCControlEvent event)
{
	MainMenuLayer* layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();

	if (layer)
	{
		layer->ShowGuildRedEnvoRecordLayer(false);
	}
}

void GuildRedEnvolopeRecordLayer::onBtnClose(CCObject* sender, CCControlEvent event)
{
	MainMenuLayer* layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();

	if (layer)
	{
		layer->ShowGuildRedEnvoRecordLayer(false);
	}
}

CCSize GuildRedEnvolopeRecordLayer::cellSizeForTable(CCTableView *table)
{
	return CCSize(550, 70);
}

CCTableViewCell* GuildRedEnvolopeRecordLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();

	if(cell)
	{
		RefreshCell(cell, idx);
	}
	else
	{
		cell = new CCTableViewCell();
		cell->autorelease();
		RefreshCell(cell, idx);
	}

	return cell;
}

unsigned int GuildRedEnvolopeRecordLayer::numberOfCellsInTableView(CCTableView *table)
{
	return GuildManager::Get()->getRedEnvoRecordInfo().size();
}

void GuildRedEnvolopeRecordLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void GuildRedEnvolopeRecordLayer::ConfigUI()
{
    bool needSetOffset = false;
    CCPoint offset = CCPointZero;
	if(this->m_layerTableView)
    {
        offset.x = m_layerTableView->getContentOffset().x;
        offset.y = m_layerTableView->getContentOffset().y;
        needSetOffset = true; 
        this->m_layerTableView->removeFromParentAndCleanup(true);
        setLayerTableView(NULL);
    }

    // 添加商品列表
	setLayerTableView(CCTableView::create(this, m_layerRecordContent->getContentSize()));
    m_layerTableView->setDirection(kCCScrollViewDirectionVertical);
	m_layerTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

    m_layerRecordContent->addChild(m_layerTableView);

    m_layerTableView->setTouchPriority(kCCLayerMenuTouchPriority);
    m_layerTableView->setDelegate(this);
    m_layerTableView->reloadData();

}

void GuildRedEnvolopeRecordLayer::RefreshCell(CCTableViewCell* cell, unsigned int index)
{
	if (! cell->getChildByTag(111))
	{
		CCLayer* layer = CreateSingleGuildRedEnvoRecordLayer();
		cell->addChild(layer, 0, 111);
		layer->setPosition(ccp(0, 0));
		layer->setAnchorPoint(ccp(0, 0));
	}

	SingleGuildRedEnvoRecordLayer* layer1 = dynamic_cast<SingleGuildRedEnvoRecordLayer*>(cell->getChildByTag(111));
	if (layer1 != NULL && (index) < GuildManager::Get()->getRedEnvoRecordInfo().size())
	{
		layer1->InitLayer(index, this, GuildManager::Get()->getRedEnvoRecordInfo()[index]);
	}
}

CCLayer* GuildRedEnvolopeRecordLayer::CreateSingleGuildRedEnvoRecordLayer()
{
    CCNodeLoader* loader = new SingleGuildRedEnvoRecordLayerLoader();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("single_red_envo_record", loader);

    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/single_guild_red_envo_record");

    return dynamic_cast<CCLayer*>(node);
}

SingleGuildRedEnvoRecordLayer::SingleGuildRedEnvoRecordLayer() :
	m_labelDate(NULL),
	m_labelInfo(NULL),
	m_labelTime(NULL)
{
}

SingleGuildRedEnvoRecordLayer::~SingleGuildRedEnvoRecordLayer()
{
	setLabelDate(NULL);
	setLabelInfo(NULL);
	setLabelTime(NULL);
}

SEL_MenuHandler SingleGuildRedEnvoRecordLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

SEL_CCControlHandler SingleGuildRedEnvoRecordLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
	return NULL;
}

bool SingleGuildRedEnvoRecordLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_date",						 CCLabelTTF*,       m_labelDate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_info",						 CCLabelTTF*,       m_labelInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_time",						 CCLabelTTF*,       m_labelTime);

	return true;
}

void SingleGuildRedEnvoRecordLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);
}

void SingleGuildRedEnvoRecordLayer::InitLayer(unsigned int index, GuildRedEnvolopeRecordLayer* layer, RedEnvoRecordInfo info)
{
	struct tm* time;
	time_t long_time;
	char timebuffer[15] = {0};
	char timebuffer1[20] = {0};

	long_time = info.modify_tm;
	time = localtime(&long_time);

	sprintf(timebuffer, "%d-%d-%d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);
	m_labelDate->setString(timebuffer);

	sprintf(timebuffer1, "%02d:%02d:%02d", time->tm_hour, time->tm_min, time->tm_sec);
	m_labelTime->setString(timebuffer1);

	std::vector<GuildRedEnvolopeInfo> info1 = GuildManager::Get()->getRedEnvolopeInfoFromXML();
	string award3 = "";
	for( int i = 0; i < info1.size(); i++)
	{
		if (info1[i].envo_id = info.envo_id)
		{
			award3 = info1[i].award3;
			info.envo_type = info1[i].envo_type;
			break;
		}
	}

	if (info.envo_type == 1)
	{
		if (award3 == "")
			return;

		vector<string> awards = split(award3, ";");
		vector<string> awards_id;
		vector<string> real_get_awards_id;
		if (awards.size() == 1)
		{
			awards_id.push_back(split(awards[0], "/")[0]);
		}
		else if (awards.size() == 2)
		{
			awards_id.push_back(split(awards[0], "/")[0]);
			awards_id.push_back(split(awards[1], "/")[0]);
		}
		else if (awards.size() == 3)
		{
			awards_id.push_back(split(awards[0], "/")[0]);
			awards_id.push_back(split(awards[1], "/")[0]);
			awards_id.push_back(split(awards[2], "/")[0]);
		}

		vector<string> logs = split(info.log, "/");
		int num = 0;
		int award_index = 0;

		for (int i = 0; i < logs.size(); i++)
		{
			if (logs[i] != "0")
			{
				num++;

				if (i >= 2 )
				{
					if (logs[i] != "0")
					{
						real_get_awards_id.push_back(awards_id[award_index]);
						award_index++;
					}
				}
			}
		}

		char temp[200] = {0};

		if (num == 2)
		{
			sprintf(temp, Localizatioin::getLocalization("G_RRECORD_TEXT3"), logs[0].c_str(), logs[1].c_str());
		}
		else if (num == 3)
		{
			sprintf(temp, Localizatioin::getLocalization("G_RRECORD_TEXT2"), logs[0].c_str(), logs[1].c_str(), logs[2].c_str(), ItemManager::Get()->getItemInfoById(atoi(real_get_awards_id[0].c_str()))->m_name);
		}
		else if (num == 4)
		{
			sprintf(temp, Localizatioin::getLocalization("G_RRECORD_TEXT4"), logs[0].c_str(), logs[1].c_str(), 
				logs[2].c_str(), ItemManager::Get()->getItemInfoById(atoi(real_get_awards_id[0].c_str()))->m_name,
				logs[3].c_str(), ItemManager::Get()->getItemInfoById(atoi(real_get_awards_id[1].c_str()))->m_name);
		}
		else if (num == 5)
		{
			sprintf(temp, Localizatioin::getLocalization("G_RRECORD_TEXT5"), logs[0].c_str(), logs[1].c_str(), 
				logs[2].c_str(), ItemManager::Get()->getItemInfoById(atoi(real_get_awards_id[0].c_str()))->m_name,
				logs[3].c_str(), ItemManager::Get()->getItemInfoById(atoi(real_get_awards_id[1].c_str()))->m_name,
				logs[4].c_str(), ItemManager::Get()->getItemInfoById(atoi(real_get_awards_id[2].c_str()))->m_name);
		}

		m_labelInfo->setString(temp);
	}
	else if (info.envo_type == 2)
	{
		vector<string> logs = split(info.log, "/");
		char temp[200] = {0};

		sprintf(temp, Localizatioin::getLocalization("G_RRECORD_TEXT3"), logs[0].c_str(), logs[1].c_str());

		m_labelInfo->setString(temp);
	}
}

void SingleGuildRedEnvoRecordLayer::onEnter()
{
}

std::vector<std::string> SingleGuildRedEnvoRecordLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }

    return result;
}
