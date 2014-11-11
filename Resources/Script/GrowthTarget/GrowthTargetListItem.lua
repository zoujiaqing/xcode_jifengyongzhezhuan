require("Script/Equipment/TipColor")
require("Script/GameConfig/Target")
require("Script/GrowthTarget/GrowthTargetDataCenter")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/Fairy/FairyLuaInterface")

GrowthTargetListItem = {
	baseNode = nil,
	controlBase = nil,
	unlockSystemMenuItem = nil,
	getRewardButton = nil,
	targetID = 0,
	rewardButtonList = {},
	rewardItemList = {},
	desLabel = nil,
	progressLabel = nil,
	getRewardTextLabel = nil,
	systemIcon = nil,
	startTextLabel = nil,
	systemFramePic = nil,
	hintBt = nil,
}

function GrowthTargetListItem:create(parentList,targetId,index)
	local item = setmetatable({},self)
	self.__index = self
	item.rewardButtonList = {}
	setmetatable(item.rewardButtonList,{})
	item.rewardItemList = {}
	setmetatable(item.rewardItemList,{})
	item:init(parentList,targetId,index)
	return item
end

-- 重置参数
function GrowthTargetListItem:resetValue()
	self.baseNode = nil
	self.controlBase = nil
	self.unlockSystemMenuItem = nil
	self.getRewardButton = nil
	self.targetID = 0
	self.rewardButtonList = {}
	self.rewardItemList = {}
	self.desLabel = nil
	self.progressLabel = nil
	self.getRewardTextLabel = nil
	self.systemIcon = nil
	self.startTextLabel = nil
	self.systemFramePic = nil
	self.hintBt = nil
end

function GrowthTargetListItem:onGetRewardButtonClicked(func)
	if self.getRewardButton ~= nil then
		self.getRewardButton:getMenuItem():registerScriptTapHandler(func)
	end
end

function GrowthTargetListItem:onFirstRewardButtonClicked(func)
	if self.rewardButtonList[1] ~= nil then
		self.rewardButtonList[1]:getMenuItem():registerScriptTapHandler(func)
	end
end

function GrowthTargetListItem:onSecondRewardButtonClicked(func)
	if self.rewardButtonList[2] ~= nil then
		self.rewardButtonList[2]:getMenuItem():registerScriptTapHandler(func)
	end
end

function GrowthTargetListItem:onThirdRewardButtonClicked(func)
	if self.rewardButtonList[3] ~= nil then
		self.rewardButtonList[3]:getMenuItem():registerScriptTapHandler(func)
	end
end

function GrowthTargetListItem:onForthRewardButtonClicked(func)
	if self.rewardButtonList[4] ~= nil then
		self.rewardButtonList[4]:getMenuItem():registerScriptTapHandler(func)
	end
end

function GrowthTargetListItem:resetStatus()
	if self.getRewardTextLabel ~= nil then
		local str = ""
		if GrowthTargetDataList[self.targetID].status == 3 or Target[self.targetID]["Link_UI"] == "0" then
			str = LanguageLocalization:GetLocalization("M_UNDERGROUND_GETREWARD")
		else
			str = "前往"
		end
		self.getRewardTextLabel:setString(str)
	end

	if self.getRewardButton ~= nil then
		if GrowthTargetDataList[self.targetID].status == 4 then
			self.getRewardButton:getMenuItem():setEnabled(false)
		end
	end
end


local function onOperationTipCallBackLeft(tag)

end

local function onOperationTipCallBackRight(tag)
	if GrowthTargetLayer.tipFairyId ~= 0 then
		FairyInfoLayer.fairyId = GrowthTargetLayer.tipFairyId
		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 2
		FairyInfoLayer:CreateLayer(nil)
	end
end

function GrowthTargetListItem:showRewardTip(rewardIndex)
	local menuNode = self.rewardButtonList[rewardIndex]:getMenuNode()
	ItemManager:Get():showItemTipsById(self.rewardItemList[rewardIndex].id,menuNode:getTouchPoint())

	local itemId = self.rewardItemList[rewardIndex].id
	local isFairy = G_GetIsFairyById(itemId)
	local info = ItemManager:Get():getItemInfoById(itemId)
	if isFairy or info.m_type == 21 then
		GrowthTargetLayer.tipFairyId = itemId % 10000;
		local detailText = LanguageLocalization:GetLocalization("M_EQUIPMENT_INFORMATION")
		ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onOperationTipCallBackRight,detailText,true)
	end
end

function GrowthTargetListItem:setGetRewardAlready()
	if self.getRewardTextLabel ~= nil then
		local str = ""
		if GrowthTargetDataList[self.targetID].status == 4 then
			str = LanguageLocalization:GetLocalization("M_LOGINREWARD_ALREADYGOT")
		end
		self.getRewardTextLabel:setString(str)
	end

	if self.getRewardButton ~= nil then
		if (GrowthTargetDataList[self.targetID].status == 2 and Target[self.targetID]["Link_UI"] == "0") or (GrowthTargetDataList[self.targetID].status == 4) then
			self.getRewardButton:getMenuItem():setEnabled(false)
		else
			self.getRewardButton:getMenuItem():setEnabled(true) 
		end
	end

	if self.progressLabel ~= nil and self.hintBt ~= nil and GrowthTargetDataList[self.targetID] ~= nil then
			local targetNum = GrowthTargetDataList[self.targetID].tatalProgress
			local currentNum = GrowthTargetDataList[self.targetID].currentProgress
			local str = "[" .. currentNum .. "/" .. targetNum .. "]"
			self.progressLabel:setString(str)
			if GrowthTargetDataList[self.targetID].status == 3 then
				self.progressLabel:setColor(TipColorGreen)
				self.hintBt:showTipIcon(ccp(1,0))
			elseif GrowthTargetDataList[self.targetID].status == 4 then
				self.progressLabel:setColor(TipColorGreen)
				self.hintBt:hideTipIcon()
			else
				self.progressLabel:setColor(TipColorRed)
				self.hintBt:hideTipIcon()
			end
		end
end

function GrowthTargetListItem:resetTargetID(target)
	CCLuaLog("GrowthTargetListItem:resetTargetID: " .. target)
	if Target[target] ~= nil then
		self.targetID = target
		for i = 1,4 do
			local bt = self.rewardButtonList[i]
			bt:getMenuItem():setTag(target)
			if Target[target]["Reward" .. i] ~= nil and Target[target]["Reward" .. i] ~= "0" then
				local rewardId , rewardNum = string.match(Target[target]["Reward" .. i],"(%d+)\/(%d+)")
				local icon = ItemManager:Get():getIconSpriteById(tonumber(rewardId))
				bt:SetNormalIcon(icon)
				bt:setItemLabel(MainMenuLayer:GetNumByFormat(tonumber(rewardNum)),20,KJLinXin)
				bt:setVisible(true)
			else
				bt:setVisible(false)
			end
		end
	
		if self.unlockSystemMenuItem ~= nil then
			self.unlockSystemMenuItem:setTag(target)
		end

		if self.getRewardButton ~= nil then
			self.getRewardButton:getMenuItem():setTag(target)
		end

		if self.desLabel ~= nil then
			local str = ""
			if Target[target]["Target_Description"] ~= nil then
				str = LanguageLocalization:GetLocalization(Target[target]["Target_Description"])
			end
			self.desLabel:setString(str)
		end

		if self.progressLabel ~= nil and self.hintBt ~= nil and GrowthTargetDataList[target] ~= nil then
			local targetNum = GrowthTargetDataList[target].tatalProgress
			local currentNum = GrowthTargetDataList[target].currentProgress
			local str = "[" .. currentNum .. "/" .. targetNum .. "]"
			self.progressLabel:setString(str)
			if GrowthTargetDataList[target].status == 3 then
				self.progressLabel:setColor(TipColorGreen)
				self.hintBt:showTipIcon(ccp(1,0))
			elseif GrowthTargetDataList[target].status == 4 then
				self.progressLabel:setColor(TipColorGreen)
				self.hintBt:hideTipIcon()
			else
				self.progressLabel:setColor(TipColorRed)
				self.hintBt:hideTipIcon()
			end
		end

		if self.getRewardTextLabel ~= nil then
			local str = ""
			if GrowthTargetDataList[target].status == 3 or Target[target]["Link_UI"] == "0" then
				str = LanguageLocalization:GetLocalization("M_UNDERGROUND_GETREWARD")
			else
				str = "前往"
			end
			self.getRewardTextLabel:setString(str)
		end

		if self.getRewardButton ~= nil then
			if GrowthTargetDataList[target].status == 2 and Target[target]["Link_UI"] == "0" then
				self.getRewardButton:getMenuItem():setEnabled(false)
			else
				self.getRewardButton:getMenuItem():setEnabled(true) 
			end
		end

		if self.systemIcon ~= nil then
			self.systemIcon:removeFromParentAndCleanup(true)
			self.systemIcon = nil
		end
		if Target[target]["Open_Systemicon"] ~= nil and Target[target]["Open_Systemicon"] ~= "0" then
			local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_AIM")
			local iconPic,iconPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_UNLOCKSYSTEM")
			local iconName = "map_ui_system_icon_FRAME_" .. string.upper(Target[target]["Open_Systemicon"])
			local icon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",iconName)
			self.systemIcon = icon
			if self.systemIcon ~= nil and self.systemIcon ~= nil then
				self.systemIcon:setPosition(ccpSub(iconPicPt,bgPt))
				self.baseNode:addChild(self.systemIcon,20)
			end
			if self.systemFramePic ~= nil then
				self.systemFramePic:setVisible(true)
			end
			if self.startTextLabel ~= nil then
				self.startTextLabel:setVisible(true)
			end
		else
			if self.systemFramePic ~= nil then
				self.systemFramePic:setVisible(false)
			end
			if self.startTextLabel ~= nil then
				self.startTextLabel:setVisible(false)
			end
		end
	end
end


function GrowthTargetListItem:init(parentList,targetId,index)
	CCLuaLog("GrowthTargetListItem:init: " .. targetId .. "," .. index)
	self.targetID = targetId
	if parentList ~= nil and Target[targetId] ~= nil then
		self.baseNode = CCSprite:create()
		local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_AIM")
		self.controlBase = parentList:AddBaseItem(bgFrame:getContentSize(),self.baseNode)
		-- 背景图片
		local bgPic,bgPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_AIM1")
		if bgPic ~= nil then
			bgPic:setPosition(ccpSub(bgPicPt,bgPt))
			self.baseNode:addChild(bgPic,0)
		end
		--  目标文件描述
		local desRect,deslabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_TEXT_AIMDESCRIPTION")
		if desRect ~= nil and Target[targetId]["Target_Description"] ~= nil then
			local str = LanguageLocalization:GetLocalization(Target[targetId]["Target_Description"])
			self.desLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22,desRect:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
			self.desLabel:setPosition(ccpSub(deslabelPt,bgPt))
			self.desLabel:setColor(ccc3(255,184,0))
			self.baseNode:addChild(self.desLabel,10)
		end
		-- 目标进度
		local progressRect,progressPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_TEXT_AIMPROGRESS")
		local hintRect,hintPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_ICON_HINT")
		if progressRect ~= nil and GrowthTargetDataList[targetId] ~= nil then
			local targetNum = GrowthTargetDataList[targetId].tatalProgress
			local currentNum = GrowthTargetDataList[targetId].currentProgress
			local str = "[" .. currentNum .. "/" .. targetNum .. "]"
			self.progressLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22,progressRect:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
			self.progressLabel:setPosition(ccpSub(progressPt,bgPt))
			self.hintBt = IconButton:create(hintRect,nil,hintRect,0)
			if GrowthTargetDataList[targetId].status == 3 then
				self.progressLabel:setColor(TipColorGreen)
				self.hintBt:showTipIcon(ccp(1,0))
			elseif GrowthTargetDataList[targetId].status == 4 then
				self.progressLabel:setColor(TipColorGreen)
				self.hintBt:hideTipIcon()
			else
				self.progressLabel:setColor(TipColorRed)
				self.hintBt:hideTipIcon()
			end
			self.hintBt:setPosition(ccpSub(hintPt,bgPt))
			self.baseNode:addChild(self.progressLabel,10)
			self.baseNode:addChild(self.hintBt,20)
		end
		-- 奖励文本
		local rewardTextRect,rewardTextPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_TEXT_REWARD")
		if rewardTextRect ~= nil then
			local str = LanguageLocalization:GetLocalization("T_common_text_reward") .. ":"
			local rewardTextLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22,rewardTextRect:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
			rewardTextLabel:setPosition(ccpSub(rewardTextPt,bgPt))
			rewardTextLabel:setColor(ccc3(242,205,36))
			self.baseNode:addChild(rewardTextLabel,10)
		end
		-- 开启功能
		local textRect,textPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_TEXT_UNLOCKSYSTEM")
		if textRect ~= nil then
			local str = "开启功能"
			self.startTextLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22)
			self.startTextLabel:setPosition(ccpSub(textPt,bgPt))
			self.startTextLabel:setColor(TipColorOrange)
			self.baseNode:addChild(self.startTextLabel,10)
		end

		local icon,unlockPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_UNLOCKSYSTEM")
		self.systemFramePic = icon
		if self.systemFramePic ~= nil then
			self.systemFramePic:setPosition(ccpSub(unlockPt,bgPt))
			self.baseNode:addChild(self.systemFramePic,20)
		end

		if Target[targetId]["Open_Systemicon"] ~= nil and Target[targetId]["Open_Systemicon"] ~= "0" then
			local iconPic,iconPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_UNLOCKSYSTEM")
			local iconName = "map_ui_system_icon_FRAME_" .. string.upper(Target[targetId]["Open_Systemicon"])
			local icon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",iconName)
			self.systemIcon = icon
			if bgPic ~= nil and self.systemIcon ~= nil then
				icon:setPosition(ccpSub(iconPicPt,bgPt))
				self.baseNode:addChild(self.systemIcon,20)
			end
		else
			if self.systemFramePic ~= nil then
				self.systemFramePic:setVisible(false)
			end
			if self.startTextLabel ~= nil then
				self.startTextLabel:setVisible(false)
			end
		end
		-- 领取&前往
		local getRewardFrame,getRewardPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_BUTTON_GETREWARD")
		local clickedGetRewardFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_AIM_BUTTON_GETREWARD_CLICKED")
		local cannotGetRewardFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_AIM_BUTTON_GETREWARD_CANT")
		if getRewardFrame ~= nil and GrowthTargetDataList[targetId] ~= nil then
			self.getRewardButton = IconButton:create(getRewardFrame,nil,clickedGetRewardFrame,-130)
			self.getRewardButton:setPosition(ccpSub(getRewardPt,bgPt))
			self.getRewardButton:SetButtonName("getReward_" .. index)
			self.baseNode:addChild(self.getRewardButton,15)
			local menu = self.getRewardButton:getMenuItem()
			menu:setTag(targetId)
			menu:setDisabledImage(cannotGetRewardFrame)
			self.getRewardButton:setContentSize(menu:getContentSize())
			if GrowthTargetDataList[targetId].status == 2 and Target[targetId]["Link_UI"] == "0" then
				menu:setEnabled(false)
			else
				menu:setEnabled(true) 
			end
		end
		local getRewardTextFrame,getRewardTextPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_BUTTON_TEXT")
		if getRewardTextFrame ~= nil and GrowthTargetDataList[targetId] ~= nil then
			local str = ""
			if GrowthTargetDataList[targetId].status == 3 or Target[targetId]["Link_UI"] == "0" then
				str =  LanguageLocalization:GetLocalization("M_UNDERGROUND_GETREWARD")
			else
				str = "前往"
			end
			self.getRewardTextLabel = TXGUI.UILabelTTF:create(str,KJLinXin,22)
			self.getRewardTextLabel:setPosition(ccpSub(getRewardTextPt,bgPt))
			self.getRewardTextLabel:setColor(ccc3(255,255,255))
			self.baseNode:addChild(self.getRewardTextLabel,20)
		end
		-- 奖励
		for i = 1,4 do
			if Target[targetId]["Reward" .. i] ~= nil and Target[targetId]["Reward" .. i] ~= "0" then
				local rewardId , rewardNum = string.match(Target[targetId]["Reward" .. i],"(%d+)\/(%d+)")
				local rewardInfo = {}
				setmetatable(rewardInfo,{})
				rewardInfo.id = tonumber(rewardId)
				rewardInfo.number = tonumber(rewardNum)
				table.insert(self.rewardItemList,rewardInfo)
			end
		end

		for i = 1,4 do
			local rewardFrame,rewardPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_AIM_FRAME_REWARD" .. i)
			local clickedRewardFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_AIM_FRAME_REWARD" .. i)
			if rewardFrame ~= nil then
				local bt = IconButton:create(rewardFrame,nil,clickedRewardFrame,-130)
				bt:setPosition(ccpSub(rewardPt,bgPt))
				self.baseNode:addChild(bt,15)
				local menuItem = bt:getMenuItem()
				menuItem:setTag(targetId)
				self.rewardButtonList[i] = bt
				bt:setContentSize(menuItem:getContentSize())
				--if Target[targetId]["Reward" .. i] ~= nil and Target[targetId]["Reward" .. i] ~= "0" then
				--	local rewardId , rewardNum = string.match(Target[targetId]["Reward" .. i],"(%d+)\/(%d+)")
				--	local icon = ItemManager:Get():getIconSpriteById(tonumber(rewardId))
				--	bt:SetNormalIcon(icon)
				--	bt:setItemLabel(MainMenuLayer:GetNumByFormat(tonumber(rewardNum)),20,KJLinXin)
				if self.rewardItemList[i] ~= nil then
					local icon = ItemManager:Get():getIconSpriteById(self.rewardItemList[i].id)
					bt:SetNormalIcon(icon)
					bt:setItemLabel(MainMenuLayer:GetNumByFormat(self.rewardItemList[i].number),20,KJLinXin)
				else
					bt:setVisible(false)
				end
			end
		end
	end
end