// project-specific headers go here
#include "object.h"
#include "../common/common.h"
#include <math.h>
#include "../base/ProbabilityGen.hpp"
#include "buffeffect.h"

using namespace google::protobuf;
	
Entity::Entity(player_key_t id, uint8_t groupid) :
	base::Object<player_key_t> (id),
	m_groupid(groupid),
	m_pos(0,0),
	m_direct(0 -1),
	m_buffmanager(new BuffEffectManager(this))
{
}
	
Entity::Entity(const Entity& pentity) {
	m_groupid = pentity.m_groupid;
   	m_fightprop = pentity.m_fightprop;
	m_prop = pentity.m_prop;
    m_pos = pentity.m_pos;
    m_direct = pentity.m_direct;
	m_buffmanager = new BuffEffectManager(*pentity.m_buffmanager);
}

const Entity& Entity::operator=(const Entity& pentity) {
	m_groupid = pentity.m_groupid;
   	m_fightprop = pentity.m_fightprop;
	m_prop = pentity.m_prop;
    m_pos = pentity.m_pos;
    m_direct = pentity.m_direct;
	m_buffmanager = new BuffEffectManager(*pentity.m_buffmanager);
	return *this;
}
	
Entity::~Entity() {
	if(!m_del) {
		sig_obj_del(this);
		m_del = true;
	}
	delete m_buffmanager;
}
	
TileWorld<>::Rectangle Entity::space() {
	return m_prop.space+m_pos;
}
	
void Entity::count_under_attack(uint32_t attack_type,uint32_t damage, FightProp& prop, AttackValueType& type, uint32_t& hurt, uint32_t attack_num) {
	static const uint32_t ALL_CONSTANT = 10000;
	static const uint32_t DODGE_CONSTANT = 0;
	static const uint32_t PARRY_REDUCE_CONSTANT = 5000;
	static const uint32_t CRIT_MULT_CONSTANT = 15000;
	static const uint32_t MIN_HURT_CONSTANT = 5;
	if(m_fightprop.invincible) {
		type = ATTACK_NORMAL;
		hurt =  0; 
		return ;
	}
	base::ProbabilityGen<AttackValueType> gen;
	uint32_t probability_max = 8000;
	uint32_t current_dodge = DODGE_CONSTANT + m_fightprop.dodge * 10 > prop.accurate *10 ? 
		DODGE_CONSTANT + m_fightprop.dodge * 10 - prop.accurate *10:
		0 ;
	current_dodge = current_dodge > probability_max? probability_max : current_dodge;
	probability_max -= current_dodge;
	gen.push(current_dodge, ATTACK_DODGE);
	uint32_t current_parry = m_fightprop.parry * 10 > prop.wreck * 10 ?
		m_fightprop.parry * 10 - prop.wreck * 10 :
		0;
	current_parry = current_parry > probability_max? probability_max : current_parry;
	probability_max -= current_parry;
	gen.push(current_parry, ATTACK_PARRY);
	uint32_t  current_crit = prop.critical_strike * 10 > m_fightprop.tenacity * 10 ?
		prop.critical_strike * 10 - m_fightprop.tenacity * 10 :
		0;
	current_crit = current_crit > probability_max? probability_max : current_crit;
	gen.push(current_crit, ATTACK_CRIT);
	gen.push(ALL_CONSTANT - current_dodge - current_parry - current_crit, ATTACK_NORMAL);
	
	type = gen.get_one();

	switch (attack_type) {
		case (ATTACK_PHYSICAL):
		{
			uint32_t hurt1 = (prop.physical_attack  > m_fightprop.physical_defence? (prop.physical_attack  - m_fightprop.physical_defence)/attack_num * damage /100:0) * base::ranged_random(95, 105) / 100;
			uint32_t hurt2 = prop.physical_attack * MIN_HURT_CONSTANT / 1000;
			hurt = hurt1 > hurt2? hurt1: hurt2;
		}
		break;
		case (ATTACK_MAGIC):
		{
			uint32_t hurt1 = (prop.magic_attack > m_fightprop.magic_defence? (prop.magic_attack - m_fightprop.magic_defence)/attack_num * damage /100:0) * base::ranged_random(95, 105) / 100;
			uint32_t hurt2 = prop.magic_attack * MIN_HURT_CONSTANT / 1000;
			hurt = hurt1 > hurt2? hurt1: hurt2;
		}
		break;
		case (ATTACK_SKILL):
		{
			uint32_t hurt1 = (prop.skill_attack  > m_fightprop.skill_defence? (prop.skill_attack - m_fightprop.skill_defence)/attack_num* damage /100:0) * base::ranged_random(95, 105) / 100;
			uint32_t hurt2 = prop.skill_attack * MIN_HURT_CONSTANT / 1000;
			hurt = hurt1 > hurt2? hurt1: hurt2;
		}
		break;
	}
	hurt = hurt > 1? hurt: 1;

	if(prop.world_boss) {
		hurt = (damage - m_fightprop.skill_defence/10000) * (m_fightprop.health_point / 100) / attack_num ; 
	}

	switch (type) {
		case (ATTACK_DODGE):
		{
			hurt = 0;
		}
		break;
		case (ATTACK_PARRY):
		{
			hurt = hurt * (ALL_CONSTANT - PARRY_REDUCE_CONSTANT) / ALL_CONSTANT ;
		}
		break;
		case (ATTACK_CRIT):
		{
			hurt = hurt * (CRIT_MULT_CONSTANT + prop.slay * 10 ) / ALL_CONSTANT; 
		}
		break;
		case (ATTACK_NORMAL):
		{
			hurt = hurt;
		}
	}
	uint32_t pre_health_point = m_prop.health_point;
	m_prop.health_point = m_prop.health_point > hurt ? m_prop.health_point - hurt : 0 ;
	sig_health_point(this, m_prop.health_point, NULL);
	if(!m_prop.health_point && pre_health_point) {
		sig_dead(this);
	}
}

uint32_t Entity::check_attack_hurt(uint32_t attack_type, uint32_t coefficient, FightProp& def_prop, uint32_t cli_hurt, uint32_t attack_num, uint32_t fixed_hurt) {
	static const uint32_t ALL_CONSTANT = 10000;
	static const uint32_t CRIT_MULT_CONSTANT = 15000;
	static const uint32_t MIN_HURT_CONSTANT = 5;
	FightProp &prop = m_fightprop;
	uint32_t max_hurt = 0;
	if(def_prop.invincible) {
		return max_hurt;
	}

	switch (attack_type) {
		case (ATTACK_PHYSICAL):
		{
			uint32_t hurt1 = prop.physical_attack > def_prop.physical_defence ? (prop.physical_attack - def_prop.physical_defence) / attack_num * coefficient/100 * 105/100 : 0;
			uint32_t hurt2 = prop.physical_attack * MIN_HURT_CONSTANT / 1000;
			max_hurt = hurt1 > hurt2 ? hurt1: hurt2;
		}
		break;
		case (ATTACK_MAGIC):
		{
			uint32_t hurt1 = prop.magic_attack > def_prop.magic_defence ? (prop.magic_attack - def_prop.magic_defence) / attack_num * coefficient/100 * 105/100 : 0;
			uint32_t hurt2 = prop.magic_attack * MIN_HURT_CONSTANT / 1000;
			max_hurt = hurt1 > hurt2 ? hurt1: hurt2;
		}
		break;
		case (ATTACK_SKILL):
		{
			uint32_t hurt1 = prop.skill_attack + fixed_hurt > def_prop.skill_defence ? (prop.skill_attack  + fixed_hurt - def_prop.skill_defence) *coefficient/100/attack_num * 105/100 : 0; 
			uint32_t hurt2 = prop.skill_attack * MIN_HURT_CONSTANT / 1000;
			max_hurt = hurt1 > hurt2? hurt1: hurt2;
		}
		break;
	}
	max_hurt = max_hurt > 1 ? max_hurt: 1;
	max_hurt = max_hurt * (CRIT_MULT_CONSTANT + prop.slay * 10 ) / ALL_CONSTANT; 
	if (cli_hurt > max_hurt) {
		ERROR_LOG("check_attack_hurt: max_hurt:%u cli_hurt:%u p[%u %u %u]", 
				max_hurt, cli_hurt, get_id().id(), get_id().regtime(), get_id().channelid());
#if 0 // TEST DEL ME 	// 目前只打日志
		return cli_hurt;	
#endif
		return max_hurt;
	} else {
		return cli_hurt;
	}
}

void Entity::under_attack(Entity* pEntity, uint32_t hurt)
{
	uint32_t pre_health_point = m_prop.health_point;
	m_prop.health_point = m_prop.health_point > hurt ? m_prop.health_point - hurt : 0 ;
	sig_health_point(this, m_prop.health_point, pEntity);
	if(!m_prop.health_point && pre_health_point) {
		sig_dead(this);
	}
};
	
bool Entity::attach(const TileWorld<>::Point& point, uint8_t rule ) {
	if( (m_groupid  | rule) != 0) {
		return false;
	} else {
		TileWorld<>::Rectangle rec = space() ;
		return rec.point_in_range(point);		
	}
}
	
bool Entity::attach(const TileWorld<>::Rectangle& rec, uint8_t rule ) {
	if( (m_groupid  | rule) != 0) {
		return false;
	} else {
		TileWorld<>::Rectangle pos_rec = space();
		return pos_rec.touch(rec);		
	}
}
	
void Entity::set_group(uint8_t groupid) {
	m_groupid = groupid;
}

uint8_t Entity::get_group() const {
	return m_groupid;
}

void Entity::pack_basic_battle_info(BasicUserBattleInfo* battle_info) const
{
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
	battle_info->set_courage(m_fightprop.courage);
	battle_info->set_charm(m_fightprop.charm); 
	battle_info->set_trick(m_fightprop.trick);
	battle_info->set_speed(m_fightprop.speed); 
	battle_info->set_proficiency(m_fightprop.proficiency); 
	battle_info->set_current_hp(m_prop.health_point);
}
	
bool Entity::dead() {
	return m_prop.health_point == 0;
}

