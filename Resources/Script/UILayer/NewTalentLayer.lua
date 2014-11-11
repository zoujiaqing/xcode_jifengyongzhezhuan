-- 新天赋页面

NewTalentLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if NewTalentLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(NewTalentLayer.notificationFunc);
	end

	NewTalentLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("NewTalentLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= NewTalentLayer.uiLayerInstance then
			if G_CurLayerInstance ~= NewTalentLayer.uiLayerInstance then
				NewTalentLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function NewTalentLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["NewTalentLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/NewTalentLayer.plist",self.uiLayerInstance, "NewTalentLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function NewTalentLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function NewTalentLayer:destroyed()
	CCLuaLog("--- NewTalentLayer:destroyed() ---")
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
    RemoveOneLayer(NewTalentLayer.uiLayerInstance)
	NewTalentLayer:destroyed()
end

function NewTalentLayer:InitLayer()
	CCLuaLog("--- NewTalentLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)


    PushOneLayer(self.uiLayerInstance,"","")
    SetChatBar(false,-1)
end
