
require("Script/Skill/SkillSelectLayer")
require("Script/SpriteElfExplore/ElfExploreLayer")
require("Script/UILayer/PvAILayer")
require("Script/DailyTask/dailytaskLayer")
require("Script/MainMenuUILayer")
require("Script/GameConfig/uiTagDefine")
require("Script/ManorUI")

require("Script/UILayer/buydiamondUI")
require("Script/DailyGoals/dailyGoalsLayer")
require("Script/DailyReward/dailyRewardMainLayer")
require("Script/GameSetting")
require("Script/Activity/dailyActivityLayer")
require("Script/UILayer/FeedBackUI")
require("Script/UILayer/SevenDaysUI")
require("Script/UILayer/OnlineRewardUI")
require("Script/UILayer/FirstPayRewardUI")
require("Script/DeathTip/DeathTipSystem")
require("Script/UILayer/operationUI")
require("Script/Mail/MailUILayer")
require("Script/Notice2")
require("Script/Notice1")
require("Script/UILayer/yongZheBaoZhang")
require("Script/UILayer/lotteryUI")
require("Script/GameConfig/EnterAppearUI")
require("Script/UILayer/ThreeDaysUI")
require("Script/DailySalary/dailyRewardSalaryLayer")
require("Script/GameConfig/AutoAppearUI")
require("Script/GameConfig/PKTypeCH")

require("Script/FairyLayer/FairyLevelUpLayer")
require("Script/FairyLayer/FairyAdvanceLayer")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/FairyLayer/FairyCollectionLayer")
require("Script/FairyLayer/FairyTrainLayer")


require("Script/UILayer/ReputationShopLayer")
require("Script/UILayer/PayHintLayer")
require("Script/Instance/HeroInstanceListLayer")
require("Script/Instance/HeroInstanceWinLayer")
require("Script/DailyRewardPVAI/dailyRewardPvaiLayer")
require("Script/Talent/NewTalentLayer")
require("Script/Talent/TitleLayer")

require("Script/Instance/CoinBossMgr")

require("Script/FairyLayer/FairyExchangeLayer")
require("Script/FairyLayer/FairyExchangeContactLayer")

local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");

function setLoginNum(num)
  print("setLoginNum"..num)
    AutoApearUInum = 0
	AutoAppearloginNum = num
end

function  getPKTypeCH(type)
	local rst = ""
	local getPKTypeCHItem = PKTypeCH[type]
	rst = getPKTypeCHItem.ch 
	print("getPKTypeCH"..rst..type)
	return rst 

end

function IsLoadAutoAppear()
if AutoAppear then
    print("IsLoadAutoAppear true")
else
	print("IsLoadAutoAppear false")
end
	return AutoAppear
end

function  getEnterAppearUI(Priority)
	local appearTab={id = 0 ,UnlockLevel = 0}
	if  Priority >0 and  Priority <= #Controls then
		CCLuaLog("getEnterAppearUI aaa")
		local v = Controls[Priority]
		appearTab.id = tonumber(v.ID)
		appearTab.UnlockLevel = tonumber(v.UnlockLevel)
		local userLevel = UserData:GetUserLevel()
		if appearTab.UnlockLevel == 0 or userLevel <  appearTab.UnlockLevel then
			CCLuaLog("getEnterAppearUI bbb")
			appearTab.id = 0
		end
		--  print( appearTab.id .."unlocklevel"..appearTab.UnlockLevel)
	end
	return appearTab
end

function  EnterFristAppearUI()
	local totalnum = #Controls
	
	while (true)    do
		AutoApearUInum = AutoApearUInum + 1
		if  AutoApearUInum > totalnum then
		    AutoAppear = false
			return 
		end
		local apptab = getEnterAppearUI(AutoApearUInum)
		if apptab.id ~= 0 then
			if apptab.id == 101 then
				if SevenDaysUI:getEnabeReWard() then
			  		StopHeroMove()
					AutoAppear = true
			  		on7dayRewardButtonClicked()
			  		break
				end
			elseif apptab.id == 100 then
				StopHeroMove()
				AutoAppear = true
				onNotice2()
				break
			elseif apptab.id == 103 then
				if  DailyRewardMainLayer:IsOneButtonCanClick(6003) then
					StopHeroMove()
					AutoAppear = true
					DailyRewardPvaiLayer:CreateLayer(nil)
					break
				end
			elseif apptab.id == 102 then
				if  ThreeDaysUI:getEnabeReWard() then
					StopHeroMove()
					AutoAppear = true
					ThreeDaysUI:CreateLayer(nil)
					break
				end
			elseif apptab.id == 104 then
				CCLuaLog("apptab.id == 104")
				if MonthCardManager:Get():hasMonthCard() then
					StopHeroMove()
					AutoAppear = true
					showUILayerByTag(UITagTable["MonthCardRewardLayer"].tag,true)
					break
				end
			end
		else
			if AutoApearUInum >=  #Controls then
				break
			end
		end
	end
end
if	topLayout ~= nil then
	--switchButtonNode = topLayout:FindChildObjectByName("button_shrink").__UIButton__:getMenuItemSprite();
	--onlineRewardButtonNode = topLayout:FindChildObjectByName("button_onlinereward").__UIButton__:getMenuItemSprite();
	--onlineRewardTimeLabel = topLayout:FindChildObjectByName("olreward_time").__UILabel__;
	--onlineRewardTimeLabel:setColor(ccc3(0,255,0))
	firstPayRewardButtonNode = topLayout:FindChildObjectByName("button_firstpay").__UIButton__:getMenuItemSprite();
	sevenDayRewardButtonNode = topLayout:FindChildObjectByName("button_7day").__UIButton__:getMenuItemSprite();
	pvpButtonNode = topLayout:FindChildObjectByName("button_activity").__UIButton__:getMenuItemSprite();
	--skillUpgradeNode = topLayout:FindChildObjectByName("button_skillupgrade").__UIButton__:getMenuItemSprite();
	--mysteryShopNode = topLayout:FindChildObjectByName("button_mysteryshop").__UIButton__:getMenuItemSprite();
	--elfExploerNode = topLayout:FindChildObjectByName("button_exploer").__UIButton__:getMenuItemSprite();
	communicateNode = topLayout:FindChildObjectByName("button_communicate").__UIButton__:getMenuItemSprite();
	enchaseNode = topLayout:FindChildObjectByName("button_enchase").__UIButton__:getMenuItemSprite();
	--dailyTaskNode = topLayout:FindChildObjectByName("button_dailytask").__UIButton__:getMenuItemSprite();
    --gardenButtonNode = topLayout:FindChildObjectByName("button_garden").__UIButton__:getMenuItemSprite();
	friendNode = topLayout:FindChildObjectByName("button_friendship").__UIButton__:getMenuItemSprite();
	dailyGoalsNode = topLayout:FindChildObjectByName("button_dailyGoals").__UIButton__:getMenuItemSprite();
	--dailyReWardNode = topLayout:FindChildObjectByName("dailyReWard").__UIButton__:getMenuItemSprite();	
	--heroTreasureNode = topLayout:FindChildObjectByName("button_herotreasure").__UIButton__:getMenuItemSprite();
	--lotteryNode = topLayout:FindChildObjectByName("button_lottery").__UIButton__:getMenuItemSprite();
	--yzbzNode = topLayout:FindChildObjectByName("button_yong_zhe_bao_zhang").__UIButton__:getMenuItemSprite();
    settingNode = topLayout:FindChildObjectByName("button_setting").__UIButton__:getMenuItemSprite();
    vipNode = topLayout:FindChildObjectByName("buyDiamond").__UIButton__:getMenuItemSprite();
	staminaNode = topLayout:FindChildObjectByName("staminaBt").__UIButton__:getMenuItemSprite();
	activityNode = topLayout:FindChildObjectByName("button_activity").__UIButton__:getMenuItemSprite();	
	operationNode =topLayout:FindChildObjectByName("operationBtn").__UIButton__:getMenuItemSprite();
--	topLayout:FindChildObjectByName("operationBtn").__UIButton__:setVisible(false)
    mailNode =topLayout:FindChildObjectByName("mailBtn").__UIButton__:getMenuItemSprite();
	rankListNode = topLayout:FindChildObjectByName("button_ranklist").__UIButton__:getMenuItemSprite();
	button_fairyring=topLayout:FindChildObjectByName("button_jingling_zhuan_pan").__UIButton__:getMenuItemSprite();
	--button_fairyring:setVisible(false) --default not show on the main menu
	button_ring=topLayout:FindChildObjectByName("button_normal_zhuan_pan").__UIButton__:getMenuItemSprite();
	--button_ring:setVisible(false)
	button_fairy =topLayout:FindChildObjectByName("button_fairy").__UIButton__:getMenuItemSprite();
    button_genius =topLayout:FindChildObjectByName("button_genius").__UIButton__:getMenuItemSprite();

	button_target = topLayout:FindChildObjectByName("button_target").__UIButton__:getMenuItemSprite();
	button_fairyexchange = topLayout:FindChildObjectByName("button_fairyexchange").__UIButton__:getMenuItemSprite();
	--AutoApearUInum = 0
	--AutoAppearloginNum = 0
	print("init gameUILayer"..AutoAppearloginNum)
	--AutoAppear = false
end

-- onlinereward button message 
function onOnlineRewardButtonClicked(pObj)
	CCLuaLog("onOnlineRewardButtonClicked!");
	StopHeroMove()
	OnlineRewardUI:CreateLayer(nil);
end

-- firstpay button message 
function onFirstPayRewardButtonClicked(pObj)
	CCLuaLog("onFirstPayRewardButtonClicked!");
	StopHeroMove()
	FirstPayRewardUI:CreateLayer(nil);
end

-- switch button message 
function onSwitchButtonClicked(pObj)
	CCLuaLog("onSwitchButtonClicked!");
	StopHeroMove()
	FeedBackUI:CreateLayer(nil);
end

-- sevenday reward message 
function on7dayRewardButtonClicked(pObj)
	CCLuaLog("on7dayRewardButtonClicked!");
	StopHeroMove()
	SevenDaysUI:CreateLayer(nil);
end

function onDailyReWardButton(pObj)
    print("onDailyReWardButton")
    StopHeroMove()
    DailyRewardMainLayer:CreateLayer(nil)
end

function onHeroTreasureButton(pObj)
    print("onHeroTreasureButton")
    StopHeroMove()
	yongZheBaoZhang:CreateLayer(nil)
    --DailyRewardMainLayer:CreateLayer(nil)
end

function onLotteryButton(pObj)
    print("onLotteryButton")
    StopHeroMove()
	lotteryUI:CreateLayer(nil)
    --DailyRewardMainLayer:CreateLayer(nil)
end

function onNotice1()
    print("GameUILayer onNotice1")
    StopHeroMove()
    Notice1:CreateLayer(nil)
end

function onNotice2()
    print("GameUILayer onNotice2")
    StopHeroMove()
    Notice2:CreateLayer(nil)
end

function IsLoadNotice2()
	return Notice2:IsLoadAlready()
end

function onGameSetting(pObj)
      print("onGameSetting")
      StopHeroMove()
      GameSetting:CreateLayer(nil)
end

function onVipSetting(pObj)
    StopHeroMove()
    buyDiamondUI:CreateLayer(nil)
	CCLuaLog("onVipSetting.")
end

function onGardenButton(pObj)
    --print("onGardenButton")
    --StopHeroMove()
	--ManorUI:CreateLayer(nil)
end

-- pvp button clicked 
-- show the ppve layout
function onPvPButtonClicked(pObj)
	CCLuaLog("onPvPButtonClicked!");
	StopHeroMove()
	-- show ppve ui layout
end

function onActivityClicked(pObj)
	StopHeroMove()
	DailyActivityLayer:CreateLayer(nil)
end

--Note: ¬ª√±√à¬°√ó√™√ä¬Ø¬∂√í¬ª¬ª√å√•√Å¬¶√Å¬Ω√ï√ü¬µ√Ñ√ä√Ω√Å¬ø
function getStaminaExchangeNums(nowPowerBuyCount)
	local value = ExchangeItems[2700+nowPowerBuyCount]
	if value~= nil then
		local staminaNum, diamondNum;
		local ToItems = value.ToItems
		local itemStr = ToItems
		local _start,_end = string.find(itemStr,"/")
		_start = tonumber(_start);
		if nil ~= _start and 1 ~= _start then
			local tmpStr = string.sub(itemStr,_start+1,-1)
			staminaNum = tonumber(tmpStr)
		end
			
		local FromItem = value.FromItems
		itemStr = FromItem
		_start,_end = string.find(itemStr,"/")
		_start = tonumber(_start);
		if nil ~= _start and 1 ~= _start then
			local tmpStr = string.sub(itemStr,_start+1,-1)
			diamondNum = tonumber(tmpStr)
		end
		CCLuaLog("staminaNum: " .. staminaNum .. " diamondNum: " .. diamondNum)
		return staminaNum, diamondNum
	end
	return nil
end

function onStaminaConfirmed(sender)
	CCLuaLog("onStaminaConfirmed.");
	local nowPowerBuyCount = getNowPowerBuyCount()
	if nowPowerBuyCount ~= nil and nowPowerBuyCount < 20 then
		local msgname="CSEquipmentComposeReq"
		Req = {
			 exchange_item_id= 2700+nowPowerBuyCount,
			 from_item_pos ={},		
		}
		send_message(msgname, Req, true)
	end
end

function onStaminaClicked(pObj)
	StopHeroMove()
	CCLuaLog("onStaminaClicked!");
	local buyText = LanguageLocalization:GetLocalization("M_DIAMONDSHOP_BUY")
	local buyText1 = LanguageLocalization:GetLocalization("M_BUY_ACTION")
	
	local nowPowerBuyCount = getNowPowerBuyCount()
	if nowPowerBuyCount ~= nil and nowPowerBuyCount < 20 then

		local staminaNum, diamondNum = getStaminaExchangeNums(nowPowerBuyCount)
		assert(diamondNum ~= nil, "can't find stamina exchange")
		local content = buyText .. staminaNum .. buyText1;
		DiamondWarningLayer:Show(content,diamondNum,onStaminaConfirmed,nil,ccc3(255,255,255),"","")

		local remainBuyCount = getRemainPowerBuyCount()

		if remainBuyCount > 0 then
			DiamondWarningLayer:SetTipContentColor(GetLuaLocalization("M_VIP_BUY_PHYSICAL")..remainBuyCount, ccc3(0,255,0))
		else
			DiamondWarningLayer:SetTipContentColor(GetLuaLocalization("M_VIP_BUY_PHYSICAL")..remainBuyCount, ccc3(255,0,0))
		end
	else

	end
end

--Note: skill panel enter
function onSkillUpgradeButtonClicked(pObj)
	print("function onSkillUpgradeButtonClicked(pObj)")
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
    SkillEquipLayer:CreateLayer(nil)	
	OnClickSkillButton()
end

function onMysteryShopButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	               
    local mainLayer = MainMenuLayer:getMainMenuLayer()
	if nil ~= mainLayer then
		mainLayer:showMagicShop()
    end
 
end

function onDailyTaskButtonClicked(pObj)
	--GameAudioManager:sharedManager():playEffect(350001,false);
	--StopHeroMove()
	--DailyTaskLayer:CreateLayer(nil)
end

function onDailyGoalsButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	DailyGoalsLayer:CreateLayer(nil)
end

function onOperationButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	operationUI:CreateLayer(nil)
end

function onMailButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	MailUILayer:createLayer()
end

function onElfExploerButtonClicked(pObj)
	print("function onElfExploerButtonClicked(pObj)")
	--GameAudioManager:sharedManager():playEffect(350001,false);
	--StopHeroMove()
	--ElfExploerLayer:CreateLayer(nil)
end

local function showNoFuctionAlert()
	local rstString = LanguageLocalization:GetLocalization("M_FUNCTION_NOTOPEN");
    GameApi:showMessage(rstString);
end

function onCommunicateButtonClicked(pObj)
	local userLevel = UserData:GetUserLevel()
	if userLevel <= 1 then
		local rstString = LanguageLocalization:GetLocalization("M_NOT_REACH_SYSTEMLEVEL");
		GameApi:showMessage(rstString);
		return
	end
	--showNoFuctionAlert();
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	PvAILayer:openPvAILayer()
end

function onEnchaseButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["EnchaseUILayer"].tag,true)
	--showUILayerByTag(UITagTable["AlchemyUILayer"].tag,true)
end

function onTargetButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["GrowthTargetLayer"].tag,true)
end

function onFrinedButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["NearByFriendUILayer"].tag,true)
end

function onRankListButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	OnPressBestFighterRankListBtn()
end

--¬µ√ò√è√Ç¬≥√á¬∏¬±¬±¬æ√ñ√ê¬µ√Ñ√ç¬º¬±√™√í√æ¬≤√ò√è√î√ä¬æ¬¥¬¶√Ä√≠
function onfairyButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["FairyBagLayer"].tag,true)
    CCLuaLog("onfairyButtonClicked ")    
end

function onGeniusButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	--showUILayerByTag(UITagTable["FairyBagLayer"].tag,true)
    CCLuaLog("onGeniusButtonClicked ")
    showUILayerByTag(UITagTable["NewTalentLayer"].tag,true)
end

function onFairyExchangeButtonClicked(pObj)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["FairyExchangeLayer"].tag,true)
end



--µÿœ¬≥«∏±±æ÷–µƒÕº±Í“˛≤ÿœ‘ æ¥¶¿Ì
local specialEntranceHideBtnTable = {	
	"button_communicate",--pvai

	--"button_task",
	--"button_reward",
	--"button_garden",
	"button_activity",
	--"button_exploer",
	--"button_dailytask",
	"button_dailyGoals",
	--"dailyReWard",
	"operationBtn",
	"button_target",
	"button_firstpay",
	--"olreward_time",
	--"button_onlinereward",
	"button_7day",
	--"button_lottery",
	--"button_herotreasure",
	--"button_yong_zhe_bao_zhang",

    --"mailNode",
}

function HideSpecialEntranceBtn()

	if topLayout == nil then
		return false
	end
    
	for _ , v in pairs(specialEntranceHideBtnTable) do
		assert(topLayout:FindChildObjectByName(v), "can't find")
		topLayout:FindChildObjectByName(v).__UIButton__:setVisible(false)
		print("hide...."..v)
	end
	return true
end

function ShowWBSettingButton()
	if topLayout == nil then
		return false
	end

	assert(topLayout:FindChildObjectByName("button_bossCombatSetting"), "can't find")
	topLayout:FindChildObjectByName("button_bossCombatSetting").__UIButton__:setVisible(true)
	return true
end

function ShowBestFighterRankListBtn()
	if topLayout == nil then
		return false
	end

	assert(topLayout:FindChildObjectByName("button_ranklist"), "can't find")
	topLayout:FindChildObjectByName("button_ranklist").__UIButton__:setVisible(true)

	return true
end

function onfairyringButtonClicked()
	if topLayout == nil then
		return false
	end
	StopHeroMove()
	
	lotteryUI:CreateLayer(nil, 10080)
end

if	topLayout ~= nil then
	--switchButtonNode:registerScriptTapHandler(onSwitchButtonClicked);
	--onlineRewardButtonNode:registerScriptTapHandler(onOnlineRewardButtonClicked);
	--firstPayRewardButtonNode:registerScriptTapHandler(onFirstPayRewardButtonClicked);
	sevenDayRewardButtonNode:registerScriptTapHandler(on7dayRewardButtonClicked);
	pvpButtonNode:registerScriptTapHandler(onPvPButtonClicked);
	--skillUpgradeNode:registerScriptTapHandler(onSkillUpgradeButtonClicked);
	--mysteryShopNode:registerScriptTapHandler(onMysteryShopButtonClicked);
	--elfExploerNode:registerScriptTapHandler(onElfExploerButtonClicked);
	communicateNode:registerScriptTapHandler(onCommunicateButtonClicked);
	enchaseNode:registerScriptTapHandler(onEnchaseButtonClicked);
	--dailyTaskNode:registerScriptTapHandler(onDailyTaskButtonClicked);
    --gardenButtonNode:registerScriptTapHandler(onGardenButton);
	friendNode:registerScriptTapHandler(onFrinedButtonClicked);
	dailyGoalsNode:registerScriptTapHandler(onDailyGoalsButtonClicked);
    --dailyReWardNode:registerScriptTapHandler(onDailyReWardButton);
	--heroTreasureNode:registerScriptTapHandler(onHeroTreasureButton);
	--lotteryNode:registerScriptTapHandler(onLotteryButton);
	
	--yzbzNode:registerScriptTapHandler(onYZBZButtonClicked);
    settingNode:registerScriptTapHandler(onGameSetting);
    -- vipNode:registerScriptTapHandler(onVipSetting);
	staminaNode:registerScriptTapHandler(onStaminaClicked);
	activityNode:registerScriptTapHandler(onActivityClicked);
	operationNode:registerScriptTapHandler(onOperationButtonClicked);
    mailNode:registerScriptTapHandler(onMailButtonClicked);
	button_fairyring:registerScriptTapHandler(onfairyringButtonClicked);
	button_ring:registerScriptTapHandler(onLotteryButton);
	rankListNode:registerScriptTapHandler(onRankListButtonClicked);
	button_fairy:registerScriptTapHandler(onfairyButtonClicked)
    button_genius:registerScriptTapHandler(onGeniusButtonClicked)
	button_target:registerScriptTapHandler(onTargetButtonClicked)
	button_fairyexchange:registerScriptTapHandler(onFairyExchangeButtonClicked)
end


--Note: √ì√É√ì√ö√î¬§¬º√ì√î√òLua√é√Ñ¬º√æ¬£¬¨√é√∞√â¬æ¬≥√Ω
function InitGameUILayer()
	return true
end

 function updateGameUI(dt)
	if nil ~= dailyReWardNode then
		DailyRewardMainLayer:ShowRewardTipIconOrNot()

	end

    -- ºÏ≤‚ÃÏ∏≥∞¥≈•
    if TalentMgr ~= nil and #TalentMgr.unlockTitles > 0 then
        local talentBtn = topLayout:FindChildObjectByName("button_genius").__UIButton__
        talentBtn:showTipIcon()

        if  NewTalentLayer.uiLayout ~= nil then
            local checkTitleBtn = NewTalentLayer.uiLayout:FindChildObjectByName("checkTitleBtn").__UIButton__
            checkTitleBtn:showTipIcon()
        end
        
    else
        local talentBtn = topLayout:FindChildObjectByName("button_genius").__UIButton__
        talentBtn:hideTipIcon()

        if  NewTalentLayer.uiLayout ~= nil then
            local checkTitleBtn = NewTalentLayer.uiLayout:FindChildObjectByName("checkTitleBtn").__UIButton__
            checkTitleBtn:hideTipIcon()
        end
    end

	-- ºÏ≤‚¡˘∆∑∂“ªª∞¥≈•
	local fairyExchangeIndex = GetFairyExchangeIndex()

	local fairyExchangeBtn = topLayout:FindChildObjectByName("button_fairyexchange").__UIButton__
	if fairyExchangeIndex == -1 then
		fairyExchangeBtn:hideTipIcon()
	else
		fairyExchangeBtn:showTipIcon()
	end
    
end

-- ◊™∑¢–¬ ÷“˝µºœ˚œ¢
function OnTutorialMenuEvent(name)
    GameApi:showMessage("tutorial event "..name)
    TutorialsManager:Get():HandleOneEvent(name, 1)
end
CCLuaLog("LoadGameUILayer.lua  Loaded!")
