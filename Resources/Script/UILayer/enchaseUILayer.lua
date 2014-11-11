require("Script/ChatToolLayer")
require("Script/CommonDefine")
require("Script/UILayer/EnchaseGemStoneTable")
require("Script/UILayer/enchaseExchangeUILayer")
require("Script/UILayer/enchaseRecastContainer")
require("Script/GameConfig/ExchangeParameter")
require("Script/GameConfig/uiTagDefine")
require("Script/HelpUI/HelpManager")
require("Script/Fairy/FairyDataCenter")
require("Script/UILayer/PayHintLayer")

EnchaseUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	exchangeUILayer = nil,
	recastContainer = nil,
	notificationFunc = 0,
	equipBtnList = {},
	selectedEquipStones = {},
	--heroEquipToggle = nil,
	--fairyEquipToggle = nil,
	selectedEquipIndex = 0,
	selectedStoneIndex = 0,
	selectEquipFlag = -1,

	diamondLabel = nil,
	creamLabel = nil,
	equipToggleTable ={},
	--lastClickToggleIndex = -1,
}

EnchaseEquipBtn = {
	equipNode = nil,
	button = nil,
	pic = nil,
	nameLabel = nil,
	levelLabel = nil,
	smallArmoryList = {},
}

EnchaseSmallArmoryIcon = {
}

function EnchaseSmallArmoryIcon:createEnchaseSmallArmoryIcon(extension)
	local enchasesmallArmoryIcn = setmetatable(extension or {},self);
	self.__index = self;
	return enchasesmallArmoryIcn;
end

function EnchaseEquipBtn:createEnchaseEquipBtn(extension)
	--CCLuaLog("-------------------create new EnchaseEquipBtn---------------");
	local enchaseEquipBtn = setmetatable(extension or {},self);
	self.__index = self;
	return enchaseEquipBtn;
end

local function onReceiveDestructorHandler()
	RemoveOneLayer(EnchaseUILayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("enchaseUI")
	if EnchaseUILayer.exchangeUILayer ~= nil then
		EnchaseUILayer.exchangeUILayer:destory()
	end
	if	EnchaseUILayer.recastContainer ~= nil then
		EnchaseUILayer.recastContainer:destory()
	end
	if EnchaseUILayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(EnchaseUILayer.notificationFunc)
	end
	EnchaseUILayer:resetValue()
end

function EnchaseUILayer:CreateLayer()
	CCLuaLog("EnchaseUILayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["EnchaseUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/enchaseUI.plist",self.uiLayerInstance, "enchaseUI", true)
			self:InitLayer();
		end
	end

	return self.uiLayerInstance
end

function EnchaseUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.exchangeUILayer = nil
	self.recastContainer = nil
	self.equipBtnList = {}
	self.selectedEquipStones = {}
	self.notificationFunc = 0
	self.heroEquipToggle = nil
	self.fairyEquipToggle = nil
	self.selectedEquipIndex = 0
	self.selectedStoneIndex = 0
	self.selectEquipFlag = -1

	self.diamondLabel = nil
	self.creamLabel = nil
	self.equipToggleTable = nil
	--self.lastClickToggleIndex = -1
end

function EnchaseUILayer:RemoveLayer()
	--CCLuaLog("EnchaseUILayer:RemoveLayer()!");
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end
	SetChatBar(true,-1)
end

local function getRecastCostNum(exchangeId)
	local str = ExchangeParameter[exchangeId]["FromItems"]
	local beganPos = string.find(str,"/") + 1
	local endPos = string.len(str)
	local num = string.sub(str,beganPos,endPos)
	if num == nil then
		num = 0
	end
	return num
end

function EnchaseUILayer:getSelectEquipPos()
	local equipPos = self.selectedEquipIndex + kItemEnchaseEquip + self.selectEquipFlag * 100
	return equipPos
end

function EnchaseUILayer:showRecastSpend()
	local userInfo = UserData:GetUserInfo();
	if userInfo.m_gemRecastTimes > 0 then
		self.recastContainer:setFreeCastTime(userInfo.m_gemRecastTimes)
	else
		local lockNum = 0;
		self.selectedStoneIndex = index
		local equipPos = self:getSelectEquipPos()
		for i = 1,4 do
			local isLock = EquipEnchaseLockTable:getEquipEnchaseLock(equipPos,i)
			if isLock and (self.selectedEquipStones[i].gemId ~= 0) then
				lockNum = lockNum + 1
			end
		end
		local castedGemNum = 0
		local castedDiamond = 0
		if lockNum == 0 then
			castedDiamond = getRecastCostNum(20790)
			castedGemNum = getRecastCostNum(20791)
		elseif lockNum == 1 then
			castedDiamond = getRecastCostNum(20800)
			castedGemNum = getRecastCostNum(20801)	
		elseif lockNum == 2 then
			castedDiamond = getRecastCostNum(20900)
			castedGemNum = getRecastCostNum(20901)
		else
			castedDiamond = getRecastCostNum(21000)
			castedGemNum = getRecastCostNum(21001)		
		end
        SetRequireDiamondCount(castedDiamond)
		self.recastContainer:setRecastCost(castedDiamond,castedGemNum)
	end
end

function EnchaseUILayer:showEquipmentStoneInfo(index,isKeepingLock)
	local equipPos = index + kItemEnchaseEquip + self.selectEquipFlag * 100
	--CCLuaLog("EnchaseUILayer:showEquipmentStoneInfo: " .. equipPos)
	local backItem = ItemManager:Get():findItemByPos(equipPos)
	if backItem ~= nil then
		if not backItem.isEmpty then
			if backItem.m_equipInfo ~= nil then
				local equipHoles = backItem.m_equipInfo:getEquipHole()
				if equipHoles ~= nil then
					for i = 1,4 do 
						if equipHoles[i] ~= nil then
							if equipHoles[i].item_gem_id == 0 then
								EnchaseUILayer.selectedEquipStones[i]:unselectItem(true)
							else
								local isLock = EquipEnchaseLockTable:getEquipEnchaseLock(equipPos,i)
								EnchaseUILayer.selectedEquipStones[i]:setGem(equipHoles[i].item_gem_id,equipHoles[i].hole_index,equipHoles[i].attr_key,equipHoles[i].attr_value,equipHoles[i].min_value,equipHoles[i].max_value)
								EnchaseUILayer.selectedEquipStones[i]:changeLockState(isLock)
							end
						else
							EnchaseUILayer.selectedEquipStones[i]:unselectItem(false)
						end
					end
				--else
				--	CCLuaLog("showEquipmentStoneInfo: " .. index .. " ,equipHoles is nil")
				end
			--else
			--	CCLuaLog("showEquipmentStoneInfo: " .. index .. " ,m_equipInfo is nil")
			end
		else
			for i = 1,4 do 
				EnchaseUILayer.selectedEquipStones[i]:unselectItem(false)
			end
		end
	end
	self:showRecastSpend()
end

function EnchaseUILayer:setChoosedEquipment(index)
	--CCLuaLog("EnchaseUILayer:setChoosedEquipment: " .. index .. ", selectedEquipIndex: " .. self.selectedEquipIndex)
	if self.selectedEquipIndex == index then
		if self.equipBtnList[index] ~= nil and self.equipBtnList[index].button ~= nil then
			self.equipBtnList[index].button:getMenuItemSprite():selected()
		end
		return
	end

	if self.equipBtnList[self.selectedEquipIndex] ~= nil and self.equipBtnList[self.selectedEquipIndex].button ~= nil then
		self.equipBtnList[self.selectedEquipIndex].button:getMenuItemSprite():unselected()
	end
	if self.equipBtnList[index] ~= nil and self.equipBtnList[index].button ~= nil then
		self.equipBtnList[index].button:getMenuItemSprite():selected()
	end

	self.selectedEquipIndex = index
	self:showEquipmentStoneInfo(index,false)
end

function EnchaseUILayer:checkEquipHoleExist(equipPos,index)
	local hasHole = 0;
	local backItem = ItemManager:Get():findItemByPos(equipPos)
	if backItem ~= nil then
		if (backItem.isLocked == false) and (backItem.isEmpty == false) then
			if backItem.m_equipInfo ~= nil then
				local equipHoles = backItem.m_equipInfo:getEquipHole()
				if equipHoles ~= nil and equipHoles[index] ~= nil then
					if  equipHoles[index].item_gem_id == 0 then
						hasHole = 1; -- 装备孔存在，但无宝石
					else	
						hasHole = 2; -- 装备孔存大，但已经有宝石
					end
				end
			end
		end
	end
	--CCLuaLog("EnchaseUILayer:checkEquipHoleExist: " .. equipPos .. "," .. hasHole)
	return hasHole
end

local function confirmToInlay(obj)
	local equipPos = EnchaseUILayer:getSelectEquipPos()
	EnchaseUILayer.exchangeUILayer:show(true,equipPos,EnchaseUILayer.selectedStoneIndex)
end

local function confirmToUnlockEquipHole(obj)
    CheckPayHint()
	local equipPos = EnchaseUILayer:getSelectEquipPos()

	local exchangeId = 40080 + 10 * EnchaseUILayer.selectedStoneIndex

    
	GemStonesFromLua:getInstance():GemHoleUnlockReq(exchangeId,equipPos)
end

function EnchaseUILayer:showDiamondLabel()
	if self.diamondLabel ~= nil then
		local info = UserData:GetUserInfo()
		local diamond = info.m_diamond
		self.diamondLabel:setString(diamond)
	end
end

function EnchaseUILayer:showCreamLabel()
	if EnchaseUILayer.creamLabel ~= nil then
		local info = UserData:GetUserInfo()
		local anima = info.m_playerGemAnima
		EnchaseUILayer.creamLabel:setString(anima)
	end
end

function EnchaseUILayer:reqUnlockHole()
	
	local equipPos = self:getSelectEquipPos()
	-- 计算解锁需要钻石用于显示，
	--CCLuaLog("reqUnlockHole: " .. equipPos)
	local backItem = ItemManager:Get():findItemByPos(equipPos)
	if backItem ~= nil then
		if backItem.m_equipInfo ~= nil then
			local equipHoles = backItem.m_equipInfo:getEquipHole()
			if equipHoles ~= nil then
				local diamond = 0
				for i = 1,self.selectedStoneIndex do
					if equipHoles[i] == nil then
						local exchangeId = 40080 + 10 * i
						local str = ExchangeParameter[exchangeId]["FromItems"]
						local beganPos = string.find(str,"/") + 1
						local endPos = string.len(str)
						local num = string.sub(str,beganPos,endPos)
						if num == nil then
							num = 0
						end
						diamond = diamond + num
					end
				end
				MessageBox:Show(string.format("插槽已锁，是否花%d钻提前解锁",diamond),confirmToUnlockEquipHole,nil,MB_YESNO,ccc3(255,255,255),26)
                SetRequireDiamondCount(diamond)
			end
		end
	end
end

function EnchaseUILayer:oneStoneLockClicked(index)
	local equipPos = self:getSelectEquipPos()
	local hasHole = self:checkEquipHoleExist(equipPos,index)
	if hasHole == 2 then
		local isLock = EquipEnchaseLockTable:getEquipEnchaseLock(equipPos,index)
		isLock = not isLock
		EquipEnchaseLockTable:setEquipEnchaseLock(equipPos,index,isLock)
		self.selectedEquipStones[index]:changeLockState(isLock)
		self:showRecastSpend()
	end
end

function EnchaseUILayer:oneStoneClicked(index)
	self.selectedStoneIndex = index
	local equipPos = self:getSelectEquipPos()
	local hasHole = self:checkEquipHoleExist(equipPos,self.selectedStoneIndex)
	if hasHole == 1 then
		self.exchangeUILayer:show(true,equipPos,self.selectedStoneIndex)
	elseif hasHole == 2 then
		MessageBox:Show("已有宝石存在，是否要覆盖原有宝石",confirmToInlay,nil,MB_YESNO,ccc3(255,255,255),26)
	else
		--GameApi:showMessage("插槽已锁");
		self:reqUnlockHole();
	end
	
end

local function closeEnchaseLayerBt(obj)
	--CCLuaLog("EnchaseUILayer:closeEnchaseLayerBt()!");
	RemoveOneLayer(EnchaseUILayer.uiLayerInstance)
	EnchaseUILayer:RemoveLayer();
end

local function exchageCreamBtClicked(obj)
	EnchaseUILayer.exchangeUILayer:show(false,0,0)
end

local function underGroundBtClicked(obj)
	InstanceManager:Get():onEnterSpecialEntrance(ENTERANCE_UNDERGROUND)
end

local function oneEquipmentClicked(tag)
	EnchaseUILayer:setChoosedEquipment(tag)
end

local function onGemStoneIconClicked(tag)
	EnchaseUILayer:oneStoneClicked(tag)
end

local function onGemStoneLockClicked(tag)
	EnchaseUILayer:oneStoneLockClicked(tag)
end

local function onRecastBtnClicked(tag)
	local equipPos = EnchaseUILayer:getSelectEquipPos()
	local holes = {};
	for i = 1,4 do
		local isLock = EquipEnchaseLockTable:getEquipEnchaseLock(equipPos,i)
		if (not isLock) and (EnchaseUILayer.selectedEquipStones[i].gemId ~= 0) then
			holes[i] = i;
		else
			holes[i] = 0;
		end
	end
	local spenndType = EnchaseUILayer.recastContainer.spendFlag;
    CheckPayHint()
	GemStonesFromLua:getInstance():GemRecastReq(equipPos,holes[1],holes[2],holes[3],holes[4],spenndType)
end

local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= EnchaseUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= EnchaseUILayer.uiLayerInstance then
				closeEnchaseLayerBt(nil)
				ret = 1;
			end
		end
	elseif message == GM_ATTR_SP_DIAMOND_UPDATE then
		EnchaseUILayer:showDiamondLabel()
	elseif message == GM_ATTR_PLAYER_GEM_ANIMA then
		--CCLuaLog("message == GM_ATTR_PLAYER_GEM_ANIMA")
		EnchaseUILayer:showCreamLabel()
	end

	return ret;
end

local function showEquipName(index,name,color)
	if(EnchaseUILayer.equipBtnList[index] ~= nil) and (EnchaseUILayer.equipBtnList[index].nameLabel ~= nil) then
		EnchaseUILayer.equipBtnList[index].nameLabel:setString(name)
		if color ~= nil then
			EnchaseUILayer.equipBtnList[index].nameLabel:setColor(color)
		end
	end
end

local function showEquipPicture(index,sprite,frame)
	if(EnchaseUILayer.equipBtnList[index] ~= nil) and (EnchaseUILayer.equipBtnList[index].pic ~= nil) then
		EnchaseUILayer.equipBtnList[index].pic:setSprite(sprite,false)
		if frame ~= nil then
			EnchaseUILayer.equipBtnList[index].pic:addExtraPic(frame,ccp(0,0))
		end
	end
end

local function showEquipLevel(index,level,color)
	if(EnchaseUILayer.equipBtnList[index] ~= nil) and (EnchaseUILayer.equipBtnList[index].levelLabel ~= nil) then
		EnchaseUILayer.equipBtnList[index].levelLabel:setString(level)
		if color ~= nil then
			EnchaseUILayer.equipBtnList[index].levelLabel:setColor(color)
		end
	end
end

local function getSmallArmoryIconSprite(id)
	local iconName = nil
	if id == 417001 then
		iconName = "map_ui2_FRAME_ENCHASE_ICON_SMALL_ARMORYGREEN"
	elseif id == 417002 then
		iconName = "map_ui2_FRAME_ENCHASE_ICON_SMALL_ARMORYBLUE"
	elseif id == 417003 then
		iconName = "map_ui2_FRAME_ENCHASE_ICON_SMALL_ARMORYPURPLE"
	elseif id == 417004 then
		iconName = "map_ui2_FRAME_ENCHASE_ICON_SMALL_ARMORYYELLOW"
	elseif id == 417005 then
		iconName = "map_ui2_FRAME_ENCHASE_ICON_SMALL_ARMORYORANGE"
	end

	if iconName ~= nil then
		local icon = AspriteManager:getInstance():getOneFrame("UI/ui2.bin",iconName)
		return icon
	else
		return nil
	end
end

local function showSingleEquipmentInfo(equipItem,index,itemInfo)
	--CCLuaLog("showSingleEquipmentInfo: " .. index)
	if itemInfo ~= nil and equipItem ~= nil then
		local sprite = ItemManager:Get():getIconSpriteById(equipItem:getItemId())
		if sprite ~= nil then
			local frame = nil
			--if itemInfo.m_quality > 0 then
			--	frame = ItemManager:Get():getIconFrame(itemInfo.m_quality)
			--end
			showEquipPicture(index,sprite,frame)
			local nameColor = ItemManager:Get():getLabelColorByQuality(itemInfo.m_quality)
			showEquipName(index,itemInfo.m_name,nameColor)
			showEquipLevel(index,string.format("Lv %d",equipItem.itemLevel))
		end
	end
end

local function showSinglePosWithoutEquip(index)
	--CCLuaLog("showSinglePosWithoutEquip: " .. index)
	showEquipName(index,"",nil)
	showEquipLevel(index,"")
	local picName = nil
	if index == 1 then
		picName = "map_ui2_FRAME_ENCHASE_ICON_HELMET"
	elseif index == 2 then
		picName = "map_ui2_FRAME_ENCHASE_ICON_ARMOR"
	elseif index == 3 then
		picName = "map_ui2_FRAME_ENCHASE_ICON_WEAPON"
	elseif index == 4 then
		picName = "map_ui2_FRAME_ENCHASE_ICON_SHOE"
	elseif index == 5 then
		picName = "map_ui2_FRAME_ENCHASE_ICON_RING"
	elseif index == 6 then
		picName = "map_ui2_FRAME_ENCHASE_ICON_NECKLACE"
	end

	local pic = nil
	if picName ~= nil then
		pic = AspriteManager:getInstance():getOneFrame("UI/ui2.bin",picName)
	end

	showEquipPicture(index,pic)
end

local function showEquipHoles(enchaseItem,index)
	if enchaseItem ~= nil and enchaseItem.m_equipInfo ~= nil then
		local equipHoles = enchaseItem.m_equipInfo:getEquipHole()
		if equipHoles ~= nil then
			for i = 1,4 do 
				if equipHoles[i] ~= nil then
					if equipHoles[i].item_gem_id == 0 then
						EnchaseUILayer.equipBtnList[index].smallArmoryList[i]:setSprite(nil)
					else
						local sprite = getSmallArmoryIconSprite(equipHoles[i].item_gem_id)
						EnchaseUILayer.equipBtnList[index].smallArmoryList[i]:setSprite(sprite,false)
					end
				else
					local lockSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_ICON_SMALL_LOCK")
					if EnchaseUILayer.equipBtnList[index] ~= nil and EnchaseUILayer.equipBtnList[index].smallArmoryList[i] ~= nil then
						EnchaseUILayer.equipBtnList[index].smallArmoryList[i]:setSprite(lockSprite,false)
					end
				end
			end
		else
			for i = 1,4 do 
				if EnchaseUILayer.equipBtnList[index] ~= nil and EnchaseUILayer.equipBtnList[index].smallArmoryList[i] ~= nil then
						EnchaseUILayer.equipBtnList[index].smallArmoryList[i]:setSprite(nil)
				end		
			end
		end
	end
end

local function showSingleEquip(pos,index)
	--CCLuaLog("showSingleEquip: " .. pos .. " , " .. index)
	local enchaseItem = ItemManager:Get():findItemByPos(pos)
	local backItem = ItemManager:Get():findItemByPos(pos - kItemEnchaseEquip + kItemHeroEquip)
	if backItem ~= nil and enchaseItem ~= nil then
		local itemInfo = backItem:getItemInfo()
		if (backItem.isLocked == false) and (backItem.isEmpty == false) and itemInfo ~= nil then
			showSingleEquipmentInfo(backItem,index,itemInfo)
		else
			showSinglePosWithoutEquip(index)
		end
		showEquipHoles(enchaseItem,index)
	end
end

--local function showHeroEquip()
--	for i =1,6 do
--		showSingleEquip(kItemHeroEquip + i,i)
--	end
--end
--
--local function showFairyEquip()
--	for i =1,6 do
--		showSingleEquip(kItemFairyEquip + i,i)
--	end
--end

local function showEquips()
	for i =1,6 do
		if EnchaseUILayer.selectEquipFlag ~= -1 then
			local pos = kItemEnchaseEquip + EnchaseUILayer.selectEquipFlag * 100 + i
			showSingleEquip(pos,i)
		end
	end
end

local function unSelectEquip()
	if EnchaseUILayer.selectedEquipIndex == 0 then
		return
	end

	if EnchaseUILayer.equipBtnList[EnchaseUILayer.selectedEquipIndex] ~= nil and EnchaseUILayer.equipBtnList[EnchaseUILayer.selectedEquipIndex].button ~= nil then
		EnchaseUILayer.equipBtnList[EnchaseUILayer.selectedEquipIndex].button:getMenuItemSprite():unselected()
	end

	EnchaseUILayer.selectedEquipIndex = 0
end

function EnchaseUILayer:setDefaultSelectedEquip()
	local pos = kItemEnchaseEquip + self.selectEquipFlag * 100
	local hasDefaultEquip = false

	for i =1,6 do
		local backPackItem = ItemManager:Get():findItemByPos(pos + i)
		if backPackItem ~= nil and backPackItem.isEmpty == false and backPackItem.isLocked == false then
			self:setChoosedEquipment(i)
			hasDefaultEquip = true
			break;
		end
	end

	if not hasDefaultEquip then
		-- 没有任何装备
		for i = 1,4 do 
			EnchaseUILayer.selectedEquipStones[i]:unselectItem(false)
		end
	end
	
end

local function isFairyExist(index)
	if index == 0 then
		return true
	elseif index == 1 or index == 2 then
		local fairy = getUserFairyByPos(index)
		if fairy ~= nil then
			return true
		else
			return false
		end
	elseif index == 3 or index == 4 then
		local fairy = getUserFairyByPos(index + FAIRY_POS_START_AST -3)
		if fairy ~= nil then
			return true
		else
			return false
		end
	else
		return false
	end
end

local function enchaseToggleClicked(tag)
	if EnchaseUILayer.selectEquipFlag == tag then
		return
	end

	if not isFairyExist(tag) then
		GameApi:showMessage("您还没有该精灵")
		if EnchaseUILayer.equipToggleTable[tag] ~= nil then
			EnchaseUILayer.equipToggleTable[tag]:unselected()
		end
		return
	end
	--EnchaseUILayer.selectEquipFlag = tag
	if EnchaseUILayer.equipToggleTable[EnchaseUILayer.selectEquipFlag] ~= nil then
		EnchaseUILayer.equipToggleTable[EnchaseUILayer.selectEquipFlag]:unselected()
	end
	
	if EnchaseUILayer.equipToggleTable[tag] ~= nil then
		EnchaseUILayer.selectEquipFlag = tag
		EnchaseUILayer.equipToggleTable[tag]:selected()
		--showFairyEquip()
		showEquips()
		unSelectEquip()
		EnchaseUILayer:setDefaultSelectedEquip()
	end
end

local function onHelpBtClicked(obj)
	setRecentHelpIndex("EnchaseUILayer")
	showUILayerByTag(UITagTable["HelpUILayer"].tag,true)
	SetChatBar(true,-1)
end

function EnchaseUILayer:InitRecastContainer()
	self.recastContainer = EnchaseRecastContainer:create(self.uiLayout)
	self.recastContainer.recastBtn:getMenuItemSprite():registerScriptTapHandler(onRecastBtnClicked)
end

function EnchaseUILayer:InitExchangeUI()
	self.exchangeUILayer = EnchaseExchangeLayer:create(self.uiLayerInstance)
end

function EnchaseUILayer:InitEquipmentUI()
	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	local parentControl = self.uiLayout:FindChildObjectByName("enchaseContainer").__UIContainer__
	local equipBigBg,bgPt =  AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_ENCHASE_FRAME_EQUIPAREA")
	--CCLuaLog(string.format("equipBigBg: wh = %f, %f, pt: %f, %f",equipBigBg:getContentSize().width,equipBigBg:getContentSize().height,bgPt.x,bgPt.y))
	for i = 1,6 do
		self.equipBtnList[i] = EnchaseEquipBtn:createEnchaseEquipBtn()
		self.equipBtnList[i].smallArmoryList = EnchaseSmallArmoryIcon:createEnchaseSmallArmoryIcon()
		self.equipBtnList[i].equipNode = CCNode:create()

		local normalSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_FRAME_EQUIP_HELMET")
		local clickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_FRAME_EQUIP_HELMET_CLICKED")
		local menuItem = CCMenuItemSprite:create(normalSprite,clickedSprite)
		self.equipBtnList[i].button = TXGUI.UIButton:new(string.format("enchaseBt%d",i), i, 0, parentControl, menuItem, nil,-130, "")
		menuItem:registerScriptTapHandler(oneEquipmentClicked)

		self.equipBtnList[i].button:setScale(scale)
		local equipBtNode,nodePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_ENCHASE_FRAME_EQUIP1");
		local equipSprite,picPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_ENCHASE_FRAME_ICON");
		local equipPic = TXGUI.UIPicture:new(string.format("enchasePic%d",i),i,1,parentControl,equipSprite)
		--equipPic:setPosition(ccp(picPt.x - nodePt.x,picPt.y - nodePt.y));
		equipPic:setScale(scale)
		self.equipBtnList[i].pic = equipPic

		local equipName,namePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_ENCHASE_TEXT_EQUIP_HELMET");	
		local nameLabel = TXGUI.UILabelTTF:create(" ",KJLinXin,24);
		nameLabel:setScale(scale)
		self.equipBtnList[i].equipNode:addChild(nameLabel)
		nameLabel:setPosition(ccp(namePt.x * scale - nodePt.x * scale,namePt.y * scale - nodePt.y * scale ));
		self.equipBtnList[i].nameLabel = nameLabel

		local equipLevel,levelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_ENCHASE_TEXT_LEVEL_HELMET");
		local levelLabel = TXGUI.UILabelTTF:create(" ",KJLinXin,24);
		levelLabel:setScale(scale)
		self.equipBtnList[i].equipNode:addChild(levelLabel);
		levelLabel:setPosition(ccp(levelPt.x * scale - nodePt.x * scale,levelPt.y * scale - nodePt.y * scale));
		self.equipBtnList[i].levelLabel = levelLabel;
			
		local totalRow = 3;
		local totalColumn = 2;
		local column = (i - 1) % totalColumn;
		local row = (i - 1 - column) / totalColumn;
		local spaceX = equipBigBg:getContentSize().width * scale - 2 * equipBtNode:getContentSize().width * scale
		local spaceY = equipBigBg:getContentSize().height * scale - 3 * equipBtNode:getContentSize().height * scale
		local containerPtX,containerPtY = parentControl:getCurrentNode():getPosition()
		local x = (bgPt.x - 568)* scale  +(column - 0.5) * (spaceX * 0.5 +  equipBtNode:getContentSize().width * scale)
		local y =(bgPt.y - 320)* scale +(1 - row) * (spaceY / 3  + equipBtNode:getContentSize().height * scale)
		--CCLuaLog(string.format("containerPtX: containerPtY = %f, %f",containerPtX,containerPtY))

		self.equipBtnList[i].equipNode:setPosition(ccp(x,y))
		self.equipBtnList[i].button:setPosition(ccp(x,y))
		self.equipBtnList[i].pic:setPosition(ccp(picPt.x * scale - nodePt.x * scale + x,picPt.y * scale - nodePt.y * scale + y))
		for j = 1,4 do
			local smallArmorySprite,smallPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin",string.format("map_ui2_FRAME_ENCHASE_FRAME_SMALL_ARMORY%d",j));
			local smallPic = TXGUI.UIPicture:new(string.format("enchaseSmallPic_%d_%d",i,j),i,2,parentControl,smallArmorySprite)
			smallPic:setScale(scale)
			smallPic:setPosition(ccp(x + smallPt.x * scale- nodePt.x * scale,y + smallPt.y * scale - nodePt.y * scale));
			self.equipBtnList[i].smallArmoryList[j] = smallPic
		end

		parentControl:getCurrentNode():addChild(self.equipBtnList[i].equipNode)
	end
	for i = 1,4 do
		self.selectedEquipStones[i] = EnchaseGemStoneTable:createSelectGemStoneTable();
		self.selectedEquipStones[i]:init(self.uiLayout,i);
		self.selectedEquipStones[i].iconBtn:getMenuItemSprite():registerScriptTapHandler(onGemStoneIconClicked);
		self.selectedEquipStones[i].lockBtn:getMenuItemSprite():registerScriptTapHandler(onGemStoneLockClicked);
	end

end

function EnchaseUILayer:InitLayer()

	self:InitEquipmentUI()
	self:InitExchangeUI()
	self:InitRecastContainer()
	--showHeroEquip()

	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(closeEnchaseLayerBt)
	local exchangeBt = self.uiLayout:FindChildObjectByName("exchangeCreamBt").__UIButton__:getMenuItemSprite()
	exchangeBt:registerScriptTapHandler(exchageCreamBtClicked)
	local underGroundBt = self.uiLayout:FindChildObjectByName("underGroundBt").__UIButton__:getMenuItemSprite()
	underGroundBt:registerScriptTapHandler(underGroundBtClicked)
	local helpBt = self.uiLayout:FindChildObjectByName("helpBt").__UIButton__:getMenuItemSprite()
	helpBt:registerScriptTapHandler(onHelpBtClicked)
	self.diamondLabel = self.uiLayout:FindChildObjectByName("diamondLabel").__UILabel__
	self.creamLabel = self.uiLayout:FindChildObjectByName("creamLabel").__UILabel__
	self.equipToggleTable = {}
	for i = 0,4 do 
		local button = self.uiLayout:FindChildObjectByName("fairyEnchaseButtonToggle" .. i).__UIButtonToggle__
		local toggle = button:getMenuItemToggle()
		toggle:registerScriptTapHandler(enchaseToggleClicked)
		toggle:setTag(i)
		self.equipToggleTable[i] = button
	end

	if self.notificationFunc == 0 then
		self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	end
	--self.selectEquipFlag = 0
	enchaseToggleClicked(0)
	showEquips()
	--enchaseHeroToggleClicked(nil)
	self:showRecastSpend()
	self:showDiamondLabel()
	self:showCreamLabel()
	GameAudioManager:sharedManager():playEffect(350001,false,0)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

end

function onReceiveEquipInlayGem(error)
	if error == 0 then
		GameApi:showMessage("镶嵌成功")
		EnchaseUILayer.exchangeUILayer:hide()
		local equipPos = EnchaseUILayer:getSelectEquipPos()
		showSingleEquip(equipPos,EnchaseUILayer.selectedEquipIndex)
		EnchaseUILayer:showEquipmentStoneInfo(EnchaseUILayer.selectedEquipIndex,true)
	else
		GameApi:showMessage("宝石不存在")
	end
end

function onRecieveGemRecast(error)
	if error == 0 then
		--GameApi:showMessage("重铸成功");
		EnchaseUILayer:showEquipmentStoneInfo(EnchaseUILayer.selectedEquipIndex,true)
		EnchaseUILayer:showRecastSpend()
	else
		GameApi:showMessage("重铸失败");
	end
end

function onReceiveEquipHoleUnlock(ret)
	if ret then
		GameApi:showMessage("解锁成功")
		local equipPos = EnchaseUILayer:getSelectEquipPos()
		showSingleEquip(equipPos,EnchaseUILayer.selectedEquipIndex)
		EnchaseUILayer:showEquipmentStoneInfo(EnchaseUILayer.selectedEquipIndex,false)
	else
		GameApi:showMessage("解锁失败")
	end
end
