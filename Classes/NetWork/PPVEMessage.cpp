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
#include "PPVEMessage.h"
#include "BoneNPCManager.h"
#include "LevelDefine.h"
#include "UserData.h"


#include "cocos2d.h"
#include "tolua++.h"
#include "CCLuaEngine.h"
#include "SceneLayer.h"
#include "GameManager.h"
#include "GameResourceManager.h"
#include "cs_basic.pb.h"
#include "cs_core.pb.h"
#include "cs_battle.pb.h"
#include "errcode.pb.h"
#include "XLogicManager.h"

#include "MainMenuLayer.h"
#include "PPVELayer.h"
#include "Localization.h"

#include "OnlineNetworkManager.h"

PPVEMessage::PPVEMessage()
{
}

void PPVEMessage::InitHandler(MessageHandler * messageHolder)
{
	m_pMessageHolder = messageHolder;
}

void PPVEMessage::handleMsgGetRoom(NORMAL_MSG_PACKAGE* pkg)
{
	/// clear all rooms
	int tag = MainMenuLayer::getLayerTagByName("PPVELayer");
	PPVELayer* layer = MainMenuLayer::getUILayerByTag<PPVELayer>(tag,false);
	if(layer == NULL)
	{
		return;
	}
	layer->ClearAllRooms();

	if (pkg)
	{
		m_lRooms.clear();
		CSPVEQueryRoomRsp* msg = dynamic_cast<CSPVEQueryRoomRsp*>(pkg->msg);
		for(int i = 0; i < msg->rooms_size(); i++)
		{
			const PVERoomBase& room = msg->rooms(i);
			m_lRooms.push_back(room);

			/// show room list
			layer->AddNewRoom(room);
		}
	}

	/// call global script function
	if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPPVEModule())
	{ 
		CCLuaEngine::defaultEngine()->executeGlobalFunction("UpdatePPVEPage");
	}
}

void PPVEMessage::getRoomList()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVEQueryRoomReq");
	CSPVEQueryRoomReq* bodyMsg = (CSPVEQueryRoomReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PPVEMessage::createRoom()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVECreateRoomReq");
	CSPVECreateRoomReq* bodyMsg = (CSPVECreateRoomReq*)msg;
	const USERINFO& user = UserData::GetUserInfo();
    
    int level = GetPPVELayer()->getFuBenLevel();
	bodyMsg->set_top_level(level);

	/// to do... set battle level id 
	int instanceID = GetPPVELayer()->GetCurrentInstance();
	bodyMsg->set_battle_id(instanceID);

	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

/*
* room closed by host
*/
void PPVEMessage::handleMsgCloseRoom(NORMAL_MSG_PACKAGE* pkg)
{
	/// call global script function
	if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPPVEModule())
	{ 
		/// 必须加上判断是不是在ppve房间中的检测
		CCLuaEngine::defaultEngine()->executeGlobalFunction("closePPVERoom");
	}	
}


void PPVEMessage::exitRoom()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVEExitRoomReq");
	CSPVEExitRoomReq* bodyMsg = (CSPVEExitRoomReq*)msg;

	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PPVEMessage::JoinRoom(int roomID)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVEJoinRoomReq");
	CSPVEJoinRoomReq* bodyMsg = (CSPVEJoinRoomReq*)msg;
	bodyMsg->set_room_id(roomID);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PPVEMessage::HandleMsgJoinRoom(NORMAL_MSG_PACKAGE* pkg)
{
	m_lSeats.clear();
	CSPVEJoinRoomRsp* msg = (CSPVEJoinRoomRsp*)pkg->msg;
    
	int roomid = msg->room_id();
	int toplevel = msg->top_level();
    
	for(int i = 0; i < msg->seats_size(); i++)
	{
		const PVESeatBase& seat = msg->seats(i);
		CCLOG("%d-%d,%s, %d", seat.id(), seat.avatar(), seat.name().c_str(), seat.ready());
		m_lSeats.push_back(seat);
	}
    
	lua_State* L_state = CCLuaEngine::defaultEngine()->getLuaState();
    
	/// update all the seat information
	for(int n = 0; n < 4; n++)
	{
		bool bSeatON = false;

		for(size_t i = 0; i < m_lSeats.size(); i++)
		{
			if (m_lSeats[i].id() == n)
			{
				lua_getglobal(L_state, "PlayerOnSeat");
				tolua_pushnumber(L_state, n);

				/// seat have player
				lua_newtable(L_state);
                
				lua_pushstring(L_state, "seat_id");
				lua_pushnumber(L_state, m_lSeats[i].id());
				lua_settable(L_state,-3);

				lua_pushstring(L_state, "level");
				lua_pushnumber(L_state, m_lSeats[i].level());
				lua_settable(L_state,-3);
                
				lua_pushstring(L_state, "name");
				lua_pushstring(L_state, m_lSeats[i].name().c_str());					
				lua_settable(L_state,-3);

				lua_pushstring(L_state, "Avatar");
				lua_pushnumber(L_state, m_lSeats[i].avatar());
				lua_settable(L_state,-3);

				lua_pushstring(L_state, "fightNum");
				lua_pushnumber(L_state, m_lSeats[i].effective());
				lua_settable(L_state,-3);

				lua_pushstring(L_state, "bReady");
				lua_pushboolean(L_state, m_lSeats[i].ready());
				lua_settable(L_state,-3);
                
				bSeatON = true;
								
				lua_pcall(L_state, 2, LUA_MULTRET, 0);
				break;
			}
		}
        
		/// not palyer on this seat
		if (!bSeatON)
		{
			lua_getglobal(L_state, "PlayerLeaveSeat");
			tolua_pushnumber(L_state, n);
			lua_pcall(L_state, 1, LUA_MULTRET, 0);
		}
	}
    
	/// call global script function
	if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPPVEModule())
	{ 
		lua_getglobal(L_state, "EnterRoom");
		tolua_pushnumber(L_state, roomid);
		tolua_pushnumber(L_state, toplevel);
		lua_pcall(L_state, 2, LUA_MULTRET, 0);
	}
}

void PPVEMessage::setReady(bool bReady)
{
	if (bReady)
	{
		Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVEReadyReq");
		CSPVEReadyReq* bodyMsg = (CSPVEReadyReq*)msg;
		OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);	
	}
	else
	{
		Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVECancelReadyReq");
		CSPVECancelReadyReq* bodyMsg = (CSPVECancelReadyReq*)msg;
		OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
	}
}

/// start game now
void PPVEMessage::startGame()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVEStartReq");
	CSPVEStartReq* bodyMsg = (CSPVEStartReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PPVEMessage::kickOffGuest(int seat)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVEKickoffReq");
	CSPVEKickoffReq* bodyMsg = (CSPVEKickoffReq*)msg;
	bodyMsg->set_seat(seat);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PPVEMessage::HandlerMessage(string cmd, NORMAL_MSG_PACKAGE* package)
{
	if(cmd == "CSPVEQueryRoomRsp")
	{
		handleMsgGetRoom(package);
	}

	else if (cmd == "CSPVECloseRoomRsp")
	{
		handleMsgCloseRoom(package);
	}

	else if (cmd == "CSPVEJoinRoomRsp")
	{
		HandleMsgJoinRoom(package);
	}
    
	else if (cmd == "CSPVEKickoffRsp")
	{
		/// 被房主剔除
		/// call global script function
		if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPPVEModule())
		{ 
			CCLuaEngine::defaultEngine()->executeGlobalFunction("kickOffFromRoom");
		}
	}
    
	else if (cmd == "CSPVEJoinRoomFailureRsp")
	{
		string strMsgError = Localizatioin::getLocalization("M_JOINROOM_FAIL");;
		ShowMessage(strMsgError.c_str());
	}

	else if (cmd == "CSPVEBaseInfoRsp")
	{
		 CSPVEBaseInfoRsp* msg = (CSPVEBaseInfoRsp*)package->msg;
		 if (msg)
		 {
			 lua_State* L_state = CCLuaEngine::defaultEngine()->getLuaState();

			 lua_getglobal(L_state, "PlayerBaseInfo");
			 tolua_pushnumber(L_state, msg->countdown());
			 tolua_pushnumber(L_state, msg->remain_times());
			 lua_pcall(L_state, 2, LUA_MULTRET, 0);
		 }
	}
}