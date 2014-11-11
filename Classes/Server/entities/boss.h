#ifndef BOSS_H_
#define BOSS_H_
#include "../base/ProbabilityGen.hpp"
#ifndef FRONT
#include "../world/server_attribute.h"
#endif

class FlowerDemon: public Monster {
public:
	FlowerDemon(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_change_value(3),
		m_change_value_2(0),
		m_check(true)
	{
	}

	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold = m_prop.health_point * 0.6 ;
		m_fightprop.vision = 100000;
	}

	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold ) {
				if(m_change_value >=2 ) {
					(m_skill_control.exchange_current(1900221));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900211));
					m_change_value --;
					if(!m_change_value)m_change_value = 3;
				}
			} else {
				if(m_change_value >=3) {
					(m_skill_control.exchange_current(1900221));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900211));
					m_change_value --;
					if(!m_change_value)m_change_value = 5;
				} 
			}
		}
		m_check = Monster::attack_check(des);
		if(!m_check && m_skill_control.available()) {
			m_skill_control.exchange_current(1900221);
			m_check = Monster::attack_check(des);
			if(m_prop.health_point >= m_threshold && m_change_value == 3 ) {
				m_change_value = 1;
			} else if(m_prop.health_point < m_threshold && m_change_value == 5) {
				m_change_value = 1;
			} else {
				m_change_value ++;
			}
		}
		return m_check;
	}

private:
	uint32_t m_threshold;
	uint32_t m_change_value;
	uint32_t m_change_value_2;
	bool m_check;
};

class BoarDemon: public Monster {
public:
	BoarDemon(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_change_value(2),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold = m_prop.health_point / 2;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_skill_control.available() && m_check) {
			if(m_prop.health_point >= m_threshold ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900111));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900121));
					m_change_value = 2;
				}
			} else {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900121));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900111));
					m_change_value = 3;
				}
			}
	
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}

private:
	uint32_t m_threshold;
	uint32_t m_change_value;
	bool m_check;
};

class SpiderBanshee: public Monster {
public:
	SpiderBanshee(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true),
		m_summon1(false),
		m_summon2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point / 2;
		m_threshold2 = m_prop.health_point * 0.2;
		m_change_value = 3;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_skill_control.available() && m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900311));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900321));
					m_change_value = 3;
				}
			} else if( m_prop.health_point <= m_threshold1 && !m_summon1) {
				(m_skill_control.exchange_current(1900331));
				m_summon1 = true;
			} else if( m_prop.health_point <= m_threshold2 && !m_summon2) {
				(m_skill_control.exchange_current(1900331));
				m_summon2 = true;
			}  else {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900311));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900321));
					m_change_value = 2;
				}
			}
	
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
};

class HugeStoneElement: public Monster {
public:
	HugeStoneElement(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff1(false),
		m_buff2(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.8;
		m_threshold2 = m_prop.health_point * 0.6;
		m_threshold3 = m_prop.health_point * 0.2;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_skill_control.available() && m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				(m_skill_control.exchange_current(1900411));
			} else if (m_prop.health_point >= m_threshold2 && !m_buff1) {
				(m_skill_control.exchange_current(1900421));
				m_buff1 = true;
			} else if(m_prop.health_point >= m_threshold2) {
				(m_skill_control.exchange_current(1900411));
			} else if (m_prop.health_point <= m_threshold3 && !m_buff2) {
				(m_skill_control.exchange_current(1900421));
				m_buff2 = true;
			} else {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900411));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900431));
					m_change_value = 3;
				}
			} 
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	bool m_buff1;
	bool m_buff2;
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_threshold3;
	uint32_t m_change_value;
	bool m_check;
};

class CactusKing: public Monster {
public:
	CactusKing(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold = m_prop.health_point * 0.6;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900511));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900521));
					m_change_value = 2;
				}
			}	else {
				if(m_change_value == 3) {
					(m_skill_control.exchange_current(1900521));
					m_change_value --;
				} else if(m_change_value) { 
					(m_skill_control.exchange_current(1900511));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900531));
					m_change_value = 3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold;
	uint32_t m_change_value;
	bool m_check;
};

class DarkMagic: public Monster {
public:
	DarkMagic(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true),
		m_summon1(false),
		m_summon2(false),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
		m_cd = base::Time().seconds();
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if (base::Time().seconds() >= m_cd + 10 ) {
				uint32_t last_skill = m_skill_control.current_skill_id();	
				(m_skill_control.exchange_current(1901021));
				m_check = Monster::attack_check(des);
				if(m_check) {
					m_cd = base::Time().seconds();
					return m_check;
				}
				(m_skill_control.exchange_current(last_skill));
			}
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901011));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901031));
					m_change_value = 3;
				}
			} else if (!m_summon1) {
				(m_skill_control.exchange_current(1901041));
				m_summon1 = true;
			} else if (m_prop.health_point <= m_threshold2 && !m_buff ) {
				(m_skill_control.exchange_current(1901051));
				m_buff = true;
			} else if (m_prop.health_point <= m_threshold2 && !m_summon2 ) {
				(m_skill_control.exchange_current(1901041));
				m_summon2 = true;
			} else {
				if(m_change_value > 1) {
					(m_skill_control.exchange_current(1901011));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1901031));
					if(!m_change_value) {
						m_change_value = 4;
					} else {
						m_change_value--;
					}
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_cd;
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
	bool m_buff;
};

class ZombieBoss: public Monster {
public:
	ZombieBoss(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true),
		m_summon1(false),
		m_summon2(false)	
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.6;
		m_threshold2 = m_prop.health_point * 0.2;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900611));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900621));
					m_change_value = 3;
				}
			} else if(m_prop.health_point <= m_threshold1 && !m_summon1 ) {
				(m_skill_control.exchange_current(1900631));
				m_summon1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_summon2 ) {
				(m_skill_control.exchange_current(1900631));
				m_summon2 = true;
			}  else {
				if(m_change_value > 1) {
					(m_skill_control.exchange_current(1900611));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900621));
					if(!m_change_value) {
						m_change_value = 4;
					} else {
						m_change_value--;
					}
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
};

class ElectromagneticBoss: public Monster {
public:
	ElectromagneticBoss(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold = m_prop.health_point * 0.5;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900711));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900721));
					m_change_value = 3;
				}
			}	else {
				if(m_change_value == 4) {
					(m_skill_control.exchange_current(1900731));
					m_change_value --;
				} else if(m_change_value) { 
					(m_skill_control.exchange_current(1900721));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900711));
					m_change_value = 4;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold;
	uint32_t m_change_value;
	bool m_check;
};

class CrystalBeast: public Monster {
public:
	CrystalBeast(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold = m_prop.health_point * 0.5;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold ) {
				if(m_change_value > 1) {
					(m_skill_control.exchange_current(1900811));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900821));
					if(m_change_value) {
						m_change_value --;
					} else {
						m_change_value = 3;
					}
				}
			}	else {
				if(!m_buff) {
					(m_skill_control.exchange_current(1900831));
					m_buff = true;
				} else if(m_change_value == 3) {
					(m_skill_control.exchange_current(1900811));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1900821));
					m_change_value = 3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff;
};

class ForbiddenCare: public Monster {
public:
	ForbiddenCare(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1900911));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900921));
					m_change_value = 2;
				}
			} else if(m_prop.health_point >= m_threshold2) {
				if(m_change_value == 4) {
					(m_skill_control.exchange_current(1900931));
					m_change_value --;
				} else if(m_change_value){ 
					(m_skill_control.exchange_current(1900921));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900911));
					m_change_value =4;
				}
			} else {
				if(m_change_value == 4) {
					(m_skill_control.exchange_current(1900941));
					m_change_value --;
				} else if(m_change_value){ 
					(m_skill_control.exchange_current(1900921));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1900931));
					m_change_value =4;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
};

class UndeadGhost: public Monster {
public:
	UndeadGhost(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.6;
		m_change_value = 1;
		m_fightprop.vision = 100000;
		m_cd = base::Time().seconds();
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901231));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901211));
					m_change_value = 2;
				}
			} else if(base::Time().seconds() > m_cd+15) {
				(m_skill_control.exchange_current(1901221));
				m_cd = base::Time().seconds();
			} else {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901231));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901211));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_change_value;
	uint32_t m_cd;
	bool m_check;
};

class Miasmic: public Monster {
public:
	Miasmic(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_summon1(false),
		m_summon2(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.6;
		m_threshold1 = m_prop.health_point * 0.2;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value >1 ) {
					(m_skill_control.exchange_current(1901111));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901121));
					m_change_value --;
					if(!m_change_value)
						m_change_value = 3;
				}
			} else if(m_prop.health_point >= m_threshold2 && !m_summon1) {
				(m_skill_control.exchange_current(1901131));
				m_summon1 = true;
			} else if(!m_summon2) {
				(m_skill_control.exchange_current(1901131));
				m_summon2 = true;
			} else {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901121));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901111));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	bool m_summon1;
	bool m_summon2;
	uint32_t m_change_value;
	bool m_check;
};

class HugeStoneElementVariant: public Monster {
public:
	HugeStoneElementVariant(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff1(false),
		m_buff2(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.8;
		m_threshold2 = m_prop.health_point * 0.6;
		m_threshold3 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				(m_skill_control.exchange_current(1901311));
			} else if (!m_buff1 && m_prop.health_point > m_threshold2) {
				(m_skill_control.exchange_current(1901321));
				m_buff1 = true;	
			} else if (!m_buff2 && m_prop.health_point <= m_threshold2 ) {
				(m_skill_control.exchange_current(1901321));
				m_buff2 = true;	
			} else {
				if(m_change_value > 2) {
					(m_skill_control.exchange_current(1901331));
					m_change_value --;
				} else if (m_change_value) { 
					(m_skill_control.exchange_current(1901311));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901341));
					m_change_value = 3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_threshold3;
	bool m_buff1;
	bool m_buff2;
	uint32_t m_change_value;
	bool m_check;
};

class Icebeast: public Monster {
public:
	Icebeast(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff1(false),
		m_buff2(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.2;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1902211));
					m_change_value --;
				} else { 
					(m_skill_control.exchange_current(1902221));
					m_change_value = 2;
				}
			} else if (!m_buff1 && m_prop.health_point > m_threshold2) {
				(m_skill_control.exchange_current(1902241));
				m_buff1 = true;	
			} else if (!m_buff2 && m_prop.health_point <= m_threshold2) {
				(m_skill_control.exchange_current(1902241));
				m_buff2 = true;	
			} else if ( m_prop.health_point >= m_threshold2 ){
				if(m_change_value==2) {
					(m_skill_control.exchange_current(1902211));
					m_change_value --;
				} else if(m_change_value==1){ 
					(m_skill_control.exchange_current(1902221));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1902231));
					m_change_value = 2;
				}
			} else {
				if(m_change_value==5) {
					(m_skill_control.exchange_current(1902211));
					m_change_value --;
				} else if(m_change_value>=3){ 
					(m_skill_control.exchange_current(1902221));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1902231));
					m_change_value --;
					if(!m_change_value)	
						m_change_value = 5;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	bool m_buff1;
	bool m_buff2;
	uint32_t m_change_value;
	bool m_check;
};

class FieryDragon: public Monster {
public:
	FieryDragon(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff1(false),
		m_buff2(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value >=3) {
					(m_skill_control.exchange_current(1901411));
					m_change_value --;
				} else if(m_change_value == 2) { 
					(m_skill_control.exchange_current(1901421));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901451));
					m_change_value = 4;
				}
			} else if (!m_buff1 && m_prop.health_point <= m_threshold2) {
				(m_skill_control.exchange_current(1901441));
				m_buff1 = true;	
			} else {
				if(m_change_value==4) {
					(m_skill_control.exchange_current(1901431));
					m_change_value --;
				} else if(m_change_value==3){ 
					(m_skill_control.exchange_current(1901411));
					m_change_value --;
				} else if(m_change_value)  {
					(m_skill_control.exchange_current(1901421));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901451));
					m_change_value = 5;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	bool m_buff1;
	bool m_buff2;
	uint32_t m_change_value;
	bool m_check;
};

class ChamberBoss: public Monster {
public:
	ChamberBoss(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff(false),
		m_summon1(false),
		m_summon2(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901711));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901721));
					m_change_value = 3;
				}
			} else if (!m_summon1 && m_prop.health_point >= m_threshold2) {
				(m_skill_control.exchange_current(1901731));
				m_summon1 = true;	
			} else if (!m_buff && m_prop.health_point <= m_threshold2) {
				(m_skill_control.exchange_current(1901741));
				m_buff = true;	
			} else if (!m_summon2 && m_prop.health_point <= m_threshold2) {
				(m_skill_control.exchange_current(1901731));
				m_summon2 = true;	
			}  else {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901721));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1901711));
					m_change_value = 3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	bool m_buff;
	bool m_summon1;
	bool m_summon2;
	uint32_t m_change_value;
	bool m_check;
};

class MechanicalCrab: public Monster {
public:
	MechanicalCrab(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff(false),
		m_fire1(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.4;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901811));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901831));
					m_change_value = 3;
				}
			} else if (!m_fire1 && m_prop.health_point <= m_threshold1)  {
				(m_skill_control.exchange_current(1901821));
				m_fire1 = true;		
			} else if (!m_buff && m_prop.health_point <= m_threshold2) {
				(m_skill_control.exchange_current(1901841));
				m_buff = true;	
			} else {
				if(m_change_value > 2) {
					(m_skill_control.exchange_current(1901831));
					m_change_value --;
				} else if(m_change_value) {
					(m_skill_control.exchange_current(1901811));
					m_change_value --;
				}  else  {
					(m_skill_control.exchange_current(1901821));
					m_change_value = 4;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
	
	bool locking(CSMonsterAttackRsp* attack_rsp) {
		Entity* enemy = m_hatred.get_current();
		if(enemy) {
//			DEBUG_LOG("CURRENT LOCKING TARGET %lu",enemy->get_id().id());
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
			if(m_skill_control.current_skill_id() == 1901821 ) {
				RailgunSkill* pskill = dynamic_cast<RailgunSkill*>(m_skill_control.get_current_skill());
				if(pskill ) {
					TileWorld<>::Point des = pskill->get_point();
					return walk_to(des);
				}
			} else  {
				return walk_to(enemy);
			}
		} else {
			stat_ = PATROL;
		}
		return false;
	}

private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	bool m_buff;
	uint32_t m_change_value;
	bool m_fire1;
	bool m_check;
};

class UndeadGhost2: public Monster {
public:
	UndeadGhost2(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true),
		m_summon1(false),
		m_summon2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1901931));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1901911));
					m_change_value = 2;
				}
			} else if(m_prop.health_point >= m_threshold2 && !m_summon1) {
				(m_skill_control.exchange_current(1901941));
				m_summon1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_summon2) {
				(m_skill_control.exchange_current(1901941));
				m_summon2 = true;
			} else if(m_prop.health_point >= m_threshold2) {
				if(m_change_value == 3) {
					(m_skill_control.exchange_current(1901921));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1901931));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1901911));
					m_change_value =3;
				}

			} else {
				if(m_change_value == 4) {
					(m_skill_control.exchange_current(1901921));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1901931));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1901911));
					m_change_value =4;
				}
			}

		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
};

class CastleGuard1: public Monster {
public:
	CastleGuard1(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_buff(false),
		m_check(true)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_change_value = 1;
		m_fightprop.vision = 100000;
		m_threshold1 = m_prop.health_point * 0.6;
		m_cd = base::Time().seconds();
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point <= m_threshold1 && !m_buff ) {
 				(m_skill_control.exchange_current(1901531));
				m_buff = true;
			} else if(m_change_value) {
				(m_skill_control.exchange_current(1901511));
				m_change_value --;
			} else {
				(m_skill_control.exchange_current(1901521));
				m_change_value = 3;
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	bool m_buff;
	uint32_t m_change_value;
	bool m_check;
	uint32_t m_threshold1;
	uint32_t m_cd;
};

class CastleGuard2: public Monster {
public:
	CastleGuard2(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(true),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_fightprop.vision = 100000;
		m_threshold1 = m_prop.health_point * 0.7;
		m_cd = base::Time().seconds();
		m_change_value = 1;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_change_value) {
				(m_skill_control.exchange_current(1901611));
				m_change_value --;	
			} else {
				(m_skill_control.exchange_current(1901631));
				m_change_value = 2;
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	bool m_check;
	bool m_buff;
	uint32_t m_threshold1;
	uint32_t m_cd;
	uint32_t m_change_value;
};

class Penpda: public Monster {
public:
	Penpda(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_summon1(false),
		m_summon2(false),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1902011));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1902021));
					m_change_value = 2;
				}
			} else if(m_prop.health_point >= m_threshold2 && !m_summon1) {
				(m_skill_control.exchange_current(1902051));
				m_summon1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_buff) {
				(m_skill_control.exchange_current(1902041));
				m_buff = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_summon2) {
				(m_skill_control.exchange_current(1902051));
				m_summon2 = true;
			}  else if(m_prop.health_point >= m_threshold2) {
				if(m_change_value == 2) {
					(m_skill_control.exchange_current(1902031));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1902021));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1902011));
					m_change_value =2;
				}
			} else {
				if(m_change_value == 4) {
					(m_skill_control.exchange_current(1902031));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1902021));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1902011));
					m_change_value =4;
				}

			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
	bool m_buff;
};

class VampireBats: public Monster {
public:
	VampireBats(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff1(false),
		m_buff2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950111));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950121));
					m_change_value = 2;
				}
			} else if(m_prop.health_point > m_threshold2 && !m_buff1) {
				(m_skill_control.exchange_current(1950141));
				m_buff1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_buff2) {
				(m_skill_control.exchange_current(1950141));
				m_buff2 = true;
			}  else {
				if(m_change_value == 3) {
					(m_skill_control.exchange_current(1950111));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950121));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950131));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff1;
	bool m_buff2;
};

class ElfQueen: public Monster {
public:
	ElfQueen(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950211));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950221));
					m_change_value = 2;
				}
			} else if(m_prop.health_point <= m_threshold2 && !m_buff) {
				(m_skill_control.exchange_current(1950241));
				m_buff = true;
			}  else {
				if(m_change_value == 3) {
					(m_skill_control.exchange_current(1950211));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950221));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950231));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff;
};

class DragonStorm: public Monster {
public:
	DragonStorm(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950311));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950321));
					m_change_value = 2;
				}
			} else if(m_prop.health_point <= m_threshold2 && !m_buff) {
				(m_skill_control.exchange_current(1950341));
				m_buff = true;
			}  else {
				if(m_change_value > 1) {
					(m_skill_control.exchange_current(1950311));
					m_change_value --;
				} else if (m_change_value)  {
					(m_skill_control.exchange_current(1950321));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950331));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff;
};

class Aeolus: public Monster {
public:
	Aeolus(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
		m_cd = base::Time().seconds();
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if (base::Time().seconds() >= m_cd + 10 ) {
                uint32_t last_skill = m_skill_control.current_skill_id();
                (m_skill_control.exchange_current(1950441));
                m_check = Monster::attack_check(des);
                if(m_check) {
                    m_cd = base::Time().seconds();
                    return m_check;
                }
				(m_skill_control.exchange_current(last_skill));
			}
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950411));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950421));
					m_change_value = 2;
				}
			} else if(m_prop.health_point <= m_threshold2 && !m_buff) {
				(m_skill_control.exchange_current(1950451));
				m_buff = true;
			}  else {
				if(m_change_value > 1) {
					(m_skill_control.exchange_current(1950411));
					m_change_value --;
				} else if (m_change_value)  {
					(m_skill_control.exchange_current(1950421));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950431));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff;
	uint32_t m_cd;
};

class PaganCaptain: public Monster {
public:
	PaganCaptain(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff1(false),
		m_buff2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950511));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950521));
					m_change_value = 2;
				}
			} else if(m_prop.health_point > m_threshold2 && !m_buff1) {
				(m_skill_control.exchange_current(1950541));
				m_buff1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_buff2) {
				(m_skill_control.exchange_current(1950541));
				m_buff2 = true;
			}  else {
				if(m_change_value == 2) {
					(m_skill_control.exchange_current(1950511));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950521));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950531));
					m_change_value =2;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff1;
	bool m_buff2;
};

class PaganScientist: public Monster {
public:
	PaganScientist(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_summon1(false),
		m_summon2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950611));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950621));
					m_change_value = 2;
				}
			} else if(m_prop.health_point > m_threshold2 && !m_summon1) {
				(m_skill_control.exchange_current(1950641));
				m_summon1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_summon2) {
				(m_skill_control.exchange_current(1950641));
				m_summon2 = true;
			}  else {
				if(m_change_value == 3) {
					(m_skill_control.exchange_current(1950611));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950621));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950631));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
};

class PaganHunter: public Monster {
public:
	PaganHunter(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_summon1(false),
		m_summon2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950711));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950721));
					m_change_value = 2;
				}
			} else if(m_prop.health_point > m_threshold2 && !m_summon1) {
				(m_skill_control.exchange_current(1950741));
				m_summon1 = true;
			} else if(m_prop.health_point <= m_threshold2 && !m_summon2) {
				(m_skill_control.exchange_current(1950741));
				m_summon2 = true;
			}  else {
				if(m_change_value == 3) {
					(m_skill_control.exchange_current(1950721));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950711));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950731));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
};

class LavaDevil: public Monster {
public:
	LavaDevil(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_change_value = 3;
		m_fightprop.vision = 100000;
#ifndef FRONT
		wb_hp_check();
#endif
		gen.push(1999911);
		gen.push(1999921);
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
			if(m_change_value >= 2) {
				uint32_t skill_id = gen.get_one(false);
				(m_skill_control.exchange_current(skill_id));
				m_change_value --;
			} else if(m_change_value == 1) {
				(m_skill_control.exchange_current(1999931));
				m_change_value --;
			} else {
				(m_skill_control.exchange_current(1999941));
				m_change_value = 3;
			}
 		}
		m_check = Monster::attack_check(des);
		return m_check;
	}

private:
#ifndef FRONT
	void wb_hp_check()
	{
		if (sServerAttr.value(SRV_ATTR_LAVADEVIL_HP) == 0)
			sServerAttr.set(SRV_ATTR_LAVADEVIL_HP, m_fightprop.health_point);

		if (sServerAttr.value(SRV_ATTR_LD_DEFAULT_HP) == 0)
			sServerAttr.set(SRV_ATTR_LD_DEFAULT_HP, m_fightprop.health_point);
		
		if (sServerAttr.value(SRV_ATTR_LD_DEFAULT_HP) !=  m_fightprop.health_point)
			sServerAttr.set(SRV_ATTR_LD_DEFAULT_HP, m_fightprop.health_point);
		
		if (sServerAttr.value(SRV_ATTR_LAVADEVIL_HP) < sServerAttr.value(SRV_ATTR_LD_DEFAULT_HP)) 
			sServerAttr.set(SRV_ATTR_LAVADEVIL_HP, sServerAttr.value(SRV_ATTR_LD_DEFAULT_HP));	

		m_prop.health_point = sServerAttr.value(SRV_ATTR_LAVADEVIL_HP);
		m_fightprop.health_point = sServerAttr.value(SRV_ATTR_LAVADEVIL_HP);
	}
#endif
	
private:	
	uint32_t m_change_value;
	bool m_check;
	base::AverageGen<uint32_t> gen;
};

class PaganElder: public Monster {
public:
	PaganElder(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
		m_cd = base::Time().seconds();
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
    		if (base::Time().seconds() >= m_cd + 10 ) {
				uint32_t last_skill = m_skill_control.current_skill_id();	
				(m_skill_control.exchange_current(1950841));
				m_check = Monster::attack_check(des);
				if(m_check) {
					m_cd = base::Time().seconds();
					return m_check;
				}
				(m_skill_control.exchange_current(last_skill));
			}        
            if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950821));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950811));
					m_change_value = 2;
				}
			} else if(m_prop.health_point < m_threshold2 && !m_buff) {
				(m_skill_control.exchange_current(1950851));
				m_buff = true;
			} else {
				if(m_change_value == 2) {
					(m_skill_control.exchange_current(1950831));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950821));
					m_change_value --;
				} else  {
					(m_skill_control.exchange_current(1950811));
					m_change_value =2;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
    uint32_t m_cd;
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff;
};

class FireGod: public Monster {
public:
	FireGod(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
		m_buff1(false),
        m_buff2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.2;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
            if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1950911));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1950931));
					m_change_value = 3;
				}
			} else if(m_prop.health_point < m_threshold1 && !m_buff1) {
				(m_skill_control.exchange_current(1950941));
				m_buff1 = true;
			} else if(m_prop.health_point < m_threshold2 && !m_buff2) {
				(m_skill_control.exchange_current(1950941));
				m_buff2 = true;
			}  else {
				if(m_change_value >= 4) {
					(m_skill_control.exchange_current(1950911));
					m_change_value --;
				} else if (m_change_value ==3)  {
					(m_skill_control.exchange_current(1950931));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1950911));
					m_change_value --;
				}  else  {
					(m_skill_control.exchange_current(1950921));
					m_change_value =5;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_buff1;
	bool m_buff2;
};

class GreenGiant: public Monster {
public:
	GreenGiant(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
            if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1951011));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1951021));
					m_change_value = 2;
				}
			} else {
				if(m_change_value >= 4) {
					(m_skill_control.exchange_current(1951011));
					m_change_value --;
				} else if (m_change_value ==3)  {
					(m_skill_control.exchange_current(1951021));
					m_change_value --;
				} else if (m_change_value >=1)  {
					(m_skill_control.exchange_current(1951011));
					m_change_value --;
				}  else  {
					(m_skill_control.exchange_current(1951031));
					m_change_value =5;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_change_value;
	bool m_check;
};

class StarCarrier: public Monster {
public:
	StarCarrier(uint32_t random_id, TileWorld<>* tileworld, EffectManager* effect_manager) :
		Monster(random_id, tileworld, effect_manager),
		m_check(false),
        m_summon1(false),
        m_summon2(false)
	{
	}
	
	void init_info(const Monster_t& info, const MapMonster_t& map_info) {
		Monster::init_info(info, map_info);
		m_threshold1 = m_prop.health_point * 0.7;
		m_threshold2 = m_prop.health_point * 0.3;
		m_change_value = 1;
		m_fightprop.vision = 100000;
	}
	
	bool attack_check(Entity* des) {
		if(m_check) {
            if(m_prop.health_point >= m_threshold1 ) {
				if(m_change_value) {
					(m_skill_control.exchange_current(1951111));
					m_change_value --;
				} else {
					(m_skill_control.exchange_current(1951121));
					m_change_value = 2;
				}
			} else if(m_prop.health_point < m_threshold1 && !m_summon1) {
				(m_skill_control.exchange_current(1951141));
				m_summon1 = true;
			} else if(m_prop.health_point < m_threshold2 && !m_summon2) {
				(m_skill_control.exchange_current(1951141));
				m_summon2 = true;
			} else {
				if(m_change_value >= 3) {
					(m_skill_control.exchange_current(1951111));
					m_change_value --;
				} else if (m_change_value ==2)  {
					(m_skill_control.exchange_current(1951121));
					m_change_value --;
				} else if (m_change_value ==1)  {
					(m_skill_control.exchange_current(1951111));
					m_change_value --;
				}  else  {
					(m_skill_control.exchange_current(1951131));
					m_change_value =3;
				}
			}
		}
		m_check = Monster::attack_check(des);
		return m_check;
	}
private:
	uint32_t m_threshold1;
	uint32_t m_threshold2;
	uint32_t m_change_value;
	bool m_check;
	bool m_summon1;
	bool m_summon2;
};



#endif
