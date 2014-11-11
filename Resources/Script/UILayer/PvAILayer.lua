require("Script/CommonDefine")
require("Script/Language")
require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/characterString")
require("Script/GameConfig/vip")
require("Script/GameConfig/ExchangeParameter")
require("Script/Friend/PlayerItemCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/UILayer/PayHintLayer")


PvAILayer = {
	LayerInstance = nil,
	Layout = nil,

	buyLayerInstance = nil,
	buyLayout = nil,
	notificationFunc = 0,
}

PlayerListInfo = {}
OffsetPositionY = 0
HeroRank = 0
LastFightIndex = 1
ItemPosYTable = {}

updatePvAIInfoFunc = 0

--本地函数
--更新挑战冷却时间
local function onUpdateCoolDownTime(remainTime)		
	local remainText = LanguageLocalization:GetLocalization("M_PVAI_COOLDOWNTIME")
	local remainStr = TimeManager:Get():secondsToString(remainTime)
	local coolDownLabel = PvAILayer.Layout:FindChildObjectByName("pvaiCoolDownTime").__UILabel__
	if	coolDownLabel ~= nil then
		if remainTime ~= 0 then
			coolDownLabel:setString(remainText..remainStr)
		else
			coolDownLabel:setString("")
		end
	end
end


--销毁页面
local function destroyPvAILayer()
	if PvAILayer.LayerInstance ~= nil then		
		TXGUI.UIManager:sharedManager():removeUILayout("PvAILayout")
		TXGUI.UIManager:sharedManager():removeUILayout("PvAIBuyTimeLayout")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		if PvAILayer.notificationFunc ~= 0 then
			NotificationCenter:defaultCenter():unregisterScriptObserver(PvAILayer.notificationFunc)
		end
		RemoveOneLayer(PvAILayer.LayerInstance)	
        PvAILayer.LayerInstance:removeFromParentAndCleanup(true)
		PvAILayer:ResetValue()
		TimeManager:Get():stopTimer(TIMER_PVAI_COOLDOWN)

		
	end
end

--关闭页面
local function closePvAILayer(obj)
	RemoveOneLayer(PvAILayer.LayerInstance)
	destroyPvAILayer()
	SetChatBar(true,-1)
end


-- CCLayer 析构的回调
local function onReceiveLayerDestructor()
	destroyPvAILayer()
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= PvAILayer.LayerInstance then
			if G_CurLayerInstance ~= PvAILayer.LayerInstance then
				destroyPvAILayer()
				ret = 1
			end
		end
	end

	return ret
end

function sendGetPvAIRewardMsg()
	CSSelectPVAIACCURewardReq = {
	}
	local msgname="CSSelectPVAIACCURewardReq"
	local ret = send_message(msgname, CSSelectPVAIACCURewardReq, true)
	return ret;
end

--获取pvai挑战玩家列表信息
local function sendPvAIInfoReq()
	CSPVAIInfoReq = {
	}
	local msgname="CSPVAIInfoReq"
	local ret = send_message(msgname, CSPVAIInfoReq, true)
	return ret
end

local function sendCSAttributesSetReq()
	CSAttributesSetReq = {
		key = PB_ATTR_GET_DAILY_REP_EXCHANGE_FLAG,
		value = 1,
	}
	local msgname="CSAttributesSetReq"
	local ret = send_message(msgname, CSAttributesSetReq, true)
	return ret
end

function onMsgPvAIReward(msg)
	PvAIManager:Get():onMsgPvAIReward(msg.accu_coin, msg.accu_reputation, msg.one_hour_coin, msg.one_hour_reputation)
	return false
end

local function checkBtnInView(index)
	local ret = false
	local playerList = PvAILayer.Layout:FindChildObjectByName("playerScrollList").__UIScrollList__
	local viewHeight = playerList:getViewSize().height
	local offsetY = playerList:getContentOffset().y
	local btnY = ItemPosYTable[index]

	local btnOffset = viewHeight / 2 - btnY
	if btnOffset > offsetY and btnOffset < offsetY + viewHeight then
		ret = true
	end
	return ret
end

local function onPressFightBtn(obj)
	if not checkBtnInView(obj) then
		CCLuaLog("out btn !!!")
		return 
	end

    if obj == LastFightIndex then
        TutorialsManager:Get():HandleOneEvent("pvaiFightBtn5", 1)
    end    

	GameAudioManager:sharedManager():playEffect(350001,false);
	local remainCount = PvAIManager:Get():getRemainCount()
	local remainTime = PvAIManager:Get():getCoolDownEndTime() - TimeManager:Get():getCurServerTime()
	if	remainCount ~= 0 and remainTime <= 0 then
		closePvAILayer(nil)
		PvAIManager:Get():sendPvAIReq(obj)
	elseif remainCount == 0 then
		local rstString = LanguageLocalization:GetLocalization("M_PVAI_NOMORETIME");
		GameApi:showMessage(rstString)
	elseif remainTime > 0 then
		local rstString = LanguageLocalization:GetLocalization("M_PVAI_INCD");
		GameApi:showMessage(rstString)
	end	

    
end

local function onPressViewBtn(tag)
	if not checkBtnInView(tag) then
		CCLuaLog("out btn !!!")
		return 
	end

	if PlayerListInfo[tag] == nil then
		return
	end

	--playerId = PLAYER_ID:new()
	--playerId:setPlayId(PlayerListInfo[tag].userid,PlayerListInfo[tag].reg_time,PlayerListInfo[tag].channel_id)
	local playerId = PlayerIdMake(PlayerListInfo[tag].userid,PlayerListInfo[tag].reg_time,PlayerListInfo[tag].channel_id)
	PlayerItemCenter:setSelectedPlayer(playerId)
	showUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,true)
end

local function addSinglePlayer(playerList, playerData, index)
	local playerBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_GRID_OTHERPLAYER1")
	local bgListItemSize = playerBg:getContentSize()		
	local ListItemNode = CCNode:create()

	local bgControl = playerList:AddBaseItem(bgListItemSize, ListItemNode)

	local _, posY = bgControl:getCurrentNode():getPosition()
	ItemPosYTable[index] = posY
	CCLuaLog(""..posY)
	local viewHeight = playerList:getViewSize().height
	posY = - posY + viewHeight / 2

	--背景
	bgControl:getCurrentNode():addChild(playerBg, 10)

	--头像
	local headFrame = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_INSTANCE_GRID1")
	local _, headIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_ICON_OTHERPLAYER1")

	headFrame:setPosition(ccpSub(headIconPos, bgPos))

	headFrame:setScale(0.5)
	bgControl:getCurrentNode():addChild(headFrame, 10)

	--头像具体内容
	local headPicName = Characters[playerData.type].HeadPicture
	local headPic = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", headPicName)
	headPic:setPosition(ccpSub(headIconPos, bgPos))
	headPic:setScale(0.5)

	
	bgControl:getCurrentNode():addChild(headPic, 10)

    -- 精灵
	--playerData.fairys = {2001, 2002, 2003, 2004, 2005}
    for i = 1, math.min(#playerData.fairys, 4) do
        local fairyId = playerData.fairys[i]
		local fairyFrame = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_PVAI_FRAME_OTHERFAIRY")

		local fairyBasicInfo = getFairyBasicInfo(fairyId)
		local fairyIcon = fairyBasicInfo:getFairyIcon()

		local _ , fairyFramePos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_FRAME_OTHERFAIRY"..i)

		fairyFrame:setPosition(ccpSub(fairyFramePos, bgPos))
		fairyIcon:setPosition(ccpSub(fairyFramePos, bgPos))

		fairyFrame:setScale(0.8)
		fairyIcon:setScale(0.8)
		bgControl:getCurrentNode():addChild(fairyFrame, 10)
		bgControl:getCurrentNode():addChild(fairyIcon, 10)
    end

	--名称
	local nameLabel = TXGUI.UILabelTTF:create(playerData.name, KJLinXin, 24)
	local _, nameLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_TEXT_OTHERPLAYERNAME1")
	nameLabel:setPosition(ccpSub(nameLabelPos, bgPos))
	--nameLabel:setColor(ccc3(255,162,0))

	bgControl:getCurrentNode():addChild(nameLabel, 10)


	--等级
	local levelLabel = TXGUI.UILabelTTF:create("Lv"..playerData.lv, KJLinXin, 22)
	local _, levelLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_TEXT_OTHERPLAYERLEVEL1")
	levelLabel:setPosition(ccpSub(levelLabelPos, bgPos))
    levelLabel:setColor(ccc3(255,162,0))
	
	bgControl:getCurrentNode():addChild(levelLabel, 10)

	--排名
	--local rankLabel = TXGUI.UILabelTTF:create(""..GetLuaLocalization("M_PVAI_RANK2")..playerData.rank, KJLinXin, 22)
    local rankLabel = TXGUI.UILabelTTF:create(""..playerData.rank, KJLinXin, 22)
	local _, rankLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_TEXT_OTHERPLAYERRANK1")
	rankLabel:setPosition(ccpSub(rankLabelPos, bgPos))
    --rankLabel:setColor(ccc3(255,162,0))
	
	bgControl:getCurrentNode():addChild(rankLabel, 10)

	--挑战按钮
	local normalSprite, fightBtnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_BUTTON_FIGHT1")
    local clickedSprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_PVAI_BUTTON_FIGHT_CLICKED1")
 
	fightBtn = IconButton:create(normalSprite, nil, clickedSprite, -132)
	fightBtn:setPosition(ccpSub(fightBtnPos, bgPos))
    fightBtn:setContentSize(normalSprite:getContentSize())

	if index == LastFightIndex then
		print("fight index : "..index)
		fightBtn:SetButtonName("pvaiFightBtn5")
	end
	
	--bgControl:getCurrentNode():addChild(fightBtn, 10)

	--注册函数
	local btnMenuItem = fightBtn:getMenuItem()
	btnMenuItem:setTag(index)

	--挑战提示文本，或查看信息提示文本
	local infoLabel = TXGUI.UILabelTTF:create("", KJLinXin, 24)
	local _, infoLabelPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_TEXT_FIGHT1")
	infoLabel:setPosition(ccpSub(infoLabelPos, bgPos))	
	

	-- 前三名图标
	if playerData.rank <= 3 then
		local iconRankSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_RANK"..playerData.rank)
		local _, iconRankPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_ICON_NO1")
		iconRankSprite:setPosition(ccpSub(iconRankPos, bgPos))
		bgControl:getCurrentNode():addChild(iconRankSprite, 10)
	end

	--按钮分类处理
	if playerData.status == "PB_AI_STATUS_FRONT" then
		bgControl:getCurrentNode():addChild(fightBtn, 10)
		bgControl:getCurrentNode():addChild(infoLabel, 12)
		btnMenuItem:registerScriptTapHandler(onPressFightBtn)
		infoLabel:setString("挑战")
	elseif playerData.status == "PB_AI_STATUS_TOP10" then
		bgControl:getCurrentNode():addChild(fightBtn, 10)
		bgControl:getCurrentNode():addChild(infoLabel, 12)
		btnMenuItem:registerScriptTapHandler(onPressViewBtn)
		infoLabel:setString("查看")
	elseif playerData.status == "PB_AI_STATUS_SELF" then
		nameLabel:setColor(ccc3(0,255,0))
		levelLabel:setColor(ccc3(0,255,0))
		rankLabel:setColor(ccc3(0,255,0))

		local selfMarkPic = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_MYMARK")
		local _, selfMarkPicPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_PVAI_ICON_MYMARK1")

		selfMarkPic:setPosition(ccpSub(selfMarkPicPos, bgPos))
		bgControl:getCurrentNode():addChild(selfMarkPic, 12)

		OffsetPositionY = posY - viewHeight / 2 - bgListItemSize.height / 2
		HeroRank = playerData.rank
	elseif playerData.status == "PB_AI_STATUS_BACK" then
        bgControl:getCurrentNode():addChild(fightBtn, 10)
		bgControl:getCurrentNode():addChild(infoLabel, 12)
		btnMenuItem:registerScriptTapHandler(onPressFightBtn)
		infoLabel:setString("挑战")
	end

	
end

local function refreshPlayerList()
	if PvAILayer.Layout == nil then
		return
	end

	local playerList = PvAILayer.Layout:FindChildObjectByName("playerScrollList").__UIScrollList__
	if nil == playerList then
		return
	end

	playerList:clearChildrens()

	--找到玩家前一名对手
	for i = 1, #PlayerListInfo do
		if PlayerListInfo[i].status == "PB_AI_STATUS_SELF" then
			LastFightIndex = i - 1
		end
	end	
	
	for i = 1, #PlayerListInfo do
		addSinglePlayer(playerList, PlayerListInfo[i], i)
	end	

	if HeroRank > 5 then
		playerList:setStartOffset(ccp(0, OffsetPositionY))
	end

end

local function onMsgPvAIInfoRsp(msg)
	if msg.infos ~= nil then
		PlayerListInfo = {}
		for _ , info in pairs(msg.infos) do
            local fairyList = {}
            if info.fairys ~= nil then
                for _ , id in pairs(info.fairys) do
                    table.insert(fairyList, id)
					CCLuaLog("fairy .."..id)
                end
            end
            

			PlayerListInfo[#PlayerListInfo + 1] = {
				userid = info.userid,
				reg_time = info.reg_time,
				channel_id = info.channel_id,
				rank = info.rank,
				name = info.name,
				type = info.type,
				lv = info.lv,
				status = info.ai_status,
                fairys = fairyList,
			}
		end
		if updatePvAIInfoFunc ~= 0 then
			CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updatePvAIInfoFunc)
			updatePvAIInfoFunc = 0
		end
		refreshPlayerList()
	end
end

--根据职业获取头像
function PvAIGetHeadIcon(character)
	
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

	return AspriteManager:getInstance():getOneFrame("UI/ui_character_select.bin", frameResTable[character])	 
end



--PvAILayer  类函数
--重置
function PvAILayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil
	self.buyLayerInstance = nil
	self.buyLayout = nil
	self.notificationFunc = 0
end


function PvAILayer:openPvAILayer()
	sendPvAIInfoReq()
	PushOneWaitingLayer("CSPVAIInfoReq")
	sendGetPvAIRewardMsg()

	if	self.LayerInstance == nil then
        CCLuaLog("PvAILayer.LayerInstance == nil")
		self:CreateLayer()
	end

	PushOneLayer(self.LayerInstance, "", "")

	--不断拉取玩家列表，直到拉取成功
	updatePvAIInfoFunc =  CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(sendPvAIInfoReq, 3, false)
end


--创建页面
function PvAILayer:CreateLayer()
	--self:ResetValue()
	self.LayerInstance = CCLayer:create()
	local tag = UITagTable["PvAILayer"].tag
	self.LayerInstance:setTag(tag)
	self.LayerInstance:registerDestructorScriptHandler(onReceiveLayerDestructor)

	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/PVAIUI.plist",self.LayerInstance, "PvAILayout", true)
	self.Layout = uiLayout	

	local mainMenuNode = GetMainMenuLayerNode()
	mainMenuNode:addChild(self.LayerInstance)
	
	self:InitLayer()	

	if self.buyLayerInstance == nil then
		self.buyLayerInstance = CCLayer:create()
		self.LayerInstance:addChild(self.buyLayerInstance, 100)
		self.buyLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/PvAIBuyTimeUI.plist",self.buyLayerInstance, "PvAIBuyTimeLayout", true)
		self:InitBuyLayer()
	end

end

local function getRemainBuyCount()
	local vipLevel = UserData:GetVipLevel()
	local maxBuyCount = Vip[vipLevel].Arena_Number_Purchase
	local nowBuyCount = PvAIManager:Get():getBuyCount()

	return maxBuyCount - nowBuyCount
end

--获得所需钻石数
local function getBuyDiamondCost(nowCount)
	CCLuaLog("-------- nowCount : "..nowCount)
	local exchangeId = 30710 + nowCount
	if ExchangeParameter[exchangeId] == nil then		
		exchangeId = 30710
	end
	local str = ExchangeParameter[exchangeId]["FromItems"]
	local beganPos = string.find(str,"/") + 1
	local endPos = string.len(str)
	local num = string.sub(str,beganPos,endPos)
	if num == nil then
		num = 0
	end

	return 0 + num
end

local function refreshBuyTimeLayer()
	if PvAILayer.buyLayout == nil then
		return
	end

	--所需钻石
	local nowBuyCount = PvAIManager:Get():getBuyCount()
	local diamondCost = getBuyDiamondCost(nowBuyCount)
	local diaLabel = PvAILayer.buyLayout:FindChildObjectByName("diaNum").__UILabel__
	diaLabel:setString(""..diamondCost)

	local remainCount = getRemainBuyCount()
	
	local remainLabel = PvAILayer.buyLayout:FindChildObjectByName("remainTxt").__UILabel__
	remainLabel:setString(GetLuaLocalization("M_BUYTIME_REMAINTIME")..remainCount)
	
end

--初始化页面
function PvAILayer:InitLayer()
	
	--启动倒计时
	local endTime = PvAIManager:Get():getCoolDownEndTime()
	local nowTime = TimeManager:Get():getCurServerTime()
	if endTime == 0 then
		endTime = nowTime
	end

	InitChallengeCoolDownTime(endTime - nowTime)

	--发送获取奖励消息
	sendGetPvAIRewardMsg()

    --注册回调函数
	--奖励消息回调函数
	addMsgCallBack("CSSelectPVAIACCURewardRsp", onMsgPvAIReward)
	addMsgCallBack("CSPVAIInfoRsq", onMsgPvAIInfoRsp)

	local function onClickBuyTimeBtn(tag)		
		self.buyLayerInstance:setVisible(true)
		refreshBuyTimeLayer()
	end

	local function onClickRepuShopBtn(tag)		
		CCLuaLog("onClickRepuShopBtn")
		showUILayerByTag(UITagTable["ReputationShopLayer"].tag,true)
		local userInfo = UserData:GetUserInfo()
		if userInfo.b_reputationExchangeFlag then
			local repuShopBtn = self.Layout:FindChildObjectByName("repuShopBtn").__UIButton__
			repuShopBtn:hideTipIcon()
			sendCSAttributesSetReq()
			userInfo.b_reputationExchangeFlag = false
			UserData:SetUserInfo(userInfo)
		end
	end


	local buyBtn = self.Layout:FindChildObjectByName("buyTimeBtn").__UIButton__:getMenuItemSprite()
	buyBtn:registerScriptTapHandler(onClickBuyTimeBtn)

	local repuShopBtn = self.Layout:FindChildObjectByName("repuShopBtn").__UIButton__
	repuShopBtn:getMenuItemSprite():registerScriptTapHandler(onClickRepuShopBtn)
	local userInfo = UserData:GetUserInfo()
	if userInfo.b_reputationExchangeFlag then
		repuShopBtn:showTipIcon()
	end
	--关闭按钮
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite()
	closeBtn:registerScriptTapHandler(closePvAILayer)
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	--初始化玩家头像
	local userHeadIconObj = self.Layout:FindChildObjectByName("iconCharacter")
	if userHeadIconObj ~= nil then
		local userHeadIcon = userHeadIconObj.__UIPicture__
		local userType = UserData:GetUserType()
		local userHeadIconRes = PvAIGetHeadIcon(userType)
		if userHeadIconRes ~= nil then
			userHeadIcon:setSprite(userHeadIconRes)
		end
	end
	SetChatBar(false,-1)
end

function PvAILayer:InitBuyLayer()
	self.buyLayerInstance:setVisible(false)

	local function onConfirmBuyTime(tag)
		local diamondCount = UserData:GetDiamond()
		local nowBuyCount = PvAIManager:Get():getBuyCount()
		local nowCost = getBuyDiamondCost(nowBuyCount)
		local remainCount = getRemainBuyCount()

		if diamondCount < nowCost then
			GameApi:showMessage(GetLuaLocalization("S_Gh_Diamond_Content"))
            PayHintLayer:CreateLayer()
			return
		elseif remainCount <= 0 then
			GameApi:showMessage(GetLuaLocalization("M_GARDEN_BUYTIME_MAX"))
			return
		end

		local function sendBuyTimeReq(nowBuyNum)
			CSExchangeParameterReq = {
				id = 30710 + nowBuyNum,
				}
			local msgname="CSExchangeParameterReq"
			local ret = send_message(msgname, CSExchangeParameterReq, true)
			return ret
		end

		
		sendBuyTimeReq(nowBuyCount)
		PvAILayer.buyLayerInstance:setVisible(false)
	end

	local function onCloseBuyLayer(tag)
		PvAILayer.buyLayerInstance:setVisible(false)
	end

	local confirmBtn = self.buyLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	confirmBtn:registerScriptTapHandler(onConfirmBuyTime)

	local cancelBtn = self.buyLayout:FindChildObjectByName("cancelBt").__UIButton__:getMenuItemSprite()
	cancelBtn:registerScriptTapHandler(onCloseBuyLayer)
end

--初始化挑战冷却时间
function InitChallengeCoolDownTime(seconds)
	local nowTime = TimeManager:Get():getCurServerTime()
	TimeManager:Get():registerLuaTimer(onUpdateCoolDownTime, TIMER_PVAI_COOLDOWN, nowTime + seconds)

	local remainStr = TimeManager:Get():secondsToString(seconds)

	local coolDownLabel = PvAILayer.Layout:FindChildObjectByName("pvaiCoolDownTime").__UILabel__
	if	coolDownLabel ~= nil then
		coolDownLabel:setString(remainStr)
	end
	return true
end