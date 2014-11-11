require("Script/GameConfig/uiTagDefine")
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairyBagListItem")
require("Script/Fairy/FairySelectLayer")
require("Script/Fairy/FairyFragListItem")
require("Script/FairyLayer/FairyAdvanceLayer")
require("Script/FairyLayer/FairyLevelUpLayer")
require("Script/FairyLayer/FairyTrainLayer")
require("Script/FairyLayer/FairyCollectionLayer")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/Fairy/FairyRebirthLayer")
require("Script/Fairy/FairyGetwayLayer")

FairyBagLayer = {
	TotalFairyBagCap = 200,
	uiLayerInstance = nil,
	uiLayout = nil,
	bagToggle = nil,
	fragToggle = nil,
	bagContainer = nil,
	fragContainer = nil,
	isFairyBagInit = false,
	isFairyFragInit = false,
	fairyItemList = nil,
	fairyNumLabel = nil,
	listItemTable = {},
	fragItemTable = {},
	fairyShowedTable = {},
	fairyOrderedTable = {},
	fairyListIndex = 1,
	notificationFunc = 0,
	lastClickFairyItemTag = 0,
}
-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RemoveOneLayer(FairyBagLayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("FairyBagLayout")
	if FairyBagLayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyBagLayer.notificationFunc)
	end
	FairyBagLayer:resetValue()
	SetChatBar(true,-1)
end

function FairyBagLayer:createLayer()
	if self.uiLayerInstance == nil then 
		--self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyBagLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/fairyBagUI.plist",self.uiLayerInstance, "FairyBagLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyBagLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.bagToggle = nil
	self.fragToggle = nil
	self.bagContainer = nil
	self.fragContainer = nil
	self.isFairyBagInit = false
	self.isFairyFragInit = false
	self.fairyItemList = nil
	self.fairyNumLabel = nil
	self.listItemTable = {}
	self.fragItemTable = {}
	self.fairyShowedTable = {}
	self.fairyOrderedTable = {}
	self.fairyListIndex = 1
	self.notificationFunc = 0
	self.lastClickFairyItemTag = 0
end

function FairyBagLayer:onReceiveFairyComposeRsp(exchangeId)
	--CCLuaLog("FairyBagLayer:onReceiveFairyComposeRsp(exchangeId)  " ..exchangeId )
	if self.uiLayerInstance ~= nil then
		if ExchangeItems[exchangeId] ~= nil and ExchangeItems[exchangeId].Type == "12" then
			if self.isFairyFragInit then
				for k,v in pairs(self.fragItemTable) do
					if v.exchangeId == exchangeId then
						v:updateFragAmount()
					end
				end
			end
			if self.isFairyBagInit then
				 self:clearFairyBag()
			end
		end
	end
end

function FairyBagLayer:onReceiveFairyEquipedRsp()
	if self.uiLayerInstance ~= nil then
		if self.isFairyBagInit then
			 self:clearFairyBag()
			 self:InitBagFairyData()
		end
	end
end

function FairyBagLayer:clearFairyBag()
	self.listItemTable = {}
	self.fairyShowedTable = {}
	if self.fairyItemList ~= nil then
		self.fairyItemList:clearChildrens()
		self.isFairyBagInit = false
		self.fairyListIndex = 1
	end
end

function FairyBagLayer:ShowFairyNum(isWarning)
	if self.fairyNumLabel ~= nil then
		self.fairyNumLabel:setString(getUserFairyNum() .. "/" .. self.TotalFairyBagCap)
	end
	if isWarning and self.TotalFairyBagCap - getUserFairyNum() < 10 then
		GameApi:showMessage("精灵背包空间不足，请清理")
	end
end

function FairyBagLayer:removeOneFairyInBag(posList)
	if posList ~= nil then
		for k,v in pairs(posList) do
			local pos = v
			--CCLuaLog("removeOneFairyInBag: " .. pos)
			if pos ~= nil then
				if self.fairyItemList ~= nil and self.listItemTable[pos] ~= nil then
					self.fairyItemList:removeItem(self.listItemTable[pos].controlBase)
					self.listItemTable[pos] = nil
				end
				for _index,val in pairs(self.fairyOrderedTable) do
					if pos == val.pos then
						self.fairyOrderedTable[_index] = nil
					end
				end
			end
		end
	end
	self:ShowFairyNum(false)
	self:AddFairyData(10)
end

local function onCloseButtonClicked(tag)
	if FairyBagLayer.uiLayerInstance ~= nil then
		FairyBagLayer.uiLayerInstance:removeFromParentAndCleanup(true)
		GameAudioManager:sharedManager():playEffect(350001,false)
	end
end

--local function onHelpButtonClicked(tag)
--
--end
--
--local function onGuildButtonClicked(tag)
--	FairyCollectionLayer:CreateLayer(nil)
--end

local function onRebirthButtonClicked(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	FairySelectLayer:createLayer()
	FairySelectLayer:setRebirthModle()
end

local function onDecomposeButtonClicked(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	FairySelectLayer:createLayer()
	FairySelectLayer:setDecomposeModle()
end

local function onAutoEquipFairyButtonClicked(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	sendCALBattleFairyReq()
end

local function onGetFairyButtonClicked(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	showUILayerByTag(UITagTable["MarketLayer"].tag,true)
	onCloseButtonClicked(nil)
end

local function onBagToggleClicked(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	if FairyBagLayer.fragToggle ~= nil then
		FairyBagLayer.fragToggle:unselected()
	end

	if FairyBagLayer.bagToggle ~= nil then
		FairyBagLayer.bagToggle:selected()
	end

	if FairyBagLayer.bagContainer ~= nil then
		FairyBagLayer.bagContainer:setVisible(true)
	end

	if FairyBagLayer.fragContainer ~= nil then
		FairyBagLayer.fragContainer:setVisible(false)
	end
	FairyBagLayer:InitBagFairyData()
end

local function onFragToggleClicked(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	if FairyBagLayer.bagToggle ~= nil then
		FairyBagLayer.bagToggle:unselected()
	end

	if FairyBagLayer.fragToggle ~= nil then
		FairyBagLayer.fragToggle:selected()
	end

	if FairyBagLayer.bagContainer ~= nil then
		FairyBagLayer.bagContainer:setVisible(false)
	end

	if FairyBagLayer.fragContainer ~= nil then
		FairyBagLayer.fragContainer:setVisible(true)
	end
	FairyBagLayer:InFairyFragData()
end

local function onDecomposeButtonCallBack(tag)
	--CCLuaLog("onDecomposeButtonCallBack	" .. tag)
	--FairyRebirthLayer:createLayer()
	--FairyRebirthLayer:setFairyPos(tag,2)
end

local function onLevelUpButtonCallBack(tag)
	--CCLuaLog("onLevelUpButtonCallBack	" .. tag)
	if FairyBagLayer:checkItemInView(tag) then
		GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
		FairyLevelUpLayer.lastLayer = 2
		FairyLevelUpLayer.fairyPos = tag
		FairyLevelUpLayer:CreateLayer(nil)
		FairyBagLayer:checkNewFairyClick(tag)
	end
end

local function onFairyBackButtonCallBack(tag)
	if FairyBagLayer:checkItemInView(tag)  then
		if FairyBagLayer.lastClickFairyItemTag ~= tag then
			GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
			if FairyBagLayer.listItemTable[FairyBagLayer.lastClickFairyItemTag] ~= nil then
				FairyBagLayer.listItemTable[FairyBagLayer.lastClickFairyItemTag].backItem:unselected()
			end
			FairyBagLayer.lastClickFairyItemTag = tag
		end
		if FairyBagLayer.listItemTable[tag] ~= nil then
			FairyBagLayer.listItemTable[tag].backItem:selected()
		end
		FairyBagLayer:checkNewFairyClick(tag)
	end
end

local function onTrainButtonCallBack(tag)
	--CCLuaLog("onTrainButtonCallBack	" .. tag)
	if FairyBagLayer:checkItemInView(tag) then
		GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
		FairyTrainLayer.lastLayer = 2
		FairyTrainLayer.fairyPos = tag
		FairyTrainLayer:CreateLayer(nil)
		FairyBagLayer:checkNewFairyClick(tag)
	end
end

local function onGradeUpButtonCallBack(tag)
	--CCLuaLog("onGradeUpButtonCallBack " .. tag)
	if FairyBagLayer:checkItemInView(tag) then
		GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
		FairyAdvanceLayer.lastLayer = 2
		FairyAdvanceLayer.fairyPos = tag
		FairyAdvanceLayer:CreateLayer(nil)
		FairyBagLayer:checkNewFairyClick(tag)
	end
end

local function onReBirthUpButtonCallBack(tag)
	--CCLuaLog("onReBirthUpButtonCallBack	" .. tag)
	--FairyRebirthLayer:createLayer()
	--FairyRebirthLayer:setFairyPos(tag,1)
end

local function onFairyIconButtonCallBack(tag)
	if FairyBagLayer:checkItemInView(tag) then
		GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
		FairyInfoLayer.fairyPos = tag
		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 1
		FairyInfoLayer:CreateLayer()
		FairyBagLayer:checkNewFairyClick(tag)
	end
end

local function onFragIconButtonCallBack(tag)
	--CCLuaLog("onFragIconButtonCallBack	" .. tag)
	local needNum,fairyId = getFairyfragExchangeInfo(tag)
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false)
	FairyInfoLayer.fairyId = fairyId
	FairyInfoLayer.buttonState = 0
	FairyInfoLayer.infoState = 2
	FairyInfoLayer:CreateLayer()
end

local function onFragExchangeButtonCallBack(tag)
	--CCLuaLog("onFragExchangeButtonCallBack	" .. tag)
	if FairyBagLayer.fragItemTable[tag] ~= nil then
		OnlineNetworkManager:sShareInstance():sendEquipComposeReq(FairyBagLayer.fragItemTable[tag].exchangeId,tag + kFairyFragment,1) 
	end
end

local function onFragGetwayButtonCallBack(tag)
	if FairyBagLayer.fragItemTable[tag] ~= nil then
		local fragItem = ItemManager:Get():findItemByPos(tag + kFairyFragment)
		if fragItem ~= nil then
			FairyGetwayLayer:createLayer(fragItem:getItemId())
		end
	end
end

function FairyBagLayer:showFairyTipIcon()
	if self.bagToggle ~= nil then
		local hasNewFairy = G_hasNewFairy()
		if hasNewFairy then
			self.bagToggle:showTipIcon()
		else
			self.bagToggle:hideTipIcon()
		end
	end
end

function FairyBagLayer:showFairyFlagTipIcon()
	if self.fragToggle ~= nil then
		local info = UserData:GetUserInfo()
		if info.b_hasEnouchFairyFlag then
			self.fragToggle:showTipIcon()
		else
			self.fragToggle:hideTipIcon()
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyBagLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyBagLayer.uiLayerInstance then
				if FairyBagLayer.uiLayerInstance ~= nil then
					FairyBagLayer.uiLayerInstance:removeFromParentAndCleanup(true)
				end
				ret = 1
			end
		end
	elseif message == GM_ATTR_NEW_FAIRY_INFO then
		if nil ~= FairyBagLayer.uiLayerInstance then
			FairyBagLayer:showFairyTipIcon()
		end
	elseif message == GM_ATTR_FARIY_FLAG_ENOUGH then
		if nil ~= FairyBagLayer.uiLayerInstance then
			--FairyBagLayer:showFairyFlagTipIcon()
		end
	end

	return ret
end

-- 精灵排序
local function sortFairyList(a,b)
	
	--return a.pos < b.pos
	return compareFairyOrder(a,b)
end

function FairyBagLayer:checkItemInView(pos)
	if self.fairyItemList ~= nil and self.listItemTable ~= nil and self.listItemTable[pos] ~= nil then
		return self.fairyItemList:checkChildInView(self.listItemTable[pos].controlBase)
	end
	return false
end

local function onReceiveBagListScrollMax()
	--CCLuaLog("FairyBagLayer:onReceiveBagListScrollMax()")
	FairyBagLayer:AddFairyData(10)
end

function FairyBagLayer:AddFairyData(num)
	--CCLuaLog("FairyBagLayer:AddFairyData()" .. num)
	local index = 0 
	for k,v in pairs(self.fairyOrderedTable) do
		if index < num and self.fairyShowedTable[v.pos] == nil then
			local item = FairyBagListItem:create(self.fairyItemList,v,self.fairyListIndex)
			item:setDecomposeButtonCallBack(onDecomposeButtonCallBack)
			item:setLevelUpButtonCallBack(onLevelUpButtonCallBack)
			item:setTrainButtonCallBack(onTrainButtonCallBack)
			item:setRankUpButtonCallBack(onGradeUpButtonCallBack)
			item:setRebirthMenuItem(onReBirthUpButtonCallBack)
			item:setHeadButtonCallBack(onFairyIconButtonCallBack)
			item:setBackButtonCallBack(onFairyBackButtonCallBack)
			--table.insert(self.listItemTable,item)
			self.listItemTable[v.pos] = item
			self.fairyShowedTable[v.pos] = 1
			self.fairyListIndex = self.fairyListIndex + 1
			index = index + 1
			checkCollectionFairyStatus(v.fairyId)
		end
	end
	local hitLabel = self.uiLayout:FindChildObjectByName("hitLabel").__UILabel__
	if index >= num then
		self.fairyItemList:setCanReceiveScrollCallback(true)
		hitLabel:setVisible(true)
	else
		hitLabel:setVisible(false)
	end
end

function FairyBagLayer:InitBagFairyData()
	--CCLuaLog("FairyBagLayer:InitBagFairyData()")
	local isWarning = false
	if not self.isFairyBagInit then
		self.isFairyBagInit = true
		isWarning = true
		local fairyList = getUserFairyList()
		if fairyList ~= nil then
			self.fairyOrderedTable = {}
			for k,v in pairs(fairyList) do
				table.insert(self.fairyOrderedTable,v)
			end
			table.sort(self.fairyOrderedTable,sortFairyList)
			self.fairyListIndex = 1
			self:AddFairyData(10)
		end
	end
	self:ShowFairyNum(isWarning)
end

-- 精灵碎片比较是否足够
local function checkFairyEnough(fairyFrag)
	local needNum,fairyId,exchangeId = getFairyfragExchangeInfo(fairyFrag:getItemId())
	if fairyFrag.amount >= needNum then
		return 1
	else
		return 0
	end
end

-- 精灵碎片对应精灵的资质
local function checkFairyQuality(fairyFrag)
	local needNum,fairyId,exchangeId = getFairyfragExchangeInfo(fairyFrag:getItemId())
	local fairyBasicInfo = getFairyBasicInfo(fairyId)
	if fairyBasicInfo ~= nil then
		return fairyBasicInfo.quality
	else
		return 0
	end
end

-- 精灵碎片排序
local function fragItemSort(a,b)
	local frag_a = ItemManager:Get():findItemByPos(a)
	local frag_b = ItemManager:Get():findItemByPos(b)
	if frag_a ~= nil and frag_b ~= nil then
		local needNum_a,fairyId_a = getFairyfragExchangeInfo(frag_a:getItemId())
		local needNum_b,fairyId_b = getFairyfragExchangeInfo(frag_b:getItemId())
		if fairyId_a ~= 0 and fairyId_b ~= 0 then
			local isEnough_a = checkFairyEnough(frag_a)
			local isEnough_b = checkFairyEnough(frag_b)
			if isEnough_a ~= isEnough_b then
				return isEnough_a > isEnough_b
			else
				local quality_a = checkFairyQuality(frag_a)
				local quality_b = checkFairyQuality(frag_b)
				if quality_a ~= quality_b then
					return quality_a > quality_b
				else
					if frag_a.amount ~= frag_b.amount then
						return frag_a.amount > frag_b.amount
					else
						return frag_a:getItemId() < frag_b:getItemId()
					end
				end
			end
		else
			return fairyId_a ~= 0
		end
	else
		return frag_a ~= nil
	end
end

function FairyBagLayer:checkNewFairyClick(pos)
	local fairyInfo = getUserFairyByPos(pos)
	if fairyInfo ~= nil and fairyInfo.isNewFlag then
		setUserFairyNewFlag(pos,false)
		if self.listItemTable ~= nil and self.listItemTable[pos] ~= nil then
			self.listItemTable[pos]:showTipIcon(false)
		end
	end
end

function FairyBagLayer:InFairyFragData()
	CCLuaLog("FairyBagLayer:InFairyFragData()")
	if not self.isFairyFragInit then
		self.isFairyFragInit = true
		local uiList = self.uiLayout:FindChildObjectByName("fragItemList").__UIScrollList__
		local control = nil 
		local itemNum = 0
		local fragTable = {}
		for i = 1 , FAIRYFRAGMENTBAGCAPACITY do
			local fragItem = ItemManager:Get():findItemByPos(kFairyFragment + i)
			if not fragItem.isEmpty then
				table.insert(fragTable,kFairyFragment + i)
			end
		end
		table.sort(fragTable,fragItemSort)
		for k,v in pairs(fragTable) do
			itemNum = itemNum + 1
			local fragItem = ItemManager:Get():findItemByPos(v)
			local index = itemNum % 2
			if (control == nil) or (index == 1) then
				local baseNode = CCSprite:create()
				local bgFrame,framePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME")
				control = uiList:AddBaseItem(bgFrame:getContentSize(),baseNode)
			end
			local fragListItem = FairyFragListItem:create(control:getCurrentNode(),fragItem,v)
			self.fragItemTable[fragItem.position] = fragListItem
			if index ~= 1 then
				local firstFrame,firstPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1")
				local secondFrame,secondPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME2")
				fragListItem.baseNode:setPosition(ccpSub(secondPt,firstPt))
			end
			fragListItem:setTipButtonCallBack(onFragIconButtonCallBack)
			fragListItem:setExchangeButtonCallBack(onFragExchangeButtonCallBack)
			fragListItem:setGetwayButtonCallBack(onFragGetwayButtonCallBack)
		end
	end
end

function FairyBagLayer:ResetFragData()
    if self.isFairyFragInit then
        self.isFairyFragInit = false;
        self.fragItemTable = {};
        local uiList = self.uiLayout:FindChildObjectByName("fragItemList").__UIScrollList__
		uiList:clearChildrens()
		uiList:setStartOffset(ccp(0,0))
    end
end

function FairyBagLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	--local helpBt = self.uiLayout:FindChildObjectByName("helpBt").__UIButton__:getMenuItemSprite()
	--helpBt:registerScriptTapHandler(onHelpButtonClicked)
	--local guildBt = self.uiLayout:FindChildObjectByName("guildBt").__UIButton__:getMenuItemSprite()
	--guildBt:registerScriptTapHandler(onGuildButtonClicked)
	local shopBt = self.uiLayout:FindChildObjectByName("shopBt").__UIButton__:getMenuItemSprite()
	shopBt:registerScriptTapHandler(onGetFairyButtonClicked)
	local equipBt = self.uiLayout:FindChildObjectByName("equipBt").__UIButton__:getMenuItemSprite()
	equipBt:registerScriptTapHandler(onAutoEquipFairyButtonClicked)
	local decomposeBt = self.uiLayout:FindChildObjectByName("decomposeBt").__UIButton__:getMenuItemSprite()
	decomposeBt:registerScriptTapHandler(onDecomposeButtonClicked)
	local rebirthBt = self.uiLayout:FindChildObjectByName("rebirthBt").__UIButton__:getMenuItemSprite()
	rebirthBt:registerScriptTapHandler(onRebirthButtonClicked)
	self.bagToggle = self.uiLayout:FindChildObjectByName("bagButtonToggle").__UIButtonToggle__
	self.fragToggle = self.uiLayout:FindChildObjectByName("fragButtonToggle").__UIButtonToggle__
	self.bagToggle:getMenuItemToggle():registerScriptTapHandler(onBagToggleClicked)
	self.fragToggle:getMenuItemToggle():registerScriptTapHandler(onFragToggleClicked)
	self.bagContainer = self.uiLayout:FindChildObjectByName("bagContainer").__UIContainer__
	self.fragContainer = self.uiLayout:FindChildObjectByName("fragContainer").__UIContainer__
	self.fairyItemList = self.uiLayout:FindChildObjectByName("bagItemList").__UIScrollList__
	self.fairyItemList:setScrollMaxHandler(onReceiveBagListScrollMax)
	self.fairyNumLabel = self.uiLayout:FindChildObjectByName("fairyNumLabel").__UILabel__
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	onBagToggleClicked(nil)
	self:showFairyTipIcon()
	--self:showFairyFlagTipIcon()
	SetChatBar(false,-1)
	PushOneLayer(self.uiLayerInstance,"","")
end