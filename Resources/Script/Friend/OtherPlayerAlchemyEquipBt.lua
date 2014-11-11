require("Script/GameConfig/characterString")
require("Script/GameConfig/Localization")
require("Script/Friend/PlayerItemCenter")
require("Script/Equipment/TipManager")

OtherPlayerAlchemyEquipBt = {
	index = 0,	-- 索引
	btNode	= nil,	-- 按键
	itemId = 0,	-- 装备ID
	userLevel = 100,
}

function OtherPlayerAlchemyEquipBt:create(extension)
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

function OtherPlayerAlchemyEquipBt:init(uiLayout,index)
	if uiLayout ~= nil then
		self.index = index
		self.btNode	= uiLayout:FindChildObjectByName(string.format("alchemyGrid%d",index)).__UIButton__;
		self.btNode:setTag(index)
	end
end

function OtherPlayerAlchemyEquipBt:checkPlayerEquipUnlock(index)
	if (self.userLevel - 20)/10 < index then
		return false
	end
	return true
end

function OtherPlayerAlchemyEquipBt:setUserLevel(level)
	self.userLevel = level
	if not self:checkPlayerEquipUnlock(self.index) then
		local lockPic = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_STORAGE_GRID1_LOCKED")
		local scale = self.btNode:getMenuItemSprite():getContentSize().width / lockPic:getContentSize().width
		lockPic:setScale(scale)
		lockPic:setContentSize(self.btNode:getMenuItemSprite():getContentSize())
		if lockPic and self.btNode then
			self.btNode:setIconSprite(lockPic)
		end
	end
end

function OtherPlayerAlchemyEquipBt:setItem(playerItem)
	if self:checkPlayerEquipUnlock(self.index) then
		if  playerItem ~= nil then
			self.itemId = playerItem.itemId
			local sprite = ItemManager:Get():getIconSpriteById(self.itemId)
			if self.btNode ~= nil then
				self.btNode:setIconSprite(sprite)
				ItemManager:Get():showSpriteParticleEffect(sprite,playerItem.itemId,ccp(sprite:getContentSize().width * 0.5,sprite:getContentSize().height * 0.5))
			end
		else
			if self.btNode ~= nil then
				self.btNode:setIconSprite(nil)
				self.btNode:setFrameSprite(nil)
			end
		end
	--else
	--	local lockPic = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_FRAME_SOUL")
	--	local scale = self.btNode:getMenuItemSprite():getContentSize().width / lockPic:getContentSize().width
	--	lockPic:setScale(scale)
	--	lockPic:setContentSize(self.btNode:getMenuItemSprite():getContentSize())
	--	if lockPic and self.btNode then
	--		self.btNode:setIconSprite(lockPic)
	--	end
	end
end

function OtherPlayerAlchemyEquipBt:registerBtClickedCallback(callFunc)
	if self.btNode ~= nil then
		local menuItem = self.btNode:getMenuItemSprite()
		if menuItem ~= nil then 
			menuItem:registerScriptTapHandler(callFunc)
		end
	end
end

function OtherPlayerAlchemyEquipBt:ShowTips()
	--CCLuaLog("OtherPlayerEquipBt:ShowTips()")
	local page = OtherPlayerEquipLayer:getCurrentPage()
	if page < 5 then
		local items = getPlayerItems(PlayerItemCenter:getSelectedPlayer())
		if items ~= nil and self.btNode ~= nil then
			local equipPos = self.index + kAlchemyHeroEquip + page * 100
			--CCLuaLog("equipPos: " .. equipPos)
			local item = items:getEquipItemByPos(equipPos)
			if item ~= nil then
				local menu = self.btNode:getMenuNode()
				local pt = menu:getTouchPoint()
				showOtherAlchemyTip(item.itemId,item.itemAttrTable,item.itemLevel,item.ss_info,pt.x,pt.y)
			end
		end
	end
end

function OtherPlayerEquipBt:HideTips()
	TipMgr:Get():destroyTip()
end

function OtherPlayerEquipBt:setVisible(isVisible)
	if self.btNode ~= nil then
		self.btNode:setVisible(isVisible)
	end
end