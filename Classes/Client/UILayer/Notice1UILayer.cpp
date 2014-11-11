//
//  Notice1UILayer.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-10-23.
//
//
#include "json_res.h"
#include "Notice1UILayer.h"
#include "NoticeSceneLayer.h"

#include "GameResourceManager.h"
#include "Localization.h"
#include "GameFontManager.h"
#include "GMessage.h"
#include "UserData.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "LuaTinkerManager.h"
#include "HtmlNode.h"
#include "AssetsManager.h"
#include "XLogicManager.h"
#include "FileManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "LoginManager.h"
using namespace TXGUI;

const char* url = "http://10.1.1.155/server/test_json.php";
const char* countKey = "countKeyttt";
const char* annIdKey = "annIdKey";

Notice1UILayer::Notice1UILayer()
{
    this->m_pLayer = NULL;
    this->m_pList = NULL;
}

Notice1UILayer::~Notice1UILayer()
{
    UIManager::sharedManager()->RemoveUILayout("Notice1UILayer");
    delete  m_pAssetsManager;
    m_pAssetsManager = NULL;
}

void Notice1UILayer::setTouchEnabled(bool value)
{
	CCLayer::setTouchEnabled(value);
}

bool Notice1UILayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
   /* CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();*/
    
    m_pAssetsManager = new AssetsManager("", "");
    m_pAssetsManager->setDelegate(this);
    
	/*CCSprite* pSprite = CCSprite::create("UI/frontCover.jpg");
	if (pSprite == NULL)
	{
		pSprite = new CCSprite();
		pSprite->initWithTexture(NULL,CCRectZero);
	}
    
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pSprite->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(pSprite, -1,1001);*/
    
	/*if (pSprite->getTexture())
	{
		float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
		float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
		float scale = (scaleX > scaleY) ? scaleX : scaleY;
		pSprite->setScaleX(scale);
		pSprite->setScaleY(scale);
	}*/
    
    this->requestHttp();
    
	return true;
}

void Notice1UILayer::loadPlist()
{
	UIManager *manager = UIManager::sharedManager();
	m_pLayer = manager->CreateUILayoutFromFile("UIplist/Notice1.plist", this, "Notice1UILayer", true);
    manager->RegisterMenuHandler("Notice1UILayer", "Notice1UILayer::OnClose", menu_selector(Notice1UILayer::OnClose), this);
	m_pList = m_pLayer->FindChildObjectByName<UIScrollList>("NOTICE1_TEXT_CONTENT");
    m_pList->clearChildrens();
}

void Notice1UILayer::requestHttp()
{
	std::string url = "";
    const char* urlx = NetWorkConfigInfoFromServer::sharedInstance()->GetNoticeURL();
	url.append(urlx);
	url.append("?channel=");
	unsigned int channelId = LoginManager::Get()->getChannelID();
	char channel[20];
	sprintf(channel,"%u",channelId);
	url.append(channel);
    m_pAssetsManager->start(url);
}

void Notice1UILayer::OnClose(CCObject *sender)
{
    UIManager::sharedManager()->RemoveUILayout("Notice1UILayer");
    scheduleOnce(schedule_selector(Notice1UILayer::gotoLayer), 0.02f);
}

void Notice1UILayer::gotoLayer(float dt)
{
    //this->removeFromParentAndCleanup(true);
    //XLogicManager::sharedManager()->goToLayer(XLogicManager::E_SELECT_ZONE,NULL);
	NoticeSceneLayer* parent = dynamic_cast<NoticeSceneLayer*>(this->getParent());
	if(NULL != parent)
	{
		parent->gotoLayer();
	}
	else
	{
		this->removeFromParentAndCleanup(true);
	}
}

void Notice1UILayer::insert(int r, int g, int b, int fontsize, const std::string& context)
{
	CCLOG("Notice1UILayer::insert: %s",context.c_str());
    CCSize contentSize = m_pList->getContentSize();
    float scale = UIManager::sharedManager()->getScaleFactor();
    //CCSize sz = CCSizeMake(contentSize.width/scale, contentSize.height/scale);
    //float maxWidth = sz.width-10*scale;
    //float maxWidth = sz.width;
    
    ccColor3B wordColor = ccc3(r,g,b);
	/*HtmlNode* node = HtmlNode::create(maxWidth);
	node->begin();
	node->addLabel(context.c_str(),fontsize,KJLinXin,wordColor);
	node->finish();*/
	CCSize sz = CCSizeMake(contentSize.width/scale, 0);
	CCLabelTTF* node = CCLabelTTF::create(context.c_str(),KJLinXin,fontsize,sz,kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
	if(node->getContentSize().width != 0 && node->getContentSize().height != 0)
	{
		node->setColor(wordColor);
		node->setAnchorPoint(ccp(0.5,0.5));

		CCSize size = node->getContentSize();
		m_pList->AddBaseItem(CCSize(size.width, size.height),node);
	}
	
    
    //float oldPos_y = node->getPosition().y;
    //node->setPosition(ccp((0+m_pList->getContentSize().width)/2, oldPos_y));
}

void Notice1UILayer::operationDidFinish(AssetsManager *operation, const char* strVal)
{
    CCLOG("reslist:%s, strlen:%d\n", strVal, strlen(strVal));
    CCLOG("Notice1UILayer: operationDidFinish\n");
    
    if(strlen(strVal) == 0)
    {
        scheduleOnce(schedule_selector(Notice1UILayer::transScene), 0.5f);
        return;
    }
    
    Json::Value jsonValue;
    if (!parseJsonStr(strVal, jsonValue))
    {
        CCLOGERROR("Notice1UILayer Error: %s",strVal);
        scheduleOnce(schedule_selector(Notice1UILayer::transScene), 0.5f);
        return;
    }
    
    const Json::Value noteArray = jsonValue["note"];
    const Json::Value contextArray = noteArray["context"];
    const Json::Value channidArray = jsonValue["channelid"];
    
    std::string str_count = noteArray["count"].asString();
     int  count= atoi(str_count.c_str());
    std::string str_annid =  noteArray["ann_id"].asString();
     int  annid = atoi(str_annid.c_str());
    
    int reviewcnt = CCUserDefault::sharedUserDefault()->getIntegerForKey(countKey, 0);
    int reviewAnnid = CCUserDefault::sharedUserDefault()->getIntegerForKey(annIdKey, 0);
    
    if(annid == reviewAnnid)
    {
        if(reviewcnt >= count && count != 0)
        {
            scheduleOnce(schedule_selector(Notice1UILayer::transScene),0.5f);
            return;
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(countKey, reviewcnt+1);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(countKey, 1);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(annIdKey, annid);
        CCUserDefault::sharedUserDefault()->flush();
    }                 
    
    loadPlist();
    
    for (int i=0; i<contextArray.size(); i++) {
        
        std::string str_text = contextArray[i]["text"].asString();
         std::string str_fontsize= contextArray[i]["fontsize"].asString();
        int fontsize = atoi(str_fontsize.c_str());
        std::string str_red= contextArray[i]["red"].asString();
        int red = atoi(str_red.c_str());
        
        std::string str_green = contextArray[i]["green"].asString();
        int green = atoi(str_green.c_str());
        std::string str_blue= contextArray[i]["blue"].asString();
        int blue = atoi(str_blue.c_str());
        
        //str_text = "见面。text:%s, fontsize:%d, red:%d, green:%d, blue:%d对于最终入驻于《";
        //CCLOG("text:%s, fontsize:%d, red:%d, green:%d, blue:%d \n", str_text.c_str(), fontsize, red, green, blue);
        //CCLOG("str_text length:%d\n", str_text.length());
        //if(fontsize != 0)
		{
			 insert(red,green,blue,25,str_text);
		}
       
    }
}

void Notice1UILayer::operationDidFail(AssetsManager *operation)
{
    CCLOG("Notice1UILayer: operationDidFail\n");
	OnClose(NULL);
}

void Notice1UILayer::transScene(float dt)
{
    OnClose(NULL);
}

