#pragma once

#include "UserData.h"

//#include "vector"

//enum ElfState
//{
//	kType_Elf_WAITING = 1,
//	kType_Elf_NEW,
//	kType_Elf_FIGHT,
//	kType_Elf_UnKnow,
//};

// Note: 精灵绑定数值
//class ElfAttachData
//{
//public:
//	ElfAttachData();
//
//	void SetElfId(int elfId);
//	void SetStateId(int stateId);
//public:
//	unsigned int nElfId;
//	int nState;
//};

// Note: 精灵相关数据
class SpriteElfDataCenter
{
public:    	
	SpriteElfDataCenter();
	virtual ~SpriteElfDataCenter(); 

	static SpriteElfDataCenter* Get();
	static void Destroy();

	/**
	* Instruction : 添加一项数据 1 从网络层获取 2 奖励等获取
	* @param 
	*/	
	//void AddOneAttachData(ElfAttachData data);
	/**
	* Instruction : 装备某一精灵
	* @param 
	*/	
	void EquipOneElf(unsigned int elfId);	
	/**
	* Instruction : 发送获取精灵列表
	* @param 
	*/
	void SendGetElfListMessage(PLAYER_ID roleId,const char* name);
	/**
	* Instruction : 设置精灵的状态
	* @param 
	*/
	void SendSetElfStateMessage(unsigned int elfId,int state);
	/**
	* Instruction : 设置某项数据状态
	* @param 
	*/	
	//void SetOneElfState(unsigned int elfId,int state);
	/**
	* Instruction : 获取某精灵的状态
	* @param 
	*/
	//unsigned int GetOneElfState(unsigned int elfId);

	//std::vector<ElfAttachData> GetElfData()
	//{
	//	return m_vecElfData;
	//}

	/**
	* Instruction : 获取主角的ID
	* @param 
	*/
	PLAYER_ID GetHeroUserId();
	/**
	* Instruction : 
	* @param 
	*/
	void ResetValue();
protected:	

private:	
	//unsigned int m_nEquipElfId;
	////std::map<unsigned int,ElfAttachData> m_mapElfIdAndValue;
	//std::vector<ElfAttachData> m_vecElfData;

	bool m_bHeroHasSendMessage;
};