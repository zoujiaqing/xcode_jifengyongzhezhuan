require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameterControl")
require("Script/MainMenuUILayer")
vipReward = {
	LayerInstance = nil,
	Layout = nil,

	TEXT_TITLE=nil,
	
	
				
	
}

function vipReward:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil

	self.TEXT_TITLE=nil
	
   --removeMsgCallBack("CSEquipmentComposeRsp", vipReward.CSEquipmentComposeRspCallBack)
   -removeMsgCallBack("CSAttributesRsp", vipReward.CSAttributesRspCallBack)
end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= vipReward.LayerInstance then
			if G_CurLayerInstance ~= vipReward.LayerInstance then
				vipRewardCloseBtnClick(nil)
			end
		end
	end
end

local function onReceiveDestructorHandler()
       	vipReward:ResetValue()
        TXGUI.UIManager:sharedManager():removeUILayout("vipRewardPanel")
end

function vipReward:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	 self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/vipReward.plist",self.LayerInstance, "vipRewardPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["vipReward"].tag
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

function vipReward:InitLayer()
	self.closeBtn = self.Layout:FindChildObjectByName("BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
	self.closeBtn:registerScriptTapHandler(vipRewardCloseBtnClick);

	
	--set label value
	--[[local s = GetLuaLocalization;
	self.Layout:FindChildObjectByName("titleLabel").__UILabel__:setString(s('M_HEROTREASURE_DESCRIPTION'))
	self.Layout:FindChildObjectByName("labelconsume").__UILabel__:setString(s('M_HEROTREASURE_COST'))
	self.Layout:FindChildObjectByName("labelget").__UILabel__:setString(s('M_HEROTREASURE_GET'))
	self.Layout:FindChildObjectByName("labelremaintimes").__UILabel__:setString(s('M_HEROTREASURE_REMAINTIME'))
	self.Layout:FindChildObjectByName("labelopen").__UILabel__:setString(s('M_HEROTREASURE_OPEN'))

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

	
	addMsgCallBack("CSAttributesRsp", vipReward.CSAttributesRspCallBack, vipReward)
	
	addMsgCallBack("CSEquipmentComposeRsp", vipReward.CSEquipmentComposeRspCallBack, vipReward)
	self:sendCSGetUserAttributesReq()
	--]]

end
------change lebel value----


function vipReward:onVipChange()
	
	self.vip = UserData:GetUserInfo().m_vip
	
       

end





-------------------message call back-----------------------------------------------
function vipReward:CSAttributesRspCallBack(msg) 
	--[[print("Rcv CSAttributesRspCallBack msg")
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
			vipReward:onVipChange();
	    elseif v.key == PB_ATTR_PLAYER_HOLD_EXP then 


	    self.exp_got_now = v.value
	       self:updateExpGotNow()
	    
	    elseif v.key == PB_ATTR_PLAYER_STORE_EXP then
	    --挂机转存经验值

	    self.all_exp = v.value
	  self:updateExpAll()
	    
	    end
	end
	return false--]]
end




----------------------send network msg----------------------------------------------------
function vipReward:sendCSGetUserAttributesReq()
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

function vipReward:sendCSEquipmentComposeReq(item, mult)
	--get open times on baozhang
	--[[if mult == 0 then
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
	--]]
end

-------------------button callback------------------------------------------------
function vipRewardCloseBtnClick(pObj)
	TXGUI.UIManager:sharedManager():removeUILayout("vipRewardPanel")
	--TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
	SetChatBar(true,-1)
	CCTextureCache:sharedTextureCache():removeUnusedTextures()

	RemoveOneLayer(vipReward.LayerInstance)
	--NotificationCenter:defaultCenter():unregisterScriptObserver(vipReward.closeCallBackFun)

	vipReward.LayerInstance:removeFromParentAndCleanup(true);
	vipReward.LayerInstance = nil
	vipReward:ResetValue()
    if nil ~= DailyRewardMainLayer then
            DailyRewardMainLayer:ShowOrNot(true)
    end
end



--------------notification-----

local function updateNotification(message)
	if G_CurLayerInstance ~= vipReward.LayerInstance then
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
