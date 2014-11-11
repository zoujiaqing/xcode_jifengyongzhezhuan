#ifndef ROLESELECT_H
#define ROLESELECT_H

#include "cocos2d.h"
#include "TXGUIHeader.h"
#include "cs_basic.pb.h"
#include <vector>
#include <map>
using namespace std;
using namespace cocos2d;
class FrontCoverLayer ;
class UserRole
{
    public:
	UserRole()
		  :roleType(1)
		  ,roleName("222")
		  ,reg_time(0)
		  ,login_time(0)
		  ,roleLevel(3){}
	     
	virtual ~UserRole(){}
	inline void setRoleType(int val){roleType = val ; }
	inline void setRoleName(string val){roleName = val ; }
	inline void setRoleLevel(int val){roleLevel = val ; }
	inline void setRegTime(unsigned int val){reg_time = val ; }
	inline void setLoginTime(unsigned int val){login_time = val ; }
	int  getRoleType(){return roleType ;}
	string  getRoleName(){return roleName ;}
	int  getRoleLevel(){return roleLevel ;}
	unsigned int  getRegTime(){return reg_time ;}
	unsigned int  getLoginTime(){return login_time ;}
	private:
	int roleType ;
	string roleName ;
	int roleLevel ;
	unsigned int reg_time ;
	unsigned int login_time ;

};
class Roleselect :
	public cocos2d::CCLayer//, 
	//public ISocketEvent
{
public:
	Roleselect();
	virtual ~Roleselect();

	CREATE_FUNC(Roleselect);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	virtual void setVisible(bool visible);

	int getIndex(){return index ;}



	void update(float dt);
		
static	 void setFrontCoverLayer (FrontCoverLayer * vptr ){pFrontCoverLayer = vptr ;}
    
protected:
	void OnSelectRole(CCObject* pSender);
	void OnClose(CCObject* pSender);
	void OnConfirm(CCObject* pSender);
	
private:

	
private:
	int index  ;
	int severIndex ;
	UILayout* m_layout;
	CCMenuItem* m_MenuItem ;
	map<int ,vector<UserRole*>* >  mMap_UserRole ;
	vector<UserRole *> *	pvec_UserRoleUser  ;
	static FrontCoverLayer * pFrontCoverLayer ;

};

#endif