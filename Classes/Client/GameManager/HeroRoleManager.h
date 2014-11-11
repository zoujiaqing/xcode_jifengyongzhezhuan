#pragma once

#include "SASpriteDefine.h"
#include "RoleBase.h"
#include "Singleton_t.h"

#include <list>

using namespace std;

class HeroRoleManager : 
	public TSingleton<HeroRoleManager>
{
public:
	HeroRoleManager();
	virtual ~HeroRoleManager();
	
	RoleBase* createRole(PLAYER_ID s_id,const char *name, int type, bool isPlayerSet,bool isCreateEffect = true);

	//// called when RoleBase destroyed
	void	RemoveRole(RoleBase* role);

protected:
	list<RoleBase*> m_lstRoles;
};