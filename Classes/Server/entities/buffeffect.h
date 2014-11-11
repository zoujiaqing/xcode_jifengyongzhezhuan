#ifndef BUFFEFFECT_H_
#define BUFFEFFECT_H_
#include <set>
#include "../base/EventableObject.hpp"
#include "../base/Singleton.hpp"
#include <stdint.h>
#include "object.h"
#include "../config/battleconfig.h"

class BuffEffect: public base::EventableObject {
public:
	BuffEffect(uint32_t dead_time, uint32_t id);

	virtual void do_buff(Entity* entity, const FightProp& m_base_fight_prop) =0;
	
	virtual void undo_buff(Entity* entity, const FightProp& m_base_fight_prop) =0;

	int del_self();
	
	uint32_t get_id();

	sigslot::signal1<BuffEffect*> sig_del_self;
protected:
	uint32_t m_id;
};

class InvincibleBuff : public BuffEffect {
public:
	InvincibleBuff(uint32_t dead_time, uint32_t id);
	
	void do_buff(Entity* entity, const FightProp& m_base_fight_prop);
	
	void undo_buff(Entity* entity, const FightProp& m_base_fight_prop);
};

class StealthBuff : public BuffEffect {
public:
	StealthBuff(uint32_t dead_time, uint32_t id);
	
	void do_buff(Entity* entity, const FightProp& m_base_fight_prop);
	
	void undo_buff(Entity* entity, const FightProp& m_base_fight_prop);
};

class SpeedBuff : public BuffEffect {
public:
	SpeedBuff(uint32_t dead_time, uint32_t speed, uint32_t id);
	
	void do_buff(Entity* entity, const FightProp& m_base_fight_prop);

	void undo_buff(Entity* entity, const FightProp& m_base_fight_prop);
private:
	uint32_t m_speed;
};

class AttrBuff: public BuffEffect {
public:
	AttrBuff(uint32_t dead_time, SkillPart_t& skill_part, uint32_t id);

	void do_buff(Entity* entity, const FightProp& m_base_fight_prop);

	void undo_buff(Entity* entity, const FightProp& m_base_fight_prop);

private:
	SkillPart_t m_skill_part;
};

//觉得有点问题
class AttrPerBuff: public BuffEffect {
public:
	AttrPerBuff(uint32_t dead_time, uint32_t id,int32_t percent);

	void do_buff(Entity* entity, const FightProp& m_base_fight_prop);
	
	void undo_buff(Entity* entity, const FightProp& m_base_fight_prop);

private:
	int32_t	m_percent;

};

class SkillBuff: public BuffEffect {
public:
	SkillBuff(uint32_t dead_time, const FightProp& prop, uint32_t id);
	
	void do_buff(Entity* entity, const FightProp& m_base_fight_prop);
	
	void undo_buff(Entity* entity, const FightProp& m_base_fight_prop);
private:
	FightProp m_prop;
};

class BuffEffectManager : public sigslot::has_slots<> {
public:
	BuffEffectManager(Entity* entity);
	
	BuffEffectManager(const BuffEffectManager& manager);
	
	const BuffEffectManager& operator=(const BuffEffectManager& manager);

	virtual ~BuffEffectManager();
	
	void load_prop(Entity* entity);

	void add_buff(BuffEffect* pEffect);
	
	void add_hero_buff(uint32_t id, uint32_t level);

	void add_hero_buff(uint32_t id, uint32_t level, int32_t percent);
	
	void add_fairy_skill_buff(uint32_t id,uint32_t fairy_skill_attack);

	void redo_buff();

	void on_effect_del_self(BuffEffect* pEffect);

	void clear();

	bool exist_buff(uint32_t id);

	sigslot::signal1<Entity*> sig_undo_buff;
protected:
	Entity* m_entity;
	FightProp* m_base_fight_prop;	
	typedef std::set<BuffEffect*> BuffList;
	BuffList m_buffs;
};

#endif
