require("Script/GameConfig/uiTagDefine")

WorldBossRemainTimeLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	inBattle = false,
	isStart = false,
}



-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()

	WorldBossRemainTimeLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("WorldBossRemainTimeUI")
end

function WorldBossRemainTimeLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["WorldBossRemainTimeLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/WorldBossRemainTimeUI.plist",self.uiLayerInstance, "WorldBossRemainTimeUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function WorldBossRemainTimeLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.inBattle = false
	self.isStart = false
end

-- 删除UI
function WorldBossRemainTimeLayer:destroyed()
	CCLuaLog("--- WorldBossRemainTimeLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	TimeManager:Get():stopTimer(TIMER_WORLDBOSS_START_REMAIN)
	TimeManager:Get():stopTimer(TIMER_WORLDBOSS_END_REMAIN)

	self:resetValue()
end

local function onUpdateEndRemainTime(remainTime)
	if remainTime <= 0 then
		remainTime = 0
	end

	local remainStr = TimeManager:Get():secondsToString(remainTime)
	local remainTimeLabel = nil

	if	WorldBossRemainTimeLayer.inBattle then
		remainTimeLabel = WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainTime2").__UILabel__
	else
		remainTimeLabel = WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainTime1").__UILabel__
	end

	if	remainTimeLabel ~= nil then
		remainTimeLabel:setString(remainStr)
	end
end

-- 剩余时间更新
local function onUpdateStartRemainTime(remainTime)
	if remainTime <= 0 then
		remainTime = 0
	end

	if	remainTime > 0 then
		local remainStr = TimeManager:Get():secondsToString(remainTime)
		local remainTimeLabel = nil

		remainTimeLabel = WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainTime1").__UILabel__

		if	remainTimeLabel ~= nil then
			remainTimeLabel:setString(remainStr)
		end

		--隐藏传送点
		--SetPortalVisible(false)
	elseif not WorldBossRemainTimeLayer.isStart then
		local isOpen, nextStartTime, nextEndTime = GetWorldBossStartState()
		local nowTime = TimeManager:Get():getCurServerTime()
		TimeManager:Get():registerLuaTimer(onUpdateEndRemainTime, TIMER_WORLDBOSS_END_REMAIN, nextEndTime)

		local remainTextLabel = WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainText1").__UILabel__
		remainTextLabel:setString(GetLuaLocalization("M_WORLDBOSS_ACTIVITYTIME"))

		local remainStr = TimeManager:Get():secondsToString(nextEndTime - nowTime)
		local remainTimeLabel = WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainTime1").__UILabel__
		if	remainTimeLabel ~= nil then
			remainTimeLabel:setString(remainStr)
		end

		remainTextLabel:setColor(ccc3(0,255,0))
		remainTimeLabel:setColor(ccc3(0,255,0))	
		
		WorldBossRemainTimeLayer.isStart = true

		--显示传送点
		--SetPortalVisible(true)
	end
end

function WorldBossRemainTimeLayer:InitLayer()

	if self.inBattle then
		self.uiLayout:FindChildObjectByName("remainBG1").__UIPicture__:setVisible(false)
		self.uiLayout:FindChildObjectByName("remainText1").__UILabel__:setVisible(false)
		self.uiLayout:FindChildObjectByName("remainTime1").__UILabel__:setVisible(false)

		local isOpen, nextStartTime, nextEndTime = GetWorldBossStartState()
		local nowTime = TimeManager:Get():getCurServerTime()
		
		--战斗中一定为开启状态
		if isOpen then
			TimeManager:Get():registerLuaTimer(onUpdateEndRemainTime, TIMER_WORLDBOSS_END_REMAIN, nextEndTime)

			local remainTextLabel = self.uiLayout:FindChildObjectByName("remainText2").__UILabel__
			remainTextLabel:setString(GetLuaLocalization("M_WORLDBOSS_ACTIVITYTIME"))

			local remainStr = TimeManager:Get():secondsToString(nextEndTime - nowTime)
			local remainTimeLabel = self.uiLayout:FindChildObjectByName("remainTime2").__UILabel__
			if	remainTimeLabel ~= nil then
				remainTimeLabel:setString(remainStr)
			end

			remainTextLabel:setColor(ccc3(0,255,0))
			remainTimeLabel:setColor(ccc3(0,255,0))	

			WorldBossRemainTimeLayer.isStart = true
		end

	else
		self.uiLayout:FindChildObjectByName("remainBG2").__UIPicture__:setVisible(false)
		self.uiLayout:FindChildObjectByName("remainText2").__UILabel__:setVisible(false)
		self.uiLayout:FindChildObjectByName("remainTime2").__UILabel__:setVisible(false)

		local isOpen, nextStartTime, nextEndTime = GetWorldBossStartState()
		local nowTime = TimeManager:Get():getCurServerTime()
		if not isOpen then
			TimeManager:Get():registerLuaTimer(onUpdateStartRemainTime, TIMER_WORLDBOSS_START_REMAIN, nextStartTime)

			local remainTextLabel = self.uiLayout:FindChildObjectByName("remainText1").__UILabel__
			remainTextLabel:setString(GetLuaLocalization("M_WORLDBOSS_STARTTIME"))

			local remainStr = TimeManager:Get():secondsToString(nextStartTime - nowTime)
			local remainTimeLabel = self.uiLayout:FindChildObjectByName("remainTime1").__UILabel__
			if	remainTimeLabel ~= nil then
				remainTimeLabel:setString(remainStr)
			end

			remainTextLabel:setColor(ccc3(255,0,0))
			remainTimeLabel:setColor(ccc3(255,0,0))
		else
			TimeManager:Get():registerLuaTimer(onUpdateEndRemainTime, TIMER_WORLDBOSS_END_REMAIN, nextEndTime)

			local remainTextLabel = self.uiLayout:FindChildObjectByName("remainText1").__UILabel__
			remainTextLabel:setString(GetLuaLocalization("M_WORLDBOSS_ACTIVITYTIME"))

			local remainStr = TimeManager:Get():secondsToString(nextEndTime - nowTime)
			local remainTimeLabel = self.uiLayout:FindChildObjectByName("remainTime1").__UILabel__
			if	remainTimeLabel ~= nil then
				remainTimeLabel:setString(remainStr)
			end

			remainTextLabel:setColor(ccc3(0,255,0))
			remainTimeLabel:setColor(ccc3(0,255,0))			

			WorldBossRemainTimeLayer.isStart = true
		end
	end

	--未开启默认不显示
	self.uiLayout:FindChildObjectByName("unopenText").__UILabel__:setVisible(false)
end

function WorldBossRemainTimeLayer:OnEnd()
	TimeManager:Get():stopTimer(TIMER_WORLDBOSS_START_REMAIN)
	TimeManager:Get():stopTimer(TIMER_WORLDBOSS_END_REMAIN)

	if	not WorldBossRemainTimeLayer.inBattle then
		WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainText1").__UILabel__:setVisible(false)
		WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("remainTime1").__UILabel__:setVisible(false)
		WorldBossRemainTimeLayer.uiLayout:FindChildObjectByName("unopenText").__UILabel__:setVisible(true)
	end

	--隐藏传送点
	--SetPortalVisible(false)
end