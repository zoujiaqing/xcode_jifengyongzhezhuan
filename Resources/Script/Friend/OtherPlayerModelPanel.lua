-- 玩家模型面板
OtherPlayerModelPanel = {
	scrollPage = nil,
	uiContainer = nil,
	playerModel = nil,
	playerLayer = nil,
}

function OtherPlayerModelPanel:create(extension)
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

function OtherPlayerModelPanel:init(uiLayout)
	local container = uiLayout:FindChildObjectByName("equipContainer").__UIContainer__
	if container ~= nil then
		self.uiContainer = container
	end
	local page = uiLayout:FindChildObjectByName("modelScrollPage").__UIScrollPage__
	if page ~= nil then
		self.scrollPage = page
		self.scrollPage:setTouchStolenDistance(10)
	end
end

function OtherPlayerModelPanel:registerPageChangeCallback(func)
	if self.scrollPage ~= nil then
		self.scrollPage:registerPageChangedCallbackFunc(func)
	end
end

function OtherPlayerModelPanel:moveToPage(page)
	if self.scrollPage ~= nil then
		self.scrollPage:moveToPage(page,false)
	end
end

function OtherPlayerModelPanel:getCurPage()
	local page = 0
	if self.scrollPage ~= nil then
		page = self.scrollPage:getCurPage()
	end
	return page
end

function OtherPlayerModelPanel:createModels(weaponId,heroType,fairyTable)
	if self.scrollPage ~= nil then
		self.playerLayer = self:createPlayerModel(heroType,weaponId)
		self:createElfModel(FAIRY_POS_START_BTL,fairyTable)
		self:createElfModel(FAIRY_POS_START_BTL+1,fairyTable)
		self:createElfModel(FAIRY_POS_START_AST,fairyTable)
		self:createElfModel(FAIRY_POS_START_AST+1,fairyTable)
		self:createFriendModel()

		self.scrollPage:moveToPage(0)
	end
end

function OtherPlayerModelPanel:createFriendModel()
	local layer = CCLayer:create()
	self.scrollPage:addPage(layer, true)
end

function OtherPlayerModelPanel:createPlayerModel(type,weaponId)
	local layer = CCLayer:create()
	--local playerId = PLAYER_ID:new()
	--playerId:setPlayId(10000,0,0)
	local playerId = PlayerIdMake(10000,0,0)
	self.playerModel = HeroRoleManager:Get():createRole(playerId, "",type, false,false)
	if self.playerModel ~= nil then
		self.playerModel:SetAnim(1000,0,true)
		if weaponId ~= 0 then
			self.playerModel:SetEquipItemsData(weaponId)
		end
		local scale = TXGUI.UIManager:sharedManager():getScaleFactor()
		self.playerModel:getRoot():setScale(1.5)
		layer:addChild(self.playerModel)
		local pt = ccp(0,self.scrollPage:getContentSize().height * (-0.3) / scale)
		self.playerModel:setPosition(pt)
	end
	self.scrollPage:addPage(layer, true)
	return layer
end

function OtherPlayerModelPanel:createElfModel(fairyPos,fairyTable)
	local layer = CCLayer:create()
	if fairyTable ~= nil and fairyTable[fairyPos] ~= nil then
		local fairyId = fairyTable[fairyPos].fairyId
		if fairyId ~= 0 then
			SpriteFactory:sharedFactory():ShowElfOnLayerAtPoint(layer,fairyId,ccp(0,0))
		end
	end
	self.scrollPage:addPage(layer, true)
	return layer
end
