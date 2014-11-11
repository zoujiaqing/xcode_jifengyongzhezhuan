require("Script/ChatToolLayer")
--require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/Activity/dailyActivityLayerDataCenter")
require("Script/Instance/HeroInstanceMgr")

DailyActivityLayer = {
	LayerInstance = nil,
    LayerReward = nil,

    ListButtons = {},
	ListButtonsMenuItems = {},

	SelectItemIndex = 0,
}

function DailyActivityLayer:ResetValue()
	self.LayerInstance = nil
    self.LayerReward = nil

    self.ListButtons = {}
	self.ListButtonsMenuItems = {}

	self.SelectItemIndex = 0

	ActivityLayerDataCenter:ResetValue();
end

local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("ActivityUI")
	DailyActivityLayer:ResetValue()
end

function DailyActivityLayer:CreateLayer(parentLayer)
    print("DailyActivityLayer:CreateLayer")
	self:ResetValue()
	ActivityLayerDataCenter:InitAllData()
    
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
    
    self.LayerReward = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ActivityUI.plist",self.LayerInstance, "ActivityUI", true)
       
    if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["DailyActivityLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
	end
	PushOneLayer(self.LayerInstance,"","")    

	self:Init()    
    self:setupScrollist()
end

function DailyActivityLayer:Init()
    local bnt_close = self.LayerReward:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
    bnt_close:registerScriptTapHandler(onDailyActivityLayerCloseClick)
    
	--Note: 设置标题
    local label_tile = self.LayerReward:FindChildObjectByName("text_tile").__UILabel__;
    label_tile:setString(LanguageLocalization:GetLocalization("M_ACTIVITY_TITLE"))

	waitForActivityReceivedTimeMsg();
end

function DailyActivityLayer:setupScrollist()

    local scrollList = self.LayerReward:FindChildObjectByName("scrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end
	scrollList:clearChildrens()
    self.ListButtons = {}
	self.ListButtonsMenuItems = {}

	local dataCenterTable = ActivityLayerDataCenter.Items
	local itemsCount = GetTableCount(dataCenterTable)
	local itemCount = 0
	local id_1 = nil
	local id_2 = nil
	for index,value in pairsByKeys(dataCenterTable) do
		itemCount = itemCount + 1

		local id = value.id
		if id_1 == nil then
			id_1 = id
		elseif id_2 == nil then
			id_2 = id
		end

		if id_1 ~= nil and id_2 ~= nil then
			self:insertItemToList(scrollList,id_1,id_2)
			id_1 = nil
			id_2 = nil
		else
			if(itemCount == itemsCount) then
				self:insertItemToList(scrollList,id_1,id_2)
				id_1 = nil
				id_2 = nil
			end
		end
	end	
end

function DailyActivityLayer:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

function DailyActivityLayer:insertOneChildListItemToList(id,scrollList,parentNode,bgIcon,normalIcon,clickIcon)
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
	local btn = TXGUI.UIButton:newExpand("", 0, 0, scrollList, menuItem, nil,-130, "");
	local parentControlPos = parentNode:getPosition()
	local newBtnPos = ccp(parentControlPos.x + relativePoint.x , parentControlPos.y + relativePoint.y)
	btn:getAnimationNode():setPosition(newBtnPos);

	--Note: 定义回调接口
	self.ListButtons[id] = btn
	local btnMenuItem = btn:getMenuItemSpriteExpand()
	self.ListButtonsMenuItems[id] = btnMenuItem
	btnMenuItem:setTag(id)
	btnMenuItem:registerScriptTapHandler(onDailyActivityLayerOneItemClickEvent)

	--Note: 插入子控件
	self:ShowOneItemIcon(btn,id)
	self:ShowOneItemName(btn,id)
	self:ShowOneItemDescription(btn,id)
	self:ShowOneItemOpenOrNot(btn,id)
	self:ShowOneItemTopDescription(btn,id)

	return btn , btn:getMenuItemSpriteExpand();
end

--Note: 插入数据项到List中 2列的List
function DailyActivityLayer:insertItemToList(scrollList,id_1,id_2)
	--print("--------- DailyRewardMainLayer:insertItemToList id1 id2 " .. id_1 .. "/" .. id_2)

	--Note: 创建一个背景ListeItem作为子Button的父节点
    local strBin = "UI/ui_dailyreward.bin"
    local bgListItemIconBgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_LIST";	
	local normal = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName)
	local button = scrollList:AddNewItem(normal, clicked, "", "", ccc3(255,255,255), -130);
    local parentControl = button:getAnimationNode()

	local noramlChildItemNormalIconBgName = ""
	local clickChildItemNormalIconBgName = ""
	
	local childBtn = nil
	local childBtnMenuItem = nil

	if nil ~= id_1 then
		local state = ActivityLayerDataCenter:GetOneDailyActivityItemState(id_1)
		if nil ~= state then
			noramlChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1"
			clickChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1_LOCK"
			childBtn , childBtnMenuItem = self:insertOneChildListItemToList(id_1,scrollList,button,bgListItemIconBgName,noramlChildItemNormalIconBgName,clickChildItemNormalIconBgName)
			self:SetOneButtonState(id_1)	
		end
	end

	if nil ~= id_2 then
		local state = ActivityLayerDataCenter:GetOneDailyActivityItemState(id_2)
		if nil ~= state then
			noramlChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION2"
			clickChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION2_LOCK"
			childBtn , childBtnMenuItem = self:insertOneChildListItemToList(id_2,scrollList,button,bgListItemIconBgName,noramlChildItemNormalIconBgName,clickChildItemNormalIconBgName)
			self:SetOneButtonState(id_2)
		end
	end
end

function DailyActivityLayer:SetButtonMenuItemEnable(id,bEnable)
	local menuItem = self.ListButtonsMenuItems[id]
	if nil ~= menuItem then
		local nState = 1;
		menuItem:setEnabled(bEnable)
		if bEnable then
			nState = 2;
		end
		self:PlayOneListItemEffect(id,GetBlinkStateByActivityId(id)) --播放特效
		menuItem:setMenuItemDispalyState(nState)
	end
end

function DailyActivityLayer:IsOneButtonCanClick(id)
	local state = ActivityLayerDataCenter:GetOneDailyActivityItemState(id)
	if state == 1 then
		return true
	end
	return false
end

function DailyActivityLayer:showLevelErrorTips(level)
	GameApi:showMessage(GetLuaLocalization("M_ACTIVITY_LEVEL_ERROR_1") .. level .. GetLuaLocalization("M_ACTIVITY_LEVEL_ERROR_2"))
end

function DailyActivityLayer:SetOneButtonState(id)
	local bEnable = self:IsOneButtonCanClick(id)
	self:SetButtonMenuItemEnable(id,bEnable)
end

function DailyActivityLayer:FlushAllItem()
	if nil == self.LayerReward then
		return
	end
	local dataCenterTable = ActivityLayerDataCenter.Items
	for index,value in pairsByKeys(dataCenterTable) do
		self:FlushOneListItem(value.id)
	end
end

function DailyActivityLayer:FlushOneListItem(id)
	if nil == self.LayerReward then
		return 
	end
	
	self:SetOneButtonState(id)

	local btn = self.ListButtons[id]
	if nil ~= btn then
		--self:ShowOneItemName(btn,id)
		--self:ShowOneItemState(btn,id)
	end
end

--播放特效
function DailyActivityLayer:PlayOneListItemEffect(id,bEnable)
	local btn = self.ListButtons[id]
	if nil ~= btn and isActivityTypeNotZero(id) then
		local particle = btn:getAnimationNode():getChildByTag(10000)
		if nil ~= particle then
			btn:getAnimationNode():removeChild(particle, true)
		end
		
		if bEnable then
			particle = ParticleManagerX:sharedManager():getParticles_uieffect("particle_effect_taskguide")
            if nil ~= particle then
                --particle:setPosition(ccp(btn:getAnimationNode():getContentSize().width * 0.5,btn:getAnimationNode():getContentSize().height * 0.5));
                local x, y = btn:getAnimationNode():getChildByTag(10001):getPosition()
                particle:setPosition( ccp(x , y) )
                btn:getAnimationNode():addChild(particle,10,10000)           
            end

		end
	end
end


function DailyActivityLayer:ShowOneItemIcon(button,id)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local binName = "UI/ui_dailyreward.bin"
	local bgIconName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_ACTIVITY_ICON_FUNCTION1"

	--Note: 显示图标
	AspriteManager:getInstance():getFramesRelativePos(binName,bgIconName,childFrameName,anchorPoint,relativePoint);
	local iconName,binName = ActivityLayerDataCenter:GetOneActivityItemIcon(id)
	if nil ~= iconName then
		local icon = AspriteManager:getInstance():getOneFrame(binName,iconName)		
		icon:setPosition(relativePoint);
		button:getAnimationNode():addChild(icon,1,10001);
	end	
end

function DailyActivityLayer:ShowOneItemName(button,id)
	local bgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_ACTIVITY_TEXT_FUNCTIONNAME1"
	local content = ActivityLayerDataCenter:GetOneActivityItemName(id)
	local txtColor = ccc3(255,255,255)
	local anchorPoint = CCPoint(0.5,0.5);
	self:ShowOneLabel(bgName,childFrameName,content,24,txtColor,anchorPoint,button:getAnimationNode(),10,id,kCCTextAlignmentCenter);
end

function DailyActivityLayer:ShowOneItemTopDescription(button,id)
	local bgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_ACTIVITY_TEXT_FUNCTIONTIME1"
	local content = ActivityLayerDataCenter:GetOneActivityItemTimeDescription(id)
	local txtColor = ccc3(255,255,255)
	local anchorPoint = CCPoint(0.5,0.5);
	self:ShowOneLabel(bgName,childFrameName,content,20,txtColor,anchorPoint,button:getAnimationNode(),10,id+3,kCCTextAlignmentLeft);
end

function DailyActivityLayer:ShowOneItemDescription(button,id)
	local bgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_ACTIVITY_TEXT_DESCRIPTION1"
	local content = ActivityLayerDataCenter:GetOneActivityItemDescription(id)
	local txtColor = ccc3(255, 211, 93) -- 255, 211, 93
	local anchorPoint = CCPoint(0.5,0.5);
	self:ShowOneLabel(bgName,childFrameName,content,20,txtColor,anchorPoint,button:getAnimationNode(),11,id+1,kCCTextAlignmentCenter);
end

function DailyActivityLayer:ShowOneItemOpenOrNot(button,id)
	local bgName = "map_ui_dailyreward_FRAME_ACTIVITY_BUTTON_FUNCTION1"
	local childFrameName = "map_ui_dailyreward_FRAME_ACTIVITY_TEXT_FUNCTIONSTATE1"
	local content,txtColor = ActivityLayerDataCenter:GetOneActivityItemStateContent(id)
	local anchorPoint = CCPoint(0.5,0.5);
	self:ShowOneLabel(bgName,childFrameName,content,20,txtColor,anchorPoint,button:getAnimationNode(),12,id+2,kCCTextAlignmentCenter);
end

--Note: 显示文本函数
function DailyActivityLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
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


function onDailyActivityLayerOneItemClickEvent(nTag)
	print("onDailyActivityLayerOneItemClickEvent" .. nTag)

	DailyActivityLayer.SelectItemIndex = nTag
	if false == DailyActivityLayer:IsOneButtonCanClick(nTag) then
		return 
	end

	--判断等级够不够
	local isCan, level = ActivityLayerDataCenter:IsUserLevelCanEnter(nTag)
	if isCan == false then
		DailyActivityLayer:showLevelErrorTips(level)
		return
	end

	local childLayer = nil

	local itemType = itemTypeList[nTag] --ActivityLayerDataCenter中定义
	if itemType == nil then
		return
	end

	if	itemType.type == "pvp" then
		onPVPLayerOpen()
	elseif itemType.type == "ppve" then
		onPPVELayerOpen()
	elseif itemType.type == "tower_defence" then
		onCityDefendOpen()
	elseif itemType.type == "world_boss_noon" or itemType.type == "world_boss_night" then 
		onWorldBossOpen()
	elseif itemType.type == "pvptop" then 
		onPvpTopOpen()
    elseif itemType.type == "hero_instance" then 
		onHeroInstanceOpen()
	elseif itemType.type == "activity_undergroundcity" then
		onUndergroundOpen()
	end

end

--Note: PVP
function onPVPLayerOpen()
	print("------------ onPVPLayerOpen")

	local mainLayer = MainMenuLayer:getMainMenuLayer()
	if nil ~= mainLayer then
		mainLayer:ShowPVPLayer()
	end
end

--Note: PPVE
function onPPVELayerOpen()
	print("------------- onPPVELayerOpen")

	local mainLayer = MainMenuLayer:getMainMenuLayer()
	if nil ~= mainLayer then
		mainLayer:ShowPPVELayer()
	end
end

--Note: 主城防守
function onCityDefendOpen()
	print("------------- onCityDefendOpen")
	--InstanceManager:Get():onEnterSpecialEntrance(ENTERANCE_CITYDEFEND)
    OpenCityDefenceRoomList()
end

--Note: 世界boss
function onWorldBossOpen()
	print("------------- onWorldBossOpen")
	--InstanceManager:Get():onEnterSpecialEntrance(ENTERANCE_WORLDBOSS)
    OpenCoinBossRankLayer()
end

--Note:pvptop 
function onPvpTopOpen()
	print("------------- onPvpTopOpen")
	if not TimeManager:Get():isInited() then
		PushOneWaitingLayer("CSPingReq")
		return
	end

	InstanceManager:Get():onEnterSpecialEntrance(ENTERANCE_BESTFIGHTER)
end

-- 勇者试炼
function onHeroInstanceOpen()
	print("------------- onHeroInstanceOpen")
    HeroInstanceMgr:Init()
    HeroInstanceListLayer:CreateLayer()
end

-- 地下城
function onUndergroundOpen()
	InstanceManager:Get():onEnterSpecialEntrance(ENTERANCE_UNDERGROUND)
end

--Note: 关闭奖励页面的回调
function onDailyActivityLayerCloseClick(pObj)
	if nil ~= DailyActivityLayer.LayerInstance then
		--SetChatBar(true,-1);		
		RemoveOneLayer(DailyActivityLayer.LayerInstance)
		--TXGUI.UIManager:sharedManager():removeUILayout("ActivityUI")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyActivityLayer.LayerInstance:removeFromParentAndCleanup(true)
		DailyActivityLayer.LayerInstance = nil
		DailyActivityLayer:ResetValue()
	end
end

local function closeCallBack(message)
	if nil ~= DailyActivityLayer.LayerInstance then
		if G_CurLayerInstance ~= DailyActivityLayer.LayerInstance then
			onDailyActivityLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		onDailyActivityLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
