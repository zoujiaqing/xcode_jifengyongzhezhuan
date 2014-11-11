#include "userskill.h"
#ifndef FRONT
#include "../msg_dispatcher/msg_dispatcher.h"
#else
#include "../frontend/virtualplayer.h"
#endif
#include "../config/battleconfig.h"

UserSkill::UserSkill(uint32_t skill_id, uint32_t skill_level) :
	skill_id(skill_id),
	skill_level(skill_level) {

}

UserSkill::UserSkill():
	skill_id(0),
	skill_level(0) {

}

UserSkillManager::UserSkillManager(Player* pPlayer) :
	m_player(pPlayer) {
}

void UserSkillManager::init_from_db(const db_skill_query_out* msg) {
	if(msg) {
		for(int i = 0; i < msg->skills_size(); i++) {
			m_skill_map[msg->skills(i).skill_id()] = UserSkill(msg->skills(i).skill_id(), msg->skills(i).skill_level());
		}
	}
}
	
void UserSkillManager::unpack_skills(CSGetUserSkillRsp* rsp) {
	if(rsp) {
		for(int i = 0; i < rsp->userskills_size(); i++) {
			m_skill_map[rsp->userskills(i).skill_id()] = UserSkill(rsp->userskills(i).skill_id(), rsp->userskills(i).skill_level());
			g_skill_hurt_config->unpack_hero_skill(rsp->userskills(i));
		}
	}
}
	
void UserSkillManager::pack_skills(CSGetUserSkillRsp* rsp) {
	add_skill_with_skills_is_null();
	rsp->set_userid(m_player->id());
	rsp->set_reg_time(m_player->regtime());
	rsp->set_channel_id(m_player->channelid());
	m_skill_map[4999999] = UserSkill(4999999, 1);
	SkillMap::iterator itr = m_skill_map.begin();
	for(; itr!=m_skill_map.end(); ++itr) {	
		uint32_t max_level= g_skill_hurt_config->get_hurt_max_level(itr->second.skill_id);
		if(max_level == 0)
		{
			ERROR_LOG("p %u has wrong skill id %u ",m_player->id(),itr->second.skill_id);
			continue;
		}
		if(max_level < itr->second.skill_level)
		{
			ERROR_LOG("p %u has  skill id %u with level %u exceed max_level %u",m_player->id(),itr->second.skill_id,itr->second.skill_level,max_level);
			itr->second.skill_level =  max_level;
		}
		SkillHurt_t* skill_hurt = g_skill_hurt_config->get_hurt_info(itr->second.skill_id, itr->second.skill_level);	
		if(!skill_hurt)
		{
			ERROR_LOG("p %u has wrong skill id %u skill level %u  which shoud be existed",m_player->id(),itr->second.skill_id,itr->second.skill_level);
			continue;
		}
		UserSkillInfo* skill_info = rsp->add_userskills();
		skill_info->set_skill_id(skill_hurt->skill_id);
		skill_info->set_level(skill_hurt->level);
		//skill_info->set_part(skill_hurt->part);
		skill_info->set_part(0);
		skill_info->set_skill_level(skill_hurt->skill_level);
		skill_info->set_money(skill_hurt->money);
		skill_info->set_exploit(skill_hurt->exploit);
		skill_info->set_own_cd(skill_hurt->own_cd);
		skill_info->set_share_cd(skill_hurt->share_cd);
		skill_info->set_max_level(max_level);

		std::vector<SkillPart_t>::iterator part_itr = skill_hurt->parts.begin();
		for(;part_itr!=skill_hurt->parts.end(); ++ part_itr) {
			UserSkillPart* part_info = skill_info->add_parts();
			part_info->set_part(part_itr->part);
			part_info->set_continue_time(part_itr->continue_time);
			part_info->set_continue_time2(part_itr->continue_time2);
			part_info->set_can_attack_number(part_itr->can_attack_number);
			part_info->set_hurt(part_itr->hurt);
			BasicUserBattleInfo* base_info = part_info->mutable_exp_battle_info();
			base_info->set_physical_attack(part_itr->physical_attack);
			base_info->set_magic_attack(part_itr->magic_attack);
			base_info->set_skill_attack(part_itr->skill_attack);
			base_info->set_physical_defence(part_itr->physical_defence);
			base_info->set_magic_defence(part_itr->magic_defence);
			base_info->set_skill_defence(part_itr->skill_defence);
			base_info->set_health_point(part_itr->health_point);
			base_info->set_accurate(part_itr->accurate);
			base_info->set_dodge(part_itr->dodge);
			base_info->set_wreck(part_itr->wreck);
			base_info->set_parry(part_itr->parry);
			base_info->set_critical_strike(part_itr->critical_strike);
			base_info->set_tenacity(part_itr->tenacity);
			base_info->set_slay(part_itr->slay);
			base_info->set_speed(part_itr->speed);
			base_info->set_courage(0);
			base_info->set_charm(0);
			base_info->set_trick(0);
			base_info->set_proficiency(0);
		}
	}	
}
	
void UserSkillManager::pack_skills(InterUserSkillInfo* req) {
	SkillMap::iterator itr =  m_skill_map.begin();
	for(; itr!=m_skill_map.end(); ++itr) {
		InterUserSkillInfo_UserSkill * info = req->add_skills();
		info->set_id(itr->second.skill_id);
		info->set_level(itr->second.skill_level);
	}
}

void UserSkillManager::unpack_skills(const InterUserSkillInfo* rsp) {
	m_skill_map.clear();
	int size = rsp->skills_size();
	for(int i=0; i< size ; i++) {
		m_skill_map[rsp->skills(i).id()] = UserSkill(rsp->skills(i).id(), rsp->skills(i).level());
	}
}
	
void UserSkillManager::add_skill(uint32_t id, uint32_t level) {
#ifndef FRONT
	SkillHurt_t* skill_hurt = g_skill_hurt_config->get_hurt_info(id, level);
	if(skill_hurt) {
		SkillMap::iterator itr = m_skill_map.find(id);
		if(itr !=m_skill_map.end()) {
			if(itr->second.skill_level != level -1){
				ERROR_LOG("p %u has wrong skill id %u level which shoud be %u",m_player->id(),itr->second.skill_level,level-1);
				return;
			}
		} else {
			if(level>1)
			{
				ERROR_LOG("p %u has wrong skill level %u which should be 0 ",m_player->id(),id);
				return;
			}
		}
		if(!m_player->check_item_num(ATTR_PLAYER_COIN, skill_hurt->money)
			|| !m_player->check_item_num(ATTR_PLAYER_EXPLOIT, skill_hurt->exploit)
			|| !m_player->check_level(skill_hurt->level) 
		) {
			ERROR_LOG("p %u do not meet the condition of the skill %u",m_player->id(),itr->second.skill_id);
			return;
		}
		m_player->del_item(ATTR_PLAYER_COIN, skill_hurt->money);
		DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u", ST_CONSUME_COIN, m_player->zone_id(), base::Time().seconds(), m_player->name().c_str(), m_player->id(), m_player->regtime(), m_player->channelid(), m_player->type(), skill_hurt->money, SC_CONSUME_FOR_SKILL, m_player->lv());
		m_player->del_item(ATTR_PLAYER_EXPLOIT, skill_hurt->exploit);
		DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u", ST_CONSUME_EXPLOIT, m_player->zone_id(), base::Time().seconds(), m_player->name().c_str(), m_player->id(), m_player->regtime(), m_player->channelid(), m_player->type(), skill_hurt->exploit, SC_CONSUME_FOR_SKILL, m_player->lv());
		m_skill_map[id] = UserSkill(id, level);
		DEBUG_LOG(" [Statistics] %u %u %u %s %u %u %u %u %u %u %u", ST_SKILL_LEVELUP, m_player->zone_id(), base::Time().seconds(), m_player->name().c_str(), m_player->id(), m_player->regtime(), m_player->channelid(), m_player->type(), id, level, m_player->lv());
		db_skill_add_in req;
		req.set_skill_id(id);
		req.set_skill_level(level);
		sMsgDispatcher.send_db_msg(m_player, req);
		m_player->do_once_daily_task(DAILY_TASK_TYPE_SKILL_LVUP);
		m_player->do_once_daily_goals(ATTR_DAILY_TIMES_SKILL_LVUP);
	}
	else{
		ERROR_LOG("p %u has wrong skill id %u skill level %u ",m_player->id(),id,level);
	}
#endif
}
	
uint32_t UserSkillManager::get_skill_level(uint32_t id) {
	SkillMap::iterator itr = m_skill_map.find(id);
	if(itr !=m_skill_map.end()) {
		return itr->second.skill_level ;
	}
	else if(id==4999999) {
		return 1;
	}else{
		ERROR_LOG("p %u has wrong skill id %u ",m_player->id(),id);
	}
	return 0;
}

std::map<uint32_t, UserSkill> UserSkillManager::get_skill_map() {
	return m_skill_map;
}

void UserSkillManager::add_skill_with_skills_is_null()
{
#ifndef FRONT
	// hard code
	if (unlikely(!m_skill_map.size())) {
		switch(m_player->type()) {
			case FIGHTER_GRIL:
			case FIGHTER_BOY:
				{
					add_skill(4011101, 1);
					add_skill(4011201, 1);
					add_skill(4011301, 1);
					add_skill(4012101, 1);
					add_skill(4013101, 1);
					add_skill(4014101, 1);


					m_player->set_property(ATTR_SET_BEGIN, 4012101 );
					m_player->set_property(ATTR_SET_BEGIN + 1, 4013101 );
					m_player->set_property(ATTR_SET_BEGIN + 2, 4014101 );
				}   
				break;  
			case MAGICIAN_GRIL:
			case MAGICIAN_BOY:
				{   
					add_skill(4041101, 1);
					add_skill(4041201, 1);
					add_skill(4041301, 1);

					add_skill(4042101, 1);
					add_skill(4043101, 1);
					add_skill(4044101, 1);
					m_player->set_property(ATTR_SET_BEGIN, 4042101);
					m_player->set_property(ATTR_SET_BEGIN+1, 4043101);
					m_player->set_property(ATTR_SET_BEGIN+2, 4044101);
				}
				break;
			case BRAVO_GRIL : // 刺客
			case BRAVO_BOY  : // 刺客
				{
					add_skill(4051101, 1);
					add_skill(4051102, 1);
					add_skill(4051103, 1);

					add_skill(4052101, 1);
					add_skill(4053101, 1);
					add_skill(4054101, 1);
					m_player->set_property(ATTR_SET_BEGIN,4052101);
					m_player->set_property(ATTR_SET_BEGIN+1,4053101);
					m_player->set_property(ATTR_SET_BEGIN+2,4054101);
				}
				break;
		} 	
	}
#endif
}
	
bool UserSkillManager::update_valid() {
#ifndef FRONT
	HeroSkillList skill_list =  g_hero_skill_config->get_hero_skill_list(m_player->type());
	HeroSkillList::iterator itr = skill_list.begin();
	for(; itr!= skill_list.end(); ++itr) {
		uint32_t skill_id = itr->skill_id;
		uint32_t skill_level = get_skill_level(skill_id);
		SkillHurt_t* skill_hurt = g_skill_hurt_config->get_hurt_info(skill_id, skill_level+1);	
		if(skill_hurt) {
			if(m_player->check_level(skill_hurt->level, false) ) {
				return true;
			}
		}
	}
#endif
	return false;	
}

#ifndef FRONT
	
void UserSkillManager::game_first_start() {
	switch(m_player->type()) {
		case FIGHTER_GRIL:
		case FIGHTER_BOY:
		{
			m_skill_map[4013101] = UserSkill(4013101, 80);
			m_skill_map[4014101] = UserSkill(4014101, 80);
			m_skill_map[4012101] = UserSkill(4012101, 80);
			m_skill_map[4017101] = UserSkill(4017101, 80);
			m_skill_map[4016101] = UserSkill(4016101, 1);
		}   
		break;  
		case MAGICIAN_GRIL:
		case MAGICIAN_BOY:
		{   
			m_skill_map[4042101] = UserSkill(4042101, 80);
			m_skill_map[4043101] = UserSkill(4043101, 80);
			m_skill_map[4044101] = UserSkill(4044101, 80);
			m_skill_map[4047101] = UserSkill(4047101, 80);
			m_skill_map[4046101] = UserSkill(4046101, 1);
		}
		break;
		case BRAVO_GRIL : // 刺客
		case BRAVO_BOY  : // 刺客
		{
			m_skill_map[4053101] = UserSkill(4053101, 80);
			m_skill_map[4052101] = UserSkill(4052101, 80);
			m_skill_map[4054101] = UserSkill(4054101, 80);
			m_skill_map[4057101] = UserSkill(4057101, 80);
			m_skill_map[4056101] = UserSkill(4056101, 1);
		}
		break;
	} 	
}

void UserSkillManager::after_game_first_start() {
	switch(m_player->type()) {
		case FIGHTER_GRIL:
		case FIGHTER_BOY:
		{
			m_skill_map[4013101] = UserSkill(4013101, 1);
			m_skill_map.erase(4014101);
			m_skill_map.erase(4012101);
			m_skill_map.erase(4017101);
			m_skill_map.erase(4016101);
		}   
		break;  
		case MAGICIAN_GRIL:
		case MAGICIAN_BOY:
		{   
			m_skill_map[4042101] = UserSkill(4042101, 1);
			m_skill_map.erase(4043101);
			m_skill_map.erase(4044101);
			m_skill_map.erase(4047101);
			m_skill_map.erase(4046101);
		}
		break;
		case BRAVO_GRIL : // 刺客
		case BRAVO_BOY  : // 刺客
		{
			m_skill_map[4053101] = UserSkill(4053101, 1);
			m_skill_map.erase(4052101);
			m_skill_map.erase(4054101);
			m_skill_map.erase(4057101);
			m_skill_map.erase(4056101);
		}
		break;
	} 
}
#endif
