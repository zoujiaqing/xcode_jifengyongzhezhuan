require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/GameConfig")
require("Script/UILayer/StarReward")


starRewardUI = {
	LayerInstance = nil, 
	LayerLayout = nil,
	
	bt1=nil,
	bt2=nil,
	bt3=nil,
	isOpenMainRewardBtn = false,
}

function starRewardUI:ResetValue()
	self.currentPage = 0
	--self.firstPage = 1
	self.sendCategory=false
	self.LayerInstance = nil;
	self.LayerLayout = nil;
	self.pic_area_node= nil
	self.currentPage=nil
	self.ListTipId={}
    self.ListTipBtn={}
end

local function onReceiveDestructorHandler()
	CCLuaLog("starRewardUI destructor");
	TXGUI.UIManager:sharedManager():removeUILayout("starRewardUI");
	starRewardUI:ResetValue();
	--[[if nil ~= notification then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notification);
		notification = nil;
	end
	--]]
end

function starRewardUI:CreateLayer(parentLayer)
	CCLuaLog("starRewardUI:CreateLayer");
	self:ResetValue();

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create();
	else
		self.LayerInstance = parentLayer;
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler);

	self.LayerLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/starReward.plist", self.LayerInstance, "starRewardUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode();
		local tag = UITagTable["starRewardUI"].tag;
		self.LayerLayout:SetNewPriority(-132);
		self.LayerLayout:EnableModeless(true);
		mainMenuNode:addChild(self.LayerInstance, 30, tag);
	end

	-- PushOneLayer(self.LayerInstance, "", "");

	self:Init();
	

end

function getCurrentPage(instanceId)
	print("instance id is"..instanceId)
	for	i=1, #StarReward do
		local begin, last = string.find(StarReward[i].raidID2, ""..instanceId)
		if begin ~= nil then
			print("bingo, find instance in index:"..i)
			
			return i
		end
		
	end
	return #StarReward
end

function showStarRewardUI(instanceId)
	local ret = getCurrentPage(instanceId)
	if ret ~= nil then
		--starRewardUI.firstPage = ret;
		starRewardUI.MaxPage = ret;
		starRewardUI:CreateLayer()
	else
		GameApi:showMessage("instanceId miss in starreward table")
	end
end

function setFirstPageNum(pageNum)
	starRewardUI.firstPage = pageNum
	if starRewardUI.firstPage == 0 then
		starRewardUI.firstPage = 1
	end
end

function setIsOpenRewardBtn(flag)
	starRewardUI.isOpenMainRewardBtn = flag
end

local function onstarRewardCloseClick(pObj)
	CCLuaLog("onstarRewardCloseClick");
	if nil ~= starRewardUI.LayerInstance then
		RemoveOneLayer(starRewardUI.LayerInstance);
		CCTextureCache:sharedTextureCache():removeUnusedTextures();
		starRewardUI.LayerInstance:removeFromParentAndCleanup(true);
		--starRewardUI.LayerInstance = nil;
		--starRewardUI:ResetValue();
		-- SetChatBar(true,-1)
	end
end

local function closeCallBack(message)
	if nil ~= starRewardUI.LayerInstance then
		if G_CurLayerInstance ~= starRewardUI.LayerInstance then
			onstarRewardCloseClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		--onDailyActivityLayerCloseClick(nil)
		onstarRewardCloseClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

function starRewardUI:Init()
	local container_name="map_ui_FRAME_STARREWARD_HUDONG_FRAM"
	local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_BIN,container_name)
	self.area_pos=nodePt
	self.area_node=nodeFrame
	self.pic_area_node =  CCNode:create()
	local CCSize s = self.area_node:getContentSize()
	self.pic_area_node:setContentSize(s)
	self.pic_area_node:setPosition(CCPoint(s.width/2, s.height/2))
	
	self.LayerLayout:FindChildObjectByName("ItemContainer").__UIPicture__:getCurrentNode():addChild(self.pic_area_node)
	
	local bnt_close = self.LayerLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	bnt_close:registerScriptTapHandler(onstarRewardCloseClick)

	notification = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
	--get all 3 button
	for i=1, 3 do
		_G["starRewardUI"]["bt"..i] = self.LayerLayout:FindChildObjectByName("bt"..i).__UIButton__
		_G["starRewardUI"]["bt"..i]:getMenuItemSprite():registerScriptTapHandler(function()
			onstarRewardButtonClicked(i)
		end)
		_G["starRewardUI"]["butttonLabel"..i] = self.LayerLayout:FindChildObjectByName("butttonLabel"..i).__UILabel__
		
	end
	--get item bg picture uipicture
	for i=1, 12 do
		starRewardUI["item"..i]=self.LayerLayout:FindChildObjectByName("item"..i).__UIPicture__
	end
	
	for i=1, 3 do
		local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_BIN,"map_ui_FRAME_STARREWARD_TEXT_GOT"..i)
		starRewardUI["picPos"..i] = nodePt
	end
	self.leftBt = self.LayerLayout:FindChildObjectByName("leftBt").__UIButton__
	self.leftBt:getMenuItemSprite():registerScriptTapHandler(
	function()
	starRewardUI:moveToPage(-1, true)
	end);
		
	self.rightBt = self.LayerLayout:FindChildObjectByName("rightBt").__UIButton__
	self.rightBt:getMenuItemSprite():registerScriptTapHandler(
	function()
	starRewardUI:moveToPage(1, true)
	end);
	self.fubenName = self.LayerLayout:FindChildObjectByName("fubenName").__UILabel__
	
	addMsgCallBack("CSInstanceStarRewardRsp", starRewardUI.CSInstanceStarRewardRsp, starRewardUI)
	addMsgCallBack("CSInstanceGetStarRewardRsp", starRewardUI.CSInstanceGetStarRewardRsp, starRewardUI)
	
	self:moveToPage(self.firstPage)
end
--tip按钮
function starRewardUIonPressIconButton(nTag)
   print("onPressTipBoxButton:"..nTag)
   local id = nTag
   local item_id = starRewardUI.ListTipId[id]
   local iconbtn = starRewardUI.ListTipBtn[id]
   local menu = iconbtn:getMenuNode()
   local pos = menu:getTouchPoint()
   ItemManager:Get():showItemTipsById(item_id, pos)
end

function starRewardUI:addIconsOneRows(rowNumber)
	i=4*rowNumber+1
	local award = StarReward[self.currentPage]["raiddrops"..(rowNumber+1)]
	local uiScale =  TXGUI.UIManager:sharedManager():getScaleFactor();
	for item_id, itemNumber in string.gfind(award, "(%d+)\/(%d+)") do
		print("item"..item_id..",number:"..itemNumber)
		local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_BIN,"map_ui_FRAME_STARREWARD_REWARDICON"..i)
	  -- local spriteSize = iconpiture:getSpriteSize()
	  -- self.iconPos[i] = CCPoint(labelPt.x, labelPt.y)
	   local boxLockSpriteOff = AspriteManager:getInstance():getOneFrame(KUI_BIN, "map_ui_FRAME_ALCHEMY_ICON_CLICKED")   

	   local pEmptyItem = CCSprite:create()
	   pEmptyItem:setContentSize(labelFrame:getContentSize())
	   
	   boxBtn = IconButton:create(pEmptyItem,nil,boxLockSpriteOff,-136)
	   
	   local btnMenuItem = boxBtn:getMenuItem()
	   btnMenuItem:setTag(i)
	   btnMenuItem:registerScriptTapHandler(starRewardUIonPressIconButton)
	   
	   self.pic_area_node:addChild(boxBtn)
	   boxBtn:setPosition(ccpSub(labelPt,self.area_pos))
		
	   local sprite = ItemManager:Get():getIconSpriteById(item_id)
	   --add blue or green kuang
	  -- local itemInfo = ItemManager:Get():getItemInfoById(id);
	   local itemInfo = ItemManager:Get():getItemInfoById(item_id);
	   if nil ~= itemInfo then
			if itemInfo.m_quality > 0 then
				local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality);
				if nil ~= framePic then
					boxBtn:setFrameSprite(framePic, ccp(0,0));
					--framePic:setScale(uiScale)
					
				end
			end
		end
	   --sprite:setScale(uiScale)
	   boxBtn:SetNormalIcon(sprite);
	   boxBtn:setScale(0)
	   
	   local scaleIn = CCScaleTo:create(0.2, 1); 
	   boxBtn:runAction(scaleIn)
	    -- boxBtn:setHandlerPriority(-136)
	   	--add item number
		local	childFrameName = "map_ui_FRAME_STARREWARD_REWARDNUMBER"..i
		local	iconframe,iconPt = AspriteManager:getInstance():getOneFrameAndPosition(KUI_BIN,childFrameName)  
		 local numberContent = MainMenuLayer:GetNumByFormat(itemNumber)
		local nameLabel = TXGUI.UILabelTTF:create(""..numberContent,KJLinXin,24)
		nameLabel:setPosition(ccpSub(iconPt,self.area_pos))
		self.pic_area_node:addChild(nameLabel,4);
		
	   self.ListTipId[i] = item_id
	   self.ListTipBtn[i] = boxBtn
	   self["item"..i]:setVisible(true)
	   i=i+1
	end
end

function starRewardUI:showRewardItem(page)

	if self.pic_area_node ~= nil then
		self.pic_area_node:removeAllChildrenWithCleanup(true)
	end
	for i=1, 12 do
		self["item"..i]:setVisible(false)
	end
	self:addIconsOneRows(0)
	self:addIconsOneRows(1)
	self:addIconsOneRows(2)
	
end

function starRewardUI:updateStarNumber(newNumber)
	self:showStarNumber(newNumber)
end

function starRewardUI:showStarNumber(newNumber)
	local i=1
	local n
	for num in string.gfind(StarReward[self.currentPage].starneed, "(%d+)") do
		print("num"..i.."is:"..num)
		num = tonumber(num)
		if newNumber >= num  then
			n = num
			self.LayerLayout:FindChildObjectByName("num"..i).__UILabel__:setColor(ccc3(0, 255, 0))
		else
			n = newNumber
			self.LayerLayout:FindChildObjectByName("num"..i).__UILabel__:setColor(ccc3(255, 255, 255))
		end
		self.LayerLayout:FindChildObjectByName("num"..i).__UILabel__:setString(n.."/"..num)
		
		i=i+1
	end
end
--[[

message CSInstanceStarRewardRsp {
	required uint32 instance_category	= 1;
	required uint32 stars				= 2; // 当前副本类别中累计的星数
	required uint32 one_status 			= 3; // 一星奖励的领取状态: 0-不满足领取条件 | 1-可以领取 | 2-已经领取
	required uint32 two_status 			= 4; // 
	required uint32 three_status 		= 5; //
}

--]]
function starRewardUI:CSInstanceStarRewardRsp(msg)
	if msg.instance_category ~= self.currentPage then
		print("CSInstanceStarRewardRsp not current page rsp"..msg.instance_category..","..self.currentPage)
		return
	end
	--self.LayerLayout:FindChildObjectByName("xuyangxingshu").__UILabel__:setString("星数:"..msg.stars)
	self:updateStarNumber(msg.stars)
	--self.status1 = msg.one_status
	--self.status2 = msg.two_status
	--self.status3 = msg.three_status
	self:changeButtonState(1, msg.one_status)
	self:changeButtonState(2, msg.two_status)
	self:changeButtonState(3, msg.three_status)
	return true;
end
--button id 1, 2, 3
--state  0-不满足领取条件 | 1-可以领取 | 2-已经领取
function starRewardUI:changeButtonState(buttonId, state)
	if state == 0 then
		starRewardUI["bt"..buttonId]:setEnabled(false)
		starRewardUI["butttonLabel"..buttonId]:setString("不可领")
	elseif state == 1 then
		starRewardUI["bt"..buttonId]:setEnabled(true)
		starRewardUI["butttonLabel"..buttonId]:setString("领取")
	elseif state == 2 then
		starRewardUI["bt"..buttonId]:setEnabled(false)
		starRewardUI["butttonLabel"..buttonId]:setString("已领取")
		local getIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,"map_ui_system_icon_FRAME_ICON_ALREADYGOT");
		if getIcon ~= nil then
			if self.sendCategory then
				self.sendCategory = false
				getIcon:setScale(3)
				--local ac1 = CCMoveTo:create(0.1, ccpSub(self["picPos"..buttonId],self.area_pos))
				local ac2 = CCScaleTo:create(0.2, 1)
				--local ac3 = CCFadeIn:create(0.2)
				--local array = CCArray:createWithCapacity(3)
				--array:addObject(ac1)
				--array:addObject(ac2)
				--array:addObject(ac3)
				--local spaw = CCSpawn:create(ac2)
				getIcon:runAction(ac2)
			end	
				--getIcon:setPosition(ccp(0,0))
				getIcon:setPosition(ccpSub(self["picPos"..buttonId],self.area_pos))	
			
			self.pic_area_node:addChild(getIcon,100)			
		end
	end
end
--[[
message CSInstanceGetStarRewardRsp {
	required uint32 instance_category = 1;
	required uint32 reward_type		  = 2;	
	required uint32 status	          = 3;	// 0-领取成功 | 1-领取失败(包裹已满)
}

--]]
function starRewardUI:CSInstanceGetStarRewardRsp(msg)
	if msg.instance_category ~= self.currentPage then
		print("CSInstanceStarRewardRsp not current page rsp")
		return true
	end
	if msg.status == 0 then
		--GameApi:showMessage("领取成功")
		print("reward type is "..msg.reward_type)
		local award = StarReward[msg.instance_category]["raiddrops"..msg.reward_type]
		for item_id, itemNumber in string.gfind(award, "(%d+)\/(%d+)") do
			local itemInfo = ItemManager:Get():getItemInfoById(item_id)
			 local numberContent = MainMenuLayer:GetNumByFormat(itemNumber)
			GameApi:showMessage("成功领取"..numberContent..""..itemInfo.m_name)
		end
		--self:changeButtonState(msg.reward_type, 2)
	elseif  msg.status == 1 then
		GameApi:showMessage("领取失败")
	end
	return true
end
--[[
message CSInstanceStarRewardReq {
	required uint32 instance_category = 1; // 普通副本类别ID
}
--]]
function starRewardUI:CSInstanceStarRewardReq(category)
	local msgname="CSInstanceStarRewardReq"
	Req = {
		instance_category = category
	}
	send_message(msgname, Req, true)
end
--[[
message CSInstanceGetStarRewardReq {
	required uint32 instance_category = 1;
	required uint32 reward_type		  = 2;	// 奖励的星级
}
--]]
function starRewardUI:CSInstanceGetStarRewardReq(category, t)
	self.sendCategory=true
	local msgname="CSInstanceGetStarRewardReq"
	Req = {
		instance_category = category,
		reward_type = t
	}
	send_message(msgname, Req, true)
	self:changeButtonState(t, 2)
end

function onstarRewardButtonClicked(id)
	print("press "..id)
	--[[
	local index = "status"..id
	if not starRewardUI[index] then
		print("index is null "..id)
		return
	end

	if starRewardUI[index] == 0 then
		GameApi:showMessage("星数不足")
		return
	elseif starRewardUI[index] == 2 then
		GameApi:showMessage("已领取")
		return
	end
	--]]
	starRewardUI:CSInstanceGetStarRewardReq(starRewardUI.currentPage, id)
end



function starRewardUI:moveToPage(next, add)
	local page=0
	if add ~= nil and add == true then
		--if self.currentPage ~= nil then
			page = self.currentPage+next
		--end
	else
		page = next
	end
	print("starrewardata table number is "..#StarReward)
	if page < 1 then
		page = 1
	elseif page > self.MaxPage	then
		page  = self.MaxPage
	elseif page > #StarReward then
		page = #StarReward
	end
	if self.currentPage~= nil and self.currentPage == page then
		return
	end
	self.currentPage = page
	print("move to page"..self.currentPage)
	if StarReward[self.currentPage] ~= nil then
		self.fubenName:setString(LanguageLocalization:GetLocalization(StarReward[self.currentPage].raidname))
	end
	self.status1 = nil
	self.status2 = nil
	self.status3 = nil
	self:showRewardItem(self.currentPage)
	self:CSInstanceStarRewardReq(self.currentPage)
	
end

function starRewardUI:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

function starRewardUIShowtip(show)
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("InstanceList");
	if	topLayout ~= nil then
		starRewardButton = topLayout:FindChildObjectByName("BUTTON_SHOW_STAR_REWARD").__UIButton__;
		if show then
			starRewardButton:ShowRotateEffect()
			XLogicManager:sharedManager():setHasStarReward(true)
		else
			starRewardButton:HideRotateEffect()
			XLogicManager:sharedManager():setHasStarReward(false)
		end
	else
		print("instancelist layout is null")
	end
end

function sendCheckInstanceStarRewardReq()
	local msgname="CSChkInstanceStarRewardReq"
	Req = {
	}
	send_message(msgname, Req, true)
end
function CSChkInstanceStarRewardRsp(msg)
	starRewardUIShowtip(msg.all_or_nothing)
	starRewardUI.firstPage = msg.last_raid_id
	if starRewardUI.firstPage == 0 then
		starRewardUI.firstPage = 1
	end

	-- for mainRewardBtn
	if(starRewardUI.isOpenMainRewardBtn == false) then
		return true
	end

	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	local mainRewardBtn = topLayout:FindChildObjectByName("button_starReward").__UIButton__
	if(msg.all_or_nothing) then
		mainRewardBtn:setVisible(true)
		mainRewardBtn:ShowRotateEffect()
	else
		mainRewardBtn:setVisible(false)
	end

	return true
end
addMsgCallBack("CSChkInstanceStarRewardRsp", CSChkInstanceStarRewardRsp)