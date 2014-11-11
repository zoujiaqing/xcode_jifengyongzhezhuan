require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")

local initedTimer = false;
local isShowTip = false;
local notification = nil;
local ONLINE_REWARD_TYPE = '7';
local onlineRewardLayer = nil;
local onlineRewardTimer = nil;
local currLevel = 0;--第几重，从0开始
local countDown = -1;--倒计时(秒)
local OnlineRewardData = {};
local OnlineRewardData1 = {};

OnlineRewardUI = {
	LayerInstance = nil, 
	LayerLayout = nil,
	get_ui_button = nil;
	reward_num_countdown = nil;
	ListButtons = {},
	ListItemIds = {},
}

function OnlineRewardUI:ResetValue()
	self.LayerInstance = nil;
	self.LayerLayout = nil;
	self.reward_num_countdown = nil;
	self.get_ui_button = nil;

	self.ListButtons = {};
	self.ListItemIds = {};
end

local function onReceiveDestructorHandler()
	CCLuaLog("OnlineRewardUI destructor");
	TXGUI.UIManager:sharedManager():removeUILayout("OnlineRewardUI");
	OnlineRewardUI:ResetValue();
	--[[
	if nil ~= notification then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notification);
		notification = nil;
	end
	--]]
end

function OnlineRewardUI:CreateLayer(parentLayer)
	CCLuaLog("OnlineRewardUI:CreateLayer");
	self:ResetValue();

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create();
	else
		self.LayerInstance = parentLayer;
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler);

	self.LayerLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/OnlineRewardUI.plist", self.LayerInstance, "OnlineRewardUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode();
		local tag = UITagTable["OnlineRewardLayer"].tag;
		mainMenuNode:addChild(self.LayerInstance, 0, tag);
	end

	PushOneLayer(self.LayerInstance, "", "");

	self:InitData();
	self:Init();
	self:setupContent();

	onlineRewardLayer = self;
end

local function onOnlineRewardLayerCloseClick(pObj)
	CCLuaLog("onOnlineRewardLayerCloseClick");
	if nil ~= OnlineRewardUI.LayerInstance then
		RemoveOneLayer(OnlineRewardUI.LayerInstance);
		CCTextureCache:sharedTextureCache():removeUnusedTextures();
		OnlineRewardUI.LayerInstance:removeFromParentAndCleanup(true);
		--OnlineRewardUI.LayerInstance = nil;
		--OnlineRewardUI:ResetValue();
	end
end

--在线奖励计时器

local function sendOnlineRewardReq()
	local msgname = "CSAskOnlineGiftReq";
	local req = {}
	local ret = send_message(msgname, req, true)
	return ret;
end

local function onOnlineRewardLayerGetClick(pObj)
	CCLuaLog("onOnlineRewardLayerGetClick");
	if nil ~= OnlineRewardUI.LayerInstance then
		req = {};
		local msgname = "CSGetOnlineGiftReq";
		send_message(msgname, req, true)
	end
end

local function closeCallBack(message)
	if nil ~= OnlineRewardUI.LayerInstance then
		if G_CurLayerInstance ~= OnlineRewardUI.LayerInstance then
			onOnlineRewardLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		CCLuaLog("on onlineReward layer close");
		if nil ~= onlineRewardTimer and (TimeManager:Get():hasTimer(TIMER_ONLINEREWARD_COUNTDOWN)) then
			TimeManager:Get():unregisterLuaTimer(onlineRewardTimer, TIMER_ONLINEREWARD_COUNTDOWN);
		end
		onOnlineRewardLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

local function onOnlineRewardItemButtonClick(index)
	CCLuaLog("onOnlineRewardItemButtonClick: index is " .. index);
	local item_id = OnlineRewardUI.ListItemIds[index];
	local iconbtn = OnlineRewardUI.ListButtons[index]
	local menu = iconbtn:getMenuNode()
	local pos = menu:getTouchPoint()
	ItemManager:Get():showItemTipsById(item_id, pos)
end

function OnlineRewardUI:InitData()
	OnlineRewardData = {};
	OnlineRewardData1 = {};
	--从每日领取奖历表中取出在线奖励数据
	for	id, value in pairsByKeys(AwardDaily) do
		if value ~= nil and value.Type1 == ONLINE_REWARD_TYPE then
			if value.Type2 == currLevel .. '' then
				--当前重数据
				CCLuaLog("onlineReward curr level id: " .. id);
				self:insertDataToRewardDataList(OnlineRewardData, value);
			elseif value.Type2 == (currLevel+1) .. '' then
				--下一重数据
				CCLuaLog("onlineReward next level id: " .. id);
				self:insertDataToRewardDataList(OnlineRewardData1, value);
			end
		end
	end
end

local function getItemIdAndNums(itemStr)
	local itemId,num;
	local _start,_end = string.find(itemStr,"/")
	_start = tonumber(_start);
	if nil ~= _start and 1 ~= _start then
		local tmpStr = string.sub(itemStr,_start+1,-1)
		num = tonumber(tmpStr)
		tmpStr = string.sub(itemStr, 1, _start-1)
		itemId = tonumber(tmpStr)
	end

	return itemId, num;
end

function OnlineRewardUI:insertDataToRewardDataList(container, data)
	if container == nil or data == nil then
		return
	end

	local itemId, num, itemStr;
	for i=1, 11 do
		itemStr = data["RaidDrops" .. i]
		if itemStr ~= nil and itemStr ~= '0' then
			itemId, num = getItemIdAndNums(itemStr);
			CCLuaLog("itemId = " .. itemId .. " num = " .. num);
			num =  MainMenuLayer:GetNumByFormat(tonumber(num))
			container[itemId] = {count=""..num,};
		end
	end
end

function OnlineRewardUI:Init()
	local bnt_close = self.LayerLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	bnt_close:registerScriptTapHandler(onOnlineRewardLayerCloseClick)

	self.reward_num_countdown = self.LayerLayout:FindChildObjectByName("reward_num_countdown").__UILabel__; 

	self.get_ui_button = self.LayerLayout:FindChildObjectByName("getBtn").__UIButton__;
	if countDown == 0 then
		self.get_ui_button:setEnabled(true);
	else
		self.get_ui_button:setEnabled(false);
	end
	
end

function OnlineRewardUI:setupContent()
	local get_btn = self.get_ui_button:getMenuItemSprite();
	get_btn:registerScriptTapHandler(onOnlineRewardLayerGetClick);

	local reward1_text = self.LayerLayout:FindChildObjectByName("reward1_text").__UILabel__;
	reward1_text:setString(LanguageLocalization:GetLocalization("M_ONLINE_AWARD_" .. (currLevel+1) ));
	
	local basename = "reward_";
	local btnname = "btn";
	local numname = "num";
	local index = 1;
	for	id, value in pairsByKeys(OnlineRewardData) do
		self:updateItemMessage(id, index, value.count);
		index = index + 1;
	end

	index = 7; --NOTE: preview items index start by 7
	for	id, value in pairsByKeys(OnlineRewardData1) do
		self:updateItemMessage(id, index, value.count);
		index = index + 1;
	end

	if index > 7 then --NOTE: preview text visible if has any item
		local label = self.LayerLayout:FindChildObjectByName("rewardpreview_text").__UILabel__;
		label:setVisible(true);
	end
end

function OnlineRewardUI:updateItemMessage(id, index, count)
		local button = self.LayerLayout:FindChildObjectByName("reward_btn" .. index).__UIButton__;
		button:setVisible(true);
		self.ListItemIds[index] = id;
		self.ListButtons[index] = button;

		local reward_btn = button:getMenuItemSprite();
		reward_btn:setTag(index);
		reward_btn:registerScriptTapHandler(onOnlineRewardItemButtonClick)
		
		local iconSprite = ItemManager:Get():getIconSpriteById(id);
		button:setIconSprite(iconSprite)

		--local itemInfo = ItemManager:Get():getItemInfoById(id);
		--if nil ~= itemInfo then
		--	if itemInfo.m_quality > 0 then
		--		local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality);
		--		if nil ~= framePic then
		--			iconSprite:addChild(framePic);
		--			framePic:setPosition(ccp(iconSprite:getContentSize().width * 0.5,iconSprite:getContentSize().height * 0.5));
		--		end
		--	end
		--end

		local label = self.LayerLayout:FindChildObjectByName("reward_num" .. index).__UILabel__;
		label:setString(count);
		label:setVisible(true);
		--label:getLabelTTF():setAnchorPoint(ccp(1,0.5));
end


function updateOnlineRewardCountDown(remainTime, timeStr)
	if nil ~= onlineRewardLayer.reward_num_countdown then
		onlineRewardLayer.reward_num_countdown:setString(timeStr);
	end

	if nil ~= onlineRewardLayer.get_ui_button and remainTime <= 0 then --可领取
		onlineRewardLayer.get_ui_button:setEnabled(true);	
	end
end

local function getOnlineRewardTimeLabel()
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local label = nil
	if topLayout ~= nil then
		local control = topLayout:FindChildObjectByName("olreward_time")
		if control ~= nil then
			label = control.__UILabel__
		end
	end
	return label
end

local function onUpdateOnlineRewardTime(remainTime)
	--CCLuaLog("remainTime = " .. remainTime);
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if topLayout == nil then
		return
	end

	local timeStr = string.format("%02d:%02d", math.mod(math.floor(remainTime/60), 60), math.mod(remainTime, 60));
	local olTimeLabel = getOnlineRewardTimeLabel();
	if olTimeLabel ~= nil then
		olTimeLabel:setString(timeStr);
	end

	if onlineRewardLayer ~= nil then
		updateOnlineRewardCountDown(remainTime, timeStr);	
	end
	
	if remainTime <= 0 then
		countDown = 0;
		remainTime = 0;
		if not isShowTip then
			--Note: show tip
			local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
			if topLayout ~= nil then --have mainmenu
				local onlineRewardButtonNode = topLayout:FindChildObjectByName("button_onlinereward").__UIButton__;
				if onlineRewardButtonNode ~= nil then
					onlineRewardButtonNode:showTipIcon();
					isShowTip = true;
				end
			end
		end
	elseif isShowTip then
		--Note: hide tip
		local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
		if topLayout ~= nil then --have mainmenu
			local onlineRewardButtonNode = topLayout:FindChildObjectByName("button_onlinereward").__UIButton__;
			if onlineRewardButtonNode ~= nil then
				onlineRewardButtonNode:hideTipIcon();
				isShowTip = false;
			end
		end
	end

end

local function renewOnlineRewardTimer()
	local nowTime = TimeManager:Get():getCurServerTime()
	local seconds = countDown;
	if (TimeManager:Get():hasTimer(TIMER_ONLINEREWARD_COUNTDOWN)) then
		TimeManager:Get():renewTimer(TIMER_ONLINEREWARD_COUNTDOWN, nowTime + seconds)
		CCLuaLog("--------renew onlinereward timer : " .. seconds)
	else
		onlineRewardTimer = TimeManager:Get():registerLuaTimer(onUpdateOnlineRewardTime, TIMER_ONLINEREWARD_COUNTDOWN, nowTime + seconds)
		CCLuaLog("--------init onlinereward timer : " .. seconds)
	end

end

local function onMessageAskOnlineGiftRsp(msg)
	currLevel = msg.award_id;--在线奖励重数，从0开始
	countDown = msg.time;--领取时间
	CCLuaLog("onMessageAskOnlineGiftRsp currLevel " .. currLevel .. " countDown " .. countDown);
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local olTimeLabel = getOnlineRewardTimeLabel();
	if topLayout ~= nil and olTimeLabel ~= nil then --have mainmenu
		local onlineRewardButtonNode = topLayout:FindChildObjectByName("button_onlinereward").__UIButton__;
		
		if countDown >= 0 then --start timer and countdown
			renewOnlineRewardTimer();
			onlineRewardButtonNode:setVisible(true);
			olTimeLabel:setVisible(true);
			if countDown == 0 then
				onlineRewardButtonNode:showTipIcon();
				isShowTip = true;
			else
				onlineRewardButtonNode:hideTipIcon();
				isShowTip = false;
			end
		else --no more online rewards
			onlineRewardButtonNode:setVisible(false);
			olTimeLabel:setVisible(false);
		end
	end
end

local function onMessageGetOnlineGiftRsp(msg)
	CCLuaLog("onMessageGetOnlineGiftRsp res = " .. msg.res);
	if msg.res == GET_ON_GIFT_SUCC then --success
		CCLuaLog("success");
		GameApi:showMessage(GetLuaLocalization("M_GET_SUCCESS"))
		sendOnlineRewardReq();
		onOnlineRewardLayerCloseClick(nil)
	end
end

function registOnlineRewardRspAndSendReq()
	if not initedTimer then
		CCLuaLog("registOnlineRewardRspAndSendReq");
		addMsgCallBack("CSAskOnlineGiftRsp", onMessageAskOnlineGiftRsp)
		addMsgCallBack("CSGetOnlineGiftRsp", onMessageGetOnlineGiftRsp)
		notification = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
		initedTimer = true;
	end

	sendOnlineRewardReq();
	return initedTimer;
end

