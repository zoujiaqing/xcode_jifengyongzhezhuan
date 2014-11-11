-- 精灵图鉴页面
require("Script/Fairy/FairyConfigTable")
require("Script/FairyLayer/FairyInfoLayer")

FairyCollectionLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

	nextLayer = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if FairyCollectionLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyCollectionLayer.notificationFunc);
	end

	FairyCollectionLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyCollectionUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyCollectionLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyCollectionLayer.uiLayerInstance then
				FairyCollectionLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function CreateFairyCollectionLayer()
	FairyCollectionLayer:CreateLayer()
end

function FairyCollectionLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyCollectionLayer"].tag
			parentNode:addChild(self.uiLayerInstance,19,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyCollectionUI.plist",self.uiLayerInstance, "FairyCollectionUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyCollectionLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function FairyCollectionLayer:destroyed()
	CCLuaLog("--- FairyCollectionLayer:destroyed() ---")
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
-- nextLayer : 1. 六品兑换
local function onPressCloseBtn(tag)
	RemoveOneLayer(FairyCollectionLayer.uiLayerInstance)
	FairyCollectionLayer:destroyed()

	if FairyCollectionLayer.nextLayer ~= 0 then		

		if FairyCollectionLayer.nextLayer == 1 then
			FairyCollectionLayer.nextLayer = 0
			showUILayerByTag(UITagTable["FairyExchangeLayer"].tag,true)
		end

		
	end
end

local nowSheetIndex = 1
local rowLists = {}

local fairyBtns = {}
local fairyIconNodes = {}
local fairyNameLabels = {}

local fairyIds = {}

-- 点击某个精灵按钮
local function onPressFairyBtn(tag)
	--CCLuaLog("--- onPressFairyBtn ---"..tag)
    local rowIndex = math.floor((tag - 1) / 6) + 1
    local bgControl = rowLists[rowIndex]
    local contentList = FairyCollectionLayer.uiLayout:FindChildObjectByName("contentScrollList").__UIScrollList__
    if not contentList:checkChildInView(bgControl) then
        return
    end

	local fairyId = fairyIds[nowSheetIndex][tag]
	
	local fairyBasicInfo = getFairyBasicInfo(fairyId)
	if fairyBasicInfo.quality > 5 and not checkCollectionFairyStatus(fairyId) then
		GameApi:showMessage(LanguageLocalization:GetLocalization("FAIRY_LOCK_HINT"))
	else
		FairyInfoLayer.fairyId = fairyId
		CCLuaLog("--- FairyInfoLayer.fairyId ---"..fairyId)

		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 2
		FairyInfoLayer:CreateLayer(nil)
	end
end

-- 刷新图鉴列表
local function refreshFairyPage()
	CCLuaLog("--- refreshFairyPage ---")

	local contentList = FairyCollectionLayer.uiLayout:FindChildObjectByName("contentScrollList").__UIScrollList__
	contentList:setStartOffset(ccp(0, 0))

    -- 获得单行的高度
    local rowWidth = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_FAIRY_DATABASE_FRAME_ALL"):getContentSize().width
    local rowHeight = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_FAIRY_DATABASE_FRAME_ALL"):getContentSize().height + 5

	--添加新的整行
	local function addRow(index)
		--CCLuaLog("--- addRow ---"..index)
		local itemBg = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_FAIRY_DATABASE_FRAME_ALL")
		local bgListItemSize = itemBg:getContentSize()		
		local ListItemNode = CCNode:create()

		local bgControl = contentList:AddBaseItem(bgListItemSize, ListItemNode)
		rowLists[index] = bgControl
	end

	--添加单个按钮组件
	local function addBtn(index)
		--CCLuaLog("--- addBtn ---"..index)
		if index > #rowLists * 6 then
			addRow(math.floor((index - 1) / 6) + 1)
		end

		local row = math.floor((index - 1) / 6) + 1
		local bgControl = rowLists[row]

		local rowIndex = index - (row - 1) * 6

		--添加背景按钮
		--CCLuaLog("--- addBgBtn ---")
		local _, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_DATABASE_FRAME_ALL")
		local normalSprite, fairyBtnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_DATABASE_FRAME"..rowIndex)
		local clickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_DATABASE_FRAME"..rowIndex)
 
		fairyBtn = IconButton:create(normalSprite, nil, clickedSprite, -133)
		fairyBtn:setPosition(ccpSub(fairyBtnPos, bgPos))

		local btnMenuItem = fairyBtn:getMenuItem()
		btnMenuItem:setTag(index)

		btnMenuItem:registerScriptTapHandler(onPressFairyBtn)
	
		bgControl:getCurrentNode():addChild(fairyBtn)

		fairyBtns[index] = fairyBtn

		--精灵icon
		local fairyBasicInfo = getFairyBasicInfo(fairyIds[nowSheetIndex][index])
		local fairyIcon = fairyBasicInfo:getFairyIcon()

		local _, fairyIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_DATABASE_FRAME"..rowIndex.."_ICON")

		local fairyNode = CCNode:create()
		fairyNode:setPosition(ccpSub(fairyIconPos, fairyBtnPos))
		fairyNode:addChild(fairyIcon)

		fairyBtn:addChild(fairyNode, 10)

		fairyIconNodes[index] = fairyNode

		--精灵名称
		local fairyName = fairyBasicInfo.name
		local nameFrame, nameLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_DATABASE_FRAME"..rowIndex.."_NAME")
		local nameLabel = TXGUI.UILabelTTF:create(fairyName, KJLinXin, 18, nameFrame:getContentSize(),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter)
		nameLabel:setPosition(ccpSub(nameLabelPos, fairyBtnPos))
        nameLabel:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))

		fairyBtn:addChild(nameLabel, 10)
		fairyNameLabels[index] = nameLabel
		
		local getFrame, getPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_DATABASE_FRAME"..rowIndex.."_ALREADYGOT")
		local alreadyIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_FAIRY_ALREADYGOT")
		alreadyIcon:setPosition(ccpSub(getPos, fairyBtnPos))
		fairyBtn:addChild(alreadyIcon, 15,5000)
		if not checkCollectionFairyStatus(fairyIds[nowSheetIndex][index]) then
			alreadyIcon:setVisible(false)
		end

	end

	--更新某个按钮组件上的精灵
	local function refreshFairy(index, fairyId)
		--CCLuaLog("--- refreshFairy ---"..index)
		if fairyBtns[index] == nil then
			addBtn(index)
		end

		fairyBtns[index]:setVisible(true)

		local fairyBasicInfo = getFairyBasicInfo(fairyIds[nowSheetIndex][index])

		-- 更新数据
		fairyIconNodes[index]:removeAllChildrenWithCleanup(true)
		if fairyBasicInfo.quality > 5 and not checkCollectionFairyStatus(fairyId) then
			local newIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_FAIRY_UNLOCK")
			fairyIconNodes[index]:addChild(newIcon)
			fairyNameLabels[index]:setString("???")
		else
			local newIcon = fairyBasicInfo:getFairyIcon()
			fairyIconNodes[index]:addChild(newIcon)
			fairyNameLabels[index]:setString(fairyBasicInfo.name)
		end
		fairyNameLabels[index]:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))

		local statusNode = fairyBtns[index]:getChildByTag(5000)
		if checkCollectionFairyStatus(fairyIds[nowSheetIndex][index]) then
			if statusNode ~= nil then
				statusNode:setVisible(true)
			end
		else
			if statusNode ~= nil then
				statusNode:setVisible(false)
			end
		end
	end

	for i = 1, #fairyIds[nowSheetIndex] do
		refreshFairy(i, fairyIds[nowSheetIndex][i])
	end

	if #fairyBtns > #fairyIds[nowSheetIndex] then
		for i = #fairyIds[nowSheetIndex] + 1, #fairyBtns do
			fairyBtns[i]:setVisible(false)
		end
	end

    -- 重新设置界面大小
    local scale = TXGUI.UIManager:sharedManager():getScaleFactor()
    local rowNum = math.floor((#fairyIds[nowSheetIndex] - 1) / 6) + 1
    local newSize = CCSize(rowWidth * scale, rowHeight * rowNum * scale)
    contentList:setContentSize(newSize)
end

--点击某个页面标签
local function onPressSelectBtn(tag)
	CCLuaLog("--- onPressSelectBtn ---"..tag)

	for	i = 1, 4 do
		if tag == i then
			FairyCollectionLayer.uiLayout:FindChildObjectByName("sheetBtn"..i).__UIButton__:getMenuItemSprite():selected()
		else
			FairyCollectionLayer.uiLayout:FindChildObjectByName("sheetBtn"..i).__UIButton__:getMenuItemSprite():unselected()
		end
	end

	nowSheetIndex = tag
	refreshFairyPage()
end

local function initFairyIds()
	fairyIds = {}
	fairyIds[1] = {}
	fairyIds[2] = {}
	fairyIds[3] = {}
	fairyIds[4] = {}
	for k , v in pairs(fairys) do
		if tonumber(v.Quality) == 1 then
			fairyIds[4][#(fairyIds[4]) + 1] = k
		elseif tonumber(v.Quality) == 2 then
			fairyIds[3][#(fairyIds[3]) + 1] = k
		elseif tonumber(v.Quality) == 3 then
			fairyIds[2][#(fairyIds[2]) + 1] = k
		elseif tonumber(v.Quality) >= 4 then
			fairyIds[1][#(fairyIds[1]) + 1] = k
		end
	end

	local function sortFunc(a, b)
		local quality_a = getFairyQualityById(a)
		local quality_b = getFairyQualityById(b)
		if quality_a ~= quality_b then
			return quality_a < quality_b
		else
			return b > a
		end
	end

	for i = 1, 4 do
		table.sort(fairyIds[i], sortFunc)
	end
	
end

function FairyCollectionLayer:InitLayer()
	CCLuaLog("--- FairyCollectionLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)

	

	-- 4个页面标签
	for i = 1, 4 do
		self.uiLayout:FindChildObjectByName("sheetBtn"..i).__UIButton__:getMenuItemSprite():setTag(i)		
		self.uiLayout:FindChildObjectByName("sheetBtn"..i).__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressSelectBtn)
	end

	-- 初始化数据
	rowLists = {}

	fairyBtns = {}
	fairyIconNodes = {}
	fairyNameLabels = {}

	initFairyIds()

	onPressSelectBtn(1)


	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)
end