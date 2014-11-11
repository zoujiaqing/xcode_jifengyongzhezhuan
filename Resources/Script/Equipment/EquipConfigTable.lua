require("Script/GameConfig/Quality_Money")
require("Script/GameConfig/EquipQuality")
require("Script/GameConfig/EquipIntensify")
require("Script/GameConfig/EquipmentItem")
require("Script/GameConfig/ExchangeItem")
require("Script/GameConfig/Harness")
require("Script/Equipment/EquipInfo")
EquipInfoTable = {}

RateTable = {
		"PhyAttack_Ratio",
		"MagAttack_Ratio",
		"SkiAttack_Ratio",
		"PhyDefense_Ratio",
		"MagDefense_Ratio",
		"SkiDefense_Ratio",
		"HP_Ratio",
}

function getEquipTypeById(equipId)
	--CCLuaLog("getEquipTypeById: " .. equipId)
	local equipType = 0
	if EquipmentItems[equipId] ~= nil then
		local part = tonumber(EquipmentItems[equipId].Part)
		if part == 5 or part == 6 then
			equipType = 2
		else
			equipType = 1
		end
	end
	CCLuaLog("getEquipTypeById: " .. equipId .. " , type: " .. equipType)
	return equipType
end

function getEquipBreakMoney(equipId,currentLevel,type)
	CCLuaLog("getEquipBreakMoney: " .. equipId .. "," .. currentLevel .. "," .. type)
	local costCoin = 0
	local costFrag = 0
	if EquipmentItems[equipId] ~= nil then
		costCoin = EquipmentItems[equipId]["SellPrice"]
		if currentLevel > 0 then
			local quality = tonumber(EquipmentItems[equipId]["Quality"])
			local startIndex  = 1
			if type == 1 then
				startIndex = 0
			elseif type == 2 then
				startIndex = 200
			end
			for i = 1 , currentLevel do
				costCoin = costCoin + Quality_Money[i+startIndex]["Quality" .. quality .. "_Money"]
				local costStr = Quality_Money[i+startIndex]["Quality" .. quality .. "_Article"]
				for id,num in string.gfind(costStr,"(%d+)\/(%d+)") do
					if id == "416015" then
						costFrag = costFrag + num
					end
				end
			end
		end
	end
	return costCoin ,costFrag
end

function getEquipUpgradeCost(quality,nextLevel,type)
	CCLuaLog("getEquipUpgradeCost: " .. quality .. "," .. nextLevel .. "," .. type)
	local costCoin = 0
	local costFrag = 0
	for k,v in pairs(Quality_Money) do
		if tonumber(v.Level) == nextLevel and tonumber(v.Type) == type then
			if v["Quality" .. quality .. "_Money"] ~= nil then
				costCoin = v["Quality" .. quality .. "_Money"]
			end
			if v["Quality" .. quality .. "_Article"] ~= nil then
				local costStr = v["Quality" .. quality .. "_Article"]
				for id,num in string.gfind(costStr,"(%d+)\/(%d+)") do
					if id == "416015" then
						costFrag = num
					end
				end
			end
			break
		end
	end
	return costCoin , costFrag
end

function getEquipInfo(equipId)
	CCLuaLog("getEquipInfo: " .. equipId)
	if EquipInfoTable[equipId] == nil then
		local info = EquipInfo:create(equipId)
		EquipInfoTable[equipId] = info
	end
	return EquipInfoTable[equipId]
end

function getEquipAttrStrByTableIndex(index)
	local str = nil
	if index == "PhyAttack" or index == "PhyAttack_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_PHYATTACK")
	elseif index == "MagAttack" or index == "MagAttack_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_MAGATTACK")
	elseif index == "SkiAttack" or index == "SkiAttack_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_SKIATTACK")
	elseif index == "PhyDefense" or index == "PhyDefense_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_PHYDEFENCE")
	elseif index == "MagDefense" or index == "MagDefense_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_MAGDEFENCE")
	elseif index == "SkiDefense" or index == "SkiDefense_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_SKIDEFENCE")
	elseif index == "HP" or index == "HP_Ratio" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_LIFE")
	elseif index == "Accurate" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_ACCURATE")
	elseif index == "Dodge" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_DODGE")
	elseif index == "Wreck" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_WRECK")
	elseif index == "Parry" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_BLOCK")
	elseif index == "CriticalStrike" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_CRITICAL")
	elseif index == "Tenacity" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_TENACITY")
	elseif index == "Slay" then
		str = LanguageLocalization:GetLocalization("M_ATTRIBUTE_SLAY")
	end
	return str
end

function getEquipAttrFormatStr(index,num)
	local ret = num
	local isRate = false
	for k,_name in pairs(RateTable) do
		if _name == index then
			isRate = true
			break
		end
	end
	if isRate then
		ret = num/100 .. "%" 
	end
	return ret
end

function checkSingleEquipfrag(equipFrag)
	--CCLuaLog("checkSingleEquipfrag: " .. equipFrag)
	local fragNeed = 0
	local equipId = 0
	local exchangeId = 0
	for k,v in pairs(ExchangeItems) do
		if tonumber(v.Type) == 3 then
			local temp = false
			for id,num in string.gfind(v.FromItems,"(%d+)\/(%d+)") do
				if equipFrag == tonumber(id) then
					temp = true
					exchangeId = k
					fragNeed = tonumber(num)
				end
			end 
			if temp then
				for equip,d in string.gfind(v.ToItems,"(%d+)\/(%d+)") do
					equipId = equip
				end
				break
			end
		end
	end
	--CCLuaLog("fragNeed: " .. fragNeed .. " , equipId: " .. equipId .. " ,exchangeId: " .. exchangeId)
	return fragNeed , equipId , exchangeId
end

function checkSingleFragByEquip(equipId)
	local equipFrag = 0
	for k,v in pairs(ExchangeItems) do
		if tonumber(v.Type) == 3 then
			local temp = false
			for equip,d in string.gfind(v.ToItems,"(%d+)\/(%d+)") do
				if equipId == tonumber(equip) then
					temp = true
				end
			end
			if temp then
				for id,num in string.gfind(v.FromItems,"(%d+)\/(%d+)") do
					equipFrag = tonumber(id)
				end 
				break
			end
		end
	end
	return equipFrag
end
function checkSingleFragEnough(fragId,currentNum)
	local fragNeed = checkSingleEquipfrag(fragId)
	local ret = false
	if currentNum >= fragNeed then
		ret = true
	end
	return ret
end

local suitEquipTempTable = {}
-- 判断装备是否套装
function IsSuitEquip(equipId)
	if equipId == nil then
		return false
	end
	local ret = false
	if suitEquipTempTable[equipId] == nil then
		for k,v in pairs(Harness) do
			if ret then
				break
			end
			for id in string.gfind(v.Harness_ID,"(%d+)") do
				if id ~= nil then
					local targetId = tonumber(id)
					if targetId == equipId then
						ret = true
						break
					end
				end
			end 
		end
		suitEquipTempTable[equipId] = ret
	else
		ret = suitEquipTempTable[equipId]
	end
	return ret
end

-- 得到套装ID
function getSuitIdByEquip(equipId,equipNum)
	local suitId = 0
	for k,v in pairs(Harness) do
		local suitNum =  tonumber(v.Number)
		for id in string.gfind(v.Harness_ID,"(%d+)") do
			if id ~= nil then
				local targetId = tonumber(id)
				if targetId == equipId  and suitNum == equipNum then
					suitId = tonumber(k)
				end
			end
		end 
	end
	return suitId
end

-- 套装的属性key值
local SuitAttriKeyTable = {
		"PhyAttack",
		"MagAttack",
		"SkiAttack",
		"PhyDefense",
		"MagDefense",
		"SkiDefense",
		"HP",
		"PhyAttack_Ratio",
		"MagAttack_Ratio",
		"SkiAttack_Ratio",
		"PhyDefense_Ratio",
		"MagDefense_Ratio",
		"SkiDefense_Ratio",
		"HP_Ratio",
		"Accurate",
		"Dodge",
		"Wreck",
		"Parry",
		"CriticalStrike",
		"Tenacity",
		"Slay",
} 

function getSuitDes(suitId)
	local str = ""
	if Harness[suitId] ~= nil then
		local suitNum = tonumber(Harness[suitId]["Number"])
		str = LanguageLocalization:GetLocalization(Harness[suitId]["Harness_Name"]) .. suitNum .. LanguageLocalization:GetLocalization("M_SET1") .. ":"
		for k,v in pairs(SuitAttriKeyTable) do
			if Harness[suitId][v] ~= nil then
				local num =  tonumber(Harness[suitId][v])
				if num ~= 0 then
					str = str .. " " .. getEquipAttrStrByTableIndex(v) .. "+" .. getEquipAttrFormatStr(v,num)
				end
			end
		end
	end
	return str
end