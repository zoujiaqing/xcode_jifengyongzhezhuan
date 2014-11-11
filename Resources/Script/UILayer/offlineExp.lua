require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameterControl")
require("Script/MainMenuUILayer")
offlineExp = {
	LayerInstance = nil,
	Layout = nil,

	TEXT_TITLE=nil,
	TEXT_REMAINONHOOKTIME = nil,
	
				
	NUM_REMAINONHOOKTIME= nil,	
	TEXT_CANONHOOKTIME	= nil	,
	BAR_EXP	= nil,	
	BUTTON_GOT1	= nil,
	TEXT_GOT1= nil,	
	TEXT_EXPGOTNOW= nil,
	NUM_EXPGOTNOW= nil,			
	BUTTON_EXCHANGE	= nil,
					
	NUM_ALLEXP = nil,			
	TEXT_EXPSAVE = nil,
	BUTTON_GOT2 = nil,			
					
	TEXT_GOT2 = nil,			
	BUTTON_CLOSE= nil,
	timer = nil,
}

function offlineExp:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil

	self.TEXT_TITLE=nil
	self.TEXT_REMAINONHOOKTIME = nil
	
				
	self.NUM_REMAINONHOOKTIME= nil	
	self.TEXT_CANONHOOKTIME	= nil	
	self.BAR_EXP	= nil				
	self.BUTTON_GOT1	= nil		
	self.TEXT_GOT1= nil		
	self.TEXT_EXPGOTNOW= nil			
	self.NUM_EXPGOTNOW= nil			
	self.BUTTON_EXCHANGE	= nil						
					
	self.NUM_ALLEXP = nil			
	self.TEXT_EXPSAVE = nil		
	self.BUTTON_GOT2 = nil			
					
	self.TEXT_GOT2 = nil				
	self.BUTTON_CLOSE= nil				


	if(TimeManager:Get():hasTimer(TIMER_OFFLINE_EXP_COUNTDOWN)) then
		TimeManager:Get():unregisterLuaTimer(self.timer, TIMER_OFFLINE_EXP_COUNTDOWN)
	end
   removeMsgCallBack("CSEquipmentComposeRsp", offlineExp.CSEquipmentComposeRspCallBack)
   removeMsgCallBack("CSAttributesRsp", offlineExp.CSAttributesRspCallBack)
end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= offlineExp.LayerInstance then
			if G_CurLayerInstance ~= offlineExp.LayerInstance then
				offlineExpCloseBtnClick(nil)
			end
		end
	end
end

local function onReceiveDestructorHandler()
       	offlineExp:ResetValue()
        TXGUI.UIManager:sharedManager():removeUILayout("offlineExpPanel")
end

function offlineExp:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	 self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/offlineExp.plist",self.LayerInstance, "offlineExpPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["offlineExp"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		--InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()
	--self:ShowLeftBgIcons()

	--[[
	for index,value in pairs(SkillDataCenter.DataTable) do
		self:ShowOneSkillItem(index,value.SkillID)
	end	
	--]]
	PushOneLayer(self.LayerInstance,"","")
	--SetChatBar(true,-1)
end

function offlineExp:InitLayer()
	self.closeBtn = self.Layout:FindChildObjectByName("BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
	self.closeBtn:registerScriptTapHandler(offlineExpCloseBtnClick);

	
	--set label value
	--[[local s = GetLuaLocalization;
	self.Layout:FindChildObjectByName("titleLabel").__UILabel__:setString(s('M_HEROTREASURE_DESCRIPTION'))
	self.Layout:FindChildObjectByName("labelconsume").__UILabel__:setString(s('M_HEROTREASURE_COST'))
	self.Layout:FindChildObjectByName("labelget").__UILabel__:setString(s('M_HEROTREASURE_GET'))
	self.Layout:FindChildObjectByName("labelremaintimes").__UILabel__:setString(s('M_HEROTREASURE_REMAINTIME'))
	self.Layout:FindChildObjectByName("labelopen").__UILabel__:setString(s('M_HEROTREASURE_OPEN'))
--]]
local s = GetLuaLocalization;
	self.TEXT_TITLE=self.Layout:FindChildObjectByName("TEXT_TITLE").__UILabel__;
	self.TEXT_TITLE:setString(s('M_OFFLINE_EXP_TITLE'))
	self.TEXT_REMAINONHOOKTIME = self.Layout:FindChildObjectByName("TEXT_REMAINONHOOKTIME").__UILabel__;
	self.TEXT_REMAINONHOOKTIME:setString(s('M_OFFLINE_EXP_MAXTIME'))
	

	self.NUM_REMAINONHOOKTIME = self.Layout:FindChildObjectByName("NUM_REMAINONHOOKTIME").__UILabel__;	

	--剩余挂机时间06:52:24
	self.TEXT_CANONHOOKTIME	 = self.Layout:FindChildObjectByName("TEXT_CANONHOOKTIME").__UILabel__;
	self.TEXT_CANONHOOKTIME:setString(s('M_OFFLINE_EXP_REMAINTIME'))
	
	self.NUM_CANONHOOKTIME	 = self.Layout:FindChildObjectByName("NUM_CANONHOOKTIME").__UILabel__;
	self.TEXT_GOT1= self.Layout:FindChildObjectByName("TEXT_GOT1").__UILabel__;
	self.TEXT_EXPGOTNOW= self.Layout:FindChildObjectByName("TEXT_EXPGOTNOW").__UILabel__;
	self.TEXT_EXPGOTNOW:setString(s('M_OFFLINE_EXP_GETNOW'))
	
	self.TEXT_EXPSAVE = self.Layout:FindChildObjectByName("TEXT_EXPSAVE").__UILabel__;
	self.TEXT_EXPSAVE:setString(s('M_OFFLINE_EXP_SAVENOW'))
	self.TEXT_GOT2 = self.Layout:FindChildObjectByName("TEXT_GOT2").__UILabel__;

	self.BAR_EXP	= self.Layout:FindChildObjectByName("BAR_EXP").__UIPicture__;
	  self.BAR_EXP:getCurrentNode():setAnchorPoint(ccp(0,0.5))
	  local tmpSize = self.BAR_EXP:getSpriteSize()
          local oldPos = self.BAR_EXP:getPosition()
	  self.BAR_EXP:setPosition(ccp(oldPos.x-tmpSize.width/2,oldPos.y))


	self.FRAME_EXP	= self.Layout:FindChildObjectByName("FRAME_EXP").__UIPicture__;


--	local equipBtNode,nodePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","")

	self.NUM_EXPGOTNOW= self.Layout:FindChildObjectByName("NUM_EXPGOTNOW").__UILabel__;
					
	self.NUM_ALLEXP = self.Layout:FindChildObjectByName("NUM_ALLEXP").__UILabel__;
	    ---button
	self.BUTTON_GOT1	= self.Layout:FindChildObjectByName("BUTTON_GOT1").__UIButton__
	self.BUTTON_EXCHANGE	= self.Layout:FindChildObjectByName("BUTTON_EXCHANGE").__UIButton__;
	self.BUTTON_GOT2 = self.Layout:FindChildObjectByName("BUTTON_GOT2").__UIButton__;
					
	self.itemGot1 = self.BUTTON_GOT1:getMenuItemSprite();
	self.itemGot1:registerScriptTapHandler(
function()
	  self:sendCSEquipmentComposeReq(100, self.exp_got_now);
end
	);

	

	self.itemGot2 =  self.BUTTON_GOT2:getMenuItemSprite();
	self.itemGot2:registerScriptTapHandler(
function()
	  self:sendCSEquipmentComposeReq(101, self.all_exp);
end
);

        self.itemExchage = self.BUTTON_EXCHANGE:getMenuItemSprite();
		self.itemExchage:registerScriptTapHandler(
function()
	  self:sendCSEquipmentComposeReq(102, self.exp_got_now);
end
);
	

	self.vip = UserData:GetUserInfo().m_vip
	self:onVipChange()

	
	addMsgCallBack("CSAttributesRsp", offlineExp.CSAttributesRspCallBack, offlineExp)
	
	addMsgCallBack("CSEquipmentComposeRsp", offlineExp.CSEquipmentComposeRspCallBack, offlineExp)
	self:sendCSGetUserAttributesReq()
	

end
------change lebel value----
function onUpdateCDTime1(remainTime)
	if offlineExp.NUM_CANONHOOKTIME ~= nil then 
	 	offlineExp.NUM_CANONHOOKTIME:setString(""..TimeManager:Get():secondsToString(remainTime))
	end
end

function offlineExp:onVipChange()
	
	self.vip = UserData:GetUserInfo().m_vip
	self.offhour = Vip[self.vip].Off_Line
	self.NUM_REMAINONHOOKTIME:setString(self.offhour.."H");
       

end


function offlineExp:updateExpGotNow()
 	if offlineExp.NUM_EXPGOTNOW ~= nil then 
	        local mainMenuNode = GetMainMenuLayerNode()
		
		self.NUM_EXPGOTNOW:setString("" ..  MainMenuLayer:GetNumByFormat(self.exp_got_now))
		self.full_exp = UserData:GetUserInfo().suspend_limit_exp;

		local scalei = self.exp_got_now / self.full_exp
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		scalei = scalei * scale;
		print("updateExpgetnow"..scalei.."all exp :"..self.full_exp)
		self.BAR_EXP:setScale(scalei,1);
	end
end 

function offlineExp:updateExpAll()

	self.NUM_ALLEXP:setString("" .. MainMenuLayer:GetNumByFormat(self.all_exp))
end 
-------------------message call back-----------------------------------------------
function offlineExp:CSAttributesRspCallBack(msg) 
	print("Rcv CSAttributesRspCallBack msg")
	if self.LayerInstance == nil then
		return
	end
	
			--PB_ATTR_HOLD_ON_LINE_ACCU_TIME,
			--PB_ATTR_PLAYER_HOLD_EXP,
			--PB_ATTR_PLAYER_STORE_EXP,
	for _,v in ipairs(msg.values) do
		print("\tkey:" .. v.key .. "value : "..v.value)
		if v.key == PB_ATTR_HOLD_ON_LINE_ACCU_TIME then

			

			--self.accumulate_time = v.value
		
			--self.TEXT_CANONHOOKTIME:setString(TimeManager:Get():secondsToString(self.offhour*3600-v.value))
			--self.labelRemainNumber:setString(""..(Vip[self.vip].Mine-v.value))
			self.remain_time = self.offhour*3600-v.value;
			print("remain time is :" .. self.remain_time.. self.offhour)
--			self.remain_time = 8600
			local nowTime = TimeManager:Get():getCurServerTime()
			if(TimeManager:Get():hasTimer(TIMER_OFFLINE_EXP_COUNTDOWN)) then
				TimeManager:Get():renewTimer(TIMER_OFFLINE_EXP_COUNTDOWN, nowTime+self.remain_time)
			else
				self.timer=TimeManager:Get():registerLuaTimer(onUpdateCDTime1, TIMER_OFFLINE_EXP_COUNTDOWN, nowTime+self.remain_time)
			end
	    elseif v.key == PB_ATTR_PLAYER_VIP_LV then --vip level
			offlineExp:onVipChange();
	    elseif v.key == PB_ATTR_PLAYER_HOLD_EXP then 


	    self.exp_got_now = v.value
	       self:updateExpGotNow()
	    
	    elseif v.key == PB_ATTR_PLAYER_STORE_EXP then
	    --挂机转存经验值

	    self.all_exp = v.value
	  self:updateExpAll()
	    
	    end
	end
	return false
end



function offlineExp:CSEquipmentComposeRspCallBack(msg)

	HideWaitingLayer()
	if self.LayerInstance == nil then
		return false
	end
	if msg.result ~= 0 then
	    GameApi:showMessage("领取失败"..msg.result);
	     return false
	end
	if msg.exchange_item_id == 100 or msg.exchange_item_id == 101 then
		local m = 0;
		if  msg.exchange_item_id == 100 then
			m = self.exp_got_now
		else
			m = self.all_exp
		end
	
	  GameApi:showMessage("领取成功");
	  return true
	elseif msg.exchange_item_id == 102 then 
	       
	  GameApi:showMessage(GetLuaLocalization("M_OFFLINE_EXP_SAVESUCCESS"));
	  return true

	end
	return false
end
----------------------send network msg----------------------------------------------------
function offlineExp:sendCSGetUserAttributesReq()
	--get open times on baozhang
	local msgname="CSAttributesReq"
	Req = {
		keys = {
		PB_ATTR_HOLD_ON_LINE_ACCU_TIME,
			PB_ATTR_PLAYER_HOLD_EXP,
			PB_ATTR_PLAYER_STORE_EXP,
			} --PB_ATTR_DAILY_TIMES_HERO_TREASURE
		
	}
	send_message(msgname, Req, true)
end

function offlineExp:sendCSEquipmentComposeReq(item, mult)
	--get open times on baozhang
	if mult == 0 then
	   if item == 102 then
	      GameApi:showMessage("没有挂机经验,不能进行转存储");
	   else
	      GameApi:showMessage("没有可领取经验");
	   end
	   return
	end
	local msgname="CSEquipmentComposeReq"
	Req = {
		 exchange_item_id= item,
		 from_item_pos ={},
		 multi= mult,
		
	}
	send_message(msgname, Req, true)
end

-------------------button callback------------------------------------------------
function offlineExpCloseBtnClick(pObj)
	TXGUI.UIManager:sharedManager():removeUILayout("offlineExpPanel")
	--TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
	SetChatBar(true,-1)
	CCTextureCache:sharedTextureCache():removeUnusedTextures()

	RemoveOneLayer(offlineExp.LayerInstance)
	--NotificationCenter:defaultCenter():unregisterScriptObserver(offlineExp.closeCallBackFun)

	offlineExp.LayerInstance:removeFromParentAndCleanup(true);
	offlineExp.LayerInstance = nil
	offlineExp:ResetValue()
    if nil ~= DailyRewardMainLayer then
            DailyRewardMainLayer:ShowOrNot(true)
    end
end



--------------notification-----

local function updateNotification(message)
	if G_CurLayerInstance ~= offlineExp.LayerInstance then
		return
	end

	if message == GM_ATTR_VIP_LEVEL_CHANGE then
		--print("vip level up")
		--self.vip = UserData:GetUserInfo().m_vip
		--self.onVipChange()
	
	end
	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(closeLayerCallBack);
