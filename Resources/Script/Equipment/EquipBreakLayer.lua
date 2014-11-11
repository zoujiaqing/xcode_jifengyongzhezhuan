require("Script/GameConfig/uiTagDefine")
require("Script/Equipment/EquipConfigTable")
require("Script/Equipment/EquipMessage")

EquipBreakLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	equipPos = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("equipBreakLayout")
	EquipBreakLayer:resetValue()
end

function setBreakedEquipPos(pos)
	EquipBreakLayer.equipPos = pos
end

function EquipBreakLayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["EquipBreakLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/equipBreakUI.plist",self.uiLayerInstance, "equipBreakLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function EquipBreakLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.equipPos = 0
end

function EquipBreakLayer:onReceiveDecomposeResult()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end 
end

local function onConfirmBtClicked()
	sendEquipmentDecomposeReq(EquipBreakLayer.equipPos)
end

local function onCancellBtClicked()
	if EquipBreakLayer.uiLayerInstance ~= nil then
		EquipBreakLayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

function EquipBreakLayer:InitLayer()
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	confirmBt:registerScriptTapHandler(onConfirmBtClicked)
	local cancellBt = self.uiLayout:FindChildObjectByName("cancellBt").__UIButton__:getMenuItemSprite()
	cancellBt:registerScriptTapHandler(onCancellBtClicked)
	self:showEquipCost()
end

function EquipBreakLayer:showEquipName(name)
	local requestLabel = self.uiLayout:FindChildObjectByName("requestLabel").__UIPicture__
	local nodeFrame,nodePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_DECOMPOSITION_TEXT2")
	local width = nodeFrame:getContentSize().width
	local node = HtmlNode:create(width)
	node:begin()
	node:addLabel(LanguageLocalization:GetLocalization("M_DECOMPOSITION_TEXT1"),24,KJLinXin,ccc3(255,255,255))
	node:addLabel(name,24,KJLinXin,ccc3(0,255,0))
	node:addLabel("?",24,KJLinXin,ccc3(255,255,255))
	node:finish()
	node:setAnchorPoint(ccp(0.5,0.5))
	node:setPosition(ccp(node:getContentSize().width,node:getContentSize().height * 0.5))
	requestLabel:getCurrentNode():addChild(node)
end

function EquipBreakLayer:showEquipCost()
	local goldLabel = self.uiLayout:FindChildObjectByName("coinLabel").__UILabel__
	local fragLabel = self.uiLayout:FindChildObjectByName("fragLabel").__UILabel__
	local fragPic = self.uiLayout:FindChildObjectByName("fragPic").__UIPicture__
	if self.equipPos ~= 0 then
		local item = ItemManager:Get():findItemByPos(self.equipPos)
		if item ~= nil then	
			local info = item:getItemInfo()
			if info ~= nil then
				local equipId = item:getItemId()
				local equipLevel = item.itemLevel
				local equipQuility = info.m_quality
				local name = info.m_name
				self:showEquipName(name)
				local costGold , costFrag = getEquipBreakMoney(equipId,equipLevel,getEquipTypeById(equipId))
				goldLabel:setString(costGold)
				if costFrag == 0 then
					fragLabel:setVisible(false)
					fragPic:setVisible(false)
				else
					fragLabel:setVisible(true)
					fragPic:setVisible(true)
					fragLabel:setString(costFrag)
				end
			end
		end
	end
end