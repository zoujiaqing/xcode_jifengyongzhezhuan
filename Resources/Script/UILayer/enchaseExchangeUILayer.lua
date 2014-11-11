
EnchaseExchangeLayer = {
	enchaseExchangeUILayer = nil;

	greenGemBtn = nil;
	blueGemBtn = nil;
	purpleGemBtn = nil;
	yellowGemBtn = nil;
	orangeGemBtn = nil;
	exchangeBtn = nil;
	exchangeAllBtn = nil;
	useBtn = nil;

	--greenGemName = nil;
	greenGemNumber = nil;
	--blueGemName = nil;
	blueGemNumber = nil;
	--purpleGemName = nil;
	purpleGemNumber = nil;
	--yellowGemName = nil;
	yellowGemNumber = nil;
	--orangeGemName = nil;
	orangeGemNumber = nil;

	selectedItemId = 0;
	selectedBtn = nil;
	useGemFlag = true;
	equipPos = 0;
	holePos = 0;
}

function EnchaseExchangeLayer:create(parentNode)
	--local exchangeLayer = setmetatable(extension or {},self);
	--self.__index = self;
	self:init(parentNode)
	return self;
end

function EnchaseExchangeLayer:destory()
	if self.enchaseExchangeUILayer ~= nil then
		TXGUI.UIManager:sharedManager():removeUILayout("enchaseExchangeLayout")
		self.enchaseExchangeUILayer = nil
		self.selectedItemId = 0;
		self.selectedBtn = nil;
		self.useGemFlag = nil;
		self.equipPos = 0;
		self.holePos = 0;
	end
end

function EnchaseExchangeLayer:showGemsNumber()
	local greenNum = ItemManager:Get():getItemNumberById(417001)
	local blueNum = ItemManager:Get():getItemNumberById(417002)
	local purpleNum = ItemManager:Get():getItemNumberById(417003)
	local yellowNum = ItemManager:Get():getItemNumberById(417004)
	local orangeNum = ItemManager:Get():getItemNumberById(417005)

	self.greenGemNumber:setString("x " .. greenNum)
	self.blueGemNumber:setString("x " .. blueNum)
	self.purpleGemNumber:setString("x " .. purpleNum)
	self.yellowGemNumber:setString("x " .. yellowNum)
	self.orangeGemNumber:setString("x " .. orangeNum)
end


local function onCloseClicked(tag)
	EnchaseExchangeLayer:hide()
	--EnchaseExchangeLayer.enchaseExchangeUILayer:setVisible(false)
end

local function onGreenGemClicked(obj)
	if EnchaseExchangeLayer.selectedBtn ~= nil then
		EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():unselected()
	end
	EnchaseExchangeLayer.selectedBtn = EnchaseExchangeLayer.greenGemBtn
	EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():selected()
	EnchaseExchangeLayer.selectedItemId = 417001
end

local function onBlueGemClicked(obj)
	if EnchaseExchangeLayer.selectedBtn ~= nil then
		EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():unselected()
	end
	EnchaseExchangeLayer.selectedBtn = EnchaseExchangeLayer.blueGemBtn
	EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():selected()
	EnchaseExchangeLayer.selectedItemId = 417002
end

local function onPurpleGemClicked(obj)
	if EnchaseExchangeLayer.selectedBtn ~= nil then
		EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():unselected()
	end
	EnchaseExchangeLayer.selectedBtn = EnchaseExchangeLayer.purpleGemBtn
	EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():selected()
	EnchaseExchangeLayer.selectedItemId = 417003
end

local function onYellowGemClicked(obj)
	if EnchaseExchangeLayer.selectedBtn ~= nil then
		EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():unselected()
	end
	EnchaseExchangeLayer.selectedBtn = EnchaseExchangeLayer.yellowGemBtn
	EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():selected()
	EnchaseExchangeLayer.selectedItemId = 417004
end

local function onOrangeGemClicked(obj)
	if EnchaseExchangeLayer.selectedBtn ~= nil then
		EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():unselected()
	end
	EnchaseExchangeLayer.selectedBtn = EnchaseExchangeLayer.orangeGemBtn
	EnchaseExchangeLayer.selectedBtn:getMenuItemSprite():selected()
	EnchaseExchangeLayer.selectedItemId = 417005
end

function onReceivedExchangeRsp(error)
	if error == 0 then
		--GameApi:showMessage("兑换成功");
		EnchaseExchangeLayer:showGemsNumber();
	else
		GameApi:showMessage("兑换失败");
	end
end

local function onUseGemStone(obj)
	if EnchaseExchangeLayer.selectedItemId ~= 0 then
		GemStonesFromLua:getInstance():EquipInlayGemReq(EnchaseExchangeLayer.equipPos,EnchaseExchangeLayer.holePos,EnchaseExchangeLayer.selectedItemId)
	else
		GameApi:showMessage("请选择一个纹章")
	end
end

local function onSingleGemExchageBtClicked(obj)
	if EnchaseExchangeLayer.selectedItemId ~= 0 then
		GemStonesFromLua:getInstance():GemAnimaExchangeReq(EnchaseExchangeLayer.selectedItemId,1)
	else
		GameApi:showMessage("请选择一个纹章")
	end
end

local function onAllGemExchageBtClicked(obj)
	if EnchaseExchangeLayer.selectedItemId ~= 0 then
		local gemNum = ItemManager:Get():getItemNumberById(EnchaseExchangeLayer.selectedItemId)
		if gemNum == 0 then
			GameApi:showMessage("物品不足");
		else
			GemStonesFromLua:getInstance():GemAnimaExchangeReq(EnchaseExchangeLayer.selectedItemId,gemNum)
		end
	else
		GameApi:showMessage("请选择一种纹章")
	end
end

function EnchaseExchangeLayer:setDefaultSelectGem()
	local index = 0;
	for i = 417001 , 417005 do
		local num = ItemManager:Get():getItemNumberById(i)
		if num > 0 then
			index = i
			break
		end
	end

	if index == 417001 then
		onGreenGemClicked(nil)
	elseif index == 417002 then
		onBlueGemClicked(nil)
	elseif index == 417003 then
		onPurpleGemClicked(nil)
	elseif index == 417004 then
		onYellowGemClicked(nil)
	elseif index == 417005 then
		onOrangeGemClicked(nil)
	else
		if self.selectedBtn ~= nil then
			self.selectedBtn:getMenuItemSprite():unselected()
		end
		self.selectedBtn = nil
		self.selectedItemId = 0
	end
end

function EnchaseExchangeLayer:show(isUse,equip,hole)
	self.enchaseExchangeUILayer:setVisible(true)
	self.useGemFlag = isUse
	self.equipPos = equip
	self.holePos = hole
	self:showGemsNumber()
	self.exchangeBtn:setVisible(not isUse)
	self.exchangeAllBtn:setVisible(not isUse)
	self.useBtn:setVisible(isUse)
	self:setDefaultSelectGem()
end

function EnchaseExchangeLayer:hide()
	self.enchaseExchangeUILayer:setVisible(false)
end

function EnchaseExchangeLayer:init(parentNode)
	self.enchaseExchangeUILayer = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/enchaseExchangeUI.plist",parentNode, "enchaseExchangeLayout", false)
	self.greenGemBtn =	self.enchaseExchangeUILayer:FindChildObjectByName("armoryGreenBt").__UIButton__
	self.blueGemBtn = self.enchaseExchangeUILayer:FindChildObjectByName("armoryBlueBt").__UIButton__
	self.purpleGemBtn =	self.enchaseExchangeUILayer:FindChildObjectByName("armoryPinkBt").__UIButton__
	self.yellowGemBtn = self.enchaseExchangeUILayer:FindChildObjectByName("armoryYellowBt").__UIButton__
	self.orangeGemBtn = self.enchaseExchangeUILayer:FindChildObjectByName("armoryOrangeBt").__UIButton__
	self.exchangeBtn = self.enchaseExchangeUILayer:FindChildObjectByName("exchangeBt").__UIButton__
	self.exchangeAllBtn = self.enchaseExchangeUILayer:FindChildObjectByName("exchangeAllBt").__UIButton__
	self.useBtn = self.enchaseExchangeUILayer:FindChildObjectByName("enchaseBt").__UIButton__

	self.greenGemBtn:getMenuItemSprite():registerScriptTapHandler(onGreenGemClicked)
	self.blueGemBtn:getMenuItemSprite():registerScriptTapHandler(onBlueGemClicked)
	self.purpleGemBtn:getMenuItemSprite():registerScriptTapHandler(onPurpleGemClicked)
	self.yellowGemBtn:getMenuItemSprite():registerScriptTapHandler(onYellowGemClicked)
	self.orangeGemBtn:getMenuItemSprite():registerScriptTapHandler(onOrangeGemClicked)

	self.useBtn:getMenuItemSprite():registerScriptTapHandler(onUseGemStone)
	self.exchangeBtn:getMenuItemSprite():registerScriptTapHandler(onSingleGemExchageBtClicked)
	self.exchangeAllBtn:getMenuItemSprite():registerScriptTapHandler(onAllGemExchageBtClicked)

	local closeBtn = self.enchaseExchangeUILayer:FindChildObjectByName("exchangeCloseBt").__UIButton__
	closeBtn:getMenuItemSprite():registerScriptTapHandler(onCloseClicked)

	self.greenGemNumber =	self.enchaseExchangeUILayer:FindChildObjectByName("greenNumberLable").__UILabel__
	self.blueGemNumber = self.enchaseExchangeUILayer:FindChildObjectByName("blueNumberLable").__UILabel__
	self.purpleGemNumber =	self.enchaseExchangeUILayer:FindChildObjectByName("purpleNumberLable").__UILabel__
	self.yellowGemNumber = self.enchaseExchangeUILayer:FindChildObjectByName("yellowNumberLable").__UILabel__
	self.orangeGemNumber = self.enchaseExchangeUILayer:FindChildObjectByName("orangeNumberLable").__UILabel__

	self:hide()
end