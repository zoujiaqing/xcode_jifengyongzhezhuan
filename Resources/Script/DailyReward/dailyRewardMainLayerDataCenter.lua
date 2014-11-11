require("Script/DailyReward/dailyreceive")
require("Script/OsCommonMethod")
require("Script/Language")
require("Script/CommonDefine")

dailyRewardOneItem = {
	id = 0,
	--Note: 用于标示入口按钮的状态（1 显示可领取和可接受，领取以后不能重复进入 2 单状态，可重复进入，显示可领取）
	type = 0,
	--Note: 状态 1 可领取 2 已领取 （默认为可领取状态）
	state = 0,
}

function dailyRewardOneItem:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

dailyRewardMainLayerDataCenter = {
	Items = {} ,
	IsInitData = false ,

	--Note: 用于计算配表的ID范围
	IsCalTaskIdRange = false ,
	MinItemId = 0,
	MaxItemId = 0,
}

function dailyRewardMainLayerDataCenter:ClearData()
	self.IsInitData = false
	self.Items = {}
end

function dailyRewardMainLayerDataCenter:InitAllData()
	if self.IsInitData then
		return
	end
	for index,value in pairs(dailyreceive) do
		local newItem = dailyRewardOneItem:New()
		newItem.id = index
		newItem.type = tonumber(value.Dailyreceive_types)
		newItem.state = 1
		self.Items[index] = newItem
	end

	self.IsInitData = true
end

function dailyRewardMainLayerDataCenter:GetDailyRewardIdRange()
	if false == self.IsCalTaskIdRange then
		for index,value in pairs(dailyreceive) do			
			if 0 == self.MinItemId then
				self.MinItemId = index
			elseif index < self.MinItemId then
				self.MinItemId = index
			end

			if 0 == self.MaxItemId then
				self.MaxItemId = index
			elseif index > self.MaxItemId then
				self.MaxItemId = index
			end
		end

		self.IsCalTaskIdRange = true
	end
end

function dailyRewardMainLayerDataCenter:TrackOneDailyRewardStateChangeEvent(id,value)
	local oneItem = self.Items[id]
	if nil ~= oneItem then
		if value == 0 then
			oneItem.state = 2 --Note: 已领取
		else
			oneItem.state = 1 --Note: 可领取
		end		
	end

	--Note: 刷新用户界面
	if nil ~= DailyRewardMainLayer then
		DailyRewardMainLayer:FlushOneListItem(id)
	end	
end

--Note: 管理函数

function dailyRewardMainLayerDataCenter:ResetValue()
	self.Items = {}
end

--Note: 基本信息查询函数
--Note: 名字
function dailyRewardMainLayerDataCenter:GetOneDailyRewardItemName(id)
	local oneItem = dailyreceive[id]
	if nil == oneItem then
		return nil
	end
	local languageFlag = oneItem.Dailyreceive_name
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end
function dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(id ,key)
	local oneItem = dailyreceive[id]
	if nil == oneItem then
		return nil
	end
	local _start1Str = nil
	if key == 1  then
		_start1Str = oneItem.start_time_1
	elseif  key ==2 then
		_start1Str = oneItem.end_time_1
	elseif key == 3 then
		_start1Str = oneItem.start_time_2
	elseif key == 4 then
		_start1Str = oneItem.end_time_2
	end

	local timeNumber = 0
	
	for a=1 ,3  do
		 local i ,j = string.find(_start1Str ,"/")
		 if i == nil then
			  local hour = _start1Str 
			   timeNumber =timeNumber*60 + tonumber(hour)
			   break
		 else
			local hour = string.sub(_start1Str ,1,i-1)
			 timeNumber =timeNumber*60 + tonumber(hour)
			 _start1Str=string.sub(_start1Str ,i+1)
		 end
	end
	--print(timeNumber)
	return timeNumber 
end
function dailyRewardMainLayerDataCenter:GetOneDailyRewardItemIcon(id)
	local oneItem = dailyreceive[id]
	if nil == oneItem then
		return nil
	end

	local binName = KICON_BIN
	local iconName = "map_ui_system_icon_FRAME_" .. string.upper(oneItem.Dailyreceive_icon)
	return iconName,binName
end

function dailyRewardMainLayerDataCenter:GetOneDailyRewardItemType(id)
	local oneItem = self.Items[id]
	if nil == oneItem then
		return nil
	end
	return oneItem.type
end


function dailyRewardMainLayerDataCenter:GetOneDailyRewardItemState(id)
	local oneItem = self.Items[id]
	if nil == oneItem then
		return nil
	end
	return oneItem.state
end

--Note: 判断当前任务ID
--function dailyGoalsDataCenter:GetTaskIDByIndex(_index)
	--local count = 1
	--for index,value in pairsByKeys(self.TaskItems) do
		--if _index == count then
			--return value.taskID
		--end 
		--count = count + 1
	--end
	--return nil
--end

-------------------------------------------------暴露给c++使用代码--------------------------

function IsOneKeyContainsDailyRewardId(id,value)
	dailyRewardMainLayerDataCenter:GetDailyRewardIdRange()
	if dailyRewardMainLayerDataCenter.MaxItemId >= id and dailyRewardMainLayerDataCenter.MinItemId <= id then
		print("------------------ IsOneKeyContainsDailyRewardId " .. id .. " value " .. value)
		dailyRewardMainLayerDataCenter:InitAllData()
		dailyRewardMainLayerDataCenter:TrackOneDailyRewardStateChangeEvent(id,value)
		--Note: 是否显示提示Icon
		if nil ~= DailyRewardMainLayer then
			DailyRewardMainLayer:ShowRewardTipIconOrNot()
		end
		return true
	end
	return false
end