// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#include "UserData.h"
#include "cocos2d.h"
#include "LevelManager.h"

// -------------------------------------------------------------------------------------------------

USERINFO::USERINFO()
{
	// 基类的值也会置为0，基类的值不一定是0的，特别是数据的内存加密引入以后。
	//memset(this, 0x00, sizeof(USERINFO));
    
	memset(userSession, 0x00, sizeof(userSession));
	memset(szName, 0x00, sizeof(szName));
	memset(ip, 0x00, sizeof(ip));
	memset(lastServerName, 0x00, sizeof(lastServerName));

	serverId = 0;
	port = 0;
	createdTime = 0;
	tokenId = 0;
	level = 0;
	exp = 0;
	nextExp = 0;
	mapId = 0;
	xPos = 0;
	yPos = 0;
	orient = 0;
	type = 0;
	battleId = 0;
	battleSide = 0;
	m_spriteEnergy = 0;
	m_gold = 0;
	m_diamond = 0;
	m_spriteChip = 0;
	m_reputation = 0;
	m_stamina = 0;
	m_alchemyBagUnlockPos = 0;
	m_alchemyStorageUnlockPos = 0;
	m_searchSpriteNpc = 0;
	m_equipLvUpMaxTimes = 0;
	m_equipLvFlag = 0;
	m_equipLvUpCd = 0;
	m_gemRecastTimes = 0;
	m_player_exploit = 0;
	m_playerExploerExperience = 0;
	m_playerGemAnima = 0;
	m_enterManorTimes = 0;
	m_autofightUnderCd = 0;
	m_buyManorTimes = 0;
	m_autofightNormalCd = 0;
	m_autofightJingYingCd = 0;
	m_newMailNum = 0;
	m_attachMailNum = 0;
	m_totalMailNum = 0;
	m_fragStone = 0;
	pay_money = 0;
	mine_money = 0;
	train_money = 0;
	suspend_limit_exp = 0;
	m_vip = 0;
	m_lastStaminaRestoreTime = 0;

    m_continueloginTime = 1;
    m_loginRewardTime = 1;
    m_vipRewardTime = 1;
    m_payRewardTime = 1;
    m_buyDiamondRewardTime = 1;
    m_pvaiRewardTime = 1;
    m_palyHoldExp = 1;
    first_login = false;
	b_hasEnoughEquipFrag = false;
	b_hasEnouchFairyFlag = false;
	b_hasTargetIconFlag = false;
	b_reputationExchangeFlag = true;

	m_pvpDueledTimesNum = 0;
	m_pvpDuelTimesNum = 0;
	m_pvpRed = 0;
	m_pvpRedTimer = 0;

    m_junior_explore_time = 0;
    m_senior_explore_time = 0;

    m_spec_left_times = 0;
    m_green_extract_times = 0;

    m_diamond_acc = 0;
    m_soulStone = 0;

    m_shopRefreshTime = 0;
    m_refreshTimes = 0;

    m_treasure_explore_times = 0;
    m_treasure_explore_mapIndex = 1;
    m_treasure_explore_success_rate = 0;
    m_treausre_already_get_index = 0;
    m_treasure_levelup_flag = 0;
    m_worldboss_forbid_roles = 0;
    m_worldboss_revive_by_cd = 0;
    m_worldboss_revive_by_diamond = 0;
    m_guild_bless_times = 0;
    m_guild_contribute_times = 0;
    m_guild_contributes = 0;
    m_guild_join_cd_time = 0;

    m_get91_dailyBonus = 0;
    m_get91_vipBonus = 0;
    m_get91_firstChargeBonus = 0;
    m_91Vip = 0;

	current_title = 0;
	guildName = "";
	m_guildId.setGuildId(0,0);
}

void USERINFO::Set( const BasicUserInfo& info )
{
	id.setPlayId(info.uid(),info.reg_time(),info.channel_id());

	memcpy(szName,  info.name().c_str(), info.name().length());


	cocos2d::CCPoint pos =  LevelManager::sShareInstance()->pointCellToPixel(ccp(info.xpos(), info.ypos()));

	xPos = pos.x;
	yPos = pos.y;


	orient = info.orient();
	type = info.type();
	level = info.lv();
	nextExp = info.next_lv();
	m_vip = info.vip_lv();//add user vip data
	current_title = info.current_title();
	guildName = info.gname();
	m_guildId.setGuildId(info.gid_high(),info.gid_low());

	if(info.has_battle_side())
		battleSide = info.battle_side();
	effectiveness = info.effectiveness();
	if(info.has_battle_info())
	{
		const BasicUserBattleInfo& bi = info.battle_info();
		Set(bi);
	}

}

void USERINFO::Set( const BasicUserBattleInfo& bi )
{
	BATTLEINFO::Set(bi);
}

BATTLEINFO::BATTLEINFO()
{
	physical_attack = 0;
	magic_attack = 0;
	skill_attack = 0;
	physical_defence = 0;
	magic_defence = 0;
	skill_defence = 0;
	health_point = 0;
	accurate  = 0; // 命中
	dodge = 0;		// 闪避
	wreck = 0;		// 破击
	parry = 0;		// 格挡
	critical_strike = 0;	// 暴击
	tenacity = 0;			// 任性
	slay = 0;				// 必杀
	proficiency = 0;
	speed = 0;				// 减速
	total_hp = 0;
	courage = 0; // 勇气
	charm = 0; // 魔力
	trick = 0; // 技巧
	effectiveness = 0; // 战斗力
	ragePoint = 0;		//// 精灵怒气 
	totalRangePoint = 0;	
	secRagePoint = 0;
	totalRangePoint = 0;

	totalRangePoint = 1000;
	secRagePoint = 0;
	secTotalRangePoint = 1000;

	rageBasicInited = false;

}

BATTLEINFO BATTLEINFO::operator+( const BATTLEINFO& info )
{
	BATTLEINFO ret;
	
	ret.physical_attack = physical_attack + info.physical_attack;
	ret.magic_attack = magic_attack + info.magic_attack;
	ret.skill_attack = skill_attack + info.skill_attack;
	ret.physical_defence = physical_defence + info.physical_defence;
	ret.magic_defence = magic_defence + info.magic_defence;
	ret.skill_defence = skill_defence + info.skill_defence;
	ret.health_point = health_point + info.health_point;
	ret.accurate = accurate + info.accurate;
	ret.dodge = dodge + info.dodge;
	ret.wreck = wreck + info.wreck;
	ret.parry = parry + info.parry;
	ret.critical_strike = critical_strike + info.critical_strike;
	ret.tenacity = tenacity + info.tenacity;
	ret.slay = slay + info.slay;
	ret.proficiency = proficiency + info.proficiency;
	ret.speed = speed + info.speed;
	ret.total_hp = total_hp + info.total_hp;
	ret.courage = courage + info.courage;
	ret.charm = charm + info.charm;
	ret.trick = trick + info.trick;
	ret.effectiveness = effectiveness + info.effectiveness;


	return ret;
}

const BATTLEINFO& BATTLEINFO::operator=( const BATTLEINFO& info )
{
	if(this == &info)
		return * this;

	physical_attack = info.physical_attack;
	magic_attack = info.magic_attack;
	skill_attack = info.skill_attack;
	physical_defence = info.physical_defence;
	magic_defence = info.magic_defence;
	skill_defence = info.skill_defence;
	health_point = info.health_point;
	accurate = info.accurate;
	dodge = info.dodge;
	wreck = info.wreck;
	parry = info.parry;
	critical_strike = info.critical_strike;
	tenacity = info.tenacity;
	slay = info.slay;
	proficiency = info.proficiency;
	speed = info.speed;
	total_hp = info.total_hp;
	courage = info.courage;
	charm = info.charm;
	trick = info.trick;
	effectiveness = info.effectiveness;
	ragePoint = info.ragePoint;		//// 精灵怒气 
	totalRangePoint = info.totalRangePoint;	
	secRagePoint = info.secRagePoint;
	secTotalRangePoint = info.secTotalRangePoint;

	rageBasicInited = info.rageBasicInited;

	return *this;
}

MonsterBattleInfo BATTLEINFO::GetMonsterBattleInfo()
{
	MonsterBattleInfo battleInfo;
	battleInfo.set_physical_attack(this->physical_attack);
	battleInfo.set_magic_attack(this->magic_attack);
	battleInfo.set_skill_attack(this->skill_attack);
	battleInfo.set_physical_defence(this->physical_defence);
	battleInfo.set_magic_defence(this->magic_defence);
	battleInfo.set_skill_defence(this->skill_defence);
	battleInfo.set_accurate(this->accurate);
	battleInfo.set_dodge(this->dodge);
	battleInfo.set_wreck(this->wreck);
	battleInfo.set_parry(this->parry);
	battleInfo.set_critical_strike(this->critical_strike);
	battleInfo.set_tenacity(this->tenacity);
	battleInfo.set_slay(this->slay);
	battleInfo.set_proficiency(this->proficiency);
	return battleInfo;
}

void BATTLEINFO::Set( const MonsterBattleInfo& bi )
{
	this->physical_attack = bi.physical_attack();
	this->magic_attack = bi.magic_attack();
	this->skill_attack = bi.skill_attack();
	this->physical_defence = bi.physical_defence();
	this->magic_defence = bi.magic_defence();
	this->skill_defence = bi.skill_defence();
	//this->health_point = bi.health_point();
	//this->total_hp = bi.health_point();
	this->accurate = bi.accurate();
	this->dodge = bi.dodge();
	this->wreck = bi.wreck();
	this->parry = bi.parry();
	this->critical_strike = bi.critical_strike();
	this->tenacity = bi.tenacity();
	this->slay = bi.slay();
	this->proficiency = bi.proficiency();
}

void BATTLEINFO::Set( const BasicUserBattleInfo& bi )
{
		physical_attack = bi.physical_attack();
		
		magic_attack = bi.magic_attack();
		skill_attack = bi.skill_attack();
		physical_defence = bi.physical_defence();
		magic_defence = bi.magic_defence();
		skill_defence = bi.skill_defence();
		total_hp = bi.health_point();
		accurate = bi.accurate();
		dodge = bi.dodge();
		wreck = bi.wreck();
		parry = bi.parry();
		critical_strike = bi.critical_strike();
		tenacity = bi.tenacity();
		slay = bi.slay();
		proficiency = bi.proficiency();
		speed = bi.speed();
		courage = bi.courage();
		trick = bi.trick();
		charm = bi.charm();
	
	
		if(bi.has_current_hp())
		{
			health_point = bi.current_hp();
		}
		else
		{
			health_point = bi.health_point();
		}
}

void BATTLEINFO::ResetRagePoint()
{
	ragePoint = 0;
	secRagePoint = 0;

	rageBasicInited = false;
}

// -------------------------------------------------------------------------------------------------
MONSTERINFO::MONSTERINFO()
{

}

void MONSTERINFO::Set( const MonsterInfo& info )
{
	id.setPlayId(info.id(),0,0);
	//this->id = info.id();
	if(info.has_battle_info())
	{
		const MonsterBattleInfo& bi = info.battle_info();
		BATTLEINFO::Set(bi);
	}
}


// -------------------------------------------------------------------------------------------------

UserData::UserData()
{
	// 不能这样简单的使用，基类的值呢？基类的很多值默认值不是0
	//memset(&m_stUserinfo, 0, sizeof(m_stUserinfo));
    
    m_stUserinfo.m_continueloginTime = 1;
    m_stUserinfo.m_loginRewardTime = 1;
    m_stUserinfo.m_vipRewardTime = 1;
    m_stUserinfo.m_payRewardTime = 1;
    m_stUserinfo.m_buyDiamondRewardTime = 1;
    m_stUserinfo.m_pvaiRewardTime = 1;
    m_stUserinfo.m_palyHoldExp = 1;
    m_stUserinfo.first_login=false;
	m_stUserinfo.totalRangePoint = 1000;
	m_stUserinfo.secTotalRangePoint = 1000;

	string str = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("lastIp");
	sprintf(m_stUserinfo.ip,"%s",str.c_str());
	string server = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("lastServerName");
	sprintf(m_stUserinfo.lastServerName,"%s",server.c_str());
	m_stUserinfo.port = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("lastPort");
	m_stUserinfo.serverId = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("serverId");
	
}



const USERINFO& UserData::GetUserInfo()
{
	return UserData::Get()->m_stUserinfo;
}

USERINFO* UserData::GetUserInfo( PLAYER_ID id )
{
	if(id == m_stUserinfo.id)
		return &m_stUserinfo;

	std::map<PLAYER_ID, USERINFO>::iterator iter = m_userInfoList.find(id);
	if(iter != m_userInfoList.end())
	{
		return &iter->second;
	}
	return NULL;
}

std::list<PLAYER_ID>* UserData::GetOtherUserId()
{
	std::list<PLAYER_ID>* otherUsers = new std::list<PLAYER_ID>();
	for(std::map<PLAYER_ID, USERINFO>::iterator iter = m_userInfoList.begin();
		iter != m_userInfoList.end();iter++)
	{
		PLAYER_ID playerId = iter->first;
		if(m_stUserinfo.id != playerId)
		{
			otherUsers->push_back(playerId);
		}
	}
	return otherUsers;
}

void UserData::clearOtherUserInfo()
{
	m_userInfoList.clear();
}

void UserData::removeOtherUser(PLAYER_ID id)
{
	std::map<PLAYER_ID, USERINFO>::iterator iter = m_userInfoList.find(id);
	if(iter != m_userInfoList.end())
	{
		m_userInfoList.erase(iter);
	}
}

PLAYER_ID UserData::getUserId()
{
	return UserData::Get()->m_stUserinfo.id;
}


const char * UserData::getUserName()
{
	return UserData::Get()->m_stUserinfo.szName;
}

void UserData::setUserName(const char* newName)
{
	strcpy(UserData::Get()->m_stUserinfo.szName,newName);
}

const char*	UserData::getUserSession()
{
	return UserData::Get()->m_stUserinfo.userSession;
}

void UserData::SetUserInfo(USERINFO info)
{
	//CCLOG("UserData::SetUserInfo(){ %d, %s }",info.id,info.userSession);
	UserData::Get()->m_stUserinfo = info;
}

void UserData::SetBasicUserInfo(BasicUserInfo info)
{
	UserData::Get()->mBasicUserInfo = info;
}

void UserData::SetUserInfo( PLAYER_ID id, USERINFO& info )
{
	if(m_stUserinfo.id == id)
		return;

	m_userInfoList[id] = info;
}

uint64_t UserData::getTokenId()
{
	return UserData::Get()->m_stUserinfo.tokenId;
}

unsigned int UserData::GetUserLevel()
{
	return UserData::Get()->m_stUserinfo.level;
}

unsigned int UserData::GetUserExp()
{
	return UserData::Get()->m_stUserinfo.exp;
}

unsigned int UserData::GetUserMapId()
{
	return UserData::Get()->m_stUserinfo.mapId;
}

unsigned int UserData::GetUserXPos()
{
	return UserData::Get()->m_stUserinfo.xPos;
}

unsigned int UserData::GetUserYPos()
{
	return UserData::Get()->m_stUserinfo.yPos;
}

unsigned int UserData::GetUserOrient()
{
	return UserData::Get()->m_stUserinfo.orient;
}

unsigned int UserData::GetUserType()
{
	return UserData::Get()->m_stUserinfo.type;
}

unsigned int UserData::GetSpriteEnergy()
{
	return UserData::Get()->m_stUserinfo.m_spriteEnergy;
}

unsigned int UserData::GetGold()
{
	return UserData::Get()->m_stUserinfo.m_gold;
}

unsigned int UserData::GetDiamond()
{
	return UserData::Get()->m_stUserinfo.m_diamond;
}

unsigned int UserData::GetUserChip()
{
	return UserData::Get()->m_stUserinfo.m_spriteChip;
}

unsigned int UserData::GetVipLevel()
{
	return UserData::Get()->m_stUserinfo.m_vip;
}

unsigned int UserData::GetLastPort()
{
	return UserData::Get()->m_stUserinfo.port;
}

const char* UserData::GetLastIP()
{
	return UserData::Get()->m_stUserinfo.ip;
}

const char* UserData::GetLastServerName()
{
	return UserData::Get()->m_stUserinfo.lastServerName;
}

unsigned int UserData::getHeroHp()
{
	return UserData::Get()->m_stUserinfo.health_point;
}

unsigned int UserData::getHeroTotalHp()
{
	return UserData::Get()->m_stUserinfo.total_hp;
}

unsigned int UserData::getEnterManorTimes()
{
    return UserData::Get()->m_stUserinfo.m_enterManorTimes;
}

unsigned int UserData::getBuyManorTimes()
{
    return UserData::Get()->m_stUserinfo.m_buyManorTimes;
}

unsigned int UserData::getContinueloginTime()
{
    return UserData::Get()->m_stUserinfo.m_continueloginTime;
}

unsigned int UserData::getLoginRewardTime()
{
    return UserData::Get()->m_stUserinfo.m_loginRewardTime;
}

unsigned int UserData::getVipRewardTime()
{
    return UserData::Get()->m_stUserinfo.m_vipRewardTime;
}

unsigned int UserData::getPayRewardTime()
{
    return UserData::Get()->m_stUserinfo.m_payRewardTime;
}

unsigned int UserData::getBuyDiamondRewardTime()
{
    return UserData::Get()->m_stUserinfo.m_buyDiamondRewardTime;
}

unsigned int UserData::getPvaiRewardTime()
{
    return UserData::Get()->m_stUserinfo.m_pvaiRewardTime;
}

unsigned int UserData::getPalyHoldExp()
{
    return UserData::Get()->m_stUserinfo.m_palyHoldExp;
}

unsigned int UserData::getUserEffectiveness()
{
    return UserData::Get()->m_stUserinfo.effectiveness;
}

unsigned int UserData::getRagePoint()
{
	return UserData::Get()->m_stUserinfo.ragePoint;
}

unsigned int UserData::getTotalRagePoint()
{
	return UserData::Get()->m_stUserinfo.totalRangePoint;
}

unsigned int UserData::getSecRagePoint()
{
	return UserData::Get()->m_stUserinfo.secRagePoint;
}

unsigned int UserData::getSecTotalRagePoint()
{
	return UserData::Get()->m_stUserinfo.secTotalRangePoint;
}

// -------------------------------------------------------------------------------------------------

void EntityInfo::Set( INFOID* pInfo )
{
	if(pInfo)
	{
		std::map<PLAYER_ID, INFOID*>::iterator iter = m_infoList.find(pInfo->id);
		if(iter != m_infoList.end())
		{
			delete iter->second;
			iter->second = NULL;
		}
		m_infoList[pInfo->id] = pInfo;
	}
}

INFOID* EntityInfo::GetInfo( PLAYER_ID id )
{
	std::map<PLAYER_ID, INFOID*>::iterator iter = m_infoList.find(id);
	if(iter != m_infoList.end())
	{
		return iter->second;
	}
	return NULL;
}

void EntityInfo::Clear( void )
{
	for(std::map<PLAYER_ID, INFOID*>::iterator iter = m_infoList.begin(); iter != m_infoList.end(); iter++)
	{
		delete iter->second;
		iter->second = NULL;
	}

	m_infoList.clear();
}

// PLAY_ID
PLAYER_ID::PLAYER_ID()
	: m_uid(0)
	,m_regTime(0)
	, m_channelId(0)
{
	
}

PLAYER_ID::PLAYER_ID(unsigned int id,unsigned int regTime,unsigned int channel)
	: m_uid(id),
	  m_regTime(regTime),
	  m_channelId(channel)
{

}

PLAYER_ID::PLAYER_ID(const PLAYER_ID& playId)
{
	*this = playId;
}

void PLAYER_ID::setPlayId(uint32_t id, uint32_t regTime,uint32_t channel)
{
	m_uid = id;
	m_regTime = regTime;
	m_channelId = channel;
}

void PLAYER_ID::setPlayId(const PLAYER_ID& playId)
{
	m_uid = playId.getID();
	m_regTime = playId.getRegTime();
	m_channelId = playId.getChannel();
	//memcpy(this, &playId, sizeof(PLAYER_ID));
}

bool PLAYER_ID::isEqual(const PLAYER_ID& playId)
{
	if(m_uid == playId.getID() && m_regTime == playId.getRegTime())
	{
		return true;
	}

	return false;
}

PLAYER_ID&  PLAYER_ID::operator= (const PLAYER_ID& playId)
{
	if(this == &playId)
	{
		return *this;
	}

	setPlayId(playId);
	return *this;
}

bool PLAYER_ID::operator== (const PLAYER_ID& playId)
{
	bool ret = isEqual(playId);
	return ret;
}

bool PLAYER_ID::operator!= (const PLAYER_ID& playId)
{
	bool ret = isEqual(playId);
	return !ret;
}

bool PLAYER_ID::operator < (const PLAYER_ID& player) const
{
    if (m_uid == player.getID())
    {
        return m_regTime < player.getRegTime();
    } else
    {
        return (m_uid < player.getID());
    }

}

GUILD_ID::GUILD_ID()
{
	m_highId = 0;
	m_lowId = 0;
}

GUILD_ID::GUILD_ID(unsigned int high_id,unsigned int low_id)
{
	m_highId = high_id;
	m_lowId = low_id;
}

GUILD_ID::GUILD_ID(const GUILD_ID& guildId)
{
	m_highId = guildId.getHighID();
	m_lowId = guildId.getLowID();
}

void GUILD_ID::setGuildId(unsigned int high_id,unsigned int low_id)
{
	m_highId = high_id;
	m_lowId = low_id;
}

void GUILD_ID::setGuildId(const GUILD_ID& guildId)
{
	m_highId = guildId.getHighID();
	m_lowId = guildId.getLowID();
}
	
bool GUILD_ID::isEqual(const GUILD_ID& guildId)
{
	if(m_highId == guildId.getHighID() && m_lowId == guildId.getLowID())
	{
		return true;
	}

	return false;
}

GUILD_ID&  GUILD_ID::operator= (const GUILD_ID& guildId)
{
	if(this == &guildId)
	{
		return *this;
	}

	setGuildId(guildId);
	return *this;
}

bool GUILD_ID::operator== (const GUILD_ID& guildId)
{
	bool ret = isEqual(guildId);
	return ret;
}

bool GUILD_ID::operator!= (const GUILD_ID& guildId)
{
	bool ret = isEqual(guildId);
	return !ret;
}

bool GUILD_ID::operator < (const GUILD_ID& guildId) const
{
    if (m_lowId == guildId.getLowID())
    {
        return m_highId < guildId.getHighID();
    } else
    {
        return (m_lowId < guildId.getLowID());
    }

}

//bool PLAY_ID::operator< (const PLAY_ID& leftPlayer,const PLAY_ID& rightPlayer)
//{
//	return (leftPlayer.getID() < rightPlayer.getID());
//}