-- 下一级装备属性 是 装备信息PlayerEquipBase中的子项,
PlayerEquipNextAttr = {
	attrKey			= 0,	-- 属性key
	attrValue		= 0,	-- 基本属性
};

function PlayerEquipNextAttr:create(extension)
	local info = setmetatable(extension or {},self)
	self.__index = self
	return info
end

-- 参数equipmentBase 可参考网络协议中的EquipmentBase:Item_Attr
function PlayerEquipNextAttr:init(itemAttr)
	self.attrKey = itemAttr.key
	self.attrValue = itemAttr.value
end