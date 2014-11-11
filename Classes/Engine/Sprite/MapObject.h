#ifndef ISEER_MAPOBJECT_H
#define ISEER_MAPOBJECT_H


#include "SpriteBase.h"

//////////////////////////////////////////////////////////////////////////
//new map object
//////////////////////////////////////////////////////////////////////////
class MapObject : public SpriteBase
{
public:

    MapObject();
    MapObject( const char* spriteName, int objectId, int animIndex);
    virtual ~MapObject();
    

protected:

	//init
    void				initObjectWithId( const char* spriteName, int objectId, int animIndex);
    bool				ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);	


	//overrides
	virtual cocos2d::CCRect getRect();


	//actions
    void				resumeDefaultAnim(cocos2d::CCNode *sender, void *data);

	
  

public:

    CC_SYNTHESIZE(int, m_objectID, ObjectId);
    CC_SYNTHESIZE(int, m_curAnimID, CurAnimId);
    CC_SYNTHESIZE(bool, m_playing, Isplaying); 

};

#endif
