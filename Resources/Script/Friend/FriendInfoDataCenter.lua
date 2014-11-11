require("Script/Friend/FriendInfo")
require("Script/handleMessage")
-- 好友数据
FriendInfoDataCenter = {
	FriendList = {},
	NearByPlayerList = {},
	RedPlayerList = {},
	EnemiesPlayerList = {},
	PlayerInfoTable = {},
	friendListCallBack = nil,
	nearPlayerCallBack = nil,
	redPlayerCallBack = nil,
	enemiesPlayerCallBack = nil,
	updateFunc = 0,
}

function FriendInfoDataCenter:clearFriendInfo()
	self.FriendList= {}
	self.NearByPlayerList = {}
	self.RedPlayerList = {}
	self.EnemiesPlayerList = {}
	for k,v in pairs(self.PlayerInfoTable) do
		if self.PlayerInfoTable[k] ~= nil then
			self.PlayerInfoTable[k] = nil
		end
	end
	self.PlayerInfoTable = {}
end

local function updateFriendList()
	CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(FriendInfoDataCenter.updateFunc)
	FriendInfoDataCenter.updateFunc = 0
	if FriendInfoDataCenter.friendListCallBack ~= nil then
		FriendInfoDataCenter.friendListCallBack()
	end
	if FriendInfoDataCenter.nearPlayerCallBack ~= nil then
		FriendInfoDataCenter.nearPlayerCallBack()
	end
	if FriendInfoDataCenter.redPlayerCallBack ~= nil then
		FriendInfoDataCenter.redPlayerCallBack()
	end
	if FriendInfoDataCenter.enemiesPlayerCallBack ~= nil then
		FriendInfoDataCenter.enemiesPlayerCallBack()
	end
end

function sendCSPartnerInfoReq()
	CSPartnerInfoReq = {}
	local msgname="CSPartnerInfoReq"
	local ret = send_message(msgname, CSPartnerInfoReq, true)
	return ret
end

function sendCSNearbyRednameReq()
	CSNearbyRednameReq = {}
	local msgname="CSNearbyRednameReq"
	local ret = send_message(msgname, CSNearbyRednameReq, true)
	return ret
end

function sendCSNearByReq()
	CSNearByReq = {}
	local msgname="CSNearByReq"
	local ret = send_message(msgname, CSNearByReq, true)
	return ret
end

function sendCSEnemyInfoReq()
	CSEnemyInfoReq = {}
	local msgname="CSEnemyInfoReq"
	local ret = send_message(msgname, CSEnemyInfoReq, true)
	return ret
end

--Note 收到宿敌列表
local function receiveCSEnemyInfoRsp(msg)

	-- 接收数据
	for k,v in pairs(FriendInfoDataCenter.EnemiesPlayerList) do
		FriendInfoDataCenter.EnemiesPlayerList[k] = nil
	end

	FriendInfoDataCenter.PlayerInfoTable = {}
	FriendInfoDataCenter.EnemiesPlayerList = {}

	if msg.enemy ~= nil then

		for k,v in pairs(msg.enemy) do
			local player = msg.enemy[k]
			local friendInfo = FriendInfo:createFriendInfo()
			--local userId = PLAYER_ID:new()
			--userId:setPlayId(player.userid,player.reg_tm,player.channel_id)
			local userId = PlayerIdMake(player.userid,player.reg_tm,player.channel_id)
			friendInfo:setID(userId)
			friendInfo:setName(player.name)
			--friendInfo:setRegTime(player.reg_tm)
			friendInfo:setLevel(player.lv)
			friendInfo:setStrength(player.powerscore)
			friendInfo:setType(player.type)
			friendInfo:setReputation(player.prestige)
			friendInfo:setRank(player.pvai_rank)
			friendInfo:setLeftPkNum(player.left_pvped_times)
			friendInfo:setRegTime(player.reg_tm)
			friendInfo:setChannelId(player.channel_id)
			friendInfo:setGuildName(player.gname)
			--friendInfo:setOnLineFlag(player.isol)
			friendInfo.onlineFlag = player.isol
			friendInfo:setKilledPlayerNum(player.killed_times)
			FriendInfoDataCenter.PlayerInfoTable[userId] = friendInfo
			FriendInfoDataCenter.EnemiesPlayerList[userId] = userId
		end
	end

	-- 下一帧更新UI响应
	if FriendInfoDataCenter.updateFunc == 0 then
		FriendInfoDataCenter.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateFriendList, 0.01, false);
	end
	return true;	
end

--Note: 收到红名列表
local function receiveCSNearbyRednameRsp(msg)

	-- 接收数据
	for k,v in pairs(FriendInfoDataCenter.RedPlayerList) do
		FriendInfoDataCenter.RedPlayerList[k] = nil
	end

	FriendInfoDataCenter.PlayerInfoTable = {}
	FriendInfoDataCenter.RedPlayerList = {}

	if msg.nearby ~= nil then

		for k,v in pairs(msg.nearby) do
			local player = msg.nearby[k]
			local friendInfo = FriendInfo:createFriendInfo()
			--local userId = PLAYER_ID:new()
			--userId:setPlayId(player.userid,player.reg_tm,player.channel_id)
			local userId = PlayerIdMake(player.userid,player.reg_tm,player.channel_id)
			friendInfo:setID(userId)
			friendInfo:setName(player.name)
			--friendInfo:setRegTime(player.reg_tm)
			friendInfo:setLevel(player.lv)
			friendInfo:setStrength(player.powerscore)
			friendInfo:setType(player.type)
			friendInfo:setReputation(player.prestige)
			friendInfo:setRank(player.pvai_rank)
			friendInfo:setLeftPkNum(player.left_pvped_times)
			friendInfo:setRegTime(player.reg_tm)
			friendInfo:setChannelId(player.channel_id)
			--friendInfo:setOnLineFlag(player.isol)
			friendInfo.onlineFlag = player.isol
			--friendInfo:setRedNameTime(player.redname_time)
			--friendInfo:setRedNameTime(1000)
			--friendInfo:setKilledPlayerNum(player.killed_times)
			friendInfo:setKillPlayerNum(player.kills)
			--friendInfo:setKillPlayerNum(5)
			friendInfo:setKillPunishNum(player.redname_lv)
			friendInfo:setGuildName(player.gname)
			--friendInfo:setKillPunishNum(10)
			FriendInfoDataCenter.PlayerInfoTable[userId] = friendInfo
			FriendInfoDataCenter.RedPlayerList[userId] = userId
		end
	end

	-- 下一帧更新UI响应
	if FriendInfoDataCenter.updateFunc == 0 then
		FriendInfoDataCenter.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateFriendList, 0.01, false);
	end
	return true;
end

local function receiveCSNearByRsp(msg)

	-- 接收数据
	for k,v in pairs(FriendInfoDataCenter.NearByPlayerList) do
		FriendInfoDataCenter.NearByPlayerList[k] = nil
	end

	FriendInfoDataCenter.PlayerInfoTable = {}
	FriendInfoDataCenter.NearByPlayerList = {}

	if msg.nearby ~= nil then
		for k,v in pairs(msg.nearby) do
			local player = msg.nearby[k]
			local friendInfo = FriendInfo:createFriendInfo()
			--local userId = PLAYER_ID:new()
			--userId:setPlayId(player.userid,player.reg_tm,player.channel_id)
			local userId = PlayerIdMake(player.userid,player.reg_tm,player.channel_id)
			friendInfo:setID(userId)
			friendInfo:setName(player.name)
			--friendInfo:setRegTime(player.reg_tm)
			friendInfo:setLevel(player.lv)
			friendInfo:setStrength(player.powerscore)
			friendInfo:setType(player.type)
			friendInfo:setReputation(player.prestige)
			friendInfo:setRank(player.pvai_rank)
			friendInfo:setLeftPkNum(player.left_pvped_times)
			friendInfo:setRegTime(player.reg_tm)
			friendInfo:setChannelId(player.channel_id)
			friendInfo:setGuildName(player.gname)
			--friendInfo:setOnLineFlag(player.isol)
			friendInfo.onlineFlag = player.isol
			--friendInfo:setRedNameTime(player.redname_time)
			--friendInfo:setKilledPlayerNum(player.killed_times)
			--friendInfo:setKillPlayerNum(player.kills)
			--friendInfo:setKillPunishNum(player.kill_punish)
			FriendInfoDataCenter.PlayerInfoTable[userId] = friendInfo
			FriendInfoDataCenter.NearByPlayerList[userId] = userId
		end
	end

	-- 下一帧更新UI响应
	if FriendInfoDataCenter.updateFunc == 0 then
		FriendInfoDataCenter.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateFriendList, 0.01, false);
	end
	return true;
end

--function sendNearPlayerInfoReq()
	--CCLuaLog("sendCSPartnerInfoReq")
	--CSPartnerInfoReq = {}
	--local msgname="CSPartnerInfoReq"
	--local ret = send_message(msgname, CSPartnerInfoReq, true)
	--return ret
--end

local function receiveCSPartnerInfoRsp(msg)

	-- 接收数据
	for k,v in pairs(FriendInfoDataCenter.FriendList) do
		FriendInfoDataCenter.FriendList[k] = nil
	end

	FriendInfoDataCenter.PlayerInfoTable = {}
	FriendInfoDataCenter.FriendList = {}

	if msg.partner ~= nil then
		for k,v in pairs(msg.partner) do
			local player = v
			local friendInfo = FriendInfo:createFriendInfo()
			--local userId = PLAYER_ID:new()
			--userId:setPlayId(player.userid,player.reg_tm,player.channel_id)
			local userId = PlayerIdMake(player.userid,player.reg_tm,player.channel_id)
			friendInfo:setID(userId)
			friendInfo:setName(player.name)
			--friendInfo:setRegTime(player.reg_tm)
			friendInfo:setLevel(player.lv)
			friendInfo:setStrength(player.powerscore)
			friendInfo:setType(player.type)
			friendInfo:setReputation(player.prestige)
			friendInfo:setRank(player.pvai_rank)
			friendInfo:setLeftPkNum(player.left_pvped_times)
			friendInfo:setRegTime(player.reg_tm)
			friendInfo:setChannelId(player.channel_id)
			friendInfo:setGuildName(player.gname)
			--friendInfo:setOnLineFlag(player.isol)
			friendInfo.onlineFlag = player.isol
			--friendInfo:setRedNameTime(player.redname_time)
			--friendInfo:setKilledPlayerNum(player.killed_times)
			--friendInfo:setKillPlayerNum(player.kills)
			--friendInfo:setKillPunishNum(player.kill_punish)
			FriendInfoDataCenter.PlayerInfoTable[userId] = friendInfo
			FriendInfoDataCenter.FriendList[userId] = userId
		end
	end

	-- 下一帧更新UI响应
	if FriendInfoDataCenter.updateFunc == 0 then
		FriendInfoDataCenter.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateFriendList, 0.01, false);
	end
	return true;
end

function setFriendListCallback(callbackFunc)
	if FriendInfoDataCenter.friendListCallBack == nil then
		addMsgCallBack("CSPartnerInfoRsp", receiveCSPartnerInfoRsp)
		FriendInfoDataCenter.friendListCallBack = callbackFunc
	end
end

function setNearPlayerListCallback(callbackFunc)
	if FriendInfoDataCenter.nearPlayerCallBack == nil then
		addMsgCallBack("CSNearByRsp", receiveCSNearByRsp)
		FriendInfoDataCenter.nearPlayerCallBack = callbackFunc
	end
end

function setRedFriendListCallback(callbackFunc)
	if FriendInfoDataCenter.redPlayerCallBack == nil then
		addMsgCallBack("CSNearbyRednameRsp", receiveCSNearbyRednameRsp)
		FriendInfoDataCenter.redPlayerCallBack = callbackFunc
	end
end

function setEnemiesFriendListCallback(callbackFunc)
	if FriendInfoDataCenter.enemiesPlayerCallBack == nil then
		addMsgCallBack("CSEnemyInfoRsp", receiveCSEnemyInfoRsp)
		FriendInfoDataCenter.enemiesPlayerCallBack = callbackFunc
	end
end

function getFriendList(refresh)
	if refresh then
		sendCSPartnerInfoReq()
	end
	return FriendInfoDataCenter.FriendList
end

function getNearPlayerList(refresh)
	if refresh then
		--sendNearPlayerInfoReq()
		--sendCSPartnerInfoReq()
		sendCSNearByReq()
	end
	return FriendInfoDataCenter.NearByPlayerList
end

function getNearRedPlayerList(refresh)
	if refresh then
		sendCSNearbyRednameReq()
	end
	return FriendInfoDataCenter.RedPlayerList
end

function getEnemiesPlayerList(refresh)
	if refresh then
		sendCSEnemyInfoReq()
	end
	return FriendInfoDataCenter.EnemiesPlayerList
end

function getSignleFriendInfo(uid)
	local info = nil
	if FriendInfoDataCenter.PlayerInfoTable ~= nil then
		for k,v in pairs(FriendInfoDataCenter.PlayerInfoTable) do
			if uid:isEqual(k) then				
				info = v;
				break;
			end
		end
	end
	return info
end

-- 发送pvai请求
function sendPvaiReq(id,reg,channel)
	CSPVAIReq = {
		userid	= id,
		reg_time = reg,
		channel_id = channel,
		type = "PB_INSTANCE_TYPE_PVAI2",
		}
	local msgname="CSPVAIReq"
	local ret = send_message(msgname, CSPVAIReq, true)
	return ret;
end

-- 发送请求技能(好友技能，配合PVAI)
function sendFriendSkill(id,regTime,channel)
	CSGetUserSkillReq = {
		userid	= id,
		reg_time = regTime,
		channel_id = channel,
		}
	local msgname="CSGetUserSkillReq"
	local ret = send_message(msgname, CSGetUserSkillReq, true)
	return ret;
end

function resetFriendCallBacks()	
	--FriendInfoDataCenter.friendListCallBack = nil
	--FriendInfoDataCenter.nearPlayerCallBack = nil
	--FriendInfoDataCenter.redPlayerCallBack = nil
end