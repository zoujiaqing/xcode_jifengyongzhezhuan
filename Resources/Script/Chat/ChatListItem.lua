require("Script/Chat/ChatFuncTable")
ChatListItem = {
	node = nil,
	maxWidth = 0,
	fontSize = 20,
	playerId = nil,
	playerName = "",
	priority = 0,
	funcIndex = 0,
	isButtonEnable = true,
}

function ChatListItem:create(extension)
	local playerinfo = setmetatable(extension or {},self)
	self.__index = self
	return playerinfo
end

function ChatListItem:resetValue()
	self.node = nil
	self.maxWidth = 0
	self.fontSize = 20
	self.playerId = nil
	self.priority = 0
	self.funcIndex = 0
	self.playerName = ""
	self.isButtonEnable = true
end

function ChatListItem:destroy()
	if self.funcIndex ~= 0 then
		removeFuncTable(self.funcIndex)
	end
	self:resetValue()
end

local function onNameClicked(tag)
	if ChatLayer.chatStats ~= 1 then
		local obj = getFuncTable(tag)
		if obj ~= nil and obj.playerId ~= nil and obj.playerName ~= nil then
			if not obj.playerId:isEqual(UserData:getUserId()) then
				showUILayerByTag(UITagTable["ChatPlayerOption"].tag,true)
				ChatPlayerOption:setSelectedPlayer(obj.playerId,obj.playerName)
			end
		end
	end
end

function ChatListItem:createSystemNode(note)
	--local nameColor = ccc3(255, 255, 255)
	local worldColor = ccc3(255,198,118)
	self.node = HtmlNode:create(self.maxWidth)
	self.node:begin()
	local str = LanguageLocalization:GetLocalization("M_CHAT_SYSTEM") .. ":" .. note.content
	self.node:addLabel(str,self.fontSize,KJLinXin,worldColor)
	--self.funcIndex = self.node:addHyperLink(note.name,self.fontSize,KJLinXin,nameColor,onNameClicked,self.priority,self.isButtonEnable)
	--self.node:addLabel(": " .. note.content,self.fontSize,KJLinXin,worldColor)
	self.node:finish()

	if self.funcIndex ~= 0 then
		setFuncTable(self.funcIndex,self)
	end
end

function ChatListItem:createWorldChatNode(note)
	local nameColor = ccc3(255, 255, 255)
	local worldColor = ccc3(255, 137, 41)
	self.node = HtmlNode:create(self.maxWidth)
	self.node:begin()
	local str = getLocalizationString('M_CHAT_WORLD')
	self.node:addLabel(str,self.fontSize,KJLinXin,worldColor)
	self.funcIndex = self.node:addHyperLink(note.name,self.fontSize,KJLinXin,nameColor,onNameClicked,self.priority,self.isButtonEnable)
	self.node:addLabel(": " .. note.content,self.fontSize,KJLinXin,worldColor)
	self.node:finish()

	if self.funcIndex ~= 0 then
		setFuncTable(self.funcIndex,self)
	end
end

function ChatListItem:createPartyChatNode(note)
	local nameColor = ccc3(255, 255, 255)
	local worldColor = ccc3(17, 179, 255)
	self.node = HtmlNode:create(self.maxWidth)
	self.node:begin()
	local str = getLocalizationString('M_CHAT_CONFERENCE')
	self.node:addLabel(str,self.fontSize,KJLinXin,worldColor)
	self.funcIndex = self.node:addHyperLink(note.name,self.fontSize,KJLinXin,nameColor,onNameClicked,self.priority,self.isButtonEnable)
	self.node:addLabel(": " .. note.content,self.fontSize,KJLinXin,worldColor)
	self.node:finish()

	if self.funcIndex ~= 0 then
		setFuncTable(self.funcIndex,self)
	end
end

function ChatListItem:createPersonalChatNode(note)
	local nameColor = ccc3(255, 255, 255)
	local privateColor = ccc3(206, 58, 255)
	self.node = HtmlNode:create(self.maxWidth)
	self.node:begin()
	local str = getLocalizationString('M_CHAT_PRIVATE')
	self.node:addLabel(str,self.fontSize,KJLinXin,privateColor)
	if note.isSend then
		self.node:addLabel("你对",self.fontSize,KJLinXin,privateColor)
		self.funcIndex = self.node:addHyperLink(note.name,self.fontSize,KJLinXin,nameColor,onNameClicked,self.priority,self.isButtonEnable)
		self.node:addLabel("说: " .. note.content,self.fontSize,KJLinXin,privateColor)
	else
		self.funcIndex = self.node:addHyperLink(note.name,self.fontSize,KJLinXin,nameColor,onNameClicked,self.priority,self.isButtonEnable)
		self.node:addLabel("对你说: " .. note.content,self.fontSize,KJLinXin,privateColor)
	end
		
	self.node:finish()

	if self.funcIndex ~= 0 then
		setFuncTable(self.funcIndex,self)
	end
end

function ChatListItem:init(note,width,fontSize,priority,isEnable)
	self.maxWidth = width
	self.fontSize = fontSize
	self.playerId = note.targetId
	self.priority = priority
	self.playerName = note.name
	self.isButtonEnable = isEnable
	if note.isSystem then
		self:createSystemNode(note)
	else
		if note.noteType == "WORLD" then
			self:createWorldChatNode(note)
		elseif note.noteType == "PM" then
			self:createPersonalChatNode(note)
		elseif note.noteType == "PARTY" then
			self:createPartyChatNode(note)
		end
	end
end