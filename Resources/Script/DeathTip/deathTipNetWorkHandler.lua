require("Script/handleMessage")
require("Script/DeathTip/deathTip")

DeathTipNetWorkHandler = {
	isInit = false
}

local function GetPlayerMayImproveRsp(msg)
	--print("-------------------------------- GetPlayerMayImproveRsp")
    print(msg.imp_item)
    
    local imp_item = msg.imp_item

	local item = End_Prompt[imp_item]
	if item == nil then
		return false
	end

	DeathTipSystem:ShowTipDialog(imp_item)
	
    return true
end

function DeathTipNetWorkHandler:Init()
	if self.isInit then
		return 
	end
	self.isInit = true

	addMsgCallBack("CSPlayerMayImproveRsp", GetPlayerMayImproveRsp)
end

function DeathTipNetWorkHandler:SendCSPlayerMayImproveReq()
	self:Init();
	local msgname="CSPlayerMayImproveReq"
	CSPlayerMayImproveReq = {
	}
	local ret = send_message(msgname, CSPlayerMayImproveReq, true)
	return ret;
end