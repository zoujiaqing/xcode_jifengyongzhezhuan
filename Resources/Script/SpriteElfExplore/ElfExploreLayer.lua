require("Script/ChatToolLayer")
require("Script/CommonDefine")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/SpriteElfExplore/ExchangeElfLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/DailyTask/dailyTaskDataCenter")

ElfExploerLayer = {
	LayerInstance = nil,
	Layout = nil,
	eventContainer = nil,
	iconContentLabel = nil,
	diamondContentLabel = nil,
	expContentLabel = nil,
	exploerExpContentLabel = nil ,
	elfMapStatus = 0 
}

function ElfExploerLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.eventContainer = nil
	self.iconContentLabel = nil
	self.diamondContentLabel = nil
	self.expContentLabel = nil
	self.exploerExpContentLabel = nil
	self.elfMapStatus = 0 
end

local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("ElfExploerPanel")
	ElfExploerLayer:ResetValue()
end

--Note: 创建Layer
function ElfExploerLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ElfExploerPanel.plist",self.LayerInstance, "ElfExploerPanel", true);
	self.Layout = uiLayout

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["ElfExploerLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	self:ShowLeftScrollList()
	self:ShowEventPanel(false)

	PushOneLayer(self.LayerInstance,"","")	
	
	self:FreshRightPanel();
	self:ShowExploreNumsInfoPanel()
	SetChatBar(false,-1);
end

local function onHelpBtClicked(obj)
	setRecentHelpIndex("ElfExploerLayer")
	showUILayerByTag(UITagTable["HelpUILayer"].tag,true)
end


function ElfExploerLayer:InitLayer()

 if ElfExploreDataCenter.ExploreLeftTimes == 0 then
	  self.elfMapStatus = 0 
 end
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(ElfExploerLayerCloseBtnClick);

	local freshBtn = self.Layout:FindChildObjectByName("freshBtn").__UIButton__:getMenuItemSprite();
	freshBtn:registerScriptTapHandler(ElfExploerFreshBtnClick);

	local goBtn = self.Layout:FindChildObjectByName("exploerBtn").__UIButton__:getMenuItemSprite();
	goBtn:registerScriptTapHandler(ElfExploerGoBtnClick);

	local choseBtn_1 = self.Layout:FindChildObjectByName("choice_Btn_1").__UIButton__:getMenuItemSprite();
	choseBtn_1:registerScriptTapHandler(ElfExploerChoiceBtnClick_1);

	local choseBtn_2 = self.Layout:FindChildObjectByName("choice_Btn_2").__UIButton__:getMenuItemSprite();
	choseBtn_2:registerScriptTapHandler(ElfExploerChoiceBtnClick_2);

	local helpBt = self.Layout:FindChildObjectByName("helpBt").__UIButton__:getMenuItemSprite()
	helpBt:registerScriptTapHandler(onHelpBtClicked)

	self.eventContainer = self.Layout:FindChildObjectByName("eventPanelContainer").__UIContainer__;
	if nil ~= self.eventContainer then
		self.eventContainer:setVisible(false);
	end

	local fairyEcountInfoLabel = self.Layout:FindChildObjectByName("fairyEncountContent").__UILabel__;
	local flag = "ELF_CAN_MEET";
	local encountContent = Localization[flag].SimpleChinese
	fairyEcountInfoLabel:setString(encountContent)

	--Note: 底部信息面板
	local downContainer = self.Layout:FindChildObjectByName("commonDownContainer").__UIContainer__;
	self.iconContentLabel = downContainer:FindChildObjectByName("money_num").__UILabel__;
	self.diamondContentLabel = downContainer:FindChildObjectByName("diamond_num").__UILabel__;
	--self.expContentLabel = downContainer:FindChildObjectByName("experience_num").__UILabel__;
	--self.exploerExpContentLabel = downContainer:FindChildObjectByName("Experience_label").__UILabel__;
	--local exchangeElfBtn = downContainer:FindChildObjectByName("exchangeBtn").__UIButton__;
	--local exchangeElfBtnMenuItem = exchangeElfBtn:getMenuItemSprite();
	--exchangeElfBtnMenuItem:registerScriptTapHandler(ElfExchangeBtnClick);
	--local exchangeIconName = "map_ui_system_icon_FRAME_ICON_EXCHANGE_FAIRY";
	--local exchangeIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,exchangeIconName);
	--exchangeElfBtn:setIconSprite(exchangeIcon)
	--Note: 刷新信息面板	
	self:ShowMoney()
	self:ShowExp()
	self:ShowDiamond()
	--self:ShowExploerExp()

	--CCLuaLog(string.format("winSize: %B",elfMapStatus));
	
end

function ElfExploerLayer:FreshRightPanel()
	if 0 ~= ElfExploreDataCenter.MapId then
		local mapId = ElfExploreDataCenter.MapId

		local event_id,elfId = ElfExploreDataCenter:GetMapElfIdAndEventId(mapId)	
		
		--Note: 显示地图名称
		local mapName = ElfExploreDataCenter:GetOneMapName(mapId)
		local mapNameLabel = self.Layout:FindChildObjectByName("exploer_map_name").__UILabel__;	
		mapNameLabel:setString(mapName)
		local freshBtn = self.Layout:FindChildObjectByName("freshBtn").__UIButton__:getMenuItemSprite();
	    print("ElfExploerLayer:FreshRightPanel"..ElfExploreDataCenter.ExploreLeftTimes)
		if nil ~= event_id then
		
			if ElfExploreDataCenter.ExploreLeftTimes == 0 then
					print("ElfExploreDataCenter.ExploreLeftTimes == 0 ")
				  elfMapStatus =0
				  freshBtn:setEnabled(true);
			else
			   print("ElfExploreDataCenter.ExploreLeftTimes != 0")
				elfMapStatus =1 
				freshBtn:setEnabled(false);

			end
		   
			mapNameLabel:setColor(ccc3(255,144,0))
		else
		    elfMapStatus =0
			freshBtn:setEnabled(true);
			mapNameLabel:setColor(ccc3(255,255,255))
		end


		--Note: 显示地图Icon
		local mapIconName = ElfExploreDataCenter:GetOneMapIconName(mapId)
		local mapIconPic = self.Layout:FindChildObjectByName("exploer_map_icon").__UIPicture__;	
		local mapIconFramePic = self.Layout:FindChildObjectByName("exploer_map_frame_icon").__UIPicture__;
		local mapIconSprite = CCSprite:create(mapIconName)
		mapIconPic:setSprite(mapIconSprite)

		--Note: 显示精灵		
		local container = self.Layout:FindChildObjectByName("ElfExploerContainer").__UIContainer__;
		SpriteFactory:sharedFactory():RemoveElfFromParent(container:getCurrentNode())
		if nil ~= event_id then
			if mapIconPic ~= nil then	
				--local px,py = mapIconPic:getCurrentNode():getPosition()
				local size = mapIconPic:getCurrentNode():getContentSize()		
				local px = size.width / 2
				local py = size.height / 2
				local modePt = CCPoint(px,py)								
				SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(mapIconPic:getCurrentNode(),elfId,modePt)
			end
		end

		local FrameSize = mapIconFramePic:getSpriteSize();
		local SpriteSize = mapIconSprite:getContentSize();
		mapIconSprite:setScaleX(FrameSize.width / SpriteSize.width);
		mapIconSprite:setScaleY(FrameSize.width / SpriteSize.width);
	end
end

function ElfExploerLayer:ShowExploreNumsInfoPanel()
	--Note: 显示今日还能够云游的次数
	if nil == self.Layout then
		return
	end

	local leftTimeLabel = self.Layout:FindChildObjectByName("exploer_tip_info").__UILabel__;
	if ElfExploreDataCenter.ExploreLeftTimes == 0 then
		--local tipContentLabel = self.Layout:FindChildObjectByName("exploer_tip_txt_info").__UILabel__;
		--local tipFlag = "EXPLOER_Common_content_2"
		--tipContentLabel:setString(Localization[tipFlag].SimpleChinese)
		leftTimeLabel:setString("0")
		--Note: 隐藏云游和刷新按钮
		local freshBtn = self.Layout:FindChildObjectByName("freshBtn").__UIButton__:getMenuItemSprite();
		freshBtn:setVisible(false);
		local goBtn = self.Layout:FindChildObjectByName("exploerBtn").__UIButton__:getMenuItemSprite();
		goBtn:setVisible(false);
		local freshLabel = self.Layout:FindChildObjectByName("exploer_fresh_btn_txt").__UILabel__;
		local goLabel = self.Layout:FindChildObjectByName("exploer_go_btn_txt").__UILabel__;
		freshLabel:setVisible(false);
		goLabel:setVisible(false);
	else
		local tipInfo = "" .. ElfExploreDataCenter.ExploreLeftTimes
		leftTimeLabel:setString(tipInfo)
	end
end
function ElfExploerLayer:ShowExploreResfashNumsInfoPanel()
   print("ElfExploerLayer:ShowExploreResfashNumsInfoPanel")
   if ElfExploreDataCenter.ExploreLeftTimes == 0 then
	  self.elfMapStatus = 0 
	  local freshBtn = self.Layout:FindChildObjectByName("freshBtn").__UIButton__:getMenuItemSprite();
	  freshBtn:setEnabled(true)
   end

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
    elfLeftIcon:setVisible(true)
	button:getAnimationNode():addChild(elfLeftIcon,1000);

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
	local iconPicName_1 = "map_ui_system_icon_FRAME_ICON_REWARD"
	local iconPicName_2 = "map_ui_system_icon_FRAME_ICON_REWARD"

	local event_id,elfId = ElfExploreDataCenter:GetMapElfIdAndEventId(ElfExploreDataCenter.MapId)	

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
		--local diamondContent = MainMenuLayer:GetNumByFormat(diamond)
		self.diamondContentLabel:setString("" .. diamond)
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
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		ElfExploerLayer.LayerInstance:removeFromParentAndCleanup(true);
		ElfExploerLayer.LayerInstance = nil
		ElfExploerLayer:ResetValue()
		SpriteFactory:sharedFactory():ResetValue();
	end
end

local function DaimondLayerFreshOkBtnClick(pObj)
	local leftTime = ElfExploreDataCenter.ExploreLeftTimes
	if leftTime <= 0 then
		local content = "Today Over "
		GameApi:showMessage(content)
	else
		ElfExploreManger:Get():SendExploreReq(2)
		PushOneWaitingLayer("CSExploreReq")		
	end
end

local function DaimondLayerFreshCancelBtnClick(pObj)
end

function ElfExploerFreshBtnClick(pObj)
	print("function ElfExploerFreshBtnClick(pObj)")

	local diamondNum = dailyTaskDataCenter:GetDailyTaskNeedDiamondByExchangeID(20300)
	local languageFlag = 'Dailytask_4011_Name'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	languageFlag = "EXPLOER_TIPS1"
	local tip_content = LanguageLocalization:GetLocalization(languageFlag)
	DiamondWarningLayer:Show(content,diamondNum,DaimondLayerFreshOkBtnClick,DaimondLayerFreshCancelBtnClick,ccc3(255,255,255),"",tip_content)
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
		ElfExploerLayer.elfMapStatus = 0 ;
	    local freshBtn = ElfExploerLayer.Layout:FindChildObjectByName("freshBtn").__UIButton__:getMenuItemSprite();
	    freshBtn:setEnabled(true);
		ElfExploerLayer:ShowEventPanel(true,eventContent,choiceContent_1,choiceContent_2)
	end
end

function ElfExploerChoiceBtnClick_1(pObj)
	print("function ElfExploerChoiceBtnClick_1(pObj)")
	ElfExploerLayer:ShowEventPanel(false)
	ElfExploreManger:Get():SendExploreDetermineReq(1)
	PushOneWaitingLayer("CSExploreDetermineReq")	
end

function ElfExploerChoiceBtnClick_2(pObj)
	print("function ElfExploerChoiceBtnClick_2(pObj)")
	ElfExploerLayer:ShowEventPanel(false)
	ElfExploreManger:Get():SendExploreDetermineReq(2)
	PushOneWaitingLayer("CSExploreDetermineReq")
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
		--ElfExploerLayer:ShowExploerExp()
	end
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);