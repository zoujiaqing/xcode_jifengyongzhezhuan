require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/SpriteElfExplore/ExchangeElfLayer")

ManorLayer = {
	LayerInstance = nil,
	Layout = nil,
}

function ManorLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
end

--Note: 创建Layer
function ManorLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ElfExploerPanel.plist",self.LayerInstance, "ElfExploerPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		mainMenuNode:addChild(self.LayerInstance)
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	self:ShowLeftScrollList()
	self:ShowEventPanel(false)

	PushOneLayer(self.LayerInstance,"","")
end

function ElfExploerLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(ElfExploerLayerCloseBtnClick);
end

function ElfExploerLayer:ShowLeftScrollList()

	local scrollList = self.Layout:FindChildObjectByName("elfScrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end

	scrollList:clearChildrens()		

	local validElf = ElfExploreDataCenter:GetValidElf()
	for index,value in pairsByKeys(validElf) do
		self:InsertOneSkillItem(scrollList,value)
	end
end

function ElfExploerLayer:InsertOneSkillItem(scrollList,elfId)
	if nil == scrollList then
		return 
	end

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	-- Note: List Item 
	local normalListItemIconBgName = "map_ui_FRAME_EXPLORE_FRAME_FAIRYICON";
	local clickListItemIconBgName = "map_ui_FRAME_EXPLORE_FRAME_FAIRYICON";
	local normal = AspriteManager:getInstance():getOneFrame(KUI_BIN,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(KUI_BIN,clickListItemIconBgName)
		
	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);

	--Note: 技能图标
	local backGroundFrameName = "map_ui_FRAME_EXPLORE_FRAME_FAIRYBAR"
	self:ShowOneListItem(backGroundFrameName,button,elfId);
end

function ElfExploerLayer:ShowOneListItem(backGroundFrameName,button,elfId)
	--Note: 显示精灵头像
	local anchorPoint = ccp(0.5,0.5)
	local relativePoint = ccp(0,0)
	local childFrameName = "map_ui_FRAME_EXPLORE_ICON_FAIRY";
	AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	local iconName = GetOneElfIcon(elfId)
	childFrameName = "map_ui_system_icon_FRAME_" .. iconName
	local elfLeftIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,childFrameName)
	elfLeftIcon:setPosition(relativePoint);
	button:getAnimationNode():addChild(elfLeftIcon);

	--Note: 精灵名称
	childFrameName = "map_ui_FRAME_EXPLORE_TEXT_FAIRYNAME"
	local ElfName = GetOneElfName(elfId)
	self:ShowOneLabel(backGroundFrameName,childFrameName,ElfName,18,ccc3(255,255,0),anchorPoint,button:getAnimationNode(),1,1001)

	--Note: 精灵描述
	childFrameName = "map_ui_FRAME_EXPLORE_TEXT_FAIRYSKILL"
	local SkillDescription = Localization['M_SKILL'].SimpleChinese .. GetOneElfSkillName(elfId)
	self:ShowOneLabel(backGroundFrameName,childFrameName,SkillDescription,18,ccc3(0,255,0),anchorPoint,button:getAnimationNode(),1,1002)

	--Note: 出现地点
	childFrameName = "map_ui_FRAME_EXPLORE_TEXT_PLACE"
	self:ShowOneLabel(backGroundFrameName,childFrameName,Localization['S_Where_content'].SimpleChinese .. ":",18,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),1,1003)

	--Note: 出现地点
	childFrameName = "map_ui_FRAME_EXPLORE_TEXT_FAIRYPLACE"
	local existWhere = GetOneElfExistWhere(elfId)
	self:ShowOneLabel(backGroundFrameName,childFrameName,existWhere,18,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),1,1004)
end

function ElfExploerLayer:ShowEventPanel(bShow,eventContent,choiceContent_1,choiceContent_2)
	if nil == self.eventContainer then
		return
	end

	self.eventContainer:setVisible(bShow)

	local eventContentLabel = self.eventContainer:FindChildObjectByName("exploer_event_content_label").__UILabel__;
	local choiceContentLabel_1 = self.eventContainer:FindChildObjectByName("exploer_choice_content_1").__UILabel__;
	local choiceContentLabel_2 = self.eventContainer:FindChildObjectByName("exploer_choice_content_2").__UILabel__;

	if false == bShow then
		return
	end

	eventContentLabel:setString(eventContent)
	choiceContentLabel_1:setString(choiceContent_1)
	choiceContentLabel_2:setString(choiceContent_2)

	--Note: 显示ICon
	local iconPicName_1 = "map_ui_system_icon_FRAME_ICON_FAIRYSKILL_3011"
	local iconPicName_2 = "map_ui_system_icon_FRAME_ICON_FAIRYSKILL_3011"
	local event_id,elfId = ElfExploreDataCenter:GetMapElfIdAndEventId(ElfExploreDataCenter.MapId)

	print("step 1 " .. elfId)

	local iconFramePic_1 = self.eventContainer:FindChildObjectByName("goldPacker_1").__UIPicture__;
	local iconFramePic_2 = self.eventContainer:FindChildObjectByName("goldPacker_2").__UIPicture__;

	if 1 == event_id then
		local iconName = GetOneElfIcon(elfId)
		iconPicName_1 = "map_ui_system_icon_FRAME_" .. iconName
	elseif 2 == event_id then
		local iconName = GetOneElfIcon(elfId)
		iconPicName_2 = "map_ui_system_icon_FRAME_" .. iconName		
	end
	
	local iconPic_1 = AspriteManager:getInstance():getOneFrame(KICON_BIN,iconPicName_1);
	iconFramePic_1:setSprite(iconPic_1)
	local iconPic_2 = AspriteManager:getInstance():getOneFrame(KICON_BIN,iconPicName_2);
	iconFramePic_2:setSprite(iconPic_2)
end

--Note: 显示文本
function ElfExploerLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
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
	local nameLabel = parentNode:getChildByTag(nTag)
	if nil == nameLabel then
		nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)
		nameLabel:setTag(nTag)
		parentNode:addChild(nameLabel,zOrder)
	end	
	nameLabel:setString(content)
	nameLabel:setColor(color)
	nameLabel:setPosition(relativePoint)
	if zOrder == nil then
		zOrder = 0
	end
	
	return nameLabel
end

function ElfExploerLayer:ShowMoney()
	if nil ~= self.iconContentLabel then
		local money = UserData:GetUserInfo().m_gold
		local moneyContent = MainMenuLayer:GetNumByFormat(money)
		self.iconContentLabel:setString("" .. moneyContent)
	end
end

function ElfExploerLayer:ShowExp()
	if nil ~= self.expContentLabel then
		local exploerExp = UserData:GetUserInfo().m_playerExploerExperience
		local exploerExpContent = MainMenuLayer:GetNumByFormat(exploerExp)
		self.expContentLabel:setString("" .. exploerExpContent)
	end
end

function ElfExploerLayer:ShowDiamond()
	if nil ~= self.diamondContentLabel then
		local diamond = UserData:GetUserInfo().m_diamond
		local diamondContent = MainMenuLayer:GetNumByFormat(diamond)
		self.diamondContentLabel:setString("" .. diamondContent)
	end
end

function ElfExploerLayer:ShowExploerExp()
	if nil ~= self.exploerExpContentLabel then
		--local exploerExp = UserData:GetUserInfo().m_playerExploerExperience
		--local exploerExpContent = MainMenuLayer:GetNumByFormat(exploerExp)
		self.exploerExpContentLabel:setString(Localization['ELF_Exhange_Content'].SimpleChinese)
	end
end

-------------------事件回调------------------------------------------------
function ElfExploerLayerCloseBtnClick(pObj)	
	if nil ~= ElfExploerLayer.LayerInstance then
		SetChatBar(true,-1);
		RemoveOneLayer(ElfExploerLayer.LayerInstance)
		TXGUI.UIManager:sharedManager():removeUILayout("ElfExploerPanel")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		ElfExploerLayer.LayerInstance:removeFromParentAndCleanup(true);
		ElfExploerLayer.LayerInstance = nil
		ElfExploerLayer:ResetValue()
	end
end

function ElfExploerFreshBtnClick(pObj)
	print("function ElfExploerFreshBtnClick(pObj)")

	local leftTime = ElfExploreDataCenter.ExploreLeftTimes
	if leftTime <= 0 then
		local content = "Today Over "
		GameApi:showMessage(content)
	else
		ElfExploreManger:Get():SendExploreReq(2)
	end
end

function ElfExploerGoBtnClick(pObj)
	print("function ElfExploerGoBtnClick(pObj)")

	local leftTime = ElfExploreDataCenter.ExploreLeftTimes
	if leftTime <= 0 then
		local content = "Today Over "
		GameApi:showMessage(content)
	else
		local mapId = ElfExploreDataCenter.MapId
		local eventContent = ElfExploreDataCenter:GetOneMapIconEventContent(mapId)
		local choiceContent_1 = ElfExploreDataCenter:GetOneMapIconChoiceContent_1(mapId)
		local choiceContent_2 = ElfExploreDataCenter:GetOneMapIconChoiceContent_2(mapId)

		ElfExploerLayer:ShowEventPanel(true,eventContent,choiceContent_1,choiceContent_2)
	end
end

function ElfExploerChoiceBtnClick_1(pObj)
	print("function ElfExploerChoiceBtnClick_1(pObj)")
	ElfExploerLayer:ShowEventPanel(false)
	ElfExploreManger:Get():SendExploreDetermineReq(1)
end

function ElfExploerChoiceBtnClick_2(pObj)
	print("function ElfExploerChoiceBtnClick_2(pObj)")
	ElfExploerLayer:ShowEventPanel(false)
	ElfExploreManger:Get():SendExploreDetermineReq(2)
end

function ElfExchangeBtnClick(pObj)
	print("function ElfExchangeBtnClick(pObj) click ")
	ElfExploerLayerCloseBtnClick(nil)
	ExchangeElfLayer:CreateLayer(nil)
end

function ElfExploerLayer:RewardCommit(content)
	if content ~= "" then
		GameApi:showMessage(content)
	end	
end


---------------------------------------------------------------------------
local function closeCallBack(message)
	if nil ~= ElfExploerLayer.LayerInstance then
		if G_CurLayerInstance ~= ElfExploerLayer.LayerInstance then
			ElfExploerLayerCloseBtnClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		ElfExploerLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	elseif message == GM_ATTR_COIN_UPDATE then
		ElfExploerLayer:ShowMoney()
	elseif message == GM_ATTR_SP_DIAMOND_UPDATE then
		ElfExploerLayer:ShowDiamond()
	elseif message == GM_ATTR_EXP_UPDATE then
		ElfExploerLayer:ShowExp()
	elseif message == GM_ATTR_PLAYER_EXPLOER_EXP then
		ElfExploerLayer:ShowExploerExp()
	end
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);