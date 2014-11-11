require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/UILayer/ThreeDaysUI")
require("Script/DailyRewardPVAI/dailyRewardPvaiLayer")
require("Script/DailyReward/dailyRewardMainLayerDataCenter")
require("Script/DailySalary/dailyRewardSalaryLayer")
require("Script/UILayer/offlineExp")
require("Script/UILayer/yongZheBaoZhang")
require("Script/UILayer/lotteryUI")
require("Script/UILayer/vipreward")
require("Script/UILayer/dailyRewardStaminaLayer")
require("Script/UILayer/exchangeCodeLayer")
DailyRewardMainLayer = {
	LayerInstance = nil,
    LayerReward = nil,

    ListButtons = {},
	ListButtonsMenuItems = {},

	SelectItemIndex = 0,
}

function DailyRewardMainLayer:ResetValue()
	self.LayerInstance = nil
    self.LayerReward = nil

    self.ListButtons = {}
	self.ListButtonsMenuItems = {}

	self.SelectItemIndex = 0
end

local function onReceiveDestructorHandler()	
	TXGUI.UIManager:sharedManager():removeUILayout("DailyReward")
	DailyRewardMainLayer:ResetValue()
end

function DailyRewardMainLayer:CreateLayer(parentLayer)
    --print("DailyRewardMainLayer:CreateLayer")
	self:ResetValue()
	dailyRewardMainLayerDataCenter:InitAllData()
    
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
    
    self.LayerReward = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/Dailyreward.plist",self.LayerInstance, "DailyReward", true)
       
    if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["DailyReward"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
	end
	PushOneLayer(self.LayerInstance,"","")    
	
	self:Init()    
    self:setupScrollist()
end

function DailyRewardMainLayer:Init()
    local bnt_close = self.LayerReward:FindChildObjectByName("rewardCloseBtn").__UIButton__:getMenuItemSprite();
    bnt_close:registerScriptTapHandler(onDailyRewardMainLayerCloseClick)
    
	--Note: 设置标题
    local label_tile = self.LayerReward:FindChildObjectByName("reward_text_tile").__UILabel__;
    label_tile:setString(LanguageLocalization:GetLocalization("M_DAILYREWARD_TITLE"))
end

function DailyRewardMainLayer:ShowOrNot(bShow)
	if nil ~= self.LayerReward then
		self.LayerReward:setVisible(bShow)
	end
end

function DailyRewardMainLayer:setupScrollist()

    local scrollList = self.LayerReward:FindChildObjectByName("rewardScrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end
	scrollList:clearChildrens()
    self.ListButtons = {}
	self.ListButtonsMenuItems = {}

	local dataCenterTable = dailyRewardMainLayerDataCenter.Items
	local itemsCount = GetTableCount(dataCenterTable)
	local itemCount = 0
	local id_1 = nil
	local id_2 = nil
	local buttonIndex = 0
	for index,value in pairsByKeys(dataCenterTable) do
		itemCount = itemCount + 1

		local id = value.id
		if id_1 == nil then
			id_1 = id
		elseif id_2 == nil then
			id_2 = id
		end

		if id_1 ~= nil and id_2 ~= nil then
			self:insertItemToList(scrollList,id_1,id_2,buttonIndex)
			buttonIndex = buttonIndex + 1
			id_1 = nil
			id_2 = nil
		else
			if(itemCount == itemsCount) then
				self:insertItemToList(scrollList,id_1,id_2,buttonIndex)
				buttonIndex = buttonIndex + 1
				id_1 = nil
				id_2 = nil
			end
		end
	end	
end

function DailyRewardMainLayer:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end


function DailyRewardMainLayer:insertOneChildListItemToList(id,scrollList,parentNode,bgIcon,normalIcon,clickIcon,buttonIndex)
	local strBin = "UI/ui_dailyreward.bin"
	--Note: 得到子Button的相对位置
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	AspriteManager:getInstance():getFramesRelativePos(strBin,bgIcon,normalIcon,anchorPoint,relativePoint);
	self:ScaleByScreen(relativePoint)
	--Note:创建一个按钮
	local normalSprite = AspriteManager:getInstance():getOneFrame(strBin,normalIcon)
	local clickedSprite = AspriteManager:getInstance():getOneFrame(strBin,clickIcon)
	local menuItem = MenuItemExpand:create(normalSprite,clickedSprite);
	local btn = TXGUI.UIButton:newExpand("", 0, 0, scrollList, menuItem, nil,-130, "_click");
	local parentControlPos = parentNode:getPosition()
	local newBtnPos = ccp(parentControlPos.x + relativePoint.x , parentControlPos.y + relativePoint.y)
	btn:getAnimationNode():setPosition(newBtnPos);
	btn:setName("ListButton_" .. buttonIndex)	
	--print("------------------------ btn name " .. buttonIndex)

	--Note: 定义回调接口
	self.ListButtons[id] = btn
	local btnMenuItem = btn:getMenuItemSpriteExpand()
	self.ListButtonsMenuItems[id] = btnMenuItem
	btnMenuItem:setTag(id)
	btnMenuItem:registerScriptTapHandler(onDailyRewardMainLayerOneItemClickEvent)

	--Note: 插入子控件
	self:ShowOneItemIcon(btn,id)
	self:ShowOneItemName(btn,id)
	self:ShowOneItemState(btn,id)

	return btn , btn:getMenuItemSpriteExpand();
end

--Note: 插入数据项到List中 2列的List
function DailyRewardMainLayer:insertItemToList(scrollList,id_1,id_2,buttonIndex)
	--print("--------- DailyRewardMainLayer:insertItemToList id1 id2 " .. id_1 .. "/" .. id_2)

	--Note: 创建一个背景ListeItem作为子Button的父节点
    local strBin = "UI/ui_dailyreward.bin"
    local bgListItemIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_LIST";	
	local normal = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName)
	local button = scrollList:AddNewItem(normal, clicked, "", "", ccc3(255,255,255), -130);
    local parentControl = button:getAnimationNode()

	local noramlChildItemNormalIconBgName = ""
	local clickChildItemNormalIconBgName = ""
	
	local childBtn = nil
	local childBtnMenuItem = nil

	if nil ~= id_1 then
		local state = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemState(id_1)
		if nil ~= state then
			noramlChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1"
			clickChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
			childBtn , childBtnMenuItem = self:insertOneChildListItemToList(id_1,scrollList,button,bgListItemIconBgName,noramlChildItemNormalIconBgName,clickChildItemNormalIconBgName,2*(buttonIndex))
			self:SetOneButtonState(id_1)	
		end
	end

	if nil ~= id_2 then
		local state = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemState(id_2)
		if nil ~= state then
			noramlChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION2"
			clickChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION2_GOT"
			childBtn , childBtnMenuItem = self:insertOneChildListItemToList(id_2,scrollList,button,bgListItemIconBgName,noramlChildItemNormalIconBgName,clickChildItemNormalIconBgName,2*(buttonIndex)+1)
			self:SetOneButtonState(id_2)
		end
	end
end

function DailyRewardMainLayer:SetButtonMenuItemEnable(id,bEnable)
	local menuItem = self.ListButtonsMenuItems[id]
	if nil ~= menuItem then
		local nState = 1;
		menuItem:setEnabled(bEnable)
		if bEnable then
			nState = 2;
		end
		menuItem:setMenuItemDispalyState(nState)
	end
end

function DailyRewardMainLayer:IsOneButtonCanClick(id)
	local state = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemState(id)
	local type = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemType(id)
	if 2 == type then
		return true
	elseif 1 == type then
		if 1 == state then
			return true
		end
	end
	return false
end

function DailyRewardMainLayer:SetOneButtonState(id)
	local bEnable = self:IsOneButtonCanClick(id)
	self:SetButtonMenuItemEnable(id,bEnable)
end

function DailyRewardMainLayer:FlushOneListItem(id)
	if nil == self.LayerReward then
		return 
	end
	
	self:SetOneButtonState(id)

	local btn = self.ListButtons[id]
	if nil ~= btn then
		self:ShowOneItemName(btn,id)
		self:ShowOneItemState(btn,id)
	end
end

function DailyRewardMainLayer:ShowOneItemIcon(button,id)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local binName = "UI/ui_dailyreward.bin"
	local bgIconName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_DAILYREWARD_ICON_FUNCTION1"

	--Note: 显示图标
	AspriteManager:getInstance():getFramesRelativePos(binName,bgIconName,childFrameName,anchorPoint,relativePoint);
	local iconName,binName = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemIcon(id);
	if nil ~= iconName then
		local icon = AspriteManager:getInstance():getOneFrame(binName,iconName)		
		icon:setPosition(relativePoint);
		button:getAnimationNode():addChild(icon,1);
	end	
end

function DailyRewardMainLayer:ShowOneItemName(button,id)
	local bgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_DAILYREWARD_TEXT_FUNCTIONNAME1"
	local content = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemName(id)
	local txtColor = ccc3(255,255,255)
	local anchorPoint = CCPoint(0.5,0.5);
	self:ShowOneLabel(bgName,childFrameName,content,24,txtColor,anchorPoint,button:getAnimationNode(),10,id,kCCTextAlignmentCenter);
end

function DailyRewardMainLayer:ShowOneItemState(button,id)
	local bgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_DAILYREWARD_TEXT_FUNCTIONSTATE1"
	local content = ""
	local txtColor = ccc3(255,255,255)
	local anchorPoint = CCPoint(0.5,0.5);
	local state = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemState(id)
	local type = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemType(id)
	if 1 == type then
		if state == 2 then
			content = LanguageLocalization:GetLocalization("M_DAILYREWARD_ALREADYGOT")
		elseif state == 1 then
			content = LanguageLocalization:GetLocalization("M_DAILYREWARD_WAITFORGET")
		end
	else
		if state == 1 then
			content = LanguageLocalization:GetLocalization("M_DAILYREWARD_WAITFORGET")
		else
			content = ""
		end
	end
	self:ShowOneLabel(bgName,childFrameName,content,24,txtColor,anchorPoint,button:getAnimationNode(),10,id+1,kCCTextAlignmentCenter);
end

--Note: 显示文本函数
function DailyRewardMainLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local binName = "UI/ui_dailyreward.bin"
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

--Note: 显示是否有每日奖励可以领取
local staminaReward = 0
function DailyRewardMainLayer:ShowRewardTipIconOrNot()
	--local bShowOrNot = false
	--local dataCenterTable = dailyRewardMainLayerDataCenter.Items
	--local timeBool = DailyRewardStaminaLayer:getTimeAttrubeBool() 
	--
	--if timeBool == true then
	--    staminaReward = 1 
	--    local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	--    local dailyRewardButton = nil
	--    if nil ~= topLayout then
	--		dailyRewardButton = topLayout:FindChildObjectByName("dailyReWard").__UIButton__
	--    end
	--	if nil ~= dailyRewardButton then
	--		
	--			dailyRewardButton:showTipIcon()
	--	end
	--	return 
	--else
	--
	--    if staminaReward == 2 then
	--	   return
	--	end
	--	
	--end
	--
	--
	--for index,value in pairsByKeys(dataCenterTable) do
	--	local id = value.id
	--	local state = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemState(id)
	--	local type = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemType(id)
	--	if 1 == type and state == 1 then
	--		bShowOrNot = true
	--		break
	--	end
	--end
	--
	--local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	--local dailyRewardButton = nil
	--if nil ~= topLayout then
	--	local control = topLayout:FindChildObjectByName("dailyReWard")
	--	if control ~= nil then
	--		dailyRewardButton = control.__UIButton__
	--	end
	--end
	--
	--if dailyRewardButton then
	--	if bShowOrNot then
	--	   staminaReward = 1 
	--		dailyRewardButton:showTipIcon()
	--	else
	--	    staminaReward = 2
	--		dailyRewardButton:hideTipIcon()
	--	end
	--end
end

function onDailyRewardMainLayerOneItemClickEvent(nTag)
	--print("onDailyRewardMainLayerOneItemClickEvent" .. nTag)
	--Note: 注释部分为设置ListItem的选中状态之间的切换
	--if DailyRewardMainLayer.SelectItemIndex == 0 then
		--DailyRewardMainLayer.SelectItemIndex = nTag
		--DailyRewardMainLayer.ListButtonsMenuItems[nTag]:selected()
	--else
		--DailyRewardMainLayer.ListButtonsMenuItems[DailyRewardMainLayer.SelectItemIndex]:unselected()
		--DailyRewardMainLayer.SelectItemIndex = nTag
		--DailyRewardMainLayer.ListButtonsMenuItems[nTag]:selected()
	--end

	DailyRewardMainLayer.SelectItemIndex = nTag
	if false == DailyRewardMainLayer:IsOneButtonCanClick(nTag) then
		return 
	end

	local childLayer = nil

	if 6000 == nTag then	--Note: 每日薪金
		childLayer = onDailyReward_Salary()
	elseif 6001 == nTag then	--Note: 每日登陆奖励
		childLayer = onDailyReward_Login()
	elseif 6003 == nTag then	--Note: 每日竞技场奖励
		childLayer = onDailyReward_PVAI()
	elseif 6004 == nTag then	--Note: 每日自动挂机
		childLayer = onDailyReward_OnHold()
	elseif 6005 == nTag then	--Note: VIP
		childLayer = onDailyReward_VIP()
    elseif 6010 == nTag then	--Note: 面包作坊
		childLayer = onDailyReward_RECUPERATE()
    elseif 6030 == nTag then
		childLayer = onExchangeCode_Enter()
	--elseif 6006 == nTag then	--Note: da zhuan pan 
		--childLayer = onDailyReward_dazhuanpan()
	--elseif 6002 == nTag then	--Note: yong zhe bao zhang
		--childLayer = onDailyReward_yzbz()
	end

	if nil ~= childLayer then
		--Note: 隐藏当前页面
		DailyRewardMainLayer:ShowOrNot(false)
	end
end

--Note: 每日薪金
function onDailyReward_RECUPERATE()
	print("DailyRewardStaminaLayer")
	return DailyRewardStaminaLayer:CreateLayer(nil)
end
--Note: 兑换码
function onExchangeCode_Enter()
	print("onExchangeCode")
	return ExchangeCodelUILayer:createLayer()
end

--Note: 每日薪金
function onDailyReward_Salary()
	print("onDailyloginReward")
	DailyRewardSalaryLayer.IsShowMainSalaryLayer = true
	return DailyRewardSalaryLayer:CreateLayer(nil)
end

--Note: 每日登陆奖励
function onDailyReward_Login()
	print("onDailyReward_Login")
	return ThreeDaysUI:CreateLayer(nil)
end

--Note: 每日竞技场奖励
function onDailyReward_PVAI()
	print("onDailyReward_PVAI")
	return DailyRewardPvaiLayer:CreateLayer(nil)
end

--Note: 每日自动挂机
function onDailyReward_OnHold()
	print("onDailyReward_OnHold")
	return offlineExp:CreateLayer(nil)
end
--Note: 每日自动挂机
function onDailyReward_yzbz()
	print("onDailyReward_yzbz")
	return yongZheBaoZhang:CreateLayer(nil)
end

--Note: da zhuan pan
function onDailyReward_dazhuanpan()
	print("onDailyReward_dazhuanpan")
	lotteryUI:CreateLayer(nil)
	return nil
end

--Note: VIP
function onDailyReward_VIP()
	print("onDailyReward_VIP")
	vipReward:CreateLayer(nil)
	return nil
end

--Note: 关闭奖励页面的回调
function onDailyRewardMainLayerCloseClick(pObj)
	if nil ~= DailyRewardMainLayer.LayerInstance then
		SetChatBar(true,-1);
		RemoveOneLayer(DailyRewardMainLayer.LayerInstance)
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyRewardMainLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyRewardMainLayer.LayerInstance = nil
		DailyRewardMainLayer:ResetValue()
	end
end


local function closeCallBack(message)
	if nil ~= DailyRewardMainLayer.LayerInstance then
		if G_CurLayerInstance ~= DailyRewardMainLayer.LayerInstance then
			onDailyRewardMainLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		onDailyRewardMainLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	elseif message == GM_E_ENTER_MAINLAND then
		if DailyRewardMainLayer.LayerInstance ~= nil then
			DailyRewardMainLayer:ShowRewardTipIconOrNot();
		end		
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);