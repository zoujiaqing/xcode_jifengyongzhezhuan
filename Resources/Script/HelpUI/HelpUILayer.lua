require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/help")
require("Script/HelpUI/HelpManager")
require("Script/HelpUI/HelpUIItem")

HelpUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	uiList = nil,
	helpIndex = 0,
	helpDataTable = nil,
	nodeList = nil,
	notificationFunc = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RemoveOneLayer(HelpUILayer.uiLayerInstance)
	if HelpUILayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(HelpUILayer.notificationFunc);
	end
	TXGUI.UIManager:sharedManager():removeUILayout("HelpLayout")
	HelpUILayer:resetValue()
end

function HelpUILayer:createLayer()
	local index = getRecentHelpIndex()
	CCLuaLog("HelpUILayer:createLayer():" .. index)
	if self.uiLayerInstance == nil and index ~= 0 then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["HelpUILayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/helpUI.plist",self.uiLayerInstance, "HelpLayout", true)
			self.helpIndex = index
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function HelpUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.helpIndex = 0
	self.helpDataTable = nil
	self.nodeList = nil
	self.uiList= nil
	self.notificationFunc = nil
end

function HelpUILayer:InitHelpDataByIndex()
	--CCLuaLog(" HelpUILayer:InitHelpDataByIndex()")
	self.helpDataTable = {}
	for k,v in pairs(help) do
		local info = v
		local num = tonumber(info.system_id)
		info.system_id = num
		if info.system_id == self.helpIndex then
			CCLuaLog("helpData; " .. info.system_id)
			table.insert(self.helpDataTable,info)
		else
			CCLuaLog("helpData not equal: " .. info.system_id)
			CCLuaLog("self.helpIndex: " .. self.helpIndex)
		end
	end
end

local function helpDataSort(a,b)
	return a.title_id < b.title_id
end

local function onCloseButtonClicked()
	if HelpUILayer.uiLayerInstance ~= nil then
		HelpUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

-- 删除UI
function HelpUILayer:destroyed()
	if HelpUILayer.uiLayerInstance ~= nil then
		HelpUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= HelpUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= HelpUILayer.uiLayerInstance then
				HelpUILayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function HelpUILayer:InitUIList()
	--CCLuaLog(" HelpUILayer:InitUIList()")
	if self.helpDataTable ~= nil and self.uiList ~= nil then	
		self.nodeList = {}
		table.sort(self.helpDataTable,helpDataSort)
		local listWidth = self.uiList:getContentSize().width
		local scale  = TXGUI.UIManager:sharedManager():getScaleFactor()
		local width = listWidth / scale
		for k,v in pairs(self.helpDataTable) do
			local titleIndex = v.title_text
			local contentIndex = v.answer_text
			if titleIndex ~= nil and contentIndex ~= nil then
				local title = LanguageLocalization:GetLocalization(titleIndex)
				local content = LanguageLocalization:GetLocalization(contentIndex)
				local item = HelpUIItem:create(width,title,content)
				if item ~= nil then
					table.insert(self.nodeList,item)
				end
			end
		end

		for k,v in pairs(self.nodeList) do 
			local node = v.baseNode
			local base = self.uiList:AddBaseItem(node:getContentSize(),node)
			local oldPos_y = node:getPositionY()
			node:setPosition((0 + self.uiList:getContentSize().width ) * 0.5,oldPos_y)
		end
	end
end

function HelpUILayer:InitLayer()
	--CCLuaLog("HelpUILayer:InitLayer()")
	self.uiList = self.uiLayout:FindChildObjectByName("contentList").__UIScrollList__
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
	self:InitHelpDataByIndex()
	self:InitUIList()
	PushOneLayer(self.uiLayerInstance,"","")
end