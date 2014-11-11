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
#include "AdvPch.h"
#include <stdlib.h>
#include <map>

#define MONSTER_PLIST "Data/monster.plist"

using namespace std;


struct Monster 
{
	unsigned short numbersID;			// equals to family ID
	unsigned short mID; 
	char defName[KMAX_ELF_NAME_LEN];
	unsigned char type;
	unsigned char growType;
	unsigned short hp;
	unsigned short atk;
	unsigned short def;
	unsigned short spAtk;
	unsigned short spDef; 
	unsigned short spd;
	unsigned short yieldingExp;
	unsigned char catchRate;
	unsigned short yieldingEV;
	unsigned short evolvesFrom;
	unsigned short evolvesTo;
	unsigned short evolvingLv;
	unsigned short maxHeight;
	unsigned short minHeight;
	unsigned short maxWeight;
	unsigned short minWeight;
	unsigned short features;
	unsigned char maxGender;
	unsigned char minGender;
	unsigned char freeForbidden;

	Monster()
	{ 

	}

	Monster(const  Monster &monster)
	{
		*this = monster;
	}
};

typedef  Monster Monsters_t;
typedef std::map<int, Monsters_t> MonstersDictionary;