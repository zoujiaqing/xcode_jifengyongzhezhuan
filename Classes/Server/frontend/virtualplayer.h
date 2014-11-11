#ifndef VIRTUALPLAYER_H_
#define VIRTUALPLAYER_H_

#include "explorer.h"
#include "../base/AttributeList.hpp"
#include "../entities/object.h"
#include "../proto/protocodec.h"
#include "../proto/cs_core.pb.h"
#include "../entities/buffeffect.h"
#include "../entities/userskill.h"

class Player : public Entity {
public:
	Player(BasicUserInfo& info, CSGetUserSkillRsp& skill_msg,  Explorer* explorer);

	void set_info(BasicUserInfo& info);

	void set_skill(CSGetUserSkillRsp& skill_msg);

    void pack_basic_info(BasicUserInfo* info);
    
	void pack_basic_battle_info(BasicUserInfo* info);
    
	void pack_basic_battle_info(BasicUserBattleInfo* info);

//	bool enter_map(Map::IdType mid, uint32_t x, uint32_t y, uint32_t orient);
    
	void leave_map();

	void finish_instance(uint32_t instance_id, uint32_t star, CSBattleLeaveRsp& leave_rsp);
	
	int send_msg(const google::protobuf::Message& msg);

	int send_stderr(uint32_t errcode);

	uint32_t id() {return m_info.uid();};

	uint32_t regtime() {return m_info.reg_time();};

	uint32_t channelid() {return m_info.channel_id();};

	bool is_ai(){return false;};


	void add_buff(uint32_t id);

	void add_buff(uint32_t id,uint32_t value);

	void player_walk_process(){};

	void do_once_daily_goals(uint32_t){};

    void sig_pvp2_fail(){};

	bool unlock_instance(uint32_t instance_id){return true;};

	uint32_t check_pve_condition(){return 0;};

	void refresh();

	uint32_t check_attack_hurt(uint32_t skill_id, FightProp& def_prop, uint32_t cli_hurt) { return cli_hurt; }
public:
	base::AttributeList 			m_attribute_list;

private:
	BasicUserInfo m_info;
	Explorer* m_explorer;

private:
	UserSkillManager 				m_skills;
};
	
inline Player::Player(BasicUserInfo& info, CSGetUserSkillRsp& skill_msg, Explorer* explorer):
	Entity(player_key_t(info.uid(), info.reg_time(), info.channel_id()), PLAYER_DEFAULT_GROUP),
	m_info(info) ,
	m_explorer(explorer),
	m_skills(this)
{
	set_info(m_info);
	m_skills.unpack_skills(&skill_msg);
}	
	
inline void Player::set_skill(CSGetUserSkillRsp& skill_msg)
{
	m_skills.unpack_skills(&skill_msg);
}
	
inline void Player::set_info(BasicUserInfo& info)
{
	m_info = info;
	if(m_info.has_battle_info()) {
		BasicUserBattleInfo battle_info = m_info.battle_info();
		m_fightprop.physical_attack = battle_info.physical_attack();
		m_fightprop.magic_attack = battle_info.magic_attack();
		m_fightprop.skill_attack = battle_info.skill_attack();
		m_fightprop.physical_defence = battle_info.physical_defence();
		m_fightprop.magic_defence = battle_info.magic_defence();
		m_fightprop.skill_defence = battle_info.skill_defence();
		m_fightprop.health_point = battle_info.health_point();
		m_fightprop.accurate = battle_info.accurate();
		m_fightprop.dodge = battle_info.dodge();
		m_fightprop.wreck = battle_info.wreck();
		m_fightprop.parry = battle_info.parry();
		m_fightprop.critical_strike = battle_info.critical_strike();
		m_fightprop.tenacity = battle_info.tenacity();
		m_fightprop.slay = battle_info.slay();
		m_fightprop.speed = battle_info.speed();
		m_fightprop.courage = battle_info.courage();
		m_fightprop.charm = battle_info.charm();
		m_fightprop.trick = battle_info.trick();
		m_fightprop.proficiency = battle_info.proficiency();
		m_fightprop.invincible 	= false;
		m_fightprop.stealth		= false;
		m_buffmanager->clear();
	}
	m_prop.health_point = m_fightprop.health_point;
	ObjectRange_t* range  = g_range_config->get_role_range_info(m_info.type());
	if(range) {
		m_prop.space = range->body_range;
	}
};
	
inline void Player::refresh()
{
	if(m_info.has_battle_info()) {
		BasicUserBattleInfo* battle_info = m_info.mutable_battle_info();
		battle_info->set_physical_attack(m_fightprop.physical_attack);
		battle_info->set_magic_attack(m_fightprop.magic_attack);
		battle_info->set_skill_attack(m_fightprop.skill_attack);
		battle_info->set_physical_defence(m_fightprop.physical_defence);
		battle_info->set_magic_defence(m_fightprop.magic_defence);
		battle_info->set_skill_defence(m_fightprop.skill_defence);
		battle_info->set_health_point(m_fightprop.health_point);
		battle_info->set_accurate(m_fightprop.accurate);
		battle_info->set_dodge(m_fightprop.dodge);
		battle_info->set_wreck(m_fightprop.wreck);
		battle_info->set_parry(m_fightprop.parry);
		battle_info->set_critical_strike(m_fightprop.critical_strike);
		battle_info->set_tenacity(m_fightprop.tenacity);
		battle_info->set_slay(m_fightprop.slay);
		battle_info->set_speed(m_fightprop.speed);
		battle_info->set_courage(m_fightprop.courage);
		battle_info->set_charm(m_fightprop.charm);
		battle_info->set_trick(m_fightprop.trick);
		battle_info->set_proficiency(m_fightprop.proficiency);
		battle_info->set_current_hp(m_prop.health_point);
	}
}

inline void Player::pack_basic_info(BasicUserInfo* info)
{
	refresh(); 
	m_info.set_xpos(m_pos.x);
    m_info.set_ypos(m_pos.y);
	*info = m_info;
	return;
}

inline void Player::pack_basic_battle_info(BasicUserInfo* info) {
	refresh();
	*info = m_info;
	return;
} 
	
inline void Player::pack_basic_battle_info(BasicUserBattleInfo* info) {
	refresh();
	*info = m_info.battle_info();
	return;
}

/*	
inline bool Player::enter_map(Map::IdType mid, uint32_t x, uint32_t y, uint32_t orient)
{
	return true;
}*/

inline void Player::leave_map()
{

}
	
inline void Player::finish_instance(uint32_t instance_id, uint32_t star, CSBattleLeaveRsp& leave_rsp)
{

}

inline int Player::send_msg(const google::protobuf::Message& msg)
{
    const FullPacket* fp = encode(msg);
    if (fp) {
		m_explorer->virtual_hdlr(fp->packet, fp->len);
		return 0;
    }
    return -1;
}

inline int Player::send_stderr(uint32_t errcode)
{
	return 0;
}

inline void Player::add_buff(uint32_t id){
	uint32_t level = m_skills.get_skill_level(id);
	if(level){
		m_buffmanager->add_hero_buff(id, level);
	}
}

inline void Player::add_buff(uint32_t id,uint32_t value)
{
	uint32_t level = m_skills.get_skill_level(id);
	if(!level) {
		m_buffmanager->add_fairy_skill_buff(id,value);
	}
}

#endif
