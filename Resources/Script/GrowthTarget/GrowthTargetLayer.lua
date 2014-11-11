require("Script/GameConfig/uiTagDefine")
require("Script/GrowthTarget/GrowthTargetListItem")
require("Script/GrowthTarget/GrowthTargetMessage")

GrowthTargetLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	listItemTable = {},
	notificationFunc = 0,
	tipFairyId = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RemoveOneLayer(GrowthTargetLayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("GrowthTargetLayout")
	if GrowthTargetLayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(GrowthTargetLayer.notificationFunc)
	end
	GrowthTargetLayer:resetValue()
end

function GrowthTargetLayer:createLayer()
	if self.uiLayerInstance == nil then 
		--self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["GrowthTargetLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/GrowthTargetUI.plist",self.uiLayerInstance, "GrowthTargetLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function GrowthTargetLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.listItemTable = {}
	self.tipFairyId = 0
end

function GrowthTargetLayer:onReceiveTargetList()
	if self.uiLayerInstance ~= nil then
		-- to do
		self:InitTargetList()
	end
end

function GrowthTargetLayer:onReceiveTargetListUpdate(oldTable,nextTable)
	if self.uiLayerInstance ~= nil then
		for k,v in pairs(nextTable) do
			local updateNewTableFlag = false
			if Target[v] ~= nil and Target[v]["Previous_Target"] ~= nil then
				local preTarget = tonumber(Target[v]["Previous_Target"])
				for _index,item in pairs(self.listItemTable) do
					if item.targetID == preTarget then
						item:resetTargetID(v)
						self.listItemTable[v] = item
						self.listItemTable[preTarget] = nil
						updateNewTableFlag = true
						break
					end
				end
			end
			if not updateNewTableFlag then
				for _index,item in pairs(self.listItemTable) do
					if item.targetID == v then
						item:resetStatus()
						break
					end
				end
			end
		end
		
		local uiList = self.uiLayout:FindChildObjectByName("contentList").__UIScrollList__
		for k,v in pairs(oldTable) do
			for _index,item in pairs(self.listItemTable) do
				if item.targetID == v then
					item:setGetRewardAlready()
					break
				end
			end
		end
	end
end

local function onCloseButtonClicked(tag)
	if GrowthTargetLayer.uiLayerInstance ~= nil then
		GrowthTargetLayer.uiLayerInstance:removeFromParentAndCleanup(true)
		SetChatBar(true,-1)
		GameAudioManager:sharedManager():playEffect(350001,false)
	end
end

local function onClickGetRewardButton(tag)
	--CCLuaLog("onClickGetRewardButton: " .. tag)
	if GrowthTargetDataList[tag] ~= nil then
		--CCLuaLog("GrowthTargetDataList[tag].status: " .. GrowthTargetDataList[tag].status)
		if GrowthTargetDataList[tag].status == 3 then
			sendGetTargetRewardListReq(tag)
		elseif GrowthTargetDataList[tag].status == 2 then
			if Target[tag] ~= nil and Target[tag]["Link_UI"] ~= nil then
			    onCloseButtonClicked(nil)
				showUILayerByName(Target[tag]["Link_UI"],true)
			end
		end
	end
end

local function onClickFirstReward(tag)
	GrowthTargetLayer:showRewardTip(tag,1)
end

local function onClickSecondReward(tag)
	GrowthTargetLayer:showRewardTip(tag,2)
end

local function onClickThirdReward(tag)
	GrowthTargetLayer:showRewardTip(tag,3)
end

local function onClickForthReward(tag)
	GrowthTargetLayer:showRewardTip(tag,4)
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= GrowthTargetLayer.uiLayerInstance then
			if G_CurLayerInstance ~= GrowthTargetLayer.uiLayerInstance then
				if GrowthTargetLayer.uiLayerInstance ~= nil then
					GrowthTargetLayer.uiLayerInstance:removeFromParentAndCleanup(true)
				end
				ret = 1
			end
		end
	end

	return ret
end

function GrowthTargetLayer:showRewardTip(targetID,rewardIndex)
	if self.listItemTable[targetID] ~= nil then
		self.listItemTable[targetID]:showRewardTip(rewardIndex)
	end
end

function GrowthTargetLayer:InitTargetList()
	
	local targetIdTable = {}
	for k,v in pairs(GrowthTargetDataList) do
		if v.status ~= 4 then
			table.insert(targetIdTable,v.targetID)
		end
	end
	table.sort(targetIdTable)

	local index = 1
	local uiList = self.uiLayout:FindChildObjectByName("contentList").__UIScrollList__
	for k,v in pairs(targetIdTable) do
		if GrowthTargetDataList[v] ~= nil then
			local item = GrowthTargetListItem:create(uiList,v,index)
			item:onGetRewardButtonClicked(onClickGetRewardButton)
			item:onFirstRewardButtonClicked(onClickFirstReward)
			item:onSecondRewardButtonClicked(onClickSecondReward)
			item:onThirdRewardButtonClicked(onClickThirdReward)
			item:onForthRewardButtonClicked(onClickForthReward)
			index = index + 1
			self.listItemTable[v] = item
		end
	end
end

function GrowthTargetLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)

	sendGetTargetListReq()

	
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	--SetChatBar(false,-1)
	PushOneLayer(self.uiLayerInstance,"","")
end