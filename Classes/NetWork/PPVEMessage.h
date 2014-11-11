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

/*
* 处理ppve的发送的消息及接收的消息，保存用户数据在这个类
*/
class PPVEMessage : 
	public TSingleton<PPVEMessage>, 
	public BaseMsgHandler
{
public:
	PPVEMessage();
    
	void InitHandler(MessageHandler * messageHolder);

public:
	
	/// send request message to server
	void	getRoomList();

	//// create one room , i am the host 
	void	createRoom();

	void	exitRoom();

	/// join a room
	void    JoinRoom(int roomID);

	/// set ready or not
	void    setReady(bool bReady);

	/// start game now
	void    startGame();

	/// kiff off guest
	void    kickOffGuest(int seat);

	vector<PVERoomBase>& getRoomData(){return m_lRooms;};

	vector<PVESeatBase>& getSeatData(){return m_lSeats;}

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
	vector<PVERoomBase> m_lRooms;
	vector<PVESeatBase> m_lSeats;
};

