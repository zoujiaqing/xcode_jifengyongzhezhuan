require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/npc")
require("Script/GameConfig/ExchangeParameter")
require("Script/client_version")

UnderBoxLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	rewardLayerInstance = nil,
	rewardLayout = nil,

	notificationFunc = nil,
	UnderBoxNpcId = 0,

	RewardCoin = 0,
	RewardArmory = {},
}

local UnderBoxNumber = 8
local UnderBoxGetStatus = {0,0,0,0,0,0,0,0}
local UnderBoxOpenNum = 0
local UnderBoxResetNum = 0

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if UnderBoxLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(UnderBoxLayer.notificationFunc);
	end

	UnderBoxLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("UnderBoxUI")
	TXGUI.UIManager:sharedManager():removeUILayout("UnderBoxRewardUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= UnderBoxLayer.uiLayerInstance then
			if G_CurLayerInstance ~= UnderBoxLayer.uiLayerInstance then
				UnderBoxLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end



-- 发送开启宝箱请求
local function sendOpenBoxReq(id)
	CSGetUGCityBoxRewardReq = {
		index = id,
		}
	local msgname="CSGetUGCityBoxRewardReq"
	local ret = send_message(msgname, CSGetUGCityBoxRewardReq, true)
	return ret;
end

-- 发送重置宝箱请求
local function sendResetBoxReq(nowResetNum)
	CSExchangeParameterReq = {
		id = 20700 + nowResetNum,
		}
	local msgname="CSExchangeParameterReq"
	local ret = send_message(msgname, CSExchangeParameterReq, true)
	return ret;
end

-- 收到宝箱开启奖励
local function onMsgBoxReward(msg)
	if msg.succ then
		--GameApi:showMessage("Open Success")
		UnderBoxLayer.RewardCoin = 888
		UnderBoxLayer.RewardArmory = {}
		if msg.reward ~= nil then
			UnderBoxLayer.RewardCoin = msg.reward.add_coin
			if msg.reward.rewards ~= nil then
				local index = 1
				for k,v in pairs(msg.reward.rewards) do
					local itemId = v.item_id
					local itemLevel = v.item_level
					local itemNum = v.item_num
					UnderBoxLayer.RewardArmory[index] = {}
					UnderBoxLayer.RewardArmory[index]["id"] = v.item_id
					UnderBoxLayer.RewardArmory[index]["level"] = v.item_level
					UnderBoxLayer.RewardArmory[index]["num"] = v.item_num
					index = index + 1
					print("onMsgBoxReward : "..itemId.." "..itemNum)
				end
			end
			RefreshUnderBoxRewardLayer()
			UnderBoxLayer.rewardLayerInstance:setVisible(true)
		end

		RefreshUnderBoxLayer()
	else
		GameApi:showMessage("Open Failed")
	end
	return true
end

-- 收到宝箱重置消息
local function onMsgBoxReset(msg)
	CCLuaLog("onMsgBoxReset..."..msg.id)
	local ret = false
	if msg.id >= 20700 and msg.id <= 20704 then
		ret = true
		if msg.succ then
			GameApi:showMessage(GetLuaLocalization("M_UNDERGROUND_RESETSUCCESS"))
			RefreshUnderBoxLayer()
		end
	end
	return ret
end


--通过index获取pos
local function getPosByIndex(index)
	local _, boxIconPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_UGREWARD_ICON_BOX"..index)
	local _, boxTextPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_UGREWARD_TEXT_BOX"..index)
	local _, boxGetPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_UGREWARD_ICON_GOT"..index)
	local _, listAreaPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_UGREWARD_LIST_AREA")

	local retIconPos = ccpSub(boxIconPos, listAreaPos)
	local retTextPos = ccpSub(boxTextPos, listAreaPos)
	local retGetPos = ccpSub(boxGetPos, listAreaPos)

	return retIconPos, retTextPos, retGetPos
end

--获取某个宝箱的状态
local function getBoxType(index)
	local type = 0
	if UnderBoxOpenNum >= index then
		if UnderBoxGetStatus[index] == 0 then
			type = 1
		else
			type = 2
		end
	end
	return type
end

--点击宝箱
local function onPressBoxButton(tag)
	local type = getBoxType(tag)
	if type == 0 then 
		GameApi:showMessage(GetLuaLocalization("M_UNDERGROUND_ERROR1"))
	elseif type == 1 then
		sendOpenBoxReq(tag - 1)
	elseif type == 2 then
		GameApi:showMessage(GetLuaLocalization("M_UNDERGROUND_ALREADYGET"))
	end
end

--添加宝箱
local function addSingleBox(type, index)
	local page = UnderBoxLayer.uiLayout:FindChildObjectByName("underBoxScrollPage").__UIScrollPage__
	local pageCount = page:getPageCount()
	if pageCount == 0 then
		local layer = CCLayer:create()
		page:addPage(layer, true)
	end
	--暂时只用第一页，实现上下翻滚后扩充
	local pageLayer = page:getPageLayer(0)

	--宝箱文本
	local boxTextLabel = TXGUI.UILabelTTF:create(GetLuaLocalization("M_AUTOFIGHT_UNDERGROUND_SHEET"..index),KJLinXin,22)
	boxTextLabel:setColor(ccc3(255,210,0))


	--宝箱按钮
	local boxBtn = nil
	local getSprite = nil

	if type == 0 then --未解锁

		--宝箱按钮
		local boxLockSpriteOn = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_TREASUREBOX_LOCK")
		local boxLockSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_TREASUREBOX_LOCK")
		boxBtn = IconButton:new(boxLockSpriteOff, nil, boxLockSpriteOn)

		
	elseif type == 1 then --未领取

		--宝箱按钮
		local boxSpriteOn = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_TREASUREBOX_OPEN")
		local boxSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_TREASUREBOX_CLOSE")
		boxBtn = IconButton:new(boxSpriteOff, nil, boxSpriteOn)

	elseif type == 2 then --已领取

		--宝箱按钮
		local boxSpriteOn = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_TREASUREBOX_OPEN")
		local boxSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_TREASUREBOX_OPEN")
		boxBtn = IconButton:new(boxSpriteOff, nil, boxSpriteOn)

		--已领取图标
		getSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_UGREWARD_GOT")
		local _, _, getPos = getPosByIndex(index)
		getSprite:setPosition(getPos)
	end

	--注册函数
	local btnMenuItem = boxBtn:getMenuItem()
	btnMenuItem:setTag(index)
	btnMenuItem:registerScriptTapHandler(onPressBoxButton)	
	
	--设置位置
	local iconPos, textPos = getPosByIndex(index)
	boxBtn:setPosition(iconPos)
	boxTextLabel:setPosition(textPos)

	--添加结点	
	pageLayer:addChild(boxBtn)
	if getSprite~= nil then
		pageLayer:addChild(getSprite)
	end
	pageLayer:addChild(boxTextLabel)
	
end

-- 关闭宝箱页面按钮回调
local function onCloseBtClicked(tag)
	RemoveOneLayer(UnderBoxLayer.uiLayerInstance)
	UnderBoxLayer:destroyed()
	SetChatBar(true,-1)
end

-- 关闭奖励页面按钮回调
local function onCloseRewardBtClicked(tag)
	UnderBoxLayer.rewardLayerInstance:setVisible(false)
end

-- 确认重置宝箱
local function onConfirmResetBox(obj)
	sendResetBoxReq(UnderBoxResetNum)
end

--重置宝箱按钮
local function onResetBtClicked(tag)
	if UnderBoxOpenNum == 0 then
		GameApi:showMessage(GetLuaLocalization("M_UNDERGROUND_ERROR2"))
		return
	end

	local diamond = 0
	local exchangeId = 20700 + UnderBoxResetNum

	--重置次数已用完，已达最高vip等级
	if ExchangeParameter[exchangeId] == nil then
		GameApi:showMessage(GetLuaLocalization("M_ELITEINSTANCE_NORESET_TIME"))
		return
	end

	local str = ExchangeParameter[exchangeId]["FromItems"]
	local beganPos = string.find(str,"/") + 1
	local endPos = string.len(str)
	local num = string.sub(str,beganPos,endPos)
	if num == nil then
		num = 0
	end
	diamond = diamond + num

	DiamondWarningLayer:Show(GetLuaLocalization("M_UNDERGROUND_RESETNEED"), diamond, onConfirmResetBox, nil, ccc3(255,255,255),"","")
	
end


function UnderBoxLayer:CreateLayer()
	CCLuaLog("UnderBoxLayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["UnderBoxLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/UnderBoxUI.plist",self.uiLayerInstance, "UnderBoxUI", true)
			self:InitLayer()
		end
	end

	if self.rewardLayerInstance == nil then
		self.rewardLayerInstance = CCLayer:create()
		self.uiLayerInstance:addChild(self.rewardLayerInstance, 100)
		self.rewardLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/UnderBoxRewardUI.plist",self.rewardLayerInstance, "UnderBoxRewardUI", true)
		self:InitRewardLayer()
	end

	return self.uiLayerInstance
end

-- 重置参数
function UnderBoxLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.rewardLayerInstance = nil
	self.rewardLayout = nil
	self.notificationFunc = nil

	self.RewardCoin = 0
end

-- 删除UI
function UnderBoxLayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.rewardLayerInstance ~= nil then
		self.rewardLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
end

function UnderBoxLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseBtClicked);

	local resetBt = self.uiLayout:FindChildObjectByName("resetBt").__UIButton__:getMenuItemSprite();
	resetBt:registerScriptTapHandler(onResetBtClicked);

	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	PushOneLayer(self.uiLayerInstance,"","")
	SetChatBar(false,-1)

	addMsgCallBack("CSGetUGCityBoxRewardRsp", onMsgBoxReward)
	addMsgCallBack("CSExchangeParameterRsp", onMsgBoxReset)
end

function UnderBoxLayer:InitRewardLayer()
	local closeBt = self.rewardLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite();
	closeBt:registerScriptTapHandler(onCloseRewardBtClicked);

	UnderBoxLayer.rewardLayerInstance:setVisible(false)
end

--更新宝箱领取状态数据
function UpdateUnderBoxGetStatus(status)
	local result = status
	for i = 1, 8 do
		UnderBoxGetStatus[i] = result % 2
		result = math.modf( result / 2 )
		print("box status "..UnderBoxGetStatus[i])
	end
	return true
end

--更新宝箱重置次数
function UpdateUnderBoxResetNum(resetNum)
	UnderBoxResetNum = resetNum
	return true
end

--更新宝箱开启个数
function UpdateUnderBoxOpenNum(openNum)
	UnderBoxOpenNum = openNum
	return true
end

--刷新页面
function RefreshUnderBoxLayer()
	local npcImageName = "UI/"..npcs[UnderBoxLayer.UnderBoxNpcId]["BodyIcon"]..".png"
	local imagePath = GameResourceManager:sharedManager():storedFullPathFromRelativePath(npcImageName);
	local npcSprite = CCSprite:create(imagePath)
    local npcPic = UnderBoxLayer.uiLayout:FindChildObjectByName("npcPic").__UIPicture__

	local lastPt = CCPointZero
	local widthOffset = 0;
	local heightOffset = 0;


	if getClientVersion() == "hq" then
		npcSprite:setScale(1.667)
	end

	if getClientVersion() == "nq" then
		npcSprite:setScale(2.0)
	end

	local anchorPoint = npcPic:getCurrentNode():getAnchorPoint();
	widthOffset = widthOffset - anchorPoint.x * npcPic:getSpriteSize().width
	heightOffset = heightOffset - anchorPoint.y * npcPic:getSpriteSize().height
	lastPt = ccp(npcPic:getCurrentNode():getPosition())
	
	npcSprite:setAnchorPoint(ccp(0,0))
	npcPic:setMySprite(npcSprite)
	npcSprite:setPosition(ccp(lastPt.x + widthOffset,lastPt.y + heightOffset))

	-- chensy note
	-- npcSprit:setScale(2)

	local page = UnderBoxLayer.uiLayout:FindChildObjectByName("underBoxScrollPage").__UIScrollPage__
	page:removeAllPages()
	local layer = CCLayer:create()
	page:addPage(layer, true)

	for i = 1, UnderBoxNumber do
		local type = getBoxType(i)
		addSingleBox(type, i)
	end

	--更新描述文本
	local desLabel = UnderBoxLayer.uiLayout:FindChildObjectByName("descriptionText").__UILabel__
	desLabel:setString(GetLuaLocalization( "M_UNDERGROUND_TEXT_LAYER"..(UnderBoxOpenNum+1) ) )
end

--刷新奖励页面
function RefreshUnderBoxRewardLayer()
	local iconLabel = UnderBoxLayer.rewardLayout:FindChildObjectByName("coinNum").__UILabel__
	iconLabel:setString(UnderBoxLayer.RewardCoin)

	for index = 1, 3 do
		print("Reward "..index)
		local armoryPic = UnderBoxLayer.rewardLayout:FindChildObjectByName("armoryPic"..index).__UIPicture__
		local armoryLabel = UnderBoxLayer.rewardLayout:FindChildObjectByName("armoryNum"..index).__UILabel__
		if UnderBoxLayer.RewardArmory[index] ~= nil then			
			local iconSprite = ItemManager:Get():getIconSpriteById(UnderBoxLayer.RewardArmory[index]["id"])
			armoryPic:setSprite(iconSprite)
			
			armoryLabel:setString(" X "..UnderBoxLayer.RewardArmory[index]["num"])
			armoryPic:setVisible(true)
			armoryLabel:setVisible(true)
		else
			armoryPic:setVisible(false)
			armoryLabel:setVisible(false)
		end
	end
end

---重置地下城宝箱数据
function ResetUnderBoxData()
    UnderBoxNumber = 8
    UnderBoxGetStatus = {0,0,0,0,0,0,0,0}
    UnderBoxOpenNum = 0
    UnderBoxResetNum = 0
end