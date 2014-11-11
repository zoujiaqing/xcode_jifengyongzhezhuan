-- 装备镶嵌孔
PlayerEquipHole = {
	holeIndex		= 0,	-- 孔编号
	gemId			= 0,	-- 宝石ID
	attrKey			= 0,	-- 属性Key
	attrValue		= 0,	-- 属性value
	attrValueMin	= 0,	-- 属性最小value
	attrValueMax	= 0,	-- 属性最大value
}

function PlayerEquipHole:create(extension)
	local info = setmetatable(extension or {},self)
	self.__index = self
	return info
end

-- 参数csEquipHole 可参考网络协议中的CSEquipHole
function PlayerEquipHole:init(csEquipHole)
	self.holeIndex = csEquipHole.hole_index
	self.gemId = csEquipHole.item_gem_id
	self.attrKey = csEquipHole.attr_key
	self.attrValue = csEquipHole.attr_value
	if csEquipHole.attr_min_v ~= nil then
		self.attrValueMin = csEquipHole.attr_min_v
	end

	if csEquipHole.attr_max_v ~= nil then
		self.attrValueMax = csEquipHole.attr_max_v
	end
end