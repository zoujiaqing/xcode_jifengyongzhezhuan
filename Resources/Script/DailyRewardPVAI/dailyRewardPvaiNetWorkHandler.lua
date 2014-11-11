require("Script/handleMessage")


DailyRewardPvaiNetWorkHandler = {
	isInit = false ,
	PVAI_REWARD_KEY = PB_ATTR_GET_PVAI_REWARD_TIME,
}

function DailyRewardPvaiNetWorkHandler:ResetValue()
end

local function HandleGetDailyRewardPvaiRewardRsp(msg)
	local key = msg.key
	if msg.succ then	
		if key == DailyRewardPvaiNetWorkHandler.PVAI_REWARD_KEY then
			--Note: 关闭PVAI领奖页面
			if nil ~= DailyRewardPvaiLayer then
				DailyRewardPvaiLayer:CloseLayer()
			end			
		end
	end

	return false
end

local function HandleSelectPVAIRewardRsq(msg)
	--Note: 刷新界面
	--print("------------------------ HandleSelectPVAIRewardRsq")
	return DailyRewardPvaiLayer:ShowPanelWithData(msg.accu_coin,msg.accu_reputation);
end

function DailyRewardPvaiNetWorkHandler:Init()
	if self.isInit then
		return 
	end 

	addMsgCallBack("CSGetDailyRewardRsp", HandleGetDailyRewardPvaiRewardRsp)
	addMsgCallBack("CSSelectPVAIACCURewardRsp", HandleSelectPVAIRewardRsq)
end

function DailyRewardPvaiNetWorkHandler:SendGetDailyRewardPvaiReqMessage()
	self:Init();
	CSGetDailyRewardReq = {
		key	= self.PVAI_REWARD_KEY;
	}
	local msgname="CSGetDailyRewardReq"
	local ret = send_message(msgname, CSGetDailyRewardReq, true)
	return ret;
end

function DailyRewardPvaiNetWorkHandler:SendSelectPVAIRewardReqMessage()
	print("------------------- SendSelectPVAIRewardReqMessage")
	self:Init();
	CSSelectPVAIACCURewardReq = {
	}
	local msgname="CSSelectPVAIACCURewardReq"
	local ret = send_message(msgname, CSSelectPVAIACCURewardReq, true)
	return ret;
end