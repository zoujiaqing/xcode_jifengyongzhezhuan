require("Script/GameConfig/characterString")
require("Script/Skill/SkillSelectLayer")

UserInfoUILayer = {
	STAMINA_RESTORE_CD = 360, -- 6 * 60
	MAX_STAMINA = 100,
	uiLayerInstance = nil,
	uiLayout = nil,
	notificationFunc = 0,
	updateFunction = 0,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RemoveOneLayer(UserInfoUILayer.uiLayerInstance)
	TXGUI.UIManager:sharedManager():removeUILayout("UserInfoLayout")
	if UserInfoUILayer.notificationFunc ~= 0 then
		NotificationCenter:defaultCenter():unregisterScriptObserver(UserInfoUILayer.notificationFunc)
	end
	if UserInfoUILayer.updateFunction ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(UserInfoUILayer.updateFunction)
	end
	UserInfoUILayer:resetValue()
end

function UserInfoUILayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["UserInfoUILayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,30,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/UserInfoUI.plist",self.uiLayerInstance, "UserInfoLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function UserInfoUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = 0
	self.updateFunction = 0
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= UserInfoUILayer.uiLayerInstance then
			if G_CurLayerInstance ~= UserInfoUILayer.uiLayerInstance then
				if UserInfoUILayer.uiLayerInstance ~= nil then
					UserInfoUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
				end
				ret = 1
			end
		end
	end

	return ret
end

local function userInfoUpdate(dt)
	UserInfoUILayer:setNextStaminaRestoreTime()
	UserInfoUILayer:setTotalStaminaRestoreTime()
	UserInfoUILayer:setUserStamina()
end

local function onCloseButtonClicked(tag)
	if UserInfoUILayer.uiLayerInstance ~= nil then
		UserInfoUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

local function onBagButtonClicked(tag)
	if UserInfoUILayer.uiLayerInstance ~= nil then
		UserInfoUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
	showUILayerByTag(UITagTable["BackPackLayer"].tag,true)	
end

local function onChangeButtonClicked(tag)
	local num = ItemManager:Get():getItemNumberById(416027)
	if num > 0 then
		if UserInfoUILayer.uiLayerInstance ~= nil then
			UserInfoUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
		end
		showUILayerByTag(UITagTable["ChangeNameLayer"].tag,true)
	else
		GameApi:showMessage(LanguageLocalization:GetLocalization("M_CHANGENAME_ERROR1"))
	end
end

local function onSkillButtonClicked(tag)
	if UserInfoUILayer.uiLayerInstance ~= nil then
		UserInfoUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
    SkillEquipLayer:CreateLayer(nil)	
	OnClickSkillButton()
end

local function onMapButtonClicked(tag)
	if UserInfoUILayer.uiLayerInstance ~= nil then
		UserInfoUILayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
	local layer = tolua.cast(showUILayerByTag(UITagTable["CityListLayer"].tag,true),"CityListLayer")
	if layer ~= nil then
		layer:openCityList()
	end
end


function UserInfoUILayer:checkSingleFairyUnlock(pos,sum)
	local level = G_GetFairyPosUnlockLevel(pos)
	local userInfo = UserData:GetUserInfo()
	local num = sum
	if userInfo.level >= level then
		num = num + 1
	end
	return num
end

function UserInfoUILayer:checkSingleFairyExist(pos,sum)
	local num = sum
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		num = num + 1
	end
	return num
end

function UserInfoUILayer:checkFairyNum()
	local level = G_GetFairyPosUnlockLevel(pos)
	local fairyNum = 0
	local fairyTotal = 0
	fairyTotal = self:checkSingleFairyUnlock(FAIRY_POS_START_BTL,fairyTotal)
	fairyTotal = self:checkSingleFairyUnlock(FAIRY_POS_START_BTL + 1,fairyTotal)
	fairyTotal = self:checkSingleFairyUnlock(FAIRY_POS_START_AST,fairyTotal)
	fairyTotal = self:checkSingleFairyUnlock(FAIRY_POS_START_AST + 1,fairyTotal)

	fairyNum = self:checkSingleFairyExist(FAIRY_POS_START_BTL,fairyNum)
	fairyNum = self:checkSingleFairyExist(FAIRY_POS_START_BTL + 1,fairyNum)
	fairyNum = self:checkSingleFairyExist(FAIRY_POS_START_AST,fairyNum)
	fairyNum = self:checkSingleFairyExist(FAIRY_POS_START_AST + 1,fairyNum)
	local fairyNumLabel = self.uiLayout:FindChildObjectByName("fairyNumLabel").__UILabel__
	fairyNumLabel:setString(fairyNum .. "/" .. fairyTotal)
end

function UserInfoUILayer:setUserStamina()
	local userInfo = UserData:GetUserInfo()
	local staminaNumLabel = self.uiLayout:FindChildObjectByName("staminaNumLabel").__UILabel__
	staminaNumLabel:setString(userInfo.m_stamina .. "/" .. UserInfoUILayer.MAX_STAMINA)
end

function UserInfoUILayer:timeToString(time)
	--local s = math.modf(time / 1000)
	local temp = math.modf(time/60)
	local second = time - temp * 60
	local hour = math.modf(temp / 60)
	local minute = temp - hour * 60

	return string.format("%02d: %02d: %02d",hour,minute,second)
end

function UserInfoUILayer:setNextStaminaRestoreTime()
	local userInfo = UserData:GetUserInfo()
	local time = UserInfoUILayer.STAMINA_RESTORE_CD - GameApi:GetLastStaminaRestoreTime()
	if time < 0 then
		time = 0
	end
	local str = self:timeToString(time)
	local timeNumLabel = self.uiLayout:FindChildObjectByName("timeNumLabel").__UILabel__
	timeNumLabel:setString(str)
end

function UserInfoUILayer:setTotalStaminaRestoreTime()
	local userInfo = UserData:GetUserInfo()
	local time = 0
	if UserInfoUILayer.MAX_STAMINA > userInfo.m_stamina then
		local nextTime = UserInfoUILayer.STAMINA_RESTORE_CD - GameApi:GetLastStaminaRestoreTime()
		if nextTime < 0 then
			nextTime = 0
		end
		time = nextTime + UserInfoUILayer.STAMINA_RESTORE_CD * ((UserInfoUILayer.MAX_STAMINA - userInfo.m_stamina) / 1 - 1)
	end
	local str = self:timeToString(time)
	local allTimeNumLabel = self.uiLayout:FindChildObjectByName("allTimeNumLabel").__UILabel__
	allTimeNumLabel:setString(str)
end

function UserInfoUILayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local bagBt = self.uiLayout:FindChildObjectByName("bagBt").__UIButton__:getMenuItemSprite()
	bagBt:registerScriptTapHandler(onBagButtonClicked)
	local skillBt = self.uiLayout:FindChildObjectByName("skillBt").__UIButton__
	skillBt:getMenuItemSprite():registerScriptTapHandler(onSkillButtonClicked)
	local changeBt = self.uiLayout:FindChildObjectByName("changeBt").__UIButton__:getMenuItemSprite()
	changeBt:registerScriptTapHandler(onChangeButtonClicked)
	if getNewSkillId() ~= 0 then
		skillBt:showTipIcon()
	end
	local mapBt = self.uiLayout:FindChildObjectByName("mapBt").__UIButton__:getMenuItemSprite()
	mapBt:registerScriptTapHandler(onMapButtonClicked)
	
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	self.updateFunction = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(userInfoUpdate, 1.0, false)

	local userInfo = UserData:GetUserInfo()
	if userInfo.m_vip >= 1 then
		local vipName = "map_ui_FRAME_ICON_HEADVIP" .. userInfo.m_vip
		local sprite = AspriteManager:getInstance():getOneFrame(KUI_BIN,vipName)
		local vipPic = self.uiLayout:FindChildObjectByName("VipPic").__UIPicture__
		vipPic:setSprite(sprite,false)
	end

	local headPicName = Characters[userInfo.type].bagHeadPic
	local headIcon = AspriteManager:getInstance():getOneFrame(KICON_BIN,headPicName)
	local headPic = self.uiLayout:FindChildObjectByName("HeadIcon").__UIPicture__
	headPic:setSprite(headIcon)

	local nameLabel = self.uiLayout:FindChildObjectByName("userNameLabel").__UILabel__
	nameLabel:setString(userInfo.szName)

	local userLevelLabel = self.uiLayout:FindChildObjectByName("userLevelLabel").__UILabel__
	userLevelLabel:setString("Lv." .. userInfo.level)

	local attributeLabel = self.uiLayout:FindChildObjectByName("attributeLabel").__UILabel__
	attributeLabel:setString("战斗力:" .. userInfo.effectiveness)
	
	local expLabel = self.uiLayout:FindChildObjectByName("expLabel").__UILabel__
	expLabel:setString(userInfo.exp .. "/" .. userInfo.nextExp)

	self:checkFairyNum()

	local coinLabel = self.uiLayout:FindChildObjectByName("coinLabel").__UILabel__
	coinLabel:setString(MainMenuLayer:GetNumByFormat(userInfo.m_gold))

	local diamondLabel = self.uiLayout:FindChildObjectByName("diamondLabel").__UILabel__
	diamondLabel:setString(MainMenuLayer:GetNumByFormat(userInfo.m_diamond))

	local stoneLabel = self.uiLayout:FindChildObjectByName("stoneLabel").__UILabel__
	stoneLabel:setString(MainMenuLayer:GetNumByFormat(userInfo.m_soulStone))

	local reputationLabel = self.uiLayout:FindChildObjectByName("reputationLabel").__UILabel__
	reputationLabel:setString(MainMenuLayer:GetNumByFormat(userInfo.m_reputation))

	self:setNextStaminaRestoreTime()
	self:setTotalStaminaRestoreTime()
	self:setUserStamina()
end