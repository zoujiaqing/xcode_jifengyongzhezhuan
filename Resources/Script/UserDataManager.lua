require("Script/Friend/PlayerItemCenter")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Marquee/MarqueeDataCenter")
require("Script/HelpUI/HelpManager")
require("Script/DailyTask/dailyTaskDataCenter")
require("Script/DailyGoals/dailyGoalsDataCenter")
require("Script/DailyReward/dailyRewardMainLayerDataCenter")
require("Script/Instance/TimingActivityMgr")
require("Script/UILayer/SevenDaysUI")
require("Script/UILayer/ThreeDaysUI")
require("Script/UILayer/lotteryUI")
require("Script/UILayer/operationUI")
require("Script/Activity/dailyActivityLayerDataCenter")
require("Script/Mail/MailMsg")
require("Script/Equipment/EquipMessage")
require("Script/Fairy/FairyDataCenter")
require("Script/GrowthTarget/GrowthTargetMessage")
-- 管理用户数据

function checkUserInfo()
	initMarqueeData()
	initFairyInfo()
	registerMailCallBack()
	registerEquipCallBack()
	registerFairyCallBack()
	registerTargetCallBack()
	return true
end

function clearUserInfo()
	-- 清除其它玩家的物品数据
	PlayerItemCenter:clearOtherPlayerItems()
	-- 清除好友信息
	FriendInfoDataCenter:clearFriendInfo()
	clearMarqueeData()

	dailyTaskDataCenter:ClearAllTaskItems()
	dailyGoalsDataCenter:ClearData()
	dailyRewardMainLayerDataCenter:ClearData()
	ResetTimingActivityMgrData()
	SevenDaysUI:clearData()
	ThreeDaysUI:clearData()
	lotteryUI:clearData()
	operationUI:clearData()
	ActivityLayerDataCenter:clearData()
	unRegisterMailInfo()
	unRegisterEquipCallBack()
	unRegisterFairyCallBack()
	unRegisterFairyCallBack()
	return true
end