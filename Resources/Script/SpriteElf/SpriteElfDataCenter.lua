require("Script/SpriteElf/fairybasicinfo")
require("Script/Language")
require("Script/GameConfig/Localization")

spriteElfDataItem = {
	ID = 0,
	--Note: elf id
	elfId = 0,
	--Note: 1 Normal 2 New 3 Battle
	elfState = 0,
	--Note: 
	elfIsLock = false
}

function spriteElfDataItem:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

LuaSpriteElfDataCenter = {
	Items = {} ,
	battleFairyIndex = 0 ,
	AttributeLabelCount = 5,
	SumAttributeLabelCount = 7,
	SortTips = {"potential","PhyAttack","MagAttack","SkiAttack","PhyDefense","MagDefense","SkiDefense","HP",},
	SumSortTips = {"HP","SkiAttack","SkiDefense","PhyAttack","PhyDefense","MagAttack","MagDefense"},
	SortLanguageFlags = {"M_FAIRY_POTENTIAL","M_CARD_PHYATTACK","M_CARD_MAGATTACK","M_CARD_SKIATTACK","M_CARD_PHYDEFENCE",
	"M_CARD_MAGDEFENCE","M_CARD_SKIDEFENCE","M_ATTRIBUTE_LIFE",},
	SumSortLanguageFlags = {"M_ATTRIBUTE_LIFE","M_ATTRIBUTE_SKIATTACK","M_ATTRIBUTE_SKIDEFENCE","M_ATTRIBUTE_PHYATTACK",
	"M_ATTRIBUTE_PHYDEFENCE","M_ATTRIBUTE_MAGATTACK","M_ATTRIBUTE_MAGDEFENCE",},
}

--Note: 管理函数

function LuaSpriteElfDataCenter:ResetValue()
	self.Items = {}
	self.battleFairyIndex = 0 	
end

function LuaSpriteElfDataCenter:InitData()
	for index,value in pairs(fairys) do
		local id = tonumber(value.show_order)
		local newElfItem = spriteElfDataItem:New()
		newElfItem.ID = id
		newElfItem.elfId = index
		newElfItem.elfState = 0
		newElfItem.elfIsLock = false
		self.Items[id] = newElfItem
	end
end

function LuaSpriteElfDataCenter:RevertData()
	for index,value in pairs(self.Items) do
		value.elfState = 0
		value.elfIsLock = false
	end
	self.battleFairyIndex = 0 	
end

function LuaSpriteElfDataCenter:GetAllFairyAddAttributes()
	local count = 0
	local resutTable = {}
	for index,value in pairsByKeys(self.SumSortTips) do
		local key = value
		if key ~= "potential" then
			count = count + 1
			resutTable[count] = 0
			for _index,_value in pairs(self.Items) do
				if _value.elfState ~= 0 then
					local fairyItem = fairys[_value.elfId] 
					local addValue = tonumber(fairyItem[key])
					resutTable[count] = resutTable[count] + addValue
				end
			end
		end
	end
	count = 0;
	for index,value in pairsByKeys(resutTable) do
		count = count + 1
		local flag = self.SumSortLanguageFlags[index]
		local content = Localization[flag].SimpleChinese
		resutTable[count] = content .. "+" .. resutTable[count]
	end
	return resutTable
end

function LuaSpriteElfDataCenter:GetOneFairyAddAttributes(elfId)
	local item = fairys[elfId]
	if item == nil then
		return nil
	end

	local count = 0
	local resutTable = {}
	for index,value in pairsByKeys(self.SortTips) do
		local key = value
		local keyContent = tonumber(item[key])
		if keyContent ~= 0 then
			count = count + 1;
			local flag = self.SortLanguageFlags[index]
			local content = Localization[flag].SimpleChinese
			resutTable[count] = content .. "+" .. keyContent
			if count == self.AttributeLabelCount then
				break
			end
		end
	end
	return resutTable
end


function LuaSpriteElfDataCenter:IsHasNewStateElf()
	for index,value in pairs(self.Items) do
		if value.elfState == 2 then
			return true
		end
	end
	return false
end

function LuaSpriteElfDataCenter:IsOneElfExistInElfDataTable(elfId)
	for index,value in pairs(self.Items) do
		if elfId == value.elfId and value.elfState ~= 0 then
			return true
		end
	end
	return false
end


--Note: 基本信息查询函数
--Name='FAIRY_2001_NAME',get_way='0',skill_ID='3001',skill_name='FAIRY_2001_SKILLNAME',skill_description='FAIRY_2001_SKILLDESCRIPTION',
--potential='100',reputation='0',coin='0',icon='icon_fairy_2001',skill_icon='icon_fairyskill_3001',next='2002',
--unlocklevel='1',points='0',star='1',getway_description='FAIRY_GETWAY_2001',show_order='1'
function LuaSpriteElfDataCenter:GetFairyElfID(index)
	--print("--------------------------- GetFairyElfID " .. index);
	--print("--------------------------- GetFairyElfID size " .. table.getn(self.Items));
	local item = self.Items[index]
	if item == nil then
		--print("--------------------------- GetFairyElfID null ");
		return nil
	end
	return item.elfId
end

function LuaSpriteElfDataCenter:GetFairyElfState(index)
	local item = self.Items[index]
	if item == nil then
		return nil
	end
	return item.elfState
end

function LuaSpriteElfDataCenter:GetFairyName(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	return Localization[fairyItem.Name].SimpleChinese
end

function LuaSpriteElfDataCenter:GetFairyGetWay(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	return tonumber(fairyItem.get_way)
end

function LuaSpriteElfDataCenter:GetFairyGetSkillId(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	return tonumber(fairyItem.skill_ID)
end

function LuaSpriteElfDataCenter:GetFairyGetSkillName(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	return Localization[fairyItem.skill_name].SimpleChinese
end

function LuaSpriteElfDataCenter:GetFairyGetSkillDescription(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	return Localization[fairyItem.skill_description].SimpleChinese
end

function LuaSpriteElfDataCenter:GetFairyGetWayDescription(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	return Localization[fairyItem.getway_description].SimpleChinese
end

function LuaSpriteElfDataCenter:GetFairySkillIcon(elfId)
	local fairyItem = fairys[elfId]
	if fairyItem == nil then
		return nil
	end
	local iconName = string.upper(fairyItem.skill_icon)
	return "map_ui_system_icon_FRAME_" .. iconName
end

function LuaSpriteElfDataCenter:GetFairyIsLock(index)
	local item = self.Items[index]
	if item == nil then
		return nil
	end
	local state = item.elfState
	if 0 == state then
		return true
	end
	return false
end

function LuaSpriteElfDataCenter:GetFairyIsFighter(index)
	local item = self.Items[index]
	if item == nil then
		return nil
	end
	local state = item.elfState
	if 3 == state then
		return true
	end
	return false
end

function LuaSpriteElfDataCenter:GetIndexByFairyId(elfId)
	for index,value in pairs(self.Items) do
		if value.elfId == elfId then
			return index
		end
	end
	return nil
end

function LuaSpriteElfDataCenter:GetTotalPotential()
	local sum = 0
	for index,value in pairs(self.Items) do
		if value.elfState ~= 0 then
			sum = sum + tonumber(fairys[value.elfId].potential)
		end
	end
	return sum
end


--Note: set

function LuaSpriteElfDataCenter:SetFairyState(elfId,state)
	for index,value in pairs(self.Items) do
		if value.elfId == elfId then
			value.elfState = state
			break
		end
	end
end

----------------------------------------------------------------
--GLOBAL FUNCTION CALLED BY C++
function InitSpriteElfDataCenter()
	print("--------------------------------------------------------------------- InitSpriteElfDataCenter")
	LuaSpriteElfDataCenter:InitData()
end