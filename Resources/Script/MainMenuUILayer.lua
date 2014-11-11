require("Script/GameConfig/uiTagDefine")
require("Script/UILayer/enchaseUILayer")
require("Script/Friend/FriendLayer")
require("Script/Friend/RedFriendUILayer")
require("Script/Friend/EnemiesFriendUILayer")
require("Script/Friend/NearByFriendUILayer")
require("Script/Friend/addFriend")
require("Script/Friend/OtherPlayerEquipLayer")
require("Script/Instance/UnderBoxLayer")
require("Script/Instance/CityDefendChatLayer")
require("Script/Instance/CityDefendRoomListLayer")
require("Script/Instance/CityDefendRoomLayer")
require("Script/SpriteElfExplore/ElfExploreLayer")
require("Script/SpriteElfExplore/ExchangeElfLayer")
require("Script/SpriteElf/SpriteElfLayer")
require("Script/SpriteElf/SpriteElfStrengthenLayer")
require("Script/Skill/SkillEquipLayer")
require("Script/Skill/SkillSelectLayer")
require("Script/DailyTask/dailytaskLayer")
require("Script/UILayer/PvAILayer")
require("Script/UILayer/yongZheBaoZhang")
require("Script/Equipment/equipUpgradeLayer")
require("Script/Equipment/EquipBreakLayer")
require("Script/Chat/ChatLayer")
require("Script/Chat/ChatPlayerOption")
require("Script/Chat/ShieldLayer")
require("Script/HelpUI/HelpUILayer")
require("Script/Instance/WorldBossRemainTimeLayer")
require("Script/Instance/WorldBossHPLayer")
require("Script/Instance/WorldBossDamageLayer")
require("Script/Instance/WorldBossFailLayer")
require("Script/Instance/WorldBossOverLayer")
require("Script/Activity/dailyActivityLayer")
require("Script/Instance/BestFighterMgr")
require("Script/Instance/BestFighterRemainTimeLayer")
require("Script/Instance/BestFighterQueueRemainLayer")
require("Script/Instance/BestFighterMatchEmptyLayer")
require("Script/Instance/BestFighterOverRewardLayer")
require("Script/Instance/BestFighterPointShopLayer")
require("Script/Instance/BestFighterRankListLayer")
--require("Script/UILayer/lotteryUI")
require("Script/FairyLayer/FairyLevelUpLayer")
require("Script/FairyLayer/FairyAdvanceLayer")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/FairyLayer/FairyCollectionLayer")
require("Script/FairyLayer/FairyTrainLayer")
require("Script/Fairy/FairyBagLayer")
require("Script/Fairy/FairyContactInfoLayer")
require("Script/GrowthTarget/GrowthTargetLayer")

require("Script/UILayer/ReputationShopLayer")
require("Script/UILayer/PayHintLayer")
require("Script/UILayer/UserInfoUILayer")
require("Script/Instance/HeroInstanceListLayer")
require("Script/Instance/HeroInstanceHPLayer")
require("Script/Instance/HeroInstanceWinLayer")

require("Script/DailyRewardPVAI/dailyRewardPvaiLayer")
require("Script/UILayer/ThreeDaysUI")
require("Script/Talent/NewTalentLayer")
require("Script/Talent/TitleLayer")
require("Script/UILayer/MonthCardRewardLayer")
require("Script/UILayer/ChangeNameLayer")

require("Script/Instance/CoinBossRankLayer")
require("Script/Instance/CoinBossDamageLayer")
require("Script/Instance/CoinBossWinLayer")

require("Script/FairyLayer/FairyExchangeLayer")
require("Script/FairyLayer/FairyExchangeContactLayer")

function GetMainMenuLayerNode()
	local node =  SpriteFactory:sharedFactory():GetMainMenuLayerNode()
	return node
end

-- lua实现的ui的创建显示的具体实现，会由C++调用，统一的实现函数
function getLuaLayerByTag(tag,isAutoCreate)
	local layer = nil;
	if tag == UITagTable["EnchaseUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			--layer = EnchaseUILayer:CreateLayer()
			EnchaseUILayer:CreateLayer()
		end
	elseif tag == UITagTable["lotteryUI"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			lotteryUI:CreateLayer(nil, 10080)
		end
	elseif tag == UITagTable["FriendUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FriendUILayer:CreateLayer()
		end
	elseif tag == UITagTable["NearByFriendUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			NearByFriendUILayer:CreateLayer()
		end
	elseif tag == UITagTable["AddFriendUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			AddFriendUILayer:CreateLayer()
		end
	elseif tag == UITagTable["UnderBoxLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			UnderBoxLayer:CreateLayer()
		end
	elseif tag == UITagTable["ElfExploerLayer"].tag then
		--layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		--if layer == nil then
			--ElfExploerLayer:CreateLayer()
		--end
	elseif tag == UITagTable["ExchangeElfLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ExchangeElfLayer:CreateLayer()
		end
	elseif tag == UITagTable["SpriteElfPanel"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CreateSpriteElfLayer()
		end
	elseif tag == UITagTable["DailyTaskLayer"].tag then
		--layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		--if layer == nil then
			--DailyTaskLayer:CreateLayer()
		--end
	elseif tag == UITagTable["SkillSelectLayer"].tag then
		--layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		--if layer == nil then
			--SkillSelectLayer:CreateLayer(nil)
		--end	
	elseif tag == UITagTable["DailySalaryLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			DailyRewardSalaryLayer:CreateLayer(nil)
		end		
	elseif tag == UITagTable["OtherPlayerEquipLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			OtherPlayerEquipLayer:CreateLayer()
		end
	elseif tag == UITagTable["yongZheBaoZhang"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			yongZheBaoZhang:CreateLayer()
		end
	elseif tag == UITagTable["PvAILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			PvAILayer:openPvAILayer()
		end
	elseif tag == UITagTable["ChatUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ChatLayer:CreateLayer()
		end
	elseif tag == UITagTable["ChatPlayerOption"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ChatPlayerOption:CreateLayer()
		end
	elseif tag == UITagTable["ShieldLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ShieldLayer:CreateLayer()
		end
	elseif tag == UITagTable["CityDefendChatLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CityDefendChatLayer:CreateLayer()
		end
	elseif tag == UITagTable["CityDefendRoomListLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CityDefendRoomListLayer:CreateLayer()
		end
	elseif tag == UITagTable["CityDefendRoomLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CityDefendRoomLayer:CreateLayer()
		end
	elseif tag == UITagTable["DailySalaryLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			DailyRewardSalaryLayer:CreateLayer(nil)
		end
	elseif tag == UITagTable["WorldBossRemainTimeLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			WorldBossRemainTimeLayer:CreateLayer()
		end
	elseif tag == UITagTable["WorldBossHPLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			WorldBossHPLayer:CreateLayer()
		end
	elseif tag == UITagTable["WorldBossDamageLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			WorldBossDamageLayer:CreateLayer()
		end
	elseif tag == UITagTable["WorldBossFailLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			WorldBossFailLayer:CreateLayer()
		end
	elseif tag == UITagTable["WorldBossOverLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			WorldBossOverLayer:CreateLayer()
		end
	elseif tag == UITagTable["DailyActivityLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			DailyActivityLayer:CreateLayer(nil)
		end
	elseif tag == UITagTable["HelpUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			HelpUILayer:createLayer()
		end
	elseif tag == UITagTable["BestFighterRemainTimeLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			BestFighterRemainTimeLayer:CreateLayer()
		end
	elseif tag == UITagTable["BestFighterQueueRemainLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			BestFighterQueueRemainLayer:CreateLayer()
		end
	elseif tag == UITagTable["BestFighterMatchEmptyLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			BestFighterMatchEmptyLayer:CreateLayer()
		end
	elseif tag == UITagTable["BestFighterOverRewardLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			BestFighterOverRewardLayer:CreateLayer()
		end
	elseif tag == UITagTable["BestFighterPointShopLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			initPointShopData()
			BestFighterPointShopLayer:CreateLayer()
		end
	elseif tag == UITagTable["BestFighterRankListLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			BestFighterRankListLayer:CreateLayer()
		end
	elseif tag == UITagTable["RedFriendUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			RedFriendUILayer:CreateLayer()
		end
	elseif tag == UITagTable["EquipUpgradeLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			EquipUpgradeLayer:createLayer()
		end
	elseif tag == UITagTable["EquipBreakLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			EquipBreakLayer:createLayer()
		end
	elseif tag == UITagTable["EnemiesFriendUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			EnemiesFriendUILayer:CreateLayer()
		end
	elseif tag == UITagTable["FairyLevelUpLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyLevelUpLayer:CreateLayer()
		end		
	elseif tag == UITagTable["FairyAdvanceLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyAdvanceLayer:CreateLayer()
		end
	elseif tag == UITagTable["FairyCollectionLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyCollectionLayer:CreateLayer()
		end
	elseif tag == UITagTable["FairyExchangeLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyExchangeLayer:CreateLayer()
		end
	elseif tag == UITagTable["FairyExchangeContactLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyExchangeContactLayer:CreateLayer()
		end
	elseif tag == UITagTable["FairyInfoLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyInfoLayer:CreateLayer()
		end
	elseif tag == UITagTable["FairyTrainLayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyTrainLayer:CreateLayer()
		end
	elseif tag ==  UITagTable["FairyBagLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyBagLayer:createLayer()
		end
	elseif tag ==  UITagTable["FairyContactInfoLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			FairyContactInfoLayer:createLayer()
		end
	elseif tag ==  UITagTable["ReputationShopLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ReputationShopLayer:CreateLayer()
		end
    elseif tag ==  UITagTable["HeroInstanceListLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			HeroInstanceListLayer:CreateLayer()
		end
    elseif tag ==  UITagTable["HeroInstanceHPLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			HeroInstanceHPLayer:CreateLayer()
		end
    elseif tag ==  UITagTable["HeroInstanceWinLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			HeroInstanceWinLayer:CreateLayer()
		end
		
	elseif tag == UITagTable["UserInfoUILayer"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			UserInfoUILayer:createLayer()
		end
    elseif tag == UITagTable["dailyRewardPVAIPanel"].tag then
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			DailyRewardPvaiLayer:CreateLayer()
		end
    elseif tag ==  UITagTable["PayHintLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			PayHintLayer:CreateLayer()
		end
	elseif tag ==  UITagTable["ThreeDaysUI"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ThreeDaysUI:CreateLayer(nil)
		end
	elseif tag ==  UITagTable["GrowthTargetLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			GrowthTargetLayer:createLayer(nil)
		end
    elseif tag ==  UITagTable["NewTalentLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			NewTalentLayer:CreateLayer(nil)
		end
    elseif tag ==  UITagTable["TitleLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			TitleLayer:CreateLayer(nil)
		end
	 elseif tag ==  UITagTable["MonthCardRewardLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			MonthCardRewardLayer:createLayer(nil)
		end
    elseif tag ==  UITagTable["CoinBossRankLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CoinBossRankLayer:CreateLayer(nil)
		end
    elseif tag ==  UITagTable["CoinBossDamageLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CoinBossDamageLayer:CreateLayer(nil)
		end
    elseif tag ==  UITagTable["CoinBossWinLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			CoinBossWinLayer:CreateLayer(nil)
		end
	elseif tag ==  UITagTable["ChangeNameLayer"].tag then 
		layer = MainMenuLayer:getMainMenuLayer():findUILayerByTag(tag)
		if layer == nil then
			ChangeNameLayer:createLayer(nil)
		end
		
	end	
end

-- 显示任一UI面板,isAutoCreate表示该UI还没创建情况下是否自动创建.true 自动创建未显示.false 不处理，返回NULL，并且自动显示，统一的调用函数
function showUILayerByTag(tag,isAutoCreate)
	local layer = nil
	local mainLayer = MainMenuLayer:getMainMenuLayer()
	if mainLayer ~= nil then
		layer = mainLayer:showUILayerByTag(tag,isAutoCreate)
	end
	return layer
end

function showUILayerByName(name,isAutoCreate)
	if false == showUILayerByHardCode(name) then	
		local uiTagTableItem = UITagTable[name]
		if nil == uiTagTableItem then
			return nil
		end
		local tag = uiTagTableItem.tag;
		return showUILayerByTag(tag,isAutoCreate)
	end
end

function showUILayerByHardCode(name)
	local bShowByHardCode = false
	if "SpriteElfStrengthenPanel" == name then
		bShowByHardCode = true
		SpriteFactory:sharedFactory():ShowElfPanelWithBackPackLayer()
	elseif "NormalFBTranspoint" == name then
		--print("---------- normal point ")
		bShowByHardCode = true
		local hero = GameManager:Get():getHero()
		if nil ~= hero then
			hero:MoveToSpriteTranspoint(true)
		end
	elseif "EliteFBTranspoint" == name then
		bShowByHardCode = true
		local hero = GameManager:Get():getHero()
		if nil ~= hero then
			hero:MoveToSpriteTranspoint(false)
		end
	elseif "EnterUnderCity" == name then
		bShowByHardCode = true
		InstanceManager:Get():onEnterSpecialEntrance(ENTERANCE_UNDERGROUND)
	end
	return bShowByHardCode
end

-- 得到任一UI面板,isAutoCreate表示该UI还没创建情况下是否自动创建.true 自动创建未显示.false 不处理，返回NULL，统一的调用函数
function getUILayerByTag(tag,isAutoCreate)
	local layer = nil
	local mainLayer = MainMenuLayer:getMainMenuLayer()
	if mainLayer ~= nil then
		layer = mainLayer:getUILayerByTag(tag,isAutoCreate)
	end
	return layer
	--return MainMenuLayer:getMainMenuLayer():showUILayerByTag(tag,isAutoCreate)
end

-- 隐藏lua中创建的UI面板。是隐藏还是删除根据各个UI自行设定逻辑,由C++调用，统一的实现函数
function hideLuaLayerByTag()
	
end

-- 关闭任意面板，统一的调用函数
function hideUILayerByTag(tag)
	MainMenuLayer:getMainMenuLayer():hideUILayerByTag(tag)
end

function HideOneLayerByTag(tag)
	local ret = false
	local node =  SpriteFactory:sharedFactory():GetMainMenuLayerNode()
	local layer = node:getChildByTag(tag)
	if layer ~= nil then
		layer:setVisible(false)
		ret = true
	end
	return ret
end

function ShowOneLayerByTag(tag)
	local ret = false
	local node =  SpriteFactory:sharedFactory():GetMainMenuLayerNode()
	local layer = node:getChildByTag(tag)
	if layer ~= nil then
		layer:setVisible(true)
		ret = true
	end
	return ret
end