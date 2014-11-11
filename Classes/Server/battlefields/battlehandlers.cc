#include "battlehandlers.h"

#include "../proto/protocodec.h"
#include "../proto/cs_battle.pb.h"
#include "../proto/cs_config.pb.h"
#include "../common/log.h"
#include "../config/battleconfig.h"
#ifndef FRONT
#include "../inter_service/interserver.h"
#endif

int battle_prepare_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSBattlePrepareReq* req = static_cast<const CSBattlePrepareReq*> (msg);
	if(req) {
		DEBUG_LOG("p %u prepare BATTLE :%d",pPlayer->id(),req->instance_id());
		CSBattlePrepareRsp rsp;
		std::set<std::pair<uint32_t, uint32_t> > monster_value_set;
		g_instance_config->pack_mapcontent(*(rsp.mutable_map_contents()), req->instance_id(), monster_value_set);
		std::set<std::pair<uint32_t, uint32_t> >::iterator itr = monster_value_set.begin();
		for(; itr!=monster_value_set.end(); ++itr) {
			CSMonsterValue_t* monster_value = rsp.add_monster_values();	
			g_monster_value_config->pack_monster_value(*monster_value, itr->first, itr->second);
		}
		pPlayer->send_msg(rsp);
	}
	return 0;
}

int battle_prepare_front_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSBattlePrepareRsp* req = static_cast<const CSBattlePrepareRsp*> (msg);
	if(req) {
		g_instance_config->unpack_mapcontent(req->map_contents());
		for(int i=0; i<req->monster_values_size() ; ++i) {
			g_monster_value_config->unpack_monster_value(req->monster_values(i));
		}
	}
	return 0;
}

int battle_create_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSBattleCreateReq *req = static_cast<const CSBattleCreateReq*> (msg);
	if (req) {
		DEBUG_LOG("p %u CREATE BATTLE:%d MPA:%d", pPlayer->id(), req->battle_id(), req->map_id());
		if(req->map_id()) {
			sInstance.transfer_next(pPlayer);
		} else {
			if (sInstance.create_instance(pPlayer, req->battle_id()) == 0) {
				sInstance.init_instance(pPlayer);
			}
		}
	//	sBattlefield.create_battle(pPlayer, req->battle_id());
	}
	return 0;
}

int battle_load_complete_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	sInstance.load_complete(pPlayer);
#ifndef FRONT
	sWBInstanceMgr.load_complete(pPlayer);
#endif
#ifndef FRONT
	sBBInstanceMgr.load_complete(pPlayer);
#endif
	return 0;
}

int battle_leave_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	KDEBUG_LOG(pPlayer->id(),"PLAYER LEAVE BATTLE ");
	sInstance.leave_instance(pPlayer);
#ifndef FRONT
	sWBInstanceMgr.leave_instance(pPlayer);
#endif
#ifndef FRONT
	sBBInstanceMgr.leave_instance(pPlayer);
#endif
	sBattlefield.leave_battle(pPlayer);
#ifndef FRONT
	sInterServer.leave_interserver(pPlayer);
#endif
	return 0;
}

int battle_move_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSBattleMoveReq * req = static_cast<const CSBattleMoveReq*> (msg);
	if (req) {
		sBattlefield.move(pPlayer, req);
	}
	return 0;
}

int player_attack_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSPlayerAttackReq * req = static_cast<const CSPlayerAttackReq*> (msg);
	if (req) {
		sBattlefield.player_attack(pPlayer, req);
	}
	return 0;
}

int skill_begin_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSSkillBeginReq * req = static_cast<const CSSkillBeginReq*> (msg);
	if (req) {
		sBattlefield.skill_begin(pPlayer, req);
	}
	return 0;
}

int player_hit_player_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSPlayerHitPlayerReq * req = static_cast<const CSPlayerHitPlayerReq*> (msg);
	if (req) {
		sBattlefield.player_attack(pPlayer, req);
	}
	return 0;
}

int battle_player_revival_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	sBattlefield.revival(pPlayer);
	return 0;
}

int player_hp_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSPlayerHPReq * req = static_cast<const CSPlayerHPReq*> (msg);
	if (req) {
		sInstance.player_hp_change(pPlayer, req->blood());
	}
	return 0;
}

int monster_die_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
	const CSMonsterDieReq * req = static_cast<const CSMonsterDieReq*> (msg);
	if (req && sInstance.in_pve(pPlayer)) {
		sBattlefield.monster_die(pPlayer, req);
	}
	return 0;
}

int wb_view_players_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
#ifndef FRONT
	sWBInstanceMgr.wb_view_players(pPlayer);
#endif
	return 0;
}

int wb_btl_player_revival_cmd(Player* pPlayer, const google::protobuf::Message* msg) {
#ifndef FRONT
	sBattlefield.wb_btl_revival(pPlayer);
#endif
	return 0;
}

int wb_battle_leave_cmd(Player* pPlayer, const google::protobuf::Message* msg){
	KDEBUG_LOG(pPlayer->id(),"PLAYER LEAVE WBOSS BATTLE ");
#ifndef FRONT
	sWBInstanceMgr.leave_instance(pPlayer);
	sBattlefield.wb_leave_battle(pPlayer);
#endif
	return 0;
}

int wb_btl_reward_cmd(Player* pPlayer, const google::protobuf::Message* msg)
{
#ifndef FRONT
   sWBInstanceMgr.wb_reward_req_with_boss_dead(pPlayer);
#endif
	return 0;
}


int bb_btl_leave_cmd(Player* pPlayer, const google::protobuf::Message* msg){
	KDEBUG_LOG(pPlayer->id(),"PLAYER LEAVE BABEL BATTLE ");
#ifndef FRONT
	sBBInstanceMgr.leave_instance(pPlayer);
	sBattlefield.bb_leave_battle(pPlayer);
#endif
	return 0;
}


