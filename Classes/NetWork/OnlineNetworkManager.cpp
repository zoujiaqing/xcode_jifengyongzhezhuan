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
#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "UserData.h"
#include "cs_basic.pb.h"
#include "cs_core.pb.h"
#include "cs_battle.pb.h"
#include "LevelManager.h"

#include "PPVEMessage.h"
#include "PVPMessage.h"
#include "NetStateChecker.h"
#include "MessageFilter.h"
#include "UserData.h"
#include "LoginManager.h"
#include "LuaTinkerManager.h"
#include "MainMenuLayer.h"
#include "MainLandManager.h"
#include "WaitingLayerManager.h"
#include "UserData.h"
#include "XLogicManager.h"

OnlineNetworkManager* OnlineNetworkManager::sInstance = NULL;

#ifdef WIN32
	const float PKG_TIMEOUT = 300.0f;
#else
	const float PKG_TIMEOUT = 20.0f;
#endif

	const float CONNECT_TIMEOUT = 180.0f; //change from 5 to 180 for network delay

OnlineNetworkManager* OnlineNetworkManager::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new OnlineNetworkManager();
	}

	return sInstance;
}

void OnlineNetworkManager::Destroy()
{
	if (sInstance )
	{
		delete sInstance;
		sInstance = NULL;
	}
}



OnlineNetworkManager::OnlineNetworkManager() : 
    m_updateHpAmount(0)
{
	m_serverIP = NetWorkConfigInfoFromServer::sharedInstance()->GetGetWayIP();
	m_serverPort = NetWorkConfigInfoFromServer::sharedInstance()->GetGetWayPort();


	/// init ppve message handler
	PPVEMessage::Create();
    PVPMessage::Create();

	MessageFilter::Create();

    // 创建一些发送消息队列
    CreateUpdateHpQueue();
}

OnlineNetworkManager::~OnlineNetworkManager()
{
	/// destroy message filter
	MessageFilter::Destroy();

	/// uninit ppve message handler
	PPVEMessage::Destroy();
    PVPMessage::Destroy();

	NetStateChecker::Destroy();

}

//------------------------------------------------------------------
//				events
//------------------------------------------------------------------
void OnlineNetworkManager::LostConnection2Server(const char * msg)
{
	CCLOG("LostConnection2Server : %s", msg);
	ShowMessage(msg);
	this->onConnectResult(KSocketTimeout, 0);
}

void OnlineNetworkManager::setServerIP(const uint32_t serverIP)
{
	in_addr tmpInAddr;
	tmpInAddr.s_addr = serverIP;
	m_serverIP = inet_ntoa(tmpInAddr);
}

void OnlineNetworkManager::setServerIP(const char *serverIP)
{
	m_serverIP = serverIP;
}

void OnlineNetworkManager::setServerPort(const uint32_t serverPort)
{
	m_serverPort = serverPort;
}

//------------------------------------------------------------------
//				gameLogic
//------------------------------------------------------------------
void OnlineNetworkManager::startConnect()
{
	CCLOG("server IP: %s", m_serverIP.c_str());
	CCLOG("server port: %u", m_serverPort);
	if(m_serverIP=="0"||m_serverPort==0){
		
		ShowMessage("server ip is 0");
		return;
	}
	NetWorkUtil::connect2Server(m_serverIP.c_str(), m_serverPort, CONNECT_TIMEOUT, NETWORK_TAG_CONNECT);	
	//WaitingLayerManager::Get()->PushOneMessage("ConnectReq");
}

bool OnlineNetworkManager::startReconnect(std::string ip, uint32_t port)
{
    if (m_serverIP == ip &&
        m_serverPort == port)
    {
        return false;
    }

    setServerIP(ip.c_str());
    setServerPort(port);

	NetWorkUtil::connect2Server(m_serverIP.c_str(), m_serverPort, CONNECT_TIMEOUT, NETWORK_TAG_CONNECT);	
	WaitingLayerManager::Get()->PushOneMessage("ConnectReq");
	return true;
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendBattleCreateMessage(int battleId, int mapId)
{
	CCLOG("sendBattleCreateMessage()");
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattleCreateReq");
	CSBattleCreateReq* bodyMsg = (CSBattleCreateReq*)msg;
	bodyMsg->set_battle_id(battleId);
	bodyMsg->set_map_id(mapId);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_BATTLE_CREATE, true, true);
}
void OnlineNetworkManager::sendLoginOutMessage()
{
	CCLOG("sendLoginOutMessage()");
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSLoginOutReq");
	CSLoginOutReq* bodyMsg = (CSLoginOutReq*)msg;
	
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGINOUT, true, true);
}
//------------------------------------------------------------------
void OnlineNetworkManager::sendLoginMessage()
{
	CCLOG("sendLoginMessage()");
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSLoginReq");
	CSLoginReq* bodyMsg = (CSLoginReq*)msg;
	/*char cSession[SESSION_LENGTH];
	memcpy(cSession, UserData::getUserSession(), SESSION_LENGTH);
	bodyMsg->set_sid(cSession, SESSION_LENGTH);*/
	bodyMsg->set_uid(UserData::getUserId().getID());
	bodyMsg->set_channel_id(UserData::getUserId().getChannel());
	bodyMsg->set_reg_time(UserData::getUserId().getRegTime());
	USERINFO info = UserData::GetUserInfo();
	bodyMsg->set_server_id(info.serverId);
	//bodyMsg->set_game_id(LoginManager::Get()->getGameID());
	//bodyMsg->set_tokenid(UserData::getTokenId());
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGIN, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSLoginReq");
}

void OnlineNetworkManager::sendPlatformLoginMessage(const char* sess,int platformId,int serverId,int userId,const char* appID,
	const char* appKey,const char* cpId,const char* channelId,const char* extraId,unsigned int gameId,const char* deviceID,
	const char* platform,const char* macAddress,const char* eqtype,const char* svnNumber)
{
	/*CCLOG("CSCheckSessionReq: ");
	CCLOG("sess: %s",sess);
	CCLOG("platformId: %d",platformId);
	CCLOG("userId: %d",userId);
	CCLOG("serverId: %d",serverId);
	CCLOG("appID: %s",appID);
	CCLOG("appKey: %s",appKey);
	CCLOG("cpId: %s",cpId);
	CCLOG("channelId: %s",channelId);
	CCLOG("extraId: %s",extraId);
	CCLOG("gameId: %d",gameId);*/
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSCheckSessionReq");
	CSCheckSessionReq* bodyMsg = (CSCheckSessionReq*)msg;
	CCAssert(bodyMsg != NULL,"CSCheckSessionReq is NULL");
	if(bodyMsg != NULL)
	{
		bodyMsg->set_sess_id(sess);
		bodyMsg->set_channel_id_self(platformId);
		bodyMsg->set_server_id(serverId);
		bodyMsg->set_user_id(userId);
		bodyMsg->set_app_id(appID);
		bodyMsg->set_app_key(appKey);
		bodyMsg->set_cp_id(cpId);
		bodyMsg->set_channel_id(channelId);
		bodyMsg->set_extra_data(extraId);
		bodyMsg->set_game_id(gameId);

		// 统计相关
		bodyMsg->set_platform(platform);
		bodyMsg->set_version(svnNumber);
		bodyMsg->set_mac(macAddress);
		bodyMsg->set_eqtype(eqtype);
		bodyMsg->set_equdid(deviceID);
		NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGIN, true, true);
		if(XLogicManager::sharedManager()->isIOSNeedWaitingLayer == true)
        {
			//jackniu disable hui
			//WaitingLayerManager::Get()->PushOneMessage("CSCheckSessionReq");
            
            //XLogicManager::sharedManager()->isIOSNeedWaitingLayer = false;
        }
	}
}

void OnlineNetworkManager::sendGetRoleListMessage(unsigned int serverId)
{

	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetRoleListReq");
	CSGetRoleListReq* bodyMsg = (CSGetRoleListReq*)msg;
	bodyMsg->set_uid(UserData::getUserId().getID());
	bodyMsg->set_channel_id(UserData::getUserId().getChannel());
	//bodyMsg->add_server_id(serverId);
	bodyMsg->set_server_id(serverId);
	
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGIN, true, true);
	if(XLogicManager::sharedManager()->isIOSNeedWaitingLayer == true)
    {
    	//jackniu disable hui
       // WaitingLayerManager::Get()->PushOneMessage("CSGetRoleListReq");
    }
}

void OnlineNetworkManager::sendGetRoleListMessage(const vector<unsigned int>& serverList)
{
	/*Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetRoleListReq");
	CSGetRoleListReq* bodyMsg = (CSGetRoleListReq*)msg;
	bodyMsg->set_uid(UserData::getUserId().getID());
	bodyMsg->set_channel_id(UserData::getUserId().getChannel());
	for(vector<unsigned int>::const_iterator it = serverList.cbegin();it != serverList.cend();it++)
	{
		bodyMsg->add_server_id((*it));
	}
	
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGIN, true, true);*/
}

void OnlineNetworkManager::sendGetTokenMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetTokenReq");
	CSGetTokenReq* bodyMsg = (CSGetTokenReq*)msg;
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGIN, true, true);
	//WaitingLayerManager::Get()->PushOneMessage("CSGetTokenReq");
}

void OnlineNetworkManager::sendCheckTokenMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSCheckTokenReq");
	CSCheckTokenReq* bodyMsg = (CSCheckTokenReq*)msg;
	USERINFO info = UserData::GetUserInfo();
	bodyMsg->set_user_id(info.id.getID());
	bodyMsg->set_channel_id(info.id.getChannel());
	bodyMsg->set_tokenid(info.tokenId);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOGIN, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSCheckTokenReq");
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendLogoutMessage()
{
	CCLOG("sendLogoutMessage()");
	// currently simply disconnect to online server and set ip and port to login server
	this->cleanServerConnect(false);
	//this->setServerIP(ONLINE_SERVER_IP);
	//this->setServerPort(ONLINE_SERVER_PORT);
}


//------------------------------------------------------------------
void OnlineNetworkManager::sendPlayerEnterMapMessage(int mapId, int xPos, int yPos,int orient)
{
	CCLOG("CSEnterMapReq()");
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEnterMapReq");
	CSEnterMapReq* bodyMsg = (CSEnterMapReq*)msg;
	bodyMsg->set_mapid(mapId);
	bodyMsg->set_xpos(xPos);
	bodyMsg->set_ypos(yPos);
	bodyMsg->set_orient(orient);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);

	//切换地图添加转菊花
	WaitingLayerManager::Get()->PushOneMessage("CSEnterMapReq");	
}


//------------------------------------------------------------------
void OnlineNetworkManager::sendPlayerMoveMessage(int xPos, int yPos, unsigned int animID, bool bFlip)
{
	//CCLOG("sendPlayerMoveMessage()");
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSWalkReq");
	CSWalkReq* bodyMsg = (CSWalkReq*)msg;
	bodyMsg->set_xpos(xPos);
	bodyMsg->set_ypos(yPos);
	bodyMsg->set_animid(animID);
	bodyMsg->set_flip(bFlip);

	//don't cache the walk message if failed
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_WALK, false, true);
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendPlayerBattleMoveMessage(PLAYER_ID id, const cocos2d::CCPoint& pos, const cocos2d::CCPoint& direction, bool bEnd)
{
	static Machtimer machTimer;

	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattleMoveReq");
	CSBattleMoveReq* bodyMsg = (CSBattleMoveReq*)msg;
	bodyMsg->set_id(id.getID());
	bodyMsg->set_reg_time(id.getRegTime());
	bodyMsg->set_channel_id(id.getChannel());
	CSPoint* pPos = bodyMsg->mutable_pos();
	cocos2d::CCPoint phyPos = LevelManager::sShareInstance()->pointPixelToCell(pos);
	pPos->set_xpos(phyPos.x);
	pPos->set_ypos(phyPos.y);
	CSPoint* pDir = bodyMsg->mutable_direct();
	pDir->set_xpos((int)direction.x);
	pDir->set_ypos((int)direction.y);

	if(bEnd)
	{
		bodyMsg->set_time(-1);
	}
	else
	{
		bodyMsg->set_time(machTimer.elapsedMS());
	}
	

	//don't cache the walk message if failed
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_BATTLE_MOVE, true, true);
	//CCLOG("Send Move(%f, %f) time : %ld", phyPos.x, phyPos.y, machTimer.elapsedMS());
	machTimer.start();
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendPlayerAttackMessage(const CSPlayerAttackReq& attackReq)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPlayerAttackReq");
	CSPlayerAttackReq* bodyMsg = (CSPlayerAttackReq*)msg;
	*bodyMsg = attackReq;
	
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_ATTACK, true, true);
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendPlayerAttackOtherPlayerMessage(const CSPlayerHitPlayerReq& attackReq)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPlayerHitPlayerReq");
	CSPlayerHitPlayerReq* bodyMsg = (CSPlayerHitPlayerReq*)msg;
	*bodyMsg = attackReq;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_ATTACK, true, true);
}


//------------------------------------------------------------------
void OnlineNetworkManager::sendGetOtherPlayersMessage()
{
    bool isMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
    if(!isMainLandLevel)//add by jackniu ppve,press kuai shu jia ru button will call this func.disable it 
        return; 
	CCLOG("sendGetOtherPlayersMessage()");
	Message* msg = 
		MessageUtil::sShareInstance()->createMessageInstance("CSGetMapUsersReq");
	CSGetMapUsersReq* bodyMsg = (CSGetMapUsersReq*)msg;

	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GET_OTHER_PLAYERS, true, true);
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendCreateRoleMessage(const std::string &nick, const uint32_t type ,
												const char* deviceID,const char* platform,const char* ipAddress,
												const char* macAddress,const char* eqtype,const char* svnNumber)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSCreateRoleReq");
	CSCreateRoleReq* bodyMsg = (CSCreateRoleReq*)msg;
	bodyMsg->set_name(nick.c_str());
  //  bodyMsg->set_userid(UserData::getUserId().getID());
	//bodyMsg->set_channel_id(UserData::getUserId().getChannel());
	bodyMsg->set_role_type(type);
	USERINFO mUserData = UserData::GetUserInfo();
	bodyMsg->set_server_id(mUserData.serverId);
	bodyMsg->set_equdid(deviceID);
	bodyMsg->set_platform(platform);
	bodyMsg->set_ip(ipAddress);
	bodyMsg->set_mac(macAddress);
	bodyMsg->set_eqtype(eqtype);
	bodyMsg->set_version(svnNumber);
	
	//mUserData.serverId;
	WaitingLayerManager::Get()->PushOneMessage("CSCreateRoleReq");
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_CREATE_ROLE, true, true);
}

void OnlineNetworkManager::sendBackPackMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAllItemsReq");
	CSAllItemsReq* bodyMsg = (CSAllItemsReq*)msg;
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ITEM_REQ, true, true);
}

void OnlineNetworkManager::sendLoadCompleteMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattleLoadCompleteReq");
	CSBattleLoadCompleteReq* bodyMsg = (CSBattleLoadCompleteReq*)msg;
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_LOAD_COMPLETE, true, true);
}

void OnlineNetworkManager::sendLeaveBattleReqMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattleLeaveReq");
	CSBattleLeaveReq* bodyMsg = (CSBattleLeaveReq*)msg;
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_LEAVE_BATTLE, true, true);
}

//------------------------------------------------------------------
void OnlineNetworkManager::sendGetInstanceListMessage(uint32_t start, uint32_t end)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSInstanceReq");
	CSInstanceReq* bodyMsg = (CSInstanceReq*)msg;
	bodyMsg->set_start(start);
	bodyMsg->set_end(end);

	//don't cache the walk message if failed
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GET_BATTLE_LIST, true, true);
}

void OnlineNetworkManager::sendBackPackMove(unsigned int from,unsigned int to)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSItemMoveReq");
	CSItemMoveReq* bodyMsg = (CSItemMoveReq*)msg;
	bodyMsg->set_pos_from(from);
	bodyMsg->set_pos_to(to);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ITEM_MOVE, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSItemMoveReq");
}

void OnlineNetworkManager::sendEquipAllMsg(unsigned int fairyPos)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEquipAllBestReq");
	CSEquipAllBestReq* bodyMsg = (CSEquipAllBestReq*)msg;
	bodyMsg->set_pos(fairyPos);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ITEM_MOVE, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSEquipAllBestReq");
}

void OnlineNetworkManager::sendBackPackReOrder(unsigned int type)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSItemOrderReq");
	CSItemOrderReq* bodyMsg = (CSItemOrderReq*)msg;
	bodyMsg->set_target(type);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ITEM_REORDER, true, true);
}

void OnlineNetworkManager::sendBackPackRemoveItem(unsigned int pos, unsigned int itemId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSItemRemoveReq");
	CSItemRemoveReq* bodyMsg = (CSItemRemoveReq*)msg;
	bodyMsg->set_pos(pos);
	bodyMsg->set_item_id(itemId);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ITEM_REMOVE, true, true);
}
void OnlineNetworkManager::sendUseItem(unsigned int pos, unsigned int itemId,unsigned int num/* = 1*/)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSItemUseReq");
	CSItemUseReq* bodyMsg = (CSItemUseReq*)msg;
	bodyMsg->set_pos(pos);
	bodyMsg->set_item_id(itemId);
	bodyMsg->set_num(num);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ITEM_USE, true, true);
}

//void OnlineNetworkManager::sentEquipUpgradeMessage()
//{
//	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetEquipLvUpCDReq");
//	CSGetEquipLvUpCDReq* bodyMsg = (CSGetEquipLvUpCDReq*)msg;
//	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPUPGRADE_REQ, true, true);
//}

void OnlineNetworkManager::sentEquipUpgradeStrength(unsigned int pos)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEquipmentStrengthReq");
	CSEquipmentStrengthReq* bodyMsg = (CSEquipmentStrengthReq*)msg;
	bodyMsg->set_pos(pos);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPUPGRADE_STRENGTH, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSEquipmentStrengthReq");
}

void OnlineNetworkManager::sendEquipComposeReq(unsigned int exchangeId,unsigned int pos,unsigned int multi)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEquipmentComposeReq");
	CSEquipmentComposeReq* bodyMsg = (CSEquipmentComposeReq*)msg;
	bodyMsg->set_exchange_item_id(exchangeId);
	if(pos != 0)
	{
		bodyMsg->add_from_item_pos(pos);
	}
	bodyMsg->set_multi(multi);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPCOMPOSE_REQ, true, true);
}

void OnlineNetworkManager::sendItemExchangeReq(unsigned int exchangeId,unsigned int pos[],int length)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEquipmentComposeReq");
	CSEquipmentComposeReq* bodyMsg = (CSEquipmentComposeReq*)msg;
	bodyMsg->set_exchange_item_id(exchangeId);
	for(int i =0;i< length;i++)
	{
		bodyMsg->add_from_item_pos(pos[i]);
	}
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPCOMPOSE_REQ, true, true);
}

void OnlineNetworkManager::sendGetChipStatusReq(unsigned int chipId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetChipComposeStatusReq");
	CSGetChipComposeStatusReq* bodyMsg = (CSGetChipComposeStatusReq*)msg;
	bodyMsg->set_opt_target(chipId);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPCOMPOSE_REQ, true, true);
}

//宝石镶嵌

void OnlineNetworkManager::sendEquipInlayGemReq(unsigned int equip,unsigned int hole,unsigned int gemId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEquipInlayGemReq");
	CSEquipInlayGemReq* bodyMsg = (CSEquipInlayGemReq*)msg;
	bodyMsg->set_equip_pos(equip);
	bodyMsg->set_hole_index(hole);
	bodyMsg->set_gem_id(gemId);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPINLAYGEM_REQ, true, true);
}

void OnlineNetworkManager::sendGemRecastReq(unsigned int equip,unsigned int holes[],unsigned int holeNum,unsigned int key)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSResetGemAttrReq");
	CSResetGemAttrReq* bodyMsg = (CSResetGemAttrReq*)msg;
	bodyMsg->set_equip_pos(equip);
	for(unsigned int i=0;i < holeNum;i++)
	{
		if(holes[i] != 0)
		{
			bodyMsg->add_holes_index(holes[i]);
		}
	}
	if(key != 0)
	{
		bodyMsg->set_cost_attr_key(key);
	}
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GEMRECAST_REQ, true, true);
}

void OnlineNetworkManager::sendExchangeParameterReq(unsigned int exchangeId,unsigned int* params, int len)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExchangeParameterReq");
	CSExchangeParameterReq* bodyMsg = (CSExchangeParameterReq*)msg;
	bodyMsg->set_id(exchangeId);

	for(int i = 0; i < len; i++)
	{
		bodyMsg->add_param(params[i]);
	}
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EXCHANGEPARAM_REQ, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSExchangeParameterReq");
}

void OnlineNetworkManager::sendElfExchangeReq(unsigned int elfId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSEquipmentComposeReq");
	CSEquipmentComposeReq* bodyMsg = (CSEquipmentComposeReq*)msg;
	bodyMsg->set_exchange_item_id(elfId);

	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EQUIPCOMPOSE_REQ, true, true);
}

void OnlineNetworkManager::sendFindSoulStoneMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSearchSpiritStoneReq");
	CSSearchSpiritStoneReq* bodyMsg = (CSSearchSpiritStoneReq*)msg;
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SEARCH_SPRITE_STONE, true, true);
}

void OnlineNetworkManager::sendSoulStoneToStorage(unsigned int pos)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSpiritStoneMoveBagReq");
	CSSpiritStoneMoveBagReq* bodyMsg = (CSSpiritStoneMoveBagReq*)msg;
	bodyMsg->set_pos(pos);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_MOVE_SPRITE_STONE_TO_STORAGE, true, true);
}

void OnlineNetworkManager::sendUpgradeSoulStone(unsigned int pos)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSpiritStoneLevelUpReq");
	CSSpiritStoneLevelUpReq* bodyMsg = (CSSpiritStoneLevelUpReq*)msg;
	bodyMsg->set_pos(pos);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_UPGRADE_SPRITE_STONE, true, true);
}

void OnlineNetworkManager::sendTranStoneToEnergy(bool isAll,unsigned int pos)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSpiritStoneToSPReq");
	CSSpiritStoneToSPReq* bodyMsg = (CSSpiritStoneToSPReq*)msg;
	if(!isAll)
	{
		bodyMsg->set_pos(pos);
	}
	else
	{
		bodyMsg->set_pos(0);
	}
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_TRANS_SPRITE_TO_ENERGE, true, true);
}

void OnlineNetworkManager::sendSpriteExchangeMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetExchangeSSInfoReq");
	CSGetExchangeSSInfoReq* bodyMsg = (CSGetExchangeSSInfoReq*)msg;
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SPRITE_EXCHANGE, true, true);
}

/// <summary>
//	Task 请求相关的协议
/// </summary>
void OnlineNetworkManager::sendRqsTaskInfoMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSTaskInfoReq");
	CSTaskInfoReq* bodyMsg = (CSTaskInfoReq*)msg;	
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_TASK, true, true);
	//WaitingLayerManager::Get()->PushOneMessage("CSTaskInfoReq");
}

void OnlineNetworkManager::sendRqsTaskLogMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSTaskLogReq");
	CSTaskLogReq* bodyMsg = (CSTaskLogReq*)msg;	
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_TASK, true, true);	
}

void OnlineNetworkManager::sendRqsTaskDoStepMessage(unsigned int task_id,unsigned int step_id,unsigned int step_value)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSTaskDoStepReq");
	CSTaskDoStepReq* bodyMsg = (CSTaskDoStepReq*)msg;	
	bodyMsg->set_task_id(task_id);
	bodyMsg->set_step_id(step_id);
	bodyMsg->set_step_value(step_value);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_TASK, true, true);	
	//WaitingLayerManager::Get()->PushOneMessage("CSTaskDoStepReq");
}

void OnlineNetworkManager::sendReqNetStateMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPingReq");
	CSPingReq* bodyMsg = (CSPingReq*)msg;	

	long delayTime = CCDirector::sharedDirector()->getNetDelayTime();
	bodyMsg->set_last_delay(delayTime);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_TASK, true, true);
}

void OnlineNetworkManager::sendGetTaskStatusMessage(int taskId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSTaskStatusReq");
	CSTaskStatusReq* bodyMsg = (CSTaskStatusReq*)msg;	
	bodyMsg->set_task_id(taskId);
	NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_TASK_INFO, true, true);

	WaitingLayerManager::Get()->PushOneMessage("CSTaskStatusReq");
}

void OnlineNetworkManager::sendSkillBeginMessage( const CSSkillBeginReq& req )
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSkillBeginReq");
	CSSkillBeginReq* bodyMsg = (CSSkillBeginReq*)msg;
	*bodyMsg = req;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_ATTACK, true, true);
}

/// <summary>
//	精灵相关协议
/// </summary>
void OnlineNetworkManager::sendReqOneHeroElfMessage(PLAYER_ID roleId,const char* roleName)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSMonListReq");
	CSMonListReq* bodyMsg = (CSMonListReq*)msg;
	bodyMsg->set_uid(roleId.getID());
	bodyMsg->set_reg_time(roleId.getRegTime());
	bodyMsg->set_channel_id(roleId.getChannel());
	//bodyMsg->set_name(roleName);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ELF, true, true);
}

void OnlineNetworkManager::sendReqSetElfStateMessage(unsigned int elfId,unsigned int state)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSetMonStatusReq");
	CSSetMonStatusReq* bodyMsg = (CSSetMonStatusReq*)msg;
	bodyMsg->set_mon_id(elfId);
	bodyMsg->set_mon_status(state);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ELF, true, true);
}

void OnlineNetworkManager::sendQueryShardReq()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSQueryShardReq");
	CSQueryShardReq* bodyMsg = (CSQueryShardReq*)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_QUERY_REQ, true, true);
}

void OnlineNetworkManager::sendAttributesReq(int attributes[],int length)
{
	if(length == 0)
	{
		return;
	}
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAttributesReq");
	CSAttributesReq* bodyMsg = (CSAttributesReq*)msg;
	for(int i =0;i<length;i++)
	{
		bodyMsg->add_keys(attributes[i]);
	}
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_ATTRIBUTESREQ, true, true);
}

void OnlineNetworkManager::sendUserInfoReq(PLAYER_ID id)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetUserInfoReq");
	CSGetUserInfoReq* bodyMsg = (CSGetUserInfoReq*)msg;
	bodyMsg->set_userid(id.getID());
	bodyMsg->set_reg_time(id.getRegTime());
	bodyMsg->set_channel_id(id.getChannel());
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_USERINFO, true, true);
}

void OnlineNetworkManager::sendOnceSetReqMessage(unsigned int index)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSOnceSetReq");
	CSOnceSetReq* bodyMsg = (CSOnceSetReq*)msg;
	bodyMsg->set_index(index);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SET, true, true);
}

void OnlineNetworkManager::sendOnceReqMessage(int indexs[],int length)
{
/* index available [1 10000]
 * index used
 * 
 * 1001
 * 1002
 *  
*/
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSOnceReq");
	CSOnceReq* bodyMsg = (CSOnceReq*)msg;

	for(int i =0;i<length;i++)
	{
		bodyMsg->add_indexs(indexs[i]);
	}

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SET, true, true);
}

void OnlineNetworkManager::sendOnceReqMessage(std::vector<int> &vec)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSOnceReq");
	CSOnceReq* bodyMsg = (CSOnceReq*)msg;

	int length = vec.size();
	for(int i =0;i<length;i++)
	{
		bodyMsg->add_indexs(vec[i]);
	}

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SET, true, true);
}

void OnlineNetworkManager::sendOnceReqMessage(std::map<unsigned int,bool> &_map,unsigned int attachValue)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSOnceReq");
	CSOnceReq* bodyMsg = (CSOnceReq*)msg;

	for (std::map<unsigned int,bool>::iterator iter = _map.begin();
		 iter != _map.end(); iter++)
	{
		bodyMsg->add_indexs((*iter).first - attachValue);
	}

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SET, true, true);
}

void OnlineNetworkManager::sendGetUserSkillReqMessage(PLAYER_ID roleId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetUserSkillReq");
	CSGetUserSkillReq * bodyMsg = (CSGetUserSkillReq *)msg;

	bodyMsg->set_userid(roleId.getID());
	bodyMsg->set_reg_time(roleId.getRegTime());
	bodyMsg->set_channel_id(roleId.getChannel());

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SKILL_UI, true, true);
}

void OnlineNetworkManager::sendAddUserSkillReqMessage(unsigned int skillId,unsigned int levelId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAddUserSkillReq");
	CSAddUserSkillReq * bodyMsg = (CSAddUserSkillReq *)msg;

	bodyMsg->set_skill_id(skillId);
	bodyMsg->set_skill_level(levelId);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SKILL_UI, true, true);
}

void OnlineNetworkManager::sendUserSkillDiffReqMessage(unsigned int skillId,unsigned int levelId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSUserSkillDiffReq");
	CSUserSkillDiffReq * bodyMsg = (CSUserSkillDiffReq *)msg;

	bodyMsg->set_skill_id(skillId);
	bodyMsg->set_skill_level(levelId);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SKILL_UI, true, true);
}

void OnlineNetworkManager::sendGetSlotReqMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAttributesReq");
	CSAttributesReq * bodyMsg = (CSAttributesReq *)msg;

	bodyMsg->add_keys(SKILL_SLOT_1);
	bodyMsg->add_keys(SKILL_SLOT_2);
	bodyMsg->add_keys(SKILL_SLOT_3);
	bodyMsg->add_keys(SKILL_SLOT_4);
	bodyMsg->add_keys(SKILL_SLOT_5);

	bodyMsg->add_keys(NEW_SKILL_ID);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SKILL_UI, true, true);
}

void OnlineNetworkManager::sendSetSlotAttributeMessage(unsigned int slotIndex,unsigned skillId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAttributesSetReq");
	CSAttributesSetReq * bodyMsg = (CSAttributesSetReq *)msg;

	bodyMsg->set_key(slotIndex - 1 + SKILL_SLOT_1);
	bodyMsg->set_value(skillId);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_SKILL_UI, true, true);
}

// Note: 精灵唤醒相关协议
void OnlineNetworkManager::SendElfAwakeMessage(unsigned int awakeType)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSTrainReq");
	CSTrainReq  * bodyMsg = (CSTrainReq  *)msg;

	bodyMsg->set_type(awakeType);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, ELF_TAG_AWAKE, true, true);
}

void OnlineNetworkManager::SendAwakeConfigMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSTrainDetermineReq");
	CSTrainDetermineReq  * bodyMsg = (CSTrainDetermineReq  *)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, ELF_TAG_AWAKE, true, true);
}

// Note: 精灵探索协议
void OnlineNetworkManager::SendExploreReq(unsigned int type)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExploreReq");
	CSExploreReq * bodyMsg = (CSExploreReq *)msg;
	bodyMsg->set_option(type);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, ELF_TAG_EXPLORE, true, true);
}

void OnlineNetworkManager::SendExploreDetermineReq(unsigned int optionId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExploreDetermineReq");
	CSExploreDetermineReq  * bodyMsg = (CSExploreDetermineReq  *)msg;
	bodyMsg->set_option(optionId);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, ELF_TAG_EXPLORE, true, true);
}
void OnlineNetworkManager::SendPVPLearnReq(unsigned int uid ,unsigned int reg_tm ,unsigned int channel_id ,bool mandatory)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPLearnReq");
	CSPVPLearnReq  * bodyMsg = (CSPVPLearnReq  *)msg;
	//bodyMsg->set_option(optionId);
	bodyMsg->set_uid(uid);
	bodyMsg->set_channel_id(channel_id);
	bodyMsg->set_reg_time(reg_tm);
	bodyMsg->set_mandatory(mandatory);
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, PVPLERN, true, true);
}

 void OnlineNetworkManager::SendPVPAckLearnReq(unsigned int uid ,unsigned int reg_tm ,unsigned int channel_id ,bool accept,int type )
 {
	 Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPAckLearnReq");
	 CSPVPAckLearnReq  * bodyMsg = (CSPVPAckLearnReq  *)msg;
	 //bodyMsg->set_option(optionId);
	 bodyMsg->set_uid(uid);
	 bodyMsg->set_channel_id(channel_id);
	 bodyMsg->set_reg_time(reg_tm);
	 bodyMsg->set_accept(accept);
	 bodyMsg->set_type(type);
	 
	 KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, PVPACKLERN, true, true);

 }
void OnlineNetworkManager::SendDelRoleReqMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSDelRoleReq");
	CSDelRoleReq  * bodyMsg = (CSDelRoleReq  *)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, DEL_HERO_PLAYER, true, true);
}

void OnlineNetworkManager::sendPvAIReq(PLAYER_ID userId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVAIReq");
	CSPVAIReq  * bodyMsg = (CSPVAIReq  *)msg;
	bodyMsg->set_userid(userId.getID());
	bodyMsg->set_reg_time(userId.getRegTime());
	bodyMsg->set_channel_id(userId.getChannel());
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, PVAI_TAG_REQ, true, true);
}

#if 0
void OnlineNetworkManager::sendOnceDailyReq(unsigned int *instanceIds, int len)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSOnceDailyReq");
	CSOnceDailyReq  * bodyMsg = (CSOnceDailyReq  *)msg;
	
	for(int it = 0; it < len; it++) 
	{
	
		bodyMsg->add_indexs(instanceIds[it]);
	}
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, JING_YING_FU_BEN_DAILY_ONCE, true, true);
}
#endif

void OnlineNetworkManager::sendDailyTaskReqMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSDailyTaskInfoReq");
	CSDailyTaskInfoReq  * bodyMsg = (CSDailyTaskInfoReq  *)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, DAILY_TASK_TAG, true, true);
}

void OnlineNetworkManager::sendDailyTaskAcceptReqMessage(unsigned int taskId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAcceptDailyTaskReq");
	CSAcceptDailyTaskReq  * bodyMsg = (CSAcceptDailyTaskReq  *)msg;
	bodyMsg->set_daily_task_id(taskId);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, DAILY_TASK_TAG, true, true);
}

void OnlineNetworkManager::sendGiveUpTaskReqMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGiveUpDailyTaskReq");
	CSGiveUpDailyTaskReq  * bodyMsg = (CSGiveUpDailyTaskReq  *)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, DAILY_TASK_TAG, true, true);
}

void OnlineNetworkManager::sendGetDailyTaskRewardReqMessage()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetDailyTaskRewardReq");
	CSGetDailyTaskRewardReq  * bodyMsg = (CSGetDailyTaskRewardReq  *)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, DAILY_TASK_TAG, true, true);
}

void OnlineNetworkManager::SendBattlePlayerRevival()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattlePlayerRevivalReq");
	CSBattlePlayerRevivalReq   * bodyMsg = (CSBattlePlayerRevivalReq   *)msg;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, DAILY_TASK_TAG, true, true);
}

void OnlineNetworkManager::sendRemoteMonsterDieReqMessage(std::vector<int> monsterIdVec)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSMonsterDieReq");
	CSMonsterDieReq   * bodyMsg = (CSMonsterDieReq *)msg;

	for (size_t i = 0; i < monsterIdVec.size(); i++)
	{
		bodyMsg->add_monsters(monsterIdVec[i]);
	}

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, REMOTE_MONSTER_DIE, true, true);
}

void OnlineNetworkManager::sendBattlePrepareReq(int battleId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattlePrepareReq");
	CSBattlePrepareReq   * bodyMsg = (CSBattlePrepareReq   *)msg;

	bodyMsg->set_instance_id(battleId);

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, PREPARE_BATTLE, true, true);
}

void OnlineNetworkManager::sendLocalBattlePrepareRsq(CSBattlePrepareRsp prepareRsq)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBattlePrepareRsp");
	CSBattlePrepareRsp   * bodyMsg = (CSBattlePrepareRsp   *)msg;
	*bodyMsg = prepareRsq;

	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, PREPARE_BATTLE, true, true);
}

void OnlineNetworkManager::sendGetRandomName(unsigned int sexual)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSNickNameReq");
	CSNickNameReq   * bodyMsg = (CSNickNameReq   *)msg;
	bodyMsg->set_sexual(sexual);
	USERINFO info = UserData::GetUserInfo();
	bodyMsg->set_zone_id(info.serverId);
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_CHANGE_NICK, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSNickNameReq");
}

void OnlineNetworkManager::sendUserDeviceInfoToServer(const char* deviceID,const char* platform,const char* ipAddress,
	const char* macAddress,const char* eqtype,const char* svnNumber)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSDeviceInfo");
	CSDeviceInfo * bodyMsg = (CSDeviceInfo*)msg;
	bodyMsg->set_version(svnNumber);
	bodyMsg->set_platform(platform);
	bodyMsg->set_ip(ipAddress);
	bodyMsg->set_mac(macAddress);
	bodyMsg->set_equdid(deviceID);
	bodyMsg->set_eqtype(eqtype);
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_USERINFO, true, true);
}

void OnlineNetworkManager::sendGetAnnouncementPreReq()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAnnouncementPreReq");
	CSAnnouncementPreReq* bodyMsg = (CSAnnouncementPreReq*)msg;
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_QUERY_REQ, true, true);
}

void OnlineNetworkManager::sendHeroHPChangeReq(int nBlood)
{
	bool isPVE = LevelManager::sShareInstance()->isCurrentPVELevel();
	bool isUnder = LevelManager::sShareInstance()->isCurrentUndergroundLevel();
	bool isElite = LevelManager::sShareInstance()->isCurrentEliteLevel();
	if (isPVE || isUnder || isElite)
	{
// 		Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPlayerHPReq");
// 		CSPlayerHPReq* bodyMsg = dynamic_cast<CSPlayerHPReq*>(msg);
// 		if (bodyMsg)
// 		{
// 			bodyMsg->set_blood(nBlood);
// 			KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_HP, true, true);
// 		}	
        m_updateHpAmount += nBlood;
    }
}

void OnlineNetworkManager::CreateUpdateHpQueue()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OnlineNetworkManager::_sendHeroHPChangeReq),
        this, 2.0f, false);
}

void OnlineNetworkManager::_sendHeroHPChangeReq(float f)
{
    if (m_updateHpAmount != 0)
    {
        Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPlayerHPReq");
        CSPlayerHPReq* bodyMsg = dynamic_cast<CSPlayerHPReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_blood(m_updateHpAmount);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_HP, true, true);
            m_updateHpAmount = 0;
        }
    }
}

void OnlineNetworkManager::SendMoveFairyPosReq(unsigned int fromPos,unsigned int toPos)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSMoveFairyPosReq");
	CSMoveFairyPosReq* bodyMsg = dynamic_cast<CSMoveFairyPosReq*>(msg);
	if (bodyMsg)
	{
		bodyMsg->set_from_pos(fromPos);
		bodyMsg->set_to_pos(toPos);
		KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_RED_NAME_REMAIN, true, true);
	}
	
}

void OnlineNetworkManager::SendPlayerRedNameTimerReq()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetRednameTimeReq");
	if (msg)
	{
		CSGetRednameTimeReq* bodyMsg = dynamic_cast<CSGetRednameTimeReq*>(msg);
		if (bodyMsg)
		{
			const USERINFO& userInfo = UserData::GetUserInfo();
			bodyMsg->set_userid(userInfo.id.getID());
			bodyMsg->set_channelid(userInfo.id.getChannel());
			bodyMsg->set_reg_tm(userInfo.id.getRegTime());
			KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_RED_NAME_REMAIN, true, true);
		}
	}		
}

void OnlineNetworkManager::SendExtractSprite(int type)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSpiritDrawReq");
    if (msg)
    {
        CSSpiritDrawReq* bodyMsg = dynamic_cast<CSSpiritDrawReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_type(type);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EXTRACT_SPRITE, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSSpiritDrawReq");
        }
    }		
}
// 请求玩家排名情况
void OnlineNetworkManager::SendGetPlayerRankNum(int rankName, int userId, int regTime, int channelId)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSQueryPlayerRankInfoReq");
    if (msg)
    {
        CSQueryPlayerRankInfoReq* bodyMsg = dynamic_cast<CSQueryPlayerRankInfoReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_rank_name(rankName);
            bodyMsg->set_userid(userId);
            bodyMsg->set_reg_time(regTime);
            bodyMsg->set_channel_id(channelId);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_PLAYER_RANK, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSQueryPlayerRankInfoReq");
        }
    }	
}

// 请求特定类型的排行榜信息 
void OnlineNetworkManager::SendIndexTypeRankInfo(int rankName, int rankType, int startPos, int endPos)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSQueryRankReq");
    if (msg)
    {
        CSQueryRankReq* bodyMsg = dynamic_cast<CSQueryRankReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_rank_name(rankName);
            bodyMsg->set_rank_type(rankType);
            bodyMsg->set_rank_start(startPos);
            bodyMsg->set_rank_end(endPos);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_RANK_INFO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSQueryRankReq");
        }
    }	
}

void OnlineNetworkManager::SendGetPVAIInfo()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVAIInfoReq");
    if (msg)
    {
        CSPVAIInfoReq* bodyMsg = dynamic_cast<CSPVAIInfoReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, PVAI_TAG_INFO_REQ, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSPVAIInfoReq");
        }
    }	
}

 void OnlineNetworkManager::SendGetStarRewardInfo()
 {
     //Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSChkInstanceStarRewardReq");
     //if (msg)
     //{
     //    CSChkInstanceStarRewardReq* bodyMsg = dynamic_cast<CSChkInstanceStarRewardReq*>(msg);
     //    if (bodyMsg)
     //    {
     //        KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_STAR_REWARD, true, true);
     //        //WaitingLayerManager::Get()->PushOneMessage("CSChkInstanceStarRewardReq");
     //    }
     //}	
 }

 void OnlineNetworkManager::SendGetTreasureExploreInfo()
 {
     Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetExploreInfoReq");
     if (msg)
     {
         CSGetExploreInfoReq* bodyMsg = dynamic_cast<CSGetExploreInfoReq*>(msg);
         if (bodyMsg)
         {
             KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EXPLORE_TREASUER_INFO, true, true);
             WaitingLayerManager::Get()->PushOneMessage("CSGetExploreInfoReq");
         }
     }	
 }

 void OnlineNetworkManager::SendExploreTreasure(int mapId)
 {
     Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExchangeParameterReq");
     if (msg)
     {
         CSExchangeParameterReq* bodyMsg = dynamic_cast<CSExchangeParameterReq*>(msg);
         if (bodyMsg)
         {
             bodyMsg->set_id(mapId);
             KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EXPLORE_TREASUER, true, true);
             WaitingLayerManager::Get()->PushOneMessage("CSExchangeParameterReq");
         }
     }	
 }

 void OnlineNetworkManager::SendGuildBuy(int id, int index)
 {
     Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExchangeParameterReq");
     if (msg)
     {
         CSExchangeParameterReq* bodyMsg = dynamic_cast<CSExchangeParameterReq*>(msg);
         if (bodyMsg)
         {
             bodyMsg->set_id(id);
             bodyMsg->add_param(index);
             KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_EXPLORE_TREASUER, true, true);
             WaitingLayerManager::Get()->PushOneMessage("CSExchangeParameterReq");
         }
     }	
 }

 void OnlineNetworkManager::SendGetTreasureReward()
 {
     Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetExploreTreasureReq");
     if (msg)
     {
         CSGetExploreTreasureReq* bodyMsg = dynamic_cast<CSGetExploreTreasureReq*>(msg);
         if (bodyMsg)
         {
             KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GET_TREASURE_REWARD, true, true);
             WaitingLayerManager::Get()->PushOneMessage("CSGetExploreTreasureReq");
         }
     }	
 }

void OnlineNetworkManager::SendGetTopupAwardReq(int award_id)
{
    {
        Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetTopupAwardReq");
        if (msg)
        {
            CSGetTopupAwardReq* bodyMsg = dynamic_cast<CSGetTopupAwardReq*>(msg);
            if (bodyMsg)
            {
                bodyMsg->set_id(award_id);
                KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GET_TOPUP_REWARD, true, true);
            }
        }	
    }
}

void OnlineNetworkManager::SendGetShopTimes(std::vector<int> itemList)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSShoppingReq");
    if (msg)
    {
        CSShoppingReq* bodyMsg = dynamic_cast<CSShoppingReq*>(msg);
        if (bodyMsg)
        {
            for (size_t i = 0; i < itemList.size(); ++i)
            {
                bodyMsg->add_shopping_ids(itemList[i]);
            }

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_RANK_INFO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSShoppingReq");
        }
    }	
}

void OnlineNetworkManager::SendGetDailyReward(int type)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetDailyRewardReq");
    if (msg)
    {
        CSGetDailyRewardReq* bodyMsg = dynamic_cast<CSGetDailyRewardReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_key(type);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GET_DAILY_REWARD, true, true);
        }
    }	
}

void OnlineNetworkManager::sendMonthCardInfoReq()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSMcardInfoReq");
	CSMcardInfoReq* bodyMsg = dynamic_cast<CSMcardInfoReq*>(msg);
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_RANK_INFO, true, true);
}

void OnlineNetworkManager::sendMonthCardGetReq(unsigned int cardId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSMcardRewardReq");
	CSMcardRewardReq* bodyMsg = dynamic_cast<CSMcardRewardReq*>(msg);
	bodyMsg->set_card_id(cardId);
	KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_RANK_INFO, true, true);
}

void OnlineNetworkManager::SendGetExchangeParameters(std::vector<int> exchangeIds)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExchangeParameterListReq");
    if (msg)
    {
        CSExchangeParameterListReq* bodyMsg = dynamic_cast<CSExchangeParameterListReq*>(msg);
        if (bodyMsg)
        {
            for (size_t i = 0; i < exchangeIds.size(); ++i)
            {
                bodyMsg->add_ids(exchangeIds[i]);
            }

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NETWORK_TAG_GET_EXCHANGE_PARAMETERS, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSExchangeParameterListReq");
        }
    }
}

void OnlineNetworkManager::SendGetGuildList(unsigned int begin, unsigned int end)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSListGuildReq");
    if (msg)
    {
        CSListGuildReq* bodyMsg = dynamic_cast<CSListGuildReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_begin(begin);
            bodyMsg->set_end(end);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GET_GUILD_LIST, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSListGuildReq");
        }
    }
}

void OnlineNetworkManager::SendSearchIndexGuild(std::string name)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSearchGuildReq");
    if (msg)
    {
        CSSearchGuildReq* bodyMsg = dynamic_cast<CSSearchGuildReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_name(name.c_str());
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_SEARCH_GUILD, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSSearchGuildReq");
        }
    }
}

void OnlineNetworkManager::SendJoinGuild(int highGid, int lowGid)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSJoinGuildReq");
    if (msg)
    {
        CSJoinGuildReq* bodyMsg = dynamic_cast<CSJoinGuildReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_gid_high(highGid);
            bodyMsg->set_gid_low(lowGid);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_JOIN_GUILD, true, true);
        }
    }
}

void OnlineNetworkManager::SendCreateGuild(std::string name)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSCreateGuildReq");
    if (msg)
    {
        CSCreateGuildReq* bodyMsg = dynamic_cast<CSCreateGuildReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_name(name.c_str());
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_CREATE_GUILD, true, true);
        }
    }
}

void OnlineNetworkManager::SendGetGuildInfo()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSQueryPlayerGuildInfoReq");
    if (msg)
    {
        CSQueryPlayerGuildInfoReq* bodyMsg = dynamic_cast<CSQueryPlayerGuildInfoReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GET_GUILD_INFO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSQueryPlayerGuildInfoReq");
        }
    }
}

void OnlineNetworkManager::SendModifyNotice(std::string notice, std::string note)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSetGuildNoticeReq");
    if (msg)
    {
        CSSetGuildNoticeReq* bodyMsg = dynamic_cast<CSSetGuildNoticeReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_guild_notice(notice);
            bodyMsg->set_guild_note(note);

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_SET_GUILD_NOTICE, true, true);
        }
    }
}

void OnlineNetworkManager::SendGetGuildMember()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSListPlayerGuildReq");
    if (msg)
    {
        CSListPlayerGuildReq* bodyMsg = dynamic_cast<CSListPlayerGuildReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GET_GUILD_MEMBER, true, true);
        }
        WaitingLayerManager::Get()->PushOneMessage("CSListPlayerGuildReq");
    }
}

void OnlineNetworkManager::SendChangeGuildMemberStatus(std::vector<unsigned int> vt_user_id, 
                            std::vector<unsigned int> vt_reg_time, 
                            std::vector<unsigned int> vt_channel_id,
                            std::vector<unsigned int> vt_status)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSChangeOtherPlayerGuildStatusReq");
    if (msg)
    {
        CSChangeOtherPlayerGuildStatusReq* bodyMsg = dynamic_cast<CSChangeOtherPlayerGuildStatusReq*>(msg);
        if (bodyMsg)
        {
            for (size_t i = 0; i < vt_user_id.size(); ++i)
            {
                PlayerGuild* pInfo = bodyMsg->add_keys();
                pInfo->set_userid(vt_user_id[i]);
                pInfo->set_reg_tm(vt_reg_time[i]);
                pInfo->set_channel_id(vt_channel_id[i]);
                pInfo->set_status(vt_status[i]);
            }

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_CHANGE_GUILD_MEMBER_STATUS, true, true);
        }
    }
}

void OnlineNetworkManager::SendQuitGuild()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSQuitGuildReq");
    if (msg)
    {
        CSQuitGuildReq* bodyMsg = dynamic_cast<CSQuitGuildReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_QUIT_GUILD, true, true);
        }
    }
}

void OnlineNetworkManager::SendJiesanGuld()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSDropGuildReq");
    if (msg)
    {
        CSDropGuildReq* bodyMsg = dynamic_cast<CSDropGuildReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_Drop_GUILD, true, true);
        }
    }
}

void OnlineNetworkManager::SendGetGuildShopItems(int type)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetShoppingItemsReq");
    if (msg)
    {
        CSGetShoppingItemsReq* bodyMsg = dynamic_cast<CSGetShoppingItemsReq*>(msg);
        if (bodyMsg)
        {
            if(type != 0)
            {
                bodyMsg->set_type(0,type);
            }
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_SHOP_ITEMS, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGetShoppingItemsReq");
        }
    }
}

void OnlineNetworkManager::SendGetGuildBlessInfo(bool flag)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGoddessBlessReq");
    if (msg)
    {
        CSGoddessBlessReq* bodyMsg = dynamic_cast<CSGoddessBlessReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_ischeck(flag);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_BLESS, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGoddessBlessReq");
        }
    }
}

void OnlineNetworkManager::SendGetGuildContributeInfo(std::vector<int> temp)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExchangeItemInfoReq");
    if (msg)
    {
        CSExchangeItemInfoReq* bodyMsg = dynamic_cast<CSExchangeItemInfoReq*>(msg);
        if (bodyMsg)
        {
            for(int i = 0; i < temp.size(); i++)
            {
                bodyMsg->add_params(temp[i]);
            }
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_CONTRIBUTE_INFO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSExchangeItemInfoReq");
        }
    }
}

void OnlineNetworkManager::SendGuildContribute(int type)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPGContributeToGuildReq");
    if (msg)
    {
        CSPGContributeToGuildReq* bodyMsg = dynamic_cast<CSPGContributeToGuildReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_param(type);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_CONTRIBUTE_BUTTON, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSPGContributeToGuildReq");
        }
    }
}

void OnlineNetworkManager::SenderGuildLApplySettting(int type, int fightNum)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSSetPlayerGuildApplyConditionReq");
    if (msg)
    {
        CSSetPlayerGuildApplyConditionReq* bodyMsg = dynamic_cast<CSSetPlayerGuildApplyConditionReq*>(msg);
        if (bodyMsg)
        {
            bodyMsg->set_type(type);
            if(type == 2)
            {
                bodyMsg->set_fight(fightNum);
            }
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_APPLAY_SETTING, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSSetPlayerGuildApplyConditionReq");
        }
    }
}

void OnlineNetworkManager::SendGetGuildRecordInfoReq(unsigned int highID, unsigned int lowID, unsigned int zoneID)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRecordListReq");
    if (msg)
    {
        CSGuildRecordListReq* bodyMsg = dynamic_cast<CSGuildRecordListReq*>(msg);
        if (bodyMsg)
        {
			bodyMsg->set_guild_id_high(highID);
			bodyMsg->set_guild_id_low(lowID);
			bodyMsg->set_zone_id(zoneID);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_RECORD_INFO, true, true);
        }
    }
}

void OnlineNetworkManager::SendGetGuildUnionInfoReq()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSUnionInfoReq");
	if (msg)
	{
		CSUnionInfoReq* bodyMsg = dynamic_cast<CSUnionInfoReq*>(msg);
		if (bodyMsg)
		{
			KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_UNION_INFO, true, true);
		}
	}
}

void OnlineNetworkManager::SendGetGuildChooseInfoReq(int instanceId)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetRecruitMemberReq");
	if (msg)
	{
		CSGetRecruitMemberReq* bodyMsg = dynamic_cast<CSGetRecruitMemberReq*>(msg);
		if (bodyMsg)
		{
			bodyMsg->set_instance_id(instanceId);
			KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_UNION_INFO, true, true);
		}
	}
}

void OnlineNetworkManager::SendCreateGuildInstanceReq(int instanceId, bool hasChoose, CSPlayerGuildInfo info)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVAI3Req");
	if (msg)
	{
		CSPVAI3Req* bodyMsg = dynamic_cast<CSPVAI3Req*>(msg);
		if (bodyMsg)
		{
			bodyMsg->set_instance_id(instanceId);
			if(hasChoose){
				bodyMsg->mutable_infos()->CopyFrom(info);
			}
			KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_CREATE_INSTANCE, true, true);
		}
	}
}

void OnlineNetworkManager::SendGetGuildRedEnvoInfoReq()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRedPacketQueryReq");
    if (msg)
    {
        CSGuildRedPacketQueryReq* bodyMsg = dynamic_cast<CSGuildRedPacketQueryReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_RED_ENVO_INFO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGuildRedPacketQueryReq");
        }
    }
}

void OnlineNetworkManager::SendFirstSendRedEnvoReq(unsigned int envoID, unsigned int envoType)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRedPacketFirstCheckReq");
    if (msg)
    {
        CSGuildRedPacketFirstCheckReq* bodyMsg = dynamic_cast<CSGuildRedPacketFirstCheckReq*>(msg);
        if (bodyMsg)
        {
			bodyMsg->set_packet_id(envoID);
			bodyMsg->set_packet_type(envoType);
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_SEND_RED_ENVO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGuildRedPacketFirstCheckReq");
        }
    }
}

void OnlineNetworkManager::SendSecondSendRedEnvoReq(unsigned int envo_id, unsigned int envo_type)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRedPacketSendReq");
    if (msg)
    {
        CSGuildRedPacketSendReq* bodyMsg = dynamic_cast<CSGuildRedPacketSendReq*>(msg);
        if (bodyMsg)
        {
			bodyMsg->set_packet_id(envo_id);
			bodyMsg->set_packet_type(envo_type);

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_SECOND_SEND_RED_ENVO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGuildRedPacketSendReq");
        }
    }
}

void OnlineNetworkManager::SendGetRedEnvoReq(CSGuildRedPacketKey key)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRedPacketGetAwardReq");
    if (msg)
    {
        CSGuildRedPacketGetAwardReq* bodyMsg = dynamic_cast<CSGuildRedPacketGetAwardReq*>(msg);
        if (bodyMsg)
        {
			bodyMsg->mutable_key()->set_channelid(key.channelid());
			bodyMsg->mutable_key()->set_reg_tm(key.reg_tm());
			bodyMsg->mutable_key()->set_send_tm(key.send_tm());
			bodyMsg->mutable_key()->set_userid(key.userid());
			bodyMsg->mutable_key()->set_zone_id(key.zone_id());

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_GET_RED_ENVO, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGuildRedPacketGetAwardReq");
        }
    }
}

void OnlineNetworkManager::SendGetRedEnvoRecordReq(CSGuildRedPacketKey key)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRedPacketRecordReq");
    if (msg)
    {
        CSGuildRedPacketRecordReq* bodyMsg = dynamic_cast<CSGuildRedPacketRecordReq*>(msg);
        if (bodyMsg)
        {
			bodyMsg->mutable_key()->set_channelid(key.channelid());
			bodyMsg->mutable_key()->set_reg_tm(key.reg_tm());
			bodyMsg->mutable_key()->set_send_tm(key.send_tm());
			bodyMsg->mutable_key()->set_userid(key.userid());
			bodyMsg->mutable_key()->set_zone_id(key.zone_id());

            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_GET_RED_ENVO_RECORD, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGuildRedPacketRecordReq");
        }
    }
}

void OnlineNetworkManager::SendGetRedEnvoConfigReq()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGuildRedPacketConfigReq");
    if (msg)
    {
        CSGuildRedPacketConfigReq* bodyMsg = dynamic_cast<CSGuildRedPacketConfigReq*>(msg);
        if (bodyMsg)
        {
            KSendMessageResult result = NetWorkUtil::sendMessage(bodyMsg, PKG_TIMEOUT, NET_TAG_GUILD_GET_RED_ENVO_CONFIG, true, true);
            WaitingLayerManager::Get()->PushOneMessage("CSGuildRedPacketConfigReq");
        }
    }
}
