#ifndef USERSKILL_H_
#define	USERSKILL_H_

#include <map>
#include "../proto/cs_core.pb.h"
#include "../proto/db.pb.h"
#include "../proto/inter.pb.h"
#include "../config/battleconfig.h"

using namespace rudb;

class Player;

class UserSkill {
public:
	UserSkill();
	UserSkill(uint32_t skill_id, uint32_t skill_level);
	uint32_t skill_id;
	uint32_t skill_level;
};

class UserSkillManager {
public:
	UserSkillManager(Player* player);

	void init_from_db(const db_skill_query_out* msg);

	void unpack_skills(CSGetUserSkillRsp* rsp);
	
	void pack_skills(CSGetUserSkillRsp* rsp);

	void pack_skills(InterUserSkillInfo* req);

	void unpack_skills(const InterUserSkillInfo* rsp);

	void add_skill(uint32_t id, uint32_t level);
	
	uint32_t get_skill_level(uint32_t id);

	std::map<uint32_t, UserSkill> get_skill_map();

	bool update_valid();

#ifndef FRONT
	void game_first_start(); 	
	
	void after_game_first_start(); 	
#endif

private:
	void add_skill_with_skills_is_null();

private:
typedef std::map<uint32_t, UserSkill> SkillMap;
	SkillMap m_skill_map;
	Player* m_player;
};

#endif
