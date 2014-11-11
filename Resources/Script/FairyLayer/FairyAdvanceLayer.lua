-- 精灵进阶页面
require("Script/FairyLayer/FairyMgr")
require("Script/GameConfig/Item")

FairyAdvanceLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
	fairyPos = 0,
	lastLayer = 0, -- 上一个界面，需要关闭后打开 1： 人物背包 2：精灵背包
    itemEnough = true,
    fairyEnough = true,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if FairyAdvanceLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyAdvanceLayer.notificationFunc);
	end

	FairyAdvanceLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyAdvanceUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyAdvanceLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyAdvanceLayer.uiLayerInstance then
				FairyAdvanceLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function FairyAdvanceLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyAdvanceLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyAdvanceUI.plist",self.uiLayerInstance, "FairyAdvanceUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyAdvanceLayer:resetValue()
    CCLuaLog("--- FairyAdvanceLayer:resetValue() ---")
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	FairyAdvanceLayer.fairyPos = 0
    FairyAdvanceLayer.itemEnough = true
    FairyAdvanceLayer.fairyEnough = true
end

-- 删除UI
function FairyAdvanceLayer:destroyed()
	--CCLuaLog("--- FairyAdvanceLayer:destroyed() ---")
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
	RemoveOneLayer(FairyAdvanceLayer.uiLayerInstance)
	FairyAdvanceLayer:destroyed()

	if FairyAdvanceLayer.lastLayer == 1 then
		showUILayerByTag(UITagTable["BackPackLayer"].tag,true)
	elseif FairyAdvanceLayer.lastLayer == 2 then
        CCLuaLog("open FairyBagLayer after close")
		showUILayerByTag(UITagTable["FairyBagLayer"].tag,true)
	end
end

-- 判断是否为顶阶或不可升阶
local function canFairyAdvance(pos)
	local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)
	local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)

	local canAdvance = true
	if fairyBasicInfo.quality == 1 or fairyBasicInfo.quality == 2 then
		canAdvance = false	
	end

	return canAdvance
end

-- 判断是否升到了顶阶
local function isGradeMax(pos)
    local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)
	local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)

    local isMax = false
	if userFairyInfo.fairyGrade == 7 then
		isMax = true
	elseif fairyBasicInfo.quality == 3 and userFairyInfo.fairyGrade == 2 then
		isMax = true
	end

	return isMax
end

-- 更新精灵阶级
local function refreshFairyGradeInfo()
	local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)
	local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)
    local quality = fairyBasicInfo.quality

	local fairyName = fairyBasicInfo.name
	local fairyGrade = userFairyInfo.fairyGrade

	--左右两个名字
	local nameStr1 = fairyName.." + "..fairyGrade
	local nameStr2 = fairyName.." + "..(fairyGrade + 1)

    local nameLabel1 = FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyName1").__UILabel__
    local nameLabel2 = FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyName2").__UILabel__

    nameLabel1:setString(nameStr1)
    nameLabel1:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))

    nameLabel2:setString(nameStr2)
    nameLabel2:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))
end


--更新精灵基础信息
local function refreshFairyBasicInfo()
	if FairyAdvanceLayer.fairyPos == 0 or FairyAdvanceLayer.uiLayout == nil then
		return
	end

	-- 更新精灵阶级
	refreshFairyGradeInfo()

	--等级
	local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)
	local level = userFairyInfo.fairyLevel
	local levelStr = "Lv."..level

	FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyLevel1").__UILabel__:setString(levelStr)
	FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyLevel2").__UILabel__:setString(levelStr)

	--精灵动画
	local fairyId = userFairyInfo.fairyId

	for i = 1, 2 do
		local fairyFrame = FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyFrame"..i).__UIPicture__
		local size = fairyFrame:getCurrentNode():getContentSize()		
		local px = size.width / 2
		local py = size.height / 2
		local modePt = CCPoint(px,py)								
		SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(fairyFrame:getCurrentNode(), fairyId, modePt)
	end

end

local costItemIds = {}

local advancePillDelta = 0
local costCoin = 0

-- 更新所需物品信息
local function refreshCostItems(costItemTable)
	
	local costItems = {}

	for i = 1, #costItemTable do
		if 416001 == costItemTable[i].id then
			costCoin = costItemTable[i].num
		else
			costItems[#costItems + 1] = costItemTable[i]
		end
	end

	-- 金币更新
	local coinLabel = FairyAdvanceLayer.uiLayout:FindChildObjectByName("coinNum").__UILabel__
	coinLabel:setString(""..costCoin)

	-- 物品更新
	local fairyEnoughFlag = true
    local itemEnoughFlag = true

	for i = 1, 4 do
		CCLuaLog("--- costItems : "..i)
		if costItems[i] ~= nil then
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemIcon"..i):setVisible(true)
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemBtn"..i):setVisible(true)
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemNum"..i):setVisible(true)

			local itemId = costItems[i].id
			local needNum = costItems[i].num

			CCLuaLog("--- needItem : "..itemId)

			-- 判断是物品还是精灵
			local nowNum = 0
			local itemSprite = nil
            local isItem = nil
			if Items[itemId] ~= nil then
                isItem = true
				nowNum = ItemManager:Get():getItemNumberById(itemId)
				itemSprite = ItemManager:Get():getIconSpriteById(itemId)
			else
                isItem = false
				nowNum = GetAdvanceFairyCountById(itemId, FairyAdvanceLayer.fairyPos)

				local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)
				local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)
				itemSprite = fairyBasicInfo:getFairyIcon()
			end
			
			

			-- 物品图标设置			
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemIcon"..i).__UIPicture__:setSprite(itemSprite, false)

			-- 物品数量设置
			local itemLabel = FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemNum"..i).__UILabel__
			itemLabel:setString(""..nowNum.."/"..needNum)

            -- 获得进阶丹差额
            if itemId == 416020 then
                advancePillDelta = needNum - nowNum
            end

			if nowNum < needNum then
                if isItem then
                    itemEnoughFlag = false
                    CCLuaLog("--- itemEnoughFlag : false ---")
                else
                    fairyEnoughFlag = false
                    CCLuaLog("--- fairyEnoughFlag : false ---")
                end
				itemLabel:setColor(ccc3(255,0,0))
			else
				itemLabel:setColor(ccc3(0,255,0))
			end

			-- 存下物品id供回调
			costItemIds[i] = itemId
		else
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemIcon"..i):setVisible(false)
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemBtn"..i):setVisible(false)
			FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemNum"..i):setVisible(false)
		end		
	end

	itemEnough = itemEnoughFlag
    fairyEnough = fairyEnoughFlag
end

-- 达到最高阶之后更新面板
local function updateMaxGradeInfo()
	refreshFairyGradeInfo()
	refreshCostItems({})

	local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)
	RefreshAttrPanel(FairyAdvanceLayer.uiLayout, userFairyInfo.fairyAttr , nil)

	FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyName2").__UILabel__:setString(GetLuaLocalization("M_AD_ERROR3"))
	FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyName2").__UILabel__:setColor(ccc3(255,255,0))
	FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyLevel2").__UILabel__:setVisible(false)

	-- 隐藏进阶后属性
	for i = 1, 6 do
		FairyAdvanceLayer.uiLayout:FindChildObjectByName("attrUpNum"..i).__UILabel__:setVisible(false)
	end

	-- 金币信息
	FairyAdvanceLayer.uiLayout:FindChildObjectByName("coinNum").__UILabel__:setString("0")
end

--发送精灵进阶消息
local function sendFairyAdvanceReq(view)
	CSFairyUpGradeLevelReq = {
		fairy_pos = FairyAdvanceLayer.fairyPos,
		req_info = view,
		}
	local msgname="CSFairyUpGradeLevelReq"
	local ret = send_message(msgname, CSFairyUpGradeLevelReq, true)
	return ret
end

--收到精灵进阶消息
local function onMsgFairyAdvanceRsp(msg)
	CCLuaLog("--- onMsgFairyAdvanceRsp ---")

	if msg ~= nil then
		if msg.fairy_pos ~= FairyAdvanceLayer.fairyPos then
			return
		end

		-- 进阶成功后不刷新页面，再次请求查看，每次都用查看信息来刷新
		if msg.succ then
			GameApi:showMessage(GetLuaLocalization("M_AD_TEXT2"))
			GameAudioManager:sharedManager():playEffect(320000,false)
			-- 升阶特效
			local fairyIcon = FairyAdvanceLayer.uiLayout:FindChildObjectByName("fairyFrame1").__UIPicture__
			local frameSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_AD1_ANIMATION_FRAME")
			local frameSize = frameSprite:getContentSize()

			local particle = ParticleManager:Get():createEffectSpriteAndPlay(443,"", 100000, 1, false);
			particle:setPosition(ccp(frameSize.width/2, 0))

			fairyIcon:getCurrentNode():addChild(particle, 10)

			-- 延迟一段时间再刷新界面或发送新的请求
			local function onParticleFinish()
				if canFairyAdvance(FairyAdvanceLayer.fairyPos) and not isGradeMax(FairyAdvanceLayer.fairyPos) then
					sendFairyAdvanceReq(true)
                    PushOneWaitingLayer("CSFairyUpGradeLevelReq")
				else
					updateMaxGradeInfo()
				end
			end

			local delayAction =	CCDelayTime:create(1.0)
			local callback = CCCallFunc:create(onParticleFinish)
			local action = CCSequence:createWithTwoActions(delayAction,callback)
			fairyIcon:getCurrentNode():runAction(action)
			
			return
		end

		refreshFairyGradeInfo()

		-- 消耗物品信息，包括金币
		if msg.consume_items ~= nil then
			CCLuaLog("--- msg.consume_items ~= nil ---")
			local items = {}
			for _ , item in pairs(msg.consume_items) do		
				items[#items + 1] = {
					id = item.id,
					num = item.num,
				}				
			end

			refreshCostItems(items)
		end

		--属性信息
		if msg.fairy_info ~= nil then
			local nextAttrs = {}
			for _ , attr in pairs(msg.fairy_info.fairy_attrs) do
				nextAttrs[attr.key] = attr.value			
			end

			-- 获得当前属性列表
			local userFairyInfo = getUserFairyByPos(FairyAdvanceLayer.fairyPos)

			RefreshAttrPanel(FairyAdvanceLayer.uiLayout, userFairyInfo.fairyAttr , nextAttrs)
		end
	end

end

-- 发送兑换进阶丹请求
local function sendResetBoxReq(deltaPill)
	CSExchangeParameterReq = {
		id = 21080,
        param = {
            deltaPill
            }
		}
	local msgname="CSExchangeParameterReq"
	local ret = send_message(msgname, CSExchangeParameterReq, true)
	return ret;
end

--点击进阶按钮
local function onPressAdvanceBtn(tag)
	CCLuaLog("--- onPressAdvanceBtn ---")
	-- 不能进阶
	if not canFairyAdvance(FairyAdvanceLayer.fairyPos) then
		GameApi:showMessage(GetLuaLocalization("M_AD_ERROR2"))
		return
	end

    --已达到最高阶
    if isGradeMax(FairyAdvanceLayer.fairyPos) then
		GameApi:showMessage(GetLuaLocalization("M_AD_ERROR2"))
		return
	end

    local nowCoin = UserData:GetUserInfo().m_gold
    if nowCoin < costCoin then
        GameApi:showMessage(GetLuaLocalization("M_FINDSPIRIT_COINSHORTAGE"))
		return
    end
	
	if not fairyEnough then
        -- 精灵不足   
        local function onConfirm()
            --直接关闭不返回背包
            RemoveOneLayer(FairyAdvanceLayer.uiLayerInstance)
	        FairyAdvanceLayer:destroyed()

            MainMenuLayer:getMainMenuLayer():ShowMarketLayer(true, 0)
        end  

        MessageBox:Show(GetLuaLocalization("M_CANT_FAIRYAD1"),onConfirm,nil,MB_YESNO,ccc3(255,255,255),26)
        return
    elseif not itemEnough then 

		if advancePillDelta > 0 then
			GameApi:showMessage(GetLuaLocalization("M_UPGRADEROCK_NOTENOUGH"))
		else
			GameApi:showMessage(GetLuaLocalization("M_FAIRY_NUMBER_LACK"))				
		end

        	
		

		--[[
        local needDiamondCount = advancePillDelta * 3   
        CCLuaLog("needDiamondCount : "..needDiamondCount)
        local function onConfirm()
            local nowDiamondCount = UserData:GetDiamond()
            if needDiamondCount > nowDiamondCount then
                PayHintLayer:CreateLayer()
            else
                --发送钻石兑换进阶丹协议
                sendResetBoxReq(advancePillDelta)
                --发送进阶协议
                sendFairyAdvanceReq(false)
                PushOneWaitingLayer("CSFairyUpGradeLevelReq")
            end
        end
        
        DiamondWarningLayer:Show(GetLuaLocalization("M_CANT_FAIRYAD2"),needDiamondCount,onConfirm,nil,ccc3(255,255,255),"","")
		
		--]]
		return
	end


	sendFairyAdvanceReq(false)
    PushOneWaitingLayer("CSFairyUpGradeLevelReq")
end

local tipFairyId = 0
local function onOperationTipCallBackLeft(tag)

end

local function onOperationTipCallBackRight(tag)
	if tipFairyId ~= 0 then
		FairyInfoLayer.fairyId = tipFairyId
		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 2
		FairyInfoLayer:CreateLayer(nil)
	end
end

-- 点击物品按钮
local function onPressItemBtn(tag)
	CCLuaLog("--- onPressItemBtn ---")
	
	local menu = FairyAdvanceLayer.uiLayout:FindChildObjectByName("itemBtn"..tag).__UIButton__:getMenuNode()
    local pos = menu:getTouchPoint()

	ItemManager:Get():showItemTipsById(costItemIds[tag], pos)

	local isFairy = G_GetIsFairyById(tonumber(costItemIds[tag]))
	local info = ItemManager:Get():getItemInfoById(tonumber(costItemIds[tag]))
	if isFairy or (info ~= nil and info.m_type == 21) then
		tipFairyId = costItemIds[tag] % 10000;
		local detailText = LanguageLocalization:GetLocalization("M_EQUIPMENT_INFORMATION")
		ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onOperationTipCallBackRight,detailText,true)
	end
end

function FairyAdvanceLayer:InitLayer()
	CCLuaLog("--- FairyAdvanceLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
	self.uiLayout:FindChildObjectByName("advanceBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressAdvanceBtn)

	--物品按钮回调，显示tips
	for i = 1, 4 do
		self.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__:getMenuItemSprite():setTag(i)
		self.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressItemBtn)	
	end
	

	refreshFairyBasicInfo()

	--发送查看进阶消息
	if canFairyAdvance(FairyAdvanceLayer.fairyPos) and not isGradeMax(FairyAdvanceLayer.fairyPos) then
		sendFairyAdvanceReq(true)
        PushOneWaitingLayer("CSFairyUpGradeLevelReq")
	else
		updateMaxGradeInfo()
	end

	

	--消息回调注册
	addMsgCallBack("CSFairyUpGradeLevelRsp", onMsgFairyAdvanceRsp)

	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)
end