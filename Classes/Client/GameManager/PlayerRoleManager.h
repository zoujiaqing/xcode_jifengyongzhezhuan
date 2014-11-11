#pragma once
#include "Singleton_t.h"
#include "cocos2d.h"
#include "Roleselect.h"
#include <vector>

USING_NS_CC;

class PlayerRoleManager:public TSingleton<PlayerRoleManager>
{
public:
	PlayerRoleManager();
	~PlayerRoleManager();
	void clearRoleList();
	void onReceivedRoleList(CSGetRoleListRsp* msg);
	map<int ,vector<UserRole*>* >&  getMapUserRole(){return mMap_UserRole ;}
	map<int ,long>&  getRoleReqTime(){return mMap_RoleReqTime ;}
private:
	map<int ,vector<UserRole*>* >  mMap_UserRole;
	map<int ,long> mMap_RoleReqTime; // 群拉服务器角色列表的CD
};