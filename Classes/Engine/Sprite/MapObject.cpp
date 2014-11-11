#include <iostream>
#include "MapObject.h"
#include "AspriteManager.h"
#include "EnginePlayerManager.h"
#include "AnimSprite.h"

#include "UIDefine.h"
#include "CrossPlatform.h"
#include "LevelLayer.h"



using namespace cocos2d;



MapObject::MapObject( const char* spriteName, int objectId, int animIndex)
	:SpriteBase(MAP_TAG_ANIMATIONS)
{    
    initObjectWithId(spriteName, objectId, animIndex);     
}

MapObject::~MapObject()
{
    
}

void MapObject::initObjectWithId( const char* spriteName, int objectId, int animIndex)
{
    float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
    
	m_playing = false;	
	m_objectID = objectId;
	m_curAnimID = animIndex;

	m_anim = EnginePlayerManager::getInstance()->addAnimSprite(spriteName, 0, 0);	
	CCPoint pt = ccp(IPHONE_HD_OFFSET_X / deviceScale, IPHONE_HD_OFFSET_Y / deviceScale);
	m_anim->SetPos(pt.x, pt.y);	

	m_anim->SetAnim(animIndex,1, true);
	CCSpriteBatchNode* batchNode = m_anim->getBatchNode();
	m_rootSprite->addChild(batchNode, 1);

}

void MapObject::resumeDefaultAnim(CCNode *sender, void *data)
{
    m_playing = false;   
    m_anim->SetAnim(m_curAnimID , 1, true);
}


CCRect MapObject::getRect()
{
    float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
    
	CCRect rect = SpriteBase::getRect();
	rect.origin.x += IPHONE_HD_OFFSET_X / deviceScale;
	rect.origin.y += IPHONE_HD_OFFSET_Y / deviceScale;

	return rect;
}

bool MapObject::ccTouchBegan(CCTouch* touch, CCEvent* event)
{	
	if (canDealWithTouch(touch))
	{
		if(!m_playing)
		{
			m_playing = true;		
			m_anim->SetAnim(m_curAnimID + 1, 1, false);
			m_anim->setEndActionWithTarget(this, callfuncND_selector(MapObject::resumeDefaultAnim));
		}

		return true;
	}
  
    return false;    
}


