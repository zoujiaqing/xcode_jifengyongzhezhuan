#ifndef HERO_EFFECT_DEFINE_H
#define HERO_EFFECT_DEFINE_H

#include "SASpriteDefine.h"

enum HeroEffectType
{
	kParticle = 0,
};

enum EffectType
{
	EUnkownEffect = 0,
	NormalAttackEffect = 1,
	MonsterAttackEffect,
	CritAttackEffect,
	TranspointEffect,
};

enum RoleEffectType
{
	kBattleType = 1,
	kTranspointType,
};

class ArmatureEffectData
{
public:
	ArmatureEffectData()
	{

	}

	~ArmatureEffectData()
	{
		mRoleEffectTypeToData.clear();
	}

	std::map<EffectType ,ArmatureData > mRoleEffectTypeToData;
};

class RoleArmatureEffectData
{
public:
	RoleArmatureEffectData()
	{

	}

	~RoleArmatureEffectData()
	{
		for (std::map<int,ArmatureEffectData *>::iterator iter = kvRoleTypeAndValues.begin();
			iter != kvRoleTypeAndValues.end(); iter++)
		{
			delete((*iter).second);
		}

		kvRoleTypeAndValues.clear();
	}
public:
	std::map<int,ArmatureEffectData *> kvRoleTypeAndValues;
};

#endif