require("Script/Activity/activity")
--require("Script/UILayer/ActivityCentre")

--[[ 结构
--table[id]=[table,table,...]
--]]
local timeData = {}
local mgrInited = false
local syncedServerTime = false
local notificationFunc = nil;
local receivedTime = false;

local activityIdList = {}
--世界boss
activityIdList[1] = {6240}
--主城防守
activityIdList[2] = {6220}
--最强勇者 
activityIdList[3] = {6250}


--是否存在正在计时的活动
local function hasTimingActivity()
	for index,value in pairs(Activity) do
		if isActivityTypeNotZero(index) then
			if GetBlinkStateByActivityId(index) then --ActivityLayerDataCenter:GetOneDailyActivityItemState(index) == 1 then
				return true, index
			end
		end
	end
	return false, index
end

--播放主城活动按钮特效
function PlayMainMenuActivityButtonEffect()
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if nil == topLayout then
		return
	end
	local activityButton = topLayout:FindChildObjectByName("button_activity").__UIButton__
	local particle = activityButton:getAnimationNode():getChildByTag(10000)
	local bEnable, index = hasTimingActivity() 
	
	if bEnable then
		if nil == particle then
			particle = ParticleManagerX:sharedManager():getParticles_uieffect("particle_effect_taskguide")
            if nil ~= particle then
                particle:setPosition(ccp(activityButton:getAnimationNode():getContentSize().width * 0.5,activityButton:getAnimationNode():getContentSize().height * 0.5));
                activityButton:getAnimationNode():addChild(particle,10,10000)
            end

		end

		local iconName,binName = ActivityLayerDataCenter:GetOneActivityItemIcon(index)
		CCLuaLog("-------------iconName: " .. iconName)
		if nil ~= iconName then
			local icon = AspriteManager:getInstance():getOneFrame(binName,iconName)		
			activityButton:setIconSprite(icon)
		end	
	else
		if nil ~= particle then
			activityButton:getAnimationNode():removeChild(particle,true)
		end

		local iconName,binName = ActivityLayerDataCenter:GetMainMenuActivityItemIcon()
		if nil ~= iconName then
			local icon = AspriteManager:getInstance():getOneFrame(binName,iconName)		
			activityButton:setIconSprite(icon)
		end	
	end
end


--收到定时活动时间消息
local function onMsgTimingActivityDataRsp(msg)
	if msg.interval ~= nil then
		local nowTime = TimeManager:Get():getCurServerTime()
		for _ , timePair in pairs(msg.interval) do
			if timeData[timePair.id] == nil then
				timeData[timePair.id] = {}; --table
			end

			for _, timePair2 in pairs(timePair.ac_time) do
				table.insert(timeData[timePair.id], {
					enterTime = timePair2.enter_time,
					startTime = timePair2.start_time,
					endTime = timePair2.end_time
				});
			CCLuaLog("update activity data time id " .. timePair.id .. " enterTime " .. timePair2.enter_time .. " startTime : " .. timePair2.start_time .. " endTime : " .. timePair2.end_time .. " nowTime : " .. nowTime);
			end

			--test
			--[[
			if timePair.id == 6220 then
				timePair.enter_time = nowTime + 30;
				timePair.start_time = nowTime + 30;
				timePair.end_time = nowTime + 90;
				CCLuaLog("-------change time, enter_time: " .. timePair.enter_time .. " nowTime : " .. nowTime);
			end
			if timePair.id == 6250 then
				timePair.enter_time = nowTime + 90;
				timePair.start_time = nowTime + 90;
				timePair.end_time = nowTime + 150;
				CCLuaLog("-------change time, enter_time: " .. timePair.enter_time .. " nowTime : " .. nowTime);
			end
			CCLuaLog("update activity data time id " .. timePair.id .. " enterTime " .. timePair.enter_time .. " startTime : " .. timePair.start_time .. " endTime : " .. timePair.end_time .. " nowTime : " .. nowTime);
			timeData[timePair.id] = {
				enterTime = timePair.enter_time,
				startTime = timePair.start_time,
				endTime = timePair.end_time,
			} 
			--]]
		end

		receivedTime = true;
		--服务器有更新
		ActivityLayerDataCenter:updateActivityStateData()
		PlayMainMenuActivityButtonEffect()
		renewActivityUpdateTimer() --刷新了服务器时间需要更新本地计时器

		if notificationFunc ~= nil then --反注册广播
			NotificationCenter:defaultCenter():unregisterScriptObserver(notificationFunc);
		end
	end
end

function waitForActivityReceivedTimeMsg()
	if not receivedTime then --未收到服务器发来的时间
		CCLuaLog("waitForActivityReceivedTimeMsg...");
		WaitingLayerManager:Get():PushOneMessage("CSGetSpecActivityTimeReq");
	end
end

--活动定时器回调
local function onUpdateActivityTime(remainTime)
	--CCLuaLog("-----------activity remainTime " .. math.floor(remainTime/3600) .. ":" .. math.mod(math.floor(remainTime/60), 60) .. ":" .. math.mod(remainTime, 60) .. "------------------ " )
	if remainTime == 0 then --计时完成则更新计时器
		CCLuaLog("-----------activity remainTime 0------------------ " )
		ActivityLayerDataCenter:updateActivityStateData()
		PlayMainMenuActivityButtonEffect()
		DailyActivityLayer:FlushAllItem()
		renewActivityUpdateTimer()
	end
end

--查找下一个倒计时秒数,可能是开始时间或是结束时间
local function getNextTimerSeconds()
	local seconds = 86400
	local nowTime = TimeManager:Get():getCurServerTime()
	for index,value in pairs(Activity) do -- 找出限时活动里最少的计时时间秒数
		if value.Activity_Type == '1' then --只对限时活动做检测
			local timePair = timeData[index]
			if timePair == nil then
				return seconds
			end

			for _, timeRange in pairs(timePair) do
				local timeDiff = timeRange.enterTime - nowTime;
				if timeDiff > 0 and timeDiff < seconds then
					--CCLuaLog("---------------nextTimer index: " .. index .. " enterTime : " .. timeRange.enterTime .. " nowTime : " .. nowTime);
					seconds = timeDiff
				end
				timeDiff = timeRange.endTime - nowTime;
				if timeDiff > 0 and timeDiff < seconds then
					--CCLuaLog("---------------nextTimer index: " .. index .. " endTime : " .. timeRange.endTime .. " nowTime : " .. nowTime);
					seconds = timeDiff
				end
			end	
		end
	end

	if	seconds == 86400 then --所有限时活动全部检测都没有，可能是在一天的结束，所以需要再次请求服务器时间
		--SendGetTimingActivityDataReq()
	end

	return seconds
end

--初始化或更新活动定时器
function renewActivityUpdateTimer()
	local nowTime = TimeManager:Get():getCurServerTime()
	local seconds = getNextTimerSeconds()
	if (TimeManager:Get():hasTimer(TIMER_ACTIVITY_UPDATE)) then
		TimeManager:Get():renewTimer(TIMER_ACTIVITY_UPDATE, nowTime + seconds)
		CCLuaLog("--------renew activity timer : " .. seconds)
	else
		TimeManager:Get():registerLuaTimer(onUpdateActivityTime, TIMER_ACTIVITY_UPDATE, nowTime + seconds)
		CCLuaLog("--------init activity timer : " .. seconds)
	end
end

--发送获取定时活动时间消息
function SendGetTimingActivityDataReq()
	if not syncedServerTime then
		return
	end
	CCLuaLog("---- SendGetTimingActivityDataReq ----")
	CSGetSpecActivityTimeReq = {
		}
	local msgname="CSGetSpecActivityTimeReq"
	local ret = send_message(msgname, CSGetSpecActivityTimeReq, true)
	return ret
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_ATTR_SERVER_TIME_SYNCED then
		syncedServerTime = true;
		SendGetTimingActivityDataReq()
	end

	return ret
end

--初始化管理器，注册回调函数
function InitTimingActivityMgr()
	if not mgrInited then
		CCLuaLog("---- InitTimingActivityMgr ----")
		addMsgCallBack("CSGetSpecActivityTimeRsp", onMsgTimingActivityDataRsp)
		notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
		mgrInited = true
	end
	return true
end


local function getActivityStartState(activityType)
	local ret = false

	local nextStartTime = 0
	local nextStartId = 0

	local nextEndTime = 0
	local nextEndId = 0

	local nowTime = TimeManager:Get():getCurServerTime()

	if nowTime > 0 and #activityIdList[activityType] > 0 then
		for _,id in pairs(activityIdList[activityType]) do
		
			local timePair = timeData[id];
			if timePair ~= nil then
				for _,value in pairs(timePair) do

					--下一个开启时间
					if ret == false and value.startTime > nowTime then
						if nextStartTime == 0 or value.startTime < nextStartTime then
							nextStartTime = value.startTime	
							nextStartId = id
						end
					end

					--下一个结束时间
					if ret == false and value.endTime > nowTime then
						if nextEndTime == 0 or value.endTime < nextEndTime then
							nextEndTime = value.endTime
							nextEndId = id	
						end
					end
				
					--id不相同时表示正在活动中
					if nextStartId ~= nextEndId then
						ret = true
					end
				end
			end
		end

	end
	return ret, nextStartTime, nextEndTime
end

-- 获取世界BOSS开启状态，及最近的结束时间 
function GetWorldBossStartState()
	return getActivityStartState(1)
end

-- 获取主城防守开启状态，及最近的开始结束时间 
function GetCityDefendStartState()
	--return getActivityStartState(2)
    return true
end

-- 获取最强勇者开启状态，及最近的开始结束时间 
function GetBestFighterStartState()
	return getActivityStartState(3)
end

--查看给定的活动是否开启
function GetOpenStateByActivityId(activityId)
	local ret = false;
	local activityTimes = timeData[activityId];
	if nil == activityTimes then
		--CCLuaLog("---activityTimes = nil ")
		return ret
	end
	local nowTime = TimeManager:Get():getCurServerTime()
	--CCLuaLog("----activityTimes enterTime : " .. activityTimes.enterTime .. " nowTime: " .. nowTime .. " endTime: " .. activityTimes.endTime);
	for _, timeValue in pairs(activityTimes) do
		if ret == false and timeValue.enterTime <= nowTime and nowTime < timeValue.endTime then
			ret = true;
		end
	end
	return ret;
end

--查看给定的活动是否闪动
function GetBlinkStateByActivityId(id)
	CCLuaLog("GetBlinkStateByActivityId");
	local ret = false;
	local activityTimes = timeData[id];
	if nil == activityTimes then
	CCLuaLog("GetBlinkStateByActivityId1");
		return ret
	end
	local nowTime = TimeManager:Get():getCurServerTime()
	local activity = Activity[id];
	if activity == nil then
	CCLuaLog("GetBlinkStateByActivityId2");
		return ret;
	end

	CCLuaLog("blink.............................." .. id .. " .. " .. tostring(ret));
	if activity.Activity_Type == '1' then --1类型按进入时间算
		for _, timeValue in pairs(activityTimes) do
			if ret == false and timeValue.enterTime <= nowTime and nowTime < timeValue.endTime then
				ret = true;
			end
		end
	elseif activity.Activity_Type == '2' then --2类型按开始时间算
		for _, timeValue in pairs(activityTimes) do
			if ret == false and timeValue.startTime <= nowTime and nowTime < timeValue.endTime then
				ret = true;
			end
		end
	else --其它类型不闪
		ret = false;
	end

	return ret;
end

function ResetTimingActivityMgrData()
	timeData = {}
	mgrInited = false
	syncedServerTime = false
	notificationFunc = nil
	receivedTime = false
end