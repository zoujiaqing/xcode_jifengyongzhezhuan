-- 称号页面

TitleLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if TitleLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(TitleLayer.notificationFunc);
	end

	TitleLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("TitleLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= TitleLayer.uiLayerInstance then
			if G_CurLayerInstance ~= TitleLayer.uiLayerInstance then
				TitleLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function TitleLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["TitleLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/TitleLayer.plist",self.uiLayerInstance, "TitleLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function TitleLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function TitleLayer:destroyed()
	CCLuaLog("--- TitleLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
    SetChatBar(true,-1)
end

--关闭页面
local function onPressCloseBtn(tag)
    CCLuaLog("--- onPressCloseBtn ---")
    RemoveOneLayer(TitleLayer.uiLayerInstance)
	TitleLayer:destroyed()
end

function TitleLayer:InitLayer()
	CCLuaLog("--- TitleLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
    self.uiLayout:FindChildObjectByName("detailLayer"):setVisible(false)


    PushOneLayer(self.uiLayerInstance,"","")
    SetChatBar(false,-1)
end
