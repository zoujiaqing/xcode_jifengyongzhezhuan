#ifndef MONSTER_H_
#define MONSTER_H_
#include "../base/Manager.hpp"
#include "../base/ProbabilityGen.hpp"
#include "../proto/cs_battle.pb.h"
#include "object.h"
#include "../config/battleconfig.h"
#include "skill.h"

using namespace taomee;
using namespace base;

#define HATRED_FREQUENCY (4)
#define HATRED_BASE		(10)

class Hatred:public sigslot::has_slots<> {
public:
	Hatred();	

	void add(Entity* entity, uint32_t value=HATRED_BASE);
	
	void on_player_leave(base::Object<player_key_t>* pPlayer);

	bool exist(Entity* entity);

	bool empty();

	Entity* get_current(bool update=true);	

private:
	std::map<Entity*, uint32_t> m_hatred_map;
	uint32_t m_cool_time;
	Entity* m_current;
};

class MonsterManager;

class Monster : public Entity, public sigslot::has_slots<> {
public:
	Monster(UserID_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager);
	
	~Monster();

	void pack_battle_info(MonsterBattleInfo* info);
	
	virtual	void init_info(const Monster_t& info, const MapMonster_t& map_info);

	enum MonsterStat {
		PATROL = 1,
		LOCKING = 2,
		IDLE = 3,
		CHASE = 4,
		BUFF = 5,	
	};
	
	MonsterStat get_stat();
		
	void dead();
	
	bool patrol();	
	
	virtual bool locking(CSMonsterAttackRsp* attack_rsp);	

	bool try_locking(Entity* pEntity);

	bool walk_to(Entity* pEntity);

	bool walk_to(TileWorld<>::Point pos);

	void update(CSMonsterMoveRsp* move_rsp, CSMonsterAttackRsp* attack_rsp, CSMonsterEffectRsp* effect_rsp);

	void on_player_leave(base::Object<player_key_t>* pPlayer);
	
	void on_player_del(Entity* pPlayer);

	void under_attack(Entity* pEntity, uint32_t blood);

	void under_attack_back(const TileWorld<>::Point& start_point, const TileWorld<>::Point& point);

	bool stop();

	void pack_move(MonsterMoveValue* move_msg);

	uint32_t get_type_id() {return type_id_;}
	
	uint32_t get_level() {return level_;}
	
	uint32_t is_boss() {return boss_;}
	
	bool is_elite() {return elite_;}
	
    bool is_friend() {return friend_;}
    
    bool is_city() {return city_;}

	uint32_t get_hp_line() {return hp_line_;};
	
	uint32_t get_speed() {return m_prop.speed;}

	void set_speed(uint32_t speed){m_prop.speed = speed;};

	Entity* get_current_target();	

	virtual bool attack_check(Entity* des);
	
	void do_attack(Entity* pEntity, CSMonsterAttackRsp* attack_msg );
	
	void do_effect(CSMonsterEffectRsp* effect_msg );

	void do_buff(base::Manager<Monster>* monsters, CSBuffEffectRsp* buff_rsp);

	void set_idle(uint32_t time);

protected:

	Hatred m_hatred;

//	Skill* m_current_pskill;
	SkillControl m_skill_control;

	MonsterStat stat_;

	uint32_t m_accumulate;
public:
	sigslot::signal1<const Message&> sig_send_msg;

private:
	
	/*attributes*/
	
	uint32_t type_id_;

	uint32_t level_;
	
	bool boss_;

	bool elite_;

    bool friend_;

    bool city_;

	uint32_t hp_line_;
	
	TileWorld<>::RandomRangePath m_patrol;	

	TileWorld<>* m_pworld;	

	TileWorld<>::Points m_road;	

	TileWorld<>::Point m_target_point;
	
	EffectManager* m_effect_manager;

	uint32_t m_idle_time;

friend class MonsterManager;
};

class MonsterManager: public base::Manager<Monster> {
public:
	MonsterManager();
	
	~MonsterManager();

	bool empty();

    bool city_crash();

	Monster* add_monster(MapMonster_t * mapmonster, TileWorld<>* world, EffectManager* effect_manager);

	int update(CSMonsterMoveRsp* move_rsp, CSMonsterAttackRsp* attack_rsp, CSMonsterEffectRsp* effect_rsp);

	void do_buff(CSBuffEffectRsp* buff_rsp);

	void pack_move(CSMonsterMoveRsp *rsp);
	
	void try_locking(Entity* pEntity);

	void on_boss_dead(base::Object<player_key_t>* boss);

    void on_city_crash(Entity* city);

	void on_send_msg(const Message& msg);

	sigslot::signal1<const Message&> sig_send_msg;

private:
	uint32_t m_increase_key;
	
	uint32_t m_boss_num;
	
	bool m_boss_dead;

    bool m_city_crash;
};

#endif
