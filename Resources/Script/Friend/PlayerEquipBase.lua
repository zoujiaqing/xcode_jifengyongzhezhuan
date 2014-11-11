require("Script/Friend/PlayerEquipNextAttr")
require("Script/Friend/PlayerEquipHole")
-- 装备信息(升到下级所需要的花费、下一等级的属性增量，镶嵌宝石)
PlayerEquipBase = {
	nextAttrTable	= nil,	-- 装备属性
	holeTable		= nil,  -- 宝石插孔
	leverUpCost		= 0,	-- 下一级强化花费
	suitActivationTable = nil, -- 套装激活状态
}

function PlayerEquipBase:create(extension)
	local info = setmetatable(extension or {},self)
	self.__index = self
	return info
end

-- 参数equipmentBase 可参考网络协议中的EquipmentBase
function PlayerEquipBase:init(equipmentBase)
	self.cost_levelup = equipmentBase.cost_levelup
	if equipmentBase.incr_attr_next_lv ~= nil then
		if self.nextAttrTable == nil then
			self.nextAttrTable = {}
			setmetatable(self.nextAttrTable,{})
		end

		for k,v in pairs(equipmentBase.incr_attr_next_lv) do
			local nextAttr = PlayerEquipNextAttr:create()
			nextAttr:init(equipmentBase.incr_attr_next_lv[k])
			self.nextAttrTable[k] = nextAttr
		end
	end

	if equipmentBase.equip_holes ~= nil then
		if self.holeTable == nil then
			self.holeTable = {}
			setmetatable(self.holeTable,{})
		end

		for k,v in pairs(equipmentBase.equip_holes) do
			local hole = PlayerEquipHole:create()
			hole:init(equipmentBase.equip_holes[k])
			self.holeTable[k] = hole
		end
	end

	if equipmentBase.rel_ids ~= nil then
		if self.suitActivationTable == nil then
			self.suitActivationTable = {}
			setmetatable(self.suitActivationTable,{})
		end

		for k,v in pairs(equipmentBase.rel_ids) do
			self.suitActivationTable[v] = true
		end
	end
end