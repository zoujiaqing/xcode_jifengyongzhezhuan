require("Script/ChatToolLayer")
require("Script/CommonDefine")
--require("Script/DailyTask/dailyTaskDataCenter")

DailyTaskLayer = {
	LayerInstance = nil,
	Layout = nil,
	TaskNameLabel = nil,
	TaskConditionLabel = nil,
	TaskCompleteNumsLabel = nil,
	TaskRewardCoinLabel = nil,
	TaskRewardExpolitLabel = nil,
	TaskRewardMedalLabel = nil,
	TaskRewardMedalIcon = nil,
	UserCoinLabel = nil,
	UserExpolitLabel = nil,
	AcceptTaskBtn = nil,
	CompelteTaskRightNowBtn = nil,
	GiveUpTaskBtn = nil,
	FreshTaskBtn = nil,
	FullStarTaskBtn = nil,
	LayerTitleLabel = nil,
	AcceptTaskBtnLabel = nil,
	CompelteTaskRightNowBtnLabel = nil,
	GiveUpTaskBtnLabel = nil,
	FreshTaskBtnLabel = nil,
	FullStarTaskBtnLabel = nil,
	ListButtons = {},
	ListItemButtons = {},
	SelectItemIndex = -1,
	SelectedListItem = nil,
}

function DailyTaskLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.ListButtons = {}
	self.ListItemButtons = {}
	self.SelectItemIndex = -1
	self.SelectedListItem = nil
	self.TaskNameLabel = nil
	self.TaskConditionLabel = nil
	self.TaskCompleteNumsLabel = nil
	self.TaskRewardCoinLabel = nil
	self.TaskRewardExpolitLabel = nil
	self.TaskRewardMedalLabel = nil
	self.TaskRewardMedalIcon = nil
	self.UserCoinLabel = nil
	self.UserExpolitLabel = nil
	self.AcceptTaskBtn = nil
	self.CompelteTaskRightNowBtn = nil
	self.GiveUpTaskBtn = nil
	self.FreshTaskBtn = nil
	self.FullStarTaskBtn = nil
	self.LayerTitleLabel = nil
	self.AcceptTaskBtnLabel = nil
	self.CompelteTaskRightNowBtnLabel = nil
	self.GiveUpTaskBtnLabel = nil
	self.FreshTaskBtnLabel = nil
	self.FullStarTaskBtnLabel = nil
end

local function onReceiveDestructorHandler()	
	TXGUI.UIManager:sharedManager():removeUILayout("dailyTaskPanel")
	DailyTaskLayer:ResetValue()
end

--Note: 创建Layer
function DailyTaskLayer:CreateLayer(parentLayer)
	self:ResetValue()
	dailyTaskDataCenter:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/dailyTask.plist",self.LayerInstance, "dailyTaskPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
        
        local tag = UITagTable["DailyTaskLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
        
		InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	self:ShowCompleteTaskNums()
	self:FreshUserCoinAndExpolitInfo();
	PushOneLayer(self.LayerInstance,"","")		

	SetChatBar(true,-1)
	OnlineNetworkManager:sShareInstance():sendDailyTaskReqMessage();
end

function DailyTaskLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(DailyTaskLayerCloseBtnClick);

	self.TaskNameLabel = self.Layout:FindChildObjectByName("taskNameLabel").__UILabel__
	self.TaskConditionLabel = self.Layout:FindChildObjectByName("taskConditionLabel").__UILabel__
	self.TaskCompleteNumsLabel = self.Layout:FindChildObjectByName("TaskCompelteTimesLabel").__UILabel__

	self.TaskRewardCoinLabel = self.Layout:FindChildObjectByName("taskIconNumLabel").__UILabel__
	self.TaskRewardExpolitLabel = self.Layout:FindChildObjectByName("taskExpolitNumLabel").__UILabel__
	self.TaskRewardMedalLabel = self.Layout:FindChildObjectByName("taskMedalNumLabel").__UILabel__
	self.TaskRewardMedalIcon = self.Layout:FindChildObjectByName("taskMedalPic").__UIPicture__

	self.AcceptTaskBtn = self.Layout:FindChildObjectByName("acceptAndRewardBtn").__UIButton__:getMenuItemSprite();
	self.CompelteTaskRightNowBtn = self.Layout:FindChildObjectByName("completeRightNowBtn").__UIButton__:getMenuItemSprite();
	self.GiveUpTaskBtn = self.Layout:FindChildObjectByName("giveUpBtn").__UIButton__:getMenuItemSprite();
	self.FreshTaskBtn = self.Layout:FindChildObjectByName("freshBtn").__UIButton__:getMenuItemSprite();
	self.FullStarTaskBtn = self.Layout:FindChildObjectByName("fullStarBtn").__UIButton__:getMenuItemSprite();

	self.AcceptTaskBtn:registerScriptTapHandler(DailyTaskLayerAcceptTaskBtnClick);
	self.CompelteTaskRightNowBtn:registerScriptTapHandler(DailyTaskLayerCompelteTaskRightNowBtnClick);
	self.GiveUpTaskBtn:registerScriptTapHandler(DailyTaskLayerGiveUpTaskBtnClick);
	self.FreshTaskBtn:registerScriptTapHandler(DailyTaskLayerFreshTaskBtnClick);
	self.FullStarTaskBtn:registerScriptTapHandler(DailyTaskLayerFullStarTaskBtnClick);

	self.LayerTitleLabel = self.Layout:FindChildObjectByName("PanelTitle").__UILabel__
	self.AcceptTaskBtnLabel = self.Layout:FindChildObjectByName("taskAccpetAndRewardBtnLabel").__UILabel__
	self.CompelteTaskRightNowBtnLabel = self.Layout:FindChildObjectByName("compelteRightNowBtnLabel").__UILabel__
	self.GiveUpTaskBtnLabel = self.Layout:FindChildObjectByName("giveUpBtnLabel").__UILabel__
	self.FreshTaskBtnLabel = self.Layout:FindChildObjectByName("freshBtnLabel").__UILabel__
	self.FullStarTaskBtnLabel = self.Layout:FindChildObjectByName("fullStarBtnLabel").__UILabel__

	self.UserCoinLabel = self.Layout:FindChildObjectByName("coinLabel").__UILabel__
	self.UserExpolitLabel = self.Layout:FindChildObjectByName("expolitLabel").__UILabel__

	self:InitBtnLabelContent()
end

function DailyTaskLayer:InitBtnLabelContent()
	local languageFlag = 'Dailytask_Title'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	self.LayerTitleLabel:setString(content)

	languageFlag = 'Dailytask_AcceptTask'
	content = LanguageLocalization:GetLocalization(languageFlag)
	self.AcceptTaskBtnLabel:setString(content)

	languageFlag = 'Dailytask_UpdateTask'
	content = LanguageLocalization:GetLocalization(languageFlag)
	self.FreshTaskBtnLabel:setString(content)

	languageFlag = 'Dailytask_Fullstarupdate'
	content = LanguageLocalization:GetLocalization(languageFlag)
	self.FullStarTaskBtnLabel:setString(content)

	languageFlag = 'Dailytask_Completenow'
	content = LanguageLocalization:GetLocalization(languageFlag)
	self.CompelteTaskRightNowBtnLabel:setString(content)

	languageFlag = 'Dailytask_Giveup'
	content = LanguageLocalization:GetLocalization(languageFlag)
	self.GiveUpTaskBtnLabel:setString(content)
end

function DailyTaskLayer:FreshUserCoinAndExpolitInfo()
	if nil ~= self.Layout then
		if nil ~= self.UserCoinLabel and nil ~= self.UserExpolitLabel then
			local money = UserData:GetUserInfo().m_gold
			local moneyContent = MainMenuLayer:GetNumByFormat(money)
			self.UserCoinLabel:setString("" .. moneyContent)
			local expolit = UserData:GetUserInfo().m_player_exploit
			local expolitContent = MainMenuLayer:GetNumByFormat(expolit)
			self.UserExpolitLabel:setString("" .. expolitContent)
		end
	end
end

function DailyTaskLayer:FreshRightPanel()
end

function DailyTaskLayer:IsListEmpty()
	local count = table.getn(self.ListButtons)
	if count == 0 then
		return true
	else 
		return false
	end
end

function DailyTaskLayer:ShowLeftScrollList()
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
    
	for index,value in pairsByKeys(dailyTaskDataCenter.TaskItems) do
		local taskId = value.taskID
		self:InsertOneSkillItem(scrollList,taskId)
	end	

	local runningTaskId = dailyTaskDataCenter.RunningTaskID
	local index = dailyTaskDataCenter:GetTaskIndexByID(runningTaskId)
	if nil == index then
		index = 1
	end
	self.SelectItemIndex = index
	onDailyTaskLayerListItemClick(self.SelectItemIndex - 1)
end

function DailyTaskLayer:GetOneTaskConditionInfo(taskId)
	local taskCondition = dailyTaskDataCenter:GetOneDailyTaskDescription(taskId)
	local curStep,allSteps = dailyTaskDataCenter:GetOneTaskProgressInfo(taskId)
	taskCondition = taskCondition .. "(" .. curStep .. "/" .. allSteps .. ")"
	return taskCondition
end

function DailyTaskLayer:ShowOneTaskConditionInfo(taskId)
	if nil == self.Layout then
		return 
	end
	if nil ~= self.TaskConditionLabel then
		local condition = self:GetOneTaskConditionInfo(taskId)
		self.TaskConditionLabel:setString(condition)
	end
end

function DailyTaskLayer:ShowOneTaskDetailPanel()
	if self.Layout == nil then
		return
	end

	local selectIndex = self.SelectItemIndex
	if -1 == selectIndex then
		return
	end

	local taskItem = dailyTaskDataCenter:GetTaskItemByIndex(selectIndex)
	local taskName = dailyTaskDataCenter:GetOneDailyTaskName(taskItem.taskID)
	local taskCondition = self:GetOneTaskConditionInfo(taskItem.taskID)

	self.TaskNameLabel:setString(taskName)
	self.TaskConditionLabel:setString(taskCondition)

	local rewardTxtColor = ccc3(192,255,0)
	local starNum = dailyTaskDataCenter:GetOneDailyTaskStarNum(taskItem.taskID)
	local moneyNum , expolitNum , modelNum = dailyTaskDataCenter:GetRewardInfo(starNum)
	self.TaskRewardCoinLabel:setString("" .. moneyNum)
	self.TaskRewardExpolitLabel:setString("" .. expolitNum)
	self.TaskRewardCoinLabel:setColor(rewardTxtColor)
	self.TaskRewardExpolitLabel:setColor(rewardTxtColor)
	self.TaskRewardMedalLabel:setColor(rewardTxtColor)

	if nil == modelNum then
		self.TaskRewardMedalLabel:setString("")
		self.TaskRewardMedalIcon:setVisible(false)
	else
		self.TaskRewardMedalLabel:setString("" .. modelNum)
		self.TaskRewardMedalIcon:setVisible(true)
	end

	self:ShowDownBtns(taskItem.taskID)
end

function DailyTaskLayer:InsertOneSkillItem(scrollList,taskId)
	if nil == scrollList then
		return 
	end

	print("Insert One Item " .. taskId)
    
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	--Note: 定义背景框大小
	local backGroundFrameName = "map_ui2_FRAME_DAILYTASK_FRAME_TASK1";
	-- Note: List Item 
    
    
	local normalListItemIconBgName = "map_ui2_FRAME_DAILYTASK_FRAME_TASK1";
	local clickListItemIconBgName = "map_ui2_FRAME_DAILYTASK_FRAME_TASK1_SELECTED";
	local normal = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,clickListItemIconBgName)
		
	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);
        
        
	self:ShowOneListItemTaskState(backGroundFrameName,button,taskId);
	self:ShowOneListItemStarNum(backGroundFrameName,button,taskId);
	self:ShowOneListItemTaskName(backGroundFrameName,button,taskId);	
			
	local menuItem = button:getMenuItemSprite();
	menuItem:registerScriptTapHandler(onDailyTaskLayerListItemClick);

	local listItemCount = table.getn(self.ListItemButtons);
	self.ListItemButtons[listItemCount+1] = menuItem
	
	local listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = button
    
end

--Note: 显示每日任务剩余次数
function DailyTaskLayer:ShowCompleteTaskNums()
	if nil == self.Layout then
		return 
	end

	print(" DailyTaskLayer:ShowCompleteTaskNums() " .. dailyTaskDataCenter.TaskCompleteNums)
	local leftNums = dailyTaskDataCenter.TaskCompleteNums
	local flag = 'Dailytask_NumsLabel';
	local leftNumsContent = LanguageLocalization:GetLocalization(flag) .. "(" .. leftNums .. "/" .. dailyTaskDataCenter.TaskAcceptableTotalNums .. ")"
	self.TaskCompleteNumsLabel:setString(leftNumsContent)
end

--Note:每日任务是否全部做完
function DailyTaskLayer:IsAllTaskCompelted()
	local compelteNums = dailyTaskDataCenter.TaskCompleteNums
	local totalNums = dailyTaskDataCenter.TaskAcceptableTotalNums
	--print("*********************** daily task compelteNums " .. compelteNums)
	--print("*********************** daily task " .. totalNums)
	if compelteNums >= totalNums then
		return true
	end
	return false
end

--Note: 判断是否显示入口按钮
function DailyTaskLayer:ShowDailyTaskEnterBtnOrNot()
	local isCompelted = self:IsAllTaskCompelted()
	if isCompelted then
		--print("*********************** daily task isCompelted true")
		local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
		local dailyTaskButton = nil
		if nil ~= topLayout then
			dailyTaskButton = topLayout:FindChildObjectByName("button_dailytask").__UIButton__
			if nil ~= dailyTaskButton then
				dailyTaskButton:setVisible(false)
			end
		end
	end
end

--Note: 显示按钮
function DailyTaskLayer:ShowDownBtns(taskId)
	if self.Layout == nil then
		return
	end

	local taskState = dailyTaskDataCenter:GetOneTaskState(taskId)
	if nil == taskState then
		return 
	end

	self.AcceptTaskBtn:setVisible(false)
	self.CompelteTaskRightNowBtn:setVisible(false)
	self.GiveUpTaskBtn:setVisible(false)
	self.FreshTaskBtn:setVisible(false)
	self.FullStarTaskBtn:setVisible(false)
	self.AcceptTaskBtnLabel:setVisible(false)
	self.CompelteTaskRightNowBtnLabel:setVisible(false)
	self.GiveUpTaskBtnLabel:setVisible(false)
	self.FreshTaskBtnLabel:setVisible(false)
	self.FullStarTaskBtnLabel:setVisible(false)

	if dailyTaskDataCenter:IsUpToMaxAcceptTask() then
		return 
	end
	
	if 0 == taskState then
		local languageFlag = "Dailytask_AcceptTask"
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.AcceptTaskBtnLabel:setString(content)
		self.AcceptTaskBtn:setVisible(true)	
		self.AcceptTaskBtnLabel:setVisible(true)
		local bAccepted = dailyTaskDataCenter:GetIsOthTaskAccepted(taskId)
		local bShow = true
		if bAccepted then
			bShow = false
		end
		self.FreshTaskBtn:setVisible(bShow)
		self.FullStarTaskBtn:setVisible(bShow)		
		self.FreshTaskBtnLabel:setVisible(bShow)
		self.FullStarTaskBtnLabel:setVisible(bShow)
	elseif 1 == taskState then
		self.CompelteTaskRightNowBtn:setVisible(true)
		self.GiveUpTaskBtn:setVisible(true)
		self.CompelteTaskRightNowBtnLabel:setVisible(true)
		self.GiveUpTaskBtnLabel:setVisible(true)
	elseif 2 == taskState then
		local languageFlag = "Dailytask_WaitReward"
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.AcceptTaskBtnLabel:setString(content)
		self.AcceptTaskBtn:setVisible(true)
		self.AcceptTaskBtnLabel:setVisible(true)
	end
end

function DailyTaskLayer:ShowOneListItemTaskNameByTaskId(taskId)
	if nil == self.Layout then
		return
	end
	local index = dailyTaskDataCenter:GetTaskIndexByID(taskId)
	local button = self.ListButtons[index]
	local bgName = "map_ui2_FRAME_DAILYTASK_FRAME_TASK1"
	self:ShowOneListItemTaskName(bgName,button,taskId)
end

--Note: 显示任务名称
function DailyTaskLayer:ShowOneListItemTaskName(backGroundFrameName,button,taskId)
	local taskName = dailyTaskDataCenter:GetOneDailyTaskName(taskId)
	if nil == taskName then
		return 
	end

	local taskState = dailyTaskDataCenter:GetOneTaskState(taskId)
	if nil == taskState then
		return 
	end

	local additionTxt = ""
	if 1 == taskState then
		additionTxt = "(" .. LanguageLocalization:GetLocalization('Dailytask_Accepted') .. ")"
	elseif 2 == taskState then
		additionTxt = "(" .. LanguageLocalization:GetLocalization('Dailytask_WaitReward') .. ")"
	end

	taskName = taskName .. additionTxt

	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui2_FRAME_DAILYTASK_TEXT_TASK1"
	self:ShowOneLabel(backGroundFrameName,childFrameName,taskName,20,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),10,taskId);
end

function DailyTaskLayer:ShowOneListItemTaskStateByTaskId(taskId)
	if nil == self.Layout then
		return
	end
	local index = dailyTaskDataCenter:GetTaskIndexByID(taskId)
	local button = self.ListButtons[index]
	local bgName = "map_ui2_FRAME_DAILYTASK_FRAME_TASK1"
	self:ShowOneListItemTaskState(bgName,button,taskId)
end

--Note: 显示任务状态Icon
function DailyTaskLayer:ShowOneListItemTaskState(backGroundFrameName,button,taskId)
	local taskState = dailyTaskDataCenter:GetOneTaskState(taskId)
	if nil == taskState then
		return 
	end

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui2_FRAME_DAILYTASK_FRAME_ICON1";

	AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		
	local skillIconName = nil;
	if 0 == taskState then
		skillIconName = "map_ui2_FRAME_DAILYTASK_ICON_NOTACCEPT"
	elseif 1 == taskState then
		skillIconName = "map_ui2_FRAME_DAILYTASK_ICON_ACCEPT"
	elseif 2 == taskState then
		skillIconName = "map_ui2_FRAME_DAILYTASK_ICON_COMPLETE"
	end
	 
	if nil ~= skillIconName then
		local skillIcon = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,skillIconName)		
		skillIcon:setPosition(relativePoint);
		local stateIcon = button:getAnimationNode():getChildByTag(taskId + 100000)
		if nil ~= stateIcon then
			stateIcon:removeFromParentAndCleanup(true)
		end
		button:getAnimationNode():addChild(skillIcon,1,taskId + 100000);
	end
end

--Note: 显示星星数目
function DailyTaskLayer:ShowOneListItemStarNum(backGroundFrameName,button,taskId)
	local startNum = dailyTaskDataCenter:GetOneDailyTaskStarNum(taskId)
	if nil == startNum then
		return 
	end

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui2_FRAME_DAILYTASK_FRAME_STAR1";

	local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	pChildSprite:setPosition(relativePoint);
	button:getAnimationNode():addChild(pChildSprite,2);

	for index = 1,startNum do
		local starIconName = "map_ui2_FRAME_DAILYTASK_GRID_STAR1_" .. index
		AspriteManager:getInstance():getFramesRelativePos(KUI_BIN_2,backGroundFrameName,starIconName,anchorPoint,relativePoint);
		local starRcsIconName = "map_ui2_FRAME_DAILYTASK_ICON_STAR"
		local starIcon = AspriteManager:getInstance():getOneFrame(KUI_BIN_2,starRcsIconName)		
		starIcon:setPosition(relativePoint);
		button:getAnimationNode():addChild(starIcon,3);
	end

end

--Note: 显示文本函数
function DailyTaskLayer:ShowOneLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
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

function onDailyTaskLayerListItemClick(nTag)
	print("onDailyTaskLayerListItemClick " .. nTag)

	local listItemIndex = nTag+1
	DailyTaskLayer.SelectItemIndex = listItemIndex
		
	local item = DailyTaskLayer.ListItemButtons[listItemIndex]
	if nil ~= DailyTaskLayer.SelectedListItem then
		DailyTaskLayer.SelectedListItem:unselected()
	end			
	item:selected()
	DailyTaskLayer.SelectedListItem = item

	DailyTaskLayer:ShowOneTaskDetailPanel()
end

-------------------事件回调------------------------------------------------
function DailyTaskLayerCloseBtnClick(pObj)	
	if nil ~= DailyTaskLayer.LayerInstance then
		GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);

		SetChatBar(true,-1);
		RemoveOneLayer(DailyTaskLayer.LayerInstance)		
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyTaskLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyTaskLayer:ResetValue()		
	end
end

--Note: 接受任务
function DailyTaskLayerAcceptTaskBtnClick(pObj)	
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);
	
	local taskItem = dailyTaskDataCenter:GetTaskItemByIndex(DailyTaskLayer.SelectItemIndex)
	if nil == taskItem then
		return 
	end
	local taskState = dailyTaskDataCenter:GetOneTaskState(taskItem.taskID)
	if 0 == taskState then
		print("DailyTaskLayerAcceptTaskBtnClick Accept")
		--Note: 判断是否有已接受任务
		local bAccepted = dailyTaskDataCenter:GetIsOthTaskAccepted(taskItem.taskID)
		if bAccepted then
			local languageFlag = 'Dailytask_TaskWarningTip_1'
			local content = LanguageLocalization:GetLocalization(languageFlag)
			GameApi:showMessage(content);
		else
			OnlineNetworkManager:sShareInstance():sendDailyTaskAcceptReqMessage(taskItem.taskID)
			--ShowWaitingLayer()
			PushOneWaitingLayer("CSAcceptDailyTaskReq")
		end		
	elseif 2 == taskState then
		print("DailyTaskLayerAcceptTaskBtnClick Reward")
		OnlineNetworkManager:sShareInstance():sendGetDailyTaskRewardReqMessage()
		--ShowWaitingLayer()
		PushOneWaitingLayer("CSGetDailyTaskRewardReq")
	end
end

--Note: 砖石立即完成任务
local function DiamondLayerRightNowOkBtnClick(pObj)	
	if DailyTaskLayer.LayerInstance == nil then
		return 
	end
	OnlineNetworkManager:sShareInstance():sendExchangeParameterReq(20201,0,0)
	--ShowWaitingLayer()
	PushOneWaitingLayer("CSExchangeParameterReq")
end

local function DiamondLayerRightNowCancelBtnClick(pObj)
end

--Note: 立即完成任务
function DailyTaskLayerCompelteTaskRightNowBtnClick(pObj)
	if DailyTaskLayer.LayerInstance == nil then
		return 
	end

	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);

	print("DailyTaskLayerCompelteTaskRightNowBtnClick")
	local diamondNum = dailyTaskDataCenter:GetDailyTaskNeedDiamondByExchangeID(20201)
	local languageFlag = 'Dailytask_Usediamond3'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	DiamondWarningLayer:Show(content,diamondNum,DiamondLayerRightNowOkBtnClick,DiamondLayerRightNowCancelBtnClick,ccc3(255,255,255),"","")
end

--Note: 放弃任务
function DailyTaskLayerGiveUpTaskBtnClick(pObj)
	print("DailyTaskLayerGiveUpTaskBtnClick")
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);

	OnlineNetworkManager:sShareInstance():sendGiveUpTaskReqMessage()
	--ShowWaitingLayer()
	PushOneWaitingLayer("CSGiveUpDailyTaskReq")
end

--Note: 刷新任务
local function DiamondLayerFreshOkBtnClick(pObj)
	if DailyTaskLayer.LayerInstance == nil then
		return 
	end
	OnlineNetworkManager:sShareInstance():sendExchangeParameterReq(20100,0,0)
	--ShowWaitingLayer()
	PushOneWaitingLayer("CSExchangeParameterReq")
end

local function DiamondLayerFreshCancelBtnClick(pObj)
end

function DailyTaskLayerFreshTaskBtnClick(pObj)
	print("DailyTaskLayerFreshTaskBtnClick")
	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);
	
	local diamondNum = dailyTaskDataCenter:GetDailyTaskNeedDiamondByExchangeID(20100)
	local languageFlag = 'Dailytask_Usediamond1'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	DiamondWarningLayer:Show(content,diamondNum,DiamondLayerFreshOkBtnClick,DiamondLayerFreshCancelBtnClick,ccc3(255,255,255),"","")
end

function DiamondLayerFullStarOkBtnClick(pObj)
	if DailyTaskLayer.LayerInstance == nil then
		return 
	end
	OnlineNetworkManager:sShareInstance():sendExchangeParameterReq(20200,0,0)
	--ShowWaitingLayer()
	PushOneWaitingLayer("CSExchangeParameterReq")
end

function DiamondLayerFullStarCancelBtnClick(pObj)
end

function DailyTaskLayerFullStarTaskBtnClick(pObj)
	if DailyTaskLayer.LayerInstance == nil then
		return 
	end

	print("DailyTaskLayerFullStarTaskBtnClick")

	GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);

	local diamondNum = dailyTaskDataCenter:GetDailyTaskNeedDiamondByExchangeID(20200)
	local languageFlag = 'Dailytask_Usediamond2'
	local content = LanguageLocalization:GetLocalization(languageFlag)
	languageFlag = 'Dailytask_Fullstar_hint'
	local tipContent = LanguageLocalization:GetLocalization(languageFlag)
	DiamondWarningLayer:Show(content,diamondNum,DiamondLayerFullStarOkBtnClick,DiamondLayerFullStarCancelBtnClick,ccc3(255,255,255),tipContent,"")
end

---------------------------------------------------------------------------
local function closeCallBack(message)
	if nil ~= DailyTaskLayer.LayerInstance then
		if G_CurLayerInstance ~= DailyTaskLayer.LayerInstance then
			DailyTaskLayerCloseBtnClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		DailyTaskLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	elseif message == GM_ATTR_COIN_UPDATE then
		DailyTaskLayer:FreshUserCoinAndExpolitInfo()
	elseif message == GM_ATTR_PLAYER_EXPOLIT then
		DailyTaskLayer:FreshUserCoinAndExpolitInfo()
	elseif message == GM_E_ENTER_MAINLAND then
		--print("*********************** daily task GM_E_ENTER_MAINLAND")
		DailyTaskLayer:ShowDailyTaskEnterBtnOrNot()
	end	
end

-----------------------------------------------------------------
--local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);