#include "RoleBase.h"
#include "SkeletonAnimRcsManager.h"
#include "HeroRoleManager.h"


RoleBase::RoleBase() : SpriteSeer(0, true)
{
	init();
}

RoleBase::RoleBase(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet,bool isCreateEffect) :
	SpriteSeer(type, isPlayerSet)
{
	/// init base type
	SpriteSeer::initWithId(s_id, type, name, isCreateEffect);

	/// init subclass
	init();
}

void RoleBase::init()
{
	//// to do

}

RoleBase::~RoleBase()
{
	clearAllData();

	/// remove myself from the container
	HeroRoleManager::Get()->RemoveRole(this);
}

void RoleBase::clearAllData()
{

}