require("Script/GameConfig/uiTagDefine")

BestFighterRemainTimeLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	BestFighterRemainTimeLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("BestFighterRemainTimeUI")
end

function BestFighterRemainTimeLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["BestFighterRemainTimeLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/BestFighterRemainTimeUI.plist",self.uiLayerInstance, "BestFighterRemainTimeUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function BestFighterRemainTimeLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function BestFighterRemainTimeLayer:destroyed()
	CCLuaLog("--- BestFighterRemainTimeLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	TimeManager:Get():stopTimer(TIMER_BESTFIGHTER_REMAIN)

	self:resetValue()
end

local function onUpdateRemainTime(remainTime)
	if remainTime <= 0 then
		remainTime = 0
	end

	local remainStr = TimeManager:Get():secondsToString(remainTime)
	local remainTimeLabel = nil

	remainTimeLabel = BestFighterRemainTimeLayer.uiLayout:FindChildObjectByName("remainTime").__UILabel__

	if	remainTimeLabel ~= nil then
		remainTimeLabel:setString(remainStr)
	end
end

function BestFighterRemainTimeLayer:InitLayer()

	local isOpen, nextStartTime, nextEndTime = GetBestFighterStartState()
	local nowTime = TimeManager:Get():getCurServerTime()

	TimeManager:Get():registerLuaTimer(onUpdateRemainTime, TIMER_BESTFIGHTER_REMAIN, nextEndTime)

	local remainTextLabel = self.uiLayout:FindChildObjectByName("remainText").__UILabel__
	remainTextLabel:setString(GetLuaLocalization("M_WORLDBOSS_ACTIVITYTIME"))

	local remainStr = TimeManager:Get():secondsToString(nextEndTime - nowTime)
	local remainTimeLabel = self.uiLayout:FindChildObjectByName("remainTime").__UILabel__
	if	remainTimeLabel ~= nil then
		remainTimeLabel:setString(remainStr)
	end

	remainTextLabel:setColor(ccc3(255,30,0))
	remainTimeLabel:setColor(ccc3(255,30,0))
end