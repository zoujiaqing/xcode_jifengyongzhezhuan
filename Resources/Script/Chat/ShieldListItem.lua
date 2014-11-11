require("Script/Chat/ChatFuncTable")
require("Script/Chat/ShieldPlayerCenter")
ShieldListItem = {
	control = nil,
	node = nil,
	playerId = nil,
	playerName = "",
	priority = 0,
	tag = 0,
}

function ShieldListItem:create(extension)
	local playerinfo = setmetatable(extension or {},self)
	self.__index = self
	return playerinfo
end

function ShieldListItem:resetValue()
	self.control = nil
	self.node = nil
	self.playerId = nil
	self.priority = 0
	self.playerName = ""
	self.tag = 0
end

function ShieldListItem:destroy()
	if self.tag ~= 0 then
		removeTagFunc(self.tag)
	end
	self:resetValue()
end

local function onDeleteBtClicked(tag)
	CCLuaLog("onDeleteBtClicked " .. tag)
	local obj = getTagFunc(tag)
	if obj ~= nil and obj.playerId ~= nil then
		sendCSDelDndReq(obj.playerId)
	end
end

function ShieldListItem:init(id,name,priority,parentList,tag)
	self.playerId = id
	self.priority = priority
	self.playerName = name
	self.tag = tag
	if parentList ~= nil then
		self.node = CCNode:create()
		local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_CHATSHIELDLIST_FRAME_PLAYER1")
		local backFrame,framePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_CHATSHIELDLIST_BUTTON_PLAYER1")
		local labelFrame,labelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_CHATSHIELDLIST_TEXT_PLAYER1")
		local btFrame,btPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_CHATSHIELDLIST_BUTTON_DELETE")
		local btClickSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_CHATSHIELDLIST_BUTTON_DELETE_CLICKED")
		self.control = parentList:AddBaseItem(backFrame:getContentSize(),self.node)

		self.node:addChild(backFrame,0,0)
		backFrame:setPosition(ccpSub(framePt,nodePt))

		local nameLabel = TXGUI.UILabelTTF:create(name,KJLinXin,28)
		nameLabel:setPosition(ccpSub(labelPt,nodePt))
		self.node:addChild(nameLabel,1,1)

		if btFrame ~= nil then
			local menuItem = CCMenuItemSprite:create(btFrame,btClickSprite)
			local deleteBt = UIMenu:create()
			deleteBt:addChild(menuItem)
			deleteBt:setInitPriority(priority)
			deleteBt:setPosition(ccpSub(btPt,nodePt))
			self.node:addChild(deleteBt,2,2)
			menuItem:registerScriptTapHandler(onDeleteBtClicked)
			menuItem:setTag(self.tag)

			if self.tag ~= 0 then
				setTagFunc(self.tag,self)
			end
		end
	end

end