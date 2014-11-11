#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

// 3rd-party lib headers go here
#include <google/protobuf/message.h>
// project-specific headers go here
#ifdef FRONT
#include "../frontend/virtualplayer.h"
#else
#include "../entities/player.h"
#endif
#include "../entities/monster.h"
#include "../common/fwd_decl.h"
#include "../base/Manager.hpp"
#include "../base/EventableObject.hpp"
#include "../base/Singleton.hpp"
#include "../base/Singleton.hpp"
#include "../proto/protocodec.h"
#include "battlefield.h"

#define JOINTWAITTIMEOUT (15)

#define LOADWAITTIMEOUT (30)

#define GAMEBEGIN_BATTLE (280000)


class Instance: public base::Manager<Player>, public base::EventableObject {
public:
	Instance(uint32_t instance_id, InstanceType type, uint32_t timeout, uint32_t star_time);

	void player_join(Player* pPlayer);

	int init();
	
	int join_timeout(uint32_t& map_id);

	int start_battle_timeout(uint32_t& map_id);
	
	int start_battle(uint32_t& map_id);
	
	void transfer_next(Player* pPlayer);

	void on_player_leave(base::Manager<Player>*, Player* pPlayer);	
	
	void on_player_dead(Entity* pPlayer);	
	
	void on_player_hp_change(Entity* pPlayer, uint32_t hp, Entity* source);	

	void on_battle_leave(base::Object<uint32_t>* pbattle);

	void load_complete(Player* pPlayer);

	int dead_by_timeout();

	void on_player_leave_battle(base::Manager<Player>*, Player*);
	
	void player_hp_change(Player* pPlayer, int blood);

	bool is_pve();

private:
	void finish_pve_battle();
	void finish_pvp_battle();
	void finish_pvai_battle();
	void finish_pvp2_battle();
	void finish_pvp3_battle();
	void finish_pvp4_battle();

private:
	uint32_t m_instance_id;

	uint32_t m_current_map_id;	

	base::Manager<Battlefield> m_battlefields;

typedef std::map<Player*, bool> CompleteMap;
	CompleteMap m_user_complete;

	InstanceType m_type;

	uint32_t m_timeout;

	uint32_t m_finish_time;
	
	uint32_t m_star_time;

	uint32_t m_dead_mon_num;

	base::Time m_start_time;

	base::EventHandler m_timeout_handler;
	
	base::EventHandler m_start_timeout_handler;
	
	base::EventHandler m_join_timeout_handler;

	Player* m_p_ai;
};

class InstanceMgr: public base::GManager<Player>, public Singleton <InstanceMgr> 
{
public:
	int create_instance(Player* pPlayer, uint32_t instance_id);

	int load_complete(Player* pPlayer);
	
	int transfer_next(Player* pPlayer);

	int join_instance(Player* pPlayer, Player* owner);
	
	int join_instance(Player* pPlayer, Player* owner, uint32_t side);
	
	int init_instance(Player* owner);

	int leave_instance(Player* pPlayer);
	
	int player_hp_change(Player* pPlayer, int blood);

	bool in_pve(Player* pPlayer);

};

#define	sInstance InstanceMgr::get_singleton()

#endif
