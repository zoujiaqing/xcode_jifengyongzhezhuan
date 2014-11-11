require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameterControl")
require("Script/MainMenuUILayer")
require("Script/MainMenuUILayer")
--require("Script/UILayer/ActivityCentre")
--require("Script/UILayer/ShowActivity")
require("Script/UILayer/lotteryUI")
require("Script/GameConfig/InstanceList")
require("Script/UILayer/dailyRewardStaminaLayer")
require("Script/UILayer/PayTop")
require("Script/GameConfig/Item")
require("Script/GameConfig/EquipmentItem")
require("Script/SpriteElf/fairybasicinfo")
require("Script/Fairy/FairyDataCenter")
require("Script/UILayer/showQueryResult")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/Fairy/FairyLuaInterface")

KUI_OPERAION_BIN="UI/ui_operation.bin"

local inited = false


local active_id = 0;
local active_id_dcount = 0;

operationUI = {
	isSendReq =false,
	LayerInstance = nil,
	Layout = nil,

	TEXT_TITLE=nil,
	contentList2=nil,
	growupTitleContainer=nil,
	closeBtn= nil,

	BUTTON_BUY= nil,
	ListIconId = {},
	ListIconBtn = {},
	ListDuiHuanData={},
	--day_recharge_active = {},
}

function operationUI:clearData()
	for i, v in pairs(ActivityCentre) do

		v.status=nil
	end
	isSendReq=false
end

function operationUI:ResetValue()

	self.LayerInstance = nil
	self.Layout = nil

	self.TEXT_TITLE=nil
	self.lastSelectIndex = 1
	self.tempnode=nil
	self.closeBtn= nil
	for i, v in pairs(ActivityCentre) do
		if v.button_label ~= nil then
			v.button_label = nil
		end
		if v.button_item ~= nil then
			v.button_item = nil
		end
		if v.info_label ~= nil then
			v.info_label = nil
		end
		--v.status=nil
	end
	ListDuiHuanData={}
	removeMsgCallBack("CSActivityListRsp", operationUI.CSActivityListRspCallBack)
	removeMsgCallBack("CSGetActivityRewardRsp", operationUI.CSGetActivityRewardRspCallBack)
	removeMsgCallBack("CSUpdateActivityInfoRsp", operationUI.CSUpdateActivityInfoRspCallBack)
	removeMsgCallBack("CSDelActivityInfoRsp", operationUI.CSDelActivityInfoRspCallBack)
	--removeMsgCallBack("CSGetActivityDataRsp", operationUI.CSGetActivityDataRspCallBack)

	removeMsgCallBack("CSQueryRankRsp", operationUI.CSQueryRankRspCallBack)		--注销接收整个排行消息.
	removeMsgCallBack("CSQueryPlayerRankInfoRsp", operationUI.CSQueryPlayerRankInfoRspCallBack)		--注销网络消息接收.
end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= operationUI.LayerInstance then
			if G_CurLayerInstance ~= operationUI.LayerInstance then
				operationUICloseBtnClick(nil)
			end
		end
	end
end
-- get instance id by dropped matarials
function operationUI:searchInstanceIdByMat(mat)
	local instanceId = 0
	for k,v in pairs(instances) do
		if (v.Type == "1") and (v.RaidDrops1 ~= nil) then
			local dropStr = v.RaidDrops1
			if dropStr ~= nil then
				local endPos = string.find(dropStr,"/")
				if endPos ~= nil then
					local drop = string.sub(dropStr,1,endPos - 1)
					local dropId = tonumber(drop)
					if dropId == mat then
						if instanceId == 0 then
							instanceId = k
						else
							-- 取编号最小的副本
							if k < instanceId then
								instanceId = k
							end
						end
					end
				end
			end
		end
	end
	return instanceId
end

local function onReceiveDestructorHandler()
    TXGUI.UIManager:sharedManager():removeUILayout("operationUIPanel")
	operationUI:ResetValue()
	inited = false
	CCLuaLog("----set inited = false-----")

	local addFriendUI = getUILayerByTag(UITagTable["ShowQueryResultUILayer"].tag,false)
	if addFriendUI ~= nil then
		addFriendUI:removeFromParentAndCleanup(true)
	end
end

function operationUI:CreateLayer(parentLayer)
	if inited then
		CCLuaLog("----inited = true return-----")
		return
	end
	inited = true
	CCLuaLog("----set inited = true-----")

	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/operationUI.plist",self.LayerInstance, "operationUIPanel", true);
	self.Layout = uiLayout

	if uiLayout == nil then
		inited = false
		CCLuaLog("----set inited = false-2----")
		return
	end

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["operationUI"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		--InitMoneyConfigLayer(self.LayerInstance)
	end

	self:InitLayer()
	
	PushOneLayer(self.LayerInstance,"","")

	addMsgCallBack("CSActivityListRsp", operationUI.CSActivityListRspCallBack, operationUI)

	addMsgCallBack("CSGetActivityRewardRsp", operationUI.CSGetActivityRewardRspCallBack, operationUI)
	addMsgCallBack("CSUpdateActivityInfoRsp", operationUI.CSUpdateActivityInfoRspCallBack, operationUI)
	addMsgCallBack("CSDelActivityInfoRsp", operationUI.CSDelActivityInfoRspCallBack, operationUI)

	addMsgCallBack("CSQueryRankRsp", operationUI.CSQueryRankRspCallBack, operationUI);		--监听充值钻石所有玩家的排行消息返回.
	addMsgCallBack("CSQueryPlayerRankInfoRsp", operationUI.CSQueryPlayerRankInfoRspCallBack, operationUI);		--监听个人排行消息返回.

    sendCSActivityListReq()

	--self:sendCSQueryPlayerRankInfoReq_paydiamond();			--查询自己的排名信息.
	--SetChatBar(true,-1)
end
--when game start send a request

---------------------message callback----------------------
--[[
message Activity_Step {
	required uint32 step_type			= 1;	// 活动步骤类型

}
message Activity_Info {
	required uint32 activity_id				= 1;	// 活动id
	repeated Activity_Step activity_steps	= 2;	// 活动步骤
	required uint32 revc_reward_times		= 3;	// 领取奖励次数
	required PB_ACTIVITY_STATUS_T status	= 4;	// 活动状态
}
// 活动相关协议------------------------------------------------------------------------------------
enum PB_ACTIVITY_STATUS_T {
    PB_ACTIVITY_STATUS_UNKNOW          = 0,    // 活动未知状态
    PB_ACTIVITY_STATUS_NOT_START       = 1,    // 活动未开始
    PB_ACTIVITY_STATUS_UNFINISH        = 2,    // 活动未完成状态
    PB_ACTIVITY_STATUS_FINISH          = 3,    // 活动完成状态
    PB_ACTIVITY_STATUS_REWARD_REVC     = 4,    // 活动奖励已经领取
    PB_ACTIVITY_STATUS_END             = 5,    // 活动奖励结束
    PB_ACTIVITY_STATUS_INVALID         = 6,    // 活动领取奖励时间失效
}

message CSActivityListRsp {
	repeated Activity_Info activitys		= 1;	// 活动列表
}
--]]

function operationUI:checkAddTipIcon()
	--show tip button
	--------print("checkAddTipIcon")
	local showtip=false
	local ActivityCentreIndex

	for i, v in pairs(ShowActivity) do
		if v.Activity_Content == nil then
			----print("Erorr: Activity_Content is null for showactiviyt id"..i)
			return
		end
		for w, d in string.gfind(v.Activity_Content, "(.-)(\$)") do
			if string.find(w,"#", 1) ~= nil then
				for center_index in string.gfind(w, "#(.-)#") do
					----print("activity center index is " .. center_index)
					ActivityCentreIndex=tonumber(center_index)
					v = ActivityCentre[ActivityCentreIndex]
					----print("ActivityCentre".."status"..ActivityCentreIndex)
					if v~=nil and  v.status ~= nil and (v.status) == PB_ACTIVITY_STATUS_FINISH then
						showtip = true
						--------print("-----------------------------show tip bingo")
						break
					end
				end
			end
		end
	end
	--[[for i,v in pairs(ActivityCentre) do
		--------print("ActivityCentre".."status"..i)
		if  v.status ~= nil and (v.status) == PB_ACTIVITY_STATUS_FINISH then
			showtip = true
			--------print("-----------------------------show tip bingo")
			break
		end
	end
--]]
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local operationBtn
	--local fairylotteryBtn
	if nil ~= topLayout then
		operationBtn = topLayout:FindChildObjectByName("operationBtn").__UIButton__
		--fairylotteryBtn = topLayout:FindChildObjectByName("button_fairyring").__UIButton__
		if nil == operationBtn then
			--------print("operationBtn is null")
			return;
		end
	else
		--------print("topLayout is null")
		return;
	end
	--------print("show tip is ")

	if showtip then
		--------print("true")
		operationBtn:showTipIcon()
	else
		--------print("false")
		operationBtn:hideTipIcon()
	end
	--fairy button add tip icon
	checkFairyTipIcon();
	
end
--[[
// 活动详细信息数据请求
message CSGetActivityDataReq {
	
}

message CSActivityCentreItemInfo {
	required uint32 index 					= 1;	//下标索引，比如现在第一个是10410
	optional bytes activity_type			= 2;
	optional bytes activity_show			= 3;
	optional bytes activity_qualification	= 4;
	optional bytes activity_qualification2	= 5;
	optional bytes award1					= 6;
	optional bytes award2					= 7;
	optional uint32 startdate				= 8;	// 活动起始时间
	optional uint32 enddate					= 9;	// 活动截止时间
	optional uint32 starttime				= 10;	// 活动开始时间
	optional uint32 endtime					= 11;	// 活动结束时间
	optional uint32 specialtm				= 12;	// 活动特殊时间
	optional uint32 rewarddate				= 13;	// 活动领奖截止时间
}

// 单个活动信息
message CSShowActivityItemInfo {
	required bytes activity_show    			= 1;	// 活动显示ID
	optional bytes activity_condition    		= 2;	// 活动状态显示
	optional bytes activity_name				= 3;	// 活动名称
	optional bytes font_color 					= 4;	// 字体颜色
	optional bytes buttonid						= 5;	// 按钮类型
	optional bytes buttonid_text				= 6;	// 按钮的文本
	optional bytes openlayername				= 7;	// 弹出界面ID
	optional bytes activity_content				= 8;	// 活动内容
	optional bytes itemflag						= 9;	// 兑换活动标志位
	
}
message CSGetActivityDataRsp {
	repeated CSShowActivityItemInfo show_info			= 1; // show activity活动列表
	repeated CSActivityCentreItemInfo center_info		= 2; //activity center info
}

--]]
function operationUI:CSGetActivityDataRspCallBack(msg)
	ShowActivity={}
	ActivityCentre={}
	local index=1
	----print("CSGetActivityDataRspCallBack")
	for _,v in ipairs(msg.show_info) do
		ShowActivity[index] = {}
		local tmp = ShowActivity[index] 
		tmp.Activity_Show = v.activity_show
		tmp.Activity_Condition = v.activity_condition
		tmp.Activity_Name = v.activity_name
		tmp.Font_Color = v.font_color
		tmp.ButtonID = v.buttonid
		tmp.ButtonID_Text = v.buttonid_text
		tmp.OpenLayerName = v.openlayername
		tmp.Activity_Content = v.activity_content
		tmp.ItemFlag = v.itemflag
		--print("activity_show:"..ShowActivity[index].Activity_Show..",ItemFlag"..ShowActivity[index].ItemFlag)
		index  = index + 1
		
	end
	for _,v in ipairs(msg.center_info) do
		ActivityCentre[v.index] = {}
		local tmp = ActivityCentre[v.index]
		tmp.Activity_Type = v.activity_type
		
		tmp.Activity_Show = v.activity_show
		tmp.Activity_Qualification = v.activity_qualification
		tmp.Activity_Qualification2 = v.activity_qualification2
		tmp.Award1 = v.award1
		tmp.Award2 = v.award2
		tmp.extend_1 = v.extend_1
		tmp.extend_2 = v.extend_2
		tmp.startdate =  v.startdate or 1111
		tmp.enddate =  v.enddate or 1111
		tmp.starttime =  v.starttime or 1111
		tmp.endtime =  v.endtime or 1111

		--print(v.index..":enddate:"..tmp.enddate..",startdate:"..v.startdate)
	end
	
end

function operationUI:CSActivityListRspCallBack(msg)
	self:CSUpdateActivityInfoRspCallBack(msg)

	return true
end

function operationUI:activityStateChange(id, state)
									
	if ActivityCentre[id].status ~= nil and ActivityCentre[id].status == state then
		return
	end
	
	ActivityCentre[id].status = state

	if ActivityCentre[id] ~= nil then
		if  ActivityCentre[id].status ~= nil and ActivityCentre[id].status ~= PB_ACTIVITY_STATUS_FINISH then
			local label = ActivityCentre[id].button_label
			local infolabel = ActivityCentre[id].info_label
			if label ~= nil and  ActivityCentre[id].status == PB_ACTIVITY_STATUS_REWARD_REVC then
				label:setString("已领取")

				if infolabel ~= nil and ActivityCentre[id].infoStrList ~= nil and ActivityCentre[id].infoStrList[2] ~= nil then
					infolabel:setString(ActivityCentre[id].infoStrList[2]);
				end
			end

			if ActivityCentre[id].button_item ~= nil then
				if id == 10420 then 
					ActivityCentre[id].button_item:setEnabled(false) --grow up buy button is a uibutton. others are uimenu
					return
				end
				local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,"map_ui_operation_FRAME_OPERATION_BUTTON")

				local diableframe = graylightWithCCSprite(btFrame, false)
				local diableframe2 = graylightWithCCSprite(btFrame, false)

				--local menuItem = CCMenuItemSprite:create(diableframe,diableframe)
				ActivityCentre[id].button_item:setNormalImage(diableframe)
				ActivityCentre[id].button_item:setSelectedImage(diableframe2)
				ActivityCentre[id].button_item:setEnabled(false)
				--uimenu:addChild(menuItem)
			end
		end
	end
	--check if show tip icon
	local showActivityIndex = tonumber(ActivityCentre[id].Activity_Show)
	if self.listIndex2ActivityId == nil then
		return
	end
	for i, v in ipairs(self.listIndex2ActivityId) do
		if v == showActivityIndex then
			------print("match button "..i)
			if self:activity_Center_have_valid_item(showActivityIndex) then 
				self.ListButtons[i]:showTipIcon()
			else
				self.ListButtons[i]:hideTipIcon()
			end
		end
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
function operationUI:CSGetActivityRewardRspCallBack(msg)
	print("Rcv CSGetActivityRewardRspCallBack msg")
	print("activity_id"..msg.activity_id)
	if msg.is_succ == true then
		if msg.activity_id == 10420 then
			--grow up buy
			GameApi:showMessage("恭喜您成功购买成长计划")
		else
			GameApi:showMessage("恭喜您成功领取")
			
			--suihua shit function. update number
			local id = msg.activity_id - 1
			for i = 1, 10 do
				local index = 10 * id + i
				print("index is "..index)
				if ActivityCentre[id] ~= nil and self.ListDuiHuanData[index] ~= nil then
					local dtable = self.ListDuiHuanData[index]
					print("update dui hua label")
					
					local b = dtable.baseNumber
					local getNumber, istype6 = self:getItemNumberById(tonumber(dtable.item_number))
					if getNumber < 0 then
						getNumber = 0
					end
					--dtable.baseNumber = getNumber
					local nameLabel = dtable.label
					local str = MainMenuLayer:GetNumByFormat(getNumber).."/"..MainMenuLayer:GetNumByFormat(b)
					nameLabel:setString(str)
					print("str is "..str)
					if getNumber >= tonumber(b) then 
						nameLabel:setColor(ccc3(0, 255, 0))
					else
						nameLabel:setColor(ccc3(255, 0, 0))
					end
				else
					break;
				end
			end
		end
	else
		--GameApi:showMessage("领取失败")
	end
	return true
end

function operationUI:CSUpdateActivityInfoRspCallBack(msg)
	----print("Rcv CSUpdateActivityInfoRspCallBack msg")
	local recharge_table_i = 1;
	for _,v in ipairs(msg.activitys) do
		-- v is Activity_Info
		--print("id:"..v.activity_id.."times:"..v.revc_reward_times.."status is :"..v.status)
		repeat 
			if ActivityCentre[v.activity_id] == nil then
				----print("didn't match the server content")					
				break;
			end

			if tonumber(ActivityCentre[v.activity_id].Activity_Type) == 11 then				
				for __,vt in pairs(v.activity_steps) do
					--PayTopUI.player_pay_count = vt.step_value;		--获得玩家的充值数量.
					PayTopUI.day_recharge_active[recharge_table_i] = {id = v.activity_id, value = vt.step_value};
					recharge_table_i = recharge_table_i + 1;
					--------print("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" .. vt.step_value);
					----------print("Andy: ****************************" .. day_recharge_active[v.activity_id].value);
				end
			end

            print("activity_id: ***" .. v.activity_id.."status: ***"..v.status);
            for i, v in pairs(v.activity_steps) do
                print("\n****step_type: "..v.step_type)
                print("\n****step_id: "..v.step_id)
                print("\n****step_value: "..v.step_value)
            end

            if v.activity_steps ~= nil then
                for i, v1 in pairs(v.activity_steps) do
                    if v1.step_type == 12 then
                        print("\n****v.activity_id: "..tostring(v.activity_id))
                        print("\n****step_value: "..tostring(v1.step_value))
                        ActivityCentre[v.activity_id].cost_num = v1.step_value
                        ActivityCentre[v.activity_id].step_type = v1.step_type
                    end
                end
            end
			
			ActivityCentre[v.activity_id].time_limit = v.time_limit
			ActivityCentre[v.activity_id].revc_reward_times = v.revc_reward_times
			self:activityStateChange((v.activity_id), tonumber(v.status))
			--print("activity_id: ***" .. v.activity_id.."status: ***"..v.status);
		until true
	end
	
	self:checkAddTipIcon()

	return true
end

function operationUI:CSDelActivityInfoRspCallBack(msg)
	----------print("Rcv CSDelActivityInfoRspCallBack msg")
	for _, v in ipairs(msg.activity_ids) do
		ActivityCentre[v.activity_id].status=255--255 means delete
	end
	return true
end
---------------------message send out----------------------
function sendCSActivityListReq()
	msgname="CSActivityListReq"
	Req = {}
	send_message(msgname, Req, true)
end

function sendCSGetActivityDataReq()
	
	if isSendReq ~= true then

		isSendReq = true
		local msgname="CSGetActivityDataReq"
		local Req = {}
		----print("CSGetActivityDataReq")
		send_message(msgname, Req, true)
		
	end
	sendCSActivityListReq();
	checkFairyTipIcon()

	--ShowWaitingLayer()
end

function operationUI:sendCSGetActivityRewardReq(id, ...)
	local msgname="CSGetActivityRewardReq"
	Req = {
	activity_id=id,
	param={unpack(arg)},
	}
	send_message(msgname, Req, true)
end
---------------------UI funciont---------------------------
function operationUI:get_index_in_activity_centre(index)
	------print("PB_ACTIVITY_STATUS_NOT_START"..PB_ACTIVITY_STATUS_NOT_START.."PB_ACTIVITY_STATUS_INVALID"..PB_ACTIVITY_STATUS_INVALID)
	--for i, v in pairs(ActivityCentre) do
    --    v.status = PB_ACTIVITY_STATUS_UNFINISH
	--end
	for i, v in pairs(ActivityCentre) do
		--print("***status: "..v.status)
		if v.Activity_Show == index.."" and v.status ~= nil and  tonumber(v.status)>PB_ACTIVITY_STATUS_NOT_START and
		tonumber(v.status) < PB_ACTIVITY_STATUS_INVALID then
			------print("get_index_in_activity_centre"..i)
			return i
		end
	end
	return -1
end

function operationUI:get_index_in_activity_centre_no_status(index)
	------print("PB_ACTIVITY_STATUS_NOT_START"..PB_ACTIVITY_STATUS_NOT_START.."PB_ACTIVITY_STATUS_INVALID"..PB_ACTIVITY_STATUS_INVALID)
	for i, v in pairs(ActivityCentre) do
		------print(v.)
		if v.Activity_Show == index.."" then
			------print("get_index_in_activity_centre"..i)
			return i
		end
	end
	return -1
end

function operationUI:activity_Center_have_valid_item(index)
	
	for i, v in pairs(ActivityCentre) do
		if v.Activity_Show == index.."" then
			
			local s = v.status
			if  s ~= nil and  tonumber(s) == PB_ACTIVITY_STATUS_FINISH then
				------print(index .. "have valid item for "..index);
				return true;
			end
		end
	end
	return false
end

function operationUI:setupbuttonAreaScrollist()
	--[[if self.loaded ~= nil then
		return;
	end
	self.loaded=true
    --]]
	self.scrollList = self.Layout:FindChildObjectByName("BUTTONAREA_LIST").__UIScrollList__
	
	
	if nil == self.scrollList then
		return
	end
	
	self.scrollList:clearChildrens()
 	self.ListButtons = {}
	self.ListItemButtons = {}

	self.listIndex2ActivityId={}

	local buttonIndex = 1
	local lastactivity=-1;

	for index,value in pairsByKeys(ShowActivity) do
		------print("setupbuttonAreaScrollist index"..index.."activity_show:"..value.Activity_Show)
		local acid = self:get_index_in_activity_centre(value.Activity_Show)
		
		----print("acid:"..acid)
		if acid ~= -1 then
			--local s = ActivityCentre[acid].status

			if tonumber(lastactivity) ~= tonumber(value.Activity_Show) then
				
				--------print("index is  "..index.."activity show"..value.Activity_Show.."acid:"..acid.."status:"..s)
				lastactivity = value.Activity_Show
				self:InsertButtonAreaOneItem(self.scrollList,value, buttonIndex, self:activity_Center_have_valid_item(value.Activity_Show))
				self.listIndex2ActivityId[buttonIndex]=tonumber(value.Activity_Show)
				buttonIndex = buttonIndex + 1
			end
		else
			--GameApi:showMessage("Wrong:show activity id"..value.Activity_Show)
		end

	end
	if #operationUI.ListItemButtons == 0 then
	   GameApi:showMessage("server have no valid activiry item")
	   return
	end
	operationUI.ListItemButtons[1]:selected()

	self.SelectItemIndex = 1
	self.SelectedListItem = operationUI.ListItemButtons[1]
	
end

function operationUI:InsertButtonAreaOneItem(scrollList,value, index, showtip)
	if nil == scrollList then
		return
	end

	----------print("Insert One Item " .. index)

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	--Note: 定义背景框大小
	local backGroundFrameName = "map_ui_operation_FRAME_OPERATION_FRAME_BUTTON_ACTIVITY1";
	-- Note: List Item


	local normalListItemIconBgName = "map_ui_operation_FRAME_OPERATION_BUTTON_ACTIVITY1";
	local clickListItemIconBgName = "map_ui_operation_FRAME_OPERATION_BUTTON_ACTIVITY1_SELECTED";
	local normal = AspriteManager:getInstance():getOneFrame(KUI_OPERAION_BIN,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(KUI_OPERAION_BIN,clickListItemIconBgName)

	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);

	if showtip then
		button:showTipIcon()
	end
	if value.Activity_Condition == "1" then --hot icon
		self:addHotIcon(backGroundFrameName,button);
	end
	if value.Activity_Condition == "2" then -- new icon
		self:addNewIcon(backGroundFrameName,button);
	end
	--add label

	local childFrameName = "map_ui_operation_FRAME_OPERATION_TEXT_ACTIVITY1"
	self:addButtonLabel(backGroundFrameName,childFrameName,value.Activity_Name,32,ccc3(255,206,87),button:getAnimationNode(),10,index,kCCTextAlignmentCenter);

	--add picture

	local menuItem = button:getMenuItemSprite();
	menuItem:registerScriptTapHandler(onOperationButtonListItemClick);

	local listItemCount = table.getn(self.ListItemButtons);
	self.ListItemButtons[listItemCount+1] = menuItem

	local listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = button

end

function onOperationButtonListItemClick(nTag)
	----------print("onOperationButtonListItemClick " .. nTag)
	
	local listItemIndex = nTag+1
	
	--if operationUI.lastSelectIndex ~= nil and operationUI.lastSelectIndex == listItemIndex then	
	--	return
	--end
	--operationUI.lastSelectIndex = listItemIndex
	operationUI.SelectItemIndex = listItemIndex

	local item = operationUI.ListItemButtons[listItemIndex]
	if nil ~= operationUI.SelectedListItem then
		operationUI.SelectedListItem:unselected()
	end
	item:selected()
	operationUI.SelectedListItem = item
	local local_activity_show = operationUI.listIndex2ActivityId[listItemIndex];
    --print("**local_activity_show: "..local_activity_show)
	--[[local isGrowup=false
	for i, v in pairs(ShowActivity) do
		if tonumber(v.Activity_Show) == local_activity_show and v.ButtonID == "7" then
			isGrowup = true;
		end
	end
	--operationUI.contentList2:setVisible(true)
	operationUI.Layout:FindChildObjectByName("BUTTONNEW").__UIButton__:setVisible(false)

	operationUI.Layout:FindChildObjectByName("BUTTONNEWLABEL").__UILabel__:setVisible(false)
	if isGrowup == true then
		------print("grow up");
		operationUI.growupTitleContainer:setVisible(true)
		
		operationUI.Layout:FindChildObjectByName("TitleContent").__UILabel__:setVisible(false)
		
		--operationUI.titlebg:setVisible(false)
		operationUI:setupContentAreaForGrownUp(listItemIndex)
	else
		operationUI.Layout:FindChildObjectByName("TitleContent").__UILabel__:setVisible(true)
		--operationUI.titlebg:setVisible(true)
		operationUI.growupTitleContainer:setVisible(false)
	
		operationUI:setupContentAreaScrollist(nTag+1)
	end--]]
    local buttonID = 0
    for i, v in pairs(ShowActivity) do
        if tonumber(v.Activity_Show) == local_activity_show then
            buttonID = v.ButtonID
            --print("***buttonid: "..buttonID)
        end
    end

    --右上侧title上的按钮和按钮上的文字，每次初始化为不显示
    local btn = operationUI.Layout:FindChildObjectByName("BUTTONNEW").__UIButton__    
	btn:setVisible(false)
    operationUI.Layout:FindChildObjectByName("BUTTONNEWLABEL").__UILabel__:setVisible(false)

    if buttonID == "7" then
		operationUI.growupTitleContainer:setVisible(true)	
		operationUI.Layout:FindChildObjectByName("TitleContent").__UILabel__:setVisible(false)        
        operationUI:setupContentAreaForGrownUp(listItemIndex)
    else
		operationUI.Layout:FindChildObjectByName("TitleContent").__UILabel__:setVisible(true)
		operationUI.growupTitleContainer:setVisible(false)
		operationUI:setupContentAreaScrollist(nTag+1)       
    end
end

function operationUI:setupContentAreaForGrownUp(id)
	--set title
	local buygrowupbt = self.growupTitleContainer:FindChildObjectByName("growUpBuy").__UIButton__:getMenuItemSprite()

	--local bgIconPath = "UI/growup.png"
	--local bg = CCSprite:create(bgIconPath)
	--if bg ~= nil then
	--	self.growupTitleContainer:FindChildObjectByName("growupbg").__UIPicture__:addExtraPic(bg, ccp())
	--end
	ActivityCentre[10420].button_item =  self.growupTitleContainer:FindChildObjectByName("growUpBuy").__UIButton__
	if ActivityCentre[10420] ~= nil and ActivityCentre[10420].status ~= nil and ActivityCentre[10420].status == PB_ACTIVITY_STATUS_REWARD_REVC then
		------print("grow up money is already buyed. so set button go gray")
		operationUI.growupTitleContainer:FindChildObjectByName("growUpBuy").__UIButton__:setEnabled(false)
	else
		buygrowupbt:registerScriptTapHandler(
            function()
			    ------print("sendCSGetActivityRewardReq");
			    self:sendCSGetActivityRewardReq(10420)
		    end
        )
	end
	--set title word

	local sjparam = ActivityCentre[10420].Activity_Qualification
	local i = 0;vip = 2; diamondNum = 888
	for a, b, c in string.gfind(sjparam, "(%d+)\/(%d+)\/(%d+)") do
		if i == 0 then
			i = 1
			vip = c
		else
			diamondNum = c
		end
	end
	
	operationUI.growupTitleContainer:FindChildObjectByName("growUpDiamondNum").__UILabel__:setString(diamondNum);

--------------------------------------		
	local contentList = self.contentList2
    
	if nil == contentList then
		return
	end
	contentList:clearChildrens()
	contentList:setStartOffset(ccp(0, 0))

	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	local size = CCSizeMake(contentList:getContentSize().width / scale,contentList:getContentSize().height / scale)
	local local_activity_show = self.listIndex2ActivityId[id]
	local index = 0;
	local color = ccc3(250, 62, 25)
	local ActivityCentreIndex = 0;
	local allFreeDiamondNum = 0
	for i, v in pairs(ShowActivity) do
		if tonumber(v.Activity_Show) == local_activity_show then
			local node = nil
			local one_activity = nil
			local container_name = "map_ui_operation_FRAME_OPERATION_BACKGROUND_FRAME1"
			local nodeFrame, nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, container_name)

			node = CCNode:create()	
			contentList:AddBaseItem(nodeFrame:getContentSize(), node)		
			nodeFrame:setPosition(ccpSub(nodePt, nodePt))
			node:addChild(nodeFrame, -1);

			local textIndex = 0
			for w, d in string.gfind(v.Activity_Content, "(.-)(\$)") do			
				local fontSize = 26
				
				if string.find(w, "#", 1) ~= nil then
					local index = 1
					for center_index in string.gfind(w, "#(.-)#") do
						------print("grown up  is " .. center_index)
						ActivityCentreIndex = tonumber(center_index)

						--self:addItem(container_name, node, nodePt, index, center_index)
						--(backGroundFrameName,parentNode, nodePt,start_number, itemid)

						one_activity = ActivityCentre[tonumber(center_index)]
						local award;

						if one_activity.Award1 ~= "0" then
							award = one_activity.Award1;
						else
							award = one_activity.Award2;
						end
						----------print("Award"..award);
						local i = 1;
						for item_id, b, c in string.gfind(award, "(%d+)\/(%d+)\/(%d+)") do					
							------print("item id is "..item_id)										
							local childFrameName = "map_ui_operation_FRAME_OPERATION_REWARD"..index

							local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
							btFrame:setPosition(ccpSub(btPt, nodePt))
							node:addChild(btFrame, 2);
							local sprite = ItemManager:Get():getIconSpriteById(item_id)       --奖励的砖石的按钮的图片
							if sprite ~= nil then
							    childFrameName = "map_ui_operation_FRAME_OPERATION_GROWUP_ICON_REWARD"..index   
								local iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
								
							    --local boxLockSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_ALCHEMY_ICON_CLICKED")
                                
							    local pEmptyItem = CCSprite:create()
							    pEmptyItem:setContentSize(iconframe:getContentSize())

							    boxBtn = IconButton:new(pEmptyItem, nil, iconframe)

							    local btnMenuItem = boxBtn:getMenuItem()
							    btnMenuItem:setTag(tonumber(center_index) * 10 + i)
							    btnMenuItem:registerScriptTapHandler(operationUIonPressIconButton)
							    self:addFrame(boxBtn, item_id);
							    node:addChild(boxBtn, 4)
							    --iconpiture:getCurrentNode():addChild(boxBtn)
							    boxBtn:setPosition(ccpSub(iconPt, nodePt))

							    boxBtn:SetNormalIcon(sprite);

							    self.ListIconId[tonumber(center_index) * 10 + i] = item_id
							    self.ListIconBtn[tonumber(center_index) * 10 + i] = boxBtn
								--add item number
								childFrameName = "map_ui_operation_FRAME_OPERATION_GROWUP_ICON_NUMBER"..index
								iconframe, iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
								allFreeDiamondNum = allFreeDiamondNum + tonumber(b)
								local nameLabel = TXGUI.UILabelTTF:create(""..MainMenuLayer:GetNumByFormat(b), KJLinXin, 24)
								nameLabel:setPosition(ccpSub(iconPt, nodePt))
								node:addChild(nameLabel, 5);
							end
							i = i + 1
							index = index + 1
						end --end for center index award
					end  --end for ##
				else --nomal text
					if textIndex == 0 then --the upper text 
						childFrameName = "map_ui_operation_FRAME_OPERATION_REWARD1TEXT1"
										  
						iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
						iconPt.x = iconPt.x - iconframe:getContentSize().width / 2
						local nameLabel = TXGUI.UILabelTTF:create(w, KJLinXin, 24)
						nameLabel:setColor(ccc3(126, 255, 0))
						nameLabel:setAnchorPoint(ccp(0, 0.5))
						nameLabel:setPosition(ccpSub(iconPt, nodePt))
						node:addChild(nameLabel, 5);
						textIndex = textIndex + 1
					else
						--the downer text
						childFrameName = "map_ui_operation_FRAME_OPERATION_REWARD1TEXT2"
						iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
						iconPt.x = iconPt.x - iconframe:getContentSize().width / 2
						local nameLabel = TXGUI.UILabelTTF:create(w, KJLinXin, 22)
						nameLabel:setColor(ccc3(155, 207, 255))
						nameLabel:setAnchorPoint(ccp(0, 0.5))
						nameLabel:setPosition(ccpSub(iconPt, nodePt))
						node:addChild(nameLabel, 5);
					end					
				end
			end -- end for w, d in string.gfind(v.Activity_Content, "(.-)(\$)") do

			local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, "map_ui_operation_FRAME_OPERATION_BUTTON1")
			local btClickSprite = AspriteManager:getInstance():getOneFrame(KUI_OPERAION_BIN, "map_ui_operation_FRAME_OPERATION_BUTTON1_CLICKED")
			--local diableframe = graylightWithCCSprite(btFrame, false)
			local enable = true
			if one_activity ~= nil and one_activity.status ~= nil and one_activity.status == PB_ACTIVITY_STATUS_REWARD_REVC then
				btClickSprite = graylightWithCCSprite(btFrame, false)
				btFrame = graylightWithCCSprite(btFrame, false)
				enable = false
			end
			
			local menuItem = CCMenuItemSprite:create(btFrame, btClickSprite)
			
			local button = UIMenu:create()
			if enable == false then
				menuItem:setEnabled(false)
			end
			button:addChild(menuItem, 1, 1001)
			button:setInitPriority(-131)
			button:setPosition(ccpSub(btPt, nodePt))
			if ActivityCentreIndex ~= 0 then
				menuItem:setTag(tonumber(ActivityCentreIndex))
			end
			node:addChild(button, 2, 1001)
			menuItem:registerScriptTapHandler(
                function(tag)
					------print("tag:"..tag)
					if ActivityCentre[tag].status ~= nil and ActivityCentre[tag].status == PB_ACTIVITY_STATUS_REWARD_REVC then
						return
					end

					self:sendCSGetActivityRewardReq(tag)
			    end
			)
			--add button label

			local labelFrame, labelPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, "map_ui_operation_FRAME_OPERATION_TEXT1_BUTTON")
			local labeltext = "领取"
			local nameLabel = TXGUI.UILabelTTF:create(labeltext, KJLinXin, 26)
			nameLabel:setPosition(ccpSub(labelPt, nodePt))
			node:addChild(nameLabel, 3, 1000)
			--one_activity.status=PB_ACTIVITY_STATUS_UNFINISH
	
			if one_activity ~= nil then
				one_activity.button_label = nameLabel --text tag 1000 button  tag1001
				one_activity.button_item = menuItem
			end					
		end --end if tonumber(v.Activity_Show) == local_activity_show then
	end -- for i, v in pairs(ShowActivity) do
	--show title content
	local container_name="map_ui_operation_FRAME_OPERATION_BUY_TEXT"
	local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, container_name)
	local CCSize s = nodeFrame:getContentSize()
	local tempnode =  CCNode:create()
	--local CCSize s = nodeFrame:getContentSize()
	tempnode:setContentSize(s)
	
	--tempnode:setPosition(CCPoint(s.width / 2, s.height / 2))
	local node = HtmlNode:create(s.width)
	node:begin()
	node:addLabel(GetLuaLocalization("M_GROW_1"), 20, KJLinXin, ccc3(255, 255, 255))
	node:addLabel(GetLuaLocalization("M_GROW_2"), 20, KJLinXin, ccc3(255, 255, 255))
	
	node:addLabel(" VIP"..vip.." ", 20,KJLinXin, ccc3(126, 255, 0))
	node:addLabel(GetLuaLocalization("M_GROW_3"), 20,KJLinXin, ccc3(255, 255, 255))
	node:addLabel(""..allFreeDiamondNum, 20, KJLinXin, ccc3(126, 255, 0))
	node:addLabel(GetLuaLocalization("M_GROW_4"), 20, KJLinXin, ccc3(255, 255, 255))
	--node:setPosition(CCPoint(s.width / 2, s.height / 2))
	node:finish()
	node:setAnchorPoint(ccp(0, 0.6))
	tempnode:addChild(node)
	operationUI.growupTitleContainer:FindChildObjectByName("growUpTitleContent").__UILabel__:getCurrentNode():addChild(tempnode)
end

--Note: 显示hot icon
function operationUI:addHotIcon(backGroundFrameName,button)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_operation_FRAME_OPERATION_TAB_HOT";

	local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_OPERAION_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	pChildSprite:setPosition(relativePoint);
	button:getAnimationNode():addChild(pChildSprite,2);
	--button:setIconSprite(pChildSprite)
end

--Note: 显示new  icon
function operationUI:addNewIcon(backGroundFrameName,button)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_operation_FRAME_OPERATION_TAB_NEW";

	local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_OPERAION_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	pChildSprite:setPosition(relativePoint);
	button:getAnimationNode():addChild(pChildSprite,2);
	--button:setIconSprite(pChildSprite)
end

--Note: 显示文本函数
function operationUI:newLabel(content,fontSize,contentSize,color,HorAligment,VerAligment)
	--local relativePoint = CCPoint(0,0);
	--local contentSize = CCSize(labelSize.width,labelSize.height)
	if HorAligment == nil then
		HorAligment = kCCTextAlignmentLeft
	end
	if VerAligment == nil then
		VerAligment = kCCVerticalTextAlignmentCenter
	end

	nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)

	nameLabel:setColor(color)

	return nameLabel
end

--Note: 显示button文本函数
function operationUI:addButtonLabel(bgName,uiName,content,fontSize,color,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_OPERAION_BIN,bgName,uiName,anchorPoint,relativePoint);
	local labelSize = pLabelSprite:getContentSize();
	local contentSize = CCSize(labelSize.width,labelSize.height)
	--
	local nameLabel = parentNode:getChildByTag(nTag)
	if nil == nameLabel then
		nameLabel=self:newLabel(content,fontSize,contentSize,color,HorAligment,VerAligment)
		--nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)
		nameLabel:setTag(nTag)

		parentNode:addChild(nameLabel,zOrder)
	end
	--nameLabel:setString(content)
	--nameLabel:setColor(color)
	nameLabel:setPosition(relativePoint)
	--return nameLabel
end

function operationUI:createNode(str, width, fontSize, fontColor)

		node = HtmlNode:create(width)
		node:begin()
		node:addLabel(str,fontSize,KJLinXin,fontColor)
		node:finish()
		node:setAnchorPoint(ccp(0,0.5))
		return node
end

function operationUI:scrolllistAddNode(list, node)
			local base = list:AddBaseItem(node:getContentSize(),node)
			local oldPos_y = node:getPositionY()
			node:setPosition((list:getContentSize().width ) * 0.8,oldPos_y)

			local size = list:getContentSize()
			local viewSize = list:getViewSize()
			
			local offset = ccp(0,size.height - viewSize.height)
			list:setStartOffset(offset)
			
			

end

function operationUI:createContentListForGrowUp()

	self.contentList2 = self.Layout:FindChildObjectByName("CONTENTAREA_LIST1").__UIScrollList__
	self.growupTitleContainer = self.Layout:FindChildObjectByName("growupTitleContainer").__UIContainer__
	--self:setupContentAreaScrollist(specialtag)
	--self.contentList2:setVisible(false)
	self.growupTitleContainer:setVisible(false)
end

function operationUI:setupContentAreaScrollist(id)
	local local_activity_show = self.listIndex2ActivityId[id];

    print("**************************id is "..id)
	
	local contentList = self.contentList2
    --contentList = self.Layout:FindChildObjectByName("CONTENTAREA_LIST").__UIScrollList__
	if nil == contentList then
		----------print("contentList is nulll")
		return
	end
	contentList:clearChildrens()
	
	contentList:setStartOffset(ccp(0,0))
	--local contentSize=contentList:getContentSize()
	--local buttonIndex = 1

	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	local size = CCSizeMake(contentList:getContentSize().width / scale,contentList:getContentSize().height / scale)

	local index = 0;
	local color=ccc3(250, 62, 25)
	local ActivityCentreIndex=0;
	--add title
	local container_name="map_ui_operation_FRAME_OPERATION_BUY_TEXT"
	local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,container_name)
	local CCSize s = nodeFrame:getContentSize()
	if self.tempnode ~= nil then
		self.tempnode:removeFromParentAndCleanup(true)
	end
	self.tempnode =  CCNode:create()
	----local CCSize s = nodeFrame:getContentSize()
	self.tempnode:setContentSize(s)
	
	----tempnode:setPosition(CCPoint(s.width/2, s.height/2))
	local titleNode = HtmlNode:create(nodeFrame:getContentSize().width)
	titleNode:begin()

	
	local title = operationUI.Layout:FindChildObjectByName("TitleContent").__UILabel__:getCurrentNode()
	
	title:addChild(self.tempnode, 5, 1000)
	
		--local CCSize s = nodeFrame:getContentSize()
	self.tempnode:setContentSize(nodeFrame:getContentSize())
			
	local addTitlenode = false		

    local isConsumeActivity = false
    local consumeActivityId

	print("local_activity_show:"..local_activity_show)
    for i, v in pairs(ActivityCentre) do 
        if tonumber(v.Activity_Show) == local_activity_show and v.step_type ~= nil then
            isConsumeActivity = true
            consumeActivityId = i
            break;
        end
    end

	for i, v in pairs(ShowActivity) do
		if tonumber(v.Activity_Show) == local_activity_show then
			local node=nil
			local one_activity=nil
			local nodeFrame
			local nodePt
			--color=allColors[tonumber(v.Font_Color)]
			local r, g, b = string.match(v.Font_Color, "(%d+)\/(%d+)\/(%d+)")
			color = ccc3(tonumber(r), tonumber(g), tonumber(b))
			--------print("Activity_Show:"..v.Activity_Show)

            local frameIndex = ""

            if v.ItemFlag ~= nil and v.ItemFlag == "1" then    --右边选项条有两套UI（有进度的和没进度的），这里决定用哪套
                frameIndex = "2"
            else
                frameIndex = ""
            end
			
			local showTextForAward=false
			--add icon list node

			if string.find(v.Activity_Content,"#", 1) ~= nil then
				--when add #. finish the title
				--------print("showTextForAward is true");
				showTextForAward = true
				local container_name="map_ui_operation_FRAME_OPERATION_FRAME"..frameIndex.."_REWARDALL"     --右侧选项条背景
				nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,container_name)
				if node == nil then
					node = CCNode:create()
					contentList:AddBaseItem(nodeFrame:getContentSize(),node)
					nodeFrame:setPosition(ccp(0,0))
					node:addChild(nodeFrame,-1);
				end
			end
			local str
			for w, d in string.gfind(v.Activity_Content, "(.-)(\$)") do
				--one_activity = ActivityCentre[10006]
				--get font size
				local fontSize=26
				--this line has #. process it seprately
				if string.find(w,"#", 1) ~= nil then
					local index = 1

					for center_index in string.gfind(w, "#(.-)#") do
						----------print("activity center index is " .. center_index)
						ActivityCentreIndex=tonumber(center_index)
						print(ActivityCentreIndex)
						--self:addItem(container_name, node, nodePt, index, center_index)
						--(backGroundFrameName,parentNode, nodePt,start_number, itemid)

						one_activity = ActivityCentre[tonumber(center_index)]

						if one_activity == nil then
							GameApi:showMessage("activity center table has no item for index:"..center_index)
							return
						end

						local award;

						if one_activity.Award1 ~= "0" then
							award = one_activity.Award1;
						else
							award = one_activity.Award2;
						end
						----------print("Award"..award);
						local i = 1;
						for item_id, b, c in string.gfind(award, "(%d+)\/(%d+)\/(%d+)") do
							--local a, b, c = string.match(award, "(%d+)\/(%d+)\/(%d+)")
							----------print("item id is "..item_id)
							if index > 4 then
								index = 1
								node = CCNode:create()
								contentList:AddBaseItem(nodeFrame:getContentSize(), node)
							end
							local childFrameName = "map_ui_operation_FRAME_OPERATION_FRAME"..frameIndex.."_REWARD"..index

							local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
							btFrame:setPosition(ccpSub(btPt, nodePt))
							node:addChild(btFrame,2);
							local sprite = ItemManager:Get():getIconSpriteById(item_id)
							
							if sprite ~= nil then
								childFrameName = "map_ui_operation_FRAME_OPERATION_ICON"..frameIndex.."_REWARD"..index
								local iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
								--[[add item icon
								childFrameName = "map_ui_operation_FRAME_OPERATION_ICON_REWARD"..index
								local iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,childFrameName)
								sprite:setPosition(ccpSub(iconPt,nodePt))
								node:addChild(sprite,3);
								--]]
								local boxLockSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_ALCHEMY_ICON_CLICKED")

								local pEmptyItem = CCSprite:create()
								pEmptyItem:setContentSize(iconframe:getContentSize())

								boxBtn = IconButton:new(pEmptyItem, nil, boxLockSpriteOff)

								local btnMenuItem = boxBtn:getMenuItem()
								btnMenuItem:setTag(tonumber(center_index) * 10 + i)
								btnMenuItem:registerScriptTapHandler(operationUIonPressIconButton)
								self:addFrame(boxBtn, item_id);
								node:addChild(boxBtn, 4)
								--iconpiture:getCurrentNode():addChild(boxBtn)
								boxBtn:setPosition(ccpSub(iconPt, nodePt))

								boxBtn:SetNormalIcon(sprite);
								
								self.ListIconId[tonumber(center_index) * 10 + i] = item_id
								self.ListIconBtn[tonumber(center_index) * 10 + i] = boxBtn
								--add item number
								childFrameName = "map_ui_operation_FRAME_OPERATION_ICON"..frameIndex.."_NUMBER"..index
								iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
								if v.ItemFlag ~= nil and v.ItemFlag == "1" then
                                    local nameLabel, getNumber
                                    --print("\n*****center_index: "..center_index)
                                    --print("\n*****Activity_Type: "..ActivityCentre[tonumber(center_index)].Activity_Type)
                                    if isConsumeActivity == true then
                                        getNumber = ActivityCentre[consumeActivityId].cost_num
                                        nameLabel = TXGUI.UILabelTTF:create(getNumber.."/"..MainMenuLayer:GetNumByFormat(b), KJLinXin, 18)
                                    else
                                        getNumber, istype6 = self:getItemNumberById(tonumber(item_id))
                                        nameLabel = TXGUI.UILabelTTF:create(MainMenuLayer:GetNumByFormat(getNumber).."/"..MainMenuLayer:GetNumByFormat(b), KJLinXin, 18)
                                    end

             --                       if ActivityCentre[tonumber(center_index)].Activity_Type == "5" then
									    --getNumber, istype6 = self:getItemNumberById(tonumber(item_id))			    
									    --nameLabel = TXGUI.UILabelTTF:create(MainMenuLayer:GetNumByFormat(getNumber).."/"..MainMenuLayer:GetNumByFormat(b), KJLinXin, 18)
             --                       elseif ActivityCentre[tonumber(center_index)].Activity_Type == "12" then
									    --getNumber = self:getConsumeNumberByActivityID(tonumber(center_index))			    
									    --nameLabel = TXGUI.UILabelTTF:create(MainMenuLayer:GetNumByFormat(getNumber).."/"..MainMenuLayer:GetNumByFormat(b), KJLinXin, 18)   
             --                       else
									    --getNumber, istype6 = self:getItemNumberById(tonumber(item_id))			    
									    --nameLabel = TXGUI.UILabelTTF:create(MainMenuLayer:GetNumByFormat(getNumber).."/"..MainMenuLayer:GetNumByFormat(b), KJLinXin, 18)                                        
             --                       end     
                                                                   
									if getNumber >= tonumber(b) then 
										nameLabel:setColor(ccc3(0, 255, 0))
									else
										nameLabel:setColor(ccc3(255, 0, 0))
									end

									nameLabel:setPosition(ccpSub(iconPt, nodePt))
									node:addChild(nameLabel, 5);
									
										--need update the label when user do the action 							
									print("duihuan label is "..(tonumber(center_index) * 10 + i))
									self.ListDuiHuanData[tonumber(center_index) * 10 + i]={}
									self.ListDuiHuanData[tonumber(center_index) * 10 + i].baseNumber = b
									--self.ListDuiHuanData[tonumber(center_index) * 10 +i ].istype6 = istype6
									self.ListDuiHuanData[tonumber(center_index) * 10 + i].item_number = tonumber(item_id)
									self.ListDuiHuanData[tonumber(center_index) * 10 + i].label = nameLabel								
								else
									local nameLabel = TXGUI.UILabelTTF:create(""..MainMenuLayer:GetNumByFormat(b), KJLinXin, 24)
									nameLabel:setPosition(ccpSub(iconPt, nodePt))
									node:addChild(nameLabel, 5);
								end						
							end
							i=i+1
							index = index+1
						end --end for center index award
					end  --end for ##
				else --nomal text
					if w ~= nil then
						str = w
					end
				end
			end -- end for w, d in string.gfind(v.Activity_Content, "(.-)(\$)") do
			

			-- set nameLabel
			if str ~= nil then
				if showTextForAward then

					local index = 0;
					one_activity.infoStrList = {}
					for w, d in string.gfind(str, "(.-)(\&)") do
						if w ~= nil and w ~= '' then
							one_activity.infoStrList[index] = w
							index =index + 1
						end
					end
					
								print("type:"..one_activity.Activity_Type)
					if index ~= 0 then
						if one_activity.status == PB_ACTIVITY_STATUS_UNFINISH then
							for a, b, c in string.gfind(one_activity.Activity_Qualification, "(%d+)\/(%d+)\/(%d+)") do
								costnum = 0
								if one_activity.Activity_Type == '11' then
									active_id_dcount = local_activity_show * 10;	--记录当前的活动id.
									for _, dv in ipairs(PayTopUI.day_recharge_active) do
										if(dv.id == active_id_dcount) then
											PayTopUI.player_pay_count = dv.value;
										end
									end
									costnum = PayTopUI.player_pay_count or 0
								elseif one_activity.Activity_Type == '5' then
									costnum = one_activity.cost_num or 0
								end
								str = string.format(one_activity.infoStrList[0], c - costnum)
							end
						elseif one_activity.status == PB_ACTIVITY_STATUS_FINISH then
							str = one_activity.infoStrList[1]
						elseif one_activity.status == PB_ACTIVITY_STATUS_REWARD_REVC then
							str = one_activity.infoStrList[2]
						end 
					end
					childFrameName = "map_ui_operation_FRAME_OPERATION_TEXT"..frameIndex.."_REWARDINFO"
										  
					iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, childFrameName)
					iconPt.x = iconPt.x-iconframe:getContentSize().width / 2
					local nameLabel = TXGUI.UILabelTTF:create(str, KJLinXin, 24)
					nameLabel:setColor(ccc3(126, 255, 0))
					nameLabel:setAnchorPoint(ccp(0, 0.5))
					nameLabel:setPosition(ccpSub(iconPt, nodePt))
					node:addChild(nameLabel, 5);
					one_activity.info_label = nameLabel
				else
					--local size = CCSizeMake(self.contentList:getContentSize().width / scale,self.contentList:getContentSize().height / scale)
					--node
					--[[
					--		tmp.startdate
					--		tmp.enddate
					--		tmp.starttime 
					--		tmp.endtime 
					--		]]
					if string.find(str, "time", 1) ~= nil then
						local center_index = self:get_index_in_activity_centre_no_status(local_activity_show);
						if center_index == -1 then
							GameApi:showMessage("time error,center table has no item for index:"..local_activity_show)
						else
							local y, m ,d ,h,min
							local startDate = ActivityCentre[center_index].startdate
							y, m ,d = GameApi:GetTimeBySec(ActivityCentre[center_index].startdate)
						
							local fromDate=y.."年"..m.."月"..d.."日"
							y, m ,d = GameApi:GetTimeBySec(ActivityCentre[center_index].enddate)
							local endDate=m.."月"..d.."日"
							--y, m ,d ,h,min= GameApi:GetTimeBySec(ActivityCentre[center_index].starttime)
							--local fromTime = h.."时"..min.."分"
							--y, m ,d ,h,min= GameApi:GetTimeBySec(ActivityCentre[center_index].endtime)
							--local endTime = h.."时"..min.."分"
							str= fromDate.."到"..endDate --.."每天".." "..fromTime.."到"..endTime							
						end
					end
						
					titleNode:addLabel(str.."\n", 20, KJLinXin, color)
					--local node = self:createNode(w, size.width, 20, color)					
				end
			end
			-- set nameLabel


			--add button param button id 1:show another panel 2:get the reward
			if v.ButtonID ~= '0' and one_activity ~= nil then --
				local container_name="map_ui_operation_FRAME_OPERATION_FRAME_REWARDALL"
				local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, container_name)
				if node == nil then
					node = CCNode:create()
					contentList:AddBaseItem(nodeFrame:getContentSize(), node)
				end

				local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, "map_ui_operation_FRAME_OPERATION_BUTTON")
				local btClickSprite = AspriteManager:getInstance():getOneFrame(KUI_OPERAION_BIN, "map_ui_operation_FRAME_OPERATION_BUTTON_CLICKED")
				--local diableframe = graylightWithCCSprite(btFrame, false)
				local enable=true
				if one_activity ~= nil and one_activity.status ~= nil and one_activity.status ~= PB_ACTIVITY_STATUS_FINISH then
					btClickSprite =  graylightWithCCSprite(btFrame, false)
					btFrame= graylightWithCCSprite(btFrame, false)
					enable = false
				end
				if btFrame ~= nil then
					local menuItem = CCMenuItemSprite:create(btFrame, btClickSprite)
					local button = UIMenu:create()
					if enable == false then
						menuItem:setEnabled(false)
					end
					button:addChild(menuItem, 1, 1001)
					button:setInitPriority(-131)
					button:setPosition(ccpSub(btPt, nodePt))
					if ActivityCentreIndex ~= 0 then
						----------print("set menu item tag "..ActivityCentreIndex)
						menuItem:setTag(tonumber(ActivityCentreIndex))
					end
					node:addChild(button, 2, 1001)                    --领取按钮
					menuItem:registerScriptTapHandler(
                        function(tag)
						--self:sendCSEquipmentComposeReq(102, self.exp_got_now);				
							--print("*******tag: "..tag)
							if ActivityCentre[tag] ~= nil and ActivityCentre[tag].status ~= nil and ActivityCentre[tag].status == PB_ACTIVITY_STATUS_REWARD_REVC then
								return
							end
							self:sendCSGetActivityRewardReq(tag)
							--GameApi:showMessage("Coming Soon");					
						end
					)
					--add button label

					local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN, "map_ui_operation_FRAME_OPERATION_TEXT_BUTTON")
					local labeltext = "领取"
					if one_activity ~= nil and one_activity.status == PB_ACTIVITY_STATUS_REWARD_REVC then
						labeltext = "已领取"
					else
						labeltext = GetLuaLocalization(v.ButtonID_Text)
					end
					local nameLabel = TXGUI.UILabelTTF:create(labeltext, KJLinXin, 26)
					nameLabel:setPosition(ccpSub(labelPt, nodePt))
					node:addChild(nameLabel, 3, 1000)
					if one_activity ~= nil then
						one_activity.button_label = nameLabel --text tag 1000 button  tag1001
						one_activity.button_item = menuItem
					end
				end
			end --button id ~= 0
			
			--we have  one button on the top right . use this button when we have no activity center index corespond
			if tonumber(v.ButtonID)  ~= 0 and tonumber(v.ButtonID)  ~= 2  and showTextForAward == false then                     										
				local btn = operationUI.Layout:FindChildObjectByName("BUTTONNEW").__UIButton__
				btn:setVisible(true)
				btn:getMenuItemSprite():registerScriptTapHandler(
                    function ()
					    if v.ButtonID == "1" then
						    local tempid = 10000;
						    if ActivityCentre[10000].status == 3 then
							    tempid = 10000
						    elseif ActivityCentre[10080].status == 3 then
							    tempid = 10080
						    elseif  ActivityCentre[10090].status == 3 then
							    tempid = 10090
						    end
						    lotteryUI.ID = tempid
						    --------print("operaton lotteryUI.ID="..lotteryUI.ID)
						    lotteryUI:CreateLayer(nil)
				        elseif v.ButtonID == "3" then		--查询充值排行
					        active_id = local_activity_show * 10;
					        PayTopUI.Rank_active_id = active_id;	--保存起来用于判断活动结束.
	
					        self:sendCSQueryPlayerRankInfoReq_paydiamond();			--查询自己的排名信息.
					        self:sendCSQueryRankReq_paydiamond();
				        elseif v.ButtonID == "4" then
							
					        MessageBox:Show("竞技场排名,功能暂未实现.");	--by andyhuang
				        elseif v.ButtonID == "5" then		--查询玩家的充值数量.
					        active_id_dcount = local_activity_show * 10;	--记录当前的活动id.
							
					        for _, dv in ipairs(PayTopUI.day_recharge_active) do
						        if(dv.id == active_id_dcount) then
							        PayTopUI.player_pay_count = dv.value;
						        end
					        end
					        MessageBox:Show(GetLuaLocalization("M_RECHARGE_TIPS")..tostring(PayTopUI.player_pay_count or 0)..GetLuaLocalization("M_DIAMOND"), nil, nil, MB_OK, ccc3(255, 255, 255), 26);
				        elseif v.ButtonID == "6" then --收集
					        if  one_activity ~= nil and one_activity.status ~= nil and one_activity.status ~= PB_ACTIVITY_STATUS_FINISH then
						        if string.len(one_activity.Activity_Qualification) > 1 then
							        sjparam = one_activity.Activity_Qualification
						        else
							        sjparam = one_activity.Activity_Qualification2
						        end
						        local a, b, c = string.gmatch(sjparam, "(%d+)\/(%d+)\/(%d+)") 
								
						        local instance= self:searchInstanceIdByMat(b)
						        ------print("instance is "..instance.."star:"..InstanceManager:Get():getOneInstanceStars(instance))
						        if instance ~= 0 and InstanceManager:Get():getOneInstanceStars(instance) ~= -1 then
							        InstanceManager:Get():autoSearchPathToInstance(instance)
							        operationUICloseBtnClick()
						        else
							        --GameApi:showMessage(GetLuaLocalization("M_FINDMATERIAL_ERROR1"));
							        GameApi:showMessage(GetLuaLocalization("M_EXCHANGE_TIPS"));
						        end
                            end
                        elseif v.ButtonID == "8" then  --消耗类活动
                            local tag =  UITagTable["ShowQueryResultUILayer"].tag 
		                    layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		                    if layer == nil then
			                    ShowQueryResultUILayer:CreateLayer(v.Activity_Show)   
                            end
                        end
				    end
                );				
				
				local labeltext="领取"
					
				if one_activity ~= nil and one_activity.status == PB_ACTIVITY_STATUS_REWARD_REVC then
					labeltext = "已领取"
				else
					labeltext = GetLuaLocalization(v.ButtonID_Text)
				end
				if v.ButtonID == "6" and one_activity ~= nil and one_activity.status ~= nil and one_activity.status ~= PB_ACTIVITY_STATUS_FINISH then
					labeltext = "收集" 
				end
				operationUI.Layout:FindChildObjectByName("BUTTONNEWLABEL").__UILabel__:setVisible(true)
				operationUI.Layout:FindChildObjectByName("BUTTONNEWLABEL").__UILabel__:setString(labeltext)
			end
		end --end if tonumber(v.Activity_Show) == local_activity_show then
	end -- for i, v in pairs(ShowActivity) do
	
	--title text
	titleNode:finish()
	titleNode:setAnchorPoint(ccp(0, 0.4))
	self.tempnode:addChild(titleNode)
	addTitlenode = true	
end

function operationUI:getConsumeNumberByActivityID(id)
    return ActivityCentre[id].cost_num
end

function operationUI:getItemNumberById(item_id)
	local getNumber
	local istype6 = false --type 6 item only get the number form userinfo
	--print("item_id is"..item_id)
	if Items[tonumber(item_id)] ~= nil and Items[tonumber(item_id)].Type=="6" then
		istype6 = true
		if tonumber(item_id) == 416001 then
			getNumber = UserData:GetUserInfo().m_gold
		elseif tonumber(item_id) == 416002 then
			getNumber = UserData:GetUserInfo().m_diamond
		elseif tonumber(item_id) == 416015 then
			getNumber = UserData:GetUserInfo().m_fragStone	
		elseif tonumber(item_id) == 416018 then
			getNumber = UserData:GetUserInfo().m_soulStone	
		end
	elseif fairys[tonumber(item_id)] ~= nil then
		getNumber=GetFairyCountById(tonumber(item_id), 0)
	else
		getNumber = ItemManager:Get():getItemNumberById(item_id)
	end
	
	return getNumber, istype6
end

function operationUI:addFrame(boxBtn, item_id)
	--local itemInfo = ItemManager:Get():getItemInfoById(item_id);
	--if nil ~= itemInfo then
	--	if itemInfo.m_quality > 0 then
	--		local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality);
	--		if nil ~= framePic then
	--			boxBtn:setFrameSprite(framePic, ccp(0,0));
	--
	--		end
	--	end
	--end
end
--[[
    PB_ACTIVITY_STATUS_UNKNOW          = 0,    // 活动未知状态
    PB_ACTIVITY_STATUS_NOT_START       = 1,    // 活动未开始
    PB_ACTIVITY_STATUS_UNFINISH        = 2,    // 活动未完成状态
    PB_ACTIVITY_STATUS_FINISH          = 3,    // 活动完成状态
    PB_ACTIVITY_STATUS_REWARD_REVC     = 4,    // 活动奖励已经领取
    PB_ACTIVITY_STATUS_END             = 5,    // 活动奖励结束
    PB_ACTIVITY_STATUS_INVALID         = 6,    // 活动领取奖励时间失效
--]]
--tip按钮

local tipFairyId = 0
local function onOperationTipCallBackLeft(tag)

end

local function onOperationTipCallBackRight(tag)
	if tipFairyId ~= 0 then
		FairyInfoLayer.fairyId = tipFairyId
		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 2
		FairyInfoLayer:CreateLayer(nil)
	end
end

local function onClickEquipMiddleCallBack(tag)
	if tipFairyId ~= 0 then
		G_ShowFairyGetwayLayer(tipFairyId)
	end
end

function operationUIonPressIconButton(nTag)
   ----------print("onPressTipBoxButton:"..nTag)
   local id = nTag
   local item_id = operationUI.ListIconId[id]
   local iconbtn = operationUI.ListIconBtn[id]
   local menu = iconbtn:getMenuNode()
   local pos = menu:getTouchPoint()
   ItemManager:Get():showItemTipsById(item_id, pos)
   
   local isFairy = G_GetIsFairyById(tonumber(item_id))
   local info = ItemManager:Get():getItemInfoById(tonumber(item_id))
   if isFairy or (info ~= nil and info.m_type == 21) then
		tipFairyId = item_id % 10000;
		local detailText = LanguageLocalization:GetLocalization("M_EQUIPMENT_INFORMATION")
		ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onOperationTipCallBackRight,detailText,true)
   end

   --if EquipmentItems[tonumber(item_id)] ~= nil or (Items[tonumber(item_id)] ~= nil and Items[tonumber(item_id)]["Type"] == "19") then
	--	tipFairyId = tonumber(item_id)
	--	ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onClickEquipMiddleCallBack,LanguageLocalization:GetLocalization("M_FAIRY_GET_WAY"),true)
   --end
end
--[[
 PB_ACTIVITY_STATUS_UNKNOW          = 0,    // 活动未知状态
    PB_ACTIVITY_STATUS_NOT_START       = 1,    // 活动未开始
    PB_ACTIVITY_STATUS_UNFINISH        = 2,    // 活动未完成状态
    PB_ACTIVITY_STATUS_FINISH          = 3,    // 活动完成状态
    PB_ACTIVITY_STATUS_REWARD_REVC     = 4,    // 活动奖励已经领取
    PB_ACTIVITY_STATUS_END             = 5,    // 活动奖励结束
    PB_ACTIVITY_STATUS_INVALID         = 6,    // 活动领取奖励时间失效
	--]]
function operationUI:addItem(backGroundFrameName,parentNode, nodePt,start_number, itemid)



	local tt = ActivityCentre[tonumber(itemid)]
	local award;

	if tt.Award1 ~= "0" then
		award = tt.Award1;
	else
		award = tt.Award2;
	end
	----------print("Award"..award);
	local index = 1
	for a, b, c in string.gfind(award, "(%d+)\/(%d+)\/(%d+)") do
		--local a, b, c = string.match(award, "(%d+)\/(%d+)\/(%d+)")
		----------print("item id is "..a)
		local childFrameName = "map_ui_operation_FRAME_OPERATION_FRAME_REWARD"..index

		local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,childFrameName)
		btFrame:setPosition(ccpSub(btPt,nodePt))
		parentNode:addChild(btFrame,2);
		local sprite = ItemManager:Get():getIconSpriteById(a)
		if sprite ~= nil then
			--add item icon
			childFrameName = "map_ui_operation_FRAME_OPERATION_ICON_REWARD"..index
			local iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,childFrameName)
			sprite:setPosition(ccpSub(iconPt,nodePt))
			parentNode:addChild(sprite,3);

			--add item number
			childFrameName = "map_ui_operation_FRAME_OPERATION_ICON_NUMBER"..index
			iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_OPERAION_BIN,childFrameName)
			local nameLabel = TXGUI.UILabelTTF:create(""..b,KJLinXin,28)
			nameLabel:setPosition(ccpSub(iconPt,nodePt))
			parentNode:addChild(nameLabel,4);

		end
		index = index+1
		if index > 4 then
			return index
		end
	end --end for
	return 0



end
--[[
this cann add hyper link not used now . maybe later
function ChatShowPage:showSingleNote(note)
	if contentList ~= nil then
		local listNode = ChatListItem:create();
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
		local size = CCSizeMake(contentList:getContentSize().width / scale,contentList:getContentSize().height / scale)
		listNode:init(note,size.width,self.fontSize,self.menuPriority,self.isButtonEnable)
		self.chatListTable[note.noteIndex] = listNode
		if node ~= nil then
			node:setAnchorPoint(ccp(0,0.5))
			local base = contentList:AddBaseItem(node:getContentSize(),node)
			local oldPos_y = node:getPositionY()
			node:setPosition((0 + contentList:getContentSize().width ) * 0.5,oldPos_y)
			--node:setPosition((node:getContentSize().width + contentList:getContentSize().width /scale ) * 0.5,oldPos_y)
			contentListItemTable[note.noteIndex] = base
			local size = contentList:getContentSize()
			local viewSize = contentList:getViewSize()
			if size.height > viewSize.height then
				local offset = ccp(0,size.height - viewSize.height)
				contentList:setStartOffset(offset)
			end
		end
	end
end
--]]

function operationUI:InitLayer()


	self.closeBtn = self.Layout:FindChildObjectByName("BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
	self.closeBtn:registerScriptTapHandler(operationUICloseBtnClick);
	operationUI.Layout:FindChildObjectByName("BUTTONNEW").__UIButton__:setVisible(false)
	operationUI.Layout:FindChildObjectByName("BUTTONNEWLABEL").__UILabel__:setVisible(false)
	self:setupbuttonAreaScrollist()
	self:createContentListForGrowUp()
	--------print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 13")
	--operationUI.createlist2 = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(operationUIcreatelist2, 0.2, false)
	--addMsgCallBack("CSAttributesRsp", operationUI.CSAttributesRspCallBack, operationUI)

	--addMsgCallBack("CSEquipmentComposeRsp", operationUI.CSEquipmentComposeRspCallBack, operationUI)
	--self:sendCSGetUserAttributesReq()

	self.titlebg = self.Layout:FindChildObjectByName("titlebg").__UIPicture__
	
	--local bgIconPath = "UI/operation_bg.png"
	--local bg = CCSprite:create(bgIconPath)
	--if bg ~= nil then
	--	self.titlebg:setSprite(bg)
	--end
	
	self:setupContentAreaScrollist(1)
	
	
	self.buyDiamondBtn = operationUI.Layout:FindChildObjectByName("BUTTON_PAYDIAMOND").__UIButton__;
	self.buyDiamondBtn:getMenuItemSprite():registerScriptTapHandler(function(pobj)
		MainMenuLayer:getMainMenuLayer():ShowChargeLayer(true)		
	end)
end

--隐藏界面.
function operationUI:ShowOrNot(bShow)
	if nil ~= self.Layout then
		self.Layout:setVisible(bShow)
	end
end;

--查询个人充值总排行,前50名的玩家.
function operationUI:sendCSQueryRankReq_paydiamond()
	local msgname = "CSQueryRankReq";

	Req = {
		rank_name = active_id,	--代表钻石充值排行.
		rank_type = 1,			--代表总排行.
		rank_start = 1,			--起始排名为1.
		rank_end = 50			--表示前100名的玩家.
	};

	send_message(msgname, Req, true);
	--PushOneWaitingLayer("CSQueryRankReq");
end;

--前50名玩家信息返回.
function operationUI:CSQueryRankRspCallBack(msg)
	PayTopUI.PayRank_data = {};

	for i, v in ipairs(msg.rank_info) do
		PayTopUI.PayRank_data[v.rank] = {player_name = v.name,pay_count = v.score};
		-- --------print("*******************************Rank: " .. i .. " Name: " .. v.name .. " Score: " .. v.score);
	end;

	--------print("table size is :" .. table.getn(msg.rank_info) .. "!!!");
	if table.getn(msg.rank_info) == 0 then	--如果当前没有玩家上榜.
		GameApi:showMessage("当前没有玩家上榜.");
		return true;
	end
	local childLayer_PayTop = nil
	childLayer_PayTop = PayTopUI:CreateLayer(nil);	--todo需要将数据作为参数传递进去.

	if nil ~= childLayer_PayTop then
		self:ShowOrNot(false);
	end;
--	self:sendCSQueryPlayerRankInfoReq_paydiamond();			--查询自己的排名信息.

	return true;

end

--发送网络协议,请求个人充值钻石排名数据.
function operationUI:sendCSQueryPlayerRankInfoReq_paydiamond()
	local msgname = "CSQueryPlayerRankInfoReq";		--构造协议头字符串.
	Req = {
		rank_name = active_id,								--标示为充值钻石的排行榜 - 类型.
		userid = UserData:getUserId():getID(),				--todo 获得玩家的id
		reg_time = UserData:getUserId():getRegTime(),		--todo
		channel_id = UserData:getUserId():getChannel()		--todo
	};
	----------print("userid : " .. UserData:getUserId():getID() .. "reg_time : " .. UserData:getUserId():getRegTime() .. "channel_id : " .. UserData:getUserId():getChannel() .. "andy________________" .. Req.rank_name);
	send_message(msgname, Req, true);				--发送消息.
	--PushOneWaitingLayer("CSQueryPlayerRankInfoReq");
end

--个人充值信息返回.
function operationUI:CSQueryPlayerRankInfoRspCallBack(msg)

	PayTopUI.player_rank = 0;

	for _, v in pairs(msg.rank_info) do
		if v.rank_type == 1 then 						--如果排行榜是总排行.
			PayTopUI.player_rank = v.rank;				--获得玩家的排名.
		end
		------print(v.rank_type.."||||||||||||||||"..v.rank);
	end

	return true;
end
-------------------button callback------------------------------------------------

 function operationUICloseBtnClick(pObj)
	if operationUI.LayerInstance == nil then
		return
	end
	operationUI.contentList2:clearChildrens()
	
	operationUI.scrollList:clearChildrens()
   --TXGUI.UIManager:sharedManager():removeUILayout("operationUIPanel")
   --TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
   SetChatBar(true,-1)
   CCTextureCache:sharedTextureCache():removeUnusedTextures()
	
   RemoveOneLayer(operationUI.LayerInstance)
   --NotificationCenter:defaultCenter():unregisterScriptObserver(operationUI.closeCallBackFun)

   operationUI.LayerInstance:removeFromParentAndCleanup(true);
   --operationUI.LayerInstance = nil
   --operationUI:ResetValue()
end

-----------------------------------------------------------------

local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(closeLayerCallBack);
--send a msg to get all status
addMsgCallBack("CSActivityListRsp", operationUI.CSActivityListRspCallBack, operationUI)
addMsgCallBack("CSGetActivityDataRsp", operationUI.CSGetActivityDataRspCallBack, operationUI)

