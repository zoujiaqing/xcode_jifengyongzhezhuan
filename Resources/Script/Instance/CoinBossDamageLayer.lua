-- 金币副本实时伤害页面

CoinBossDamageLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if CoinBossDamageLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(CoinBossDamageLayer.notificationFunc);
	end

	CoinBossDamageLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("CoinBossDamageLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= CoinBossDamageLayer.uiLayerInstance then
			if G_CurLayerInstance ~= CoinBossDamageLayer.uiLayerInstance then
				CoinBossDamageLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function CoinBossDamageLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["CoinBossDamageLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/CoinBossDamageUI.plist",self.uiLayerInstance, "CoinBossDamageLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

local lastMaxHurt = 0

-- 重置参数
function CoinBossDamageLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil

	lastMaxHurt = 0
end

-- 删除UI
function CoinBossDamageLayer:destroyed()
	CCLuaLog("--- CoinBossDamageLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	TimeManager:Get():stopTimer(TIMER_COIN_BOSS_REMAIN)

	self:resetValue()
    --SetChatBar(true,-1)
end

function CoinBossDamageLayer:InitLayer()
	CCLuaLog("--- CoinBossDamageLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)


	local firstHurt = AwardDaily[4300].Active
	RefreshCoinBossRealTimeHurt(0, firstHurt)

end

local function onUpdateRemainTime(remainTime)
	CCLuaLog("--- CoinBossDamageLayer:onUpdateRemainTime ---")
	
	if remainTime <= 0 then
		remainTime = 0
	end

	local remainStr = TimeManager:Get():secondsToString(remainTime)
	local remainTimeLabel = nil

	remainTimeLabel = CoinBossDamageLayer.uiLayout:FindChildObjectByName("remainTime").__UILabel__

	if	remainTimeLabel ~= nil then
		remainTimeLabel:setString(remainStr)
	end

end

function InitCoinBossRemainTime(remainTime)
	local nowTime = TimeManager:Get():getCurServerTime()
	TimeManager:Get():registerLuaTimer(onUpdateRemainTime, TIMER_COIN_BOSS_REMAIN, nowTime + remainTime)

	local remainStr = TimeManager:Get():secondsToString(remainTime)
	local remainTimeLabel = CoinBossDamageLayer.uiLayout:FindChildObjectByName("remainTime").__UILabel__

	if	remainTimeLabel ~= nil then
		remainTimeLabel:setString(remainStr)
	end
end

function RefreshCoinBossRealTimeHurt(nowHurt, nextHurt)
	if CoinBossDamageLayer.uiLayout == nil then
		return
	end

	local hurtLabel = CoinBossDamageLayer.uiLayout:FindChildObjectByName("hurtNum").__UILabel__

	hurtLabel:setColor(ccc3(255, 84, 0))


	hurtLabel:setString(""..nowHurt.."/"..nextHurt)

	local function labelFunc()
		local scale = TXGUI.UIManager:sharedManager():getScaleFactor()

		local arrayOfActions = CCArray:create()

		local scale1 = CCScaleTo:create(0.1, 2 * scale)
		local scale2 = CCScaleTo:create(0.1, 1 * scale)
		local scale3 = CCScaleTo:create(0.1, 2 * scale)
		local scale4 = CCScaleTo:create(0.1, 1 * scale)

		arrayOfActions:addObject(scale1)
		arrayOfActions:addObject(scale2)
		arrayOfActions:addObject(scale3)
		arrayOfActions:addObject(scale4)

		return CCSequence:create(arrayOfActions)
	end

	if lastMaxHurt ~= 0 and lastMaxHurt ~= nextHurt then		
		hurtLabel:getCurrentNode():runAction(labelFunc())
		GameAudioManager:sharedManager():playEffect(350012,false)
	end
	lastMaxHurt = nextHurt
end