require("Script/OsCommonMethod")
require("Script/GameConfig/Item")
require("Script/SpriteElf/SpriteElfLayer")
require("Script/SpriteElfExplore/Explore")
require("Script/GameConfig/ExchangeItem")


RewardInfo = {
	item_id = 0,
	item_level = 0,
	item_num = 0,
}

function RewardInfo:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

Reward = {
	rewards = {},
	map_id = 0,
	add_coin = 0,
	add_exp = 0,
	add_exploit = 0,
}

function Reward:New()
	local oneTable = {}
	setmetatable(oneTable,self);
	self.__index = self;
	return oneTable
end

ElfExploreDataCenter = {
	MapId = 0,
	MapGetType = 0,
	ExploreLeftTimes = 0,
	ExploreReward = Reward:New(),
}

function ElfExploreDataCenter:SetMapId(mapId)
	self.MapId = mapId
end

function ElfExploreDataCenter:SetMapGetType(type)
	self.MapGetType = type
end

function ElfExploreDataCenter:ResetRewardInfo()
	self.ExploreReward = nil
	self.ExploreReward = Reward:New()
	self.ExploreReward.rewards = {}
end

function ElfExploreDataCenter:SetBasicRewardInfo(map_id,add_coin,add_exp,add_exploit)
	self:ResetRewardInfo()
	
	self.ExploreReward.map_id = map_id
	self.ExploreReward.add_coin = add_coin
	self.ExploreReward.add_exp = add_exp
	self.ExploreReward.add_exploit = add_exploit
end

function ElfExploreDataCenter:SetOtherRewardInfo(item_id,item_level,item_num)
	rewardInfo = RewardInfo:New()
	rewardInfo.item_id = item_id
	rewardInfo.item_level = item_level
	rewardInfo.item_num = item_num

	InsertOneItemToTable(self.ExploreReward.rewards,rewardInfo)
end

function ElfExploreDataCenter:ExploreRewardCommit()
	local rewardInfo = "";
	local coinContent = Localization['S_Coin_content'].SimpleChinese
	local expContent = Localization['S_Exp_content'].SimpleChinese
	local exploitContent = Localization['S_Exploit_content'].SimpleChinese

	if 0 ~= self.ExploreReward.add_coin then
		rewardInfo = rewardInfo .. coinContent .. " *" .. self.ExploreReward.add_coin
	end

	if 0 ~= self.ExploreReward.add_exp then
		rewardInfo = rewardInfo .. expContent .. " *" .. self.ExploreReward.add_exp
	end

	if 0 ~= self.ExploreReward.add_exploit then
		rewardInfo = rewardInfo .. exploitContent .. " *" .. self.ExploreReward.add_exploit
	end

	for index,value in pairs(self.ExploreReward.rewards) do
		local item_id = value.item_id
		local item_level = value.item_level
		local item_num = value.item_num

		--Note: 判断是否精灵
		if TellIsElfId(item_id) then
			rewardInfo = rewardInfo .. "ELF " .. " " .. item_id
			--Note:更新精灵，新得到的精灵
			InsertNewItem(G_LuaElfDataTable,item_id,2)			
			ElfExploerLayer:ShowLeftScrollList()
		else
			local name = Items[item_id].Name
			rewardInfo = rewardInfo .. name .. " *" .. item_num
		end
	end

	--Note:刷新页面
	ElfExploerLayer:RewardCommit(rewardInfo)
	self:SetMapId(self.ExploreReward.map_id)
	ElfExploerLayer:FreshRightPanel()
end

function ElfExploreDataCenter:GetValidElf()
	local resultTable = GetExploreElfTabel()
	return resultTable
end

function ElfExploreDataCenter:GetOneMapName(mapId)
	local flag = Explore[mapId].UI_Name;
	return Localization[flag].SimpleChinese
end

function ElfExploreDataCenter:GetOneMapIconName(mapId)
	local iconName = Explore[mapId].LookFace;
	iconName = "UI/" .. iconName .. ".png"
	return iconName
end

function ElfExploreDataCenter:GetOneMapIconEventContent(mapId)
	local flag = Explore[mapId].Event;
	return Localization[flag].SimpleChinese
end

function ElfExploreDataCenter:GetOneMapIconChoiceContent_1(mapId)
	local flag = Explore[mapId].Option1_Text;
	return Localization[flag].SimpleChinese
end

function ElfExploreDataCenter:GetOneMapIconChoiceContent_2(mapId)
	local flag = Explore[mapId].Option2_Text;
	return Localization[flag].SimpleChinese
end

function ElfExploreDataCenter:PushExploreLeftNums(leftNums)	
	self.ExploreLeftTimes = leftNums
	--print("self.ExploreLeftTimes = leftNums " .. leftNums)
	ElfExploerLayer:ShowExploreNumsInfoPanel()
end

function ElfExploreDataCenter:GetExchangeItemIdByElfId(elfId)
	for index,value in pairs(ExchangeItems) do
		local nType = tonumber(value.Type)
		if 5 == nType then
			local ToItems = value.ToItems
			local idStr = ToItems
			local _start,_end = string.find(idStr,"/")
			if nil ~= _start and 1 ~= _start then
				local tmpIdStr = string.sub(idStr,1,_start-1)
				local id = tonumber(tmpIdStr)
				if TellIsElfId(id) and elfId == id then
					return index
				end
			end
		end
	end
	return nil
end

function ElfExploreDataCenter:SendExchangeElfMessagge(elfId)
	local exchangeItemId = self:GetExchangeItemIdByElfId(elfId)
	if nil ~= exchangeItemId then
		ElfExploreManger:Get():SendExchangeElfReq(exchangeItemId)
	end	
end

function ElfExploreDataCenter:GetExchangeElfUnLockLevel(elfId)
	local itemId = self:GetExchangeItemIdByElfId(elfId)
	print("itemId " .. itemId)
	if nil ~= itemId then
		return tonumber(ExchangeItems[itemId].Level)
	end
	return nil
end

function ElfExploreDataCenter:GetMapElfIdAndEventId(id)
	local item = Explore[id]
	if nil == item then
		return nil,nil
	end
	local uiIdStr = item.UI_ID

	print("ElfExploreDataCenter:GetMapElfIdAndEventId " .. id)
	print(uiIdStr)

	local _start,_end = string.find(uiIdStr,"/")
	if nil ~= _start and 1 ~= _start then
		local tmpIdStr = string.sub(uiIdStr,1,_start-1)
		local event_id = tonumber(tmpIdStr)

		print("event_id " .. event_id)

		tmpIdStr = string.sub(uiIdStr,_end+1,string.len(uiIdStr))
		local elf_id = tonumber(tmpIdStr)
		print("elf_id " .. elf_id)
		return event_id,elf_id
	end
	return nil,nil
end

------------------------------------------------------------------
function PushExploreMapIdToData(type,mapId)
	if mapId == ElfExploreDataCenter.MapId then
		return 
	end
	ElfExploreDataCenter:SetMapGetType(type)
	ElfExploreDataCenter:SetMapId(mapId)
	if 2 == type then
		ElfExploerLayer:FreshRightPanel()
	end	
end

function PushExploreBasicRewardInfo(map_id,add_coin,add_exp,add_exploit)
	ElfExploreDataCenter:SetBasicRewardInfo(map_id,add_coin,add_exp,add_exploit)
end

function PushExploreOtherRewardInfo(item_id,item_level,item_num)
	ElfExploreDataCenter:SetOtherRewardInfo(item_id,item_level,item_num)
end

function ExploreRewardCommit()
	ElfExploreDataCenter:ExploreRewardCommit()
end

function PushExploreLeftNums(leftNums)
	ElfExploreDataCenter:PushExploreLeftNums(leftNums)
end

function PushExchangeElfRet(elfId)
	print("function PushExchangeElfRet(elfId) " .. elfId)
	--Note:兑换精灵
	InsertNewItem(G_LuaElfDataTable,elfId,2)
	ExchangeElfLayer:FreshLayer()
end