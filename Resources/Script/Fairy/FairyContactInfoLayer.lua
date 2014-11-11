require("Script/GameConfig/uiTagDefine")
require("Script/Fairy/FairyContactListItem")

FairyContactInfoLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	notificationFunc = 0,
	contactTable = {},
}
-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	--RemoveOneLayer(FairyContactInfoLayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("FairyContactLayout")
	--if FairyContactInfoLayer.notificationFunc ~= 0 then
	--	NotificationCenter:defaultCenter():unregisterScriptObserver(FairyContactInfoLayer.notificationFunc)
	--end
	FairyContactInfoLayer:resetValue()
end

function FairyContactInfoLayer:createLayer(playerId)
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,180))
		--self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyContactInfoLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,30,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyContactUI.plist",self.uiLayerInstance, "FairyContactLayout", true)
			self:InitLayer(playerId)
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyContactInfoLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.contactTable = {}
end

local function onCloseButtonClicked(tag)
	if FairyContactInfoLayer.uiLayerInstance ~= nil then
		FairyContactInfoLayer.uiLayerInstance:removeFromParentAndCleanup(true)
		GameAudioManager:sharedManager():playEffect(350001,false)
	end
end

function FairyContactInfoLayer:AddFairyContactInfo(uiList,pos,playerId)
	local fairyInfo = nil 
	if playerId == nil then
		fairyInfo = getUserFairyByPos(pos)
	else
		fairyList = getOtherPlayerFairyList(playerId)
		if fairyList ~= nil then
			fairyInfo = fairyList[pos]
		end
	end
	if fairyInfo ~= nil then
		local contactList = FairyContactListItem:create(uiList,fairyInfo)
		table.insert(self.contactTable,contactList)
	end
end

function FairyContactInfoLayer:InitLayer(playerId)

	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	--self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	local uiList = self.uiLayout:FindChildObjectByName("contentList").__UIScrollList__
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_BTL,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_BTL + 1,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_AST,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_AST + 1,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_PRT,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_PRT + 1,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_PRT + 2,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_PRT + 3,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_PRT + 4,playerId)
	self:AddFairyContactInfo(uiList,FAIRY_POS_START_PRT + 5,playerId)
	--PushOneLayer(self.uiLayerInstance,"","")
end