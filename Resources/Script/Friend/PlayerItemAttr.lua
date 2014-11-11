-- 物品基本属性信息
PlayerItemAttr = {
	attrKey			= 0,	-- 属性key
	basicValue		= 0,	-- 基本属性
	intensifyValue	= 0,	-- 强化属性
	genValue		= 0,	-- 宝石属性
}

function PlayerItemAttr:create(extension)
	local info = setmetatable(extension or {},self)
	self.__index = self
	return info
end

-- 参数equipmentBase 可参考网络协议中的CSItemAttr
function PlayerItemAttr:init(csItemAttr)
	self.attrKey = csItemAttr.attr_key
	self.basicValue = csItemAttr.attr_basic_value
	if csItemAttr.attr_intensify_value ~= nil then
		self.intensifyValue = csItemAttr.attr_intensify_value
	end
	if csItemAttr.attr_gem_value ~= nil then
		self.genValue = csItemAttr.attr_gem_value
	end
end