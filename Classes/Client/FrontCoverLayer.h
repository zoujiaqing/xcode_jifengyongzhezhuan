#ifndef FRONT_COVER_LAYER_H
#define FRONT_COVER_LAYER_H

#include "cocos2d.h"
#include "TXGUIHeader.h"
#include "Roleselect.h"
#include <vector>
using namespace std;
using namespace cocos2d;

class FrontCoverLayer :
	public cocos2d::CCLayer//, 
	//public ISocketEvent
{
public:
	FrontCoverLayer();
	virtual ~FrontCoverLayer();

	CREATE_FUNC(FrontCoverLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	virtual void setVisible(bool visible);

	void menuLoginGameCallback(CCObject* pSender);
    
	void menuLogOutCallback(CCObject* pSender);
	void OkQuitGameClick(CCObject* obj);
	void onClickedMoreRole(CCObject* pSender);

	void onClickedMoreSever(CCObject* pSender);

	void onCLickedSeverCloseBt(CCObject* pSender);
	
	void onClickedOneSever(CCObject* pSender);

	void onClickedLatestSever(CCObject* pSender);

	void onClickedRecommendServer(CCObject* pSender);

	void onClickedReturnToLogin(CCObject* pSender);

	void onClickedNoticeBt(CCObject* pSender);

	void  setRoleselect(Roleselect * val){ m_Roleselect = val;}

	void ReadyToLogin(); // 连接无线平台成功,准备连接后台服务器
    
	void onReceiveSeverInfo();
	void onReceivedRoleList(CSGetRoleListRsp* msg);
	
    
protected:
	virtual void keyBackClicked();//监听Android 返回键
	virtual void keyMenuClicked();//监听Android 菜单键

	void onReceivedServerPageChanged(); //监听服务器页面翻页

	void OkBtnClick(CCObject* obj);
	void CancelBtnClick(CCObject* obj);	
	void onLogoutConformBtClicked(CCObject* obj);
	void onSelectSever();
	void enterRoleselect();
    void showmsg(const char* msg);
private:

	void addNewSever();
	void clearSeverList();
	void setLastLoginSever(int status = 0);
	void setLastServerLabel(const char * name);
	void setRecommendServerLabel(const char* name,int server,int status = 0);
	CCSprite* getServerStatusSprite(int status);
	CCPoint getStatusSpritePosition();
	CCPoint getRoleSpritePosition();
	CCPoint getRoleHeadSpritePosition(int count ,int index);
	CCSprite *getRoleHeadSprite(int playerType);
	//unsigned int getLastRole(unsigned int sever_id);
	bool checkHasRoleList(unsigned int serverID,bool checkLastRole = true);
	void checkAndGetRoleByServer(unsigned int serverID);
	void checkNeedReconnect(unsigned int serverID);
private:
	CCMenuItemImage *pLoginItem;
	CCMenuItemImage *logOutItem;
    
    //CCLabelTTF * _downloadTTF;
    //CCProgressTimer* _downloadBar;
    //CCSprite* _barFrame;
    CCLabelTTF*	m_visionLabel;

    double m_accumulator;
    double m_lastTickTime;

	UIScrollPage* m_severScrollPage;
	std::vector<IconButton*> m_lstButton;
	UILayout* m_layout;

	bool m_bserverSelect;
	bool m_bLogin;
	CCPoint m_statusPoint;
	CCPoint m_rolePoint;
	string m_recommendServerName;
	string m_LastServerName;
	Roleselect * m_Roleselect ;
	unsigned int m_lastServerStat;
	bool m_bLatestServerFlag;
	bool m_bCheckLatestServer;
	int  m_server_req_type ;
	
};
//class CSGetRoleListReq {
//	 uint32 uid        = 1; // 登录米米号
//	 uint32 channel_id = 2; // 渠道id
//	 uint32 server_id  = 3; // 服务器id
//}
//message CSGetRoleListRsp {
//	message Roles {
//		required uint32 uid         = 1; // 登录米米号
//		required uint32 reg_time    = 2; // 角色注册时间
//		required uint32 channel_id  = 3; // 渠道id
//		required uint32 server_id   = 4; // 服务器id
//	}
//	repeated Roles                  roles = 1;  // 角色列表
//}
#endif