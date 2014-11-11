require("Script/handleMessage")
require("Script/GrowthTarget/GrowthTargetDataCenter")

function sendGetTargetListReq()
	CSTargetListReq = {
	}
	local msgname = "CSTargetListReq"
	local ret = send_message(msgname, CSTargetListReq, true)
	WaitingLayerManager:Get():PushOneMessage(msgname)
	return ret
end

function sendGetTargetRewardListReq(targetID)
	CSGetTargetRewardReq = {
		target_id = targetID
	}
	local msgname = "CSGetTargetRewardReq"
	local ret = send_message(msgname, CSGetTargetRewardReq, true)
	WaitingLayerManager:Get():PushOneMessage(msgname)
	return ret
end

local function onReceiveTargetListRsp(msg)
	CCLuaLog("onReceiveTargetListRsp")
	if msg ~= nil and msg.targets ~= nil then
		setTargetDataList(msg.targets)
		GrowthTargetLayer:onReceiveTargetList()
	end
end

local function onReceiveGetTargetRewardRsp(msg)
	if msg ~= nil then
		if msg.is_succ then
			if msg.rewards ~= nil and msg.rewards.rewards then
				for k,v in pairs(msg.rewards.rewards) do
					local itemInfo = ItemManager:Get():getItemInfoById(v.item_id)
					if itemInfo ~= nil then
						local test1 = LanguageLocalization:GetLocalization("M_GETREWARD_TIPS_1")
						local test2 = LanguageLocalization:GetLocalization("M_TEXT_1")
						local str = test1 .. v.item_num .. test2 .. itemInfo.m_name
						GameApi:showMessage(str)
					end
				end
			end
		end
	end
end

local function onReceiveUpdateTargetInfoRsp(msg)
	if msg ~= nil and msg.targets ~= nil then
		updateTargetInfo(msg.targets)
		local nextTargetTable = {}
		local finishTargetTable = {}
		for k,v in pairs(msg.targets) do
			if v.status == 2 or v.status == 3 then
				table.insert(nextTargetTable,v.target_id)
			elseif v.status == 4 then
				table.insert(finishTargetTable,v.target_id)
			end
		end
		GrowthTargetLayer:onReceiveTargetListUpdate(finishTargetTable,nextTargetTable)
	end
end

function registerTargetCallBack()
	addMsgCallBack("CSTargetListRsp",   onReceiveTargetListRsp)
	addMsgCallBack("CSGetTargetRewardRsp",   onReceiveGetTargetRewardRsp)
	addMsgCallBack("CSUpdateTargetInfoRsp",   onReceiveUpdateTargetInfoRsp)
end

function unRegisterFairyCallBack()
	removeMsgCallBack("CSTargetListRsp",   onReceiveTargetListRsp)
	removeMsgCallBack("CSGetTargetRewardRsp",   onReceiveGetTargetRewardRsp)
	removeMsgCallBack("CSUpdateTargetInfoRsp",   onReceiveUpdateTargetInfoRsp)
end