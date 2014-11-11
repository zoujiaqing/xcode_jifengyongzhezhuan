require("Script/GameConfig/uiTagDefine")
--require("Script/GameConfig/month_card")

MonthCardRewardLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	notificationFunc = 0,
	card_id = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RemoveOneLayer(MonthCardRewardLayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("MonthCardRewardLayout")
	if MonthCardRewardLayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(MonthCardRewardLayer.notificationFunc)
	end
	MonthCardRewardLayer:resetValue()
end

function MonthCardRewardLayer:createLayer()
	if self.uiLayerInstance == nil then 
		--self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["MonthCardRewardLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/MonthCardRewardUI.plist",self.uiLayerInstance, "MonthCardRewardLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function MonthCardRewardLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.card_id = 0
end

local function onCloseButtonClicked(tag)
	if MonthCardRewardLayer.uiLayerInstance ~= nil then
		MonthCardRewardLayer.uiLayerInstance:removeFromParentAndCleanup(true)
		SetChatBar(true,-1)
		GameAudioManager:sharedManager():playEffect(350001,false)
		XLogicManager:sharedManager():notifyShowNextAppearUI();
	end
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= MonthCardRewardLayer.uiLayerInstance then
			if G_CurLayerInstance ~= MonthCardRewardLayer.uiLayerInstance then
				if MonthCardRewardLayer.uiLayerInstance ~= nil then
					MonthCardRewardLayer.uiLayerInstance:removeFromParentAndCleanup(true)
				end
				ret = 1
			end
		end
	elseif message == GM_MONTHCARD_GET_SUCCESS then
		if nil ~= MonthCardRewardLayer.uiLayerInstance then
			local hasCard = MonthCardManager:Get():hasMonthCard()
			if hasCard then
				MonthCardRewardLayer:resetMonthCardInfo()
				MonthCardRewardLayer:resetAction()
			else
				onCloseButtonClicked(nil)
			end
			ret = 1
		end
	end

	return ret
end

local function sendMcardRewardReq(cardId)
	CSMcardRewardReq = {
		card_id = cardId,
	}
	local msgname = "CSMcardRewardReq"
	local ret = send_message(msgname, CSMcardRewardReq, true)
	return ret
end

local function onClickGetRewardButton(tag)
	if MonthCardRewardLayer.uiLayerInstance ~= nil then
		sendMcardRewardReq(MonthCardRewardLayer.card_id)
	end
end

local function onClickBuyCardButton(tag)
	if MonthCardRewardLayer.uiLayerInstance ~= nil then
		showUILayerByTag(UITagTable["ChargeLayer"].tag,true)
	end
	onCloseButtonClicked(nil)
end

function MonthCardRewardLayer:resetAction()
	if MonthCardRewardLayer.uiLayerInstance ~= nil then
		local action = CCScaleTo:create(0.3, 1.0)
		MonthCardRewardLayer.uiLayerInstance:setScale(0.1)
		MonthCardRewardLayer.uiLayerInstance:runAction(action)
	end
end

function MonthCardRewardLayer:resetMonthCardInfo()
	local cardInfo = MonthCardManager:Get():getOneMonthCard()
	if cardInfo ~= nil and cardInfo.card_id ~= 0 then
		self.card_id = cardInfo.card_id
		if cardInfo.is_alarm then
			local getRewardContainer = self.uiLayout:FindChildObjectByName("getRewardContainer").__UIContainer__
			getRewardContainer:setVisible(false)
			local allTextLabel = self.uiLayout:FindChildObjectByName("allText").__UILabel__
			allTextLabel:setVisible(true)
			allTextLabel:setString(LanguageLocalization:GetLocalization("M_MONTHCARD_NONE"))
			local getRewardLabel = self.uiLayout:FindChildObjectByName("getRewardLabel").__UILabel__
			getRewardLabel:setVisible(false)
			local buyCardLabel = self.uiLayout:FindChildObjectByName("buyCardLabel").__UILabel__
			buyCardLabel:setVisible(true)
			local getRewardBt = self.uiLayout:FindChildObjectByName("getRewardBt").__UIButton__
			getRewardBt:setVisible(false)
			local buyCardBt = self.uiLayout:FindChildObjectByName("buyCardBt").__UIButton__
			buyCardBt:setVisible(true)
		elseif cardInfo.day_count == 0 then
			local getRewardContainer = self.uiLayout:FindChildObjectByName("getRewardContainer").__UIContainer__
			getRewardContainer:setVisible(false)
			local allTextLabel = self.uiLayout:FindChildObjectByName("allText").__UILabel__
			allTextLabel:setVisible(true)
			allTextLabel:setString(string.format(LanguageLocalization:GetLocalization("M_MONTHCARD_TIPS4"),cardInfo.diamond_amount))
			local getRewardLabel = self.uiLayout:FindChildObjectByName("getRewardLabel").__UILabel__
			getRewardLabel:setVisible(true)
			local buyCardLabel = self.uiLayout:FindChildObjectByName("buyCardLabel").__UILabel__
			buyCardLabel:setVisible(false)
			local getRewardBt = self.uiLayout:FindChildObjectByName("getRewardBt").__UIButton__
			getRewardBt:setVisible(true)
			local buyCardBt = self.uiLayout:FindChildObjectByName("buyCardBt").__UIButton__
			buyCardBt:setVisible(false)
		else
			local getRewardContainer = self.uiLayout:FindChildObjectByName("getRewardContainer").__UIContainer__
			getRewardContainer:setVisible(true)
			local allTextLabel = self.uiLayout:FindChildObjectByName("allText").__UILabel__
			allTextLabel:setVisible(false)
			local getRewardLabel = self.uiLayout:FindChildObjectByName("getRewardLabel").__UILabel__
			getRewardLabel:setVisible(true)
			local buyCardLabel = self.uiLayout:FindChildObjectByName("buyCardLabel").__UILabel__
			buyCardLabel:setVisible(false)
			local getRewardBt = self.uiLayout:FindChildObjectByName("getRewardBt").__UIButton__
			getRewardBt:setVisible(true)
			local buyCardBt = self.uiLayout:FindChildObjectByName("buyCardBt").__UIButton__
			buyCardBt:setVisible(false)

			local text1 = self.uiLayout:FindChildObjectByName("text1").__UILabel__
			text1:setString(string.format(LanguageLocalization:GetLocalization("M_MONTHCARD_TIPS1"),cardInfo.diamond_amount))
			local text2 = self.uiLayout:FindChildObjectByName("text2").__UILabel__
			text2:setString(string.format("%02d",cardInfo.day_count))
		end
	end
end

function MonthCardRewardLayer:InitMonthCard()
	local hasCard = MonthCardManager:Get():hasMonthCard()
	if hasCard then
		self:resetMonthCardInfo()
	end
end

function MonthCardRewardLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local getRewardBt = self.uiLayout:FindChildObjectByName("getRewardBt").__UIButton__
	getRewardBt:getMenuItemSprite():registerScriptTapHandler(onClickGetRewardButton)
	
	local buyCardBt = self.uiLayout:FindChildObjectByName("buyCardBt").__UIButton__
	buyCardBt:getMenuItemSprite():registerScriptTapHandler(onClickBuyCardButton)
	
	local pic = self.uiLayout:FindChildObjectByName("diamondBG").__UIPicture__
	local sprite = CCSprite:create("UI/ui_pic_monthcard.png")
	pic:setSprite(sprite,false)
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	self:InitMonthCard()
	--SetChatBar(false,-1)
	PushOneLayer(self.uiLayerInstance,"","")
end