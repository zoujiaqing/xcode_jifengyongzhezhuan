#pragma once

#include "cocos2d.h"
#include "SpriteElf.h"

USING_NS_CC;

class SpriteFactory
{
public :
	SpriteFactory();
	void PrintLog();

	static SpriteFactory* sharedFactory();
	static void Destroy();
	int GetValue();

	/**
	* Instruction : 在面板上显示精灵动画
	* @param 
	*/
	void ShowElfOnLayerAtPoint(CCNode* parentNode,int type,CCPoint pos);
	/**
	* Instruction : 向Lua中传入精灵ID
	* @param 
	*/
	//void PushElfIdToLuaTable();
	/**
	* Instruction : 获取精灵状态
	* @param 
	*/
	//int GetOneElfState(int elfId);
	/**
	* Instruction : 重置变量
	* @param 
	*/
	void ResetValue();
	/**
	* Instruction : 
	* @param 
	*/
	void RemoveElfFromParent(CCNode *parentNode);
	/**
	* Instruction : 从主角身上移除精灵
	* @param 
	*/
	void RemoveElfFromHero(PLAYER_ID roleId,bool bDeleteElf = true);

    void  RemovePosElfFromHero(PLAYER_ID s_id) ;
	/**
	* Instruction : 绑定精灵岛主角身上
	* @param 
	*/
	void AttachElfToOneHero(unsigned int elfTypeId, PLAYER_ID heroId, unsigned int grade = 0);
	/**
	* Instruction : 
	* @param 
	*/
	CCNode* GetMainMenuLayerNode();
	/**
	* Instruction : 
	* @param 
	*/
	bool ShowElfPanelWithBackPackLayer();

	// Note: 精灵唤醒相关协议
	void SendElfAwakeMessage(unsigned int awakeType);
	void SendAwakeConfigMessage();
private:
	static SpriteFactory* mFactory;

	SpriteElf* mAttachElf;
};
