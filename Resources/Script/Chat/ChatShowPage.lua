require("Script/Chat/ChatDataCenter")
require("Script/Chat/ChatListItem")
ChatShowPage = {
	uiContainer = nil,
	uiList = nil,
	noteIndexTable = nil,
	uiListItemTable = nil,
	chatListTable = nil,
	fontSize = 20,
	menuPriority = -130,
	isButtonEnable = true,
}

function ChatShowPage:create(extension)
	local playerinfo = setmetatable(extension or {},self)
	self.__index = self
	playerinfo.noteIndexTable = {}
	setmetatable(playerinfo.noteIndexTable,{})
	playerinfo.uiListItemTable = {}
	setmetatable(playerinfo.uiListItemTable,{})
	playerinfo.chatListTable = {}
	setmetatable(playerinfo.chatListTable,{})
	return playerinfo
end

function ChatShowPage:resetValue()
	self.uiContainer = nil
	self.uiList = nil
	self.noteIndexTable = nil
	self.uiListItemTable = nil
	self.chatListTable = nil
	self.fontSize = 20
	self.menuPriority = -130
	self.isButtonEnable = true
end

function ChatShowPage:destroy()
	unregisterUINoteNotice(self)
	if self.chatListTable ~= nil then
		for k,v in pairs(self.chatListTable) do
			v:destroy()
		end
	end
	self:resetValue()
end

function ChatShowPage:setVisible(b_isVisible)
	if self.uiContainer ~= nil then
		self.uiContainer:setVisible(b_isVisible)
	end
end

function ChatShowPage:onReceivedNewNote(note)
	--CCLuaLog("ChatShowPage:onReceivedNewNote")
	if self.noteIndexTable ~= nil and note ~= nil then
		table.insert(self.noteIndexTable,note)
		local length = table.getn(self.noteIndexTable)
		if length > 20 then
			local removeIndex = self.noteIndexTable[1].noteIndex
			if self.uiList ~= nil and self.uiListItemTable[removeIndex] ~= nil then
				self.uiList:removeItem(self.uiListItemTable[removeIndex])
				self.uiListItemTable[note.noteIndex] = nil
			end
			if self.chatListTable[note.noteIndex] ~= nil then
				self.chatListTable[note.noteIndex]:destroy()
				self.chatListTable[note.noteIndex] = nil
			end
			table.remove(self.noteIndexTable,1)		
		end
	end
	self:showSingleNote(note)
end

function ChatShowPage:getNoteContent(note)
	local text = nil
	if note ~= nil then
		text = note.name .. ": " .. note.content
	end
	return text
end

function ChatShowPage:showSingleNote(note)
	if self.uiList ~= nil then
		local listNode = ChatListItem:create();
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
		local size = CCSizeMake(self.uiList:getContentSize().width / scale,self.uiList:getContentSize().height / scale)
		listNode:init(note,size.width,self.fontSize,self.menuPriority,self.isButtonEnable)
		self.chatListTable[note.noteIndex] = listNode
		if listNode.node ~= nil then
			listNode.node:setAnchorPoint(ccp(0,0.5))
			local base = self.uiList:AddBaseItem(listNode.node:getContentSize(),listNode.node)
			local oldPos_y = listNode.node:getPositionY()
			listNode.node:setPosition((0 + self.uiList:getContentSize().width ) * 0.5,oldPos_y)
			--listNode.node:setPosition((listNode.node:getContentSize().width + self.uiList:getContentSize().width /scale ) * 0.5,oldPos_y)
			self.uiListItemTable[note.noteIndex] = base
			local size = self.uiList:getContentSize()
			local viewSize = self.uiList:getViewSize()
			if size.height > viewSize.height then
				local offset = ccp(0,size.height - viewSize.height)
				self.uiList:setStartOffset(offset)
			end
		end
	end
end

function ChatShowPage:addChatType(t)
	registerUINoteNotice(t,self.onReceivedNewNote,self)
	local typeTable = getNotesStore(t)
	if typeTable ~= nil then
		for k,v in pairs(typeTable) do
			if self.noteIndexTable ~= nil and v ~= nil then
				table.insert(self.noteIndexTable,v)
			end
		end
	end
end

function ChatShowPage:setButtonEnable(isEnable)
	self.isButtonEnable = isEnable
end

local function NoteSort(a,b)
	return a.noteIndex < b.noteIndex
end

function ChatShowPage:init(uiLayout,containerName,listName,size,priority)
	self.uiContainer = uiLayout:FindChildObjectByName(containerName).__UIContainer__
	self.uiList = uiLayout:FindChildObjectByName(listName).__UIScrollList__
	self.fontSize = size
	self.menuPriority = priority
	table.sort(self.noteIndexTable,NoteSort)
	local length = table.getn(self.noteIndexTable)
	while length > 10 do
		table.remove(self.noteIndexTable,1)
		length = table.getn(self.noteIndexTable)
	end
	for k,v in pairs(self.noteIndexTable) do
		self:showSingleNote(v)
	end
end