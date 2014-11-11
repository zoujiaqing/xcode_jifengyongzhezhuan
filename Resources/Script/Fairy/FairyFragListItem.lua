require("Script/Fairy/FairyConfigTable")
require("Script/Equipment/TipColor")

FairyFragListItem = {
	baseNode = nil,
	selectMenuItem = nil,
	getwayMenuItem = nil,
	framMenuBt = nil,
	fragPos = 0,
	exchangeId = 0,
	needNum = 0,
	numberLabel = nil,
}

function FairyFragListItem:create(parentNode,fragItem,index)
	local item = setmetatable({},self)
	self.__index = self
	item:init(parentNode,fragItem,index)
	return item
end

-- 重置参数
function FairyFragListItem:resetValue()
	self.baseNode = nil
	self.selectMenuItem = nil
	self.getwayMenuItem = nil
	self.framMenuBt = nil
	self.isSelectFlag = false
	self.numberLabel = nil
	self.fragPos = 0
	self.exchangeId = 0
	self.needNum = 0
end

function FairyFragListItem:updateFragAmount()
	--CCLuaLog("FairyFragListItem:updateFragAmount()")
	if 	self.numberLabel ~= nil then
		local fragItem = ItemManager:Get():findItemByPos(self.fragPos + kFairyFragment)
		if fragItem ~= nil then
			if fragItem.amount >= self.needNum then
				self.numberLabel:setColor(TipColorGreen)
			else
				self.numberLabel:setColor(ccc3(255,255,255))
			end
			self.numberLabel:setString(fragItem.amount .. "/" .. self.needNum)
			--self:showTipIcon(fragItem.amount >= self.needNum)
		end
	end		
			
end

function FairyFragListItem:setTipButtonCallBack(func)
	if self.framMenuBt ~= nil then
		local menuItem = self.framMenuBt:getMenuItem()
		menuItem:registerScriptTapHandler(func)
	end
end

function FairyFragListItem:setExchangeButtonCallBack(func)
	if self.selectMenuItem ~= nil then
		self.selectMenuItem:registerScriptTapHandler(func)
	end
end

function FairyFragListItem:setGetwayButtonCallBack(func)
	if self.getwayMenuItem ~= nil then
		self.getwayMenuItem:registerScriptTapHandler(func)
	end
end 

function FairyFragListItem:showTipIcon(isVisible)
	if self.framMenuBt ~= nil then
		if isVisible then
			self.framMenuBt:showTipIcon(ccp(-1,0))
		else
			self.framMenuBt:hideTipIcon()
		end
	end
end

function FairyFragListItem:init(parentNode,fragItem,index)
	self:initItemUI(parentNode,fragItem,index)
end

function FairyFragListItem:initItemUI(parentNode,fragItem,index)
	local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME")
	self.baseNode = CCNode:create()
	local itemInfo = fragItem:getItemInfo()
	if itemInfo ~= nil then
		parentNode:addChild(self.baseNode)
		self.fragPos = fragItem.position
		local framePic,framePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1")
		local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_CLICKED")
		if framePic ~= nil then
			--self.baseNode:setContentSize(framePic:getContentSize())
			--self.framMenuItem = CCMenuItemSprite:create(framePic,clickedFrame)
			--local bt = UIMenu:create()
			--bt:addChild(self.framMenuItem)
			--bt:setInitPriority(-130)
			--bt:setPosition(ccpSub(framePt,bgPt))
			--self.baseNode:addChild(bt,10)
			--self.framMenuItem:setTag(fragItem:getItemId())

			self.baseNode:setContentSize(framePic:getContentSize())
			self.framMenuBt = IconButton:create(framePic,nil,clickedFrame,-130)
			self.framMenuBt:setPosition(ccpSub(framePt,bgPt))
			self.baseNode:addChild(self.framMenuBt,10)
			local menuItem = self.framMenuBt:getMenuItem()
			menuItem:setTag(fragItem:getItemId())
			self.framMenuBt:setContentSize(menuItem:getContentSize())
		end

		if self.framMenuBt ~= nil then
			local headIcon = ItemManager:Get():getIconSpriteById(fragItem:getItemId())
			if headIcon ~= nil then
				local headFrameRect,headFramePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_ICON")
				self.framMenuBt:SetNormalIcon(headIcon)
				headIcon:setPosition(ccpSub(headFramePt,framePt))
			end
		end
		--local headFrameRect,headFramePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_ICON")
		--local headIcon = ItemManager:Get():getIconSpriteById(fragItem:getItemId())
		--if headIcon ~= nil then
		--	headIcon:setPosition(ccpSub(headFramePt,bgPt))
		--	self.baseNode:addChild(headIcon,12)
		--end

		local nameRect,namelabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_NAME")
		if nameRect ~= nil then
			local nameLabel = TXGUI.UILabelTTF:create(itemInfo.m_name,KJLinXin,20,nameRect:getContentSize(),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter)
			nameLabel:setPosition(ccpSub(namelabelPt,bgPt))
			nameLabel:setColor(ccc3(255,255,255))
			self.baseNode:addChild(nameLabel,12)
		end

		local needNum,fairyId,exchangeId = getFairyfragExchangeInfo(fragItem:getItemId())
		self.exchangeId = exchangeId
		self.needNum = needNum
		local numLabelRect,numLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_LV")
		if numLabelRect ~= nil then
			self.numberLabel = TXGUI.UILabelTTF:create(fragItem.amount .. "/" .. needNum,KJLinXin,28)
			self.numberLabel:setPosition(ccpSub(numLabelPt,bgPt))
			if fragItem.amount >= needNum then
				self.numberLabel:setColor(TipColorGreen)
			else
				self.numberLabel:setColor(ccc3(255,255,255))
			end
			--self:showTipIcon(fragItem.amount >= self.needNum)
			self.baseNode:addChild(self.numberLabel,12)
		end

		local getWayIcon,getWayIconPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_BUTTON1")
		local getWayclickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_BUTTON1_CLICKED")
		if getWayIcon ~= nil then
			--self.selectMenuItem = CCMenuItemSprite:create(exchangeIcon,clickedFrame)
			--local bt = UIMenu:create()
			--bt:addChild(self.selectMenuItem)
			--bt:setInitPriority(-130)
			--bt:setPosition(ccpSub(exchangeIconPt,bgPt))
			--self.baseNode:addChild(bt,15)
			--self.selectMenuItem:setTag(fragItem.position)
			local bt = IconButton:create(getWayIcon,nil,getWayclickedFrame,-130)
			bt:setPosition(ccpSub(getWayIconPt,bgPt))
			bt:SetButtonName("FairyFrag_GetWay" .. index)
			self.baseNode:addChild(bt,15)
			self.getwayMenuItem = bt:getMenuItem()
			self.getwayMenuItem:setTag(fragItem.position)
			bt:setContentSize(self.getwayMenuItem:getContentSize())
		end

		local getwayLabelRect, getwayLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_BUTTON1_TEXT")
		if getwayLabelRect ~= nil then
			local levelLabel = TXGUI.UILabelTTF:create("获取",KJLinXin,28)
			levelLabel:setPosition(ccpSub(getwayLabelPt,bgPt))
			levelLabel:setColor(ccc3(255,255,255))
			self.baseNode:addChild(levelLabel,20)
		end

		local exchangeIcon,exchangeIconPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_BUTTON")
		local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_BUTTON_CLICKED")
		if exchangeIcon ~= nil then
			local bt = IconButton:create(exchangeIcon,nil,clickedFrame,-130)
			bt:setPosition(ccpSub(exchangeIconPt,bgPt))
			bt:SetButtonName("FairyFrag_" .. index)
			self.baseNode:addChild(bt,15)
			self.selectMenuItem = bt:getMenuItem()
			self.selectMenuItem:setTag(fragItem.position)
			bt:setContentSize(self.selectMenuItem:getContentSize())
		end

		local exchangeLabelRect, exchangeLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_TATTER_FRAME1_BUTTON_TEXT")
		if exchangeLabelRect ~= nil then
			local levelLabel = TXGUI.UILabelTTF:create("兑换",KJLinXin,28)
			levelLabel:setPosition(ccpSub(exchangeLabelPt,bgPt))
			levelLabel:setColor(ccc3(255,255,255))
			self.baseNode:addChild(levelLabel,20)
		end
	end
end
