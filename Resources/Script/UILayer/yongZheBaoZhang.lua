require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameterControl")

yongZheBaoZhang = {
	LayerInstance = nil,
	Layout = nil,

	--iconFrameDailySalary = nil,
	labelcoin = nil,
	labeldiamond = nil,
	labelRemainNumber = nil,
	buttonMenuItem = nil,
	closeBtn = nil,
	buttonKaiQi  = nil,
	open_times = 0,
	all_times = nil,
	num_coin = nil,
}

function yongZheBaoZhang:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil

	--self.iconFrameDailySalary = nil
	self.labelcoin = nil
	self.labeldiamond = nil
	self.labelRemainNumber = nil
	self.buttonMenuItem = nil
	self.buttonKaiQi = nil
	self.closeBtn = nil
	self.num_coin = nil
	self.vip=nil
	removeMsgCallBack("CSGetDailyRewardRsp", yongZheBaoZhang.CSGetDailyRewardRspCallBack)
	removeMsgCallBack("CSAttributesRsp", yongZheBaoZhang.CSAttributesRspCallBack)
end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= yongZheBaoZhang.LayerInstance then
			if G_CurLayerInstance ~= yongZheBaoZhang.LayerInstance then
				yongZheBaoZhangCloseBtnClick(nil)
			end
		end
	end
end


function yongZheBaoZhang:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/yongZheBaoZhang.plist",self.LayerInstance, "yongZheBaoZhangPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["yongZheBaoZhang"].tag
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

function yongZheBaoZhang:InitLayer()
	self.closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	self.closeBtn:registerScriptTapHandler(yongZheBaoZhangCloseBtnClick);

	
	--set label value
	local s = GetLuaLocalization;
	self.Layout:FindChildObjectByName("titleLabel").__UILabel__:setString(s('M_HEROTREASURE_DESCRIPTION'))
	self.Layout:FindChildObjectByName("labelconsume").__UILabel__:setString(s('M_HEROTREASURE_COST'))
	self.Layout:FindChildObjectByName("labelget").__UILabel__:setString(s('M_HEROTREASURE_GET'))
	self.Layout:FindChildObjectByName("labelremaintimes").__UILabel__:setString(s('M_HEROTREASURE_REMAINTIME'))
	self.Layout:FindChildObjectByName("labelopen").__UILabel__:setString(s('M_HEROTREASURE_OPEN'))

	local bgIconPath = "UI/ui_pic_herotreasure.png"
	local herotreasure = CCSprite:create(bgIconPath)
	if herotreasure ~= nil then
		self.Layout:FindChildObjectByName("pic").__UIPicture__:setSprite(herotreasure)
	end
	self.buttonMenuItem = self.Layout:FindChildObjectByName("button_kai_qi").__UIButton__:getMenuItemSprite();
	self.buttonMenuItem:registerScriptTapHandler(kaiQiBtnClick);
	self.buttonKaiQi  = self.Layout:FindChildObjectByName("button_kai_qi").__UIButton__
	
	--self.iconFrameDailySalary = self.Layout:FindChildObjectByName("iconFrameDailySalary").__UIPicture__
	self.labelcoin = self.Layout:FindChildObjectByName("labelcoin").__UILabel__
	self.labeldiamond = self.Layout:FindChildObjectByName("labeldiamond").__UILabel__
	self.labelRemainNumber = self.Layout:FindChildObjectByName("labelRemainNumber").__UILabel__
	self.vip = UserData:GetUserInfo().m_vip
	self.num_coin = UserData:GetUserInfo().mine_money
	self.labelcoin:setString(self.num_coin)
	
	
	self:updateBuyTimeLable()
	yongZheBaoZhang:updateCoinLable()
	
	addMsgCallBack("CSAttributesRsp", yongZheBaoZhang.CSAttributesRspCallBack, yongZheBaoZhang)
	--addMsgCallBack("CSAttributesRsp", yongZheBaoZhang.CSAttributesRspCallBack, yongZheBaoZhang)
	--addMsgCallBack("CSNotifyLevelConfInfoRsp", yongZheBaoZhang.CSNotifyLevelConfInfoRspCallback, yongZheBaoZhang)
	
	addMsgCallBack("CSGetDailyRewardRsp", yongZheBaoZhang.CSGetDailyRewardRspCallBack, yongZheBaoZhang)
	self:sendCSGetUserAttributesReq()
	

end
------change lebel value----
function yongZheBaoZhang:updateBuyTimeLable()
	self.vip = UserData:GetUserInfo().m_vip
	print("current vip is " .. self.vip)
	local t = Vip[self.vip].Mine-self.open_times;
	if t > 0 then 
		self.labelRemainNumber:setString(""..t)
	else
		self.labelRemainNumber:setString("0")
	end
end

function yongZheBaoZhang:updateDiamondLable()
	if (self.open_times*getExchangeParameterCostNum(20501) + getExchangeParameterCostNum(20500))> 40 then
				self.need_diamond = 40;
			else
				self.need_diamond  = self.open_times*getExchangeParameterCostNum(20501)+getExchangeParameterCostNum(20500)
			end
			self.labeldiamond:setString("".. self.need_diamond )
end 


function yongZheBaoZhang:updateCoinLable()
	
	self.num_coin = UserData:GetUserInfo().mine_money
	print("yongZheBaoZhang:updateCoinLable"..self.num_coin);
	if self.labelcoin == nil then
		return
	end
	self.labelcoin:setString("" .. self.num_coin)
end 
-------------------message call back-----------------------------------------------
function yongZheBaoZhang:CSAttributesRspCallBack(msg) 
	print("Rcv CSAttributesRspCallBack msg")
	if self.LayerInstance == nil then
		return
	end
	--print(msg.succ, msg.id)
	for _,v in ipairs(msg.values) do
		print("\tkey:" .. v.key .. "value : "..v.value)
		if v.key == PB_ATTR_DAILY_TIMES_HERO_TREASURE then--yi kai chai chi shu
			print("Rcv PB_ATTR_DAILY_TIMES_HERO_TREASURE msg")
			--PB_ATTR_DAILY_TIMES_HERO_TREASURE
			self.open_times = v.value
			
			--self.labelRemainNumber:setString(""..(Vip[self.vip].Mine-v.value))
			
			yongZheBaoZhang:updateBuyTimeLable();
			yongZheBaoZhang:updateDiamondLable();
			return false;
		elseif v.key == PB_ATTR_PLAYER_VIP_LV then --vip level
			yongZheBaoZhang:updateBuyTimeLable();
		end
	end
	return false
end



--[[
message CSGetDailyRewardRsp {
        required bool succ                                      = 1;    
        required uint32 key                 = 2;    
}

--]]
function yongZheBaoZhang:CSGetDailyRewardRspCallBack(msg)

	--HideWaitingLayer()
	if self.LayerInstance == nil then
		return
	end
	if(msg.succ) then
	--GameApi:showMessage( LanguageLocalization:GetLocalization());
		GameApi:showMessage(string.format("获得%u金币", self.num_coin));
	else 
		print("max vip is "..#Vip) 
		if self.vip <  #Vip then
			GameApi:showMessage(LanguageLocalization:GetLocalization("M_HEROTREASURE_ERROR2"));
		else
			GameApi:showMessage(LanguageLocalization:GetLocalization("M_HEROTREASURE_ERROR1"));
		end
		self.buttonKaiQi:setEnabled(false);
	end
end
----------------------send network msg----------------------------------------------------
function yongZheBaoZhang:sendCSGetUserAttributesReq()
	--get open times on baozhang
	local msgname="CSAttributesReq"
	Req = {
		keys = {PB_ATTR_DAILY_TIMES_HERO_TREASURE} --PB_ATTR_DAILY_TIMES_HERO_TREASURE
		
	}
	send_message(msgname, Req, true)
end
-------------------button callback------------------------------------------------
function yongZheBaoZhangCloseBtnClick(pObj)
	TXGUI.UIManager:sharedManager():removeUILayout("yongZheBaoZhangPanel")
	--TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
	SetChatBar(true,-1)
	CCTextureCache:sharedTextureCache():removeUnusedTextures()

	RemoveOneLayer(yongZheBaoZhang.LayerInstance)
	--NotificationCenter:defaultCenter():unregisterScriptObserver(yongZheBaoZhang.closeCallBackFun)

	yongZheBaoZhang.LayerInstance:removeFromParentAndCleanup(true);
	yongZheBaoZhang.LayerInstance = nil
	yongZheBaoZhang:ResetValue()
end

function kaiQiBtnClick(pObj)
	--check if button enable
	if yongZheBaoZhang.buttonMenuItem:isEnabled() == false then
		--GameApi:showMessage( "");
		return;
	end
	--current diamond number
	user_diamond = UserData:GetUserInfo().m_diamond
	print("user diamond is " .. user_diamond)
	if(user_diamond < yongZheBaoZhang.need_diamond) then
		GameApi:showMessage("钻石不足");
		return;
	end
	local msgname="CSGetDailyRewardReq"
	Req = {
		key = PB_ATTR_DAILY_TIMES_HERO_TREASURE --6PB_ATTR_DAILY_TIMES_HERO_TREASURE
		
	}
	local ret = send_message(msgname, Req, true)
	if ret == true then 
		--ShowWaitingLayer()
		PushOneWaitingLayer(msgname)
	end
	return ret;
end



local function updateNotification(message)
	if G_CurLayerInstance ~= yongZheBaoZhang.LayerInstance then
		return
	end
	if message == GM_ATTR_BAO_ZHANG_COIN_CHANGE then
		print("update coin ")
		yongZheBaoZhang:updateCoinLable()
	end
	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(closeLayerCallBack);