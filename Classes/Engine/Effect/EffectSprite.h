#pragma once

#include "cocos2d.h"
#include "SASpriteDefine.h"
#include "HeroEffectDefine.h"
#include "RoleActorBase.h"
#include "BoneSpriteMoveable.h"
#include "map"

class RoleActorBase;

class EffectSprite : public BoneSpriteMoveable
{
public:
	EffectSprite(int Type);	
	virtual ~EffectSprite();

	bool isTouchSelf(cocos2d::CCPoint pt);
	CCSize getCurContentSize();
	CCSize getColliderRectSize();

	static EffectSprite* effectWithID(int s_id,const char *name);
	bool initWithId(int id,const char *name);

	void setPlayerNameLabelPos(CCPoint pos);
	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID);

	virtual void SetAnim(int animId, int nloop, bool isLoop = true,BoneSpriteBase *listerner = NULL);
    virtual void SetAnim(int animId, int nloop, bool isLoop,CCAnimationEventListener *listerner);
    
	void setPreType(RoleActionType type){this->preType = type;}
	RoleActionType getPreType(){return this->preType;}

private:
	CCLabelTTF *mEffectNameLabel;
	bool bStartAnim;

	//用于存储不同播放类型
	RoleActionType preType;
};