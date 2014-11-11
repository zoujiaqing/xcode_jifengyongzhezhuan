FairyBagListItem = {
	controlBase = nil,
	baseNode = nil,
	backItem = nil,
	decomposeMenuItem = nil,
	levelUPMenuItem = nil,
	trainMenuItem = nil,
	rankUPMenuItem = nil,
	reBirthMenuItem = nil,
	headIconBt = nil,
}

function FairyBagListItem:create(parentList,fairyInfo,index)
	local item = setmetatable({},self)
	self.__index = self
	item:init(parentList,fairyInfo,index)
	return item
end

-- 重置参数
function FairyBagListItem:resetValue()
	self.controlBase = nil
	self.baseNode = nil
	self.backItem = nil
	self.decomposeMenuItem = nil
	self.levelUPMenuItem = nil
	self.trainMenuItem = nil
	self.rankUPMenuItem = nil
	self.reBirthMenuItem = nil
	self.headIconBt = nil
end

function FairyBagListItem:setDecomposeButtonCallBack(func)
	if self.decomposeMenuItem ~= nil then
		self.decomposeMenuItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:setLevelUpButtonCallBack(func)
	if self.levelUPMenuItem ~= nil then
		self.levelUPMenuItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:setTrainButtonCallBack(func)
	if self.trainMenuItem ~= nil then
		self.trainMenuItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:setRankUpButtonCallBack(func)
	if self.rankUPMenuItem ~= nil then
		self.rankUPMenuItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:setRebirthMenuItem(func)
	if self.reBirthMenuItem ~= nil then
		self.reBirthMenuItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:setHeadButtonCallBack(func)
	if self.headIconBt ~= nil then
		local menuItem = self.headIconBt:getMenuItem()
		menuItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:setBackButtonCallBack(func)
	if self.backItem ~= nil then
		self.backItem:registerScriptTapHandler(func)
	end
end

function FairyBagListItem:showTipIcon(isVisible)
	if self.headIconBt ~= nil then
		if isVisible then
			self.headIconBt:showTipIcon(ccp(-1,0))
		else
			self.headIconBt:hideTipIcon()
		end
	end
end

function FairyBagListItem:getStatusMarkIcon(pos)
	local sprite = nil
	if isBattleFairy(pos) then
		sprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_ICON_FIGHT")
	elseif  isAssistFairy(pos) then
		sprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_ICON_HELP")
	elseif  isFriendlyFairy(pos) then
		sprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_ICON_FRIEND")
	end
	return sprite
end

function FairyBagListItem:init(parentList,fairyInfo,index)
	--CCLuaLog("FairyBagListItem:init(parentList,fairyInfo)")
	if parentList ~= nil and fairyInfo ~= nil then
		local fairyBasicInfo = getFairyBasicInfo(fairyInfo.fairyId)
		if fairyBasicInfo ~= nil then
			local baseNode = CCSprite:create()
			local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_FRAME_ITEM")
			--local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_FRAME_FAIRY1_CLICKED")
			--local button = parentList:AddNewItem(bgFrame,clickedFrame,"","",ccc3(255,255,255),-130)
			self.controlBase = parentList:AddBaseItem(bgFrame:getContentSize(),baseNode)
			--self.controlBase:setTag(fairyInfo.pos)

			local bgPic,bgPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_FRAME_FAIRY1")
			local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_FRAME_FAIRY1_CLICKED")
			--baseNode:addChild(bgPic,0)
			--bgPic:setPosition(ccpSub(bgPicPt,bgPt))
			if bgPic ~= nil then
			local bt = IconButton:create(bgPic,nil,clickedFrame,-130)
				bt:setPosition(ccpSub(bgPicPt,bgPt))
				baseNode:addChild(bt,0)
				self.backItem = bt:getMenuItem()
				self.backItem:setTag(fairyInfo.pos)
			end

			local headFrameRect,headFramePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_ICON_FAIRY1")
			if headFrameRect ~= nil then
				--self.headIconItem = CCMenuItemSprite:create(headFrameRect,headFrameRect)
				--local headIcon = fairyBasicInfo:getFairyIcon()
				--local bt = UIMenu:create()
				--bt:addChild(self.headIconItem)
				--bt:setInitPriority(-130)
				--bt:setPosition(ccpSub(headFramePt,bgPt))
				--baseNode:addChild(bt,15)
				--if headIcon ~= nil then
				--	baseNode:addChild(headIcon,20)
				--	headIcon:setPosition(ccpSub(headFramePt,bgPt))
				--end
				--self.headIconItem:setTag(fairyInfo.pos)

				self.headIconBt = IconButton:create(headFrameRect,nil,headFrameRect,-130)
				self.headIconBt:setPosition(ccpSub(headFramePt,bgPt))
				baseNode:addChild(self.headIconBt,15)
				local menuItem = self.headIconBt:getMenuItem()
				menuItem:setTag(fairyInfo.pos)
				self.headIconBt:setContentSize(menuItem:getContentSize())
				local headIcon = fairyBasicInfo:getFairyIcon()
				if headIcon ~= nil then
					self.headIconBt:SetNormalIcon(headIcon)
				end
			end

			local nameRect,namelabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_FAIRYNAME")
			if nameRect ~= nil then
				local str = fairyBasicInfo.name
				if fairyInfo.fairyGrade > 0 then
					str = str .. "+" .. fairyInfo.fairyGrade
				end
				local nameLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22)
				nameLabel:setPosition(ccpSub(namelabelPt,bgPt))
				nameLabel:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))
				baseNode:addChild(nameLabel,12)
			end

			local levelLabelRect,levelLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_FAIRYLV")
			if levelLabelRect ~= nil then
				local levelLabel = TXGUI.UILabelTTF:create("Lv. " .. fairyInfo.fairyLevel,KJLinXin,24)
				levelLabel:setPosition(ccpSub(levelLabelPt,bgPt))
				levelLabel:setColor(ccc3(255,255,255))
				baseNode:addChild(levelLabel,12)
			end

			local qualityLabelRect,qualityLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_FAIRYQUALITY")
			if levelLabelRect ~= nil then
				local qualityLabel = TXGUI.UILabelTTF:create(getFairyQualityStr(fairyBasicInfo.quality),KJLinXin,24)
				qualityLabel:setPosition(ccpSub(qualityLabelPt,bgPt))
				qualityLabel:setColor(ccc3(255,255,255))
				baseNode:addChild(qualityLabel,12)
			end

			local statusSprite = self:getStatusMarkIcon(fairyInfo.pos)
			if statusSprite ~= nil then
				local statusRect,statusPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_ICON_FAIRYMARK")
				
				baseNode:addChild(statusSprite,20)
				statusSprite:setPosition(ccpSub(statusPt,bgPt))
			end


			--if fairyBasicInfo.quality >=3 and fairyInfo.fairyGrade <1 and fairyInfo.pos >= FAIRY_POS_START_NOM then
			--	local decomposeFrame,decomposePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_DECOMPOSE")
			--	local clickedDecomposeFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_DECOMPOSE_CLICKED")
			--	if decomposeFrame ~= nil then
			--		--self.decomposeMenuItem = CCMenuItemSprite:create(decomposeFrame,clickedDecomposeFrame)
			--		--local bt = UIMenu:create()
			--		--bt:addChild(self.decomposeMenuItem)
			--		--bt:setInitPriority(-130)
			--		--bt:setPosition(ccpSub(decomposePt,bgPt))
			--		--baseNode:addChild(bt,15)
			--		--self.decomposeMenuItem:setTag(fairyInfo.pos)
			--		local bt = IconButton:create(decomposeFrame,nil,clickedDecomposeFrame,-130)
			--		bt:setPosition(ccpSub(decomposePt,bgPt))
			--		bt:SetButtonName("FairyDecompose_" .. index)
			--		baseNode:addChild(bt,15)
			--		self.decomposeMenuItem = bt:getMenuItem()
			--		self.decomposeMenuItem:setTag(fairyInfo.pos)
			--	end
			--	local decomposLabelRect,decomposLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_DECOMPOSE")
			--	if decomposLabelRect ~= nil then
			--		local label = TXGUI.UILabelTTF:create("分解",KJLinXin,28)
			--		label:setPosition(ccpSub(decomposLabelPt,bgPt))
			--		label:setColor(ccc3(255,255,255))
			--		baseNode:addChild(label,20)
			--	end
			--end

			--if fairyBasicInfo.quality >=3 and fairyInfo.fairyGrade >= 1 and fairyInfo.pos >= FAIRY_POS_START_NOM then
			--	local decomposeFrame,decomposePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_DECOMPOSE")
			--	local clickedDecomposeFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_DECOMPOSE_CLICKED")
			--	if decomposeFrame ~= nil then
			--		--self.reBirthMenuItem = CCMenuItemSprite:create(decomposeFrame,clickedDecomposeFrame)
			--		--local bt = UIMenu:create()
			--		--bt:addChild(self.reBirthMenuItem)
			--		--bt:setInitPriority(-130)
			--		--bt:setPosition(ccpSub(decomposePt,bgPt))
			--		--baseNode:addChild(bt,15)
			--		--self.reBirthMenuItem:setTag(fairyInfo.pos)
			--		local bt = IconButton:create(decomposeFrame,nil,clickedDecomposeFrame,-130)
			--		bt:setPosition(ccpSub(decomposePt,bgPt))
			--		bt:SetButtonName("FairyRebirth_" .. index)
			--		baseNode:addChild(bt,15)
			--		self.reBirthMenuItem = bt:getMenuItem()
			--		self.reBirthMenuItem:setTag(fairyInfo.pos)
			--	end
			--	local decomposLabelRect,decomposLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_DECOMPOSE")
			--	if decomposLabelRect ~= nil then
			--		local label = TXGUI.UILabelTTF:create("重生",KJLinXin,28)
			--		label:setPosition(ccpSub(decomposLabelPt,bgPt))
			--		label:setColor(ccc3(255,255,255))
			--		baseNode:addChild(label,20)
			--	end
			--end

			local levelUpFrame,levelUpPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_LU")
			local clickedlevelUpFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_LU_CLICKED")
			if levelUpFrame ~= nil then
				--self.levelUPMenuItem = CCMenuItemSprite:create(levelUpFrame,clickedlevelUpFrame)
				--local bt = UIMenu:create()
				--bt:addChild(self.levelUPMenuItem)
				--bt:setInitPriority(-130)
				--bt:setPosition(ccpSub(levelUpPt,bgPt))
				--baseNode:addChild(bt,15)
				--self.levelUPMenuItem:setTag(fairyInfo.pos)
				local bt = IconButton:create(levelUpFrame,nil,clickedlevelUpFrame,-130)
				bt:setPosition(ccpSub(levelUpPt,bgPt))
				bt:SetButtonName("FairyLevelUp_" .. index)
				baseNode:addChild(bt,15)
				self.levelUPMenuItem = bt:getMenuItem()
				self.levelUPMenuItem:setTag(fairyInfo.pos)
				bt:setContentSize(self.levelUPMenuItem:getContentSize())
			end
			local levelUpLabelRect,levelUpLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_LU")
			if levelUpLabelRect ~= nil then
				local label = TXGUI.UILabelTTF:create(LanguageLocalization:GetLocalization("M_FAIRY_BAG_LU"),KJLinXin,28)
				label:setPosition(ccpSub(levelUpLabelPt,bgPt))
				label:setColor(ccc3(255,255,255))
				baseNode:addChild(label,20)
			end

			local trainFrame,trainPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_TRAIN")
			local clickedTrainFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_TRAIN_CLICKED")
			if trainFrame ~= nil then
				--self.trainMenuItem = CCMenuItemSprite:create(trainFrame,clickedTrainFrame)
				--local bt = UIMenu:create()
				--bt:addChild(self.trainMenuItem)
				--bt:setInitPriority(-130)
				--bt:setPosition(ccpSub(trainPt,bgPt))
				--baseNode:addChild(bt,15)
				--self.trainMenuItem:setTag(fairyInfo.pos)
				local bt = IconButton:create(trainFrame,nil,clickedTrainFrame,-130)
				bt:setPosition(ccpSub(trainPt,bgPt))
				bt:SetButtonName("FairyTrain_" .. index)
				baseNode:addChild(bt,15)
				self.trainMenuItem = bt:getMenuItem()
				self.trainMenuItem:setTag(fairyInfo.pos)
				bt:setContentSize(self.trainMenuItem:getContentSize())
			end
			local trainLabelRect,trainLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_TRAIN")
			if trainLabelRect ~= nil then
				local label = TXGUI.UILabelTTF:create(LanguageLocalization:GetLocalization("M_PTT_BUTTON3"),KJLinXin,28)
				label:setPosition(ccpSub(trainLabelPt,bgPt))
				label:setColor(ccc3(255,255,255))
				baseNode:addChild(label,20)
			end

			local rankUpFrame,rankUpPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_AD")
			local clickedRankUPFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_BUTTON_AD_CLICKED")
			if rankUpFrame ~= nil then
				--self.rankUPMenuItem = CCMenuItemSprite:create(rankUpFrame,clickedRankUPFrame)
				--local bt = UIMenu:create()
				--bt:addChild(self.rankUPMenuItem)
				--bt:setInitPriority(-130)
				--bt:setPosition(ccpSub(rankUpPt,bgPt))
				--baseNode:addChild(bt,15)
				--self.rankUPMenuItem:setTag(fairyInfo.pos)
				local bt = IconButton:create(rankUpFrame,nil,clickedRankUPFrame,-130)
				bt:setPosition(ccpSub(rankUpPt,bgPt))
				bt:SetButtonName("FairyGradeUp_" .. index)
				baseNode:addChild(bt,15)
				self.rankUPMenuItem = bt:getMenuItem()
				self.rankUPMenuItem:setTag(fairyInfo.pos)
				bt:setContentSize(self.rankUPMenuItem:getContentSize())
			end
			local rankUpLabelRect,rankUpLabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_TEXT_AD")
			if rankUpLabelRect ~= nil then
				local label = TXGUI.UILabelTTF:create(LanguageLocalization:GetLocalization("M_FAIRY_BAG_AD"),KJLinXin,28)
				label:setPosition(ccpSub(rankUpLabelPt,bgPt))
				label:setColor(ccc3(255,255,255))
				baseNode:addChild(label,20)
			end
		end

		if fairyInfo.isNewFlag then
			self:showTipIcon(true)
		end
	end
end