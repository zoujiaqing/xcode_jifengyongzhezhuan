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

#include "RoleBase.h"

class HeroFight : public RoleBase
{
public:
	HeroFight(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet, bool isCreateEffect = true);
	
	virtual ~HeroFight();

protected:

};


class OtherHeroFight : public RoleBase
{
public:
	OtherHeroFight(PLAYER_ID s_id, const char *name, int type, bool isPlayerSet, bool isCreateEffect = true);

	virtual ~OtherHeroFight();

	void SetJoystickVelocity(const CCPoint& v){m_JoystickVelocity = v;}
	virtual CCPoint GetJoystickVelocity(void);


	virtual bool DoNormalAttack(unsigned int skillId , const CCPoint& rolePos,  const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster);

	virtual bool DoSkillAttack(unsigned int skillId, const CCPoint& rolePos,  const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster);

protected:

	CCPoint m_JoystickVelocity;


};



