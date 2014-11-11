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
#ifndef ONLINE_NETWORK_MANAGER_H
#define ONLINE_NETWORK_MANAGER_H

#include "NetWorkUtil.h"
#include "cs_battle.pb.h"
#include "UserData.h"

#include "NetWorkConfigure.h"
#include "cs_config.pb.h"

class OnlineNetworkManager : public NetWorkUtil
{
	friend class PPVEMessage;
    friend class PVPMessage;

public:

	static OnlineNetworkManager*	sShareInstance();
	static void Destroy();

	void	setServerIP(const uint32_t serverIP);
	void	setServerIP(const char *serverIP);
	void	setServerPort(const uint32_t serverPort);

	void	LostConnection2Server(const char * msg = "");

public:

	//------------------------------------------------------------------
	//				start socket connect to online server
	//------------------------------------------------------------------
	void			startConnect();

    //------------------------------------------------------------------
    //				socket reconnect
    //------------------------------------------------------------------
    bool            startReconnect(std::string ip, uint32_t port);

	//------------------------------------------------------------------
	//				player enter map message
	//------------------------------------------------------------------
	void			sendPlayerEnterMapMessage(int mapId, int xPos, int yPos,int orient);

	//------------------------------------------------------------------
	//				player leave map message
	//------------------------------------------------------------------
	//void			sendPlayerLeaveMapMessage(int mapId);

	//------------------------------------------------------------------
	//				player walk message	
	//------------------------------------------------------------------
	void			sendPlayerMoveMessage(int xPos, int yPos, unsigned int animID, bool bFlip);
	void			sendPlayerBattleMoveMessage(PLAYER_ID id, const cocos2d::CCPoint& pos, const cocos2d::CCPoint& direction, bool bEnd);

	//------------------------------------------------------------------
	//				attack
	//------------------------------------------------------------------
	void			sendPlayerAttackMessage(const CSPlayerAttackReq& attackReq);
	void			sendPlayerAttackOtherPlayerMessage(const CSPlayerHitPlayerReq& attackReq);
	void			sendSkillBeginMessage(const CSSkillBeginReq& req);

	//------------------------------------------------------------------
	//				send get other players message	
	//------------------------------------------------------------------
	void			sendGetOtherPlayersMessage();



	void			sendGetInstanceListMessage(uint32_t start, uint32_t end);

	void			sendGetAnnouncementPreReq();

	//------------------------------------------------------------------
	//				login related message	
	//------------------------------------------------------------------
	//void			sendCheckSessionMessage();
	void			sendCreateRoleMessage(const std::string &nick, const uint32_t type,
											const char* deviceID,const char* platform,const char* ipAddress,
											const char* macAddress,const char* eqtype,const char* svnNumber);
	void			sendPlatformLoginMessage(const char* sess,int platformId,int serverId,int userId,const char* appID,
											const char* appKey,const char* cpId,const char* channelId,const char* extraId,
											unsigned int gameId,const char* deviceID,const char* platform,
											const char* macAddress,const char* eqtype,const char* svnNumber);
	void            sendLoginOutMessage();
	void			sendLoginMessage();
	void			sendUserDeviceInfoToServer(const char* deviceID,const char* platform,const char* ipAddress,
												const char* macAddress,const char* eqtype,const char* svnNumber);
	void			sendGetRoleListMessage(unsigned int serverId);
	// 拉取多个服务器的角色列表
	void			sendGetRoleListMessage(const vector<unsigned int>& serverList);
	void			sendLogoutMessage();
	void			sendBattleCreateMessage(int battleId, int mapId);
	void			sendGetTokenMessage();
	void			sendCheckTokenMessage();

	void			sendLoadCompleteMessage();
	void			sendLeaveBattleReqMessage();

	////			items
	void			sendBackPackMessage();
	void			sendBackPackMove(unsigned int from,unsigned int to);
	void			sendBackPackReOrder(unsigned int type);
	void			sendBackPackRemoveItem(unsigned int pos,unsigned int itemId);
	void			sendEquipAllMsg(unsigned int fairyPos);
	// 浣跨敤鐗╁搧(绀煎寘 ...)	
	void			sendUseItem(unsigned int pos, unsigned int itemId,unsigned int num = 1);
	void			sendEquipComposeReq(unsigned int exchangeId,unsigned int pos,unsigned int multi = 1);
	void			sendItemExchangeReq(unsigned int exchangeId,unsigned int pos[],int length);
	void			sendElfExchangeReq(unsigned int exchangeItemId);
	//void			sentEquipUpgradeMessage();
	void			sentEquipUpgradeStrength(unsigned int pos);
	void			sendFindSoulStoneMessage();
	void			sendSoulStoneToStorage(unsigned int pos);
	void			sendUpgradeSoulStone(unsigned int pos);
	void			sendTranStoneToEnergy(bool isAll,unsigned int pos = 0);
	void			sendSpriteExchangeMessage();
	
	//瀹濈煶闀跺祵
	void			sendEquipInlayGemReq(unsigned int equip,unsigned int hole,unsigned int gemId);			// 瀹濈煶闀跺祵璇锋眰
	void			sendGemRecastReq(unsigned int equip,unsigned int holes[],unsigned int holeNum,unsigned int key);	//瀹濈煶閲嶉摳璇锋眰
	void			sendExchangeParameterReq(unsigned int exchangeId,unsigned int* params=NULL, int len=0);					// 閽荤煶绯荤粺鍗忚

	////		    sever
	void			sendQueryShardReq();

	///				Attributes
	void			sendAttributesReq(int attributes[],int length);
	void			sendUserInfoReq(PLAYER_ID id);
	void			sendGetChipStatusReq(unsigned int chipId);
	/// <summary>
	//	Task 璇锋眰鐩稿叧鐨勫崗璁?
	/// </summary>
	/**
	* Instruction : 璇锋眰浠诲姟淇℃伅鍒楄〃
	* @param 
	*/	
	void			sendRqsTaskInfoMessage();
	/**
	* Instruction : 璇锋眰鑾峰緱浠诲姟Log
	* @param 
	*/
	void			sendRqsTaskLogMessage();
	/**
	* Instruction : 鍚戞湇鍔″櫒鍙戦�佹煇涓�涓换鍔℃楠?
	* @param 
	*/	
	void			sendRqsTaskDoStepMessage(unsigned int task_id,unsigned int step_id,unsigned int step_value);
	/**
	* Instruction : 
	* @param 
	*/
	void			sendReqNetStateMessage();

	void			sendGetTaskStatusMessage(int taskId);

	// Note: 绮剧伒鐩稿叧鍗忚
	/**
	* Instruction : 鑾峰彇鑻遍泟绮剧伒
	* @param 
	*/
	void			sendReqOneHeroElfMessage(PLAYER_ID roleId,const char* roleName);
	/**
	* Instruction : 璁剧疆绮剧伒鐨勭姸鎬佷俊鎭?
	* @param 
	*/
	void			sendReqSetElfStateMessage(unsigned int elfId,unsigned int state);

	// Note: 璁剧疆鍗忚
	void			sendOnceSetReqMessage(unsigned int index);
	void			sendOnceReqMessage(int indexs[],int length);
	void			sendOnceReqMessage(std::vector<int> &vec);
	void			sendOnceReqMessage(std::map<unsigned int,bool> &map,unsigned int attachValue);

	// Note: 鎶�鑳経I鍗忚
	void			sendGetUserSkillReqMessage(PLAYER_ID roleId);
	void			sendAddUserSkillReqMessage(unsigned int skillId,unsigned int levelId);
	void			sendUserSkillDiffReqMessage(unsigned int skillId,unsigned int levelId);
	void			sendGetSlotReqMessage();
	void			sendSetSlotAttributeMessage(unsigned int slotIndex,unsigned skillId);

	// Note: 绮剧伒鍞ら啋鐩稿叧鍗忚
	void SendElfAwakeMessage(unsigned int awakeType);
	void SendAwakeConfigMessage();

	// Note: 绮剧伒鎺㈢储鍗忚
	void SendExploreReq(unsigned int type);
	void SendExploreDetermineReq(unsigned int optionId);

	// Note: 鐜╁鍒犻櫎璐﹀彿淇℃伅鍗忚
	void SendDelRoleReqMessage();

	// Note: PvAI 鐩稿叧鍗忚
	void sendPvAIReq(PLAYER_ID userId);
	
	void sendMonthCardInfoReq();
	void sendMonthCardGetReq(unsigned int cardId);
	//jing ying fu ben protocal
	//instanceId is between 1-100000;

	//void sendOnceDailyReq(unsigned int *instanceIds, int len);

	// Note: 姣忔棩浠诲姟鐩稿叧鍗忚
	void sendDailyTaskReqMessage();
	void sendDailyTaskAcceptReqMessage(unsigned int taskId);
	void sendGiveUpTaskReqMessage();
	void sendGetDailyTaskRewardReqMessage();

	void SendBattlePlayerRevival();

	//pve鍚敤鏈湴鏈嶅姟鍣ㄦ椂锛屽彧鍙戝線杩滅▼鏈嶅姟鍣ㄧ殑鍗忚
	void sendRemoteMonsterDieReqMessage(std::vector<int> monsterIdVec);
	void sendRemotePlayerHPReqMessage();

	//pve data prepare
	void sendBattlePrepareReq(int battleId);
	void sendLocalBattlePrepareRsq(CSBattlePrepareRsp prepareRsq);

	void sendGetRandomName(unsigned int sexual);

	void sendHeroHPChangeReq(int nBlood);
	//battle
	void SendPVPLearnReq(unsigned int uid ,unsigned int reg_tm ,unsigned int channel_id ,bool mandatory);
    void SendPVPAckLearnReq(unsigned int uid ,unsigned int reg_tm ,unsigned int channel_id ,bool accept,int type );

	// Note: 鑾峰彇鑷韩鐜╁鐨勭孩鍚嶄俊鎭?
	void SendPlayerRedNameTimerReq();

    // 璇锋眰鎶藉彇绮剧伒
    void SendExtractSprite(int type);

	// 鏂扮殑绮剧伒鍔熻兘
	void SendMoveFairyPosReq(unsigned int fromPos,unsigned int toPos);

    // 请求玩家排名情况
    void SendGetPlayerRankNum(int rankName, int userId, int regTime, int channelId);
    // 请求特定类型的排行榜信息 
    void SendIndexTypeRankInfo(int rankName, int rankType, int startPos, int endPos);
    // 请求竞技场信息
    void SendGetPVAIInfo();
    // 请求星数奖励信息
    void SendGetStarRewardInfo();
    // 请求探宝界面数据信息
    void SendGetTreasureExploreInfo();
    // 探宝
    void SendExploreTreasure(int mapId);
    void SendGuildBuy(int id, int index);
    // 领取探宝奖励
    void SendGetTreasureReward();

    // 领取首冲奖励
    void SendGetTopupAwardReq(int award_id);

    // 请求获取商品购买次数列表
    void SendGetShopTimes(std::vector<int> itemList);

    // 请求每日奖励领取
    void SendGetDailyReward(int type);

    void SendGetExchangeParameters(std::vector<int> exchangeIds);

    // guild
    void SendGetGuildList(unsigned int begin, unsigned int end);
    void SendSearchIndexGuild(std::string name);
    void SendJoinGuild(int highGid, int lowGid);
    void SendCreateGuild(std::string name);
    void SendGetGuildInfo();
    void SendModifyNotice(std::string notice, std::string note);
    void SendGetGuildMember();
    void SendChangeGuildMemberStatus(std::vector<unsigned int> vt_user_id, 
                                     std::vector<unsigned int> vt_reg_time, 
                                     std::vector<unsigned int> vt_channel_id,
                                     std::vector<unsigned int> vt_status);
    void SendQuitGuild();
    void SendJiesanGuld();
    void SendGetGuildShopItems(int type = 0);
    void SendGetGuildBlessInfo(bool flag);
    void SendGetGuildContributeInfo(std::vector<int> temp);
    void SendGuildContribute(int type);
    void SenderGuildLApplySettting(int type, int fightNum = 0);

	void SendGetGuildRecordInfoReq(unsigned int highID, unsigned int lowID, unsigned int zoneID);
	void SendGetGuildUnionInfoReq();
	void SendGetGuildChooseInfoReq(int instanceId);
	void SendCreateGuildInstanceReq(int instanceId, bool hasChoose, CSPlayerGuildInfo info);

	void SendGetGuildRedEnvoInfoReq();

	void SendFirstSendRedEnvoReq(unsigned int envoID, unsigned int envoType);

	void SendSecondSendRedEnvoReq(unsigned int envo_id, unsigned int envo_type);

	void SendGetRedEnvoReq(CSGuildRedPacketKey key);

	void SendGetRedEnvoRecordReq(CSGuildRedPacketKey key);

	void SendGetRedEnvoConfigReq();

protected:

	OnlineNetworkManager();
	~OnlineNetworkManager();	

private :
    void CreateUpdateHpQueue();
    void _sendHeroHPChangeReq(float f);

protected:

	void   TestEcho();

private:

	static OnlineNetworkManager*						sInstance;

	std::string	 m_serverIP;
	uint32_t m_serverPort;

    int m_updateHpAmount;
};



#endif
