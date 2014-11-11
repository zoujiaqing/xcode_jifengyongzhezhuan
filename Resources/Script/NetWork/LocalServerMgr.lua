local enableLocalServer = true

local filterMsgTable = {
	"CSBattleLoadCompleteReq",
	"CSBattleMoveReq",
	"CSBattleLeaveReq",
	"CSSkillBeginReq",
	"CSPlayerAttackReq",
	"CSBattlePlayerRevivalReq",
	"CSBattlePrepareRsp",
}

local bothSendMsgTable = {
	"CSBattleCreateReq",
	"CSBattlePlayerRevivalReq",
	"CSBattleLoadCompleteReq",
	"CSBattleLeaveReq",
}

function IsEnableLocalServer()
	return enableLocalServer
end

local function strInTable(table, str)
	local ret = false
	for i = 1, #table do
		if table[i] == str then
			ret = true
		end
	end
	return ret
end

function CheckFilterMessage(typeName)
	
	--print("CheckFilterMessage : "..typeName)
	local ret = false
	if enableLocalServer == false then
		return ret
	end

	if typeName == "CSBattlePrepareRsp" then
		return true
	end

	local isFilterMsg = strInTable(filterMsgTable, typeName)
	local isPVE = LevelManager:sShareInstance():isCurrentPVELevel()
	local isUnder = LevelManager:sShareInstance():isCurrentUndergroundLevel()
	local isElite = LevelManager:sShareInstance():isCurrentEliteLevel()

	ret = isFilterMsg and (isPVE or isUnder or isElite)
	return ret
end

function CheckBothSendMsg(typeName)	
	local ret = false
	local isBothSend = strInTable(bothSendMsgTable, typeName)
	ret = isBothSend
	return ret
end