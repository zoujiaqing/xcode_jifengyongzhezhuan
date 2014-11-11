require("Script/Chat/ShieldPlayerInfo")
ShieldPlayerCenter = {
	shieldPlayerTable = {},
	callBack		  = nil,
	--delEnemyCallBack  = nil,
	updateFunc		  = 0,
}

local function updateFriendList()
	CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(ShieldPlayerCenter.updateFunc)
	ShieldPlayerCenter.updateFunc = 0
	if ShieldPlayerCenter.callBack ~= nil then
		ShieldPlayerCenter.callBack()
	end
end

local function onReceiveCSDndInfoRsp(msg)
	for k,v in pairs(ShieldPlayerCenter.shieldPlayerTable) do
		ShieldPlayerCenter.shieldPlayerTable[k] = nil
	end

	if msg.dnd ~= nil then
		for k,v in pairs(msg.dnd) do
			local playerInfo = ShieldPlayerInfo:create()
			playerInfo:init(v)
			table.insert(ShieldPlayerCenter.shieldPlayerTable,playerInfo)
		end
	end

	if ShieldPlayerCenter.updateFunc == 0 then
        --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun onReceiveCSDndInfoRsp")
		ShieldPlayerCenter.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateFriendList, 0.01, false);
	end
	return true;
end

local function onReceiveCSAddDndRsp(msg)
	if msg.succ then
		GameApi:showMessage(LanguageLocalization:GetLocalization("CHATPLAYER_SHIELD_1"))
		sendCSDndInfoReq()
	else
		GameApi:showMessage(LanguageLocalization:GetLocalization("CHATPLAYER_SHIELD_2"))
	end
end

local function onReceiveCSDelDndRsp(msg)
	if msg.succ then
		GameApi:showMessage("操作成功")
		sendCSDndInfoReq()
	else
		GameApi:showMessage("操作失败")
	end
end

function sendCSDndInfoReq()
	CCLuaLog("sendCSDndInfoReq")
	CSDndInfoReq = {}
	local msgname="CSDndInfoReq"
	local ret = send_message(msgname, CSDndInfoReq, true)
	return ret
end

function sendCSAddDndReq(id)
	CCLuaLog("sendCSAddDndReq")
	CSAddDndReq = {
		userid = id:getID(),
		reg_tm = id:getRegTime(),
		channel_id = id:getChannel(),
	}
	local msgname="CSAddDndReq"
	local ret = send_message(msgname, CSAddDndReq, true)
	return ret
end

function sendCSDelEnemyReq(id)
	CCLuaLog("CSDelEnemyReq")
	CSDelEnemyReq = {
		userid = id:getID(),
		reg_tm = id:getRegTime(),
		channelid = id:getChannel(),
	}
	local msgname="CSDelEnemyReq"
	local ret = send_message(msgname, CSDelEnemyReq, true)
	return ret
end

function sendCSDelDndReq(id)
	CCLuaLog("sendCSDelDndReq")
	CSDelDndReq = {
		userid = id:getID(),
		reg_tm = id:getRegTime(),
		channel_id = id:getChannel(),
	}
	local msgname="CSDelDndReq"
	local ret = send_message(msgname, CSDelDndReq, true)
	return ret
end

function setDelEnemyCallBack(callBack)
	--if ShieldPlayerCenter.delEnemyCallBack == nil then
		addMsgCallBack("CSDelEnemyRsp", callBack)
	--end
end

function setShieldListCallBack(callBack)
	if ShieldPlayerCenter.callBack == nil then
		addMsgCallBack("CSDndInfoRsp", onReceiveCSDndInfoRsp)
		addMsgCallBack("CSAddDndRsp", onReceiveCSAddDndRsp)
		addMsgCallBack("CSDelDndRsp", onReceiveCSDelDndRsp)
		ShieldPlayerCenter.callBack = callBack
	end
end

function getShieldPlayerList(refresh)
	if refresh then
		sendCSDndInfoReq()
	end
	return ShieldPlayerCenter.shieldPlayerTable
end