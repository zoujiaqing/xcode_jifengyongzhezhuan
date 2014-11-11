// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//

#pragma once

#include "ILogicManager.h"
#include "SceneLayer.h"
#include "errcode.pb.h"
#include "cs_battle.pb.h"
#include "../../NetWork/cs_basic.pb.h"
#include "cocos2d.h"

#include "TXGUIHeader.h"
#include "IEvent.h"


using namespace cocos2d;

//// 等待指定网络协议的回包�?
class WaittingForRsp
{
public:
	void Start();
	void Stop(); 
	
	void OnHandleMessage(const std::string& cmd);

	bool IsReceived(char* pCmdName);
protected:
	
	bool m_bStart;
	std::list<std::string> m_cmdList;

};

/*
* @XLogicManager, 	
@/// 逻辑面板的管理器，负责切换到不同的核心游戏模�? 这些模块以游戏步骤为依据进行切分
*
*/
class XLogicManager : 
	public CCNode, 
	public ILogicManager , 
	public TXGUI::ObserverProtocol,
    public ISocketEvent
{
public:
	/// 逻辑面板的枚举变量类�?
	typedef enum  
	{
		E_NOE          = 0,
		E_SELECT_ZONE  = 1,		//选择服务器分区功能面�?
		E_CREATE_PLAYER_LAYER ,	//创建玩家面板
		E_SELECT_CHARACTER ,    //角色选择界面步骤
		E_ENTER_MAINLAND,
		E_ENTER_FB,
		E_MAIN_GAME_LAYER ,		//开始游戏面�?
		E_GAME_HELP_LAYER ,		//帮助面板�?
		E_PVP_LAYER ,	//pvp战斗面板
		E_PVE_LAYER,	//pve副本面板
		E_TASK_PANEL_LAYER ,
		E_UPDATE_RESOURCE,
        E_Notice,
        E_FIRST_FIGHT,

		E_XLOGICMANAGER,

		// add more...

	}GAME_LOGIC_LAYER;

	typedef enum
	{
		E_WAITIN_FOR_COMMOND = 0, // 空闲状�?
		E_SENDING_SEVER = 1,	// 已经发送登�?进副�?回主城协议包去服务器，等待回包，不接收类似的操作
	}LOGIC_CHANGE_LAYER_STATUS;

	//MessageHandler status
	typedef enum
	{
		E_NONE,		//reset status
		E_BATTLE_CREATE,
		E_LOAD_COMPLETE,
	}MESSAGE_STATUS;

	typedef void (*errFun)(db_errcode_t); 

	struct FB_DATA
	{
		unsigned int mapID;
		unsigned int battleID;
	};
public:
	XLogicManager();
	virtual ~XLogicManager();

	//void  updateFps(float dt);

	void  WillEnterForeground();

	void  DidEnterBackground();

		
	/*
	* @prototype: startGame()
	* @note:
	*     start the game now, bring the player to the hometown.
	* @param: void
	* @return: void
	*/
	virtual void startGame();
	virtual void exitGame();

	// static instance
	static XLogicManager* sharedManager();
    static void pureXLogicManager();
	// functions
	// go to a specific scene
	void  goToLayer(GAME_LOGIC_LAYER sceneTag, void *userData = NULL); 

	void connectToServer();

	void setStartConectint();
    
    void backToLogin();

	void onConnectToSever();

	int getLoginStep();

	void logIn();

	void logInError(db_errcode_t error);

	void logOut();

	void networkDisConnected();
	void notifyShowNextAppearUI();
	void fromGameBackToLogin();

	void registerNetErrFun(errFun fun) { pNetErrFun = fun; }

	void unRegisterNetErrFun() {  pNetErrFun = NULL;  }

	void createRole(const std::string nick, const unsigned int type);

	void backToTown();

	void LeaveBattle();

	void FinishBattle();

	void prepareBattle(int instanceId, int mapId);
	
	void createBattle(int battleId, int mapId);
	
	void changeMap(int mapId, KMapFromType fromMapType);

	void receivedItemsInfo();

	// ”Œœ∑∆Ù∂Ø ±œÚ∑˛ŒÒ∆˜«Î«Û±ÿ“™µƒ–≈œ¢
	//void initUserInfoFromSever(float time);

	db_errcode_t getNetErrorCode();

	void reqPlayerAttributes(int attributeId);

	void reqPlayerAttributes(int attributes[],int length);

	void reqAllPlayerAttributes();

	bool isAlreadyLogin();
    void setisAlreadyLogin(bool flag) {b_isLoginGame = flag;}
	void loginSuccess();

	cocos2d::CCNode* GetCurLayer(void){return m_pCurLayer;}
	GAME_LOGIC_LAYER GetCurLayerTag(void){return m_eCurLayer;}

	void	setNeedShowPPVEAfterBacktoTown(bool bShowPPve);

	void	checkNeedShowPPVEPanel();

	// 从服务器收到的回包。用来确定指定的协议是否已经收到，用来加载时的流程控制�?
	void OnHandleMessage(const std::string& cmd);

	WaittingForRsp& GetWaittingForRsp(void){return m_waittForRsp;}
	bool getAutoLogin(){return m_autoLogin;}
	void setAutoLogin(bool val){m_autoLogin = val ;}
	// receive user's role list. auto to select first one to login. if empty ,create one.
	void onReceivedRoleList(CSGetRoleListRsp* msg);
	void entryCreateRole();
	void entryLoginRole();

	void onCreateRoleSuceess();

	void onReceiveSession();

	void sendUserDeviceInfo();

	void sendPlatformLoginMessage(const char* sess,int platformId,int serverId,int userId,const char* appID,
			const char* appKey,const char* cpId,const char* channelId,const char* extraId,
			unsigned int gameId);

	void onPlatformCallBack(int error);

	void setMessageStatus(MESSAGE_STATUS msgStatus);
	MESSAGE_STATUS getMessageStatus(){ return m_msgStatus; }

	void setLoginStatus(int step);
    
    void startXXX();
    void updateChangeScene(float f);

    void setHasStarReward(bool flag);

    cocos2d::CCNode* getpCurLayer() { return m_pCurLayer; }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void sendStatAtLaunch();
#endif
    bool isIOSDisconnect;
    bool isIOSInScene;
    bool isChangeScene;
    bool isIOSNeedWaitingLayer;
    std::string accountName;
    bool hasStarReward;
protected:
	void getConfigInfo(float f);
	/// on messagebox network error!
	void OnNetworkMessageOK(CCObject* pObject);

	void OnConnectedNetFrame(float dt);

	void OnBuyKeyComfirmClicked(CCObject* pObject);

	/// 消息响应函数
	virtual void onBroadcastMessage(BroadcastMessage* msg);
    
    /// override ISocketEvent
	virtual void OnSocketError(int nError);
	virtual void OnConnected(int nResult);
	virtual void OnDisconnect();

	void goToFb(unsigned int mapId, unsigned int battleId);

	// clear user data while back to login scene
	void clearUserData();
protected:
    
	GAME_LOGIC_LAYER  m_eCurLayer;
	LOGIC_CHANGE_LAYER_STATUS  m_eCurStatus;
	MESSAGE_STATUS m_msgStatus;
	int m_loginStep;
	bool m_autoLogin ;
	errFun pNetErrFun;
	db_errcode_t mErrorCode;
	bool isGameStart;
	bool isRequiredItemsInfo;
	float	m_loginDataTime;
	bool	isInitLoginData;
	bool	b_isLoginGame;
	//// time ticked when application move to background
	long long   m_LastEnterBackgroundTime; 

	cocos2d::CCNode* m_pCurLayer;


	// TODO : remove
	/// ppve panel show flag
	bool	m_bShowPPVEPanel;

	WaittingForRsp m_waittForRsp;

};

extern "C"{

	extern void playerlogOut();
}