
EnchaseRecastContainer = {
	containerInstance = nil;
	diamondBtnToggle = nil;
	creamBtnToggle = nil;
	diamondSpendLabel = nil;
	creamSpendLabel = nil;
	recastBtn = nil;

	spendFlag = 0; -- 花费类型 0 免费 1 钻石 2 碎片
}

function EnchaseRecastContainer:create(uiLayout)
	self:init(uiLayout)
	return self;
end

function EnchaseRecastContainer:destory()
	self.containerInstance = nil;
	self.diamondBtnToggle = nil;
	self.creamBtnToggle = nil;
	self.diamondSpendLabel = nil;
	self.creamSpendLabel = nil;
	self.recastBtn = nil;
	self.spendFlag = 0;
end

local function onDiamondToggleClicked(obj)
	CCLuaLog("onDiamondToggleClicked")
	if EnchaseRecastContainer.creamBtnToggle ~= nil then
		EnchaseRecastContainer.creamBtnToggle:unselected()
	end

	if EnchaseRecastContainer.diamondBtnToggle ~= nil then
		EnchaseRecastContainer.diamondBtnToggle:selected()
	end

	EnchaseRecastContainer.spendFlag = 416002
end

local function onCreamToggleClicked(obj)
	CCLuaLog("onCreamToggleClicked")
	if EnchaseRecastContainer.creamBtnToggle ~= nil then
		EnchaseRecastContainer.creamBtnToggle:selected()
	end

	if EnchaseRecastContainer.diamondBtnToggle ~= nil then
		EnchaseRecastContainer.diamondBtnToggle:unselected()
	end

	EnchaseRecastContainer.spendFlag = 416005
end

function EnchaseRecastContainer:setFreeCastTime(num)
	CCLuaLog("EnchaseRecastContainer:setFreeCastTime(" .. num .. ")")
	self.spendFlag = 0
	self.containerInstance:setVisible(false)
	self.freeRecastLabel:setVisible(true)
	self.freeRecastLabel:setString("剩余重铸次数 " .. num)
end

function EnchaseRecastContainer:setRecastCost(diamond,cream)
	CCLuaLog("EnchaseRecastContainer:setRecastCost(" .. diamond .. ")")
	self.containerInstance:setVisible(true)
	self.freeRecastLabel:setVisible(false)
	self.diamondSpendLabel:setString(diamond)
	self.creamSpendLabel:setString(cream)
	if self.spendFlag == 0 then
		onDiamondToggleClicked(nil)
	end
end

function EnchaseRecastContainer:init(uiLayout)
	self.containerInstance = uiLayout:FindChildObjectByName("enchaseRecastContainer").__UIContainer__;
	self.diamondBtnToggle = uiLayout:FindChildObjectByName("diamondSpentButtonToggle").__UIButtonToggle__;
	self.creamBtnToggle = uiLayout:FindChildObjectByName("creamSpentButtonToggle").__UIButtonToggle__;
	self.diamondSpendLabel = uiLayout:FindChildObjectByName("diamondSpentNumLabel").__UILabel__;
	self.creamSpendLabel = uiLayout:FindChildObjectByName("creamSpentNumLabel").__UILabel__;
	self.recastBtn = uiLayout:FindChildObjectByName("recastBt").__UIButton__;
	self.freeRecastLabel = uiLayout:FindChildObjectByName("freeRecastLabel").__UILabel__;

	local diamonToggle = self.diamondBtnToggle:getMenuItemToggle()
	if diamonToggle ~= nil then
		diamonToggle:registerScriptTapHandler(onDiamondToggleClicked);
	end

	local creamToggle = self.creamBtnToggle:getMenuItemToggle()
	if creamToggle ~= nil then
		creamToggle:registerScriptTapHandler(onCreamToggleClicked);
	end

	self:setFreeCastTime(3)
end

