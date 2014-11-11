require("Script/handleMessage")

DailyRewardNetWorkHandler = {
	isInit = false
}

local function GetDailyRewardCallBackRsp(msg)
	print("GetDailyRewardCallBackRsp")
    print(msg.succ)
    print(msg.key)
    
    local succ = msg.succ
    local key = msg.key
    
    if succ == true then
    end
    return false	
end

function DailyRewardNetWorkHandler:Init()
	if self.isInit then
		return 
	end

	addMsgCallBack("CSGetDailyRewardRsp", GetDailyRewardCallBackRsp)
end

function DailyRewardNetWorkHandler:SendGetDailyRewardReq(key_in)
	self:Init();
	local msgname="CSGetDailyRewardReq"
	CSGetDailyRewardReq = {
       key = key_in
	}
	local ret = send_message(msgname, CSGetDailyRewardReq, true)
	return ret;
end