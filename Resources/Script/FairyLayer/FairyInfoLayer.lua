-- 精灵信息页面
require("Script/GameConfig/Fairy_Quality")
require("Script/GameConfig/Fairy_Parameter")
require("Script/GameConfig/fairyskill")
require("Script/SpriteElf/fairybasicinfo")
require("Script/FairyLayer/FairyAdvanceLayer")
require("Script/FairyLayer/FairyLevelUpLayer")
require("Script/FairyLayer/FairyTrainLayer")
require("Script/Fairy/FairyGetwayLayer")

FairyInfoLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

	buttonState = 0, -- 0: 不显示按钮 1：显示2个按钮 2：显示4个按钮
	infoState = 1, -- 1: 使用pos 2: 使用id

	fairyPos = 0,
	fairyId = 0,
    playId = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if FairyInfoLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FairyInfoLayer.notificationFunc);
	end

	FairyInfoLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyInfoUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FairyInfoLayer.uiLayerInstance then
			if G_CurLayerInstance ~= FairyInfoLayer.uiLayerInstance then
				FairyInfoLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function FairyInfoLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyInfoLayer"].tag
			parentNode:addChild(self.uiLayerInstance,20,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyInfoUI.plist",self.uiLayerInstance, "FairyInfoUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyInfoLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.buttonState = 0
	self.infoState = 1
	self.fairyPos = 0
	self.fairyId = 0
    self.playId = nil
end

-- 删除UI
function FairyInfoLayer:destroyed()
	CCLuaLog("--- FairyInfoLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
	--SetChatBar(true,-1)
end

--关闭页面
local function onPressCloseBtn(tag)
	--RemoveOneLayer(FairyInfoLayer.uiLayerInstance)
	FairyInfoLayer:destroyed()
end

-- 点击精灵替换按钮
local function onPressReplaceBtn(tag)
	CCLuaLog("--- onPressReplaceBtn ---")
	G_SelectFairyToEquip(true)
	FairyInfoLayer:destroyed()
end

-- 点击查看掉落按钮
local function onPressGetwayBtn(tag)
	if FairyInfoLayer.infoState == 1 then
		local userFairyInfo = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId)
		if userFairyInfo ~= nil then
			FairyGetwayLayer:createLayer(userFairyInfo.fairyId)
		end
	else
		FairyGetwayLayer:createLayer(FairyInfoLayer.fairyId)
	end
	FairyInfoLayer:destroyed()
end

-- 点击精灵升级按钮
local function onPressLevelupBtn(tag)
	CCLuaLog("--- onPressLevelupBtn ---")
	FairyLevelUpLayer.lastLayer = 1
	FairyLevelUpLayer.fairyPos = FairyInfoLayer.fairyPos
	FairyLevelUpLayer:CreateLayer(nil)
end

-- 点击精灵特训按钮
local function onPressTrainBtn(tag)
	CCLuaLog("--- onPressTrainBtn ---")
	FairyTrainLayer.lastLayer = 1
	FairyTrainLayer.fairyPos = FairyInfoLayer.fairyPos
	FairyTrainLayer:CreateLayer(nil)
end

-- 点击精灵进阶按钮
local function onPressAdvanceBtn(tag)
	CCLuaLog("--- onPressAdvanceBtn ---")
	FairyAdvanceLayer.lastLayer = 1
	FairyAdvanceLayer.fairyPos = FairyInfoLayer.fairyPos
	FairyAdvanceLayer:CreateLayer(nil)
end

-- 点击精灵卸下按钮
local function onPressUnloadBtn(tag)
	CCLuaLog("--- onPressUnloadBtn ---")
	G_EquipDownFairyFriend(FairyInfoLayer.fairyPos)
	FairyInfoLayer:destroyed()
end

--更新底部按钮显示
--根据FairyInfoLayer.buttonState
-- 0：不显示 1：显示两个 2：显示4个
local function updateButtonState()
	local state = FairyInfoLayer.buttonState

	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyReplaceBtn"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyLevelupBtn"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyTrainBtn"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyAdvanceBtn"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyReplaceBtn2"):setVisible(state == 1)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyUnloadBtn"):setVisible(state == 1)

	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyReplaceText"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyLevelupText"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyTrainText"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyAdvanceText"):setVisible(state == 2)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyReplaceText2"):setVisible(state == 1)
	FairyInfoLayer.uiLayout:FindChildObjectByName("fairyUnloadText"):setVisible(state == 1)
end

local function getFairyBasicAttrTable(fairyId)
	local attrTable = {}

	local userType = UserData:GetUserType()

	if userType == 3 or userType == 4 then
		attrTable[1] = Fairy_Quality[1].MagAttack + fairys[fairyId].MagAttack
	else
		attrTable[1] = Fairy_Quality[1].PhyAttack + fairys[fairyId].PhyAttack
	end

	attrTable[2] = Fairy_Quality[1].PhyDefense + fairys[fairyId].PhyDefense
	attrTable[3] = Fairy_Quality[1].MagDefense + fairys[fairyId].MagDefense
	attrTable[4] = Fairy_Quality[1].SkiAttack + fairys[fairyId].SkiAttack
	attrTable[5] = Fairy_Quality[1].SkiDefense + fairys[fairyId].SkiDefense
	attrTable[6] = Fairy_Quality[1].HP + fairys[fairyId].HP

	local fairyQuality = fairys[fairyId].Quality
	local fairyIntensity = Fairy_Parameter[1]["Intensity"..fairyQuality] / 10000

	for i = 1, 6 do
		attrTable[i] = math.floor(attrTable[i] * fairyIntensity)
	end

	return attrTable
end

local function getFairyRealAttrTable(fairyPos)
	local attrTable = {}

	local userFairyInfo = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId)
	local orgAttrTable = userFairyInfo.fairyAttr
	--local orgTrainAttrTable = userFairyInfo.fairyTrainAttr

	local userType = UserData:GetUserType()

	if userType == 3 or userType == 4 then
		attrTable[1] = orgAttrTable[2]-- + (orgTrainAttrTable[2] or 0)
	else
		attrTable[1] = orgAttrTable[1]-- + (orgTrainAttrTable[1] or 0)
	end

	attrTable[2] = orgAttrTable[4]-- + (orgTrainAttrTable[4] or 0)
	attrTable[3] = orgAttrTable[5]-- + (orgTrainAttrTable[5] or 0)
	attrTable[4] = orgAttrTable[3]-- + (orgTrainAttrTable[3] or 0)
	attrTable[5] = orgAttrTable[6]-- + (orgTrainAttrTable[6] or 0)
	attrTable[6] = orgAttrTable[7]-- + (orgTrainAttrTable[7] or 0)

	return attrTable
end

local function initFairyInfoBasicInfo()

	-- 精灵初始信息
	if FairyInfoLayer.infoState == 2 then
		local fairyBasicInfo = getFairyBasicInfo(FairyInfoLayer.fairyId)

		-- 名称
		local fairyName = fairyBasicInfo.name

		local nameStr = fairyName.." + 0"
		FairyInfoLayer.uiLayout:FindChildObjectByName("fairyName").__UILabel__:setString(nameStr)
        FairyInfoLayer.uiLayout:FindChildObjectByName("fairyName").__UILabel__:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))

		--等级
		FairyInfoLayer.uiLayout:FindChildObjectByName("fairyLevel").__UILabel__:setString("Lv.1")

		--精灵动画
		local fairyFrame = FairyInfoLayer.uiLayout:FindChildObjectByName("fairyFrame").__UIPicture__
		local size = fairyFrame:getCurrentNode():getContentSize()		
		local px = size.width / 2
		local py = size.height / 2
		local modePt = CCPoint(px,py)								
		SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(fairyFrame:getCurrentNode(), FairyInfoLayer.fairyId, modePt)

		-- 精灵资质
		local qualityStr = getFairyQualityStr(fairyBasicInfo.quality)
		FairyInfoLayer.uiLayout:FindChildObjectByName("fairyQuality").__UILabel__:setString(qualityStr)
        FairyInfoLayer.uiLayout:FindChildObjectByName("fairyQuality").__UILabel__:setColor(ccc3(255, 184, 0))

		-- 属性值
		local attrTable = getFairyBasicAttrTable(FairyInfoLayer.fairyId)

		for i = 1, 6 do
			FairyInfoLayer.uiLayout:FindChildObjectByName("attrNum"..i).__UILabel__:setString("+"..attrTable[i])
		end


	else
		local userFairyInfo = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId)
		local fairyBasicInfo = getFairyBasicInfo(userFairyInfo.fairyId)

		-- 名称
		local fairyName = fairyBasicInfo.name
		local fairyGrade = userFairyInfo.fairyGrade

		local nameStr = fairyName.." + "..fairyGrade
		FairyInfoLayer.uiLayout:FindChildObjectByName("fairyName").__UILabel__:setString(nameStr)
        FairyInfoLayer.uiLayout:FindChildObjectByName("fairyName").__UILabel__:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))

		--等级
		local level = userFairyInfo.fairyLevel
		local levelStr = "Lv."..level

		FairyInfoLayer.uiLayout:FindChildObjectByName("fairyLevel").__UILabel__:setString(levelStr)

		--精灵动画
		local fairyId = userFairyInfo.fairyId

		local fairyFrame = FairyInfoLayer.uiLayout:FindChildObjectByName("fairyFrame").__UIPicture__
		local size = fairyFrame:getCurrentNode():getContentSize()		
		local px = size.width / 2
		local py = size.height / 2
		local modePt = CCPoint(px,py)								
		SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(fairyFrame:getCurrentNode(), fairyId, modePt)

		-- 精灵资质
		local qualityStr = getFairyQualityStr(fairyBasicInfo.quality)
		FairyInfoLayer.uiLayout:FindChildObjectByName("fairyQuality").__UILabel__:setString(qualityStr)
        FairyInfoLayer.uiLayout:FindChildObjectByName("fairyQuality").__UILabel__:setColor(ccc3(255, 184, 0))

		-- 获得实时属性值
		local attrTable = getFairyRealAttrTable(FairyInfoLayer.fairyPos)

		for i = 1, 6 do
			FairyInfoLayer.uiLayout:FindChildObjectByName("attrNum"..i).__UILabel__:setString("+"..attrTable[i])
		end

	end

	-- 物攻魔攻标签
	local userType = UserData:GetUserType()

	if userType == 3 or userType == 4 then
		FairyInfoLayer.uiLayout:FindChildObjectByName("attrName1").__UILabel__:setString(GetLuaLocalization("M_FAIRY_LU_ATTRIBUTE7"))
	else
		FairyInfoLayer.uiLayout:FindChildObjectByName("attrName1").__UILabel__:setString(GetLuaLocalization("M_FAIRY_LU_ATTRIBUTE1"))
	end	
end

local function initContactInfo()
    local contactNames = {}
    local talentNames = {}

	local contactInfos = {}
	local talentInfos = {}

	local contactActiveTable = {}
	local talentActiveTable = {}

	-- 初始化连携数据
	local fairyId = 0
	if FairyInfoLayer.infoState == 2 then
		fairyId = FairyInfoLayer.fairyId
	else
		fairyId = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId).fairyId
	end

	local fetters = getFettersByFairyId(fairyId)
	for i = 1, #fetters do
		local fetterId = fetters[i]
		local fetterName = getFetterNameById(fetterId)
		local fetterDes = getFetterDescribeById(fetterId)
        contactNames[i] = fetterName .. "："
		contactInfos[i] = fetterDes

		if FairyInfoLayer.infoState == 1 then
			local fairyFetter = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId).fairyFetters
			if fairyFetter ~= nil and fairyFetter[fetterId] ==  1 then
				contactActiveTable[i] = true
			else
				contactActiveTable[i] = false
			end
		else
			contactActiveTable[i] = false
		end
	end

	--- 初始化天赋数据
	local talentNameTable, talentContentTable = getTalentContentTable(fairyId)
	
	for i = 1, 7 do
		if talentContentTable[i] ~= nil then
			table.insert(talentNames, talentNameTable[i])
            table.insert(talentInfos, talentContentTable[i])

			if FairyInfoLayer.infoState == 1 then
				local grade = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId).fairyGrade
				if grade >= i then
					table.insert(talentActiveTable, true)
				else
					table.insert(talentActiveTable, false)
				end
			else
				table.insert(talentActiveTable, false)
			end
		end
	end


	-- listType : 1 : 连携信息 2：天赋信息
	local function addSingleItem(listType, itemList, itemInfoStr, active)
		local itemFrameStr = ""
		if listType == 1 then
			itemFrameStr = "map_ui_FRAME_FAIRY_INFO_TEXT_CONTACT1"
		else
			itemFrameStr = "map_ui_FRAME_FAIRY_INFO_TEXT_TALENT1"
		end

		local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin",itemFrameStr)
		local bgListItemSize = itemBg:getContentSize()		
		local ListItemNode = CCNode:create()        		

		--描述信息
		local nameLabel = TXGUI.UILabelTTF:create(itemInfoStr, KJLinXin, 16, CCSizeMake(bgListItemSize.width,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop)
        ListItemNode:addChild(nameLabel)

		itemList:AddBaseItem(nameLabel:getContentSize(), ListItemNode)
		
		
		if active then
			nameLabel:setColor(ccc3(255,255,0))
		end
	end

    	-- listType : 1 : 连携信息 2：天赋信息
	local function addDoubleItem(listType, itemList, itemNameStr, itemInfoStr, active)
		local itemFrameStr = ""
		if listType == 1 then
			itemFrameStr = "map_ui_FRAME_FAIRY_INFO_TEXT_CONTACT1"
		else
			itemFrameStr = "map_ui_FRAME_FAIRY_INFO_TEXT_TALENT1"
		end

		local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin",itemFrameStr)
		local bgListItemSize = itemBg:getContentSize()		
		local ListItemNode = CCNode:create()     
        
        -- 名称信息   
        local nameLabel = TXGUI.UILabelTTF:create(itemNameStr, KJLinXin, 16, CCSizeMake(0,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop)	
        	
        ListItemNode:addChild(nameLabel)

		-- 内容信息
		local contentLabel = TXGUI.UILabelTTF:create(itemInfoStr, KJLinXin, 16, CCSizeMake(bgListItemSize.width - nameLabel:getContentSize().width,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop)
        contentLabel:setPosition(ccp(nameLabel:getContentSize().width + contentLabel:getContentSize().width / 2 - bgListItemSize.width / 2, 0))
        ListItemNode:addChild(contentLabel)

        nameLabel:setPosition(ccp(nameLabel:getContentSize().width / 2 - bgListItemSize.width / 2,  contentLabel:getContentSize().height / 2 - nameLabel:getContentSize().height / 2))

		itemList:AddBaseItem(CCSizeMake(bgListItemSize.width, contentLabel:getContentSize().height), ListItemNode)
		
		
		if active then
            nameLabel:setColor(ccc3(255,184,0))
			contentLabel:setColor(ccc3(255,195,118))
        else
            nameLabel:setColor(ccc3(255,255,255))
			contentLabel:setColor(ccc3(255,255,255))
		end
	end

	-- 建立连携信息列表
	local contactList = FairyInfoLayer.uiLayout:FindChildObjectByName("contactScrollList").__UIScrollList__
	
	for i = 1, #contactInfos do
		addDoubleItem(1, contactList, contactNames[i], contactInfos[i], contactActiveTable[i])
	end	

	if #contactInfos == 0 then
		addSingleItem(1, contactList, GetLuaLocalization("M_FAIRY_NOLINK"), false)
	end

	-- 建立天赋信息列表
	local talentList = FairyInfoLayer.uiLayout:FindChildObjectByName("talentScrollList").__UIScrollList__
	
	for i = 1, #talentInfos do
		addDoubleItem(2, talentList, talentNames[i], talentInfos[i], talentActiveTable[i])
	end	

	if #talentInfos == 0 then
		addSingleItem(2, talentList, GetLuaLocalization("M_FAIRY_NOGENIUS"), false)
	end

	-- 初始化技能信息
	local fairyId = 0
	if FairyInfoLayer.infoState == 2 then
		fairyId = FairyInfoLayer.fairyId
	else
		local userFairyInfo = getPlayerFairyByPos(FairyInfoLayer.fairyPos, FairyInfoLayer.playId)
		fairyId = userFairyInfo.fairyId
	end
	
	--CCLuaLog("--- fairyId ---"..fairyId)
	local skillDesTable = {}
	for _, v in pairs(fairyskill) do
		--CCLuaLog("--- skill ---")
		--CCLuaLog("--- v.fairy_ID ---"..v.fairy_ID)
		if tonumber(v.fairy_ID) == fairyId then
			--CCLuaLog("--- find skill ---")
			if tonumber(v.Skill_type2) == 0 then
				--CCLuaLog("--- find skill 0---")
				skillDesTable[2] = v.skill_description
			elseif tonumber(v.Skill_type2) == 1 then
				--CCLuaLog("--- find skill 1---")
				skillDesTable[1] = v.skill_description
			elseif tonumber(v.Skill_type2) == 2 then
				--CCLuaLog("--- find skill 2---")
				skillDesTable[3] = v.skill_description
			end
		end
	end

	for i = 1, 3 do
		if skillDesTable[i] ~= nil then
			--CCLuaLog("--- skillDesTable[i] ~= nil ---"..i)
			FairyInfoLayer.uiLayout:FindChildObjectByName("skillText"..i).__UILabel__:setString(GetLuaLocalization(skillDesTable[i]))
		else
			FairyInfoLayer.uiLayout:FindChildObjectByName("skillText"..i).__UILabel__:setString("")
		end
	end

    -- 初始化怒气信息
    local rageBasic = GetFairyRageBasic(fairyId)
    local rageSpeed = GetFairyRageRate(fairyId)

    FairyInfoLayer.uiLayout:FindChildObjectByName("rageNum").__UILabel__:setString(""..rageBasic)
    FairyInfoLayer.uiLayout:FindChildObjectByName("rageSpeedNum").__UILabel__:setString(""..(100 + rageSpeed / 10).."%")

end

function FairyInfoLayer:InitLayer()
	CCLuaLog("--- FairyInfoLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
	self.uiLayout:FindChildObjectByName("fairyReplaceBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressReplaceBtn)
	self.uiLayout:FindChildObjectByName("fairyLevelupBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressLevelupBtn)
	self.uiLayout:FindChildObjectByName("fairyTrainBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressTrainBtn)
	self.uiLayout:FindChildObjectByName("fairyAdvanceBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressAdvanceBtn)
	self.uiLayout:FindChildObjectByName("fairyReplaceBtn2").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressReplaceBtn)
	self.uiLayout:FindChildObjectByName("fairyUnloadBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressUnloadBtn)
	self.uiLayout:FindChildObjectByName("fairyDropBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressGetwayBtn)

	updateButtonState()


	initFairyInfoBasicInfo()
	initContactInfo()

	--PushOneLayer(self.uiLayerInstance,"","")
	--SetChatBar(false,-1)
end

function ShowFairyInfoLayer(fairyId)
	FairyInfoLayer.fairyId = fairyId
	CCLuaLog("--- FairyInfoLayer.fairyId ---"..fairyId)

	FairyInfoLayer.buttonState = 0
	FairyInfoLayer.infoState = 2
	FairyInfoLayer:CreateLayer(nil)
end