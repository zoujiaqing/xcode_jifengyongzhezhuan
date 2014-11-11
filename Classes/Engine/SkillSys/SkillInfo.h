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
#pragma once
#include <map>
#include <string>
#include "PhysicsLayer.h"
#include "StateEventType.h"
#include "SASpriteDefine.h"
#include "UserData.h"
#include "EncodeValue.hpp"

typedef enum Skill_Phase
{
	PHASE_UNKNOWN	= -1,
	PHASE_PERFORM	= 1,
	PHASE_SHOW	= 2,
	PHASE_IMPACT= 3,
} SKILL_PHASE;


typedef enum Spell_Show_Type	
{
	SHOW_IMMEDIATE,     // 在目标处瞬间出现
	SHOW_FLYINGTO,      // 从施法者飞向目标者
	SHOW_CONTINUOUS,    // 连续类表现
} SPELL_SHOW_TYPE;

class BasicUserBattleInfo;
class UserSkillPart;
class UserSkillInfo;

struct ClientBasicUserBattleInfo {
	EncodeValue<unsigned int> physical_attack ; // 物理攻击
	EncodeValue<unsigned int> magic_attack; // 魔法攻击
	EncodeValue<unsigned int> skill_attack; // 技能攻击
	EncodeValue<unsigned int> physical_defence; // 物理防御
	EncodeValue<unsigned int> magic_defence ; // 魔法防御
	EncodeValue<unsigned int> skill_defence; // 技能防御
	EncodeValue<unsigned int> health_point; // 生命值
	EncodeValue<unsigned int> accurate ; // 精准
	EncodeValue<unsigned int> dodge ; // 闪避
	EncodeValue<unsigned int> wreck; // 破击
	EncodeValue<unsigned int> parry; // 格挡
	EncodeValue<unsigned int> critical_strike; // 暴击
	EncodeValue<unsigned int> tenacity; // 韧性
	EncodeValue<unsigned int> slay; // 必杀
	EncodeValue<unsigned int> courage; // 勇气
	EncodeValue<unsigned int> charm; // 魔力
	EncodeValue<unsigned int> trick; // 技巧
	EncodeValue<unsigned int> speed; // 极速
	EncodeValue<unsigned int> proficiency; // 潜能
	EncodeValue<unsigned int> current_hp ; // 生命值

	ClientBasicUserBattleInfo(void){}
	ClientBasicUserBattleInfo(const BasicUserBattleInfo& info);

	const ClientBasicUserBattleInfo& operator = (const BasicUserBattleInfo& info);
};

struct ClientUserSkillPart 
{
	EncodeValue<unsigned int> part;
	ClientBasicUserBattleInfo exp_battle_info;

	EncodeValue<float> continue_time;
	EncodeValue<unsigned int> can_attack_number;
	EncodeValue<unsigned int> hurt;
	EncodeValue<float> continue_time2;

	ClientUserSkillPart(){}
	ClientUserSkillPart(const UserSkillPart& info);

	const ClientUserSkillPart& operator = (const UserSkillPart& info);
};

struct ClientUserSkillInfo 
{
	EncodeValue<unsigned int> skill_id;
	EncodeValue<unsigned int> level;
	EncodeValue<unsigned int> part;
	EncodeValue<unsigned int> skill_level;
	EncodeValue<unsigned int> money;
	EncodeValue<unsigned int> exploit;
	EncodeValue<float> own_cd;
	EncodeValue<float> share_cd;

	std::vector<ClientUserSkillPart>		parts;

	EncodeValue<unsigned int> max_level;

	ClientUserSkillInfo(void){}
	ClientUserSkillInfo(const UserSkillInfo & rhs);

	const ClientUserSkillInfo& operator = (const UserSkillInfo& info);
	
};



typedef struct tagDispData
{
	/// sound 
	unsigned int SoundId;
	/// effect Id
	unsigned int EffectId;
}DISP_DATA, *LPDISP_DATA;
typedef const DISP_DATA*   LPCDISP_DATA;


typedef struct tagPerform_Phase
{
	float       fTotalTime;
	DISP_DATA   DispData;

	tagPerform_Phase()
	{
		fTotalTime  = 0.0f;
	}

	~tagPerform_Phase() {};
} PERFORM_PHASE, *LPPERFORM_PHASE;
typedef const PERFORM_PHASE*   LPCPERFORM_PHASE;



typedef struct tagShow_Phase
{
	// Public data
	SPELL_SHOW_TYPE emShowType;
	unsigned int    SoundId;

	// Immediate show data
	unsigned int  CasterEffectId;
	unsigned int  CasterEffectPos;
	unsigned int  TargetEffectId;

	int  TargetEffectPos;

	// Flying show data
	unsigned int    FlyingEffectId;
	int  StartPos;
	int  TargetPos;
	float   fFlyingSpeed;

	tagShow_Phase()
	{
		emShowType  = SHOW_IMMEDIATE;
		SoundId  = 0;

		FlyingEffectId = 0;
		fFlyingSpeed = 0.0f;

		CasterEffectId = 0;
		TargetEffectId = 0;

		StartPos = 0;
		TargetPos = 0;
	}
} SHOW_PHASE, *LPSHOW_PHASE;
typedef const SHOW_PHASE*   LPCSHOW_PHASE;


typedef struct tagImpact_Phase
{
	float       fTotalTime;
	DISP_DATA   DispData;
	unsigned   int  ABoxAnimationId;

	tagImpact_Phase()
	{
		fTotalTime  = 0.0f;
	}

	~tagImpact_Phase() {};
} IMPACT_PHASE, *LPIMPACT_PHASE;
typedef const IMPACT_PHASE* LPCIMPACT_PHASE;


typedef struct tagSkillInfo
{
	struct ANIMATION_INFO
	{
		unsigned int animationID;
		bool isFlipX;
		bool isFlipY;
		ANIMATION_INFO()
			: animationID(-1)
			,isFlipX(false)
			,isFlipY(false)
		{
			
		}
		
	};

	tagSkillInfo(void)
		:Id(0)
		,partID(0)
		,linkSkillId(0)
		,linkTime(0.0f)
		,displacement(0)
		,beatBackDistance(0)
		,isSpecial(false)
		,canCommonBreak(false)
		,canSpecialBreak(false)
		,damage(0)
		,criticalRate(0.0f)
		,attackNumber(0)
		,canControl(false)
		,continueTime(0.0f)
		,whichTime(0)
		,type(ToNormalAttack)
		,iShakeCamera(0)
		,bCameraStartTime(0.0f)
		,fCoolTime(0.0f)
		,SoundId(0)
		,bSoundLoop(false)
		,aim(E_AIM_NONE)
		,effectId(0)
		,linkEffectId(0)
		,longDistanceAttackRange(0)
		,bAdaptationDistance(false)
		,bBasedOnFrame(false)
		,roleType(kTypeFighter_Boy)
		,girleVoiceID(0)
		,boyVoiceID(0)
		,ragePoint(0)
		,bInvincible(false)
		,skillPriority(0)
		,skillConflict(0)
		,bRotateEffect(false)
		,effectZOrder(E_DYNAMIC)
	{

	}

	enum ATTACK_TYPE
	{
		E_NORMAL = 1,
		E_MAGIC,
		E_SKILL,
		E_FAIRY_SKILL,

		E_DODGE,	// 闪避
		E_PARRY,	// 格挡
		E_STRIKE,	// 暴击
	};

	enum AIM_TYPE
	{
		E_AIM_NONE,
		E_AIM_ENEMY,
		E_AIM_HERO
	};

	enum EFFECT_Z_ORDER
	{
		E_DYNAMIC,
		E_BEHIDE_HERO,
		E_FRONT_HERO
	};

	// skill Id
	int Id;
	EventType type;

	int partID;
	int linkSkillId;

	ANIMATION_INFO animation[4];
	
	float linkTime;
	int displacement;
	int beatBackDistance;
	bool isSpecial;
	bool canCommonBreak;
	bool canSpecialBreak;
	int damage;
	float criticalRate;

	// 主角该技能过程中可以击中的次数
	int attackNumber;

	// 技能释放过程中是否可以移动（0为不行，1为可以）
	bool canControl;

	// 该技能可持续时间
	float continueTime;

	// 主角形成位移时的速度由哪个时间控制（1为动画时间，2为continue_time。。。）
	int whichTime;

	/// skill description
	char   SkillDesp[512];


	/// camera effect
	int   iShakeCamera;
	float  bCameraStartTime;
	float  fCoolTime;
	
	int SoundId;

	bool bSoundLoop;
	AIM_TYPE aim;

	int effectId;
	int linkEffectId;
	int effectsoundID;

	std::string actionName;
	std::string actionIcon;

	int longDistanceAttackRange;
	float longDistanceEffectSpeed;
	bool bShowFarWay;	// 在某个地方瞬间出来
	bool bDisappear;
	float alpha;

	float ownCD;
	float shareCD;

	bool bAdaptationDistance;

	bool bBasedOnFrame;

	RoleJobType roleType;

	bool bBuff;

	ATTACK_TYPE attackType;

	unsigned int firstSkillID;

	unsigned int boyVoiceID;
	unsigned int girleVoiceID;
	unsigned int ragePoint;

	bool bInvincible;

	int skillPriority;
	int skillConflict;

	EFFECT_Z_ORDER effectZOrder;
	bool bRotateEffect;

	/// states of skill 
	PERFORM_PHASE  Perform;
	SHOW_PHASE  Show;
	IMPACT_PHASE  Impact;

}SKILL_INFO, *LPSKILL_INFO;
typedef const SKILL_INFO*   LPCSKILL_INFO;


typedef struct tagSkillSlotInfo
{
	unsigned int slotIndex;
	unsigned int skillId;

	tagSkillSlotInfo(){
		slotIndex = 0;
		skillId = 0;
	}
}SKILL_SLOT_INFO, *LPSKILL_SLOT_INFO;
typedef const SKILL_SLOT_INFO*   LPCSKILL_SLOT_INFO;

struct FAIRY_SKILL_INFO : public INFOID, public BATTLEINFO
{
	enum TYPE
	{
		E_ATTACK,
		E_BUFFER,

	};

	enum ASSIST_EFFECT_TYPE
	{
		E_EFFECT_UnKnow = 0,
		E_EFFECT_ATTACK,
		E_EFFECT_DEFEND,
		E_EFFECT_HP
	};

	enum STYLE
	{
		E_SkillAttack = 0,
		E_NormalAttack,
		E_AssistAttack,
	};

	enum AIM
	{
		E_HERO,
		E_ENEMY,
		E_FULL_SCREEN,

	};

	enum EFFECT_Z_ORDER
	{
		E_DYNAMIC,
		E_BEHIDE_HERO,
		E_FRONT_HERO
	};

	FAIRY_SKILL_INFO(void)
		: fairy_ID(0)
		, type(E_ATTACK)
		, style(E_SkillAttack)
		, assistEffectType(E_EFFECT_UnKnow)
		, aim(E_HERO)
		, bLock(false)
		, attack_number(0)
		, continueTime(0.0f)
		, effectID(0)
		, longDistanceAttackRange(0)
		, longDistanceEffectSpeed(0)
		, soundID(0)
		, effectSoundID(0)
		, hurt(0)
		, hurtAddValue(0)
		, effectZOrder(E_DYNAMIC)
		, ownCD(0.0f)
		, isEffectDisappearWhenHit(false)
		, isBasedOnCd(false)
		, bloodDownPercent(0)
		,effectAmi(100000)
	{
		enemyNumAtRange.first = 0;
		enemyNumAtRange.second = 0;
	}

	unsigned int  fairy_ID;
	TYPE type;
	STYLE style;
	ASSIST_EFFECT_TYPE assistEffectType;
	AIM aim;
	bool bLock;
	tagSkillInfo::ANIMATION_INFO animation[4];
	int attack_number;
	float continueTime;
	int effectID;
	int longDistanceAttackRange;
	int longDistanceEffectSpeed;
	int soundID;
	int effectSoundID;
	int hurt;
	int hurtAddValue;
	int effectAmi ;
	// Note: 辅助精灵相关属性
	bool isEffectDisappearWhenHit;
	bool isBasedOnCd;
	float bloodDownPercent;
	std::pair<unsigned int,unsigned int> enemyNumAtRange;

	EFFECT_Z_ORDER effectZOrder;

	float ownCD;

};