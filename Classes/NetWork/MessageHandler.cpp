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
#include "PPVEMessage.h"
#include "PVPMessage.h"
#include "MessageHandler.h"
#include "BoneNPCManager.h"
#include "LevelDefine.h"
#include "UserData.h"
#include "TaskConfigFromLuaManager.h"
#include "GameConfigFromLuaManager.h"
//#include "iAd.cs.pb.h"

#include "cocos2d.h"
#include "tolua++.h"
#include "CCLuaEngine.h"
#include "SceneLayer.h"
#include "GameManager.h"
#include "GameResourceManager.h"
#include "cs_basic.pb.h"
#include "cs_core.pb.h"
#include "cs_battle.pb.h"
#include "errcode.pb.h"
#include "XLogicManager.h"
#include "LevelManager.h"
#include "SpriteMonsterMgr.h"
#include "MonsterData.h"
#include "SpriteSeer.h"
#include "ParticleManager.h"
#include "InstanceListLayer.h"
#include "EquipmentUpgradeUILayer.h"
#include "MainMenuLayer.h"
#include "AlchemyUILayer.h"
#include "ItemManager.h"
#include "InstanceManager.h"
#include <time.h>
#include "TaskManager.h"
#include "NetStateChecker.h"
#include "Localization.h"
#include "SpriteElfDataCenter.h"
#include "OnlineNetworkManager.h"
#include "SpriteElfManager.h"
#include "HelloWorldScene.h"
#include "SpriteElfConfigFromLua.h"
#include "FrontCoverLayer.h"
#include "MainLandManager.h"
#include "GMessage.h"
#include "NotificationCenter.h"
#include "StoryDataCenter.h"
#include "StoryInstanceDirector.h"
#include "TimeManager.h"
#include "BattleUILayer.h"
#include "MessageBoxLayer.h"
#include "SkillDispMgr.h"
#include "GameDataManager.h"
#include "GameAudioManager.h"
#include "EquipMixLayer.h"
#include "GemStonesFromLua.h"
#include "PvAIManager.h"
#include "GuildInstanceManager.h"
#include "LuaTinkerManager.h"
#include "TutorialsManager.h"
#include "BackPackLayer.h"
#include "MainMenuLayer.h"
#include "ShopUILayer.h"
#include "TalentLayer.h"
#include "AutoOnHook.h"
#include "PopupRewardManager.h"
#include "UIControlsApearControler.h"
#include "CardUILayer.h"
#include "AccountsLayer.h"
#include "SpriteExtractResult.h"
#include "SpriteExtractLayer.h"
#include "FirstChargeLayer.h"
//#include "Notice1UILayer.h"
#include "LuaTinkerManager.h"
#include "MessageFilter.h"
#include "detect.hpp"
#include "cs_config.pb.h"
#include "GameUILayer.h"
#include "WaitingLayerManager.h"
#include "SystemConditionManager.h"
#include "GuildManager.h"
#include "NickNameManager.h"
#include "SkeletonAnimRcsManager.h"
#include "PKLayer.h"
#include "InstanceListLayer.h"
#include "TimeManager.h"
#include "UserData.h"
#include "RankManager.h"
#include "SpriteFactory.h"
#include "MarketLayer.h"
#include "MysteryLayer.h"
#include "MarketManager.h"
#include "TreasureExploreManager.h"
#include "MonthCardManager.h"
#include "CallNativeFuncManager.h"
#include "GuildInfoLayer.h"
#include "GuildInstanceManager.h"
using namespace cocos2d;

//using namespace iAdCSProto;

MessageHandler* MessageHandler::sInstance = NULL;

static  bool get1002value=false;//this can indicate we alreay get the 1002 value

MessageHandler::MessageHandler()
	:mCurWildMsgPackage(NULL)
	,mCurNormMsgPackage(NULL)
{
	/// init external handler
	PPVEMessage::Get()->InitHandler(this);
    PVPMessage::Get()->InitHandler(this);

	registerMessageHandlerFunction();
}


MessageHandler::~MessageHandler()
{
	mCmd2HandlerFunctionList.clear();
	
}


MessageHandler* MessageHandler::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new MessageHandler();
	}

	return sInstance;
}

void MessageHandler::Destroy()
{
	if (sInstance)
	{
		delete sInstance;
		sInstance = NULL;
	}
}

void MessageHandler::handleMessage(NORMAL_MSG_PACKAGE* package)
{
	mCurNormMsgPackage = package;
	std::string cmd = mCurNormMsgPackage->header.cmd;
    
    //CCLOG("MessageHandler::handleMessage cmd:%s\n", cmd.c_str());
    
	HANDLER_FUNCTION_LIST::iterator itor = mCmd2HandlerFunctionList.find(cmd);
	if (itor != mCmd2HandlerFunctionList.end())
	{
		handlerFunc func = itor->second;
		(this->*func)();
	}
	else
	{
		///find external message handler to handle the message
		map<std::string, BaseMsgHandler*>::iterator it = mExternalHandler.find(cmd);
		if (it != mExternalHandler.end())
		{
			BaseMsgHandler* pHandler = it->second;
			pHandler->HandlerMessage(cmd, mCurNormMsgPackage);
		}
	}
	postHandleMsg();
}

bool MessageHandler::hasHandlerForMessage(std::string cmd)
{
	HANDLER_FUNCTION_LIST::iterator itor = mCmd2HandlerFunctionList.find(cmd);
	if (itor != mCmd2HandlerFunctionList.end())
	{
		return true;
	}
	else
	{
		///find external message handler to handle the message
		map<std::string, BaseMsgHandler*>::iterator it = mExternalHandler.find(cmd);
		if (it != mExternalHandler.end())
		{
			return true;
		}
	}
	CCLOG("C++ didn't have haddler for msg %s", cmd.c_str());
	return false;

}

bool  MessageHandler::handleMessage(WILD_MSG_PACKAGE* package)
{	
	mCurWildMsgPackage = package;
#if 0
	std::string luaScriptName = "Script/handleMessage.lua";
	CCLuaEngine* luaEngine = (CCLuaEngine*)CCScriptEngineManager::sharedManager()->getScriptEngine();
	if (luaEngine)
	{
		lua_State *lua_S = luaEngine->getLuaState();		
		tolua_pushusertype(lua_S,(void*)this,"MessageHandler");
	}

	//execute the script
	const char *path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(luaScriptName.c_str());
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeScriptFile(path);
#endif
     bool ret = LuaTinkerManager::Get()->callLuaFunc<bool>(
    "Script/handleMessage.lua",
    "process_message");
    CCLOG("%s ret %d", __FUNCTION__, ret);

	mCurWildMsgPackage = NULL;
    return ret;
}

////////////////////////////
///  register external message handler
void MessageHandler::registerExternalMsgHandler()
{
    ///////////////PPVE Rsp///////////////////
    
	/// register PPve message handler class
	mExternalHandler["CSPVEQueryRoomRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVEBaseInfoRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVEExitRoomRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVECloseRoomRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVEKickoffRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVEStartRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVEJoinRoomFailureRsp"] = PPVEMessage::Get();
	mExternalHandler["CSPVEJoinRoomRsp"] = PPVEMessage::Get();
    
    ///////////////PVP Rsp///////////////////
    mExternalHandler["CSPVPQueryRoomRsp"] = PVPMessage::Get();
    mExternalHandler["CSPVPBaseInfoRsp"] = PVPMessage::Get();
    mExternalHandler["CSPVPJoinRoomRsp"] = PVPMessage::Get();
    mExternalHandler["CSPVPExitRoomRsp"] = PVPMessage::Get();
    mExternalHandler["CSPVPCloseRoomRsp"] = PVPMessage::Get();
    mExternalHandler["CSPVPKickoffRsp"] = PVPMessage::Get();
    mExternalHandler["CSPVPStartRsp"] = PVPMessage::Get();
    
    ////////////Talent Rsp////////////////////
    mExternalHandler["CSQueryDowerRsp"] = PVPMessage::Get();
    mExternalHandler["CSDowerInfoRsp"] = PVPMessage::Get();
    mExternalHandler["CSDowerRsp"] = PVPMessage::Get();
    mExternalHandler["CSAccelerateDowerRsp"] = PVPMessage::Get();
    
    ///////////OnHook Rsp/////////////////////
    mExternalHandler["CSABMonsterRsp"] = PVPMessage::Get();
    mExternalHandler["CSAutoBattleRsp"] = PVPMessage::Get();
    mExternalHandler["CSABRewardRsp"] = PVPMessage::Get();
    mExternalHandler["CSExchangeParameterRsp"] = PVPMessage::Get();
    mExternalHandler["CSCancelAutoBattleRsp"] = PVPMessage::Get();
    mExternalHandler["CSAttributesRsp"] = PVPMessage::Get();
    
    //////////ShopUI Rsp/////////////////////////////
    mExternalHandler["CSGetShopItemsRsp"] = PVPMessage::Get();
    mExternalHandler["CSBuyShopItemRsp"] = PVPMessage::Get();
    mExternalHandler["CSRepoRecycleBinItemRsp"] = PVPMessage::Get();
    mExternalHandler["CSItemMoveToRecycleBinRsp"] = PVPMessage::Get();
    
    mExternalHandler["CSPlayerDieShowRsp"] = PVPMessage::Get();
    mExternalHandler["CSNewMailRsp"] = PVPMessage::Get();
}

//----------------------------------------------------------------------
//		register all message hander functions here
//----------------------------------------------------------------------
void MessageHandler::registerMessageHandlerFunction()
{
    
    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSNotifyCliSysFlagRsp",
                                                                        &MessageHandler::handleNotifySysFlagRsp));

    
	//error msg
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("ErrorCode", 
		&MessageHandler::handleMsgError));
    
	//login msg
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSLoginRsp",
		&MessageHandler::handleMsgLoginIn));
    
    
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetRoleListRsp", 
		&MessageHandler::handleMsgRoleList));
	//player enter map
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSEnterMapRsp",
		&MessageHandler::handleMsgPlayerEnterMap));
	////player leave map
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSLeaveMapRsp",
		&MessageHandler::handleMsgPlayerLeaveMap));
	////player walk
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSWalkRsp",
		&MessageHandler::handleMsgPlayerWalk));
	////get other players
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetMapUsersRsp",
		&MessageHandler::handleMsgGetOtherPlayers));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleCreateRsp",
		&MessageHandler::handleMsgBattleCreate));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonsterMoveRsp",
		&MessageHandler::handleMsgMonsterMove));
    
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonsterAttackRsp",
		&MessageHandler::handleMsgMonsterAttack));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonsterHitRsp",
		&MessageHandler::handleMsgMonsterHit));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonsterDieRsp",
		&MessageHandler::handleMsgMonsterDie));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonsterBloodRsp",
		&MessageHandler::handleMsgPlayerAttackMonster));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBuffEffectRsp",
		&MessageHandler::handleMsgMonsterBuff));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleLeaveRsp",
		&MessageHandler::handleLeaveBattle));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSInstanceRsp",
		&MessageHandler::handleMsgInstanceList));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSEquipNotify",
		&MessageHandler::handleMsgNewEquipNoteRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSAllItemsRsp",
		&MessageHandler::handleBackPackItems));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSItemMoveRsp",
		&MessageHandler::handleBackPackMove));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSItemRemoveRsp",
		&MessageHandler::handleBackPackItemRemove));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSItemUpdateRsp",
		&MessageHandler::handleBackPackItemUpdate));	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSItemAddRsp",
		&MessageHandler::handleAddItem));	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSItemUseRsp",
		&MessageHandler::handleUserItem));	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSFreezeTimeRsp",
		&MessageHandler::handleUserFreezeRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetChipComposeStatusRsp",
		&MessageHandler::handleChipStatusRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSEquipmentComposeRsp",
		&MessageHandler::handleEquipCompose));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSearchSpiritStoneRsp",
		&MessageHandler::handleSearchSpriteStoneRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSpiritStoneMoveBagRsp",
		&MessageHandler::handleSpriteStoneMoveStorageRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSpiritStoneLevelUpRsp",
		&MessageHandler::handleSpriteUpgradeRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSpiritStoneToSPRsp",
		&MessageHandler::handleSpriteTransToEnergy));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetExchangeSSInfoRsp",
		&MessageHandler::handleSpriteExchangeRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBasicUserBattleInfoRsp",
		&MessageHandler::handleBasicUserBattleInfoRsp));

	//��ʯ��Ƕ
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSEquipInlayGemRsp",
		&MessageHandler::handleEquipInlayGemRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSResetGemAttrRsp",
		&MessageHandler::handleGemRecastRsp));
	
	//diamond
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSExchangeParameterRsp",
		&MessageHandler::handleExchangeParameterRsp));
	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSAttributesRsp",
		&MessageHandler::handleAttributesRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetUserInfoRsp",
		&MessageHandler::handleUserInfoRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSLevelUpRsp",
		&MessageHandler::handleLevelUpRsp));
	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSCreateRoleRsp",
		&MessageHandler::handleAckMsgCreateRole));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPlayerAttackRsp",
		&MessageHandler::handlePlayerAttack));
    
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSkillBeginRsp",
		&MessageHandler::handleSkillBegin));
    
	// Note: TASK
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTaskInfoRsp",
		&MessageHandler::handleRsqTaskInfoMessage));	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTaskLogRsp",
		&MessageHandler::handleRsqTaskLogMessage));	
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTaskDoStepRsp",
		&MessageHandler::handleRsqTaskStepMessage));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTaskStatusRsp",
		&MessageHandler::handleRsqTaskStatusMessage));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPingRsp",
		&MessageHandler::handleRsqNetStateMessage));		

	// Note: Elf
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonListRsp", 
		&MessageHandler::handleRsqElfListMessage));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSetMonStatusRsp",
		&MessageHandler::handleRsqElfStateMessage));	

	//���˸���
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleUserJoinRsp",
		&MessageHandler::handleMsgUserJoin));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSCheckSessionRsp",
		&MessageHandler::handleCheckSession));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSCheckTokenRsp",
		&MessageHandler::handleCheckToken));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetTokenRsp",
		&MessageHandler::handGetToken));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleUserLeaveRsp",
		&MessageHandler::handleMsgUserLeave));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleMoveRsp",
		&MessageHandler::handleMsgBattleMove));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSQueryShardRsp",
		&MessageHandler::handleQuerySharedRsp));

	// PVP
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPlayerHitPlayerRsp", &MessageHandler::handlePVPPlayerHitPlayer));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSOnceRsp", &MessageHandler::handleOnceRspMessage));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleLoadCompleteRsp",
		&MessageHandler::handleMsgLoadComplete));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleTimeoutRsp",
		&MessageHandler::handleMsgBattleTimeout));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattlePlayerDelRsp",
		&MessageHandler::handleMsgPlayerDie));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPlayerHPRsp",
		&MessageHandler::handleMsgPlayerHP));

	// Skill UI
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetUserSkillRsp",
		&MessageHandler::handleMsgGetUserSkillRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSUserSkillDiffRsp",
		&MessageHandler::handleMsgUserSkillDiffRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSAddUserSkillRsp",
		&MessageHandler::handleMsgAddUserSkillRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleUserAddBloodRsp",
		&MessageHandler::handleMsgBattleUserAddBloodRsp));
	
	// Note: SpriteElf UI
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTrainDetermineRsp",
		&MessageHandler::handleMsgTrainDetermineRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTrainRsp",
		&MessageHandler::handleMsgTrainRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattleMonsterCreateRsp",
		&MessageHandler::handleMsgBattleMonsterCreateRsp));

	// Note: ����ϵͳ
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSExploreRsp",
		&MessageHandler::handleExploreRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSExploreDetermineRsp",
		&MessageHandler::handleExploreDetermineRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSNotifyLevelConfInfoRsp",
		&MessageHandler::handleNotifyMoneyConfigInfo));

	// pvai
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVAIInfoRsq",
		&MessageHandler::handleMsgPvAIInfoRsq));
			
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMonsterEffectRsp",
		&MessageHandler::handleMsgMonsterEffectRsq));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVPLearnRsp",
		&MessageHandler::handleMsgPVPLearnRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVPLearnPrepareRsp",
		&MessageHandler::handleMsgPVPLearnPrepareRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVPAckLearnRefuseRsp",
		&MessageHandler::handleMsgAckLearnRefuseRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVPAckLearnRefuseRsp",
		&MessageHandler::handleMsgLearnMandatoryRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMcardInfoRsp",
		&MessageHandler::handleMcardInfoRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSMcardRewardRsp",
		&MessageHandler::handleMcardRewardRsp));
	//jing ying fu ben 
	//mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSOnceDailyRsp",
	//	&MessageHandler::handleOnceDailyRsp));
	//// handle message echo
	//mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("iAdCSProto.cli_echo_rsp_t",
	//	&MessageHandler::handleMsgEcho));
    
    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVXRewardRsp",
                                                                        &MessageHandler::handlePVXRewardRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPVP2RewardRsp",
                                                                        &MessageHandler::handlePVP2RewardRsp));

	// Note: ÿ������ϵͳ
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSDailyTaskInfoRsp",
		&MessageHandler::handleDailyTaskInfoRsq));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSAcceptDailyTaskRsp",
		&MessageHandler::handleDailyTaskAcceptRsq));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGiveUpDailyTaskRsp",
		&MessageHandler::handleDailyTaskGiveUpRsq));		

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSNotifyUpdateDailyTaskInfoRsp",
		&MessageHandler::handleDailyTaskNotifyUpdateTaskInfoRsq));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetDailyTaskRewardRsp",
		&MessageHandler::handleDailyTaskGetRewardRsq));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattlePlayerRevivalRsp",
		&MessageHandler::handleBattlePlayerRevivalRsp));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSBattlePrepareRsp",
		&MessageHandler::handleMsgBattlePrepareRsq));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSkillUpdateValidRsq",
		&MessageHandler::handleMsgSkillNeedUpdateRsq));	

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSNickNameRsp",
		&MessageHandler::handleMsgNickNameRsp));	
	
    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSLoginAnnouncementRsp",
                                                                        &MessageHandler::handleMsgLoginAnnouncementRsp));
    
    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSAnnouncementPreRsp",
                                                                        &MessageHandler::handleMsgAnnouncementPreRsp));
	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSNotifyMailNumInfoRsp",
		&MessageHandler::handleMailNumRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetRednameTimeRsp",
		&MessageHandler::HandleMsgCSGetRednameTimeRsp));
	
    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSpiritDrawRsp", 
        &MessageHandler::HandleMsgSpriteDrawRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSQueryPlayerRankInfoRsp", 
    &MessageHandler::HandleMsgPlayerRank));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSChkInstanceStarRewardRsp", 
        &MessageHandler::HandleStarReward));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetExploreInfoRsp", 
        &MessageHandler::HandleMsgTreasureInfoRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetExploreTreasureRsp", 
        &MessageHandler::HandleMsgTreasureRewardGetRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSNotifyItemExploreRewardRsp", 
        &MessageHandler::HandleMsgTresureItems));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSQueryRankRsp", 
        &MessageHandler::HandleMsgRankInfoRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetTopupAwardRsp", 
        &MessageHandler::HandleMsgGetTopupAwardRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSShoppingRsp",
        &MessageHandler::HandleMsgGetShoppingRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSExchangeParameterListRsp", 
        &MessageHandler::HandleExchangeParameterListRsp));

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSTargetFinishedRsp", 
        &MessageHandler::HandleTargetFinishedRsp));
	
    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSListGuildRsp", 
        &MessageHandler::HandleGuildListRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSearchGuildRsp",
        &MessageHandler::HandleGuildSearchRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSCreateGuildRsp", 
        &MessageHandler::HandleGuildCreateRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSJoinGuildRsp", 
        &MessageHandler::HandleGuildJoinRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSQueryPlayerGuildInfoRsp", 
        &MessageHandler::HandleGetGuildInfoRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSListPlayerGuildRsp", 
        &MessageHandler::HandleGetGuildMemberRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSSetGuildNoticeRsp", 
        &MessageHandler::HandleSetGuildNoticeRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSChangePlayerGuildStatusRsp", 
        &MessageHandler::HandleChangeGuildMemberStatusRsp));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetShoppingItemsRsp", 
        &MessageHandler::HandleGuildShopItems));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGoddessBlessRsp", 
        &MessageHandler::HandleGuildBless));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSExchangeItemInfoRsp", 
        &MessageHandler::HandleGuildContributeInfo));

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSPGContributeToGuildRsp", 
        &MessageHandler::HandleGuildContribute));   

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSQuitGuildRsp", 
        &MessageHandler::HandleGuildQuild));  

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSDropGuildRsp", 
        &MessageHandler::HandleGuildDrop));  

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildNoteRsp", 
        &MessageHandler::HandleGuildTips));  

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRecordListRsp", 
        &MessageHandler::HandleGetGuildRecordInfo));  

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSUnionInfoRsp", 
		&MessageHandler::HandleGetGuildUnionInfo));  

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGetRecruitMemberRsp", 
		&MessageHandler::HandleGetGuildChooseInfo));  

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRedPacketQueryRsp", 
        &MessageHandler::HandleGetGuildRedEnvoInfo)); 

    mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRedPacketFirstCheckRsp", 
        &MessageHandler::HandleGuildFirstSendRedEnvoRsp)); 

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRedPacketGetAwardRsp", 
        &MessageHandler::HandleGuildGetRedEnvoRsp)); 

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRedPacketRecordRsp", 
        &MessageHandler::HandleGuildGetRedEnvoRecordRsp)); 

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRedPacketConfigRsp", 
        &MessageHandler::HandleGuildGetRedEnvoConfigRsp)); 

	mCmd2HandlerFunctionList.insert(std::pair<std::string, handlerFunc>("CSGuildRedPacketSendRsp", 
        &MessageHandler::HandleGuildSecondSendRedEnvoRsp)); 

	registerExternalMsgHandler();
}

//----------------------------------------------------------------------
void MessageHandler::postHandleMsg()
{
	mCurNormMsgPackage = NULL;
}

void MessageHandler::handleNotifySysFlagRsp()
{
    CSNotifyCliSysFlagRsp* msg = dynamic_cast<CSNotifyCliSysFlagRsp*>(mCurNormMsgPackage->msg);
    int flagsize = msg->sys_flags_size();
    for(int i = 0; i<flagsize; i++)
    {
        CSNotifyCliSysFlagRsp::SysFlag  sys = msg->sys_flags(i);
        if(sys.sys_index() == CSNotifyCliSysFlagRsp_SysIndex_DOWER_SYSTEM)
        {
            bool flag = sys.flag();
			//TimeManager::Get()->settalentLevelupFlag(flag);
			//TalentLayer::m_TalentLevelupFlag = flag;
   //         UILayout* layout = UIManager::sharedManager()->getUILayout("MainMenu");
   //         if(layout)
   //         {
   //             UIButton* button = layout->FindChildObjectByName<UIButton>("button_talent");
   //             if(button)
   //             {
   //                 if(flag)
   //                     button->showTipIcon();
   //                 else
   //                     button->hideTipIcon();
   //             }
   //         }
        }
    }
}

//----------------------------------------------------------------------
//		all message hander functions here
//----------------------------------------------------------------------
void MessageHandler::handleMsgError()
{
	int tag1 = MainMenuLayer::getLayerTagByName("ForcePKAnimLayer");
	MainMenuLayer::hideUILayerByTag(tag1);
	db_errcode_t errorId = (db_errcode_t)mCurNormMsgPackage->header.ret;   
    //ShowMessage(str);
    switch(errorId)
	{
    case olerr_not_enough_level:
	case olerr_not_enough_treasure_box:
	case olerr_not_enough_key:
    {
		// not need error text
        break;
    }
    default:
		char * str = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/errcode.lua", "getErrStr", errorId);
        ShowMessage(str);
        break;
    }
	// TODO : 
	//if(errorId == dberr_role_notfound)
	{
		XLogicManager::sharedManager()->logInError(errorId);
	}
    #if 0
	CCLOG("handleMsgError: %d",errorId);
	{
		std::string content = "";
		switch(errorId)
		{
		case olerr_enter_manor_times_max:			
			content = Localizatioin::getLocalization("S_EnterMonorTimesMax_Content");
			break;
		case olerr_warm_sport_lv_max:
			content = Localizatioin::getLocalization("S_WarmSportLvMax_Content");
			break;
		case olerr_not_enough_diamond:
			content = Localizatioin::getLocalization("S_Gh_Diamond_Content");
            break;
		case olerr_not_enough_coin:
			content = Localizatioin::getLocalization("S_Not_Enough_Coin_Content");
			break;
		case olerr_not_enough_exploit:
			content = Localizatioin::getLocalization("S_Not_Enough_Exploit_Content");
			break;
		case olerr_not_enough_level:
			content = Localizatioin::getLocalization("S_Not_Enough_Level_Content");
			break;
		case olerr_not_enough_pvai_times:
			content = Localizatioin::getLocalization("M_PVAI_NOMORETIME");
			break;
		case olerr_not_clear_pvai_cd:
			content = Localizatioin::getLocalization("M_PVAI_INCD");
			break;
		case olerr_has_same_attr_in_item:
			content = Localizatioin::getLocalization("M_HAVESAME");
			break;
		case dberr_role_notfound:
		case dberr_role_exists:
		case dberr_crtrole_time_limit:
		case dberr_duplicated_nickname:
		case olerr_can_transfer_next:
			XLogicManager::sharedManager()->logInError(errorId);
			break;
		case olerr_nofound_explore_id:
			break;
		case olerr_not_enough_explore_times:
			break;
        case olerr_enter_spec_map_num_max:
            content = Localizatioin::getLocalization("M_ELITEINSTANCE_FINISH_THIS_INSTANCE");
            break;
        case olerr_instance_not_enough_star:
            content = "do not have enough star";
            break;
		case olerr_ppve_times_limit:
			content = Localizatioin::getLocalization("M_CHALLENGETIMESOUT");
			break;
		case olerr_daily_task_accept:	// ÿ�������Ѿ�����
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_1");
				break;
			}
		case olerr_daily_task_not_accept:	// û�н���ÿ������
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_2");
				break;
			}
		case olerr_accept_daily_task_timeout:	// ���ܵ�ÿ�������Ѿ�ʧ??
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_3");
				break;
			}
		case olerr_accept_daily_task_max_star:	// ���ܵ�ÿ����������������
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_4");
				break;
			}
		case olerr_finish_daily_task_times_limit:	// ÿ��������ɴﵽ����?
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_5");
				break;
			}
		case olerr_daily_task_id_invalid:	// ÿ������id��Ч
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_6");
				break;
			}
		case olerr_daily_task_not_finish:	// ÿ������δ��??
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_7");
				break;
			}
		case olerr_daily_task_finish:	// ÿ�������Ѿ����?
			{
				content = Localizatioin::getLocalization("Dailytask_Warnning_8");
				break;
			}
		case olerr_not_enough_pos:
			{
				content = Localizatioin::getLocalization("M_EQUIPMENT_SPACESHORTAGE");
				break;
			}
		default:
            {
                char buff[128];
                snprintf(buff, 128, "error: %d", errorId);
                content = string(buff);
			    break;
            }
		}

		if (content != "")
		{
			ShowMessage(content.c_str());
		}
	}
    #endif

}

void MessageHandler::handleMailNumRsp()
{
	CSNotifyMailNumInfoRsp* msg = dynamic_cast<CSNotifyMailNumInfoRsp*>(mCurNormMsgPackage->msg);
	USERINFO mUserData = UserData::GetUserInfo();
	mUserData.m_newMailNum =  msg->new_mail_num();
	mUserData.m_totalMailNum = msg->total_mail_num();
	mUserData.m_attachMailNum = msg->has_attach_num();
	UserData::SetUserInfo(mUserData);
	if(GameManager::Get()->GetSceneLayer() && GameManager::Get()->GetSceneLayer()->getMainMenuLayer())
	{
		GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->resetMailUIMsg();
	}
}

void MessageHandler::handleChipStatusRsp()
{
	CSGetChipComposeStatusRsp* msg = dynamic_cast<CSGetChipComposeStatusRsp*>(mCurNormMsgPackage->msg);
	USERINFO mUserData = UserData::GetUserInfo();
	if(msg->opt_target() == ITEM_TARGET_EQUIP_CHIP)
	{
		if(mUserData.b_hasEnoughEquipFrag != msg->can_compose())
		{
			mUserData.b_hasEnoughEquipFrag = msg->can_compose();
			UserData::SetUserInfo(mUserData);
			NotificationCenter::defaultCenter()->broadcast(GM_ATTR_FRAG_CAN_CONPOSE,this);
		}
	}
	else if(msg->opt_target() == ITEM_TARGET_MONSTER_CHIP)
	{
		if(mUserData.b_hasEnouchFairyFlag != msg->can_compose())
		{
			mUserData.b_hasEnouchFairyFlag = msg->can_compose();
			UserData::SetUserInfo(mUserData);
			NotificationCenter::defaultCenter()->broadcast(GM_ATTR_FARIY_FLAG_ENOUGH,this);
		}
	}
}

//----------------------------------------------------------------------
void MessageHandler::handleMsgBattleCreate()
{
    CCLOG("handleMsgBattleCreate callback!\n");
	XLogicManager::sharedManager()->setMessageStatus(XLogicManager::E_BATTLE_CREATE);
	//����PVAI Layer
	//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/MainMenuUILayer.lua", "HideOneLayerByTag", 10001);
	int tag = MainMenuLayer::getLayerTagByName("ForcePKAnimLayer");
    MainMenuLayer::hideUILayerByTag(tag);

	//������еĹ�������?
	SpriteMonsterMgr::Get()->resetData();
    
	CSBattleCreateRsp* msg = dynamic_cast<CSBattleCreateRsp*>(mCurNormMsgPackage->msg);	
	
	MonsterData::GetMonsterInfo()->reset();

	 UserData::Get()->clearOtherUserInfo();
	EntityInfo::Get()->Clear();

	int monsterCount = msg->monsterinfo_size();
	for (int i = 0; i < monsterCount; ++i)
	{
		MONSTERINFO* pMonsterInfo = new MONSTERINFO();
		pMonsterInfo->Set(msg->monsterinfo(i));

		//CCAssert(pMonsterInfo->id != UserData::getUserId(),"monster = userID");
		EntityInfo::Get()->Set(pMonsterInfo);
		MonsterData::GetMonsterInfo()->addMonser(msg->monsterinfo(i));
	}

	int userCount = msg->userinfo_size();
	InstanceManager::Get()->setPlayerCount(userCount);

	USERINFO mUserData = UserData::GetUserInfo();
	for(int i=0;i<userCount;i++)
	{
		BasicUserInfo info = msg->userinfo(i);
		PLAYER_ID playerId;
		playerId.setPlayId(info.uid(),info.reg_time(),info.channel_id());
		LevelManager::sShareInstance()->addOnePlayerTypeToBattle(info.type());
		if(playerId == mUserData.id)
		{
			mUserData.Set(info);
			unsigned int cell_x = info.xpos();
			unsigned int cell_y = info.ypos();

			CCAssert(!(cell_x == 0 && cell_y == 0), "Player pos(0, 0)");

			CCPoint point = LevelManager::sShareInstance()->pointCellToPixel(ccp(cell_x,cell_y));
			mUserData.xPos = point.x;
			mUserData.yPos = point.y;
			UserData::SetUserInfo(mUserData);
			InstanceManager::Get()->setPlayerPosition(point);

			if (LevelManager::sShareInstance()->isPVAILevel(msg->battle_id()))
			{
				USERINFO* pUserInfo = UserData::Get()->GetUserInfo(playerId);
				pUserInfo->Set(info);
				UserData::Get()->SetUserInfo(pUserInfo->id, *pUserInfo);
			}

			//break;
		}
		//else
		//{
		//	unsigned int side = 0;
		//	if(info.has_battle_side())
		//		side = info.battle_side();
		//}
	}

	unsigned int mapId = msg->map_id();
	unsigned int battleId = msg->battle_id();
	InstanceManager::Get()->setCurInstanceId(battleId);
	LevelManager::sShareInstance()->SetBattleLevel(battleId);
	if (LevelManager::sShareInstance()->isHeroInstanceLevel(battleId))
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/HeroInstanceListLayer.lua", "SetNowHeroInstanceId", battleId);
	}

	int portalX = msg->portal().xpos();
	int portalY = msg->portal().ypos();
	CCPoint portalPos =  LevelManager::sShareInstance()->pointCellToPixel(ccp(portalX,portalY));
	InstanceManager::Get()->setPortalPosition(portalPos);

	unsigned int portalId = msg->portal_map_id();
	InstanceManager::Get()->setPortalId(portalId);	

	if (msg->has_timeout())
	{
		int instanceTime = msg->timeout();
		InstanceManager::Get()->setInstanceTime(instanceTime);
	}

	int birthCount = msg->monsterbirth_size();
	vector<CCPoint> points;
	for (int i = 0; i < msg->monsterbirth_size(); i++)
	{
		CCPoint monsterBornPoint = LevelManager::sShareInstance()->pointCellToPixel(ccp(msg->monsterbirth(i).xpos(), msg->monsterbirth(i).ypos()));
		points.push_back(monsterBornPoint);
	}
	InstanceManager::Get()->setMonsterBornPoints(points);
	
	XLogicManager::FB_DATA fbData;
	fbData.mapID = mapId;
	fbData.battleID = battleId;
    //USERINFO mUserData = UserData::GetUserInfo();

    XLogicManager::sharedManager()->goToLayer(XLogicManager::E_ENTER_FB, &fbData);  

	//拉取自动战斗信息
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "InitAutoFightInfo");
}

void MessageHandler::handleMsgBattleMonsterCreateRsp()
{
	//TIMEUSPEND.time_begin();
	CSBattleMonsterCreateRsp* msg = dynamic_cast<CSBattleMonsterCreateRsp*>(mCurNormMsgPackage->msg);	
	
	int monsterCount = msg->monsterinfo_size();
	for (int i = 0; i < monsterCount; ++i)
	{		
		MonsterInfo info = msg->monsterinfo(i);
		MonsterInitInfo mInitInfo;
		mInitInfo.uid = info.id();
		mInitInfo.typeId = info.type_id();
		CSPoint position = info.point();
		mInitInfo.totalHp = info.blood();
		mInitInfo.speed = info.speed();
		mInitInfo.isBoss = info.boss();
		mInitInfo.pos = LevelManager::sShareInstance()->pointCellToPixel(ccp(position.xpos(), position.ypos()));
		mInitInfo.monsterName = "monster";
		mInitInfo.isElite = info.elite();
		mInitInfo.hp_line = info.hp_line();
		mInitInfo.m_level = info.level();
		mInitInfo.isAlliance = info.city();

		//mInitInfo.useCache = true;
		mInitInfo.useCache = false;

		MONSTERINFO* pMonsterInfo = new MONSTERINFO();
		pMonsterInfo->Set(msg->monsterinfo(i));
		CCAssert(pMonsterInfo->id != UserData::getUserId(),"monster = userID");
		EntityInfo::Get()->Set(pMonsterInfo);

		LevelLayer * layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			layer->addMonster(mInitInfo);	
			if (msg->is_summon())
			{
				ParticleManager::Get()->playOneTimeEffect(207, mInitInfo.pos);
			}
		}
		else
		{			
			MonsterData::GetMonsterInfo()->addMonser(msg->monsterinfo(i));
		}
		//LevelManager::sShareInstance()->getCurrentLevelLayer()->addMonster(uid, nick.c_str(), typeId,totalHp, pixPoint, speed, isBoss);		
	}
	//TIMEUSPEND.time_end("MonsterCreate");
}

void MessageHandler::handleMsgMonsterMove()
{
	CSMonsterMoveRsp* msg = dynamic_cast<CSMonsterMoveRsp*>(mCurNormMsgPackage->msg);	

	int moveCount = msg->moves_size();
	for (int i = 0; i < moveCount; ++i)
	{
		MonsterMoveValue moves = msg->moves(i);
		int pathCount = moves.paths_size();
		int uid = moves.id();

		if(pathCount > 0)
		{
			SpriteMonster * monster = SpriteMonsterMgr::Get()->GetMonster(uid);
			//CCAssert(monster != NULL, "can't find the monster when try to move it!");
			if (monster)
			{
				CCPoint pos = monster->getPosition();
				CCPoint cellPos =  LevelManager::sShareInstance()->pointPixelToCell(pos);
				//CCLOG("now pos: %f, %f", cellPos.x, cellPos.y);

				std::vector<CCPoint> points;
				for(int j = 0; j < pathCount; ++j)
				{
					CSPoint path = moves.paths(j);	
					CCPoint pixPoint = LevelManager::sShareInstance()->pointCellToPixel(ccp(path.xpos(), path.ypos()));
					LevelLayer*ll= LevelManager::sShareInstance()->getCurrentLevelLayer();
					if(ll == NULL)//jackniu network break, reconnect. crash here
						return;
					bool isReachable =	ll->getPhysicsLayer()->isPointReachable(pixPoint);
					if (!isReachable)
					{
						ASSERT(0, "Point unreachable");
						continue;
					}
					//CCLOG("points: %d, %d", path.xpos(), path.ypos());
					points.push_back(pixPoint);
				}

				//CCLOG("move monster");
				monster->onEventMsgMove(points);
			}

		}
	}
}

void MessageHandler::handleMsgMonsterDie()
{
	//CCLOG("----------handleMsgMonsterDie------------");
	CSMonsterDieRsp* msg = dynamic_cast<CSMonsterDieRsp*>(mCurNormMsgPackage->msg);	
	int deadCount = msg->monsters_size();

	vector<int> monsterIdVec;
	for(int i=0;i<deadCount;i++)
	{		
		unsigned int uid = msg->monsters(i);
		//CCLOG("---Monster %d Die---", uid);
		monsterIdVec.push_back(uid);
		SpriteMonster* monster = SpriteMonsterMgr::Get()->GetMonster(uid);
		if(monster)
		{
			monster->onEventMsgDead();
		}
	}

	bool isPVE = LevelManager::sShareInstance()->isCurrentPVELevel();
	bool isUnder = LevelManager::sShareInstance()->isCurrentUndergroundLevel();
	bool isElite = LevelManager::sShareInstance()->isCurrentEliteLevel();
	if ((isPVE || isUnder || isElite) && monsterIdVec.size() > 0)
	{
		OnlineNetworkManager::sShareInstance()->sendRemoteMonsterDieReqMessage(monsterIdVec);
		CCLOG("------sendRemoteMonsterDieReqMessage----------");
	}
}

void MessageHandler::handleMsgMonsterHit()
{
	CSMonsterHitRsp* msg = dynamic_cast<CSMonsterHitRsp*>(mCurNormMsgPackage->msg);	
	int userCount = msg->users_size();
	for(int i=0;i<userCount;i++)
	{
		if (!msg->users(i).reg_time() && !msg->users(i).channel_id())
		{
			//怪物被打变红
			int monsterId = msg->users(i).userid();
			SpriteMonster * monster = SpriteMonsterMgr::Get()->GetMonster(monsterId);
			if (monster)
			{
				monster->playBecomeRedEffect(true);
			}
			continue;
		}


		PLAYER_ID playId;
		playId.setPlayId(msg->users(i).userid(),msg->users(i).reg_time(),msg->users(i).channel_id());
		int blood = msg->users(i).blood();		
		unsigned int type = msg->users(i).hurt_type();
		unsigned int ragePoint = msg->users(i).rage_point();
		
		if(playId == UserData::getUserId())
		{

			OnlineNetworkManager::sShareInstance()->sendHeroHPChangeReq(-blood);
		}

		LevelLayer* pLevelLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if(pLevelLayer)
		{
			SpriteSeer* hero = pLevelLayer->getPlayer(playId);
			if(hero)
			{
				CCPoint damagePoint = ccpAdd(hero->getPosition(),hero->getAttachPoint(kType_PLAYER_NAME));
				switch(type)
				{
				case 1: // ����
					GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackMissEffect(damagePoint);
					break;
				case 2: // ��
					hero->ByAttacked(blood, ragePoint);
					GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackBlockEffect(damagePoint);
					break;
				case 3: // ����
					hero->ByAttacked(blood, ragePoint, true);
					break;
				default:
					hero->ByAttacked(blood, ragePoint);
					break;
				}
				
			}
		}

		
	}
}

void MessageHandler::handleMsgMonsterBuff()
{
	CCLOG("MessageHandler::handleMsgMonsterBuff.............");
	CSBuffEffectRsp* msg = dynamic_cast<CSBuffEffectRsp*>(mCurNormMsgPackage->msg);	
	int buffCount = msg->effects_size();
	for (int i = 0; i < buffCount; i++)
	{
		CSMonsterBuffEffectInfo effectInfo = msg->effects(i);

		int animId = effectInfo.action_id();

		BuffInfo info;
		info.skillId = effectInfo.skill_id();
		info.effectId = effectInfo.effect_id();
		info.delayTime = effectInfo.delay_time();
		info.deadTime = effectInfo.dead_time();

		int buffInfoSize = effectInfo.buff_info_size();
		for (int j = 0; j < buffInfoSize; j++)
		{
			CSMonsterBuffEffectInfo_BuffEffectInfo buffInfo = effectInfo.buff_info(j);
			int monsterId = buffInfo.id();			
			MonsterBattleInfo battleInfo = buffInfo.battle_info();
			SpriteMonster * monster = SpriteMonsterMgr::Get()->GetMonster(monsterId);
			if (monster)
			{
				monster->onEventMsgBuff(animId, info, battleInfo);
			}
		}
	}
}

void MessageHandler::handleMsgPlayerAttackMonster()
{
	CSMonsterBloodRsp* msg = dynamic_cast<CSMonsterBloodRsp*>(mCurNormMsgPackage->msg);
		unsigned int count = msg->monsters_size();
		for(size_t i=0;i<count;i++)
		{
			CSAttackMonsterBackInfo monsterInfo = msg->monsters(i);
			SpriteMonster* monster = SpriteMonsterMgr::Get()->GetMonster(monsterInfo.id());
			if(!monster)
				continue;
			unsigned int curBlood = monster->getBlood();
			if(curBlood != monsterInfo.blood())
			{
				monster->setBlood(monsterInfo.blood());					
			}
		}

		if(count > 0)
		{
			// UIѪ��ֻ��ʾ���һ��??
			CSAttackMonsterBackInfo currentMonsterInfo = msg->monsters(count -1);
			unsigned int monsterId = currentMonsterInfo.id();
			unsigned int monsterType = currentMonsterInfo.monster_type();
			SpriteMonster* monster = SpriteMonsterMgr::Get()->GetMonster(monsterId);
			if(monster)
			{
				unsigned int isBoss = LuaTinkerManager::Get()->getLuaConfig<unsigned int>("MonsterBasicInfomation", "MonsterBasicInfomation", monsterType, "IsBoss");
				unsigned int nameType = 1;
				if(currentMonsterInfo.elite())
				{
					nameType = 2;
				}
				if(isBoss > 0)
				{
					nameType = 3;
				}
				PLAYER_ID playId;
				playId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());
				bool isSelf = playId == UserData::getUserId();
				PLAYER_ID monster_id;
				monster_id.setPlayId(monsterId,0,0);
				ParticleManager::Get()->SetEnemyHpLable(currentMonsterInfo.blood(),monster_id,nameType,monsterType,isSelf);
			}
		}
}

void MessageHandler::handleLeaveBattle()
{
	XLogicManager::sharedManager()->setNeedShowPPVEAfterBacktoTown(false);
	GameDataManager::Get()->setHeroAutoAttack(false);
	TimeManager::Get()->stopTimer(TIMER_INSTANCE_COUNTDOWN);
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{				
		BattleCoverLayer* battleCoverLayer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
		battleCoverLayer->closeBattleCoverLayer(NULL);
	}
	if (LevelManager::sShareInstance()->isCurrentPVAILevel())
	{
		PvAIManager::Get()->setAIAutoAttack(false);
	}
	if (LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
	{
		GuildInstanceManager::Get()->setAIAutoAttack(false);
	}
	CSBattleLeaveRsp* msg = dynamic_cast<CSBattleLeaveRsp*>(mCurNormMsgPackage->msg);
	if(msg != NULL)
	{
		MainMenuLayer::hideUILayerByTag(MainMenuLayer::getLayerTagByName("BattleUILayer"));
		HSJoystick::sharedJoystick()->setIsEnable(false);

		if(msg->initiative())
		{ // ���������?
			XLogicManager::sharedManager()->backToTown();
		}
		else
		{
            int star = msg->star();
            int killtime = msg->finish_time();
            int killnum = msg->dead_mon_num();
			int battleTag = mLayer->getLayerTagByName("BattleUILayer");
			BattleUILayer* layer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
            int killcombo = layer->GetMaxComboNumber();
            
            CSReward reward = msg->rewards();
            int size = reward.rewards_size();
            
            int add_coin = reward.add_coin();
            int add_diamond = reward.add_diamond();
			int add_fairyexp = reward.add_phase_exp();
            int add_exp = reward.add_exp();
            int add_exploit = reward.add_exploit();
			int add_contributions = 0;
            
            bool pvp = LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel();
			bool pvai = LevelManager::sShareInstance()->isCurrentPVAILevel();
			bool isPveMode = LevelManager::sShareInstance()->isCurrentPVELevel();
			bool isPAIvEMode = LevelManager::sShareInstance()->isCurrentGuildInstanceLevel();
			bool isBestFighter = LevelManager::sShareInstance()->isCurrentBestFighterLevel();

			std::vector<accountReward> vec;
			for(int i=0; i<size; i++)
			{
				CSReward_RewardInfo info = reward.rewards(i);

				if(416025 == info.item_id())
				{
					//416025是公会贡献，不放在掉落列表中
					add_contributions = info.item_num();
				}
				else
				{
					accountReward accReWard;
					accReWard.item_id = info.item_id();
					accReWard.item_level = info.item_level();
					accReWard.item_num = info.item_num();
					vec.push_back(accReWard);
				}
			}

			if (msg->has_next_instance_id())
			{
				LuaTinkerManager::Get()->callLuaFunc<int>("Script/Instance/UndergroundInstance.lua", "SetNextUnderInstanceId", msg->next_instance_id());
			}
			else
			{
				LuaTinkerManager::Get()->callLuaFunc<int>("Script/Instance/UndergroundInstance.lua", "SetNextUnderInstanceId", 0);
			}

			if (isPAIvEMode)
			{
				AccountsLayer::s_accountDetailData.killtime = killtime;
				AccountsLayer::s_accountDetailData.killnum = killnum;
				AccountsLayer::s_accountDetailData.killcombo = killcombo;
				AccountsLayer::s_accountDetailData.add_coin = add_coin;
				AccountsLayer::s_accountDetailData.add_contributions = add_contributions;
				AccountsLayer::s_accountDetailData.vec = vec;
				AccountsLayer::s_accountDetailData.isPAIvEMode = true;
			}
			else if (isPveMode)
			{
				AccountsLayer::s_accountDetailData.star = star;
				AccountsLayer::s_accountDetailData.killtime = killtime;
				AccountsLayer::s_accountDetailData.killnum = killnum;
				AccountsLayer::s_accountDetailData.killcombo = killcombo;
				AccountsLayer::s_accountDetailData.add_coin = add_coin;

				//AccountsLayer::s_accountDetailData.add_diamond = add_diamond;
				// 钻石掉落改为精元掉落
				AccountsLayer::s_accountDetailData.add_diamond = add_fairyexp;
				
				AccountsLayer::s_accountDetailData.add_exp = add_exp;
				AccountsLayer::s_accountDetailData.add_exploit = add_exploit;
				AccountsLayer::s_accountDetailData.vec = vec;
				AccountsLayer::s_accountDetailData.isPveMode = true;
                
			}
            else
			{
				if(!(pvp || pvai || isBestFighter))
				{
					AccountsLayer *player = AccountsLayer::create();
					MainMenuLayer* layer = GetMainMenuLayer();
					int tag = layer->getLayerTagByName("AccountsLayer");
                    layer->removeChildByTag(tag, true);
					layer->addChild(player,10,tag);
					player->openWinBattle();
					//player->accountBattle(star, killtime, killnum, killcombo, add_coin, add_diamond, add_exp, add_exploit, vec);
					player->accountBattle(star, killtime, killnum, killcombo, add_coin, add_fairyexp, add_exp, add_exploit, add_contributions, vec);
				}
			}
            
			///get current map type
			bool bPPveMap = LevelManager::sShareInstance()->isCurrentPPVELevel();
			
			if (bPPveMap)
			{
				XLogicManager::sharedManager()->setNeedShowPPVEAfterBacktoTown(true);
			}

			// Note: �жϴ˴��Ƿ��о��鶯����??
			bool bCutAnimation = false;
			bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
			bool bPVEMode = LevelManager::sShareInstance()->isCurrentPVELevel();
			bool bStoryMode = false;
			unsigned int nStoryType = 0;
			//bPVEMode = false;
			
			//not use star number to check if the first play of this instance. use battle_times to 
			//check. if battle_times == 1 , play the story
			//int nInstanceStars = InstanceManager::Get()->getOneInstanceStars(InstanceManager::Get()->getCurInstanceId());
			InstanceInfo *	info =  InstanceManager::Get()->getOneInstance(InstanceManager::Get()->getCurInstanceId());
			if(bPVEMode && false == bMainLandLevel &&  info->battle_times == 1)
			{
				unsigned int mapId = LevelManager::sShareInstance()->getCurMapId();
				bStoryMode = StoryDataCenter::Get()->IsOneItemHasStory(InstanceManager::Get()->getCurInstanceId(),mapId,1,nStoryType);
				if (bStoryMode)
				{
					// Note: ���븱��ʱ������??
					if (nStoryType == 1)
					{
						StoryInstanceDirector::Get()->Begin(InstanceManager::Get()->getCurInstanceId(),mapId,nStoryType);
						bCutAnimation = true;
					}
				}
			}
			if (false == bCutAnimation)
			{
				XLogicManager::sharedManager()->FinishBattle();
			}
		}		
	}
}

void MessageHandler::handleMsgMonsterAttack()
{
	CSMonsterAttackRsp* msg = dynamic_cast<CSMonsterAttackRsp*>(mCurNormMsgPackage->msg);	

	int attackCount = msg->attacks_size();
	for (int i = 0; i < attackCount; ++i)
	{
		CSMonsterAttackInfo attack = msg->attacks(i);
		
		int uid = attack.id();
		int animId = attack.action_id();
		CSPoint position = attack.pos();
		CSPoint direct = attack.direct();		
		//int userId = attack.userid();

		CCPoint attackDir = ccp(direct.xpos(), direct.ypos());
		
		//��Ч������?
		SkillInfo info;
		
		info.skillId = attack.skill_id();
		info.type = attack.type();
		info.effectId = attack.effect_id();
		info.playType = attack.effect_animation();
		CSPoint spos = attack.start_pos();
		CSPoint epos = attack.end_pos();
		CSPoint vpos = attack.virtual_pos();
		info.startPosition = LevelManager::sShareInstance()->pointCellToPixel(ccp(spos.xpos(), spos.ypos()));
		info.endPosition = LevelManager::sShareInstance()->pointCellToPixel(ccp(epos.xpos(), epos.ypos()));
		info.virPosition = LevelManager::sShareInstance()->pointCellToPixel(ccp(vpos.xpos(), vpos.ypos()));
		info.speed = attack.skill_speed();
		info.delayTime = attack.delay_time();
		int shakeTimeCount = attack.shake_time_size();
		for (int i = 0; i < shakeTimeCount; i++)
		{
			float shakeTime = attack.shake_time(i);
			info.shakeTimeVec.push_back(shakeTime);
		}

		CCPoint pixPosition = LevelManager::sShareInstance()->pointCellToPixel(ccp(position.xpos(), position.ypos()));
		
		SpriteMonster * monster = SpriteMonsterMgr::Get()->GetMonster(uid);
		if(monster)
		{
			if(monster->fsm_rule.GetState() == MONSTER_STATE_HURT)
			{
				continue;
			}

			if (attack.has_dead_time())
			{
				monster->setAttackContinueTime(attack.dead_time() / 1000.0f);
			}

			//˲���յ�
			if (info.type == ATTACK_TYPE_BLINK || info.type == ATTACK_TYPE_HIDE)
			{
				CCPoint nowPos = monster->getPosition();
				monster->setBlinkPoint(info.endPosition);
				//CCLOG("Blink.........(%f, %f) to (%f, %f)", nowPos.x, nowPos.y, info.endPosition.x, info.endPosition.y);
			}
			else if (info.type == ATTACK_TYPE_STAND || info.type == ATTACK_TYPE_FAR)
			{
				ParticleManager::Get()->playEffectByInfoMonster(info, monster);
			}

			//���ù����ٶȣ�������BOSS�õ�
			monster->setAttackSpeed(info.speed * 16);

			monster->onEventMsgAttack(pixPosition, attackDir, animId, info);
		}
	}
}
void MessageHandler::handleMsgPVPLearnPrepareRsp()
{
	    
		CSPVPLearnPrepareRsp* msg = dynamic_cast<CSPVPLearnPrepareRsp*>(mCurNormMsgPackage->msg);
		PVPLearnInfo LearnInfo ;
		bool isbuyopen = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/buydiamondUI.lua","isbuyDiamondUIOpen");
		bool isMainLandLevel = MainLandManager::Get()->isMainLandType(LevelManager::sShareInstance()->getCurMapId());
		if(LevelManager::sShareInstance()->getCurrentLevelStatus()!=EInHomeTown|| isbuyopen||!isMainLandLevel)
		{
			return ;
		}
		LearnInfo.channel_id = msg->channel_id();
		LearnInfo.uid = msg->uid() ;
		LearnInfo.reg_tm = msg->reg_time() ;
		LearnInfo.mandatory = msg->mandatory() ;
		LearnInfo.type = msg->type() ;
		LearnInfo.playName = msg->name() ;
		LearnInfo.status = 0 ;
		MainMenuLayer* mainlayer = GetMainMenuLayer();
		int tag = MainMenuLayer::getLayerTagByName("PkDiolgUILayer");
		mainlayer->removeChildByTag(tag);

		PVPLearnLayer* layer = (PVPLearnLayer*)	PVPLearnLayer::create() ;
		

		layer->setStyle( LearnInfo);
		mainlayer->addChild(layer,Z_Order_WaitingLayer ,tag);
	    //XLogicManager::sharedManager()->setVisblePKButton(true);


}
void MessageHandler::handleMsgLearnMandatoryRsp()
{
	CSPVPLearnRsp* msg = dynamic_cast<CSPVPLearnRsp*>(mCurNormMsgPackage->msg);
}

void MessageHandler::handleMsgPVPLearnRsp()
{
	
	
	CSPVPLearnRsp* msg = dynamic_cast<CSPVPLearnRsp*>(mCurNormMsgPackage->msg);
	PVPLearnInfo LearnInfo ;
	LearnInfo.channel_id = msg->channel_id() ;
	LearnInfo.uid = msg->uid();
	LearnInfo.playName = msg->name() ;
	LearnInfo.effectiveness = msg->effectiveness() ;
	LearnInfo.reg_tm = msg->reg_time() ;
	LearnInfo.type = msg->type() ;
	LearnInfo.mandatory = msg->mandatory();
	LearnInfo.status = 1;
	bool isbuyopen = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/buydiamondUI.lua","isbuyDiamondUIOpen");
	bool isMainLandLevel = MainLandManager::Get()->isMainLandType(LevelManager::sShareInstance()->getCurMapId());
	if(PVPLearnLayer::getShow()|| LevelManager::sShareInstance()->getCurrentLevelStatus()!=EInHomeTown|| isbuyopen||!isMainLandLevel)
	{
		OnlineNetworkManager::sShareInstance()->SendPVPAckLearnReq(LearnInfo.uid ,LearnInfo.reg_tm ,LearnInfo.channel_id ,false,3);
		return ;
	}
	//
	PVPLearnLayer* layer = (PVPLearnLayer*)	PVPLearnLayer::create() ;
	MainMenuLayer* mainlayer = GetMainMenuLayer();
	int tag = MainMenuLayer::getLayerTagByName("PkDiolgUILayer");
	mainlayer->addChild(layer,Z_Order_WaitingLayer ,tag);
	layer->setVisible(false);
	if(LearnInfo.mandatory)
	{
		int tag1 = MainMenuLayer::getLayerTagByName("ForcePKAnimLayer");
		MainMenuLayer::showUILayerByTag<CCLayer>(tag1);
	
		layer->mandSendAccept();
        layer->setStyle(LearnInfo);
	    //scheduleOnce(schedule_selector(HelloWorld::updateLogin), 0.1f);

	}else
	{
		
		layer->setVisblePKButton(true ,LearnInfo);

	}

	
  

	
}
void MessageHandler::handleMsgAckLearnRefuseRsp()
{

	CSPVPAckLearnRefuseRsp* msg = dynamic_cast<CSPVPAckLearnRefuseRsp*>(mCurNormMsgPackage->msg);
	PVPLearnInfo LearnInfo ;
    int tag1 = MainMenuLayer::getLayerTagByName("ForcePKAnimLayer");
    MainMenuLayer::hideUILayerByTag(tag1);
	LearnInfo.channel_id = msg->channel_id() ;
	LearnInfo.uid = msg->uid();
	//LearnInfo.playName = msg
	LearnInfo.playName = msg->name();
	//LearnInfo.effectiveness = msg->effectiveness() ;
	LearnInfo.reg_tm = msg->reg_time() ;
	LearnInfo.type = msg->type() ;
	if(LearnInfo.type<1)LearnInfo.type =1 ;
	if(LearnInfo.type>3)LearnInfo.type =3 ;
	const char* rst = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/GameUILayer.lua", "getPKTypeCH", LearnInfo.type+6);
	char strShow[100];
	strcpy(strShow ,LearnInfo.playName.c_str());
	strcat(strShow ," ");
	strcat(strShow ,rst);
	//char  Msg[100];
	// snprintf(Msg ,100 ,"-%d",-num);
	 ShowMessage(strShow );
	//LearnInfo.status = 1;
	//PVPLearnLayer* layer = (PVPLearnLayer*)	PVPLearnLayer::create() ;
	//layer->setVisblePKButton(true ,LearnInfo);



}
void MessageHandler::handleMsgMonsterEffectRsq()
{
	//CCLOG("-----------------handleMsgMonsterEffectRsq---------------");
	CSMonsterEffectRsp* msg = dynamic_cast<CSMonsterEffectRsp*>(mCurNormMsgPackage->msg);
	
	int effectCount = msg->effects_size();
	for (int i = 0; i < effectCount; i++)
	{
		CSMonsterEffectInfo effectInfo = msg->effects(i);
		SkillInfo info;
		info.monsterId = effectInfo.id();
		info.type = effectInfo.type();
		info.playType = effectInfo.effect_animation();
		info.effectId = effectInfo.effect_id();
		info.delayTime = effectInfo.delay_time();
		info.speed = effectInfo.skill_speed();
		info.startPosition = LevelManager::sShareInstance()->pointCellToPixel(ccp(effectInfo.start_pos().xpos(), effectInfo.start_pos().ypos()));
		info.endPosition = LevelManager::sShareInstance()->pointCellToPixel(ccp(effectInfo.end_pos().xpos(), effectInfo.end_pos().ypos()));

		if (ATTACK_TYPE_JUMP == info.type)
		{
			SpriteMonster * monster = SpriteMonsterMgr::Get()->GetMonster(info.monsterId);
			if (monster)
			{
				monster->setJumpDownPosition(info.endPosition);
			}			
		}
		else
		{
			ParticleManager::Get()->playEffectByInfo(info);
		}
		
	}
}

void MessageHandler::handleCheckSession()
{
	CCLOG("handleCheckSession");
	CSCheckSessionRsp* msg = dynamic_cast<CSCheckSessionRsp*>(mCurNormMsgPackage->msg);
	unsigned int uid = msg->user_id();
	int channel_id = msg->channel_id();
	USERINFO mUserData = UserData::GetUserInfo();
	mUserData.id.setPlayId(uid,0,channel_id);
	if(msg->has_access_token())
	{
		sprintf(mUserData.accessToken,"%s",msg->access_token().c_str());
	}
	else
	{
		sprintf(mUserData.accessToken,"%s","");
	}
	UserData::SetUserInfo(mUserData);

	XLogicManager::sharedManager()->onReceiveSession();
	//XLogicManager::sharedManager()->connectToServer();
}

void MessageHandler::handleCheckToken()
{
	CSCheckTokenRsp* msg = dynamic_cast<CSCheckTokenRsp*>(mCurNormMsgPackage->msg);
	if(msg->success())
	{
		XLogicManager::sharedManager()->onReceiveSession();
	}
	else
	{
		USERINFO info = UserData::GetUserInfo();
		info.tokenId = 0;
		UserData::SetUserInfo(info);
		XLogicManager::sharedManager()->backToLogin();
	}
}

void MessageHandler::handGetToken()
{
	CSGetTokenRsp* msg = dynamic_cast<CSGetTokenRsp*>(mCurNormMsgPackage->msg);
	USERINFO mUserData = UserData::GetUserInfo();
	mUserData.tokenId = msg->tokenid();
	UserData::SetUserInfo(mUserData);
}

//----------------------------------------------------------------------
void MessageHandler::handleMsgLoginIn()
{
	//XLogicManager::sharedManager()->createRole("welseyTest6",0);
	//return;

	CSLoginRsp* msg = dynamic_cast<CSLoginRsp*>(mCurNormMsgPackage->msg);	
	
	//XLogicManager::sharedManager()->onConnectToSever();

	USERINFO mUserData = UserData::GetUserInfo();
	CCAssert(mUserData.id.getID() == msg->userid(),"error userid");
	CCAssert(mUserData.id.getChannel() == msg->channel_id(),"error channel");
	//mUserData.tokenId = msg->tokenid();
	mUserData.level = msg->lv();
	//mUserData.exp = msg->exp();
	mUserData.type = msg->type();
	strcpy(mUserData.szName,msg->name().c_str());
	mUserData.id.setRegTime(msg->reg_time());
	UserData::SetUserInfo(mUserData);

	//CCLOG("tokenid: %lu",msg->tokenid());
	//CCLOG("name: %s",msg->name().c_str());
	//CCLOG("lv: %d",msg->lv());
	//CCLOG("exp: %d",msg->exp());

	MonsterData::GetMonsterInfo()->reset();
	XLogicManager::sharedManager()->loginSuccess();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UserDataManager.lua","checkUserInfo");
	ItemManager::Get()->checkAlchemyEquipUnlock();
}

void MessageHandler::handleUserFreezeRsp()
{
	CSFreezeTimeRsp* msg = dynamic_cast<CSFreezeTimeRsp*>(mCurNormMsgPackage->msg);

	unsigned int time = msg->tick();
	if(time == 0)
	{
		const char* text = Localizatioin::getLocalization("M_FOREVER_FREEZE");
		ShowMessage(text);
	}
	else
	{
		const char* text1 = Localizatioin::getLocalization("M_FREEZE_TIP_1");
		const char* text2 = Localizatioin::getLocalization("M_FREEZE_TIP_2");
		unsigned int day = time / (24 * 3600) + 1;
		char showText[100];
		sprintf(showText,"%s%u%s",text1,day,text2);
		ShowMessage(showText);
	}
	
	WaitingLayerManager::Get()->clearMessages();
	HideWaitingLayer();
};

void MessageHandler::handleMsgRoleList()
{
	CSGetRoleListRsp* msg = dynamic_cast<CSGetRoleListRsp*>(mCurNormMsgPackage->msg);
	XLogicManager::sharedManager()->onReceivedRoleList(msg);
}

//----------------------------------------------------------------------
void MessageHandler::handleMsgPlayerEnterMap()
{
	//fix bug:多人副本切后台时重新回到界面需要切换到主城，但是在loading状态，所以注释掉，加了转菊花
	//if(LevelManager::sShareInstance()->getCurrentLevelStatus() == EChangeMap)
	//{
	//	return;
	//}

	CSEnterMapRsp* msg = dynamic_cast<CSEnterMapRsp*>(mCurNormMsgPackage->msg);

	BasicUserInfo infor = msg->userinfo();
	USERINFO mUserData = UserData::GetUserInfo();

	//ASSERT(infor.uid() == mUserData.id,"Not a Same UUID");
	PLAYER_ID playerId;
	playerId.setPlayId(infor.uid(),infor.reg_time(),infor.channel_id());
	if(playerId == mUserData.id)
	{
		mUserData.Set(infor);

		mUserData.level = infor.lv();
		mUserData.mapId = msg->mapid();
		mUserData.xPos = infor.xpos();
		mUserData.yPos = infor.ypos();

		mUserData.orient = infor.orient();
		strcpy(mUserData.szName,infor.name().c_str());
		UserData::SetUserInfo(mUserData);
		MonsterData::GetMonsterInfo()->reset();
        if(!get1002value)
	    {//jackniu when firt to go to mainland. we sould send this
	     //msg to check if we firt play the game.
              int index[1];
        	 index[0] = 1002;
        	 OnlineNetworkManager::sShareInstance()->sendOnceReqMessage(index,1);
        }
        else
        {
         XLogicManager::sharedManager()->goToLayer(XLogicManager::E_ENTER_MAINLAND, &mUserData.mapId);
        }
	}
	else
	{
        // judge is in guild or not with me 
        bool inGuildScene = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE);
        if(inGuildScene)
        {
            bool isSameGuild = CallNativeFuncManager::getInstance()->isOwnGuildPlayer(infor.gid_high(), infor.gid_low());
            if(!isSameGuild)
            {
                return;
            }
        }
        
		//
		PLAYER_ID playerId;
		playerId.setPlayId(infor.uid(),infor.reg_time(),infor.channel_id());
		std::string nick = infor.name();
		int xPos = infor.xpos();
		int yPos = infor.ypos();

		// ���ʱ��û�������??
		//CCPoint pixelPos = LevelManager::sShareInstance()->pointCellToPixel(ccp(infor.xpos(), infor.ypos()));
		//xPos = pixelPos.x;
		//yPos = pixelPos.y;

		int orient = infor.orient();
		int type = infor.type();
		unsigned int side = infor.battle_side();
		
		USERINFO* pUserInfo = UserData::Get()->GetUserInfo(playerId);
		if(pUserInfo)
		{
			pUserInfo->Set(infor);
			UserData::Get()->SetUserInfo(pUserInfo->id, *pUserInfo);
		}
		else
		{
			USERINFO otherUserInfo;		
			otherUserInfo.Set(infor);
			UserData::Get()->SetUserInfo(otherUserInfo.id, otherUserInfo);
		}
		
		//
		LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			layer->addOtherPlayer(playerId, nick.c_str(), type, ccp(xPos, yPos), orient, side);
			SpriteSeer * seer = layer->getPlayer(playerId);
			if (seer)
			{
				if (infor.red_name())
				{
					seer->ShowNameLabelColor(true);
				}
				else
				{
					seer->ShowNameLabelColor(false);
				}
			}
		}
		else
		{
			LevelManager::OTHER_PLAYER_INFO playerInfo;
			playerInfo.uid = playerId;
			playerInfo.userName = nick;
			playerInfo.type = type;
			playerInfo.pos.x = xPos;
			playerInfo.pos.y = yPos;
			playerInfo.orient = orient;
			playerInfo.battleSide = side;

			LevelManager::sShareInstance()->m_otherPlayerInfo.push(playerInfo);
		}
	}
	CCLOG("seer enter map: %s, %d pos: %d, %d ", infor.name().c_str(), msg->mapid(), infor.xpos(), infor.ypos());	
}

//----------------------------------------------------------------------
void MessageHandler::handleMsgPlayerLeaveMap()
{
	CSLeaveMapRsp* msg = dynamic_cast<CSLeaveMapRsp*>(mCurNormMsgPackage->msg);
	//
	//int mapId = msg->map_id();	
	PLAYER_ID playerId;
	playerId.setPlayId(msg->uid(),msg->reg_time(),msg->channel_id());
	LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (layer)
	{
		layer->removeOtherPlayer(playerId);
	}
}

//----------------------------------------------------------------------
void MessageHandler::handleMsgPlayerWalk()
{
	bool isMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if (!isMainLandLevel)
	{
		return;
	}
	CSWalkRsp* msg = dynamic_cast<CSWalkRsp*>(mCurNormMsgPackage->msg);

	PLAYER_ID playerId;
	playerId.setPlayId(msg->uid(),msg->reg_time(),msg->channel_id());
	//uint64_t uuid = msg->uid();
	unsigned int xPos = msg->xpos();
	unsigned int yPos = msg->ypos();
	unsigned int animID = msg->animid();
	bool bFlip = msg->flip();

	//int xPos = msg->dest_x();
	//int yPos = msg->dest_y();

	LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (layer)
	{
		//layer->updateOtherPlayer(uuid,ccp(xPos,yPos), animID, bFlip);
		LevelLayer::WAKL_INFO walkInfo;
		walkInfo.walk.uid = playerId;
		walkInfo.walk.pt.x = xPos;
		walkInfo.walk.pt.y = yPos;

		walkInfo.walk.pt = LevelManager::sShareInstance()->pointCellToPixel(walkInfo.walk.pt);

		walkInfo.walk.animID = animID;
		walkInfo.walk.bFlip = bFlip;

		std::map<PLAYER_ID, LevelLayer::WAKL_INFO>::iterator iter;
		iter = layer->m_walkInfo.find(playerId);
		if(iter != layer->m_walkInfo.end())
		{
			iter->second.walk = walkInfo.walk;
		}
		else
		{
			walkInfo.idleTime.time = 0.0f;
			walkInfo.idleTime.curTime = 0.0f;
			layer->m_walkInfo.insert(std::make_pair(playerId, walkInfo));
		}
		
	}

	CCLOG("receive: seer walk dest: %d, %d, %d",playerId.getID(), xPos, yPos);
}


//----------------------------------------------------------------------
void MessageHandler::handleMsgGetOtherPlayers()
{
	bool isMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if (!isMainLandLevel)
	{
		return;
	}
	CSGetMapUsersRsp* msg = dynamic_cast<CSGetMapUsersRsp*>(mCurNormMsgPackage->msg);
	int playerCount = msg->userinfo_size();
	CCLOG("CSGetMapUsersRsp: size = %d",playerCount);
	for (int i = 0; i < playerCount; ++i)
	{
		BasicUserInfo info = msg->userinfo(i);
		//
		PLAYER_ID playerId;
		playerId.setPlayId(info.uid(),info.reg_time(),info.channel_id());
		//int uid = info.uid();
		std::string nick = info.name();
		int xPos = info.xpos();
		int yPos = info.ypos();

		CCAssert(!(xPos == 0 && yPos == 0), "Player pos(0, 0)");

		CCPoint pos(xPos, yPos);
		pos = LevelManager::sShareInstance()->pointCellToPixel(pos);
		xPos = pos.x;
		yPos = pos.y;

		int type = info.type();
		unsigned int side = info.battle_side();

		
		if(playerId == UserData::getUserId())
		{
            continue;
        }

        // judge is in guild or not with me 
        bool inGuildScene = LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE);
        if(inGuildScene)
        {
            bool isSameGuild = CallNativeFuncManager::getInstance()->isOwnGuildPlayer(info.gid_high(), info.gid_low());
            if(!isSameGuild)
            {
                return;
            }
        }

		USERINFO* pUserInfo = UserData::Get()->GetUserInfo(playerId);
		if(pUserInfo)
		{
			pUserInfo->Set(info);
			UserData::Get()->SetUserInfo(playerId, *pUserInfo);
		}
		else
		{
			USERINFO otherUserInfo;	
			otherUserInfo.Set(info);
			UserData::Get()->SetUserInfo(otherUserInfo.id, otherUserInfo);
		}
			
		LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			layer->addOtherPlayer(playerId, nick.c_str(), type, ccp(xPos, yPos), info.orient(), side);
			SpriteSeer * seer = layer->getPlayer(playerId);
			if (seer)
			{
				if (info.red_name())
				{
					seer->ShowNameLabelColor(true);
				}
				else
				{
					seer->ShowNameLabelColor(false);
				}
			}
		}
		else
		{
			LevelManager::OTHER_PLAYER_INFO playerInfo;
			playerInfo.uid = playerId;
			playerInfo.userName = nick;
			playerInfo.type = type;
			playerInfo.pos.x = xPos;
			playerInfo.pos.y = yPos;
			playerInfo.battleSide = side;

			LevelManager::sShareInstance()->m_otherPlayerInfo.push(playerInfo);
		}
	}
}

void MessageHandler::handleAckMsgCheckSession()
{

}

void MessageHandler::handleQuerySharedRsp()
{
	//CSQueryShardRsp* msg = dynamic_cast<CSQueryShardRsp*>(mCurNormMsgPackage->msg);
	//if(msg)
	//{
	//	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	//	CCArray* children = scene->getChildren();
	//	CCObject* child;
	//	CCARRAY_FOREACH(children, child)
	//	{
	//		HelloWorld* layer = dynamic_cast<HelloWorld*>(child);
	//		if(layer && layer->getFrontLayer())
	//		{
	//			layer->getFrontLayer()->onReceiveSeverInfo(msg);
	//		}
	//	}
	//	
	//}
}

void MessageHandler::handleAckMsgCreateRole()
{
	CSCreateRoleRsp* msg = dynamic_cast<CSCreateRoleRsp*>(mCurNormMsgPackage->msg);	

	USERINFO mUserData = UserData::GetUserInfo();
	unsigned int uid = msg->userid();
	unsigned int retTime = msg->reg_time();
	unsigned int channelId = msg->channel_id();
	mUserData.id.setPlayId(uid,retTime,channelId);

	UserData::SetUserInfo(mUserData);

	// to do
	XLogicManager::sharedManager()->onCreateRoleSuceess();
}


void MessageHandler::handleMsgBattlePVE()
{
	
}

void MessageHandler::handleMsgEcho()
{
	
}

void MessageHandler::handleBackPackItems()
{
	CSAllItemsRsp* msg = dynamic_cast<CSAllItemsRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		ItemManager* itemManager = ItemManager::Get();
		unsigned int defaultPackPos = msg->def_bag_size();
		unsigned int defaultStoragePos = msg->def_store_size();
		unsigned int defaultAlchemyStoragePos = msg->def_ss_bag_size();
		unsigned int defaultAlchemyBagPos = msg->def_ss_store_size();
		unsigned int defaultEquipUpgradeTimes = msg->def_equip_lvup_max_cd_time();
		//unsigned int defaultResetGemsTimes = msg->def_reset_gem_times();
		//unsigned int defaultManorTimes = msg->def_enter_manor_times();

		itemManager->resetItems();
		itemManager->setBackPackDefaultPos(defaultPackPos);
		itemManager->setStorageDefaultPos(defaultStoragePos);
		itemManager->setAlchemyBackDefaultCapacity(defaultAlchemyBagPos);
		itemManager->setAlchemyStorageDefaultCapacity(defaultAlchemyStoragePos);
		itemManager->setEquipMaxTimes(defaultEquipUpgradeTimes);
		int count = msg->items_size();
		for(int i=0;i<count;i++)
		{
			CSItemInfo packInfo = msg->items(i);
			itemManager->setItem(packInfo);
		}
		XLogicManager::sharedManager()->receivedItemsInfo();
		//itemManager->openBackPack();
	}
}

void MessageHandler::handleAddItem()
{
	CSItemAddRsp* msg = dynamic_cast<CSItemAddRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		int count = msg->items_size();
		for(int i=0;i<count;i++)
		{
			CSItemInfo packInfo = msg->items(i);
			ItemManager::Get()->setItem(packInfo);

			unsigned int pos = packInfo.pos();
			if(pos >= kItemHeroEquip && pos <= kAlchemyBag)
			{
				int tag = MainMenuLayer::getLayerTagByName("GameUILayer");
				// icon��ʶ
				GameUILayer* mainlayer = MainMenuLayer::getUILayerByTag<GameUILayer>(tag,false);
				if(mainlayer)
				{
					mainlayer->checkEquipUpgradeIcon();
				}
			}
		}
	}
}

void MessageHandler::handleMsgInstanceList()
{
	CSInstanceRsp* msg = dynamic_cast<CSInstanceRsp*>(mCurNormMsgPackage->msg);	

	int instanceCount = msg->new_instances_size();
	std::vector<InstanceInfo> instanceInfos;
	for (int i = 0; i < instanceCount; ++i)
	{
		CSInstanceInfo instance = msg->new_instances(i);

		InstanceInfo instanceInfo;
		instanceInfo.id = instance.instance_id();
		instanceInfo.star = instance.star();
		instanceInfo.can_enter_flag= instance.can_enter_flag();	
        instanceInfo.refresh_times = instance.refresh_times();	
        instanceInfo.isgray = false;
		instanceInfo.battle_times= instance.btl_times();
		instanceInfos.push_back(instanceInfo);	

		InstanceManager::Get()->addToTotalInstanceList(instanceInfo);
	}

    int count = instanceInfos.size();

    InstanceListLayer * instanceListLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
	if(instanceListLayer && instanceListLayer->getShowInstance())
	{
        InstanceManager::Get()->curInstanceList.clear();

        for(int i=0;i < count;i++)
        {
            InstanceManager::Get()->curInstanceList.push_back(instanceInfos[i]);
        }

		instanceListLayer->onEventMsgInstanceList(instanceInfos);
    }
#if 0
	if (InstanceManager::Get()->isInstanceListChanged(instanceInfos))
	{
		InstanceManager::Get()->curInstanceList.clear();
		int count = instanceInfos.size();

		for(int i=0;i < count;i++)
		{
			InstanceManager::Get()->curInstanceList.push_back(instanceInfos[i]);
		}

		InstanceListLayer * instanceListLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
		if(instanceListLayer)
		{
			instanceListLayer->onEventMsgInstanceList(instanceInfos);
		}
	}
#endif
}

void MessageHandler::handleBackPackItemRemove()
{
	CSItemRemoveRsp* msg = dynamic_cast<CSItemRemoveRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int pos = msg->pos();
		unsigned int itemId = msg->item_id();
		ItemManager::Get()->removeItem(pos,itemId);
	}
}

void MessageHandler::handleBackPackMove()
{
	CSItemMoveRsp* msg = dynamic_cast<CSItemMoveRsp*>(mCurNormMsgPackage->msg);

	if(msg)
	{
		unsigned int pos_from = msg->pos_from();
		unsigned int pos_to = msg->pos_to();
		ItemManager::Get()->exchangeItemPos(pos_from,pos_to);

		// Note: 武器换装
		SpriteSeer* pSeer = GameManager::Get()->getHero();
		if (pSeer)
		{
			unsigned int weaponId = ItemManager::Get()->getHeroWeaponId();
			pSeer->SetEquipItemsData(weaponId);
		}
	}
}

void MessageHandler::handleBackPackItemUpdate()
{
	CSItemUpdateRsp* msg = dynamic_cast<CSItemUpdateRsp*>(mCurNormMsgPackage->msg);

	if(msg)
	{
		ItemManager* itemManager = ItemManager::Get();
		int count = msg->item_size();
		for(int i=0;i<count;i++)
		{
			CSItemInfo packInfo = msg->item(i);
			itemManager->setItem(packInfo);
		}
	}
}

//void MessageHandler::handleEquipnetList()
//{
//	CSGetEquipLvUpCDRsp* msg = dynamic_cast<CSGetEquipLvUpCDRsp*>(mCurNormMsgPackage->msg);
//	if(msg)
//	{
//		ItemManager* itemManager = ItemManager::Get();
//		unsigned int coldTime = msg->cold_time();
//		int tag = MainMenuLayer::getLayerTagByName("EquipmentUpgradeUILayer");
//		EquipmentUpgradeUILayer* layer = MainMenuLayer::showUILayerByTag<EquipmentUpgradeUILayer>(tag);
//		if(layer)
//		{
//			layer->setUpgradeColdTime(coldTime);
//		}
//		
//		/*if(GameManager::Get()->GetSceneLayer() != NULL)
//		{
//		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
//		if (mLayer)
//		{
//		mLayer->ShowEquipmentUpgradeUI(coldTime);
//		}
//		}*/
//	}
//}
void MessageHandler::handleEquipmentStrength()
{
	CSEquipmentStrengthRsp* msg = dynamic_cast<CSEquipmentStrengthRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		ItemManager* itemManager = ItemManager::Get();
		//unsigned int coldTime = msg->cold_time();
		unsigned int success = msg->result();
		unsigned int pos = msg->pos();
		if(0 == success)
		{
			int tag = MainMenuLayer::getLayerTagByName("EquipmentUpgradeUILayer");
			EquipmentUpgradeUILayer* layer = MainMenuLayer::getUILayerByTag<EquipmentUpgradeUILayer>(tag,false);
			if(layer)
			{
				layer->successToStrength(pos);
			}
			tag = MainMenuLayer::getLayerTagByName("GameUILayer");
			// icon��ʶ
			GameUILayer* mainlayer = MainMenuLayer::getUILayerByTag<GameUILayer>(tag,false);
			if(mainlayer)
			{
				mainlayer->checkEquipUpgradeIcon();
			}
		}
		else
		{
			// ǿ��ʧ��
			CCLOG("EquipmentStrength: result= %d",success);
			const char* errMsg;
			switch(success)
			{
			case 1:
				errMsg = Localizatioin::getLocalization("M_NOT_EQUIP_GRID");
				break;
			case 2:
				errMsg = Localizatioin::getLocalization("M_NOEQUIP");
				break;
			case 3:
				errMsg = Localizatioin::getLocalization("M_NOT_A_EQUIP");
				break;
			case 4:
				errMsg = Localizatioin::getLocalization("M_EQUIPLEVEL_HIGHER");
				break;
			case 5:
				errMsg = Localizatioin::getLocalization("M_NO_MORE_ENHANCETIME");
				break;
			case 6:
				errMsg = Localizatioin::getLocalization("M_ENHANCE_INCD");
				break;
			case 7:
				errMsg = Localizatioin::getLocalization("M_FINDSPIRIT_COINSHORTAGE");
				break;
			default:
				errMsg = Localizatioin::getLocalization("M_UNKNOW_ERROR");
				break;
			}
			ShowMessage(errMsg);
		}
	}
}

void MessageHandler::handleUserItem()
{
	CSItemUseRsp* msg = dynamic_cast<CSItemUseRsp*>(mCurNormMsgPackage->msg);
	{
		if(msg)
		{
			ItemBase* info = ItemManager::Get()->getItemInfoById(msg->item_id());
			if(info)
			{
				if(info->m_useDes != "")
				{
					ShowMessage(info->m_useDes.c_str());
				}
				
			}
			int count = msg->to_items_size();
			if(count > 0)
			{
				for(int i =0;i < count; i++)
				{
					CSItemUseRsp_Item item = msg->to_items(i);
					if(item.id() == 416007 && msg->item_id() == 411241)
					{
						char text[100];
						const char* subText1 = Localizatioin::getLocalization("M_GETREWARD_TIPS_1");
						const char* subText2 = Localizatioin::getLocalization("M_MERIT");
						sprintf(text,"%s%u%s",subText1,item.num(),subText2);
						ShowMessage(text);
					}
					else if(item.id() == 416001 && msg->item_id() == 411240)
					{
						char text[100];
						const char* subText1 = Localizatioin::getLocalization("M_GETREWARD_TIPS_1");
						const char* subText2 = Localizatioin::getLocalization("M_GOLD");
						sprintf(text,"%s%u%s",subText1,item.num(),subText2);
						ShowMessage(text);
					}
					else
					{
						
						ItemBase* getInfo = ItemManager::Get()->getItemInfoById(item.id());
						if(getInfo)
						{
							char text[100];
							const char* subText1 = Localizatioin::getLocalization("M_GETREWARD_TIPS_1");
							const char* subText2 = Localizatioin::getLocalization("M_TEXT_1");
							sprintf(text,"%s%u%s%s",subText1,item.num(),subText2,getInfo->m_name);
							ShowMessage(text);
						}
					}

				}
			}
		}
	}
}

void MessageHandler::handleEquipCompose()
{
	CSEquipmentComposeRsp* msg = dynamic_cast<CSEquipmentComposeRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int ret = msg->result();
		unsigned int exchangeId = msg->exchange_item_id();
       
		/*if(ret != 0)
		{
			switch(ret)
			{
			case 1:
				// ����Ľ�����??
				ShowMessage(Localizatioin::getLocalization("M_EQUIPMENT_CHANGEINFORMATIONFALI"));
				break;
			case 2:
				// ��������Ʒ��??
				ShowMessage(Localizatioin::getLocalization("M_EQUIPMENT_CHANGEGOODSNOTMATCH"));
				break;
			case 3:
				// ��Ʒ����
				ShowMessage(Localizatioin::getLocalization("M_EQUIPMENT_GOODSSHORTAGE"));
				break;
			case 4:
				{
					// �ո񲻹�
					ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
					if(item && item->m_exchangeType == 4)
					{
						ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITWAREHOUSE_FULL"));
					}
					else
					{
						ShowMessage(Localizatioin::getLocalization("M_EQUIPMENT_SPACESHORTAGE"));
					}
					break;
				}
			case 5:
				{
					//ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
					//if(item && item->m_exchangeType == 3)
					//{
					//char content[50];
					//const char* str = Localizatioin::getLocalization("M_COMBINE_LEVELNOTREACH");
					//sprintf(content,"%d%s",item->m_requiredLevel,str);
					//ShowMessage(content);
					//}
					break;
				}
			case 6:
				// vip�ȼ���������error??
				break;
			default:
				ShowMessage(Localizatioin::getLocalization("M_UNKNOW_ERROR"));
				break;
			}
			
		}
		else*/
		if(ret == 0)
		{
			ExchangeItem* item = ItemManager::Get()->getExchageItemById(exchangeId);
            
			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
            BackPackLayer* back = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
            if(back)
            {
                ShopUILayer* shop = back->getpShopUILayer();
                if(shop && item && item->m_exchangeType == 8)
                {
                    shop->freshExchangeCoinVal();
                }
            }
            
			if(item && item->m_exchangeType == 4)
			{
				ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITCHANGE_SUCCESS"));
			}
			else if(item && item->m_exchangeType == 5)
			{
				std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_toItems->begin();
				if (iter != item->m_toItems->end())
				{
					unsigned int key = (*iter).first;
					SpriteElfConfigFromLuaManager::getInstance()->PushExchangeElfRet(key);
				}
			}
			else if(item && item->m_exchangeType == 3)
			{
				// Note: ����������??
				SpriteSeer* pSeer = GameManager::Get()->getHero();
				if (pSeer)
				{
					unsigned int weaponId = ItemManager::Get()->getHeroWeaponId();
					pSeer->SetEquipItemsData(weaponId);
				}
				GameAudioManager::sharedManager()->playEffect(350005,false);

				char str[100];
				const char* text1 = Localizatioin::getLocalization("M_FRAGMENT_SPLICE_1");
				const char* text2 = Localizatioin::getLocalization("M_FRAGMENT_SPLICE_2");
				string equipName = "";
				std::list<std::pair<unsigned int ,unsigned int> >::iterator iter = item->m_toItems->begin();
				if (iter != item->m_toItems->end())
				{
					unsigned int equipId = (*iter).first;
					ItemBase* info = ItemManager::Get()->getItemInfoById(equipId);
					if(info)
					{
						equipName = info->m_name;
					}
				}
				sprintf(str,"%s%s%s",text1,equipName.c_str(),text2);
				ShowMessage(str);
				/*int tag = MainMenuLayer::getLayerTagByName("EquipMixLayer");
				EquipMixLayer* layer = MainMenuLayer::showUILayerByTag<EquipMixLayer>(tag,false);
				if(layer)
				{
					layer->onReceivedComposeSuccess();
				}

				tag = MainMenuLayer::getLayerTagByName("GameUILayer");
				// icon��ʶ
				GameUILayer* mainlayer = MainMenuLayer::getUILayerByTag<GameUILayer>(tag,false);
				if(mainlayer)
				{
					mainlayer->checkEquipUpgradeIcon();
				}*/
			}
			else if(item && item->m_exchangeType == 6)
			{
				int count = msg->items_size();
				if(count > 0)
				{
					for(int i = 0;i < count;i++)
					{
						ItemBase* itemInfo = ItemManager::Get()->getItemInfoById(msg->items(i).id());
						if(itemInfo)
						{
							string str = CCString::createWithFormat(Localizatioin::getLocalization("M_EXCHANGE_SUCCESS"),msg->items(i).num())->m_sString;
							string text = CCString::createWithFormat("%s%s",str.c_str(),itemInfo->m_name)->m_sString;
							ShowMessage(text.c_str());
						}
						
					}
				}
				GemStonesFromLua::getInstance()->GemAnimaExchangeRsp(0);
			}
			else if(item && item->m_exchangeType == 9)
			{
				int tag = MainMenuLayer::getLayerTagByName("CardUILayer");
				CardUILayer* layer = MainMenuLayer::showUILayerByTag<CardUILayer>(tag,false);
				if(layer)
				{
					layer->onReceivedCardComposeSuccess(0);
				}
				GameAudioManager::sharedManager()->playEffect(350005,false);
			}else if (item && item->m_exchangeType == 2)
			{
				// Note: 钻石兑换体力
				ShowMessage(Localizatioin::getLocalization("M_BUY_ACTION_SUCCESS"));
			}
			else if (item && item->m_exchangeType == 16)
            {
                 ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITCHANGE_SUCCESS"));
                CCNotificationCenter::sharedNotificationCenter()->postNotification("updateItemExchangeInfo");
            }
            else
			{	
				GameAudioManager::sharedManager()->playEffect(350005,false);
				ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITCHANGE_SUCCESS"));
			}
		}
	}
}

void MessageHandler::handleSearchSpriteStoneRsp()
{
	CSSearchSpiritStoneRsp* msg = dynamic_cast<CSSearchSpiritStoneRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		//AlchemyUILayer* layer = ShowAlchemyUILayer();
		int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
		AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag);

		unsigned int ret = msg->ret();
		if(ret == 0)
		{
			unsigned int costCoin = msg->cost_coin();
			unsigned int npcIndex = msg->mon_id();
			unsigned int itemId = 0;
			if(msg->has_item_id())
			{
				itemId = msg->item_id();
			}
			if(layer)
			{
				layer->onReceivedSearchResult(costCoin,npcIndex+1,itemId);
			}
		}
		else
		{
			if(ret == 1)
			{
				ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_COINSHORTAGE"));
			}
			else if(ret == 2)
			{
				ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITSTONEFALL"));
			}
			else
			{
				ShowMessage(Localizatioin::getLocalization("M_UNKNOW_ERROR"));
			}
			if(layer)
			{
				layer->onReceivedSearchError(ret);
			}
			
		}
	}
}

void MessageHandler::handleSpriteStoneMoveStorageRsp()
{
	CSSpiritStoneMoveBagRsp* msg = dynamic_cast<CSSpiritStoneMoveBagRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int ret = msg->ret();
		if(ret != 0)
		{
			if(ret == 1)
			{
				ShowMessage("error pos");
			}
			else if(ret == 2)
			{
				ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_BACKPACKFULL"));
			}
			else
			{
				ShowMessage(Localizatioin::getLocalization("M_UNKNOW_ERROR"));
			}
		}
		//else
		//{
		//	XLogicManager::sharedManager()->reqPlayAttributes();
		//}
	}
}

void MessageHandler::handleSpriteUpgradeRsp()
{
	CSSpiritStoneLevelUpRsp* msg = dynamic_cast<CSSpiritStoneLevelUpRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int ret = msg->ret();
		if(ret == 0)
		{
			//unsigned int costEnergy = msg->cost_sp();
			ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITPROMOTE_SUCCESS"));
			//AlchemyUILayer* layer = ShowAlchemyUILayer();
			int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
			AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag);
			if(layer)
			{
				layer->onReceivedUpgradeSuccess();
			}
		}
		else
		{
			switch(ret)
			{
			case 1:
				ShowMessage(Localizatioin::getLocalization("M_ALCHEMY_SOULNOTINBAG"));
				break;
			case 2:
				ShowMessage(Localizatioin::getLocalization("M_ALCHEMY_NOSOULHERE"));
				break;
			case 3:
				ShowMessage(Localizatioin::getLocalization("M_ALCHEMY_NOTENOUGH_ENERGY"));
				break;
			case 4:
				ShowMessage(Localizatioin::getLocalization("M_REACH_HIGHESTLEVEL"));
				break;
			default:
				ShowMessage(Localizatioin::getLocalization("M_UNKNOW_ERROR"));
				break;
			}
			
		}
	}
}

void MessageHandler::handleSpriteTransToEnergy()
{
	CSSpiritStoneToSPRsp* msg = dynamic_cast<CSSpiritStoneToSPRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int ret = msg->ret();
		if(ret == 0)
		{
			unsigned int key = msg->sp_key();
			unsigned int num = msg->sp_value();
			CCLOG("success to trans soul energy: [key: %d ,value: %d]",key,num);
			int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
			AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag,false);
			if(layer)
			{
				layer->onReceiveSpriteTrans();
			}
		}
		else
		{
			switch(ret)
			{
			case 1:
				ShowMessage(Localizatioin::getLocalization("M_ALCHEMY_SOULNOTINBAG"));
				break;
			case 2:
				ShowMessage(Localizatioin::getLocalization("M_ALCHEMY_NOSOULHERE"));
				break;
			default:
				ShowMessage(Localizatioin::getLocalization("M_UNKNOW_ERROR"));
				break;
			}
		}
	}
}

void MessageHandler::handleSpriteExchangeRsp()
{
	CSGetExchangeSSInfoRsp* msg = dynamic_cast<CSGetExchangeSSInfoRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		int tag = MainMenuLayer::getLayerTagByName("AlchemyUILayer");
		AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(tag,false);
		if(layer)
		{
			layer->onReceivedExchangeInfo(msg);
		}
	}
}

void MessageHandler::handleEquipInlayGemRsp()
{
	CSEquipInlayGemRsp* msg = dynamic_cast<CSEquipInlayGemRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		GemStonesFromLua::getInstance()->EquipInlayGemRsp(msg->ret());
	}
}

void MessageHandler::handleGemRecastRsp()
{
	CSResetGemAttrRsp* msg = dynamic_cast<CSResetGemAttrRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		GemStonesFromLua::getInstance()->GemRecastRsp(msg->ret());
	}
}

void MessageHandler::handleExchangeParameterRsp()
{
	CSExchangeParameterRsp* msg = dynamic_cast<CSExchangeParameterRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		bool success = msg->succ();
		unsigned int exchangeId = msg->id();
		if(exchangeId >= 40090 && exchangeId <= 40120)
		{
			GemStonesFromLua::getInstance()->GemHoleUnlockRsp(success);
		}
        if(success && exchangeId == 10100) // speed talent
        {
            GetTalentLayer()->startCD(0);
            GetTalentLayer()->updateCoinAndDiamand();
        }
        else if(success && (exchangeId == 10400 ||
            exchangeId == 10500 || exchangeId == 10501 )) //speed auto hook
        {
            GetAutoOnHook()->startCD(0);
        }
        else if(success && exchangeId == 10800) //speed shop
        {
// 			int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
// 			BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
//             if (layer == NULL)
//             {
//                 return;
//             }
// 
//             ShopUILayer* shop = layer->getpShopUILayer();
//             if (shop == NULL)
//             {
//                 return;
//             }
// 
//             shop->freshShopList();

            SceneLayer* sceneLayer = GameManager::Get()->GetSceneLayer();
            if (sceneLayer != NULL)
            {
                MainMenuLayer* layer = sceneLayer->getMainMenuLayer();
                if (layer != NULL)
                {
                    MarketLayer* marketLayer = dynamic_cast<MarketLayer*>(layer->getMarketLayer());
                    if (marketLayer != NULL)
                    {
                        MysteryLayer* mysteryLayer = dynamic_cast<MysteryLayer*>(marketLayer->getCurrentLayer());
                        if (mysteryLayer)
                        {
                            mysteryLayer->TryRefreshList();
                        }
                    }
                }
            }

        } 
		else if(exchangeId >= 20000 && exchangeId <=20004)
		{
			
			MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"))->onExchangeParameterRsp(success);
		}
		else if(exchangeId >= 20010 && exchangeId <=20014)
		{
			
			MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"))->onExchangeParameterRsp(success);
		}
		else if (exchangeId == 20201 || exchangeId == 20100 || exchangeId == 20200)	// Note: ÿ��������ضһ�Э��?
		{			
			LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "ExchangeDailyTaskByDiamond",exchangeId);
		}
		else if(exchangeId == 20310)
		{
			AlchemyUILayer* layer = MainMenuLayer::getUILayerByTag<AlchemyUILayer>(MainMenuLayer::getLayerTagByName("AlchemyUILayer"));
			if(layer)
			{
				layer->onReceivedTopSearch();
			}
		}
        else if (exchangeId == 20520 && success == true)
        {
			std::string str =  Localizatioin::getLocalization("M_REGET_SUCCESS_TEXT");
			ShowMessage(str.c_str());

			NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_AUTO_BATTLE_TIMES,this);

			/*
			InstanceListLayer * instanceListLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));

            if (instanceListLayer)
            {
                NormalInstanceInfoLayer* infoLayer = dynamic_cast<NormalInstanceInfoLayer*>(instanceListLayer->getInstanceInfoLayer());

                if (infoLayer != NULL)
                {
                    infoLayer->RefreshTimes();
                }
            }
			*/

			/*
			AutoOnHook* autoOnHook = GetAutoOnHook();
			if (autoOnHook)
			{
				autoOnHook->RefreshTimesAndStamina();
			}
			*/

        } else if ((exchangeId == 31000 || exchangeId == 31001) && success == true)
        {
            MarketManager::Get()->ShowShopSuccess();
        }
        else if(exchangeId == 31010 && success == true)
        {
            // treasure explore, 31010为探宝器           
        }
        else if (exchangeId == 31012 && success == true)
        {
            ShowMessage(Localizatioin::getLocalization("M_FINDSPIRIT_SPIRITPROMOTE_SUCCESS"));
            CCNotificationCenter::sharedNotificationCenter()->postNotification("treasure_upgrade_success");
        }
        else if(exchangeId == 31013 && success == true)
        {
            ShowMessage(Localizatioin::getLocalization("M_SHOP_RESET"));            
        }
	}
}

/// <summary>
//	Task related
/// </summary>
void MessageHandler::handleRsqTaskInfoMessage()
{
	CSTaskInfoRsp* msg = dynamic_cast<CSTaskInfoRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		TaskManager::getInstance()->TaskManager::ClearTaskData();

		int acceptable_tasks_count = msg->acceptable_tasks_size();
		int accept_tasks = msg->accept_tasks_size();
		// Note: �ɽ����񲿷�
		for(int i=0;i<acceptable_tasks_count;i++)
		{
			unsigned int acceptable_task_id = msg->acceptable_tasks(i);
			TaskManager::getInstance()->AddOneAcceptableTask(acceptable_task_id);
		}
		// Note: ��������??
		for(int i=0;i<accept_tasks;i++)
		{
			TaskInfo taskInfo = msg->accept_tasks(i);

			unsigned int task_id = taskInfo.task_id();
			unsigned int step_id = taskInfo.step_id();
			unsigned int step_value = taskInfo.step_value();

			tTaskStepData taskData;
			taskData.t_step = step_id;
			taskData.t_step_value = step_value;
			TaskManager::getInstance()->AddOneAcceptedTask(task_id,taskData);
		}
		
		TaskManager::getInstance()->UpdateTaskData();
		TaskManager::getInstance()->UpdateUI();
		
		TaskManager::getInstance()->RqsTaskLogFromServer();

		unsigned int firstLevelId = TaskManager::getInstance()->OnlyGetFirstLevelTaskID();
		if (false == TutorialsManager::Get()->IsOneTaskWithOneTutorial((int)(firstLevelId))
			&& false == TutorialsManager::Get()->IsLeadTaskWithOneTutorial(firstLevelId)
			&& false == StoryInstanceDirector::Get()->IsStart())
		{
			eTaskGlobleState taskState = TaskManager::getInstance()->getTaskState();
			if (taskState == E_ACCEPT_TASK)
			{
				TaskManager::getInstance()->DisplayOneNpcChatLayer(TaskManager::getInstance()->getDialogNpcId());
				TaskManager::getInstance()->setTaskState(E_TASK_STATE_UNKNOW);
			}
			else if (taskState == E_OVER_TASK)
			{
				TaskManager::getInstance()->DisplayOneNpcChatLayer(TaskManager::getInstance()->getDialogNpcId());
				TaskManager::getInstance()->setTaskState(E_TASK_STATE_UNKNOW);
			}
		}
	}

	UIControlAppearControler::Get()->SetTaskMessageFromServerOrNot(true);
	UIControlAppearControler::Get()->SendTaskStateQueMessage();
}

void MessageHandler::handleRsqNetStateMessage()
{
	CSPingRsp* msg = dynamic_cast<CSPingRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int serTime = msg->time();
		//unsigned int serUTime = msg->utime();
		//CCLOG("==================Server Time: %d.%d=============", serTime, serUTime);
		TimeManager::Get()->syncServerTime(serTime);       
		NetStateChecker::Get()->RecieveRqsFromServer();
        NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SERVER_TIME_SYNCED,this);
	}
}

void MessageHandler::handleRsqTaskLogMessage()
{
	CSTaskLogRsp* msg = dynamic_cast<CSTaskLogRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		std::vector<unsigned int> vecTaskIds;
		int tasks_count = msg->tasks_size();
		for(int i=0;i<tasks_count;i++)
		{
			TaskInfo task_info = msg->tasks(i);
			unsigned int task_id = task_info.task_id();
			vecTaskIds.push_back(task_id);
		}		
		TaskManager::getInstance()->ShowRewardTaskMessage(vecTaskIds);
	}
}

void MessageHandler::handleRsqTaskStepMessage()
{
	CSTaskDoStepRsp* msg = dynamic_cast<CSTaskDoStepRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int task_id = msg->task_id();
		unsigned int step_id = msg->step_id();
		unsigned int step_value = msg->step_value();

		bool bSendTaskReq = true;

		if (step_id == 1)
		{
			//// TODO:��ʾ�����Ѿ���ȡ����??
			CCLOG("One Task Accepted %d ",task_id);
			TaskManager::getInstance()->setTaskState(E_ACCEPT_TASK);

			TutorialsManager::Get()->StartOneTutorial(task_id,kTutorialTask,false);
			if (TutorialsManager::Get()->IsOneTaskWithOneTutorial(task_id))
			{
				TaskManager::getInstance()->SetOneTaskStepToServer(task_id,2,1);
				WaitingLayerManager::Get()->PushOneMessage("CSTaskDoStepReq");
			}
			//jackniu 2014-5-2. not use popup function
			//PopupRewardManager::Get()->ShowOneLayerByTaskState(task_id,1);
			StoryInstanceDirector::Get()->Begin(task_id,0,1,true);

			if (false == GameDataManager::Get()->IsEliteTranspointValid() && 
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Task/TaskState.lua", "IsOneTaskElite", (task_id)))
			{
				GameDataManager::Get()->TrackEliteValid();
				SpriteTransPoint* tranpoint = GameManager::Get()->GetSceneLayer()->getLevelLayer()->getEliteTranspoint();
				if (tranpoint)
				{
					tranpoint->setAppearence(true);
				}
			}
		}
		else
		{
			// Note: �ж������Ƿ����?
			unsigned int totalStep = TaskConfigFromLuaManager::getInstance()->GetOneTaskStepNums(task_id);
			if (step_id >= totalStep)
			{
				// TODO:������ɵ���??
				CCLOG("One Task Compelte %d ",task_id);
				// Note: �����õ��Ľ�??
				unsigned int elfId = TaskConfigFromLuaManager::getInstance()->GetOneTaskRewardElfID(task_id);
				if (elfId != 0)
				{
					// Note: New State
					//SpriteElfDataCenter::Get()->SendSetElfStateMessage(elfId,2);
				}
				TaskManager::getInstance()->setTaskState(E_OVER_TASK);
				//jackniu 2014-5-2. not use popup function
				//PopupRewardManager::Get()->ShowOneLayerByTaskState(task_id,2);
				StoryInstanceDirector::Get()->Begin(task_id,0,3,true);
				UIControlAppearControler::Get()->TackTaskOverEvent(task_id);
				MainLandManager::Get()->onEventTaskUnlock(task_id);
			}
		}

		// Note: ˢ������״??
		TaskManager::getInstance()->RqsTaskFromServer();
	}
}

void MessageHandler::handleRsqTaskStatusMessage()
{
	CSTaskStatusRsp* msg = dynamic_cast<CSTaskStatusRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		int taskId = msg->task_id();
		PB_TASK_STATUS_T taskStatus = msg->status();
		if (taskStatus == PB_TASK_FINISH)
		{
			MainLandManager::Get()->onEventTaskUnlock(taskId);			
		}		

		SystemConditionManager::Get()->TackTaskStatus(taskId,taskStatus);
		UIControlAppearControler::Get()->TackTaskStatus(taskId,taskStatus);
	}
}

void MessageHandler::handleMsgUserJoin()
{
	CSBattleUserJoinRsp* msg = dynamic_cast<CSBattleUserJoinRsp*>(mCurNormMsgPackage->msg);	

	USERINFO mUserData = UserData::GetUserInfo();

	int userCount = msg->userinfo_size();
	for (int i = 0; i < userCount; i++)
	{
		BasicUserInfo info = msg->userinfo(i);
		PLAYER_ID playerId;
		playerId.setPlayId(info.uid(),info.reg_time(),info.channel_id());
		if(playerId != mUserData.id)
		{
			// Note: ����������ҵĶ�����??
			SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(info.type(),true);

			//unsigned int uid = info.uid();
			std::string nick = info.name();
			int xPos = info.xpos();
			int yPos = info.ypos();
			int orient = info.orient();
			int type = info.type();

			CCAssert(xPos != 0 || yPos != 0, "Player pos(0, 0)");

			unsigned int side = 0;
			if(info.has_battle_side())
				side = info.battle_side();

			USERINFO* pUserInfo = UserData::Get()->GetUserInfo(playerId);
			if(pUserInfo)
			{
				pUserInfo->Set(info);
				UserData::Get()->SetUserInfo(pUserInfo->id, *pUserInfo);
			}
			else
			{
				USERINFO otherUserInfo;	
				otherUserInfo.Set(info);
				UserData::Get()->SetUserInfo(otherUserInfo.id, otherUserInfo);
				
			}
			

			CCPoint pixPos = LevelManager::sShareInstance()->pointCellToPixel(ccp(xPos, yPos));

			LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
			if (layer)
			{
				layer->addOtherPlayer(playerId, nick.c_str(), type, pixPos, orient, side);
				SpriteSeer * seer = layer->getPlayer(playerId);
				if (seer)
				{
					if (info.red_name())
					{
						seer->ShowNameLabelColor(true);
					}
					else
					{
						seer->ShowNameLabelColor(false);
					}
				}

				bool isPVP = LevelManager::sShareInstance()->isCurrentPVPLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel();
				bool isPVAI = LevelManager::sShareInstance()->isCurrentPVAILevel();
				bool isBestFighter = LevelManager::sShareInstance()->isCurrentBestFighterLevel();

				if (isPVP || isPVAI || isBestFighter)
				{
					PvAIManager::Get()->setAIPlayerId(playerId);
				}

				bool isGuildInstance = LevelManager::sShareInstance()->isCurrentGuildInstanceLevel();
				if (isGuildInstance)
				{
					GuildInstanceManager::Get()->setAIPlayerId(playerId);
				}
			}			
		}
	}
}

void MessageHandler::handleMsgUserLeave()
{
	CSBattleUserLeaveRsp* msg = dynamic_cast<CSBattleUserLeaveRsp*>(mCurNormMsgPackage->msg);	

	USERINFO mUserData = UserData::Get()->GetUserInfo();	
	PLAYER_ID playId;
	playId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());
	if(playId != mUserData.id)
	{
		LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			layer->removeOtherPlayer(playId);
			CCLOG("player leave battle: %d ", msg->userid());
		}			
		UserData::Get()->removeOtherUser(playId);
	}	
}

void MessageHandler::handleMsgBattleMove()
{
	if (XLogicManager::sharedManager()->getMessageStatus() != XLogicManager::E_LOAD_COMPLETE)
	{//ignore BattleMove message before LOAD_COMPLETE status, try fix Bug#96436
		return;
	}

	CSBattleMoveRsp* msg = dynamic_cast<CSBattleMoveRsp*>(mCurNormMsgPackage->msg);	

	uint32_t uid = msg->id();
	uint32_t regTime = msg->reg_time();
	uint32_t channel = msg->channel_id();
	PLAYER_ID playId;
	playId.setPlayId(uid,regTime,channel);
	CSPoint directPos = msg->direct();
	CSPoint pos = msg->pos();
	float timeSend = msg->time() / 1000.0f;

	static Machtimer machTimer;

	CCPoint direct = ccp(directPos.xpos(), directPos.ypos());	
	CCPoint position = LevelManager::sShareInstance()->pointCellToPixel(ccp(pos.xpos(), pos.ypos()));
	
	LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	if (layer)
	{
		bool isAiPlayer = false;
		if(LevelManager::sShareInstance()->isCurrentGuildInstanceLevel()){
			if(GuildInstanceManager::Get()->getAIPlayerId() == playId){
				isAiPlayer = true;
			}
		}


		PLAYER_ID userId = UserData::Get()->getUserId();
		if (userId != playId && !isAiPlayer)
		{
			SpriteSeer * seer = layer->getPlayer(playId);

			if (seer)
			{
				CCPoint nowPos = seer->getPosition();
				seer->SetMoveType(MOVE_BY_MOUSE);
				seer->setCanMove(true);
				seer->setDirection(direct);

				///calculate the time spent
				float distance = ccpDistance(position, seer->getLastPosition());
				float curDistance = ccpDistance(position, seer->getPosition());
				
				float d = curDistance - distance;
				float rate = 0.0f;


				if(curDistance > 0.2f * seer->GetMoveRate())
				{
					seer->SetAcceleratiion(360.0f);
					
				}
				else
				{
					seer->SetAcceleratiion(0.0f);
					seer->SetAdditionMoveRate(0.0f);
				}
	

				
				seer->setLastPosition(position);


				seer->moveToTarget(position, seer->getPosition(), NULL, NULL, rate);
				

				// CCLOG("handleMsgBattleMove additionRate : %f , distance : %f : time : %f", rate, ccpDistance(seer->getPosition(), seer->getLastPosition()), machTimer.elapsedMS() / 1000.0f);
				machTimer.start();

			}
		}

	}
}

void MessageHandler::handlePlayerAttack()
{
}

void MessageHandler::handleSkillBegin()
{
	CSSkillBeginRsp* msg = dynamic_cast<CSSkillBeginRsp*>(mCurNormMsgPackage->msg);	

	USERINFO mUserData = UserData::Get()->GetUserInfo();	

	PLAYER_ID playId;
	playId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());
	if(playId != mUserData.id )
	{
		LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			CCPoint rolePos(msg->pos().xpos(), msg->pos().ypos());
			rolePos = LevelManager::sShareInstance()->pointCellToPixel(rolePos);

			CCPoint roleDir(msg->start_dir_x(), msg->start_dir_y());

			CCPoint monsterPos(msg->target_pos().xpos(), msg->target_pos().ypos());
			monsterPos = LevelManager::sShareInstance()->pointCellToPixel(monsterPos);

			CCPoint aimDir(msg->direct_x(), msg->direct_y());
			bool bAimMonster = msg->aim();
			unsigned int pos = msg->entity_pos();
			if(SkillDispMgr::Get()->GetSkillInfo(msg->action_id()))
				layer->DoOtherPlayerSkill(playId, msg->action_id(), rolePos, roleDir, aimDir, monsterPos,  bAimMonster);
			else
				layer->DoElfSkill(playId, msg->action_id(), rolePos, roleDir, aimDir, monsterPos,  bAimMonster ,pos);

			//static Machtimer ttime;
			//CCLOG("CSSkillBeginRsp: %d, %ld els : %ld", msg->userid(), msg->action_id(), ttime.elapsedMS());
			//ttime.start();
			
		}			
	}	
}

void MessageHandler::handleRsqElfListMessage()
{
	CSMonListRsp* msg = dynamic_cast<CSMonListRsp*>(mCurNormMsgPackage->msg);
	
	if(msg)
	{
		PLAYER_ID playerId;
		playerId.setPlayId(msg->uid(),msg->reg_time(),msg->channel_id());
		CCLOG("handleRsqElfListMessage recieve %d ,%d",msg->uid(),msg->reg_time());
		//unsigned int roleId = msg->uid();
		//std::string roleName = msg->name();
		int elf_count = msg->mon_info_size();
		
		int assit_num = 0 ;
		vector<MonInfo> battle_spriteElf_vec ;
		if( !MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId()))
		{	
			if(SpriteElfManager::Get()->IsHasElfPlay(playerId))
			return ;
		
		}
		battle_spriteElf_vec.clear() ;
		SpriteFactory::sharedFactory()->RemoveElfFromHero(playerId);
		SpriteFactory::sharedFactory()->RemovePosElfFromHero(playerId);
		
		for(int i=0;i<elf_count;i++)
		{
			MonInfo monInfo = msg->mon_info(i);

			unsigned int mon_id = monInfo.mon_id();
			unsigned int mon_state = monInfo.mon_status();
			unsigned int pos = monInfo.pos() ;
			if(mon_state == 3)
			{
				if(pos == 1)
					SpriteElfManager::Get()->SetPlayerElfPos(playerId,mon_id ,SpriteElfManager::E_Fight_Left);
				else if(pos == 2 )
					SpriteElfManager::Get()->SetPlayerElfPos(playerId,mon_id ,SpriteElfManager::E_Fight_Right);
				else
					continue ;
				
				battle_spriteElf_vec.push_back(monInfo);
			}
			else if(mon_state == 4 )
			{
				if(assit_num >= 2 )continue ;
				if( pos == 20 )
					SpriteElfManager::Get()->SetPlayerElfPos(playerId,mon_id ,SpriteElfManager::E_Assist_Pos_1);
				else if(pos == 21 )
					SpriteElfManager::Get()->SetPlayerElfPos(playerId,mon_id ,SpriteElfManager::E_Assist_Pos_2);
				assit_num ++ ;
			}
			else
				continue ;

			// Note: 预加载声音文件
			bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
			if (bMainLandLevel == false)
			{
				unsigned int skillId = SkillDispMgr::Get()->GetFairySkillID(mon_id);
				const FAIRY_SKILL_INFO* pFairySkillInfo = SkillDispMgr::Get()->GetFairySkillInfo(skillId);
				if (pFairySkillInfo)
				{
					GameAudioManager::sharedManager()->preLoadEffect(pFairySkillInfo->soundID);
					GameAudioManager::sharedManager()->preLoadEffect(pFairySkillInfo->effectSoundID);
				}
			}
		}

		for(int i = 0 ; i<battle_spriteElf_vec.size() ;i++)
		{
			MonInfo monInfo = battle_spriteElf_vec.at(i);

			unsigned int mon_id = monInfo.mon_id();
			unsigned int mon_state = monInfo.mon_status();
			unsigned int pos = monInfo.pos() ;
            unsigned int grade = monInfo.grade();

			SpriteElfConfigFromLuaManager::getInstance()->PushElfListMessageToLua(playerId, mon_id, mon_state, grade);
		}
		if(assit_num >0)
			SpriteElfManager::Get()->CreateAssistElfs();
		SpriteElfConfigFromLuaManager::getInstance()->GetElfListMessageOver();
	}
}

void MessageHandler::handleRsqElfStateMessage()
{
	CSSetMonStatusRsp* msg = dynamic_cast<CSSetMonStatusRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int result = msg->ret();
		if (result == 0)	//�ɹ�
		{
			unsigned int mon_id = msg->mon_id();
			unsigned int mon_state = msg->mon_status();

			SpriteElfConfigFromLuaManager::getInstance()->PushElfStateMessageToLua(mon_id,mon_state);
		}
	}
}

void MessageHandler::handleLevelUpRsp()
{
	CSLevelUpRsp* msg = dynamic_cast<CSLevelUpRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int newLevel = msg->level();
		USERINFO info = UserData::GetUserInfo();
		info.level = newLevel;
		UserData::SetUserInfo(info);
		NotificationCenter::defaultCenter()->broadcast(GM_ATTR_HERO_LEVEL_UPDATE,this);

		TaskManager::getInstance()->RqsTaskFromServer();
		UIControlAppearControler::Get()->TackHeroLevelEvent();
		//jackniu 2014-5-2. not use popup function
		//PopupRewardManager::Get()->ShowOneLayerByHeroLevel(newLevel);

		int mapID = LevelManager::sShareInstance()->getCurMapId();
		//if(MainLandManager::Get()->isMainLand( mapID ))
		{
			SpriteSeer* hero = GameManager::Get()->getHero();
			if (hero)
			{
				SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(120);
				ParticleManager::Get()->playOneTimeEffect(120, hero->getPosition(), 0,100000);
				GameAudioManager::sharedManager()->playEffect(320000,false);
			}			
		}
	}
}

void MessageHandler::handleUserInfoRsp()
{
	CSGetUserInfoRsp* msg = dynamic_cast<CSGetUserInfoRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		PLAYER_ID playerId;
		playerId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());
		if(playerId == UserData::getUserId())
		{
			BasicUserInfo userInfo = msg->userinfo();
			USERINFO info = UserData::GetUserInfo();
			info.level = userInfo.lv();
			info.nextExp = userInfo.next_lv();
			info.guildName = userInfo.gname();
			UserData::SetUserInfo(info);
			//UserData::SetBasicUserInfo(userInfo);
			MessageFilter::Get()->setServerBasicUserInfo(userInfo);

			if(info.current_title != userInfo.current_title())
			{
				info.current_title = userInfo.current_title();
				UserData::SetUserInfo(info);
				NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_USER_EQUIP_TITLE,this);
			}
			if(info.m_guildId.getHighID() != userInfo.gid_high() || info.m_guildId.getLowID() != userInfo.gid_low())
			{
				info.m_guildId.setGuildId(userInfo.gid_high(),userInfo.gid_low());
				UserData::SetUserInfo(info);
				NotificationCenter::defaultCenter()->broadcast(GM_CHANGE_USER_GUILD,this);
			}
			if(userInfo.has_battle_info())
			{
				BasicUserBattleInfo battleInfo = userInfo.battle_info();
				if(battleInfo.physical_attack() != info.physical_attack)
				{
					info.physical_attack = battleInfo.physical_attack();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PHYSICAL_ATTACK_UPDATE,this);
				}
				if(battleInfo.magic_attack() != info.magic_attack)
				{
					info.magic_attack = battleInfo.magic_attack();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_MAGIC_ATTACK_UPDATE,this);
				}
				if(battleInfo.skill_attack() != info.skill_attack)
				{
					info.skill_attack = battleInfo.skill_attack();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SKILL_ATTACK_UPDATE,this);
				}
				if(battleInfo.physical_defence() != info.physical_defence)
				{
					info.physical_defence = battleInfo.physical_defence();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PHYSICAL_DEFENCE_UPDATE,this);
				}
				if(battleInfo.magic_defence() != info.magic_defence)
				{
					info.magic_defence = battleInfo.magic_defence();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_MAGIC_DEFENCE_UPDATE,this);
				}
				if(battleInfo.skill_defence() != info.skill_defence)
				{
					info.skill_defence = battleInfo.skill_defence();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SKILL_DEFENCE_UPDATE,this);
				}
				if(battleInfo.health_point() != info.total_hp)
				{
					info.total_hp = battleInfo.health_point();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_HEALTH_POINT_UPDATE,this);
				}
				if(battleInfo.accurate() != info.accurate)
				{
					info.accurate = battleInfo.accurate();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_ACCURATE_UPDATE,this);
				}
				if(battleInfo.dodge() != info.dodge)
				{
					info.dodge = battleInfo.dodge();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_DODGE_UPDATE,this);
				}
				if(battleInfo.wreck() != info.wreck)
				{
					info.wreck = battleInfo.wreck();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_WRECK_UPDATE,this);
				}
				if(battleInfo.parry() != info.parry)
				{
					info.parry = battleInfo.parry();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PARRY_UPDATE,this);
				}
				if(battleInfo.critical_strike() != info.critical_strike)
				{
					info.critical_strike = battleInfo.critical_strike();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_CRITICAL_STRIKE_UPDATE,this);
				}
				if(battleInfo.tenacity() != info.tenacity)
				{
					info.tenacity = battleInfo.tenacity();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_TENACITY_UPDATE,this);
				}
				if(battleInfo.slay() != info.slay)
				{
					info.slay = battleInfo.slay();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SLAY_UPDATE,this);
				}
				if(battleInfo.speed() != info.speed)
				{
					info.speed = battleInfo.speed();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SPEED_UPDATE,this);
				}
				if(battleInfo.proficiency() != info.proficiency)
				{
					info.proficiency = battleInfo.proficiency();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PROFICIENCY_UPDATE,this);
				}
				if(battleInfo.courage() != info.courage)
				{
					info.courage = battleInfo.courage();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_COURAGE_UPDATE,this);
				}
				if(battleInfo.charm() != info.charm)
				{
					info.charm = battleInfo.charm();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_CHARM_UPDATE,this);
				}
				if(battleInfo.trick() != info.trick)
				{
					info.trick = battleInfo.trick();
					UserData::SetUserInfo(info);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_TRICK_UPDATE,this);
				}
			}
		}
		else
		{
			USERINFO otherUserInfo;	
			otherUserInfo.Set( msg->userinfo());
			UserData::Get()->SetUserInfo(otherUserInfo.id,otherUserInfo);
		}
	}
}

void MessageHandler::handleAttributesRsp()
{
	CSAttributesRsp* msg = dynamic_cast<CSAttributesRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		bool slot_set=false;
		unsigned int count = msg->values_size();
		for(unsigned int i=0;i< count; ++i)
		{
			CSAttributesRsp_KeyValue attribute = msg->values(i);
			unsigned int key = attribute.key();
			unsigned int value = attribute.value();
            
			bool bKeyInDailyTaskRange = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyGoals/dailyGoalsDataCenter.lua", "IsOneKeyContainsDailyGoalsId",key,value);
			bool bKeyInDailyRewardRange = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyReward/dailyRewardMainLayerDataCenter.lua", "IsOneKeyContainsDailyRewardId",key,value);
            
            if(key >= PB_ATTR_PPVE_TIMES_DAILY_BEGIN && key <= PB_ATTR_PPVE_TIMES_DAILY_END)
            {
                long enterTimes = attribute.value();
                if(enterTimes >= 1)
                {
					ShowMessage(Localizatioin::getLocalization("M_PPVE_NO_TIME"));
                    return;
                }
                else
                {
                    /// call global script function
                    CCLuaEngine::defaultEngine()->executeGlobalFunction("gotoNewRoom");
                }
                //break;
            }

			if(key >= PB_ATTR_EXCHANGE_TIMES_BEGIN && key <= PB_ATTR_EXCHANGE_TIMES_END)
            {
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "UpdateExchangeTimes", key, attribute.value());
            }

            
			switch(key)
			{
                case PB_ATTR_DOWER_LVUP_CD:
                {
                    break;
                }
				case PB_ATTR_EQUIP_LVUP_CD:
                {
					//long nowTime = TimeManager::Get()->getCurServerTime();
					if (TimeManager::Get()->hasTimer(PB_ATTR_EQUIP_LVUP_CD))
					{
						TimeManager::Get()->renewTimer(PB_ATTR_EQUIP_LVUP_CD, attribute.value());
					}
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_equipLvUpCd)
					{
						info.m_equipLvUpCd = attribute.value();
						UserData::SetUserInfo(info);
					}
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_EQUIP_LVUP_CD,this);
					break;	
                }
                case PB_ATTR_AB_NORMAL_MAP_CD:
                {
                   
                  
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_autofightNormalCd)
					{
						info.m_autofightNormalCd= attribute.value();
						UserData::SetUserInfo(info);
					}
                    
                    NotificationCenter::defaultCenter()->broadcast(GM_ATTR_AUTO_FIGHT_CD,this);

					break;	
                }
                case PB_ATTR_AB_SPECIAL_MAP_CD:
                {
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_autofightJingYingCd)
					{
						info.m_autofightJingYingCd= attribute.value();
						UserData::SetUserInfo(info);
					}
                    NotificationCenter::defaultCenter()->broadcast(GM_ATTR_AUTO_FIGHT_CD,this);

					break;	
                }

                case PB_ATTR_BUY_DIAMOND_ACCU :
                {
                    USERINFO info = UserData::GetUserInfo();
                    if(attribute.value() != info.m_diamond_acc)
                    {
                        info.m_diamond_acc= attribute.value();
                        UserData::SetUserInfo(info);

                        NotificationCenter::defaultCenter()->broadcast(GM_ATTR_DIAMOND_ACCU, this);
                    }

                    break;	
                }

				case PB_ATTR_AB_UGCITY_MAP_CD:
                {                   
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_autofightUnderCd)
					{
						info.m_autofightUnderCd= attribute.value();
						UserData::SetUserInfo(info);
					}
                    NotificationCenter::defaultCenter()->broadcast(GM_ATTR_AUTO_FIGHT_CD,this);

					break;	
                }

                case PB_ATTR_GET_SPEC_P_FAIRY_LEFT_TIMES :
                {
                    USERINFO info = UserData::GetUserInfo();
                    if(attribute.value() != info.m_spec_left_times)
                    {
                        info.m_spec_left_times = attribute.value();
                        UserData::SetUserInfo(info);
                    }

                    break;	
                }

                case PB_ATTR_PLAYER_EPLORE_TICKET :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_green_extract_times)
                        {
                            info.m_green_extract_times = attribute.value();
                            UserData::SetUserInfo(info);
                        }

                        NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SPRITE_EXTRACT,this);
                        break;
                    }

                case PB_ATTR_SHOP_CD:
                {
     //               long nowTime = TimeManager::Get()->getCurServerTime();
     //               int val = attribute.value()-nowTime;
     //               if(val <= 0)
     //               {
     //                   TimeManager::Get()->stopTimer(PB_ATTR_SHOP_CD);
     //                   val = 0;
     //               }
     //               
     //               //TimeManager::Get()->setshopFreshTime(val);
     //               
                    USERINFO info = UserData::GetUserInfo();
                    if(attribute.value() != info.m_shopRefreshTime)
                    {
                        info.m_shopRefreshTime = attribute.value();
                        UserData::SetUserInfo(info);
                    }

                    long nowTime = TimeManager::Get()->getCurServerTime();
                    int val = attribute.value() - nowTime;
                    if(val <= 0)
                    {
                        TimeManager::Get()->stopTimer(PB_ATTR_SHOP_CD);
                        val = 0;
                    }

					int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
					BackPackLayer* back = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
                    if(back)
                    {
                        ShopUILayer* shop = back->getpShopUILayer();
                        if(shop)
                            shop->initRemainTimer(attribute.value());
                    }
                    break;
                }

                case PB_ATTR_DAILY_ACC_REFRESH_SHOP_TIMES :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_refreshTimes)
                        {
                            info.m_refreshTimes = attribute.value();
                            UserData::SetUserInfo(info);
                        }

                        NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_SHOP_REFRESH_TIMES,this);

                        break;
                    }

                case PB_ATTR_RECYCLE_BIN_START_POS:
                {
                    int val = attribute.value();
                    break;
                }
				case PB_ATTR_PLAYER_COIN:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_gold)
					{
						info.m_gold = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_COIN_UPDATE,this);
					}
					break;
				}
                case PB_ATTR_PLAYER_VIP_LV:
                {
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_vip)
					{
						info.m_vip = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_VIP_LEVEL_CHANGE,this);
					}
					break;
				}    
				case PB_ATTR_PLAYER_SP:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_spriteEnergy)
					{
						info.m_spriteEnergy = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SP_UPDATE,this);
					}
					break;
				}
				case PB_ATTR_PLAYER_DIAMOND:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_diamond)
					{
						info.m_diamond = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SP_DIAMOND_UPDATE,this);
					}
					break;
				}
				case PB_ATTR_BUY_BAG_CAPACITY:
				{
					unsigned int unlockbagCapacity = attribute.value();
					ItemManager::Get()->setBackPackUnlockPos(unlockbagCapacity);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_BUY_BAG_CAPACITY,this);					
					break;
				}
				case PB_ATTR_BUY_STORE_CAPACITY:
				{
					unsigned int unlockbagCapacity = attribute.value();
					ItemManager::Get()->setStorageUnlockPos(unlockbagCapacity);
					NotificationCenter::defaultCenter()->broadcast(GM_ATTR_BUY_STORE_CAPACITY,this);	
					break;	
				}
				case PB_ATTR_BUY_SS_BAG_CAPACITY:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_alchemyStorageUnlockPos)
					{
						if(attribute.value() != 0)
						{
							info.m_alchemyStorageUnlockPos = attribute.value();
							UserData::SetUserInfo(info);
							ItemManager::Get()->setAlchemyStorageUnlockCapacity(info.m_alchemyStorageUnlockPos);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_BUY_SS_BAG_CAPACITY,this);
						}
					}
					break;
				}
				case PB_ATTR_EXP:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.exp)
					{
						info.exp = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_EXP_UPDATE,this);
					}
					break;
				}
				case PB_ATTR_PLAYER_SS_CHIP:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_spriteChip || 0 == info.m_spriteChip )
					{
						info.m_spriteChip = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SP_CHIP_UPDATE,this);
					}
					break;
				}
				case PB_ATTR_PLAYER_SS_MOD_ID:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_searchSpriteNpc)
					{
						info.m_searchSpriteNpc = attribute.value();
						UserData::SetUserInfo(info);
					}
					break;
				}
				case PB_ATTR_PLAYER_GEM_ANIMA:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_playerGemAnima)
					{
						info.m_playerGemAnima = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAYER_GEM_ANIMA,this);
					}
					break;
				}
				case PB_ATTR_EQUIP_LVUP_FLAG:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_equipLvFlag)
					{
						info.m_equipLvFlag = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_EQUIP_LVUP_UESED_TIMES,this);
					}
					break;
				}
				/*case PB_ATTR_EQUIP_LVUP_MAX_TIMES:
				{
					USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_equipLvUpTotalTimes)
					{
						info.m_equipLvUpTotalTimes = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_EQUIP_LVUP_MAX_TIMES,this);
					}
					break;
				}*/
				case PB_ATTR_PLAYER_PRESTIGE:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_reputation)
						{
							info.m_reputation = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_REPUTATION_UPDATE,this);
						}
						break;
					}
				case PB_ATTR_PLAYER_STAMINA:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_stamina)
						{
							info.m_stamina = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_STAMINA_UPDATE,this);
						}
						break;
					}
				case PB_ATTR_PLAYER_RESET_GEM_TIMES:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_gemRecastTimes)
						{
							info.m_gemRecastTimes = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_GEM_RECAST,this);
						}
						break;
					}
				case PB_ATTR_EFFECTIVENESS:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.effectiveness)
						{
							info.effectiveness = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_ABILITY_ALL_UPDATE,this);
						}
						break;
					}
				case PB_ATTR_PLAYER_EXPLOIT:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_player_exploit)
						{
							info.m_player_exploit = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAYER_EXPOLIT,this);
						}
						break;
					}
				case PB_ATTR_PLAYER_SPEC_INSTANCE_BTL_TIME:
					{
						/*unsigned int battletime = attribute.value();
						InstanceListLayer * instanceListLayer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
						if(instanceListLayer/* && instanceListLayer->getShowInstance()*/
						//{
						//	instanceListLayer->showJingYingPlayTimes(battletime);
						//}*/
						break;
					}
				case PB_ATTR_PLAYER_EXPLORE_TIMES:
					{
						unsigned int leftNums = attribute.value();
						SpriteElfConfigFromLuaManager::getInstance()->PushExploreLeftNums(leftNums);
						break;
					}
				case PB_ATTR_DAILY_TIMES_EXPLORE :
					{
						unsigned int Nums = attribute.value();
						SpriteElfConfigFromLuaManager::getInstance()->PushExploreNums(Nums);
						break;
					}
				case PB_ATTR_SVC_TIME_ZONE :
					{
						unsigned int timeNum = attribute.value();
						int type  = timeNum>>16 ;
						int value = timeNum & 0xffff;
						if(type == 2)
							value = -value ;
						TimeManager::Get()->setTimeZone(value);
					    LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/dailyRewardStaminaLayer.lua", "setGetStaminaTime",  0);
					   break;
					}
				case PB_ATTR_GET_STAMINA_RESTORE_TIME :
					{
						unsigned int timeNum = attribute.value();
						 LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/dailyRewardStaminaLayer.lua", "setGetStaminaTime",  timeNum);
						 break;
					}
				case PB_ATTR_PLAYER_EXPLORE_EXP:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_playerExploerExperience)
						{
							info.m_playerExploerExperience = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAYER_EXPLOER_EXP,this);
						}
						break;
					}
				case PB_ATTR_PLAYER_REFINE_STONE:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_fragStone)
						{
							info.m_fragStone = attribute.value();
							UserData::SetUserInfo(info);
							NotificationCenter::defaultCenter()->broadcast(GM_ATTR_FRAG_STONE,this);
						}
						break;
					}
				case SKILL_SLOT_1:
					{
						unsigned int skillId = attribute.value();
						GameConfigFromLuaManager::Get()->PushSkillSlotInfoToLua(attribute.key(),skillId);
						SkillDispMgr::Get()->PushOneSlotInfoToTable(1,skillId);
						slot_set=true;
						break;
					}
				case SKILL_SLOT_2:
					{
						unsigned int skillId = attribute.value();
						GameConfigFromLuaManager::Get()->PushSkillSlotInfoToLua(attribute.key(),skillId);
						SkillDispMgr::Get()->PushOneSlotInfoToTable(2,skillId);
						slot_set=true;
						break;
					}
				case SKILL_SLOT_3:
					{
						unsigned int skillId = attribute.value();
						GameConfigFromLuaManager::Get()->PushSkillSlotInfoToLua(attribute.key(),skillId);
						SkillDispMgr::Get()->PushOneSlotInfoToTable(3,skillId);
						slot_set=true;
						break;
					}
				case SKILL_SLOT_4:
					{
						unsigned int skillId = attribute.value();
						GameConfigFromLuaManager::Get()->PushSkillSlotInfoToLua(attribute.key(),skillId);
						SkillDispMgr::Get()->PushOneSlotInfoToTable(4,skillId);
						slot_set=true;
						break;
					}
				case SKILL_SLOT_5:
					{
						unsigned int skillId = attribute.value();
						GameConfigFromLuaManager::Get()->PushSkillSlotInfoToLua(attribute.key(),skillId);
						SkillDispMgr::Get()->PushOneSlotInfoToTable(5,skillId);
						slot_set=true;
						break;
					}
				case NEW_SKILL_ID:
					{
						unsigned int skillId = attribute.value();
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Skill/SkillDataCenter.lua", "SetNewSkillId", skillId);
						NotificationCenter::defaultCenter()->broadcast(GM_GET_NEW_SKILL,this);
						break;
					}
				case PB_ATTR_PLAYER_PVAI_TIMES:
					{
						PvAIManager::Get()->setRemainCount(attribute.value());
						PvAIManager::Get()->refreshPvAILayer();

						//ʣ���������?ʱ��ʾpvai btn�ĸ�̾����ʾ
						UILayout * layout = UIManager::sharedManager()->getUILayout("MainMenu");	
						if (layout)
						{
							UIButton * pvaiBtn = layout->FindChildObjectByName<UIButton>("button_communicate");
							if (pvaiBtn)
							{
                                int roleRank = PvAIManager::Get()->getHeroRank();
								if (attribute.value() > 0 && roleRank != 1)
								{
									pvaiBtn->showTipIcon();							
								}
								else
								{
									pvaiBtn->hideTipIcon();
								}								
							}
						}						
						break;
					}
				case PB_ATTR_PVAI_BUY_TIMES:
					{
						PvAIManager::Get()->setBuyCount(attribute.value());											
						break;
					}
                    
                case PB_ATTR_PLAYER_ENTER_MANOR_TIMES:
                {
                    USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_enterManorTimes)
					{
						info.m_enterManorTimes = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_COIN_UPDATE,this);
					}
                    break;
                }
                    
                case PB_ATTR_MANOR_BUY_TIMES:
                {
                    USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_buyManorTimes)
					{
						info.m_buyManorTimes = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_COIN_UPDATE,this);
					}
                    break;
                }
				case PB_ATTR_PVAI_CD:
					{
						long endTime = attribute.value();
						if (TimeManager::Get()->hasTimer(TIMER_PVAI_COOLDOWN))
						{
							TimeManager::Get()->renewTimer(TIMER_PVAI_COOLDOWN, endTime);
							PvAIManager::Get()->setCoolDownEndTime(endTime);
						}
						else
						{
							PvAIManager::Get()->setCoolDownEndTime(endTime);					
						}
						break;
					}
				case PB_ATTR_PLAYER_LAST_RESTORE_POWER_TIME:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_lastStaminaRestoreTime)
						{
							info.m_lastStaminaRestoreTime = attribute.value();
							UserData::SetUserInfo(info);
						}
						break;
					}
				case PB_ATTR_CONSTEL_1_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(1,attribute.value());
					break;
				case PB_ATTR_CONSTEL_2_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(2,attribute.value());
					break;
				case PB_ATTR_CONSTEL_3_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(3,attribute.value());
					break;
				case PB_ATTR_CONSTEL_4_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(4,attribute.value());
					break;
				case PB_ATTR_CONSTEL_5_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(5,attribute.value());
					break;
				case PB_ATTR_CONSTEL_6_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(6,attribute.value());
					break;
				case PB_ATTR_CONSTEL_7_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(7,attribute.value());
					break;
				case PB_ATTR_CONSTEL_8_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(8,attribute.value());
					break;
				case PB_ATTR_CONSTEL_9_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(9,attribute.value());
					break;
				case PB_ATTR_CONSTEL_10_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(10,attribute.value());
					break;
				case PB_ATTR_CONSTEL_11_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(11,attribute.value());
					break;
				case PB_ATTR_CONSTEL_12_CAPACIT:
					ItemManager::Get()->setConstellationCardNum(12,attribute.value());
					break;
                    
				case PB_ATTR_DAILY_TIMES_DAILY_TASK:
					{
						int nums = attribute.value();
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "UpdateDailyTaskComplteNums",nums);
					}
					break;
				case PB_ATTR_GET_DAILY_GOALS_REWARD_TIMES:
					{
						int nums = attribute.value();						
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyGoals/dailyGoalsDataCenter.lua", "DailyGoalsGetRewardTimesChangeEvent",nums);
					}
					break;
				case PB_ATTR_DAILY_GOALS_ACTIVITY:
					{
						int nums = attribute.value();
						if (nums > 100)
						{
							nums = 100;
						}
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyGoals/dailyGoalsDataCenter.lua", "DailyGoalsActiveNumChangeEvent",nums);
					}
					break;
				case 	PB_ATTR_DAILY_LOGIN_COUNT :
					{
						int dialyLoginNums = attribute.value();
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "setLoginNum",dialyLoginNums);
					}
					break;

					/*  case PB_ATTR_CONTINUE_LOGIN_TIMES:
					{
					int loginNums = attribute.value();
					loginNums = loginNums % 7;
					if (loginNums == 0)
					{
					loginNums = 7;
					}
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyLogin/dailyLoginLayerDataCenter.lua", "DailyLoginTimesChangeEvent",loginNums);
					}
					break;*/
				// Note: �ϴ���ȡÿ��vip����ʱ��
				case PB_ATTR_GET_DAILY_VIP_REWARD_TIME:
				{
					int nVipRewardGetFlag = attribute.value();
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyLogin/dailyLoginLayerDataCenter.lua", "DailyLoginVipRewardGetStateChangeEvent",nVipRewardGetFlag);
				}
					break;
				// Note: ÿ�յ�½������ȡʱ��
				case PB_ATTR_GET_DAILY_LOGIN_REWARD_TIME:
				{
					int nLoginRewardGetFlag = attribute.value();
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyLogin/dailyLoginLayerDataCenter.lua", "DailyLoginLoginRewardGetStateChangeEvent",nLoginRewardGetFlag);					
				}
					break;
                case PB_ATTR_PLAYER_HOLD_EXP:
                {
                    USERINFO info = UserData::GetUserInfo();
					if(attribute.value() != info.m_palyHoldExp)
					{
						info.m_palyHoldExp = attribute.value();
						UserData::SetUserInfo(info);
						NotificationCenter::defaultCenter()->broadcast(GM_ATTR_COIN_UPDATE,this);
					}
                }
                    break;
				//���³Ǳ�??
				case PB_ATTR_UGCITY_BOXES_STATUS:
                {
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/UnderBoxLayer.lua", "UpdateUnderBoxGetStatus", attribute.value());
                }
                    break;
				case PB_ATTR_UGCITY_BOXES_RESET_TIMES:
                {
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/UnderBoxLayer.lua", "UpdateUnderBoxResetNum", attribute.value());
                }
                    break;
				case PB_ATTR_UGCITY_BOXES_OPEN_NUM:
                {
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/UnderBoxLayer.lua", "UpdateUnderBoxOpenNum", attribute.value());
                }
                    break;

				//���Ƿ���
				case PB_ATTR_PLAYER_CDEF_TIMES:
                {
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/CityDefendRoomListLayer.lua", "UpdateCityDefendRemainCount", attribute.value());
                }
                    break;
				case PB_ATTR_MXA_VIEW_PLAYER:
				{//number of players at the same mainland
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameSetting.lua", "setNumberOfPLayersSettingValue", attribute.value());
				}
					break;
				case PB_ATTR_GAME_PIC_QUALITY:
				{//quality setting
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameSetting.lua", "setQualitySettingValue", attribute.value());
				}
					break;
				case PB_ATTR_PLAYER_PVP2_SCORE:
				{//quality setting
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/BestFighterMgr.lua", "UpdateBestFighterPoint", attribute.value());
				}
					break;
				case PB_ATTR_DIAMOND_EX_POWER_TIMES:
				{//buy power number
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "RefreshPowerBuyCount", attribute.value());
				}
					break;
				case PB_ATTR_DAILY_BABEL_INSTANCE_ID:
				{//buy power number
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/HeroInstanceListLayer.lua", "SetNowCompleteInstanceId", attribute.value());
				}
					break;

				// Note: 好友UI
				case PB_ATTR_PLAYER_PVP_DUEL_TIMES:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_pvpDuelTimesNum)
						{
							info.m_pvpDuelTimesNum = attribute.value();
							UserData::SetUserInfo(info);							
						}
					}
					break;
				case PB_ATTR_PLAYER_PVP_DUELED_TIMES:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_pvpDueledTimesNum)
						{
							info.m_pvpDueledTimesNum = attribute.value();
							UserData::SetUserInfo(info);
						}
					}
					break;
				case PB_ATTR_PLAYER_POTENTIAL_DRUG:
					{
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/SpriteElf/SpriteElfStrengthenLayer.lua", "RefreshStrengthenSelfPotentialCount", attribute.value());
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/FairyLayer/FairyTrainLayer.lua", "RefreshTrainDrugCount", attribute.value());
					}
					break;
				case PB_ATTR_PLAYER_FAIRY_ELEM:
					{
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/FairyLayer/FairyLevelUpLayer.lua", "RefreshFairyElementCount", attribute.value());
					}
					break;
				case PB_ATTR_PLAYER_SHOP_REF_TOKEN:
					{
						unsigned int count = attribute.value();

						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "UpdateRefreshCardCount", count);

						int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
						BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
						if(layer)
						{
							ShopUILayer *shopUI = layer->getpShopUILayer();
							if(shopUI && shopUI->isVisible())
							{
								shopUI->updateRefreshCardCount();
							}
						}
					}
					break;
				case PB_ATTR_PLAYER_SOUL_STONE:
					{
						unsigned int count = attribute.value();
						USERINFO info = UserData::GetUserInfo();
						if(count != info.m_soulStone)
						{
							info.m_soulStone = count;
							UserData::SetUserInfo(info);
						    NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SP_SOUL_STONE_UPDATE,this);
						}

						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/main.lua", "UpdateSoulStoneCount", count);

						int tag = MainMenuLayer::getLayerTagByName("BackPackLayer");
						BackPackLayer* layer = MainMenuLayer::getUILayerByTag<BackPackLayer>(tag);
						if(layer)
						{
							ShopUILayer *shopUI = layer->getpShopUILayer();
							if(shopUI && shopUI->isVisible())
							{
								shopUI->updateSoulStoneCount();
							}
						}
					}
					break;
				case PB_ATTR_PLAYER_PVP_RED:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_pvpRed)
						{
							info.m_pvpRed = attribute.value();
							UserData::SetUserInfo(info);
						}
					}
					break;

                case PB_ATTR_JUNIOR_EXPLORE_TIME :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_junior_explore_time)
                        {
                            info.m_junior_explore_time = attribute.value();
                            UserData::SetUserInfo(info);
                            TryUpdateSpriteExtractLayer();

                            NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SPRITE_EXTRACT, this);
                        }
                    }
                    break;

                case PB_ATTR_SENIOR_EXPLORE_TIME :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_senior_explore_time)
                        {
                            info.m_senior_explore_time = attribute.value();
                            UserData::SetUserInfo(info);
                            TryUpdateSpriteExtractLayer();

                            NotificationCenter::defaultCenter()->broadcast(GM_ATTR_SPRITE_EXTRACT, this);
                        }
                    }
                    break;

                case PB_ATTR_91_VIP_DAILY_GIFT_GET_TIME :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if (attribute.value() != info.m_get91_dailyBonus)
                        {
                            info.m_get91_dailyBonus = attribute.value();
                            UserData::SetUserInfo(info);
                            NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAY_UCVIP_REFRESH, this);
                        }
                    }
                    break;


                case PB_ATTR_91_VIP_GIFT_GET_TIME :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if (attribute.value() != info.m_get91_vipBonus)
                        {
                            info.m_get91_vipBonus = attribute.value();
                            UserData::SetUserInfo(info);
                            NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAY_UCVIP_REFRESH, this);
                        }
                    }
                    break;

                case PB_ATTR_91_VIP_TOPUS_STATUS :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if (attribute.value() != info.m_get91_firstChargeBonus)
                        {
                            info.m_get91_firstChargeBonus = attribute.value();
                            UserData::SetUserInfo(info);
                            NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAY_UCVIP_REFRESH, this);
                        }
                    }
                    break;

                case PB_ATTR_91_VIP_STATUS :
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if (attribute.value() != info.m_91Vip)
                        {
                            info.m_91Vip = attribute.value();
                            UserData::SetUserInfo(info);
                            NotificationCenter::defaultCenter()->broadcast(GM_ATTR_PLAY_UCVIP_REFRESH, this);
                        }
                    }
                    break;

				case PB_ATTR_PLAYER_PVP_RED_TIME:
					{
						USERINFO info = UserData::GetUserInfo();
						if(attribute.value() != info.m_pvpRedTimer)
						{
							info.m_pvpRedTimer = attribute.value();
							UserData::SetUserInfo(info);

							SceneLayer* pScene = GameManager::Get()->GetSceneLayer();
							if (pScene)
							{
								MainMenuLayer* pMainMenuLayer = pScene->getMainMenuLayer();
								if (pMainMenuLayer)
								{
									GameUILayer* pGameUILayer = pMainMenuLayer->GetGameUILayer();
									if (pGameUILayer)
									{
										long serverTime = TimeManager::Get()->getCurServerTime();
										if ((info.m_pvpRedTimer - serverTime) > 0)
										{
											pGameUILayer->ShowRedNamePanel();
											SpriteSeer* pHero = GameManager::Get()->getHero();
											if (pHero)
											{
												pHero->ShowNameLabelColor(true);
											}
										}
										else
										{
											pGameUILayer->HideRedNamePanel();
											SpriteSeer* pHero = GameManager::Get()->getHero();
											if (pHero)
											{
												pHero->ShowNameLabelColor(false);
											}
										}
									}
								}
							}
						}
						break;
					}

                case PB_ATTR_EXPLORE_DAILY_TIMES:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_treasure_explore_times)
                        {
                            info.m_treasure_explore_times = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;

                case  PB_ATTR_EXPLORE_MAP_INDEX:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_treasure_explore_mapIndex)
                        {
                            info.m_treasure_explore_mapIndex = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;

                case PB_ATTR_EXPLORE_SUCCESS_RATE:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_treasure_explore_success_rate)
                        {
                            info.m_treasure_explore_success_rate = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;

                case PB_ATTR_EXPLORE_HAVE_GOT_MAP_INDEX:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_treausre_already_get_index)
                        {
                            info.m_treausre_already_get_index = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;
                case PB_ATTR_EXPLORE_REWARD_LEVELUP_FLAG:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_treasure_levelup_flag)
                        {
                            info.m_treasure_levelup_flag = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;
				case PB_ATTR_DOWER_LIGHT:
					{
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Talent/TalentMgr.lua", "RefreshDowerLightCount", attribute.value());
					}
					break;
				case PB_ATTR_STAR_NUM:
					{
						LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Talent/TalentMgr.lua", "RefreshDowerStarNum", attribute.value());
					}
					break;
                case PB_ATTR_WBOSS_VIEW_NUMBER:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_worldboss_forbid_roles)
                        {
                            info.m_worldboss_forbid_roles = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                        CCNotificationCenter::sharedNotificationCenter()->postNotification("CombatSettingNotice");
                    }
                    break;
                //case PB_ATTR_AUTO_COST_DIAMOND:
                //    {
                //        USERINFO info = UserData::GetUserInfo();
                //        if(attribute.value() != info.m_worldboss_revive_by_diamond)
                //        {
                //            info.m_worldboss_revive_by_diamond = attribute.value();
                //            UserData::SetUserInfo(info);
                //        }
                //        CCNotificationCenter::sharedNotificationCenter()->postNotification("CombatSettingNotice");
                //    }
                //    break;
                //case PB_ATTR_AUTO_WAIT_TIMEOUT:
                //    {
                //        USERINFO info = UserData::GetUserInfo();
                //        if(attribute.value() != info.m_worldboss_revive_by_cd)
                //        {
                //            info.m_worldboss_revive_by_cd = attribute.value();
                //            UserData::SetUserInfo(info);
                //        }
                //        CCNotificationCenter::sharedNotificationCenter()->postNotification("CombatSettingNotice");
                //    }
                //    break;
                case PB_ATTR_PLAYER_GUILD_DIALY_GOLD_BLESS:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_guild_bless_times)
                        {
                            info.m_guild_bless_times = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                        CCNotificationCenter::sharedNotificationCenter()->postNotification("update_guild_blesstimes");
                    }
                    break;
                case PB_ATTR_PLAYER_GUILD_CONTRIBUTE_TIMES:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_guild_contribute_times)
                        {
                            info.m_guild_contribute_times = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                        CCNotificationCenter::sharedNotificationCenter()->postNotification("update_guild_contribute_times");
                    }
					break;
				case PB_ATTR_PLAYER_GUILD_INSTANCE_TIMES:
					{
						GuildInstanceListLayer::guildInstancePlayTimes = attribute.value();
						CCNotificationCenter::sharedNotificationCenter()->postNotification("update_guild_instance_times");
					}
					break;
                case PB_ATTR_PLAYER_CONTRIBUTE:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_guild_contributes)
                        {
                            info.m_guild_contributes = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;
                case PB_ATTR_PLAYER_GUILD_LAST_LEAVE_TIME:
                    {
                        USERINFO info = UserData::GetUserInfo();
                        if(attribute.value() != info.m_guild_join_cd_time)
                        {
                            info.m_guild_join_cd_time = attribute.value();
                            UserData::SetUserInfo(info);
                        }
                    }
                    break;
				case PB_ATTR_GET_DAILY_REP_EXCHANGE_FLAG:
					{
						USERINFO info = UserData::GetUserInfo();
						bool flag = (attribute.value() == 0);
						if(flag != info.b_reputationExchangeFlag)
                        {
                            info.b_reputationExchangeFlag = flag;
                            UserData::SetUserInfo(info);
                        }
						break;
					}
			}
		}

        if(slot_set)
        {
             USERINFO mUserData = UserData::GetUserInfo();
             bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
             if(mUserData.first_login && !bMainLandLevel)
             {
                BattleUILayer * layer = MainMenuLayer::getUILayerByTag<BattleUILayer>(MainMenuLayer::getLayerTagByName("BattleUILayer"),false);
		        if (layer)
                  layer->initSkillIcon();

             }
        }
            
	}
}

void MessageHandler::handleMsgPlayerHP()
{
	CSPlayerHPRsp* msg = dynamic_cast<CSPlayerHPRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		PLAYER_ID playId;
		playId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());
		unsigned int blood = msg->blood();
		USERINFO* info = UserData::Get()->GetUserInfo(playId);
		if(info)
		{
			info->health_point = blood;
			UserData::Get()->SetUserInfo(playId,*info);

			USERINFO playerInfo = UserData::GetUserInfo();
			if(playId != playerInfo.id)
			{
				if(info->battleSide % 2 !=  playerInfo.battleSide % 2)
				{
					if(info->health_point != info->total_hp)
					{
						bool isSelf = false;
						if(msg->has_userid())
						{
							PLAYER_ID source;
							source.setPlayId(msg->source_id(),msg->source_reg_time(),msg->source_channel_id());
							if(playerInfo.id == source)
							{
								isSelf = true;
							}
						}
						ParticleManager::Get()->SetEnemyHpLable(info->health_point,playId,4,0,isSelf);
					}
				}
			}
		}
	}
}

void MessageHandler::handlePVPPlayerHitPlayer()
{
	CSPlayerHitPlayerRsp* msg = dynamic_cast<CSPlayerHitPlayerRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int ragePoint = 0;
		bool isFairy = true;
		const SKILL_INFO* pSkillInfo = SkillDispMgr::Get()->GetSkillInfo(msg->action_id());
		if(pSkillInfo)
		{
			isFairy = false;
			ragePoint = pSkillInfo->ragePoint;
		}

		for(int i = 0; i < msg->users_size(); i++)
		{
			const AttackPlayerInfo& info = msg->users(i);
			
			//unsigned int uid = info.userid();
			PLAYER_ID playId;
			playId.setPlayId(info.userid(),info.reg_time(),info.channel_id());
			unsigned int blood = info.blood();
			unsigned int type = info.hurt_type();

			SpriteSeer * player = NULL;
			SpriteSeer* hero = GameManager::Get()->getHero();
			if (hero && hero->GetID() == playId)
			{
				player = hero;
			}
			else
			{
				LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
				if (layer && layer->m_othersPlayerInfo.find(playId) != layer->m_othersPlayerInfo.end())
				{
					player = layer->m_othersPlayerInfo.find(playId)->second.seer;
				}
			}

			if (player)
			{
				CCPoint damagePoint = ccpAdd(player->getPosition(),player->getAttachPoint(kType_PLAYER_NAME));
				switch(type)
				{
				case 1: // ����
					GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackMissEffect(damagePoint);
					break;
				case 2: // ��
					player->ByAttacked(blood, ragePoint,false,isFairy);
					GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->HandleAttackBlockEffect(damagePoint);
					break;
				case 3: // ����
					player->ByAttacked(blood, ragePoint, true,isFairy);
					break;
				default:
					player->ByAttacked(blood, ragePoint,false,isFairy);
					break;
				}
			}
		}
	}
}

void MessageHandler::handleOnceRspMessage()
{
	CSOnceRsp* msg = dynamic_cast<CSOnceRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		unsigned int count = msg->values_size();
		for(unsigned int i=0;i< count; ++i)
		{
			 CSOnceRsp_Once once = msg->values(i);

			 unsigned int index	= once.index();
			 bool is_set = once.is_set();

			 // Note: ���������ڼ�¼��ѧ�ؼ���ť�Ƿ��Ѿ����ֹ�
			 if (index < 1000)
			 {				 
				 if (is_set)
				 {
					 UIControlAppearControler::Get()->TackRevControlState(index);
				 }
				 UIControlAppearControler::Get()->AddOneControllReqMessage(index);
			 }
			 else if (index < kType_MainLandStory_Key)
			 {
			    switch(index)
                {
                case 1001: 
    				 if (is_set)
    				 {
    					 // Note: 1001���ھ�Ӣ���������ŵĿ�������
    					 GameDataManager::Get()->checkEliteTranspointValid(index);
    				 }
                     break;
                case 1002:
                    {
                         USERINFO mUserData = UserData::GetUserInfo();
                         bool enablenbfb = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "GetenableNiuBiFuBen");
                         if(enablenbfb)
                            mUserData.first_login = !is_set;
                         else
                             mUserData.first_login = false;//didn't go to niu bi fu ben
                         
						 UserData::SetUserInfo(mUserData);
                         get1002value = true;
                         XLogicManager::sharedManager()->goToLayer(XLogicManager::E_ENTER_MAINLAND, &mUserData.mapId);
        
                         //SceneLayer* sl = (SceneLayer*)XLogicManager::sharedManager()->GetCurLayer();
                         //sl->onFirstLoadFire();
                         
                        break;
                    }
			    }
			 }
			 else if (index < kType_PopupReward_Key)
			 {
				 if (is_set)
				 {
					 StoryInstanceDirector::Get()->PushUsedItemId(index-kType_MainLandStory_Key);
				 }
			 }
			 else if (index < 10000)
			 {
				 if (is_set)
				 {
				 //jackniu 2014-5-2. not use popup function
					// PopupRewardManager::Get()->PushOneItem(index - kType_PopupReward_Key);
				 }
			 }
			 // Note: ���������ڱ�ʾUI�̳�ID
			 else if (index < 20000)
			 {
				 if (is_set)
				 {
					 TutorialsManager::Get()->InsertOneCompelteTutorial(index-10000);
				 }				 
				 TutorialsManager::Get()->RevOneTutorialMessage(index-10000);
			 }
		}
	}
}

void MessageHandler::handleMsgLoadComplete()
{
	XLogicManager::sharedManager()->setMessageStatus(XLogicManager::E_LOAD_COMPLETE);
	CSBattleLoadCompleteRsp* msg = dynamic_cast<CSBattleLoadCompleteRsp*>(mCurNormMsgPackage->msg);
	CCLOG("==========================load complete================");
	int curInstanceTime = InstanceManager::Get()->getInstanceTime();
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{
		if (curInstanceTime > 0)
		{
			int battleTag = mLayer->getLayerTagByName("BattleUILayer");
			BattleUILayer* battleUILayer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
			battleUILayer->initRemainTime(curInstanceTime);			
		}

		if (msg->has_remains())
		{
			if (LevelManager::sShareInstance()->isCurrentWorldBossLevel())
			{
				LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/CoinBossMgr.lua", "InitCoinBossRemainTimeMgr", msg->remains());
			}
		}

		if (LevelManager::sShareInstance()->isCurrentPPVELevel() 
			|| LevelManager::sShareInstance()->isCurrentMandPKLevel()||LevelManager::sShareInstance()->isCurrentCommonPKLevel()
			|| LevelManager::sShareInstance()->isCurrentPVPLevel()
			|| LevelManager::sShareInstance()->isCurrentCityDefendLevel()
			|| LevelManager::sShareInstance()->isCurrentBestFighterLevel())
		{					
			BattleCoverLayer* battleCoverLayer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
			battleCoverLayer->closeBattleCoverLayer(NULL);
		}
	}

	if (GameDataManager::Get()->getHeroAutoAttack())
	{
		SpriteSeer * hero = GameManager::Get()->getHero();
		if (hero)
		{
			hero->startAutoAttack();
		}	

		if (mLayer)
		{				
			BattleCoverLayer* battleCoverLayer = MainMenuLayer::getUILayerByTag<BattleCoverLayer>(MainMenuLayer::getLayerTagByName("BattleCoverLayer"));
			battleCoverLayer->openBattleCoverLayer(COVER_AUTOATTACK);
		}
	}

	if (LevelManager::sShareInstance()->isCurrentPVAILevel())
	{
		PvAIManager::Get()->setAIAutoAttack(true);
	}
	if (LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
	{
		GuildInstanceManager::Get()->setAIAutoAttack(true);
	}
	if (LevelManager::sShareInstance()->isCurrentWorldBossLevel())
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/WorldBossMgr.lua", "GetWorldBossViewPlayer");
	}
}

void MessageHandler::handleMsgBattleTimeout()
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	if (mLayer)
	{		
		int battleTag = mLayer->getLayerTagByName("BattleUILayer");
		BattleUILayer* battleUILayer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
        battleUILayer->clearSkillProgressTimer();

		MainMenuLayer::hideUILayerByTag(MainMenuLayer::getLayerTagByName("BattleUILayer"));
		HSJoystick::sharedJoystick()->setIsEnable(false);
        
		GameDataManager::Get()->setHeroAutoAttack(false);
        
        int tag = mLayer->getLayerTagByName("AccountsLayer");
        AccountsLayer *layer = (AccountsLayer*)mLayer->findUILayerByTag(tag);
        if(layer == NULL)
        {
            AccountsLayer *player = AccountsLayer::create();
            mLayer->addChild(player,10,tag);
            player->openFailureBattleTimeOut();
        }else
        {
            layer->closeAccountsLayer();
            layer = AccountsLayer::create();
            mLayer->addChild(layer,10,tag);
            layer->openFailureBattleTimeOut();
        }
    }
}

void MessageHandler::handleMsgPlayerDie()
{
	CSBattlePlayerDelRsp* msg = dynamic_cast<CSBattlePlayerDelRsp*>(mCurNormMsgPackage->msg);	

	PLAYER_ID playId;
	playId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());

	if(playId == UserData::getUserId())
	{

		MainMenuLayer* mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		if(GameManager::Get()->getHero())
		{
			GameManager::Get()->getHero()->Die();
			GameDataManager::Get()->setHeroAutoAttack(false);
		}
		if (LevelManager::sShareInstance()->isCurrentPVAILevel())
		{
			PvAIManager::Get()->setAIAutoAttack(false);
		}
		if (LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
		{
			GuildInstanceManager::Get()->setAIAutoAttack(false);
		}
        
        //clear cd when player die
        int battleTag = mLayer->getLayerTagByName("BattleUILayer");
        BattleUILayer* layer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
        if(layer)
        {
            layer->clearSkillProgressTimer();
        }
        
		if(!LevelManager::sShareInstance()->isCurrentPVAILevel()
           && !LevelManager::sShareInstance()->isCurrentPVPLevel()
		   && !LevelManager::sShareInstance()->isCurrentMandPKLevel()&&!LevelManager::sShareInstance()->isCurrentCommonPKLevel()
           && !LevelManager::sShareInstance()->isCurrentBestFighterLevel()
           && !LevelManager::sShareInstance()->isCurrentWorldBossLevel())
		{
			if (mLayer)
			{
				int tag = mLayer->getLayerTagByName("AccountsLayer");
				if(mLayer->findUILayerByTag(tag) == NULL)
				{
					AccountsLayer *player = AccountsLayer::create();
					mLayer->addChild(player,10,tag);
					player->openFailureBattleDie(msg->exchange_id());
				}
			}
		}
	}
	else
	{
		LevelLayer * layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			SpriteSeer* otherPlayer = layer->getPlayer(playId);
			if (otherPlayer)
			{
				otherPlayer->Die();
			}
		}
	}
}

void MessageHandler::handleMsgGetUserSkillRsp()
{
	CSGetUserSkillRsp* msg = dynamic_cast<CSGetUserSkillRsp*>(mCurNormMsgPackage->msg);

	//���±��ط�������Ҽ�??
	Message* filterMsg = MessageUtil::sShareInstance()->createMessageInstance("CSGetUserSkillRsp");
	filterMsg->CopyFrom(* msg);
	CSGetUserSkillRsp* serverSkill = dynamic_cast<CSGetUserSkillRsp*>(filterMsg);
	MessageFilter::Get()->setServerUserSkill(*serverSkill);
	if(serverSkill)
	{
		delete serverSkill ;
		serverSkill = NULL ;
	}
	
	if(msg)
	{
		//unsigned int user_id = msg->userid();
		PLAYER_ID playerId;
		playerId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());
		int count = msg->userskills_size();
        
		vector<int> equipSkills;
		USERINFO mUserData = UserData::GetUserInfo();
		for (int index = 0;index<count;index++)
		{
			UserSkillInfo skillInfo = msg->userskills(index);
			if (playerId == UserData::Get()->getUserId())
			{
				
				if(!mUserData.first_login) {
					if(SkillDispMgr::Get()->GetSkillInfo(skillInfo.skill_id())->type != ToNormalAttack)
						GameConfigFromLuaManager::Get()->PushSkillItemToLua(skillInfo.skill_id(),skillInfo.skill_level(),skillInfo.max_level());				

				}
			}
			else if (playerId == PvAIManager::Get()->getAIPlayerId() || playerId == GuildInstanceManager::Get()->getAIPlayerId())
			{
				equipSkills.push_back(skillInfo.skill_id());
			}

			SkillDispMgr::Get()->PushOneSKillUserInfoToTable(playerId, skillInfo.skill_id(),skillInfo);

			// Note: 预加载声音文件
			//bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
			//if (false == bMainLandLevel)
			{
				const SKILL_INFO* pSkillInfo = SkillDispMgr::Get()->GetSkillInfo(skillInfo.skill_id());
				if (pSkillInfo)
				{
					unsigned int soundId = pSkillInfo->SoundId;
					unsigned int effectSoundId = pSkillInfo->effectsoundID;
					unsigned int boyVoice = pSkillInfo->boyVoiceID;
					unsigned int girlVoice = pSkillInfo->girleVoiceID;
					GameAudioManager::sharedManager()->preLoadEffect(soundId);
					GameAudioManager::sharedManager()->preLoadEffect(effectSoundId);
					GameAudioManager::sharedManager()->preLoadEffect(boyVoice);
					GameAudioManager::sharedManager()->preLoadEffect(girlVoice);
				}
			}
		}

		if (playerId == PvAIManager::Get()->getAIPlayerId())
		{
			PvAIManager::Get()->setAISkillsId(equipSkills);
		}
		if (playerId == GuildInstanceManager::Get()->getAIPlayerId())
		{
			GuildInstanceManager::Get()->setAISkillsId(equipSkills);
		}
		if(!mUserData.first_login)
			GameConfigFromLuaManager::Get()->SkillLevelUpSucMessage();
	}
}

void MessageHandler::handleMsgUserSkillDiffRsp()
{
	CSUserSkillDiffRsp* msg = dynamic_cast<CSUserSkillDiffRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{ 

		unsigned int skill_id = msg->skill_id();
		unsigned int skill_level = msg->skill_level();
		
		unsigned int next_money = msg->money();
		unsigned int next_exploit = msg->exploit();
		unsigned int next_lv = msg->lv();
		float diff_own_cd	= msg->own_cd();
		float continue_time_cd = msg->continue_time();
		unsigned int diff_hurt = msg->hurt();

		BasicUserBattleInfo BattleInfo = msg->battle_info();

		// �������?
		unsigned int physical_attack = BattleInfo.physical_attack();
		// ħ������
		unsigned int magic_attack = BattleInfo.magic_attack();
		// ���ܹ�??
		unsigned int skill_attack = BattleInfo.skill_attack();
		// �������?
		unsigned int physical_defence = BattleInfo.physical_defence();
		// ħ������
		unsigned int magic_defence = BattleInfo.magic_defence();
		// ���ܷ�??
		unsigned int skill_defence = BattleInfo.skill_defence();
		// ����??
		unsigned int health_point = BattleInfo.health_point();
		// ��׼ 
		unsigned int accurate	= BattleInfo.accurate();
		// ���� 
		unsigned int dodge		= BattleInfo.dodge();
		// �ƻ�
		unsigned int wreck		= BattleInfo.wreck();
		// ��
		unsigned int parry		= BattleInfo.parry();
		// ����
		unsigned int critical_strike	= BattleInfo.critical_strike();
		// ��??
		unsigned int tenacity	= BattleInfo.tenacity();
		// ��ɱ
		unsigned int slay		= BattleInfo.slay();
		// ��??
		unsigned int speed		= BattleInfo.speed();
		// Ǳ��
		unsigned int proficiency	= BattleInfo.proficiency();

		GameConfigFromLuaManager::Get()->PushDiffInfoToLua(skill_id,skill_level,next_money,next_exploit,next_lv,diff_own_cd,
			continue_time_cd,diff_hurt,physical_attack,magic_attack,skill_attack,physical_defence,magic_defence,skill_defence,health_point,
			accurate,dodge,wreck,parry,critical_strike,tenacity,slay,speed,proficiency);
	}
}

void MessageHandler::handleMsgAddUserSkillRsp()
{
	CSAddUserSkillRsp* msg = dynamic_cast<CSAddUserSkillRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		unsigned int skill_id = msg->skill_id();
		unsigned int skill_level = msg->skill_level();
		unsigned int max_level = msg->max_level();

		// Note: 刚刚升级时，自动装配技能
		if (skill_level == 1)
		{
			GameConfigFromLuaManager::Get()->EquipSkillToLeftSlot(skill_id);
			LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Skill/SkillDataCenter.lua", "SetNewSkillId", skill_id);
			NotificationCenter::defaultCenter()->broadcast(GM_GET_NEW_SKILL,this);
		}
		GameConfigFromLuaManager::Get()->AddUserSkillRspToLua(skill_id,skill_level,max_level);
	}
}

void MessageHandler::handleMsgTrainRsp()
{
	CSTrainRsp * msg = dynamic_cast<CSTrainRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		int deltaCourage = msg->courage();
		int deltaCharm = msg->charm();
		int deltaTrick = msg->trick();
		bool bSuc = msg->succ();

		SpriteElfConfigFromLuaManager::getInstance()->HandleAwakeSuccessMessage(deltaCourage,deltaCharm,deltaTrick,bSuc);
	}
}

void MessageHandler::handleMsgTrainDetermineRsp()
{
	CSTrainDetermineRsp* msg = dynamic_cast<CSTrainDetermineRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		// Note: Save Suc
		SpriteElfConfigFromLuaManager::getInstance()->HandleSaveAwakeResultMessage();
	}
}

void MessageHandler::handleExploreRsp()
{
	CSExploreRsp* msg = dynamic_cast<CSExploreRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		unsigned int type = msg->option();
		unsigned int map_id = msg->map_id();

		SpriteElfConfigFromLuaManager::getInstance()->PushExploreMapIdToData(type,map_id);
	}
}

void MessageHandler::handleExploreDetermineRsp()
{
	CSExploreDetermineRsp* msg = dynamic_cast<CSExploreDetermineRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		CSReward reward = msg->rewards();
		unsigned int	add_coin = reward.add_coin();
		unsigned int	add_exp = reward.add_exp();
		unsigned int	add_exploit = reward.add_exploit();
		unsigned int	map_id = msg->map_id();

		SpriteElfConfigFromLuaManager::getInstance()->PushExploreBasicRewardInfo(map_id,add_coin,add_exp,add_exploit);

		unsigned int count = reward.rewards_size();
		for (unsigned int index = 0;index<count;index++)
		{
			CSReward_RewardInfo rewardInfo = reward.rewards(index);
			unsigned int item_id = rewardInfo.item_id();
			unsigned int item_level = rewardInfo.item_level();
			unsigned int item_num = rewardInfo.item_num();

			SpriteElfConfigFromLuaManager::getInstance()->PushExploreOtherRewardInfo(item_id,item_level,item_num);
		}

		SpriteElfConfigFromLuaManager::getInstance()->ExploreRewardCommit();
	}
}

void MessageHandler::handleNotifyMoneyConfigInfo()
{
	CSNotifyLevelConfInfoRsp* msg = dynamic_cast<CSNotifyLevelConfInfoRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{		
		unsigned int trainMoney = msg->train_money();
		
		//SpriteElfConfigFromLuaManager::getInstance()->PushElfStrengthData(trainMoney,2,10,50);
    	USERINFO info =  UserData::GetUserInfo();
        //CCLOG("%s, my %d, server %d", __FUNCTION__, info.mine_money, msg->mine_money());
        if(msg->mine_money() != info.mine_money)
		{
			info.mine_money = msg->mine_money();
			UserData::SetUserInfo(info);
			NotificationCenter::defaultCenter()->broadcast(GM_ATTR_BAO_ZHANG_COIN_CHANGE,this);
		}

		if (msg->pay_money() != info.pay_money)
		{
			info.pay_money = msg->pay_money();
			UserData::SetUserInfo(info);
		}
        if (msg->train_money() != info.train_money)
		{
			info.train_money = msg->train_money();
			UserData::SetUserInfo(info);
		}
        if (msg->suspend_limit_exp() != info.suspend_limit_exp)
		{
			info.suspend_limit_exp = msg->suspend_limit_exp();
			UserData::SetUserInfo(info);
		}
    }
}

void MessageHandler::handleMsgPvAIInfoRsq()
{
	CCLOG("handleMsgPvAIInfoRsq----------------------");
	CSPVAIInfoRsq* msg = dynamic_cast<CSPVAIInfoRsq*>(mCurNormMsgPackage->msg); 
	if (msg)
	{		
		int heroRank = msg->rank();
        
        // if role arena rank is no.1,then hide arena button exclamatory mark
        UIManager *manager = UIManager::sharedManager();
        UILayout* layout = manager->getUILayout("MainMenu");
        UIButton* pvaiBtn = layout->FindChildObjectByName<TXGUI::UIButton>("button_communicate");
		int remainPvAICount = PvAIManager::Get()->getRemainCount();
        if (pvaiBtn)
        {
            if (remainPvAICount > 0 && heroRank != 1)
			{				
				pvaiBtn->showTipIcon();
			}
			else
			{
				pvaiBtn->hideTipIcon();
			}
        }
		PvAIManager::Get()->setHeroRank(heroRank);

		//�Ƽ�����б�?
		vector<AIPlayerInfo> aiPlayerInfos;
		int userCount = msg->infos_size();
		for (int i = 0; i < userCount; i++)
		{
			CSPVAIInfoRsq_CSPVAIInfo playerInfo = msg->infos(i);
			AIPlayerInfo aiPlayerInfo;
			aiPlayerInfo.id.setPlayId(playerInfo.userid(),playerInfo.reg_time(),playerInfo.channel_id());
			aiPlayerInfo.rank = playerInfo.rank();
			aiPlayerInfo.name = playerInfo.name();
			aiPlayerInfo.type = playerInfo.type();
			aiPlayerInfo.level = playerInfo.lv();
			aiPlayerInfo.aiStatus = playerInfo.ai_status();
			aiPlayerInfos.push_back(aiPlayerInfo);
		}
		PvAIManager::Get()->setAIPlayerVec(aiPlayerInfos);

		//�����־��??
		vector<AILogInfo> aiLogs;
		int logCount = msg->logs_size();
		for (int i = 0; i < logCount; i++)
		{
			CSPVAIInfoRsq_CSPVAILog logInfo = msg->logs(i);
			AILogInfo aiLogInfo;
			aiLogInfo.name = logInfo.name();
			aiLogInfo.direct = logInfo.direct();
			aiLogInfo.win = logInfo.win();
			aiLogs.insert(aiLogs.begin(), aiLogInfo);
			//aiLogs.push_back(aiLogInfo);
		}
		//��ʱ��������
		//AILogInfo aiLogInfo;
		//aiLogInfo.name = "testpp";
		//aiLogInfo.direct = true;
		//aiLogInfo.win = false;
		//aiLogs.push_back(aiLogInfo);

		PvAIManager::Get()->setAILogInfoVec(aiLogs);
        PvAIManager::Get()->refreshPvAILayer();
	}
}

void MessageHandler::handleBasicUserBattleInfoRsp()
{
	CSBasicUserBattleInfoRsp* msg = dynamic_cast<CSBasicUserBattleInfoRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{		
		PLAYER_ID playerId;
		playerId.setPlayId(msg->id(),msg->reg_time(),msg->channel_id());
		if(playerId == UserData::getUserId())
		{
			USERINFO userInfo = UserData::GetUserInfo();
			int nBlood = msg->battle_info().current_hp() - userInfo.health_point;

			OnlineNetworkManager::sShareInstance()->sendHeroHPChangeReq(nBlood);
		}

		USERINFO* pUserData = UserData::Get()->GetUserInfo(playerId);
		if(pUserData)
		{
			BasicUserBattleInfo info = msg->battle_info();
			pUserData->Set(info);
		}
	}
}

void  MessageHandler::handlePVXRewardRsp()
{
	GameDataManager::Get()->setHeroAutoAttack(false);
	//�����������ǰ��ֹͣaiս��
	if (LevelManager::sShareInstance()->isCurrentPVAILevel())
	{
		PvAIManager::Get()->setAIAutoAttack(false);
	}
	if (LevelManager::sShareInstance()->isCurrentGuildInstanceLevel())
	{
		GuildInstanceManager::Get()->setAIAutoAttack(false);
	}

    CSPVXRewardRsp* msg = dynamic_cast<CSPVXRewardRsp*>(mCurNormMsgPackage->msg);
    unsigned int add_coin = msg->coin();
    
    AccountsLayer *player = AccountsLayer::create();
	MainMenuLayer* layer = GetMainMenuLayer();
	int tag = layer->getLayerTagByName("AccountsLayer");
    layer->removeChildByTag(tag, true);
	layer->addChild(player,10,tag);
    bool iswin = msg->is_win();

	MainMenuLayer::hideUILayerByTag(MainMenuLayer::getLayerTagByName("BattleUILayer"));
	HSJoystick::sharedJoystick()->setIsEnable(false);
	std::vector<accountReward> vec;   
	if(msg->has_reward())
	{
		for(int i=0; i<msg->reward().rewards_size(); i++)
            {
                CSReward_RewardInfo info = msg->reward().rewards(i);
                accountReward accReWard;
                accReWard.item_id = info.item_id();
                accReWard.item_level = info.item_level();
                accReWard.item_num = info.item_num();
                vec.push_back(accReWard);
            }
	}
    if(iswin)
    {
		if (LevelManager::sShareInstance()->isCurrentPVAILevel())
		{
			LevelLayer * levellayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
			PLAYER_ID aiPlayerId = PvAIManager::Get()->getAIPlayerId();
			if (levellayer && aiPlayerId.getID() != 0)
			{
				SpriteSeer * aiPlayer = levellayer->getPlayer(aiPlayerId);
				
				if (aiPlayer)
				{
					USERINFO * info = UserData::Get()->GetUserInfo(aiPlayerId);
					if (info->health_point <= 0)
					{
						aiPlayer->Die();
					}					
				}
			}
		}
		if (LevelManager::sShareInstance()->isCurrentCommonPKLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel())
		{
		  player->openWinPK();
		  CSReward reward_val =	msg->reward();
		  CSReward_RewardInfo info = reward_val.rewards(0);
		  unsigned int id = info.item_id();
		  player->setRandItem(info.item_id() ,info.item_num());
		  //showItemTipsById
		}
		else
			player->openWinPVP();
        player->accountWinPvP(add_coin, 0, 0, msg->reputation(),vec);
    }
    else
    {
		if (LevelManager::sShareInstance()->isCurrentCommonPKLevel()|| LevelManager::sShareInstance()->isCurrentMandPKLevel())
		{
                 player->openFailurePK();
		}else
		{
			 player->openFailurePVP();
		}
        player->accountWinPvP(add_coin, 0, 0, msg->reputation(),vec);
    }
}

void  MessageHandler::handlePVP2RewardRsp()
{
	GameDataManager::Get()->setHeroAutoAttack(false);
	

    CSPVP2RewardRsp* msg = dynamic_cast<CSPVP2RewardRsp*>(mCurNormMsgPackage->msg);
	if(msg->btl_ret() != PB_PVP2_BTL_RET_WIN)
		return;//jackniu only precess win msg. other process by lua


    
    AccountsLayer *player = AccountsLayer::create();
	MainMenuLayer* layer = GetMainMenuLayer();
	int tag = layer->getLayerTagByName("AccountsLayer");
    layer->removeChildByTag(tag, true);
	layer->addChild(player,10,tag);
   

	MainMenuLayer::hideUILayerByTag(MainMenuLayer::getLayerTagByName("BattleUILayer"));
	HSJoystick::sharedJoystick()->setIsEnable(false);
	std::vector<accountReward> vec;   
	if(msg->has_reward())
	{
		for(int i=0; i<msg->reward().rewards_size(); i++)
            {
                CSReward_RewardInfo info = msg->reward().rewards(i);
                accountReward accReWard;
                accReWard.item_id = info.item_id();
                accReWard.item_level = info.item_level();
                accReWard.item_num = info.item_num();
                vec.push_back(accReWard);
            }
	}
   	player->accountWinPvP(0, 0, 0, 0,vec);
	player->accountWinPvP2(msg->bcoin(), msg->bpvp2_score(),
		msg->ccoin(), msg->cpvp2_score());

	player->openWinPVP2();
   
}

void MessageHandler::handleDailyTaskInfoRsq()
{
	CSDailyTaskInfoRsp* msg = dynamic_cast<CSDailyTaskInfoRsp*>(mCurNormMsgPackage->msg);

	int Count = msg->daily_tasks_size();
	for (int i = 0; i < Count; ++i)
	{
		DailyTaskInfo taskInfo = msg->daily_tasks(i);
		unsigned int task_id = taskInfo.daily_task_id();
		int task_state = taskInfo.daily_task_status();
		bool bSuc = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "FreshDailyTaskList",task_id,task_state);
	}
	if (Count > 0)
	{
		bool bSuc = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "FreshDailyTaskEnd");
	}	
}

void MessageHandler::handleDailyTaskAcceptRsq()
{
	CSAcceptDailyTaskRsp* msg = dynamic_cast<CSAcceptDailyTaskRsp*>(mCurNormMsgPackage->msg);
	bool bSuc = msg->succ();
	unsigned int taskId = msg->daily_task_id();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "AcceptOneDailyTask",bSuc,taskId);
}

void MessageHandler::handleDailyTaskGiveUpRsq()
{
	CSGiveUpDailyTaskRsp* msg = dynamic_cast<CSGiveUpDailyTaskRsp*>(mCurNormMsgPackage->msg);
	bool bSuc = msg->succ();
	unsigned int taskId = msg->daily_task_id();

	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "GiveUpOneDailyTask",bSuc,taskId);
}

void MessageHandler::handleDailyTaskNotifyUpdateTaskInfoRsq()
{
	CSNotifyUpdateDailyTaskInfoRsp* msg = dynamic_cast<CSNotifyUpdateDailyTaskInfoRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		unsigned int updateDailyTaskCount = msg->update_daily_tasks_size();
		for (unsigned int index = 0;index<updateDailyTaskCount;index++)
		{
			DailyTaskInfo taskInfo = msg->update_daily_tasks(index);
			unsigned int task_id = taskInfo.daily_task_id();
			int task_state = taskInfo.daily_task_status();

			LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "DailyTaskUpdateOneTask",task_id,task_state);			
		}
	}
}

void MessageHandler::handleDailyTaskGetRewardRsq()
{
	CSGetDailyTaskRewardRsp* msg = dynamic_cast<CSGetDailyTaskRewardRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		bool bSuc = msg->succ();
		unsigned int taskId = msg->daily_task_id();

		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/DailyTask/dailyTaskDataCenter.lua", "DailyTaskGetReward",bSuc,taskId);		
	}
}

void MessageHandler::handleBattlePlayerRevivalRsp()
{
	CSBattlePlayerRevivalRsp * msg = dynamic_cast<CSBattlePlayerRevivalRsp *>(mCurNormMsgPackage->msg);
	if (msg)
	{
		PLAYER_ID playId;
		playId.setPlayId(msg->userid(),msg->reg_time(),msg->channel_id());

		if(playId == UserData::getUserId())
		{
			USERINFO userInfo = UserData::GetUserInfo();
			int nBlood = userInfo.total_hp - userInfo.health_point;
			OnlineNetworkManager::sShareInstance()->sendHeroHPChangeReq(nBlood);

		}

		MainMenuLayer* pMainMenuLayer = GetMainMenuLayer();
		int tag = pMainMenuLayer->getLayerTagByName("AccountsLayer");
		AccountsLayer *accountslayer = dynamic_cast<AccountsLayer*>(pMainMenuLayer->findUILayerByTag(tag));
		if(accountslayer != NULL)
		{
			if(playId == UserData::getUserId())
			{
				accountslayer->DoRevival();
				


				int musicId = 0;
				int mapID = LevelManager::sShareInstance()->getCurMapId();
				if(MainLandManager::Get()->isMainLand( mapID ))
				{
					musicId = MainLandManager::Get()->GetCityBGM( mapID );

				}
				else
				{
					int instanceId = InstanceManager::Get()->getCurInstanceId();
					musicId = GameConfigFromLuaManager::Get()->getInstanceMusicId(instanceId, mapID);
				}
				GameAudioManager::sharedManager()->playBGMWithMap(musicId);

			}
		}	

		LevelLayer * layer =  LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (layer)
		{
			SpriteSeer* player = layer->getPlayer(playId);
			if (player)
			{
				player->Revival();
				if(playId == UserData::getUserId() && LevelManager::sShareInstance()->isCurrentWorldBossLevel())
				{
					LuaTinkerManager::Get()->callLuaFunc<bool>("Script/Instance/WorldBossMgr.lua", "HideWorldBossFailLayer");
				}
			}
		}
	}
}

void MessageHandler::handleMsgBattlePrepareRsq()
{
	CSBattlePrepareRsp* msg = dynamic_cast<CSBattlePrepareRsp*>(mCurNormMsgPackage->msg);
	if (msg)
	{
		//CSMapContentList_t mapContents = msg->map_contents();	
		OnlineNetworkManager::sShareInstance()->sendLocalBattlePrepareRsq(*msg);

		//create battle
		int instanceId = InstanceManager::Get()->getPreInstanceId();
		int mapId = InstanceManager::Get()->getPreMapId();

		if (instanceId)
		{			
			XLogicManager::sharedManager()->createBattle(instanceId, mapId);
		}

		InstanceManager::Get()->setPreInstanceId(0);
		InstanceManager::Get()->setPreMapId(0);
	}
}

void MessageHandler::handleMsgSkillNeedUpdateRsq()
{
	CSSkillUpdateValidRsq* msg = dynamic_cast<CSSkillUpdateValidRsq*>(mCurNormMsgPackage->msg);
	{
		bool bValid = msg->valid();

		bool ret = LuaTinkerManager::Get()->callLuaFunc<bool>(
			"Script/Skill/SkillSelectLayer.lua",
			"SkillHandleSkillUpdateNotification",bValid);
	}
}

void MessageHandler::handleMsgNickNameRsp()
{
	CSNickNameRsp* msg = dynamic_cast<CSNickNameRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		int count = msg->nick_size();
		unsigned int sexual = msg->sexual();
		if(count > 0)
		{
			for(int i = 0;i < count;++i)
			{
				string nickName = msg->nick(i);
				NickNameManager::Get()->onReceiveNewNickName(nickName.c_str(),sexual);
			}
		}
	}
}

void MessageHandler::handleMsgLoginAnnouncementRsp()
{
}

void MessageHandler::handleMsgAnnouncementPreRsp()
{
    CSAnnouncementPreRsp* msg = dynamic_cast<CSAnnouncementPreRsp*>(mCurNormMsgPackage->msg);
    if(msg)
    {
        LuaTinkerManager::Get()->callLuaFunc<void>("Script/GameUILayer.lua", "onNotice2");
    }
}

void MessageHandler::handleMsgBattleUserAddBloodRsp()
{
	CSBattleUserAddBloodRsp* msg = dynamic_cast<CSBattleUserAddBloodRsp*>(mCurNormMsgPackage->msg);
	if(msg)
	{
		if(GameManager::Get()->GetSceneLayer())
		{
			MainMenuLayer* layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
			if(layer)
			{
				PLAYER_ID playerId;
				playerId.setPlayId(msg->id(),msg->reg_time(),msg->channel_id());
				SpriteSeer* player = NULL;
				if(playerId == UserData::getUserId())
				{
					player = GameManager::Get()->getHero();
				}
				else
				{
					LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
					if (layer && layer->m_othersPlayerInfo.find(playerId) != layer->m_othersPlayerInfo.end())
					{
						player = layer->m_othersPlayerInfo.find(playerId)->second.seer;
					}
				}
				if(player)
				{
					CCPoint damagePoint = ccpAdd(player->getPosition(),player->getAttachPoint(kType_PLAYER_NAME));
					layer->HandleAddBloodNumberEffect(damagePoint,msg->blood());
				}
				
			}
		}
		
	}
}

void MessageHandler::HandleMsgCSGetRednameTimeRsp()
{
	if (mCurNormMsgPackage)
	{
		CSGetRednameTimeRsp* msg = dynamic_cast<CSGetRednameTimeRsp*>(mCurNormMsgPackage->msg);
		if(msg)
		{
			USERINFO userInfo = UserData::GetUserInfo();
			if (userInfo.id.getID() == msg->userid() &&
				userInfo.id.getChannel() == msg->channelid() &&
				userInfo.id.getRegTime() == msg->reg_tm())
			{
				// Note: 当前玩家
				unsigned int redNameRemainTime = msg->time();
				long nowTime = TimeManager::Get()->getCurServerTime();

				if (redNameRemainTime != 0)
				{
					USERINFO info = UserData::GetUserInfo();
					info.m_pvpRedTimer = nowTime + redNameRemainTime;
					UserData::SetUserInfo(info);
				}			

				SceneLayer* pScene = GameManager::Get()->GetSceneLayer();
				if (pScene)
				{
					MainMenuLayer* pMainMenuLayer = pScene->getMainMenuLayer();
					if (pMainMenuLayer)
					{
						GameUILayer* pGameUILayer = pMainMenuLayer->GetGameUILayer();
						if (pGameUILayer)
						{
							if (redNameRemainTime > 0)
							{
								pGameUILayer->ShowRedNamePanel();
								SpriteSeer* pHero = GameManager::Get()->getHero();
								if (pHero)
								{
									pHero->ShowNameLabelColor(true);
								}
							}
							else
							{
								pGameUILayer->HideRedNamePanel();
								SpriteSeer* pHero = GameManager::Get()->getHero();
								if (pHero)
								{
									pHero->ShowNameLabelColor(false);
								}
							}
						}
					}
				}
			}
			else
			{
				// Note: 其他玩家
				LevelLayer* layer = LevelManager::sShareInstance()->getCurrentLevelLayer();
				if (layer)
				{
					PLAYER_ID userId(msg->userid(),msg->channelid(),msg->reg_tm());
					SpriteSeer * seer = layer->getPlayer(userId);
					if (seer)
					{
						if (msg->time() > 0)
						{
							seer->ShowNameLabelColor(true);
						}
						else
						{
							seer->ShowNameLabelColor(false);
						}
					}
				}
			}
		}
	}
}

void MessageHandler::HandleMsgSpriteDrawRsp()
{
    CSSpiritDrawRsp* msg = dynamic_cast<CSSpiritDrawRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        std::vector<int> spriteList;
        spriteList.clear();

        if (msg->spirits_size() == 0)
        {
            return;
        }

        for (int i = 0; i < msg->spirits_size(); ++i)
        {
            spriteList.push_back(msg->spirits(i));
        }

        // 显示抽取界面
        MainMenuLayer* layer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
        if (layer != NULL)
        {
            SpriteExtractResultLayer* resultLayer = dynamic_cast<SpriteExtractResultLayer*>(layer->getSpriteExtractResultLayer());
            if (resultLayer != NULL)
            {
                resultLayer->SpriteOut(spriteList, msg->type());
            } else
            {
                layer->ShowSpriteExtractResultLayer(true);

                resultLayer = dynamic_cast<SpriteExtractResultLayer*>(layer->getSpriteExtractResultLayer());
                resultLayer->SpriteOut(spriteList, msg->type());
            }
        }

        TryUpdateSpriteExtractLayer();
    }
}

void MessageHandler::TryUpdateSpriteExtractLayer()
{
    // 显示抽取界面
    SceneLayer* sceneLayer = GameManager::Get()->GetSceneLayer();
    if (sceneLayer == NULL)
    {
        return;
    }

    MainMenuLayer* layer = sceneLayer->getMainMenuLayer();
    if (layer == NULL)
    {
        return;
    }

    SpriteExtractLayer* extractLayer = dynamic_cast<SpriteExtractLayer*>(layer->getSpriteExtractResultLayer());
    if (extractLayer != NULL)
    {
        extractLayer->InitTime();
    }
}

void MessageHandler::HandleMsgPlayerRank()
{
    CSQueryPlayerRankInfoRsp* msg = dynamic_cast<CSQueryPlayerRankInfoRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        //RANK_NAME_LV_SCORE			= 2;	// 等级
        //RANK_NAME_POWER_SCORE		    = 3;	// 战斗力
        //RANK_NAME_REPUTATION_SCORE	= 4;	// 声望
        //RANK_NAME_FAIRY_SCORE   	    = 5;	// 精灵
        int type = msg->rank_name();
        if(type != RANK_NAME_LV_SCORE && type != RANK_NAME_POWER_SCORE
            && type != RANK_NAME_REPUTATION_SCORE && type != RANK_NAME_FAIRY_SCORE)
        {
            return;
        }

        int playerRank = 0;
        for(int i = 0; i < msg->rank_info_size(); i++)
        {
            int type = msg->rank_info(i).rank_type();
            if(type != 1)
            {
                continue;
            }
            playerRank = msg->rank_info(i).rank();
        }
        RankManager::getInstance()->OnReceivePlayerRank(playerRank);
    }
}

void MessageHandler::HandleStarReward()
{
    //CSChkInstanceStarRewardRsp* msg = dynamic_cast<CSChkInstanceStarRewardRsp*>(mCurNormMsgPackage->msg);
    //if (!msg)
    //{
    //    return;
    //}
    //UILayout * layout = UIManager::sharedManager()->getUILayout("MainMenu");	
    //if (!layout)
    //{
    //    return;
    //}
    //UIButton * mainRewardBtn = layout->FindChildObjectByName<UIButton>("button_starReward");
    //if(msg->all_or_nothing())
    //{
    //    mainRewardBtn->setVisible(true);
    //    mainRewardBtn->ShowRotateEffect();
    //    XLogicManager::sharedManager()->hasStarReward = true;
    //}
    //else
    //{
    //    mainRewardBtn->setVisible(false);
    //    XLogicManager::sharedManager()->hasStarReward = false;
    //}
   
    ////
    //LuaTinkerManager::Get()->callLuaFunc<void,int>("Script/UILayer/starRewardUI.lua", "setFirstPageNum", msg->last_raid_id());
}

void MessageHandler::HandleMsgTreasureInfoRsp()
{
    // the treasure remain times, mapidx, rate data is before then this msg
    CSGetExploreInfoRsp* msg = dynamic_cast<CSGetExploreInfoRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        //0不提示，1提示成功，2提示失败
        USERINFO info = UserData::GetUserInfo();        
        int rate = info.m_treasure_explore_success_rate;
        int mapindex = info.m_treasure_explore_mapIndex;

        // 刷新奖励 升级奖励 次数和开启等级
        if(msg->param_size() == 2)
        {
            //int refreshTimes = msg->param(0);
			int resetTimes = msg->param(0);
            int refreshOpenLv = msg->param(1);
            //TreasureExploreManager::Get()->setRefreshTimes(refreshTimes);
			TreasureExploreManager::Get()->setResetTimes(resetTimes);
            TreasureExploreManager::Get()->setRefreshOpenLv(refreshOpenLv);
        }

        // show tip clamation
        UIManager *manager = UIManager::sharedManager();
        UILayout* layout = manager->getUILayout("MainMenu");
        UIButton* exploreBtn = layout->FindChildObjectByName<TXGUI::UIButton>("button_treasurehunt");
        if (exploreBtn)
        {
            int alreadyTimes = info.m_treasure_explore_times;
            int canFreeTimes = TreasureExploreManager::Get()->GetCurMapFreeTimes(mapindex);
            if((canFreeTimes - alreadyTimes) > 0)
            {
                exploreBtn->showTipIcon();
            }
            else
            {
                exploreBtn->hideTipIcon();
            }
        }

        CCNotificationCenter::sharedNotificationCenter()->postNotification("updateTreasureInfo");

        // explore success or fail
        switch(msg->res())
        {
        case 0:
            return;
            break;

        case 1:
            {
                // success            
                if(mapindex > 10)
                {
                    ShowMessage(Localizatioin::getLocalization("M_TREASURE_SUCCESS2"));
                }
                else
                {
                    ShowMessage(Localizatioin::getLocalization("M_TREASURE_SUCCESS1"));
                }                
            }
            break;

        case 2:
            {
                int ForwardRate = TreasureExploreManager::Get()->getForwardRate();
                // fail
                //CCString* str = CCString::createWithFormat(Localizatioin::getLocalization("M_TREASURE_FAIL"), (rate - ForwardRate));
                //ShowMessage(str->getCString());
                ShowMessage(Localizatioin::getLocalization("M_TREASURE_FAIL"));
            }
            break;
        }
        TreasureExploreManager::Get()->setForwardRate(rate);
    }    
}

void MessageHandler::HandleMsgTreasureRewardGetRsp()
{
    CSGetExploreTreasureRsp* msg = dynamic_cast<CSGetExploreTreasureRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        if(msg->res())
        {
            // success            
            ShowMessage(Localizatioin::getLocalization("M_GET_SUCCESS"));
        }
        else
        {
            // fail
            ShowMessage(Localizatioin::getLocalization("M_TREASURE_RECEIVEFAIL"));
        }
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification("updateTreasureInfo");
}

void MessageHandler::HandleMsgTresureItems()
{
    CSNotifyItemExploreRewardRsp* msg = dynamic_cast<CSNotifyItemExploreRewardRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        TreasureExploreManager::Get()->onReceiveTreasureItems(msg);
    }
}

void MessageHandler::HandleMsgRankInfoRsp()
{
    CSQueryRankRsp* msg = dynamic_cast<CSQueryRankRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        std::vector<RankInfoX> tempVec;
        tempVec.clear();
        for(int i = 0; i < msg->rank_info_size(); i++)
        {
            RankInfoX info;
            info.userid = msg->rank_info(i).userid();
            info.reg_time = msg->rank_info(i).reg_time();
            info.channel_id = msg->rank_info(i).channel_id();
            info.name = msg->rank_info(i).name();
            info.score = msg->rank_info(i).score();
            info.rank = msg->rank_info(i).rank();
            info.updown = msg->rank_info(i).updown();
            info.rank_name = msg->rank_name();
            info.type = msg->rank_info(i).type();
            info.guild_name = msg->rank_info(i).gname();
            tempVec.push_back(info);
        }
        RankManager::getInstance()->OnReceiveRankInfoData(tempVec);
    }
}

void MessageHandler::HandleMsgGetTopupAwardRsp()
{
    CSGetTopupAwardRsp* msg = dynamic_cast<CSGetTopupAwardRsp*>(mCurNormMsgPackage->msg);
    if (msg)
    {
        if (msg->result() == GET_TOPUP_AWARD_SUCCESS)
        {
            // 显示抽取界面
            SceneLayer* sceneLayer = GameManager::Get()->GetSceneLayer();
            if (sceneLayer == NULL)
            {
                return;
            }

            MainMenuLayer* layer = sceneLayer->getMainMenuLayer();
            if (layer == NULL)
            {
                return;
            }

            FirstChargeLayer* firstLayer = dynamic_cast<FirstChargeLayer*>(layer->getFirstChargeLayer());
            if (firstLayer != NULL)
            {
                firstLayer->setGetBtnEnable();
            }
        }
    }
}

void MessageHandler::HandleMsgGetShoppingRsp()
{
    CSShoppingRsp* msg = dynamic_cast<CSShoppingRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        std::map<int, int> mapResult;
        mapResult.clear();

        for (int i = 0; i < msg->shoppings_size(); ++i)
        {
            CSShoppingInfo shopInfo = msg->shoppings(i);
            mapResult.insert(std::make_pair(shopInfo.shopping_id(), shopInfo.buy_times()));
        }

        MarketManager::Get()->UpdateShopInfo(mapResult);
		NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_MARKET_TIME_INFO,this);
    }
}

void MessageHandler::HandleTargetFinishedRsp()
{
	CSTargetFinishedRsp * msg = dynamic_cast<CSTargetFinishedRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
		bool flag =  msg->res();
		USERINFO info = UserData::GetUserInfo();
		if(flag != info.b_hasTargetIconFlag)
		{
			info.b_hasTargetIconFlag= flag;
			UserData::SetUserInfo(info);
		}
        NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_TARGET_ICON,this);
	}
}

void MessageHandler::HandleGuildListRsp()
{
    CSListGuildRsp* msg = dynamic_cast<CSListGuildRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGuildList(msg);
    }
}

void MessageHandler::HandleGuildSearchRsp()
{
    CSSearchGuildRsp* msg = dynamic_cast<CSSearchGuildRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGuildSearchList(msg);
    }
}

void MessageHandler::HandleGuildCreateRsp()
{
    CSCreateGuildRsp* msg = dynamic_cast<CSCreateGuildRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {   
        GuildManager::Get()->onGuildCreateResult(msg);
    }
}

void MessageHandler::HandleGuildJoinRsp()
{
    CSJoinGuildRsp* msg = dynamic_cast<CSJoinGuildRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {   
        GuildManager::Get()->onGuildJoinResult(msg);
    }
}

void MessageHandler::HandleGetGuildInfoRsp()
{
    CSQueryPlayerGuildInfoRsp* msg = dynamic_cast<CSQueryPlayerGuildInfoRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGetGuildInfo(msg);
    }
}

void MessageHandler::HandleGetGuildMemberRsp()
{
    CSListPlayerGuildRsp* msg = dynamic_cast<CSListPlayerGuildRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGetGuildMember(msg);
    }
}

void MessageHandler::HandleSetGuildNoticeRsp()
{

}

void MessageHandler::HandleGetGuildRecordInfo()
{
	CSGuildRecordListRsp* msg = dynamic_cast<CSGuildRecordListRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGuildRecordInfo(msg);
	}
}

void MessageHandler::HandleChangeGuildMemberStatusRsp()
{
    CSChangePlayerGuildStatusRsp* msg = dynamic_cast<CSChangePlayerGuildStatusRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onUpdateGuildMember(msg);
    }
}

void MessageHandler::HandleGuildShopItems()
{
    CSGetShoppingItemsRsp* msg = dynamic_cast<CSGetShoppingItemsRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGetGuildShopItems(msg);
    }
}

void MessageHandler::HandleGuildBless()
{
    CSGoddessBlessRsp* msg = dynamic_cast<CSGoddessBlessRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGetGuildBlessInfo(msg);
    }
}

void MessageHandler::HandleGuildContributeInfo()
{
    CSExchangeItemInfoRsp* msg = dynamic_cast<CSExchangeItemInfoRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        GuildManager::Get()->onGetGuildContributeInfo(msg);
    }
}

void MessageHandler::HandleGuildContribute()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuildContributeInfo_update");
}

void MessageHandler::HandleGuildQuild()
{
    GuildManager::Get()->onRspQuitGuild();
}
void MessageHandler::HandleGuildDrop()
{
    GuildManager::Get()->onRspDropGuild();
}

void MessageHandler::HandleGuildTips()
{
    CSGuildNoteRsp* msg = dynamic_cast<CSGuildNoteRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        ShowMessage(msg->text().c_str());
    }
}

void MessageHandler::handleMsgNewEquipNoteRsp()
{
	CSEquipNotify * msg = dynamic_cast<CSEquipNotify*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
		unsigned int equipPos =  msg->pos();
		ItemManager::Get()->setEquipNewNote(equipPos,true);
	}
}

void MessageHandler::HandleExchangeParameterListRsp()
{
    CSExchangeParameterListRsp* msg = dynamic_cast<CSExchangeParameterListRsp*>(mCurNormMsgPackage->msg);
    if (msg != NULL)
    {
        std::vector<ExchangePrice> vtPrices;
        vtPrices.clear();

        for (int i = 0; i < msg->prices_size(); ++i)
        {
            ExchangePrice price;
            price.exchangeId = msg->prices(i).id();
            price.price = msg->prices(i).price();
            vtPrices.push_back(price);
        }

        MarketManager::Get()->UpdateExchangeList(vtPrices);
    }
}

void MessageHandler::handleMcardInfoRsp()
{
	CSMcardInfoRsp* msg = dynamic_cast<CSMcardInfoRsp*>(mCurNormMsgPackage->msg);
	if(msg != NULL && msg->info_size() > 0 )
	{
		MonthCardManager::Get()->setMonthCardInfo(msg);
	}
}

void MessageHandler::handleMcardRewardRsp()
{
	CSMcardRewardRsp* msg = dynamic_cast<CSMcardRewardRsp*>(mCurNormMsgPackage->msg);
	if(msg != NULL)
	{
		
	}
}

void MessageHandler::HandleGetGuildRedEnvoInfo()
{
	CSGuildRedPacketQueryRsp* msg = dynamic_cast<CSGuildRedPacketQueryRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGuildRedEnvoInfo(msg);
	}
}

void MessageHandler::HandleGuildGetRedEnvoRsp()
{
	CSGuildRedPacketGetAwardRsp* msg = dynamic_cast<CSGuildRedPacketGetAwardRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGetRedEnvoRsp(msg);
	}
}

void MessageHandler::HandleGuildGetRedEnvoRecordRsp()
{
	CSGuildRedPacketRecordRsp* msg = dynamic_cast<CSGuildRedPacketRecordRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGetRedEnvoRecordRsp(msg);
	}
}

void MessageHandler::HandleGuildGetRedEnvoConfigRsp()
{
	CSGuildRedPacketConfigRsp* msg = dynamic_cast<CSGuildRedPacketConfigRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGetRedEnvoConfigRsp(msg);
	}
}

void MessageHandler::HandleGuildFirstSendRedEnvoRsp()
{
	CSGuildRedPacketFirstCheckRsp* msg = dynamic_cast<CSGuildRedPacketFirstCheckRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onFirstSendRedEnvoRsp(msg);
	}
}

void MessageHandler::HandleGuildSecondSendRedEnvoRsp()
{
	CSGuildRedPacketSendRsp* msg = dynamic_cast<CSGuildRedPacketSendRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onSecondSendRedEnvoRsp(msg);
	}
}


void MessageHandler::HandleGetGuildUnionInfo()
{
	CSUnionInfoRsp* msg = dynamic_cast<CSUnionInfoRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGuildUnionInfo(msg);
	}
}

void MessageHandler::HandleGetGuildChooseInfo()
{
	CSGetRecruitMemberRsp* msg = dynamic_cast<CSGetRecruitMemberRsp*>(mCurNormMsgPackage->msg);

	if (msg != NULL)
	{
		GuildManager::Get()->onGuildChooseInfo(msg);
	}
}
