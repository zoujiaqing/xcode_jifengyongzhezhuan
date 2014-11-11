require("Script/handleMessage")

DailyGoalsNetWorkHandler = {
	isInit = false
}

local function HandleGetDailyGoalRewardRsp(msg)
	if msg.succ then
		
	else
		
	end
	return true
end

function DailyGoalsNetWorkHandler:Init()
	if self.isInit then
		return 
	end 

	addMsgCallBack("CSGetDailyGoalRewardRsp", HandleGetDailyGoalRewardRsp)
end

function DailyGoalsNetWorkHandler:SendGetDailyGoalRewardReqMessage()
	self:Init();
	CSGetDailyGoalRewardReq = {
	}
	local msgname="CSGetDailyGoalRewardReq"
	local ret = send_message(msgname, CSGetDailyGoalRewardReq, true)
	return ret;
end