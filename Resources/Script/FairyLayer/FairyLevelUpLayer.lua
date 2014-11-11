-- 精灵升级页面
require("Script/Fairy/FairySelectLayer")

FairyLevelUpLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

	fairyPos = 0,
	selectFairysPos = {},

	nowExpSprite = nil,
	nextExpSprite = nil,

	selectTab = 1,
	lastLayer = 0, -- 上一个界面，需要关闭后打开 1： 人物背包 2：精灵背包
}

local FRIRY_SER_COUNT = 6

local fairyElementCount = 0
local nextNeedElementCount = 0
local nextFiveNeedCount = 0
local scaleSpeed = 2

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if FairyLevelUpLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyLevelUpLayer.notificationFunc);
	end

	FairyLevelUpLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyLevelUpUI")
end

-- 更新选择精灵信息
local function refreshSelectFairyPanel()
	local fairys = FairyLevelUpLayer.selectFairysPos
	for i = 1, FRIRY_SER_COUNT do
		if fairys[i] ~= nil then
			local userFairyInfo = getUserFairyByPos(fairys[i])
			local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)

			local fairySprite = fairyBasicInfo:getFairyIcon()
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairySerIcon"..i).__UIPicture__:setSprite(fairySprite)            
		else
			local addMarkIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_ADDFAIRY")
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairySerIcon"..i).__UIPicture__:setSprite(addMarkIcon)           
		end
	end
end

-- 发送精灵升级请求
local function sendFairyLevelupReq(selectFairyTable, elementCount, view)

	CSFairyAddExpReq = {
		fairy_pos = FairyLevelUpLayer.fairyPos,
		del_fairy_poss = selectFairyTable,
		fairy_elem = elementCount,
		req_info = view,
		}
	local msgname="CSFairyAddExpReq"
	local ret = send_message(msgname, CSFairyAddExpReq, true)
	return ret
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyLevelUpLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyLevelUpLayer.uiLayerInstance then
				FairyLevelUpLayer:destroyed()
				ret = 1
			end
		end
	elseif message == GM_FAIRY_SELECT_DONE then
		if FairyLevelUpLayer.uiLayerInstance ~= nil then
			FairyLevelUpLayer.selectFairysPos = FairySelectLayer:getSelectedFairys()
			refreshSelectFairyPanel()
			if FairyLevelUpLayer.selectTab == 1 then
				sendFairyLevelupReq(FairyLevelUpLayer.selectFairysPos, 0, true)
                PushOneWaitingLayer("CSFairyAddExpReq")
			end	
		end		
	end

	return ret
end

function FairyLevelUpLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyLevelUpLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyLevelUpUI.plist",self.uiLayerInstance, "FairyLevelUpUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
local expGrowing = false
function FairyLevelUpLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil

	self.fairyPos = 0
	self.selectFairysPos = {}

	self.nowExpSprite = nil
	self.nextExpSprite = nil

	self.selectTab = 1
    expGrowing = false
end

-- 删除UI
function FairyLevelUpLayer:destroyed()
	CCLuaLog("--- FairyLevelUpLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
	SetChatBar(true,-1)
end

local function initExpBar()

	local function blinkSprite(sprite)
		local arrayOfActions = CCArray:create()
		local fade1 = CCFadeTo:create(1, 0)
		local fade2 = CCFadeTo:create(1, 255)

		arrayOfActions:addObject(fade1)
		arrayOfActions:addObject(fade2)

		local sequence = CCSequence:create(arrayOfActions)
		local repeatAct =  CCRepeatForever:create(tolua.cast(sequence, 'CCActionInterval'))

		sprite:stopAllActions()
		sprite:runAction(repeatAct)
	end

	local scale = TXGUI.UIManager:sharedManager():getScaleFactor()

	-- 重新设置绿色经验条锚点
	local nextExpPic = FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyExpNext").__UIPicture__

	local nextExpSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_LU_EXPERIENCE1")
	nextExpPic:setSprite(nextExpSprite, false)

	nextExpSprite:setAnchorPoint(ccp(0, 0.5))
	local oldPosX, oldPosY = nextExpSprite:getPosition()	
	nextExpSprite:setPosition(ccp(oldPosX - scale * nextExpSprite:getContentSize().width/2, oldPosY));

	-- 重新设置蓝色经验条锚点
	local nowExpPic = FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyExpNow").__UIPicture__

	local nowExpSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_LU_EXPERIENCE2")
	nowExpPic:setSprite(nowExpSprite, false)

	nowExpSprite:setAnchorPoint(ccp(0, 0.5))
	oldPosX, oldPosY = nowExpSprite:getPosition()
	nowExpSprite:setPosition(ccp(oldPosX - scale * nowExpSprite:getContentSize().width/2, oldPosY));

	-- 保存用于调整比例
	FairyLevelUpLayer.nowExpSprite = nowExpSprite
	FairyLevelUpLayer.nextExpSprite = nextExpSprite

	blinkSprite(nextExpSprite)
end

-- 更新经验条比例 -1表示不更改
local function updateExpBar(nowRate, nextRate)
	local scale = TXGUI.UIManager:sharedManager():getScaleFactor()

	if nowRate >= 0 then
        if nowRate > 1 then
            nowRate = 1
        end
		FairyLevelUpLayer.nowExpSprite:setScaleX(scale * nowRate)
	end

	if nextRate >= 0 then
        if nextRate > 1 then
            nextRate = 1
        end
		FairyLevelUpLayer.nextExpSprite:setScaleX(scale * nextRate)
	end	
end

-- 初始化精灵的基本信息
local useFairyEatBtnPos = nil
local useMarrowEatBtnPos = nil

local function initFairyLevelUpBasicInfo()
	if FairyLevelUpLayer.fairyPos == 0 or FairyLevelUpLayer.uiLayout == nil then
		return
	end

	local userFairyInfo = getUserFairyByPos(FairyLevelUpLayer.fairyPos)
	local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)

	-- 当前等级
	local level = userFairyInfo.fairyLevel
	local levelStr = "Lv."..level

	FairyLevelUpLayer.uiLayout:FindChildObjectByName("masterLevelText").__UILabel__:setString(levelStr)
    FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyLevel").__UILabel__:setString(levelStr)

	-- 精灵头像
	local fairySprite = fairyBasicInfo:getFairyIcon()
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyMasterIcon").__UIPicture__:setSprite(fairySprite)

    -- 精灵名称
    local fairyName = fairyBasicInfo.name
	local fairyGrade = userFairyInfo.fairyGrade

	local nameStr = fairyName.." + "..fairyGrade

	FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyName").__UILabel__:setString(nameStr)
    FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyName").__UILabel__:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))

    -- 精灵动画
	local fairyId = userFairyInfo.fairyId

	local fairyFrame = FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyFrame").__UIPicture__
	local size = fairyFrame:getCurrentNode():getContentSize()		
	local px = size.width / 2
	local py = size.height / 2
	local modePt = CCPoint(px,py)								
	SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(fairyFrame:getCurrentNode(), fairyId, modePt)
	
    -- 初始化两个标签下吞噬按钮位置
    useFairyEatBtnPos = FairyLevelUpLayer.uiLayout:FindChildObjectByName("eatBtn"):getPosition()
    useMarrowEatBtnPos = FairyLevelUpLayer.uiLayout:FindChildObjectByName("midEatFrame"):getPosition()
end

local lastExpInfo = {} -- 仅供动态放缩特效使用
local nowExpInfo = {}
local nextExpInfo = {}

local function refreshExpInfo()
	local userFairyInfo = getUserFairyByPos(FairyLevelUpLayer.fairyPos)
	local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId) 
	if fairyBasicInfo ~= nil then
		-- 下一级和这一级的经验差
		local nowLevelExpDelta = getExpFairyLevelUpNeed(nowExpInfo.level + 1,fairyBasicInfo.quality) - getExpFairyLevelUpNeed(nowExpInfo.level,fairyBasicInfo.quality)
		local nextLevelExpDelta = getExpFairyLevelUpNeed(nextExpInfo.level + 1,fairyBasicInfo.quality) - getExpFairyLevelUpNeed(nextExpInfo.level,fairyBasicInfo.quality)

		local nowRate = nowExpInfo.exp / nowLevelExpDelta

		local nextRate = nowRate
		if nextExpInfo.level > nowExpInfo.level then
			nextRate = 1
		else
			nextRate = nextExpInfo.exp / nextLevelExpDelta
		end

		updateExpBar(nowRate, nextRate)
		lastExpInfo.exp = nowExpInfo.exp
		lastExpInfo.level = nowExpInfo.level
	end
end


local function onFairyEatSuccess()
	-- 更新当前等级
	local level = nextExpInfo.level
	local levelStr = "Lv."..level

	FairyLevelUpLayer.uiLayout:FindChildObjectByName("masterLevelText").__UILabel__:setString(levelStr)
    FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyLevel").__UILabel__:setString(levelStr)

    -- 播放粒子特效
    for i = 1, #FairyLevelUpLayer.selectFairysPos do
        local serIcon = FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairySerIcon"..i).__UIPicture__

        particle = ParticleManagerX:sharedManager():getParticles_uieffect("particle_effect_upgradesuccess")
        if nil ~= particle then
            particle:setPosition(ccp(serIcon:getCurrentNode():getContentSize().width * 0.5, serIcon:getCurrentNode():getContentSize().height * 0.5));
            serIcon:getCurrentNode():removeChildByTag(1001, true)
            serIcon:getCurrentNode():addChild(particle, 10, 1001)
        end
    end

	-- 清空已添加的精灵
	FairyLevelUpLayer.selectFairysPos = {}

    -- 延迟一段时间清空精灵
    local delayAction =	CCDelayTime:create(1.0)
	local callback = CCCallFunc:create(refreshSelectFairyPanel)
	local action = CCSequence:createWithTwoActions(delayAction,callback)

    local masterIcon = FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyMasterIcon").__UIPicture__
	masterIcon:getCurrentNode():runAction(action)

	--先隐藏下一级提示
	updateExpBar(-1, 0)

    -- 经验条动作结束后刷新消息
    local function onExpUpdateFinish()
        if FairyLevelUpLayer.selectTab == 2 then
            if fairyElementCount < nextNeedElementCount then
		        sendFairyLevelupReq({}, fairyElementCount, true)
                PushOneWaitingLayer("CSFairyAddExpReq")
	        else
		        sendFairyLevelupReq({}, nextNeedElementCount, true)
                PushOneWaitingLayer("CSFairyAddExpReq")
	        end
        end
        expGrowing = false
    end

	
	local scale = TXGUI.UIManager:sharedManager():getScaleFactor()

	local function scaleOnce(sprite, nowScale, nextScale)
		CCLuaLog("--- scaleOnce ---")
        expGrowing = true
		local arrayOfActions = CCArray:create()

		local scale1 = CCScaleTo:create((nextScale * scale - nowScale * scale)/scaleSpeed , nextScale * scale, 1)
        local scaleEnd = CCCallFunc:create(onExpUpdateFinish)

		arrayOfActions:addObject(scale1)
        arrayOfActions:addObject(scaleEnd)

		local sequence = CCSequence:create(arrayOfActions)

		sprite:stopAllActions()
		sprite:runAction(sequence)
	end

	local function scaleTwice(sprite, nowScale, nextScale)
		CCLuaLog("--- scaleTwice ---")
        expGrowing = true
		local arrayOfActions = CCArray:create()

		--local scale1 = CCScaleTo:create(-0.5/scaleSpeed , scale, 1)
		local scale1 = CCScaleTo:create((1 - nowScale) * scale /scaleSpeed , scale, 1)
		local scale2 = CCScaleTo:create(0 , 0, 1)
		local scale3 = CCScaleTo:create(nextScale * scale / scaleSpeed , nextScale * scale, 1)
        local scaleEnd = CCCallFunc:create(onExpUpdateFinish)

		arrayOfActions:addObject(scale1)
		arrayOfActions:addObject(scale2)
		arrayOfActions:addObject(scale3)
        arrayOfActions:addObject(scaleEnd)

		local sequence = CCSequence:create(arrayOfActions)

		sprite:stopAllActions()
		sprite:runAction(sequence)

	end

	local userFairyInfo = getUserFairyByPos(FairyLevelUpLayer.fairyPos)
	local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId) 

	local lastLevelExpDelta = getExpFairyLevelUpNeed(lastExpInfo.level + 1,fairyBasicInfo.quality) - getExpFairyLevelUpNeed(lastExpInfo.level,fairyBasicInfo.quality)
	local lastRate = lastExpInfo.exp / lastLevelExpDelta

	local nextLevelExpDelta = getExpFairyLevelUpNeed(nextExpInfo.level + 1,fairyBasicInfo.quality) - getExpFairyLevelUpNeed(nextExpInfo.level,fairyBasicInfo.quality)
	local nextRate = nextExpInfo.exp / nextLevelExpDelta

	local expSprite = FairyLevelUpLayer.nowExpSprite
	if lastExpInfo.level == nextExpInfo.level then
		scaleOnce(expSprite, lastRate, nextRate)
	else
		scaleTwice(expSprite, lastRate, nextRate)

		-- 精灵头像特效
		local fairyIcon = FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyMasterIcon").__UIPicture__
		local frameSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_LU_MASTER_FRAME")
		local frameSize = frameSprite:getContentSize()

		local particle = ParticleManager:Get():createEffectSpriteAndPlay(442,"", 100000, 1, false);
		particle:setPosition(ccp(frameSize.width/2, frameSize.height/2))
		fairyIcon:getCurrentNode():addChild(particle)
	end

	GameApi:showMessage(GetLuaLocalization("M_AD_TEXT3"))
	GameAudioManager:sharedManager():playEffect(320000,false)
end

-- 收到精灵升级回包
local function onMsgFairyLevelRsp(msg)
	CCLuaLog("--- onMsgFairyLevelRsp ---")

	if msg ~= nil then
		
		-- 消耗金币信息
		if msg.cost_coin ~= nil then
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("coinNum").__UILabel__:setString(""..msg.cost_coin)
		else
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("coinNum").__UILabel__:setString("0")
		end

		if msg.succ then
			CCLuaLog("--- onMsgFairyLevelRsp : SUCCESS ---")
		end
		
		
		if msg.fairy_info ~= nil then
			CCLuaLog("--- msg.fairy_info ~= nil ---")
			--属性信息
			local nextAttrs = {}
			for _ , attr in pairs(msg.fairy_info.fairy_attrs) do
				--CCLuaLog("nextAttrs")
				nextAttrs[attr.key] = attr.value			
			end

			-- 获得当前属性列表
			local userFairyInfo = getUserFairyByPos(FairyLevelUpLayer.fairyPos)

			RefreshAttrPanel(FairyLevelUpLayer.uiLayout, userFairyInfo.fairyAttr , nextAttrs)

			-- 如果等级未提升，提升后属性设为空
			local nowLevel = userFairyInfo.fairyLevel
			CCLuaLog("--- nowLevel : "..nowLevel)

			local nextLevel = msg.fairy_info.fairy_basic.fairy_lv

			nowExpInfo.level = nowLevel
			nextExpInfo.level = nextLevel
			CCLuaLog("--- nextLevel : "..nextLevel)
			if nowLevel == nextLevel then
				for i = 1, 6 do
					FairyLevelUpLayer.uiLayout:FindChildObjectByName("attrUpNum"..i).__UILabel__:setString("")
				end
			end

			-- 更新升级后等级
			local levelStr = "Lv."..nextLevel
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("masterLevelUpText").__UILabel__:setString(levelStr)

			-- 更新经验信息
			local nowExp = userFairyInfo.fairyExp
			local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId) 
			local nowBasicExp = getExpFairyLevelUpNeed(nowLevel,fairyBasicInfo.quality)

			local nextExp = msg.fairy_info.fairy_basic.fairy_exp
			local nextBasicExp = getExpFairyLevelUpNeed(nextLevel,fairyBasicInfo.quality)

			CCLuaLog("--- nowExp : "..nowExp)
			CCLuaLog("--- nextExp : "..nextExp)
            CCLuaLog("--- nowBasicExp : "..nextBasicExp)

			nowExpInfo.exp = nowExp
			nextExpInfo.exp = nextExp

			-- 获得经验数量
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("masterExpNum").__UILabel__:setString(""..(nextExp + nextBasicExp - nowExp - nowBasicExp))

			-- 更新所需精元信息
			FairyLevelUpLayer.uiLayout:FindChildObjectByName("costMarrowNum").__UILabel__:setString(""..userFairyInfo.fairyNextLvExp)
			nextNeedElementCount = userFairyInfo.fairyNextLvExp
            nextFiveNeedCount = nextNeedElementCount + getExpFairyLevelUpNeed(nowLevel + 5,fairyBasicInfo.quality) - getExpFairyLevelUpNeed(nowLevel + 1,fairyBasicInfo.quality)
			
			if msg.succ then
				onFairyEatSuccess()
			else
				-- 更新经验条
				refreshExpInfo()
			end
		end
	end
	
end

--关闭页面
local function onPressCloseBtn(tag)
	RemoveOneLayer(FairyLevelUpLayer.uiLayerInstance)
	FairyLevelUpLayer:destroyed()

	if FairyLevelUpLayer.lastLayer == 1 then
		showUILayerByTag(UITagTable["BackPackLayer"].tag,true)
	elseif FairyLevelUpLayer.lastLayer == 2 then
		showUILayerByTag(UITagTable["FairyBagLayer"].tag,true)
	end
end

--点击使用精灵
local function onPressUseFairyBtn(tag)
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyContainer").__UIContainer__:setVisible(true)
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("marrowContainer").__UIContainer__:setVisible(false)

	FairyLevelUpLayer.uiLayout:FindChildObjectByName("useFairyBtn").__UIButton__:getMenuItemSprite():selected()
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("useMarrowBtn").__UIButton__:getMenuItemSprite():unselected()

	FairyLevelUpLayer.selectTab = 1

    -- 调整吞噬按钮位置
    if  useFairyEatBtnPos ~= nil then
        FairyLevelUpLayer.uiLayout:FindChildObjectByName("eatText"):setPosition(useFairyEatBtnPos)
        FairyLevelUpLayer.uiLayout:FindChildObjectByName("eatBtn"):setPosition(useFairyEatBtnPos)
    end    

	sendFairyLevelupReq(FairyLevelUpLayer.selectFairysPos, 0, true)
    PushOneWaitingLayer("CSFairyAddExpReq")
end

--点击使用精元
local function onPressUseMarrowBtn(tag)
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("fairyContainer").__UIContainer__:setVisible(false)
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("marrowContainer").__UIContainer__:setVisible(true)

	FairyLevelUpLayer.uiLayout:FindChildObjectByName("useFairyBtn").__UIButton__:getMenuItemSprite():unselected()
	FairyLevelUpLayer.uiLayout:FindChildObjectByName("useMarrowBtn").__UIButton__:getMenuItemSprite():selected()

	FairyLevelUpLayer.selectTab = 2

    -- 调整吞噬按钮位置
    if  useMarrowEatBtnPos ~= nil then
        FairyLevelUpLayer.uiLayout:FindChildObjectByName("eatText"):setPosition(useMarrowEatBtnPos)
        FairyLevelUpLayer.uiLayout:FindChildObjectByName("eatBtn"):setPosition(useMarrowEatBtnPos)
    end

	if fairyElementCount < nextNeedElementCount then
		sendFairyLevelupReq({}, fairyElementCount, true)
        PushOneWaitingLayer("CSFairyAddExpReq")
	else
		sendFairyLevelupReq({}, nextNeedElementCount, true)
        PushOneWaitingLayer("CSFairyAddExpReq")
	end

end

--点击快速添加
local function onPressQuickAddBtn(tag)
	CCLuaLog("--- onPressQuickAddBtn ---")
	local fairyList = getUserFairyList()

	--local userFairyInfo = getUserFairyByPos(FairyLevelUpLayer.fairyPos)
	--local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)

	if fairyList ~= nil then
		local sortTable = {}
		for k,v in pairs(fairyList) do
			local fairyBasicInfo = getFairyBasicInfo(v.fairyId)
			local quality = fairyBasicInfo.quality

			-- 只有资质为1或2的才能吞噬
			if quality == 1 or quality == 2 then
	            -- 非出战助战小伙伴才能吞噬                
				if v.pos ~= FairyLevelUpLayer.fairyPos and isNormalFairy(v.pos) then
					table.insert(sortTable,v)
				end				
			end			
		end

        local function expSort(a, b)
            local quality1 = getFairyQualityById(a.fairyId)
		    local quality2 = getFairyQualityById(b.fairyId)

            local exp1 = getExpFairyApply(a.fairyLevel, quality1, a.fairyId)
            local exp2 = getExpFairyApply(b.fairyLevel, quality2, b.fairyId)

            return exp1 < exp2
        end

		table.sort(sortTable, expSort)

		FairyLevelUpLayer.selectFairysPos = {}
		CCLuaLog("--- #sortTable ---"..#sortTable)
		for i = #sortTable - FRIRY_SER_COUNT + 1, #sortTable do
			if sortTable[i] ~= nil then	
				CCLuaLog("--- sortTable[i].pos ---"..sortTable[i].pos)			
				table.insert(FairyLevelUpLayer.selectFairysPos, sortTable[i].pos)
			end
		end

        if #FairyLevelUpLayer.selectFairysPos == 0 then
            GameApi:showMessage(GetLuaLocalization("M_FAIRY_LU_MORE"))
        else
            refreshSelectFairyPanel()
		    if FairyLevelUpLayer.selectTab == 1 then
			    sendFairyLevelupReq(FairyLevelUpLayer.selectFairysPos, 0, true)
                PushOneWaitingLayer("CSFairyAddExpReq")
		    end	
        end
	end

	
end

-- 发送强化次数消息
local function sendLevelUpTimesReq(count)
    CSFairyAddExpReq = {
		fairy_pos = FairyLevelUpLayer.fairyPos,
		req_info = false,
        times = count,
		}
	local msgname="CSFairyAddExpReq"
	local ret = send_message(msgname, CSFairyAddExpReq, true)
	return ret
end

-- 点击强化或强化5次
local function onPressTimes(count)
    local needCount = nextNeedElementCount
    if count == 5 then 
        needCount = nextFiveNeedCount
    end
    if fairyElementCount < needCount then
        -- 精元不足提示
        local function onPressGetFairy()
            RemoveOneLayer(FairyLevelUpLayer.uiLayerInstance)
	        FairyLevelUpLayer:destroyed()
            MainMenuLayer:getMainMenuLayer():ShowMarketLayer(true, 1)
        end

        local function onPressDecFairy()
            RemoveOneLayer(FairyLevelUpLayer.uiLayerInstance)
	        FairyLevelUpLayer:destroyed()
            G_OpenFairyDecomposeLayer()
        end

        MessageBox:Show(GetLuaLocalization("M_FAIRY_LU_ERROR_2"),onPressGetFairy,onPressDecFairy,MB_OKCANCELCLOSE,ccc3(255,255,255),26)
        MessageBox:AdjustStyle(GetLuaLocalization("M_GET_FAIRY"), GetLuaLocalization("M_DECOMPOSE_FAIRY"))
	else
		sendLevelUpTimesReq(count)
        PushOneWaitingLayer("CSFairyAddExpReq")
	end
end

--点击吞噬按钮
local function onPressEatBtn(tag)
	CCLuaLog("--- onPressEatBtn ---")
    if expGrowing then
        return
    end

	if FairyLevelUpLayer.selectTab == 1 then
		if #FairyLevelUpLayer.selectFairysPos ~= 0 then
			sendFairyLevelupReq(FairyLevelUpLayer.selectFairysPos, 0, false)
            PushOneWaitingLayer("CSFairyAddExpReq")
        else
            GameApi:showMessage(GetLuaLocalization("M_FAIRYLU_NONE"))
		end 		
	else
        onPressTimes(1)
    end
end

-- 点击强化5次
local function onPressFiveEatBtn(tag)
    if expGrowing then
        return
    end

    onPressTimes(5) 
end

-- 点击某个精灵按钮，添加精灵
local function onPressFairyBtn(tag)
	CCLuaLog("--- onPressFairyBtn ---")
	FairySelectLayer:createLayer()
	FairySelectLayer:setSelectModle(true, false, FRIRY_SER_COUNT, FairyLevelUpLayer.fairyPos,3,true)
    FairySelectLayer:setDefaultSelectPos(FairyLevelUpLayer.selectFairysPos)
end

function FairyLevelUpLayer:InitLayer()
	CCLuaLog("--- FairyLevelUpLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册按钮回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
	self.uiLayout:FindChildObjectByName("useFairyBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressUseFairyBtn)
	self.uiLayout:FindChildObjectByName("useMarrowBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressUseMarrowBtn)
	self.uiLayout:FindChildObjectByName("quickAddBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressQuickAddBtn)
	self.uiLayout:FindChildObjectByName("eatBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressEatBtn)
    self.uiLayout:FindChildObjectByName("eatFiveBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressFiveEatBtn)

	-- 精灵按钮的回调
	for i = 1, FRIRY_SER_COUNT do
		self.uiLayout:FindChildObjectByName("fairyBtn"..i).__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressFairyBtn)
	end

	--总精元数量
	self.uiLayout:FindChildObjectByName("nowMarrowNum").__UILabel__:setString(""..fairyElementCount)




	--初始化经验条
	initExpBar()
	initFairyLevelUpBasicInfo()

	refreshSelectFairyPanel()

    --初始化显示使用精灵面板
	onPressUseFairyBtn(nil)
	

	--消息回调注册
	addMsgCallBack("CSFairyAddExpRsp", onMsgFairyLevelRsp)



	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false, -1)
end

function FairyLevelUpLayer:DebugExpSpeed(speed)
	scaleSpeed = speed / 50000
	GameApi:showMessage("Set Exp Speed "..speed)
end

----------------------------- 辅助全局函数 ----------------------------
function RefreshFairyElementCount(count)
	fairyElementCount = count

	if FairyLevelUpLayer.uiLayout ~= nil then
		FairyLevelUpLayer.uiLayout:FindChildObjectByName("nowMarrowNum").__UILabel__:setString(""..count)
	end
end