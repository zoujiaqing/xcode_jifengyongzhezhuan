require("Script/Friend/PlayerItemCenter")
-- ÕÊº“ Ù–‘√Ê∞Â
OtherPlayerAttrPanel = {
	uiList	= nil,
	uiContainer = nil,
	--playerPic = nil,
	--playerPicInitPosX = nil,
	--playerPicInitPosY = nil,
}

function OtherPlayerAttrPanel:create(extension)
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

function OtherPlayerAttrPanel:init(uilist,uiContainer)
	self:resetValue()
	if uilist ~= nil and uiContainer ~= nil then
		self.uiList = uilist
		self.uiContainer = uiContainer
		--self.playerPic = pic
		--local pos = self.playerPic:getPosition()
		--playerPicInitPosX = pos.x
		--playerPicInitPosY = pos.y
	end
end

function OtherPlayerAttrPanel:resetValue()
	if self.uiList ~= nil then
		self.uiList:clearChildrens()
	end
	self.uiList = nil
	self.uiContainer = nil
	--self.playerPic = nil
end

function OtherPlayerAttrPanel:setVisible(isVisible)
	if self.uiContainer ~= nil then
		self.uiContainer:setVisible(isVisible)
	else
		CCLuaLog("self.uiContainer = nil")
	end
end

function OtherPlayerAttrPanel:setEffectiveness(uid)
	local playerItems = getPlayerItems(uid)
	if playerItems ~= nil then
		self:addOneAttribute(GM_ATTR_ABILITY_ALL_UPDATE,"M_ATTRIBUTE_ALL",playerItems.effectiveness,"M_ATTRIBUTE_ALL_DESCRIPTION");
	end
end

function OtherPlayerAttrPanel:setUserInfo(uid)
	if self.uiList ~= nil then
		self.uiList:clearChildrens()

		local userInfo = UserData:Get():GetUserInfo(uid)
		if userInfo ~= nil then
			local effectiveness = 0
			local playerItems = getPlayerItems(uid)
			if playerItems ~= nil then
				effectiveness = playerItems.effectiveness
			end
			self:addOneAttribute(GM_ATTR_ABILITY_ALL_UPDATE,"M_ATTRIBUTE_ALL",effectiveness,"M_ATTRIBUTE_ALL_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_PHYSICAL_ATTACK_UPDATE,"M_ATTRIBUTE_PHYATTACK",userInfo.physical_attack,"M_ATTRIBUTE_PHYATTACK_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_MAGIC_ATTACK_UPDATE,"M_ATTRIBUTE_MAGATTACK",userInfo.magic_attack,"M_ATTRIBUTE_MAGATTACK_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_SKILL_ATTACK_UPDATE,"M_ATTRIBUTE_SKIATTACK",userInfo.skill_attack,"M_ATTRIBUTE_SKIATTACK_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_PHYSICAL_DEFENCE_UPDATE,"M_ATTRIBUTE_PHYDEFENCE",userInfo.physical_defence,"M_ATTRIBUTE_PHYDEFENCE_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_MAGIC_DEFENCE_UPDATE,"M_ATTRIBUTE_MAGDEFENCE",userInfo.magic_defence,"M_ATTRIBUTE_MAGDEFENCE_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_SKILL_DEFENCE_UPDATE,"M_ATTRIBUTE_SKIDEFENCE",userInfo.skill_defence,"M_ATTRIBUTE_SKIDEFENCE_DESCRIPTION")	
			self:addOneAttribute(GM_ATTR_HEALTH_POINT_UPDATE,"M_ATTRIBUTE_LIFE",userInfo.total_hp,"M_ATTRIBUTE_LIFE_DESCRIPTION");	
			self:addOneAttribute(GM_ATTR_ACCURATE_UPDATE,"M_ATTRIBUTE_ACCURATE",userInfo.accurate,"M_ATTRIBUTE_ACCURATE_DESCRIPTION");	
			self:addOneAttribute(GM_ATTR_DODGE_UPDATE,"M_ATTRIBUTE_DODGE",userInfo.dodge,"M_ATTRIBUTE_DODGE_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_WRECK_UPDATE,"M_ATTRIBUTE_WRECK",userInfo.wreck,"M_ATTRIBUTE_WRECK_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_PARRY_UPDATE,"M_ATTRIBUTE_BLOCK",userInfo.parry,"M_ATTRIBUTE_BLOCK_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_CRITICAL_STRIKE_UPDATE,"M_ATTRIBUTE_CRITICAL",userInfo.critical_strike,"M_ATTRIBUTE_CRITICAL_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_TENACITY_UPDATE,"M_ATTRIBUTE_TENACITY",userInfo.tenacity,"M_ATTRIBUTE_TENACITY_DESCRIPTION");	
			self:addOneAttribute(GM_ATTR_SLAY_UPDATE,"M_ATTRIBUTE_SLAY",userInfo.slay,"M_ATTRIBUTE_SLAY_DESCRIPTION");
			self:addOneAttribute(GM_ATTR_SPEED_UPDATE,"M_ATTRIBUTE_SPEED",userInfo.speed,"M_ATTRIBUTE_SPEED_DESCRIPTION");
			--self:showUserPic(userInfo.type)
		end
	end
end

--function OtherPlayerAttrPanel:showUserPic(type)
--	if self.playerPic ~= nil then	
--		local picName = getCharacterField(type,"attrPic");
--		local pic = CCSprite:create(picName);
--		if pic ~= nil then
--			pic:setAnchorPoint(ccp(0,0));
--			--local oldPointX,oldPointY = self.playerPic:getCurrentNode():getPosition();
--			local x = self.playerPic:getCurrentNode():getContentSize().width * self.playerPic:getCurrentNode():getScaleX();
--			local y = self.playerPic:getCurrentNode():getContentSize().height * self.playerPic:getCurrentNode():getScaleY();
--			self.playerPic:setSprite(pic);
--			pic:setPosition(ccp(playerPicInitPosX - x * 0.5,playerPicInitPosY - y * 0.5));
--		end
--	end
--end

function OtherPlayerAttrPanel:addOneAttribute(attri,name,number,des)
	if self.uiList ~= nil then
		local nameText = Localization[name]["SimpleChinese"]
		local desText = Localization[des]["SimpleChinese"]
		if (nameText ~= nil) and (desText ~= nil) and (number ~= nil) then
			local base = self.uiList:FindChildObjectByName(name)
			if base~= nil then
				local numLabel = base:getCurrentNode():getChildByTag(1)
				if numLabel ~= nil then
					numLabel:setString(number)
				end
			else
				local attrName,namePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_ATTRIBUTE_TEXT_ATTRIBUTE")
				local attrNumber,numberPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_ATTRIBUTE_NUMBER_ATTRIBUTE")
				local attrDes,desPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_ATTRIBUTE_TEXT_ATTRIBUTE_DESCRIPTION")
				local nodeSize = CCSize(attrDes:getContentSize().width,namePt.y - desPt.y +
										attrDes:getContentSize().height * 0.5 + attrName:getContentSize().height * 0.5)
				local baseNode = CCNode:create()
				base = self.uiList:AddBaseItem(nodeSize,baseNode)
				base:setName(name)

				local nameLabel = TXGUI.UILabelTTF:create(nameText,KJLinXin,24,attrName:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
				baseNode:addChild(nameLabel,0,0)
				nameLabel:setPosition(ccp(attrName:getContentSize().width * 0.5 - nodeSize.width * 0.5,nodeSize.height * 0.5 - attrName:getContentSize().height * 0.5))
				nameLabel:setColor(ccc3(238,166,26))

				local numberLabel = TXGUI.UILabelTTF:create(number,KJLinXin,24,attrNumber:getContentSize(),kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter)
				baseNode:addChild(numberLabel,1,1)
				numberLabel:setColor(ccc3(238,166,26))
				numberLabel:setPosition(ccp(nodeSize.width * 0.5 - attrNumber:getContentSize().width * 0.5,nodeSize.height * 0.5 - attrNumber:getContentSize().height * 0.5))

				local desLabel = TXGUI.UILabelTTF:create(desText,KJLinXin,18,attrDes:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter)
				baseNode:addChild(desLabel,2,2)
				desLabel:setPosition(ccp(attrDes:getContentSize().width * 0.5 -nodeSize.width * 0.5,attrDes:getContentSize().height * 0.5 - nodeSize.height * 0.5))
			end
		end
	end
end


