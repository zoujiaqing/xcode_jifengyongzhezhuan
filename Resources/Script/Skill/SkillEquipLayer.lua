require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/Skill/SkillDataCenter")
require("Script/GameConfig/uiTagDefine")
SkillEquipLayer = {
	LayerInstance = nil,
	Layout = nil,
	iconContentLabel = nil,
	exploitContentLabel = nil,

}

function SkillEquipLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.iconContentLabel = nil
	self.exploitContentLabel = nil

end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= SkillEquipLayer.LayerInstance then
			if G_CurLayerInstance ~= SkillEquipLayer.LayerInstance then
				SkillEquipLayerCloseBtnClick(nil)
			end
		end
	end
end

local function onReceiveDestructorHandler()	
	TXGUI.UIManager:sharedManager():removeUILayout("SkillEquipPanel")
	SkillEquipLayer:ResetValue()
end

local firstBtnItem = nil

--Note: 创建Layer
function SkillEquipLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/SkillEquip.plist",self.LayerInstance, "SkillEquipPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["SkillEquipLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)

	self:InitLayer()
	self:ShowLeftBgIcons()
	self:InitScrollPage()

	--for index,value in pairs(SkillDataCenter.DataTable) do
		--self:ShowOneSkillItem(index,value.SkillID)
	--end	

	--高亮第一个技能按钮背景

	
	PushOneLayer(self.LayerInstance,"","")

	if firstBtnItem ~= nil then
		firstBtnItem:activate()
	end
	--SetChatBar(true,-1)
end

function SkillEquipLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(SkillEquipLayerCloseBtnClick);

	--local backToSkillLearnBtn = self.Layout:FindChildObjectByName("backSkillLearnBtn").__UIButton__:getMenuItemSprite();
	--backToSkillLearnBtn:registerScriptTapHandler(BackToSkillLearnPanelBtnClick);
	--backToSkillLearnBtn:setVisible(false)

	--Note:初始化左侧技能槽事件回调
	local itemCount = 5;--SkillDataCenter:GetOneRoleSkillCount(SkillDataCenter:GetHeroTypeId())
	for index = 1,itemCount do
		local btnName = "skillIconBtn_" .. index
		local btn = self.Layout:FindChildObjectByName(btnName).__UIButton__:getMenuItemSprite();
		btn:registerScriptTapHandler(SkillEquipLayerSkillIconBtnClick);
		btn:setTag(index)
	end

	self.iconContentLabel = self.Layout:FindChildObjectByName("MoneyLabel").__UILabel__
	self.exploitContentLabel = self.Layout:FindChildObjectByName("ExpolitLabel").__UILabel__
	local money = UserData:GetUserInfo().m_gold
	local moneyContent = MainMenuLayer:GetNumByFormat(money)
	self.iconContentLabel:setString("" .. moneyContent)
	local expolit = UserData:GetUserInfo().m_player_exploit
	local expolitContent = MainMenuLayer:GetNumByFormat(expolit)
	self.exploitContentLabel:setString("" .. expolitContent)

    
    -- 隐藏技能和功勋
    self.iconContentLabel:setVisible(false)
    self.exploitContentLabel:setVisible(false)
    self.Layout:FindChildObjectByName("iconPic"):setVisible(false)
    self.Layout:FindChildObjectByName("expolitPic"):setVisible(false)
end

function SkillEquipLayer:InitScrollPage()
	local layoutRawCount = 3
	local layoutColumnCount = 2
	local itemCount = SkillDataCenter:GetOneRoleSkillCount(SkillDataCenter:GetHeroTypeId())
	local onePageCount = layoutRawCount *  layoutColumnCount
	local val_1,val_2 = math.modf(itemCount/onePageCount)
	print("------------------------------------- val_1 val_2 " .. val_1 .. "  " .. val_2)
	if val_2 ~= 0 then
		val_1 = val_1 + 1
	end
	for index = 1,val_1 do
		local idList = SkillDataCenter:GetOnePageIdList(index,onePageCount,SkillDataCenter:GetHeroTypeId())
		local count = table.getn(idList)
		print("------------------- idList table " .. count)
		self:InertOneScrollPage(index,onePageCount,idList);
	end
end

function SkillEquipLayer:InertOneScrollPage(startPageIndex,onePageCount,idList)
	if nil == idList then
		print("------------------- idList Nil")
		return
	end

	local startIndex = (startPageIndex-1)*onePageCount

	local page = self.Layout:FindChildObjectByName("skillItemsScrollPage").__UIScrollPage__
	page:removeAllPages()
	local layer = CCLayer:create()
	page:addPage(layer, true)

	local count = 0
	for index,value in pairs(idList) do
		count = count + 1
		local id  = value
		self:CreateOneSkillItem(layer,startIndex+count,index,id)
	end

	page:moveToPage(0)
end

function SkillEquipLayer:ShowLeftBgIcons()
	local bgIconPath = "UI/ui_skillequip.png"
	local bgIcon = CCSprite:create(bgIconPath)
	local bgIconFramePic = self.Layout:FindChildObjectByName("equipFrameRect").__UIPicture__
	if nil ~= bgIconFramePic then
		bgIconFramePic:setSprite(bgIcon);
	end

	for index,value in pairs(SkillDataCenter.SlotDataTable) do
		self:ShowOneSlotIcon(value.SkillId,value.SlotIndex)
	end

	local attackFrameIconName = "skillIconAttack"
	local attackFrameIconPic = self.Layout:FindChildObjectByName(attackFrameIconName).__UIPicture__
	local attackIconName = "map_ui_system_icon_FRAME_ICON_ATTACK"
	local attackIconPic = AspriteManager:getInstance():getOneFrame(KICON_BIN,attackIconName)
	attackFrameIconPic:setSprite(attackIconPic);
end

function SkillEquipLayer:ShowOneSlotIcon(skillId,slotIndex)

	local btnName = "skillIconBtn_" .. slotIndex
	local btn = self.Layout:FindChildObjectByName(btnName).__UIButton__;

	local iconName = ""
	local isLock = SkillDataCenter:IsOneSlotLock(slotIndex)
	if isLock then
		--Note: 锁定状态
		iconName = "map_ui2_FRAME_SKILLLEARN_ICON_UNLOCK"
		local skillIcon = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,iconName)
		btn:setIconSprite(skillIcon)
		btn:setDragMove(false)
	else
		if skillId >0 then
			--Note: 技能状态
			iconName = SkillDataCenter:GetOneSkillIconName(skillId);
			local skillIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,iconName)
			btn:setIconSprite(skillIcon)
			btn:setDragMove(true)
		else
			--Note: Nothing to do
			btn:setIconSprite(nil)
			btn:setDragMove(false)
		end		
	end
end

local frameBtnItems = {}

function OnPressSkillFrameBtn(tag)
	--CCLuaLog("press skill frame "..tag)
	if frameBtnItems[tag] ~= nil then
		--CCLuaLog("find btn")
		for _, btnItem in pairs(frameBtnItems) do
			--CCLuaLog("unselected")
			btnItem:unselected()
		end

		frameBtnItems[tag]:selected()

		local skillDesContent = SkillDataCenter:GetSkillDescription(tag)

		local skillDesLabel = SkillEquipLayer.Layout:FindChildObjectByName("skillExplainContent").__UILabel__
		skillDesLabel:setString("" .. skillDesContent)
	end

end

function SkillEquipLayer:CreateOneSkillItem(parentNode,itemIndex,itemPageIndex,skillId)
	if parentNode == nil then
		return 
	end

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local backGroundFrameName = "map_ui2_FRAME_SKILLEQUIP_LISTAREA"
	local childFrameName = "map_ui2_FRAME_SKILLEQUIP_FRAME_SKILL" .. itemPageIndex;
	local clickedFrameName = childFrameName .. "_SELECTED"

	--Note:创建一个背景
	AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,backGroundFrameName,childFrameName,anchorPoint,relativePoint);		
	 
	if nil ~= childFrameName then
		local normalFrame = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,childFrameName)
		local clickedFrame = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,clickedFrameName)
 
		local skillFrameBtn = IconButton:new(normalFrame, nil, clickedFrame)
				
		skillFrameBtn:setPosition(relativePoint)

		local btnMenuItem = skillFrameBtn:getMenuItem()
		btnMenuItem:setTag(skillId)

		btnMenuItem:registerScriptTapHandler(OnPressSkillFrameBtn)

		frameBtnItems[skillId] = btnMenuItem

		if itemIndex == 1 then
			firstBtnItem = btnMenuItem
		end

		local tmpIcon = parentNode:getChildByTag(skillId)
		if nil ~= tmpIcon then
			tmpIcon:removeFromParentAndCleanup(true)
		end
		parentNode:addChild(skillFrameBtn,1,skillId);
	end

	--Note:创建一个Button
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local normalIcon = "map_ui2_FRAME_SKILLEQUIP_ICONGRID_SKILL" .. itemPageIndex;
	local clickIcon = "map_ui2_FRAME_SKILLEQUIP_ICONGRID_SKILL" .. itemPageIndex .. "_SELECTED";
	local childFrameName = "map_ui2_FRAME_SKILLEQUIP_ICONGRID_SKILL" .. itemPageIndex;

	AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	local normalSprite = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,normalIcon)
	local clickedSprite = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,clickIcon)
	local btn = IconButton:new(normalSprite, nil, clickedSprite)
	btn:setPosition(relativePoint);
	local tmpBtn = parentNode:getChildByTag(skillId+1)
	if nil ~= tmpBtn then
		tmpBtn:removeFromParentAndCleanup(true)
	end
	parentNode:addChild(btn,1,skillId+1);
	btn:SetButtonName("skillBtn_" .. itemIndex)	
	local btnMenuItem = btn:getMenuItem()
	btnMenuItem:setTag(skillId)
	--btnMenuItem:registerScriptTapHandler(onDailyRewardMainLayerOneItemClickEvent)
	local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);
	local skillIconName = ""
	local skillIconSprite = nil
	if skillLevel == 0 then
		skillIconName = "map_ui2_FRAME_SKILLLEARN_ICON_UNLOCK"
		skillIconSprite = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,skillIconName)
	else
		skillIconName = SkillDataCenter:GetOneSkillIconName(skillId)
		skillIconSprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,skillIconName)
	end

	btn:SetNormalIcon(skillIconSprite)
	if skillLevel == 0 then
		btn:setDragMove(false)
	else
		btn:SetButtonIndex(itemIndex)
		btn:creatDragReceiver(ESkillEquipItem)
		btn:setDragMove(true)
	end	

	--Note : 显示解锁等级
	local txtColor = ccc3(255,255,255)
	local anchorPoint = CCPoint(0.5,0.5);
	local content = ""
	if skillLevel == 0 then
		content = "Lv" .. SkillDataCenter:GetOneSkillUnLockLevel(skillId);
	end
	local childFrameName = "map_ui2_FRAME_SKILLEQUIP_TEXT_SKILL" .. itemPageIndex .. "_UNLOCKLEVEL"
	self:ShowOneLabel(backGroundFrameName,childFrameName,content,28,txtColor,anchorPoint,parentNode,10,skillId+2,kCCTextAlignmentCenter);

	--Note : 显示技能名称
	childFrameName = "map_ui2_FRAME_SKILLEQUIP_TEXT_SKILL" .. itemPageIndex .. "_NAME"
	local skillName = SkillDataCenter:GetSkillName(skillId)
	self:ShowOneLabel(backGroundFrameName,childFrameName,skillName,28,txtColor,anchorPoint,parentNode,10,skillId+3,kCCTextAlignmentCenter);

	--Note : 显示技能等级
	content = ""
	if skillLevel ~= 0 then
		content = "Lv" .. SkillDataCenter:GetOneSkillLevel(skillId);
	end
	content = ""
	print("--------------------------- content " .. content)
	childFrameName = "map_ui2_FRAME_SKILLEQUIP_TEXT_SKILL" .. itemPageIndex .. "_LEVEL"
	self:ShowOneLabel(backGroundFrameName,childFrameName,content,24,txtColor,anchorPoint,parentNode,10,skillId+4,kCCTextAlignmentCenter);
end

--Note: 显示文本函数
function SkillEquipLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local binName = KUI_BIN_2
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(binName,bgName,uiName,anchorPoint,relativePoint);
	local labelSize = pLabelSprite:getContentSize();
	local contentSize = CCSize(labelSize.width,labelSize.height)
	if HorAligment == nil then
		HorAligment = kCCTextAlignmentLeft
	end
	if VerAligment == nil then
		VerAligment = kCCVerticalTextAlignmentCenter
	end
	local bCreateNewLabel = true
	local nameLabel = nil
	if nil ~= nTag then
		nameLabel = parentNode:getChildByTag(nTag)
		if nil ~= nameLabel then
			bCreateNewLabel = false
		end
	end
	
	if bCreateNewLabel then
		nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)
		if nil ~= nTag then
			nameLabel:setTag(nTag)
		end		
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

--Note: 显示技能Item itemIndex 1 - 5 skillId 0 锁住 （废弃）
function SkillEquipLayer:ShowOneSkillItem(itemIndex,skillId)

	--local unLockLevelTxt = "";
	--local skillName = "";
	--local skillLevelTxt = ""
	--local skillIconName = ""	
--
	--local skillLevel = SkillDataCenter:GetOneSkillLevel(skillId);
--
	--local containerName = "skillItem_" .. itemIndex
	--local container = self.Layout:FindChildObjectByName(containerName).__UIContainer__;
--
	----Note ： 显示Btn
	--local skillIconSprite = nil
--
	--if skillLevel == 0 then
		--skillIconName = "map_ui2_FRAME_SKILLLEARN_ICON_UNLOCK"
		--skillIconSprite = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,skillIconName)
	--else
		--skillIconName = SkillDataCenter:GetOneSkillIconName(skillId)
		--skillIconSprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,skillIconName)
	--end
--
	--local skillIconBtn = container:FindChildObjectByName("skillBtn_" .. itemIndex).__UIButton__;
	--skillIconBtn:setIconSprite(skillIconSprite)
	--if skillLevel == 0 then
		--skillIconBtn:setDragMove(false)
	--else
		--skillIconBtn:setDragMove(true)
	--end	
--
	----Note : 显示解锁等级
	--local unLockLevelLabel = container:FindChildObjectByName("unLockLabel").__UILabel__;
	--if skillLevel == 0 then
		--local txt = "Lv" .. SkillDataCenter:GetOneSkillUnLockLevel(skillId);
		--unLockLevelLabel:setString(txt)
	--else
		--unLockLevelLabel:setString("")
	--end
--
	----Note : 显示技能名称
	--local skillNameLabel = container:FindChildObjectByName("skillNameLabel").__UILabel__;
	--skillName = SkillDataCenter:GetSkillName(skillId)
	--skillNameLabel:setString(skillName)
--
	----Note : 显示技能等级
	--local skillLevelLabel = container:FindChildObjectByName("SkillLevelLabel").__UILabel__; 
	--if skillLevel == 0 then
		--skillLevelLabel:setString("")
	--else		
		--local txt = "Lv" .. SkillDataCenter:GetOneSkillLevel(skillId);
		--skillLevelLabel:setString(txt)
	--end

end

-------------------事件回调------------------------------------------------
function SkillEquipLayerCloseBtnClick(pObj)
	if SkillEquipLayer.LayerInstance == nil then
		return
	end
	--TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
	SetChatBar(true,-1)
	CCTextureCache:sharedTextureCache():removeUnusedTextures()

	RemoveOneLayer(SkillEquipLayer.LayerInstance)
	--NotificationCenter:defaultCenter():unregisterScriptObserver(SkillEquipLayer.closeCallBackFun)

	SkillEquipLayer.LayerInstance:removeFromParentAndCleanup(true);
	SkillEquipLayer.LayerInstance = nil
	SkillEquipLayer:ResetValue()
end

function SkillEquipLayerSkillIconBtnClick(nTag)
	print("function SkillEquipLayerSkillIconBtnClick(nTag) " .. nTag)
end

function BackToSkillLearnPanelBtnClick(pObj)
	SkillEquipLayerCloseBtnClick(nil)
	SkillSelectLayer:CreateLayer(nil)
end

---------------------C++ 回调 -----------------------------------------------
function ListItemDragToSLotItem(skillItemIndex,slotIndex)
	--Note: 判断slot是否锁住

	local isLock = SkillDataCenter:IsOneSlotLock(slotIndex)
	if false == isLock then
		local skillId = SkillDataCenter:GetSkillIdByTableIndex(skillItemIndex);
		local toSlotSkillId = SkillDataCenter:GetOneSlotSkillId(slotIndex)
		if 0 == toSlotSkillId then
			local isExist,dstSlotIndex = SkillDataCenter:GetOneSkillExistInOtherSlot(slotIndex,skillId)
			if isExist then
				SkillDataCenter:SetOneSlotSkillId(dstSlotIndex,0)
				SkillDataCenter:SetOneSlotSkillId(slotIndex,skillId)
				SkillUIManager:getInstance():sendSetSlotAttributeMessage(dstSlotIndex,0)
				SkillUIManager:getInstance():sendSetSlotAttributeMessage(slotIndex,skillId)
				SkillEquipLayer:ShowOneSlotIcon(0,dstSlotIndex)
				SkillEquipLayer:ShowOneSlotIcon(skillId,slotIndex)				
			else
				SkillEquipLayer:ShowOneSlotIcon(skillId,slotIndex)
				SkillUIManager:getInstance():sendSetSlotAttributeMessage(slotIndex,skillId)
				SkillDataCenter:SetOneSlotSkillId(slotIndex,skillId)				
			end
		else
			if toSlotSkillId == skillId then
				--Note : Nothing to do
			else
				--Note : 判断是否已经存在该技能在其他槽中
				local isExist,dstSlotIndex = SkillDataCenter:GetOneSkillExistInOtherSlot(slotIndex,skillId)
				if isExist then
					SkillDataCenter:SetOneSlotSkillId(slotIndex,skillId)
					SkillDataCenter:SetOneSlotSkillId(dstSlotIndex,toSlotSkillId)
					SkillUIManager:getInstance():sendSetSlotAttributeMessage(slotIndex,skillId)
					SkillUIManager:getInstance():sendSetSlotAttributeMessage(dstSlotIndex,toSlotSkillId)
					SkillEquipLayer:ShowOneSlotIcon(skillId,slotIndex)
					SkillEquipLayer:ShowOneSlotIcon(toSlotSkillId,dstSlotIndex)		
				else
					SkillEquipLayer:ShowOneSlotIcon(skillId,slotIndex)
					SkillUIManager:getInstance():sendSetSlotAttributeMessage(slotIndex,skillId)
					SkillDataCenter:SetOneSlotSkillId(slotIndex,skillId)
				end
			end
		end
	end

	return true
end

function SlotItemToSelf(preSlotIndex,toSlotIndex)
	--Note: 判断slot是否锁住

	local isLock = SkillDataCenter:IsOneSlotLock(toSlotIndex)
	if false == isLock then
		--Note: 判断原槽中是否有技能
		local preSlotSkillId = SkillDataCenter:GetOneSlotSkillId(preSlotIndex)
		local toSlotSkillId = SkillDataCenter:GetOneSlotSkillId(toSlotIndex)

		if 0 == toSlotSkillId then
			SkillDataCenter:SetOneSlotSkillId(preSlotIndex,0)
			SkillDataCenter:SetOneSlotSkillId(toSlotIndex,preSlotSkillId)
			SkillUIManager:getInstance():sendSetSlotAttributeMessage(preSlotIndex,0)
			SkillUIManager:getInstance():sendSetSlotAttributeMessage(toSlotIndex,preSlotSkillId)
			SkillEquipLayer:ShowOneSlotIcon(0,preSlotIndex)
			SkillEquipLayer:ShowOneSlotIcon(preSlotSkillId,toSlotIndex)
		else
			SkillDataCenter:SetOneSlotSkillId(preSlotIndex,toSlotSkillId)
			SkillDataCenter:SetOneSlotSkillId(toSlotIndex,preSlotSkillId)
			SkillUIManager:getInstance():sendSetSlotAttributeMessage(preSlotIndex,toSlotSkillId)
			SkillUIManager:getInstance():sendSetSlotAttributeMessage(toSlotIndex,preSlotSkillId)
			SkillEquipLayer:ShowOneSlotIcon(toSlotSkillId,preSlotIndex)
			SkillEquipLayer:ShowOneSlotIcon(preSlotSkillId,toSlotIndex)			
		end
	end

	return true
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		SkillEquipLayerCloseBtnClick(nil)
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
	end
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(closeLayerCallBack);