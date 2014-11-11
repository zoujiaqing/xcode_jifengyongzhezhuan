require("Script/GameConfig/uiTagDefine")

BestFighterMatchEmptyLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
	rewardCoin = 0,
	rewardPoint = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if BestFighterMatchEmptyLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(BestFighterMatchEmptyLayer.notificationFunc);
	end

	BestFighterMatchEmptyLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("BestFighterMatchEmptyUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= BestFighterMatchEmptyLayer.uiLayerInstance then
			if G_CurLayerInstance ~= BestFighterMatchEmptyLayer.uiLayerInstance then
				BestFighterMatchEmptyLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function BestFighterMatchEmptyLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["BestFighterMatchEmptyLayer"].tag
			parentNode:addChild(self.uiLayerInstance,100,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/BestFighterMatchEmptyUI.plist",self.uiLayerInstance, "BestFighterMatchEmptyUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function BestFighterMatchEmptyLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.rewardCoin = 0
	self.rewardPoint = 0
end

-- 删除UI
function BestFighterMatchEmptyLayer:destroyed()
	CCLuaLog("--- BestFighterMatchEmptyLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
	SetChatBar(true,-1)
end

local function onPressConfirmBtn(tag)
	RemoveOneLayer(BestFighterMatchEmptyLayer.uiLayerInstance)
	BestFighterMatchEmptyLayer:destroyed()
	SendBestFighterJoinReq()
end

function BestFighterMatchEmptyLayer:InitLayer()
	CCLuaLog("--- BestFighterMatchEmptyLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	local backBtnMenu = self.uiLayout:FindChildObjectByName("confirmBtn").__UIButton__:getMenuItemSprite()
	backBtnMenu:registerScriptTapHandler(onPressConfirmBtn)

	self.uiLayout:FindChildObjectByName("coinNum").__UILabel__:setString(""..MainMenuLayer:GetNumByFormat(self.rewardCoin))
	self.uiLayout:FindChildObjectByName("pointNum").__UILabel__:setString(""..self.rewardPoint)

	PushOneLayer(self.uiLayerInstance,"","")

	HideOneLayerByTag(UITagTable["BackPackLayer"].tag)
	HideOneLayerByTag(UITagTable["OtherPlayerEquipLayer"].tag)
	HideOneLayerByTag(UITagTable["ChatPlayerOption"].tag)

	SetChatBar(false,-1)
end