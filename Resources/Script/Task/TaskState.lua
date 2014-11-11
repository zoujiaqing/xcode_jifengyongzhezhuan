require("Script/Task/task")
require("Script/GameConfig/InstanceList")
require("Script/GameConfig/Localization")
require("Script/GameConfig/npc")
require("Script/GameConfig/Item")
require("Script/GameConfig/MonsterBasicInfomation")

--[[
dofile("E:/cocos2d-x/Adventure/source/Resources/Script/Task/task.lua")
dofile("E:/cocos2d-x/Adventure/source/Resources/Script/GameConfig/InstanceList.lua")
dofile("E:/cocos2d-x/Adventure/source/Resources/Script/GameConfig/Localization.lua")
dofile("E:/cocos2d-x/Adventure/source/Resources/Script/GameConfig/npc.lua")
dofile("E:/cocos2d-x/Adventure/source/Resources/Script/GameConfig/MonsterBasicInfomation.lua")
--]]

TasksState={}

local function GetTaskErrorLog(errorCode)
	local TASK_STRING_HEADER = "----------------------> TASK ERROR: "
	local resultString = TASK_STRING_HEADER .. errorCode;
	CCLuaLog(resultString);
end

-- common function for table
function list_iter (t)
	local i = 0
	local n = table.getn(t)
	return function ()
		i = i + 1
		if i <= n then return t[i] end
	end
end

function TasksState:InsertOneTask(task_Id,currentStep,attachData)
	self[task_Id]={curStep=currentStep,data=attachData}
	--print( table.getn(self))
end

function TasksState:RemoveOneTask(taskId)
	self[taskId] = nil
end

function TasksState:IsOneTaskExist(taskId)
	return true
end

function TasksState:GetOneNPCTask(npcId)
	for element in list_iter(self) do
		print(element.curStep)
	end
end

-- use for get task state for temp
function IsTaskWaitForGetReward(taskId,stepId)
	--Note: 对话任务不处理
	if stepId == 2 then
		return false
	end
	local taskStepCount=table.getn(tasks[taskId].Task_step)
	if taskStepCount <= stepId  then
		return true
	end
	return false
end

function GetOneTaskAttachNpcIds(taskId)
	print("taskid:"..taskId)
	local npcId = tasks[taskId].NPC_receivetask
	if nil == npcId then
		GetTaskErrorLog("NPC ID ERROR , " .. taskId);
	end

	return npcId
end

function GetOneTaskName(taskId)
	return tasks[taskId].Task_name
end

function GetOneTaskRewardMoneyTable(taskId)
	return tasks[taskId].Money
end

function GetOneTaskRewardExpTable(taskId)
	return tasks[taskId].Experience
end

-- index start from 1
function GetRecieveTextNpc(taskId,index)
	local recieveTxt = tasks[taskId].Receive_text[index*2-1]
	if nil == recieveTxt then
		GetTaskErrorLog("GetRecieveTextNpc")
	end
	return recieveTxt
end

function GetRecieveTextHero(taskId,index)
	local recieveTxt = tasks[taskId].Receive_text[index*2]
	if nil == recieveTxt then
		GetTaskErrorLog("GetRecieveTextHero")
	end
	return recieveTxt
end

function TellIsRecieveTalkOver(taskId,index)
	n = table.getn(tasks[taskId].Receive_text)
	--print(n)
	if(index>(n/2)) then
		return true
	end
	return false
end

function GetCompleteTextNpc(taskId,index)
	local txt = tasks[taskId].Complete_text[index*2-1]
	if nil == txt then
		GetTaskErrorLog("GetCompleteTextNpc taskID " .. taskId)
	end
	return txt
end

function GetCompleteTextHero(taskId,index)
	local txt = tasks[taskId].Complete_text[index*2]
	if nil == txt then
		GetTaskErrorLog("GetCompleteTextHero taskID " .. taskId)
	end
	return txt
end

function TellIsCompleteTalkOver(taskId,index)
	n = table.getn(tasks[taskId].Complete_text)
	--print(n)
	if(index>(n/2)) then
		return true
	end
	return false
end

function GetOneTaskStepNums(taskId)
	return table.getn(tasks[taskId].Task_step)
end

function GetCommonNpcTalk()
	return 'T_common_text_1'
end

function GetCommonHeroTalk()
	return 'T_common_text_2'
end

function GetCommonCompleteContent()
	return 'T_common_text_3'
end

function GetTaskType(taskId)
	local taskItem = tasks[taskId]
	if nil == taskItem then
		return 0
	end
	return taskItem.Task_step[2][1]
end

function GetOneTaskTalkToNpcId(taskId)
	return tasks[taskId].NPC_talk
end

function GetOneTaskClearanceIconName(taskId,step)
	local lead_intance = 0
	local taskType = GetTaskType(taskId)
	if 2 == taskType then
		lead_intance = tasks[taskId].Task_step[step][2]
	elseif 5 == taskType then
		return "ICON_INSTANCE"
	else
		lead_intance = tasks[taskId].lead_instance
	end	

	if nil == lead_intance then
		GetTaskErrorLog("nil == lead_intance , task id " .. taskId)
	end

	local oneInstance = instances[lead_intance]
	if nil == oneInstance then
		GetTaskErrorLog("nil == oneInstance , lead_intance id " .. lead_intance)
	end

	return oneInstance.Icon
end

function GetTaskInstanceId(taskId,step)
	local TaskStepTable = tasks[taskId].Task_step
	taskType = TaskStepTable[2][1]
	if 1 == taskType then
		return 0
	elseif 2 == taskType then
		return TaskStepTable[step][2]
	elseif 3 == taskType then
		return tasks[taskId].lead_instance
	elseif 4 == taskType then
		return tasks[taskId].lead_instance
	elseif 5 == taskType then
		return TaskStepTable[step][2]
	end
end

function GetOneTaskDescription(taskId)
	 return tasks[taskId].Task_description
end

function GetTextContentFromLocalization(flagString)
	return Localization[flagString].SimpleChinese
end

function GetOneTaskProgress(taskId,value,bWaitForReward)
	local taskType = GetTaskType(taskId)
	if taskType == 1 then
		local npcName = GetTextContentFromLocalization(npcs[tasks[taskId].NPC_talk].Name)
		return npcName .. "  " .. "(" .. "0/1" .. ")"
	elseif taskType == 2 then
		local instanceId = tasks[taskId].lead_instance
		if nil == instanceId then
			GetTaskErrorLog("GetOneTaskProgress type 2 instance id " .. instanceId)
		end
		local oneInstance = instances[instanceId]
		if nil == oneInstance then
			GetTaskErrorLog("GetOneTaskProgress type 2 oneInstance nil " .. instanceId)
		end
		local instanceName = GetLuaLocalization(oneInstance.RaidName)
		if bWaitForReward then
			return instanceName .. "  " .. "(" .. "1/1" .. ")"
		else
			return instanceName .. "  " .. "(" .. "0/1" .. ")"
		end		
	elseif taskType == 3 then
		local monsterId = tasks[taskId].Task_step[2][2]
		if nil == monsterId then
			GetTaskErrorLog("GetOneTaskProgress type 3 monster id " .. monsterId)
		end
		local oneMonster = MonsterBasicInfomation[monsterId]
		if nil == oneMonster then
			GetTaskErrorLog("GetOneTaskProgress type 3 monster nil " .. monsterId)
		end
		local monsterName = GetTextContentFromLocalization(oneMonster.Name)
		if bWaitForReward then
			progress = "(" .. tasks[taskId].Task_step[2][3] .. "/" .. tasks[taskId].Task_step[2][3] .. ")"
		else
			progress = "(" .. value .. "/" .. tasks[taskId].Task_step[2][3] .. ")"
		end		
		return monsterName .. "  " .. progress
	elseif taskType == 4 then
		local itemId = tasks[taskId].Task_step[2][2]
		if nil == itemId then
			GetTaskErrorLog("GetOneTaskProgress type 4 Item id " .. itemId)
		end
		local oneItem = Items[itemId]
		if nil == oneItem then
			GetTaskErrorLog("GetOneTaskProgress type 4 Item nil " .. itemId)
		end
		local itemName = oneItem.Name
		itemName = GetTextContentFromLocalization(itemName)
		if bWaitForReward then
			progress = "(" .. tasks[taskId].Task_step[2][3] .. "/" .. tasks[taskId].Task_step[2][3] .. ")"
		else
			progress = "(" .. value .. "/" .. tasks[taskId].Task_step[2][3] .. ")"
		end
		return itemName .. "  " .. progress
	end
end

function GetOneTaskRewardElfID(taskId)
	local rewardTable = tasks[taskId].Common_reward
	for index,value in pairs(rewardTable) do
		id = value[1]
		if id >= 2001 and id <= 2999 then
			return id
		end
	end
end

function IsOneTaskElite(taskId)
	local item = tasks[taskId]
	if nil == item then
		return false
	end
	if taskId >= 700000 then
		return true
	else
		return false
	end
end

function GetPreBuildTaskID(taskId)
	local result = 0

	local item = tasks[taskId]
	if nil == item then
		return result
	end

	local preTask = item.Pre_task
	if nil == preTask then
		return result
	end

	result = tonumber(preTask)
	return result
end

function GetAllTutorialAttachedTaskId()
	for index,value in pairs(tasks) do
		local taskItem = value
		local taskId = index
		if nil ~= taskItem then
			local type = taskItem.Task_step[2][1]
			if type== 5 then
				local tutorialId = taskItem.Task_step[2][2]
				UIControlAppearControler:Get():InsertOneTutorialAndTask(tutorialId,taskId)
			end
		end
	end
	return true
end