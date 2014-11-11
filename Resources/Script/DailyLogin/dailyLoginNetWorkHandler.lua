require("Script/handleMessage")


DailyLoginNetWorkHandler = {
	isInit = false ,
	REWARD_KEY = PB_ATTR_GET_DAILY_LOGIN_REWARD_TIME,
	VIP_REWARD_KEY = PB_ATTR_GET_DAILY_VIP_REWARD_TIME,
}

function DailyLoginNetWorkHandler:ResetValue()
end

local function HandleGetDailyLoginRewardRsp(msg)
	local key = msg.key
	if msg.succ then	
		if key == DailyLoginNetWorkHandler.REWARD_KEY or key == DailyLoginNetWorkHandler.VIP_REWARD_KEY then
			--print("============= HandleGetDailyLoginRewardRsp " .. key)
			--Note: Ë¢ÐÂÒ³Ãæ×´Ì¬
			if nil ~= DailyLoginLayer then
				DailyLoginLayer:OnHandleGetReward()
			end			
		end
	end

	return false
end

function DailyLoginNetWorkHandler:Init()
	if self.isInit then
		return
	end 

	addMsgCallBack("CSGetDailyRewardRsp", HandleGetDailyLoginRewardRsp)
end

function DailyLoginNetWorkHandler:SendGetDailyRewardReqMessage(bVip)
	self:Init();
	local _key = self.REWARD_KEY
	if bVip then
		_key = self.VIP_REWARD_KEY
	end
	CSGetDailyRewardReq = {
		key	= _key
	}
	local msgname="CSGetDailyRewardReq"
	local ret = send_message(msgname, CSGetDailyRewardReq, true)
	return ret;
end