require("Script/Chat/ShieldPlayerCenter")
require("Script/Chat/ShieldListItem")

ShieldLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	uiList = nil,
	itemsList = {},
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	ShieldLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("shieldLayout")
end

function ShieldLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ShieldLayer"].tag
			parentNode:addChild(self.uiLayerInstance,82,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/shieldLayer.plist",self.uiLayerInstance, "shieldLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function ShieldLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiList = nil
	self.itemsList = {}
end

-- 删除UI
function ShieldLayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.itemsList ~= nil then
		for k,v in pairs(self.itemsList) do
			v:destroy()
			self.itemsList[k] = nil
		end
	end
	self:resetValue()
end

function ShieldLayer:resetList(refresh)
	if self.uiList ~= nil then
		
		if self.itemsList ~= nil then
			for k,v in pairs(self.itemsList) do
				v:destroy()
				self.itemsList[k] = nil
			end
			
		end

		local tag = 0
		self.uiList:clearChildrens()
		local shieldTable = getShieldPlayerList(refresh)
		for k,v in pairs(shieldTable) do 
			tag = tag + 1
			local shieldItem = ShieldListItem:create()
			shieldItem:init(v.playerId,v.name,-139,self.uiList,tag)
		end
	end
end

-- 关闭按钮回调
local function onCloseBtClicked(tag)
	ShieldLayer:destroyed()
end

local function onShieldListFresh()
	ShieldLayer:resetList(false)
end

function ShieldLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseBtClicked)
	self.uiList = self.uiLayout:FindChildObjectByName("shieldList").__UIScrollList__
	setShieldListCallBack(onShieldListFresh)
	sendCSDndInfoReq()
end