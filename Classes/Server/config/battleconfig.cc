#include <map>
#include <vector>
#include <list>
#include <set>
#include <deque>
#include <sstream>
#ifndef FRONT
#include "../msg_dispatcher/msg_dispatcher.h"
#include "../items/item.h"
#endif
#include "battleconfig.h"


#ifndef FRONT
extern BackPack test_pack;
#endif
MonsterConfig* g_monster_config;
SkillConfig* g_skill_config;
TileConfig* g_tile_config;
MapContentConfig* g_mapcontent_config;
RangeConfig* g_range_config;
MonsterValueConfig *g_monster_value_config;
InstanceConfig* g_instance_config;
HeroSkillConfig *g_hero_skill_config;
SkillHurtConfig *g_skill_hurt_config;
FairySkillConfig *g_fairy_skill_config;
SkillUnlockConfig *g_skill_unlock_config;
//------------------------------------------------ MonsterConfig -----------------------------------------
MonsterConfig::MonsterMap MonsterConfig::monster_map;

void MonsterConfig::load_config(std::string& strFilePath) {
	static const char* MONSTER_CONFIG_NAME = "MonsterBasicInfomation.xml";
	m_strFilePath = strFilePath + MONSTER_CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

void MonsterConfig::clear() {
	MonsterMap::iterator itr = monster_map.begin();
	for(; itr!=monster_map.end(); ++itr) {
		delete itr->second;
	}
	monster_map.clear();
}

int MonsterConfig::on_real_load_config(xmlNodePtr cur) {


	KINFO_LOG(0, "----------------------[BEGIN_MONSTER_CONFIG]-----------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("Monster")))
		{
			Monster_t* monster_config = new Monster_t();
			taomee::get_xml_prop(monster_config->id, cur, "MonsterID");
			taomee::get_xml_prop(monster_config->is_boss, cur, "IsBoss");
			taomee::get_xml_prop(monster_config->is_city, cur, "IsCity");
			taomee::get_xml_prop(monster_config->attack_type, cur, "AttackType");
			taomee::get_xml_prop(monster_config->attack_range_type, cur, "AttackRangeType");
			taomee::get_xml_prop(monster_config->speed, cur, "Speed");

			std::string source;
			std::deque<uint32_t> content;
			taomee::get_xml_prop_def(source, cur, "Skill", "");
			int cnt = get_variable_value(1,source,content);
			if(!cnt) {
				ERROR_LOG("MONSTER:%d WITHOUT SKILL", monster_config->id);
			}
			for(; cnt>0; cnt--) {
				uint32_t skill_id = content.front();
				monster_config->skills.push_back(skill_id);
				content.pop_front();
				/*load range*/
				Skill_t* skill = g_skill_config->get_skill_info(skill_id);
				if(skill->type == MELEE_ATTACK) {
					ActionRange_t* up_action_range = g_range_config->get_action_range_info(monster_config->id, skill->up_animation);
					skill->insert(TileWorld<>::Point(1,1), up_action_range->attack_range);	
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->insert(TileWorld<>::Point(-1,-1), down_action_range->attack_range);	
					skill->effect_time = down_action_range->attack_start;
					skill->dead_time = down_action_range->attack_end;
				} else if(skill->type == TARGET_ATTACK) { 
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					ActionRange_t* action_range = g_range_config->get_action_range_info(skill->effect_id, 100000);
					skill->release_time = down_action_range->attack_start;
					skill->effect_time = down_action_range->attack_start + action_range->attack_start;
					skill->dead_time = down_action_range->attack_start + action_range->attack_end;
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
				} else if(skill->type == SHOOT_ATTACK) {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->effect_time = down_action_range->attack_start;	
					ActionRange_t* action_range = g_range_config->get_action_range_info(skill->effect_id, 100000);
					ObjectOffset_t* offset = g_range_config->get_offset_info(monster_config->id);
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
					skill->insert(TileWorld<>::Point(-1,-1), rec);
					skill->insert(TileWorld<>::Point(1,1), offset->up_offset);
					skill->insert(TileWorld<>::Point(-1,-1), offset->down_offset);
				} else if(skill->type == OBJECT_FIX_MOVE) {
					ActionRange_t* action_range = g_range_config->get_action_range_info(monster_config->id, skill->up_animation);
					skill->effect_time = action_range->attack_start;	
					ObjectOffset_t* offset = g_range_config->get_offset_info(monster_config->id);
					action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_up);
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
					offset = g_range_config->get_offset_info(monster_config->id);
					action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_down);
					rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(-1,-1), rec);
					skill->insert(TileWorld<>::Point(1,1), offset->up_offset);
					skill->insert(TileWorld<>::Point(-1,-1), offset->down_offset);
				} else if(skill->type == SUMMON) {
					ActionRange_t* up_action_range = g_range_config->get_action_range_info(monster_config->id, skill->up_animation);
					skill->insert(TileWorld<>::Point(1,1), up_action_range->attack_range);	
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->insert(TileWorld<>::Point(-1,-1), down_action_range->attack_range);	
					skill->effect_time = down_action_range->attack_start;
					skill->dead_time = down_action_range->attack_end;
				} else if(skill->type == MULT_REMOTE) {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					ActionRange_t* action_range = g_range_config->get_action_range_info(skill->effect_id, 100000);
					skill->release_time = down_action_range->attack_start;
					skill->effect_time = down_action_range->attack_start + action_range->attack_start;
					skill->dead_time = down_action_range->attack_start + action_range->attack_end;
					skill->link_effect_time  = action_range->attack_start;
					skill->link_dead_time	 = action_range->attack_end;
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
				} else if(skill->type == SELF_BUF) {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->effect_time = down_action_range->attack_start;
					skill->dead_time = down_action_range->attack_end;
				} else if(skill->type == FOLLOW_ATTACK) {
					ActionRange_t* up_action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_up);
					skill->insert(TileWorld<>::Point(1,1), up_action_range->attack_range);	
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_down);
					skill->insert(TileWorld<>::Point(-1,-1), down_action_range->attack_range);	
 				} else if(skill->type == TELEPORT) {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->release_time = down_action_range->attack_start;
				} else if(skill->type == JUMP_ATTACK) {
					ActionRange_t* action_range = g_range_config->get_action_range_info(monster_config->id, skill->up_animation);
					skill->effect_time = action_range->attack_start;	
					action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_up);
					skill->link_effect_time = action_range->attack_start;
					skill->link_dead_time	= action_range->attack_end;
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
					action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_down);
					rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(-1,-1), rec);
				} else if(skill->type == STEALTH)  {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->release_time = down_action_range->attack_start;
				} else if(skill->type == MULT_SHOOT_ATTACK) {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->effect_time = down_action_range->attack_start;	
					ActionRange_t* action_range = g_range_config->get_action_range_info(skill->effect_id, 100000);
					ObjectOffset_t* offset = g_range_config->get_offset_info(monster_config->id);
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
					skill->insert(TileWorld<>::Point(-1,-1), rec);
					skill->insert(TileWorld<>::Point(1,1), offset->up_offset);
					skill->insert(TileWorld<>::Point(-1,-1), offset->down_offset);
				} else if(skill->type == RAIL_GUN) {
					ActionRange_t* action_range = g_range_config->get_action_range_info(monster_config->id, skill->up_animation);
					skill->effect_time = action_range->attack_start;	
					ObjectOffset_t* offset = g_range_config->get_offset_info(monster_config->id);
					action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_up);
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
					offset = g_range_config->get_offset_info(monster_config->id);
					action_range = g_range_config->get_action_range_info(monster_config->id, skill->link_animation_down);
					rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(-1,-1), rec);
					skill->insert(TileWorld<>::Point(1,1), offset->up_offset);
					skill->insert(TileWorld<>::Point(-1,-1), offset->down_offset);
				} else if(skill->type == NOTARGET_MELEE_ATTACK) {
					ActionRange_t* up_action_range = g_range_config->get_action_range_info(monster_config->id, skill->up_animation);
					skill->insert(TileWorld<>::Point(1,1), up_action_range->attack_range);	
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->insert(TileWorld<>::Point(-1,-1), down_action_range->attack_range);	
					skill->effect_time = down_action_range->attack_start;
					skill->dead_time = down_action_range->attack_end;
				} else if(skill->type == NOTARGET_SHOOT_ATTACK) {
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					skill->effect_time = down_action_range->attack_start;	
					ActionRange_t* action_range = g_range_config->get_action_range_info(skill->effect_id, 100000);
					ObjectOffset_t* offset = g_range_config->get_offset_info(monster_config->id);
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
					skill->insert(TileWorld<>::Point(-1,-1), rec);
					skill->insert(TileWorld<>::Point(1,1), offset->up_offset);
					skill->insert(TileWorld<>::Point(-1,-1), offset->down_offset);
				} else if(skill->type == NOTARGET_ATTACK) { 
					ActionRange_t* down_action_range = g_range_config->get_action_range_info(monster_config->id, skill->down_animation);
					ActionRange_t* action_range = g_range_config->get_action_range_info(skill->effect_id, 100000);
					skill->release_time = down_action_range->attack_start;
					skill->effect_time = down_action_range->attack_start + action_range->attack_start;
					skill->dead_time = down_action_range->attack_start + action_range->attack_end;
					TileWorld<>::Rectangle rec = action_range->attack_range;
					skill->insert(TileWorld<>::Point(1,1), rec);
				} 
			}	
			monster_map[monster_config->id] = monster_config;
			INFO_LOG("LOAD MONSTER ID:%d AT:%d ART:%d SP:%d",monster_config->id, monster_config->attack_type, monster_config->attack_range_type, monster_config->speed);
		}
		cur = cur->next;
	}
	KINFO_LOG(0,"--------------------------[END_MONSTER_CONFIG]------------------------");
	return 0;
}


Monster_t* MonsterConfig::get_monster_info(uint32_t id) 
{
	return monster_map[id] != NULL ? monster_map[id] : NULL;
}

Instance_t::~Instance_t() {
	MapContentList::iterator itr = contents.begin(); 
	for(; itr!=contents.end(); ++ itr) {
		delete (*itr);
	}
}

InstanceConfig::InstanceMap InstanceConfig::instance_map;
base::AverageGen<Instance_t*> InstanceConfig::gen;
base::AverageGen<Instance_t*> InstanceConfig::gen_pvai;
base::AverageGen<Instance_t*> InstanceConfig::gen_pvai2;
base::AverageGen<Instance_t*> InstanceConfig::gen_pvp2;
base::AverageGen<Instance_t*> InstanceConfig::gen_pvp3;
base::AverageGen<Instance_t*> InstanceConfig::gen_pvp4;

void InstanceConfig::load_config(std::string& strFilePath) 
{
	static const char* MAPCONTENT_CONFIG_NAME = "Map_Basic.xml";
	m_strFilePath = strFilePath + MAPCONTENT_CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

void InstanceConfig::clear()
{
	InstanceMap::iterator itr = instance_map.begin();
	for( ; itr!=InstanceConfig::instance_map.end(); ++itr) {
		delete itr->second;
	}
	instance_map.clear();
	gen.clear();
	gen_pvai.clear();
	gen_pvai2.clear();
	gen_pvp2.clear();
	gen_pvp3.clear();
	gen_pvp4.clear();
}

int InstanceConfig::on_real_load_config(xmlNodePtr cur) {
	KINFO_LOG(0, "----------------------[BEGIN_INSTANCE_CONFIG]-----------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("Map")))
		{
			uint32_t id = 0;
			uint32_t map_id = 0;
			taomee::get_xml_prop(id, cur, "RaidID");
			taomee::get_xml_prop(map_id, cur, "MapID");
#ifndef FRONT
			g_tile_config->load_config(map_id);
#endif
			Instance_t* instance = NULL;	
			MapContent_t* mapcontent = NULL;
			if(instance_map.find(id) == instance_map.end()) {
				instance = new Instance_t();
				instance->id = id;
				taomee::get_xml_prop(instance->type, cur, "Type");
				taomee::get_xml_prop_def(instance->chapter, cur, "Chapter", 0);
				if(instance->type == PVP) {
					gen.push(instance);
				}
				if(instance->type == PVAI) {
					gen_pvai.push(instance);	
				}
				if(instance->type == PVAI2) {
					gen_pvai2.push(instance);	
				}
				if (instance->type == PVP2) {
					gen_pvp2.push(instance);
				}
				if (instance->type == PVP3) {
					gen_pvp3.push(instance);
				}
				if (instance->type == PVP4) {
					gen_pvp4.push(instance);
				}
				instance_map[id] = instance;

#define GEN_MAP_REWARD(member, param) \
				do { \
					std::string source; \
					std::deque<uint32_t> content;\
					taomee::get_xml_prop(source, cur, ""#member""); \
					base::AverageGen<Reward_t> gen; \
					int cnt = get_variable_value(3,source,content); \
					for(; cnt >0; cnt--) { \
						Reward_t reward; \
						reward.item_id =content.front(); \
						test_pack.get_item_type(reward.item_id); \
                        reward.item_level = 1; \
                        content.pop_front(); \
                        reward.item_num =content.front(); \
                        content.pop_front(); \
                        reward.pro =content.front(); \
                        content.pop_front(); \
                        gen.push(reward); \
						instance->rewards_set.insert(reward.item_id); \
					} \
					(param).push_back(gen); \
				} while(0);

#define GEN_MAP_SPIRIT(member, param) \
				do { \
					std::string source; \
					std::deque<uint32_t> content;\
					taomee::get_xml_prop(source, cur, ""#member""); \
					base::AverageGen<Reward_t> gen; \
					int cnt = get_variable_value(4,source,content); \
					for(; cnt >0; cnt--) { \
						Reward_t reward; \
						reward.item_id =content.front(); \
						test_pack.get_item_type(reward.item_id); \
                        content.pop_front(); \
                        reward.item_num = content.front(); \
                        content.pop_front(); \
                        reward.pro =content.front(); \
                        content.pop_front(); \
                        reward.item_level = content.front(); \
                        gen.push(reward); \
						instance->rewards_set.insert(reward.item_id); \
					} \
					(param).push_back(gen); \
				} while(0);
#ifndef FRONT 
	/*			GEN_MAP_REWARD(Drops0,instance->drops);
				GEN_MAP_REWARD(Drops1,instance->drops);
				GEN_MAP_REWARD(Drops2,instance->drops);
				GEN_MAP_REWARD(Drops3,instance->drops);
				GEN_MAP_REWARD(Drops4,instance->drops);*/
				GEN_MAP_REWARD(RaidDrops1, instance->rewards);
				GEN_MAP_REWARD(RaidDrops2, instance->rewards);
				GEN_MAP_REWARD(RaidDrops3, instance->rewards);
				GEN_MAP_REWARD(RaidDrops4, instance->rewards);
				GEN_MAP_REWARD(RaidDrops5, instance->rewards);

				GEN_MAP_SPIRIT(Spirit1, instance->rewards);
				GEN_MAP_SPIRIT(Spirit2, instance->rewards);
				GEN_MAP_SPIRIT(Spirit3, instance->rewards);
				//GEN_MAP_REWARD(RaidDrops5, instance->flop_rewards);
#endif

#undef GEN_MAP_REWARD
			} else {
				instance = instance_map[id];
				MapContentList::iterator itr = instance_map[id]->contents.begin();
				for(; itr!=instance_map[id]->contents.end(); itr++) {
					if( (*itr)->map_id == map_id ) {
						mapcontent = (*itr);
						break;
					}
				}
			}
			if(!mapcontent) {
				mapcontent = new MapContent_t();
				mapcontent->map_id = map_id;
				taomee::get_xml_prop_def(mapcontent->trans_des, cur, "TransDestination", 0);
#ifndef FRONT
				if(mapcontent->trans_des) {
					g_tile_config->load_config(mapcontent->trans_des);
				}
#endif
				MapContentList::iterator itr = instance_map[id]->contents.begin();
				bool insert = false;
				for(; itr!=instance_map[id]->contents.end(); ++itr) {
					if( (*itr)->trans_des == map_id ) {
						instance_map[id]->contents.insert(++itr, mapcontent);
						insert = true;
						break;
					}
					if(mapcontent->trans_des == (*itr)->map_id) {
						instance_map[id]->contents.insert(itr, mapcontent);
						insert = true;
						break;
					}	
				}
				if(!insert) {
					instance_map[id]->contents.push_back(mapcontent);
				}
			}
			std::string source;
			std::deque<uint32_t> content;
			taomee::get_xml_prop(source, cur, "HeroBornCoordinate");
			mapcontent->birth_place_gen.clear();
			int cnt = get_variable_value(2,source,content);
			for(int i=0; i< cnt; i++)
			{
				TileWorld<>::Point birth;
				birth.x = content.front();
				content.pop_front();
				birth.y = content.front();
				content.pop_front();
				mapcontent->birth_place_gen.push_back(birth);
			}
			taomee::get_xml_prop_def(mapcontent->trans_des, cur, "TransDestination", 0);
			if(mapcontent->trans_des) {	
				source.clear();
				content.clear();
				taomee::get_xml_prop(source, cur, "TransCoordinate");
				get_variable_value(2,source,content);
				mapcontent->trans_place.x = content.front();
				content.pop_front();
				mapcontent->trans_place.y = content.front();
				content.pop_front();
			}
			taomee::get_xml_prop(instance->experience, cur, "exp");
			taomee::get_xml_prop(instance->money, cur, "money");
			taomee::get_xml_prop(instance->exploit, cur, "Exploit");
			taomee::get_xml_prop_def(instance->timeout, cur, "count_down", 0);
			taomee::get_xml_prop_def(instance->fight_num, cur, "Number", 0);
			taomee::get_xml_prop(instance->star_time, cur, "Star_Time");
			taomee::get_xml_prop(instance->effectiveness, cur, "Fighting");
			taomee::get_xml_prop(instance->phase_exp, cur, "Phase_Exp");
		
			source.clear();
			content.clear();
			taomee::get_xml_prop_def(source, cur, "MosterBirth", "");
			cnt = get_variable_value(2,source,content);
			for(int i=0; i< cnt; i++)
			{
				TileWorld<>::Point birth;
				birth.x = content.front();
				content.pop_front();
				birth.y = content.front();
				content.pop_front();
				mapcontent->monster_birth.push_back(birth);
			}
			KINFO_LOG(0,"LOAD INSTANCE ID :%d MAP ID:%d HERO BORN:%d", instance->id, mapcontent->map_id, mapcontent->birth_place_gen.size());
		}
		cur = cur->next;
	}
//	assert(!gen.empty());
	KINFO_LOG(0,"--------------------------[END_INSTANCE_CONFIG]------------------------");
	return 0;
}

MapContent_t* InstanceConfig::get_mapcontent_info(uint32_t instance_id, uint32_t map_id) {
	if(instance_map.find(instance_id) != instance_map.end()) {
		MapContentList::iterator itr = instance_map[instance_id]->contents.begin();
		for(; itr!=instance_map[instance_id]->contents.end(); itr++) {
			if( (*itr)->map_id == map_id || !map_id ) {
				return (*itr);
			}
		}
	}
	return NULL;
}

Instance_t* InstanceConfig::get_instance_info(uint32_t instance_id) {
	if(instance_map.find(instance_id) != instance_map.end()) {
		return instance_map[instance_id];
	}
	return NULL;
}

Instance_t* InstanceConfig::get_one_pvp_instance() {
	return gen.get_one(false);
}
	
Instance_t* InstanceConfig::get_one_pvai_instance(uint32_t type) {
	if (type == PVAI) {
		return gen_pvai.get_one(false);
	} else {
		return gen_pvai2.get_one(false);
	}
}

Instance_t* InstanceConfig::get_one_pvp2_instance()
{
	return gen_pvp2.get_one(false);
}


Instance_t* InstanceConfig::get_one_pvp3_instance()
{
	return gen_pvp3.get_one(false);
}


Instance_t* InstanceConfig::get_one_pvp4_instance()
{
	return gen_pvp4.get_one(false);	
}

void InstanceConfig::pack_ppve_instance(CSPPVEBattleRsp* rsp) {
	InstanceMap::iterator itr = instance_map.begin();
	for( ; itr!=InstanceConfig::instance_map.end(); ++itr) {
		if(itr->second->type == PPVE) {
			rsp->add_ids(itr->second->id);
		}
	}
}

bool InstanceConfig::exist_instance(uint32_t id) {
	return instance_map.find(id) != instance_map.end();
}

bool InstanceConfig::pve_instance(uint32_t id) {
	if(instance_map.find(id) != instance_map.end() && (instance_map[id]->type == PVE || instance_map[id]->type == PVE2) )
		return true;
	return false;
}

InstanceType InstanceConfig::get_instance_type(uint32_t id)
{
	if(instance_map.find(id) != instance_map.end()) {
		return static_cast<InstanceType>(instance_map[id]->type);
	} else {
		return PVUNKNOW;
	}
}

void InstanceConfig::instance_range(uint32_t& start, uint32_t& end) {
	if(!instance_map.empty() && (!start && !end)) {
		start = instance_map.begin()->first;
		end = instance_map.rbegin()->first;
	}
}

void InstanceConfig::get_chapter_instance(uint32_t chapter_id, InstanceType type, std::set<uint32_t> &instance_id_set)
{
	instance_id_set.clear();
	if (chapter_id) {
		InstanceMap::iterator it = instance_map.begin();
		for(; it != instance_map.end(); it++) {
			if (it->second->chapter == chapter_id && it->second->type == static_cast<uint32_t>(type)) {
				instance_id_set.insert(it->first);
			}
		}
	}
}

bool InstanceConfig::is_chapter_last_instance(uint32_t instance_id, uint32_t &chapter_id)
{
	InstanceMap::iterator it = instance_map.begin();
	for(; it != instance_map.end(); it++) {
		if (it->first == instance_id) {
			chapter_id = it->second->chapter;
			++it;
			break;
		}
	}
	if (it != instance_map.end()) {
		if (it->second->chapter == chapter_id) {
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

uint32_t InstanceConfig::get_next_instance_id(uint32_t instance_id)
{
	InstanceMap::iterator temp = instance_map.end();
	// NOTI 必须保证instance id 大小顺序
	InstanceMap::iterator it = instance_map.lower_bound(instance_id);
	if (it != instance_map.end()
		&& it->second->id == instance_id) {
		temp = it;
		temp++;
	}
	if (temp != instance_map.end()
		&& it->second->type == temp->second->type) {
		return temp->second->id;
	} else {
		return 0;
	}
}

void InstanceConfig::pack_mapcontent(CSMapContentList_t& msg, uint32_t instance_id, std::set<std::pair<uint32_t, uint32_t> >& monster_value_set)
{
	msg.set_instance_id(instance_id);
	if(instance_map.find(instance_id) != instance_map.end()) {
		Instance_t* pinstance = instance_map[instance_id];
		MapContentList::iterator itr = pinstance->contents.begin();
		for(; itr!=pinstance->contents.end(); ++itr) {
			CSMapContent_t* mapcontent = msg.add_map_contents();
			mapcontent->set_map_id((*itr)->map_id);
			mapcontent->set_trans_des((*itr)->trans_des);
			CSPoint* point = mapcontent->mutable_trans_place();
			point->set_xpos((*itr)->trans_place.x);
			point->set_ypos((*itr)->trans_place.y);
			std::vector < TileWorld<>::Point >::iterator itr_birth =  (*itr)->birth_place_gen.begin();
			for(; itr_birth!= (*itr)->birth_place_gen.end(); ++itr_birth ) {
				CSPoint* birth_point = mapcontent->add_birth_place_gen();
				birth_point->set_xpos(itr_birth->x);	
				birth_point->set_ypos(itr_birth->y);	
			}
			itr_birth = (*itr)->monster_birth.begin();
			for(; itr_birth!= (*itr)->monster_birth.end(); ++itr_birth ) {
				CSPoint* birth_point = mapcontent->add_monster_birth();
				birth_point->set_xpos(itr_birth->x);	
				birth_point->set_ypos(itr_birth->y);	
			}
			MapContent_t::MonsterNumMap::iterator itr_num = (*itr)->monster_nums.begin();
			for(; itr_num!= (*itr)->monster_nums.end(); ++itr_num ) {
				CSMapContent_t_CSMonsterNum* monster_num = mapcontent->add_monster_nums();
				monster_num->set_monster_id(itr_num->first);	
				monster_num->set_monster_num(itr_num->second);	
			}
			std::map<uint32_t, MapMonsterGroup_t>::iterator itr_group = (*itr)->monster_group.begin();
			for(; itr_group!=(*itr)->monster_group.end(); ++itr_group) {
				CSMapMonsterGroup_t* monster_group = mapcontent->add_monster_group();
				monster_group->set_refresh_time(itr_group->second.refresh_time);
				monster_group->set_times(itr_group->second.times);
				monster_group->set_group_id(itr_group->first);
				std::vector<MapMonster_t>::iterator itr_monster = itr_group->second.monsters.begin();
				for(; itr_monster!=itr_group->second.monsters.end(); itr_monster++) {
					CSMapMonster_t* map_monster = monster_group->add_monsters();
					map_monster->set_raid_id(itr_monster->raid_id);	
					map_monster->set_monster_id(itr_monster->monster_id);	
					map_monster->set_monster_value_type(itr_monster->monster_value_type);
					map_monster->set_monster_level(itr_monster->monster_level);
					map_monster->set_elite(itr_monster->elite);
					monster_value_set.insert(std::pair<uint32_t, uint32_t> (itr_monster->monster_value_type, itr_monster->monster_level));	
					CSPoint* birth_place = map_monster->mutable_birth_place();
					birth_place->set_xpos(itr_monster->birth_place.x);
					birth_place->set_ypos(itr_monster->birth_place.y);
					CSRangeInfo* patrol_place = map_monster->mutable_patrol_place();
					CSPoint* start = patrol_place->mutable_start();
					start->set_xpos(itr_monster->patrol_place.start.x);
					start->set_ypos(itr_monster->patrol_place.start.y);
					CSPoint* end = patrol_place->mutable_end();
					end->set_xpos(itr_monster->patrol_place.end.x);
					end->set_ypos(itr_monster->patrol_place.end.y);
				/*summon_monstert*/
					Monster_t* pmonster = g_monster_config->get_monster_info(itr_monster->monster_id);
					if(pmonster && pmonster->is_boss) {
						std::vector<uint32_t>::iterator itr = pmonster->skills.begin();
						for(; itr!=pmonster->skills.end(); ++itr) {
							Skill_t* skill = g_skill_config->get_skill_info((*itr));
							if(skill->type == SUMMON) {
								std::vector<MapMonster_t>::iterator itr_summon = skill->group.monsters.begin();
								for(; itr_summon!= skill->group.monsters.end(); ++ itr_summon) {
									monster_value_set.insert(std::pair<uint32_t, uint32_t> (itr_summon->monster_value_type, itr_monster->monster_level));										}
							}
						}
					}
				}
			}
			
		}
	}
}

void InstanceConfig::unpack_mapcontent( const CSMapContentList_t& msg)
{
	uint32_t instance_id = msg.instance_id();
	InstanceMap::iterator itr = instance_map.find(instance_id);
	Instance_t* pinstance = NULL;
	if(itr != instance_map.end()) {
		itr->second->contents.clear();
		pinstance = itr->second;
	}
	for(int i = 0; i < msg.map_contents_size(); i++) {
		const CSMapContent_t& mapcontent = msg.map_contents(i);
		MapContent_t* map_content = new MapContent_t();
		pinstance->contents.push_back(map_content);	
		map_content->map_id = mapcontent.map_id();
		map_content->trans_des = mapcontent.trans_des();	
		map_content->trans_place.x = mapcontent.trans_place().xpos();
		map_content->trans_place.y = mapcontent.trans_place().ypos();
			
		for(int n = 0; n < mapcontent.birth_place_gen_size(); n++) {
			const CSPoint& birth_point = mapcontent.birth_place_gen(n);
			TileWorld<>::Point point(birth_point.xpos(), birth_point.ypos());
			map_content->birth_place_gen.push_back(point);
		}
		for(int n2 = 0; n2 < mapcontent.monster_birth_size(); n2++) {
			const CSPoint& birth_point = mapcontent.monster_birth(n2);
			TileWorld<>::Point point(birth_point.xpos(), birth_point.ypos());
			map_content->monster_birth.push_back(point);
		}
		for(int n3 = 0; n3 < mapcontent.monster_nums_size(); n3++) {
			const CSMapContent_t_CSMonsterNum& monster_num = mapcontent.monster_nums(n3);
			map_content->monster_nums[monster_num.monster_id()] = monster_num.monster_num();
		};
		for(int n4 = 0; n4 < mapcontent.monster_group_size(); n4++) {
			const CSMapMonsterGroup_t& monster_group = mapcontent.monster_group(n4);
			MapMonsterGroup_t group;
			group.refresh_time = monster_group.refresh_time();
			group.times = monster_group.times();
			uint32_t group_id = monster_group.group_id();
		
			for(int m=0; m<monster_group.monsters_size(); m++) {
				const CSMapMonster_t& map_monster = monster_group.monsters(m);	
				MapMonster_t monster;
				monster.raid_id = map_monster.raid_id();
				monster.monster_id = map_monster.monster_id();
				monster.monster_value_type = map_monster.monster_value_type();
				monster.monster_level = map_monster.monster_level();
				monster.elite = map_monster.elite();
				monster.birth_place.x = map_monster.birth_place().xpos();
				monster.birth_place.y = map_monster.birth_place().ypos();
				monster.patrol_place.start.x = map_monster.patrol_place().start().xpos();	
				monster.patrol_place.start.y = map_monster.patrol_place().start().ypos();	
				monster.patrol_place.end.x = map_monster.patrol_place().end().xpos();	
				monster.patrol_place.end.y = map_monster.patrol_place().end().ypos();	
				group.monsters.push_back(monster);
			}
	
			map_content->monster_group[group_id] = group;
		};
	}
}


uint32_t InstanceConfig::get_instance_btl_times(uint32_t id)
{
	if(instance_map.find(id) != instance_map.end()) {
		return instance_map[id]->fight_num;
	} 
	return 0;
}

SkillConfig::SkillMap SkillConfig::skill_map;

void SkillConfig::load_config(std::string& strFilePath) 
{
	static const char* SKILL_CONFIG_NAME = "MonsterSkill.xml";
	m_strFilePath = strFilePath + SKILL_CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

void SkillConfig::clear() {
	SkillMap::iterator itr = skill_map.begin();
	for(; itr!=skill_map.end(); ++itr) {
		delete itr->second;
	}
	skill_map.clear();
}

int SkillConfig::on_real_load_config(xmlNodePtr cur) {
	KINFO_LOG(0, "----------------------[BEGIN_SKILL_CONFIG]-----------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("MonsterSkill")))
		{
			Skill_t* skill_config = new Skill_t();
			taomee::get_xml_prop(skill_config->id, cur, "skill_id");
			taomee::get_xml_prop(skill_config->type, cur, "attack_type");
			taomee::get_xml_prop(skill_config->skill_type, cur, "skill_type");
			taomee::get_xml_prop(skill_config->damage, cur, "damage");
			float cool_down;
			taomee::get_xml_prop(cool_down, cur, "cool_time");
			skill_config->cool_down = cool_down*1000;
			taomee::get_xml_prop(skill_config->can_attack_number, cur, "can_attack_number");
			taomee::get_xml_prop_def(skill_config->remote_attack_range, cur, "remote_attack_range", 0);
			taomee::get_xml_prop_def(skill_config->displacement, cur, "displacement", 0);
			taomee::get_xml_prop_def(skill_config->rate, cur, "rate", 0);
			taomee::get_xml_prop_def(skill_config->move_speed, cur, "move_speed", 0);
			uint32_t start_frame, end_frame;
			taomee::get_xml_prop_def(start_frame, cur, "start_frame", 0);
			taomee::get_xml_prop_def(end_frame, cur, "end_frame", 0);
			skill_config->effect_time 	= 1000 * start_frame/24;
			skill_config->dead_time		= 1000 * end_frame/24;
			taomee::get_xml_prop(skill_config->up_animation, cur, "up_animation_id");
			taomee::get_xml_prop(skill_config->down_animation, cur, "down_animation_id");
			taomee::get_xml_prop(skill_config->effect_animation, cur, "effect_animation");
			assert(skill_config->up_animation);
			assert(skill_config->down_animation);
			taomee::get_xml_prop_def(skill_config->effect_id, cur, "effect_id", 0);
			taomee::get_xml_prop_def(skill_config->link_animation_down, cur, "link_animation_down_id", 0);
			taomee::get_xml_prop_def(skill_config->link_animation_up, cur, "link_animation_up_id", 0);
			
			taomee::get_xml_prop_def(skill_config->continue_time, cur, "continue_time", 0);
			skill_config->continue_time *=1000;
			float attack_rate_float;
			taomee::get_xml_prop_def(attack_rate_float, cur, "attack_rate", 0);
			skill_config->attack_rate = attack_rate_float * 1000;
			float move_start_time;
			taomee::get_xml_prop_def(move_start_time, cur, "move_start_time", 0);
			skill_config->move_start_time = move_start_time * 1000;
			
			taomee::get_xml_prop_def(skill_config->fight_prop.physical_attack, cur, "phy_attack", 0);
			taomee::get_xml_prop_def(skill_config->fight_prop.magic_attack, cur, "mag_attack", 0);
			taomee::get_xml_prop_def(skill_config->fight_prop.skill_attack, cur, "ski_attack", 0);
			taomee::get_xml_prop_def(skill_config->fight_prop.physical_defence, cur, "phy_defence", 0);
			taomee::get_xml_prop_def(skill_config->fight_prop.magic_defence, cur, "mag_defence", 0);
			taomee::get_xml_prop_def(skill_config->fight_prop.skill_defence, cur, "ski_defence", 0);
			taomee::get_xml_prop(skill_config->rage_point, cur, "rage_point");

			std::string source; 
			std::deque<uint32_t> content;
			taomee::get_xml_prop_def(source, cur, "summon_monster", "");
			int cnt = get_variable_value(6, source, content);
			for(int i=0; i< cnt; i++) {
				MapMonster_t monster;
				monster.birth_place.x = content.front();
				content.pop_front();
				monster.birth_place.y = content.front();
				content.pop_front();
				monster.monster_id = content.front();
				content.pop_front();
				monster.elite = content.front();
				content.pop_front();
				monster.monster_value_type = content.front();
				content.pop_front();
				uint32_t num = content.front();
				content.pop_front();
				for(uint32_t i=0; i< num ; i++) {
					skill_config->group.monsters.push_back(monster);	
				}
			}
			source.clear(); 
			content.clear();
			taomee::get_xml_prop_def(source, cur, "path_point", "");
			cnt = get_variable_value(2, source, content);
			assert(cnt ==2 || cnt==0);
			if(cnt == 2) {	
				skill_config->point1.x = content.front();
				content.pop_front();
				skill_config->point1.y = content.front();
				content.pop_front();
				skill_config->point2.x = content.front();
				content.pop_front();
				skill_config->point2.y = content.front();
				content.pop_front();
			}
			source.clear();
			content.clear();
			std::deque<int32_t> effect_content;
			taomee::get_xml_prop_def(source, cur, "effect_direction", "");
			cnt = get_variable_value(2, source, effect_content);
			assert(cnt ==1 || cnt==0);
			if(cnt == 1) {	
				skill_config->effect_direction.x = effect_content.front();
				effect_content.pop_front();
				skill_config->effect_direction.y = effect_content.front();
				effect_content.pop_front();
			}
			source.clear();
			content.clear();
			taomee::get_xml_prop_def(source, cur, "effect_fixpoint", "");
			cnt = get_variable_value(2, source, content);
			assert(cnt ==1 || cnt==0);
			if(cnt == 1) {	
				skill_config->effect_fixpoint.x = content.front();
				content.pop_front();
				skill_config->effect_fixpoint.y = content.front();
				content.pop_front();
			}
			source.clear();
			content.clear();
			std::deque<uint32_t> time_content;
			taomee::get_xml_prop_def(source, cur, "camera_shake_time", "");
			cnt = get_variable_value(1, source, time_content);
			for(; cnt>0; cnt--) {
				skill_config->camera_shake_time.push_back(time_content.front());
				time_content.pop_front();	
			}
			skill_map[skill_config->id] = skill_config;
			KINFO_LOG(0,"LOAD SKILL ID:%d TYPE:%d CD:%d START:%d END:%d", skill_config->id, skill_config->type, skill_config->cool_down, skill_config->effect_time, skill_config->dead_time );
		}
		cur = cur->next;
	}
	KINFO_LOG(0,"--------------------------[END_SKILL_CONFIG]------------------------");
	return 0;
}


Skill_t* SkillConfig::get_skill_info(uint32_t id) 
{
	return skill_map[id] != NULL ? skill_map[id] : NULL;
}

#ifdef FRONT
static const char* TILE_CONFIG_PATH = "spriteBin/";
#else
static const char* TILE_CONFIG_PATH = "./conf/texturemap/";
#endif

TileConfig::TileMap TileConfig::tile_map;

void TileConfig::load_config(uint32_t map_id) {
	if(tile_map.find(map_id) == tile_map.end() ) {
		std::string strFilePath = TILE_CONFIG_PATH;
		std::ostringstream oss;
		oss << strFilePath << map_id << "iPad.map";
		TileWorld<> *p = new TileWorld<>(oss.str().c_str());	
		tile_map[map_id] = p;
	}
}

void TileConfig::clear() {
	TileMap::iterator itr = TileConfig::tile_map.begin();
	for( ; itr!=TileConfig::tile_map.end(); ++itr) {
		delete itr->second;
	}
	TileConfig::tile_map.clear();
}

TileWorld<>* TileConfig::get_tile_info(uint32_t id) {
#ifdef FRONT
	load_config(id);
#endif
	return TileConfig::tile_map[id];
}



void MapContentConfig::load_config(std::string& strFilePath) 
{
	static const char* MAPCONTENT_CONFIG_NAME = "Monster_Basic.xml";
	m_strFilePath = strFilePath + MAPCONTENT_CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

void MapContentConfig::clear()
{
}

int MapContentConfig::on_real_load_config(xmlNodePtr cur) {
	KINFO_LOG(0, "----------------------[BEGIN_MAPCONTENT_CONFIG]-----------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("Monster")))
		{
			uint32_t id = 0;
			uint32_t map_id = 0;
			taomee::get_xml_prop(id, cur, "RaidID");
			taomee::get_xml_prop(map_id, cur, "MapID");
			MapContent_t* mapcontent = g_instance_config->get_mapcontent_info(id,map_id);	
			Instance_t* instance	= g_instance_config->get_instance_info(id);
			assert(mapcontent && instance);	

			MapMonster_t* mapmonster = new MapMonster_t();
			uint32_t group;	
			taomee::get_xml_prop(mapmonster->raid_id, cur, "RaidMonsterID");
			taomee::get_xml_prop(mapmonster->monster_id, cur, "MonsterID");
			taomee::get_xml_prop(mapmonster->monster_value_type, cur, "MonsterType");
			taomee::get_xml_prop(mapmonster->monster_level, cur, "MosterLevel");
			
			assert(g_monster_value_config->get_monster_info(mapmonster->monster_value_type, mapmonster->monster_level));
			assert(g_monster_config->get_monster_info(mapmonster->monster_id));

			taomee::get_xml_prop_def(group, cur, "Group", 0);
			taomee::get_xml_prop_def(mapcontent->monster_group[group].refresh_time, cur, "Rate", 0);
			taomee::get_xml_prop_def(mapcontent->monster_group[group].times, cur, "Turn", 0);

			uint32_t elite;
			taomee::get_xml_prop_def(elite, cur, "Elite", 0);
			if(elite) {
				mapmonster->elite = true;
			} else {
				mapmonster->elite = false;
			}
			uint32_t Friend;
			taomee::get_xml_prop_def(Friend, cur, "friend", 0);
			if(Friend) {
				mapmonster->Friend = true;
			} else {
				mapmonster->Friend = false;
			}
			std::string source;
			std::deque<uint32_t> content;
			taomee::get_xml_prop(source, cur, "MosterBornCoordinate");
			get_variable_value(2,source,content);
			TileWorld<>::Point point_born;
			point_born.x = content.front();
			content.pop_front();
			point_born.y = content.front();
			content.pop_front();

			mapmonster->birth_place = point_born;	

			source.clear();
			content.clear();
			taomee::get_xml_prop(source, cur, "MosterPatrolRange");
			get_variable_value(2,source,content);
			TileWorld<>::Point point_start;
			point_start.x = content.front();
			content.pop_front();
			point_start.y = content.front();
			content.pop_front();
			TileWorld<>::Point point_end;
			point_end.x = content.front();
			content.pop_front();
			point_end.y = content.front();
			content.pop_front();

			mapmonster->patrol_place = TileWorld<>::Rectangle(point_start, point_end);	

			mapcontent->monster_group[group].monsters.push_back(*mapmonster);
			if(mapcontent->monster_group[group].times)
			{
				mapcontent->monster_nums[mapmonster->monster_id] += mapcontent->monster_group[group].times;
				instance->monster_nums[mapmonster->monster_id] += mapcontent->monster_group[group].times;
			} else {
				mapcontent->monster_nums[mapmonster->monster_id] ++;
				instance->monster_nums[mapmonster->monster_id] ++;
			}
			KINFO_LOG(0,"LOAD RAID ID :%d MAP ID:%d HERO BORN:%d MONSTER [ RAID :%d ID:%d LEVEL:%d BORN:(%d %d) PATROL:(%d ,%d)(%d , %d) TURN:%d ", instance->id, mapcontent->map_id, mapcontent->birth_place_gen.size(), mapmonster->raid_id, mapmonster->monster_id, mapmonster->monster_level,point_born.x, point_born.y, point_start.x, point_start.y, point_end.x, point_end.y, mapcontent->monster_group[group].times);
			delete mapmonster;	
		}
		cur = cur->next;
	}
	KINFO_LOG(0,"--------------------------[END_MAPCONTENT_CONFIG]------------------------");
	return 0;
}


RangeConfig::RangeMap RangeConfig::role_range_map;
RangeConfig::OffsetMap RangeConfig::offset_map;

void RangeConfig::load_config(std::string& strFilePath) {
	static const char* RANGE_CONFIG_NAME = "AreaConfigTable.xml";
	m_strFilePath = strFilePath + RANGE_CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_role_config);
}

void RangeConfig::clear() {
	RangeMap::iterator itr = role_range_map.begin();
	for( ; itr!=RangeConfig::role_range_map.end(); ++itr) {
		delete itr->second;
	}
	role_range_map.clear();
	OffsetMap::iterator itr2 = offset_map.begin();
	for(; itr2!=offset_map.end(); ++itr2) {
		delete itr2->second;
	}
	offset_map.clear();
}

int RangeConfig::on_real_load_role_config(xmlNodePtr cur) {
	KINFO_LOG(0, "----------------------[BEGIN_ROLE_RANGE_CONFIG]-----------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("Role")))
		{
			ObjectRange_t* range = new ObjectRange_t();
			taomee::get_xml_prop(range->role_id, cur, "id");
			xmlNodePtr cur2 = cur->xmlChildrenNode;
			while(cur2) {
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("Actions")))
				{
					xmlNodePtr cur3 = cur2->xmlChildrenNode;
					while(cur3) {
						if(!xmlStrcmp(cur3->name, reinterpret_cast<const xmlChar*>("Action"))) {
							ActionRange_t action;
							taomee::get_xml_prop(action.action_id, cur3, "id");

							xmlNodePtr cur4 = cur3->xmlChildrenNode;
							while(cur4) {
								if(!xmlStrcmp(cur4->name, reinterpret_cast<const xmlChar*>("A"))) {
									taomee::get_xml_prop_def(action.attack_start, cur4, "startFrame", 0);
									action.attack_start = 1000 * action.attack_start/24;	
									taomee::get_xml_prop_def(action.attack_end, cur4, "endFrame", 10);
									action.attack_end = 1000 * action.attack_end/24;
									taomee::get_xml_prop(action.attack_range.start.x, cur4, "upLeftPX");
									taomee::get_xml_prop(action.attack_range.start.y, cur4, "upLeftPY");
									taomee::get_xml_prop(action.attack_range.end.x, cur4, "downRightPX");
									taomee::get_xml_prop(action.attack_range.end.y, cur4, "downRightPY");
								}
								if(!xmlStrcmp(cur4->name, reinterpret_cast<const xmlChar*>("C"))) {
									taomee::get_xml_prop(range->body_range.start.x, cur4, "upLeftPX");
									taomee::get_xml_prop(range->body_range.start.y, cur4, "upLeftPY");
									taomee::get_xml_prop(range->body_range.end.x, cur4, "downRightPX");
									taomee::get_xml_prop(range->body_range.end.y, cur4, "downRightPY");
								}
								cur4 = cur4->next;
							}
							range->attack_map[action.action_id] = action;
						}
						cur3 = cur3->next;
					}

				}else if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("PT")))
				{
					ObjectOffset_t* offset = new ObjectOffset_t();
					xmlNodePtr cur3 = cur2->xmlChildrenNode;
					while(cur3) {
						if(!xmlStrcmp(cur3->name, reinterpret_cast<const xmlChar*>("pt"))) {
							uint32_t type;
							taomee::get_xml_prop(type, cur3, "Type");
							if(type == 3) {
								taomee::get_xml_prop(offset->up_offset.x, cur3, "posX");
								taomee::get_xml_prop(offset->up_offset.y, cur3, "posY");
							} else if(type == 4) {
								taomee::get_xml_prop(offset->down_offset.x, cur3, "posX");
								taomee::get_xml_prop(offset->down_offset.y, cur3, "posY");
							}
						}
						cur3 = cur3->next;	
					}
					offset_map[range->role_id] = offset;
				}
				cur2 = cur2->next;
			}
			role_range_map[range->role_id] = range;	
			KINFO_LOG(0,"LOAD RANGE ID:%d BODY:(%d %d) (%d %d)", range->role_id, range->body_range.start.x, range->body_range.start.y, range->body_range.end.x, range->body_range.end.y );
		}
		cur = cur->next;
	}
	KINFO_LOG(0,"--------------------------[END_ROLE_RANGE_CONFIG]------------------------");
	return 0;
}

ActionRange_t* RangeConfig::get_action_range_info(uint32_t role_id, uint32_t action_id) {
	ObjectRange_t* role_range = get_role_range_info(role_id);
	if(role_range !=NULL) {
		if (role_range->attack_map.find(action_id) != role_range->attack_map.end()) {
			return &(role_range->attack_map)[action_id];
		}
	} 
	return NULL;
}

ObjectRange_t* RangeConfig::get_role_range_info(uint32_t id) {
	if(role_range_map.find(id) != role_range_map.end()) {
		return RangeConfig::role_range_map[id];
	} else {
		return NULL;
	}
}

ObjectOffset_t* RangeConfig::get_offset_info(uint32_t id) {
	if(offset_map.find(id) != offset_map.end()) {
		return offset_map[id];
	} else {
		return NULL;
	}
}

/*------------------------------------ MonsterValueConfig ------------------------------*/
MonsterValueConfig::MonsterMap MonsterValueConfig::monster_map;

void MonsterValueConfig::load_config(std::string& strFilePath) {
	static const char* MONSTER_CONFIG_NAME = "Monster_Quality.xml";
	m_strFilePath = strFilePath + MONSTER_CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

void MonsterValueConfig::clear() {
	MonsterMap::iterator itr = monster_map.begin();
	for(; itr!= monster_map.end(); ++itr) {
		delete itr->second;
	}
	monster_map.clear();
}
int MonsterValueConfig::on_real_load_config(xmlNodePtr cur) {
	KINFO_LOG(0, "----------------------[BEGIN_MONSTER_CONFIG]-----------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("Monster")))
		{
			Monster_value_t* monster_config = new Monster_value_t();
			taomee::get_xml_prop(monster_config->id, cur, "ID");
			taomee::get_xml_prop(monster_config->type, cur, "Type");
			taomee::get_xml_prop(monster_config->level, cur, "Level");
			taomee::get_xml_prop(monster_config->physical_attack, cur, "PhyAttack");
			taomee::get_xml_prop(monster_config->magic_attack, cur, "MagAttack");
			taomee::get_xml_prop(monster_config->skill_attack, cur, "SkiAttack");
			taomee::get_xml_prop(monster_config->physical_defence, cur, "PhyDefense");
			taomee::get_xml_prop(monster_config->magic_defence, cur, "MagDefense");
			taomee::get_xml_prop(monster_config->skill_defence, cur, "SkiDefense");
			taomee::get_xml_prop(monster_config->health_point, cur, "HP");
			taomee::get_xml_prop(monster_config->accurate, cur, "Accurate");
			taomee::get_xml_prop(monster_config->dodge, cur, "Dodge");
			taomee::get_xml_prop(monster_config->wreck, cur, "Wreck");
			taomee::get_xml_prop(monster_config->parry, cur, "Parry");
			taomee::get_xml_prop(monster_config->critical_strike, cur, "CriticalStrike");
			taomee::get_xml_prop(monster_config->tenacity, cur, "Tenacity");
			taomee::get_xml_prop(monster_config->slay, cur, "Slay");
			taomee::get_xml_prop_def(monster_config->courage, cur, "Courage", 0);
			taomee::get_xml_prop_def(monster_config->charm, cur, "Charm", 0);
			taomee::get_xml_prop_def(monster_config->trick, cur, "Trick", 0);
			taomee::get_xml_prop_def(monster_config->proficiency, cur, "Proficiency", 0);
			taomee::get_xml_prop_def(monster_config->hp_line, cur, "HP_Line", 0);

			monster_map[ std::pair<uint32_t, uint32_t> (monster_config->type, monster_config->level)] = monster_config;
		}
		cur = cur->next;
	}
	KINFO_LOG(0,"--------------------------[END_SKILL_CONFIG]------------------------");
	return 0;

}

Monster_value_t* MonsterValueConfig::get_monster_info(uint32_t monster_type, uint32_t monster_level) {
	if(monster_map.find(std::pair<uint32_t, uint32_t>(monster_type, monster_level)) != monster_map.end() ) {
		return monster_map[ std::pair<uint32_t, uint32_t>(monster_type, monster_level) ];
	}
	return NULL;
}
	
void MonsterValueConfig::pack_monster_value(CSMonsterValue_t& msg, uint32_t monster_type, uint32_t monster_level) {
	MonsterMap::iterator itr = monster_map.find(std::pair<uint32_t, uint32_t>(monster_type, monster_level));
	if(itr != monster_map.end() ) {
		msg.set_id(itr->second->id);	
		msg.set_type(itr->second->type);	
		msg.set_level(itr->second->level);	
		msg.set_physical_attack(itr->second->physical_attack);	
		msg.set_magic_attack(itr->second->magic_attack);	
		msg.set_skill_attack(itr->second->skill_attack);	
		msg.set_physical_defence(itr->second->physical_defence);	
		msg.set_magic_defence(itr->second->magic_defence);	
		msg.set_skill_defence(itr->second->skill_defence);	
		msg.set_health_point(itr->second->health_point);	
		msg.set_accurate(itr->second->accurate);	
		msg.set_dodge(itr->second->dodge);	
		msg.set_wreck(itr->second->wreck);	
		msg.set_parry(itr->second->parry);	
		msg.set_critical_strike(itr->second->critical_strike);	
		msg.set_tenacity(itr->second->tenacity);	
		msg.set_slay(itr->second->slay);	
		msg.set_courage(itr->second->courage);	
		msg.set_charm(itr->second->charm);	
		msg.set_trick(itr->second->trick);	
		msg.set_proficiency(itr->second->proficiency);	
		msg.set_hp_line(itr->second->hp_line);	
	}
}
	
void MonsterValueConfig::unpack_monster_value(const CSMonsterValue_t& msg) {
	MonsterMap::iterator itr = monster_map.find(std::pair<uint32_t, uint32_t>(msg.type(), msg.level()));
	if(itr != monster_map.end() ) {
		delete itr->second;
	}
	Monster_value_t* monster_config = new Monster_value_t();
	monster_config->id = msg.id(); 
	monster_config->type = msg.type();
	monster_config->level = msg.level();
	monster_config->physical_attack = msg.physical_attack();
	monster_config->magic_attack = msg.magic_attack();
	monster_config->skill_attack = msg.skill_attack();
	monster_config->physical_defence = msg.physical_defence();
	monster_config->magic_defence	= msg.magic_defence();
	monster_config->skill_defence	= msg.skill_defence();
	monster_config->health_point	= msg.health_point();
	monster_config->accurate		= msg.accurate();
	monster_config->dodge			= msg.dodge();
	monster_config->wreck			= msg.wreck();
	monster_config->parry			= msg.parry();
	monster_config->critical_strike	= msg.critical_strike();
	monster_config->tenacity		= msg.tenacity();
	monster_config->slay			= msg.slay();
	monster_config->courage			= msg.courage();
	monster_config->charm			= msg.charm();
	monster_config->trick			= msg.trick();
	monster_config->proficiency		= msg.proficiency();
	monster_config->hp_line			= msg.hp_line();	
	monster_map[ std::pair<uint32_t, uint32_t> (monster_config->type, monster_config->level)] = monster_config;
}	

/*------------------------------------ HeroSkillConfig -------------------------------------*/
HeroSkillConfig::HeroSkillMap HeroSkillConfig::hero_skill_map;
HeroSkillConfig::SkillInfoMap HeroSkillConfig::skill_info_map;
HeroSkillConfig::SkillInfoMap HeroSkillConfig::link_skill_info_map;
void HeroSkillConfig::load_config(std::string& strFilePath)
{
	static const char* CONFIG_NAME = "skills.xml";
	m_strFilePath = strFilePath + CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}
void HeroSkillConfig::clear()
{
	hero_skill_map.clear();
	skill_info_map.clear();
}

int HeroSkillConfig::on_real_load_config(xmlNodePtr cur)
{
	KINFO_LOG(0, "------------------[HERO_SKILL_CONFIG]------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("skill"))) {
			xmlNodePtr cur2 = cur->xmlChildrenNode;
			HeroSkill_t hero_skill;
			while(cur2) {
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("skill_id"))) {
					xmlChar *skill_id =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(skill_id));
					iss >> hero_skill.skill_id;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("role_type"))) {
					xmlChar *role_type =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(role_type));
					iss >> hero_skill.role_type;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("part"))) {
					xmlChar *part =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(part));
					iss >> hero_skill.skill_part;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("skill_type"))) {
					xmlChar *skill_type =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(skill_type));
					iss >> hero_skill.skill_type;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("link_skill"))) {
					xmlChar *link_skill =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(link_skill));
					iss >> hero_skill.link_skill_id;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("is_buff"))) {
					xmlChar *buff =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(buff));
					iss >> hero_skill.buff;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("is_invincible"))) {
					xmlChar *invincible =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(invincible));
					iss >> hero_skill.invincible;
				}
				if(!xmlStrcmp(cur2->name, reinterpret_cast<const xmlChar*>("transparency"))) {
					xmlChar *transparency =xmlNodeGetContent(cur2);					
					std::istringstream iss(reinterpret_cast<const char*>(transparency));
					uint32_t value;
					iss >> value;
					if(value) {
						hero_skill.stealth= true;
						INFO_LOG("HERO SKILL:%d STEALTH",hero_skill.skill_id);
					}
				}
				cur2 = cur2->next;	
			}
			if(hero_skill.skill_part == 1) 
				hero_skill_map[hero_skill.role_type].push_back(hero_skill);
			skill_info_map[hero_skill.skill_id] = hero_skill;
			if (hero_skill.link_skill_id) {
				link_skill_info_map[hero_skill.link_skill_id] = hero_skill;
			}
		}
		cur = cur->next;
	}
	KINFO_LOG(0, "------------------[HERO_SKILL_CONFIG]------------------");
	return 0;
}

HeroSkillList HeroSkillConfig::get_hero_skill_list(uint32_t type)
{
	HeroSkillMap::iterator it = hero_skill_map.find(type%2?type:type-1);
	if (it != hero_skill_map.end()) {
		return it->second;
	} else {
		return HeroSkillList();
	}
}
	
HeroSkill_t *HeroSkillConfig::get_hero_skill_info(uint32_t skill_id)
{
	SkillInfoMap::iterator itr = skill_info_map.find(skill_id);
	return itr != skill_info_map.end()? &(itr->second) : NULL;
}

HeroSkill_t *HeroSkillConfig::get_first_part_skill_info(uint32_t skill_id)
{
	SkillInfoMap::iterator itr = link_skill_info_map.find(skill_id);
	if (itr != link_skill_info_map.end()) {
		return get_first_part_skill_info(itr->second.skill_id);
	} else {
		itr = skill_info_map.find(skill_id);
		return itr != skill_info_map.end()? &(itr->second) : NULL;
	}
}

bool HeroSkillConfig::is_buff(uint32_t skill_id) {
	SkillInfoMap::iterator itr = skill_info_map.find(skill_id);
	return itr != skill_info_map.end()? itr->second.buff : false;
}

bool HeroSkillConfig::is_invincible(uint32_t skill_id) {
	SkillInfoMap::iterator itr = skill_info_map.find(skill_id);
	return itr != skill_info_map.end()? itr->second.invincible : false;
}

bool HeroSkillConfig::is_stealth(uint32_t skill_id) {
	SkillInfoMap::iterator itr = skill_info_map.find(skill_id);
	return itr != skill_info_map.end()? itr->second.stealth : false;
}

/*------------------------------------ SkillHurtConfig -------------------------------------*/
SkillHurtConfig::SkillHurtMap SkillHurtConfig::skill_hurt_map;
SkillHurtConfig::SkillHurtMax SkillHurtConfig::skill_hurt_max;

void SkillHurtConfig::load_config(std::string& strFilePath)
{
	static const char* CONFIG_NAME = "Skill_Hurt.xml";
	m_strFilePath = strFilePath + CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}
void SkillHurtConfig::clear()
{
	SkillHurtMap::iterator itr = skill_hurt_map.begin();
	for(; itr!= skill_hurt_map.end(); ++itr) {
		delete itr->second;
	} 
	skill_hurt_map.clear();
	skill_hurt_max.clear();
}

int SkillHurtConfig::on_real_load_config(xmlNodePtr cur)
{
	KINFO_LOG(0, "------------------[SKILL_HURT_CONFIG]------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("Skill_Hurt"))) {
			SkillHurt_t *skill_hurt_config = new SkillHurt_t();
			uint32_t skill_id, skill_level;
			taomee::get_xml_prop(skill_id, cur, "Skill_ID");
			taomee::get_xml_prop(skill_level, cur, "Skill_Level");
			if(skill_hurt_map.find(std::pair<uint32_t , uint32_t>(skill_id, skill_level)) == skill_hurt_map.end()) {
				skill_hurt_config = new SkillHurt_t();
				skill_hurt_config->skill_id = skill_id;
				skill_hurt_config->skill_level 	= skill_level;
				skill_hurt_map[std::pair<uint32_t, uint32_t>(skill_hurt_config->skill_id, skill_hurt_config->skill_level)] = skill_hurt_config;
			} else {
				skill_hurt_config = skill_hurt_map[std::pair<uint32_t, uint32_t>(skill_id, skill_level)];
			}
			taomee::get_xml_prop(skill_hurt_config->level, cur, "Level");
			taomee::get_xml_prop(skill_hurt_config->money, cur, "Money");
			taomee::get_xml_prop(skill_hurt_config->exploit, cur, "Exploit");
			taomee::get_xml_prop(skill_hurt_config->own_cd, cur, "Own_CD");
			taomee::get_xml_prop(skill_hurt_config->share_cd, cur, "Share_CD");
			SkillPart_t * skill_part = new SkillPart_t();
			taomee::get_xml_prop(skill_part->part, cur, "Part");
			taomee::get_xml_prop(skill_part->continue_time, cur, "Continue_Time");
			taomee::get_xml_prop(skill_part->continue_time2, cur, "Continue_Time2");
			taomee::get_xml_prop(skill_part->can_attack_number, cur, "Can_Attack_Number");
			taomee::get_xml_prop(skill_part->hurt, cur, "Hurt");
			taomee::get_xml_prop(skill_part->physical_attack, cur, "PhyAttack");
			taomee::get_xml_prop(skill_part->magic_attack, cur, "MagAttack");
			taomee::get_xml_prop(skill_part->skill_attack, cur, "SkiAttack");
			taomee::get_xml_prop(skill_part->physical_defence, cur, "PhyDefense");
			taomee::get_xml_prop(skill_part->magic_defence, cur, "MagDefense");
			taomee::get_xml_prop(skill_part->skill_defence, cur, "SkiDefense");
			taomee::get_xml_prop(skill_part->health_point, cur, "HP");
			taomee::get_xml_prop(skill_part->accurate, cur, "Accurate");
			taomee::get_xml_prop(skill_part->dodge, cur, "Dodge");
			taomee::get_xml_prop(skill_part->wreck, cur, "Wreck");
			taomee::get_xml_prop(skill_part->parry, cur, "Parry");
			taomee::get_xml_prop(skill_part->critical_strike, cur, "CriticalStrike");
			taomee::get_xml_prop(skill_part->tenacity, cur, "Tenacity");
			taomee::get_xml_prop(skill_part->slay, cur, "Slay");
			taomee::get_xml_prop(skill_part->speed, cur, "Speed");
			skill_hurt_config->parts.push_back(*skill_part);
			if(skill_hurt_max[skill_id]< skill_level) skill_hurt_max[skill_id] = skill_level;
			delete skill_part;
		}
		cur = cur->next;
	}
	KINFO_LOG(0, "------------------[SKILL_HURT_CONFIG]------------------");
	return 0;
}

SkillHurt_t* SkillHurtConfig::get_hurt_info(uint32_t skill_id, uint32_t level)
{
	SkillHurtMap::iterator it = skill_hurt_map.find(std::pair<uint32_t, uint32_t>(skill_id, level));
	if (it != skill_hurt_map.end()) {
		return it->second;
	} else {
		return NULL;
	}
}

uint32_t SkillHurtConfig::get_hurt_max_level(uint32_t skill_id)
{
	SkillHurtMax::iterator itr = skill_hurt_max.find(skill_id);
	return itr!=skill_hurt_max.end()?itr->second:0;
}

void SkillHurtConfig::pack_diff(uint32_t id, uint32_t level, CSUserSkillDiffRsp* rsp)
{
	SkillHurt_t* now = get_hurt_info(id, level);
	rsp->set_skill_id(id);
	rsp->set_skill_level(level);
	if(now) {
		rsp->set_money(now->money);
		rsp->set_exploit(now->exploit);
		rsp->set_lv(now->level);	
		BasicUserBattleInfo* info = rsp->mutable_battle_info();
		info->set_physical_attack(now->parts[0].physical_attack); 
		info->set_magic_attack(now->parts[0].magic_attack); 
		info->set_skill_attack(now->parts[0].skill_attack); 
		info->set_physical_defence(now->parts[0].physical_defence); 
		info->set_magic_defence(now->parts[0].magic_defence); 
		info->set_skill_defence(now->parts[0].skill_defence); 
		info->set_health_point(now->parts[0].health_point); 
		info->set_accurate(now->parts[0].accurate); 
		info->set_dodge(now->parts[0].dodge); 
		info->set_wreck(now->parts[0].wreck); 
		info->set_parry(now->parts[0].parry); 
		info->set_critical_strike(now->parts[0].critical_strike); 
		info->set_tenacity(now->parts[0].tenacity); 
		info->set_slay(now->parts[0].slay);
		info->set_courage(0);
		info->set_charm(0);	
		info->set_trick(0); 
		info->set_speed(now->parts[0].speed);
		info->set_proficiency(0);
		rsp->set_own_cd((int)now->own_cd);
		rsp->set_share_cd((int)now->share_cd);
		rsp->set_hurt(now->parts[0].hurt); 
		rsp->set_continue_time(now->parts[0].continue_time); 
	}
}
	
void SkillHurtConfig::unpack_hero_skill(const UserSkillInfo& msg)
{
	SkillHurt_t *skill_hurt_config = new SkillHurt_t();
	skill_hurt_config->skill_id = msg.skill_id();
	skill_hurt_config->level	= msg.level();
	skill_hurt_config->part		= msg.part();
	skill_hurt_config->skill_level = msg.skill_level();
	skill_hurt_config->money		= msg.money();
	skill_hurt_config->exploit		= msg.exploit();
	skill_hurt_config->own_cd		= msg.own_cd();
	skill_hurt_config->share_cd		= msg.share_cd();
	for(int i=0; i<msg.parts_size(); ++i) {
		const UserSkillPart& skill_part_msg = msg.parts(i);
		SkillPart_t skill_part;
		skill_part.part = skill_part_msg.part();
		skill_part.continue_time = skill_part_msg.continue_time();
		skill_part.continue_time2 = skill_part_msg.continue_time2();
		skill_part.can_attack_number = skill_part_msg.can_attack_number();
		skill_part.hurt = skill_part_msg.hurt();
		const BasicUserBattleInfo& exp_info =  skill_part_msg.exp_battle_info();
		skill_part.physical_attack = exp_info.physical_attack();
		skill_part.magic_attack = exp_info.magic_attack();
		skill_part.skill_attack = exp_info.skill_attack();
		skill_part.physical_defence = exp_info.physical_defence();
		skill_part.magic_defence = exp_info.magic_defence();
		skill_part.skill_defence = exp_info.skill_defence();
		skill_part.health_point = exp_info.health_point();
		skill_part.accurate		= exp_info.accurate();
		skill_part.dodge		= exp_info.dodge();
		skill_part.wreck		= exp_info.wreck();
		skill_part.parry		= exp_info.parry();
		skill_part.critical_strike = exp_info.critical_strike();
		skill_part.tenacity = exp_info.tenacity();
		skill_part.slay		= exp_info.slay();
		skill_part.speed	= exp_info.speed();
		skill_hurt_config->parts.push_back(skill_part);
	}
	SkillHurtMap::iterator it = skill_hurt_map.find(std::pair<uint32_t, uint32_t>(skill_hurt_config->skill_id, skill_hurt_config->skill_level));
	if (it != skill_hurt_map.end()) {
		delete it->second;
	}
	skill_hurt_map[std::pair<uint32_t, uint32_t>(skill_hurt_config->skill_id, skill_hurt_config->skill_level)] = skill_hurt_config;	
}

/*------------------------------------ FairySkillConfig -------------------------------------*/
FairySkillConfig::FairySkillMap FairySkillConfig::fairy_skill_map;

void FairySkillConfig::load_config(std::string& strFilePath)
{
	static const char* CONFIG_NAME = "fairyskill.xml";
	m_strFilePath = strFilePath + CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

void FairySkillConfig::clear()
{
	FairySkillMap::iterator it = fairy_skill_map.begin();
	for(; it != fairy_skill_map.end(); ++it) {
		delete it->second;
	}
	fairy_skill_map.clear();
}

int FairySkillConfig::on_real_load_config(xmlNodePtr cur)
{
	INFO_LOG("------------------[Begin FAIRY SKILL CONFIG]------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("fairyskill"))) {
			FairySkill_t *conf = new FairySkill_t();
			taomee::get_xml_prop(conf->id, cur, "skill_ID");
			taomee::get_xml_prop(conf->type, cur, "skill_type");
			taomee::get_xml_prop(conf->continue_time, cur, "continue_time");
			taomee::get_xml_prop(conf->hurt, cur, "hurt");
			taomee::get_xml_prop(conf->hurt_add, cur, "hurt_add");
			conf->continue_time = conf->continue_time/1000;
			taomee::get_xml_prop(conf->fight_prop.physical_attack, cur, "PhyAttack");
			taomee::get_xml_prop(conf->fight_prop.magic_attack, cur, "MagAttack");
			taomee::get_xml_prop(conf->fight_prop.skill_attack, cur, "SkiAttack");
			taomee::get_xml_prop(conf->fight_prop.physical_defence, cur, "PhyDefense");
			taomee::get_xml_prop(conf->fight_prop.magic_defence, cur, "MagDefense");
			taomee::get_xml_prop(conf->fight_prop.skill_defence, cur, "SkiDefense");
			taomee::get_xml_prop(conf->fight_prop.health_point, cur, "HP");
			taomee::get_xml_prop(conf->fight_prop.accurate, cur, "Accurate");
			taomee::get_xml_prop(conf->fight_prop.dodge, cur, "Dodge");
			taomee::get_xml_prop(conf->fight_prop.wreck, cur, "Wreck");
			taomee::get_xml_prop(conf->fight_prop.parry, cur, "Parry");
			taomee::get_xml_prop(conf->fight_prop.critical_strike, cur, "CriticalStrike");
			taomee::get_xml_prop(conf->fight_prop.tenacity, cur, "Tenacity");
			taomee::get_xml_prop(conf->fight_prop.slay, cur, "Slay");

			fairy_skill_map[conf->id] = conf;
		}
		cur = cur->next;
	}
	INFO_LOG("------------------[END FAIRY SKILL CONFIG]------------------");
	return 0;
}
	
FairySkill_t* FairySkillConfig::get_fairy_skill(uint32_t skill_id)
{
	FairySkillMap::iterator it = fairy_skill_map.find(skill_id);
	if (it != fairy_skill_map.end()) {
		return it->second;
	} else {
		return NULL;
	}
}
	
bool FairySkillConfig::is_buff(uint32_t skill_id)
{
	FairySkill_t* skill = get_fairy_skill(skill_id);
	if(skill && skill->type == FairySkillBuff) {
		return true;
	}
	return false;
}
	
uint32_t FairySkillConfig::add_blood(uint32_t skill_id,uint32_t skill_attack)
{
	FairySkill_t* skill = get_fairy_skill(skill_id);
	if(skill && skill->type == FairySkillBuff) {
		return skill->fight_prop.health_point * skill_attack/100;
	}
	return 0;
}

/*------------------------------------ SkillUnlockConfig -------------------------------------*/

void SkillUnlockConfig::load_config(std::string& strFilePath) {
	static const char* CONFIG_NAME = "skills_ui.xml";
	m_strFilePath = strFilePath + CONFIG_NAME;
	taomee::load_xmlconf(m_strFilePath.c_str(), on_real_load_config);
}

int SkillUnlockConfig::on_real_load_config(xmlNodePtr cur) {
	INFO_LOG("------------------[Begin SKILL UNLOCK CONFIG]------------------");
	cur = cur->xmlChildrenNode;
	while(cur) {
		if(!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar*>("skill_ui"))) {
			uint32_t skill_id;
			taomee::get_xml_prop(skill_id, cur, "skill_ID");
			SkillHurt_t* skill_hurt = g_skill_hurt_config->get_hurt_info(skill_id, 1);
			if(skill_hurt) {
				taomee::get_xml_prop(skill_hurt->level, cur, "unlock_level");
			}
		}
		cur = cur->next;
	}
	INFO_LOG("------------------[END SKILL UNLOCK CONFIG]------------------");
	return 0;
}





