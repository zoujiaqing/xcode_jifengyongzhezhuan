#ifndef FRONT
#include "../stat/stat.h"
#include "wb_instance.h"
#include "babel_instance.h"
#endif
#include "instance.h"
#include "../proto/errcode.pb.h"
	
Instance::Instance(uint32_t instance_id, InstanceType type, uint32_t timeout, uint32_t star_time):
	m_instance_id(instance_id),
	m_current_map_id(0),
	m_type(type),
	m_timeout(timeout),
	m_finish_time(0),
	m_star_time(star_time),
	m_dead_mon_num(0),
	m_p_ai(NULL)
{
	/*get first map_info*/
	MapContent_t* mapcontent  = g_instance_config->get_mapcontent_info(m_instance_id, 0);
	if (!mapcontent) {
		ERROR_RETURN_VOID("ASSERT not found MapContent_t! m_instance_id:%u", m_instance_id);
	}
	m_current_map_id = mapcontent->map_id;
	sig_obj_pre_decrease.connect(this, &Instance::on_player_leave);
}
	
void Instance::player_join(Player* pPlayer) {
	if(pPlayer->is_ai()) {
		m_p_ai = pPlayer;
	} else {
		add_obj(pPlayer);
	}
}
	
int Instance::init() {
	if(!empty() && m_current_map_id) {
		Player* pPlayer = next_ref(NULL);
		if(pPlayer) {
			sBattlefield.create_battle(pPlayer, m_instance_id, m_current_map_id);
			Battlefield* pbattle = sBattlefield.get_battle(pPlayer); 
			pPlayer->sig_dead.connect(this,&Instance::on_player_dead);
			pPlayer->sig_health_point.connect(this,&Instance::on_player_hp_change);
			if(pbattle) {
				pPlayer = next_ref(pPlayer);
				while(pPlayer) {
					pPlayer->sig_dead.connect(this,&Instance::on_player_dead);
					pPlayer->sig_health_point.connect(this,&Instance::on_player_hp_change);
					pbattle->player_join(pPlayer);
					pPlayer = next_ref(pPlayer);
				}
				if(m_p_ai) {
					pbattle->player_join(m_p_ai);
				}
				m_battlefields.add_obj(pbattle);
				pbattle->sig_obj_del.connect(this,&Instance::on_battle_leave);
				pbattle->sig_obj_pre_decrease.connect(this,&Instance::on_player_leave_battle);			
#ifndef FRONT
				if(!(m_type==PPVE || m_type ==CDEF)) {
					pbattle->init_defence_map_server();
				}
#endif
				CSBattleCreateRsp rsp;			
				pbattle->pack_main_info(&rsp);
				rsp.set_timeout(m_timeout);
				pPlayer = next_ref(NULL);
				while(pPlayer) {
					m_user_complete[pPlayer] = false;
					DEBUG_LOG("p %u %p set false",pPlayer->id(), pPlayer);
#ifndef FRONT
					if(!is_pve())
#endif
					{
						pPlayer->send_msg(rsp);
					}
#ifndef FRONT
					if (m_type == PVE3) {	// 地下城副本，进对战就算次数
						pPlayer->do_once_daily_goals(ATTR_DAILY_TIMES_PVE3);
					}
					STAT_MGR.join_in_instance(pPlayer,m_type,m_instance_id);
#endif
					DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u", ST_JOIN_INSTANCE, pPlayer->zone_id(), base::Time().seconds(), pPlayer->name().c_str(), pPlayer->id(), pPlayer->regtime(), pPlayer->channelid(), pPlayer->type(), m_instance_id, m_type, pPlayer->lv());
#ifndef FRONT
					if(m_type == PVP3 || m_type == PVP4){
						if(pPlayer->property(ATTR_PLAYER_PVP_RED)){
							pPlayer->add_buff(0,0 - pPlayer->property(ATTR_PLAYER_PVP_RED));
							CSBasicUserBattleInfoRsp rsp;
							rsp.set_id(pPlayer->id());
							rsp.set_reg_time(pPlayer->regtime());
							rsp.set_channel_id(pPlayer->channelid());
							BasicUserBattleInfo* info = rsp.mutable_battle_info();
							pPlayer->pack_basic_battle_info(info);
							pbattle->send_to_players(rsp);
						}
					}
#endif
					pPlayer = next_ref(pPlayer);
				}
				if((m_type == PPVE || m_type == PVP || m_type == PVP2 || m_type == PVP3 || m_type == PVP4 || m_type == CDEF) && size() > 1 ) { 	
					m_start_timeout_handler = sGloble.Timer.add_event(*this, &Instance::start_battle_timeout, m_current_map_id, base::Time()+LOADWAITTIMEOUT);
				}
			}
		}
		else{
			ERROR_LOG("the instance do not have player");
		}

	}
	m_p_ai = NULL;
	return 0;
}
	
int Instance::join_timeout(uint32_t& map_id) {
	if(map_id == m_current_map_id) {
		Battlefield* pbattle = m_battlefields.get_ref_by_key(map_id);
		if(pbattle) {
			Player* pPlayer = next_ref(NULL);	
			while(pPlayer) {
				Battlefield* pbattle_now = static_cast<Battlefield*>(sBattlefield.get_manager_by_key(pPlayer->get_id()));
				if(pbattle_now != pbattle) {
					pbattle_now->reduce_obj(pPlayer);
					pbattle->player_join(pPlayer);	
					CSBattleCreateRsp rsp;			
					pbattle->pack_main_info(&rsp);
					pPlayer->send_msg(rsp);	
				}
				pPlayer = next_ref(pPlayer);
			}
			m_start_timeout_handler = sGloble.Timer.add_event(*this, &Instance::start_battle_timeout, map_id, base::Time()+LOADWAITTIMEOUT);
		}
	}
	return 0;
}
	
int Instance::start_battle_timeout(uint32_t& map_id) {
	std::vector<Player*> leave_players;
	CompleteMap::iterator itr = m_user_complete.begin();
	for(; itr!= m_user_complete.end();) {
		CompleteMap::iterator tmp_itr = itr;
		tmp_itr ++;
		if(!(itr->second)) {
			leave_players.push_back(itr->first);	
		}
		itr = tmp_itr;
	}
	std::vector<Player*>::iterator leave_itr = leave_players.begin();
	for(;leave_itr!=leave_players.end();leave_itr++) {
		CSBattleLeaveRsp rsp;	
		rsp.set_initiative(true);
		Player* pPlayer = *leave_itr;
		Battlefield* pbattle = m_battlefields.get_ref_by_key(map_id);
		if(pbattle) {
			pbattle->reduce_obj(pPlayer);
			reduce_obj(pPlayer);
		}
		pPlayer->send_msg(rsp);	
		pPlayer->send_stderr(olerr_start_battle_timeout);
	}
	return 0;
}
	
int Instance::start_battle(uint32_t& map_id) {
	Battlefield* pbattle = m_battlefields.get_ref_by_key(map_id);
	if(pbattle) {
		CompleteMap::iterator itr = m_user_complete.begin();
		for(; itr!= m_user_complete.end(); ++ itr) {
			itr->second = false;
			DEBUG_LOG("p %u %p start battle set false",itr->first->id(), itr->first);
		}
		base::Time now;
		m_start_time = now;
#ifndef FRONT
		if(m_timeout) {
			m_timeout_handler =	sGloble.Timer.add_event(*this, &Instance::dead_by_timeout, now+m_timeout);
		}
#endif
		DEBUG_LOG("BATTLE START %d TIMEOUT:%d", now.seconds(), m_timeout);
#ifndef FRONT
		if(!is_pve())
#endif
		{
			if(pbattle->set_complete()) {
				m_start_timeout_handler.deactivate();
				m_join_timeout_handler.deactivate();
				pbattle->init_defence_map();
			}
		}
	} else {
		ERROR_LOG("ERROR START BATTLE ID:%d",map_id);
	}
	return 0;
}
	
void Instance::transfer_next(Player* pPlayer) {
	Battlefield* pbattle_now = static_cast<Battlefield*>(sBattlefield.get_manager_by_key(pPlayer->get_id()));
	if(pbattle_now && pbattle_now->monster_clear()) {
		MapContent_t* mapcontent  = g_instance_config->get_mapcontent_info(m_instance_id, pbattle_now->get_map_id());
		if (!mapcontent) {
			ERROR_RETURN_VOID("ASSERT not found MapContent_t! m_instance_id:%u map_id:%u", 
					m_instance_id, pbattle_now->get_map_id());
		}
		m_current_map_id = mapcontent->trans_des;
		if(m_current_map_id) {
			Battlefield* pbattle = m_battlefields.get_ref_by_key(m_current_map_id);
			if(pbattle) {
				pbattle_now->reduce_obj(pPlayer);
				pbattle->player_join(pPlayer);
			} else {
				pbattle = sBattlefield.create_battle(pPlayer, m_instance_id, m_current_map_id, true);
				if(pbattle) { 
					m_battlefields.add_obj(pbattle);
					if((m_type == PPVE || m_type == CDEF ) && size()>1 ) {
						m_join_timeout_handler = sGloble.Timer.add_event(*this, &Instance::join_timeout, m_current_map_id, base::Time()+JOINTWAITTIMEOUT);
					}
					pbattle->sig_obj_del.connect(this,&Instance::on_battle_leave);
					pbattle->sig_obj_pre_decrease.connect(this,&Instance::on_player_leave_battle);			
				}
			}
			if(pbattle) {
				m_timeout_handler.deactivate();
				base::Time now;
				if(m_timeout) {
					if(now.seconds() < m_start_time.seconds() + m_timeout) {
						m_timeout = m_timeout + m_start_time.seconds() - now.seconds() ; 
					} else {
						m_timeout = 1;
					}
				}
				uint32_t now_s = now.seconds();
				DEBUG_LOG("GET %d TIMEOUT:%d", now_s, m_timeout);
				m_finish_time += now.seconds() - m_start_time.seconds();
#ifndef FRONT
				if(!is_pve())
#endif
				{
					CSBattleCreateRsp rsp;			
					pbattle->pack_main_info(&rsp);
					rsp.set_timeout(m_timeout);
					pPlayer->send_msg(rsp);
				}
			}
		}
	} else {
		DEBUG_LOG("p %u MULTI TRANSFER NEXT CURRENT MAP ID:%d",pPlayer->id(), m_current_map_id);
		pPlayer->send_stderr(olerr_can_transfer_next);
	}
}
	
void Instance::on_player_leave(base::Manager<Player>*, Player* pPlayer) {
	m_user_complete.erase(pPlayer);	
	DEBUG_LOG("p %u %p erase",pPlayer->id(), pPlayer);
	pPlayer->m_attribute_list.del("revival_times");
#ifndef FRONT
	pPlayer->clear_buff();
	bool stat_btl_count = false;
	// 失败奖励奖励
	// 执行至此的player都经过reduce_obj ,故都是losera
	switch(m_type) {
		case PVE:
			/*统计副本获得星数,这里是失败的*/
			STAT_MGR.instance_star(pPlayer,m_instance_id,0);
			stat_btl_count = true;
			break;
		case PVP:
			pPlayer->finish_pvp_battle(false);
			break;
		case PVAI:
		case PVAI2:
			if (!pPlayer->is_ai()) {
				pPlayer->finish_pvai_battle(m_type, false);
			}
			break;
		case PVP2:
			pPlayer->finish_pvp2_battle(m_instance_id, PVP2_BTL_FAIL);
			break;
		case PVP3:
			pPlayer->finish_pvp3_battle(m_instance_id, false);
			break;
		case PVP4:
			pPlayer->finish_pvp4_battle(m_instance_id, false);
			break;
		case PPVE:
		case PVE2:
		case PVE3:
		case CDEF:
		case GAMEBEGIN:
			stat_btl_count = true;
		default:
			break;
	}
	if (stat_btl_count) {
		pPlayer->add_property(ATTR_ACCU_FIGHT_COUNT, 1);
	}
	DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u %u", ST_LEAVE_INSTANCE, pPlayer->zone_id(), base::Time().seconds(), pPlayer->name().c_str(), pPlayer->id(), pPlayer->regtime(), pPlayer->channelid(), pPlayer->type(), m_instance_id, m_type, 0, pPlayer->lv());
#else 
	if(m_user_complete.empty()) {
		return ;
	}
#endif
	CompleteMap::iterator itr = m_user_complete.begin();
	for(; itr!= m_user_complete.end(); ++ itr) {
		if(!itr->second) {
			return ;
		}
	}
	start_battle(m_current_map_id);
}
	
void Instance::on_player_dead(Entity* pPlayer) {

}
	
void Instance::on_player_hp_change(Entity* p, uint32_t hp, Entity* source) {
	if(m_instance_id != GAMEBEGIN_BATTLE) {
		CSPlayerHPRsp rsp;
		rsp.set_userid(p->get_id().id());
		rsp.set_reg_time(p->get_id().regtime());
		rsp.set_channel_id(p->get_id().channelid());
		rsp.set_blood(hp);
		if(source) {
			rsp.set_source_id(source->get_id().id());
			rsp.set_source_reg_time(source->get_id().regtime());		
			rsp.set_source_channel_id(source->get_id().channelid());		
		}
		Player* pPlayer = next_ref(NULL);
		while(pPlayer) {
			pPlayer->send_msg(rsp);
			pPlayer = next_ref(pPlayer);
		}
	} else {
		p->m_prop.health_point = p->m_fightprop.health_point;
	}
}
	
void Instance::load_complete(Player* pPlayer) {
	DEBUG_LOG("p %u %p INSTANCE LOAD COMPLETE",pPlayer->id(), pPlayer);
	m_user_complete[pPlayer] = true;
	Battlefield* pbattle = m_battlefields.get_ref_by_key(m_current_map_id);
	if(pbattle) {
		pbattle->load_complete(pPlayer);
	}
	CompleteMap::iterator itr = m_user_complete.begin();
	for(; itr!= m_user_complete.end(); ++ itr) {
		if(!itr->second) {
			DEBUG_LOG("p %u INSTANCE LOAD COMPLETE WAIT FOR OTHERS:%d %p",pPlayer->id(), itr->first->id(), itr->first);
			return ;
		}
	}
	start_battle(m_current_map_id);
}
	
int Instance::dead_by_timeout() {
	#ifndef FRONT
	if(m_type == PVAI || m_type == PVAI2) {
		uint32_t now = base::Time().seconds();
		DEBUG_LOG("PVAI DEAD BY TIMEOUT:%d", now);
		uint32_t ai_hp_percent = 0;
		AiBattle* aibattle = dynamic_cast<AiBattle*>(m_battlefields.get_ref_by_key(m_current_map_id));
		if(aibattle) {
			ai_hp_percent = aibattle->get_ai_hp_percent();
		} else {
			return 0;
		}
		Player* pPlayer = next_ref(NULL);	
		while(pPlayer) {
			if(!pPlayer->is_ai()) {
				if(pPlayer->m_prop.health_point * 100 / pPlayer->m_fightprop.health_point  > ai_hp_percent) {
					aibattle->set_ai_hp(0);
				} else {
					pPlayer->m_prop.health_point = 0;
				}
			}
			pPlayer = next_ref(pPlayer);
		}
		return 0;
	} else if (m_type == PVP || m_type == PVP2 || m_type == PVP3 || m_type == PVP4) {
		uint32_t now = base::Time().seconds();
		DEBUG_LOG("PVP DEAD BY TIMEOUT:%d", now);
		uint32_t group_now_hp1, group_now_hp2;
		group_now_hp1 = group_now_hp2 = 0;
		PVPBattle* pvp_battle = dynamic_cast<PVPBattle*>(m_battlefields.get_ref_by_key(m_current_map_id));
		if (!pvp_battle) return 0;
		Player* pPlayer = next_ref(NULL);
		while(pPlayer) {
			if(pPlayer->get_group() % 2){
				group_now_hp1 += pPlayer->m_prop.health_point;
			} else {
				group_now_hp2 += pPlayer->m_prop.health_point;
			}
			pPlayer = next_ref(pPlayer);
		}
		uint32_t win_group 
			= (group_now_hp1 * 1.0 / pvp_battle->group_total_hp1() > group_now_hp2 * 1.0 / pvp_battle->group_total_hp2()) ? 1 : 0;
		pPlayer = next_ref(NULL);
		while(pPlayer) {
			if(pPlayer->get_group() % 2 != win_group) {
				pPlayer->m_prop.health_point = 0;
			} 
			pPlayer = next_ref(pPlayer);
		}
		return 0;
	} else 
	#endif
	{
		uint32_t now = base::Time().seconds();
		DEBUG_LOG("DEAD BY TIMEOUT:%d", now);
		CSBattleTimeoutRsp rsp;
		Player* pPlayer = next_ref(NULL);	
		while(pPlayer) {
			#ifndef FRONT
			pPlayer->send_msg(rsp);
			#endif
			sBattlefield.leave_battle(pPlayer, false);
			pPlayer = next_ref(pPlayer);
		}
	}
	delete this;
	return 0;
}
	
void Instance::on_player_leave_battle(base::Manager<Player>* manager, Player* pPlayer) {
	if((static_cast<Battlefield*>(manager))->get_id() == m_current_map_id)
		reduce_obj(pPlayer);
}
	
void Instance::player_hp_change(Player* pPlayer, int blood) {
	uint32_t flag = 0;			//统计判断，现在发现这个函数会被多次调用
	if (is_pve()) {
		if(pPlayer->m_prop.health_point){
			flag ++;
		}
		DEBUG_LOG("HP CHANGE : %d %d",pPlayer->m_prop.health_point, -blood); 
		if((int)pPlayer->m_prop.health_point < -blood) {
			flag++;
			pPlayer->m_prop.health_point = 0;
		} else {
			pPlayer->m_prop.health_point += blood;
		}
		if(pPlayer->m_prop.health_point > pPlayer->m_fightprop.health_point) {
			pPlayer->m_prop.health_point = pPlayer->m_fightprop.health_point;
		}
		if(!pPlayer->m_prop.health_point) {
			if(flag == 2){
				// 统计：玩家死亡
				DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u", ST_DEAD_RECORD, pPlayer->zone_id(), base::Time().seconds(), pPlayer->name().c_str(), pPlayer->id(), pPlayer->regtime(), pPlayer->channelid(), pPlayer->type(), m_instance_id, m_type, pPlayer->lv());
#ifndef FRONT
				STAT_MGR.role_die(pPlayer,m_instance_id);
#endif
				pPlayer->sig_dead(pPlayer);

			}
		}
	}
}
	
bool Instance::is_pve() {
	return (m_type == PVE || m_type == PVE2 || m_type == PVE3 || m_type == GAMEBEGIN);
}
	
void Instance::on_battle_leave(base::Object<uint32_t>* pbattle) {
	base::Time now;
	Battlefield* p_battle_dead = static_cast<Battlefield*>(pbattle);
	if (p_battle_dead) {
		m_dead_mon_num += p_battle_dead->get_dead_mon_num();
	}
	if(m_battlefields.empty()) {
		Player* pPlayer= next_ref(NULL);
		while(pPlayer !=NULL) {
			pPlayer->m_attribute_list.del("revival_times");
#ifndef FRONT
			pPlayer->clear_buff();
#endif
			pPlayer = next_ref(pPlayer);
		}
		m_finish_time += now.seconds() - m_start_time.seconds();
		switch(m_type) {
			case PVE:		// 普通副本
			case PVE2:		// 精英副本
			case PVE3:		// 地下城副本
			case PPVE:		// 多人副本	(TODO 多人副本用finish_pve_battle是否合适，奖励问题)
			case CDEF:		// 主城防守
			case GAMEBEGIN:
#ifndef FRONT
				this->finish_pve_battle();
#endif
				break;
			case PVP:		// 角斗场
				this->finish_pvp_battle();
				break;
			case PVAI:		// 竞技场
			case PVAI2:		// 切磋
				this->finish_pvai_battle();
				break;
			case PVP2:		// 最强勇者
				this->finish_pvp2_battle();
				break;
			case PVP3:
				this->finish_pvp3_battle();
				break;
			case PVP4:
				this->finish_pvp4_battle();
				break;
			default:
				break;
		}
		delete this;
	}
}

void Instance::finish_pve_battle()
{
	Player* pPlayer = next_ref(NULL);
	while(pPlayer !=NULL) {
		CSBattleLeaveRsp leave_rsp;
		leave_rsp.set_initiative(false);
		uint32_t star = 0;
		if(m_finish_time <= 0.3 * m_star_time) {
			star = 4;
		} else if(m_finish_time <= 0.5 * m_star_time ) {
			star = 3;
		} else if(m_finish_time <= 0.8 * m_star_time ) {
			star = 2; 
		} else {
			star = 1;
		}
		leave_rsp.set_star(star - 1);	// 方便客户端显示用，所以需要减1
		leave_rsp.set_finish_time(m_finish_time);
		leave_rsp.set_dead_mon_num(m_dead_mon_num);
		pPlayer->finish_instance(m_instance_id, star, leave_rsp);
		pPlayer->send_msg(leave_rsp);
		// 完成副本
		DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u %u", ST_COMPLETE_INSTANCE, pPlayer->zone_id(), base::Time().seconds(), pPlayer->name().c_str(), pPlayer->id(), pPlayer->regtime(), pPlayer->channelid(), pPlayer->type(), m_instance_id, m_type, star - 1, pPlayer->lv());
#ifndef FRONT
		/*统计副本获得星数*/
		if(m_type == PVE){
			STAT_MGR.instance_star(pPlayer,m_instance_id,star);
		}
		if(m_instance_id == GAMEBEGIN_BATTLE) {
			pPlayer->after_game_first_start();
			pPlayer->set_once(FIRST_BATTLE);
		}
		// 记录战斗场次及其胜利场次
		pPlayer->add_property(ATTR_ACCU_FIGHT_COUNT, 1);
		pPlayer->add_property(ATTR_ACCU_FIGHT_WIN_COUNT, 1);
#endif
		pPlayer = next_ref(pPlayer);
	}
}

void Instance::finish_pvp_battle()
{
#ifndef FRONT
	// 执行至此的player都没有经过reduce_obj ,故都是winner
	Player* pPlayer = next_ref(NULL);
	while(pPlayer !=NULL) {
		pPlayer->finish_pvp_battle(true);
		pPlayer = next_ref(pPlayer);
	}
#endif
}

void Instance::finish_pvai_battle()
{
#ifndef FRONT
	// 执行至此的player都没有经过reduce_obj ,故都是winner
	Player* pPlayer = next_ref(NULL);
	while(pPlayer !=NULL) {
		if (!pPlayer->is_ai()) {
			pPlayer->finish_pvai_battle(m_type, true);
		}
		pPlayer = next_ref(pPlayer);
	}
#endif
}

void Instance::finish_pvp2_battle()
{
#ifndef FRONT
	// 执行至此的player都没有经过reduce_obj ,故都是winner
	Player* pPlayer = next_ref(NULL);
	while(pPlayer !=NULL) {
		pPlayer->finish_pvp2_battle(m_instance_id, PVP2_BTL_WIN);
		pPlayer = next_ref(pPlayer);
	}
#endif
}


void Instance::finish_pvp3_battle()
{
#ifndef FRONT
	// 执行至此的player都没有经过reduce_obj ,故都是winner
	Player* pPlayer = next_ref(NULL);
	while(pPlayer !=NULL) {
		pPlayer->finish_pvp3_battle(m_instance_id, true);
		pPlayer = next_ref(pPlayer);
	}
#endif
}


void Instance::finish_pvp4_battle()
{
#ifndef FRONT
	// 执行至此的player都没有经过reduce_obj ,故都是winner
	Player* pPlayer = next_ref(NULL);
	while(pPlayer !=NULL) {
		pPlayer->finish_pvp4_battle(m_instance_id, true);
		pPlayer = next_ref(pPlayer);
	}
#endif
}



int InstanceMgr::create_instance(Player* pPlayer, uint32_t instance_id) {
	DEBUG_LOG("p %u CREATE INSTANCE %d ",pPlayer->id(), instance_id);
#ifndef FRONT
	if(instance_id == GAMEBEGIN_BATTLE && pPlayer->get_once(FIRST_BATTLE)) {
		return -1;
	}
#endif
	Instance_t* instance = g_instance_config->get_instance_info(instance_id);
	if(instance) {
		if ((InstanceType)instance->type == PPVE2) {
#ifndef FRONT
			// 世界boss
			sWBInstanceMgr.create_instance(pPlayer, instance_id);
#endif
			return -1;
		}
		if((InstanceType)instance->type == PPVE3){
#ifndef FRONT
			uint32_t ret = pPlayer->check_ppve3_condition(instance_id);
			if(ret) {
				pPlayer->send_stderr(ret);
				return -1;
			}
			sBBInstanceMgr.create_instance(pPlayer, instance_id);
			return -1;
#endif
		}
		if (((InstanceType)instance->type == PVE || (InstanceType)instance->type == PVE2 || (InstanceType)instance->type == PVE3) 
			&& !pPlayer->unlock_instance(instance_id)) {
			ERROR_LOG("p %u  CAN'T CREATE INSTANCE %d ",pPlayer->id(), instance_id);
			return -1;
		}
#ifndef FRONT
		// pve 条件检查
		if((InstanceType)instance->type == PVE) {
			uint32_t ret = pPlayer->check_pve_condition(instance_id);
			if (ret) {
				pPlayer->send_stderr(ret);
				return -1;
			}
		}
		else if((InstanceType)instance->type == PVE2){
			// car 添加
			uint32_t ret = pPlayer->check_pve2_condition();
			if (ret) {
				pPlayer->send_stderr(ret);
				return -1;
			}

		}
#endif

		Instance* pinstance = new Instance(instance_id, (InstanceType)instance->type, instance->timeout, instance->star_time);
		add_manager(pinstance);
		pinstance->add_obj(pPlayer);
#ifndef FRONT
		//统计地下层副本
		if((InstanceType)instance->type == PVE3){
			STAT_MGR.fight_in_ug(pPlayer,instance_id);
		}
#endif
		return 0;
	} else {
		ERROR_LOG("p %u  CAN'T CREATE UNEXIST INSTANCE %d ",pPlayer->id(), instance_id);
		return -1;
	}
}
	
int InstanceMgr::load_complete(Player* pPlayer) {
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(pPlayer->get_id()));
	if(pinstance) {	
		pinstance->load_complete(pPlayer);
	} else {
		ERROR_LOG("p %u  CAN'T LOAD COMPLETE UNEXIST INSTANCE",pPlayer->id());
	}
	return 0;
}
	
int InstanceMgr::transfer_next(Player* pPlayer) {
	DEBUG_LOG("p %u TRANSFER NEXT INSTANCE",pPlayer->id());
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(pPlayer->get_id()));
	if(pinstance) {	
		pinstance->transfer_next(pPlayer);
	} else {
		ERROR_LOG("p %u  CAN'T TRANSFER UNEXIST INSTANCE",pPlayer->id());
	}
	return 0;
}
	
int InstanceMgr::join_instance(Player* pPlayer, Player* owner) {
	DEBUG_LOG("p %u JOIN INSTANCE OWNER: %u",pPlayer->id(), owner->id());
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(owner->get_id()));	
	if(pinstance) {
		pinstance->player_join(pPlayer);
	} else {
		ERROR_LOG("p %u JOIN INSTANCE OWNER: %u",pPlayer->id(), owner->id());
	}
	return 0;
}
	
int InstanceMgr::init_instance(Player* owner) {
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(owner->get_id()));	
	if(pinstance) {
		return pinstance->init();
	} else {
		ERROR_LOG("p %u  CAN'T INIT UNEXIST INSTANCE",owner->id());
	}
	return 0;
}
	
int InstanceMgr::leave_instance(Player* pPlayer) {
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(pPlayer->get_id()));	
	if(pinstance) {
		DEBUG_LOG("p %u leave with instance unfinish",pPlayer->get_id().id());
		pinstance->reduce_obj(pPlayer);
	} else {
		ERROR_LOG("p %u  CAN'T LEAVE UNEXIST INSTANCE",pPlayer->id());
	}
	return 0;
}
	
int InstanceMgr::player_hp_change(Player* pPlayer, int blood) {
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(pPlayer->get_id()));	
	if(pinstance) {
		pinstance->player_hp_change(pPlayer, blood);
	} else {
		ERROR_LOG("p %u  CAN'T HP CHANGE UNEXIST INSTANCE",pPlayer->id());
	}
	return 0;
}
	
bool InstanceMgr::in_pve(Player* pPlayer) {
	Instance* pinstance = static_cast<Instance*>(get_manager_by_key(pPlayer->get_id()));	
	if(pinstance) {
		return pinstance->is_pve();
	} else {
		return false;
	}
}
