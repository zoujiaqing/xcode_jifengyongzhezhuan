
require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameterControl")
--require("Script/UILayer/ActivityCentre")
--require("Script/UILayer/ShowActivity")
require("Script/GameConfig/ExchangeParameterControl")
KUI_OPERAION_BIN="UI/ui_operation.bin"
--local reward_num=0

lotteryUI = {
	ID=10000,
	LayerInstance = nil,
	Layout = nil,
	showTipForOnce=true,
	TEXT_TITLE=nil,
	movePic=nil,
	superGridPic=nil,

	closeBtn= nil,
	bingoitem=nil,	
	BUTTON_BUY= nil,
	pic_area_node=nil,
	updatePicPos=0,
	reward_num=0,
	iconID={},
	iconNUM={},
	iconNumber={},
	iconPos={},
	ListTipId={},
    ListTipBtn={},
	ListRewards={},
	ListContent={},
	moveSpeed=nil,
	moveLeftTimes=0,
	stopMoveFlag=nil,
	effectId = nil,
	certainGetTimes = nil,
	mustGetId = nil,
}

function lotteryUI:clearData()
	self.ID=10000
	self.LayerInstance = nil
	self.Layout = nil
	self.showTipForOnce=true
	self.TEXT_TITLE=nil
	self.movePic=nil
	self.superGridPic=nil

	self.closeBtn= nil
	self.bingoitem=nil
	self.BUTTON_BUY= nil
	self.pic_area_node=nil
	self.updatePicPos=0
	self.iconID={}
	self.iconNUM={}
	self.iconNumber={}
	self.iconPos={}
	self.ListTipId={}
    self.ListTipBtn={}
	self.ListRewards={}
	self.ListContent={}
	self.moveSpeed=nil
	self.moveLeftTimes=0
	self.stopMoveFlag=nil
	self.reward_num=0
	self.effectId=nil
	self.certainGetTimes = nil
	self.mustGetId = nil
end

function lotteryUI:ResetValue()

	self:stopMove()
	self.ListRewards={}
	self.reward_num=0
	self.LayerInstance = nil
	self.Layout = nil
	self.movePic=nil
	self.superGridPic=nil
	self.TEXT_TITLE=nil
	self.fairyhitnode=nil
	showTipForOnce=false
	self.bingoitem=nil
	self.closeBtn= nil
	self.pic_area_node = nil
		--self.iconID= nil
		--self.iconNUM=nil
	--self.iconNumber= nil
	--self.iconPos= nil
	--self.ListTipId= nil
   --self.ListTipBtn= nil
  
    self.updatePicPos = 0
    self.stopMoveFlag=nil
	self.moveSpeed=nil
	self.moveLeftTimes=0
	self.effectId=nil
	self.certainGetTimes = nil
	self.mustGetId = nil

	--removeMsgCallBack("CSAttributesRsp", lotteryUI.CSAttributesRspCallBack)
	removeMsgCallBack("CSActivityListRsp", lotteryUI.CSActivityListRspCallBack)
	
	removeMsgCallBack("CSGetActivityRewardRsp", lotteryUI.CSGetActivityRewardRspCallBack)
	removeMsgCallBack("CSUpdateActivityInfoRsp", lotteryUI.CSUpdateActivityInfoRspCallBack)
	removeMsgCallBack("CSDelActivityInfoRsp", lotteryUI.CSDelActivityInfoRspCallBack)
	removeMsgCallBack("CSNotifyRewardToOtherRsp", lotteryUI.CSNotifyRewardToOtherRspCallBack)
	removeMsgCallBack("CSAttributesRsp", lotteryUI.CSAttributesRspCallBack)
	removeMsgCallBack("CSExchangeParameterListRsp", lotteryUI.CSExchangeParameterListRspCallBack)
end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= lotteryUI.LayerInstance then
			if G_CurLayerInstance ~= lotteryUI.LayerInstance then
				lotteryUICloseBtnClick(nil)
			end
		end
	end
end

local function onReceiveDestructorHandler()
       	lotteryUI:ResetValue()
        TXGUI.UIManager:sharedManager():removeUILayout("lotteryUIPanel")
end

function lotteryUI:formatNumber(num)
	return MainMenuLayer:GetNumByFormat(tonumber(num))
end

function lotteryUI:CreateLayer( parentLayer, id)
--[[

	local level = UserData:GetUserInfo().level
	if  level < 11 then
		GameApi:showMessage("等级达到11级才可开启")
		return;
	end
--]]
	self:ResetValue()
	--[[
	local tempid=10000;
	if ActivityCentre[10000].status == PB_ACTIVITY_STATUS_FINISH then
		tempid=10000
	elseif ActivityCentre[10080].status == PB_ACTIVITY_STATUS_FINISH then
		tempid=10080
	elseif  ActivityCentre[10090].status == PB_ACTIVITY_STATUS_FINISH then
		tempid=10090
	end
	self.ID = tempid
	--]]
	self.ID = 10000
	if id ~= nil then
		self.ID = id
	end
	if self.ID == 10000 then
		for i =10000, 10009 do
			if ActivityCentre[i]~= nil and  ActivityCentre[i].status ~= nil and ActivityCentre[i].status > 0 and  ActivityCentre[i].status < 6 then
				self.ID = i
				break;
			end
		end
		lotteryUI.normal_diamond_index=21051
		lotteryUI.ten_diamond_index=21052
		lotteryUI.consume_coin=21050
	elseif self.ID == 10080 then
		print("id is 10080")
		for i =10080, 10090 do
			if ActivityCentre[i]~= nil and  ActivityCentre[i].status ~= nil and ActivityCentre[i].status > 0 and  ActivityCentre[i].status < 6 then
				self.ID = i
				break
			end
		end
		lotteryUI.normal_diamond_index=21061
		lotteryUI.ten_diamond_index=21062
		lotteryUI.consume_coin=21060
	else
		lotteryUI.normal_diamond_index=21051
		lotteryUI.ten_diamond_index=21052
		lotteryUI.consume_coin=21050
	end

	for _,v in pairs(ActivityCentre[self.ID]) do
		print(_..":"..v)
	end

	if ActivityCentre[self.ID].extend_2 ~= nil then

		local index = 1;
		
		lotteryUI.freetime = 0
		lotteryUI.certainGetTimes = 0
		for a, b, c, d in string.gfind(ActivityCentre[lotteryUI.ID].extend_2, "(.+)\/(.+)\/(.+)/(.+)") do
			lotteryUI.freetime = tonumber(a)
			lotteryUI.certainGetTimes = tonumber(b)
			lotteryUI.effectId = tonumber(c)
			lotteryUI.mustGetId = tonumber(d)
			if lotteryUI.effectId ~= nil then
				SkeletonAnimRcsManager:getInstance():LoadOneRoleRcsOnly(lotteryUI.effectId)
			end
		end

	end
	SkeletonAnimRcsManager:getInstance():LoadOneRoleRcsOnly(451)
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	 self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/lotteryUI.plist",self.LayerInstance, "lotteryUIPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["lotteryUI"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		--InitMoneyConfigLayer(self.LayerInstance)
	end	
	
	print("lotteryUI.ID is "..lotteryUI.ID)
	if ActivityCentre[lotteryUI.ID] == nil then
		print("ActivityCentre id is null"..lotteryUI.ID)
		return
	end

	--get starttime and endtime
	
	
	if ActivityCentre[lotteryUI.ID].startdate ~= nil then
		local startdate = tonumber(ActivityCentre[lotteryUI.ID].startdate)
		local enddate = tonumber(ActivityCentre[lotteryUI.ID].enddate)

		local startdateStr = os.date("%Y年%m月%d日%H时%M分", startdate);
		local enddateStr = os.date("%Y年%m月%d日%H时%M分", enddate);

		self.Layout:FindChildObjectByName("TEXT_TIME").__UILabel__:setString(startdateStr .. " 至\n" .. enddateStr .. "   ");
	end


	--get icon id and number
	
	local index = 1
	 for a, b, c in string.gfind(ActivityCentre[lotteryUI.ID].Award2, "(%d+)\/(%d+)\/(%d+)") do
		--local a, b, c = string.match(award, "(%d+)\/(%d+)\/(%d+)")
		----print("item id is "..a)
		local childFrameName = "map_ui_operation_FRAME_OPERATION_FRAME_REWARD"..index
		self.iconID[index]=a
		self.iconNUM[index] = b
		self.iconNumber[index] =self:formatNumber(b)--MainMenuLayer:GetNumByFormat(b)
		index = index + 1
		
	end --end for center index award
	
	self:InitLayer()
	self:consusmerLabelUpdate()
	PushOneLayer(self.LayerInstance,"","")
	addMsgCallBack("CSActivityListRsp", lotteryUI.CSActivityListRspCallBack, lotteryUI)
	
	addMsgCallBack("CSGetActivityRewardRsp", lotteryUI.CSGetActivityRewardRspCallBack, lotteryUI)
	addMsgCallBack("CSUpdateActivityInfoRsp", lotteryUI.CSUpdateActivityInfoRspCallBack, lotteryUI)
	addMsgCallBack("CSDelActivityInfoRsp", lotteryUI.CSDelActivityInfoRspCallBack, lotteryUI)
	addMsgCallBack("CSNotifyRewardToOtherRsp", lotteryUI.CSNotifyRewardToOtherRspCallBack, lotteryUI)
	
	addMsgCallBack("CSAttributesRsp", lotteryUI.CSAttributesRspCallBack, lotteryUI)
	addMsgCallBack("CSExchangeParameterListRsp", lotteryUI.CSExchangeParameterListRspCallBack, lotteryUI)
	if self.ID >= 10080 then
		self:sendCSGetUserAttributesReq();
	end
	
	self:sendCSExchangeParameterListReq(lotteryUI.normal_diamond_index,lotteryUI.ten_diamond_index,lotteryUI.consume_coin);
end

--send msg
function lotteryUI:sendCSGetUserAttributesReq()
	
	local msgname="CSAttributesReq"
	Req = {
		keys = {71} 
		
	}
	send_message(msgname, Req, true)
end


function lotteryUI:sendCSExchangeParameterListReq(a,b,c)
	local msgname="CSExchangeParameterListReq"
	Req = {
		ids = {
			 a,b,c
		} 
		
	}
	send_message(msgname, Req, true)
end

---------------------message callback---------------------
function lotteryUI:CSAttributesRspCallBack(msg) 
	print("lotteryUI Rcv CSAttributesRspCallBack msg")
	if self.LayerInstance == nil then
		return
	end
	--[[ 下面实现了精灵转盘几次必中的逻辑
	local container_name="map_ui_operation_FRAME_LOTTERY_VIP_TIME_TEXT"
	local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,container_name)
	local CCSize s = nodeFrame:getContentSize()
	local uiScale =  TXGUI.UIManager:sharedManager():getScaleFactor();
	if self.ID == 10080 and self.fairyhitnode==nil then
		self.fairyhitnode =  CCNode:create()
		--local CCSize s = nodeFrame:getContentSize()
		self.fairyhitnode:setContentSize(s)
		self.fairyhitnode:setPosition(CCPoint(s.width/2, s.height/2))
		
		self.Layout:FindChildObjectByName("lefttime").__UILabel__:getCurrentNode():addChild(self.fairyhitnode)
		
	end
		
	for _,v in ipairs(msg.values) do
		print("\tkey:" .. v.key .. "value : "..v.value)
		if v.key == 71 and self.ID == 10080 then
			self.fairyhitnode:removeAllChildrenWithCleanup(true)
			--you can get purple jingling in thr 4th play. then play more 10 times. you can get a purple jingling
			local all = 10
			if v.value < 4 then
				all = 4
			end
			local left=v.value--user play all times
			if v.value >= 4 then
				left=(v.value-4)%10
			end
			if (all - left)==1 then
				node = HtmlNode:create(s.width*2)
				node:begin()
				node:addLabel("此次必中",19,KJLinXin,ccc3(255,138,0))
				node:addLabel("紫色精灵蛋",19,KJLinXin,ccc3(142,30,206))
				node:finish()
				node:setAnchorPoint(ccp(0,1))
				self.fairyhitnode:addChild(node)
				--self.Layout:FindChildObjectByName("lefttime").__UILabel__:setString("此次必中紫色精灵蛋")
			else
				node = HtmlNode:create(s.width*2)
				node:begin()
				node:addLabel("再抽",19,KJLinXin,ccc3(255,138,0))
				node:addLabel(" ".. (all - left).." ",19,KJLinXin,ccc3(236,46,15))
				node:addLabel("次必中",19,KJLinXin,ccc3(255,138,0))
				node:addLabel("紫色精灵蛋",19,KJLinXin,ccc3(142,30,206))
				node:finish()
				node:setAnchorPoint(ccp(0,1))
				self.fairyhitnode:addChild(node)
				--self.Layout:FindChildObjectByName("lefttime").__UILabel__:setString("再抽"..(10 - left).."次必中紫色精灵蛋")
			end
	    end
	end
	--]]
	return false
end
--[[
// 通知其他玩家获取奖励情况
message CSNotifyRewardToOtherRsp { 		// TODO(lyle) 将来会调整顺序
    required bytes player_name        	= 1; 	// 活动奖励的玩家名字
	required CSReward reward 	      	= 2; 	// 获得的奖励
}

--]]
function lotteryUI:CSNotifyRewardToOtherRspCallBack(msg) 
	--print("Rcv CSNotifyRewardToOtherRspCallBack msg")
	if msg.activity_id ~= nil and msg.activity_id ~= self.ID then
		return
	end
	for _, v in pairs(msg.reward.rewards) do
			--print("get item "..v.item_id.." number "..v.item_num)
			local bingoitem = v.item_id
			local itembase= ItemManager:Get():getItemInfoById(bingoitem)

			self:InsertOneItemToContentList(msg.player_name, "获得"..itembase.m_name .. "X" .. v.item_num)
			--lotteryUI.bingoitem
		end
	return true
end
--[[
message Activity_Step {
	required uint32 step_type			= 1;	// 活动步骤类型
	required uint32 step_value1			= 2;	// 活动步骤需的id
	required uint32 step_value2			= 3;	// 活动步骤完成数量
}
message Activity_Info {
	required uint32 activity_id				= 1;	// 活动id
	repeated Activity_Step activity_steps	= 2;	// 活动步骤
	required uint32 revc_reward_times		= 3;	// 领取奖励次数
	required PB_ACTIVITY_STATUS_T status	= 4;	// 活动状态
}
message CSActivityListRsp {
	repeated Activity_Info activitys		= 1;	// 活动列表
}
--]]


function lotteryUI:CSExchangeParameterListRspCallBack(msg) 
	for _,v in pairs(msg.prices) do

		if v.id ~= nil and v.price ~= nil then
			if v.id == lotteryUI.normal_diamond_index then
				lotteryUI.normal_diamond_cost=v.price
				self.numberCostLabel:setString(lotteryUI.normal_diamond_cost);
			elseif v.id == lotteryUI.ten_diamond_index then
				lotteryUI.ten_diamond_cost=v.price
				self.Layout:FindChildObjectByName("NUMBER_COSTMANYTIME").__UILabel__:setString(lotteryUI.ten_diamond_cost);
			elseif v.id == lotteryUI.consume_coin then
				lotteryUI.consume_coin_cost=v.price
			end
			
		end
	end
end

function lotteryUI:consusmerLabelUpdate()
	if ActivityCentre[self.ID].revc_reward_times == nil then
	return
	end
	
	self.textCost1Label = self.Layout:FindChildObjectByName("TEXT_COST1").__UILabel__;
	self.textCostLabel = self.Layout:FindChildObjectByName("TEXT_COST").__UILabel__;
	self.numberCostLabel = self.Layout:FindChildObjectByName("NUMBER_COST").__UILabel__;
	self.iconCostPic = self.Layout:FindChildObjectByName("ICON_COST").__UIPicture__;
	
	self.textNumberLabel = self.Layout:FindChildObjectByName("TEXT_NUMBER").__UILabel__;
	self.Layout:FindChildObjectByName("NUM_REMAINTIME").__UILabel__:setString(ActivityCentre[self.ID].time_limit - ActivityCentre[self.ID].revc_reward_times);

	if self.certainGetTimes > 0 then

		local mustGetName = nil
		if fairys[tonumber(self.mustGetId)] ~= nil then
			mustGetName = GetLuaLocalization(fairys[tonumber(self.mustGetId)].Name)
		else
			local itembase= ItemManager:Get():getItemInfoById(self.mustGetId)
			if itembase ~= nil then
				mustGetName = itembase.m_name
			end
		end

		if mustGetName ~= nil then
			if self.certainGetTimes - ActivityCentre[self.ID].revc_reward_times % self.certainGetTimes == 1 then
				self.textNumberLabel:setString("本次必得"..mustGetName);
			else
				self.textNumberLabel:setString(string.format(GetLuaLocalization("M_TURNTABLE_GET"), self.certainGetTimes - ActivityCentre[self.ID].revc_reward_times % self.certainGetTimes - 1)..mustGetName);
			end
		end

	end

	if ActivityCentre[self.ID].revc_reward_times < lotteryUI.freetime then
		self.textCost1Label:setVisible(true);
		
		self.textCost1Label:setString(string.format(GetLuaLocalization("M_TURNTABLE_TITLE_1"), lotteryUI.freetime-ActivityCentre[self.ID].revc_reward_times))
		
		self.textCostLabel:setVisible(false);
		self.numberCostLabel:setVisible(false);
		self.iconCostPic:setVisible(false);
		
	else
		self.textCost1Label:setVisible(false);

		self.textCostLabel:setVisible(true);
		self.numberCostLabel:setVisible(true);
		self.iconCostPic:setVisible(true);
		
		--	self.Layout:FindChildObjectByName("leftNumber").__UILabel__:setString(""..(lotteryUI.playtime - ActivityCentre[self.ID].revc_reward_times))
		--end
	end
end
function lotteryUI:CSActivityListRspCallBack(msg) 
	--print("Rcv CSActivityListRspCallBack msg")
	for _,v in ipairs(msg.activitys) do
			-- v is Activity_Info
			--print("id:"..v.activity_id.."times:"..v.revc_reward_times.."status is :"..v.status)
			ActivityCentre[v.activity_id].status = v.status;
			ActivityCentre[v.activity_id].revc_reward_times = v.revc_reward_times;
			self:consusmerLabelUpdate()

	end
	checkFairyTipIcon()
	return true
end
function checkFairyTipIcon()

	print("checkFairyTipIcon")
	
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");

	local fairylotteryBtn
	if nil ~= topLayout then
		
		fairylotteryBtn = topLayout:FindChildObjectByName("button_jingling_zhuan_pan").__UIButton__
	
	else
		--print("topLayout is null")
		return;
	end
	local showButton = false;
	for i =10080, 10090 do
		if fairylotteryBtn ~= nil and ActivityCentre[i]~= nil and 
		ActivityCentre[i].status ~= nil and  ActivityCentre[i].status > 0 and ActivityCentre[i].status < 6 then
			fairylotteryBtn:setVisible(true)
			if ActivityCentre[i].revc_reward_times < 1 then
				fairylotteryBtn:showTipIcon()
			else
				fairylotteryBtn:hideTipIcon()
			end
			showButton = true
		end
	end
	if showButton == false then
		fairylotteryBtn:setVisible(false)
	else
		
	end
	
	--check normal zhan pan
	showButton = false;
	local btn =  topLayout:FindChildObjectByName("button_normal_zhuan_pan").__UIButton__
	
	for i =10000, 10009 do
		if btn ~= nil and ActivityCentre[i]~= nil and 
		ActivityCentre[i].status ~= nil and  ActivityCentre[i].status > 0 and ActivityCentre[i].status < 6 then
			btn:setVisible(true)
			if ActivityCentre[i].revc_reward_times < 1 then
				btn:showTipIcon()
			else
				btn:hideTipIcon()
			end
			showButton = true
		end
	end
	if showButton == false then
		btn:setVisible(false)
	
	end

end
--[[
message CSGetActivityRewardRsp {
	required uint32 activity_id             = 1;    // 活动id
	required bool is_succ					= 2;	// 领取是否成功
	optional CSReward rewards       		= 3; 	// 副本奖励
}

// 通知活动信息更新
message CSUpdateActivityInfoRsp {
	repeated Activity_Info activitys        = 1;    // 有更新的活动列表
}
// 通知活动信息删除
message CSDelActivityInfoRsp {
	repeated uint32 activity_ids			= 1;	// 需要删除的活动列表
}

--]]
function lotteryUI:showGetItem() 
	local showBigPrizeEff = false
	
	if self.reward_num + table.getn(lotteryUI.ListRewards) > 100 then 
		self.reward_num = 0
		self.ListContent = {}
		self.contentList:clearChildrens()
		self.contentList:setStartOffset(ccp(0,0))
	end

	for _, v in pairs(lotteryUI.ListRewards) do

		local itemName = nil
		if fairys[tonumber(v.item_id)] ~= nil then
			itemName = GetLuaLocalization(fairys[tonumber(v.item_id)].Name)
		else
			local itembase= ItemManager:Get():getItemInfoById(v.item_id)
			if itembase ~= nil then
				itemName = itembase.m_name
			end
		end

		if tonumber(self.iconID[1]) == tonumber(v.item_id) and tonumber(v.item_num) == tonumber(self.iconNUM[1]) then
			showBigPrizeEff = true
		end

		if itemName ~= nil then
			self:InsertOneItemToContentList("您", "获得"..itemName .. "X" .. v.item_num)
			GameApi:showMessage("获得"..itemName .. "X" .. v.item_num)
		end
	end
	if showBigPrizeEff then
		local particleFrame,particlenodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_BIGPRIZE")
		local particle1 = ParticleManager:Get():createEffectSpriteAndPlay(451,"", 100000, 1, false);
		self.pic_area_node:addChild(particle1,10)
		particle1:setPosition(ccpSub(particlenodePt,self.pic_area_node_pos))
		GameAudioManager:sharedManager():playEffect(320000,false);
	end
	lotteryUI.ListRewards={}
end
function lotteryUI:CSGetActivityRewardRspCallBack(msg) 
	--print("Rcv CSGetActivityRewardRspCallBack msg")
	--self:stopMove()
	
	if msg.is_succ == true then
		--GameApi:showMessage("成功领取")
		local tmp = nil
		for _, v in pairs(msg.rewards.rewards) do
			--print("get item "..v.item_id.." number "..v.item_num)
			tmp = v
			if tonumber(self.iconID[1]) == tonumber(v.item_id) and tonumber(v.item_num) == tonumber(self.iconNUM[1]) then
				self.bingoitem = v.item_id
				self.bingoitem_num = v.item_num
			end

			self.ListRewards[_]={}
			self.ListRewards[_].item_id=v.item_id
			self.ListRewards[_].item_num=v.item_num
		end
		
		if self.bingoitem == nil then
			self.bingoitem = tmp.item_id
			self.bingoitem_num = tmp.item_num
		end
		
		--lotteryUI:InsertOneItemToContentList("--bingoitem", self.bingoitem)
		--lotteryUI:InsertOneItemToContentList("--bingomovePicIndex", lotteryUI.movePicIndex)
	else
		GameApi:showMessage("领取失败")
		lotteryUI.moveSpeed = 0.04
		--[[
		if lotteryUI.stopMoveTimerFun ~= 0 then
			CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(lotteryUI.stopMoveTimerFun)
			lotteryUI.stopMoveTimerFun = 0
		end
		--]]
		lotteryUI.moveLeftTimes = 10
		--lotteryUI:InsertOneItemToContentList("stop", "领取失败")
		--self:stopMove()
	end
	return true
end
function lotteryUI:CSUpdateActivityInfoRspCallBack(msg) 
	----print("Rcv CSUpdateActivityInfoRspCallBack msg")
	self:CSActivityListRspCallBack(msg)
	return true
end
function lotteryUI:CSDelActivityInfoRspCallBack(msg) 
	--print("Rcv CSDelActivityInfoRspCallBack msg")
	--print("Rcv CSDelActivityInfoRspCallBack msg")
	for _, v in ipairs(msg.activity_ids) do
		ActivityCentre[v.activity_id].status=255--255 means delete
	end
	
	return true
end

---------------------message send out----------------------
function lotteryUI:sendCSActivityListReq()
	local msgname="CSActivityListReq"
	Req = {}
	send_message(msgname, Req, true)
end

function lotteryUI:sendCSGetActivityRewardReq(id, ...)
	local msgname="CSGetActivityRewardReq"
	Req = {
	activity_id=id,
	param={unpack(arg)},
	}
	send_message(msgname, Req, true)
	PushOneWaitingLayer("CSGetActivityRewardReq")
end
---------------------UI funciont---------------------------
--显示活动说明
function lotteryUI:setupbuttonAreaScrollist()
--[[
	local first = 2
	local last = 10
	if self.ID >= 10080 then
		first = 10
		last = 20
	end
	for i=first,last do
		if Localization['M_TURNTABLE_EXPLANATION_'..i] ~= nil then
			self:InsertOneItemToInfoList(GetLuaLocalization("M_TURNTABLE_EXPLANATION_"..i))
		end
	end
--]]

	local index = 0;
	if ActivityCentre[lotteryUI.ID].extend_1 ~= nil then
		for w, d in string.gfind(ActivityCentre[lotteryUI.ID].extend_1.."$", "(.-)(\$)") do
			if index > 1 then
				self:InsertOneItemToInfoList(w)
			end
			index = index + 1
		end
	end

	self.infoList:setStartOffset(ccp(0,0))
	
end


function lotteryUI:InsertOneItemToInfoList(str)
	if nil == self.infoList then
		return 
	end

	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	local size = CCSizeMake(self.infoList:getContentSize().width / scale,self.infoList:getContentSize().height / scale)
	local node = self:createNode(size.width, 20, str, ccc3(255, 227, 182))
	self:scrolllistAddNode(self.infoList, node)
end

function lotteryUI:InsertOneItemToContentList(who, str)
	if nil == self.contentList then
		return 
	end

	self.reward_num = self.reward_num+1
	if self.reward_num > 100 then 
		self.reward_num = 1
		self.ListContent = {}
		self.contentList:clearChildrens()
		self.contentList:setStartOffset(ccp(0,0))
	end

	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	local size = CCSizeMake(self.contentList:getContentSize().width / scale,self.contentList:getContentSize().height / scale)

	local node = self:createNode2Color(size.width, 20, who, str, ccc3(254, 174, 49), ccc3(255, 227, 182))
	self:scrolllistAddNodeIntoFirst(self.contentList, node)

end

function lotteryUI:createNode(width, fontSize, str, fontColor)

		node = HtmlNode:create(width)
		node:begin()
		node:addLabel(str,fontSize,KJLinXin,fontColor)
		node:finish()
		node:setAnchorPoint(ccp(0,0.5))
		return node

end

function lotteryUI:createNode2Color(width, fontSize, str1, str2, fontColor1, fontColor2)

		node = HtmlNode:create(width)
		node:begin()
		node:addLabel(str1.." ",fontSize,KJLinXin,fontColor1)
		node:addLabel(str2,fontSize,KJLinXin,fontColor2)
		node:finish()
		node:setAnchorPoint(ccp(0,0.5))
		return node

end

function lotteryUI:scrolllistAddNode(list, node)
	local base = list:AddBaseItem(node:getContentSize(),node)
	local oldPos_y = node:getPositionY()
	node:setPosition((0 + list:getContentSize().width ) * 0.5,oldPos_y)

	local size = list:getContentSize()
	local viewSize = list:getViewSize()
	if size.height > viewSize.height then
		local offset = ccp(0,size.height - viewSize.height)
		list:setStartOffset(offset)
	end
end


function lotteryUI:scrolllistAddNodeIntoFirst(list, node)

	local base = list:AddBaseItem(node:getContentSize(),node)
	local oldPos_y = node:getPositionY()
	if self.reward_num > 1 then
		local a = self.ListContent[1]:getPositionY() - self.ListContent[1]:getContentSize().height * 0.5 - node:getPositionY() - node:getContentSize().height * 0.5
		oldPos_y = self.ListContent[self.reward_num - 1]:getPositionY() + self.ListContent[self.reward_num - 1]:getContentSize().height * 0.5 - node:getContentSize().height * 0.5
		for i=1,self.reward_num - 1 do
			self.ListContent[i]:setPosition(self.ListContent[i]:getPositionX(), self.ListContent[i]:getPositionY()-node:getContentSize().height - a) 
		end
	end

	self.ListContent[self.reward_num] = node
	
	node:setPosition((0 + list:getContentSize().width ) * 0.5,oldPos_y)
end

--tip按钮
function lotteryUIonPressIconButton(nTag)
   --print("onPressTipBoxButton:"..nTag)
   local id = nTag
   local item_id = lotteryUI.ListTipId[id]
   local iconbtn = lotteryUI.ListTipBtn[id]
   local menu = iconbtn:getMenuNode()
   local pos = menu:getTouchPoint()
   ItemManager:Get():showItemTipsById(item_id, pos)
end

function lotteryUI:showPicArea()
	local container_name="map_ui_operation_FRAME_LOTTERY_PIC_AREA"
	local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,container_name)
	self.pic_area_node_pos=nodePt
	if self.pic_area_node==nil then
		self.pic_area_node =  CCNode:create()
		local CCSize s = nodeFrame:getContentSize()
		self.pic_area_node:setContentSize(s)
		self.pic_area_node:setPosition(CCPoint(s.width/2, s.height/2))
		
		self.Layout:FindChildObjectByName("ItemContainer").__UIPicture__:getCurrentNode():addChild(self.pic_area_node)
		
	end
	local particleFrame,particlenodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_LIGHT_LEFT")
	local particle1 = ParticleManager:Get():createEffectSpriteAndPlay(431,"", 100000, 1, true);
	self.pic_area_node:addChild(particle1)
	   --iconpiture:getCurrentNode():addChild(boxBtn)
	particle1:setPosition(ccpSub(particlenodePt,nodePt))
	particleFrame,particlenodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_LIGHT_RIGHT")
	particle1 = ParticleManager:Get():createEffectSpriteAndPlay(431,"", 100000, 1, true);
	self.pic_area_node:addChild(particle1)
	   --iconpiture:getCurrentNode():addChild(boxBtn)
	particle1:setPosition(ccpSub(particlenodePt,nodePt))
	

	local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_operation.bin", "map_ui_operation_FRAME_LOTTERY_ICON_REWARD1")
	local pEmptyItem = CCSprite:create()
	pEmptyItem:setContentSize(labelFrame:getContentSize())
	   
	boxBtn = IconButton:new(pEmptyItem,nil,nil)
	   
	local btnMenuItem = boxBtn:getMenuItem()
	btnMenuItem:setTag(1)
	btnMenuItem:registerScriptTapHandler(lotteryUIonPressIconButton)
	   
	self.pic_area_node:addChild(boxBtn,3)
	boxBtn:setPosition(ccpSub(labelPt,nodePt))
	self.ListTipId[1] = self.iconID[1]
	self.ListTipBtn[1] = boxBtn


	--create 2~10 item icon
	for i=2, 10 do
	
		local iconpiture = nil
	  
		--iconpiture = self.LayoutEnter:FindChildObjectByName("map_ui_operation_FRAME_LOTTERY_ICON_REWARD"..i).__UIPicture__;
	  
		-- local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_ICON_REWARD"..i)
		local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_GRID_REWARD"..i)
		self.pic_area_node:addChild(labelFrame,2)
		labelFrame:setPosition(ccpSub(labelPt,nodePt))

		-- local spriteSize = iconpiture:getSpriteSize()
		self.iconPos[i] = CCPoint(labelPt.x, labelPt.y)
		local boxLockSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui_operation.bin", "map_ui_operation_FRAME_LOTTERY_ICON_CLICKED")

		local pEmptyItem = CCSprite:create()
		pEmptyItem:setContentSize(labelFrame:getContentSize())
	   
		boxBtn = IconButton:new(pEmptyItem,nil,boxLockSpriteOff)
	   
		local btnMenuItem = boxBtn:getMenuItem()
		btnMenuItem:setTag(i)
		btnMenuItem:registerScriptTapHandler(lotteryUIonPressIconButton)
	   
	   
		self.pic_area_node:addChild(boxBtn,3)

		boxBtn:setPosition(ccpSub(labelPt,nodePt))
		local item_id = 411306
		if self.iconID[i] ~= nil then
			item_id = self.iconID[i]
		end
		local sprite = ItemManager:Get():getIconSpriteById(item_id)
		--add blue or green kuang
		-- local itemInfo = ItemManager:Get():getItemInfoById(id);
		local itemInfo = ItemManager:Get():getItemInfoById(item_id);
--[[ icon has frame jackniu delete this
		if nil ~= itemInfo then
			if itemInfo.m_quality > 0 then
				local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality);
				if nil ~= framePic then
					boxBtn:setFrameSprite(framePic, ccp(0,0));
					
				end
			end
		end
--]]
		boxBtn:SetNormalIcon(sprite);
	   
	   	--add item number
		local	childFrameName = "map_ui_operation_FRAME_LOTTERY_NUMBER_REWARD"..i
		local	iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,childFrameName)  
		local nameLabel = TXGUI.UILabelTTF:create(""..self.iconNumber[i],KJLinXin,16)
		nameLabel:setPosition(ccpSub(iconPt,nodePt))
		self.pic_area_node:addChild(nameLabel,4);

		self.ListTipId[i] = item_id
		self.ListTipBtn[i] = boxBtn
	end --end for create 2~10 icon button
   
	
	if lotteryUI.effectId ~= nil then

		--local sa = self.Layout:FindChildObjectByName("SUPERAWARD_ANIMATION").__UIPicture__;

		particleFrame,particlenodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_SUPERAWARD_ANIMATION")
		particle1 = ParticleManager:Get():createEffectSpriteAndPlay(lotteryUI.effectId,"", 100000, 1, true);
		self.pic_area_node:addChild(particle1)
		particle1:setPosition(ccpSub(particlenodePt,nodePt))
	end


end

-- 移动位置
local function lotteryUIupdateiconpos(event)
		--[[if userInfo ~= nil and userInfo.total_hp ~= 0 then
			lotteryUI.getUserInfoFlag = true
			CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(lotteryUI.updatePicPos)
			lotteryUI.updatePicPos = 0
			lotteryUI:setUserInfo()
		end
		--]]
	
--[[
	if lotteryUI.bingoindex ~= nil then 
		--slow down the move
		--print("lotteryUI.bingoindex ~= nil")
		lotteryUI.inner_loop=lotteryUI.inner_loop+1
		if lotteryUI.inner_loop <= lotteryUI.outer_loop then
			return
		else
			lotteryUI.inner_loop = 0
			lotteryUI.outer_loop=lotteryUI.outer_loop+lotteryUI.outer_deta
		end
		if lotteryUI.bingoindex == lotteryUI.movePicIndex then
			lotteryUI.bingoindex = nil
			
			lotteryUI:stopMove(lotteryUI.bingoindex)
			return
		end
	end
--]]
	----print("movePicIndex"..lotteryUI.movePicIndex)
	--[[if lotteryUI.bingoitem ~= nil then
	
	--print("lotteryUI.bingoitem :"..lotteryUI.bingoitem)
	end
	--print("lotteryUI.bingoitem == lotteryUI.iconID"..lotteryUI.movePicIndex..":"..lotteryUI.iconID[lotteryUI.movePicIndex] )
	--]]
	if lotteryUI.updatePicPos ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(lotteryUI.updatePicPos)
		lotteryUI.updatePicPos = 0

		lotteryUI.moveLeftTimes = lotteryUI.moveLeftTimes - 1
		lotteryUI.movePicIndex = lotteryUI.movePicIndex + 1
		if lotteryUI.movePicIndex >10 then
			lotteryUI.movePicIndex=1
		end
	
		GameAudioManager:sharedManager():playEffect(350016,false);
		if lotteryUI.movePicIndex == 1 then
			lotteryUI.superGridPic:setVisible(true)
			lotteryUI.movePic:setVisible(false)
		else
			lotteryUI.movePic:setVisible(true)
			lotteryUI.superGridPic:setVisible(false)
			lotteryUI.movePic:setPosition(ccpSub(lotteryUI.iconPos[lotteryUI.movePicIndex],lotteryUI.pic_area_node_pos))
		end
	
		if lotteryUI.moveSpeed == nil then
			lotteryUI.moveSpeed = 0.4
		end
	
		--lotteryUI:InsertOneItemToContentList(lotteryUI.movePicIndex, lotteryUI.iconID[lotteryUI.movePicIndex].."|"..lotteryUI.moveLeftTimes.."|"..lotteryUI.moveSpeed)

		--if lotteryUI.moveSpeed <= 0.04 and lotteryUI.movePicIndex == 5 then
		if lotteryUI.moveLeftTimes <70 and lotteryUI.bingoitem ~= nil and tonumber(lotteryUI.bingoitem) == tonumber(lotteryUI.iconID[lotteryUI.movePicIndex]) 
			and  tonumber(lotteryUI.bingoitem_num) == tonumber(lotteryUI.iconNUM[lotteryUI.movePicIndex]) then
			if lotteryUI.moveLeftTimes <= 40 and lotteryUI.moveLeftTimes > 10 then
				lotteryUI.moveSpeed = 0.04
				--[[
				if lotteryUI.stopMoveTimerFun ~= 0 then
					CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(lotteryUI.stopMoveTimerFun)
					lotteryUI.stopMoveTimerFun = 0
				end
				--]]
				lotteryUI.moveLeftTimes = 10

				--lotteryUI:InsertOneItemToContentList("bingoitem", lotteryUI.bingoitem)
			end
		end

		if lotteryUI.moveLeftTimes == 0 then
			--lotteryUI:InsertOneItemToContentList("stop", lotteryUI.bingoitem)
			lotteryUI:stopMove(lotteryUI.bingoitem)
			return
		elseif lotteryUI.moveLeftTimes >= 70 then 
			lotteryUI.moveSpeed = lotteryUI.moveSpeed - 0.04 
		elseif lotteryUI.moveLeftTimes <= 10 then
			lotteryUI.moveSpeed = lotteryUI.moveSpeed + 0.04 
		end

		lotteryUI.updatePicPos = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(lotteryUIupdateiconpos, lotteryUI.moveSpeed, false)
	end
end
local function stopMoveTimer()
	if lotteryUI.stopMoveTimerFun ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(lotteryUI.stopMoveTimerFun)
		lotteryUI.stopMoveTimerFun = 0
	end
	lotteryUI.moveLeftTimes = 10
	--lotteryUI:InsertOneItemToContentList("stop", "stopMoveTimer")
end

function lotteryUI:startMove()
	--[[self.outer_loop=5
	self.outer_deta=2
	self.inner_loop=0
	--]]
	--self.movePicIndex=math.random(10)
	--[[if self.movePic == nil then
		--print("new self.movePic")
		local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_GRID_SELECTED")
		self.movePic=labelFrame
		self.pic_area_node:addChild(self.movePic, 5)
		--self.movePic=self.Layout:FindChildObjectByName("GRID_SELECTED").__UIPicture__:getCurrentNode()
	end--]]
	--print("self.movePic:setPosition")
	--self.movePic:setPosition(ccpSub(self.iconPos[self.movePicIndex],self.pic_area_node_pos))
	--self.movePic:setVisible(true)
	--print("self.movePic:setPosition2")
	if lotteryUI.updatePicPos == 0 then
		lotteryUI.moveSpeed = 0.4
		lotteryUI.bingoitem=nil
		lotteryUI.stopMoveFlag = nil
		lotteryUI.moveLeftTimes = 80
		--print("lotteryUI:startMove start timer")
        --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 11")
		lotteryUI.updatePicPos = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(lotteryUIupdateiconpos, lotteryUI.moveSpeed, false)
        --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 12")
		--lotteryUI.stopMoveTimerFun = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(stopMoveTimer, 5, false)
		
	end
end

function lotteryUI:stopMove(itemid)
	if lotteryUI.updatePicPos ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(lotteryUI.updatePicPos)
		lotteryUI.updatePicPos = 0
	end
	if itemid ~= nil then
		if self.movePic ~= nil then
			if self.movePicIndex == 1 then

			else
				local blink = CCBlink:create(2, 4)
				self.movePic:runAction(blink)
			end
			self:showGetItem()
		end
	else
		--[[if self.movePic ~= nil then
			self.movePic:setVisible(false)
		end
		--]]
	end
end

function lotteryUI:InitLayer()
	
	
	self.closeBtn = self.Layout:FindChildObjectByName("BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
	self.closeBtn:registerScriptTapHandler(lotteryUICloseBtnClick);
	local bgIconPath = "UI/ui_pic_lottery.png"
	local lotterypic = CCSprite:create(bgIconPath)
	if lotterypic ~= nil then
		self.Layout:FindChildObjectByName("PIC_AREA").__UIPicture__:setSprite(lotterypic)
	end
	self:showPicArea()
	
	self.BUTTON_PLAY = self.Layout:FindChildObjectByName("BUTTON_PLAY").__UIButton__:getMenuItemSprite();
	self.BUTTON_PLAY:registerScriptTapHandler(function(pobj)
		self:buttonAction()
		--self:InsertOneItemToContentList("1", "1111111111111111111111111")
	end);

	self.BUTTON_PLAYMANYTIME = self.Layout:FindChildObjectByName("BUTTON_PLAYMANYTIME").__UIButton__:getMenuItemSprite();
	self.BUTTON_PLAYMANYTIME:registerScriptTapHandler(function(pobj)
		self:buttonAction(10)
		--self:InsertOneItemToContentList("1", "2")
	end);
	
	--Init movePic
	self.movePicIndex=math.random(10)
	self.movePic=AspriteManager:getInstance():getOneFrame(KUI_OPERAION_BIN,"map_ui_operation_FRAME_LOTTERY_GRID_SELECTED")
	self.superGridPic=self.Layout:FindChildObjectByName("GRID_REWARD1_1").__UIPicture__;
	self.pic_area_node:addChild(self.movePic, 5)
	--self.pic_area_node:addChild(self.superGridPic, 5)
	self.movePic:setVisible(false)
	self.superGridPic:setVisible(false)
	--self.movePic:setPosition(ccpSub(self.iconPos[self.movePicIndex],self.pic_area_node_pos))
	
	self.contentList = self.Layout:FindChildObjectByName("BUTTONAREA_LIST").__UIScrollList__
    self.infoList = self.Layout:FindChildObjectByName("info_list").__UIScrollList__
	self.ListContent = {}
	self.reward_num = 0;

	self:setupbuttonAreaScrollist()		
	--self.iconContentLabel = self.Layout:FindChildObjectByName("coinlabel").__UILabel__;
	self.diamondContentLabel = self.Layout:FindChildObjectByName("NUMBER_DIAMOND_INALL").__UILabel__;		
	self:ShowMoney()
	self:ShowDiamond()
	--addMsgCallBack("CSAttributesRsp", lotteryUI.CSAttributesRspCallBack, lotteryUI)
	
	--addMsgCallBack("CSEquipmentComposeRsp", lotteryUI.CSEquipmentComposeRspCallBack, lotteryUI)
	--self:sendCSGetUserAttributesReq()
	
end
------change lebel value----

function lotteryUIDiamondLayerOkBtnClick()
lotteryUI:startMove()
lotteryUI:sendCSGetActivityRewardReq(lotteryUI.ID, 10)

end

function lotteryUICoinLayerOkBtnClick()
lotteryUI:startMove()
lotteryUI:sendCSGetActivityRewardReq(lotteryUI.ID)
end

function lotteryUIDiamondLayercancelBtnClick()
end
function lotteryUI:buttonAction(id)
	
	if lotteryUI.updatePicPos ~= 0 then --if we didn't rcv msg rsp, disable the button move
		return
	end
	if id == 10 and lotteryUI.ten_diamond_cost ~= nil then
		if ActivityCentre[self.ID].revc_reward_times ~= nil and ActivityCentre[self.ID].revc_reward_times +10 > ActivityCentre[self.ID].time_limit then
			GameApi:showMessage(GetLuaLocalization('M_LOTTERY_VIP_TIME_2'))
			return
		end
		if ActivityCentre[self.ID].revc_reward_times < lotteryUI.freetime then
			GameApi:showMessage("今日免费转动次数未用完，无法进行十连抽")
			return
		end
		
		--print("DailyTaskLayerCompelteTaskRightNowBtnClick")
		
        --[[
		local lotteryRemindCost = CCUserDefault:sharedUserDefault():getBoolForKey("lotteryRemindCost")
        if lotteryRemindCost then
            lotteryUIDiamondLayerOkBtnClick()
        else
            DiamondWarningLayer:Show("十连抽需要消耗",diamondNum,lotteryUIDiamondLayerOkBtnClick,lotteryUIDiamondLayercancelBtnClick,ccc3(255,255,255),"","")
            CCUserDefault:sharedUserDefault():setBoolForKey("lotteryRemindCost", true)
            CCUserDefault:sharedUserDefault():flush()
        end
        ]]
		
		local diamondNum = lotteryUI.ten_diamond_cost
		--GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);
		if UserData:GetUserInfo().m_diamond < diamondNum then
			GameApi:showMessage("您的钻石数不足")
			return
		end
        DiamondWarningLayer:Show("十连抽需要消耗",diamondNum,lotteryUIDiamondLayerOkBtnClick,lotteryUIDiamondLayercancelBtnClick,ccc3(255,255,255),"","")
		
	elseif lotteryUI.normal_diamond_cost ~= nil then
		if ActivityCentre[self.ID].revc_reward_times ~= nil and ActivityCentre[self.ID].revc_reward_times +1 > ActivityCentre[self.ID].time_limit then
			GameApi:showMessage(GetLuaLocalization('M_LOTTERY_VIP_TIME_2'))
			return
		else
			if ActivityCentre[self.ID].revc_reward_times >= self.freetime then
				if UserData:GetUserInfo().m_diamond < lotteryUI.normal_diamond_cost then
					GameApi:showMessage("您的钻石数不足")
					return
				end
				if self.showTipForOnce == true then
					self.showTipForOnce = false
					local diamondNum = lotteryUI.normal_diamond_cost
					DiamondWarningLayer:Show("抽奖一次需要消耗",diamondNum,lotteryUICoinLayerOkBtnClick,lotteryUIDiamondLayercancelBtnClick,ccc3(255,255,255),"","")
					return
				end
			end 
		end
		
		self:sendCSGetActivityRewardReq(lotteryUI.ID)
		self:startMove()
	end
	
end


function lotteryUI:updateExpAll()

	self.NUM_ALLEXP:setString("" .. MainMenuLayer:GetNumByFormat(self.all_exp))
end 
-------------------message call back-----------------------------------------------





-------------------button callback------------------------------------------------

 function lotteryUICloseBtnClick(pObj)
 
   TXGUI.UIManager:sharedManager():removeUILayout("lotteryUIPanel")
   --TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
   SetChatBar(true,-1)
   CCTextureCache:sharedTextureCache():removeUnusedTextures()

   RemoveOneLayer(lotteryUI.LayerInstance)
   --NotificationCenter:defaultCenter():unregisterScriptObserver(lotteryUI.closeCallBackFun)

   lotteryUI.LayerInstance:removeFromParentAndCleanup(true);
   lotteryUI.LayerInstance = nil
   lotteryUI:ResetValue()
end


--------------notification-----
function lotteryUI:ShowMoney()
	--[[
	if nil ~= self.iconContentLabel then
		local money = UserData:GetUserInfo().m_gold
		local moneyContent = MainMenuLayer:GetNumByFormat(money)
		self.iconContentLabel:setString("" .. moneyContent)
	end
	--]]
end



function lotteryUI:ShowDiamond()
	if nil ~= self.diamondContentLabel then
		local diamond = UserData:GetUserInfo().m_diamond
		--local diamondContent = MainMenuLayer:GetNumByFormat(diamond)
		self.diamondContentLabel:setString("" .. diamond)
	end
end

local function updateNotification(message)
	if G_CurLayerInstance ~= lotteryUI.LayerInstance then
		return
	end
	if message == GM_ATTR_COIN_UPDATE then
		lotteryUI:ShowMoney()
	elseif message == GM_ATTR_SP_DIAMOND_UPDATE then
		lotteryUI:ShowDiamond()
	end
	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(closeLayerCallBack);
