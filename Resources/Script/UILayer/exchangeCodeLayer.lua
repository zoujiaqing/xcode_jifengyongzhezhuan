
ExchangeCodelUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	uiTextInput = nil,
}

local function onReceiveDestructorHandler()
	RemoveOneLayer(ExchangeCodelUILayer.uiLayerInstance)
	--if HelpUILayer.notificationFunc ~= nil then
		--NotificationCenter:defaultCenter():unregisterScriptObserver(HelpUILayer.notificationFunc);
	--end
	removeMsgCallBack("CSMagicCardRsp", onReceiveCSMagicCardRsp)
	TXGUI.UIManager:sharedManager():removeUILayout("ExchangeCodeLayout")
	ExchangeCodelUILayer:resetValue()
end

function ExchangeCodelUILayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ExchangeCodeUI"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/exchangeCodeUI.plist",self.uiLayerInstance, "ExchangeCodeLayout", true);
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

function ExchangeCodelUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiTextInput = nil
end

function ExchangeCodelUILayer:destroy()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

local function sendExchangeCodeToServer(card)
	CSMagicCardReq = {
		card_passwd = card;
		type = "EXCHANGE";
	}
	local msgname ="CSMagicCardReq"
	local ret = send_message(msgname, CSMagicCardReq, true)
	return ret;
end

local function onReceiveCSMagicCardRsp(msg)
	CCLuaLog("onReceiveCSMagicCardRsp")
	if msg ~= nil then
		if msg.succ then
			local text = LanguageLocalization:GetLocalization("M_FINDSPIRIT_SPIRITCHANGE_SUCCESS")
			GameApi:showMessage(text)
			ExchangeCodelUILayer:destroy()
		end
	end
end

local function onCloseButtonClicked()
	ExchangeCodelUILayer:destroy()
end

local function onConfirmButtonClicked()
	if ExchangeCodelUILayer.uiTextInput ~= nil then
		local exchangeCode = ExchangeCodelUILayer.uiTextInput:getString()
		if exchangeCode ~= nil and exchangeCode ~= "" then
			sendExchangeCodeToServer(exchangeCode)
		end
	end
end

function ExchangeCodelUILayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	confirmBt:registerScriptTapHandler(onConfirmButtonClicked)
	--self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
	self.uiTextInput = self.uiLayout:FindChildObjectByName("textInput").__UIEditBox__
	addMsgCallBack("CSMagicCardRsp", onReceiveCSMagicCardRsp)
	PushOneLayer(self.uiLayerInstance,"","")
end