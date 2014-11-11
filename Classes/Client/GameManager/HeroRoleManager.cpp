#include "HeroRoleManager.h"
#include "HeroFighter.h"

#include "XLogicManager.h"
#include "GameManager.h"
#include "LevelLayer.h"
#include "UserData.h"

HeroRoleManager::HeroRoleManager()
{
	
}

HeroRoleManager::~HeroRoleManager()
{

}

RoleBase* HeroRoleManager::createRole(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet, bool isCreateEffect )
{
	RoleBase* hero = 0;

	if(UserData::Get()->getUserId() == s_id)
	{
		hero = new HeroFight(s_id, name, type, isPlayerSet, isCreateEffect);
	}
	else
	{
		hero = new OtherHeroFight(s_id, name, type, isPlayerSet, isCreateEffect);
	}
	hero->autorelease();	

	m_lstRoles.push_back(hero);
	return hero;
}

void HeroRoleManager::RemoveRole(RoleBase* role)
{
	if (NULL != role)
	{
		// 其它玩家也是用这个类，如果其它玩家退出的话，那么主角不是不存在了？所以不能置为NULL
		if(role->getTag() == MAP_TAG_SEER)
		{
			GameManager::Get()->setHero(NULL);
		}

		m_lstRoles.remove(role);
	}
}