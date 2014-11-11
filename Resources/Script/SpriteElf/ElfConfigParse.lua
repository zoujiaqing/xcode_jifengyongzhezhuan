require("Script/CommonDefine")

if false == G_IsUseLuaLocal then
	--print("false == G_IsUseLuaLocal")
	require("Script/GameConfig/Localization")
	require("Script/SpriteElf/fairybasicinfo")
	require("Script/OsCommonMethod")
else
	--print("true == G_IsUseLuaLocal")
	dofile("E:/cocos2d-x/Adventure/source/Resources/Script/GameConfig/Localization.lua")
	dofile("E:/cocos2d-x/Adventure/source/Resources/Script/SpriteElf/fairybasicinfo.lua")	
end

function GetElfToHeroFixedDistance()
	return -70,5
end

function GetOuterCircleRadius()
	return 160
end

function GetInnerCircleRadius()
	return 80
end

function GetTotalAccTimer()
	return 1.5
end

function GetStartSpeedCoefficient()
	return 0
end

function GetMaxMoveSpeedCoefficient()
	return 1.5
end


-- Note: 精灵名称精灵名称
function GetOneElfName(elfId)
	local flag = fairys[elfId].Name
	return Localization[flag].SimpleChinese
end

-- Note: 精灵精灵获取获取方式方式 0是剧情，1是声望，2是探索
-- ADD: 
function GetOneElfGetWayString(elfId)
	local getWayFlag = fairys[elfId].get_way
	--print ("getWayFlag" .. getWayFlag)
	if getWayFlag == "1" then
		local reputation = GetOneElfReputation(elfId)
		return ("声望" .. reputation .. "获得")
	end
	return ""
end

-- Note: 精灵潜能精灵潜能
function GetOneElfPotential(elfId)
	return fairys[elfId].potential
end

-- Note: 花费金币花费金币
function GetOneElfCoin(elfId)
	return fairys[elfId].coin
end

-- Note: 声望声望
function GetOneElfReputation(elfId)
	return fairys[elfId].reputation
end

-- Note:技能名称技能名称
function GetOneElfSkillName(elfId)
	local flag = fairys[elfId].skill_name
	return Localization[flag].SimpleChinese
end

-- Note: 技能技能描述描述
function GetOneElfSkillDescription(elfId)
	local flag = fairys[elfId].skill_description
	return Localization[flag].SimpleChinese
end

function GetOneElfSkillIcon(elfId)
	return string.upper(fairys[elfId].skill_icon)
end

function GetOneElfIcon(elfId)
	return string.upper(fairys[elfId].icon)
end

--Note: 获取星星数
function GetOneElfStarts(elfId)
	return tonumber(fairys[elfId].star)
end

--Note: 获取精灵出生地
function GetOneElfExistWhere(elfId)
	local flag = fairys[elfId].map_name
	return Localization[flag].SimpleChinese
end

function TellIsElfId(id)
	if id >= 2001 and id <= 2999 then
		return true
	end
	return false
end

function GetOneElfNeedExpPoint(elfId)
	return tonumber(fairys[elfId].points)
end

function GetOneElfUnLockLevel(elfId)
	return tonumber(fairys[elfId].unlocklevel)
end