require("Script/GameConfig/uiTagDefine")
require("Script/Instance/CityDefendRoomLayer")
require("Script/GameConfig/RaidMusic")

CityDefendRoomListLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	createRoomLayerInstance = nil,
	createRoomLayout = nil,

	notificationFunc = nil,
	updateListFunc = 0,

	remainCount = 5,
	endTime = 0,
}

-- 改动较少且遍历繁琐，此处直接写上
CityInstanceList = {}
CityInstanceList[1] = {battle_id=260001, top_level=20}
CityInstanceList[2] = {battle_id=260002, top_level=30}
CityInstanceList[3] = {battle_id=260003, top_level=40}
CityInstanceList[4] = {battle_id=260004, top_level=50}
--CityInstanceList[5] = {battle_id=260005, top_level=60}
--CityInstanceList[6] = {battle_id=260007, top_level=70}

CityInstanceFightingList = {}

local function initInstanceFighting()
    for k, v in pairs(Map_Basic) do
        if tonumber(v.Type) == 7 then
            CityInstanceFightingList[tonumber(v.RaidID)] = tonumber(v.Fighting_Condition)
        end
    end
end

CityDefendRoomListData = {}

-- 活动时间更新
local function onUpdateRemainTime(remainTime)
	if remainTime < 0 then
		remainTime = 0
	end

	local remainStr = TimeManager:Get():secondsToString(remainTime)
	local remainTimeLabel = CityDefendRoomListLayer.uiLayout:FindChildObjectByName("remainTimeNum").__UILabel__
	if	remainTimeLabel ~= nil then
		remainTimeLabel:setString(remainStr)
	end
end


--网络消息收发
local function sendCreateRoomReq(topLevel, battleId)
	CSCDEFCreateRoomReq = {
		top_level = topLevel,
		battle_id = battleId,
		}
	local msgname="CSCDEFCreateRoomReq"
	local ret = send_message(msgname, CSCDEFCreateRoomReq, true)
	return ret;
end

local function sendJoinRoomReq(roomId)
	CSCDEFJoinRoomReq = {
		room_id = roomId,
		}
	local msgname="CSCDEFJoinRoomReq"
	local ret = send_message(msgname, CSCDEFJoinRoomReq, true)
	return ret;
end

local function getCityNumByLevel(level)
	return (level - CityInstanceList[1].top_level) / 10 + 1
end

local function getFightNumByLevel(level)
    local cityNum = getCityNumByLevel(level)
    local fightNum = CityInstanceFightingList[CityInstanceList[cityNum].battle_id]
    return fightNum or 0
end

local function onMsgCityDefendJoinRoomRsp(msg)
	CCLuaLog("onMsgCityDefendJoinRoomRsp ..")

	if msg ~= nil then
		--lua table 传参有问题，直接修改全局变量
		CityDefendRoomData.room_id = msg.room_id
		CityDefendRoomData.top_level = msg.top_level
        CityDefendRoomData.fightNum = getFightNumByLevel(msg.top_level)

		if	msg.seats ~= nil then
			CityDefendRoomData.seats = {}
			for _ , seat in pairs(msg.seats) do
				--位置转化为从1开始
				CityDefendRoomData["seats"][seat.id + 1] = {
					id = seat.id + 1,
					avatar = seat.avatar,
					name = seat.name,
					level = seat.level,
					ready = seat.ready,
                    fightNum = seat.effective,
				}
			end
		end
	end

	CityDefendRoomLayer.RefreshLayer()
	--showUILayerByTag(UITagTable["CityDefendRoomLayer"].tag,true)
end

local function onMsgCityDefendJoinRoomFailRsp(msg)
	CCLuaLog("---Join Room Failed : "..msg.errcode)
	GameApi:showMessage(GetLuaLocalization("M_JOINROOM_FAIL"))
end






local function getFightNumByRoomId(id)
    local level = 0
    for k, v in pairs(CityDefendRoomListData) do
        if v.room_id == id then
            level = v.top_level
            break
        end
    end

    return getFightNumByLevel(level)
end

--点击加入房间
local function onPressJoinSingleRoom(tag)
	CCLuaLog("join room .."..tag)

	--检测剩余次数
	if CityDefendRoomListLayer.remainCount <= 0 then
		GameApi:showMessage(GetLuaLocalization("M_ACTIVITY_NOMORETIME"))
		return
	end

    --检测战斗力
    local userFightingNum = UserData:GetUserInfo().effectiveness
    local requireNum = getFightNumByRoomId(tag)

    if userFightingNum < requireNum then
        GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_ERROR6"))
		return
    end

	sendJoinRoomReq(tag)
end

--添加单个房间
local function addSingleRoom(roomList, roomData)
	local normalRoomBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYDEFEND_GRID_ROOM")
	local clickRoomBg = AspriteManager:getInstance():getOneFrame("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYDEFEND_GRID_ROOM_CLICKED")

	local button = roomList:AddNewItem(normalRoomBg,
		clickRoomBg,
		"",
		"",
		ccc3(255,255,255),
		-130);

	--注册函数
	local btnMenuItem = button:getMenuItemSprite()
	btnMenuItem:setTag(roomData.room_id)
	btnMenuItem:registerScriptTapHandler(onPressJoinSingleRoom)	

	--添加房间详细信息

	--主城图标


	local cityIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_CITY"..getCityNumByLevel(roomData.top_level))
	local _, cityIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYDEFEND_ICON_CITY")
	cityIconSprite:setPosition(ccpSub(cityIconPos, bgPos))

	button:getAnimationNode():addChild(cityIconSprite, 10)

	--房间号
	local roomNumLabel = TXGUI.UILabelTTF:create(""..roomData.room_id, KJLinXin, 24)
	local _, roomNumPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYDEFEND_NUMBER_ROOMNUMBER")
	roomNumLabel:setPosition(ccpSub(roomNumPos, bgPos))

	button:getAnimationNode():addChild(roomNumLabel, 10)

    

	--战斗力
	local fightNumStr = ""..getFightNumByLevel(roomData.top_level)
	local fightNumLabel = TXGUI.UILabelTTF:create(fightNumStr, KJLinXin, 30)
	local _, fightNumPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYDEFEND_NUMBER_ROOMFIGHT")
	--fightNumLabel:setColor(ccc3(0,228,0))
	fightNumLabel:setPosition(ccpSub(fightNumPos, bgPos))

	button:getAnimationNode():addChild(fightNumLabel, 10)

	--人数
	local headCountStr = ""..roomData.current_online.."/"..roomData.max_online
	local headCountLabel = TXGUI.UILabelTTF:create(headCountStr, KJLinXin, 24)
	local _, headCountPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYDEFEND_NUMBER_HEADCOUNT")
	headCountLabel:setPosition(ccpSub(headCountPos, bgPos))

	button:getAnimationNode():addChild(headCountLabel, 10)

end

--刷新房间列表
function CityDefendRoomListLayer:refreshRoomListLayer()
	CCLuaLog("CityDefendRoomListLayer:refreshRoomListLayer")
	if CityDefendRoomListLayer.uiLayout == nil then
		return
	end

	local roomList = CityDefendRoomListLayer.uiLayout:FindChildObjectByName("roomScrollList").__UIScrollList__
	if nil == roomList then
		return
	end

	roomList:clearChildrens()

	for i=1, #CityDefendRoomListData do
		addSingleRoom(roomList, CityDefendRoomListData[i])
	end
end

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if CityDefendRoomListLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(CityDefendRoomListLayer.notificationFunc);
	end

	if CityDefendRoomListLayer.updateListFunc ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(CityDefendRoomListLayer.updateListFunc)
		CityDefendRoomListLayer.updateListFunc = 0
	end

	CityDefendRoomListLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("CityDefendRoomListUI")
	TXGUI.UIManager:sharedManager():removeUILayout("CityDefendCreateRoomUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= CityDefendRoomListLayer.uiLayerInstance then
			if G_CurLayerInstance ~= CityDefendRoomListLayer.uiLayerInstance then
				CityDefendRoomListLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

-- 关闭房间列表页面按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(CityDefendRoomListLayer.uiLayerInstance)
	CityDefendRoomListLayer:destroyed()
	SetChatBar(true,-1)
end

--关闭创建房间页面
local function onCloseCreateRoomBtClicked(tag)
	CCLuaLog("---onCloseCreateRoomBtClicked---")
	CityDefendRoomListLayer.createRoomLayerInstance:setVisible(false)
end

function CityDefendRoomListLayer:CreateLayer()
	CCLuaLog("CityDefendChatLayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["CityDefendRoomListLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/CityDefendRoomListUI.plist",self.uiLayerInstance, "CityDefendRoomListUI", true)
			self:InitLayer()
		end
	end

	if self.createRoomLayerInstance == nil then
		self.createRoomLayerInstance = CCLayer:create()
		self.uiLayerInstance:addChild(self.createRoomLayerInstance, 100)
		self.createRoomLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/CityDefendCreateRoomUI.plist",self.createRoomLayerInstance, "CityDefendCreateRoomUI", true)
		self:InitCreateRoomLayer()
	end
	
	return self.uiLayerInstance
end

-- 重置参数
function CityDefendRoomListLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.createRoomLayerInstance = nil
	self.createRoomLayout = nil
	self.notificationFunc = nil
	self.updateListFunc = 0
end

-- 删除UI
function CityDefendRoomListLayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.createRoomLayerInstance ~= nil then
		self.createRoomLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	if CityDefendRoomListLayer.updateListFunc ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(CityDefendRoomListLayer.updateListFunc)
		CityDefendRoomListLayer.updateListFunc = 0
	end

	--隐藏底部两个按钮
	local chatToolBarLayout = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar")
	if chatToolBarLayout ~= nil then
		local createBtn = chatToolBarLayout:FindChildObjectByName("createButton").__UIButton__
		createBtn:setVisible(false)

		local enterBtn  = chatToolBarLayout:FindChildObjectByName("EnterButton").__UIButton__
		enterBtn:setVisible(false)

		local createLabel = chatToolBarLayout:FindChildObjectByName("createLabel").__UILabel__
		createLabel:setVisible(false)

		local enterLabel  = chatToolBarLayout:FindChildObjectByName("joinLabel").__UILabel__
		enterLabel:setVisible(false)
	end	

	TimeManager:Get():stopTimer(TIMER_CITYDEFEND_REMAIN)

	self:resetValue()
end

-- 点击兑换商店按钮
local function onPressExchangeBtn(tag)
    CCLuaLog("onPressExchangeBtn")
    CallNativeFuncManager:getInstance():AddIndexLayerToMainScene(2)
end

function CityDefendRoomListLayer:InitLayer()

	--检测背包是否已满
	ItemManager:Get():checkBagEmptyItemNum()

	--添加背景
	local imagePath = GameResourceManager:sharedManager():storedFullPathFromRelativePath("game.png")
	local bgSprite = CCSprite:create(imagePath)

	local visibleSize = CCDirector:sharedDirector():getVisibleSize()
	local bgSize = bgSprite:getContentSize()

	local scaleX = visibleSize.width / bgSize.width
	local scaleY = visibleSize.height / bgSize.height

	bgSprite:setScaleX(scaleX)
	bgSprite:setScaleY(scaleY)
	
	bgSprite:setPosition(ccp(visibleSize.width/2, visibleSize.height/2))

	self.uiLayerInstance:addChild(bgSprite, -10)

	-- 更新剩余次数
	local remainCountLabel = self.uiLayout:FindChildObjectByName("todayTimeNum").__UILabel__
	remainCountLabel:setString(""..self.remainCount.."/3")	

	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseBtClicked)

    local exchangeBt = self.uiLayout:FindChildObjectByName("exchangeBt").__UIButton__:getMenuItemSprite()
	exchangeBt:registerScriptTapHandler(onPressExchangeBtn)

	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,10)

	--显示聊天栏上的创建房间和快速加入按钮

	--获取按钮并显示
	local chatToolBarLayout = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar")
	if chatToolBarLayout ~= nil then
		CCLuaLog("chatToolBarLayout ~= nil ")
		local createBtn = chatToolBarLayout:FindChildObjectByName("createButton").__UIButton__
		createBtn:setVisible(true)

		local enterBtn  = chatToolBarLayout:FindChildObjectByName("EnterButton").__UIButton__
		enterBtn:setVisible(true)

		local createLabel = chatToolBarLayout:FindChildObjectByName("createLabel").__UILabel__
		createLabel:setVisible(true)

		local enterLabel  = chatToolBarLayout:FindChildObjectByName("joinLabel").__UILabel__
		enterLabel:setVisible(true)
		ShowPPVEChatButton()
		--注册函数
		local function onCreateRoomBtClicked(tag)
			local isOpen = GetCityDefendStartState()
			if not isOpen then
				GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_TIMEISUP"))
				return
			end

			--检测剩余次数
			if CityDefendRoomListLayer.remainCount <= 0 then
				GameApi:showMessage(GetLuaLocalization("M_ACTIVITY_NOMORETIME"))
				return
			end
			CityDefendRoomListLayer.createRoomLayerInstance:setVisible(true)
		end
		createBtn:getMenuItemSprite():registerScriptTapHandler(onCreateRoomBtClicked)

		local function onQuickJoinRoomBtClicked(tag)
			local isOpen = GetCityDefendStartState()
			if not isOpen then
				GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_TIMEISUP"))
				return
			end

			
			---[[
			--检测剩余次数
			if CityDefendRoomListLayer.remainCount <= 0 then
				GameApi:showMessage(GetLuaLocalization("M_ACTIVITY_NOMORETIME"))
				return
			end
			--]]
			sendJoinRoomReq(0)
		end
		enterBtn:getMenuItemSprite():registerScriptTapHandler(onQuickJoinRoomBtClicked)
	end

	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	self.updateListFunc =  CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(SendGetCityDefendRoomListMsg, 3, false)

	addMsgCallBack("CSCDEFJoinRoomRsp", onMsgCityDefendJoinRoomRsp)
	addMsgCallBack("CSCDEFJoinRoomFailureRsp", onMsgCityDefendJoinRoomFailRsp)

    initInstanceFighting()

	self.uiLayout:setVisibleInStyle(true)
end

--创建某个具体房间
local function onPressCreateSingleRoom(tag)
    --[[
	local isOpen = GetCityDefendStartState()
	if not isOpen then
		GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_TIMEISUP"))
		return
	end
    ]]

	--检测剩余次数
	if CityDefendRoomListLayer.remainCount <= 0 then
		GameApi:showMessage(GetLuaLocalization("M_ACTIVITY_NOMORETIME"))
		return
	end

	CCLuaLog("trying create room .."..tag)
	if	CityInstanceList[tag] == nil then
		return
	end
	--showUILayerByTag(UITagTable["CityDefendRoomLayer"].tag,true)

    local userFightingNum = UserData:GetUserInfo().effectiveness
    local requireNum = CityInstanceFightingList[CityInstanceList[tag].battle_id]


	if userFightingNum < requireNum then
		GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_ERROR1"))
	else
		CCLuaLog("send create room msg...")
		sendCreateRoomReq(CityInstanceList[tag].top_level, CityInstanceList[tag].battle_id)
	end
end

function CityDefendRoomListLayer:InitCreateRoomLayer()
	local closeBt = self.createRoomLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseCreateRoomBtClicked);

	CityDefendRoomListLayer.createRoomLayerInstance:setVisible(false)

	local function addOneRoom(roomList, roomNum)
		CCLuaLog("addOneRoom..."..roomNum)
		local normalRoomBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYCREATEROOM_BUTTON_ROOM1")
		local clickRoomBg = AspriteManager:getInstance():getOneFrame("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYCREATEROOM_BUTTON_ROOM1_CLICKED")		

		--通过房间号获得房间等级
		local function getRoomLevel(roomNum)
			return CityInstanceList[1].top_level + 10 * (roomNum - 1)
		end

		local button = roomList:AddNewItem(normalRoomBg,
			clickRoomBg,
			"",
			"",
			ccc3(255,255,255),
			-133);

		--注册函数
		local btnMenuItem = button:getMenuItemSprite()
		btnMenuItem:setTag(roomNum)
		btnMenuItem:registerScriptTapHandler(onPressCreateSingleRoom)	

		--主城图标
		local cityIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_CITY"..roomNum)
		local _, cityIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYCREATEROOM_ICON_CITY1")
		cityIconSprite:setPosition(ccpSub(cityIconPos, bgPos))

		button:getAnimationNode():addChild(cityIconSprite, 10)

		--房间名称
		local roomName = GetLuaLocalization("M_CITYDEFEND_CITY"..roomNum)
		local roomNameLabel = TXGUI.UILabelTTF:create(roomName, KJLinXin, 24)
		local _, roomNamePos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYCREATEROOM_TEXT_ROOMNAME")
		roomNameLabel:setPosition(ccpSub(roomNamePos, bgPos))
		roomNameLabel:setColor(ccc3(255,184,0))

		button:getAnimationNode():addChild(roomNameLabel, 10)

        --战斗力文本
		local fightText = GetLuaLocalization("M_CITYDEFEND_NEEDFIGHT1")
        local fightTextFrame, fightTextPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYCREATEROOM_TEXT_ROOMFIGHT")
        local fightTextLabel = TXGUI.UILabelTTF:create(fightText, KJLinXin, 20, fightTextFrame:getContentSize(), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter)
	
		fightTextLabel:setPosition(ccpSub(fightTextPos, bgPos))
		fightTextLabel:setColor(ccc3(255,195,118))

		button:getAnimationNode():addChild(fightTextLabel, 10)

		--战斗力数量
        local fightNumStr = ""..CityInstanceFightingList[CityInstanceList[roomNum].battle_id]
        local fightNumFrame, fightNumPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui_ppve.bin","map_ui_ppve_FRAME_CITYCREATEROOM_NUM_ROOMFIGHT")
        local fightNumLabel = TXGUI.UILabelTTF:create(fightNumStr, KJLinXin, 20, fightNumFrame:getContentSize(), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter)
   	    fightNumLabel:setPosition(ccpSub(fightNumPos, bgPos))
        fightNumLabel:setColor(ccc3(255,195,118))

	    button:getAnimationNode():addChild(fightNumLabel, 10)

	end

	--添加房间
	local roomList = CityDefendRoomListLayer.createRoomLayout:FindChildObjectByName("roomScrollList").__UIScrollList__
	if nil == roomList then
		CCLuaLog("createRoomLayout:roomList null")
		return
	end

	roomList:clearChildrens()
	for i=1, #CityInstanceList do
		addOneRoom(roomList, i)
	end	
end

--更新剩余次数及结束时间的全局函数
function UpdateCityDefendRemainCount(count)
	CCLuaLog("UpdateCityDefendRemainCount : "..count)
	CityDefendRoomListLayer.remainCount = count
	if CityDefendRoomListLayer.uiLayout ~= nil then
		local remainCountLabel = CityDefendRoomListLayer.uiLayout:FindChildObjectByName("todayTimeNum").__UILabel__
		remainCountLabel:setString(""..CityDefendRoomListLayer.remainCount.."/3")	
	end
end