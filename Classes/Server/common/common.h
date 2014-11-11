#ifndef  __COMMON_H__
#define  __COMMON_H__

#include <stdint.h>
#include <map>
#include <string>

using namespace std;

// 统计信息题头定义
enum statistics_title {
	ST_DAILY_ROLE_PROPERTIES = 1,  // 每日的角色属性数据(所有用户，每日一条)
	ST_SPIRIT_INFO           = 2,  // 精灵信息(针对登陆用户, 精灵名称，出战状态)
	ST_SUIT_INFO             = 3,  // 装备信息(针对登陆用户，包括人物和精灵的装备)
	ST_SKILL_INFO            = 4,  // 技能信息(针对登陆用户，技能等级，技能名称，是否装备的状态)
	ST_DOWER_INFO            = 5,  // 天赋信息(针对登陆用户, 天赋名称，天赋等级)
	ST_SPIRIT_STONE_INFO     = 6,  // 灵石信息(针对登陆用户，灵石品质，灵石名称，灵石等级)
	ST_CARD_INFO             = 7,  // 卡牌信息(针对登陆用户, 卡牌名称，所属星座，卡牌等级)
	ST_GEM_INFO              = 8,  // 纹章信息(针对登陆用户, 纹章名称，等级，装备对象)
	ST_ROLE_LEVELUP          = 9,  // 角色升级记录
	ST_VIP_LEVELUP           = 10, // VIP升级记录
	ST_UNLOCK_SPIRIT         = 11, // 精灵解锁信息
	ST_ROLE_LOGIN_INFO       = 12, // 角色登陆信息
	ST_ROLE_LOGOUT_INFO      = 13, // 角色登出信息
	ST_GAIN_DIAMOND          = 14, // 钻石产出
	ST_CONSUME_DIAMOND       = 15, // 钻石消耗
	ST_GAIN_COIN             = 16, // 金币产出
	ST_CONSUME_COIN          = 17, // 金币消耗
	ST_GAIN_EXPLOIT          = 18, // 功勋产出
	ST_CONSUME_EXPLOIT       = 19, // 功勋消耗
	ST_GAIN_PRESTIGE         = 20, // 声望产出
	ST_EXCHANGE_ITEMS        = 21, // 商城兑换
	ST_STRENGTHEN_SUIT       = 22, // 装备强化
	ST_SYNTHESIS_SUIT        = 23, // 装备合成
	ST_SYNTHETIC_CONSUMPTION = 24, // 装备合成消耗
	ST_SKILL_LEVELUP         = 25, // 技能升级
	ST_DOWER_LEVELUP         = 26, // 天赋升级
	ST_MAIN_TASK_INFO        = 27, // 主线任务数据
	ST_BRANCH_MISSION_INFO   = 28, // 支线任务数据
	ST_ELITE_TASK_INFO       = 29, // 精英任务数据
	ST_DAILY_TASK_INFO       = 30, // 每日任务数据
	ST_TASK_RECORD           = 31, // 每日任务刷新纪录	
	ST_INSTANCE_INFO         = 32, // 副本纪录
	ST_MULTIPEOPLE_INSTANCE  = 33, // 多人副本
	ST_RESET_INSTANCE        = 34, // 副本重置
	ST_GAIN_CARD             = 35, // 卡牌产出
	ST_SYNTHESIS_CARD        = 36, // 卡牌合成
	ST_GYMKHANA              = 37, // 竞技场(PVAI)
	ST_AMPHITHEATER          = 38, // 斗技场(PVP)
	ST_IMPERIAL_CITY_GUARD   = 39, // 王城守卫
	ST_WORLD_BOSS            = 40, // 世界BOSS
	ST_GAIN_GEM              = 41, // 纹章产出
	ST_RECAST_GEM            = 42, // 纹章重铸
	ST_EXCHANGE_GEM          = 43, // 精华兑换
	ST_INLAY_GEM             = 44, // 纹章镶嵌
	ST_MANOR_REWARD          = 45, // 庄园奖励
	ST_REFRESH_MANOR         = 46, // 庄园刷新
	ST_REFRESH_EXPLORATION   = 47, // 云游刷新
	ST_EXPLORATION_RECORD    = 48, // 云游纪录
	ST_EXPLORATION_REWARD    = 49, // 云游奖励
	ST_FIND_SPIRTI_CONSUMPTION  = 50, // 寻灵消耗
	ST_GAIN_FROM_FINDING_SPIRIT = 51, // 寻灵产出
	ST_GAIN_PSIONIC          = 52, // 灵能产出
	ST_CONSUME_PSIONIC       = 53, // 灵能消耗
	ST_RECORD_SS_EXCHANGING  = 54, // 灵石兑换纪录
	ST_TRAINING              = 55, // 特训
	ST_DAILY_TARGET          = 56, // 每日目标
	ST_DAILY_REWARD          = 57, // 每日奖励
	ST_WELFARE_REWARD        = 58, // 每日福利奖励
	ST_DEAD_RECORD           = 59, // 死亡纪录
	ST_REVIVING_RECORD       = 60, // 复活纪录
	ST_TASK_INFO             = 61, // 任务数据(忽略27和28，只有两种状态：接受和完成)
	ST_JOIN_INSTANCE         = 62, // 进入副本
	ST_LEAVE_INSTANCE        = 63, // 离开副本(玩家退去或死亡)
	ST_COMPLETE_INSTANCE     = 64, // 完成副本
	ST_ITEM_COMPOSE_CONSUME  = 65, // 兑换中物品消耗
	ST_ITEM_COMPOSE_GAIN     = 66, // 兑换中物品产出
	ST_ADD_EXP               = 67, // 经验提升
	ST_LUCKY_OCT             = 68, // 运营活动
	ST_AUTOBATTLE            = 69, // 副本挂机
	ST_PLYAER_HURT_WB_ONCE   = 70, // 玩家每次给世界boss的伤害值
};

// 渠道统计定义
enum statistics_channel {
	SC_DAILY_TARGET           = 0,  // 每日目标
	SC_LOGIN_GIFTS            = 1,  // 登陆礼包   <每日领取>
	SC_DAILY_SALARY           = 2,  // 每日资薪   <每日领取>
	SC_GYM_OUTPUTS            = 3,  // 竞技场产出 <每日领取>
	SC_OFFLINE_EXP            = 4,  // 离线经验   <每日领取>
	SC_VIP_REWARD             = 5,  // VIP奖励    <每日领取>
	SC_FISRT_RECHARGE         = 6,  // 首冲奖励   <每日领取>
	SC_DAILY_TASK             = 7,  // 每日任务
	SC_REFRESH_DAILY_TASK     = 8,  // 刷新每日任务 
	SC_REFRESH_FIVE_STAR_TASK = 9,  // 刷新五星任务
	SC_TASK                   = 10, // 完成任务(包括主线和支线任务)
	SC_HERO_TREASURE          = 11, // 勇者宝藏
	SC_UGCITY_INSTANCE        = 12, // 地下城副本
	SC_RESET_UG_INSTANCE      = 13, // 重置地下城副本
	SC_BOX_REWARD             = 14, // 开地下城宝箱
	SC_RESET_UG_BOS           = 15, // 重置地下城宝箱
	SC_AUTO_BATTLE_UG_CD      = 16, // 自动战斗地下城冷却CD
	SC_INSTANCE               = 17, // 普通副本 	
	SC_VIP_INSTANCE           = 18, // 精英副本
	SC_RESET_VIP_INSTANCE     = 19, // 重置精英副本
	SC_MULTI_INSTANCE         = 20, // 多人副本
	SC_AB_INSTANCE_CD         = 21, // 自动战斗 普通副本冷却CD
	SC_AB_VIP_INSTANCE_CD     = 22, // 自动战斗 精英副本冷却CD
	SC_PVP                    = 23, // 角斗场
	SC_PVAI_CD                = 24, // PVAI冷却CD
	SC_PVAI                   = 25, // 竞技场(PVAI)
	SC_WORLD_BOSS             = 26, // 世界BOSS
	SC_WORLD_BOSS_CD          = 27, // 世界BOSS冷却CD
	SC_MAIN_DEFENSE           = 28, // 主城防守
	SC_MAIL                   = 29, // 邮件发放
	SC_CONSUME_FOR_SS         = 30, // 寻灵
	SC_SS_CALL_MONSTER        = 31, // 寻灵召唤精灵
	SC_SS_OPEN_PORE           = 32, // 灵石开孔
	SC_EXPLORE                = 33, // 云游
	SC_EXPLORE_MAP_REFRESH    = 34, // 云游地图刷新
	SC_BUY_CARDS              = 35, // 购买卡牌
	SC_CARD_OPEN_PORE         = 36, // 卡牌开孔
	SC_CONSUME_FOR_UP_EQUIP   = 37, // 升级装备(装备强化)
	SC_STRENGTH_EQUIP_CD      = 38, // 装备强化CD
	SC_COMPOSE_EQUIP          = 39, // 装备合成
	SC_EQUIP_OPEN_PORE        = 40, // 装备打孔
	SC_EMBED_EQUIP            = 41, // 装备镶嵌
	SC_SS_RECAST              = 42, // 宝石重铸
	SC_LEARN_SKILL            = 43, // 学习技能
	SC_CONSUME_FOR_SKILL      = 44, // 升级技能
	SC_SKILL_OPEN_PORE        = 45, // 技能开孔
	SC_LEARN_DOWER            = 46, // 学习天赋
	SC_CONSUME_FOR_DOWER      = 47, // 升级天赋
	SC_DOWER_UP_CD            = 48, // 天赋升级冷却CD
	SC_MANOR_HUNT             = 49, // 庄园狩猎
	SC_MANOR_GATHER           = 50, // 庄园采集
	SC_REFRESH_MANOR          = 51, // 庄园刷新
	SC_MANOR_UP_TO_A          = 52, // 庄园刷新至A级
	SC_MANOR_OPEN_PORE        = 53, // 庄园开孔
	SC_BUY_MANOR_NUM          = 54, // 庄园购买次数
	SC_REMOVE_MANOR_CD        = 55, // 消除庄园冷却CD
	SC_REVIVAL                = 56, // 复活
	SC_PACKET_OPEN_PORE       = 57, // 包裹开孔
	SC_WAREHOUSE_OPEN_PORE    = 58, // 仓库开孔
	SC_CONSUME_FOR_TRAINING   = 59, // 训练 
	SC_CONSUME_FOR_BACK_FROM_TRASH = 60, // 回购回收站的物品(购买物品)
	SC_BUY_ITEMS              = 61, // 购买道具
	SC_SELL_ITEMS             = 62, // 出售物品(道具)
	SC_REFRESH_STORE_CD       = 63, // 刷新商店冷却CD
	SC_ON_HOOK_EXP_SWITCH     = 64, // 挂机经验转换
	SC_DIAMAND_EXCHANGE_PP    = 65, // 钻石兑换体力
	SC_LEVEL_GIFTS            = 66, // 等级礼包
	SC_EXCHANGE_STORE         = 67, // 兑换商店
	SC_EXCHANGE_ITEMS         = 68, // 物品兑换
	SC_UNKNOWN                = 69, // 未知渠道
	SC_PVP_WON                = 70, // PVP获胜
	SC_PVAI_WON               = 71, // PVAI获胜
	SC_DAILY_LOGIN            = 72, // 每日登陆(登陆礼包)
	SC_PVAI_CL_RD             = 73, // PVAI累计奖励
	SC_MOVE_TO_TRASH          = 74, // 移动到回收站
	SC_ACTIVITIES             = 75, // 活动
	SC_RMB                    = 77, // RMB兑换
	SC_DAILY_REWARD           = 78, // 每日登陆奖励，vip每日领取...
	SC_LUCKY_OCT              = 79, // 运营活动
	SC_VIP_BUY_DIAMOND        = 80, // vip购买钻石
	SC_ONLINE_AWARD		      = 81, // 在线礼包 
	SC_TOPUP_AWARD            = 82, // 首充奖励
	SC_EQUIP_STRENGTH         = 83,	// 装备强化
	SC_EQUIP_DECOMPOSE        = 84,	// 装备分解
	SC_STAR_AWARD			  = 85	// 副本星数奖励
};
enum SYS_BASE_T {
	SYS_BASE_ACTIVITYCENTER_TYPE	=	1000,		// 活动中心起始位置
};

//----------------------------------------------------------------------
typedef uint32_t UserID_t;


//----------------------------------------------------------------------
/* *
 * @brief 对userid + reg_time + channel_id结构定义
 * */
class player_key_t {
public:
	player_key_t(): userid(0), reg_time(0), channel_id(0) { /*nothing*/ }
	player_key_t(UserID_t id , uint32_t tm , uint32_t c_id ) 
		: userid(id), reg_time(tm), channel_id(c_id) { /*nothing*/ }

	UserID_t id() const { return userid; }
	uint32_t regtime() const { return reg_time; }
	UserID_t channelid() const { return channel_id; }

	player_key_t &operator=(const player_key_t &key) {
		if(this == &key) return *this;
		this->userid = key.userid;
		this->reg_time = key.reg_time;
		this->channel_id = key.channel_id;
		return *this;
	}
public:
	UserID_t 	userid;
	uint32_t 	reg_time;
	uint32_t 	channel_id;
};

inline bool operator>(const player_key_t &key1, const player_key_t &key2)
{
    if (key1.userid > key2.userid) {
        return true;
    } else if(key1.userid == key2.userid) {
        if (key1.reg_time < key2.reg_time) {
            return true;
        } else if (key1.reg_time == key2.reg_time) {
			if(key1.channel_id > key2.channel_id) {
				return true;
			}
		}
    }
    return false;
}

inline bool operator<(const player_key_t &key1, const player_key_t &key2)
{
    if (key1.userid < key2.userid) {
        return true;
    } else if(key1.userid == key2.userid) {
        if (key1.reg_time > key2.reg_time) {
            return true;
        } else if (key1.reg_time == key2.reg_time) {
			if(key1.channel_id < key2.channel_id) {
				return true;
			}
		}
    }
    return false;
}

inline bool operator==(const player_key_t &key1, const player_key_t &key2)
{
	if (key1.userid == key2.userid && key1.reg_time == key2.reg_time && key1.channel_id == key2.channel_id) {
		return true;
	} else {
		return false;
	}
}

inline bool operator!=(const player_key_t &key1, const player_key_t &key2)
{
	if (key1.userid != key2.userid || key1.reg_time != key2.reg_time || key1.channel_id != key2.channel_id) {
		return true;
	} else {
		return false;
	}
}
//----------------------------------------------------------------------

inline uint32_t get_low32_bits_value(uint64_t value)
{
	return value & 0xFFFFFFFF;
}
inline uint32_t get_high32_bits_value(uint64_t value) 
{
	return value >> 32;
}
inline uint64_t get_64bits_value(uint32_t low, uint32_t high)
{
	uint64_t value = high;
	return value << 32 | low;
}
inline uint32_t get_incr_num()
{       
    static uint32_t base = 0;
    if (base == 1000000) { 
        base = 0;
    }       
    return base++;
}
//----------------------------------------------------------------------
inline std::string string_replace(const std::string &str, const std::string &str_sub_src, const std::string &str_sub_dst)
{
	std::string new_str = str;
	std::string::size_type pos = 0;
	std::string::size_type src_len = str_sub_src.size();
	std::string::size_type dst_len = str_sub_dst.size();

	while((pos=new_str.find(str_sub_src, pos)) != std::string::npos) {
		new_str.replace(pos, src_len, str_sub_dst);
		pos += dst_len;
	}
	return new_str;
}


#define INITAITOR_KEY ("initiator_key")
#define RECIPIENT_KEY ("recipient_red")

#endif  /*__COMMON_H__*/
