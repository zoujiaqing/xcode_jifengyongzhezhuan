require("Script/DailyTask/dailytask")
require("Script/DailyTask/DailyTaskReward")
require("Script/Language")

dailyTaskItem = {
	taskID = 0,
	taskAllSteps = 0,
	--Note: 任务的状态 0 没有接受任务 1 任务进行中 2 等待领取奖励
	taskState = 0,
	--Note: 从服务器获取的信息
	taskStatus = -1,
}

function dailyTaskItem:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

dailyTaskDataCenter = {
	TaskItems = {} ,
	RunningTaskID = 0 ,
	TaskCompleteNums = 0,
	--Note: Hard Code 
	TaskAcceptableTotalNums = 10,
	
	TaskWaitRewardID = 0,
}

--Note: 管理函数

function dailyTaskDataCenter:ResetValue()
	self.TaskItems = {}
	self.RunningTaskID = 0 
	self.TaskWaitRewardID = 0
end

function UpdateDailyTaskEnterButtonState()
	local bShowTipIcon = false
	if nil == dailyTaskDataCenter then
		return false
	end

	for index,value in pairs(dailyTaskDataCenter.TaskItems) do
		local taskState = value.taskState
		if 2 == taskState then
			bShowTipIcon = true
			break
		end
	end

	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if nil ~= topLayout then
		local control = topLayout:FindChildObjectByName("button_dailytask")
		if control ~= nil then
			local dailyTaskButton = control.__UIButton__
			if nil ~= dailyTaskButton then
	--			print("daily task if nil ~= dailyTaskButton then")
				if bShowTipIcon then
					dailyTaskButton:showTipIcon()
				else
					dailyTaskButton:hideTipIcon()
				end
			end
		end
	end

	return true
end

--Note: 基本信息查询函数

function dailyTaskDataCenter:IsUpToMaxAcceptTask()
	if self.TaskCompleteNums >= self.TaskAcceptableTotalNums then
		return true
	end
	return false
end

function dailyTaskDataCenter:GetOneDailyTaskName(taskId)
	local taskItem = dailytask[taskId]
	if nil == taskItem then
		return nil
	end
	local languageFlag = taskItem.Dailytask_Name
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end

function dailyTaskDataCenter:GetOneDailyTaskDescription(taskId)
	local taskItem = dailytask[taskId]
	if nil == taskItem then
		return nil
	end
	local languageFlag = taskItem.Dailytask_Description
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end

function dailyTaskDataCenter:GetOneDailyTaskType(taskId)
	local taskItem = dailytask[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Dailytask_Types)
end

function dailyTaskDataCenter:GetOneDailyTaskFrquency(taskId)
	local taskItem = dailytask[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Dailytask_Frquency)
end

function dailyTaskDataCenter:GetOneDailyTaskStarNum(taskId)
	local taskItem = dailytask[taskId]
	if nil == taskItem then
		return nil
	end
	return tonumber(taskItem.Star_Number)
end

--Note: 基本奖励信息查询函数
function dailyTaskDataCenter:GetRewardInfo(startNum)
	local heroLevel = UserData:GetUserLevel();
	local dataItem = DailyTaskReward[heroLevel]
	if nil == dataItem then
		print("ERROR : " .. "dailyTaskDataCenter:GetRewardInfo" .. "NO Level Data " .. heroLevel);
		return 
	end

	if startNum < 1 or startNum > 5 then
		print("ERROR : " .. "dailyTaskDataCenter:GetRewardInfo" .. "startNum Not In Range " .. startNum);
		return 
	end

	local moneyFlag = "Task" .. startNum .. "_Money";
	local expolitFlag = "Task" .. startNum .. "_Exploit";
	local madelFlag = nil;
	if 4 == startNum or 5 == startNum then
		madelFlag = "Task" .. startNum .. "_Medal";
	end

	local moneyNum = tonumber(dataItem[moneyFlag])
	local expolitNum = tonumber(dataItem[expolitFlag])
	local modelNum = nil

	if nil ~= madelFlag then
		modelNum = tonumber(dataItem[madelFlag])
	end

	return moneyNum , expolitNum , modelNum
end

--Note: 操纵玩家所绑定任务数据

function dailyTaskDataCenter:ClearAllTaskItems()
	self.TaskItems = {}
	self.RunningTaskID = 0
	self.TaskCompleteNums = 0
	self.TaskAcceptableTotalNums = 10
	self.TaskWaitRewardID = 0
end

--Note: 获取玩家的进度
function dailyTaskDataCenter:GetOneTaskProgressInfo(taskId)
	local taskItem = self.TaskItems[taskId]
	if nil == taskItem then
		return nil
	end
	local totalNums = taskItem.taskAllSteps
	local curStep = taskItem.taskStatus
	if -1 == taskItem.taskStatus then
		curStep = 0
	end
	return curStep,totalNums
end

function dailyTaskDataCenter:GetIsOthTaskAccepted(taskId)
	for index,value in pairs(self.TaskItems) do
		if value.taskID ~= taskId then
			if value.taskState ~= 0 then
				return true
			end
		end
	end
	return false
end

function dailyTaskDataCenter:GetTaskItemByIndex(_index)
	local count = 1
	local result = nil
	for index,value in pairsByKeys(self.TaskItems) do
		print("count " .. count)
		if _index == count then
			result = value
			break
		else
			count = count + 1
		end
	end
	return result
end

function dailyTaskDataCenter:GetTaskIndexByID(taskId)
	local count = 1
	local result = nil
	for index,value in pairsByKeys(self.TaskItems) do
		if value.taskID == taskId then
			result = count
			break
		else
			count = count + 1
		end
	end
	return result
end

function dailyTaskDataCenter:GetDailyTaskNeedDiamondByExchangeID(exchangeId)
	local str = ExchangeParameter[exchangeId]["FromItems"]
	local beganPos = string.find(str,"/") + 1
	local endPos = string.len(str)
	local num = string.sub(str,beganPos,endPos)
	if num == nil then
		num = 0
	end
	return num
end

function dailyTaskDataCenter:ClearData()
	self.TaskItems = {}
end

function dailyTaskDataCenter:InsertOneTaskItem(taskId,taskStatus)
	local newTaskItem = dailyTaskItem:New()
	newTaskItem.taskID = taskId
	newTaskItem.taskAllSteps = self:GetOneDailyTaskFrquency(taskId)
	newTaskItem.taskStatus = taskStatus
	self:UpdateOneTaskState(newTaskItem)
	self.TaskItems[taskId] = newTaskItem	
	print("self.TaskItems[taskId] state " .. self.TaskItems[taskId].taskState)
end

function dailyTaskDataCenter:UpdateOneTaskState(taskItem)
	if nil ~= taskItem then
		local curTaskStatus = taskItem.taskStatus
		local allTaskStep = taskItem.taskAllSteps
		if curTaskStatus == -1 then
			taskItem.taskState = 0	--Note: 未接受任务
		elseif allTaskStep <= curTaskStatus then
			taskItem.taskState = 2	--Note: 等奖励
		else
			taskItem.taskState = 1	--Note: 已接受
		end
	end
end

function dailyTaskDataCenter:GetOneTaskState(taskId)
	local taskItem = self.TaskItems[taskId]
	if nil ~= taskItem then
		return taskItem.taskState
	else
		print("ERROR : " .. "dailyTaskDataCenter:GetOneTaskState " .. taskId)
		return nil
	end
end



function dailyTaskDataCenter:GetRunningTaskID()
	for index,value in pairs(self.TaskItems) do
		local taskState = value.taskState
		if 1 == taskState or 2 == taskState then
			self.RunningTaskID = index
			print("self.RunningTaskID " .. self.RunningTaskID)
			return 
		end
	end
end

function dailyTaskDataCenter:SendReqGetTaskList()
	--Note:界面打开以后开始拉去任务数据
	self:ResetValue()
	--Note: 发送请求刷新任务信息

end

--Note: 发送请求接受某任务
function dailyTaskDataCenter:SendReqAcceptOneDailyTask(taskId)
end

function dailyTaskDataCenter:HandleRsqAcceptOneDailyTask(taskId)
	local taskItem = self.TaskItems[taskId]
	if nil == taskItem then
		return 
	end
	taskItem.taskState = 1
	taskItem.taskStatus = 0
	--Note: 刷新UI界面
	if nil ~= DailyTaskLayer then
		DailyTaskLayer:ShowDownBtns(taskId)
		DailyTaskLayer:ShowOneListItemTaskStateByTaskId(taskId)
		DailyTaskLayer:ShowOneListItemTaskNameByTaskId(taskId)
		DailyTaskLayer:ShowOneTaskConditionInfo(taskId)
	end
end

function dailyTaskDataCenter:HandleRsqGiveUpOneDailyTask(taskId)
	local taskItem = self.TaskItems[taskId]
	if nil == taskItem then
		return 
	end
	taskItem.taskState = 0
	taskItem.taskStatus = -1

	--Note: 刷新UI界面
	if nil ~= DailyTaskLayer then
		DailyTaskLayer:ShowDownBtns(taskId)
		DailyTaskLayer:ShowOneListItemTaskStateByTaskId(taskId)
		DailyTaskLayer:ShowOneListItemTaskNameByTaskId(taskId)
		DailyTaskLayer:ShowOneTaskConditionInfo(taskId)
	end
end

function dailyTaskDataCenter:HandleOneDailyTaskGetReward(taskId)
	self:ResetValue()
	OnlineNetworkManager:sShareInstance():sendDailyTaskReqMessage();
end

--Note: 发送领取奖励某任务
function dailyTaskDataCenter:SendReqRewardOneDailyTask(taskId)
end

function dailyTaskDataCenter:HandleRsqRewardOneDailyTask(taskId)
	--Note: 清空UI任务列表
	self:SendReqGetTaskList()
end

--Note: 某项值发生变化时回调
function dailyTaskDataCenter:OnOneTaskInfoExchange(taskId,taskStatus)
	local taskItem = self.TaskItems[taskId]
	if nil == taskItem then
		return 
	end
	taskItem.taskStatus = taskStatus
	self:UpdateOneTaskState(taskItem)
	--Note: 刷新UI界面
	if nil ~= DailyTaskLayer then
		DailyTaskLayer:ShowDownBtns(taskId)
		DailyTaskLayer:ShowOneListItemTaskStateByTaskId(taskId)
		DailyTaskLayer:ShowOneListItemTaskNameByTaskId(taskId)
		DailyTaskLayer:ShowOneTaskConditionInfo(taskId)
	end
end

----------------------------- C++ 接口函数 ------------------------
function FreshDailyTaskList(taskId,taskStatus)
	dailyTaskDataCenter:InsertOneTaskItem(taskId,taskStatus)

	return true
end

function ClearDailyTaskList()
	dailyTaskDataCenter:ClearAllTaskItems()
end

local sortFunc = function(a,b)
    return a.taskID < b.taskID;
end

function FreshDailyTaskEnd()
	UpdateDailyTaskEnterButtonState()
	--table.sort(dailyTaskDataCenter.TaskItems,sortFunc);

	--for index,value in pairsByKeys(dailyTaskDataCenter.TaskItems) do
		--print("FreshDailyTaskEnd" .. value.taskID)
	--end
	
	--Note:得到当前选定的任务
	dailyTaskDataCenter:GetRunningTaskID()

	--Note:刷新UI界面
	if nil ~= DailyTaskLayer then
		DailyTaskLayer:ShowLeftScrollList()
	end	

	return true;
end

function UpdateDailyTaskComplteNums(nums)
	dailyTaskDataCenter.TaskCompleteNums = nums
	print("UpdateDailyTaskComplteNums " .. dailyTaskDataCenter.TaskCompleteNums)
	--Note:更新剩余次数
	if nil ~= DailyTaskLayer then
		DailyTaskLayer:ShowCompleteTaskNums()
	end	
	return true
end

function AcceptOneDailyTask(bSuc,taskId)
	if bSuc then
		dailyTaskDataCenter:HandleRsqAcceptOneDailyTask(taskId)
	end
	--HideWaitingLayer()
	return true
end

function GiveUpOneDailyTask(bSuc,taskId)
	if bSuc then
		dailyTaskDataCenter:HandleRsqGiveUpOneDailyTask(taskId)
	end
	--HideWaitingLayer()
	return true
end

function ExchangeDailyTaskByDiamond(exchangeId)
	print("ExchangeDailyTaskByDiamond " .. exchangeId)
	if 20201 == exchangeId then
		--Note:使用钻石直接完成每日任务		
	elseif exchangeId == 20100 then
		--Note:刷新每日任务
		dailyTaskDataCenter:ResetValue()
		OnlineNetworkManager:sShareInstance():sendDailyTaskReqMessage();
	elseif exchangeId == 20200 then
		--Note:刷新至5星每日任务
		dailyTaskDataCenter:ResetValue()
		OnlineNetworkManager:sShareInstance():sendDailyTaskReqMessage();
	end

	--HideWaitingLayer()
	return true
end

function DailyTaskUpdateOneTask(taskId,taskStatus)
	--print("function DailyTaskUpdateOneTask(taskId,taskStatus)")
	dailyTaskDataCenter:OnOneTaskInfoExchange(taskId,taskStatus)
	UpdateDailyTaskEnterButtonState()
	return true
end

function DailyTaskGetReward(bSuc,taskId)
	if bSuc then
		dailyTaskDataCenter:HandleOneDailyTaskGetReward(taskId)
	end	
	--HideWaitingLayer()
	return true
end