require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/Item")
require("Script/GameConfig/EquipmentItem")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/Fairy/FairyLuaInterface")

BestFighterPointShopLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	
}

ItemListInfo = {}
ItemButtons = {}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if BestFighterPointShopLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(BestFighterPointShopLayer.notificationFunc);
	end

	BestFighterPointShopLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("BestFighterPointShopUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= BestFighterPointShopLayer.uiLayerInstance then
			if G_CurLayerInstance ~= BestFighterPointShopLayer.uiLayerInstance then
				BestFighterPointShopLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function BestFighterPointShopLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["BestFighterPointShopLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/BestFighterPointShopUI.plist",self.uiLayerInstance, "BestFighterPointShopUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function BestFighterPointShopLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function BestFighterPointShopLayer:destroyed()
	CCLuaLog("--- BestFighterPointShopLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
	SetChatBar(true,-1)
end

local function sendExchangeReq(exchangeId)
	CSEquipmentComposeReq = {
		exchange_item_id = exchangeId
		}
	local msgname="CSEquipmentComposeReq"
	local ret = send_message(msgname, CSEquipmentComposeReq, true)
	return ret;
end

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
	local itemId = ItemListInfo[tag].itemId

	local menu = ItemButtons[tag]:getMenuNode()
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

-- 保存每个次数以及label
local itemLabels = {}
local itemExchangeNums = {}
local itemExchangeTypes = {}

local function onPressExchangeBtn(tag)
	CCLuaLog("onPressExchangeBtn .. "..tag)
	if ItemListInfo[tag].price > GetBestFighterPoint() then
		GameApi:showMessage(GetLuaLocalization("M_NOT_ENOUGH_POINTS"))
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

	sendExchangeReq(ItemListInfo[tag].exchangeId)
end

local function addSingleItem(itemList, itemData, index)
	local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_FRAME_ITEM1")
	local bgListItemSize = itemBg:getContentSize()		
	local ListItemNode = CCNode:create()

	local bgControl = itemList:AddBaseItem(bgListItemSize, ListItemNode)

	--背景
	bgControl:getCurrentNode():addChild(itemBg, 10)

	--物品名称	
	local itemNameStr = GetLuaLocalization(Items[itemData.itemId]["Name"])

	local nameFrame, nameLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_TEXT_ITEMNAME1")
	local nameLabel = TXGUI.UILabelTTF:create(itemNameStr, KJLinXin, 22, nameFrame:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
	nameLabel:setPosition(ccpSub(nameLabelPos, bgPos))
	nameLabel:setColor(ccc3(255,255,255))

	bgControl:getCurrentNode():addChild(nameLabel, 10)

	--积分图标
	local pointIcon, pointIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_ICON_POINT")
	pointIcon:setPosition(ccpSub(pointIconPos, bgPos))
	bgControl:getCurrentNode():addChild(pointIcon, 10)

	--物品价格
	
	local priceFrame, priceLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_TEXT_ITEMCOST1")
	local priceLabel = TXGUI.UILabelTTF:create(""..itemData.price, KJLinXin, 22, priceFrame:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
	priceLabel:setPosition(ccpSub(priceLabelPos, bgPos))
	priceLabel:setColor(ccc3(255,162,0))

	bgControl:getCurrentNode():addChild(priceLabel, 10)

	--物品图标
	local itemNormalIcon = ItemManager:Get():getIconSpriteById(itemData.itemId)
	local itemClickedIcon = ItemManager:Get():getIconSpriteById(itemData.itemId)
	local _, itemIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_ICON_ITEM1")

	local itemBtn = IconButton:new(itemNormalIcon, nil, itemClickedIcon)
	itemBtn:setContentSize(itemNormalIcon:getContentSize())

	itemBtn:setPosition(ccpSub(itemIconPos, bgPos))

	--右下角数字
	local fontSize = GameFontManager:smallFontSize()
	itemBtn:setItemLabel(""..itemData.count, fontSize, KJLinXin)

	local itemMenu = itemBtn:getMenuItem()
	itemMenu:setTag(index)

	itemMenu:registerScriptTapHandler(onPressItemBtn)

	ItemButtons[index] = itemBtn

	bgControl:getCurrentNode():addChild(itemBtn, 10)

	-- 物品等级颜色框
	--local itemInfo = ItemManager:Get():getItemInfoById(itemData.itemId);
	--if nil ~= itemInfo then
	--	if itemInfo.m_quality > 0 then
	--		local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality);
	--		if nil ~= framePic then
	--			framePic:setPosition(ccpSub(itemIconPos, bgPos))
	--			bgControl:getCurrentNode():addChild(framePic, 11)
	--		end
	--	end
	--end

	--兑换按钮
	local normalSprite, exchangeBtnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_BUTTON_EXCHANGE")
    local clickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_BUTTON_EXCHANGE_CLICKED")
 
	exchangeBtn = IconButton:new(normalSprite, nil, clickedSprite)
	exchangeBtn:setPosition(ccpSub(exchangeBtnPos, bgPos))

	local btnMenuItem = exchangeBtn:getMenuItem()
	btnMenuItem:setTag(index)

	btnMenuItem:registerScriptTapHandler(onPressExchangeBtn)
	
	bgControl:getCurrentNode():addChild(exchangeBtn, 10)

	--兑换文本
	local exchangeLabel = TXGUI.UILabelTTF:create(""..GetLuaLocalization("M_EXCHANGE_BUTTON"), KJLinXin, 24)
	local _, exchangeLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_TEXT_EXCHANGE")
	exchangeLabel:setPosition(ccpSub(exchangeLabelPos, bgPos))
	exchangeLabel:setColor(ccc3(255,255,255))

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
	local _, exchangeTimeLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_PVPTOP_SHOP_TEXT_EXCHANGE_TIME")
	exchangeTimeLabel:setPosition(ccpSub(exchangeTimeLabelPos, bgPos))
	exchangeTimeLabel:setColor(ccc3(255,184,0))

    itemLabels[index] = exchangeTimeLabel
    itemExchangeNums[index] = exchangeRemainTime
    itemExchangeTypes[index] = exchangeType

	bgControl:getCurrentNode():addChild(exchangeTimeLabel, 11)	
end

local function refreshItemList()
	if BestFighterPointShopLayer.uiLayout == nil then
		return
	end

	local itemList = BestFighterPointShopLayer.uiLayout:FindChildObjectByName("itemScrollList").__UIScrollList__
	if nil == itemList then
		return
	end

	itemList:clearChildrens()
	
	for i = 1, #ItemListInfo do
		addSingleItem(itemList, ItemListInfo[i], i)
	end	

end

local function onPressCloseBtn(tag)
	RemoveOneLayer(BestFighterPointShopLayer.uiLayerInstance)
	BestFighterPointShopLayer:destroyed()
end

function BestFighterPointShopLayer:InitLayer()
	CCLuaLog("--- BestFighterPointShopLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	local closeBtnMenu = self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite()
	closeBtnMenu:registerScriptTapHandler(onPressCloseBtn)

	--更新积分
	self:RefreshPoint()	

	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

	refreshItemList()
end

function BestFighterPointShopLayer:RefreshPoint()
	if self.uiLayout ~= nil then
		self.uiLayout:FindChildObjectByName("nowPointNum").__UILabel__:setString(""..GetBestFighterPoint())
	end	
end

