require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/GameConfig")
require("Script/GameConfig/GM")
local updateFunc = nil;
local notification = nil;
FeedBackData = {};



FeedBackUI = {
	LayerInstance = nil, 
	LayerLayout = nil,
	ListButtons = {},
	ListButtonsMenuItems = {},
	
}

function FeedBackUI:ResetValue()
	FeedBackData = {};
	self.LayerInstance = nil;
	self.LayerLayout = nil;

	self.ListButtons = {};
	self.ListButtonsMenuItems = {};
end

local function onReceiveDestructorHandler()
	CCLuaLog("FeedBackUI destructor");
	TXGUI.UIManager:sharedManager():removeUILayout("FeedBackUI");
	FeedBackUI:ResetValue();
	--[[if nil ~= notification then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notification);
		notification = nil;
	end
	--]]
end

function FeedBackUI:CreateLayer(parentLayer)
	CCLuaLog("FeedBackUI:CreateLayer");
	self:ResetValue();

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create();
	else
		self.LayerInstance = parentLayer;
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler);

	self.LayerLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FeedbackUI.plist", self.LayerInstance, "FeedBackUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode();
		local tag = UITagTable["FeedBackLayer"].tag;
		mainMenuNode:addChild(self.LayerInstance, 0, tag);
	end

	PushOneLayer(self.LayerInstance, "", "");

	self:Init();
	self:setupScrollList();

end

local function onFeedBackLayerCloseClick(pObj)
	CCLuaLog("onFeedBackLayerCloseClick");
	if nil ~= FeedBackUI.LayerInstance then
		RemoveOneLayer(FeedBackUI.LayerInstance);
		CCTextureCache:sharedTextureCache():removeUnusedTextures();
		FeedBackUI.LayerInstance:removeFromParentAndCleanup(true);
		--FeedBackUI.LayerInstance = nil;
		--FeedBackUI:ResetValue();
	end
end

local function closeCallBack(message)
	if nil ~= FeedBackUI.LayerInstance then
		if G_CurLayerInstance ~= FeedBackUI.LayerInstance then
			onFeedBackLayerCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		onFeedBackLayerCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

function FeedBackUI:Init()
	local bnt_close = self.LayerLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	bnt_close:registerScriptTapHandler(onFeedBackLayerCloseClick)

	notification = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
end

function FeedBackUI:setupScrollList()
	local scrollList = self.LayerLayout:FindChildObjectByName("scrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end
	--refresh data table
		FeedBackData[1] = {name="M_SETTING_MAILBOX", enable=true,};
		FeedBackData[2] = {name="M_CUSTOMSERVICE_ONLINE", enable=true,};
	--FeedBackData[3] = {name="M_CUSTOMSERVICE_HOTSPOT", enable=true,};
	local index = 3
	print("G_GameChannelID is ".. G_GameChannelID)
	local feedbackindex = tonumber(G_GameChannelID)
	if feedbackindex == 3 or feedbackindex == 24 or feedbackindex == 61 then --91 not re dian wen ti
		index = 2;
		FeedBackData[2] =nil;
	end
	if GM[feedbackindex] ~= nil then
		print("chanell is not null")
	end
	
	if GM[feedbackindex] ~= nil and GM[feedbackindex].hot_issues == "1" then
		FeedBackData[index] = {name="M_CUSTOMSERVICE_HOTSPOT", enable=true,};
		index = index+1
	end
	if GM[feedbackindex] ~= nil and GM[feedbackindex].radiers == "1" then
		FeedBackData[index] = {name="M_SETTING_CHANGEPASSWORD", enable=true,};
		index = index+1
	end
	if GM[feedbackindex] ~= nil and GM[feedbackindex].user_center == "1" then
		FeedBackData[index] = {name="M_USER_CENTER", enable=true,};
		index = index+1
	end
	if GM[feedbackindex] ~= nil and GM[feedbackindex].game_center == "1" then
		FeedBackData[index] = {name="M_CUSTOMSERVICE_FORUM", enable=true,};
		index = index+1
	end
	if GM[feedbackindex] ~= nil and GM[feedbackindex].enter_web == "1" then
		FeedBackData[index] = {name="M_ENTER_WEB", enable=true,};
		index = index+1
	end

	local feedBackDataLength = table.getn(FeedBackData);
	CCLuaLog("feedBackDataLength: " .. feedBackDataLength);
	
	for i = 1, feedBackDataLength, 2 do
			
			if i + 1 <= feedBackDataLength then
				self:insertItemToList(scrollList, i, i+1);
			else
				self:insertItemToList(scrollList, i);
			end
		
	end
end

function FeedBackUI:insertItemToList(scrollList,id1,id2)
	local itemArea = "map_ui_FRAME_CUSTOMSERVICE_AREA_BUTTON2";
	local leftBtnNormal = "map_ui_FRAME_CUSTOMSERVICE_BUTTON_MAIL";
	local leftBtnClicked = "map_ui_FRAME_CUSTOMSERVICE_BUTTON_MAIL_CLICKED";
	local leftBtnDisable = "map_ui_FRAME_CUSTOMSERVICE_BUTTON_WEBSITE_GRAY";
	local rightBtnNormal = "map_ui_FRAME_CUSTOMSERVICE_BUTTON_NOTICEBOARD";
	local rightBtnClicked = "map_ui_FRAME_CUSTOMSERVICE_BUTTON_NOTICEBOARD_CLICKED";
	local rightBtnDisable = "map_ui_FRAME_CUSTOMSERVICE_BUTTON_WEBSITE_GRAY";
	local strBin = "UI/ui.bin"

	local normal = AspriteManager:getInstance():getOneFrame(strBin,itemArea)
	local clicked = AspriteManager:getInstance():getOneFrame(strBin,itemArea)
	local button = scrollList:AddNewItem(normal, clicked, "", "", ccc3(255,255,255), -132);

	if id1 ~= nil then
	self:insertChildToItem(scrollList, button, id1, itemArea, leftBtnNormal, leftBtnClicked, leftBtnDisable);
	end
	if id2 ~= nil then
	self:insertChildToItem(scrollList, button, id2, itemArea, rightBtnNormal, rightBtnClicked, rightBtnDisable);
	end
end

function FeedBackUI:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

function FeedBackUI:insertChildToItem(scrollList, parentNode, id, bgIcon, normalIcon, clickIcon, disableIcon)
	local strBin = "UI/ui.bin"
	--Note: 得到子Button的相对位�?
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	AspriteManager:getInstance():getFramesRelativePos(strBin,bgIcon,normalIcon,anchorPoint,relativePoint);
	self:ScaleByScreen(relativePoint)
	--Note:创建一个按�?
	local normalSprite = AspriteManager:getInstance():getOneFrame(strBin,normalIcon)
	local clickedSprite = AspriteManager:getInstance():getOneFrame(strBin,clickIcon)
	local disableSprite = AspriteManager:getInstance():getOneFrame(strBin,disableIcon);
	local menuItem = MenuItemExpand:create(normalSprite,clickedSprite, disableSprite);
	local btn = TXGUI.UIButton:newExpand("", 0, 0, scrollList, menuItem, nil,-132, "");
	local parentControlPos = parentNode:getPosition()
	local newBtnPos = ccp(parentControlPos.x + relativePoint.x , parentControlPos.y + relativePoint.y)
	btn:getAnimationNode():setPosition(newBtnPos);

	--Note: 定义回调接口
	self.ListButtons[id] = btn
	local btnMenuItem = btn:getMenuItemSpriteExpand()
	local enabled = FeedBackData[id].enable;
	--[[if id == 3 and (G_GameChannelID ~= 29 and G_GameChannelID ~= 30 and G_GameChannelID ~= 16
	and G_GameChannelID ~= 31 and G_GameChannelID ~= 35) then
		enabled=false
		FeedBackData[id].enable=false
	end
	
	if id == 6 and (G_GameChannelID ~= 29 and G_GameChannelID ~= 30  and G_GameChannelID ~= 16
	and G_GameChannelID ~= 31 and G_GameChannelID ~= 35) then
		enabled=false
		FeedBackData[id].enable=false
	end
	--]]
	self.ListButtonsMenuItems[id] = btnMenuItem
	btnMenuItem:setTag(id)
	--btnMenuItem:setEnabled(enabled == true);
	btnMenuItem:setMenuItemDispalyState((enabled == true and 2) or 3)
	btnMenuItem:registerScriptTapHandler(onFeedBackLayerOneItemClickEvent)

	--Note: 名字
	local content = LanguageLocalization:GetLocalization(FeedBackData[id].name)
	local nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, 32,normalSprite:getContentSize(),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
	btn:getAnimationNode():addChild(nameLabel, 10);
	nameLabel:setColor(ccc3(255,255,255));
end

local function closeFeedBackLayerUpdate(dt)
	if nil ~= updateFunc then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFunc);
	end

	onFeedBackLayerCloseClick(nil)
end

function onFeedBackLayerOneItemClickEvent(nTag)
	CCLuaLog("onFeedBackLayerOneItemClickEvent" .. nTag)
	local item = FeedBackData[nTag];

	if nil == item then
		return
	end
	
	if false == item.enable then
		return
	end

	

	CCLuaLog("FeedBack item clicked")
	if	item.name == "M_SETTING_MAILBOX" then
		--MailUI:CreateLayer(nil);
		MailUILayer:createLayer()
	elseif item.name == "M_SETTING_CHANGEPASSWORD" then
		--[[if FeedBackUI.LayerInstance ~= nil then
			FeedBackUI.LayerInstance:setVisible(false)
		end
		Notice2:CreateLayer(nil);
        print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 10")
		updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(closeFeedBackLayerUpdate, 0, false)
		--]]
		FeedbackLayer:Get():callbackWeb("http://jf.appgame.com")
	elseif item.name == "M_CUSTOMSERVICE_FORUM" then
		--FeedbackLayer:Get():callbackForum(nil)
		FeedbackLayer:Get():callbackWeb("http://bbs.coco.cn/forum-159-1.html")
	elseif item.name == "M_CUSTOMSERVICE_ONLINE" then
		FeedbackLayer:Get():callbackOnline(nil)
		
		
	elseif item.name == "M_CUSTOMSERVICE_HOTSPOT" then
		FeedbackLayer:Get():callbackHotSpot(nil)
	elseif item.name == "M_ENTER_WEB" then
		FeedbackLayer:Get():callbackWeb("http://jf.coco.cn")
	elseif item.name == "M_USER_CENTER" then
		LoginManager:Get():doUserCenter()
	end

end


