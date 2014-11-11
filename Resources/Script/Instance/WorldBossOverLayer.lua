require("Script/GameConfig/uiTagDefine")

WorldBossOverLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,
	isWorldBossEnded = false,
}

WorldBossOverData = {}
WorldBossOverSelfRanking = {}
WorldBossOverLastKill = {}


-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if WorldBossOverLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(WorldBossOverLayer.notificationFunc);
	end

	WorldBossOverLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("WorldBossOverUI")
	TXGUI.UIManager:sharedManager():removeUILayout("WorldBossOverUI2")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= WorldBossOverLayer.uiLayerInstance then
			if G_CurLayerInstance ~= WorldBossOverLayer.uiLayerInstance then
				WorldBossOverLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

local function isPlayerInTop()
	local ret = false
	if WorldBossOverSelfRanking ~= nil then
		if WorldBossOverSelfRanking.rank ~= nil then
			if WorldBossOverSelfRanking.rank >= 1 and WorldBossOverSelfRanking.rank <= 3 then
				ret = true
			end
		end
	end
	return ret
end

function WorldBossOverLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["WorldBossOverLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)

			local isInTop = isPlayerInTop()
			if not isInTop then
				self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/WorldBossOverUI.plist",self.uiLayerInstance, "WorldBossOverUI", true)
			else
				self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/WorldBossOverUI2.plist",self.uiLayerInstance, "WorldBossOverUI2", true)
			end
			
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function WorldBossOverLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.isWorldBossEnded = false
end

-- 删除UI
function WorldBossOverLayer:destroyed()
	CCLuaLog("--- WorldBossOverLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
end

local function sendWorldBossBattleLeaveReq()
	CSWBBattleLeaveReq = {
		}
	local msgname="CSWBBattleLeaveReq"
	local ret = send_message(msgname, CSWBBattleLeaveReq, true)
	return ret;
end

local function onPressBackBtn(tag)
	--战斗内部才返回主城	
	local inWorldBossEntrance = LevelManager:sShareInstance():getCurMapId() == InstanceManager:Get():getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS)
	if not inWorldBossEntrance then
		sendWorldBossBattleLeaveReq()
		InstanceManager:Get():onLeaveSpecialEntrance(ENTERANCE_WORLDBOSS)
	else
		WorldBossOverLayer:destroyed()	
		SetChatBar(true,-1)
	end	

	CCDirector:sharedDirector():getScheduler():setTimeScale(1.0)
	
end

function WorldBossOverLayer:InitLayer()
	CCLuaLog("--- WorldBossOverLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	local backBtnMenu = self.uiLayout:FindChildObjectByName("backBtn").__UIButton__:getMenuItemSprite()
	backBtnMenu:registerScriptTapHandler(onPressBackBtn)

	self.isWorldBossEnded = true

	--战斗内部才隐藏面板
	local inWorldBossEntrance = LevelManager:sShareInstance():getCurMapId() == InstanceManager:Get():getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS)
	if not inWorldBossEntrance then
		OnWorldBossEndHideLayer()
	else
		HideOneLayerByTag(UITagTable["WorldBossDamageLayer"].tag)
		WorldBossRemainTimeLayer:OnEnd()
		SetChatBar(false,-1)
	end	

	GameDataManager:Get():setHeroAutoAttack(false)

	--排行榜上数据
	for index = 1, 3 do
		local rankNameLabel = self.uiLayout:FindChildObjectByName("name"..index).__UILabel__
		local rankDamageLabel = self.uiLayout:FindChildObjectByName("damage"..index).__UILabel__
		local rankCoinLabel = self.uiLayout:FindChildObjectByName("coinNum"..index).__UILabel__
		local rankReputationLabel = self.uiLayout:FindChildObjectByName("reputationNum"..index).__UILabel__

		if WorldBossOverData[index] ~= nil then
			local iconRankSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_RANK"..index)

			local iconRankPic = self.uiLayout:FindChildObjectByName("iconNo"..index).__UIPicture__
			iconRankPic:setSprite(iconRankSprite)

			rankNameLabel:setString(""..WorldBossOverData[index].rank.."."..WorldBossOverData[index].name)

			local hurtPercentStr = GetWorldBossDamagePercentStr(WorldBossOverData[index].percentage)
			rankDamageLabel:setString(""..MainMenuLayer:GetNumByFormatK(WorldBossOverData[index].hurt).."".."["..hurtPercentStr.."]")

			rankCoinLabel:setString(""..MainMenuLayer:GetNumByFormat(WorldBossOverData[index].coin))
			rankReputationLabel:setString(""..WorldBossOverData[index].reputation)
		else
			rankNameLabel:setString("")
			rankDamageLabel:setString("")
			rankCoinLabel:setString("")
			rankReputationLabel:setString("")

			self.uiLayout:FindChildObjectByName("iconCoin"..index).__UIPicture__:setVisible(false)
			self.uiLayout:FindChildObjectByName("iconReputation"..index).__UIPicture__:setVisible(false)
		end
	end

	--最后一击
	local lastNameLabel = self.uiLayout:FindChildObjectByName("name4").__UILabel__
	local lastDamageLabel = self.uiLayout:FindChildObjectByName("damage4").__UILabel__
	local lastCoinLabel = self.uiLayout:FindChildObjectByName("coinNum4").__UILabel__
	local lastReputationLabel = self.uiLayout:FindChildObjectByName("reputationNum4").__UILabel__

	if	WorldBossOverLastKill ~= nil and WorldBossOverLastKill.rank ~= nil and WorldBossOverLastKill.rank > 0 then
		lastNameLabel:setString(""..WorldBossOverLastKill.rank.."."..WorldBossOverLastKill.name)

		local hurtPercentStr = GetWorldBossDamagePercentStr(WorldBossOverLastKill.percentage)
		lastDamageLabel:setString(""..MainMenuLayer:GetNumByFormatK(WorldBossOverLastKill.hurt).."".."["..hurtPercentStr.."]")

		lastCoinLabel:setString(""..MainMenuLayer:GetNumByFormat(WorldBossOverLastKill.coin))
		lastReputationLabel:setString(""..WorldBossOverLastKill.reputation)
	else
		lastNameLabel:setString("")
		lastDamageLabel:setString("")

		lastCoinLabel:setVisible(false)
		lastReputationLabel:setVisible(false)

		self.uiLayout:FindChildObjectByName("title2").__UILabel__:setVisible(false)
		self.uiLayout:FindChildObjectByName("iconCoin4").__UIPicture__:setVisible(false)
		self.uiLayout:FindChildObjectByName("iconReputation4").__UIPicture__:setVisible(false)		
	end

	--自己的奖励
	local isInTop = isPlayerInTop()
	if not isInTop then
		local myNameLabel = self.uiLayout:FindChildObjectByName("myName").__UILabel__
		local myDamageLabel = self.uiLayout:FindChildObjectByName("myDamage").__UILabel__
		local myCoinLabel = self.uiLayout:FindChildObjectByName("myCoinNum").__UILabel__
		local myReputationLabel = self.uiLayout:FindChildObjectByName("myReputationNum").__UILabel__
		if	WorldBossOverSelfRanking ~= nil and WorldBossOverSelfRanking.rank ~= nil and WorldBossOverSelfRanking.rank > 0 then
			myNameLabel:setString(""..WorldBossOverSelfRanking.rank.."."..WorldBossOverSelfRanking.name)

			local hurtPercentStr = GetWorldBossDamagePercentStr(WorldBossOverSelfRanking.percentage)
			myDamageLabel:setString(""..MainMenuLayer:GetNumByFormatK(WorldBossOverSelfRanking.hurt).."".."["..hurtPercentStr.."]")

			myCoinLabel:setString(""..MainMenuLayer:GetNumByFormat(WorldBossOverSelfRanking.coin))
			myReputationLabel:setString(""..WorldBossOverSelfRanking.reputation)	
		else
			myNameLabel:setString("")
			myDamageLabel:setString("")
			myCoinLabel:setString("")
			myReputationLabel:setString("")

			self.uiLayout:FindChildObjectByName("title3").__UILabel__:setVisible(false)
			self.uiLayout:FindChildObjectByName("iconCoin5").__UIPicture__:setVisible(false)
			self.uiLayout:FindChildObjectByName("iconReputation5").__UIPicture__:setVisible(false)
		end
	else
		local myNameLabel = self.uiLayout:FindChildObjectByName("name"..WorldBossOverSelfRanking.rank).__UILabel__
		local myDamageLabel = self.uiLayout:FindChildObjectByName("damage"..WorldBossOverSelfRanking.rank).__UILabel__

		myNameLabel:setColor(ccc3(0,255,0))
		myDamageLabel:setColor(ccc3(0,255,0))
	end


end