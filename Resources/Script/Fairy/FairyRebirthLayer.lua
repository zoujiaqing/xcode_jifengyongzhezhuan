require("Script/GameConfig/uiTagDefine")
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairyMessage")

FairyRebirthLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	fairyPos = 0,
	uiList = nil,
	itemIdTable = {},
	materialBtTable = {},
	fairyPosList = {},
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyRebirthLayout")
	FairyRebirthLayer:resetValue()
end

function FairyRebirthLayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyRebirthLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyRebirthUI.plist",self.uiLayerInstance, "FairyRebirthLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyRebirthLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiList = nil
	self.fairyPos = 0
	self.itemIdTable = {}
	self.fairyPosList = {}
	self.materialBtTable = {}
end
local function onCloseButtonClicked(tag)
	if FairyRebirthLayer.uiLayerInstance ~= nil then
		FairyRebirthLayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

local function onMaterialButtonClicked(tag)
	if FairyRebirthLayer.uiLayout ~= nil then
		--local materialBt = FairyRebirthLayer.uiLayout:FindChildObjectByName("materialBt" .. tag).__UIButton__
		local materialBt = FairyRebirthLayer.materialBtTable[tag]
		local pos = materialBt:getMenuNode():getTouchPoint()
		if FairyRebirthLayer.itemIdTable[tag] ~= nil then
			ItemManager:Get():showItemTipsById(FairyRebirthLayer.itemIdTable[tag],pos)
		end
	end
end

local function onConfirmButtonClicked(tag)
	sendFairyRebirthReq(FairyRebirthLayer.fairyPosList,false)
end

local function onCancellButtonClicked(tag)
	if FairyRebirthLayer.uiLayerInstance ~= nil then
		FairyRebirthLayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

--function FairyRebirthLayer:hideSingleMaterial(index)
--	local materialIcon = self.uiLayout:FindChildObjectByName("materialBt" .. index)
--	if 	materialIcon ~= nil then
--		local bt = materialIcon.__UIButton__
--		bt:setVisible(false)
--	end
--end

function FairyRebirthLayer:setSingleMaterial(itemId,itemNum,index)
	CCLuaLog("setSingleMaterial: " .. itemId)
	--local materialIcon = self.uiLayout:FindChildObjectByName("materialBt" .. index)
	if self.uiList ~= nil then
		local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_REBIRTH_MATERIAL1")
		local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_REBIRTH_MATERIAL1_CLICKED")
		local button = self.uiList:AddNewItem(bgFrame,clickedFrame,"","",ccc3(255,255,255),-138)
		local menuItem = button:getMenuItemSprite()
		menuItem:setTag(index)
		menuItem:registerScriptTapHandler(onMaterialButtonClicked)

		local icon = ItemManager:Get():getIconSpriteById(itemId)
		button:setIconSprite(icon)
		local size = button:getMenuItemSprite():getContentSize()
		button:setTextLabel(itemNum,18,ccc3(255,171,0),KJLinXin, ccp(size.width * (0.4),size.height * (-0.4)),ccp(1,0))
		self.itemIdTable[index] = itemId
		self.materialBtTable[index] = button
		--local bt = materialIcon.__UIButton__
		--local icon = ItemManager:Get():getIconSpriteById(itemId)
		--bt:setIconSprite(icon)
		--bt:setVisible(true)
		--local size = bt:getMenuItemSprite():getContentSize()
		--bt:setTextLabel(itemNum,18,ccc3(255,171,0),KJLinXin, ccp(size.width * (0.4),size.height * (-0.4)),ccp(1,0))
		--self.itemIdTable[index] = itemId
	end
end

function FairyRebirthLayer:setMaterial(itemTable)
	local index = 1
	for k,v in pairs(itemTable) do
		local itemId = v.id
		local itemNum = v.num
		self:setSingleMaterial(itemId,itemNum,index)
		index = index + 1
	end
	--if index < 5 then
	--	for i = index , 5 do 
	--		self:hideSingleMaterial(i)
	--	end
	--end
end

function FairyRebirthLayer:onReceiveFairyRebirthRsp(msg)
	if msg.req_info then
		if msg.to_items ~= nil then
			self:setMaterial(msg.to_items)
		end

		local size = self.uiList:getContentSize()
		local viewSize = self.uiList:getViewSize()
		local fairyListStartPos = self.uiList:getContentOffset()
		local offset = ccp(fairyListStartPos.x + size.width - viewSize.width,fairyListStartPos.y)
		self.uiList:setStartOffset(offset)
	else
		GameApi:showMessage(GetLuaLocalization("M_FAIRY_REBIRTH_SUCCESS"))
		FairyBagLayer:clearFairyBag()
		FairyBagLayer:InitBagFairyData()
		onCloseButtonClicked(nil)
	end
end

--function FairyRebirthLayer:onReceiveFairyDecomposeRsp(msg)
--	if msg.req_info then
--		if msg.to_items ~= nil then
--			self:setMaterial(msg.to_items)
--		end
--	else
--		GameApi:showMessage(GetLuaLocalization("M_FAIRY_BAG_1"))
--		FairyBagLayer:removeOneFairyInBag(msg.fairy_pos)
--		onCloseButtonClicked(nil)
--	end
--end


function FairyRebirthLayer:InitLayer()
	self.uiList = self.uiLayout:FindChildObjectByName("fairyList").__UIScrollList__
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	confirmBt:registerScriptTapHandler(onConfirmButtonClicked)
	local cancellBt = self.uiLayout:FindChildObjectByName("cancellBt").__UIButton__:getMenuItemSprite()
	cancellBt:registerScriptTapHandler(onCancellButtonClicked)
	--for i = 1,5 do
	--	local materialBt = self.uiLayout:FindChildObjectByName("materialBt" .. i).__UIButton__
	--	materialBt:setVisible(false)
	--	local materialItem = materialBt:getMenuItemSprite()
	--	materialItem:registerScriptTapHandler(onMaterialButtonClicked)
	--	materialItem:setTag(i)
	--end
end

function FairyRebirthLayer:setFairyPos(fairyList)
	self.fairyPosList = fairyList

	local titleStr = ""
	local statusStr = ""
	local expStr = ""
	--local decomposeLabel = self.uiLayout:FindChildObjectByName("decomposeLabel").__UILabel__
	--
	--decomposeLabel:setVisible(false)
	titleStr = GetLuaLocalization("M_FAIRY_REBIRTH_TITLE1")
	statusStr = GetLuaLocalization("M_FAIRY_REBIRTHALL")
	expStr = GetLuaLocalization("M_FAIRY_REBIRTH_EXPLANATION1")

	local label = self.uiLayout:FindChildObjectByName("titleLabel").__UILabel__
	label:setString(titleStr)
	label = self.uiLayout:FindChildObjectByName("rebirthLabel").__UILabel__
	label:setString(statusStr)
	label = self.uiLayout:FindChildObjectByName("explantionLabel").__UILabel__
	label:setString(expStr)
 
	--local fairyInfo = getUserFairyByPos(fairyPos)
	--local fairyBasicInfo = getFairyBasicInfo(fairyInfo.fairyId)
	--if fairyInfo ~= nil and fairyBasicInfo ~= nil then
	--	label = self.uiLayout:FindChildObjectByName("nameLabel").__UILabel__
	--	local str = fairyBasicInfo.name
	--	if fairyInfo.fairyGrade > 0 then
	--		str = str .. "+" .. fairyInfo.fairyGrade
	--	end
	--	label:setString(str)
	--	label:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))
	--
	--	label = self.uiLayout:FindChildObjectByName("levelLabel").__UILabel__
	--	label:setString("Lv." .. fairyInfo.fairyLevel)
	--end
	
	sendFairyRebirthReq(self.fairyPosList,true)
end