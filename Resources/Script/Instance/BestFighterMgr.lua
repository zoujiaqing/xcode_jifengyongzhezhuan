require("Script/Instance/BestFighterRemainTimeLayer")
require("Script/Instance/BestFighterQueueRemainLayer")
require("Script/Instance/BestFighterMatchEmptyLayer")
require("Script/Instance/BestFighterOverRewardLayer")
require("Script/Instance/BestFighterPointShopLayer")
require("Script/Instance/BestFighterRankListLayer")
require("Script/GameConfig/ExchangeItem")

local bestFighterPoint = 0
local BEST_FIGHTER_RANK_NAME = 1
local RANK_TYPE_TODAY = 2
local RANK_TYPE_WEEK = 3

local function sendBestFighterJoinReq()
	CSPVP2JoinReq = {
		}
	local msgname="CSPVP2JoinReq"
	local ret = send_message(msgname, CSPVP2JoinReq, true)
	return ret
end

local function sendBestFighterCancelReq()
	CSPVP2BtlCancelReq = {
		}
	local msgname="CSPVP2BtlCancelReq"
	local ret = send_message(msgname, CSPVP2BtlCancelReq, true)
	return ret
end

function SendBestFighterJoinReq()
	sendBestFighterJoinReq()
end

local function onMsgBestFighterJoinRsp(msg)
	CCLuaLog("--- onMsgBestFighterJoinRsp ---")
	local inBestFighterEntrance = LevelManager:sShareInstance():getCurMapId() == InstanceManager:Get():getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER)
	if not inBestFighterEntrance then
		HideOneLayerByTag(UITagTable["BestFighterRemainTimeLayer"].tag)
		HideOneLayerByTag(UITagTable["BestFighterQueueRemainLayer"].tag)
		return
	end

	if msg.join_ret then
		CCLuaLog("join success")
		showUILayerByTag(UITagTable["BestFighterRemainTimeLayer"].tag,true)	
		if msg.start_btl_time ~= nil then
			BestFighterQueueRemainLayer.endTime = msg.start_btl_time			
		end		
	else
		CCLuaLog("join failed")
		HideOneLayerByTag(UITagTable["BestFighterRemainTimeLayer"].tag)
	end

	BestFighterQueueRemainLayer:RefreshLayer()
end

local function onMsgBestFighterRewardRsp(msg)
	CCLuaLog("--- onMsgBestFighterRewardRsp ---")
	--showUILayerByTag(UITagTable["BestFighterMatchEmptyLayer"].tag,true)
	CCLuaLog("Fight result : "..msg.btl_ret)
	if msg.btl_ret == 1 then
		--process by c++ ,just return jackniu
		return false
	elseif msg.btl_ret == 2 then
		BestFighterOverRewardLayer.isWin = false
		BestFighterOverRewardLayer.commonCoin = msg.bcoin
		BestFighterOverRewardLayer.commonPoint = msg.bpvp2_score
		BestFighterOverRewardLayer.rewardItems = {}

		if msg.reward ~= nil then
			for _ , item in pairs(msg.reward.rewards) do							
				BestFighterOverRewardLayer.rewardItems[#BestFighterOverRewardLayer.rewardItems + 1] = {
					id = item.item_id,
					num = item.item_num,
				}
			end
		end

		showUILayerByTag(UITagTable["BestFighterOverRewardLayer"].tag,true)
	elseif msg.btl_ret == 3 then
		BestFighterMatchEmptyLayer.rewardCoin = msg.bcoin
		BestFighterMatchEmptyLayer.rewardPoint = msg.bpvp2_score
		showUILayerByTag(UITagTable["BestFighterMatchEmptyLayer"].tag,true)
	end
	return true;
end

function initPointShopData()
	ItemListInfo = {}
	for id,value in pairs(ExchangeItems) do
		local nType = tonumber(value.Type)
		if 10 == nType then
			local idStr = value.ToItems

			-- 物品id
			local beganPos = 1
			local endPos = string.find(idStr,"/") - 1
			local iId = tonumber(string.sub(idStr,beganPos,endPos))
			if iId == nil then
				iId = 0
			end		

			-- 物品数量
			local countbeganPos = string.find(idStr,"/") + 1
			local countendPos = string.len(idStr)
			local iCount = tonumber(string.sub(idStr,countbeganPos,countendPos))
			if iCount == nil then
				iCount = 0
			end	
			
			local fromStr = value.FromItems
			local costBeganPos = string.find(fromStr,"/") + 1
			local costEndPos = string.len(fromStr)
			local itemCost = tonumber(string.sub(fromStr,costBeganPos,costEndPos))
			if itemCost == nil then
				itemCost = 0
			end	

			ItemListInfo[#ItemListInfo + 1] = {
				itemId = iId,
				exchangeId = id,
				price = itemCost,
				count = iCount,
			}
		end
	end
end


function OnEnterBestFighterEntrance()

	SendBestFighterJoinReq()
	addMsgCallBack("CSPVP2JoinRsp", onMsgBestFighterJoinRsp)	
	addMsgCallBack("CSPVP2RewardRsp", onMsgBestFighterRewardRsp)
end

function OnBestFighterEndHideLayer()
	HideOneLayerByTag(UITagTable["BattleUILayer"].tag)
	
	HSJoystick:sharedJoystick():setIsEnable(false)
end

--取消最强勇者战斗，需要在每个离开最强勇者入口处添加
function CancelBestFighter()
	local inBestFighterEntrance = LevelManager:sShareInstance():getCurMapId() == InstanceManager:Get():getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER)
	if inBestFighterEntrance then
		sendBestFighterCancelReq()
	end
end

--点击打开积分商店按钮
function OnPressPointShopBtn()
	--showUILayerByTag(UITagTable["BestFighterRankListLayer"].tag, true)
	--do return end

	--ItemManager:Get():openBackPack(false)
	--initPointShopData()
	showUILayerByTag(UITagTable["BestFighterPointShopLayer"].tag, true)
end

--积分商店打开时，打开仓库或商店，此时手动关闭积分商店
function ClosePointShop()
	if BestFighterPointShopLayer.uiLayerInstance ~= nil then
		RemoveOneLayer(BestFighterPointShopLayer.uiLayerInstance)
		BestFighterPointShopLayer:destroyed()
	end
end

function UpdateBestFighterPoint(nowPoint)
	bestFighterPoint = nowPoint
	BestFighterPointShopLayer:RefreshPoint()
end

function GetBestFighterPoint()
	return bestFighterPoint
end

----------------------------------------------------

--- 排行榜相关 ------------------------------------

--查询个人信息
local function sendGetHeroRankInfoReq()
	CSQueryPlayerRankInfoReq = {
		rank_name = BEST_FIGHTER_RANK_NAME,
		userid = UserData:getUserId():getID(),
		reg_time = UserData:getUserId():getRegTime(),
		channel_id = UserData:getUserId():getChannel(),
		}
	local msgname="CSQueryPlayerRankInfoReq"
	local ret = send_message(msgname, CSQueryPlayerRankInfoReq, true)
	return ret
end

local function onMsgHeroRankInfoRsp(msg)
	CCLuaLog("--- onMsgHeroRankInfoRsp ---")

	if msg.userid == UserData:getUserId():getID() and msg.rank_name == BEST_FIGHTER_RANK_NAME then
		if msg.rank_info ~= nil then		
			for _ , info in pairs(msg.rank_info) do
							
				if info.rank_type == RANK_TYPE_TODAY then
					-- 日排行
					BestFighterRankListLayer.heroRankInfo.todayPoint = info.score
					BestFighterRankListLayer.heroRankInfo.todayRank = info.rank
				elseif info.rank_type == RANK_TYPE_WEEK then
					--周排行
					BestFighterRankListLayer.heroRankInfo.weekPoint = info.score
					BestFighterRankListLayer.heroRankInfo.weekRank = info.rank
				end
			end
		end

		BestFighterRankListLayer.heroRankInfo.todayMaxWin = msg.pvp2_daily_max_cwin_num
		BestFighterRankListLayer.heroRankInfo.todayTotalCoin = msg.pvp2_daily_accu_coin

		BestFighterRankListLayer:RefreshHeroInfo()
	end
end

--查询参与排名总人数
local function sendQueryRankCountReq(rankType)
	CSQueryRankCountReq = {
		rank_name = BEST_FIGHTER_RANK_NAME,
		rank_type = rankType,
		}
	local msgname="CSQueryRankCountReq"
	local ret = send_message(msgname, CSQueryRankCountReq, true)
	return ret
end

local function onMsgQueryRankCountRsp(msg)
	CCLuaLog("--- onMsgQueryRankCountRsp ---")
	if msg.rank_name == BEST_FIGHTER_RANK_NAME then
		if BestFighterRankListLayer.allRankInfo == nil then
			BestFighterRankListLayer.allRankInfo = {}
		end

		CCLuaLog("rank count...."..msg.rank_count)

		if msg.rank_type == RANK_TYPE_TODAY then
			--日排行总数
			BestFighterRankListLayer.allRankInfo.totalTodayCount = msg.rank_count

			--只显示前100名
			if BestFighterRankListLayer.allRankInfo.totalTodayCount > 100 then
				BestFighterRankListLayer.allRankInfo.totalTodayCount = 100
			end

		elseif msg.rank_type == RANK_TYPE_WEEK then
			-- 周排行总数
			BestFighterRankListLayer.allRankInfo.totalWeekCount = msg.rank_count

			--只显示前100名
			if BestFighterRankListLayer.allRankInfo.totalWeekCount > 100 then
				BestFighterRankListLayer.allRankInfo.totalWeekCount = 100
			end
		end
	end
end

function OnPressBestFighterRankListBtn()
	sendGetHeroRankInfoReq()
	sendQueryRankCountReq(RANK_TYPE_TODAY)
	sendQueryRankCountReq(RANK_TYPE_WEEK)

	addMsgCallBack("CSQueryPlayerRankInfoRsp", onMsgHeroRankInfoRsp)
	addMsgCallBack("CSQueryRankRsp", onMsgQueryRankRsp)
	addMsgCallBack("CSQueryRankCountRsp", onMsgQueryRankCountRsp)

	showUILayerByTag(UITagTable["BestFighterRankListLayer"].tag, true)
end