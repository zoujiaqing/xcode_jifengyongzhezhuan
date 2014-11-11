--require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/ChatToolLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameterControl")
require("Script/MainMenuUILayer")
require("Script/MainMenuUILayer")
require("Script/UILayer/diamond")
require("Script/UILayer/BuyDiamondDataCenter")
require("Script/Language")
require("Script/GameConfig/Localization")
require("Script/UILayer/TopUp_Award")
buyDiamondUI = {
	
	LayerInstance = nil,
	Layout = nil,

	TEXT_TITLE=nil,

	TEXT_VIPLEVELHINT= nil,
		--7480/8000
	NUM_PROGRESS_DIAMOND= nil,
		--当前
	TEXT_NOWLEVEL= nil,
		--购买
	TEXT_BUY= nil,
		--当前picute
	ICON_NOWLEVEL= nil,
		
	FRAME_PROGRESS= nil,
	remain_diamond=0,	
	closeBtn= nil,
	diamondList = nil,	
	BUTTON_BUY= nil,
	current_page=0,
}

function buyDiamondUI:ResetValue()
self.scroollpageindex = nil
	self.diamondList = nil
	self.LayerInstance = nil
	self.Layout = nil
	
	self.TEXT_TITLE=nil
self.TEXT_VIPLEVELHINT= nil
	--7480/8000
	self.NUM_PROGRESS_DIAMOND= nil
	--当前
	self.TEXT_NOWLEVEL= nil
	self.remain_diamond=0
	--购买
	self.TEXT_BUY= nil
	--当前picute
	self.ICON_NOWLEVEL= nil
	
	self.FRAME_PROGRESS= nil
	
	self.closeBtn= nil
	self.gift_diamond= nil
	self.BUTTON_BUY = nil
	self.current_page =0
	self.scrollPage = nil
   removeMsgCallBack("CSAttributesRsp", buyDiamondUI.CSAttributesRspCallBack)
end

local function closeLayerCallBack(message)
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= buyDiamondUI.LayerInstance then
			if G_CurLayerInstance ~= buyDiamondUI.LayerInstance then
				buyDiamondUICloseBtnClick(nil)
			end
		end
	end
end

local function onReceiveDestructorHandler()
       	buyDiamondUI:ResetValue()
        TXGUI.UIManager:sharedManager():removeUILayout("buyDiamondUIPanel")
end

function buyDiamondUI:CreateLayer(parentLayer, gift_diamond) --gift_diamond is used for add more dimond to user
	self:ResetValue()
	self.gift_diamond=gift_diamond
	self.diamondList = {}
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
	 self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/vipUI.plist",self.LayerInstance, "buyDiamondUIPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["buyDiamondUI"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		--InitMoneyConfigLayer(self.LayerInstance)
	end	

	--Note: 从服务器获取需要的信息
	ShopItemInfoFromServer:sharedInstance():PrepareNeedDataForLua();

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

function buyDiamondUI:setupScrollist()

    self.scrollList = self.Layout:FindChildObjectByName("BUTTONAREA_LIST").__UIScrollList__
	if nil == self.scrollList then
		return 
	end
	self.scrollList:clearChildrens()
 	self.ListButtons = {}
	self.ListItemButtons = {}
    
	

	local buttonIndex = 1
	for index,value in pairsByKeys(BuyDiamondDataCenter.Items) do
			self:InsertOneSkillItem(self.scrollList,value, buttonIndex)
			self.diamondList[buttonIndex] = value
			buttonIndex = buttonIndex + 1
			
	end	
	if buyDiamondUI.ListItemButtons[1] == nil then
		return
	end
	buyDiamondUI.ListItemButtons[1]:selected()
	
	self.SelectItemIndex = 1
	self.SelectedListItem = buyDiamondUI.ListItemButtons[1]
end

function buyDiamondUI:InsertOneSkillItem(scrollList,value, index)
	if nil == scrollList then
		return 
	end

	print("Insert One Item " .. index)
    
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	--Note: 定义背景框大小
	local backGroundFrameName = "map_ui_vip_FRAME_VIP_FRAME_BUTTON_DIAMOND1";
	-- Note: List Item 
    
    
	local normalListItemIconBgName = "map_ui_vip_FRAME_VIP_BUTTON_DIAMOND1";
	local clickListItemIconBgName = "map_ui_vip_FRAME_VIP_BUTTON_DIAMOND1_SELECTED";
	local normal = AspriteManager:getInstance():getOneFrame(KUI_VIP_BIN,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(KUI_VIP_BIN,clickListItemIconBgName)
		
	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);
        
        
	
	self:addStarIcon(backGroundFrameName,button);

	print("------------------------------ product id " .. value)
	local shopItemInfo = ShopItemInfoFromServer:sharedInstance():GetOneShopItemInfo(value)
	--add money
	local moneyValue = shopItemInfo.product_price
	local diamondValue = shopItemInfo.item_base_num
	local diamondGiftValue = shopItemInfo.item_gift_num
	moneyValue = moneyValue*0.01
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_vip_FRAME_VIP_NUM_MONEY1"
	self:addButtonLabel(backGroundFrameName,childFrameName,"需" .. moneyValue .. "元",24,ccc3(195,109,16),anchorPoint,button:getAnimationNode(),10,index,kCCTextAlignmentRight);
	 childFrameName = "map_ui_vip_FRAME_VIP_NUM_DIAMOND1"
	self:addButtonLabel(backGroundFrameName,childFrameName,diamondValue .. "",24,ccc3(255,206,87),anchorPoint,button:getAnimationNode(),10,100+index,kCCTextAlignmentRight);
	--add picture
	self:addButtonPicture(backGroundFrameName,button, index);
	local menuItem = button:getMenuItemSprite();
	menuItem:registerScriptTapHandler(onbuyDiamondUIListItemClick);

	--add add diamond icons and values
	self:addButtonAdditionDiamondValues(backGroundFrameName,button, index,shopItemInfo);

	local listItemCount = table.getn(self.ListItemButtons);
	self.ListItemButtons[listItemCount+1] = menuItem
	
	local listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = button
    
end

function onbuyDiamondUIListItemClick(nTag)
	print("onbuyDiamondUIListItemClick " .. nTag)

	local listItemIndex = nTag+1
	buyDiamondUI.SelectItemIndex = listItemIndex
		
	local item = buyDiamondUI.ListItemButtons[listItemIndex]
	if nil ~= buyDiamondUI.SelectedListItem then
		buyDiamondUI.SelectedListItem:unselected()
	end			
	item:selected()
	buyDiamondUI.SelectedListItem = item

	--buyDiamondUI:ShowOneTaskDetailPanel()
end

--Note: 显示star
function buyDiamondUI:addStarIcon(backGroundFrameName,button)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_vip_FRAME_VIP_ICON_DIAMOND1";

	local pChildSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_VIP_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	pChildSprite:setPosition(relativePoint);
	button:getAnimationNode():addChild(pChildSprite,2);

end

--显示 button picture
function buyDiamondUI:addButtonPicture(backGroundFrameName,button, index)
	
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_vip_FRAME_VIP_ICON_DIAMONDPIC1";

	AspriteManager:getInstance():getFramesRelativePos(KUI_VIP_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	
	
	if index > 6 then
		index = 6
	end
	local IconName = "map_ui_system_icon_FRAME_ICON_DIAMOND"..index;
	--print("icon name is " .. IconName)
	local pChildSprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,IconName)		
	pChildSprite:setPosition(relativePoint);
	local stateIcon = button:getAnimationNode():getChildByTag(index + 100000)
	if nil ~= stateIcon then
		stateIcon:removeFromParentAndCleanup(true)
	end
	button:getAnimationNode():addChild(pChildSprite,1,index + 100000);	
end

function buyDiamondUI:addButtonAdditionDiamondValues(backGroundFrameName,button, index,shopItemInfo)
	local flag = ""
	local value = 0
	local showPanel = false
	--[[if BuyDiamondDataCenter:IsFirstChange() then
		flag = "M_MULTIPLE"
		--value = BuyDiamondDataCenter:GetFirstChangeMagnification()
	else
		flag = "M_ADD"
		--value = shopItemInfo.item_gift_num
		if shopItemInfo.item_gift_num ~= 0 || self:gift_diamond ~=nil  then
			showPanel = false
		end
	end
	--]]
	flag = "M_ADD"
	
	if first_pay_currAwardId ~= nil then
		local data = TopUp_Award[first_pay_currAwardId];
		if data ~= nil and first_pay_currState == WAITTING_FOR_TOPUP then
			 self.gift_diamond = data.Power
		end
	end	
	if shopItemInfo.item_gift_num ~= 0 or self.gift_diamond ~= nil  then
		showPanel = true
	end
	if showPanel then
		local relativePoint = CCPoint(0,0);
		local anchorPoint = CCPoint(0.5,0.5);
		local childFrameName = "map_ui_vip_FRAME_VIP_ICON_ONSALE";
		local content = Localization[flag].SimpleChinese

		AspriteManager:getInstance():getFramesRelativePos(KUI_VIP_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
		local IconName = "map_ui_system_icon_FRAME_ICON_ONSALE"
		local pChildSprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,IconName)		
		pChildSprite:setPosition(relativePoint);
		button:getAnimationNode():addChild(pChildSprite,1);

		local addDiamond = shopItemInfo.item_gift_num;
		if self.gift_diamond ~= nil then
			addDiamond = addDiamond + tonumber(self.gift_diamond-1)*shopItemInfo.item_base_num
		end
		local childFrameName = "map_ui_vip_FRAME_VIP_TEXT_ONSALEADD";
		self:addButtonLabel(backGroundFrameName,childFrameName,content,20,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),10,nil,kCCTextAlignmentCenter);
		local childFrameName = "map_ui_vip_FRAME_VIP_TEXT_ONSALENUM";
		self:addButtonLabel(backGroundFrameName,childFrameName,"" .. addDiamond,20,ccc3(255,255,255),anchorPoint,button:getAnimationNode(),10,nil,kCCTextAlignmentCenter);
	end
end

--显示 current vip picture
function buyDiamondUI:addVipPicture(vip)
	
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
	local childFrameName = "map_ui_vip_FRAME_VIP_ICON_DIAMONDPIC1";

	--AspriteManager:getInstance():getFramesRelativePos(KUI_VIP_BIN,backGroundFrameName,childFrameName,anchorPoint,relativePoint);
	local IconName = "map_ui_system_icon_FRAME_ICON_VIP"..(vip);
	print("icon name is " .. IconName)
	local pChildSprite = AspriteManager:getInstance():getOneFrame(KICON_BIN,IconName)	
	
	
	self.ICON_NOWLEVEL:setSprite(pChildSprite);

end

--Note: 显示文本函数
function buyDiamondUI:newLabel(content,fontSize,contentSize,color,HorAligment,VerAligment)
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
function buyDiamondUI:addButtonLabel(bgName,uiName,content,fontSize,color,anchorPoint,parentNode,zOrder,nTag,HorAligment,VerAligment)
	local relativePoint = CCPoint(0,0);
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_VIP_BIN,bgName,uiName,anchorPoint,relativePoint);
	local labelSize = pLabelSprite:getContentSize();
	local contentSize = CCSize(labelSize.width,labelSize.height)
	--
	local nameLabel = nil
	if nTag ~= nil then
		nameLabel = parentNode:getChildByTag(nTag)
	end
	if nil == nameLabel then
		nameLabel=self:newLabel(content,fontSize,contentSize,color,HorAligment,VerAligment)
		--nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)
		if nTag ~= nil then
			nameLabel:setTag(nTag)
		end		
		
		parentNode:addChild(nameLabel,zOrder)
	end	
	--nameLabel:setString(content)
	--nameLabel:setColor(color)
	nameLabel:setPosition(relativePoint)
	--return nameLabel
end

function buyDiamondUI:scrolllistAddNode(list, node)
	local base = list:AddBaseItem(node:getContentSize(),node)
	local oldPos_y = node:getPositionY()
	node:setPosition((0 + list:getContentSize().width ) * 0.5,oldPos_y)

	--[[local size = list:getContentSize()
	local viewSize = list:getViewSize()
	if size.height > viewSize.height then
		local offset = ccp(0,size.height - viewSize.height)
		list:setStartOffset(offset)
	end
	--]]

end
function buyDiamondUI:createNode(str, width, fontSize, fontColor)

		node = HtmlNode:create(width)
		node:begin()
		node:addLabel(str,fontSize,KJLinXin,fontColor)
		node:finish()
		node:setAnchorPoint(ccp(0,0.5))
		return node
end
function buyDiamondUI:InsertOneItem(list ,str)

	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
	local size = CCSizeMake(list:getContentSize().width / scale,list:getContentSize().height / scale)
	local fontSize=22
	
	local node = self:createNode(str, size.width, fontSize, ccc3(255,206,87))
	self:scrolllistAddNode(list, node)

end

function buyDiamondUI:initScrollPage()
	
	if self.scrollPage == nil then
		error("scrollPage is nil")
		return
	end
	i = self:getCurPage()
	if self.scroollpageindex ~= nil and self.scroollpageindex == i then	
		return
	end
	self.scroollpageindex = i
	self.scrollPage:clearChildrens()
	
	local s = GetLuaLocalization;
	local strlist=""
	for j = 1, 15 do
		local index = "description_vip"..(i+1).."_"..j
	
		local languageItem = Localization[index]
		if nil ~= languageItem then
			--j ..",".. s(index)
			--print("initScrollPage index is " .. index)
			--nameLabel =  TXGUI.UILabelTTF:create("sdfasldfjlas",KJLinXin,22)
			--nameLabel:setColor(ccc3(255,206,87))
		   --strlist = strlist .. j ..". ".. s(index)
		   self:InsertOneItem(self.scrollPage, j ..". ".. s(index))
		else
			break
		end
			
	end --end for j
	--[[
	local pageCount = self.scrollPage:getPageCount()
	local w = self.scrollPage:getContentSize().width
	local h = self.scrollPage:getContentSize().height
	--print("initScrollPage w and h : ".. w .."," .. h)
	 local scale =  TXGUI.UIManager:sharedManager():getScaleFactor(); 
	for i = 0, pageCount do 
		print("init page " .. i)
		local pageLayer = self.scrollPage:getPageLayer(i)
		if pageLayer == nil then
			return
		end
		local s = GetLuaLocalization;
		local strlist=""
		for j = 1, 15 do
			local index = "description_vip"..(i+1).."_"..j
		
			local languageItem = Localization[index]
			if nil ~= languageItem then
				--j ..",".. s(index)
				--print("initScrollPage index is " .. index)
				--nameLabel =  TXGUI.UILabelTTF:create("sdfasldfjlas",KJLinXin,22)
				--nameLabel:setColor(ccc3(255,206,87))
			   strlist = strlist .. j ..". ".. s(index).."\n"
			else
				break
			end
			
		end --end for j
		
		local nameLabel=self:newLabel(strlist,20,CCSize(0, 0),ccc3(255,206,87))
		pageLayer:addChild(nameLabel, 100)

		local size = nameLabel:getContentSize()
		print("label size is"..size.width..","..size.height)
		--nameLabel:setAnchorPoint(CCPoint(0, 1))
		nameLabel:setPosition(CCPoint(0,h/scale/2 - size.height/2))
	end
	--]]
	
	
end
function buyDiamondUI:getCurPage()
	return self.current_page --0 to 9
end
local function buyDiamondOnReceivedPageChanged()
        CCLuaLog("buyDiamondOnReceivedPageChanged")
        --if  buyDiamondUI.scrollPage ~= nil then
                --CCLuaLog("curPage: " .. OtherPlayerEquipLayer.modelPanel:getCurPage())
	   local page = buyDiamondUI:getCurPage()
	   buyDiamondUI:addVipPicture(page+1) --page is from 0 to 9 vip is from 1 to 10
	   if buyDiamondUI.vip > 0 and buyDiamondUI.vip == page+1 then --self.vip is from 0 to 10
		   buyDiamondUI.TEXT_NOWLEVEL:setString("当前");
	   else
		   buyDiamondUI.TEXT_NOWLEVEL:setString("");
	   end
	   buyDiamondUI:initScrollPage()
        --end
end

function buyDiamondUI:InitLayer()
	
	--再购买xx钻石，你将永久成为vip 5
	self.TEXT_VIPLEVELHINT=self.Layout:FindChildObjectByName("TEXT_VIPLEVELHINT").__UILabel__;
	--7480/8000
	self.NUM_PROGRESS_DIAMOND=self.Layout:FindChildObjectByName("NUM_PROGRESS_DIAMOND").__UILabel__;
	--当前
	self.TEXT_NOWLEVEL=self.Layout:FindChildObjectByName("TEXT_NOWLEVEL").__UILabel__;
	--购买
	self.TEXT_BUY=self.Layout:FindChildObjectByName("TEXT_BUY").__UILabel__;
	--当前picute
	self.ICON_NOWLEVEL	= self.Layout:FindChildObjectByName("ICON_NOWLEVEL").__UIPicture__;
	
	self.FRAME_PROGRESS=self.Layout:FindChildObjectByName("FRAME_PROGRESS").__UIPicture__;
	self.BAR_PROGRESS=self.Layout:FindChildObjectByName("BAR_PROGRESS").__UIPicture__;
	self.BAR_PROGRESS:getCurrentNode():setAnchorPoint(ccp(0,0.5))
	 local tmpSize = self.BAR_PROGRESS:getSpriteSize()
     local oldPos = self.BAR_PROGRESS:getPosition()
	 self.BAR_PROGRESS:setPosition(ccp(oldPos.x-tmpSize.width/2,oldPos.y))
	self.closeBtn = self.Layout:FindChildObjectByName("BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
	self.closeBtn:registerScriptTapHandler(buyDiamondUICloseBtnClick);
	
	self.BUTTON_BUY		= self.Layout:FindChildObjectByName("BUTTON_BUY").__UIButton__
	self.itemGot1 = self.BUTTON_BUY	:getMenuItemSprite();
	self.itemGot1:registerScriptTapHandler(
function()
	--local i = buyDiamondUI.SelectItemIndex
	print("topup type is " .. type(buyDiamondUI.diamondList) .. buyDiamondUI.SelectItemIndex)
	--GameApi:showMessage(string.format("Hi %s 你choose %d",  UserData:GetUserInfo().szName, buyDiamondUI.diamondList[buyDiamondUI.SelectItemIndex])
--GameApi:showMessage("对不起，目前不支持付费")
PayManager:Get():payForProduct(buyDiamondUI.SelectItemIndex, buyDiamondUI.diamondList[buyDiamondUI.SelectItemIndex]);
end
	);
	
	self.BUTTON_PREVIOUSPAGE = self.Layout:FindChildObjectByName("BUTTON_PREVIOUSPAGE").__UIButton__
	self.BUTTON_PREVIOUSPAGE:getMenuItemSprite():registerScriptTapHandler(
function()
buyDiamondUI:moveToPage(-1, true)
end
	);
		
	self.BUTTON_NEXTPAGE = self.Layout:FindChildObjectByName("BUTTON_NEXTPAGE").__UIButton__
	self.BUTTON_NEXTPAGE:getMenuItemSprite():registerScriptTapHandler(
function()
buyDiamondUI:moveToPage(1, true)
end
	);
	
	
	
	self:setupScrollist()		
	local page = self.Layout:FindChildObjectByName("storageScrollPage").__UIScrollList__
	if page ~= nil then
		print("add scroll page");
		self.scrollPage = page
	end
	--self.scrollPage:registerPageChangedCallbackFunc(buyDiamondOnReceivedPageChanged)
	
	--self:initScrollPage()
	self.vip = UserData:GetUserInfo().m_vip
	self:onVipChange()
	--self.remain_diamond = 0
	self:onDiamondChange()
	buyDiamondOnReceivedPageChanged()

	addMsgCallBack("CSAttributesRsp", buyDiamondUI.CSAttributesRspCallBack, buyDiamondUI)
	--addMsgCallBack("CSEquipmentComposeRsp", buyDiamondUI.CSEquipmentComposeRspCallBack, buyDiamondUI)
	self:sendCSGetUserAttributesReq()

end
------change lebel value----


function buyDiamondUI:onVipChange()
	if self.vip == UserData:GetUserInfo().m_vip then 
		return;
	end
	self.vip = UserData:GetUserInfo().m_vip
	
	--self.offhour = Vip[self.vip].Off_Line
	--self.NUM_REMAINONHOOKTIME:setString(self.offhour.."H");
	GameApi:showMessage("当前VIP等级为"..self.vip)

	self:onDiamondChange()
	buyDiamondOnReceivedPageChanged()
    --self:sendCSGetUserAttributesReq()

end

function buyDiamondUI:moveToPage(next, add)
	if add ~= nil and add == true then
		self.current_page = self.current_page+next
	else
		self.current_page = next
	end
	if self.current_page < 0 then
		self.current_page = 0
	elseif self.current_page > 9 then
		self.current_page  = 9
	end
	buyDiamondOnReceivedPageChanged()
end

function buyDiamondUI:onDiamondChange()
	
	if buyDiamondUI.TEXT_VIPLEVELHINT == nil then 
		return;
	end
	print("max vip is "..#Vip)
	--GameApi:showMessage("累积购买钻石"..self.remain_diamond)
	local myvip;
	if self.vip <  #Vip then
		myvip = self.vip+1
		local left = Vip[self.vip+1].Diamond-self.remain_diamond
		if left < 0 then
			left = 0
		end
		self.TEXT_VIPLEVELHINT:setString(string.format("再购买%d钻石，您将永久成为VIP%d", left,self.vip+1));
	else
		myvip = #Vip
		self.TEXT_VIPLEVELHINT:setString(string.format("恭喜！您已经成为最高等级的VIP%d", self.vip));
	end
	
	self.NUM_PROGRESS_DIAMOND:setString(string.format("%d/%d", self.remain_diamond, Vip[myvip].Diamond));
    local scalei = self.remain_diamond / Vip[myvip].Diamond
	if scalei > 1 then
		scalei = 1
	end
	self.BAR_PROGRESS:setScale(scalei,1);
	if self.vip > 1 then
		self:moveToPage(self.vip-1)
	end
end



function buyDiamondUI:updateExpAll()

	self.NUM_ALLEXP:setString("" .. MainMenuLayer:GetNumByFormat(self.all_exp))
end 
-------------------message call back-----------------------------------------------
function buyDiamondUI:CSAttributesRspCallBack(msg) 
	print("Rcv CSAttributesRspCallBack msg")
	if self.LayerInstance == nil then
		return
	end
	
			--PB_ATTR_HOLD_ON_LINE_ACCU_TIME,
			--PB_ATTR_PLAYER_HOLD_EXP,
			--PB_ATTR_PLAYER_STORE_EXP,
	for _,v in ipairs(msg.values) do
		print("\tkey:" .. v.key .. "value : "..v.value)
		if v.key == PB_ATTR_BUY_DIAMOND_ACCU then

		
			self.remain_diamond = v.value;
			print("PB_ATTR_BUY_DIAMOND_ACCU  is :" .. self.remain_diamond)

			self:onDiamondChange();
	    elseif v.key == PB_ATTR_PLAYER_VIP_LV then --vip level
			print("vip upgrade")
			self:onVipChange();
	    end
	end
	return false
end



----------------------send network msg----------------------------------------------------
function buyDiamondUI:sendCSGetUserAttributesReq()
	--get open times on baozhang
	local msgname="CSAttributesReq"
	Req = {
		keys = {
		PB_ATTR_BUY_DIAMOND_ACCU,
			
			} --PB_ATTR_DAILY_TIMES_HERO_TREASURE
		
	}
	send_message(msgname, Req, true)
end



-------------------button callback------------------------------------------------

 function buyDiamondUICloseBtnClick(pObj)
   TXGUI.UIManager:sharedManager():removeUILayout("buyDiamondUIPanel")
   --TXGUI.UIManager:sharedManager():removeUILayout("chatToolbar")
   SetChatBar(true,-1)
   CCTextureCache:sharedTextureCache():removeUnusedTextures()

   RemoveOneLayer(buyDiamondUI.LayerInstance)
   --NotificationCenter:defaultCenter():unregisterScriptObserver(buyDiamondUI.closeCallBackFun)

   buyDiamondUI.LayerInstance:removeFromParentAndCleanup(true);
   buyDiamondUI.LayerInstance = nil
   buyDiamondUI:ResetValue()
end

function isbuyDiamondUIOpen()
	return buyDiamondUI.LayerInstance ~= nil
end
--------------notification-----

local function buyDiamondUIupdateNotification(message)
	--print("buyDiamondUIupdateNotification")
	if G_CurLayerInstance ~= buyDiamondUI.LayerInstance then
		return
	end
	--print("buyDiamondUIupdateNotification2")
	if message == GM_ATTR_VIP_LEVEL_CHANGE then
		print("vip level up")
		
		buyDiamondUI:onVipChange()
	
	end
	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(buyDiamondUIupdateNotification);
local closeCallBackFun = NotificationCenter:defaultCenter():registerScriptObserver(closeLayerCallBack);
