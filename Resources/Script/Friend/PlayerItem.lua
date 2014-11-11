require("Script/Friend/PlayerItemAttr")
require("Script/Friend/PlayerEquipBase")
require("Script/Friend/PlayerAlchemyBase")
-- 玩家物品
PlayerItem = {
	pos			= 0,	-- 物品位置
	itemId		= 0,	-- 物品ID
	itemNum		= 0,	-- 物品数量
	itemLevel	= 0,	-- 物品等级
	itemAttrTable = nil,	-- 物品属性
	equipInfo	= nil,	-- 装备信息
	ss_info     = nil,  -- 灵石信息
}

function PlayerItem:create(extension)
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

-- 参数equipmentBase 可参考网络协议中的CSItemInfo
function PlayerItem:init(csItemInfo)
	CCLuaLog("PlayerItem:init: " .. csItemInfo.pos)
	self.pos = csItemInfo.pos
	self.itemId = csItemInfo.item_id
	self.itemNum = csItemInfo.item_num
	if csItemInfo.item_level ~= nil then
		self.itemLevel = csItemInfo.item_level
	end

	if csItemInfo.item_attrs ~= nil then
		if self.itemAttrTable == nil then
			self.itemAttrTable = {}
			setmetatable(self.itemAttrTable,{})
		end
		for k,v in pairs(csItemInfo.item_attrs) do 
			local itemAttr = PlayerItemAttr:create()
			itemAttr:init(csItemInfo.item_attrs[k])
			self.itemAttrTable[k] = itemAttr
		end
	end

	if csItemInfo.equip_info ~= nil then
		if self.equipInfo == nil then
			self.equipInfo = PlayerEquipBase:create()
		end
		self.equipInfo:init(csItemInfo.equip_info)
	end

	if csItemInfo.ss_info ~= nil then
		if self.ss_info == nil then
			self.ss_info = PlayerAlchemyBase:create()
		end
		self.ss_info:init(csItemInfo.ss_info)
	end
end

function PlayerItem:resetValue()
	self.pos = 0
	self.itemId = 0
	self.itemNum = 0
	self.itemLevel = 0
	self.itemAttrTable = nil
	self.equipInfo = nil
	self.ss_info = nil
end