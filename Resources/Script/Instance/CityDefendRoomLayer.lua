require("Script/GameConfig/uiTagDefine")

CityDefendRoomLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,
}

CityDefendRoomData = {
	room_id = 1,
	top_level = 50,
}

CityDefendRoomData["seats"] = {}

-- 关闭主城防守房间页面按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(CityDefendRoomLayer.uiLayerInstance)
	CityDefendRoomLayer:destroyed()
	showUILayerByTag(UITagTable["CityDefendRoomListLayer"].tag,true)
end

--网络消息收发
--发送退出房间消息
local function sendExitRoomReq()
	CSCDEFExitRoomReq = {
		}
	local msgname="CSCDEFExitRoomReq"
	local ret = send_message(msgname, CSCDEFExitRoomReq, true)
	return ret;
end

--发送玩家准备消息
local function sendReadyReq()
	CSCDEFReadyReq = {
		}
	local msgname="CSCDEFReadyReq"
	local ret = send_message(msgname, CSCDEFReadyReq, true)
	return ret;
end

--发送取消准备消息
local function sendCancelReadyReq()
	CSCDEFCancelReadyReq = {
		}
	local msgname="CSCDEFCancelReadyReq"
	local ret = send_message(msgname, CSCDEFCancelReadyReq, true)
	return ret;
end

--发送开始游戏
local function sendStartGameReq()
	CSCDEFStartReq = {
		}
	local msgname="CSCDEFStartReq"
	local ret = send_message(msgname, CSCDEFStartReq, true)
	return ret;
end

--发送踢出玩家
local function sendKickPlayerReq(index)
	CSCDEFKickoffReq = {
		seat = index,
		}
	local msgname="CSCDEFKickoffReq"
	local ret = send_message(msgname, CSCDEFKickoffReq, true)
	return ret;
end


--收到房间关闭消息
local function onMsgCityDefendRoomCloseRsp(msg)
	onCloseBtClicked(nil)
	GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_ERROR2"))
end

--收到被房主踢出消息
local function onMsgCityDefendKickOffRsp(msg)
	onCloseBtClicked(nil)
	GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_ERROR5"))
end

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if CityDefendRoomLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(CityDefendRoomLayer.notificationFunc);
	end

	CityDefendRoomLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("CityDefendRoomUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= CityDefendChatLayer.uiLayerInstance then
			if G_CurLayerInstance ~= CityDefendRoomLayer.uiLayerInstance then
				CityDefendRoomLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end



function CityDefendRoomLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["CityDefendRoomLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/CityDefendRoomUI.plist",self.uiLayerInstance, "CityDefendRoomUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function CityDefendRoomLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function CityDefendRoomLayer:destroyed()
	sendExitRoomReq()

	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
end

-- 根据房间数据刷新页面，目前仅通过这个创建页面
function CityDefendRoomLayer:RefreshLayer()
	if #(CityDefendRoomData["seats"]) < 1 then
		CCLuaLog("==== Room Data Error ====")
		return
	end

	--页面不存在则创建页面
	if CityDefendRoomLayer.uiLayerInstance == nil then
		CityDefendRoomLayer:CreateLayer()
	end

	--更新房间界面
	--标题
	local roomTitle = CityDefendRoomLayer.uiLayout:FindChildObjectByName("titleTxt").__UILabel__
	roomTitle:setString("房间"..CityDefendRoomData.room_id)

	--房间名称
	local function getRoomNumByLevel(level)
		return (level - 10) / 10
	end

	local roomNameStr = GetLuaLocalization("M_CITYDEFEND_CITY"..getRoomNumByLevel(CityDefendRoomData.top_level))
	local roomNameLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("roomNameTxt").__UILabel__
	roomNameLabel:setString(roomNameStr)

	--玩家头像
	local function getHeadIcon(avatar)		
		local frameResTable =  {
			"map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSWOMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDWOMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINWOMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERMAN",
			"map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERWOMAN",
		}

		return AspriteManager:getInstance():getOneFrame("UI/ui_character_select.bin", frameResTable[avatar])	
	end

    -- 初始化房主信息
    local hostHeadIcon = CityDefendRoomLayer.uiLayout:FindChildObjectByName("hostHeadIcon").__UIPicture__
	hostHeadIcon:setSprite(getHeadIcon(CityDefendRoomData["seats"][1].avatar))

    local hostLevelLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("hostPlayerLevel").__UILabel__
	hostLevelLabel:setString("Lv."..CityDefendRoomData["seats"][1].level)

    local hostNameLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("hostPlayerName").__UILabel__
	hostNameLabel:setString(CityDefendRoomData["seats"][1].name)

    local hostFightLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("hostFightNum").__UILabel__
	hostFightLabel:setString(""..GetLuaLocalization("M_MSROOM_FIGHT")..CityDefendRoomData["seats"][1].fightNum)

    --踢出按钮回调函数
	local function onClickKickBtn(tag)
		CCLuaLog("sendKickPlayerReq..."..tag)
		sendKickPlayerReq(tag)
	end

    --判断玩家是不是房主
	local heroName = UserData:getUserName()
	local hostName = CityDefendRoomData["seats"][1].name
	local isHost = (heroName == hostName)
	

    -- 逐个添加玩家信息
    for i = 1, 3 do
        -- 头像
        local guestHeadIcon = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestHeadIcon"..i).__UIPicture__

        --准备标记
        local guestReadyIcon = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestIconReady"..i).__UIPicture__

        --等级
	    local guestLevelLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestPlayerLevel"..i).__UILabel__

        --名称
	    local guestNameLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestPlayerName"..i).__UILabel__

        --等待文本
	    local guestWaitLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestWaitTxt"..i).__UILabel__

        --战斗力
        local guestFightLabel = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestFightNum"..i).__UILabel__

        --踢出按钮
	    local guestKickBtn = CityDefendRoomLayer.uiLayout:FindChildObjectByName("guestKickBt"..i).__UIButton__

        if CityDefendRoomData["seats"][i+1] ~= nil then
            guestHeadIcon:setSprite(getHeadIcon(CityDefendRoomData["seats"][i+1].avatar))
		    guestHeadIcon:setVisible(true)

            guestReadyIcon:setVisible(CityDefendRoomData["seats"][i+1].ready)

            guestLevelLabel:setString("Lv."..CityDefendRoomData["seats"][i+1].level)

            guestNameLabel:setString(CityDefendRoomData["seats"][i+1].name)

            guestWaitLabel:setVisible(false)

            guestFightLabel:setString(""..GetLuaLocalization("M_MSROOM_FIGHT")..CityDefendRoomData["seats"][i+1].fightNum)

            guestKickBtn:setVisible(isHost)

        else
            guestHeadIcon:setVisible(false)

            guestReadyIcon:setVisible(false)

            guestLevelLabel:setString("")

            guestNameLabel:setString("")

            guestWaitLabel:setVisible(true)

            guestFightLabel:setString("")

            guestKickBtn:setVisible(false)
        end

        local kickBtnMenu = guestKickBtn:getMenuItemSprite()
        kickBtnMenu:setTag(i)
	    kickBtnMenu:registerScriptTapHandler(onClickKickBtn)
    end  



    local function getSelfGuestIndex()
        if isHost then return 0 end

        for i = 1, 3 do
            if heroName == CityDefendRoomData["seats"][i+1].name then
                return i
            end
        end
    end

    local selfGuestIndex = getSelfGuestIndex()

	--开始及准备按钮
	local goBtn = CityDefendRoomLayer.uiLayout:FindChildObjectByName("goBt").__UIButton__
	local readyBtn = CityDefendRoomLayer.uiLayout:FindChildObjectByName("readyBt").__UIButton__
	if isHost then
		goBtn:setVisible(true)
		readyBtn:setVisible(false)
	else
		goBtn:setVisible(false)
		readyBtn:setVisible(true)
	end

	--注册按钮回调函数



	--开始按钮注册
	local function onClickGoBtn(tag)
        --sendStartGameReq()
        --do return end

        local function isAllReady()
            local ret = true
            for i = 1, 3 do
                if CityDefendRoomData["seats"][i+1] ~= nil and not CityDefendRoomData["seats"][i+1].ready then
                    ret = false
                    break
                end
            end

            return ret
        end

		if #(CityDefendRoomData["seats"]) > 1 then
			if isAllReady() then
				CCLuaLog("sendStartGameReq...")
				sendStartGameReq()
			else
				GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_ERROR3"))
			end
		else
			GameApi:showMessage(GetLuaLocalization("M_CITYDEFEND_ERROR4"))
		end		
	end

	local goBtnMenu = goBtn:getMenuItemSprite()
	goBtnMenu:registerScriptTapHandler(onClickGoBtn)

	--准备按钮注册
	local function onClickReadyBtn(tag)
		local isReady = CityDefendRoomData["seats"][selfGuestIndex + 1].ready
		if isReady then
			sendCancelReadyReq()
		else
			sendReadyReq()
		end
	end

	local readyBtnMenu = readyBtn:getMenuItemSprite()
	readyBtnMenu:registerScriptTapHandler(onClickReadyBtn)
end

-- 初始化页面
function CityDefendRoomLayer:InitLayer()
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

	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseBtClicked);

	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,10)

	addMsgCallBack("CSCDEFCloseRoomRsp", onMsgCityDefendRoomCloseRsp)
	addMsgCallBack("CSCDEFKickoffRsp", onMsgCityDefendKickOffRsp)

	self.uiLayout:setVisibleInStyle(true)

	-- 显示自动喊话按钮
	ShowAutoChatItems(3)
	ShowPPVEChatButton()
end