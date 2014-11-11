require("Script/GameConfig/characterString")
require("Script/GameConfig/EquipmentItem")
require("Script/GameConfig/Localization")
require("Script/Friend/PlayerItemCenter")
require("Script/Equipment/TipManager")

OtherPlayerEquipBt = {
	index = 0,	-- 索引
	btNode	= nil,	-- 按键
	itemId = 0,	-- 装备ID
}

function OtherPlayerEquipBt:create(extension)
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

function OtherPlayerEquipBt:init(uiLayout,index)
	if uiLayout ~= nil then
		self.index = index
		self.btNode	= uiLayout:FindChildObjectByName(string.format("equipGrid%d",index)).__UIButton__;
		self.btNode:setTag(index)
	end
end

function OtherPlayerEquipBt:setFairyFriend(isLocked,friend)
	if self.btNode ~= nil then
		if isLocked then
			local lockPic = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_STORAGE_GRID1_LOCKED")
			self.btNode:setIconSprite(lockPic)
		else
			if friend == nil then
				self.btNode:setIconSprite(nil)
			else
				local fairyId = friend.fairyId
				local fairyinfo = getFairyBasicInfo(fairyId)
				if fairyinfo ~= nil then
					local fairyIcon = fairyinfo:getFairyIcon()
					self.btNode:setIconSprite(fairyIcon)
				end
			end
		end
		self.btNode:setFrameSprite(nil)
		self.btNode:setTextLabel("",18,ccc3(255,171,0),KJLinXin, ccp(0,0),ccp(0,1));
	end
end	

function OtherPlayerEquipBt:setItem(playerItem)
	if  playerItem ~= nil then
		self.itemId = playerItem.itemId
		local sprite = ItemManager:Get():getIconSpriteById(self.itemId)
		if self.btNode ~= nil then
			self.btNode:setIconSprite(sprite)
			local quility = self:getEquipQuility(self.itemId)
			--local frameSprite = ItemManager:Get():getIconFrame(quility)
			--self.btNode:setFrameSprite(frameSprite)

			local itemLevel = playerItem.itemLevel
			local itemType = getEquipTypeById(self.itemId)
			local str = ""
			if itemType == 2 then
				str = "+" .. itemLevel
			else
				str = itemLevel
			end
			local size = self.btNode:getMenuItemSprite():getContentSize()
			self.btNode:setTextLabel(str,18,ccc3(255,171,0),KJLinXin, ccp(size.width * (-0.4),size.height * (0.4)),ccp(0,1))
		end
	else
		if self.btNode ~= nil then
			self.btNode:setIconSprite(nil)
			self.btNode:setFrameSprite(nil)
			self.btNode:setTextLabel("",18,ccc3(255,171,0),KJLinXin, ccp(0,0), ccp(0,0));
		end
	end
end

function OtherPlayerEquipBt:getEquipQuility(equipId)
	local quility = 0
	if EquipmentItems[equipId] ~= nil then
		quility = EquipmentItems[equipId]["Quality"]
	end
	return quility
end

function OtherPlayerEquipBt:getEquipLabelColor(equipId)
	local quility = self:getEquipQuility(equipId)
	local color = ccc3(255,255,255)
	if quility == 1 then
		color = ccc3(0,255,0)
	elseif quility == 2 then
		color = ccc3(20,100,255)
	elseif quility == 3 then
		color = ccc3(170,40,255)
	elseif quility == 4 then
		color = ccc3(255,255,0)
	elseif quility == 5 then
		color = ccc3(255,144,0)
	end
	return color
end

function OtherPlayerEquipBt:getEquipName(equipId)
	local name = " "
	if EquipmentItems[equipId] ~= nil then
		local localName = EquipmentItems[equipId]["Name"]
		if Localization[localName] ~= nil then
			name = Localization[localName]["SimpleChinese"]
		end
	end
	return name
end

function OtherPlayerEquipBt:getEquipRequiredLevel(equipId)
	local level = 1
	if EquipmentItems[equipId] ~= nil then
		level = EquipmentItems[equipId]["Level"]
	end
	return level
end

function OtherPlayerEquipBt:getOccupationStr(equipId)
	local str = ""
	local type = 0
	if EquipmentItems[equipId] ~= nil then
		type = EquipmentItems[equipId]["Character"]
	end
	if (type ~= 0) and (Characters[type] ~= nil) then
		local occupation = Characters[type]["Name"]
		str = Localization["M_EQUIPMENT_OCCUPATION"]["SimpleChinese"] .. occupation
	else
		str = Localization["M_EQUIPMENT_OCCUPATION"]["SimpleChinese"] .. Localization["M_ALLOCCUPTATION"]["SimpleChinese"]
	end
	return str
end

function OtherPlayerEquipBt:getSellSprice(equipId)
	local price = 0
	local str = ""
	if EquipmentItems[equipId] ~= nil then
		price = EquipmentItems[equipId]["SellPrice"]
	end
	str = Localization["M_PRICE"]["SimpleChinese"] .. price
	return str
end

--function OtherPlayerEquipBt:registerTapCallback(callFunc)
	--if self.btNode ~= nil then
		--self.btNode:registerTapHandler(callFunc)
	--end
--end
--
--function OtherPlayerEquipBt:registerTapCancellCallback(callFunc)
	--if self.btNode ~= nil then
		--self.btNode:registerTapCancellHandler(callFunc)
	--end
--end

function OtherPlayerEquipBt:registerBtClickedCallback(callFunc)
	if self.btNode ~= nil then
		local menuItem = self.btNode:getMenuItemSprite()
		if menuItem ~= nil then 
			menuItem:registerScriptTapHandler(callFunc)
		end
	end
end

function OtherPlayerEquipBt:ShowTips()
	--CCLuaLog("OtherPlayerEquipBt:ShowTips()")
	local page = OtherPlayerEquipLayer:getCurrentPage()
	if page < 5 then
		local items = getPlayerItems(PlayerItemCenter:getSelectedPlayer())
		if items ~= nil and self.btNode ~= nil then
			local equipPos = self.index + kItemHeroEquip + page * 100
			--CCLuaLog("equipPos: " .. equipPos)
			local item = items:getEquipItemByPos(equipPos)
			local enchaseItem = items:getEquipItemByPos(equipPos + kItemEnchaseEquip - kItemHeroEquip)
			if item ~= nil then
				local menu = self.btNode:getMenuNode()
				local pt = menu:getTouchPoint()
				local equipHoles = nil
				if enchaseItem ~= nil and enchaseItem.equipInfo ~= nil then
					equipHoles = enchaseItem.equipInfo.holeTable
				end
				local fairyPos = getFairyPosByEquipPos(equipPos)
				local fairyInfo = getPlayerFairyByPos(fairyPos,PlayerItemCenter:getSelectedPlayer())
				local fairyId = nil
				if fairyInfo ~= nil then
					fairyId = fairyInfo.fairyId
				end
				local playerLevel = 0
				local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
				if userInfo ~= nil then
					playerLevel = userInfo.level
				end
				local suitActivationTable = nil
				if item.equipInfo ~= nil then
					suitActivationTable = item.equipInfo.suitActivationTable
				end
				showEquipTip(item.itemId,item.itemLevel,playerLevel,pt,nil,equipHoles,fairyId,suitActivationTable)

				OtherPlayerEquipLayer.selectEquipId = self.itemId
				--ItemManager:Get():setTipLuaHander(onOtherPlayerEquipTipCallBackLeft,"",false,onOtherPlayerEquipTipCallBackRight,LanguageLocalization:GetLocalization("M_FAIRY_GET_WAY"),true)
			end
		end
	else
		local fairyPos = self.index + FAIRY_POS_START_PRT - 1
		OtherPlayerEquipLayer:showSingleInfoByPos(fairyPos)
	end
end

--function OtherPlayerEquipBt:ShowTips()
--	local items = getPlayerItems(PlayerItemCenter:getSelectedPlayer())
--	if items ~= nil and self.btNode ~= nil then
--		local equipPos = self.index + kItemHeroEquip
--		if self.index > 6 then
--			equipPos = self.index - 6 + kItemFairyEquip
--		end
--		local item = items:getEquipItemByPos(equipPos)
--		if item ~= nil then
--			if EquipmentItems[self.itemId] ~= nil then
--				local sprite = ItemManager:Get():getIconSpriteById(self.itemId)
--				local quility = self:getEquipQuility(self.itemId)
--				local frameSprite = ItemManager:Get():getIconFrame(quility)
--				if frameSprite ~= nil then
--					sprite:addChild(frameSprite)
--					frameSprite:setPosition(ccp(sprite:getContentSize().width * 0.5,sprite:getContentSize().height * 0.5))
--				end
--				if sprite ~= nil then
--					local menu = self.btNode:getMenuNode()
--					local pt = menu:getTouchPoint()
--					TipMgr:Get():createTip(sprite,pt)
--					TipMgr:Get():add(self:getEquipName(self.itemId),self:getEquipLabelColor(self.itemId),22)
--					local levelStr = Localization["M_EQUIPMENT_UPGRADELEVEL"]["SimpleChinese"] .. item.itemLevel
--					TipMgr:Get():add(levelStr,ccc3(255,127,0) ,20)
--					if item.itemAttrTable ~= nil then
--						for k,v in pairs(item.itemAttrTable) do 
--							local itemAttr = item.itemAttrTable[k]
--							if itemAttr.basicValue ~= 0 then
--								local attrText = ItemManager:Get():getAttributionText(itemAttr.attrKey,itemAttr.basicValue + itemAttr.intensifyValue)
--								if attrText ~= nil then
--									TipMgr:Get():add(attrText,ccc3(255,127,0),20)
--								end
--							end
--						end
--					end
--					TipMgr:Get():add(Localization["M_EQUIPMENT_WEARREQUIREMENT"]["SimpleChinese"],ccc3(255,255,255) ,20)
--					local requiredLevel = self:getEquipRequiredLevel(self.itemId)
--					TipMgr:Get():add(Localization["M_EQUIPMENT_ROLELEVEL"]["SimpleChinese"] .. requiredLevel,ccc3(255,255,255) ,20)
--					TipMgr:Get():add(self:getOccupationStr(self.itemId),ccc3(255,255,255) ,20)
--					TipMgr:Get():add(Localization["M_EQUIPMENT_DESCRIPTION"]["SimpleChinese"],ccc3(255,127,0) ,20)
--					TipMgr:Get():add(self:getSellSprice(self.itemId),ccc3(255,255,255) ,20)
--					TipMgr:Get():addNodeDone()
--				end
--			end
--		end
--	end
--end

function OtherPlayerEquipBt:HideTips()
	TipMgr:Get():destroyTip()
end

function OtherPlayerEquipBt:setVisible(isVisible)
	if self.btNode ~= nil then
		self.btNode:setVisible(isVisible)
	end
end