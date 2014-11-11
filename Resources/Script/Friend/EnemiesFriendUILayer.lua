require("Script/GameConfig/uiTagDefine")
require("Script/Friend/SelectedFriendContainer")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Friend/FriendListItem")
require("Script/Friend/PlayerItemCenter")
require("Script/Friend/addFriend")
require("Script/Chat/ChatPlayerOption")

EnemiesFriendUILayer = {
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
	if EnemiesFriendUILayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(EnemiesFriendUILayer.notificationFunc);
	end

	EnemiesFriendUILayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("enemiesFriendUI")

	resetFriendCallBacks()
end

function EnemiesFriendUILayer:CreateLayer()
	CCLuaLog("EnemiesFriendUILayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["EnemiesFriendUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/enemiesFriend.plist",self.uiLayerInstance, "enemiesFriendUI", true)
			self:InitLayer();
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function EnemiesFriendUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiList = nil
	self.nearList = nil
	self.notificationFunc = nil
	self.detailInfoContainer = nil
	self.selectedListItem = nil
end

-- 删除UI
function EnemiesFriendUILayer:destroyed()
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

local function onReceivedEnemiesListFresh()
	print("-------------------------- enemies call back")
	EnemiesFriendUILayer:resetList(false)
end

-- 关闭按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(EnemiesFriendUILayer.uiLayerInstance)
	EnemiesFriendUILayer:destroyed()
	SetChatBar(true,-1)
end

-- 好友界面
local function onFriendBtClicked(tag)
	showUILayerByTag(UITagTable["FriendUILayer"].tag,true)
end
-- 附近好友
local function onNearFriendBtClicked(pObj)
	showUILayerByTag(UITagTable["NearByFriendUILayer"].tag,true)
end
-- 红名
local function onRedFriendBtClicked(tag)
	showUILayerByTag(UITagTable["RedFriendUILayer"].tag,true)
	--print("Red name")
end

-- 刷新按钮
local function onUpdateBtClicked(tag)
	sendCSEnemyInfoReq()
end

local function onMoreBtClicked(tag)
	if (EnemiesFriendUILayer.nearList ~= nil) and (EnemiesFriendUILayer.nearList[tag] ~= nil) then
		local id = EnemiesFriendUILayer.nearList[tag]:getID()
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			setPlayerId(playerInfo:getID():getID())
			setRegtime(playerInfo:getID():getRegTime())
			setChannel(playerInfo:getID():getChannel())
			setPlayerName(playerInfo:getName())
			setPlayerLevel(playerInfo:getLevel())
			setPlayerStrength(playerInfo:getStrength())
			setInitPlayer(1)
			ChatPlayerOption:EnableModeless(true)
		end
	end	
end

-- list中button回调
local function onClickedFriendListItem(tag)
	if (EnemiesFriendUILayer.nearList ~= nil) and (EnemiesFriendUILayer.nearList[tag] ~= nil) then
		local id = EnemiesFriendUILayer.nearList[tag]:getID()
		EnemiesFriendUILayer:setSelectedItem(EnemiesFriendUILayer.nearList[tag])

		if EnemiesFriendUILayer.detailInfoContainer ~= nil then
			--local friendTable = getNearPlayerInfo(false)
			local playerInfo = getSignleFriendInfo(id)
			if playerInfo ~= nil then
				EnemiesFriendUILayer.detailInfoContainer:setSelectFriend(playerInfo)
			end
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= EnemiesFriendUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= EnemiesFriendUILayer.uiLayerInstance then
				EnemiesFriendUILayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function EnemiesFriendUILayer:setSelectedItem(item)
	if self.selectedListItem ~= nil then
		self.selectedListItem:setButtonUnselected()
	end
	if item ~= nil then
		item:setButtonSelected()
	end

	self.selectedListItem = item
end

function EnemiesFriendUILayer:resetList(refresh)
	if self.nearList ~= nil then
		for k,v in pairs(self.nearList) do
			self.nearList[k]:destroy()
			self.nearList[k] = nil
		end

		local tag = 0
		if self.uiList ~= nil then
			self.uiList:clearChildrens()
			self.selectedListItem = nil
			local friendTable = getEnemiesPlayerList(refresh)
			for k,v in pairs(friendTable) do 
				print("-------------------------- enemies friend tag " .. tag)
				local friendInfo = getSignleFriendInfo(v)
				if friendInfo ~= nil then
					tag = tag + 1
					local friendItem = FriendListItem:createFriendListItem()
					friendItem:init(self.uiList,friendInfo,tag)
					friendItem:SetOnlineText(friendInfo.onlineFlag)
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

local function onReceiveCSDelEnemyRsp(msg)
	if msg ~= nil then
		if msg.succ == 1 then
			sendCSEnemyInfoReq()
		end
	end
end

function EnemiesFriendUILayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseBtClicked);

	--Note: 其他标签页回调接口
	local friendListBt = self.uiLayout:FindChildObjectByName("friendListBtn").__UIButton__:getMenuItemSprite();
	friendListBt:registerScriptTapHandler(onFriendBtClicked);
	local nearFriendListBt = self.uiLayout:FindChildObjectByName("nearByFriendBtn").__UIButton__:getMenuItemSprite();
	nearFriendListBt:registerScriptTapHandler(onNearFriendBtClicked);
	local redFriendListBt = self.uiLayout:FindChildObjectByName("redFriendBtn").__UIButton__:getMenuItemSprite();
	redFriendListBt:registerScriptTapHandler(onRedFriendBtClicked);	

	local updateListBt = self.uiLayout:FindChildObjectByName("friendUpdateBt").__UIButton__
	if nil ~= updateListBt then
		--updateListBt:setVisible(false)
		local btnMenuItem = updateListBt:getMenuItemSprite();
		btnMenuItem:registerScriptTapHandler(onUpdateBtClicked);
	end
	local updateListLabel = self.uiLayout:FindChildObjectByName("friendUpdateLabel").__UILabel__
	if nil ~= updateListLabel then
		--updateListLabel:setVisible(false)
	end

	self.detailInfoContainer = SelectedFriendContainer:create()
	self.detailInfoContainer:init(self.uiLayout)

	self.uiList = self.uiLayout:FindChildObjectByName("friendList").__UIScrollList__;
	self.nearList = {}

	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

	setEnemiesFriendListCallback(onReceivedEnemiesListFresh)
	sendCSEnemyInfoReq()

	setDelEnemyCallBack(onReceiveCSDelEnemyRsp)

	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUEL_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUELED_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_RED)
end