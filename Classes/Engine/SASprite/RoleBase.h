#pragma once

#include "SASpriteDefine.h"
#include "cocos2d.h"
#include "SpriteSeer.h"
USING_NS_CC;

class RoleActorBase;



class RoleBase : public SpriteSeer
{
public:
	RoleBase(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet,bool isCreateEffect = true);
	virtual ~RoleBase(void);

public:
	virtual void init();
	void clearAllData();

protected:
	/// prototype
	RoleBase();

protected:
	/// property

};