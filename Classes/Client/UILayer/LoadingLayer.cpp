#include "LoadingLayer.h"
#include "GameManager.h"
#include "SceneLayer.h"
#include "UIDefine.h"
#include "UIManager.h"
#include "GameResourceManager.h"

LoadingLayer::LoadingLayer()
{

}

LoadingLayer::~LoadingLayer()
{
	
}

bool LoadingLayer::init()
{
	bool bRet = false;
	do {
		CCLOG("LoadingLayer::init()");
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("UI/LOADING.jpg");
		CCSprite* pSprite = CCSprite::create(path);
		pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		pSprite->setAnchorPoint(ccp(0.5,0.5));
		this->addChild(pSprite, 0);

		float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
		float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
		float scale = (scaleX > scaleY) ? scaleX : scaleY;
		pSprite->setScaleX(scale);
		pSprite->setScaleY(scale);

		/*CCSprite* pTextSprite = CCSprite::create("LoadingText.png");
		scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
		pTextSprite->setScale(scale);
		pTextSprite->setPosition(ccp(pTextSprite->getContentSize().width / 2,pTextSprite->getContentSize().height / 2));
		addChild(pTextSprite,1);*/

		setTouchEnabled(true);
		bRet = true;
	}while (0);
	return true;
}

void LoadingLayer::update(float dt)
{
	SceneLayer* layer = GameManager::Get()->GetSceneLayer();
	if(layer && !b_Hide)
	{
		b_Hide = true;
		layer->hideLoading();
	}
}

void LoadingLayer::ShowLoading(bool isShowOnly /* = false */)
{
	this->setVisible(true);
	if (false == isShowOnly)
	{
		b_Hide = false;
		// loading加载成功的回调，第二个参数为延迟时间
		scheduleOnce(schedule_selector(LoadingLayer::update), 0.01f);
	}
}

void LoadingLayer::HideLoading()
{
	this->setVisible(false);
	b_Hide = true;
}

bool LoadingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(isVisible())
	{
		return true;
	}
  
	return false;
}

void LoadingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchMoved(pTouch,pEvent);
}

void LoadingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchEnded(pTouch,pEvent);
}

void LoadingLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchCancelled(pTouch,pEvent);
}

void LoadingLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCMenuOnMessageBoxPripority, true);
}


