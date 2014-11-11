#ifndef SPRITE_BASE_H
#define SPRITE_BASE_H


#include "cocos2d.h"
#include "UserData.h"

class PLAYER_ID;
class AnimSprite;

//enum PointPosType
//{
//	POS_HEAD = 0,
//	POS_BODY
//};

//////////////////////////////////////////////////////////////////////////
//the base class for all level object
//////////////////////////////////////////////////////////////////////////
class SpriteBase : public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{

public: 
	SpriteBase(int type);
	virtual ~SpriteBase();


	//position and rect
	virtual void				setPosition(cocos2d::CCPoint newPosition);
	virtual cocos2d::CCPoint	getPosition();

	virtual cocos2d::CCRect		getRect();

	virtual int					getType()	{return m_type;}

	/// id set/get
	virtual PLAYER_ID		GetID() {return m_ID;}
	virtual void    SetID(PLAYER_ID id) {m_ID = id;}

	virtual unsigned int		GetTypeId(){return m_TypeID;}

	virtual void				addChildToRoot(cocos2d::CCNode* pChild);

	// pointPosType
	// 0 头部
	// 1 身体
	virtual bool				getAttachPoint(int pointPosType,cocos2d::CCPoint &point);

	cocos2d::CCNode*			getRoot(void){return m_rootSprite;}

protected:

	//check touch event
	virtual bool				canDealWithTouch(cocos2d::CCTouch* touch);
	virtual bool				containsTouchLocation(cocos2d::CCTouch* touch);	
	virtual bool				isTouchSelf(cocos2d::CCPoint pt);

protected:

	//the type is based on the object tag at level
	int		m_type;

	/// id number
	PLAYER_ID  m_ID;

	/// type id
	unsigned int m_TypeID;

	//root
	cocos2d::CCNode*			m_rootSprite;

	//animation
	AnimSprite*					m_anim;
};


#endif