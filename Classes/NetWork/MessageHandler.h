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
#ifndef MESSAGE_HANDER_H
#define MESSAGE_HANDER_H


#include <map>
#include <string>

using namespace std;


#include "message.h"

#include "BaseMsgHandler.h"

class BaseMsgHandler;
//------------------------------------------------------------------------
//				message handler
//------------------------------------------------------------------------

class MessageHandler
{
	friend class PPVEMessage;
    friend class PVPMessage;

public:

	static MessageHandler*			sShareInstance();
	static void Destroy();


	//------------------------------------------------------------------------
	//				handle message
	//------------------------------------------------------------------------
	void							handleMessage(NORMAL_MSG_PACKAGE* package);
	//this func call lua to process the msg . return false means lua process fail
	bool 							handleMessage(WILD_MSG_PACKAGE* package);

	//------------------------------------------------------------------------
	//				only valid at cur frame
	//------------------------------------------------------------------------
	WILD_MSG_PACKAGE*				getWildMsgPackage()			{return mCurWildMsgPackage;}

	bool hasHandlerForMessage(std::string cmd);

protected:

	MessageHandler();
	~MessageHandler();

	//------------------------------------------------------------------------
	//				deal with message
	//------------------------------------------------------------------------
	void							registerMessageHandlerFunction();
	/// register external message handler
	void  registerExternalMsgHandler();

	//
	typedef void					(MessageHandler::*handlerFunc)();

	//------------------------------------------------------------------------
	//				handler function
	//------------------------------------------------------------------------
	//
	void							postHandleMsg();
    void                            handleNotifySysFlagRsp();
	//------------------------------------------------------------------------
	//				handler all message error
	//------------------------------------------------------------------------	
	void							handleMsgError();

	//
	void							handleMsgPlayerEnterMap();
	//
	void							handleMsgPlayerLeaveMap();
	//
	void							handleMsgPlayerWalk();
	//
	void							handleMsgGetOtherPlayers();
	//
	void							handleMsgBattleCreate();

	//怪物相关
	void							handleMsgMonsterMove();	

	void							handleMsgMonsterAttack();

	void							handleMsgMonsterDie();

	void							handleMsgMonsterBuff();

	void							handleMsgInstanceList();

	// login related
	void							handleCheckSession();
	void							handleCheckToken();
	void							handGetToken();
	void							handleMsgLoginIn();
	void							handleMsgRoleList();
	void							handleAckMsgCheckSession();
	void							handleAckMsgCreateRole();
	void							handleQuerySharedRsp();
	void							handleUserFreezeRsp();

	void							handleMsgMonsterHit();
	void							handleMsgPlayerAttackMonster();
	void							handleLeaveBattle();

	// Item related
	void							handleBackPackItems();
	void							handleAddItem();
	void							handleBackPackMove();
	void							handleBackPackItemRemove();
	void							handleBackPackItemUpdate();
	void							handleEquipnetList();
	void							handleEquipmentStrength();
	void							handleEquipCompose();
	void							handleUserItem();
	void							handleMsgNewEquipNoteRsp();

	// 宝石镶嵌
	void							handleEquipInlayGemRsp();
	void							handleGemRecastRsp();

	// diamond
	void							handleExchangeParameterRsp();

	// attributes
	void							handleChipStatusRsp();
	void							handleAttributesRsp();
	void							handleLevelUpRsp();
	void							handleUserInfoRsp();
	void							handleMailNumRsp();

	void							handleSearchSpriteStoneRsp();
	void							handleSpriteStoneMoveStorageRsp();
	void							handleSpriteUpgradeRsp();
	void							handleSpriteTransToEnergy();
	void							handleSpriteExchangeRsp();

	// month card

	void							handleMcardInfoRsp();
	void							handleMcardRewardRsp();
	////////////////////////////////////////////////////////////////////////
	//add more later : battle server message

    
	//  These function to handle response message of PVE request from client
	void		handleMsgBattlePVE();

	// These function to handle echo message received from battle server
	void        handleMsgEcho();

	/// <summary>
	//	Task related
	/// </summary>
	// Note: 请求任务数据 回复
	void							handleRsqTaskInfoMessage();
	// Note: 请求任务Log 回复
	void							handleRsqTaskLogMessage();
	// Note: 设置任务步骤 回复
	void							handleRsqTaskStepMessage();

	void							handleRsqNetStateMessage();

	void							handleRsqTaskStatusMessage();

	/// <summary>
	//	精灵相关
	/// </summary>
	/**
	* Instruction : 获取精灵信息回包
	* @param 
	*/	
	void							handleRsqElfListMessage();
	/**
	* Instruction : 获取设置精灵状态回包
	* @param 
	*/
	void							handleRsqElfStateMessage();

	//多人副本
	void							handleMsgUserJoin();
	void							handleMsgUserLeave();
	void							handleMsgBattleMove();
	void							handlePlayerAttack();
	void							handleSkillBegin();

	// PVP
	void							handlePVPPlayerHitPlayer();

	void							handleOnceRspMessage();

	void							handleMsgLoadComplete();

	void							handleMsgBattleTimeout();
	void							handleMsgPlayerDie();
	void							handleMsgPlayerHP();

	//Skill UI
	void							handleMsgGetUserSkillRsp();
	void							handleMsgUserSkillDiffRsp();
	void							handleMsgAddUserSkillRsp();

	void							handleMsgBattleUserAddBloodRsp();
	// Note: 
	void							handleMsgTrainRsp();
	void							handleMsgTrainDetermineRsp();

	// Note: 精灵探索系统
	void							handleExploreRsp();
	void							handleExploreDetermineRsp();

	//防守
	void							handleMsgBattleMonsterCreateRsp();
	
	void							handleNotifyMoneyConfigInfo();

	// pvai
	void							handleMsgPvAIInfoRsq();

	void							handleMsgMonsterEffectRsq();
	//jing ying fu ben
	//void handleOnceDailyRsp();

	// 战斗中，释放buffer技能时，攻击力等会变动，需要从服务器取得最新的值。
	void							handleBasicUserBattleInfoRsp();
    
    void                            handlePVXRewardRsp();
    void                            handlePVP2RewardRsp();

	void							handleDailyTaskInfoRsq();
	void							handleDailyTaskAcceptRsq();
	void							handleDailyTaskGiveUpRsq();
	void							handleDailyTaskNotifyUpdateTaskInfoRsq();
	void							handleDailyTaskGetRewardRsq();

	void							handleBattlePlayerRevivalRsp();
	void handleCSNotifyLevelConfInfoRsp();

	//pve local server
	void							handleMsgBattlePrepareRsq();

	void							handleMsgSkillNeedUpdateRsq();

	void							handleMsgNickNameRsp();
    
    void                            handleMsgLoginAnnouncementRsp();
        
    void                            handleMsgAnnouncementPreRsp();
//battle 
	void							handleMsgPVPLearnPrepareRsp();
	void							handleMsgPVPLearnRsp();
	void                            handleMsgAckLearnRefuseRsp();
	void                            handleMsgLearnMandatoryRsp();

	// Note: 玩家红名协议
	void							HandleMsgCSGetRednameTimeRsp();

    // 精灵抽取协议
    void                            HandleMsgSpriteDrawRsp();

    void                            HandleMsgPlayerRank();
    void                            HandleMsgRankInfoRsp();
    void                            HandleStarReward();
    void                            HandleMsgTreasureInfoRsp();
    void                            HandleMsgTreasureRewardGetRsp();
    void                            HandleMsgTresureItems();

    // 首冲
    void                            HandleMsgGetTopupAwardRsp();

    // 商城
    void                            HandleMsgGetShoppingRsp();

    // 获取服务器的商品价格
    void                            HandleExchangeParameterListRsp();

	void							HandleTargetFinishedRsp();
    void                            HandleGuildListRsp();
    void                            HandleGuildSearchRsp();
    void                            HandleGuildCreateRsp();
    void                            HandleGuildJoinRsp();
    void                            HandleGetGuildInfoRsp();
    void                            HandleGetGuildMemberRsp();
    void                            HandleSetGuildNoticeRsp();
    void                            HandleChangeGuildMemberStatusRsp();
    void                            HandleGuildShopItems();
    void                            HandleGuildBless();
    void                            HandleGuildContributeInfo();
    void                            HandleGuildContribute();
    void                            HandleGuildQuild();
    void                            HandleGuildDrop();
    void                            HandleGuildTips();
	void							HandleGetGuildRecordInfo();
	void							HandleGetGuildUnionInfo();
	void							HandleGetGuildChooseInfo();
	void							HandleGetGuildRedEnvoInfo();
	void							HandleGuildFirstSendRedEnvoRsp();
	void							HandleGuildGetRedEnvoRsp();
	void							HandleGuildGetRedEnvoRecordRsp();
	void							HandleGuildGetRedEnvoConfigRsp();
	void							HandleGuildSecondSendRedEnvoRsp();
private :
    void TryUpdateSpriteExtractLayer();

protected:

	static MessageHandler*			sInstance;

	WILD_MSG_PACKAGE*				mCurWildMsgPackage;
	NORMAL_MSG_PACKAGE*				mCurNormMsgPackage;

	typedef std::map<std::string, handlerFunc> HANDLER_FUNCTION_LIST;
	HANDLER_FUNCTION_LIST			mCmd2HandlerFunctionList;

	/// external handlers, handle extra message which defined in other source code file
	map<std::string, BaseMsgHandler*> mExternalHandler;
};



#endif
