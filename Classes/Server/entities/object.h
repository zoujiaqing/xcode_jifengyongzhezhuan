#ifndef RU_OBJECT_H_
#define RU_OBJECT_H_
#include <stdint.h>
#include "../common/common.h"
#include "../base/Manager.hpp"
#ifdef FRONT
#include "../base/EncodeValue.hpp"
#endif
#include "../battlefields/tileworld.hpp"
#include "../proto/cs_battle.pb.h"

#define PLAYER_MIN 10000

#define MONSTER_MIN 2000

#define MONSTER_MAX 1000


#define PLAYER_DEFAULT_GROUP (1)
#define MONSTER_DEFAULT_GROUP (1<<2)

using namespace taomee;
using namespace google::protobuf;

#ifndef FRONT
typedef uint32_t CoreType; 
#else
typedef EncodeValue<uint32_t> CoreType;
#endif

class BuffEffectManager;

enum PVPGroup {
	PVPGroup1 = 1,
	PVPGroup2 = 2,
};

enum AttackQualityType {
	ATTACK_PHYSICAL	= 1,
	ATTACK_MAGIC	= 2,
	ATTACK_SKILL	= 3,
};

enum AttackValueType {
	ATTACK_DODGE	= 1,
	ATTACK_PARRY	= 2,
	ATTACK_CRIT		= 3,
	ATTACK_NORMAL	= 4,
};

/**
 * @brief 壨ҡ顢䡢ȵȣĻ
 */
class Object {
public:
	virtual ~Object() { }
	virtual void update() = 0;

private:
};

class FightProp {
public:
	FightProp() :
		physical_attack(0),
		magic_attack(0),
		skill_attack(0),
		physical_defence(0), 
		magic_defence(0),
		skill_defence(0),
		health_point(0),
		accurate(0),
		dodge(0),
		wreck(0),
		parry(0),
		critical_strike(0),
		tenacity(0),
		slay(0),
		speed(0),
		courage(0),
		charm(0),
		trick(0),
		proficiency(0),
		vision(0),
		invincible(false),
		stealth(false),
		world_boss(false) {
	}
	CoreType physical_attack; 
	CoreType magic_attack; 
	CoreType skill_attack; 
	CoreType physical_defence; 
	CoreType magic_defence; 
	CoreType skill_defence; 
	CoreType health_point; /*it mean the top hp*/ 
	CoreType accurate;
	CoreType dodge;
	CoreType wreck;
	CoreType parry;
	CoreType critical_strike;
	CoreType tenacity;
	CoreType slay;
	CoreType speed;
	CoreType courage;
	CoreType charm;
	CoreType trick;
	CoreType proficiency;

	uint32_t vision;
	bool invincible;
	bool stealth;
	bool world_boss;
};

class Prop {
public:
	Prop() :
		health_point(0),
		experience(0),
		speed(0) {
	}
	CoreType health_point;
	uint32_t experience;
	CoreType speed;
	TileWorld<>::Rectangle space;
};

class Entity: public base::Object<player_key_t> {
public:
	Entity(player_key_t id=player_key_t(0, 0, 0), uint8_t groupid=0);

	Entity(const Entity& pentity);

	const Entity& operator=(const Entity& pentity);	

	virtual ~Entity();
	
	TileWorld<>::Rectangle space();

	void count_under_attack(uint32_t attack_type,uint32_t damage, FightProp& prop, AttackValueType& type, uint32_t& hurt, uint32_t attack_num=1);
	
	// ҹУ
	uint32_t check_attack_hurt(uint32_t attack_type, uint32_t coefficient, FightProp& def_prop, uint32_t cli_hurt, uint32_t attack_num, uint32_t fixed_hurt = 0);
	
	virtual	void under_attack(Entity* pEntity, uint32_t hurt);

	virtual void under_skill(Entity* pEntity, uint32_t skill_id) {};

	virtual bool attach(const TileWorld<>::Point& point, uint8_t rule );
	
	virtual bool attach(const TileWorld<>::Rectangle& rec, uint8_t rule );

	void set_group(uint8_t groupid);

	uint8_t get_group() const;

	virtual void set_idle(uint32_t) {};

	virtual void add_buff(uint32_t id) {};
	
	void pack_basic_battle_info(BasicUserBattleInfo* battle_info) const;
	
	bool dead();

    virtual bool is_city() {return false;};

public:
	sigslot::signal1<Entity*> sig_under_attack_back;	
	 
	sigslot::signal1<Entity*> sig_dead;	
	
	sigslot::signal1<Entity*> sig_leave_battle;	
	
	sigslot::signal3<Entity*, uint32_t, Entity*> sig_health_point;	

public:
	uint8_t m_groupid;

	FightProp m_fightprop;

	Prop m_prop;

	TileWorld<>::Point m_pos;
	
	TileWorld<>::Point m_birth_pos;
	
	TileWorld<>::Point m_direct;
	
	BuffEffectManager* m_buffmanager;
};

#endif // RU_OBJECT_H_
