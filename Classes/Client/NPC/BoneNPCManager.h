#pragma once

#include "NpcSprite.h"
#include "NPCDataDefine.h"
#include "map"

class BoneNPCManager
{
public:	
	~BoneNPCManager();
	static BoneNPCManager* getInstance(void);
	static void Destroy();
	void Init();
	std::map<int,NPCData *> * LoadNPCConfigInfoFromLua(int level);
	void LoadOneLevelNPC(int levelID,CCNode *parentNode);
	/**
	* Instruction : 更新某一个NPC头上标示符
	* @param 
	*/
	void UpdateOneNpcFlagIcon(unsigned int npcId);
	/**
	* Instruction : 更新所有NPC标示符
	* @param 
	*/
	void UpdateAllNpcFlagIcon();
	/**
	* Instruction : 获取NPC的
	* @param 
	*/
	NPCData * GetOneNpcBasicData(unsigned int npc_id,bool bFromLua = false);
	/**
	* Instruction : 获取某一个NPC
	* @param 
	*/
	SpriteNPC* GetOneNpc(unsigned int npc_id);
	/**
	* Instruction : 获取Npc相关的角色
	* @param 
	*/
	std::map<int,SpriteNPC *> * GetNpcTypeIdAndSprites(){
		return mMapNpcTypeIdAndSprites;	
	}

	void ResetValue();

	void ShowAllNpc(bool bShow);
private:
	BoneNPCManager();
	std::map<int,SpriteNPC *> *mMapNpcTypeIdAndSprites;
};