require("Script/GameConfig/Marquee")
require("Script/GameConfig/Item")
require("Script/GameConfig/EquipmentItem")
require("Script/SpriteElf/fairybasicinfo")
MarqueeMessageText = {
	color = ccc3(0,0,0),
	text = "",
}

MarqueeMessage = {
	msgNode = nil,
	text = nil,
	index = 0,
	priority = 0,
}

MarqueeFormat = {
	type = 0, 
	textTable = nil,-- 1 通用字符串，2 物品ID 3，精灵ID， 4 数字
	colorTable = nil,
}

function MarqueeMessageText:create(tColor,tStr)
	local playerinfo = setmetatable({},self)
	self.__index = self
	playerinfo.color = tColor
	playerinfo.text = tStr
	return playerinfo
end

function MarqueeFormat:createByID(id)
	local message = nil
	if id ~= nil and Marquee[id] ~= nil then
		message = setmetatable({},self)
		self.__index = self
		--message.index = order
		--message.priority = priority
		message.text = {}
		setmetatable(message.text,{})
		local ret = message:init(id)
		if not ret then
			message = nil
		end
	end
	return message
end

local function splitStringByPattern(str,pattern)
	local subStrTable = {}

end

function MarqueeFormat:init(id)
	local succ = false
	--CCLuaLog("MarqueeFormat:init: " .. id)
	if Marquee[id] ~= nil then
		self.type = id
		self.textTable = nil
		if Marquee[id].text_id ~= nil then
			self.textTable = {}
			for subText in string.gfind(Marquee[id].text_id,"([%a_%d]+)") do
				--CCLuaLog("subText: " .. subText)
				table.insert(self.textTable,subText)
			end
		end
		local colorTable = nil
		if Marquee[id].RGB_number ~= nil then
			self.colorTable = {}
			for r,g,b in string.gfind(Marquee[id].RGB_number,"(%d+)\/(%d+)\/(%d+)") do
				local tempColor = ccc3(r,g,b)
				table.insert(self.colorTable,tempColor)
				--CCLuaLog("r: " .. r .. " g: " .. g .. " b: " .. b)
			end
		end
		succ = true
	end
	return succ
end

function MarqueeMessage:create(order,priority)
	local message = setmetatable({},self)
	self.__index = self
	message.index = order
	message.priority = priority
	message.text = {}
	setmetatable(message.text,{})
	return message
end

function MarqueeMessage:addTextByFormat(format,text1,text2,itemId,num)
	if format ~= nil and format.textTable ~= nil then
		local isFirstStr = true
		for k,v in pairs(format.textTable) do
			local text = ""
			local color = nil
			if v == "1" then
				if isFirstStr then
					text = text1
					isFirstStr = false
				else
					text = text2
				end
			elseif v == "2" then
				if Items[itemId] ~= nil then
					local nameContent = Items[itemId].Name
					local quality = Items[itemId].Quality
					color = ItemManager:Get():getLabelColorByQuality(quality)
					text = LanguageLocalization:GetLocalization(nameContent)
				elseif EquipmentItems[itemId] ~= nil then
					local nameContent = EquipmentItems[itemId].Name
					local quality = EquipmentItems[itemId].Quality
					color = ItemManager:Get():getLabelColorByQuality(quality)
					text = LanguageLocalization:GetLocalization(nameContent)
				end
			elseif v == "3" then
				if fairys[itemId] ~= nil then
					local nameContent = fairys[itemId].Name
					text = LanguageLocalization:GetLocalization(nameContent)
				end
			elseif v == "4" then
				text = num
			else
				text = LanguageLocalization:GetLocalization(v)
			end
			if color == nil then
				-- 非物品使用配置的颜色
				if format.colorTable ~= nil and format.colorTable[k] ~= nil then
					color = format.colorTable[k]
				end
			end
			if color == nil then
				color = ccc3(255,255,255)
			end
			local msg = MarqueeMessageText:create(color,text)
			table.insert(self.text,msg)
		end
	end
end

function MarqueeMessage:addText(tColor,tStr)
	local msg = MarqueeMessageText:create(tColor,tStr)
	table.insert(self.text,msg)
end

function MarqueeMessage:destroy()
	for k,v in pairs(self.text) do 
		v = nil
	end
	self.text = nil

	if self.msgNode ~= nil then
		self.msgNode = nil
	end
end

function MarqueeMessage:getNode()
	if self.msgNode == nil then
		local length = 0
		local height = 0
		self.msgNode = CCNode:create()
		for k,v in pairs(self.text) do 
			if v.text ~= nil then
				--CCLuaLog("text: " .. v.text)
				local node = TXGUI.UILabelTTF:create(v.text,KJLinXin,20);
				node:setColor(v.color)
				node:setAnchorPoint(ccp(0,0.5))
				node:setPosition(ccp(length,0))
				self.msgNode:addChild(node)
				length = length + node:getContentSize().width
				if node:getContentSize().height > height then
					height = node:getContentSize().height
				end
			end
		end
		self.msgNode:setContentSize(CCSizeMake(length,height))
	end
	return self.msgNode
end

