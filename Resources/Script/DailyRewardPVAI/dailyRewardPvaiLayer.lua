require("Script/Language")
require("Script/CommonDefine")
require("Script/DailyRewardPVAI/dailyRewardPvaiNetWorkHandler")

DailyRewardPvaiLayer = {
	LayerInstance = nil,
	Layout = nil,
	AcceptRewardBtn = nil,
	LayerTitleLabel = nil,
	AcceptRewardBtnLabel = nil,
	DescriptionLabel = nil,
	CoinLabel = nil,
	ReputationLabel = nil,
}

function DailyRewardPvaiLayer:ResetValue()
	self.LayerInstance = nil
	self.Layout = nil

	self.AcceptRewardBtn = nil
	self.LayerTitleLabel = nil
	self.AcceptRewardBtnLabel = nil
	self.DescriptionLabel = nil

	self.CoinLabel = nil
	self.ReputationLabel = nil
end

local function onReceiveDestructorHandler()	
	TXGUI.UIManager:sharedManager():removeUILayout("dailyRewardPVAIPanel")
	DailyRewardPvaiLayer:ResetValue()
end

--Note: 创建Layer
function DailyRewardPvaiLayer:CreateLayer(parentLayer)
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
	
	local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/dailyRewardPVAI.plist",self.LayerInstance, "dailyRewardPVAIPanel", true);
	self.Layout = uiLayout
	
	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
        
        local tag = UITagTable["DailyRewardPvaiLayer"].tag
		mainMenuNode:addChild(self.LayerInstance,1,tag)
        
		--InitMoneyConfigLayer(self.LayerInstance)
	end	

	self:InitLayer()

	DailyRewardPvaiNetWorkHandler:ResetValue();
	DailyRewardPvaiNetWorkHandler:SendSelectPVAIRewardReqMessage();

	return self.LayerInstance
end

function DailyRewardPvaiLayer:ShowPanelWithData(coinNum,Reputation)
	if self.LayerInstance == nil then
		print("DailyRewardPvaiLayer is not inited ignore msg")
		return false
	end

	self.AcceptRewardBtn:setVisible(true)	
	self.AcceptRewardBtnLabel:setVisible(true)
	
	local coinContent = MainMenuLayer:GetNumByFormat(coinNum)
	local reputationContent = MainMenuLayer:GetNumByFormat(Reputation)
	if nil ~= self.CoinLabel then
		self.CoinLabel:setString("" .. coinContent)
	end
	if nil ~= self.ReputationLabel then
		self.ReputationLabel:setString("" .. reputationContent)
	end
	return true
end

function DailyRewardPvaiLayer:CloseLayer()
	DailyRewardPvaiLayerCloseBtnClick(nil)
end

function DailyRewardPvaiLayer:InitLayer()
	local closeBtn = self.Layout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
	closeBtn:registerScriptTapHandler(DailyRewardPvaiLayerCloseBtnClick);

	self.AcceptRewardBtn = self.Layout:FindChildObjectByName("rewardBtn").__UIButton__
	local acceptRewardBtnMenuItem = self.AcceptRewardBtn:getMenuItemSprite();
	acceptRewardBtnMenuItem:registerScriptTapHandler(DailyRewardPvaiAcceptRewardBtnClick);
	self.AcceptRewardBtn:setVisible(false)
	self.AcceptRewardBtnLabel = self.Layout:FindChildObjectByName("rewardBtnLabel").__UILabel__
	self.AcceptRewardBtnLabel:setVisible(false)
		local bgIconPath = "UI/ui_pic_pvaireward.png"
	local herotreasure = CCSprite:create(bgIconPath)
	if herotreasure ~= nil then
		self.Layout:FindChildObjectByName("pic").__UIPicture__:setSprite(herotreasure)
	end
	--Note: 显示Title
	self.LayerTitleLabel = self.Layout:FindChildObjectByName("titleLabel").__UILabel__
	if nil ~= self.LayerTitleLabel then
		local languageFlag = 'M_PVAIREWARD_TITLE'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.LayerTitleLabel:setString(content)
	end

	self.DescriptionLabel = self.Layout:FindChildObjectByName("descriptionLabel").__UILabel__
	if nil ~= self.DescriptionLabel then
		local languageFlag = 'M_PVAIREWARD_DESCRIPTION'
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.DescriptionLabel:setString(content)
	end

	self.CoinLabel = self.Layout:FindChildObjectByName("coinNumLabel").__UILabel__
	if nil ~= self.CoinLabel then
		self.CoinLabel:setString("")
	end
	self.ReputationLabel = self.Layout:FindChildObjectByName("reputaionNumLabel").__UILabel__
	if nil ~= self.ReputationLabel then
		self.ReputationLabel:setString("")
	end
end

-------------------事件回调------------------------------------------------
local function closeLayer()
	if nil ~= DailyRewardPvaiLayer.LayerInstance then

		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		DailyRewardPvaiLayer.LayerInstance:removeFromParentAndCleanup(true);
		DailyRewardPvaiLayer:ResetValue()
		XLogicManager:sharedManager():notifyShowNextAppearUI();
	end
end

function DailyRewardPvaiLayerCloseBtnClick(pObj)
	closeLayer()
	if nil ~= DailyRewardMainLayer then
		DailyRewardMainLayer:ShowOrNot(true)
	end	
end

function DailyRewardPvaiAcceptRewardBtnClick(pObj)
	DailyRewardPvaiNetWorkHandler:SendGetDailyRewardPvaiReqMessage()
end
---------------------------------------------------------------------------
local function closeCallBack(message)
	if nil ~= DailyRewardPvaiLayer.LayerInstance then
		if G_CurLayerInstance ~= DailyRewardPvaiLayer.LayerInstance then
			closeLayer(nil)
		end
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		DailyRewardPvaiLayerCloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

-----------------------------------------------------------------
local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
