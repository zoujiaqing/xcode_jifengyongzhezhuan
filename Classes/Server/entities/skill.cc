#include "skill.h"
#include "../base/Time.hpp"
#include "../battlefields/battlefield.h"
#include "buffeffect.h"
#include <algorithm>

EffectSpace::EffectSpace(FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t effect_time, uint32_t dead_time, uint32_t attack_num, uint32_t rage_point):
	m_status(IDLE),
	m_effect_time(effect_time),
	m_dead_time(dead_time),
	m_update_time(0),
	m_attacker_prop(attacker_prop),
	m_skill_type(skill_type),
	m_damage(damage),
	m_attack_number(attack_num),
	m_rage_point(rage_point)
{
	if(!m_effect_time) {
		m_status = EFFECT; 
	}
	if(!m_dead_time) {
		m_status = DEAD;
	}
}

bool EffectSpace::update()
{
	if(m_status == DEAD) {
		return false;
	}
	m_update_time += UPDATETIME;
	if(m_update_time >= m_effect_time) {
		m_status = EFFECT;
	}
	if(m_update_time >= m_dead_time) {
		m_status = DEAD;
		return false;
	}
	return true;
};

void EffectSpace::link_object(Entity* pObject) {
	pObject->sig_under_attack_back.connect(this, &EffectRec::on_under_attack_back);	
}
	
void EffectSpace::on_under_attack_back( Entity* ) {
	if(m_status != EFFECT)
		m_status = DEAD;
}
	
SummonEffect::SummonEffect(MapMonsterGroup_t& group, uint32_t effect_time):
	m_status(IDLE),
	m_effect_time(effect_time),
	m_update_time(0),
	m_group(group),
	m_effect(false) {
}
	
bool SummonEffect::update(){
	if(m_effect) {
		return false;
	}
	m_update_time += UPDATETIME;
	if(m_update_time >= m_effect_time) {
		m_status = EFFECT;
	}
	return true;
};
	
void SummonEffect::effect_battle(Battlefield* battle)
{
	battle->dynamic_add_monster(m_group);
	m_effect = true;
};

EffectRec::EffectRec(TileWorld<>::Rectangle rec, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t effect_time, uint32_t dead_time, uint32_t effect_interval, uint32_t attack_num, uint32_t rage_point):
	EffectSpace(attacker_prop, skill_type, damage, effect_time, dead_time, attack_num, rage_point),
	m_rec(rec),
	m_effect_update(0),
	m_effect_interval(effect_interval) 
{
};
	
bool EffectRec::effect(Entity* des, CSMonsterHitRsp* rsp) {
	if( status() == EFFECT && !m_effected_list.count(des) && m_rec.touch(des->space())) {
		DEBUG_LOG("EFFECTREC EFFECT (%d %d)", base::Time().seconds(), base::Time().u_seconds());
		CSMonsterHitRsp_UserInfo * user = rsp->add_users();
		user->set_userid(des->get_id().id());
		user->set_reg_time(des->get_id().regtime());
		user->set_channel_id(des->get_id().channelid());
		uint32_t hurt = 0;
		AttackValueType type;
		des->count_under_attack(m_skill_type,m_damage, m_attacker_prop ,type, hurt, m_attack_number);
		user->set_hurt_type(type);
		user->set_blood(hurt);
		user->set_rage_point(m_rage_point/m_attack_number);
		m_effected_list.insert(des);
		return true;
	}
	return false;
}

bool EffectRec::update() {
	bool ret = EffectSpace::update();
	if(status() == EFFECT && m_effect_interval) {
		m_effect_update += UPDATETIME;
		if(m_effect_update >= m_effect_interval) {
			m_effected_list.clear();
			m_effect_update = 0;
		}
	}
	return ret;
}

void EffectRec::link_object_dead(Entity* object)
{
	object->sig_obj_del.connect(this,&EffectRec::on_object_dead);
}
	
void EffectRec::on_object_dead(base::Object<player_key_t>*) {
	m_status = DEAD;
}
	
MoveEffectRec::MoveEffectRec(TileWorld<>::Rectangle rec, TileWorld<>::Point src, TileWorld<>::Point des, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t speed, uint32_t effect_time, uint32_t attack_num, uint32_t rage_point):
	EffectSpace(attacker_prop, skill_type, damage, effect_time, 0, attack_num, rage_point),
	m_rec(rec),
	m_src(src),
	m_des(des),
	m_speed(speed),
	m_accumulate(0),
	m_last_effect(false)
{
	m_status = IDLE; 
};
	
MoveEffectRec::~MoveEffectRec(){
}

bool MoveEffectRec::effect(Entity* des, CSMonsterHitRsp* rsp) {
	TileWorld<>::Rectangle effect_rec = m_rec;
	effect_rec.move(m_src);
//	DEBUG_LOG("MOVE EFFECT NOW PALCE:(%d %d) (%d %d) DES(%d %d) (%d %d) STATUS:%d",effect_rec.start.x,effect_rec.start.y, effect_rec.end.x,effect_rec.end.y, des->space().start.x, des->space().start.y, des->space().end.x, des->space().end.y, status());
	if( status() == EFFECT && !m_effected_list.count(des) && effect_rec.touch(des->space())) {
		CSMonsterHitRsp_UserInfo * user = rsp->add_users();
		user->set_userid(des->get_id().id());
		user->set_reg_time(des->get_id().regtime());
		user->set_channel_id(des->get_id().channelid());
		uint32_t hurt = 0;
		AttackValueType type;
		des->count_under_attack(m_skill_type,m_damage, m_attacker_prop ,type, hurt, m_attack_number);
		user->set_hurt_type(type);
		user->set_blood(hurt);
		user->set_rage_point(m_rage_point/m_attack_number);
		m_effected_list.insert(des);
		return true;
	}
	return false;
}
	
bool MoveEffectRec::update() {
	if (m_src == m_des || m_status == DEAD) {
		if(!m_last_effect) {
			m_last_effect = true;
			return true;
		} else {
			m_status = DEAD;
			return false;
		}
	}
	m_update_time += UPDATETIME;
	if(m_update_time >= m_effect_time) {
		m_status = EFFECT;
		m_accumulate += m_speed * 10 * UPDATETIME / 1000;
		m_src = TileWorld<>::walk_direct_path(m_src, m_des, m_accumulate);
//		DEBUG_LOG("TEST MOVE START(%d %d) END(%d %d) %p",m_src.x,m_src.y, m_des.x, m_des.y, this);
		return true;
	}
	return true;
}
	
ObjectFixMoveEffect::ObjectFixMoveEffect(TileWorld<>::Rectangle rec, TileWorld<>::Point src, TileWorld<>::Point des, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t speed, uint32_t effect_time, uint32_t effect_interval, uint32_t attack_num, uint32_t rage_point, uint32_t move_time):
	MoveEffectRec(rec, src, des, attacker_prop, skill_type, damage, speed, effect_time, attack_num, rage_point),
	m_object(NULL),
	end_des(des),
	m_effect_update(0),
	m_effect_interval(effect_interval), 
	m_move_time(move_time)
{
	m_effect_time = effect_time;
}	
	
ObjectFixMoveEffect::~ObjectFixMoveEffect(){
	if(m_object) {
		m_object->m_pos = end_des;
	}
}
	
void ObjectFixMoveEffect::link_object(Entity* object)
{
	m_object = object;
	m_object->sig_under_attack_back.connect(this, &ObjectFixMoveEffect::on_object_stop);
}
	
void ObjectFixMoveEffect::link_object_leave(Entity* object)
{
	m_object = object;
	m_object->sig_obj_del.connect(this,&ObjectFixMoveEffect::on_object_leave);
}
	
void ObjectFixMoveEffect::on_object_leave(base::Object<player_key_t>* pObject)
{
	m_object = NULL;
	m_status = DEAD;
}

void ObjectFixMoveEffect::on_object_stop(Entity* pObject)
{
	m_object = NULL;
	m_status = DEAD;
	pObject->set_idle(UPDATETIME*4);
}
	
bool ObjectFixMoveEffect::update() {
	bool ret = false;
	if(m_move_time) {
		m_update_time += UPDATETIME;
		if(m_update_time >= m_move_time + m_effect_time) {
			m_status = DEAD;
			ret = false;
		}else if(m_update_time >= m_effect_time) {
			m_status = EFFECT;
			m_accumulate += m_speed * 10 * UPDATETIME / 1000;
			m_src = TileWorld<>::walk_direct_path(m_src, m_des, m_accumulate);
	//		DEBUG_LOG("TEST MOVE START(%d %d) END(%d %d) %p",m_src.x,m_src.y, m_des.x, m_des.y, this);
			ret = true;
		}else {
			ret = true;
		}
	} else {
		ret = MoveEffectRec::update(); 
	}
	if(status() == EFFECT && m_effect_interval) {
		m_effect_update += UPDATETIME;
		if(m_effect_update >= m_effect_interval) {
			m_effected_list.clear();
			m_effect_update = 0;
		}
	}
	if(m_object) m_object->m_pos = m_src;
	return ret;
}
	
ObjectMoveEffect::ObjectMoveEffect(TileWorld<>::Rectangle rec, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t speed, uint32_t effect_time, uint32_t dead_time, uint32_t effect_interval, uint32_t attack_num, uint32_t rage_point) :
	EffectSpace(attacker_prop, skill_type, damage, effect_time, dead_time, attack_num, rage_point),
	m_rec(rec),
	m_effect_update(0),
	m_effect_interval(effect_interval),
	m_speed(speed) 
{

}	
	
bool ObjectMoveEffect::effect(Entity* des, CSMonsterHitRsp* rsp)
{
	if(m_object && status() == EFFECT ) {
		TileWorld<>::Rectangle rec = m_rec;
		rec.move(m_object->m_pos);
		if( status() == EFFECT && !m_effected_list.count(des) && rec.touch(des->space())) {
			CSMonsterHitRsp_UserInfo * user = rsp->add_users();
			user->set_userid(des->get_id().id());
			user->set_reg_time(des->get_id().regtime());
			user->set_channel_id(des->get_id().channelid());
			uint32_t hurt = 0;
			AttackValueType type;
			des->count_under_attack(m_skill_type,m_damage, m_attacker_prop ,type, hurt, m_attack_number);
			user->set_hurt_type(type);
			user->set_blood(hurt);
			user->set_rage_point(m_rage_point/m_attack_number);
			m_effected_list.insert(des);
			return true;
		}
	}
	return false;
}
	
void ObjectMoveEffect::link_object(Entity* object) {
	m_object = object;
	m_object->sig_obj_del.connect(this,&ObjectMoveEffect::on_object_leave);
}
	
void ObjectMoveEffect::on_object_leave(base::Object<player_key_t>* pObject) {
	m_object = NULL;
}
		
bool ObjectMoveEffect::update() {
	bool ret = EffectSpace::update();
	if(status() == EFFECT) {
		m_effect_update += UPDATETIME;
		DEBUG_LOG("NOW OBJECT MOVE EFFECT :%d :%d", m_effect_update, m_effect_interval);
		if(m_effect_update >= m_effect_interval) {
			m_effected_list.clear();
			m_effect_update = 0;
		}
	}
	return ret;
}
	
ObjectJumpEffect::ObjectJumpEffect(TileWorld<>::Rectangle rec, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t effect_time, uint32_t dead_time, uint32_t attack_num, uint32_t rage_point) :
	EffectSpace(attacker_prop, skill_type, damage, effect_time, dead_time, attack_num, rage_point),
	m_rec(rec)
{

}	
	
bool ObjectJumpEffect::effect(Entity* des, CSMonsterHitRsp* rsp)
{
	if(m_object && status() == EFFECT ) {
		TileWorld<>::Rectangle rec = m_rec;
		rec.move(m_object->m_pos);
		if( status() == EFFECT && !m_effected_list.count(des) && rec.touch(des->space())) {
			CSMonsterHitRsp_UserInfo * user = rsp->add_users();
			user->set_userid(des->get_id().id());
			user->set_reg_time(des->get_id().regtime());
			user->set_channel_id(des->get_id().channelid());
			uint32_t hurt = 0;
			AttackValueType type;
			des->count_under_attack(m_skill_type,m_damage, m_attacker_prop ,type, hurt, m_attack_number);
			user->set_hurt_type(type);
			user->set_blood(hurt);
			user->set_rage_point(m_rage_point/m_attack_number);
			m_effected_list.insert(des);
			return true;
		}
	}
	return false;
}
	
void ObjectJumpEffect::link_object(Entity* object) {
	m_object = object;
	m_object->sig_obj_del.connect(this,&ObjectJumpEffect::on_object_leave);
}
	
void ObjectJumpEffect::on_object_leave(base::Object<player_key_t>* pObject) {
	m_object = NULL;
}

EffectManager::~EffectManager() {
	{
		SummonEffectList::iterator itr = m_summon_effect_list.begin();
		for(;itr!=m_summon_effect_list.end();) {
			delete *itr;
			itr++;
		}
	}
	{
		EffectList::iterator itr = m_effect_list.begin();
		for(;itr!=m_effect_list.end();) {
			delete *itr;
			itr++;
		}
	}
}

void EffectManager::update() {
	{
		SummonEffectList::iterator itr = m_summon_effect_list.begin();
		for(;itr!=m_summon_effect_list.end();) {
			if(!(*itr)->update()) {
				delete *itr;
				itr = m_summon_effect_list.erase(itr);
			} else {
				++itr;
			}
		}
	}	
	{
		EffectList::iterator itr = m_effect_list.begin();
		for(;itr!=m_effect_list.end();) {
			if(!(*itr)->update()) {
				delete *itr;
				itr = m_effect_list.erase(itr);
			} else {
				++itr;
			}
		}
	}
}
	
void EffectManager::effect(Entity* des, CSMonsterHitRsp* rsp) {
	EffectList::iterator itr = m_effect_list.begin();
	for(;itr!=m_effect_list.end();) {
		if((*itr)->status() == EFFECT) {
			(*itr) -> effect(des, rsp);
		}
		itr++;
	}	
}

void EffectManager::effect_battle(Battlefield* battle) {
	SummonEffectList::iterator itr = m_summon_effect_list.begin();
	for(;itr!=m_summon_effect_list.end();) {
		if((*itr)->status() == EFFECT) {
			(*itr) -> effect_battle(battle);
		}
		itr++;
	}
}
	
void EffectManager::add_effect(EffectSpace* peffect ) {
	m_effect_list.push_back(peffect);
}
	
void EffectManager::add_effect(SummonEffect* peffect) {
	m_summon_effect_list.push_back(peffect);
}

Skill::Skill(uint32_t id) :
	m_id(id), m_available(true), m_update_time(0), m_break(true) {

};
	
void Skill::update() {
	if(!m_available) {
		m_update_time += UPDATETIME;
		if(m_update_time >= m_cool_down) {
			m_update_time = 0;
			m_available = true;
		}
	} else {
		m_update_time = 0;
	}
}
	
bool Skill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	m_update_time = 0;
	m_available = false;
    info->set_effect_animation(m_effect_animation);
	src->sig_under_attack_back.connect(this, &Skill::on_monster_under_attack_back);
	return true;
}
	
void Skill::on_monster_under_attack_back(Entity* src) {
	m_available = true;
	m_update_time  = 0;
}
	
uint32_t Skill::get_id() {
	return m_id;
}
	
bool Skill::available() {
	return m_available;
}
	
Skill* Skill::generate_skill(uint32_t role_id, uint32_t id) {
	return NULL;
}

MeleeSkill::MeleeSkill(uint32_t id) :
	Skill(id)
{
}
	
bool MeleeSkill::try_action(Entity* src, Entity* des) {
	Skill::try_action(src, des);
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.touch(des->space())) {
			return true;	
		}
	}
	return false;
}

bool MeleeSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	info->set_type(MELEE_ATTACK);
	if(m_recs.find(direct) != m_recs.end()) {
		TileWorld<>::Rectangle rec = m_recs[direct];
		rec.move(src->m_pos);
		if(	rec.touch(des->space()) )
		{
			EffectRec* effect= new EffectRec(rec, src->m_fightprop, m_skill_type, m_damage, m_effect_time, m_dead_time, m_attack_rate, m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break) {
				effect->link_object(src);
			}
			effect->link_object_dead(src);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}
			src->set_idle(m_cool_down);	
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.touch(des->space())) {
			EffectRec* effect= new EffectRec(rec, src->m_fightprop, m_skill_type, m_damage, m_effect_time, m_dead_time, m_attack_rate, m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break) {
				effect->link_object(src);
			}
			direct = itr->first;
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}
			src->set_idle(m_cool_down);	
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}

NoTargetMeleeSkill::NoTargetMeleeSkill(uint32_t id) :
	Skill(id)
{
}
	
bool NoTargetMeleeSkill::try_action(Entity* src, Entity* des) {
	return true;	
}

bool NoTargetMeleeSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	info->set_type(MELEE_ATTACK);
	RecMap::iterator itr = m_recs.begin();
	TileWorld<>::Rectangle rec = itr->second;
	rec.move(src->m_pos);
	EffectRec* effect= new EffectRec(rec, src->m_fightprop, m_skill_type, m_damage, m_effect_time, m_dead_time, m_attack_rate, m_attack_number, m_rage_point);
	pManager->add_effect(effect);
	if(m_break) {
		effect->link_object(src);
	}
	direct = itr->first;
	if(direct.y > 0) {
		info->set_action_id(m_up_animation);
	} else {
		info->set_action_id(m_down_animation);
	}
	src->set_idle(m_cool_down);	
	return Skill::do_action(src, des, direct, pManager, info);
}

	
ShootSkill::ShootSkill(uint32_t id, TileWorld<>* tileworld) :
	Skill(id),
	m_tileworld(tileworld)
{
}
	
bool ShootSkill::try_action(Entity* src, Entity* des) {
	Skill::try_action(src, des);
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.distance2(des->space()) <= m_dead_len*m_dead_len ) {
			if (m_tileworld->search_path_end(src->m_pos, des->m_pos) == des->m_pos )
				return true;	
		}
	}
	return false;
}
	
bool ShootSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(direct.x == 0) {direct.x = 1;};
	if(direct.y == 0) {direct.y = 1;};
	if(m_recs.find(direct) != m_recs.end() && m_offsets.find(direct) != m_offsets.end()) {
		TileWorld<>::Rectangle rec = m_recs[direct];
		TileWorld<>::Point offset = m_offsets[direct];
		{
			TileWorld<>::Point start_pos = src->m_pos;
			start_pos += offset;
			TileWorld<>::Point end_pos 	= des->space().center();		
			if(start_pos == end_pos) {
					end_pos.y += m_dead_len; 
			}
			float dis = start_pos.distance(end_pos);
			if(dis < m_dead_len) {
				end_pos -= start_pos;
				end_pos *= (m_dead_len/dis);
				end_pos += start_pos;
			}
			DEBUG_LOG("MOVE EFFECT START:(%d %d) DIRECT(%d %d) ",start_pos.x,start_pos.y, direct.x, direct.y);
			MoveEffectRec* effect= new MoveEffectRec(rec, start_pos, end_pos, src->m_fightprop, m_skill_type, m_damage, m_speed, m_effect_time, m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break)
				effect->link_object(src);
/*template code*/
			info->set_type(2);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}
			CSPoint* start_point = info->mutable_start_pos();
			start_point->set_xpos(start_pos.x);
			start_point->set_ypos(start_pos.y);
			CSPoint* end_point = info->mutable_end_pos();
			end_point->set_xpos(end_pos.x);
			end_point->set_ypos(end_pos.y);
			info -> set_skill_speed(m_speed);
			info -> set_delay_time(m_effect_time);
			info -> set_effect_id(m_effect_id);
			src->set_idle(m_cool_down);	
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}

NoTargetShootSkill::NoTargetShootSkill(uint32_t id, TileWorld<>* tileworld) :
	Skill(id),
	m_tileworld(tileworld)
{
}
	
bool NoTargetShootSkill::try_action(Entity* src, Entity* des) {
	return true;	
}
	
bool NoTargetShootSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	TileWorld<>::Rectangle rec = m_recs.begin()->second;
	TileWorld<>::Point offset = m_offsets.begin()->second;
	TileWorld<>::Point start_pos = src->m_pos;
	start_pos += offset;
	TileWorld<>::Point end_pos = m_direct;
	float dis = end_pos.distance(TileWorld<>::Point());
	end_pos *= (m_dead_len/dis);
	end_pos += start_pos;
	DEBUG_LOG("MOVE EFFECT START:(%d %d) DIRECT(%d %d) ",start_pos.x,start_pos.y, direct.x, direct.y);
	MoveEffectRec* effect= new MoveEffectRec(rec, start_pos, end_pos, src->m_fightprop, m_skill_type, m_damage, m_speed, m_effect_time, m_attack_number, m_rage_point);
	pManager->add_effect(effect);
	if(m_break)
		effect->link_object(src);
/*template code*/
	info->set_type(2);
	if(direct.y > 0) {
		info->set_action_id(m_up_animation);
	} else {
		info->set_action_id(m_down_animation);
	}
	CSPoint* start_point = info->mutable_start_pos();
	start_point->set_xpos(start_pos.x);
	start_point->set_ypos(start_pos.y);
	CSPoint* end_point = info->mutable_end_pos();
	end_point->set_xpos(end_pos.x);
	end_point->set_ypos(end_pos.y);
	info -> set_skill_speed(m_speed);
	info -> set_delay_time(m_effect_time);
	info -> set_effect_id(m_effect_id);
	src->set_idle(m_cool_down);	
	return Skill::do_action(src, des, direct, pManager, info);
}

	
ObjectFixMoveSkill::ObjectFixMoveSkill(uint32_t id, TileWorld<>* tileworld):
	Skill(id),
	m_tileworld(tileworld)
{
}

bool ObjectFixMoveSkill::try_action(Entity* src, Entity* des)
{
	Skill::try_action(src, des);
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.distance2(des->space()) <= m_dead_len*m_dead_len ) {
			return true;	
		}
	}
	return false;
}

bool ObjectFixMoveSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info)
{
	if(direct.x == 0) {direct.x = 1;};
	if(direct.y == 0) {direct.y = 1;};
	if(m_recs.find(direct) != m_recs.end() && m_offsets.find(direct) != m_offsets.end()) {
		TileWorld<>::Rectangle rec = m_recs[direct];
		TileWorld<>::Point offset = m_offsets[direct];
		{
			TileWorld<>::Point start_pos = src->m_pos;
			//start_pos += offset; //delete wrong offset;
			TileWorld<>::Point end_pos 	= des->m_pos;		
			if(start_pos == end_pos) {
					end_pos.y += m_dead_len; 
			}
			float dis = start_pos.distance(end_pos);
			if(dis < m_dead_len) {
				end_pos -= start_pos;
				end_pos *= (m_dead_len/dis);
				end_pos += start_pos;
			}
			TileWorld<>::Point virtual_pos = end_pos;
			end_pos = m_tileworld->search_path_end(start_pos, end_pos); 
			ObjectFixMoveEffect* effect= new ObjectFixMoveEffect(rec, start_pos, end_pos, src->m_fightprop, m_skill_type, m_damage, m_speed, m_effect_time, 0, m_attack_number, m_rage_point, m_dead_len*1000/m_speed);
			if(m_break)
				effect->link_object(src);
			effect->link_object_leave(src);
			pManager->add_effect(effect);
/*template code*/
			info->set_type(4);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
				info -> set_effect_id(m_link_animation_up);
			} else {
				info->set_action_id(m_down_animation);
				info -> set_effect_id(m_link_animation_down);
			}
			CSPoint* start_point = info->mutable_start_pos();
			start_point->set_xpos(start_pos.x);
			start_point->set_ypos(start_pos.y);
			CSPoint* end_point = info->mutable_end_pos();
			end_point->set_xpos(end_pos.x);
			end_point->set_ypos(end_pos.y);
			CSPoint* virtual_point = info->mutable_virtual_pos();
			virtual_point->set_xpos(virtual_pos.x);
			virtual_point->set_ypos(virtual_pos.y);
			info -> set_skill_speed(m_speed);
			info -> set_delay_time(m_effect_time);
			DEBUG_LOG("MOVE EFFECT START:(%d %d) END:(%d %d) DIRECT(%d %d) DELAY:%d",start_pos.x,start_pos.y, end_pos.x, end_pos.y, direct.x, direct.y, m_effect_time);
			src->set_idle(m_cool_down);	
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}
	
ObjectMoveSkill::ObjectMoveSkill(uint32_t id) :
	Skill(id),
	m_current_rate(0)
{
}
	
bool ObjectMoveSkill::try_action(Entity* src, Entity* des) {
	return true;
}
	
void ObjectMoveSkill::update() {
	Skill::update();
}
	
bool ObjectMoveSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(!m_recs.empty()) {
		TileWorld<>::Rectangle rec = m_recs.begin()->second;
		ObjectMoveEffect* effect = new ObjectMoveEffect(rec, src->m_fightprop, m_skill_type, m_damage, m_speed, m_effect_time, m_continue_time+m_effect_time, m_attack_rate, m_attack_number, m_rage_point);
		if(m_break) {
			effect->link_object(src);
		}
		pManager->add_effect(effect);
		SpeedBuff * buff = new SpeedBuff(m_continue_time, m_speed, 0);
		src->m_buffmanager->add_buff(buff);
		info->set_type(FOLLOW_ATTACK);
		info->set_delay_time(m_effect_time);
		if(direct.y > 0) {
			info->set_action_id(m_up_animation);
			info -> set_effect_id(m_link_animation_up);
		} else {
			info->set_action_id(m_down_animation);
			info -> set_effect_id(m_link_animation_down);
		}
		info->set_skill_speed(m_speed);
		info->set_dead_time(m_continue_time + m_effect_time);
		src->set_idle(m_effect_time);
		return Skill::do_action(src, des, direct, pManager, info);
	}
	return false;
}
	
bool ObjectMoveSkill::do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info) {
	if(m_update_time >= m_continue_time + m_effect_time) {
		src->set_idle(m_cool_down - m_continue_time - m_effect_time);
	}
	return true;
}
	
TargetSkill::TargetSkill(uint32_t id):
	Skill(id) {
}

bool TargetSkill::try_action(Entity* src, Entity* des) {
	Skill::try_action(src, des);
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.distance2(des->space()) <= m_dead_len*m_dead_len ) {
			return true;	
		}
	}
	return false;
}
	
bool TargetSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(!m_recs.empty()) {
		TileWorld<>::Rectangle rec = m_recs.begin()->second;
		{
			TileWorld<>::Point pos 	= des->space().center();		
			DEBUG_LOG("TARGET EFFECT POS:(%d %d) DIRECT(%d %d) ",pos.x,pos.y, direct.x, direct.y);
			rec.move(des->space().center()-=rec.center());
			EffectRec* effect= new EffectRec(rec, src->m_fightprop, m_skill_type, m_damage, m_effect_time, m_dead_time,0 , m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break)
				effect->link_object(src);
/*template code*/
			info->set_type(TARGET_ATTACK);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}
			CSPoint* end_point = info->mutable_end_pos();
			end_point->set_xpos(pos.x);
			end_point->set_ypos(pos.y);
			info -> set_delay_time(m_release_time);
			info -> set_effect_id(m_effect_id);
			src->set_idle(m_cool_down);	
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}

NoTargetSkill::NoTargetSkill(uint32_t id):
	Skill(id) {
}

bool NoTargetSkill::try_action(Entity* src, Entity* des) {
	return true;	
}
	
bool NoTargetSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(!m_recs.empty()) {
		TileWorld<>::Rectangle rec = m_recs.begin()->second;
		{
			TileWorld<>::Point target = m_des;
			TileWorld<>::Point pos 	= target;		
			DEBUG_LOG("TARGET EFFECT POS:(%d %d) DIRECT(%d %d) ",pos.x,pos.y, direct.x, direct.y);
			rec.move(target);
			EffectRec* effect= new EffectRec(rec, src->m_fightprop, m_skill_type, m_damage, m_effect_time, m_dead_time, m_attack_rate, m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break)
				effect->link_object(src);
/*template code*/
			info->set_type(TARGET_ATTACK);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}
			CSPoint* end_point = info->mutable_end_pos();
			end_point->set_xpos(pos.x);
			end_point->set_ypos(pos.y);
			info -> set_delay_time(m_release_time);
			info -> set_effect_id(m_effect_id);
			src->set_idle(m_cool_down);	
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}
	
TargetMultiSkill::TargetMultiSkill(uint32_t id) :
	Skill(id),
	m_current_num(0),
	m_current_rate(0)
{
}

bool TargetMultiSkill::try_action(Entity* src, Entity* des) {
	Skill::try_action(src, des);
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.distance2(des->space()) <= m_dead_len*m_dead_len ) {
			return true;	
		}
	}
	return false;
}
	
void TargetMultiSkill::update() {
	Skill::update();
	if(m_update_time > m_release_time) {
		if(m_current_num < m_effect_num) {
			m_current_rate += UPDATETIME;
		} else {
			m_current_rate = 0;
		}	
	}
}
	
bool TargetMultiSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(!m_recs.empty()) {
		TileWorld<>::Rectangle rec = m_recs.begin()->second;
		{
			TileWorld<>::Point pos 	= des->space().center();		
			DEBUG_LOG("TARGET MULTI EFFECT POS:(%d %d) DIRECT(%d %d) ",pos.x,pos.y, direct.x, direct.y);
			info->set_type(MULT_REMOTE);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}
			CSPoint* end_point = info->mutable_end_pos();
			end_point->set_xpos(pos.x);
			end_point->set_ypos(pos.y);
			info -> set_delay_time(m_release_time);
			info -> set_effect_id(m_effect_id);
			src->set_idle(m_cool_down);	
			info -> set_dead_time(m_dead_time);
			for(std::vector<uint32_t>::iterator itr = m_camera_shake_time.begin(); itr!= m_camera_shake_time.end(); ++itr) {
				info->add_shake_time(*itr);
			}
			m_current_num = 0;
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}
	
bool TargetMultiSkill::do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info) {
	if(!m_recs.empty() && m_current_rate >= m_attack_rate ) {
		TileWorld<>::Rectangle rec = m_recs.begin()->second;
		{
			CSMonsterEffectInfo * effect_info = info->add_effects();
		    effect_info->set_effect_animation(m_effect_animation);
			TileWorld<>::Point pos 	= des->space().center();		
			DEBUG_LOG("TARGET MULTI EFFECT POS:(%d %d) DIRECT(%d %d) ",pos.x,pos.y, direct.x, direct.y);
			rec.move(des->space().center()-=rec.center());
			EffectRec* effect= new EffectRec(rec, src->m_fightprop, m_skill_type, m_damage, m_link_effect_time, m_link_effect_end,0 , m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break)
				effect->link_object(src);
			effect_info->set_type(MULT_REMOTE);
			effect_info -> set_effect_id(m_effect_id);
			CSPoint* end_point = effect_info->mutable_end_pos();
			end_point->set_xpos(pos.x);
			end_point->set_ypos(pos.y);
			effect_info -> set_delay_time(0);
			m_current_rate = 0;
			m_current_num ++;
			return true;
		}
	}
	return false;
}

SummonSkill::SummonSkill(uint32_t id) :
	Skill(id) {
}
	
bool SummonSkill::try_action(Entity* src, Entity* des) {
	return true;
}
	
bool SummonSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	info->set_type(5);
	Monster* pmonster = dynamic_cast<Monster*>(src);
	if(pmonster) {
		std::vector<MapMonster_t>::iterator itr = m_group.monsters.begin();
		for(; itr!= m_group.monsters.end(); itr++) {
			itr->monster_level = pmonster->get_level();
		}
	}
	SummonEffect* effect= new SummonEffect(m_group, m_effect_time);
	pManager->add_effect(effect);
	if(direct.y > 0) {
		info->set_action_id(m_up_animation);
	} else {
		info->set_action_id(m_down_animation);
	}
	src->set_idle(m_cool_down);	
	return Skill::do_action(src, des, direct, pManager, info);
}
	
BuffSkill::BuffSkill(uint32_t id) :
	Skill(id) {
}
	
bool BuffSkill::try_action(Entity* src, Entity* des) {
	return true;
}
	
bool BuffSkill::do_buff(Entity* src, base::Manager<Monster>* des, CSBuffEffectRsp* rsp) {
	FightProp  add_prop = src->m_fightprop;
	add_prop.physical_attack = src->m_fightprop.physical_attack * prop.physical_attack /100;
	add_prop.magic_attack = src->m_fightprop.magic_attack * prop.magic_attack /100;
	add_prop.skill_attack = src->m_fightprop.skill_attack * prop.skill_attack /100;
	add_prop.physical_defence = src->m_fightprop.physical_defence * prop.physical_defence /100;
	add_prop.magic_defence = src->m_fightprop.magic_defence * prop.magic_defence /100;
	add_prop.skill_defence = src->m_fightprop.skill_defence * prop.skill_defence /100;
	add_prop.accurate = prop.accurate;
	add_prop.dodge = prop.dodge;
	add_prop.wreck = prop.wreck;
	add_prop.parry = prop.parry;
	add_prop.critical_strike = prop.critical_strike;
	add_prop.tenacity = prop.tenacity;
	add_prop.slay = prop.slay;
	add_prop.health_point = prop.health_point;

	CSMonsterBuffEffectInfo* info =rsp->add_effects();
	SkillBuff * buff = new SkillBuff(m_continue_time,add_prop, 0);
	src->m_buffmanager->add_buff(buff);
	TileWorld<>::Point direct = src->m_direct;
	if(direct.y > 0) {
		info->set_action_id(m_up_animation);
	} else {
		info->set_action_id(m_down_animation);
	}
	info->set_effect_id(m_effect_id);
	info->set_delay_time(m_effect_time);
	info->set_dead_time(m_continue_time);
	info->set_skill_id(m_id);
	src->set_idle(m_cool_down);	

	CSMonsterBuffEffectInfo_BuffEffectInfo* buff_info = info->add_buff_info();
	buff_info->set_id(src->get_id().id());
	
	MonsterBattleInfo* battle_info = buff_info->mutable_battle_info();
	(static_cast<Monster*>(src))->pack_battle_info(battle_info);
	
	m_update_time = 0;
    m_available = false;

	DEBUG_LOG("DO BUFF: CONTINUE TIME:%d",m_continue_time);
	return true;
}
	
TeleportSkill::TeleportSkill(uint32_t id, TileWorld<>* tileworld) :
	Skill(id),
	m_tileworld(tileworld)
{
}

bool TeleportSkill::try_action(Entity* src, Entity* des) {
	float dis = src->m_pos.distance(des->m_pos);
	if(dis < m_dead_len/2) {
		return true;
	} else {
		return false;
	}
}

bool TeleportSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	TileWorld<>::Point end_pos = src->m_pos;
	TileWorld<>::Point start_pos = des->m_pos;		
	if(start_pos == end_pos) {
		end_pos.y += m_dead_len; 
	}
	float dis = start_pos.distance(end_pos);
	end_pos -= start_pos;
	end_pos *= ((m_dead_len/dis) + 1);
	end_pos += start_pos;
	TileWorld<>::Point end_pos1 = m_tileworld->search_path_end(src->m_pos, end_pos); 
	end_pos = src->m_pos;
	if(start_pos == end_pos) {
		end_pos.y += m_dead_len; 
	}
	end_pos -= start_pos;
	end_pos *= (-(m_dead_len/dis) + 1);
	end_pos += start_pos;
	TileWorld<>::Point end_pos2 = m_tileworld->search_path_end(src->m_pos, end_pos); 
	
	TileWorld<>::Point destination = end_pos1.distance2(des->m_pos) > end_pos2.distance2(des->m_pos)? end_pos1: end_pos2;
	if(destination.distance2(src->m_pos) <=4 ) {
		destination = src->m_birth_pos;
	}
	info->set_type(TELEPORT);
	direct = destination - src->m_pos;
	if(direct.y > 0) {
		info->set_action_id(m_up_animation);
		info->set_effect_id(m_link_animation_up);
	} else {
		info->set_action_id(m_down_animation);
		info -> set_effect_id(m_link_animation_down);
	}
	DEBUG_LOG("TELEPORT EFFECT FROM_POS:(%d %d) USER_POS:(%d %d) END_POS:(%d %d) END1(%d, %d) END2(%d,%d)", src->m_pos.x, src->m_pos.y, des->m_pos.x, des->m_pos.y, destination.x, destination.y, end_pos1.x, end_pos1.y, end_pos2.x, end_pos2.y );
	CSPoint* end_point = info->mutable_end_pos();
	end_point->set_xpos(destination.x);
	end_point->set_ypos(destination.y);
	src->m_pos = destination;
	src->set_idle(m_cool_down);
	return Skill::do_action(src, des, direct, pManager, info);
}

StealthSkill::StealthSkill(uint32_t id, TileWorld<>* tileworld) :
	Skill(id),
	m_tileworld(tileworld)
{
}

bool StealthSkill::try_action(Entity* src, Entity* des) {
	float dis = src->m_pos.distance(des->m_pos);
	if(dis < m_dead_len/2) {
		return true;
	} else {
		return false;
	}
}

bool StealthSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	TileWorld<>::Point end_pos = src->m_pos;
	TileWorld<>::Point start_pos = des->m_pos;		
	if(start_pos == end_pos) {
		end_pos.y += m_dead_len; 
	}
	float dis = start_pos.distance(end_pos);
	end_pos -= start_pos;
	end_pos *= (m_dead_len/dis);
	end_pos += start_pos;
	TileWorld<>::Point end_pos1 = m_tileworld->search_path_end(des->m_pos, end_pos);
	float dis_choose1 = end_pos1.distance2(des->m_pos);
	
	end_pos.counterclockwise90(des->m_pos);
	TileWorld<>::Point end_pos2 = m_tileworld->search_path_end(des->m_pos, end_pos);
	float dis_choose2 = end_pos2.distance2(des->m_pos);
 
	end_pos.counterclockwise90(des->m_pos);
	TileWorld<>::Point end_pos3 = m_tileworld->search_path_end(des->m_pos, end_pos);
	float dis_choose3 = end_pos3.distance2(des->m_pos);
 
	end_pos.counterclockwise90(des->m_pos);
	TileWorld<>::Point end_pos4 = m_tileworld->search_path_end(des->m_pos, end_pos);
	float dis_choose4 = end_pos4.distance2(des->m_pos);

	float min = std::min<float>(std::min<float>(std::min<float>(dis_choose1, dis_choose2),dis_choose3), dis_choose4);

	base::AverageGen<TileWorld<>::Point> gen;
	if(dis_choose1 == min) {
		gen.push(end_pos2);	
		gen.push(end_pos3);	
		gen.push(end_pos4);	
	} else if (dis_choose2 == min) {
		gen.push(end_pos1);	
		gen.push(end_pos3);	
		gen.push(end_pos4);
	} else if(dis_choose3 != min) {
		gen.push(end_pos1);	
		gen.push(end_pos2);	
		gen.push(end_pos4);	
	} else {
		gen.push(end_pos1);	
		gen.push(end_pos2);	
		gen.push(end_pos3);	
	}
	TileWorld<>::Point destination = gen.get_one();	
	info->set_type(STEALTH);
	direct = destination - src->m_pos;
	if(direct.y > 0) {
		info->set_action_id(m_up_animation);
		info->set_effect_id(m_link_animation_up);
	} else {
		info->set_action_id(m_down_animation);
		info -> set_effect_id(m_link_animation_down);
	}
	DEBUG_LOG("STEALTH EFFECT FROM_POS:(%d %d) USER_POS:(%d %d) END_POS:(%d %d)", src->m_pos.x, src->m_pos.y, des->m_pos.x, des->m_pos.y, destination.x, destination.y);
	CSPoint* end_point = info->mutable_end_pos();
	end_point->set_xpos(destination.x);
	end_point->set_ypos(destination.y);
	src->m_pos = destination;
	src->set_idle(m_cool_down);
	return Skill::do_action(src, des, direct, pManager, info);
}

JumpSkill::JumpSkill(uint32_t id):
	Skill(id),
	m_target(NULL)
{
}

bool JumpSkill::try_action(Entity* src, Entity* des) {
	if(src->space().center().distance2(des->space().center()) <= m_dead_len*m_dead_len ) {
		return true;	
	}
	return false;
}
	
bool JumpSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(direct.x == 0) {direct.x = 1;};
	if(direct.y == 0) {direct.y = 1;};
	if(m_recs.find(direct) != m_recs.end()) {
		TileWorld<>::Rectangle rec = m_recs[direct];
		{
			TileWorld<>::Point pos 	= des->space().center();		
			DEBUG_LOG("JUMP ACTION POS:(%d %d) DIRECT(%d %d) ",pos.x,pos.y, direct.x, direct.y);
			ObjectJumpEffect* effect= new ObjectJumpEffect(rec, src->m_fightprop, m_skill_type, m_damage, m_link_effect_time, m_link_effect_end, m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			effect->link_object(src);
/*template code*/
			info->set_type(JUMP_ATTACK);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
				info->set_effect_id(m_link_animation_up);
			} else {
				info->set_action_id(m_down_animation);
				info->set_effect_id(m_link_animation_down);
			}
			CSPoint* end_point = info->mutable_end_pos();
			end_point->set_xpos(pos.x);
			end_point->set_ypos(pos.y);
			src->m_pos = pos;
			info -> set_delay_time(m_release_time);
			src->set_idle(m_cool_down);	
			des->sig_dead.connect(this, &JumpSkill::on_target_dead);
			des->sig_obj_del.connect(this, &JumpSkill::on_target_del);
			m_target = des;
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}
	
bool JumpSkill::do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info) {
	if(m_update_time >= m_effect_time && m_target) {
		CSMonsterEffectInfo * effect_info = info->add_effects();
		effect_info->set_effect_animation(m_effect_animation);
		effect_info->set_type(JUMP_ATTACK);
		effect_info->set_id(src->get_id().id());
		CSPoint* end_point = effect_info->mutable_end_pos();
		end_point->set_xpos(m_target->m_pos.x);
		end_point->set_ypos(m_target->m_pos.y);
		src->m_pos = m_target->m_pos;
		m_target = NULL;
		return true;
	}
	return false;
}
	
void JumpSkill::on_target_del(base::Object<player_key_t>* object) {
	if(object == m_target) {
		m_target = NULL;
	}
}
	
void JumpSkill::on_target_dead(Entity* object) {
	if(object == m_target) {
		m_target = NULL;
	}
}
		
MultiShootSkill::MultiShootSkill(uint32_t id) :
	Skill(id),
	m_current_num(0),
	m_current_rate(0)
{
}
	
bool MultiShootSkill::try_action(Entity* src, Entity* des) {
	Skill::try_action(src, des);
	RecMap::iterator itr = m_recs.begin();
	for(;itr!=m_recs.end(); itr++) {
		TileWorld<>::Rectangle rec = itr->second;
		rec.move(src->m_pos);
		if(rec.distance2(des->space()) <= m_dead_len*m_dead_len ) {
			return true;	
		}
	}
	return false;
}
	
bool MultiShootSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(direct.x == 0) {direct.x = 1;};
	if(direct.y == 0) {direct.y = 1;};
	if(m_recs.find(direct) != m_recs.end() && m_offsets.find(direct) != m_offsets.end()) {
		TileWorld<>::Rectangle rec = m_recs[direct];
		TileWorld<>::Point offset = m_offsets[direct];
		{
			TileWorld<>::Point start_pos = src->m_pos;
			start_pos += offset;
			TileWorld<>::Point end_pos 	= des->space().center();		
			if(start_pos == end_pos) {
					end_pos.y += m_dead_len; 
			}
			float dis = start_pos.distance(end_pos);
			if(dis < m_dead_len) {
				end_pos -= start_pos;
				end_pos *= (m_dead_len/dis);
				end_pos += start_pos;
			}
			DEBUG_LOG("MOVE EFFECT START:(%d %d) DIRECT(%d %d) ",start_pos.x,start_pos.y, direct.x, direct.y);
			info->set_type(MULT_SHOOT_ATTACK);
			if(direct.y > 0) {
				info->set_action_id(m_up_animation);
			} else {
				info->set_action_id(m_down_animation);
			}

			info -> set_effect_id(m_effect_id);
			src->set_idle(m_cool_down);	
			info -> set_dead_time(m_dead_time);
			return Skill::do_action(src, des, direct, pManager, info);
		}
	}
	return false;
}

void MultiShootSkill::update() {
	Skill::update();
	if(m_update_time > m_release_time) {
		if(m_current_num < m_effect_num) {
			m_current_rate += UPDATETIME;
		} else {
			m_current_rate = 0;
		}	
	}
}

bool MultiShootSkill::do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info) {
	if(direct.x == 0) {direct.x = 1;};
	if(direct.y == 0) {direct.y = 1;};
	if(m_recs.find(direct) != m_recs.end() && m_offsets.find(direct) != m_offsets.end()) {
		CSMonsterEffectInfo * effect_info = info->add_effects();
		TileWorld<>::Rectangle rec = m_recs[direct];
		TileWorld<>::Point offset = m_offsets[direct];
		{
			TileWorld<>::Point start_pos = src->m_pos;
			start_pos += offset;
			TileWorld<>::Point end_pos 	= des->space().center();		
			if(start_pos == end_pos) {
					end_pos.y += m_dead_len; 
			}
			float dis = start_pos.distance(end_pos);
			if(dis < m_dead_len) {
				end_pos -= start_pos;
				end_pos *= (m_dead_len/dis);
				end_pos += start_pos;
			}
			DEBUG_LOG("MOVE EFFECT START:(%d %d) DIRECT(%d %d) ",start_pos.x,start_pos.y, direct.x, direct.y);
			MoveEffectRec* effect= new MoveEffectRec(rec, start_pos, end_pos, src->m_fightprop, m_skill_type, m_damage, m_speed, m_effect_time, m_attack_number, m_rage_point);
			pManager->add_effect(effect);
			if(m_break)
				effect->link_object(src);
/*template code*/
		    effect_info->set_effect_animation(m_effect_animation);
			effect_info->set_type(MULT_SHOOT_ATTACK);
			CSPoint* start_point = effect_info->mutable_start_pos();
			start_point->set_xpos(start_pos.x);
			start_point->set_ypos(start_pos.y);
			CSPoint* end_point = effect_info->mutable_end_pos();
			end_point->set_xpos(end_pos.x);
			end_point->set_ypos(end_pos.y);
			effect_info -> set_skill_speed(m_speed);
			effect_info -> set_delay_time(0);
			effect_info -> set_effect_id(m_effect_id);
			m_current_rate = 0;
			m_current_num ++;	
			return true;
		}
	}
	return false;
}
	
RailgunSkill::RailgunSkill(uint32_t id) :
	Skill(id),
	m_target_move(false)
{
}
	
bool RailgunSkill::try_action(Entity* src, Entity* des) {
	Skill::try_action(src, des);
	if(src->m_pos == m_point1 || src->m_pos == m_point2) {
		return true;
	}
	return false;
}
	
bool RailgunSkill::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(direct.x == 0) {direct.x = 1;};
	if(direct.y == 0) {direct.y = 1;};
	if(m_recs.find(direct) != m_recs.end()) {
		TileWorld<>::Rectangle rec = m_recs[direct];
		TileWorld<>::Point start_pos = src->m_pos;
		TileWorld<>::Point end_pos;		
		if(start_pos == m_point1) {
			end_pos = m_point2;
		} else {
			end_pos = m_point1;
		}
		ObjectFixMoveEffect* effect= new ObjectFixMoveEffect(rec, start_pos, end_pos, src->m_fightprop, m_skill_type, m_damage, m_speed, m_effect_time, m_attack_rate, m_attack_number, m_rage_point);
		if(m_break)
			effect->link_object(src);
		pManager->add_effect(effect);
/*template code*/
		info->set_type(4);
		if(direct.y > 0) {
			info->set_action_id(m_up_animation);
			info -> set_effect_id(m_link_animation_up);
		} else {
			info->set_action_id(m_down_animation);
			info -> set_effect_id(m_link_animation_down);
		}
		CSPoint* start_point = info->mutable_start_pos();
		start_point->set_xpos(start_pos.x);
		start_point->set_ypos(start_pos.y);
		CSPoint* end_point = info->mutable_end_pos();
		end_point->set_xpos(end_pos.x);
		end_point->set_ypos(end_pos.y);
		info -> set_skill_speed(m_speed);
		info -> set_delay_time(m_effect_time);
		DEBUG_LOG("MOVE EFFECT START:(%d %d) END:(%d %d) DIRECT(%d %d) DELAY:%d",start_pos.x,start_pos.y, end_pos.x, end_pos.y, direct.x, direct.y, m_effect_time);
		src->set_idle(m_cool_down);	
		m_target_move = false;
		return Skill::do_action(src, des, direct, pManager, info);
	}
	return false;
}
	
TileWorld<>::Point RailgunSkill::get_point() {
	base::AverageGen<TileWorld<>::Point> gen;
	gen.push(m_point1);
	gen.push(m_point2);
	return gen.get_one();	
}
	
SkillControl::SkillControl(TileWorld<>* tileworld) :
	m_id(0),
	m_tileworld(tileworld)
{
}
	
SkillControl::~SkillControl() {
	SkillMap::iterator itr = m_skill_map.begin();
	for(; itr!= m_skill_map.end(); ++itr) {
		delete itr->second;
	}
	itr = m_buff_skill_map.begin();
	for(; itr!= m_buff_skill_map.end(); ++itr) {
		delete itr->second;
	}	
}
	
void SkillControl::generate_skills(const Monster_t& info) {
	for(size_t i = 0; i < info.skills.size(); i++) {
		add_skill(info.skills[i]);
	}
	if(!m_skill_map.empty()) {
		m_id = m_skill_map.begin()->first;
	} else if(!m_buff_skill_map.empty()) {
		m_id = m_buff_skill_map.begin()->first;
	} else {
		ERROR_RETURN_VOID("ASSERT generate_skills!");
	}
}
	
void SkillControl::add_skill(uint32_t id) {
	Skill_t* skill_info = g_skill_config->get_skill_info(id);
	if(skill_info) {
		switch (skill_info->type) {
			case MELEE_ATTACK:
			{	
				MeleeSkill* pskill = new MeleeSkill(id);
				pskill->m_recs= skill_info->m_recs; 
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_rate	= skill_info->attack_rate;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case SHOOT_ATTACK:
			{
				ShootSkill* pskill = new ShootSkill(id, m_tileworld);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_offsets= skill_info->m_offsets;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_speed = skill_info->rate;
				pskill->m_dead_len = skill_info->remote_attack_range;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case TARGET_ATTACK:
			{
				TargetSkill* pskill = new TargetSkill(id);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_release_time = (skill_info->release_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_dead_len = skill_info->remote_attack_range;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case MULT_REMOTE:
			{
				TargetMultiSkill* pskill = new TargetMultiSkill(id);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_release_time = (skill_info->release_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = pskill->m_effect_time + skill_info->continue_time;
				pskill->m_dead_len = skill_info->remote_attack_range;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_rate	= skill_info->attack_rate;
				pskill->m_effect_num 	= skill_info->continue_time/skill_info->attack_rate;
				pskill->m_camera_shake_time = skill_info->camera_shake_time;
				pskill->m_link_effect_time = (skill_info->link_effect_time / UPDATETIME) * UPDATETIME;
				pskill->m_link_effect_end  = skill_info->link_dead_time;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case OBJECT_FIX_MOVE:
			{
				ObjectFixMoveSkill* pskill = new ObjectFixMoveSkill(id, m_tileworld);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_offsets= skill_info->m_offsets;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_speed = skill_info->move_speed;
				pskill->m_dead_len = skill_info->displacement;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_link_animation_up = skill_info->link_animation_up;
				pskill->m_link_animation_down = skill_info->link_animation_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case SUMMON:
			{	
				SummonSkill* pskill = new SummonSkill(id);
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_group			= skill_info->group;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;			
			case SELF_BUF:
			{	
				BuffSkill* pskill = new BuffSkill(id);
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_continue_time	 = skill_info->continue_time;
				pskill->prop = skill_info->fight_prop;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_buff_skill_map[id] = pskill;
			}
			break;
			case FOLLOW_ATTACK:
			{	
				ObjectMoveSkill* pskill = new ObjectMoveSkill(id);
				pskill->m_recs= skill_info->m_recs; 
				pskill->m_effect_time = (skill_info->move_start_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_link_animation_up = skill_info->link_animation_up;
				pskill->m_link_animation_down = skill_info->link_animation_down;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_rate	= skill_info->attack_rate;
				pskill->m_continue_time	 = skill_info->continue_time;
				pskill->m_speed = skill_info->move_speed;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case TELEPORT:
			{	
				TeleportSkill* pskill = new TeleportSkill(id, m_tileworld);
				pskill->m_release_time = (skill_info->release_time / UPDATETIME)* UPDATETIME ;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_link_animation_up = skill_info->link_animation_up;
				pskill->m_link_animation_down = skill_info->link_animation_down;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_dead_len = skill_info->displacement;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case JUMP_ATTACK:
			{	
				JumpSkill* pskill = new JumpSkill(id);
				pskill->m_recs= skill_info->m_recs; 
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_link_animation_up = skill_info->link_animation_up;
				pskill->m_link_animation_down = skill_info->link_animation_down;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_dead_len 		= skill_info->remote_attack_range;
				pskill->m_release_time	 = skill_info->move_start_time;
				pskill->m_link_effect_time = (skill_info->link_effect_time + skill_info->move_start_time) / UPDATETIME * UPDATETIME;
				pskill->m_link_effect_end  = skill_info->link_dead_time + skill_info->move_start_time;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case STEALTH:
			{	
				StealthSkill* pskill = new StealthSkill(id, m_tileworld);
				pskill->m_release_time = (skill_info->release_time / UPDATETIME)* UPDATETIME ;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_link_animation_up = skill_info->link_animation_up;
				pskill->m_link_animation_down = skill_info->link_animation_down;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_dead_len = skill_info->displacement;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case MULT_SHOOT_ATTACK:
			{
				MultiShootSkill* pskill = new MultiShootSkill(id);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_offsets= skill_info->m_offsets;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_speed = skill_info->rate;
				pskill->m_dead_len = skill_info->remote_attack_range;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case RAIL_GUN:
			{
				RailgunSkill* pskill = new RailgunSkill(id);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_offsets= skill_info->m_offsets;
				pskill->m_effect_time = (skill_info->move_start_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_speed = skill_info->move_speed;
				pskill->m_dead_len = skill_info->displacement;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_link_animation_up = skill_info->link_animation_up;
				pskill->m_link_animation_down = skill_info->link_animation_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_attack_rate	= skill_info->attack_rate;
				pskill->m_point1		= skill_info->point1;
				pskill->m_point2		= skill_info->point2;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case NOTARGET_MELEE_ATTACK:
			{	
				NoTargetMeleeSkill* pskill = new NoTargetMeleeSkill(id);
				pskill->m_recs= skill_info->m_recs; 
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_rate	= skill_info->attack_rate;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case NOTARGET_SHOOT_ATTACK:
			{
				NoTargetShootSkill* pskill = new NoTargetShootSkill(id, m_tileworld);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_offsets= skill_info->m_offsets;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_speed = skill_info->rate;
				pskill->m_dead_len = skill_info->remote_attack_range;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_direct		= skill_info->effect_direction;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
			case NOTARGET_ATTACK:
			{
				NoTargetSkill* pskill = new NoTargetSkill(id);
				pskill->m_recs= skill_info->m_recs;
				pskill->m_effect_time = (skill_info->effect_time / UPDATETIME)* UPDATETIME ;
				pskill->m_release_time = (skill_info->release_time / UPDATETIME)* UPDATETIME ;
				pskill->m_dead_time = skill_info->dead_time;
				pskill->m_dead_len = skill_info->remote_attack_range;
				pskill->m_cool_down = skill_info->cool_down;
				pskill->m_effect_id = skill_info->effect_id;
				pskill->m_up_animation = skill_info->up_animation;
				pskill->m_down_animation = skill_info->down_animation;
				pskill->m_skill_type 	= skill_info->skill_type;
				pskill->m_damage 		= skill_info->damage;
				pskill->m_attack_number	= skill_info->can_attack_number;
				pskill->m_attack_rate	= skill_info->attack_rate;
				pskill->m_rage_point	= skill_info->rage_point;
				pskill->m_des	= skill_info->effect_fixpoint;
				pskill->m_effect_animation	= skill_info->effect_animation;
				m_skill_map[id] = pskill;
			}
			break;
		}
	}
}
	
bool SkillControl::available() {
	if(m_id) {
		SkillMap::iterator itr = m_skill_map.find(m_id);
		if(itr != m_skill_map.end()) return itr->second->available();
		itr = m_buff_skill_map.find(m_id);
		if(itr != m_buff_skill_map.end()) return itr->second->available();
	}
	return false;
}
	
void SkillControl::update() {
	SkillMap::iterator itr = m_skill_map.begin();
	for(; itr!=m_skill_map.end(); ++itr) {
		itr->second->update();	
	}
	itr = m_buff_skill_map.begin();
	for(; itr!=m_buff_skill_map.end(); ++itr) {
		itr->second->update();	
	}
}
	
bool SkillControl::try_action( Entity* src, Entity* des ) {
	if(m_id && m_skill_map.find(m_id)!= m_skill_map.end()) {
		return m_skill_map[m_id]->try_action(src, des);
	}
	return false;
}
	
bool SkillControl::do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info) {
	if(m_id && m_skill_map.find(m_id)!= m_skill_map.end()) {
		return m_skill_map[m_id]->do_action(src, des, direct, pManager, info);
	} 
	return false;
}
	
bool SkillControl::do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info) {
	if(m_id && m_skill_map.find(m_id)!= m_skill_map.end()) {
        return m_skill_map[m_id]->do_effect(src, des, direct, pManager, info);
	}
	return false;
}
	
bool SkillControl::do_buff(Entity* src, Entity* des, CSBuffEffectRsp* info) {
	if(m_id && m_buff_skill_map.find(m_id) != m_buff_skill_map.end()) {
		return m_buff_skill_map[m_id]->do_buff(src, des, info);
	}
	return false;
}
	
bool SkillControl::has_buff() {
	if(m_id && m_buff_skill_map.find(m_id) != m_buff_skill_map.end()) {
		return true;
	}
	return false;
}
	
bool SkillControl::do_buff(Entity* src, base::Manager<Monster>* des, CSBuffEffectRsp* info) {
	if(m_id && m_buff_skill_map.find(m_id) != m_buff_skill_map.end()) {
		return m_buff_skill_map[m_id]->do_buff(src, des, info);
	}
	return false;
}
	
bool SkillControl::exchange_current(uint32_t id) {
	if(m_skill_map.find(id)!=m_skill_map.end() || m_buff_skill_map.find(id) != m_buff_skill_map.end() ) {
		DEBUG_LOG("EXCHANGE SKILL:LAST:%u, CURRENT:%u ", m_id, id);
		m_id = id;
		return true;
	} 
	return false;
}
	
uint32_t SkillControl::current_skill_id() {
	return m_id;
}
	
Skill* SkillControl::get_current_skill() {
	if(m_id) {
		SkillMap::iterator itr = m_skill_map.find(m_id);
		if(itr != m_skill_map.end()) return itr->second;
		itr = m_buff_skill_map.find(m_id);
		if(itr != m_buff_skill_map.end()) return itr->second;
	}
	return NULL;
}
	
