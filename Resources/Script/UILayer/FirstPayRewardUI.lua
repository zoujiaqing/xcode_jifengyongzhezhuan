require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/UILayer/TopUp_Award")

local isShowTip = false;
local notification = nil;
local firstPayRewardLayer = nil;
local initMsgCallback = false;
 first_pay_currAwardId = nil;--当前的首充奖励
 first_pay_currState = nil;--当前的首充状态 

local FirstPayRewardData = {};

FirstPayRewardUI = {
	LayerInstance = nil, 
	LayerLayout = nil,
	get_ui_button = nil;
	get_ui_label = nil;

	ListButtons = {},
	ListItemIds = {},
}

function FirstPayRewardUI:ResetValue()
	self.LayerInstance = nil;
	self.LayerLayout = nil;
	self.get_ui_button = nil;
	self.get_ui_label = nil;

	self.ListButtons = {};
	self.ListItemIds = {};
end

local function onReceiveDestructorHandler()
	CCLuaLog("FirstPayRewardUI destructor");
	TXGUI.UIManager:sharedManager():removeUILayout("FirstPayRewardUI");
	FirstPayRewardUI:ResetValue();
	if nil ~= notification then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notification);
		notification = nil;
	end
end

function FirstPayRewardUI:CreateLayer(parentLayer)
	CCLuaLog("FirstPayRewardUI:CreateLayer");
	self:ResetValue();

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create();
	else
		self.LayerInstance = parentLayer;
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler);

	self.LayerLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FirstPayRewardUI.plist", self.LayerInstance, "FirstPayRewardUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode();
		local tag = UITagTable["FirstPayRewardLayer"].tag;
		mainMenuNode:addChild(self.LayerInstance, 0, tag);
	end

	PushOneLayer(self.LayerInstance, "", "");
	--registTopupRewardRsp()
	self:InitData();
	self:Init();
	self:setupContent();

	firstPayRewardLayer = self;
end

local function onFirstPayRewardLayerCloseClick(pObj)
	CCLuaLog("onFirstPayRewardLayerCloseClick");
	if nil ~= FirstPayRewardUI.LayerInstance then
		RemoveOneLayer(FirstPayRewardUI.LayerInstance);
		CCTextureCache:sharedTextureCache():removeUnusedTextures();
		FirstPayRewardUI.LayerInstance:removeFromParentAndCleanup(true);
		--FirstPayRewardUI.LayerInstance = nil;
		--FirstPayRewardUI:ResetValue();
	end
end

local function onFirstPayRewardLayerPayClick(pObj)
	CCLuaLog("onFirstPayRewardLayerPayClick");
	local Power=nil
	if first_pay_currAwardId ~= nil then
		local data = TopUp_Award[first_pay_currAwardId];
		if data ~= nil and first_pay_currState == WAITTING_FOR_TOPUP then
			Power = data.Power
		end
	end
	buyDiamondUI:CreateLayer(nil, Power);
	onFirstPayRewardLayerCloseClick(pObj);
end

local function closeCallBack(message)
	if nil ~= FirstPayRewardUI.LayerInstance then
		if G_CurLayerInstance ~= FirstPayRewardUI.LayerInstance then
			onFirstPayRewardLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		onFirstPayRewardLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

local function onFirstPayRewardItemButtonClick(index)
	CCLuaLog("onFirstPayRewardItemButtonClick: index is " .. index);
	local item_id = FirstPayRewardUI.ListItemIds[index];
	local iconbtn = FirstPayRewardUI.ListButtons[index]
	local menu = iconbtn:getMenuNode()
	local pos = menu:getTouchPoint()
	ItemManager:Get():showItemTipsById(item_id, pos)
end

function FirstPayRewardUI:InitData()
	FirstPayRewardData = {};
	--取出在线奖励数据

	local data = TopUp_Award[first_pay_currAwardId];
	if data ~= nil then
		self:insertDataToRewardDataList(FirstPayRewardData, data.Award);
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

-------------------------------------------------------
-- 参数:待分割的字符串,分割字符
-- 返回:子串表.(含有空串)
local function lua_string_split(str, split_char)
    local sub_str_tab = {};
    while (true) do
        local pos = string.find(str, split_char);
        if (not pos) then
            sub_str_tab[#sub_str_tab + 1] = str;
            break;
        end
        local sub_str = string.sub(str, 1, pos - 1);
        sub_str_tab[#sub_str_tab + 1] = sub_str;
        str = string.sub(str, pos + 1, #str);
    end

    return sub_str_tab;
end

function FirstPayRewardUI:insertDataToRewardDataList(container, str)
	if container == nil or str == nil or str == '0' then
		return
	end

	local data = lua_string_split(str, ';');

	local itemId, num, itemStr;
	for i=1, #data do
		itemStr = data[i]
		if itemStr ~= nil and itemStr ~= '0' then
			itemId, num = getItemIdAndNums(itemStr);
			num =  MainMenuLayer:GetNumByFormat(tonumber(num))
			container[itemId] = {count=""..num,};
		end
	end
end

function FirstPayRewardUI:Init()
	local bnt_close = self.LayerLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	bnt_close:registerScriptTapHandler(onFirstPayRewardLayerCloseClick)

	local bnt_pay = self.LayerLayout:FindChildObjectByName("payBtn").__UIButton__:getMenuItemSprite();
	bnt_pay:registerScriptTapHandler(onFirstPayRewardLayerPayClick)
	
	self.get_ui_button = self.LayerLayout:FindChildObjectByName("getBtn").__UIButton__;
	self.get_ui_label = self.LayerLayout:FindChildObjectByName("getText").__UILabel__;

	notification = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
	
end

function FirstPayRewardUI:setupContent()
	local get_btn = self.get_ui_button:getMenuItemSprite();
	get_btn:registerScriptTapHandler(self.onGetButtonClick);

	--add image
	local award = TopUp_Award[first_pay_currAwardId];
	if award == nil then
		return
	end

	local framePic = award.Base_Map;
		if nil ~= framePic then
		local pic = self.LayerLayout:FindChildObjectByName("framePic").__UIPicture__;
		local newsprite = CCSprite:create("UI/" .. framePic .. ".png");
		if pic ~= nil and newsprite ~= nil then
			--pic:setSprite(newsprite);
			newsprite:setPosition(ccp(newsprite:getContentSize().width * 0.5,newsprite:getContentSize().height * 0.5));
			pic:getCurrentNode():addChild(newsprite);
		end
	end

	--add text
	--local text1 = self.LayerLayout:FindChildObjectByName("text_content1").__UILabel__;
	--text1:setString(GetLuaLocalization("M_GET_SUCCESS"));	
	local text2 = self.LayerLayout:FindChildObjectByName("text_content2").__UILabel__;
	text2:setString(GetLuaLocalization(award.Headline_Text));	
	local text3 = self.LayerLayout:FindChildObjectByName("text_content3").__UILabel__;
	text3:setString(GetLuaLocalization(award.Content_Text));	

	--add item
	local index = 1;
	for	id, value in pairsByKeys(FirstPayRewardData) do
		self:updateItemMessage(id, index, value.count);
		index = index + 1;
	end
end

function FirstPayRewardUI:onGetButtonClick(pObj)
	CCLuaLog("onFirstPayGetButtonClick");
	--只有可以领取状态才可响应
	if first_pay_currState == nil or first_pay_currAwardId == nil then
		return
	end
	
	if first_pay_currState ~= DONE_TOPUP_NOT_GET_AWARD then
		GameApi:showMessage(GetLuaLocalization("M_UNFINISH_FRISTCHARGE"))
		return;
	end

	req = {
		id = first_pay_currAwardId 
	}
	local msgname="CSGetTopupAwardReq"
	local ret = send_message(msgname, req , true)
	return ret;
end

function FirstPayRewardUI:updateItemMessage(id, index, count)
		local button = self.LayerLayout:FindChildObjectByName("reward_btn" .. index).__UIButton__;
		button:setVisible(true);
		self.ListItemIds[index] = id;
		self.ListButtons[index] = button;

		local reward_btn = button:getMenuItemSprite();
		reward_btn:setTag(index);
		reward_btn:registerScriptTapHandler(onFirstPayRewardItemButtonClick)
		
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
end

----------------------------通信----------------------------
--[[

enum TOPUP_AWARD_STATUS {
	NONE_TOPUP_AWARD         = 0;  // 当前没有首充奖励活动
	DONE_TOPUP_NOT_GET_AWARD = 1;  // 玩家已经完成首充，但未领奖
	DONE_TOPUP_AND_GET_AWARD = 2;  // 玩家完成首充，并且已经领取奖励
	WAITTING_FOR_TOPUP       = 3;  // 玩家没有进行首充 
}

--]]
local function onMsgCheckTopupAwardRsp(msg)
	--可领与未领显示按钮
	if msg.status == DONE_TOPUP_NOT_GET_AWARD or msg.status == WAITTING_FOR_TOPUP then
		first_pay_currAwardId = msg.id;
	else
		first_pay_currAwardId = nil;
	end
	first_pay_currState = msg.status;
	
	--first_pay_currAwardId = nil
	--first_pay_currState = DONE_TOPUP_AND_GET_AWARD

	CCLuaLog("onMsgCheckTopupAwardRsp state = " .. first_pay_currState);
	if first_pay_currAwardId~= nil then
		CCLuaLog("id is :"..first_pay_currAwardId);
	end
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local button = topLayout:FindChildObjectByName("button_firstpay").__UIButton__;
	if first_pay_currAwardId ~= nil then -- show topup award button on mainmenu
		button:setVisible(true);
		if first_pay_currState == DONE_TOPUP_NOT_GET_AWARD then
			button:showTipIcon();
		end
	else
		button:setVisible(false);
		MainMenuLayer:getMainMenuLayer():HideFirstChargeEffect()
	end
end

function getFirstPayStatus()
	print("first_pay_currState = " .. first_pay_currState)
	return first_pay_currState
end

function getFirstPayAwardId()
	print("first_pay_currAwardId = " .. first_pay_currAwardId)
	return first_pay_currAwardId
end

local function onMsgGetTopupAwardRsp(msg)
	if msg.result == GET_TOPUP_AWARD_SUCCESS then
		local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
		local button = topLayout:FindChildObjectByName("button_firstpay").__UIButton__;
		if button ~= nil then 
			button:setVisible(false);
			MainMenuLayer:getMainMenuLayer():HideFirstChargeEffect()
			GameApi:showMessage(GetLuaLocalization("M_GET_SUCCESS"))

			first_pay_currAwardId = -1;--重置
			first_pay_currState = -1;
			if firstPayRewardLayer ~= nil then--改变按钮状态
				firstPayRewardLayer.get_ui_button:setEnabled(false);
				firstPayRewardLayer.get_ui_label:setString(GetLuaLocalization("M_LOGINREWARD_ALREADYGOT"));
			end
		end
		
	end
end

local function sendCheckTopupAwardRep()
	req = {};
	local msgname="CSCheckTopupAwardReq"
	local ret = send_message(msgname, req , true)
	return ret;
end

--注册回调
function registTopupRewardRsp()
	if not initMsgCallback then
		CCLuaLog("registTopupRewardRsp");
		addMsgCallBack("CSCheckTopupAwardRsp", onMsgCheckTopupAwardRsp);
		addMsgCallBack("CSGetTopupAwardRsp", onMsgGetTopupAwardRsp);
		initMsgCallback = true;
	end
	sendCheckTopupAwardRep();
end
