require("Script/Friend/PlayerItem")

-- 单个用户的物品信息
PlayerTotalItem = {
	id			= nil,		-- 玩家ID 类型为PLAYER_ID
	exp			= 0,		-- 玩家当前经验值
	effectiveness = 0,
	itemList	= nil		-- 玩家物品列表	
}

function PlayerTotalItem:create(extension)
	local info = setmetatable(extension or {},self)
	self.__index = self
	return info
end

function PlayerTotalItem:resetValue()
	self.id = nil
	self.regTime = 0
	self.expBar = 0
	self:resetItemList()
end

function PlayerTotalItem:resetItemList()
	if self.itemList ~= nil then
		for k,v in pairs(self.itemList) do
			self.itemList[k]:resetValue()
			self.itemList[k] = nil
		end
	end
	self.itemList = nil
end

function PlayerTotalItem:setId(uid)
	self.id = uid
end

function PlayerTotalItem:setExp(exp)
	self.exp = exp
end

function PlayerTotalItem:SetEffectiveness(num)
	self.effectiveness = num
end

-- 参数equipmentBase 可参考网络协议中的CSItemInfo
function PlayerTotalItem:addItem(itemPos,itemInfo)
	if itemInfo == nil then
		return 
	end

	if self.itemList == nil then
		self.itemList = {}
		setmetatable(self.itemList,{})
	end

	local playerItem = PlayerItem:create()
	playerItem:init(itemInfo)
	self.itemList[itemPos] = playerItem
end

function PlayerTotalItem:getEquipItemByPos(pos)
	local equipItem = nil
	if self.itemList ~= nil and self.itemList[pos] ~= nil then
		equipItem = self.itemList[pos]
	end
	return equipItem
end

-- 得到玩家出战的精灵ID
function PlayerTotalItem:getElfEquiped()
	local elfId = 0
	if self.itemList ~= nil then
		for k,v in pairs(self.itemList) do
			if (v.pos > kFairyStartId) and (v.pos < kItemGemStone) then
				if v.itemNum == 3 then
					elfId = v.itemId
					break
				end
			end
		end
	end
	return elfId
end