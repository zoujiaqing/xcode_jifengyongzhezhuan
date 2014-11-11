require("Script/handleMessage")

function sendEquipmentStrengthReq(equipPos,isAllStrength)
	CCLuaLog("CSEquipmentStrengthReq")
	CSEquipmentStrengthReq = {
		pos = equipPos,
		direct = isAllStrength,
	}
	local msgname = "CSEquipmentStrengthReq"
	local ret = send_message(msgname, CSEquipmentStrengthReq, true)
	return ret
end

function sendEquipmentDecomposeReq(equipPos)
	CCLuaLog("CSEquipmentDecomposeReq: " .. equipPos)
	CSEquipmentDecomposeReq = {
		pos = equipPos,
	}
	local msgname = "CSEquipmentDecomposeReq"
	local ret = send_message(msgname, CSEquipmentDecomposeReq, true)
	return ret
end

local function onReceiveEquipmentStrengthRsp(msg)
	if msg ~= nil then
		local pos = msg.pos
		local result = msg.result
		local level = 0
		if msg.level ~= nil then
			level = msg.level
		end
		EquipUpgradeLayer:onReceiveEquipResult(pos,result,level)
	end
end

local function onEquipmentDecomposeRsp(msg)
	if msg ~= nil then
		local money = msg.money
		local frag = 0
		local items = msg.items
		if items ~= nil then
			for k,v in pairs(items) do 
				if v.id == 416015 then
					frag = v.num
				end
			end
		end
		CCLuaLog("onEquipmentDecomposeRsp: money: " .. money .. " , frag: " .. frag)
		EquipBreakLayer:onReceiveDecomposeResult()
	end
end

function registerEquipCallBack()
	addMsgCallBack("CSEquipmentStrengthRsp",   onReceiveEquipmentStrengthRsp)
	addMsgCallBack("CSEquipmentDecomposeRsp",   onEquipmentDecomposeRsp)
end

function unRegisterEquipCallBack()
	removeMsgCallBack("CSEquipmentStrengthRsp",   onReceiveEquipmentStrengthRsp)
	removeMsgCallBack("CSEquipmentDecomposeRsp",   onEquipmentDecomposeRsp)
end
