require("Script/Language")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/handleMessage")

DailyRewardStaminaLayer = {
	LayerInstance = nil,
	Layout = nil,
	GetStaminaBtn = nil,
	LayerTitleLabel = nil,
	GetStaminaBtnLabel = nil,
	labelDesLabel = nil,
	labelDes2Label = nil,
	countDownTime1 = nil,
	countDownTime2 = nil ,
	startStaminaTimer1 = 0 ,
	endStaminaTimer1 = 0 ,
	startStaminaTimer2 = 0 ,
	endStaminaTimer2 = 0 ,
	daojishi = true ,
	endGetStaminaTimer = 0
}

function DailyRewardStaminaLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil

	self.GetStaminaBtn = nil
	self.LayerTitleLabel = nil
	self.GetStaminaBtnLabel = nil
	self.labelDesLabel = nil
	self.labelDes2Label = nil
	self.countDownTime1 = nil
	self.countDownTime2 = nil 
	self.startStaminaTimer1 = 0 
	self.endStaminaTimer1 = 0 
	self.startStaminaTimer2 = 0 
	self.endStaminaTimer2 = 0 
	self.daojishi = true
	removeMsgCallBack("CSGetDailyRewardRsp",  DailyRewardStaminaLayer.HandleGetDailyRewardStaminaRsp)
	TimeManager:Get():stopTimer(TIMER_REWARD_STAMINA_REMAIN);
	
end
-- 剩余时间更新
local function onUpdateStartRemainTime(remainTime)
	if remainTime <= 0 then
		remainTime = 0
	end

	if	remainTime > 0 then
	 print("onUpdateStartRemainTime > 0")
		local remainStr = TimeManager:Get():secondsToString(remainTime)
		local remainTimeLabel = nil

			remainTimeLabel = DailyRewardStaminaLayer.countDownTime2

		if	remainTimeLabel ~= nil then
			remainTimeLabel:setString(remainStr)
		end
	else
	   print("onUpdateStartRemainTime == 0")
	   DailyRewardStaminaLayer.labelDes2Label:setVisible(true)
	   DailyRewardStaminaLayer.countDownTime1:setVisible(false)
	   DailyRewardStaminaLayer.countDownTime2:setVisible(false)
	   DailyRewardStaminaLayer.GetStaminaBtn:getMenuItemSprite():setEnabled(true)
	end
end
function setGetStaminaTime(timeNum)

   local endOfTime = timeNum+ TimeManager:Get():getTimeZone()*60
  	local nowTime = TimeManager:Get():getCurServerTime()+ TimeManager:Get():getTimeZone()*60
	local relativeTime = nowTime % 86400 
	local startDayTime = nowTime - relativeTime 
	
	if  endOfTime  > startDayTime then
	    print(" endOfTime  > startDayTime")
		DailyRewardStaminaLayer.endGetStaminaTimer = endOfTime % 86400 
	else
	   print(" endOfTime  <= startDayTime")
	   DailyRewardStaminaLayer.endGetStaminaTimer = 0 
	end
  local dayhour =( DailyRewardStaminaLayer.endGetStaminaTimer) /3600
  print("setGetStaminaTime"..DailyRewardStaminaLayer.endGetStaminaTimer.."hour"..dayhour)
  if DailyRewardStaminaLayer.LayerInstance == nil then
   print("setGetStaminaTime == nil")
  end
end
local function onReceiveDestructorHandler()	
	TXGUI.UIManager:sharedManager():removeUILayout("DailyStamina")
	DailyRewardStaminaLayer:ResetValue()
end

--Note: 创建Layer
function DailyRewardStaminaLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
    
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/DailyStamina.plist",self.LayerInstance, "DailyStamina", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()        
        local tag = UITagTable["DailyRewardStaminaLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,1,tag)       
	end	

	self:InitLayer()
	
	
	
	return self.LayerInstance
end




function DailyRewardStaminaLayer:HandleGetDailyRewardStaminaRsp(msg)

	--HideWaitingLayer()
	if self.LayerInstance == nil then
		return
	end
	if(msg.succ) then
	--GameApi:showMessage( LanguageLocalization:GetLocalization());
	   self.endGetStaminaTimer =  self.endStaminaTimer2 
	   local dtNextTime ,dtNextEndTime =DailyRewardStaminaLayer:getTimeAttrube()
	   local remainStr = TimeManager:Get():secondsToString(dtNextTime )
	   local timerid = TIMER_REWARD_STAMINA_REMAIN
	   print(timerid)
	  print("DailyRewardStaminaLayer:HandleGetDailyRewardStaminaRsp".."id"..timerid.."dtNextTime"..dtNextTime.."dtNextEndTime"..dtNextEndTime)
	  TimeManager:Get():stopTimer(TIMER_REWARD_STAMINA_REMAIN)
	  TimeManager:Get():registerLuaTimer(onUpdateStartRemainTime, TIMER_REWARD_STAMINA_REMAIN, dtNextEndTime)
	   self.countDownTime2:setString(remainStr);
		self.labelDes2Label:setVisible(false)
	   self.countDownTime1:setVisible(true)
	   self.countDownTime2:setVisible(true)
	   self.GetStaminaBtn:getMenuItemSprite():setEnabled(false)
	   DailyRewardMainLayer:ShowRewardTipIconOrNot()
	   if DailyRewardStaminaLayer.LayerInstance == nil then
		 print("HandleGetDailyRewardStaminaRsp == nil")
	   end
	else 

		print("HandleGetDailyRewardStaminaRsp error ") 
		
		
	end
end
function DailyRewardStaminaLayer:getTimeAttrubeBool()
 local timeZoneSec = TimeManager:Get():getTimeZone()*60 
 local nowTime = TimeManager:Get():getCurServerTime()+timeZoneSec
 local thisTimedt = nowTime%86400 
 self.startStaminaTimer1 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,1)
 self.endStaminaTimer1 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,2) 
 self.startStaminaTimer2 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,3) 
 self.endStaminaTimer2 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,4) 
 
 if thisTimedt < self.startStaminaTimer1 then
	
	  self.daojishi = true 
	 
	elseif thisTimedt < self.endStaminaTimer1 then
	
	   self.daojishi = false
	   if self.endStaminaTimer1 <= self.endGetStaminaTimer then
		   self.daojishi = true ;
		   
	   end
	elseif thisTimedt < self.startStaminaTimer2  then
	
	   self.daojishi = true 
	elseif thisTimedt < self.endStaminaTimer2 then
	 --print("dd4")
	   self.daojishi = false
	   if self.endStaminaTimer2 <= self.endGetStaminaTimer then
		   self.daojishi = true ;
	   end
	else 
	  
	   self.daojishi = true
	end
	
	return ( not self.daojishi)

end
 function DailyRewardStaminaLayer:getTimeAttrube()
    local timeZoneSec = TimeManager:Get():getTimeZone()*60 
	local nowTime = TimeManager:Get():getCurServerTime()+timeZoneSec
	local beginTime = nowTime/86400 
	local begintime1 = (math.ceil(beginTime)-1)*86400
	local thisTimedt = nowTime%86400 
	--local thisTimedt = nowTime - beginDay 
	--print(nowTime.."dt"..thisTimedt .."endGetStaminaTimer"..self.endGetStaminaTimer .."timeZoneSec"..timeZoneSec.."yu"..nowTime-begintime1.."hour"..thisTimedt/3600)

	local nextEndTime =0 ;
	local dNextTime = 0 ;
	if thisTimedt < self.startStaminaTimer1 then
	--print("dd1")
	  self.daojishi = true 
	  dNextTime = self.startStaminaTimer1 - thisTimedt
	  nextEndTime = self.startStaminaTimer1 
	elseif thisTimedt < self.endStaminaTimer1 then
	--print("dd2")
	   self.daojishi = false
	   if self.endStaminaTimer1 <= self.endGetStaminaTimer then
		   self.daojishi = true ;
		   dNextTime = self.startStaminaTimer2 - thisTimedt
		   nextEndTime = self.startStaminaTimer2
	   end
	elseif thisTimedt < self.startStaminaTimer2  then
	--print("dd3")
	   self.daojishi = true 
	 
	    dNextTime = self.startStaminaTimer2 - thisTimedt
		nextEndTime = self.startStaminaTimer2 
	elseif thisTimedt < self.endStaminaTimer2 then
	 --print("dd4")
	   self.daojishi = false
	   if self.endStaminaTimer2 <= self.endGetStaminaTimer then
		   self.daojishi = true ;
		   dNextTime = self.startStaminaTimer1+86400 - thisTimedt
		   nextEndTime = self.startStaminaTimer1+86400
	   end
	else 
	-- print("dd5")
	   self.daojishi = true
	    dNextTime = self.startStaminaTimer1+86400 - thisTimedt
		nextEndTime = self.startStaminaTimer1+86400 
	end
		nextEndTime = begintime1+nextEndTime-timeZoneSec
	return dNextTime ,nextEndTime
end

function DailyRewardStaminaLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(DailyRewardStaminaLayerCloseBtnClick);

	self.GetStaminaBtn = self.Layout:FindChildObjectByName("GetStaminaBtn").__UIButton__
	local GetStaminaBtnMenuItem = self.GetStaminaBtn:getMenuItemSprite();
	GetStaminaBtnMenuItem:registerScriptTapHandler(confirmToGetStamina);
	
	--self.AcceptRewardBtn:setVisible(false)
	self.GetStaminaBtnLabel = self.Layout:FindChildObjectByName("getStaminaLabel").__UILabel__
	--self.AcceptRewardBtnLabel:setVisible(false)
	--if nil ~= self.GetStaminaBtnLabel then
		--local languageFlag = 'M_RECUPERATE_NAME'
		--local content = LanguageLocalization:GetLocalization(languageFlag)
		--self.GetStaminaBtnLabel:setString(content)
	--end
	local bgIconPath = "UI/ui_pic_staminareward.png"
	local herotreasure = CCSprite:create(bgIconPath)
	if herotreasure ~= nil then
		self.Layout:FindChildObjectByName("pic").__UIPicture__:setSprite(herotreasure)
	end
	--Note: 显示Title
	self.LayerTitleLabel = self.Layout:FindChildObjectByName("titleLabel").__UILabel__
	if nil ~= self.LayerTitleLabel then
		local languageFlag = 'M_RECUPERATE_NAME'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.LayerTitleLabel:setString(content)
	end

	self.labelDesLabel = self.Layout:FindChildObjectByName("labelDes").__UILabel__
	if nil ~= self.labelDesLabel then
		local languageFlag = 'M_RECUPERATE_TEXT_1'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.labelDesLabel:setString(content)
	end

	self.labelDes2Label = self.Layout:FindChildObjectByName("labelDes2").__UILabel__
	if nil ~= self.labelDes2Label then
		local languageFlag = 'M_RECUPERATE_TEXT_2'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.labelDes2Label:setString(content)
	end
	self.countDownTime1 = self.Layout:FindChildObjectByName("countDownTime1").__UILabel__
	if nil ~= self.countDownTime1 then
		local languageFlag = 'M_RECUPERATE_TEXT_3'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.countDownTime1:setString(content)
	end

	self.startStaminaTimer1 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,1)
	self.endStaminaTimer1 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,2) 
	self.startStaminaTimer2 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,3) 
	self.endStaminaTimer2 = dailyRewardMainLayerDataCenter:GetOneDailyRewardItemStartTime(6010 ,4) 

	local dtNextTime ,dtNextEndTime =DailyRewardStaminaLayer:getTimeAttrube()
	local remainStr = TimeManager:Get():secondsToString(dtNextTime )
	print("dtNextTime"..dtNextTime.."dtNextEndTime"..dtNextEndTime.."aa"..remainStr)

	
	addMsgCallBack("CSGetDailyRewardRsp", DailyRewardStaminaLayer.HandleGetDailyRewardStaminaRsp,DailyRewardStaminaLayer)
	
	
	
	self.countDownTime2 = self.Layout:FindChildObjectByName("countDownTime2").__UILabel__
	
	if self.daojishi == true then
		TimeManager:Get():registerLuaTimer(onUpdateStartRemainTime, TIMER_REWARD_STAMINA_REMAIN, dtNextEndTime)
		self.labelDes2Label:setVisible(false)
	
		self.countDownTime2:setString(remainStr);
		GetStaminaBtnMenuItem:setEnabled(false)
	else
	    GetStaminaBtnMenuItem:setEnabled(true)
		self.labelDes2Label:setVisible(true)
		self.countDownTime1:setVisible(false)
		self.countDownTime2:setVisible(false)
	end

end

-------------------事件回调------------------------------------------------
local function closeLayer()
	if nil ~= DailyRewardStaminaLayer.LayerInstance then
	
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyRewardStaminaLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyRewardStaminaLayer.LayerInstance = nil
		
		DailyRewardStaminaLayer:ResetValue()
	else
	print("DailyRewardStaminaLayer.LayerInstance == nil")
	end
end
 function confirmToGetStamina(obj)
	print("confirmToGetStamina")
	DailyRewardStaminaLayer:SendGetDailyReqMessage()
	--DailyRewardStaminaLayerCloseBtnClick(nil)
end

function DailyRewardStaminaGetStaminaBtnClick(pObj)
	  MessageBox:Show(LanguageLocalization:GetLocalization("M_RECUPERATE_TIPS"),confirmToGetStamina,nil,MB_YESNO,ccc3(255,255,255),26)
end
function DailyRewardStaminaLayerCloseBtnClick(pObj)
	if nil ~= DailyRewardMainLayer then
		DailyRewardMainLayer:ShowOrNot(true)
	end	
	closeLayer()
end
function DailyRewardStaminaLayer:SendGetDailyReqMessage()
	
	local sum = PB_ATTR_GET_STAMINA_RESTORE_TIME
	print("DailyRewardStaminaLayer:SendGetDailyReqMessage"..sum)
	CSGetDailyRewardReq = {
		key	= PB_ATTR_GET_STAMINA_RESTORE_TIME
	}
	local msgname="CSGetDailyRewardReq"
	local ret = send_message(msgname, CSGetDailyRewardReq, true)
	print("DailyRewardStaminaLayer:SendGetDailyReqMessage11")
	return ret;
end
---------------------------------------------------------------------------

local function updateNotification(message)

	if message == GM_NETWORK_DISCONNCT then
		DailyRewardStaminaLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeLayer()
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
