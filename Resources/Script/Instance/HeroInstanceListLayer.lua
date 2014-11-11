-- 勇者试炼副本列表页面
require("Script/GameConfig/InstanceList")
require("Script/GameConfig/ExchangeParameter")

HeroInstanceListLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
}

local nowCompleteInstanceId = 0
function SetNowCompleteInstanceId(id)
    nowCompleteInstanceId = id
end

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if HeroInstanceListLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(HeroInstanceListLayer.notificationFunc);
	end

	HeroInstanceListLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("HeroInstanceListUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= HeroInstanceListLayer.uiLayerInstance then
			if G_CurLayerInstance ~= HeroInstanceListLayer.uiLayerInstance then
				HeroInstanceListLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function HeroInstanceListLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["HeroInstanceListLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/HeroInstanceListUI.plist",self.uiLayerInstance, "HeroInstanceListUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function HeroInstanceListLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function HeroInstanceListLayer:destroyed()
	CCLuaLog("--- HeroInstanceListLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
    SetChatBar(true,-1)
end



local instanceDatas = nil
-- 初始化勇者试炼副本数据
local function initInstanceData()
    instanceDatas = {}

    local function sortFunc(a, b)
        return a.id < b.id
    end

    -- 初始化副本基本信息
    for k, v in pairs(instances) do
        if tonumber(v.Type) == 14 then
            local instanceData = {}
            instanceData.id = k
            instanceData.name = GetLuaLocalization(v.RaidName)
            instanceData.icon = v.Icon

            table.insert(instanceDatas, instanceData)
        end
    end

    table.sort(instanceDatas, sortFunc)

    -- 初始化副本花费信息（金币或者门票）
    for i = 1, #instanceDatas do
        local exchangeTable = ExchangeParameter[20059 + i]
        instanceDatas[i].costItemId , instanceDatas[i].costCount = string.match(exchangeTable.FromItems, "(%d+)/(%d+)")
    end

end



-- 点击副本按钮
local function onPressInstanceBtn(tag)
	CCLuaLog("--- onPressInstanceBtn ---"..tag)
    CCLuaLog("instanceId : "..instanceDatas[tag].id)
    SendHeroInstanceCreateReq(instanceDatas[tag].id)
    PushOneWaitingLayer("CSCancelWaitingRsp")

    --SetNowHeroInstanceId(instanceDatas[tag].id)
end

-- 建立副本列表
local function setupInstanceList()
    local page = HeroInstanceListLayer.uiLayout:FindChildObjectByName("instanceListScrollPage").__UIScrollPage__

    local _, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_LIST_AREA")

    local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()

    local function addSingleInstance(page, index, instanceData)

        -- pageIndex 从0开始
        local pageIndex = math.floor((index - 1) / 6)
        local pageCount = page:getPageCount()
        if pageIndex > pageCount - 1 then
           local layer = CCLayer:create()
           layer:setAnchorPoint(ccp(0, 0))
           page:addPage(layer, false)            
        end

        local nowLayer = page:getPageLayer(pageIndex)
        local nowLayerPos = ccp(nowLayer:getPosition())
        CCLuaLog("------------------------- nowLayerPos.x, nowLayerPos.y-------------"..nowLayerPos.x.." "..nowLayerPos.y)

        -- 创建副本节点
        local curInstanceNode = CCNode:create()
        nowLayer:addChild(curInstanceNode)

        -- 位置偏移
        local deltaX = 150 * ((index - 1) % 6)

        local function getRealPos(orgPos)
            local deltaPos = ccpMult(ccpSub(orgPos, bgPos), scale)
            local offsetPos = ccpMult(ccp(deltaX, 0), scale)
            local realPos = ccpAdd(deltaPos, offsetPos)
            return realPos
        end

        -- 副本名称
		local instanceNameLabel = TXGUI.UILabelTTF:create(instanceData.name, KJLinXin, 22)
		local _, instanceNameLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_TEXT_INSTANCENAME")
        local realPos = getRealPos(instanceNameLabelPos)
        CCLuaLog("------------------------- realPos.x, realPos.y-------------"..realPos.x.." "..realPos.y)

		instanceNameLabel:setPosition(getRealPos(instanceNameLabelPos))
        instanceNameLabel:setScale(scale)
        instanceNameLabel:setColor(ccc3(255,184,0))

        nowLayer:addChild(instanceNameLabel)

        -- 副本按钮
		local normalSprite, instanceBtnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_GRID1")
		local clickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_GRID1_CLICKED")
 
		local instanceBtn = IconButton:create(normalSprite, nil, clickedSprite, -130)
		instanceBtn:setPosition(getRealPos(instanceBtnPos))

		local btnMenuItem = instanceBtn:getMenuItem()
		btnMenuItem:setTag(index)

		btnMenuItem:registerScriptTapHandler(onPressInstanceBtn)

        instanceBtn:setScale(scale)
        nowLayer:addChild(instanceBtn)

        -- 副本图标
        local instanceIconResStr = "map_ui_system_icon_FRAME_"..string.upper(instanceData.icon)
        local instanceIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",instanceIconResStr)

        if nowCompleteInstanceId - instanceDatas[1].id + 1 < index and index <= 10 and index ~= 1 then
            instanceIcon = graylightWithCCSprite(instanceIcon, false)
        end

        --instanceBtn:AddExternalSprite(instanceIcon, ccp(0, 0))
        instanceBtn:addChild(instanceIcon)

        -- 花费文本 
        local spendTextLabel = TXGUI.UILabelTTF:create(GetLuaLocalization("M_HEROINSTANCE_SPEND"), KJLinXin, 22)
		local _, spendTextLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_TEXT_SPEND")
		spendTextLabel:setPosition(getRealPos(spendTextLabelPos))
        spendTextLabel:setColor(ccc3(255,195,118))

        spendTextLabel:setScale(scale)
        nowLayer:addChild(spendTextLabel)

        -- 花费物品icon
        local costItemIcon, costItemPos
        if tonumber(instanceData.costItemId) == 416001 then
            costItemIcon, costItemPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_ICON_COIN")
        else
            costItemIcon, costItemPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_ICON_TICKET")  
        end
        costItemIcon:setPosition(getRealPos(costItemPos))

        costItemIcon:setScale(scale)
        nowLayer:addChild(costItemIcon)

        -- 花费物品数量
        
        
        local spendCountLabel = nil
		
        if tonumber(instanceData.costItemId) == 416001 then
            local coinCountFrame, coinCountLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_NUM_SPEND1")
            spendCountLabel = TXGUI.UILabelTTF:create(" X "..MainMenuLayer:GetNumByFormat(instanceData.costCount), KJLinXin, 22, 
                                                    coinCountFrame:getContentSize(), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter)
            spendCountLabel:setPosition(getRealPos(coinCountLabelPos))
        else
            local ticketCountFrame, ticketCountLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_NUM_SPEND2")
            spendCountLabel = TXGUI.UILabelTTF:create(" X "..instanceData.costCount, KJLinXin, 22, 
                                    ticketCountFrame:getContentSize(), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter)
            spendCountLabel:setPosition(getRealPos(ticketCountLabelPos))
        end

        spendCountLabel:setScale(scale)
        nowLayer:addChild(spendCountLabel)

        -- 门票文本
        if tonumber(instanceData.costItemId) ~= 416001 then
            local ticketTextLabel = TXGUI.UILabelTTF:create(GetLuaLocalization("M_HEROINSTANCE_TICKET"..index), KJLinXin, 22)
		    local _, ticketTextLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_HEROINSTANCE_TEXT_TICKET")
		    ticketTextLabel:setPosition(getRealPos(ticketTextLabelPos))
            ticketTextLabel:setColor(ccc3(255,195,118))

            ticketTextLabel:setScale(scale)
            nowLayer:addChild(ticketTextLabel)
        end

    end

    for i = 1, #instanceDatas do
        addSingleInstance(page, i, instanceDatas[i])
    end    

    page:moveToPage(0)
end

-- 打开商店
local function onPressShopBtn(tag)
	CCLuaLog("--- onPressShopBtn ---")
    MainMenuLayer:getMainMenuLayer():ShowMarketLayer(true, 2)
    RemoveOneLayer(HeroInstanceListLayer.uiLayerInstance)
	HeroInstanceListLayer:destroyed()
end

--关闭页面
local function onPressCloseBtn(tag)
    CCLuaLog("--- onPressCloseBtn ---")
    RemoveOneLayer(HeroInstanceListLayer.uiLayerInstance)
	HeroInstanceListLayer:destroyed()
end

function HeroInstanceListLayer:InitLayer()
	CCLuaLog("--- HeroInstanceListLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
    self.uiLayout:FindChildObjectByName("shopBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressShopBtn)

    if instanceDatas == nil then
        initInstanceData()
    end
    
    setupInstanceList()

    PushOneLayer(self.uiLayerInstance,"","")
    SetChatBar(false,-1)
end

------------------------------------------------ 辅助函数 -----------------------------
local nowInstanceId = 0
function SetNowHeroInstanceId(instanceId)
    nowInstanceId = instanceId
end

function GetNowHeroInstanceId()
    return nowInstanceId
end

function GetNextHeroInstanceId()
    CCLuaLog("--- GetNextHeroInstanceId ---")
    if instanceDatas == nil then
        initInstanceData()
    end

    for k, instanceData in pairs(instanceDatas) do
        if instanceData.id == nowInstanceId then
            if instanceDatas[k+1] ~= nil then
                return instanceDatas[k+1].id
            else
                CCLuaLog("--- next == nil ---")
                return 0
            end
        end
    end
end

function GetNextHeroInstanceCostData()
    if instanceDatas == nil then
        initInstanceData()
    end

    for k, instanceData in pairs(instanceDatas) do
        if instanceData.id == nowInstanceId then
            if instanceDatas[k+1] ~= nil then
                return tonumber(instanceDatas[k+1].costItemId) , tonumber(instanceDatas[k+1].costCount)
            else
                CCLuaLog("--- next == nil ---")
                return nil
            end
        end
    end
end