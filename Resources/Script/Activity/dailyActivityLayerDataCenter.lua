require("Script/Activity/activity")
require("Script/OsCommonMethod")
require("Script/Language")
require("Script/CommonDefine")
require("Script/Instance/TimingActivityMgr")

activityOneItem = {
	id = 0,
	--Note: 状态 1 可进入 2 不可进入
	state = 1,
}

itemTypeList = {} --类型
itemTypeList[6210] = {type="ppve"}
itemTypeList[6240] = {type="world_boss_noon"}
itemTypeList[6250] = {type="pvptop"}
itemTypeList[6260] = {type="pvp"}
itemTypeList[6270] = {type="hero_instance"}
itemTypeList[6290] = {type="activity_undergroundcity"}
itemTypeList[6300] = {type="tower_defence"}

openItemList = {
	6210, --ppve
	6260, --pvp
	6250,
    6270,
	6290,
    6300,
}

--是否类型为限时
function isActivityTypeNotZero(id)
	local oneActivity = Activity[id]
	if nil == oneActivity then
		return false
	end
	if oneActivity.Activity_Type == '0' then
		return false
	end
	return true
end

--获取活动是否开启, 1为开启, 2为关闭
local function getActivityItemState(index)
	local ret = 2;

	for _,i in pairs(openItemList) do
		if index == i then
			if ActivityLayerDataCenter:IsUserLevelCanEnter(index) then
				return 1
			end			
		end 
	end
	--[[
	if index == 6203 then
		return 1
	end
	--]]
	if not isActivityTypeNotZero(index) then
		return 1
	end

	if	GetOpenStateByActivityId(index) then
		return 1
	end
 
	return ret
end

function activityOneItem:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

ActivityLayerDataCenter = {
	Items = {} ,
	IsInitData = false ,

	--Note: 用于计算配表的ID范围
	IsCalIdRange = false ,
	MinItemId = 0,
	MaxItemId = 0,
}
function ActivityLayerDataCenter:clearData()
	self.Items = {} 
	self.IsInitData = false 

	--Note: 用于计算配表的ID范围
	self.IsCalIdRange = false 
	self.MinItemId = 0
	self.MaxItemId = 0
end

function ActivityLayerDataCenter:InitAllData()
	if self.IsInitData then
		return
	end
	for index,value in pairs(Activity) do
		local newItem = activityOneItem:New()
		newItem.id = index
		newItem.state = getActivityItemState(index)
		self.Items[index] = newItem
	end

	self.IsInitData = true
end

--更新最新的活动开启状态
function ActivityLayerDataCenter:updateActivityStateData()
	self:InitAllData()
	for index,value in pairs(Activity) do
		local item = self.Items[index]
		if	item ~= nil then
			local newState = getActivityItemState(index)
			self.Items[index].state = newState
			CCLuaLog("------------------------update activity state : " .. self.Items[index].state .. " index : " .. index .. " newState: " .. newState)
		else
			CCLuaLog("------------------update activity state nil----------------")
		end
	end
end


function ActivityLayerDataCenter:GetDailyRewardIdRange()
	if false == self.IsCalIdRange then
		for index,value in pairs(Activity) do			
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

		self.IsCalIdRange = true
	end
end

function ActivityLayerDataCenter:TrackOneActivityStateChangeEvent(id,value)

end

function ActivityLayerDataCenter:IsUserLevelCanEnter(id)
	local userInfo = UserData:GetUserInfo();
	local oneItem = Activity[id]
	local levelNum = tonumber(oneItem.Activity_Level)
	if nil ~= oneItem and userInfo ~= nil then
		if userInfo.level >= levelNum then
			return true, levelNum
		end
	end

	return false, levelNum
end

function getCityDefendOpenLv()
	local oneItem = Activity[6300]
	local levelNum = tonumber(oneItem.Activity_Level)
    return levelNum
end

--Note: 管理函数

function ActivityLayerDataCenter:ResetValue()
	self.Items = {}
	self.IsInitData = false
end

--Note: 基本信息查询函数
--Note: 名字
function ActivityLayerDataCenter:GetOneActivityItemName(id)
	local oneItem = Activity[id]
	if nil == oneItem then
		return nil
	end
	local languageFlag = oneItem.Activity_Name
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end

--Note: 获取时间描述
function ActivityLayerDataCenter:GetOneActivityItemTimeDescription(id)
	local oneItem = Activity[id]
	if nil == oneItem then
		return nil
	end
	local languageFlag = oneItem.Activity_Time
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end

--Note: 描述
function ActivityLayerDataCenter:GetOneActivityItemDescription(id)
	local oneItem = Activity[id]
	if nil == oneItem then
		return nil
	end
	local languageFlag = oneItem.Activity_Description
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content
end

--Note: 是否开启
function ActivityLayerDataCenter:GetOneActivityItemStateContent(id)
	local txtColor = ccc3(130, 207, 41) 
	local state = self:GetOneDailyActivityItemState(id)
	local languageFlag = "M_HEROTREASURE_OPEN"
	if 1 ~= state then
		languageFlag = "M_ACTIVITY_CLOSE"
		txtColor = ccc3(255,0,0)
	end
	local content = LanguageLocalization:GetLocalization(languageFlag)
	return content,txtColor
end

function ActivityLayerDataCenter:GetOneActivityItemIcon(id)
	local oneItem = Activity[id]
	if nil == oneItem then
		return nil
	end

	local binName = KICON_BIN
	local iconName = "map_ui_system_icon_FRAME_" .. string.upper(oneItem.Activity_Icon)
	local nResult = TXGUI.getResourceIDByName(iconName)
	if -1 == nResult then
		iconName = "map_ui_system_icon_FRAME_ICON_REWARD"
	end
	return iconName,binName
end
--主城活动的按钮
function ActivityLayerDataCenter:GetMainMenuActivityItemIcon()
	local binName = KICON_BIN
	local iconName = "map_ui_system_icon_FRAME_ICON_ACTIVITY"
	return iconName,binName
end

function ActivityLayerDataCenter:GetOneDailyActivityItemState(id)
	local oneItem = self.Items[id]
	if nil == oneItem then
		return nil
	end
	return oneItem.state
end

-------------------------------------------------暴露给c++使用代码--------------------------
