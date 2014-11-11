require("Script/GameConfig/uiTagDefine")
require("Script/Friend/SelectedFriendContainer")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Friend/FriendListItem")
require("Script/Friend/PlayerItemCenter")
require("Script/Friend/addFriend")
require("Script/Chat/ChatPlayerOption")

NearByFriendUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	nearList = {},
	uiList = nil,
	notificationFunc = nil,
	selectedListItem = nil,
	detailInfoContainer = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if NearByFriendUILayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(NearByFriendUILayer.notificationFunc);
	end

	NearByFriendUILayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("nearByFriendUI")

	resetFriendCallBacks()
end

function NearByFriendUILayer:CreateLayer()
	CCLuaLog("NearByFriendUILayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["NearByFriendUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/nearByFriendUI.plist",self.uiLayerInstance, "nearByFriendUI", true)
			self:InitLayer();
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function NearByFriendUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiList = nil
	self.nearList = nil
	self.notificationFunc = nil
	self.detailInfoContainer = nil
	self.selectedListItem = nil
end

-- 删除UI
function NearByFriendUILayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.detailInfoContainer ~= nil then
		self.detailInfoContainer:resetValue()
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
end

local function onReceivedNearListFresh()
	NearByFriendUILayer:resetList(false)
end

-- 关闭按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(NearByFriendUILayer.uiLayerInstance)
	NearByFriendUILayer:destroyed()
	SetChatBar(true,-1)
end

-- 好友界面
local function onFriendBtClicked(tag)
	showUILayerByTag(UITagTable["FriendUILayer"].tag,true)
end
-- 红名
local function onRedFriendBtClicked(tag)
	showUILayerByTag(UITagTable["RedFriendUILayer"].tag,true)
end
-- 宿敌
local function onEnemiesBtClicked(tag)
	showUILayerByTag(UITagTable["EnemiesFriendUILayer"].tag,true)
	--print("Enemies")
end

-- 刷新按钮
local function onUpdateBtClicked(tag)
	sendCSNearByReq()
end

-- 添加好友
local function onAddFriendBtClicked(tag)
	if NearByFriendUILayer.selectedListItem ~= nil then
		local id = NearByFriendUILayer.selectedListItem.id
		local friendInfo = getSignleFriendInfo(id)
		if friendInfo ~= nil then
			sendAddFriendReq(friendInfo:getName())
		end
	end
end

-- 显示玩家详细信息
local function onClickedDetailBt()
	if NearByFriendUILayer.selectedListItem ~= nil then
		local id = NearByFriendUILayer.selectedListItem:getID()
		PlayerItemCenter:setSelectedPlayer(id)
		showUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,true)
	end
end

local function onChatBtClicked(tag)
	if (NearByFriendUILayer.nearList ~= nil) and (NearByFriendUILayer.nearList[tag] ~= nil) then
		local id = NearByFriendUILayer.nearList[tag]:getID()
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			reqToPrivateChat(id,playerInfo:getName())
		end
	end
end

local function onMoreBtClicked(tag)
	if (NearByFriendUILayer.nearList ~= nil) and (NearByFriendUILayer.nearList[tag] ~= nil) then
		local id = NearByFriendUILayer.nearList[tag]:getID()
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			setPlayerId(playerInfo:getID():getID())
			setRegtime(playerInfo:getID():getRegTime())
			setChannel(playerInfo:getID():getChannel())
			setPlayerName(playerInfo:getName())
			setPlayerLevel(playerInfo:getLevel())
			setPlayerStrength(playerInfo:getStrength())
			setInitPlayer()
			ChatPlayerOption:EnableModeless(true)
		end
	end	
end

-- list中button回调
local function onClickedFriendListItem(tag)
	if (NearByFriendUILayer.nearList ~= nil) and (NearByFriendUILayer.nearList[tag] ~= nil) then
		local id = NearByFriendUILayer.nearList[tag]:getID()
		NearByFriendUILayer:setSelectedItem(NearByFriendUILayer.nearList[tag])

		if NearByFriendUILayer.detailInfoContainer ~= nil then
			--local friendTable = getNearPlayerInfo(false)
			local playerInfo = getSignleFriendInfo(id)
			if playerInfo ~= nil then
				NearByFriendUILayer.detailInfoContainer:setSelectFriend(playerInfo)
			end
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= NearByFriendUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= NearByFriendUILayer.uiLayerInstance then
				NearByFriendUILayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function NearByFriendUILayer:setSelectedItem(item)
	if self.selectedListItem ~= nil then
		self.selectedListItem:setButtonUnselected()
	end
	if item ~= nil then
		item:setButtonSelected()
	end

	self.selectedListItem = item
end

function NearByFriendUILayer:resetList(refresh)
	if self.nearList ~= nil then
		for k,v in pairs(self.nearList) do
			self.nearList[k]:destroy()
			self.nearList[k] = nil
		end

		local tag = 0
		if self.uiList ~= nil then
			self.uiList:clearChildrens()
			self.selectedListItem = nil
			--local friendTable = getNearPlayerInfo(refresh)
			local friendTable = getNearPlayerList(refresh)
			for k,v in pairs(friendTable) do 
				local friendInfo = getSignleFriendInfo(v)
				if friendInfo ~= nil then
					tag = tag + 1
					local friendItem = FriendListItem:createFriendListItem()
					friendItem:init(self.uiList,friendInfo,tag)
					--friendItem:SetOnlineText(friendInfo:getOnLineFlag())
					if friendItem.rootNode ~= nil then
						friendItem:setButtonCallBack(onClickedFriendListItem)
					end
					friendItem:setChatBtCallBack(onChatBtClicked)
					friendItem:setMoreBtCallBack(onMoreBtClicked)
					self.nearList[tag] = friendItem
					-- 设置默认选项
					if self.selectedListItem == nil then
						self:setSelectedItem(friendItem)
						if self.detailInfoContainer ~= nil then
							self.detailInfoContainer:setSelectFriend(friendInfo)
						end
					end
				end
			end
		end
	end
	if nil ~= self.detailInfoContainer then
		self.detailInfoContainer:showPlayerInfo();
	end	
end

function NearByFriendUILayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseBtClicked);
	--local friendListBt = self.uiLayout:FindChildObjectByName("friendListBt").__UIButton__:getMenuItemSprite();
	--friendListBt:registerScriptTapHandler(onFriendListBtClicked);
	local updateListBt = self.uiLayout:FindChildObjectByName("friendUpdateBt").__UIButton__:getMenuItemSprite();
	updateListBt:registerScriptTapHandler(onUpdateBtClicked);
	--local addFriendBt = self.uiLayout:FindChildObjectByName("addFriendBt").__UIButton__:getMenuItemSprite();
	--addFriendBt:registerScriptTapHandler(onAddFriendBtClicked);

	--local detailSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL")
	--local detailClickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL_CLICKED")
	--local detailBt = self.uiLayout:FindChildObjectByName("detailBt").__UIButton__:getMenuItemSprite();
	--detailBt:setNormalImage(detailSprite)
	--detailBt:setSelectedImage(detailClickedSprite)
	--detailBt:registerScriptTapHandler(onClickedDetailBt)

	--Note: 其他标签页回调接口
	local friendListBt = self.uiLayout:FindChildObjectByName("friendListBtn").__UIButton__:getMenuItemSprite();
	friendListBt:registerScriptTapHandler(onFriendBtClicked);
	local redFriendListBt = self.uiLayout:FindChildObjectByName("redFriendBtn").__UIButton__:getMenuItemSprite();
	redFriendListBt:registerScriptTapHandler(onRedFriendBtClicked);
	local enemiesListBt = self.uiLayout:FindChildObjectByName("enemiesBtn").__UIButton__:getMenuItemSprite();
	enemiesListBt:registerScriptTapHandler(onEnemiesBtClicked);

	self.detailInfoContainer = SelectedFriendContainer:create()
	self.detailInfoContainer:init(self.uiLayout)

	self.uiList = self.uiLayout:FindChildObjectByName("friendList").__UIScrollList__;
	self.nearList = {}

	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

	setNearPlayerListCallback(onReceivedNearListFresh)
	sendCSNearByReq()

	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUEL_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUELED_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_RED)
end