require("Script/Fairy/FairyConfigTable")

FairySelectListItem = {
	baseNode = nil,
	selectMenuItem = nil,
	framMenuItem = nil,
	isSelectFlag = false,
    index = 0,
}

function FairySelectListItem:create(parentNode,fairyInfo,index,IsShowExp)
	local item = setmetatable({},self)
	self.__index = self
	item:init(parentNode,fairyInfo,index,IsShowExp)
	return item
end

-- 重置参数
function FairySelectListItem:resetValue()
	self.baseNode = nil
	self.selectMenuItem = nil
	self.framMenuItem = nil
	self.isSelectFlag = false
    self.index = 0
end

function FairySelectListItem:setButtonCallBack(func)
	if self.selectMenuItem ~= nil then
		self.selectMenuItem:registerScriptTapHandler(func)
	end
	if self.framMenuItem ~= nil then
		self.framMenuItem:registerScriptTapHandler(func)
	end
end

function FairySelectListItem:setItemStatus(isSelected)
	self.isSelectFlag = isSelected
	if isSelected then
		if self.selectMenuItem ~= nil then
			self.selectMenuItem:selected()
		end
		if self.framMenuItem ~= nil then
			self.framMenuItem:selected()
		end
	else
		if self.selectMenuItem ~= nil then
			self.selectMenuItem:unselected()
		end
		if self.framMenuItem ~= nil then
			self.framMenuItem:unselected()
		end
	end
end

function FairySelectListItem:init(parentNode,fairyInfo,index,IsShowExp)
	self:initItemUI(parentNode,fairyInfo,index,IsShowExp)
end

function FairySelectListItem:initItemUI(parentNode,fairyInfo,index,IsShowExp)
	local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME")
	local fairyBasicInfo = getFairyBasicInfo(fairyInfo.fairyId)
	self.baseNode = CCNode:create()
	self.index = index
    if fairyBasicInfo ~= nil then
		parentNode:addChild(self.baseNode)

		local framePic,framePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1")
		local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_CLICKED")
		if framePic ~= nil then
			self.baseNode:setContentSize(framePic:getContentSize())
			--self.framMenuItem = CCMenuItemSprite:create(framePic,clickedFrame)
			--local bt = UIMenu:create()
			--bt:addChild(self.framMenuItem)
			--bt:setInitPriority(-134)
			--bt:setPosition(ccpSub(framePt,bgPt))
			--self.baseNode:addChild(bt,10)
			--self.framMenuItem:setTag(fairyInfo.pos)
			local bt = IconButton:create(framePic,nil,clickedFrame,-134)
			bt:setPosition(ccpSub(framePt,bgPt))
			bt:SetButtonName("FairySelect_" .. index)
			self.baseNode:addChild(bt,10)
			self.framMenuItem = bt:getMenuItem()
			self.framMenuItem:setTag(fairyInfo.pos)
			bt:setContentSize(self.framMenuItem:getContentSize())
		end

		local headFrameRect,headFramePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_ICON")
		local headIcon = fairyBasicInfo:getFairyIcon()
		if headIcon ~= nil then
			headIcon:setPosition(ccpSub(headFramePt,bgPt))
			self.baseNode:addChild(headIcon,12)
		end

		local nameRect,namelabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_NAME")
		if nameRect ~= nil then
			local str = fairyBasicInfo.name
			if fairyInfo.fairyGrade > 0 then
				str = str .. "+" .. fairyInfo.fairyGrade
			end
			local nameLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22)
			nameLabel:setPosition(ccpSub(namelabelPt,bgPt))
			nameLabel:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))
			self.baseNode:addChild(nameLabel,12)
		end

		local levelLabelRect,levelLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_LV")
		if levelLabelRect ~= nil then
			local levelLabel = TXGUI.UILabelTTF:create("Lv. " .. fairyInfo.fairyLevel,KJLinXin,22)
			levelLabel:setPosition(ccpSub(levelLabelPt,bgPt))
			levelLabel:setColor(ccc3(255,255,255))
			self.baseNode:addChild(levelLabel,12)
		end

		local qualityLabelRect,qualityLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_QUALITY")
		if levelLabelRect ~= nil then
			local qualityLabel = TXGUI.UILabelTTF:create(getFairyQualityStr(fairyBasicInfo.quality),KJLinXin,22)
			qualityLabel:setPosition(ccpSub(qualityLabelPt,bgPt))
			qualityLabel:setColor(ccc3(255,255,255))
			self.baseNode:addChild(qualityLabel,12)
		end

		if IsShowExp then
			local experienceLabelRect,experienceLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_EXPERIENCE")
			if experienceLabelRect ~= nil then
				local exp = getExpFairyApply(fairyInfo.fairyLevel,fairyBasicInfo.quality,fairyInfo.fairyId)
				local experienceLabel = TXGUI.UILabelTTF:create(exp,KJLinXin,22)
				experienceLabel:setPosition(ccpSub(experienceLabelPt,bgPt))
				experienceLabel:setColor(ccc3(255,255,255))
				self.baseNode:addChild(experienceLabel,12)
			end

			local experienceIcon,experienceIconPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_EXPERIENCE_ICON")
			if experienceIcon ~= nil then
				experienceIcon:setPosition(ccpSub(experienceIconPt,bgPt))
				self.baseNode:addChild(experienceIcon,12)
			end
		end

		local chooseIcon,chooseIconPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_CHOOSE")
		if chooseIcon ~= nil then
			chooseIcon:setPosition(ccpSub(chooseIconPt,bgPt))
			self.baseNode:addChild(chooseIcon,12)
		end

		local selectIcon,selectIconPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_FRAME1_SELECT")
		if selectIcon ~= nil then
			local normalIcon = CCSprite:create()
			normalIcon:setContentSize(selectIcon:getContentSize())
			self.selectMenuItem = CCMenuItemSprite:create(normalIcon,selectIcon)
			local bt = UIMenu:create()
			bt:addChild(self.selectMenuItem)
			bt:setInitPriority(-135)
			bt:setPosition(ccpSub(selectIconPt,bgPt))
			self.baseNode:addChild(bt,15)
			self.selectMenuItem:setTag(fairyInfo.pos)
			
		end

		local statusSprite = self:getStatusMarkIcon(fairyInfo.pos)
		if statusSprite ~= nil then
			local statusRect,statusPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_ICON_STATE")
			self.baseNode:addChild(statusSprite,20)
			statusSprite:setPosition(ccpSub(statusPt,bgPt))
		end
	end
end

function FairySelectListItem:getStatusMarkIcon(pos)
	local sprite = nil
	if isBattleFairy(pos) then
		sprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_ICON_STATE1")
	elseif  isAssistFairy(pos) then
		sprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_ICON_STATE2")
	elseif  isFriendlyFairy(pos) then
		sprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_CHOOSE_ICON_STATE3")
	end
	return sprite
end