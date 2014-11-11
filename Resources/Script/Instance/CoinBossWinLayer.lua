-- 金币副本胜利页面

CoinBossWinLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

	winData = {},
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if CoinBossWinLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(CoinBossWinLayer.notificationFunc);
	end

	CoinBossWinLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("CoinBossWinLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= CoinBossWinLayer.uiLayerInstance then
			if G_CurLayerInstance ~= CoinBossWinLayer.uiLayerInstance then
				CoinBossWinLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function CoinBossWinLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["CoinBossWinLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/CoinBossWinUI.plist",self.uiLayerInstance, "CoinBossWinLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

local nowFactor = 0
-- 重置参数
function CoinBossWinLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil

	nowFactor = 0
end

-- 删除UI
function CoinBossWinLayer:destroyed()
	CCLuaLog("--- CoinBossWinLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
    SetChatBar(true,-1)
end

-- 离开副本
local function onPressLeaveBtn(tag)
    CCLuaLog("--- onPressShowRewardLayer ---")
	XLogicManager:sharedManager():LeaveBattle()
end

-- 翻倍奖励
local function onPressDoubleBtn(tag)
    CCLuaLog("--- onPressShowRewardLayer ---")

	local function sendDoubleReq()
		local msgBody = {
		}
		send_message("CSCBDoubleReq", msgBody, true)
	end

	sendDoubleReq()
	PushOneWaitingLayer("CSCBDoubleReq")
end

-- 收到翻倍请求
local function onMsgDoubleRsp(msg)
	CCLuaLog("--------------- onMsgDoubleRsp-----------")
	local data = {}
	data.factor = msg.factor

	CCLuaLog("factor : "..msg.factor)
	data.nextNum = msg.next_item_num
	data.nextCost = msg.next_cost_num
	RefreshRewardFactor(data)
end


function CoinBossWinLayer:InitLayer()
	CCLuaLog("--- CoinBossWinLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	local winData = self.winData

	-- 最高伤害
	self.uiLayout:FindChildObjectByName("maxHurtNum").__UILabel__:setString(""..winData.maxHurt)

	-- 本次伤害
	self.uiLayout:FindChildObjectByName("nowHurtNum").__UILabel__:setString(""..winData.nowHurt)

	-- 当前排名
	self.uiLayout:FindChildObjectByName("rankNum").__UILabel__:setString(""..winData.rankNum)

	-- 排名上升处理
	if winData.rankChange > 0 then
		self.uiLayout:FindChildObjectByName("rankChangeNum").__UILabel__:setString(""..winData.rankChange)
		local upIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_497023")
		self.uiLayout:FindChildObjectByName("changeIcon").__UIPicture__:setSprite(upIcon)
	else
		self.uiLayout:FindChildObjectByName("rankChangeNum").__UILabel__:setString("")
	end

	-- 判断是否新纪录
	if winData.maxHurt == winData.nowHurt then
		local newRecordIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_NEWRECORD")
		local newRecordPic = self.uiLayout:FindChildObjectByName("newrecordPic").__UIPicture__
		newRecordPic:setSprite(newRecordIcon)

		local arrayOfActions = CCArray:create()

		local scale1 = CCScaleTo:create(0.5, 2)
		local scale2 = CCScaleTo:create(0.5, 1)

		arrayOfActions:addObject(scale1)
		arrayOfActions:addObject(scale2)

		local sequence = CCSequence:create(arrayOfActions)
		--local repeatAct =  CCRepeatForever:create(tolua.cast(sequence, 'CCActionInterval'))
		newRecordPic:getCurrentNode():runAction(sequence)
	end

	-- 奖励物品
	local itemIcon = ItemManager:Get():getIconSpriteById(winData.itemId)
	self.uiLayout:FindChildObjectByName("rewardIcon").__UIPicture__:setSprite(itemIcon)	

	-- 按钮回调
	self.uiLayout:FindChildObjectByName("leaveBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressLeaveBtn)
	self.uiLayout:FindChildObjectByName("againBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressDoubleBtn)

	local rewardBtn = self.uiLayout:FindChildObjectByName("rewardFrameBtn").__UIButton__

    local function onPressRewardFrame(tag)
        
        local menu = rewardBtn:getMenuNode()
        local pos = menu:getTouchPoint()

        ItemManager:Get():showItemTipsById(winData.itemId, pos)
    end

	rewardBtn:getMenuItemSprite():registerScriptTapHandler(onPressRewardFrame)

	addMsgCallBack("CSCBDoubleRsp", onMsgDoubleRsp)

    PushOneLayer(self.uiLayerInstance,"","")
    SetChatBar(false,-1)
end



function RefreshRewardFactor(data)
	-- 倍率
	local label = CCLabelAtlas:create(""..data.factor, "UI/ui_numble_4.png", 41, 43, 48)
	CoinBossWinLayer.uiLayout:FindChildObjectByName("numberIcon1").__UIPicture__:getCurrentNode():removeAllChildrenWithCleanup(true)
	CoinBossWinLayer.uiLayout:FindChildObjectByName("numberIcon1").__UIPicture__:getCurrentNode():addChild(label)
	
	if nowFactor ~= data.factor then
		local arrayOfActions = CCArray:create()

		local scale1 = CCScaleTo:create(0.1, 2)
		local scale2 = CCScaleTo:create(0.1, 1)
		local scale3 = CCScaleTo:create(0.1, 2)
		local scale4 = CCScaleTo:create(0.1, 1)

		arrayOfActions:addObject(scale1)
		arrayOfActions:addObject(scale2)
		arrayOfActions:addObject(scale3)
		arrayOfActions:addObject(scale4)

		local sequence = CCSequence:create(arrayOfActions)
		label:runAction(sequence)
	end
	
	nowFactor = data.factor

	-- 再来X次文本
	local str = GetLuaLocalization("M_AGAIN_GET1")..data.nextNum..GetLuaLocalization("M_AGAIN_GET2")
	CoinBossWinLayer.uiLayout:FindChildObjectByName("againText").__UILabel__:setString(str)

	-- 钻石数
	CoinBossWinLayer.uiLayout:FindChildObjectByName("diaNum").__UILabel__:setString(""..data.nextCost)
end