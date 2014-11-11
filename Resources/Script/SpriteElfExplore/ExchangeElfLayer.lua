require("Script/ChatToolLayer")
require("Script/CommonDefine")
require("Script/SpriteElf/SpriteElfLayer")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/GameConfig/uiTagDefine")
require("Script/SpriteElf/SpriteElfDataCenter")

ExchangeElfLayer = {
	LayerInstance = nil,
	Layout = nil,
	SelectItemIndex = nil,
	SelectedButton = nil,
	exploerExpContentLabel = nil,
	ValidElfTable = {},
	ElfListTable = {},
	ElfListItemTable = {}
}

function ExchangeElfLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.SelectItemIndex = nil
	self.SelectedButton = nil
	self.exploerExpContentLabel = nil
	self.ValidElfTable = {}
	self.ElfListTable = {}
	self.ElfListItemTable = {}
end

function ExchangeElfLayer:ResetListData()
	self.SelectItemIndex = nil
	self.SelectedButton = nil
	self.ValidElfTable = {}
	self.ElfListTable = {}
	self.ElfListItemTable = {}
end

local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("ExchangeElfPanel")
	ExchangeElfLayer:ResetListData()
end

--Note: 创建Layer
function ExchangeElfLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ExchangeElf.plist",self.LayerInstance, "ExchangeElfPanel", true);
	self.Layout = uiLayout

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["ExchangeElfLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	self:ShowLeftScrollList()
	self:ShowElfBgPic()
		
	onExchangeElfListItemClick(0)
	PushOneLayer(self.LayerInstance,"","")	
	--SetChatBar(true,-1);
end

function ExchangeElfLayer:InitLayer()	
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(ExchangeElfLayerCloseBtnClick);

	local exchangeBtn = self.Layout:FindChildObjectByName("exchangeBtn").__UIButton__:getMenuItemSprite();
	exchangeBtn:registerScriptTapHandler(ExchangeElfLayerExchangeBtnClick);

	--Note: 底部信息面板
	local downContainer = self.Layout:FindChildObjectByName("commonDownContainer").__UIContainer__;
	self.exploerExpContentLabel = downContainer:FindChildObjectByName("Experience_label").__UILabel__;

	local backToExploerBtn = downContainer:FindChildObjectByName("backToElfExploreBtn").__UIButton__;
	local backToExploerBtnMenuItem = backToExploerBtn:getMenuItemSprite();
	backToExploerBtnMenuItem:registerScriptTapHandler(BackToElfExploerBtnClick);
	local backToExploerIconName = "map_ui_system_icon_FRAME_ICON_EXPLORE";
	local backToExploerIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,backToExploerIconName);
	backToExploerBtn:setIconSprite(backToExploerIcon)

	local backToExploerLabel = downContainer:FindChildObjectByName("backToExploreLabel").__UILabel__;
	local languageFlag = "M_DAILYTARGET_5040_NAME"
	local content = LanguageLocalization:GetLocalization(languageFlag)
	backToExploerLabel:setString(content)

	ExchangeElfLayer:ShowExploerExp()
end

function ExchangeElfLayer:FreshLayer()
	--self:ResetListData()
	--self:ShowLeftScrollList()		
	if nil ~= self.SelectItemIndex and self.SelectItemIndex > 0 then
		onExchangeElfListItemClick(self.SelectItemIndex-1)	
	end	
end

function ExchangeElfLayer:ShowElfBgPic()
	local elfBgPic = self.Layout:FindChildObjectByName("frameDecoration").__UIPicture__
	if nil ~= elfBgPic then
		local path = GameResourceManager:sharedManager():storedFullPathFromRelativePath("UI/ui_fairy.png")
		local sprite = CCSprite:create(path)
		elfBgPic:setSprite(sprite)
	end
end

function ExchangeElfLayer:ShowElfOnLayer(elfId)
	if nil == elfId then
		return
	end
	local container = self.Layout:FindChildObjectByName("ExchangeElfContainer").__UIContainer__;
	local elfBgPic = self.Layout:FindChildObjectByName("frameShowFairy").__UIPicture__
	if elfBgPic ~= nil then	
		--local px,py = elfBgPic:getCurrentNode():getPosition()
		--local scale = TXGUI.UIManager:sharedManager():getScaleFactor()
		local size = elfBgPic:getCurrentNode():getContentSize()		
		local px = size.width / 2
		local py = size.height / 2
		local modePt = CCPoint(px,py)		
		SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(elfBgPic:getCurrentNode(),elfId,modePt)
	end
end

function ExchangeElfLayer:ShowOneElfSkillInfo(elfId)
	local skillNameLabel = self.Layout:FindChildObjectByName("skill_name_text").__UILabel__;
	local skillDescriptionLabel = self.Layout:FindChildObjectByName("skill_description_text").__UILabel__;		
	if nil == elfId then
		skillNameLabel:setString("")
		skillDescriptionLabel:setString("")
	else
		skillNameLabel:setString(GetOneElfSkillName(elfId))
		skillDescriptionLabel:setString(GetOneElfSkillDescription(elfId))
	end
end

function ExchangeElfLayer:ShowSkillLeftIcon(elfId)
	local skillIcon = self.Layout:FindChildObjectByName("iconFairySkill").__UIPicture__
	if nil ~= skillIcon then
		if nil ~= elfId then
			local iconName = GetOneElfSkillIcon(elfId)
			local childFrameName = "map_ui_system_icon_FRAME_" .. iconName
			local sprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,childFrameName)
			skillIcon:setSprite(sprite)
		else
			skillIcon:setSprite(nil)
			local frameIcon = self.Layout:FindChildObjectByName("frameFairySkill").__UIPicture__
			frameIcon:setVisible(false)
		end
	end
end

function ExchangeElfLayer:ShowExpLevelInfo(elfId)

	local pointLabel = self.Layout:FindChildObjectByName("exp_num_text").__UILabel__;
	local levelLabel = self.Layout:FindChildObjectByName("level_num_text").__UILabel__;
	local flagPointLabel = self.Layout:FindChildObjectByName("exp_common_text").__UILabel__;
	local flagLevelLabel = self.Layout:FindChildObjectByName("level_common_text").__UILabel__;

	local alreadyGotLabel = self.Layout:FindChildObjectByName("alreadygot_text").__UILabel__;

	local isExist = LuaSpriteElfDataCenter:IsOneElfExistInElfDataTable(elfId)

	if isExist then
		pointLabel:setString("")
		levelLabel:setString("")
		flagPointLabel:setVisible(false)
		flagLevelLabel:setVisible(false)
		alreadyGotLabel:setVisible(true)
	else
		local point = GetOneElfNeedExpPoint(elfId)
		local level = ElfExploreDataCenter:GetExchangeElfUnLockLevel(elfId)
		pointLabel:setString("" .. point)
		levelLabel:setString("" .. level)
		flagPointLabel:setVisible(true)
		flagLevelLabel:setVisible(true)
		alreadyGotLabel:setVisible(false)
	end 
end

function ExchangeElfLayer:FreshRightPanel()	
	local id = self.ValidElfTable[self.SelectItemIndex]
	local isExist = LuaSpriteElfDataCenter:IsOneElfExistInElfDataTable(id)

	self:ShowElfOnLayer(id)
	self:ShowOneElfSkillInfo(id)
	self:ShowSkillLeftIcon(id)
	self:ShowExpLevelInfo(id)

	--Note: 隐藏兑换按钮
	local exchangeBtn = self.Layout:FindChildObjectByName("exchangeBtn").__UIButton__:getMenuItemSprite();
	local exchangeBtnTxt = self.Layout:FindChildObjectByName("exchange_text").__UIButton__
	if isExist then		
		exchangeBtn:setVisible(false)		
		exchangeBtnTxt:setVisible(false)
	else
		exchangeBtn:setVisible(true)		
		exchangeBtnTxt:setVisible(true)
	end
end

function ExchangeElfLayer:ShowLeftScrollList()
	self.ValidElfTable = GetExchangeAbleElfTabel()

	local scrollList = self.Layout:FindChildObjectByName("elfScrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end

	scrollList:clearChildrens()		

	for index,value in pairsByKeys(self.ValidElfTable) do
		self:InsertOneElfItem(scrollList,value)
	end
end

function onExchangeElfListItemClick(nTag)
	print("local function onListItemClick(nTag) " .. nTag);
	local tmpTag = nTag+1

	if tmpTag == ExchangeElfLayer.SelectItemIndex then
		local btn = ExchangeElfLayer.ElfListTable[ExchangeElfLayer.SelectItemIndex]
		btn:selected()
		return
	end

	ExchangeElfLayer.SelectItemIndex = tmpTag

	local id = ExchangeElfLayer.ValidElfTable[ExchangeElfLayer.SelectItemIndex]
	if id ~= nil then		
		local btn = ExchangeElfLayer.ElfListTable[ExchangeElfLayer.SelectItemIndex]
		if nil ~= ExchangeElfLayer.SelectedButton then
			ExchangeElfLayer.SelectedButton:unselected()
		end			
		btn:selected()
		ExchangeElfLayer.SelectedButton = btn
	else
		SpriteFactory:sharedFactory():ResetValue();
	end
	
	ExchangeElfLayer:FreshRightPanel()
end

function ExchangeElfLayer:InsertOneElfItem(scrollList,elfId)

		local backGroundFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_FRAME_FAIRYBAR";
		local childFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_FRAME_FAIRYICON";
		local normal = AspriteManager:getInstance():getOneFrame(KUI_BIN,childFrameName)
		childFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_FRAME_FAIRYICON_SELECTED"
		local clicked = AspriteManager:getInstance():getOneFrame(KUI_BIN,childFrameName)

		local button = scrollList:AddNewItem(normal,
			clicked,
			"",
			"",
			ccc3(255,255,255),
			-130);

		local menuItem = button:getMenuItemSprite();
		menuItem:registerScriptTapHandler(onExchangeElfListItemClick);

		local listItemNum = table.getn(self.ElfListTable)
		self.ElfListTable[listItemNum+1] = menuItem
		self.ElfListItemTable[listItemNum+1] = button

		local relativePoint = CCPoint(0,0);
		local anchorPoint = CCPoint(0.5,0.5);

		-- Note: 显示精灵精灵名称
		childFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_TEXT_FAIRYNAME";
		local ElfName = GetOneElfName(elfId)
		self:ShowOneLabel(backGroundFrameName,childFrameName,ElfName,28,ccc3(238,166,26),anchorPoint,button:getAnimationNode())

		-- Note: 显示精灵潜能名称
		childFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_TEXT_POTENTIAL";
		local txtFlag = "M_POTENTIAL"
		local contentTxt = Localization[txtFlag].SimpleChinese
		self:ShowOneLabel(backGroundFrameName,childFrameName,contentTxt,18,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),kCCTextAlignmentRight)

		-- Note: 显示精灵潜能数值
		childFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_NUMBER_POTENTIAL";		
		local ElfPotential = GetOneElfPotential(elfId)
		self:ShowOneLabel(backGroundFrameName,childFrameName,"+" .. ElfPotential,18,ccc3(255,255,255),anchorPoint,button:getAnimationNode())		

		--Note: 显示左侧头像
		childFrameName = "map_ui_FRAME_EXCHANGE_FAIRY_ICON_FAIRY";
		AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		local iconName = GetOneElfIcon(elfId)
		childFrameName = "map_ui_system_icon_FRAME_" .. iconName
		local elfLeftIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,childFrameName)
		elfLeftIcon:setPosition(relativePoint);
        elfLeftIcon:setVisible(true)
		button:getAnimationNode():addChild(elfLeftIcon,1000);
end

function ExchangeElfLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,bgName,uiName,anchorPoint,relativePoint);
	local labelSize = pLabelSprite:getContentSize();
	local contentSize = CCSize(labelSize.width,labelSize.height)
	if HorAligment == nil then
		HorAligment = kCCTextAlignmentLeft
	end
	if VerAligment == nil then
		VerAligment = kCCVerticalTextAlignmentCenter
	end
	local nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)
	nameLabel:setColor(color)
	nameLabel:setPosition(relativePoint)
	parentNode:addChild(nameLabel,5)
	return nameLabel
end

function ExchangeElfLayer:ShowExploerExp()
	if nil ~= self.exploerExpContentLabel then
		local exploerExp = UserData:GetUserInfo().m_playerExploerExperience
		local exploerExpContent = MainMenuLayer:GetNumByFormat(exploerExp)
		self.exploerExpContentLabel:setString("" .. exploerExpContent)
	end	
end


------------------------------------------------------------------------------------------
function ExchangeElfLayerCloseBtnClick(pObj)
	if nil ~= ExchangeElfLayer.LayerInstance then
		SetChatBar(true,-1);
		RemoveOneLayer(ExchangeElfLayer.LayerInstance)
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		ExchangeElfLayer.LayerInstance:removeFromParentAndCleanup(true);
		ExchangeElfLayer.LayerInstance = nil
		ExchangeElfLayer:ResetValue()	
		SpriteFactory:sharedFactory():ResetValue();
	end
end

function ExchangeElfLayerExchangeBtnClick(pObj)
	print("function ExchangeElfLayerExchangeBtnClick(pObj)")
	local id = ExchangeElfLayer.ValidElfTable[ExchangeElfLayer.SelectItemIndex]
	local isExist = LuaSpriteElfDataCenter:IsOneElfExistInElfDataTable(id)
	if false == isExist then
		ElfExploreDataCenter:SendExchangeElfMessagge(id)
	end	
end

function BackToElfExploerBtnClick(pObj)
	ExchangeElfLayerCloseBtnClick(nil)
	--ElfExploerLayer:CreateLayer(nil)
end

local function closeCallBack(message)
	if nil ~= ExchangeElfLayer.LayerInstance then
		if G_CurLayerInstance ~= ExchangeElfLayer.LayerInstance then
			ExchangeElfLayerCloseBtnClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		ExchangeElfLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	elseif message == GM_ATTR_PLAYER_EXPLOER_EXP then
		ExchangeElfLayer:ShowExploerExp()
	end
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);