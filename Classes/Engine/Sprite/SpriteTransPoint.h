#ifndef  SPRITE_TRANSPOINT_H
#define  SPRITE_TRANSPOINT_H


#include "SpriteBase.h"
#include "EffectSprite.h"
using namespace cocos2d;

class EffectSprite;
//////////////////////////////////////////////////////////////////////////
//new Sprite transpoint
//////////////////////////////////////////////////////////////////////////
class SpriteTransPoint : public SpriteBase
{

public:

    SpriteTransPoint();
    virtual ~SpriteTransPoint();
    
    static SpriteTransPoint*				node(const char *name,cocos2d::CCPoint pos,int mapId);    
    
	//info
	int										getMapId();   
	void									setMapId(int id){this->m_mapId = id;}
    
	//overrides
	virtual bool							ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void							ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	//virtual cocos2d::CCRect		getRect();
	virtual bool				isTouchSelf(cocos2d::CCPoint pt);
	virtual cocos2d::CCRect		getRect();
	cocos2d::CCPoint			getPosition();
	/**
	* Instruction : 
	* @param 
	*/
	void	MoveToSelfPosTarget();	
public:
    
    SEL_CallFunc							m_fnChangeMap;
    const char * getName();
	void  setAppearence(bool show);
protected:

	//init
    bool									initWithName(const char *name,cocos2d::CCPoint pos,int mapId);    
    
private:

    cocos2d::CCPoint						m_pos;
	const char * 									m_name; //added by jackniu
    int										m_mapId;
	EffectSprite*							m_effect;

};

#endif // _SPRITE_TRANS_H_

