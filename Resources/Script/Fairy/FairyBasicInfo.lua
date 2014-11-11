require("Script/SpriteElf/fairybasicinfo")

-- 由配表决定的某类精灵的固定属性
FairyBasicInfo = {
	fairyId		= 0,	-- 精灵ID
	name		= "",
	quality		= 0,	-- 资质
	icon		= "",	-- iconID
	fetterTable = nil,	-- 连携信息。存放连携ID
}

function FairyBasicInfo:create(fairyId)
	local item = setmetatable({},self)
	self.__index = self
	item:init(fairyId)
	return item
end

function FairyBasicInfo:init(fairyId)
	--CCLuaLog("FairyBasicInfo:init(fairyId): " .. fairyId)
	self.fairyId = fairyId
	self:initBasicInfo()
end

function FairyBasicInfo:initBasicInfo()
	if fairys[self.fairyId] ~= nil then
		self.name = LanguageLocalization:GetLocalization(fairys[self.fairyId].Name)
		self.quality = tonumber(fairys[self.fairyId].Quality)
		self.icon = "map_ui_system_icon_FRAME_" .. string.upper(fairys[self.fairyId].icon)
	end
end

function FairyBasicInfo:getFetterTable()
	if self.fetterTable == nil then
		self.fetterTable = getFettersByFairyId(self.fairyId)
	end
	return self.fetterTable
end

function FairyBasicInfo:getFairyIcon()
	--CCLuaLog("FairyBasicInfo:getFairyIcon()" .. self.icon)
	local sprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",self.icon)
	return sprite
end
