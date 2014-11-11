#ifndef BATTLECONFIG_H_
#define BATTLECONFIG_H_
extern "C"{
	#include "../common/log.h"
}

#include <deque>
#include <string>
#include <deque>
#include <stack>
#ifdef FRONT
#include "../frontend/xmlparser.hpp"
#else
#include <libtaomee++/conf_parser/xmlparser.hpp>
#endif
#include "../entities/object.h"
#include "../proto/cs_core.pb.h"
#include "../proto/cs_config.pb.h"
#include "base_config.h"
#include "../battlefields/tileworld.hpp"


/*used to get value from string like "1/1/1/;1/1/1"*/
template <class ValueType>
inline int get_variable_value(int element_cnt, std::string &source, std::deque<ValueType> &stack)
{
    stack.clear();
    std::string::size_type pos;
    std::string::size_type pos1;
    std::string::size_type pos_pre = 0;
    std::string::size_type pos_pre1 = 0;
    //if(element_cnt < 2) {
    //    ERROR_RETURN(("input"), -1);
    //}
	if(source=="") {
		return 0;
	}
    int info_cnt = 1;
    ValueType value= ValueType();
    do {
        pos = source.find(';', pos_pre);
        if(pos != std::string::npos) {
            info_cnt++;
        }
        std::string t = source.substr(pos_pre, pos - pos_pre);
        if(element_cnt == 1) {
            pos1 = t.find('/', pos_pre1);
            if(pos1 != std::string::npos) {
                //ERROR_RETURN(("format error or no value"), -1);
            }
			if(t != "") {
            	std::istringstream s(t);
            	s>>value;
            	stack.push_back(value);
			}
        } else {
            int format_cnt = 0;
            do {
                pos1 = t.find('/', pos_pre1);
                /*not enought '/'*/
                if(pos1 == std::string::npos && format_cnt < (element_cnt - 1)) {
                    //ERROR_RETURN(("format error or no value"), -1);
                    return -1;
                }
                if(pos1 != std::string::npos) {
                    format_cnt++;
                    /*too many '/'*/
                    if(format_cnt > (element_cnt - 1)) {
                        //ERROR_RETURN(("format error "), 100);
                    }
                }
                std::string t1 = t.substr(pos_pre1, pos1 - pos_pre1);
                std::istringstream s(t1);
                s>>value;
                stack.push_back(value);
                pos_pre1 = pos1 + 1;
            } while(pos1 != std::string::npos);
        }
        pos_pre = pos + 1;
    } while(pos != std::string::npos);
    source.clear();
    return info_cnt;
}



class Monster_t {
public:
	uint32_t id;
	bool is_boss;
	bool is_city;
	uint32_t experience;
	uint32_t attack_type;
	uint32_t attack_range_type;
	uint32_t speed;
	std::vector<uint32_t> skills;
};

class MonsterConfig: public CConfig 
{
	public:
		virtual void load_config(std::string& strFilePath);
		virtual void clear();

		Monster_t* get_monster_info(uint32_t id);

		typedef std::map<uint32_t, Monster_t*>  MonsterMap;
		static MonsterMap monster_map;
	private:
		static int on_real_load_config(xmlNodePtr cur);
};


class TileConfig {
public:
	void load_config(uint32_t map_id);
	void clear();

	TileWorld<>* get_tile_info(uint32_t id);

typedef std::map<uint32_t, TileWorld<>* > TileMap;
	static TileMap tile_map;
private:
};

class MapMonster_t {
public:
	uint32_t raid_id;
	uint32_t monster_id;
	uint32_t monster_value_type;
	uint32_t monster_level;
	TileWorld<>::Point birth_place;
	TileWorld<>::Rectangle patrol_place;
	bool elite;
    bool Friend;
};

class MapMonsterGroup_t {
public:
	float refresh_time;
	uint32_t times;
	std::vector<MapMonster_t> monsters;
};

enum SkillType {
	MELEE_ATTACK= 1, //
	SHOOT_ATTACK= 2, //
	TARGET_ATTACK= 3, //
	OBJECT_FIX_MOVE  = 4, //
	SUMMON 		= 5, //
	MULT_REMOTE = 6, //
	SELF_BUF	= 7, //buff
	TELEPORT	= 8, //fly
	FOLLOW_ATTACK = 9, //follow attack
	STEALTH 	= 10,//hidden
	JUMP_ATTACK = 11,//jump attack
	MULT_SHOOT_ATTACK = 12,//multi fire
	RAIL_GUN	= 14, //	
	NOTARGET_MELEE_ATTACK = 15,
	NOTARGET_SHOOT_ATTACK = 16,
	NOTARGET_ATTACK		  = 17,
};

typedef std::map<TileWorld<>::Point, TileWorld<>::Rectangle> RecMap;
typedef std::map<TileWorld<>::Point, TileWorld<>::Point> OffsetMap;
class Skill_t {
public:
	void insert(TileWorld<>::Point direct, TileWorld<>::Rectangle rec) {
		direct.unitised();
		INFO_LOG("ID:%d SET (%d, %d)", id, direct.x, direct.y);
		m_recs[direct] = rec;	
		rec = rec.mirrorY();
		direct.x = -direct.x;
		INFO_LOG("ID:%d SET (%d, %d)", id, direct.x, direct.y);
		m_recs[direct] = rec;	
	}

	void insert(TileWorld<>::Point direct, TileWorld<>::Point point) {
		direct.unitised();
		m_offsets[direct] = point;
		direct.x = -direct.x;
		point.x = - point.x;
		m_offsets[direct] = point;
	}

	uint32_t id;
	uint32_t type;
	uint32_t skill_type;
	uint32_t damage;
	uint32_t cool_down;
	uint32_t can_attack_number;
	RecMap m_recs;
	OffsetMap m_offsets;
	uint32_t release_time;
	uint32_t effect_time;
	uint32_t dead_time;

	uint32_t up_animation;
	uint32_t down_animation;
    uint32_t effect_animation;

	uint32_t rage_point;

	/*remote attack special*/
	uint32_t remote_attack_range;
	uint32_t rate;
	uint32_t effect_id;
	/*type == 4*/
	uint32_t move_speed;
	uint32_t displacement;
	uint32_t link_animation_down;
	uint32_t link_animation_up;
	/*type == 5*/
	MapMonsterGroup_t group;	
	
	/*type == 6 && 8*/
	uint32_t continue_time;
	uint32_t attack_rate;
	uint32_t move_start_time;

	uint32_t link_effect_time;
	uint32_t link_dead_time;

	std::vector<uint32_t> camera_shake_time;

	/*type == 7*/
	FightProp fight_prop;
	/*type == 14*/
	TileWorld<>::Point point1; 
	TileWorld<>::Point point2; 

	/*type == 16*/
	TileWorld<>::Point effect_direction;
	
	/*type == 17*/
	TileWorld<>::Point effect_fixpoint;
};


class SkillConfig: public CConfig 
{
	public:
		virtual void load_config(std::string& strFilePath);
		virtual void clear();

		Skill_t* get_skill_info(uint32_t id);

		typedef std::map<uint32_t, Skill_t*> SkillMap;
		static SkillMap skill_map;
	private:
		static int on_real_load_config(xmlNodePtr cur);
};


class MapContent_t {
public:
	uint32_t map_id;
	uint32_t trans_des;
	TileWorld<>::Point trans_place;
	std::vector < TileWorld<>::Point > birth_place_gen;
	std::map<uint32_t, MapMonsterGroup_t> monster_group;
	std::vector< TileWorld<>::Point > monster_birth;
typedef std::map<uint32_t , uint32_t> MonsterNumMap;
	MonsterNumMap monster_nums;
};

typedef std::vector<MapContent_t*> MapContentList;


class MapContentConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();
	
private:
	static int on_real_load_config(xmlNodePtr cur);
};

class Items_t {
public:
	Items_t() : item_id(0), item_level(1), item_num(1), pro(0) { }
	Items_t(uint32_t id, uint32_t lv = 1, uint32_t num = 1, uint32_t prob = 0)
		: item_id(id),
		  item_level(lv),
		  item_num(num),
		  pro(prob) {

		  }
public:
	uint32_t item_id;
	uint32_t item_level;
	uint32_t item_num;
	uint32_t pro;
};

typedef Items_t Reward_t;

enum InstanceType {
	PVUNKNOW			= 0,
	PVE 				= 1,
	PPVE 				= 2,
	PVP					= 3,
	PVAI				= 4,
	PVE2 				= 5,
	PVE3 				= 6,
	CDEF				= 7,
	PVAI2				= 8,
	GAMEBEGIN			= 9,
	WORLDBOSS			= 10,
	PVP2				= 11,
	PPVE2				= 10,	// 世界boss
	PVP3				= 12,   // 决斗
	PVP4				= 13,	// 强制决斗
	PPVE3				= 14,	// 通天塔
};

enum InstanceId {
	PVE2_START = 240000,
};

class Instance_t {
public:
	~Instance_t();
	uint32_t id;
	uint32_t type;
	uint32_t chapter;
	MapContentList contents;
typedef std::vector< base::AverageGen<Reward_t> > InstanceReward;
	InstanceReward drops;
	InstanceReward rewards;
	InstanceReward flop_rewards;
typedef std::set<uint32_t> InstanceRewardItemSet;
	InstanceRewardItemSet rewards_set;			// 概率奖励物品id
	uint32_t experience;
	uint32_t money;
	uint32_t exploit;
	uint32_t timeout; 
	uint32_t fight_num;
	uint32_t star_time;
	uint32_t effectiveness;    					// 推荐战斗力
	uint32_t phase_exp;    					// 推荐战斗力
typedef std::map<uint32_t , uint32_t> MonsterNumMap;
	MonsterNumMap monster_nums;
};

class InstanceConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();
	
	MapContent_t* get_mapcontent_info(uint32_t instance_id, uint32_t map_id = 0);

	Instance_t* get_instance_info(uint32_t instance_id);
	
	Instance_t* get_one_pvp_instance();
	
	Instance_t* get_one_pvai_instance(uint32_t type = PVAI);
	
	Instance_t* get_one_pvp2_instance();

	Instance_t* get_one_pvp3_instance();

	Instance_t* get_one_pvp4_instance();

	void pack_ppve_instance(CSPPVEBattleRsp* rsp);
	
	bool exist_instance(uint32_t id);
	
	bool pve_instance(uint32_t id);
	
	InstanceType get_instance_type(uint32_t id);

	void instance_range(uint32_t& start, uint32_t& end); 
	// 获取章节副本列表
	void get_chapter_instance(uint32_t chapter_id, InstanceType type, std::set<uint32_t> &instance_id_set);
	// 是否是章节最后一个副本, 并返回章节的id
	bool is_chapter_last_instance(uint32_t instance_id, uint32_t &chapter_id);
	// 获取instance_id下一个副本id
	uint32_t get_next_instance_id(uint32_t instance_id);

	void pack_mapcontent(CSMapContentList_t& msg, uint32_t instance_id, std::set<std::pair<uint32_t, uint32_t> >& monster_value_set );

	void unpack_mapcontent(const CSMapContentList_t& msg);

	uint32_t get_instance_btl_times(uint32_t id);

typedef std::map<uint32_t, Instance_t*> InstanceMap;

	static InstanceMap instance_map;

private:
	static int on_real_load_config(xmlNodePtr cur);
	static base::AverageGen<Instance_t*> gen;
	static base::AverageGen<Instance_t*> gen_pvai;
	static base::AverageGen<Instance_t*> gen_pvai2;
	static base::AverageGen<Instance_t*> gen_pvp2;
	static base::AverageGen<Instance_t*> gen_pvp3;
	static base::AverageGen<Instance_t*> gen_pvp4;
};




class ActionRange_t {
public:
	ActionRange_t(): action_id(0) , attack_start(0), attack_end(0) {;};
	uint32_t action_id;
	TileWorld<>::Rectangle attack_range;
	uint32_t attack_start;
	uint32_t attack_end;
};

class ObjectRange_t {
public:
	uint32_t role_id;
	TileWorld<>::Rectangle body_range;
typedef std::map<uint32_t, ActionRange_t> ActionMap;
	ActionMap attack_map;
};

class ObjectOffset_t {
public:
	uint32_t role_id;
	TileWorld<>::Point up_offset;
	TileWorld<>::Point down_offset;
};

class RangeConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();
	
	ActionRange_t* get_action_range_info(uint32_t role_id, uint32_t action_id);

	ObjectRange_t* get_role_range_info(uint32_t id);
	
	ObjectOffset_t* get_offset_info(uint32_t id) ;

private:
typedef std::map<uint32_t, ObjectRange_t*> RangeMap;
static RangeMap role_range_map;

typedef std::map<uint32_t, ObjectOffset_t*> OffsetMap;
	
	static OffsetMap offset_map;
	static int on_real_load_role_config(xmlNodePtr cur);
};

class Monster_value_t {
public:
	uint32_t id;
	uint32_t type;
	uint32_t level;
	uint32_t physical_attack; 
	uint32_t magic_attack; 
	uint32_t skill_attack; 
	uint32_t physical_defence; 
	uint32_t magic_defence; 
	uint32_t skill_defence; 
	uint32_t health_point; 
	uint32_t accurate;
	uint32_t dodge;
	uint32_t wreck;
	uint32_t parry;
	uint32_t critical_strike;
	uint32_t tenacity;
	uint32_t slay;
	uint32_t courage;
	uint32_t charm;
	uint32_t trick;
	uint32_t proficiency;
	uint32_t hp_line;
};

class MonsterValueConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();

	Monster_value_t* get_monster_info(uint32_t monster_type, uint32_t monster_level);

	void pack_monster_value(CSMonsterValue_t& msg, uint32_t monster_type, uint32_t monster_level);
	
	void unpack_monster_value(const CSMonsterValue_t& msg);
	
private:
typedef std::map< std::pair<uint32_t, uint32_t> , Monster_value_t*> MonsterMap;
	static MonsterMap monster_map;
	static int on_real_load_config(xmlNodePtr cur);
};

class SkillPart_t {
public:
	uint32_t part;
	float	 continue_time;
	float	 continue_time2;
	uint32_t can_attack_number;
	uint32_t hurt;
	uint32_t physical_attack; 
	uint32_t magic_attack; 
	uint32_t skill_attack; 
	uint32_t physical_defence; 
	uint32_t magic_defence; 
	uint32_t skill_defence; 
	uint32_t health_point; 
	uint32_t accurate;
	uint32_t dodge;
	uint32_t wreck;
	uint32_t parry;
	uint32_t critical_strike;
	uint32_t tenacity;
	uint32_t slay;

	uint32_t speed;	
};

class SkillHurt_t {
public:
	uint32_t skill_id;
	uint32_t level;
	uint32_t part;
	uint32_t skill_level;
	uint32_t money;
	uint32_t exploit;
	float	 own_cd;
	float	 share_cd;
	std::vector<SkillPart_t>  parts;
};

class HeroSkill_t {
public:
	HeroSkill_t() :
		buff(false),
		invincible(false),
		stealth(false)
	{

	}
	uint32_t role_type;
	uint32_t skill_id;
	uint32_t skill_part;
	uint32_t skill_type;
	uint32_t link_skill_id;
	bool 	 buff;
	bool	 invincible;
	bool	 stealth;
};

typedef std::vector< HeroSkill_t > HeroSkillList;

class HeroSkillConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();

	HeroSkillList get_hero_skill_list(uint32_t type);

	HeroSkill_t *get_hero_skill_info(uint32_t skill_id);
	
	HeroSkill_t *get_first_part_skill_info(uint32_t skill_id);

	bool is_buff(uint32_t skill_id);
	bool is_invincible(uint32_t skill_id);
	bool is_stealth(uint32_t skill_id);
private:
	typedef std::map< uint32_t , HeroSkillList> HeroSkillMap;
	static HeroSkillMap hero_skill_map;
	typedef std::map< uint32_t , HeroSkill_t> SkillInfoMap;
	static SkillInfoMap skill_info_map;
	static SkillInfoMap link_skill_info_map;
	static int on_real_load_config(xmlNodePtr cur);
};


class SkillHurtConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();

	SkillHurt_t* get_hurt_info(uint32_t skill_id, uint32_t level);
	uint32_t get_hurt_max_level(uint32_t skill_id);
	void pack_diff(uint32_t id, uint32_t level, CSUserSkillDiffRsp* rsp);
	void unpack_hero_skill(const UserSkillInfo& msg);
private:
	typedef std::map<std::pair<uint32_t , uint32_t>, SkillHurt_t* > SkillHurtMap;
	static SkillHurtMap skill_hurt_map;
	typedef std::map<uint32_t , uint32_t> SkillHurtMax;
	static SkillHurtMax skill_hurt_max;
	static int on_real_load_config(xmlNodePtr cur);
};

class FairySkill_t {
public:
	uint32_t id;
	uint32_t type;
	uint32_t continue_time;
	uint32_t hurt;				// 精灵技能伤害百分比
	uint32_t hurt_add;			// 精灵技能伤害固定值
	FightProp fight_prop;
	uint32_t speed;	
};

enum FairySkillType {
	FairySkillNormal = 0,
	FairySkillBuff	 = 1,
};

class FairySkillConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	virtual void clear();

	FairySkill_t* get_fairy_skill(uint32_t skill_id);

	bool is_buff(uint32_t skill_id);
	uint32_t add_blood(uint32_t skill_id,uint32_t skill_attack);

private:
	typedef std::map<uint32_t, FairySkill_t*> FairySkillMap;
	static FairySkillMap fairy_skill_map;
	static int on_real_load_config(xmlNodePtr cur);
};

class SkillUnlockConfig: public CConfig {
public:
	virtual void load_config(std::string& strFilePath);
	void clear(){};
private:
	static int on_real_load_config(xmlNodePtr cur);
};


extern MonsterConfig* g_monster_config;
extern SkillConfig* g_skill_config;
extern TileConfig* g_tile_config;
extern MapContentConfig* g_mapcontent_config;
extern RangeConfig* g_range_config;
extern MonsterValueConfig *g_monster_value_config;
extern InstanceConfig* g_instance_config;
extern SkillHurtConfig *g_skill_hurt_config;
extern HeroSkillConfig *g_hero_skill_config;
extern FairySkillConfig *g_fairy_skill_config;
extern SkillUnlockConfig *g_skill_unlock_config;
#endif

