#pragma once

#include "SpriteElf.h"
#include "Singleton_t.h"
#include "SpriteSeer.h"
#include <map>
#include "vector"

class ElfAssistSkillObserver;

class SpriteElfManager : public TSingleton<SpriteElfManager>
{
	//friend class SpriteElf;
public:
	enum EFairyPos
	{
		E_Fight_Left = 0,
		E_Fight_Right,
		E_Assist_Pos_1,
		E_Assist_Pos_2
	};
public:    
	SpriteElfManager();
	virtual ~SpriteElfManager(); 

	/**
	* Instruction : 创建精灵对象
	* @param 
	*/	
	SpriteElf*	CreateInstance(PLAYER_ID s_id,int type, int grade = 0);

	// Note: 创建辅助战斗精灵
	SpriteElf* CreateAssistInstance(PLAYER_ID s_id,int type);

	/**
	* Instruction : 创建一个精灵对象
	* @param 
	*/
	SpriteElf* CreateInstance(int type, int grade = 0);
	bool IsHasElf(PLAYER_ID s_id,unsigned int elfTypeId);
	SpriteElf* GetOneInstanceFromPos(PLAYER_ID s_id,unsigned int pos);
	/**
	* Instruction : 根据Id获取一个精灵对象
	* @param 
	*/	

	SpriteElf* GetOneInstance(PLAYER_ID s_id,unsigned int elfTypeId);
	bool GetInstances(PLAYER_ID s_id,std::map<unsigned int,SpriteElf*>& elfs);
	bool IsHasElfAtPos(PLAYER_ID s_id,unsigned int pos);

	/**
	* Instruction : 获取所有的精灵实例
	* @param 
	*/	
	const std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >& GetAllInstance(void) const 
	{
		return m_mapElfIdAndInstances;
	}

	/**
	* Instruction : 根据精灵Id删除对象
	* @param 
	*/	
	bool RemoveOneElf(PLAYER_ID s_id,unsigned int elfTypeId,bool bDeleteElf  = true);

	// Note: 删除英雄绑定的所有精灵
	bool RemoveElfFromHero(PLAYER_ID s_id,bool bDeleteElf  = true);
    bool RemovePosElfFromHero(PLAYER_ID s_id);
	/**
	* Instruction : 绑定精灵岛主角身上
	* @param 
	*/
	void AttachElfToOneHero(unsigned int elfTypeId, PLAYER_ID heroId, unsigned int grade, SpriteSeer* parent = 0);

	/**
	* Instruction : 清空精灵
	* @param 
	*/
	void ClearAllData();

	/**
	* Instruction : 删除所有的精灵
	* @param 
	*/
	void RemoveAllElfInstance();

	/**
	* Instruction : 插入一个追逐状态
	* @param 
	*/
	void PushOneElfChaseState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData = 0);

	/**
	* Instruction : 插入一个靠近状态
	* @param 
	*/
	void PushOneElfCloseToState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData = 0);

	/**
	* Instruction : 插入一个攻击状态
	* @param 
	*/
	void PushOneElfAttackState(SpriteSeer* pHero,int skillId,BaseElfAttachData* pData = 0);

	/**
	* Instruction : 检测是否有声望可以解锁精灵
	* @param 
	*/
	void CheckWhetherNewElfReleaseByReputation();

	/**
	* Instruction : called by LevelLayer
	* @param 
	*/	
	void Update(float dt);

	void SetPlayerElfPos(PLAYER_ID id,unsigned int elfId,EFairyPos pos);
	bool GetPlayerElfPos(PLAYER_ID id,unsigned int elfId,EFairyPos& pos);

	unsigned int GetFairyIdByPos(PLAYER_ID id, EFairyPos pos);
	int GetFairyRageBasicByPos(PLAYER_ID id, EFairyPos pos);
	int GetFairyRageRateByPos(PLAYER_ID id, EFairyPos pos);

	const std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> >& GetElfPosInfo(){
		return m_mapElfIdAndPos;
	}
	bool IsHasElfPlay(PLAYER_ID s_id);

	void DoObserverEvent(PLAYER_ID id,unsigned int elfId,unsigned int type);
	void AddAssistSkillStartObserver(ElfAssistSkillObserver* pObserver);
	void RemoveAssistSkillObserver(ElfAssistSkillObserver* pObserver);\

	// Note: 创建辅助精灵
	void CreateAssistElfs();
protected:
	/**
	* Instruction : 添加一个实例
	* @param 
	*/	
	bool Add(PLAYER_ID id,SpriteElf * instance,unsigned int elfTypeId);
	bool AddAssistElf(PLAYER_ID id,SpriteElf * instance,unsigned int elfTypeId);
public:
	// Note: 公共数据
	static cocos2d::CCPoint s_fixDistanceToHero;
protected:
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> > m_mapElfIdAndInstances;
	std::map<PLAYER_ID, std::map<unsigned int,EFairyPos> > m_mapElfIdAndPos;

	std::vector<ElfAssistSkillObserver*> m_vecAssistSkillEventObserver;	

	std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> > m_mapAssistElfIdAndInstances;
};
