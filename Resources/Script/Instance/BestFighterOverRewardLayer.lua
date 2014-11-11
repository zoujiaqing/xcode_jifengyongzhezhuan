require("Script/GameConfig/uiTagDefine")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/Fairy/FairyLuaInterface")

BestFighterOverRewardLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

	isWin = false,

	commonCoin = 0,
	commonPoint = 0,

	winCoin = 0,
	winPoint = 0,

	rewardItems = {},
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if BestFighterOverRewardLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(BestFighterOverRewardLayer.notificationFunc);
	end

	BestFighterOverRewardLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("BestFighterOverRewardUI")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= BestFighterOverRewardLayer.uiLayerInstance then
			if G_CurLayerInstance ~= BestFighterOverRewardLayer.uiLayerInstance then
				BestFighterOverRewardLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function BestFighterOverRewardLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["BestFighterOverRewardLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/BestFighterOverRewardUI.plist",self.uiLayerInstance, "BestFighterOverRewardUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function BestFighterOverRewardLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil

	self.isWin = false

	self.commonCoin = 0
	self.commonPoint = 0

	self.winCoin = 0
	self.winPoint = 0

	self.rewardItems = {}
end

-- 删除UI
function BestFighterOverRewardLayer:destroyed()
	CCLuaLog("--- BestFighterOverRewardLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
end

local function onPressBackBtn(tag)
	RemoveOneLayer(BestFighterOverRewardLayer.uiLayerInstance)
	BestFighterOverRewardLayer:destroyed()
	XLogicManager:sharedManager():LeaveBattle()
end

local tipFairyId = 0
local function onOperationTipCallBackLeft(tag)

end

local function onOperationTipCallBackRight(tag)
	if tipFairyId ~= 0 then
		FairyInfoLayer.fairyId = tipFairyId
		FairyInfoLayer.buttonState = 0
		FairyInfoLayer.infoState = 2
		FairyInfoLayer:CreateLayer(nil)
	end
end

local function onPressItemBtn(tag)
	CCLuaLog("onPressItemBtn .. "..tag)
	local itemId = BestFighterOverRewardLayer.rewardItems[tag].id

	local menu = BestFighterOverRewardLayer.uiLayout:FindChildObjectByName("itemBtn"..tag).__UIButton__:getMenuNode()
    local pos = menu:getTouchPoint()

	ItemManager:Get():showItemTipsById(itemId, pos)

	local isFairy = G_GetIsFairyById(itemId)
	local info = ItemManager:Get():getItemInfoById(itemId)
	if isFairy or info.m_type == 21 then
		tipFairyId = itemId % 10000;
		local detailText = LanguageLocalization:GetLocalization("M_EQUIPMENT_INFORMATION")
		ItemManager:Get():setTipLuaHander(onOperationTipCallBackLeft,"",false,onOperationTipCallBackRight,detailText,true)
	end
end

function BestFighterOverRewardLayer:InitLayer()
	CCLuaLog("--- BestFighterOverRewardLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	local backBtnMenu = self.uiLayout:FindChildObjectByName("backBtn").__UIButton__:getMenuItemSprite()
	backBtnMenu:registerScriptTapHandler(onPressBackBtn)

	--根据结果显示隐藏相关图片
	self.uiLayout:FindChildObjectByName("winPic").__UIPicture__:setVisible(self.isWin)
	self.uiLayout:FindChildObjectByName("failPic").__UIPicture__:setVisible(not self.isWin)

	--根据字段数值来确定是否显示连胜
	self.uiLayout:FindChildObjectByName("winCoinIcon").__UIPicture__:setVisible(self.winCoin ~= 0)
	self.uiLayout:FindChildObjectByName("winPointIcon").__UIPicture__:setVisible(self.winCoin ~= 0)

	self.uiLayout:FindChildObjectByName("winRewardText").__UILabel__:setVisible(self.winCoin ~= 0)
	self.uiLayout:FindChildObjectByName("itemRewardText").__UILabel__:setVisible(#self.rewardItems ~= 0)

	self.uiLayout:FindChildObjectByName("winCoinNum").__UILabel__:setVisible(self.winCoin ~= 0)
	self.uiLayout:FindChildObjectByName("winPointNum").__UILabel__:setVisible(self.winCoin ~= 0)

	--设置奖励数字
	self.uiLayout:FindChildObjectByName("commonCoinNum").__UILabel__:setString(""..MainMenuLayer:GetNumByFormat(self.commonCoin))
	self.uiLayout:FindChildObjectByName("commonPointNum").__UILabel__:setString(""..self.commonPoint)

	if self.isWin then
		self.uiLayout:FindChildObjectByName("winCoinNum").__UILabel__:setString(""..MainMenuLayer:GetNumByFormat(self.winCoin))
		self.uiLayout:FindChildObjectByName("winPointNum").__UILabel__:setString(""..self.winPoint)
	end

	-- 物品奖励文本显示
	if #self.rewardItems == 0 then
		self.uiLayout:FindChildObjectByName("winRewardText").__UILabel__:setVisible(self.winCoin ~= 0)
	end

	-- 物品按钮响应函数
	for i = 1, 5 do
		local itemBtnMenu = self.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__:getMenuItemSprite()
		itemBtnMenu:setTag(i)
		itemBtnMenu:registerScriptTapHandler(onPressItemBtn)
	end

	self:RefreshItems()

	PushOneLayer(self.uiLayerInstance,"","")

	OnBestFighterEndHideLayer()
end

function BestFighterOverRewardLayer:RefreshItems()
	CCLuaLog("--- BestFighterOverRewardLayer:RefreshItems() ---")
	for i = 1, 5 do
		if self.rewardItems[i] ~= nil then
			local itemIcon = self.uiLayout:FindChildObjectByName("itemIcon"..i).__UIPicture__
			local itemRes = ItemManager:Get():getIconSpriteById(self.rewardItems[i].id)
			itemIcon:setSprite(itemRes)

			local label = TXGUI.UILabelTTF:create(self.rewardItems[i].num, KJLinXin, 20)
			label:setAnchorPoint(ccp(1,0))
			local x = itemRes:getContentSize().width
			label:setPosition(ccp(x,0))

			itemIcon:getCurrentNode():addChild(label)
		else
			self.uiLayout:FindChildObjectByName("itemBtn"..i).__UIButton__:setVisible(false)
		end
	end
end