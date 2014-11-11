ChangeNameLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	uiTextInput = nil,
	notificationFunc = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	removeMsgCallBack("CSChangeNameRsp", onReceiveCSChangeNameRsp)
	TXGUI.UIManager:sharedManager():removeUILayout("ChangeNameLayout")
	if ChangeNameLayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(ChangeNameLayer.notificationFunc)
	end
	ChangeNameLayer:resetValue()
end

function ChangeNameLayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ChangeNameLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,30,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ChangeNameUI.plist",self.uiLayerInstance, "ChangeNameLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function ChangeNameLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.uiTextInput = nil
end

function ChangeNameLayer:destroy()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= ChangeNameLayer.uiLayerInstance then
			if G_CurLayerInstance ~= ChangeNameLayer.uiLayerInstance then
				if ChangeNameLayer.uiLayerInstance ~= nil then
					ChangeNameLayer.uiLayerInstance:removeFromParentAndCleanup(true)
				end
				ret = 1
			end
		end
	end

	return ret
end

local function sendNewNameToServer(new_name)
	CSChangeNameReq = {
		nick_name = new_name;
	}
	local msgname ="CSChangeNameReq"
	local ret = send_message(msgname, CSChangeNameReq, true)
	return ret;
end

local function onReceiveCSChangeNameRsp(msg)
	if msg ~= nil then
		if msg.res then
			local name = msg.nick_name
			UserData:setUserName(name)
			NotificationCenter:defaultCenter():broadcast(GM_GET_PLAYER_CHANGE_NAME,nil,"","")
			ChangeNameLayer:destroy()
		end
	end
end

local function onCloseButtonClicked(tag)
	ChangeNameLayer:destroy()
end

local function onConfirmButtonClicked(tag)
	if ChangeNameLayer.uiTextInput ~= nil then
		local newName = ChangeNameLayer.uiTextInput:getString()
		if newName ~= nil and newName ~= "" then
			sendNewNameToServer(newName)
		end
	end
end

function ChangeNameLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	confirmBt:registerScriptTapHandler(onConfirmButtonClicked)
	self.uiTextInput = self.uiLayout:FindChildObjectByName("NameInput").__UIEditBox__
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	addMsgCallBack("CSChangeNameRsp", onReceiveCSChangeNameRsp)
end