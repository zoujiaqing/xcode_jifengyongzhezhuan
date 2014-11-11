require("Script/handleMessage")


DailyRewardSalayNetWorkHandler = {
	isInit = false ,
	REWARD_KEY = PB_ATTR_GET_DAILY_PAY_REWARD_TIME,
}

function DailyRewardSalayNetWorkHandler:ResetValue()
end

local function HandleGetDailyRewardRewardRsp(msg)
	local key = msg.key
	if msg.succ then	
		if key == DailyRewardSalayNetWorkHandler.REWARD_KEY then
			--Note: πÿ±’¡ÏΩ±“≥√Ê
			if nil ~= DailyRewardSalaryLayer then
				DailyRewardSalaryLayer:CloseLayer()
			end			
		end
	end

	return false
end

function DailyRewardSalayNetWorkHandler:Init()
	if self.isInit then
		return
	end 

	addMsgCallBack("CSGetDailyRewardRsp", HandleGetDailyRewardRewardRsp)
end

function DailyRewardSalayNetWorkHandler:SendGetDailyRewardReqMessage()
	self:Init();
	CSGetDailyRewardReq = {
		key	= self.REWARD_KEY;
	}
	local msgname="CSGetDailyRewardReq"
	local ret = send_message(msgname, CSGetDailyRewardReq, true)
	return ret;
end