#include "SpriteBase.h"
#include "LevelDefine.h"

#include "AnimSprite.h"
#include "Engine.h"
#include "EnginePlayerManager.h"
#include "CConnectionData.h"
#include "GameManager.h"
#include "SpriteSeer.h"

SpriteBase::SpriteBase(int type)
	:m_type(type)
	,m_anim(NULL)
	,m_TypeID(-1)
{
//	CCSprite::init();
//	CCNode::create();

	//add root sprite
	m_ID.setPlayId(0,0,0);
	m_rootSprite =  CCNode::create();

	m_rootSprite->setScale(LevelMultiResolution::sGetLevelScale());

	this->addChild(m_rootSprite, 0);	
}



SpriteBase::~SpriteBase()
{
	this->unscheduleAllSelectors();	

	if (m_anim != NULL)
	{
		EnginePlayerManager::getInstance()->removeAsprite(m_anim);
		m_anim = NULL;
	}
}

bool SpriteBase::canDealWithTouch(cocos2d::CCTouch* touch)
{
	return containsTouchLocation(touch) && Engine::CanTouchNow();
}

bool SpriteBase::containsTouchLocation(cocos2d::CCTouch* touch)
{
    if (GameManager::Get()->getHero() == NULL)
        return false;   
    
	CCPoint oldPT = touch->getLocationInView();
	oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);
    
	CCPoint ptHero = GameManager::Get()->getHero()->getPosition();
    
	CCPoint newPT = LevelMultiResolution::sTransformWindowPointToMap(oldPT);
	if (isTouchSelf(CCPointMake(newPT.x, newPT.y)))
	{        
		return true;
	}
	else
	{
		return false;
	}
}

bool SpriteBase::isTouchSelf(cocos2d::CCPoint pt)
{
	cocos2d::CCRect rect = getRect();
	
	if((pt.x >= rect.origin.x) && 
		(pt.x <= rect.origin.x + rect.size.width) &&
		(pt.y >= rect.origin.y) && 
		(pt.y <= rect.origin.y + rect.size.height))
	{
		return true;
	}

	return false;	
}


void SpriteBase::setPosition(cocos2d::CCPoint newPosition)
{
    float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
    
    CCPoint realPosition = CCPoint(newPosition.x / deviceScale, newPosition.y / deviceScale);
    
	m_rootSprite->setPosition(realPosition);
}

cocos2d::CCPoint SpriteBase::getPosition()
{
    return m_rootSprite->getPositionInPixels();
}

cocos2d::CCRect SpriteBase::getRect()
{
	if(m_anim != NULL)
	{
		//cocos2d::CCSize size = m_anim->getCurContentSize();
		cocos2d::CCRect rect = m_anim->getCurFrameRect();		

		return CCRectMake(rect.origin.x , rect.origin.y , rect.size.width * LevelMultiResolution::sGetLevelScale(), 
			rect.size.height * LevelMultiResolution::sGetLevelScale());	
	}	

	return CCRectMake(0, 0, 0, 0);	
}

void SpriteBase::addChildToRoot( cocos2d::CCNode* pChild )
{
	m_rootSprite->addChild(pChild);
}

bool SpriteBase::getAttachPoint(int pointPosType,CCPoint &point)
{
	point = CCPointZero;
	std::map<int,std::map<int,CCPoint> > * pointsMap = CCConnectionData::sharedConnectionData()->getAttachPointsMap();
	std::map<int,std::map<int,CCPoint> >::iterator iter = pointsMap->find(m_TypeID);
	if (iter == pointsMap->end())
	{
		// not found
		return false;
	}
	int vecSize = (*iter).second.size();
	if (pointPosType < 0 || pointPosType >= vecSize)
	{
		return false;
	}
	point = (*iter).second[(int)pointPosType];
	return true;
}
