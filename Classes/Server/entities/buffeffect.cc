#include "buffeffect.h"
#include "../base/Time.hpp"
#include "../common/fwd_decl.h"
	
BuffEffect::BuffEffect(uint32_t deadtime, uint32_t id):
	m_id(id)
{
	sGloble.Timer.add_event(*this, &BuffEffect::del_self, base::Time()+deadtime);
}
	
int BuffEffect::del_self() {
	sig_del_self(this);
	delete this;
	return 0;
}
	
uint32_t BuffEffect::get_id() {
	return m_id;
}
	
InvincibleBuff::InvincibleBuff(uint32_t dead_time, uint32_t id):
	BuffEffect(dead_time, id) {
}
	
void InvincibleBuff::do_buff(Entity* entity, const FightProp& m_base_fight_prop) {
	entity->m_fightprop.invincible = true;
}
	
void InvincibleBuff::undo_buff(Entity* entity, const FightProp& m_base_fight_prop) {
	entity->m_fightprop.invincible = false;
}
	
StealthBuff::StealthBuff(uint32_t dead_time, uint32_t id) :
	BuffEffect(dead_time, id) {

}
void StealthBuff::do_buff(Entity* entity, const FightProp& m_base_fight_prop) {
	entity->m_fightprop.stealth = true;
}
	
void StealthBuff::undo_buff(Entity* entity, const FightProp& m_base_fight_prop) {
	entity->m_fightprop.stealth = false;
}
	
SpeedBuff::SpeedBuff(uint32_t dead_time, uint32_t speed, uint32_t id):
	BuffEffect(dead_time, id),
	m_speed(speed)
{
}

void SpeedBuff::do_buff(Entity* entity, const FightProp& m_base_fight_prop) {
	uint32_t save_speed = entity->m_prop.speed;
	entity->m_prop.speed = m_speed;
	m_speed = save_speed;
}
	
void SpeedBuff::undo_buff(Entity* entity, const FightProp& m_base_fight_prop) {
	uint32_t save_speed = entity->m_prop.speed;
	entity->m_prop.speed = m_speed;
	m_speed = save_speed;
}
	
AttrBuff::AttrBuff(uint32_t dead_time, SkillPart_t& skill_part, uint32_t id):
	BuffEffect(dead_time, id),
	m_skill_part(skill_part)
{
	
}
	
void AttrBuff::do_buff(Entity* entity, const FightProp& m_base_fight_prop)
{
	entity->m_fightprop.physical_attack += m_skill_part.physical_attack;
	entity->m_fightprop.magic_attack += m_skill_part.magic_attack;
	entity->m_fightprop.skill_attack += m_skill_part.skill_attack;
	entity->m_fightprop.physical_defence += m_skill_part.physical_defence;
	entity->m_fightprop.magic_defence += m_skill_part.magic_defence;
	entity->m_fightprop.skill_defence += m_skill_part.skill_defence;
	entity->m_fightprop.accurate += m_skill_part.accurate;
	entity->m_fightprop.dodge += m_skill_part.dodge;
	entity->m_fightprop.wreck += m_skill_part.wreck;
	entity->m_fightprop.parry += m_skill_part.parry;
	entity->m_fightprop.critical_strike += m_skill_part.critical_strike;
	entity->m_fightprop.tenacity += m_skill_part.tenacity;
	entity->m_fightprop.slay += m_skill_part.slay;
}


void AttrBuff::undo_buff(Entity* entity, const FightProp& m_base_fight_prop)
{
	entity->m_fightprop.physical_attack -= m_skill_part.physical_attack;
	entity->m_fightprop.magic_attack -= m_skill_part.magic_attack;
	entity->m_fightprop.skill_attack -= m_skill_part.skill_attack;
	entity->m_fightprop.physical_defence -= m_skill_part.physical_defence;
	entity->m_fightprop.magic_defence -= m_skill_part.magic_defence;
	entity->m_fightprop.skill_defence -= m_skill_part.skill_defence;
	entity->m_fightprop.accurate -= m_skill_part.accurate;
	entity->m_fightprop.dodge -= m_skill_part.dodge;
	entity->m_fightprop.wreck -= m_skill_part.wreck;
	entity->m_fightprop.parry -= m_skill_part.parry;
	entity->m_fightprop.critical_strike -= m_skill_part.critical_strike;
	entity->m_fightprop.tenacity -= m_skill_part.tenacity;
	entity->m_fightprop.slay -= m_skill_part.slay;
}


#define PERCENT (100)
AttrPerBuff::AttrPerBuff(uint32_t dead_time, uint32_t id,int32_t percent):
	BuffEffect(dead_time, id),
	m_percent(percent)
{
	
}
void AttrPerBuff::do_buff(Entity* entity, const FightProp& m_base_fight_prop)
{
	entity->m_fightprop.physical_attack += (int32_t)(entity->m_fightprop.physical_attack)/PERCENT*m_percent;
	entity->m_fightprop.magic_attack += (int32_t)(entity->m_fightprop.magic_attack)/PERCENT*m_percent;
	entity->m_fightprop.skill_attack += (int32_t)(entity->m_fightprop.skill_attack)/PERCENT*m_percent;
	entity->m_fightprop.physical_defence += (int32_t)(entity->m_fightprop.physical_defence)/PERCENT*m_percent;
	entity->m_fightprop.magic_defence += (int32_t)(entity->m_fightprop.magic_defence/PERCENT)*m_percent;
	entity->m_fightprop.skill_defence += (int32_t)(entity->m_fightprop.skill_defence/PERCENT)*m_percent;
	entity->m_fightprop.accurate += (int32_t)(entity->m_fightprop.accurate/PERCENT)*m_percent;
	entity->m_fightprop.dodge += (int32_t)(entity->m_fightprop.dodge)/PERCENT*m_percent;
	entity->m_fightprop.wreck += (int32_t)(entity->m_fightprop.wreck)/PERCENT*m_percent;
	entity->m_fightprop.parry += (int32_t)(entity->m_fightprop.parry)/PERCENT*m_percent;
	entity->m_fightprop.critical_strike += (int32_t)(entity->m_fightprop.critical_strike)/PERCENT*m_percent;
	entity->m_fightprop.tenacity += (int32_t)(entity->m_fightprop.tenacity)/PERCENT*m_percent;
	entity->m_fightprop.slay += (int32_t)(entity->m_fightprop.slay)/PERCENT*m_percent;

}
	
void AttrPerBuff::undo_buff(Entity* entity, const FightProp& m_base_fight_prop)
{
	entity->m_fightprop.physical_attack -= (int32_t)(entity->m_fightprop.physical_attack)/PERCENT*m_percent;
	entity->m_fightprop.magic_attack -= (int32_t)(entity->m_fightprop.magic_attack)/PERCENT*m_percent;
	entity->m_fightprop.skill_attack -= (int32_t)(entity->m_fightprop.skill_attack)/PERCENT*m_percent;
	entity->m_fightprop.physical_defence -= (int32_t)(entity->m_fightprop.physical_defence)/PERCENT*m_percent;
	entity->m_fightprop.magic_defence -= (int32_t)(entity->m_fightprop.magic_defence/PERCENT)*m_percent;
	entity->m_fightprop.skill_defence -= (int32_t)(entity->m_fightprop.skill_defence/PERCENT)*m_percent;
	entity->m_fightprop.accurate -= (int32_t)(entity->m_fightprop.accurate/PERCENT)*m_percent;
	entity->m_fightprop.dodge -= (int32_t)(entity->m_fightprop.dodge)/PERCENT*m_percent;
	entity->m_fightprop.wreck -= (int32_t)(entity->m_fightprop.wreck)/PERCENT*m_percent;
	entity->m_fightprop.parry -= (int32_t)(entity->m_fightprop.parry)/PERCENT*m_percent;
	entity->m_fightprop.critical_strike -= (int32_t)(entity->m_fightprop.critical_strike)/PERCENT*m_percent;
	entity->m_fightprop.tenacity -= (int32_t)(entity->m_fightprop.tenacity)/PERCENT*m_percent;
	entity->m_fightprop.slay -= (int32_t)(entity->m_fightprop.slay)/PERCENT*m_percent;


}
	
SkillBuff::SkillBuff(uint32_t dead_time, const FightProp& prop, uint32_t id) :
	BuffEffect(dead_time, id),
	m_prop(prop) {

}

void SkillBuff::do_buff(Entity* entity, const FightProp& m_base_fight_prop)
{
	entity->m_fightprop.physical_attack += m_prop.physical_attack;
	entity->m_fightprop.magic_attack += m_prop.magic_attack;
	entity->m_fightprop.skill_attack += m_prop.skill_attack;
	entity->m_fightprop.physical_defence += m_prop.physical_defence;
	entity->m_fightprop.magic_defence += m_prop.magic_defence;
	entity->m_fightprop.skill_defence += m_prop.skill_defence;
	entity->m_fightprop.accurate += m_prop.accurate;
	entity->m_fightprop.dodge += m_prop.dodge;
	entity->m_fightprop.wreck += m_prop.wreck;
	entity->m_fightprop.parry += m_prop.parry;
	entity->m_fightprop.critical_strike += m_prop.critical_strike;
	entity->m_fightprop.tenacity += m_prop.tenacity;
	entity->m_fightprop.slay += m_prop.slay;
	entity->m_prop.health_point += m_prop.health_point;
	if(entity->m_prop.health_point > entity->m_fightprop.health_point) {
		entity->m_prop.health_point = entity->m_fightprop.health_point;
	}
}
	
void SkillBuff::undo_buff(Entity* entity, const FightProp& m_base_fight_prop)
{
	entity->m_fightprop.physical_attack -= m_prop.physical_attack;
	entity->m_fightprop.magic_attack -= m_prop.magic_attack;
	entity->m_fightprop.skill_attack -= m_prop.skill_attack;
	entity->m_fightprop.physical_defence -= m_prop.physical_defence;
	entity->m_fightprop.magic_defence -= m_prop.magic_defence;
	entity->m_fightprop.skill_defence -= m_prop.skill_defence;
	entity->m_fightprop.accurate -= m_prop.accurate;
	entity->m_fightprop.dodge -= m_prop.dodge;
	entity->m_fightprop.wreck -= m_prop.wreck;
	entity->m_fightprop.parry -= m_prop.parry;
	entity->m_fightprop.critical_strike -= m_prop.critical_strike;
	entity->m_fightprop.tenacity -= m_prop.tenacity;
	entity->m_fightprop.slay -= m_prop.slay;
}

BuffEffectManager::BuffEffectManager(Entity* entity) :
	m_entity(entity),
	m_base_fight_prop(&(entity->m_fightprop))
{
}
	
BuffEffectManager::BuffEffectManager(const BuffEffectManager& manager) {
	m_entity = manager.m_entity;
	m_base_fight_prop = manager.m_base_fight_prop;
}
	
const BuffEffectManager& BuffEffectManager::operator=(const BuffEffectManager& manager) {
	m_entity = manager.m_entity;
	m_base_fight_prop = manager.m_base_fight_prop;
	return *this;
}
	
BuffEffectManager::~BuffEffectManager() {
	clear();
}
	
void BuffEffectManager::load_prop(Entity* entity) {
	m_base_fight_prop = &(entity->m_fightprop);
}

void BuffEffectManager::add_buff(BuffEffect* pEffect) {
	if(exist_buff(pEffect->get_id())) {
		delete pEffect;
		return;
	}
	if( !m_buffs.count(pEffect)) {
		m_buffs.insert(pEffect);
		pEffect->sig_del_self.connect(this, &BuffEffectManager::on_effect_del_self);
		pEffect->do_buff(m_entity, *m_base_fight_prop);
	}
}
	
void BuffEffectManager::add_hero_buff(uint32_t id, uint32_t level) {
	SkillHurt_t *skill_hurt = g_skill_hurt_config->get_hurt_info(id, level);
	if(skill_hurt) {
		if(g_hero_skill_config->is_invincible(id)) {
			InvincibleBuff * buff = new InvincibleBuff(skill_hurt->parts[0].continue_time2, id);
			add_buff(buff);
		} else if(g_hero_skill_config->is_stealth(id)) {
			StealthBuff * buff = new StealthBuff(skill_hurt->parts[0].continue_time, id);
			add_buff(buff);
		} else if(g_hero_skill_config->is_buff(id)) {
			AttrBuff * buff = new AttrBuff(skill_hurt->parts[0].continue_time, skill_hurt->parts[0], id);
			add_buff(buff);
		}
	}
}

void BuffEffectManager::add_hero_buff(uint32_t id, uint32_t level,int32_t percent) {
	//十分钟 这里就是红名buff 这个buff就在竞技的时候生效 CAR TOO DIRTY
	AttrPerBuff * buff = new AttrPerBuff(600, id, percent);
	add_buff(buff);
}
	

void BuffEffectManager::add_fairy_skill_buff(uint32_t id,uint32_t fairy_skill_attack) {
	FairySkill_t *skill = g_fairy_skill_config->get_fairy_skill(id);
	if(skill && skill->type == FairySkillBuff) {	
		FightProp prop;	
		prop.physical_attack = fairy_skill_attack*skill->fight_prop.physical_attack/100;
		prop.magic_attack = fairy_skill_attack*skill->fight_prop.magic_attack/100;
		prop.skill_attack = fairy_skill_attack*skill->fight_prop.skill_attack/100;
		prop.physical_defence = fairy_skill_attack*skill->fight_prop.physical_defence/100;
		prop.magic_defence = fairy_skill_attack*skill->fight_prop.magic_defence/100;
		prop.skill_defence = fairy_skill_attack*skill->fight_prop.skill_defence/100;
		prop.accurate = skill->fight_prop.accurate;
		prop.dodge = skill->fight_prop.dodge;
		prop.wreck = skill->fight_prop.wreck;
		prop.parry = skill->fight_prop.parry;
		prop.critical_strike = skill->fight_prop.critical_strike;
		prop.tenacity = skill->fight_prop.tenacity;
		prop.slay = skill->fight_prop.slay;
		prop.health_point = fairy_skill_attack*skill->fight_prop.health_point/100;
		BuffEffect * buff = new SkillBuff(skill->continue_time, prop, id);
		add_buff(buff);
	}
}
	
void BuffEffectManager::redo_buff() {
	BuffList::iterator itr = m_buffs.begin();
	for(; itr!=m_buffs.end(); ++itr) {
		(*itr)->undo_buff(m_entity, *m_base_fight_prop);
		(*itr)->do_buff(m_entity, *m_base_fight_prop);
	}
}
	
void BuffEffectManager::on_effect_del_self(BuffEffect* pEffect) {
	m_buffs.erase(pEffect);
	pEffect->undo_buff(m_entity, *m_base_fight_prop);
	sig_undo_buff(m_entity);
}
	
void BuffEffectManager::clear() {
	BuffList::iterator itr = m_buffs.begin();
	for(; itr!=m_buffs.end(); ++itr) {
		delete *itr;
	}
	m_buffs.clear();
}
	
bool BuffEffectManager::exist_buff(uint32_t id) {
	BuffList::iterator itr = m_buffs.begin();
	for(; itr!=m_buffs.end(); ++itr) {
		if((*itr)->get_id() == id) return true;
	}
	return false;
}
