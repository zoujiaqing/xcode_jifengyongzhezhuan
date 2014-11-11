require("Script/GameConfig/uiTagDefine")
require("Script/Friend/SelectedFriendContainer")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Friend/FriendListItem")
require("Script/Friend/PlayerItemCenter")

FriendUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	notificationFunc = nil,
	friendList = {},
	uiList = nil,
	selectedListItem = nil,
	detailInfoContainer = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if FriendUILayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(FriendUILayer.notificationFunc);
	end
	local addFriendUI = getUILayerByTag(UITagTable["AddFriendUILayer"].tag,false)
	if addFriendUI ~= nil then
		addFriendUI:removeFromParentAndCleanup(true)
	end

	--local equipUI = getUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,false)
	--if equipUI ~= nil then
	--	equipUI:removeFromParentAndCleanup(true)
	--end

	FriendUILayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("friendLayout")

	resetFriendCallBacks()
end

function FriendUILayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FriendUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/friendUI.plist",self.uiLayerInstance, "friendLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FriendUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
	self.friendList = nil
	self.uiList = nil
	self.selectedListItem = nil
	self.detailInfoContainer = nil
end

-- 删除UI
function FriendUILayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	--if self.detailInfoContainer ~= nil then
		--self.detailInfoContainer:resetValue()
	--end
--
	--if self.notificationFunc ~= nil then
		--NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	--end
	--local addFriendUI = getUILayerByTag(UITagTable["AddFriendUILayer"].tag,false)
	--if addFriendUI ~= nil then
		--addFriendUI:removeFromParentAndCleanup(true)
	--end
--
	--local equipUI = getUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,false)
	--if equipUI ~= nil then
		--equipUI:removeFromParentAndCleanup(true)
	--end
--
	--self:resetValue()
end

function FriendUILayer:setSelectedItem(item)
	if self.selectedListItem ~= nil then
		self.selectedListItem:setButtonUnselected()
	end
	if item ~= nil then
		item:setButtonSelected()
	end

	self.selectedListItem = item
end

-- 关闭按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(FriendUILayer.uiLayerInstance)
	FriendUILayer:destroyed()
	SetChatBar(true,-1)
end

-- 附近玩家按钮回调
local function onNearFriendBtClicked(pObj)
	showUILayerByTag(UITagTable["NearByFriendUILayer"].tag,true)
end
-- 红名
local function onRedFriendBtClicked(tag)
	showUILayerByTag(UITagTable["RedFriendUILayer"].tag,true)
	--print("Red name")
end
-- 宿敌
local function onEnemiesBtClicked(tag)
	showUILayerByTag(UITagTable["EnemiesFriendUILayer"].tag,true)
end

-- 刷新按钮
local function onUpdateBtClicked(tag)
	sendCSPartnerInfoReq()
end

-- 红名玩家按钮回调
local function onRedFriendButtonClicked(pObj)
	showUILayerByTag(UITagTable["RedFriendUILayer"].tag,true)
end

-- 添加好友按钮回调
local function onAddFriendButtonClicked(pObj)
	showUILayerByTag(UITagTable["AddFriendUILayer"].tag,true)
end

-- 发送删除好友消息
local function sendDelPartnerReq(id)
	local ret = false
	if id ~= nil then
		CSDelPartnerReq = {
			userid	= id:getID(),
			reg_tm	= id:getRegTime(),
			channel_id = id:getChannel(),
		}
		local msgname="CSDelPartnerReq"
		ret = send_message(msgname, CSDelPartnerReq, true)
	end
	return ret;
end

-- 删除好友回包
local function receiveDeleteFriendRsp(msg)
	if msg.succ then
		GameApi:showMessage("删除成功");
		sendCSPartnerInfoReq()
	else
		GameApi:showMessage("删除失败");
	end
	return true
end

-- 显示玩家详细信息
local function onClickedDetailBt()
	if FriendUILayer.selectedListItem ~= nil then
		local id = FriendUILayer.selectedListItem:getID()
		PlayerItemCenter:setSelectedPlayer(id)
		showUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,true)
	end
end

-- 确认删除好友
local function onConfirmDeleteFriend(obj)
	if FriendUILayer.selectedListItem ~= nil then
		--local friendTable = getFriendInfo(false)
		local id = FriendUILayer.selectedListItem.id
		--if friendTable[id] ~= nil then
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			sendDelPartnerReq(playerInfo:getID())
		end
	end
end

-- 删除好友按钮回调
local function onDeleteFriendButtonClicked(tag)
	if FriendUILayer.selectedListItem ~= nil then
		local nameText = Localization['M_FRIEND_DELETE1']["SimpleChinese"];
		MessageBox:Show(nameText,onConfirmDeleteFriend,nil,MB_YESNO,ccc3(255,255,255),26)
		
	end
end

-- 与好友对战
local function onFightFriendBtClicked()
	if FriendUILayer.selectedListItem ~= nil then
		local id = FriendUILayer.selectedListItem.id
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			sendPvaiReq(playerInfo:getID():getID(),playerInfo:getID():getRegTime(),playerInfo:getID():getChannel())
			sendFriendSkill(playerInfo:getID():getID(),playerInfo:getID():getRegTime(),playerInfo:getID():getChannel())
			PushOneWaitingLayer("CSPVAIReq")
		end
	end
end

local function onReceivedFriendListFresh()
	CCLuaLog("onReceivedFriendListFresh")
	FriendUILayer:resetList(false)
end

local function onChatBtClicked(tag)
	if (FriendUILayer.friendList ~= nil) and (FriendUILayer.friendList[tag] ~= nil) then
		local id = FriendUILayer.friendList[tag]:getID()
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			reqToPrivateChat(id,playerInfo:getName())
		end
	end
end

local function onMoreBtClicked(tag)
	if (FriendUILayer.friendList ~= nil) and (FriendUILayer.friendList[tag] ~= nil) then
		local id = FriendUILayer.friendList[tag]:getID()
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

local function onClickedFriendListItem(tag)
	if (FriendUILayer.friendList ~= nil) and (FriendUILayer.friendList[tag] ~= nil) then
		local id = FriendUILayer.friendList[tag]:getID()
		FriendUILayer:setSelectedItem(FriendUILayer.friendList[tag])

		if FriendUILayer.detailInfoContainer ~= nil then
			--local friendTable = getFriendInfo(false)
			local playerInfo = getSignleFriendInfo(id)
			if playerInfo ~= nil then
				FriendUILayer.detailInfoContainer:setSelectFriend(playerInfo)
			end
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= FriendUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= FriendUILayer.uiLayerInstance then
				FriendUILayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function FriendUILayer:resetList(refresh)
	if self.detailInfoContainer ~= nil then
		self.detailInfoContainer:unselectedFriend()
	end

	if self.friendList ~= nil then
		for k,v in pairs(self.friendList) do
			self.friendList[k]:destroy()
			self.friendList[k] = nil
		end

		local tag = 0
		if self.uiList ~= nil then
			self.uiList:clearChildrens()
			self.selectedListItem = nil
			--local friendTable = getFriendInfo(refresh)
			local friendTable = getFriendList(refresh)
			for k,v in pairs(friendTable) do 
				local friendInfo = getSignleFriendInfo(v)
				--local friendInfo = v
				if friendInfo ~= nil then
					tag = tag + 1
					local friendItem = FriendListItem:createFriendListItem()
					friendItem:init(self.uiList,friendInfo,tag)
					--friendItem:SetOnlineText(friendInfo:getOnLineFlag())
					friendItem:SetOnlineText(friendInfo.onlineFlag)
					if friendItem.rootNode ~= nil then
						friendItem:setButtonCallBack(onClickedFriendListItem)
					end
					friendItem:setChatBtCallBack(onChatBtClicked)
					friendItem:setMoreBtCallBack(onMoreBtClicked)

					self.friendList[tag] = friendItem

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

	if self.detailInfoContainer ~= nil then
		self.detailInfoContainer:showPlayerInfo()
	end
end

function FriendUILayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseBtClicked);
	--local nearFriendBt = self.uiLayout:FindChildObjectByName("friendNearbyBt").__UIButton__:getMenuItemSprite();
	--nearFriendBt:registerScriptTapHandler(onNearFriendButtonClicked);
	local addFriendBt = self.uiLayout:FindChildObjectByName("addFriendBt").__UIButton__:getMenuItemSprite();
	addFriendBt:registerScriptTapHandler(onAddFriendButtonClicked);
	local deleteFriendBt = self.uiLayout:FindChildObjectByName("deleteFriendBt").__UIButton__:getMenuItemSprite();
	deleteFriendBt:registerScriptTapHandler(onDeleteFriendButtonClicked);
	--local pvaiBt = self.uiLayout:FindChildObjectByName("fightBt").__UIButton__:getMenuItemSprite();
	--pvaiBt:registerScriptTapHandler(onFightFriendBtClicked);

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

	--Note:红名
	--local redFriendBt = self.uiLayout:FindChildObjectByName("redFriendBt").__UIButton__:getMenuItemSprite();
	--if redFriendBt ~= nil then
		--redFriendBt:registerScriptTapHandler(onRedFriendButtonClicked);
	--end

	--local detailSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL")
	--local detailClickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL_CLICKED")
	--local detailBt = self.uiLayout:FindChildObjectByName("detailBt").__UIButton__:getMenuItemSprite();
	--detailBt:setNormalImage(detailSprite)
	--detailBt:setSelectedImage(detailClickedSprite)
	--detailBt:registerScriptTapHandler(onClickedDetailBt)

	--Note: 其他标签页回调接口
	local nearFriendListBt = self.uiLayout:FindChildObjectByName("nearByFriendBtn").__UIButton__:getMenuItemSprite();
	nearFriendListBt:registerScriptTapHandler(onNearFriendBtClicked);
	local redFriendListBt = self.uiLayout:FindChildObjectByName("redFriendBtn").__UIButton__:getMenuItemSprite();
	redFriendListBt:registerScriptTapHandler(onRedFriendBtClicked);
	local enemiesListBt = self.uiLayout:FindChildObjectByName("enemiesBtn").__UIButton__:getMenuItemSprite();
	enemiesListBt:registerScriptTapHandler(onEnemiesBtClicked);

	self.detailInfoContainer = SelectedFriendContainer:create()
	self.detailInfoContainer:init(self.uiLayout)

	self.uiList = self.uiLayout:FindChildObjectByName("friendList").__UIScrollList__;
	self.friendList = {}
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
	addMsgCallBack("CSDelPartnerRsp", receiveDeleteFriendRsp)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

	setFriendListCallback(onReceivedFriendListFresh)
	sendCSPartnerInfoReq()

	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUEL_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUELED_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_RED)
end