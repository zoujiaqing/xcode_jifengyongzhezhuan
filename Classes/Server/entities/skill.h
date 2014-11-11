#ifndef SKILL_HPP_
#define SKILL_HPP_

#include "object.h"
#include <vector>
#include "../config/battleconfig.h"
#include "../base/DeferEvent.hpp"

#define UPDATETIME (200)

class Skill;

class Monster;

class Battlefield;

enum Status {
	IDLE = 1,
	EFFECT = 2,
	DEAD =3,
};

class EffectSpace: public sigslot::has_slots<> {
public:
	EffectSpace(FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t effect_time, uint32_t dead_time, uint32_t attack_num, uint32_t rage_point);
	virtual ~EffectSpace(){};
	virtual bool effect(Entity* des, CSMonsterHitRsp* rsp)=0;
	virtual bool update();
	Status status() {
		return m_status;
	};

	void on_under_attack_back(Entity*);

	virtual void link_object(Entity* pObject);

protected:
	Status m_status;
	uint32_t m_effect_time;
	uint32_t m_dead_time;
	uint32_t m_update_time;
//	Entity * m_attacker;
	FightProp m_attacker_prop;
	uint32_t m_skill_type;
	uint32_t m_damage;
	uint32_t m_attack_number;
	uint32_t m_rage_point;
};

class SummonEffect:public sigslot::has_slots<> {
public:
	SummonEffect(MapMonsterGroup_t& group, uint32_t effect_time);
	bool update();
	void effect_battle(Battlefield* battle);
	Status status() {
		return m_status;
	};
protected:
	Status m_status;
	uint32_t m_effect_time;
	uint32_t m_update_time;
	MapMonsterGroup_t m_group;
	bool m_effect;
};

class EffectRec:public EffectSpace {
public:
	EffectRec(TileWorld<>::Rectangle rec, FightProp attacker_prop, uint32_t skill_type, uint32_t damage,  uint32_t effect_time, uint32_t dead_time, uint32_t effect_interval, uint32_t attack_num, uint32_t rage_point);
	bool effect(Entity* des, CSMonsterHitRsp* rsp);
	bool update();
	void link_object_dead(Entity* pObject);
	void on_object_dead(base::Object<player_key_t>*);
private:
typedef std::set<Entity*> EffectedList;
	TileWorld<>::Rectangle m_rec;
	EffectedList m_effected_list;
	uint32_t m_effect_update;
	uint32_t m_effect_interval;
};

class MoveEffectRec: public EffectSpace {
public:
	MoveEffectRec(TileWorld<>::Rectangle rec, TileWorld<>::Point src, TileWorld<>::Point des, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t speed, uint32_t effect_time, uint32_t attack_num, uint32_t rage_point);	
	~MoveEffectRec();	
	bool effect(Entity* des, CSMonsterHitRsp* rsp);
	bool update();
protected:
	typedef std::set<Entity*> EffectedList;
	TileWorld<>::Rectangle m_rec;
	TileWorld<>::Point m_src;
	TileWorld<>::Point m_des;
	EffectedList m_effected_list;	
	uint32_t m_speed;
	uint32_t m_accumulate;
	bool m_last_effect;
};

class ObjectFixMoveEffect: public MoveEffectRec 
{
public:
	ObjectFixMoveEffect(TileWorld<>::Rectangle rec, TileWorld<>::Point src, TileWorld<>::Point des, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t speed, uint32_t effect_time, uint32_t effect_interval, uint32_t attack_num, uint32_t rage_point, uint32_t move_time=0);	
	~ObjectFixMoveEffect();
	void link_object(Entity* object);
	void link_object_leave(Entity* object);
	void on_object_leave(base::Object<player_key_t>* pObject);
	void on_object_stop(Entity* pObject);
	bool update();
private:
	Entity* m_object; 
	TileWorld<>::Point end_des;
	uint32_t m_effect_update;
	uint32_t m_effect_interval;
	uint32_t m_move_time;
};

class ObjectMoveEffect: public EffectSpace {
public:
	ObjectMoveEffect(TileWorld<>::Rectangle rec, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t speed, uint32_t effect_time, uint32_t dead_time, uint32_t effect_interval, uint32_t attack_num, uint32_t rage_point);
	bool effect(Entity* des, CSMonsterHitRsp* rsp);
	void link_object(Entity* object);
	void on_object_leave(base::Object<player_key_t>* pObject);
	bool update();
protected:
	typedef std::set<Entity*> EffectedList;
	TileWorld<>::Rectangle m_rec;
	EffectedList m_effected_list;
	uint32_t m_effect_update;
	uint32_t m_effect_interval;
	uint32_t m_speed;
	Entity* m_object; 
};

class ObjectJumpEffect: public EffectSpace {
public:
	ObjectJumpEffect(TileWorld<>::Rectangle rec, FightProp attacker_prop, uint32_t skill_type, uint32_t damage, uint32_t effect_time, uint32_t dead_time, uint32_t attack_num, uint32_t rage_point);
	bool effect(Entity* des, CSMonsterHitRsp* rsp);
	void link_object(Entity* object);
	void on_object_leave(base::Object<player_key_t>* pObject);
protected:
	typedef std::set<Entity*> EffectedList;
	TileWorld<>::Rectangle m_rec;
	EffectedList m_effected_list;
	Entity* m_object; 
};

class EffectManager {
public:
	~EffectManager();
	void update();	
	void effect(Entity* des, CSMonsterHitRsp* rsp);	
	void effect_battle(Battlefield* battle);	
	void add_effect(EffectSpace*);
	void add_effect(SummonEffect*);
private:
typedef	std::vector<EffectSpace*> EffectList;
	EffectList m_effect_list;
typedef std::vector<SummonEffect*> SummonEffectList;
	SummonEffectList m_summon_effect_list;
};

class Skill: public sigslot::has_slots<> {
public:
	Skill(uint32_t id);
	uint32_t get_id();
	bool available();
	uint32_t get_effect_time(){return m_effect_time;};
	virtual uint32_t speed(){return 0;};
	virtual void update();
	virtual bool try_action(Entity* src, Entity* des){return false;};
	virtual bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	virtual bool do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info){return false;};
	virtual bool do_buff(Entity* src, base::Manager<Monster>* des, CSBuffEffectRsp* info) {return false;};
	virtual bool do_buff(Entity* src, Entity* des, CSBuffEffectRsp* info) {return false;};
	void on_monster_under_attack_back(Entity* src);
	static Skill* generate_skill(uint32_t role_id, uint32_t id);
friend class SkillControl;
protected:
	uint32_t m_id;
	bool m_available;
	uint32_t m_effect_time;
	uint32_t m_dead_time;
	uint32_t m_update_time;
	uint32_t m_cool_down;
	
	uint32_t m_up_animation;
	uint32_t m_down_animation;
    uint32_t m_effect_animation;

	uint32_t m_rage_point;

	uint32_t m_effect_type;
	uint32_t m_attack;
	uint32_t m_hit;

	uint32_t m_damage;
	uint32_t m_skill_type;

	uint32_t m_attack_number;

	bool m_break;
};

class SkillControl;

class MeleeSkill: public Skill {
public:
	MeleeSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_attack_rate;
	RecMap m_recs;
};

class NoTargetMeleeSkill: public Skill {
public:
	NoTargetMeleeSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_attack_rate;
	RecMap m_recs;
};

class ShootSkill: public Skill {
public:
	uint32_t speed(){return m_speed;};
	ShootSkill(uint32_t id, TileWorld<>* tileworld);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_speed;
	uint32_t m_dead_len;
	RecMap m_recs;
	OffsetMap m_offsets;
	uint32_t m_effect_id;
	TileWorld<>* m_tileworld;
};

class NoTargetShootSkill: public Skill {
public:
	uint32_t speed(){return m_speed;};
	NoTargetShootSkill(uint32_t id, TileWorld<>* tileworld);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_speed;
	uint32_t m_dead_len;
	RecMap m_recs;
	OffsetMap m_offsets;
	uint32_t m_effect_id;
	TileWorld<>* m_tileworld;
	TileWorld<>::Point m_direct;
};

class ObjectFixMoveSkill: public Skill {
public:
	ObjectFixMoveSkill(uint32_t id, TileWorld<>* tileworld);
	uint32_t speed(){return m_speed;};
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_speed;
	uint32_t m_dead_len;
	RecMap m_recs;
	OffsetMap m_offsets;
	uint32_t m_link_animation_up;
	uint32_t m_link_animation_down;
	TileWorld<>* m_tileworld;
};

class ObjectMoveSkill: public Skill {
public:
	ObjectMoveSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	bool do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info);
	void update();
friend class SkillControl;
friend class Skill;
private:
	RecMap m_recs;
	uint32_t m_speed;
	uint32_t m_link_animation_up;
	uint32_t m_link_animation_down;
	uint32_t m_attack_rate;
	uint32_t m_current_rate;
	uint32_t m_continue_time;
}; 

class TargetSkill: public Skill {
public:
	TargetSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_release_time;
	uint32_t m_dead_len;
	uint32_t m_effect_id;
	RecMap m_recs;
};

class NoTargetSkill: public Skill {
public:
	NoTargetSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_release_time;
	uint32_t m_dead_len;
	uint32_t m_effect_id;
	uint32_t m_attack_rate;
	RecMap m_recs;
	TileWorld<>::Point m_des;	
};

class TargetMultiSkill: public Skill {
public:
	TargetMultiSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	bool do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info);
	void update();
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_release_time;
	uint32_t m_dead_len;
	uint32_t m_effect_id;
	RecMap m_recs;
	uint32_t m_effect_num;
	uint32_t m_attack_rate;
	uint32_t m_current_num;
	uint32_t m_current_rate;
	std::vector<uint32_t> m_camera_shake_time;
	uint32_t m_link_effect_time;
	uint32_t m_link_effect_end;
};

class SummonSkill: public Skill {
public:
	SummonSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	MapMonsterGroup_t m_group;
};

class BuffSkill: public Skill {
public:
	BuffSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_buff(Entity* src, base::Manager<Monster>* des, CSBuffEffectRsp* info);
friend class SkillControl;
friend class Skill;
	uint32_t m_continue_time;
	uint32_t m_effect_id;
	FightProp  prop;	
};

class TeleportSkill: public Skill {
public:
	TeleportSkill(uint32_t id, TileWorld<>* tileworld);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_link_animation_up;
	uint32_t m_link_animation_down;
	uint32_t m_release_time;
	uint32_t m_dead_len;
	TileWorld<>* m_tileworld;
};

class StealthSkill: public Skill {
public:
	StealthSkill(uint32_t id, TileWorld<>* tileworld);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_link_animation_up;
	uint32_t m_link_animation_down;
	uint32_t m_release_time;
	uint32_t m_dead_len;
	TileWorld<>* m_tileworld;
};

class JumpSkill: public Skill {
public:
	JumpSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	bool do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info);
	void on_target_del(base::Object<player_key_t>*);
	void on_target_dead(Entity*);
friend class SkillControl;
friend class Skill;
private:
	RecMap m_recs;
	uint32_t m_link_animation_up;
	uint32_t m_link_animation_down;
	uint32_t m_release_time;
	uint32_t m_dead_len;
	uint32_t m_link_effect_time;
	uint32_t m_link_effect_end;
	Entity* m_target;
};

class MultiShootSkill: public Skill {
public:
	uint32_t speed(){return m_speed;};
	MultiShootSkill(uint32_t id);
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	bool do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info);
	void update();
friend class SkillControl;
friend class Skill;
private:
	uint32_t m_release_time;
	uint32_t m_speed;
	uint32_t m_dead_len;
	RecMap m_recs;
	OffsetMap m_offsets;
	uint32_t m_effect_id;

	uint32_t m_effect_num;
	uint32_t m_attack_rate;
	uint32_t m_current_num;
	uint32_t m_current_rate;
	uint32_t m_link_effect_time;
	uint32_t m_link_effect_end;
};

class RailgunSkill: public Skill {
public:
	RailgunSkill(uint32_t id);
	uint32_t speed(){return m_speed;};
	bool try_action(Entity* src, Entity* des);
	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	TileWorld<>::Point get_point();
	bool target_move(){return m_target_move;};
	void set_target_move(){m_target_move=true;};
friend class SkillControl;
friend class Skill;
private:
	bool m_target_move;
	uint32_t m_speed;
	uint32_t m_dead_len;
	RecMap m_recs;
	OffsetMap m_offsets;
	uint32_t m_link_animation_up;
	uint32_t m_link_animation_down;
	uint32_t m_attack_rate;
	TileWorld<>::Point m_point1;
	TileWorld<>::Point m_point2;
};

class SkillControl {
public:
	SkillControl(TileWorld<>* tileworld);
	
	~SkillControl();

	void generate_skills(const Monster_t& info);

	void add_skill(uint32_t id);	
	
	bool available();

	void update();

	bool try_action(Entity* src, Entity* des);

	bool do_action(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterAttackInfo* info);
	
	bool do_effect(Entity* src, Entity* des, TileWorld<>::Point& direct, EffectManager* pManager, CSMonsterEffectRsp* info);
	
	bool do_buff(Entity* src, Entity* des, CSBuffEffectRsp* info);
	
	bool has_buff();
	
	bool do_buff(Entity* src, base::Manager<Monster>* des, CSBuffEffectRsp* info);

	bool exchange_current(uint32_t id);
	
	uint32_t current_skill_id();

	Skill* get_current_skill();

private:
	typedef std::map<uint32_t, Skill*> SkillMap;
	SkillMap m_skill_map;
	SkillMap m_buff_skill_map;
	uint32_t m_id;
	TileWorld<>* m_tileworld;
};

#endif
