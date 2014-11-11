require("Script/Language")

-- 好友列表的UI节点
FriendListItem = {
	id			= nil,		-- 玩家ID
	rootNode	= nil,		-- 根节点
	headIcon	= nil,		-- 头像
	headFrame	= nil,		-- 头像边框
	levelLabel	= nil,		-- 等级文本
	nameLabel	= nil,		-- 名字文本
	onlineLabel	= nil,		-- 玩家是否在线文本
	chatMenuItem	= nil,		-- 聊天按钮
	moreMenuItem	= nil,		-- 更多按钮
}

function FriendListItem:createFriendListItem(extension)
	CCLuaLog("-------------------create new FriendListItem---------------")
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

function FriendListItem:resetValue()
	self.id = nil
	self.rootNode = nil
	self.headIcon = nil
	self.headFrame = nil
	self.levelLabel = nil
	self.nameLabel = nil
	self.chatMenuItem = nil
	self.moreMenuItem = nil
end

function FriendListItem:destroy()
	self:resetValue()
end

function FriendListItem:getHeadIconByType(type)
	local headIconName = getCharacterField(type,"HeadPicture")
	local playerIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",headIconName)
	return playerIcon
end

function FriendListItem:setButtonSelected()
	if self.rootNode ~= nil then
		local menuItem = self.rootNode:getMenuItemSprite()
		menuItem:selected()
	end
end

function FriendListItem:setButtonUnselected()
	if self.rootNode ~= nil then
		local menuItem = self.rootNode:getMenuItemSprite()
		menuItem:unselected()
	end
end

function FriendListItem:setButtonCallBack(func)
	if self.rootNode ~= nil then
		local menuItem = self.rootNode:getMenuItemSprite()
		menuItem:registerScriptTapHandler(func)
	end
end

function FriendListItem:setChatBtCallBack(func)
	if self.chatMenuItem ~= nil then
		self.chatMenuItem:registerScriptTapHandler(func)
	end
end

function FriendListItem:setMoreBtCallBack(func)
	if self.moreMenuItem ~= nil then
		self.moreMenuItem:registerScriptTapHandler(func)
	end
end

function FriendListItem:getID()
	return self.id
end

function FriendListItem:SetOnlineText(flag)
	if self.onlineLabel ~= nil then
		local bOnline = true
		if flag == 0 then
			bOnline = false
			self.onlineLabel:setColor(ccc3(155,155,155))
		else
			self.onlineLabel:setColor(ccc3(56,221,7))
		end

		local languageFlag = nil
		if bOnline then
			languageFlag = "M_FRIEND_ONLINE_1"
		else
			languageFlag = "M_FRIEND_ONLINE_2"
		end
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.onlineLabel:setString(content)		
	end
end

function FriendListItem:init(parentList,playerInfo,tag)
	self.id = playerInfo:getID()
	if parentList ~= nil and playerInfo ~= nil then
		local friendFrame,friendPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_FRAME_PLAYER1")
		local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_FRIEND_FRAME_PLAYER1_SELECTED")
		local button = parentList:AddNewItem(friendFrame,clickedFrame,"","",ccc3(255,255,255),-130)
		button:setTag(tag)
		local headFrameRect,headFramePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_ICON_PLAYER1")
		self.headIcon = self:getHeadIconByType(playerInfo:getType())
		if self.headIcon ~= nil then
			self.headIcon:setPosition(ccpSub(headFramePt,friendPt))
			button:getAnimationNode():addChild(self.headIcon,10)
			self.headIcon:setScale(0.6)
		end

		self.headFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FRAME_HEADSHOTINBATTLE")
		if self.headFrame ~= nil then
			self.headFrame:setPosition(ccpSub(headFramePt,friendPt))
			button:getAnimationNode():addChild(self.headFrame,11)
			self.headFrame:setScale(0.8)
		end

		local levelLabelRect,labelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_TEXT_LEVEL_PLAYER1")
		if levelLabelRect ~= nil then
			self.levelLabel = TXGUI.UILabelTTF:create("Lv " .. playerInfo:getLevel(),KJLinXin,28)
			self.levelLabel:setPosition(ccpSub(labelPt,friendPt))
			self.levelLabel:setColor(ccc3(255,255,255))
			button:getAnimationNode():addChild(self.levelLabel,12)
		end

		local nameLabelRect,namePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_TEXT_NAME_PLAYER1")
		if nameLabelRect ~= nil then
			self.nameLabel = TXGUI.UILabelTTF:create(playerInfo:getName(),KJLinXin,28)
			self.nameLabel:setPosition(ccpSub(namePt,friendPt))
			self.nameLabel:setColor(ccc3(255,90,40))
			button:getAnimationNode():addChild(self.nameLabel,13)
		end

		local onlineLabelRect,onlineLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_TEXT_ONLINE_PLAYER1")
		if onlineLabelRect ~= nil then
			self.onlineLabel = TXGUI.UILabelTTF:create("",KJLinXin,28)
			self.onlineLabel:setPosition(ccpSub(onlineLabelPt,friendPt))
			self.onlineLabel:setColor(ccc3(255,255,255))
			button:getAnimationNode():addChild(self.onlineLabel,14)
		end

		local chatFrame,chatPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_ICON_CHAT")
		if chatFrame ~= nil then
			self.chatMenuItem = CCMenuItemSprite:create(chatFrame,chatFrame)
			local chatBt = UIMenu:create()
			chatBt:addChild(self.chatMenuItem)
			chatBt:setInitPriority(-131)
			chatBt:setPosition(ccpSub(chatPt,friendPt))
			button:getAnimationNode():addChild(chatBt,15)
			self.chatMenuItem:setTag(tag)
		end

		local moreFrame,morePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_ICON_MORE")
		local moreClickFrame,moreClickPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_ICON_MORE_CLICKED")
		if moreFrame ~= nil then
			self.moreMenuItem = CCMenuItemSprite:create(moreFrame,moreClickFrame)
			local moreBt = UIMenu:create()
			moreBt:addChild(self.moreMenuItem)
			moreBt:setInitPriority(-131)
			moreBt:setPosition(ccpSub(morePt,friendPt))
			button:getAnimationNode():addChild(moreBt,16)
			self.moreMenuItem:setTag(tag)
		end

		local moreBtnLabelRect,moreBtnLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_FRIEND_ICON_MORE_TEXT")
		if moreBtnLabelRect ~= nil then
			local languageFlag = 'M_FRIEND_MORE'
			local content = LanguageLocalization:GetLocalization(languageFlag)
			local nameLabel = TXGUI.UILabelTTF:create(content,KJLinXin,28)
			nameLabel:setPosition(ccpSub(moreBtnLabelPt,friendPt))
			nameLabel:setColor(ccc3(255,255,255))
			button:getAnimationNode():addChild(nameLabel,17)
		end

		self.rootNode = button
	end
end