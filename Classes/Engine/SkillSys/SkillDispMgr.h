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

#include "SkillInfo.h"
#include "Singleton_t.h"
#include "ABoxAnimation.h"
#include "PhysicsLayer.h"
#include <map>
#include <vector>
#include "StateEventType.h"
#include "SASpriteDefine.h"
#include "cs_core.pb.h"

using namespace std;

class ABoxAnimation;


class SkillDispMgr : public TSingleton<SkillDispMgr>
{
public:
	SkillDispMgr();
	virtual ~SkillDispMgr();

public:

	void	Load(const char* pSkillFile, const char* pAnimationTableFile);
	void	LoadFairy(const char* pSkillFile, const char* pAnimationTableFile);

	/// add one skill data
	bool	AddSkillInfo(SKILL_INFO  & skill);

	/// remove one skill data 
	void	RemoveSkillInfo(SKILL_INFO & skill);

	/// query skill information
	const	SKILL_INFO* GetSkillInfo(unsigned int skillId = 0); 
	const	FAIRY_SKILL_INFO* GetFairySkillInfo(unsigned int skillId = 0); 
	const	FAIRY_SKILL_INFO* GetFairyNormalSkillInfo(unsigned int skillId = 0); 
	const	FAIRY_SKILL_INFO* GetFairyAssistSkillInfo(unsigned int skillId = 0); 

	const std::vector<int>& GetAllSkillID(RoleJobType type){return m_roleAllSkillID[type];}

	/// play current skill
	bool	PlaySkill(unsigned int skillId, SKILL_PHASE phase);
    
    /// play skill
	bool    PlaySkill(const SKILL_INFO *skill, SKILL_PHASE phase);

	/// remove loop sound
	void	StopLoopSound(int skillId);

	/// add ABoxAnimation
	bool   AddCollisionBoxAnim(unsigned int Id, ABoxAnimation* boxAnimation);

	/// get collision box animation data
	ABoxAnimation* GetCollisionBoxAnim(unsigned int Id);

    void clearCD();
	void SetCD(PLAYER_ID userID, int skillID);
	bool IsCD(PLAYER_ID userID, int skillID);
    
    float getShareCDTime(PLAYER_ID userID);
    float getOwnCDTime(PLAYER_ID userID, int skillId);

	bool  PushOneSKillUserInfoToTable(PLAYER_ID userID, unsigned int skillId,UserSkillInfo skillInfo);
	bool GetOneSkillInfoById(PLAYER_ID userID, unsigned int skillid,ClientUserSkillInfo& userSkillInfo);

	bool PushOneSlotInfoToTable(unsigned int slotIndex,unsigned int skillId);

	unsigned int GetSkillIDFromSlot(int slotIndex);
	int GetSkillSlotCount(void){return m_mapSlotIndexAndInfo.size();}
	bool IsEquipSkill(unsigned int id);

	const std::map< unsigned int, SKILL_SLOT_INFO>& GetSlotTable(){
		return m_mapSlotIndexAndInfo;
	}

	unsigned int GetFairySkillID(unsigned int fairyID);
	unsigned int GetFairyNormalSkillID(unsigned int fairyID);
	unsigned int GetFairyAssistSkillID(unsigned int fairyID);

	void Update(float dt);

	unsigned int GetNormalAttackID( PLAYER_ID userID, int roleType );


	void ClearUserSkillData();
protected:
	/// clear all the collision box animation data
	void  ClearCollisionBoxAnim();
	
	void	LoadAnimationTable(const char* pFile);
	void	LoadSkill(const char* pFile);

	void	LoadFairyAnimationTable(const char* pFile);
	void	LoadFairySkill(const char* pFile);

	EventType GetTypeFromActionName(const std::string& name);

protected:
	map<unsigned int , SKILL_INFO> m_TableSkillInfo;
	map<unsigned int , ABoxAnimation* > m_TableBoxAnimation;
	
	// 由左边动画的ID找到各个方向的ID
	struct ANIMATION_DIRECTION_ID
	{
		unsigned int down;
		unsigned int up;
	};
	map<unsigned int, ANIMATION_DIRECTION_ID> m_TableAnimatiionMap;

	
	std::map<PLAYER_ID, float> m_shareCD;
	std::map<PLAYER_ID, std::map<int, float> >  m_ownCD;

	std::map< RoleJobType, std::vector<int> > m_roleAllSkillID; 

	std::map<PLAYER_ID, std::map< unsigned int, ClientUserSkillInfo > > m_mapSkillIdAndUserSkillInfo;
	std::map< unsigned int, SKILL_SLOT_INFO> m_mapSlotIndexAndInfo;

	std::map<unsigned int, unsigned int> m_fairyAnimationMap;
	std::map<unsigned int, FAIRY_SKILL_INFO> m_fairySkillInfo;
	std::map<unsigned int, FAIRY_SKILL_INFO> m_fairyNormalSkillInfo;
	std::map<unsigned int, FAIRY_SKILL_INFO> m_fairyAssistSkillInfo;
};