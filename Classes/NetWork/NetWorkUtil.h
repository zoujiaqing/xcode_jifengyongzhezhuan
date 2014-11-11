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
#ifndef NETWORK_UTIL_H
#define NETWORK_UTIL_H


#include "byte_array_t.h"
#include "message.h"
#include "CSocketUtil.h"
#include "MessageUtil.h"
#include "MessageCache.h"

// this line should be the last when try to include the <winsock2.h>
#include "cocos2d.h"

#include "IEvent.h"
#include "NotificationCenter.h"

using namespace google::protobuf;
using namespace cocos2d;

class MessageCache;

//////////////////////////////////////////////////////////////////////////
typedef struct
{	
	float			timeout;
	int				tag;

	std::string		cmd;

}Timeout_t;

//////////////////////////////////////////////////////////////////////////
typedef  enum
{
    KSocketNone  = -1,
	KSocketDisconnect	= 0	,
	KSocketTimeout		,
	KSocketConnecting	,
    KSocketCancel

}KSocketStatus;

//////////////////////////////////////////////////////////////////////////
typedef enum
{	
	ES_ERR_CONNECT		= 0	,
	ES_ERR_SEND			,
	ES_ERR_RECIVE		,		

}KSocketError;

//////////////////////////////////////////////////////////////////////////

typedef enum
{
	KResultSuccess				= 0,
	KResultSocketError			,
	KResultEncodeBufferError	,
	KResultCannotSendNow		,
	KResultWaiting

}KSendMessageResult;


//------------------------------------------------------------------
//				tags
//------------------------------------------------------------------
enum NetWorkTags
{
	NETWORK_TAG_CONNECT				= 0,
	NETWORK_TAG_PLAYER_ENTER_MAP	,
	NETWORK_TAG_PLAYER_LEAVE_MAP	,
	NETWORK_TAG_PLAYER_WALK			,
	NETWORK_TAG_PLAYER_BATTLE_MOVE	,
	NETWORK_TAG_GET_OTHER_PLAYERS	,
	NETWORK_TAG_GET_PLAYER_ITEMS	,
	NETWORK_TAG_USE_NORMAL_ITEM		,
	NETWORK_TAG_USE_MONSTER_ITEM	,
	NETWORK_TAG_DROP_ITEM			,
	NETWORK_TAG_PLAYER_CHANGE_NICK	,
	NETWORK_TAG_SEND_MAIL			,
	NETWORK_TAG_GET_MAIL_HEAD_LIST	,
	NETWORK_TAG_GET_MAIL_BODY		,
	NETWORK_TAG_DELETE_MAIL			,
	NETWORK_TAG_GET_MAIL_ENCLOSURE	,
	NETWORK_TAG_PLAYER_ATTACK		,
	NETWORK_TAG_SKILL_BEGIN			,

	// quest
	NETWORK_TAG_OBTAIN_TASK			,
	NETWORK_TAG_CANCEL_TASK			,
	NETWORK_TAG_FINISH_TASK			,
	NETWORK_TAG_GET_TASK_FLAG_LIST	,
	NETWORK_TAG_GET_TASK_BUFF_LIST	,
	NETWORK_TAG_GET_TASK_LIST		,
	NETWORK_TAG_SET_TASK_STEP		,
	NETWORK_TAG_GET_BATTLE_LIST     ,

	NETWORK_TAG_ATTRIBUTESREQ		,
	NETWORK_TAG_USERINFO			,
	// login
	NETWORK_TAG_CHECK_SESSION		,
	NETWORK_TAG_CREATE_ROLE			,
	NETWORK_TAG_LOGIN				,
	NETWORK_TAG_LOGINOUT			,
	NETWORK_TAG_LOAD_COMPLETE       ,
	NETWORK_TAG_QUERY_REQ			,
	NETWORK_TAG_RED_NAME_REMAIN		,

	NETWORK_TAG_PLAYER_LEAVE_BATTLE ,

	NETWORK_TAG_BATTLE_CREATE       ,
	NETWORK_TAG_START_PVE_BATTLE	,
	NETWORK_TAG_BATTLE_FINISH_LOADING	,
	NETWORK_TAG_BATTLE_USE_SKILL	,
    NETWORK_TAG_BATTLE_SWITCH_SPRITE,
    NETWORK_TAG_BATTLE_RUNAWAY,
	BATTLE_TAG_CONNECT,

	NETWORK_TAG_ITEM_REQ	,
	NETWORK_TAG_ITEM_MOVE	,
	NETWORK_TAG_ITEM_REORDER	,
	NETWORK_TAG_ITEM_REMOVE	,
	NETWORK_TAG_ITEM_USE	,
	NETWORK_TAG_TASK,
	NETWORK_TAG_TASK_INFO,
	NETWORK_TAG_EQUIPUPGRADE_REQ	,
	NETWORK_TAG_EQUIPUPGRADE_STRENGTH	,
	NETWORK_TAG_EQUIPCOMPOSE_REQ	,
	NETWORK_TAG_EQUIPINLAYGEM_REQ	,
	NETWORK_TAG_GEMRECAST_REQ	,
	//	diamond
	NETWORK_TAG_EXCHANGEPARAM_REQ	,
	// sprite stone
	NETWORK_TAG_SEARCH_SPRITE_STONE	,
	NETWORK_TAG_MOVE_SPRITE_STONE_TO_STORAGE,
	NETWORK_TAG_UPGRADE_SPRITE_STONE,
	NETWORK_TAG_TRANS_SPRITE_TO_ENERGE,
	NETWORK_TAG_SPRITE_EXCHANGE,
	NETWORK_TAG_ELF,
	NETWORK_TAG_SET,
	NETWORK_TAG_SKILL_UI,
	ELF_TAG_AWAKE,
	ELF_TAG_EXPLORE,
	DEL_HERO_PLAYER,

	PVAI_TAG_REQ,
	PVAI_TAG_INFO_REQ,
	//get the times user has played for a jing ying fu ben in one day.
	JING_YING_FU_BEN_DAILY_ONCE, 
	LUA_MSG,//send from lua
	// test echo message
	BATTLE_TAG_ECHO,   // for test     
	DAILY_TASK_TAG,

	//local server
	REMOTE_MONSTER_DIE,
	REMOTE_PLAYER_HP,
	PREPARE_BATTLE,
	NETWORK_TAG_PLAYER_HP,
	PVPLERN ,
	PVPACKLERN ,

    NETWORK_TAG_EXTRACT_SPRITE,
    NETWORK_TAG_PLAYER_RANK,
    NETWORK_TAG_RANK_INFO,

    NETWORK_TAG_GET_TOPUP_REWARD,

    NETWORK_TAG_GET_SHOP_TIMES,
    NETWORK_TAG_STAR_REWARD,

    NETWORK_TAG_GET_DAILY_REWARD,
    NETWORK_TAG_EXPLORE_TREASUER_INFO,
	NETWORK_TAG_EXPLORE_TREASUER,
    NETWORK_TAG_GET_TREASURE_REWARD,

    NETWORK_TAG_GET_EXCHANGE_PARAMETERS,

    NET_TAG_GET_GUILD_LIST,
    NET_TAG_SEARCH_GUILD,
    NET_TAG_JOIN_GUILD,
    NET_TAG_CREATE_GUILD,
    NET_TAG_GET_GUILD_INFO,
    NET_TAG_SET_GUILD_NOTICE,
    NET_TAG_GET_GUILD_MEMBER,
    NET_TAG_CHANGE_GUILD_MEMBER_STATUS,
    NET_TAG_QUIT_GUILD,
    NET_TAG_Drop_GUILD,
    NET_TAG_GUILD_SHOP_ITEMS,
    NET_TAG_GUILD_BLESS,
    NET_TAG_GUILD_CONTRIBUTE_INFO,
    NET_TAG_GUILD_CONTRIBUTE_BUTTON,
    NET_TAG_GUILD_APPLAY_SETTING,
	NET_TAG_GUILD_RECORD_INFO,
	NET_TAG_GUILD_UNION_INFO,
	NET_TAG_GUILD_CHOOSE_INFO,
	NET_TAG_GUILD_CREATE_INSTANCE,
	NET_TAG_GUILD_RED_ENVO_INFO,
	NET_TAG_GUILD_SEND_RED_ENVO,
	NET_TAG_GUILD_SECOND_SEND_RED_ENVO,
	NET_TAG_GUILD_GET_RED_ENVO,
	NET_TAG_GUILD_GET_RED_ENVO_RECORD,
	NET_TAG_GUILD_GET_RED_ENVO_CONFIG
};


//////////////////////////////////////////////////////////////////////////
typedef struct
{
	char ip[16];
	int port;
	float timeOut;

}SocketInfo_t;




//////////////////////////////////////////////////////////////////////////
//NetWorkUtil
//////////////////////////////////////////////////////////////////////////
class NetWorkUtil : public CCObject , public EventProducer<ISocketEvent>
#ifdef USE_BOOST_ASIO
	, public CSocketUtilDelegate
#endif
	, public TXGUI::ObserverProtocol
{
	friend class MessageFilter;
	friend class MessageCache;

public:
	
	//------------------------------------------------------------------------
	//					connect / disconnect
	//------------------------------------------------------------------------
	virtual void						connect2Server(const char *ip,const int port,float timeout,int tag);
	
	//------------------------------------------------------------------------
	//			@isFailedCache flag determine whether to cache message
	//			if message send failed
	//			@clean up flag determine whether memory delegate
	//			if cleanup be false, isFailedCache will make no sense
	//------------------------------------------------------------------------
	KSendMessageResult					sendMessage(Message* bodyMsg,
		float timeout, 
		int tag,
		bool isFailedCache,
		bool cleanUp = true);
	//true for sucess , false failed
	bool  sendMessage(
		char *msgName,
		void *bodyBuff,
		int bodylen,
        bool isFailedCache);


	KSendMessageResult					sendMessage(MessageHeader* headerMsg,
		Message* bodyMsg, 
		float timeout,
		int tag,
		bool isFailedCache,
		bool cleanUp = true);	



	MessageHeader*	encodeMessageHeader(unsigned int userid, int userRoleTm);
	
	virtual void						cleanServerConnect(bool needReconnect);
	
	//------------------------------------------------------------------------
	//					register time out handler
	//------------------------------------------------------------------------
	//void								registerTimeOutEventHandler(CCObject* handler, SEL_CallFuncND callBackfunc);


	//------------------------------------------------------------------------
	//					socket status
	//------------------------------------------------------------------------
	inline  KSocketStatus				getSocketStatus() { return m_bScoketStatus;}


#ifdef USE_BOOST_ASIO
	// 从服务器收到回报的回调。
	virtual void DidReadCompleted(unsigned int uBytes, CSocketUtilDelegate::ERROR_CODE error);
	// 发送数据到服务器结束的回调。
	virtual void DidWriteCompleted(unsigned int uBytes, CSocketUtilDelegate::ERROR_CODE error);

	virtual void DidConnected(CSocketUtilDelegate::ERROR_CODE error);
#endif

	void onBroadcastMessage(TXGUI::BroadcastMessage* msg);

protected:

	NetWorkUtil();
	virtual ~NetWorkUtil();

	void								update(float dt);

	//------------------------------------------------------------------------
	//				filter messages
	//------------------------------------------------------------------------
	void								filter();
	
	//------------------------------------------------------------------------
	//				dispatch message
	//------------------------------------------------------------------------
	void								dispatchMessage(char *headBuff,int headlen, char *bodyBuff,int bodylen,int tag);

	//------------------------------------------------------------------------
	//				@headerMsg & bodyMsg is input message content
	//				@headerMsg & bodyMsg must not be null
	//				@outBuffer is for out
	//------------------------------------------------------------------------
	bool								encodeMessage(MessageHeader* headerMsg, Message* bodyMsg, byte_array_t& outBuffer);
	bool								encodeCRCMessage(MessageHeader* headerMsg, Message* bodyMsg, byte_array_t& outBuffer);

	//------------------------------------------------------------------------
	//				@inputBuffer is for in	
	//				@bufferLen is the length of the inputBuffer
	//				@totalLen is the total length for cur message
	//------------------------------------------------------------------------
	bool								decodeMessage(char* inputBuffer, 
		int bufferLen, 
		char*& headerBuffer,
		int& headerLen,
		char*& bodyBuffer, 
		int& bodyLen, 
		int& totalLen);

	//------------------------------------------------------------------------
	//				event interfaces
	//------------------------------------------------------------------------
	//virtual void						onTimeOut(std::string cmd,int tag);
	virtual void						onConnectResult(KSocketStatus status, int tag);

	// 真正的断线
	virtual void						onDisconnect(void);

	void								resetTimeOut();


	virtual void						reconnect2Server(void);
    virtual void						connect(const char *ip,const int port,float timeout,int tag);
    

	KSendMessageResult	sendMessage(MessageHeader* headerMsg, Message* bodyMsg, float timeout, int tag);

	KSendMessageResult	HandleLocalMessage(MessageHeader* headerMsg, Message* bodyMsg, float timeout, int tag);

private:
		
	//
	static const int					KRecvQueueLen = 1024*1024*2;
	int									m_bRecvQueueLen;
	char								m_bRecvQueue[KRecvQueueLen];

	//
	CSocketUtil		m_bSocket;		// 用的是异步的方式。且只能一个一个包的来收和发。
	KSocketStatus	m_bScoketStatus;
	SocketInfo_t	m_bSocketInfo;
	Timeout_t		m_timeOut;


	//
	bool			m_canSend;

	//
	//CCObject*					m_timeOutEventHandler;
	//SEL_CallFuncND				m_timeOutCallBackFun;
	MessageCache* m_pMessageCache;

#ifdef USE_BOOST_ASIO
	CacheMessageInfo m_curWritingMsgInfo;
#endif

	unsigned int m_maxReconnectCount; 
	unsigned int m_curReconnectCount;
};



#endif
