require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/Item")
require("Script/Fairy/FairyConfigTable")
require("Script/Mail/MailMsg")


MailUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	attachmentPos = nil,
	currentMailId_high = 0,
	currentMailId_low = 0,
	currentBtIndex = 0,
	startMailIndex = 0,
	overMailIndex = 0,
	mailIndexTable = nil,
	getAttachmentIndex = 0, -- 1单领，2多领
}

local function onReceiveDestructorHandler()
	RemoveOneLayer(MailUILayer.uiLayerInstance)
	--if HelpUILayer.notificationFunc ~= nil then
		--NotificationCenter:defaultCenter():unregisterScriptObserver(HelpUILayer.notificationFunc);
	--end
	TXGUI.UIManager:sharedManager():removeUILayout("MailUI")
	MailUILayer:ResetValue()
end

function MailUILayer:createLayer()
	if self.uiLayerInstance == nil and index ~= 0 then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["MailUI"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/MailUI.plist",self.uiLayerInstance, "MailUI", true);
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

function MailUILayer:ResetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.attachmentPos = nil
	self.currentMailId_high = 0
	self.currentMailId_low = 0
	self.currentBtIndex = 0
	self.startMailIndex = 0
	self.overMailIndex = 0
	self.getAttachmentIndex = 0
	self.mailIndexTable = nil
end

local function onCloseButtonClicked(tag)
	if MailUILayer.uiLayerInstance ~= nil then
		MailUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end	

local function onClickGetAttachmentBt(tag)
	MailUILayer.getAttachmentIndex = 1
	sendSetSingleMailStatusReq(MailUILayer.currentMailId_high,MailUILayer.currentMailId_low,3)
end

local function onClickConfirmToDelete()
	sendSetSingleMailStatusReq(MailUILayer.currentMailId_high,MailUILayer.currentMailId_low,4)
end

local function onClickConfirmToDeleteMailWithoutAttach()
	if MailInfoList ~= nil then
		local statusTable = {}
		for k,v in pairs(MailInfoList) do 
			if v.has_attach and v.mail_status == PB_MAIL_STATUS_GOT then
				local msgInfo = {}
				msgInfo.idHigh = v.mail_id_high
				msgInfo.idLow = v.mail_id_low
				msgInfo.status = PB_MAIL_STATUS_DEL
				table.insert(statusTable,msgInfo)
			end
		end
		if table.getn(statusTable) > 0 then
			sendSetMailStatusReq(statusTable)
		end
	end
end

local function onClickDeleteMailBt(tag)
	local info = getMailDetailById(MailUILayer.currentMailId_high,MailUILayer.currentMailId_low)
	if info ~= nil then
		if info.mail_status ~= 3 and info.items ~= nil and table.getn(info.items)>0 then
			local str = LanguageLocalization:GetLocalization("M_MESSAGE_DELETED_TIPS_2")
			MessageBox:Show(str,onClickConfirmToDelete,nil,MB_YESNO,ccc3(255,255,255),26)
		else
			sendSetSingleMailStatusReq(MailUILayer.currentMailId_high,MailUILayer.currentMailId_low,4)
		end
	end
end

local function onClickNextPageBt(tag)
	local userInfo = UserData:GetUserInfo()
	local totalMail = userInfo.m_totalMailNum
	local index = totalMail - MailUILayer.overMailIndex
	CCLuaLog("nextMailNum: " .. index)
	if index > 4 then
		index = 4
	end
	if index >0 then
		sendGetMailListReq(MailUILayer.overMailIndex+1,MailUILayer.overMailIndex+index)
	end
end

local function onClickLastPageBt(tag)
	local userInfo = UserData:GetUserInfo()
	local totalMail = userInfo.m_totalMailNum
	local index = MailUILayer.startMailIndex - 4
	local reqNum = totalMail
	CCLuaLog("nextMailNum: " .. index)
	if index < 1 then
		index = 1
	end
	if reqNum >= 3 then
		reqNum = 3
	end
	sendGetMailListReq(index,index + reqNum)
end

local function onClickGetAllBt(tag)
	if MailInfoList ~= nil then
		local statusTable = {}
		for k,v in pairs(MailInfoList) do 
			if v.has_attach and v.mail_status ~= PB_MAIL_STATUS_GOT and v.mail_status ~= PB_MAIL_STATUS_DEL then
				local msgInfo = {}
				msgInfo.idHigh = v.mail_id_high
				msgInfo.idLow = v.mail_id_low
				msgInfo.status = PB_MAIL_STATUS_GOT
				table.insert(statusTable,msgInfo)
				CCLuaLog("index " .. v.index)
			end
		end
		if table.getn(statusTable) > 0 then
			sendSetMailStatusReq(statusTable)
			MailUILayer.getAttachmentIndex = 2
		end
	end
end

local function onClickAttachmentBt(tag)
	if MailUILayer.attachmentPos ~= nil and MailUILayer.attachmentPos[tag] ~= nil then
		local pos = MailUILayer.attachmentPos[tag]:getTouchPoint()
		ItemManager:Get():showItemTipsById(tag, pos)
	end
end

local function onClickMailListBt(tag)
	CCLuaLog("onClickMailListBt: " .. tag)
	local mailIndex = 0
	if MailUILayer.mailIndexTable ~= nil then
		mailIndex = MailUILayer.mailIndexTable[tag]
	end
	CCLuaLog("mailIndex: " .. mailIndex)
	if MailInfoList ~= nil and mailIndex ~= 0 and MailInfoList[mailIndex] ~= nil then
		MailUILayer:setMenuItemStatus(tag)
		MailUILayer.currentBtIndex = tag
		local info = getMailDetailById(MailInfoList[mailIndex].mail_id_high,MailInfoList[mailIndex].mail_id_low)
		if info ~= nil then
			MailUILayer:showMailDetail(info)
		else
			sendGetMailInfoReq(MailInfoList[mailIndex].mail_id_high,MailInfoList[mailIndex].mail_id_low)
		end
		CCLuaLog("MailInfoList[" .. mailIndex .. "].mail_status: " .. MailInfoList[mailIndex].mail_status)
		if MailInfoList[mailIndex].mail_status == 0 or MailInfoList[mailIndex].mail_status == 1 then
			sendSetSingleMailStatusReq(MailInfoList[mailIndex].mail_id_high,MailInfoList[mailIndex].mail_id_low,2)
		end
	end
end

function MailUILayer:showPageNum()
	local userInfo = UserData:GetUserInfo()
	local totalMail = userInfo.m_totalMailNum
	local totalPage,temp = math.modf(totalMail/4)
	if temp > 0 then
		totalPage = totalPage +1
	end
	local currentPage = math.modf(MailUILayer.startMailIndex/4) + 1
	if self.uiLayout ~= nil then
		local pageLable = self.uiLayout:FindChildObjectByName("pageNumLabel").__UILabel__
		if pageLable ~= nil then
			if totalPage ~= 0 then
				pageLable:setString(currentPage .. "/" .. totalPage)
			else
				pageLable:setString("")
			end
		end
	end
end

function MailUILayer:unSelectLastBt(index)
	if self.uiLayout ~= nil then
		if index ~= 0 then
			local uiCon = self.uiLayout:FindChildObjectByName("mailBt" .. index)
			if uiCon ~= nil then
				local lastBt = uiCon.__UIButton__
				if lastBt ~= nil then
					lastBt:getMenuItemSprite():unselected()
				end
			end
		end
	end
end

function MailUILayer:setMenuItemStatus(index)
	if self.uiLayout ~= nil then
		if index ~= self.currentBtIndex then
			self:unSelectLastBt(self.currentBtIndex)
		end
		local currentBt = self.uiLayout:FindChildObjectByName("mailBt" .. index).__UIButton__
		if currentBt ~= nil then
			currentBt:getMenuItemSprite():selected()
		end
	end 
end

function MailUILayer:showOneMailBasicInfo(mailInfo,mailIndex,index)
	--CCLuaLog("MailUILayer:showOneMailBasicInfo: " .. mailIndex .. "," .. index)
	if self.mailIndexTable ~= nil then
		self.mailIndexTable[index] = mailIndex
	end
	if mailInfo ~= nil and self.uiLayout ~= nil then
		local picture = self.uiLayout:FindChildObjectByName("mailIcon" .. index).__UIPicture__
		if picture ~= nil then
			local strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_OPEN"
			if mailInfo.mail_status == 0 or mailInfo.mail_status == 1 then
				strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_NEW"
			end
			local mailIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", strmailIcon)
			picture:setSprite(mailIconSprite)
			picture:setVisible(true)
		end
		local titleLable = self.uiLayout:FindChildObjectByName("mailText" .. index).__UILabel__
		if titleLable ~= nil then
			titleLable:setString(mailInfo.title)
			titleLable:setVisible(true)
		end
		local bt = self.uiLayout:FindChildObjectByName("mailBt" .. index).__UIButton__
		if bt~= nil then
			bt:setVisible(true)
			local menuItem = bt:getMenuItemSprite()
			menuItem:setTag(index)
		end
	else
		--CCLuaLog("mailInfo is nil")
		local picture = self.uiLayout:FindChildObjectByName("mailIcon" .. index).__UIPicture__
		local titleLable = self.uiLayout:FindChildObjectByName("mailText" .. index).__UILabel__
		local bt = self.uiLayout:FindChildObjectByName("mailBt" .. index).__UIButton__
		if picture~= nil then
			picture:setVisible(false)
		end
		if titleLable~= nil then
			titleLable:setVisible(false)
		end
		if bt~= nil then
			bt:setVisible(false)
		end
	end
end

function MailUILayer:showCurrentMailTitle(title)
	if self.uiLayout ~= nil then
		local titleLable = self.uiLayout:FindChildObjectByName("mailTitle").__UILabel__
		if titleLable ~= nil then
			titleLable:setString(title)
		end
	end
end

function MailUILayer:showCurrentMailTime(time)
	if self.uiLayout ~= nil then
		local titleLable = self.uiLayout:FindChildObjectByName("mailTimeLable").__UILabel__
		if titleLable ~= nil then
			titleLable:setString(time)
		end
	end
end

function MailUILayer:showCurrentMailSender(name)
	if self.uiLayout ~= nil then
		local titleLable = self.uiLayout:FindChildObjectByName("mailSender2Lable").__UILabel__
		if titleLable ~= nil then
			titleLable:setString(name)
		end
	end
end

function MailUILayer:showCurrentMailContent(content)
	--CCLuaLog("MailUILayer:showCurrentMailContent:" .. content)
	if self.uiLayout ~= nil then
		local contentList = self.uiLayout:FindChildObjectByName("mailContentList").__UIScrollList__
		if contentList ~= nil then
			contentList:clearChildrens()
			contentList:setStartOffset(ccp(0,0))
			local contentSize = contentList:getContentSize()
			local listWidth = contentSize.width
			local scale  = TXGUI.UIManager:sharedManager():getScaleFactor()
			local width = listWidth / scale
			contentSize.height = 0
			contentSize.width = width
			local label = TXGUI.UILabelTTF:create(content,KJLinXin,24,contentSize,0,1)
			local size = label:getContentSize()
			if size.width ~= 0 and size.height ~= 0 then
				contentList:AddBaseItem(size,label)
			end
		end
	end
end

function MailUILayer:showAttachMent(attachmentInfo)
	if self.uiLayout ~= nil then
		local lable = self.uiLayout:FindChildObjectByName("attachmentLable").__UILabel__
		local contentList = self.uiLayout:FindChildObjectByName("mailAttachmentList").__UIScrollList__
		local getBt = self.uiLayout:FindChildObjectByName("getBt").__UIButton__
		if attachmentInfo ~= nil then
			if lable ~= nil then
				lable:setVisible(true)
			end
			local hasAttachment = false
			if contentList ~= nil then
				contentList:setVisible(true)
				contentList:clearChildrens()
				self.attachmentPos = {}
				for k,v in pairs(attachmentInfo) do
					hasAttachment = true
					local itemID = v.item_id
					local itemNum = v.item_num
					local sprite = ItemManager:Get():getIconSpriteById(itemID)
					local fairyInfo = getFairyBasicInfo(itemID)
					local btSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin", "map_ui2_FRAME_MAIL_FRAME_ATTACHMENT1")
					if btSprite ~= nil then
						local button = contentList:AddNewItem(btSprite,btSprite,"","",ccc3(255,255,255),-132)
						local menuItem = button:getMenuItemSprite()
						menuItem:setTag(itemID)
						menuItem:registerScriptTapHandler(onClickAttachmentBt)
						self.attachmentPos[itemID] = button:getMenuNode()
						if fairyInfo == nil then
							button:getAnimationNode():addChild(sprite,2)
							--local quility = 0
							--if Items[itemID] ~= nil then
							--	quility = Items[itemID]["Quality"]
							--end
							--local frameSprite = ItemManager:Get():getIconFrame(quility)
							--button:setFrameSprite(frameSprite)
						else
							local fairyIcon = fairyInfo:getFairyIcon()
							if fairyIcon ~= nil then
								sprite = fairyIcon
							end
							if sprite ~= nil then
								button:getAnimationNode():addChild(sprite,2)
							end
						end
						local label = TXGUI.UILabelTTF:create(itemNum,KJLinXin,20)
						label:setAnchorPoint(ccp(1,0));
						local x = btSprite:getContentSize().width * 0.4;
						local y = btSprite:getContentSize().height * 0.4;
						label:setPosition(ccp(x,-y));
						button:getAnimationNode():addChild(label,4)
						
					end
				end
				if getBt ~= nil then
					getBt:setVisible(hasAttachment)
				end
				--local size = contentList:getContentSize()
				---local viewSize = contentList:getViewSize()
				--local startPoint = contentList:getContentOffset()
				--local offset = ccp(startPoint.x + size.width - viewSize.width,startPoint.y)
				contentList:setStartOffset(ccp(0,0))
			end
		else
			if lable ~= nil then
				lable:setVisible(false)
			end
			if contentList ~= nil then
				contentList:setVisible(false)
			end
			if getBt ~= nil then
				getBt:setVisible(false)
			end
		end
	end
end

function MailUILayer:setAlreadyGot(isGot)
	if self.uiLayout ~= nil then
		local pic = self.uiLayout:FindChildObjectByName("alreadyGotPic").__UIPicture__
		if pic ~= nil then
			if isGot then
				pic:setVisible(true)
			else
				pic:setVisible(false)
			end
		end
	end
end

function MailUILayer:freshMailInfoList()
	--CCLuaLog("MailUILayer:freshMailInfoList")
	if self.uiLayerInstance ~= nil then
		local index = 1
		self:unSelectLastBt(self.currentBtIndex)
		self.startMailIndex = 0
		self.overMailIndex = 0
		self.currentBtIndex = 0
		self.mailIndexTable = {}
		self:showMailDetail(nil)
		if MailInfoList ~= nil then
			local n = table.getn(MailInfoList)
			CCLuaLog("MailInfoList: " .. n)
			--if n > 0 then 
				for k,v in pairs(MailInfoList) do
					if index <= 4 then
						if self.startMailIndex == 0 or self.startMailIndex > v.index then
							self.startMailIndex = v.index
						end
						self:showOneMailBasicInfo(v,v.index,index)
						if v.index > self.overMailIndex then
							self.overMailIndex = v.index
						end
						index = index + 1
					end
				end
			--end
		end
		if index <= 4 then
			for i = index,4 do
				self:showOneMailBasicInfo(nil,0,i)
			end
		end
		self:setPageButtonStatus()
		if self.startMailIndex ~= 0 then
			onClickMailListBt(1)
		end
		self:showPageNum()
	end
	--CCLuaLog("startMailIndex: " .. self.startMailIndex .. ", overMailIndex: " .. self.overMailIndex)
end

function MailUILayer:setDeleteBtVisible(isVisible)
	local deleteBt = self.uiLayout:FindChildObjectByName("deleteBt").__UIButton__
	if deleteBt ~= nil then
		deleteBt:setVisible(isVisible)
	end
end

function MailUILayer:showMailDetail(info)
	--CCLuaLog("MailUILayer:showMailDetail")
	if self.uiLayerInstance ~= nil then
		if info ~= nil then
			self.currentMailId_high = info.mail_id_high
			self.currentMailId_low = info.mail_id_low
			self:showCurrentMailTitle(info.title)
			local year,mon,day = GameApi:GetTimeBySec(info.mail_id_high)
			local time = year .. "-" .. mon .. "-" .. day
			self:showCurrentMailTime(time)
			self:showCurrentMailSender(info.from)
			self:showCurrentMailContent(info.content)
			self:showAttachMent(info.items)
			self:setCurrentMailStatus(info.mail_id_high,info.mail_id_low,info.mail_status)
			self:setDeleteBtVisible(true)
		else
			self.currentMailId_high = 0
			self.currentMailId_low = 0
			self:showCurrentMailTitle("")
			self:showCurrentMailTime("")
			self:showCurrentMailSender("")
			self:showCurrentMailContent("")
			self:showAttachMent(nil)
			self:setCurrentMailStatus(0,0,0)
			self:setDeleteBtVisible(false)
		end
	end
end

function MailUILayer:onReceiveAttachmentGot()
	if self.getAttachmentIndex == 1 then
		local str = LanguageLocalization:GetLocalization("M_MESSAGE_DELETED_TIPS_3")
		MessageBox:Show(str,onClickConfirmToDelete,nil,MB_YESNO,ccc3(255,255,255),26)
	elseif self.getAttachmentIndex == 2 then
		local str = LanguageLocalization:GetLocalization("M_MESSAGE_DELETED_TIPS_4")
		MessageBox:Show(str,onClickConfirmToDeleteMailWithoutAttach,nil,MB_YESNO,ccc3(255,255,255),26)
	end
end

function MailUILayer:setCurrentMailStatus(id_high,id_low,status)
	--CCLuaLog("MailUILayer:setCurrentMailStatus: " .. status)
	if self.uiLayerInstance ~= nil then
		if self.currentMailId_high == id_high and self.currentMailId_low == id_low then
			local getBt = self.uiLayout:FindChildObjectByName("getBt").__UIButton__
			if status == 3 then
				self:setAlreadyGot(true)
				if getBt ~= nil then
					getBt:setEnabled(false)
					getBt:setVisible(true)
				end
			else
				if getBt ~= nil then
					getBt:setEnabled(true)
				end
				self:setAlreadyGot(false)
			end
			if status ~= 0 and status ~= 1 and self.currentBtIndex ~= nil then
				--CCLuaLog("self.currentBtIndex: " .. self.currentBtIndex)
				--local mailIndex = 0
				--if self.mailIndexTable ~= nil and self.mailIndexTable[self.currentBtIndex] ~= nil then
				--	mailIndex = self.mailIndexTable[self.currentBtIndex]
				--end
				if mailIndex ~= 0 then
					local index = self.currentBtIndex
					local picture = self.uiLayout:FindChildObjectByName("mailIcon" .. index).__UIPicture__
					if picture ~= nil then
						local strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_OPEN"
						local mailIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", strmailIcon)
						picture:setSprite(mailIconSprite)
						picture:setVisible(true)
					end
				end
			end
		else
			self:setNotCurrentMailStatus(id_high,id_low,status)
		end
	end
end

function MailUILayer:setNotCurrentMailStatus(id_high,id_low,status)
	local mailIndex = 0
	local btIndex = 0
	if MailInfoList ~= nil then
		for k,v in pairs(MailInfoList) do
			if v.mail_id_high == id_high and v.mail_id_low == id_low then
				mailIndex = v.index
				break
			end
		end
	end
	if self.mailIndexTable ~= nil then
		for k,v in pairs(self.mailIndexTable) do
			if v == mailIndex then
				btIndex = k
				break
			end
		end
	end
	if btIndex ~= 0 and (status == PB_MAIL_STATUS_GOT or status == PB_MAIL_STATUS_READ) then
		local control = self.uiLayout:FindChildObjectByName("mailIcon" .. btIndex)
		if control ~= nil then
			picture = control.__UIPicture__
			if picture ~= nil then
				local strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_OPEN"
				local mailIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", strmailIcon)
				picture:setSprite(mailIconSprite)
				picture:setVisible(true)
			end
		end
	end
end

function MailUILayer:setPageButtonStatus()
	local userInfo = UserData:GetUserInfo()
	local totalMail = userInfo.m_totalMailNum
	if self.uiLayout ~= nil then
		local nextPageBt = self.uiLayout:FindChildObjectByName("NextPageBt").__UIButton__
		local lastPageBt = self.uiLayout:FindChildObjectByName("LastPageBt").__UIButton__
		if nextPageBt ~= nil and lastPageBt ~= nil then
			if totalMail > self.overMailIndex then
				nextPageBt:setVisible(true)
			else
				nextPageBt:setVisible(false)
			end
			if self.startMailIndex > 1 then
				lastPageBt:setVisible(true)
			else
				lastPageBt:setVisible(false)
			end
		end
	end
end

function MailUILayer:InitUI()
	self.mailBtList = {}
	self.mailIconList = {}
	self.mailTextList = {}
	for i = 1,4 do
		local bt = self.uiLayout:FindChildObjectByName("mailBt" .. i).__UIButton__
		local menuItem = bt:getMenuItemSprite()
		menuItem:registerScriptTapHandler(onClickMailListBt)
		bt:setVisible(false)
	end
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local getBt = self.uiLayout:FindChildObjectByName("getBt").__UIButton__
	if getBt ~= nil then
		local getItem = getBt:getMenuItemSprite()
		getItem:registerScriptTapHandler(onClickGetAttachmentBt)
		getBt:setVisible(false)
	end
	local deleteBt = self.uiLayout:FindChildObjectByName("deleteBt").__UIButton__
	if deleteBt ~= nil then
		local deleteItem = deleteBt:getMenuItemSprite()
		deleteItem:registerScriptTapHandler(onClickDeleteMailBt)
		deleteBt:setVisible(false)
	end
	local nextPageBt = self.uiLayout:FindChildObjectByName("NextPageBt").__UIButton__:getMenuItemSprite()
	nextPageBt:registerScriptTapHandler(onClickNextPageBt)
	local lastPageBt = self.uiLayout:FindChildObjectByName("LastPageBt").__UIButton__:getMenuItemSprite()
	lastPageBt:registerScriptTapHandler(onClickLastPageBt)
	local getALLBt = self.uiLayout:FindChildObjectByName("getALLBt").__UIButton__:getMenuItemSprite()
	getALLBt:registerScriptTapHandler(onClickGetAllBt)

	local pic = self.uiLayout:FindChildObjectByName("alreadyGotPic").__UIPicture__
	if pic ~= nil then
		local strmailIcon = "map_ui_system_icon_FRAME_ICON_ALREADYGOT"
		local iconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", strmailIcon)
		pic:setSprite(iconSprite)
		pic:setVisible(false)
	end	
end

function MailUILayer:InitCurrentMail()
	local userInfo = UserData:GetUserInfo()
	local totalMail = userInfo.m_totalMailNum
	local index = totalMail
	--CCLuaLog("totalMailNum: " .. index)
	if index > 4 then
		index = 4
	end
	if index >0 then
		sendGetMailListReq(1,index)
		self.startMailIndex = 1
		if index > self.overMailIndex then
			self.overMailIndex = index
		end
	end
	self.currentBtIndex = 0
	self:setPageButtonStatus()
end

function MailUILayer:InitLayer()
	--self.uiList = self.uiLayout:FindChildObjectByName("contentList").__UIScrollList__
	
	--self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);


	self:InitUI()
	self:InitCurrentMail()
	PushOneLayer(self.uiLayerInstance,"","")

end