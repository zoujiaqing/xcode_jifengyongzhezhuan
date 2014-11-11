#ifndef RU_BATTLEFIELD_H_
#define RU_BATTLEFIELD_H_

// 3rd-party lib headers go here
#include <google/protobuf/message.h>
// project-specific headers go here
#include "../entities/monster.h"
#ifdef FRONT
#include "../frontend/virtualplayer.h"
#else 
#include "../entities/aiplayer.h"
#include "../entities/player.h"
#endif
#include "../common/fwd_decl.h"
#include "../base/Manager.hpp"
#include "../base/EventableObject.hpp"
#include "../base/Singleton.hpp"
#ifndef FRONT
#include "../base/GroupViewer.hpp"
#endif
#include "../proto/protocodec.h"
#include "../battlefields/tileworld.hpp"
#include "../entities/skill.h"


#include <deque>
#define WORLDWIDE (0)

#define WAITTIMEOUT (5)



using namespace taomee;

/*
class BattleKey {
public:
typedef uint32_t IdType;
	Player::IdType player_id;
	IdType battle_id;
	bool operator< (const BattleKey& key) const {
		return (player_id < key.player_id || (player_id == key.player_id  && battle_id < key.battle_id ));
	};

	bool operator== (const BattleKey& key) const {
		return (player_id == key.player_id && battle_id == key.battle_id);
	};
};*/

/**
 * @brief 战场
 */
class Battlefield: public base::Object<uint32_t>, public base::Manager<Player>, public base::EventableObject {
public:
	typedef uint32_t Key;

	typedef uint32_t IdType;

	Battlefield(Player* pPlayer, uint32_t battle_id, uint32_t map_id = 0, bool transfer = false);

	virtual void player_join(Player* pPlayer);

	virtual void load_complete(Player* pPlayer);

	virtual void attack(Player *pPlayer, const CSPlayerAttackReq * req);
	
	virtual void skill_begin(Player *pPlayer, const CSSkillBeginReq * req);

	virtual void on_buffer_finish(Entity* entity);

	void on_buffer_finish(Player* pPlayer);

	virtual void move(Player* pPlayer, const CSBattleMoveReq * req);
	
	void monster_die(Player* pPlayer, const CSMonsterDieReq *req);

	virtual int update();

	virtual void update_effect();

	virtual void on_player_leave(base::Manager<Player>*, Player*);

	virtual	void on_player_dead(Entity* pPlayer);

	void send_to_players(const google::protobuf::Message& msg);
	
	void send_to_players_except(const google::protobuf::Message& msg, Player* pPlayer);

	void init_map(uint32_t map_id = 0 );
	
	void check_wait_group();

	uint32_t generate_monster_id();
	
	void dynamic_add_monster(MapMonsterGroup_t group);

	int add_wait_group(MapMonsterGroup_t&, uint32_t& times);

	virtual void pack_main_info(CSBattleCreateRsp* rsp);

	virtual	bool monster_clear();

	virtual void revival(Player *pPlayer);

	int server_update();

public:
	uint32_t get_map_id();
	
	uint32_t get_next_map_id();

	IdType get_battle_id();	

	Player* get_owner();

	bool get_complete();

	virtual bool set_complete();

	void init_defence_map();
	
	void init_defence_map_server();

	uint32_t get_dead_mon_num();

protected:
	MonsterManager monsters_;

	IdType  m_battle_id;

	TileWorld<>* m_tile_world;

	Player* owner;
	
	std::vector< TileWorld<>::Point> m_hero_born;
	
	std::vector< TileWorld<>::Point> m_monster_born;

	EffectManager m_effect_manager;

	std::deque<MapMonsterGroup_t> m_wait_group;

	base::EventHandler m_wait_handler;
	
/*instance info*/	
	TileWorld<>::Point m_portal;

	uint32_t m_map_id;

	uint32_t m_next_map_id;

	bool m_complete;

	bool m_transfer;

	uint32_t m_dead_mon_num;

};

class CDEFBattle: public Battlefield {
public:
	CDEFBattle(Player* pPlayer, uint32_t battle_id, uint32_t map_id);
	bool monster_clear();
};

class PVPBattle: public Battlefield {
public:
	PVPBattle(Player* pPlayer, uint32_t battle_id, uint32_t map_id);

	typedef uint32_t Key;

	typedef uint32_t IdType;
	
	void player_join(Player* pPlayer);

	int update();
	
	void on_player_leave(base::Manager<Player>*, Player*);

	void attack(Player *pPlayer, const CSPlayerHitPlayerReq * req);

public:
	void set_mode(uint32_t mode) { mode_ = mode; }
	uint32_t group_total_hp1() { return group_total_hp1_; }
	uint32_t group_total_hp2() { return group_total_hp2_; }
private:
	uint32_t mode_;
	std::set<uint32_t> dead_player_;
	uint32_t group_total_hp1_;
	uint32_t group_total_hp2_;
};

#ifndef FRONT
class AiBattle: public Battlefield {
public:
    AiBattle(Player* pPlayer, uint32_t battle_id, uint32_t map_id);
	
	void player_join(Player* pPlayer);
	
	void on_player_dead(Entity* pPlayer);
	
	void load_complete(Player* pPlayer);
	
	void pack_main_info(CSBattleCreateRsp* rsp);

    typedef uint32_t Key;

    typedef uint32_t IdType;

    int update();
	
	void on_player_leave(base::Manager<Player>*, Player*);
	
	void attack(Player *pPlayer, const CSPlayerHitPlayerReq * req);

	void skill_begin(Player *pPlayer, const CSSkillBeginReq * req);

	uint32_t get_ai_hp_percent();
	
	void set_ai_hp(uint32_t hp);
	
	const AIPlayer &ai() { return m_ai; }
private:
	AIPlayer m_ai;
	UserID_t m_ai_id;
	uint32_t m_ai_reg_time;
	uint32_t m_ai_channel_id;
	std::string m_ai_name;
};
// 无奖励 排名 cd间隔的pvai
class AiBattle2 : public AiBattle {
public:
	AiBattle2(Player* pPlayer, uint32_t battle_id, uint32_t map_id);
	int update();
	void on_player_leave(base::Manager<Player>*, Player*);
};

#endif

#ifndef FRONT
#define WBOSS_REVIVAL_CD		20
#define WBOSS_UNTOUCH_CD		10
class WBossBattle : public Battlefield {
public:
    WBossBattle(uint32_t battle_id);
	void player_join(Player* pPlayer);
	void load_complete(Player* pPlayer);
	void attack(Player *pPlayer, const CSPlayerAttackReq *req);
	void skill_begin(Player *pPlayer, const CSSkillBeginReq * req);
	int update();
	void on_buffer_finish(Entity* entity);
	void move(Player* pPlayer, const CSBattleMoveReq * req);
	void update_effect(); 
	void on_player_leave(base::Manager<Player>*, Player*);
	void on_player_dead(Entity* pPlayer);
	int sync_boss_info();
	void revival(Player *pPlayer);
	void revival_by_cd_over(Player *pPlayer);
	void pack_main_info(CSBattleCreateRsp* rsp, Player *pPlayer);
public:
	bool set_complete();
	uint64_t monsters_total_hp() { return monsters_total_hp_; }
private:
	int revival_timeout(Player *& pPlayer);
private:
	typedef std::map<Player*, uint32_t> players_load_complete_t;
	players_load_complete_t players_load_complete_;

	uint64_t monsters_total_hp_;
};


#define BBOSS_REVIVAL_CD		20
#define BBOSS_UNTOUCH_CD		10

#define BBOSS_DEAD_TIME         (60)
class BabelBattle : public Battlefield {
public:
    BabelBattle(uint32_t battle_id);
	void player_join(Player* pPlayer);
	void load_complete(Player* pPlayer);
	void attack(Player *pPlayer, const CSPlayerAttackReq *req);
	void skill_begin(Player *pPlayer, const CSSkillBeginReq * req);
	int update();
	void on_buffer_finish(Entity* entity);
	void move(Player* pPlayer, const CSBattleMoveReq * req);
	void update_effect(); 
	void on_player_leave(base::Manager<Player>*, Player*);
	void on_player_dead(Entity* pPlayer);
	int sync_boss_info();
	void pack_main_info(CSBattleCreateRsp* rsp, Player *pPlayer);
public:
	bool set_complete();
	uint64_t monsters_total_hp() { return monsters_total_hp_; }
private:
	typedef std::map<Player*, uint32_t> players_load_complete_t;
	players_load_complete_t players_load_complete_;

	uint64_t monsters_total_hp_;
};

#endif

class BattlefieldManager: public base::GManager<Player>, public Singleton <BattlefieldManager> {
public:
	BattlefieldManager();

	Battlefield* create_battle(Player* pPlayer, uint32_t battle_id, uint32_t map_id = 0, bool transfer = false);

#ifndef FRONT
	WBossBattle* create_wboss_battle(uint32_t battle_id);
	int wb_btl_revival(Player* pPlayer);

	BabelBattle* create_babel_battle(uint32_t battle_id);
#endif

	Battlefield* get_battle(Player* pPlayer);

	int leave_battle(Player* pPlayer, bool rsp=true);
	
	int wb_leave_battle(Player* pPlayer);

	int bb_leave_battle(Player* pPlayer);

	int player_attack(Player* pPlayer, const CSPlayerAttackReq * req);
	
	int player_attack(Player* pPlayer, const CSPlayerHitPlayerReq * req);
	
	int skill_begin(Player* pPlayer, const CSSkillBeginReq * req);

	int move(Player* pPlayer, const CSBattleMoveReq * req);

	int monster_die(Player* pPlayer, const CSMonsterDieReq *req);
	
	int revival(Player* pPlayer);

	bool exist(const Player* pPlayer);

	uint32_t map_id(Player* pPlayer);
	
	uint32_t set_pvp_mode(Player* pPlayer, uint32_t mode);
};

#define	sBattlefield BattlefieldManager::get_singleton()

#endif // RU_BATTLEFIELD_H_
