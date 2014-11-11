require("Script/GameConfig/EquipQuality")
require("Script/GameConfig/EquipIntensify")

AttrIndex = {
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

EquipInfo = {
	equipId = 0,
	basicInfo = nil,
	intensityInfo = nil,
	lockedInfo = nil,
}

function EquipInfo:create(equipId)
	local item = setmetatable({},self)
	self.__index = self
	item:init(equipId)
	return item
end

function EquipInfo:init(equipId)
	self.equipId = tonumber(equipId)
	self.basicInfo = {}
	self.intensityInfo = {}
	self.lockedInfo = {}
	setmetatable(self.basicInfo,{})
	setmetatable(self.intensityInfo,{})
	setmetatable(self.lockedInfo,{})
	CCLuaLog("equipId: " .. equipId)
	if EquipQuality[self.equipId] ~= nil then
		for k,v in pairs(AttrIndex) do 
			self:initSingleBasicInfo(v)
		end
	end
	for k,v in pairs(EquipIntensify) do 
		local id = tonumber(v.ID)
		if id == self.equipId then
			for _index,_name in pairs(AttrIndex) do 
				self:initSingleIntensityInfo(_name,v)
			end
		end
	end
end

function EquipInfo:initSingleIntensityInfo(attrName,info)
	local typeNum = tonumber(info.Type)
	if typeNum == 1 or typeNum == 2 then
		local equipAttr = {}
		setmetatable(equipAttr,{})
		equipAttr.index = attrName
		equipAttr.value = info[attrName]
		table.insert(self.intensityInfo,equipAttr)
	elseif typeNum == 3 then
		if tonumber(info[attrName]) ~= 0 then
			local equipAttr = {}
			setmetatable(equipAttr,{})
			equipAttr.index = attrName
			equipAttr.value = info[attrName]
			equipAttr.level = info.Intensify_Level
			table.insert(self.lockedInfo,equipAttr)
		end
	end
end

function EquipInfo:initSingleBasicInfo(attrName)
	if EquipQuality[self.equipId][attrName] ~= nil then
		local num = tonumber(EquipQuality[self.equipId][attrName])
		if num ~= 0 then
			local equipAttr = {}
			setmetatable(equipAttr,{})
			equipAttr.index = attrName
			equipAttr.value = EquipQuality[self.equipId][attrName]
			table.insert(self.basicInfo,equipAttr)
		end
	end
end