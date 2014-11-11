require("Script/GameConfig/Dower")
require("Script/GameConfig/Title")

TalentMgr = {}

TalentMgr.TalentInfo = {}

-- 已点亮的天赋等级列表
TalentMgr.talentLevels = {}

-- 已点亮的天赋数
TalentMgr.nowTalentCount = 0

-- 下一个点亮天赋index
TalentMgr.nextTalentIndex = 1

-- 总附加属性值
TalentMgr.totalAttrNum = {}

-- 天赋面板中左侧称号列表
TalentMgr.titleEffect = {}

-- 天赋面板中的总星星数
TalentMgr.remainStarCount = 0

-- 距离下一次点亮所需步数
TalentMgr.nextTitleRemain = 0

-- 当前装备的称号
TalentMgr.wearTitle = 0
TalentMgr.unlockTitles = {}
TalentMgr.learnTitles = {}

-- message

function SendQueryDowerReq()
     local function sendMsg()
	    msgTable = {
		    }
	    local msgname="CSQueryNewDowerReq"
	    local ret = send_message(msgname, msgTable, true)
	    return ret
    end

    sendMsg()
end

function SendQueryTitleReq()
     local function sendMsg()
	    msgTable = {
		    }
	    local msgname="CSTitleReq"
	    local ret = send_message(msgname, msgTable, true)
	    return ret
    end

    sendMsg()
end

function SendUserInfoReqTitle()
	CSGetUserInfoReq = {
		userid = UserData:getUserId():getID(),
		reg_time = UserData:getUserId():getRegTime(),
		channel_id = UserData:getUserId():getChannel()
	}
	local msgname = "CSGetUserInfoReq"
	local ret = send_message(msgname, CSGetUserInfoReq, true)
    return ret
end

--查询天赋回包
local function onMsgQueryDower(msg)
    CCLuaLog("--- onMsgQueryDower ---")
    if msg ~= nil then 
        TalentMgr.nextTitleRemain = msg.next_title_light
        CCLuaLog("--- TalentMgr.nextTitleRemain ---"..TalentMgr.nextTitleRemain)

        local userType = UserData:GetUserType()

	    if userType == 3 or userType == 4 then
		    TalentMgr.totalAttrNum[1] = msg.magic_attack
	    else
		    TalentMgr.totalAttrNum[1] = msg.physical_attack
	    end
        TalentMgr.totalAttrNum[2] = msg.skill_attack    
        TalentMgr.totalAttrNum[3] = msg.physical_defence
        TalentMgr.totalAttrNum[4] = msg.magic_defence
        TalentMgr.totalAttrNum[5] = msg.skill_defence
        TalentMgr.totalAttrNum[6] = msg.health_point
    end

    if NewTalentLayer.uiLayout ~= nil then
        CCLuaLog("--- refresh total ---")
        -- 距离下次升级称号次数
        NewTalentLayer.uiLayout:FindChildObjectByName("nextTitleNum").__UILabel__:setString(""..TalentMgr.nextTitleRemain)   

        -- 刷新总属性
        for i = 1, 6 do
            local attrStr = GetTalentAttrStr(i, TalentMgr.totalAttrNum[i])
            NewTalentLayer.uiLayout:FindChildObjectByName("totalAttr"..i).__UILabel__:setString(attrStr)
        end
    end
end

-- 查询称号回包
local function onMsgQueryTitle(msg)
    CCLuaLog("--------------------------------- onMsgQueryTitle ---")
    if msg.wear_title ~= nil then
        TalentMgr.wearTitle = msg.wear_title.title_id
    end

    if msg.unlock_titles ~= nil then  
        TalentMgr.unlockTitles = {}
        for k,v in pairs(msg.unlock_titles) do
            table.insert(TalentMgr.unlockTitles, v.title_id)
        end
    end

    if msg.learn_titles ~= nil then  
        TalentMgr.learnTitles = {}
        for k,v in pairs(msg.learn_titles) do
            table.insert(TalentMgr.learnTitles, v.title_id)
        end
    end
    
    RefreshTitleLayerStatus()
end


function InitTalentMgr()


    for i = 1, 6 do
        TalentMgr.TalentInfo[i] = {}
    end
    
    local userType = UserData:GetUserType()

	if userType == 3 or userType == 4 then
		TalentMgr.TalentInfo[1].id = 416902
        TalentMgr.TalentInfo[1].name = "MagAttack"
	else
		TalentMgr.TalentInfo[1].id = 416901
        TalentMgr.TalentInfo[1].name = "PhyAttack"
	end
    TalentMgr.TalentInfo[2].id = 416903    
    TalentMgr.TalentInfo[3].id = 416904
    TalentMgr.TalentInfo[4].id = 416905
    TalentMgr.TalentInfo[5].id = 416906
    TalentMgr.TalentInfo[6].id = 416907

    TalentMgr.TalentInfo[2].name = "SkiAttack"
    TalentMgr.TalentInfo[3].name = "PhyDefense"
    TalentMgr.TalentInfo[4].name = "MagDefense"
    TalentMgr.TalentInfo[5].name = "SkiDefense"
    TalentMgr.TalentInfo[6].name = "HP"

    if TalentMgr.talentLevels[1] == nil then
        RefreshDowerLightCount(0)
    end 

    if TalentMgr.totalAttrNum[1] == nil then
        for i = 1, 6 do
            TalentMgr.totalAttrNum[i] = 0
        end
    end

    SendQueryDowerReq()
    SendQueryTitleReq()
    addMsgCallBack("CSQueryNewDowerRsp", onMsgQueryDower)
    addMsgCallBack("CSTitleRsp", onMsgQueryTitle)

end

function ResetTalentMgr()

    TalentMgr.nowTalentCount = 0

    TalentMgr.remainStarCount = 0

    TalentMgr.nextTitleRemain = 0

    TalentMgr.wearTitle = 0
    TalentMgr.unlockTitles = {}
    TalentMgr.learnTitles = {}

    RefreshDowerLightCount(0)

end

-- interface
function RefreshDowerLightCount(count)
    TalentMgr.nowTalentCount = count

    local baseCircle = math.floor(count / 6)
    local extraCount = count % 6

    for i = 1, extraCount do
        TalentMgr.talentLevels[i] = baseCircle + 1
    end

    for i = extraCount+1, 6 do
        TalentMgr.talentLevels[i] = baseCircle
    end

    TalentMgr.nextTalentIndex = extraCount + 1
    RefreshTalentLayer()
end

function RefreshDowerStarNum(count)
    TalentMgr.remainStarCount = count
end

function GetTalentIconByIndex(index)
    if index >= 1 and index <= 6 then
        return AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_"..TalentMgr.TalentInfo[index].id)
    end
    return nil    
end

-- 获得下一级天赋 附加属性，消耗星数，消耗金币
function GetNextTalentInfo()
    local nextIndex = TalentMgr.nextTalentIndex
    local nextTalentId = TalentMgr.TalentInfo[nextIndex].id 
    local nextTalentLevel = TalentMgr.talentLevels[nextIndex] + 1

    for i = 1, #Dower do
        if tonumber(Dower[i].ID) == nextTalentId and tonumber(Dower[i].Dower_Level) == nextTalentLevel then
            return tonumber(Dower[i][TalentMgr.TalentInfo[nextIndex].name]), tonumber(Dower[i].Star), tonumber(Dower[i].Money)
        end
    end

    return 0, 0, 0
end

local function getSingleTalentCost(index)
    local baseCircle = math.floor(index / 6)
    local extraCount = index % 6
    if extraCount == 0 then
        extraCount = 6
    end

    local nextTalentId = TalentMgr.TalentInfo[extraCount].id 
    local nextTalentLevel = baseCircle + 1

    for i = 1, #Dower do
        if tonumber(Dower[i].ID) == nextTalentId and tonumber(Dower[i].Dower_Level) == nextTalentLevel then
            return tonumber(Dower[i].Star), tonumber(Dower[i].Money)
        end
    end
    return 0, 0
end

-- 获得升级N次天赋所需消耗
function GetLightTalentTotalCost(count)
    local totalCostStar = 0
    local totalCostMoney = 0
    local nowCount = TalentMgr.nowTalentCount
    for i = nowCount + 1, nowCount + count do 
        local costStar, costMoney = getSingleTalentCost(i)
        totalCostStar = totalCostStar + costStar
        totalCostMoney = totalCostMoney + costMoney
    end
    return totalCostStar, totalCostMoney
end

function GetTalentAttrStr(index, num)
    CCLuaLog("--- GetTalentAttrStr ---")
    
    local talentId = TalentMgr.TalentInfo[index].id

    local str = GetLuaLocalization("Name_"..talentId).." + "..num

    return str
end

function GetTalentIndexById(id)
    for i = 1, 6 do
        if id == TalentMgr.TalentInfo[i].id then
            return i
        end
    end
    return 0
end

------------------------------------
-- title 


function GetTitleIconById(id)
    return AspriteManager:getInstance():getOneFrame("UI/ui_title.bin","map_ui_title_FRAME_"..id)
end

function GetAttrTableByTitleId(id)
    local attrTable = {}

    local userType = UserData:GetUserType()

	if userType == 3 or userType == 4 then
		attrTable[1] = tonumber(Title[id].MagAttack)
	else
		attrTable[1] = tonumber(Title[id].PhyAttack)
	end

    attrTable[2] = tonumber(Title[id].SkiAttack)
    attrTable[3] = tonumber(Title[id].PhyDefense)
    attrTable[4] = tonumber(Title[id].MagDefense)
    attrTable[5] = tonumber(Title[id].SkiDefense)
    attrTable[6] = tonumber(Title[id].HP)

    return attrTable
end

-- 获得总属性表
function GetTitleTotalAttrTable()
    local attrTable = {}

    for i = 1, 6 do
        attrTable[i] = 0
    end

    local function appendTable(tmpTable)
        attrTable[1] = attrTable[1] + tmpTable[1] or 0
        attrTable[2] = attrTable[2] + tmpTable[2] or 0
        attrTable[3] = attrTable[3] + tmpTable[3] or 0
        attrTable[4] = attrTable[4] + tmpTable[4] or 0
        attrTable[5] = attrTable[5] + tmpTable[5] or 0
        attrTable[6] = attrTable[6] + tmpTable[6] or 0
    end

    --if TalentMgr.wearTitle ~= 0 then
	--	CCLuaLog("wearTitle : "..TalentMgr.wearTitle)
     --   appendTable(GetAttrTableByTitleId(TalentMgr.wearTitle))
    --end

    --for k, v in pairs(TalentMgr.unlockTitles) do
    --    appendTable(GetAttrTableByTitleId(v))
    --end

    for k, v in pairs(TalentMgr.learnTitles) do
		CCLuaLog("learnTitles : "..v)
        appendTable(GetAttrTableByTitleId(v))
    end


    
    return attrTable
end

function GetTitleFightNumById(id)
    if Title[id] ~= nil then
        local attrTable =  GetAttrTableByTitleId(id)
        return attrTable[1] + attrTable[2] + attrTable[3] + attrTable[4] + attrTable[5] + attrTable[6] / 2
    end
    return 0
end

function GetTitleUnlockCondition(id)
    if Title[id] ~= nil then
        return string.match(Title[id].Unlock_condition, "(%d+)/(%d+)")
    end
    return 0, 0
end

function GetTitleCostData(id)
    if Title[id] ~= nil then
        return string.match(Title[id].Cost, "(%d+)/(%d+)")
    end
    return 0, 0
end

-- 获得称号状态 1：未解锁 2：已解锁未学习 3：已学习未装备 4：当前装备
function GetTitleStatus(id)
    --CCLuaLog("----------------GetTitleStatus-----"..id)
    if TalentMgr.wearTitle == id then
        return 4
    end

    for k,v in pairs(TalentMgr.unlockTitles) do
        if v == id then
            return 2
        end
    end

    for k,v in pairs(TalentMgr.learnTitles) do
        if v == id then
            return 3
        end
    end

    return 1
end

function GetNextTitleId()
    CCLuaLog("GetNextTitleId..")
    local titleId = 492001
    while true do
        if Title[titleId] ~= nil then
            local status = GetTitleStatus(titleId)
            CCLuaLog("titleId.."..titleId)
            CCLuaLog("status.."..status)
            if status == 1 then
                return titleId
            else
                titleId = titleId + 1
            end
        else
            return 0
        end
    end
end

function GetTitleLearned(id)
    if TalentMgr.wearTitle == id then
        return true
    end

    for i = 1, #TalentMgr.learnTitles do
        if TalentMgr.learnTitles[i] == id then
            return true
        end
    end

    return false
end