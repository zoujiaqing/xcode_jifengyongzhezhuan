require("Script/GameConfig/Item")
require("Script/UILayer/EquipEnchaseLockTable")

EnchaseGemStoneTable = {
	--isLock = true;
	index = 0,
	gemId = 0,
	itemPos = 0,
	iconBtn = nil,
	lockBtn = nil,
	unlockLabel = nil,
	frameGrid = nil,
	nameLabel = nil,
	attrLabel = nil,
	lockLevelLabel = nil,
}

function EnchaseGemStoneTable:createSelectGemStoneTable(extension)
	local gemStoneBtn = setmetatable(extension or {},self);
	self.__index = self;
	return gemStoneBtn;
end

function EnchaseGemStoneTable:init(uiLayout,index)
	self.index = index
	self.iconBtn =  uiLayout:FindChildObjectByName(string.format("armoryBt%d",index)).__UIButton__
	self.unlockLabel = uiLayout:FindChildObjectByName(string.format("unLockText%d",index)).__UILabel__
	self.frameGrid = uiLayout:FindChildObjectByName(string.format("armoryGrid%d",index)).__UIPicture__
	self.nameLabel = uiLayout:FindChildObjectByName(string.format("armoryName%d",index)).__UILabel__
	self.attrLabel = uiLayout:FindChildObjectByName(string.format("armoryAttributeLabel%d",index)).__UILabel__
	self.lockBtn = uiLayout:FindChildObjectByName(string.format("armoryAttributeBt%d",index)).__UIButton__
	self.lockLevelLabel = uiLayout:FindChildObjectByName(string.format("lockLevelText%d",index)).__UILabel__
	--self.iconBtn:getMenuItemSprite():registerScriptTapHandler(self.onGemStoneIconClicked);
end

function EnchaseGemStoneTable:setGem(gemId,pos,attrKey,attrValue,minValue,maxValue)
	self.unlockLabel:setVisible(false)
	--self.isLock = false;
	self.gemId = gemId;
	self.itemPos = pos;
	local sprite = ItemManager:Get():getIconSpriteById(gemId)
	if sprite ~= nil then
		self.iconBtn:setIconSprite(sprite)
	end
	local attrText = ItemManager:Get():getAttributionText(attrKey,attrValue)
	if attrText ~= nil then	
		self.nameLabel:setString(attrText)
	end

	if Items[gemId] ~= nil and Items[gemId]["Quality"] ~= nil then
		local color = ItemManager:Get():getLabelColorByQuality(Items[gemId]["Quality"])
		self.nameLabel:setColor(color)
	end
	local maxText = string.format("(最大值%d)",maxValue)
	self.attrLabel:setString(maxText)
	local frameSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_ICON_ARMORY_GRID")
	self.frameGrid:setSprite(frameSprite,false)
	self.lockLevelLabel:setVisible(false)
end

function EnchaseGemStoneTable:unselectItem(hasHole)
	self.unlockLabel:setVisible(not hasHole)
	
	self.itemPos = 0;
	self.gemId = 0;
	self.attrLabel:setString("")
	self.nameLabel:setString("")
	if hasHole then
		--self.isLock = false;
		local frameSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_ICON_ARMORY_GRID")
		self.frameGrid:setSprite(frameSprite,false)
		self.lockLevelLabel:setVisible(false)
		self.iconBtn:setIconSprite(nil)
	else
		--self.isLock = true;
		local lockSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_ICON_LOCK")
		self.iconBtn:setIconSprite(lockSprite)
		self.lockLevelLabel:setVisible(true)
		self.frameGrid:setSprite(nil)
	end
	
end

function EnchaseGemStoneTable:changeLockState(isLock)
	--self.isLock = not self.isLock;
	if isLock then
		local frameSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_ICON_ARMORY_LOCK")
		self.frameGrid:setSprite(frameSprite,false)
	else
		local frameSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_ENCHASE_ICON_ARMORY_GRID")
		self.frameGrid:setSprite(frameSprite,false)
	end
end