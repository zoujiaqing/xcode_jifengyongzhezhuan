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

#include "NetWorkUtil.h"
#include "cs_battle.pb.h"
#include "cs_core.pb.h"

#include "Singleton_t.h"
#include "BaseMsgHandler.h"

class BaseMsgHandler;
class MessageHandler;

class PVPMessage :
public TSingleton<PVPMessage>,
public BaseMsgHandler
{
public:
	PVPMessage();
	void InitHandler(MessageHandler * messageHolder);
public:
	
    
    void HandleMsgNewMail(NORMAL_MSG_PACKAGE* pkg);
    
    void sendAccountBar();
    void HandleMsgAccountBar(NORMAL_MSG_PACKAGE* pkg);
    
    ////////////////////ShopUI//////////////////////////
    void sendShopItems();
    void HandleMsgShopItems(NORMAL_MSG_PACKAGE* pkg);
    
    void buyShopItem(int index);
    void HandleBuyShopItem(NORMAL_MSG_PACKAGE* pkg);
    
    void sendToShop(int pos);
    void HandleSendToShop(NORMAL_MSG_PACKAGE* pkg);
    
    void sendToBag(int pos);
    void HandleSendToBag(NORMAL_MSG_PACKAGE* pkg);
    
    
    ////////////////////Talent//////////////////////////
    void getTalentList();
    void getOneTalent(int talent_id, int level);
    void levelUpTalent(int talent_id);
    
    void HandleMsgTalentList(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgOneTalent(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgLevelUpTalent(NORMAL_MSG_PACKAGE* pkg);
    //////////////////////AutoOnHook///////////////////
    
    void sendCSABMonsterReq(unsigned int instance_id, unsigned int instance_type=0);
    void sendCSAutoBattleReq(unsigned int instance_id, unsigned int param);
    void sendCSCancelAutoBattleReq();
    void sendSpeedUpReq(int diamand_id, int freshWay = 0);
    
    void HandleMsgCSABMonsterRsp(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgAutoBattleRsp(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgCSABRewardRsp(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgCSCancelAutoBattleRsp(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgSpeedUpRsp(NORMAL_MSG_PACKAGE* pkg);
    void HandleMsgCSAttributesRsp(NORMAL_MSG_PACKAGE* pkg);

    //////////////////////PVP//////////////////////////
	/// send request message to server
	void	pvp_getRoomList();
    
	////create one room, i am the host
	void	pvp_createRoom_1vs1();
    void	pvp_createRoom_2vs2();
    
	void	pvp_exitRoom();
    
	/// join a room
	void    pvp_JoinRoom(int roomID);
    
	/// set ready or not
	void    pvp_setReady(bool bReady);
    
	/// start game now
	void    pvp_startGame();
    
	/// kiff off guest
	void    pvp_kickOffGuest(int seat);
    
    void    pvp_changeSeat(int seat);
    
	vector<PVPRoomBase>& pvp_getRoomData(){return m_lRooms;};
    
	vector<PVPSeatBase>& pvp_getSeatData(){return m_lSeats;}
    
protected:
	void	HandlerMessage(string cmd, NORMAL_MSG_PACKAGE* package);
    
	void	handleMsgCloseRoom(NORMAL_MSG_PACKAGE* pkg);
    
	/// handle  response
	void	handleMsgGetRoom(NORMAL_MSG_PACKAGE* package);
    
	/// handle message of join room
	void	HandleMsgJoinRoom(NORMAL_MSG_PACKAGE* pkg);
    
protected:
	MessageHandler* m_pMessageHolder;
	/// rooms of ppve
	vector<PVPRoomBase> m_lRooms;
	vector<PVPSeatBase> m_lSeats;
};


