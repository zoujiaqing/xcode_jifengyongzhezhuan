// project-specific headers go here
#include "battlefield.h"

#ifndef FRONT
#include "../stat/stat.h"
#include "../msg_dispatcher/msg_dispatcher.h"
#include "wb_instance.h"
#else
#include "LuaTinkerManager.h"
#endif
#include "../base/Time.hpp"
#include "../proto/cs_battle.pb.h"
#include "../config/battleconfig.h"
#include "../common/common.h"

#define GAMEBEGIN_BATTLE (280000)

using namespace google::protobuf;

Battlefield::Battlefield(Player* pPlayer, uint32_t battle_id, uint32_t map_id, bool transfer) :
	base::Object<uint32_t>(map_id),
	m_battle_id(battle_id),
	owner(pPlayer),
	m_complete(false),
	m_transfer(transfer),
	m_dead_mon_num(0)
{
	init_map(map_id);
	if(pPlayer) {
		player_join(pPlayer);
	}
	sig_obj_post_decrease.connect(this, &Battlefield::on_player_leave);
	monsters_.sig_send_msg.connect(this, &Battlefield::send_to_players);
}

void Battlefield::player_join(Player* pPlayer) 
{
#ifndef FRONT
	if(m_battle_id == GAMEBEGIN_BATTLE ) {
		pPlayer->game_first_start();
	}
#endif
	//firstly, leave last map
	pPlayer->leave_map();

	if(m_hero_born.size() > pPlayer->get_group()) {
		pPlayer->m_pos = m_hero_born[pPlayer->get_group()];
	} else if( !m_hero_born.empty() ) {
		pPlayer->m_pos = m_hero_born[0];
	} else {
		ERROR_LOG("p %u haven't pos to join battle: %d", pPlayer->id(), get_battle_id());
	}
	if(!m_transfer) {
		pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
	}
	if(pPlayer != owner) {
		CSBattleUserJoinRsp rsp;
		BasicUserInfo * info = rsp.add_userinfo();
		pPlayer->pack_basic_info(info);
		info->set_battle_side(pPlayer->get_group());
		pPlayer->pack_basic_battle_info(info);
		DEBUG_LOG("p %u info sended to the other players in the map ",pPlayer->id());
		send_to_players(rsp);
	}
	add_obj(pPlayer);
	pPlayer->sig_dead.connect(this, &Battlefield::on_player_dead);
	pPlayer->m_buffmanager->sig_undo_buff.connect(this, &Battlefield::on_buffer_finish);
}
	
void Battlefield::load_complete(Player* pPlayer)
{
	Player* pPlayer_join = next_ref(NULL);
	CSBattleUserJoinRsp rsp;
	while(pPlayer_join) {
		if(pPlayer != pPlayer_join) {
			BasicUserInfo * info = rsp.add_userinfo();
			pPlayer_join->pack_basic_info(info);
			info->set_battle_side(pPlayer_join->get_group());
			pPlayer_join->pack_basic_battle_info(info);
		}
		pPlayer_join = next_ref(pPlayer_join);
	}
	pPlayer->send_msg(rsp);
}

void Battlefield::attack(Player *pPlayer, const CSPlayerAttackReq * req) {
/*Todo must check the available of action*/
	CSPlayerAttackRsp attack_rsp;
	CSMonsterBloodRsp blood_rsp;
	CSMonsterDieRsp rsp;

/*
message CSPlayerAttackReq { //用户打击怪物
    required uint32 userid = 1; //用户ID
    required CSPoint start_pos = 2; //开始位置
    required CSPoint start_direct = 3; //开始方向
    required CSPoint end_pos = 4; //结束位置
    required CSPoint end_direct = 5; //结束方向
    required uint32 action_id = 6; //动作ID
    repeated AttackMonsterInfo monsters = 7; //被攻击怪物信息
}
 
*/
	blood_rsp.set_userid(pPlayer->id());
	blood_rsp.set_reg_time(pPlayer->regtime());
	blood_rsp.set_channel_id(pPlayer->channelid());
	
	pPlayer->m_pos = TileWorld<>::Point(req->end_pos().xpos(), req->end_pos().ypos());

	attack_rsp.set_userid(req->userid());
	attack_rsp.set_reg_time(req->reg_time());
	attack_rsp.set_channel_id(req->channel_id());

	CSPoint* start_point = attack_rsp.mutable_start_pos();
	start_point->set_xpos(req->start_pos().xpos());
	start_point->set_ypos(req->start_pos().ypos());

	CSPoint* start_direct = attack_rsp.mutable_start_direct();
	start_direct->set_xpos(req->start_direct().xpos());
	start_direct->set_ypos(req->start_direct().ypos());
	
	CSPoint* end_point = attack_rsp.mutable_end_pos();
	end_point->set_xpos(req->end_pos().xpos());
	end_point->set_ypos(req->end_pos().ypos());
	
	CSPoint* end_direct = attack_rsp.mutable_end_direct();
	end_direct->set_xpos(req->end_direct().xpos());
	end_direct->set_ypos(req->end_direct().ypos());

	attack_rsp.set_action_id(req->action_id());
	
	for(int i =0; i<req->monsters_size(); i++) {
		AttackMonsterInfo info = req->monsters(i);
		*attack_rsp.mutable_monsters() = req->monsters();
		Monster *pmonster = monsters_.get_ref_by_key(player_key_t(info.id(), 0, 0));
		if(pmonster) {
			if(!pmonster->is_boss()) {
				pmonster->under_attack_back( TileWorld<>::Point(info.start_pos().xpos(), info.start_pos().ypos()) , TileWorld<>::Point(info.pos().xpos(), info.pos().ypos()));
			}
			uint32_t real_hurt = pPlayer->check_attack_hurt(req->action_id(), pmonster->m_fightprop, info.blood());
			pmonster->under_attack(pPlayer, real_hurt);
			CSAttackMonsterBackInfo* rsp_info = blood_rsp.add_monsters();
			rsp_info->set_blood(pmonster->m_prop.health_point);
			rsp_info->set_monster_type(pmonster->get_type_id());
			rsp_info->set_elite(pmonster->is_elite());
			rsp_info->set_id(pmonster->get_id().id());
			DEBUG_LOG("p %u ATTACK MONSTER:%u HIT:%d HEALTH:%d POS:%d %d", pPlayer->id(), pmonster->get_id().id(), real_hurt, pmonster->m_prop.health_point, info.pos().xpos(), info.pos().ypos());
			if(!pmonster->m_prop.health_point) {
				rsp.add_monsters(pmonster->get_id().id());				
				m_dead_mon_num++;
				pmonster->safe_release();
			}
		} else {
			ERROR_LOG("p %u ERROR ATTACK MONSTER %u",pPlayer->id(), info.id());
		}
	}
	send_to_players(attack_rsp);
	send_to_players(blood_rsp);
	send_to_players(rsp);
}
	
void Battlefield::skill_begin(Player *pPlayer, const CSSkillBeginReq * req) {
/*
message CSSkillBeginReq {
    required uint32 userid = 1;
    required CSPoint pos   = 2;
    required CSPoint target_pos = 3;
    required float   direct_x   = 4;
    required float   direct_y   = 5;
    required bool    aim        = 6;
}
*/
	CSSkillBeginRsp rsp;
	uint32_t entity_pos = 0;
	if(req->entity_pos()){
		entity_pos = req->entity_pos();
	}
	rsp.set_action_id(req->action_id());
	rsp.set_userid(req->userid());
	rsp.set_reg_time(req->reg_time());
	rsp.set_channel_id(req->channel_id());
	CSPoint* pos = rsp.mutable_pos();
	pos->set_xpos(req->pos().xpos());
	pos->set_ypos(req->pos().ypos());
	rsp.set_start_dir_x(req->start_dir_x());
	rsp.set_start_dir_y(req->start_dir_y());
	CSPoint* target_pos  = rsp.mutable_target_pos();
	target_pos->set_xpos(req->target_pos().xpos());
	target_pos->set_ypos(req->target_pos().ypos());
	rsp.set_direct_x(req->direct_x());
	rsp.set_direct_y(req->direct_y());
	rsp.set_aim(req->aim());
	rsp.set_entity_pos(req->entity_pos());

	uint32_t skill_id = req->action_id();
	if(g_hero_skill_config->is_buff(skill_id) || g_hero_skill_config->is_invincible(skill_id)) {
		pPlayer->add_buff(skill_id);
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		send_to_players(rsp);	
	} else if(entity_pos && g_fairy_skill_config->is_buff(skill_id)) {

#ifndef FRONT
		DEBUG_LOG("CAR DEBUG PLAYER %u FAIRY USE SKILL %u",req->userid(),skill_id);
		uint32_t skill_attr = pPlayer->get_fairy_attr(entity_pos,ITEM_ATTR_SKIATK);
		DEBUG_LOG("CAR DEBUG FAIRY SKILL ATTACK ATTR %u",skill_attr);
#else
		uint32_t skill_attr = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerAttackFairyByPos",entity_pos);
#endif
		CSBasicUserBattleInfoRsp rsp;
		pPlayer->add_buff(skill_id,skill_attr);
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		send_to_players(rsp);
		uint32_t blood = g_fairy_skill_config->add_blood(skill_id,skill_attr);
		if(blood) {
			CSBattleUserAddBloodRsp blood_rsp;
			blood_rsp.set_id(pPlayer->id());
			blood_rsp.set_reg_time(pPlayer->regtime());
			blood_rsp.set_channel_id(pPlayer->channelid());
			blood_rsp.set_blood(blood);	
			send_to_players(blood_rsp);
		}
	}
	send_to_players(rsp);
}
	
void Battlefield::on_buffer_finish(Entity* entity) {
	Player *pPlayer = dynamic_cast<Player*>(entity);
	if(pPlayer) {
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		send_to_players(rsp);
	}
#ifndef FRONT   
	AIPlayer * ai = dynamic_cast<AIPlayer*>(entity);
	if(ai) {
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(ai->id);
		rsp.set_reg_time(ai->regtime);
		rsp.set_channel_id(ai->channelid);
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		ai->pack_basic_battle_info(info);
		send_to_players(rsp);
	}
#endif
}
	
void Battlefield::move(Player* pPlayer, const CSBattleMoveReq * req) {
/*Todo must check the available of move*/
	TileWorld<>::Point pos(req->pos().xpos(), req->pos().ypos());
	if(!m_tile_world->is_pos_walkable(pos)) {
		ERROR_LOG("PLAYER:%u CAN'T MOVE TO UNWALKALBE POS:%d %d", pPlayer->id(), pos.x, pos.y);
		return; 
	};
	pPlayer->m_pos.x = req->pos().xpos();
	pPlayer->m_pos.y = req->pos().ypos(); 
	pPlayer->m_direct.x = req->direct().xpos();
	pPlayer->m_direct.y = req->direct().ypos();
	//DEBUG_LOG("PLAYER %u MOVE TO: %d %d", pPlayer->id(),pPlayer->m_pos.x, pPlayer->m_pos.y);
	CSBattleMoveRsp rsp;
	rsp.set_id(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	CSPoint* one_point =rsp.mutable_pos();
	one_point->set_xpos(pPlayer->m_pos.x);
	one_point->set_ypos(pPlayer->m_pos.y);
	CSPoint* one_direct =rsp.mutable_direct();
	one_direct->set_xpos(pPlayer->m_direct.x);
	one_direct->set_ypos(pPlayer->m_direct.y);
	rsp.set_time(req->time());
	send_to_players(rsp);
}
	
void Battlefield::monster_die(Player* pPlayer, const CSMonsterDieReq *req) {
	for(int i =0; i<req->monsters_size(); i++) {
		uint32_t id = req->monsters(i);
		Monster *pmonster = monsters_.get_ref_by_key(player_key_t(id, 0, 0));
		if(pmonster) {
			m_dead_mon_num++;
			pmonster->safe_release();
		} else {
			ERROR_LOG("USER :%u ERROR ATTACK MONSTER %u",pPlayer->id(), id);
		}
	}
	if(monster_clear()) {
		if(!get_next_map_id()) {
			safe_release();
		} else {
			sGloble.Timer.add_event(*this, &Battlefield::server_update, base::Time(), UPDATETIME, -1);
		}
	}
}

int Battlefield::update() {
	if(monster_clear() && !get_next_map_id()) {
		CSMonsterDieRsp rsp;
		Monster* monster = monsters_.next_ref(NULL);
		while(monster != NULL) {
			rsp.add_monsters(monster->get_id().id());				
			monster = monsters_.next_ref(monster);
		}
		send_to_players(rsp);
		safe_release();
		return 0;
	}
	if(empty()) {
		safe_release();
		return 0;
	}
	check_wait_group();
	update_effect();	
	CSMonsterMoveRsp move_rsp;
	CSMonsterAttackRsp attack_rsp;
	CSMonsterEffectRsp effect_rsp;
	monsters_.update(&move_rsp, &attack_rsp, &effect_rsp);
	if(move_rsp.moves_size()) {
		send_to_players(move_rsp);
	}
	if(attack_rsp.attacks_size()) {
		send_to_players(attack_rsp);
	}
	if(effect_rsp.effects_size()) {
		send_to_players(effect_rsp);
	}
	CSBuffEffectRsp buff_effect_rsp;
	monsters_.do_buff(&buff_effect_rsp);
	if(buff_effect_rsp.effects_size()) {
		send_to_players(buff_effect_rsp);
	}
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead())
			monsters_.try_locking(pPlayer);
		pPlayer = next_ref(pPlayer);
	}
	return 0;
}
	
void Battlefield::update_effect() {
	m_effect_manager.update();
	m_effect_manager.effect_battle(this);
	CSMonsterHitRsp hit_rsp;
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead()) 
			m_effect_manager.effect(pPlayer, &hit_rsp);
		pPlayer = next_ref(pPlayer);
	}
	if(hit_rsp.users_size()) {
		send_to_players(hit_rsp);
	}
}
	
void Battlefield::on_player_leave(base::Manager<Player>*, Player* pPlayer) {
	DEBUG_LOG("p %u LEAVE", pPlayer->id());
	if(owner == pPlayer) {
		owner = NULL;
	}
	CSBattleUserLeaveRsp rsp;
	rsp.set_userid(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	send_to_players_except(rsp, pPlayer);
	pPlayer->sig_leave_battle(pPlayer);
}

void Battlefield::on_player_dead(Entity* entity)
{
#ifndef FRONT
	Player* pPlayer = static_cast<Player*> (entity);
	if(pPlayer) {
		DEBUG_LOG("CAR DEBUG PLAYER %u DEAD", pPlayer->id());
		CSBattlePlayerDelRsp rsp;
		rsp.set_userid(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		uint32_t times= pPlayer->m_attribute_list.get<uint32_t>("revival_times");
		if(times < 3) {
			rsp.set_exchange_id(pPlayer->m_attribute_list.get<uint32_t>("revival_times")+30700);
		} else {
			rsp.set_exchange_id(0);
		}
		send_to_players(rsp);
	}
#endif
}

void Battlefield::send_to_players(const Message& msg) {
	Player* pPlayer = next_ref(NULL);
	while (pPlayer) {
		pPlayer->send_msg(msg);
		pPlayer = next_ref(pPlayer);
	}
}
	
void Battlefield::send_to_players_except(const google::protobuf::Message& msg, Player* p) {
	Player* pPlayer = next_ref(NULL);
	while (pPlayer) {
		if(p != pPlayer) {
			pPlayer->send_msg(msg);
		}
		pPlayer = next_ref(pPlayer);
	}
}

void Battlefield::init_map(uint32_t map_id) {
	MapContent_t* mapcontent  = g_instance_config->get_mapcontent_info(m_battle_id, map_id);
	m_tile_world = g_tile_config->get_tile_info(mapcontent->map_id);
	if (!m_tile_world) {
		ERROR_RETURN_VOID("ASSERT not found m_tile_world! map_id:%u", mapcontent->map_id);
	}
	std::map<uint32_t, MapMonsterGroup_t>::iterator itr = mapcontent->monster_group.begin();
	for(; itr!=mapcontent->monster_group.end(); ++itr) {
		if(itr->second.refresh_time == 0) {
			std::vector<MapMonster_t>::iterator monster_itr = itr->second.monsters.begin();
			for(; monster_itr != itr->second.monsters.end(); ++ monster_itr) {
				monsters_.add_monster(&(*monster_itr), m_tile_world, &m_effect_manager );
			}
		}
	}
	m_hero_born = mapcontent -> birth_place_gen;
	m_portal = mapcontent -> trans_place;
	m_map_id = mapcontent -> map_id;
	m_next_map_id = mapcontent->trans_des;
	m_monster_born = mapcontent -> monster_birth;
}
	
void Battlefield::check_wait_group() {
	while(true) {
		uint32_t num = monsters_.size();
		if(num <= 10 && !m_wait_group.empty()) {
				MapMonsterGroup_t group = *(m_wait_group.begin());
				std::vector<MapMonster_t>::iterator monster_itr = group.monsters.begin();
				CSBattleMonsterCreateRsp rsp;
				for(; monster_itr != group.monsters.end();) {
					monster_itr->raid_id +=group.times* 1000000;
					Monster * monster = monsters_.add_monster(&(*monster_itr), m_tile_world, &m_effect_manager );
					num ++;
					if(monster) {
				/*temp code 10000000*/
						monster->m_fightprop.vision = 10000000; 
						MonsterInfo* monsterinfo = rsp.add_monsterinfo();
						monsterinfo->set_id(monster->get_id().id());
						monsterinfo->set_type_id(monster->get_type_id());
						monsterinfo->set_level(monster->get_level());
						monsterinfo->set_blood(monster->m_prop.health_point);
						monsterinfo->set_boss(monster->is_boss());
						monsterinfo->set_speed(monster->get_speed());
						monsterinfo->set_elite(monster->is_elite());
						monsterinfo->set_hp_line(monster->get_hp_line());
						CSPoint* point = monsterinfo->mutable_point();
						point->set_xpos(monster->m_pos.x);
						point->set_ypos(monster->m_pos.y);
						CSPoint* direct = monsterinfo->mutable_direct();
						direct->set_xpos(monster->m_direct.x);
						direct->set_ypos(monster->m_direct.y);
						MonsterMoveValue * moves = monsterinfo->mutable_moves();
						monster->pack_move(moves);
						MonsterBattleInfo* info = monsterinfo->mutable_battle_info();
						monster->pack_battle_info(info);
					}
					monster_itr = group.monsters.erase(monster_itr);
					if(num>=20) {
						break;
					}
				}
				rsp.set_is_summon(false);
				send_to_players(rsp);
				if(group.monsters.empty())
					m_wait_group.pop_front();
		} else {
			break;
		}
	}
}
	
uint32_t Battlefield::generate_monster_id() {
	static const int start = 10000;
	static const int end   = 1000000;
	int raid = base::ranged_random(start, end/2);
	int i=raid;
	for(; i<end; i++) {
		if(!monsters_.exist_key(player_key_t(i, 0, 0))) {
			break;
		}
	}
	return i;
}
	
void Battlefield::dynamic_add_monster(MapMonsterGroup_t group) {
	std::vector<MapMonster_t>::iterator monster_itr = group.monsters.begin();
	CSBattleMonsterCreateRsp rsp;
	for(; monster_itr != group.monsters.end();) {
		monster_itr->birth_place = m_tile_world->find_one_surrounding_points2(monster_itr->birth_place, 4);
		monster_itr->raid_id = generate_monster_id();
		Monster * monster = monsters_.add_monster(&(*monster_itr), m_tile_world, &m_effect_manager );
		if(monster) {
			/*temp code 10000000*/
			monster->m_fightprop.vision = 10000000; 
			MonsterInfo* monsterinfo = rsp.add_monsterinfo();
			monsterinfo->set_id(monster->get_id().id());
			monsterinfo->set_type_id(monster->get_type_id());
			monsterinfo->set_level(monster->get_level());
			monsterinfo->set_blood(monster->m_prop.health_point);
			monsterinfo->set_boss(monster->is_boss());
			monsterinfo->set_speed(monster->get_speed());
			monsterinfo->set_elite(monster->is_elite());
			monsterinfo->set_hp_line(monster->get_hp_line());
			CSPoint* point = monsterinfo->mutable_point();
			point->set_xpos(monster->m_pos.x);
			point->set_ypos(monster->m_pos.y);
			CSPoint* direct = monsterinfo->mutable_direct();
			direct->set_xpos(monster->m_direct.x);
			direct->set_ypos(monster->m_direct.y);
			MonsterMoveValue * moves = monsterinfo->mutable_moves();
			monster->pack_move(moves);
			MonsterBattleInfo* info = monsterinfo->mutable_battle_info();
			monster->pack_battle_info(info);
			DEBUG_LOG("MONSTER %u IS SUMMONED INTO THE BATTLEFIELD",monster->get_id().id());
		}
		monster_itr = group.monsters.erase(monster_itr);
	}	
	rsp.set_is_summon(true);
	send_to_players(rsp);
}
	
int Battlefield::add_wait_group(MapMonsterGroup_t& group, uint32_t& times) {
	group.times = times;
	m_wait_group.push_back(group);
	check_wait_group();
	return 0;
}

void Battlefield::pack_main_info(CSBattleCreateRsp* rsp) {
	Player* pPlayer = next_ref(NULL);
	while(pPlayer!=NULL) {
		BasicUserInfo* info = rsp->add_userinfo();
		pPlayer->pack_basic_info(info);
		info->set_battle_side(pPlayer->get_group());
		pPlayer->pack_basic_battle_info(info);
		pPlayer = next_ref(pPlayer);
	}
	Monster* monster = monsters_.next_ref(NULL);
	while(monster != NULL) {
		MonsterInfo* monsterinfo = rsp->add_monsterinfo();
		monsterinfo->set_id(monster->get_id().id());
		monsterinfo->set_type_id(monster->get_type_id());
		monsterinfo->set_level(monster->get_level());
		monsterinfo->set_blood(monster->m_prop.health_point);
		monsterinfo->set_boss(monster->is_boss());
		monsterinfo->set_speed(monster->get_speed());
		monsterinfo->set_elite(monster->is_elite());
		monsterinfo->set_hp_line(monster->get_hp_line());
		CSPoint* point = monsterinfo->mutable_point();
		point->set_xpos(monster->m_pos.x);
		point->set_ypos(monster->m_pos.y);
		CSPoint* direct = monsterinfo->mutable_direct();
		direct->set_xpos(monster->m_direct.x);
		direct->set_ypos(monster->m_direct.y);
		MonsterMoveValue * moves = monsterinfo->mutable_moves();
		monster->pack_move(moves);
		MonsterBattleInfo* info = monsterinfo->mutable_battle_info();
		monster->pack_battle_info(info);
		monster = monsters_.next_ref(monster);
	}
	rsp->set_battle_id(get_battle_id());
	rsp->set_map_id(get_map_id());
	CSPoint* portal = rsp->mutable_portal();
	portal->set_xpos(m_portal.x);
	portal->set_ypos(m_portal.y);
	rsp->set_portal_map_id(get_next_map_id());
	for(std::vector< TileWorld<>::Point>::iterator itr =m_monster_born.begin(); itr!=m_monster_born.end(); ++itr) {
		CSPoint* point = rsp->add_monsterbirth();	
		point->set_xpos(itr->x);
		point->set_ypos(itr->y);	
	}
}
	
bool Battlefield::monster_clear() {
	DEBUG_LOG("CAR DEBUG mosters empty",monsters_.empty());
	return  monsters_.empty() && !m_wait_handler.available() && m_wait_group.empty(); 
}
	
void Battlefield::revival(Player *pPlayer) {
	pPlayer->add_buff(4999999);
#ifndef FRONT
	uint32_t revival_times = pPlayer->m_attribute_list.get<uint32_t>("revival_times");
	if(revival_times<3) {
		int ret = pPlayer->exchange_par_svr_call(30700+revival_times);
		if(ret) {
			pPlayer->send_stderr(ret);
			return ;
		}
	} else {
		return ;
	}
	CSBattlePlayerRevivalRsp rsp;
	rsp.set_userid(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	send_to_players(rsp);	
	pPlayer->m_attribute_list.set("revival_times", revival_times + 1);
	//统计复活
	STAT_MGR.revive(pPlayer,m_map_id);
	DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u", ST_REVIVING_RECORD, pPlayer->zone_id(), base::Time().seconds(), pPlayer->name().c_str(), pPlayer->id(), pPlayer->regtime(), pPlayer->channelid(), pPlayer->type(), revival_times + 1, pPlayer->lv());
#endif
	pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
}
	
int Battlefield::server_update()
{
	if(empty())
		safe_release();
	return 0;
}
	
uint32_t Battlefield::get_map_id() {
	return m_map_id;
}
	
uint32_t Battlefield::get_next_map_id() {
	return m_next_map_id;
}
	
Battlefield::IdType Battlefield::get_battle_id() {
	return m_battle_id;
}
	
Player* Battlefield::get_owner() {
	return owner;
}

bool Battlefield::get_complete() {
	return m_complete;
}
	
bool Battlefield::set_complete() {
	if(!m_complete) {
		m_complete = true;
		CSBattleLoadCompleteRsp rsp;
		rsp.set_timeout(true);
		send_to_players(rsp);
		sGloble.Timer.add_event(*this, &Battlefield::update, base::Time(), UPDATETIME, -1);
//** temperate code for front end bug
		Player* pPlayer = next_ref(NULL);
		while(pPlayer) {
			if(!(pPlayer->m_prop.health_point)) {
				CSBattlePlayerDelRsp rsp;
				rsp.set_userid(pPlayer->id());
				rsp.set_reg_time(pPlayer->regtime());
				rsp.set_channel_id(pPlayer->channelid());
				uint32_t times= pPlayer->m_attribute_list.get<uint32_t>("revival_times");
				if(times < 3) {
				rsp.set_exchange_id(pPlayer->m_attribute_list.get<uint32_t>("revival_times")+30700);
				} else {
					rsp.set_exchange_id(0);
				}
				send_to_players(rsp);
			}
			pPlayer = next_ref(pPlayer);
		}
//***
		return true;	
	}
	return false;
}
	
void Battlefield::init_defence_map() {
	MapContent_t* mapcontent  = g_instance_config->get_mapcontent_info(m_battle_id, m_map_id);
	float group_timer = 0;
	uint32_t set_timer = 0;
	base::Time r_time;
	while(true) {
		std::map<uint32_t, MapMonsterGroup_t>::iterator itr = mapcontent->monster_group.begin();
		bool continue_set = false;
		for(; itr!=mapcontent->monster_group.end(); ++itr) {
			if(itr->second.times > set_timer && itr->second.refresh_time) {
				r_time.ms_timeadd(group_timer * 1000);
				m_wait_handler = sGloble.Timer.add_event(*this, &Battlefield::add_wait_group, itr->second, set_timer, r_time);
				group_timer = itr->second.refresh_time;
				continue_set = true;
			}
		}
		set_timer ++;
		if(!continue_set) break;
	}
}
	
void Battlefield::init_defence_map_server() {
#ifndef FRONT
	MapContent_t* mapcontent  = g_instance_config->get_mapcontent_info(m_battle_id, m_map_id);
	if(!mapcontent)return;
	float group_timer = 0;
	uint32_t set_timer = 0;
	std::map<uint32_t, MapMonsterGroup_t> monster_group = mapcontent->monster_group;
	while(true) {
		std::map<uint32_t, MapMonsterGroup_t>::iterator itr = monster_group.begin();
		bool continue_set = false;
		for(; itr!=monster_group.end(); ++itr) {
			if(itr->second.times > set_timer && itr->second.refresh_time) {
				std::vector<MapMonster_t> monsters = itr->second.monsters;
				std::vector<MapMonster_t>::iterator monster_itr = monsters.begin();
				for(; monster_itr != monsters.end();) {
					monster_itr->raid_id += set_timer * 1000000;
					monsters_.add_monster(&(*monster_itr), m_tile_world, &m_effect_manager );
					monster_itr ++;
				}
				group_timer = itr->second.refresh_time;
				continue_set = true;
			}
		}
		set_timer ++;
		if(!continue_set) break;
	}
#endif
}

uint32_t Battlefield::get_dead_mon_num()
{
	return m_dead_mon_num;
}
	
CDEFBattle::CDEFBattle(Player* pPlayer, uint32_t battle_id, uint32_t map_id):
	Battlefield(pPlayer, battle_id, map_id) {

}
	
bool CDEFBattle::monster_clear()
{
	return  ((base::Manager<Monster>)monsters_).empty() && !m_wait_handler.available() && m_wait_group.empty(); 
}
	
PVPBattle::PVPBattle(Player* pPlayer, uint32_t battle_id, uint32_t map_id) :
	Battlefield(pPlayer, battle_id, map_id),
	mode_(PVP),
	group_total_hp1_(0), 
	group_total_hp2_(0)
{

	if (pPlayer->get_group() % 2) {
		group_total_hp1_ += pPlayer->m_fightprop.health_point;
	} else {
		group_total_hp2_ += pPlayer->m_fightprop.health_point;
	}
}

void PVPBattle::player_join(Player* pPlayer)
{
	Battlefield::player_join(pPlayer);
	
	if (pPlayer->get_group() % 2) {
		group_total_hp1_ += pPlayer->m_fightprop.health_point;
	} else {
		group_total_hp2_ += pPlayer->m_fightprop.health_point;
	}
}

int PVPBattle::update() {
	uint32_t count_group1 = 0;
	uint32_t count_group2 = 0;
	Player* pPlayer = next_ref(NULL);
	while(pPlayer!=NULL) {
		// 计算各组当前还剩的人数
		if (pPlayer->m_prop.health_point) {
			if(pPlayer->get_group() % 2){
				count_group1 ++;
			} else {
				count_group2 ++;
			}
		}
		if (!pPlayer->m_prop.health_point 
			&& dead_player_.find(pPlayer->id()) == dead_player_.end()) {
			// 告知其他玩家该用户挂了
			pPlayer->sig_dead(pPlayer);
			dead_player_.insert(pPlayer->id());
		}
		pPlayer = next_ref(pPlayer);
	}
	if(!count_group1 || !count_group2 ) {
		Player* pPlayer = next_ref(NULL);
		while(pPlayer!=NULL) {
			if ((pPlayer->get_group()%2 == 1 && count_group1)
				|| (pPlayer->get_group()%2 == 0 && !count_group1)) {
				// 胜利什么都不做,坐等loser被sig_dead
			} else {
				// 释放用户并发奖励
				reduce_obj(pPlayer);
				pPlayer->sig_dead(pPlayer);
			}
			pPlayer = next_ref(pPlayer);
		}
		safe_release();
	}
	return 0;
}

void PVPBattle::on_player_leave(base::Manager<Player>* mPlayer, Player* pPlayer)
{
	DEBUG_LOG("p %u LEAVE", pPlayer->id());
	if(owner == pPlayer) {
		owner = NULL;
	}
}
	
void PVPBattle::attack(Player *pPlayer, const CSPlayerHitPlayerReq * req) {
	CSPlayerHitPlayerRsp attack_rsp;
/*
message CSPlayerAttackReq { //用户打击怪物
    required uint32 userid = 1; //用户ID
    required CSPoint start_pos = 2; //开始位置
    required CSPoint start_direct = 3; //开始方向
    required CSPoint end_pos = 4; //结束位置
    required CSPoint end_direct = 5; //结束方向
    required uint32 action_id = 6; //动作ID
    repeated AttackMonsterInfo monsters = 7; //被攻击怪物信息
}
 
*/
	pPlayer->m_pos = TileWorld<>::Point(req->end_pos().xpos(), req->end_pos().ypos());

	attack_rsp.set_userid(req->userid());
	attack_rsp.set_reg_time(req->reg_time());
	attack_rsp.set_channel_id(req->channel_id());
	CSPoint* start_point = attack_rsp.mutable_start_pos();
	start_point->set_xpos(req->start_pos().xpos());
	start_point->set_ypos(req->start_pos().ypos());

	CSPoint* start_direct = attack_rsp.mutable_start_direct();
	start_direct->set_xpos(req->start_direct().xpos());
	start_direct->set_ypos(req->start_direct().ypos());
	
	CSPoint* end_point = attack_rsp.mutable_end_pos();
	end_point->set_xpos(req->end_pos().xpos());
	end_point->set_ypos(req->end_pos().ypos());
	
	CSPoint* end_direct = attack_rsp.mutable_end_direct();
	end_direct->set_xpos(req->end_direct().xpos());
	end_direct->set_ypos(req->end_direct().ypos());

	attack_rsp.set_action_id(req->action_id());
	
	*attack_rsp.mutable_users() = req->users();
	for(int i =0; i<req->users_size(); i++) {
		AttackPlayerInfo info = req->users(i);
		player_key_t other_key(info.userid(), info.reg_time(), info.channel_id());
		Player *other = get_ref_by_key(other_key);
		if(other) {
			DEBUG_LOG("p %u ATTACK USER:%u HEALTH:%d", pPlayer->id(), other->id(), other->m_prop.health_point);
			uint32_t real_hurt = pPlayer->check_attack_hurt(req->action_id(), other->m_fightprop, info.blood());
			other->under_attack(pPlayer, real_hurt);
		} else {
			ERROR_LOG("p %u ERROR ATTACK USER %u",pPlayer->id(), info.userid());
		}
	}
	send_to_players(attack_rsp);
}

#ifndef FRONT   
 
AiBattle::AiBattle(Player* pPlayer, uint32_t battle_id, uint32_t map_id):
	Battlefield(pPlayer, battle_id, map_id)	
{
	if(pPlayer->is_ai()){
		m_ai = AIPlayer(pPlayer);
		m_ai_id = pPlayer->id();
		m_ai_reg_time = pPlayer->regtime();
		m_ai_channel_id = pPlayer->channelid();
		m_ai_name = pPlayer->name();
	}
}
	
uint32_t AiBattle::get_ai_hp_percent() {
	return m_ai.m_prop.health_point * 100  /m_ai.m_fightprop.health_point ;
}
	
void AiBattle::set_ai_hp(uint32_t hp) {
	m_ai.m_prop.health_point = hp;
}


void AiBattle::player_join(Player* pPlayer)
{
	if(pPlayer->is_ai()){
		if(m_hero_born.size() > pPlayer->get_group()) {
			pPlayer->m_pos = m_hero_born[pPlayer->get_group()];
		} else if( !m_hero_born.empty() ) {
			pPlayer->m_pos = m_hero_born[0];
		}
		m_ai = AIPlayer(pPlayer);
		m_ai_id = pPlayer->id();
		m_ai_reg_time = pPlayer->regtime();
		m_ai_channel_id = pPlayer->channelid();
		m_ai_name = pPlayer->name();

		CSBattleUserJoinRsp rsp;
		BasicUserInfo * info = rsp.add_userinfo();
		m_ai.pack_basic_user_info(info);
		send_to_players(rsp);
		pPlayer->sig_dead.connect(this, &AiBattle::on_player_dead);
		m_ai.m_buffmanager->sig_undo_buff.connect((Battlefield*)(this), &Battlefield::on_buffer_finish);
	} else {
		Battlefield::player_join(pPlayer);
	}
}
	
void AiBattle::on_player_dead(Entity* pPlayer) {
	CSBattlePlayerDelRsp rsp;
	rsp.set_userid(pPlayer->get_id().id());
	rsp.set_reg_time(pPlayer->get_id().regtime());
	rsp.set_channel_id(pPlayer->get_id().channelid());
	rsp.set_exchange_id(0);
	send_to_players(rsp);
}
	
void AiBattle::load_complete(Player* pPlayer)
{
	CSBattleUserJoinRsp rsp;
	BasicUserInfo * info = rsp.add_userinfo();
	m_ai.pack_basic_user_info(info);
	pPlayer->send_msg(rsp);
}
	
void AiBattle::pack_main_info(CSBattleCreateRsp* rsp)
{
	Battlefield::pack_main_info(rsp);
	BasicUserInfo* info = rsp->add_userinfo();
	m_ai.pack_basic_user_info(info);
}
    
int AiBattle::update() {
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->m_prop.health_point) {
			reduce_obj(pPlayer);
			safe_release();
			return 0;
		} else if(pPlayer->is_ai())  {
			reduce_obj(pPlayer);
		}
		pPlayer= next_ref(pPlayer);
	}
	if(!m_ai.m_prop.health_point) {
	//	reduce_obj(m_ai.get_id());
		
		Player* pPlayer = next_ref(NULL);
		while(pPlayer != NULL) {
			if(!pPlayer->is_ai()) {
				PVAI_Finish_User_Process *finish_pvai = new PVAI_Finish_User_Process(pPlayer, true);
				finish_pvai->add_query_user(pPlayer->id(), pPlayer->regtime(), pPlayer->channelid());
				finish_pvai->add_query_user(m_ai_id, m_ai_reg_time, m_ai_channel_id);
				finish_pvai->add_query_callback();
				break;
			}
			pPlayer= next_ref(pPlayer);
		}
		safe_release();
		return 0;	
	}
	return 0;
}

void AiBattle::on_player_leave(base::Manager<Player>*, Player* pPlayer)
{
	// pvai离开战斗定为输 写日志
	if (!pPlayer->is_ai()) {
		PVAI_Finish_User_Process *finish_pvai = new PVAI_Finish_User_Process(pPlayer, false);
		finish_pvai->add_query_user(pPlayer->id(), pPlayer->regtime(), pPlayer->channelid());
		finish_pvai->add_query_user(m_ai_id, m_ai_reg_time, m_ai_channel_id);
		finish_pvai->add_query_callback();
	}
}

void AiBattle::attack(Player *pPlayer, const CSPlayerHitPlayerReq * req) {
	CSPlayerHitPlayerRsp attack_rsp;
/*
message CSPlayerAttackReq { //用户打击怪物
    required uint32 userid = 1; //用户ID
    required CSPoint start_pos = 2; //开始位置
    required CSPoint start_direct = 3; //开始方向
    required CSPoint end_pos = 4; //结束位置
    required CSPoint end_direct = 5; //结束方向
    required uint32 action_id = 6; //动作ID
    repeated AttackMonsterInfo monsters = 7; //被攻击怪物信息
}
 
*/
	pPlayer->m_pos = TileWorld<>::Point(req->end_pos().xpos(), req->end_pos().ypos());

	attack_rsp.set_userid(req->userid());
	attack_rsp.set_reg_time(req->reg_time());
	attack_rsp.set_channel_id(req->channel_id());
	CSPoint* start_point = attack_rsp.mutable_start_pos();
	start_point->set_xpos(req->start_pos().xpos());
	start_point->set_ypos(req->start_pos().ypos());

	CSPoint* start_direct = attack_rsp.mutable_start_direct();
	start_direct->set_xpos(req->start_direct().xpos());
	start_direct->set_ypos(req->start_direct().ypos());
	
	CSPoint* end_point = attack_rsp.mutable_end_pos();
	end_point->set_xpos(req->end_pos().xpos());
	end_point->set_ypos(req->end_pos().ypos());
	
	CSPoint* end_direct = attack_rsp.mutable_end_direct();
	end_direct->set_xpos(req->end_direct().xpos());
	end_direct->set_ypos(req->end_direct().ypos());

	attack_rsp.set_action_id(req->action_id());
	
	*attack_rsp.mutable_users() = req->users();
	for(int i =0; i<req->users_size(); i++) {
		AttackPlayerInfo info = req->users(i);
		player_key_t other_key(info.userid(), info.reg_time(), info.channel_id());
		Player *other = get_ref_by_key(other_key);
		//if(info.userid() == m_ai_id) {
		if(other_key == player_key_t(m_ai_id, m_ai_reg_time, m_ai_channel_id)) {
			uint32_t real_hurt = pPlayer->check_attack_hurt(req->action_id(), m_ai.m_fightprop, info.blood());
			DEBUG_LOG("AT BATTLE USER :%u ATTACK AI USER:%u HEALTH:%d HURT:%d", pPlayer->id(),m_ai_id, m_ai.m_prop.health_point, real_hurt);
			m_ai.under_attack(pPlayer, real_hurt);
			CSPlayerHPRsp rsp;
		    rsp.set_userid(m_ai_id);
		    rsp.set_reg_time(m_ai_reg_time);
		    rsp.set_channel_id(m_ai_channel_id);
		    rsp.set_blood(m_ai.m_prop.health_point);
			rsp.set_source_id(pPlayer->id());
			rsp.set_source_reg_time(pPlayer->regtime());
			rsp.set_source_channel_id(pPlayer->channelid());
			pPlayer->send_msg(rsp);
		} else if(other) {
			if(!other->is_ai()) {
				uint32_t real_hurt =  info.blood();//pPlayer->check_attack_hurt(req->action_id(), other->m_fightprop, info.blood());
				//DEBUG_LOG("AI BATTLE USER :%u ATTACK USER:%u HEALTH:%d HURT:%d", pPlayer->id(), other->id(), other->m_prop.health_point, real_hurt);
				other->under_attack(&m_ai, real_hurt);
			}
		} else {
			ERROR_LOG("USER :%u ERROR ATTACK USER %u",pPlayer->id(), info.userid());
		}
	}
//	send_to_players(attack_rsp);
}
	
void AiBattle::skill_begin(Player *pPlayer, const CSSkillBeginReq * req) {
	uint32_t entity_pos = 0;
	if(req->entity_pos()){
		entity_pos = req->entity_pos();
	}

	DEBUG_LOG("CAR DEBUG AI BATTLE SKILL BEIGN");

	CSSkillBeginRsp rsp;
	rsp.set_action_id(req->action_id());
	rsp.set_userid(req->userid());
	rsp.set_reg_time(req->reg_time());
	rsp.set_channel_id(req->channel_id());
	CSPoint* pos = rsp.mutable_pos();
	pos->set_xpos(req->pos().xpos());
	pos->set_ypos(req->pos().ypos());
	rsp.set_start_dir_x(req->start_dir_x());
	rsp.set_start_dir_y(req->start_dir_y());
	CSPoint* target_pos  = rsp.mutable_target_pos();
	target_pos->set_xpos(req->target_pos().xpos());
	target_pos->set_ypos(req->target_pos().ypos());
	rsp.set_direct_x(req->direct_x());
	rsp.set_direct_y(req->direct_y());
	rsp.set_aim(req->aim());
	rsp.set_entity_pos(req->entity_pos());

	uint32_t skill_id = req->action_id();
	if(g_hero_skill_config->is_buff(skill_id) || g_hero_skill_config->is_invincible(skill_id)) {
		CSBasicUserBattleInfoRsp rsp;
		//if(req->userid() == pPlayer->id()) {
		if (player_key_t(req->userid(), req->reg_time(), req->channel_id()) 
			== player_key_t(pPlayer->id(), pPlayer->regtime(), pPlayer->channelid())) {
			pPlayer->add_buff(skill_id);
			rsp.set_id(pPlayer->id());
			rsp.set_reg_time(pPlayer->regtime());
			rsp.set_channel_id(pPlayer->channelid());
			BasicUserBattleInfo* info = rsp.mutable_battle_info();
			pPlayer->pack_basic_battle_info(info);
		} else {
			m_ai.add_buff(skill_id);
			rsp.set_id(m_ai.get_id().id());
			BasicUserBattleInfo* info = rsp.mutable_battle_info();
			m_ai.pack_basic_battle_info(info);
		}
		send_to_players(rsp);	
	} else 	if(entity_pos&&g_fairy_skill_config->is_buff(skill_id)) {
		CSBasicUserBattleInfoRsp rsp;
		//if(req->userid() == pPlayer->id()) {
		if (player_key_t(req->userid(), req->reg_time(), req->channel_id()) 
			== player_key_t(pPlayer->id(), pPlayer->regtime(), pPlayer->channelid())) {
			DEBUG_LOG("CAR DEBUG PLAYER %u FAIRY USE SKILL %u",req->userid(),skill_id);
			uint32_t skill_attr = pPlayer->get_fairy_attr(entity_pos,ITEM_ATTR_SKIATK);
			DEBUG_LOG("CAR DEBUG FAIRY SKILL ATTACK ATTR %u",skill_attr);
			pPlayer->add_buff(skill_id,skill_attr);
			rsp.set_id(pPlayer->id());
			rsp.set_reg_time(pPlayer->regtime());
			rsp.set_channel_id(pPlayer->channelid());
			BasicUserBattleInfo* info = rsp.mutable_battle_info();
			pPlayer->pack_basic_battle_info(info);
			uint32_t blood = g_fairy_skill_config->add_blood(skill_id,skill_attr);
				if(blood) {
				CSBattleUserAddBloodRsp blood_rsp;
				blood_rsp.set_id(pPlayer->id());
				blood_rsp.set_reg_time(pPlayer->regtime());
				blood_rsp.set_channel_id(pPlayer->channelid());
				blood_rsp.set_blood(blood);	
				send_to_players(blood_rsp);
			}
		} else {
			DEBUG_LOG("CAR DEBUG PLAYER %u FAIRY USE SKILL %u",req->userid(),skill_id);
			uint32_t skill_attr = pPlayer->get_fairy_attr(entity_pos,ITEM_ATTR_SKIATK);
			DEBUG_LOG("CAR DEBUG FAIRY SKILL ATTACK ATTR %u",skill_attr);
			m_ai.add_buff(skill_id,skill_attr);
			rsp.set_id(m_ai.get_id().id());
			rsp.set_reg_time(m_ai.get_id().regtime());
			rsp.set_channel_id(m_ai.get_id().channelid());
			BasicUserBattleInfo* info = rsp.mutable_battle_info();
			m_ai.pack_basic_battle_info(info);
			uint32_t blood = g_fairy_skill_config->add_blood(skill_id,skill_attr);
			if(blood) {
				CSBattleUserAddBloodRsp blood_rsp;
				blood_rsp.set_id(m_ai.get_id().id());
				blood_rsp.set_reg_time(m_ai.get_id().regtime());
				blood_rsp.set_channel_id(m_ai.get_id().channelid());
				blood_rsp.set_blood(blood);	
				send_to_players(blood_rsp);
			}
		}
		send_to_players(rsp);	
	}
	send_to_players(rsp);
}

AiBattle2::AiBattle2(Player* pPlayer, uint32_t battle_id, uint32_t map_id)
	: AiBattle(pPlayer, battle_id, map_id)
{
	// nothing
}

int AiBattle2::update()
{
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->m_prop.health_point) {
			reduce_obj(pPlayer);
			safe_release();
			return 0;
		} else if(pPlayer->is_ai())  {
			reduce_obj(pPlayer);
		}
		pPlayer= next_ref(pPlayer);
	}
	if(!ai().m_prop.health_point) {
		safe_release();
		return 0;	
	}
	return 0;

}
void AiBattle2::on_player_leave(base::Manager<Player>*, Player*)
{
	// nothing
}
#endif
	
#ifndef FRONT
WBossBattle::WBossBattle(uint32_t battle_id)
	: Battlefield(0, battle_id)
{
	monsters_total_hp_ = 0;
	Monster *pmonster = monsters_.next_ref(NULL);
	while(pmonster!= NULL) {
		monsters_total_hp_ += pmonster->m_fightprop.health_point;
		pmonster = monsters_.next_ref(pmonster);
	}
	DEBUG_LOG("WBOSS MONSTER TOTAL HP:%lu", monsters_total_hp_);
}

void WBossBattle::player_join(Player* pPlayer)
{
	pPlayer->leave_map();
	if(!exist_obj(pPlayer)) {
	
		if( !m_hero_born.empty() ) {
			pPlayer->m_pos = m_hero_born[0];
		} else {
			ERROR_LOG("p %u haven't pos to join battle: %d", pPlayer->id(), get_battle_id());
		}
		if(!m_transfer) {
			pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
		}
		add_obj(pPlayer);
		players_load_complete_[pPlayer] = 0;
		pPlayer->sig_dead.connect(this, &WBossBattle::on_player_dead);
		pPlayer->m_buffmanager->sig_undo_buff.connect(this, &WBossBattle::on_buffer_finish);
		//统计世界boss参加人数
		STAT_MGR.daily_wordboss(pPlayer);
	}
}

void WBossBattle::load_complete(Player* pPlayer)
{
	// 玩家load_complete完成
	players_load_complete_[pPlayer] = 1;
	// 第一个人进入的时候boss才开始打架
	if (!get_complete()) {
		set_complete();
	}
}

void WBossBattle::attack(Player *pPlayer, const CSPlayerAttackReq *req)
{
	/*Todo must check the available of action*/
	CSPlayerAttackRsp attack_rsp;
	CSWBMonsterBloodRsp blood_rsp;
	CSMonsterDieRsp rsp;
	
	pPlayer->m_pos = TileWorld<>::Point(req->end_pos().xpos(), req->end_pos().ypos());

	attack_rsp.set_userid(req->userid());
	attack_rsp.set_reg_time(req->reg_time());
	attack_rsp.set_channel_id(req->channel_id());

	CSPoint* start_point = attack_rsp.mutable_start_pos();
	start_point->set_xpos(req->start_pos().xpos());
	start_point->set_ypos(req->start_pos().ypos());

	CSPoint* start_direct = attack_rsp.mutable_start_direct();
	start_direct->set_xpos(req->start_direct().xpos());
	start_direct->set_ypos(req->start_direct().ypos());
	
	CSPoint* end_point = attack_rsp.mutable_end_pos();
	end_point->set_xpos(req->end_pos().xpos());
	end_point->set_ypos(req->end_pos().ypos());
	
	CSPoint* end_direct = attack_rsp.mutable_end_direct();
	end_direct->set_xpos(req->end_direct().xpos());
	end_direct->set_ypos(req->end_direct().ypos());

	attack_rsp.set_action_id(req->action_id());
	
	for(int i =0; i<req->monsters_size(); i++) {
		AttackMonsterInfo info = req->monsters(i);
		*attack_rsp.mutable_monsters() = req->monsters();
		Monster *pmonster = monsters_.get_ref_by_key(player_key_t(info.id(), 0, 0));
		if(pmonster) {
			if(!pmonster->is_boss()) {
				pmonster->under_attack_back( TileWorld<>::Point(info.start_pos().xpos(), info.start_pos().ypos()) , TileWorld<>::Point(info.pos().xpos(), info.pos().ypos()));
			}
			// 伤害记录
			uint32_t hurt = pmonster->m_prop.health_point;
			uint32_t real_hurt = pPlayer->check_attack_hurt(req->action_id(), pmonster->m_fightprop, info.blood());
			DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u %u", ST_PLYAER_HURT_WB_ONCE, pPlayer->zone_id(), base::Time().seconds(), pPlayer->name().c_str(), pPlayer->id(), pPlayer->regtime(), pPlayer->channelid(), pPlayer->type(), pPlayer->property(ATTR_EFFECTIVENESS), req->action_id(), real_hurt, pPlayer->lv());
			pmonster->under_attack(pPlayer, real_hurt);
			hurt -= pmonster->m_prop.health_point;
			uint64_t history_hurt = pPlayer->m_attribute_list.get<uint64_t>("wboss_turn_hurt");
			pPlayer->m_attribute_list.set<uint64_t>("wboss_turn_hurt", history_hurt + hurt);
			// 伤害排名
			uint64_t m_total_hurt = sWBInstanceMgr.get_player_total_hurt(pPlayer->get_id());
			uint32_t m_rank =  sWBInstanceMgr.update_player_total_hurt(pPlayer->get_id(), m_total_hurt + hurt);
			pPlayer->m_attribute_list.set<uint32_t>("wboss_rank", m_rank);
			sWBInstanceMgr.set_top5_name(m_rank, pPlayer->get_id(), pPlayer->name());
			// boss血量
			CSWBMonsterBloodRsp_MonsterInfo* rsp_info = blood_rsp.add_monsters();
			rsp_info->set_id(pmonster->get_id().id());
			rsp_info->set_blood(pmonster->m_prop.health_point);
			rsp_info->set_total_hp(pmonster->m_fightprop.health_point);
			DEBUG_LOG("p %u ATTACK MONSTER:%u HIT:%d HEALTH:%d POS:%d %d", pPlayer->id(), pmonster->get_id().id(), real_hurt, pmonster->m_prop.health_point, info.pos().xpos(), info.pos().ypos());
			// 记录死亡信息
			if(!pmonster->m_prop.health_point) {
				rsp.add_monsters(pmonster->get_id().id());				
				m_dead_mon_num++;
				pmonster->safe_release();
				sWBInstanceMgr.set_last_killer(pPlayer->get_id(), pPlayer->name());
			}
		} else {
			ERROR_LOG("p %u ERROR ATTACK MONSTER %u",pPlayer->id(), info.id());
		}
	}
	// 发给一组人
	pPlayer->send_msg_to_wb_btl_observers(attack_rsp);
	pPlayer->send_msg_to_wb_btl_observers(blood_rsp);
	if (rsp.monsters_size()) {
		pPlayer->send_msg_to_wb_btl_observers(rsp);
	}
}

void WBossBattle::skill_begin(Player *pPlayer, const CSSkillBeginReq * req)
{
	uint32_t entity_pos = 0;
	if(req->entity_pos()){
		entity_pos = req->entity_pos();
	}

	CSSkillBeginRsp rsp;
	rsp.set_action_id(req->action_id());
	rsp.set_userid(req->userid());
	rsp.set_reg_time(req->reg_time());
	rsp.set_channel_id(req->channel_id());
	CSPoint* pos = rsp.mutable_pos();
	pos->set_xpos(req->pos().xpos());
	pos->set_ypos(req->pos().ypos());
	rsp.set_start_dir_x(req->start_dir_x());
	rsp.set_start_dir_y(req->start_dir_y());
	CSPoint* target_pos  = rsp.mutable_target_pos();
	target_pos->set_xpos(req->target_pos().xpos());
	target_pos->set_ypos(req->target_pos().ypos());
	rsp.set_direct_x(req->direct_x());
	rsp.set_direct_y(req->direct_y());
	rsp.set_aim(req->aim());
	rsp.set_entity_pos(req->entity_pos());

	uint32_t skill_id = req->action_id();
	if(g_hero_skill_config->is_buff(skill_id) || g_hero_skill_config->is_invincible(skill_id)) {
		pPlayer->add_buff(skill_id);
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);
	} else if(entity_pos && g_fairy_skill_config->is_buff(skill_id)) {
		uint32_t skill_attr = pPlayer->get_fairy_attr(entity_pos,ITEM_ATTR_SKIATK);
		CSBasicUserBattleInfoRsp rsp;
		pPlayer->add_buff(skill_id,skill_attr);
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);
		uint32_t blood = g_fairy_skill_config->add_blood(skill_id,skill_attr);
		if(blood) {
			CSBattleUserAddBloodRsp blood_rsp;
			blood_rsp.set_id(pPlayer->id());
			blood_rsp.set_reg_time(pPlayer->regtime());
			blood_rsp.set_channel_id(pPlayer->channelid());
			blood_rsp.set_blood(blood);	
			// 发个一组玩家
			pPlayer->send_msg_to_wb_btl_observers(blood_rsp);
		}
	}
	// 发个一组玩家
	pPlayer->send_msg_to_wb_btl_observers(rsp);
}

int WBossBattle::update()
{
	if(monster_clear()) {
		CSMonsterDieRsp rsp;
		Monster* monster = monsters_.next_ref(NULL);
		while(monster != NULL) {
			rsp.add_monsters(monster->get_id().id());				
			monster = monsters_.next_ref(monster);
		}
		send_to_players(rsp);
		safe_release();
		return 0;
	}
	update_effect();	
	CSMonsterMoveRsp move_rsp;
	CSMonsterAttackRsp attack_rsp;
	CSMonsterEffectRsp effect_rsp;
	monsters_.update(&move_rsp, &attack_rsp, &effect_rsp);
	if(move_rsp.moves_size()) {
		send_to_players(move_rsp);
	}
	if(attack_rsp.attacks_size()) {
		send_to_players(attack_rsp);
	}
	if(effect_rsp.effects_size()) {
		send_to_players(effect_rsp);
	}
	CSBuffEffectRsp buff_effect_rsp;
	monsters_.do_buff(&buff_effect_rsp);
	if(buff_effect_rsp.effects_size()) {
		send_to_players(buff_effect_rsp);
	}
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead())
			monsters_.try_locking(pPlayer);
		pPlayer = next_ref(pPlayer);
	}
	return 0;
}

void WBossBattle::on_buffer_finish(Entity* entity) {
	Player *pPlayer = dynamic_cast<Player*>(entity);
	if(pPlayer) {
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);
	}
}

void WBossBattle::move(Player* pPlayer, const CSBattleMoveReq * req) {
/*Todo must check the available of move*/
	TileWorld<>::Point pos(req->pos().xpos(), req->pos().ypos());
	if(!m_tile_world->is_pos_walkable(pos)) {
		ERROR_LOG("PLAYER:%u CAN'T MOVE TO UNWALKALBE POS:%d %d", pPlayer->id(), pos.x, pos.y);
		return; 
	};
	pPlayer->m_pos.x = req->pos().xpos();
	pPlayer->m_pos.y = req->pos().ypos(); 
	pPlayer->m_direct.x = req->direct().xpos();
	pPlayer->m_direct.y = req->direct().ypos();
	//DEBUG_LOG("PLAYER %u MOVE TO: %d %d", pPlayer->id(),pPlayer->m_pos.x, pPlayer->m_pos.y);
	CSBattleMoveRsp rsp;
	rsp.set_id(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	CSPoint* one_point =rsp.mutable_pos();
	one_point->set_xpos(pPlayer->m_pos.x);
	one_point->set_ypos(pPlayer->m_pos.y);
	CSPoint* one_direct =rsp.mutable_direct();
	one_direct->set_xpos(pPlayer->m_direct.x);
	one_direct->set_ypos(pPlayer->m_direct.y);
	rsp.set_time(req->time());
	// 发个一组玩家
	pPlayer->send_msg_to_wb_btl_observers(rsp);
}

void WBossBattle::update_effect() {
	m_effect_manager.update();
	m_effect_manager.effect_battle(this);
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead() && players_load_complete_[pPlayer]) {
			CSMonsterHitRsp hit_rsp;
			m_effect_manager.effect(pPlayer, &hit_rsp);
			if(hit_rsp.users_size()) {
				// 发个一组玩家
				pPlayer->send_msg_to_wb_btl_observers(hit_rsp);
			}
		}
		pPlayer = next_ref(pPlayer);
	}
}

void WBossBattle::on_player_leave(base::Manager<Player>*, Player* pPlayer)
{
	DEBUG_LOG("WBOSS p %u LEAVE", pPlayer->id());
	CSBattleUserLeaveRsp rsp;
	rsp.set_userid(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	// 发送给一组用户，除了自己
	pPlayer->send_msg_to_wb_btl_observers(rsp, pPlayer);
	// send_to_players_except(rsp, pPlayer);
	pPlayer->sig_leave_battle(pPlayer);
	players_load_complete_.erase(pPlayer);
	pPlayer->sig_dead.disconnect(this);
}
void WBossBattle::on_player_dead(Entity* entity)
{
	Player* pPlayer = static_cast<Player*> (entity);
	if(pPlayer) {
		DEBUG_LOG("WBOSS p %u DEAD", pPlayer->id());
		CSBattlePlayerDelRsp rsp;
		rsp.set_userid(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		rsp.set_exchange_id(0);
		// 发送给一组用户
		pPlayer->send_msg_to_wb_btl_observers(rsp);
		// 单次死亡结算在wb_instance中
		// 死亡复活定时器
		pPlayer->on_player_wb_dead();
		// 死亡重新加入时间限制
		pPlayer->set_property(ATTR_WBOSS_REVIVAL_TIME, base::Time().seconds() + WBOSS_REVIVAL_CD);
	}
}

int WBossBattle::sync_boss_info()
{
	// 每秒同步排行榜信息
	CSWBRTHurtRankRsp rt_rank_rsp;
	sWBInstanceMgr.pack_wb_rt_hurt_rank(rt_rank_rsp.mutable_ranking());
	// 排行榜信息发送给世界boss主城中所有人
	Map *m = sMsgDispatcher.get_map(WORLDBOSS_MAP_ID);
	if (m) {
		m->send_to_players(rt_rank_rsp);
	}
	// 每秒同步boss血量信息给所有玩家
	CSWBMonsterBloodRsp blood_rsp;
	Monster *pmonster = monsters_.next_ref(NULL);
	while(pmonster!= NULL) {
		CSWBMonsterBloodRsp_MonsterInfo* rsp_info = blood_rsp.add_monsters();
		rsp_info->set_id(pmonster->get_id().id());
		rsp_info->set_blood(pmonster->m_prop.health_point);
		rsp_info->set_total_hp(pmonster->m_fightprop.health_point);

		pmonster = monsters_.next_ref(pmonster);
	}
	// 发送给在战场中所有玩家
	Player *pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead()) {
			pPlayer->send_msg(blood_rsp);
			rt_rank_rsp.clear_self_ranking();
			uint32_t rank = pPlayer->m_attribute_list.get<uint32_t>("wboss_rank");
			uint64_t player_total_hurt = sWBInstanceMgr.get_player_total_hurt(pPlayer->get_id());
			sWBInstanceMgr.pack_wb_stat(rt_rank_rsp.mutable_self_ranking(),
										rank, player_total_hurt, pPlayer->name(), false);

			pPlayer->send_msg(rt_rank_rsp);
		}
		pPlayer = next_ref(pPlayer);
	}

	return 0;
}

void WBossBattle::revival(Player *pPlayer) {
	// 消耗钻石
	int ret = pPlayer->exchange_par_svr_call(20510);
	if(ret) {
		pPlayer->send_stderr(ret);
		return ;
	}
	pPlayer->add_buff(4999999);
	
	CSBattlePlayerRevivalRsp rsp;
	rsp.set_userid(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	// 发个一组玩家
	pPlayer->send_msg_to_wb_btl_observers(rsp);

	pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
	pPlayer->m_attribute_list.set<uint64_t>("wboss_turn_hurt", 0);
	
	pPlayer->on_player_wb_revival();
}

void WBossBattle::revival_by_cd_over(Player *pPlayer)
{
	if (pPlayer->property(ATTR_WBOSS_REVIVAL_TIME) <= base::Time().seconds()) {
		pPlayer->add_buff(4999999);
		CSBattlePlayerRevivalRsp rsp;
		rsp.set_userid(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);

		pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
		pPlayer->m_attribute_list.set<uint64_t>("wboss_turn_hurt", 0);
		
		sWBInstanceMgr.observer_add(pPlayer);
	} else {
		pPlayer->send_stderr(olerr_wboss_not_clear_revival_cd);
	}
}

void WBossBattle::pack_main_info(CSBattleCreateRsp* rsp, Player *pPlayer) {
	if (pPlayer!=NULL) {
		BasicUserInfo* info = rsp->add_userinfo();
		pPlayer->pack_basic_info(info);
		info->set_battle_side(pPlayer->get_group());
		pPlayer->pack_basic_battle_info(info);
	}
	Monster* monster = monsters_.next_ref(NULL);
	while(monster != NULL) {
		MonsterInfo* monsterinfo = rsp->add_monsterinfo();
		monsterinfo->set_id(monster->get_id().id());
		monsterinfo->set_type_id(monster->get_type_id());
		monsterinfo->set_level(monster->get_level());
		monsterinfo->set_blood(monster->m_prop.health_point);
		monsterinfo->set_boss(monster->is_boss());
		monsterinfo->set_speed(monster->get_speed());
		monsterinfo->set_elite(monster->is_elite());
		monsterinfo->set_hp_line(monster->get_hp_line());
		CSPoint* point = monsterinfo->mutable_point();
		point->set_xpos(monster->m_pos.x);
		point->set_ypos(monster->m_pos.y);
		CSPoint* direct = monsterinfo->mutable_direct();
		direct->set_xpos(monster->m_direct.x);
		direct->set_ypos(monster->m_direct.y);
		MonsterMoveValue * moves = monsterinfo->mutable_moves();
		monster->pack_move(moves);
		MonsterBattleInfo* info = monsterinfo->mutable_battle_info();
		monster->pack_battle_info(info);
		monster = monsters_.next_ref(monster);
	}
	rsp->set_battle_id(get_battle_id());
	rsp->set_map_id(get_map_id());
	CSPoint* portal = rsp->mutable_portal();
	portal->set_xpos(m_portal.x);
	portal->set_ypos(m_portal.y);
	rsp->set_portal_map_id(get_next_map_id());
	for(std::vector< TileWorld<>::Point>::iterator itr =m_monster_born.begin(); itr!=m_monster_born.end(); ++itr) {
		CSPoint* point = rsp->add_monsterbirth();	
		point->set_xpos(itr->x);
		point->set_ypos(itr->y);	
	}
}

bool WBossBattle::set_complete() {
	if(!m_complete) {
		m_complete = true;
		sGloble.Timer.add_event(*this, &WBossBattle::update, base::Time(), UPDATETIME, -1);
		sGloble.Timer.add_event(*this, &WBossBattle::sync_boss_info, base::Time(), UPDATETIME*5, -1);
	}
	return true;	
}


BabelBattle::BabelBattle(uint32_t battle_id)
	: Battlefield(0, battle_id)
{
	monsters_total_hp_ = 0;
	Monster *pmonster = monsters_.next_ref(NULL);
	while(pmonster!= NULL) {
		//monsters_total_hp_ = 100000;
		monsters_total_hp_ += pmonster->m_fightprop.health_point;
		pmonster = monsters_.next_ref(pmonster);
	}
	DEBUG_LOG("CAR DEBUG BBOSS MONSTER TOTAL HP:%lu %u", monsters_total_hp_,battle_id);
}

void BabelBattle::player_join(Player* pPlayer)
{
	pPlayer->leave_map();
	if(!exist_obj(pPlayer)) {
	
		if( !m_hero_born.empty() ) {
			pPlayer->m_pos = m_hero_born[0];
		} else {
			ERROR_LOG("p %u haven't pos to join battle: %d", pPlayer->id(), get_battle_id());
		}
		if(!m_transfer) {
			pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
		}
		add_obj(pPlayer);
		players_load_complete_[pPlayer] = 0;
		pPlayer->sig_dead.connect(this, &BabelBattle::on_player_dead);
		pPlayer->m_buffmanager->sig_undo_buff.connect(this, &BabelBattle::on_buffer_finish);
	}
}

void BabelBattle::load_complete(Player* pPlayer)
{
	// 玩家load_complete完成
	players_load_complete_[pPlayer] = 1;
	// 第一个人进入的时候boss才开始打架
	if (!get_complete()) {
		set_complete();
	}
}

void BabelBattle::attack(Player *pPlayer, const CSPlayerAttackReq *req)
{

    static uint32_t hurt_sec = base::Time().seconds();
	/*Todo must check the available of action*/
	CSPlayerAttackRsp attack_rsp;
	CSBBMonsterBloodRsp blood_rsp;
	CSMonsterDieRsp rsp;
	bool send_blood_flag = false;
	
	pPlayer->m_pos = TileWorld<>::Point(req->end_pos().xpos(), req->end_pos().ypos());

	attack_rsp.set_userid(req->userid());
	attack_rsp.set_reg_time(req->reg_time());
	attack_rsp.set_channel_id(req->channel_id());

	CSPoint* start_point = attack_rsp.mutable_start_pos();
	start_point->set_xpos(req->start_pos().xpos());
	start_point->set_ypos(req->start_pos().ypos());

	CSPoint* start_direct = attack_rsp.mutable_start_direct();
	start_direct->set_xpos(req->start_direct().xpos());
	start_direct->set_ypos(req->start_direct().ypos());
	
	CSPoint* end_point = attack_rsp.mutable_end_pos();
	end_point->set_xpos(req->end_pos().xpos());
	end_point->set_ypos(req->end_pos().ypos());
	
	CSPoint* end_direct = attack_rsp.mutable_end_direct();
	end_direct->set_xpos(req->end_direct().xpos());
	end_direct->set_ypos(req->end_direct().ypos());

	attack_rsp.set_action_id(req->action_id());
	
	for(int i =0; i<req->monsters_size(); i++) {
		AttackMonsterInfo info = req->monsters(i);
		*attack_rsp.mutable_monsters() = req->monsters();
		Monster *pmonster = monsters_.get_ref_by_key(player_key_t(info.id(), 0, 0));
		if(pmonster) {
			if(!pmonster->is_boss()) {
				pmonster->under_attack_back( TileWorld<>::Point(info.start_pos().xpos(), info.start_pos().ypos()) , TileWorld<>::Point(info.pos().xpos(), info.pos().ypos()));
			}
		    uint32_t hurt = pmonster->m_prop.health_point;
		    uint32_t real_hurt = pPlayer->check_attack_hurt(req->action_id(), pmonster->m_fightprop, info.blood());
            if(pmonster->is_boss()) {
                uint32_t now = base::Time().seconds();
                if(now != hurt_sec) {
                    pmonster->under_attack(pPlayer, (pmonster->m_fightprop.health_point)/BBOSS_DEAD_TIME);
                    hurt_sec = now;
                } else {
                    pmonster->under_attack(pPlayer, 0);
                }
            } else { 
			// 伤害记录
		    	pmonster->under_attack(pPlayer, real_hurt);
		    	hurt -= pmonster->m_prop.health_point;
            }
			if(pmonster->is_boss()){
				// boss血量
				CSBBMonsterBloodRsp_MonsterInfo* rsp_info = blood_rsp.add_monsters();
				rsp_info->set_id(pmonster->get_id().id());
				rsp_info->set_blood(pmonster->m_prop.health_point);
				rsp_info->set_total_hp(pmonster->m_fightprop.health_point);
				send_blood_flag = true;
			}
			DEBUG_LOG("CAR DEBUG p %u ATTACK MONSTER:%u HIT:%d HEALTH:%d POS:%d %d", pPlayer->id(), pmonster->get_id().id(), real_hurt, pmonster->m_prop.health_point, info.pos().xpos(), info.pos().ypos());
			// 记录死亡信息
			if(!pmonster->m_prop.health_point) {
				rsp.add_monsters(pmonster->get_id().id());				
				m_dead_mon_num++;
				pmonster->safe_release();
			}
		} else {
			ERROR_LOG("p %u ERROR ATTACK MONSTER %u",pPlayer->id(), info.id());
		}
	}
	// 发给一组人
	pPlayer->send_msg_to_wb_btl_observers(attack_rsp);
	if(send_blood_flag)
		pPlayer->send_msg_to_wb_btl_observers(blood_rsp);
	if (rsp.monsters_size()) {
		pPlayer->send_msg_to_wb_btl_observers(rsp);
	}
}

void BabelBattle::skill_begin(Player *pPlayer, const CSSkillBeginReq * req)
{
	uint32_t entity_pos = 0;
	if(req->entity_pos()){
		entity_pos = req->entity_pos();
	}

	CSSkillBeginRsp rsp;
	rsp.set_action_id(req->action_id());
	rsp.set_userid(req->userid());
	rsp.set_reg_time(req->reg_time());
	rsp.set_channel_id(req->channel_id());
	CSPoint* pos = rsp.mutable_pos();
	pos->set_xpos(req->pos().xpos());
	pos->set_ypos(req->pos().ypos());
	rsp.set_start_dir_x(req->start_dir_x());
	rsp.set_start_dir_y(req->start_dir_y());
	CSPoint* target_pos  = rsp.mutable_target_pos();
	target_pos->set_xpos(req->target_pos().xpos());
	target_pos->set_ypos(req->target_pos().ypos());
	rsp.set_direct_x(req->direct_x());
	rsp.set_direct_y(req->direct_y());
	rsp.set_aim(req->aim());
	rsp.set_entity_pos(req->entity_pos());

	uint32_t skill_id = req->action_id();
	DEBUG_LOG("p %u USE SKILL %u",req->userid(),skill_id);
	if(g_hero_skill_config->is_buff(skill_id) || g_hero_skill_config->is_invincible(skill_id)) {
		pPlayer->add_buff(skill_id);
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);
	} else if(entity_pos && g_fairy_skill_config->is_buff(skill_id)) {
		uint32_t skill_attr = pPlayer->get_fairy_attr(entity_pos,ITEM_ATTR_SKIATK);
		DEBUG_LOG("CAR DEBUG FAIRY SKILL ATTACK ATTR %u",skill_attr);
		CSBasicUserBattleInfoRsp rsp;
		pPlayer->add_buff(skill_id,skill_attr);
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);
		uint32_t blood = g_fairy_skill_config->add_blood(skill_id,skill_attr);
		if(blood) {
			CSBattleUserAddBloodRsp blood_rsp;
			blood_rsp.set_id(pPlayer->id());
			blood_rsp.set_reg_time(pPlayer->regtime());
			blood_rsp.set_channel_id(pPlayer->channelid());
			blood_rsp.set_blood(blood);	
			// 发个一组玩家
			pPlayer->send_msg_to_wb_btl_observers(blood_rsp);
		}
	}
	// 发个一组玩家
	pPlayer->send_msg_to_wb_btl_observers(rsp);
}

int BabelBattle::update()
{
	DEBUG_LOG("CAR DEBUG UPDATE");
	if(monster_clear()) {
		CSMonsterDieRsp rsp;
		Monster* monster = monsters_.next_ref(NULL);
		while(monster != NULL) {
			rsp.add_monsters(monster->get_id().id());				
			monster = monsters_.next_ref(monster);
		}
		send_to_players(rsp);
		safe_release();
		return 0;
	}
	update_effect();	
	CSMonsterMoveRsp move_rsp;
	CSMonsterAttackRsp attack_rsp;
	CSMonsterEffectRsp effect_rsp;
	monsters_.update(&move_rsp, &attack_rsp, &effect_rsp);
	if(move_rsp.moves_size()) {
		send_to_players(move_rsp);
	}
	if(attack_rsp.attacks_size()) {
		send_to_players(attack_rsp);
	}
	if(effect_rsp.effects_size()) {
		send_to_players(effect_rsp);
	}
	CSBuffEffectRsp buff_effect_rsp;
	monsters_.do_buff(&buff_effect_rsp);
	if(buff_effect_rsp.effects_size()) {
		send_to_players(buff_effect_rsp);
	}
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead())
			monsters_.try_locking(pPlayer);
		pPlayer = next_ref(pPlayer);
	}
	return 0;
}

void BabelBattle::on_buffer_finish(Entity* entity) {
	Player *pPlayer = dynamic_cast<Player*>(entity);
	if(pPlayer) {
		CSBasicUserBattleInfoRsp rsp;
		rsp.set_id(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		BasicUserBattleInfo* info = rsp.mutable_battle_info();
		pPlayer->pack_basic_battle_info(info);
		// 发个一组玩家
		pPlayer->send_msg_to_wb_btl_observers(rsp);
	}
}

void BabelBattle::move(Player* pPlayer, const CSBattleMoveReq * req) {
/*Todo must check the available of move*/
	TileWorld<>::Point pos(req->pos().xpos(), req->pos().ypos());
	if(!m_tile_world->is_pos_walkable(pos)) {
		ERROR_LOG("PLAYER:%u CAN'T MOVE TO UNWALKALBE POS:%d %d", pPlayer->id(), pos.x, pos.y);
		return; 
	};
	pPlayer->m_pos.x = req->pos().xpos();
	pPlayer->m_pos.y = req->pos().ypos(); 
	pPlayer->m_direct.x = req->direct().xpos();
	pPlayer->m_direct.y = req->direct().ypos();
	//DEBUG_LOG("PLAYER %u MOVE TO: %d %d", pPlayer->id(),pPlayer->m_pos.x, pPlayer->m_pos.y);
	CSBattleMoveRsp rsp;
	rsp.set_id(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	CSPoint* one_point =rsp.mutable_pos();
	one_point->set_xpos(pPlayer->m_pos.x);
	one_point->set_ypos(pPlayer->m_pos.y);
	CSPoint* one_direct =rsp.mutable_direct();
	one_direct->set_xpos(pPlayer->m_direct.x);
	one_direct->set_ypos(pPlayer->m_direct.y);
	rsp.set_time(req->time());
	// 发个一组玩家
	pPlayer->send_msg_to_wb_btl_observers(rsp);
}

void BabelBattle::update_effect() {
	m_effect_manager.update();
	m_effect_manager.effect_battle(this);
	Player* pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead() && players_load_complete_[pPlayer]) {
			CSMonsterHitRsp hit_rsp;
			m_effect_manager.effect(pPlayer, &hit_rsp);
			if(hit_rsp.users_size()) {
				// 发个一组玩家
				pPlayer->send_msg_to_wb_btl_observers(hit_rsp);
			}
		}
		pPlayer = next_ref(pPlayer);
	}
}

void BabelBattle::on_player_leave(base::Manager<Player>*, Player* pPlayer)
{
	DEBUG_LOG("CAR DEBUG Babel p %u LEAVE", pPlayer->id());
	CSBattleUserLeaveRsp rsp;
	rsp.set_userid(pPlayer->id());
	rsp.set_reg_time(pPlayer->regtime());
	rsp.set_channel_id(pPlayer->channelid());
	// 发送给一组用户，除了自己
	pPlayer->send_msg_to_wb_btl_observers(rsp, pPlayer);
	// send_to_players_except(rsp, pPlayer);
	pPlayer->sig_leave_battle(pPlayer);
	players_load_complete_.erase(pPlayer);
	pPlayer->sig_dead.disconnect(this);
}
void BabelBattle::on_player_dead(Entity* entity)
{
	Player* pPlayer = static_cast<Player*> (entity);
	if(pPlayer) {
		DEBUG_LOG("CAR DEBUG Babel p %u DEAD", pPlayer->id());
		CSBattlePlayerDelRsp rsp;
		rsp.set_userid(pPlayer->id());
		rsp.set_reg_time(pPlayer->regtime());
		rsp.set_channel_id(pPlayer->channelid());
		rsp.set_exchange_id(0);
		// 发送给一组用户
		pPlayer->send_msg_to_wb_btl_observers(rsp);
		// 单次死亡结算在wb_instance中
		// 死亡复活定时器
		pPlayer->on_player_wb_dead();
		// 死亡重新加入时间限制
		pPlayer->set_property(ATTR_WBOSS_REVIVAL_TIME, base::Time().seconds() + WBOSS_REVIVAL_CD);
	}
}

int BabelBattle::sync_boss_info()
{
	// 每秒同步boss血量信息给所有玩家
	CSBBMonsterBloodRsp blood_rsp;
	Monster *pmonster = monsters_.next_ref(NULL);
	while(pmonster!= NULL) {
		if(pmonster->is_boss()){
			CSBBMonsterBloodRsp_MonsterInfo* rsp_info = blood_rsp.add_monsters();
			rsp_info->set_id(pmonster->get_id().id());
			rsp_info->set_blood(pmonster->m_prop.health_point);
			rsp_info->set_total_hp(pmonster->m_fightprop.health_point);
		}

		pmonster = monsters_.next_ref(pmonster);
	}
	// 发送给在战场中所有玩家
	Player *pPlayer = next_ref(NULL);
	while(pPlayer != NULL) {
		if(!pPlayer->dead()) {
			pPlayer->send_msg(blood_rsp);
		}
		pPlayer = next_ref(pPlayer);
	}

	return 0;
}


void BabelBattle::pack_main_info(CSBattleCreateRsp* rsp, Player *pPlayer) {
	if (pPlayer!=NULL) {
		BasicUserInfo* info = rsp->add_userinfo();
		pPlayer->pack_basic_info(info);
		info->set_battle_side(pPlayer->get_group());
		pPlayer->pack_basic_battle_info(info);
	}
	Monster* monster = monsters_.next_ref(NULL);
	while(monster != NULL) {
		MonsterInfo* monsterinfo = rsp->add_monsterinfo();
		monsterinfo->set_id(monster->get_id().id());
		monsterinfo->set_type_id(monster->get_type_id());
		monsterinfo->set_level(monster->get_level());
		monsterinfo->set_blood(monster->m_prop.health_point);
		monsterinfo->set_boss(monster->is_boss());
		monsterinfo->set_speed(monster->get_speed());
		monsterinfo->set_elite(monster->is_elite());
		monsterinfo->set_hp_line(monster->get_hp_line());
		CSPoint* point = monsterinfo->mutable_point();
		point->set_xpos(monster->m_pos.x);
		point->set_ypos(monster->m_pos.y);
		CSPoint* direct = monsterinfo->mutable_direct();
		direct->set_xpos(monster->m_direct.x);
		direct->set_ypos(monster->m_direct.y);
		MonsterMoveValue * moves = monsterinfo->mutable_moves();
		monster->pack_move(moves);
		MonsterBattleInfo* info = monsterinfo->mutable_battle_info();
		monster->pack_battle_info(info);
		monster = monsters_.next_ref(monster);
	}
	rsp->set_battle_id(get_battle_id());
	rsp->set_map_id(get_map_id());
	CSPoint* portal = rsp->mutable_portal();
	portal->set_xpos(m_portal.x);
	portal->set_ypos(m_portal.y);
	rsp->set_portal_map_id(get_next_map_id());
	for(std::vector< TileWorld<>::Point>::iterator itr =m_monster_born.begin(); itr!=m_monster_born.end(); ++itr) {
		CSPoint* point = rsp->add_monsterbirth();	
		point->set_xpos(itr->x);
		point->set_ypos(itr->y);	
	}
}

bool BabelBattle::set_complete() {
	if(!m_complete) {
		m_complete = true;
		sGloble.Timer.add_event(*this, &BabelBattle::update, base::Time(), UPDATETIME, -1);
		sGloble.Timer.add_event(*this, &BabelBattle::sync_boss_info, base::Time(), UPDATETIME*5, -1);
	}
	return true;	
}

#endif

BattlefieldManager::BattlefieldManager()
{
};

Battlefield* BattlefieldManager::create_battle(Player* pPlayer, uint32_t battle_id, uint32_t map_id, bool transfer) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		pbattle->reduce_obj(pPlayer);
	};
	Instance_t* instance = g_instance_config->get_instance_info(battle_id);	
	if(instance && (instance->type == PPVE || instance->type == PVE || instance->type == PVE2 || instance->type == PVE3 || instance->type == GAMEBEGIN )) {
		pbattle = new Battlefield(pPlayer, battle_id, map_id, transfer);
		add_manager(pbattle);
	} else if(instance && (instance->type == PVP || instance->type == PVP2 || instance->type == PVP3 || instance->type == PVP4)) {
		pbattle = new PVPBattle(pPlayer, battle_id, map_id);
		add_manager(pbattle);
	}  else if(instance && (instance->type == CDEF)) {
		pbattle = new CDEFBattle(pPlayer, battle_id, map_id);
		add_manager(pbattle);
	}
#ifndef FRONT
	else if(instance && (instance->type == PVAI)) {
		pbattle = new AiBattle(pPlayer, battle_id, map_id);
		add_manager(pbattle); 
	} else if (instance && (instance->type == PVAI2)) {
		pbattle = new AiBattle2(pPlayer, battle_id, map_id);
		add_manager(pbattle);
	}
#endif
 	else {
		ERROR_LOG("ERROR BATTLE_ID:%d",battle_id);
	}
	return pbattle;
}

#ifndef FRONT
WBossBattle* BattlefieldManager::create_wboss_battle(uint32_t battle_id)
{
	WBossBattle* pbattle = NULL; 
	Instance_t* instance = g_instance_config->get_instance_info(battle_id);
	if (instance) {
		pbattle = new WBossBattle(battle_id);
		add_manager(pbattle);
	} else {
		ERROR_LOG("ERROR BATTLE_ID:%d",battle_id);
	}
	return pbattle;
}

int BattlefieldManager::wb_btl_revival(Player* pPlayer)
{
	WBossBattle* pbattle = static_cast<WBossBattle*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		pbattle ->revival_by_cd_over(pPlayer);
	} else {
		DEBUG_LOG("%u WBOSS BTL REVIVAL WITHOUT BATTLE",pPlayer->id());
	}
	return 0;

}

BabelBattle* BattlefieldManager::create_babel_battle(uint32_t battle_id)
{
	BabelBattle* pbattle = NULL; 
	Instance_t* instance = g_instance_config->get_instance_info(battle_id);
	if (instance) {
		pbattle = new BabelBattle(battle_id);
		add_manager(pbattle);
	} else {
		ERROR_LOG("ERROR BATTLE_ID:%d",battle_id);
	}
	return pbattle;

}
#endif
	
Battlefield* BattlefieldManager::get_battle(Player* pPlayer) {
	return static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
}
	
int BattlefieldManager::leave_battle(Player* pPlayer, bool rsp_msg) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		DEBUG_LOG("leave pbattle");
		pbattle->reduce_obj(pPlayer);
	}
#ifndef FRONT
	if(rsp_msg) {
		CSBattleLeaveRsp rsp;
		rsp.set_initiative(true);
		pPlayer->send_msg(rsp);
	}
#endif
	return 0;
}

int BattlefieldManager::wb_leave_battle(Player* pPlayer)
{
#ifndef FRONT
	WBossBattle* wbbattle = dynamic_cast<WBossBattle*>(get_manager_by_key(pPlayer->get_id()));
	if (wbbattle) {
		DEBUG_LOG("leave wboss");
		wbbattle->reduce_obj(pPlayer);
	}
#endif
	return 0;
}

int BattlefieldManager::bb_leave_battle(Player* pPlayer)
{
#ifndef FRONT
	BabelBattle* wbbattle = dynamic_cast<BabelBattle*>(get_manager_by_key(pPlayer->get_id()));
	if (wbbattle) {
		DEBUG_LOG("CAR DEBUG BABEL");
		wbbattle->reduce_obj(pPlayer);
	}
#endif
	return 0;

}

int BattlefieldManager::player_attack(Player* pPlayer,const CSPlayerAttackReq * req) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		//DEBUG_LOG("RECV ATTACK ID:%s",req->DebugString().c_str());
		if(pPlayer->m_prop.health_point && pbattle->get_complete())
			pbattle->attack(pPlayer, req);
		return 0;
	} else {
		ERROR_LOG("%u ATTACK WITHOUT BATTLE",pPlayer->id());
	}
	return 0;
}
	
int BattlefieldManager::player_attack(Player* pPlayer, const CSPlayerHitPlayerReq * req) {
	PVPBattle* pbattle = dynamic_cast<PVPBattle*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		//DEBUG_LOG("RECV ATTACK ID:%s",req->DebugString().c_str());
		if(pPlayer->m_prop.health_point && pbattle->get_complete())
			pbattle->attack(pPlayer, req);
		return 0;
	}
#ifndef FRONT
	AiBattle* aibattle = dynamic_cast<AiBattle*>(get_manager_by_key(pPlayer->get_id()));
	if(aibattle) {
		if(pPlayer->m_prop.health_point)
			aibattle->attack(pPlayer, req);
		return 0;
	}
	AiBattle2* aibattle2 = dynamic_cast<AiBattle2*>(get_manager_by_key(pPlayer->get_id()));
	if(aibattle2) {
		aibattle2->attack(pPlayer, req);
		return 0;
	}
	ERROR_LOG("%u ATTACK WITHOUT BATTLE",pPlayer->id());
#endif
	return 0;
}
	
int BattlefieldManager::skill_begin(Player* pPlayer, const CSSkillBeginReq * req) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		//DEBUG_LOG("RECV ATTACK ID:%s",req->DebugString().c_str());
		if(pPlayer->m_prop.health_point)
			pbattle->skill_begin(pPlayer, req);
		return 0;
	} else {
		ERROR_LOG("%u ATTACK WITHOUT BATTLE",pPlayer->id());
	}
	return 0;
}

int BattlefieldManager::move(Player* pPlayer, const CSBattleMoveReq * req) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		pbattle -> move(pPlayer, req);
	} else {
		DEBUG_LOG("%u MOVE WITHOUT BATTLE",pPlayer->id());
	}
	return 0;
}
	
int BattlefieldManager::monster_die(Player* pPlayer, const CSMonsterDieReq *req) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		pbattle -> monster_die(pPlayer, req);
	} else {
		DEBUG_LOG("%u MONSTER DIE WITHOUT BATTLE",pPlayer->id());
	}
	return 0;
}
	
int BattlefieldManager::revival(Player* pPlayer) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		pbattle -> revival(pPlayer);
	} else {
		DEBUG_LOG("%u MOVE WITHOUT BATTLE",pPlayer->id());
	}
	return 0;
}
	
bool BattlefieldManager::exist(const Player* pPlayer) {
	return exist_obj(pPlayer);
}
	
uint32_t BattlefieldManager::map_id(Player* pPlayer) {
	Battlefield* pbattle = static_cast<Battlefield*>(get_manager_by_key(pPlayer->get_id()));
	if(pbattle) {
		return 	pbattle -> get_map_id();
	} else {
		return 0;
	}
}

uint32_t BattlefieldManager::set_pvp_mode(Player* pPlayer, uint32_t mode)
{
	PVPBattle* pvpbattle = dynamic_cast<PVPBattle*>(get_manager_by_key(pPlayer->get_id()));
	if(pvpbattle) {
		pvpbattle->set_mode(mode);
	}
	return 0;
}
