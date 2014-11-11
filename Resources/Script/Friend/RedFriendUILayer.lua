require("Script/GameConfig/uiTagDefine")
require("Script/Friend/SelectedFriendContainer")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Friend/FriendListItem")
require("Script/Friend/PlayerItemCenter")
require("Script/Friend/addFriend")
require("Script/Chat/ChatPlayerOption")

RedFriendUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	nearList = {},
	uiList = nil,
	notificationFunc = nil,
	selectedListItem = nil,
	detailInfoContainer = nil,

	--Note: 红名倒计时
	redNameTimer = nil,			-- 计时器
	countDown = -1,				-- 倒计时(秒)
	redNameTimerLabel = nil,	-- 红名显示Label
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if RedFriendUILayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(RedFriendUILayer.notificationFunc);
	end

	RedFriendUILayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("redFriendUI")

	resetFriendCallBacks()

	if nil ~= RedFriendUILayer.redNameTimer and (TimeManager:Get():hasTimer(TIMER_RED_NAME_REMAIN)) then
		TimeManager:Get():unregisterLuaTimer(RedFriendUILayer.redNameTimer, TIMER_RED_NAME_REMAIN);
	end
end

function RedFriendUILayer:CreateLayer()
	CCLuaLog("RedFriendUILayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["RedFriendUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/redFriendUI.plist",self.uiLayerInstance, "redFriendUI", true)
			self:InitLayer();
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function RedFriendUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiList = nil
	self.nearList = nil
	self.notificationFunc = nil
	self.detailInfoContainer = nil
	self.selectedListItem = nil
	self.redNameTimer = nil
	self.countDown = -1
	self.redNameTimerLabel = nil
end

-- 删除UI
function RedFriendUILayer:destroyed()
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

--Note 红名时间相关的协议
function RedFriendUILayer:sendCSGetRednameTimeReq(playerInfo)
	local ret = false
	if playerInfo ~= nil then
		local id = playerInfo:getID()
		CSGetRednameTimeReq = {
			userid = id:getID(),
			reg_tm = id:getRegTime(),
			channelid = id:getChannel(),
		}
		local msgname="CSGetRednameTimeReq"
		ret = send_message(msgname, CSGetRednameTimeReq, true)
	end
	return ret
end

local function onUpdateRedNameTime(remainTime)
	
	if remainTime > 0 then
		--local dayNum = math.floor(remainTime / 86400)
		--local leftTime = math.mod(remainTime,86400)
		local hourNum = math.floor(remainTime / 3600)
		local leftTime = math.mod(remainTime,3600)
		local minuterNum = math.floor(leftTime / 60)
		local secNum = math.mod(leftTime,60)

		local timeStr = nil
		if hourNum >= 100 then
			if hourNum > 999 then
				hourNum = 999
			end
			timeStr = string.format("%03d:%02d:%02d",hourNum,minuterNum,secNum);
		else
			timeStr = string.format("%02d:%02d:%02d",hourNum,minuterNum,secNum);
		end	
	
		if RedFriendUILayer.redNameTimerLabel ~= nil then
			RedFriendUILayer.redNameTimerLabel:setString(timeStr);
		end
	else
		if RedFriendUILayer.redNameTimerLabel ~= nil then
			RedFriendUILayer.redNameTimerLabel:setString("0");
		end		
		sendCSNearbyRednameReq()
	end
end

local function renewRedNameTimer()
	local nowTime = TimeManager:Get():getCurServerTime()
	local seconds = RedFriendUILayer.countDown;
	if (TimeManager:Get():hasTimer(TIMER_RED_NAME_REMAIN)) then
		TimeManager:Get():renewTimer(TIMER_RED_NAME_REMAIN, nowTime + seconds)
		CCLuaLog("--------renew RedName timer : " .. seconds)
	else
		onlineRewardTimer = TimeManager:Get():registerLuaTimer(onUpdateRedNameTime, TIMER_RED_NAME_REMAIN, nowTime + seconds)
		CCLuaLog("--------init RedName timer : " .. seconds)
	end

end

local function receiveCSGetRednameTimeRsp(msg)
	if nil ~= msg then
		print("---------------------------------------receiveCSGetRednameTimeRsp time " .. msg.time)
		RedFriendUILayer.countDown = msg.time
		if RedFriendUILayer.countDown > 0 then --start timer and countdown
			renewRedNameTimer()
		else
			sendCSNearbyRednameReq()
		end
	end
end

function RedFriendUILayer:setCSGetRednameTimeCallBack()
	addMsgCallBack("CSGetRednameTimeRsp", receiveCSGetRednameTimeRsp)
end

--local function onReceivedNearListFresh()
	--RedFriendUILayer:resetList(false)
--end

local function onReceivedNearRedListFresh()
	print("-------------------------- red friend call back")
	RedFriendUILayer:resetList(false)
end

-- 关闭按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(RedFriendUILayer.uiLayerInstance)
	RedFriendUILayer:destroyed()
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
-- 宿敌
local function onEnemiesBtClicked(tag)
	showUILayerByTag(UITagTable["EnemiesFriendUILayer"].tag,true)
	--print("Enemies")
end

-- 刷新按钮
local function onUpdateBtClicked(tag)
	--sendCSNearByReq()
	sendCSNearbyRednameReq()
end

-- 添加好友
local function onAddFriendBtClicked(tag)
	if RedFriendUILayer.selectedListItem ~= nil then
		local id = RedFriendUILayer.selectedListItem.id
		local friendInfo = getSignleFriendInfo(id)
		if friendInfo ~= nil then
			sendAddFriendReq(friendInfo:getName())
		end
	end
end

-- 显示玩家详细信息
local function onClickedDetailBt()
	if RedFriendUILayer.selectedListItem ~= nil then
		local id = RedFriendUILayer.selectedListItem:getID()
		PlayerItemCenter:setSelectedPlayer(id)
		showUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,true)
	end
end

local function onChatBtClicked(tag)
	if (RedFriendUILayer.nearList ~= nil) and (RedFriendUILayer.nearList[tag] ~= nil) then
		local id = RedFriendUILayer.nearList[tag]:getID()
		local playerInfo = getSignleFriendInfo(id)
		if playerInfo ~= nil then
			reqToPrivateChat(id,playerInfo:getName())
		end
	end
end

local function onMoreBtClicked(tag)
	if (RedFriendUILayer.nearList ~= nil) and (RedFriendUILayer.nearList[tag] ~= nil) then
		local id = RedFriendUILayer.nearList[tag]:getID()
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
	if (RedFriendUILayer.nearList ~= nil) and (RedFriendUILayer.nearList[tag] ~= nil) then
		local id = RedFriendUILayer.nearList[tag]:getID()
		RedFriendUILayer:setSelectedItem(RedFriendUILayer.nearList[tag])

		if RedFriendUILayer.detailInfoContainer ~= nil then
			--local friendTable = getNearPlayerInfo(false)
			local playerInfo = getSignleFriendInfo(id)
			if playerInfo ~= nil then
				RedFriendUILayer.detailInfoContainer:setSelectFriend(playerInfo)
				RedFriendUILayer:sendCSGetRednameTimeReq(playerInfo)
			end
		end
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= RedFriendUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= RedFriendUILayer.uiLayerInstance then
				RedFriendUILayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function RedFriendUILayer:setSelectedItem(item)
	if self.selectedListItem ~= nil then
		self.selectedListItem:setButtonUnselected()
	end
	if item ~= nil then
		item:setButtonSelected()
	end

	self.selectedListItem = item
end

function RedFriendUILayer:resetList(refresh)
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
			--local friendTable = getNearPlayerList(refresh)
			local friendTable = getNearRedPlayerList(refresh)
			for k,v in pairs(friendTable) do 
				print("-------------------------- red friend tag " .. tag)
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
							RedFriendUILayer:sendCSGetRednameTimeReq(friendInfo)
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

function RedFriendUILayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseBtClicked);
	--local friendListBt = self.uiLayout:FindChildObjectByName("friendListBt").__UIButton__:getMenuItemSprite();
	--friendListBt:registerScriptTapHandler(onFriendListBtClicked);
	local updateListBt = self.uiLayout:FindChildObjectByName("friendUpdateBt").__UIButton__:getMenuItemSprite();
	updateListBt:registerScriptTapHandler(onUpdateBtClicked);
	--local addFriendBt = self.uiLayout:FindChildObjectByName("addFriendBt").__UIButton__:getMenuItemSprite();
	--addFriendBt:registerScriptTapHandler(onAddFriendBtClicked);

	self.redNameTimerLabel = self.uiLayout:FindChildObjectByName("redNameTimeNumLabel").__UILabel__

	--local detailSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL")
	--local detailClickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL_CLICKED")
	--local detailBt = self.uiLayout:FindChildObjectByName("detailBt").__UIButton__:getMenuItemSprite();
	--detailBt:setNormalImage(detailSprite)
	--detailBt:setSelectedImage(detailClickedSprite)
	--detailBt:registerScriptTapHandler(onClickedDetailBt)

	--Note: 其他标签页回调接口
	local friendListBt = self.uiLayout:FindChildObjectByName("friendListBtn").__UIButton__:getMenuItemSprite();
	friendListBt:registerScriptTapHandler(onFriendBtClicked);
	local nearFriendListBt = self.uiLayout:FindChildObjectByName("nearByFriendBtn").__UIButton__:getMenuItemSprite();
	nearFriendListBt:registerScriptTapHandler(onNearFriendBtClicked);
	local enemiesListBt = self.uiLayout:FindChildObjectByName("enemiesBtn").__UIButton__:getMenuItemSprite();
	enemiesListBt:registerScriptTapHandler(onEnemiesBtClicked);

	self.detailInfoContainer = SelectedFriendContainer:create()
	self.detailInfoContainer:init(self.uiLayout)

	self.uiList = self.uiLayout:FindChildObjectByName("friendList").__UIScrollList__;
	self.nearList = {}

	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

	--setNearPlayerListCallback(onReceivedNearListFresh)
	setRedFriendListCallback(onReceivedNearRedListFresh)
	--sendCSNearByReq()
	sendCSNearbyRednameReq()

	self:setCSGetRednameTimeCallBack()

	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUEL_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_DUELED_TIMES)
	XLogicManager:sharedManager():reqPlayerAttributes(PB_ATTR_PLAYER_PVP_RED)
end