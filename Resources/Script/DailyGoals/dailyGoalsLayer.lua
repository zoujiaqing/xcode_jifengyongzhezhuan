require("Script/Language")
require("Script/MainMenuUILayer")
require("Script/ChatToolLayer")
require("Script/CommonDefine")
require("Script/GameConfig/Item")
require("Script/DailyGoals/dailyGoalsNetWorkHandler")

DailyGoalsLayer = {
	LayerInstance = nil,
	Layout = nil,
	AcceptRewardBtn = nil,
	LayerTitleLabel = nil,
	TipLabel = nil,
	AcceptRewardBtnLabel = nil,
	ListButtons = {},
	ListItemButtons = {},
	SelectItemIndex = -1,
	SelectedListItem = nil,
	RewardPanelActiveNumLabel = nil,
	RewardPanelProgressIcon = nil,
	UserCoinLabel = nil,
	UserDiamondLabel = nil,

	InitScrollListCallBack = nil,
}

function DailyGoalsLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.ListButtons = {}
	self.ListItemButtons = {}
	self.SelectItemIndex = -1
	self.SelectedListItem = nil
	
	self.AcceptRewardBtn = nil
	self.LayerTitleLabel = nil
	self.AcceptRewardBtnLabel = nil

	self.RewardPanelActiveNumLabel = nil
	self.RewardPanelProgressIcon = nil

	self.UserCoinLabel = nil
	self.UserDiamondLabel = nil

	self.InitScrollListCallBack = nil
end

local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("dailyGoalsPanel")
	DailyGoalsLayer:ResetValue()
end

local function DelayLoadInitLeftScrollList()	
	DailyGoalsLayer:ShowLeftScrollList()
	if DailyGoalsLayer.InitScrollListCallBack ~= nil then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(DailyGoalsLayer.InitScrollListCallBack);
		DailyGoalsLayer.InitScrollListCallBack = nil
	end
	RemoveOneWaitingLayerByForce()
end

--Note: 创建Layer
function DailyGoalsLayer:CreateLayer(parentLayer)
	self:ResetValue()
	dailyGoalsDataCenter:InitAllData()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/dailyGoalsUI.plist",self.LayerInstance, "dailyGoalsPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
        
        local tag = UITagTable["DailyGoalsLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
        
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	--self:ShowLeftScrollList()
	self:InitRightPanel()
	self:FreshRightPanel()
	self:FreshUserCoinAndDiamondInfo();
	PushOneLayer(self.LayerInstance,"","")		

	--DailyGoalsNetWorkHandler:SendGetDailyGoalRewardReqMessage()
    --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun DailyGoalsLayer:CreateLayer")
	self.InitScrollListCallBack = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(DelayLoadInitLeftScrollList, 0.2, false);
	PushOneWaitingLayer("");
	SetChatBar(true,-1)

	dailyGoalsDataCenter:SendTaskStatuMessage()
end

function DailyGoalsLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(DailyGoalsLayerCloseBtnClick);

	self.AcceptRewardBtn = self.Layout:FindChildObjectByName("acceptRewardBtn").__UIButton__
	local acceptRewardBtnMenuItem = self.AcceptRewardBtn:getMenuItemSprite();
	acceptRewardBtnMenuItem:registerScriptTapHandler(DailyGoalsLayerAcceptRewardBtnClick);

	self.RewardPanelActiveNumLabel = self.Layout:FindChildObjectByName("activitiesTipNums").__UILabel__;
	self.LayerTitleLabel = self.Layout:FindChildObjectByName("PanelTitle").__UILabel__;
	self.TipLabel = self.Layout:FindChildObjectByName("tipLabel").__UILabel__;

	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
	self.RewardPanelProgressIcon = self.Layout:FindChildObjectByName("bar_activity").__UIPicture__;
	self.RewardPanelProgressIcon:getCurrentNode():setAnchorPoint(ccp(0.5,0))
	--self.RewardPanelProgressIcon:getCurrentNode():setScaleY(scale)
	local tmpSize = self.RewardPanelProgressIcon:getCurrentNode():getContentSize()
	local oldPos = self.RewardPanelProgressIcon:getPosition()
	self.RewardPanelProgressIcon:setPosition(ccp(oldPos.x,oldPos.y - scale * tmpSize.height / 2))

		--local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		--self.RewardPanelProgressIcon:getCurrentNode():setAnchorPoint(ccp(0.5,0));
		--self.RewardPanelProgressIcon:getCurrentNode():setScaleY(scale);
		--local oldPos = self.RewardPanelProgressIcon():getCurrentNode():getPosition();
		--self.RewardPanelProgressIcon:getCurrentNode():setPosition(ccpSub(oldPos,ccp(0,scale * self.RewardPanelProgressIcon:getCurrentNode():getContentSize().height * 0.5)));

	self.UserCoinLabel = self.Layout:FindChildObjectByName("coinNumLabel").__UILabel__;
	self.UserDiamondLabel = self.Layout:FindChildObjectByName("diamondNumLabel").__UILabel__;

	self:InitBtnLabelContent()
end

function DailyGoalsLayer:InitBtnLabelContent()
	local languageFlag = 'M_DAILYTARGET_TITLE'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	self.LayerTitleLabel:setString(content)

	local languageFlag = 'M_DAILYTARGET_TIP'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	self.TipLabel:setString(content)
end

function DailyGoalsLayer:IsListEmpty()
	local count = table.getn(self.ListButtons)
	if count == 0 then
		return true
	else 
		return false
	end
end

function DailyGoalsLayer:ShowLeftScrollList()
	if nil == self.Layout then
		return
	end

	local scrollList = self.Layout:FindChildObjectByName("leftScrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end

	self.ListButtons = {}
	self.ListItemButtons = {}
	self.SelectItemIndex = -1
	self.SelectedListItem = nil
	scrollList:clearChildrens()

	for index,value in pairsByKeys(dailyGoalsDataCenter.TaskItems) do
		local taskId = value.taskID
		self:InsertOneSkillItem(scrollList,taskId)
	end		
end

function DailyGoalsLayer:ShowOneTaskDetailPanel()
	if self.Layout == nil then
		return
	end

	local selectIndex = self.SelectItemIndex
	if -1 == selectIndex then
		return
	end
end

--Note: 显示任务状态Icon
function DailyGoalsLayer:ShowOneListItemTaskIcon(backGroundFrameName,button,taskId)
	local taskIconName,BIN_NAME = dailyGoalsDataCenter:GetOneTaskIcon(taskId)
	if nil == taskIconName then
		return 
	end

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_SYSTEM1";

	AspriteManager:getInstance():getFramesRelativePos(KDailyObjective_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
			 
	if nil ~= taskIconName then
		local taskIcon = AspriteManager:getInstance():getOneFrame(BIN_NAME,taskIconName)		
		taskIcon:setPosition(relativePoint);
		local stateIcon = button:getAnimationNode():getChildByTag(taskId + 100000)
		if nil ~= stateIcon then
			stateIcon:removeFromParentAndCleanup(true)
		end
		button:getAnimationNode():addChild(taskIcon,1,taskId + 100000);
	end
end

--Note: 显示Flag Icon(前往 完成 or nothing)
function DailyGoalsLayer:ShowOneListItemTaskFlagIcon(backGroundFrameName,button,taskId)
	if dailyGoalsDataCenter:IsOneTaskLocked(taskId) then
		return 
	end

	local flagIconName = "map_ui_system_icon_FRAME_ICON_COMPLETE"
	local childFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_COMPLETE";

	if false == dailyGoalsDataCenter:IsOneTaskCompelte(taskId) then
		flagIconName = "map_ui_system_icon_FRAME_ICON_GO"
		childFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_GO";
	end

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	

	AspriteManager:getInstance():getFramesRelativePos(KDailyObjective_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
			 
	if nil ~= flagIconName then
		local flagIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,flagIconName)
		flagIcon:setPosition(relativePoint);
		--local stateIcon = button:getAnimationNode():getChildByTag(taskId + 100000)
		--if nil ~= stateIcon then
			--stateIcon:removeFromParentAndCleanup(true)
		--end
		button:getAnimationNode():addChild(flagIcon,1);
	end
end

function DailyGoalsLayer:ShowOneListItemTaskRewardIconTips(backGroundFrameName,button,taskId)
	local rewardTips = dailyGoalsDataCenter:GetOneGoalTaskTipsInfoOnly(taskId)
	local rewardCounts = table.getn(rewardTips)

	if rewardCounts == 1 then
		local iconName = dailyGoalsDataCenter:GetIconRcsNameByName(rewardTips[1].key)
		local iconContent = LanguageLocalization:GetLocalization(rewardTips[1].content)
		self:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName,iconContent,1,2)
	elseif rewardCounts == 2 then
		local iconName_1 = dailyGoalsDataCenter:GetIconRcsNameByName(rewardTips[1].key)
		local iconContent_1 = LanguageLocalization:GetLocalization(rewardTips[1].content)		

		local iconName_2 = dailyGoalsDataCenter:GetIconRcsNameByName(rewardTips[2].key)
		local iconContent_2 = LanguageLocalization:GetLocalization(rewardTips[2].content)		

		self:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName_1,iconContent_1,2,1)
		self:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName_2,iconContent_2,2,2)
	elseif rewardCounts >= 3 then
		local iconName = dailyGoalsDataCenter:GetIconRcsNameByName(rewardTips[1].key)
		local iconContent = LanguageLocalization:GetLocalization(rewardTips[1].content)

		local iconName_2 = dailyGoalsDataCenter:GetIconRcsNameByName(rewardTips[2].key)
		local iconContent_2 = LanguageLocalization:GetLocalization(rewardTips[2].content)

		local iconName_3 = dailyGoalsDataCenter:GetIconRcsNameByName(rewardTips[3].key)
		local iconContent_3 = LanguageLocalization:GetLocalization(rewardTips[3].content)

		self:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName,iconContent,1,1)
		self:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName_2,iconContent_2,1,2)
		self:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName_3,iconContent_3,1,3)
	end
end

function DailyGoalsLayer:ShowOneListItemTaskRewardOneIconTip(backGroundFrameName,button,taskId,iconName,iconContent,type,pos)
	local frameIconName = "";
	local frameIconContent = "";
	
	if 1 == type then
		frameIconName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_REWARD" .. pos
		frameIconContent = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_NUMBER_REWARD" .. pos
	elseif 2 == type then
		frameIconName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_REWARD" .. (pos+3)
		frameIconContent = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_NUMBER_REWARD" .. (pos+3)
	end
		
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = frameIconName;

	--Note: 显示图标
	AspriteManager:getInstance():getFramesRelativePos(KDailyObjective_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
			 
	if nil ~= iconName then
		local icon = AspriteManager:getInstance():getOneFrame(KDailyObjective_BIN,iconName)		
		icon:setPosition(relativePoint);
		button:getAnimationNode():addChild(icon,1);
	end

	--Note: 显示文本
	local txtColor = ccc3(255,255,255)
	local childFrameName = frameIconContent
	self:ShowOneLabel(backGroundFrameName,childFrameName,iconContent,20,txtColor,anchorPoint,button:getAnimationNode(),10,nil,kCCTextAlignmentCenter);
end

--Note: 显示任务名称
function DailyGoalsLayer:ShowOneListItemTaskName(backGroundFrameName,button,taskId)
	local taskName = dailyGoalsDataCenter:GetOneGoalTaskName(taskId)
	if nil == taskName then
		return 
	end

	local isTaskLock = dailyGoalsDataCenter:IsOneTaskLocked(taskId)

	local txtColor = ccc3(255,255,255)
	local additionTxt = ""
	if false == isTaskLock then
		local curStep,allSteps = dailyGoalsDataCenter:GetOneTaskProcessInfo(taskId)
		if curStep > allSteps then
			curStep = allSteps
		end
		additionTxt = "(" ..  curStep .. "/" .. allSteps .. ")"
	else
		txtColor = ccc3(134,143,153)
	end

	taskName = taskName .. additionTxt

	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_TEXT_SYSTEMNAME1"
	self:ShowOneLabel(backGroundFrameName,childFrameName,taskName,28,txtColor,anchorPoint,button:getAnimationNode(),10,taskId);
end

function DailyGoalsLayer:ShowOneListItemTaskRewardConstTips(backGroundFrameName,button,taskId)
	local txtColor = ccc3(255,255,255)
	local languageFlag = "M_DAILYTARGET_REWARD_TIP"
	local content = ""

	local rewardTips = dailyGoalsDataCenter:GetOneGoalTaskTipsInfoOnly(taskId)
	local rewardCounts = table.getn(rewardTips)
	if rewardCounts ~= 0 then
		content = LanguageLocalization:GetLocalization(languageFlag)
	end

	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_TEXT_REWARDTIP"
	self:ShowOneLabel(backGroundFrameName,childFrameName,content,20,txtColor,anchorPoint,button:getAnimationNode(),10,taskId+2,kCCTextAlignmentCenter);
end

function DailyGoalsLayer:ShowOneListItemTaskRewardInfo(backGroundFrameName,button,taskId)
	local activeNum = dailyGoalsDataCenter:GetOneGoalTaskActive(taskId)
	if nil == activeNum then
		return 
	end

	local txtColor = ccc3(40,184,252)
	local displayTxt = ""
	local isTaskLock = dailyGoalsDataCenter:IsOneTaskLocked(taskId)
	if false == isTaskLock then
		local languageFlag = "M_DAILYTARGET_COMPLETE"
		local content = LanguageLocalization:GetLocalization(languageFlag)
		displayTxt = content .. " " .. activeNum .. LanguageLocalization:GetLocalization("M_DAILYTARGET_ACTIVITY")
	else
		txtColor = ccc3(134,143,153)

		local unLockLevel = dailyGoalsDataCenter:GetOneGoalTaskUnLockHeroLevel(taskId)
		displayTxt = "Lv:" .. " " .. unLockLevel .. LanguageLocalization:GetLocalization("M_DAILYTARGET_OPENLEVEL")
	end

	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_TEXT_COMPLETETASK_ACTIVITY1"
	self:ShowOneLabel(backGroundFrameName,childFrameName,displayTxt,20,txtColor,anchorPoint,button:getAnimationNode(),10,taskId+1);
	
end

function DailyGoalsLayer:InsertOneSkillItem(scrollList,taskId)
	if nil == scrollList then
		return 
	end

	print("Insert One Item " .. taskId)

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	--Note: 定义背景框大小
	local backGroundFrameName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_BUTTON_SYSTEM1";
	-- Note: List Item 
	local normalListItemIconBgName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_BUTTON_SYSTEM1";
	local clickListItemIconBgName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_BUTTON_SYSTEM1";
	local normal = AspriteManager:getInstance():getOneFrame(KDailyObjective_BIN,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(KDailyObjective_BIN,clickListItemIconBgName)
		
	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);

	self:ShowOneListItemTaskFlagIcon(backGroundFrameName,button,taskId)
	self:ShowOneListItemTaskIcon(backGroundFrameName,button,taskId);
	self:ShowOneListItemTaskName(backGroundFrameName,button,taskId);		
	self:ShowOneListItemTaskRewardInfo(backGroundFrameName,button,taskId);
	self:ShowOneListItemTaskRewardConstTips(backGroundFrameName,button,taskId);
	self:ShowOneListItemTaskRewardIconTips(backGroundFrameName,button,taskId);

	local menuItem = button:getMenuItemSprite();
	menuItem:registerScriptTapHandler(onDailyGoalsLayerListItemClick);

	local listItemCount = table.getn(self.ListItemButtons);
	self.ListItemButtons[listItemCount+1] = menuItem
	
	local listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = button
end

--Note: 显示文本函数
function DailyGoalsLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(KDailyObjective_BIN,bgName,uiName,anchorPoint,relativePoint);
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

--Note: 刷新领取奖励按钮状态
function DailyGoalsLayer:FreshGetRewardButtonState()
	if self.Layout == nil then
		return 
	end
	
	if dailyGoalsDataCenter:IsCanGetReward() then
		self.AcceptRewardBtn:setEnabled(true)
	else
		self.AcceptRewardBtn:setEnabled(false)
	end
end

--Note: 刷新右侧奖励面板
function DailyGoalsLayer:FreshRightPanel()
	if self.Layout == nil then
		return
	end

	local isCanGetReward = true
	if dailyGoalsDataCenter.GetRewardTimes <= 0 then
		isCanGetReward = false
	end

	--Note: 确定领取奖励按钮的颜色
	self:FreshGetRewardButtonState();

	--Note: 显示当前活跃度值
	if nil ~= self.RewardPanelActiveNumLabel then
		self.RewardPanelActiveNumLabel:setString(dailyGoalsDataCenter.ActiveNum .. "")
	end

	--Note: 显示活跃度进度条
	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
	local progress,validNums = dailyGoalsDataCenter:GetActiveProgessAndValidRewardItemsNum()
	if nil ~= self.RewardPanelProgressIcon then
		self.RewardPanelProgressIcon:setScale(scale,progress*scale)
	end

	local containerName = ""
	local container = nil

	local activityItemNum = table.getn(dailyGoalsDataCenter.ActiveRewardItems)
	for index = 1,activityItemNum,1 do
		containerName = "markRewardContainer_" .. index
		container = self.Layout:FindChildObjectByName(containerName).__UIContainer__;

		local normalMarkIcon = container:FindChildObjectByName("normalMarkIcon").__UIPicture__;
		local cangetMarkIcon = container:FindChildObjectByName("cangetMarkIcon").__UIPicture__;
		local gettedMarkIcon = container:FindChildObjectByName("gettedMarkIcon").__UIPicture__;

		local activeLabelName = "rewardNeedActivities_" .. index
		local activeLabel = self.Layout:FindChildObjectByName(activeLabelName).__UILabel__;

		normalMarkIcon:setVisible(true)
		cangetMarkIcon:setVisible(true)
		gettedMarkIcon:setVisible(true)

		if validNums >= index then
			normalMarkIcon:setVisible(false)
			local rewardGetTimes = dailyGoalsDataCenter.GetRewardTimes
			if rewardGetTimes >= index then
				cangetMarkIcon:setVisible(false)
			else
				gettedMarkIcon:setVisible(false)
			end

			activeLabel:setColor(ccc3(255,255,255))
		else
			cangetMarkIcon:setVisible(false)
			gettedMarkIcon:setVisible(false)

			activeLabel:setColor(ccc3(0,0,0))
		end
	end
end

function DailyGoalsLayer:InitRightPanel()
	for index,value in pairs(dailyGoalsDataCenter.ActiveRewardItems) do
		local id = value.itemId
		local activeNum = value.activeNum
		local rewardId = value.rewardId
		local rewardNum = value.rewardNum
		local rewardIconName = value.rewardIconName

		local activeLabelName = ""
		local containerName = "markRewardContainer_1"
		local container = nil

		activeLabelName = "rewardNeedActivities_" .. id
		containerName = "markRewardContainer_" .. id

		local activeLabel = self.Layout:FindChildObjectByName(activeLabelName).__UILabel__;
		activeLabel:setString("" .. activeNum)

		container = self.Layout:FindChildObjectByName(containerName).__UIContainer__;

		--不再设置每日目标右侧icon
		--[[
		local markIcon = container:FindChildObjectByName("markIcon").__UIPicture__;
		if nil ~= markIcon then
			local markIconName = nil
			if rewardIconName == nil then
				markIconName = "map_ui_system_icon_FRAME_" .. Items[rewardId].LookFace
			else
				markIconName = "map_ui_system_icon_FRAME_" .. string.upper(rewardIconName)
			end
			local icon = AspriteManager:getInstance():getOneFrame(KICON_BIN,markIconName)
			if nil == icon then
				icon = AspriteManager:getInstance():getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_499999")
			end
			markIcon:setSprite(icon)
		end
		]]

		local markNumber = container:FindChildObjectByName("rewardNumLabel").__UILabel__;
		local markNumberContent = MainMenuLayer:GetNumByFormat(rewardNum)
		markNumber:setString("" .. markNumberContent)
	end
end

function DailyGoalsLayer:FreshUserCoinAndDiamondInfo()
	if nil ~= self.Layout then
		if nil ~= self.UserCoinLabel and nil ~= self.UserDiamondLabel then
			local money = UserData:GetUserInfo().m_gold
			local moneyContent = MainMenuLayer:GetNumByFormat(money)
			self.UserCoinLabel:setString("" .. moneyContent)
			local diamond = UserData:GetUserInfo().m_diamond
			--local diamondContent = MainMenuLayer:GetNumByFormat(diamond)
			self.UserDiamondLabel:setString("" .. diamond)
		end
	end
end

function onDailyGoalsLayerListItemClick(nTag)
	--print("++++++ onDailyGoalsLayerListItemClick " .. nTag)

	local listItemIndex = nTag+1
	DailyGoalsLayer.SelectItemIndex = listItemIndex	
	--DailyGoalsLayer.SelectedListItem = item

	local taskId = dailyGoalsDataCenter:GetTaskIDByIndex(DailyGoalsLayer.SelectItemIndex)
	--print("+++++++++ taskId " .. taskId)
	if nil ~= taskId then
		local name = dailyGoalsDataCenter:GetOneGoalTaskLayerName(taskId)
		--print("+++++++++ name " .. name)
		if name ~= nil then
			--print("-------------> LayerName " .. name)
			--if false == dailyGoalsDataCenter:IsOneTaskCompelte(taskId) then
			local bEnterLayer = false
			if dailyGoalsDataCenter:IsOneTaskCompelte(taskId) then
				if dailyGoalsDataCenter:GetIsCanEnterOnCompelte(taskId) then
					bEnterLayer = true
				else
					--print("-------------- DailyGoalsTask Completed " .. taskId)
				end
			else 				
				if false == dailyGoalsDataCenter:IsOneTaskLocked(taskId) then
					bEnterLayer = true
				else
					local languageFlag = 'M_UNFINISH'
					local content = LanguageLocalization:GetLocalization(languageFlag)
					GameApi:showMessage(content)	
				end
			end

			if bEnterLayer then
				local isAutoCreate = true;
				local layer = showUILayerByName(name,isAutoCreate)
				if name == "TreasureExploreLayer" then
					DailyGoalsLayerCloseBtnClick(nil)
				end
				if name == "NormalFBTranspoint" or name == "EliteFBTranspoint" or name == "EnterUnderCity" then
					PushOneLayer(nil,"","")	
				end
			end
		end
	end
end

-------------------事件回调------------------------------------------------
function DailyGoalsLayerCloseBtnClick(pObj)	
	if nil ~= DailyGoalsLayer.LayerInstance then
		if nil ~= pObj then
			SetChatBar(true,-1);
		end		
		RemoveOneLayer(DailyGoalsLayer.LayerInstance)		
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyGoalsLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyGoalsLayer:ResetValue()
	end
end

function DailyGoalsLayerAcceptRewardBtnClick(pObj)
	DailyGoalsNetWorkHandler:SendGetDailyGoalRewardReqMessage()
end
---------------------------------------------------------------------------
local function closeCallBack(message)
	if nil ~= DailyGoalsLayer.LayerInstance then
		if G_CurLayerInstance ~= DailyGoalsLayer.LayerInstance then
			DailyGoalsLayerCloseBtnClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--SetChatBar(true,-1)
		DailyGoalsLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	elseif message == GM_ATTR_COIN_UPDATE then
		DailyGoalsLayer:FreshUserCoinAndDiamondInfo()
	elseif message == GM_ATTR_SP_DIAMOND_UPDATE then
		DailyGoalsLayer:FreshUserCoinAndDiamondInfo()
	elseif message == GM_E_ENTER_MAINLAND then
		DailyGoalsBtnShowTipOrNot()
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);