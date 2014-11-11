-- 精灵兑换页面
require("Script/GameConfig/ExchangeItem")
require("Script/FairyLayer/FairyCollectionLayer")
require("Script/OsCommonMethod")
require("Script/GameConfig/Item")
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/FairyLayer/FairyExchangeContactLayer")
require("Script/Fairy/FairyGetwayLayer")

FairyExchangeLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

	pageIndex = 1,
	exchangeData = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if FairyExchangeLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyExchangeLayer.notificationFunc);
	end

	FairyExchangeLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyExchangeUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyExchangeLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyExchangeLayer.uiLayerInstance then
				FairyExchangeLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function FairyExchangeLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyExchangeLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyExchangeUI.plist",self.uiLayerInstance, "FairyExchangeUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyExchangeLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.pageIndex = 1
	self.exchangeData = nil
end

-- 删除UI
function FairyExchangeLayer:destroyed()
	CCLuaLog("--- FairyExchangeLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
	SetChatBar(true,-1)
end

local desFairyList = {}

-- 初始化兑换数据
local function initExchangeData()
	if FairyExchangeLayer.exchangeData ~= nil then
		return
	end

	local startIndex = 2800
	FairyExchangeLayer.exchangeData = {}
	desFairyList = {}

	for i = startIndex, startIndex + 10 do
		local itemData = ExchangeItems[i]
		if itemData == nil then
			break
		end

		local fromList = Split(itemData.FromItems, ";")
		local needItemList = {}
		for j = 1, #fromList do			
			local id, num = string.match(fromList[j], "(%d+)/(%d+)")
			table.insert(needItemList, {tonumber(id), tonumber(num)})
		end
		local desId, desNum = string.match(itemData.ToItems, "(%d+)/(%d+)")
		local requireLevel = itemData.Level

		local singleTable = {}
		singleTable.exchangeId = i
		singleTable.needItemList = needItemList
		singleTable.desId = tonumber(desId)
		singleTable.desNum = tonumber(desNum)
		singleTable.level = tonumber(requireLevel)

		table.insert(FairyExchangeLayer.exchangeData, singleTable)
		table.insert(desFairyList, tonumber(desId))
	end
end

-- 判断是否可以兑换精灵，
function GetFairyExchangeIndex()
	initExchangeData()

	local exchangeIndex = -1

	for i = 1, #FairyExchangeLayer.exchangeData do
		local nowData = FairyExchangeLayer.exchangeData[i]

		local level = nowData.level
		local userLevel = UserData:GetUserLevel()		

		if level <= userLevel then
			local resList = nowData.needItemList

			local itemEnoughFlag = true

			for j = 1, #resList do
				local itemId, itemNum = resList[j][1], resList[j][2]

				local nowNum = 0

				if Items[itemId] ~= nil then
					nowNum = ItemManager:Get():getItemNumberById(itemId)
				else
					nowNum = GetExchangeFairyCountById(itemId)
				end

				if nowNum < itemNum then
					itemEnoughFlag = false
					break
				end

			end

			if itemEnoughFlag then
				exchangeIndex = i
				break
			end	
		end

	end

	return exchangeIndex
end



local fairyBtn = nil
local itemBtns = {}
local exchangeState = true

-- 刷新兑换页面
local function refreshExchangeLayer()
	local nowIndex = FairyExchangeLayer.pageIndex
	local nowData = FairyExchangeLayer.exchangeData[nowIndex]
	if nowData == nil then
		return
	end

	-- 更新左右按钮状态
	FairyExchangeLayer.uiLayout:FindChildObjectByName("lastBtn").__UIButton__:setVisible(nowIndex ~= 1)
	FairyExchangeLayer.uiLayout:FindChildObjectByName("nextBtn").__UIButton__:setVisible(nowIndex ~= #FairyExchangeLayer.exchangeData)

	local fairyId = nowData.desId
	-- 如果是碎片则转为精灵
	if fairyId > 10000 then
		fairyId = fairyId % 10000
	end

	fairyBtn = FairyExchangeLayer.uiLayout:FindChildObjectByName("fairyBtn").__UIButton__

	-- 更新左侧大图
	local leftPic = FairyExchangeLayer.uiLayout:FindChildObjectByName("leftFairyFrame").__UIPicture__
	local fairyPic = CCSprite:create("UI/ui_fairy_body_picture_"..fairyId..".png")
	if fairyPic ~= nil then
		leftPic:setSprite(fairyPic)
	end
	
	-- 更新精灵scroll page index
	local page = FairyExchangeLayer.uiLayout:FindChildObjectByName("fairyScrollPage").__UIScrollPage__
	local curPageIndex = page:getCurPage()
	if curPageIndex ~= nowIndex - 1 then
		page:moveToPage(nowIndex - 1, true)
	end


	-- 更新右侧精灵
	--[[
	local fairyFrame = FairyExchangeLayer.uiLayout:FindChildObjectByName("rightFairyFrame").__UIPicture__
	local size = fairyFrame:getCurrentNode():getContentSize()		
	local px = size.width / 2
	local py = size.height / 2
	local modePt = CCPoint(px,py)			
	SpriteFactory:sharedFactory():RemoveElfFromParent(fairyFrame:getCurrentNode())					
	SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(fairyFrame:getCurrentNode(), fairyId, modePt)
	--]]

	-- 更新所需等级
	local levelEnoughFlag = true

	local level = nowData.level
	local userLevel = UserData:GetUserLevel()
	local levelStr = GetLuaLocalization("M_EXCHANGE_LEVEL")..level
	local levelLabel = FairyExchangeLayer.uiLayout:FindChildObjectByName("fairyLevel").__UILabel__

	levelLabel:setString(levelStr)

	if userLevel < level then
		levelEnoughFlag = false
		levelLabel:setColor(ccc3(250, 30, 25))
	else
		levelLabel:setColor(ccc3(56, 221, 7))
	end
	

	local resList = nowData.needItemList

	-- 更新所需物品图标及数量
	local itemEnoughFlag = true
	exchangeState = true

	for i = 1, 4 do
		itemBtns[i] = FairyExchangeLayer.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__

		if resList[i] ~= nil then
			local itemId = resList[i][1]
			local itemNum = resList[i][2]

			local nowNum = 0
			local itemSprite = nil
			if Items[itemId] ~= nil then
				nowNum = ItemManager:Get():getItemNumberById(itemId)
				itemSprite = ItemManager:Get():getIconSpriteById(itemId)
			else
				nowNum = GetExchangeFairyCountById(itemId)
				CCLuaLog("fairy id : "..itemId)
				CCLuaLog("now Num :"..nowNum)
				

				local fairyBasicInfo = getFairyBasicInfo(tonumber(itemId))
				itemSprite = fairyBasicInfo:getFairyIcon()

			end			

			FairyExchangeLayer.uiLayout:FindChildObjectByName("itemIcon"..i).__UIPicture__:setSprite(itemSprite, false)

			local itemLabel = FairyExchangeLayer.uiLayout:FindChildObjectByName("itemNum"..i).__UILabel__
			itemLabel:setString(""..nowNum.."/"..itemNum)

			if nowNum < tonumber(itemNum) then
				itemLabel:setColor(ccc3(250, 30, 25))
				itemEnoughFlag = false
				exchangeState = false
			else
				itemLabel:setColor(ccc3(56, 221, 7))
			end

			FairyExchangeLayer.uiLayout:FindChildObjectByName("itemIcon"..i).__UIPicture__:setVisible(true)
			itemBtns[i]:setVisible(true)

		else
			FairyExchangeLayer.uiLayout:FindChildObjectByName("itemIcon"..i).__UIPicture__:setVisible(false)
			itemBtns[i]:setVisible(false)
			FairyExchangeLayer.uiLayout:FindChildObjectByName("itemNum"..i).__UILabel__:setString("")
		end
	end

	-- 更新兑换按钮状态
	if itemEnoughFlag and levelEnoughFlag then
		FairyExchangeLayer.uiLayout:FindChildObjectByName("exchangeDisablePic").__UIPicture__:setVisible(false)
		FairyExchangeLayer.uiLayout:FindChildObjectByName("exchangeBtn").__UIButton__:setVisible(true)
	else
		FairyExchangeLayer.uiLayout:FindChildObjectByName("exchangeDisablePic").__UIPicture__:setVisible(true)
		FairyExchangeLayer.uiLayout:FindChildObjectByName("exchangeBtn").__UIButton__:setVisible(false)
	end
end

local function onScrollPageChange()
	local page = FairyExchangeLayer.uiLayout:FindChildObjectByName("fairyScrollPage").__UIScrollPage__
	local curPageIndex = page:getCurPage()

	FairyExchangeLayer.pageIndex = curPageIndex + 1
	refreshExchangeLayer()
end

-- 初始化精灵动画列表
local function initFairyScrollPage()

	local page = FairyExchangeLayer.uiLayout:FindChildObjectByName("fairyScrollPage").__UIScrollPage__

	--local _, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_EXCHANGE_SIX_FAIRY_SIX")

	local function addSingleFairy(page, i, fairyId)

		local layer = CCLayer:create()

		SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(layer, fairyId, ccp(0, 0))

		page:addPage(layer, true)	

	end

	for i = 1, #desFairyList do
        addSingleFairy(page, i, desFairyList[i] % 10000)
    end    

	page:registerPageChangedCallbackFunc(onScrollPageChange)
    page:moveToPage(0)

end

--关闭页面
local function onPressCloseBtn(tag)
	RemoveOneLayer(FairyExchangeLayer.uiLayerInstance)
	FairyExchangeLayer:destroyed()
end

-- 点击精灵图鉴按钮
local function onPressCollectionBtn(tag)
	CCLuaLog("-----------onPressCollectionBtn---------------")
	FairyCollectionLayer.nextLayer = 1

	showUILayerByTag(UITagTable["FairyCollectionLayer"].tag,true)
end

-- 点击上一个按钮
local function onPressLastBtn(tag)
	if FairyExchangeLayer.pageIndex > 1 then
		FairyExchangeLayer.pageIndex = FairyExchangeLayer.pageIndex - 1
	end

	refreshExchangeLayer()
end

-- 点击下一个按钮
local function onPressNextBtn(tag)
	if FairyExchangeLayer.pageIndex < #FairyExchangeLayer.exchangeData then
		FairyExchangeLayer.pageIndex = FairyExchangeLayer.pageIndex + 1
	end

	refreshExchangeLayer()
end

-- 点击兑换按钮
local function onPressExchangeBtn(tag)
	if not exchangeState then
		GameApi:showMessage(GetLuaLocalization("M_EXCHANGE_LEVEL"))
	end

	local nowIndex = FairyExchangeLayer.pageIndex
	local nowData = FairyExchangeLayer.exchangeData[nowIndex]

	local exchangeId = nowData.exchangeId

	local msgBody = {
        exid = exchangeId,
    }
	send_message("CSExchangeHighQualityReq", msgBody, true)
	PushOneWaitingLayer("CSExchangeHighQualityReq")
end

-- 收到兑换成功的消息
local function onMsgFairyExchange(msg)
	if msg.is_succ then
		local exchangeId = msg.exid
		local itemId = 0
		local itemNum = 0

		for i = 1, #FairyExchangeLayer.exchangeData do
			if FairyExchangeLayer.exchangeData[i].exchangeId == exchangeId then
				itemId = FairyExchangeLayer.exchangeData[i].desId
				itemNum = FairyExchangeLayer.exchangeData[i].desNum
				break
			end
		end

		local itemName = ""
		if Items[itemId] ~= nil then
			itemName = GetLuaLocalization(Items[itemId].Name)
		end


		local infoStr = ""..string.format(GetLuaLocalization("M_EXCHANGE_SUCCESS"), itemNum)..itemName
		GameApi:showMessage(infoStr)
		refreshExchangeLayer()
	else
		GameApi:showMessage("exchange failed")	
	end

end

-- 显示tip及按钮
local function showItemTip(itemId, pos, showInfo)
	local function onOperationTipCallBackLeft(tag)
		if itemId > 10000 then
			itemId = itemId % 10000
		end

		if showInfo then
			FairyInfoLayer.fairyId = tonumber(itemId)
			FairyInfoLayer.buttonState = 0
			FairyInfoLayer.infoState = 2
			FairyInfoLayer:CreateLayer(nil)
		else
			FairyGetwayLayer:createLayer(tonumber(itemId))
		end		
	end

	local function onOperationTipCallBackRight(tag)
		if itemId > 10000 then
			itemId = itemId % 10000
		end

		FairyExchangeContactLayer.fairyId = itemId
		showUILayerByTag(UITagTable["FairyExchangeContactLayer"].tag,true)
	end


	ItemManager:Get():showItemTipsById(itemId, pos)

	local leftStr = GetLuaLocalization("M_FAIRY_GET_WAY")
	if showInfo then
		leftStr = GetLuaLocalization("M_EQUIPMENT_INFORMATION")
	end
	

	ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft, leftStr, true, onOperationTipCallBackRight, GetLuaLocalization("M_FAIRY_GROUP_TEXT"), true)
end

-- 点击精灵动画
local function onPressFairyBtn(tag)
	local nowIndex = FairyExchangeLayer.pageIndex
	local nowData = FairyExchangeLayer.exchangeData[nowIndex]

	local fairyId = nowData.desId

	local menu = fairyBtn:getMenuNode()
    local pos = menu:getTouchPoint()
	
	showItemTip(fairyId, pos, true)
end

-- 点击所需物品icon
local function onPressItemBtn(tag)
	local nowIndex = FairyExchangeLayer.pageIndex
	local nowData = FairyExchangeLayer.exchangeData[nowIndex]
	local resList = nowData.needItemList

	local itemId = resList[tag][1]

	local menu = itemBtns[tag]:getMenuNode()
    local pos = menu:getTouchPoint()

    showItemTip(itemId, pos, false)
end



function FairyExchangeLayer:InitLayer()
	CCLuaLog("--- FairyExchangeLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
	self.uiLayout:FindChildObjectByName("collectionBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCollectionBtn)
	self.uiLayout:FindChildObjectByName("lastBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressLastBtn)
	self.uiLayout:FindChildObjectByName("nextBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressNextBtn)
	self.uiLayout:FindChildObjectByName("exchangeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressExchangeBtn)
	self.uiLayout:FindChildObjectByName("fairyBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressFairyBtn)

	for i = 1, 4 do
		self.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__:getMenuItemSprite():setTag(i)
		self.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressItemBtn)
	end

	-- 各种初始化
	initExchangeData()
	initFairyScrollPage()
	refreshExchangeLayer()

	addMsgCallBack("CSExchangeHighQualityRsp", onMsgFairyExchange)

	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)
end