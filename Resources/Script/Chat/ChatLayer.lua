require("Script/GameConfig/uiTagDefine")
--require("Script/Chat/ChatDataCenter")
require("Script/Chat/ChatShowPage")
require("Script/GameConfig/ItemConfigTab")
require("Script/CommonDefine")
ChatLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	chatLabel = nil,
	notificationFunc = nil,
	selectedBtToggle = nil,
	worldChannelToggle = nil,
	personalChannelToggle = nil,
	guildChannelToggle = nil,

	selectedChatToggle = nil,
	worldChatToggle = nil,
	personalChatToggle = nil,
	guildChatToggle = nil,

	uiTextInput = nil,
	channelIndex = 0, -- 1 世界频道 2 私聊 3公会频道
	worldChannelPage = nil,
	personChannelPage = nil,
	guildChannelPage = nil,
	selectedCannelPage = nil,

	privateChatPlayerId = nil,
	privateChatName = "",
	chatStats = 0, -- 0 正常状态，1 组队界面（点人名无链接）
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if ChatLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(ChatLayer.notificationFunc);
	end

	if ChatLayer.worldChannelPage ~= nil then
		ChatLayer.worldChannelPage:destroy()
	end

	if ChatLayer.personChannelPage ~= nil then
		ChatLayer.personChannelPage:destroy()
	end

	if ChatLayer.guildChannelPage ~= nil then
		ChatLayer.guildChannelPage:destroy()
	end

	local chatOptionUI = getUILayerByTag(UITagTable["ChatPlayerOption"].tag,false)
	if chatOptionUI ~= nil then
		chatOptionUI:removeFromParentAndCleanup(true)
	end

	local shieldUI = getUILayerByTag(UITagTable["ShieldLayer"].tag,false)
	if shieldUI ~= nil then
		shieldUI:removeFromParentAndCleanup(true)
	end

	--local equipUI = getUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,false)
	--if equipUI ~= nil then
	--	equipUI:removeFromParentAndCleanup(true)
	--end

	ChatLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("chatUILayout")
end

function ChatLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ChatUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,80,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/chatUI.plist",self.uiLayerInstance, "chatUILayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function ChatLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.selectedBtToggle = nil
	self.worldChannelToggle = nil
	self.personalChannelToggle = nil
	self.guildChannelToggle = nil
	self.worldChatToggle = nil
	self.personalChatToggle = nil
	self.guildChatToggle = nil
	self.uiTextInput = nil
	self.channelIndex = 0
	self.worldChannelPage = nil
	self.personChannelPage = nil
	self.guildChannelPage = nil
	self.selectedCannelPage = nil
	self.selectedChatToggle = nil
	self.chatLabel = nil
	self.privateChatPlayerId = nil
	self.privateChatName = ""
	self.chatStats = 0
end

-- 删除UI
function ChatLayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

function ChatLayer:startPrivateChat(playerId,playerName)
	if playerId ~= nil and playerName ~= nil then
		if self.selectedChatToggle ~= nil then
			self.selectedChatToggle:unselected()
		end
		self.selectedChatToggle = ChatLayer.personalChatToggle
		if self.selectedChatToggle ~= nil then
			self.selectedChatToggle:selected()
		end
		self.channelIndex = 2
		if self.chatLabel ~= nil then
			self.chatLabel:setString(playerName)
		end
		self.privateChatPlayerId = playerId
		self.privateChatName = playerName
	end
end

-- 关闭按钮回调
local function onCloseBtClicked(tag)
	--RemoveOneLayer(ChatLayer.uiLayerInstance)
	ChatLayer:destroyed()
	--SetChatBar(true,-1)
end

-- 频道按钮
local function onWorldChannelBtClicked(tag)
	if ChatLayer.selectedBtToggle ~= nil then
		ChatLayer.selectedBtToggle:unselected()
	end

	if ChatLayer.selectedCannelPage ~= nil then
		ChatLayer.selectedCannelPage:setVisible(false)
	end

	ChatLayer.selectedBtToggle = ChatLayer.worldChannelToggle
	ChatLayer.selectedCannelPage = ChatLayer.worldChannelPage

	ChatLayer.selectedBtToggle:selected()
	ChatLayer.selectedCannelPage:setVisible(true)
end

local function onPersonalChannelBtClicked(tag)
	if ChatLayer.selectedBtToggle ~= nil then
		ChatLayer.selectedBtToggle:unselected()
	end
	
	if ChatLayer.selectedCannelPage ~= nil then
		ChatLayer.selectedCannelPage:setVisible(false)
	end

	ChatLayer.selectedBtToggle = ChatLayer.personalChannelToggle
	ChatLayer.selectedCannelPage = ChatLayer.personChannelPage

	ChatLayer.selectedBtToggle:selected()
	ChatLayer.selectedCannelPage:setVisible(true)
end

local function onGuildChannelBtClicked(tag)
	--GameApi:showMessage("该功能未开放")
	if ChatLayer.selectedBtToggle ~= nil then
		ChatLayer.selectedBtToggle:unselected()
	end
	
	if ChatLayer.selectedCannelPage ~= nil then
		ChatLayer.selectedCannelPage:setVisible(false)
	end

	ChatLayer.selectedBtToggle = ChatLayer.guildChannelToggle
	ChatLayer.selectedCannelPage = ChatLayer.guildChannelPage

	ChatLayer.selectedBtToggle:selected()
	ChatLayer.selectedCannelPage:setVisible(true)
end

-- 聊天按钮
local function onWorldChatBtClicked(tag)
	if ChatLayer.selectedChatToggle ~= nil then
		ChatLayer.selectedChatToggle:unselected()
	end

	ChatLayer.selectedChatToggle = ChatLayer.worldChatToggle
	ChatLayer.selectedChatToggle:selected()
	ChatLayer.channelIndex = 1
	if ChatLayer.chatLabel ~= nil then
		local str = getLocalizationString('M_CHAT_WORLDCHAT')
		if str ~= nil then
			ChatLayer.chatLabel:setString(str)
		end
	end
end

local function onPersonalChatBtClicked(tag)
	local recentPlayerId = getRecentChatPlayerId()
	local recentPlayerName = getRecentChatPlayerName()

	if recentPlayerId ~= nil and recentPlayerName ~= nil and recentPlayerName ~= "" then
		ChatLayer:startPrivateChat(recentPlayerId,recentPlayerName)
	else
		GameApi:showMessage("没有最近聊天对象")
		if ChatLayer.personalChatToggle ~= nil then
			ChatLayer.personalChatToggle:unselected()
		end
	end
end

local function onGuildChatBtClicked(tag)
	if ChatLayer.selectedChatToggle ~= nil then
		ChatLayer.selectedChatToggle:unselected()
	end

	ChatLayer.selectedChatToggle = ChatLayer.guildChatToggle
	ChatLayer.selectedChatToggle:selected()
	ChatLayer.channelIndex = 3
	if ChatLayer.chatLabel ~= nil then
		local str = getLocalizationString('M_CHAT_CONFERENCECHAT')
		if str ~= nil then
			ChatLayer.chatLabel:setString(str)
		end
	end
end

local function onSendMessageBtClicked(tag)
	if ChatLayer.uiTextInput ~= nil then
		local content = ChatLayer.uiTextInput:getString()
		local debugRet = DebugMgr:ExecDebugCmd(content)
		if debugRet then
			ChatLayer.uiTextInput:setString("")
			return
		end
		ChatLayer.uiTextInput:resignFirstResponder()
		if content ~= nil and content ~= "" then
			if ChatLayer.channelIndex == 1 then
				sendWorldNote(content,true)
			elseif ChatLayer.channelIndex == 2 then
				-- to do playerid
				if ChatLayer.privateChatPlayerId ~= nil and ChatLayer.privateChatName ~= "" then
					sendPersonalNote(content,true,ChatLayer.privateChatPlayerId,ChatLayer.privateChatName)
				end
			elseif ChatLayer.channelIndex == 3 then
				sendGuildNote(content,true)
			end
			ChatLayer.uiTextInput:setString("")
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= ChatLayer.uiLayerInstance then
			if G_CurLayerInstance ~= ChatLayer.uiLayerInstance then
				ChatLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

local function onFriendListBtClicked(tag)
	showUILayerByTag(UITagTable["FriendUILayer"].tag,true)
end

local function onShieldListBtClicked(tag)
	showUILayerByTag(UITagTable["ShieldLayer"].tag,true)
end

function ChatLayer:SetPPVEModle()
	self.chatStats = 1
end

function ChatLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseBtClicked)
	local sendBt = self.uiLayout:FindChildObjectByName("sendChatBt").__UIButton__:getMenuItemSprite()
	sendBt:registerScriptTapHandler(onSendMessageBtClicked)
	self.worldChannelToggle = self.uiLayout:FindChildObjectByName("channelBt1").__UIButtonToggle__
	self.worldChannelToggle:getMenuItemToggle():registerScriptTapHandler(onWorldChannelBtClicked)
	self.personalChannelToggle = self.uiLayout:FindChildObjectByName("channelBt2").__UIButtonToggle__
	self.personalChannelToggle:getMenuItemToggle():registerScriptTapHandler(onPersonalChannelBtClicked)
	self.guildChannelToggle = self.uiLayout:FindChildObjectByName("channelBt3").__UIButtonToggle__
	self.guildChannelToggle:getMenuItemToggle():registerScriptTapHandler(onGuildChannelBtClicked)
	self.worldChatToggle = self.uiLayout:FindChildObjectByName("chatChannelBt1").__UIButtonToggle__
	self.worldChatToggle:getMenuItemToggle():registerScriptTapHandler(onWorldChatBtClicked)
	self.personalChatToggle = self.uiLayout:FindChildObjectByName("chatChannelBt2").__UIButtonToggle__
	self.personalChatToggle:getMenuItemToggle():registerScriptTapHandler(onPersonalChatBtClicked)
	self.guildChatToggle = self.uiLayout:FindChildObjectByName("chatChannelBt3").__UIButtonToggle__
	self.guildChatToggle:getMenuItemToggle():registerScriptTapHandler(onGuildChatBtClicked)
	self.chatLabel = self.uiLayout:FindChildObjectByName("chatLabel").__UILabel__

	--local friendListBt = self.uiLayout:FindChildObjectByName("friendBt").__UIButton__:getMenuItemSprite();
	--friendListBt:registerScriptTapHandler(onFriendListBtClicked);
	local shieldListBt = self.uiLayout:FindChildObjectByName("shieldListBt").__UIButton__:getMenuItemSprite();
	shieldListBt:registerScriptTapHandler(onShieldListBtClicked);

	self.uiTextInput = self.uiLayout:FindChildObjectByName("chatInput").__UIEditBox__
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	self.worldChannelPage = ChatShowPage:create()
	self.worldChannelPage:addChatType("WORLD")
	self.worldChannelPage:addChatType("PARTY")
	self.worldChannelPage:addChatType("PM")
	self.worldChannelPage:init(self.uiLayout,"worldChatContainer","worldChatList",24,-135)

	self.guildChannelPage = ChatShowPage:create()
	self.guildChannelPage:addChatType("PARTY")
	self.guildChannelPage:init(self.uiLayout,"guildChatContainer","guildChatList",24,-135)

	self.personChannelPage = ChatShowPage:create()
	self.personChannelPage:addChatType("PM")
	self.personChannelPage:init(self.uiLayout,"personChatContainer","personChatList",24,-135)

	--PushOneLayer(self.uiLayerInstance,"","")
	--SetChatBar(false,-1)

	onWorldChannelBtClicked(nil)
	onWorldChatBtClicked(nil)
end