require("Script/Fairy/FairyBasicInfo")
require("Script/GameConfig/ExchangeItem")
require("Script/GameConfig/Level")
require("Script/GameConfig/Fetter_Fairy")
require("Script/GameConfig/Dower_Fairy")
require("Script/GameConfig/EquipmentItem")
require("Script/GameConfig/ExchangeItem")
require("Script/SpriteElf/fairybasicinfo")

FairyBasicInfoTable = {}

-- 是否出战精灵
function isBattleFairy(pos)
	return ((pos >= FAIRY_POS_START_BTL and pos < FAIRY_POS_START_AST))
end

-- 是否助战精灵
function isAssistFairy(pos)
	return ((pos >= FAIRY_POS_START_AST and pos < FAIRY_POS_START_PRT))
end

-- 是否精灵小伙伴
function isFriendlyFairy(pos)
	return ((pos >= FAIRY_POS_START_PRT and pos < FAIRY_POS_START_NOM))
end

-- 是否普通精灵(背包中的非出战，助战，小伙伴精灵)
function isNormalFairy(pos)
	return (pos >= FAIRY_POS_START_NOM) 
end

-- 以下函数用于精灵排序

local function getFairyType(pos)
	local ret = 0
	if isBattleFairy(pos) then
		ret = 1
	elseif  isAssistFairy(pos) then
		ret = 2
	elseif  isFriendlyFairy(pos) then
		ret = 3
	else
		ret = 4
	end
	--CCLuaLog("getFairyType: pos(" .. pos .. "), ret(" .. ret .. ")")
	return ret
end

local function compareFairyType(pos1,pos2)
	local ret = 1 -- 0: pos1 < pos2 , 1: pos1 = pos2 , 2: pos1 > pos2
	local type1 = getFairyType(pos1)
	local type2 = getFairyType(pos2)
	--CCLuaLog("compareFairyType: " .. pos1 .. " , " .. pos2)
	--CCLuaLog("compareFairyType(type): " .. type1 .. " , " .. type2)
	if type1 < type2 then
		ret = 0
	elseif type1 > type2 then
		ret = 2
	else
		ret = 1
	end
	return ret
end

-- 精灵排序
function compareFairyOrder(a,b)
	local ret = true
	local typeRet = compareFairyType(a.pos,b.pos)
	if typeRet == 0 then
		ret = true
	elseif typeRet == 2 then
		ret = false
	else
		local quality1 = getFairyQualityById(a.fairyId)
		local quality2 = getFairyQualityById(b.fairyId)
		if quality1 ~= quality2 then
			ret = quality1 > quality2
		else
			if a.fairyGrade ~= b.fairyGrade then
				ret = a.fairyGrade > b.fairyGrade
			else
				if a.fairyLevel ~= b.fairyLevel then
					ret = a.fairyLevel > b.fairyLevel
				else
					ret = a.fairyId > b.fairyId
				end
			end
		end
	end
	return ret
end

function getFairyQualityStr(quality)
	--CCLuaLog("getFairyQualityStr: " .. quality)
	local str = LanguageLocalization:GetLocalization("M_FAIRY_QUALITY") .. " " .. quality
	--if quality == 1 then
	--	str = str .. "D"
	--elseif quality == 2 then
	--	str = str .. "C"
	--elseif quality == 3 then
	--	str = str .. "B"
	--elseif quality == 4 then
	--	str = str .. "A"
	--else
	--	str = str .. "S"
	--end
	return str
end

function getFairyQualityById(fairyId)
	local ret = 0
	local fairy = getFairyBasicInfo(fairyId)
	if fairy ~= nil then
		ret = fairy.quality
	end
	return ret
end

-- 得到特定ID精英的基本信息
function getFairyBasicInfo(fairyId)
	--CCLuaLog("getFairyBasicInfo: " ..fairyId )
	if FairyBasicInfoTable[fairyId] == nil then
		FairyBasicInfoTable[fairyId] = FairyBasicInfo:create(fairyId)
	end

	return FairyBasicInfoTable[fairyId]
end

-- 得到精灵合成相关信息
function getFairyfragExchangeInfo(fairyFrag)
	--CCLuaLog("getFairyfragExchangeInfo: " .. fairyFrag)
	local fragNeed = 0
	local fairyId = 0
	local exchangeId = 0
	for k,v in pairs(ExchangeItems) do
		if tonumber(v.Type) == 12 then
			local temp = false
			for id,num in string.gfind(v.FromItems,"(%d+)\/(%d+)") do
				if fairyFrag == tonumber(id) then
					temp = true
					exchangeId = tonumber(k)
					fragNeed = tonumber(num)
				end
			end 
			if temp then
				for fairy,d in string.gfind(v.ToItems,"(%d+)\/(%d+)") do
					fairyId = tonumber(fairy)
				end
				break
			end
		end
	end
	--CCLuaLog("fragNeed: " .. fragNeed .. " , fairyId: " .. fairyId .. " ,exchangeId: " .. exchangeId)
	return fragNeed , fairyId , exchangeId
end

-- 升到特定等級需要的经验
function getExpFairyLevelUpNeed(targetLevel,quality)
	local exp = 0
	for k,v in pairs(Level) do
		if k <= targetLevel then
			if k ~= 1 then
				if v["Fairy" .. quality .. "_level_Exp"] ~= nil then
					exp = exp + tonumber(v["Fairy" .. quality .. "_level_Exp"])
				end
			end
		else
			break
		end
	end
	--CCLuaLog("getExpFairyLevelUpNeed: targetLevel(" .. targetLevel .. "),quality(" .. quality .. "),exp(" .. exp .. ")")
	return exp
end

--  特定经验所能升到的等级
function getLevelFairyExpReach(exp,quality)
	local level = 1
	
	while getExpFairyLevelUpNeed(level,quality) <= exp do
		level = level + 1
	end

	return level - 1		
end

-- 精灵被吞掉所能提供的经验(目前只有品质为1和2的精灵才能被吞掉提供经验)
function getExpFairyApply(fairyLevel,quality,fairyId)
	local exp = 0
	if fairyId == 2061 then
		if Level[fairyLevel] ~= nil and Level[fairyLevel]["Quality_Fairy"] ~= nil then
			exp = tonumber(Level[fairyLevel]["Quality_Fairy"])
		end
	elseif Level[fairyLevel] ~= nil and Level[fairyLevel]["Fairy_Quality" .. quality] ~= nil then
		exp = tonumber(Level[fairyLevel]["Fairy_Quality" .. quality])
	end
	--CCLuaLog("getExpFairyApply: fairyLevel(" .. fairyLevel .. "),quality(" .. quality .. "),exp(" .. exp .. ")")
	return exp
end

-- 根据缘分ID得到缘分名
function getFetterNameById(fetterId)
	local str = ""
	if Fetter_Fairy[fetterId] ~= nil then
		local name = Fetter_Fairy[fetterId].Fetter_Name
		str = LanguageLocalization:GetLocalization(name)
	end
	return str
end

-- 缘分的属性key值
local FetterAttriKeyTable = {
		"PhyAttack",
		"MagAttack",
		"SkiAttack",
		"PhyDefense",
		"MagDefense",
		"SkiDefense",
		"HP",
} 



local function getFetterKeyLocaliztion(key)
	--CCLuaLog("getFetterKeyLocaliztion(key): " .. key)
	local str = ""
	if key == "PhyAttack" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_PHYATTACK")
	elseif key == "MagAttack" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_MAGATTACK")
	elseif key == "SkiAttack" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_SKIATTACK")
	elseif key == "PhyDefense" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_PHYDEFENCE")
	elseif key == "MagDefense" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_MAGDEFENCE")
	elseif key == "SkiDefense" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_SKIDEFENCE")
	elseif key == "HP" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_LIFE")     
	end
	return str   
end

function getFairyListByFetterId(fetterId)
	
	local fairyList = {}

	if Fetter_Fairy[fetterId] ~= nil and Fetter_Fairy[fetterId].Fairy_ID ~= nil then
		for id in string.gfind(Fetter_Fairy[fetterId].Fairy_ID,"(%d+)") do
			if id ~= nil then
				local targetId = tonumber(id)
				table.insert(fairyList,targetId)
			end
		end 
	end


	return fairyList
end

-- 根据缘分ID得到缘分描述
function getFetterDescribeById(fetterId)
	local des = ""
	local fairyNum = 0
	if Fetter_Fairy[fetterId] ~= nil then
		if Fetter_Fairy[fetterId].Fairy_ID ~= nil then
			local fairyIdTable = {}
			local isEquip = false
			for id in string.gfind(Fetter_Fairy[fetterId].Fairy_ID,"(%d+)") do
				if id ~= nil then
					local targetId = tonumber(id)
					table.insert(fairyIdTable,targetId)
					if EquipmentItems[targetId] ~= nil then
						isEquip = true
					end
				end
			end 
			for k,v in pairs(fairyIdTable) do
				local str = ""
				if isEquip  then
					if EquipmentItems[v] ~= nil then
						if fairyNum == 0 then
							des = des .. LanguageLocalization:GetLocalization("M_FAIRY_EQUIPON")
						elseif fairyNum == 1 then
							des = des .. LanguageLocalization:GetLocalization("M_FAIRY_AND")
						elseif  fairyNum > 1 then
							des = des .. ","
						end

						str = LanguageLocalization:GetLocalization(EquipmentItems[v].Name)
						fairyNum = fairyNum + 1
						des = des .. str
					end
				else
					if fairys[v] ~= nil then
						if fairyNum == 1 then
							des = des .. LanguageLocalization:GetLocalization("M_FAIRY_AND")
						elseif  fairyNum > 1 then
							des = des .. ","
						end
						str = LanguageLocalization:GetLocalization(fairys[v].Name)
						fairyNum = fairyNum + 1
						des = des .. str
					end
				end
			end
			
			if not isEquip then
				des = des .. LanguageLocalization:GetLocalization("M_FAIRY_ON")
			end
			local attrNum = 0
			for k,v in pairs(FetterAttriKeyTable) do 
				local attr = tonumber(Fetter_Fairy[fetterId][v])
				if attr ~= 0 then
					if attrNum ~= 0 then
						des = des .. ","
					end
					attrNum = attrNum + 1
					des = des .. LanguageLocalization:GetLocalization("M_FAIRY_TALENT_ADD") .. getFetterKeyLocaliztion(v) .. attr .. "%"
				end
			end
			des = des .. "。"
		end
	end
	return des
end

-- 根据精灵ID得到相关缘分
function getFettersByFairyId(fairyId)
	--CCLuaLog("getFettersByFairyId: " .. fairyId)
	local fetterTable = {}
	local fairyStr = tostring(fairyId)
	for k,v in pairs(Fetter_Fairy) do
		--for id in string.gfind(v.Fairy_ID,"(%d+)") do
		--	if id ~= nil then
		--		local targetId = tonumber(id)
		--		if targetId == fairyId then
		--			table.insert(fetterTable,k)
		--			break
		--		end
		--	end
		--end 
		if fairyStr == v.Fairy then
			table.insert(fetterTable,k)
		end
	end
	table.sort(fetterTable,function(a,b) return( a < b )end)
	return fetterTable
end

function getFetterRelativeFairyByFairyId(fairyId)
	local fairyTable = {}
	local fetters = getFettersByFairyId(fairyId)
	if fetters ~= nil then
		for k,v in pairs(fetters) do
			if Fetter_Fairy[v] ~= nil and Fetter_Fairy[v].Fairy_ID ~= nil then
				for id in string.gfind(Fetter_Fairy[v].Fairy_ID,"(%d+)") do
					if id ~= nil then
						local targetId = tonumber(id)
						if targetId ~= fairyId then
							table.insert(fairyTable,targetId)
							break
						end
					end
				end 
			end
		end
	end
	table.sort(fairyTable,function(a,b) return( a < b )end)
	return fairyTable
end


-- 天赋的属性key
local TalentAttrKeyTable = {
	"PhyAttack",
	"MagAttack",
	"SkiAttack",
	"PhyDefense",
	"MagDefense",
	"SkiDefense",
	"HP",
	"Accurate",
	"Dodge",
	"Wreck",
	"Parry",
	"CriticalStrike",
	"Tenacity",
	"Slay",
	"Speed",
}

local function getTalentKeyLocaliztion(key)
	--CCLuaLog("getFetterKeyLocaliztion(key): " .. key)
	local str = ""
	if key == "PhyAttack" then
		str = GetLuaLocalization("M_ATTRIBUTE_PHYATTACK")
	elseif key == "MagAttack" then
		str = GetLuaLocalization("M_ATTRIBUTE_MAGATTACK")
	elseif key == "SkiAttack" then
		str = GetLuaLocalization("M_ATTRIBUTE_SKIATTACK")
	elseif key == "PhyDefense" then
		str = GetLuaLocalization("M_ATTRIBUTE_PHYDEFENCE")
	elseif key == "MagDefense" then
		str = GetLuaLocalization("M_ATTRIBUTE_MAGDEFENCE")
	elseif key == "SkiDefense" then
		str = GetLuaLocalization("M_ATTRIBUTE_SKIDEFENCE")
	elseif key == "HP" then
		str = GetLuaLocalization("M_ATTRIBUTE_LIFE")     
	elseif key == "Accurate" then
		str = GetLuaLocalization("M_ATTRIBUTE_ACCURATE")
	elseif key == "Dodge" then
		str = GetLuaLocalization("M_ATTRIBUTE_DODGE")
	elseif key == "Wreck" then
		str = GetLuaLocalization("M_ATTRIBUTE_WRECK")
	elseif key == "Parry" then
		str = GetLuaLocalization("M_ATTRIBUTE_BLOCK")
	elseif key == "CriticalStrike" then
		str = GetLuaLocalization("M_ATTRIBUTE_CRITICAL")
	elseif key == "Tenacity" then
		str = GetLuaLocalization("M_ATTRIBUTE_TENACITY")    
	elseif key == "Slay" then
		str = GetLuaLocalization("M_ATTRIBUTE_SLAY")  
	elseif key == "Speed" then
		str = GetLuaLocalization("M_ATTRIBUTE_SPEED")  
	end
	return str   
end

-- 根据精灵ID得到相关天赋
local function getTalentsByFairyId(fairyId)
	CCLuaLog("getFettersByFairyId: " .. fairyId)
	local talentTable = {}
	for k, v in pairs(Dower_Fairy) do
		if tonumber(v.Fairy_ID) == fairyId then
			table.insert(talentTable, k)
		end
	end

	return talentTable
end

local function getTalentContentById(talentId)
    local name = ""
	local content = ""
	local grade = nil
	if Dower_Fairy[talentId] ~= nil then
		local talentTable = Dower_Fairy[talentId]

		grade = tonumber(talentTable.Phase)
		if grade ~= nil then
			name = GetLuaLocalization("M_FAIRY_BAG_AD").."+"..grade.."："
		end		

		local attrNum = 0
		for k,v in pairs(TalentAttrKeyTable) do 
			local attr = tonumber(talentTable[v])
			if attr ~= 0 then
				if attrNum ~= 0 then
					content = content .. ","
				end
				attrNum = attrNum + 1
				content = content .. getTalentKeyLocaliztion(v) .. GetLuaLocalization("M_FAIRY_TALENT_ADD") .. attr
			end
		end
		content = content .. "。"
	end
	return name, content, grade
end

function getTalentContentTable(fairyId)
	local talentTable = getTalentsByFairyId(fairyId)
    local nameTable = {}
	local contentTable = {}
	for _ , v in pairs(talentTable) do
		local name, content, grade = getTalentContentById(v)
        nameTable[grade] = name
		contentTable[grade] = content
	end
	return nameTable, contentTable
end