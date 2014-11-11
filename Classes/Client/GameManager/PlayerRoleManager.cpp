#include "PlayerRoleManager.h"

PlayerRoleManager::PlayerRoleManager()
{
	mMap_UserRole.clear();
}

PlayerRoleManager::~PlayerRoleManager()
{
	clearRoleList();
}

void PlayerRoleManager::clearRoleList()
{
	map<int ,vector<UserRole*>* > & pMap_UserRole = getMapUserRole();
    {
        vector<UserRole *> * pvec_UserRole = NULL ;
        map<int ,vector<UserRole*>* >::iterator miter = pMap_UserRole.begin();
        for(;miter!= pMap_UserRole.end();miter++)
        {
            pvec_UserRole = miter->second;
            vector<UserRole *>::iterator iter = pvec_UserRole->begin();
            for (; iter != pvec_UserRole->end(); iter++) {
                CC_SAFE_DELETE(*iter);
            }
            CC_SAFE_DELETE(pvec_UserRole);

        }

    }
    pMap_UserRole.clear();
	mMap_RoleReqTime.clear();
}

void PlayerRoleManager::onReceivedRoleList(CSGetRoleListRsp* msg)
{
	map<int ,vector<UserRole*>* > & pMap_UserRole = getMapUserRole();

	/*int count = msg->servers_size();
	for(int i=0;i < count;i++)
	{
		unsigned int serverId = msg->servers(i);
		map<int ,vector<UserRole*>* >::iterator  miter = pMap_UserRole.find(serverId);
		if(miter == pMap_UserRole.end())
		{
			vector<UserRole *> *  pvec_UserRole = new vector<UserRole *>();
			pMap_UserRole.insert( make_pair(serverId,pvec_UserRole));
		}
	}*/

	int serverID = msg->server_id();
	map<int ,vector<UserRole*>* >::iterator  miter = pMap_UserRole.find(serverID);
	if(miter == pMap_UserRole.end())
	{
		vector<UserRole *> *  pvec_UserRole = new vector<UserRole *>();
		pMap_UserRole.insert( make_pair(serverID,pvec_UserRole));
	}
	else
	{
		vector<UserRole *> *  pvec_UserRole = miter->second;
		vector<UserRole *>::iterator iter = pvec_UserRole->begin();
        for (; iter != pvec_UserRole->end(); iter++) {
            CC_SAFE_DELETE(*iter);
        }
		pvec_UserRole->clear();
	}

	int count = msg->roles_size();
    // login
    int isloginRole = false ;
    USERINFO info = UserData::GetUserInfo();
	
	unsigned int maxLoginTime =  0 ;
    for(int i=0;i < count;i++)
    {
        CSGetRoleListRsp::Roles role = msg->roles(i);
        
        CCAssert(role.uid() == info.id.getID(),"receive error uid");
        CCAssert(role.channel_id() == info.id.getChannel(),"receive error channel_id");
        CCAssert(role.reg_time() != 0,"receive error reg_time, 0");
        UserRole * pUserRole = new UserRole();
        vector<UserRole *> * pvec_UserRoleUser = NULL ;
        //pUserRole->setRoleType(role.);
        map<int ,vector<UserRole*>* >::iterator  miter = pMap_UserRole.find(role.server_id());
		if(miter == pMap_UserRole.end())
		{
			vector<UserRole *> *  pvec_UserRole = new vector<UserRole *>();
			pMap_UserRole.insert( make_pair(role.server_id() ,pvec_UserRole));
			pvec_UserRoleUser = pvec_UserRole ;
		}
        else
        {
			pvec_UserRoleUser = miter->second ;
        }
        pUserRole->setRegTime(role.reg_time());
        pUserRole->setRoleType(role.role_type());
        pUserRole->setRoleLevel(role.lv());
        pUserRole->setRoleName(role.name());
        pUserRole->setLoginTime(role.last_login_tm());
        if(pvec_UserRoleUser->size() == 0)
           pvec_UserRoleUser->push_back(pUserRole);
        else
        {
            vector<UserRole*>::iterator  iter ;
            bool isInsert  = false ;
           for( iter = pvec_UserRoleUser->begin();iter !=pvec_UserRoleUser->end();++iter )
           {
              
              if((*iter)->getRegTime()>role.reg_time())
              {
                  pvec_UserRoleUser->insert(iter,pUserRole);
                  isInsert = true ;
                  break;
              }

           }
           if(!isInsert)
           {
               pvec_UserRoleUser->push_back(pUserRole);
           }
        }
       
       if(role.server_id() == info.serverId)
       {
          
           if(maxLoginTime <= role.last_login_tm())
           {
               maxLoginTime = role.last_login_tm();
               info.id.setRegTime(role.reg_time());
           }
           isloginRole = true ;
           
           
       }
       
       
    }
       if(!isloginRole)
       {
           info.id.setRegTime(0);
       
       }
       UserData::SetUserInfo(info);
}

