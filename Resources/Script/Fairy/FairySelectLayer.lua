require("Script/GameConfig/uiTagDefine")
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairySelectListItem")
require("Script/Fairy/FairyDecomposeLayer")

FairySelectLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	isMultiSelect = false,
	isAllFairy = false,
	multiSelectNum = 0,
	maxQuality = 10,
	minQuality = 0,
	exceptPos = nil,
	isRebirthModle = false,
	isDecomposeModle = false,
	isShowExp = false,
	isAllSelectClicked = false,
	isBlueSelectClicked = false,
	isPurpleSelectClicked = false,
	isFiveSelectClicked = false,
	updateFunc = 0,
	updateSum = 0,
	fairyListTable = {},
	fairyListItemTable = {},
	selectedItemTable = {}, -- 已选择的精灵列表(界面打开时重置)
}
-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("FairySelectLayout")
	if FairySelectLayer.updateFunc ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(FairySelectLayer.updateFunc)
	end
	FairySelectLayer:resetValue()
end

function FairySelectLayer:getSelectedFairys()
	return self.selectedItemTable
end

function FairySelectLayer:createLayer()
	if self.uiLayerInstance == nil then 
		--self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairySelectLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/fairySelectUI.plist",self.uiLayerInstance, "FairySelectLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairySelectLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.isMultiSelect = false
	self.isAllFairy = false
	self.multiSelectNum = 0
	self.maxQuality = 10
	self.minQuality = 0
	self.exceptPos = nil
	self.isShowExp = false
	self.isRebirthModle = false
	self.isDecomposeModle = false
	self.isAllSelectClicked = false
	self.isBlueSelectClicked = false
	self.isPurpleSelectClicked = false
	self.isFiveSelectClicked = false
	self.updateFunc = 0
	self.updateSum = 0
	self.fairyListTable = {}
	self.fairyListItemTable = {}
end

local function onCloseButtonClicked(tag)
	if FairySelectLayer.uiLayerInstance ~= nil then
		FairySelectLayer.uiLayerInstance:removeFromParentAndCleanup(true)
		GameAudioManager:sharedManager():playEffect(350001,false)
	end
end

local function onChooseButtonClicked(tag)
	local num = table.getn(FairySelectLayer.selectedItemTable)
	if num > 0 then
		if FairySelectLayer.uiLayerInstance ~= nil then
			FairySelectLayer.uiLayerInstance:removeFromParentAndCleanup(true)
		end
		NotificationCenter:defaultCenter():broadcast(GM_FAIRY_SELECT_DONE,nil,"","")
	else
		GameApi:showMessage("您还没有选择精灵")
	end
end

function FairySelectLayer:checkAllselectStatus(pos)
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		fairyId = fairy.fairyId
		local fairyInfo = getFairyBasicInfo(fairyId)
		if fairyInfo ~= nil then 
			if self.isDecomposeModle then
				if fairyInfo.quality == 3 then
					if FairySelectLayer.isBlueSelectClicked then
						FairySelectLayer.isBlueSelectClicked = false
						local decomposeBlueBt = FairySelectLayer.uiLayout:FindChildObjectByName("decomposeBlueBt").__UIButtonToggle__
						decomposeBlueBt:unselected()
					end
				elseif fairyInfo.quality == 4 then
					if FairySelectLayer.isPurpleSelectClicked then
						FairySelectLayer.isPurpleSelectClicked = false
						local decomposePurpleBt = FairySelectLayer.uiLayout:FindChildObjectByName("decomposePurpleBt").__UIButtonToggle__
						decomposePurpleBt:unselected()
					end
				elseif fairyInfo.quality == 5 then
					if FairySelectLayer.isFiveSelectClicked then
						FairySelectLayer.isFiveSelectClicked = false
						local decomposeFiveBt = FairySelectLayer.uiLayout:FindChildObjectByName("decomposeFiveBt").__UIButtonToggle__
						decomposeFiveBt:unselected()
					end
				end
			elseif self.isRebirthModle then
				local rebirthAllBt = FairySelectLayer.uiLayout:FindChildObjectByName("rebirthAllBt").__UIButtonToggle__
				rebirthAllBt:unselected()
			end
		end
	end
end

function FairySelectLayer:SelectItem(pos)
	if self.fairyListTable[pos] ~= nil then
		local lastStatus = self.fairyListTable[pos].isSelectFlag
		if lastStatus then
			-- 已经是选中状态，改为非选中
			self.fairyListTable[pos]:setItemStatus(not lastStatus)
			FairySelectLayer:checkAllselectStatus(pos)
			for k, v in pairs(self.selectedItemTable) do
				if v == pos then
					table.remove(self.selectedItemTable,k)
				end
			end
		else
			local num = table.getn(self.selectedItemTable)
			if self.isMultiSelect then
				-- 多选模式
				if num < self.multiSelectNum then
					table.insert(self.selectedItemTable,pos)
					self.fairyListTable[pos]:setItemStatus(not lastStatus)
				else
					GameApi:showMessage("已到最大可选数量")
				end
			else
				-- 单选模式
				if num > 0 then
					for k,v in pairs(self.selectedItemTable) do
						if self.fairyListTable[v] ~= nil then
							self.fairyListTable[v]:setItemStatus(false)
						end
					end
				end
				self.selectedItemTable = {}
				table.insert(self.selectedItemTable,pos)
				self.fairyListTable[pos]:setItemStatus(not lastStatus)
			end
		end
		
	end
end

local function onItemSelected(tag)
	if not FairySelectLayer:checkItemInView(tag) then
		-- 选中的时候该列处于隐藏状态
		if FairySelectLayer.fairyListTable[tag] ~= nil and FairySelectLayer.fairyListTable[tag].isSelectFlag then
			-- 将选中的效果还原为原中
			FairySelectLayer.fairyListTable[tag]:setItemStatus(tag,FairySelectLayer.fairyListTable[tag].isSelectFlag)
		end
		return
	end
	--CCLuaLog("onItemSelected: " .. tag)
    if FairySelectLayer.fairyListTable[tag] ~= nil then
        TutorialsManager:Get():HandleOneEvent("FairySelect_"..FairySelectLayer.fairyListTable[tag].index, 1)
    end
	FairySelectLayer:SelectItem(tag)
end

local function onDecomposeBlueButtonClicked(tag)
	FairySelectLayer.isBlueSelectClicked = not FairySelectLayer.isBlueSelectClicked
	local decomposeBlueBt = FairySelectLayer.uiLayout:FindChildObjectByName("decomposeBlueBt").__UIButtonToggle__
	if FairySelectLayer.isBlueSelectClicked then
		decomposeBlueBt:selected()
	else
		decomposeBlueBt:unselected()
	end
	for k,v in pairs(FairySelectLayer.fairyListTable) do
		if FairySelectLayer.fairyListTable[k] ~= nil and (FairySelectLayer.isBlueSelectClicked == not FairySelectLayer.fairyListTable[k].isSelectFlag) then
			local fairy = getUserFairyByPos(k)
			if fairy ~= nil then
				fairyId = fairy.fairyId
				local fairyInfo = getFairyBasicInfo(fairyId)
				if fairyInfo ~= nil and fairyInfo.quality == 3 then
					FairySelectLayer:SelectItem(k)
				end
			end
			
		end
	end
end

local function onDecomposePurpleButtonClicked(tag)
	FairySelectLayer.isPurpleSelectClicked = not FairySelectLayer.isPurpleSelectClicked
	local decomposePurpleBt = FairySelectLayer.uiLayout:FindChildObjectByName("decomposePurpleBt").__UIButtonToggle__
	if FairySelectLayer.isPurpleSelectClicked then
		decomposePurpleBt:selected()
	else
		decomposePurpleBt:unselected()
	end
	for k,v in pairs(FairySelectLayer.fairyListTable) do
		if FairySelectLayer.fairyListTable[k] ~= nil and (FairySelectLayer.isPurpleSelectClicked == not FairySelectLayer.fairyListTable[k].isSelectFlag) then
			local fairy = getUserFairyByPos(k)
			if fairy ~= nil then
				fairyId = fairy.fairyId
				local fairyInfo = getFairyBasicInfo(fairyId)
				if fairyInfo ~= nil and fairyInfo.quality == 4 then
					FairySelectLayer:SelectItem(k)
				end
			end
			
		end
	end
end

local function onDecomposeFiveButtonClicked(tag)
	FairySelectLayer.isFiveSelectClicked = not FairySelectLayer.isFiveSelectClicked
	local decomposeFiveBt = FairySelectLayer.uiLayout:FindChildObjectByName("decomposeFiveBt").__UIButtonToggle__
	if FairySelectLayer.isFiveSelectClicked then
		decomposeFiveBt:selected()
	else
		decomposeFiveBt:unselected()
	end
	for k,v in pairs(FairySelectLayer.fairyListTable) do
		if FairySelectLayer.fairyListTable[k] ~= nil and (FairySelectLayer.isFiveSelectClicked == not FairySelectLayer.fairyListTable[k].isSelectFlag) then
			local fairy = getUserFairyByPos(k)
			if fairy ~= nil then
				fairyId = fairy.fairyId
				local fairyInfo = getFairyBasicInfo(fairyId)
				if fairyInfo ~= nil and fairyInfo.quality == 5 then
					FairySelectLayer:SelectItem(k)
				end
			end
			
		end
	end
end



local function onDecomposeButtonClicked(tag)
	local num = table.getn(FairySelectLayer.selectedItemTable)
	if num > 0 then
		FairyDecomposeLayer:createLayer()
		FairyDecomposeLayer:setFairyPos(FairySelectLayer.selectedItemTable)
		onCloseButtonClicked(nil)
	else
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_CHOOSE_DECOMPOSE"))
	end
end

local function onRebirthAllSelectButtonClicked(tag)
	FairySelectLayer.isAllSelectClicked = not FairySelectLayer.isAllSelectClicked
	local rebirthAllBt = FairySelectLayer.uiLayout:FindChildObjectByName("rebirthAllBt").__UIButtonToggle__
	if FairySelectLayer.isAllSelectClicked then
		rebirthAllBt:selected()
	else
		rebirthAllBt:unselected()
	end
	for k,v in pairs(FairySelectLayer.fairyListTable) do
		if FairySelectLayer.fairyListTable[k] ~= nil and (FairySelectLayer.isAllSelectClicked == not FairySelectLayer.fairyListTable[k].isSelectFlag) then
			FairySelectLayer:SelectItem(k)
		end
	end
end

local function onRebirthButtonClicked(tag)
local num = table.getn(FairySelectLayer.selectedItemTable)
	if num > 0 then
		FairyRebirthLayer:createLayer()
		FairyRebirthLayer:setFairyPos(FairySelectLayer.selectedItemTable)
		onCloseButtonClicked(nil)
	else
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_CHOOSE_REBIRTH"))
	end
end

function FairySelectLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local chooseBt = self.uiLayout:FindChildObjectByName("chooseBt").__UIButton__:getMenuItemSprite()
	chooseBt:registerScriptTapHandler(onChooseButtonClicked)
	self.selectedItemTable = {}
end

function FairySelectLayer:checkFairyCanSelected(pos)
	if self.exceptPos == pos then
		return false
	end
	local quality = 0
	local grade = 0
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		fairyId = fairy.fairyId
		grade = fairy.fairyGrade
		local fairyInfo = getFairyBasicInfo(fairyId)
		if fairyInfo ~= nil then
			quality = fairyInfo.quality
		end
	end
	if self.maxQuality ~= nil and self.maxQuality <= quality then
		return false
	end
	if self.minQuality ~= nil and self.minQuality > quality then
		return false
	end
	if self.isRebirthModle and grade < 1 then
		return false
	end
	if self.isDecomposeModle and grade >= 1 then
		return false
	end
	if self.isAllFairy then
		return true
	elseif pos >= FAIRY_POS_START_NOM then
		return true
	end

	return false
end

function FairySelectLayer:checkItemInView(pos)
	if self.fairyListItemTable ~= nil and self.fairyListItemTable[pos] ~= nil then
		local uiList = self.uiLayout:FindChildObjectByName("fairyItemList").__UIScrollList__
		return uiList:checkChildInView(self.fairyListItemTable[pos])
	end
	return false
end

local function fairySelectUpdate(dt)
	FairySelectLayer.updateSum = FairySelectLayer.updateSum + 1
	if FairySelectLayer.updateSum > 1 then
		if FairySelectLayer.updateFunc ~= 0 then
			CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(FairySelectLayer.updateFunc)
		end
		FairySelectLayer:InitUI()
		if FairySelectLayer.isRebirthModle then
			FairySelectLayer:InitRebirthUI()
		end
		if FairySelectLayer.isDecomposeModle then
			FairySelectLayer:InitDecomposeUI()
		end
		WaitingLayerManager:Get():clearMessages()
	end
end

function FairySelectLayer:InitUI()
	local fairyList = getUserFairyList()
	if fairyList ~= nil then
		local uiList = self.uiLayout:FindChildObjectByName("fairyItemList").__UIScrollList__
		local sortLable = {}
		for k,v in pairs(fairyList) do
			table.insert(sortLable,v)
		end
		table.sort(sortLable,compareFairyOrder)
		local control = nil 
		local itemNum = 0
		for k,v in pairs(sortLable) do
			if v ~= nil and self:checkFairyCanSelected(v.pos) then
				--CCLuaLog("v.pos" .. v.pos)
				itemNum = itemNum + 1
				local index = itemNum % 2
				if (control == nil) or (index == 1) then
					local baseNode = CCSprite:create()
					local bgFrame,framePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME")
					control = uiList:AddBaseItem(bgFrame:getContentSize(),baseNode)
					--bgPt = framePt
				end
				local fairyItem = FairySelectListItem:create(control:getCurrentNode(),v,itemNum,self.isShowExp)
				self.fairyListTable[v.pos] = fairyItem
				self.fairyListItemTable[v.pos] = control
				if index ~= 1 then
					local firstFrame,firstPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1")
					local secondFrame,secondPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME2")
					fairyItem.baseNode:setPosition(ccpSub(secondPt,firstPt))
				end
				fairyItem:setButtonCallBack(onItemSelected)
			end
		end
	end
end

function FairySelectLayer:InitDecomposeUI()
	local container = self.uiLayout:FindChildObjectByName("decomposeContainer").__UIContainer__
	container:setVisible(true)
	local titleLable = self.uiLayout:FindChildObjectByName("titleLabel").__UILabel__
	titleLable:setString(LanguageLocalization:GetLocalization("M_FAIRY_REBIRTH_TITLE2"))
	local decomposeBlueBt = self.uiLayout:FindChildObjectByName("decomposeBlueBt").__UIButtonToggle__:getMenuItemToggle()
	decomposeBlueBt:registerScriptTapHandler(onDecomposeBlueButtonClicked)
	local decomposePurpleBt = self.uiLayout:FindChildObjectByName("decomposePurpleBt").__UIButtonToggle__:getMenuItemToggle()
	decomposePurpleBt:registerScriptTapHandler(onDecomposePurpleButtonClicked)
	local decomposeFiveBt = self.uiLayout:FindChildObjectByName("decomposeFiveBt").__UIButtonToggle__:getMenuItemToggle()
	decomposeFiveBt:registerScriptTapHandler(onDecomposeFiveButtonClicked)
	local decomposeBt = self.uiLayout:FindChildObjectByName("decomposeBt").__UIButton__:getMenuItemSprite()
	decomposeBt:registerScriptTapHandler(onDecomposeButtonClicked)

	local chooseBt = self.uiLayout:FindChildObjectByName("chooseBt").__UIButton__
	chooseBt:setVisible(false)
	local chooseLabel = self.uiLayout:FindChildObjectByName("chooseLabel").__UILabel__
	chooseLabel:setVisible(false)
end
function FairySelectLayer:InitRebirthUI()
	local container = self.uiLayout:FindChildObjectByName("rebirthContainer").__UIContainer__
	container:setVisible(true)
	local titleLable = self.uiLayout:FindChildObjectByName("titleLabel").__UILabel__
	titleLable:setString(LanguageLocalization:GetLocalization("M_FAIRY_REBIRTH_TITLE1"))
	local rebirthAllBt = self.uiLayout:FindChildObjectByName("rebirthAllBt").__UIButtonToggle__:getMenuItemToggle()
	rebirthAllBt:registerScriptTapHandler(onRebirthAllSelectButtonClicked)
	local rebirthBt = self.uiLayout:FindChildObjectByName("rebirthBt").__UIButton__:getMenuItemSprite()
	rebirthBt:registerScriptTapHandler(onRebirthButtonClicked)

	local chooseBt = self.uiLayout:FindChildObjectByName("chooseBt").__UIButton__
	chooseBt:setVisible(false)
	local chooseLabel = self.uiLayout:FindChildObjectByName("chooseLabel").__UILabel__
	chooseLabel:setVisible(false)
	
end


-- 设置选择模式:isMultiSelect 多选/单选 isAllFairy 所有精灵/非出战精灵 , exceptPos 需要排除的精灵位置 , quality 品质过滤(不高于此)
function FairySelectLayer:setSelectModle(isMultiSelect,isAllFairy,multiSelectNum,exceptPos,quality,isShowExp)
	self.isMultiSelect = isMultiSelect
	self.isAllFairy = isAllFairy
	self.multiSelectNum = multiSelectNum
	self.exceptPos = exceptPos
	self.maxQuality = quality
	if isShowExp ~= nil then
		self.isShowExp = isShowExp
	end
	if FairySelectLayer.updateFunc == 0 then
		FairySelectLayer.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(fairySelectUpdate,0.6,false)
		PushOneWaitingLayer("fairySelect")
	end
end

-- 设置默认选择的位置,selectedTable为table，中间存放需要选择的pos值
function FairySelectLayer:setDefaultSelectPos(selectedTable)
	if selectedTable ~= nil then
		for k,v in pairs(selectedTable) do
			self:SelectItem(v)
		end
	end
end

-- 设置为精灵重生模式
function FairySelectLayer:setRebirthModle()
	self.isRebirthModle = true
	self.isMultiSelect = true
	self.isAllFairy = false
	self.multiSelectNum = 200
	self.minQuality = 3
	self.isShowExp = false
	if FairySelectLayer.updateFunc == 0 then
		FairySelectLayer.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(fairySelectUpdate,0.6,false)
		PushOneWaitingLayer("fairySelect")
	end
end

-- 设置为精灵分解模式
function FairySelectLayer:setDecomposeModle()
	self.isDecomposeModle = true
	self.isMultiSelect = true
	self.isAllFairy = false
	self.multiSelectNum = 200
	self.minQuality = 3
	self.isShowExp = false
	if FairySelectLayer.updateFunc == 0 then
		FairySelectLayer.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(fairySelectUpdate,0.6,false)
		PushOneWaitingLayer("fairySelect")
	end
end