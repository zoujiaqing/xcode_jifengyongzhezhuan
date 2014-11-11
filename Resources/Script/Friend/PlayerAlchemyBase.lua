require("Script/Friend/PlayerEquipNextAttr")
require("Script/Friend/PlayerEquipHole")
-- 灵石信息
PlayerAlchemyBase = {
	consume		= 0 , --灵石升级需要的花费(=0时表示已经达到最高级，不能升级)
	convert		= 0 , -- 兑换成精华数量
}

function PlayerAlchemyBase:create(extension)
	local info = setmetatable(extension or {},self)
	self.__index = self
	return info
end

-- 参数equipmentBase 可参考网络协议中的EquipmentBase
function PlayerAlchemyBase:init(ssBase)
	self.consume = ssBase.consume
	self.convert = ssBase.convert
end