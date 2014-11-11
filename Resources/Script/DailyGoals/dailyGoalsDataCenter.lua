require("Script/DailyGoals/Daily_Goals")
require("Script/DailyGoals/AwardDaily")
require("Script/DailyGoals/dailyGoalsLayer")
require("Script/OsCommonMethod")
require("Script/Language")
require("Script/CommonDefine")

DailyGoalsActiveRewardItem = {
	itemId = 0,
	activeNum = 0,
	rewardId = 0,
	rewardNum = 0,
	rewardIconName = nil,
}

function DailyGoalsActiveRewardItem:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

DailyGoalsRewardTip = {
	key = "",
	content = "",
}

function DailyGoalsRewardTip:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

dailyGoalsItem = {
	taskID = 0,
	taskAllSteps = 0,
	--Note: 从服务器获取的信息，当前完成步骤数
	taskStatus = 0,

	--Note: 奖励提示图标容器
	RewardTips = {},
}

function dailyGoalsItem:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

dailyGoalsDataCenter = {
	TaskItems = {} ,
	ActiveRewardItems = {},
	MinTaskId = 0,
	MaxTaskId = 0,
	IsCalTaskIdRange = false,
	IsInitData = false,
	SortTips = {"Inlay","Card_Brand","Reputation","Diamond","Medal","Money","Exp","Exploit","Fighting",},
	ActiveNum = 0,
	GetRewardTimes = 0,
}

function dailyGoalsDataCenter:ClearData()
	--print("++++++++++++++++++++++++ dailyGoalsDataCenter:ClearData")
	self.IsInitData = false
	self.TaskItems = {}
	self.ActiveRewardItems = {}
	self.MinTaskId = 0
	self.MaxTaskId = 0
	self.IsCalTaskIdRange = false
	self.IsInitData = false
	self.ActiveNum = 0
	self.GetRewardTimes = 0
end

function dailyGoalsDataCenter:InitAllData()
	if self.IsInitData then
		return
	end
	--print("++++++++++ dailyGoalsDataCenter:InitAllData")
	for index,value in pairs(Daily_Goals) do
		local newItem = dailyGoalsItem:New()
		newItem.taskID = index
		newItem.taskAllSteps = self:GetOneGoalTaskNeedNums(index)
		newItem.taskStatus = 0
		newItem.RewardTips = {}
		self.TaskItems[index] = newItem
		self:GetOneGoalTaskTipsInfo(newItem,index)
	end

	--Note: 更新奖励信息
	for index,value in pairs(AwardDaily) do
		local type = tonumber(value.Type1)
		if 1 == type then
			local newItem = DailyGoalsActiveRewardItem:New()
			local id = tonumber(value.Type2)
			local activeNum = tonumber(value.Active)
			local rewardId = 0;
			local rewardNum = 0;

			for indx = 1,20,1 do
				local flag = "RaidDrops" .. indx
				local str = value[flag]
				if str ~= nil and str ~= "0" and str ~= "" then
					local beganPos = string.find(str,"/") + 1
					local endPos = string.len(str)
					rewardId = tonumber(string.sub(str,1,beganPos-2));
					rewardNum = tonumber(string.sub(str,beganPos,endPos));
					break
				end
			end

			local iconName = value.Task_LookFace
			if iconName ~= nil and iconName ~= "0" and iconName ~= "" then
				newItem.rewardIconName = iconName
			end			
				
			newItem.itemId = id
			newItem.activeNum = activeNum
			newItem.rewardId = rewardId
			newItem.rewardNum = rewardNum

			self.ActiveRewardItems[id] = newItem
		end
	end

	self.IsInitData = true
end

function dailyGoalsDataCenter:SendTaskStatuMessage()
	for index,value in pairs(Daily_Goals) do
		local taskIdString = value.Task
		if "" ~= taskIdString and nil ~= taskIdString then
			local taskId = tonumber(taskIdString)
			if nil ~= taskId then
				--print("----------------------------- send get task statu " .. taskId)
				OnlineNetworkManager:sShareInstance():sendGetTaskStatusMessage(taskId);
			end			
		end
	end
end

function dailyGoalsDataCenter:GetTaskIdRange()
	if false == self.IsCalTaskIdRange then
		for index,value in pairs(Daily_Goals) do			
			if 0 == self.MinTaskId then
				self.MinTaskId = index
			elseif index < self.MinTaskId then
				self.MinTaskId = index
			end

			if 0 == self.MaxTaskId then
				self.MaxTaskId = index
			elseif index > self.MaxTaskId then
				self.MaxTaskId = index
			end
		end

		--print("----------------------------------- max task id " .. self.MaxTaskId)
		--print("----------------------------------- min task id " .. self.MinTaskId)

		self.IsCalTaskIdRange = true
	end
end

function dailyGoalsDataCenter:TrackOneTaskStateChangeEvent(id,value)
	print("dailyGoalsDataCenter:TrackOneTaskStateChangeEvent id ; value " .. id .. ";" .. value)
	local taskItem = self.TaskItems[id]
	if nil ~= taskItem then
		taskItem.taskStatus = value
	end
end

--Note: 管理函数

function dailyGoalsDataCenter:ResetValue()
	--print("+++++++++++ dailyGoalsDataCenter:ResetValue")
	self.TaskItems = {}
end

--Note: 基本信息查询函数
--Note: 名字
function dailyGoalsDataCenter:GetOneGoalTaskName(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end
	local languageFlag = taskItem.Task_Text
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end

function dailyGoalsDataCenter:GetIsCanEnterOnCompelte(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return false
	end
	local value = tonumber(taskItem.Can_enter_aftercomplete)
	if 1 == value then
		return true
	else
		return false
	end
end

--Note: 获取任务点击打开时需要跳转到的页面
function dailyGoalsDataCenter:GetOneGoalTaskLayerName(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end
	
	return taskItem.OpenLayerName
end


--Note: 解锁当条任务的解锁等级
function dailyGoalsDataCenter:GetOneGoalTaskUnLockHeroLevel(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Level)
end

--Note: 解锁任务需要达到的任务ID
function dailyGoalsDataCenter:GetOneGoalTaskUnLockTaskId(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Task)
end

--Note: 解锁当条任务需要完成的次数
function dailyGoalsDataCenter:GetOneGoalTaskNeedNums(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Number)
end

--Note: 解锁当条任务完成获取的活跃点
function dailyGoalsDataCenter:GetOneGoalTaskActive(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Active)
end

function dailyGoalsDataCenter:GetOneTaskIcon(taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return nil
	end

	local binName = KDailyObjective_BIN
	local iconName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_LOCK1"
	if false == self:IsOneTaskLocked(taskId) then
		iconName = "map_ui_system_icon_FRAME_" .. string.upper(taskItem.Task_LookFace)
		binName = KICON_BIN
	end
	
	return iconName,binName
end

function dailyGoalsDataCenter:IsOneTaskLocked(taskId)

	--print("--------------========================================================== IsOneTaskLocked taskId " .. taskId)
	local taskItem = Daily_Goals[taskId]
	if nil == taskItem then
		return true
	end

	local curStep,allStep = self:GetOneTaskProcessInfo(taskId)
	--print("-------------- IsOneTaskLocked curStep " .. curStep)
	--print("-------------- IsOneTaskLocked allStep " .. allStep)
	if curStep >= allStep then
		return false
	end

	local unlockLevel = tonumber(taskItem.Level)
	local heroLevel = UserData:GetUserLevel()
	--print("-------------- IsOneTaskLocked heroLevel " .. heroLevel)
	--print("-------------- IsOneTaskLocked unlockLevel " .. unlockLevel)
	if heroLevel < unlockLevel then
		return true
	end

	local unLockTaskId = self:GetOneGoalTaskUnLockTaskId(taskId)
	if nil ~= unLockTaskId then
		local nTaskState = SystemConditionManager:Get():GetTaskStatus(unLockTaskId)
		--Note: 任务没有接
		if nTaskState == 0 then
			--print("--------------------- one task not touch " .. unLockTaskId)
			return true
		end
	end

	return false
end

function dailyGoalsDataCenter:GetOneTaskProcessInfo(taskId)
	local taskItem = self.TaskItems[taskId]
	if nil == taskItem then
		return nil
	end

	return taskItem.taskStatus,taskItem.taskAllSteps
end

function dailyGoalsDataCenter:IsOneTaskCompelte(taskId)
	local curStep,allStep = self:GetOneTaskProcessInfo(taskId)
	if curStep < allStep then
		return false
	else
		if self:IsOneTaskLocked(taskId) then
			return false
		end
	end 
	return true
end

--Note: 解锁任务提示的图标信息
function dailyGoalsDataCenter:GetOneGoalTaskTipsInfo(taskItem,taskId)
	if nil == taskItem then
		return nil
	end

	local goalTaskDataItem = Daily_Goals[taskId]
	
	for index,value in pairsByKeys(self.SortTips) do
		local key = value
		local keyContent = goalTaskDataItem[key]

		if keyContent == "1" then
			local numTipContent = goalTaskDataItem[key .. "_Text"]
			local count = table.getn(taskItem.RewardTips)
			local newTipItem = DailyGoalsRewardTip:New();
			newTipItem.key = key
			newTipItem.content = numTipContent
			--print("-------- id,key,content > " .. taskId .. ";" .. key .. ";" .. numTipContent)			
			taskItem.RewardTips[count+1] = newTipItem
		end
	end
end

function dailyGoalsDataCenter:GetOneGoalTaskTipsInfoOnly(taskId)
	local taskItem = self.TaskItems[taskId]
	if nil == taskItem then
		return nil
	end

	return taskItem.RewardTips
end

--Note: 获取某Tip对应的图标Icon名字
--SortTips = {"Inlay","Card_Brand","Reputation","Diamond","Medal","Money","Exp","Exploit","Fighting",},
function dailyGoalsDataCenter:GetIconRcsNameByName(name)
	local rcsName = nil
	if "Inlay" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_ARMORY"
	elseif "Card_Brand" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_CARD"
	elseif "Reputation" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_REPUTATION"
	elseif "Diamond" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_DIAMOND"
	elseif "Medal" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_MEDAL"
	elseif "Money" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_COIN"
	elseif "Exp" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_EXP"
	elseif "Exploit" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_EXPLOIT"
	elseif "Fighting" == name then
		rcsName = "map_ui_dailyobjective_FRAME_DAILYOBJECTIVE_ICON_ATTRIBUTE"
	end

	return rcsName
end

--Note: 获取活跃度的进度和达到了第几个满足条件
function dailyGoalsDataCenter:GetActiveProgessAndValidRewardItemsNum()
	local validItemNums = 0
	local maxActive = 0
	for index,value in pairs(self.ActiveRewardItems) do
		if value.activeNum > maxActive then
			maxActive = value.activeNum 
		end

		if value.activeNum <= self.ActiveNum then
			validItemNums = validItemNums + 1
		end
	end

	return (self.ActiveNum / maxActive) , validItemNums
end

--Note: 判断当前是否可以领取奖励
function dailyGoalsDataCenter:IsCanGetReward()
	local getRewardTimes = self.GetRewardTimes;
	local progress,validNums = self:GetActiveProgessAndValidRewardItemsNum()

	if validNums > getRewardTimes then
		--Note: 可以领取奖励
		return true
	end

	return false
end

--Note: 判断当前任务ID
function dailyGoalsDataCenter:GetTaskIDByIndex(_index)
	local count = 1
	for index,value in pairsByKeys(self.TaskItems) do
		if _index == count then
			return value.taskID
		end 
		count = count + 1
	end
	return nil
end

-------------------------------------------------暴露给c++使用代码--------------------------
function IsOneKeyContainsDailyGoalsId(id,value)
	dailyGoalsDataCenter:GetTaskIdRange()
	if dailyGoalsDataCenter.MaxTaskId >= id and dailyGoalsDataCenter.MinTaskId <= id then
		--print("++++++++++++++　IsOneKeyContainsDailyGoalsId id " .. id ..  "  value  " .. value)
		dailyGoalsDataCenter:InitAllData()
		dailyGoalsDataCenter:TrackOneTaskStateChangeEvent(id,value)
		return true
	end
	return false
end

function DailyGoalsActiveNumChangeEvent(num)
	dailyGoalsDataCenter:InitAllData()
	dailyGoalsDataCenter.ActiveNum = num

	DailyGoalsBtnShowTipOrNot()

	if nil ~= DailyGoalsLayer then
		DailyGoalsLayer:FreshRightPanel()
	end	
	--print("---------------- DailyGoalsActiveNumChangeEvent " .. dailyGoalsDataCenter.ActiveNum)
	return true
end

function DailyGoalsGetRewardTimesChangeEvent(num)
	dailyGoalsDataCenter:InitAllData()
	dailyGoalsDataCenter.GetRewardTimes = num

	DailyGoalsBtnShowTipOrNot()
	
	if nil ~= DailyGoalsLayer then
		DailyGoalsLayer:FreshRightPanel()
	end	
	return true
end	

function DailyGoalsBtnShowTipOrNot()
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local dailyGoalButton = nil
	if nil ~= topLayout then
		dailyGoalButton = topLayout:FindChildObjectByName("button_dailyGoals").__UIButton__
	end

	if dailyGoalsDataCenter:IsCanGetReward() then
		if dailyGoalButton then
			dailyGoalButton:showTipIcon()
		end	
	else
		if dailyGoalButton then
			dailyGoalButton:hideTipIcon()
		end				
	end
end