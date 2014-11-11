require("Script/Language")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/DailySalary/dailyRewardSalaryNetWorkHandler")

DailyRewardSalaryLayer = {
	LayerInstance = nil,
	Layout = nil,
	AcceptRewardBtn = nil,
	LayerTitleLabel = nil,
	AcceptRewardBtnLabel = nil,
	DescriptionLabel = nil,
	CoinLabel = nil,
	IsShowMainSalaryLayer = false,
}

function DailyRewardSalaryLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil

	self.AcceptRewardBtn = nil
	self.LayerTitleLabel = nil
	self.AcceptRewardBtnLabel = nil
	self.DescriptionLabel = nil

	self.CoinLabel = nil
	self.IsShowMainSalaryLayer = false
end

local function onReceiveDestructorHandler()	
	TXGUI.UIManager:sharedManager():removeUILayout("DailySalary")
	DailyRewardSalaryLayer:ResetValue()
end

--Note: 创建Layer
function DailyRewardSalaryLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
    
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/DailySalary.plist",self.LayerInstance, "DailySalary", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()        
        local tag = UITagTable["DailySalaryLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,1,tag)       
	end	

	self:InitLayer()

	local coinNum = UserData:GetUserInfo().pay_money;
	self:ShowPanelWithData(coinNum)

	if self.IsShowMainSalaryLayer == false then
		PushOneLayer(self.LayerInstance,"","")	
	end

	return self.LayerInstance
end

function DailyRewardSalaryLayer:ShowPanelWithData(coinNum)
	self.AcceptRewardBtn:setVisible(true)	
	self.AcceptRewardBtnLabel:setVisible(true)

	local coinContent = MainMenuLayer:GetNumByFormat(coinNum)
	if nil ~= self.CoinLabel then
		self.CoinLabel:setString("" .. coinContent)
	end
end

function DailyRewardSalaryLayer:CloseLayer()
	DailyRewardSalaryLayerCloseBtnClick(nil)
end

function DailyRewardSalaryLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(DailyRewardSalaryLayerCloseBtnClick);

	self.AcceptRewardBtn = self.Layout:FindChildObjectByName("rewardBtn").__UIButton__
	local acceptRewardBtnMenuItem = self.AcceptRewardBtn:getMenuItemSprite();
	acceptRewardBtnMenuItem:registerScriptTapHandler(DailyRewardSalaryAcceptRewardBtnClick);
	self.AcceptRewardBtn:setVisible(false)
	self.AcceptRewardBtnLabel = self.Layout:FindChildObjectByName("rewardBtnLabel").__UILabel__
	self.AcceptRewardBtnLabel:setVisible(false)
	if nil ~= self.AcceptRewardBtnLabel then
		local languageFlag = 'Dailytask_WaitReward'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.AcceptRewardBtnLabel:setString(content)
	end
	local bgIconPath = "UI/ui_pic_dailysalary.png"
	local herotreasure = CCSprite:create(bgIconPath)
	if herotreasure ~= nil then
		self.Layout:FindChildObjectByName("pic").__UIPicture__:setSprite(herotreasure)
	end
	--Note: 显示Title
	self.LayerTitleLabel = self.Layout:FindChildObjectByName("titleLabel").__UILabel__
	if nil ~= self.LayerTitleLabel then
		local languageFlag = 'Dailyreceive_6000_name'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.LayerTitleLabel:setString(content)
	end

	self.DescriptionLabel = self.Layout:FindChildObjectByName("descriptionLabel").__UILabel__
	if nil ~= self.DescriptionLabel then
		local languageFlag = 'M_DAILYSALARY_DESCRIPTION'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.DescriptionLabel:setString(content)
	end

	self.CoinLabel = self.Layout:FindChildObjectByName("coinNumLabel").__UILabel__
	if nil ~= self.CoinLabel then
		self.CoinLabel:setString("")
	end
end

-------------------事件回调------------------------------------------------
local function closeLayer()
	if nil ~= DailyRewardSalaryLayer.LayerInstance then
		if false == DailyRewardSalaryLayer.IsShowMainSalaryLayer then
			RemoveOneLayer(DailyRewardSalaryLayer.LayerInstance)
		end
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyRewardSalaryLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyRewardSalaryLayer:ResetValue()
		print("DailyRewardSalaryLayer")
		XLogicManager:sharedManager():notifyShowNextAppearUI();
	end
		
end

function DailyRewardSalaryLayerCloseBtnClick(pObj)
	if DailyRewardSalaryLayer.IsShowMainSalaryLayer then
		if nil ~= DailyRewardMainLayer then
			DailyRewardMainLayer:ShowOrNot(true)
		end
	end
	closeLayer()
end

function DailyRewardSalaryAcceptRewardBtnClick(pObj)
	DailyRewardSalayNetWorkHandler:SendGetDailyRewardReqMessage()
end
---------------------------------------------------------------------------
local function closeCallBack(message)
	if nil ~= DailyRewardSalaryLayer.LayerInstance then
		if G_CurLayerInstance ~= DailyRewardSalaryLayer.LayerInstance then
			closeLayer()
		end
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		DailyRewardSalaryLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
