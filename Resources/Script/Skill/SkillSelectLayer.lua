require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/Skill/SkillDataCenter")
require("Script/Skill/SkillEquipLayer")
require("Script/GameConfig/uiTagDefine")
local TIMER_RATE = 1

SkillSelectLayer = {
	LayerInstance = nil,
	Layout = nil,
	ScrollList = nil,
	SelectedListItem = nil,
	SelectItemIndex = -1,
	isRevCurLevelData = false,
	isRevNextLevelData = false,
	isShowSkillUpdateFlag = false,

	isLevelUpSuc = true,	
	iconContentLabel = nil,
	exploitContentLabel = nil,
	ListItemButtons = {},
	ListButtons = {}
}

function SkillSelectLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.ScrollList = nil
	self.SelectedListItem = nil
	self.SelectItemIndex = -1
	self.isRevCurLevelData = false
	self.isRevNextLevelData = false

	self.isLevelUpSuc = true
	self.iconContentLabel = nil
	self.exploitContentLabel = nil
	self.ListItemButtons = {}
	self.ListButtons = {}
end

--local function SkillSelectCloseLayerCallBack(message)
--
	--if message == GM_LUA_LAYER_CLOSE then
		--if nil ~= SkillSelectLayer.LayerInstance then
			--if G_CurLayerInstance ~= SkillSelectLayer.LayerInstance then
				--SkillSelectLayerCloseBtnClick(nil)
			--end			
		--end
	--end
--end

local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("SkillLearnPanel")	
	SkillSelectLayer:ResetValue()
end

--Note: 创建Layer
function SkillSelectLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	SetLayerOneTouch(self.LayerInstance)
	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/SkillLearn.plist",self.LayerInstance, "SkillLearnPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["SkillSelectLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	self:ShowLeftList()
	onSkillLearnPanelItemClick(0)
	
	PushOneLayer(self.LayerInstance,"","")	

	SetChatBar(true,-1)
end

function SkillSelectLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(SkillSelectLayerCloseBtnClick);
	local upgradeBtn = self.Layout:FindChildObjectByName("upLevelBtn").__UIButton__:getMenuItemSprite();
	upgradeBtn:registerScriptTapHandler(SkillSelectLayerUpgradeBtnClick);
	local equipBtn = self.Layout:FindChildObjectByName("equipBtn").__UIButton__:getMenuItemSprite();
	equipBtn:registerScriptTapHandler(SkillSelectLayerEquipBtnClick);

	self.iconContentLabel = self.Layout:FindChildObjectByName("MoneyLabel").__UILabel__
	self.exploitContentLabel = self.Layout:FindChildObjectByName("ExpolitLabel").__UILabel__
	local money = UserData:GetUserInfo().m_gold
	local moneyContent = MainMenuLayer:GetNumByFormat(money)
	self.iconContentLabel:setString("" .. moneyContent)
	local expolit = UserData:GetUserInfo().m_player_exploit
	local expolitContent = MainMenuLayer:GetNumByFormat(expolit)
	self.exploitContentLabel:setString("" .. expolitContent)
end


local sortFunc = function(a,b)
    return a.SkillUnLockLevel < b.SkillUnLockLevel;
end

function SkillSelectLayer:ShowLeftList()
	local scrollList = self.Layout:FindChildObjectByName("leftScrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end

	scrollList:clearChildrens()		
	
	table.sort(SkillDataCenter.DataTable,sortFunc);
	for index,value in pairs(SkillDataCenter.DataTable) do
		self:InsertOneSkillItem(scrollList,value)
	end
end

--Note:技能的自动学习
function SkillSelectLayer:FirstCheckWhetherSkillUnLock()
end

function SkillSelectLayer:CheckWhetherNewSkillUnLockWhenHeroLevelUp()
	local heroLevel = UserData:GetUserLevel()

	for index,value in pairs(SkillDataCenter.DataTable) do
		if nil ~= value then
			local skillId = value.SkillID
			local skillUnLockHeroLevel = value.SkillUnLockLevel
			local skillLevel = value.SkillLevel
			if 0 == skillLevel or nil == skillLevel then
				--Note:自动发送学习技能的消息
				if heroLevel >= skillUnLockHeroLevel then
					print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> heroLevel " .. heroLevel)
					print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> skillLevel " .. skillLevel)
					print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> skillUnLockHeroLevel " .. skillUnLockHeroLevel)
					--Note: 判断玩家是否达到解锁条件
					SkillUIManager:getInstance():sendAddUserSkillReqMessage(skillId,skillLevel+1)
					PushOneWaitingLayer("CSAddUserSkillReq")
				end
			end
		end		
	end
end

--Note: Private
--Note: List中插入一条
function SkillSelectLayer:InsertOneSkillItem(scrollList,oneSkillBasicData)
	if nil == scrollList then
		return 
	end
	
	local skillId = oneSkillBasicData.SkillID;

	local skillName = SkillDataCenter:GetSkillName(skillId)
	local skillIconName = SkillDataCenter:GetOneSkillIconName(skillId)
	local skillLevel = oneSkillBasicData.SkillLevel
	local skillUnLockLevel = oneSkillBasicData.SkillUnLockLevel
	local heroLevel = UserData:GetUserLevel()

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	--Note: 定义背景框大小
	local backGroundFrameName = "map_ui2_FRAME_SKILLLEARN_FRAME_SKILL1";
	local pBGFrame = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,backGroundFrameName);
	local bgFrameSize = pBGFrame:getContentSize();

	-- Note: List Item 
	local normalListItemIconBgName = "map_ui2_FRAME_SKILLLEARN_BUTTON_SKILL1";
	local clickListItemIconBgName = "map_ui2_FRAME_SKILLLEARN_BUTTON_SKILL1_SELECTED";
	local normal = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,clickListItemIconBgName)
		
	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);

	--Note: 技能图标
	self:ShowOneListItem(backGroundFrameName,button,skillId,skillLevel,skillUnLockLevel);
			
	local menuItem = button:getMenuItemSprite();
	menuItem:registerScriptTapHandler(onSkillLearnPanelItemClick);

	local listItemCount = table.getn(self.ListItemButtons);
	self.ListItemButtons[listItemCount+1] = menuItem
	
	local listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = button
end

--Note: 选中某ListItem
function onSkillLearnPanelItemClick(nTag)
	--print("SkillSelectLayer:onSkillLearnPanelItemClick " .. nTag)
	CCLuaLog("------------------------------------------------------ skill learn panel list item select " .. nTag)

	local listItemIndex = nTag+1
	SkillSelectLayer.SelectItemIndex = listItemIndex
		
	local item = SkillSelectLayer.ListItemButtons[listItemIndex]
	if nil ~= SkillSelectLayer.SelectedListItem then
		SkillSelectLayer.SelectedListItem:unselected()
	end			
	item:selected()
	SkillSelectLayer.SelectedListItem = item
	
	SkillSelectLayer:SendSkillDiffMessage()
end

function SkillSelectLayer:SendSkillDiffMessage()
	self.isRevCurLevelData = false
	self.isRevNextLevelData = false

	SkillDataCenter.skillLevelBasicData:ResetValue()
	SkillDataCenter.skillNextLevelBasicData:ResetValue()

	local skillId = SkillDataCenter:GetSkillIdByTableIndex(self.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);

	--print("SendSkillDiffMessage skillId " .. skillId);
	--print("SendSkillDiffMessage skillLevel " .. skillLevel);

	if 0 == skillLevel then
		SkillUIManager:getInstance():sendUserSkillDiffReqMessage(skillId,skillLevel+1);
		PushOneWaitingLayer("CSUserSkillDiffReq")
		self.isRevCurLevelData = true
	--Note:当前已经满级
	elseif  SkillDataCenter:IsUptoMaxLevel(skillId) then
		SkillUIManager:getInstance():sendUserSkillDiffReqMessage(skillId,skillLevel);
		PushOneWaitingLayer("CSUserSkillDiffReq")
		self.isRevNextLevelData = true	
	else
		SkillUIManager:getInstance():sendUserSkillDiffReqMessage(skillId,skillLevel);
		PushOneWaitingLayer("CSUserSkillDiffReq")
		SkillUIManager:getInstance():sendUserSkillDiffReqMessage(skillId,skillLevel+1);
		PushOneWaitingLayer("CSUserSkillDiffReq")
	end		
end

--Note: 
function SkillSelectLayer:ShowOneListItem(bgFrameName,bgButton,skillId,skillLevel,skillUnLockLevel)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui2_FRAME_SKILLLEARN_ICONGRID_SKILL1";

	local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,bgFrameName,childFrameName,anchorPoint,relativePoint);
	pChildSprite:setPosition(relativePoint);
	bgButton:getAnimationNode():addChild(pChildSprite,2);
	
	local skillIconName = SkillDataCenter:GetOneSkillIconName(skillId);
	if nil ~= skillIconName then
		local skillIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,skillIconName)		
		skillIcon:setPosition(relativePoint);
		bgButton:getAnimationNode():addChild(skillIcon,1);
	end

	--Note: 显示锁住按钮
	if skillLevel == 0 then
		--Note: 判断玩家是否达到解锁条件
		local isCanLearn = SkillDataCenter:IsSkillCanLearn(skillId)
		if false == isCanLearn then
			childFrameName = "map_ui2_FRAME_SKILLLEARN_ICON_UNLOCK";
			local lockIcon = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,childFrameName)		
			lockIcon:setPosition(relativePoint);
			bgButton:getAnimationNode():addChild(lockIcon,2);
		end
	end

	--Note: 显示解锁玩家等级
	if skillLevel == 0 then
		local isCanLearn = SkillDataCenter:IsSkillCanLearn(skillId)
		if false == isCanLearn then
			local playerLevel = "Lv" .. SkillDataCenter:GetOneSkillUnLockLevel(skillId);
			childFrameName = "map_ui2_FRAME_SKILLLEARN_TEXT_SKILL1_LOCKLEVEL";
			self:ShowOneLabel(bgFrameName,childFrameName,playerLevel,24,ccc3(255,255,255),anchorPoint,bgButton:getAnimationNode(),10,1001,kCCTextAlignmentCenter);
		end
	end

	--Note: 显示技能名称
	local skillName = SkillDataCenter:GetSkillName(skillId);
	childFrameName = "map_ui2_FRAME_SKILLLEARN_TEXT_SKILL1_NAME";
	self:ShowOneLabel(bgFrameName,childFrameName,skillName,28,ccc3(255,255,255),anchorPoint,bgButton:getAnimationNode(),10,1002,kCCTextAlignmentCenter);

	--Note: 显示技能等级
	if skillLevel ~= 0 then
		local skillLevelLabel = "Lv" .. SkillDataCenter:GetOneSkillLevel(skillId);
		childFrameName = "map_ui2_FRAME_SKILLLEARN_TEXT_SKILL1_LEVEL";
		self:ShowOneLabel(bgFrameName,childFrameName,skillLevelLabel,20,ccc3(255,255,255),anchorPoint,bgButton:getAnimationNode(),10,1003,kCCTextAlignmentCenter);
	end
end

--Note: 显示文本
function SkillSelectLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,bgName,uiName,anchorPoint,relativePoint);
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

function SkillSelectLayer:ShowRightPanel()
	--print("function SkillSelectLayer:ShowRightPanel()")
	self:ShowTopSkillInfoItem()
	self:ShowAttributeString()
	self:ShowDiffAttributeInfo()
	self:ShowDownButtonState()
end

function SkillSelectLayer:ShowTopSkillInfoItem()
	if(self.SelectItemIndex == -1) then
		return 
	end

	local skillId = SkillDataCenter:GetSkillIdByTableIndex(self.SelectItemIndex)
	
	--Note:显示技能图标
	local skillIcon = self.Layout:FindChildObjectByName("topListItemIcon").__UIPicture__
	if nil ~= skillIcon then
		local skillIconName = SkillDataCenter:GetOneSkillIconName(skillId);
		local sprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,skillIconName)
		skillIcon:setSprite(sprite)
	end

	--Note:显示技能名称
	local skillName = SkillDataCenter:GetSkillName(skillId);
	local skillNameLabel = self.Layout:FindChildObjectByName("skillNameLabel").__UILabel__;
	skillNameLabel:setString(skillName)


	--Note: 显示技能等级
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);
	local skillLevelLabel = self.Layout:FindChildObjectByName("skillLevelLabel").__UILabel__;
	if 0 ~= skillLevel then
		local skillLevelTxt = "Lv" .. skillLevel;		
		skillLevelLabel:setVisible(true)
		skillLevelLabel:setString(skillLevelTxt)
	else
		skillLevelLabel:setVisible(false)
	end

	--Note: 显示技能描述
	local skillDescription = SkillDataCenter:GetSkillDescription(skillId)
	local skillDescriptionLabel = self.Layout:FindChildObjectByName("skillDescriptionLabel").__UILabel__;
	skillDescriptionLabel:setString(skillDescription)
	skillDescriptionLabel:setColor(ccc3(85,201,255))

	--Note:显示升级金币值
	local money,expolite = SkillDataCenter:GetSkillLevelUpValue();
	local moneyLabel = self.Layout:FindChildObjectByName("moneyLabel").__UILabel__;
	moneyLabel:setString("" .. money)
	local expoliteLabel = self.Layout:FindChildObjectByName("exploitValue").__UILabel__;
	expoliteLabel:setString("" .. expolite)

	local nextLevelUpValue = ""
	local nextLevelHeroLevelLabel = self.Layout:FindChildObjectByName("nextLevelValue").__UILabel__;
	nextLevelHeroLevelLabel:setColor(ccc3(187,110,0))
	local nextLevelHeroLevelFlagLabel = self.Layout:FindChildObjectByName("nextLevelTxtNeed").__UILabel__;
	if false == SkillDataCenter:IsUptoMaxLevel(skillId) and 0 ~= skillLevel then
		--Note: 显示升级需要的级别
		local bTurnRed = false
		local nextLevelUpLevel = SkillDataCenter:GetLevelUpHeroLevel()
		--Note: 如果人物等级没达到 Level变红色
		local heroLevel = UserData:GetUserLevel()
		if heroLevel < nextLevelUpLevel then
			bTurnRed = true
		end

		nextLevelUpValue = LanguageLocalization:GetLocalization("M_ROLELEVEL") .. " Lv" .. nextLevelUpLevel
		nextLevelHeroLevelLabel:setString(nextLevelUpValue)
		nextLevelHeroLevelFlagLabel:setVisible(true)

		if bTurnRed then
			nextLevelHeroLevelLabel:setColor(ccc3(255,0,0))
		else
			nextLevelHeroLevelLabel:setColor(ccc3(187,110,0))
		end
	else
		nextLevelHeroLevelLabel:setString("")
		nextLevelHeroLevelFlagLabel:setVisible(false)
	end
	
end

function SkillSelectLayer:ShowAttributeString()
	local rstString = "";

	local skillId = SkillDataCenter:GetSkillIdByTableIndex(self.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);

	local commonBasicData = nil
	if 0 == skillLevel then

		commonBasicData = SkillDataCenter.skillNextLevelBasicData
	else
		commonBasicData = SkillDataCenter.skillLevelBasicData	
	end
	local basicData = commonBasicData.diff_battle_info;

	if self.isRevNextLevelData and self.isRevCurLevelData then
		local skillId = SkillDataCenter:GetSkillIdByTableIndex(SkillSelectLayer.SelectItemIndex)
		local attributeTable = SkillDataCenter:GetOneSkillAttruibte(skillId)
		for index,value in pairs(attributeTable) do
			local attributeType = tonumber(value)
			if attributeType == 1 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_1") .. "+" .. basicData.health_point) .. " ";
			elseif attributeType == 2 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_2") .. commonBasicData.diff_hurt) .. "% ";
			elseif attributeType == 3 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_3") .. commonBasicData.diff_own_cd) .. "s ";
			elseif attributeType == 4 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_4") .. commonBasicData.continue_cd) .. "s ";
			elseif attributeType == 5 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_5") .. "+" .. basicData.physical_attack) .. " ";
			elseif attributeType == 6 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_6") .. "+" .. basicData.physical_defence) .. " ";
			elseif attributeType == 7 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_7") .. "+" .. basicData.magic_attack) .. " ";
			elseif attributeType == 8 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_8") .. "+" .. basicData.magic_defence) .. " ";
			elseif attributeType == 9 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_9") .. basicData.skill_attack) .. " ";
			elseif attributeType == 10 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_10") .. "+" .. basicData.skill_defence) .. " ";
			elseif attributeType == 11 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_11") .. "+" .. basicData.accurate) .. " ";
			elseif attributeType == 12 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_12") .. "+" .. basicData.dodge) .. " ";
			elseif attributeType == 13 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_13") .. "+" .. basicData.wreck) .. " ";
			elseif attributeType == 14 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_14") .. "+" .. basicData.parry) .. " ";
			elseif attributeType == 15 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_15") .. "+" .. basicData.critical_strike) .. " ";
			elseif attributeType == 16 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_16") .. "+" .. basicData.tenacity) .. " ";
			elseif attributeType == 17 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_17") .. "+" .. basicData.slay) .. " ";
			elseif attributeType == 18 then
				rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_18") .. "+" .. basicData.speed) .. " ";
			end
		end
	end
	
	local attributeLabel = self.Layout:FindChildObjectByName("skillAttributeLabel").__UILabel__;
	attributeLabel:setString(rstString)
	attributeLabel:setColor(ccc3(195,236,255))

	return rstString;
end

function SkillSelectLayer:ShowDiffAttributeInfo()
	local rstString = ""

	local attributeLabel = self.Layout:FindChildObjectByName("nextLevelTxtAttibute").__UILabel__;
	local nextAttributeFlagLabel = self.Layout:FindChildObjectByName("nextLevelTxtLabel").__UILabel__;
	attributeLabel:setColor(ccc3(126,255,12))

	local skillId = SkillDataCenter:GetSkillIdByTableIndex(self.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);
	if  0 == skillLevel or SkillDataCenter:IsUptoMaxLevel(skillId) then
		attributeLabel:setString(rstString)		
		nextAttributeFlagLabel:setVisible(false)
		print("function SkillSelectLayer:ShowDiffAttributeInfo()")
		return rstString
	end

	nextAttributeFlagLabel:setVisible(true)

	if self.isRevNextLevelData and self.isRevCurLevelData then
		local basicData = SkillDataCenter.skillLevelBasicData.diff_battle_info;
		local nextLevelBasicData = SkillDataCenter.skillNextLevelBasicData.diff_battle_info;

		local health_point_interpolation = nextLevelBasicData.health_point - basicData.health_point
		if 0 ~= health_point_interpolation then
			local addFlag = ""
			if health_point_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_1") .. addFlag .. health_point_interpolation) .. " ";
		end
		local diff_hurt_interpolation = SkillDataCenter.skillNextLevelBasicData.diff_hurt - SkillDataCenter.skillLevelBasicData.diff_hurt
		if 0 ~= diff_hurt_interpolation then
			local addFlag = ""
			if diff_hurt_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_2_2") .. addFlag .. diff_hurt_interpolation) .. "% ";
		end
		local diff_own_cd_interpolation = SkillDataCenter.skillNextLevelBasicData.diff_own_cd - SkillDataCenter.skillLevelBasicData.diff_own_cd
		if 0 ~= diff_own_cd_interpolation then
			local addFlag = ""
			if diff_own_cd_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_3") .. addFlag .. diff_own_cd_interpolation) .. " ";
		end
		local continue_cd_interpolation = SkillDataCenter.skillNextLevelBasicData.continue_cd - SkillDataCenter.skillLevelBasicData.continue_cd
		if 0 ~= continue_cd_interpolation then
			local addFlag = ""
			if continue_cd_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_4") .. addFlag .. continue_cd_interpolation) .. " ";
		end
		local physical_attack_interpolation = nextLevelBasicData.physical_attack - basicData.physical_attack
		if 0 ~= physical_attack_interpolation then
			local addFlag = ""
			if physical_attack_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_5") .. addFlag .. physical_attack_interpolation) .. " ";
		end
		local physical_defence_interpolation = nextLevelBasicData.physical_defence - basicData.physical_defence
		if 0 ~= physical_defence_interpolation then
			local addFlag = ""
			if physical_defence_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_6") .. addFlag .. physical_defence_interpolation) .. " ";
		end
		local magic_attack_interpolation = nextLevelBasicData.magic_attack - basicData.magic_attack
		if 0 ~= magic_attack_interpolation then
			local addFlag = ""
			if magic_attack_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_7") .. addFlag .. magic_attack_interpolation) .. " ";
		end
		local magic_defence_interpolation = nextLevelBasicData.magic_defence - basicData.magic_defence
		if 0 ~= magic_defence_interpolation then
			local addFlag = ""
			if magic_defence_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_8") .. addFlag .. magic_defence_interpolation) .. " ";
		end
		local skill_attack_interpolation = nextLevelBasicData.skill_attack - basicData.skill_attack
		if 0 ~= skill_attack_interpolation then
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_9_2") .. skill_attack_interpolation) .. " ";
		end
		local skill_defence_interpolation = nextLevelBasicData.skill_defence - basicData.skill_defence
		if 0 ~= skill_defence_interpolation then
			local addFlag = ""
			if skill_defence_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_10") .. addFlag .. skill_defence_interpolation) .. " ";
		end
		local accurate_interpolation = nextLevelBasicData.accurate - basicData.accurate
		if 0 ~= accurate_interpolation then
			local addFlag = ""
			if accurate_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_11") .. addFlag .. accurate_interpolation) .. " ";
		end
		local dodge_interpolation = nextLevelBasicData.dodge - basicData.dodge
		if 0 ~= dodge_interpolation then
			local addFlag = ""
			if dodge_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_12") .. addFlag .. dodge_interpolation) .. " ";
		end
		local wreck_interpolation = nextLevelBasicData.wreck - basicData.wreck
		if 0 ~= wreck_interpolation then
			local addFlag = ""
			if wreck_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_13") .. addFlag .. wreck_interpolation) .. " ";
		end
		local parry_interpolation = nextLevelBasicData.parry - basicData.parry
		if 0 ~= parry_interpolation then
			local addFlag = ""
			if parry_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_14") .. addFlag .. parry_interpolation) .. " ";
		end
		local critical_strike_interpolation = nextLevelBasicData.critical_strike - basicData.critical_strike
		if 0 ~= critical_strike_interpolation then
			local addFlag = ""
			if critical_strike_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_15") .. addFlag .. critical_strike_interpolation) .. " ";
		end
		local tenacity_interpolation = nextLevelBasicData.tenacity - basicData.tenacity
		if 0 ~= tenacity_interpolation then
			local addFlag = ""
			if tenacity_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_16") .. addFlag .. tenacity_interpolation) .. " ";
		end
		local slay_interpolation = nextLevelBasicData.slay - basicData.slay
		if 0 ~= slay_interpolation then
			local addFlag = ""
			if slay_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_17") .. addFlag .. slay_interpolation) .. " ";
		end
		local speed_interpolation = nextLevelBasicData.speed - basicData.speed
		if 0 ~= speed_interpolation then
			local addFlag = ""
			if speed_interpolation > 0 then
				addFlag = "+"
			end
			rstString = rstString .. (LanguageLocalization:GetLocalization("SKI_CORE_ATTRIBUTE_18") .. addFlag ..speed_interpolation) .. " ";
		end
	end	
	
	attributeLabel:setString(rstString)

	return rstString;
end

function SkillSelectLayer:ShowDownButtonState()
	local skillId = SkillDataCenter:GetSkillIdByTableIndex(SkillSelectLayer.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);

	--print("function SkillSelectLayer:ShowDownButtonState() " .. skillId)
	--print("function SkillSelectLayer:ShowDownButtonState() " .. skillLevel)

	local txt = "";
	--Note: 显示锁住按钮
	if skillLevel == 0 then
		txt = LanguageLocalization:GetLocalization("S_Learn_common_content")
	else
		--Note: 显示升级按钮
		txt = LanguageLocalization:GetLocalization("S_Upgrade_common_content")
	end

	local attributeLabel = self.Layout:FindChildObjectByName("upgradeBtnLabel").__UILabel__;
	attributeLabel:setString(txt)
end

function SkillSelectLayer:OnRecieveDiffInfo()
	if self.isRevNextLevelData and self.isRevCurLevelData then
		--print("function SkillSelectLayer:OnRecieveDiffInfo()")
		self:ShowRightPanel()
		self:ShowLeftPanelHeroLevel()
	end
end

function SkillSelectLayer:ShowLeftPanelHeroLevel()
	local skillId = SkillDataCenter:GetSkillIdByTableIndex(self.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);
	
	local button = self.ListButtons[self.SelectItemIndex]
	--Note: 显示技能等级
	if skillLevel ~= 0 then
		local skillLevelLabel = "Lv" .. SkillDataCenter:GetOneSkillLevel(skillId);
		local childFrameName = "map_ui2_FRAME_SKILLLEARN_TEXT_SKILL1_LEVEL";
		local bgFrameName = "map_ui2_FRAME_SKILLLEARN_FRAME_SKILL1";
		local anchorPoint = ccp(0.5,0.5)
		self:ShowOneLabel(bgFrameName,childFrameName,skillLevelLabel,20,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),10,1003,kCCTextAlignmentCenter);
	end
end

function SkillSelectLayer:ShowSkillUpdateFlag(bShowOrNot)
	--if bShowOrNot then
		--print("---------------> " .. "ShowSkillUpdateFlag" .. "true")
	--else
		--print("---------------> " .. "ShowSkillUpdateFlag" .. "false")
	--end
	do return end
	
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local skillButton = nil
	if nil ~= topLayout then
		skillButton = topLayout:FindChildObjectByName("button_skillupgrade").__UIButton__
	end

	if skillButton then
		if bShowOrNot then
			--print("---------------> " .. "showTipIcon")
			skillButton:showTipIcon()
		else
			--print("---------------> " .. "hideTipIcon")
			skillButton:hideTipIcon()
		end
	end
end

-----------------------------------------------------------------
------事件代码
function SkillSelectLayerCloseBtnClick(pObj)
	if SkillSelectLayer.LayerInstance == nil then
		return
	end
	SetChatBar(true,-1)
	CCTextureCache:sharedTextureCache():removeUnusedTextures()

	RemoveOneLayer(SkillSelectLayer.LayerInstance)
	--NotificationCenter:defaultCenter():unregisterScriptObserver(SkillSelectLayer.closeCallBackFun)

	SkillSelectLayer.LayerInstance:removeFromParentAndCleanup(true);
	SkillSelectLayer.LayerInstance = nil
	SkillSelectLayer:ResetValue()
end

function SkillSelectLayerUpgradeBtnClick(pObj)
	local skillId = SkillDataCenter:GetSkillIdByTableIndex(SkillSelectLayer.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);
	if skillLevel == 0 then
		--Note: 判断玩家是否达到解锁条件
		local isCanLearn = SkillDataCenter:IsSkillCanLearn(skillId)
		if isCanLearn then
			SkillUIManager:getInstance():sendAddUserSkillReqMessage(skillId,skillLevel+1)
			--ShowWaitingLayer()
			PushOneWaitingLayer("CSAddUserSkillReq")
		else
			GameApi:showMessage(LanguageLocalization:GetLocalization("S_Not_Enough_Level_Content"))
		end
	else
		if SkillDataCenter:IsUptoMaxLevel(skillId) then
			local txt = LanguageLocalization:GetLocalization("S_UpToMaxLevel_content")
			GameApi:showMessage(txt)
		else
			SkillUIManager:getInstance():sendAddUserSkillReqMessage(skillId,skillLevel+1)
			--ShowWaitingLayer()
			PushOneWaitingLayer("CSAddUserSkillReq")
		end		
	end
end

function SkillSelectLayerEquipBtnClick(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	SkillSelectLayerCloseBtnClick(nil)
	SkillEquipLayer:CreateLayer(nil)
	SetChatBar(true,-1)
end

--Note: 得到状态信息值
function PushDiffInfoToLua(skill_id,skill_level,
	next_money,next_exploit,next_lv,diff_own_cd,continue_cd,diff_hurt,
	physical_attack,magic_attack,skill_attack,physical_defence,magic_defence,skill_defence,health_point,
	accurate,dodge,wreck,parry,critical_strike,tenacity,slay,speed,proficiency)

	local skillId = SkillDataCenter:GetSkillIdByTableIndex(SkillSelectLayer.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);

	local basicData = nil;
	if skill_level == skillLevel then
		--print("SkillSelectLayer.isRevCurLevelData = true")
		basicData = SkillDataCenter.skillLevelBasicData
		SkillSelectLayer.isRevCurLevelData = true		
	else
		--print("SkillSelectLayer.isRevNextLevelData = true")
		SkillSelectLayer.isRevNextLevelData = true
		basicData = SkillDataCenter.skillNextLevelBasicData
	end

	basicData.skill_id = skill_id;
	basicData.skill_level = skill_level;

	basicData.next_money = next_money
	basicData.next_exploit = next_exploit
	basicData.next_lv = next_lv
	basicData.diff_own_cd = diff_own_cd * TIMER_RATE
	basicData.continue_cd = continue_cd * TIMER_RATE
	basicData.diff_hurt = diff_hurt

	basicData.diff_battle_info.physical_attack = physical_attack;
	basicData.diff_battle_info.magic_attack = magic_attack;
	basicData.diff_battle_info.skill_attack = skill_attack;
	basicData.diff_battle_info.physical_defence = physical_defence;
	basicData.diff_battle_info.magic_defence = magic_defence;
	basicData.diff_battle_info.skill_defence = skill_defence;
	basicData.diff_battle_info.health_point = health_point;
	basicData.diff_battle_info.accurate = accurate;
	basicData.diff_battle_info.dodge = dodge;
	basicData.diff_battle_info.wreck = wreck;
	basicData.diff_battle_info.parry = parry
	basicData.diff_battle_info.critical_strike = critical_strike;
	basicData.diff_battle_info.tenacity = tenacity;
	basicData.diff_battle_info.slay = slay;
	basicData.diff_battle_info.speed = speed;
	basicData.diff_battle_info.proficiency = proficiency;

	SkillSelectLayer:OnRecieveDiffInfo()

	return true
end

function handleMsgAddUserSkillRsp(skill_id,skill_level,max_level)
	--HideWaitingLayer()
	
	--Note: 判断是否升级成功
	local skillId = skill_id --SkillDataCenter:GetSkillIdByTableIndex(SkillSelectLayer.SelectItemIndex)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);

	--print(">>>>>>>>>>>>>>>>>>>>>>>>> skill_id " .. skill_id)
	--print(">>>>>>>>>>>>>>>>>>>>>>>>> skillId " .. skillId)
	--print(">>>>>>>>>>>>>>>>>>>>>>>>> skillLevel " .. skillLevel)
	--print(">>>>>>>>>>>>>>>>>>>>>>>>> skill_level " .. skill_level)
	--if skill_id ~= skillId then		
		--return false
	--end

	if (skillLevel+1) ~= skill_level then
		return false
	end

	--Note:升级成功
	SkillDataCenter:PushOneSkillItem(skill_id,skill_level,max_level)

	--Note: 判断UI界面是否打开，打开以后才显示对应的效果
	if SkillSelectLayer.LayerInstance ~= nil then
		local txt = LanguageLocalization:GetLocalization("S_Upgrade_common_content")
		local attributeLabel = SkillSelectLayer.Layout:FindChildObjectByName("upgradeBtnLabel").__UILabel__;
		attributeLabel:setString(txt)

		SkillUIManager:getInstance():sendGetUserSkillReqMessage(true)	
		PushOneWaitingLayer("CSGetUserSkillReq")
		SkillSelectLayer.isLevelUpSuc = false;	

		--Note: 播放特效
		local skillIcon = SkillSelectLayer.Layout:FindChildObjectByName("topListItemIcon").__UIPicture__:getCurrentNode()
		if nil ~= skillIcon then		
			local particle = skillIcon:getChildByTag(10010)
			if nil ~= particle then
				skillIcon.removeChild(particle)
			end
		
			particle = ParticleManagerX:sharedManager():getParticles_uieffect("particle_effect_upgradesuccess")
            if nil ~= particle then
                particle:setPosition(ccp(skillIcon:getContentSize().width * 0.5,skillIcon:getContentSize().height * 0.5));
                skillIcon:addChild(particle,10,10010)
            end

		end	

		GameAudioManager:sharedManager():playEffect(350006,false);
	end
	
	return true
end

function SkillLevelUpSucMessage()	
	if false == SkillSelectLayer.isLevelUpSuc then
		SkillSelectLayer.isLevelUpSuc = true
		SkillSelectLayer:SendSkillDiffMessage()
	end
	return true
end

local function skillSelectLayer_updateNotification(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= SkillSelectLayer.LayerInstance then
			if G_CurLayerInstance ~= SkillSelectLayer.LayerInstance then
				SkillSelectLayerCloseBtnClick(nil)
			end			
		end
	elseif message == GM_NETWORK_DISCONNCT then
		SkillSelectLayerCloseBtnClick(nil)
	elseif message == GM_ATTR_COIN_UPDATE then
		if nil ~= SkillSelectLayer.iconContentLabel then
			local money = UserData:GetUserInfo().m_gold
			local moneyContent = MainMenuLayer:GetNumByFormat(money)
			SkillSelectLayer.iconContentLabel:setString("" .. moneyContent)
		end
	elseif message == GM_ATTR_PLAYER_EXPOLIT then
		if nil ~= SkillSelectLayer.exploitContentLabel then
			local expolit = UserData:GetUserInfo().m_player_exploit
			local expolitContent = MainMenuLayer:GetNumByFormat(expolit)
			SkillSelectLayer.exploitContentLabel:setString("" .. expolitContent)
		end
	elseif message == GM_E_ENTER_MAINLAND then
		--print("------------------> " .. "enter main land")
		--if SkillSelectLayer.LayerInstance ~= nil then
			SkillSelectLayer:ShowSkillUpdateFlag(SkillSelectLayer.isShowSkillUpdateFlag)
		--end		
	elseif message == GM_ATTR_HERO_LEVEL_UPDATE then
		--print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< GM_ATTR_HERO_LEVEL_UPDATE")
		SkillSelectLayer:CheckWhetherNewSkillUnLockWhenHeroLevelUp()
	end	
end

function SkillHandleSkillUpdateNotification(bValid)
	--if bValid then
		--print("------------------> " .. "HandleSkillUpdateNotification" .. "true")
	--else
		--print("------------------> " .. "HandleSkillUpdateNotification" .. "false")
	--end
	
	SkillSelectLayer.isShowSkillUpdateFlag = bValid
	SkillSelectLayer:ShowSkillUpdateFlag(SkillSelectLayer.isShowSkillUpdateFlag)
	return true
end

function CheckWhetherNewSkillUnLockWhenHeroLevelUp()
	SkillSelectLayer:CheckWhetherNewSkillUnLockWhenHeroLevelUp()
	return true
end


-----------------------------------------------------------------
if SkillSelectLayerNotificationFunc == nil then
	SkillSelectLayerNotificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(skillSelectLayer_updateNotification);
end

--local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(skillSelectLayer_updateNotification);
--local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(SkillSelectCloseLayerCallBack)