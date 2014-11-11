require("Script/GameConfig/Fairy_getway")
require("Script/GameConfig/RaidMusic")
require("Script/GameConfig/EquipmentItem")
require("Script/GameConfig/Item")
require("Script/SpriteElf/fairybasicinfo")
require("Script/Equipment/EquipConfigTable")

FairyGetwayLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	notificationFunc = 0,
	fairyID = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	--RemoveOneLayer(FairyContactInfoLayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("FairyGetwayLayout")
	if FairyGetwayLayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyGetwayLayer.notificationFunc)
	end
	FairyGetwayLayer:resetValue()
end

function FairyGetwayLayer:createLayer(fairyId)
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,180))
		--self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyGetwayLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,30,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyGetwayUI.plist",self.uiLayerInstance, "FairyGetwayLayout", true)
			self:InitLayer(fairyId)
		end
	else
		if fairyId ~= self.fairyID then
			self:ResetFairy(fairyId)
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyGetwayLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.fairyID = 0
end

function FairyGetwayLayer:destory()
	if FairyGetwayLayer.uiLayerInstance ~= nil then
		FairyGetwayLayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

local function onCloseButtonClicked(tag)
	FairyGetwayLayer:destory()
	GameAudioManager:sharedManager():playEffect(350001,false)
end

local function onInstanceBtClicked(tag)
	--CCLuaLog("onInstanceBtClicked:" .. tag)
	local s = FairyGetwayLayer.uiLayerInstance:getContentSize()
	if Map_Basic[tag] ~= nil and Map_Basic[tag]["RaidID"] ~= nil and Map_Basic[tag]["Type"] ~= nil then
		local isCanEnter = false
		local instanceInfo = InstanceManager:Get():getOneInstance(Map_Basic[tag]["RaidID"])
		if instanceInfo ~= nil and instanceInfo.id ~= 0 then
			isCanEnter = instanceInfo.can_enter_flag
		end
		if isCanEnter then
			InstanceListLayer:showInstanceInfo(FairyGetwayLayer.uiLayerInstance,Map_Basic[tag]["RaidID"],Map_Basic[tag]["Type"],ccp(s.width / 2, s.height / 2))
		else
			GameApi:showMessage(LanguageLocalization:GetLocalization("M_INSTANCE_LEVEL_LOCK"))
		end
	end
end

local function onSystemBtClicked(tag)
	--CCLuaLog("onSystemBtClicked:" .. tag)
	local uiLink = Fairy_getway[FairyGetwayLayer.fairyID]["Drop_system_UI" .. tag]
	local reqLevel = Fairy_getway[FairyGetwayLayer.fairyID]["Drop_system_level" .. tag]
	local userInfo = UserData:GetUserInfo()
	if uiLink ~= nil and userInfo ~= nil then
		if userInfo.level >= tonumber(reqLevel) then
			if (uiLink == "GuildShopLayer" or uiLink == "GuildInstanceListLayer") and userInfo.m_guildId:getHighID() == 0 and userInfo.m_guildId:getLowID() == 0 then
				GameApi:showMessage(LanguageLocalization:GetLocalization("M_MUST_ADDSOCIATY"))
			else
				FairyGetwayLayer:destory()
				--CCLuaLog("uiLink:" .. uiLink)
				showUILayerByName(uiLink,true)
			end
		else
			GameApi:showMessage(LanguageLocalization:GetLocalization("M_DAILYTARGET_LEVEL_ERROR"))
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyGetwayLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyGetwayLayer.uiLayerInstance then
				if FairyGetwayLayer.uiLayerInstance ~= nil then
					FairyGetwayLayer.uiLayerInstance:removeFromParentAndCleanup(true)
				end
				ret = 1
			end
		end
	end

	return ret
end

local function instanceSort(a,b)
	local type_a = tonumber(Map_Basic[a]["Type"])
	local type_b = tonumber(Map_Basic[b]["Type"])
	if type_a == type_b then
		return a < b
	else
		return type_a > type_b
	end
end

function FairyGetwayLayer:addInstanceItem(instanceId,parentLayer,pt)
	--CCLuaLog("FairyGetwayLayer:addInstanceItem: " .. instanceId)
	local instanceIcon = Map_Basic[instanceId]["Icon"]
	local instanceName = Map_Basic[instanceId]["RaidName"]
	local instanceType = Map_Basic[instanceId]["Type"]
	local instanceChapter = Map_Basic[instanceId]["Chapter"]
	local instanceRaidId = Map_Basic[instanceId]["RaidID"]
	
	if instanceIcon ~= nil and instanceName ~= nil and instanceType ~= nil and instanceChapter ~= nil and instanceRaidId ~= nil then
		local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_COPY_FRAME")
		local clickFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_COPY_FRAME_ON")
		local typeFrame,typePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_COPY_TYPE")
		local nameFrame,namePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_COPY_NAME")
		local chapterFrame,chapterPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_COPY_CHAPTER")
		local iconName = "map_ui_system_icon_FRAME_" .. string.upper(instanceIcon)
		local icon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",iconName)
		local isCanEnter = false
		local instanceInfo = InstanceManager:Get():getOneInstance(instanceRaidId)
		if instanceInfo ~= nil and instanceInfo.id ~= 0 then
			isCanEnter = instanceInfo.can_enter_flag
		end
		if not isCanEnter then
			icon = graylightWithCCSprite(icon,false)
		end
		local instanceBt = IconButton:create(bgFrame,icon,clickFrame,-138)
		instanceBt:setContentSize(bgFrame:getContentSize())
		instanceBt:getMenuItem():registerScriptTapHandler(onInstanceBtClicked)
		instanceBt:getMenuItem():setTag(instanceId)
		local typeStr = GetLuaLocalization("FAIRY_DROPINSTANCE_" .. instanceType)
		
		local typeLabel = TXGUI.UILabelTTF:create(typeStr,KJLinXin,20)
		typeLabel:setColor(ccc3(255,184,0))
		instanceBt:addChild(typeLabel,10)
		typeLabel:setPosition(ccpSub(typePt,bgPt))

		local nameLabel = TXGUI.UILabelTTF:create(GetLuaLocalization(instanceName),KJLinXin,20)
		nameLabel:setColor(ccc3(255,255,255))
		instanceBt:addChild(nameLabel,11)
		nameLabel:setPosition(ccpSub(namePt,bgPt))

		local chapterLabel = TXGUI.UILabelTTF:create(GetLuaLocalization("FAIRY_DROPCHAPTER_" .. instanceChapter),KJLinXin,20)
		chapterLabel:setColor(ccc3(17,179,255))
		instanceBt:addChild(chapterLabel,12)
		chapterLabel:setPosition(ccpSub(chapterPt,bgPt))

		--instanceBt:SetButtonIndex(index)
	    parentLayer:addChild(instanceBt)
		instanceBt:setPosition(pt)
	end
end

function FairyGetwayLayer:addSystemItem(index,parentLayer,pt)
	local systemIcon = Fairy_getway[self.fairyID]["Drop_system_icon" .. index]
	local uiLink = Fairy_getway[self.fairyID]["Drop_system_UI" .. index]
	if systemIcon ~= nil and uiLink ~= nil then
		local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_SYSTEM_FRAME")
		local clickFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_SYSTEM_FRAME_ENTER")
		local iconPic,iconPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_SYSTEM_FRAME_ICON")
		local iconName = "map_ui_system_icon_FRAME_" .. string.upper(systemIcon)
		local icon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",iconName)
		local systemBt = IconButton:create(bgFrame,icon,clickFrame,-138)
		systemBt:getMenuItem():registerScriptTapHandler(onSystemBtClicked)
		systemBt:getMenuItem():setTag(index)
		systemBt:setContentSize(bgFrame:getContentSize())
		--systemBt:SetButtonIndex(index)
		parentLayer:addChild(systemBt)
		systemBt:setPosition(pt)
		--self.systemIcon = icon
		--if self.systemIcon ~= nil and self.systemIcon ~= nil then
		--	self.systemIcon:setPosition(ccpSub(iconPicPt,bgPt))
		--	self.baseNode:addChild(self.systemIcon,20)
		--end
		--if self.systemFramePic ~= nil then

	end
end

function FairyGetwayLayer:checkEquipmentId(targetId)
	local equipId = 0
	local fragId = 0
	if EquipmentItems[targetId] ~= nil then
		equipId = targetId
		fragId = checkSingleFragByEquip(equipId)
	elseif Items[targetId] ~= nil and Items[targetId]["Type"] == "19" then
		fragId = targetId
		local temp = 0
		temp,equipId = checkSingleEquipfrag(fragId)
	end
	return equipId ,fragId
end

function FairyGetwayLayer:InitInstanceDrop()
	--CCLuaLog("FairyGetwayLayer:InitInstanceDrop " .. self.fairyID)
	if self.uiLayout == nil then
		--CCLuaLog("self.uiLayout is nil")
		return
	end
	if Fairy_getway[self.fairyID] ~= nil then
		--CCLuaLog("000000000")
		local hasInstanceFlag = false
		if tonumber(Fairy_getway[self.fairyID]["Is_drop"]) == 1 then
			--CCLuaLog("111111111")
			local instanceTable = {}
			if fairys[self.fairyID] ~= nil then
				local fairyFragId = "44" .. self.fairyID
				for k,v in pairs(Map_Basic) do
					if Map_Basic[k]["Type"] == "1" or Map_Basic[k]["Type"] == "5" then
						local hasFrag = false
						for i = 1,5 do
							local str = Map_Basic[k]["RaidDrops" .. i]
							if str ~= nil and str ~= "0" then
								for id,a,b in string.gfind(str,"(%d+)\/(%d+)\/(%d+)") do
									if id == fairyFragId then
										hasFrag = true
									end
								end
							end
						end

						for i = 1,3 do
							local str = Map_Basic[k]["Spirit" .. i]
							if str ~= nil and str ~= "0" then
								for id,a,b in string.gfind(str,"(%d+)\/(%d+)\/(%d+)") do
									if tonumber(id) == self.fairyID then
										hasFrag = true
									end
								end
							end
						end
					
						if hasFrag then
							table.insert(instanceTable,k)
						end
					end
				end
			else
				--CCLuaLog("22222222")
				local equipId, equipFragId = self:checkEquipmentId(self.fairyID)
				--CCLuaLog("equipId: " .. equipId .. " ,equipFragId: " .. equipFragId)
				if equipId ~= 0 or equipFragId ~= 0 then
					for k,v in pairs(Map_Basic) do
						if Map_Basic[k]["Type"] == "1" or Map_Basic[k]["Type"] == "5" then
							local hasFrag = false
							for i = 1,5 do
								local str = Map_Basic[k]["RaidDrops" .. i]
								if str ~= nil and str ~= "0" then
									for a,b,c in string.gfind(str,"(%d+)\/(%d+)\/(%d+)") do
										local id = tonumber(a)
										if (id == equipId or id == equipFragId) and id ~= 0 then
											hasFrag = true
										end
									end
								end
							end
							if hasFrag then
								table.insert(instanceTable,k)
							end
						end
					end
				end

			end
			if #instanceTable > 0 then
				local function fiterInstatnceFuc(t)
					local temp = {}
					local instanceTab = {}
					for k,v in pairs(t) do
						local instanceID = tonumber(Map_Basic[v]["RaidID"])
						if instanceTab[instanceID] == nil then
							instanceTab[instanceID] = true
							table.insert(temp,v)
						end
					end
					return temp
				end
				instanceTable = fiterInstatnceFuc(instanceTable)
				hasInstanceFlag = true
				table.sort(instanceTable,instanceSort)

				local page = self.uiLayout:FindChildObjectByName("instanceScrollPage").__UIScrollPage__
				local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_COPY_FRAME")
				local buttonWidth = bgFrame:getContentSize().width
				local width = page:getContentSize().width
				local index = 1
				local currentLayer = nil
				local pageColumn = 3
				local cellOffset = (width - buttonWidth * pageColumn) / pageColumn
				for k,v in pairs(instanceTable) do
					local column = (index - 1) % pageColumn
					if column == 0 then
						currentLayer = CCLayer:create()
						currentLayer:setAnchorPoint(ccp(0,0))
						page:addPage(currentLayer)
					end
					local delta_x = (column - 1) * buttonWidth + (column - 1) * cellOffset
					local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
					self:addInstanceItem(v,currentLayer,ccp(delta_x/scale,0))
					index = index + 1
				end
			end
		end

		local label = self.uiLayout:FindChildObjectByName("instanceNoneLabel").__UILabel__
		if label ~= nil then
			label:setVisible(not hasInstanceFlag)
		end
	end
	
end

function FairyGetwayLayer:InitSystemDrop()
	if Fairy_getway[self.fairyID] ~= nil and self.uiLayout ~= nil then
		local page = self.uiLayout:FindChildObjectByName("systemScrollPage").__UIScrollPage__
		local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_SPIRIT_GET_SYSTEM_FRAME")
		local buttonWidth = bgFrame:getContentSize().width
		local width = page:getContentSize().width
		local index = 1
		local currentLayer = nil
		local pageColumn = 4
		local cellOffset = (width - buttonWidth * pageColumn) / pageColumn
		local hasDropFlag = false
		local fairyTable = Fairy_getway[self.fairyID]
		while fairyTable["Drop_system_icon" .. index] ~= nil and fairyTable["Drop_system_UI" .. index] ~= nil and fairyTable["Drop_system_icon" .. index] ~= "0" and fairyTable["Drop_system_UI" .. index] ~= "0" do
			local column = (index - 1) % pageColumn
			hasDropFlag = true
			if column == 0 then
				currentLayer = CCLayer:create()
				currentLayer:setAnchorPoint(ccp(0,0))
				page:addPage(currentLayer)
			end
			local delta_x = (column - 1) * buttonWidth + (column - 1) * cellOffset
			local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
			self:addSystemItem(index,currentLayer,ccp(delta_x/scale,0))
			index = index + 1
		end

		local label = self.uiLayout:FindChildObjectByName("systemNoneLabel").__UILabel__
		if label ~= nil then
			label:setVisible(not hasDropFlag)
		end
		
	end
end

function FairyGetwayLayer:InitLayer(fairyId)
	--CCLuaLog("FairyGetwayLayer: " .. fairyId)
	self.fairyID = fairyId
	if Items[self.fairyID] ~= nil and Items[self.fairyID]["Type"] == "21" then
		local num = tonumber(self.fairyID)
		local x1,x2 = math.modf(num/10000)
		self.fairyID = x2 * 10000
		self.fairyID = tonumber(tostring(self.fairyID))
	end
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	self:InitLabel()
	self:InitInstanceDrop()
	self:InitSystemDrop()
end

function FairyGetwayLayer:InitLabel()
	if EquipmentItems[self.fairyID] ~= nil or (Items[self.fairyID] ~= nil and Items[self.fairyID]["Type"] == "19") then
		local label = self.uiLayout:FindChildObjectByName("systemNoneLabel").__UILabel__
		label:setString(LanguageLocalization:GetLocalization("M_EQUIPMENT_DROP_SYSTEM_NO"))
		label = self.uiLayout:FindChildObjectByName("instanceNoneLabel").__UILabel__
		label:setString(LanguageLocalization:GetLocalization("M_EQUIPMENT_DROP_RAID_NO"))
		label = self.uiLayout:FindChildObjectByName("textLabel").__UILabel__
		label:setString(LanguageLocalization:GetLocalization("M_GET_ACCOUTER_TEXT"))
	else
		local label = self.uiLayout:FindChildObjectByName("systemNoneLabel").__UILabel__
		label:setString(LanguageLocalization:GetLocalization("M_FAIRY_DROP_SYSTEM_NO"))
		label = self.uiLayout:FindChildObjectByName("instanceNoneLabel").__UILabel__
		label:setString(LanguageLocalization:GetLocalization("M_FAIRY_DROP_RAID_NO"))
		label = self.uiLayout:FindChildObjectByName("textLabel").__UILabel__
		label:setString(LanguageLocalization:GetLocalization("M_GET_FAIRY_TEXT"))
	end
end

function FairyGetwayLayer:ResetFairy(newFairyId)
	if FairyGetwayLayer.uiLayerInstance ~= nil then
		self.fairyID = newFairyId
		if Items[self.fairyID] ~= nil and Items[self.fairyID]["Type"] == "21" then
			local num = tonumber(self.fairyID)
			local x1,x2 = math.modf(num/10000)
			self.fairyID = x2 * 10000
			self.fairyID = tonumber(tostring(self.fairyID))
		end
		local systemScrollPage = self.uiLayout:FindChildObjectByName("systemScrollPage").__UIScrollPage__
		systemScrollPage:removeAllPages()
		local instanceScrollPage = self.uiLayout:FindChildObjectByName("instanceScrollPage").__UIScrollPage__
		instanceScrollPage:removeAllPages()
		self:InitLabel()
		self:InitInstanceDrop()
		self:InitSystemDrop()
	end
end