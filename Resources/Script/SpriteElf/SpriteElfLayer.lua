--File : SpriteElfLayer.lua
require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/GameConfig/uiTagDefine")
require("Script/SpriteElf/ElfConfigParse")
require("Script/SpriteElf/SpriteElfStrengthenLayer")
require("Script/ChatToolLayer")
require("Script/SpriteElf/SpriteElfDataCenter")

--Note: Used For Create Layer
G_SpriteElfLayer = nil
G_AttributeLabels = {}
G_SumAttributeLabels = {}
G_exloperElfNeedTaskId = 503042
G_exloperElfNeedHeroLevel = 21

local function notificationCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then		
		if nil ~= G_SpriteElfLayer then
			if G_CurLayerInstance ~= G_SpriteElfLayer then
				OnElfLayerCloseEvent(nil)
			end			
		end
	elseif message == GM_ATTR_REPUTATION_UPDATE then

	elseif message == GM_E_ENTER_MAINLAND then
		--print("============================= ShowNewElfTipIconOrNot ====================== ");
		ShowNewElfTipIconOrNot()
	end
end
	
function CreateSpriteElfLayer()
	G_SpriteElfLayer = CCLayer:create()	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/SpriteElfPanel.plist",G_SpriteElfLayer, "SpriteElfPanel", true);	
	local mainMenuNode = GetMainMenuLayerNode()
	local tag = UITagTable["SpriteElfPanel"].tag
	mainMenuNode:addChild(G_SpriteElfLayer,10,tag)
	
	InitElfLayer(G_SpriteElfLayer)
	InitSpriteElfLayer()
	SetChatBar(false,-1)
	
	OnlineNetworkManager:sShareInstance():sendGetTaskStatusMessage(G_exloperElfNeedTaskId);

	PushOneLayer(G_SpriteElfLayer,"","")	
end	

function OnElfLayerCloseEvent(pObj)
	TXGUI.UIManager:sharedManager():removeUILayout("SpriteElfPanel")
	SetChatBar(true,-1)
	CCTextureCache:sharedTextureCache():removeUnusedTextures()

	RemoveOneLayer(G_SpriteElfLayer)
	
	G_SpriteElfLayer:removeFromParentAndCleanup(true);
	SpriteFactory:sharedFactory():ResetValue();
	G_SpriteElfLayer = nil

	ShowNewElfTipIconOrNot()
end

function ClearElfLayerAllData()
    InitAllData()
	G_FightElfID = 0
	LuaSpriteElfDataCenter:RevertData()
end

local function onHelpBtClicked(obj)
	setRecentHelpIndex("SpriteElfLayer")
	showUILayerByTag(UITagTable["HelpUILayer"].tag,true)
end

----------------------------------------
local function onExploerElfBtClicked(obj)
	--local isConditionValid = true
--
	--local unlockLevel = G_exloperElfNeedHeroLevel
	--local heroLevel = UserData:GetUserLevel()
	--if heroLevel < unlockLevel then
		--isConditionValid = false
	--else
		--local unLockTaskId = G_exloperElfNeedTaskId
		--if nil ~= unLockTaskId then
			--local nTaskState = SystemConditionManager:Get():GetTaskStatus(unLockTaskId)
			----Note: 任务没有接
			--if nTaskState == 0 then
				--isConditionValid = false
			--end
		--end		
	--end
--
	--if isConditionValid then
		--GameAudioManager:sharedManager():playEffect(350001,false);
		--ElfExploerLayer:CreateLayer(nil)
	--else
		--local languageFlag = 'M_UNFINISH'
		--local content = LanguageLocalization:GetLocalization(languageFlag)
		--GameApi:showMessage(content)	
	--end
end

--Note: UI Logic Parts
--Note: Start Function
function InitSpriteElfLayer()

	InitAllData()
	--InitChatToolLayerPanel()
	
	g_layout = TXGUI.UIManager:sharedManager():getUILayout("SpriteElfPanel");	
	local layer = g_layout:getCurrentNode():getParent()
	
	g_fightBtn = g_layout:FindChildObjectByName("battleBtn").__UIButton__:getMenuItemSprite();
	g_fightBtn:registerScriptTapHandler(GoToFightBtnClick);	
	g_fightLable = g_layout:FindChildObjectByName("battle_text").__UILabel__;
	 
	g_skillNameLabel = g_layout:FindChildObjectByName("skill_name_text").__UILabel__;
	g_skillDescriptionLabel = g_layout:FindChildObjectByName("skill_description_text").__UILabel__;	
	g_frameFairySkillPic = g_layout:FindChildObjectByName("frameFairySkill").__UIPicture__;
	
	g_closeBtn = g_layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	g_closeBtn:registerScriptTapHandler(OnElfLayerCloseEvent);	

	local helpBt = g_layout:FindChildObjectByName("helpBt").__UIButton__:getMenuItemSprite()
	helpBt:registerScriptTapHandler(onHelpBtClicked)

	--local exploerElfBtn = g_layout:FindChildObjectByName("exploerElfBt").__UIButton__:getMenuItemSprite()
	--exploerElfBtn:registerScriptTapHandler(onExploerElfBtClicked)

	g_fairyAttributeContainer = g_layout:FindChildObjectByName("attributePanelContainer").__UIContainer__
	g_fairyAttributeContainer:setVisible(false)

	local loopUpAttributesBtn = g_layout:FindChildObjectByName("attributeBtn").__UIButton__:getMenuItemSprite();
	loopUpAttributesBtn:registerScriptTapHandler(OnLookUpAllAttributeBtnEvent);	

	local loopUpAttributesBtnLabel = g_layout:FindChildObjectByName("attribute_btn_text").__UILabel__;
	loopUpAttributesBtnLabel:setString(Localization["M_LOOK_ALLATTRIBUTES"].SimpleChinese)

	--Note:属性面板
	local attributesPanelOkBtn = g_fairyAttributeContainer:FindChildObjectByName("lookUpOkBtn").__UIButton__:getMenuItemSprite();
	attributesPanelOkBtn:registerScriptTapHandler(OnAttributesPanelOkBtnEvent);	

	local attributesPanelOkBtnLabel = g_fairyAttributeContainer:FindChildObjectByName("lookUpBtnLabel").__UILabel__;
	attributesPanelOkBtnLabel:setString(Localization["M_CONFIRM"].SimpleChinese)

	local attributesPanelTitleLabel = g_fairyAttributeContainer:FindChildObjectByName("lookUpTitleLabel").__UILabel__;
	attributesPanelTitleLabel:setString(Localization["M_ELF_GAINS"].SimpleChinese)

	local attributesPanelBgPic = g_fairyAttributeContainer:FindChildObjectByName("lookUpBg").__UIPicture__;
	if nil ~= attributesPanelBgPic then
		local path = GameResourceManager:sharedManager():storedFullPathFromRelativePath("UI/ui_card_attribute.png")
		local sprite = CCSprite:create(path)
		attributesPanelBgPic:setSprite(sprite)
	end

	-- 特训按钮
	local trainBtn = g_layout:FindChildObjectByName("trainButton").__UIButton__;
	local trainBtnMenuItem = trainBtn:getMenuItemSprite();
	local potentialLabel = g_layout:FindChildObjectByName("trainNumPotentialLabel").__UILabel__;	
	local sumPotential = LuaSpriteElfDataCenter:GetTotalPotential()
	potentialLabel:setString("" .. sumPotential)	
	trainBtnMenuItem:registerScriptTapHandler(ChatToolTrainButtonClick);

	G_AttributeLabels = {}
	local attributeLabelCount = LuaSpriteElfDataCenter.AttributeLabelCount
	for index = 1,attributeLabelCount,1 do
		local labelName = "attribute_txt_" .. index
		local label = g_layout:FindChildObjectByName(labelName).__UILabel__;
		G_AttributeLabels[index] = label
	end

	G_SumAttributeLabels = {}
	local sumAttributeLabelCount = LuaSpriteElfDataCenter.SumAttributeLabelCount
	for index = 1,sumAttributeLabelCount,1 do
		local labelName = "lookUpAtt_" .. index
		local label = g_fairyAttributeContainer:FindChildObjectByName(labelName).__UILabel__;
		label:setColor(ccc3(36,255,0))
		G_SumAttributeLabels[index] = label
	end
	
	ShowElfBgPic(g_layout)
	ShowElfList(g_layout)
	
	return true
end

--Note: 隐藏右侧面板
function HideRightParts()
	g_fightBtn:setVisible(false)
	g_fightLable:setVisible(false)
	g_skillNameLabel:setVisible(false)
	g_skillDescriptionLabel:setVisible(false)
	g_frameFairySkillPic:setVisible(false)
end

--[[
Instruction : Show Elf Equip BackGround Picture
@param 
@result 
--]]
function ShowElfBgPic(layout)
	local elfBgPic = layout:FindChildObjectByName("frameDecoration").__UIPicture__
	if nil ~= elfBgPic then
		local path = GameResourceManager:sharedManager():storedFullPathFromRelativePath("UI/ui_fairy.png")
		--print("rcs path " .. path)
		local sprite = CCSprite:create(path)
		elfBgPic:setSprite(sprite)
	end
end

--[[
Instruction : Show Elf Model On Bg
@param 
@result 
--]]
function ShowElfOnLayer(layout,typeId)
	local container = layout:FindChildObjectByName("SpriteElfContainer").__UIContainer__;
	local elfBgPic = layout:FindChildObjectByName("frameShowFairy").__UIPicture__
	if elfBgPic ~= nil then	
		local size = elfBgPic:getCurrentNode():getContentSize()		
		--local scale = TXGUI.UIManager:sharedManager():getScaleFactor()
		local px = size.width / 2
		local py = size.height / 2
		local modePt = CCPoint(px,py)
		SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(elfBgPic:getCurrentNode(),typeId,modePt)
	end
end

function InitAllData()
	g_selectedButton = nil
	g_SelectItemIndex = -1
	--G_FightElfID = 0
	g_ElfListTable = {}
    g_ElfListItemTable = {}
end

--[[
Instruction : Create And Show List
@param 
@result 
--]]
function ShowElfList(layout)
	if nil ~= layout then		
		
		scrollList = layout:FindChildObjectByName("elfScrollList").__UIScrollList__
		scrollList:clearChildrens()		
				
		if nil ~= LuaSpriteElfDataCenter.Items then			
			for index,value in pairsByKeys(LuaSpriteElfDataCenter.Items) do
				local itemState = value.elfState
				local tmpElfIndex = value.elfId
				AddNewListItem(scrollList,tmpElfIndex,itemState)
				if itemState == 3 then
					LuaSpriteElfDataCenter.battleFairyIndex = index
				end
			end						
		end	
				
		SelectDefaultListItem()
	end	
end

--Note: 显示是否有每日奖励可以领取
function ShowNewElfTipIconOrNot(bShow)
	local bShowOrNot = bShow
	if nil == bShowOrNot then
		bShowOrNot = LuaSpriteElfDataCenter:IsHasNewStateElf()
	end	
	
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local fairyButton = nil
	if nil ~= topLayout then
		fairyButton = topLayout:FindChildObjectByName("button_fairy").__UIButton__
	end

	if fairyButton then
		if bShowOrNot then
			fairyButton:showTipIcon()
		else
			fairyButton:hideTipIcon()
		end
	end
end

--[[
Instruction : Add New Item To List
@param listItemState 1 Normal 2 Lock
@param elfState 1 waiting 2 new 3 fight 4 lock
@result 
--]]
g_ElfListTable = {}
g_ElfListItemTable = {}

backGroundFrameName = "map_ui_FRAME_FAIRY_FRAME_FAIRYBAR";

function AddNewListItem(scrollList,elfId,itemState)
	
		backGroundFrameName = "map_ui_FRAME_FAIRY_FRAME_FAIRYBAR";
		local pBGFrame = AspriteManager:getInstance():getOneFrame(KUI_BIN,backGroundFrameName);
		local bgFrameSize = pBGFrame:getContentSize();		

		-- Note: Frame Elf Nor BackGround
		childFrameName = "map_ui_FRAME_FAIRY_FRAME_FAIRYICON";
		local normal = AspriteManager:getInstance():getOneFrame(KUI_BIN,childFrameName)
		childFrameName = "map_ui_FRAME_FAIRY_FRAME_FAIRYICON_SELECTED";
		local clicked = AspriteManager:getInstance():getOneFrame(KUI_BIN,childFrameName)

		if itemState == 0 then
			childFrameName = "map_ui_FRAME_FAIRY_FRAME_FAIRYICON_LOCKED";
			normal = AspriteManager:getInstance():getOneFrame(KUI_BIN,childFrameName)
			clicked = AspriteManager:getInstance():getOneFrame(KUI_BIN,childFrameName)
		end 
		
		local button = scrollList:AddNewItem(normal,
			clicked,
			"",
			"",
			ccc3(255,255,255),
			-130);
			
		local menuItem = button:getMenuItemSprite();
		menuItem:registerScriptTapHandler(onListItemClick);
		
		if nil == g_ElfListTable then
			g_ElfListTable = {}
		end
		
		if nil == g_ElfListItemTable then
			g_ElfListItemTable = {}
		end
		
		local listItemNum = table.getn(g_ElfListTable)
		g_ElfListTable[listItemNum+1] = menuItem		
		g_ElfListItemTable[listItemNum+1] = button

		local relativePoint = CCPoint(0,0);
		local anchorPoint = CCPoint(0.5,0.5);
		local childFrameName = "";

		childFrameName = "map_ui_FRAME_FAIRY_FRAME_FAIRYICON";
		AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		menuItem:setPosition(relativePoint);

		if itemState == 1 then
			-- Note: nothing do
		elseif itemState == 2 then
			-- Note: Frame Flag New BackGround
			childFrameName = "map_ui_FRAME_FAIRY_ICON_NEW";
			pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
			pChildSprite:setPosition(relativePoint);
			button:getAnimationNode():addChild(pChildSprite,0,1);
		elseif itemState == 3 then
			-- Note: Frame Flag InBattle BackGround
			childFrameName = "map_ui_FRAME_FAIRY_ICON_INBATTLE";
			pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
			pChildSprite:setPosition(relativePoint);
			button:getAnimationNode():addChild(pChildSprite,0,1);
		elseif itemState == 0 then
			-- Note: Frame Flag InBattle BackGround
			childFrameName = "map_ui_FRAME_FAIRY_ICON_LOCK";
			pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
			pChildSprite:setPosition(relativePoint);
			button:getAnimationNode():addChild(pChildSprite,0,1);
		end		
		
		-- Note: 显示精灵精灵名称
		childFrameName = "map_ui_FRAME_FAIRY_TEXT_FAIRYNAME";
		local ElfName = LuaSpriteElfDataCenter:GetFairyName(elfId)
		ShowOneLabel(backGroundFrameName,childFrameName,ElfName,28,ccc3(238,166,26),anchorPoint,button:getAnimationNode())
				
		-- Note: 显示精灵精灵获取方式
		childFrameName = "map_ui_FRAME_FAIRY_TEXT_GETWAY";
		local ElfGetWayString = LuaSpriteElfDataCenter:GetFairyGetWayDescription(elfId)
		if "" ~= ElfGetWayString then
			ShowOneLabel(backGroundFrameName,childFrameName,ElfGetWayString,18,ccc3(255,255,255),anchorPoint,button:getAnimationNode())
		end
		
		--Note: 显示左侧头像
		childFrameName = "map_ui_FRAME_FAIRY_ICON_FAIRY";
		AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		local iconName = GetOneElfIcon(elfId)
		if nil ~= iconName then
			childFrameName = "map_ui_system_icon_FRAME_" .. iconName
			local elfLeftIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,childFrameName)
			if nil ~= elfLeftIcon then
				elfLeftIcon:setPosition(relativePoint);
				elfLeftIcon:setVisible(true)
				button:getAnimationNode():addChild(elfLeftIcon,1000);	
			end
		end
end

--[[
Instruction : 显示一段文本
@param 
@result 
--]]
function ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,HorAligment,VerAligment)
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

--Note: 显示技能图标
function ShowSkillLeftIcon(elfID)
	local iconName = LuaSpriteElfDataCenter:GetFairySkillIcon(elfID)
	local skillIcon = g_layout:FindChildObjectByName("iconFairySkill").__UIPicture__
	if nil ~= skillIcon then
		local sprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,iconName)
		skillIcon:setSprite(sprite)
	end
end

--[[
Instruction : ListItem Click Event Handler
@param 
@result 
--]]
function onListItemClick(sender)		
	CCLuaLog("onListItemClick" .. sender)
	local listItemIndex = sender+1

	if listItemIndex == g_SelectItemIndex then
		local btn = g_ElfListTable[listItemIndex]
		btn:selected()
		return
	end

	g_SelectItemIndex = listItemIndex
		
	local btn = g_ElfListTable[listItemIndex]
	if nil ~= g_selectedButton then
		g_selectedButton:unselected()
	end			
	btn:selected()
	g_selectedButton = btn
	
	local id = LuaSpriteElfDataCenter:GetFairyElfID(listItemIndex)
	ShowElfOnLayer(g_layout,id)
	ShowOneElfSkillInfo(id)
	ShowSkillLeftIcon(id)
	ShowOneElfAddAttributeInfo(id)

	local isLock = LuaSpriteElfDataCenter:GetFairyIsLock(listItemIndex)
	if isLock then
		g_fightBtn:setVisible(false)
		g_fightLable:setVisible(false)
	else
		g_fightBtn:setVisible(true)
		g_fightLable:setVisible(true)
		local isFight = LuaSpriteElfDataCenter:GetFairyIsFighter(listItemIndex)
		if isFight then
			g_fightLable:setString("取消出战")
		else
			g_fightLable:setString("出战")
		end
	end
end

--Note: 显示精灵技能信息
function ShowOneElfSkillInfo(elfId)
	g_skillNameLabel:setString(LuaSpriteElfDataCenter:GetFairyGetSkillName(elfId))
	g_skillDescriptionLabel:setString(LuaSpriteElfDataCenter:GetFairyGetSkillDescription(elfId))
end

function ShowOneElfAddAttributeInfo(elfId)
	local attributeTitleLabel = g_layout:FindChildObjectByName("attribute_title_txt").__UILabel__;
	local titleFlag = "M_FAIRY_ADDATTRIBUTE"
	if attributeTitleLabel ~= nil then
		local content = Localization[titleFlag].SimpleChinese
		attributeTitleLabel:setString(content)
	end

	local fairyAttribute = LuaSpriteElfDataCenter:GetOneFairyAddAttributes(elfId)
	for index,value in pairs(G_AttributeLabels) do
		local label = value
		local item = fairyAttribute[index]
		if item == nil then
			label:setString("")
		else
			label:setString(item)
		end
	end
end

function ShowAllElfAddAttributeInfo()
	local sumFairyAttribute = LuaSpriteElfDataCenter:GetAllFairyAddAttributes()
	for index,value in pairs(G_SumAttributeLabels) do
		local label = value
		local item = sumFairyAttribute[index]
		if item == nil then
			label:setString("")
		else
			label:setString(item)
		end
	end
end

--Note: 点击战斗点击战斗按钮按钮
function GoToFightBtnClick(pObj)
	print "GoToFightBtnClick"
	if -1 ~= g_SelectItemIndex then
		local elfId = LuaSpriteElfDataCenter:GetFairyElfID(g_SelectItemIndex)
		local elfState = LuaSpriteElfDataCenter:GetFairyElfState(g_SelectItemIndex)	
		print ("fight elfId " .. elfId)
		
		if elfState == 3 then
			G_FightElfID = 0
			SpriteElfDataCenter:Get():SendSetElfStateMessage(elfId,1)
			PushOneWaitingLayer("CSSetMonStatusReq")			
			SpriteFactory:sharedFactory():RemoveElfFromHero(SpriteElfDataCenter:Get():GetHeroUserId())			
			g_fightLable:setString("出战")
		else
			local fightItemIndex = LuaSpriteElfDataCenter.battleFairyIndex
			if 0 ~= fightItemIndex and fightItemIndex ~= g_SelectItemIndex then
				local preFightItemId = LuaSpriteElfDataCenter:GetFairyElfID(fightItemIndex)
				print("preFightItemId " .. preFightItemId)
				SpriteElfDataCenter:Get():SendSetElfStateMessage(preFightItemId,1)
				PushOneWaitingLayer("CSSetMonStatusReq")
			end
			SpriteElfDataCenter:Get():SendSetElfStateMessage(elfId,3)
			PushOneWaitingLayer("CSSetMonStatusReq")			
		end

	end
end

function OnLookUpAllAttributeBtnEvent(nTag)
	print("function OnLookUpAllAttributeBtnEvent(nTag) -------------- ")
	if false == g_fairyAttributeContainer:isVisible() then
		ShowAllElfAddAttributeInfo()
		g_fairyAttributeContainer:setVisible(true)
	end	
end

function OnAttributesPanelOkBtnEvent(nTag)
	print("function OnAttributesPanelOkBtnEvent(nTag) -------------- ")	
	g_fairyAttributeContainer:setVisible(false)
end

--Note: recieve rqs state message from server
function HandleRsqStateMessageFromServer(elfId,stateId)

	--print("----------- HandleRsqMessageFromServer " .. elfId .. stateId)
	
	if stateId == 2 then
		LuaSpriteElfDataCenter:SetFairyState(elfId,stateId)
		
		local Flag = "ELF_GET_NEW";
		local Content = Localization[Flag].SimpleChinese
		GameApi:showMessage(Content)		
		ShowNewElfTipIconOrNot(true)
	elseif stateId == 1 then
		LuaSpriteElfDataCenter:SetFairyState(elfId,stateId)
		local itemIndex = LuaSpriteElfDataCenter:GetIndexByFairyId(elfId)
		ShowListItemRightIconByState(itemIndex,stateId)
	elseif stateId == 3 then
		G_FightElfID = elfId
		LuaSpriteElfDataCenter:SetFairyState(elfId,stateId)
		SpriteElfDataCenter:Get():EquipOneElf(elfId)
		LuaSpriteElfDataCenter.battleFairyIndex = g_SelectItemIndex
		ShowListItemRightIconByState(LuaSpriteElfDataCenter.battleFairyIndex,stateId)
		g_fightLable:setString("取消出战")
	end

	return true
end

--Note: Show Sprite Come on State
--Note: 1 waiting 2 new 3 fight
function ShowListItemRightIconByState(itemIndex,state)
	local btnItem = g_ElfListItemTable[itemIndex]
	local childFlagIcon = btnItem:getAnimationNode():getChildByTag(1)
	if nil ~= childFlagIcon then
		childFlagIcon:removeFromParentAndCleanup(true)
		childFlagIcon = nil
	end
		
	local anchorPoint = CCPoint(0.5,0.5)
	local relativePoint = CCPoint(0,0)

	if state == 1 then
		-- Note: nothing do
	elseif state == 2 then
		-- Note: Frame Flag New BackGround
		local childFrameName = "map_ui_FRAME_FAIRY_ICON_NEW";
		local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		pChildSprite:setPosition(relativePoint);
		btnItem:getAnimationNode():addChild(pChildSprite,0,1);
	elseif state == 3 then
		-- Note: Frame Flag InBattle BackGround
		local childFrameName = "map_ui_FRAME_FAIRY_ICON_INBATTLE";
		local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		pChildSprite:setPosition(relativePoint);
		btnItem:getAnimationNode():addChild(pChildSprite,0,1);
	end
	
end

--Note: Get Player SpriteList Info
function HandleRsqListMessageFromServer(roleId,elfId,stateId,grade)
	--print("=========================================================== HandleRsqListMessageFromServer " .. elfId .. "  " .. stateId)
	--Note: 判断是否为主角
	local heroId = SpriteElfDataCenter:Get():GetHeroUserId()
	if roleId:isEqual(heroId) then
		--print("=========================================================== HandleRsqListMessageFromServer heroid equal id  " .. roleId:getID())
		--print("=========================================================== HandleRsqListMessageFromServer heroid equal heroid  " .. heroId:getID())
		LuaSpriteElfDataCenter:SetFairyState(elfId,stateId)
		--Note: Fight State
		if stateId == 3 then
			print("stateId == 3")
			G_FightElfID = elfId
			SpriteFactory:sharedFactory():AttachElfToOneHero(elfId,roleId,grade)
		elseif stateId == 2 then
			ShowNewElfTipIconOrNot(true)
		end
	else
		--Note: Fight State
		if stateId == 3 then
			SpriteFactory:sharedFactory():AttachElfToOneHero(elfId,roleId,grade)
		end
	end

	return true
end

G_RecieveElfListFromServer = false

function HandleGetElfListMessageOver()
	G_RecieveElfListFromServer = true
	return true
end

function IsGetElfListMessageOver()
	return G_RecieveElfListFromServer
end

--Note: Select Default ListItem when Init Layer open
function SelectDefaultListItem()
	if LuaSpriteElfDataCenter.battleFairyIndex == 0 then		
		onListItemClick(0)
	else
		onListItemClick(LuaSpriteElfDataCenter.battleFairyIndex-1)
	end	
end

--Note:获取获取英雄英雄战斗中战斗中精灵精灵
function GetHeroFightElfID()
	return G_FightElfID
end

---------------------------------------------------Chat Tool Bar UI Parts-------------------------------

function ChatToolTrainButtonClick(pObj)
	print("ChatToolTrainButtonClick")
	if nil ~= G_SpriteElfLayer then
		OnElfLayerCloseEvent(0)
	end
	--CCSpriteElfStrengthenLayer:CreateElfStengthenLayer(nil);
	SpriteFactory:sharedFactory():ShowElfPanelWithBackPackLayer()
end

function InitChatToolLayerPanel()
	local chatLayout = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar");
	local trainBtn = chatLayout:FindChildObjectByName("trainButton").__UIButton__;
	local trainBtnMenuItem = trainBtn:getMenuItemSprite();
	local potentialLabel = chatLayout:FindChildObjectByName("trainNumPotentialLabel").__UILabel__;
	
	local sumPotential = LuaSpriteElfDataCenter:GetTotalPotential()
	potentialLabel:setString("" .. sumPotential)
	
	local frameName = "map_ui_system_icon_FRAME_ICON_TRAIN"
	if nil ~= trainBtn then
		local sprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,frameName)
		trainBtn:setIconSprite(sprite)
	end	
	
	trainBtnMenuItem:registerScriptTapHandler(ChatToolTrainButtonClick);
end

--Note: 获取可用探索精灵列表
function GetExploreElfTabel()
	local resultTable = {}
	--Add : fairys排序
	for index,value in pairsByKeys(fairys) do
		getWay = tonumber(value.get_way)
		if 2 == getWay then
			local isAdd = false
			if nil == value.unlocklevel then
				isAdd = true
			else
				local unLockLevel = tonumber(value.unlocklevel)
				local userLevel = UserData:GetUserLevel()
				if userLevel >= unLockLevel then
					isAdd = true
				end
			end

			if isAdd then
				if LuaSpriteElfDataCenter:IsOneElfExistInElfDataTable(index) then
					isAdd = false
				end
			end

			if isAdd then
				InsertOneItemToTable(resultTable,index)
			end
		end
	end
	return resultTable
end

--Note:获取通过兑换方式可以获取到的精灵
function GetExchangeAbleElfTabel()
	local resultTable = {}
	--Add : fairys排序
	for index,value in pairsByKeys(fairys) do
		getWay = tonumber(value.get_way)
		if 3 == getWay then
			local isAdd = true
			if isAdd then
				InsertOneItemToTable(resultTable,index)
			end
		end
	end
	return resultTable
end

--------------------------------------------------------------------------------------------------------
local notificationCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(notificationCallBack)