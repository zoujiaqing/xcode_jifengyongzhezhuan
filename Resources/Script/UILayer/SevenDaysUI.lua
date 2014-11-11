require("Script/bit")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/GameConfig/vip")
local notification = nil;
SevenDaysData = {
	initMsgHandle = false,
	currDay = 0, --当前累计登录天数
	markBits = 0, --七日领取标识,按位处理, 1 << 1 ... 1 << 7 ,注：1为已领取, 0为未领取
	selectDay = 0, --当前选中哪天
	markBitsVip = 0,
	rewardData = {} --rewardData[day] = {{[itemId] = {num}},...} 奖励数据
};


SevenDaysUI = {
	LayerInstance = nil, 
	LayerLayout = nil,
	getButton = nil,
	ListButtons = {},
	ListItemIds = {},
	ListButtonsMenuItems = {},
	ListDayButtons = {},
	ListDaySelectIcon = {},
	ListDayButtonsMenuItems = {},
	ListDayGotIcon = {},
}

function SevenDaysUI:ResetValue()
	self.LayerInstance = nil;
	self.LayerLayout = nil;	
	self.getButton = nil;
	
	self.ListButtons = {};
	self.ListItemIds = {};
	self.ListButtonsMenuItems = {};
	self.ListDayButtons = {};
	self.ListDaySelectIcon = {};
	self.ListDayButtonsMenuItems = {};
	self.ListDayGotIcon = {};
end
function SevenDaysUI:clearData()
	SevenDaysData.initMsgHandle = false
	SevenDaysData.currDay = 0 --当前累计登录天数
	SevenDaysData.markBits = 0 --七日领取标识,按位处理, 1 << 1 ... 1 << 7 ,注：1为已领取, 0为未领取
	SevenDaysData.markBitsVip = 0 
	SevenDaysData.selectDay = 0 --当前选中哪天
	SevenDaysData.rewardData = {} --rewardData[day] = {{[itemId] = {num}},...} 奖励数据
end
local function onReceiveDestructorHandler()
	CCLuaLog("SevenDaysUI destructor");
	TXGUI.UIManager:sharedManager():removeUILayout("SevenDaysUI");
	SevenDaysUI:ResetValue();
	--[[if nil ~= notification then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notification);
		notification = nil;
	end
	--]]
end

function SevenDaysUI:CreateLayer(parentLayer)
	CCLuaLog("SevenDaysUI:CreateLayer");
	self:ResetValue();

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create();
	else
		self.LayerInstance = parentLayer;
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler);

	self.LayerLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/SevenDaysUI.plist", self.LayerInstance, "SevenDaysUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode();
		local tag = UITagTable["SevenDaysLayer"].tag;
		mainMenuNode:addChild(self.LayerInstance, 0, tag);
	end

	PushOneLayer(self.LayerInstance, "", "");

	self:InitData();
	self:Init();
	self:setupContent();

end

local function onSevenDaysLayerCloseClick(pObj)
	CCLuaLog("onSevenDaysLayerCloseClick");
	if nil ~= SevenDaysUI.LayerInstance then
		RemoveOneLayer(SevenDaysUI.LayerInstance);
		CCTextureCache:sharedTextureCache():removeUnusedTextures();
		SevenDaysUI.LayerInstance:removeFromParentAndCleanup(true);
		XLogicManager:sharedManager():notifyShowNextAppearUI();
		TipMgr:Get():destroyTip()
		--SevenDaysUI.LayerInstance = nil;
		--SevenDaysUI:ResetValue();
	end
end

local function closeCallBack(message)
	if nil ~= SevenDaysUI.LayerInstance then
		if G_CurLayerInstance ~= SevenDaysUI.LayerInstance then
			onSevenDaysLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		onSevenDaysLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

local function isSevenDayCanGetAward(day)
CCLuaLog("isSevenDayCanGetAward mark bit "..tostring(SevenDaysData.markBits))
	local markBool = bit:_and(bit:_not(SevenDaysData.markBits), bit:_lshift(1,day) ) > 1 
	local dayBool = SevenDaysData.currDay >= day;
	CCLuaLog("markBool = " .. tostring(markBool) .. " dayBool .. " .. tostring(dayBool));
	return dayBool and markBool, not markBool;
end
--add for vip award jackniu 2014-5-1
local function isSevenDayCanGetAwardVip(day)
CCLuaLog("isSevenDayCanGetAwardVip mark bit vip".. tostring(SevenDaysData.markBitsVip))

	local markBool = bit:_and(bit:_not(SevenDaysData.markBitsVip), bit:_lshift(1,day) ) > 1 
	local dayBool = SevenDaysData.currDay >= day;
	
	return dayBool and markBool, not markBool;
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

function SevenDaysUI:insertDataToRewardDataList(container, data)
	if container == nil or data == nil then
		return
	end

	local itemId=0
	local num=0
	local itemStr
	local index = 1;
	for i=1, 11 do
		itemStr = data["RaidDrops" .. i]
		if itemStr ~= nil  and itemStr ~= '0' then
			
			itemId, num = getItemIdAndNums(itemStr);
			
			CCLuaLog("itemId = " .. itemId .. " num = " .. num);
			--[[
			if num > 1000 then
				num = num / 1000
				num = num .. "k";
			end
			--]]
			num =  MainMenuLayer:GetNumByFormat(tonumber(num))
			container[#container+1] = {itemId=itemId, count=""..num,};
			index = index + 1;
		end
	end
end

function SevenDaysUI:InitData()
	SevenDaysData.rewardData = {};
	
	SevenDaysData.selectDay = SevenDaysData.currDay;
	--从每日领取奖历表中取出七日奖励数据
	for i = 1, 10 do 
		SevenDaysData.rewardData[""..i] = {};
	end
	for	id, value in pairsByKeys(AwardDaily) do
		if  value.Type1 == '8' or value.Type1 == '11' then
			CCLuaLog("sevendays ui type ,"..value.Type1.. "id = " .. id .. " day = " .. value.Type2);
			
			self:insertDataToRewardDataList(SevenDaysData.rewardData[value.Type2], value);
		end
	end
end
function SevenDaysUI:getEnabeReWard()
	self:InitData();
	local enabledBool, getBool = isSevenDayCanGetAward(SevenDaysData.selectDay);
	return enabledBool
end
function SevenDaysUI:Init()
	local bnt_close = self.LayerLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	bnt_close:registerScriptTapHandler(onSevenDaysLayerCloseClick)

	self.getButton = self.LayerLayout:FindChildObjectByName("getBtn").__UIButton__;
	self.getButtonVip = self.LayerLayout:FindChildObjectByName("getBtnVip").__UIButton__;
	self.buyDiamondBtn = self.LayerLayout:FindChildObjectByName("buyDiamondBtn").__UIButton__;
	self.buyDiamondBtn:getMenuItemSprite():registerScriptTapHandler(function(pobj)
		--print("sssssssssssss")
		MainMenuLayer:getMainMenuLayer():ShowChargeLayer(true)		
		onSevenDaysLayerCloseClick()
	end)
	local enabledBool, getBool = isSevenDayCanGetAward(SevenDaysData.selectDay);
	self.getButton:setEnabled(enabledBool);
	self.getButton:getMenuItemSprite():registerScriptTapHandler(SevenDaysUI.onGetButtonCallBack)
	
	enabledBool, getBool = isSevenDayCanGetAwardVip(SevenDaysData.selectDay);
	self.getButtonVip:setEnabled(enabledBool);
	self.getButtonVip:getMenuItemSprite():registerScriptTapHandler(SevenDaysUI.onGetButtonVipCallBack)
	--累计登录天数
	--local label = self.LayerLayout:FindChildObjectByName("text_logindays").__UILabel__;
	--label:setString(LanguageLocalization:GetLocalization("M_SEVEN_LANDING_1") .. SevenDaysData.currDay .. LanguageLocalization:GetLocalization("M_SEVEN_LANDING_2"));

	--初始化进度条
	local rate = SevenDaysData.currDay / 8.0;
	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
	rate = rate * scale;
	local bar = self.LayerLayout:FindChildObjectByName("bar").__UIPicture__;
	if bar ~= nil then
		bar:getCurrentNode():setAnchorPoint(ccp(0,0.5))
		local tmpSize = bar:getSpriteSize()
        local oldPos = bar:getPosition()
		bar:setPosition(ccp(oldPos.x-tmpSize.width/2,oldPos.y))
		bar:setScale(rate, 1);
	end

	--进度条文字颜色改变
	for i = 1, 8 do
		local label = self.LayerLayout:FindChildObjectByName("text_daynum" .. i).__UILabel__;
		if label ~= nil then
			if SevenDaysData.currDay >= i then
				label:setColor(ccc3(255,255,255));
			else
				label:setColor(ccc3(116,113,119));
			end
		end
	end
	
	notification = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
end

function SevenDaysUI:setupContent()
	CCLuaLog("SevenDaysUI:setupContent() selectDay = " .. SevenDaysData.selectDay);
	local scrollList = self.LayerLayout:FindChildObjectByName("scrollList").__UIScrollList__
	if nil == scrollList or SevenDaysData.selectDay == 0 then
		return 
	end
	
	for i = 8, 1, -1 do
		self:insertItemToList(scrollList, i);
	end

	self:adjustScrollListStartOffset(scrollList);
	
	self:updateShowBySelectDay(SevenDaysData.currDay);
end


function SevenDaysUI:adjustScrollListStartOffset(scrollList)
	if nil == scrollList or SevenDaysData.currDay == 0 then
		return
	end

	CCLuaLog("contentsize width = " .. scrollList:getContentSize().width .. " viewsize width = " .. scrollList:getViewSize().width);
	local itemWidth =  scrollList:getContentSize().width  / 8.0; --单个按钮宽度
	
	local itemArea = "map_ui_7day_FRAME_7DAY_DAY1_REWARDBACK";
	local strBin = "UI/ui_7day.bin"
	local sprite = AspriteManager:getInstance():getOneFrame(strBin,itemArea)
	if sprite ~= nil then
		local adjustWidth = itemWidth * (9-SevenDaysData.currDay); --当前天数显示长度
		if adjustWidth > scrollList:getViewSize().width then --偏移可见区的量
			scrollList:setStartOffset(ccp(adjustWidth - scrollList:getViewSize().width, 0));
		end
	end
end

local function onSevenDaysRewardItemButtonClick(index)
	CCLuaLog("SevenDaysUI:onRewardItemButtonClick: index is " .. index);
	local item_id = SevenDaysUI.ListItemIds[index];
	local iconbtn = SevenDaysUI.ListButtons[index]
	local menu = iconbtn:getMenuNode()
	local pos = menu:getTouchPoint()
	ItemManager:Get():showItemTipsById(item_id, pos)
end

function SevenDaysUI:onGetButtonCallBack()
	CCLuaLog("onGetButtonCallBack selectDay = " .. SevenDaysData.selectDay );

	local enabledBool, getBool = isSevenDayCanGetAward(SevenDaysData.selectDay);
	if enabledBool then
		CSGetLoginRewardReq = {
			someday = SevenDaysData.selectDay
		};
		local msgName = "CSGetLoginRewardReq";
		send_message(msgName, CSGetLoginRewardReq, true)
	end
end

local function confirmToBuyDimond(obj)
  MainMenuLayer:getMainMenuLayer():ShowChargeLayer(true)
end

function SevenDaysUI:onGetButtonVipCallBack()

	CCLuaLog("onGetButtonVipCallBack selectDay = " .. SevenDaysData.selectDay );
	
	
	local vip = UserData:GetUserInfo().m_vip
	if vip < 1 then
		--MessageBox:Show("您当前vip等级不足，现在成为vip即可领取丰厚七日奖励;",confirmToBuyDimond,nil,MB_YESNO,ccc3(255,255,255),26)
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_7DAY_VIPNOTENOUGH"));
		return
	end
	CSGetLoginRewardReq = {
			someday = SevenDaysData.selectDay+1000
	};
	local msgName = "CSGetLoginRewardReq";
	send_message(msgName, CSGetLoginRewardReq, true)
	
end

function SevenDaysUI:setMenuItemSelected(day,isSelected)
	local button = self.ListDayButtonsMenuItems[day]
	local selectIcon = self.ListDaySelectIcon[day]
	if isSelected then
		if selectIcon ~= nil then
			selectIcon:setVisible(true);
		end
		if button ~= nil then
			button:setVisible(false);
		end
	else
		if selectIcon ~= nil then
			selectIcon:setVisible(false);
		end
		if button ~= nil then
			button:setVisible(true);
		end
	end
end

--更新已领取标记
function SevenDaysUI:updateGotMarkShow(day)
	local gotIcon = self.ListDayGotIcon[day];
	if gotIcon ~= nil then 
		local enabledBool, getBool = isSevenDayCanGetAward(day);
		local enabledBoolvip, getBoolvip = isSevenDayCanGetAwardVip(day);
		gotIcon:setVisible(getBool and getBoolvip);
	end
end

function SevenDaysUI:updateShowBySelectDay(sd)
	--更新大图选中与是否已领取图片显示
	for	day, button in pairsByKeys(self.ListDayButtonsMenuItems) do
		CCLuaLog("updateShowBySelectDay day = " .. day .. " sd = " .. sd);
		if day ~= nil and button ~= nil then
			if (day==sd) then
				self:setMenuItemSelected(day,true);
			else
				self:setMenuItemSelected(day,false);
			end

			self:updateGotMarkShow(day);
		end
	end

	--更新选中天数
--	local label = self.LayerLayout:FindChildObjectByName("text_day").__UILabel__;
	--label:setString(LanguageLocalization:GetLocalization("M_SEVEN_DAY_" .. sd) .. ":");

	--更新物品显示
	self:hideRewardItems();
	for	day, value in pairsByKeys(SevenDaysData.rewardData) do
		if day ~= nil and value ~= nil and day == tostring(sd) then --找到选择的天数物品数据，并更新显示
			for	index, value2 in pairsByKeys(value) do
				self:updateItemMessage(value2.itemId, index, value2.count);
			end
		end
	end

	--更新领取按钮
	local enabledBool, getBool = isSevenDayCanGetAward(SevenDaysData.selectDay);
	CCLuaLog("update ...."..tostring(enabledBool))
	self.getButton:setEnabled(enabledBool);

	--改变按钮文字
	local btn_gettext = self.LayerLayout:FindChildObjectByName("btn_gettext").__UILabel__;
	if btn_gettext ~= nil then
		if getBool then --已领取
			btn_gettext:setString( LanguageLocalization:GetLocalization("M_LOGINREWARD_ALREADYGOT") );
		else --领取奖励
			if enabledBool==false and getBool == false then
			btn_gettext:setString( "改天来领" );
			else
			btn_gettext:setString( LanguageLocalization:GetLocalization("Dailytask_WaitReward") );
			end
		end
	end
	enabledBool, getBool = isSevenDayCanGetAwardVip(SevenDaysData.selectDay);
	self.getButtonVip:setEnabled(enabledBool);
	--改变vip按钮文字
	btn_gettext = self.LayerLayout:FindChildObjectByName("btn_gettextVip").__UILabel__;
	if btn_gettext ~= nil then
		if getBool then --已领取
			btn_gettext:setString( LanguageLocalization:GetLocalization("M_LOGINREWARD_ALREADYGOT") );
		else --领取奖励
			if enabledBool==false and getBool == false then
			btn_gettext:setString( "改天来领" );
			else
			btn_gettext:setString( "VIP领取" );
			end
		end
	end
	--更新进度条显示
	--箭头
	for i = 1, 8 do
		local arrow = self.LayerLayout:FindChildObjectByName("bar_mark" .. i).__UIPicture__;
		if arrow ~= nil then
			if i == sd then
				arrow:setVisible(true);
			else
				arrow:setVisible(false);
			end
		end
	end
	
end

function SevenDaysUI:hideRewardItems()
	for index = 1, 6 do
		local button = self.LayerLayout:FindChildObjectByName("reward_btn" .. index).__UIButton__;
		if button ~= nil then
			button:setVisible(false);
		end

		local label = self.LayerLayout:FindChildObjectByName("reward_num" .. index).__UILabel__;
		if label ~= nil then
			label:setVisible(false);
		end
	end
end

function SevenDaysUI:updateItemMessage(id, index, count)
	CCLuaLog("updateItemMessage id = " .. id .. " index = " .. index .. " count = " .. count);
		if id == 0 then
		return;
		end
		local button = self.LayerLayout:FindChildObjectByName("reward_btn" .. index);
		if button == nil then
			return
		end
		button = button.__UIButton__;
		button:setVisible(true);
		self.ListItemIds[index] = id;
		self.ListButtons[index] = button;

		local reward_btn = button:getMenuItemSprite();
		reward_btn:setTag(index);
		reward_btn:registerScriptTapHandler(onSevenDaysRewardItemButtonClick)
		
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

local function onSevenDaySelectHandler(nTag)
	CCLuaLog("---------------------------------------SevenDaysUI:onDaySelectHandler nTag = " .. nTag);
	SevenDaysData.selectDay = nTag;
	SevenDaysUI:updateShowBySelectDay(SevenDaysData.selectDay);
end

function SevenDaysUI:insertItemToList(scrollList,index)
	CCLuaLog("insertItemToList index = " .. index);
	local itemArea = "map_ui_7day_FRAME_7DAY_DAY1_REWARDBACK";
	local itemSelectIcon = "map_ui_7day_FRAME_7DAY_DAY1_REWARDBACK_SELECT";
	local picIcon = "map_ui_7day_FRAME_7DAY_DAY1_REWARDPIC";
	local gotIcon = "map_ui_7day_FRAME_7DAY_DAY1_GOTMARK";
	local labelIcon = "map_ui_7day_FRAME_7DAY_DAY1_TEXT";
	local strBin = "UI/ui_7day.bin"
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	local normal = AspriteManager:getInstance():getOneFrame(strBin,itemArea)
	local clicked = AspriteManager:getInstance():getOneFrame(strBin,itemSelectIcon)
	local button = scrollList:AddNewItem(normal, clicked, "", "", ccc3(255,255,255), -132);
    button:setName("sevenDay"..index)
	local btnMenuItem = button:getMenuItemSprite()
	--Note: 定义回调接口
	self.ListDayButtons[index] = button
	self.ListDayButtonsMenuItems[index] = btnMenuItem
	btnMenuItem:setTag(index)
	btnMenuItem:registerScriptTapHandler(onSevenDaySelectHandler)

	--选中框
	local selectIcon = AspriteManager:getInstance():getOneFrame(strBin,itemSelectIcon)
	self.ListDaySelectIcon[index] = selectIcon
	button:getAnimationNode():addChild(selectIcon,9);
	selectIcon:setVisible(false);
	
	local picStr = "UI/ui_7day_reward" .. index .. ".png";
	local sprite =  CCSprite:create(picStr);
	if sprite == nil then
		return
	end
	--Note: 得到图片的相对位置
	AspriteManager:getInstance():getFramesRelativePos(strBin,itemArea,picIcon,anchorPoint,relativePoint);
	self:ScaleByScreen(relativePoint)
	sprite:setPosition(relativePoint);
	button:getAnimationNode():addChild(sprite,10);

	--Note: 文字
	local labelContent = AspriteManager:getInstance():getOneFrame(strBin,labelIcon)
	local content = LanguageLocalization:GetLocalization("M_SEVEN_DAY_" .. index);
	local nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, 16,labelContent:getContentSize(),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
	nameLabel:setColor(ccc3(255,150,0));
	AspriteManager:getInstance():getFramesRelativePos(strBin,itemArea,labelIcon,anchorPoint,relativePoint);
	self:ScaleByScreen(relativePoint)
	nameLabel:setPosition(relativePoint);
	button:getAnimationNode():addChild(nameLabel, 11);
	
	--已领取图片
	local gotSprite = AspriteManager:getInstance():getOneFrame(strBin,gotIcon)
	self.ListDayGotIcon[index] = gotSprite
	AspriteManager:getInstance():getFramesRelativePos(strBin,itemArea,gotIcon,anchorPoint,relativePoint);
	--self:ScaleByScreen(relativePoint)
	gotSprite:setPosition(relativePoint);
	button:getAnimationNode():addChild(gotSprite,11);
	gotSprite:setVisible(false);
	
	--vip icon
	for index = 5, 6 do
		local button = self.LayerLayout:FindChildObjectByName("reward_btn" .. index).__UIButton__;
		if button == nil then
			return
		end	
		local gotIcon="map_ui_7day_FRAME_7DAY_VIP"..index
		local gotSprite = AspriteManager:getInstance():getOneFrame(strBin,gotIcon)
		local itemArea = "map_ui_7day_FRAME_7DAY_FRAME_REWARD"..index;
		AspriteManager:getInstance():getFramesRelativePos(strBin,itemArea,gotIcon,anchorPoint,relativePoint);
		--self:ScaleByScreen(relativePoint)
		gotSprite:setPosition(relativePoint);
		button:getAnimationNode():addChild(gotSprite,11);
	end
	
end

function SevenDaysUI:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

--拉取登录累计天数与领取标识
local function sendSevenDaysAttrReq()
	CSAttributesReq = {
		keys = {
			PB_ATTR_GET_DAILY_LOGIN_TIMES,
			PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK,
			6103,
		}
	};
	local msgName = "CSAttributesReq";
	send_message(msgName, CSAttributesReq, true)
end

--获取属性
local function onMessageAttributesRsp(msg)
	CCLuaLog("onMessageAttributesRsp sevendays");
	for _,v in ipairs(msg.values) do
		if v.key == PB_ATTR_GET_DAILY_LOGIN_TIMES then
			--CCLuaLog("PB_ATTR_GET_DAILY_LOGIN_TIMES = " .. PB_ATTR_GET_DAILY_LOGIN_TIMES .. " value = " .. v.value);
			SevenDaysData.currDay = v.value;
			SevenDaysData.selectDay = SevenDaysData.currDay;

			if SevenDaysData.currDay > 8 then
				SevenDaysData.currDay = 8;
			elseif SevenDaysData.currDay <= 0 then --领完或不存在此活动
				local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
				if topLayout ~= nil then
					topLayout:FindChildObjectByName("button_7day").__UIButton__:setVisible(false);--隐藏主界面七日登录按钮
				end
			end
		elseif v.key == PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK then
			CCLuaLog("PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK = " .. PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK .. " value = " .. v.value);
			SevenDaysData.markBits = v.value;
			checkSevenDayButtonVisible()
		elseif v.key == 6103 then
			CCLuaLog("PB_ATTR_GET_DAILY_VIP_LOGIN_REWARD_MARK    = " .. PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK .. " value = " .. v.value);
			SevenDaysData.markBitsVip = v.value;
			checkSevenDayButtonVisible()
			--update button
			
		end
	end
end
function checkSevenDayButtonVisible()
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if topLayout ~= nil 
	and SevenDaysData.markBitsVip ~= nil and SevenDaysData.markBitsVip >= (254+256)  -- 1<<8+1<<7+....1<<1
	and SevenDaysData.markBits ~= nil and SevenDaysData.markBits >= (254+256)  then
		topLayout:FindChildObjectByName("button_7day").__UIButton__:setVisible(false);--隐藏主界面七日登录按钮
	end
print("hidden seven day icon2")
	
end
--获取奖励回调
local function onMessageGetLoginRewardRsp(msg)
	print("onMessageGetLoginRewardRsp succ = " .. tostring(msg.succ));
	if msg.succ == true then
		GameApi:showMessage(GetLuaLocalization("M_GET_SUCCESS"))
		SevenDaysUI:updateShowBySelectDay(SevenDaysData.selectDay);
	end
end

function registSevenDaysRewardRspAndSendReq()
	print("registSevenDaysRewardRspAndSendReq");
	if not SevenDaysData.initMsgHandle then
		addMsgCallBack("CSAttributesRsp", onMessageAttributesRsp)
		addMsgCallBack("CSGetLoginRewardRsp", onMessageGetLoginRewardRsp)
		SevenDaysData.initMsgHandle = true;
	end

	--if SevenDaysData.currDay == 0 then
		print("sendSevenDaysAttrReq");
		sendSevenDaysAttrReq();
	--end

	return SevenDaysData.initMsgHandle;
end

local function updateNotificationSevenDays(message)
			
        if message == GM_E_ENTER_MAINLAND or 
			message == GM_ATTR_VIP_LEVEL_CHANGE then
            registSevenDaysRewardRspAndSendReq()
        end

end

local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotificationSevenDays);
