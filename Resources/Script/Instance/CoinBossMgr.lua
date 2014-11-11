require("Script/Instance/CoinBossRankLayer")
require("Script/Instance/CoinBossDamageLayer")
require("Script/Instance/CoinBossWinLayer")

local function sendCoinBossBattleCreateReq()
	CSBattleCreateReq = {
		battle_id = 290000,
		map_id = 0,
		}
	local msgname="CSBattleCreateReq"
	local ret = send_message(msgname, CSBattleCreateReq, true)
	return ret
end

-- 进入金币副本
function OnPressEnterCoinBossInstance()
    sendCoinBossBattleCreateReq()
	PushOneWaitingLayer("CSBattleCreateReq")
end

-- 拉取排行奖励
function GetCoinBossRankReward()
    local msgBody = {
        rank_name = 6,
        rank_type = 3,
        rank_start = 1,
        rank_size = 10,
    }
	send_message("CSRankRewardListReq", msgBody, true)
end

-- 收到排行奖励
local function onMsgRankRewardRsp(msg)
	CCLuaLog("--------------- onMsgRankRewardRsp-----------")
    if msg.rank_name ~= 6 or msg.rank_type ~= 3 then
        return 
    end

    CoinBossRankLayer.rankRewardList = {}
	CCLuaLog("------------ step 1")
    for _, rewardList in pairs(msg.items) do
		CCLuaLog("------------ step 2")
        local rewardData = {}
        rewardData.rank = rewardList.rank_start
		rewardData.rankEnd = rewardList.rank_end

        for _, item in pairs(rewardList.items) do
			--CCLuaLog("------------ step 3")
            local itemData = {}
            itemData.id = item.item_id
            itemData.num = item.item_num
            table.insert(rewardData, itemData)
        end
        
        table.insert(CoinBossRankLayer.rankRewardList, rewardData)
    end

    local function sortFunc(a, b)
        return a.rank < b.rank
    end
    table.sort(CoinBossRankLayer.rankRewardList, sortFunc)

    RefreshRankRewardDetailLayer()
end

-- 拉取玩家排行
function GetCoinBossPlayerRank()
    local msgBody = {
        rank_name = 6,
        rank_type = 3,
        rank_start = 1,
        rank_end = 30,
    }
	send_message("CSQueryRankReq", msgBody, true)
end

-- 收到玩家排行
local function onMsgPlayerRankRsp(msg)
	CCLuaLog("--------------- onMsgPlayerRankRsp-----------")
    if msg.rank_name ~= 6 or msg.rank_type ~= 3 then
        return 
    end

    CoinBossRankLayer.rankPlayerList = {}

    for _, info in pairs(msg.rank_info) do
		CCLuaLog("rank player")
        local playerData = {}
        playerData.userId = info.userid
        playerData.regTime = info.reg_time
        playerData.channelId = info.channel_id
        playerData.name = info.name
		playerData.rank = info.rank
		playerData.hurtNum = math.floor(info.score / 100)
		playerData.vip = info.score % 100

		table.insert(CoinBossRankLayer.rankPlayerList, playerData)
    end

	local function compareFunc(a, b)
        return a.rank < b.rank
    end
    table.sort(CoinBossRankLayer.rankPlayerList, compareFunc)

	RefreshCoinBossRankList()

end

-- 拉取个人信息
function GetCoinBossSelfRankInfo()
	local msgBody = {
        rank_name = 6,
		userid = UserData:getUserId():getID(),
		reg_time = UserData:getUserId():getRegTime(),
		channel_id = UserData:getUserId():getChannel(),
    }
	send_message("CSQueryPlayerRankInfoReq", msgBody, true)
end

-- 收到个人信息
local function onMsgSelfRankRsp(msg)
	CCLuaLog("--------------- onMsgSelfRankRsp-----------")
	if msg.rank_name ~= 6 then
		return
	end

	local userId = UserData:getUserId():getID()
	if msg.userid ~= userId then
		return
	end

	CoinBossRankLayer.selfRankData = {}
	CoinBossRankLayer.selfRankData.name = msg.name
	CoinBossRankLayer.selfRankData.remainCount = msg.wb_daily_times

	for _ , info in pairs(msg.rank_info) do
		if info.rank_type == 3 then
			CoinBossRankLayer.selfRankData.rank = info.rank
			CoinBossRankLayer.selfRankData.vip = info.score % 100
			CoinBossRankLayer.selfRankData.hurtNum = math.floor(info.score / 100)
		end
	end

	RefreshSelfRankInfo()
	RefreshCoinBossRankList()
end


-- 收到实时最高伤害及下一个目标
local function onMsgCoinBossRealTimeHurtRsp(msg)
	CCLuaLog("--------------- onMsgCoinBossRealTimeHurtRsp-----------")
	local nowTotalHurt = msg.max_acc_hurt
	local nextHurt = msg.next_target
	RefreshCoinBossRealTimeHurt(nowTotalHurt, nextHurt)
end


-- 收到金币副本结束包
local function onMsgCoinBossFinishRsp(msg)
	CCLuaLog("--------------- onMsgCoinBossFinishRsp-----------")

	
	GameDataManager:Get():setHeroAutoAttack(false)	
	HSJoystick:sharedJoystick():setIsEnable(false)

	HideOneLayerByTag(UITagTable["BattleUILayer"].tag)

	CoinBossWinLayer.winData = {}
	CoinBossWinLayer.winData.maxHurt = msg.his_max_hurt
	CoinBossWinLayer.winData.nowHurt = msg.max_acc_hurt
	CoinBossWinLayer.winData.rankNum = msg.rank
	CoinBossWinLayer.winData.rankChange = msg.updown
	CoinBossWinLayer.winData.itemId = msg.item_id

	showUILayerByTag(UITagTable["CoinBossWinLayer"].tag,true)
end



---------------------------------------------------------------------------
-- 打开金币副本页面
function OpenCoinBossRankLayer()

    showUILayerByTag(UITagTable["CoinBossRankLayer"].tag,true)

    GetCoinBossRankReward()
	GetCoinBossPlayerRank()
	GetCoinBossSelfRankInfo()

	PushOneWaitingLayer("CSRankRewardListReq")
	PushOneWaitingLayer("CSQueryRankReq")
	PushOneWaitingLayer("CSQueryPlayerRankInfoReq")

    addMsgCallBack("CSRankRewardListRsp", onMsgRankRewardRsp)
    addMsgCallBack("CSQueryRankRsp", onMsgPlayerRankRsp)
	addMsgCallBack("CSQueryPlayerRankInfoRsp", onMsgSelfRankRsp)
	addMsgCallBack("CSCBFinshBattleRsp", onMsgCoinBossFinishRsp)
	addMsgCallBack("CSCBHurtNotifyRsp", onMsgCoinBossRealTimeHurtRsp)	
end

-- 进入金币副本的处理
function OnEnterCoinBossInstance()
	showUILayerByTag(UITagTable["CoinBossDamageLayer"].tag,true)
	showUILayerByTag(UITagTable["WorldBossHPLayer"].tag,true)
end

function InitCoinBossRemainTimeMgr(remainTime)
	InitCoinBossRemainTime(remainTime)
end