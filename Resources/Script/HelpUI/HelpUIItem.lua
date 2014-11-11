
HelpUIItem = {
	baseNode = nil,
	maxWidth = 0,
	distance = 10,
	titlefontSize = 28,
	textfontSize = 20,
	titleColor = ccc3(190,228,155),
	contentColor = ccc3(255,255,255),
}

function HelpUIItem:create(width,title,content)
	local playerinfo = setmetatable({},self)
	self.__index = self
	playerinfo:init(width,title,content)
	return playerinfo
end

function HelpUIItem:createTitlePoint()
	-- to do
	--local titleSprite = CCSprite:create()
	local titleSprite = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_HELP_ICON_TITLEMARK")
	return titleSprite
end

function HelpUIItem:init(width,title,content)
	self.maxWidth = width
	self.baseNode = CCNode:create()
	local titleSprite = self:createTitlePoint()
	self.baseNode:addChild(titleSprite)
	local titleSpriteLength = titleSprite:getContentSize().width
	
	self.titlefontSize = titleSprite:getContentSize().height
	local listwidth = self.maxWidth - titleSpriteLength
	local size = CCSizeMake(listwidth,0)
	local titleNode = TXGUI.UILabelTTF:create(title,KJLinXin,self.titlefontSize,size,0,1)
	titleNode:setColor(self.titleColor)
	self.baseNode:addChild(titleNode)
	local contentNode = TXGUI.UILabelTTF:create(content,KJLinXin,self.textfontSize,size,0,1)
	self.baseNode:addChild(contentNode)

	--CCLuaLog("titleNode: width: " .. titleNode:getContentSize().width .. "  height: " .. titleNode:getContentSize().height)
	--CCLuaLog("contentNode: width: " .. contentNode:getContentSize().width .. "  height: " .. contentNode:getContentSize().height)
	local height = titleNode:getContentSize().height + contentNode:getContentSize().height + self.distance
	--CCLuaLog("helpListItem: width: " .. self.maxWidth .. "  height: " .. height)
	self.baseNode:setContentSize(CCSizeMake(self.maxWidth,height))
	self.baseNode:setAnchorPoint(ccp(0.5,0.5))
	titleSprite:setPosition(ccp(titleSpriteLength * 0.5 - self.maxWidth * 0.5, height - titleNode:getContentSize().height * 0.5))
	titleNode:setPosition(ccp(titleSpriteLength + titleNode:getContentSize().width * 0.5 - self.maxWidth * 0.5,height - titleNode:getContentSize().height * 0.5))
	--CCLuaLog("helpListItem: titleNode Position : " .. titleNode:getPositionX() .. " , " .. titleNode:getPositionY())
	contentNode:setPosition(ccp(titleSpriteLength + contentNode:getContentSize().width * 0.5 - self.maxWidth * 0.5,contentNode:getContentSize().height * 0.5))
	--CCLuaLog("helpListItem: contentNode Position : " .. contentNode:getPositionX() .. " , " .. contentNode:getPositionY())
end

function HelpUIItem:destroy()
	self.baseNode = nil
	self.maxWidth = 0
	self.titleColor = ccc3(190,228,155)
	self.contentColor = ccc3(255,255,255)
end