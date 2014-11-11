-- 声望商店页面
require("Script/GameConfig/ExchangeItem")
require("Script/GameConfig/Item")
require("Script/Fairy/FairyConfigTable")
require("Script/GameConfig/EquipmentItem")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/Fairy/FairyLuaInterface")

ReputationShopLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RemoveOneLayer(ReputationShopLayer.uiLayerInstance)	
	if ReputationShopLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(ReputationShopLayer.notificationFunc);
	end

	ReputationShopLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("ReputationShopUI")
end

local nowRepu = 0
function ReputationShopLayer:RefreshReputation()
	if self.uiLayout ~= nil then 
		local info = UserData:GetUserInfo()
		nowRepu = info.m_reputation
		--local str = MainMenuLayer:GetNumByFormat(nowRepu)

		self.uiLayout:FindChildObjectByName("repuNum").__UILabel__:setString(""..nowRepu)
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= ReputationShopLayer.uiLayerInstance then
			if G_CurLayerInstance ~= ReputationShopLayer.uiLayerInstance then
				ReputationShopLayer:destroyed()
				ret = 1
			end
		end
	elseif message == GM_ATTR_REPUTATION_UPDATE then
		ReputationShopLayer:RefreshReputation()
	end

	return ret
end

function ReputationShopLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ReputationShopLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ReputationShopUI.plist",self.uiLayerInstance, "ReputationShopUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function ReputationShopLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function ReputationShopLayer:destroyed()
	CCLuaLog("--- ReputationShopLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
end

--关闭页面
local function onPressCloseBtn(tag)
	ReputationShopLayer:destroyed()
	showUILayerByTag(UITagTable["PvAILayer"].tag,true)
end

-- 初始化声望兑换物品
local function initRepuShopList()
	local repuShopList = {}
	local itemButtons = {}
	for k,v in pairs(ExchangeItems) do
		if tonumber(v.Type) == 15 then
			CCLuaLog("find repu item...")
			local repuCost = string.match(v.FromItems, "/(%d+)")
			local itemId, itemCount = string.match(v.ToItems, "(%d+)/(%d+)")
			local listItem = {
				exchangeId = k,
				cost = tonumber(repuCost),
				id = tonumber(itemId),
				count = tonumber(itemCount),
			}
			table.insert(repuShopList, listItem)
		end
	end

    local function sortFunc(a, b)
        return a.exchangeId < b.exchangeId
    end
    table.sort(repuShopList, sortFunc)

	local tipFairyId = 0
	local function onOperationTipCallBackLeft(tag)

	end

	local function onClickEquipMiddleCallBack(tag)
		if tipFairyId ~= 0 then
			G_ShowFairyGetwayLayer(tipFairyId)
		end
	end

	local function onOperationTipCallBackRight(tag)
		if tipFairyId ~= 0 then
			FairyInfoLayer.fairyId = tipFairyId
			FairyInfoLayer.buttonState = 0
			FairyInfoLayer.infoState = 2
			FairyInfoLayer:CreateLayer(nil)
		end
	end

	local function onPressItemBtn(tag)
		CCLuaLog("onPressItemBtn .. "..tag)
		local itemId = repuShopList[tag].id

		local menu = itemButtons[tag]:getMenuNode()
		local pos = menu:getTouchPoint()

		ItemManager:Get():showItemTipsById(itemId, pos)

		local isFairy = G_GetIsFairyById(itemId)
		local info = ItemManager:Get():getItemInfoById(itemId)
		if isFairy or info.m_type == 21 then
			tipFairyId = itemId % 10000;
			local detailText = LanguageLocalization:GetLocalization("M_EQUIPMENT_INFORMATION")
			ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onOperationTipCallBackRight,detailText,true)
		end

		--if EquipmentItems[itemId] ~= nil or (Items[itemId] ~= nil and Items[itemId]["Type"] == "19") then
		--	tipFairyId = itemId
		--	ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onClickEquipMiddleCallBack,LanguageLocalization:GetLocalization("M_FAIRY_GET_WAY"),true)
		--end
	end

	local function sendExchangeReq(exchangeId)
		CSEquipmentComposeReq = {
			exchange_item_id = exchangeId
			}
		local msgname="CSEquipmentComposeReq"
		local ret = send_message(msgname, CSEquipmentComposeReq, true)
		return ret
	end

    -- 保存每个次数以及label
    local itemLabels = {}
    local itemExchangeNums = {}
    local itemExchangeTypes = {}

	local function onPressExchangeBtn(tag)
		CCLuaLog("onPressExchangeBtn .. "..tag)
		if nowRepu < repuShopList[tag].cost then
            GameApi:showMessage(GetLuaLocalization("M_REPUTATION_SHORTAGE"))
            return
        end

        -- 判断购买次数
        if itemExchangeTypes[tag] ~= 0 then
            if itemExchangeNums[tag] == 0 then
                if itemExchangeTypes[tag] == 1 then
                    GameApi:showMessage(GetLuaLocalization("M_PVAI_EXCHANGE_TIME1"))
                elseif itemExchangeTypes[tag] == 2 then
                    GameApi:showMessage(GetLuaLocalization("M_PVAI_EXCHANGE_TIME2"))
                end            
		        return
            else
                itemExchangeNums[tag] = itemExchangeNums[tag] - 1

                local timeTextStr = ""
                local todayFormatStr = GetLuaLocalization("M_SHOP_TODAYBUY")
                local totalFormatStr = GetLuaLocalization("M_PVAI_EXCHANGE")

                if itemExchangeTypes[tag] == 1 then
                    timeTextStr = string.format(todayFormatStr, itemExchangeNums[tag])
                elseif itemExchangeTypes[tag] == 2 then
                    timeTextStr = string.format(totalFormatStr, itemExchangeNums[tag])
                end

                itemLabels[tag]:setString(timeTextStr)
            end
        end

        
        sendExchangeReq(repuShopList[tag].exchangeId)	
	end

	-- 一次添加两条物品
	local function addTwinItem(contentList, item1, item2, index)
		CCLuaLog("addTwinItem .. ")
		local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin", "map_ui_FRAME_PVAI_EXCHANGE_FRAME")

		local bgListItemSize = itemBg:getContentSize()		
		local ListItemNode = CCNode:create()

		local bgControl = contentList:AddBaseItem(bgListItemSize, ListItemNode)

		for i = 1, 2 do
			local itemData = (i == 1) and item1 or item2
			local itemIndex = (i == 1) and index or (index + 1)

			if itemData ~= nil then
				-- 按钮背景
				local btnBg, btnBgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_FRAME"..i)
				btnBg:setPosition(ccpSub(btnBgPos, bgPos))
				bgControl:getCurrentNode():addChild(btnBg, 10)

				-- 物品icon
				local itemNormalIcon = nil
				local itemClickedIcon = nil
				local itemNameStr = ""
				if Items[itemData.id] ~= nil then
					itemNormalIcon = ItemManager:Get():getIconSpriteById(itemData.id)
					itemClickedIcon = ItemManager:Get():getIconSpriteById(itemData.id)

					itemNameStr = (Items[itemData.id] ~= nil ) and GetLuaLocalization(Items[itemData.id]["Name"]) or ""
                elseif EquipmentItems[itemData.id] ~= nil then
                    itemNormalIcon = ItemManager:Get():getIconSpriteById(itemData.id)
					itemClickedIcon = ItemManager:Get():getIconSpriteById(itemData.id)

					itemNameStr = (EquipmentItems[itemData.id] ~= nil ) and GetLuaLocalization(EquipmentItems[itemData.id]["Name"]) or ""
				else
					local fairyBasicInfo = getFairyBasicInfo(itemData.id)
					itemNormalIcon = fairyBasicInfo:getFairyIcon()
					itemClickedIcon = fairyBasicInfo:getFairyIcon()	
					
					itemNameStr = fairyBasicInfo.name	
				end

                if itemNormalIcon == nil then
                    break
                end
				
                local iconBtnFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_FRAME1_ICON")
                local outSize = CCSize(iconBtnFrame:getContentSize().width * 1.2, iconBtnFrame:getContentSize().height * 1.2)
				local _, itemIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_FRAME"..i.."_ICON")

				local itemBtn = IconButton:create(itemNormalIcon, nil, itemClickedIcon, -135)
				itemBtn:setContentSize(outSize)

				itemBtn:setPosition(ccpSub(itemIconPos, bgPos))
				
				--物品数量
				local fontSize = GameFontManager:smallFontSize()
				itemBtn:setItemLabel(""..itemData.count, fontSize, KJLinXin)

				local itemMenu = itemBtn:getMenuItem()
				itemMenu:setTag(itemIndex)

				itemMenu:registerScriptTapHandler(onPressItemBtn)

				itemButtons[itemIndex] = itemBtn

				bgControl:getCurrentNode():addChild(itemBtn, 10)

				-- 物品名称
				local nameFrame, nameLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_FRAME"..i.."_NAME")
				local nameLabel = TXGUI.UILabelTTF:create(itemNameStr, KJLinXin, 22, nameFrame:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
				nameLabel:setPosition(ccpSub(nameLabelPos, bgPos))
				nameLabel:setColor(ccc3(255,184,0))

				bgControl:getCurrentNode():addChild(nameLabel, 10)

				-- 声望图标
				local repuIcon, repuIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_FRAME"..i.."_REPU")
				repuIcon:setPosition(ccpSub(repuIconPos, bgPos))
				bgControl:getCurrentNode():addChild(repuIcon, 10)

				-- 声望数量
				local repuNumFrame, repuNumPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_FRAME"..i.."_PRICE")
				local repuNumLabel = TXGUI.UILabelTTF:create(""..itemData.cost, KJLinXin, 22, repuNumFrame:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
				repuNumLabel:setPosition(ccpSub(repuNumPos, bgPos))

				bgControl:getCurrentNode():addChild(repuNumLabel, 10)

				-- 兑换按钮
				local normalSprite, exchangeBtnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_BUTTON_EXCHANGE"..i)
				local clickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_BUTTON_EXCHANGE"..i.."_CLICKED")
 
				exchangeBtn = IconButton:create(normalSprite, nil, clickedSprite, -135)
				exchangeBtn:setPosition(ccpSub(exchangeBtnPos, bgPos))

				local btnMenuItem = exchangeBtn:getMenuItem()
				btnMenuItem:setTag(itemIndex)

				btnMenuItem:registerScriptTapHandler(onPressExchangeBtn)
	
				bgControl:getCurrentNode():addChild(exchangeBtn, 10)

				-- 兑换文本
				local exchangeLabel = TXGUI.UILabelTTF:create(""..GetLuaLocalization("M_EXCHANGE_BUTTON"), KJLinXin, 24)
				local _, exchangeLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_TEXT_EXCHANGE"..i)
				exchangeLabel:setPosition(ccpSub(exchangeLabelPos, bgPos))

				bgControl:getCurrentNode():addChild(exchangeLabel, 11)	

                --兑换次数
                local timeTextStr = ""
                local todayFormatStr = GetLuaLocalization("M_SHOP_TODAYBUY")
                local totalFormatStr = GetLuaLocalization("M_PVAI_EXCHANGE")

                local exchangeRemainTime, exchangeType = GetItemExchangeRemainTime(itemData.exchangeId)
                if exchangeType == 1 then
                    timeTextStr = string.format(todayFormatStr, exchangeRemainTime)
                elseif exchangeType == 2 then
                    timeTextStr = string.format(totalFormatStr, exchangeRemainTime)
                end

	            local exchangeTimeLabel = TXGUI.UILabelTTF:create(timeTextStr, KJLinXin, 14)
	            local _, exchangeTimeLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVAI_EXCHANGE_TEXT_TIME"..i)
	            exchangeTimeLabel:setPosition(ccpSub(exchangeTimeLabelPos, bgPos))
	            exchangeTimeLabel:setColor(ccc3(255,184,0))

                itemLabels[itemIndex] = exchangeTimeLabel
                itemExchangeNums[itemIndex] = exchangeRemainTime
                itemExchangeTypes[itemIndex] = exchangeType

	            bgControl:getCurrentNode():addChild(exchangeTimeLabel, 11)	
			end

		end
	end

	local contentList = ReputationShopLayer.uiLayout:FindChildObjectByName("contentScrollList").__UIScrollList__
	for i = 1, #repuShopList, 2 do
		local item1 = repuShopList[i]
		local item2 = repuShopList[i+1]
		addTwinItem(contentList, item1, item2, i)
	end
end

function ReputationShopLayer:InitLayer()
	CCLuaLog("--- ReputationShopLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)

	initRepuShopList()

	self:RefreshReputation()
	PushOneLayer(self.uiLayerInstance, "", "")
end