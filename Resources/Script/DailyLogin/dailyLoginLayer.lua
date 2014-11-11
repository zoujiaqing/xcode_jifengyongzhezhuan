require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/DailyLogin/dailyLoginLayerDataCenter")
require("Script/DailyLogin/dailyLoginNetWorkHandler")

DailyLoginLayer = {
	LayerInstance = nil,
    Layout = nil,

    ListButtons = {},
	ListButtonsMenuItems = {},

	SelectButtonIndex = 0,
}

function DailyLoginLayer:ResetValue()
	self.LayerInstance = nil
    self.Layout = nil

    self.ListButtons = {}
	self.ListButtonsMenuItems = {}

	self.SelectButtonIndex = 0
end

local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("DailyLogin")
	DailyLoginLayer:ResetValue()
end


function DailyLoginLayer:CreateLayer(parentLayer)
    --print("DailyRewardMainLayer:CreateLayer")
	self:ResetValue()
	dailyLoginLayerDataCenter:ResetValue()
	dailyLoginLayerDataCenter:InitAllData()
    
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
    
    self.Layout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/DailyLogin.plist",self.LayerInstance, "DailyLogin", true)
       
    if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["DailyLoginLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
	end

	self:Init()    
    self:setupScrollist()

	return self.LayerInstance
end

function DailyLoginLayer:Init()
    local bnt_close = self.Layout:FindChildObjectByName("loginCloseBtn").__UIButton__:getMenuItemSprite();
    bnt_close:registerScriptTapHandler(onDailyLoginLayerCloseClick)
    
	--Note: 设置标题
    local label_tile = self.Layout:FindChildObjectByName("login_text_tile").__UILabel__;
    label_tile:setString(LanguageLocalization:GetLocalization("M_DAILYREWARD_TITLE"))
end

function DailyLoginLayer:FlushLoginRewardLayer()
	if nil == self.Layout then
		return 
	end
	self:setupScrollist()
end

function DailyLoginLayer:setupScrollist()
	if nil == self.Layout then
		return 
	end

    local scrollList = self.Layout:FindChildObjectByName("scrollList").__UIScrollList__
	if nil == scrollList then
		print("DailyLoginLayer:setupScrollist() scroll list nil")
		return 
	end

	scrollList:clearChildrens()
    self.ListButtons = {}
	self.ListButtonsMenuItems = {}

	local dataCenterTable = dailyLoginLayerDataCenter.Items
	local itemsCount = GetTableCount(dataCenterTable)
	--print("---------- count " .. itemsCount)
	local itemCount = 0
	local id_1 = nil
	local id_2 = nil
	for index,value in pairsByKeys(dataCenterTable) do
		--print("-------------- insert index " .. index)
		itemCount = itemCount + 1

		local id = value.itemId
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

function DailyLoginLayer:insertOneChildListItemToList(id,scrollList,parentNode,bgIcon,normalIcon,bLeft)
	local strBin = "UI/ui_dailyreward.bin"
	--Note: 得到子Button的相对位置
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	AspriteManager:getInstance():getFramesRelativePos(strBin,bgIcon,normalIcon,anchorPoint,relativePoint);
	--Note:创建一个按钮
	local normalSprite = AspriteManager:getInstance():getOneFrame(strBin,normalIcon)
	normalSprite:setPosition(relativePoint);
	parentNode:addChild(normalSprite,1)

	--Note: 插入子控件
	self:ShowOneItemIcon(normalSprite,id)
	self:ShowOneItemName(normalSprite,id)
	self:ShowOneItemRewards(normalSprite,id)
	self:ShowOneItemRewardBtn(scrollList,parentNode,id,bLeft)
end

--Note: 插入数据项到List中 2列的List
function DailyLoginLayer:insertItemToList(scrollList,id_1,id_2)
	--print("--------- DailyLoginLayer:insertItemToList id1 id2 " .. id_1 .. "/" .. id_2)

	--Note: 创建一个背景ListeItem作为子Button的父节点
    local strBin = "UI/ui_dailyreward.bin"
	local bgListItemIconBgName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_LINE1";	
	local bgListItemIcon = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName);
	local bgListItemSize = bgListItemIcon:getContentSize();		
	local ListItemNode = CCNode:create();
	local parentControl = scrollList:AddBaseItem(bgListItemSize,ListItemNode);

	local noramlChildItemNormalIconBgName = ""
	local clickChildItemNormalIconBgName = ""
	
	local childBtn = nil
	local childBtnMenuItem = nil

	if nil ~= id_1 then
		noramlChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_REWARDDAY1"
		self:insertOneChildListItemToList(id_1,scrollList,ListItemNode,bgListItemIconBgName,noramlChildItemNormalIconBgName,true)
	end

	if nil ~= id_2 then
		noramlChildItemNormalIconBgName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_REWARDDAY2"
		self:insertOneChildListItemToList(id_2,scrollList,ListItemNode,bgListItemIconBgName,noramlChildItemNormalIconBgName,false)
	end
end

function DailyLoginLayer:ShowOneItemIcon(parentNode,id)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local bgSize = parentNode:getContentSize();	
	local binName = "UI/ui_dailyreward.bin"
	local bgIconName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_REWARDDAY1"
	local childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_ICON_REWARDDAY1"

	----Note: 显示图标
	AspriteManager:getInstance():getFramesRelativePos(binName,bgIconName,childFrameName,anchorPoint,relativePoint);
	local iconName,binName = dailyLoginLayerDataCenter:GetOneItemIcon(id);
	if nil ~= iconName then
		local icon = AspriteManager:getInstance():getOneFrame(binName,iconName)				
		local newPos = ccp(relativePoint.x + bgSize.width/2,relativePoint.y + bgSize.height/2)
		icon:setPosition(newPos);
		parentNode:addChild(icon,1);
	end	
end

function DailyLoginLayer:ShowOneItemName(parentNode,id)
	local bgName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_REWARDDAY1"
	local childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_TEXT_REWARDDAY1"
	local content = dailyLoginLayerDataCenter:GetOneItemName(id)
	local txtColor = ccc3(255, 255, 255)
	local anchorPoint = CCPoint(0.5,0.5);
	local bgSize = parentNode:getContentSize();	
	self:ShowOneLabel(bgName,childFrameName,bgSize,content,24,txtColor,anchorPoint,parentNode,10,id,kCCTextAlignmentCenter);
end

function DailyLoginLayer:ShowOneItemRewards(parentNode,id)
	local rewardIdArray = dailyLoginLayerDataCenter.Items[id].rewardIdArray
	local rewardNumArray = dailyLoginLayerDataCenter.Items[id].rewardNumArray

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local bgName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_REWARDDAY1"
	local bgSize = parentNode:getContentSize();	
	local binName = "UI/ui_dailyreward.bin"

	local count = GetTableCount(rewardIdArray)
	for index = 1,count,1 do
		local id = rewardIdArray[index]
		local num = rewardNumArray[index]
		
		local childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_ICON_DAY1_REWARD" .. index

		local iconName = dailyLoginLayerDataCenter:GetOneTypeRewardIcon(id)
		local iconSprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,iconName)

		AspriteManager:getInstance():getFramesRelativePos(binName,bgName,childFrameName,anchorPoint,relativePoint);
		relativePoint = ccp(relativePoint.x + bgSize.width/2,relativePoint.y + bgSize.height/2)

		iconSprite:setPosition(relativePoint);
		parentNode:addChild(iconSprite,1);

		local txtColor = ccc3(255, 255, 255)
		local rewardNumContent = MainMenuLayer:GetNumByFormat(num)
		childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_NUM_DAY1_REWARD" .. index
		self:ShowOneLabel(bgName,childFrameName,bgSize,rewardNumContent,18,txtColor,anchorPoint,parentNode,10,id+1,kCCTextAlignmentCenter);
	end
end

function DailyLoginLayer:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

function DailyLoginLayer:ShowOneItemRewardBtn(scrollList,parentNode,id,bLeft)
	local state = dailyLoginLayerDataCenter:GetOneItemState(id)
	--Note:创建一个按钮	
	local binName = "UI/ui_dailyreward.bin"
	local normalIcon = "map_ui_dailyreward_FRAME_LOGINREWARD_BUTTON_GETREWARD1"
	local clickIcon = "map_ui_dailyreward_FRAME_LOGINREWARD_BUTTON_GETREWARD1_CANTGET"
	local normalSprite = AspriteManager:getInstance():getOneFrame(binName,normalIcon)
	local clickedSprite = AspriteManager:getInstance():getOneFrame(binName,clickIcon)
	local menuItem = MenuItemExpand:create(normalSprite,clickedSprite);
	local btn = TXGUI.UIButton:newExpand("", 0, 0, scrollList, menuItem, nil,-132, "");

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);	
	local bgName = "map_ui_dailyreward_FRAME_LOGINREWARD_FRAME_LINE1"
	local childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_BUTTON_GETREWARD1_CLICKED"
	if false == bLeft then
		childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_BUTTON_GETREWARD2_CLICKED"
	end
	AspriteManager:getInstance():getFramesRelativePos(binName,bgName,childFrameName,anchorPoint,relativePoint);
	local parentControlPosX,parentControlPosY = parentNode:getPosition()
	self:ScaleByScreen(relativePoint)
	local newBtnPos = ccp(parentControlPosX + relativePoint.x , parentControlPosY + relativePoint.y)
	btn:getAnimationNode():setPosition(newBtnPos)

	--Note: 定义回调接口
	self.ListButtons[id] = btn
	local btnMenuItem = btn:getMenuItemSpriteExpand()
	self.ListButtonsMenuItems[id] = btnMenuItem
	btnMenuItem:setTag(id)
	btnMenuItem:registerScriptTapHandler(onDailyLoginLayerOneItemClickEvent)

	self:FlushOneButtonState(id)

	return  btn , btn:getMenuItemSpriteExpand();
end

function DailyLoginLayer:FlushOneButtonState(id)
	--Note: 当前状态 1 可领取（高亮） 2 可领取（灰色） 3 已领取（灰色）
	local state  = dailyLoginLayerDataCenter:GetOneItemState(id)
	local languageFlag = ""
	local bDark = true
	
	if state == 1 or state == 2 then
		languageFlag = "M_UNDERGROUND_GETREWARD"
	else
		languageFlag = "M_LOGINREWARD_ALREADYGOT"
	end

	if state == 1 then
		bDark = false
	end

	local content = LanguageLocalization:GetLocalization(languageFlag)

	local btn = self.ListButtons[id]
	local btnMenuItem = self.ListButtonsMenuItems[id]
	if bDark then
		btnMenuItem:setEnabled(false)
		btnMenuItem:setMenuItemDispalyState(1)
	else
		btnMenuItem:setEnabled(true)
		btnMenuItem:setMenuItemDispalyState(2)
	end

	local bgName = "map_ui_dailyreward_FRAME_LOGINREWARD_BUTTON_GETREWARD1"
	local childFrameName = "map_ui_dailyreward_FRAME_LOGINREWARD_TEXT_GETREWARD1"
	local txtColor = ccc3(255, 255, 255)
	local anchorPoint = CCPoint(0.5,0.5);
	self:ShowOneLabel(bgName,childFrameName,nil,content,24,txtColor,anchorPoint,btn:getAnimationNode(),10,id+10,kCCTextAlignmentCenter);

	--if dailyLoginLayerDataCenter:TellOneItemIsVip(id) then		
	--end
end

--Note: 显示文本函数
function DailyLoginLayer:ShowOneLabel(bgName,uiName,bgSize,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local binName = "UI/ui_dailyreward.bin"
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(binName,bgName,uiName,anchorPoint,relativePoint);
	if bgSize ~= nil then
		relativePoint = ccp(relativePoint.x + bgSize.width/2,relativePoint.y + bgSize.height/2)
	end	
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

function DailyLoginLayer:CloseLayer()
	onDailyLoginLayerCloseClick(nil)
end

function DailyLoginLayer:OnHandleGetReward()	
	local id = self.SelectButtonIndex
	--print("=============== DailyLoginLayer:OnHandleGetReward() " .. id)
	GameApi:showMessage( LanguageLocalization:GetLocalization("M_GET_SUCCESS"));
	dailyLoginLayerDataCenter:SetOneItemState(id,3)
	self:FlushOneButtonState(id);
end

function onDailyLoginLayerOneItemClickEvent(nTag)	
	--Note: 当前状态 1 可领取（高亮） 2 可领取（灰色） 3 已领取（灰色）
	local id = nTag
	local state  = dailyLoginLayerDataCenter:GetOneItemState(id)
	if 1 == state then
		--print("============= onDailyLoginLayerOneItemClickEvent " .. nTag);
		DailyLoginLayer.SelectButtonIndex = id
		if dailyLoginLayerDataCenter:TellOneItemIsVip(id) then
			--print("================== send vip ")
			DailyLoginNetWorkHandler:SendGetDailyRewardReqMessage(true)
		else
			--print("================== send not vip ")
			DailyLoginNetWorkHandler:SendGetDailyRewardReqMessage(false)
		end		
	end
end

--Note: 关闭奖励页面的回调
local function closeLayer()
	if nil ~= DailyLoginLayer.LayerInstance then		
		CCTextureCache:sharedTextureCache():removeUnusedTextures()		
		DailyLoginLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyLoginLayer.LayerInstance = nil
		DailyLoginLayer:ResetValue()
		print("DailyLoginLayer")
		XLogicManager:sharedManager():notifyShowNextAppearUI();
	end
		
end

function onDailyLoginLayerCloseClick(pObj)
	closeLayer()
	if nil ~= DailyRewardMainLayer then
		DailyRewardMainLayer:ShowOrNot(true)
	end	
end


local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		onDailyLoginLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeLayer()
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);