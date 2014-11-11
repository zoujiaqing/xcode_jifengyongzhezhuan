#include "TipTouchLayer.h"
#include "TipMgr.h"
#include "UIDefine.h"

TipTouchLayer::TipTouchLayer()
{

}

TipTouchLayer::~TipTouchLayer()
{

}

bool TipTouchLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	setTouchEnabled(true);
	return true;
}

void TipTouchLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCTipsLayerTouchPriority, false);
}

bool TipTouchLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(this->isVisible())
	{
		onTouchClicked();
		return true;
	}
	return false;
}

void TipTouchLayer::onTouchClicked()
{
	TipMgr::Get()->destroyTip();
}