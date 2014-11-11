#include "monster.h"
#include "boss.h"
#include <assert.h>
#include <math.h>
#include "buffeffect.h"
	
Hatred::Hatred():
	m_cool_time(0),
	m_current(NULL)
{

}
	
void Hatred::add(Entity* entity, uint32_t value) {
	m_hatred_map[entity] += value;
    if(entity->is_city()) {
        m_current = entity;
    }
}
	
void Hatred::on_player_leave(base::Object<player_key_t>* pPlayer) {
	m_hatred_map.erase(static_cast<Entity*>(pPlayer));
	if(m_current == pPlayer) {
		m_current = NULL;
	}
}
	
bool Hatred::exist(Entity* entity) {
	return m_hatred_map.find(entity) != m_hatred_map.end();
}
	
bool Hatred::empty() {
	return m_hatred_map.empty();
}
	
Entity* Hatred::get_current(bool update) {
	if(m_current && m_current->m_fightprop.stealth) {
		m_hatred_map.erase(m_current);
		m_current = NULL;
	}
	if(!m_current) {
		uint32_t max_hatred = 0;
		std::map<Entity*, uint32_t>::iterator itr = m_hatred_map.begin();
		for(; itr!= m_hatred_map.end(); ++itr) {
			if(itr->second > max_hatred) {
				max_hatred = itr->second;
				m_current = itr->first;
			}
		}
		m_cool_time = HATRED_FREQUENCY; 
		return m_current;
	} else if(m_cool_time) {
		if(update)
			m_cool_time--;
		return m_current;
	} else if(m_current->is_city()) {
        return m_current;
    } else {
		uint32_t max_hatred = m_hatred_map[m_current] * 1.1;
		std::map<Entity*, uint32_t>::iterator itr = m_hatred_map.begin();
		for(; itr!= m_hatred_map.end(); ++itr) {
			if(itr->second > max_hatred) {
				max_hatred = itr->second;
				m_current = itr->first;
				m_cool_time = HATRED_FREQUENCY;
			}
		}
		return m_current;
	}
}
	
Monster::Monster(UserID_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
	Entity(player_key_t(random_id, 0, 0),MONSTER_DEFAULT_GROUP),
	m_skill_control(tileworld),
//	m_current_pskill(NULL),
	m_accumulate(0),
	m_pworld(tileworld),
	m_target_point(-1,-1),
	m_effect_manager(effect_manager),
	m_idle_time(0)
{
}
		
Monster::~Monster() {
//	delete m_current_pskill;
}
	
void Monster::pack_battle_info(MonsterBattleInfo* info)
{
	info->set_physical_attack(m_fightprop.physical_attack);
	info->set_skill_attack(m_fightprop.skill_attack);
	info->set_magic_attack(m_fightprop.magic_attack);
	info->set_physical_defence(m_fightprop.physical_defence);
	info->set_magic_defence(m_fightprop.magic_defence);
	info->set_skill_defence(m_fightprop.skill_defence);
	info->set_health_point(m_prop.health_point);
	info->set_accurate(m_fightprop.accurate);
	info->set_dodge(m_fightprop.dodge);
	info->set_wreck(m_fightprop.wreck);
	info->set_parry(m_fightprop.parry);
	info->set_critical_strike(m_fightprop.critical_strike);
	info->set_tenacity(m_fightprop.tenacity);
	info->set_slay(m_fightprop.slay);
	info->set_proficiency(m_fightprop.proficiency);
}

void Monster::init_info(const Monster_t& info, const MapMonster_t& map_info) {
	m_prop.experience = info.experience;	
	m_prop.speed = info.speed;	
	type_id_ = info.id;
	boss_ 	 = info.is_boss;

	ObjectRange_t* range  = g_range_config->get_role_range_info(type_id_);
    if(range) {
        m_prop.space = range->body_range;
    } else {
        ERROR_LOG("ERROR MONSTER TYPE [type=%u]", get_type_id());
    }
//	m_current_pskill = Skill::generate_skill(type_id_, info.skills[0]);	
	m_skill_control.generate_skills(info);
	
	level_ = map_info.monster_level;
	Monster_value_t* monster_value = g_monster_value_config->get_monster_info(map_info.monster_value_type, map_info.monster_level);
	if (!monster_value) {
		ERROR_RETURN_VOID("ASSERT not found Monster_value_t! type:%u level:%u", 
				map_info.monster_value_type, map_info.monster_level);
	}
	m_fightprop.physical_attack = monster_value->physical_attack + monster_value->courage * 0.5;
	m_fightprop.magic_attack = monster_value->magic_attack + monster_value->charm * 0.5;
	m_fightprop.skill_attack = monster_value->skill_attack + monster_value->trick * 0.5;
	m_fightprop.physical_defence = monster_value->physical_defence + monster_value->courage * 0.15;
	m_fightprop.magic_defence = monster_value->magic_defence + monster_value->charm * 0.15;
	m_fightprop.skill_defence = monster_value->skill_defence + monster_value->trick * 0.15;
	m_prop.health_point  = monster_value->health_point;
	m_fightprop.health_point  = monster_value->health_point;
	if (!m_prop.health_point) {
		ERROR_RETURN_VOID("ASSERT health_point is 0");
	}
	m_fightprop.accurate = monster_value->accurate;
	m_fightprop.dodge = monster_value->dodge;
	m_fightprop.wreck = monster_value->wreck;
	m_fightprop.parry = monster_value->parry;
	m_fightprop.critical_strike = monster_value->critical_strike;
	m_fightprop.tenacity = monster_value->tenacity;
	m_fightprop.slay = monster_value->slay;
	m_fightprop.proficiency = monster_value->proficiency;
	
	/*temp code */
	m_fightprop.vision = 20;

	hp_line_ = monster_value->hp_line;

    city_ = info.is_city;

	m_pos = map_info.birth_place;
	m_birth_pos = map_info.birth_place;
	m_patrol = map_info.patrol_place;
	stat_ = PATROL;
	elite_ = map_info.elite;
	friend_ = map_info.Friend;
    m_buffmanager->load_prop(this);
}
	
Monster::MonsterStat Monster::get_stat() {
	return stat_;
}

void Monster::dead() {
	delete this;
}
	
bool Monster::patrol() {
	if(stat_ == PATROL) {
		if(stop()) {
			TileWorld<>::Point end = m_patrol.get_one_path(m_pos);
			//DEBUG_LOG("FIND PATROL ID:%d :(%d , %d) TO (%d, %d)", get_id(), m_pos.x, m_pos.y, end.x, end.y );
			const TileWorld<>::Points* pPoints = m_pworld->findpath(m_pos, end);
			m_accumulate = 0; 
			if(pPoints) {
				m_road = *pPoints;
				m_road.push_back(m_pos);
			} else {
			//	DEBUG_LOG("FIND PATROL ERROR");
			}
			return true;
		} else {
			m_pos = TileWorld<>::walk_path(m_road, m_direct, m_accumulate); 
			return false;
		}
	}
	return false;
}
	
bool Monster::try_locking(Entity* pEntity) {
	if(pEntity->m_fightprop.stealth) {
		return false;
	}
	if(	m_hatred.exist(pEntity)) {
		return true;
	}
	if( (uint32_t) (m_pos.distance2(pEntity->m_pos) ) <= m_fightprop.vision * m_fightprop.vision || pEntity->is_city() ) 
	{
	    m_hatred.add(pEntity);
//		DEBUG_LOG("MONSTER %lu ADD LOCKING %lu",get_id().id(), pEntity->get_id().id());
		pEntity->sig_obj_del.connect(this,&Monster::on_player_leave);
		pEntity->sig_leave_battle.connect(this,&Monster::on_player_del);
		pEntity->sig_dead.connect(this,&Monster::on_player_del);
		stat_ = LOCKING;
		return true;
	}
	return false;
}
	

bool Monster::locking(CSMonsterAttackRsp* attack_rsp) {
	Entity* enemy = m_hatred.get_current();
	if(enemy) {
//		DEBUG_LOG("CURRENT LOCKING TARGET %lu",enemy->get_id().id());
		if(m_skill_control.available() && attack_check(enemy)) {
			if(!m_skill_control.has_buff()) {
	//		DEBUG_LOG("MONSTER %d ATTACK CHECK OK SKILL:%d",get_id(),m_current_pskill->get_id());
				do_attack(enemy, attack_rsp);
				m_accumulate = 0;
				return false;
			} else {
				stat_ = BUFF;
				return false;
			}
		}
		return walk_to(enemy);
	} else {
		stat_ = PATROL;
	}
	return false;
}
	
Entity* Monster::get_current_target() {
	return m_hatred.get_current(false);
}
	
bool Monster::attack_check(Entity* des) {
//	if( m_current_pskill) {
	if(m_skill_control.available() && m_skill_control.has_buff()) {
		return true;
	}else if(m_skill_control.available() && des) {
		return m_skill_control.try_action(this, des);
//		return m_current_pskill->try_effect(this, des);
	} else {
		ERROR_LOG("MONSTER %d NO SKILL",get_id().id());
	}
	return false;
}
	
void Monster::do_attack(Entity* pEntity, CSMonsterAttackRsp* attack_msg ) {
/*
message CSMonsterAttackInfo {
    required uint32 id = 1;  //怪物在副本中的ID
    required CSPoint pos = 2; //怪物的位置
    required CSPoint direct = 3;//怪物的方向
    required uint32 action_id = 4; //动作ID
}
*/
//	if(m_current_pskill && m_current_pskill->available()) {
	if(m_skill_control.available()) {
			CSMonsterAttackInfo* info = attack_msg->add_attacks();
			info->set_id(get_id().id());
			m_direct = (pEntity->m_pos - m_pos);
			m_direct.unitised();
			CSPoint* pos = info->mutable_pos();
			pos->set_xpos(m_pos.x);	
			pos->set_ypos(m_pos.y);	
//			m_current_pskill->effect(this, pEntity, m_direct, m_effect_manager, info) ;
			m_skill_control.do_action(this, pEntity, m_direct, m_effect_manager, info) ;
			DEBUG_LOG("MONSTER %u DO ATTACK SKILL:%d POS:(%d %d) USER POS(%d %d) TIME(%d ,%d)",get_id().id(),m_skill_control.current_skill_id(),m_pos.x, m_pos.y, pEntity->m_pos.x, pEntity->m_pos.y, base::Time().seconds(), base::Time().u_seconds() );
			m_road.clear();
			info->set_skill_id(m_skill_control.current_skill_id());
			info->set_id(get_id().id());	
			CSPoint* direct = info->mutable_direct();
			direct->set_xpos(m_direct.x);
			direct->set_ypos(m_direct.y);
			info->set_userid(pEntity->get_id().id());
			info->set_reg_time(pEntity->get_id().regtime());
			info->set_channel_id(pEntity->get_id().channelid());
	}
}
	
void Monster::do_effect(CSMonsterEffectRsp* effect_msg ) {
	Entity* enemy = m_hatred.get_current(false);
	if(enemy) {
		m_skill_control.do_effect(this, enemy, m_direct, m_effect_manager, effect_msg);
	}
}
	
void Monster::do_buff(base::Manager<Monster>* monsters, CSBuffEffectRsp* buff_rsp) {
	if(stat_ == BUFF && m_skill_control.has_buff()) {
		m_skill_control.do_buff(this, monsters, buff_rsp);
		DEBUG_LOG("MONSTER %u DO BUFF:%d TIME(%d ,%d)",get_id().id(), m_skill_control.current_skill_id(), base::Time().seconds(), base::Time().u_seconds() );
		stat_ = IDLE;
	}
}
	
void Monster::set_idle(uint32_t time) {
	stat_ = IDLE;
	m_idle_time = time;
}
	
bool Monster::walk_to(Entity* pEntity) {
	/*magic_number 4*/
	if(!m_prop.speed) {
		return false;
	}
	if(!stop()) {
		m_pos = TileWorld<>::walk_path(m_road, m_direct, m_accumulate); 
		//DEBUG_LOG("MONSTER POS:(%d %d) TIME(%d ,%d)",m_pos.x, m_pos.y, base::Time().seconds(), base::Time().u_seconds());
	}
	if( m_target_point.distance2(pEntity->m_pos) >= 122 || m_pos.distance2(pEntity->m_pos) <= 36 || stop())
	{
//		DEBUG_LOG("LOCKING ID:%lu WALK FROM :(%d , %d) TO (%d, %d) ID:%lu", get_id().id(), m_pos.x, m_pos.y, pEntity->m_pos.x, pEntity->m_pos.y, pEntity->get_id().id() );
		if(pEntity->m_pos == m_pos) {
			m_road.clear();
			m_target_point = pEntity->m_pos;
			return false;
		}
		if( m_pos.distance2(pEntity->m_pos) >= 122 && stat_ != CHASE ) {
			m_target_point = m_pworld->find_one_surrounding_points2(pEntity->m_pos, 10); 
		} else {
			m_target_point = pEntity->m_pos;
		}
		if(!m_pworld->is_pos_walkable(m_target_point)) {
			ERROR_LOG("POINT TO ERROR POS:%d %d ID:%d",m_target_point.x, m_target_point.y, pEntity->get_id().userid);
		};
		const TileWorld<>::Points* pPoints = m_pworld->findpath(m_pos, m_target_point);
		if(pPoints) {
			m_road = *(pPoints);
			m_direct = (m_target_point - m_pos);
			m_direct.unitised();
			m_road.push_back(m_pos);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
	
bool Monster::walk_to(TileWorld<>::Point pos) {
	if(!m_prop.speed) {
		return false;
	}
	if(!stop()) {
		m_pos = TileWorld<>::walk_path(m_road, m_direct, m_accumulate); 
		//DEBUG_LOG("MONSTER POS:(%d %d) TIME(%d ,%d)",m_pos.x, m_pos.y, base::Time().seconds(), base::Time().u_seconds());
	}
	if(stop())	{
		DEBUG_LOG("LOCKING ID:%u WALK FROM :(%d , %d) TO (%d, %d)", get_id().id(), m_pos.x, m_pos.y, pos.x, pos.y);
		if(pos == m_pos) {
			m_road.clear();
			m_target_point = pos;
			return false;
		}
		const TileWorld<>::Points* pPoints = m_pworld->findpath(m_pos, pos);
		if(pPoints) {
			m_road = *(pPoints);
			m_direct = (m_target_point - m_pos);
			m_direct.unitised();
			m_road.push_back(m_pos);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
	
void Monster::update(CSMonsterMoveRsp* move_rsp, CSMonsterAttackRsp* attack_rsp, CSMonsterEffectRsp* effect_rsp) {
	/*Todo must update all skill*/
	//DEBUG_LOG("UPDATE POS %d %d",m_pos.x, m_pos.y);
/*	if( m_current_pskill && !m_current_pskill->available()) {
		m_current_pskill->update();
	}
	if(!m_current_pskill->available()) {
		return ;
	}*/
	if(m_idle_time) {
		m_idle_time = m_idle_time > UPDATETIME ? m_idle_time-UPDATETIME : 0;
	}
	if(!m_skill_control.available()) {
		m_skill_control.update();	
	}
	if(!m_skill_control.available()) {
		do_effect(effect_rsp);
	}
	/*Todo must delete such code*/
	if(stat_!= IDLE) {
		m_accumulate += m_prop.speed * 10 *	UPDATETIME / 1000;
	}
	switch (stat_) {
		case IDLE:
			{
				if(m_idle_time) {
					
				} else if(!m_hatred.empty()) {
					stat_ = LOCKING;
				} else {
					stat_ = PATROL;
				}
			}
			break;
		case PATROL:
			{
				//if(patrol()) {
					patrol();
					if(m_prop.speed) {
						MonsterMoveValue * move = move_rsp->add_moves();
						pack_move(move);
					}
				//};
			}
			break;
		case CHASE:
		case LOCKING:
			{
				if(locking(attack_rsp) && m_prop.speed) {
					MonsterMoveValue * move = move_rsp->add_moves();
					pack_move(move);
					DEBUG_LOG("TIME(%d %d) %s",base::Time().seconds(), base::Time().u_seconds(), move->ShortDebugString().c_str());
				};
			}
			break;
		case BUFF:
			break;
	} 
}
	
void Monster::on_player_leave(base::Object<player_key_t>* obj) {
	if(m_hatred.empty()) {
		stat_ = PATROL;
		m_target_point  = TileWorld<>::Point(-1, -1);
	}
	m_hatred.on_player_leave(obj);
}
	
void Monster::on_player_del(Entity* pPlayer) {
	on_player_leave(pPlayer);
}
	
void Monster::under_attack(Entity* pEntity, uint32_t blood) {
	if(m_prop.health_point >blood ) {
		m_prop.health_point-=blood;
	} else {
		m_prop.health_point = 0;
	}
	if(!m_hatred.exist(pEntity)) {
		pEntity->sig_obj_del.connect(this,&Monster::on_player_leave);
		pEntity->sig_leave_battle.connect(this,&Monster::on_player_del);
		pEntity->sig_dead.connect(this,&Monster::on_player_del);
	}
	m_hatred.add(pEntity, 10);
}
	
void Monster::under_attack_back(const TileWorld<>::Point& start_point, const TileWorld<>::Point& point) {
	if(!is_boss() && !is_elite()) {
		m_pos = point;
		m_road.clear();
		set_idle(start_point.distance(point) * 1000 /12 + UPDATETIME * 3);
		base::Time now;
//		DEBUG_LOG("MONSTER %u UNDER ATTACK STARTPS:(%d %d) BACKPOS:(%d %d) TIME(%d.%d)", get_id().id(), start_point.x, start_point.y, m_pos.x, m_pos.y, now.seconds(), now.u_seconds());
		sig_under_attack_back(this);
	}
}
	
bool Monster::stop() {
	if(m_road.empty() || (m_road.front() == m_pos )) {
		return true;
	}
	return false;
}
	
void Monster::pack_move(MonsterMoveValue* move_msg) {
	move_msg->set_id(get_id().id());
	CSPoint * direct = move_msg->mutable_direct();
	direct->set_xpos(m_direct.x);	
	direct->set_ypos(m_direct.y);
	TileWorld<>::Points::reverse_iterator itr = m_road.rbegin();
	for(; itr!=m_road.rend(); ++itr) {
		CSPoint* one_path = move_msg->add_paths();
		one_path->set_xpos(itr->x);
		one_path->set_ypos(itr->y);
//		DEBUG_LOG("POINT: %d %d", itr->x, itr->y);
	}
}
	
MonsterManager::MonsterManager():
	m_increase_key(1),
	m_boss_num(0),
	m_boss_dead(false),
	m_city_crash(false)
{
}
	
MonsterManager::~MonsterManager() {
	Monster * pmonster = next_ref(NULL);
	for (; pmonster!=NULL;) {
		pmonster->safe_release();
		pmonster = next_ref(NULL);	
	}
}
	
bool MonsterManager::empty() {
	return base::Manager<Monster>::empty() || m_boss_dead;	
}

bool MonsterManager::city_crash() {
    return m_city_crash;
}
	
Monster* MonsterManager::add_monster(MapMonster_t * mapmonster, TileWorld<>* world, EffectManager* effect_manager) {
	if (!mapmonster) {
		ERROR_RETURN(("ASSERT param MapMonster_t err!"), NULL);
	}
	if (!world) {
		ERROR_RETURN(("ASSERT param TileWorld<>err!"), NULL);
	}
	Monster_t* pmonster_t = g_monster_config->get_monster_info(mapmonster->monster_id);	
	if (!pmonster_t) {
		ERROR_RETURN(("ASSERT not found Monster_t! monster_id:%u", mapmonster->monster_id), NULL);
	}
	Monster * pmonster = NULL;
	switch(mapmonster->monster_id) {
		case 19002: //MonsterID="19002" Name="花妖"
		{
			pmonster = new FlowerDemon(mapmonster->raid_id, world, effect_manager);	
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19001:
		{
			pmonster = new BoarDemon(mapmonster->raid_id, world, effect_manager);	
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19003:
		{
			pmonster = new SpiderBanshee(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19004:
		{
			pmonster = new HugeStoneElement(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19005:
		{
			pmonster = new CactusKing(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19006:
		{
			pmonster = new ZombieBoss(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19007:
		{
			pmonster = new ElectromagneticBoss(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19008:
		{
			pmonster = new CrystalBeast(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19009:
		{
			pmonster = new ForbiddenCare(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19010:
		{
			pmonster = new DarkMagic(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19011:
		{
			pmonster = new Miasmic(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19012:
		{
			pmonster = new UndeadGhost(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19013:
		{
			pmonster = new HugeStoneElementVariant(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19014:
		{
			pmonster = new FieryDragon(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19015:
		{
			pmonster = new CastleGuard1(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19016:
		{
			pmonster = new CastleGuard2(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19017:
		{
			pmonster = new ChamberBoss(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19018:
		{
			pmonster = new MechanicalCrab(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19019:
		{
			pmonster = new UndeadGhost2(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19020:
		{
			pmonster = new Penpda(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19022:
		{
			pmonster = new Icebeast(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19501:
		{
			pmonster = new VampireBats(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19502:
		{
			pmonster = new ElfQueen(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19503:
		{
			pmonster = new DragonStorm(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19504:
		{
			pmonster = new Aeolus(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19505:
		{
			pmonster = new PaganCaptain(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19506:
		{
			pmonster = new PaganScientist(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19507:
		{
			pmonster = new PaganHunter(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
        case 19508:
		{
			pmonster = new PaganElder(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
        case 19509:
		{
			pmonster = new FireGod(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
        case 19510:
		{
			pmonster = new GreenGiant(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
        case 19511:
		{
			pmonster = new StarCarrier(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		case 19999:
		{
			pmonster = new LavaDevil(mapmonster->raid_id, world, effect_manager); 
			m_boss_num ++;
			pmonster->m_fightprop.world_boss = true;
			pmonster->sig_obj_del.connect(this,&MonsterManager::on_boss_dead);
		}
		break;
		default:
			pmonster = new Monster(mapmonster->raid_id, world, effect_manager);	
	}
	pmonster->init_info(*pmonster_t, *mapmonster);
	add_obj(pmonster);
    if(pmonster->is_city()) {
		pmonster->sig_dead.connect(this,&MonsterManager::on_city_crash);
    }
	pmonster->sig_send_msg.connect(this, &MonsterManager::on_send_msg);
	return pmonster;
}	
	
int MonsterManager::update(CSMonsterMoveRsp* move_rsp, CSMonsterAttackRsp* attack_rsp, CSMonsterEffectRsp* effect_rsp ) {
	std::map<Entity*, Monster*> chase_map;
	Monster * pmonster = next_ref(NULL);
	for (; pmonster!=NULL;) {
		Entity* entity = pmonster->get_current_target();
		if(entity) {
			if(pmonster->stat_ != Monster::IDLE && (pmonster->stat_ == Monster::CHASE || chase_map.find(entity) == chase_map.end()) ) {
				chase_map[entity] = pmonster;
			}
		}
		pmonster = next_ref(pmonster);	
	}
	std::map<Entity*, Monster*>::iterator itr = chase_map.begin();
	for(;itr!=chase_map.end(); ++itr) {
		itr->second->stat_ = Monster::CHASE;
	}
	pmonster = next_ref(NULL);
	for (; pmonster!=NULL;) {
		pmonster->update(move_rsp, attack_rsp, effect_rsp);
		pmonster = next_ref(pmonster);	
	} 
	return 0;
}
	
void MonsterManager::do_buff(CSBuffEffectRsp* buff_rsp) {
	Monster * pmonster = next_ref(NULL);
	for (; pmonster!=NULL;) {
		pmonster->do_buff(this, buff_rsp);	
		pmonster = next_ref(pmonster);	
	}
}
	
void MonsterManager::pack_move(CSMonsterMoveRsp* rsp) {
	Monster * pmonster = next_ref(NULL);
	for (; pmonster!=NULL;) {
		MonsterMoveValue* one = rsp->add_moves();
		pmonster->pack_move(one);
		pmonster = next_ref(pmonster);
	}	
}
	
void MonsterManager::try_locking(Entity* pEntity) {
	Monster * pmonster = next_ref(NULL);
	for(; pmonster!= NULL;)	{
		pmonster->try_locking(pEntity);
		pmonster = next_ref(pmonster);
	}
}

void MonsterManager::on_boss_dead(base::Object<player_key_t>* boss) {
	m_boss_num --;
	if(!m_boss_num) m_boss_dead = true;
}

void MonsterManager::on_city_crash(Entity* boss) {
	 m_city_crash = true;
}
	
void MonsterManager::on_send_msg(const Message& msg) {
	sig_send_msg(msg);
}
