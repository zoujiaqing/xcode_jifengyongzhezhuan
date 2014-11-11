//
//  Notice1UILayer.cpp
//  HelloLua
//
//  Created by rickycheng  on 13-10-23.
//
//

#include "Notice1UILayer.h"
#include "NoticeSceneLayer.h"
#include "XLogicManager.h"

NoticeSceneLayer::NoticeSceneLayer()
{

}

NoticeSceneLayer::~NoticeSceneLayer()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

bool NoticeSceneLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* pSprite = CCSprite::create("UI/frontCover.jpg");
	if (pSprite == NULL)
	{
		pSprite = new CCSprite();
		pSprite->initWithTexture(NULL,CCRectZero);
	}
    
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pSprite->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(pSprite, -1,1001);
    
	if (pSprite->getTexture())
	{
		float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
		float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
		float scale = (scaleX > scaleY) ? scaleX : scaleY;
		pSprite->setScaleX(scale);
		pSprite->setScaleY(scale);
	}
    
	Notice1UILayer* pNoticeParent = Notice1UILayer::create();
    this->addChild(pNoticeParent);

	return true;
}

void NoticeSceneLayer::gotoLayer()
{
    this->removeFromParentAndCleanup(true);
    XLogicManager::sharedManager()->goToLayer(XLogicManager::E_SELECT_ZONE,NULL);
}
