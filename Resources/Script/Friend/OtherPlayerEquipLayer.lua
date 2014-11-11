require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/characterString")
require("Script/Friend/PlayerItemCenter")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Friend/OtherPlayerEquipBt")
require("Script/Friend/OtherPlayerAlchemyEquipBt")
require("Script/Friend/OtherPlayerAttrPanel")
require("Script/Friend/OtherPlayerModelPanel")
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairyLuaInterface")
require("Script/Equipment/TipColor")
require("Script/FairyLayer/FairyInfoLayer")


OtherPlayerEquipLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	notificationFunc = 0,
	userId = nil,
	attrPanel = nil,	
	modelPanel = nil,
	getUserInfoFlag = false,
	getUserAttrFlag = false, 
	getUserItemFlag = false,
	getUserFairyFlag = false,
	updateUserFunc = 0,
	equipBtList = {},
	alchemyBtList = {},
	--expBar = nil,
	--expLabel = nil,
	isUserInfoInit = false,
	isFairyInit = false,
	isModelInit = false,
	fairyBtTable = nil,
	fairyTable = nil,
	lastSelectFairyIndex = -1,
	fairyListStartPos = nil,
	selectEquipId = 0,
}

local function onReceiveDestructorHandler()
	RemoveOneLayer(OtherPlayerEquipLayer.uiLayerInstance)
	if OtherPlayerEquipLayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(OtherPlayerEquipLayer.notificationFunc);
	end
	if OtherPlayerEquipLayer.updateUserFunc ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(OtherPlayerEquipLayer.updateUserFunc)
	end
	OtherPlayerEquipLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("otherPlayerEquipUI")
	SetChatBar(true,-1)
end

function OtherPlayerEquipLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["OtherPlayerEquipLayer"].tag
			parentNode:addChild(self.uiLayerInstance,20,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/otherPlayerEquipUI.plist",self.uiLayerInstance, "otherPlayerEquipUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

function OtherPlayerEquipLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.userId = nil
	self.getUserInfoFlag = false
	self.getUserAttrFlag = false
	self.getUserItemFlag = false
	self.getUserFairyFlag = false
	self.updateUserFunc = 0
	self.equipBtList = {}
	self.alchemyBtList = {}
	if self.attrPanel ~= nil then
		self.attrPanel:resetValue()
		self.attrPanel = nil
	end
	self.modelPanel = nil
	--self.expBar = nil
	--self.expLabel = nil
	self.isUserInfoInit = false
	self.isFairyInit = false
	self.isModelInit = false
	self.fairyBtTable = nil
	self.lastSelectFairyIndex = -1
	self.fairyTable = nil
	self.fairyListStartPos = nil
	self.selectEquipId = 0
	PlayerItemCenter:setSelectedPlayer(nil)
end

function OtherPlayerEquipLayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc)
	end

	self:resetValue()
end

function OtherPlayerEquipLayer:setPlayerEquipTitle()
	if self.uiLayout ~= nil then
		local iconTitle = self.uiLayout:FindChildObjectByName("iconTitle").__UIPicture__
		--local playerInfo = getSignleFriendInfo(self.userId)
		local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
		if iconTitle ~= nil and userInfo ~= nil then
			local titleSprite = nil
			if userInfo.current_title ~= 0 then
				local str ="map_ui_title_FRAME_" .. userInfo.current_title
				titleSprite = AspriteManager:getInstance():getOneFrame("UI/ui_title.bin",str)
			end
			iconTitle:setSprite(titleSprite,false)
		end
	end
end

function OtherPlayerEquipLayer:setPlayerName()
	if self.uiLayout ~= nil then
		local nameLabel = self.uiLayout:FindChildObjectByName("playerNameLabel").__UILabel__
		--local playerInfo = getSignleFriendInfo(self.userId)
		local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
		if nameLabel ~= nil and userInfo ~= nil then
			nameLabel:setString(userInfo.szName)
		end
	end
end

function OtherPlayerEquipLayer:setPlayerGuildName()
	if self.uiLayout ~= nil then
		local guildNameLabel = self.uiLayout:FindChildObjectByName("guildNameLabel").__UILabel__
		--local playerInfo = getSignleFriendInfo(self.userId)
		local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
		if guildNameLabel ~= nil and userInfo ~= nil and userInfo.guildName ~= "" then
			local str = string.format(LanguageLocalization:GetLocalization("M_SOCIATY_NAMEMARK"),userInfo.guildName)
			guildNameLabel:setString(str)
		end
	end
end


function OtherPlayerEquipLayer:setPlayerLevel()
	if self.uiLayout ~= nil then
		local nameLabel = self.uiLayout:FindChildObjectByName("playerLevelLabel").__UILabel__
		local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
		--local playerInfo = getSignleFriendInfo(self.userId)
		if userInfo ~= nil then
			if nameLabel ~= nil then
				nameLabel:setString("Lv" .. userInfo.level)
			end

			for i = 1,8 do
				if self.alchemyBtList[i] ~= nil then
					self.alchemyBtList[i]:setUserLevel(userInfo.level)
				end
			end
		end
	end
end

function OtherPlayerEquipLayer:setPlayerVip()
	if self.uiLayout ~= nil then
		local playerVipPic = self.uiLayout:FindChildObjectByName("playerVipPic").__UIPicture__
		local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
		--local playerInfo = getSignleFriendInfo(self.userId)
		if playerVipPic ~= nil and userInfo ~= nil and userInfo.m_vip > 0 then
			local vipName = "map_ui_FRAME_ICON_HEADVIP" .. userInfo.m_vip
			local sprite = AspriteManager:getInstance():getOneFrame(KUI_BIN,vipName)
			playerVipPic:setSprite(sprite,false)
		end
	end
end

function OtherPlayerEquipLayer:setEffectiveness()
	if self.uiLayout ~= nil then
		local playerPowerLabel = self.uiLayout:FindChildObjectByName("playerPowerLabel").__UILabel__
		local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
		if playerPowerLabel ~= nil and userInfo ~= nil then
			playerPowerLabel:setString( LanguageLocalization:GetLocalization("M_BAG_POWERINALL") .. userInfo.effectiveness)
		end
		
	end
end

local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= OtherPlayerEquipLayer.uiLayerInstance then
			if G_CurLayerInstance ~= OtherPlayerEquipLayer.uiLayerInstance then
				OtherPlayerEquipLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function onOtherPlayerEquipTipCallBackLeft()

end

function onOtherPlayerEquipTipCallBackRight()
	if OtherPlayerEquipLayer.selectEquipId ~= 0 then
		G_ShowFairyGetwayLayer(OtherPlayerEquipLayer.selectEquipId)
	end
end

local function checkUserInfo(t)
	--CCLuaLog("checkUserInfo")
	if not OtherPlayerEquipLayer.getUserInfoFlag then
		--CCLuaLog("not OtherPlayerEquipLayer.getUserInfoFlag")
		if OtherPlayerEquipLayer.userId ~= nil then
			local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
			if userInfo ~= nil and userInfo.total_hp ~= 0 then
				OtherPlayerEquipLayer.getUserInfoFlag = true
				OtherPlayerEquipLayer:setUserInfo()
				OtherPlayerEquipLayer.isUserInfoInit = true
			end
		end
	end
	if not OtherPlayerEquipLayer.getUserFairyFlag then
		--CCLuaLog("not OtherPlayerEquipLayer.getUserFairyFlag")
		if OtherPlayerEquipLayer.userId ~= nil then
			local fairyInfo = getOtherPlayerFairyList(OtherPlayerEquipLayer.userId)
			if fairyInfo ~= nil then
				OtherPlayerEquipLayer.getUserFairyFlag = true
				OtherPlayerEquipLayer.fairyTable = fairyInfo
			end
		end
	end
	if not OtherPlayerEquipLayer.isFairyInit then
		--CCLuaLog("not OtherPlayerEquipLayer.isFairyInit")
		OtherPlayerEquipLayer:initFairyList()
	end
	if not OtherPlayerEquipLayer.isModelInit then
		--CCLuaLog("not OtherPlayerEquipLayer.isModelInit")
		OtherPlayerEquipLayer:initModelPage()
	end
	if OtherPlayerEquipLayer.isUserInfoInit and OtherPlayerEquipLayer.isFairyInit and OtherPlayerEquipLayer.isModelInit then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(OtherPlayerEquipLayer.updateUserFunc)
		OtherPlayerEquipLayer.updateUserFunc = 0
	end
end

local function onReceivedPlayerItems()
	if OtherPlayerEquipLayer.uiLayerInstance ~= nil then
		OtherPlayerEquipLayer.getUserItemFlag = true
		--OtherPlayerEquipLayer:showPlayerEquip()
		--OtherPlayerEquipLayer:showModel()
	end
end

local function onReceivedPlayerAttributation()
	if OtherPlayerEquipLayer.uiLayerInstance ~= nil then
		OtherPlayerEquipLayer.getUserAttrFlag = true
		--OtherPlayerEquipLayer:setPlayerExp()
		if OtherPlayerEquipLayer.attrPanel ~= nil then
			OtherPlayerEquipLayer.attrPanel:setEffectiveness(OtherPlayerEquipLayer.userId)
		end
	end
end

local function onCloseBtClicked(tag)
	OtherPlayerEquipLayer:destroyed()
end

local function onAttrBtClicked(tag)
	--CCLuaLog("onAttrBtClicked")
	if OtherPlayerEquipLayer.attrPanel ~= nil then
		OtherPlayerEquipLayer.attrPanel:setVisible(true)
	end
	local page = OtherPlayerEquipLayer:getCurrentPage()
	if page == 4 or page == 3 then
		local pos = FAIRY_POS_START_AST + (page - 3)
		OtherPlayerEquipLayer:showSingleInfoByPos(pos)
	elseif page == 2 or page == 1 then
		pos = FAIRY_POS_START_BTL + (page - 1)
		OtherPlayerEquipLayer:showSingleInfoByPos(pos)
	elseif page == 5 then
		OtherPlayerEquipLayer:showFetterInfo()
	end
end

local function onAttrCloseBtClicked(tag)
--	if OtherPlayerEquipLayer.attrPanel ~= nil then
--		OtherPlayerEquipLayer.attrPanel:setVisible(false)
--	end
	onCloseBtClicked(nil)
end


local function onEquipBtClicked(tag)
	if OtherPlayerEquipLayer.equipBtList[tag] ~= nil then
		OtherPlayerEquipLayer.equipBtList[tag]:ShowTips()
	end
end

local function onAlchemyBtClicked(tag)
	if OtherPlayerEquipLayer.alchemyBtList[tag] ~= nil then
		OtherPlayerEquipLayer.alchemyBtList[tag]:ShowTips()
	end
end

local function onAlchemyLeftBtClicked(tag)
	OtherPlayerEquipLayer:showPlayerAlchemy(false)
end

local function onAlchemyRightBtClicked(tag)
	OtherPlayerEquipLayer:showPlayerAlchemy(true)
end

local function onReceivedPageChanged()
	--CCLuaLog("onReceivedPageChanged")
	local page = OtherPlayerEquipLayer.modelPanel:getCurPage()
	if OtherPlayerEquipLayer.modelPanel ~= nil then
		--CCLuaLog("curPage: " .. OtherPlayerEquipLayer.modelPanel:getCurPage())
		OtherPlayerEquipLayer:setEquipIndex(page)
		OtherPlayerEquipLayer:setAlchemyIndex(page)
		OtherPlayerEquipLayer:setPageInfo(page)
		OtherPlayerEquipLayer:FairyListAutoWithPage(page)
	end

	if page == 0 then
		CallNativeFuncManager:getInstance():SetRankLayerVipFlgVisible(true)
	else
		CallNativeFuncManager:getInstance():SetRankLayerVipFlgVisible(false)
	end
end

local function onFairyListBtClicked(tag)
	if OtherPlayerEquipLayer.lastSelectFairyIndex == tag then
		return
	end
	if OtherPlayerEquipLayer.lastSelectFairyIndex ~= -1 then
		if OtherPlayerEquipLayer.fairyBtTable[OtherPlayerEquipLayer.lastSelectFairyIndex] ~= nil then
			OtherPlayerEquipLayer.fairyBtTable[OtherPlayerEquipLayer.lastSelectFairyIndex]:getMenuItemSprite():unselected()
		end
	end
	OtherPlayerEquipLayer.lastSelectFairyIndex = tag
	if OtherPlayerEquipLayer.fairyBtTable[OtherPlayerEquipLayer.lastSelectFairyIndex] ~= nil then
		OtherPlayerEquipLayer.fairyBtTable[OtherPlayerEquipLayer.lastSelectFairyIndex]:getMenuItemSprite():selected()
	end
	if OtherPlayerEquipLayer.modelPanel ~= nil then
		OtherPlayerEquipLayer.modelPanel:moveToPage(5 - OtherPlayerEquipLayer.lastSelectFairyIndex)
	end
end

function OtherPlayerEquipLayer:showSingleInfoByPos(pos)
	if self.fairyTable ~= nil and self.fairyTable[pos] ~= nil then
		FairyInfoLayer.fairyPos = pos
		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 1
		FairyInfoLayer.playId = self.userId
		FairyInfoLayer:CreateLayer()
	end
end

function OtherPlayerEquipLayer:showFetterInfo()
	FairyContactInfoLayer:createLayer(self.userId)
end

function OtherPlayerEquipLayer:FairyListAutoWithPage(page)
	local uiList = self.uiLayout:FindChildObjectByName("fairyList").__UIScrollList__
	if self.fairyBtTable[5-page] ~= nil then
		local scale = TXGUI.UIManager:sharedManager():getScaleFactor()
		local buttonWidth = self.fairyBtTable[5-page]:getButtonSize().width * scale
		local contentSize = uiList:getContentSize()
		local viewSize = uiList:getViewSize()
		local pt = self.fairyBtTable[5-page]:getPosition()
		local currentOffset = uiList:getContentOffset()
		local btLeftEdge = contentSize.width - viewSize.width * 0.5 + pt.x - buttonWidth *0.5
		local btRightEdge = contentSize.width - viewSize.width * 0.5 + pt.x + buttonWidth *0.5
		local leftEdge = contentSize.width - currentOffset.x - viewSize.width
		local rightEage = contentSize.width - currentOffset.x
		if btLeftEdge < leftEdge then
			local newOffset = ccp(buttonWidth *0.5 - viewSize.width * 0.5 - pt.x,currentOffset.y)
			uiList:setStartOffset(newOffset)
		elseif btRightEdge > rightEage then
			local newOffset = ccp(viewSize.width * 0.5 - buttonWidth *0.5 - pt.x,currentOffset.y)
			uiList:setStartOffset(newOffset)
		end
		if self.lastSelectFairyIndex ~= -1 then
			if self.fairyBtTable[self.lastSelectFairyIndex] ~= nil then
				self.fairyBtTable[self.lastSelectFairyIndex]:getMenuItemSprite():unselected()
			end
		end
		self.lastSelectFairyIndex = 5-page
		if self.fairyBtTable[self.lastSelectFairyIndex] ~= nil then
			self.fairyBtTable[self.lastSelectFairyIndex]:getMenuItemSprite():selected()
		end
	end
end

function OtherPlayerEquipLayer:setUserInfo()
	CCLuaLog("setUserInfo")
	--self:setPlayerExp()
	self:setPlayerName()
	self:setPlayerLevel()
	self:setPlayerVip()
	self:setEffectiveness()
	self:setPlayerEquipTitle()
	self:setPlayerGuildName()
	--self:showModel()
	if self.attrPanel ~= nil then
		self.attrPanel:setUserInfo(self.userId)
	end
end

function OtherPlayerEquipLayer:isFairyPosLocked(pos)
	local level = G_GetFairyPosUnlockLevel(pos)
	local userInfo = UserData:Get():GetUserInfo(self.userId)
	--local playerInfo = getSignleFriendInfo(self.userId)
	if userInfo ~= nil then
		if userInfo.level < level then
			return true
		end
	end
	return false
end

function OtherPlayerEquipLayer:setFairyIcon(index,pos)
	if self:isFairyPosLocked(pos) then
		local lockPic = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_EQUIPMENT_FAIRY_FRAME1_LOCK")
		self:setFairyListIcon(index,lockPic)
	else
		if self.fairyTable ~= nil and self.fairyTable[pos] ~= nil then
			local fairyId = self.fairyTable[pos].fairyId
			local fairyinfo = getFairyBasicInfo(fairyId)
			if fairyinfo ~= nil then
				local fairyIcon = fairyinfo:getFairyIcon()
				self:setFairyListIcon(index,fairyIcon)
			end
		end
	end
end


function OtherPlayerEquipLayer:setFairyListIcon(index,icon)
	if self.fairyBtTable[index] ~= nil and icon ~= nil then
		self.fairyBtTable[index]:setIconSprite(icon)
	end
end

function OtherPlayerEquipLayer:initFairyList()
	if self.getUserInfoFlag and not self.isFairyInit and self.getUserFairyFlag then
		self.isFairyInit = true
		local uiList = self.uiLayout:FindChildObjectByName("fairyList").__UIScrollList__

		-- added by seanxiao at 2014/4/21
		uiList:clearChildrens()

		self.fairyBtTable = {}
		for i =	5,0 , -1 do
			local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_EQUIPMENT_FAIRY_FRAME1")
			local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_EQUIPMENT_FAIRY_FRAME1_CLICKED")
			local button = uiList:AddNewItem(bgFrame,clickedFrame,"","",ccc3(255,255,255),-135)
			local menuItem = button:getMenuItemSprite()
			menuItem:registerScriptTapHandler(onFairyListBtClicked)
			menuItem:setTag(5-i)
			self.fairyBtTable[5-i] = button
		end

		-- uilist pos reset updated by seanxiao at 2014/4/21
		local size = uiList:getContentSize()
		local viewSize = uiList:getViewSize()
		if fairyListStartPos == nil then
			fairyListStartPos = uiList:getContentOffset()
		end
		local offset = ccp(fairyListStartPos.x + size.width - viewSize.width,fairyListStartPos.y)
		uiList:setStartOffset(offset)
		local userInfo = UserData:Get():GetUserInfo(self.userId)
		local userType = 1
		if userInfo ~= nil then
			userType = userInfo.type
		end
		local headPicName = Characters[userType].bagHeadPic
		local headPic = AspriteManager:getInstance():getOneFrame(KICON_BIN,headPicName)
		self:setFairyListIcon(5,headPic)
		self:setFairyIcon(4,FAIRY_POS_START_BTL)
		self:setFairyIcon(3,FAIRY_POS_START_BTL + 1)
		self:setFairyIcon(2,FAIRY_POS_START_AST)
		self:setFairyIcon(1,FAIRY_POS_START_AST + 1)
		local friendIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_FAIRY_FRIEND")
		self:setFairyListIcon(0,friendIcon)
	end
end

function OtherPlayerEquipLayer:initModelPage()
	if self.getUserInfoFlag and not self.isModelInit and self.getUserFairyFlag and self.getUserItemFlag then
		self.isModelInit = true
		local items = getPlayerItems(self.userId)
		if items ~= nil and self.modelPanel ~= nil then
			local weapon = items:getEquipItemByPos(3 + kItemHeroEquip)
			local userInfo = UserData:Get():GetUserInfo(OtherPlayerEquipLayer.userId)
			--local playerInfo = getSignleFriendInfo(self.userId)
			local elfId = items:getElfEquiped()
			if userInfo ~= nil then

				-- updated by seanxiao
				self.uiLayout:FindChildObjectByName("modelScrollPage").__UIScrollPage__:removeAllPages()

				if weapon ~= nil then
					self.modelPanel:createModels(weapon.itemId,userInfo.type,self.fairyTable)
				else
					self.modelPanel:createModels(0,userInfo.type,self.fairyTable)
				end
			end
			self.modelPanel:registerPageChangeCallback(onReceivedPageChanged)
		end
		self:setEquipIndex(0)
		self:setAlchemyIndex(0)
		self:setPageInfo(0)
	end
end

function OtherPlayerEquipLayer:getCurrentPage()
	local page = 0
	if self.modelPanel ~= nil then
		page = self.modelPanel:getCurPage()
	end
	return page
end

function OtherPlayerEquipLayer:setPageInfo(page)
	CCLuaLog("OtherPlayerEquipLayer:setPageInfo: " .. page)
	if page == 5 then
		self:showPlayerInfoVisible(false)
		self:showFairyInfoVisible(false,false,false,false)
		self:showFriendInfoVisible(true)
	elseif page == 4 or page == 3 then
		local pos = FAIRY_POS_START_AST + (page - 3)
		self:showPlayerInfoVisible(false)
		self:showFriendInfoVisible(false)
		local fairyId = 0
		if self.fairyTable ~= nil and self.fairyTable[pos] ~= nil then
			fairyId = self.fairyTable[pos].fairyId
		end
		self:showFairyInfoVisible(true,false,self:isFairyPosLocked(pos),fairyId == 0)
		self:showFairyNameAndLevel(pos)
		self:showFairyFetters(pos,fairyId)
	elseif page == 2 or page == 1 then
		local pos = FAIRY_POS_START_BTL + (page - 1)
		self:showPlayerInfoVisible(false)
		self:showFriendInfoVisible(false)
		local fairyId = 0
		if self.fairyTable ~= nil and self.fairyTable[pos] ~= nil then
			fairyId = self.fairyTable[pos].fairyId
		end
		self:showFairyInfoVisible(true,true,self:isFairyPosLocked(pos),fairyId == 0)
		self:showFairyNameAndLevel(pos)
		self:showFairyFetters(pos,fairyId)
	else
		self:showPlayerInfoVisible(true)
		self:showFairyInfoVisible(false,false,false,false)
		self:showFriendInfoVisible(false)
	end
end

function OtherPlayerEquipLayer:showFairyNameAndLevel(pos)
	local name = ""
	local levelStr = ""
	local effectivenessStr = ""
	local quality = 0
	local qualityStr = ""
	if self.fairyTable ~= nil and self.fairyTable[pos] ~= nil then
		local fairyId = self.fairyTable[pos].fairyId
		local grade = self.fairyTable[pos].fairyGrade
		local effectiveness = self.fairyTable[pos].fairyEffectiveness
		local fairyinfo = getFairyBasicInfo(fairyId)
		effectivenessStr = LanguageLocalization:GetLocalization("M_BAG_POWERSUPPLY") .. effectiveness
		local level = self.fairyTable[pos].fairyLevel
		levelStr = "Lv." .. level
		if fairyinfo ~= nil then
			local fairyName = fairyinfo.name
			quality = fairyinfo.quality
			qualityStr = LanguageLocalization:GetLocalization("M_FAIRY_QUALITY") .. " " .. quality
			if grade > 0 then
				name = fairyName .. "+" .. grade
			else
				name = fairyName
			end
		end
	end

	local nameLabel = self.uiLayout:FindChildObjectByName("fairyNameLabel").__UILabel__
	nameLabel:setString(name)
	nameLabel:setColor(ItemManager:Get():getFairyLabelColorByQuality(quality))
	
	local levelLabel = self.uiLayout:FindChildObjectByName("fairyLevelLabel").__UILabel__
	levelLabel:setString(levelStr)

	local fairyPowerLabel = self.uiLayout:FindChildObjectByName("fairyPowerLabel").__UILabel__
	fairyPowerLabel:setString(effectivenessStr)

	local fairyQualityLabel = self.uiLayout:FindChildObjectByName("fairyQualityLabel").__UILabel__
	fairyQualityLabel:setString(qualityStr)
end

function OtherPlayerEquipLayer:showFairyFetters(pos,fairyId)
	local chemicalTextLabel = self.uiLayout:FindChildObjectByName("chemicalTextList").__UIScrollList__
	if chemicalTextLabel ~= nil then
		chemicalTextLabel:clearChildrens()
		for i=1 ,6 do
			local fetterId = G_GetFairyFetterIdById(fairyId,7-i)
			if fetterId ~= 0 then
				local name = G_GetFetterNameByFetterId(fetterId)
				local sprite = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_TEXT_CHEMICAL1")
				local label = TXGUI.UILabelTTF:create(name,KJLinXin,16,sprite:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
				local isActivation = G_GetFairyFetterIsActivation(pos,fetterId,self.userId)
				if isActivation then
					label:setColor(TipColorOrange)
				else
					label:setColor(TipColorGray)
				end
				chemicalTextLabel:AddBaseItem(label:getContentSize(),label)
			end
		end
		local size = chemicalTextLabel:getContentSize()
		local viewSize = chemicalTextLabel:getViewSize()
		local startPoint = ccp(0,0)
		local offset = ccp(startPoint.x + size.width - viewSize.width,startPoint.y)
		chemicalTextLabel:setStartOffset(offset)
	end
	--for i=1 ,5 do
	--	local chemicalLabelName = "chemicalTextLabel" .. i
	--	local chemicalTextLabel = self.uiLayout:FindChildObjectByName(chemicalLabelName).__UILabel__
	--	local fetterId = G_GetFairyFetterIdById(fairyId,i)
	--	if fetterId ~= 0 then
	--		local name = G_GetFetterNameByFetterId(fetterId)
	--		chemicalTextLabel:setString(name)
	--		local isActivation = G_GetFairyFetterIsActivation(pos,fetterId,self.userId)
	--		if isActivation then
	--			chemicalTextLabel:setColor(TipColorOrange)
	--		else
	--			chemicalTextLabel:setColor(TipColorGray)
	--		end
	--	else
	--		chemicalTextLabel:setString("")
	--	end
	--end
end

function OtherPlayerEquipLayer:showFairyInfoVisible(isVisible,isFight,isLock,isEmpty)
	local nameLabel = self.uiLayout:FindChildObjectByName("fairyNameLabel").__UILabel__
	nameLabel:setVisible(isVisible and (not isLock))
	local levelLabel = self.uiLayout:FindChildObjectByName("fairyLevelLabel").__UILabel__
	levelLabel:setVisible(isVisible and (not isLock))
	local fightPic = self.uiLayout:FindChildObjectByName("fairyfightIcon").__UIPicture__
	fightPic:setVisible(isVisible and isFight)
	local helpPic = self.uiLayout:FindChildObjectByName("fairyhelpIcon").__UIPicture__
	helpPic:setVisible(isVisible and (not isFight))
	local lockPic = self.uiLayout:FindChildObjectByName("fairyLockIcon").__UIPicture__
	lockPic:setVisible(isVisible and isLock)
	local fairyNonePic = self.uiLayout:FindChildObjectByName("fairyNonePic").__UIPicture__
	fairyNonePic:setVisible(isVisible and isEmpty and (not isLock))
	local fairyFetterContainer = self.uiLayout:FindChildObjectByName("chemicalContainer").__UIContainer__
	fairyFetterContainer:setVisible(isVisible and (not isLock) and (not isEmpty))
	local fairyPowerLabel = self.uiLayout:FindChildObjectByName("fairyPowerLabel").__UILabel__
	fairyPowerLabel:setVisible(isVisible and (not isLock) and (not isEmpty))
	local fairyQualityLabel = self.uiLayout:FindChildObjectByName("fairyQualityLabel").__UILabel__
	fairyQualityLabel:setVisible(isVisible and (not isLock) and (not isEmpty))
end

function OtherPlayerEquipLayer:showPlayerInfoVisible(isVisible)
	local nameLabel = self.uiLayout:FindChildObjectByName("playerNameLabel").__UILabel__
	nameLabel:setVisible(isVisible)
	local levelLabel = self.uiLayout:FindChildObjectByName("playerLevelLabel").__UILabel__
	levelLabel:setVisible(isVisible)
	local playerPowerLabel = self.uiLayout:FindChildObjectByName("playerPowerLabel").__UILabel__
	playerPowerLabel:setVisible(isVisible)
	local playerVipPic = self.uiLayout:FindChildObjectByName("playerVipPic").__UIPicture__
	playerVipPic:setVisible(isVisible)
	local iconTitle = self.uiLayout:FindChildObjectByName("iconTitle").__UIPicture__
	iconTitle:setVisible(isVisible)
	local guildNameLabel = self.uiLayout:FindChildObjectByName("guildNameLabel").__UILabel__
	guildNameLabel:setVisible(isVisible)
end

function OtherPlayerEquipLayer:showFriendInfoVisible(isVisible)
	local nameLabel = self.uiLayout:FindChildObjectByName("fairyFriendLabel").__UILabel__
	nameLabel:setVisible(isVisible)
	local pic = self.uiLayout:FindChildObjectByName("fairyBgPic").__UIPicture__
	pic:setVisible(isVisible)
	local alchemyControlContainer = self.uiLayout:FindChildObjectByName("alchemyControlContainer").__UIContainer__
	alchemyControlContainer:setVisible(not isVisible)
	
	if isVisible then
		self:showPlayerAlchemy(false)
	end
end

function OtherPlayerEquipLayer:setAlchemyIndex(pageIndex)
	if pageIndex < 5 then
		local equipStartPos = kAlchemyHeroEquip + pageIndex * 100
		local items = getPlayerItems(self.userId)
		if items ~= nil then
			for i = 1,8 do
				local item = items:getEquipItemByPos(i + equipStartPos)
				if self.alchemyBtList[i] ~= nil then
					self.alchemyBtList[i]:setItem(item)
				end
			end
		end
	end
end

function OtherPlayerEquipLayer:setEquipIndex(pageIndex)
	CCLuaLog("OtherPlayerEquipLayer:setEquipIndex: " .. pageIndex)
	if pageIndex < 5 then
		local equipStartPos = kItemHeroEquip + pageIndex * 100
		local items = getPlayerItems(self.userId)
		if items ~= nil then
			for i = 1,6 do
				local item = items:getEquipItemByPos(i + equipStartPos)
				if self.equipBtList[i] ~= nil then
					self.equipBtList[i]:setItem(item)
				end
			end
		end
	elseif pageIndex == 5 then
		if self.fairyTable ~= nil then
			for i = 1,6 do
				--local item = items:getEquipItemByPos(i + equipStartPos)
				if self.equipBtList[i] ~= nil then
					self.equipBtList[i]:setFairyFriend(self:isFairyPosLocked(i+FAIRY_POS_START_PRT -1),self.fairyTable[i + FAIRY_POS_START_PRT -1])
				end
			end
		end
	end
end

--function OtherPlayerEquipLayer:setPlayerExp()
--	if self.getUserInfoFlag and self.getUserAttrFlag then
--		local percent = 1.0
--	
--		local userInfo = UserData:Get():GetUserInfo(self.userId)
--		local items = getPlayerItems(self.userId)
--		if userInfo ~= nil and items ~= nil then
--			if userInfo.nextExp ~= 0 then
--				percent = (1.0 * items.exp) / userInfo.nextExp
--			end
--			if self.expLabel ~= nil then
--				self.expLabel:setString(string.format("exp %.2f%%",percent * 100))
--			end
--
--			if self.expBar ~= nil then
--				local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
--				self.expBar:setScaleX(scale * percent)
--				self.expBar:setScaleY(scale)
--			end
--		end
--	end
--end

function OtherPlayerEquipLayer:initAlchemyButton()
	if self.uiLayout ~= nil then
		for i =1,8 do
			local grid = self.uiLayout:FindChildObjectByName("alchemyGrid" .. i).__UIButton__
			if grid ~= nil then
				local normalSprite = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_FRAME_SOUL")
				local clickedSprite = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_EQUIPMENT_FRAME_SOULCLICK")
				local scale = grid:getMenuItemSprite():getContentSize().width / normalSprite:getContentSize().width
				normalSprite:setScale(scale)
				normalSprite:setContentSize(CCSizeMake(normalSprite:getContentSize().width * scale,normalSprite:getContentSize().height * scale))
				clickedSprite:setScale(scale)
				clickedSprite:setContentSize(CCSizeMake(clickedSprite:getContentSize().width * scale,clickedSprite:getContentSize().height * scale))
				grid:getMenuItemSprite():setNormalImage(normalSprite)
				grid:getMenuItemSprite():setSelectedImage(clickedSprite)
			end
		end
	end
end

function OtherPlayerEquipLayer:showPlayerAlchemy(isAlchemyVisible)
	if self.uiLayout ~= nil then
		local alchemyContainer = self.uiLayout:FindChildObjectByName("alchemyContainer").__UIContainer__
		local equipItemContainer = self.uiLayout:FindChildObjectByName("equipItemContainer").__UIContainer__
		local alchemyLeftBt = self.uiLayout:FindChildObjectByName("alchemyLeftBt").__UIButton__
		local alchemyRightBt = self.uiLayout:FindChildObjectByName("alchemyRightBt").__UIButton__
		local alchemyLabel = self.uiLayout:FindChildObjectByName("alchemyLabel").__UILabel__
		if alchemyContainer ~= nil then
			alchemyContainer:setVisible(isAlchemyVisible)
		end
		if equipItemContainer ~= nil then
			equipItemContainer:setVisible(not isAlchemyVisible)
		end
		if alchemyLeftBt ~= nil then
			alchemyLeftBt:setEnabled(isAlchemyVisible)
		end
		if alchemyRightBt ~= nil then
			alchemyRightBt:setEnabled(not isAlchemyVisible)
		end
		if alchemyLabel ~= nil then
			if isAlchemyVisible then
				alchemyLabel:setString(LanguageLocalization:GetLocalization("M_ALCHEMY_SOUL"))
			else
				alchemyLabel:setString(LanguageLocalization:GetLocalization("M_FAIRY_EQUIPON"))
			end
		end
	end
end

function OtherPlayerEquipLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseBtClicked)
	local fairyInfoBt = self.uiLayout:FindChildObjectByName("fairyInfoBt").__UIButton__:getMenuItemSprite()
	fairyInfoBt:registerScriptTapHandler(onAttrBtClicked)
	local alchemyLeftBt = self.uiLayout:FindChildObjectByName("alchemyLeftBt").__UIButton__:getMenuItemSprite()
	alchemyLeftBt:registerScriptTapHandler(onAlchemyLeftBtClicked)
	local alchemyRightBt = self.uiLayout:FindChildObjectByName("alchemyRightBt").__UIButton__:getMenuItemSprite()
	alchemyRightBt:registerScriptTapHandler(onAlchemyRightBtClicked)

	--local attrBt = self.uiLayout:FindChildObjectByName("playerAttributeBt").__UIButton__:getMenuItemSprite()
	--attrBt:registerScriptTapHandler(onAttrBtClicked)
	--local attrCloseBt = self.uiLayout:FindChildObjectByName("attributeCloseBt").__UIButton__:getMenuItemSprite()
	--attrCloseBt:registerScriptTapHandler(onAttrCloseBtClicked)
	--self.expLabel = self.uiLayout:FindChildObjectByName("equipExpLabel").__UILabel__
	--local expPic = self.uiLayout:FindChildObjectByName("equipmentExpBarPic").__UIPicture__
	--if expPic ~= nil then
	--	self.expBar = expPic:getCurrentNode();
	--	local ptx,pty = self.expBar:getPosition()
	--	self.expBar:setAnchorPoint(ccp(0,0.5))
	--	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	--	self.expBar:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * self.expBar:getContentSize().width * 0.5,0)))
	--	self.expBar:setScaleX(0)
	--end
	self.userId = PlayerItemCenter:getSelectedPlayer()
	if self.userId ~= nil then
		resetOtherPlayerFairys(self.userId)
		OtherPlayerEquipLayer:SendMsg(self.userId:getID(),self.userId:getRegTime(),self.userId:getChannel())
	end
	
	self.getUserInfoFlag = false
	if self.updateUserFunc == 0 then
		self.updateUserFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(checkUserInfo, 0.01, false)
	end
	if self.notificationFunc == 0 then
		self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	end
	for i = 1,6 do
		local equipBt = OtherPlayerEquipBt:create()
		equipBt:init(self.uiLayout,i)
		equipBt:registerBtClickedCallback(onEquipBtClicked)
		self.equipBtList[i] = equipBt
	end

	for i = 1,8 do
		local alchemyBt = OtherPlayerAlchemyEquipBt:create()
		alchemyBt:init(self.uiLayout,i)
		alchemyBt:registerBtClickedCallback(onAlchemyBtClicked)
		self.alchemyBtList[i] = alchemyBt
	end

	local uiList = self.uiLayout:FindChildObjectByName("attributeList").__UIScrollList__
	local attrContainer = self.uiLayout:FindChildObjectByName("attributeContainer").__UIContainer__
	--local playerPic = self.uiLayout:FindChildObjectByName("attributeMC").__UIPicture__
	local fairyNonePic = self.uiLayout:FindChildObjectByName("fairyNonePic").__UIPicture__
	local sprite = CCSprite:create("UI/ui_fairy_lock.png")
	if sprite ~= nil then
		fairyNonePic:setSprite(sprite,false)
		fairyNonePic:setVisible(false)
	end
	local fairyBgPic = self.uiLayout:FindChildObjectByName("fairyBgPic").__UIPicture__
	sprite = CCSprite:create("UI/ui_fairy_friend.png")
	if sprite ~= nil then
		fairyBgPic:setSprite(sprite,false)
		fairyBgPic:setVisible(false)
	end

	self.attrPanel = OtherPlayerAttrPanel:create()
	self.attrPanel:init(uiList,attrContainer)
	self.modelPanel = OtherPlayerModelPanel:create()
	self.modelPanel:init(self.uiLayout)
	onAttrBtClicked()
	self:initAlchemyButton()
	self:showPlayerAlchemy(false)

	PushOneLayer(self.uiLayerInstance, "", "")
end

function OtherPlayerEquipLayer:SendMsg(userId, regTime, channelId)
	UserData:Get():removeOtherUser(PlayerIdMake(userId, regTime, channelId))
	setGetUserItemCallback(onReceivedPlayerItems)
	setGetUserAttrCallback(onReceivedPlayerAttributation)
	sendGetUserItemInfo(userId, regTime, channelId)
	sendCSGetUserAttributesReq(userId, regTime, channelId)
	sendCSGetUserInfoReq(userId, regTime, channelId)	
	sendGetFairyListReq(userId,regTime,channelId)
end






------------------------------------downcode added by seanxiao---------------------
function callCreateFunc()
	CCLuaLog("OtherPlayerEquipLayer:CreateLayerForRankLayer()!");
	OtherPlayerEquipLayer:CreateLayer()
end

-- get index player info
function setPlayerIDInfo(userId, regTime, channelId)
	OtherPlayerEquipLayer:GetPlayerIDInfo(userId, regTime, channelId)
end

function OtherPlayerEquipLayer:GetPlayerIDInfo(userId, regTime, channelId)

	-- second solution
	self.getUserInfoFlag = false
	self.getUserFairyFlag = false
	self.isFairyInit = false
	self.isModelInit = false
	if OtherPlayerEquipLayer.updateUserFunc == 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(OtherPlayerEquipLayer.updateUserFunc)
		OtherPlayerEquipLayer.updateUserFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(checkUserInfo, 0.01, false)
	end
	
	-- get user info
	PlayerItemCenter:setSelectedPlayer(PlayerIdMake(userId, regTime, channelId))
	self.userId = PlayerItemCenter:getSelectedPlayer()
	OtherPlayerEquipLayer:SendMsg(userId, regTime, channelId)
end