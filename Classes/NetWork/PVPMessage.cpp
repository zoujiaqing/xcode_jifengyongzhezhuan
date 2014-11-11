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
#include "PVPMessage.h"
#include "BoneNPCManager.h"
#include "LevelDefine.h"
#include "UserData.h"

#include "cocos2d.h"
#include "tolua++.h"
#include "CCLuaEngine.h"
#include "cs_basic.pb.h"
#include "cs_core.pb.h"
#include "cs_battle.pb.h"
#include "errcode.pb.h"

#include "MainMenuLayer.h"
#include "PVPLayer.h"
#include "Localization.h"

#include "OnlineNetworkManager.h"
#include "TalentLayer.h"
#include "AutoOnHook.h"
#include "ShopUILayer.h"
#include "BackPackLayer.h"
#include "AccountsLayer.h"
#include "MysteryLayer.h"
#include "MarketLayer.h"
#include "MysteryLayer.h"
#include "WaitingLayerManager.h"

USING_NS_CC;

PVPMessage::PVPMessage()
{
}

void PVPMessage::InitHandler(MessageHandler * messageHolder)
{
	m_pMessageHolder = messageHolder;
}


void PVPMessage::HandleMsgNewMail(NORMAL_MSG_PACKAGE* pkg)
{
    //CCLOG("HandleMsgNewMail\n");
    //TalentLayer::m_MailFlag = true;
    //
    //UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
    //if(layout)
    //{
    //    UIButton* button = layout->FindChildObjectByName<UIButton>("mailBtn");
    //    if(button)
    //    {
    //        if(TalentLayer::m_MailFlag)
    //        {
    //            button->showTipIcon();
    //            button->setVisible(true);
    //        }
    //        else
    //        {
    //            button->setVisible(false);
    //        }
    //    }
    //}
}

void PVPMessage::handleMsgGetRoom(NORMAL_MSG_PACKAGE* pkg)
{
    CCLOG("pvp message handleMsgGetRoom\n");
	/// clear all rooms
	int tag = MainMenuLayer::getLayerTagByName("PVPLayer");
	PVPLayer* layer = MainMenuLayer::getUILayerByTag<PVPLayer>(tag,false);
	if(layer == NULL)
	{
		return;
	}
	layer->ClearAllRooms();

  //  PVPLayer* pvp = GetPVPLayer();
    //if(!pvp)
      //  return;
    
	layer->ClearAllRooms();
    
	if (pkg)
	{
		m_lRooms.clear();
		CSPVPQueryRoomRsp* msg = dynamic_cast<CSPVPQueryRoomRsp*>(pkg->msg);
		for(int i = 0; i < msg->rooms_size(); i++)
		{
			const PVPRoomBase& room = msg->rooms(i);
			m_lRooms.push_back(room);
			/// show room list
			layer->AddNewRoom(room);
		}
	}
    
	/// call global script function
	if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPVPModule())
	{
		CCLuaEngine::defaultEngine()->executeGlobalFunction("UpdatePVPPage");
	}
}

void PVPMessage::sendAccountBar()
{
    unsigned int channelId = UserData::getUserId().getChannel();
    unsigned int userId = UserData::getUserId().getID();
    unsigned int regTime = UserData::getUserId().getRegTime();
    
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPlayerDieShowReq");
    CSPlayerDieShowReq* bodyMsg = (CSPlayerDieShowReq*)msg;
    //bodyMsg->set_userid(userId);
    //bodyMsg->set_channel_id(channelId);
    //bodyMsg->set_reg_time(regTime);
    /*bodyMsg->set_channel_id(channelId);
    bodyMsg->set_userid(userId);
    bodyMsg->set_reg_time(regTime);*/
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleMsgAccountBar(NORMAL_MSG_PACKAGE* pkg)
{
	CSPlayerDieShowRsp* bodyMsg = (CSPlayerDieShowRsp*)pkg->msg;
	unsigned int attr[3] = {0};
	unsigned int count = bodyMsg->values_size();
	if(count > 0)
	{
		for(int i =0 ;i < count;i++)
		{
			if(i < 3)
			{
				attr[i] = bodyMsg->values(i);
			}
		}
	}
	/*unsigned int attack = bodyMsg->attack(); 
    unsigned int defend = bodyMsg->defend();
    unsigned int live = bodyMsg->hp();*/
    
    MainMenuLayer* layer = GetMainMenuLayer();
    CCAssert(layer != NULL,"");
    int tag = layer->getLayerTagByName("AccountsLayer");
    AccountsLayer* accountLayer = (AccountsLayer*)layer->getChildByTag(tag);
    if(accountLayer != NULL)
        accountLayer->setBarValue(attr[0], attr[1], attr[2]);
}

void PVPMessage::pvp_getRoomList()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPQueryRoomReq");
    CSPVPQueryRoomReq* bodyMsg = (CSPVPQueryRoomReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::pvp_createRoom_1vs1()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPCreateRoomReq");
	CSPVPCreateRoomReq* bodyMsg = (CSPVPCreateRoomReq*)msg;
	const USERINFO& user = UserData::GetUserInfo();
    
    bool val = GetPVPLayer()->getsp();
    if(val)
	   bodyMsg->set_top_level(user.level);
    else
        bodyMsg->set_top_level(0);
    
    bodyMsg->set_type(1);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::pvp_createRoom_2vs2()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPCreateRoomReq");
	CSPVPCreateRoomReq* bodyMsg = (CSPVPCreateRoomReq*)msg;
	const USERINFO& user = UserData::GetUserInfo();
    
    bool val = GetPVPLayer()->getsp();
    if(val)
        bodyMsg->set_top_level(user.level);
    else
        bodyMsg->set_top_level(0);

    bodyMsg->set_type(2);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

/*
 * room closed by host
 */
void PVPMessage::handleMsgCloseRoom(NORMAL_MSG_PACKAGE* pkg)
{
	/// call global script function
	if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPVPModule())
	{
		CCLuaEngine::defaultEngine()->executeGlobalFunction("closePVPRoom");
	}
}

void PVPMessage::pvp_exitRoom()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPExitRoomReq");
	CSPVPExitRoomReq* bodyMsg = (CSPVPExitRoomReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::pvp_JoinRoom(int roomID)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPJoinRoomReq");
	CSPVPJoinRoomReq* bodyMsg = (CSPVPJoinRoomReq*)msg;
	bodyMsg->set_room_id(roomID);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleMsgJoinRoom(NORMAL_MSG_PACKAGE* pkg)
{
	m_lSeats.clear();
	CSPVPJoinRoomRsp* msg = (CSPVPJoinRoomRsp*)pkg->msg;
    
	int roomid = msg->room_id();
	int toplevel = msg->top_level();
    int type = msg->type();
    
	for(int i = 0; i < msg->seats_size(); i++)
	{
		const PVPSeatBase& seat = msg->seats(i);
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
				lua_getglobal(L_state, "pvp_PlayerOnSeat");
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
			lua_getglobal(L_state, "pvp_PlayerLeaveSeat");
			tolua_pushnumber(L_state, n);
			lua_pcall(L_state, 1, LUA_MULTRET, 0);
		}
	}
    
	/// call global script function
	if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPVPModule())
	{
		lua_getglobal(L_state, "pvp_EnterRoom");
		tolua_pushnumber(L_state, roomid);
		tolua_pushnumber(L_state, toplevel);
        tolua_pushnumber(L_state, type);
		lua_pcall(L_state, 3, LUA_MULTRET, 0);
	}
}

void PVPMessage::pvp_setReady(bool bReady)
{
	if (bReady)
	{
		Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPReadyReq");
		CSPVPReadyReq* bodyMsg = (CSPVPReadyReq*)msg;
		OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
	}
	else
	{
		Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPCancelReadyReq");
		CSPVPCancelReadyReq* bodyMsg = (CSPVPCancelReadyReq*)msg;
		OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
	}
}

/// start game now
void PVPMessage::pvp_startGame()
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPStartReq");
	CSPVPStartReq* bodyMsg = (CSPVPStartReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::pvp_kickOffGuest(int seat)
{
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPKickoffReq");
	CSPVPKickoffReq* bodyMsg = (CSPVPKickoffReq*)msg;
	bodyMsg->set_seat(seat);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::pvp_changeSeat(int seat)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSPVPChangeSeatReq");
	CSPVPKickoffReq* bodyMsg = (CSPVPKickoffReq*)msg;
	bodyMsg->set_seat(seat);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

/////////////////////Talent/////////////////////
void  PVPMessage::getTalentList()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSQueryDowerReq");
	CSQueryDowerReq* bodyMsg = (CSQueryDowerReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void  PVPMessage::getOneTalent(int talent_id, int level)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSDowerInfoReq");
	CSDowerInfoReq* bodyMsg = (CSDowerInfoReq*)msg;
    bodyMsg->set_id(talent_id);
    bodyMsg->set_level(level);
    
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::levelUpTalent(int talent_id)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSDowerReq");
	CSDowerReq* bodyMsg = (CSDowerReq*)msg;
    bodyMsg->set_id(talent_id);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleMsgTalentList(NORMAL_MSG_PACKAGE* pkg)
{
    TalentLayer *layer  = GetTalentLayer();
    if(!layer)
        return;
    
    layer->clearScrollist();
    CSQueryDowerRsp* msg = (CSQueryDowerRsp*)pkg->msg;
    int downsize = msg->dower_size();
    int timeout = msg->timeout();
    
    GetTalentLayer()->startCD(timeout);
    
    int first_talentId, first_level;
    
    for(int i=0; i<downsize; i++)
    {
        CSQueryDowerRsp_Dower down =  msg->dower(i);
        unsigned int id = down.id();
        unsigned int level = down.level();
        unsigned int plus = down.plus();
        unsigned int each_plus = down.each_plus();
        
        layer->addNewTalent(id, level, plus);
        layer->seteachplus(each_plus);
        
        if(i==0)
        {
            first_talentId = id;
            first_level = level;
            getOneTalent(first_talentId, first_level);
            layer->setcurTalentID(first_talentId);
        }
    }
}

void PVPMessage::HandleMsgOneTalent(NORMAL_MSG_PACKAGE* pkg)
{
    CSDowerInfoRsp* msg = (CSDowerInfoRsp*)pkg->msg;
    unsigned int id = msg->id();
    unsigned int level = msg->level();
    unsigned int plus = msg->plus();
    unsigned int next_exploit = msg->next_exploit();
    unsigned int next_time = msg->next_time();
    
    unsigned int diff_physical_attack = msg->diff_physical_attack();
    unsigned int diff_magic_attack = msg->diff_magic_attack();
    unsigned int diff_skill_attack = msg->diff_skill_attack();
    unsigned int diff_physical_defence = msg->diff_physical_defence();
    unsigned int diff_magic_defence = msg->diff_magic_defence();
    unsigned int diff_skill_defence = msg->diff_skill_defence();
    unsigned int diff_hp = msg->diff_hp();
    
    GetTalentLayer()->showOne(
                  id,
                  level,
                  plus,
                  next_exploit,
                  next_time,
                              
                  diff_physical_attack,
                  diff_magic_attack,
                  diff_skill_attack,
                              
                  diff_physical_defence,
                  diff_magic_defence,
                  diff_skill_defence,
                  diff_hp);
}

void PVPMessage::HandleMsgLevelUpTalent(NORMAL_MSG_PACKAGE* pkg)
{
    CSDowerRsp* msg = (CSDowerRsp*)pkg->msg;
    unsigned int level = msg->level();
    unsigned int timeout = msg->timeout();
    unsigned int next_exploit = msg->next_exploit();
    unsigned int plus = msg->plus();
    GetTalentLayer()->updateLevel(level, plus, next_exploit);
    GetTalentLayer()->startCD(timeout);
}


////////////////////////////AutoOnHook//////////////////////////////
void PVPMessage::sendCSABMonsterReq(unsigned int instance_id, unsigned int instance_type)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSABMonsterReq");
	CSABMonsterReq* bodyMsg = (CSABMonsterReq*)msg;
    //required uint32 instance_id = 1; // 副本ID
    bodyMsg->set_id(instance_id);
    if(instance_type != 0)
        bodyMsg->set_instance_type(instance_type);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::sendCSAutoBattleReq(unsigned int instance_id, unsigned int param)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSAutoBattleReq");
	CSAutoBattleReq* bodyMsg = (CSAutoBattleReq*)msg;
    //required uint32 instance_id = 1; // 副本ID
	//required uint32 turn_num 	= 2; // 轮数(=0时表示消耗所有体力值)
    bodyMsg->set_id(instance_id);
    bodyMsg->set_param(param);
	CCLog("================================");
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::sendCSCancelAutoBattleReq()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSCancelAutoBattleReq");
	CSCancelAutoBattleReq* bodyMsg = (CSCancelAutoBattleReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::sendSpeedUpReq(int diamand_id, int freshWay)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSExchangeParameterReq");
	CSExchangeParameterReq* bodyMsg = (CSExchangeParameterReq*)msg;
    bodyMsg->set_id(diamand_id);
	bodyMsg->add_param(freshWay);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleMsgCSABMonsterRsp(NORMAL_MSG_PACKAGE* pkg)
{
    AutoOnHook* hook = GetAutoOnHook();
    if(!hook)
        return;
    
    CSABMonsterRsp* msg = (CSABMonsterRsp*)pkg->msg;

    unsigned int instance_id = msg->id();
    unsigned int type = msg->instance_type();
    hook->initAutoOnHook(instance_id, (PB_INSTANCE_TYPE_T)type);

    
    int monsize = msg->mon_infos_size();
    
    std::vector<OnHookA2> vecs;
    for(int i=0; i<monsize; i++)
    {
        CSABMonsterRsp_MonInfo info = msg->mon_infos(i);
        //required uint32 mon_id	= 1; // 怪物id
		//required uint32 mon_num	= 2; // 怪物num
        unsigned int mon_id = info.mon_id();
        unsigned int mon_num = info.mon_num();
        OnHookA2 a2;
        a2.mon_id = mon_id;
        a2.mon_num = mon_num;
        vecs.push_back(a2);
    }
    GetAutoOnHook()->setScrollist(vecs);
}

void PVPMessage::HandleMsgCSABRewardRsp(NORMAL_MSG_PACKAGE* pkg)
{
    AutoOnHook* hook = GetAutoOnHook();
    if(!hook){
        CSABRewardRsp* msg = (CSABRewardRsp*)pkg->msg;
        const CSReward reward = msg->rewards();
        int size = reward.rewards_size();

        for(int item_index=0; item_index<size; item_index++)
        {
            autoOnHookReward r;
            CSReward_RewardInfo info = reward.rewards(item_index);
            r.item_id = info.item_id();
            r.item_level = info.item_level();
            r.item_num = info.item_num();
            g_rewards.push_back(r);
           
        }

        return;
    }
    
    CSABRewardRsp* msg = (CSABRewardRsp*)pkg->msg;
    const CSReward reward = msg->rewards();

    unsigned int instance_id = msg->id();
    unsigned int type = msg->instance_type();
    hook->initAutoOnHook(instance_id, (PB_INSTANCE_TYPE_T)type);
    
	
	//扫荡后金币、钻石等奖励以物品形式显示在物品栏
	//by arthurwang 14.10.13
	/*
    unsigned int diamand_number = reward.add_diamond();
    unsigned int coin_number = reward.add_coin();
    unsigned int exploit_number = reward.add_exploit();
    unsigned int exp_number = reward.add_exp();
    GetAutoOnHook()->setIconAllGet(diamand_number,coin_number,exploit_number,exp_number);
	*/
	hook->ClearReward();

	if(reward.add_coin() > 0){
		hook->SetReward(reward.add_coin(), 416001 , 1);
	}
	if(reward.add_diamond() > 0){
		hook->SetReward(reward.add_diamond(), 416002 , 1);
	}
	if(reward.add_exp() > 0){
		hook->SetReward(reward.add_exp(), 416003 , 1);
	}
	if(reward.add_exploit() > 0){
		hook->SetReward(reward.add_exploit(), 416004 , 1);
	}
	if(reward.add_phase_exp() > 0){
		hook->SetReward(reward.add_phase_exp(), 416019 , 1);
	}

    int size = reward.rewards_size();
    
    CCLOG("hand reward size:%d\n", size);
    
   // hook->clearVecIDs();
    for(int item_index=0; item_index<size; item_index++)
    {
        CSReward_RewardInfo info = reward.rewards(item_index);
        unsigned int item_id = info.item_id();
        unsigned int item_level = info.item_level();
        unsigned int item_num = info.item_num();
        hook->SetReward(item_num, item_id, item_level); //fix BUG #101335
        CCLOG("item_id:%d\n", item_id);
    }
	if (hook->getIsProgressDone() && !hook->getIsShownReward())
	{
		hook->ShowReward();
	}

	hook->setIsWaitingReward(false);

	hook->RefreshTimesAndStamina(NULL);
}



void PVPMessage::HandleMsgAutoBattleRsp(NORMAL_MSG_PACKAGE* pkg)
{
    //required uint32 ret			= 1; // 0 成功 !0 失败(1:上个战斗自动战斗未完成 2:地图不能自动战斗 3...)
	//required uint32 cd			= 2; // 自动战斗完成需要的时间
	//required uint32 instance_id	= 3; // 自动战斗副本
    
    AutoOnHook* hook = GetAutoOnHook();
    CSAutoBattleRsp* msg = (CSAutoBattleRsp*)pkg->msg;

    if(!hook){
        g_rcv_battle_rsp = true;
        missing_ret = msg->ret();
        missing_instance_id = msg->id();
        missing_type = msg->instance_type();
        return;
    }
    
    unsigned int ret = msg->ret();
    //unsigned int cd = msg->cd();
    unsigned int instance_id = msg->id();
    unsigned int type = msg->instance_type();
    hook->initAutoOnHook(instance_id, (PB_INSTANCE_TYPE_T)type);
    if(ret != 0)
    {
        CCLOG("Error: auto battle ret a fail value");
    }
	hook->MsgAutoBattleRsp(ret);
    
    #if 0
    if(ret == 0)
        GetAutoOnHook()->startCD(cd);
    if(ret == 1)
        GetAutoOnHook()->startCD(cd);
    #endif
    //hook->hideCloseBtn();
}

void PVPMessage::HandleMsgCSCancelAutoBattleRsp(NORMAL_MSG_PACKAGE* pkg)
{
}

void PVPMessage::HandleMsgSpeedUpRsp(NORMAL_MSG_PACKAGE* pkg)
{
    CSExchangeParameterRsp* msg = (CSExchangeParameterRsp*)pkg->msg;
    int suc = msg->succ();
    int id = msg->id();
    
    if(suc && id == 10100) // speed talent
    {
       TalentLayer* layer = GetTalentLayer();
       if(layer)
       {
           layer->startCD(0);
       }
    }
    else if(suc && (id == 10400 || id == 10500 || id == 10501 )) //speed auto hook
    {
        AutoOnHook* layer = GetAutoOnHook();
        if(layer)
        {
            layer->startCD(0);
        }
    }
    else if(suc && id == 10800) //speed shop
    {
        SceneLayer* sceneLayer = GameManager::Get()->GetSceneLayer();
        if (sceneLayer != NULL)
        {
            MainMenuLayer* layer = sceneLayer->getMainMenuLayer();
            if (layer != NULL)
            {
                MarketLayer* marketLayer = dynamic_cast<MarketLayer*>(layer->getMarketLayer());
                if (marketLayer != NULL)
                {
                    MysteryLayer* mysteryLayer = dynamic_cast<MysteryLayer*>(marketLayer->getCurrentLayer());
                    if (mysteryLayer)
                    {
                        mysteryLayer->TryRefreshList();
                    }
                }
            }
        }
    }
}

void PVPMessage::HandleMsgCSAttributesRsp(NORMAL_MSG_PACKAGE* pkg)
{
}

////////////////ShopUI///////////////
void PVPMessage::sendShopItems()
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetShopItemsReq");
	CSGetShopItemsReq* bodyMsg = (CSGetShopItemsReq*)msg;
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
    WaitingLayerManager::Get()->PushOneMessage("CSGetShopItemsReq");
}

void PVPMessage::buyShopItem(int index)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBuyShopItemReq");
	CSBuyShopItemReq* bodyMsg = (CSBuyShopItemReq*)msg;
    bodyMsg->set_index(index);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleMsgShopItems(NORMAL_MSG_PACKAGE* pkg)
{
    CSGetShopItemsRsp* msg = (CSGetShopItemsRsp*)pkg->msg;
    int size = msg->items_size();
     std::vector<shopItem> items;
    for(int i = 0; i < size; i++)
    {
        ShopItemInfo info = msg->items(i);
        ShopItemInfo_Item to_item = info.to_item();
        ShopItemInfo_Item from_item = info.from_item();

        shopItem  item;

        item.to_id = to_item.item_id();
        item.to_num= to_item.item_num();

        item.from_id = from_item.item_id();
        item.from_num = from_item.item_num();

        item.isBuy = info.is_buy();
        item.index = info.index();

        items.push_back(item);
    }
    
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
    ShopUILayer *shopUI = layer->getpShopUILayer();
    if(shopUI)
    {
        shopUI->setupShopList(items);
    }

    SceneLayer* sceneLayer = GameManager::Get()->GetSceneLayer();
    if (sceneLayer != NULL)
    {
        MainMenuLayer* layer = sceneLayer->getMainMenuLayer();
        if (layer != NULL)
        {
            MarketLayer* marketLayer = dynamic_cast<MarketLayer*>(layer->getMarketLayer());
            if (marketLayer != NULL)
            {
                MysteryLayer* mysteryLayer = dynamic_cast<MysteryLayer*>(marketLayer->getCurrentLayer());
                if (mysteryLayer)
                {
                    mysteryLayer->InitLayer(items);
                }
            }
        }
    }
}

void PVPMessage::HandleBuyShopItem(NORMAL_MSG_PACKAGE *pkg)
{
    CSBuyShopItemRsp* msg = (CSBuyShopItemRsp*)pkg->msg;
    bool suc = msg->succ();
    int index = msg->index();
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
    ShopUILayer *shopUI = layer->getpShopUILayer();
    if(shopUI)
       shopUI->buyshop(index,suc);

    SceneLayer* sceneLayer = GameManager::Get()->GetSceneLayer();
    if (sceneLayer != NULL)
    {
        MainMenuLayer* layer = sceneLayer->getMainMenuLayer();
        if (layer != NULL)
        {
            MarketLayer* marketLayer = dynamic_cast<MarketLayer*>(layer->getMarketLayer());
            if (marketLayer != NULL)
            {
                MysteryLayer* mysteryLayer = dynamic_cast<MysteryLayer*>(marketLayer->getCurrentLayer());
                if (mysteryLayer)
                {
                    mysteryLayer->buyItem(index, suc);
                }
            }
        }
    }
}

void PVPMessage::sendToShop(int pos)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSRepoRecycleBinItemReq");
	CSRepoRecycleBinItemReq* bodyMsg = (CSRepoRecycleBinItemReq*)msg;
    bodyMsg->set_pos(pos);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleSendToShop(NORMAL_MSG_PACKAGE* pkg)
{
    CSRepoRecycleBinItemRsp* msg = (CSRepoRecycleBinItemRsp*)pkg->msg;
    int coin = msg->coin();
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
    ShopUILayer *shopUI = layer->getpShopUILayer();
    shopUI->floatCoin(coin);
    shopUI->reconstructCallBuyScrollist();
}

void PVPMessage::sendToBag(int pos)
{
	CCAssert(pos < 2000 && pos > 0,"pos is error");
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSItemMoveToRecycleBinReq");
	CSItemMoveToRecycleBinReq* bodyMsg = (CSItemMoveToRecycleBinReq*)msg;
    bodyMsg->set_pos(pos);
	OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
}

void PVPMessage::HandleSendToBag(NORMAL_MSG_PACKAGE* pkg)
{
    CSItemMoveToRecycleBinRsp* msg = (CSItemMoveToRecycleBinRsp*)pkg->msg;
    int coin = msg->coin();
	int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
	BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
    ShopUILayer *shopUI = layer->getpShopUILayer();
	if (shopUI)
	{
		shopUI->floatCoin(coin);
	}    
}

void PVPMessage::HandlerMessage(string cmd, NORMAL_MSG_PACKAGE* package)
{
    if(cmd == "CSPVPQueryRoomRsp")
	{
		handleMsgGetRoom(package);
	}
	else if (cmd == "CSPVPCloseRoomRsp")
	{
		handleMsgCloseRoom(package);
	}
	else if (cmd == "CSPVPJoinRoomRsp")
	{
		HandleMsgJoinRoom(package);
	}
	else if (cmd == "CSPVPKickoffRsp")
	{
		///call global script function
		if (GetMainMenuLayer() && GetMainMenuLayer()->IsInPVPModule())
		{
			CCLuaEngine::defaultEngine()->executeGlobalFunction("pvp_kickOffFromRoom");
		}
	}
	else if (cmd == "CSPVPJoinRoomFailureRsp")
	{
		string strMsgError = Localizatioin::getLocalization("M_JOINROOM_FAIL");;
		ShowMessage(strMsgError.c_str());
	}
	else if (cmd == "CSPVPBaseInfoRsp")
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
    /////////////talent message Rsp/////////////////////
    else if(cmd == "CSQueryDowerRsp")
    {
        HandleMsgTalentList(package);
    }
    else if(cmd == "CSDowerInfoRsp")
    {
        HandleMsgOneTalent(package);
    }
    else if(cmd == "CSDowerRsp")
    {
        HandleMsgLevelUpTalent(package);
    }
    else if(cmd == "CSABMonsterRsp")
    {
        HandleMsgCSABMonsterRsp(package);
    }
    else if(cmd == "CSAutoBattleRsp")
    {
        HandleMsgAutoBattleRsp(package);
    }
    else if(cmd == "CSABRewardRsp")
    {
        HandleMsgCSABRewardRsp(package);
    }
    else if(cmd == "CSCancelAutoBattleRsp")
    {
        HandleMsgCSCancelAutoBattleRsp(package);
    }
    else if(cmd == "CSExchangeParameterRsp")
    {
        HandleMsgSpeedUpRsp(package);
    }
    else if(cmd == "CSAttributesRsp")
    {
        HandleMsgCSAttributesRsp(package);
    }
    else if(cmd == "CSGetShopItemsRsp")
    {
        HandleMsgShopItems(package);
    }
    else if(cmd == "CSBuyShopItemRsp")
    {
        HandleBuyShopItem(package);
    }
    else if(cmd == "CSItemMoveToRecycleBinRsp")
    {
        HandleSendToBag(package);
    }
    else if(cmd == "CSRepoRecycleBinItemRsp")
    {
        HandleSendToShop(package);
    }
    else if(cmd == "CSPlayerDieShowRsp")
    {
        HandleMsgAccountBar(package);
    }
    else if(cmd == "CSNewMailRsp")
    {
        HandleMsgNewMail(package);
    }
}






