//
//  TipMgr.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-5-6.
//
//

#include "TipMgr.h"
#include "UILabel.h"

#include "ASprite.h"
#include "GameFontManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "GameAudioManager.h"
#include "TXGUIHeader.h"
#include "IconButton.h"
#include "Localization.h"
#include "TipTouchLayer.h"
#include "HtmlNode.h"
#include "XLogicManager.h"

#include "ccUTF8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "TmLoginManager.h"
#endif
using namespace cocos2d;


//utf8◊÷∑˚≥§∂»1-6£¨ø…“‘∏˘æ›√ø∏ˆ◊÷∑˚µ⁄“ª∏ˆ◊÷Ω⁄≈–∂œ’˚∏ˆ◊÷∑˚≥§∂»
//110xxxxx 10xxxxxx
//1110xxxx 10xxxxxx 10xxxxxx
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//

#define UTFLEN(x)  utf8_look_for_table[(x)]

static unsigned char utf8_look_for_table[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

static int getUtf8Length(const char *str)
{
    int clen = strlen(str);
    int len = 0;
    for(const char *ptr = str;
        *ptr!=0&&len<clen;
        len++, ptr+=UTFLEN((unsigned char)*ptr));
    
    return len;
}

static char* subUtfString(const char *str, unsigned int start, unsigned int end)
{
    int len = getUtf8Length(str);
    
    if(start >= len) return NULL;
    if(end > len) end = len;
    
    const char *sptr = str;
    for(int i = 0; i < start; ++i,sptr+=UTFLEN((unsigned char)*sptr));
    
    const char *eptr = sptr;
    for(int i = start; i < end; ++i,eptr += UTFLEN((unsigned char)*eptr));
    
    int retLen = eptr - sptr;
    char *retStr = (char*)malloc(retLen+1);
    memcpy(retStr, sptr, retLen);
    retStr[retLen] = 0;
    return retStr;
}

TipMgr::TipMgr()
{
    this->m_pColorLayer = NULL;
    this->m_pIcon = NULL;
    this->m_bkSprite = NULL;
    this->m_childcnt = 0;
    this->m_maxHeight = 0;
    this->m_maxWidth = 0;
    m_touchLayer = NULL;

    this->m_tapx = 10;
    this->m_tapy = 30;
	m_delta_y = 2;
	m_delta_x = 5;
	m_start_x = 10;
	m_start_y = 10;
	m_maxContentSize = 280;

	pbtn_ok = NULL;
	pbtn_cancle = NULL;
	pbtn_middle = NULL;
	label_ok = NULL;
	label_cancle = NULL;
	m_okHandler = NULL;
	m_cancellHander = NULL;
	m_middleHander = NULL;
	label_middle = NULL;
	m_luaOkHandler = 0;
	m_luaCanellHander = 0;
    
	m_marqueePoint = CCPointZero;
    m_pChildNode = CCArray::create();
    m_pChildNode->retain();
}

TipMgr::~TipMgr()
{
    CC_SAFE_RELEASE(m_pChildNode);
}

void TipMgr::checkTipLayer()
{
#ifdef TargetForKy
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif
#ifdef TargetForPP
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif
#ifdef TargetForNd91
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif
#ifdef TargetForTB
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif
#ifdef TargetForI4
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif
#ifdef TargetForItools
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif
#ifdef TargetForXy
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        return;
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(g_sdk_ret_code == 5){//duoku zhu xiao ui return to front copver
	//crash here	
		cocos2d::CCLog("timpmr duoku just return:");
		g_sdk_ret_code=0;
		return;
	}

#endif
	if(GameManager::Get()->GetSceneLayer())
	{
		int tag = MainMenuLayer::getLayerTagByName("TipTouchLayer");
		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		m_touchLayer = MainMenuLayer::getUILayerByTag<CCLayer>(tag,false);
		if(m_touchLayer == NULL)
		{
			m_touchLayer = TipTouchLayer::create();
			mLayer->addChild(m_touchLayer,0,tag);
		}
	}
}

void TipMgr::createTip(cocos2d::CCSprite *icon, CCPoint spt)
{
	GameAudioManager::sharedManager()->playEffect(350001,false);
	checkTipLayer();
    destroyTip();
    m_pt = spt;
    
    ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
    int idx = getResourceIDByName("map_ui_FRAME_TIPS_BACKGROUND");
    CCPoint pt;
    m_bkSprite = as->getSpriteFromFrame_Middle(idx, 0, pt);

    float scaleFactor = UIManager::sharedManager()->getScaleFactor();
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    m_pChildNode->removeAllObjects();
	m_pColorLayer = CCLayer::create();
	UIManager* manager = UIManager::sharedManager();
	UILayout* layout = manager->CreateUILayoutFromFile("UIplist/tipLayer.plist",m_pColorLayer,"TipLayout",true);
	layout->setScale(1.0f);
	CCAssert(layout,"TipLayout create fail");
	if(!layout)
	{
		destroyTip();
		return;
	}
	manager->RegisterMenuHandler("TipLayout","TipMgr::OnOK",menu_selector(TipMgr::OnOK), this);
	manager->RegisterMenuHandler("TipLayout","TipMgr::OnCancel",menu_selector(TipMgr::OnCancel), this);
	manager->RegisterMenuHandler("TipLayout","TipMgr::OnMiddleBtClicked",menu_selector(TipMgr::OnMiddleBtClicked), this);
	pbtn_ok = layout->FindChildObjectByName<UIButton>("okBt");
	pbtn_cancle = layout->FindChildObjectByName<UIButton>("cancellBt");
	pbtn_middle = layout->FindChildObjectByName<UIButton>("middleBt");
	label_ok = layout->FindChildObjectByName<UILabel>("OkLable");
	label_cancle = layout->FindChildObjectByName<UILabel>("CancellLable");
	label_middle = layout->FindChildObjectByName<UILabel>("MiddleLable");
    m_pIcon = icon;
    
    this->m_childcnt = 0;
    this->m_maxHeight = 0;
    this->m_maxWidth = 0;
}

void TipMgr::add(cocos2d::CCSprite *labelIcon, const char *message, cocos2d::ccColor3B color, int fontsize)
{
    /*CCLabelTTF* labelName = CCLabelTTF::create(message, KJLinXin, fontsize);
    int length = getUtf8Length(message);
    if(labelName->getContentSize().width >= m_pColorLayer->getContentSize().width)
    {
        char *str1 = subUtfString(message, 0, length/3);
        char *str2 = subUtfString(message, length/3,length);
        add(labelIcon, str1, color, fontsize);
		free(str1);
        add(str2, color, fontsize);
		free(str2);
    }
    else
    {
        CCNode *node = CCNode::create();
        CCLabelTTF* labelName = CCLabelTTF::create(message, KJLinXin, fontsize);
        labelName->setColor(color);
        node->addChild(labelIcon);
        node->addChild(labelName);
        float height = max(labelIcon->getContentSize().height, labelName->getContentSize().height);
        node->setContentSize(CCSize(m_pColorLayer->getContentSize().width, height));
        
        labelIcon->setPosition(ccp(m_tapx-node->getContentSize().width/2 + labelIcon->getContentSize().width/2, node->getContentSize().height/2));
        labelName->setPosition(ccp(labelIcon->getPosition().x+labelIcon->getContentSize().width/2 + labelName->getContentSize().width/2,node->getContentSize().height/2));
        labelName->setAnchorPoint(ccp(0,0));
        labelIcon->setAnchorPoint(ccp(0,0));

        m_pChildNode->addObject(node);
    }*/
	HtmlNode* node = HtmlNode::create(m_maxContentSize);
	node->begin();
	node->addImage(labelIcon);
	node->addLabel(message,fontsize,KJLinXin,color);
	node->finish();
	node->setAnchorPoint(ccp(0,0));
	node->setPosition(CCPointZero);
	m_pChildNode->addObject(node);
}

void TipMgr::add(const char *message, cocos2d::ccColor3B color, int fontsize)
{
	HtmlNode* node = HtmlNode::create(m_maxContentSize);
	node->begin();
	node->addLabel(message,fontsize,KJLinXin,color);
	node->finish();
	node->setAnchorPoint(ccp(0,0));
	node->setPosition(CCPointZero);
	m_pChildNode->addObject(node);
}

HtmlNode* TipMgr::addPart(const char *message, cocos2d::ccColor3B color, int fontsize)
{
	HtmlNode* node = HtmlNode::create(m_maxContentSize);
	node->begin();
	node->addLabel(message,fontsize,KJLinXin,color);
	return node;
}

void TipMgr::addPart(HtmlNode* node, const char *message, cocos2d::ccColor3B color, int fontsize,bool isFinish)
{
	node->addLabel(message,fontsize,KJLinXin,color);
	if(isFinish)
	{
		node->finish();
		node->setAnchorPoint(ccp(0,0));
		node->setPosition(CCPointZero);
		m_pChildNode->addObject(node);
	}
}

void TipMgr::addNodeDone()
{
	CCAssert(m_pColorLayer,"m_pColorLayer is NULL");
	CCAssert(m_bkSprite,"m_bkSprite is NULL");

    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	int layerTag = MainMenuLayer::getLayerTagByName("TipsLayer");
    mLayer->addChild(m_pColorLayer,87,layerTag);
    
	int spriteTag = MainMenuLayer::getLayerTagByName("TipsSpriteLayer");
    mLayer->addChild(m_bkSprite,86,spriteTag);
    
	m_maxHeight = m_start_y;
    if(m_pChildNode && m_pChildNode->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildNode, child)
        {
            CCNode* pChild = (CCNode*) child;
            if (pChild)
            {
                m_maxHeight += pChild->getContentSize().height + m_delta_y;
                m_maxWidth = max(m_maxWidth, pChild->getContentSize().width + m_start_x);
            }
        }
    }
	m_maxWidth += m_pIcon->getContentSize().width + m_tapx + m_delta_x;
	float iconHeight = m_pIcon->getContentSize().height + m_tapy;
    m_maxHeight = m_maxHeight > iconHeight ? m_maxHeight : iconHeight;
    //m_maxWidth += m_pIcon->getContentSize().width + 50;
    //CCNode* plastnode = (CCNode*)m_pChildNode->objectAtIndex(m_pChildNode->count()-1);
    //m_maxHeight += plastnode->getContentSize().height*1.5f;
    
    const char* str_normal = "map_ui_FRAME_TIPS_BUTTON";
    CCSprite* normal = AspriteManager::getInstance()->getOneFrame(KUI_BIN, str_normal);
	CCSize btSize = normal->getContentSize();
	m_maxWidth = max(m_maxWidth, 3 * btSize.width);
    m_maxHeight += normal->getContentSize().height;
    
    //int width = m_pColorLayer->getContentSize().width;
	float width = m_maxWidth;
    float height = m_maxHeight;
    m_pColorLayer->setContentSize(CCSize(width,height));
    
    posNode();
	if(m_touchLayer)
	{
		m_touchLayer->setVisible(true);
	}

}

void TipMgr::posNode()
{
    if(m_pChildNode && m_pChildNode->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildNode, child)
        {
            CCNode* node = (CCNode*) child;
            if (node)
            {
                node->setAnchorPoint(ccp(0,0));
                
                if(m_childcnt == 0)
                {
					node->setPosition(ccp(m_start_x + node->getContentSize().width * 0.5f, m_pColorLayer->getContentSize().height - node->getContentSize().height - m_start_y));
                }
                else
                {
                    CCNode* prevnode = (CCNode*)m_pColorLayer->getChildByTag(m_childcnt-1);
                    if(prevnode)
                    {
						node->setPosition(ccp(m_start_x + node->getContentSize().width * 0.5f, prevnode->getPosition().y - node->getContentSize().height - m_delta_y));
                    }
                }
                
                m_pColorLayer->addChild(node,0,m_childcnt);
                m_childcnt += 1;
            }
        }
    }
    
    m_pColorLayer->setAnchorPoint(ccp(0.f, 0.f));
    m_pColorLayer->addChild(m_pIcon);
    m_pIcon->setAnchorPoint(CCPointZero);
    m_pIcon->setPosition(ccp(m_pColorLayer->getContentSize().width - m_pIcon->getContentSize().width-m_tapx,
                             m_pColorLayer->getContentSize().height - m_pIcon->getContentSize().height-m_tapy));
    
	CCSize buttonSize = pbtn_ok->getMenuItemSprite()->getContentSize();
	//pbtn_ok->setPosition(ccp(m_pColorLayer->getContentSize().width/2 - buttonSize.width/2,buttonSize.height/2));
	pbtn_ok->setPosition(ccp(buttonSize.width/2,buttonSize.height/2));
	pbtn_middle->setPosition(ccp(m_pColorLayer->getContentSize().width/2,buttonSize.height/2));
	//pbtn_cancle->setPosition(ccp(m_pColorLayer->getContentSize().width/2 + buttonSize.width/2,buttonSize.height/2));
	pbtn_cancle->setPosition(ccp(m_pColorLayer->getContentSize().width - buttonSize.width/2,buttonSize.height/2));
	if(label_ok)
	{
		//label_ok->setPosition(ccp(m_pColorLayer->getContentSize().width/2 - buttonSize.width/2, buttonSize.height/2));
		label_ok->setPosition(ccp(buttonSize.width/2, buttonSize.height/2));
	}
	if(label_cancle)
	{
		//label_cancle->setPosition(ccp(m_pColorLayer->getContentSize().width/2 + buttonSize.width/2,buttonSize.height/2));
		label_cancle->setPosition(ccp(m_pColorLayer->getContentSize().width - buttonSize.width/2,buttonSize.height/2));
	}
	if(label_middle)
	{
		label_middle->setPosition(ccp(m_pColorLayer->getContentSize().width/2,buttonSize.height/2));
	}
	float scale = UIManager::sharedManager()->getScaleFactor();
	m_pColorLayer->setScale(scale);
	m_pColorLayer->setContentSize(CCSizeMake(m_pColorLayer->getContentSize().width * scale,m_pColorLayer->getContentSize().height * scale));

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	// tips 离点击点的偏移量
	float offset_x = 0.1f * visibleSize.width;
	float offset_y = 0.05f * visibleSize.height;
	if(m_pt.x > m_pColorLayer->getContentSize().width + offset_x * scale)  //pos at right
	{
		m_pt.x -= m_pColorLayer->getContentSize().width + offset_x * scale;
	}
	else
	{
		m_pt.x += offset_x;
	}

	if(m_pt.y > getMarqueePoint() - offset_y * scale - m_pColorLayer->getContentSize().height )
	{
		m_pt.y =  getMarqueePoint() - offset_y * scale - m_pColorLayer->getContentSize().height ;
	}

	m_pColorLayer->setPosition(m_pt);
	float scalex = m_pColorLayer->getContentSize().width/m_bkSprite->getContentSize().width;
	float scaley = m_pColorLayer->getContentSize().height/m_bkSprite->getContentSize().height;
	m_bkSprite->setAnchorPoint(CCPointZero);
	m_bkSprite->setScaleX(scalex);
	m_bkSprite->setScaleY(scaley);
	m_bkSprite->setPosition(m_pt);

	pbtn_ok->setVisible(false);
	pbtn_cancle->setVisible(false);
	pbtn_middle->setVisible(false);
	if(label_ok)
	{
		label_ok->setVisible(false);
	}
	if(label_cancle)
	{
		label_cancle->setVisible(false);
	}
	if(label_middle)
	{
		label_middle->setVisible(false);
	}
}

float TipMgr::getMarqueePoint()
{
	/*if(m_marqueePoint.equals(CCPointZero))
	{
		ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
		CCPoint pt = CCPointZero;
		CCSprite* sp = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_TEXT_REVOLVELAMP"),0,pt);
		CCSize size = sp->getContentSize();
		float scale = UIManager::sharedManager()->getScaleFactor();
		m_marqueePoint.y = (pt.y - size.height * 0.5f) * scale;
	}*/
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	m_marqueePoint.y = visibleSize.height;
	return m_marqueePoint.y;
}

void TipMgr::destroyTip()
{
	checkTipLayer();
	UIManager::sharedManager()->RemoveUILayout("TipLayout");
    if(m_pColorLayer)
    {
        m_pColorLayer->removeFromParent();
        m_pColorLayer = NULL;
    }
    
    if(m_bkSprite)
    {
        m_bkSprite->removeFromParent();
        m_bkSprite = NULL;
    }
	pbtn_ok = NULL;
	pbtn_cancle = NULL;
	pbtn_middle = NULL;
	label_ok = NULL;
	label_cancle = NULL;
	label_middle = NULL;
	m_okHandler = NULL;
	m_cancellHander = NULL;
	m_middleHander = NULL;
	m_okTarget = NULL;
	m_cancellTarget = NULL;
	m_middleTarget = NULL;
	m_luaOkHandler = 0;
	m_luaCanellHander = 0;

	if(m_touchLayer)
	{
		m_touchLayer->setVisible(false);
	}
}

void TipMgr::removeTipAndTouchLayer()
{
	destroyTip();
	if(m_touchLayer)
	{
		m_touchLayer->removeFromParentAndCleanup(true);
		m_touchLayer = NULL;
	}
}

void TipMgr::OnOK(CCObject *sender)
{
    //CCLOG("on ok");
	if(m_okHandler != NULL && m_okTarget != NULL)
	{
		(m_okTarget->*m_okHandler)(sender);
	}
	destroyTip();
}

void TipMgr::OnCancel(CCObject *sender)
{
    //CCLOG("on cancle");
	if(m_cancellHander != NULL && m_cancellTarget != NULL)
	{
		(m_cancellTarget->*m_cancellHander)(sender);
	}
	destroyTip();
}

void TipMgr::OnMiddleBtClicked(CCObject *sender)
{
    //CCLOG("on cancle");
	if(m_middleHander != NULL && m_middleTarget != NULL)
	{
		(m_middleTarget->*m_middleHander)(sender);
	}
	destroyTip();
}

void TipMgr::setOKButton(cocos2d::CCObject*	target,cocos2d::SEL_MenuHandler handler,const char* text,bool isVisible)
{
	m_okTarget = target;
	m_okHandler = handler;

	if(pbtn_ok)
	{
		pbtn_ok->setVisible(isVisible);
	}
	if(label_ok)
	{
		label_ok->setVisible(isVisible);
		if(isVisible)
		{
			label_ok->setString(text);
		}
	}
	
}

void TipMgr::setCancellButton(cocos2d::CCObject* target,cocos2d::SEL_MenuHandler handler,const char* text,bool isVisible)
{
	m_cancellTarget = target;
	m_cancellHander = handler;
	if(pbtn_cancle)
	{
		pbtn_cancle->setVisible(isVisible);
	}
	if(label_cancle)
	{
		label_cancle->setVisible(isVisible);
		if(isVisible)
		{
			label_cancle->setString(text);
		}
	}
}

void TipMgr::setMiddleButton(cocos2d::CCObject* target,cocos2d::SEL_MenuHandler handler,const char* text,bool isVisible)
{
	m_middleTarget = target;
	m_middleHander = handler;
	if(pbtn_middle)
	{
		pbtn_middle->setVisible(isVisible);
	}
	if(label_middle)
	{
		label_middle->setVisible(isVisible);
		if(isVisible)
		{
			label_middle->setString(text);
		}
	}
}


void TipMgr::setOKLuaHandle(int hander,const char* text,bool isVisible)
{
	m_luaOkHandler = hander;
	if(pbtn_ok)
	{
		pbtn_ok->setVisible(isVisible);
		if(hander != 0)
		{
			pbtn_ok->getMenuItemSprite()->registerScriptTapHandler(hander);
		}
	}
	if(label_ok)
	{
		label_ok->setVisible(isVisible);
		if(isVisible)
		{
			label_ok->setString(text);
		}
	}
}

void TipMgr::setCancellLuaHandle(int hander,const char* text,bool isVisible)
{
	m_luaCanellHander = hander;
	if(pbtn_cancle)
	{
		pbtn_cancle->setVisible(isVisible);
		if(hander != 0)
		{
			pbtn_cancle->getMenuItemSprite()->registerScriptTapHandler(hander);
		}
	}
	if(label_cancle)
	{
		label_cancle->setVisible(isVisible);
		if(isVisible)
		{
			label_cancle->setString(text);
		}
	}
}

void TipMgr::setMiddleLuaHandle(int hander,const char* text,bool isVisible)
{
	m_luaMiddleHander = hander;
	if(pbtn_middle)
	{
		pbtn_middle->setVisible(isVisible);
		if(hander != 0)
		{
			pbtn_middle->getMenuItemSprite()->registerScriptTapHandler(hander);
		}
	}
	if(label_middle)
	{
		label_middle->setVisible(isVisible);
		if(isVisible)
		{
			label_middle->setString(text);
		}
	}
}