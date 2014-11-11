require("Script/bit")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/DailyReward/dailyRewardMainLayerDataCenter")
local notification = nil;
ThreeDaysData = {
	initMsgHandle = false,
	currDay = 0, --当前累计登录天数
	markBits = 0, --七日领取标识,按位处理, 1 << 1 ... 1 << 7 ,注：1为已领取, 0为未领取
	selectDay = 0, --当前选中哪天
	rewardData = {} --rewardData[day] = {{[itemId] = {num}},...} 奖励数据
};


ThreeDaysUI = {
	LayerInstance = nil, 
	LayerLayout = nil,
	ThreeDaysBackNot1Pic = nil ,
	ThreeDaysBackNot2Pic = nil ,
	ThreeDaysBackNot3Pic = nil ,
	ThreeDaysBack1Pic = nil ,
	ThreeDaysBack2Pic = nil ,
	ThreeDaysBack3Pic = nil ,
	
	ThreeDaysSet1Pic = nil ,
	ThreeDaysSet2Pic = nil ,
	ThreeDaysSet3Pic = nil ,
	
	ThreeDays1Get1  = nil ,
	ThreeDays1Get2  = nil ,
	ThreeDays1Get3  = nil ,
	threeDays1GetNum1 = nil ,
	threeDays1GetNum2 = nil ,
	threeDays1GetNum3 = nil ,
	getBtn1 = nil ,
	reward_text1 = nil ,
	
	ThreeDays2Get1  = nil ,
	ThreeDays2Get2  = nil ,
	ThreeDays2Get3  = nil ,
	threeDays2GetNum1 = nil ,
	threeDays2GetNum2 = nil ,
	threeDays2GetNum3 = nil ,
	getBtn2 = nil ,
	reward_text2 = nil ,
	
	ThreeDays3Get1  = nil ,
	ThreeDays3Get2  = nil ,
	ThreeDays3Get3  = nil ,
	threeDays3GetNum1 = nil ,
	threeDays3GetNum2 = nil ,
	threeDays3GetNum3 = nil ,
	getBtn3 = nil ,
	reward_text3 = nil ,
	
	closeBtn = nil ,
}

function ThreeDaysUI:ResetValue()
	self.LayerInstance = nil;
	self.LayerLayout = nil;	
	ThreeDaysBackNot1Pic = nil ;
	ThreeDaysBackNot2Pic = nil ;
	ThreeDaysBackNot3Pic = nil ;
	ThreeDaysBack1Pic = nil ;
	ThreeDaysBack2Pic = nil ;
	ThreeDaysBack3Pic = nil ;
	
	ThreeDaysSet1Pic = nil ;
	ThreeDaysSet2Pic = nil ;
	ThreeDaysSet3Pic = nil ;
	
	ThreeDays1Get1  = nil ;
	ThreeDays1Get2  = nil ;
	ThreeDays1Get3  = nil ;
	threeDays1GetNum1 = nil ;
	threeDays1GetNum2 = nil ;
	threeDays1GetNum3 = nil ;
	getBtn1 = nil ;
	reward_text1 = nil ;
	
	ThreeDays2Get1  = nil ;
	ThreeDays2Get2  = nil ;
	ThreeDays2Get3  = nil ;
	threeDays2GetNum1 = nil ;
	threeDays2GetNum2 = nil ;
	threeDays2GetNum3 = nil ;
	getBtn2 = nil ;
	reward_text2 = nil ;
	
	ThreeDays3Get1  = nil ;
	ThreeDays3Get2  = nil ;
	ThreeDays3Get3  = nil ;
	threeDays3GetNum1 = nil ;
	threeDays3GetNum2 = nil ;
	threeDays3GetNum3 = nil ;
	getBtn3 = nil ;
	reward_text3 = nil ;
	
	closeBtn = nil ;
	--self:clearData() 
end
function clearThreeDaysData()
	ThreeDaysUI:clearData()
end
function ThreeDaysUI:clearData()
	ThreeDaysData.initMsgHandle = false
	ThreeDaysData.currDay = 0 --当前累计登录天数
	ThreeDaysData.markBits = 0 --七日领取标识,按位处理, 1 << 1 ... 1 << 7 ,注：1为已领取, 0为未领取
	ThreeDaysData.selectDay = 0 --当前选中哪天
	ThreeDaysData.rewardData = {} --rewardData[day] = {{[itemId] = {num}},...} 奖励数据
end
local function onReceiveDestructorHandler()
	CCLuaLog("ThreeDaysUI destructor");
	TXGUI.UIManager:sharedManager():removeUILayout("ThreeDaysUI");
	ThreeDaysUI:ResetValue();
	--[[if nil ~= notification then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notification);
		notification = nil;
	end
	--]]
end

function ThreeDaysUI:CreateLayer(parentLayer)
	CCLuaLog("ThreeDaysUI:CreateLayer");
	self:ResetValue();

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create();
	else
		self.LayerInstance = parentLayer;
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler);

	self.LayerLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ThreeDaysUI.plist", self.LayerInstance, "ThreeDaysUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode();
		local tag = UITagTable["ThreeDaysUI"].tag;
		mainMenuNode:addChild(self.LayerInstance, 0, tag);
	end

	PushOneLayer(self.LayerInstance, "", "");

	self:InitData();
	self:Init();
	self:setupContent();

end

local function onThreeDaysLayerCloseClick(pObj)
	CCLuaLog("onThreeDaysLayerCloseClick");
	if nil ~= ThreeDaysUI.LayerInstance then
		RemoveOneLayer(ThreeDaysUI.LayerInstance);
		CCTextureCache:sharedTextureCache():removeUnusedTextures();
		ThreeDaysUI.LayerInstance:removeFromParentAndCleanup(true);
		XLogicManager:sharedManager():notifyShowNextAppearUI();
		--SevenDaysUI.LayerInstance = nil;
		--SevenDaysUI:ResetValue();
	end
end

local function closeCallBack(message)
	if nil ~= ThreeDaysUI.LayerInstance then
		if G_CurLayerInstance ~= ThreeDaysUI.LayerInstance then
			onThreeDaysLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		onThreeDaysLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

local function isThreeDayCanGetAward(day)
	--local markBool = bit:_and(bit:_not(ThreeDaysData.markBits), bit:_lshift(1,day) ) > 1 
	--local dayBool = ThreeDaysData.currDay >= day;
	--CCLuaLog("markBool = " .. tostring(markBool) .. " dayBool .. " .. tostring(dayBool));
	 local dayBool = (ThreeDaysData.currDay ~= 0) and (day == ThreeDaysData.currDay);
	 local markBool = ThreeDaysData.markBits == 0 ;
	 CCLuaLog("markBool = " .. tostring(markBool) .. " dayBool .. " .. tostring(dayBool));
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

function ThreeDaysUI:insertDataToRewardDataList(container, data)
	if container == nil or data == nil then
		return
	end

	local itemId, num, itemStr;
	local index = 1;
	for i=1, 11 do
		itemStr = data["RaidDrops" .. i]
		if itemStr ~= nil and itemStr ~= '0' then
			itemId, num = getItemIdAndNums(itemStr);
			CCLuaLog("itemId = " .. itemId .. " num = " .. num);
			if num > 1000 then
				num = num / 1000
				num = num .. "k";
			end
			container[index] = {itemId=itemId, count=""..num,};
			if index >= 3 then
			 break
			end
			index = index + 1;
		end
	end
end

function ThreeDaysUI:InitData()
	ThreeDaysData.rewardData = {};
	
	ThreeDaysData.selectDay = ThreeDaysData.currDay;
	--从每日领取奖历表中取出七日奖励数据
	for	id, value in pairsByKeys(AwardDaily) do
		if value ~= nil and value.Type1 == '2' then
			CCLuaLog("threedays ui id = " .. id .. " day = " .. value.Type2);
			if value.Type2 >= '1'  and value.Type2 <= '3' then
			ThreeDaysData.rewardData[value.Type2] = {};
			self:insertDataToRewardDataList(ThreeDaysData.rewardData[value.Type2], value);
			end
		end
	end
end
function ThreeDaysUI:getEnabeReWard()
	self:InitData();
	local enabledBool, getBool = isThreeDayCanGetAward(ThreeDaysData.selectDay);
	return enabledBool
end
function ThreeDaysUI:Init()


    ThreeDaysBackNot1Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysBackNot1Pic").__UIPicture__ ;
	ThreeDaysBackNot2Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysBackNot2Pic").__UIPicture__  ;
	ThreeDaysBackNot3Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysBackNot3Pic").__UIPicture__  ;
	ThreeDaysBack1Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysBack1Pic").__UIPicture__  ;
	ThreeDaysBack2Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysBack2Pic").__UIPicture__  ;
	ThreeDaysBack3Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysBack3Pic").__UIPicture__  ;
	
	ThreeDaysSet1Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysSet1Pic").__UIPicture__  ;
	ThreeDaysSet2Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysSet2Pic").__UIPicture__  ;
	ThreeDaysSet3Pic = self.LayerLayout:FindChildObjectByName("ThreeDaysSet3Pic").__UIPicture__  ;
	
	ThreeDays1Get1  = self.LayerLayout:FindChildObjectByName("ThreeDays1Get1").__UIPicture__  ;
	ThreeDays1Get2  = self.LayerLayout:FindChildObjectByName("ThreeDays1Get2").__UIPicture__  ;
	ThreeDays1Get3  = self.LayerLayout:FindChildObjectByName("ThreeDays1Get3").__UIPicture__  ;
	threeDays1GetNum1 = self.LayerLayout:FindChildObjectByName("threeDays1GetNum1").__UILabel__ ;
	threeDays1GetNum2 = self.LayerLayout:FindChildObjectByName("threeDays1GetNum2").__UILabel__ ;
	threeDays1GetNum3 = self.LayerLayout:FindChildObjectByName("threeDays1GetNum3").__UILabel__ ;
	getBtn1 = self.LayerLayout:FindChildObjectByName("getBtn1").__UIButton__; 
	getBtn1:getMenuItemSprite():registerScriptTapHandler(ThreeDaysUI.onGetButtonCallBack);
	reward_text1 = self.LayerLayout:FindChildObjectByName("reward_text1").__UILabel__ ;
	
	ThreeDays2Get1  = self.LayerLayout:FindChildObjectByName("ThreeDays2Get1").__UIPicture__  ;
	ThreeDays2Get2  = self.LayerLayout:FindChildObjectByName("ThreeDays2Get2").__UIPicture__  ;
	ThreeDays2Get3  = self.LayerLayout:FindChildObjectByName("ThreeDays2Get3").__UIPicture__  ;
	threeDays2GetNum1 = self.LayerLayout:FindChildObjectByName("threeDays2GetNum1").__UILabel__ ;
	threeDays2GetNum2 = self.LayerLayout:FindChildObjectByName("threeDays2GetNum2").__UILabel__ ;
	threeDays2GetNum3 = self.LayerLayout:FindChildObjectByName("threeDays2GetNum3").__UILabel__ ;
	getBtn2 = self.LayerLayout:FindChildObjectByName("getBtn2").__UIButton__; 
	getBtn2:getMenuItemSprite():registerScriptTapHandler(ThreeDaysUI.onGetButtonCallBack);
	reward_text2 = self.LayerLayout:FindChildObjectByName("reward_text2").__UILabel__ ;
	
	ThreeDays3Get1  = self.LayerLayout:FindChildObjectByName("ThreeDays3Get1").__UIPicture__  ;
	ThreeDays3Get2  = self.LayerLayout:FindChildObjectByName("ThreeDays3Get2").__UIPicture__  ;
	ThreeDays3Get3  = self.LayerLayout:FindChildObjectByName("ThreeDays3Get3").__UIPicture__  ;
	threeDays3GetNum1 = self.LayerLayout:FindChildObjectByName("threeDays3GetNum1").__UILabel__ ;
	threeDays3GetNum2 = self.LayerLayout:FindChildObjectByName("threeDays3GetNum2").__UILabel__ ;
	threeDays3GetNum3 = self.LayerLayout:FindChildObjectByName("threeDays3GetNum3").__UILabel__ ;
	getBtn3 = self.LayerLayout:FindChildObjectByName("getBtn3").__UIButton__; 
	getBtn3:getMenuItemSprite():registerScriptTapHandler(ThreeDaysUI.onGetButtonCallBack);
	reward_text3 = self.LayerLayout:FindChildObjectByName("reward_text3").__UILabel__ ;
	
	
	closeBtn = self.LayerLayout:FindChildObjectByName("closeBtn").__UIButton__; 
    closeBtn:getMenuItemSprite():registerScriptTapHandler(onThreeDaysLayerCloseClick);

	
	notification = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
end

function ThreeDaysUI:setupContent()
	CCLuaLog("ThreeDaysUI:setupContent() selectDay = " .. ThreeDaysData.selectDay);
	
	 --local itemArea = "map_ui_7day_FRAME_3DAY_SETTING" .. index;
	local strBin = "UI/ui_7day.bin"
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.0,0.0);
	
	local picStr = "UI/ui_3day_setting.png";
	local sprite1 =  CCSprite:create(picStr);
	local sprite2 =  CCSprite:create(picStr);
	local sprite3 =  CCSprite:create(picStr);
	sprite1:setAnchorPoint(anchorPoint)
	sprite2:setAnchorPoint(anchorPoint)
	sprite3:setAnchorPoint(anchorPoint)
	sprite1:setPosition(ccp( 0.0, 0.0))
	sprite2:setPosition(ccp( 0.0, 0.0))
	sprite3:setPosition(ccp( 0.0, 0.0))
	--Note: 得到图片的相对位置
	--AspriteManager:getInstance():getFramesRelativePos(strBin,itemArea,picIcon,anchorPoint,relativePoint);
	--self:ScaleByScreen(relativePoint)
	--sprite:setPosition(relativePoint);
	--button:getAnimationNode():addChild(sprite,10);
	
	ThreeDaysSet1Pic:getCurrentNode():addChild(sprite1)
	ThreeDaysSet2Pic:getCurrentNode():addChild(sprite2)
	ThreeDaysSet3Pic:getCurrentNode():addChild(sprite3)
	--ThreeDaysSet2Pic:setSprite(sprite2)
	--ThreeDaysSet3Pic:setSprite(sprite3)
	
	for i = 3, 1, -1 do
		self:insertItemToList( i);
	end
	--self:updateShowBySelectDay(SevenDaysData.currDay);
end







 function ThreeDaysUI:onGetButtonCallBack( nTag)
	--CCLuaLog("onGetButtonCallBack selectDay = " .. ThreeDaysData.selectDay .."tag =".. nTag );
   CCLuaLog("onGetButtonCallBack selectDay = " .. ThreeDaysData.selectDay .."tag =");
	local enabledBool, getBool = isThreeDayCanGetAward(ThreeDaysData.selectDay);
	if enabledBool then
		CSGetDailyRewardReq = {
			key =PB_ATTR_GET_DAILY_LOGIN_REWARD_TIME ,
		};
		local msgName = "CSGetDailyRewardReq";
		 CCLuaLog("onGetButtonCallBack selectDay =1 " .. ThreeDaysData.selectDay .."tag =");
		send_message(msgName, CSGetDailyRewardReq, true)		
	end
end






function ThreeDaysUI:updateShowBySelectDay(sd)
	
      self:insertItemToList(sd)
end



function ThreeDaysUI:updateItemMessage(id, index, indexid , count)
	CCLuaLog("updateItemMessage id = " .. id .. " index = " .. index .. "indexid = " .. indexid .." count = " .. count);
	
	
	
	 local ThreeDaysGet1 = nil;
	 local ThreeDaysGet2 = nil;
	 local ThreeDaysGet3 = nil ;
	 local threeDaysGetNum1 = nil;
	 local threeDaysGetNum2 = nil;
	 local threeDaysGetNum3 = nil;
	
	 
	 
	 local ThreeDaysGet = nil ;
	 local threeDaysGetNum = nil ;
	 
     if index == 1 then
	 
	  ThreeDaysGet1 = ThreeDays1Get1;
	  ThreeDaysGet2 = ThreeDays1Get2;
	  ThreeDaysGet3 = ThreeDays1Get3 ;
	  threeDaysGetNum1 = threeDays1GetNum1;
	  threeDaysGetNum2 = threeDays1GetNum2;
	  threeDaysGetNum3 = threeDays1GetNum3;
	
	 elseif index == 2 then 
	
	  ThreeDaysGet1 = ThreeDays2Get1;
	  ThreeDaysGet2 = ThreeDays2Get2;
	  ThreeDaysGet3 = ThreeDays2Get3 ;
	  threeDaysGetNum1 = threeDays2GetNum1;
	  threeDaysGetNum2 = threeDays2GetNum2;
	  threeDaysGetNum3 = threeDays2GetNum3;
	 
	 elseif index == 3 then 
	
	  ThreeDaysGet1 = ThreeDays3Get1;
	  ThreeDaysGet2 = ThreeDays3Get2;
	  ThreeDaysGet3 = ThreeDays3Get3 ;
	  threeDaysGetNum1 = threeDays3GetNum1;
	  threeDaysGetNum2 = threeDays3GetNum2;
	  threeDaysGetNum3 = threeDays3GetNum3;
	 
	 end
	
	 
	 if indexid == 1 then
	 ThreeDaysGet = ThreeDaysGet1 ;
	 threeDaysGetNum = threeDaysGetNum1 ;
	 
	 elseif indexid == 2 then
	 ThreeDaysGet = ThreeDaysGet2 ;
	 threeDaysGetNum = threeDaysGetNum2 ;
	 elseif indexid == 3 then
	 ThreeDaysGet = ThreeDaysGet3 ;
	 threeDaysGetNum = threeDaysGetNum3 ;
	 else
	  return 
	 end
	 
	 
	 local iconSprite = ItemManager:Get():getIconSpriteById(id);
	 local  iconCs = ThreeDaysGet:getSpriteSize();
	 local  imCs = iconSprite:getContentSize();
	 local  xSc = iconCs.width/imCs.width ;
	 local  ySc = iconCs.height/imCs.height ;
	 if xSc > ySc then
		xSc = ySc
	 end
	 local anchorPoint = CCPoint(0.5,0.5);
	 iconSprite:setScale(xSc/TXGUI.UIManager:sharedManager():getScaleFactor())
	iconSprite:setAnchorPoint(anchorPoint)
	
	iconSprite:setPosition(ccp(ThreeDaysGet:getCurrentNode():getContentSize().width/2 ,ThreeDaysGet:getCurrentNode():getContentSize().height/2 ))
	 --iconSprite:setScaleY(xSc*TXGUI.UIManager:sharedManager():getScaleFactorY())
	 ThreeDaysGet:getCurrentNode():addChild(iconSprite);
	 threeDaysGetNum:setString(count);
		
		
		--label:getLabelTTF():setAnchorPoint(ccp(1,0.5));
end



function ThreeDaysUI:insertItemToList(index)

     local ThreeDaysBackNotPic = nil ;
	 local ThreeDaysBackPic = nil ;
	 
	 local getBtn = nil;
	 local reward_text = nil ;
	 
	
	
	
	 
     if index == 1 then
	  ThreeDaysBackNotPic = ThreeDaysBackNot1Pic ;
	  ThreeDaysBackPic = ThreeDaysBack1Pic ;
	  getBtn = getBtn1;
	  reward_text = reward_text1 ;
	 elseif index == 2 then 
	  ThreeDaysBackNotPic = ThreeDaysBackNot2Pic ;
	  ThreeDaysBackPic = ThreeDaysBack2Pic ;
	
	  getBtn = getBtn2;
	  reward_text = reward_text2 ;
	 elseif index == 3 then 
	  ThreeDaysBackNotPic = ThreeDaysBackNot3Pic ;
	  ThreeDaysBackPic = ThreeDaysBack3Pic ;
	 
	  getBtn = getBtn3;
	  reward_text = reward_text3 ;
	 end
	 getBtn:setEnabled(false)
	 if ThreeDaysData.currDay == index then
		ThreeDaysBackNotPic:setVisible(false)
		ThreeDaysBackPic:setVisible(true)
		if ThreeDaysData.markBits == 0 then
			getBtn:setEnabled(true);
		else
			reward_text:setString( LanguageLocalization:GetLocalization("M_LOGINREWARD_ALREADYGOT") );
		end
	 
	 else
		ThreeDaysBackNotPic:setVisible(true)
		ThreeDaysBackPic:setVisible(false)
	    
	 end
	 
	 for	day, value in pairsByKeys(ThreeDaysData.rewardData) do
		if day ~= nil and value ~= nil and day == tostring(index) then --找到选择的天数物品数据，并更新显示
			for	indexid, value2 in pairsByKeys(value) do
				self:updateItemMessage(value2.itemId, index, indexid , value2.count);
			end
		end
	end
	 
	 
	
end

function ThreeDaysUI:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

--拉取登录累计天数与领取标识
local function sendThreeDaysAttrReq()
	CSAttributesReq = {
		keys = {
			PB_ATTR_CONTINUE_LOGIN_TIMES,
		    PB_ATTR_GET_THREE_DAY_REWARD_ENABLE_FLAG,
		}
	};
	local msgName = "CSAttributesReq";
	send_message(msgName, CSAttributesReq, true)
end

--获取属性
local function onMessageAttributesRsp(msg)
	CCLuaLog("onMessageAttributesRsp Threedays");
	for _,v in ipairs(msg.values) do
		if v.key == PB_ATTR_CONTINUE_LOGIN_TIMES then
			print("PB_ATTR_GET_DAILY_LOGIN_TIMES = " .. PB_ATTR_CONTINUE_LOGIN_TIMES .. " value = " .. v.value);
			ThreeDaysData.currDay = v.value;
			

			if ThreeDaysData.currDay > 3 then
				ThreeDaysData.currDay = 3;
			elseif ThreeDaysData.currDay <= 1 then --领完或不存在此活动
				ThreeDaysData.currDay = 1;
			end
		
			ThreeDaysData.selectDay = ThreeDaysData.currDay;
		elseif v.key == PB_ATTR_GET_THREE_DAY_REWARD_ENABLE_FLAG then
			CCLuaLog("PB_ATTR_GET_DAILY_LOGIN_REWARD_MARK = " .. 6007 .. " value = " .. v.value);
			local val = 0 ;
			if v.value == 0 then
				val = 1 ;
			end
			IsOneKeyContainsDailyRewardId(6001 ,val)
			ThreeDaysData.markBits = v.value;
			
			
		end
	end
end

--获取奖励回调
local function onMessageGetLoginRewardRsp(msg)
	print("onMessageGetLoginRewardRsp succ = " .. tostring(msg.succ));
	if ThreeDaysUI.LayerInstance ~= nil then
		if msg.succ == true then
			ThreeDaysData.markBits = 1
			IsOneKeyContainsDailyRewardId(6001 ,0)
			GameApi:showMessage(GetLuaLocalization("M_GET_SUCCESS"))
			ThreeDaysUI:updateShowBySelectDay(ThreeDaysData.selectDay);
			onThreeDaysLayerCloseClick(nil)
		end
	end
end

function registThreeDaysRewardRspAndSendReq()
	print("registThreeDaysRewardRspAndSendReq" ,ThreeDaysData.currDay);
	if not ThreeDaysData.initMsgHandle then
		addMsgCallBack("CSAttributesRsp", onMessageAttributesRsp)
		addMsgCallBack("CSGetDailyRewardRsp", onMessageGetLoginRewardRsp)
		ThreeDaysData.initMsgHandle = true;
	end

	if ThreeDaysData.currDay == 0 then
		
		sendThreeDaysAttrReq();
	end

	return ThreeDaysData.initMsgHandle;
end

local function updateNotificationThreeDays(message)
			--print("updateNotificationThreeDays" ,ThreeDaysData.currDay);
        if message == GM_E_ENTER_MAINLAND then
            registThreeDaysRewardRspAndSendReq()
        end

end
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotificationThreeDays);
