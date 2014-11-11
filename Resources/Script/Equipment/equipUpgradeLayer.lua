require("Script/GameConfig/uiTagDefine")
require("Script/Equipment/EquipConfigTable")
require("Script/Equipment/EquipMessage")
EquipUpgradeLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	equipPos = 0,
	equipId  = 0,
	equipLevel = 0,
	equipQuility = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("equipUpgradeLayout")
	EquipUpgradeLayer:resetValue()
end

function EquipUpgradeLayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["EquipUpgradeLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/equipUpgrade.plist",self.uiLayerInstance, "equipUpgradeLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function EquipUpgradeLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.equipPos = 0
	self.equipId = 0
	self.equipLevel = 0
	self.equipQuility = 0
end

function setUpgradedEquipPos(pos)
	EquipUpgradeLayer:setEquipPos(pos)
	return true
end

function EquipUpgradeLayer:setEquipPos(pos)
	self.equipPos = pos
end

local function onCloseButtonClicked()
	if EquipUpgradeLayer.uiLayerInstance ~= nil then
		EquipUpgradeLayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

local function onUpgradBtClicked()
	sendEquipmentStrengthReq(EquipUpgradeLayer.equipPos,false)
end

local function onAllUpgradeBtClicked()
	sendEquipmentStrengthReq(EquipUpgradeLayer.equipPos,true)
end

function EquipUpgradeLayer:showEquipErrorCode(errcode)
	CCLuaLog("EquipUpgradeLayer:showEquipErrorCode: " .. errcode)
	if errcode == 1 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_NOT_EQUIP_GRID"))
	elseif errcode == 2 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_NOEQUIP"))
	elseif errcode == 3 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_NOT_A_EQUIP"))
	elseif errcode == 4 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_STRENGTHENEQUIPAGE_TEXT_4"))
	elseif errcode == 5 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_NO_MORE_ENHANCETIME"))
	elseif errcode == 6 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_ENHANCE_INCD"))
	elseif errcode == 7 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_STRENGTHENEQUIPAGE_TEXT_2"))
	elseif errcode == 8 then
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_STRENGTHENEQUIPAGE_TEXT_3"))
	end
end

function  EquipUpgradeLayer:playSuccessEffect()
	if EquipUpgradeLayer.uiLayerInstance ~= nil then
		GameAudioManager:sharedManager():playEffect(350006,false)
		local equipPic = self.uiLayout:FindChildObjectByName("equipPic").__UIPicture__
		local particle = equipPic:getCurrentNode():getChildByTag(10010)
		if nil ~= particle then
			equipPic:getCurrentNode():removeChild(particle,true)
		end
	
		particle = ParticleManagerX:sharedManager():getParticles_uieffect("particle_effect_upgradesuccess")
		if nil ~= particle then
			particle:setPosition(ccp(equipPic:getCurrentNode():getContentSize().width * 0.5,equipPic:getCurrentNode():getContentSize().height * 0.5));
			equipPic:getCurrentNode():addChild(particle,100,10010)
		end
	end
end


function EquipUpgradeLayer:showEquipSuccessEffect(level)
	if level ~= 0 then
		self:showEquipLevel(level)
		self:showEquipAttr(level)
		self:showEquipCost(level)
		self:playSuccessEffect()
	end
end


function EquipUpgradeLayer:onReceiveEquipResult(pos,result,level)
	if self.uiLayerInstance ~= nil then
		if pos == self.equipPos then
			if result == 0 then
				self:showEquipSuccessEffect(level)
			else
				self:showEquipErrorCode(result)
			end
		end
	end 
end

function EquipUpgradeLayer:showEquipLevel(level)
	local label = self.uiLayout:FindChildObjectByName("equipLevel").__UILabel__
	if getEquipTypeById(self.equipId) == 2 then
		label:setString("+" .. level)
	else
		local userLevel = UserData:GetUserLevel()
		label:setString(level .. "/" .. userLevel * 2)
	end
end

function EquipUpgradeLayer:showEquipExtraAttr(index,attr,requiredLevel,value)
	--CCLuaLog("showEquipExtraAttr(" .. index .. "," .. attr .. "," .. requiredLevel .. "," .. value ..")")
	local attrLabel = self.uiLayout:FindChildObjectByName("attrLabel" .. index).__UILabel__
	local attrNum = self.uiLayout:FindChildObjectByName("attrNum" .. index).__UILabel__
	local attrNextNum = self.uiLayout:FindChildObjectByName("attrNextNum" .. index).__UILabel__
	local attrLock = self.uiLayout:FindChildObjectByName("attrLock" .. index).__UIPicture__
	if attr ~= nil then
		attrLabel:setString(getEquipAttrStrByTableIndex(attr))
		attrNum:setString("+".. getEquipAttrFormatStr(attr,value))
		attrNextNum:setString(requiredLevel .. LanguageLocalization:GetLocalization("M_DAILYTARGET_OPENLEVEL"))
		attrLabel:setVisible(true)
		attrNum:setVisible(true)
		local level = tonumber(requiredLevel)
		CCLuaLog("userLevel: " .. self.equipLevel .. " , level: " .. level)
		if self.equipLevel >= level then
			attrNextNum:setVisible(false)
			attrLock:setVisible(false)
			attrLabel:setColor(ccc3(0,255,0))
			attrNum:setColor(ccc3(0,255,0))
		else
			attrNextNum:setVisible(true)
			attrLock:setVisible(true)
			attrLabel:setColor(ccc3(255,0,0))
			attrNum:setColor(ccc3(255,0,0))
			attrNextNum:setColor(ccc3(255,0,0))
		end
	else
		attrLabel:setVisible(false)
		attrNum:setVisible(false)
		attrNextNum:setVisible(false)
		attrLock:setVisible(false)
	end
end

function EquipUpgradeLayer:showEquipBasicAttr(index,attr,currentValue,addValue)
	--CCLuaLog("showEquipBasicAttr(" .. index .. "," .. attr .. "," .. currentValue .. "," .. addValue ..")")
	local attrLabel = self.uiLayout:FindChildObjectByName("attrLabel" .. index).__UILabel__
	local attrNum = self.uiLayout:FindChildObjectByName("attrNum" .. index).__UILabel__
	local attrNextNum = self.uiLayout:FindChildObjectByName("attrNextNum" .. index).__UILabel__
	local attrArrow = self.uiLayout:FindChildObjectByName("attrArrow" .. index).__UIPicture__
	if attr ~= nil then
		attrLabel:setString(getEquipAttrStrByTableIndex(attr))
		attrNum:setString("+"..getEquipAttrFormatStr(attr,currentValue))
		attrNextNum:setString("+"..getEquipAttrFormatStr(attr,currentValue+addValue))
		attrLabel:setVisible(true)
		attrNum:setVisible(true)
		attrNextNum:setVisible(true)
		attrArrow:setVisible(true)
	else
		attrLabel:setVisible(false)
		attrNum:setVisible(false)
		attrNextNum:setVisible(false)
		attrArrow:setVisible(false)
	end
end

function EquipUpgradeLayer:showEquipAttr(equipLevel)
	local equipAttrInfo = getEquipInfo(self.equipId)
	if equipAttrInfo ~= nil then
		for i = 1 ,4 do 
			local index = nil
			local currentValue = 0
			local addValue = 0
			if equipAttrInfo.basicInfo[i] ~= nil then
				index = equipAttrInfo.basicInfo[i].index
				currentValue = equipAttrInfo.basicInfo[i].value
			end
			if equipAttrInfo.intensityInfo ~= nil then
				for k,v in pairs(equipAttrInfo.intensityInfo) do 
					if v.index == index then
						addValue = v.value
						break
					end
				end
			end
			currentValue = currentValue + addValue * equipLevel
			self:showEquipBasicAttr(i,index,currentValue,addValue)
		end
		for i = 5,6 do
			local index = nil
			local value = 0
			local level = 0
			if equipAttrInfo.lockedInfo ~= nil then
				if equipAttrInfo.lockedInfo[i - 4] ~= nil then
					index = equipAttrInfo.lockedInfo[i - 4].index
					value = equipAttrInfo.lockedInfo[i - 4].value
					level = equipAttrInfo.lockedInfo[i - 4].level
				end
			end
			self:showEquipExtraAttr(i,index,level,value)
		end
	end
end

function EquipUpgradeLayer:showEquipCost(itemLevel)
	local goldLabel = self.uiLayout:FindChildObjectByName("costGoldLabel").__UILabel__
	local fragLabel = self.uiLayout:FindChildObjectByName("costFragStoneLabel").__UILabel__
	local costFragPic = self.uiLayout:FindChildObjectByName("costFragPic").__UIPicture__
	local owerFragPic = self.uiLayout:FindChildObjectByName("owerFragPic").__UIPicture__
	local owerGoldLabel = self.uiLayout:FindChildObjectByName("owerGoldLabel").__UILabel__
	local owerFragLabel = self.uiLayout:FindChildObjectByName("owerFragLabel").__UILabel__
	local costGold , costFrag = getEquipUpgradeCost(self.equipQuility,itemLevel+1,getEquipTypeById(self.equipId))
	local userInfo = UserData:GetUserInfo()
	local totalGold = userInfo.m_gold
	local totalFrag = userInfo.m_fragStone
	goldLabel:setString(costGold)
	owerGoldLabel:setString(MainMenuLayer:GetNumByFormat(totalGold))
	fragLabel:setString(costFrag)
	owerFragLabel:setString(MainMenuLayer:GetNumByFormat(totalFrag))
	if getEquipTypeById(self.equipId) == 1 then
		fragLabel:setVisible(false)
		costFragPic:setVisible(false)
		owerFragLabel:setVisible(false)
		owerFragPic:setVisible(false)
	else
		fragLabel:setVisible(true)
		costFragPic:setVisible(true)
		owerFragLabel:setVisible(true)
		owerFragPic:setVisible(true)
	end

end

function EquipUpgradeLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local closeBt = self.uiLayout:FindChildObjectByName("strenghBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onUpgradBtClicked)
	local closeBt = self.uiLayout:FindChildObjectByName("allStrenghBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onAllUpgradeBtClicked)
	
	if self.equipPos ~= 0 then
		local item = ItemManager:Get():findItemByPos(self.equipPos)
		if item ~= nil then
			self.equipId = item:getItemId()
			self.equipLevel = item.itemLevel
			if self.equipId ~= 0 then
				CCLuaLog("equipID: " .. self.equipId)
				local info = item:getItemInfo()
				if info ~= nil then
					self.equipQuility = info.m_quality
					local equipPic = self.uiLayout:FindChildObjectByName("equipPic").__UIPicture__
					local sprite = ItemManager:Get():getIconSpriteById(self.equipId)
					--local frame = ItemManager:Get():getIconFrame(info.m_quality)
					--if frame ~= nil then
					--	sprite:addChild(frame)
					--	frame:setPosition(sprite:getContentSize().width * 0.5,sprite:getContentSize().height * 0.5)
					--end
					equipPic:setSprite(sprite,false)
					local equipNameLabel = self.uiLayout:FindChildObjectByName("equipName").__UILabel__
					equipNameLabel:setString(info.m_name)
					local equipQualityLabel = self.uiLayout:FindChildObjectByName("qualityLabel").__UILabel__
					local color = ItemManager:Get():getLabelColorByQuality(info.m_quality)
					equipQualityLabel:setString("品质" .. info.m_order)
					equipQualityLabel:setColor(color)
					equipNameLabel:setColor(color)

					self:showEquipLevel(item.itemLevel)
					self:showEquipAttr(item.itemLevel)
					self:showEquipCost(item.itemLevel)
				end
			end
		end
		
	end
end